/*
 * mipi_wrapper.c - RTK hdmi rx driver
 *
 * Copyright (C) 2017 Realtek Semiconductor Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include "v4l2_hdmi_dev.h"
#include "mipi_wrapper.h"
#include "hdmirx_reg.h"

#include "rx_drv/hdmiInternal.h"

MIPI_TOP_INFO mipi_top;

/*=================== extern Variable/Function ===================*/
extern HDMI_INFO_T hdmi;
extern int hdmi_stream_on;

extern HDMI_ERR_T Hdmi_CheckConditionChange(void);
extern void hdmirx_wrapper_isr(void);
extern void set_hdmirx_wrapper_control_0(int fifo_stage,int fw_dma_en,int polarity_set_en,int polarity_set,int yuv_fmt,int hdmirx_en);
extern void restartHdmiRxWrapperDetection(void);
#ifdef CONFIG_RTK_HDCPRX_2P2
extern void  Hdmi_HDCP_2_2_msg_hander(void);
#endif
extern void start_hdcp1x_repeater_work(void);
/*======================================================*/

extern struct ion_device *rtk_phoenix_ion_device;

const struct MIPI_COLOR_COEFF cs_coeff[COLOR_COEFF_MAX] = {
	/* Y2R_REC709_FULL, 10bit Rec.709 Full range (0~1023) YUV2RGB (0~1023) */
	{.c1 = 0x04000, .c2 = 0x00000, .c3 = 0x064CA,
	 .c4 = 0x04000, .c5 = 0x1F402, .c6 = 0x1E20A,
	 .c7 = 0x04000, .c8 = 0x076C3, .c9 = 0x00000,
	 .k1 = 0xF366, .k2 = 0x053E, .k3 = 0xF127},
	/* Y2R_REC709_LIMITED, 10bit Rec.709 Limited range (64~960) YUV2RGB (0~1023) */
	{.c1 = 0x04A85, .c2 = 0x00000, .c3 = 0x072BC,
	 .c4 = 0x04A85, .c5 = 0x1F529, .c6 = 0x1DDE4,
	 .c7 = 0x04A85, .c8 = 0x08732, .c9 = 0x00000,
	 .k1 = 0xF07D, .k2 = 0x04CE, .k3 = 0xEDEF},
	/* Y2R_REC601_FULL */
	{.c1 = 0x04000, .c2 = 0x00000, .c3 = 0x059BA,
	 .c4 = 0x04000, .c5 = 0x1E9F9, .c6 = 0x1D24B,
	 .c7 = 0x04000, .c8 = 0x07168, .c9 = 0x00000,
	 .k1 = 0xFBFF, .k2 = 0x021E, .k3 = 0xFC74},
	/* Y2R_REC601_LIMITED */
	{.c1 = 0x04A85, .c2 = 0x00000, .c3 = 0x06625,
	 .c4 = 0x04A85, .c5 = 0x1E6EC, .c6 = 0x1CBF7,
	 .c7 = 0x04A85, .c8 = 0x0811A, .c9 = 0x00000,
	 .k1 = 0xEC9E, .k2 = 0x0879, .k3 = 0xEEB2},
};

const unsigned long scale_ratio8[] =
{
	0x3800, 0x4800, 0x5000, 0x5800, 0x6000, 0x6800, 0x7000, 0x7800,
	0x8000, 0x8800, 0x9000, 0x9800, 0xa000, 0xa800, 0xb000, 0xb800,
	0xc000, 0xc800, 0xd000, 0xd800, 0xe000, 0xe800, 0xf000, 0xf800,
	0x12000, 0x14000, 0x16000, 0x18000, 0x1a000, 0x1c000, 0x1e000, 0x20000
};

