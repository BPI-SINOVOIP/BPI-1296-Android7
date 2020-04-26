/*
 * hdmirx_wrapper.c - RTK hdmi rx driver
 *
 * Copyright (C) 2017 Realtek Semiconductor Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/printk.h>
#include <linux/ratelimit.h>
#include <linux/workqueue.h>

#include "hdmirx_wrapper.h"
#include "v4l2_hdmi_dev.h"
#include "hdmirx_reg.h"
#include "mipi_wrapper.h"

#include "rx_drv/hdmiInternal.h"

/*=================== extern Variable/Function ===================*/
extern HDMI_INFO_T hdmi;
extern HDMIRX_IOCTL_STRUCT_T hdmi_ioctl_struct;
extern MIPI_TOP_INFO mipi_top;
#if HDMI2p0
extern u_int8_t bHDMI_6G_flag;
#endif

extern HDMI_DVI_MODE_T IsHDMI(void);
#if HDMI_REPEATER_SUPPORT
extern void Set_Hdmitx_PLL(void);
#endif
/*======================================================*/

static void rxdetect_work_fun(struct work_struct *work);
static DECLARE_DELAYED_WORK(rxdetect_work, rxdetect_work_fun);

static void rxdetect_work_fun(struct work_struct *work)
{
	if(hdmi_ioctl_struct.detect_done)
		set_hdmirx_wrapper_interrupt_en(0,0,1);
}
 
const unsigned int resolution_format[29][2]=
{
	{ 640, 480},
	{ 720, 288},
	{ 720, 240},
	{ 720, 400},
	{ 720, 480},
	{ 720, 576},
	{ 800, 600},
	{1024, 768},
	{1152, 864},
	{1280, 600},
	{1280, 720},
	{1280, 768},
	{1280, 800},
	{1280, 960},
	{1280,1024},
	{1360, 768},
	{1366, 768},
	{1400, 900},
	{1400,1050},
	{1440, 480},
	{1440, 576},
	{1600, 900},
	{1600,1200},
	{1680,1050},
	{1920, 540},
	{1920,1072},
	{1920,1080},
	{3840,2160},
	{4096,2160},
};
const unsigned int format_number = sizeof(resolution_format)/sizeof(resolution_format[0]);

