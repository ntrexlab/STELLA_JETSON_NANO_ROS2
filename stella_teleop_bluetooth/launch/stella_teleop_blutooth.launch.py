#!/usr/bin/python3

from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='stella_teleop_bluetooth',
            executable='stella_teleop_bluetooth_node',
            name='stella_teleop_bluetooth_node',
            output='screen'
        )
    ])

