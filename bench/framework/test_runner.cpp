#include "test_runner.hpp"
#include <bench/core/clock.hpp>
#include <bench/core/probe.hpp>
#include <bench/core/test_instance.hpp>
#include <bench/framework/counter_probe.hpp>
#if BENCHMARK_SNMP
#include <bench/framework/snmp_probe.hpp>
#endif
#include <bench/framework/test_thread.hpp>
#include <bench/log/logger.hpp>
#include <utils/for_each.hpp>
#include <utils/memory.hpp>
#include <algorithm>
#include <chrono>

namespace bench
{
namespace framework
{

static duration compute_sampling_period(duration test_duration)
{
    const duration longest = std::chrono::minutes(1);
    const duration shortest = std::chrono::milliseconds(100);
    return std::min(std::max(shortest, test_duration / 100), longest);
}

void test_runner::create_threads()
{
    _synchronizer.reset(_test->config.thread_count);

    for (auto i = std::size_t(0); i < _test->config.thread_count; i++)
    {
        _threads.emplace_back(new test_thread(*_test, _synchronizer));
    }

    _synchronizer.wait_workers();
}

void test_runner::destroy_threads()
{
    _synchronizer.send_order(&test_thread::terminate);
    _threads.clear();
}

void test_runner::create_probes()
{
    _counter_probe.reset(new counter_probe(_threads));
    _custom_probes = create_test_probes(*_test);

    // insert the iteration probe first  so it's not affected by the delay in other probes
    _all_probes.push_back(_counter_probe.get());
#if BENCHMARK_SNMP
    _all_probes.push_back(_snmp_probe.get());
#endif
    for (auto & probe : _custom_probes)
        _all_probes.push_back(probe.get());
}

void test_runner::destroy_probes()
{
    _counter_probe.reset();
    _custom_probes.clear();
    _all_probes.clear();
}

bool test_runner::step1_setup()
{
    try
    {
        auto start_time = clock::now();
        _logger.setup_started(*_test);

        _synchronizer.send_order(&test_thread::setup);
        setup_probes();
        _synchronizer.wait_workers();
        _test->setup_duration = clock::now() - start_time;

        _synchronizer.rethrow();

        _logger.setup_succeeded(*_test);
        return true;
    }
    catch (std::exception & e)
    {
        _test->errors.push_back({e.what(), "Error happened during test setup"});
        _logger.setup_failed(*_test);
        return false;
    }
}

void test_runner::step2_loop()
{
    duration sampling_period = compute_sampling_period(_test->config.duration);

    try
    {
        _test->start_time = clock::now();
        _logger.loop_started(*_test);

        _synchronizer.send_order(&test_thread::test);
        sample_probes();
        while (!_synchronizer.wait_workers_for(sampling_period))
        {
            sample_probes();
            log_progress();
        }
        sample_probes();
        _test->test_duration = clock::now() - _test->start_time;

        _synchronizer.rethrow();

        _logger.loop_succeeded(*_test);
    }
    catch (std::exception & e)
    {
        _test->errors.push_back({e.what(), "Error happened during test execution"});
        _logger.loop_failed(*_test);
    }
}

void test_runner::step3_cleanup()
{
    try
    {
        auto start_time = clock::now();
        _logger.cleanup_started(*_test);

        _synchronizer.send_order(&test_thread::cleanup);
        cleanup_probes();
        _synchronizer.wait_workers();

        _test->cleanup_duration = clock::now() - start_time;

        _synchronizer.rethrow();

        _logger.cleanup_succeeded(*_test);
    }
    catch (std::exception & e)
    {
        _test->errors.push_back({e.what(), "Error happened during test cleanup"});
        _logger.cleanup_failed(*_test);
    }
}

void test_runner::init_test_results()
{
    for (auto * probe : _all_probes)
    {
        for (auto & m : probe->measurements())
        {
            _test->result[m.first].name = m.second.name;
            _test->result[m.first].unit = m.second.unit;
        }
    }
}

void test_runner::setup_probes()
{
    for (auto * probe : _all_probes)
    {
        try
        {
            probe->setup();
        }
        catch (...)
        {
            _synchronizer.report_exception(std::current_exception());
        }
    }
}

void test_runner::cleanup_probes()
{
    for (auto * probe : _all_probes)
    {
        try
        {
            probe->cleanup();
        }
        catch (...)
        {
            _synchronizer.report_exception(std::current_exception());
        }
    }
}

void test_runner::sample_probes()
{
    time_point now = clock::now();

    for (auto * probe : _all_probes)
    {
        try
        {
            probe->take_sample();

            for (auto & m : probe->measurements())
            {
                _test->result[m.first].samples.push_back({now, m.second.value});
            }
        }
        catch (...)
        {
            _synchronizer.report_exception(std::current_exception());
        }
    }
}

#if BENCHMARK_SNMP
void test_runner::set_snmp_peers(const std::vector<std::string> & snmp_peers)
{
    _snmp_probe.reset(new snmp_probe(snmp_peers));
}
#endif

} // namespace framework
} // namespace bench