const HDMI_VIC_TABLE_T hdmi_vic_table[] = {
/* {width, height, fps, interlace} */
	{ 0, 0, 60, 0 }, // No such vic, set fps=60 for standard timings and DVI mode
	{ 640, 480, 60, 0 }, // vic: 1
	{ 720, 480, 60, 0 }, // vic: 2 , 480p
	{ 720, 480, 60, 0 }, // vic: 3
	{1280, 720, 60, 0 }, // vic: 4 , 720p60
	{1920,1080, 60, 1 }, // vic: 5, 1080i60
	{ 720, 480, 60, 1 }, // vic: 6, NTSC
	{ 720, 480, 60, 1 }, // vic: 7, NTSC
	{ 720, 240, 60, 0 }, // vic: 8
	{ 720, 240, 60, 0 }, // vic: 9
	{2880, 480, 60, 1 }, // vic: 10
	{2880, 480, 60, 1 }, // vic: 11
	{2880, 240, 60, 0 }, // vic: 12
	{2880, 240, 60, 0 }, // vic: 13
	{1440, 480, 60, 0 }, // vic: 14
	{1440, 480, 60, 0 }, // vic: 15
	{1920,1080, 60, 0 }, // vic: 16, 1080p60
	{ 720, 576, 50, 0 }, // vic: 17, 576p
	{ 720, 576, 50, 0 }, // vic: 18
	{1280, 720, 50, 0 }, // vic: 19, 720p50
	{1920,1080, 50, 1 }, // vic: 20, 1080i50
	{ 720, 576, 50, 1 }, // vic: 21, PAL
	{ 720, 576, 50, 1 }, // vic: 22
	{ 720, 288, 50, 0 }, // vic: 23
	{ 720, 288, 50, 0 }, // vic: 24
	{2880, 576, 50, 1 }, // vic: 25
	{2880, 576, 50, 1 }, // vic: 26
	{2880, 288, 50, 0 }, // vic: 27
	{2880, 288, 50, 0 }, // vic: 28
	{1440, 576, 50, 0 }, // vic: 29
	{1440, 576, 50, 0 }, // vic: 30
	{1920,1080, 50, 0 }, // vic: 31, 1080p50
	{1920,1080, 24, 0 }, // vic: 32
	{1920,1080, 25, 0 }, // vic: 33
	{1920,1080, 30, 0 }, // vic: 34
	{2880, 480, 60, 0 }, // vic: 35
	{2880, 480, 60, 0 }, // vic: 36
	{2880, 576, 50, 0 }, // vic: 37
	{2880, 576, 50, 0 }, // vic: 38
	{1920,1080, 50, 1 }, // vic: 39
	{1920,1080,100, 1 }, // vic: 40
	{1280, 720,100, 0 }, // vic: 41
	{ 720, 576,100, 0 }, // vic: 42
	{ 720, 576,100, 0 }, // vic: 43
	{ 720, 576,100, 1 }, // vic: 44
	{ 720, 576,100, 1 }, // vic: 45
	{1920,1080,120, 1 }, // vic: 46
	{1280, 720,120, 0 }, // vic: 47
	{ 720, 480,120, 0 }, // vic: 48
	{ 720, 480,120, 0 }, // vic: 49
	{ 720, 480,120, 1 }, // vic: 50
	{ 720, 480,120, 1 }, // vic: 51
	{ 720, 576,200, 0 }, // vic: 52
	{ 720, 576,200, 0 }, // vic: 53
	{ 720, 576,200, 1 }, // vic: 54
	{ 720, 576,200, 1 }, // vic: 55
	{ 720, 480,240, 0 }, // vic: 56
	{ 720, 480,240, 0 }, // vic: 57
	{ 720, 480,240, 1 }, // vic: 58
	{ 720, 480,240, 1 }, // vic: 59
	{1280, 720, 24, 0 }, // vic: 60
	{1280, 720, 25, 0 }, // vic: 61
	{1280, 720, 30, 0 }, // vic: 62
	{1920,1080,120, 0 }, // vic: 63
	{1920,1080,100, 0 }, // vic: 64
	{1280, 720, 24, 0 }, // vic: 65
	{1280, 720, 25, 0 }, // vic: 66
	{1280, 720, 30, 0 }, // vic: 67
	{1280, 720, 50, 0 }, // vic: 68
	{1280, 720, 60, 0 }, // vic: 69
	{1280, 720,100, 0 }, // vic: 70
	{1280, 720,120, 0 }, // vic: 71
	{1920,1080, 24, 0 }, // vic: 72
	{1920,1080, 25, 0 }, // vic: 73
	{1920,1080, 30, 0 }, // vic: 74
	{1920,1080, 50, 0 }, // vic: 75
	{1920,1080, 60, 0 }, // vic: 76
	{1920,1080,100, 0 }, // vic: 77
	{1920,1080,120, 0 }, // vic: 78
	{1680, 720, 24, 0 }, // vic: 79
	{1680, 720, 25, 0 }, // vic: 80
	{1680, 720, 30, 0 }, // vic: 81
	{1680, 720, 50, 0 }, // vic: 82
	{1680, 720, 60, 0 }, // vic: 83
	{1680, 720,100, 0 }, // vic: 84
	{1680, 720,120, 0 }, // vic: 85
	{2560,1080, 24, 0 }, // vic: 86
	{2560,1080, 25, 0 }, // vic: 87
	{2560,1080, 30, 0 }, // vic: 88
	{2560,1080, 50, 0 }, // vic: 89
	{2560,1080, 60, 0 }, // vic: 90
	{2560,1080,100, 0 }, // vic: 91
	{2560,1080,120, 0 }, // vic: 92
	{3840,2160, 24, 0 }, // vic: 93
	{3840,2160, 25, 0 }, // vic: 94
	{3840,2160, 30, 0 }, // vic: 95
	{3840,2160, 50, 0 }, // vic: 96
	{3840,2160, 60, 0 }, // vic: 97
	{4096,2160, 24, 0 }, // vic: 98
	{4096,2160, 25, 0 }, // vic: 99
	{4096,2160, 30, 0 }, // vic: 100
	{4096,2160, 50, 0 }, // vic: 101
	{4096,2160, 60, 0 }, // vic: 102
	{3840,2160, 24, 0 }, // vic: 103
	{3840,2160, 25, 0 }, // vic: 104
	{3840,2160, 30, 0 }, // vic: 105
	{3840,2160, 50, 0 }, // vic: 106
	{3840,2160, 60, 0 }, // vic: 107
};

