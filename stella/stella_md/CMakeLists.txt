################################################################################
# Set minimum required version of cmake, project name and compile options
################################################################################
cmake_minimum_required(VERSION 3.5)
project(stella_md)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

#set(CMAKE_C_COMPILER "/opt/gcc-9.1.0/bin/gcc-9.1")
#set(CMAKE_CXX_COMPILER "/opt/gcc-9.1.0/bin/g++-9.1")

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

################################################################################
# Find ament packages and libraries for ament and system dependencies
################################################################################
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(tf2_msgs REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(tf2 REQUIRED)

################################################################################
# Build
################################################################################
include_directories(
  include
    ${catkin_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME}_node src/main.cpp src/MwSerial.cpp src/SerialCOM.cpp src/UsbPacket.cpp src/UsbSerial.cpp src/MDC24D100D.cpp src/MwIEEE754.cpp src/mobilerobot.cpp)
ament_target_dependencies(${PROJECT_NAME}_node
  "rclcpp"
  "geometry_msgs"
  "nav_msgs"
  "tf2_msgs"
  "tf2_ros"
  "tf2"
)
#target_link_libraries(${PROJECT_NAME}_node ${CMAKE_CURRENT_SOURCE_DIR}/lib/pi_mw24d100d.a)

################################################################################
# Install
################################################################################
install(
  DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}/
)

install(TARGETS
  ${PROJECT_NAME}_node
  DESTINATION lib/${PROJECT_NAME})


################################################################################
# Macro for ament package
################################################################################
ament_export_include_directories(include)
ament_package()
