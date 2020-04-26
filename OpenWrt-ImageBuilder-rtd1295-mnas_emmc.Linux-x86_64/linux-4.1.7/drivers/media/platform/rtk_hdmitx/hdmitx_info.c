/*
 * hdmitx_info.c - RTK hdmitx driver
 *
 * Copyright (C) 2017 Realtek Semiconductor Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/platform_device.h>

#include <drm/drm_edid.h>
#include <drm/drm_crtc.h>

#include "hdmitx.h"
#include "hdmitx_api.h"
#include "hdmitx_rpc.h"
#include "rtk_edid.h"

static unsigned char hpd_interlock = 1;

struct VO_STANDARD_TABLE_T {
	unsigned int width;
	unsigned int height;
	unsigned int fps;
	unsigned int interlace;
};

const struct VO_STANDARD_TABLE_T vo_standard_table[] = {
	/* {width, height, fps, interlace} */
	{  720,  480, 60, 1},/* VO_STANDARD_NTSC_M = 0 */
	{  720,  480, 60, 1},/* VO_STANDARD_NTSC_J = 1 */
	{  720,  480, 60, 1},/* VO_STANDARD_NTSC_443 = 2 */
	{  720,  576, 60, 1},/* VO_STANDARD_PAL_B = 3 */
	{  720,  576, 60, 1},/* VO_STANDARD_PAL_D = 4 */
	{  720,  576, 60, 1},/* VO_STANDARD_PAL_G = 5 */
	{  720,  576, 60, 1},/* VO_STANDARD_PAL_H = 6 */
	{  720,  576, 60, 1},/* VO_STANDARD_PAL_I = 7 */
	{  720,  576, 60, 1},/* VO_STANDARD_PAL_N = 8 */
	{  720,  576, 60, 1},/* VO_STANDARD_PAL_NC = 9 */
	{  720,  576, 60, 1},/* VO_STANDARD_PAL_M = 10 */
	{  720,  576, 60, 1},/* VO_STANDARD_PAL_60 = 11 */
	{ 0, 0, 0, 0},/* VO_STANDARD_SECAM = 12 */
	{ 1280,  720, 60, 0},/* VO_STANDARD_HDTV_720P_60 = 13 */
	{ 1280,  720, 50, 0},/* VO_STANDARD_HDTV_720P_50 = 14 */
	{ 1280,  720, 30, 0},/* VO_STANDARD_HDTV_720P_30 = 15 */
	{ 1280,  720, 25, 0},/* VO_STANDARD_HDTV_720P_25 = 16 */
	{ 1280,  720, 24, 0},/* VO_STANDARD_HDTV_720P_24 = 17 */
	{ 1920, 1080, 60, 1},/* VO_STANDARD_HDTV_1080I_60 = 18 */
	{ 1920, 1080, 50, 1},/* VO_STANDARD_HDTV_1080I_50 = 19 */
	{ 1920, 1080, 30, 0},/* VO_STANDARD_HDTV_1080P_30 = 20 */
	{ 1920, 1080, 25, 0},/* VO_STANDARD_HDTV_1080P_25 = 21 */
	{ 1920, 1080, 24, 0},/* VO_STANDARD_HDTV_1080P_24 = 22 */
	{ 0, 0, 0, 0},/* VO_STANDARD_VGA = 23 */
	{ 0, 0, 0, 0},/* VO_STANDARD_SVGA = 24 */
	{ 1920, 1080, 60, 0},/* VO_STANDARD_HDTV_1080P_60 = 25 */
	{ 1920, 1080, 50, 0},/* VO_STANDARD_HDTV_1080P_50 = 26 */
	{ 1920, 1080, 59, 1},/* VO_STANDARD_HDTV_1080I_59 = 27 */
	{ 1280,  720, 59, 0},/* VO_STANDARD_HDTV_720P_59 = 28 */
	{ 1920, 1080, 23, 0},/* VO_STANDARD_HDTV_1080P_23 = 29 */
	{ 1920, 1080, 59, 0},/* VO_STANDARD_HDTV_1080P_59 = 30 */
	{ 1920, 1080, 60, 0},/* VO_STANDARD_HDTV_1080P_60_3D = 31 */
	{ 1920, 1080, 50, 0},/* VO_STANDARD_HDTV_1080P_50_3D = 32 */
	{ 1920, 1080, 30, 0},/* VO_STANDARD_HDTV_1080P_30_3D = 33 */
	{ 1920, 1080, 24, 0},/* VO_STANDARD_HDTV_1080P_24_3D = 34 */
	{ 1280,  720, 60, 0},/* VO_STANDARD_HDTV_720P_60_3D = 35 */
	{ 1280,  720, 50, 0},/* VO_STANDARD_HDTV_720P_50_3D = 36 */
	{ 1280,  720, 30, 0},/* VO_STANDARD_HDTV_720P_30_3D = 37 */
	{ 1280,  720, 24, 0},/* VO_STANDARD_HDTV_720P_24_3D = 38 */
	{ 1280,  720, 59, 0},/* VO_STANDARD_HDTV_720P_59_3D = 39 */
	{ 1920, 1080, 60, 1},/* VO_STANDARD_HDTV_1080I_60_3D = 40 */
	{ 1920, 1080, 59, 1},/* VO_STANDARD_HDTV_1080I_59_3D = 41 */
	{ 1920, 1080, 50, 1},/* VO_STANDARD_HDTV_1080I_50_3D = 42 */
	{ 1920, 1080, 23, 0},/* VO_STANDARD_HDTV_1080P_23_3D = 43 */
	{ 3840, 2160, 30, 0},/* VO_STANDARD_HDTV_2160P_30 = 44  */
	{ 3840, 2160, 29, 0},/* VO_STANDARD_HDTV_2160P_29 = 45 */
	{ 3840, 2160, 25, 0},/* VO_STANDARD_HDTV_2160P_25 = 46 */
	{ 3840, 2160, 24, 0},/* VO_STANDARD_HDTV_2160P_24 = 47 */
	{ 3840, 2160, 23, 0},/* VO_STANDARD_HDTV_2160P_23 = 48 */
	{ 4096, 2160, 24, 0},/* VO_STANDARD_HDTV_4096_2160P_24 = 49 */
	{ 3840, 2160, 60, 0},/* VO_STANDARD_HDTV_2160P_60 = 50 */
	{ 3840, 2160, 50, 0},/* VO_STANDARD_HDTV_2160P_50 = 51 */
	{ 4096, 2160, 25, 0},/* VO_STANDARD_HDTV_4096_2160P_25 =  52 */
	{ 4096, 2160, 30, 0},/* VO_STANDARD_HDTV_4096_2160P_30 = 53 */
	{ 4096, 2160, 50, 0},/* VO_STANDARD_HDTV_4096_2160P_50 = 54 */
	{ 4096, 2160, 60, 0},/* VO_STANDARD_HDTV_4096_2160P_60 = 55 */
	{ 3840, 2160, 60, 0},/* VO_STANDARD_HDTV_2160P_60_420 = 56 */
	{ 3840, 2160, 50, 0},/* VO_STANDARD_HDTV_2160P_50_420 = 57 */
	{ 4096, 2160, 60, 0},/* VO_STANDARD_HDTV_4096_2160P_60_420 = 58 */
	{ 4096, 2160, 50, 0},/* VO_STANDARD_HDTV_4096_2160P_50_420 = 59 */
	{ 1920, 1080, 60, 0},/* VO_STANDARD_DP_FORMAT_1920_1080P_60 = 60 */
	{ 3840, 2160, 30, 0},/* VO_STANDARD_DP_FORMAT_2160P_30 = 61 */
	{ 3840, 2160, 24, 0},/* VO_STANDARD_HDTV_2160P_24_3D = 62 */
	{ 3840, 2160, 23, 0},/* VO_STANDARD_HDTV_2160P_23_3D = 63 */
	{ 3840, 2160, 59, 0},/* VO_STANDARD_HDTV_2160P_59 = 64 */
	{ 3840, 2160, 59, 0},/* VO_STANDARD_HDTV_2160P_59_420 =65 */
	{ 3840, 2160, 25, 0},/* VO_STANDARD_HDTV_2160P_25_3D = 66 */
	{ 3840, 2160, 30, 0},/* VO_STANDARD_HDTV_2160P_30_3D = 67 */
	{ 3840, 2160, 50, 0},/* VO_STANDARD_HDTV_2160P_50_3D = 68 */
	{ 3840, 2160, 60, 0},/* VO_STANDARD_HDTV_2160P_60_3D = 69 */
	{ 4096, 2160, 24, 0},/* VO_STANDARD_HDTV_4096_2160P_24_3D = 70 */
	{ 4096, 2160, 25, 0},/* VO_STANDARD_HDTV_4096_2160P_25_3D = 71 */
	{ 4096, 2160, 30, 0},/* VO_STANDARD_HDTV_4096_2160P_30_3D = 72 */
	{ 4096, 2160, 50, 0},/* VO_STANDARD_HDTV_4096_2160P_50_3D = 73 */
	{ 4096, 2160, 60, 0},/* VO_STANDARD_HDTV_4096_2160P_60_3D = 74 */
	{ 1280,  720, 60, 0},/* VO_STANDARD_DP_FORMAT_1280_720P_60 = 75 */
	{ 3840, 2160, 60, 0},/* VO_STANDARD_DP_FORMAT_3840_2160P_60 = 76 */
	{ 1024,  768, 60, 0},/* VO_STANDARD_DP_FORMAT_1024_768P_60 = 77 */
	{ 3840, 2160, 50, 0},/* VO_STANDARD_HDTV_2160P_50_422_12bit = 78 */
	{ 3840, 2160, 60, 0},/* VO_STANDARD_HDTV_2160P_60_422_12bit = 79 */
};


