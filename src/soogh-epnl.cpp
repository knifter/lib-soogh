
#include "soogh-epnl.h"
#include "soogh-conf.h"

#ifdef SOOGH_USE_EPNL

#include "tools-log.h"

#include <esp_display_panel.hpp>

#include "soogh-epnl_waveshare_lcd4.h"

esp_panel::board::Board *_epnl_board = nullptr;

// ---------------------------------------------------------------------------
void epnl_init()
{
#ifdef SOOGH_DEV_WAVESHARE_LCD4
    waveshare_lcd4_init();
#endif

	static esp_panel::board::Board *board = new esp_panel::board::Board();
	if(!board->init())
	{
		ERROR("board->init() failed!");
		while(1);
	};

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

    if(auto *lcd = board->getLCD())
        lcd->configFrameBufferNumber(2);

	if(!board->begin())
	{
		ERROR("board->begin() failed!");
		while(1);
	};

    _epnl_board = board;
};

#endif // SOOGH_USE_EPNL
