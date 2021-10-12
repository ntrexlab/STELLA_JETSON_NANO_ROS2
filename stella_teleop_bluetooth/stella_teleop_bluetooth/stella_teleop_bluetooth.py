#! /usr/bin/env python3

import serial
import time
import rclpy
from rclpy.node import Node

from geometry_msgs.msg import Twist

class TeleopBluetooth(Node):

  def __init__(self):
    self.serial_port = serial.Serial(
      port="/dev/BT",
      baudrate=9600,
      bytesize=serial.EIGHTBITS,
      parity=serial.PARITY_NONE,
      stopbits=serial.STOPBITS_ONE,
    )
    
    time.sleep(1)
      
    super().__init__('stella_teleop_bluetooth_node')
      
    self.pub = self.create_publisher(Twist, 'cmd_vel', 10)
      
    timer_period = 0.1
      
    self.timer = self.create_timer(timer_period, self.timer_callback)
    
    self.target_linear_vel   = 0.0
    self.target_angular_vel  = 0.0
    
  def timer_callback(self):
    if self.serial_port.inWaiting() > 0:
      data = self.serial_port.readline()
      a = data[0:5]
      a = str(a)
      a = a.split("/")
      a[0] = a[0].replace("b'","")
      a[1] = a[1].replace("'","")
      
      if a[0] == 'F':
        self.target_linear_vel = float(a[1]) * 1
      elif a[0] == 'B':
        self.target_linear_vel = float(a[1]) * -1
      elif a[0] == 'L':
        self.target_angular_vel = float(a[1]) * 1.57
      elif a[0] == 'R':
        self.target_angular_vel = float(a[1]) * -1.57
      elif a[0] == 'S':
        self.target_linear_vel  = 0.0
        self.target_angular_vel = 0.0
									
    else:
      self.target_linear_vel  = 0.0
      self.target_angular_vel = 0.0		
      
    twist = Twist()
    
    twist.linear.x = self.target_linear_vel
    twist.linear.y = 0.0
    twist.linear.z = 0.0
    
    twist.angular.x = 0.0
    twist.angular.y = 0.0
    twist.angular.z = self.target_angular_vel
    
    self.pub.publish(twist)
    
    
def main(args=None):
  
  rclpy.init(args=args)
  teleop_bluetooth = TeleopBluetooth()
  try:
      rclpy.spin(teleop_bluetooth)
  except KeyboardInterrupt:
      teleop_bluetooth.get_logger().info("Exiting Program")
  except Exception as exception_error:
      teleop_bluetooth.get_logger().info("Error occurred. Exiting Program")
      teleop_bluetooth.get_logger().info("Error: " + str(exception_error))
  finally:
      teleop_bluetooth.destroy_node()
      rclpy.shutdown()
  
if __name__ == '__main__':
  main()
