#pragma once

#include <bench/tests/qdb/qdb_test_template.hpp>

#include <time.h>

namespace bench
{
namespace tests
{
namespace qdb
{
namespace ts
{
class col_double_average : public qdb_test_template<col_double_average>
{
public:
    explicit col_double_average(bench::test_config config) : qdb_test_template(config), _ts_size(config.content_size)
    {
    }

    void setup() override
    {
        qdb_test_template::setup();

        _qdb.ts_create(alias(0), {qdb_ts_column_info_t{"double_col", qdb_ts_column_double}});

        std::vector<qdb_ts_double_point> points(_ts_size);

        qdb_time_t cursor = 1490206139; 

        std::generate(points.begin(), points.end(), [&cursor]()
        {
            return qdb_ts_double_point{qdb_timespec_t{cursor++, 0}, static_cast<double>(cursor)};   
        });

        _inserted_range.begin = points.front().timestamp;
        _inserted_range.end = points.back().timestamp;

        _qdb.ts_col_double_inserts(alias(0), "double_col", points.data(), points.size());
    }

    void run_iteration(std::uint32_t iteration)
    {
        _qdb.ts_col_double_average(alias(0), "double_col", _inserted_range);
    }

    void cleanup() override
    {
        _qdb.remove(alias(0));
        qdb_test_template::cleanup();
    }

    static std::string name()
    {
        return "qdb_ts_col_double_average";
    }

    static std::string description()
    {
        return "Each thread averages the whole time series";
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
