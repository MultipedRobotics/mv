cmake_minimum_required(VERSION 3.5)
project(mv-ros2)

# Default to C99
# if(NOT CMAKE_C_STANDARD)
#     set(CMAKE_C_STANDARD 99)
# endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

# Binary ----------------------------------------------------
add_executable(hello bin/main.cpp)
ament_target_dependencies(hello rclcpp std_msgs)

install(TARGETS
  hello
  DESTINATION lib/${PROJECT_NAME})

# Library ----------------------------------------------------
add_library(mv
    src/protocol1.cpp
    src/protocol2.cpp
    src/SerialPort.cpp
    src/ax12.cpp)
target_include_directories(mv PUBLIC src)
ament_target_dependencies(mv rclcpp std_msgs)

install(
  DIRECTORY include/
  DESTINATION include
)

install(
  TARGETS mv
  EXPORT export_my_library
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)

# install(TARGETS
#     mv
#     DESTINATION lib/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # uncomment the line when a copyright and license is not present in all source files
  #set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # uncomment the line when this package is not in a git repo
  #set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
