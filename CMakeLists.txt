cmake_minimum_required(VERSION 3.8)
project(glovebox_bt)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find required packages
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(behaviortree_cpp REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(tf2_eigen REQUIRED)
find_package(moveit_core REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(ament_index_cpp REQUIRED)
find_package(moveit_ros_planning_interface REQUIRED)
find_package(force_controller REQUIRED)

# include_directories(include)

set(
  THIS_PACKAGE_INCLUDE_DEPENDS 
  pluginlib
  
  # below: added from hybrid force control
  force_controller
  geometry_msgs
  tf2
  tf2_ros
  tf2_geometry_msgs
  tf2_eigen
  moveit_core
  moveit_ros_planning_interface
  sensor_msgs

  rclcpp
  behaviortree_cpp
)

add_executable(run_glovebox_tree src/run_glovebox_tree.cpp)
add_executable(run_hybrid_multidim_control src/run_hybrid_multidim_control.cpp)

add_library(glovebox_bt
  src/behaviors/move_arm_to_pose.cpp
  src/behaviors/hybrid_multidim_control.cpp
  src/utils/param_handler.cpp
  src/utils/utils.cpp
  # src/run_glovebox_tree.cpp
  # src/run_hybrid_multidim_control.cpp
)

target_link_libraries(run_glovebox_tree glovebox_bt)
target_link_libraries(run_hybrid_multidim_control glovebox_bt)

ament_target_dependencies(glovebox_bt ${THIS_PACKAGE_INCLUDE_DEPENDS})
# ament_target_dependencies(run_hybrid_multidim_control ${THIS_PACKAGE_INCLUDE_DEPENDS})

# Specify header search path (run_glovebox_tree?)
target_include_directories(glovebox_bt PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)


# Install executable
install(
  TARGETS glovebox_bt
  run_glovebox_tree
  run_hybrid_multidim_control
  DESTINATION lib/${PROJECT_NAME}
)

# Install behavior trees
install(
  DIRECTORY behavior_trees/
  DESTINATION share/${PROJECT_NAME}/behavior_trees
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
