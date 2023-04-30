cmake_minimum_required(VERSION 3.25.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# PROJECT_VERSION_MAJOR year
# PROJECT_VERSION_MINOR month
# PROJECT_VERSION_PATCH day
# PROJECT_VERSION_TWEAK tweak
project(mv VERSION 2023.01.28.0)

if(PROJECT_IS_TOP_LEVEL)
    cmake_host_system_information(RESULT HOST QUERY HOSTNAME)
    cmake_host_system_information(RESULT OSN QUERY OS_NAME)
    cmake_host_system_information(RESULT OS_VERSION QUERY OS_RELEASE)
    cmake_host_system_information(RESULT PROC QUERY PROCESSOR_DESCRIPTION)

    message(STATUS "-------------------------------------")
    message(STATUS "  Project: ${PROJECT_NAME}")
    message(STATUS "  Version: ${CMAKE_PROJECT_VERSION}")
    message(STATUS "  C++ ${CMAKE_CXX_STANDARD}")
    message(STATUS "-------------------------------------")
    message(STATUS " ${HOST}")
    message(STATUS " ${OSN}: ${OS_VERSION}")
    message(STATUS " ${PROC}")
    message(STATUS "-------------------------------------")

    set(BUILD_EXAMPLES ON)
    set(BUILD_GTESTS ON)
else()
    message(STATUS "-> ${PROJECT_NAME} is submodule")
    set(BUILD_EXAMPLES OFF)
    set(BUILD_GTESTS OFF)
endif()

# configure_file(${path to configure file 'config.h.in'}
# include_directories(${PROJECT_BINARY_BIN}) // this allows the 'config.h' file to be used throughout the program

# set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
# set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
# set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
# include(CPack)

# Boost -----------------------------------------------------------
# find_package(Boost REQUIRED system)

# Threads ---------------------------------------------------------
# find_package(Threads REQUIRED)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE src)

# Examples -------------------------------------------------------
# if(${BUILD_EXAMPLES})
#     add_subdirectory(examples/unix)
# endif()

# Tests --------------------------------------------------------------
if(${BUILD_GTESTS})
    add_subdirectory(test)
endif()
