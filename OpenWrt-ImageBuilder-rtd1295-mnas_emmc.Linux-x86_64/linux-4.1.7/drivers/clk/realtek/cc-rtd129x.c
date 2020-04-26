/*
 * cc-rtd129x.c - RTD129x clock controller
 *
 * Copyright (C) 2017 Realtek Semiconductor Corporation
 * Copyright (C) 2017 Cheng-Yu Lee <cylee12@realtek.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#define pr_fmt(fmt) "rtk-clk: " fmt

#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/clk.h>
#include <linux/clkdev.h>
#include <linux/clk-provider.h>
#include <linux/bitops.h>
#include <linux/pm.h>
#include <linux/suspend.h>
#include <soc/realtek/rtk_mmio.h>
#include "common.h"
#include "clk-pll.h"
#include "clk-mmio-gate.h"
#include "clk-mmio-mux.h"
#include <dt-bindings/clock/rtk,cc-rtd129x.h>

#define CC_SUSPEND_RATE     0x1
#define CC_SUSPEND_PARENT   0x2
#define CC_SUSPEND_ENABLE   0x4

static DEFINE_SPINLOCK(clk_div_lock);

#define _D(_rate, _div, _val) \
{ \
	.rate = _rate, \
	.div = _div, \
	.val = _val, \
}

#define _NF(_rate, _n, _f) \
{ \
	.rate = _rate, \
	.mask = 0x7FFFF, \
	.val = ((_n) << 11) | (_f), \
}

#define _MNO(_rate, _m, _n, _o) \
{ \
	.rate = _rate, \
	.mask = 0x63FF0, \
	.val = ((_m) << 4) | ((_n) << 12) | ((_o) << 17), \
}

struct clk_pm_data {
	unsigned int flags;
	struct clk *parent;
	unsigned long rate;
	int enable_cnt;
};

struct cc_desc {
	void *reg;
	struct regmap *regmap;
	struct clk_onecell_data data;
	struct clk_pm_data *pm_data;
	struct clk_reg_init_data init_data;
};

static const struct div_table scpu_div_tbl[] = {
	_D(1000000000, 1, 1),
	_D(500000000, 2, 2),
	_D(250000000, 4, 3),
	/* mapped for the case when reg val is 0 */
	_D(1000000000, 1, 0),
	DIV_TABLE_END
};

static const struct freq_table scpu_tbl[] = {
	_NF(1000000000, 34,   75),
	_NF(1100000000, 37, 1517),
	_NF(1200000000, 41,  910),
	_NF(1300000000, 45,  303),
	_NF(1400000000, 48, 1745),
	_NF(1500000000, 52, 1137),
	_NF(1600000000, 56,  531),
	_NF(1800000000, 63, 1365),
	/* special cases */
	_NF(1200000000, 41, 1024),
	_NF(1300000000, 45, 1024),
	_NF(1503000000, 48, 1744),
	FREQ_TABLE_END
};

static struct clk_pll_scpu pll_scpu = {
	.div_offset = 0x30,
	.div_shift  = 7,
	.div_width  = 2,
	.div_tbl = scpu_div_tbl,
	.lock = &clk_div_lock,
	.clkp = {
		.ssc_offset = 0x500,
		.pll_offset = 0x504,
		.rdy_offset = 0x51c,
		.pow_offset = CLK_OFFSET_INVALID,
		.freq_tbl = scpu_tbl,
		.base.hw.init = &(struct clk_init_data) {
			.name = "pll_scpu",
			.ops = &clk_pll_scpu_ops,
			.parent_names = (const char *[]){ "osc27M" },
			.num_parents = 1,
			.flags = CLK_IGNORE_UNUSED | CLK_GET_RATE_NOCACHE,
		},
	},
};

static const struct freq_table bus_tbl[] = {
	_NF(200000000,  4,  835),
	_NF(243000000,  6,    0),
	_NF(256000000,  6,  986),
	_NF(256000000,  6, 1024),
	_NF(257000000,  6, 1061),

	_NF(300000000,  8,  227),
	_NF(320000000,  8, 1744),
	_NF(340000000,  9, 1213),
	_NF(360000000, 10,  682),
	_NF(380000000, 11,  151),
	_NF(400000000, 11, 1668),
	_NF(420000000, 12, 1137),
	_NF(440000000, 13,  606),

	_NF(459000000, 14,    0),
	_NF(486000000, 15,    0),
	_NF(482000000, 14, 1744),
	_NF(513000000, 16,    0),
	_NF(540000000, 17,    0),
	FREQ_TABLE_END
};

