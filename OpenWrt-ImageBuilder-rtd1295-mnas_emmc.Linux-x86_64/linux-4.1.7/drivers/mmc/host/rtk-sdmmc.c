/*
 * Realtek SD card driver
 *
 * Authors:
 * Copyright (C) 2015 Realtek Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/blkdev.h>
#include <linux/clk.h>
#include <linux/debugfs.h>
#include <linux/device.h>
#include <linux/dma-mapping.h>
#include <linux/err.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/stat.h>
#include <linux/delay.h>
#include <linux/irq.h>
#include <linux/utsname.h>
#include <linux/mmc/card.h>
#include <linux/mmc/host.h>
#include <linux/mmc/mmc.h>
#include <linux/mmc/sd.h>
#include <linux/mmc/sdio.h>
#include <linux/mmc/slot-gpio.h>
#include <linux/bitops.h>
#include <linux/regulator/consumer.h>

#include <linux/clkdev.h>   // clk_get

#include <linux/clk-provider.h>

//#include <linux/syscalls.h>
//#include <uapi/asm-generic/unistd.h>

#include <linux/kthread.h>
#include <soc/realtek/rtd129x_lockapi.h>

#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/of_gpio.h>

#include "rtk-sdmmc-reg.h"
#include "rtk-sdmmc.h"

#define DRIVER_NAME    "rtk-sdmmc"
#define BANNER      "Realtek SD/MMC Host Driver"

#define SD_ALLOC_LENGTH    2048
#define MAX_PHASE    31
#define TUNING_CNT    3

#define CMD25_WO_STOP_COMMAND

#define PM_SD_SUSPEND_ON           0
#define PM_SD_SUSPEND_FREEZE       1
#define PM_SD_SUSPEND_STANDBY      2
#define PM_SD_SUSPEND_MEM          3
#define PM_SD_SUSPEND_MIN          PM_SD_SUSPEND_FREEZE
#define PM_SD_SUSPEND_MAX          4

/*extern void set_gpio20_high(void);
extern void set_gpio20_low(void);
extern void set_gpio18_high(void);
extern void set_gpio18_low(void);

#define TOGGLE_GPIO_HIGH20    set_gpio20_high()
#define TOGGLE_GPIO_LOW20     set_gpio20_low()
#define TOGGLE_GPIO_HIGH18    set_gpio18_high()
#define TOGGLE_GPIO_LOW18     set_gpio18_low()

#ifndef TOGGLE_GPIO_HIGH20
        #define TOGGLE_GPIO_HIGH20
#endif
#ifndef TOGGLE_GPIO_LOW20
        #define TOGGLE_GPIO_LOW20
#endif

#ifndef TOGGLE_GPIO_HIGH18
        #define TOGGLE_GPIO_HIGH18
#endif
#ifndef TOGGLE_GPIO_LOW18
        #define TOGGLE_GPIO_LOW18
#endif*/
enum eINT_enable_mode {
	CMD_RSP_ONLY = 0,
	DATA_ONLY,
	CMD_RSP_DATA_WRITE,
	CMD_RSP_DATA_READ,
	CMD_RSP_DATA_WRITE_TUNNING,
	CMD_RSP_DATA_READ_TUNNING,
	DATA_WRITE_ONLY,
	DATA_READ_ONLY,
};

static volatile u32 data_xfer_mode;

DECLARE_COMPLETION(rtk_sdmmc_wait);
static int rtk_sdmmc_send_stop_cmd(struct mmc_command *cmd, struct rtk_sdmmc_host *rtk_host,int flag);
static int rtk_sdmmc_send_cmd_get_rsp(struct sdmmc_cmd_pkt *cmd_info);
static int rtk_sdmmc_stream(struct sdmmc_cmd_pkt *cmd_info);
static void rtk_sdmmc_hw_initial(struct rtk_sdmmc_host *rtk_host);
static void rtk_sdmmc_timeout(unsigned long data);
static void rtk_sdmmc_plug(unsigned long data);
static void rtk_sdmmc_cmd12_fun(unsigned long data);
static void rtk_sdmmc_set_access_mode(struct rtk_sdmmc_host *rtk_host,u8 level);
void remove_sdcard(struct rtk_sdmmc_host *rtk_host);
#ifdef CONFIG_MMC_RTK_EMMC
int get_RTK_initial_flag(void);
#endif
void rtk_sdmmc_sync(struct rtk_sdmmc_host *rtk_host);
static void rtk_sdmmc_speed(struct rtk_sdmmc_host *rtk_host, enum sdmmc_clock_speed sd_speed);
static unsigned int sd_in_receive_data_state; //CMD25_WO_STOP_COMMAND
static unsigned int sd_current_blk_address; //CMD25_WO_STOP_COMMAND
static u32 *dma_phy_addr = NULL;
static u32 *dma_virt_addr = NULL;
static unsigned int g_crinit = 0;
static unsigned int sdmmc_rca = 0;
static unsigned int mmc_detected = 0;
static u8 g_cmd[6];
bool suspend_SD_insert_flag=false;
int poll=0;
int get_RTK_PM_STATE(void);
int pre_cmd=0;
static volatile u32 wait_stop_command_irq_done;
static volatile u32 do_stop_command_wo_complete;
//static struct rw_semaphore cr_sd_rw_sem;
static struct semaphore cr_sd_sem;
#ifdef CMD25_WO_STOP_COMMAND
struct task_struct *rtk_SD_task = NULL;
struct mmc_command cmd12_stop_cmd;
#endif

struct rtk_sdmmc_host *rtk_global_host = NULL;
void mmc_blk_set_ro(struct mmc_card* card);
bool INT4_flag=false;
bool CMD8_SD1=false;
bool CMD55_MMC=false;
int driving_capacity=0;
bool suspend_flag=false;
int mini_SD=0;
bool broken_flag=false;
u32 g_ro = 0;
int irq_counter=0;
int irq_error_bit=1;
#ifdef CONFIG_RTK_XEN_SUPPORT
#define XEN 1
#else
#define XEN 0
#endif
//=============================================
//static struct workqueue_struct *cmd12_qu;
//static struct delayed_work rtk_cmd12_delayed_work;
//=============================================

void rtk_sdmmc_wait_stop_cmd_irq_done(void)
{
    unsigned long timeout;
    unsigned int istimeout = 1;
    timeout = jiffies + msecs_to_jiffies(100);
    while( time_before(jiffies, timeout) ) {
//      printk(KERN_ERR "<<%d>>(%d)\n", __LINE__, wait_stop_command_irq_done);
        if( wait_stop_command_irq_done == 0 ) {
                istimeout = 0;
                break;;
        }
    }
    do_stop_command_wo_complete = 0;
    if( istimeout )
        printk(KERN_ERR "****** WAIT wait_stop_command_irq_done TIMEOUT(%d,%d))\n", do_stop_command_wo_complete, wait_stop_command_irq_done);
}

void card_broken(struct rtk_sdmmc_host *rtk_host)
{
       	void __iomem *sdmmc_base = rtk_host->sdmmc;
       	unsigned long timeout = 0;

        printk(KERN_ERR "SD Card is broken!!!\n");
        
	writel(0x0, sdmmc_base + CR_SD_DMA_CTL3); //stop dma control
	data_xfer_mode = CMD_RSP_ONLY;
        writeb(0xff, sdmmc_base + CR_CARD_STOP); //SD Card module transfer stop and idle state
        writeb(0x40, sdmmc_base + SD_BUS_STATUS);
        mdelay(10);
        writeb(0x3b, sdmmc_base + CARD_CLOCK_EN_CTL);
        rtk_host->ops->card_power(rtk_host, 0);
        rtk_sdmmc_sync(rtk_host);
}

static int rtk_sdmmc_pm_complete(struct device *dev)
{
	unsigned long flags2;

        struct mmc_host *mmc = dev_get_drvdata(dev);
        struct rtk_sdmmc_host *rtk_host = NULL;
        rtk_host = mmc_priv(mmc);
        printk(KERN_ERR "[SD] Realtek rtk_sdmmc_pm_complete OK!!!\n");
        //work around for A version because EMMC reset after resume
        if(mmc->ios.clock >= UHS_SDR104_MAX_DTR && mmc->ios.timing == MMC_TIMING_UHS_SDR104) {
		printk(KERN_ERR "[SD] restore SDR104 card driving capability!!!\n");
		
		rtk_lockapi_lock(flags2, __FUNCTION__);    //lock 0x98012xxx for fear that emmc access followed by 98xx_x9xx or 98xxx8xx
		switch(driving_capacity%10) {
                case 0:
                case 1:
                        writel(0x0000aaaa, rtk_host->emmc + 0x634);         //JIM modified this part for dynamically adjusting driving capacity
                        writel(0xaaaaaaaa, rtk_host->emmc + 0x638);
                        break;
                case 2:
                case 3:
                        writel(0x0000bbbb, rtk_host->emmc + 0x634);
                        writel(0xbbbbbbbb, rtk_host->emmc + 0x638);
                        break;
                case 4:
                case 5:
                        writel(0x0000cccc, rtk_host->emmc + 0x634);
                        writel(0xcccccccc, rtk_host->emmc + 0x638);
                        break;
                case 6:
                case 7:
                        writel(0x0000dddd, rtk_host->emmc + 0x634);
                        writel(0xdddddddd, rtk_host->emmc + 0x638);
                        break;
                case 8:
                case 9:
                        writel(0x0000eeee, rtk_host->emmc + 0x634);
                        writel(0xeeeeeeee, rtk_host->emmc + 0x638);
                        break;
                default:
                        writel(0x0000aaaa, rtk_host->emmc + 0x634);
                        writel(0xaaaaaaaa, rtk_host->emmc + 0x638);
                }

		
		rtk_lockapi_unlock(flags2, __FUNCTION__);  //unlock
       } else if(mmc->ios.clock >= UHS_SDR50_MAX_DTR && mmc->ios.timing == MMC_TIMING_UHS_SDR50) {
		printk(KERN_ERR "[SD] restore SDR50 card driving capability!!!\n");

                rtk_lockapi_lock(flags2, __FUNCTION__);    //lock 0x98012xxx for fear that emmc access followed by 98xx_x9xx or 98xxx8xx

                writel(0x00005555, rtk_host->emmc + 0x634);   //JIM modified this part from 3333 to 5555 to enlarge the signal to work around a issue that can not recognize some SD 3.0 card
                writel(0x55555555, rtk_host->emmc + 0x638);         //JIM modified this part from 33333333 to 55555555

                rtk_lockapi_unlock(flags2, __FUNCTION__);  //unlock
	}

	/*if(get_RTK_PM_STATE()!= PM_SD_SUSPEND_STANDBY) {
                printk(KERN_ERR "[SD] Realtek SD card whole resume process OK!!!\n");
                u32 det_time = 0;
                unsigned int reg_tmp = 0;
                unsigned long timeout = 0;
                u32 reginfo = 0;
                void __iomem *sdmmc_base = rtk_host->sdmmc;
                void __iomem *pll_base = rtk_host->pll;
                rtk_host->ins_event = EVENT_REMOV;
                rtk_host->rtflags &= ~RTKCR_FCARD_DETECTED;
                det_time = 1;
                //reset
                timeout = jiffies + msecs_to_jiffies(100);

                while(time_before(jiffies, timeout)){
			rtk_lockapi_lock(flags2, __FUNCTION__);    //lock 0x98012xxx for fear that emmc access followed by 98xx_x9xx or 98xxx8xx
                        if((!(readl(rtk_host->emmc + EMMC_DMA_CTL3) & 0x01))) { //; jim just skip this and need to check with james
                        	rtk_lockapi_unlock(flags2, __FUNCTION__);  //unlock
				break;
			}
			rtk_lockapi_unlock(flags2, __FUNCTION__);  //unlock
			//msleep(1);
                }
                //writel(readl(pll_base + 0x0C) | (0x1 << 25) | (0x1 << 31), pll_base + 0x0C);

                //writel(readl(sdmmc_base + 0x20) | 0x00000002, sdmmc_base + 0x20);    //add by jim for test, change the state to reset the SD IP
                //writel(readl(sdmmc_base + 0x20) | 0x00000003, sdmmc_base + 0x20);    //comment by jim 2016.9.1
		writel(readl(sdmmc_base + 0x20) & (~0x01), sdmmc_base + 0x20);    //modified by JIM 2016.9.1 for power saving, set L4 gated enabled
    		writel(readl(sdmmc_base + 0x20) | 0x00000001, sdmmc_base + 0x20);   //reset the DMA

                writel(0x0, sdmmc_base + CR_SD_DMA_CTL3); //stop dma control
                writeb(0xff, sdmmc_base + CR_CARD_STOP); //SD Card module transfer stop and idle state

                rtk_host->ops->card_power(rtk_host, 0); //power off, Jim add

                rtk_sdmmc_sync(rtk_host);
                rtk_host->rtflags &= ~RTKCR_FCARD_SELECTED;
                sdmmc_rca = 0;
                mmc_detect_change(rtk_host->mmc, msecs_to_jiffies(det_time));
                rtk_host->int_status_old = 0x20;
       }*/
        return 0;
}

void remove_sdcard(struct rtk_sdmmc_host *rtk_host) {
	unsigned long flags2;
	u32 det_time = 0;
        unsigned int reg_tmp = 0;
        unsigned long timeout = 0;
        u32 reginfo = 0;
        void __iomem *sdmmc_base = rtk_host->sdmmc;
        void __iomem *pll_base = rtk_host->pll;
        rtk_host->ins_event = EVENT_REMOV;
        rtk_host->rtflags &= ~RTKCR_FCARD_DETECTED;
        det_time = 1;
#ifdef CONFIG_MACH_RTK1195
        //reset
        timeout = jiffies + msecs_to_jiffies(100);

        while(time_before(jiffies, timeout)){
             rtk_lockapi_lock(flags2, __FUNCTION__);    //lock 0x98012xxx for fear that emmc access followed by 98xx_x9xx or 98xxx8xx
             if((!(readl(rtk_host->emmc + EMMC_DMA_CTL3) & 0x01)) && (readb(rtk_host->sdio + SDIO_NORML_INT_STA) & 0x02)) {
                    rtk_lockapi_unlock(flags2, __FUNCTION__);  //unlock
                    break;
             }
             rtk_lockapi_unlock(flags2, __FUNCTION__);  //unlock
        }
#endif
        //writel(readl(pll_base + 0x0C) | (0x1 << 25) | (0x1 << 31), pll_base + 0x0C);
        //writel(readl(sdmmc_base + 0x20) | 0x00000002, sdmmc_base + 0x20);    //add by jim for test, change the state to reset the SD IP
        //writel(readl(sdmmc_base + 0x20) | 0x00000003, sdmmc_base + 0x20);
 
        writel(readl(sdmmc_base + 0x20) & (~0x01), sdmmc_base + 0x20);    //modified by JIM 2016.9.1 for power saving, set L4 gated enabled
    	writel(readl(sdmmc_base + 0x20) | 0x00000001, sdmmc_base + 0x20);   //reset the DMA
	
        writel(0x0, sdmmc_base + CR_SD_DMA_CTL3); //stop dma control
	data_xfer_mode = CMD_RSP_ONLY;
        writeb(0xff, sdmmc_base + CR_CARD_STOP); //SD Card module transfer stop and idle state

        rtk_host->ops->card_power(rtk_host, 0); //power off, Jim add

        rtk_sdmmc_sync(rtk_host);
        rtk_host->rtflags &= ~RTKCR_FCARD_SELECTED;
        sdmmc_rca = 0;
        mmc_detect_change(rtk_host->mmc, msecs_to_jiffies(det_time));
        rtk_host->int_status_old = 0x20;
}

/*SYSCALL_DEFINE0(sdcard_unplug) {
	printk(KERN_ERR "Simulation of  SD card unplug and plug!\n");
	remove_sdcard(rtk_global_host);
	return 0;
}*/

static int rtk_sdmmc_pm_suspend(struct device *dev)
{
        int ret = 0;
        struct mmc_host *mmc = dev_get_drvdata(dev);
	struct rtk_sdmmc_host *rtk_host = NULL;
        rtk_host = mmc_priv(mmc);

	u32 det_time = 0;
	unsigned int reg_tmp = 0;
	unsigned long timeout = 0;
	u32 reginfo = 0;

	void __iomem *sdmmc_base = rtk_host->sdmmc;
        void __iomem *pll_base = rtk_host->pll;

	unsigned long flags2;

	suspend_flag=true;

	if(get_RTK_PM_STATE() == PM_SD_SUSPEND_STANDBY) printk(KERN_ERR "[SD] Realtek SD card reader idle suspend!!\n");
	else printk(KERN_ERR "[SD] Realtek SD card reader suspend to ram start!!\n"); 
        
        ret = mmc_suspend_host(mmc);
        del_timer_sync(&rtk_host->timer);
	del_timer_sync(&rtk_host->plug_timer);

	#ifdef CMD25_WO_STOP_COMMAND
        del_timer_sync(&rtk_host->rtk_sdmmc_stop_cmd);
	#endif

	reginfo = readb(sdmmc_base + CARD_EXIST);
	if(reginfo & SD_EXISTENCE) {
		printk(KERN_ERR "[SD] Realtek SD turn off card power!!\n");
		suspend_SD_insert_flag=true;
		rtk_host->ins_event = EVENT_REMOV;
		rtk_host->rtflags &= ~RTKCR_FCARD_DETECTED;
		det_time = 1;
#ifdef CONFIG_MACH_RTK1195
		//reset, this is workaround for phoenix, phoenix 1195 DMA reset will have impact in EMMC SDIO, so we need to make sure that emmc and sdio dma is inactive now
		timeout = jiffies + msecs_to_jiffies(100);

		while(time_before(jiffies, timeout)){
			rtk_lockapi_lock(flags2, __FUNCTION__);
			if((!(readl(rtk_host->emmc + EMMC_DMA_CTL3) & 0x01)) && (readb(rtk_host->sdio + SDIO_NORML_INT_STA) & 0x02)){ //; jim skip this comment
				rtk_lockapi_unlock(flags2, __FUNCTION__);
				break;
			}
			rtk_lockapi_unlock(flags2, __FUNCTION__);
			//msleep(1);
		}
#endif
		//writel(readl(pll_base + 0x0C) | (0x1 << 25) | (0x1 << 31), pll_base + 0x0C);
			 
		writel(readl(sdmmc_base + 0x20) & (~0x01), sdmmc_base + 0x20);    //modified by JIM 2016.9.1 for power saving, set L4 gated enabled
    		writel(readl(sdmmc_base + 0x20) | 0x00000001, sdmmc_base + 0x20);   //reset the DMA
		
		writel(0x0, sdmmc_base + CR_SD_DMA_CTL3); //stop dma control
		data_xfer_mode = CMD_RSP_ONLY;
		writeb(0xff, sdmmc_base + CR_CARD_STOP); //SD Card module transfer stop and idle state

		rtk_host->ops->card_power(rtk_host, 0); //power off, Jim add

		rtk_sdmmc_sync(rtk_host);
		rtk_host->rtflags &= ~RTKCR_FCARD_SELECTED;
		sdmmc_rca = 0;
		mmc_detect_change(rtk_host->mmc, msecs_to_jiffies(det_time));
	}
	else suspend_SD_insert_flag=false;
        return ret;
}


