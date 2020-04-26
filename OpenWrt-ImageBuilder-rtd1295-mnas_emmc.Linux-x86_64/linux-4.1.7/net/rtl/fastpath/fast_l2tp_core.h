/*
 * Copyright (C) 2017 Realtek Semiconductor Corporation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef FAST_L2TP_CORE_H
#define FAST_L2TP_CORE_H

#include <linux/skbuff.h>

int __init fast_l2tp_init(void);
void __exit fast_l2tp_exit(void);
void event_ppp_dev_down(const char *name);
int fast_l2tp_to_wan(void *skb);
//#ifdef CONFIG_SUPPORT_RUSSIA_FEATURES
int fast_l2tp_rx(void *skb);
//#else
//void fast_l2tp_rx(void *skb);
//#endif

int fast_l2tp_tx_lcp_echo_reply(unsigned char *data, int rightShift, void *skb);

void l2tp_tx_id(void *skb);
void (*l2tp_tx_id_hook)(void *skb);
unsigned long get_fast_l2tp_lastxmit(void);
int fast_l2tp_fw;
#endif