/* EISA ID is based upon compressed ASCII */
static const unsigned char eisa_id[] = {
	' ', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
	'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
	'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
	'X', 'Y', 'Z', ' ', ' ', ' ', ' ', ' '
};

static const char * const mode[] = {
	"DVI", "HDMI", "OFF", "MHL", "OFF"
};

static const char * const pixel_format[] = {
	"RGB", "YUV422", "YUV444", "YUV420",
	"Reserved", "Reserved", "Reserved", "IDO-Defined"
};

static const char * const depth[] = {
	"8 Bits", "10 Bits", "12 Bits"
};

static const char * const colorimetry[] = {
	"No Data", "SMPTE 170M", "ITU 709", "Extend"
};

static const char * const extended_colorimetry[] = {
	"xvYCC601", "xvYCC709", "sYCC601", "AdobeYCC601",
	"AdobeRGB", "BT2020YcCbcCrx", "BT2020RGB", "Reserved"
};

static const char * const format3d[] = {
	"Auto", "FramePacking", "Side-by-Side", "Top-and-Bottom"
};

static const char * const yes_no[] = {
	"No", "Yes"
};

ssize_t show_hdmitx_info(struct device *cd, struct device_attribute *attr, char *buf)
{
	unsigned int ret_count;
	unsigned int vo_standard;
	unsigned int vic;
	unsigned char color_index;
	unsigned char depp_depth;
	struct VIDEO_RPC_VOUT_CONFIG_TV_SYSTEM hdmitx_tv_system;

	RPC_ToAgent_QueryConfigTvSystem(&hdmitx_tv_system);

	ret_count = sprintf(buf, "------ HDMI Info -----\n");

	vo_standard = hdmitx_tv_system.videoInfo.standard;

	/* Mode:DVI/HDMI/OFF */
	if (hdmitx_tv_system.hdmiInfo.hdmiMode <= 4)
		ret_count += sprintf(buf+ret_count, "Mode: %s\n",
			mode[hdmitx_tv_system.hdmiInfo.hdmiMode]);

	/* RTK VO_STANDARD */
	ret_count += sprintf(buf+ret_count, "RTK VO_STANDARD: %u\n", vo_standard);

	if (hdmitx_tv_system.hdmiInfo.hdmiMode == VO_HDMI_OFF)
		return ret_count;

	/* VIC */
	vic = hdmitx_tv_system.hdmiInfo.dataByte4&0x7F;
	if (vic == 0) {
		/* extended VIC */
		switch (vo_standard) {
		case VO_STANDARD_HDTV_2160P_23:
		case VO_STANDARD_HDTV_2160P_24:
			vic = 93;
			break;
		case VO_STANDARD_HDTV_2160P_25:
			vic = 94;
			break;
		case VO_STANDARD_HDTV_2160P_29:
		case VO_STANDARD_HDTV_2160P_30:
			vic = 95;
			break;
		case VO_STANDARD_HDTV_4096_2160P_24:
			vic = 98;
			break;
		default:
			break;
		}
	}
	ret_count += sprintf(buf+ret_count, "VIC: %u\n", vic);

	/* Resolution: Width x Height I/P fps */
	ret_count += sprintf(buf+ret_count, "Resolution: %ux%u%s @ %uHz\n",
		vo_standard_table[vo_standard].width,
		vo_standard_table[vo_standard].height,
		(hdmitx_tv_system.videoInfo.enProg&0x1)?"P":"I",
		vo_standard_table[vo_standard].fps);

	/* Pixel format: RGB/YUV422/YUV444/YUV420 */
	switch (hdmitx_tv_system.videoInfo.standard) {
	case VO_STANDARD_HDTV_2160P_50_420:
	case VO_STANDARD_HDTV_2160P_59_420:
	case VO_STANDARD_HDTV_2160P_60_420:
	case VO_STANDARD_HDTV_4096_2160P_50_420:
	case VO_STANDARD_HDTV_4096_2160P_60_420:
		ret_count += sprintf(buf+ret_count, "Pixel format: %s\n",
			pixel_format[3]);/* YUV420 */
		break;
	default:
		ret_count += sprintf(buf+ret_count, "Pixel format: %s\n",
			pixel_format[hdmitx_tv_system.hdmiInfo.dataByte1>>5]);
		break;
	}

	/* Color depth: 24 Bits/30 Bits/36 Bits */
	depp_depth = (hdmitx_tv_system.hdmiInfo.dataInt0>>2)&0xF;
	if (depp_depth == 5)
		ret_count += sprintf(buf+ret_count, "Color depth: %s\n", depth[1]);
	else if (depp_depth == 6)
		ret_count += sprintf(buf+ret_count, "Color depth: %s\n", depth[2]);
	else
		ret_count += sprintf(buf+ret_count, "Color depth: %s\n", depth[0]);

	/* Colorimetry:SMPTE 170M/ITU 709/xvYCC601/xvYCC709/sYCC601/AdobeYCC601/AdobeRGB/BT2020YcCbcCrx/BT2020RGB */
	color_index = (hdmitx_tv_system.hdmiInfo.dataByte2>>6)&0x3;
	if (color_index != 0x3) {
		/* Colorimetry C1:C0 */
		ret_count += sprintf(buf+ret_count, "Colorimetry: %s\n",
			colorimetry[color_index]);
	} else {
		/* Extended Colorimetry EC2:EC1:EC0 */
		color_index = (hdmitx_tv_system.hdmiInfo.dataByte3>>4)&0x7;
		ret_count += sprintf(buf+ret_count, "Colorimetry: %s\n",
			extended_colorimetry[color_index]);
	}

	/* 3D:"N/A"/FramePacking/Side-by-Side/Top-and-Bottom */
	switch (hdmitx_tv_system.videoInfo.standard) {
	case VO_STANDARD_HDTV_1080P_60_3D:
	case VO_STANDARD_HDTV_1080P_50_3D:
	case VO_STANDARD_HDTV_1080P_30_3D:
	case VO_STANDARD_HDTV_1080P_24_3D:
	case VO_STANDARD_HDTV_720P_60_3D:
	case VO_STANDARD_HDTV_720P_50_3D:
	case VO_STANDARD_HDTV_720P_30_3D:
	case VO_STANDARD_HDTV_720P_24_3D:
	case VO_STANDARD_HDTV_720P_59_3D:
	case VO_STANDARD_HDTV_1080I_60_3D:
	case VO_STANDARD_HDTV_1080I_59_3D:
	case VO_STANDARD_HDTV_1080I_50_3D:
	case VO_STANDARD_HDTV_1080P_23_3D:
	case VO_STANDARD_HDTV_2160P_24_3D:/* 4K 3D */
	case VO_STANDARD_HDTV_2160P_23_3D:
		ret_count += sprintf(buf+ret_count, "3D: %s\n",
			format3d[(hdmitx_tv_system.hdmiInfo.dataInt0>>8)&0x3]);
		break;
	default:
		ret_count += sprintf(buf+ret_count, "3D: N/A\n");
		break;
	}

	/* HDMI2.0:YES/NO */
	ret_count += sprintf(buf+ret_count, "HDMI2.0: %s\n",
		yes_no[hdmitx_tv_system.hdmiInfo.hdmi2p0_feature&0x1]);

	/* Scramble:YES/NO */
	ret_count += sprintf(buf+ret_count, "Scramble: %s\n",
		yes_no[(hdmitx_tv_system.hdmiInfo.hdmi2p0_feature>>1)&0x1]);

	return ret_count;
}

