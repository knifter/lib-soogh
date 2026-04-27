#include "soogh-epnl_waveshare_LCD4.h"

#ifdef SOOGH_DEV_WAVESHARE_LCD4
#if WAVESHARE_LCD_VERSION == 3

#include <Wire.h>
#include <Arduino.h>

#define ADDR            0x20   // TCA9554PWR
#define V3_TP_INT_GPIO  16     // TP_INT wired directly to ESP32

#define REG_INPUT   0x00
#define REG_OUTPUT  0x01
#define REG_DIR     0x03

#define V3_TP_RST_BIT   (1 << 0)
#define V3_BL_BIT       (1 << 1)
#define V3_LCD_RST_BIT  (1 << 2)
#define V3_SDCD_BIT     (1 << 3)
#define V3_BEE_EN_BIT   (1 << 5)
#define V3_RTC_INT_BIT  (1 << 6)

#if !defined(GT911_ADDR_HIGH)
    #define GT911_ADDR_HIGH     0
#endif
#if GT911_ADDR_HIGH == 1
    #define GT911_ADDR_STATE       HIGH
#else
    #define GT911_ADDR_STATE       LOW
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
    // Init sequence (TP_INT/GT911 address select via GPIO16, not expander):
    //      BIT0     BIT1    BIT2     BIT3    BIT5    BIT6
    //      TP_RST   BL      LCD_RST  SDCD    BEE_EN  RTC_INT   HEX
    // 0:   OUT      OUT     OUT      OUT     OUT     OUT        0x00  // DIR: all output (TCA9554: 0=output)
    // 1:   1        1       1        0       0       0          0x07  // init; GPIO16=HIGH → GT911@0x5D
    // 2:   0        1       0        0       0       0          0x02  // rst TP+LCD
    // 3:   1        1       1        0       0       0          0x07  // un-rst: GT911 latches address; restore GPIO16 as input

    exp_write(REG_DIR,    0x00);
    exp_write(REG_OUTPUT, V3_TP_RST_BIT | V3_BL_BIT | V3_LCD_RST_BIT | 0 | 0 | 0);
    delay(10);

    pinMode(V3_TP_INT_GPIO, OUTPUT);
    digitalWrite(V3_TP_INT_GPIO, GT911_ADDR_STATE);

    exp_write(REG_OUTPUT, 0             | V3_BL_BIT | 0              | 0 | 0 | 0);
    delay(5);
    exp_write(REG_OUTPUT, V3_TP_RST_BIT | V3_BL_BIT | V3_LCD_RST_BIT | 0 | 0 | 0);
    delay(200); // GT911: ≥200ms before first I2C

    pinMode(V3_TP_INT_GPIO, INPUT);

    Serial.println("[BOARD] waveshare_lcd4 v3 init done");
};

void waveshare_lcd4_set_backlight(uint8_t pct)
{
    uint8_t cur = exp_read(REG_OUTPUT);
    if(pct > 0)
        exp_write(REG_OUTPUT, cur |  V3_BL_BIT);
    else
        exp_write(REG_OUTPUT, cur & ~V3_BL_BIT);
};

void waveshare_lcd4_beep(bool on)
{
    uint8_t cur = exp_read(REG_OUTPUT);
    if(on)
        exp_write(REG_OUTPUT, cur |  V3_BEE_EN_BIT);
    else
        exp_write(REG_OUTPUT, cur & ~V3_BEE_EN_BIT);
};

#endif // WAVESHARE_LCD_VERSION == 3
#endif // SOOGH_DEV_WAVESHARE_LCD4
