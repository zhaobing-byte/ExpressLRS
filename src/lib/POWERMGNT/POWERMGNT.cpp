#include "POWERMGNT.h"
#include "../../src/targets.h"

#if defined(Regulatory_Domain_AU_915) || defined(Regulatory_Domain_EU_868) || defined(Regulatory_Domain_FCC_915) || defined(Regulatory_Domain_AU_433) || defined(Regulatory_Domain_EU_433)
extern SX127xDriver Radio;
#elif Regulatory_Domain_ISM_2400
extern SX1280Driver Radio;
#endif

#if defined(TARGET_R9M_TX) || defined(TARGET_TX_ES915TX)
extern DAC TxDAC;
#endif

PowerLevels_e POWERMGNT::CurrentPower = (PowerLevels_e)DefaultPowerEnum;

PowerLevels_e POWERMGNT::incPower()
{
    if (CurrentPower < MaxPower)
    {
        setPower((PowerLevels_e)((uint8_t)CurrentPower + 1));
    }
    return CurrentPower;
}

PowerLevels_e POWERMGNT::decPower()
{
    if (CurrentPower > 0)
    {
        setPower((PowerLevels_e)((uint8_t)CurrentPower - 1));
    }
    return CurrentPower;
}

PowerLevels_e POWERMGNT::currPower()
{
    return CurrentPower;
}

void POWERMGNT::init()
{


#if defined(TARGET_R9M_TX) || defined(TARGET_TX_ES915TX)
    Serial.println("Init DAC Driver");
#endif
#ifdef TARGET_R9M_LITE_PRO_TX
    //initialize both 12 bit DACs
    pinMode(GPIO_PIN_RFamp_APC1, OUTPUT);
    pinMode(GPIO_PIN_RFamp_APC2, OUTPUT);
    analogWriteResolution(12);
#endif
#ifdef GPIO_PIN_FAN_EN  && (GPIO_PIN_FAN_EN != UNDEF_PIN)
    pinMode(GPIO_PIN_FAN_EN, OUTPUT);
#endif
#if defined(GPIO_PIN_RF_AMP_EN) && (GPIO_PIN_RF_AMP_EN != UNDEF_PIN)
    pinMode(GPIO_PIN_RF_AMP_EN, OUTPUT);
    digitalWrite(GPIO_PIN_RF_AMP_EN, HIGH);
#endif
}

void POWERMGNT::setDefaultPower()
{
    setPower((PowerLevels_e)DefaultPowerEnum);
}

