#define ESP_PANEL_BOARD_NAME                "Waveshare:ESP32-S3-Touch-LCD-4-Square"
#define ESP_PANEL_BOARD_WIDTH               (480)
#define ESP_PANEL_BOARD_HEIGHT              (480)

////////// LCD //////////
#define ESP_PANEL_BOARD_USE_LCD             (1)
#define ESP_PANEL_BOARD_LCD_CONTROLLER      ST7701
#define ESP_PANEL_BOARD_LCD_BUS_TYPE        (ESP_PANEL_BUS_TYPE_RGB)

// 3-wire SPI control panel (sends vendor init sequence before RGB takeover)
#define ESP_PANEL_BOARD_LCD_RGB_USE_CONTROL_PANEL       (1)
#define ESP_PANEL_BOARD_LCD_RGB_SPI_IO_CS               (42)
#define ESP_PANEL_BOARD_LCD_RGB_SPI_IO_SCK              (2)
#define ESP_PANEL_BOARD_LCD_RGB_SPI_IO_SDA              (1)
#define ESP_PANEL_BOARD_LCD_RGB_SPI_CS_USE_EXPNADER     (0)  // CS is MCU GPIO, not expander
#define ESP_PANEL_BOARD_LCD_RGB_SPI_SCL_USE_EXPNADER    (0)
#define ESP_PANEL_BOARD_LCD_RGB_SPI_SDA_USE_EXPNADER    (0)
#define ESP_PANEL_BOARD_LCD_RGB_SPI_MODE                (0)
#define ESP_PANEL_BOARD_LCD_RGB_SPI_CMD_BYTES           (1)
#define ESP_PANEL_BOARD_LCD_RGB_SPI_PARAM_BYTES         (1)
#define ESP_PANEL_BOARD_LCD_RGB_SPI_USE_DC_BIT          (1)

// RGB timing — verified working values for this panel
#define ESP_PANEL_BOARD_LCD_RGB_CLK_HZ          (10 * 1000 * 1000)
#define ESP_PANEL_BOARD_LCD_RGB_HPW             (10)
#define ESP_PANEL_BOARD_LCD_RGB_HBP             (10)
#define ESP_PANEL_BOARD_LCD_RGB_HFP             (20)
#define ESP_PANEL_BOARD_LCD_RGB_VPW             (10)
#define ESP_PANEL_BOARD_LCD_RGB_VBP             (10)
#define ESP_PANEL_BOARD_LCD_RGB_VFP             (10)
#define ESP_PANEL_BOARD_LCD_RGB_PCLK_ACTIVE_NEG (0)
// #define ESP_PANEL_BOARD_LCD_RGB_HPW             (8)
// #define ESP_PANEL_BOARD_LCD_RGB_HBP             (50)
// #define ESP_PANEL_BOARD_LCD_RGB_HFP             (10)
// #define ESP_PANEL_BOARD_LCD_RGB_VPW             (8)
// #define ESP_PANEL_BOARD_LCD_RGB_VBP             (20)
// #define ESP_PANEL_BOARD_LCD_RGB_VFP             (10)
// #define ESP_PANEL_BOARD_LCD_RGB_PCLK_ACTIVE_NEG (1)    // pclk_idle_high=1 in lgfx

// 16-bit parallel bus, RGB565 in framebuffer
// Note: panel is configured for RGB666 (COLMOD=0x60) but the 16-bit data bus
// means only RGB565 (5-6-5 bits) can be transferred. The '666' just means the
// controller accepts 6-bit colour internally; framebuffer data is still RGB565.
#define ESP_PANEL_BOARD_LCD_RGB_DATA_WIDTH      (16)
#define ESP_PANEL_BOARD_LCD_RGB_PIXEL_BITS      (ESP_PANEL_LCD_COLOR_BITS_RGB565)

// Bounce buffer: pre-fetches scanlines into SRAM to reduce PSRAM contention.
// WIDTH * 10 = 4800 pixels = 9600 bytes. Must satisfy: size * N = W * H (N even).
// 480 * 480 / 4800 = 48 (even). Good.
#define ESP_PANEL_BOARD_LCD_RGB_BOUNCE_BUF_SIZE (ESP_PANEL_BOARD_WIDTH * 10)

// RGB interface GPIO mapping
#define ESP_PANEL_BOARD_LCD_RGB_IO_HSYNC        (38)
#define ESP_PANEL_BOARD_LCD_RGB_IO_VSYNC        (39)
#define ESP_PANEL_BOARD_LCD_RGB_IO_DE           (40)
#define ESP_PANEL_BOARD_LCD_RGB_IO_PCLK         (41)
#define ESP_PANEL_BOARD_LCD_RGB_IO_DISP         (-1)  // not used

// Data bus: B0-B4, G0-G5, R0-R4
#define ESP_PANEL_BOARD_LCD_RGB_IO_DATA0        (5)   // B0
#define ESP_PANEL_BOARD_LCD_RGB_IO_DATA1        (45)  // B1
#define ESP_PANEL_BOARD_LCD_RGB_IO_DATA2        (48)  // B2
#define ESP_PANEL_BOARD_LCD_RGB_IO_DATA3        (47)  // B3
#define ESP_PANEL_BOARD_LCD_RGB_IO_DATA4        (21)  // B4
#define ESP_PANEL_BOARD_LCD_RGB_IO_DATA5        (14)  // G0
#define ESP_PANEL_BOARD_LCD_RGB_IO_DATA6        (13)  // G1
#define ESP_PANEL_BOARD_LCD_RGB_IO_DATA7        (12)  // G2
#define ESP_PANEL_BOARD_LCD_RGB_IO_DATA8        (11)  // G3
#define ESP_PANEL_BOARD_LCD_RGB_IO_DATA9        (10)  // G4
#define ESP_PANEL_BOARD_LCD_RGB_IO_DATA10       (9)   // G5
#define ESP_PANEL_BOARD_LCD_RGB_IO_DATA11       (46)  // R0
#define ESP_PANEL_BOARD_LCD_RGB_IO_DATA12       (3)   // R1
#define ESP_PANEL_BOARD_LCD_RGB_IO_DATA13       (8)   // R2
#define ESP_PANEL_BOARD_LCD_RGB_IO_DATA14       (18)  // R3
#define ESP_PANEL_BOARD_LCD_RGB_IO_DATA15       (17)  // R4

