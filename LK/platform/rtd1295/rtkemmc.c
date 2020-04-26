/*
 *  This is a driver for the eMMC controller found in Realtek Phoenix 1195
 *  SoCs.
 *
 *  Copyright (C) 2013 Realtek Semiconductors, All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <platform/rbus/crt_sys_reg.h>
#include <platform/rbus/sb2_reg.h>
#include <platform/io.h>
#include <platform/sb2.h>
#include <platform/setting.h>
#include <debug.h>
#include <lib/heap.h>
#include <lib/bio.h>
#include <arch/ops.h>
#include <arch/arm64.h>
#include <arch/arm64/mmu.h>
#include "rtkemmc.h"
#include <lib/console.h>

#define LOCAL_TRACE	0
#ifdef FT_TOOL
static u8 HS200_PHASE_INHERITED = 0;
#else
#ifndef LK_WAS_LOADED_FROM_SPI
static u8 HS200_PHASE_INHERITED = 1;
#else
static u8 HS200_PHASE_INHERITED = 0; // because FSBL was designed for SPI boot
#endif
#endif
static unsigned int savedVP0 = 0xff, savedVP1 = 0xff;

#ifdef DEBUG
#define dbg_printf(...) printf(__VA_ARGS__)
#else
#define dbg_printf(...)
#endif


static struct backupRegs gRegTbl;

unsigned char* ext_csd = NULL;

static bdev_t mmc_dev;
static ssize_t rtkemmc_bdev_read_block(struct bdev *bdev, void *buf, bnum_t block, uint count);
static ssize_t rtkemmc_bdev_write_block(struct bdev *bdev, const void *_buf, bnum_t block, uint count);


static inline void __writel(unsigned int value, volatile unsigned int *addr)
{
	*addr = value;
}

static inline unsigned int __readl(volatile unsigned int *addr)
{
	return *addr;
}

#define wait_done(_a, _m, _v) __wait_done(U64_PTR(_a), _m, _v)

static void __wait_done(volatile unsigned int *addr, unsigned int mask, unsigned int value)
{
	int n = 0;
	while (((*addr) & mask) != value)  {
		if(n++ > 0x1000) {
			printf("TIMEOUT\n");
			printf("%s: addr=0x%08x, mask=0x%08x, value=0x%08x\n",
				__func__, PTR_U32(addr), mask, value);
			break;
		}
		mdelay(1);
		sync();
	}
}

#define EMMC_BLOCK_SIZE        0x200
#define MAX_DESCRIPTOR_NUM    (8)
#define EMMC_MAX_SCRIPT_BLK   (8)
#define EMMC_MAX_MULTI_BLK    (EMMC_MAX_SCRIPT_BLK * MAX_DESCRIPTOR_NUM)

static unsigned int *rw_descriptor;
static unsigned int emmc_rca;
unsigned int emmc_cid[4]={0};
unsigned int rpmb_size=0; //extern for rpmb.c use

struct emmc_error_entry {
	unsigned int error;
	const char *msg;
};

static struct emmc_error_entry emmc_error_table[] =  {
	{.error = CR_EMMC_EBE,  .msg = "End bit error"},
	{.error = CR_EMMC_SBE,  .msg = "Start bit error"},
	{.error = CR_EMMC_HLE,  .msg = "Hardware locked write error"},
	{.error = CR_EMMC_FRUN, .msg = "FIFO underrun/overrun error"},
	{.error = CR_EMMC_HTO,  .msg = "Data starvation by host timeout"},
	{.error = CR_EMMC_DRTO, .msg = "Data read timeout"},
	{.error = CR_EMMC_RTO,  .msg = "Response timeout"},
	{.error = CR_EMMC_DCRC, .msg = "Data CRC error"},
	{.error = CR_EMMC_RCRC, .msg = "Response CRC error"},
	{.error = CR_EMMC_RE,   .msg = "Response error"},
	{.error = 0, .msg = NULL}
};

//static unsigned int check_error(void)
unsigned int check_error(void)
{
	unsigned int error;
	struct emmc_error_entry *p;

	error = cr_readl(CR_EMMC_RINTSTS);
	ISB;
	sync();

	dbg_printf("EMMC_RINTSTS = 0x%08x\n", error);
	dbg_printf("EMMC_STATUS  = 0x%08x\n", cr_readl(CR_EMMC_STATUS));

	for (p = emmc_error_table; p->error != 0; p++) {
		if ((error & p->error) == p->error) {
			printf("eMMC Error: %s\n", p->msg);
			return p->error;
		}
	}
	return 0;
}

static void reset_ip(void)
{
    cr_writel(0x00000081, CR_EMMC_BMOD); //reset
    ISB;
    sync();
    cr_writel(0x02000000, CR_EMMC_CTRL);
    ISB;
    sync();
    cr_writel(0x00000001, CR_EMMC_PWREN);
    ISB;
    sync();
    cr_writel(0x0000ffff, CR_EMMC_INTMASK);
    ISB;
    sync();
    cr_writel(0xffffffff, CR_EMMC_RINTSTS);
    ISB;
    sync();
    cr_writel(0x00000080, CR_EMMC_CLKDIV);
    ISB;
    sync();
    cr_writel(0x00000000, CR_EMMC_CLKSRC);
    ISB;
    sync();
    cr_writel(0x00010001, CR_EMMC_CLKENA);
    ISB;
    sync();
    cr_writel(0xa0200000, CR_EMMC_CMD);
    ISB;
    sync();
    cr_writel(0xffffff40, CR_EMMC_TMOUT);
    ISB;
    sync();
    cr_writel(0x00000000, CR_EMMC_CTYPE);
    ISB;
    sync();
    cr_writel(0x0001007f, CR_EMMC_FIFOTH);
    ISB;
    sync();
    cr_writel(0x02000010, CR_EMMC_CTRL);
    ISB;
    sync();
    cr_writel(0x00000080, CR_EMMC_BMOD);
    ISB;
    sync();
    cr_writel(0x0000ffcf, CR_EMMC_INTMASK);
    ISB;
    sync();
    cr_writel(0x00000000, CR_EMMC_IDINTEN);
    ISB;
    sync();
    cr_writel(0x0000ffff, CR_EMMC_RINTSTS);
    ISB;
    sync();
    cr_writel(0x00000001, CR_EMMC_UHSREG);
	ISB;
    sync();
	cr_writel(0x02000001, CR_EMMC_CARD_THR_CTL);
	ISB;
	sync();
}		


// rsp_con -  0: no rsp, 1: short rsp, 3: long rsp
//static unsigned int emmc_send_cmd_get_rsp(unsigned int cmd_idx, unsigned int sd_arg, unsigned int rsp_con, unsigned int crc)
unsigned int emmc_send_cmd_get_rsp(unsigned int cmd_idx, unsigned int sd_arg, unsigned int rsp_con, unsigned int crc)
{ 
	unsigned int ret_error = 0;
	
	/* clear all interrupt status */
	cr_writel(0x0000ffff, CR_EMMC_RINTSTS);
	ISB;
	sync();

	/* enable all interrupts */
	cr_writel(0x0000ffff, CR_EMMC_INTMASK);	
	ISB;
	sync();

	/* cmd argument */
	cr_writel(sd_arg, CR_EMMC_CMDARG);
	ISB;
	sync();

	dbg_printf("%s\n", __func__);
	dbg_printf("  cmd_idx = 0x%08x\n", cmd_idx);
	dbg_printf("  sd_arg  = 0x%08x\n", sd_arg);
	dbg_printf("  rsp_con = %d\n", rsp_con);
	dbg_printf("  crc     = %d\n", crc);

	cr_writel(0xA0000000 | cmd_idx | (rsp_con << 6) | (crc  << 8), 
	    CR_EMMC_CMD);
	ISB;
	sync();

	wait_done(CR_EMMC_RINTSTS, 0x4, 0x4); 	
	wait_done(CR_EMMC_STATUS, 0x200, 0x0); 	
	ISB;
	sync();

	dbg_printf("EMMC_RESP0 = 0x%08x\n", cr_readl(CR_EMMC_RESP0));
	dbg_printf("EMMC_RESP1 = 0x%08x\n", cr_readl(CR_EMMC_RESP1));
	dbg_printf("EMMC_RESP2 = 0x%08x\n", cr_readl(CR_EMMC_RESP2));
	dbg_printf("EMMC_RESP3 = 0x%08x\n", cr_readl(CR_EMMC_RESP3));

	if(cmd_idx ==MMC_SEND_CID ){
		emmc_cid[3] = SWAP_32(cr_readl(CR_EMMC_RESP0));
		emmc_cid[2] = SWAP_32(cr_readl(CR_EMMC_RESP1));
		emmc_cid[1] = SWAP_32(cr_readl(CR_EMMC_RESP2));
		emmc_cid[0] = SWAP_32(cr_readl(CR_EMMC_RESP3));
	}


	ret_error = check_error();
	
	return ret_error;
}	


