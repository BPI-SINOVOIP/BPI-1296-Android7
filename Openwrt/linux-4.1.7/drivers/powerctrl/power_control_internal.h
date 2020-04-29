/*
 * power_control_internal.h
 *
 * Copyright (C) 2016 Realtek Semiconductor Corporation
 * Copyright (C) 2016 Cheng-Yu Lee <cylee12@realtek.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef _POWER_CONTROL_INTERNAL_H_
#define _POWER_CONTROL_INTERNAL_H_

#define POWER_CONTROL_CALL_STATE_NONE   0
#define POWER_CONTROL_CALL_STATE_OFF    1
#define POWER_CONTROL_CALL_STATE_ON     2

struct power_control_priv;

struct power_control_priv {
	int count_on;
	int count_off;
	int count_x;
	int last_call;

	unsigned int flags;
};
#define priv_get(p)       ((p)->priv)
#define priv_set(p, priv) (p)->priv = priv

static inline void __power_on_called(struct power_control_priv *priv)
{
	priv->count_on ++;
	priv->count_x ++;
	priv->last_call = POWER_CONTROL_CALL_STATE_ON;
}

static inline void __power_off_called(struct power_control_priv *priv)
{
	priv->count_off ++;
	priv->count_x --;
	priv->last_call = POWER_CONTROL_CALL_STATE_OFF;
}


extern struct mutex power_control_list_lock;
extern int power_control_list_lock_r;


static inline void __list_lock(void)
{
	mutex_lock(&power_control_list_lock);
};

static inline void __list_unlock(void)
{
	mutex_unlock(&power_control_list_lock);
}

extern struct list_head power_control_list;
#define to_power_control(_p) container_of((_p), struct power_control, list)

#endif
