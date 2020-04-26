/*
 * Copyright 2015 Realtek Semiconductor Corp.
 */
#ifndef _TVINPUTHAL_H_
#define _TVINPUTHAL_H_

#include <hardware/tv_input.h>

enum {
    TV_INPUT_HW_HDMI,
//    TV_INPUT_HW_TUNER,
    TV_INPUT_HW_MAX_NUM,
};

#define TV_INPUT_ID(type, dev, stream)	(type << 24 | dev << 16 | stream)
#define TV_INPUT_TYPE(id)		((id >> 24) & 0xFF)
#define TV_INPUT_DEVICE(id)		((id >> 16) & 0xFF)
#define TV_INPUT_STREAM(id)     ((id >>  0) & 0xFFFF)

#endif //_TVINPUTHAL_H_
