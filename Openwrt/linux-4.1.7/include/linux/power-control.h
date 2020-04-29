/*
 * Copyright (C) 2017 Realtek Semiconductor Corporation
 * Copyright (C) 2017 Cheng-Yu Lee <cylee12@realtek.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef _LINUX_POWER_CONTROL_H_
#define _LINUX_POWER_CONTROL_H_

struct device_node;
struct power_control;

#ifdef CONFIG_POWER_CONTROL
const char *power_control_get_name(struct power_control *pctrl);
int power_control_power_on(struct power_control *pctrl);
int power_control_power_off(struct power_control *pctrl);
int power_control_is_powered_on(struct power_control *pctrl);
struct power_control *power_control_get(const char *name);
struct power_control *of_power_control_get_by_index(struct device_node *node, int index);
struct power_control *of_power_control_get(struct device_node *node, const char *id);
bool power_control_is_match(const struct power_control *a, const struct power_control *b);
int power_control_register(struct power_control *pctrl);
void power_control_unregister(struct power_control *pctrl);
#else

inline static const char *power_control_get_name(struct power_control *pctrl)
{
	return NULL;
}

inline static int power_control_power_on(struct power_control *pctrl)
{
	return -EINVAL;
}

inline static int power_control_power_off(struct power_control *pctrl)
{
	return -EINVAL;
}

inline static int power_control_is_powered_on(struct power_control *pctrl)
{
	return -EINVAL;
}

inline static struct power_control *power_control_get(const char *name)
{
	return ERR_PTR(-ENOSYS);
}

inline static struct power_control *of_power_control_get_by_index(struct device_node *node,
	int index)
{
	return ERR_PTR(-ENOSYS);
}

inline static struct power_control *of_power_control_get(struct device_node *node,
				       const char *id)
{
	return ERR_PTR(-ENOSYS);
}

inline static bool power_control_is_match(const struct power_control *a,
	const struct power_control *b) {
	return true;
}

inline static int power_control_register(struct power_control *pctrl)
{
	return -ENOSYS;
}

inline static void power_control_unregister(struct power_control *pctrl)
{
}

#endif

inline static int power_control_enable_hw_pm(struct power_control *pctrl)
{
	return power_control_power_off(pctrl);
}

inline static int power_control_disable_hw_pm(struct power_control *pctrl)
{
	return power_control_power_on(pctrl);
}

#endif
