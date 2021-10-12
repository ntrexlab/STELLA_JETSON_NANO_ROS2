#!/bin/bash

echo "remap the devices serial port(ttyUSBX, ttySX) to  ydlidar, AHRS, Motordriver, Bluetooth"
echo "devices usb connection as /dev/YDLIDAR, /dev/AHRS, /dev/MW, /dev/BT  , check it using the command : ls -l /dev|grep -e ttyUSB -e ttyS0"
echo "start copy stella.rules to  /etc/udev/rules.d/"
echo "$HOME/colcon_ws/src/STELLA_ROS2_RASPBERRYPI/stella_bringup/stella.rules"
sudo cp $HOME/colcon_ws/src/STELLA_ROS2_RASPBERRYPI/stella_bringup/stella.rules  /etc/udev/rules.d
echo " "
echo "Restarting udev"
echo ""
sudo udevadm trigger
echo "finish "