static struct clk_pll pll_bus = {
	.ssc_offset = 0x520,
	.pll_offset = 0x524,
	.rdy_offset = CLK_OFFSET_INVALID,
	.pow_offset = CLK_OFFSET_INVALID,
	.freq_tbl = bus_tbl,
	.base.hw.init = &(struct clk_init_data) {
		.name = "pll_bus",
		.ops = &clk_pll_ops,
		.parent_names = (const char *[]){ "osc27M" },
		.num_parents = 1,
		.flags = CLK_IGNORE_UNUSED | CLK_GET_RATE_NOCACHE,
	},
};

static struct clk_fixed_factor pll_bus_div2 = {
	.div = 2,
	.mult = 1,
	.hw.init = &(struct clk_init_data) {
		.name = "pll_bus_div2",
		.ops = &clk_fixed_factor_ops,
		.parent_names = (const char *[]){ "pll_bus" },
		.num_parents = 1,
		.flags = 0,
	},
};

static struct clk_mmio_mux clk_sys = {
	.mux_offset = 0x30,
	.mask = BIT(1) - 1,
	.shift = 0,
	.lock = &clk_div_lock,
	.base.hw.init = &(struct clk_init_data) {
		.name = "clk_sys",
		.ops = &clk_mmio_mux_ops,
		.parent_names = (const char *[]){
			"pll_bus",
			"pll_bus_div2",
		},
		.num_parents = 2,
		.flags = 0,
	},
};

static struct clk_mmio_mux clk_sys_sb2 = {
	.mux_offset = 0x18,
	.mask = BIT(1) - 1,
	.shift = 0,
	.base.hw.init = &(struct clk_init_data) {
		.name = "clk_sys_sb2",
		.ops = &clk_mmio_mux_ops,
		.parent_names = (const char *[]){
			"clk_sys",
			"osc27M",
		},
		.num_parents = 2,
		.flags = 0,
	},
};

static int clk_sys_sb2_notifier(struct notifier_block *nb, unsigned long action,
	void *data)
{
	struct clk *pclk = NULL;
	struct clk *clk = clk_sys_sb2.base.hw.clk;
	struct clk_notifier_data *cd = data;

	switch (action) {
	case PRE_RATE_CHANGE:
		pclk = clk_get_parent_by_index(clk, 1);
		break;

	case POST_RATE_CHANGE:
	case ABORT_RATE_CHANGE:
		pclk = clk_get_parent_by_index(clk, 0);
		break;
	}

	if (pclk)
		clk_set_parent(clk, pclk);
	return NOTIFY_OK;
}

static struct notifier_block clk_sys_sb2_nb = {
	.notifier_call = clk_sys_sb2_notifier,
};

static struct clk_pll pll_bus_h = {
	.ssc_offset = 0x540,
	.pll_offset = 0x544,
	.rdy_offset = CLK_OFFSET_INVALID,
	.pow_offset = CLK_OFFSET_INVALID,
	.freq_tbl = bus_tbl,
	.base.hw.init = &(struct clk_init_data) {
		.name = "pll_bus_h",
		.ops = &clk_pll_ops,
		.parent_names = (const char *[]){ "osc27M" },
		.num_parents = 1,
		.flags = CLK_IGNORE_UNUSED | CLK_GET_RATE_NOCACHE,
	},
};

static struct clk_fixed_factor clk_sysh = {
	.div = 1,
	.mult = 1,
	.hw.init = &(struct clk_init_data) {
		.name = "clk_sysh",
		.ops = &clk_fixed_factor_ops,
		.parent_names = (const char *[]){ "pll_bus_h" },
		.num_parents = 1,
		.flags = 0,
	},
};

static const struct freq_table ddsx_tbl[] = {
	_NF(432000000, 13, 0),
	FREQ_TABLE_END
};

