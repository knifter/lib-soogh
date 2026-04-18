
#include "soogh-conf.h"

#include "soogh-epnl.h"

#include "tools-log.h"

#include <lvgl.h>

#include "soogh-lgfx.h"
#include "soogh-epnl.h"

bool lvgl_lock(uint32_t timeout_ms);     // Acquire LVGL mutex before calling any lv_* API
void lvgl_unlock(void);                  // Release after done
void lvgl_port_task(void *arg);          // FreeRTOS task entry point

static SemaphoreHandle_t    _lv_task_mutex = NULL;

static void                 lv_disp_flush_cb(lv_display_t*, const lv_area_t*, uint8_t*);
#ifdef SOOGH_USE_LGFX
#define LV_BUF_SIZE         (DISPLAY_HEIGHT*DISPLAY_WIDTH / 5)         // Atleast 1/10th of the screen, but increase for higher performance
static inline void          lv_disp_flush_wait_cb(lv_display_t*);
#endif
#ifdef SOOGH_USE_EPNL
static bool                 lv_vsync_cb(void *user_data);
#endif

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

#if LV_USE_LOG
void serial_log_cb(const char* line)
{
    Serial.print(line);
};
#endif

#if LV_USE_LOG
static void serial_log_cb(const char* line) { Serial.print(line); }
#endif