ssize_t show_edid_info(struct device *cd, struct device_attribute *attr, char *buf)
{
	unsigned int ret_count;
	unsigned int vendor_id;
	unsigned char id_index[3];

	ret_count = sprintf(buf, "------ EDID Info -----\n");

	vendor_id = hdmitx_edid_info.mfg_id[0];
	vendor_id = (vendor_id<<8) | hdmitx_edid_info.mfg_id[1];
	ret_count += sprintf(buf+ret_count, "Vendor ID:%04x\n", vendor_id);

	if (hdmitx_edid_info.hdmi_id == HDMI_2P0_IDENTIFIER)
		ret_count += sprintf(buf+ret_count, "HDMI version:2.0\n");
	else
		ret_count += sprintf(buf+ret_count, "HDMI version:1.x\n");

	/* EISA ID for manufacturer name, using five-bit codes */
	id_index[0] = (vendor_id>>10)&0x1F;
	id_index[1] = (vendor_id>>5)&0x1F;
	id_index[2] = vendor_id&0x1F;
	ret_count += sprintf(buf+ret_count, "TV name:%c%c%c\n",
		eisa_id[id_index[0]], eisa_id[id_index[1]], eisa_id[id_index[2]]);

	ret_count += sprintf(buf+ret_count, "ProductCode:%02x%02x\n",
		hdmitx_edid_info.prod_code[1], hdmitx_edid_info.prod_code[0]);

	ret_count += sprintf(buf+ret_count, "SerialNumber:%08x\n",
		hdmitx_edid_info.serial);

	ret_count += sprintf(buf+ret_count, "ManufactureYear:%u\n",
		1990+hdmitx_edid_info.mfg_year);

	ret_count += sprintf(buf+ret_count, "ManufactureWeek:%u\n",
		hdmitx_edid_info.mfg_week);

	if (hdmitx_edid_info.hdmi_id == HDMI_2P0_IDENTIFIER) {
		ret_count += sprintf(buf+ret_count, "Max TMDS character rate:%u\n",
			hdmitx_edid_info.max_tmds_char_rate);

		ret_count += sprintf(buf+ret_count, "Deep Color 420:0x%x\n",
			hdmitx_edid_info.dc_420);
	}
	return ret_count;
}