static struct clk_pll pll_ddsa = {
	.ssc_offset = 0x560,
	.pll_offset = 0x564,
	.rdy_offset = CLK_OFFSET_INVALID,
	.pow_offset = CLK_OFFSET_INVALID,
	.freq_tbl = ddsx_tbl,
	.base.hw.init = &(struct clk_init_data) {
		.name = "pll_ddsa",
		.ops = &clk_pll_ops,
		.parent_names = (const char *[]){ "osc27M" },
		.num_parents = 1,
		.flags = CLK_IGNORE_UNUSED | CLK_GET_RATE_NOCACHE,
	},
};

static struct clk_pll pll_ddsb = {
	.ssc_offset = 0x580,
	.pll_offset = 0x584,
	.rdy_offset = CLK_OFFSET_INVALID,
	.pow_offset = 0x178,
	.pow_on_val = 0x3,
	.pow_off_val = 0x4,
	.freq_tbl = ddsx_tbl,
	.base.hw.init = &(struct clk_init_data) {
		.name = "pll_ddsb",
		.ops = &clk_pll_ops,
		.parent_names = (const char *[]){ "osc27M" },
		.num_parents = 1,
		.flags = CLK_GET_RATE_NOCACHE,
	},
};

static const struct freq_table gpu_tbl[] = {
	_NF(300000000,  8,  227),
	_NF(320000000,  8, 1744),
	_NF(340000000,  9, 1213),
	_NF(360000000, 10,  682),
	_NF(380000000, 11,  151),
	_NF(400000000, 11, 1668),
	_NF(420000000, 12, 1137),
	_NF(440000000, 13,  606),
	_NF(460000000, 14,   75),
	_NF(480000000, 14, 1592),
	_NF(500000000, 15, 1061),
	_NF(520000000, 16,  530),
	_NF(540000000, 17,    0),
	_NF(560000000, 17, 1517),
	_NF(580000000, 18,  986),
	_NF(600000000, 19,  455),
	_NF(620000000, 19, 1972),
	_NF(640000000, 20, 1441),
	_NF(660000000, 21,  910),
	_NF(680000000, 22,  379),
	_NF(460000000, 13, 1365),
	FREQ_TABLE_END
};

static struct clk_pll pll_gpu = {
	.ssc_offset = 0x5a0,
	.pll_offset = 0x5a4,
	.rdy_offset = 0x5bc,
	.pow_offset = 0x1c4,
	.pow_on_val = 0x3,
	.pow_off_val = 0x4,
	.freq_tbl = gpu_tbl,
	.base.hw.init = &(struct clk_init_data) {
		.name = "pll_gpu",
		.ops = &clk_pll_ops,
		.parent_names = (const char *[]){ "osc27M" },
		.num_parents = 1,
		.flags = CLK_GET_RATE_NOCACHE,
	},
};

static struct clk_composite_init_data clk_gpu_init = {
	.mux_offset = CLK_OFFSET_INVALID,
	.gate_offset = 0x0c,
	.gate_shift = 11,
	.parent_names =  (const char *[]){ "pll_gpu" },
	.num_parents = 1,
	.name = "clk_gpu",
	.flags = CLK_SET_RATE_PARENT | CLK_IGNORE_UNUSED,
};

static struct freq_table ve_tbl[] = {
	_MNO(189000000,  5, 0, 0),
	_MNO(270000000,  8, 0, 0),
	_MNO(405000000, 13, 0, 0),
	_MNO(432000000, 14, 0, 0),
	_MNO(459000000, 15, 0, 0),
	_MNO(486000000, 16, 0, 0),
	_MNO(513000000, 17, 0, 0),
	_MNO(540000000, 18, 0, 0),
	_MNO(567000000, 19, 0, 0),
	_MNO(594000000, 20, 0, 0),
	_MNO(648000000, 22, 0, 0),
	_MNO(675000000, 23, 0, 0),
	_MNO(702000000, 24, 0, 0),
	_MNO(715000000, 51, 1, 0),
	FREQ_TABLE_END
};