static int rtk_sdmmc_pm_resume(struct device *dev)
{
        int ret = 0;
        struct mmc_host *mmc = dev_get_drvdata(dev);
        struct rtk_sdmmc_host *rtk_host = NULL;
        rtk_host = mmc_priv(mmc);

	void __iomem *pll_base = rtk_host->pll;
        void __iomem *sdmmc_base = rtk_host->sdmmc;

	u32 reginfo = 0;
	u32 det_time = 0;
       
	if(get_RTK_PM_STATE() == PM_SD_SUSPEND_STANDBY) printk(KERN_ERR "[SD] Realtek SD card reader idle resume OK!!\n");
        else printk(KERN_ERR "[SD] Realtek SD card reader resume OK!!\n");
	
	if(get_RTK_PM_STATE() == PM_SD_SUSPEND_STANDBY) {
		writel(0x0000003, pll_base + CR_PLL_SD1);
		mdelay(10); //delay 500 ms to fit SD spec
		writel(0x00002003, pll_base + CR_PLL_SD1); //PLL_SD1
	}
	else rtk_sdmmc_hw_initial(rtk_host);   //suspend will cause poweroff and the SD PLL will be set as default, so we need to initial the SD PLL and clock before resume

	reginfo = readb(sdmmc_base + CARD_EXIST);
	if(reginfo & SD_EXISTENCE && suspend_SD_insert_flag==true) {
		printk(KERN_ERR "[SD] Realtek SD turn on card power!!\n");
		rtk_host->rtflags &= ~RTKCR_FCARD_DETECTED;
		rtk_host->wp = 0;

		rtk_host->ops->card_power(rtk_host, 1); //power on, Jim add

		rtk_host->ins_event = EVENT_INSER;
		rtk_host->rtflags |= RTKCR_FCARD_DETECTED;
		det_time = 1;
		writel(0x00000000, sdmmc_base + CR_SD_DMA_CTL3); //stop dma control
		data_xfer_mode = CMD_RSP_ONLY;
		writeb(0x00, sdmmc_base + CR_CARD_STOP); //SD Card module transfer no stop
		writeb(0x02, sdmmc_base + CARD_SELECT); //Specify the current active card module for the coming data transfer, bit 2:0 = 010
		writeb(0x04, sdmmc_base + CR_CARD_OE); //arget module is SD/MMC card module, bit 2 =1
		writeb(0x04, sdmmc_base + CARD_CLOCK_EN_CTL); //SD Card Module Clock Enable, bit 2 = 1
		writeb(0xD0, sdmmc_base + SD_CONFIGURE1);

		if(get_RTK_PM_STATE()!=PM_SD_SUSPEND_STANDBY) rtk_sdmmc_speed(rtk_host, SDMMC_CLOCK_400KHZ);

		writeb(0x00, sdmmc_base + SD_STATUS2);
		rtk_sdmmc_sync(rtk_host);
		rtk_host->rtflags &= ~RTKCR_FCARD_SELECTED;
		sdmmc_rca = 0;
		mmc_detect_change(rtk_host->mmc, msecs_to_jiffies(det_time));
	}
	setup_timer(&rtk_host->timer, rtk_sdmmc_timeout, (unsigned long)rtk_host);
    	setup_timer(&rtk_host->plug_timer, rtk_sdmmc_plug, (unsigned long)rtk_host);

	#ifdef CMD25_WO_STOP_COMMAND
    	setup_timer(&rtk_host->rtk_sdmmc_stop_cmd, rtk_sdmmc_cmd12_fun, (unsigned long)rtk_host);
	#endif
	rtk_sdmmc_sync(rtk_host);
	mod_timer(&rtk_host->plug_timer, jiffies + 3*HZ);

	ret = mmc_resume_host(mmc);

	suspend_flag=false;

        return ret;
}

static const struct dev_pm_ops rtk_sdmmc_pmops = {
    .complete   = rtk_sdmmc_pm_complete,
    .suspend    = rtk_sdmmc_pm_suspend,
    .resume     = rtk_sdmmc_pm_resume,
};

static void rtk_sdmmc_reset(struct rtk_sdmmc_host *rtk_host)
{
    void __iomem *sdmmc_base = rtk_host->sdmmc;

    writel(0x00000000, sdmmc_base + CR_SD_DMA_CTL3);
    data_xfer_mode = CMD_RSP_ONLY;
    writel(0x00000016, sdmmc_base + CR_SD_ISREN);
    writel(0x00000007, sdmmc_base + CR_SD_ISREN);
    writeb(0x00, sdmmc_base + SD_TRANSFER);
    writeb(0xFF, sdmmc_base + CR_CARD_STOP); //SD Card module transfer stop and idle state.
    writeb(0x00, sdmmc_base + CR_CARD_STOP); //SD Card module transfer start.
}

void rtk_sdmmc_sync(struct rtk_sdmmc_host *rtk_host){
    writel(0x00000000, rtk_host->sysbrdg + SDMMC_SYNC);
}

static u8 rtk_sdmmc_get_rsp_type(struct mmc_command* cmd)
{
    u32 rsp_type = 0;
    u32 rsp_type_chk = mmc_resp_type(cmd);

    if(rsp_type_chk == MMC_RSP_R1)
       rsp_type = SD_R1;
    else if(rsp_type_chk == MMC_RSP_R1B)
       rsp_type = SD_R1b;
    else if(rsp_type_chk == MMC_RSP_R2)
       rsp_type = SD_R2;
    else if(rsp_type_chk == MMC_RSP_R3)
       rsp_type = SD_R3;
    else if(rsp_type_chk == MMC_RSP_R6)
       rsp_type = SD_R6;
    else if(rsp_type_chk == MMC_RSP_R7)
       rsp_type = SD_R7;
    else
       rsp_type = SD_R0;

    return rsp_type;
}

static u8 rtk_sdmmc_get_rsp_len(u8 rsp_para)
{
    switch(rsp_para & 0x03){
    case 0:
        return 0;
    case 1:
        return 6;
    case 2:
        return 16;
    default:
        return 0;
    }
}

static u8 rtk_sdmmc_search_final_phase(struct rtk_sdmmc_host *rtk_host, u32 phase_map)
{
    struct timing_phase_path path[MAX_PHASE + 1];
    struct timing_phase_path swap;
    int i = 0;
    int j = 0;
    int k = 0;
    int cont_path_cnt = 0;
    int new_block = 1;
    int max_len = 0;
    int final_path_idx = 0;
    u8 final_phase = 0xFF;

    /* Parse phase_map, take it as a bit-ring */
    for(i = 0 ; i < MAX_PHASE + 1 ; i++){
        if(phase_map & (1 << i)){
            if(new_block){
                new_block = 0;
                j = cont_path_cnt++;
                path[j].start = i;
                path[j].end = i;
            }else
                path[j].end = i;
        }else{
            new_block = 1;
            if(cont_path_cnt){
                /* Calculate path length and middle point */
                int idx = cont_path_cnt - 1;
                path[idx].len = path[idx].end - path[idx].start + 1;
                path[idx].mid = path[idx].start + path[idx].len / 2;
            }
        }
    }

    if(cont_path_cnt == 0){
        rtk_sdmmc_debug(" %s No continuous phase path\n", __func__);
        goto finish;
    }else{
        /* Calculate last continuous path length and middle point */
        int idx = cont_path_cnt - 1;
        path[idx].len = path[idx].end - path[idx].start + 1;
        path[idx].mid = path[idx].start + path[idx].len / 2;
    }

    /* Connect the first and last continuous paths if they are adjacent */
    if (!path[0].start && (path[cont_path_cnt - 1].end == MAX_PHASE)){
        /* Using negative index */
        path[0].start = path[cont_path_cnt - 1].start - MAX_PHASE - 1;
        path[0].len += path[cont_path_cnt - 1].len;
        path[0].mid = path[0].start + path[0].len / 2;
        /* Convert negative middle point index to positive one */
        if (path[0].mid < 0)
            path[0].mid += MAX_PHASE + 1;
        cont_path_cnt--;
    }

    /* Sorting path array,jamestai20141223 */
    for(k = 0 ; k < cont_path_cnt ; ++k){
        for(i = 0 ; i < cont_path_cnt - 1 - k ; ++i){
            if(path[i].len < path[i+1].len){
                swap.end = path[i+1].end;
                swap.len = path[i+1].len;
                swap.mid = path[i+1].mid;
                swap.start = path[i+1].start;

                path[i+1].end = path[i].end;
                path[i+1].len = path[i].len;
                path[i+1].mid = path[i].mid;
                path[i+1].start = path[i].start;

                path[i].end = swap.end;
                path[i].len = swap.len;
                path[i].mid = swap.mid;
                path[i].start = swap.start;
            }
        }
    }

    /* Choose the longest continuous phase path */
    max_len = 0;
    final_phase = 0;
    final_path_idx = 0;
    for(i = 0 ; i < cont_path_cnt ; i++){
        if(path[i].len > max_len){
            max_len = path[i].len;
            if(max_len > 6)    //for compatibility issue, continue len should bigger than 6
                final_phase = (u8)path[i].mid;
            else
                final_phase = 0xFF;
            final_path_idx = i;
        }

        rtk_sdmmc_debug("%s path[%d].start = %d\n", __func__, i, path[i].start);
        rtk_sdmmc_debug("%s path[%d].end = %d\n", __func__, i, path[i].end);
        rtk_sdmmc_debug("%s path[%d].len = %d\n", __func__, i, path[i].len);
        rtk_sdmmc_debug("%s path[%d].mid = %d\n", __func__, i, path[i].mid);
    }

finish:
    rtk_sdmmc_debug("%s Final chosen phase: %d\n", __func__, final_phase);
    return final_phase;
}

static int rtk_sdmmc_change_tx_phase(struct rtk_sdmmc_host *rtk_host, u8 sample_point)
{
    void __iomem *pll_base = rtk_host->pll;
    unsigned int temp_reg = 0;

    temp_reg = readl(pll_base + CR_PLL_SD1);
    temp_reg = (temp_reg & ~0x000000F8) | (sample_point << 3);
    writel(temp_reg,  pll_base + CR_PLL_SD1);

    rtk_sdmmc_sync(rtk_host);
    udelay(100);

    return 0;
}

static int rtk_sdmmc_change_rx_phase(struct rtk_sdmmc_host *rtk_host, u8 sample_point)
{
    void __iomem *pll_base = rtk_host->pll;
    unsigned int temp_reg = 0;

    temp_reg = readl(pll_base + CR_PLL_SD1);
    temp_reg = (temp_reg & ~0x00001F00) | (sample_point << 8);
    writel(temp_reg, pll_base + CR_PLL_SD1);

    rtk_sdmmc_sync(rtk_host);
    udelay(100);

    return 0;
}

static int rtk_sdmmc_tuning_tx_cmd(struct rtk_sdmmc_host *rtk_host, u8 sample_point)
{
    struct mmc_command cmd;
    struct sdmmc_cmd_pkt cmd_info;
    void __iomem *sdmmc_base = rtk_host->sdmmc;

    rtk_sdmmc_change_tx_phase(rtk_host, sample_point);

    memset(&cmd, 0x00, sizeof(struct mmc_command));
    memset(&cmd_info, 0x00, sizeof(struct sdmmc_cmd_pkt));

    cmd.opcode = MMC_SEND_STATUS;
    cmd.arg = (sdmmc_rca << RCA_SHIFTER);
    cmd_info.cmd = &cmd;
    cmd_info.rtk_host = rtk_host;
    cmd_info.rsp_para2 = 0x41;
    cmd_info.rsp_len = rtk_sdmmc_get_rsp_len(0x41);

    rtk_sdmmc_send_cmd_get_rsp(&cmd_info);
    pre_cmd=cmd.opcode;
    if(readb(sdmmc_base + SD_TRANSFER) & 0x10)
        return -1;
    return 0;
}

static int rtk_sdmmc_tuning_rx_cmd(struct rtk_sdmmc_host *rtk_host, u8 sample_point, struct scatterlist* p_sg)
{
    struct sdmmc_cmd_pkt cmd_info;
    struct mmc_request mrq = {NULL};
    struct mmc_command cmd = {0};
    struct mmc_data data = {0};
    void __iomem *sdmmc_base = rtk_host->sdmmc;

    rtk_sdmmc_change_rx_phase(rtk_host, sample_point);
    mrq.cmd = &cmd;
    mrq.data = &data;
    mrq.cmd->data = mrq.data;
    mrq.data->error = 0;
    mrq.data->mrq = &mrq;

    cmd.opcode = MMC_SEND_TUNING_BLOCK;
    cmd.arg = 0;
    cmd.flags = MMC_RSP_R1 | MMC_CMD_ADTC;

    data.blksz = 64;
    data.blocks = 1;
    data.flags = MMC_DATA_READ;
    data.sg = p_sg;
    data.sg_len = 1;

    mrq.cmd->error = 0;
    mrq.cmd->mrq = &mrq;
    mrq.cmd->data = mrq.data;
    mrq.data->error = 0;
    mrq.data->mrq = &mrq;

    memset(&cmd_info, 0x00, sizeof(struct sdmmc_cmd_pkt));
    cmd_info.cmd = mrq.cmd;
    cmd_info.rtk_host = rtk_host;
    cmd_info.rsp_para2 = rtk_sdmmc_get_rsp_type(cmd_info.cmd);
    cmd_info.rsp_len = rtk_sdmmc_get_rsp_len(cmd_info.rsp_para2);
    cmd_info.data = mrq.cmd->data;

    rtk_sdmmc_stream(&cmd_info);
    pre_cmd=cmd.opcode;
    if(readb(sdmmc_base + SD_STATUS1) & 0x01)
        return -1;

    return 0;
}

static int rtk_sdmmc_tuning_tx(struct rtk_sdmmc_host *rtk_host)
{
    int sample_point;
    int ret = 0;
    int i = 0;
    u32 raw_phase_map[TUNING_CNT] = {0};
    u32 phase_map = 0;
    u8 final_phase = 0;

    for(sample_point = 0 ; sample_point <= MAX_PHASE ; sample_point++){
        for(i = 0 ; i < TUNING_CNT ; i++){
            if (!(rtk_host->rtflags & RTKCR_FCARD_DETECTED)){
                ret = -MMC_ERR_RMOVE;
                goto out ;
            }

            ret = rtk_sdmmc_tuning_tx_cmd(rtk_host, (u8)sample_point);
            if(0 == ret)
                raw_phase_map[i] |= (1 << sample_point);
        }
    }

    phase_map = 0xFFFFFFFF;
    for(i = 0 ; i < TUNING_CNT ; i++){
        rtk_sdmmc_debug("%s TX raw_phase_map[%d] = 0x%08x\n", __func__, i, raw_phase_map[i]);
        phase_map &= raw_phase_map[i];
    }
    printk(KERN_DEBUG "%s TX phase_map = 0x%08x\n", __func__, phase_map);

    if(phase_map){
        final_phase = rtk_sdmmc_search_final_phase(rtk_host, phase_map);
        rtk_sdmmc_debug("%s final phase = 0x%08x\n", __func__, final_phase);
        if(final_phase == 0xFF){
            rtk_sdmmc_debug("%s final phase = 0x%08x\n", __func__, final_phase);
            ret = -EINVAL;
            goto out ;
        }
        rtk_sdmmc_change_tx_phase(rtk_host, final_phase);
        ret = 0;
        goto out ;
    }else{
        rtk_sdmmc_debug("%s  fail !phase_map\n", __func__);
        ret = -EINVAL;
        goto out ;
    }

out:
    return ret;
}

static int rtk_sdmmc_tuning_rx(struct rtk_sdmmc_host *rtk_host)
{
    int sample_point = 0;
    int ret = 0;
    int i = 0;
    u32 raw_phase_map[TUNING_CNT] = {0};
    u32 phase_map = 0;
    u8 final_phase = 0;
    u8 *ssr;
    struct scatterlist sg;

    ssr = kmalloc(512, GFP_KERNEL | GFP_DMA);
    if(!ssr)
        return -ENOMEM;

    sg_init_one(&sg, ssr, 512);
    for(sample_point = 0 ; sample_point <= MAX_PHASE ; sample_point++){
        for(i = 0 ; i < TUNING_CNT ; i++){
            if (!(rtk_host->rtflags & RTKCR_FCARD_DETECTED)){
                ret = -MMC_ERR_RMOVE;
                goto out ;
            }

            ret = rtk_sdmmc_tuning_rx_cmd(rtk_host, (u8)sample_point, &sg);
            if (0 == ret)
                raw_phase_map[i] |= (1 << sample_point);
        }
    }

    phase_map = 0xFFFFFFFF;
    for(i = 0 ; i < TUNING_CNT ; i++){
        rtk_sdmmc_debug("%s RX raw_phase_map[%d] = 0x%08x\n", __func__, i, raw_phase_map[i]);
        phase_map &= raw_phase_map[i];
    }
    printk(KERN_DEBUG "%s RX phase_map = 0x%08x\n", __func__, phase_map);

    if(phase_map){
        final_phase = rtk_sdmmc_search_final_phase(rtk_host, phase_map);
        rtk_sdmmc_debug("%s final phase = 0x%08x\n", __func__, final_phase);
        if (final_phase == 0xFF){
            rtk_sdmmc_debug("%s final phase = 0x%08x\n", __func__, final_phase);
            ret = -EINVAL;
            goto out ;
        }
        rtk_sdmmc_change_rx_phase(rtk_host, final_phase);
        ret = 0;
        goto out ;
    }else{
        rtk_sdmmc_debug("%s  fail !phase_map\n", __func__);
        ret = -EINVAL;
        goto out ;
    }

out:
    kfree(ssr);
    return ret;
}

static int rtk_sdmmc_wait_voltage_stable_low(struct rtk_sdmmc_host *rtk_host)
{
    u8 status = 0;
    u8 i = 0;
    void __iomem *sdmmc_base = rtk_host->sdmmc;

    while(1){
        status = readb(sdmmc_base + SD_BUS_STATUS);
        if((status & (SD_DAT3_0_LEVEL | SD_CMD_LEVEL)) == 0x0)
            break;
        msleep(3);
        if(i++>100)
            break;
    }

    return 0;
}

static int rtk_sdmmc_wait_voltage_stable_high(struct rtk_sdmmc_host *rtk_host)
{
    u8 status = 0;
    u8 i = 0;
    void __iomem *sdmmc_base = rtk_host->sdmmc;

    while(1){
        status = readb(sdmmc_base + SD_BUS_STATUS);
        if((status & (SD_DAT3_0_LEVEL | SD_CMD_LEVEL)) == (SD_DAT3_0_LEVEL | SD_CMD_LEVEL))
            break;
        msleep(3);
        if(i++>100)
            break;
    }

    return 0;
}

