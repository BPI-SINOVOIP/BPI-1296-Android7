#ifndef REMOTE_H
#define REMOTE_H

#include "libcgi.h"

#define DEV_REMOTE	"/dev/input/event0"

// keycode please refer to linux_kernel/inlcude/uapi/linux/input.h
// And it must be the same as android/device/realtek/phoenix/venus_IR_input.kl
enum {
    // System command
    KEYCODE_POWER       = 116,
    
	// Navigation control
	KEYCODE_DPAD_UP 	= 103,
	KEYCODE_DPAD_DOWN 	= 108,
	KEYCODE_DPAD_LEFT 	= 105,
	KEYCODE_DPAD_RIGHT 	= 106,
	KEYCODE_OK          = 352,  // 0x160
	
	KEYCODE_HOME 		= 102,  // 0x66
	KEYCODE_MENU 		= 357,  // 0x165
	KEYCODE_BACK		= 158,  // 0x9E
	
	// Volume control
	KEYCODE_MUTE		= 113,
	KEYCODE_VOLUME_UP	= 115,
	KEYCODE_VOLUME_DOWN = 114,
	
	// Playback control
	KEYCODE_PLAY		= 207,  // 0xCF
	KEYCODE_PAUSE       = 207,  // 0xCF
	KEYCODE_STOP		= 128,  // 0x80
	KEYCODE_FF			= 208,  // 0xD0
	KEYCODE_FR			= 168,  // 0xA8
	KEYCODE_NEXT		= 407,  // 0x197
	KEYCODE_PREV		= 412,  // 0x19C
    KEYCODE_REPEAT      = 232,  // 0xE8

    KEYCODE_SOURCE      = 353,  // 0x161
    KEYCODE_PIP         = 154,  // 0x9A
    KEYCODE_TV_SYTEM    = 377,  // 0x179
    KEYCODE_INFO        = 358,  // 0x166
    
    KEYCODE_SEARCH      = 372,  // 0x174
    KEYCODE_ZOOM        = 372,  // 0x174
};

static const PARAMETER_TABLE tbl_CommandSet[] = {
    INSERT_PARAM("POWER", KEYCODE_POWER),
        
	INSERT_PARAM("UP", KEYCODE_DPAD_UP),
	INSERT_PARAM("DOWN", KEYCODE_DPAD_DOWN),
	INSERT_PARAM("LEFT", KEYCODE_DPAD_LEFT),
	INSERT_PARAM("RIGHT", KEYCODE_DPAD_RIGHT),
	INSERT_PARAM("OK", KEYCODE_OK),
	
	INSERT_PARAM("HOME", KEYCODE_HOME),
	INSERT_PARAM("MENU", KEYCODE_MENU),
	INSERT_PARAM("BACK", KEYCODE_BACK),
	
	INSERT_PARAM("VOL_MUTE", KEYCODE_MUTE),
	INSERT_PARAM("VOL_UP", KEYCODE_VOLUME_UP),
	INSERT_PARAM("VOL_DOWN", KEYCODE_VOLUME_DOWN),
	
	INSERT_PARAM("PLAY", KEYCODE_PLAY),
	INSERT_PARAM("STOP", KEYCODE_STOP),
	INSERT_PARAM("FF", KEYCODE_FF),
	INSERT_PARAM("FR", KEYCODE_FR),
	INSERT_PARAM("NEXT", KEYCODE_NEXT),
	INSERT_PARAM("PREV", KEYCODE_PREV),
	INSERT_PARAM("REPEAT", KEYCODE_REPEAT),

    INSERT_PARAM("SOURCE", KEYCODE_SOURCE),
    INSERT_PARAM("PIP", KEYCODE_PIP),
    INSERT_PARAM("TV_SYS", KEYCODE_TV_SYTEM),
    INSERT_PARAM("INFO", KEYCODE_INFO),
    
    INSERT_PARAM("SEARCH", KEYCODE_SEARCH),
    INSERT_PARAM("ZOOM", KEYCODE_ZOOM),
};


int handleRemoteKey(int fdRemote);

#endif // REMOTE_H
