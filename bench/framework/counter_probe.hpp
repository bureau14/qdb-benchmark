#pragma once

#include <bench/core/probe.hpp>
#include <bench/framework/test_thread.hpp>

namespace bench
{
namespace framework
{
class counter_probe : public probe
{
public:
    counter_probe(const test_thread_collection & threads) : _threads(threads)
    {
        define_measurement("test.iterations", "Test Loop Total Iterations", unit::none, threads.size());
        define_measurement("test.successes", "Test Loop Successes", unit::none, threads.size());
        define_measurement("test.failures", "Test Loop Failures", unit::none, threads.size());
        define_measurement("test.avg_duration", "Test Loop Average Duration", unit::nanosecond, threads.size());
    }

    void setup() override
    {
    }

    void take_sample() override
    {
        for (std::size_t i = 0; i < _threads.size(); i++)
        {
            set_measured_value("test.iterations", i, _threads[i]->iterations());
            set_measured_value("test.successes", i, _threads[i]->successes());
            set_measured_value("test.failures", i, _threads[i]->failures());
            set_measured_value("test.avg_duration", i, _threads[i]->avg_duration());
        }
    }

    void cleanup() override
    {
    }

private:
    const test_thread_collection & _threads;
};
}
}