/* 4taps-- Vertical sampling */
const short coef_4t8p_ratio8[][4][4] =
{
	/* for all upscale ratio */
	{
		{   -8,   256,    16,    -8 },
		{  -24,   240,    48,    -8 },
		{  -28,   212,    88,   -16 },
		{  -28,   176,   132,   -24 },
	},
	/* I=8, D=9, wp= 0.111, ws= 0.122, alpha= 0.100 */
	{
		{  -24,   241,     5,    34 },
		{  -47,   249,    46,     8 },
		{  -60,   237,    99,   -20 },
		{  -59,   204,   155,   -44 },
	},
	/* I=8, D=10, wp= 0.100, ws= 0.110, alpha= 0.100 */
	{
		{   -1,   222,    28,     7 },
		{  -26,   230,    67,   -15 },
		{  -44,   223,   113,   -36 },
		{  -53,   198,   160,   -49 },
	},
	/* I=8, D=11, wp= 0.091, ws= 0.100, alpha= 0.100 */
	{
		{   22,   208,    47,   -21 },
		{   -4,   212,    82,   -34 },
		{  -25,   204,   120,   -43 },
		{  -40,   186,   156,   -46 },
	},
	/* I=8, D=12, wp= 0.083, ws= 0.092, alpha= 0.100 */
	{
		{   36,   198,    62,   -40 },
		{   12,   197,    92,   -45 },
		{   -9,   188,   122,   -45 },
		{  -27,   172,   150,   -39 },
	},
	/* I=8, D=13, wp= 0.077, ws= 0.085, alpha= 0.100 */
	{
		{   47,   190,    72,   -53 },
		{   24,   186,    97,   -51 },
		{    2,   177,   121,   -44 },
		{  -17,   163,   143,   -33 },
	},
	/* I=8, D=14, wp= 0.071, ws= 0.079, alpha= 0.100 */
	{
		{   55,   184,    78,   -61 },
		{   32,   176,   100,   -52 },
		{   10,   167,   120,   -41 },
		{   -9,   154,   138,   -27 },
	},
	/* I=8, D=15, wp= 0.067, ws= 0.073, alpha= 0.100 */
	{
		{   61,   178,    82,   -65 },
		{   38,   169,   101,   -52 },
		{   16,   160,   118,   -38 },
		{   -3,   146,   134,   -21 },
	},
	/* I=8, D=16, wp= 0.063, ws= 0.069, alpha= 0.100 */
	{
		{   65,   172,    85,   -66 },
		{   42,   163,   101,   -50 },
		{   21,   153,   116,   -34 },
		{    1,   142,   130,   -17 },
	},
	/* I=8, D=17, wp= 0.059, ws= 0.065, alpha= 0.100 */
	{
		{   67,   167,    87,   -65 },
		{   45,   158,   101,   -48 },
		{   25,   147,   114,   -30 },
		{    6,   137,   126,   -13 },
	},
	/* I=8, D=18, wp= 0.056, ws= 0.061, alpha= 0.100 */
	{
		{   69,   162,    88,   -63 },
		{   48,   152,   100,   -44 },
		{   28,   142,   112,   -26 },
		{   10,   133,   122,    -9 },
	},
	/* I=8, D=19, wp= 0.053, ws= 0.058, alpha= 0.100 */
	{
		{   71,   156,    88,   -59 },
		{   50,   147,    99,   -40 },
		{   31,   137,   110,   -22 },
		{   13,   129,   119,    -5 },
	},
	/* I=8, D=20, wp= 0.050, ws= 0.055, alpha= 0.100 */
	{
		{   72,   151,    88,   -55 },
		{   52,   142,    98,   -36 },
		{   34,   133,   107,   -18 },
		{   16,   125,   116,    -1 },
	},
	/* I=8, D=21, wp= 0.048, ws= 0.052, alpha= 0.100 */
	{
		{   72,   147,    87,   -50 },
		{   53,   138,    96,   -31 },
		{   36,   128,   105,   -13 },
		{   19,   121,   113,     3 },
	},
	/* I=8, D=22, wp= 0.045, ws= 0.050, alpha= 0.100 */
	{
		{   73,   140,    87,   -44 },
		{   55,   132,    95,   -26 },
		{   38,   125,   102,    -9 },
		{   23,   116,   110,     7 },
	},
	/* I=8, D=23, wp= 0.043, ws= 0.048, alpha= 0.100 */
	{
		{   73,   135,    86,   -38 },
		{   56,   127,    93,   -20 },
		{   40,   120,   100,    -4 },
		{   25,   113,   107,    11 },
	},
	/* I=8, D=24, wp= 0.042, ws= 0.046, alpha= 0.100 */
	{
		{   73,   129,    85,   -31 },
		{   57,   123,    91,   -15 },
		{   42,   117,    97,     0 },
		{   28,   109,   104,    15 },
	},
	/* I=8, D=25, wp= 0.040, ws= 0.044, alpha= 0.100 */
	{
		{   72,   125,    83,   -24 },
		{   58,   118,    89,    -9 },
		{   44,   112,    95,     5 },
		{   31,   106,   101,    18 },
	},
	/* I=8, D=26, wp= 0.038, ws= 0.042, alpha= 0.100 */
	{
		{   72,   120,    82,   -18 },
		{   59,   114,    87,    -4 },
		{   46,   108,    93,     9 },
		{   34,   102,    98,    22 },
	},
	/* I=8, D=27, wp= 0.037, ws= 0.041, alpha= 0.100 */
	{
		{   72,   114,    81,   -11 },
		{   59,   109,    86,     2 },
		{   48,   104,    90,    14 },
		{   36,   100,    95,    25 },
	},
	/* I=8, D=28, wp= 0.036, ws= 0.039, alpha= 0.100 */
	{
		{   71,   111,    79,    -5 },
		{   60,   105,    84,     7 },
		{   49,   101,    88,    18 },
		{   39,    97,    92,    28 },
	},
	/* I=8, D=29, wp= 0.034, ws= 0.038, alpha= 0.100 */
	{
		{   71,   106,    78,     1 },
		{   60,   102,    82,    12 },
		{   51,    97,    86,    22 },
		{   41,    94,    90,    31 },
	},
	/* I=8, D=30, wp= 0.033, ws= 0.037, alpha= 0.100 */
	{
		{   70,   103,    77,     6 },
		{   61,    98,    81,    16 },
		{   52,    95,    84,    25 },
		{   43,    91,    88,    34 },
	},
	/* I=8, D=31, wp= 0.032, ws= 0.035, alpha= 0.100 */
	{
		{   70,    99,    76,    11 },
		{   61,    96,    79,    20 },
		{   53,    92,    82,    29 },
		{   45,    88,    86,    37 },
	},
	/* I=8, D=36, wp= 0.028, ws= 0.031, alpha= 0.100 */
	{
		{   68,    86,    72,    30 },
		{   62,    84,    74,    36 },
		{   57,    82,    76,    41 },
		{   52,    79,    78,    47 },
	},
	/* I=8, D=40, wp= 0.025, ws= 0.028, alpha= 0.100 */
	{
		{   67,    79,    70,    40 },
		{   63,    78,    71,    44 },
		{   59,    76,    73,    48 },
		{   55,    76,    74,    51 },
	},
	/* I=8, D=44, wp= 0.023, ws= 0.025, alpha= 0.100 */
	{
		{   66,    76,    68,    46 },
		{   63,    75,    69,    49 },
		{   60,    74,    70,    52 },
		{   57,    72,    72,    55 },
	},
	/* I=8, D=48, wp= 0.021, ws= 0.023, alpha= 0.100 */
	{
		{   66,    73,    67,    50 },
		{   63,    73,    68,    52 },
		{   61,    72,    69,    54 },
		{   59,    70,    70,    57 },
	},
	/* I=8, D=52, wp= 0.019, ws= 0.021, alpha= 0.100 */
	{
		{   65,    71,    67,    53 },
		{   64,    70,    67,    55 },
		{   62,    70,    68,    56 },
		{   60,    69,    69,    58 },
	},
	/* I=8, D=56, wp= 0.018, ws= 0.020, alpha= 0.100 */
	{
		{   65,    70,    66,    55 },
		{   64,    69,    67,    56 },
		{   62,    69,    67,    58 },
		{   61,    68,    68,    59 },
	},
	/* I=8, D=60, wp= 0.017, ws= 0.018, alpha= 0.100 */
	{
		{   65,    69,    66,    56 },
		{   64,    68,    66,    58 },
		{   63,    67,    67,    59 },
		{   61,    68,    67,    60 },
	},
	/* I=8, D=64, wp= 0.016, ws= 0.017, alpha= 0.100 */
	{
		{   65,    67,    66,    58 },
		{   64,    67,    66,    59 },
		{   63,    67,    66,    60 },
		{   62,    66,    67,    61 },
	},
};

