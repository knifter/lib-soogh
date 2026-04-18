#include "soogh-lgfx.h"

#ifdef SOOGH_USE_LGFX
#include <LovyanGFX.hpp>

#include "soogh-lgfx_waveshare_LCD4.h"
#include "soogh-lgfx_wt32sc01.h"

#include <lvgl.h>
#include "esp_heap_caps.h"

LGFX _lgfx;

void lgfx_init()
{
    _lgfx.init();
    _lgfx.setRotation(SOOGH_DISP_ROTATE);
    _lgfx.initDMA();  
};

#endif // SOOGH_USE_LGFX
