#pragma once

#include <bench/tests/common/test_template.hpp>
#include <bench/tests/couchbase/couchbase_facade.hpp>
#include <bench/tests/couchbase/server_probe.hpp>
#include <utils/static_unique_pointer_cast.hpp>
#include <utils/unique_alias_provider.hpp>
#include <utils/unique_content_provider.hpp>

namespace bench
{
namespace tests
{
namespace couchbase
{

template <typename Derived>
class couchbase_test_template : public test_template<Derived>,
                                protected utils::unique_alias_provider,
                                protected utils::unique_content_provider
{
public:
    couchbase_test_template(test_config config)
        : test_template<Derived>(config), unique_content_provider(config.content_size), _cluster_uri(config.cluster_uri)
    {
    }

    void setup() override
    {
        _couchbase.connect(_cluster_uri);
    }

    static probe_collection create_probes(test_config cfg)
    {
        probe_collection probes;
        probes.emplace_back(static_unique_pointer_cast<probe>(std::make_unique<server_probe>(cfg.cluster_uri)));
        return probes;
    }

protected:
    couchbase_facade _couchbase;

private:
    std::string _cluster_uri;
};

} // namespace couchbase
} // namespace tests
} // namespace bench