int get_ratio8_index(unsigned int delta)
{
	int i;
	int idx;

	if (delta < 0x4000 ) { /* upscaling */
		idx = 0;
		goto exit;
	}

	/*
	* VscaleRatio8 is incrementing, we find the first one that's bigger
	* than or equal to delta instead of min ads diff.
	* This way it will round up to lower pass filter to avoid alias
	*/
	for (i = 1; i < sizeof(scale_ratio8)/sizeof(unsigned int); i++){
		if (scale_ratio8[i] >= delta ) {
			idx = i;
			goto exit;
		}
	}

	/* last index */
	idx = sizeof(scale_ratio8)/sizeof(unsigned int) - 1;

exit:
	return idx;
}

void set_scaling_coeffs (unsigned int *coeff, int delta)
{
	int i;
	int x;
	int y;
	int idx;
	int taps;
	short const *p ;

	taps = 4;
	idx = get_ratio8_index(delta);
	p = coef_4t8p_ratio8[idx][0];

	HDMIRX_INFO("%s idx=%d", __func__, idx);

	for (i = 0; i < (taps << 2); i++) {
		x = i &  7;
		y = i >> 3;

		coeff[i] =
			(x < 4 ? p[x * taps + taps-1-y] : p[(7-x) * taps + y]) << 4;
	}
}

void register_mipi_ion(struct v4l2_hdmi_dev *dev)
{
	struct mipi_ion_buf *ion_buf;
	unsigned long size;
	size_t len;
	int ret_val;

	size = dev->width * dev->height * dev->bpp/8;
	/* Increase 1/8 size for prevent MIPI write out of range */
	size = roundup(size*9/8, 4096);

	ion_buf = &dev->ion_buf;
	ion_buf->client = ion_client_create(rtk_phoenix_ion_device, "hdmirx");

	ion_buf->handle = ion_alloc(ion_buf->client, size, 4096,
		RTK_PHOENIX_ION_HEAP_MEDIA_MASK,
		ION_FLAG_NONCACHED | ION_FLAG_HWIPACC);

	if (IS_ERR(ion_buf->handle))
		HDMIRX_ERROR("%s ion_alloc fail", __func__);

	ret_val = ion_phys(ion_buf->client, ion_buf->handle, &ion_buf->phys_addr, &len);
	if (ret_val != 0)
		HDMIRX_ERROR("%s ion_phys fail", __func__);

	HDMIRX_INFO("ion_addr=0x%08lx size=%zd", ion_buf->phys_addr, len);
}

void deregister_mipi_ion(struct v4l2_hdmi_dev *dev)
{
	struct mipi_ion_buf *ion_buf;

	ion_buf = &dev->ion_buf;

	if (ion_buf->client != NULL) {
		HDMIRX_INFO("%s", __func__);
		ion_free(ion_buf->client, ion_buf->handle);
		ion_buf->client = NULL;
	}
}

void mipi_reset_work_func(struct work_struct *work)
{
	struct reset_control *reset_mipi;

	reset_mipi = rstc_get("rstn_mipi");
	reset_control_assert(reset_mipi);
	reset_control_deassert(reset_mipi);
}

void stop_mipi_process(void)
{
	set_enable_mipi(0);
	set_hdmirx_wrapper_control_0(-1, 0,-1,-1,-1,-1);//Stop DMA
	schedule_work(&mipi_top.mipi_reset_work);//Reset mipi clock

    hdmi_stream_on = 0;
    mipi_top.mipi_init = 0;
}

void set_mipi_env(void)
{
	HDMIRX_DEBUG("[%s]",__FUNCTION__);

	// on 1195
	hdmi_rx_reg_write32(MIPI_APHY_REG15, MIPI_APHY_REG15_SEL_DIV_RG(DEMUX16), HDMI_RX_MIPI_PHY);//SEL_DIV_RG
	hdmi_rx_reg_write32(MIPI_DPHY_REG4, MIPI_DPHY_REG4_SL_RG(fourlane) | MIPI_DPHY_REG4_Sw_rst(disable), HDMI_RX_MIPI_PHY);//Lane mux sel & soft reset

	// 4 lane
	hdmi_rx_reg_write32(MIPI_DPHY_PWDB, MIPI_DPHY_PWDB_L3_pwdb(enable) | MIPI_DPHY_PWDB_L2_pwdb(enable) | 
										MIPI_DPHY_PWDB_L1_pwdb(enable) | MIPI_DPHY_PWDB_L0_pwdb(enable), 
										HDMI_RX_MIPI_PHY);//Lane1 & Lane2 power on

	hdmi_rx_reg_write32(MIPI_DPHY_REG0, MIPI_DPHY_REG0_Div_sel(div_bit_16) | MIPI_DPHY_REG0_Lane3_en(enable) | 
										MIPI_DPHY_REG0_Lane2_en(enable) | MIPI_DPHY_REG0_Lane1_en(enable) | 
										MIPI_DPHY_REG0_Lane0_en(enable), HDMI_RX_MIPI_PHY);//16bit data width, Lane1 & Lane2 enable

	hdmi_rx_reg_write32(MIPI_DPHY_REG9, MIPI_DPHY_REG9_Lane3_clk_edge(posedge) | MIPI_DPHY_REG9_Lane2_clk_edge(posedge) |
										MIPI_DPHY_REG9_Lane1_clk_edge(posedge) | MIPI_DPHY_REG9_Lane0_clk_edge(posedge), HDMI_RX_MIPI_PHY);//Lane CLK edge

	hdmi_rx_reg_write32(MIPI_DPHY_REG1, MIPI_DPHY_REG1_yuv_src_sel(SEL_UYVY) | MIPI_DPHY_REG1_dec_format(YUV422_dec_format), HDMI_RX_MIPI_PHY);// SEL_UYVY //SEL_YUYV & Dec data format for YUV422

	hdmi_rx_reg_write32(MIPI_DPHY_REG2, MIPI_DPHY_REG2_D_type(DTYPE_YUY8), HDMI_RX_MIPI_PHY);//MIPI data type YUV422 8bit

	hdmi_rx_reg_write32(MIPI_DPHY_REG6	, MIPI_DPHY_REG6_Dvld_lane1(0x6) | MIPI_DPHY_REG6_Dvld_lane0(0x6), HDMI_RX_MIPI_PHY); 	   // 0x6
	hdmi_rx_reg_write32(MIPI_DPHY_REG7	, MIPI_DPHY_REG7_Dvld_lane3(0x6) | MIPI_DPHY_REG7_Dvld_lane2(0x6), HDMI_RX_MIPI_PHY);

}

