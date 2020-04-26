/*
 * Realtek EMMC driver
 *
 * Authors:
 * Copyright (C) 2015 Realtek Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#include <linux/mbus.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/dma-mapping.h>
#include <linux/scatterlist.h>
#include <linux/irq.h>
#include <linux/gpio.h>
#include <linux/mmc/host.h>
#include <asm/unaligned.h>

#include <linux/sched.h>                
#include <linux/wait.h>                 
#include <linux/slab.h>                 
#include <linux/semaphore.h>           
#include <linux/mmc/card.h>            
#include <linux/mmc/mmc.h>              
#include <linux/mmc/sd.h>              
#include <linux/workqueue.h>            
#include <linux/completion.h>           
#include "reg_mmc.h"               
#include "reg_sys.h"               
#include "reg_iso.h"               
#include "rtkemmc.h"                    
#include "rtkemmc_ops.h"                    
#include "mmc_debug.h"  
#include <soc/realtek/rtd129x_cpu.h>
#include <soc/realtek/rtd129x_lockapi.h>


#ifdef MMC_DBG
static unsigned sd_reg = 0;
#endif
extern volatile int g_bTuning;
extern u32 rtkemmc_global_blkcnt;
extern u32 rtkemmc_global_blksize;
extern u32 rtkemmc_global_bytecnt;
extern u32 rtkemmc_global_dbaddr;
extern int cmd_resend;
DECLARE_COMPLETION(rtk_emmc_wait);

void sync(struct rtkemmc_host *emmc_port)
{
    	dmb(sy);
    	writel(0x0, emmc_port->sb2_membase+0x20);
    	dmb(sy);
}
EXPORT_SYMBOL_GPL(sync);

#define TIMEOUT_MS 3000
#define TIMEOUT_DMA 50
#define TIMEOUT_CMD 10000 //10 secs
int rtkemmc_wait_opt_end(char* drv_name, struct rtkemmc_host *emmc_port,u8 cmdcode,u32 cmd_idx,u8 ignore_log)
{
	volatile u8 sd_transfer_reg;
	volatile int loops=0,dma_val=0;
    	volatile int err=CR_TRANS_OK;
    	volatile unsigned long timeend=0;
    	volatile unsigned int sd_trans=0;
    	unsigned long flags;
    	unsigned int dma_to=0;

	switch(CMD_IDX_MASK(cmd_idx))
	{
		case MMC_READ_SINGLE_BLOCK:
		case MMC_READ_MULTIPLE_BLOCK:
		case MMC_WRITE_BLOCK:
		case MMC_WRITE_MULTIPLE_BLOCK:
		case MMC_SEND_EXT_CSD:
			dma_to=TIMEOUT_DMA;
			break;
	}
	MMCPRINTF("%s - data_to=%u, dma_to=0x%08x\n","rtkemmc",TIMEOUT_MS,dma_to);
        err = rtk_int_enable_and_waitfor(emmc_port,cmdcode,cmd_idx,TIMEOUT_MS,dma_to,ignore_log);
    	return err;
}
EXPORT_SYMBOL_GPL(rtkemmc_wait_opt_end);

int rtk_int_enable_and_waitfor(struct rtkemmc_host *emmc_port, u8 cmdcode, u32 cmd_idx, unsigned long msec, unsigned long dma_msec, u8 ignore_log)
{
    	volatile unsigned long timeend=0;
	int err = 0;
#ifndef ENABLE_EMMC_INT_MODE
    	emmc_port->int_waiting = NULL;
#else
    	emmc_port->int_waiting = &rtk_emmc_wait;
	MMCPRINTF("%s : msec = %u \n", __func__, msec);
	MMCPRINTF("%s : emmc_port->tmout = %u \n", __func__, emmc_port->tmout);
	MMCPRINTF("rtk wait complete addr = %08x\n", (unsigned int) emmc_port->int_waiting);
    	/* timeout timer fire */
    	if (&emmc_port->timer)
    	{
#ifndef CONFIG_MMC_RTKEMMC_JIFFY_NOT_WORK_ON_1_LAYER_FPGA	
		MMCPRINTF("now = 0x%08x\n", jiffies);
        	MMCPRINTF("timer started : ");
        	timeend = jiffies + msecs_to_jiffies(msec)+emmc_port->tmout;
        	MMCPRINTF("TO = 0x%08x\n", timeend);
        	mod_timer(&emmc_port->timer, timeend );
#endif
    	}
#endif

    	err = rtk_int_waitfor(emmc_port,cmdcode,cmd_idx,msec,dma_msec,ignore_log);

    	return err;
}
EXPORT_SYMBOL_GPL(rtk_int_enable_and_waitfor);

