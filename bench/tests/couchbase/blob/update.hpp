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
class update : public couchbase_test_template<update>
{
public:
    explicit update(bench::test_config config) : couchbase_test_template(config)
    {
    }

    void setup() override
    {
        couchbase_test_template::setup();
        _couchbase.blob_put(alias(0), content(0));
    }

    void run_iteration(std::uint32_t iteration)
    {
        _couchbase.blob_update(alias(0), content(iteration));
    }

    void cleanup() override
    {
        _couchbase.remove(alias(0));
    }

    static std::string name()
    {
        return "couchbase_blob_update";
    }

    static std::string description()
    {
        return "Each thread repeats couchbase.update on a single entry";
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
