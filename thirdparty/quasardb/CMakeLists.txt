if(WIN32)
    find_file(QDB_API_DLL
        qdb_api.dll
        HINTS ${CMAKE_CURRENT_LIST_DIR}/bin
    )

    if(NOT EXISTS ${QDB_API_DLL})
        message(STATUS "Could NOT find qdb_api.dll")
        return()
    endif()

    message(STATUS "Found qdb_api.dll: ${QDB_API_DLL}")
endif()

find_library(QDB_API_LIBRARY
    qdb_api
    HINTS ${CMAKE_CURRENT_LIST_DIR}/lib
)

if(NOT EXISTS ${QDB_API_LIBRARY})
    message(STATUS "Could NOT find qdb_api library")
    return()
endif()

message(STATUS "Found qdb_api library: ${QDB_API_LIBRARY}")

find_path(QDB_API_INCLUDE
    qdb/client.h
    HINTS ${CMAKE_CURRENT_LIST_DIR}/include
)

if(NOT IS_DIRECTORY ${QDB_API_INCLUDE})
    message(STATUS "Could NOT find qdb_api include directory")
    return()
endif()

if (WIN32)
    add_library(qdb_api STATIC IMPORTED GLOBAL)
else()
    add_library(qdb_api SHARED IMPORTED GLOBAL)
endif()

set_target_properties(qdb_api
    PROPERTIES
    IMPORTED_LOCATION ${QDB_API_LIBRARY}
    INTERFACE_INCLUDE_DIRECTORIES ${QDB_API_INCLUDE}
)

if (WIN32)
    install(FILES ${QDB_API_DLL} DESTINATION bin)
else()
    install(FILES ${QDB_API_LIBRARY} DESTINATION lib)
endif()