unsigned int rx_pitch_measurement(unsigned int output_h, MIPI_OUT_COLOR_SPACE_T output_color)
{
	unsigned int pitch, pitch_factor;
	HDMIRX_INFO("rx_pitch_measurement,output_color=%x\n",output_color);

	switch(output_color)
	{
		case OUT_8BIT_YUV422:
		case OUT_8BIT_YUV420:
			pitch_factor = 1;
			break;
		case OUT_10BIT_YUV422:
			pitch_factor = 2;
			break;
		default: //all other RGB case
			pitch_factor = 4;
			break;
	}

	pitch = output_h * pitch_factor;
	// pitch has to be n*16 byte
	if(output_color < OUT_ARGB)
		pitch = roundup16(pitch);
	return pitch;
}

void set_video_DDR_start_addr(struct v4l2_hdmi_dev *dev)
{
	unsigned int addry;
	unsigned int addruv;
	unsigned int offset;
	unsigned long flags = 0;
	struct hdmi_dmaqueue *hdmidq;
	struct mipi_ion_buf *ion_buf;

	hdmidq = &dev->hdmidq;
	ion_buf = &dev->ion_buf;

	offset = roundup16(mipi_top.pitch) * roundup16(mipi_top.v_output_len);

	spin_lock_irqsave(&dev->slock, flags);
	hdmidq->skip_frame[0] = 1;
	hdmidq->skip_frame[1] = 1;

	addry = ion_buf->phys_addr;
	addruv = ion_buf->phys_addr + offset;
	set_video_dest_addr(addry, addruv, addry, addruv);
	spin_unlock_irqrestore(&dev->slock, flags);

	HDMIRX_INFO("[%s] addr1y(0x%08x) addr1uv(0x%08x) addr2y(0x%08x) addr2uv(0x%08x)",
		__func__, addry, addruv, addry, addruv);
}

void set_mipi_type(unsigned int type)
{
	unsigned int reg_val;

	HDMIRX_DEBUG("[%s] type(%u)",__FUNCTION__,type);

	reg_val = hdmi_rx_reg_read32(MIPI_TYPE, HDMI_RX_MIPI);

	reg_val = reg_val & (~MIPI_TYPE_mipi_type_mask);

	hdmi_rx_reg_write32(MIPI_TYPE, reg_val | MIPI_TYPE_mipi_type(type), HDMI_RX_MIPI);
}

void set_pic_dest_addr(int addry, int addruv)
{
	if(addry >= 0)
	{
		hdmi_rx_reg_write32(MIPI_SA2, addry, HDMI_RX_MIPI);
	}
	if(addruv >= 0)
	{
		hdmi_rx_reg_write32(MIPI_SA2_UV, addruv, HDMI_RX_MIPI);
	}
}

void set_pic_dest_size(unsigned int dst_width, unsigned int pitch)
{

	hdmi_rx_reg_write32(MIPI_SIZE1, MIPI_SIZE1_dst_width_pic(dst_width) | MIPI_SIZE1_pitch_pic(pitch), HDMI_RX_MIPI);
}

void set_video_dest_addr(int addr1y, int addr1uv, int addr2y, int addr2uv)
{
	if(addr1y >= 0)
		hdmi_rx_reg_write32(MIPI_SA0, addr1y, HDMI_RX_MIPI);

	if(addr1uv >= 0)
		hdmi_rx_reg_write32(MIPI_SA0_UV, addr1uv, HDMI_RX_MIPI);

	if(addr2y >= 0)
		hdmi_rx_reg_write32(MIPI_SA1, addr2y, HDMI_RX_MIPI);

	if(addr2uv >= 0)
		hdmi_rx_reg_write32(MIPI_SA1_UV, addr2uv, HDMI_RX_MIPI);
}

void set_video_dest_size(unsigned int dst_width, unsigned int pitch)
{

	hdmi_rx_reg_write32(MIPI_SIZE0, MIPI_SIZE0_dst_width_video(dst_width) | MIPI_SIZE0_pitch_video(pitch), HDMI_RX_MIPI);
}

void set_video_src_size(unsigned int src_width)
{
	unsigned int reg_val;

	reg_val = hdmi_rx_reg_read32(MIPI_SIZE2, HDMI_RX_MIPI);
	reg_val = reg_val & (~MIPI_SIZE2_src_width_video_mask);
	hdmi_rx_reg_write32(MIPI_SIZE2, reg_val | MIPI_SIZE2_src_width_video(src_width), HDMI_RX_MIPI);
}

