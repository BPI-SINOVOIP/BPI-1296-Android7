/*
 *  venus_ir.c - RTK irda driver
 *
 * Copyright (C) 2017 Realtek Semiconductor Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/platform_device.h>
#include <linux/of_irq.h>
#include <linux/of_address.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/types.h>
#include <linux/input.h>
#include <linux/kfifo.h>
#include <linux/freezer.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/poll.h>
#include <linux/clk.h>
#include <linux/reset.h>

#ifdef CONFIG_SUPPORT_INPUT_DEVICE
#include "ir_input.h"
#endif

#include "venus_ir.h"
#include "ir_sw_decoder.h"
#include <soc/realtek/memory.h>
#include <soc/realtek/rtk_ipc_shm.h>

#define REPEAT_MODE 0xFF
#define REPEAT_MODE_FIFO 1024
//#define REPEAT_MODE_SELFTEST

#define DELAY_FIRST_REPEAT
#define KEYMAP_TABLE_COLUMN 2

#define VENUS_IR_MAJOR			234
#define VENUS_IR_DEVICE_NUM		1
#define VENUS_IR_MINOR			50
#define VENUS_IR_DEVICE_FILE	"venus_ir"

#define IRTX_CUSTOMER_CODE RTK_MK5_CUSTOMER_CODE

#define FIFO_DEPTH	16		/* driver queue depth */
#define REPEAT_TIMEOUT 	500

#define DEV_DEBUG
#ifdef DEV_DEBUG
#define RTK_debug(fmt, ...) printk(fmt, ##__VA_ARGS__)
#else
#define RTK_debug(fmt, ...)
#endif

#ifdef DELAY_FIRST_REPEAT
static unsigned int PressRecvMs;
static unsigned int firstRepeatInterval = 300; // default disable first repeat delay
static unsigned int firstRepeatTriggered = false;
#endif

static unsigned int driver_mode = SINGLE_WORD_IF;
static bool bUseUserTable = false;
//static unsigned int irda_dev_count =1;	/* for multiple support 2015.04.22*/

static bool RPRepeatIsHandling = false;
static bool isKeypadTimerCreate = false;
//static bool f_CursorEnable = false;
static unsigned int debounce = 0;
static unsigned int lastSrValue = 0;
static struct Venus_IRTx_KeycodeTable IRTx_KeycodeTable;
static int IRTx_KeycodeTableEnable=0;
static int start_tx=0;
extern long int Input_Keybit_Length;

static struct venus_hw_param hw_param[] = {
	//psr, per, sf	or	raw_deb, raw_ctl, sf
	{ {0}, {0}, 0 },								//reserved
	{ {0x5a13133b}, {0x0001162d}, 0x0000021b },		//NEC
	{ {0x00242424}, {0x00010000}, 0x0000021b },		//RC5
	{ {0x00051b54}, {0x00010000}, 0x0000021b },		//SHARP
	{ {0x18181830}, {0x00010006}, 0x0000021b },		//SONY
	{ {0x25151034}, {0x00000025}, 0x0000021b },		//C03
	{ {0x1c0e0e0e}, {0x00030008}, 0x0000021b },		//RC6
	{ {0x00000a8b}, {0x03138850}, 0x00001517 },		//RAW_NEC
	{ {0x2f082a5a}, {0x0001002a}, 0xff00021b },		//RCA
	{ {0x24120d30}, {0x00010010}, 0x6767021b },		//PANASONIC
	{ {0x1e143c64}, {0x0003001a}, 0x0000021b },		//KONKA
	{ {0x00000545}, {0x03138848}, 0x00000a8b },		//RAW_RC6
	{ {0x00111111}, {0x00000000}, 0x0000021b },		//RC6_MODE_6
	{ {0x2d161643}, {0x0001102d}, 0x0000021b },		//TOSHIBA
	{ {0x00000a8b}, {0x03138848}, 0x00001517 },		//RAW_DIRECTTV
	{ {0x0000021b}, {0x03138850}, 0x00000437 },		//RAW_COMCAST
	{ {0x00000a8b}, {0x03138848}, 0x00001517 },		//RAW_MOTO
	{ {0x00000a8b}, {0x03138848}, 0x00001517 },		//RAW_CISCO
};

static struct venus_key rtk_mk5_tv_keys[] = {
	{0x18, KEY_POWER},			//POWER
	{0x5A, KEY_SELECT},			//SOURCE
	{0x58, KEY_CYCLEWINDOWS},	//PIP
	{0x1A, KEY_TV},				//TV SYSTEM
	{0x14, KEY_HOME},			//HOME
	{0x56, KEY_OPTION},			//OPTION MENU
	{0x54, KEY_INFO},			//MEDIA_INFO
	{0x19, KEY_REPLY},			//REPEAT
	{0x57, KEY_BACK},			//RETURN
	{0x55, KEY_PLAY},			//PLAY/PAUSE
	{0x17, KEY_STOP},			//STOP
	{0x15, KEY_ZOOM},			//ZOOM_IN
	{0x4F, KEY_REWIND},			//FR
	{0x4D, KEY_UP},				//UP
	{0x16, KEY_FASTFORWARD},	//FF
	{0x0C, KEY_LEFT},			//LEFT
	{0x4C, KEY_OK},				//OK
	{0x0E, KEY_RIGHT},			//RIGHT
	{0x08, KEY_PREVIOUS},		//PREVIOUS
	{0x48, KEY_DOWN},			//DOWN
	{0x09, KEY_NEXT},			//NEXT
	{0x4B, KEY_VOLUMEDOWN},		//VOL-
	{0x49, KEY_VOLUMEUP},		//VOL+
	{0x0B, KEY_TOUCHPAD_OFF},	//CURSOR
	{0x0A, KEY_TOUCHPAD_ON},	// GESTURE
	{0xEEEE, REL_X},			//REL_X			//mouse X-axi	0xEEEE : REL event type
	{0xEEEE, REL_Y},			//REL_Y			//mouse Y-axi	0xEEEE : REL event type
	{0xFFFF, BTN_LEFT},			//BTN_LEFT		//mouse left key	0xFFFF : KEY event type
};

static struct venus_key_table rtk_mk5_tv_key_table = {
	.keys = rtk_mk5_tv_keys,
	.size = ARRAY_SIZE(rtk_mk5_tv_keys),
	.cust_code = RTK_MK5_CUSTOMER_CODE,
	.scancode_msk = 0x00FF0000,
	.custcode_msk = 0xFFFF,
};

struct rtk_ir_cdev {
	dev_t dev_no;
	struct cdev *cdev;
	struct class *class;
	
	wait_queue_head_t rx_wait;
	struct kfifo ir_rxfifo;
	struct kfifo ir_txfifo;
};

struct rtk_ir_dev {
	struct device *dev;
		
	void __iomem *sys_reg;
	void __iomem *ir_reg;
	
	int ir_protocol;
	int irtx_protocol;

	int rpmode;
	int rpmode_fifosize;
	unsigned char *rpmode_txfifo;
	unsigned char *rpmode_rxfifo;

	unsigned int sample_rate;
	unsigned int dpir_val;
	unsigned int ir_irq;
	unsigned int multiple_support;
	unsigned int irda_dev_count;
	struct ir_sw_decoder *pdec;

	unsigned int lastRecvMs;
	
	struct venus_key_table *p_rtk_key_table;
	struct timer_list ir_wakeup_timer;
	struct timer_list ir_keypad_timer;
	struct clk * clk;
	struct reset_control *rstc;

	spinlock_t ir_splock;
};

struct user_key_data{
	struct venus_key_table key_table;
	unsigned int max_size;
};

static struct user_key_data rtk_mk5_user_keys = {
	.key_table = {
		.keys = NULL,
		.size = 0,
	},
	.max_size = 0,
};

static struct user_key_data yk_54lu_user_keys = {
	.key_table = {
		.keys = NULL,
		.size = 0,
	},
	.max_size = 0,
};

static struct user_key_data rtk_mk5_2_user_keys = {
	.key_table = {
		.keys = NULL,
		.size = 0,
	},
	.max_size = 0,
};

static struct user_key_data libra_ms_user_keys = {
	.key_table = {
		.keys = NULL,
		.size = 0,
	},
	.max_size = 0,
};

static struct user_key_data jaecs_t118_user_keys = {
	.key_table = {
		.keys = NULL,
		.size = 0,
	},
	.max_size = 0,
};

static struct user_key_data rtk_mk3_user_keys = {
	.key_table = {
		.keys = NULL,
		.size = 0,
	},
	.max_size = 0,
};

static struct user_key_data yk_70lb_user_keys = {
	.key_table = {
		.keys = NULL,
		.size = 0,
	},
	.max_size = 0,
};

static struct user_key_data rtk_mk4_user_keys = {
	.key_table = {
		.keys = NULL,
		.size = 0,
	},
	.max_size = 0,
};

static struct user_key_data netg_ms_user_keys = {
	.key_table = {
		.keys = NULL,
		.size = 0,
	},
	.max_size = 0,
};

struct rtk_ir_dev *ir_dev;
struct rtk_ir_cdev *ir_cdev;

