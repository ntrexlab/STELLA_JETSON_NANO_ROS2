#!/usr/bin/python3

from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='stella_camera',
            executable='stella_camera_node',
            name='stella_camera_node',
            output='screen'
        )
    ])


