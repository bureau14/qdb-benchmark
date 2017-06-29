add_definitions(
    -DBOOST_SYSTEM_STATIC_LINK=1
)

add_library(boost_system
    src/error_code.cpp
)