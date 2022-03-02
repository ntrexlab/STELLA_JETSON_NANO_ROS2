#pragma once

#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>


class SerialCOM
{
private:

public:
    SerialCOM();
    ~SerialCOM();

    int Usb_Serial_Open(char *str, int BaudRate,int pi);
    int __Read(char *buff);
    int __Write(unsigned char *buff, int length);
    void Close();
};