static void rtk_sdmmc_set_access_mode(struct rtk_sdmmc_host *rtk_host,u8 level)
{
    void __iomem *sdmmc_base = rtk_host->sdmmc;
    u32 tmp_bits = 0;

    tmp_bits = readb(sdmmc_base + SD_CONFIGURE1) & ~MODE_SEL_MASK;

    if(level == ACCESS_MODE_SD20)
        tmp_bits |= MODE_SEL_SD20;
    else if(level == ACCESS_MODE_DDR)
        tmp_bits |= MODE_SEL_DDR;
    else if(level == ACCESS_MODE_SD30)
        tmp_bits |= (MODE_SEL_SD30 | SD30_ASYNC_FIFO_RST);
    else
        tmp_bits |= MODE_SEL_SD20;

    writeb(tmp_bits, sdmmc_base + SD_CONFIGURE1);
}

static void rtk_sdmmc_set_bits(struct rtk_sdmmc_host *rtk_host, u8 set_bit)
{
    void  *sdmmc_base = rtk_host->sdmmc;
    u32 tmp_bits = 0;

    tmp_bits = readb(sdmmc_base + SD_CONFIGURE1);
    if((tmp_bits & MASK_BUS_WIDTH) != set_bit){
        tmp_bits &= ~MASK_BUS_WIDTH;
        writeb(tmp_bits | set_bit, sdmmc_base + SD_CONFIGURE1);
    }
}

static void rtk_sdmmc_set_div(struct rtk_sdmmc_host *rtk_host, u32 set_div){
    void __iomem *sdmmc_base = rtk_host->sdmmc;
    u8 tmp_div = 0;

    rtk_sdmmc_sync(rtk_host);
    tmp_div = readb(sdmmc_base + SD_CONFIGURE1) & ~MASK_CLOCK_DIV;
    if(set_div != CLOCK_DIV_NON){
        writeb(tmp_div | set_div | SDCLK_DIV, sdmmc_base + SD_CONFIGURE1);
    }else{
        writeb(tmp_div, sdmmc_base + SD_CONFIGURE1);
    }
    rtk_sdmmc_sync(rtk_host);
}

static void rtk_sdmmc_set_speed(struct rtk_sdmmc_host *rtk_host, u8 level)
{
    void __iomem *sdmmc_base = rtk_host->sdmmc;

    switch(level){
        case 0: //ddr50 , highest speed
            rtk_sdmmc_debug("%s: speed 2100\n", __func__);
            writel(0x00002100, sdmmc_base + CR_SD_CKGEN_CTL);
            break;
        case 1:
            rtk_sdmmc_debug("%s: speed 2101\n", __func__);
            writel(0x00002101, sdmmc_base + CR_SD_CKGEN_CTL);
            break;
        case 2:
            rtk_sdmmc_debug("%s: speed 2102\n", __func__);
            writel(0x00002102, sdmmc_base + CR_SD_CKGEN_CTL);
            break;
        case 3:
            rtk_sdmmc_debug("%s: speed 2103\n", __func__);
            writel(0x00002103, sdmmc_base + CR_SD_CKGEN_CTL);
            break;
        default :
            rtk_sdmmc_debug("%s: default speed 2102\n", __func__);
            writel(0x00002102, sdmmc_base + CR_SD_CKGEN_CTL);
            break;
    }
    rtk_sdmmc_sync(rtk_host);
}

static void rtk_sdmmc_speed(struct rtk_sdmmc_host *rtk_host, enum sdmmc_clock_speed sd_speed)
{
    void __iomem *sdmmc_base = rtk_host->sdmmc;
    void __iomem *emmc_base = rtk_host->emmc;
    void __iomem *pll_base = rtk_host->pll;
    void __iomem *sysbrdg_base = rtk_host->sysbrdg;

    unsigned long flags2;
	
    switch(sd_speed){
        case SDMMC_CLOCK_200KHZ:
            rtk_sdmmc_debug("%s: speed SDMMC_CLOCK_200KHZ\n", __func__);
            rtk_sdmmc_set_div(rtk_host, CLOCK_DIV_256); //0x580 = 0xd0
            rtk_sdmmc_set_speed(rtk_host, 1); //0x478 = 0x2101
            break;
        case SDMMC_CLOCK_400KHZ:
            rtk_sdmmc_debug("%s: speed SDMMC_CLOCK_400KHZ\n", __func__);
            rtk_sdmmc_set_div(rtk_host, CLOCK_DIV_256); //0x580 = 0xd0
            rtk_sdmmc_set_speed(rtk_host, 0); //0x478 = 0x2100
            break;
        case SDMMC_CLOCK_6200KHZ:
            rtk_sdmmc_debug("%s: speed SDMMC_CLOCK_6200KHZ\n", __func__);
            rtk_sdmmc_set_div(rtk_host, CLOCK_DIV_NON); //0x580 = 0x10
            rtk_sdmmc_set_speed(rtk_host, 3); //0x478 = 0x2103
            break;
        case SDMMC_CLOCK_25000KHZ:
            rtk_sdmmc_debug("%s: speed SDMMC_CLOCK_25000KHZ\n", __func__);
            if(rtk_host->mmc->ios.timing == MMC_TIMING_UHS_SDR12){
                rtk_sdmmc_set_div(rtk_host, CLOCK_DIV_NON); //0x580 = 0x10
                rtk_sdmmc_set_speed(rtk_host, 2); //0x478 = 0x2101
            }else{
                rtk_sdmmc_set_div(rtk_host, CLOCK_DIV_NON); //0x580 = 0x10
                rtk_sdmmc_set_speed(rtk_host, 1); //0x478 = 0x2101
            }
            break;
        case SDMMC_CLOCK_50000KHZ:
            rtk_sdmmc_debug("%s: speed SDMMC_CLOCK_50000KHZ\n", __func__);
            if(rtk_host->mmc->ios.timing == MMC_TIMING_UHS_DDR50){

                //writel(readl(pll_base + CR_PLL_SD2) | 0x00000003, pll_base + CR_PLL_SD2); //PLL_SD2

                writel(readl(sdmmc_base + CR_SD_CKGEN_CTL) | 0x00070000, sdmmc_base + CR_SD_CKGEN_CTL); //Switch SD source clock to 4MHz by Hsin-yin

		if(readl(sysbrdg_base+0x204)!=0x0)
        		writel(0x40000000,sdmmc_base+0x2c);
                writel(0x00000006, pll_base + 0x01EC);           //JIM modified, 1AC->1EC
                writel(0x00324388,  pll_base + CR_PLL_SD3);
                mdelay(2);
                writel(0x00000007, pll_base + 0x01EC);            //JIM modified, 1AC->1EC	
		if(readl(sysbrdg_base+0x204)!=0x0)
                        writel(0x00000000,sdmmc_base+0x2c);

                writel(readl(sdmmc_base + CR_SD_CKGEN_CTL) & 0xFFF8FFFF, sdmmc_base + CR_SD_CKGEN_CTL); //Switch SD source clock to normal clock source by Hsin-yin
                writeb(readb(sdmmc_base + SD_CONFIGURE1) & 0xEF, sdmmc_base + SD_CONFIGURE1); //Reset FIFO pointer by Hsin-yin
                rtk_sdmmc_set_div(rtk_host, CLOCK_DIV_NON); //0x580 = 0x10
                rtk_sdmmc_set_speed(rtk_host, 0); //0x478 = 0x2100
            }else if(rtk_host->mmc->ios.timing == MMC_TIMING_UHS_SDR25){
                rtk_sdmmc_set_div(rtk_host, CLOCK_DIV_NON); //0x580 = 0x10
                rtk_sdmmc_set_speed(rtk_host, 1); //0x478 = 0x2101
            }else{
                rtk_sdmmc_set_div(rtk_host, CLOCK_DIV_NON); //0x580 = 0x10
                rtk_sdmmc_set_speed(rtk_host, 0); //0x478 = 0x2100
            }
            break;
        case SDMMC_CLOCK_100000KHZ:
            rtk_sdmmc_debug("%s: speed SDMMC_CLOCK_100000KHZ\n", __func__);
      
            rtk_lockapi_lock(flags2, __FUNCTION__);
            writel(0x00005555, emmc_base + 0x634);         //JIM modified this part from 3333 to 5555 to enlarge the signal to work around a issue that can not recognize some SD 3.0 card
            writel(0x55555555, emmc_base + 0x638);         //JIM modified this part from 33333333 to 55555555
            rtk_lockapi_unlock(flags2, __FUNCTION__);

            rtk_sdmmc_set_div(rtk_host, CLOCK_DIV_NON); //0x580 = 0x10
            rtk_sdmmc_set_speed(rtk_host, 0); //0x478 = 0x2100
            break;
        case SDMMC_CLOCK_208000KHZ:
            rtk_sdmmc_debug("%s: speed SDMMC_CLOCK_208000KHZ\n", __func__);

	    rtk_lockapi_lock(flags2, __FUNCTION__);
	    switch(driving_capacity%10) {
                case 0:
                case 1:
                writel(0x0000aaaa, emmc_base + 0x634);         //JIM modified this part for dyamically adjusting driving capacity
                writel(0xaaaaaaaa, emmc_base + 0x638);
                break;
                case 2:
                case 3:
                writel(0x0000bbbb, emmc_base + 0x634);
                writel(0xbbbbbbbb, emmc_base + 0x638);
                break;
                case 4:
                case 5:
                writel(0x0000cccc, emmc_base + 0x634);
                writel(0xcccccccc, emmc_base + 0x638);
                break;
                case 6:
                case 7:
                writel(0x0000dddd, emmc_base + 0x634);
                writel(0xdddddddd, emmc_base + 0x638);
                break;
                case 8:
                case 9:
                writel(0x0000eeee, emmc_base + 0x634);
                writel(0xeeeeeeee, emmc_base + 0x638);
                break;
                default:
                writel(0x0000aaaa, emmc_base + 0x634);
                writel(0xaaaaaaaa, emmc_base + 0x638);
            }

	    rtk_lockapi_unlock(flags2, __FUNCTION__);	    

	    if(readl(sysbrdg_base+0x204)!=0x0)
                        writel(0x40000000,sdmmc_base+0x2c);    
            writel(0x00000006, pll_base + 0x01EC);         //JIM modified, 1AC->1EC
            writel(readl(sdmmc_base + CR_SD_CKGEN_CTL) | 0x00070000, sdmmc_base + CR_SD_CKGEN_CTL); //Switch SD source clock to 4MHz by Hsin-yin
            mdelay(2);
            writel(0x00000007, pll_base + 0x01EC);          //JIM modified, 1AC->1EC
	    if(readl(sysbrdg_base+0x204)!=0x0)
                        writel(0x00000000,sdmmc_base+0x2c);
	    
	    if(readl(sysbrdg_base+0x204)!=0x0) {
                writel(0x40000000,sdmmc_base+0x2c);
		mdelay(100);
            	writel(0x00000006, pll_base + 0x01EC);            //JIM modified, 1AC->1EC
            	writel(0x00b64388, pll_base + CR_PLL_SD3); //SD clock rate formula: (ssc_div_n +3) *4.5/4, jamestai20141222
            	mdelay(2);
            	writel(0x00000007, pll_base + 0x01EC);             //JIM modified, 1AC->1EC
		writel(0x00000000,sdmmc_base+0x2c);
	    }
	    else {
            	/*Workaround: PLL clcok 208 MHz  */
            	writel(0x00000006, pll_base + 0x01EC);           //JIM modified, 1AC->1EC
            	writel(0x00b74388, pll_base + CR_PLL_SD3); //SD clock rate formula: (ssc_div_n +3) *4.5/4, jamestai20141222
            	mdelay(2);
            	writel(0x00000007, pll_base + 0x01EC);           //JIM modified, 1AC->1EC

            	mdelay(100);
            	writel(0x00000006, pll_base + 0x01EC);            //JIM modified, 1AC->1EC
            	writel(0x00b64388, pll_base + CR_PLL_SD3); //SD clock rate formula: (ssc_div_n +3) *4.5/4, jamestai20141222
            	mdelay(2);
            	writel(0x00000007, pll_base + 0x01EC);             //JIM modified, 1AC->1EC
	    }

	    if(readl(sysbrdg_base+0x204)!=0x0)
		writel(0x40000000,sdmmc_base+0x2c);
            writel(0x00000006, pll_base + 0x01EC);              //JIM modified, 1AC->1EC
            writel(readl(sdmmc_base + CR_SD_CKGEN_CTL) & 0xFFF8FFFF, sdmmc_base + CR_SD_CKGEN_CTL); //Switch SD source clock to normal clock source by Hsin-yin
            mdelay(2);
            writel(0x00000007, pll_base + 0x01EC);              //JIM modified, 1AC->1EC 
	    if(readl(sysbrdg_base+0x204)!=0x0)
                writel(0x00000000,sdmmc_base+0x2c);

            writeb(readb(sdmmc_base + SD_CONFIGURE1) & 0xEF, sdmmc_base + SD_CONFIGURE1); //Reset FIFO pointer by Hsin-yin

            rtk_sdmmc_set_div(rtk_host, CLOCK_DIV_NON); //0x580 = 0x10
            rtk_sdmmc_set_speed(rtk_host, 0); //0x478 = 0x2100

            break;
        default :
            rtk_sdmmc_debug("%s: default speed SDMMC_CLOCK_400KHZ\n", __func__);
            rtk_sdmmc_set_div(rtk_host, CLOCK_DIV_256); //0x580 = 0xd0
            rtk_sdmmc_set_speed(rtk_host, 0); //0x478 = 0x2100
            break;
    }

    rtk_sdmmc_sync(rtk_host);
}

static int rtk_sdmmc_allocate_dma_buf(struct rtk_sdmmc_host *rtk_host, struct mmc_command *cmd)
{
    if(!dma_virt_addr){
        dma_virt_addr = dma_alloc_coherent(rtk_host->dev, SD_ALLOC_LENGTH, &rtk_host->paddr, GFP_KERNEL);
        dma_phy_addr = (u32 *)rtk_host->paddr;
    }else
        return 0;

    if(!dma_virt_addr){
        WARN_ON(1);
        printk(KERN_ERR "%s: allocate rtk sd dma buf FAIL !\n", __func__);
        cmd->error = -ENOMEM;
        return 0;
    }

    return 1;
}

static int rtk_sdmmc_free_dma_buf(struct rtk_sdmmc_host *rtk_host)
{
    if (dma_virt_addr)
        dma_free_coherent(rtk_host->dev, SD_ALLOC_LENGTH, dma_virt_addr ,rtk_host->paddr);
    else
        return 0;

    return 1;
}

static u32 rtk_sdmmc_swap_endian(u32 input)
{
    u32 output = 0;
    output = (input & 0xFF000000) >> 24 |
             (input & 0x00FF0000) >> 8 |
             (input & 0x0000FF00) << 8 |
             (input & 0x000000FF) << 24;
    return output;
}

static void rtk_sdmmc_read_rsp(struct rtk_sdmmc_host *rtk_host, u32 *rsp, int reg_count)
{
    void __iomem *sdmmc_base = rtk_host->sdmmc;

    if(reg_count == 6){
        rsp[0] = readb(sdmmc_base + SD_CMD1) << 24 |
                 readb(sdmmc_base + SD_CMD2) << 16 |
                 readb(sdmmc_base + SD_CMD3) << 8 |
                 readb(sdmmc_base + SD_CMD4);
    }else if(reg_count == 16){
        rsp[0] = rtk_sdmmc_swap_endian(rsp[0]);
        rsp[1] = rtk_sdmmc_swap_endian(rsp[1]);
        rsp[2] = rtk_sdmmc_swap_endian(rsp[2]);
        rsp[3] = rtk_sdmmc_swap_endian(rsp[3]);
    }
}

static u32 rtk_sdmmc_get_cmd_timeout(struct sdmmc_cmd_pkt *cmd_info)
{
    struct rtk_sdmmc_host *rtk_host = cmd_info->rtk_host;
    u32 timeout = 0;

    timeout += msecs_to_jiffies(1);
    cmd_info->timeout = rtk_host->timeout = timeout;

    return 0;
}

int rtk_sdmmc_cpu_wait(char* drv_name, struct rtk_sdmmc_host *rtk_host, u8 cmdcode){

    int ret = CR_TRANSFER_TO;
    unsigned long old_jiffles = jiffies;
    unsigned long timeout = 0;
    void __iomem *sdmmc_base = rtk_host->sdmmc;
poll=1;

    writel(0x00000016, sdmmc_base + CR_SD_ISREN); //disable all
    switch( data_xfer_mode ) {
        case CMD_RSP_ONLY:
        case CMD_RSP_DATA_WRITE:
            writel(0x00000007, sdmmc_base + CR_SD_ISREN);
            break;
        case CMD_RSP_DATA_READ:
        case DATA_ONLY:
            writel(0x00000015, sdmmc_base + CR_SD_ISREN);
            break;
        default:
            writel(0x00000007, sdmmc_base + CR_SD_ISREN);
            break;
    }
    rtk_sdmmc_sync(rtk_host);

    writeb((u8)(cmdcode | START_EN), sdmmc_base + SD_TRANSFER);

    timeout = 300;
    while(time_before(jiffies, old_jiffles + timeout)){

        rtk_sdmmc_sync(rtk_host);
        if((readb(sdmmc_base + SD_STATUS2) & 0x01) == 1){
            ret = CR_TRANSFER_FAIL;
            break;
        }

        if(!(rtk_host->rtflags & RTKCR_FCARD_DETECTED)){
            ret = CR_TRANSFER_FAIL;
            break;
        }

        if((readb(sdmmc_base + SD_TRANSFER) & (END_STATE | IDLE_STATE)) == (END_STATE | IDLE_STATE)){
            ret = CR_TRANS_OK;
            break;
        }

        if((readb(sdmmc_base + SD_TRANSFER) & (ERR_STATUS)) == (ERR_STATUS)){
            ret = CR_TRANSFER_FAIL;
            printk(KERN_ERR "%s(%d) trans error(error status) :\n trans: 0x%08x, st1: 0x%08x, st2: 0x%08x, bus: 0x%08x\n",
                    __func__,
                    __LINE__,
                    readb(sdmmc_base + SD_TRANSFER),
                    readb(sdmmc_base + SD_STATUS1),
                    readb(sdmmc_base + SD_STATUS2),
                    readb(sdmmc_base + SD_BUS_STATUS));
	    if(rtk_host!=NULL && rtk_host->mrq!=NULL && rtk_host->mrq->cmd!=NULL) printk(KERN_ERR "%s: error opcode = %d\n", __func__, rtk_host->mrq->cmd->opcode);
            break;
        }
    }

    if (ret == CR_TRANSFER_TO){
        printk(KERN_ERR "%s(%d) trans error(timeout) :\n trans: 0x%08x, st1: 0x%08x, st2: 0x%08x, bus: 0x%08x\n",
                __func__,
                __LINE__,
                readb(sdmmc_base+SD_TRANSFER),
                readb(sdmmc_base+SD_STATUS1),
                readb(sdmmc_base+SD_STATUS2),
                readb(sdmmc_base+SD_BUS_STATUS));
        if(rtk_host!=NULL && rtk_host->mrq!=NULL && rtk_host->mrq->cmd!=NULL) printk(KERN_ERR "%s: error opcode = %d\n", __func__, rtk_host->mrq->cmd->opcode);
        return ret;
    }

    return ret;
}

