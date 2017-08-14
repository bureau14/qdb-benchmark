#pragma once

#include <chrono>
#include <string>

namespace bench
{

struct test_config
{
    std::string cluster_uri;
    std::size_t content_size;
    std::size_t iterations;
    bool no_cleanup;
    std::size_t thread_count;
    std::chrono::seconds duration;

    std::string cluster_public_file;
    std::string user_credentials_file;
    std::string encryption_algorithm;
};

} // namespace bench
