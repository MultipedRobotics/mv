cmake_minimum_required(VERSION 3.25.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 17)

set(PROJECT mv)

set(BUILDFOR "apple" CACHE STRING "what to build for: apple or pico")

if (BUILDFOR STREQUAL "apple" OR BUILDFOR STREQUAL "linux")
  # PROJECT_VERSION_MAJOR year
  # PROJECT_VERSION_MINOR month
  # PROJECT_VERSION_PATCH day
  # PROJECT_VERSION_TWEAK tweak
  project(${PROJECT} VERSION 2023.01.28.0 LANGUAGES C CXX)

  # Threads ---------------------------------------------------------
  find_package(Threads REQUIRED)
  string(PREPEND CMAKE_EXE_LINKER_FLAGS " -Wl ")
  set(PROJECT_TARGET_PLATFORM "Unix")
elseif(BUILDFOR STREQUAL "pico")
  set(PICO_BOARD pico) # pico or pico_w or pico2
  set(PICO_SDK_PATH "$ENV{HOME}/github/pico-sdk")
  set(ENV{PATH} "$ENV{HOME}/github/gcc-arm/bin:$ENV{PATH}")
  set(PICO_SDK_PATH "${PICO_SDK_PATH}" CACHE PATH "Path to the PICO SDK")
  set(PICO_CXX_ENABLE_EXCEPTIONS 0)

  include(${PICO_SDK_PATH}/pico_sdk_init.cmake)

  # Need ASM for tinyUSB and picotool to work
  project(${PROJECT} LANGUAGES C CXX ASM)

  # Initialize the SDK
  pico_sdk_init()

  string(PREPEND CMAKE_EXE_LINKER_FLAGS " -Wl,--print-memory-usage ")
  set(PROJECT_TARGET_PLATFORM "Pi Pico")
else()
  message(FATAL_ERROR "No BUILDFOR given")
endif()

string (APPEND CMAKE_CXX_FLAGS_RELEASE " -O3 ")
string (APPEND CMAKE_C_FLAGS_RELEASE " -O3 ")


if(PROJECT_IS_TOP_LEVEL)
    cmake_host_system_information(RESULT HOST QUERY HOSTNAME)
    cmake_host_system_information(RESULT OSN QUERY OS_NAME)
    cmake_host_system_information(RESULT OS_VERSION QUERY OS_RELEASE)
    cmake_host_system_information(RESULT PROC QUERY PROCESSOR_DESCRIPTION)

    message(STATUS "-------------------------------------")
    message(STATUS " Project: ${PROJECT_NAME}")
    message(STATUS " Target: ${PROJECT_TARGET_PLATFORM}")
    message(STATUS " Board: ${PICO_BOARD}")
    message(STATUS " Version: ${CMAKE_PROJECT_VERSION}")
    message(STATUS " C++ ${CMAKE_CXX_STANDARD}")
    message(STATUS " C ${CMAKE_C_STANDARD}")
    message(STATUS "-------------------------------------")
    message(STATUS " ${HOST}")
    message(STATUS " ${OSN}: ${OS_VERSION}")
    message(STATUS " ${PROC}")
    message(STATUS "-------------------------------------")

    set(BUILD_EXAMPLES ON)
    if (NOT PICO_SDK)
      set(BUILD_GTESTS ON)
    else()
      set(BUILD_GTESTS OFF)
    endif()
else()
    message(STATUS "-> ${PROJECT_NAME} is submodule")
    set(BUILD_EXAMPLES OFF)
    set(BUILD_GTESTS OFF)
endif()

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE src)

# Examples -------------------------------------------------------
if(${BUILD_EXAMPLES})
  if (PICO_SDK)
    message(STATUS "building pico examples")
    add_subdirectory(examples/pico)
  else()
    message(STATUS "building unix examples")
    add_subdirectory(examples/unix)
  endif()
endif()

# Tests --------------------------------------------------------------
# if(${BUILD_GTESTS})
#   message(STATUS "building unix tests")
#   add_subdirectory(test)
# endif()
