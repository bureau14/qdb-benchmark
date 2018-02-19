#pragma once

#include <bench/core/computations.hpp>
#include <bench/log/logger.hpp>
#include <bench/log/unit.hpp>
#include <fmt/format.h>
#include <chrono>

namespace bench
{
namespace log
{

class console_logger : public logger
{
public:
    void fatal_error(const std::string & message) override
    {
        fmt::print("ERROR: {}\n", message);
    }

    void pause(std::chrono::seconds duration) override
    {
        fmt::print("\nWait {}\n\n", duration_to_string(duration));
    }

    void schedule(const std::vector<test_instance> & tests) override
    {
        fmt::print("The following test will be performed:");
        for (auto i = 0u; i < tests.size(); i++)
        {
            auto & test = tests[i];
            fmt::print("\n  {}. {}", i + 1, test.tclass.name);
            fmt::print(", threads={}", test.config.thread_count);
            if (test.tclass.size_dependent) fmt::print(", size={}", test.config.content_size);
        }
        fmt::print("\n\n");

        _test_count = tests.size();
        _test_num = 0;
    }

    void summary(size_t success_count, size_t total_test_count) override
    {
        fmt::print("Succeeded {} out of {} {}.\n\n", success_count, total_test_count,
                   (total_test_count != 1) ? "tests" : "test");
    }

    // Whole test

    void test_started(const test_instance & test) override
    {
        _test_num++;

        _header = fmt::format("{}/{}  ", _test_num, _test_count);

        fmt::print("{}Now running \"{}\"", _header, test.tclass.name);
        if (test.config.thread_count > 1)
            fmt::print(", on {} threads", test.config.thread_count);
        else
            fmt::print(", on a single thread");

        if (test.tclass.size_dependent)
        {
            fmt::print(", with a payload of {}", unit::byte(static_cast<double>(test.config.content_size)));
        }
    }

    void test_finished(const test_instance &) override {}

    // Test setup

    void setup_started(const test_instance & test) override
    {
        fmt::print("\n{}Setting up test... \n", _header);
    }

    void setup_failed(const test_instance & test) override
    {
        fmt::print("{}Setting up test... FAILED\n", _header);
        fmt::print("{}Error = {}\n", _header, test.errors.back().message);
        fmt::print("{}Details = {}\n", _header, test.errors.back().details);
    }

    void setup_succeeded(const test_instance & test) override
    {
        fmt::print("{}Setting up test... OK ({})\n", _header, duration_to_string(test.setup_duration));
    }

    // Test loop

    void loop_started(const test_instance & test) override
    {
        fmt::print("{}Executing test... ", _header);
    }

    void loop_progress(const test_instance & test) override
    {
        auto elapsed = clock::now() - test.start_time;
        auto iterations = compute_iteration_count(test);
        fmt::print("\r{}Executing test... {}iterations ({})    ", _header, unit::none(iterations),
                   duration_to_string(elapsed));
    }

    void loop_failed(const test_instance & test) override
    {
        fmt::print("\n{}Executing test... FAILED\n", _header);
        fmt::print("{}Error = {}\n", _header, test.errors.back().message);
        fmt::print("{}Details = {}\n", _header, test.errors.back().details);
        fmt::print("{}Total iterations = {}\n", _header, unit::none(compute_iteration_count(test)));
    }

    void loop_succeeded(const test_instance & test) override
    {
        fmt::print("\n{}Executing test... OK ({})\n", _header, duration_to_string(test.test_duration));

        if (test.tclass.size_dependent)
        {
            fmt::print("{}Average throughput = {}\n", _header, unit::byte_per_second(compute_average_throughput(test)));
        }

        fmt::print("{}Average frequency = {}\n", _header, unit::hertz(compute_average_frequency(test)));
        fmt::print("{}Total iterations = {}\n", _header, unit::none(compute_iteration_count(test)));
    }

    // Test cleanup

    void cleanup_started(const test_instance & test) override
    {
        fmt::print("{}Cleaning up... \n", _header);
    }

    void cleanup_failed(const test_instance & test) override
    {
        fmt::print("{}Cleaning up... FAILED\n", _header);
        fmt::print("{}Error = {}\n", _header, test.errors.back().message);
        fmt::print("{}Details = {}\n", _header, test.errors.back().details);
    }

    void cleanup_succeeded(const test_instance & test) override
    {
        fmt::print("{}Cleaning up... OK ({})\n", _header, duration_to_string(test.cleanup_duration));
    }

private:
    size_t _test_count;
    size_t _test_num;
    std::string _header;

    std::string duration_to_string(bench::duration duration)
    {
        return unit::milliseconds(
            static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(duration).count()));
    }
};

} // namespace log
} // namespace bench
