#pragma once

#include <bench/log/logger.hpp>

namespace bench
{
namespace log
{
class teamcity_logger : public logger
{
public:
    void fatal_error(const std::string & message) override;
    void pause(std::chrono::duration<int>) override;
    void schedule(const std::vector<test_instance> &) override;
    void test_finished(int num, int total, const test_instance &) override;
    void test_started(int num, int total, const test_instance &) override;
};
}
}