#pragma once

#define BIN_PACKET_LEN		13

struct SerialMessage
{
    int length;
    long ID;
    char data[8];
};

struct SerialMessage_TX
{
    int length;
    long ID;
    char data[8];
};

struct Usb2Packet
{
    union 
    {
        SerialMessage msg;
    }; 
};

struct Usb2Packet_TX
{
    union 
    {
        SerialMessage_TX msg;
    };
};
    