int rtk_ir_init(void)
{
	unsigned int reg_dpir = 0;
	unsigned int protocol = ir_dev->ir_protocol;

	ir_dev->lastRecvMs = jiffies_to_msecs(jiffies);

	writel(IRCR_RESET, ir_dev->ir_reg+SATURN_ISO_IR_CR_OFF);

	if(protocol==RAW_NEC || protocol==RAW_RC6 || protocol==RAW_DIRECTTV || protocol==RAW_COMCAST || protocol==RAW_MOTO|| protocol==RAW_CISCO) {
		writel(hw_param[protocol].ir_sf, ir_dev->ir_reg+SATURN_ISO_IR_SF_OFF);
		writel(hw_param[protocol].ir_raw_deb, ir_dev->ir_reg+SATURN_ISO_IR_RAW_DEB_OFF);
		writel(hw_param[protocol].ir_raw_ctl, ir_dev->ir_reg+SATURN_ISO_IR_RAW_CTRL_OFF);		
	} else if(protocol == REPEAT_MODE) {
		writel( (ir_dev->sample_rate*27 - 1), ir_dev->ir_reg+SATURN_ISO_IR_SF_OFF);
		writel( 0x1a, ir_dev->ir_reg+SATURN_ISO_IR_RAW_DEB_OFF);
		writel( 0x03001050, ir_dev->ir_reg+SATURN_ISO_IR_RAW_CTRL_OFF);
	} else {
		writel(hw_param[protocol].ir_psr, ir_dev->ir_reg+SATURN_ISO_IR_PSR_OFF);
		writel(hw_param[protocol].ir_per, ir_dev->ir_reg+SATURN_ISO_IR_PER_OFF);
		writel(hw_param[protocol].ir_sf, ir_dev->ir_reg+SATURN_ISO_IR_SF_OFF);
	}

	reg_dpir = (ir_dev->dpir_val*1000 *27)/(readl(ir_dev->ir_reg+SATURN_ISO_IR_SF_OFF)+1);
	writel(reg_dpir, ir_dev->ir_reg+SATURN_ISO_IR_DPIR_OFF);

	switch(protocol) {
		case NEC:
			writel(0x1, ir_dev->ir_reg+SATURN_ISO_IR_CR1_OFF);
			writel(0x87000000, ir_dev->ir_reg+SATURN_ISO_IR_PSR_UP_OFF);
			writel(0x00002200, ir_dev->ir_reg+SATURN_ISO_IR_PER_UP_OFF);
			writel(0x5df, ir_dev->ir_reg+SATURN_ISO_IR_CR_OFF);
			break;
		case RC5:
			writel(0x70c, ir_dev->ir_reg+SATURN_ISO_IR_CR_OFF);
			break;
		case SHARP:
			writel(0x58e, ir_dev->ir_reg+SATURN_ISO_IR_CR_OFF);
			break;
		case SONY:
			writel(0xdd3, ir_dev->ir_reg+SATURN_ISO_IR_CR_OFF);
			break;
		case C03:
			writel(0x5ff, ir_dev->ir_reg+SATURN_ISO_IR_CR_OFF);
			break;
		case RC6:
			writel(0x715, ir_dev->ir_reg+SATURN_ISO_IR_CR_OFF);
			writel(0x123, ir_dev->ir_reg+SATURN_ISO_IR_CTRL_RC6_OFF);
			break;
		case RAW_NEC:
			writel(0x7300, ir_dev->ir_reg+SATURN_ISO_IR_CR_OFF);
			break;
		case RCA:	//TCL REMOTE Control
			writel(0x7d7, ir_dev->ir_reg+SATURN_ISO_IR_CR_OFF);
			break;
		case PANASONIC:
			writel(0x00af05c0, ir_dev->ir_reg+SATURN_ISO_IR_CR_OFF);
			break;
		case KONKA:
			writel(0x7cf, ir_dev->ir_reg+SATURN_ISO_IR_CR_OFF);
			break;
		case RAW_RC6:
			writel(0x7300, ir_dev->ir_reg+SATURN_ISO_IR_CR_OFF);
			break;
		case RC6_MODE_6:
			writel(0x00a50720, ir_dev->ir_reg+SATURN_ISO_IR_CR_OFF);
			writel(0x123, ir_dev->ir_reg+SATURN_ISO_IR_CTRL_RC6_OFF);
			break;
		case TOSHIBA:
			writel(0x010007df, ir_dev->ir_reg+SATURN_ISO_IR_CR_OFF);
			break;
		case RAW_DIRECTTV:
		case RAW_COMCAST:
		case RAW_MOTO:
		case RAW_CISCO:
		case REPEAT_MODE:
			writel(0x7300, ir_dev->ir_reg+SATURN_ISO_IR_CR_OFF);
			break;
	}
	switch(ir_dev->irtx_protocol) {
		case NEC_TX:
			writel(0x00000400, ir_dev->ir_reg+SATURN_ISO_IRTX_CFG_OFF);	//Inactive Ouput Level set 1 to default low

			// PWM=38kHz, carrier freq, duty factor = 1/3
			// PWM clock source divisor = 33MHz/2^(1+1)=8.25MHz =>0x1, duty = 217/3=72 =>0x47
			// PWM output clock divisor = 8.25MHz/217=38.018kHz =>0xD8
			writel(0x13AB0, ir_dev->ir_reg+SATURN_ISO_IRTX_PWM_SETTING_OFF);

			// measured unit-high width=560us => freq=1.786kHz
			// IRTX output frequncy = XTAL 33MHz / (IRTX_FD+1)=1.786kHz
			writel(0x3B0D, ir_dev->ir_reg+SATURN_ISO_IRTX_TIM_OFF);

			// IRTX Enable, Output Modulation disable, LSB first, Output Inverse
			// Inactive Ouput High Level (acturally the implement is low level)
			// FIFO Output Inverse Bypass
			writel(0x80000710, ir_dev->ir_reg+SATURN_ISO_IRTX_CFG_OFF);

			// Data Threshold=16, Data finish Interrupt Enable, Data Empty Interrupt Disable, Data Request Interrupt Disable
			writel(0xf04, ir_dev->ir_reg+SATURN_ISO_IRTX_INT_EN_OFF);

			//tx fifo reset
			writel(0x80000000, ir_dev->ir_reg+SATURN_ISO_IRTX_FIFO_ST_OFF);	//irtx fifo reset
			writel(0x00000000, ir_dev->ir_reg+SATURN_ISO_IRTX_FIFO_ST_OFF);	//irtx fifo normal
			break;
		case REPEAT_MODE:
			writel(0x00000400, ir_dev->ir_reg+SATURN_ISO_IRTX_CFG_OFF);	//Inactive Ouput Level set 1 to default low
			writel(0x13AB0, ir_dev->ir_reg+SATURN_ISO_IRTX_PWM_SETTING_OFF);
			writel( ir_dev->sample_rate*27 - 1, ir_dev->ir_reg+SATURN_ISO_IRTX_TIM_OFF);
			writel(0x80000510, ir_dev->ir_reg+SATURN_ISO_IRTX_CFG_OFF);
			// Data Threshold=16, Data finish Interrupt Enable, Data Empty Interrupt Disable, Data Request Interrupt Disable
			writel(0xf06, ir_dev->ir_reg+SATURN_ISO_IRTX_INT_EN_OFF);

			//tx fifo reset
			writel(0x80000000, ir_dev->ir_reg+SATURN_ISO_IRTX_FIFO_ST_OFF);	//irtx fifo reset
			writel(0x00000000, ir_dev->ir_reg+SATURN_ISO_IRTX_FIFO_ST_OFF);	//irtx fifo normal
			break;
	}
	return 0;
}

#ifdef CONFIG_SUPPORT_INPUT_DEVICE
static void rtk_ir_wakeup_timer(unsigned long data)
{
	unsigned long lock_flags;

	//send power key to wakeup android   victor20140617
	spin_lock_irqsave(&ir_dev->ir_splock, lock_flags);

	venus_ir_input_report_key(KEY_STOP);
	venus_ir_input_report_end();

	spin_unlock_irqrestore(&ir_dev->ir_splock, lock_flags);
}
#endif

static void rtk_ir_keypad_timer(unsigned long data)
{
	unsigned long lock_flags;
	spin_lock_irqsave(&ir_dev->ir_splock, lock_flags);
	if (isKeypadTimerCreate)
	{
		isKeypadTimerCreate = false;
		venus_ir_input_report_end();
	}
	spin_unlock_irqrestore(&ir_dev->ir_splock, lock_flags);
}

void rtk_irrp_enable(int en)
{
	unsigned int regValue;
	if(en) {
		writel(0x5df, ir_dev->ir_reg+SATURN_ISO_IR_CR_OFF);
	} else {
		regValue = readl(ir_dev->ir_reg+SATURN_ISO_IR_CR_OFF);
		writel((regValue & ~0x100), ir_dev->ir_reg+SATURN_ISO_IR_CR_OFF);	//irrx stop
	}
}

void rtk_irtx_enable(int en)
{
	unsigned int regValue;
	
	if(en) {
		regValue =	readl(ir_dev->ir_reg+SATURN_ISO_IRTX_CFG_OFF);
		writel((regValue | 0x1), ir_dev->ir_reg+SATURN_ISO_IRTX_CFG_OFF);		//irtx stop
	} else {
		regValue =	readl(ir_dev->ir_reg+SATURN_ISO_IRTX_CFG_OFF);
		writel((regValue & ~0x1), ir_dev->ir_reg+SATURN_ISO_IRTX_CFG_OFF);		//irtx stop
	}
}

int venus_ir_IrMessage2Keycode(unsigned int message, unsigned int *p_keycode)
{
	int i, count;
	int custcode_off=0, scancode_off=0;
	unsigned int scancode;
	struct venus_key_table *key_table = ir_dev->p_rtk_key_table;

	RTK_debug(KERN_DEBUG "scancode=0x%x\n", message);

	for(count=0; count<ir_dev->irda_dev_count; count++) {	/* for multiple support 2015.04.22*/
		for(i=0; i<32; i++) {
			if((0x1<<i)&(key_table[count].custcode_msk)) {
				custcode_off = i;
				break;
			}
		}
		if (((message & key_table[count].custcode_msk)>>custcode_off) == key_table[count].cust_code) {
			for(i=0; i<32; i++) {
				if((0x1<<i)&(key_table[count].scancode_msk)) {
					scancode_off = i;
					break;
				}
			}
			scancode = (message & key_table[count].scancode_msk) >> scancode_off;
			for (i=0; i<key_table[count].size; i++) {
				if (scancode == key_table[count].keys[i].scancode) {
					*p_keycode = key_table[count].keys[i].keycode;
					RTK_debug(KERN_DEBUG "report key scancode=0x%x keycode=%d\n", scancode, *p_keycode);
					return 0;
				}
			}
		}
	}
	return -1;
}

static int examine_ir_avail(uint32_t *regValue, uint32_t *irrp2Value, int *dataRepeat)
{
	unsigned int srValue = readl(ir_dev->ir_reg+SATURN_ISO_IR_SR_OFF);
	
	*irrp2Value = 0;
	
	if(srValue & 0x1) {
		if(srValue & 0x2)// 0x2 is check for "Repeat flag for NEC only"
			*dataRepeat = 1;
		else
			*dataRepeat = 0;

		writel(0x00000003, ir_dev->ir_reg+SATURN_ISO_IR_SR_OFF);
		*regValue = readl(ir_dev->ir_reg+SATURN_ISO_IR_RP_OFF);

		if(ir_dev->ir_protocol == PANASONIC)
			*irrp2Value = readl(ir_dev->ir_reg+SATURN_ISO_IR_RP2_OFF); /* read IRRP2 */

		if(ir_dev->ir_protocol == RC6) {
			if((lastSrValue&0xffff) == (*regValue&0xffff)) {
				*dataRepeat = 1;
				debounce = 0;
			}
			else
				*dataRepeat = 0;
			lastSrValue = *regValue;
		} else if(ir_dev->ir_protocol == RCA) {
			if(lastSrValue == *regValue) {
				*dataRepeat = 1;
				debounce = 0; /* for first repeat interval*/
			}
			else
				*dataRepeat = 0;
			lastSrValue = *regValue;
		}
		return 0;
	} else
		return -ERESTARTSYS;
}

static int repeat_key_handle(unsigned int regValue, unsigned int irrp2Value, int dataRepeat)
{
	unsigned int keycode;
	int received = 0, ret = 0;

	if((RPRepeatIsHandling== true) && ((jiffies_to_msecs(jiffies)- ir_dev->lastRecvMs) < debounce)) {
		ir_dev->lastRecvMs = jiffies_to_msecs(jiffies);
		received = 1;
	} else {
		ir_dev->lastRecvMs = jiffies_to_msecs(jiffies);
		if(ir_dev->ir_protocol == RC6)
			regValue = ~(regValue)&0x1fffff;  // 21 bits
		else if(ir_dev->ir_protocol == RC5)
			regValue = (regValue&0xfff);

		if(ir_dev->ir_protocol == PANASONIC && irrp2Value != 0x20020000) {
			RTK_debug(KERN_DEBUG "Venus IR: Maker code = [%08X].\n", irrp2Value);
		} else if(ir_dev->ir_protocol == RAW_DIRECTTV || ir_dev->ir_protocol == RAW_COMCAST || ir_dev->ir_protocol == RAW_MOTO || ir_dev->ir_protocol==RAW_CISCO) {
//			printk(KERN_ERR "msg_code = 0x%04x\n", regValue);
			if(ir_dev->p_rtk_key_table == &rtk_mk5_tv_key_table) {
				keycode = regValue;
			}
			else {
				ret = venus_ir_IrMessage2Keycode(regValue, &keycode);
				if(ret == -1) {
					keycode = regValue;
					ret = 0;
				}
			}
//			printk(KERN_ERR "keycode = 0x%04x\n", keycode);
			if (ret == 0) {
				kfifo_in(&ir_cdev->ir_rxfifo, &keycode, sizeof(unsigned int));
//				if (isKeypadTimerCreate) {
//					isKeypadTimerCreate = false;
//					venus_ir_input_report_end();
//				}
				venus_ir_input_report_key(keycode);
				venus_ir_input_report_end();
			}
//			isKeypadTimerCreate = true;
			received = 1;
		}
		else {
			kfifo_in(&ir_cdev->ir_rxfifo, &regValue, sizeof(unsigned int));
			if (-1 != venus_ir_IrMessage2Keycode(regValue, &keycode)) {
#ifdef CONFIG_SUPPORT_INPUT_DEVICE
				venus_ir_input_report_key(keycode);
				isKeypadTimerCreate = true;
#endif
			}

			if(driver_mode == DOUBLE_WORD_IF)
				kfifo_in(&ir_cdev->ir_rxfifo, (unsigned char *)&dataRepeat, sizeof(int));
#ifdef DELAY_FIRST_REPEAT
			if(dataRepeat == 1)
				firstRepeatTriggered = true;
			else {
				PressRecvMs = ir_dev->lastRecvMs;
				firstRepeatTriggered = false;
			}
#endif
			received = 1;
		}
	}
	return received;
}

