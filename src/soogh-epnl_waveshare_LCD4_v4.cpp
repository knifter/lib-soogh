#include "soogh-epnl_waveshare_LCD4.h"

#ifdef SOOGH_DEV_WAVESHARE_LCD4
#if WAVESHARE_LCD_VERSION != 3

#include <Wire.h>
#include <Arduino.h>

#define ADDR  0x24   // CH32V003

#define REG_INPUT   0x00
#define REG_OUTPUT  0x02   // CH32V003 output latch
#define REG_DIR     0x03
#define REG_PWM     0x05   // backlight PWM (0=max, 247=off)

#define V4_TP_RST_BIT   (1 << 1)    // EXIO1
#define V4_TP_INT_BIT   (1 << 2)    // EXIO2
#define V4_LCD_RST_BIT  (1 << 3)    // EXIO3
#define V4_SDCD_BIT     (1 << 4)    // EXIO4
#define V4_SYS_EN_BIT   (1 << 5)    // EXIO5
#define V4_BEE_EN_BIT   (1 << 6)    // EXIO6
#define V4_RTC_INT_BIT  (1 << 7)    // EXIO7

#if !defined(GT911_ADDR_HIGH)
    #define GT911_ADDR_HIGH     1
#endif

#if GT911_ADDR_HIGH == 1
    #define GT911_ADDR_BIT  (V4_TP_INT_BIT)  // TP_INT HIGH → GT911@0x5D (default)
#else
    #define GT911_ADDR_BIT  (0)              // TP_INT LOW  → GT911@0x14
#endif

static void exp_write(uint8_t reg, uint8_t val)
{
    Wire.beginTransmission(ADDR);
    Wire.write(reg);
    Wire.write(val);
    Wire.endTransmission();
};

static uint8_t exp_read(uint8_t reg)
{
    Wire.beginTransmission(ADDR);
    Wire.write(reg);
    Wire.endTransmission(false);
    Wire.requestFrom(ADDR, (uint8_t)1);
    return Wire.available() ? Wire.read() : 0xFF;
};

void waveshare_lcd4_init()
{
    // Init sequence:
    //      EXIO1    EXIO2   EXIO3   EXIO4   EXIO5   EXIO6   EXIO7
    //      TP_RST   TP_INT  LCD_RST SDCD    SYS_EN  BEE_EN  RTC_INT   HEX
    // 0:   OUT      OUT     OUT     OUT     OUT     OUT     IN         0x7E  // Config TP_INT as output as well
    // 1:   1        1       1       0       1       0       0          0x2E  // init, TP_INT = HIGH = GT911@0x5D
    // 2:   0        1       0       0       1       0       0          0x24  // rst LCD+GT11
    // 3:   1        1       1       0       1       0       0          0x2E  // rst off: GT911 samples TP_INT for address
    // 4:   1        0       1       0       1       0       0          0x2A  // Don't drive TP_INT and
    // 5:   OUT      IN      OUT     OUT     OUT     OUT     IN         0x7A  // restore TP_INT as INPUT

    // Config TP_INT as output as well
    exp_write(REG_DIR,      V4_TP_RST_BIT | V4_TP_INT_BIT | V4_LCD_RST_BIT | V4_SDCD_BIT | V4_SYS_EN_BIT | V4_BEE_EN_BIT);
    // init, TP_INT = HIGH = GT911@0x5D
    exp_write(REG_OUTPUT,   V4_TP_RST_BIT | GT911_ADDR_BIT | V4_LCD_RST_BIT | 0           | V4_SYS_EN_BIT | 0            );
    delay(10);

    // rst LCD+GT11
    exp_write(REG_OUTPUT,   0             | GT911_ADDR_BIT | 0              | 0           | V4_SYS_EN_BIT | 0            );
    delay(5);

    // rst off: GT911 samples TP_INT for address
    exp_write(REG_OUTPUT,   V4_TP_RST_BIT | GT911_ADDR_BIT | V4_LCD_RST_BIT | 0           | V4_SYS_EN_BIT | 0            );
    delay(200); // GT911: ≥200ms before first I2C

    // Don't drive TP_INT and
    exp_write(REG_OUTPUT,   V4_TP_RST_BIT | 0              | V4_LCD_RST_BIT | 0           | V4_SYS_EN_BIT | 0            );
    // restore TP_INT as INPUT
    exp_write(REG_DIR,      V4_TP_RST_BIT | 0              | V4_LCD_RST_BIT | V4_SDCD_BIT | V4_SYS_EN_BIT | V4_BEE_EN_BIT);

    // Serial.println("[BOARD] waveshare_lcd4 v4 init done");
};

void waveshare_lcd4_set_backlight(uint8_t pct)
{
    // PWM: 0=max brightness, 247=off (inverted scale)
    uint8_t duty = (pct == 0) ? 247 : (uint8_t)(247 - ((uint32_t)pct * 247 / 100));
    exp_write(REG_PWM, duty);
};

void waveshare_lcd4_beep(bool on)
{
    // NOTE: Beeper on/off requires a small delay (10ms? less?) after the call. Dunno why. uC turned IO-Expander shitty program mode magic I guess.
    uint8_t cur = exp_read(REG_OUTPUT);
    if(on)
        exp_write(REG_OUTPUT, cur |  V4_BEE_EN_BIT);
    else
        exp_write(REG_OUTPUT, cur & ~V4_BEE_EN_BIT);
};

#endif // WAVESHARE_LCD_VERSION != 3
#endif // SOOGH_DEV_WAVESHARE_LCD4
