#!/usr/bin/python3

from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='stella_ahrs',
            executable='stella_ahrs_node',
            name='stella_ahrs_node',
            output='screen'
        )
    ])


