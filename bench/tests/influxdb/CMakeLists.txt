

include_directories(${influxdb-api_INCLUDE_DIRS})

add_library(bench-tests-influxdb
    ts/col_blob_insert.hpp
    ts/col_double_aggregate_part.hpp
    ts/col_double_aggregate.hpp
    ts/col_double_insert.hpp

    influxdb_test_template.hpp
    influxdb_facade.cpp
    influxdb_facade.hpp
    influxdb_tests.hpp
    server_probe.hpp
)
target_compile_options(bench-tests-influxdb
    INTERFACE
    -DBENCHMARK_INFLUXDB=1
)

target_link_libraries(bench-tests-influxdb
    influxdb-api
)