int venus_irtx_getscancode(unsigned int keycode, unsigned int* p_scancode, unsigned int* p_scancode_msk)
{
	unsigned int i;

	if((!p_scancode)||(!p_scancode_msk)) {
		RTK_debug(KERN_ALERT "[ERR] venus_ir : the p_scancode/p_scancode_msk pointer is NULL\n");
		return -1;
	}

	*p_scancode_msk = ir_dev->p_rtk_key_table->scancode_msk;

	for (i = 0; i < ir_dev->p_rtk_key_table->size; i++) {
		if (keycode == ir_dev->p_rtk_key_table->keys[i].keycode) {
			*p_scancode = ir_dev->p_rtk_key_table->keys[i].scancode;
			return 0;
		}
	}

	return -1;
}

static int venus_irtx_nec_send(u32 scancode, u32 customer_code)
{
	volatile unsigned char AGC_burs_reg[3], NecAddr_reg[6], NecCmd_reg[6], NecCmdEnd_reg;
	unsigned short NecAddr, NecCmd;
	unsigned int i;
	volatile int reg_bit_offset;
	volatile unsigned int reg_val;

	NecAddr = (unsigned short)(customer_code & 0x0000ffff);
	NecCmd = (unsigned short)(((~(scancode&0x000000ff))<<8)  |  (scancode&0x000000ff));

	memset ((void*)AGC_burs_reg, 0, sizeof(AGC_burs_reg));
	memset ((void*)NecAddr_reg, 0, sizeof(NecAddr_reg));
	memset ((void*)NecCmd_reg, 0, sizeof(NecCmd_reg));

	AGC_burs_reg[2] = 0xff;
	AGC_burs_reg[1] = 0xff;
	AGC_burs_reg[0] = 0x00;
	NecCmdEnd_reg= 0x80;

	reg_bit_offset = (8*sizeof(NecAddr_reg))-1;
	for (i=0; i<(8*sizeof(NecAddr)); i++)
	{
		if (reg_bit_offset<0) {
			RTK_debug(KERN_ALERT "{IRTX]_venus_irtx_nec_send: ERROR : tx reg_bit_offset of NecAddr_reg is negtive \n");
			return -1;
		} else if(NecAddr & (0x1<<i)) {
			__set_bit(reg_bit_offset, (unsigned long *)NecAddr_reg);
			reg_bit_offset= reg_bit_offset-4;
		} else {
			__set_bit(reg_bit_offset, (unsigned long *)NecAddr_reg);
			reg_bit_offset= reg_bit_offset-2;
		}
	}

	reg_bit_offset = (8*sizeof(NecCmd_reg))-1;
	for (i=0; i<(8*sizeof(NecCmd)); i++) {
		if (reg_bit_offset<0) {
			RTK_debug(KERN_ALERT "{IRTX]_venus_irtx_nec_send: ERROR : tx reg_bit_offset of NecAddr_reg is negtive \n");
			return -1;
		} if(NecCmd & (0x1<<i)) {
			__set_bit(reg_bit_offset, (unsigned long *)NecCmd_reg);
			reg_bit_offset= reg_bit_offset-4;
		} else {
			__set_bit(reg_bit_offset, (unsigned long *)NecCmd_reg);
			reg_bit_offset= reg_bit_offset-2;
		}
	}

	reg_val = ((AGC_burs_reg[2]<<24) | (AGC_burs_reg[1]<<16) |(AGC_burs_reg[0]<<8) | NecAddr_reg[5] );
	writel( reg_val,  ir_dev->ir_reg+SATURN_ISO_IRTX_FIFO_OFF);

	reg_val = ((NecAddr_reg[4]<<24) | (NecAddr_reg[3]<<16) |(NecAddr_reg[2]<<8) | NecAddr_reg[1] );
	writel( reg_val,  ir_dev->ir_reg+SATURN_ISO_IRTX_FIFO_OFF);

	reg_val = ((NecAddr_reg[0]<<24) | (NecCmd_reg[5]<<16) |(NecCmd_reg[4]<<8) | NecCmd_reg[3] );
	writel( reg_val,  ir_dev->ir_reg+SATURN_ISO_IRTX_FIFO_OFF);

	reg_val = ((NecCmd_reg[2]<<24) | (NecCmd_reg[1]<<16) |(NecCmd_reg[0]<<8) | NecCmdEnd_reg );
	writel( reg_val,  ir_dev->ir_reg+SATURN_ISO_IRTX_FIFO_OFF);

	return 0;
}

static int venus_irtx_send_start(unsigned int scancode, unsigned int customer_code)
{
	unsigned int regValue;

	if (ir_dev->irtx_protocol== NEC_TX) {
		if ( -1 == venus_irtx_nec_send(scancode, customer_code)) {
			RTK_debug(KERN_ALERT " %s  %s  %d \n" ,__FILE__ ,__FUNCTION__, __LINE__);
			return -1;
		}
	} else {
		RTK_debug(KERN_ALERT " %s  %s  %d \n" ,__FILE__ ,__FUNCTION__, __LINE__);
		return -1;
	}
	regValue = readl(ir_dev->ir_reg+SATURN_ISO_IRTX_CFG_OFF);  //get irtx_cfg
	writel(0x80000711, ir_dev->ir_reg+SATURN_ISO_IRTX_CFG_OFF);  //start Tx

	regValue = readl(ir_dev->ir_reg+SATURN_ISO_IRTX_CFG_OFF);  //get irtx_cfg

	return 0;
}

int venus_irtx_send(unsigned int keycode)
{
	unsigned int scancode;
	unsigned int scancode_msk;

	if (-1 == venus_irtx_getscancode(keycode, &scancode, &scancode_msk)) {
		RTK_debug(KERN_ALERT " %s  %s  %d  venus_irtx_getscancode fail \n" ,__FILE__ ,__FUNCTION__, __LINE__);
		return -1;
	}

	if (-1 == venus_irtx_send_start(scancode, IRTX_CUSTOMER_CODE)) {
		RTK_debug(KERN_ALERT " %s  %s  %d _venus_irtx_send_start fail\n" ,__FILE__ ,__FUNCTION__, __LINE__);
		return -1;
	}

	return 0;
}

int venus_irtx_set_wake_up_keys(u32 keycode, u32* p_wake_up_keys)
{
	unsigned int i, j;
	struct RTK119X_ipc_shm_ir *p_table =(struct RTK119X_ipc_shm_ir*)p_wake_up_keys;

	if((!p_wake_up_keys)) {
		RTK_debug(KERN_ERR "[ERR] venus_ir : p_wake_up_keys pointer is NULL\n");
		return -1;
	}

	p_table->dev_count = htonl(ir_dev->irda_dev_count);

	for(j=0; j<ir_dev->irda_dev_count; j++) {
		p_table-> key_tbl[j].protocol = htonl(ir_dev->ir_protocol);
		p_table-> key_tbl[j].ir_scancode_mask = htonl(ir_dev->p_rtk_key_table[j].scancode_msk);
		p_table-> key_tbl[j].ir_cus_mask = htonl(ir_dev->p_rtk_key_table[j].custcode_msk);
		p_table-> key_tbl[j].ir_cus_code = htonl(ir_dev->p_rtk_key_table[j].cust_code);

		for (i = 0; i < ir_dev->p_rtk_key_table[j].size; i++) {
			if (keycode == ir_dev->p_rtk_key_table[j].keys[i].keycode) {
				p_table->key_tbl[j].ir_wakeup_scancode = htonl(ir_dev->p_rtk_key_table[j].keys[i].scancode);
				break;
			}
		}
	}
	p_table-> RTK119X_ipc_shm_ir_magic = htonl(0x49525641); //IRVA(IR Version A)

	return 0;
}

static bool venus_ir_checkkeycode(struct user_key_data *pUserKeys, unsigned int keycode)
{
	int i;
	for (i = 0; i < pUserKeys->key_table.size; i++) {
		if (pUserKeys->key_table.keys[i].keycode == keycode) {
			return true;
		}
	}
	return false;
}

static void venus_ir_get_user_data(uint32_t message, struct user_key_data **ppUsrData, u32 *pCode)
{
	if ((message & 0x0000ffff) == RTK_MK5_CUSTOMER_CODE) {
		*ppUsrData = &rtk_mk5_user_keys;
		*pCode = (message & 0x00ff0000)>>16;
	} else if ((message & 0x0000ffff) == YK_54LU_CUSTOMER_CODE) {
		*ppUsrData = &yk_54lu_user_keys;
		*pCode = (message & 0xffff0000)>>16;
	} else if ((message & 0x0000ffff) == RTK_MK5_2_CUSTOMER_CODE) {
		*ppUsrData = &rtk_mk5_2_user_keys;
		*pCode = (message & 0x00ff0000)>>16;
	} else if ((message & 0x0000ffff) == LIBRA_MS_CUSTOMER_CODE) {
		*ppUsrData = &libra_ms_user_keys;
		*pCode = (message & 0x00ff0000)>>16;
	} else if ((message & 0x0000ffff) == JAECS_T118_CUSTOMER_CODE) {
		*ppUsrData = &jaecs_t118_user_keys;
		*pCode = (message & 0x00ff0000)>>16;
	} else if ((message & 0x0000ffff) == RTK_MK3_CUSTOMER_CODE) {
		*ppUsrData = &rtk_mk3_user_keys;
		*pCode = (message & 0x00ff0000)>>16;
	} else if ((message & 0x0000ffff) == YK_70LB_CUSTOMER_CODE) {
		*ppUsrData = &yk_70lb_user_keys;
		*pCode = (message & 0xffff0000)>>16;
	} else if ((message & 0x0000ffff) == RTK_MK4_CUSTOMER_CODE) {
		*ppUsrData = &rtk_mk4_user_keys;
		*pCode = (message & 0x00ff0000)>>16;
	} else if ((message & 0x0000ffff) == NETG_MS_CUSTOMER_CODE) {
		*ppUsrData = &netg_ms_user_keys;
		*pCode = (message & 0x00007c0)>>6;
	} else {
		*ppUsrData = NULL;
		*pCode = 0;
	}
}