int rtk_sdmmc_int_wait(char* drv_name, struct rtk_sdmmc_host *rtk_host, u8 cmdcode)
{
    int ret = CR_TRANSFER_TO;
    unsigned long timeout = 0;
    unsigned int sd_trans = 0;
    unsigned long old_jiffles = jiffies;
    void __iomem *sdmmc_base = rtk_host->sdmmc;
    int cmd_type=0;

poll=0;
    if(broken_flag==true) return CR_TRANSFER_FAIL;
    /* timeout timer fire */
    if (&rtk_host->timer){
	if(rtk_host!=NULL && rtk_host->mrq!=NULL && rtk_host->mrq->cmd!=NULL &&
	   (rtk_host->mrq->cmd->opcode==55 || rtk_host->mrq->cmd->opcode==41 || rtk_host->mrq->cmd->opcode==8 || (pre_cmd==41 && rtk_host->mrq->cmd->opcode==1))){
		timeout = msecs_to_jiffies(100) + rtk_host->timeout;
		cmd_type=1;
	}
	else if(rtk_host!=NULL && rtk_host->mrq!=NULL && rtk_host->mrq->cmd!=NULL && (rtk_host->mrq->cmd->opcode==2 || rtk_host->mrq->cmd->opcode==3 || rtk_host->mrq->cmd->opcode==9
		|| (pre_cmd==0 && rtk_host->mrq->cmd->opcode==1))) {
		timeout = msecs_to_jiffies(600) + rtk_host->timeout;
		cmd_type=2;
	}
        else {
		timeout = msecs_to_jiffies(3000) + rtk_host->timeout;
		cmd_type=3;
	}
        mod_timer(&rtk_host->timer, (jiffies + timeout));
    }
    writel(0x00000016, sdmmc_base + CR_SD_ISREN); //disable all
    switch( data_xfer_mode ) {
        case CMD_RSP_ONLY:
        case DATA_WRITE_ONLY:
        case CMD_RSP_DATA_WRITE:
        case CMD_RSP_DATA_READ_TUNNING:
            writel(0x00000007, sdmmc_base + CR_SD_ISREN);
            break;
        case CMD_RSP_DATA_READ:
            writel(0x00000015, sdmmc_base + CR_SD_ISREN);
            break;
        default:
            writel(0x00000007, sdmmc_base + CR_SD_ISREN);
    }

    rtk_sdmmc_sync(rtk_host);

    irq_counter=0;
    irq_error_bit=0;
    writeb((u8) (cmdcode | START_EN), sdmmc_base + SD_TRANSFER); //cmd fire

    wait_for_completion(rtk_host->int_waiting);

    rtk_sdmmc_sync(rtk_host);
    sd_trans = readb(sdmmc_base + SD_TRANSFER);

    if(irq_error_bit==1) {	//error handling
	if (rtk_host->cmd_opcode==55 && CMD8_SD1==true) {
		CMD55_MMC=true; //MMC card
		printk(KERN_ERR "Reject SD 1.x command\n");
		ret = CR_TRANSFER_FAIL;
	}
	else {
		if(rtk_host!=NULL)
			printk(KERN_ERR "rtk_sdmmc_int_wait: reset the card and adjust the driving capacity, cmd=%d, SD_TRANSFER=0x%x\n",rtk_host->cmd_opcode, sd_trans);
		driving_capacity++;
		remove_sdcard(rtk_host);
		ret = CR_TRANS_OK;
	}
    }
    else if((sd_trans & (END_STATE | IDLE_STATE)) == (END_STATE | IDLE_STATE))
        ret = CR_TRANS_OK;
    else {
	if(cmd_type==1) timeout = 10;
	else if(cmd_type==2) timeout = 60;
	else timeout = 300;
        while(time_before(jiffies, old_jiffles + timeout)){
            rtk_sdmmc_sync(rtk_host);
            sd_trans = readb(sdmmc_base + SD_TRANSFER);
            if((sd_trans & (END_STATE | IDLE_STATE)) == (END_STATE | IDLE_STATE)){
                ret = CR_TRANS_OK;
                break;
            }
	    else if((sd_trans & (ERR_STATUS)) == (ERR_STATUS)) {
		if((num_online_cpus()==1||XEN) && rtk_host!=NULL && rtk_host->cmd_opcode==8) CMD8_SD1=true;
		if((num_online_cpus()==1||XEN) && rtk_host!=NULL && rtk_host->cmd_opcode==55 && CMD8_SD1==true) CMD55_MMC=true;
		ret = CR_TRANSFER_FAIL;
		printk(KERN_ERR "%s(%d) trans error(error status) :\n cfg1: 0x%02x, cfg2: 0x%02x, cfg3: 0x%02x, trans: 0x%08x, st1: 0x%08x, st2: 0x%08x, bus: 0x%08x\n",
                        __func__,
                        __LINE__,
                        readb(sdmmc_base + SD_CONFIGURE1),
                        readb(sdmmc_base + SD_CONFIGURE2),
                        readb(sdmmc_base + SD_CONFIGURE3),
                        readb(sdmmc_base + SD_TRANSFER),
                        readb(sdmmc_base + SD_STATUS1),
                        readb(sdmmc_base + SD_STATUS2),
                        readb(sdmmc_base + SD_BUS_STATUS));
                if(rtk_host!=NULL && rtk_host->mrq!=NULL && rtk_host->mrq->cmd!=NULL) printk(KERN_ERR "%s: error opcode = %d\n", __func__, rtk_host->mrq->cmd->opcode);
		if(rtk_host!=NULL && rtk_host->mrq!=NULL && rtk_host->mrq->cmd!=NULL && rtk_host->mrq->cmd->opcode!=13 && rtk_host->mrq->cmd->opcode!=19 && CMD8_SD1!=true && CMD55_MMC!=true) {
			driving_capacity++;
			remove_sdcard(rtk_host);
			ret = CR_TRANS_OK;
		}
		break;
	    }else if(rtk_host!=NULL && !(rtk_host->rtflags & RTKCR_FCARD_DETECTED)){
                ret = CR_TRANSFER_FAIL;
		break;
	    }
	}
    }
    
    if(ret == CR_TRANSFER_TO){
	if((num_online_cpus()==1||XEN) && rtk_host!=NULL && rtk_host->cmd_opcode==8) CMD8_SD1=true;
        if((num_online_cpus()==1||XEN) && rtk_host!=NULL && rtk_host->cmd_opcode==55 && CMD8_SD1==true) CMD55_MMC=true;
        printk(KERN_ERR "%s(%d) trans error(timeout) :\n trans: 0x%08x, st1: 0x%08x, st2: 0x%08x, bus: 0x%08x\n",
                __func__,
                __LINE__,
                readb(sdmmc_base + SD_TRANSFER),
                readb(sdmmc_base + SD_STATUS1),
                readb(sdmmc_base + SD_STATUS2),
                readb(sdmmc_base + SD_BUS_STATUS));
        if(rtk_host!=NULL && rtk_host->mrq!=NULL && rtk_host->mrq->cmd!=NULL) printk(KERN_ERR "%s: error opcode = %d\n", __func__, rtk_host->mrq->cmd->opcode);
    }
    return ret;
}

static int rtk_sdmmc_set_rspparam(struct sdmmc_cmd_pkt *cmd_info)
{
    rtk_sdmmc_debug("%s: opcode = %d\n", __func__, cmd_info->cmd->opcode);
    switch(cmd_info->cmd->opcode){
        case MMC_GO_IDLE_STATE: //cmd 0
            cmd_info->rsp_para1 = 0xD0; //SD1_R0;
            cmd_info->rsp_para2 = 0x74; //0x50 | SD_R0;
            cmd_info->rsp_para3 = 0x00;
            break;
        case MMC_SEND_OP_COND:
            if(mmc_detected == 1){
                cmd_info->rsp_para1 = SD1_R0;
                cmd_info->rsp_para2 = 0x45;
                cmd_info->rsp_para3 = 0x01;
                cmd_info->cmd->arg = MMC_VDD_30_31 | MMC_VDD_31_32 | MMC_VDD_32_33 | MMC_VDD_33_34 | MMC_SECTOR_ADDR;
            }
            break;
        case MMC_ALL_SEND_CID: //cmd 2
            cmd_info->rsp_para1 = -1; //SD1_R0;
            cmd_info->rsp_para2 = 0x42; //SD_R2;
            cmd_info->rsp_para3 = 0x05; //SD2_R0;
            break;
        case MMC_SET_RELATIVE_ADDR: //cmd3
            cmd_info->rsp_para1 = -1; //SD1_R0;
            cmd_info->rsp_para2 = 0x41; //SD_R1 | CRC16_CAL_DIS;
            cmd_info->rsp_para3 = 0x05;
            break;
        case MMC_SET_DSR: //cmd4
            cmd_info->rsp_para1 = -1; //don't update
            cmd_info->rsp_para3 = -1; //don't update
            break;
        case SD_APP_SET_BUS_WIDTH: //cmd6
            if((cmd_info->cmd->flags & (0x3 << 5)) == MMC_CMD_ADTC){ //SD_SWITCH
                cmd_info->rsp_para1 = -1;
                cmd_info->rsp_para2 = 0x01; //SD_R1b | CRC16_CAL_DIS;
                cmd_info->rsp_para3 = 0x05;
            }else{
                cmd_info->rsp_para1 = -1;
                if(mmc_detected == 1){
                    cmd_info->rsp_para2 = 0x09; //SD_R1b | CRC16_CAL_DIS;
                    cmd_info->rsp_para3 = 0x05;
                }else{
                    cmd_info->rsp_para2 = 0x61; //SD_R1b | CRC16_CAL_DIS;
                    cmd_info->rsp_para3 = 0x0D;
                }
            }
            break;
        case MMC_SELECT_CARD: //cmd7
	    if(cmd_info->cmd->flags == (MMC_RSP_R1 | MMC_CMD_AC)) {  //select
            	cmd_info->rsp_para1 = -1;
            	cmd_info->rsp_para2 = 0x41; //SD_R1b|CRC16_CAL_DIS;
            	cmd_info->rsp_para3 = 0x05;
	    }
	    else {	//deselect
		cmd_info->rsp_para1 = -1;
                cmd_info->rsp_para2 = 0x40; //SD_R0|CRC16_CAL_DIS;
                cmd_info->rsp_para3 = 0x05;
	    }
            break;
        case SD_SEND_IF_COND: //cmd8
            cmd_info->rsp_para1 = 0xD0;
            cmd_info->rsp_para2 = 0x41; //SD_R1;
	    
            cmd_info->rsp_para3 = 0x0;
            if (cmd_info->rtk_host->rtflags & RTKCR_FCARD_SELECTED && mmc_detected == 1){
                cmd_info->rsp_para1 = 0x50;
                cmd_info->rsp_para2 = SD_R1;
                cmd_info->rsp_para3 = 0x05;
            }
            break;
        case MMC_SEND_CSD: //cmd9
            cmd_info->rsp_para1 = -1;
            cmd_info->rsp_para2 = 0x42; //SD_R2;
            cmd_info->rsp_para3 = 0x05;
            break;
        case MMC_SEND_CID://cmd10
            cmd_info->rsp_para1 = -1;
            cmd_info->rsp_para2 = 0x42; //SD_R2;
            cmd_info->rsp_para3 = 0x5;
            break;
        case SD_SWITCH_VOLTAGE: //cmd11
            cmd_info->rsp_para1 = -1; //0;
            cmd_info->rsp_para2 = 0x41; //don't update
            cmd_info->rsp_para3 = 0x00; //don't update
            break;
        case MMC_STOP_TRANSMISSION: //cmd12
            cmd_info->rsp_para1 = -1;
	    if(suspend_flag==false) cmd_info->rsp_para2 = 0x7D; //SD_R1 | CRC16_CAL_DIS | 6 bytes response;
	    else cmd_info->rsp_para2 = 0x7C;
            cmd_info->rsp_para3 = 0x00;
            break;
        case SD_APP_SD_STATUS: //cmd13
            if((cmd_info->cmd->flags & (0x3<<5)) == MMC_CMD_ADTC){ //SD_APP_SD_STATUS
                cmd_info->rsp_para1 = -1;
                cmd_info->rsp_para2 = 0x01; //SD_R1 | CRC16_CAL_DIS;
                cmd_info->rsp_para3 = 0x05;
            }else{
                cmd_info->rsp_para1 = -1;
                cmd_info->rsp_para2 = 0x41; //SD_R1 | CRC16_CAL_DIS;
                cmd_info->rsp_para3 = -1;
            }
            break;
        case MMC_GO_INACTIVE_STATE: //cmd15
            cmd_info->rsp_para1 = -1; //don't update
            cmd_info->rsp_para3 = -1; //don't update
            break;
        case MMC_SET_BLOCKLEN: //cmd16
            cmd_info->rsp_para1 = -1;
            cmd_info->rsp_para2 = 0x41;
            cmd_info->rsp_para3 = 0x05;
            break;
        case MMC_READ_SINGLE_BLOCK: //cmd17
            cmd_info->rsp_para1 = -1;
            cmd_info->rsp_para2 = 0x1;
            cmd_info->rsp_para3 = -1;
            break;
        case MMC_READ_MULTIPLE_BLOCK: //cmd18
            cmd_info->rsp_para1 = -1;
            cmd_info->rsp_para2 = 0x01;
            if(mmc_detected == 1){
                cmd_info->rsp_para3 = 0x04; //0x04;
            }else{
                cmd_info->rsp_para3 = 0x00;//0x02;  ----jim
            }
            break;
        case MMC_SEND_TUNING_BLOCK: //cmd19
            cmd_info->rsp_para1 = -1;
            cmd_info->rsp_para2 = 0x01;
            cmd_info->rsp_para3 = -1; //0x04;
            break;
        case MMC_WRITE_DAT_UNTIL_STOP: //cmd20
            cmd_info->rsp_para1 = -1; //don't update
            cmd_info->rsp_para3 = -1; //don't update
            break;
        case SD_APP_SEND_NUM_WR_BLKS: //cmd22
            cmd_info->rsp_para1 = -1; //don't update
            cmd_info->rsp_para3 = -1; //don't update
            break;
        case MMC_SET_BLOCK_COUNT: //cmd 23
            cmd_info->rsp_para1 = -1; //don't update
            cmd_info->rsp_para3 = -1; //don't update
            break;
        case MMC_WRITE_BLOCK: //cmd24
            cmd_info->rsp_para1 = -1;
            cmd_info->rsp_para2 = 0x01;
            cmd_info->rsp_para3 = -1;
            break;
        case MMC_WRITE_MULTIPLE_BLOCK: //cmd25
            cmd_info->rsp_para1 = -1;
            cmd_info->rsp_para2 = 0x01;
            cmd_info->rsp_para3 = 0x00;//0x02;--------jim
            break;
        case MMC_PROGRAM_CSD: //cmd27
            cmd_info->rsp_para1 = -1; //don't update
            cmd_info->rsp_para3 = -1; //don't update
            break;
        case MMC_SET_WRITE_PROT: //cmd28
            cmd_info->rsp_para1 = -1; //don't update
            cmd_info->rsp_para3 = -1; //don't update
            break;
        case MMC_CLR_WRITE_PROT: //cmd29
            cmd_info->rsp_para1 = -1; //don't update
            cmd_info->rsp_para3 = -1; //don't update
            break;
        case MMC_SEND_WRITE_PROT: //cmd30
            cmd_info->rsp_para1 = -1; //don't update
            cmd_info->rsp_para3 = -1; //don't update
            break;
        case SD_ERASE_WR_BLK_START: //cmd32
            cmd_info->rsp_para1 = -1; //don't update
            cmd_info->rsp_para3 = -1; //don't update
            break;
        case SD_ERASE_WR_BLK_END: //cmd33
            cmd_info->rsp_para1 = -1; //don't update
            cmd_info->rsp_para3 = -1; //don't update
            break;
        case MMC_ERASE: //cmd38
            cmd_info->rsp_para1 = -1; //don't update
            cmd_info->rsp_para3 = -1; //don't update
            break;
        case SD_APP_OP_COND: //cmd41
            cmd_info->rsp_para1 = -1;
            cmd_info->rsp_para2 = 0x45;
            cmd_info->rsp_para3 = 0x00;
            break;
        case MMC_LOCK_UNLOCK: //cmd42
            if((cmd_info->cmd->flags & (0x3<<5)) == MMC_CMD_ADTC){ //SD_APP_SD_STATUS
                cmd_info->rsp_para1 = -1; //don't update
                cmd_info->rsp_para3 = -1; //don't update
            }else{
                cmd_info->rsp_para1 = -1; //don't update
                cmd_info->rsp_para3 = -1; //don't update
            }
            break;
        case SD_APP_SEND_SCR: //cmd51
            cmd_info->rsp_para1 = -1;
            cmd_info->rsp_para2 = 0x41;
            cmd_info->rsp_para3 = 0x05;
            break;
        case MMC_APP_CMD: //cmd55
            cmd_info->rsp_para1 = -1;
            cmd_info->rsp_para2 = 0x41;
            cmd_info->rsp_para3 = 0x05;
            break;
        case MMC_GEN_CMD: //cmd56
	    if(cmd_info->cmd->arg & 0x1) {   //read single block
                cmd_info->cmd->arg = 0x1;
                cmd_info->rsp_para1 = -1;
                cmd_info->rsp_para2 = 0x1;
                cmd_info->rsp_para3 = -1;
            }
            else {                      //write single block
                cmd_info->cmd->arg = 0x0;
                cmd_info->rsp_para1 = -1;
                cmd_info->rsp_para2 = 0x01;
                cmd_info->rsp_para3 = -1;
            }
            break;
        case MMC_EXT_READ_SINGLE: //cmd48
            cmd_info->rsp_para1 = -1;
            cmd_info->rsp_para2 = 0x01;
            cmd_info->rsp_para3 = -1;
            break;
        case MMC_EXT_WRITE_SINGLE: //cmd49
            cmd_info->rsp_para1 = -1;
            cmd_info->rsp_para2 = 0x01;
            cmd_info->rsp_para3 = -1;
            break;
        case MMC_EXT_READ_MULTIPLE: //cmd58
            cmd_info->rsp_para1 = -1;
            cmd_info->rsp_para2 = 0x01;
            if(mmc_detected == 1)
                cmd_info->rsp_para3 = 0x04;
            else
                cmd_info->rsp_para3 = 0x02;
            break;
        case MMC_EXT_WRITE_MULTIPLE: //cmd59
            cmd_info->rsp_para1 = -1;
            cmd_info->rsp_para2 = 0x01;
            cmd_info->rsp_para3 = 0x02;
            break;
        default:
            cmd_info->rsp_para1 = -1; //don't update
            cmd_info->rsp_para3 = -1; //don't update
            break;
    }

    return 0;
}

