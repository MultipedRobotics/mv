include(FetchContent)

# GTest -----------------
FetchContent_Declare(gtest
  GIT_REPOSITORY "https://github.com/google/googletest"
  GIT_TAG "origin/main"
  SOURCE_DIR "${CMAKE_BINARY_DIR}/_deps/gtest"
)
# set(EXAMPLES OFF CACHE INTERNAL "Dont build examples")
FetchContent_MakeAvailable(gtest)
if(gtest_POPULATED)
    message(STATUS "=> Found gtest")
else()
    message(STATUS "*** Didn't find gtest")
endif()

list(APPEND gtests
    protocol_1.cpp
    protocol_2.cpp
)

message(STATUS "Gecko::gTests ----------------------")
foreach(src ${gtests})
    get_filename_component(name ${src} NAME_WE)
    message(STATUS " -> ${name}")
    # message(STATUS ">> ${ALL_LIBS}")
    add_executable(${name} ${src})
    target_link_libraries(${name} gtest_main mv)
    target_include_directories(${name} PUBLIC ../src)
    add_test(NAME ${name} COMMAND ${name})
endforeach()

# run all tests -- doesn't work???
add_executable(test-all ${gtests})
target_link_libraries(test-all gtest_main mv)
add_test(NAME test-all COMMAND test-all)

# message(STATUS ">> gTest linked libraries: ${ALL_LIBS}")
