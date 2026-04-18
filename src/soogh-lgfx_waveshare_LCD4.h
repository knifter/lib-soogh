#ifndef __SOOOGH_LGFX_WAVESHARE_LCD4_H
#define __SOOOGH_LGFX_WAVESHARE_LCD4_H

#ifdef SOOGH_DEV_WAVESHARE_LCD4
#ifdef SOOGH_USE_LGFX
// Support for Waveshare ESP32-S3-Touch-LCD4 4", 480x480, CAN, RS485, Battery, RTC?

#include <LovyanGFX.hpp>
#include <lgfx/v1/platforms/esp32s3/Panel_RGB.hpp>
#include <lgfx/v1/platforms/esp32s3/Bus_RGB.hpp>

class LGFX : public lgfx::LGFX_Device
{  
    public:
        lgfx::Panel_RGB     _panel_instance;  // ST7796UI
        lgfx::Bus_RGB       _bus_instance;    
        // lgfx::Light_PWM     _light_instance;
        lgfx::Touch_GT911 _touch_instance;

        LGFX(void);
};

#endif // SOOGH_LGFX
#endif // SOOGH_DEV_WAVESHARE_LCD4
#endif // __SOOOGH_LGFX_ESP32_LCD4_H