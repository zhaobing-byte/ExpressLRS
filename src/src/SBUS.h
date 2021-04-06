#ifndef H_SBUS
#define H_SBUS

#include "SBUS.h"

#define SBUS_HEAD_BYTE 0x0F
#define SBUS_END_BYTE 0x00

#define SBUSframeLength 25       



class SBUS
{
public:
    #if CRSF_RX_MODULE

    SBUS(Stream *dev) : _dev(dev)
    {
    }

    SBUS(Stream &dev) : _dev(&dev) {}

    #endif

    void  sendRCFrameToFC();

private:
    Stream *_dev;    
};


#endif 
