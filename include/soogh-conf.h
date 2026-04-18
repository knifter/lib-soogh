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
    #define SOOGH_DISP_ROTATE           1
    #define SOOGH_ENCODER_KEYS          1
    #define SOOGH_USE_LGFX

// #define LGFX_M5STACK         
    #define LGFX_AUTODETECT

#endif

#ifdef SOOGH_DEV_M5CORE2
	#define DISPLAY_HEIGHT				240
	#define DISPLAY_WIDTH				320
    #define SOOGH_DISP_ROTATE           1
    #define SOOGH_ENCODER_KEYS          1
    #define SOOGH_TOUCH                 1
    #define SOOGH_USE_LGFX

// #define LGFX_M5STACK_CORE2
    #define LGFX_AUTODETECT
#endif

#ifdef SOOGH_DEV_WAVESHARE_LCD4
    #define DISPLAY_HEIGHT				480
    #define DISPLAY_WIDTH				480

    #define SOOGH_DISP_ROTATE           0
    #define SOOGH_TOUCH
    #define SOOGH_DOUBLEBUF
    // #define SOOGH_USE_EPNL
    #define SOOGH_USE_LGFX
#endif

#ifdef SOOGH_DEV_WT32SC01
    #define DISPLAY_HEIGHT				320
    #define DISPLAY_WIDTH				480
    #define SOOGH_DISP_ROTATE           1
    #define SOOGH_TOUCH
    #define SOOGH_DOUBLEBUF
    #define SOOGH_USE_LGFX
#endif

#endif // __SOOGH_CONF_H
