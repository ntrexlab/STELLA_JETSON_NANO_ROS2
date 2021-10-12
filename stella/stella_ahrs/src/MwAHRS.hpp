#define ACC 0x33
#define GYO 0x34
#define DEG 0x35

int16_t acc_x = 0, acc_y = 0, acc_z = 0, gyo_x = 0, gyo_y = 0, gyo_z = 0, deg_x = 0, deg_y = 0, deg_z = 0;

bool Mw_AHRS_init(int ID);