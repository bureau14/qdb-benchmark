#pragma once

#include <libcouchbase/couchbase.h>
#include <condition_variable>
#include <mutex>
#include <string>
#include <vector>

namespace bench
{
namespace tests
{
namespace couchbase
{

class couchbase_facade
{
public:
    couchbase_facade();
    ~couchbase_facade();

    void connect(const std::string & cluster_uri);

    void remove(const std::string & alias);

    void blob_put(const std::string & alias, const std::string & content);
    void blob_update(const std::string & alias, const std::string & content);
    std::string blob_get(const std::string & alias);

private:
    lcb_t _instance;
};

} // namespace couchbase
} // namespace tests
} // namespace bench