int switch_speed(unsigned int speed){
  
    //speed - 0, backward compatible timing, 1: High speed, 2: HS200
    emmc_send_cmd_get_rsp(MMC_SWITCH, 0x03b90000|(speed<<8), 1, 1);  
    emmc_send_cmd_get_rsp(MMC_SEND_STATUS, emmc_rca, 1, 1);
    ISB;
    if ((cr_readl(CR_EMMC_RESP0)&0x80)==0){
        printf("switch speed to 0x%0x success \n", speed);
		return 0;
    }   
    else{
       	printf("switch speed to 0x%0x fail \n", speed);
		return -1;
    }   
}

int switch_erase_group_def(unsigned int enable){
  
    //enable 1: Use high-capacity erase unit size
    emmc_send_cmd_get_rsp(MMC_SWITCH, 0x03af0000|(enable<<8), 1, 1);  
    emmc_send_cmd_get_rsp(MMC_SEND_STATUS, emmc_rca, 1, 1);
    ISB;
    if ((cr_readl(CR_EMMC_RESP0)&0x80)==0){
        printf("switch erase_group_def to 0x%0x success \n", enable);
		return 0;
    }   
    else{
       	printf("switch erase_group_def to 0x%0x fail \n", enable);
		return -1;
    }   
}




int frequency(unsigned int  fre, unsigned int  div){
#ifndef FPGA
	cr_writel((cr_readl(SYS_PLL_EMMC3)&0xffff) | (fre << 16), SYS_PLL_EMMC3);
	ISB;
	// ECO, if EMMC N/F code changed, toggle CR_EMMC_DUMMY_SYS bit 30
	cr_writel((~cr_readl(CR_EMMC_DUMMY_SYS)) & 0x40000000, CR_EMMC_DUMMY_SYS);
	ISB;
	sync();
#endif
	wait_done((unsigned int*)CR_EMMC_STATUS, 0x200, 0x0); 	 //card is not busy
	ISB;
	sync();
	
	//disable clock
	cr_writel(0, CR_EMMC_CLKENA);
	ISB;
	sync();

	//EMMC Cmd
	cr_writel(0xa0202000, CR_EMMC_CMD);
	ISB;
	sync();
	wait_done((unsigned int*)CR_EMMC_CMD, 0x80000000, 0x0);
	ISB;
	sync();

	//set divider
	cr_writel(div, CR_EMMC_CLKDIV);
	ISB;
	sync();

	//EMMC Cmd
	cr_writel(0xa0202000, CR_EMMC_CMD);
	ISB;
	sync();
	wait_done((unsigned int*)CR_EMMC_CMD, 0x80000000, 0x0);
	ISB;
	sync();
	
	//enable clock
	cr_writel(0x10001, CR_EMMC_CLKENA);
	ISB;
	sync();

	//EMMC Cmd
	cr_writel(0xa0202000, CR_EMMC_CMD);
	ISB;
	sync();
	wait_done((unsigned int*)CR_EMMC_CMD, 0x80000000, 0x0);
	ISB;
	sync();
	
	printf("switch frequency to 0x%x, divder to 0x%x\n", fre, div);
	return 0;	
}