int venus_ir_setkeycode(unsigned int scancode, unsigned int new_keycode, unsigned int *p_old_keycode, unsigned long *keybit_addr)
{
	int i;
	int OldKeyCode = KEY_CNT;
	struct user_key_data *pUsrData;
	unsigned int ir_scancode;

	venus_ir_get_user_data(scancode, &pUsrData, &ir_scancode);
	if(pUsrData == NULL) {
		RTK_debug(KERN_ALERT "user keys not found for %d", scancode);
		return 0;
	}
	bUseUserTable = true;

	for(i=0; i<pUsrData->key_table.size; i++) {
		if(ir_scancode == pUsrData->key_table.keys[i].scancode) {
			//mapping found, set to new keycode
			OldKeyCode = pUsrData->key_table.keys[i].keycode;
			pUsrData->key_table.keys[i].keycode = new_keycode;
			break;
		}
	}

	if(OldKeyCode != KEY_CNT) {
		__clear_bit(OldKeyCode, keybit_addr);
		__set_bit(new_keycode, keybit_addr);

 		if( venus_ir_checkkeycode(&rtk_mk5_user_keys, OldKeyCode) ||
			venus_ir_checkkeycode(&yk_54lu_user_keys, OldKeyCode) ||
			venus_ir_checkkeycode(&rtk_mk5_2_user_keys , OldKeyCode) ||
			venus_ir_checkkeycode(&libra_ms_user_keys, OldKeyCode) ||
			venus_ir_checkkeycode(&jaecs_t118_user_keys, OldKeyCode) ||
			venus_ir_checkkeycode(&rtk_mk3_user_keys, OldKeyCode) ||
			venus_ir_checkkeycode(&yk_70lb_user_keys, OldKeyCode) ||
			venus_ir_checkkeycode(&rtk_mk4_user_keys, OldKeyCode) ||
			venus_ir_checkkeycode(&netg_ms_user_keys, OldKeyCode) ||
			false ) {
			__set_bit(OldKeyCode, keybit_addr);
		}
		*p_old_keycode = OldKeyCode;
		return 0;
	}

	if(pUsrData->key_table.size == pUsrData->max_size) {
		//realloc for key table
		struct venus_key *pNew;

		pNew = (struct venus_key *)krealloc(pUsrData->key_table.keys,
			                                (pUsrData->max_size+10) * sizeof(struct venus_key),
			                                GFP_ATOMIC);
		if(!pNew)
			return -ENOMEM;

		pUsrData->key_table.keys = pNew;
		pUsrData->max_size+=10;

		if(pUsrData->key_table.size == 0) {
			// first time set, disable default key map, clear keybit table
			memset(keybit_addr, 0, Input_Keybit_Length);
		}
	}

	pUsrData->key_table.keys[pUsrData->key_table.size].scancode = ir_scancode;
	pUsrData->key_table.keys[pUsrData->key_table.size].keycode = new_keycode;
	pUsrData->key_table.size++;
	__set_bit(new_keycode, keybit_addr);

	*p_old_keycode = new_keycode;

	return 0;
}

int venus_ir_getkeycode(unsigned int scancode, unsigned int *p_keycode)
{
	struct user_key_data *pUsrData;
	int i;
	unsigned int ir_scancode;

	if(bUseUserTable == false)
		return -1;

	venus_ir_get_user_data(scancode, &pUsrData, &ir_scancode);
	if(pUsrData == NULL)
		return -1;

	for(i=0; i<pUsrData->key_table.size; i++) {
		if(ir_scancode == pUsrData->key_table.keys[i].scancode) {
			//mapping found
			*p_keycode = pUsrData->key_table.keys[i].keycode;
			RTK_debug(KERN_ALERT "[venus_ir_getkeycode]scancode=%d, keycode=%d", ir_scancode, pUsrData->key_table.keys[i].keycode);
			return 0;
		}
	}
	return -1;
}

ssize_t irda_show_ir_replace_table(struct device *dev, struct device_attribute *attr, char *buf);
ssize_t irda_store_ir_replace_table(struct device *dev, struct device_attribute *attr,
			 const char *buf, size_t count);

static DEVICE_ATTR(ir_replace_table, S_IRUGO | S_IWUSR, irda_show_ir_replace_table, irda_store_ir_replace_table);

static struct attribute *irda_dev_attrs[] = {
	&dev_attr_ir_replace_table.attr,
	NULL,
};

static struct attribute_group irda_attr_group = {
	.attrs		= irda_dev_attrs,
};

ssize_t irda_show_ir_replace_table(struct device *dev, struct device_attribute *attr, char *buf)
{
//	struct platform_device *pdev = to_platform_device(dev);

	if( ir_dev->p_rtk_key_table==NULL)
		return sprintf(buf, "IR_ERR: key table is not exist!\n");

	return sprintf(buf, "table size=%d\n", ir_dev->p_rtk_key_table->size);
}

ssize_t irda_store_ir_replace_table(struct device *dev, struct device_attribute *attr,
			 const char *buf, size_t count)
{
//	struct platform_device *pdev = to_platform_device(dev);
	char filename[64]={0};
	char buffer[768]={0};
	struct file *filep;
	char *token, *variable_token, *cur = buffer;
	unsigned int idx=0;
	unsigned int keymap_size = ir_dev->p_rtk_key_table->size;
	struct venus_key_table* new_p_rtk_key_table;

	if(buf ==NULL || count < 1) {
		RTK_debug(KERN_ALERT "IR_ERR : irda_store_ir_replace_table	====  buffer is null! \n");
		return count;
	}
	sscanf(buf, "%s", filename);

	filep=filp_open(filename, O_RDONLY, 0);
	if(IS_ERR(filep)){
		RTK_debug(KERN_ALERT "irda_store_ir_replace_table: open filename=[%s] fail!! \n", filename);
		return count;
	}

	if (kernel_read(filep, 0, buffer, 768) < 0) {
		RTK_debug(KERN_ALERT "irda_store_ir_replace_table: read file error?\n");
		goto irda_error1;
	}

	// NOTE : parsing file 
	new_p_rtk_key_table = (struct venus_key_table*)kzalloc(sizeof(struct venus_key_table), GFP_KERNEL);

	while ( (token = strsep(&cur, "\n"))) {
		variable_token = strsep(&token, "=");

		if( strcmp(variable_token, IR_STR_IRRX_PROTOCOL) == 0 )
			sscanf(token, "%d", &ir_dev->ir_protocol);
		else if ( strcmp(variable_token, IR_STR_CUST_CODE) == 0 )
			sscanf(token,"0x%X",  &(new_p_rtk_key_table->cust_code));
		else if ( strcmp(variable_token, IR_STR_SCANCODE_MSK) == 0 )
			sscanf(token,"0x%08X",  &(new_p_rtk_key_table->scancode_msk));
		else if ( strcmp(variable_token, IR_STR_CUSTCODE_MSK) == 0 )
			sscanf(token,"0x%08X",  &(new_p_rtk_key_table->custcode_msk));
		else if ( strcmp(variable_token, IR_STR_RET_IR_DPIR) == 0 )
			sscanf(token,"%d", &ir_dev->dpir_val);
		else if ( strcmp(variable_token, IR_STR_KEYMAP_SIZE) == 0 )
			sscanf(token,"%d", &keymap_size);
 		else if ( strcmp(variable_token, IR_STR_KEYMAP_TBL) == 0 ) {
			if( keymap_size<1 ) {
				RTK_debug(KERN_ALERT " : [ERR] IR_STR_KEYMAP_TBL , keymap_size=%d is not legal!! return! \n", keymap_size);
				kfree(new_p_rtk_key_table);
				goto irda_error1;
			}
			new_p_rtk_key_table->keys = (struct venus_key*)kzalloc(sizeof(struct venus_key) * (keymap_size+1), GFP_KERNEL);
			new_p_rtk_key_table->size = keymap_size;
			idx=0;
			while ((token = strsep(&cur, "\n")) && (idx < keymap_size)) {
				variable_token = strsep(&token, "=");

				if( strlen(variable_token) > 1 ) {
					sscanf(variable_token,"0x%X", 	&(new_p_rtk_key_table->keys[idx].scancode));
					sscanf(token,		"%d",  		&(new_p_rtk_key_table->keys[idx].keycode));
					idx++;
				}
			}
			break;
		}
	}
	if( ir_dev->p_rtk_key_table ) {
		kfree(ir_dev->p_rtk_key_table);
		ir_dev->p_rtk_key_table = new_p_rtk_key_table;
	}
	/* Hardware Registers Initialization */
	venus_ir_input_exit();
	rtk_ir_init();
	venus_ir_input_init();

irda_error1:
	filp_close(filep, 0);
	return count;
}

static int get_bit_cnt(int *bit_num, int *sample_cnt, unsigned long *sample, int polar)
{
	int bit_cnt = 0;

	if(polar != 0 && polar != 1)
		return -1;

	while(1) {
		if((*bit_num) == 0) {
			if(readl(ir_dev->ir_reg+SATURN_ISO_IR_RAW_WL_OFF) > 0) {
				(*sample_cnt)++;
				(*sample) = readl(ir_dev->ir_reg+SATURN_ISO_IR_RAW_FF_OFF);
				(*bit_num) = 32;
				continue;
			} else
				break;
		} else {
			if(test_bit((*bit_num)-1, sample)) {
				if(polar == 1) {
					(*bit_num)--;
					bit_cnt++;
				} else
					break;
			} else {
				if(polar == 0) {
					(*bit_num)--;
					bit_cnt++;
				} else
					break;
			}
		}
	}

	return bit_cnt;
}

static int raw_nec_decoder(int *dataRepeat)
{
	int i;
	int raw_bit_anchor = 32;
	int raw_bit_sample_cnt = 1;
	unsigned long raw_bit_sample;
	int symbol = 0;
	int length_low, length_high;
	unsigned long stopCnt;
	static unsigned long lastSymbol = 0;
	unsigned int srValue = readl(ir_dev->ir_reg+SATURN_ISO_IR_SR_OFF);

	if(srValue & 0xc) {
		writel((srValue & (~0xf)), ir_dev->ir_reg+SATURN_ISO_IR_SR_OFF); /* clear raw_fifo_val and raw_fifo_ov */
	}else{
		return symbol;
	}

	raw_bit_sample = readl(ir_dev->ir_reg+SATURN_ISO_IR_RAW_FF_OFF);

	// [decode] PREMBLE (High for 8ms / Low for 4ms)
	length_high = get_bit_cnt(&raw_bit_anchor, &raw_bit_sample_cnt, &raw_bit_sample, 0);
	length_low = get_bit_cnt(&raw_bit_anchor, &raw_bit_sample_cnt, &raw_bit_sample, 1);

	if(length_high >= 40 && length_low <= 12) {	//repeat : 9ms burst and 2.25ms space
		symbol = lastSymbol;
		goto get_symbol;
	}
	else if(length_high < 40 || length_low < 20)	//not AGC burst : 9ms burst and 4.5ms space
		goto sleep;
	else

	for(i = 0 ; i < 32 ; i++) {			//one transmit is 32 bit (8 addr, 8 invert addr, 8 cmd, 8 invert cmd)
		int length_high, length_low;

		length_high = get_bit_cnt(&raw_bit_anchor, &raw_bit_sample_cnt, &raw_bit_sample, 0);
		length_low = get_bit_cnt(&raw_bit_anchor, &raw_bit_sample_cnt, &raw_bit_sample, 1);

		if(length_high >= 2) {
			if(length_low > 10) { 		// Repeat : 560us burst and space until 110ms
				*dataRepeat = 1;
				symbol = lastSymbol;
				break;
			}
			else if(length_low >= 7)	// 1.68 ms space to logical "1"
				symbol |= (0x1 << i);
			else if(length_low >= 2)	// 560us space to logical "0"
				symbol &= (~(0x1 << i));
			else
				goto sleep;

		}
		else
			goto sleep;
	}

get_symbol:

	RTK_debug(KERN_DEBUG "Venus IR: [%d = %08X] is detected.\n", symbol, symbol);

	if(lastSymbol == symbol) {
		*dataRepeat = 1;
	}
	else {
		*dataRepeat = 0;
		lastSymbol = symbol;
	}

sleep:
	stopCnt = readl(ir_dev->ir_reg+SATURN_ISO_IR_RAW_SAMPLE_TIME_OFF) + 0xc8;

	// prepare to stop sampling .. at least 150 (= 30ms), fifo_thred = 8
	writel(0x03000050 | (stopCnt << 8),  ir_dev->ir_reg+SATURN_ISO_IR_RAW_CTRL_OFF);

	return symbol;
}

