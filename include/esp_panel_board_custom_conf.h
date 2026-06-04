#include "soogh-conf.h"

// This file must be present and named like this for ESP_PANEL_BOARD_USE_CUSTOM_FILE to work.
//
// The soogh custom board config is only used when SOOGH_DEV_WAVESHARE_LCD4_CUSTOM is defined.
// Without it (plain SOOGH_DEV_WAVESHARE_LCD4), the supported board config in ESP32_Display_Panel
// is used instead (see esp_panel_board_supported_conf.h).

#ifdef SOOGH_DEV_WAVESHARE_LCD4_CUSTOM
    #define ESP_PANEL_BOARD_DEFAULT_USE_CUSTOM      (1)
    #include "esp_panel_waveshare_LCD4_custom.h"
#endif
