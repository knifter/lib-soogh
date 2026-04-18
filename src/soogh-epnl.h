#ifndef __SOOGH_EPNL_H
#define __SOOGH_EPNL_H

#include "soogh-conf.h"

#ifdef SOOGH_USE_EPNL

#include <esp_display_panel.hpp>

extern esp_panel::board::Board *_epnl_board;

void epnl_init();

#endif // SOOGH_USE_EPNL
#endif // __SOOGH_EPNL_H