void set_init_mipi_value(MIPI_REG *mipi_reg)
{
	unsigned int reg_val = 0;

	reg_val = (mipi_reg->dst_fmt << 27) |
					(mipi_reg->src_fmt << 25) |
					(mipi_reg->src_sel << 24) |
					(mipi_reg->yuv420_uv_seq << 23) |
					(mipi_reg->vs << 22) |
					(mipi_reg->vs_near << 21) |
					(mipi_reg->vs_yodd << 20) |
					(mipi_reg->vs_codd << 19) |
					(mipi_reg->hs << 18) |
					(mipi_reg->hs_yodd << 17) |
					(mipi_reg->hs_codd << 16) |
					(mipi_reg->yuv420_fmt << 15) |
					(mipi_reg->ccs_data_format << 13) |
					(mipi_reg->yuv_to_rgb << 12) |
					(mipi_reg->chroma_ds_mode << 11) |
					(mipi_reg->chroma_ds_en << 10) |
					(mipi_reg->chroma_us_mode << 9) |
					(mipi_reg->chroma_us_en << 8) |
					(mipi_reg->hdmirx_interlace_en << 7) |
					(mipi_reg->hdmirx_interlace_polarity << 6) |
					(mipi_reg->int_en4 << 5) |
					(mipi_reg->int_en3 << 4) |
					(mipi_reg->int_en2 << 3) |
					(mipi_reg->int_en1 << 2) |
					(mipi_reg->int_en0 << 1) |
					(mipi_reg->en);

	hdmi_rx_reg_write32(MIPI, reg_val, HDMI_RX_MIPI);
	HDMIRX_INFO("Set MIPI=0x%08x",reg_val);
	HDMIRX_INFO("Read MIPI=0x%08x",hdmi_rx_reg_read32(MIPI, HDMI_RX_MIPI));
}

void set_enable_mipi(unsigned char enable)
{
	unsigned int reg_val;

	hdmi_rx_reg_mask32(MIPI, ~(MIPI_int_en0_mask|MIPI_int_en1_mask|MIPI_int_en2_mask|MIPI_int_en3_mask|MIPI_int_en4_mask|MIPI_en_mask),
							MIPI_int_en0(enable)|MIPI_int_en1(enable)|MIPI_int_en2(enable)|MIPI_int_en3(enable)|MIPI_int_en4(enable)|MIPI_en(enable),
							HDMI_RX_MIPI);
	if (enable == 0) {
		/* Clear interrupt */
		reg_val = hdmi_rx_reg_read32(MIPI_INT_ST, HDMI_RX_MIPI);
		hdmi_rx_reg_write32(MIPI_INT_ST, reg_val, HDMI_RX_MIPI);
	}
}

void set_hs_scaler(unsigned int hsi_offset, unsigned int hsi_phase, unsigned int hsd_out, unsigned int hsd_delta)
{
	hdmi_rx_reg_write32(SCALER_HSI, SCALER_HSI_hsi_offset(hsi_offset) | 
									SCALER_HSI_hsi_phase(hsi_phase), HDMI_RX_MIPI);

	hdmi_rx_reg_write32(SCALER_HSD, SCALER_HSD_hsd_out(hsd_out) |
									SCALER_HSD_hsd_delta(hsd_delta), HDMI_RX_MIPI);

	HDMIRX_INFO("hsd_out=%u,hsd_delta=0x%x", hsd_out, hsd_delta);
}

void set_hs_coeff(int delta)
{
	unsigned int c[16];

	set_scaling_coeffs(c, delta);

	/* for Y */
	hdmi_rx_reg_write32(SCALER_HSYNC0,
		SCALER_HSYNC0_hsync0_c1(c[1]) |
		SCALER_HSYNC0_hsync0_c0(c[0]), HDMI_RX_MIPI);
	hdmi_rx_reg_write32(SCALER_HSYNC1,
		SCALER_HSYNC1_hsync1_c1(c[3]) |
		SCALER_HSYNC1_hsync1_c0(c[2]), HDMI_RX_MIPI);
	hdmi_rx_reg_write32(SCALER_HSYNC2,
		SCALER_HSYNC2_hsync2_c1(c[5]) |
		SCALER_HSYNC2_hsync2_c0(c[4]), HDMI_RX_MIPI);
	hdmi_rx_reg_write32(SCALER_HSYNC3,
		SCALER_HSYNC3_hsync3_c1(c[7]) |
		SCALER_HSYNC3_hsync3_c0(c[6]), HDMI_RX_MIPI);
	hdmi_rx_reg_write32(SCALER_HSYNC4,
		SCALER_HSYNC4_hsync4_c1(c[9]) |
		SCALER_HSYNC4_hsync4_c0(c[8]), HDMI_RX_MIPI);
	hdmi_rx_reg_write32(SCALER_HSYNC5,
		SCALER_HSYNC5_hsync5_c1(c[11]) |
		SCALER_HSYNC5_hsync5_c0(c[10]), HDMI_RX_MIPI);
	hdmi_rx_reg_write32(SCALER_HSYNC6,
		SCALER_HSYNC6_hsync6_c1(c[13]) |
		SCALER_HSYNC6_hsync6_c0(c[12]), HDMI_RX_MIPI);
	hdmi_rx_reg_write32(SCALER_HSYNC7,
		SCALER_HSYNC7_hsync7_c1(c[15]) |
		SCALER_HSYNC7_hsync7_c0(c[14]), HDMI_RX_MIPI);

	/* for U,V */
	hdmi_rx_reg_write32(SCALER_HSCC0,
		SCALER_HSCC0_hscc0_c1(c[1]) |
		SCALER_HSCC0_hscc0_c0(c[0]), HDMI_RX_MIPI);
	hdmi_rx_reg_write32(SCALER_HSCC1,
		SCALER_HSCC1_hscc1_c1(c[3]) |
		SCALER_HSCC1_hscc1_c0(c[2]), HDMI_RX_MIPI);
	hdmi_rx_reg_write32(SCALER_HSCC2,
		SCALER_HSCC2_hscc2_c1(c[5]) |
		SCALER_HSCC2_hscc2_c0(c[4]), HDMI_RX_MIPI);
	hdmi_rx_reg_write32(SCALER_HSCC3,
		SCALER_HSCC3_hscc3_c1(c[7]) |
		SCALER_HSCC3_hscc3_c0(c[6]), HDMI_RX_MIPI);
	hdmi_rx_reg_write32(SCALER_HSCC4,
		SCALER_HSCC4_hscc4_c1(c[9]) |
		SCALER_HSCC4_hscc4_c0(c[8]), HDMI_RX_MIPI);
	hdmi_rx_reg_write32(SCALER_HSCC5,
		SCALER_HSCC5_hscc5_c1(c[11]) |
		SCALER_HSCC5_hscc5_c0(c[10]), HDMI_RX_MIPI);
	hdmi_rx_reg_write32(SCALER_HSCC6,
		SCALER_HSCC6_hscc6_c1(c[13]) |
		SCALER_HSCC6_hscc6_c0(c[12]), HDMI_RX_MIPI);
	hdmi_rx_reg_write32(SCALER_HSCC7,
		SCALER_HSCC7_hscc7_c1(c[15]) |
		SCALER_HSCC7_hscc7_c0(c[14]), HDMI_RX_MIPI);
}

