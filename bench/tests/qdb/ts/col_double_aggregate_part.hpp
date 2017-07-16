#pragma once

#include <bench/tests/qdb/qdb_test_template.hpp>
#include <random>

namespace bench
{
namespace tests
{
namespace qdb
{
namespace ts
{
class col_double_average_part : public qdb_test_template<col_double_average_part>
{
public:
    explicit col_double_average_part(bench::test_config config)
        : qdb_test_template(config), _ts_size(config.content_size)
    {
    }

    void setup() override
    {
        qdb_test_template::setup();
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<double> dist(-1000000, 1000000);

        _qdb.ts_create(alias(0), {qdb_ts_column_info_t{"double_col", qdb_ts_column_double}});

        std::vector<qdb_ts_double_point> points(_ts_size);

        qdb_time_t cursor = 1490206139;

        std::generate(points.begin(), points.end(), [&cursor]() {
            cursor += 2;
            return qdb_ts_double_point{qdb_timespec_t{cursor, 0}, dist(gen)};
        });

        _inserted_range.begin = points.front().timestamp;
        _inserted_range.end = points.back().timestamp;

        _qdb.ts_col_double_inserts(alias(0), "double_col", points.data(), points.size());
    }

    void run_iteration(std::uint32_t iteration)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static auto part = (_inserted_range.end.tv_sec - _inserted_range.begin.tv_sec) / 100;
        static std::uniform_int_distribution<idb_time_t> dist(_inserted_range.begin.tv_sec,
                                                              _inserted_range.end.tv_sec - part);

        auto start = dist(gen);
        qdb_ts_range_t range{qdb_timespec_t{start, 0}, qdb_timespec_t{start + part, 0}};
        _qdb.ts_col_double_average(alias(0), "double_col", range);
    }

    void cleanup() override
    {
        _qdb.remove(alias(0));
        qdb_test_template::cleanup();
    }

    static std::string name()
    {
        return "qdb_ts_col_double_average_part";
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
    qdb_ts_range_t _inserted_range;
};
} // namespace ts
} // namespace qdb
} // namespace tests
} // namespace bench
