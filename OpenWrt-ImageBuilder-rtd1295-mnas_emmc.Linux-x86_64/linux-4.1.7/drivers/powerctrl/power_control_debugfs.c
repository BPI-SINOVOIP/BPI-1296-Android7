/*
 * power_control_debugfs.c - debugfs for stat
 *
 * Copyright (C) 2016 Realtek Semiconductor Corporation
 * Copyright (C) 2016 Cheng-Yu Lee <cylee12@realtek.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/slab.h>
#include <linux/power-control.h>
#include <linux/mutex.h>
#include <linux/debugfs.h>
#include "power_control_internal.h"
#include "power_control_driver.h"

#define POWER_CONTROL_IS_HW_PM  0x1

static struct dentry *power_control_root;

int power_control_mark_as_hw_pm(struct power_control *pctrl)
{
	struct power_control_priv *priv;

	if (!pctrl)
		return -EINVAL;

	priv = priv_get(pctrl);

	priv->flags |= POWER_CONTROL_IS_HW_PM;

	return 0;
}

#define DEFINE_OPEN(open_func_name, show_func_name) \
static int open_func_name(struct inode *inode, struct file *file) \
{ \
	return single_open(file, show_func_name, inode->i_private); \
}

static const char * state_str [] = {
	"OFF",
	"ON",
	"ENABLED",
	"DISABLED",
	"INVAL",
	"NOT_PERM"
};
static const char * last_str [] = {
	"POWER_OFF",
	"POWER_ON",
	"ENABLE",
	"DISABLE",
	"NONE"
};

static int power_control_summary_show(struct seq_file *s, void *data)
{
	struct list_head *it = NULL;

	seq_printf(s, "%-28s %-5s %-9s %3s = %8s - %8s %s\n",
			"POWER_CONTROL_NAME", "TYPE", "STATE",
			"X", "CNT_ON", "CNT_OFF", "LAST_CALL");

	__list_lock();
	power_control_list_lock_r = 1;

	list_for_each(it, &power_control_list) {
		struct power_control *pctrl = to_power_control(it);
		struct power_control_priv *priv = priv_get(pctrl);
		int is_powered = power_control_is_powered_on(pctrl);
		int st;
		int last;

		if (is_powered < 0)
			st = is_powered == -EPERM ? 5 : 4;
		else
			st = (is_powered ? 1 : 0) | ((priv->flags & POWER_CONTROL_IS_HW_PM) ? 2 : 0);

		if (priv->last_call == POWER_CONTROL_CALL_STATE_NONE)
			last = 4;
		else
			last = (priv->last_call == POWER_CONTROL_CALL_STATE_ON ?  1 : 0) |
				((priv->flags & POWER_CONTROL_IS_HW_PM) ? 2 : 0);

		seq_printf(s, "%-28s %-5s %-9s %3d   %8d   %8d %s\n",
			pctrl->name,
			priv->flags & POWER_CONTROL_IS_HW_PM ? "HW_PM": "POWER",
			state_str[st],
			priv->count_x,
			priv->count_on,
			priv->count_off,
			last_str[last]
		);

	}

	power_control_list_lock_r = 0;
	__list_unlock();

	return 0;

}

DEFINE_OPEN(power_control_summary_open, power_control_summary_show);

static struct file_operations power_control_summary_ops = {
	.owner   = THIS_MODULE,
	.open    = power_control_summary_open,
	.read    = seq_read,
	.release = single_release,
};

static int __init power_control_debugfs_init(void)
{
	power_control_root = debugfs_create_dir("power_control", NULL);
	debugfs_create_file("power_control_summary", 0444,
		power_control_root, NULL, &power_control_summary_ops);
	return 0;
}
late_initcall(power_control_debugfs_init);

