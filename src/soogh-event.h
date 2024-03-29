#ifndef __EVENT_H
#define __EVENT_H

#include <stdint.h>
#include <tools-keys.h>


typedef enum : uint32_t
{
	KEY_NONE = 0x00,
    KEY_RELEASED = KEYTOOL_RELEASED,

    // key defs
	KEY_A = 0x01,
	KEY_B = 0x02,
	KEY_C = 0x04,

	// combinations
	KEY_AB = KEY_A | KEY_B,
	KEY_AC = KEY_A | KEY_C,
	KEY_BC = KEY_B | KEY_C,
	KEY_ABC = KEY_A | KEY_B | KEY_C,

    // single key modes
    KEY_A_SHORT = KEY_A | KEYTOOL_SHORT,
    KEY_B_SHORT = KEY_B | KEYTOOL_SHORT,
    KEY_C_SHORT = KEY_C | KEYTOOL_SHORT,
	KEY_A_PRESSED = KEY_A | KEYTOOL_SHORT | KEYTOOL_RELEASED,
	KEY_B_PRESSED = KEY_B | KEYTOOL_SHORT | KEYTOOL_RELEASED,
	KEY_C_PRESSED = KEY_C | KEYTOOL_SHORT | KEYTOOL_RELEASED,
    KEY_A_LONG = KEY_A | KEYTOOL_LONG,
    KEY_B_LONG = KEY_B | KEYTOOL_LONG,
    KEY_C_LONG = KEY_C | KEYTOOL_LONG,
    KEY_A_LONG_REPEAT = KEY_A | KEYTOOL_LONG_REPEAT,
    KEY_B_LONG_REPEAT = KEY_B | KEYTOOL_LONG_REPEAT,
    KEY_C_LONG_REPEAT = KEY_C | KEYTOOL_LONG_REPEAT,
    KEY_A_LONG_RELEASED = KEY_A | KEYTOOL_LONG_RELEASED,
    KEY_B_LONG_RELEASED = KEY_B | KEYTOOL_LONG_RELEASED,
    KEY_C_LONG_RELEASED = KEY_C | KEYTOOL_LONG_RELEASED,    

    // combinations of 2
	KEY_AB_SHORT = KEY_AB | KEYTOOL_SHORT,
	KEY_AC_SHORT = KEY_AC | KEYTOOL_SHORT,
	KEY_BC_SHORT = KEY_BC | KEYTOOL_SHORT,
	KEY_AB_PRESSED = KEY_AB | KEYTOOL_SHORT | KEYTOOL_RELEASED,
	KEY_AC_PRESSED = KEY_AC | KEYTOOL_SHORT | KEYTOOL_RELEASED,
	KEY_BC_PRESSED = KEY_BC | KEYTOOL_SHORT | KEYTOOL_RELEASED,
	KEY_AB_LONG = KEY_AB | KEYTOOL_LONG,
	KEY_AC_LONG = KEY_AC | KEYTOOL_LONG,
	KEY_BC_LONG = KEY_BC | KEYTOOL_LONG,
	KEY_AB_LONG_REPEAT = KEY_AB | KEYTOOL_LONG_REPEAT,
	KEY_AC_LONG_REPEAT = KEY_AC | KEYTOOL_LONG_REPEAT,
	KEY_BC_LONG_REPEAT = KEY_BC | KEYTOOL_LONG_REPEAT,

    // combinations of 3
	KEY_ABC_SHORT = KEY_ABC | KEYTOOL_SHORT,
	KEY_ABC_PRESSED = KEY_ABC | KEYTOOL_SHORT | KEYTOOL_RELEASED,
	KEY_ABC_LONG = KEY_ABC | KEYTOOL_LONG,
	KEY_ABC_LONG_REPEAT = KEY_ABC | KEYTOOL_LONG_REPEAT,
} soogh_event_t;

const char* soogh_event_name(soogh_event_t e);

#endif // __EVENT_H