unsigned char hdmirx_wrapper_convert_color_fmt(HDMI_COLOR_SPACE_T color)
{
	unsigned char yuv_fmt;

	switch(color)
	{
		case COLOR_RGB:
			yuv_fmt = 0;
			break;
		case COLOR_YUV422:
			yuv_fmt = 2;
			break;
		case COLOR_YUV444:
		case COLOR_YUV420:// YUV420to444
			yuv_fmt = 1;
			break;
		default:
			yuv_fmt = 0;
			HDMIRX_ERROR("[%s] Unsupported color(%u)",__FUNCTION__,color);
	}
	return yuv_fmt;
}

void set_hdmirx_wrapper_interrupt_en(int ver_err_en,int hor_err_en,int polarity_detect_en)
{
	unsigned int reg_val;
	reg_val = hdmi_rx_reg_read32(HDMIRX_WRAPPER_INTERRUPT_EN, HDMI_RX_HDMI_WRAPPER);

	if(ver_err_en >= 0){
		reg_val = reg_val & (~HDMIRX_WRAPPER_INTERRUPT_EN_ver_err_en_mask);
		reg_val = reg_val | HDMIRX_WRAPPER_INTERRUPT_EN_ver_err_en(ver_err_en);
	}
	if(hor_err_en >= 0){
		reg_val = reg_val & (~HDMIRX_WRAPPER_INTERRUPT_EN_hor_err_en_mask);
		reg_val = reg_val | HDMIRX_WRAPPER_INTERRUPT_EN_hor_err_en(hor_err_en);
	}
	if(polarity_detect_en >=0){
		reg_val = reg_val & (~HDMIRX_WRAPPER_INTERRUPT_EN_polarity_detect_en_mask);
		reg_val = reg_val | HDMIRX_WRAPPER_INTERRUPT_EN_polarity_detect_en(polarity_detect_en);
	}
	hdmi_rx_reg_write32(HDMIRX_WRAPPER_INTERRUPT_EN, reg_val, HDMI_RX_HDMI_WRAPPER);
}


void set_hdmirx_wrapper_active_pixels(unsigned v_pixel, unsigned h_pixel)
{

	hdmi_rx_reg_write32(HDMIRX_WRAPPER_ACTIVE_PIXELS, 
		HDMIRX_WRAPPER_ACTIVE_PIXELS_set_ver_width(v_pixel) |
		HDMIRX_WRAPPER_ACTIVE_PIXELS_set_hor_width(h_pixel), HDMI_RX_HDMI_WRAPPER);
}


void set_hdmirx_wrapper_hor_threshold(unsigned int h_threshold)
{
	hdmi_rx_reg_write32(HDMIRX_WRAPPER_HOR_THRESHOLD, 
		HDMIRX_WRAPPER_HOR_THRESHOLD_hor_threshold(h_threshold), HDMI_RX_HDMI_WRAPPER);
}


