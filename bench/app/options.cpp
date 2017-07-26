#include "options.hpp"
#include <iomanip>
#include <iostream>
#include <string>
#include <type_traits>

namespace po = boost::program_options;
namespace bench
{
namespace app
{
struct content_sizes
{
    std::vector<std::size_t> values;
};
struct thread_counts
{
    std::vector<std::size_t> values;
};
} // namespace app
} // namespace bench

void bench::app::options::init()
{
    // clang-format off
    _desc.add_options()("version,v", "Display program version and exit")
        ("help,h", "Display program help and exit")
        ("show-tests", "Display tests and exit")
        ("dry-run,n", "Display tests to be executed and exit")
        ("no-cleanup", "Disable test cleanup")
        ("cluster,c", po::value<std::string>()->default_value("qdb://127.0.0.1:2836"), "Set cluster URI")
        ("pause,p", po::value<std::size_t>()->default_value(1),
             "Set the delay between each test, in seconds")
        ("duration,d", po::value<std::size_t>()->default_value(4),
             "Set the duration of each test, in seconds")
        ("sizes", po::value<content_sizes>()->default_value({{1, 1 << 10, 1 << 20}}, "1,1k,1M"),
             "Set contents sizes, in bytes")
        ("threads", po::value<thread_counts>()->default_value({{1, 2, 4}}, "1,2,4"),
             "Set contents sizes, in bytes")
        ("user-credentials-file", po::value<std::string>()->default_value(""),
            "Path to the file containing the user's name and secret key used for connection")
        ("cluster-public-file", po::value<std::string>()->default_value(""),
            "Path to the file containing the public key of the cluster to connect to ")
        ("tests", po::value<std::string>(), "Select the tests to run");
// clang-format on

#if BENCHMARK_SNMP
    _desc.add_options()("snmp", po::value<std::vector<std::string>>(), "Set SNMP peer names");
#endif
}

void bench::app::options::parse(int argc, const char ** argv)
{
    try
    {
        po::store(po::parse_command_line(argc, argv, _desc), _vmap);

        if (_vmap.count("help"))
        {
            std::cout << "Available command line options:" << std::endl << _desc << std::endl;
            std::exit(0);
        }
        if (_vmap.count("version"))
        {
            std::exit(0);
        }
        if (_vmap.count("show-tests"))
        {
            std::cout << "Available tests:" << std::endl;
            for (const auto & test_class : _test_pool)
            {
                std::cout << "  " << std::left << std::setw(32) << test_class->name << " " << test_class->description
                          << std::endl;
            }

            std::exit(0);
        }
        if (_vmap.count("dry-run"))
        {
            std::cout << "Tests to execute:" << std::endl;
            auto tests = parse_tests();
            for (const auto & test_class : tests)
            {
                std::cout << "  " << std::left << std::setw(32) << test_class->name << " " << test_class->description
                          << std::endl;
            }

            std::exit(0);
        }
    }
    catch (po::error & e)
    {
        std::cerr << e.what() << "\n";
        std::exit(-1);
    }
}

void bench::app::options::fill_settings(settings & settings)
{
    settings.cluster_uri = get_value<std::string>("cluster");
    settings.pause = std::chrono::seconds(get_value<std::size_t>("pause"));
    settings.duration = std::chrono::seconds(get_value<std::size_t>("duration"));
    settings.content_sizes = get_value<content_sizes>("sizes").values;
    if (_vmap.count("no-cleanup"))
    {
        settings.no_cleanup = true;
    }
#if BENCHMARK_SNMP
    if (_vmap.count("snmp"))
    {
        settings.snmp_peers = get_value<std::vector<std::string>>("snmp");
    }
#endif
    settings.thread_counts = get_value<thread_counts>("threads").values;
    settings.user_credentials_file = get_value<std::string>("user-credentials-file");
    settings.cluster_public_file = get_value<std::string>("cluster-public-file");
    settings.tests = parse_tests();
}

bench::test_class_collection bench::app::options::parse_tests()
{
    if (!_vmap.count("tests"))
    {
        return _test_pool;
    }

    bench::test_class_collection chosen_tests;
    auto name = get_value<std::string>("tests");

    // All tests chosen.
    if (name == "*")
    {
        return _test_pool;
    }

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

    for (auto test : _test_pool)
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
    return chosen_tests;
}
namespace bench
{
namespace app
{

std::size_t parse_size(const std::string & str_size)
{
    switch (str_size.back())
    {
    case 'K':
    case 'k':
        return std::size_t(std::stoi(str_size) << 10);

    case 'M':
    case 'm':
        return std::size_t(std::stoi(str_size) << 20);

    case 'G':
    case 'g':
        return std::size_t(std::stoi(str_size) << 30);

    default:
        return std::size_t(std::stoi(str_size));
    }
}

void tokenize(const std::string & str, std::vector<std::size_t> & tokens, const std::string & delimiters = ",")
{
    std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    std::string::size_type pos = str.find_first_of(delimiters, lastPos);
    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        tokens.push_back(parse_size(str.substr(lastPos, pos - lastPos)));
        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
    }
}
std::istream & operator>>(std::istream & in, content_sizes & value)
{
    std::string token;
    in >> token;
    tokenize(token, value.values);
    return in;
}
std::istream & operator>>(std::istream & in, thread_counts & value)
{
    std::string token;
    in >> token;
    tokenize(token, value.values);
    return in;
}
} // namespace app
} // namespace bench