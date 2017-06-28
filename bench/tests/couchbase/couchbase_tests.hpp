#pragma once

#include <bench/tests/couchbase/batch/blob_put.hpp>
#include <bench/tests/couchbase/blob/get.hpp>
#include <bench/tests/couchbase/blob/put.hpp>
#include <bench/tests/couchbase/blob/update.hpp>

namespace bench
{
namespace tests
{
namespace couchbase
{
template <typename OutputIt>
void get_tests(OutputIt it)
{
    *it++ = new batch::blob_put::test_class();
    *it++ = new blob::put::test_class();
    *it++ = new blob::get::test_class();
    *it++ = new blob::update::test_class();
}
}
}
}