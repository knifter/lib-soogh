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

// RGB timing — from confirmed-working reference project
// VBP=40 is intentionally larger than PORCTRL register value (13) for interface stability.
// PCLK_ACTIVE_NEG=1: panel samples on falling edge (critical — wrong polarity = corrupt pixels).
#define ESP_PANEL_BOARD_LCD_RGB_CLK_HZ          (16 * 1000 * 1000)
#define ESP_PANEL_BOARD_LCD_RGB_HPW             (8)
#define ESP_PANEL_BOARD_LCD_RGB_HBP             (40)
#define ESP_PANEL_BOARD_LCD_RGB_HFP             (20)
#define ESP_PANEL_BOARD_LCD_RGB_VPW             (8)
#define ESP_PANEL_BOARD_LCD_RGB_VBP             (20)
#define ESP_PANEL_BOARD_LCD_RGB_VFP             (10)
#define ESP_PANEL_BOARD_LCD_RGB_PCLK_ACTIVE_NEG (0)

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
// #define ESP_PANEL_BOARD_LCD_RGB_BOUNCE_BUF_SIZE             (0)   // No bounce buffer - use framebuffer directly (zero-copy mode)

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

// IO multiplex: set to 1 so the 3-wire SPI init is sent immediately inside
// esp_lcd_new_panel_st7701_rgb() (before RGB DMA starts), then the SPI IO is
// deleted. This is the same flow as the confirmed-working reference project.
#define ESP_PANEL_BOARD_LCD_FLAGS_ENABLE_IO_MULTIPLEX   (0)
#define ESP_PANEL_BOARD_LCD_FLAGS_MIRROR_BY_CMD         (0)

