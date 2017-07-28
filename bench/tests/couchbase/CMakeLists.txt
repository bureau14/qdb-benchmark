if(NOT TARGET couchbase)
    message(STATUS "Not building couchbase tests")
    return()
endif()

    add_compile_options(-g3)

add_library(bench-tests-couchbase
    blob/get.hpp
    blob/put.hpp
    blob/update.hpp
    couchbase_test_template.hpp
    couchbase_facade.cpp
    couchbase_facade.hpp
    couchbase_tests.hpp
    server_probe.hpp
)
target_compile_options(bench-tests-couchbase
    INTERFACE
    -DBENCHMARK_COUCHBASE=1
)
target_link_libraries(bench-tests-couchbase 
    couchbase
)
