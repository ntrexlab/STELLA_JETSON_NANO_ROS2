#include "MwSerial.hpp"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

static UsbSerial *_MwSerial;
static SerialCOM *_MwSerial2;

int MW_SerialOpen(char *Port, int BaudRate)
{
    if (_MwSerial2->Usb_Serial_Open(Port, BaudRate) == -1)
    {
        return -1;
    }

    UsbSerial *ci = new UsbSerial();
    return 0;
}

int MW_SerialRecv(long *ID, int *length, char data[8])
{
    SerialMessage msg;

    if (_MwSerial->ReceiveSerialMessage(msg))
    {
        *ID = msg.ID;
        *length = msg.length;
        memcpy(data, msg.data, msg.length);

        return 1;
    }
    return 0;
}

int MW_SerialSend(long ID, char data[8])
{
        SerialMessage_TX msg;

        msg.ID = ID;
        msg.length = 8;

        memcpy(msg.data, data, min(8, msg.length));

        if (_MwSerial->TransmitSerialMessage(msg))
        {
            return 1;
        }
    return 0;
}

int Mw_SerialClose(void)
{
	_MwSerial2->Close();

    delete _MwSerial;
    delete _MwSerial2;

	return 0;
}
