set(FILES
    fatal_error.hpp
    memory.hpp
    random.cpp
    random.hpp
    static_unique_pointer_cast.hpp
    teamcity.cpp
    teamcity.hpp
    unique_alias.cpp
    unique_alias.hpp
    unique_alias_provider.hpp
    unique_content.cpp
    unique_content.hpp
    unique_content_provider.hpp
    watermark.cpp
    watermark.hpp
)

if (WIN32)
    list(APPEND FILES
        win32.hpp
        win32.cpp
    )
endif()

add_library(utils ${FILES})
target_link_libraries(utils cppformat)
