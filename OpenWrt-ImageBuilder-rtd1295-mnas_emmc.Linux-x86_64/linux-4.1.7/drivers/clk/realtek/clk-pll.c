/*
 * clk-pll.c - simple pll implementation for RTD-129x
 *
 * Copyright (C) 2017 Realtek Semiconductor Corporation
 * Copyright (C) 2017 Cheng-Yu Lee <cylee12@realtek.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#define pr_fmt(fmt) "rtk-clk-pll: " fmt

#include <linux/io.h>
#include <linux/slab.h>
#include <linux/clk-provider.h>
#include <linux/clk.h>
#include <linux/spinlock.h>
#include <linux/delay.h>
#include <linux/debugfs.h>
#include "common.h"
#include "clk-pll.h"

#define SSC_VALID(_pll) CLK_OFFSET_IS_VALID((_pll)->ssc_offset)
#define RDY_VALID(_pll) CLK_OFFSET_IS_VALID((_pll)->rdy_offset)
#define POW_VALID(_pll) CLK_OFFSET_IS_VALID((_pll)->pow_offset)

static int clk_pll_debugfs_u64_set(void *data, u64 val)
{
	struct clk_hw *hw = data;

	clk_set_rate(hw->clk, (unsigned long)(val));
	return 0;
}

static int clk_pll_debugfs_u64_get(void *data, u64 *val)
{
	struct clk_hw *hw = data;

	*val = (u64)clk_get_rate(hw->clk);
	return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(clk_pll_rate_op, clk_pll_debugfs_u64_get,
	clk_pll_debugfs_u64_set, "%llu\n");

static int clk_pll_debug_init(struct clk_hw *hw, struct dentry *dentry)
{
	debugfs_create_file("rtk_clk_rate", 0644, dentry, hw, &clk_pll_rate_op);
	return 0;
}


static const struct freq_table *find_rate(const struct freq_table *table,
	unsigned long rate)
{
	unsigned long best_rate = 0;
	const struct freq_table *best = NULL;

	for ( ; !IS_FREQ_TABLE_END(table); table++) {
		if (table->rate == rate)
			return table;

		if (table->rate > rate)
			continue;

		if ((rate - best_rate) > (rate - table->rate)) {
			best_rate = table->rate;
			best = table;
		}
	}

	if (best)
		pr_warn("find closest rate = %lu for %lu\n",  best->rate, rate);

	return best;
}

static const struct freq_table *find_value(const struct freq_table *table,
	unsigned long value)
{
	while (!IS_FREQ_TABLE_END(table)) {
		if (table->val == (value & table->mask))
			return table;
		table++;
	}
	return NULL;
};

static int clk_pll_prepare(struct clk_hw *hw)
{
	struct clk_pll *pll = to_clk_pll(hw);
	unsigned long flags = 0;

	if (pll->lock)
		spin_lock_irqsave(pll->lock, flags);

	pll->flags |= CLK_PLL_PREPARED;
	if (POW_VALID(pll))
		clk_reg_write(&pll->base, pll->pow_offset, pll->pow_on_val);

	if (pll->lock)
		spin_unlock_irqrestore(pll->lock, flags);

	udelay(200);

	return 0;
}

static void clk_pll_unprepare(struct clk_hw *hw)
{
	struct clk_pll *pll = to_clk_pll(hw);
	unsigned long flags = 0;

	if (pll->lock)
		spin_lock_irqsave(pll->lock, flags);

	pll->flags &= ~CLK_PLL_PREPARED;
	if (POW_VALID(pll))
		clk_reg_write(&pll->base, pll->pow_offset, pll->pow_off_val);

	if (pll->lock)
		spin_unlock_irqrestore(pll->lock, flags);
}

static int clk_pll_is_prepared(struct clk_hw *hw)
{
	struct clk_pll *pll = to_clk_pll(hw);
	unsigned long flags = 0;
	bool val;

	if (pll->lock)
		spin_lock_irqsave(pll->lock, flags);

	if (POW_VALID(pll))
		val = (clk_reg_read(&pll->base, pll->pow_offset)) == pll->pow_on_val;
	else
		val = !!(pll->flags & CLK_PLL_PREPARED);

	if (pll->lock)
		spin_unlock_irqrestore(pll->lock, flags);

	return val;
}

static long clk_pll_round_rate(struct clk_hw *hw, unsigned long rate,
	unsigned long *parent_rate)
{
	return rate;
}


static unsigned long clk_pll_recalc_rate(struct clk_hw *hw,
	unsigned long parent_rate)
{
	struct clk_pll *pll = to_clk_pll(hw);
	unsigned long flags = 0;
	const struct freq_table *freq;
	unsigned int val;

	WARN_ON_ONCE(parent_rate != 27000000);

	if (pll->lock)
		spin_lock_irqsave(pll->lock, flags);

	val = clk_reg_read(&pll->base, pll->pll_offset);

	if (pll->lock)
		spin_unlock_irqrestore(pll->lock, flags);

	freq = find_value(pll->freq_tbl, val);
	if (!freq) {
		pr_err("%s: %s %08x\n", __func__, __clk_get_name(hw->clk), val);
		WARN_ON(1);
		return 0;
	}

	return freq->rate;
}

static int clk_pll_set_rate(struct clk_hw *hw, unsigned long rate,
	unsigned long parent_rate)
{
	struct clk_pll *pll = to_clk_pll(hw);
	unsigned long flags = 0;
	const struct freq_table *freq;
	int ret = 0;

	WARN_ON_ONCE(parent_rate != 27000000);

	if (pll->lock)
		spin_lock_irqsave(pll->lock, flags);

	if (SSC_VALID(pll))
		clk_reg_update(&pll->base, pll->ssc_offset, 0xf, 0xc);

	freq = find_rate(pll->freq_tbl, rate);
	if (freq) {
		clk_reg_update(&pll->base, pll->pll_offset, freq->mask,
			freq->val);
	} else {
		pr_err("%s has no rate %lu\n", __clk_get_name(hw->clk), rate);
		ret = -EINVAL;
	}

	if (SSC_VALID(pll))
		clk_reg_update(&pll->base, pll->ssc_offset, 0xf, 0xd);

	if (SSC_VALID(pll) && RDY_VALID(pll)) {
		int set = 0;

		if (POW_VALID(pll) && !clk_pll_is_prepared(hw)) {
			set = 1;
			clk_pll_prepare(hw);
			pr_notice("%s is not prepared\n", __clk_get_name(hw->clk));
		}

		while (!(clk_reg_read(&pll->base, pll->rdy_offset) & BIT(20)))
			;

		if (set)
			clk_pll_unprepare(hw);
	}

	if (pll->lock)
		spin_unlock_irqrestore(pll->lock, flags);

	return ret;
}

const struct clk_ops clk_pll_ops = {
	.prepare = clk_pll_prepare,
	.unprepare = clk_pll_unprepare,
	.is_prepared = clk_pll_is_prepared,
	.round_rate = clk_pll_round_rate,
	.recalc_rate = clk_pll_recalc_rate,
	.set_rate = clk_pll_set_rate,
	.debug_init = clk_pll_debug_init,
};

static const struct div_table *__dtbl_find_by_rate(
	const struct div_table *dtbl, unsigned long rate)
{
	while (!IS_DIV_TABLE_END(dtbl)) {
		if (rate >= dtbl->rate)
			return dtbl;
		dtbl++;
	}
	return NULL;
}

static const struct div_table *__dtbl_find_by_val(
	const struct div_table *dtbl, unsigned int val)
{
	while (!IS_DIV_TABLE_END(dtbl)) {
		if (val == dtbl->val)
			return dtbl;
		dtbl++;
	}
	return NULL;
}

static inline void __set_dval(struct clk_pll_scpu *clks, unsigned int val)
{
	clk_reg_update(&clks->clkp.base, clks->div_offset,
		(BIT(clks->div_width) - 1) << clks->div_shift,
		val << clks->div_shift);
}

static inline unsigned int __get_dval(struct clk_pll_scpu *clks)
{
	return (clk_reg_read(&clks->clkp.base, clks->div_offset) >>
		clks->div_shift) & (BIT(clks->div_width) - 1);
}

static unsigned long clk_pll_scpu_recalc_rate(struct clk_hw *hw,
	unsigned long parent_rate)
{
	struct clk_pll_scpu *clks = to_clk_pll_scpu(hw);
	unsigned long rate;
	const struct div_table *dtblv;
	unsigned int val;

	rate = clk_pll_recalc_rate(hw, parent_rate);

	val = __get_dval(clks);
	dtblv = __dtbl_find_by_val(clks->div_tbl, val);
	rate /= dtblv->div;

	pr_debug("%s: rate=%lu,v=%d,d=%d\n", __func__, rate, val, dtblv->div);

	return rate;
}

static int clk_pll_scpu_set_rate(struct clk_hw *hw, unsigned long rate,
	unsigned long parent_rate)
{
	struct clk_pll_scpu *clks = to_clk_pll_scpu(hw);
	unsigned long flags = 0;
	const struct div_table *ndtblv, *cdtblv;
	unsigned long target;
	unsigned int cur_d;

	pr_debug("%s: rate = %lu\n", __func__, rate);

	/* find next in the dtbl */
	ndtblv = __dtbl_find_by_rate(clks->div_tbl, rate);
	if (!ndtblv)
		return -EINVAL;
	pr_debug("%s: next div = %d, regval = 0x%x\n", __func__,
		ndtblv->div, ndtblv->val);

	target = rate * ndtblv->div;

	/* find current in the dtbl */
	cur_d = __get_dval(clks);
	cdtblv = __dtbl_find_by_val(clks->div_tbl, cur_d);
	if (!cdtblv)
		return -EINVAL;
	pr_debug("%s: currnet div = %d, regval = 0x%x\n", __func__,
		cdtblv->div, cdtblv->val);

	if (clks->lock)
		spin_lock_irqsave(clks->lock, flags);

#ifdef CONFIG_COMMON_CLK_RTD129X
	/* workaround to prevent glitch */
	if (ndtblv->val != cdtblv->val &&
		(ndtblv->val == 1 || cdtblv->val == 1)) {
		clk_pll_set_rate(hw, 1000000000, parent_rate);
		__set_dval(clks, ndtblv->val);
		cdtblv = ndtblv;
	}
#endif

	if (ndtblv->div > cdtblv->div)
		__set_dval(clks, ndtblv->val);
	clk_pll_set_rate(hw, target, parent_rate);
	if (ndtblv->div < cdtblv->div)
		__set_dval(clks, ndtblv->val);

	if (clks->lock)
		spin_unlock_irqrestore(clks->lock, flags);

	return 0;
}

const struct clk_ops clk_pll_scpu_ops = {
	.recalc_rate = clk_pll_scpu_recalc_rate,
	.round_rate = clk_pll_round_rate,
	.set_rate = clk_pll_scpu_set_rate,
	.debug_init = clk_pll_debug_init,
};

