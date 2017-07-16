
project(influxdb-api)

set(CMAKE_CXX_STANDARD 14)
add_compile_options("-O3")

include_directories(${CPR_INCLUDE_DIRS})
add_library(${PROJECT_NAME}
    command/create.hpp
    command/drop.hpp
    command/insert.hpp
    command/query.hpp
    command.hpp
    api.cpp
    api.hpp
    measurement/tag.hpp
    measurement/field.hpp
    measurement.cpp
    measurement.hpp
)

target_link_libraries(${PROJECT_NAME}
    ${CPR_LIBRARIES}
    cppformat
)

set(INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CPR_INCLUDE_DIRS}
)

set(${PROJECT_NAME}_INCLUDE_DIRS ${INCLUDE_DIRS}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)
install(FILES ${PROJECT_NAME} DESTINATION lib)