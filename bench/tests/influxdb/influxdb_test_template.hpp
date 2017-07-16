#pragma once

#include <bench/tests/common/test_template.hpp>
#include <bench/tests/influxdb/influxdb_facade.hpp>
#include <bench/tests/influxdb/server_probe.hpp>
#include <utils/static_unique_pointer_cast.hpp>
#include <utils/unique_alias_provider.hpp>
#include <utils/unique_content_provider.hpp>

namespace bench
{
namespace tests
{
namespace influxdb
{
template <typename Derived>
class influxdb_test_template : public test_template<Derived>,
                               protected utils::unique_alias_provider,
                               protected utils::unique_content_provider
{

public:
    influxdb_test_template(test_config config)
        : test_template<Derived>(config), unique_content_provider(config.content_size), _cluster_uri(config.cluster_uri)
    {
    }

    void setup() override
    {
        _influxdb.connect("http://localhost:8086", alias(0));
    }

    void cleanup() override
    {
        _influxdb.cleanup();
    }

    static probe_collection create_probes(test_config cfg)
    {
        probe_collection probes;
        probes.emplace_back(static_unique_pointer_cast<probe>(std::make_unique<server_probe>(cfg.cluster_uri)));
        return probes;
    }

protected:
    influxdb_facade _influxdb;

private:
    std::string _cluster_uri;
};

} // namespace influxdb
} // namespace tests
} // namespace bench
