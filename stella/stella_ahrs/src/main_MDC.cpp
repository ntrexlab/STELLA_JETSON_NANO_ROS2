#include "MwSerial.hpp"
#include "MDC24D100D.hpp"


    //printf("RECV ID : %d length : %d [%02x %02x %02x %02x %02x %02x %02x %02x] \n", id,length,(unsigned char)(int)data[0], (unsigned char)(int)data[1], (unsigned char)(int)data[2] ,(unsigned char)(int)data[3],
          //  (unsigned char)(int)data[4] ,(unsigned char)(int)data[5] ,(unsigned char)(int)data[6] ,(unsigned char)(int)data[7]);


int main(void)
{
    int encoder[2] = {
        0,
    },
        id = 1, left_encoder_prev = 0, right_encoder_prev = 0;

    int delat_left = 0, delat_right = 0;

    float L_rpm = 10.0, R_rpm = 10.0;

    MW_SerialOpen("/dev/ttyUSB0",115200);

    sleep(0.5); //wait

    printf("Connect success\n");

    while (true)
    {
        MDC24D_rpm_to_pulse(id, L_rpm, R_rpm, encoder);

        delat_left  = (encoder[0] - left_encoder_prev);
        delat_right = (encoder[1] - right_encoder_prev);

        if (delat_left != 0 && delat_right != 0)
        {
            printf("delat_left : %d delat_right : %d   \n", delat_left, delat_right);
        }

        left_encoder_prev = encoder[0];
        right_encoder_prev = encoder[1];
    }
    return 0;
}