static void set_wrapper_div(unsigned int level)
{
	switch(level)
	{
		case 0:
			writel(0x2100, CR_EMMC_CKGEN_CTL);
			break;
		case 1:
			writel(0x2010, CR_EMMC_CKGEN_CTL);
			break;
		case 2:
			writel(0x2012, CR_EMMC_CKGEN_CTL);
			break;
		case 3:
			writel(0x2013, CR_EMMC_CKGEN_CTL);
			break;
		default :
			printf("[ERR] %s: level = 0x%02x\n", __func__, level);
			break;
	}
	ISB;
	sync();
}



static int switch_bus(unsigned int width)
{
	printf("%s: width = 0x%08x\n", __func__, width);

	emmc_send_cmd_get_rsp(MMC_SWITCH, 0x03b70000 | (width << 8), 1, 1);
	emmc_send_cmd_get_rsp(MMC_SEND_STATUS, emmc_rca, 1, 1);
	ISB;
	sync();
	if ((cr_readl(CR_EMMC_RESP0) & 0x80) == 0) {
		/*
		 * CR_EMMC_CTYPE:
		 *  0x00000000: 1-bit
		 *  0x00000001: 4-bit
		 *  0x00010000: 8-bit
		 *
		 * CR_EMMC_UHSREG:
		 *  0x00000001: non-DDR mode, SW 1.8V
		 *  0x00010001: DDR mode, SW 1.8V
		 */
		switch(width) {
			case 0:
				cr_writel(0x00000000, CR_EMMC_CTYPE);
				cr_writel(0x00000001, CR_EMMC_UHSREG);
				break;
			case 1:
				cr_writel(0x00000001, CR_EMMC_CTYPE);
				cr_writel(0x00000001, CR_EMMC_UHSREG);
				break;
			case 2:
				cr_writel(0x00010000, CR_EMMC_CTYPE);
				cr_writel(0x00000001, CR_EMMC_UHSREG);
				break;
			case 5:
				cr_writel(0x00000001, CR_EMMC_CTYPE);
				cr_writel(0x00010001, CR_EMMC_UHSREG);
				break;
			case 6:
				cr_writel(0x00010000, CR_EMMC_CTYPE);
				cr_writel(0x00010001, CR_EMMC_UHSREG);
				break;
			default:
				cr_writel(0x00010000, CR_EMMC_CTYPE);
				cr_writel(0x00000001, CR_EMMC_UHSREG);
				break;
		}
		ISB;
		sync();
		return 0;
	}

	return -1;
}

static void pad_driving(unsigned int clk_drv, unsigned int cmd_drv, unsigned int data_drv, unsigned int ds_drv)
{
	cr_writel(data_drv|(data_drv<<8)|(data_drv<<16)|(data_drv<<24), CR_EMMC_PDRIVE_NF1); //d0~d3	
	cr_writel(data_drv|(data_drv<<8)|(data_drv<<16)|(data_drv<<24), CR_EMMC_PDRIVE_NF2); //d4~d7	
	cr_writel((cr_readl(CR_EMMC_PDRIVE_NF3)&(0x00ff00ff))|(clk_drv<<8)|(cmd_drv<<24), CR_EMMC_PDRIVE_NF3); //d4~d7	
	cr_writel(ds_drv, CR_EMMC_PDRIVE_NF4); //data strobe
	ISB;
    sync();
    return;
}

