#include "program.hpp"
#include <bench/app/options.hpp>
#include <bench/framework/test_runner.hpp>
#include <thread>

void bench::app::program::parse_command_line(int argc, const char ** argv)
{
    options opts(_test_pool);
    opts.parse(argc, argv);
    opts.fill_settings(_settings);
}

void bench::app::program::prepare_schedule()
{
    test_config config;
    config.cluster_uri = _settings.cluster_uri;
    config.duration = _settings.duration;
    config.no_cleanup = _settings.no_cleanup;

    for (auto & test_class : _settings.tests)
    {
        for (auto thread_count : _settings.thread_counts)
        {
            config.thread_count = thread_count;
            if (test_class->size_dependent)
            {
                for (auto content_size : _settings.content_sizes)
                {
                    config.content_size = content_size;
                    _schedule.emplace_back(create_test_instance(*test_class, config));
                }
            }
            else
            {
                config.content_size = 0;
                _schedule.emplace_back(create_test_instance(*test_class, config));
            }
        }
    }

    _logger.schedule(_schedule);
}

void bench::app::program::run_scheduled_tests()
{
    bench::framework::test_runner runner(_logger);
#if BENCHMARK_SNMP
    runner.set_snmp_peers(_settings.snmp_peers);
#endif

    size_t success_count = 0;
    for (auto & test : _schedule)
    {
        runner.run(test);
        _report.add_test(test);
        if (test.errors.empty()) ++success_count;

        _logger.pause(_settings.pause);
        std::this_thread::sleep_for(_settings.pause);
    }

    const auto test_count = _schedule.size();
    _logger.summary(success_count, test_count);
}
