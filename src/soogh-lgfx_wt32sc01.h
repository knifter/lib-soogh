#ifndef __SOOOGH_LGFX_WT32SC01_H
#define __SOOOGH_LGFX_WT32SC01_H

#ifdef SOOGH_DEV_WT32SC01

#include "soogh_lgfx.h"


#include "soogh-lgfx.h"
#include <LovyanGFX.hpp>

class LGFX : public lgfx::LGFX_Device
{
    public:
        lgfx::Panel_ST7796  _panel_instance;  // ST7796UI
        lgfx::Bus_Parallel8 _bus_instance;    // MCU8080 8B
        lgfx::Light_PWM     _light_instance;
#ifdef SOOGH_TOUCH
        lgfx::Touch_FT5x06  _touch_instance;
#endif
        LGFX(void);
};

extern LGFX _lgfx;

#endif

#endif // __SOOOGH_LGFX_WT32SC01_H