int rtk_int_waitfor(struct rtkemmc_host *emmc_port, u8 cmdcode, u32 cmd_idx, unsigned long msec,unsigned long dma_msec,u8 ignore_log)
{
    	unsigned long flags;
    	unsigned long flags2;
    	unsigned long timeend=0;
    	u32 err = RTK_SUCC;
	u32 reg = 0;
	u32 reg2 = 0;
	u32 reg3 = 0;
	u32 reg_blksize = 0;
	u32 reg_bytecount = 0;	
	u32 reg_cmdidx = 0;
	u32 reg_argu = 0;
	u32 i = 0;

#ifndef ENABLE_EMMC_INT_MODE
	// remove by jinn for readable code
//<--------------ENABLE_EMMC_INT_MODE start------------------>
#else
    //printk(KERN_ERR "cmd_idx:%08x rintsts:0x%08x cmdarg:0x%08x------------------------------>\n",cmd_idx,readl(emmc_port->emmc_membase+EMMC_RINTSTS),readl(emmc_port->emmc_membase+EMMC_CMDARG));
    	if(emmc_port->rtflags & RTKCR_FOPEN_LOG){
    		printk(KERN_ERR "cmd_idx:%08x ------------------------------>\n",cmd_idx);
    		printk(KERN_ERR "rtkemmc - before cmd fired\n\trintsts : 0x%08x mintsts : 0x%08x, status : 0x%08x, idsts : 0x%08x\n\tcmd=0x%08x, cmd_arg=0x%08x\n", readl(emmc_port->emmc_membase+EMMC_RINTSTS),readl(emmc_port->emmc_membase+EMMC_MINTSTS),readl(emmc_port->emmc_membase+EMMC_STATUS),readl(emmc_port->emmc_membase+EMMC_IDSTS),cmd_idx,readl(emmc_port->emmc_membase+EMMC_CMDARG));
    		printk(KERN_ERR "\tbmod : 0x%08x ctrl : 0x%08x, pwren : 0x%08x, intmask : 0x%08x\n\tclkdiv : 0x%08x, clksrc : 0x%08x, clkena : 0x%08x, cmd : 0x%08x\n", readl(emmc_port->emmc_membase+EMMC_BMOD),readl(emmc_port->emmc_membase+EMMC_CTRL),readl(emmc_port->emmc_membase+EMMC_PWREN),readl(emmc_port->emmc_membase+EMMC_INTMASK),readl(emmc_port->emmc_membase+EMMC_CLKDIV),readl(emmc_port->emmc_membase+EMMC_CLKSRC),readl(emmc_port->emmc_membase+EMMC_CLKENA),readl(emmc_port->emmc_membase+EMMC_CMD));
    		printk(KERN_ERR "\ttmout : 0x%08x ctype : 0x%08x, fifoth : 0x%08x, idinten : 0x%08x\n\tuhsreg : 0x%08x\n", readl(emmc_port->emmc_membase+EMMC_TMOUT),readl(emmc_port->emmc_membase+EMMC_CTYPE),readl(emmc_port->emmc_membase+EMMC_FIFOTH),readl(emmc_port->emmc_membase+EMMC_IDINTEN),readl(emmc_port->emmc_membase+EMMC_UHSREG));
    		printk(KERN_ERR "------------------------------>\n");
    	}

	if (emmc_port->int_waiting)
	{
		rtk_lockapi_lock2(flags2, _at_("rtk_int_waitfor"));
		rtkemmc_hold_int_dec();
		//gpio 100 pull low
#ifdef EMMC_LA_DEBUG_GPIO
		reg = readl(emmc_port->misc_membase + MISC_GP3DATO);
		reg &= (~0x00000010);
		rtkemmc_writel(reg, emmc_port->misc_membase + MISC_GP3DATO);
		isb();
		sync(emmc_port);
#endif		
		rtkemmc_clr_int_sta();
		ClrINTState(emmc_port);
		emmc_port->int_waiting = &rtk_emmc_wait;

		//gpio 100 pull high
#ifdef EMMC_LA_DEBUG_GPIO
		reg = readl(emmc_port->misc_membase + MISC_GP3DATO);
		reg |= (0x00000010);
		rtkemmc_writel(reg, emmc_port->misc_membase + MISC_GP3DATO);
		isb();
		sync(emmc_port);
#endif			
		
		//enable necessary interrupts
		if (CMD_IDX_MASK(cmd_idx)==MMC_WRITE_MULTIPLE_BLOCK || CMD_IDX_MASK(cmd_idx)==MMC_WRITE_BLOCK )
			rtkemmc_en_wr_int();//wr case
		else if (CMD_IDX_MASK(cmd_idx)==MMC_READ_MULTIPLE_BLOCK || CMD_IDX_MASK(cmd_idx)==MMC_READ_SINGLE_BLOCK
			|| CMD_IDX_MASK(cmd_idx)==MMC_SEND_EXT_CSD)//rd case
			rtkemmc_en_rd_int();
/*		
		else if (CMD_IDX_MASK(cmd_idx)==MMC_SWITCH)
			rtkemmc_en_cd6_int();
*/
		else
			rtkemmc_en_cd_int(); //command case

			
		//EMMC_CMD bit 31 must be zero
		BUG_ON(readl(emmc_port->emmc_membase+EMMC_CMD) & CMD_START_CMD);

		//if in hs200 tuning, not to check the RINTSTS
		if (g_bTuning == 0){
#ifdef EMMC_LA_DEBUG_GPIO
			//gpio 90  pull  low
			reg = readl(emmc_port->misc_membase + MISC_GP2DATO);
			reg &= ~(0x04000000);
			rtkemmc_writel(reg, emmc_port->misc_membase + MISC_GP2DATO);
			isb();
			sync(emmc_port);
#endif			
			while (readl(emmc_port->emmc_membase+EMMC_RINTSTS) & (0xfffe)){
				printk(KERN_ERR RED_BOLD"Fail to clear EMMC_RINTSTS! EMMC_RINTSTS = 0x%08x, CMDIDX = %d \n"RESET, readl(emmc_port->emmc_membase+EMMC_RINTSTS), CMD_IDX_MASK(cmd_idx));
				rtkemmc_writel(readl(emmc_port->emmc_membase+EMMC_RINTSTS)&0x0000fffe, emmc_port->emmc_membase+EMMC_RINTSTS);
										
			}
			
#ifdef EMMC_LA_DEBUG_GPIO
			//gpio 90  pull  high
			reg = readl(emmc_port->misc_membase + MISC_GP2DATO);
			reg |= (0x04000000);
			rtkemmc_writel(reg, emmc_port->misc_membase + MISC_GP2DATO);
			isb();
			sync(emmc_port);
#endif			
		}

		if (CMD_IDX_MASK(cmd_idx)==MMC_WRITE_MULTIPLE_BLOCK || CMD_IDX_MASK(cmd_idx)==MMC_READ_MULTIPLE_BLOCK ){
			if (emmc_port->rpmb_cmd){
				cmd_idx &= (~CMD_SEND_AUTO_STOP); //CMD25/18 following CMD13, never set CMD_SEND_AUTO_STOP(bit 12)
			}
		}

		//cmd fire
		spin_lock_irqsave(&emmc_port->lock,flags);
		reg_blksize = readl(emmc_port->emmc_membase+EMMC_BLKSIZE);
		reg_bytecount = readl(emmc_port->emmc_membase+EMMC_BYTCNT);
		reg_argu = readl(emmc_port->emmc_membase+EMMC_CMDARG);
		rtkemmc_writel(cmd_idx,emmc_port->emmc_membase+EMMC_CMD);
		reg_cmdidx = readl(emmc_port->emmc_membase+EMMC_CMD);
		spin_unlock_irqrestore(&emmc_port->lock,flags);
		rtk_lockapi_unlock2(flags2, _at_("rtk_int_waitfor"));
		wait_for_completion(emmc_port->int_waiting);
		if (emmc_port->rintsts & INT_STS_ERRORS)
		{	
/*		
			if (CMD_IDX_MASK(cmd_idx)==MMC_SWITCH && (emmc_port->rintsts & INT_STS_ERRORS2) == 0) {
				goto chk_status;
			}
*/			
			if (!ignore_log)
			{
				if (CMD_IDX_MASK(cmd_idx)!=MMC_GO_IDLE_STATE) {
					printk(KERN_ERR "rtkemmc - INT_STS_ERRORS, cmd_idx=0x%08x, rintsts=0x%08x, idsts=0x%08x, status=0x%08x\n", cmd_idx, emmc_port->rintsts, emmc_port->idsts, emmc_port->status);
					printk(KERN_ERR "rtkemmc - read cmd_idx = 0x%08x\n", reg_cmdidx);
					printk(KERN_ERR "rtkemmc - read reg_argu = 0x%08x\n", reg_argu);
					printk(KERN_ERR "rtkemmc - read byte count = 0x%08x\n", reg_bytecount);
					printk(KERN_ERR "rtkemmc - read blk size = 0x%08x\n", reg_blksize);
					printk(KERN_ERR "rtkemmc - read rtkemmc_global_blksize = 0x%08x\n", rtkemmc_global_blksize);
					printk(KERN_ERR "rtkemmc - read rtkemmc_global_bytecnt = 0x%08x\n", rtkemmc_global_bytecnt);
					printk(KERN_ERR "rtkemmc - read rtkemmc_global_dbaddr = 0x%08x\n", rtkemmc_global_dbaddr);
					print_reg_sts(cmd_idx,emmc_port->rintsts);
					print_ip_desc(emmc_port);
					if((emmc_port->rintsts & INT_STS_DCRC) && (CMD_IDX_MASK(cmd_idx)==18 || CMD_IDX_MASK(cmd_idx)==25) ) {
                                                cmd_resend=1;
                                        }
					else if((emmc_port->rintsts & INT_STS_RTO_BAR) && CMD_IDX_MASK(cmd_idx)==6)
                                                cmd_resend=1;
				}
			}
			
			else{ //in tuning, only print rintsts
				if (CMD_IDX_MASK(cmd_idx)!=MMC_GO_IDLE_STATE && (emmc_port->rintsts & (INT_STS_HTO | INT_STS_HLE | INT_STS_DRTO_BDS))) {
					printk(KERN_ERR "Tuning: HTO/HLE/DRTO - INT_STS_ERRORS, cmd_idx=0x%08x, rintsts=0x%08x, idsts=0x%08x, status=0x%08x\n", cmd_idx, emmc_port->rintsts, emmc_port->idsts, emmc_port->status);
				}
			}
			//HTO always panic
			//if (emmc_port->rintsts & (INT_STS_HTO | INT_STS_HLE | INT_STS_DRTO_BDS))
			//	BUG();

			err = RTK_FAIL;
			
			// in HS200 tuning, skip status busy check with emmc error
			if (g_bTuning)
				goto chk_cmd_bit31;
			else{
				//BUG();
				goto chk_status;
			}
		}
		
		//polling ACD for multiple r/w
		if (CMD_IDX_MASK(cmd_idx)==MMC_WRITE_MULTIPLE_BLOCK || CMD_IDX_MASK(cmd_idx)==MMC_READ_MULTIPLE_BLOCK )
		{
			 //for rpmb rw cmd, don't need to polling ACD
			if (emmc_port->rpmb_cmd){
				emmc_port->rpmb_cmd = 0;
			}
			else{
#ifndef CONFIG_MMC_RTKEMMC_JIFFY_NOT_WORK_ON_1_LAYER_FPGA
				timeend = jiffies + msecs_to_jiffies(msec);
				MMCPRINTF("[%s:%d] Start polling ACD........\n", __FILE__, __LINE__);
		    		while(time_before(jiffies, timeend))
#else
				MMCPRINTF("[%s:%d] Start polling ACD........\n", __FILE__, __LINE__);
				while(1)
#endif
		    		{ 
					rtk_lockapi_lock2(flags2, _at_("rtk_int_waitfor"));
					reg = readl(emmc_port->emmc_membase+EMMC_RINTSTS);
					rtk_lockapi_unlock2(flags2, _at_("rtk_int_waitfor"));
					if (reg & INT_STS_ERRORS){
						err = RTK_FAIL;
						if (g_bTuning)
							goto chk_cmd_bit31;
						else{
							//BUG();
							goto chk_status;
						}
					}

					if ( reg & (INT_STS_ACD))
					{
						err = RTK_SUCC;
						
						if (get_rtd129x_cpu_revision() < RTD129x_CHIP_REVISION_A01 ) {				
							for (i = 0; i < 100000; i++){
								asm volatile("nop" : : : );
							}
						}

#if 1
	                   // move to while - loop if (((readl(emmc_port->emmc_membase+EMMC_STATUS) & STS_RSP_IDX_MASK)>>STS_RSP_IDX) != MMC_STOP_TRANSMISSION)
	                   // move to while - loop {
	                   // move to while - loop         printk(KERN_ERR RED_BOLD"[%s:%d] wait STOP after ACD fail ........\n"RESET, __FILE__, __LINE__);
	                   // move to while - loop         printk(KERN_ERR RED_BOLD"rtkemmc - \n\trintsts : 0x%08x mintsts : 0x%08x, status : 0x%08x, idsts : 0x%08x\n\tcmd=0x%08x, cmd_arg=0x%08x\n"RESET, readl(emmc_port->emmc_membase+EMMC_RINTSTS),readl(emmc_port->emmc_membase+EMMC_MINTSTS),readl(emmc_port->emmc_membase+EMMC_STATUS),readl(emmc_port->emmc_membase+EMMC_IDSTS),cmd_idx,readl(emmc_port->emmc_membase+EMMC_CMDARG));
	                   // move to while - loop }
	                   // move to while - loop sync(emmc_port);
	                   			i=0;
	                   			//while (((readl(emmc_port->emmc_membase+EMMC_STATUS) & STS_RSP_IDX_MASK)>>STS_RSP_IDX) != MMC_STOP_TRANSMISSION)
	                   			while (1)
	                   			{
				       			rtk_lockapi_lock2(flags2, _at_("rtk_int_waitfor"));
		                       			reg = readl(emmc_port->emmc_membase+EMMC_STATUS);
		                       			asm volatile("nop" : : : );
		                       			rtk_lockapi_unlock2(flags, _at_("rtk_int_waitfor"));
		                       			if( ( (reg & STS_RSP_IDX_MASK)>>STS_RSP_IDX ) == MMC_STOP_TRANSMISSION )
		                       			{
		                           			break;
		                       			}
		                       			if ( i == 0 )
		                       			{
		                           	    		rtk_lockapi_lock2(flags2, _at_("rtk_int_waitfor"));
		                               			printk(KERN_ERR RED_BOLD"[%s:%d] wait STOP after ACD fail ........\n"RESET, __FILE__, __LINE__);
		                               			printk(KERN_ERR RED_BOLD"rtkemmc - \n\trintsts : 0x%08x mintsts : 0x%08x, status : 0x%08x, idsts : 0x%08x\n\tcmd=0x%08x, cmd_arg=0x%08x\n"RESET, readl(emmc_port->emmc_membase+EMMC_RINTSTS),readl(emmc_port->emmc_membase+EMMC_MINTSTS),readl(emmc_port->emmc_membase+EMMC_STATUS),readl(emmc_port->emmc_membase+EMMC_IDSTS),cmd_idx,readl(emmc_port->emmc_membase+EMMC_CMDARG));
		                               			rtk_lockapi_unlock2(flags, _at_("rtk_int_waitfor"));
		                       			}
	                               			if (i++ > 0x100000)
	                               			{
	                                   			rtk_lockapi_lock2(flags2, _at_("rtk_int_waitfor"));
	                                   			printk(KERN_ERR RED_BOLD"[%s:%d] polling STOP after ACD fail ........\n"RESET, __FILE__, __LINE__);
	                                   			printk(KERN_ERR RED_BOLD"rtkemmc - \n\trintsts : 0x%08x mintsts : 0x%08x, status : 0x%08x, idsts : 0x%08x\n\tcmd=0x%08x, cmd_arg=0x%08x\n"RESET, readl(emmc_port->emmc_membase+EMMC_RINTSTS),readl(emmc_port->emmc_membase+EMMC_MINTSTS),readl(emmc_port->emmc_membase+EMMC_STATUS),readl(emmc_port->emmc_membase+EMMC_IDSTS),cmd_idx,readl(emmc_port->emmc_membase+EMMC_CMDARG));
	                                   			rtk_lockapi_unlock2(flags, _at_("rtk_int_waitfor"));
	                                   			break;
	                              			}
							usleep_range(10, 30);

	                   			}
#endif

#if 0				
						//printk(KERN_ERR "POLLING ACD DONE \n");
						if (CMD_IDX_MASK(cmd_idx)== MMC_READ_MULTIPLE_BLOCK)
						{
							//gpio 90  pull  high
							reg = readl(emmc_port->misc_membase + MISC_GP2DATO);
							reg |= (0x04000000);
							rtkemmc_writel(reg, emmc_port->misc_membase + MISC_GP2DATO);
							isb();
							sync(emmc_port);
						}
#endif
		        			break;
					}
					usleep_range(10, 30);

				}
				MMCPRINTF("[%s:%d] End polling ACD........\n", __FILE__, __LINE__);
#ifndef CONFIG_MMC_RTKEMMC_JIFFY_NOT_WORK_ON_1_LAYER_FPGA
				rtk_lockapi_lock2(flags2, _at_("rtk_int_waitfor"));
				if (!(readl(emmc_port->emmc_membase+EMMC_RINTSTS) & INT_STS_ACD) && time_after_eq(jiffies, timeend)) {
                                        if (!ignore_log)
                                                printk(KERN_ERR "[%s:%d] ACD Timeout!!!........, EMMC_RINTSTS=0x%x\n", __FILE__, __LINE__,readl(emmc_port->emmc_membase+EMMC_RINTSTS));
                                        rtk_lockapi_unlock2(flags2, _at_("rtk_int_waitfor"));
					err = RTK_TOUT;
                                        goto chk_status;
                                }
				else rtk_lockapi_unlock2(flags2, _at_("rtk_int_waitfor"));

#endif		
			}
	    }	

		//polling dma_done_int is not reliable when transaction > 4KB, i.e, more than one set of descriptor, 
		//because dma_done_int raised when one descriptor finished.
		if (dma_msec > 0)
		{
			if ((CMD_IDX_MASK(cmd_idx)==MMC_READ_SINGLE_BLOCK)||(CMD_IDX_MASK(cmd_idx)==MMC_READ_MULTIPLE_BLOCK)||(CMD_IDX_MASK(cmd_idx)==MMC_SEND_EXT_CSD))
			{
#ifndef CONFIG_MMC_RTKEMMC_JIFFY_NOT_WORK_ON_1_LAYER_FPGA
				timeend = jiffies + msecs_to_jiffies(dma_msec);
				MMCPRINTF("[%s:%d] Start polling DMA_DONE........\n", __FILE__, __LINE__);
		    		while(time_before(jiffies, timeend))
#else
				MMCPRINTF("[%s:%d] Start polling DMA_DONE........\n", __FILE__, __LINE__);
				while(1)
#endif
		    		{
					rtk_lockapi_lock2(flags2, _at_("rtk_int_waitfor"));
					reg = readl(emmc_port->emmc_membase+EMMC_RINTSTS);
					rtk_lockapi_unlock2(flags2, _at_("rtk_int_waitfor"));
					if (reg & INT_STS_ERRORS){
						err = RTK_FAIL;
						if (g_bTuning)
							goto chk_cmd_bit31;
						else{
							//BUG();
							goto chk_status;
						}
					}
				
					rtk_lockapi_lock2(flags2, _at_("rtk_int_waitfor"));
		    			if (readl(emmc_port->emmc_membase+EMMC_ISR) & ISR_DMA_DONE_INT)
					{
						//printk(KERN_ERR "POLLING DMA_DONE \n");
						//clear dma_done_int
						rtkemmc_writel(ISR_DMA_DONE_INT, emmc_port->emmc_membase+EMMC_ISR); 
						err = RTK_SUCC;
		    				MMCPRINTF("RTKEMMC - rd done, rintsts : 0x%08x, isr : 0x%08x, idsts : 0x%08x\n", readl(emmc_port->emmc_membase+EMMC_RINTSTS), readl(emmc_port->emmc_membase+EMMC_ISR), readl(emmc_port->emmc_membase+EMMC_IDSTS));
		    				rtk_lockapi_unlock2(flags2, _at_("rtk_int_waitfor"));
		        			break;
					}
					rtk_lockapi_unlock2(flags2, _at_("rtk_int_waitfor"));

					MMCPRINTF("ISR = 0x%08x \n", readl(emmc_port->emmc_membase+EMMC_ISR));
					usleep_range(10, 30);

				
				}

				MMCPRINTF("[%s:%d] End polling DMA_DONE........\n", __FILE__, __LINE__);
#ifndef CONFIG_MMC_RTKEMMC_JIFFY_NOT_WORK_ON_1_LAYER_FPGA
				rtk_lockapi_lock2(flags2, _at_("rtk_int_waitfor"));
				 if ((readl(emmc_port->emmc_membase+EMMC_ISR) & ISR_DMA_DONE_INT) && time_after_eq(jiffies, timeend)) {
                                        printk(KERN_ERR "[%s:%d] EMMC_ISR=0x%x, DMA_DONE Timeout !!!........\n", __FILE__, __LINE__,readl(emmc_port->emmc_membase+EMMC_ISR));
					rtk_lockapi_unlock2(flags2, _at_("rtk_int_waitfor"));
                                        err = RTK_TOUT;
                                        goto chk_status;
                                }
				else rtk_lockapi_unlock2(flags2, _at_("rtk_int_waitfor"));
#endif			
		    	}

		}		
					
chk_status: //check status busy 
		
#ifndef CONFIG_MMC_RTKEMMC_JIFFY_NOT_WORK_ON_1_LAYER_FPGA
		timeend = jiffies + msecs_to_jiffies(msec);
		//pr_err("[%s:%d] Start polling STATUS BUSY........\n", __FILE__, __LINE__);
	    	while(time_before(jiffies, timeend))
#else
		//pr_err("[%s:%d] Start polling STATUS BUSY........\n", __FILE__, __LINE__);
		while(1)
#endif			
		{
			rtk_lockapi_lock2(flags2, _at_("rtk_int_waitfor"));
	       		if ((readl(emmc_port->emmc_membase+EMMC_STATUS) & STS_DATA_BUSY)== 0)
			{
				SetINTState(emmc_port, INT_STAT_DATA_DONE);
				rtk_lockapi_unlock2(flags2, _at_("rtk_int_waitfor"));
         			break;
			}
			rtk_lockapi_unlock2(flags2, _at_("rtk_int_waitfor"));
			usleep_range(500, 1000);
		}
		//pr_err("[%s:%d] End polling STATUS BUSY........\n", __FILE__, __LINE__);
#ifndef CONFIG_MMC_RTKEMMC_JIFFY_NOT_WORK_ON_1_LAYER_FPGA
		rtk_lockapi_lock2(flags2, _at_("rtk_int_waitfor"));
		if ((readl(emmc_port->emmc_membase+EMMC_STATUS) & STS_DATA_BUSY) && time_after_eq(jiffies, timeend)) {
                        printk(KERN_ERR "[%s:%d] STS_DATA_BUSY Timeout !!!........, EMMC_STATUS=%x\n", __FILE__, __LINE__,readl(emmc_port->emmc_membase+EMMC_STATUS));
			rtk_lockapi_unlock2(flags2, _at_("rtk_int_waitfor"));
                        err = RTK_TOUT;
                }
		else rtk_lockapi_unlock2(flags2, _at_("rtk_int_waitfor"));
#endif	

chk_cmd_bit31:
		rtk_lockapi_lock2(flags2, _at_("rtk_int_waitfor"));
		MMCPRINTF("RTKEMMC - done: rintsts : 0x%08x, isr : 0x%08x, idsts : 0x%08x\n", emmc_port->rintsts, emmc_port->dma_isr, readl(emmc_port->emmc_membase+EMMC_IDSTS));
		rtk_lockapi_unlock2(flags2, _at_("rtk_int_waitfor"));
#ifndef CONFIG_MMC_RTKEMMC_JIFFY_NOT_WORK_ON_1_LAYER_FPGA
		timeend = jiffies + msecs_to_jiffies(TIMEOUT_CMD); //timeout 10 secs
	   	while(time_before(jiffies, timeend))
#else
		while(1)
#endif	
		{
			rtk_lockapi_lock2(flags2, _at_("rtk_int_waitfor"));
			reg = readl(emmc_port->emmc_membase+EMMC_CMD);
			rtk_lockapi_unlock2(flags2, _at_("rtk_int_waitfor"));
			if( (reg & CMD_START_CMD) == 0)
				break;
			usleep_range(10, 30);
				
		}
#ifndef CONFIG_MMC_RTKEMMC_JIFFY_NOT_WORK_ON_1_LAYER_FPGA
		rtk_lockapi_lock2(flags2, _at_("rtk_int_waitfor"));
		if ((readl(emmc_port->emmc_membase+EMMC_CMD) & CMD_START_CMD) && time_after_eq(jiffies, timeend))
		{
			printk(KERN_ERR RED_BOLD"EMMC_CMD bit31 can't recover to 0 \n"RESET);
			rtk_lockapi_unlock2(flags2, _at_("rtk_int_waitfor"));
			BUG();
		}
		else rtk_lockapi_unlock2(flags2, _at_("rtk_int_waitfor"));

#endif	

		//cmd 13 for tx tuning
		if (g_bTuning && err != RTK_SUCC && CMD_IDX_MASK(cmd_idx)== MMC_SEND_STATUS){
			rtk_lockapi_lock2(flags2, _at_("rtk_int_waitfor"));
			reg = readl(emmc_port->emmc_membase+EMMC_RINTSTS);
			rtk_lockapi_unlock2(flags2, _at_("rtk_int_waitfor"));
			if (!(reg & 0x8180)){ //only 0x8180 is the real error for tx tuning
				err = RTK_SUCC;
			}
		}
		
		return err;
	}
	err = RTK_FAIL;
    	return err;
#endif
}
EXPORT_SYMBOL_GPL(rtk_int_waitfor);

