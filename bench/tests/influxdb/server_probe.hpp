#pragma once

#include <bench/core/probe.hpp>
#include <bench/tests/influxdb/influxdb_facade.hpp>
#include <algorithm>

namespace bench
{
namespace tests
{
namespace influxdb
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
        // TODO(vianney): this assumes the cluster uri is just a single node.
        // which it is on the free version
        return cluster_uri;
    }

    std::string _cluster_uri;
    influxdb_facade _influxdb;
    std::vector<std::string> _nodes;
};

} // namespace influxdb
} // namespace tests
} // namespace bench