#pragma once

#include <bench/tests/couchbase/couchbase_test_template.hpp>

namespace bench
{
namespace tests
{
namespace couchbase
{
namespace blob
{
class put : public couchbase_test_template<put>
{
public:
    explicit put(bench::test_config config) : couchbase_test_template(config)
    {
    }

    void run_iteration(std::uint32_t iteration)
    {
        _couchbase.blob_put(alias(iteration), content(iteration));
    }

    void cleanup() override
    {
        cleanup_each([=](std::uint32_t iteration) { _couchbase.remove(alias(iteration)); });
    }

    static std::string name()
    {
        return "couchbase_blob_put";
    }

    static std::string description()
    {
        return "Each thread repeats couchbase.insert with new ids";
    }

    static bool size_dependent()
    {
        return true;
    }
};
} // namespace blob
} // namespace couchbase
} // namespace tests
} // namespace bench
