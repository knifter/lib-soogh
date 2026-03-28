#include "soogh-debug.h"

#include <array>

void debug_event_all(lv_event_t *e)
{
	char info[32];
	info[0] = '\0';

	// print ref. obj ptr
    lv_obj_t * obj = (lv_obj_t*) lv_event_get_target(e);
	if(obj)
		snprintf(info, 32, "%p", obj);

	switch(lv_event_get_code(e))
	{
	    /** Input device events = 1*/
		case LV_EVENT_PRESSED: 				DBG("%s: PRESSED", info); return;
		case LV_EVENT_PRESSING: 			DBG("%s: PRESSING", info); return;
		case LV_EVENT_PRESS_LOST: 			DBG("%s: PRESS_LOST", info); return;
		case LV_EVENT_SHORT_CLICKED:		DBG("%s: SHORT_CLICKED", info); return;
		case LV_EVENT_LONG_PRESSED: 		DBG("%s: LONG_PRESSED", info); return;
		case LV_EVENT_LONG_PRESSED_REPEAT: 	DBG("%s: LONG_PRESSED_REPEAT", info); return;
		case LV_EVENT_CLICKED: 				DBG("%s: CLICKED", info); return;
		case LV_EVENT_RELEASED: 			DBG("%s: RELEASED", info); return;
		case LV_EVENT_SCROLL_BEGIN: 		DBG("%s: SCROLL_BEGIN", info); return;
		case LV_EVENT_SCROLL_THROW_BEGIN:   DBG("%s: SCROLL_THROW_BEGIN", info); return;
		case LV_EVENT_SCROLL_END: 			DBG("%s: SCROLL_END", info); return;
		case LV_EVENT_SCROLL: 				DBG("%s: SCROLL_SCROLL", info); return;
		case LV_EVENT_GESTURE: 				DBG("%s: GESTURE", info); return;
		case LV_EVENT_KEY: 					DBG("%s: KEY", info); return;
		case LV_EVENT_ROTARY:				DBG("%s: ROTARY", info); return;
		case LV_EVENT_FOCUSED: 				DBG("%s: FOCUSED", info); return;
		case LV_EVENT_DEFOCUSED: 			DBG("%s: DEFOCUSED", info); return;
		case LV_EVENT_LEAVE: 				DBG("%s: LEAVE", info); return;
		case LV_EVENT_HIT_TEST: 			DBG("%s: HIT_TEST", info); return;
		case LV_EVENT_INDEV_RESET: 			DBG("%s: INDEV_RESET", info); return;
		case LV_EVENT_HOVER_OVER: 			DBG("%s: HOVER_OVER", info); return;
		case LV_EVENT_HOVER_LEAVE: 			DBG("%s: HOVER_LEAVE", info); return;

		/** Drawing events */
		case LV_EVENT_COVER_CHECK: 			DBG("%s: COVER_CHECK", info); return;
		case LV_EVENT_REFR_EXT_DRAW_SIZE: 	DBG("%s: EXT_DRAW_SIZE", info); return;
		case LV_EVENT_DRAW_MAIN_BEGIN: 		DBG("%s: MAIN_BEGIN", info); return;
		case LV_EVENT_DRAW_MAIN: 			DBG("%s: MAIN", info); return;
		case LV_EVENT_DRAW_MAIN_END: 		DBG("%s: MAIN_END", info); return;
		case LV_EVENT_DRAW_POST_BEGIN: 		DBG("%s: POST_BEGIN", info); return;
		case LV_EVENT_DRAW_POST: 			DBG("%s: POST", info); return;
		case LV_EVENT_DRAW_POST_END: 		DBG("%s: POST_END", info); return;
		case LV_EVENT_DRAW_TASK_ADDED:		DBG("%s: TASK_ADDED", info); return;

	    /** Special events */
		case LV_EVENT_VALUE_CHANGED: 		DBG("%s: VALUECHANGED", info); return;
		case LV_EVENT_INSERT: 				DBG("%s: INSERT", info); return;
    	case LV_EVENT_REFRESH: 				DBG("%s: REFRES", info); return;
    	case LV_EVENT_READY: 				DBG("%s: READY", info); return;
    	case LV_EVENT_CANCEL: 				DBG("%s: CANCEL", info); return;

    	// Other events
		case LV_EVENT_CREATE: 				DBG("%s: CREATE", info); return;
    	case LV_EVENT_DELETE: 				DBG("%s: DELETE", info); return;
		case LV_EVENT_CHILD_CHANGED:       	DBG("%s: CHILD_CHANGED", info); return;
		case LV_EVENT_CHILD_CREATED:       	DBG("%s: CHILD_CREATED", info); return;
		case LV_EVENT_CHILD_DELETED:       	DBG("%s: CHILD_DETELED", info); return;
		case LV_EVENT_SCREEN_UNLOAD_START: 	DBG("%s: SCREEN_UNLOAD_START", info); return;
		case LV_EVENT_SCREEN_LOAD_START:   	DBG("%s: LOAD_START", info); return;
		case LV_EVENT_SCREEN_LOADED:       	DBG("%s: LOADED", info); return;
		case LV_EVENT_SCREEN_UNLOADED:     	DBG("%s: UNLOADED", info); return;
		case LV_EVENT_SIZE_CHANGED:        	DBG("%s: SIZE_CHANGED", info); return;
		case LV_EVENT_STYLE_CHANGED:       	DBG("%s: STYLE_CHANGED", info); return;
		case LV_EVENT_LAYOUT_CHANGED:      	DBG("%s: LAYOUT_CHANGED", info); return;
		case LV_EVENT_GET_SELF_SIZE:       	DBG("%s: GET_SELF_SIZE", info); return;

		case LV_EVENT_INVALIDATE_AREA:		DBG("%s: INVALIDATE_AREA", info); return;
		case LV_EVENT_RESOLUTION_CHANGED:	DBG("%s: RESOLUTION_CHANGED", info); return;
		case LV_EVENT_COLOR_FORMAT_CHANGED:	DBG("%s: COLOR_FORMAT_CHANGED", info); return;
		case LV_EVENT_REFR_REQUEST:			DBG("%s: REFR_REQUEST", info); return;
		case LV_EVENT_REFR_START:			DBG("%s: REFR_START", info); return;
		case LV_EVENT_REFR_READY:			DBG("%s: REFR_READY", info); return;
		case LV_EVENT_RENDER_START:			DBG("%s: RENDER_START", info); return;
		case LV_EVENT_RENDER_READY:			DBG("%s: RENDER_READY", info); return;
		case LV_EVENT_FLUSH_START:			DBG("%s: FLUSH_START", info); return;
		case LV_EVENT_FLUSH_FINISH:			DBG("%s: FLUSH_FINISH", info); return;
		case LV_EVENT_FLUSH_WAIT_START:		DBG("%s: FLUSH_WAIT_START", info); return;
		case LV_EVENT_FLUSH_WAIT_FINISH:	DBG("%s: FLUSH_WAIT_FINISH", info); return;

		case LV_EVENT_VSYNC:				DBG("%s: SYNC", info); return;

	    // LV_EVENT_PREPROCESS = 0x8000,   /** This is a flag that can be set with an event so it's processed
        //                               before the class default event processing */

		default: DBG("%s: e.code = %d", info, lv_event_get_code(e)); return;
	};
};

