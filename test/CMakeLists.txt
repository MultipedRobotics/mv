include(FetchContent)

# GTest -----------------
FetchContent_Declare(gtest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.15.2 # or another tag/branch
)
FetchContent_MakeAvailable(gtest)

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

