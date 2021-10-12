#include "SerialCOM.hpp"

#define SerialMaxBuff 13 //MW_AHRS

volatile static int serial_port;

SerialCOM::SerialCOM()
{
}
SerialCOM::~SerialCOM()
{
}

int SerialCOM::Usb_Serial_Open(char *str, int BaudRate,int pi)
{
    struct termios newtio, oldtio;

    serial_port = open(str, O_RDWR | O_NOCTTY);

    if (serial_port < 0)
    {
        printf("Error %i from open: %s\n", errno, strerror(errno));
        return -1;
    }

    newtio.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;

    newtio.c_cflag &= ~CSIZE;
    newtio.c_cflag |= CS8;
    newtio.c_lflag = 0;

    if(pi > 0)
    {
        newtio.c_cc[VTIME] = 1;
        newtio.c_cc[VMIN] = pi;
    }
    

    tcflush(serial_port, TCIFLUSH);

    if (tcsetattr(serial_port, TCSANOW, &newtio) != 0)
    {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        return -1;
    }
}

int SerialCOM::__Read(char *buff)
{
    unsigned char buf[20];

    int bytes = read(serial_port, buf, 13);

    if(bytes == 0 ) return 0;

    if (bytes > 0)
    {
        memcpy(buff, buf, bytes);

        return bytes;
    }
    return 0;
}

int SerialCOM::__Write(unsigned char *buff, int length)
{
    int ret = write(serial_port,buff,length);

    return 0;
}

void SerialCOM::Close()
{
    close(serial_port);
}