static unsigned int raw_rc6_decoder(int *dataRepeat)
{
	int i,j;
	int raw_bit_anchor = 32;
	int raw_bit_sample_cnt = 1;
	unsigned long raw_bit_sample;
	int symbol = 0x8000;
	int length_low, length_high;
	int length_get = 0;
	unsigned long stopCnt;
	static unsigned long lastSymbol = 0;
	unsigned char check_rc6_tail = 0;

	unsigned int srValue = readl(ir_dev->ir_reg+SATURN_ISO_IR_SR_OFF);

	if(srValue & 0xc) {
		writel((srValue & (~0xf)), ir_dev->ir_reg+SATURN_ISO_IR_SR_OFF); /* clear raw_fifo_val and raw_fifo_ov */
	}else{
		return symbol;
	}


	raw_bit_sample = readl(ir_dev->ir_reg+SATURN_ISO_IR_RAW_FF_OFF);

	// [decode] PREMBLE (High for 2.6ms / Low for 0.8ms)
	// 1 bit == 0.1ms
	length_high = get_bit_cnt(&raw_bit_anchor, &raw_bit_sample_cnt, &raw_bit_sample, 0);
	length_low = get_bit_cnt(&raw_bit_anchor, &raw_bit_sample_cnt, &raw_bit_sample, 1);

	/* check RC6 LS */
	if (!(length_high >= 26 && length_high <= 29 && length_low >= 8 && length_low <= 10)) {	//leader pulse 2.666ms + 889us space
		goto sleep;
	}
	else{
		int bit_value = 0;
		int delay_ct = 0;
		int length_warn = 0;
		length_high = 0;
		length_low = 0;
		symbol = 0;

		for (i = 0; i < 21; i++) { /* get symbol */	//SB+MB2+MB1+MB0+TR+A7+A6+A5+A4+A3+A2+A1+A0+C7+C6+C5+C4+C3+C2+C1+C0
			delay_ct = 0;
			while (readl(ir_dev->ir_reg+SATURN_ISO_IR_RAW_WL_OFF) < 4 && delay_ct <= 12) {  /* Check HW water level(32 word) out of range */
				delay_ct ++;
			}
			if (readl(ir_dev->ir_reg+SATURN_ISO_IR_RAW_WL_OFF) >= 26) { /* Check HW water level(32 word) out of range */
				goto sleep;
			}
			if (length_high == 0 && length_low == 0) {  /* PreGet symbol : notice */
				if (length_get == 1)  { /* get high then low */
					length_low = get_bit_cnt(&raw_bit_anchor, &raw_bit_sample_cnt, &raw_bit_sample, 1);
					length_high = get_bit_cnt(&raw_bit_anchor, &raw_bit_sample_cnt, &raw_bit_sample, 0);
					bit_value = 0;
				} else { /* get low then high */
					length_high = get_bit_cnt(&raw_bit_anchor, &raw_bit_sample_cnt, &raw_bit_sample, 0);
					length_low = get_bit_cnt(&raw_bit_anchor, &raw_bit_sample_cnt, &raw_bit_sample, 1);
					bit_value = 1;
				}
			} else {
				if (length_high == 0) {  /* just get low (two high is successive then only get low) */
					length_high = get_bit_cnt(&raw_bit_anchor, &raw_bit_sample_cnt, &raw_bit_sample, 0);
					length_get = 1;
					bit_value = 0;
				} else if(length_low == 0) {  /* just get high (two low is successive then only get high) */
					length_low = get_bit_cnt(&raw_bit_anchor, &raw_bit_sample_cnt, &raw_bit_sample, 1);
					length_get = 0;
					bit_value = 1;
				}
			}
			if(length_high <= 3 || length_low <= 3) {
				length_warn++;
			}
			if (i==0) {  /* bypass start bit */
				//symbol |= (0x1 << i);
			} else {  /* assign symbol bit */
				symbol |= (bit_value << (19-(i-1)));
			}

			if (i ==4) { /* handle TR */
				if (length_high >= 11 && length_high <= 16)	{ 	/* Check if length_high continuous (valid range : 7~16 (100us unit)) */
					length_high -= 8;
				} else if (length_high >= 17)  /* Check if length_low continuous (valid range: 7~16 (100us unit)) */
					goto get_symbol;
				else
					length_high = 0;

				if (length_low >= 11 && length_low <= 16) {
					length_low -= 8;
				} else if (length_low >= 17)
					goto get_symbol;
				else
					length_low = 0;
			} else {
				if (length_high >= 7 && length_high <= 16) { /* Check if length_high continuous(valid range : 7~16 (100us unit)) */
					if (length_high >= 12 && length_low >= 7 )  /* convert length_high is continuous */
						length_high -= 8;
					else  /* Normal length_high is not continuous */
						length_high -= 4;
						/*bit_value=1;*/
				} else if (length_high >= 17) /*Check if length_low continuous(valid range: 7~16 (100us unit)) */
					goto get_symbol;
				else
					length_high = 0;

				if (length_low >= 7 && length_low <= 16) {
					if(length_low >= 12 && length_high >= 7)
						length_low -= 8;
					else
						length_low -= 4;
						/*bit_value = 0;*/
				} else if (length_low >= 17)
					goto get_symbol;
				else
					length_low = 0;
			}
		}
		if(length_warn >= 4)
			goto sleep;
	}

get_symbol:
	check_rc6_tail = 1;
	for (j = 0; j < 2; j++) {
		raw_bit_sample = readl(ir_dev->ir_reg+SATURN_ISO_IR_RAW_FF_OFF);

		if(raw_bit_sample != 0xFFFFFFFF && raw_bit_sample != 0x0) {
			symbol = 0x8000;
			goto sleep;
		}
	}

	if(i < 20)  // symbol bits >=20
		goto sleep;

	if (symbol == lastSymbol) {
		*dataRepeat = 1;
	} else {
		*dataRepeat = 0;
		lastSymbol = symbol;
    }

	if (((symbol & 0x0000ffff) == 0x000c) && (*dataRepeat == 1)) // power key don't response repeat
		goto sleep;

sleep:
	if(check_rc6_tail == 0) {
		for (j = 0; j < 2; j++) {
			raw_bit_sample = readl(ir_dev->ir_reg+SATURN_ISO_IR_RAW_FF_OFF);

			if(raw_bit_sample != 0xFFFFFFFF && raw_bit_sample != 0x0) {
				symbol = 0x8000;
				goto sleep1;
			}
		}
	}

sleep1:
	stopCnt = readl(ir_dev->ir_reg+SATURN_ISO_IR_RAW_SAMPLE_TIME_OFF) + 0xc8;
	// prepare to stop sampling ..
	writel(0x03000048 | (stopCnt << 8), ir_dev->ir_reg+SATURN_ISO_IR_RAW_CTRL_OFF); // stop sampling for at least 150 (= 30ms), fifo_thred = 8

	return symbol;
}

static int get_raw_data(void)
{
	unsigned int srValue;
	int regVal = -1;
	int len;
	int i;
	unsigned int data[FIFO_DEPTH*4];

	srValue = readl(ir_dev->ir_reg+SATURN_ISO_IR_SR_OFF);
	if(srValue & 0xc)
		writel((srValue & (~0xf)), ir_dev->ir_reg+SATURN_ISO_IR_SR_OFF);
	else
		return regVal;

#ifdef REPEAT_MODE_SELFTEST
	len = kfifo_avail(&ir_cdev->ir_txfifo);
//	if(len >=900)
//		printk(KERN_ERR "len = %d\n", len);
	if(len < 512 && start_tx==0) {
		srValue = readl(ir_dev->ir_reg+SATURN_ISO_IRTX_CFG_OFF);
		if(!(srValue & 0x1)) {
			kfifo_out(&ir_cdev->ir_txfifo, data, FIFO_DEPTH*4);
			for(i=0; i<FIFO_DEPTH; i++)
				writel( data[i], ir_dev->ir_reg+SATURN_ISO_IRTX_FIFO_OFF);

			writel(0x80000511, ir_dev->ir_reg+SATURN_ISO_IRTX_CFG_OFF);  //start Tx
		}
		start_tx=1;
	}

	len = readl(ir_dev->ir_reg+SATURN_ISO_IR_RAW_WL_OFF);
	for(i=0; i<len; i++)
		data[i] = ~(readl(ir_dev->ir_reg+SATURN_ISO_IR_RAW_FF_OFF));

	kfifo_in(&ir_cdev->ir_txfifo, data, len*4);
#else
	len = readl(ir_dev->ir_reg+SATURN_ISO_IR_RAW_WL_OFF);
	for(i=0; i<len; i++)
		data[i] = ~(readl(ir_dev->ir_reg+SATURN_ISO_IR_RAW_FF_OFF));

	kfifo_in(&ir_cdev->ir_rxfifo, data, len*4);
//	while(readl(ir_dev->ir_reg+SATURN_ISO_IR_RAW_WL_OFF)>0) {
//		srValue = readl(ir_dev->ir_reg+SATURN_ISO_IR_RAW_FF_OFF);
//		kfifo_in(&ir_cdev->ir_rxfifo, &srValue, sizeof(unsigned int));
//	}
#endif
	return 0;
}

static int raw_data_parcer(void)
{
	unsigned int srValue;
	unsigned int irrp2Value = 0;
	int dataRepeat = 0;
	unsigned int fifoval[20];
	unsigned int fifolv=0;
	int i;
	int regVal = -1;
	int received = 0;

	srValue = readl(ir_dev->ir_reg+SATURN_ISO_IR_SR_OFF);
	if(srValue & 0xc)
		writel((srValue & (~0xf)), ir_dev->ir_reg+SATURN_ISO_IR_SR_OFF);
	else
		return regVal;
	
	while(readl(ir_dev->ir_reg+SATURN_ISO_IR_RAW_WL_OFF)>0) {
		fifoval[fifolv] = readl(ir_dev->ir_reg+SATURN_ISO_IR_RAW_FF_OFF);
		fifolv++;
	}

	for(i=0; i<ir_dev->irda_dev_count; i++) {
		if(ir_dev->pdec[i].raw_decoder != NULL)
			regVal = ir_dev->pdec[i].raw_decoder(&ir_dev->pdec[i], fifoval, fifolv);
		if(regVal>=0) {
	//		printk(KERN_ERR "decoder=%d, value=0x%x\n", i, regVal);
			received = repeat_key_handle(regVal, irrp2Value, dataRepeat);
			break;
		}
	}
	return received;
}

