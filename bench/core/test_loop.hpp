#pragma once

#include <bench/core/clock.hpp>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

namespace bench
{

class test_loop
{
public:
    test_loop() = default;

    virtual ~test_loop()
    {
    }

    virtual void setup() = 0;
    virtual void run() = 0;
    virtual void cleanup() = 0;

    std::uint32_t iterations() const
    {
        return _iterations;
    }

    std::uint32_t successes() const
    {
        return _iterations - _failures;
    }

    std::uint32_t failures() const
    {
        return _failures;
    }

    std::uint64_t avg_duration()
    {
        if (_durations.size() == 0)
        {
            return 0.0;
        }
        auto sum =
            std::accumulate(std::begin(_durations), std::end(_durations), std::uint64_t{0}, std::plus<std::uint64_t>());
        auto average = sum / _durations.size();
        _durations.clear();
        return average;
    }

protected:
    void add_iteration()
    {
        _iterations++;
    }
    void add_failure()
    {
        _failures++;
    }
    void add_duration(std::uint32_t duration)
    {
        _durations.emplace_back(duration);
    }

private:
    std::atomic_uint32_t _iterations{0};
    std::atomic_uint32_t _failures{0};
    std::vector<std::uint64_t> _durations{};
};

} // namespace bench
