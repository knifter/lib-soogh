#ifndef __SOOGH_EPNL_WAVESHARE_LCD4
#define __SOOGH_EPNL_WAVESHARE_LCD4

#include "soogh-conf.h"

#ifdef SOOGH_DEV_WAVESHARE_LCD4

#include <stdint.h>

#if !defined(WAVESHARE_LCD_VERSION)
    #define WAVESHARE_LCD_VERSION 4
#endif

void waveshare_lcd4_init(void);                    // Call before panel->init()
void waveshare_lcd4_set_backlight(uint8_t pct);    // 0–100 (V3: >0 = on)
void waveshare_lcd4_beep(bool on);

#endif // SOOGH_DEV_WAVESHARE_LCD4

#endif // __SOOGH_EPNL_WAVESHARE_LCD4