/*static int rtk_scan_fun(struct rtk_sdmmc_host *rtk_host) {
	while(1) {
		u32 int_status = 0;

    		rtk_sdmmc_sync(rtk_host);
    		int_status = readl(rtk_host->sdmmc + CR_SD_ISR);
    		if(int_status & (ISRSTA_INT1 | ISRSTA_INT2 | ISRSTA_INT4)) {
			TOGGLE_GPIO_LOW18;
			TOGGLE_GPIO_HIGH18;		
		}
	}
}*/

static int rtk_sdmmc_send_stop_cmd(struct mmc_command *cmd, struct rtk_sdmmc_host *rtk_host,int flag)
{
    u8 cmd_idx = 0;
    u32 sd_arg = 0;
    s8 rsp_para1 = 0;
    s8 rsp_para2 = 0;
    s8 rsp_para3 = 0;
    u8 rsp_len = 0;
    int ret = 0;

    void __iomem *sdmmc_base = rtk_host->sdmmc;
    struct mmc_command stop_cmd;
    struct sdmmc_cmd_pkt stop_cmd_info;
    struct sdmmc_cmd_pkt *cmd_info;

    INT4_flag=false;

    cmd_info = &stop_cmd_info;

    memset(&stop_cmd, 0x00, sizeof(struct mmc_command));
    memset(&stop_cmd_info, 0x00, sizeof(struct sdmmc_cmd_pkt));

    stop_cmd.opcode = MMC_STOP_TRANSMISSION;
    stop_cmd.arg = 0x00;    //cmd->arg; //card's RCA, not necessary for cmd12
    stop_cmd_info.cmd = &stop_cmd;
    stop_cmd_info.rtk_host = rtk_host;
    stop_cmd_info.rsp_para2 = rtk_sdmmc_get_rsp_type(stop_cmd_info.cmd);
    stop_cmd_info.rsp_len = rtk_sdmmc_get_rsp_len(stop_cmd_info.rsp_para2);
    cmd_idx = cmd_info->cmd->opcode;
    rsp_len = cmd_info->rsp_len;
    rtk_host = cmd_info->rtk_host;

    rtk_sdmmc_set_rspparam(cmd_info);

    sd_arg = cmd_info->cmd->arg;
    rsp_para1 = cmd_info->rsp_para1;
    rsp_para2 = cmd_info->rsp_para2;
    rsp_para3 = cmd_info->rsp_para3;

    if (rsp_para1 != -1)
        writeb(rsp_para1, sdmmc_base + SD_CONFIGURE1);

    writeb(rsp_para2, sdmmc_base + SD_CONFIGURE2);

    if (rsp_para3 != -1)
        writeb(rsp_para3, sdmmc_base + SD_CONFIGURE3);

    g_cmd[0] = (0x40 | cmd_idx);
    g_cmd[1] = (sd_arg >> 24) & 0xff;
    g_cmd[2] = (sd_arg >> 16) & 0xff;
    g_cmd[3] = (sd_arg >> 8) & 0xff;
    g_cmd[4] = sd_arg & 0xff;
    g_cmd[5] = 0x00;

    writeb(g_cmd[0], sdmmc_base + SD_CMD0);
    writeb(g_cmd[1], sdmmc_base + SD_CMD1);
    writeb(g_cmd[2], sdmmc_base + SD_CMD2);
    writeb(g_cmd[3], sdmmc_base + SD_CMD3);
    writeb(g_cmd[4], sdmmc_base + SD_CMD4);
    writeb(g_cmd[5], sdmmc_base + SD_CMD5);
    rtk_host->cmd_opcode = cmd_idx;
//printk(KERN_ERR "send_stop_cmd: pre_cmd=%d, cmd=%d, INT4_flag=%d\n",pre_cmd,cmd_idx,INT4_flag);
    rtk_sdmmc_get_cmd_timeout(cmd_info);
    data_xfer_mode = CMD_RSP_ONLY;
	if(flag==1) ret = rtk_sdmmc_cpu_wait(DRIVER_NAME, rtk_host, SD_SENDCMDGETRSP);
	else ret = rtk_sdmmc_int_wait(DRIVER_NAME, rtk_host, SD_SENDCMDGETRSP);
    rtk_sdmmc_sync(rtk_host);
    return ret;
}

static int rtk_sdmmc_stream_cmd(u16 cmdcode, struct sdmmc_cmd_pkt *cmd_info, u8 data_len)
{
    u8 cmd_idx = cmd_info->cmd->opcode;
    u32 sd_arg = 0;
    s8 rsp_para1 = 0;
    s8 rsp_para2 = 0;
    s8 rsp_para3 = 0;
    int rsp_len = cmd_info->rsp_len;
    int ret = 0;
    u32 data = cmd_info->dma_buffer;
    u32 sa = 0;
    u32 *rsp = (u32 *)&cmd_info->cmd->resp;
    u16 byte_count = cmd_info->byte_count;
    u16 block_count = cmd_info->block_count;

    INT4_flag=true;
    struct rtk_sdmmc_host *rtk_host = cmd_info->rtk_host;
    void __iomem *sdmmc_base = rtk_host->sdmmc;

    if(!data || rsp == NULL){
        BUG_ON(1);
    }

    rtk_sdmmc_set_rspparam(cmd_info);
    sd_arg = cmd_info->cmd->arg;

    //if (rsp_para1 != -1)
    rsp_para1 = cmd_info->rsp_para1;

    /* Fix CRC error for SD_AUTOWRITE3, jamestai20150325 */
    if(cmdcode == SD_AUTOWRITE3){
        rsp_para2 = 0;
    }else{
        rsp_para2 = cmd_info->rsp_para2;
    }

    //if(rsp_para3 != -1)
    rsp_para3 = cmd_info->rsp_para3;

    sa = (data / 8);
    //sa = ((u32)data / 8);

    if((cmdcode == SD_NORMALWRITE)){
        byte_count = 512;
    }else if(cmdcode == SD_NORMALREAD && mmc_detected == 1){
        byte_count = 512;
    }

    g_cmd[0] = (0x40 | cmd_idx);
    g_cmd[1] = (sd_arg >> 24) & 0xff;
    g_cmd[2] = (sd_arg >> 16) & 0xff;
    g_cmd[3] = (sd_arg >> 8) & 0xff;
    g_cmd[4] = sd_arg & 0xff;
    g_cmd[5] = 0x00;

    writeb(g_cmd[0], sdmmc_base + SD_CMD0); //0x10
    writeb(g_cmd[1], sdmmc_base + SD_CMD1); //0x14
    writeb(g_cmd[2], sdmmc_base + SD_CMD2); //0x18
    writeb(g_cmd[3], sdmmc_base + SD_CMD3); //0x1C
    writeb(g_cmd[4], sdmmc_base + SD_CMD4); //0x20
    writeb(g_cmd[5], sdmmc_base + SD_CMD5); //0x20

    if(rsp_para1 != -1){
        writeb(readb(sdmmc_base + SD_CONFIGURE1) | rsp_para1, sdmmc_base + SD_CONFIGURE1); //0x0C
    }

    writeb(rsp_para2, sdmmc_base + SD_CONFIGURE2); //0x0C

    if(rsp_para3 != -1){
        writeb(readb(sdmmc_base + SD_CONFIGURE3) | rsp_para3, sdmmc_base + SD_CONFIGURE3); //0x0C
    }

    writeb(byte_count, sdmmc_base + SD_BYTE_CNT_L); //0x24
    writeb(byte_count >> 8, sdmmc_base + SD_BYTE_CNT_H); //0x28
    writeb(block_count, sdmmc_base + SD_BLOCK_CNT_L); //0x2C
    writeb(block_count >> 8, sdmmc_base + SD_BLOCK_CNT_H); //0x30

    if(cmd_info->cmd->data->flags & MMC_DATA_READ){
        writel((u32)sa, sdmmc_base + CR_SD_DMA_CTL1);
        writel(block_count, sdmmc_base + CR_SD_DMA_CTL2);

        if(data_len == RESP_LEN64){
            writel(RSP64_SEL | DDR_WR | DMA_XFER, sdmmc_base + CR_SD_DMA_CTL3);
        }else if(data_len == RESP_LEN17){
            writel(RSP17_SEL | DDR_WR | DMA_XFER, sdmmc_base + CR_SD_DMA_CTL3);
        }else
            writel(DDR_WR | DMA_XFER, sdmmc_base + CR_SD_DMA_CTL3);
	data_xfer_mode = CMD_RSP_DATA_READ;
        if(cmd_info->cmd->opcode == MMC_SEND_TUNING_BLOCK) {
		INT4_flag=false;
		data_xfer_mode = CMD_RSP_DATA_READ_TUNNING;
	}

    }else if(cmd_info->cmd->data->flags & MMC_DATA_WRITE){
        rtk_sdmmc_debug("%s: DMA sa = 0x%x\nDMA len = 0x%x\nDMA set = 0x%x\n", __func__, (u32)sa, block_count, DMA_XFER);
        writel((u32)sa, sdmmc_base + CR_SD_DMA_CTL1);
        writel(block_count, sdmmc_base + CR_SD_DMA_CTL2);

        if(data_len == RESP_LEN64){
            writel(RSP64_SEL | DMA_XFER, sdmmc_base + CR_SD_DMA_CTL3);
        }else if(data_len == RESP_LEN17){
            writel(RSP17_SEL | DMA_XFER, sdmmc_base + CR_SD_DMA_CTL3);
        }else
            writel(DMA_XFER, sdmmc_base + CR_SD_DMA_CTL3);

	data_xfer_mode = CMD_RSP_DATA_WRITE;
	if( cmdcode == SD_AUTOWRITE3 ) {
		data_xfer_mode = DATA_WRITE_ONLY;
        }
    }

    rtk_host->cmd_opcode = cmd_idx;
    rtk_sdmmc_get_cmd_timeout(cmd_info);
    //printk(KERN_ERR "rtk_stream_cmd invoke pre_cmd=%d, cmd=%d, INT4_flag=%d\n",pre_cmd,cmd_info->cmd->opcode,INT4_flag);
    ret = rtk_sdmmc_int_wait(DRIVER_NAME, rtk_host, cmdcode);
    /* Reset dat64_sel and rsp17_sel, #CMD19 DMA won't be auto-cleared */
    if(cmd_info->cmd->opcode == MMC_SEND_TUNING_BLOCK) {
        writel(0x00000000, sdmmc_base + CR_SD_DMA_CTL3);
	data_xfer_mode = CMD_RSP_ONLY;
    }

    if(ret == CR_TRANS_OK){
        if((cmdcode == SD_AUTOREAD1) || (cmdcode == SD_AUTOWRITE1)){
            rtk_sdmmc_debug("%s: auto read/write 1 skip response\n", __func__);
#ifdef CMD25_WO_STOP_COMMAND
        }else if(cmdcode == SD_AUTOWRITE2){
            rtk_sdmmc_debug("%s: auto write 2 skip response\n", __func__); //CMD + DATA
        }else if(cmdcode == SD_AUTOWRITE3){
            rtk_sdmmc_debug("%s: auto write 3 skip response\n", __func__); //DATA only, clear rsp
#endif
        }else{
            rtk_sdmmc_read_rsp(rtk_host,rsp, rsp_len);
            rtk_sdmmc_debug("%s: stream cmd done\n", __func__);
        }
    }

    return ret;
}

static int rtk_sdmmc_stream(struct sdmmc_cmd_pkt *cmd_info)
{
    u8 cmd_idx = cmd_info->cmd->opcode;
    int ret = 0;
    u32 i = 0;
    u32 dir = 0;
    u32 dma_nents = 0;
    u32 dma_leng = 0;
    u32 dma_addr = 0;
    u32 old_arg = 0;
    u16 cmdcode = 0;
    u8 data_len = 0;

    struct scatterlist *sg;
    struct mmc_host *host = cmd_info->rtk_host->mmc;
    struct rtk_sdmmc_host *rtk_host = cmd_info->rtk_host;

#ifdef CMD25_WO_STOP_COMMAND
    cmd12_stop_cmd.arg=cmd_info->cmd->arg;   //send stop command only use arg info and we need to save it as global variable for fear that timer trigger stop command data corruption
    rtk_host->rtk_sdmmc_cmd12 = &cmd12_stop_cmd;
	//rtk_host->rtk_sdmmc_cmd12 = cmd_info->cmd;   //bad using because of local variable , 
#endif

    if(cmd_info->data->flags & MMC_DATA_READ){
        dir = DMA_FROM_DEVICE;
    }else{
        dir = DMA_TO_DEVICE;
    }

    cmd_info->data->bytes_xfered = 0;
    dma_nents = dma_map_sg(mmc_dev(host), cmd_info->data->sg, cmd_info->data->sg_len, dir);
    sg = cmd_info->data->sg;

    old_arg = cmd_info->cmd->arg;

    for(i = 0 ; i < dma_nents ; i++, sg++){
        u32 blk_cnt = 0;

        dma_leng = sg_dma_len(sg);
        dma_addr = sg_dma_address(sg);

        rtk_sdmmc_debug("%s: dma_addr: 0x%x, dma_leng: 0x%x\n", __func__, dma_addr, dma_leng);

        if((cmd_idx == SD_SWITCH) && (cmd_info->cmd->flags | MMC_CMD_ADTC)){
            cmd_info->byte_count = 0x40;
            blk_cnt = dma_leng / 0x40;
            data_len = RESP_LEN64;
        }else if((cmd_idx == SD_APP_SD_STATUS) && ((cmd_info->cmd->flags & (0x3<<5)) == MMC_CMD_ADTC)){
            cmd_info->byte_count = 0x40;
            blk_cnt = dma_leng / 0x40;
            data_len = RESP_LEN64;
        }else if((cmd_idx == MMC_SEND_TUNING_BLOCK) | (cmd_idx == SD_APP_SEND_SCR) | (cmd_idx == SD_APP_SEND_NUM_WR_BLKS)){
            cmd_info->byte_count = 0x40; //rtk HW limite, one trigger 512 byte pass.
            blk_cnt = 1;
            data_len = RESP_LEN64;
        }else if((cmd_idx == MMC_ALL_SEND_CID) | (cmd_idx == MMC_SEND_CSD) | (cmd_idx == MMC_SEND_CID)){
            cmd_info->byte_count = 0x11;
            blk_cnt = dma_leng / 0x11;
            data_len = RESP_LEN17;
        }else{
            cmd_info->byte_count = BYTE_CNT; //rtk HW limite, one trigger 512 byte pass.
            blk_cnt = dma_leng / BYTE_CNT;
            data_len = 0;
        }

        if(blk_cnt == 0 && dma_leng){
            blk_cnt = 1;
        }

        cmd_info->block_count = blk_cnt;
        cmd_info->dma_buffer = dma_addr;
        cmdcode = rtk_host->ops->chk_cmdcode(cmd_info->cmd);

#ifdef CMD25_WO_STOP_COMMAND //new write method
        if(host->card && mmc_card_blockaddr(host->card) && (
            cmd_info->cmd->opcode == MMC_WRITE_BLOCK ||
            cmd_info->cmd->opcode == MMC_WRITE_MULTIPLE_BLOCK ||
            cmd_info->cmd->opcode == MMC_EXT_WRITE_MULTIPLE)
          ){
            if(sd_in_receive_data_state){
                if(sd_current_blk_address == cmd_info->cmd->arg){
                    cmdcode = SD_AUTOWRITE3; //DATA only
                }else{
                    rtk_sdmmc_send_stop_cmd(cmd_info->cmd, rtk_host,0); //send stop command first
		    pre_cmd=12;
                    sd_in_receive_data_state = 0; //not set SD in receive-data state
                    cmdcode = SD_AUTOWRITE2; //CMD + DATA
                }
            }else
                cmdcode = SD_AUTOWRITE2; //CMD + DATA

            //alwasy use multi-write command
            if(cmd_info->cmd->opcode == MMC_EXT_WRITE_MULTIPLE)
                cmd_info->cmd->opcode = MMC_EXT_WRITE_MULTIPLE;
            else
                cmd_info->cmd->opcode = MMC_WRITE_MULTIPLE_BLOCK;
        }else{
            if(cmd_info->cmd->opcode == MMC_SEND_STATUS){ //cmd13

            }else{
                if(sd_in_receive_data_state){
                    rtk_sdmmc_send_stop_cmd(cmd_info->cmd, rtk_host,0); //send stop command first
		    pre_cmd=12;
                    /* stop trigger, not set SD in receive-data state */
                    sd_in_receive_data_state = 0;
                }
            }
        }
#endif
        ret = rtk_sdmmc_stream_cmd(cmdcode, cmd_info, data_len);
        if(ret == 0){
#ifdef CMD25_WO_STOP_COMMAND //new write method
            if(host->card && mmc_card_blockaddr(host->card) && (
                cmd_info->cmd->opcode == MMC_WRITE_BLOCK ||
                cmd_info->cmd->opcode == MMC_WRITE_MULTIPLE_BLOCK ||
                cmd_info->cmd->opcode == MMC_EXT_WRITE_MULTIPLE)
              ){
                sd_current_blk_address = cmd_info->cmd->arg + blk_cnt;
                sd_in_receive_data_state = 1;
                mod_timer(&rtk_host->rtk_sdmmc_stop_cmd, jiffies + 20/*1*HZ*/);
            }
#endif
            cmd_info->cmd->arg += dma_leng;
            cmd_info->data->bytes_xfered += dma_leng;
        }

        if(ret){
            cmd_info->cmd->arg = old_arg;
            break;
        }

    }

    dma_unmap_sg(mmc_dev(host), cmd_info->data->sg, cmd_info->data->sg_len, dir);
    g_cmd[0] = 0x00;

    return ret;
}

static void rtk_sdmmc_swap_data(u8 *buffer, void __iomem *sdmmc_base)
{
    buffer[3] = buffer[2];
    buffer[2] = buffer[1];
    buffer[1] = buffer[0];
    buffer[0] = buffer[3+4];
    buffer[3+4] = buffer[3+3];
    buffer[3+3] = buffer[3+2];
    buffer[3+2] = buffer[3+1];
    buffer[3+1] = buffer[7+4];
    buffer[7+4] = buffer[7+3];
    buffer[7+3] = buffer[7+2];
    buffer[7+2] = buffer[7+1];
    buffer[7+1] = buffer[11+4];
    buffer[11+4] = buffer[11+3];
    buffer[11+3] = buffer[11+2];
    buffer[11+2] = buffer[11+1];
    buffer[11+1] = readb(sdmmc_base + SD_CMD5);
}

