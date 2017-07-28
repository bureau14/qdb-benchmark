#pragma once

#include <bench/core/probe.hpp>
#include <bench/tests/couchbase/couchbase_facade.hpp>
#include <algorithm>

namespace bench
{
namespace tests
{
namespace couchbase
{

class server_probe : public probe
{
public:
    server_probe(std::string cluster_uri) : _cluster_uri(cluster_uri)
    {
    }

    void setup() override
    {
        // Nothing yet
    }

    void take_sample() override
    {
    }

    void cleanup() override
    {
    }

private:
    // Returns the first node of a cluster uri
    static std::string first_node(std::string const & cluster_uri)
    {
        // TODO: this assumes the cluster uri is just a single node.
        return cluster_uri;
    }

    std::string _cluster_uri;
    couchbase_facade _couchbase;
};

} // namespace couchbase
} // namespace tests
} // namespace bench