// IO multiplex: 3-wire SPI pins do NOT share RGB pins on this board,
// so set to 0. Pins are released after init() regardless.
#define ESP_PANEL_BOARD_LCD_FLAGS_ENABLE_IO_MULTIPLEX   (0)
#define ESP_PANEL_BOARD_LCD_FLAGS_MIRROR_BY_CMD         (1)

// Vendor init: leave undefined to use the library's built-in default sequence.
// The default sequence in esp_lcd_st7701_rgb.c is correct for this panel.
// Uncomment and fill in below ONLY if you need to override it:
// #define ESP_PANEL_BOARD_LCD_VENDOR_INIT_CMD() { ... }

// Pixel format and orientation
#define ESP_PANEL_BOARD_LCD_COLOR_BITS          (ESP_PANEL_LCD_COLOR_BITS_RGB666)
// #define ESP_PANEL_BOARD_LCD_COLOR_BITS          (ESP_PANEL_LCD_COLOR_BITS_RGB565)
#define ESP_PANEL_BOARD_LCD_COLOR_BGR_ORDER     (0)  // RGB order
#define ESP_PANEL_BOARD_LCD_COLOR_INEVRT_BIT    (0)
#define ESP_PANEL_BOARD_LCD_SWAP_XY             (0)
#define ESP_PANEL_BOARD_LCD_MIRROR_X            (0)
#define ESP_PANEL_BOARD_LCD_MIRROR_Y            (0)
#define ESP_PANEL_BOARD_LCD_GAP_X               (0)
#define ESP_PANEL_BOARD_LCD_GAP_Y               (0)

// Reset handled manually in board_init() before panel->init()
#define ESP_PANEL_BOARD_LCD_RST_IO              (-1)
#define ESP_PANEL_BOARD_LCD_RST_LEVEL           (0)

// Needed to skip i2c init to avoid conflicts
#define ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST    (1)     // 0/1
#define ESP_PANEL_BOARD_EXPANDER_SKIP_INIT_HOST     (1)     // 0/1

////////// TOUCH //////////
// Set to 1 if you want the library to manage GT911.
// Set to 0 if you implement touch yourself (simpler for V3/V4 address detection).
#define ESP_PANEL_BOARD_USE_TOUCH               (0)
#if ESP_PANEL_BOARD_USE_TOUCH

    #define ESP_PANEL_BOARD_TOUCH_CONTROLLER        GT911
    #define ESP_PANEL_BOARD_TOUCH_BUS_TYPE          (ESP_PANEL_BUS_TYPE_I2C)

    #if ESP_PANEL_BOARD_TOUCH_BUS_TYPE == ESP_PANEL_BUS_TYPE_I2C
        #define ESP_PANEL_BOARD_TOUCH_BUS_SKIP_INIT_HOST    (0)
        #define ESP_PANEL_BOARD_TOUCH_I2C_HOST_ID           (1)    // I2C_NUM_1
        #define ESP_PANEL_BOARD_TOUCH_I2C_CLK_HZ            (400 * 1000)
        #define ESP_PANEL_BOARD_TOUCH_I2C_SCL_PULLUP        (1)
        #define ESP_PANEL_BOARD_TOUCH_I2C_SDA_PULLUP        (1)
        #define ESP_PANEL_BOARD_TOUCH_I2C_IO_SCL            (2)    // LCD_SCL (shared with SPI)
        #define ESP_PANEL_BOARD_TOUCH_I2C_IO_SDA            (1)    // LCD_SDA (shared with SPI)
        #define ESP_PANEL_BOARD_TOUCH_I2C_ADDRESS           (0x14)
    #endif

    #define ESP_PANEL_BOARD_TOUCH_SWAP_XY           (0)
    #define ESP_PANEL_BOARD_TOUCH_MIRROR_X          (0)
    #define ESP_PANEL_BOARD_TOUCH_MIRROR_Y          (0)

    #define ESP_PANEL_BOARD_TOUCH_RST_IO            (-1)
    #define ESP_PANEL_BOARD_TOUCH_RST_LEVEL         (0)
    #define ESP_PANEL_BOARD_TOUCH_INT_IO            (-1)
    #define ESP_PANEL_BOARD_TOUCH_INT_LEVEL         (0)

#endif // ESP_PANEL_BOARD_USE_TOUCH

////////// BACKLIGHT //////////
// Backlight is via IO expander (not a direct MCU GPIO).
// Handle in board_init() / board_set_backlight() instead.
#define ESP_PANEL_BOARD_USE_BACKLIGHT           (0)

////////// IO EXPANDER //////////
// Expander managed manually (V3/V4 detection cannot be expressed as a static config).
#define ESP_PANEL_BOARD_USE_EXPANDER            (0)

////////// File version — do not change //////////
#define ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MAJOR 1
#define ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MINOR 2
#define ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_PATCH 0

// *INDENT-ON*