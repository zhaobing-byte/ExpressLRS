#include <Arduino.h>

#include "SBUS.h"
#include "CRSF.h"

extern CRSF crsf;
void ICACHE_RAM_ATTR SBUS::sendRCFrameToFC()
{
    uint8_t outBuffer[SBUSframeLength] = {0};

    outBuffer[0] = SBUS_HEAD_BYTE;

    outBuffer[1] = (crsf.PackedRCdataOut.ch0 & 0x07FF);
    outBuffer[2] = ((crsf.PackedRCdataOut.ch0 & 0x07FF)>>8 | (crsf.PackedRCdataOut.ch1 & 0x07FF)<<3);
    outBuffer[3] = ((crsf.PackedRCdataOut.ch1 & 0x07FF)>>5 | (crsf.PackedRCdataOut.ch2 & 0x07FF)<<6);
    outBuffer[4] = ((crsf.PackedRCdataOut.ch2 & 0x07FF)>>2);
    outBuffer[5] = ((crsf.PackedRCdataOut.ch2 & 0x07FF)>>10 | (crsf.PackedRCdataOut.ch3 & 0x07FF)<<1);
    outBuffer[6] = ((crsf.PackedRCdataOut.ch3 & 0x07FF)>>7 | (crsf.PackedRCdataOut.ch4 & 0x07FF)<<4);
    outBuffer[7] = ((crsf.PackedRCdataOut.ch4 & 0x07FF)>>4 | (crsf.PackedRCdataOut.ch5 & 0x07FF)<<7);
    outBuffer[8] = ((crsf.PackedRCdataOut.ch5 & 0x07FF)>>1);
    outBuffer[9] = ((crsf.PackedRCdataOut.ch5 & 0x07FF)>>9 | (crsf.PackedRCdataOut.ch6 & 0x07FF)<<2);
    outBuffer[10] = ((crsf.PackedRCdataOut.ch6 & 0x07FF)>>6 | (crsf.PackedRCdataOut.ch7 & 0x07FF)<<5);
    outBuffer[11] = ((crsf.PackedRCdataOut.ch7 & 0x07FF)>>3);
    outBuffer[12] = 0x00;
    outBuffer[13] = 0x00;
    outBuffer[14] = 0x00;
    outBuffer[15] = 0x00;
    outBuffer[16] = 0x00;
    outBuffer[17] = 0x00;
    outBuffer[18] = 0x00;
    outBuffer[19] = 0x00;
    outBuffer[20] = 0x00;
    outBuffer[21] = 0x00;
    outBuffer[22] = 0x00;
    //flags
    outBuffer[23] = 0x00;

    outBuffer[24] = SBUS_END_BYTE;
    
    this->_dev->write(outBuffer, SBUSframeLength);

}