void print_reg_sts(u32 cmd_idx, u32 rintsts)
{
	printk(KERN_ERR "=====================================================\n");
	printk(KERN_ERR "cmd_idx 0x%08x, op_code 0x%02x(%d)\n", cmd_idx, CMD_IDX_MASK(cmd_idx), CMD_IDX_MASK(cmd_idx));
	if( rintsts & INT_STS_EBE ) {		printk(KERN_ERR "bit 15: Enb bit error(R), No CRC(W)\n");	}
	if( rintsts & INT_STS_ACD ) {		printk(KERN_ERR "bit 14: Auto commanddone\n");	}
	if( rintsts & INT_STS_SBE_BCI ) {	printk(KERN_ERR "bit 13: Start Bit Error, Busy Clear Int.\n");	}
	if( rintsts & INT_STS_HLE ) {		printk(KERN_ERR "bit 12: Hardware locked write error\n");	}
	if( rintsts & INT_STS_FRUN ) {		printk(KERN_ERR "bit 11: FIFIL underrun/overrun\n");	}
	if( rintsts & INT_STS_HTO ) {		printk(KERN_ERR "bit 10: Data startvation by host timeout(HTO)\n");	}
	if( rintsts & INT_STS_DRTO_BDS ) {	printk(KERN_ERR "bit  9: Data read timeout\n");	}
	if( rintsts & INT_STS_RTO_BAR ) {	printk(KERN_ERR "bit  8: Response timeout\n");	}
	if( rintsts & INT_STS_DCRC ) {		printk(KERN_ERR "bit  7: Data CRC error\n");	}
	if( rintsts & INT_STS_RCRC ) {		printk(KERN_ERR "bit  6: Response CRC error\n");	}
	if( rintsts & INT_STS_RXDR ) {		printk(KERN_ERR "bit  5: Receive FIFO data request(RXDT)\n");	}
	if( rintsts & INT_STS_TXDR ) {		printk(KERN_ERR "bit  4: Transmit FIFO data request(TXDR)\n");	}
	if( rintsts & INT_STS_DTO ) {		printk(KERN_ERR "bit  3: Data Transfer over(DTO)\n");	}
	if( rintsts & INT_STS_CD ) {		printk(KERN_ERR "bit  2: Command done(CD)\n");	}
	if( rintsts & INT_STS_RE ) {		printk(KERN_ERR "bit  1: Response error\n");	}
	if( rintsts & INT_STS_CRD ) {		printk(KERN_ERR "bit  0: Card detect(CD)\n");	}
	printk(KERN_ERR "=====================================================\n");
}