void debug_event_input(lv_event_t *e)
{
	char info[32];
	info[0] = '\0';

	// print ref. obj ptr
    lv_obj_t * obj = (lv_obj_t*) lv_event_get_target(e);
	if(obj)
		snprintf(info, 32, "%p", obj);

	lv_event_code_t code = lv_event_get_code(e);
	switch(code)
	{
	    /** Input device events*/
		case LV_EVENT_PRESSED: 				DBG("%s: PRESSED", info); return;
		case LV_EVENT_PRESSING: 			DBG("%s: PRESSING", info); return;
		case LV_EVENT_PRESS_LOST: 			DBG("%s: PRESS_LOST", info); return;
		case LV_EVENT_SHORT_CLICKED:		DBG("%s: SHORT_CLICKED", info); return;
		case LV_EVENT_LONG_PRESSED: 		DBG("%s: LONG_PRESSED", info); return;
		case LV_EVENT_LONG_PRESSED_REPEAT: 	DBG("%s: LONG_PRESSED_REPEAT", info); return;
		case LV_EVENT_CLICKED: 				DBG("%s: CLICKED", info); return;
		case LV_EVENT_RELEASED: 			DBG("%s: RELEASED", info); return;
		case LV_EVENT_SCROLL_BEGIN: 		DBG("%s: SCROLL_BEGIN", info); return;
		case LV_EVENT_SCROLL_THROW_BEGIN:   DBG("%s: SCROLL_THROW_BEGIN", info); return;
		case LV_EVENT_SCROLL_END: 			DBG("%s: SCROLL_END", info); return;
		case LV_EVENT_SCROLL: 				DBG("%s: SCROLL_SCROLL", info); return;
		case LV_EVENT_GESTURE: 				DBG("%s: GESTURE", info); return;
		case LV_EVENT_KEY: 					DBG("%s: KEY", info); return;
		case LV_EVENT_ROTARY:				DBG("%s: ROTARY", info); return;
		case LV_EVENT_FOCUSED: 				DBG("%s: FOCUSED", info); return;
		case LV_EVENT_DEFOCUSED: 			DBG("%s: DEFOCUSED", info); return;
		case LV_EVENT_LEAVE: 				DBG("%s: LEAVE", info); return;
		case LV_EVENT_HIT_TEST: 			DBG("%s: HIT_TEST", info); return;
		case LV_EVENT_INDEV_RESET: 			DBG("%s: INDEV_RESET", info); return;
		case LV_EVENT_HOVER_OVER: 			DBG("%s: HOVER_OVER", info); return;
		case LV_EVENT_HOVER_LEAVE: 			DBG("%s: HOVER_LEAVE", info); return;

	    /** Special events */
		case LV_EVENT_VALUE_CHANGED: 		DBG("%s: VALUECHANGED", info); return;
		case LV_EVENT_INSERT: 				DBG("%s: INSERT", info); return;
    	case LV_EVENT_REFRESH: 				DBG("%s: REFRES", info); return;
    	case LV_EVENT_READY: 				DBG("%s: READY", info); return;
    	case LV_EVENT_CANCEL: 				DBG("%s: CANCEL", info); return;

    	// Other events
		case LV_EVENT_CREATE: 				DBG("%s: CREATE", info); return;
    	case LV_EVENT_DELETE: 				DBG("%s: DELETE", info); return;

		default: return;
	};
};

