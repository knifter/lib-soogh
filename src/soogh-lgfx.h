#ifndef __SOOGH_LGFX_H
#define __SOOGH_LGFX_H

// We need to define these in this order
#include "soogh-conf.h"

#include <LovyanGFX.hpp>

#if defined(SOOGH_DEV_M5CORE)
    extern LGFX _lgfx;
#endif

#if defined(SOOGH_DEV_M5CORE2)
    extern LGFX _lgfx;
#endif

#ifdef SOOGH_DEV_WT32SC01
    #include "soogh_lgfx_wt32sc01.h"
#endif

#include <lvgl.h>

#ifdef SOOGH_ENCODER_KEYS
    extern uint32_t     lvgl_enc_last_key;
    extern bool         lvgl_enc_pressed;
    extern lv_indev_t*  lvgl_indev_keyenc;
#endif

void lgfx_init();
void lvgl_init();


#endif // __SOOGH_LGFX_H
