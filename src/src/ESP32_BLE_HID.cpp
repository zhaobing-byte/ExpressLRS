#include "ESP32_BLE_HID.h"

void initBLEHID()
{
    Serial.println("Starting BLE work!");
    bleGamepad.begin();
}

void ICACHE_RAM_ATTR updateBLEHID()
{
    if (bleGamepad.isConnected())
    {
        uint8_t AxisData[6] = {0};
        AxisData[0] = CRSF_to_UINT8(crsf.ChannelDataIn[0]) - 127;
        AxisData[1] = CRSF_to_UINT8(crsf.ChannelDataIn[1]) - 127;
        AxisData[2] = CRSF_to_UINT8(crsf.ChannelDataIn[2]) - 127;
        AxisData[3] = CRSF_to_UINT8(crsf.ChannelDataIn[3]) - 127;
        AxisData[4] = CRSF_to_UINT8(crsf.ChannelDataIn[4]) - 127;
        AxisData[5] = CRSF_to_UINT8(crsf.ChannelDataIn[5]) - 127;
        //Serial.print(".");
        bleGamepad.setAxes(AxisData[0], AxisData[1], AxisData[2], AxisData[3], AxisData[4], AxisData[5]);
        //delay(10);
    }
}