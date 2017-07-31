#pragma once

#include <bench/core/test_class.hpp>
#include <chrono>
#include <string>
#include <vector>

namespace bench
{
namespace app
{

struct settings
{
    std::string cluster_uri;
    std::vector<std::size_t> thread_counts;
    std::vector<std::size_t> content_sizes;
    std::vector<std::size_t> iteration_counts;
    test_class_collection tests;
    std::chrono::seconds pause;
    std::chrono::seconds duration;

    std::string user_credentials_file;
    std::string cluster_public_file;

    bool no_cleanup;

#if BENCHMARK_SNMP
    std::vector<std::string> snmp_peers;
#endif
};

} // namespace app
} // namespace bench
