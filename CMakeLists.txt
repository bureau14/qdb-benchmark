cmake_minimum_required(VERSION 3.2)
project(qdb-benchmark)
enable_testing()

option(QDB_ENABLE_CASSANDRA "Enables build for cassandra"   OFF)
option(QDB_ENABLE_MONGODB   "Enables build for mongodb"     OFF)
option(QDB_ENABLE_COUCHBASE "Enables build for couchbase"   OFF)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Need c++14 for std::make_unique
set(CMAKE_CXX_STANDARD 14)

if(MSVC)
    add_definitions(
        /DBOOST_ALL_NO_LIB # Disable auto-link.
    )
endif()
set(Boost_USE_STATIC_LIBS ON)

find_package(Threads)

add_subdirectory(thirdparty/ArduinoJson/src)
add_subdirectory(thirdparty/boost)
add_subdirectory(thirdparty/cppformat)
add_subdirectory(thirdparty/netsnmp)
add_subdirectory(thirdparty/quasardb)

if(QDB_ENABLE_CASSANDRA)
    add_subdirectory(thirdparty/cassandra)
endif()
if(QDB_ENABLE_MONGODB)
    add_subdirectory(thirdparty/mongodb)
endif()
if(QDB_ENABLE_COUCHBASE)
    add_subdirectory(thirdparty/couchbase)
endif()

if(NOT MSVC)
    add_compile_options(-Wall -pedantic)
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
    # Force static build
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
endif()

include_directories(${CMAKE_SOURCE_DIR})
include_directories("${CMAKE_SOURCE_DIR}/thirdparty/boost")
add_subdirectory(bench/app)
add_subdirectory(bench/core)
add_subdirectory(bench/framework)
add_subdirectory(bench/log)
add_subdirectory(bench/report)
add_subdirectory(bench/tests/dummy)
add_subdirectory(bench/tests/cassandra)
add_subdirectory(bench/tests/couchbase)
add_subdirectory(bench/tests/mongodb)
add_subdirectory(bench/tests/qdb)
add_subdirectory(bench/tests/std)
add_subdirectory(utils)

if (TARGET netsnmp)
    add_subdirectory(snmp)
endif()

add_executable(qdb-benchmark
    main.cpp
)

target_compile_definitions(qdb-benchmark
	PRIVATE
	-D_CRT_SECURE_NO_WARNINGS # disable VS warning on getenv()
)

target_link_libraries(qdb-benchmark
    bench-app
    bench-core
    bench-log
    bench-tests-dummy
    bench-tests-std
)
if (TARGET bench-tests-cassandra)
    target_link_libraries(qdb-benchmark bench-tests-cassandra)
endif()
if (TARGET bench-tests-couchbase)
    target_link_libraries(qdb-benchmark bench-tests-couchbase
    ssl
    crypto)
endif()
if (TARGET bench-tests-mongodb)
    target_link_libraries(qdb-benchmark bench-tests-mongodb)
endif()
if (TARGET bench-tests-quasardb)
    target_link_libraries(qdb-benchmark bench-tests-quasardb)
endif()

add_test(help qdb-benchmark --help)
add_test(help qdb-benchmark --tests qdb_blob_put --sizes 1M)

if(EXISTS ${QDB_API_DLL})
    add_custom_command(TARGET qdb-benchmark POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${QDB_API_DLL}
        $<TARGET_FILE_DIR:qdb-benchmark>)
endif()

set(CPACK_PACKAGE_VERSION "2.2.0")
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY 0)
include(CPack)
install(TARGETS qdb-benchmark
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
install(DIRECTORY html_report
    DESTINATION share/qdb-benchmark
)

include(ide_folders)
