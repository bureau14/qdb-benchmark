add_library(bench-tests
    qdb/blob_add_tag.hpp
    qdb/blob_get.hpp
    qdb/blob_get_noalloc.hpp
    qdb/blob_put.hpp
    qdb/blob_remove.hpp
    qdb/blob_update.hpp
    qdb/deque_pop_back.hpp
    qdb/deque_pop_front.hpp
    qdb/deque_push_back.hpp
    qdb/deque_push_front.hpp
    qdb/hset_contains.hpp
    qdb/hset_erase.hpp
    qdb/hset_insert.hpp
    qdb/int_add.hpp
    qdb/int_get.hpp
    qdb/int_put.hpp
    qdb/int_remove.hpp
    qdb/int_update.hpp
    qdb/node_status.hpp
    qdb/qdb_test_template.hpp
    qdb/tag_add_blob.hpp
    std/atomic.hpp
    std/fread.hpp
    std/fwrite.hpp
    std/mutex.hpp

    all_tests.cpp
    all_tests.hpp
    test_template.hpp
)

target_compile_options(bench-tests
    PRIVATE
    -D_CRT_SECURE_NO_WARNINGS
)

target_link_libraries(bench-tests
    bench-framework
)

target_include_directories(bench-tests
    PRIVATE 
    ${QDB_API_INCLUDE_DIR}
)

target_link_libraries(bench-tests
    ${QDB_API}
)