#include <bench/tests/all_tests.hpp>
#include <bench/tests/dummy/cleanup_fail.hpp>
#include <bench/tests/dummy/setup_fail.hpp>
#include <bench/tests/dummy/test_fail.hpp>
#include <bench/tests/qdb/blob_add_tag.hpp>
#include <bench/tests/qdb/blob_get.hpp>
#include <bench/tests/qdb/blob_get_noalloc.hpp>
#include <bench/tests/qdb/blob_put.hpp>
#include <bench/tests/qdb/blob_remove.hpp>
#include <bench/tests/qdb/blob_update.hpp>
#include <bench/tests/qdb/deque_pop_back.hpp>
#include <bench/tests/qdb/deque_pop_front.hpp>
#include <bench/tests/qdb/deque_push_back.hpp>
#include <bench/tests/qdb/deque_push_front.hpp>
#include <bench/tests/qdb/hset_contains.hpp>
#include <bench/tests/qdb/hset_erase.hpp>
#include <bench/tests/qdb/hset_insert.hpp>
#include <bench/tests/qdb/int_add.hpp>
#include <bench/tests/qdb/int_get.hpp>
#include <bench/tests/qdb/int_put.hpp>
#include <bench/tests/qdb/int_remove.hpp>
#include <bench/tests/qdb/int_update.hpp>
#include <bench/tests/qdb/tag_add_blob.hpp>
#include <bench/tests/std/atomic.hpp>
#include <bench/tests/std/fread.hpp>
#include <bench/tests/std/fwrite.hpp>
#include <bench/tests/std/mutex.hpp>

bench::test_class_collection bench::tests::get_all_tests()
{
    // clang-format off
    return
    {
      // new dummy::setup_fail::test_class(),
      // new dummy::test_fail::test_class(),
      // new dummy::cleanup_fail::test_class(),
      new qdb::blob_add_tag::test_class(),
      new qdb::blob_get::test_class(),
      new qdb::blob_get_noalloc::test_class(),
      new qdb::blob_put::test_class(),
      new qdb::blob_remove::test_class(),
      new qdb::blob_update::test_class(),
      new qdb::deque_pop_back::test_class(),
      new qdb::deque_pop_front::test_class(),
      new qdb::deque_push_back::test_class(),
      new qdb::deque_push_front::test_class(),
      new qdb::hset_contains::test_class(),
      new qdb::hset_erase::test_class(),
      new qdb::hset_insert::test_class(),
      new qdb::int_add::test_class(),
      new qdb::int_get::test_class(),
      new qdb::int_put::test_class(),
      new qdb::int_remove::test_class(),
      new qdb::int_update::test_class(),
      new qdb::tag_add_blob::test_class(),
      new std::atomic::test_class(),
      new std::fread::test_class(),
      new std::fwrite::test_class(),
      new std::mutex::test_class(),
    };
    // clang-format on
}
