cmake_minimum_required(VERSION 3.2)
project(qdb-benchmark)
enable_testing()

set(CMAKE_CXX_STANDARD 11)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS regex system thread)
find_package(Threads)

add_subdirectory(thirdparty/ArduinoJson/src)
add_subdirectory(thirdparty/cassandra)
add_subdirectory(thirdparty/cppformat)
add_subdirectory(thirdparty/mongodb)
add_subdirectory(thirdparty/netsnmp)
add_subdirectory(thirdparty/quasardb)

if(NOT MSVC)
    add_compile_options(-Wall -pedantic)
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
    # Force static build
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
endif()

include_directories(${CMAKE_SOURCE_DIR})
add_subdirectory(bench/app)
add_subdirectory(bench/core)
add_subdirectory(bench/framework)
add_subdirectory(bench/log)
add_subdirectory(bench/report)
add_subdirectory(bench/tests/dummy)
add_subdirectory(bench/tests/cassandra)
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

set(CPACK_PACKAGE_VERSION "2.0.2")
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