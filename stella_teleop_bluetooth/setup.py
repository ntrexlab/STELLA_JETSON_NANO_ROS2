import os
from glob import glob
from setuptools import setup

package_name = 'stella_teleop_bluetooth'

setup(
    name=package_name,
    version='0.0.0',
    packages=[package_name],
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        (os.path.join('share', package_name), glob('launch/*.launch.py')),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='ntrex',
    maintainer_email='lab@ntrex.co.kr',
    description='STELLA teleoperation node by bluetooth',
    license='Apache License 2.0',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'stella_teleop_bluetooth_node = stella_teleop_bluetooth.stella_teleop_bluetooth:main',
        ],
    },
)
