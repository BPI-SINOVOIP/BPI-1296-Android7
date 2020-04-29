/*
 * hdcp_top.h - RTK hdcp tx driver header file
 *
 * Copyright (C) 2017 Realtek Semiconductor Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef _HDCP_TOP_H_
#define _HDCP_TOP_H_

#include <linux/interrupt.h>
#include "hdcp.h"

#if IS_ENABLED(CONFIG_COMPAT)
long compat_hdcp_ioctl(struct file* file,unsigned int cmd, unsigned long arg);
#else
#define compat_hdcp_ioctl  NULL
#endif

irqreturn_t HDCP_interrupt_handler(int irq, void *dev_id);

#ifdef CONFIG_RTK_HDCP_1x_TEE
#define USED_TA_CMD	1
#else
#define	USED_TA_CMD	0
#endif

#if(USED_TA_CMD)
extern int ta_hdcp_check_r0(uint8_t *ro);//for kylin B walk around
extern int ta_hdcp_lib_generate_an(uint8_t *an);
extern int ta_hdcp_lib_write_bksv(uint8_t *ksv_data, uint8_t *pk);
extern void ta_hdcp_lib_set_repeater_bit_in_tx(enum hdcp_repeater rx_mode);
extern int ta_hdcp_lib_check_repeater_bit_in_tx(void);
extern void ta_hdcp_lib_set_encryption(enum encryption_state enc_state);
extern void ta_hdcp_lib_set_wider_window(void);
extern void ta_hdcp_lib_set_ri(enum ri_state ri_state);
extern void ta_hdcp_lib_set_av_mute(enum av_mute av_mute_state);
extern int ta_hdcp_lib_SHA_append_bstatus_M0(struct hdcp_sha_in *sha, uint8_t *bstatus);
extern int ta_hdcp_lib_compute_V(struct hdcp_sha_in *sha);
extern int ta_hdcp_lib_verify_V(struct hdcp_sha_in *sha);
#endif

#endif /* _HDCP_TOP_H_ */
