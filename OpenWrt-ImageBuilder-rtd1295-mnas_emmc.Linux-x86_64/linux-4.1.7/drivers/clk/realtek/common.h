/*
 * common.h - Realtek Clock Common API
 *
 * Copyright (C) 2016-2017 Realtek Semiconductor Corporation
 * Copyright (C) 2016-2017 Cheng-Yu Lee <cylee12@realtek.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __CLK_REALTEK_COMMON_H
#define __CLK_REALTEK_COMMON_H

#include <linux/clk-provider.h>
#include <linux/bitops.h>
#include <linux/io.h>
#include <linux/regmap.h>

struct clk_hw;

struct clk_reg {
	struct clk_hw hw;
	void __iomem *reg;

	struct regmap *regmap;
	int offset;

	unsigned long flags;
};

#define to_clk_reg(_hw) container_of(_hw, struct clk_reg, hw)
#define __clk_reg_hw(_ptr)  ((_ptr)->hw)

#define CLK_OFFSET_INVALID  -1
#define CLK_OFFSET_IS_VALID(_off)    ((_off) != CLK_OFFSET_INVALID)

#define CLK_REG_XEN_DOM0       BIT(0)
#define CLK_REG_XEN_DOMU       BIT(1)
#define CLK_REG_XEN_COMMON     (XEN_CLK_DOM0 | XEN_CLK_DOMU)
#define CLK_REG_OUT_KERNEL     BIT(2)

#include <xen/xen.h>

static inline unsigned long clk_reg_get_flags(struct clk_reg *b)
{
	if (b->flags & CLK_REG_OUT_KERNEL)
		return CLK_IGNORE_UNUSED;

	if (!xen_domain())
		return 0;

	if ((xen_initial_domain() && (b->flags & CLK_REG_XEN_DOMU)))
		return CLK_IGNORE_UNUSED;

	if (!xen_initial_domain() && (b->flags & CLK_REG_XEN_DOM0))
		return CLK_IGNORE_UNUSED;

	return 0;
}

struct clk_reg_init_data {
	void __iomem *reg;
	struct regmap *regmap;
	int offset;
};

static inline void clk_reg_init(struct clk_reg *b, struct clk_reg_init_data *i)
{
	b->regmap = i->regmap;
	b->reg = i->reg;
	b->offset = i->offset;
}

static inline void clk_reg_write(struct clk_reg *b, unsigned int offset,
	unsigned int val)
{
	if (b->regmap)
		regmap_write(b->regmap, b->offset + offset, val);
	else if (b->reg)
		writel(val, b->reg + offset);
	else
		WARN_ON(1);
}

static inline unsigned int clk_reg_read(struct clk_reg *b, unsigned int offset)
{
	unsigned int val = 0;

	if (b->regmap)
		regmap_read(b->regmap, b->offset + offset, &val);
	else if (b->reg)
		val = readl(b->reg + offset);
	else
		WARN_ON(1);
	return val;
}

static inline void clk_reg_update(struct clk_reg *b, unsigned int offset,
	unsigned int mask, unsigned int val)
{
	if (b->regmap) {
		regmap_update_bits(b->regmap, b->offset + offset, mask, val);
	} else if (b->reg) {
		unsigned rval = readl(b->reg + offset);

		rval = (rval & ~mask) | (val & mask);
		writel(rval, b->reg + offset);
	} else {
		WARN_ON(1);
	}
}

struct clk_composite_init_data {
	int gate_offset;
	int gate_shift;
	int mux_offset;
	int mux_width;
	int mux_shift;
	const char **parent_names;
	int num_parents;
	unsigned long flags;
	const char *name;
	struct clk *clk;
};

#endif /* __CLK_REALTEK_COMMON_H */

