#include "soogh-epnl_waveshare_LCD4.h"

#include <Wire.h>
#include <Preferences.h>
#include <Arduino.h>

// ── I2C ────────────────────────────────────────────────────────────────────
#define I2C_SDA  15
#define I2C_SCL  7

// ── Expander I2C addresses ──────────────────────────────────────────────────
#define ADDR_V3  0x20   // TCA9554PWR
#define ADDR_V4  0x24   // CH32V003

// ── Register map ───────────────────────────────────────────────────────────
// Both chips have INPUT at 0x00; everything else differs (see guide header).
#define REG_INPUT       0x00
#define REG_OUTPUT_V3   0x01   // TCA9554 output latch
#define REG_OUTPUT_V4   0x02   // CH32V003 output latch (moved!)
#define REG_DIR         0x03   // Direction register (polarity inverted on V4)
#define REG_PWM_V4      0x05   // CH32V003 backlight PWM (0=max, 247=off)

// ── Pin bit masks ───────────────────────────────────────────────────────────
// V3: LCD_RST=bit2, BL=bit1 | V4: LCD_RST=bit3, SYS_EN=bit5
#define V3_LCD_RST_BIT  (1 << 2)
#define V3_BL_BIT       (1 << 1)
#define V4_LCD_RST_BIT  (1 << 3)
#define V4_SYS_EN_BIT   (1 << 5)

// ── V4 direction register ───────────────────────────────────────────────────
// 0x3A = 0b0011_1010 → bits 1,3,4,5 are outputs; bits 0,2,6,7 are inputs.
// (In CH32V003: 1=output, 0=input — inverted from TCA9554.)
// BEE_EN (bit 6) kept as input to prevent accidental buzzer activation.
#define V4_DIR_SAFE  0x3A

// ── Runtime state ───────────────────────────────────────────────────────────
static uint8_t g_addr = ADDR_V3;
static bool    g_v4   = false;

// ── I2C helpers ─────────────────────────────────────────────────────────────
static void exp_write(uint8_t reg, uint8_t val)
{
    Wire.beginTransmission(g_addr);
    Wire.write(reg);
    Wire.write(val);
    Wire.endTransmission();
}

static uint8_t exp_read(uint8_t reg)
{
    Wire.beginTransmission(g_addr);
    Wire.write(reg);
    Wire.endTransmission(false);
    Wire.requestFrom(g_addr, (uint8_t)1);
    return Wire.available() ? Wire.read() : 0xFF;
}

static uint8_t out_reg() { return g_v4 ? REG_OUTPUT_V4 : REG_OUTPUT_V3; }

// ── Version detection ────────────────────────────────────────────────────────
static void detect_version()
{
    // Check NVS cache first to avoid re-probing (V4 probe can disturb CH32V003)
    Preferences prefs;
    uint8_t cached = 0xFF;
    if (prefs.begin("board", true)) {
        cached = prefs.getUChar("ver", 0xFF);
        prefs.end();
    }

    if (cached == 1) { g_addr = ADDR_V4; g_v4 = true;  return; }
    if (cached == 0) { g_addr = ADDR_V3; g_v4 = false; return; }

    // First boot: probe. V4 probe may disturb CH32V003 — display may fail once,
    // recovers on reboot when cache is hit.
    Wire.beginTransmission(ADDR_V4);
    bool v4 = (Wire.endTransmission() == 0);

    g_v4   = v4;
    g_addr = v4 ? ADDR_V4 : ADDR_V3;

    if (prefs.begin("board", false)) {
        prefs.putUChar("ver", v4 ? 1 : 0);
        prefs.end();
    }

    Serial.printf("[BOARD] Detected %s (0x%02X)\n", v4 ? "V4/CH32V003" : "V3/TCA9554", g_addr);
}

// ── Public API ───────────────────────────────────────────────────────────────
void waveshare_lcd4_init()
{
    Wire.begin(I2C_SDA, I2C_SCL);
    detect_version();

    if (g_v4) {
        // CH32V003: direction polarity inverted (1=output, 0=input)
        exp_write(REG_DIR, V4_DIR_SAFE);
        // Enable SYS_EN (LCD power rail) and set everything else high
        exp_write(REG_OUTPUT_V4, 0xFF);
        delay(10);
    } else {
        // TCA9554: direction polarity normal (0=output, 1=input)
        exp_write(REG_DIR, 0x00);   // all pins output
        exp_write(REG_OUTPUT_V3, 0xFF);
        delay(10);
    }

    // Pulse LCD_RST LOW → HIGH
    uint8_t rst_bit = g_v4 ? V4_LCD_RST_BIT : V3_LCD_RST_BIT;
    uint8_t cur     = exp_read(out_reg());
    exp_write(out_reg(), cur & ~rst_bit);
    delay(20);
    exp_write(out_reg(), cur | rst_bit);
    delay(120);

    Serial.println("[BOARD] board_init done, LCD reset pulsed");
}

void waveshare_lcd4_set_backlight(uint8_t pct)
{
    if (g_v4) {
        // CH32V003 PWM: 0=max brightness, 247=off (inverted scale)
        uint8_t duty = (pct == 0) ? 247 : (uint8_t)(247 - ((uint32_t)pct * 247 / 100));
        exp_write(REG_PWM_V4, duty);
    } else {
        // TCA9554: backlight is a simple on/off via bit 1
        uint8_t cur = exp_read(REG_OUTPUT_V3);
        if (pct > 0)
            exp_write(REG_OUTPUT_V3, cur |  V3_BL_BIT);
        else
            exp_write(REG_OUTPUT_V3, cur & ~V3_BL_BIT);
    }
}

bool waveshare_lcd4_is_v4() 
{ 
    return g_v4; 
};
