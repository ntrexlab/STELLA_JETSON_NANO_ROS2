import os
from glob import glob
from setuptools import setup

package_name = 'stella_camera'

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
    description='STELLA camera node',
    license='Apache License 2.0',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'stella_camera_node = stella_camera.stella_camera:main',
        ],
    },
)