static int rtk_sdmmc_send_cmd_get_rsp(struct sdmmc_cmd_pkt *cmd_info)
{
    u8 cmd_idx = cmd_info->cmd->opcode;
    u32 sd_arg = 0;
    s8 rsp_para1 = 0;
    s8 rsp_para2 = 0;
    s8 rsp_para3 = 0;
    u8 rsp_len = cmd_info->rsp_len;
    u32 *rsp = (u32 *)&cmd_info->cmd->resp;
    u32 dma_val = 0;
    u32 byte_count = 0x200;
    u32 block_count = 1;
    u64 sa = 0;
    u8 RESP17_buffer[16];
    int ret = 0;

    struct rtk_sdmmc_host *rtk_host = cmd_info->rtk_host;
    void __iomem *sdmmc_base = rtk_host->sdmmc;
    
    INT4_flag=false;

    rtk_sdmmc_set_rspparam(cmd_info);

    sd_arg = cmd_info->cmd->arg;
    rsp_para1 = cmd_info->rsp_para1;
    rsp_para2 = cmd_info->rsp_para2;
    rsp_para3 = cmd_info->rsp_para3;

    if(rsp == NULL){
        BUG_ON(1);
    }

    if ((cmd_idx == SD_IO_SEND_OP_COND)|(cmd_idx == SD_IO_RW_DIRECT)|(cmd_idx == SD_IO_RW_EXTENDED)){
        printk(KERN_INFO "%s : reject SDIO commands cmd:0x%02x \n", __func__, cmd_idx);
        return CR_TRANSFER_FAIL;
    }

    if (mini_SD==1 && cmd_idx == 8){
        printk(KERN_INFO "%s : reject SDIO commands cmd:0x%02x \n", __func__, cmd_idx);
        return CR_TRANSFER_FAIL;
    }

    if (rsp_para1 != -1)
        writeb(rsp_para1, sdmmc_base + SD_CONFIGURE1);

    writeb(rsp_para2, sdmmc_base + SD_CONFIGURE2);

    if (rsp_para3 != -1)
        writeb(rsp_para3, sdmmc_base + SD_CONFIGURE3);

    g_cmd[0] = (0x40 | cmd_idx);
    g_cmd[1] = (sd_arg >> 24) & 0xff;
    g_cmd[2] = (sd_arg >> 16) & 0xff;
    g_cmd[3] = (sd_arg >> 8) & 0xff;
    g_cmd[4] = sd_arg & 0x000000FF;
    g_cmd[5] = 0x00000000;

    writeb(g_cmd[0], sdmmc_base + SD_CMD0);
    writeb(g_cmd[1], sdmmc_base + SD_CMD1);
    writeb(g_cmd[2], sdmmc_base + SD_CMD2);
    writeb(g_cmd[3], sdmmc_base + SD_CMD3);
    writeb(g_cmd[4], sdmmc_base + SD_CMD4);
    writeb(g_cmd[5], sdmmc_base + SD_CMD5);

    rtk_host->cmd_opcode = cmd_idx;

    rtk_sdmmc_get_cmd_timeout(cmd_info);

    data_xfer_mode = CMD_RSP_ONLY;
    if(RESP_TYPE_17B & rsp_para2){
        INT4_flag=true;
        /*remap the resp dst buffer to un-cache*/
        sa = (u64)((void *)dma_phy_addr);
        sa = sa / 8;
        dma_val = RSP17_SEL | DDR_WR | DMA_XFER;
        writeb(byte_count, sdmmc_base + SD_BYTE_CNT_L); //0x24
        writeb(byte_count>>8, sdmmc_base + SD_BYTE_CNT_H); //0x28
        writeb(block_count, sdmmc_base + SD_BLOCK_CNT_L); //0x2C
        writeb(block_count>>8, sdmmc_base + SD_BLOCK_CNT_H); //0x30

        writel(sa, sdmmc_base + CR_SD_DMA_CTL1); //espeical for R2
        writel(0x00000001, sdmmc_base + CR_SD_DMA_CTL2); //espeical for R2
        writel(dma_val, sdmmc_base + CR_SD_DMA_CTL3); //espeical for R2
	data_xfer_mode = CMD_RSP_DATA_READ;
    }else if(RESP_TYPE_6B & rsp_para2){
        //do nothing
    }
//printk(KERN_ERR "get_rsp:  pre_cmd=%d,cmd=%d, INT4_flag=%d\n",pre_cmd,cmd_idx,INT4_flag);
    ret = rtk_sdmmc_int_wait(DRIVER_NAME, rtk_host, SD_SENDCMDGETRSP);
    if(ret == CR_TRANS_OK){
        if(dma_virt_addr != 0){

            rtk_sdmmc_read_rsp(rtk_host, (void *)dma_virt_addr, rsp_len);

            if(rsp_len == 16){
                memcpy(RESP17_buffer, (void *)dma_virt_addr, 16);
                rtk_sdmmc_swap_data(RESP17_buffer, sdmmc_base);
                memcpy(rsp, (void *)RESP17_buffer, 16);
            }else{
                memcpy(rsp, (void *)dma_virt_addr, 16);
            }

        }else
            rtk_sdmmc_read_rsp(rtk_host, rsp, rsp_len);

        rtk_sdmmc_sync(rtk_host);

        if (cmd_idx == MMC_SET_RELATIVE_ADDR){
            g_crinit = 1;
            printk(KERN_INFO "SD/MMC card init done.\n");
        }
    }

    memset(dma_virt_addr, 0x00, SD_ALLOC_LENGTH);

    return ret;
}

static void rtk_sdmmc_send_command(struct rtk_sdmmc_host *rtk_host, struct mmc_command *cmd)
{
    int ret = 0;

    struct sdmmc_cmd_pkt cmd_info;
    void __iomem *sdmmc_base = rtk_host->sdmmc;

    memset(&cmd_info, 0, sizeof(struct sdmmc_cmd_pkt));

    if(!rtk_host || !cmd){
        printk(KERN_ERR "%s: rtk_host or cmd is null\n", __func__);
        return;
    }

    if(cmd->opcode == MMC_SEND_OP_COND){
        mmc_detected = 1;
    }

    /*work around of bug switch voltage fail : force clock disable after cmd11*/
    if(cmd->opcode == SD_SWITCH_VOLTAGE){
        writeb(0x40, sdmmc_base + SD_BUS_STATUS);
    }

    cmd_info.cmd = cmd;
    cmd_info.rtk_host = rtk_host;
    cmd_info.rsp_para2 = rtk_sdmmc_get_rsp_type(cmd_info.cmd);
    cmd_info.rsp_len = rtk_sdmmc_get_rsp_len(cmd_info.rsp_para2);

    if(cmd->data){
        cmd_info.data = cmd->data;

        if(cmd->data->flags == MMC_DATA_READ){
            /* do nothing */
        }else if(cmd->data->flags == MMC_DATA_WRITE){
            if(rtk_host->wp ==1){
                printk(KERN_ERR "%s: card is locked!", __func__);
                ret = -1;
                cmd->retries = 0;
                goto err_out;
            }
        }else{
            printk(KERN_ERR "%s: error: cmd->data->flags= %d\n", __func__, cmd->data->flags);
            cmd->error = -MMC_ERR_INVALID;
            cmd->retries = 0;
            goto err_out;
        }

        if(cmd->opcode != SD_APP_SEND_SCR){
            ret = rtk_sdmmc_stream(&cmd_info);
        }else{
            struct scatterlist sg;
            struct mmc_data data_SCR = {0};
            void *ssr;

            ssr = kmalloc(64, GFP_KERNEL | GFP_DMA);
            if(!ssr){
                ret = CR_TRANSFER_FAIL;
                goto err_out;
            }

            sg_init_one(&sg, ssr, 64);

            rtk_sdmmc_debug("%s: data->blksz= %d, data->blocks= %d \n", __func__, cmd_info.data->blksz, cmd_info.data->blocks);

            data_SCR.blksz = cmd_info.data->blksz;
            data_SCR.blocks = cmd_info.data->blocks;
            data_SCR.sg = cmd_info.data->sg;
            data_SCR.sg_len = cmd_info.data->sg_len;

            cmd_info.data->blksz = 64;
            cmd_info.data->blocks = 1;
            cmd_info.data->sg = &sg;
            cmd_info.data->sg_len = 1;

            ret = rtk_sdmmc_stream(&cmd_info);

            if(!ret){
                sg_copy_from_buffer(data_SCR.sg, data_SCR.sg_len, ssr, data_SCR.blksz);
                cmd_info.data->bytes_xfered = data_SCR.blksz;
                rtk_sdmmc_debug("%s: SCR =\n", __func__);
            }
            kfree(ssr);
        }
    }else{
        ret = rtk_sdmmc_send_cmd_get_rsp(&cmd_info);
    }

    if(cmd->opcode == MMC_SELECT_CARD){
        rtk_host->rtflags |= RTKCR_FCARD_SELECTED;
        rtk_sdmmc_speed(rtk_host, SDMMC_CLOCK_6200KHZ);
    }else if((cmd->opcode == SD_SEND_RELATIVE_ADDR)&&((cmd->flags & (0x3 << 5)) == MMC_CMD_BCR)){
        sdmmc_rca = ((cmd->resp[0]) >> RCA_SHIFTER);
    }

err_out:
    if (ret){
        if(ret == -RTK_RMOV)
            cmd->retries = 0;

        /* If SD card card no response do SD host reset, jamestai20151008 */
        if(cmd->opcode == 49 || cmd->opcode == 59 || cmd->opcode == 48|| cmd->opcode == 58){
            rtk_sdmmc_reset(rtk_host);
            cmd->error  = -110;
        }else{
            cmd->error = -MMC_ERR_FAILED;
        }
    }
    tasklet_schedule(&rtk_host->req_end_tasklet);
}
static void rtk_sdmmc_request(struct mmc_host *host, struct mmc_request *mrq)
{
    struct rtk_sdmmc_host *rtk_host = mmc_priv(host);
    struct mmc_command *cmd;
    unsigned char card_cmd = 0;

    BUG_ON(rtk_host->mrq != NULL);
    //down_write(&cr_sd_rw_sem);
    down(&cr_sd_sem);
    cmd = mrq->cmd;
    rtk_host->mrq = mrq;
    data_xfer_mode = CMD_RSP_ONLY; // init each request
	//printk(KERN_ERR "JIM debug rtk_sdmmc_request cmd->opcode = %d\n",cmd->opcode);
    /* Check SD card extension command support,jamestai20151008*/
    if(cmd->opcode == 58 || cmd->opcode == 59){
        card_cmd = host->card->raw_scr[0] & 0x0F;
        if((card_cmd & 0x08) != 0x08){
            cmd->error = -110;
            goto done;
        }
    }

    if(cmd->opcode == 48 || cmd->opcode == 49){
        card_cmd = host->card->raw_scr[0] & 0xF;
        if((card_cmd & 0x8) != 0x8 && (card_cmd & 0x4) != 0x4){
            cmd->error = -110;
            goto done;
        }
    }

    if(!(rtk_host->rtflags & RTKCR_FCARD_DETECTED)){
        cmd->error = -MMC_ERR_RMOVE;
        cmd->retries = 0;
        goto done;
    }

    if(rtk_host && cmd){
        rtk_sdmmc_allocate_dma_buf(rtk_host, cmd);
        rtk_sdmmc_send_command(rtk_host, cmd);
    }else{
done:
        tasklet_schedule(&rtk_host->req_end_tasklet);
    }
    pre_cmd=cmd->opcode;
	//up_write(&cr_sd_rw_sem);
    //down_trylock(&cr_sd_sem);
    if(cr_sd_sem.count==0) up(&cr_sd_sem);    //unplug will release semaphore, in this case we do not need to release the semaphore
}

static int rtk_sdmmc_get_ro(struct mmc_host *host)
{
    struct rtk_sdmmc_host *rtk_host = mmc_priv(host);
    void __iomem *sdmmc_base = rtk_host->sdmmc;
    u32 reginfo = 0;

    g_ro = reginfo = readb(sdmmc_base + CARD_EXIST);

    if(reginfo & SD_WRITE_PROTECT){
        printk(KERN_INFO "%s: SD card is write protect, regCARD_EXIST = %x\n", __func__, readb(sdmmc_base + CARD_EXIST));
        return 1;
    }
    printk(KERN_INFO "%s: SD card is not write protect, regCARD_EXIST = %x\n", __func__, readb(sdmmc_base + CARD_EXIST));
    return 0;
}

static int rtk_sdmmc_get_cd(struct mmc_host *host)
{
    struct rtk_sdmmc_host *rtk_host = mmc_priv(host);
    void __iomem *sdmmc_base = rtk_host->sdmmc;
    u32 reginfo = 0;

    reginfo = readb(sdmmc_base + CARD_EXIST);

    if(reginfo & SD_EXISTENCE){
        printk(KERN_ERR "%s: SD card exists, regCARD_EXIST = %x\n", __func__, readb(sdmmc_base + CARD_EXIST));
        return 1;
    }
    printk(KERN_ERR "%s: SD card does not exist, regCARD_EXIST = %x\n", __func__, readb(sdmmc_base + CARD_EXIST));
    return 0;
}

static void rtk_sdmmc_set_ios(struct mmc_host *host, struct mmc_ios *ios)
{
    struct rtk_sdmmc_host *rtk_host = mmc_priv(host);

    if(ios->bus_mode == MMC_BUSMODE_PUSHPULL){
        rtk_sdmmc_debug("%s: ios busmode = pushpull\n", __func__);
        if(ios->bus_width == MMC_BUS_WIDTH_8){
            rtk_sdmmc_debug("%s: set bus width 8\n", __func__);
            rtk_sdmmc_set_bits(rtk_host, BUS_WIDTH_8);
        }else if (ios->bus_width == MMC_BUS_WIDTH_4){
            rtk_sdmmc_debug("%s: set bus width 4\n", __func__);
            rtk_sdmmc_set_bits(rtk_host, BUS_WIDTH_4);
        }else{
            rtk_sdmmc_set_bits(rtk_host, BUS_WIDTH_1);
            rtk_sdmmc_debug("%s: set bus width 1\n", __func__);
        }

        if(ios->clock >= UHS_SDR104_MAX_DTR && ios->timing == MMC_TIMING_UHS_SDR104){
            rtk_sdmmc_set_access_mode(rtk_host, ACCESS_MODE_SD30);
            rtk_sdmmc_speed(rtk_host, SDMMC_CLOCK_208000KHZ);
            rtk_sdmmc_debug("%s: UHS SDR104 SDMMC_CLOCK_208000KHZ\n", __func__);
        }else if(ios->clock >= UHS_SDR50_MAX_DTR && ios->timing == MMC_TIMING_UHS_SDR50){
            rtk_sdmmc_set_access_mode(rtk_host, ACCESS_MODE_SD30);
            rtk_sdmmc_speed(rtk_host, SDMMC_CLOCK_100000KHZ);
            rtk_sdmmc_debug("%s: UHS SDR50 SDMMC_CLOCK_100000KHZ\n", __func__);
        }else if(ios->clock >= UHS_SDR25_MAX_DTR && ios->timing == MMC_TIMING_UHS_SDR25){
            rtk_sdmmc_set_access_mode(rtk_host, ACCESS_MODE_SD30);
            rtk_sdmmc_speed(rtk_host, SDMMC_CLOCK_50000KHZ);
            rtk_sdmmc_debug("%s: UHS SDR25 SDMMC_CLOCK_50000KHZ\n", __func__);
        }else if(ios->clock >= UHS_SDR12_MAX_DTR && ios->timing == MMC_TIMING_UHS_SDR12){
            rtk_sdmmc_set_access_mode(rtk_host, ACCESS_MODE_SD30);
            rtk_sdmmc_speed(rtk_host, SDMMC_CLOCK_25000KHZ);
            rtk_sdmmc_debug("%s: UHS SDR12 SDMMC_CLOCK_25000KHZ\n", __func__);
        }else if(ios->clock >= UHS_DDR50_MAX_DTR && ios->timing == MMC_TIMING_UHS_DDR50){
            rtk_sdmmc_set_access_mode(rtk_host, ACCESS_MODE_DDR);
            rtk_sdmmc_speed(rtk_host, SDMMC_CLOCK_50000KHZ);
            rtk_sdmmc_debug("%s: UHS DDR50 SDMMC_CLOCK_50000KHZ\n", __func__);
        }else if(ios->clock >= HIGH_SPEED_MAX_DTR && ios->timing == MMC_TIMING_SD_HS){
            rtk_sdmmc_set_access_mode(rtk_host, ACCESS_MODE_SD20);
            rtk_sdmmc_speed(rtk_host, SDMMC_CLOCK_50000KHZ);
            rtk_sdmmc_debug("%s: High Speed SDMMC_CLOCK_50000KHZ\n", __func__);
        }else if(ios->clock >= 25000000){
            rtk_sdmmc_set_access_mode(rtk_host, ACCESS_MODE_SD20);
            rtk_sdmmc_speed(rtk_host, SDMMC_CLOCK_25000KHZ);
        }else{
            if (rtk_host->rtflags & RTKCR_FCARD_SELECTED){
                rtk_sdmmc_speed(rtk_host, SDMMC_CLOCK_6200KHZ);
                rtk_sdmmc_debug("%s: Mid speed RTKCR_FCARD_SELECTED = 1 SDMMC_CLOCK_6200KHZ\n", __func__);
            }else{
                rtk_sdmmc_speed(rtk_host, SDMMC_CLOCK_400KHZ);
                rtk_sdmmc_debug("%s: Low speed SDMMC_CLOCK_400KHZ\n", __func__);
            }
        }
    }else{  //MMC_BUSMODE_OPENDRAIN
        rtk_sdmmc_debug("%s: ios busmode != pushpull low speed SDMMC_CLOCK_400KHZ\n", __func__);
        rtk_sdmmc_speed(rtk_host, SDMMC_CLOCK_400KHZ);
        rtk_sdmmc_set_bits(rtk_host,BUS_WIDTH_1);
    }

    if (ios->power_mode == MMC_POWER_UP){
        rtk_host->ops->card_power(rtk_host, 1); //power on
        rtk_sdmmc_debug("%s: Power on\n", __func__);
    }else if(ios->power_mode == MMC_POWER_OFF){
        rtk_host->ops->card_power(rtk_host, 0); //power off
        rtk_sdmmc_debug("%s: Power off\n", __func__);
    }
}

