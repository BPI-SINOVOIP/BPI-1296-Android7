/*
 * clk-mmio-gate.h
 *
 * Copyright (C) 2017 Realtek Semiconductor Corporation
 * Copyright (C) 2017 Cheng-Yu Lee <cylee12@realtek.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __CLK_REALTEK_CLK_MMIO_GATE_H
#define __CLK_REALTEK_CLK_MMIO_GATE_H

#include "common.h"

struct clk_mmio_gate {
	struct clk_reg base;
	int gate_offset;
	u8 bit_idx;
	spinlock_t *lock;
};

#define to_clk_mmio_gate(_hw) \
	container_of(to_clk_reg(_hw), struct clk_mmio_gate, base)
#define __clk_mmio_gate_hw(_ptr) __clk_reg_hw(&(_ptr)->base)

struct clk_ops;
extern const struct clk_ops clk_mmio_gate_ops;

#endif
