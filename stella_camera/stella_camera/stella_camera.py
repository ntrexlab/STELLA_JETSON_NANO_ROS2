#! /usr/bin/env python3

import time
import rclpy
from rclpy.node import Node

import cv2
import os

from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError
 
class Camera(Node):

  def __init__(self):
      
    super().__init__('stella_camera_node')
      
    self.pub = self.create_publisher(Image, 'camera', 1)
      
    timer_period = 0.1
      
    self.timer = self.create_timer(timer_period, self.timer_callback)
    
    os.system('sudo modprobe bcm2835-v4l2')
    self.cap = cv2.VideoCapture(0)
    self.cap.set(cv2.CAP_PROP_FRAME_WIDTH,320)
    self.cap.set(cv2.CAP_PROP_FRAME_HEIGHT,240)
    
    self.bridge = CvBridge()
   
  def timer_callback(self):
      
    ret, frame = self.cap.read()
    if not ret:
        self.get_logger().info("Not Found Devices")
    frame = cv2.flip(frame,0)
    frame = cv2.flip(frame,1)
    self.pub.publish(self.bridge.cv2_to_imgmsg(frame,"bgr8"))


  
def main(args=None):
  
  rclpy.init(args=args)  
  camera = Camera()
  try:
      rclpy.spin(camera)
  except KeyboardInterrupt:
      camera.get_logger().info("Exiting Program")
  except Exception as exception_error:
      camera.get_logger().info("Error occurred. Exiting Program")
      camera.get_logger().info("Error: " + str(exception_error))
  finally:
      camera.cap.release()
      camera.destroy_node()
      rclpy.shutdown()
  
if __name__ == '__main__':
  main()
