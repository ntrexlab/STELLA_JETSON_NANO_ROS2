#include <deque>

#include "UsbPacket.hpp"
#include "SerialCOM.hpp"

#include "Lock.hpp"

using namespace std;
typedef unsigned int UINT;
typedef void *LPVOID;

static deque<Usb2Packet> _queueReceive;
static deque<Usb2Packet_TX> _queueTransmit;

class SerialCoM;

class UsbSerial
{
public:
    UsbSerial();
    ~UsbSerial();

    pthread_t ThreadTransmit_;
    pthread_t ThreadReceive_;

    void ThreadStart();
    void ThreadStop();
    void *ThreadTransmit();
    void *ThreadReceive();
    static void *ThreadReceiveStatic(LPVOID pParam);
    static void *ThreadTransmitStatic(LPVOID pParam);
    bool ReceiveSerialMessage(SerialMessage &pkt);
    bool TransmitSerialMessage(SerialMessage_TX &pkt);

    bool _stopReqThreadTransmit;
    bool _stopReqThreadReceive;

    

protected:
    UsbPacket *_bus;
    SerialCOM *_bus2;

private:
    
    CLock _lockReceive;
    CLock _lockTransmit;

};
