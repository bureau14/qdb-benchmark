if(NOT TARGET qdb_api)
    message(STATUS "Not building quasardb tests")
    return()
endif()

add_library(bench-tests-quasardb
    blob/attach_tag.hpp
    blob/get.hpp
    blob/get_noalloc.hpp
    blob/has_tag.hpp
    blob/put.hpp
    blob/remove.hpp
    blob/detach_tag.hpp
    blob/update.hpp
    deque/get_at.hpp
    deque/pop_back.hpp
    deque/pop_front.hpp
    deque/push_back.hpp
    deque/push_front.hpp
    integer/add.hpp
    integer/get.hpp
    integer/put.hpp
    integer/remove.hpp
    integer/update.hpp
    stream/write.hpp
    tag/attach_blob.hpp
    tag/get_tagged.hpp
    tag/get_tags.hpp
    ts/col_blob_insert.hpp
    ts/col_double_insert.hpp
    ts/col_double_aggregate.hpp

    node_status.hpp
    qdb_test_template.hpp
    quasardb_facade.hpp
    quasardb_facade.cpp
    quasardb_tests.hpp
)
target_compile_options(bench-tests-quasardb
    INTERFACE -DBENCHMARK_QUASARDB=1
)
target_link_libraries(bench-tests-quasardb
    qdb_api
    ArduinoJson
    utils
)