static void phase(unsigned int VP0, unsigned int VP1){
    //phase selection
    if( (VP0==0xff) && (VP1==0xff)){
        //printf("phase VP0 and VP1 no change \n");
    }
	else if( (VP0!=0xff) && (VP1==0xff)){
        //change clock to 4MHz
        cr_writel(cr_readl(CR_EMMC_CKGEN_CTL)|0x70000, CR_EMMC_CKGEN_CTL);
        sync();
        //reset pll
        cr_writel(cr_readl(SYS_PLL_EMMC1)&0xfffffffd, SYS_PLL_EMMC1);
        sync();
        //vp0 phase:0x0~0x1f
        cr_writel((cr_readl(SYS_PLL_EMMC1)&0xffffff07)|(VP0<<3), SYS_PLL_EMMC1);
        sync();
        //release reset pll
        cr_writel(cr_readl(SYS_PLL_EMMC1)|0x2, SYS_PLL_EMMC1);
        sync();
        //change clock to PLL
        cr_writel(cr_readl(CR_EMMC_CKGEN_CTL)&0xfff8ffff, CR_EMMC_CKGEN_CTL);
        sync();
	}
	else if( (VP0==0xff) && (VP1!=0xff)){
        //change clock to 4MHz
        cr_writel(cr_readl(CR_EMMC_CKGEN_CTL)|0x70000, CR_EMMC_CKGEN_CTL);
        sync();
        //reset pll
        cr_writel(cr_readl(SYS_PLL_EMMC1)&0xfffffffd, SYS_PLL_EMMC1);
        sync();
        //vp1 phase:0x0~0x1f
        cr_writel((cr_readl(SYS_PLL_EMMC1)&0xffffe0ff)|(VP1<<8), SYS_PLL_EMMC1);
        sync();
        //release reset pll
        cr_writel(cr_readl(SYS_PLL_EMMC1)|0x2, SYS_PLL_EMMC1);
        sync();
        //change clock to PLL
        cr_writel(cr_readl(CR_EMMC_CKGEN_CTL)&0xfff8ffff, CR_EMMC_CKGEN_CTL);
        sync();
	}
	else{
        //change clock to 4MHz		
        cr_writel(cr_readl(CR_EMMC_CKGEN_CTL)|0x70000, CR_EMMC_CKGEN_CTL);
        sync();
        //reset pll
        cr_writel(cr_readl(SYS_PLL_EMMC1)&0xfffffffd, SYS_PLL_EMMC1);
        sync();
        //vp0 phase:0x0~0x1f
        cr_writel((cr_readl(SYS_PLL_EMMC1)&0xffffff07)|(VP0<<3), SYS_PLL_EMMC1);
        sync();
        //vp1 phase:0x0~0x1f
        cr_writel((cr_readl(SYS_PLL_EMMC1)&0xffffe0ff)|(VP1<<8), SYS_PLL_EMMC1);
        sync();
        //release reset pll
        cr_writel(cr_readl(SYS_PLL_EMMC1)|0x2, SYS_PLL_EMMC1);
        sync();
        //change clock to PLL
        cr_writel(cr_readl(CR_EMMC_CKGEN_CTL)&0xfff8ffff, CR_EMMC_CKGEN_CTL);
        sync();
	}
    udelay(100);
	ISB;
    sync();
}

static void backup_registers(void)
{
	gRegTbl.emmc_mux_pad0 = cr_readl(CR_EMMC_muxpad0);
   	gRegTbl.emmc_mux_pad1 = cr_readl(CR_EMMC_muxpad1);
	gRegTbl.emmc_pfunc_nf1 = cr_readl(CR_EMMC_PFUNC_NF1);
	gRegTbl.emmc_pfunc_cr = cr_readl(CR_EMMC_PFUNC_CR);
    gRegTbl.emmc_pdrive_nf1 = cr_readl(CR_EMMC_PDRIVE_NF1);
    gRegTbl.emmc_pdrive_nf2 = cr_readl(CR_EMMC_PDRIVE_NF2);
    gRegTbl.emmc_pdrive_nf3 = cr_readl(CR_EMMC_PDRIVE_NF3);
    gRegTbl.emmc_ctype = cr_readl(CR_EMMC_CTYPE);
    gRegTbl.emmc_uhsreg = cr_readl(CR_EMMC_UHSREG);
	gRegTbl.emmc_ddr_reg = cr_readl(CR_EMMC_DDR_REG);
    gRegTbl.emmc_card_thr_ctl = cr_readl(CR_EMMC_CARD_THR_CTL);
    gRegTbl.emmc_clk_div = cr_readl(CR_EMMC_CLKDIV);
    gRegTbl.emmc_ckgen_ctl = cr_readl(CR_EMMC_CKGEN_CTL);
	gRegTbl.emmc_dqs_ctrl1= cr_readl(CR_EMMC_DQS_CTRL1);
}


static void card_stop(void){
    volatile unsigned int reg;
    backup_registers();

    //CRT reset eMMC
    reg = cr_readl(SYS_SOFT_RESET2);
	ISB;
    sync();
    cr_writel(reg&0xfffff7ff, SYS_SOFT_RESET2);
	ISB;
    sync();
	
    //CRT release eMMC reset
    reg = cr_readl(SYS_SOFT_RESET2);
	ISB;
    sync();
    cr_writel(reg|0x00000800, SYS_SOFT_RESET2);
	ISB;
    sync();

	// ECO, if EMMC N/F code changed, toggle CR_EMMC_DUMMY_SYS bit 30
	cr_writel(cr_readl(CR_EMMC_DUMMY_SYS) ^ 0x40000000, CR_EMMC_DUMMY_SYS);
	ISB;
	sync();

	//restore cr emmc registers
	cr_writel(gRegTbl.emmc_mux_pad0, CR_EMMC_muxpad0);
	cr_writel(gRegTbl.emmc_mux_pad1, CR_EMMC_muxpad1);
	cr_writel(gRegTbl.emmc_pfunc_nf1, CR_EMMC_PFUNC_NF1);
	cr_writel(gRegTbl.emmc_pfunc_cr, CR_EMMC_PFUNC_CR);
    cr_writel(gRegTbl.emmc_pdrive_nf1, CR_EMMC_PDRIVE_NF1);
    cr_writel(gRegTbl.emmc_pdrive_nf2, CR_EMMC_PDRIVE_NF2);
    cr_writel(gRegTbl.emmc_pdrive_nf3, CR_EMMC_PDRIVE_NF3);
    cr_writel(gRegTbl.emmc_ckgen_ctl, CR_EMMC_CKGEN_CTL);
	ISB;
    sync();

    reset_ip();

    cr_writel(gRegTbl.emmc_ctype, CR_EMMC_CTYPE);
    cr_writel(gRegTbl.emmc_uhsreg, CR_EMMC_UHSREG);
	cr_writel(gRegTbl.emmc_ddr_reg, CR_EMMC_DDR_REG);
    cr_writel(gRegTbl.emmc_card_thr_ctl, CR_EMMC_CARD_THR_CTL);
	cr_writel(gRegTbl.emmc_dqs_ctrl1, CR_EMMC_DQS_CTRL1);
	cr_writel(0, CR_EMMC_PAD_CTL); //PAD to 1.8v
    sync();

    cr_writel(0, CR_EMMC_CLKENA); // 0x10, clk enable, disable clock
    sync();

    cr_writel(0xa0202000, CR_EMMC_CMD); // 0x10, clk enable, disable clock
    sync();

    // 0x2c, wait for CIU to take the command
    wait_done((unsigned int*)CR_EMMC_CMD, 0x80000000,0);
    cr_writel(gRegTbl.emmc_clk_div, CR_EMMC_CLKDIV);
    sync();

    cr_writel(0xa0202000, CR_EMMC_CMD);  // 0x2c = start_cmd, upd_clk_reg_only, wait_prvdata_complete
    sync();

    // 0x2c, wait for CIU to take the command
    wait_done((unsigned int*)CR_EMMC_CMD, 0x80000000,0);

    cr_writel(0x10001, CR_EMMC_CLKENA); // 0x10, clk enable, disable clock
    sync();

    cr_writel(0xa0202000, CR_EMMC_CMD);  // 0x2c = start_cmd, upd_clk_reg_only, wait_prvdata_complete
    sync();

    // 0x2c, wait for CIU to take the command
    wait_done((unsigned int*)CR_EMMC_CMD, 0x80000000,0);
}

