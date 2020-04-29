/*
 * clk-pll.h
 *
 * Copyright (C) 2017 Realtek Semiconductor Corporation
 * Copyright (C) 2017 Cheng-Yu Lee <cylee12@realtek.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __CLK_REALTEK_CLK_PLL_H
#define __CLK_REALTEK_CLK_PLL_H

#include "common.h"

struct clk_ops;

struct freq_table {
	unsigned int val;
	unsigned int mask;
	unsigned long rate;
};

#define FREQ_TABLE_END { .rate = 0 }
#define IS_FREQ_TABLE_END(_f) ((_f)->rate == 0)

struct div_table {
	unsigned long rate;
	unsigned int div;
	unsigned int val;
};

#define DIV_TABLE_END   { .rate = 0 }
#define IS_DIV_TABLE_END(_d) ((_d)->rate == 0)

struct clk_pll {
	struct clk_reg base;

	int pll_offset;
	const struct freq_table *freq_tbl;

	int ssc_offset;
	int pow_offset;
	unsigned int pow_on_val;
	unsigned int pow_off_val;
	int rdy_offset;
	spinlock_t *lock;

	unsigned int flags;
};

#define to_clk_pll(_hw) container_of(to_clk_reg(_hw), struct clk_pll, base)
#define __clk_pll_hw(_ptr)  __clk_reg_hw(&(_ptr)->base)

#define CLK_PLL_PREPARED  BIT(1)

extern const struct clk_ops clk_pll_ops;

struct clk_pll_scpu {
	struct clk_pll clkp;
	int div_offset;
	int div_shift;
	int div_width;
	const struct div_table *div_tbl;
	spinlock_t *lock;
};

#define to_clk_pll_scpu(_hw) \
	container_of(to_clk_pll(_hw), struct clk_pll_scpu, clkp)
#define __clk_pll_scpu_hw(_ptr) __clk_pll_hw(&(_ptr)->clkp)

extern const struct clk_ops clk_pll_scpu_ops;

#endif /* __CLK_REALTEK_CLK_PLL_H */