PowerLevels_e POWERMGNT::setPower(PowerLevels_e Power)
{
    if (Power > MaxPower)
    {
        Power = (PowerLevels_e)MaxPower;
    }

#if defined(TARGET_TX_ESP32_SX1280_V1) || defined(TARGET_RX_ESP8266_SX1280_V1)
    switch (Power)
    {
    case PWR_10mW:
        Radio.SetOutputPower(8);
        break;
    case PWR_25mW:
    default:
        Radio.SetOutputPower(13);
        Power = PWR_25mW;
        break;
    }
#elif defined(TARGET_RX_BETAFPV_2400_V1)
    switch (Power)
    {
    case PWR_10mW:
        Radio.SetOutputPower(8);
        break;
    case PWR_25mW:
    default:
        Radio.SetOutputPower(13);
        Power = PWR_25mW;
        break;
    }
#elif defined(TARGET_TX_GHOST)
    switch (Power)
    {
    case PWR_10mW:
        Radio.SetOutputPower(0);
        break;
    case PWR_25mW:
        Radio.SetOutputPower(4);
        break;
    case PWR_100mW:
        Radio.SetOutputPower(10);
        break;
    case PWR_250mW:
        Radio.SetOutputPower(13);
        break;
    case PWR_50mW:
    default:
        Power = PWR_50mW;
        Radio.SetOutputPower(7);
        break;
    }
#elif defined(TARGET_R9M_TX) || defined(TARGET_TX_ES915TX)
    Radio.SetOutputPower(0b0000);
    TxDAC.setPower((DAC_PWR_)Power);
#ifdef GPIO_PIN_FAN_EN
    (Power >= PWR_250mW) ? digitalWrite(GPIO_PIN_FAN_EN, HIGH) : digitalWrite(GPIO_PIN_FAN_EN, LOW);
#endif
    return CurrentPower;
#elif defined(TARGET_R9M_LITE_PRO_TX)
    Radio.SetOutputPower(0b0000);
    //Set DACs PA5 & PA4
    switch (Power)
    {
    case PWR_100mW:
        analogWrite(GPIO_PIN_RFamp_APC1, 3350); //0-4095 2.7V
        analogWrite(GPIO_PIN_RFamp_APC2, 732); //0-4095  590mV
        CurrentPower = PWR_100mW;
        break;
    case PWR_250mW:
        analogWrite(GPIO_PIN_RFamp_APC1, 3350); //0-4095 2.7V
        analogWrite(GPIO_PIN_RFamp_APC2, 1080); //0-4095 870mV this is actually 200mw
        CurrentPower = PWR_250mW;
        break;
    case PWR_500mW:
        analogWrite(GPIO_PIN_RFamp_APC1, 3350); //0-4095 2.7V
        analogWrite(GPIO_PIN_RFamp_APC2, 1356); //0-4095 1.093V
        CurrentPower = PWR_500mW;
        break;
    case PWR_1000mW:
        analogWrite(GPIO_PIN_RFamp_APC1, 3350); //0-4095 2.7V
        analogWrite(GPIO_PIN_RFamp_APC2, 1853); //0-4095 1.493V
        CurrentPower = PWR_1000mW;
        break;
    default:
        CurrentPower = PWR_100mW;
        analogWrite(GPIO_PIN_RFamp_APC1, 3350); //0-4095 2.7V
        analogWrite(GPIO_PIN_RFamp_APC2, 732);  //0-4095 590mV
        break;
    }
#elif defined(TARGET_100mW_MODULE) || defined(TARGET_R9M_LITE_TX)
    switch (Power)
    {
    case PWR_10mW:
        Radio.SetOutputPower(0b1000);
        CurrentPower = PWR_10mW;
        break;
    case PWR_25mW:
        Radio.SetOutputPower(0b1100);
        CurrentPower = PWR_25mW;
        break;
    case PWR_50mW:
    default:
        Power = PWR_50mW;
        Radio.SetOutputPower(0b1111); //15
        break;
    }
#elif defined(TARGET_1000mW_MODULE)
    switch (Power)
    {
    case PWR_100mW:
        Radio.SetOutputPower(0b0101);
        break;
    case PWR_250mW:
        Radio.SetOutputPower(0b1000);
        break;
    case PWR_500mW:
        Radio.SetOutputPower(0b1100);
        break;
    case PWR_1000mW:
        Radio.SetOutputPower(0b1111);
        break;
    case PWR_50mW:
    default:
        Radio.SetOutputPower(0b0010);
        Power = PWR_50mW;
        break;
    }
#elif defined(TARGET_TX_ESP32_E28_SX1280_V1)
    switch (Power)
    {
    case PWR_10mW:
        Radio.SetOutputPower(-15);
        break;
    case PWR_25mW:
        Radio.SetOutputPower(-11);
        break;
    case PWR_50mW:
        Radio.SetOutputPower(-8);
        break;
    case PWR_100mW:
        Radio.SetOutputPower(-5);
        break;
    case PWR_250mW:
        Radio.SetOutputPower(-1);
        break;
    default:
        Power = PWR_50mW;
        Radio.SetOutputPower(-8);
        break;
    }
#elif defined(TARGET_TX_ESP32_LORA1280F27)
    switch (Power)
    {
    case PWR_10mW:
        Radio.SetOutputPower(-4);
        break;
    case PWR_25mW:
        Radio.SetOutputPower(0);
        break;
    case PWR_50mW:
        Radio.SetOutputPower(3);
        break;
    case PWR_100mW:
        Radio.SetOutputPower(6);
        break;
    case PWR_250mW:
        Radio.SetOutputPower(12);
        break;
    default:
        Power = PWR_50mW;
        Radio.SetOutputPower(3);
        break;
    }
#elif defined(TARGET_TX_BETAFPV_900_V1)
    switch (Power)
    {
    case PWR_100mW:
        Radio.SetOutputPower(0b0000);
        digitalWrite(GPIO_PIN_LED_GREEN_OUTPUT, HIGH);
        digitalWrite(GPIO_PIN_LED_RED_OUTPUT, LOW);
        Serial.println("set PWR_100mW");
        break;
    case PWR_250mW:
        Radio.SetOutputPower(0b0001);
        digitalWrite(GPIO_PIN_LED_GREEN_OUTPUT, HIGH);
        digitalWrite(GPIO_PIN_LED_RED_OUTPUT, HIGH);
        Serial.println("set PWR_250mW");
        break;
    case PWR_500mW:
        Radio.SetOutputPower(0b1000);
        digitalWrite(GPIO_PIN_LED_GREEN_OUTPUT, LOW);
        digitalWrite(GPIO_PIN_LED_RED_OUTPUT, HIGH);
        Serial.println("set PWR_500mW");
        break;
    case PWR_1000mW:
        Radio.SetOutputPower(0b1111);
        break;
    case PWR_50mW:
    default:
        Radio.SetOutputPower(0b0000);
        Power = PWR_50mW;
        digitalWrite(GPIO_PIN_LED_GREEN_OUTPUT, HIGH);
        digitalWrite(GPIO_PIN_LED_RED_OUTPUT, LOW);
        break;
    }
#elif defined(TARGET_TX_BETAFPV_2400_V1)
    switch (Power)
    {
    case PWR_10mW:
        Radio.SetOutputPower(-15);
        digitalWrite(GPIO_PIN_LED_GREEN_OUTPUT, HIGH);
        digitalWrite(GPIO_PIN_LED_RED_OUTPUT, LOW);        
        break;
    case PWR_25mW:
        Radio.SetOutputPower(-11);
        digitalWrite(GPIO_PIN_LED_GREEN_OUTPUT, HIGH);
        digitalWrite(GPIO_PIN_LED_RED_OUTPUT, LOW);        
        break;
    case PWR_50mW:
        Radio.SetOutputPower(-10);
        digitalWrite(GPIO_PIN_LED_GREEN_OUTPUT, HIGH);
        digitalWrite(GPIO_PIN_LED_RED_OUTPUT, LOW);        
        break;
    case PWR_100mW:
        Radio.SetOutputPower(-9);
        digitalWrite(GPIO_PIN_LED_GREEN_OUTPUT, HIGH);
        digitalWrite(GPIO_PIN_LED_RED_OUTPUT, LOW);
        Serial.println("set PWR_100mW");
        break;
    case PWR_250mW:
        Radio.SetOutputPower(-3);
        digitalWrite(GPIO_PIN_LED_GREEN_OUTPUT, HIGH);
        digitalWrite(GPIO_PIN_LED_RED_OUTPUT, HIGH);
        Serial.println("set PWR_250mW");
        break;
    case PWR_500mW:
        Radio.SetOutputPower(13);
        digitalWrite(GPIO_PIN_LED_GREEN_OUTPUT, LOW);
        digitalWrite(GPIO_PIN_LED_RED_OUTPUT, HIGH);
        Serial.println("set PWR_500mW");
        break;
    default:
        Power = PWR_50mW;
        Radio.SetOutputPower(-8);
        digitalWrite(GPIO_PIN_LED_GREEN_OUTPUT, HIGH);
        digitalWrite(GPIO_PIN_LED_RED_OUTPUT, LOW);    
        break;
    }
#else
#error "[ERROR] Unknown power management!"
#endif
    CurrentPower = Power;
    return Power;
}
