#ifndef __SOOGH_LVGL_H
#define __SOOGH_LVGL_H

#include "soogh-conf.h"

bool lvgl_init();
bool lvgl_start_task();
bool lvgl_lock(uint32_t timeout_ms);
void lvgl_unlock();

#ifdef SOOGH_ENCODER_KEYS
    extern uint32_t     lvgl_enc_last_key;
    extern bool         lvgl_enc_pressed;
    extern lv_indev_t*  lvgl_indev_keyenc;
#endif

#endif // __SOOGH_LVGL_H