void debug_event_drawing(lv_event_t * e)
{
	char info[32];
	info[0] = '\0';

	// print ref. obj ptr
    lv_obj_t * obj = (lv_obj_t*) lv_event_get_target(e);
	if(obj)
		snprintf(info, 32, "%p", obj);

	lv_event_code_t code = lv_event_get_code(e);
	switch(code)
	{
		/** Drawing events */
		case LV_EVENT_COVER_CHECK: 			DBG("%s: COVER_CHECK", info); return;
		case LV_EVENT_REFR_EXT_DRAW_SIZE: 	DBG("%s: EXT_DRAW_SIZE", info); return;
		case LV_EVENT_DRAW_MAIN_BEGIN: 		DBG("%s: MAIN_BEGIN", info); return;
		case LV_EVENT_DRAW_MAIN: 			DBG("%s: MAIN", info); return;
		case LV_EVENT_DRAW_MAIN_END: 		DBG("%s: MAIN_END", info); return;
		case LV_EVENT_DRAW_POST_BEGIN: 		DBG("%s: POST_BEGIN", info); return;
		case LV_EVENT_DRAW_POST: 			DBG("%s: POST", info); return;
		case LV_EVENT_DRAW_POST_END: 		DBG("%s: POST_END", info); return;
		case LV_EVENT_DRAW_TASK_ADDED:		DBG("%s: TASK_ADDED", info); return;

		default: return;
	};
};