extern unsigned int* gddr_descriptor;
void print_ip_desc(struct rtkemmc_host *emmc_port){
	unsigned int reg = 0;
	unsigned int bytecnt = 0, desc_cnt = 0, i = 0;
	unsigned int paddr_base = emmc_port->desc_paddr;
	
	printk(KERN_ERR "------------------------------>\n");
	printk(KERN_ERR "EMMC IP_DESC0 = 0x%08x\n", readl(emmc_port->emmc_membase+EMMC_IP_DESC0));
	printk(KERN_ERR "EMMC IP_DESC1 = 0x%08x\n", readl(emmc_port->emmc_membase+EMMC_IP_DESC1));
	printk(KERN_ERR "EMMC IP_DESC2 = 0x%08x\n", readl(emmc_port->emmc_membase+EMMC_IP_DESC2));
	printk(KERN_ERR "EMMC IP_DESC3 = 0x%08x\n", readl(emmc_port->emmc_membase+EMMC_IP_DESC3));

	printk(KERN_ERR "EMMC BYTCNT = 0x%08x\n", readl(emmc_port->emmc_membase+EMMC_BYTCNT));
	printk(KERN_ERR "EMMC CMDARG = 0x%08x\n", readl(emmc_port->emmc_membase+EMMC_CMDARG));
	printk(KERN_ERR "EMMC DBADDR = 0x%08x\n------------------------------>\n", readl(emmc_port->emmc_membase+EMMC_DBADDR));

	bytecnt = readl(emmc_port->emmc_membase+EMMC_BYTCNT);
	desc_cnt = bytecnt / 4096;

	for (i = 0 ; i < desc_cnt; i++){
		printk(KERN_INFO "=============== %u-th set of descriptor==============\n", i);
		printk(KERN_INFO "DESC0(0x%08x) = 0x%08x\n", paddr_base,  *(gddr_descriptor + 4*i));
		printk(KERN_INFO"DESC1(0x%08x) = 0x%08x\n", paddr_base + 16*i + 4, *(gddr_descriptor + 4*i + 1));
		printk(KERN_INFO "DESC2(0x%08x) = 0x%08x\n", paddr_base + 16*i + 8, *(gddr_descriptor + 4*i + 2));
		printk(KERN_INFO "DESC3(0x%08x) = 0x%08x\n", paddr_base + 16*i + 12, *(gddr_descriptor + 4*i + 3));
	}
	printk(KERN_INFO "=====================================================\n");	

}