static int polling_to_tran_state(void)
{
	int err=1, retry_cnt=3;
	int ret_state=0;

    while(retry_cnt-- && err)
    {
    	err = emmc_send_cmd_get_rsp(MMC_SEND_STATUS, emmc_rca, 1, 1);
		ret_state = R1_CURRENT_STATE(cr_readl(CR_EMMC_RESP0));
    }
	retry_cnt=10;
    if ((err)||(ret_state != STATE_TRAN))
    {
		//printf("Fail to polling transfer state \n ");
    }
	ISB;
	sync();
	return err;
}


static int __rtkemmc_init(void)
{
	int ret_err, n = 0;

	cr_writel(0x00000081, CR_EMMC_BMOD);
	ISB;
	sync();
	cr_writel(0x02000000, CR_EMMC_CTRL);
	ISB;
	sync();
	cr_writel(0x00000001, CR_EMMC_PWREN);
	ISB;
	sync();
	cr_writel(0x0000ffff, CR_EMMC_INTMASK);
	ISB;
	sync();
	cr_writel(0xffffffff, CR_EMMC_RINTSTS);
	ISB;
	sync();
	cr_writel(0x00000080, CR_EMMC_CLKDIV);
	ISB;
	sync();
	cr_writel(0x00000000, CR_EMMC_CLKSRC);
	ISB;
	sync();
	cr_writel(0x0001ffff, CR_EMMC_CLKENA);
	ISB;
	sync();
	cr_writel(0xa0200000, CR_EMMC_CMD);
	ISB;
	sync();
	cr_writel(0xffffff40, CR_EMMC_TMOUT);
	ISB;
	sync();
	cr_writel(0x00000000, CR_EMMC_CTYPE);
	ISB;
	sync();
	cr_writel(0x007f007f, CR_EMMC_FIFOTH);
	ISB;
	sync();
	cr_writel(0x02000010, CR_EMMC_CTRL);
	ISB;
	sync();
	cr_writel(0x00000080, CR_EMMC_BMOD);
	ISB;
	sync();
	cr_writel(0x0000ffcf, CR_EMMC_INTMASK);
	ISB;
	sync();
	cr_writel(0x00000000, CR_EMMC_IDINTEN);
	ISB;
	sync();
	cr_writel(0x0000ffff, CR_EMMC_RINTSTS);
	ISB;
	sync();
	cr_writel(0x00000001, CR_EMMC_UHSREG);
	ISB;
	sync();
	cr_writel(0x02000001, CR_EMMC_CARD_THR_CTL);
	ISB;
	sync();

	//Card identification
    frequency(0x46, 0x80); //divider = 2 * 128 = 256    

	emmc_send_cmd_get_rsp(MMC_GO_IDLE_STATE, 0, 0, 0); 
	emmc_send_cmd_get_rsp(MMC_GO_IDLE_STATE, 0, 0, 0); 
	emmc_send_cmd_get_rsp(MMC_GO_IDLE_STATE, 0, 0, 0); 
	emmc_send_cmd_get_rsp(MMC_GO_IDLE_STATE, 0, 0, 0); 
	emmc_send_cmd_get_rsp(MMC_GO_IDLE_STATE, 0, 0, 0); 
	ISB;
	sync();
	
	emmc_send_cmd_get_rsp(MMC_SEND_OP_COND, 0x40000080, 1, 0); 
	ISB;
	sync();

	while ((cr_readl(CR_EMMC_RESP0) & 0x80000000) != 0x80000000) {
		if(n++ > 0x100)
		{
			printf("CMD1 ERROR! \n");
			return ret_err;      
	 	}   
		ret_err = emmc_send_cmd_get_rsp(MMC_SEND_OP_COND, 0x40000080, 1, 0); 		
		ISB;
		sync();
	}
	emmc_send_cmd_get_rsp(MMC_ALL_SEND_CID, 0, 3, 1); 
	ISB;
	sync();
	emmc_send_cmd_get_rsp(MMC_SET_RELATIVE_ADDR, emmc_rca, 1, 1); 
	ISB;
	sync();

	//Data Transfer Mode @ legacy mode: 10M
    frequency(0x46, 0x4);//divider = 2 * 4 = 8  

	emmc_send_cmd_get_rsp(MMC_SEND_CSD, emmc_rca, 3, 1); 
	ISB;
	sync();
	emmc_send_cmd_get_rsp(MMC_SEND_CID, emmc_rca, 3, 1); 
	ISB;
	sync();
	emmc_send_cmd_get_rsp(MMC_SELECT_CARD, emmc_rca, 1, 1); 
	ISB;
	sync();
	emmc_send_cmd_get_rsp(MMC_SEND_STATUS, emmc_rca, 1, 1); 
	ISB;
	sync();

	/* switch to SDR 8 bit */
	switch_bus(0x2);

	emmc_read_write_ip(MMC_SEND_EXT_CSD, 0, ext_csd, 512);
	arch_invalidate_cache_range(PTR_U32(ext_csd), 512);

	if ((ext_csd[196] & 0x10) == 0){
		HS200_PHASE_INHERITED = 0;
	}

	rpmb_size = ext_csd[168]; //for rpmb.c use

	if (HS200_PHASE_INHERITED == 1){
		if ( switch_speed(2) == 0 ){
			//Data Transfer Mode @ HS200 mode: 200M
			cr_writel(0x2100, CR_EMMC_CKGEN_CTL);
			frequency(0xa6, 0);//divider = 1
			pad_driving(0xbb, 0xbb, 0xbb, 0x33);
			phase(savedVP0, savedVP1);
			printf("PLL_EMMC1 = 0x%08x\n", cr_readl(SYS_PLL_EMMC1));
			card_stop();
			polling_to_tran_state();
		}
	}
	else{
		//switch to high speed
		if ( switch_speed(1) == 0 ){
			//Data Transfer Mode @ HS mode: 50M
			frequency(0x57, 1);//divider = 2 * 1 = 2  
#ifdef FT_TOOL
			pad_driving(0x77, 0x77, 0x77, 0x77);
#else
			pad_driving(0x77, 0x77, 0x77, 0x33);
#endif
		}
	}

	return 0;
}