unsigned char get_hpd_interlock(void)
{
	return hpd_interlock;
}

static ssize_t hpd_interlock_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	ssize_t ret_count = 0;

	ret_count = sprintf(buf, "%s\n", hpd_interlock?"Yes":"No");

	return ret_count;
}

static ssize_t hpd_interlock_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{

	if (sysfs_streq(buf, "YES") || sysfs_streq(buf, "Yes") ||
		sysfs_streq(buf, "yes"))
		hpd_interlock = 1;
	else if (sysfs_streq(buf, "NO") || sysfs_streq(buf, "No") ||
		sysfs_streq(buf, "no"))
		hpd_interlock = 0;

	return size;
}

/* /sys/devices/platform/9800d000.hdmitx/hdmitx_info */
static DEVICE_ATTR(hdmitx_info, S_IRUGO, show_hdmitx_info, NULL);
/* /sys/devices/platform/9800d000.hdmitx/edid_info */
static DEVICE_ATTR(edid_info, S_IRUGO, show_edid_info, NULL);
/* /sys/devices/platform/9800d000.hdmitx/hpd_interlock */
static DEVICE_ATTR(hpd_interlock, 0644,
	hpd_interlock_show, hpd_interlock_store);

void register_hdmitx_sysfs(struct device *dev)
{
	device_create_file(dev, &dev_attr_hdmitx_info);
	device_create_file(dev, &dev_attr_edid_info);
	device_create_file(dev, &dev_attr_hpd_interlock);
}