static struct clk_pll pll_ve1 = {
	.ssc_offset = CLK_OFFSET_INVALID,
	.pll_offset = 0x114,
	.rdy_offset = CLK_OFFSET_INVALID,
	.pow_offset = 0x118,
	.pow_on_val = 0x3,
	.pow_off_val = 0x4,
	.freq_tbl = ve_tbl,
	.base.hw.init = &(struct clk_init_data) {
		.name = "pll_ve1",
		.ops = &clk_pll_ops,
		.parent_names = (const char *[]){ "osc27M" },
		.num_parents = 1,
		.flags = CLK_GET_RATE_NOCACHE,
	},
};

static struct clk_pll pll_ve2 = {
	.ssc_offset = CLK_OFFSET_INVALID,
	.pll_offset = 0x1d0,
	.rdy_offset = CLK_OFFSET_INVALID,
	.pow_offset = 0x1d4,
	.pow_on_val = 0x3,
	.pow_off_val = 0x4,
	.freq_tbl = ve_tbl,
	.base.hw.init = &(struct clk_init_data) {
		.name = "pll_ve2",
		.ops = &clk_pll_ops,
		.parent_names = (const char *[]){ "osc27M" },
		.num_parents = 1,
		.flags = CLK_GET_RATE_NOCACHE,
	},
};

static struct clk_composite_init_data clk_ve1_init = {
	.mux_offset = 0x4c,
	.mux_width = 2,
	.mux_shift = 0,
	.gate_offset = 0x0c,
	.gate_shift = 12,
	.parent_names =  (const char *[]){
		"clk_sysh",
		"pll_ve1",
		"pll_ve2",
		"pll_ve2",
	},
	.num_parents = 4,
	.name = "clk_ve1",
	.flags = CLK_IGNORE_UNUSED | CLK_SET_RATE_PARENT |
		CLK_SET_RATE_NO_REPARENT,
};

static struct clk_composite_init_data clk_ve2_init = {
	.mux_offset = 0x4c,
	.mux_width = 2,
	.mux_shift = 2,
	.gate_offset = 0x0c,
	.gate_shift = 13,
	.parent_names =  (const char *[]){
		"clk_sysh",
		"pll_ve1",
		"pll_ve2",
		"pll_ve2",
	},
	.num_parents = 4,
	.name = "clk_ve2",
	.flags = CLK_IGNORE_UNUSED | CLK_SET_RATE_PARENT |
		CLK_SET_RATE_NO_REPARENT,
};

static struct clk_composite_init_data clk_ve3_init = {
	.mux_offset = 0x4c,
	.mux_width = 2,
	.mux_shift = 4,
	.gate_offset = 0x0c,
	.gate_shift = 29,
	.parent_names =  (const char *[]){
		"clk_sysh",
		"pll_ve1",
		"pll_ve2",
		"pll_ve2",
	},
	.num_parents = 4,
	.name = "clk_ve3",
	.flags = CLK_IGNORE_UNUSED | CLK_SET_RATE_PARENT |
		CLK_SET_RATE_NO_REPARENT,
};

static struct clk_pll pll_vodma = {
	.ssc_offset = CLK_OFFSET_INVALID,
	.pll_offset = 0x114,
	.rdy_offset = CLK_OFFSET_INVALID,
	.pow_offset = CLK_OFFSET_INVALID,
	.freq_tbl = ve_tbl,
	.base.hw.init = &(struct clk_init_data) {
		.name = "pll_vodma",
		.ops = &clk_pll_ops,
		.parent_names = (const char *[]){ "osc27M" },
		.num_parents = 1,
		.flags = CLK_IGNORE_UNUSED | CLK_GET_RATE_NOCACHE,
	},
};

static const struct freq_table acpu_tbl[] = {
	_NF(270000000,  7,    0),
	_NF(300000000,  8,  227),
	_NF(350000000,  9, 1972),
	_NF(400000000, 11, 1668),
	_NF(450000000, 13, 1365),
	_NF(500000000, 15, 1061),
	_NF(550000000, 17,  758),
	_NF(600000000, 19,  455),
	FREQ_TABLE_END
};

static struct clk_pll pll_acpu = {
	.ssc_offset = CLK_OFFSET_INVALID,
	.pll_offset = 0x5c4,
	.rdy_offset = CLK_OFFSET_INVALID,
	.pow_offset = CLK_OFFSET_INVALID,
	.freq_tbl = acpu_tbl,
	.base.hw.init = &(struct clk_init_data) {
		.name = "pll_acpu",
		.ops = &clk_pll_ops,
		.parent_names = (const char *[]){ "osc27M" },
		.num_parents = 1,
		.flags = CLK_IGNORE_UNUSED | CLK_GET_RATE_NOCACHE,
	},
};

