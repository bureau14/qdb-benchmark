add_library(boost_test
    src/compiler_log_formatter.cpp
    src/debug.cpp
    src/decorator.cpp
    src/execution_monitor.cpp
    src/framework.cpp
    src/plain_report_formatter.cpp
    src/progress_monitor.cpp
    src/results_collector.cpp
    src/results_reporter.cpp
    src/test_tools.cpp
    src/test_tree.cpp
    src/unit_test_log.cpp
    src/unit_test_main.cpp
    src/unit_test_monitor.cpp
    src/unit_test_parameters.cpp
    src/xml_log_formatter.cpp
    src/xml_report_formatter.cpp
)

if(MSVC)
    target_compile_options(boost_test
        PUBLIC
        /wd4702 # unreachable code
    )
endif()

if(CLANG AND NOT APPLE)
    target_compile_options(boost_test
        PRIVATE
        -Wno-unused-private-field
        -Wno-macro-redefined
    )
endif()
