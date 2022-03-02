#!/usr/bin/python3

from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='stella_md',
            executable='stella_md_node',
            name='stella_md_node',
            output='screen'
        )
    ])