void debug_event_special(lv_event_t * e)
{
	char info[32];
	info[0] = '\0';

	// print ref. obj ptr
    lv_obj_t * obj = (lv_obj_t*) lv_event_get_target(e);
	if(obj)
		snprintf(info, 32, "%p", obj);

	lv_event_code_t code = lv_event_get_code(e);
	switch(code)
	{
	    /** Special events */
		case LV_EVENT_VALUE_CHANGED: 		DBG("%s: VALUECHANGED", info); return;
		case LV_EVENT_INSERT: 				DBG("%s: INSERT", info); return;
    	case LV_EVENT_REFRESH: 				DBG("%s: REFRES", info); return;
    	case LV_EVENT_READY: 				DBG("%s: READY", info); return;
    	case LV_EVENT_CANCEL: 				DBG("%s: CANCEL", info); return;

		default: return;
	};
};

void debug_event_other(lv_event_t * e)
{

	char info[32];
	info[0] = '\0';

	// print ref. obj ptr
    lv_obj_t * obj = (lv_obj_t*) lv_event_get_target(e);
	if(obj)
		snprintf(info, 32, "%p", obj);

	lv_event_code_t code = lv_event_get_code(e);
	switch(code)
	{
    	// Other events
		case LV_EVENT_CREATE: 				DBG("%s: CREATE", info); return;
    	case LV_EVENT_DELETE: 				DBG("%s: DELETE", info); return;
		case LV_EVENT_CHILD_CHANGED:       	DBG("%s: CHILD_CHANGED", info); return;
		case LV_EVENT_CHILD_CREATED:       	DBG("%s: CHILD_CREATED", info); return;
		case LV_EVENT_CHILD_DELETED:       	DBG("%s: CHILD_DETELED", info); return;
		case LV_EVENT_SCREEN_UNLOAD_START: 	DBG("%s: SCREEN_UNLOAD_START", info); return;
		case LV_EVENT_SCREEN_LOAD_START:   	DBG("%s: LOAD_START", info); return;
		case LV_EVENT_SCREEN_LOADED:       	DBG("%s: LOADED", info); return;
		case LV_EVENT_SCREEN_UNLOADED:     	DBG("%s: UNLOADED", info); return;
		case LV_EVENT_SIZE_CHANGED:        	DBG("%s: SIZE_CHANGED", info); return;
		case LV_EVENT_STYLE_CHANGED:       	DBG("%s: STYLE_CHANGED", info); return;
		case LV_EVENT_LAYOUT_CHANGED:      	DBG("%s: LAYOUT_CHANGED", info); return;
		case LV_EVENT_GET_SELF_SIZE:       	DBG("%s: GET_SELF_SIZE", info); return;

		case LV_EVENT_INVALIDATE_AREA:		DBG("%s: INVALIDATE_AREA", info); return;
		case LV_EVENT_RESOLUTION_CHANGED:	DBG("%s: RESOLUTION_CHANGED", info); return;
		case LV_EVENT_COLOR_FORMAT_CHANGED:	DBG("%s: COLOR_FORMAT_CHANGED", info); return;
		case LV_EVENT_REFR_REQUEST:			DBG("%s: REFR_REQUEST", info); return;
		case LV_EVENT_REFR_START:			DBG("%s: REFR_START", info); return;
		case LV_EVENT_REFR_READY:			DBG("%s: REFR_READY", info); return;
		case LV_EVENT_RENDER_START:			DBG("%s: RENDER_START", info); return;
		case LV_EVENT_RENDER_READY:			DBG("%s: RENDER_READY", info); return;
		case LV_EVENT_FLUSH_START:			DBG("%s: FLUSH_START", info); return;
		case LV_EVENT_FLUSH_FINISH:			DBG("%s: FLUSH_FINISH", info); return;
		case LV_EVENT_FLUSH_WAIT_START:		DBG("%s: FLUSH_WAIT_START", info); return;
		case LV_EVENT_FLUSH_WAIT_FINISH:	DBG("%s: FLUSH_WAIT_FINISH", info); return;

		case LV_EVENT_VSYNC:				DBG("%s: SYNC", info); return;

		default: return;
	};
};
