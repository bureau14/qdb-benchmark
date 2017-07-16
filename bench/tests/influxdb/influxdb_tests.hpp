#pragma once

#include <bench/tests/influxdb/ts/col_blob_insert.hpp>
#include <bench/tests/influxdb/ts/col_double_aggregate.hpp>
#include <bench/tests/influxdb/ts/col_double_aggregate_part.hpp>
#include <bench/tests/influxdb/ts/col_double_insert.hpp>

namespace bench
{
namespace tests
{
namespace influxdb
{
template <typename OutputIt>
void get_tests(OutputIt it)
{
    *it++ = new ts::col_blob_insert::test_class();
    *it++ = new ts::col_double_insert::test_class();
    *it++ = new ts::col_double_aggregate::test_class();
    *it++ = new ts::col_double_average_part::test_class();
}
} // namespace influxdb
} // namespace tests
} // namespace bench