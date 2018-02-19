#include <utils/teamcity.hpp>

#include <fmt/format.h>
#include <regex>

std::string escape(const std::string & input)
{
    static std::regex forbidden_chars("\\]|\\[|'|\\|");
    return std::regex_replace(input, forbidden_chars, "|$&");
}

void utils::teamcity::block_opened(const std::string & name)
{
    fmt::print("##teamcity[blockOpened name='{}']\n", escape(name));
}

void utils::teamcity::block_closed(const std::string & name)
{
    fmt::print("##teamcity[blockClosed name='{}']\n", escape(name));
}

void utils::teamcity::build_problem(const std::string & description)
{
    fmt::print("##teamcity[buildProblem description='{}']\n", escape(description));
}

void utils::teamcity::build_statistic(const std::string & name, double value)
{
    fmt::print("##teamcity[buildStatisticValue key='{}' value='{}']\n", escape(name), value);
}

void utils::teamcity::message(const std::string & text)
{
    fmt::print("##teamcity[message text='{}']\n", escape(text));
}

void utils::teamcity::message(const std::string & text, const std::string & error_details)
{
    fmt::print("##teamcity[message text='{}' errorDetails='{}' status='ERROR']\n", escape(text), escape(error_details));
}

void utils::teamcity::progressMessage(const std::string & message)
{
    fmt::print("##teamcity[progressMessage '{}']\n", escape(message));
}

void utils::teamcity::test_failed(const std::string & name, const std::string & message)
{
    fmt::print("##teamcity[testFailed name='{}' message='{}']\n", escape(name), escape(message));
}

void utils::teamcity::test_failed(const std::string & name, const std::string & message, const std::string & details)
{
    fmt::print("##teamcity[testFailed name='{}' message='{}' details='{}']\n", escape(name), escape(message),
               escape(details));
}

void utils::teamcity::test_finished(const std::string & name)
{
    fmt::print("##teamcity[testFinished name='{}']\n", escape(name));
}

void utils::teamcity::test_started(const std::string & name)
{
    fmt::print("##teamcity[testStarted name='{}' captureStandardOutput='true']\n", escape(name));
}