int rtk_emmc_init(void)
{
	int ret;
	int retry = 0;
	uint32_t sec_count = 0;
#ifdef LK_WAS_LOADED_FROM_SPI
	uint32_t reg_val = 0;
#endif

	arch_clean_invalidate_cache_range(MEMBASE, MEMSIZE);

	ext_csd = memalign(16, 512);
	memset(ext_csd, 0 ,512);
	rw_descriptor = memalign(8, EMMC_MAX_MULTI_BLK * sizeof(unsigned int) * 4);
	emmc_rca = 1 << 16;

	savedVP0 = (cr_readl(SYS_PLL_EMMC1) & 0x000000f8) >> 3;
	savedVP1 = (cr_readl(SYS_PLL_EMMC1) & 0x00001f00) >> 8;
#ifdef FT_TOOL
	cr_writel(0x1003, SYS_PLL_EMMC1);
#else
	cr_writel(0x03, SYS_PLL_EMMC1);
#endif
	cr_writel(0, CR_EMMC_PAD_CTL); //1.8V
	mdelay(300);

	printf("PLL_EMMC1 = 0x%08x\n", cr_readl(SYS_PLL_EMMC1));

#ifdef LK_WAS_LOADED_FROM_SPI
    printf(VT100_YELLOW"update pin mux for eMMC\n"VT100_NONE);
	//set emmc pin mux
    reg_val = cr_readl(CR_EMMC_muxpad0);
    reg_val &= ~0xFFFF0C3C;
    reg_val |= 0xaaaa0824;
    cr_writel(reg_val, CR_EMMC_muxpad0);
#endif

	for (retry = 0; retry < 3; retry ++) {
		ret = __rtkemmc_init();
		if (ret) {
			printf("emm init fail, retry = %d\n", retry);
			continue;
		}

		break;
	}

	switch_erase_group_def(1);
	sec_count = (ext_csd[215] << 24) | (ext_csd[214] << 16) | (ext_csd[213] << 8) | ext_csd[212];
	printf("sec_count = 0x%x \n", sec_count);
	
	/* START OF LK Block-Device interface */
	#define TEMP_EMMC_4G_SIZE	(0x200000000) // Temporary change to 8G to fit in 8G emmc
	bio_initialize_bdev(&mmc_dev, BIO_MMC_NAME, EMMC_BLOCK_SIZE, sec_count,
				0, NULL, BIO_FLAGS_NONE);
	mmc_dev.read_block = rtkemmc_bdev_read_block;
	mmc_dev.write_block = rtkemmc_bdev_write_block;
	bio_register_device(&mmc_dev);

	
	//hexdump(ext_csd, 512);
	//printf("EXT_CSD[HC_ERASE_GRP_SIZE] = %u \n", ext_csd[EXT_CSD_ERASE_GROUP_DEF]);
	printf("Erase Unit Size = 512KB * 0x%x \n", ext_csd[EXT_CSD_HC_ERASE_GRP_SIZE]);
	//printf("[sj] ext csd 179:0x%x \n", ext_csd[179]);

	/*int index=0;
	for(;index < 512;index ++)
		printf("ext csd[%d] is %02x\n",index,ext_csd[index]);
	printf("\n");*/

	return ret;
}

static void make_ip_des(const unsigned char *dma_addr, unsigned int dma_length)
//void make_ip_des(const unsigned char *dma_addr, unsigned int dma_length)

