#include "MDC24D100D.hpp"
#include "MwSerial.hpp"

void MDC24D_move(int ID, float L_rpm, float R_rpm)
{
    int8_t L_rpm_data[4] = {
        0,
    },
           R_rpm_data[4] = {
               0,
           };

    
    int length = 0;

    int left_encoder = 0, right_encoder = 0;

    Binary2Hex(GetBinary32((float)L_rpm), L_rpm_data);
    Binary2Hex(GetBinary32((float)R_rpm), R_rpm_data);

    unsigned char Ldata[8] = {0x1C, 0x70, 0x00, 0x01, L_rpm_data[0], L_rpm_data[1], L_rpm_data[2], L_rpm_data[3]};
    unsigned char Rdata[8] = {0x1C, 0x70, 0x00, 0x02, R_rpm_data[0], R_rpm_data[1], R_rpm_data[2], R_rpm_data[3]};
    
    unsigned char sdata_1[8] = {0x38, 0x7D, 0x00,0x01, 0x00, 0x00, 0x00, 0x00};
    unsigned char sdata_2[8] = {0x38, 0x7D, 0x00,0x02, 0x00, 0x00, 0x00, 0x00};

    unsigned char claer_data[8] = {0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00};

    MW_SerialSend(ID, (char *)sdata_1);
    sleep(0.01);
    MW_SerialSend(ID, (char *)sdata_2);
    sleep(0.01);
    MW_SerialSend(ID, (char *)Ldata);
    sleep(0.01);
    MW_SerialSend(ID, (char *)Rdata);
    sleep(0.01);
}

void MDC24D_read(int ID,int pulse[2])
{
    long id = 0;
    int length = 0;
    char data[8];

    if (MW_SerialRecv(&id, &length, data))
    {
        switch ((unsigned char)(int)data[1])
        {
            case 0x7d:
                switch ((unsigned char)(int)data[3])
                {
                    case 0x01:

                    pulse[0] = ((unsigned char)(int)data[4] | (unsigned char)(int)data[5] << 8 | (unsigned char)(int)data[6] << 16 | (unsigned char)(int)data[7] << 24);

                    break;

                    case 0x02:

                    pulse[1] = ((unsigned char)(int)data[4] | (unsigned char)(int)data[5] << 8 | (unsigned char)(int)data[6] << 16 | (unsigned char)(int)data[7] << 24);

                    break;
                }
            break;   
        }
    }       
}
