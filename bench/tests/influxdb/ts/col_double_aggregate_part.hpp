#pragma once

#include <bench/tests/influxdb/influxdb_test_template.hpp>
#include <random>

namespace bench
{
namespace tests
{
namespace influxdb
{
namespace ts
{
class col_double_average_part : public influxdb_test_template<col_double_average_part>
{
public:
    explicit col_double_average_part(bench::test_config config)
        : influxdb_test_template(config), _ts_size(config.content_size)
    {
    }

    void setup() override
    {
        influxdb_test_template::setup();
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<double> dist(-1000000, 1000000);

        std::vector<timepoint> points(_ts_size);

        idb_time_t cursor = 1490206139;

        std::generate(points.begin(), points.end(), [&cursor]() {
            ++cursor;
            return timepoint{dist(gen), cursor + 2};
        });

        _inserted_range = timerange{points.front().timestamp, points.back().timestamp};

        _influxdb.ts_col_double_inserts(alias(0), "double_col", points);
    }

    void run_iteration(std::uint32_t iteration)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static auto part = (_inserted_range.end - _inserted_range.start) / 100;
        static std::uniform_int_distribution<idb_time_t> dist(_inserted_range.start, _inserted_range.end - part);

        idb_time_t start = dist(gen);
        auto range = timerange{start, start + part};
        _influxdb.ts_col_double_average(alias(0), "double_col", range);
    }

    void cleanup() override
    {
        _influxdb.cleanup();
    }

    static std::string name()
    {
        return "influxdb_ts_col_double_average_part";
    }

    static std::string description()
    {
        return "Each thread averages a 10% part of a time series";
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
