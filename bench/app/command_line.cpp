#include "command_line.hpp"
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
    case 'k':
        return {size_t(std::stoi(s) << 10)};

    case 'M':
    case 'm':
        return {size_t(std::stoi(s) << 20)};

    case 'G':
    case 'g':
        return {size_t(std::stoi(s) << 30)};

    default:
        return {size_t(std::stoi(s))};
    }
}

static void get_tests_by_name(const bench::test_class_collection & tests,
                              const std::string & name,
                              bench::test_class_collection & chosen_tests)
{
    // All tests chosen.
    if (name == "*") return;

    if (name.empty()) throw std::runtime_error("Invalid test name: " + name);

    const bool should_remove = (name.front() == '-');
    if (should_remove && (name.size() == 1)) throw std::runtime_error("Invalid test name: " + name);

    auto begin = name.begin();
    if (should_remove) ++begin;
    auto rbegin = name.rbegin();

    const bool prefix = (*begin == '*');
    const bool suffix = (*rbegin == '*');
    const bool prefix_suffix = prefix && suffix;

    if (prefix) ++begin;
    auto end = name.end();
    if (suffix) --end;

    const auto name_part = std::string(begin, end);
    size_t chosen_count = 0;

    for (auto test : tests)
    {
        bool chosen = false;
        if (test->name == name)
        {
            chosen = true;
        }
        else if (prefix_suffix)
        {
            assert(name.size() >= 2);
            if (test->name.find(name_part) != std::string::npos) chosen = true;
        }
        else if (prefix)
        {
            if (test->name.size() >= name_part.size())
            {
                auto test_name_part = test->name.substr(test->name.size() - name_part.size());
                if (name_part == test_name_part) chosen = true;
            }
        }
        else if (suffix)
        {
            auto test_name_part = test->name.substr(0, name_part.size());
            if (name_part == test_name_part) chosen = true;
        }

        if (chosen)
        {
            ++chosen_count;
            if (should_remove)
            {
                for (auto it = std::find(chosen_tests.begin(), chosen_tests.end(), test); it != chosen_tests.end();
                     it = std::find(it, chosen_tests.end(), test))
                {
                    it = chosen_tests.erase(it);
                }
            }
            else
            {
                chosen_tests.push_back(test);
            }
        }
    }

    if (chosen_count == 0u) throw std::runtime_error("invalid test name: " + name);
}

void bench::app::command_line::parse(int argc, const char ** argv)
{
    utils::command_line parser(argc, argv);

    std::cout << "quasardb cluster benchmarking tool\n" << std::endl;

    bool version = parser.get_flag("-v", "--version", "Display program version and exit");
    bool help = parser.get_flag("-h", "--help", "Display program help and exit");
    bool dry_run = parser.get_flag("-n", "--dry-run", "Display tests to be executed and exit");
    _settings.cluster_uri = parser.get_string("-c", "--cluster", "Set cluster URI", "qdb://127.0.0.1:2836");
    _settings.pause =
        std::chrono::seconds(parser.get_integer("-p", "--pause", "Set the delay between each test, in seconds", "1"));
    _settings.duration =
        std::chrono::seconds(parser.get_integer("-d", "--duration", "Set the duration of each test, in seconds", "4"));
    _settings.content_sizes =
        parser.get_values<std::size_t>("", "--sizes", "Set contents sizes", "1,1K,1M", parse_size);
    _settings.element_numbers = parser.get_values<std::size_t>(
        "", "--elements-number", "Set number of element for each test, ex: number of elements per batch", "1",
        parse_size);
    _settings.no_cleanup = parser.get_flag("", "--no-cleanup", "Disable test cleanup");
#if BENCHMARK_SNMP
    _settings.snmp_peers = parser.get_strings("", "--snmp", "Set SNMP peer names", "");
#endif
    _settings.thread_counts = parser.get_integers("", "--threads", "Set number of threads", "1,2,4");
    parser.get_updatable_values<const test_class *>(
        "", "--tests", "Select the tests to run", "*", _settings.tests,
        [this](const std::string & name, decltype(_settings.tests) & tests) {
            get_tests_by_name(_test_pool, name, tests);
        });
    parser.check_unknown();

    if (_settings.tests.empty()) _settings.tests = _test_pool; // enable all tests by default

    if (help)
    {
        std::cout << "Available command line options:" << std::endl;
        std::cout << parser.help() << std::endl;

        std::cout << "Available tests:" << std::endl;
        for (const auto & test_class : _test_pool)
        {
            std::cout << "  " << std::left << std::setw(32) << test_class->name << " " << test_class->description
                      << std::endl;
        }

        std::exit(0);
    }

    if (version)
    {
        std::exit(0);
    }

    if (dry_run)
    {
        std::cout << "Chosen tests:" << std::endl;
        for (const auto & test_class : _settings.tests)
        {
            std::cout << "  " << std::left << std::setw(32) << test_class->name << " " << test_class->description
                      << std::endl;
        }

        std::exit(0);
    }
}