bool lvgl_init()
{
    lv_init();
    lv_tick_set_cb([]() -> uint32_t { return (uint32_t) millis(); });

#if LV_USE_LOG
    lv_log_register_print_cb(serial_log_cb);
#endif

#ifdef SOOGH_USE_LGFX
    // Allocate DMA-capable byte buffers (RGB565 = 2 bytes per pixel)
    void *fb0 = (uint8_t*) heap_caps_malloc(LV_BUF_SIZE * 2, MALLOC_CAP_INTERNAL | MALLOC_CAP_DMA);
    if(!fb0)
    {
        Serial.printf("Alloc(fb0) failed! Incoming...");
        return false;
    };
    void *fb1 = nullptr;
    #ifdef SOOGH_DOUBLEBUF
    fb1 = (uint8_t*) heap_caps_malloc(LV_BUF_SIZE * 2, MALLOC_CAP_INTERNAL | MALLOC_CAP_DMA);
    if(!fb1)
    {
        Serial.printf("Alloc(buf2) failed. Disabling Double-Buffering.");
        fb1 = nullptr;
    };
    #endif
#endif // SOOGH_LGFX
#ifdef SOOGH_USE_EPNL
    esp_panel::drivers::LCD *lcd = _epnl_board->getLCD();

    if(_epnl_board == nullptr)
    {
        Serial.printf("epnl_init() Not done.");
        return false;
    };

    // Use the panel's PSRAM framebuffers directly (DIRECT mode = no copy)
    void *fb0 = lcd->getFrameBufferByIndex(0);
    void *fb1 = nullptr;
    #ifdef SOOGH_DOUBLEBUF
    fb1 = lcd->getFrameBufferByIndex(1);
    #endif
#endif // SOOGH_LGFX

    // Create display
    lv_display_t* disp = lv_display_create(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    lv_display_set_color_format(disp, LV_COLOR_FORMAT_RGB565);
#ifdef SOOGH_USE_LGFX
    lv_display_set_buffers(disp, fb0, fb1, LV_BUF_SIZE * 2, LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_flush_wait_cb(disp, lv_disp_flush_wait_cb);
#endif
#ifdef SOOGH_USE_EPNL
    lv_display_set_buffers(disp, fb0, fb1,
                           DISPLAY_WIDTH * DISPLAY_HEIGHT * sizeof(uint16_t),
                           LV_DISPLAY_RENDER_MODE_DIRECT);
    lv_display_set_user_data(disp, lcd);
#endif
    lv_display_set_flush_cb(disp, lv_disp_flush_cb);

    // Optional: tear-free via VSync. Uncomment both lines and remove
    // lv_display_flush_ready() from flush_cb above.
    // lcd->attachRefreshFinishCallback(vsync_cb, (void *)lv_disp);

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

    // Display Mutex
    _lv_task_mutex = xSemaphoreCreateMutex();
    configASSERT(_lv_task_mutex);

    return true;
};

bool lvgl_start_task()
{
    // Run LVGL on a dedicated task (core 1, priority 5)
    //    Do NOT call lv_timer_handler() from loop() — use the task instead.
    xTaskCreatePinnedToCore(lvgl_port_task, "lvgl", 8192, nullptr, 5, nullptr, 1);

    return true;
};

// ── LVGL task (run on dedicated FreeRTOS task) ────────────────────────────────
void lvgl_port_task(void *arg)
{
    while (true) 
    {
        uint32_t next = 10; // ms
        if(lvgl_lock(10))
        {
            next = lv_timer_handler();
            lvgl_unlock();
        };
        vTaskDelay(pdMS_TO_TICKS(next));
    };
};

bool lvgl_lock(uint32_t timeout_ms)
{
    return xSemaphoreTake(_lv_task_mutex, pdMS_TO_TICKS(timeout_ms)) == pdTRUE;
};

void lvgl_unlock()
{
    xSemaphoreGive(_lv_task_mutex);
};

#ifdef SOOGH_USE_LGFX
// _pending_flush_disp is non-null while a DMA transfer is in flight.
// We reuse it as the "pending flush" pointer.
static void lv_disp_flush_cb(lv_display_t* disp, const lv_area_t* area, uint8_t* color_p)
{
    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );

    _lgfx.startWrite();
#if defined(SOOGH_DEV_M5CORE) || defined(SOOGH_DEV_M5CORE2)
    _lgfx.pushImageDMA(area->x1, area->y1, w, h, (lgfx::rgb565_t*)color_p);
#endif
#ifdef SOOGH_DEV_WT32SC01
    //_lgfx.pushImageDMA(area->x1, area->y1, w, h, (lgfx::swap565_t*)color_p);
#endif

    // DMA running — lv_flush_wait_cb will take care of the closing of the bus, or call lgfx_check_flush() regularly
};

// Called by LVGL when it doesnt have a free display buffer and needs to wait until one is _flush_ready()
static void lv_disp_flush_wait_cb(lv_display_t* disp)
{
    // Wait for the DMA transfer to complete
    while(_lgfx.dmaBusy())
    {
        yield();
    };

    //done, close the bus
    _lgfx.endWrite();

    /* "The callback need not call lv_display_flush_ready() since the caller takes care 
    of that (clearing the display's flushing flag) when your callback returns."
    */
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
#endif // SOOGH_TOUCH

#endif // SOOGH_USE_LGFX

#ifdef SOOGH_USE_EPNL
// ── Flush callback ────────────────────────────────────────────────────────────
// DIRECT render mode: LVGL renders directly into the panel's PSRAM framebuffer.
// px_map IS the framebuffer pointer. Tell the panel to switch to it, done.
// For tear-free output, call lv_display_flush_ready() from the VSync callback
// instead of here, and register lcd->attachRefreshFinishCallback(vsync_cb, lv_disp).
static void lv_disp_flush_cb(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
    esp_panel::drivers::LCD *lcd = static_cast<esp_panel::drivers::LCD *>(lv_display_get_user_data(disp));
    lcd->drawBitmap(area->x1, area->y1, area->x2 + 1, area->y2 + 1, px_map);
    lv_display_flush_ready(disp);
};

// ── Optional: VSync-triggered flush ready (tear-free) ─────────────────────────
// Uncomment lcd->attachRefreshFinishCallback() in lvgl_port_init() to activate.
// Also remove the lv_display_flush_ready() call from flush_cb above when using this.
static bool lv_vsync_cb(void *user_data)
{
    lv_display_flush_ready((lv_display_t *)user_data);
    return false;  // return true to yield to higher-priority task
};

#ifdef SOOGH_TOUCH
static void lv_touchpad_cb(lv_indev_t* indev, lv_indev_data_t* data)
{
    esp_panel::drivers::TouchPoint point[1];
    int num = _epnl_board->getTouch()->readPoints(point, 1, 0);
    if(num > 0)
    {
        data->state = LV_INDEV_STATE_PRESSED;
        data->point.x = point[0].x;
        data->point.y = point[0].y;
    }
    else
    {
        data->state = LV_INDEV_STATE_RELEASED;
    };
};
#endif // SOOGH_TOUCH

#endif // SOOGH_USE_EPNL

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
