#include "MDC24D100D.hpp"
#include "MwSerial.hpp"

bool Mw_AHRS_init(int ID)
{
    unsigned char st_data[8] = {0x18, 0x19, 0x00, 0x00, 0x00,0x00, 0x00, 0x00}; //바이너리로 전송되게 함
    unsigned char sp_data[8] = {0x18, 0x18, 0x00, 0x00, 0x14,0x00, 0x00, 0x00}; //20ms
    unsigned char ss_data[8] = {0x18, 0x15, 0x00, 0x00, 0x07,0x00, 0x00, 0x00}; //acc,gyr,deg

    MW_SerialSend(ID, (char *)st_data);
    sleep(0.2);
    MW_SerialSend(ID, (char *)sp_data);
    sleep(0.2);
    MW_SerialSend(ID, (char *)ss_data);
    sleep(0.2);

    return true;
}