static void rtk_sdmmc_hw_initial(struct rtk_sdmmc_host *rtk_host)
{
    void __iomem *sdmmc_base = rtk_host->sdmmc;
    void __iomem *pll_base = rtk_host->pll;
    void __iomem *emmc_base = rtk_host->emmc;
    void __iomem *sysbrdg_base = rtk_host->sysbrdg;
    int bErrorRetry_1 = 0;
    unsigned long flags2;

    /*SD PLL Initialization, jamestai20150721*/
    writel(readl(pll_base + CR_PLL_SD4) | 0x00000004, pll_base + CR_PLL_SD4);
    writel(readl(pll_base + CR_PLL_SD4) | 0x00000007, pll_base + CR_PLL_SD4);

	writel(0x0000003, pll_base + CR_PLL_SD1);
        mdelay(10); //delay 500 ms to fit SD spec
    writel(0x00002003, pll_base + CR_PLL_SD1); //PLL_SD1

    if(readl(sysbrdg_base+0x204)!=0x0) 
	writel(0x40000000,sdmmc_base+0x2c);
    udelay(100);
    writel(0x00000006, pll_base + 0x01EC);   //JIM modified, 1AC->1EC, we modified this part because 1AC is to change SDIO PLL instead of SD card PLL
    writel(readl(sdmmc_base + CR_SD_CKGEN_CTL) | 0x00070000, sdmmc_base + CR_SD_CKGEN_CTL); //Switch SD source clock to 4MHz by Hsin-yin
    mdelay(2);
    writel(0x00000007, pll_base + 0x01EC);    //JIM modified, 1AC->1EC
    if(readl(sysbrdg_base+0x204)!=0x0)
        writel(0x00000000,sdmmc_base+0x2c);

    if(readl(sysbrdg_base+0x204)!=0x0)
        writel(0x40000000,sdmmc_base+0x2c);
    udelay(100);
    writel(0x00000006, pll_base + 0x01EC);     //JIM modified, 1AC->1EC
    writel(0x04515893, pll_base + CR_PLL_SD2); //change from 4517893 to 4515893 for passing the EMI 
    writel(0x00564388, pll_base + CR_PLL_SD3); //Set PLL clock rate, default clock 100MHz
	//ritel(0x00324388,  pll_base + CR_PLL_SD3);
    mdelay(2);
    writel(0x00000007, pll_base + 0x01EC);      //JIM modified, 1AC->1EC
    if(readl(sysbrdg_base+0x204)!=0x0)
        writel(0x00000000,sdmmc_base+0x2c);
    /* Enable SD host clock*/
    //writel(readl(pll_base + 0x04) | (0x1 << 10), pll_base + 0x04);        //jim comment for fear that it will reset the SD CRT

	//=======
    //writel(readl(pll_base + 0x0C) | (0x1 << 25) | (0x1 << 31), pll_base + 0x0C);
   	struct clk * clk_cr = clk_get(NULL, "clk_en_cr");
        struct clk * clk_sd_ip = clk_get(NULL, "clk_en_sd_ip");
	clk_prepare_enable(clk_cr);
	clk_prepare_enable(clk_sd_ip);
	//======

    //writel(readl(sdmmc_base + 0x20) | 0x00000002, sdmmc_base + 0x20);    //add by jim for test, change the state to reset the SD IP
    //writel(readl(sdmmc_base + 0x20) | 0x00000003, sdmmc_base + 0x20);

    writel(readl(sdmmc_base + 0x20) |0x02, sdmmc_base + 0x20);    //Disable L4 gate

    writel(readl(sdmmc_base + 0x20) & (~0x01), sdmmc_base + 0x20);
    writel(readl(sdmmc_base + 0x20) | 0x00000001, sdmmc_base + 0x20);	//reset the DMA
    
    if(readl(sysbrdg_base+0x204)!=0x0)
        writel(0x40000000,sdmmc_base+0x2c);
    udelay(100);
    writel(0x00000006, pll_base + 0x01EC);       //JIM modified, 1AC->1EC
    writel(readl(sdmmc_base + CR_SD_CKGEN_CTL) & 0xFFF8FFFF, sdmmc_base + CR_SD_CKGEN_CTL); //Switch SD source clock to normal clock source by Hsin-yin
    udelay(100);
    writel(0x00000007, pll_base + 0x01EC);        //JIM modified, 1AC->1EC
    if(readl(sysbrdg_base+0x204)!=0x0)
        writel(0x00000000,sdmmc_base+0x2c);

    writeb(readb(sdmmc_base + SD_CONFIGURE1) & 0x000000EF, sdmmc_base + SD_CONFIGURE1); //Reset FIFO pointer by Hsin-yin
    writel(0x00000007, pll_base + CR_PLL_SD4); //PLL_SD4
    udelay(100);
    writeb(0xD0, sdmmc_base + SD_CONFIGURE1);
    
    rtk_sdmmc_speed(rtk_host, SDMMC_CLOCK_400KHZ);

    rtk_lockapi_lock(flags2, __FUNCTION__);
    writel(0x00003333, emmc_base + 0x634);
    writel(0x33333333, emmc_base + 0x638);
    rtk_lockapi_unlock(flags2, __FUNCTION__);
    
    
    writel(0x00000000, sdmmc_base + CR_SD_PAD_CTL); //change to 3.3v

    writel(0x00000016, sdmmc_base + CR_SD_ISR); //enable interrupt
    //writel(0x00000017, sdmmc_base + CR_SD_ISREN);
    writel(0x00000016, sdmmc_base + CR_SD_ISREN); // disable all
    writel(0x00000007, sdmmc_base + CR_SD_ISREN);
    rtk_sdmmc_sync(rtk_host);

    writeb(0x02, sdmmc_base + CARD_SELECT); //for emmc, select SD ip
    if(bErrorRetry_1){
        writeb(0x08, sdmmc_base + SD_SAMPLE_POINT_CTL); //sample point = SDCLK / 4
        writeb(0x10, sdmmc_base + SD_PUSH_POINT_CTL); //output ahead SDCLK /4
    }else{
        writeb(0x00, sdmmc_base + SD_SAMPLE_POINT_CTL); //sample point = SDCLK / 4
        writeb(0x00, sdmmc_base + SD_PUSH_POINT_CTL); //output ahead SDCLK /4
    }
}

static void rtk_sdmmc_hw_reset(struct mmc_host *host)
{
    struct rtk_sdmmc_host *rtk_host = mmc_priv(host);
    void __iomem *sdmmc_base = rtk_host->sdmmc;

    rtk_sdmmc_debug("%s: CARD_EXIST = %x\n", __func__, readb(sdmmc_base + CARD_EXIST));

    sd_in_receive_data_state = 0; //CMD25_WO_STOP_COMMAND
    sd_current_blk_address = 0; //CMD25_WO_STOP_COMMAND
    rtk_host->rtk_sdmmc_cmd12 = NULL; //CMD25_WO_STOP_COMMAND
    rtk_host->rtflags &= ~RTKCR_FCARD_SELECTED;
    sdmmc_rca = 0;

    rtk_sdmmc_hw_initial(rtk_host);

    writeb(0xFF, sdmmc_base + CR_CARD_STOP); //SD Card module transfer stop and idle state
    writeb(0x00, sdmmc_base + CR_CARD_STOP); //SD Card module transfer no stop
    writeb(0x02, sdmmc_base + CARD_SELECT); //Specify the current active card module for the coming data transfer, bit 2:0 = 010
    writeb(0x04, sdmmc_base + CR_CARD_OE); //arget module is SD/MMC card module, bit 2 =1
    writeb(0x04, sdmmc_base + CARD_CLOCK_EN_CTL); //SD Card Module Clock Enable, bit 2 = 1
    writeb(0xD0, sdmmc_base + SD_CONFIGURE1);

    writeb(0x00, sdmmc_base + SD_STATUS2); //SD CMD Response Timeout Error disable    //#x /b 0x18010584

    rtk_host->ops->card_power(rtk_host, 0); //power off
    rtk_host->ops->card_power(rtk_host, 1); //power on

    mmc_detected = 0;

    rtk_sdmmc_sync(rtk_host);
}

static int rtk_sdmmc_switch_voltage(struct mmc_host *mmc, struct mmc_ios *ios)
{
    struct rtk_sdmmc_host *rtk_host = mmc_priv(mmc);
    void __iomem *sdmmc_base = rtk_host->sdmmc;
    void __iomem *pll_base = rtk_host->pll;
    int ret = 0;

    if(ios->signal_voltage != MMC_SIGNAL_VOLTAGE_330){

        ret = rtk_sdmmc_wait_voltage_stable_low(rtk_host); //check DAT[3:0] are at LOW level

        if(ret < 0)
            goto out;

        writeb(0x3B, sdmmc_base + CARD_CLOCK_EN_CTL); //host stop clk
        mdelay(10); //delay 5 ms to fit SD spec

        /* Workaround : keep IO pad voltage at 3.3v for SD card compatibility, jamestai20141225 */
        writel(0x00000000, sdmmc_base + CR_SD_PAD_CTL);

        rtk_sdmmc_sync(rtk_host);

	writel(0x0000003, pll_base + CR_PLL_SD1);
        msleep(500); //delay 500 ms to fit SD spec

        writel(0x0004003, pll_base + CR_PLL_SD1);
        mdelay(10); //delay 5 ms to fit SD spec
        writeb(0x3F, sdmmc_base + CARD_CLOCK_EN_CTL); //force output clk
        writeb(0x80, sdmmc_base + SD_BUS_STATUS); //force a short period 1.8v clk
        rtk_sdmmc_sync(rtk_host);

        ret = rtk_sdmmc_wait_voltage_stable_high(rtk_host); //check DAT[3:0] are at HIGH level
        if (ret < 0)
            goto out;
    }

    writeb(0x00, sdmmc_base + SD_BUS_STATUS); //stop 1.8v clk

out:
    return ret;
}

static int rtk_sdmmc_execute_tuning(struct mmc_host *mmc, u32 opcode)
{
    struct rtk_sdmmc_host *rtk_host = mmc_priv(mmc);
    void __iomem *sdmmc_base = rtk_host->sdmmc;
    void __iomem *pll_base = rtk_host->pll;
    void __iomem *sysbrdg_base = rtk_host->sysbrdg;
    int ret = 0;
    unsigned int reg_tmp = 0;
    unsigned int reg_tmp2 = 0;
    unsigned int reg_tuned3318 = 0;

    reg_tmp2 = readl(pll_base + CR_PLL_SD2); //disable spectrum
    writel((reg_tmp2 & 0xFFFF1FFF), pll_base + CR_PLL_SD2); //PLL_SD2 clear [15:13]

    /*if tune tx phase fail, down 8MHz and retry*/
    do{
        ret = rtk_sdmmc_tuning_tx(rtk_host);
        if(ret == -MMC_ERR_RMOVE){
            printk(KERN_ERR "%s: Tuning TX fail.\n", __func__);
            return ret;
        }else if(ret){
            reg_tmp = readl(pll_base + CR_PLL_SD3);
            reg_tuned3318 = (reg_tmp & 0x03FF0000) >> 16;
            if (reg_tuned3318 <= 100){
                printk(KERN_ERR "%s: Tuning TX fail\n", __func__);
                return ret;
            }
            writel(readl(sdmmc_base + CR_SD_CKGEN_CTL) | 0x00070000, sdmmc_base + CR_SD_CKGEN_CTL); //Switch SD source clock to 4MHz by Hsin-yin
            reg_tmp = ((reg_tmp & (~0x3FF0000)) | ((reg_tuned3318 - 8) << 16)); //down 8MHz

	    if(readl(sysbrdg_base+0x204)!=0x0)
        	writel(0x40000000,sdmmc_base+0x2c);
            writel(0x00000006, pll_base + 0x01EC);       //JIM modified, 1AC->1EC
            writel(reg_tmp, pll_base + CR_PLL_SD3);
            mdelay(2);
            writel(0x00000007, pll_base + 0x01EC);        //JIM modified, 1AC->1EC
	    if(readl(sysbrdg_base+0x204)!=0x0)
        	writel(0x00000000,sdmmc_base+0x2c);
            writel(readl(sdmmc_base + CR_SD_CKGEN_CTL) & 0xFFF8FFFF, sdmmc_base + CR_SD_CKGEN_CTL); //Switch SD source clock to normal clock source by Hsin-yin
            writeb(readb(sdmmc_base + SD_CONFIGURE1) & 0xEF, sdmmc_base + SD_CONFIGURE1); //Reset FIFO pointer by Hsin-yin
        }
    }while(ret);

    if(ret){
        printk(KERN_ERR "%s: Tuning TX  fail\n", __func__);
        return ret;
    }

    ret = rtk_sdmmc_tuning_rx(rtk_host);
    writel(reg_tmp2, pll_base + CR_PLL_SD2); //enable spectrum

    if(ret){
        printk(KERN_ERR "%s: Tuning RX fail\n", __func__);
        return ret;
    }

    printk(KERN_INFO "%s CR_PLL_SD3: %d (0x%02x)\n", __func__, (readl(pll_base + CR_PLL_SD3) >> 16), (readl(pll_base + CR_PLL_SD3) >> 16));
    printk(KERN_INFO "%s CLK_GEN DVI: %d\n", __func__, 1 << (readl(sdmmc_base + CR_SD_CKGEN_CTL) & 0x03));

    return ret;
}

static const struct mmc_host_ops rtk_sdmmc_ops ={
    .request = rtk_sdmmc_request,
    .get_ro = rtk_sdmmc_get_ro,
    .get_cd = rtk_sdmmc_get_cd,
    .set_ios = rtk_sdmmc_set_ios,
    .hw_reset = rtk_sdmmc_hw_reset,
    .start_signal_voltage_switch = rtk_sdmmc_switch_voltage,
    .execute_tuning = rtk_sdmmc_execute_tuning,
};

#ifdef CMD25_WO_STOP_COMMAND
static void rtk_sdmmc_cmd12_fun(unsigned long data)
{
    struct rtk_sdmmc_host *rtk_host = (struct rtk_sdmmc_host *)data;
    if(sd_in_receive_data_state && rtk_host->rtk_sdmmc_cmd12 && rtk_host){
	if( down_trylock(&cr_sd_sem) != 0 ) {  // @rtk_sdmmc_cmd12_fun, lock fail
	//	printk(KERN_ERR "try to lock fail so that we delay the cmd12 timer 1\n");
                mod_timer(&rtk_host->rtk_sdmmc_stop_cmd, jiffies + 20);
	}
	else {
        	sd_in_receive_data_state = 0;
	
	//	printk(KERN_ERR "Timer cmd12 using polling mode!!!\n");
		do_stop_command_wo_complete = 1;
        	wait_stop_command_irq_done = 1;
        	rtk_sdmmc_send_stop_cmd(rtk_host->rtk_sdmmc_cmd12, rtk_host,1);
		rtk_sdmmc_wait_stop_cmd_irq_done();
		
		pre_cmd=12;
		//down_trylock(&cr_sd_sem);
		if(cr_sd_sem.count==0) up(&cr_sd_sem); // unplug will release semaphore, in this case we do not need to release the semaphore
	}
    }
}
#endif

static void rtk_sdmmc_plug(unsigned long data)
{
    u32 reginfo = 0;
    u32 det_time = 0;
    unsigned int reg_tmp = 0;
    unsigned long timeout = 0;

    struct rtk_sdmmc_host *rtk_host = (struct rtk_sdmmc_host *)data;
    void __iomem *sdmmc_base = rtk_host->sdmmc;
    void __iomem *pll_base = rtk_host->pll;

    unsigned long flags2;
#ifdef CONFIG_MMC_RTK_EMMC
    if(get_RTK_initial_flag()==0) {
        return;       //we must wait for EMMC driver initial done
    }
#endif

    reginfo = readb(sdmmc_base + CARD_EXIST);
    if((reginfo & SD_EXISTENCE) ^ (rtk_host->int_status_old & SD_EXISTENCE)){
        rtk_host->rtflags &= ~RTKCR_FCARD_DETECTED;
        rtk_host->wp = 0;
        if(reginfo & SD_EXISTENCE){
            rtk_host->ops->card_power(rtk_host, 1); //power on, Jim add

            rtk_host->ins_event = EVENT_INSER;
            rtk_host->rtflags |= RTKCR_FCARD_DETECTED;
            det_time = 1;

	    CMD8_SD1=false;
            CMD55_MMC=false;

            writel(0x00000000, sdmmc_base + CR_SD_DMA_CTL3); //stop dma control
	    data_xfer_mode = CMD_RSP_ONLY;
            writeb(0x00, sdmmc_base + CR_CARD_STOP); //SD Card module transfer no stop
            writeb(0x02, sdmmc_base + CARD_SELECT); //Specify the current active card module for the coming data transfer, bit 2:0 = 010
            writeb(0x04, sdmmc_base + CR_CARD_OE); //arget module is SD/MMC card module, bit 2 =1
            writeb(0x04, sdmmc_base + CARD_CLOCK_EN_CTL); //SD Card Module Clock Enable, bit 2 = 1
            writeb(0xD0, sdmmc_base + SD_CONFIGURE1);
            rtk_sdmmc_speed(rtk_host, SDMMC_CLOCK_400KHZ);
            writeb(0x00, sdmmc_base + SD_STATUS2);
	    printk(KERN_ERR "SD card is being inserted now...!!!\n");
        }else{
            rtk_host->ins_event = EVENT_REMOV;
            rtk_host->rtflags &= ~RTKCR_FCARD_DETECTED;
            det_time = 1;
	    driving_capacity=0;

            down_trylock(&cr_sd_sem);
	    up(&cr_sd_sem);

	    rtk_lockapi_lock(flags2, __FUNCTION__);
            writel(0x00003333, rtk_host->emmc + 0x634);
            writel(0x33333333, rtk_host->emmc + 0x638);
            rtk_lockapi_unlock(flags2, __FUNCTION__);
	    mini_SD=0;
	    broken_flag=false;
#ifdef CONFIG_MACH_RTK1195
            /*reset*/
            timeout = jiffies + msecs_to_jiffies(100);
            while(time_before(jiffies, timeout)){
		rtk_lockapi_lock(flags2, __FUNCTION__);
               if((!(readl(rtk_host->emmc + EMMC_DMA_CTL3) & 0x01)) &&
                    (readb(rtk_host->sdio + SDIO_NORML_INT_STA) & 0x02)){
                    	rtk_lockapi_unlock(flags2, __FUNCTION__);
			break;
		}
		rtk_lockapi_unlock(flags2, __FUNCTION__);
            }
#endif
	    //writel(readl(pll_base + 0x0C) | (0x1 << 25) | (0x1 << 31), pll_base + 0x0C);
	    //writel(readl(sdmmc_base + 0x20) | 0x00000002, sdmmc_base + 0x20);    //add by jim for test, change the state to reset the SD IP
	    //writel(readl(sdmmc_base + 0x20) | 0x00000003, sdmmc_base + 0x20); 

            writel(0x0, sdmmc_base + CR_SD_DMA_CTL3); //stop dma control
	    data_xfer_mode = CMD_RSP_ONLY;
            writeb(0xff, sdmmc_base + CR_CARD_STOP); //SD Card module transfer stop and idle state

            writeb(0x3b, sdmmc_base + CARD_CLOCK_EN_CTL); 

            rtk_host->ops->card_power(rtk_host, 0); //power off, Jim add
	    printk(KERN_ERR "SD card is being removed now...!!!\n");
        }
        rtk_sdmmc_sync(rtk_host);
	rtk_lockapi_lock(flags2, __FUNCTION__);
        printk(KERN_INFO "[SD] SD card power register=%x\n",readl(rtk_host->emmc + CR_PFUNC_CR));
	rtk_lockapi_unlock(flags2, __FUNCTION__);

        rtk_host->rtflags &= ~RTKCR_FCARD_SELECTED;
        sdmmc_rca = 0;
        mmc_detect_change(rtk_host->mmc, msecs_to_jiffies(det_time));
    }
	//workaround for some manufacture SD hardware issue that will have delay of write protection detection
    if(rtk_host->mmc->card  && !(g_ro & SD_WRITE_PROTECT) && (reginfo & SD_WRITE_PROTECT) && (reginfo & SD_EXISTENCE)) {
	printk(KERN_INFO "The SD card is locked!!!\n");
	g_ro = g_ro | SD_WRITE_PROTECT;
	mmc_card_set_readonly(rtk_host->mmc->card);
	mmc_blk_set_ro(rtk_host->mmc->card);
    }
    rtk_host->int_status_old = reginfo;
    mod_timer(&rtk_host->plug_timer, jiffies + HZ/10);
}

