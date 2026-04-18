#ifndef __SOOGH_LGFX_H
#define __SOOGH_LGFX_H

#include "soogh-conf.h"

#ifdef SOOGH_USE_LGFX

#include <LovyanGFX.hpp>

#include "soogh-lgfx_waveshare_LCD4.h"
#include "soogh-lgfx_wt32sc01.h"

extern LGFX _lgfx;

void lgfx_init();

#endif // __SOOGH_LGFX_H

#endif // SOOGH_USE_LGFX
