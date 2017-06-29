if(CLANG)
    add_compile_options(
        -Wno-overloaded-virtual # 'function' hides overloaded virtual function
    )
endif()

add_definitions(
    -DBOOST_FILESYSTEM_STATIC_LINK=1
)

add_library(boost_filesystem
    src/codecvt_error_category.cpp
    src/operations.cpp
    src/path.cpp
    src/path_traits.cpp
    src/portability.cpp
    src/unique_path.cpp
    src/utf8_codecvt_facet.cpp
    src/windows_file_codecvt.cpp
)