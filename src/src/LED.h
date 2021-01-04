/// LED SUPPORT ///////
#if defined(PLATFORM_ESP32) && defined(GPIO_PIN_LED)
#include <NeoPixelBus.h>
const uint16_t PixelCount = 1; // this example assumes 1 pixel
#define colorSaturation 50
NeoPixelBus<NeoRgbFeature, Neo800KbpsMethod> strip(PixelCount, GPIO_PIN_LED);

RgbColor red(colorSaturation, 0, 0);
RgbColor green(0, colorSaturation, 0);
RgbColor blue(0, 0, colorSaturation);
RgbColor white(colorSaturation);
RgbColor black(0);

HslColor hslRed(red);
HslColor hslGreen(green);
HslColor hslBlue(blue);
HslColor hslWhite(white);
HslColor hslBlack(black);

#define ledflashintervalmillis 200
uint32_t ledlastflashed;
bool LEDtoggle = false;

void updateLEDstripRaw(uint8_t r, uint8_t g, uint8_t b)
{
    strip.ClearTo(RgbColor(r, g, b));
}

void updateLEDstripLoop(uint8_t conState, uint8_t tlm_ratio)
{
    uint8_t brightness = LED_MAX_BRIGHTNESS;

    if (conState == 0 && tlm_ratio != 0)
    {
        LEDtoggle ? brightness = 0 : brightness = LED_MAX_BRIGHTNESS;

        if (millis() > (ledlastflashed + ledflashintervalmillis))
        {
            ledlastflashed = millis();
            LEDtoggle = !LEDtoggle;
        }

        switch (ExpressLRS_currAirRate_Modparams->index)
        {
        case 0:
            strip.ClearTo(RgbColor(0, 0, brightness));
            break;

        case 1:
            strip.ClearTo(RgbColor(0, brightness, 0));
            break;

        case 2:
            strip.ClearTo(RgbColor(brightness, 0, 0));
            break;

        default:
            break;
        }
        strip.Show();
    }
}

void updateLEDstrip()
{
    switch (ExpressLRS_currAirRate_Modparams->index)
    {
    case 0:
        strip.ClearTo(RgbColor(0, 0, LED_MAX_BRIGHTNESS));
        break;

    case 1:
        strip.ClearTo(RgbColor(0, LED_MAX_BRIGHTNESS, 0));
        break;

    case 2:
        strip.ClearTo(RgbColor(LED_MAX_BRIGHTNESS, 0, 0));
        break;

    default:
        break;
    }
    strip.Show();
}
#endif