static irqreturn_t rtk_ir_isr(int irq, void *dev_id)
{
	unsigned int regValue;
	unsigned int irrp2Value = 0;
	int dataRepeat = 0;
	int received = 0;
	int txlen, i;

	regValue = readl(ir_dev->sys_reg + SATURN_ISO_ISR_OFF);
	if(ir_dev->ir_protocol == RAW_NEC) {
		debounce = 0;
		regValue = raw_nec_decoder(&dataRepeat);
		if(regValue == 0)
			goto CONTINUE_IRTX;
		if (dataRepeat == 0) {
			if (isKeypadTimerCreate) {
				isKeypadTimerCreate = false;
				venus_ir_input_report_end();
			}
			received = repeat_key_handle(regValue, irrp2Value, dataRepeat);
		}
	} else if(ir_dev->ir_protocol == RAW_RC6) {
		debounce = 0;
		regValue = raw_rc6_decoder(&dataRepeat);
		if(regValue == 0x8000)
			goto CONTINUE_IRTX;
		if (dataRepeat == 0) {
			if (isKeypadTimerCreate) {
				isKeypadTimerCreate = false;
				venus_ir_input_report_end();
			}
			received = repeat_key_handle(regValue, irrp2Value, dataRepeat);
		}	
	} else if(ir_dev->ir_protocol == RAW_DIRECTTV || ir_dev->ir_protocol == RAW_COMCAST || ir_dev->ir_protocol == RAW_MOTO || ir_dev->ir_protocol == RAW_CISCO) {
		received = raw_data_parcer();
	} else if(ir_dev->ir_protocol == REPEAT_MODE) {
		get_raw_data();
	} else {
		while(examine_ir_avail(&regValue, &irrp2Value, &dataRepeat) == 0) {
			if (dataRepeat == 0) {
				if (isKeypadTimerCreate) {
					isKeypadTimerCreate = false;
					venus_ir_input_report_end();
				}
				received = repeat_key_handle(regValue, irrp2Value, dataRepeat);
			} else if(isKeypadTimerCreate == false) {
				if((jiffies_to_msecs(jiffies)- ir_dev->lastRecvMs) > REPEAT_TIMEOUT) {
					printk(KERN_ERR "Ignore repeat key\n");
					received = 1;
				} else
					received = repeat_key_handle(regValue, irrp2Value, 0);
			}
		}
	}
	if(received == 1) {
		wake_up_interruptible(&ir_cdev->rx_wait);
	}
	mod_timer(&ir_dev->ir_keypad_timer, jiffies + msecs_to_jiffies(230));

CONTINUE_IRTX:
	regValue = readl(ir_dev->ir_reg+SATURN_ISO_IRTX_INT_ST_OFF);

	if(ir_dev->rpmode) {
		if(regValue & 0x2) {
			unsigned int data[FIFO_DEPTH];
			txlen = kfifo_len(&ir_cdev->ir_txfifo) / sizeof(unsigned int);
			if(txlen > (FIFO_DEPTH-1))
				txlen = FIFO_DEPTH - 1;
			if(txlen==0) {
				printk(KERN_ERR "[IRTX] no data in fifo\n");
				writel(0xf04, ir_dev->ir_reg+SATURN_ISO_IRTX_INT_EN_OFF);
			} else {
				kfifo_out(&ir_cdev->ir_txfifo, data, txlen * sizeof(unsigned int));
				for(i=0; i<txlen; i++)
					writel( data[i], ir_dev->ir_reg+SATURN_ISO_IRTX_FIFO_OFF);
			}
		}
		if (regValue & 0x4) {
			printk(KERN_ERR "[IRTX] stop tx 0x%x\n", regValue);
			rtk_irtx_enable(0);

			writel(0x80000000, ir_dev->ir_reg+SATURN_ISO_IRTX_FIFO_ST_OFF);	//irtx fifo reset
			writel(0x00000000, ir_dev->ir_reg+SATURN_ISO_IRTX_FIFO_ST_OFF);	//irtx fifo normal

			writel(0xf06, ir_dev->ir_reg+SATURN_ISO_IRTX_INT_EN_OFF);
			start_tx = 0;
		}
	} else {
		if(	regValue & 0x4) {	// IRTX_INT_ST: FIN_FLAG = 1
			unsigned int regTmpValue=0;

			rtk_irtx_enable(0);

			writel(0x80000000, ir_dev->ir_reg+SATURN_ISO_IRTX_FIFO_ST_OFF);	//irtx fifo reset
			writel(0x00000000, ir_dev->ir_reg+SATURN_ISO_IRTX_FIFO_ST_OFF);	//irtx fifo normal

			regTmpValue = readl(ir_dev->ir_reg+SATURN_ISO_IRTX_FIFO_ST_OFF);	//irtx fifo normal
		
			rtk_irrp_enable(1);
		}
	}
	
	regValue = readl(ir_dev->sys_reg + SATURN_ISO_ISR_OFF);
	
	return IRQ_HANDLED;
}

static struct of_device_id rtk119x_irda_ids[] = {
	{ .compatible = "Realtek,rtk-irda" },
	{ /* Sentinel */ },
};
MODULE_DEVICE_TABLE(of, rtk119x_irda_ids);

int rtk119x_ir_remove(struct platform_device * pdev)
{
	struct device *dev = &pdev->dev;

	del_timer_sync(&ir_dev->ir_wakeup_timer);

	writel(0x80000000, ir_dev->ir_reg+SATURN_ISO_IR_CR_OFF);

	free_irq(ir_dev->ir_irq, rtk_ir_isr);

	kfifo_free(&ir_cdev->ir_rxfifo);
	kfifo_free(&ir_cdev->ir_txfifo);

	device_destroy(ir_cdev->class, MKDEV(VENUS_IR_MAJOR,VENUS_IR_MINOR));
    class_destroy(ir_cdev->class);
	cdev_del(ir_cdev->cdev);
	unregister_chrdev_region(ir_cdev->dev_no, VENUS_IR_DEVICE_NUM);

	if(ir_dev->rpmode) {
		devm_kfree(dev, ir_dev->rpmode_rxfifo);
		devm_kfree(dev, ir_dev->rpmode_txfifo);
	}
	devm_kfree(dev, ir_cdev);
	devm_kfree(dev, ir_dev);

	return 0;
}

int rtk_ir_open(struct inode *inode, struct file *filp)
{
	if( (filp->f_flags & O_ACCMODE) == O_WRONLY )
		kfifo_reset(&ir_cdev->ir_txfifo);

	if( (filp->f_flags & O_ACCMODE) == O_RDONLY )
		kfifo_reset(&ir_cdev->ir_rxfifo);

	return 0;
}

ssize_t rtk_ir_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	char keycode[256]={0};
	unsigned int inputKeyCode=0;
	unsigned int w_cnt=0;
	int write_Count = count;
	int i;
	unsigned int reg, fiforet;

	if(ir_dev->rpmode) {
		write_Count = kfifo_avail(&ir_cdev->ir_txfifo);
		if(count < write_Count)
			write_Count = count;

		if(copy_from_user(ir_dev->rpmode_txfifo, buf, write_Count)) {
			RTK_debug(KERN_ALERT "venus_ir_write : copy from user error!\n");
			return -EFAULT;
		}

		reg = readl(ir_dev->ir_reg+SATURN_ISO_IRTX_CFG_OFF);
		if(!(reg & 0x1)) {
			if(write_Count <= FIFO_DEPTH*4) {
				for(i=0; i<(write_Count/4); i++)
					writel( *((int *)(ir_dev->rpmode_txfifo)+i), ir_dev->ir_reg+SATURN_ISO_IRTX_FIFO_OFF);

				writel(0x80000511, ir_dev->ir_reg+SATURN_ISO_IRTX_CFG_OFF);  //start Tx
			} else {
				for(i=0; i<FIFO_DEPTH; i++)
					writel( *((int *)(ir_dev->rpmode_txfifo)+i), ir_dev->ir_reg+SATURN_ISO_IRTX_FIFO_OFF);

				writel(0x80000511, ir_dev->ir_reg+SATURN_ISO_IRTX_CFG_OFF);  //start Tx
				fiforet = kfifo_in(&ir_cdev->ir_txfifo, ir_dev->rpmode_txfifo + FIFO_DEPTH*4, write_Count - FIFO_DEPTH*4);
			}
		} else {
			kfifo_in(&ir_cdev->ir_txfifo, ir_dev->rpmode_txfifo, write_Count);
		}
		return write_Count;
	}

	if(count >= 256 || count < 1) {
		RTK_debug(KERN_ALERT "venus_ir_write : Error! count = %d \n ", (int)count);
		return -EFAULT;
	}
	w_cnt=(count-1);
	if (copy_from_user(keycode, buf, w_cnt*sizeof(char))) {
		RTK_debug(KERN_ALERT "venus_ir_write : copy from user error!\n");
		return -EFAULT;
	}

	rtk_irrp_enable(0);
	sscanf(keycode,"%d", &inputKeyCode);

	if( venus_irtx_send(inputKeyCode)!= 0 ) {
		RTK_debug(KERN_ALERT "venus_ir_write : venus_irtx_send fail! \n ");
		rtk_irrp_enable(1);
		return -EFAULT;
	}

	return count;
}

ssize_t rtk_ir_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	int uintBuf;
	int i, readCount = count;
	int len;

restart:
	if(wait_event_interruptible(ir_cdev->rx_wait, kfifo_len(&ir_cdev->ir_rxfifo) > 0) != 0) {
		if (try_to_freeze())
			goto restart;
	}

	if(kfifo_len(&ir_cdev->ir_rxfifo) < count)
		readCount = kfifo_len(&ir_cdev->ir_rxfifo);

	if(ir_dev->rpmode) {
		len = kfifo_out(&ir_cdev->ir_rxfifo, ir_dev->rpmode_rxfifo, readCount);
		readCount = len;
		if(copy_to_user(buf, ir_dev->rpmode_rxfifo, len)) {
			RTK_debug(KERN_ALERT "copy fail\n");
			return -EFAULT;
		}
	} else {
		for(i = 0 ; i < readCount ; i += len) {
			len = kfifo_out(&ir_cdev->ir_rxfifo, &uintBuf, sizeof(unsigned int));
			if (copy_to_user(buf, &uintBuf, len)) {
				RTK_debug(KERN_ALERT "copy fail\n");
				return -EFAULT;
			}
		}
	}

	return readCount;
}

long rtk_ir_compat_ioctl(struct file* file,unsigned int cmd, unsigned long arg)
{
	if (!file->f_op->unlocked_ioctl)
		return -ENOTTY;
	switch (cmd) {
		case VENUS_IR_IOC_SET_IRPSR:
			return file->f_op->unlocked_ioctl(file, cmd,(unsigned long)compat_ptr(arg));
		case VENUS_IR_IOC_SET_IRPER:
			return file->f_op->unlocked_ioctl(file, cmd,(unsigned long)compat_ptr(arg));
		case VENUS_IR_IOC_SET_IRSF:
			return file->f_op->unlocked_ioctl(file, cmd,(unsigned long)compat_ptr(arg));
		case VENUS_IR_IOC_SET_IRCR:
			return file->f_op->unlocked_ioctl(file, cmd,(unsigned long)compat_ptr(arg));
		case VENUS_IR_IOC_SET_IRIOTCDP:
			return file->f_op->unlocked_ioctl(file, cmd,(unsigned long)compat_ptr(arg));
		case VENUS_IR_IOC_SET_PROTOCOL:
			return file->f_op->unlocked_ioctl(file, cmd,(unsigned long)compat_ptr(arg));
		case VENUS_IR_IOC_FLUSH_IRRP:
			return file->f_op->unlocked_ioctl(file, cmd,(unsigned long)compat_ptr(arg));
		case VENUS_IR_IOC_SET_DEBOUNCE:
			return file->f_op->unlocked_ioctl(file, cmd,(unsigned long)compat_ptr(arg));
		case VENUS_IR_IOC_SET_FIRST_REPEAT_DELAY:
			return file->f_op->unlocked_ioctl(file, cmd,(unsigned long)compat_ptr(arg));
		case VENUS_IR_IOC_SET_DRIVER_MODE:
			return file->f_op->unlocked_ioctl(file, cmd,(unsigned long)compat_ptr(arg));
		case VENUS_IRTX_IOC_SET_TX_TABLE:
			return file->f_op->unlocked_ioctl(file, cmd,(unsigned long)compat_ptr(arg));
		case VENUS_IRRX_RAW_STOP:
			return file->f_op->unlocked_ioctl(file, cmd,(unsigned long)compat_ptr(arg));
		default:
			return -EFAULT;
	}
}

