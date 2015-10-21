#include <bench/app/command_line.hpp>
#include <utils/command_line.hpp>

#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>

static std::vector<size_t> parse_size(const std::string & s)
{
    switch (s.back())
    {
    case 'K':
    case 'k': return {size_t(std::stoi(s) << 10)};

    case 'M':
    case 'm': return {size_t(std::stoi(s) << 20)};

    case 'G':
    case 'g': return {size_t(std::stoi(s) << 30)};

    default: return {size_t(std::stoi(s))};
    }
}

static bench::test_class_collection get_tests_by_name(const bench::test_class_collection & tests,
                                                      const ::std::string & name)
{
    // All tests chosen.
    if (name == "*") return tests;

    bench::test_class_collection chosen_tests;
    if (!name.empty())
    {
        for (auto test : tests)
        {
            bool chosen = false;
            if (test->name == name)
            {
                chosen = true;
            }
            else if ((name.front() == '*') && (name.back() == '*'))
            {
                assert(name.size() >= 2);
                auto name_part = name.substr(1, name.size() - 2);
                if (test->name.find(name_part) != ::std::string::npos) chosen = true;
            }
            else if (name.front() == '*')
            {
                auto name_part = name.substr(1);
                if (test->name.size() >= name_part.size())
                {
                    auto test_name_part = test->name.substr(test->name.size() - name_part.size());
                    if (name_part == test_name_part) chosen = true;
                }
            }
            else if (name.back() == '*')
            {
                auto name_part = name.substr(0, name.size() - 1);
                auto test_name_part = test->name.substr(0, name.size() - 1);
                if (name_part == test_name_part) chosen = true;
            }

            if (chosen) chosen_tests.push_back(test);
        }
    }

    if (chosen_tests.empty()) throw std::runtime_error("Invalid test name: " + name);
    return chosen_tests;
}

void bench::app::command_line::parse(int argc, const char ** argv)
{
    utils::command_line parser(argc, argv);

    std::cout << "quasardb cluster benchmarking tool" << std::endl;

    bool version = parser.get_flag("-v", "--version", "Display program version and exists");
    bool help = parser.get_flag("-h", "--help", "Display program help and exists");
    _settings.cluster_uri =
        parser.get_string("-c", "--cluster", "Set cluster URI", "qdb://127.0.0.1:2836");
    _settings.pause = std::chrono::seconds(
        parser.get_integer("-p", "--pause", "Set the delay between each test, in seconds", "1"));
    _settings.duration = std::chrono::seconds(
        parser.get_integer("-d", "--duration", "Set the duration of each test, in seconds", "4"));
    _settings.thread_counts =
        parser.get_integers("", "--threads", "Set number of threads", "1,2,4");
    _settings.content_sizes =
        parser.get_values<std::size_t>("", "--sizes", "Set contents sizes", "1,1K,1M", parse_size);
    _settings.tests = parser.get_values<const test_class *>(
        "", "--tests", "Select the tests to run (default=all)", "",
        [this](const std::string & name) { return get_tests_by_name(_test_pool, name); });
    parser.check_unknown();

    if (_settings.tests.empty()) _settings.tests = _test_pool; // all test by default

    if (help)
    {
        std::cout << "Available command line options:" << std::endl;
        std::cout << parser.help();

        std::cout << "Available tests:" << std::endl;
        for (auto & test_class : _test_pool)
        {
            std::cout << "  " << std::left << std::setw(32) << test_class->name << " "
                      << test_class->description << std::endl;
        }

        std::exit(0);
    }

    if (version)
    {
        std::exit(0);
    }
}
