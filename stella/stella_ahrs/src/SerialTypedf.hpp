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

struct SerialMessage_TX_str
{
    char data[20];
};



struct Usb2Packet
{
    double time_recv;
    union 
    {
        SerialMessage msg;
    };
    
};

struct Usb2Packet_TX
{
    double time_recv;
    union 
    {
        SerialMessage_TX msg;
    };
    
};

struct Usb2Packet_TX_str
{
    double time_recv;
    union 
    {
        SerialMessage_TX_str msg;
    };
    
};