void set_vs_scaler(unsigned int vsi_offset, unsigned int vsi_phase, unsigned int vsd_out, unsigned int vsd_delta)
{
	hdmi_rx_reg_write32(SCALER_VSI, SCALER_VSI_vsi_offset(vsi_offset) | 
									SCALER_VSI_vsi_phase(vsi_phase), HDMI_RX_MIPI);

	hdmi_rx_reg_write32(SCALER_VSD, SCALER_VSD_vsd_out(vsd_out) |
									SCALER_VSD_vsd_delta(vsd_delta), HDMI_RX_MIPI);

	HDMIRX_INFO("vsd_out=%u,vsd_delta=0x%x\n", vsd_out, vsd_delta);
}

void set_vs_coeff(void)
{
	unsigned int c0;
	unsigned int c1;
	unsigned int c2;
	unsigned int c3;

	c0 = 0x02e80203;
	c1 = 0x06d604a5;
	c2 = 0x0b5b092a;
	c3 = 0x0dfd0d18;

	hdmi_rx_reg_write32(SCALER_VSYC0, c0, HDMI_RX_MIPI);
	hdmi_rx_reg_write32(SCALER_VSYC1, c1, HDMI_RX_MIPI);
	hdmi_rx_reg_write32(SCALER_VSYC2, c2, HDMI_RX_MIPI);
	hdmi_rx_reg_write32(SCALER_VSYC3, c3, HDMI_RX_MIPI);

	hdmi_rx_reg_write32(SCALER_VSCC0, c0, HDMI_RX_MIPI);
	hdmi_rx_reg_write32(SCALER_VSCC1, c1, HDMI_RX_MIPI);
	hdmi_rx_reg_write32(SCALER_VSCC2, c2, HDMI_RX_MIPI);
	hdmi_rx_reg_write32(SCALER_VSCC3, c3, HDMI_RX_MIPI);
}

void set_alpha(unsigned int alpha)
{

	hdmi_rx_reg_write32(CONSTANT_ALPHA, CONSTANT_ALPHA_alpha(alpha), HDMI_RX_MIPI);
}

void set_YUV2RGB_coeff(void)
{
	unsigned char coeff_index = Y2R_REC709_FULL;

	/* C1~C3 */
	hdmi_rx_reg_write32(CS_TRANS0,	cs_coeff[coeff_index].c1, HDMI_RX_MIPI);
	hdmi_rx_reg_write32(CS_TRANS1,	cs_coeff[coeff_index].c2, HDMI_RX_MIPI);
	hdmi_rx_reg_write32(CS_TRANS2,	cs_coeff[coeff_index].c3, HDMI_RX_MIPI);
	/* C4~C6 */
	hdmi_rx_reg_write32(CS_TRANS3,	cs_coeff[coeff_index].c4, HDMI_RX_MIPI);
	hdmi_rx_reg_write32(CS_TRANS4,	cs_coeff[coeff_index].c5, HDMI_RX_MIPI);
	hdmi_rx_reg_write32(CS_TRANS5,	cs_coeff[coeff_index].c6, HDMI_RX_MIPI);
	/* C7~C9 */
	hdmi_rx_reg_write32(CS_TRANS6,	cs_coeff[coeff_index].c7, HDMI_RX_MIPI);
	hdmi_rx_reg_write32(CS_TRANS7,	cs_coeff[coeff_index].c8, HDMI_RX_MIPI);
	hdmi_rx_reg_write32(CS_TRANS8,	cs_coeff[coeff_index].c9, HDMI_RX_MIPI);
	/* K1~K3*/
	hdmi_rx_reg_write32(CS_TRANS9,	cs_coeff[coeff_index].k1, HDMI_RX_MIPI);
	hdmi_rx_reg_write32(CS_TRANS10, cs_coeff[coeff_index].k2, HDMI_RX_MIPI);
	hdmi_rx_reg_write32(CS_TRANS11, cs_coeff[coeff_index].k3, HDMI_RX_MIPI);

}

