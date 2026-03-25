# Async DMA Flush for LVGL

## Problem

`lv_disp_cb` (the LVGL display flush callback in `soogh-lgfx.cpp`) originally called
`_lgfx.endWrite()` synchronously immediately after `pushImageDMA()`:

```cpp
_lgfx.startWrite();
_lgfx.pushImageDMA(...);
_lgfx.endWrite();              // blocks until DMA transfer completes
lv_disp_flush_ready(disp);
```

`endWrite()` internally calls `waitDMA()`, which blocks the CPU for the entire SPI
transfer. At 40 MHz SPI with a 1/10-screen buffer (7,680 pixels = 15,360 bytes), each
transfer takes ~3 ms. A full-screen redraw requires ~10 transfers: **~30–50 ms of CPU
stall per frame**, during which PID loops, sensor reads, and the CLI are all frozen.

---

## Solution

Decouple the DMA transfer from the LVGL "I'm done" signal. Start DMA and return
immediately from `lv_disp_cb`. A separate `lgfx_check_flush()` function polls
`_lgfx.dmaBusy()` and calls `lv_disp_flush_ready()` once the hardware is finished.
`lgfx_check_flush()` is called at the top of `SooghGUI::loop()` each iteration, so the
main loop can do useful work (PID, sensors, CLI) while DMA is in flight.

---

## Changeset

### `soogh-lgfx.cpp`

Added a static to track the in-flight flush, a new `lgfx_check_flush()` function, and
reworked `lv_disp_cb` to not block:

```cpp
static lv_disp_drv_t* _pending_flush_disp = nullptr;

void lgfx_check_flush()
{
    if(_pending_flush_disp && !_lgfx.dmaBusy())
    {
        _lgfx.endWrite();
        lv_disp_flush_ready(_pending_flush_disp);
        _pending_flush_disp = nullptr;
    }
}

static void lv_disp_cb(lv_disp_drv_t* disp, const lv_area_t* area, lv_color_t* color_p)
{
    // Safety: if previous DMA not yet drained, finish it now before starting the next
    if(_pending_flush_disp)
    {
        _lgfx.endWrite();
        lv_disp_flush_ready(_pending_flush_disp);
        _pending_flush_disp = nullptr;
    }

    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    _lgfx.startWrite();
    _lgfx.pushImageDMA(area->x1, area->y1, w, h, (lgfx::rgb565_t *)&color_p->full);
    // Don't call endWrite() — DMA is running, CPU is free
    _pending_flush_disp = disp;
    // lv_disp_flush_ready() called later from lgfx_check_flush()
}
```

### `soogh-lgfx.h`

```cpp
void lgfx_check_flush();
```

### `soogh-gui.cpp`

```cpp
time_t SooghGUI::loop()
{
    lgfx_check_flush();   // signal LVGL if DMA completed since last loop
    // ...
}
```

---

## Why lgfx_check_flush() must run in loop(), not only in lv_disp_cb

`lv_disp_flush_ready()` is not just a hardware notification — it is the gate LVGL blocks
on. Until it is called, LVGL's internal refresh task marks the display as busy and will
not render or call `flush_cb` again, regardless of how many times `lv_timer_handler()` is
called.

If `lv_disp_flush_ready()` were only called inside `lv_disp_cb` (the safety drain at the
top), the following deadlock would occur for any frame with a single dirty region (the
common case when only labels update):

1. LVGL calls `lv_disp_cb` → DMA starts → we store the pointer and return
2. `lv_timer_handler()` returns (nothing more this frame)
3. Next `gui.loop()`: `lv_timer_handler()` is called — LVGL sees display still **busy**,
   skips rendering, returns immediately
4. Repeat → **display freezes**

LVGL will not call `flush_cb` again because it is waiting for `lv_disp_flush_ready()`.
The drain only runs when `flush_cb` is called again. Neither side moves: deadlock.

`lgfx_check_flush()` at the top of `loop()` breaks this by polling outside LVGL's call
graph. It must run **before** `lv_timer_handler()` so that if DMA has finished since the
last loop, the display is marked free before LVGL attempts to render the next frame.

The safety drain inside `lv_disp_cb` handles the rarer case of multiple flush regions
within one frame (e.g. a full-screen transition where LVGL calls `flush_cb` ten times in
rapid succession). It ensures each transfer completes before the next begins — but it is
defensive only; `lgfx_check_flush()` in `loop()` would have already handled it.

---

## Why _pending_flush_disp is a pointer, not a bool

`lv_disp_flush_ready()` requires the `lv_disp_drv_t*` that was passed into `flush_cb` as
its argument — LVGL uses it to identify which display to unblock. The pointer must
therefore be stored to make the deferred call.

It conveniently doubles as the "pending" flag (`nullptr` = idle, non-null = DMA in
flight), but that is incidental. A `bool` plus a separate stored pointer would work
equally well. Alternatively, `lv_disp_get_default()` returns the default display and
would remove the need to store the pointer at all — but using the argument passed by LVGL
is more self-contained and does not rely on the single-display assumption.