// Vendor init sequence for Waveshare ESP32-S3-Touch-LCD-4 (ST7701, 480x480).
// Source: vendor_specific_init_default[] from the confirmed-working reference
// project (Marine-Displays/ESP32-S3_Square_Display, esp_lcd_st7701_rgb.c).
// The v1.0.5 library default is a generic reference sequence for a different
// panel variant — do not remove this override.
//
// Format: {cmd, {data bytes}, data_len, delay_ms}
// clang-format off
#define ESP_PANEL_BOARD_LCD_VENDOR_INIT_CMD()                                                                                           \
    {                                                                                                                                   \
        {0xFF, (uint8_t[]){0x77, 0x01, 0x00, 0x00, 0x10}, 5,   0}, /* CMD2 Bank0: unlock CMD2, select Bank 0 */                       \
        {0xC0, (uint8_t[]){0x3B, 0x00},                   2,   0}, /* LNESET: 480 display lines (0x3B+1)*8, no line-delay */          \
        {0xC1, (uint8_t[]){0x0D, 0x02},                   2,   0}, /* PORCTRL: VBP=13 lines, VFP=2 lines (vertical porch) */          \
        {0xC2, (uint8_t[]){0x31, 0x05},                   2,   0}, /* INVSET: inversion timing and frame-rate control */              \
        {0xCD, (uint8_t[]){0x08},                          1,   0}, /* COLCTRL: 18-bit color (RGB666) on the RGB data bus */           \
        {0xB0, (uint8_t[]){0x00, 0x11, 0x18, 0x0E, 0x11, 0x06, 0x07, 0x08, 0x07, 0x22, 0x04, 0x12, 0x0F, 0xAA, 0x31, 0x18}, 16, 0}, /* PVGAMCTRL: positive gamma (16 pts) */  \
        {0xB1, (uint8_t[]){0x00, 0x11, 0x19, 0x0E, 0x12, 0x07, 0x08, 0x08, 0x08, 0x22, 0x04, 0x11, 0x11, 0xA9, 0x32, 0x18}, 16, 0}, /* NVGAMCTRL: negative gamma (16 pts) */  \
        {0xFF, (uint8_t[]){0x77, 0x01, 0x00, 0x00, 0x11}, 5,   0}, /* CMD2 Bank1: select Bank 1 */                                    \
        {0xB0, (uint8_t[]){0x60},                          1,   0}, /* VRHS: Vop panel supply voltage = 4.7375 V */                    \
        {0xB1, (uint8_t[]){0x32},                          1,   0}, /* VCOMS: VCOM voltage = 0.725 V (affects contrast/flicker) */     \
        {0xB2, (uint8_t[]){0x07},                          1,   0}, /* VGHSS: VGH gate-high voltage = 15 V */                          \
        {0xB3, (uint8_t[]){0x80},                          1,   0}, /* TESTCMD: internal test register, keep at 0x80 (default) */      \
        {0xB5, (uint8_t[]){0x49},                          1,   0}, /* VGLS: VGL gate-low voltage = -10.17 V */                        \
        {0xB7, (uint8_t[]){0x85},                          1,   0}, /* PWCTRL1: gate driver power control */                           \
        {0xB8, (uint8_t[]){0x21},                          1,   0}, /* PWCTRL2: AVDD = 6.6 V, AVCL = -4.6 V */                        \
        {0xC1, (uint8_t[]){0x78},                          1,   0}, /* SPD1: source pre-charge period 1 */                             \
        {0xC2, (uint8_t[]){0x78},                          1,   0}, /* SPD2: source pre-charge period 2 */                             \
        {0xE0, (uint8_t[]){0x00, 0x1B, 0x02},              3,   0}, /* MIPISET1: MIPI/interface timing */                              \
        {0xE1, (uint8_t[]){0x08, 0xA0, 0x00, 0x00, 0x07, 0xA0, 0x00, 0x00, 0x00, 0x44, 0x44}, 11, 0}, /* SETGIP1: odd-column ST timing, period, polarity */  \
        {0xE2, (uint8_t[]){0x11, 0x11, 0x44, 0x44, 0xED, 0xA0, 0x00, 0x00, 0xEC, 0xA0, 0x00, 0x00}, 12, 0}, /* SETGIP2: even-column ST timing */              \
        {0xE3, (uint8_t[]){0x00, 0x00, 0x11, 0x11},        4,   0}, /* SETGIP3: clock group control (odd) */                           \
        {0xE4, (uint8_t[]){0x44, 0x44},                    2,   0}, /* SETGIP4: clock voltage level (odd) */                           \
        {0xE5, (uint8_t[]){0x0A, 0xE9, 0xD8, 0xA0, 0x0C, 0xEB, 0xD8, 0xA0, 0x0E, 0xED, 0xD8, 0xA0, 0x10, 0xEF, 0xD8, 0xA0}, 16, 0}, /* SETGIP5: odd STx signal timing (4 signals x 4 bytes) */   \
        {0xE6, (uint8_t[]){0x00, 0x00, 0x11, 0x11},        4,   0}, /* SETGIP6: clock group control (even, mirrors E3) */              \
        {0xE7, (uint8_t[]){0x44, 0x44},                    2,   0}, /* SETGIP7: clock voltage level (even, mirrors E4) */              \
        {0xE8, (uint8_t[]){0x09, 0xE8, 0xD8, 0xA0, 0x0B, 0xEA, 0xD8, 0xA0, 0x0D, 0xEC, 0xD8, 0xA0, 0x0F, 0xEE, 0xD8, 0xA0}, 16, 0}, /* SETGIP8: even STx signal timing (4 signals x 4 bytes) */  \
        {0xEB, (uint8_t[]){0x02, 0x00, 0xE4, 0xE4, 0x88, 0x00, 0x40}, 7, 0}, /* SETGIP9: equalizer timing and function enable */       \
        {0xEC, (uint8_t[]){0x3C, 0x00},                    2,   0}, /* SETGIP10: source OP control in normal/idle mode */              \
        {0xED, (uint8_t[]){0xAB, 0x89, 0x76, 0x54, 0x02, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x45, 0x67, 0x98, 0xBA}, 16, 0}, /* SETGIP11: pin map: controller to panel gate pins */         \
        {0xFF, (uint8_t[]){0x77, 0x01, 0x00, 0x00, 0x13}, 5,   0}, /* CMD2 Bank3: select Bank 3 */                                    \
        {0xE5, (uint8_t[]){0xE4},                          1,   0}, /* NVGAMCTRL (Bank3): VAP/VAN output voltage fine-tune */          \
        {0xFF, (uint8_t[]){0x77, 0x01, 0x00, 0x00, 0x00}, 5,   0}, /* CMD2 disable: return to standard MIPI DCS command set */        \
        {0x21, (uint8_t[]){0x00},                          0,   0}, /* INVON: display inversion ON (IPS panel requires this) */        \
        {0x3A, (uint8_t[]){0x60},                          1,   0}, /* COLMOD: pixel format 0x60 = 18-bit/pixel RGB666 */              \
        {0x11, (uint8_t[]){0x00},                          0, 120}, /* SLPOUT: exit sleep mode; 120 ms delay required */               \
        {0x29, (uint8_t[]){0x00},                          0,   0}, /* DISPON: enable display output */                                \
    }
// clang-format on

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
#define ESP_PANEL_BOARD_EXPANDER_SKIP_INIT_HOST   (1)     // 0/1

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
