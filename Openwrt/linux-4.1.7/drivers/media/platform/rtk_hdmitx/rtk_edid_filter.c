/*
 * rtk_edid_filter.c - RTK hdmitx driver
 *
 * Copyright (C) 2017 Realtek Semiconductor Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <drm/drm_edid.h>
#include <drm/drm_crtc.h>
#include <linux/types.h>

#include "hdmitx.h"
#include "rtk_edid.h"


struct edid_product_info_t {
	unsigned char mfg_name[2];
	unsigned char prod_code[2];
	__u64 vic_mask;/* VIC 1~64, BIT0=VIC1 */
	__u64 vic2_mask;/* VIC 65~128, BIT0=VIC65 */
	unsigned char extended_vic_mask;
	__u64 vic2_420_mask;/* YCbCr420 format, VIC 65~128, BIT0=VIC65 */
};


#define PRODUCT_INFO_SIZE 4

static const struct edid_product_info_t product_info[] = {
	/* ActivPanel AP4-55 doesn't support 4K */
	{.mfg_name[0] = 0x2E, .mfg_name[1] = 0x83,
	.prod_code[0] = 0x00, .prod_code[1] = 0x55,
	.vic_mask = 0xFFFFFFFFFFFFFFFF,
	.vic2_mask = 0x0,
	.extended_vic_mask = 0x0,
	.vic2_420_mask = 0x0},
	/*DENON_AVR-2312CI doesn't support 4K */
	{.mfg_name[0] = 0x11, .mfg_name[1] = 0xee,
	.prod_code[0] = 0x28, .prod_code[1] = 0x00,
	.vic_mask = 0xFFFFFFFFFFFFFFFF,
	.vic2_mask = 0x0,
	.extended_vic_mask = 0x0,
	.vic2_420_mask = 0x0},
	/* HiTV doesn't support 6G 4K */
	{.mfg_name[0] = 0x22, .mfg_name[1] = 0x69,
	.prod_code[0] = 0x01, .prod_code[1] = 0x00,
	.vic_mask = 0xFFFFFFFFFFFFFFFF,
	.vic2_mask = 0x0,
	.extended_vic_mask = 0xFF,
	.vic2_420_mask = 0xFFFFFFFFFFFFFFFF},
	/* Optika doesn't support 6G 4K */
	{.mfg_name[0] = 0x12, .mfg_name[1] = 0xCC,
	.prod_code[0] = 0x96, .prod_code[1] = 0x40,
	.vic_mask = 0xFFFFFFFFFFFFFFFF,
	.vic2_mask = 0x0,
	.extended_vic_mask = 0xFF,
	.vic2_420_mask = 0xFFFFFFFFFFFFFFFF},
};

int rtk_filter_specific_profuct_vic(struct edid *edid, struct sink_capabilities_t *sink_cap)
{
	int i;

	for (i = 0; i < PRODUCT_INFO_SIZE; i++) {
		if ((edid->mfg_id[0] == product_info[i].mfg_name[0]) &&
			(edid->mfg_id[1] == product_info[i].mfg_name[1]) &&
			(edid->prod_code[0] == product_info[i].prod_code[0]) &&
			(edid->prod_code[1] == product_info[i].prod_code[1])) {

			sink_cap->vic = sink_cap->vic&product_info[i].vic_mask;
			sink_cap->vic2 = sink_cap->vic2&product_info[i].vic2_mask;
			sink_cap->extended_vic = sink_cap->extended_vic&product_info[i].extended_vic_mask;
			sink_cap->vic2_420 = sink_cap->vic2_420&product_info[i].vic2_420_mask;

			HDMI_INFO("Filter vic_mask=0x%llx", product_info[i].vic_mask);
			HDMI_INFO("Filter extended_vic_mask=%u", product_info[i].extended_vic_mask);
			HDMI_INFO("Filter vic2_mask=0x%llx", product_info[i].vic2_mask);
			HDMI_INFO("Filter vic2_420_mask=0x%llx", product_info[i].vic2_420_mask);

			return 1;
		}
	}
	return 0;
}