void set_hdmirx_wrapper_control_0(int fifo_stage,int fw_dma_en,int polarity_set_en,int polarity_set,int yuv_fmt,int hdmirx_en)
{
	unsigned int reg_val;
	reg_val = hdmi_rx_reg_read32(HDMIRX_WRAPPER_CONTROL_0, HDMI_RX_HDMI_WRAPPER);

	if(fifo_stage >= 0){
		reg_val = reg_val & (~HDMIRX_WRAPPER_CONTROL_0_fifo_stage_mask);
		reg_val = reg_val | HDMIRX_WRAPPER_CONTROL_0_fifo_stage(fifo_stage);
	}
	if(fw_dma_en >= 0){
		reg_val = reg_val & (~HDMIRX_WRAPPER_CONTROL_0_fw_dma_en_mask);
		reg_val = reg_val | HDMIRX_WRAPPER_CONTROL_0_fw_dma_en(fw_dma_en);
	}
	if(polarity_set_en >= 0){
		reg_val = reg_val & (~HDMIRX_WRAPPER_CONTROL_0_polarity_set_en_mask);
		reg_val = reg_val | HDMIRX_WRAPPER_CONTROL_0_polarity_set_en(polarity_set_en);
	}
	if(polarity_set >= 0){
		reg_val = reg_val & (~HDMIRX_WRAPPER_CONTROL_0_polarity_set_mask);
		reg_val = reg_val | HDMIRX_WRAPPER_CONTROL_0_polarity_set(polarity_set);
	}
	if(yuv_fmt >= 0){
		reg_val = reg_val & (~HDMIRX_WRAPPER_CONTROL_0_yuv_fmt_mask);
		reg_val = reg_val | HDMIRX_WRAPPER_CONTROL_0_yuv_fmt(yuv_fmt);
	}
	if(hdmirx_en >= 0){
		reg_val = reg_val & (~(HDMIRX_WRAPPER_CONTROL_0_reg_clken_hdmirx_mask|HDMIRX_WRAPPER_CONTROL_0_hdmirx_en_mask));
		reg_val = reg_val | HDMIRX_WRAPPER_CONTROL_0_reg_clken_hdmirx(hdmirx_en) |HDMIRX_WRAPPER_CONTROL_0_hdmirx_en(hdmirx_en);
	}
	hdmi_rx_reg_write32(HDMIRX_WRAPPER_CONTROL_0, reg_val, HDMI_RX_HDMI_WRAPPER);

}

unsigned int hdmirx_wrapper_get_active_line(void)
{
	unsigned int buf0;
	buf0 = HDMIRX_WRAPPER_MONITOR_1_get_line_buf0( hdmi_rx_reg_read32(HDMIRX_WRAPPER_MONITOR_1, HDMI_RX_HDMI_WRAPPER) );
	return buf0;
}

unsigned int hdmirx_wrapper_get_active_pixel(void)
{
	unsigned int reg01;
	unsigned int buf0;

	reg01 = hdmi_rx_reg_read32(HDMIRX_WRAPPER_DEN_BUF01, HDMI_RX_HDMI_WRAPPER);
	buf0 = HDMIRX_WRAPPER_DEN_BUF01_get_den_buf0(reg01);
	return buf0;
}

static inline unsigned int check_hdmirx_resolution_match(void)
{
	unsigned int active_pixel,active_line,height;
	unsigned int i,vic;
	unsigned char color_fmt;

	vic = 0;
	if (IsHDMI())
		vic = drvif_Hdmi_AVI_VIC();

	active_pixel =	hdmirx_wrapper_get_active_pixel();
	active_line = hdmirx_wrapper_get_active_line();
	color_fmt = hdmirx_wrapper_convert_color_fmt(GET_HDMI_COLOR_SPACE());

	if (hdmi.tx_timing.progressive)
		height = active_line;
	else
		height = active_line*2;

	if (active_pixel==hdmi_vic_table[vic].width && height==hdmi_vic_table[vic].height && vic)
	{
		mipi_top.h_input_len = active_pixel;
		mipi_top.v_input_len = active_line;
		mipi_top.input_color = color_fmt;
		HDMIRX_INFO("Check resolution match => Width(%u) Height(%u) VIC(%u)",active_pixel,height,vic);
		return 1;
	}
	else
	{
		/* Support standard timings and DVI mode */
		for (i=0;i<format_number;i++)
		{
			if (active_pixel==resolution_format[i][0] && active_line==resolution_format[i][1])
			{
				mipi_top.h_input_len = active_pixel;
				mipi_top.v_input_len = active_line;
				mipi_top.input_color = color_fmt;
				if (vic == 0)
					HDMIRX_INFO("Check resolution match => Width(%u) Height(%u) DIV",active_pixel,height);
				else
					HDMIRX_ERROR("Check resolution => Width(%u) Height(%u), wrong VIC(%u)",active_pixel,height,vic);
				return 1;
			}
		}
	}

	HDMIRX_INFO("Check resolution not match => active_pixel(%u) active_line(%u) VIC(%u)",active_pixel,active_line,vic);
	return 0;
}

