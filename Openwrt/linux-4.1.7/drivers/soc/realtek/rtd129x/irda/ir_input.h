/*
 *  ir_input.h - RTK irda input device header file
 *
 * Copyright (C) 2017 Realtek Semiconductor Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef __IR_INPUT_H
#define __IR_INPUT_H

#include <linux/input.h>
#include <linux/slab.h>
#include <linux/types.h>

int venus_ir_input_init(void);
int venus_ir_input_exit(void);
void venus_ir_input_cleanup(void);
void venus_ir_input_report_key(uint32_t keycode);
void venus_ir_input_report_end(void);

#endif
