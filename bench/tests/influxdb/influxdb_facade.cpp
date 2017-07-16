#include <bench/tests/influxdb/influxdb_facade.hpp>
#include <cppformat/format.h>
#include <exception>
#include <iostream>

using api = idb::api::api;
using str_field = idb::api::measurement::field<std::string>;
using double_field = idb::api::measurement::field<double>;
using measurement = idb::api::measurement::measurement;
using measurements = idb::api::measurement::measurements;

using namespace bench::tests::influxdb;

influxdb_facade::influxdb_facade()
{
}

influxdb_facade::~influxdb_facade()
{
}

void influxdb_facade::connect(const std::string & cluster_uri, const std::string & dbname)
{
    m_api = api(cluster_uri, fmt::format("benchmark-{}", dbname));
    try
    {
        m_api.createDatabase();
    }
    catch (...)
    {
        std::rethrow_exception(std::current_exception());
    }
}

void influxdb_facade::remove(const std::string & alias)
{
    try
    {
        m_api.drop(measurement(alias));
    }
    catch (...)
    {
        std::rethrow_exception(std::current_exception());
    }
}

void influxdb_facade::cleanup()
{
    try
    {
        m_api.dropDatabase();
    }
    catch (...)
    {
        // Exception thrown can safely be ignored
    }
}

void influxdb_facade::ts_col_blob_insert(const std::string & alias,
                                         const std::string & col_name,
                                         const std::string & content)
{
    try
    {
        measurement mes(alias);
        mes << str_field(col_name, content);
        m_api.insert(mes);
    }
    catch (...)
    {
        std::rethrow_exception(std::current_exception());
    }
}

void influxdb_facade::ts_col_double_insert(const std::string & alias,
                                           const std::string & col_name,
                                           const timepoint & point)
{
    try
    {
        measurement mes(alias);
        mes << double_field(col_name, point.value) << point.timestamp;
        m_api.insert(mes);
    }
    catch (...)
    {
        std::rethrow_exception(std::current_exception());
    }
}

void influxdb_facade::ts_col_double_inserts(const std::string & alias,
                                            const std::string & col_name,
                                            const std::vector<timepoint> & points)
{
    try
    {
        measurements measures;
        for (const auto & point : points)
        {
            measurement mes(alias);
            mes << double_field(col_name, point.value) << point.timestamp;
            measures << mes;
        }
        m_api.insert(measures);
    }
    catch (...)
    {
        std::rethrow_exception(std::current_exception());
    }
}

void influxdb_facade::ts_col_double_average(const std::string & alias,
                                            const std::string & col_name,
                                            const timerange & range)
{
    try
    {
        std::string what = fmt::format("MEAN(\"{}\")", alias);
        std::string from = fmt::format("\"{}\"", col_name);
        std::string where = fmt::format("time >= '{}' AND time < '{}'", range.start, range.end);
        m_api.select(what, from, where);
    }
    catch (...)
    {
        std::rethrow_exception(std::current_exception());
    }
}