{
	unsigned int *des_base = rw_descriptor;
	unsigned int blk_cnt;                                                                                                                               
	unsigned int remain_blk_cnt;

	blk_cnt        = dma_length >> 9;
  	remain_blk_cnt = blk_cnt;
	ISB;
	sync();

	arch_clean_cache_range(PTR_U32(rw_descriptor), 4 * sizeof(unsigned int) * MAX_DESCRIPTOR_NUM);

	while(remain_blk_cnt) {
		unsigned int para;
		unsigned int cur_blk_cnt;

		/* setting des0; transfer parameter  */
		para = 0x80000000 | 0x2 | 0x10;
		para |= remain_blk_cnt == blk_cnt ? 0x8 : 0;
		para |= remain_blk_cnt <= EMMC_MAX_SCRIPT_BLK ? 0x4 : 0;
		des_base[0] = para;		

		/* setting des1; buffer size in byte */                         
		cur_blk_cnt = remain_blk_cnt > EMMC_MAX_SCRIPT_BLK ? EMMC_MAX_SCRIPT_BLK : remain_blk_cnt;
		des_base[1] = (cur_blk_cnt << 9);                                                   

		/* setting des2; Physical address to DMA to/from */                             
		des_base[2] = PTR_U32(dma_addr);

		/* setting des3; next descrpter entry */                                        
		des_base[3] = PTR_U32(des_base + 4);

		dma_addr       += (cur_blk_cnt << 9);
		remain_blk_cnt -= cur_blk_cnt; 
		des_base       += 4;

		ISB;
		sync();
	}       

	arch_clean_cache_range(PTR_U32(rw_descriptor), 4 * sizeof(unsigned int)*  MAX_DESCRIPTOR_NUM);
} 

//static int emmc_read_write_ip(unsigned int cmd_idx, unsigned int blk_addr, 
	//const unsigned char *dma_addr, unsigned int dma_length)
	
int emmc_read_write_ip(unsigned int cmd_idx, unsigned int blk_addr, 
	const unsigned char *dma_addr, unsigned int dma_length)
{
	//printf("%s: cmd_idx = %d, blk_addr = %08x\n", __func__, cmd_idx, blk_addr);

	cr_writel(0, CR_EMMC_SWC_SEL);
	cr_writel(0, CR_EMMC_CP);	
	ISB;
	sync();

	make_ip_des(dma_addr, dma_length);
	arch_clean_cache_range(PTR_U32(dma_addr), dma_length);

	cr_writel(dma_length, CR_EMMC_BYTCNT);
	cr_writel(PTR_U32(rw_descriptor), CR_EMMC_DBADDR);
	cr_writel(0x0000ffff, CR_EMMC_RINTSTS);
	cr_writel(0x0000ffff, CR_EMMC_INTMASK);
	cr_writel(blk_addr, CR_EMMC_CMDARG);		
	ISB;
	sync();

	/* -- eMMC CMD --
	 *  bit: name
	 *   6 : response_expect
	 *   8 : check_response_crc
	 *   9 : data_expected
	 *  10 : read/write (0 for read, 1 for write)
	 *  12 : send_auto_stop
	 */
	if (cmd_idx == MMC_READ_SINGLE_BLOCK || cmd_idx == MMC_SEND_EXT_CSD)
		cr_writel(0xa0000340 | cmd_idx, CR_EMMC_CMD);
	else if (cmd_idx == MMC_READ_MULTIPLE_BLOCK)
		cr_writel(0xa0001340 | cmd_idx, CR_EMMC_CMD);
	else if (cmd_idx == MMC_WRITE_BLOCK)
		cr_writel(0xa0000740 | cmd_idx, CR_EMMC_CMD);		         
	else if (cmd_idx ==MMC_WRITE_MULTIPLE_BLOCK)
		cr_writel(0xa0001740 | cmd_idx, CR_EMMC_CMD);	
	else
		return -1;
	ISB;
	sync();

	wait_done(CR_EMMC_RINTSTS, 0x4, 0x4); //command done
	wait_done(CR_EMMC_RINTSTS, 0x8, 0x8); //data transfer over
	ISB;
	sync();

	if (cmd_idx == MMC_READ_MULTIPLE_BLOCK || cmd_idx == MMC_WRITE_MULTIPLE_BLOCK) {
		wait_done(CR_EMMC_RINTSTS, 0x4000, 0x4000); //auto command done (stop command done)
	}

	//read: polling dma done
	if (cmd_idx == MMC_READ_SINGLE_BLOCK || cmd_idx == MMC_READ_MULTIPLE_BLOCK 
		|| cmd_idx == MMC_SEND_EXT_CSD) {
	    wait_done(CR_EMMC_ISR, 0x2, 0x2);
	    cr_writel(0x2, CR_EMMC_ISR);
	    ISB;
	    sync();
	}

	//printf("EMMC_RESP0 = 0x%08x\n", cr_readl(CR_EMMC_RESP0));
	//printf("EMMC_RESP1 = 0x%08x\n", cr_readl(CR_EMMC_RESP1));


	wait_done(CR_EMMC_STATUS, 0x200, 0x0);
	ISB;
	sync();

	return check_error();
}