void rtk_op_complete(struct rtkemmc_host *emmc_port)
{
	if (emmc_port->int_waiting) {
        	MMCPRINTF("int wait complete 1\n");
        	MMCPRINTF("rtk_op_complete: rtk wait complete addr = %08x\n", (unsigned int) emmc_port->int_waiting);
        	struct completion *waiting = emmc_port->int_waiting;
        	//emmc_port->int_waiting = NULL;
        	complete(waiting);
        	MMCPRINTF("========== D ==========\n");
        	MMCPRINTF("int wait complete 2\n");
    	}
    	else
        	MMCPRINTF("int wait not complete\n");
}
EXPORT_SYMBOL_GPL(rtk_op_complete);

char *rtkemmc_parse_token(const char *parsed_string, const char *token)
{
	const char *ptr = parsed_string;
    	const char *start, *end, *value_start, *value_end;
    	char *ret_str;

    	while(1) {
        	value_start = value_end = 0;
        	for(;*ptr == ' ' || *ptr == '\t'; ptr++);
        	if(*ptr == '\0')        break;
        	start = ptr;
        	for(;*ptr != ' ' && *ptr != '\t' && *ptr != '=' && *ptr != '\0'; ptr++) ;
        	end = ptr;
        	if(*ptr == '=') {
            		ptr++;
            		if(*ptr == '"') {
                		ptr++;
                		value_start = ptr;
                		for(; *ptr != '"' && *ptr != '\0'; ptr++);
                		if(*ptr != '"' || (*(ptr+1) != '\0' && *(ptr+1) != ' ' && *(ptr+1) != '\t')) {
                			printk("system_parameters error! Check your parameters     .");
                    			break;
                		}
            		} else {
                		value_start = ptr;
                		for(;*ptr != ' ' && *ptr != '\t' && *ptr != '\0' && *ptr != '"'; ptr++) ;
                		if(*ptr == '"') {
                    			printk("system_parameters error! Check your parameters.");
                    			break;
                		}
            		}	
            		value_end = ptr;
        	}

        	if(!strncmp(token, start, end-start)) {
            		if(value_start) {
                		ret_str = kmalloc(value_end-value_start+1, GFP_KERNEL);
                		// KWarning: checked ok by alexkh@realtek.com
                		if(ret_str){
                    			strncpy(ret_str, value_start, value_end-value_start);
                    			ret_str[value_end-value_start] = '\0';
                		}
                		return ret_str;
            		} else {
                		ret_str = kmalloc(1, GFP_KERNEL);
                		// KWarning: checked ok by alexkh@realtek.com
                		if(ret_str)
                    			strcpy(ret_str, "");
                		return ret_str;
            		}
        	}
    	}

    	return (char*)NULL;
}
EXPORT_SYMBOL_GPL(rtkemmc_parse_token);

void rtkemmc_chk_param(u32 *pparam, u32 len, u8 *ptr)
{
    	u32 value,i;
    	mmcrtk("\n");

    	*pparam = 0;
    	for(i=0;i<len;i++){
        	value = ptr[i] - '0';
        	// KWarning: checked ok by alexkh@realtek.com
        	if((value >= 0) && (value <=9)){
            		*pparam+=value<<(4*(len-1-i));
            		continue;
        	}

        	value = ptr[i] - 'a';
        	// KWarning: checked ok by alexkh@realtek.com
        	if((value >= 0) && (value <=5)){
            		value+=10;
            		*pparam+=value<<(4*(len-1-i));
            		continue;
        	}

        	value = ptr[i] - 'A';
        	// KWarning: checked ok by alexkh@realtek.com
        	if((value >= 0) && (value <=5)){
            		value+=10;
            		*pparam+=value<<(4*(len-1-i));
            		continue;
        	}
    	}
}
EXPORT_SYMBOL_GPL(rtkemmc_chk_param);
