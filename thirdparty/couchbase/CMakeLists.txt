find_library(COUCHBASE_LIBRARY couchbase)
if (NOT COUCHBASE_LIBRARY)
    message(STATUS "Could NOT find couchbase library")
    return()
endif()
message(STATUS "Found couchbase library: ${COUCHBASE_LIBRARY}")

find_path(COUCHBASE_INCLUDE 
    libcouchbase/couchbase.h
)

if(NOT IS_DIRECTORY ${COUCHBASE_INCLUDE})
    message(STATUS "Could NOT find couchbase include directory")
    return()
endif()

add_library(couchbase SHARED IMPORTED GLOBAL)
set_target_properties(couchbase 
    PROPERTIES 
    IMPORTED_LOCATION ${COUCHBASE_LIBRARY}
    INTERFACE_INCLUDE_DIRECTORIES ${COUCHBASE_INCLUDE}
)