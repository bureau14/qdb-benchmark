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
class col_double_aggregate : public influxdb_test_template<col_double_aggregate>
{
public:
    explicit col_double_aggregate(bench::test_config config)
        : influxdb_test_template(config), _ts_size(config.content_size)
    {
    }

    void setup() override
    {
        influxdb_test_template::setup();

        std::vector<timepoint> points(_ts_size);

        idb_time_t cursor = 1490206139;

        std::generate(points.begin(), points.end(), [&cursor]() {
            return timepoint{static_cast<double>(cursor), cursor++};
        });

        _inserted_range = timerange{points.front().timestamp, points.back().timestamp};

        _influxdb.ts_col_double_inserts(alias(0), "double_col", points);
    }

    void run_iteration(std::uint32_t iteration)
    {
        _influxdb.ts_col_double_average(alias(0), "double_col", _inserted_range);
    }

    void cleanup() override
    {
        _influxdb.cleanup();
    }

    static std::string name()
    {
        return "influxdb_ts_col_double_average";
    }

    static std::string description()
    {
        return "Each thread averages a time series";
    }

    static bool size_dependent()
    {
        return true;
    }

private:
    const size_t _ts_size;
    timerange _inserted_range;
};
} // namespace ts
} // namespace influxdb
} // namespace tests
} // namespace bench
