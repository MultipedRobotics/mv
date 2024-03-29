include(FetchContent)

# GTest -----------------
FetchContent_Declare(gtest
  GIT_REPOSITORY "https://github.com/google/googletest"
  GIT_TAG "origin/main"
  SOURCE_DIR "${CMAKE_BINARY_DIR}/gtest"
)
# set(EXAMPLES OFF CACHE INTERNAL "Dont build examples")
FetchContent_MakeAvailable(gtest)
if(gtest_POPULATED)
    message(STATUS "=> Found gtest")
else()
    message(STATUS "*** Didn't find gtest")
endif()

enable_testing()

list(APPEND gtests
    protocol_1.cpp
    protocol_2.cpp
)

message(STATUS " >> ${CMAKE_SOURCE_DIR}/src")

message(STATUS "mv::gTests ----------------------")
foreach(src ${gtests})
    get_filename_component(name ${src} NAME_WE)
    message(STATUS " gtest -> ${name} from ${src}")
    add_executable(${name} ${src})
    target_link_libraries(${name}
        PRIVATE
            GTest::gtest_main
            mv
    )
    add_test(NAME ${name} COMMAND ${name})
endforeach()

# run all tests
add_executable(test-all ${gtests})
target_link_libraries(test-all GTest::gtest_main mv)
add_test(NAME test-all COMMAND test-all)

