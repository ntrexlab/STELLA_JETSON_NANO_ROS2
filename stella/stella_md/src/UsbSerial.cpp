#include <stdio.h>
#include <pthread.h>

#include "UsbSerial.hpp"
#include "SerialTypedf.hpp"
#include "UsbPacket.hpp"

#include <unistd.h>
#include <mutex>
#include <iostream>

#define MAX_WRITE_MESSAGE 13 //bin max length

deque<Usb2Packet_TX> _queueTransmit;

bool mutex_rx = true;
bool mutex_tx = true;

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



void *UsbSerial::ThreadReceive()
{
    while (!_stopReqThreadReceive)
    {
        Usb2Packet pkt;

        if (mutex_rx = true && _bus->RecvPacket(pkt))
        {
            _queueReceive.push_back(pkt);
            mutex_rx  = false;
        }
    }
    return 0;
}

bool UsbSerial::ReceiveSerialMessage(SerialMessage &msg)
{
    if (_queueReceive.size() > 0)
    {
        switch(mutex_rx)
        {
            case false:
            {
                Usb2Packet &pkt = _queueReceive.front();
                msg = pkt.msg;
                _queueReceive.pop_front();
                mutex_rx = true;
                return true;
            }
            break;

            case true:
            {
                return false;   
            }
            break;
        }
    }
    return false;
}

void *UsbSerial::ThreadTransmit()
{
    while (!_stopReqThreadTransmit)
    {
        int no_pkt = min(MAX_WRITE_MESSAGE, (int)_queueTransmit.size());

        if (no_pkt > 0)
        {
            Usb2Packet_TX mpkt[MAX_WRITE_MESSAGE];
        
            for (int i = 0; i < no_pkt; i++)
            {
                mpkt[i] = _queueTransmit.front();
            } 
            if(mutex_tx == false)
            {
                _queueTransmit.pop_front();
                mutex_tx = true;
            } 
            _bus->SendPacket(mpkt,no_pkt);
        }
    }
    return 0;
}

bool UsbSerial::TransmitSerialMessage(SerialMessage_TX &msg)
{  
    if(mutex_tx == true)
    {
        Usb2Packet_TX pkt;
        pkt.msg = msg;
        _queueTransmit.push_back(pkt);

        mutex_tx = false;
    }
    return true;
}
