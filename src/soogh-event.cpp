#include "soogh-event.h"

#include <stdlib.h>
#include <string.h>

#include "tools-keys.h"

const char* soogh_event_name(soogh_event_t e)
{
	if(e == KEY_NONE)
		return "KEY_NONE";
	// if(e == KEY_RELEASED)
	// 	return "KEY_RELEASED";

    static char buf[sizeof("KEY_ABC_SHORT_DOUBLE_LONG_LONG_REPEAT_RELEASED")+1];
	strcpy(buf, "KEY");                                        // 4
	if(e & KEY_A)               strcat(buf, "_A");               // 1
	if(e & KEY_B)               strcat(buf, "_B");               // 1
	if(e & KEY_C)               strcat(buf, "_C");               // 1
    // if(e & KEYTOOL_PRESSED)     strcat(buf, "_PRESSED");        // 7
	if(e & KEYTOOL_SHORT)       strcat(buf, "_SHORT");          // 6
#ifndef KEYTOOL_NO_DOUBLE
	if(e & KEYTOOL_DOUBLE)     strcat(buf, "_DOUBLE");        // 6
#endif
	if(e & KEYTOOL_LONG)        strcat(buf, "_LONG");           // 5
	if(e & KEYTOOL_LONG_REPEAT) strcat(buf, "_LONG_REPEAT");    // 12
    if(e & KEYTOOL_RELEASED)    strcat(buf, "_RELEASED");       // 9
	return buf;
};
