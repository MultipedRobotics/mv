list(APPEND ex
    ax12-ex.cpp
    feedback.cpp
    path.cpp
    sync_write.cpp
)

message(STATUS "Examples ----------------------")
foreach(src ${ex})
    get_filename_component(name ${src} NAME_WE)
    message(STATUS " -> ${name}")
    # message(STATUS ">> ${ALL_LIBS}")
    add_executable(${name} ${src})
    target_link_libraries(${name} mv)
    target_include_directories(${name} PUBLIC ../src)
    add_test(NAME ${name} COMMAND ${name})
endforeach()