long rtk_ir_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {
	int retval = 0;
	switch(cmd) {
		case VENUS_IR_IOC_SET_IRPSR:
			writel(arg, ir_dev->ir_reg+SATURN_ISO_IR_PSR_OFF);
			break;
		case VENUS_IR_IOC_SET_IRPER:
			writel(arg, ir_dev->ir_reg+SATURN_ISO_IR_PER_OFF);
			break;
		case VENUS_IR_IOC_SET_IRSF:
			writel(arg, ir_dev->ir_reg+SATURN_ISO_IR_SF_OFF);
			break;
		case VENUS_IR_IOC_SET_IRCR:
			writel(arg, ir_dev->ir_reg+SATURN_ISO_IR_CR_OFF);
			break;
		case VENUS_IR_IOC_SET_IRIOTCDP:
			writel(arg, ir_dev->ir_reg+SATURN_ISO_IR_DPIR_OFF);
			break;
		case VENUS_IR_IOC_SET_PROTOCOL:
			ir_dev->ir_protocol = (int)arg;
		case VENUS_IR_IOC_FLUSH_IRRP:
			if((retval = rtk_ir_init()) == 0)
				kfifo_reset(&ir_cdev->ir_rxfifo);
			break;
		case VENUS_IR_IOC_SET_DEBOUNCE:
			RPRepeatIsHandling = true;
			debounce = (unsigned int)arg;
			break;
		case VENUS_IR_IOC_SET_FIRST_REPEAT_DELAY:
#ifdef DELAY_FIRST_REPEAT
			firstRepeatInterval = (unsigned int)arg;
#else
			retval = -EPERM;
#endif
			break;
		case VENUS_IR_IOC_SET_DRIVER_MODE:
			if(((unsigned int)arg) >= 2)
				retval = -EFAULT;
			else {
				kfifo_reset(&ir_cdev->ir_rxfifo);
				driver_mode = (unsigned int)arg;
			}
			break;

		case VENUS_IRTX_IOC_SET_TX_TABLE:
			if (copy_from_user(&IRTx_KeycodeTable, ((void __user *)arg), sizeof(struct Venus_IRTx_KeycodeTable)))
				retval =  -EFAULT;
			IRTx_KeycodeTableEnable = 1;
			break;

		case VENUS_IRRX_RAW_STOP:
			disable_irq(ir_dev->ir_irq);
			reset_control_assert(ir_dev->rstc);
			kfifo_reset(&ir_cdev->ir_txfifo);
			kfifo_reset(&ir_cdev->ir_rxfifo);
			start_tx = 0;
			reset_control_deassert(ir_dev->rstc);
			rtk_ir_init();
			enable_irq(ir_dev->ir_irq);
			break;

		default:
			retval = -ENOIOCTLCMD;
	}

	return retval;
}

unsigned int rtk_ir_poll(struct file *filp, poll_table *wait)
{
	poll_wait(filp, &ir_cdev->rx_wait, wait);

	if(kfifo_len(&ir_cdev->ir_rxfifo) > 0)
		return POLLIN | POLLRDNORM;
	else
		return 0;
}

struct file_operations venus_ir_fops = {
	.owner		=	THIS_MODULE,
	.open		=	rtk_ir_open,
	.read		=	rtk_ir_read,
	.write		=	rtk_ir_write,
	.poll		=	rtk_ir_poll,
	.unlocked_ioctl	=	rtk_ir_ioctl,
	.compat_ioctl = rtk_ir_compat_ioctl,
};

void venus_ir_set_keybit(unsigned long *addr)
{
	int i, j;
	for (j = 0; j < ir_dev->irda_dev_count; j++) {
		for (i = 0; i <ir_dev->p_rtk_key_table[j].size; i++) {
			if(ir_dev->p_rtk_key_table[j].keys[i].scancode != 0xEEEE)		//not EV_REL type
				set_bit(ir_dev->p_rtk_key_table[j].keys[i].keycode, addr);
		}
	}
}

void venus_ir_set_relbit(unsigned long *addr)
{
	int i, j;
	for (j = 0; j < ir_dev->irda_dev_count; j++) {
		for (i = 0; i <ir_dev->p_rtk_key_table[j].size; i++) {
			if(ir_dev->p_rtk_key_table[j].keys[i].scancode == 0xEEEE)		//is EV_REL type
				set_bit(ir_dev->p_rtk_key_table[j].keys[i].keycode, addr);
		}
	}
}

static void get_decoder_func(struct ir_sw_decoder *pdec, unsigned int protocol)
{
	switch(protocol) {
		case RAW_DIRECTTV:
			pdec->raw_decoder = raw_directtv_decoder;
		break;
		case RAW_COMCAST:
			pdec->raw_decoder = raw_comcast_decoder;
		break;
		case RAW_MOTO:
			pdec->raw_decoder = raw_moto_decoder;
		break;
		case RAW_CISCO:
			pdec->raw_decoder = raw_cisco_decoder;
		break;
		default:
			pdec->raw_decoder = NULL;
	}
}

int rtk119x_ir_multiple_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct device_node *irda_node;
	const char* irda_dev[]={"irda0","irda1","irda2","irda3","irda4"};

	int i, j;
   	const u32 *prop;
    int size;
    int err;
	unsigned int cust_code, scancode_msk, custcode_msk;

	if (of_property_read_u32(dev->of_node, "Realtek,irda_dev_count", &ir_dev->irda_dev_count)) {
		RTK_debug(KERN_ALERT "[WARNING]%s Parsing irda_dev_count fail\n", __func__);
		return -EINVAL;
	} else
		ir_dev->p_rtk_key_table = devm_kzalloc(dev, sizeof(struct venus_key_table)*ir_dev->irda_dev_count, GFP_KERNEL);

	ir_dev->pdec = devm_kzalloc(dev, sizeof(struct ir_sw_decoder)*ir_dev->irda_dev_count, GFP_KERNEL);

	for(i=0; i<ir_dev->irda_dev_count; i++) {
		irda_node = of_get_child_by_name(dev->of_node, irda_dev[i]);
		if(!irda_node) {
			RTK_debug(KERN_ALERT "could not find %s sub-node\n",irda_dev[i]);
			return -EINVAL;
		}
		if(of_property_read_u32(irda_node, "Realtek,irrx-protocol", &ir_dev->ir_protocol)) {
			ir_dev->ir_protocol = NEC;
			RTK_debug(KERN_ALERT "[WARNING]%s Parsing DTB fail, Use default irrx-protocol = %d .\n", __func__, ir_dev->ir_protocol);
		}
		if (of_property_read_u32(irda_node, "Realtek,irtx-protocol", &ir_dev->irtx_protocol)) {
			ir_dev->irtx_protocol = NEC_TX;
			RTK_debug(KERN_ALERT "[WARNING]%s Parsing DTB fail, Use default irtx-protocol = %d .\n", __func__, ir_dev->irtx_protocol);
		}
		
		get_decoder_func(&ir_dev->pdec[i], ir_dev->ir_protocol);
		
		prop = of_get_property(irda_node, "Realtek,keymap-tbl", &size);
    	err = size % (sizeof(u32)*KEYMAP_TABLE_COLUMN);
		if (of_property_read_u32(irda_node, "Realtek,cust-code", &cust_code))
		{
			RTK_debug(KERN_ALERT "[WARNING]%s Parsing DTB fail, Use default cust_code\n", __func__);
			err++;
		}
		if(of_property_read_u32(irda_node, "Realtek,scancode-msk", &scancode_msk))
		{
			RTK_debug(KERN_ALERT "[WARNING]%s Parsing DTB fail, Use default scancode_msk\n", __func__);
			err++;
		}
		if(of_property_read_u32(irda_node, "Realtek,custcode-msk", &custcode_msk))
		{
			RTK_debug(KERN_ALERT "[WARNING]%s Parsing DTB fail, Use default custcode_msk\n", __func__);
			err++;
		}

		if ((prop) && (!err))
		{
			int counter = size / (sizeof(u32)*KEYMAP_TABLE_COLUMN);
			//Todo : could use devm_kzalloc to replace kzalloc (by toto)			
			ir_dev->p_rtk_key_table[i].keys = (struct venus_key*)kzalloc(sizeof(struct venus_key) * (counter+1), GFP_KERNEL);
			ir_dev->p_rtk_key_table[i].keys = (struct venus_key*)devm_kzalloc(dev, sizeof(struct venus_key) * (counter+1), GFP_KERNEL);
			ir_dev->p_rtk_key_table[i].size = counter;
			ir_dev->p_rtk_key_table[i].cust_code = cust_code;
			ir_dev->p_rtk_key_table[i].scancode_msk = scancode_msk;
			ir_dev->p_rtk_key_table[i].custcode_msk = custcode_msk;
			RTK_debug(KERN_ALERT "[%s] %s  %d counter=%d cust_code=0x%x\n", __FILE__,__FUNCTION__,__LINE__,counter,cust_code); 	
			
			RTK_debug(KERN_ALERT "[%s] %s  %d scancode_msk=0x%x custcode_msk=0x%x\n", __FILE__,__FUNCTION__,__LINE__,scancode_msk,custcode_msk); 	
	
			for (j=0;j<counter;j+=1)
			{
				ir_dev->p_rtk_key_table[i].keys[j].scancode= of_read_number(prop, 1 + (j*KEYMAP_TABLE_COLUMN));
				ir_dev->p_rtk_key_table[i].keys[j].keycode = of_read_number(prop, 2 + (j*KEYMAP_TABLE_COLUMN));				
			}
		} 
		if (of_property_read_u32(dev->of_node, "Realtek,reg-ir-dpir", &ir_dev->dpir_val)) {
			ir_dev->dpir_val = 60;
			printk(KERN_ALERT "[WARNING]%s Parsing DTB fail, Use default reg-ir-dpir = %d .\n", __func__, ir_dev->dpir_val);
		}
	}
	return 0;
}