static __initdata struct clk_hw *clk_reg_list[] = {
	[CC_PLL_SCPU]   = &__clk_pll_scpu_hw(&pll_scpu),
	[CC_PLL_BUS]    = &__clk_pll_hw(&pll_bus),
	[CC_PLL_BUS_H]  = &__clk_pll_hw(&pll_bus_h),
	[CC_PLL_DDSA]   = &__clk_pll_hw(&pll_ddsa),
	[CC_PLL_DDSB]   = &__clk_pll_hw(&pll_ddsb),
	[CC_PLL_GPU]    = &__clk_pll_hw(&pll_gpu),
	[CC_PLL_VE1]    = &__clk_pll_hw(&pll_ve1),
	[CC_PLL_VE2]    = &__clk_pll_hw(&pll_ve2),
	[CC_PLL_VODMA]  = &__clk_pll_hw(&pll_vodma),
	[CC_PLL_ACPU]   = &__clk_pll_hw(&pll_acpu),

	[CC_CLK_SYS]     = &__clk_mmio_mux_hw(&clk_sys),
	[CC_CLK_SYS_SB2] = &__clk_mmio_mux_hw(&clk_sys_sb2),
};

static __initdata struct clk_hw *fixed_clks[] = {
	[CC_PLL_BUS_DIV2] = &pll_bus_div2.hw,
	[CC_CLK_SYSH] = &clk_sysh.hw,
};

static  __initdata struct clk_composite_init_data *composite_clks[] = {
	[CC_CLK_GPU] = &clk_gpu_init,
	[CC_CLK_VE1] = &clk_ve1_init,
	[CC_CLK_VE2] = &clk_ve2_init,
	[CC_CLK_VE3] = &clk_ve3_init,
};

static int init_composite_clk(struct device *dev,
	struct clk_composite_init_data *init)
{
	struct cc_desc *ccd = dev_get_drvdata(dev);
	struct clk_mmio_gate *gate = NULL;
	const struct clk_ops *gate_op = NULL;
	struct clk_mmio_mux *mux = NULL;
	const struct clk_ops *mux_op = NULL;
	struct clk *clk;

	pr_info("%s: %s\n", __func__, init->name);

	if (init->mux_offset != CLK_OFFSET_INVALID) {
		mux = kzalloc(sizeof(*mux), GFP_KERNEL);
		mux->mux_offset = init->mux_offset;
		mux->mask = BIT(init->mux_width) - 1;
		mux->shift = init->mux_shift;

		clk_reg_init(&mux->base, &ccd->init_data);

		mux_op = &clk_mmio_mux_ops;
	}

	if (init->gate_offset != CLK_OFFSET_INVALID) {
		gate = kzalloc(sizeof(*gate), GFP_KERNEL);
		gate->gate_offset = init->gate_offset;
		gate->bit_idx = init->gate_shift;

		clk_reg_init(&gate->base, &ccd->init_data);

		gate_op = &clk_mmio_gate_ops;
	}

	clk = clk_register_composite(NULL, init->name, init->parent_names,
		init->num_parents,
		mux ? &__clk_mmio_mux_hw(mux) : NULL, mux_op,
		NULL, NULL,
		gate ? &__clk_mmio_gate_hw(gate) : NULL, gate_op,
		init->flags);
	if (IS_ERR(clk)) {
		dev_err(dev, "Failed to register %s: %ld",
			init->name, PTR_ERR(clk));
		return PTR_ERR(clk);
	}

	init->clk = clk;
	clk_register_clkdev(clk, init->name, NULL);

	return 0;
}

static void dev_set_clk_cell(struct device *dev, int i, struct clk *clk)
{
	struct cc_desc *ccd = dev_get_drvdata(dev);

	if (ccd->data.clks[i]) {
		dev_warn(dev, "can't set %s to cell %d (%s already exists)\n",
			__clk_get_name(clk), i,
			__clk_get_name(ccd->data.clks[i]));
		return;
	}
	ccd->data.clks[i] = clk;
}