static void rtk_sdmmc_req_end_tasklet(unsigned long param)
{
    struct rtk_sdmmc_host *rtk_host = (struct rtk_sdmmc_host *)param;
    struct mmc_request* mrq;

    mrq = rtk_host->mrq;
    rtk_host->mrq = NULL;
    
    mmc_request_done(rtk_host->mmc, mrq);
}

static void rtk_sdmmc_timeout(unsigned long data)
{
    if(poll==1)  return;    //this timeout function is for interrupt mode

    struct rtk_sdmmc_host *rtk_host = (struct rtk_sdmmc_host *)data;

    if(rtk_host->ins_event ==EVENT_REMOV) {
        printk(KERN_ERR "command timeout because the SD card is removed\n");
        complete(rtk_host->int_waiting);
        writel(0x00000016, rtk_host->sdmmc + CR_SD_ISR);
        return;
    }

    if(broken_flag==true) {
      	complete(rtk_host->int_waiting);
	writel(0x00000016, rtk_host->sdmmc + CR_SD_ISR);
        return;
    }

    u32 int_status = readl(rtk_host->sdmmc+CR_SD_ISR);
    u32 int_status_en = readl(rtk_host->sdmmc+CR_SD_ISREN);
    printk(KERN_ERR"%s: command/data timeout, pre_cmd=%d, cmd=%d, poll=%d, ISR=%x, ISR_EN=%x\n", __func__,pre_cmd,rtk_host->cmd_opcode,poll,int_status,int_status_en);
  
    if(rtk_host->cmd_opcode==8 && int_status==0) {
	CMD8_SD1=true;    //SD 1.x
	printk(KERN_ERR "Reject SD 2.x command\n");
    }
    else if(rtk_host->cmd_opcode==55 && CMD8_SD1==true && (int_status==0x0||int_status==0x6)) {
	CMD55_MMC=true;   //MMC card
	printk(KERN_ERR "Reject SD 1.x command\n");
    }

    complete(rtk_host->int_waiting);
    writel(0x00000016, rtk_host->sdmmc + CR_SD_ISR);
    rtk_sdmmc_sync(rtk_host);

    if(pre_cmd==0 && rtk_host->cmd_opcode==1 && (int_status==0x0 || int_status==0x16)) {
	if(mini_SD==0) {
		printk(KERN_ERR "This might be mini SD card\n");
		mini_SD=1;
		remove_sdcard(rtk_host);
	}
	else {
		printk(KERN_ERR "SD/MMC card is broken!\n");
       		broken_flag=true;
       		card_broken(rtk_host);
       		return;
	}
    }
    //this statement is to workaround some SD card cannot be recognized and read command will return error, tuning the driving or PLL can fix this issue but put this reset function in case
    else if(CMD55_MMC==false && CMD8_SD1==false && rtk_host->cmd_opcode!=8 && (int_status==0x0 || int_status==0x16)) {
	printk(KERN_ERR "SD compatible issue: reset the card and adjust the driving capacity!\n");
	driving_capacity++;
    	remove_sdcard(rtk_host);
    }
}

static void rtk_sdmmc_card_power(struct rtk_sdmmc_host *rtk_host, u8 status)
{
    int res = 0;
    u32 power_status = rtk_host->power_status;
    //void __iomem *pll_base = rtk_host->pll;
    void __iomem *emmc_base = rtk_host->emmc;    //Jim modified
    unsigned long flags2;

    if(status == power_status)
        return;

    if(status){
        res = gpio_direction_output(rtk_host->sdmmc_gpio, 0);

        /* Workaround : OS reboot SD card initial fail, jamestai20150129*/
        //writel(0x33333323, pll_base + CR_PFUNC_CR);
	rtk_lockapi_lock(flags2, __FUNCTION__);
        writel(0x33333323, emmc_base + CR_PFUNC_CR);   //Jim modified
	rtk_lockapi_unlock(flags2, __FUNCTION__);

        if (res < 0){
            printk(KERN_ERR "%s: can't gpio output = %d (power on)\n", __func__, rtk_host->sdmmc_gpio);
        }else{
            power_status = 1; //card is power on
            mdelay(10); //delay 10 ms after power on
        }
    }else{
        res = gpio_direction_input(rtk_host->sdmmc_gpio);

        /* Workaround : OS reboot SD card initial fail, jamestai20150129*/
	rtk_lockapi_lock(flags2, __FUNCTION__);
        writel(0x22223322, emmc_base + CR_PFUNC_CR);    //Jim modified
	rtk_lockapi_unlock(flags2, __FUNCTION__);

        if (res < 0){
            printk(KERN_ERR "%s: can't gpio input = %d (power off)\n", __func__, rtk_host->sdmmc_gpio);
        }else{
            power_status = 0; //card is power off
            mdelay(10); //delay 10 ms after power off
        }
    }
    rtk_host->power_status = power_status;
}

static u8 rtk_sdmmc_chk_cmdcode(struct mmc_command* cmd)
{
    u8 cmdcode = 0;

    if(cmd->opcode < 59){
        if(cmd->opcode == 8 && mmc_detected == 1)
            cmdcode = SD_NORMALREAD;
        else
            cmdcode = rtk_sdmmc_cmdcode[cmd->opcode][0];

        WARN_ON(cmd->data == NULL);

        if(cmd->data->flags & MMC_DATA_WRITE){
            if(cmd->opcode == 42)
                cmdcode = SD_NORMALWRITE;
            else if(cmd->opcode == 56)
                cmdcode = SD_AUTOWRITE2;
        }
    }else
        cmdcode = SD_CMD_UNKNOW;

    return cmdcode;
}

static irqreturn_t rtk_sdmmc_irq(int irq, void *data)
{
    struct rtk_sdmmc_host *rtk_host = (struct rtk_sdmmc_host *)data;
    void __iomem *sdmmc_base = rtk_host->sdmmc;
    u32 int_status = 0;
   
    rtk_sdmmc_sync(rtk_host);
    int_status = readl(sdmmc_base + CR_SD_ISR);
#if 0
    if((num_online_cpus()==1||XEN) && (irq_counter++) > 10 && (!(int_status & ISRSTA_INT1)|| (int_status & ISRSTA_INT2))) {
        if(rtk_host!=NULL && rtk_host->cmd_opcode==55 || rtk_host->cmd_opcode==41 || rtk_host->cmd_opcode==8 || rtk_host->cmd_opcode==1) {
                complete(rtk_host->int_waiting);
                writel(0x00000016, sdmmc_base + CR_SD_ISR);
                rtk_sdmmc_sync(rtk_host);
                return IRQ_HANDLED;
        }
        else if(rtk_host!=NULL && (rtk_host->cmd_opcode==13 || rtk_host->cmd_opcode==19) && (int_status & ISRSTA_INT2) && CMD8_SD1 == true && CMD55_MMC==true) {
                complete(rtk_host->int_waiting);
                writel(0x00000016, sdmmc_base + CR_SD_ISR);
                rtk_sdmmc_sync(rtk_host);
                return IRQ_HANDLED;
        }
    }
    else if((num_online_cpus()==1||XEN) && (irq_counter++) > 100 && rtk_host!=NULL && rtk_host->cmd_opcode!=25 && rtk_host->cmd_opcode!=24) {  //single core cpu, we cannot wait for so long for fear that the irq will cause the system crash
        complete(rtk_host->int_waiting);
        writel(0x00000016, sdmmc_base + CR_SD_ISR);
        rtk_sdmmc_sync(rtk_host);
        return IRQ_HANDLED;
    }
#endif
    if((int_status & ISRSTA_INT2) && rtk_host->cmd_opcode!=13 && rtk_host->cmd_opcode!=19 ) {
	irq_error_bit=1;
	goto clear_irq;
    }
    if(INT4_flag==true) {
#if 0
	if(!(int_status & ISRSTA_INT1) || !(int_status & ISRSTA_INT4)) return IRQ_HANDLED;
#endif
	if(data_xfer_mode==CMD_RSP_DATA_READ || data_xfer_mode==DATA_ONLY)
	{
		if(!(int_status & ISRSTA_INT4)) return IRQ_HANDLED;
	}
	else {
		if(!(int_status & ISRSTA_INT1)) return IRQ_HANDLED;
	}
    }else {
	if(!(int_status & ISRSTA_INT1)) return IRQ_HANDLED;
    }
//    if((int_status & ISRSTA_INT2) && rtk_host->cmd_opcode!=13 && rtk_host->cmd_opcode!=19 ) return IRQ_HANDLED;    //if error bit is set, return except for tuning command
//    if((int_status & ISRSTA_INT2) && pre_cmd==55 && rtk_host->cmd_opcode==13) return IRQ_HANDLED;                      //ACMD13 error, return
clear_irq:
    if(poll==0){
            del_timer(&rtk_host->timer);
            complete(rtk_host->int_waiting);
    }
    writel(0x00000016, sdmmc_base + CR_SD_ISR);
    rtk_sdmmc_sync(rtk_host);
    //printk(KERN_ERR "ISR was called, pre_cmd=%d, cmd=%d, ISR=%x, SD_transfer=%x, poll=%d\n",pre_cmd,rtk_host->cmd_opcode,int_status,readb(sdmmc_base + SD_TRANSFER),poll);
    if( wait_stop_command_irq_done ) {
        wait_stop_command_irq_done = 0; // irq finish
    }
    return IRQ_HANDLED;
}

static struct rtk_sdmmc_host_ops sdmmc_ops = {
    .card_power = rtk_sdmmc_card_power,
    .chk_cmdcode = rtk_sdmmc_chk_cmdcode,
};

static const struct of_device_id rtk_sdmmc_match[] = {
    { .compatible = "Realtek,rtk1295-sdmmc", },
    { /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, rtk_sdmmc_match);

static void rtk_sdmmc_show_version(void)
{
    printk(KERN_INFO "%s: build at : %s \n", DRIVER_NAME, utsname()->version);

#ifdef CONFIG_MMC_BLOCK_BOUNCE
    printk(KERN_INFO "%s: CONFIG_MMC_BLOCK_BOUNCE enable\n", DRIVER_NAME);
#else
    printk(KERN_INFO "%s: CONFIG_MMC_BLOCK_BOUNCE disable\n", DRIVER_NAME);
#endif
}

static int rtk_sdmmc_probe(struct platform_device *pdev)
{
    struct mmc_host *mmc = NULL;
    struct rtk_sdmmc_host *rtk_host = NULL;
    int ret = 0;
    int irq = 0;

    struct device_node *sdmmc_node = pdev->dev.of_node;
    rtk_sdmmc_show_version();
    
    do_stop_command_wo_complete = 0; // probe stage
    wait_stop_command_irq_done = 0; // probe stage
 
    sema_init( &cr_sd_sem, 1 );//probe stage  
    irq = irq_of_parse_and_map(sdmmc_node, 0);
    if(!irq){
        printk(KERN_ERR "%s: fail to parse of irq.\n", __func__);
        return -ENXIO;
    }

    mmc = mmc_alloc_host(sizeof(struct rtk_sdmmc_host), &pdev->dev);
    if(!mmc){
        ret = -ENOMEM;
        goto out;
    }

    rtk_global_host=rtk_host = mmc_priv(mmc);
    memset(rtk_host, 0, sizeof(struct rtk_sdmmc_host));

    rtk_host->pll = of_iomap(sdmmc_node, 0);
    rtk_host->sdmmc = of_iomap(sdmmc_node, 1);
    rtk_host->sysbrdg = of_iomap(sdmmc_node, 2);
    rtk_host->emmc = of_iomap(sdmmc_node, 3);
    rtk_host->sdio = of_iomap(sdmmc_node, 4);
    rtk_host->int_waiting = &rtk_sdmmc_wait;

    rtk_host->sdmmc_gpio = of_get_gpio_flags(sdmmc_node, 0, NULL);
    if(gpio_is_valid(rtk_host->sdmmc_gpio)){
        ret = gpio_request(rtk_host->sdmmc_gpio, "sdmmc_gpio");
        if (ret < 0)
            printk(KERN_ERR "%s: can't request gpio %d\n", __func__, rtk_host->sdmmc_gpio);
    }else
        printk(KERN_ERR "%s: gpio %d is not valid\n", __func__, rtk_host->sdmmc_gpio);

    
    rtk_host->mmc = mmc;
    rtk_host->dev = &pdev->dev;
    rtk_host->ops = &sdmmc_ops;

    rtk_sdmmc_hw_initial(rtk_host);
    
    spin_lock_init(&rtk_host->lock);

    mmc->ocr_avail = MMC_VDD_30_31 |
                     MMC_VDD_31_32 |
                     MMC_VDD_32_33 |
                     MMC_VDD_33_34 |
                     MMC_VDD_165_195;

    mmc->caps = MMC_CAP_4_BIT_DATA |
                MMC_CAP_SD_HIGHSPEED |
                MMC_CAP_MMC_HIGHSPEED |
                MMC_CAP_HW_RESET |
                MMC_CAP_UHS_SDR104 |
                MMC_CAP_UHS_SDR12 |
                MMC_CAP_UHS_SDR25 |
                MMC_CAP_UHS_SDR50;// |
                //MMC_CAP_UHS_DDR50;

    //mmc->caps &= ~MMC_CAP_8_BIT_DATA;

    mmc->f_min = 10000000 >> 8; //RTK min bus clk is 10Mhz/256
    mmc->f_max = 208000000; //RTK max bus clk is 208Mhz
    mmc->max_segs = 1;
    mmc->max_blk_size   = 512;
    mmc->max_blk_count  = 0x1000;
    mmc->max_seg_size   = mmc->max_blk_size * mmc->max_blk_count;
    mmc->max_req_size   = mmc->max_blk_size * mmc->max_blk_count;

    tasklet_init(&rtk_host->req_end_tasklet, rtk_sdmmc_req_end_tasklet, (unsigned long)rtk_host);

    //cmd12_qu  = create_workqueue("rtk_cmd12_wq");
    //INIT_DELAYED_WORK(&rtk_cmd12_delayed_work, rtk_cmd12_func);
    
    ret = request_irq(irq, rtk_sdmmc_irq, IRQF_SHARED, DRIVER_NAME, rtk_host);
    if(ret){
        printk(KERN_ERR "%s: cannot assign irq %d\n", __func__, irq);
        goto out;
    }else
        rtk_host->irq = irq;

    setup_timer(&rtk_host->timer, rtk_sdmmc_timeout, (unsigned long)rtk_host);
    setup_timer(&rtk_host->plug_timer, rtk_sdmmc_plug, (unsigned long)rtk_host);

#ifdef CMD25_WO_STOP_COMMAND
    setup_timer(&rtk_host->rtk_sdmmc_stop_cmd, rtk_sdmmc_cmd12_fun, (unsigned long)rtk_host);
#endif
    mmc->ops = &rtk_sdmmc_ops;
    rtk_host->rtflags &= ~(RTKCR_FCARD_DETECTED|RTKCR_FCARD_SELECTED);
    rtk_host->int_status_old &= ~SD_EXISTENCE;
    sdmmc_rca = 0;
    g_crinit = 0;

    platform_set_drvdata(pdev, mmc);

    rtk_sdmmc_sync(rtk_host);

    ret = mmc_add_host(mmc);
    if(ret)
        goto out;

    mod_timer(&rtk_host->plug_timer, jiffies + 3*HZ);
//    #ifdef CMD25_WO_STOP_COMMAND
    //-------------------kernel thread for last cmd 12 ----------------------------------
      /*  rtk_SD_task = kthread_run(rtk_scan_fun, (unsigned long)rtk_host, "rtk_int_cmd12_handler");
        if (IS_ERR(rtk_SD_task)) {
                printk(KERN_ERR "KERNEL thread for SD failed!!!\n");
                rtk_SD_task = NULL;
                //return -1;
        }*/
    //-----------------------------------------------------------------------------------
//    #endif
    return 0;

out:
    if(mmc)
        mmc_free_host(mmc);
    return ret;
}

static int rtk_sdmmc_remove(struct platform_device *pdev)
{
    struct mmc_host *mmc = platform_get_drvdata(pdev);
    if(mmc){
	//kthread_stop(rtk_SD_task);
        struct rtk_sdmmc_host *rtk_host = mmc_priv(mmc);

        flush_scheduled_work();

        rtk_sdmmc_free_dma_buf(rtk_host);

        mmc_remove_host(mmc);
        if(!mmc){
            printk(KERN_INFO "Realtek SD/MMC host have removed.\n");
        }

        free_irq(rtk_host->irq, rtk_host);

        del_timer_sync(&rtk_host->timer);
        del_timer_sync(&rtk_host->plug_timer);

#ifdef CMD25_WO_STOP_COMMAND
        del_timer_sync(&rtk_host->rtk_sdmmc_stop_cmd);
#endif
        if(rtk_host->sdmmc)
            iounmap(rtk_host->sdmmc);

        if(rtk_host->pll)
            iounmap(rtk_host->pll);

        mmc_free_host(mmc);
        gpio_free(rtk_host->sdmmc_gpio);
    }

    platform_set_drvdata(pdev, NULL);

    return 0;
}
EXPORT_SYMBOL_GPL(rtk_sdmmc_remove);

static struct platform_driver rtk_sdmmc_driver = {
    .probe = rtk_sdmmc_probe,
    .remove = rtk_sdmmc_remove,
    .driver = {
        .name = DRIVER_NAME,
        .pm = &rtk_sdmmc_pmops,
        .of_match_table = rtk_sdmmc_match,
    },
};

module_platform_driver(rtk_sdmmc_driver);

MODULE_DESCRIPTION("Realtek SD/MMC host driver");
MODULE_AUTHOR("James Tai");
MODULE_LICENSE("GPL");