void set_RGB2YUV_coeff(void)
{
	//C1~C3
	hdmi_rx_reg_write32(CS_TRANS0,	0x00BB8, HDMI_RX_MIPI);
	hdmi_rx_reg_write32(CS_TRANS1,	0x02748, HDMI_RX_MIPI);
	hdmi_rx_reg_write32(CS_TRANS2,	0x003F8, HDMI_RX_MIPI);
	//C4~C6
	hdmi_rx_reg_write32(CS_TRANS3,	0x1F988, HDMI_RX_MIPI);
	hdmi_rx_reg_write32(CS_TRANS4,	0x1EA60, HDMI_RX_MIPI);
	hdmi_rx_reg_write32(CS_TRANS5,	0x01C18, HDMI_RX_MIPI);
	//C7~C9
	hdmi_rx_reg_write32(CS_TRANS6,	0x01C18, HDMI_RX_MIPI);
	hdmi_rx_reg_write32(CS_TRANS7,	0x1E6E9, HDMI_RX_MIPI);
	hdmi_rx_reg_write32(CS_TRANS8,	0x1FD7B, HDMI_RX_MIPI);
	//K1~K3
	hdmi_rx_reg_write32(CS_TRANS9,	0x00100, HDMI_RX_MIPI);
	hdmi_rx_reg_write32(CS_TRANS10, 0x00800, HDMI_RX_MIPI);
	hdmi_rx_reg_write32(CS_TRANS11, 0x00800, HDMI_RX_MIPI);
}

void mipi_scale_down(unsigned int src_width,unsigned int src_height,unsigned int dst_width,unsigned int dst_height)
{
	unsigned int delta_num, delta_den, offset, phase;

	if(src_width >= dst_width)
	{
		//set hs_scaler
		offset = 0;
		phase = 0;
		delta_num = (src_width / dst_width) << 14;
		delta_den = ((src_width % dst_width)*0x4000) / dst_width ;
		set_hs_scaler(offset,phase,dst_width, (delta_num | delta_den));//offset,phase,out,delta
		set_hs_coeff(delta_num);
	}
	if(src_height >= dst_height)
	{
		//set vs_scaler
		offset = 0;
		phase = 0;
		delta_num = (src_height / dst_height) << 14;
		delta_den = ((src_height % dst_height)*0x4000) / dst_height ;
		set_vs_scaler(offset,phase,dst_height, (delta_num | delta_den));//offset,phase,out,delta
		set_vs_coeff();
	}
}

void setup_mipi(void)
{
	MIPI_REG mipi_reg;
	unsigned int v_input,h_input;
	HDMIRX_INFO("[%s]",__FUNCTION__);

	set_mipi_env();
	set_mipi_type(0);//0:video, 1:pic

	memset(&mipi_reg, 0, sizeof(MIPI_REG));

	mipi_reg.dst_fmt = mipi_top.output_color;
	mipi_reg.src_fmt = mipi_top.input_color;
	mipi_reg.src_sel = mipi_top.src_sel;
	mipi_reg.yuv420_uv_seq = mipi_top.uv_seq;

	if(!hdmi.tx_timing.progressive)//Interlace
		v_input = mipi_top.v_input_len*2;
	else
		v_input = mipi_top.v_input_len;

	h_input = mipi_top.h_input_len;

	if(v_input > mipi_top.v_output_len)
		mipi_reg.vs = 1;

	if(h_input > mipi_top.h_output_len)
		mipi_reg.hs = 1;

	if(mipi_reg.dst_fmt == OUT_10BIT_YUV422)
		mipi_reg.yuv420_fmt=1;

	//mipi_reg.ccs_data_format

	if((mipi_top.input_color!=IN_RGB888)&&(mipi_top.output_color>=OUT_ARGB))//YUV -> RGB
	{
		mipi_reg.yuv_to_rgb = 1;
		set_alpha(0xff);
		set_YUV2RGB_coeff();
	}
	else if((mipi_top.input_color==IN_RGB888)&&(mipi_top.output_color<=OUT_10BIT_YUV420))//RGB -> YUV
	{
		mipi_reg.chroma_ds_mode = 1;//0:drop 1:avg
		mipi_reg.chroma_ds_en = 1;
		mipi_reg.yuv_to_rgb = 1;
		set_RGB2YUV_coeff();
	}

	if((mipi_top.input_color == IN_YUV444)&&(mipi_top.output_color <= OUT_10BIT_YUV420))
	{
		mipi_reg.chroma_ds_mode = 1;//0:drop 1:avg
		mipi_reg.chroma_ds_en = 1;
	}

	if((mipi_top.input_color == IN_YUV422)&&(mipi_reg.vs || mipi_reg.hs))
	{
		mipi_reg.chroma_us_mode = 0;//0:repeat 1:avg
		mipi_reg.chroma_us_en = 1;
		if(mipi_top.output_color <= OUT_10BIT_YUV420)
		{
			mipi_reg.chroma_ds_mode = 1;//0:drop 1:avg
			mipi_reg.chroma_ds_en = 1;
		}
	}

	mipi_reg.hdmirx_interlace_en = (!hdmi.tx_timing.progressive);
	mipi_reg.hdmirx_interlace_polarity = 1;// 0: Button field first, 1: Top field first

	mipi_reg.int_en4 = 1;
	mipi_reg.int_en3 = 1;
	mipi_reg.int_en2 = 1;
	mipi_reg.int_en1 = 1;
	mipi_reg.int_en0 = 1;

	mipi_reg.en = 1;

	set_video_dest_size(mipi_top.h_output_len,mipi_top.pitch);
	set_video_src_size(mipi_top.h_input_len);
	mipi_scale_down(h_input,v_input,mipi_top.h_output_len,mipi_top.v_output_len);

	hdmi_rx_reg_write32(MIPI_DPHY_REG11, MIPI_DPHY_REG11_Esc_lane3_int_flg(1)|
										MIPI_DPHY_REG11_Esc_lane2_int_flg(1)|
										MIPI_DPHY_REG11_Esc_lane1_int_flg(1)|
           								MIPI_DPHY_REG11_Esc_lane0_int_flg(1)|
           								MIPI_DPHY_REG11_Clk_ulps_int_flg(1)|
           								MIPI_DPHY_REG11_Crc16_err_int_flg(1)|
           								MIPI_DPHY_REG11_Ecc_crt_int_flg(1)|
           								MIPI_DPHY_REG11_Ecc_err_int_flg(1), HDMI_RX_MIPI_PHY);

	set_init_mipi_value(&mipi_reg);
	set_hdmirx_wrapper_control_0(-1, 1, mipi_reg.hdmirx_interlace_en, mipi_reg.hdmirx_interlace_polarity,-1,-1);
	mipi_top.mipi_init = 1;
}

