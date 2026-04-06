#ifdef SOOGH_DEV_WAVESHARE_LCD4

#include "soogh-conf.h"
#include "soogh-lgfx_waveshare_LCD4.h"

LGFX::LGFX(void)
{
    // Panel: ST7701 -> RGB
    // Lovyan does NOT support bounce-buffer, might pose tearing problems
    {   
        auto cfg = _panel_instance.config();
        cfg.memory_width  = DISPLAY_WIDTH;
        cfg.memory_height = DISPLAY_HEIGHT;
        cfg.panel_width   = DISPLAY_WIDTH;
        cfg.panel_height  = DISPLAY_HEIGHT;
        cfg.offset_x = 0;
        cfg.offset_y = 0;

        _panel_instance.config(cfg);
    };
    {
        auto cfg = _panel_instance.config_detail();

        cfg.use_psram = 1;

        _panel_instance.config_detail(cfg);
    };

    // Arduino_DataBus *bus = new Arduino_SWSPI(
    //     GFX_NOT_DEFINED /* DC */, 42 /* CS */,
    //     2 /* SCK */, 1 /* MOSI */, GFX_NOT_DEFINED /* MISO */);

    // Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
    //     40 /* DE */, 39 /* VSYNC */, 38 /* HSYNC */, 41 /* PCLK */,
    //     46 /* R0 */, 3 /* R1 */, 8 /* R2 */, 18 /* R3 */, 17 /* R4 */, 
    //     14 /* G0 */, 13 /* G1 */, 12 /* G2 */, 11 /* G3 */, 10 /* G4 */, 9 /* G5 */,
    //     5 /* B0 */, 45 /* B1 */, 48 /* B2 */, 47 /* B3 */, 21 /* B4 */,
    //     1 /* hsync_polarity */, 10 /* hsync_front_porch */, 8 /* hsync_pulse_width */, 50 /* hsync_back_porch */,
    //     1 /* vsync_polarity */, 10 /* vsync_front_porch */, 8 /* vsync_pulse_width */, 20 /* vsync_back_porch */);

    // Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
    //     480 /* width */, 480 /* height */, rgbpanel, 2 /* rotation */, true /* auto_flush */,
    //     bus, GFX_NOT_DEFINED /* RST */, st7701_type1_init_operations, sizeof(st7701_type1_init_operations));

    // BUS
    {   
        auto cfg = _bus_instance.config();
        cfg.panel = &_panel_instance;

        // RGB565, From schema:
        // B0=NC B1=IO5 B2=IO45 B3=IO48 B4=IO47 B5=IO21
        cfg.pin_d0      = GPIO_NUM_5;  // B0
        cfg.pin_d1      = GPIO_NUM_45; // B1
        cfg.pin_d2      = GPIO_NUM_48; // B2
        cfg.pin_d3      = GPIO_NUM_47; // B3
        cfg.pin_d4      = GPIO_NUM_21; // B4
        // G0=IO14 G1=IO13 G2=IO12 G3=IO11 G4=IO10 G5=IO9
        cfg.pin_d5      = GPIO_NUM_14; // G0
        cfg.pin_d6      = GPIO_NUM_13; // G1
        cfg.pin_d7      = GPIO_NUM_12; // G2
        cfg.pin_d8      = GPIO_NUM_11; // G3
        cfg.pin_d9      = GPIO_NUM_10; // G4
        cfg.pin_d10     = GPIO_NUM_9; // G5
        // R0 NC R1=IO46 R2=IO3 R3=IO8 R4=IO18 R5=IO17
        cfg.pin_d11     = GPIO_NUM_46;  // R0
        cfg.pin_d12     = GPIO_NUM_3; // R1
        cfg.pin_d13     = GPIO_NUM_8; // R2
        cfg.pin_d14     = GPIO_NUM_18;  // R3
        cfg.pin_d15     = GPIO_NUM_17;  // R4

        cfg.pin_henable = GPIO_NUM_42; // ? LCD_CS
        cfg.pin_vsync   = GPIO_NUM_39; // VSYNC
        cfg.pin_hsync   = GPIO_NUM_38; // HSYNC
        cfg.pin_pclk    = GPIO_NUM_41; // LCD_PCLK

        cfg.freq_write  = 14000000; // 160MHz possible?

        cfg.hsync_polarity    = 1;
        cfg.hsync_front_porch = 10;
        cfg.hsync_pulse_width = 8;
        cfg.hsync_back_porch  = 50;
        cfg.vsync_polarity    = 1;
        cfg.vsync_front_porch = 10;
        cfg.vsync_pulse_width = 8;
        cfg.vsync_back_porch  = 20;
        cfg.pclk_idle_high    = 1;

        _bus_instance.config(cfg);
    };
    _panel_instance.setBus(&_bus_instance);

    // Backlight: IO Expander?
    // {   
    //     auto cfg = _light_instance.config();
    //     cfg.pin_bl = 45;
    //     cfg.invert = false;
    //     cfg.freq   = 44100;
    //     cfg.pwm_channel = 7;
    //     _light_instance.config(cfg);

    // };
    // _panel_instance.light(&_light_instance);

    // Touch GT911     
    {
        auto cfg = _touch_instance.config();
        cfg.x_min      = 0;
        cfg.x_max      = DISPLAY_WIDTH;
        cfg.y_min      = 0;
        cfg.y_max      = DISPLAY_HEIGHT;
        cfg.pin_int    = GPIO_NUM_NC;   // EXIO2

        cfg.i2c_port    = 1; // I2C_NUM_1;
        cfg.pin_sda     = GPIO_NUM_1; // LCD_SDA
        cfg.pin_scl     = GPIO_NUM_2; // LCD_SCL
        cfg.bus_shared  = true;          // With IO-Expander
        cfg.i2c_addr    = 0x14;
        cfg.freq        = 400000;
        _touch_instance.config(cfg);
    };
    _panel_instance.setTouch(&_touch_instance);

    setPanel(&_panel_instance);
};

#endif
