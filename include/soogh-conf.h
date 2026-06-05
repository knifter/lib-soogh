#ifndef __SOOGH_CONF_H
#define __SOOGH_CONF_H

// Application defines: Add them to platform.io
// #define SOOGH_DOUBLEBUF
// #define SOOGH_KEYPAD

#define LGFX_USE_V1

// #define BOOTSCREEN_TIMEOUT_MS		5000
#ifdef SOOGH_DEV_M5CORE
	#define DISPLAY_HEIGHT				240
	#define DISPLAY_WIDTH				320
    #define DISPLAY_INCH_HOR            (2.0f)

    #define SOOGH_DISP_ROTATE           1
    #define SOOGH_ENCODER_KEYS          1
    #define SOOGH_USE_LGFX

// #define LGFX_M5STACK         
    #define LGFX_AUTODETECT

#endif

#ifdef SOOGH_DEV_M5CORE2
	#define DISPLAY_HEIGHT				240
	#define DISPLAY_WIDTH				320
    #define DISPLAY_INCH_HOR            (2.0f)
    #define SOOGH_DISP_ROTATE           1
    #define SOOGH_ENCODER_KEYS          1
    #define SOOGH_TOUCH                 1
    #define SOOGH_USE_LGFX

// #define LGFX_M5STACK_CORE2
    #define LGFX_AUTODETECT
#endif

// The Waveshare LCD4 has two backends:
//   SOOGH_DEV_WAVESHARE_LCD4        -> supported board in ESP32_Display_Panel (BOARD_..._LCD_4_V4)
//   SOOGH_DEV_WAVESHARE_LCD4_CUSTOM -> soogh's own custom board config (esp_panel_waveshare_LCD4_custom.h)
// The _CUSTOM variant still uses all the common LCD4 settings below.
#ifdef SOOGH_DEV_WAVESHARE_LCD4
    #define DISPLAY_HEIGHT				480
    #define DISPLAY_WIDTH				480
    #define DISPLAY_INCH_HOR            (4.0f)

    #define SOOGH_DISP_ROTATE           0
    #define SOOGH_TOUCH
    #define SOOGH_USE_EPNL
#endif

#ifdef SOOGH_DEV_WAVESHARE_LCD4_CUSTOM
    #define DISPLAY_HEIGHT				480
    #define DISPLAY_WIDTH				480
    #define DISPLAY_INCH_HOR            (4.0f)

    #define SOOGH_DISP_ROTATE           0
    #define SOOGH_TOUCH
    #define SOOGH_USE_EPNL
    // #define SOOGH_USE_LGFX
#endif

#ifdef SOOGH_DEV_WT32SC01
    #define DISPLAY_HEIGHT				320
    #define DISPLAY_WIDTH				480
    #define DISPLAY_INCH_HOR            (1.0f) // unknown
    #define SOOGH_DISP_ROTATE           1
    #define SOOGH_TOUCH
    #define SOOGH_DOUBLEBUF
    #define SOOGH_USE_LGFX
#endif

#ifdef SOOGH_USE_EPNL
    // EPNL MUST use doublebuf or flickering is your demon
    #define SOOGH_DOUBLEBUF
#endif

#endif // __SOOGH_CONF_H
