#pragma once

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


#include "SerialCOM.hpp"
#include "SerialTypedf.hpp"

#define STX 0x02
#define ETX 0x03

class SerialCom;

class UsbPacket
{
private:
  
public:
    UsbPacket();
    ~UsbPacket();
    
    int _RecvPacket(char *packet, int length);
    bool RecvPacket(Usb2Packet &pkt);
    bool SendPacket(Usb2Packet_TX *mpkt, int no_pkt);
    char checkSum(char *packet, int start_length, int end_length);
    int FindMove(char *packet, int length, char stx);
    

protected:
    SerialCOM* _bus;
};





