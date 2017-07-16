#pragma once

#include <influxdb/api.hpp>
#include <string>
#include <vector>

namespace bench
{
namespace tests
{
namespace influxdb
{

struct timepoint
{
    double value;
    idb_time_t timestamp;
};

struct timerange
{
    idb_time_t start;
    idb_time_t end;
};

class influxdb_facade
{
public:
    influxdb_facade();
    ~influxdb_facade();

    void connect(const std::string & cluster_uri, const std::string & dbname);

    void remove(const std::string & alias);
    void cleanup();

    void ts_col_blob_insert(const std::string & alias, const std::string & col_name, const std::string & content);
    void ts_col_double_insert(const std::string & alias, const std::string & col_name, const timepoint & point);
    void ts_col_double_inserts(const std::string & alias,
                               const std::string & col_name,
                               const std::vector<timepoint> & points);
    void ts_col_double_average(const std::string & alias, const std::string & col_name, const timerange & range);

private:
    idb::api::api m_api;
};

} // namespace influxdb
} // namespace tests
} // namespace bench
