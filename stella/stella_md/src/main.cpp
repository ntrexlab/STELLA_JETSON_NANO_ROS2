#include "main.hpp"
#include "mobilerobot.h"
 
stellaN1_node::stellaN1_node():Node("stella_md_node")
{
  auto qos = rclcpp::QoS(rclcpp::KeepLast(10));

  cmd_vel_sub_ = this->create_subscription<geometry_msgs::msg::Twist>("cmd_vel",10,std::bind(&stellaN1_node::command_velocity_callback,this,std::placeholders::_1));
  odom_pub_ = this->create_publisher<nav_msgs::msg::Odometry>("odom", qos);
  odom_broadcaster = std::make_unique<tf2_ros::TransformBroadcaster>(*this);
  
  update_timer_  = this->create_wall_timer(10ms, std::bind(&stellaN1_node::update_write, this));
  update_timer_2 = this->create_wall_timer(5ms, std::bind(&stellaN1_node::update_read,  this));
}
stellaN1_node::~stellaN1_node()
{
  MDC24D_move(id,0,0);
}
void stellaN1_node::update_read()
{
  update_odometry();
}

void stellaN1_node::update_write()
{
  MDC24D_move(id,left_rpm,right_rpm);
}

void *motor_read_thread(void *arg)
{
    while (run)
    {
        MDC24D_read(id,encoder);
    }
}

void stellaN1_node::command_velocity_callback(const geometry_msgs::msg::Twist::SharedPtr cmd_vel_msg)
{
  goal_linear_velocity_ = cmd_vel_msg->linear.x;
  goal_angular_velocity_ = cmd_vel_msg->angular.z;
  
  calculate_wheel_vel(cmd_vel_msg->linear.x, cmd_vel_msg->angular.z, &left_rpm, &right_rpm);

}

bool stellaN1_node::update_odometry()
{  
  delta_left  = (encoder[0] - left_encoder_prev) *-1;
  delta_right = (encoder[1] - right_encoder_prev) *-1;

//  if(delta_left  != 0 || delta_right != 0) printf("delta_left : %d      delta_right : %d     \n",delta_left,delta_right);

  delta_s = -1*(delta_left + delta_right) / 2.0 / pulse_per_distance;
  delta_th = ((delta_left - delta_right) / wheel_to_wheel_d / pulse_per_distance);
  delta_x = (delta_s * cos(th + delta_th / 2.0));
  delta_y = (delta_s * sin(th + delta_th / 2.0));
  
  x -= delta_x;
  y -= delta_y;
  th += delta_th;
   
  tf2::Quaternion Quaternion;
  Quaternion.setRPY(0,0,th);
  
  nav_msgs::msg::Odometry odom;
  
  odom.pose.pose.orientation.x = Quaternion.x();
  odom.pose.pose.orientation.y = Quaternion.y();
  odom.pose.pose.orientation.z = Quaternion.z();
  odom.pose.pose.orientation.w = Quaternion.w();
  
  geometry_msgs::msg::TransformStamped t;
  rclcpp::Time time_now = this->now();
  
  t.header.stamp = time_now;
  t.header.frame_id = "odom";
  t.child_frame_id = "base_footprint";
  
  t.transform.translation.x = x;
  t.transform.translation.y = y;
  t.transform.translation.z = 0.0;
  
  t.transform.rotation.x= Quaternion.x();
  t.transform.rotation.y= Quaternion.y();
  t.transform.rotation.z= Quaternion.z();
  t.transform.rotation.w= Quaternion.w();
  
  
  odom_broadcaster->sendTransform(t);


  odom.header.frame_id = "odom";

  odom.pose.pose.position.x = x;
  odom.pose.pose.position.y = y;
  odom.pose.pose.position.z = 0.0;

  odom.child_frame_id = "base_footprint";
  odom.twist.twist.linear.x = goal_linear_velocity_;
  odom.twist.twist.angular.z = goal_angular_velocity_;
  
  odom.header.stamp = time_now;
  odom_pub_->publish(odom);
  
  left_encoder_prev = encoder[0];
  right_encoder_prev = encoder[1];
  

  return true;
}

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  pthread_t thread;

  MW_SerialOpen("/dev/MW",115200);
  sleep(1.0);
  printf("Connect Sucess\n");
  pthread_create(&thread, NULL,motor_read_thread, NULL);
  
  rclcpp::spin(std::make_shared<stellaN1_node>());
  run = false;
  pthread_join(thread,NULL);
  
  Mw_SerialClose();
  rclcpp::shutdown();
  return 0;
}
