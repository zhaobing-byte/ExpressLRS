#ifndef H_SBUS
#define H_SBUS

#include "SBUS.h"

#define SBUS_RX_BAUDRATE 100000

#define SBUS_HEAD_BYTE 0x0F
#define SBUS_END_BYTE 0x00

#define SBUSframeLength 25   
//ESP8266 uart tx invert output   
#define UART0_INVERT 0
#define REG_UART0_BASE 0x60000000 
#define UART0_CONF0 (REG_UART0_BASE + 0x20)
#define UART0_TXD_INV (BIT(22))


class SBUS
{
public:
    #if CRSF_RX_MODULE

    SBUS(Stream *dev) : _dev(dev)
    {
    }

    SBUS(Stream &dev) : _dev(&dev) {}

    #endif

    void sendRCFrameToFC();
    void sendLinkStateToFC();
    unsigned AUX12 : 11;
private:
    Stream *_dev;    
};


#endif 