void update_mipi_hw_buffer(int src_index, struct v4l2_hdmi_dev *dev, int buffer_empty)
{
	unsigned long flags;
	unsigned int addry;
	unsigned int addruv;
	unsigned int offset;
	struct hdmi_dmaqueue *hdmidq = &dev->hdmidq;

	offset = roundup16(mipi_top.pitch) * roundup16(mipi_top.v_output_len);

	spin_lock_irqsave(&dev->slock, flags);

	if (hdmidq->skip_frame[src_index] == 0) {
		vb2_buffer_done(&hdmidq->hwbuf[src_index]->vb,VB2_BUF_STATE_DONE);
		atomic_inc(&hdmidq->rcnt);
	}

	if (buffer_empty == 0) {
		hdmidq->hwbuf[src_index] = list_entry(hdmidq->active.next, struct hdmi_buffer, list);
		list_del(&hdmidq->hwbuf[src_index]->list);
		atomic_dec(&hdmidq->qcnt);
		hdmidq->skip_frame[src_index] = 0;

		addry = hdmidq->hwbuf[src_index]->phys;
		addruv = hdmidq->hwbuf[src_index]->phys + offset;
	} else {
		addry = dev->ion_buf.phys_addr;
		addruv = dev->ion_buf.phys_addr + offset;
		hdmidq->skip_frame[src_index] = 1;
		hdmidq->hwbuf[src_index] = NULL;
	}

	spin_unlock_irqrestore(&dev->slock, flags);

	if (src_index == 0)
		set_video_dest_addr(addry, addruv, -1, -1);
	else
		set_video_dest_addr(-1, -1, addry, addruv);
}

void hdmi_hw_buf_update(int src_index, struct v4l2_hdmi_dev *dev)
{
	static int skip_num = 0;
	static int total_num = 0;
	static unsigned long prev_jif = 0;
	unsigned long flags;
	int empty;
	struct hdmi_dmaqueue *hdmidq = &dev->hdmidq;

	if ((mipi_top.src_sel == 1) &&
		(Hdmi_CheckConditionChange() != HDMI_ERR_NO)) {

		HDMIRX_INFO("Condition change, restart detection");
		restartHdmiRxWrapperDetection();
		return;
	}

	if (mipi_top.mipi_init == 0)
		return;

	if (unlikely((jiffies - prev_jif) >= (10 * HZ))) {
		HDMIRX_INFO("skip %d/%d in %lu jiffies\n",
			skip_num, total_num, jiffies - prev_jif);
		prev_jif = jiffies;
		total_num = 0;
		skip_num = 0;
	}
	total_num++;

	spin_lock_irqsave(&dev->slock, flags);
	empty = list_empty(&hdmidq->active);
	spin_unlock_irqrestore(&dev->slock, flags);

	if (empty)
		skip_num++;

	update_mipi_hw_buffer(src_index, dev, empty);
}

irqreturn_t hdmirx_mipi_isr(int irq, void *dev_id)
{
	unsigned int st_reg_val;
	unsigned int reg_val;
	struct v4l2_hdmi_dev *dev = dev_id;

	hdmirx_wrapper_isr();

#ifdef CONFIG_RTK_HDCP1x_REPEATER
	if (HDCP_FLAG1_get_wr_aksv_flag(hdmi_rx_reg_read32(HDCP_FLAG1, HDMI_RX_MAC)))//Tx write Aksv
	{
		hdmi_rx_reg_mask32(HDCP_FLAG2, ~HDCP_FLAG2_irq_aksv_en_mask, 0, HDMI_RX_MAC);//Disable AKSV interrupt
		hdmi_rx_reg_write32(HDCP_FLAG1, HDCP_FLAG1_wr_aksv_flag_mask, HDMI_RX_MAC);//Clear Aksv flag
		hdmi_rx_reg_write32(HDMI_INTCR, HDMI_INTCR_pending_mask, HDMI_RX_MAC);// Clear HDCP interrupt
		start_hdcp1x_repeater_work();
	}
#endif

#ifdef CONFIG_RTK_HDCPRX_2P2
	Hdmi_HDCP_2_2_msg_hander();

	// Return isr if dma not enabled
	if( !HDMIRX_WRAPPER_CONTROL_0_get_fw_dma_en(hdmi_rx_reg_read32(HDMIRX_WRAPPER_CONTROL_0, HDMI_RX_HDMI_WRAPPER)) )
		return IRQ_HANDLED;
#endif

	st_reg_val = hdmi_rx_reg_read32(MIPI_INT_ST, HDMI_RX_MIPI);

	if (st_reg_val & 0x30) {
		/* Clear interrupt status */
		hdmi_rx_reg_write32(MIPI_INT_ST, st_reg_val, HDMI_RX_MIPI);

		HDMIRX_INFO("MIPI frame dropped");

		return IRQ_HANDLED;
	}

	reg_val = hdmi_rx_reg_read32(MIPI_TYPE, HDMI_RX_MIPI);
	if (unlikely(reg_val & 0x01)) {
		/* Picture mode */
		if (st_reg_val & MIPI_INT_ST_fm_done2_mask)
			hdmi_rx_reg_write32(MIPI_INT_ST, st_reg_val, HDMI_RX_MIPI);
	} else {
		/* Preview or video mode */
		if (st_reg_val & MIPI_INT_ST_fm_done0_mask)
			hdmi_hw_buf_update(0, dev);
		else if (st_reg_val & MIPI_INT_ST_fm_done1_mask)
			hdmi_hw_buf_update(1, dev);

		/* Clear interrupt status */
		hdmi_rx_reg_write32(MIPI_INT_ST, st_reg_val, HDMI_RX_MIPI);
	}

	return IRQ_HANDLED;
}

