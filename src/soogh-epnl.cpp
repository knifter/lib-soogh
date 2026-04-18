
#include "soogh-epnl.h"
#include "soogh-conf.h"

#ifdef SOOGH_USE_EPNL

#include "tools-log.h"

#include <esp_display_panel.hpp>

#include "soogh-epnl_waveshare_lcd4.h"

// using namespace esp_panel::drivers;
// using namespace esp_panel::board;

esp_panel::board::Board *_epnl_board = nullptr;

// ---------------------------------------------------------------------------
void epnl_init()
{
    waveshare_lcd4_init();

	static esp_panel::board::Board *board;

	board = new esp_panel::board::Board();
	board->init();

	// Skip I2C re-init if touch / expander exist and use I2C bus (USE_TOUCH=0 / USE_EXPANDER=0 → nullptr).
// TODO: Is this still needed
	if(auto *touch = board->getTouch()) 
    {
		if(auto *bus = static_cast<esp_panel::drivers::BusI2C *>(touch->getBus()))
        {
			bus->configI2C_HostSkipInit();
        };
	};
	if(auto *expander = board->getIO_Expander())
    {
		expander->skipInitHost();
    };

	board->begin();
    _epnl_board = board;
    
	waveshare_lcd4_set_backlight(80);
};

#endif // SOOGH_USE_EPNL
