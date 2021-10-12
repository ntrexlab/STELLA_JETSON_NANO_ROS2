#include <stdio.h>
#include <pthread.h>

#include "UsbSerial.hpp"
#include "SerialTypedf.hpp"
#include "UsbPacket.hpp"

#include <unistd.h>

#define MAX_WRITE_MESSAGE 13 //bin max length

#define STR_MAX_WRITE_MESSAGE 20 //str max length

UsbSerial::UsbSerial()
{
    _bus = new UsbPacket();

    _stopReqThreadTransmit = false;
    _stopReqThreadReceive = false;

    ThreadStart();
}

UsbSerial::~UsbSerial()
{
    _stopReqThreadTransmit = true;
    _stopReqThreadReceive = true;

    ThreadStop();

    delete _bus;
}

void UsbSerial::ThreadStart()
{
    pthread_create(&ThreadTransmit_, NULL, &UsbSerial::ThreadTransmitStatic, this);
    pthread_create(&ThreadReceive_, NULL, &UsbSerial::ThreadReceiveStatic, this);
}

void UsbSerial::ThreadStop()
{
    pthread_join(ThreadTransmit_, NULL);
    pthread_join(ThreadReceive_, NULL);
}

void *UsbSerial::ThreadTransmitStatic(LPVOID pParam)
{
    return ((UsbSerial *)pParam)->ThreadTransmit();
}

void *UsbSerial::ThreadReceiveStatic(LPVOID pParam)
{
    return ((UsbSerial *)pParam)->ThreadReceive();
}

void *UsbSerial::ThreadTransmit()
{
    while (!_stopReqThreadTransmit)
    {

            int no_pkt = min(MAX_WRITE_MESSAGE, (int)_queueTransmit.size());

            if (no_pkt > 0)
            {
                sleep(0.0000001);
                Usb2Packet_TX mpkt[MAX_WRITE_MESSAGE];

                for (int i = 0; i < no_pkt; i++)
                {
                    mpkt[i] = _queueTransmit.front();
                    _queueTransmit.pop_front();
                }
                
                sleep(0.0000001);
                _bus->SendPacket(mpkt,no_pkt);
            }
    }
    return 0;
}

void *UsbSerial::ThreadReceive()
{
    while (!_stopReqThreadReceive)
    {
        Usb2Packet pkt;
    
        if (_bus->RecvPacket(pkt))
        {
            _lockReceive.Lock();
            _queueReceive.push_back(pkt);
            _lockReceive.Unlock();
        }
    }

    return 0;
}

bool UsbSerial::ReceiveSerialMessage(SerialMessage &msg)
{
    if (_queueReceive.size() > 0)
    {
        sleep(0.000001);

        Usb2Packet &pkt = _queueReceive.front();
        msg = pkt.msg;
        _queueReceive.pop_front();

        return true;
    }
    return false;
}

bool UsbSerial::TransmitSerialMessage(SerialMessage_TX &msg)
{
    sleep(0.00000001);
    Usb2Packet_TX pkt;

    pkt.msg = msg;

    _queueTransmit.push_back(pkt);
    sleep(0.00000001);
    return true;
}
