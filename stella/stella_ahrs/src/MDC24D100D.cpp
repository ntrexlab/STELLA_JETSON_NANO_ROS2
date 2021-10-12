#include "MDC24D100D.hpp"
#include "MwSerial.hpp"

void MDC24D_rpm_to_pulse(int ID, float L_rpm, float R_rpm, int pulse[2])
{
    int8_t L_rpm_data[4] = {
        0,
    },
           R_rpm_data[4] = {
               0,
           };

    char data[8];
    long id = 0;
    int length = 0;

    int left_encoder = 0, right_encoder = 0;

    Binary2Hex(GetBinary32((float)L_rpm), L_rpm_data);
    Binary2Hex(GetBinary32((float)R_rpm), R_rpm_data);

    unsigned char Ldata[8] = {0x1C, 0x70, 0x00, 0x01, L_rpm_data[0], L_rpm_data[1], L_rpm_data[2], L_rpm_data[3]};
    unsigned char Rdata[8] = {0x1C, 0x70, 0x00, 0x02, R_rpm_data[0], R_rpm_data[1], R_rpm_data[2], R_rpm_data[3]};

    MW_SerialSend(ID, (char *)Ldata);
    MW_SerialSend(ID, (char *)Rdata);

    for (int i = 0; i < 5; i++)
    {
        for (int i = 1; i <= 2; i++)
        {
            unsigned char sdata[8] = {0x38, 0x7D, 0x00, i, 0x00, 0x00, 0x00, 0x00};

            MW_SerialSend(1, (char *)sdata);

            if (MW_SerialRecv(&id, &length, data))
            {
                switch ((unsigned char)(int)data[1])
                {
                case 0x7d:
                    switch ((unsigned char)(int)data[3])
                    {
                    case 1:

                        pulse[0] = ((unsigned char)(int)data[4] | (unsigned char)(int)data[5] << 8 | (unsigned char)(int)data[6] << 16 | (unsigned char)(int)data[7] << 24);

                        break;

                    case 2:

                        pulse[1] = ((unsigned char)(int)data[4] | (unsigned char)(int)data[5] << 8 | (unsigned char)(int)data[6] << 16 | (unsigned char)(int)data[7] << 24);

                        break;
                    }
                    break;
                }
            }
        }
    }
}