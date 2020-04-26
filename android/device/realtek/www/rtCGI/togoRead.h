#ifndef _TOGO_READ_H_
#define _TOGO_READ_H_

#include "fcgiapp.h"
#include "libcgi.h"

enum {
	TOGO_TYPE_CAMERA = 0,
	TOGO_TYPE_HDMI_RX,
	TOGO_TYPE_FILE,
	TOGO_TYPE_DTV,
	TOGO_TYPE_MAX
};

enum {
	CMD_START = 0,
	CMD_STOP,
};



#endif	// #define _TOGO_READ_H_