#pragma once

#include <bench/log/logger.hpp>
#include <utils/teamcity.hpp>

namespace bench
{
namespace log
{

class teamcity_logger : public logger
{
public:
    void fatal_error(const std::string & message) override
    {
        utils::teamcity::build_problem(message);
    }

    void pause(std::chrono::seconds duration) override
    {
        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
        utils::teamcity::message("Wait " + std::to_string(seconds) + (seconds > 1 ? " seconds" : " second"));
    }

    void schedule(const std::vector<test_instance> &) override {}

    void summary(size_t /*success_count*/, size_t /*total_test_count*/) override {}

    // Whole test

    void test_started(const test_instance & test) override
    {
        utils::teamcity::test_started(make_test_name(test));
    }

    void test_finished(const test_instance & test) override
    {
        if (test.errors.size() > 0)
        {
            utils::teamcity::test_failed(make_test_name(test), test.errors.front().message,
                                         test.errors.front().details);
        }
        utils::teamcity::test_finished(make_test_name(test));
    }

    // Test setup

    void setup_started(const test_instance & test) override
    {
        utils::teamcity::block_opened("setup");
    }

    void setup_failed(const test_instance & test) override
    {
        utils::teamcity::message(test.errors.back().message, test.errors.back().details);
        utils::teamcity::block_closed("setup");
    }

    void setup_succeeded(const test_instance & test) override
    {
        utils::teamcity::block_closed("setup");
    }

    // Test loop

    void loop_started(const test_instance & test) override
    {
        utils::teamcity::block_opened("loop");
    }

    void loop_progress(const test_instance & test) override
    {
        utils::teamcity::progressMessage(std::to_string(compute_iteration_count(test)) + " iterations");
    }

    void loop_failed(const test_instance & test) override
    {
        utils::teamcity::message(test.errors.back().message, test.errors.back().details);
        utils::teamcity::block_closed("loop");
    }

    void loop_succeeded(const test_instance & test) override
    {
        std::string test_name = make_test_name(test);

        if (test.config.content_size)
        {
            utils::teamcity::build_statistic(test_name + ".throughput.average", compute_average_throughput(test));
        }
        utils::teamcity::build_statistic(test_name + ".frequency.average", compute_average_frequency(test));

        utils::teamcity::block_closed("loop");
    }

    // Test cleanup

    void cleanup_started(const test_instance & test) override
    {
        utils::teamcity::block_opened("cleanup");
    }

    void cleanup_failed(const test_instance & test) override
    {
        utils::teamcity::message(test.errors.back().message, test.errors.back().details);
        utils::teamcity::block_closed("cleanup");
    }

    void cleanup_succeeded(const test_instance & test) override
    {
        utils::teamcity::block_closed("cleanup");
    }

private:
    static std::string make_size_string(size_t size)
    {
        if (size < 1024) return std::to_string(size) + "_B";
        size /= 1024;
        if (size < 1024) return std::to_string(size) + "_kB";
        size /= 1024;
        if (size < 1024) return std::to_string(size) + "_MB";
        size /= 1024;
        return std::to_string(size) + "_GB";
    }

    static std::string make_thread_string(int thread_count)
    {
        if (thread_count > 1)
            return std::to_string(thread_count) + "_threads";
        else
            return "1_thread";
    }

    static std::string make_test_name(const bench::test_instance & test)
    {
        std::string s;
        s = test.tclass.name + "." + make_thread_string(static_cast<int>(test.config.thread_count));
        if (test.tclass.size_dependent) s += "." + make_size_string(test.config.content_size);
        return s;
    }
};

} // namespace log
} // namespace bench
