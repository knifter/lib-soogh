#include "soogh-conf.h"

// This file must be present and named like this for ESP_PANEL_BOARD_USE_SUPPORTED_FILE to work.
//
// When SOOGH_DEV_WAVESHARE_LCD4 is defined (and NOT the _CUSTOM variant), use the supported board
// config shipped in ESP32_Display_Panel. The _CUSTOM variant uses soogh's own board config
// instead (see esp_panel_board_custom_conf.h).

#if defined(SOOGH_DEV_WAVESHARE_LCD4)

    #define ESP_PANEL_BOARD_DEFAULT_USE_SUPPORTED   (1)
    #define BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_4_V4

    // File version — must match the library's supported board config version.
    #define ESP_PANEL_BOARD_SUPPORTED_FILE_VERSION_MAJOR 1
    #define ESP_PANEL_BOARD_SUPPORTED_FILE_VERSION_MINOR 3
    #define ESP_PANEL_BOARD_SUPPORTED_FILE_VERSION_PATCH 0

#endif