void restartHdmiRxWrapperDetection(void)
{
	unsigned char hdcp_state;

	stop_mipi_process();
	set_hdmirx_wrapper_interrupt_en(0,0,0);
	SET_HDMI_VIDEO_FSM(MAIN_FSM_HDMI_SETUP_VEDIO_PLL);
	SET_HDMI_AUDIO_FSM(AUDIO_FSM_AUDIO_START);
	set_hdmirx_wrapper_control_0(-1, 0,-1,-1,-1,-1);//Stop DMA
	set_hdmirx_wrapper_hor_threshold(HDMI_MIN_SUPPORT_H_PIXEL);
#if HDMI2p0
	bHDMI_6G_flag=0;
#endif

	hdcp_state = hdmi_ioctl_struct.hdcp_state;
	memset(&hdmi_ioctl_struct, 0, sizeof(hdmi_ioctl_struct));
	hdmi_ioctl_struct.hdcp_state = hdcp_state;//keep hdcp state, clear when cable unplugged
}

void hdmirx_wrapper_isr(void)
{
	static unsigned char miss_count = 0;
	unsigned int status = hdmi_rx_reg_read32(HDMIRX_WRAPPER_INTERRUPT_STATUS, HDMI_RX_HDMI_WRAPPER);
	unsigned int active_pixel,active_line;
	char *str_color[] = { "RGB", "YUV444" , "YUV422"};
	char *str_progressive[] = { "I", "P" };

	if(likely(status == 0))
	{
		//pr_info("do nothing...\n");
		return;
	}
	else if(HDMIRX_WRAPPER_INTERRUPT_STATUS_get_polarity_detect_sts(status))//POLARITY_DETECT
	{
		set_hdmirx_wrapper_interrupt_en(0,0,0);//Disable interrupt

		if(!check_hdmirx_resolution_match())//Format not match, re-enable polarity detection if miss_count<3
		{
			if(miss_count>=3)
			{
				miss_count = 0;
				HDMIRX_INFO("polarity detect fail: Reset detection...\n");
				restartHdmiRxWrapperDetection();
			}
			else
			{
				miss_count++;
				schedule_delayed_work(&rxdetect_work, msecs_to_jiffies(1000));//Enable wrapper interrupt later
			}
			return;
		}

		HDMIRX_INFO("polarity detect done: hor(%u) ver(%u) color(%s) progressive(%s)",
						mipi_top.h_input_len, mipi_top.v_input_len,str_color[mipi_top.input_color],str_progressive[hdmi.tx_timing.progressive]);

		set_hdmirx_wrapper_active_pixels(mipi_top.v_input_len, mipi_top.h_input_len);
		set_hdmirx_wrapper_hor_threshold(mipi_top.h_input_len);

		hdmi_ioctl_struct.measure_ready = 1;

#if HDMI_REPEATER_SUPPORT
		Set_Hdmitx_PLL();
#endif

		set_hdmirx_wrapper_interrupt_en(1,1,0);//Format match, enable error detection
	}
	else if(HDMIRX_WRAPPER_INTERRUPT_STATUS_get_hor_err_sts(status))//HOR_ERR
	{
		set_hdmirx_wrapper_interrupt_en(0,0,0);

		restartHdmiRxWrapperDetection();

		active_pixel =	hdmirx_wrapper_get_active_pixel();
		HDMIRX_INFO("ISR status:%x reenable polarity detection... pixel(%u)\n", status, active_pixel);
		miss_count = 0;

		return;
	}
	else if(HDMIRX_WRAPPER_INTERRUPT_STATUS_get_ver_err_sts(status))//VER_ERR
	{
		set_hdmirx_wrapper_interrupt_en(0,0,0);

		restartHdmiRxWrapperDetection();

		active_line = hdmirx_wrapper_get_active_line();
		HDMIRX_INFO("ISR status:%x reenable polarity detection... line(%u)\n", status, active_line);
		miss_count = 0;

		return;
	}
}


void hdmi_related_wrapper_init(void)
{
	//HDMIRX_INFO("[%s]", __FUNCTION__);

	set_hdmirx_wrapper_interrupt_en(0,0,0); //v,h,polarity
	set_hdmirx_wrapper_control_0(1,0,0,0,0,1);//fifo_stage,fw_dma_en,polarity_set_en,polarity_set,yuv_fmt,hdmirx_en
	set_hdmirx_wrapper_hor_threshold(HDMI_MIN_SUPPORT_H_PIXEL);
}