static int __init init_clks(struct device *dev)
{
	struct cc_desc *ccd = dev_get_drvdata(dev);
	struct clk *clk;
	int i;

	for (i = 0; i < ARRAY_SIZE(clk_reg_list); i++) {
		struct clk_hw *hw = clk_reg_list[i];
		struct clk_reg *reg = to_clk_reg(clk_reg_list[i]);

		if (!hw)
			continue;

		pr_info("%s: %s\n", __func__, hw->init->name);

		clk_reg_init(reg, &ccd->init_data);

		/* covert clk_reg flags to clk flags */
		((struct clk_init_data *)hw->init)->flags |=
			clk_reg_get_flags(reg);

		clk = clk_register(dev, hw);
		if (IS_ERR(clk)) {
			dev_err(dev, "Failed to register %s: %ld",
				hw->init->name, PTR_ERR(clk));
			continue;
		}
		clk_register_clkdev(clk, hw->init->name, NULL);

		dev_set_clk_cell(dev, i, hw->clk);

		/* ignore pll vodma and pll acpu */
		if (i == CC_PLL_VODMA || i == CC_PLL_ACPU)
			continue;

		if (ccd->pm_data)
			ccd->pm_data[i].flags |= CC_SUSPEND_RATE;
	}

	for (i = 0; i < ARRAY_SIZE(fixed_clks); i++) {
		struct clk_hw *hw = fixed_clks[i];

		if (!hw)
			continue;

		clk = clk_register(dev, hw);
		if (IS_ERR(clk)) {
			dev_err(dev, "Failed to register %s: %ld",
			hw->init->name, PTR_ERR(clk));
			continue;
		}


		clk_register_clkdev(clk, hw->init->name, NULL);

		dev_set_clk_cell(dev, i, hw->clk);
	}

	for (i = 0; i < ARRAY_SIZE(composite_clks); i++) {
		struct clk_composite_init_data *data = composite_clks[i];

		if (!data)
			continue;

		init_composite_clk(dev, data);
		dev_set_clk_cell(dev, i, data->clk);

		if (ccd->pm_data) {
			if (data->mux_offset != CLK_OFFSET_INVALID)
				ccd->pm_data[i].flags |= CC_SUSPEND_PARENT;
			if (data->gate_offset != CLK_OFFSET_INVALID)
				ccd->pm_data[i].flags |= CC_SUSPEND_ENABLE;
		}
	}

	return 0;
}
#ifdef CONFIG_PM
static int clk_pm_save(struct clk *clk, struct clk_pm_data *pm_data)
{
	if (!clk)
		return 0;

	if (pm_data->flags & CC_SUSPEND_PARENT)
		pm_data->parent = clk_get_parent(clk);

	if (pm_data->flags & CC_SUSPEND_RATE)
		pm_data->rate = clk_get_rate(clk);

	if (pm_data->flags & CC_SUSPEND_ENABLE) {
		pm_data->enable_cnt = __clk_get_enable_count(clk);

		while (__clk_get_enable_count(clk) > 0)
			clk_disable_unprepare(clk);
	}

	pr_debug("%s: data[f = 0x%x, p = %s, r = %lu, en_cnt = %d]\n",
		__clk_get_name(clk), pm_data->flags,
		__clk_get_name(pm_data->parent), pm_data->rate,
		pm_data->enable_cnt);

	return 0;
}

static int clk_pm_restore(struct clk *clk, struct clk_pm_data *pm_data)
{

	if (!clk)
		return 0;

	if (pm_data->flags & CC_SUSPEND_ENABLE) {
		while (pm_data->enable_cnt > 0) {
			clk_prepare_enable(clk);
			pm_data->enable_cnt --;
		}
	}

	if (pm_data->parent) {
		/* clear the old parent */
		clk_set_parent(clk, NULL);

		/* restore parent */
		clk_set_parent(clk, pm_data->parent);
	}

	if (pm_data->rate) {
		long old;

		old = clk_get_rate(clk);
		clk_set_rate(clk, pm_data->rate);

		pr_debug("%s: %ld -> %ld\n", __clk_get_name(clk),
			old, clk_get_rate(clk));
	}

	return 0;
}