int rtk119x_ir_probe(struct platform_device * pdev)
{
	struct device *dev = &pdev->dev;
	const u32 *prop;
	int size;
	int err=0, result;
	int i;
	unsigned int cust_code, scancode_msk, custcode_msk;

	ir_dev = devm_kzalloc(dev, sizeof(struct rtk_ir_dev), GFP_KERNEL);
	if (!ir_dev) {
		dev_err(dev, "[%s]: devm_kzalloc allocate fail\n", __func__);
		return -ENOMEM;
	}
	ir_cdev = devm_kzalloc(dev, sizeof(struct rtk_ir_cdev), GFP_KERNEL);
	if (!ir_cdev) {
		dev_err(dev, "[%s]: devm_kzalloc allocate fail\n", __func__);
		return -ENOMEM;
	}

	result = sysfs_create_group(&pdev->dev.kobj, &irda_attr_group);
	if (result < 0) {
		dev_err(dev, "rtk119x_ir_probe: sysfs_create_group() failed: %d\n", result);
		return result;
	}
	
	spin_lock_init(&ir_dev->ir_splock);

#ifdef CONFIG_SUPPORT_INPUT_DEVICE
	setup_timer(&ir_dev->ir_wakeup_timer, rtk_ir_wakeup_timer, 0);
#endif
	/* Read properties from DTB */
	ir_dev->sys_reg = of_iomap(dev->of_node, 0);
	if (!ir_dev->sys_reg)
		panic("Can't map iso_sys_reg_base for %s\n", dev->of_node->name);

	ir_dev->clk = clk_get(dev, NULL);
	ir_dev->rstc = reset_control_get(dev, NULL);

	if(ir_dev->rstc!=NULL)
		reset_control_deassert(ir_dev->rstc);
	if(ir_dev->clk!=NULL)
		clk_prepare_enable(ir_dev->clk);

	ir_dev->ir_reg = of_iomap(dev->of_node, 1);
	if (!ir_dev->ir_reg)
		panic("Can't map iso_irda_reg_base for %s\n", dev->of_node->name);

	if(of_property_read_u32(dev->of_node, "Realtek,multiple_support", &ir_dev->multiple_support)) {
		dev_err(dev, "[%s]: can't get multiple support from dtb, set to default->not support\n", __func__);
		ir_dev->multiple_support = 0;
		ir_dev->irda_dev_count = 1;
	}
	if(ir_dev->multiple_support) {
		if(!rtk119x_ir_multiple_probe(pdev))
			goto INIT;
	}
	ir_dev->pdec = devm_kzalloc(dev, sizeof(struct ir_sw_decoder)*ir_dev->irda_dev_count, GFP_KERNEL);

	if(of_property_read_u32(dev->of_node, "Realtek,irrx-protocol", &ir_dev->ir_protocol)) {
		dev_err(dev, "[%s]: can't get ir_protocol from dtb, set to default->NEC\n", __func__);
		ir_dev->ir_protocol = NEC;
	}
	if(of_property_read_u32(dev->of_node, "Realtek,irtx-protocol", &ir_dev->irtx_protocol)) {
		dev_err(dev, "[%s]: can't get irtx_protocol from dtb, set to default->NEC_TX\n", __func__);
		ir_dev->irtx_protocol = NEC_TX;
	}

	get_decoder_func(ir_dev->pdec, ir_dev->ir_protocol);

	prop = of_get_property(dev->of_node, "Realtek,keymap-tbl", &size);
	err = size % (sizeof(u32)*KEYMAP_TABLE_COLUMN);
	if(of_property_read_u32(dev->of_node, "Realtek,cust-code", &cust_code))
		err++;
	if(of_property_read_u32(dev->of_node, "Realtek,scancode-msk", &scancode_msk))
		err++;
	if(of_property_read_u32(dev->of_node, "Realtek,custcode-msk", &custcode_msk))
		err++;

	if ((prop) && (!err)) {
		int counter = size / (sizeof(u32)*KEYMAP_TABLE_COLUMN);

		ir_dev->p_rtk_key_table = devm_kzalloc(dev, sizeof(struct venus_key_table), GFP_KERNEL);
		ir_dev->p_rtk_key_table->keys = devm_kzalloc(dev, sizeof(struct venus_key) * (counter+1), GFP_KERNEL);
		ir_dev->p_rtk_key_table->size = counter;
		ir_dev->p_rtk_key_table->cust_code = cust_code;
		ir_dev->p_rtk_key_table->scancode_msk = scancode_msk;
		ir_dev->p_rtk_key_table->custcode_msk = custcode_msk;

		for (i=0; i<counter; i+=1) {
			ir_dev->p_rtk_key_table->keys[i].scancode= of_read_number(prop, 1 + (i*KEYMAP_TABLE_COLUMN));
			ir_dev->p_rtk_key_table->keys[i].keycode = of_read_number(prop, 2 + (i*KEYMAP_TABLE_COLUMN));
		}
	} else {
		ir_dev->p_rtk_key_table = &rtk_mk5_tv_key_table;
		dev_err(dev, "[%s]: Parsing DTB fail, Use default keymap table rtk_mk5_tv_key_table\n", __func__);
	}

	if (of_property_read_u32(dev->of_node, "Realtek,reg-ir-dpir", &ir_dev->dpir_val)) {
		ir_dev->dpir_val = 60;
		dev_err(dev, "[%s]: Parsing DTB fail, Use default reg-ir-dpir\n", __func__);
	}
INIT:
	of_property_read_u32(dev->of_node, "Realtek,ir-repeat-mode", &ir_dev->rpmode);

	if(ir_dev->rpmode) {
		if(of_property_read_u32(dev->of_node, "Realtek,ir-repeat-fifosize", &ir_dev->rpmode_fifosize))
			ir_dev->rpmode_fifosize = 1024;
		if(of_property_read_u32(dev->of_node, "Realtek,ir-repeat-samplerate", &ir_dev->sample_rate))
			ir_dev->sample_rate = 200;

		ir_dev->ir_protocol = REPEAT_MODE;
		ir_dev->irtx_protocol = REPEAT_MODE;
	}

	/* start initialize ir module */
	ir_dev->ir_irq = irq_of_parse_and_map(dev->of_node, 0);
	if (!ir_dev->ir_irq) {
		dev_err(dev, "[%s]: fail to parse of irq.\n", __func__);
		return -ENXIO;
	}
	
	if(request_irq(ir_dev->ir_irq, rtk_ir_isr, IRQF_SHARED, "Venus_IR", rtk_ir_isr)) {
		dev_err(dev, "[%s]: cannot register IRQ\n", __func__);
		return -EIO;
	}

	/* add node in /dev for file operation */
	if(ir_dev->rpmode) {
		if(kfifo_alloc(&ir_cdev->ir_rxfifo, ir_dev->rpmode_fifosize, GFP_KERNEL)) {
			dev_err(dev, "[%s]: kfifo_alloc failed\n", __func__);
			return -ENOMEM;
		}
		if(kfifo_alloc(&ir_cdev->ir_txfifo, ir_dev->rpmode_fifosize, GFP_KERNEL)) {
			dev_err(dev, "[%s]: kfifo_alloc failed\n", __func__);
			return -ENOMEM;
		}
		ir_dev->rpmode_rxfifo = devm_kzalloc(dev, ir_dev->rpmode_fifosize, GFP_KERNEL);
		if(!ir_dev->rpmode_rxfifo) {
			dev_err(dev, "[%s]: rpmode_rxfifo alloc failed\n", __func__);
			return -ENOMEM;
		}
		ir_dev->rpmode_txfifo = devm_kzalloc(dev, ir_dev->rpmode_fifosize, GFP_KERNEL);
		if(!ir_dev->rpmode_txfifo) {
			dev_err(dev, "[%s]: rpmode_txfifo alloc failed\n", __func__);
			return -ENOMEM;
		}
	} else {
		if(kfifo_alloc(&ir_cdev->ir_rxfifo, FIFO_DEPTH*sizeof(unsigned int), GFP_KERNEL)) {
			dev_err(dev, "[%s]: kfifo_alloc failed\n", __func__);
			return -ENOMEM;
		}
		if(kfifo_alloc(&ir_cdev->ir_txfifo, FIFO_DEPTH*sizeof(unsigned int), GFP_KERNEL)) {
			dev_err(dev, "[%s]: kfifo_alloc failed\n", __func__);
			return -ENOMEM;
		}
	}

	ir_cdev->dev_no = MKDEV(VENUS_IR_MAJOR, VENUS_IR_MINOR);
	register_chrdev_region(ir_cdev->dev_no, VENUS_IR_DEVICE_NUM, "ir_cdev");

	ir_cdev->cdev = cdev_alloc();
	ir_cdev->cdev->ops = &venus_ir_fops;
	cdev_add(ir_cdev->cdev, MKDEV(VENUS_IR_MAJOR, VENUS_IR_MINOR), 1);

	ir_cdev->class = class_create(THIS_MODULE, "ir_class");
	device_create(ir_cdev->class, NULL, MKDEV(VENUS_IR_MAJOR, VENUS_IR_MINOR), &ir_cdev, VENUS_IR_DEVICE_FILE);

	init_waitqueue_head(&ir_cdev->rx_wait);

	setup_timer(&ir_dev->ir_keypad_timer, rtk_ir_keypad_timer, 0);
	ir_dev->ir_keypad_timer.expires	= jiffies + msecs_to_jiffies(70);

	rtk_ir_init();
	venus_ir_input_init();

//	setup_timer(&ir_dev->ir_keypad_timer, rtk_ir_keypad_timer, 0);
//	ir_dev->ir_keypad_timer.expires	= jiffies + msecs_to_jiffies(70);

	return 0;
}

#ifdef CONFIG_SUSPEND
static int venus_ir_pm_suspend(struct device *dev)
{
	unsigned int regValue;
	struct RTK119X_ipc_shm __iomem *ipc = (void __iomem *)IPC_SHM_VIRT;
	struct RTK119X_ipc_shm_ir __iomem *ir_ipc = (void __iomem *)(IPC_SHM_VIRT +sizeof(struct RTK119X_ipc_shm));
	unsigned int phy_ir_ipc = RPC_COMM_PHYS + (0xC4) + sizeof(struct RTK119X_ipc_shm);

	RTK_debug("[IRDA] Enter %s\n", __FUNCTION__);

	venus_irtx_set_wake_up_keys(116, (u32*)ir_ipc);
	writel(__cpu_to_be32(phy_ir_ipc), &(ipc->ir_extended_tbl_pt));	

#ifdef CONFIG_QUICK_HIBERNATION
	if (!in_suspend) {
		// flush IRRP,
		while(readl(ir_dev->ir_reg+SATURN_ISO_IR_SR_OFF) & 0x1) {
			writel(0x00000003, ir_dev->ir_reg+SATURN_ISO_IR_SR_OFF); /* clear IRDVF */
			regValue = readl(ir_dev->ir_reg+SATURN_ISO_IR_RP_OFF); /* read IRRP */
		}
	}
#else
	// flush IRRP
	while(readl(ir_dev->ir_reg+SATURN_ISO_IR_SR_OFF) & 0x1) {
		writel(0x00000003, ir_dev->ir_reg+SATURN_ISO_IR_SR_OFF); /* clear IRDVF */
		regValue = readl(ir_dev->ir_reg+SATURN_ISO_IR_RP_OFF); /* read IRRP */
	}
#endif
	// disable interrupt
	regValue = readl(ir_dev->ir_reg+SATURN_ISO_IR_CR_OFF);
	regValue = regValue & ~(0x400);
	writel(regValue, ir_dev->ir_reg+SATURN_ISO_IR_CR_OFF);

	RTK_debug("[IRDA] Exit %s\n", __FUNCTION__);

	return 0;
}

static int venus_ir_pm_resume(struct device *dev)
{
	unsigned int regValue;

	RTK_debug("[IRDA] Enter %s\n", __FUNCTION__);

#ifdef CONFIG_QUICK_HIBERNATION
	if (!in_suspend) {
		/* reinitialize kfifo */
		kfifo_reset(&ir_cdev->ir_rxfifo);

		// flush IRRP
		while(readl(ir_dev->ir_reg+SATURN_ISO_IR_SR_OFF) & 0x1) {
			writel(0x00000003, ir_dev->ir_reg+SATURN_ISO_IR_SR_OFF); /* clear IRDVF */
			regValue = readl(ir_dev->ir_reg+SATURN_ISO_IR_RP_OFF); /* read IRRP */
		}
	}
#else
	/* reinitialize kfifo */
	kfifo_reset(&ir_cdev->ir_rxfifo);

	// flush IRRP
	while(readl(ir_dev->ir_reg+SATURN_ISO_IR_SR_OFF) & 0x1) {
		writel(0x00000003, ir_dev->ir_reg+SATURN_ISO_IR_SR_OFF); /* clear IRDVF */
		regValue = readl(ir_dev->ir_reg+SATURN_ISO_IR_RP_OFF); /* read IRRP */
	}
#endif

#ifdef CONFIG_HIBERNATION
	// reinitialize ir register
	rtk_ir_init();
#else
	rtk_ir_init();
	// enable interrupt
	regValue = readl(ir_dev->ir_reg+SATURN_ISO_IR_CR_OFF);
	regValue = regValue | 0x400;
	writel(regValue, ir_dev->ir_reg+SATURN_ISO_IR_CR_OFF);
#endif
	mod_timer(&ir_dev->ir_wakeup_timer, (jiffies + (msecs_to_jiffies(1000))) );

	RTK_debug("[IRDA] Exit %s\n", __FUNCTION__);

	return 0;
}

static const struct dev_pm_ops venus_ir_pm_ops = {
    .suspend    = venus_ir_pm_suspend,
    .resume     = venus_ir_pm_resume,
#ifdef CONFIG_HIBERNATION
    .freeze     = venus_ir_pm_suspend,
    .thaw       = venus_ir_pm_resume,
    .poweroff   = venus_ir_pm_suspend,
    .restore    = venus_ir_pm_resume,
#endif
};
#else //CONFIG_SUSPEND
static const struct dev_pm_ops venus_ir_pm_ops = {};
#endif //CONFIG_SUSPEND

static struct platform_driver rtk119x_ir_driver = {
	.driver		= {
		.name	= "rtk119x-ir",
		.owner	= THIS_MODULE,
		.of_match_table = rtk119x_irda_ids,
		.pm		= &venus_ir_pm_ops,
		.shutdown = venus_ir_pm_suspend,
	},
	.probe		= rtk119x_ir_probe,
	.remove		= rtk119x_ir_remove,
};

module_platform_driver(rtk119x_ir_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("REALTEK Corporation");
MODULE_ALIAS("platform:irda");
