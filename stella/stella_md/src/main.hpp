#include "MwSerial.hpp"
#include "MDC24D100D.hpp"

#include <tf2/LinearMath/Quaternion.h>
#include <rclcpp/rclcpp.hpp>
#include <chrono>
#include <memory>
#include <math.h>
#include <string.h>
#include <vector>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <pthread.h>

#include "geometry_msgs/msg/transform_stamped.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "tf2_msgs/msg/tf_message.hpp"
#include "tf2_ros/transform_broadcaster.h"

#define LEFT 0
#define RIGHT 1
using namespace std::chrono_literals;
using std::placeholders::_1;

bool run = true;
int id = 1;
    float left_rpm = 0, right_rpm = 0;
    int encoder[2] = {0,};

class stellaN1_node : public rclcpp::Node
{
public:
    stellaN1_node();
    ~stellaN1_node();

private:

    // ROS timer
    rclcpp::TimerBase::SharedPtr update_timer_;
    rclcpp::TimerBase::SharedPtr update_timer_2;
    rclcpp::TimerBase::SharedPtr update_timer_3;
    // ROS topic publishers
    rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr odom_pub_;

    // ROS topic subscribers
    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_sub_;
    
    std::unique_ptr<tf2_ros::TransformBroadcaster> odom_broadcaster;
    
    double goal_linear_velocity_;
    double goal_angular_velocity_;

   
    
    
    int left_encoder = 0, right_encoder = 0,delta_left = 0,delta_right = 0,left_encoder_prev=0,right_encoder_prev=0;
    double delta_th=0.0,delta_s=0.0,delta_x=0.0,delta_y=0.0,x=0.0,y=0.0,th=0.0;

    void command_velocity_callback(const geometry_msgs::msg::Twist::SharedPtr cmd_vel_msg);
    void update_read();
    void update_write();
    bool update_odometry();
};
