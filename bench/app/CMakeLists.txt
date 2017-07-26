add_library(bench-app
    program.cpp
    program.hpp
    options.hpp
    options.cpp
    settings.hpp
)

target_link_libraries(bench-app
    bench-framework
    bench-report
    boost_program_options
)
