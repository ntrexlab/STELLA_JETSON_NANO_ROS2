#include "UsbPacket.hpp"

#define Meaning 5

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

UsbPacket::UsbPacket()
{
    _bus = new SerialCOM();
}

UsbPacket::~UsbPacket()
{
    _bus->Close();
    delete _bus;
}

int UsbPacket::FindMove(char *packet, int length, char stx)
{
    for (int i = 0; i < length; ++i)
    {
        if (packet[i] == stx)
        {
            memmove(&packet[0], &packet[i], length - i);
            
            return length - i;
        }
    }
    return 0;
}

int UsbPacket::_RecvPacket(char *packet, int length)
{
    int recv_count = 0;

    int no_read = _bus->__Read(packet);

    for (int i = 0; i < 5 && 0 < length; i++)
    {
        int no_read = _bus->__Read(packet);

        if (recv_count == 0 && packet[0] != STX)
        {
            no_read = FindMove(packet, no_read, STX);
        }
        length -= no_read;
        recv_count += no_read;
    }
    return recv_count;
}
bool UsbPacket::RecvPacket(Usb2Packet &pkt)
{
    int recv_count = 0;

    bool debug_ = false;

    char packet[BIN_PACKET_LEN] = {
        0,
    };

    int ret = _RecvPacket(packet, BIN_PACKET_LEN);

    if (ret < 0)
    {
        if(debug_ == true) printf("[ERROR] Can't read packet\n");
        return false;
    }
    else if (ret == 0)
    {
        return false;
    }
    else if (ret != BIN_PACKET_LEN)
    {
        if(debug_ == true) printf("[ERROR] Packet length error: %d != %d\n", ret, BIN_PACKET_LEN);
        return false;
    }

    else if (packet[0] != STX || packet[13 - 1] != ETX)
    {
        if(debug_ == true) printf("[ERROR] STX, ETX error\n");
        return false;
    }

    else if (packet[11] != checkSum(&packet[0], 2, 10))
    {
        if(debug_ == true) printf("[ERROR] packet checksum error\n");
        return false;
    }
    SerialMessage &msg = pkt.msg;

    msg.ID = packet[2];
    msg.length = ret-5;

    memcpy(msg.data, &packet[3], msg.length);

    return true;
}

bool UsbPacket::SendPacket(Usb2Packet_TX *mpkt, int no_pkt)
{
    int packet_len = no_pkt * BIN_PACKET_LEN;

    unsigned char *mpacket = new unsigned char[packet_len];

    memset(mpacket, 0, packet_len);

    for (int i = 0; i < no_pkt; i++)
    {
        unsigned char *packet = mpacket + i * BIN_PACKET_LEN;

        const Usb2Packet_TX &pkt = mpkt[i];
        const SerialMessage_TX &msg = pkt.msg;

        packet[0] = STX;
        packet[1] = 0x0d;
        packet[2] = pkt.msg.ID;

        memcpy(&packet[3], msg.data, msg.length);

        packet[11] = checkSum((char *)&packet[0], 2, 10);
        packet[12] = ETX;
    }

    _bus->__Write(mpacket, packet_len);
    delete[] mpacket;

    return true;
}

char UsbPacket::checkSum(char *packet, int start_length, int end_length)
{
    char cs = 0;

    for (int i = start_length; i < end_length + 1; i++)
    {
        cs += packet[i];
    }

    return cs;
}
