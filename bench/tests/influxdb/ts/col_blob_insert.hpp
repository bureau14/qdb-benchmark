#pragma once

#include <bench/tests/influxdb/influxdb_test_template.hpp>

namespace bench
{
namespace tests
{
namespace influxdb
{
namespace ts
{
class col_blob_insert : public influxdb_test_template<col_blob_insert>
{
public:
    explicit col_blob_insert(bench::test_config config) : influxdb_test_template(config)
    {
    }

    void run_iteration(std::uint32_t iteration)
    {
        _influxdb.ts_col_blob_insert(alias(iteration), "blob_col", content(iteration));
    }

    void cleanup() override
    {
        _influxdb.cleanup();
    }

    static std::string name()
    {
        return "influxdb_ts_col_blob_insert";
    }

    static std::string description()
    {
        return "Each thread repeats influxdb.insert with new ids";
    }

    static bool size_dependent()
    {
        return true;
    }
};
} // namespace ts
} // namespace influxdb
} // namespace tests
} // namespace bench
