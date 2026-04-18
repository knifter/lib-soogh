#ifndef __SOOGH_EPNL_WAVESHARE_LCD4
#define __SOOGH_EPNL_WAVESHARE_LCD4

#include <stdint.h>

void waveshare_lcd4_init(void);                    // Call before panel->init()
void waveshare_lcd4_set_backlight(uint8_t pct);    // 0–100
bool waveshare_lcd4_is_v4(void);

#endif // __SOOGH_EPNL_WAVESHARE_LCD4
