#ifndef INFO_H
#define INFO_H

#include "libcgi.h"

enum {
	INFO_DEVICE_NAME 	= 0x01,
	INFO_SOFTWARE		= 0x01<<1,
	INFO_HARDWARE		= 0x01<<2,
};

static const PARAMETER_TABLE tbl_InfoSet[] = {
	INSERT_PARAM("software_info", INFO_SOFTWARE),
	INSERT_PARAM("hardware_info", INFO_HARDWARE),
	INSERT_PARAM("device_name", INFO_DEVICE_NAME)
};


int handleQueryInfo(void);

#endif // INFO_H