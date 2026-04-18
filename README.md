# lib-soogh v0.10

v0.8 frozen LVGL8 version.
v0.9 is the new LVGL9 version. For the previous LVGL8 use the LVGL8/v0.8 branch
v0.10 Moved LVGL handling to separate task.

Object-oriented GUI framework for LVGL v9 + LovyanGFX on ESP32.

Provides a screen stack, event system, and hardware bring-up layer for embedded displays. Targets M5Stack Core, M5Stack Core2, WT32-SC01, and Waveshare LCD4.

## Features

- **Screen stack** — push/pop `Screen` objects; each screen owns its LVGL widgets and lifecycle (`init`, `load`, `loop`, `close`)
- **Event routing** — hardware button events (short, long, double, combo) mapped to `soogh_event_t` and dispatched to the active screen
- **Hardware abstraction** — one `#define` selects the target device; display size, rotation, touch, double-buffering, and DMA flush are configured automatically
- **LVGL group management** — push/pop `lv_group_t` in sync with the screen stack for correct focus handling
- **Message box** — `showMessage()` for modal alerts from anywhere

## Supported devices

| Build flag                  | Device               | Resolution | Touch | Double-buf |
|-----------------------------|----------------------|------------|-------|------------|
| `SOOGH_DEV_M5CORE`          | M5Stack Core         | 320×240    | No    | No         |
| `SOOGH_DEV_M5CORE2`         | M5Stack Core2        | 320×240    | Yes   | No         |
| `SOOGH_DEV_WT32SC01`        | WT32-SC01 (ESP32-S3) | 480×320    | Yes   | Yes        |
| `SOOGH_DEV_WAVESHARE_LCD4`  | Waveshare LCD4       | 480×480    | Yes   | Yes        |

## Dependencies

- [LVGL v9.2](https://github.com/lvgl/lvgl)
- [LovyanGFX](https://github.com/lovyan03/LovyanGFX)
- [lib-tools](https://github.com/knifter/lib-tools) (KeyTools button debounce/event)

## Usage

### PlatformIO

```ini
lib_deps =
    https://github.com/lvgl/lvgl.git#release/v9.2
    https://github.com/lovyan03/LovyanGFX
    https://github.com/knifter/lib-tools
    https://github.com/knifter/lib-soogh

build_flags =
    -D SOOGH_DEV_M5CORE
```

### LVGL configuration

Copy `include/lv_conf_project.example` to your project's `include/lv_conf_project.h` and adjust as needed. The library ships its own `lv_conf.h` which includes your project file.

### Implementing a screen

Inherit from `Screen` and override the virtual methods you need:

```cpp
#include <soogh.h>

class MyScreen : public Screen
{
public:
    MyScreen(SooghGUI& gui) : Screen(gui) {}

    ScreenType type() override { return ScreenType::MAIN; }

    void init() override
    {
        // create LVGL widgets here, parented to _screen
        lv_obj_t* label = lv_label_create(_screen);
        lv_label_set_text(label, "Hello");
    }

    bool handle(soogh_event_t e) override
    {
        if(e == KEY_B_PRESSED) { _gui.popScreen(); return true; }
        return false;
    }

    void loop() override
    {
        // called every GUI tick
    }
};
```

### Application setup

```cpp
SooghGUI gui;

void setup()
{
    gui.begin();
    gui.pushScreen(std::make_shared<MyScreen>(gui));
}

void loop()
{
    soogh_event_t e = get_key_event(); // your button scanning
    gui.handle(e);
    gui.loop();
}
```

## DMA / SPI bus note

On M5Stack Core the display uses the VSPI peripheral via LovyanGFX DMA. If you share the SPI bus with another device (e.g. MAX31865), call `lgfx_check_flush()` after `gui.loop()` and before your SPI transactions to ensure the DMA transfer is complete and the bus is released:

```cpp
void loop()
{
    gui.handle(e);
    gui.loop();
    lgfx_check_flush();   // release SPI bus before using it elsewhere
    // safe to use SPI here
}
```
