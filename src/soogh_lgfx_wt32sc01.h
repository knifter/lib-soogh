#ifndef __SOOOGH_LGFX_WT32SC01_H
#define __SOOOGH_LGFX_WT32SC01_H

class LGFX_SC01 : public lgfx::LGFX_Device
{
    lgfx::Panel_ST7796  _panel_instance;  // ST7796UI
    lgfx::Bus_Parallel8 _bus_instance;    // MCU8080 8B
    lgfx::Light_PWM     _light_instance;
#ifdef SOOGH_TOUCH
    lgfx::Touch_FT5x06  _touch_instance;
#endif
    public:
        LGFX_SC01(void);
};

extern LGFX_SC01 _lgfx;

#endif // __SOOOGH_LGFX_WT32SC01_H