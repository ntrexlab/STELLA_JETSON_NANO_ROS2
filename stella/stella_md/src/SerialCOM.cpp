#include "SerialCOM.hpp"

#define SerialMaxBuff 13 //MW_AHRS

volatile static int serial_port;

SerialCOM::SerialCOM()
{

}
SerialCOM::~SerialCOM()
{
    close(serial_port);
}

int SerialCOM::Usb_Serial_Open(char *str, int BaudRate)
{
    struct termios newtio, oldtio;

    serial_port = open(str, O_RDWR | O_NONBLOCK | O_NDELAY);

    if (serial_port < 0)
    {
        printf("Error %i from open: %s\n", errno, strerror(errno));
        return -1;
    }

    newtio.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR | ICRNL;;
    newtio.c_oflag = 0;

    newtio.c_cflag &= ~CSIZE;
    newtio.c_cflag |= CS8;
    newtio.c_lflag = 0;

    newtio.c_cc[VINTR]    = 0;     /* Ctrl-c */
    newtio.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
    newtio.c_cc[VERASE]   = 0;     /* del */
    newtio.c_cc[VKILL]    = 0;     /* @ */
    newtio.c_cc[VEOF]     = 4;     /* Ctrl-d */
    newtio.c_cc[VTIME]    = 5;     /* inter-character timer unused */
    newtio.c_cc[VMIN]     = 0;     /* blocking read until 1 character arrives */
    newtio.c_cc[VSWTC]    = 0;     /* '\0' */
    newtio.c_cc[VSTART]   = 0;     /* Ctrl-q */
    newtio.c_cc[VSTOP]    = 0;     /* Ctrl-s */
    newtio.c_cc[VSUSP]    = 0;     /* Ctrl-z */
    newtio.c_cc[VEOL]     = 0;     /* '\0' */
    newtio.c_cc[VREPRINT] = 0;     /* Ctrl-r */
    newtio.c_cc[VDISCARD] = 0;     /* Ctrl-u */
    newtio.c_cc[VWERASE]  = 0;     /* Ctrl-w */
    newtio.c_cc[VLNEXT]   = 0;     /* Ctrl-v */
    newtio.c_cc[VEOL2]    = 0;     /* '\0' */

    tcflush(serial_port, TCIFLUSH);
    tcsetattr(serial_port,TCSANOW,&newtio);

    if (tcsetattr(serial_port, TCSANOW, &newtio) != 0)
    {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        return -1;
    }
}

int SerialCOM::__Read(char *buff)
{
    unsigned char buf[13];

    int bytes = read(serial_port, buf, SerialMaxBuff);

    if (bytes > 0)
    {
        /*
        for(int i=0; i<13; i++)
        {
            printf("%02x ",buf[i]);
        }
        printf("\n");
        */

        memcpy(buff, buf, bytes);

        return bytes;
    }
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
