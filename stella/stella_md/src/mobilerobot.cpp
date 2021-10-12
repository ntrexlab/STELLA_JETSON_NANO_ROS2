#include "mobilerobot.h"

void calculate_wheel_vel(float linear, float angular, float *left_rpm,  float *right_rpm)
{
    float left_speed = 0.0, right_speed = 0.0;

    left_speed =  (linear + wheel_to_wheel_d / 2.0 * angular) * motor_scale_const;
    right_speed = (linear - wheel_to_wheel_d / 2.0 * angular) * motor_scale_const;

    *left_rpm  = left_speed * -1;
    *right_rpm = right_speed * -1;
}
