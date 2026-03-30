#include "soogh-lgfx.h"

#include "soogh-conf.h"

// #define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <lvgl.h>
#include "esp_heap_caps.h"

#define LV_BUF_SIZE         (DISPLAY_HEIGHT*DISPLAY_WIDTH / 10)         // Atleast 1/10th of the screen, but increase for higher performance
static uint8_t*             _lv_buf1 = nullptr;
static uint8_t*             _lv_buf2 = nullptr;                         // Only used in double-buffering, nullptr otherwise

static lv_display_t*        _lv_disp = nullptr;                         // The LV Display
static lv_display_t*        _pending_flush_disp = nullptr;              // Probably the above display or nullptr: used in the display flush cb
static void                 lv_disp_cb(lv_display_t*, const lv_area_t*, uint8_t*);
static inline void          lv_flush_wait_cb(lv_display_t*);
static inline void          lgfx_check_flush();

#ifdef SOOGH_TOUCH
    static lv_indev_t*      _lv_touch_indev = nullptr;
    static void             lv_touchpad_cb(lv_indev_t*, lv_indev_data_t*);
#endif

#ifdef SOOGH_ENCODER_KEYS
    uint32_t            lvgl_enc_last_key = 0;
    bool                lvgl_enc_pressed = false;
    lv_indev_t*         lvgl_indev_keyenc = nullptr;
    static void lv_keys_cb(lv_indev_t*, lv_indev_data_t*);
#endif

// Device Configs
#if defined(SOOGH_DEV_M5CORE) || defined(SOOGH_DEV_M5CORE2)
// M%Stack has Lovyan Auto-detect, apparently
LGFX _lgfx;
#endif

#include "soogh_lgfx_wt32sc01.h"

void lgfx_init()
{
    _lgfx.init();
    _lgfx.setRotation(SOOGH_DISP_ROTATE);
};

#if LV_USE_LOG
void serial_log_cb(const char* line)
{
    Serial.print(line);
};
#endif

void lvgl_init()
{
    // Allocate DMA-capable byte buffers (RGB565 = 2 bytes per pixel)
    _lv_buf1 = (uint8_t*) heap_caps_malloc(LV_BUF_SIZE * 2, MALLOC_CAP_INTERNAL | MALLOC_CAP_DMA);
    if(!_lv_buf1)
    {
        Serial.printf("Alloc(buf1) failed! Incoming...");
        // We're in for a world of hurt.. So lets continue, just for the fun of it
    };
    _lv_buf2 = nullptr;
#ifdef SOOGH_DOUBLEBUF
    _lv_buf2 = (uint8_t*) heap_caps_malloc(LV_BUF_SIZE * 2, MALLOC_CAP_INTERNAL | MALLOC_CAP_DMA);
    if(!_lv_buf2)
    {
        Serial.printf("Alloc(buf2) failed. Disabling Double-Buffering.");
        _lv_buf2 = nullptr;
    };
#endif

    lv_init();

#if LV_USE_LOG
    lv_log_register_print_cb(serial_log_cb);
#endif

// Display creation
    lv_display_t* _lv_display = lv_display_create(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    lv_display_set_color_format(_lv_display, LV_COLOR_FORMAT_RGB565);
    lv_display_set_buffers(_lv_display, _lv_buf1, _lv_buf2, LV_BUF_SIZE * 2, LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_flush_cb(_lv_display, lv_disp_cb);
    lv_display_set_flush_wait_cb(_lv_display, lv_flush_wait_cb);

#ifdef SOOGH_TOUCH
    _lv_touch_indev = lv_indev_create();
    lv_indev_set_type(_lv_touch_indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(_lv_touch_indev, lv_touchpad_cb);
#endif

#ifdef SOOGH_ENCODER_KEYS
    lvgl_indev_keyenc = lv_indev_create();
    lv_indev_set_type(lvgl_indev_keyenc, LV_INDEV_TYPE_ENCODER);
    lv_indev_set_read_cb(lvgl_indev_keyenc, lv_keys_cb);

    lvgl_enc_last_key = 0;
    lvgl_enc_pressed = false;
#endif

    _lgfx.initDMA();
};

static inline void lv_flush_wait_cb(lv_display_t*)
{
    lgfx_check_flush();

    yield();
};

static inline void lgfx_check_flush()
{
    if(_pending_flush_disp && !_lgfx.dmaBusy())
    {
        _lgfx.endWrite();
        lv_display_flush_ready(_pending_flush_disp);
        _pending_flush_disp = nullptr;
    };
};


// _pending_flush_disp is non-null while a DMA transfer is in flight.
// We reuse it as the "pending flush" pointer.
static void lv_disp_cb(lv_display_t* disp, const lv_area_t* area, uint8_t* color_p)
{
    // If previous DMA transfer hasn't finished, drain it first
    if(_pending_flush_disp)
    {
        //Q: Does endWrite wait for DMA finish as well?
        while(_lgfx.dmaBusy());
        _lgfx.endWrite();
        lv_display_flush_ready(_pending_flush_disp);
        _pending_flush_disp = nullptr;
    };

    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );

    // Start SPI bus (CS-pin) and start writing pixel data through DMA
    _lgfx.startWrite();
#if defined(SOOGH_DEV_M5CORE) || defined(SOOGH_DEV_M5CORE2)
    _lgfx.pushImageDMA(area->x1, area->y1, w, h, (lgfx::rgb565_t*)color_p);
#endif
#ifdef SOOGH_DEV_WT32SC01
    //_lgfx.pushImageDMA(area->x1, area->y1, w, h, (lgfx::swap565_t*)color_p);
#endif
    // DMA running — call flush_ready asynchronously via lgfx_check_flush()
    _pending_flush_disp = disp;
};

#ifdef SOOGH_TOUCH
static void lv_touchpad_cb(lv_indev_t* indev, lv_indev_data_t* data)
{
    uint16_t touchX, touchY;
    if(!_lgfx.getTouch(&touchX, &touchY))
    {
        data->state = LV_INDEV_STATE_RELEASED;
        return;
    };
    data->state = LV_INDEV_STATE_PRESSED;
    data->point.x = touchX;
    data->point.y = touchY;
};
#endif

#ifdef SOOGH_ENCODER_KEYS
static void lv_keys_cb(lv_indev_t* indev, lv_indev_data_t* data)
{
    data->key = lvgl_enc_last_key;

    if(lvgl_enc_pressed)
        data->state = LV_INDEV_STATE_PRESSED;
    else
        data->state = LV_INDEV_STATE_RELEASED;
};
#endif
