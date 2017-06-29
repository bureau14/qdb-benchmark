if(MSVC)
    add_compile_options(
        # Disable warnings:
        /wd4267 # 'var': conversion from 'size_t' to 'type', possible loss of data
    )
endif()

include_directories(".")

add_subdirectory(libs/filesystem)
add_subdirectory(libs/program_options)
add_subdirectory(libs/regex)
add_subdirectory(libs/serialization)
add_subdirectory(libs/system)
add_subdirectory(libs/test)