unsigned int rtk_emmc_write(unsigned int blk_addr, unsigned int blk_cnt, const unsigned char *buffer)
{
	int ret_err = 0;
	unsigned int total_blk_cnt;
	unsigned int cur_blk_addr;
	unsigned int remain_blk_cnt;

	total_blk_cnt = blk_cnt;
	remain_blk_cnt = total_blk_cnt;
	cur_blk_addr   = blk_addr;

	ISB;
	sync();

	while (remain_blk_cnt) {
		unsigned int cur_blk_cnt  = remain_blk_cnt > EMMC_MAX_MULTI_BLK ?
			EMMC_MAX_MULTI_BLK : remain_blk_cnt;
		unsigned int cur_blk_size = cur_blk_cnt << 9;

		arch_clean_cache_range(PTR_U32(buffer), cur_blk_size);

		ret_err = emmc_read_write_ip(
			cur_blk_cnt > 1 ? MMC_WRITE_MULTIPLE_BLOCK : MMC_WRITE_BLOCK,
			cur_blk_addr,
			buffer,
			cur_blk_size);
		if (ret_err)
			return 0;

		remain_blk_cnt -= cur_blk_cnt;
		buffer         += cur_blk_size;
		cur_blk_addr   += cur_blk_cnt;
	}

	return total_blk_cnt;
}

unsigned int rtk_emmc_read(unsigned int blk_addr, unsigned int blk_cnt, unsigned char *buffer)
{
	int ret_err = 0;
	unsigned int total_blk_cnt;
	unsigned int cur_blk_addr;
	unsigned int remain_blk_cnt;

	total_blk_cnt = blk_cnt;
	remain_blk_cnt = total_blk_cnt;
	cur_blk_addr   = blk_addr;

	ISB;
	sync();

	while (remain_blk_cnt) {
		unsigned int cur_blk_cnt  = remain_blk_cnt > EMMC_MAX_MULTI_BLK ?
			EMMC_MAX_MULTI_BLK : remain_blk_cnt;
		unsigned int cur_blk_size = cur_blk_cnt << 9;

		arch_invalidate_cache_range(PTR_U32(buffer), cur_blk_size);

		ret_err = emmc_read_write_ip(
			cur_blk_cnt > 1 ? MMC_READ_MULTIPLE_BLOCK : MMC_READ_SINGLE_BLOCK,
			cur_blk_addr,
			buffer,
			cur_blk_size);
		if (ret_err)
			return 0;

		arch_invalidate_cache_range(PTR_U32(buffer), cur_blk_size);

		remain_blk_cnt -= cur_blk_cnt;
		buffer         += cur_blk_size;
		cur_blk_addr   += cur_blk_cnt;
	}

	return total_blk_cnt;
}

static ssize_t rtkemmc_bdev_read_block(struct bdev *bdev, void *buf, bnum_t block, uint count)
{
	return rtk_emmc_read(block, count, buf);
}

static ssize_t rtkemmc_bdev_write_block(struct bdev *bdev, const void *_buf, bnum_t block, uint count)
{
	return rtk_emmc_write(block, count, _buf);
}

void print_rtkemmc_usage(void){
	printf("rtkemmc commands usage:\n");
	printf("rktemmc read dma_addr blk_addr byte_size\n");
	printf("rtkemmc write dma_addr blk_addr byte_size\n");
	printf("rtkemmc erase from(blk addr) to(blk addr)\n");
	printf("(Erase Unit Size = 512KB * 0x%x) \n", ext_csd[EXT_CSD_HC_ERASE_GRP_SIZE]);
}

int do_rtkemmc(int argc, const cmd_args *argv)
{
	unsigned int ret = 0;
	unsigned int blk_addr, byte_size, blk_cnt;
	unsigned int from_addr, to_addr;
	unsigned char* mem_addr;
	if (argc != 5 && argc != 4) {	
		print_rtkemmc_usage();
		return -1;
	}
	//read/write commands
	if (argc == 5){
		mem_addr   = argv[2].p;
		blk_addr   = argv[3].u;
		byte_size  = argv[4].u;
		blk_cnt = byte_size >> 9;
		if( strncmp( argv[1].str, "read", 4 ) == 0 ) {
			ret = rtk_emmc_read( blk_addr, blk_cnt, mem_addr);
			return ((ret!=blk_cnt) ? -1 : 0);
		}
		else if( strncmp( argv[1].str, "write", 5 ) == 0 ) {
			ret = rtk_emmc_write( blk_addr, blk_cnt, mem_addr);
			return ((ret!=blk_cnt) ? -1 : 0);
		}
		else{
			print_rtkemmc_usage();
		}
	}
	//erase command
	else if (argc == 4){
		if( strncmp( argv[1].str, "erase", 5 ) != 0 ) {
			print_rtkemmc_usage();
		}
		from_addr = argv[2].u;
		to_addr   = argv[3].u;
		emmc_send_cmd_get_rsp(MMC_ERASE_GROUP_START, from_addr, 1, 1);
		ISB;
		sync();
		emmc_send_cmd_get_rsp(MMC_ERASE_GROUP_END, to_addr, 1, 1);
		ISB;
		sync();
		emmc_send_cmd_get_rsp(MMC_ERASE, 0, 1, 1);
		ISB;
		sync();
		ret = emmc_send_cmd_get_rsp(MMC_SEND_STATUS, emmc_rca, 1, 1); 
		ISB;
		sync();
		if (ret || (cr_readl(CR_EMMC_RESP0)&0xFDF92000)){
			printf("error 0x%x, requesting status 0x%x\n",
			ret, cr_readl(CR_EMMC_RESP0));
			return -1;
		}
		else{
			return 0 ;
		}
		
	}
	
	return -1;
}


STATIC_COMMAND_START
STATIC_COMMAND("rtkemmc", "rtk emmc commands", (console_cmd)&do_rtkemmc)
STATIC_COMMAND_END(rtkemmc);

