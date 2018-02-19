macro(set_ide_folder args)
    set(folder ${ARGV0})
    foreach(project ${ARGN})
        # message("${folder} <- ${project}")
        if(TARGET ${project})
            set_target_properties(${project} PROPERTIES FOLDER ${folder})
        endif()
    endforeach()
endmacro()

set_ide_folder("apps"
    qdb-benchmark
)

set_ide_folder("libs"
    bench-app
    bench-core
    bench-framework
    bench-log
    bench-report
    bench-tests-quasardb
    utils
)

set_ide_folder("thirdparty"
    ArduinoJson
    fmt
)

set_ide_folder("thirdparty/boost"
    boost_filesystem
    boost_program_options
    boost_regex
    boost_serialization
    boost_system
)