static int rtk_cc_suspend(struct device *dev)
{
	int i;
	struct cc_desc *ccd = dev_get_drvdata(dev);

#ifdef CONFIG_SUSPEND
	if (RTK_PM_STATE == PM_SUSPEND_STANDBY)
		return 0;
#endif
	dev_info(dev, "[CLK] Enter %s\n", __func__);

	for (i = 0; i < CC_CLK_MAX; i++)
		clk_pm_save(ccd->data.clks[i], &ccd->pm_data[i]);

	dev_info(dev, "[CLK] Exit %s\n", __func__);
	return 0;
}

static int rtk_cc_resume(struct device *dev)
{
	int i;
	struct cc_desc *ccd = dev_get_drvdata(dev);
#ifdef CONFIG_SUSPEND
	if (RTK_PM_STATE == PM_SUSPEND_STANDBY)
		return 0;
#endif
	dev_info(dev, "[CLK] Enter %s\n", __func__);

	for (i = CC_CLK_MAX - 1; i >= 0; i--)
		clk_pm_restore(ccd->data.clks[i], &ccd->pm_data[i]);

	dev_info(dev, "[CLK] Exit %s\n", __func__);
	return 0;
}
#else /* !CONFIG_PM */

#define rtk_cc_suspend NULL
#define rtk_cc_resume NULL

#endif /* CONFIG_PM */

static const struct dev_pm_ops rtk_cc_pm_ops = {
	.suspend_noirq = rtk_cc_suspend,
	.resume_noirq = rtk_cc_resume,
};

static int rtk_cc_probe(struct platform_device *pdev)
{
	struct cc_desc *ccd;
	void __iomem *reg;
	struct regmap *regmap;
	int offset = 0;
	int ret;

	dev_info(&pdev->dev, "[CLK] %s\n", __func__);

	ccd = devm_kzalloc(&pdev->dev, sizeof(*ccd), GFP_KERNEL);
	if (!ccd)
		return -ENOMEM;

	reg = of_iomap(pdev->dev.of_node, 0);
	regmap = of_get_rtk_mmio_regmap_with_offset(pdev->dev.of_node, 0,
		&offset);
	if (!reg && IS_ERR_OR_NULL(regmap))
		return -EINVAL;
	if (IS_ERR(regmap))
		regmap = NULL;

	ccd->data.clk_num = CC_CLK_MAX;
	ccd->data.clks = devm_kcalloc(&pdev->dev, CC_CLK_MAX,
		sizeof(*ccd->data.clks), GFP_KERNEL);
	if (!ccd->data.clks)
		return -ENOMEM;

	ccd->pm_data = devm_kcalloc(&pdev->dev, CC_CLK_MAX,
		sizeof(*ccd->pm_data), GFP_KERNEL);
	if (!ccd->pm_data)
		return -ENOMEM;

	ccd->init_data.reg = reg;
	if (regmap) {
		dev_info(&pdev->dev, "use mmio regmap\n");
		ccd->init_data.offset = offset;
		ccd->init_data.regmap = regmap;
	}

	platform_set_drvdata(pdev, ccd);

	ret = init_clks(&pdev->dev);
	if (ret)
		dev_err(&pdev->dev, "Failed to init clocks: %d\n", ret);

	ret = of_clk_add_provider(pdev->dev.of_node, of_clk_src_onecell_get,
		&ccd->data);
	if (ret)
		dev_err(&pdev->dev, "Failed to add provider: %d\n", ret);

	if (clk_sys.base.hw.clk)
		clk_notifier_register(clk_sys.base.hw.clk, &clk_sys_sb2_nb);

	/* set alias */
	clk_add_alias("jpeg", NULL, "clk_en_jpeg", NULL);

	return 0;
}

static const struct of_device_id rtk_cc_match[] = {
	{.compatible = "realtek,clock-controller"},
	{}
};

static struct platform_driver rtk_cc_driver = {
	.probe = rtk_cc_probe,
	.driver = {
		.name = "rtk-cc",
		.of_match_table = rtk_cc_match,
		.pm = &rtk_cc_pm_ops,
	},
};

static int __init rtk_cc_init(void)
{
	return platform_driver_register(&rtk_cc_driver);
}
core_initcall(rtk_cc_init);
