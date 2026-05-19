// testing
#include <behaviortree_cpp/bt_factory.h>
#include <behaviortree_cpp/tree_node.h>
#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include "tf2_ros/transform_listener.h"
#include <tf2_ros/buffer.h>
#include "ament_index_cpp/get_package_share_directory.hpp"

// custom nodes
#include "glovebox_bt/behaviors/hybrid_multidim_control.hpp"

int main(int argc, char **argv)
{
    // Initialize ROS2 client library
    rclcpp::init(argc, argv);
    rclcpp::NodeOptions node_options;

    // Create the shared node for ALL BT actions
    auto node = rclcpp::Node::make_shared("glovebox_bt");

    // rclcpp::executors::MultiThreadedExecutor exec;
    // exec.add_node(node);
    // std::thread ros_spin([&exec]() {
    //     exec.spin();
    // });

    BT::BehaviorTreeFactory factory;
    // auto tf_buffer = std::make_shared<tf2_ros::Buffer>(std::make_shared<rclcpp::Clock>());
    auto tf_buffer = std::make_shared<tf2_ros::Buffer>(node->get_clock());
    auto tf_listener = std::make_shared<tf2_ros::TransformListener>(*tf_buffer);

    // Register custom nodes
    factory.registerBuilder<hybrid_multidim_control::HybridMultidimControl>(
        "HybridMultidimControl",
          [&](const std::string& name, const BT::NodeConfig& config) {
        return std::make_unique<hybrid_multidim_control::HybridMultidimControl>(
        name, config, node, tf_buffer);
        });
    

    // Load BehaviorTree from installed file
    std::string share_path = ament_index_cpp::get_package_share_directory("glovebox_bt");
    std::string xml_path = share_path + "/behavior_trees/hybrid_multidim_control.xml";

    // Create the tree
    auto bt = factory.createTreeFromFile(xml_path);

    //nrg_behaviors version
    BT::NodeStatus success = bt.tickWhileRunning();

    RCLCPP_INFO(node->get_logger(), "Tree execution finished with status %s", BT::toStr(success, true).c_str());
    
    // rclcpp::spin(node);

    rclcpp::shutdown();
    return success != BT::NodeStatus::SUCCESS;

}
