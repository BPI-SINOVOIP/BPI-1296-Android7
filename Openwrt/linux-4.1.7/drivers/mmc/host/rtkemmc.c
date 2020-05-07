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

#include <linux/suspend.h>
#include <linux/sched.h>                
#include <linux/wait.h>                 
#include <linux/slab.h>                 
#include <linux/semaphore.h>            
#include <linux/mmc/card.h>            
#include <linux/mmc/host.h>             
#include <linux/mmc/mmc.h>              
#include <linux/mmc/sd.h>               
#include <linux/workqueue.h>            
#include <linux/completion.h>           
#include "reg_mmc.h"               
#include "reg_iso.h"               
#include "reg_sys.h"               
#include "rtkemmc.h"                 
#include "rtkemmc_ops.h"                
#include "mmc_debug.h"               
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/of_gpio.h>
#include <asm/cacheflush.h>
#include <linux/pm_runtime.h>
#include <soc/realtek/rtd129x_cpu.h>
#include <soc/realtek/rtd129x_lockapi.h>

#include <linux/clk.h>
#include <linux/clkdev.h>   // clk_get

#ifdef CONFIG_REALTEK_PCBMGR
#include <mach/pcbMgr.h>
#ifdef CONFIG_REALTEK_GPIO
#include <mach/venus_gpio.h>
#define EMMC_SHOUTDOWN_PROTECT
#endif
#endif

#define DRIVER_NAME "EMMC"
#define BANNER      "Realtek eMMC Driver"
#define VERSION     "$Id: rtkemmc.c Kylin 2015-11-06 18:00 $"

#define DMA_ALLOC_LENGTH     (0x80000)
#define DESC_ALLOC_LENGTH   (1024*1024)

//#define EMMC_PARAM_TEST
//#define DBG_PORT
#define PHASE_INHERITED

//#define FORCE_DO_HOST_CARD_STOP
#define FORCE_CHECK_CMD_AND_STS

#ifdef PHASE_INHERITED
static u32 VP0_saved = 0xFF, VP1_saved =0xFF;
#endif

#ifdef RTK_EMMC_HEALTH_REPORT
        int health_report_fd;
#endif 

#define DQS_INHERITED
#ifdef DQS_INHERITED
static u32 dqs_saved = 0xff;
#endif

int suspend = 0;
int suspend_VP0=0xff, suspend_VP1=0xff;
int suspend_dqs=0x88;

unsigned int GLOBAL=0;
static u32 pddrive_nf_s3[5] = {0};
unsigned char compare1[DMA_ALLOC_LENGTH];
unsigned char compare2[DMA_ALLOC_LENGTH];
static int mmc_Tuning_HS400(struct rtkemmc_host *emmc_port,u32 mode);
static void rtkemmc_dqs_tuning(struct mmc_host *host);
void set_RTK_initial_flag(int flag);
//int mmc_flush_cache(struct mmc_card *card);

static void down_speed_handling(struct rtkemmc_host *emmc_port);
//void set_RTK_initial_flag(int flag);
int mmc_select_hs200(struct mmc_card *card);
int mmc_select_ddr50(struct mmc_card *card);
int rtkemmc_switch(struct mmc_card *card, u8 acc_mod, u8 index, u8 value, u8 cmd_set);
static void rtkemmc_dump_registers(struct rtkemmc_host *emmc_port);
struct mmc_host * mmc_host_local = NULL;
static u32 rtk_emmc_bus_wid = 0;
static volatile struct backupRegs gRegTbl;
static volatile int g_bResuming;
volatile int g_bTuning;
static int bSendCmd0=0;
volatile unsigned int gCurrentBootMode=MODE_SDR;
volatile unsigned int gPreventRetry=0;
static volatile unsigned int  g_crinit=0;
static struct rw_semaphore cr_rw_sem;
//ip descriptor on ddr
unsigned int* gddr_descriptor=NULL;
static unsigned int* gddr_descriptor_org=NULL;
//internal emmc dma buffer on ddr
static unsigned char* gddr_dma=NULL;
static unsigned char* gddr_dma_org=NULL;

volatile void __iomem  *sb2_debug;   

u32 rtkemmc_global_blksize;
u32 rtkemmc_global_bytecnt;
u32 rtkemmc_global_dbaddr;


static u32 pddrive_nf_s0[5] = {0};
static u32 pddrive_nf_s2[5] = {0};

struct clk * clk_en_emmc;
struct clk * clk_en_emmc_ip;
struct clk * clk_cr;

int cmd_resend=0;
static void rtkemmc_request(struct mmc_host *host, struct mmc_request *mrq);
static int rtkemmc_get_ro(struct mmc_host *mmc);
static void rtkemmc_set_ios(struct mmc_host *host, struct mmc_ios *ios);

static void set_cmd_info(struct mmc_card *card,struct mmc_command * cmd,
struct sd_cmd_pkt * cmd_info,u32 opcode,u32 arg,u8 rsp_para);

static int rtkemmc_stop_transmission(struct mmc_card *card,int bIgnore);
static int rtkemmc_send_status(struct mmc_card *card,u16 * state,u8 divider,int bIgnore);
static int rtkemmc_wait_status(struct mmc_card *card,u8 state,u8 divider,int bIgnore);

static void rtkemmc_set_wrapper_div(struct rtkemmc_host *emmc_port,u8 level);
static int mmc_Tuning_SDR50(struct rtkemmc_host *emmc_port);	
static int mmc_Tuning_DDR50(struct rtkemmc_host *emmc_port);	
static int mmc_Tuning_HS200(struct rtkemmc_host *emmc_port,u32 mode);
static int rtkemmc_execute_tuning(struct mmc_host *host, u32 opcode);
static int rtkemmc_prepare_hs400_tuning(struct mmc_host *host, struct mmc_ios *ios);
void phase(struct rtkemmc_host *emmc_port, u32 VP0, u32 VP1);

int rtkemmc_select_timing(struct mmc_card *card);
void rtkemmc_select_card_type(struct mmc_card *card);
typedef void (*set_gpio_func_t)(u32 gpio_num,u8 dir,u8 level);

static const struct mmc_host_ops rtkemmc_ops = {
    .request        = rtkemmc_request,
    .get_ro         = rtkemmc_get_ro,
    .set_ios        = rtkemmc_set_ios,
    .execute_tuning = rtkemmc_execute_tuning,
    .prepare_hs400_tuning = rtkemmc_prepare_hs400_tuning,
    .dqs_tuning = rtkemmc_dqs_tuning
};

#define UNSTUFF_BITS(resp,start,size)					\
	({								\
		const int __size = size;				\
		const u32 __mask = (__size < 32 ? 1 << __size : 0) - 1;	\
		const int __off = 3 - ((start) / 32);			\
		const int __shft = (start) & 31;			\
		u32 __res;						\
									\
		__res = resp[__off] >> __shft;				\
		if (__size + __shft > 32)				\
			__res |= resp[__off-1] << ((32 - __shft) % 32);	\
		__res & __mask;						\
	})

void rtkemmc_hw_reset(void)
{
	struct mmc_host * mmc = mmc_host_local;
	struct rtkemmc_host *emmc_port;
	u32 reg_val=0;
	emmc_port = mmc_priv(mmc);
	if(emmc_port!=NULL) {
		printk(KERN_ERR "muxpad0=0x%x, before emmc do hw reset...\n",readl(emmc_port->emmc_membase + EMMC_muxpad0));
		reg_val = readl(emmc_port->emmc_membase + EMMC_muxpad0);
		reg_val &= ~0xFFFF0C3C;
		reg_val |= 0xaaaa0828;
		writel(reg_val, emmc_port->emmc_membase+EMMC_muxpad0);
		mdelay(1);
		printk(KERN_ERR "muxpad0=0x%x, emmc do really hw reset...\n",readl(emmc_port->emmc_membase + EMMC_muxpad0));
		writel(0x0, emmc_port->emmc_membase+0x78);
		mdelay(1);
		writel(0x1, emmc_port->emmc_membase+0x78);
	}
}
EXPORT_SYMBOL(rtkemmc_hw_reset);

static void make_ip_des(u32 dma_addr, u32 dma_length, u32 p_des_base, struct rtkemmc_host *emmc_port){

        u32  blk_cnt;
        u32  blk_cnt2;
        u32  remain_blk_cnt;

        u32  tmp_val;

        u32* des_base = gddr_descriptor ;

        __flush_dcache_area(gddr_descriptor, DESC_ALLOC_LENGTH);
        isb();
        sync(emmc_port);

        MMCPRINTF("RTKEMMC: des_base = 0x%08x\n", des_base);
        //blk_cnt must be the multiple of 512(0x200)
        blk_cnt  = dma_length>>9;
        remain_blk_cnt  = blk_cnt;

        isb();
        sync(emmc_port);

        while(remain_blk_cnt){

                /* setting des0; transfer parameter  */
                tmp_val = 0x80000000 | 0x2 | 0x10;


                if(remain_blk_cnt == blk_cnt)
                        tmp_val |= 0x8;

                if(remain_blk_cnt <= EMMC_MAX_SCRIPT_BLK)
                        tmp_val |= 0x4;

                des_base[0] = tmp_val;

                /* setting des1; buffer size in byte */
                if(remain_blk_cnt > EMMC_MAX_SCRIPT_BLK){
                        blk_cnt2 = EMMC_MAX_SCRIPT_BLK;
                }else{
                        blk_cnt2 = remain_blk_cnt;
                }
                des_base[1] = (blk_cnt2<<9);

                /* setting des2; Physical address to DMA to/from */
                des_base[2] = (dma_addr);

                /* setting des3; next descrpter entry */
                des_base[3] = p_des_base + 16;

                isb();
                sync(emmc_port);

                MMCPRINTF("%s - remain cnt : 0x%08x, desc[0]=0x%08x, desc[1]=0x%08x, desc[2]=0x%08x, desc[3]=0x%08x\n", __func__, remain_blk_cnt,des_base[0],des_base[1],des_base[2],des_base[3]);
                dma_addr = dma_addr+(blk_cnt2<<9);
                remain_blk_cnt -= blk_cnt2;
                des_base += 4;
                p_des_base += 16;  // 4 * 4
                isb();
                sync(emmc_port);
        }
        __flush_dcache_area(gddr_descriptor, DESC_ALLOC_LENGTH);
        isb();
        sync(emmc_port);
}

static void make_sg_des(struct sd_cmd_pkt *cmd_info, u32 p_des_base, struct rtkemmc_host *emmc_port){
   
        u32  blk_cnt;
        u32  blk_cnt2;
        u32  remain_blk_cnt;                                                     
        u32  tmp_val;     
        u32* des_base = gddr_descriptor ;
	u32  dir = 0;
    	u32  dma_nents = 0;
    	u32  dma_leng = 0;
    	u32  dma_addr;
	u32  i;
	struct mmc_host *host = cmd_info->emmc_port->mmc;
	struct scatterlist *sg;



	//__flush_dcache_area(gddr_descriptor, DESC_ALLOC_LENGTH);
	//isb();
	//sync(emmc_port);
	
	MMCPRINTF("RTKEMMC: des_base = 0x%08x\n", des_base); 
	
	if(cmd_info->data->flags & MMC_DATA_READ)
        	dir = DMA_FROM_DEVICE;
	else
        	dir = DMA_TO_DEVICE;
   
    	dma_nents = dma_map_sg( mmc_dev(host), cmd_info->data->sg,
                            cmd_info->data->sg_len,  dir);
    	sg = cmd_info->data->sg;

	for(i=0; i<dma_nents; i++,sg++) {
        	dma_leng = sg_dma_len(sg);
        	blk_cnt  = dma_leng>>9; 			/*blk_cnt must be the multiple of 512(0x200)*/
		remain_blk_cnt  = blk_cnt;
		dma_addr = sg_dma_address(sg);

		while(remain_blk_cnt) {
	
			tmp_val = 0x80000000 | 0x2 | 0x10;		/* setting des0; transfer parameter  */
                	if(i==0 && remain_blk_cnt == blk_cnt )
                        	tmp_val |= 0x8;

                	if(i==dma_nents-1 && remain_blk_cnt <= EMMC_MAX_SCRIPT_BLK)
 	                       tmp_val |= 0x4;

        	        des_base[0] = tmp_val;
			 /* setting des1; buffer size in byte */
	                if(remain_blk_cnt > EMMC_MAX_SCRIPT_BLK){
        	                blk_cnt2 = EMMC_MAX_SCRIPT_BLK;
                	}else{
                        	blk_cnt2 = remain_blk_cnt;
                	}

                	des_base[1] = (blk_cnt2<<9);	

                	des_base[2] = (dma_addr); 		/* setting des2; Physical address to DMA to/from */
                	des_base[3] = p_des_base + 16;			/* setting des3; next descrpter entry */

			MMCPRINTF("%s - remain cnt : 0x%08x, desc[0]=0x%08x, desc[1]=0x%08x, desc[2]=0x%08x, desc[3]=0x%08x\n", __func__, remain_blk_cnt,des_base[0],des_base[1],des_base[2],des_base[3]);
			dma_addr = dma_addr+(blk_cnt2<<9);
			remain_blk_cnt -= blk_cnt2;
                	des_base += 4;
			p_des_base += 16;  // 4 * 4
		}
        }
	//__flush_dcache_area(gddr_descriptor, DESC_ALLOC_LENGTH);
	//isb();
	wmb();
	sync(emmc_port);
}

static void rtkemmc_set_pin_mux(struct rtkemmc_host *emmc_port)
{
    u32 reg_val=0;
    
    MMCPRINTF("rtkemmc_set_pin_mux \n");

	//1295 muxpad0 is in card reader
    //set default i/f to emmc      
    reg_val = readl(emmc_port->emmc_membase + EMMC_muxpad0);
	reg_val &= ~0xFFFF0C3C;
    //reg_val |= 0xaaaa0828;	//comment by Jim
	reg_val |= 0xaaaa0824;	//2017/1/17 modified by Jim, change pin mux to NAND flash instead of emmc rstn
	rtkemmc_writel(reg_val, emmc_port->emmc_membase+EMMC_muxpad0);
	sync(emmc_port);
    	rtkemmc_writel((readl(emmc_port->emmc_membase + EMMC_muxpad1)&0xffffcfff)|0x2000, emmc_port->emmc_membase + EMMC_muxpad1); //kylin data strobe pad mux
	sync(emmc_port);
	//emmc :pfunc_nf1
	rtkemmc_writel(0x33333333, emmc_port->emmc_membase+EMMC_PFUNC_NF1);
	sync(emmc_port);
	printk(KERN_INFO "rtkemmc_set_pin_mux: EMMC_PFUNC_NF1 = 0x%08x \n", readl(emmc_port->emmc_membase + EMMC_PFUNC_NF1));

	rtkemmc_writel(0, emmc_port->emmc_membase+EMMC_PAD_CTL); //PAD to 1.8v

    sync(emmc_port);
}

u32 swap_endian(u32 input)
{
        u32 output;
        output = (input & 0xff000000)>>24|
                         (input & 0x00ff0000)>>8|
                         (input & 0x0000ff00)<<8|
                         (input & 0x000000ff)<<24;
        return output;
}

static void rtkemmc_read_rsp(struct rtkemmc_host *emmc_port,u32 *rsp, int reg_count)
{
    unsigned long flags2;
    MMCPRINTF("rsp addr=0x%p; rsp_count=%u\n", rsp, reg_count);

    rtk_lockapi_lock2(flags2, _at_("rtkemmc_read_rsp"));
    if ( reg_count==6 ){
	rsp[0] = rsp[1] = 0;
        rsp[0] = readl(emmc_port->emmc_membase + EMMC_RESP0);
        MMCPRINTF(KERN_INFO "rsp[0]=0x%08x, rsp[1]=0x%08x\n",rsp[0],rsp[1]);
    }else if(reg_count==17){
    	/*
    			1. UNSTUFF_BITS uses the reverse order as:
    			const int __off = 3 - ((start) / 32);
    		       2. be32_to_cpu is called in mmc_send_csd as	
    			csd[i] = be32_to_cpu(csd_tmp[i]);								
    		*/
        rsp[3] = readl(emmc_port->emmc_membase + EMMC_RESP0);
        rsp[2] = readl(emmc_port->emmc_membase + EMMC_RESP1);
        rsp[1] = readl(emmc_port->emmc_membase + EMMC_RESP2);
        rsp[0] = readl(emmc_port->emmc_membase + EMMC_RESP3);
        MMCPRINTF(KERN_INFO "rsp[0]=0x%08x, rsp[1]=0x%08x, rsp[2]=0x%08x, rsp[3]=0x%08x\n",rsp[0],rsp[1],rsp[2],rsp[3]);
    }
    else
        MMCPRINTF("rsp[0]=0x%08x\n",rsp[0]);
    rtk_lockapi_unlock2(flags2, _at_("rtkemmc_read_rsp"));
}

/*******************************************************
 *  *
 *******************************************************/
static int wait_done_timeout(struct rtkemmc_host *emmc_port, volatile u32 *addr, u32 mask, u32 value){
        int n = 0;
		unsigned long flags2;
        while (1)
        {	
        	rtk_lockapi_lock2(flags2, _at_("wait_done_timeout"));
        	if (((*addr) &mask) == value){ 
				rtk_lockapi_unlock2(flags2, _at_("wait_done_timeout"));
				break;
        	}
			rtk_lockapi_unlock2(flags2, _at_("wait_done_timeout"));
			
            if(n++ > 3000000)
            {
		rtk_lockapi_lock2(flags2, _at_("wait_done_timeout"));
		printk(KERN_ERR "Timeout!!! addr=%x, mask=%x, value=%x, emmc_port->emmc_membase + EMMC_STATUS=%x\n",addr,mask,value,readl(emmc_port->emmc_membase + EMMC_STATUS));
		 rtk_lockapi_unlock2(flags2, _at_("wait_done_timeout"));
                return -1;
            }
			udelay(1);
    		sync(emmc_port);
        }
}

static void rtkemmc_set_freq(struct rtkemmc_host *emmc_port, u32 freq)
{
    u32 tmp_val=0;
    unsigned long flags;

    spin_lock_irqsave(&emmc_port->lock,flags);
	wait_done_timeout(emmc_port, (u32*)(emmc_port->emmc_membase + EMMC_STATUS), 0x200, 0x0);          //card is not busy
	isb();
	sync(emmc_port);

	//disable clk_en_emmc_ip, using common clk framework`
	//tmp_val = (readl(emmc_port->crt_membase + SYS_CLOCK_ENABLE1) & 0xefffffff);
	//rtkemmc_writel(tmp_val, emmc_port->crt_membase + SYS_CLOCK_ENABLE1);
	clk_disable_unprepare(clk_en_emmc_ip);

	isb();
    sync(emmc_port);	

	tmp_val = (readl(emmc_port->crt_membase + SYS_PLL_EMMC4) & 0x06);  
	rtkemmc_writel(tmp_val, emmc_port->crt_membase + SYS_PLL_EMMC4);
	isb();
    sync(emmc_port);

	if (get_rtd129x_cpu_revision() < RTD129x_CHIP_REVISION_A01 ) {
		rtkemmc_writel(1, emmc_port->crt_membase + SYS_GROUP1_CK_SEL); 
		isb();
	    sync(emmc_port);
	}
	
    tmp_val = (readl(emmc_port->crt_membase + SYS_PLL_EMMC3) & 0xffff)|(freq<<16);
    rtkemmc_writel(tmp_val, emmc_port->crt_membase + SYS_PLL_EMMC3);
	isb();
    sync(emmc_port);

	tmp_val = (readl(emmc_port->crt_membase + SYS_PLL_EMMC4) | 0x01);  
	rtkemmc_writel(tmp_val, emmc_port->crt_membase + SYS_PLL_EMMC4);
	isb();
    sync(emmc_port);

	if (get_rtd129x_cpu_revision() < RTD129x_CHIP_REVISION_A01 ) {
		rtkemmc_writel(0, emmc_port->crt_membase + SYS_GROUP1_CK_SEL); 
		isb();
		sync(emmc_port);
	}

	// [A01] ECO, If EMMC N/F code changed, toggle CR_EMMC_DUMMY_SYS bit 30
	if (get_rtd129x_cpu_revision() >= RTD129x_CHIP_REVISION_A01 ) {
		rtkemmc_writel( readl(emmc_port->emmc_membase + EMMC_DUMMY_SYS) ^ 0x40000000, emmc_port->emmc_membase + EMMC_DUMMY_SYS);
		isb();
		sync(emmc_port);
	}

	udelay(400); //delay 200us to wait PLL stable
	isb();
	sync(emmc_port);
	
	//enable clk_en_emmc_ip
	//tmp_val = (readl(emmc_port->crt_membase + SYS_CLOCK_ENABLE1) | 0x10000000);
	//rtkemmc_writel(tmp_val, emmc_port->crt_membase + SYS_CLOCK_ENABLE1);

	clk_prepare_enable(clk_en_emmc_ip);
	isb();
    sync(emmc_port);

    spin_unlock_irqrestore(&emmc_port->lock, flags);

    printk(KERN_INFO "%s: set_freq to 0x%02x, EMMC_CKGEN_CTL=0x%08x, PLL_EMMC1=%08x, PLL_EMMC2=%08x, PLL_EMMC3=%08x, PLL_EMMC4=%08x\n",
              DRIVER_NAME, freq, readl(emmc_port->emmc_membase + EMMC_CKGEN_CTL), 
              readl(emmc_port->crt_membase + SYS_PLL_EMMC1), 
              readl(emmc_port->crt_membase + SYS_PLL_EMMC2), 
              readl(emmc_port->crt_membase + SYS_PLL_EMMC3), 
              readl(emmc_port->crt_membase + SYS_PLL_EMMC4));
}

void rtkemmc_set_pad_driving(struct rtkemmc_host *emmc_port, u32 clk_drv, u32 cmd_drv, u32 data_drv, u32 ds_drv)
{
	rtkemmc_writel(data_drv|(data_drv<<8)|(data_drv<<16)|(data_drv<<24), emmc_port->emmc_membase + EMMC_PDRIVE_NF1); //d0~d3	
	rtkemmc_writel(data_drv|(data_drv<<8)|(data_drv<<16)|(data_drv<<24), emmc_port->emmc_membase + EMMC_PDRIVE_NF2); //d4~d7	
	rtkemmc_writel((readl(emmc_port->emmc_membase + EMMC_PDRIVE_NF3)&(0x00ff00ff))|(clk_drv<<8)|(cmd_drv<<24), emmc_port->emmc_membase + EMMC_PDRIVE_NF3); //d4~d7	
	rtkemmc_writel(ds_drv, emmc_port->emmc_membase + EMMC_PDRIVE_NF4); //data strobe
	isb();
	sync(emmc_port);
    return;
}

static void rtkemmc_set_ip_div(struct rtkemmc_host *emmc_port,u32 set_div)
{
    unsigned long flags;
	u32 cur_div = readl(emmc_port->emmc_membase+EMMC_CLKDIV);
	if (cur_div == set_div){
		printk(KERN_INFO "%s, set_ip_div = cur_div(0x%08x), ignored. \n", DRIVER_NAME, cur_div);
		return;
	}

	wait_done_timeout(emmc_port, (u32*)(emmc_port->emmc_membase + EMMC_STATUS), 0x200, 0x0);
	
    spin_lock_irqsave(&emmc_port->lock,flags);
    //disable clock
    rtkemmc_writel(0, emmc_port->emmc_membase+EMMC_CLKENA);
    isb();

    //EMMC Cmd
    rtkemmc_writel(0xa0202000, emmc_port->emmc_membase+EMMC_CMD);
    isb();
    sync(emmc_port);
    wait_done_timeout(emmc_port, (u32*)(emmc_port->emmc_membase+EMMC_CMD), 0x80000000, 0x0);
    sync(emmc_port);

    //set divider
    rtkemmc_writel(set_div, emmc_port->emmc_membase+EMMC_CLKDIV);
    isb();
    sync(emmc_port);

	while (readl(emmc_port->emmc_membase + EMMC_CLKDIV) != set_div){
		printk(KERN_ERR RED_BOLD"Fail to write  EMMC_CLKDIV! EMMC_CLKDIV = 0x%08x\n"RESET, readl(emmc_port->emmc_membase+EMMC_CLKDIV));
		rtkemmc_writel(set_div, emmc_port->emmc_membase+EMMC_CLKDIV);
		sync(emmc_port);
		//BUG();
	}

    //EMMC Cmd
    rtkemmc_writel(0xa0202000, emmc_port->emmc_membase+EMMC_CMD);
    isb();
    sync(emmc_port);
    wait_done_timeout(emmc_port, (u32*)(emmc_port->emmc_membase+EMMC_CMD), 0x80000000, 0x0);
    isb();
    sync(emmc_port);

    //enable clock
    rtkemmc_writel(0x10001, emmc_port->emmc_membase+EMMC_CLKENA);
    isb();
    sync(emmc_port);

    //EMMC Cmd
    rtkemmc_writel(0xa0202000, emmc_port->emmc_membase+EMMC_CMD);
    isb();
    sync(emmc_port);
    wait_done_timeout(emmc_port, (u32*)(emmc_port->emmc_membase+EMMC_CMD), 0x80000000, 0x0);
    isb();
    spin_unlock_irqrestore(&emmc_port->lock, flags);

    printk(KERN_INFO "%s: set div to 0x%02x, EMMC_CLKDIV=%08x\n",
              DRIVER_NAME, set_div, readl(emmc_port->emmc_membase + EMMC_CLKDIV));
}

static void rtkemmc_set_bits(struct rtkemmc_host *emmc_port,u32 set_bit)
{
    unsigned long flags;

    spin_lock_irqsave(&emmc_port->lock,flags);
    rtkemmc_writel(set_bit,emmc_port->emmc_membase + EMMC_CTYPE);
    sync(emmc_port);
    spin_unlock_irqrestore(&emmc_port->lock, flags);
	wait_done_timeout(emmc_port, (u32*)(emmc_port->emmc_membase + EMMC_STATUS), 0x200, 0x0);          //card is not busy
    sync(emmc_port);
    printk(KERN_INFO "%s: set to 0x%08x, EMMC_CTYPE=%08x\n",
                DRIVER_NAME, set_bit, readl(emmc_port->emmc_membase + EMMC_CTYPE));
}

//wrapper clk divider
//clk_div	[2..0]
//000: div1
//001: div2
//010: div4
//011: div8

static void rtkemmc_set_wrapper_div(struct rtkemmc_host *emmc_port,u8 level)
{

    unsigned long flags;

    MMCPRINTF("\n");
    spin_lock_irqsave(&emmc_port->lock,flags);
    switch(level)
    {
        case 0:  
            rtkemmc_writel(0x2100,emmc_port->emmc_membase + EMMC_CKGEN_CTL);
            break;
        case 1:
            rtkemmc_writel(0x2010,emmc_port->emmc_membase + EMMC_CKGEN_CTL);
            break;
        case 2:
	    rtkemmc_writel(0x2102,emmc_port->emmc_membase + EMMC_CKGEN_CTL);
            break;
        case 3:
            rtkemmc_writel(0x2103,emmc_port->emmc_membase + EMMC_CKGEN_CTL);
            break;
	defualt:
	    printk(KERN_ERR "error wrapper div seting! \n");
	    break;
    }
	isb();
    sync(emmc_port);
    printk(KERN_INFO "%s: set_wrapper_div to 0x%08x\n",
                DRIVER_NAME, readl(emmc_port->emmc_membase + EMMC_CKGEN_CTL));
    spin_unlock_irqrestore(&emmc_port->lock, flags);
}

static u32 rtkemmc_get_cmd_timeout(struct sd_cmd_pkt *cmd_info)
{
    struct rtkemmc_host *emmc_port   = cmd_info->emmc_port;
    u16 block_count             = cmd_info->block_count;
    u32 tmout = 0;

    MMCPRINTF("\n");
    if(cmd_info->cmd->data)
    {
        tmout = msecs_to_jiffies(200);
        if(block_count>0x100)
        {
            tmout = tmout + msecs_to_jiffies(block_count>>1);
        }
    }
    else
        tmout = msecs_to_jiffies(80);

#ifdef CONFIG_ANDROID
    tmout += msecs_to_jiffies(100);
#endif

    cmd_info->timeout = emmc_port->tmout = tmout;
    return 0;
}


static int rtkemmc_allocate_dma_buf(struct rtkemmc_host *emmc_port, struct mmc_command *cmd)
{
    extern unsigned char* pPSP;
    if (!gddr_descriptor)
        gddr_descriptor_org = gddr_descriptor = dma_alloc_coherent(emmc_port->dev, DESC_ALLOC_LENGTH, &emmc_port->desc_paddr ,GFP_KERNEL);

    if (!gddr_dma)
        gddr_dma_org = gddr_dma = dma_alloc_coherent(emmc_port->dev, DMA_ALLOC_LENGTH, &emmc_port->dma_paddr ,GFP_KERNEL);

    if(!gddr_dma || !gddr_descriptor){
        WARN_ON(1);
        cmd->error = -ENOMEM;
        return 0;
    }    

    MMCPRINTF("allocate rtk dma buf : dma addr=0x%016llx, phy addr=0x%08x\n", gddr_dma, emmc_port->dma_paddr);
    MMCPRINTF("allocate rtk desc buf : desc addr=0x%016llx, phy addr=0x%08x\n", gddr_descriptor, emmc_port->desc_paddr);
    return 1;
}
static int rtkemmc_free_dma_buf(struct rtkemmc_host *emmc_port)
{    
   	if (gddr_descriptor_org)
        dma_free_coherent(emmc_port->dev, DESC_ALLOC_LENGTH, gddr_descriptor_org ,emmc_port->desc_paddr);
    MMCPRINTF("free rtk desc buf \n");
	
    if (gddr_dma_org)
        dma_free_coherent(emmc_port->dev, DMA_ALLOC_LENGTH, gddr_dma_org ,emmc_port->dma_paddr);
    MMCPRINTF("free rtk dma buf \n");
    return 1;
}


static int rtkemmc_set_rspparam(struct rtkemmc_host *emmc_port, struct sd_cmd_pkt *cmd_info)
{
    switch(cmd_info->cmd->opcode)
    {
        case MMC_GO_IDLE_STATE:
            cmd_info->cmd_para = (CMD_START_CMD|CMD_USE_HOLD_REG);
	    cmd_info->rsp_len = 6;
    	    cmd_info->cmd->arg = 0x00000000;
            break;
        case MMC_SEND_OP_COND:
            cmd_info->cmd_para = (CMD_START_CMD|CMD_USE_HOLD_REG|CMD_RSP_EXP);
            cmd_info->cmd->arg = MMC_SECTOR_ADDR|MMC_VDD_165_195;
	    cmd_info->rsp_len = 6;
            break;
        case MMC_ALL_SEND_CID:
            cmd_info->cmd_para = (CMD_START_CMD|CMD_USE_HOLD_REG|CMD_CHK_RESP_CRC|CMD_RSP_LEN|CMD_RSP_EXP);
	    cmd_info->rsp_len = 17;
    	    cmd_info->cmd->arg = 0x00000000;
            break;
        case MMC_SET_RELATIVE_ADDR:
            cmd_info->cmd_para = (CMD_START_CMD|CMD_USE_HOLD_REG|CMD_CHK_RESP_CRC|CMD_RSP_EXP);
            cmd_info->cmd->arg = (1<<RCA_SHIFTER);
	    cmd_info->rsp_len = 6;
            break;
        case MMC_SEND_CSD:
        case MMC_SEND_CID:
            cmd_info->cmd_para = (CMD_START_CMD|CMD_USE_HOLD_REG|CMD_CHK_RESP_CRC|CMD_RSP_LEN|CMD_RSP_EXP);
            cmd_info->cmd->arg = (1<<RCA_SHIFTER);
	    cmd_info->rsp_len = 17;
            break;
        case MMC_SEND_EXT_CSD:
            cmd_info->cmd_para = (CMD_START_CMD|CMD_USE_HOLD_REG|CMD_DATA_EXP|CMD_CHK_RESP_CRC|CMD_RSP_EXP);
            cmd_info->cmd->arg = 0;
	    cmd_info->rsp_len = 6;
            break;
        case MMC_SLEEP_AWAKE:
            cmd_info->cmd_para = (CMD_START_CMD|CMD_USE_HOLD_REG|CMD_CHK_RESP_CRC|CMD_RSP_EXP);
	    cmd_info->rsp_len = 6;
	    printk(KERN_INFO "%s : cmd5 arg=0x%08x\n",__func__,cmd_info->cmd->arg);
            break;
        case MMC_SELECT_CARD:
	    printk(KERN_INFO "%s : cmd7 arg : 0x%08x\n",__func__,cmd_info->cmd->arg);
	    if (cmd_info->cmd->flags == (MMC_RSP_NONE | MMC_CMD_AC))
	    {
		printk(KERN_INFO "%s : cmd7 with rsp none\n",__func__);
            	cmd_info->cmd_para = (CMD_START_CMD|CMD_USE_HOLD_REG|CMD_CHK_RESP_CRC);
	    }
	    else
	    {
		printk(KERN_INFO "%s : cmd7 with rsp\n",__func__);
            	cmd_info->cmd_para = (CMD_START_CMD|CMD_USE_HOLD_REG|CMD_CHK_RESP_CRC|CMD_RSP_LEN|CMD_RSP_EXP);
	    }
	    cmd_info->rsp_len = 6;
            break;
        case MMC_SWITCH:
            cmd_info->cmd_para = (CMD_START_CMD|CMD_USE_HOLD_REG|CMD_CHK_RESP_CRC|CMD_RSP_EXP);
	    cmd_info->rsp_len = 6;
            break;
        case MMC_SEND_STATUS:
            cmd_info->cmd_para = (CMD_START_CMD|CMD_USE_HOLD_REG|CMD_CHK_RESP_CRC|CMD_RSP_EXP);
            cmd_info->cmd->arg = (1<<RCA_SHIFTER);
	    cmd_info->rsp_len = 6;
            break;	
        case MMC_STOP_TRANSMISSION:
            cmd_info->cmd_para = (CMD_START_CMD|CMD_USE_HOLD_REG|CMD_CHK_RESP_CRC|CMD_RSP_EXP);
	    cmd_info->rsp_len = 6;
            break;
        case MMC_READ_MULTIPLE_BLOCK:
            cmd_info->cmd_para = (CMD_START_CMD|CMD_USE_HOLD_REG|CMD_SEND_AUTO_STOP|CMD_DATA_EXP|CMD_CHK_RESP_CRC|CMD_RSP_EXP|CMD_WAIT_PRV_DATA_COMPLETE);
	    cmd_info->rsp_len = 6;
            break;
	case MMC_SET_BLOCK_COUNT:
            cmd_info->cmd_para = (CMD_START_CMD|CMD_USE_HOLD_REG|CMD_CHK_RESP_CRC|CMD_RSP_EXP);
	    	cmd_info->rsp_len = 6;
            break;	
        case MMC_WRITE_MULTIPLE_BLOCK:
            cmd_info->cmd_para = (CMD_START_CMD|CMD_USE_HOLD_REG|CMD_SEND_AUTO_STOP|CMD_RD_WR|CMD_DATA_EXP|CMD_CHK_RESP_CRC|CMD_RSP_EXP);
	    cmd_info->rsp_len = 6;
            break;
        case MMC_READ_SINGLE_BLOCK:
            cmd_info->cmd_para = (CMD_START_CMD|CMD_USE_HOLD_REG|CMD_DATA_EXP|CMD_CHK_RESP_CRC|CMD_RSP_EXP|CMD_WAIT_PRV_DATA_COMPLETE);
	    cmd_info->rsp_len = 6;
            break;
        case MMC_WRITE_BLOCK:
            cmd_info->cmd_para = (CMD_START_CMD|CMD_USE_HOLD_REG|CMD_RD_WR|CMD_DATA_EXP|CMD_CHK_RESP_CRC|CMD_RSP_EXP);
	    cmd_info->rsp_len = 6;
            break;
		case MMC_ERASE_GROUP_START:
		case MMC_ERASE_GROUP_END:
		case MMC_ERASE:
			cmd_info->cmd_para = (CMD_START_CMD|CMD_USE_HOLD_REG|CMD_CHK_RESP_CRC|CMD_RSP_EXP);
	    	cmd_info->rsp_len = 6;
			break;
	case MMC_GEN_CMD:
	    if(cmd_info->cmd->arg & 0x1) {   //read single block
		cmd_info->cmd->arg = 0x1;
		cmd_info->cmd_para = (CMD_START_CMD|CMD_USE_HOLD_REG|CMD_DATA_EXP|CMD_CHK_RESP_CRC|CMD_RSP_EXP|CMD_WAIT_PRV_DATA_COMPLETE);
		cmd_info->rsp_len = 6;
		break;
	    }
	    else {      //write single block
		cmd_info->cmd->arg = 0x0;
		cmd_info->cmd_para = (CMD_START_CMD|CMD_USE_HOLD_REG|CMD_RD_WR|CMD_DATA_EXP|CMD_CHK_RESP_CRC|CMD_RSP_EXP);
		cmd_info->rsp_len = 6;
		break;
	    }
        default:
            cmd_info->cmd_para = (CMD_START_CMD|CMD_USE_HOLD_REG);
	    cmd_info->rsp_len = 6;
            break;            
    }
    MMCPRINTF(KERN_INFO "%s : cmd=0x%02x rsp_len=0x%02x arg=0x%08x para=0x%08x\n","rtkemmc", cmd_info->cmd->opcode, cmd_info->rsp_len,cmd_info->cmd->arg,cmd_info->cmd_para);
    return 0;
}

static unsigned int counter = 0;

static int SD_SendCMDGetRSP_Cmd(struct sd_cmd_pkt *cmd_info,int bIgnore)
{
    volatile u8 cmd_idx              = cmd_info->cmd->opcode;
    u32 *rsp                = (u32 *)&cmd_info->cmd->resp;
    struct rtkemmc_host *emmc_port = cmd_info->emmc_port;
    struct mmc_host *host = emmc_port->mmc;
    u32 iobase = emmc_port->emmc_membase;
    int err, retry_count=0;
    unsigned long flags;
    unsigned long flags2;
    u32 dma_val=0;
    u32 byte_count = 0x200, block_count = 1, cpu_mode=0, sa=0;
    u8 tmp9_buf[1024]={0};
    u8 state = 0;
    int cmd1_retry_cnt = 3000;
	int i = 0;
	u32 reg = 0;
	
	MMCPRINTF("%s \n", __func__);

	//printk(KERN_ERR RED_BOLD"[SD_Stream] Receive CMD%u\n"RESET, cmd_info->cmd->opcode);

	//igone CMD5
	//if (cmd_info->cmd->opcode == MMC_SLEEP_AWAKE) {
	//	printk(KERN_ERR RED_BOLD"[SD_SendCMDGetRSP_Cmd] Receive CMD5, ignore ...........\n"RESET);
	//	return 0;
	//}

    rtkemmc_set_rspparam(emmc_port,cmd_info);   //for 1295

	//if (cmd_idx == MMC_ALL_SEND_CID){
	//	printk(KERN_ERR RED_BOLD" <================ emmc driver: mmc_all_send_cid ================> \n"RESET);
	//}

    if(rsp == NULL) {
        BUG_ON(1);
    }
    if ((g_crinit == 0)&&(cmd_idx > MMC_SET_RELATIVE_ADDR))
    {
        printk("%s : ignore cmd:0x%02x since we're still in emmc init stage\n",DRIVER_NAME,cmd_idx);
        return CR_TRANSFER_FAIL;
    }

RET_CMD:
    rtk_lockapi_lock2(flags2, _at_("SD_SendCMDGetRSP_Cmd"));
    rtkemmc_writel(0, emmc_port->emmc_membase + EMMC_SWC_SEL);
    rtkemmc_writel(0, emmc_port->emmc_membase + EMMC_CP);

	//gpio 100 pull low
#ifdef EMMC_LA_DEBUG_GPIO
	reg = readl(emmc_port->misc_membase + MISC_GP3DATO);
	reg &= (~0x00000010);
	rtkemmc_writel(reg, emmc_port->misc_membase + MISC_GP3DATO);
	isb();
	sync(emmc_port);
#endif	

    rtkemmc_writel(cmd_info->cmd->arg, emmc_port->emmc_membase + EMMC_CMDARG);
    isb();
    sync(emmc_port);


	//gpio 100 pull high
#ifdef EMMC_LA_DEBUG_GPIO
	reg = readl(emmc_port->misc_membase + MISC_GP3DATO);
	reg |= (0x00000010);
	rtkemmc_writel(reg, emmc_port->misc_membase + MISC_GP3DATO);
	isb();
	sync(emmc_port);
#endif


#ifdef EMMC_LA_DEBUG_GPIO
	//gpio 90  pull  low
	reg = readl(emmc_port->misc_membase + MISC_GP2DATO);
	reg &= ~(0x04000000);
	rtkemmc_writel(reg, emmc_port->misc_membase + MISC_GP2DATO);
	isb();
	sync(emmc_port);
#endif
	
	//workaround, if fail to write EMMC_CMDARG, re-write
	while(readl(emmc_port->emmc_membase + EMMC_CMDARG) != cmd_info->cmd->arg){
		
		printk(KERN_ERR RED_BOLD"Fail to write EMMC_CMDARG ! \n"RESET);
		printk(KERN_ERR "cmd_info->cmd->arg = 0x%08x \n", cmd_info->cmd->arg );
		printk(KERN_ERR "EMMC_CMDARG = 0x%08x \n", readl(emmc_port->emmc_membase + EMMC_CMDARG));
		//BUG();
		rtkemmc_writel(cmd_info->cmd->arg, emmc_port->emmc_membase + EMMC_CMDARG);
	    isb();
	    sync(emmc_port);
		
	}
	
#ifdef EMMC_LA_DEBUG_GPIO
	//gpio 90  pull  high
	reg = readl(emmc_port->misc_membase + MISC_GP2DATO);
	reg |= (0x04000000);
	rtkemmc_writel(reg, emmc_port->misc_membase + MISC_GP2DATO);
	isb();
	sync(emmc_port);
#endif

    rtk_lockapi_unlock2(flags2, _at_("SD_SendCMDGetRSP_Cmd"));

	if (cmd_idx == MMC_SET_BLOCK_COUNT){
		emmc_port->rpmb_cmd = 1;
	}
	else{ 
		emmc_port->rpmb_cmd = 0;
	}

    err = rtkemmc_wait_opt_end(DRIVER_NAME,emmc_port,EMMC_SENDCMDGETRSP,(cmd_idx|cmd_info->cmd_para),bIgnore);

    if(err == RTK_SUCC){
		sync(emmc_port);
        rtkemmc_read_rsp(emmc_port,rsp, cmd_info->rsp_len);
        sync(emmc_port);
        //luis, TBC

		//if(cmd_idx == MMC_SEND_OP_COND) 
		//	printk(KERN_ERR RED_BOLD"[CMD1] RSP = 0x%08x\n"RESET,rsp[0]);
		
        while ((cmd_idx == MMC_SEND_OP_COND) && ((rsp[0]&0x80000000) != 0x80000000) && cmd1_retry_cnt--)
		{
        	sync(emmc_port);
			mdelay(1);
			goto RET_CMD;	
		}

		//CMD1 timeout case
		if ((cmd_idx == MMC_SEND_OP_COND) && (rsp[0]&0x80000000 != 0x80000000)){
			printk(KERN_ERR RED_BOLD"eMMC CMD1 timeout error !!\n"RESET);
			BUG();
		}

        if (cmd_idx == MMC_SET_RELATIVE_ADDR)
        {
            g_crinit = 1;
            MMCPRINTF("emmc init done ...\n");
        }
		//get cmd7 status
		if ((cmd_info->cmd->flags == (MMC_RSP_NONE | MMC_CMD_AC))&&(cmd_idx==MMC_SELECT_CARD))
		{
			printk(KERN_INFO "get status =>\n");
	        rtkemmc_send_status(host->card,&state,0,0);
		}
	}
	else {
		if (!bIgnore)
		{
        	printk(KERN_WARNING "%s: %s cmd trans fail, err=%d, ignore=%d, gPreventRetry=%d, gCurrentBootMode=%d, cmd_idx=%d\n",DRIVER_NAME,__func__, err, bIgnore,gPreventRetry,gCurrentBootMode,cmd_idx);
        }

        MMCPRINTF("%s: %s gCurrentBootMode =%d\n", DRIVER_NAME, __func__, gCurrentBootMode);

		if (gPreventRetry)
		{
	        printk(KERN_WARNING "[LY]error when card in uninit state, err=%d\n",err);
			return err;
		}
        if (gCurrentBootMode > MODE_SDR && cmd_idx > MMC_SEND_OP_COND)
        	return err;
        if (cmd_idx == MMC_SEND_STATUS) //prevent dead lock looping
            return err;
        if (retry_count++ < MAX_CMD_RETRY_COUNT)
        {
			printk(KERN_ERR RED_BOLD"cmd %d retry %d ---->\n"RESET,cmd_idx,retry_count);
            err = error_handling(emmc_port,cmd_idx,bIgnore);
        	goto RET_CMD;
        }
    }
    return err;
}

static int SD_SendCMDGetRSP(struct sd_cmd_pkt * cmd_info,int bIgnore)
{
    int rc;
    MMCPRINTF("\n");


    rc = SD_SendCMDGetRSP_Cmd(cmd_info,bIgnore);

    return rc;
}

static void duplicate_pkt(struct sd_cmd_pkt* sour,struct sd_cmd_pkt* dist)
{
    dist->emmc_port      = sour->emmc_port;
    dist->cmd         = sour->cmd;
    dist->data        = sour->data;

    dist->dma_buffer  = sour->dma_buffer;
    dist->byte_count  = sour->byte_count;
    dist->block_count = sour->block_count;

    dist->flags       = sour->flags;
    dist->rsp_len     = sour->rsp_len;
    dist->timeout     = sour->timeout;
}

static int rtkemmc_err_handle(u16 cmdcode,struct sd_cmd_pkt *cmd_info)
{
    struct mmc_host *host       = cmd_info->emmc_port->mmc;
    u16 state = 0;
    int err = 0;

    MMCPRINTF("(%s:%d) : cmd=0x%02x\n", __func__,__LINE__,cmdcode);
    if(host->card){
        if(cmdcode == EMMC_AUTOWRITE2){
            if( cmd_info->cmd->opcode == 18 ||
                cmd_info->cmd->opcode == 25 )
            {
                int stop_loop = 5;
                while(stop_loop--){

                    err = rtkemmc_stop_transmission(host->card,0);
                    if(err){
                        //mdelay(1);
                        rtkemmc_send_status(host->card,&state,0,0);
                        if(state == STATE_TRAN)
                            break;
                    }else{
                        break;
                    }
                }
            }
        }
    	MMCPRINTF("(%s:%d) - cmd=0x%/02x, before polling TRAN state\n", __func__,__LINE__,cmdcode);
        err = rtkemmc_wait_status(host->card,STATE_TRAN,0,0);
    }
	return err;
}

static int SD_Stream_Cmd(u16 cmdcode,struct sd_cmd_pkt *cmd_info, unsigned int bIgnore)
{
    u8 cmd_idx              = cmd_info->cmd->opcode;
    u32 *rsp                = (u32 *)&cmd_info->cmd->resp;
    u16 byte_count          = cmd_info->byte_count;
    u16 block_count         = cmd_info->block_count;
    struct rtkemmc_host *emmc_port = cmd_info->emmc_port;
    u32 iobase = emmc_port->emmc_membase;
    int err;
    u8 *data              = cmd_info->dma_buffer;
    unsigned long flags;
    unsigned long flags2;
    u32 cpu_mode=0;
    u32 sa=0,retry_count=0,i=0;
    u32 reg = 0;
    MMCPRINTF("%s \n", __func__);

    if(rsp == NULL) {
        BUG_ON(1);
    }

#ifdef TEST_POWER_RESCYCLE
    cmd_info->emmc_port->test_count++;
    mmcspec("test_count=%d\n",cmd_info->emmc_port->test_count);
#endif
    if ((g_crinit == 0)&&(cmd_idx > MMC_SET_RELATIVE_ADDR))
    {
        printk("%s : ignore cmd:0x%02x since we're still in emmc init stage\n",DRIVER_NAME,cmd_idx);
        return CR_TRANSFER_FAIL;
    }

    rtkemmc_set_rspparam(emmc_port,cmd_info);   //for 119x   
 
    /*************************************************************************/
    rtk_lockapi_lock2(flags2, _at_("SD_Stream_Cmd"));
    rtkemmc_writel(0, emmc_port->emmc_membase + EMMC_SWC_SEL);
    rtkemmc_writel(0, emmc_port->emmc_membase + EMMC_CP);
    rtk_lockapi_unlock2(flags2, _at_("SD_Stream_Cmd"));
    /*************************************************************************/
    if (cmd_info->data)
    	make_sg_des(cmd_info, emmc_port->desc_paddr, emmc_port);
    else if (data)
	make_ip_des(data, block_count<<9, emmc_port->desc_paddr, emmc_port);	
    else 
	BUG_ON(1);	
    	

STR_CMD_RET:
    rtk_lockapi_lock2(flags2, _at_("SD_Stream_Cmd"));
    rtkemmc_writel(block_count<<9, emmc_port->emmc_membase + EMMC_BYTCNT);
    rtkemmc_writel(emmc_port->desc_paddr, emmc_port->emmc_membase + EMMC_DBADDR);
    rtkemmc_global_blksize = readl(emmc_port->emmc_membase + EMMC_BLKSIZE);
    rtkemmc_global_bytecnt = readl(emmc_port->emmc_membase + EMMC_BYTCNT);
    rtkemmc_global_dbaddr = readl(emmc_port->emmc_membase + EMMC_DBADDR);
    MMCPRINTF("%s EMMC_BYTCNT=0x%x  \n", __func__, rtkemmc_global_bytecnt);
	 //gpio 100 pull low
#ifdef EMMC_LA_DEBUG_GPIO
	reg = readl(emmc_port->misc_membase + MISC_GP3DATO);
	reg &= (~0x00000010);
	rtkemmc_writel(reg, emmc_port->misc_membase + MISC_GP3DATO);
	isb();
	sync(emmc_port);
#endif	

    rtkemmc_writel(cmd_info->cmd->arg, emmc_port->emmc_membase + EMMC_CMDARG);
    isb();
    sync(emmc_port);

	//gpio 100 pull high
#ifdef EMMC_LA_DEBUG_GPIO
	reg = readl(emmc_port->misc_membase + MISC_GP3DATO);
	reg |= (0x00000010);
	rtkemmc_writel(reg, emmc_port->misc_membase + MISC_GP3DATO);
	isb();
	sync(emmc_port);
#endif

#ifdef EMMC_LA_DEBUG_GPIO
	//gpio 90  pull  low
	reg = readl(emmc_port->misc_membase + MISC_GP2DATO);
	reg &= ~(0x04000000);
	rtkemmc_writel(reg, emmc_port->misc_membase + MISC_GP2DATO);
	isb();
	sync(emmc_port);
#endif


	//workaround, if fail to write EMMC_CMDARG, re-write
	while(readl(emmc_port->emmc_membase + EMMC_CMDARG) != cmd_info->cmd->arg){
		printk(KERN_ERR RED_BOLD"Fail to write EMMC_CMDARG ! \n"RESET);
		printk(KERN_ERR "cmd_info->cmd->arg = 0x%08x \n", cmd_info->cmd->arg );
		printk(KERN_ERR "EMMC_CMDARG = 0x%08x \n", readl(emmc_port->emmc_membase + EMMC_CMDARG));
		//BUG();
		rtkemmc_writel(cmd_info->cmd->arg, emmc_port->emmc_membase + EMMC_CMDARG);
	    isb();
	    sync(emmc_port);
		
	}

	//gpio 90  pull  high
#ifdef EMMC_LA_DEBUG_GPIO	
	reg = readl(emmc_port->misc_membase + MISC_GP2DATO);
	reg |= (0x04000000);
	rtkemmc_writel(reg, emmc_port->misc_membase + MISC_GP2DATO);
	isb();
	sync(emmc_port);
#endif	
	
    emmc_port->cmd_opcode = cmd_idx;
    MMCPRINTF(KERN_INFO "%s - cmdidx:0x%02x, byte_cnt:0x%08x, cmd_arg:0x%08x \n", __func__, cmd_idx, readl(emmc_port->emmc_membase + EMMC_BYTCNT), readl(emmc_port->emmc_membase + EMMC_CMDARG));
    rtkemmc_get_cmd_timeout(cmd_info);
    isb();
    sync(emmc_port);
    rtk_lockapi_unlock2(flags2, _at_("SD_Stream_Cmd"));
    err = rtkemmc_wait_opt_end(DRIVER_NAME,emmc_port,cmdcode,(cmd_idx|cmd_info->cmd_para),bIgnore);
	
    wait_done_timeout(emmc_port, (u32*)(emmc_port->emmc_membase + EMMC_CMD), 0x80000000, 0x0);
    isb();
    sync(emmc_port);

#ifdef FORCE_CHG_SDR50_MODE
    //printk("hacker cmd_arg=%08x\n", cmd_arg);
    if (cmd_arg == 0x9999)
    {
        ret_err = -1;
    }
#endif

    if(err == RTK_SUCC){
        if((cmdcode == EMMC_AUTOREAD1) || (cmdcode == EMMC_AUTOWRITE1))
        {
            MMCPRINTF("AUTO READ/WRITE 1 skip response~\n");
        }
        else
        {
            rtkemmc_read_rsp(emmc_port,rsp, cmd_info->rsp_len);
            MMCPRINTF("---stream cmd done---\n");
        }
#if 0
	if ((cmd_idx > 17)&&(!bIgnore))
		BUG();
#endif
    }
    else
    {
	if(cmd_resend==1) {
                        return RTK_SUCC;
        }

        MMCPRINTF("strm cmd_idx=%d,ret_err=%d,bIgnore=%d\n",cmd_idx ,err,bIgnore);
        mmcmsg3(KERN_WARNING "%s: %s fail\n",DRIVER_NAME,__func__);
        if (bIgnore)
        	return err;
#if 0
	BUG();
#endif
        if (retry_count++ < MAX_CMD_RETRY_COUNT)
        {
    		printk(KERN_WARNING "retry %d ---->\n",retry_count);
            err = error_handling(emmc_port,cmd_idx,bIgnore);
            #ifdef FORCE_CHG_SDR50_MODE
            if ((cmd_arg == 0x9999)&&(!err))
                    return RTK_SUCC;
            #endif
            goto STR_CMD_RET;
        }
    }
    return err;
}

void rtkemmc_speed_up(void)
{
        struct mmc_host * mmc = mmc_host_local;
        struct rtkemmc_host *emmc_port;

        emmc_port = mmc_priv(mmc);

        rtkemmc_set_freq(emmc_port,0x46);  //80Mhz
        rtkemmc_set_ip_div(emmc_port,EMMC_CLOCK_DIV_8);
}
EXPORT_SYMBOL(rtkemmc_speed_up);

static u32 rtkemmc_chk_cmdcode(struct mmc_command* cmd){
    u32 cmdcode;

    if(cmd->opcode < 56){
        cmdcode = (u32)rtk_sd_cmdcode[cmd->opcode][0];
        WARN_ON(cmd->data == NULL);
        if(cmd->data->flags & MMC_DATA_WRITE){
            if(cmd->opcode == 42)
                cmdcode = EMMC_NORMALWRITE;
            else if(cmd->opcode == 56)
                cmdcode = EMMC_AUTOWRITE2;
        }
    }else{
        cmdcode = EMMC_CMD_UNKNOW;
    }

    return cmdcode;

}

//#define USE_TMP_BUF
static int SD_Stream(struct sd_cmd_pkt *cmd_info)
{
    int err=0;
    u32 i;
    struct scatterlist *sg;
    u32 dir = 0;
    u32 dma_nents = 0;
    u32 dma_leng = 0;
    u32 dma_addr;
    u32 dma_addr_sys = 0;
    u32 old_arg;
    u8 one_blk=0;
    u8 f_in_dma = 0;
    u16 cmdcode = 0;
    unsigned long flags;

    struct mmc_host *host = cmd_info->emmc_port->mmc;
    struct rtkemmc_host *emmc_port = cmd_info->emmc_port;

	//printk(KERN_ERR RED_BOLD"[SD_Stream] Receive CMD%u\n"RESET, cmd_info->cmd->opcode);
	
	//if (cmd_info->cmd->opcode == MMC_SLEEP_AWAKE) {
	//	printk(KERN_ERR RED_BOLD"[SD_Stream] Receive CMD5, ignore ...........\n"RESET);
	//	return 0;
	//}
    MMCPRINTF("%s \n", __func__);

    rtkemmc_set_rspparam(emmc_port, cmd_info);   //for 119x

    MMCPRINTF("\n");
    if(cmd_info->data->flags & MMC_DATA_READ){
        dir = DMA_FROM_DEVICE;
    }else{
        dir = DMA_TO_DEVICE;
    }

    cmd_info->data->bytes_xfered=0;
    //dma_nents = dma_map_sg( mmc_dev(host), cmd_info->data->sg, cmd_info->data->sg_len,  dir);
    //sg = cmd_info->data->sg;

#ifdef SHOW_MMC_PRD
    printk("sg_len:%u\n",cmd_info->data->sg_len);
    printk("sg:0x%p; dma_nents:%u\n",sg,dma_nents);
#endif
    old_arg=cmd_info->cmd->arg;

    //for(i=0; i<dma_nents; i++,sg++)
    //    dma_leng += sg_dma_len(sg);

    //printk("%s dma_leng=0x%x \n", __func__, dma_leng);      //hcy test
    //printk("%s cmd_info->data->blksz  =0x%x \n", __func__, 	cmd_info->data->blksz ); 
    //printk("%s cmd_info->data->blocks =0x%x \n", __func__, 	cmd_info->data->blocks); 	
    //printk("%s cmd_info->data->blksz * cmd_info->data->blocks =0x%x \n", __func__, 	cmd_info->data->blksz * cmd_info->data->blocks); 
    dma_leng = cmd_info->data->blksz * cmd_info->data->blocks;

#ifdef RTK_EMMC_HEALTH_REPORT
	//if(host->remainder == 0) printk(KERN_INFO "cmd_info->data->blksz = %d\n", cmd_info->data->blksz);
	//1G = 1024 x 1024 x 1024 bytes, so emmc size is from ext_csd,
	if((cmd_info->cmd->opcode==24 || cmd_info->cmd->opcode==25) && host->card!=NULL) {
		host->remainder += cmd_info->data->blocks;
		if(host->remainder/host->card->ext_csd.sectors > 0) {
			host->wr_emmc_size_cnt += (host->remainder/host->card->ext_csd.sectors);
			host->remainder = host->remainder % host->card->ext_csd.sectors;
		}
	}
	if(host->wr_emmc_size_cnt > 3000) printk(KERN_ERR "EMMC write count is larger than 3000 times of emmc size!!!\n");
#endif
		//				PAGE_SIZE, dir);
#ifdef SHOW_MMC_PRD
    printk("dma_addr:0x%x; dma_leng:0x%x\n",dma_addr,dma_leng);
    mmcinfo("host=%p\n",host);
    if(host->card){
    	mmcinfo("card=%p\n",host->card);
   	if(mmc_card_blockaddr(host->card))
    		printk("arg:0x%x blk\n",cmd_info->cmd->arg);
        else
                printk("arg:0x%x byte\n",cmd_info->cmd->arg);
   }
#endif

   {
            //u32 blk_cnt;
            cmd_info->byte_count = BYTE_CNT;     //rtk HW limite, one trigger 512 byte pass.
            //blk_cnt = dma_leng/BYTE_CNT;

            //if(blk_cnt == 0 && dma_leng){
            //    blk_cnt = 1;
            //}

            cmd_info->block_count =  cmd_info->data->blocks; //blk_cnt;
            //cmd_info->dma_buffer = (unsigned char *)dma_addr;     hcy: we dont need this

            cmdcode = emmc_port->ops->chk_cmdcode(cmd_info->cmd);
            err = SD_Stream_Cmd(cmdcode,cmd_info,0);


            if(err == 0){
                if(host->card){
                    if( (cmd_info->cmd->opcode == 25) &&
                        (cmdcode == EMMC_AUTOWRITE2) )
                    {
                        int stop_err;

                        stop_err = rtkemmc_stop_transmission(host->card,0);
                        if(stop_err){
                            MMCPRINTF("rtkemmc_stop_transmission fail\n");
                            goto ERR_HANDLE;
                        }
                    }
                }

                if(host->card && mmc_card_blockaddr(host->card))
                    cmd_info->cmd->arg += cmd_info->block_count;
                else
                    cmd_info->cmd->arg += dma_leng;

                cmd_info->data->bytes_xfered += dma_leng;

            }else{
ERR_HANDLE:
		//TBD
                if(rtkemmc_err_handle(cmdcode,cmd_info)){
                    if(cmd_info->cmd->opcode == 18
                    || cmd_info->cmd->opcode == 25)
                    {
                        if(host->card){
                            int stop_err;
                            stop_err = rtkemmc_stop_transmission(host->card,0);
                            if(stop_err){
                                MMCPRINTF("rtkemmc_stop_transmission fail\n");
                                //goto ERR_HANDLE;
                            }
                            rtkemmc_wait_status(host->card,STATE_TRAN,0,0);
                        }

                    }else{
                        mmcmsg3(KERN_INFO "%s: error recover fail 4\n",
                                            DRIVER_NAME);
                    }
                }
            }
        

        if(err)
            cmd_info->cmd->arg = old_arg;
        

    }
    dma_unmap_sg( mmc_dev(host), cmd_info->data->sg,
                            cmd_info->data->sg_len,  dir);
    return err;

}

static void down_speed_handling(struct rtkemmc_host *emmc_port)
{
        u8 state=0;
        int err=1;
        unsigned long flags2;
        struct mmc_host *mmc = emmc_port->mmc;
        printk(KERN_ERR "down speed handling....\n");
        //downgrade to SDR50 mode
        mmc->caps &= ~(MMC_CAP_UHS_DDR50|MMC_CAP_1_8V_DDR);
        mmc->caps2 &= ~(MMC_CAP2_HS200_1_8V_SDR);

        udelay(200);

        rtkemmc_stop_transmission(mmc->card, 1);
        wait_done_timeout(emmc_port, (u32*)(emmc_port->emmc_membase + EMMC_STATUS), 0x200, 0x0);          //card is not busy
        isb();
        sync(emmc_port);

        rtk_lockapi_lock2(flags2, _at_("down_speed_handling"));
        if (readl(emmc_port + EMMC_TBBCNT) == 1024) {
                rtk_lockapi_unlock2(flags2, _at_("down_speed_handling"));
                wait_done_timeout(emmc_port, (u32*)(emmc_port->emmc_membase + EMMC_ISR), ISR_DMA_DONE_INT, ISR_DMA_DONE_INT);          //card is not busy
                isb();
                sync(emmc_port);
                rtk_lockapi_lock2(flags2, _at_("down_speed_handling"));
                rtkemmc_clr_int_sta();
                rtk_lockapi_unlock2(flags2, _at_("down_speed_handling"));
        }
        else
                rtk_lockapi_unlock2(flags2, _at_("down_speed_handling"));

        host_card_stop2(emmc_port);
        polling_to_tran_state(emmc_port,MMC_WRITE_MULTIPLE_BLOCK,1);

        if (!g_bResuming)
                gCurrentBootMode = MODE_SDR;

        rtkemmc_select_card_type(mmc->card);
        rtkemmc_select_timing(mmc->card);

        rtkemmc_set_freq(emmc_port, 0x57); //100Mhz
        rtkemmc_set_ip_div(emmc_port, EMMC_CLOCK_DIV_2); // 100MHZ/2 = 50MHZ
        rtkemmc_set_pad_driving(emmc_port,0x33, 0x33, 0x33, 0x33);

	rtkemmc_writel(3, emmc_port->crt_membase + SYS_PLL_EMMC1);      //980001f0
        isb();

        sync(emmc_port);
        udelay(100);
}
static void rtkemmc_req_end_tasklet(unsigned long param)
{
    struct rtkemmc_host *emmc_port;
    struct mmc_request* mrq;
    unsigned long flags;
    MMCPRINTF("%s \n", __func__);

    emmc_port = (struct rtkemmc_host *)param;
    spin_lock_irqsave(&emmc_port->lock,flags);

    mrq = emmc_port->mrq;
    emmc_port->mrq = NULL;

    spin_unlock_irqrestore(&emmc_port->lock, flags);
    mmc_request_done(emmc_port->mmc, mrq);
}

static void rtkemmc_send_command(struct rtkemmc_host *emmc_port, struct mmc_command *cmd)
{
    int rc = 0;
    int rsp_type = 0;
    struct sd_cmd_pkt cmd_info;
    unsigned long flags;
    unsigned long flags2;
    unsigned int reg_val;
    unsigned int reg_val2;
    unsigned long tmout;
    unsigned long curr_jiffies;
    int istimeout;
    int read_reg_yet;
    MMCPRINTF("%s \n", __func__);
	
    memset(&cmd_info, 0, sizeof(struct sd_cmd_pkt));

    if ( !emmc_port || !cmd ){
        printk(KERN_ERR "%s: emmc_port or cmd is null\n",DRIVER_NAME);
        return ;
    }
   
    cmd_info.cmd    = cmd;
    cmd_info.emmc_port = emmc_port;
    
#ifdef FORCE_CHECK_CMD_AND_STS
	// ChiYun's comment
	// wait_done((UINT32 *)0x9801202c, 0x80000000, 0);      //cmd done
	// wait_done((UINT32 *)0x98012048, 0x000006f0, 0);      //idle
	istimeout = 1;
	read_reg_yet = 0;
	reg_val = 0xdeadbeef;
	curr_jiffies = tmout = jiffies + msecs_to_jiffies(1000);
	// time_before(jiffies, tmout) = time_after(tmout, jiffies) = ((jiffies - tmout) < 0) 
	while( time_before(jiffies, tmout) ) {
		rtk_lockapi_lock2(flags2, _at_("rtkemmc_send_command"));
		reg_val = readl(emmc_port->emmc_membase + EMMC_CMD);
		rtk_lockapi_unlock2(flags2, _at_("rtkemmc_send_command"));
		read_reg_yet = 1;
		if( !(reg_val & 0x80000000UL) ) {
			istimeout = 0;
			break;								
		}
	}
	curr_jiffies = jiffies;
	if( istimeout ) {
		rtk_lockapi_lock2(flags2, _at_("rtkemmc_send_command"));
		reg_val = readl(emmc_port->emmc_membase + EMMC_CMD);
		rtk_lockapi_unlock2(flags2, _at_("rtkemmc_send_command"));
		printk(KERN_ERR "****** tmout 0x%llx, curr_jiffies 0x%llx, jiffies 0x%llx\n", tmout, curr_jiffies, jiffies);
		printk(KERN_ERR "****** check 0x9801202c timeout!!!! (reg val 0x%08x@base_0x%08x)(%d)\n", reg_val, emmc_port->emmc_membase, read_reg_yet);
		printk(KERN_ERR "****** read  0x9801202c again!!!!!! (reg val 0x%08x@base_0x%08x)\n", reg_val, emmc_port->emmc_membase);
	}			
	
	istimeout = 1;
	read_reg_yet = 0;
	reg_val = 0xbeefdead;
	curr_jiffies = tmout = jiffies + msecs_to_jiffies(1000);
	while(time_before(jiffies, tmout)) {
		rtk_lockapi_lock2(flags2, _at_("rtkemmc_send_command"));
		reg_val = readl(emmc_port->emmc_membase + EMMC_STATUS);
		rtk_lockapi_unlock2(flags2, _at_("rtkemmc_send_command"));
		read_reg_yet = 1;
		if( !(reg_val & 0x000006f0UL) ) {
			istimeout = 0;
			break;								
		}
	}
	curr_jiffies = jiffies;
	if( istimeout ) {
		rtk_lockapi_lock2(flags2, _at_("rtkemmc_send_command"));
		reg_val = readl(emmc_port->emmc_membase + EMMC_STATUS);
		rtk_lockapi_unlock2(flags2, _at_("rtkemmc_send_command"));
		printk(KERN_ERR "****** tmout 0x%llx, curr_jiffies 0x%llx, jiffies 0x%llx\n", tmout, curr_jiffies, jiffies);
		printk(KERN_ERR "****** check 0x98012048 timeout!!!! (reg val 0x%08x@base_0x%08x)(%d)\n", reg_val, emmc_port->emmc_membase, read_reg_yet);
		printk(KERN_ERR "****** read  0x98012048 again!!!!!! (reg val 0x%08x@base_0x%08x)\n", reg_val, emmc_port->emmc_membase);
	}
#endif // FORCE_CHECK_CMD_AND_STS

    if (cmd->data){
        cmd_info.data = cmd->data;
        if(cmd->data->flags == MMC_DATA_READ){
		MMCPRINTF("%s MMC_DATA_READ\n", __func__);

            /* do nothing */
        }else if(cmd->data->flags == MMC_DATA_WRITE){
		MMCPRINTF("%s MMC_DATA_WRITE\n", __func__);

            if(emmc_port->wp ==1){
                printk(KERN_WARNING "%s: card is locked!",
                            DRIVER_NAME);
                rc = -1;
                cmd->retries = 0;
                goto err_out;
            }
        }else{
            printk(KERN_ERR "error: cmd->data->flags=%d\n",
                    cmd->data->flags);
            cmd->error = -MMC_BLK_CMD_ERR;
            cmd->retries = 0;
            goto err_out;
        }
    	if (g_bTuning)
    		MMCPRINTF("................[%s:HS200] still tuning.............. \n",DRIVER_NAME);
        rc = SD_Stream(&cmd_info);

    }
    else{
		if (g_bTuning)
    		MMCPRINTF("................[%s:HS200] still tuning.............. \n",DRIVER_NAME);
        rc = SD_SendCMDGetRSP(&cmd_info,0);
    }

    if(cmd->opcode == MMC_SWITCH){
         if((cmd->arg & 0xffff00ff) == 0x03b30001) {
            if((cmd->arg & 0x0000ff00)==0){
                emmc_port->rtflags |= RTKCR_USER_PARTITION;
            }else{
                emmc_port->rtflags &= ~RTKCR_USER_PARTITION;
            }
         }
    }
    MMCPRINTF("%s: cmd->opcode=0x%02x finish !!\n",__func__,cmd->opcode);
err_out:
    if (rc){
        if(rc == -RTK_RMOV)
            cmd->retries = 0;

        cmd->error = -MMC_BLK_CMD_ERR;
    }
    rtkemmc_req_end_tasklet(emmc_port); 	
    //tasklet_schedule(&emmc_port->req_end_tasklet);
}

static void rtkemmc_request(struct mmc_host *host, struct mmc_request *mrq)
{
    struct rtkemmc_host *emmc_port;
    struct mmc_command *cmd;
    unsigned long flags;
#ifdef USE_TMP_BUF
    unsigned char *tmp_buf=NULL;
#endif
    MMCPRINTF("%s \n", __func__);

    emmc_port = mmc_priv(host);
    BUG_ON(emmc_port->mrq != NULL);

    //spin_lock(&emmc_port->lock);
    down_write(&cr_rw_sem);
    cmd = mrq->cmd;
    emmc_port->mrq = mrq;

    if (!(emmc_port->rtflags & RTKCR_FCARD_DETECTED)){
        cmd->error = -MMC_BLK_NOMEDIUM;
        cmd->retries = 0;
        goto done;
    }

/*
 * the "tmp_buf" should used for sg_init_one() in blksz small than 512 byte case.
 * because RTK DMA HW issue,the samllest DMA size is 512 byte
 */
#ifdef USE_TMP_BUF
    if (cmd->data) {

        if(cmd->data->blksz < 512){
            WARN_ON(1);
            printk("blksz=%d\n",cmd->data->blksz);

            tmp_buf = dma_alloc_coherent(emmc_port->dev, 512,
                    &emmc_port->tmp_buf_phy_addr, GFP_KERNEL);

            if(!tmp_buf){
                WARN_ON(1);
                cmd->error = -ENOMEM;
                goto done;
            }
            memset(tmp_buf,0,512);
            emmc_port->tmp_buf = tmp_buf;
        }else{
            emmc_port->tmp_buf = NULL;
        }
    }
#endif  //#ifdef USE_TMP_BUF

    if ( emmc_port && cmd ){
        rtkemmc_allocate_dma_buf(emmc_port, cmd);
        rtkemmc_send_command(emmc_port, cmd);
    }else{
done:
	rtkemmc_req_end_tasklet(emmc_port); 	

        //tasklet_schedule(&emmc_port->req_end_tasklet);
    }
        up_write(&cr_rw_sem);
	if(cmd_resend==1) {
                        cmd_resend=0;
                        down_speed_handling(emmc_port);
        }
        //spin_unlock(&emmc_port->lock);
}

static int rtkemmc_execute_tuning(struct mmc_host *host, u32 opcode)
{
    struct rtkemmc_host *emmc_port;
    unsigned long flags;
    struct sd_cmd_pkt cmd_info;
    MMCPRINTF("%s \n", __func__);
	
    emmc_port = mmc_priv(host);
    memset(&cmd_info, 0, sizeof(struct sd_cmd_pkt));

    cmd_info.emmc_port = emmc_port;

	if (host->card){
	printk(KERN_INFO "emmc card manid = 0x%08x\n", host->card->cid.manfid);
	  if (host->card->cid.manfid == 0x13){ //micron manfid
			emmc_port->rx_tuning = 1; //micron: force to turn on rx tuning
	  }
	}
	else{
		printk(KERN_ERR "host->card is null! \n");
	}

    MMCPRINTF("rtkemmc_execute_tuning : opcode=0x%08x, mode=0x%08x\n",opcode,host->mode);
    rtkemmc_set_wrapper_div(emmc_port, 0);

    g_bTuning = 1;
    switch(host->mode)
    {
	case MODE_SDR:
		mmc_Tuning_SDR50(emmc_port);
		set_RTK_initial_flag(1);
		break;
	case MODE_DDR:
		mmc_Tuning_DDR50(emmc_port);
		set_RTK_initial_flag(1);
		break;
	case MODE_HS200:
		mmc_Tuning_HS200(emmc_port,host->mode);
		set_RTK_initial_flag(1);
                break;
	case MODE_HS400:
		mmc_Tuning_HS400(emmc_port,host->mode);
		break;
	default:
		set_RTK_initial_flag(1);
    } 
	g_bTuning = 0;
    return 0;
}

static int rtkemmc_prepare_hs400_tuning(struct mmc_host *host, struct mmc_ios *ios)
{
	unsigned long flags2;
        struct rtkemmc_host *emmc_port;
        emmc_port = mmc_priv(host);
        printk(KERN_ERR "Prepare HS400 mode...\n");
        rtk_lockapi_lock2(flags2, _at_("rtkemmc_prepare_hs400_tuning"));
        rtkemmc_writel(0x88 ,emmc_port->emmc_membase + EMMC_DQS_CTRL1);
        rtk_lockapi_unlock2(flags2, _at_("rtkemmc_prepare_hs400_tuning"));
	return 0;
}
static void rtkemmc_set_ios(struct mmc_host *host, struct mmc_ios *ios)
{
    struct rtkemmc_host *emmc_port;
    u32 tmp_clock, busmode=0;
    unsigned long flags2;
    u32 cur_timing = 0;

    emmc_port = mmc_priv(host);

    MMCPRINTF(KERN_INFO "ios->bus_mode = %u\n",ios->bus_mode);
    MMCPRINTF(KERN_INFO "ios->clock = %u\n",ios->clock);
    MMCPRINTF(KERN_INFO "ios->bus_width = %u\n",ios->bus_width);
    MMCPRINTF(KERN_INFO "ios->timing = %u\n",ios->timing);

    //down_write(&cr_rw_sem); 
    cur_timing = ios->timing;
    if (ios->clock == MMC_HIGH_52_MAX_DTR)
	cur_timing = MMC_TIMING_MMC_HS;

    MMCPRINTF(KERN_INFO "cur_timing = %u\n",cur_timing);

    if (!g_bResuming)
    {
      switch(cur_timing)
      {
	case MMC_TIMING_MMC_HS400:
		rtkemmc_set_freq(emmc_port,0xa6);  //200MHZ
             	rtkemmc_set_ip_div(emmc_port,EMMC_CLOCK_DIV_NON);
		isb();
        	sync(emmc_port);
		rtk_lockapi_lock2(flags2, _at_("rtkemmc_set_ios"));
		rtkemmc_writel(0x80000000 ,emmc_port->emmc_membase + EMMC_DDR_REG); //enable HS400
		rtk_lockapi_unlock2(flags2, _at_("rtkemmc_set_ios"));
		isb();
        	sync(emmc_port);
            	printk(KERN_INFO "%s - enable HS400, DDR_REG:0x%08x\n",__func__,readl(emmc_port->emmc_membase + EMMC_DDR_REG));
		break;
	case MMC_TIMING_MMC_HS200:
		rtkemmc_set_freq(emmc_port,0xa6);  //200MHZ
            	rtkemmc_set_ip_div(emmc_port,EMMC_CLOCK_DIV_NON);
		break;
	case MMC_TIMING_UHS_DDR50: 
		rtkemmc_set_freq(emmc_port,0x57);  //100Mhz
           	rtkemmc_set_ip_div(emmc_port,EMMC_CLOCK_DIV_NON); //100MHZ/1 = 100MHZ
		break;
	case MMC_TIMING_MMC_HS:
		rtkemmc_set_freq(emmc_port,0x57);  //100Mhz
		rtkemmc_set_ip_div(emmc_port,EMMC_CLOCK_DIV_2); //100MHZ/2 = 50MHZ
		break;
	case MMC_TIMING_LEGACY:    	
		rtkemmc_set_freq(emmc_port,0x46);  //80Mhz
		rtkemmc_set_ip_div(emmc_port,EMMC_CLOCK_DIV_256);
		break;
	default:
		printk(KERN_INFO "%s: cur_timing = %u \n", __FILE__, cur_timing);
		break;
      }
    }

   if (ios->bus_width == MMC_BUS_WIDTH_8){
        printk(KERN_INFO "set bus width 8\n");
        rtkemmc_set_bits(emmc_port,BUS_WIDTH_8);
        busmode = BUS_WIDTH_8;
	rtk_lockapi_lock2(flags2, _at_("rtkemmc_set_ios"));
        if (cur_timing == MMC_TIMING_MMC_HS400)
    	{
   		rtkemmc_writel(0x00010001, emmc_port->emmc_membase + EMMC_UHSREG); //DDR
		isb();
       		sync(emmc_port);
		rtkemmc_writel(0x88 ,emmc_port->emmc_membase + EMMC_DQS_CTRL1); //dqs dly tap
		isb();
       		sync(emmc_port);
		//rtkemmc_dump_registers(emmc_port);
       		sync(emmc_port);
		printk(KERN_INFO "HS400 : set DDR mode and dqs delay tap EMMC_UHSREG=%x\n", readl(emmc_port->emmc_membase + EMMC_UHSREG));
   	}
	else {
    		rtkemmc_writel(0x00000001, emmc_port->emmc_membase + EMMC_UHSREG); //non-DDR, such as SDR / HS200
	}
	rtk_lockapi_unlock2(flags2, _at_("rtkemmc_set_ios"));
    } else if (ios->bus_width == MMC_BUS_WIDTH_4){
       	printk(KERN_INFO "set bus width 4\n");
        rtkemmc_set_bits(emmc_port,BUS_WIDTH_4);
        busmode = BUS_WIDTH_4;
    }
    //up_write(&cr_rw_sem);
}

static void rtkemmc_dump_registers(struct rtkemmc_host *emmc_port)
{
    printk(KERN_INFO "%s : \n", __func__);
	printk(KERN_INFO "EMMC_muxpad0 = 0x%08x \n", readl(emmc_port->emmc_membase + EMMC_muxpad0));
	printk(KERN_INFO "EMMC_muxpad1 = 0x%08x \n", readl(emmc_port->emmc_membase + EMMC_muxpad1));
	printk(KERN_INFO "EMMC_muxpad2 = 0x%08x \n", readl(emmc_port->emmc_membase + EMMC_muxpad2));
	printk(KERN_INFO "EMMC_PFUNC_NF1 = 0x%08x \n", readl(emmc_port->emmc_membase + EMMC_PFUNC_NF1));
	printk(KERN_INFO "EMMC_PFUNC_CR  = 0x%08x \n", readl(emmc_port->emmc_membase + EMMC_PFUNC_CR));
	printk(KERN_INFO "EMMC_PDRIVE_NF1 = 0x%08x \n", readl(emmc_port->emmc_membase + EMMC_PDRIVE_NF1));
	printk(KERN_INFO "EMMC_PDRIVE_NF2 = 0x%08x \n", readl(emmc_port->emmc_membase + EMMC_PDRIVE_NF2));
	printk(KERN_INFO "EMMC_PDRIVE_NF3 = 0x%08x \n", readl(emmc_port->emmc_membase + EMMC_PDRIVE_NF3));
	printk(KERN_INFO "EMMC_CTYPE = 0x%08x \n", readl(emmc_port->emmc_membase + EMMC_CTYPE));
	printk(KERN_INFO "EMMC_UHSREG = 0x%08x \n", readl(emmc_port->emmc_membase + EMMC_UHSREG));
	printk(KERN_INFO "EMMC_DDR_REG = 0x%08x \n", readl(emmc_port->emmc_membase + EMMC_DDR_REG));
	printk(KERN_INFO "EMMC_CARD_THR_CTL = 0x%08x \n", readl(emmc_port->emmc_membase + EMMC_CARD_THR_CTL));
	printk(KERN_INFO "EMMC_CLKDIV = 0x%08x \n", readl(emmc_port->emmc_membase + EMMC_CLKDIV));
	printk(KERN_INFO "EMMC_CKGEN_CTL = 0x%08x \n", readl(emmc_port->emmc_membase + EMMC_CKGEN_CTL));
	printk(KERN_INFO "EMMC_DQS_CTLR1 = 0x%08x \n", readl(emmc_port->emmc_membase + EMMC_DQS_CTRL1));
	printk(KERN_INFO "EMMC_PAD_CTL = 0x%08x \n", readl(emmc_port->emmc_membase + EMMC_PAD_CTL));

	isb();
    sync(emmc_port);
}
static void rtkemmc_restore_registers(struct rtkemmc_host *emmc_port)
{
    printk(KERN_INFO "%s : \n", __func__);

	rtkemmc_writel(gRegTbl.emmc_mux_pad0, emmc_port->emmc_membase + EMMC_muxpad0);
	rtkemmc_writel(gRegTbl.emmc_mux_pad1, emmc_port->emmc_membase + EMMC_muxpad1);
	rtkemmc_writel(gRegTbl.emmc_mux_pad2, emmc_port->emmc_membase + EMMC_muxpad2);
	rtkemmc_writel(gRegTbl.emmc_pfunc_nf1, emmc_port->emmc_membase + EMMC_PFUNC_NF1);
	rtkemmc_writel(gRegTbl.emmc_pfunc_cr, emmc_port->emmc_membase + EMMC_PFUNC_CR);
	rtkemmc_writel(gRegTbl.emmc_pdrive_nf1, emmc_port->emmc_membase + EMMC_PDRIVE_NF1);
	rtkemmc_writel(gRegTbl.emmc_pdrive_nf2, emmc_port->emmc_membase + EMMC_PDRIVE_NF2);
	rtkemmc_writel(gRegTbl.emmc_pdrive_nf3, emmc_port->emmc_membase + EMMC_PDRIVE_NF3);
	rtkemmc_writel(gRegTbl.emmc_ctype, emmc_port->emmc_membase + EMMC_CTYPE);
	rtkemmc_writel(gRegTbl.emmc_uhsreg, emmc_port->emmc_membase + EMMC_UHSREG);
	rtkemmc_writel(gRegTbl.emmc_ddr_reg, emmc_port->emmc_membase + EMMC_DDR_REG);
	rtkemmc_writel(gRegTbl.emmc_card_thr_ctl, emmc_port->emmc_membase + EMMC_CARD_THR_CTL);
	rtkemmc_writel(gRegTbl.emmc_clk_div, emmc_port->emmc_membase + EMMC_CLKDIV);
	rtkemmc_writel(gRegTbl.emmc_ckgen_ctl, emmc_port->emmc_membase + EMMC_CKGEN_CTL);	
	rtkemmc_writel(gRegTbl.emmc_dqs_ctrl1, emmc_port->emmc_membase + EMMC_DQS_CTRL1);	

    isb();
	sync(emmc_port);
    rtkemmc_dump_registers(emmc_port);
}
static void rtkemmc_backup_registers(struct rtkemmc_host *emmc_port)
{
    printk(KERN_INFO "%s : \n", __func__);
	
   	gRegTbl.emmc_mux_pad0 = readl(emmc_port->emmc_membase + EMMC_muxpad0);
   	gRegTbl.emmc_mux_pad1 = readl(emmc_port->emmc_membase + EMMC_muxpad1);
	gRegTbl.emmc_mux_pad2 = readl(emmc_port->emmc_membase + EMMC_muxpad2);
	gRegTbl.emmc_pfunc_nf1 = readl(emmc_port->emmc_membase + EMMC_PFUNC_NF1);
	gRegTbl.emmc_pfunc_cr = readl(emmc_port->emmc_membase + EMMC_PFUNC_CR);
	gRegTbl.emmc_pdrive_nf1 = readl(emmc_port->emmc_membase + EMMC_PDRIVE_NF1);
	gRegTbl.emmc_pdrive_nf2 = readl(emmc_port->emmc_membase + EMMC_PDRIVE_NF2);
	gRegTbl.emmc_pdrive_nf3 = readl(emmc_port->emmc_membase + EMMC_PDRIVE_NF3);
	gRegTbl.emmc_pdrive_nf4 = readl(emmc_port->emmc_membase + EMMC_PDRIVE_NF4);
	gRegTbl.emmc_pdrive_cr0 = readl(emmc_port->emmc_membase + EMMC_PDRIVE_CR0);
	gRegTbl.emmc_pdrive_cr1 = readl(emmc_port->emmc_membase + EMMC_PDRIVE_CR1);
	gRegTbl.emmc_pdrive_sdio = readl(emmc_port->emmc_membase + EMMC_PDRIVE_SDIO);
	gRegTbl.emmc_pdrive_sdio1 = readl(emmc_port->emmc_membase + EMMC_PDRIVE_SDIO1);
	gRegTbl.emmc_ctype = readl(emmc_port->emmc_membase + EMMC_CTYPE);
	gRegTbl.emmc_uhsreg = readl(emmc_port->emmc_membase + EMMC_UHSREG);
	gRegTbl.emmc_ddr_reg = readl(emmc_port->emmc_membase + EMMC_DDR_REG);
	gRegTbl.emmc_card_thr_ctl = readl(emmc_port->emmc_membase + EMMC_CARD_THR_CTL);
	gRegTbl.emmc_clk_div = readl(emmc_port->emmc_membase + EMMC_CLKDIV);
	gRegTbl.emmc_ckgen_ctl = readl(emmc_port->emmc_membase + EMMC_CKGEN_CTL);
	gRegTbl.emmc_dqs_ctrl1 = readl(emmc_port->emmc_membase + EMMC_DQS_CTRL1);

	isb();
	sync(emmc_port);
    rtkemmc_dump_registers(emmc_port);
}

static void rtkemmc_chk_card_insert(struct rtkemmc_host *emmc_port)
{
    struct mmc_host *host=emmc_port->mmc;
	unsigned int reg = 0;

    MMCPRINTF("%s : \n", __func__);
	rtkemmc_writel(0x00000000 ,emmc_port->emmc_membase + EMMC_DDR_REG); //restore to default
	rtkemmc_writel(0, emmc_port->emmc_membase + EMMC_DQS_CTRL1);

#ifdef PHASE_INHERITED
	if (VP0_saved == 0xFF && VP1_saved == 0xFF){
		VP0_saved = (readl(emmc_port->crt_membase + SYS_PLL_EMMC1) & 0x000000f8) >> 3;
		VP1_saved = (readl(emmc_port->crt_membase + SYS_PLL_EMMC1) & 0x00001f00) >> 8;
	}
#endif

#ifdef DQS_INHERITED
	if (dqs_saved == 0xff)
		dqs_saved = readl(emmc_port->emmc_membase + EMMC_DQS_CTRL1);
#endif
	rtkemmc_writel(3, emmc_port->crt_membase + SYS_PLL_EMMC1);
        isb();
        sync(emmc_port);

        rtkemmc_writel(0x00000081, emmc_port->emmc_membase + EMMC_BMOD);
        isb();
        sync(emmc_port);
        rtkemmc_writel(0x02000000, emmc_port->emmc_membase + EMMC_CTRL);
        isb();
        sync(emmc_port);
        rtkemmc_writel(0x00000001, emmc_port->emmc_membase + EMMC_PWREN);
        isb();
        sync(emmc_port);
        rtkemmc_writel(0x0000ffce, emmc_port->emmc_membase + EMMC_INTMASK);
        isb();
        sync(emmc_port);
        rtkemmc_writel(0xffffffff, emmc_port->emmc_membase + EMMC_RINTSTS);
        isb();
        sync(emmc_port);
        rtkemmc_writel(0x00000080, emmc_port->emmc_membase + EMMC_CLKDIV);
        isb();
        sync(emmc_port);
        rtkemmc_writel(0x00000000, emmc_port->emmc_membase + EMMC_CLKSRC);
        isb();
        sync(emmc_port);
        rtkemmc_writel(0x0001ffff, emmc_port->emmc_membase + EMMC_CLKENA);
        isb();
        sync(emmc_port);
        rtkemmc_writel(0xa0200000, emmc_port->emmc_membase + EMMC_CMD);
        isb();
        sync(emmc_port);
		
        MMCPRINTF(KERN_INFO "1 EMMC_TMOUT = 0x%08x \n", readl(emmc_port->emmc_membase + EMMC_TMOUT));
        rtkemmc_writel(0xffffff40, (volatile void*)(emmc_port->emmc_membase + EMMC_TMOUT));
        isb();
        sync(emmc_port);
		MMCPRINTF(KERN_INFO "2 EMMC_TMOUT = 0x%08x \n",  (volatile void*)readl(emmc_port->emmc_membase + EMMC_TMOUT));

		rtkemmc_writel(0x00000000, emmc_port->emmc_membase + EMMC_CTYPE);
        isb();
        sync(emmc_port);
        rtkemmc_writel(0x0001007f, emmc_port->emmc_membase + EMMC_FIFOTH); // original: 0x0007f007f
        isb();
        sync(emmc_port);
        rtkemmc_writel(0x02000010, emmc_port->emmc_membase + EMMC_CTRL);
        isb();
        sync(emmc_port);
        rtkemmc_writel(0x00000080, emmc_port->emmc_membase + EMMC_BMOD);
        isb();
        sync(emmc_port);
        rtkemmc_writel(0x0000ffce, emmc_port->emmc_membase + EMMC_INTMASK);
        isb();
        sync(emmc_port);
        //rtkemmc_writel(0x00600000, emmc_port->emmc_membase + EMMC_DBADDR);
        rtkemmc_writel(0x00000000, emmc_port->emmc_membase + EMMC_IDINTEN);
        isb();
        sync(emmc_port);
        rtkemmc_writel(0x0000ffff, emmc_port->emmc_membase + EMMC_RINTSTS);
        isb();
        sync(emmc_port);
        rtkemmc_writel(0x00000001, emmc_port->emmc_membase + EMMC_UHSREG);
        isb();
        sync(emmc_port);
	rtkemmc_writel(0x02000001, emmc_port->emmc_membase + EMMC_CARD_THR_CTL);
	isb();
	sync(emmc_port);

	//Disalbe SSC
	//rtkemmc_writel( 0x4511893, emmc_port->crt_membase + SYS_PLL_EMMC2);
	//isb();
	//sync(emmc_port);

#ifdef EMMC_LA_DEBUG_GPIO
	//gpio 100 pin mux
	reg = readl(emmc_port->emmc_membase + EMMC_muxpad2);
	reg &= (~0x00003000);
	rtkemmc_writel(reg, emmc_port->emmc_membase + EMMC_muxpad2);

	//gpio 100 dir
	reg = readl(emmc_port->misc_membase + MISC_GP3DIR);
	reg |= (0x00000010);
	rtkemmc_writel(reg, emmc_port->misc_membase + MISC_GP3DIR);

	//gpio 100  pull  high
	reg = readl(emmc_port->misc_membase + MISC_GP3DATO);
	reg |= (0x00000010);
	rtkemmc_writel(reg, emmc_port->misc_membase + MISC_GP3DATO);
	isb();
	sync(emmc_port);

	//gpio 90 pin mux
	reg = readl(emmc_port->emmc_membase + EMMC_muxpad1);
	reg &= (~0x0000000c);
	rtkemmc_writel(reg, emmc_port->emmc_membase + EMMC_muxpad1);

	//gpio 90 dir
	reg = readl(emmc_port->misc_membase + MISC_GP2DIR);
	reg |= (0x04000000);
	rtkemmc_writel(reg, emmc_port->misc_membase + MISC_GP2DIR);	

	//gpio 90  pull  high
	reg = readl(emmc_port->misc_membase + MISC_GP2DATO);
	reg |= (0x04000000);
	rtkemmc_writel(reg, emmc_port->misc_membase + MISC_GP2DATO);
	isb();
	sync(emmc_port);

#endif

//switch phase
#if 0
	//change clock to 4MHz
	reg = readl(emmc_port->emmc_membase + EMMC_CKGEN_CTL);
	rtkemmc_writel(reg|0x70000, emmc_port->emmc_membase + EMMC_CKGEN_CTL);
	isb();
	sync(emmc_port);

	//reset PLL
	reg = readl(emmc_port->crt_membase + SYS_PLL_EMMC1);
	rtkemmc_writel(reg&0xfffffffd, emmc_port->crt_membase + SYS_PLL_EMMC1);
	isb();
	sync(emmc_port);

	//write phase
	rtkemmc_writel(0x903, emmc_port->crt_membase + SYS_PLL_EMMC1);
	isb();
	sync(emmc_port);

	//release reset PLL
	reg = readl(emmc_port->crt_membase + SYS_PLL_EMMC1);
	rtkemmc_writel(reg|0x2, emmc_port->crt_membase + SYS_PLL_EMMC1);
	isb();
	sync(emmc_port);

	//change clock to PLL
	reg = readl(emmc_port->emmc_membase + EMMC_CKGEN_CTL);
	rtkemmc_writel(reg&0xfff8ffff, emmc_port->emmc_membase + EMMC_CKGEN_CTL);
	isb();
	sync(emmc_port);
#endif
    rtkemmc_set_bits(emmc_port,BUS_WIDTH_1);
    rtkemmc_set_pad_driving(emmc_port, 0x33, 0x33, 0x33, 0x33);
    rtkemmc_set_wrapper_div(emmc_port, 0);
    rtkemmc_set_ip_div(emmc_port, EMMC_CLOCK_DIV_256);
    phase(emmc_port, 0, 0); //VP0, VP1 phas
    rtkemmc_set_freq(emmc_port,0x46);  //80Mhz / 256 ~= 300K
    sync(emmc_port);
    //host_card_stop(emmc_port); //card_stop here will send CMD0 to result side effect on resuming
    sync(emmc_port);
    host->ops = &rtkemmc_ops;
    emmc_port->rtflags |= RTKCR_FCARD_DETECTED;
    MMCPRINTF("%s : \n", __func__);
    rtkemmc_dump_registers(emmc_port);
}

static void rtkemmc_timeout_timer(unsigned long data)
{
    struct rtkemmc_host *emmc_port;
    unsigned long flags;

    emmc_port = (struct rtkemmc_host *)data;
    MMCPRINTF("rtkemmc_timeout_timer fired ...\n");
    MMCPRINTF("%s - int_wait=%08x\n", __func__, emmc_port->int_waiting);
    spin_lock_irqsave(&emmc_port->lock,flags);
    //down_write(&cr_rw_sem);
#ifdef ENABLE_EMMC_INT_MODE
    if(emmc_port->int_waiting)
#else
    if (1)
#endif
    {
        MMCPRINTF("1. get sd trans \n");
#ifdef ENABLE_EMMC_INT_MODE
        MMCPRINTF("========== C1 ==========\n");
        rtkemmc_hold_int_dec();
		rtkemmc_get_int_sta(&emmc_port->rintsts, &emmc_port->mintsts, &emmc_port->dma_isr);
	    rtkemmc_get_sta(&emmc_port->status, &emmc_port->idsts);
	    rtkemmc_get_mask(&emmc_port->idinten, &emmc_port->intmask);
        sync(emmc_port);
#endif

    	MMCPRINTF("rintsts =%08x, mintsts=0x%08x, int_waiting=%08x\n", emmc_port->rintsts, emmc_port->mintsts, emmc_port->int_waiting);
        MMCPRINTF("status=0x%08x idsts=0x%8x\n",emmc_port->status ,emmc_port->idsts);

    }else{
        WARN_ON(1);
    }
        
    if(emmc_port->int_waiting)
        rtk_op_complete(emmc_port);

    spin_unlock_irqrestore(&emmc_port->lock, flags);
    //up_write(&cr_rw_sem);
}

static irqreturn_t rtkemmc_irq(int irq, void *dev)
{
    struct rtkemmc_host *emmc_port = dev;

    int irq_handled = 0;
	u8 cmd = emmc_port->cmd_opcode;
    unsigned long flags;
    unsigned long flags2;
	u32 reg;
    MMCPRINTF("rtkemmc_irq interrupted ...\n");

    rtk_lockapi_lock2(flags2, _at_("rtkemmc_irq"));

    rtkemmc_get_int_sta(&emmc_port->rintsts, &emmc_port->mintsts, &emmc_port->dma_isr);
    rtkemmc_get_sta(&emmc_port->status, &emmc_port->idsts);
    rtkemmc_get_mask(&emmc_port->idinten, &emmc_port->intmask);
	sync(emmc_port);

#if 0 	//only for debugging 
    if (emmc_port->rintsts & INT_STS_ERRORS)
    {

	printk(KERN_ERR "EMMC IP_DESC0 = 0x%08x\n", readl(emmc_port->emmc_membase+EMMC_IP_DESC0));
        printk(KERN_ERR "EMMC IP_DESC1 = 0x%08x\n", readl(emmc_port->emmc_membase+EMMC_IP_DESC1));
        printk(KERN_ERR "EMMC IP_DESC2 = 0x%08x\n", readl(emmc_port->emmc_membase+EMMC_IP_DESC2));
        printk(KERN_ERR "EMMC IP_DESC3 = 0x%08x\n", readl(emmc_port->emmc_membase+EMMC_IP_DESC3));
        printk(KERN_ERR "EMMC BYTCNT = 0x%08x\n", readl(emmc_port->emmc_membase+EMMC_BYTCNT));
        printk(KERN_ERR "EMMC CMDARG = 0x%08x\n", readl(emmc_port->emmc_membase+EMMC_CMDARG));
        printk(KERN_ERR "EMMC DBADDR = 0x%08x\n", readl(emmc_port->emmc_membase+EMMC_DBADDR));

        printk(KERN_ERR "EMMC IDSTS(208C) = 0x%08x\n", emmc_port->idsts);
        printk(KERN_ERR "EMMC IDINTEN(2090) = 0x%08x\n", emmc_port->idinten);
        printk(KERN_ERR "EMMC INTMASK(2024) = 0x%08x\n", emmc_port->intmask);
        printk(KERN_ERR "EMMC MINTSTS(2040) = 0x%08x\n", emmc_port->mintsts);
        printk(KERN_ERR "EMMC STATUS(2048) = 0x%08x\n", emmc_port->status);
        printk(KERN_ERR "EMMC ISR(2424) = 0x%08x\n", emmc_port->dma_isr);
        printk(KERN_ERR "EMMC RINTSTS(2044) = 0x%08x\n", emmc_port->rintsts);

	BUG();	
    }
#endif		
    //down_write(&cr_rw_sem);
    //spin_lock(&emmc_port->lock);

	MMCPRINTF("========== I.1 ==========\n");
    MMCPRINTF("rintsts =%08x, mintsts=0x%08x, int_waiting=%08x\n", emmc_port->rintsts, emmc_port->mintsts, emmc_port->int_waiting);
	MMCPRINTF("intmask =%08x, isr=0x%08x\n", readl(emmc_port->emmc_membase+EMMC_INTMASK), emmc_port->dma_isr);   
    rtkemmc_hold_int_dec();

    rtk_lockapi_unlock2(flags2, _at_("rtkemmc_irq"));

    MMCPRINTF("we get int end \n");

#ifdef  SHOW_INT_STATUS
    printk("status=0x%08x idsts=0x%8x\n",emmc_port->status,emmc_port->idsts);
#endif
    if(emmc_port->int_waiting){
        MMCPRINTF("int wait clear 1\n");
        del_timer(&emmc_port->timer);
		if (g_bResuming) //avoid to emit complete in rtkemmc_resume
			return IRQ_HANDLED;
        rtk_op_complete(emmc_port);
        MMCPRINTF("int wait clear 2\n");
    }else{
        MMCPRINTF("No int_waiting!!!\n");
    }

    irq_handled = 1;
	sync(emmc_port);

    //up_write(&cr_rw_sem);
    //spin_unlock(&emmc_port->lock);
    
    if(irq_handled)
        return IRQ_HANDLED;
    else
        return IRQ_NONE;
}

//luis, TBD
static int rtkemmc_get_ro(struct mmc_host *mmc)
{
    return 0;
}

static int rtkemmc_wait_status(struct mmc_card *card,u8 state,u8 divider,int bIgnore)
{
    struct mmc_command cmd;
    struct sd_cmd_pkt cmd_info;
    unsigned long timeend;
    int err, bMalloc=0;
    struct mmc_host * mmc = mmc_host_local;

    MMCPRINTF("\n");
    timeend = jiffies + msecs_to_jiffies(100);    /* wait 100ms */
    
    if (card == NULL)
    {
	bMalloc=1;
	card = (struct mmc_card*)kmalloc(sizeof(struct mmc_card),GFP_KERNEL);
	card->host = mmc;
    }

    do {
        memset(&cmd, 0, sizeof(struct mmc_command));
        memset(&cmd_info, 0, sizeof(struct sd_cmd_pkt));

        set_cmd_info(card,&cmd,&cmd_info,
                     MMC_SEND_STATUS,
                     (card->rca)<<RCA_SHIFTER,
                     6);
        err = SD_SendCMDGetRSP_Cmd(&cmd_info,bIgnore);

        if(err){
	    if (!bIgnore)
            	printk(KERN_INFO "wait %s fail\n",state_tlb[state]);
            break;
        }else{
            u8 cur_state = R1_CURRENT_STATE(cmd.resp[0]);
	    if (!bIgnore)
            	MMCPRINTF(KERN_WARNING "resp[0]=0x%08x,cur_state=%s\n",cmd.resp[0],state_tlb[cur_state]);
            err = -1;
            if(cur_state == state){
                if(cmd.resp[0] & R1_READY_FOR_DATA){
                    err = 0;
                    break;
                }
            }
        }

    }while(time_before(jiffies, timeend));  

    if (bMalloc)
    {
	kfree(card);
	card = NULL;
    }
    return err;
}

static int rtkemmc_send_cmd13(struct rtkemmc_host *emmc_port, u16 * state)
{
    struct mmc_command cmd;
    struct sd_cmd_pkt cmd_info;
    int err=0;
    int bMalloc=0;
    struct mmc_host * mmc = mmc_host_local;

    memset(&cmd, 0, sizeof(struct mmc_command));
    memset(&cmd_info, 0, sizeof(struct sd_cmd_pkt));

    cmd.opcode         = MMC_SEND_STATUS;
    cmd.arg            = (1<<RCA_SHIFTER);
    cmd_info.cmd       = &cmd;
    cmd_info.emmc_port = emmc_port;
    cmd_info.rsp_len   = 6;

    gPreventRetry=1;
    err = SD_SendCMDGetRSP(&cmd_info,1);
    gPreventRetry=0;

    if(err){
        mmcmsg3(KERN_WARNING "%s: MMC_SEND_STATUS fail\n",DRIVER_NAME);
    }else{
        u8 cur_state = R1_CURRENT_STATE(cmd.resp[0]);
        *state = cur_state;
        mmcmsg1("cur_state=%s\n",state_tlb[cur_state]);
    }

    return err;
}

static int rtkemmc_send_status(struct mmc_card *card,u16 * state,u8 divider,int bIgnore)
{
    struct mmc_command cmd;
    struct sd_cmd_pkt cmd_info;
    int err=0;
    int bMalloc=0;
    struct mmc_host * mmc = mmc_host_local;
    struct rtkemmc_host *emmc_port = mmc_priv(mmc);

    memset(&cmd, 0, sizeof(struct mmc_command));
    memset(&cmd_info, 0, sizeof(struct sd_cmd_pkt));

    if (card == NULL)
    {
		bMalloc=1;
		card = (struct mmc_card*)kmalloc(sizeof(struct mmc_card),GFP_KERNEL);
		card->host = mmc;
    }
	
    set_cmd_info(card,&cmd,&cmd_info,
                 MMC_SEND_STATUS,
                 (card->rca)<<RCA_SHIFTER,
                 6);
    err = SD_SendCMDGetRSP(&cmd_info,bIgnore);

    if (bMalloc)
    {
		kfree(card);
		card = NULL;
    }

    if(err){
        mmcmsg3(KERN_WARNING "%s: MMC_SEND_STATUS fail\n",DRIVER_NAME);
    }else{
        u8 cur_state = R1_CURRENT_STATE(cmd.resp[0]);
        *state = cur_state;
	if (!bIgnore)
    	printk(KERN_INFO "cur_state=%s\n",state_tlb[cur_state]);
    }

    return err;
}

static int rtkemmc_switch_user_partition(struct mmc_card *card)
{
    struct mmc_command cmd;
    struct sd_cmd_pkt cmd_info;
    int err = 0;

    MMCPRINTF("\n");

    memset(&cmd, 0, sizeof(struct mmc_command));
    memset(&cmd_info, 0, sizeof(struct sd_cmd_pkt));

    set_cmd_info(card,&cmd,&cmd_info,
                 MMC_SWITCH,
                 0x03b30001,
                 6);
    err = SD_SendCMDGetRSP_Cmd(&cmd_info,0);

    if(err){
        mmcmsg3(KERN_WARNING "%s: MMC_SWITCH fail\n",DRIVER_NAME);
    }
    return err;

}

int rtkemmc_switch(struct mmc_card *card,
                        u8 acc_mod,
                        u8 index,
                        u8 value,
                        u8 cmd_set)
{
    struct mmc_command cmd;
    struct sd_cmd_pkt cmd_info;
    u32 arg = 0;
    int err = 0;
    int bMalloc=0;
    struct mmc_host *mmc = mmc_host_local;

    memset(&cmd, 0, sizeof(struct mmc_command));
    memset(&cmd_info, 0, sizeof(struct sd_cmd_pkt));

    if (card == NULL)
    {
	bMalloc=1;
	card = (struct mmc_card*)kmalloc(sizeof(struct mmc_card),GFP_KERNEL);
	card->host = mmc;
    }

    arg = (acc_mod << 24) |
          (index <<16) |
          (value << 8) |
          (cmd_set);

    MMCPRINTF("%s : arg=0x%08x\n",__func__,arg);
    set_cmd_info(card,&cmd,&cmd_info,
                 MMC_SWITCH,
                 arg,
                 6);

    err = SD_SendCMDGetRSP_Cmd(&cmd_info,0);
    if (bMalloc)
    {
	kfree(card);
	card = NULL;
    }

    if(err){
        mmcmsg3(KERN_WARNING "%s: MMC_SWITCH fail\n",DRIVER_NAME);
    }
    return err;

}
#ifdef CONFIG_MMC_RTKEMMC_HK_ATTR
//#define HACK_BOOT_PART_RW

#define NORMAL_PART 0
#define BOOT1_PART  1
#define BOOT2_PART  2
#define GP1_PART    3
#define GP2_PART    4
#define GP3_PART    5
#define GP4_PART    6

#ifdef HACK_BOOT_PART_RW
static int rtkemmc_switch_partition(struct mmc_card *card,u8 acc_part)
{

    rtkemmc_switch(card,
                 MMC_SWITCH_MODE_WRITE_BYTE,
                 EXT_CSD_PART_CONFIG,
                 acc_part,
                 EXT_CSD_CMD_SET_NORMAL);

    return 0;

}
#endif
#endif

/****************************************************************
 *  tuning area
 * 
****************************************************************/
void mmc_host_reset(struct rtkemmc_host *emmc_port)
{
	struct mmc_host *host=emmc_port->mmc;
	unsigned int reg = 0;


	MMCPRINTF("%s : \n", __func__);
		rtkemmc_writel(0x00000081, emmc_port->emmc_membase + EMMC_BMOD);
		isb();
		sync(emmc_port);
		//rtkemmc_writel(0x00000001, emmc_port->emmc_membase + EMMC_CTRL);
		isb();
		sync(emmc_port);
		rtkemmc_writel(0x02000000, emmc_port->emmc_membase + EMMC_CTRL);
		isb();
		sync(emmc_port);
		rtkemmc_writel(0x00000001, emmc_port->emmc_membase + EMMC_PWREN);
		isb();
		sync(emmc_port);
		rtkemmc_writel(0x0000ffce, emmc_port->emmc_membase + EMMC_INTMASK);
		isb();
		sync(emmc_port);
		rtkemmc_writel(0xffffffff, emmc_port->emmc_membase + EMMC_RINTSTS);
		isb();
		sync(emmc_port);
		//rtkemmc_writel(0x00000010, emmc_port->emmc_membase + EMMC_CTRL);
		isb();
		sync(emmc_port);

		rtkemmc_writel(0x00000080, emmc_port->emmc_membase + EMMC_CLKDIV);
		isb();
		sync(emmc_port);
		rtkemmc_writel(0x00000000, emmc_port->emmc_membase + EMMC_CLKSRC);
		isb();
		sync(emmc_port);
		//rtkemmc_writel(0x0001ffff, emmc_port->emmc_membase + EMMC_CLKENA);
		rtkemmc_writel(0x00010001, emmc_port->emmc_membase + EMMC_CLKENA);
		isb();
		sync(emmc_port);
		rtkemmc_writel(0xa0200000, emmc_port->emmc_membase + EMMC_CMD);
		isb();
		sync(emmc_port);
		
		MMCPRINTF(KERN_INFO "1 EMMC_TMOUT = 0x%08x \n", readl(emmc_port->emmc_membase + EMMC_TMOUT));
		rtkemmc_writel(0xffffff40, (volatile void*)(emmc_port->emmc_membase + EMMC_TMOUT));
		isb();
		sync(emmc_port);
		MMCPRINTF(KERN_INFO "2 EMMC_TMOUT = 0x%08x \n",  (volatile void*)readl(emmc_port->emmc_membase + EMMC_TMOUT));

		rtkemmc_writel(0x00000000, emmc_port->emmc_membase + EMMC_CTYPE);
		isb();
		sync(emmc_port);
		rtkemmc_writel(0x0001007f, emmc_port->emmc_membase + EMMC_FIFOTH);
		isb();
		sync(emmc_port);
		rtkemmc_writel(0x02000010, emmc_port->emmc_membase + EMMC_CTRL);
		isb();
		sync(emmc_port);
		rtkemmc_writel(0x00000080, emmc_port->emmc_membase + EMMC_BMOD);
		isb();
		sync(emmc_port);
		rtkemmc_writel(0x0000ffce, emmc_port->emmc_membase + EMMC_INTMASK);
		isb();
		sync(emmc_port);
		rtkemmc_writel(0x00600000, emmc_port->emmc_membase + EMMC_DBADDR);
		isb();
		sync(emmc_port);
		rtkemmc_writel(0x00000000, emmc_port->emmc_membase + EMMC_IDINTEN);
		isb();
		sync(emmc_port);
		rtkemmc_writel(0x0000ffff, emmc_port->emmc_membase + EMMC_RINTSTS);
		isb();
		sync(emmc_port);
		rtkemmc_writel(0x00000001, emmc_port->emmc_membase + EMMC_UHSREG);
		isb();
		sync(emmc_port);
		rtkemmc_writel(0x02000001, emmc_port->emmc_membase + EMMC_CARD_THR_CTL);
		isb();
		sync(emmc_port);

	MMCPRINTF("%s : \n", __func__);
}

void phase(struct rtkemmc_host *emmc_port, u32 VP0, u32 VP1){
	int time=0x200; //us
//phase selection
	if( (VP0==0xff) & (VP1==0xff)){
		#ifdef DEBUG		
		printk("phase VP0 and VP1 no change \n");
		#endif	
		}
	else if( (VP0!=0xff) & (VP1==0xff)){
		#ifdef DEBUG		
		printk("phase VP0=%x, VP1 no change \n", VP0);
		#endif	
		rtkemmc_writel((readl(emmc_port->emmc_membase + EMMC_CKGEN_CTL)|0x70000), emmc_port->emmc_membase + EMMC_CKGEN_CTL);	//change clock to 4MHz
		sync(emmc_port);	
		rtkemmc_writel((readl(emmc_port->crt_membase + SYS_PLL_EMMC1)&0xfffffffd), emmc_port->crt_membase + SYS_PLL_EMMC1);	//reset pll
		rtkemmc_writel((readl(emmc_port->crt_membase + SYS_PLL_EMMC1)&0xffffff07)|(VP0<<3), emmc_port->crt_membase + SYS_PLL_EMMC1);	//vp0 phase:0x0~0x1f
		rtkemmc_writel((readl(emmc_port->crt_membase + SYS_PLL_EMMC1)|0x2), emmc_port->crt_membase + SYS_PLL_EMMC1);	//release reset pll
		sync(emmc_port);
		udelay(200);
		rtkemmc_writel((readl(emmc_port->emmc_membase + EMMC_CKGEN_CTL)&0xfff8ffff), emmc_port->emmc_membase + EMMC_CKGEN_CTL);	//change clock to PLL
		sync(emmc_port);	
		}
	else if( (VP0==0xff) & (VP1!=0xff)){
		#ifdef DEBUG		
		printk("phase VP0 no change, VP1=%x \n", VP1);
		#endif	
		rtkemmc_writel((readl(emmc_port->emmc_membase + EMMC_CKGEN_CTL)|0x70000), emmc_port->emmc_membase + EMMC_CKGEN_CTL);	//change clock to 4MHz
		rtkemmc_writel((readl(emmc_port->emmc_membase + EMMC_CKGEN_CTL)|0x70000),emmc_port->emmc_membase + EMMC_CKGEN_CTL);	//change clock to 4MHz
		sync(emmc_port);	
		rtkemmc_writel((readl(emmc_port->crt_membase + SYS_PLL_EMMC1)&0xfffffffd), emmc_port->crt_membase + SYS_PLL_EMMC1);	//reset pll
		rtkemmc_writel((readl(emmc_port->crt_membase + SYS_PLL_EMMC1)&0xffffe0ff)|(VP1<<8), emmc_port->crt_membase + SYS_PLL_EMMC1);	//vp1 phase:0x0~0x1f
		rtkemmc_writel((readl(emmc_port->crt_membase + SYS_PLL_EMMC1)|0x2), emmc_port->crt_membase + SYS_PLL_EMMC1);	//release reset pll
		sync(emmc_port);
		udelay(200);
		rtkemmc_writel((readl(emmc_port->emmc_membase + EMMC_CKGEN_CTL)&0xfff8ffff),emmc_port->emmc_membase + EMMC_CKGEN_CTL);	//change clock to PLL
		sync(emmc_port);	
		}
	else{
		#ifdef DEBUG		
		printk("phase VP0=%x, VP1=%x \n", VP0, VP1);
		#endif	
		rtkemmc_writel((readl(emmc_port->emmc_membase + EMMC_CKGEN_CTL)|0x70000),emmc_port->emmc_membase + EMMC_CKGEN_CTL);	//change clock to 4MHz
		sync(emmc_port);	
		rtkemmc_writel((readl(emmc_port->crt_membase + SYS_PLL_EMMC1)&0xfffffffd),emmc_port->crt_membase + SYS_PLL_EMMC1);	//reset pll
		rtkemmc_writel((readl(emmc_port->crt_membase + SYS_PLL_EMMC1)&0xffffff07)|(VP0<<3),emmc_port->crt_membase + SYS_PLL_EMMC1);	//vp0 phase:0x0~0x1f
		rtkemmc_writel((readl(emmc_port->crt_membase + SYS_PLL_EMMC1)&0xffffe0ff)|(VP1<<8),emmc_port->crt_membase + SYS_PLL_EMMC1);	//vp1 phase:0x0~0x1f
		rtkemmc_writel((readl(emmc_port->crt_membase + SYS_PLL_EMMC1)|0x2),emmc_port->crt_membase + SYS_PLL_EMMC1);	//release reset pll
		sync(emmc_port);
		udelay(200);
		rtkemmc_writel((readl(emmc_port->emmc_membase + EMMC_CKGEN_CTL)&0xfff8ffff),emmc_port->emmc_membase + EMMC_CKGEN_CTL);	//change clock to PLL
		sync(emmc_port);
		}

    	printk(KERN_INFO "%s: phase adjust - EMMC_CKGEN_CTL=0x%08x, PLL_EMMC1=%08x, PLL_EMMC2=%08x, PLL_EMMC3=%08x, PLL_EMMC4=%08x\n",
			        DRIVER_NAME, readl(emmc_port->emmc_membase + EMMC_CKGEN_CTL),
			        readl(emmc_port->crt_membase + SYS_PLL_EMMC1),
			        readl(emmc_port->crt_membase + SYS_PLL_EMMC2),
			        readl(emmc_port->crt_membase + SYS_PLL_EMMC3),
			        readl(emmc_port->crt_membase + SYS_PLL_EMMC4));
}

int search_best(u32 window, u32 range)
{
        int i, j, k, max;
        int window_temp[32];
        int window_start[32];
        int window_end[32];
        int window_max=0;
        int window_best=0;
        int parse_end=1;

        for( i=0; i<0x20; i++ ) {
                window_temp[i]=0;
                window_start[i]=0;
                window_end[i]=-1;
        }
        j=1;
        i=0;
        k=0;
        max=0;

	while((i<(range-1)) && (k<(range-1))){
                parse_end=0;
                for( i=window_end[j-1]+1; i<range; i++ ){
                        if (((window>>i)&1)==1 ){
                                window_start[j]=i;
                                break;
                        }
                }
                if( i==range){
                        break;
                }
                for( k=window_start[j]+1; k<range; k++ ){
                        if(((window>>k)&1)==0){
                                window_end[j]=k-1;
                                parse_end=1;
                                break;
                        }
                }
                if(parse_end==0){
                        window_end[j]=range-1;
                }
                j++;
        }
        for(i=1; i<j; i++){
                window_temp[i]= window_end[i]-window_start[i]+1;
        }
        if((((window)&1)==1)&&(((window>>(range-1))&1)==1))
        {
                window_temp[1]=window_temp[1]+window_temp[j-1];
                window_start[1]=window_start[j-1];
        }
        for(i=1; i<j; i++){
                if(window_temp[i]>window_max){
                        window_max=window_temp[i];
                        max=i;
                }
        }
        if(window==0xffffffff){
                window_best=0x10;
        }
        else if((window==0xffff)&&(range==0x10)){
                window_best=0x8;
                }
        else if((((window&1)==1)&&(((window>>(range-1))&1)==1))&&(max==1)){
                window_best=(((window_start[max]+window_end[max]+range)/2)&0x1f);
        }
        else {
                window_best=((window_start[max]+window_end[max])/2)&0x1f;
        }
        return window_best;
}

int check_error(struct rtkemmc_host *emmc_port){
	u32 i;
	u32 error;
	error = readl(emmc_port->emmc_membase + EMMC_RINTSTS);
	isb();
	sync(emmc_port);

	if ((error&INT_STS_EBE)==INT_STS_EBE)
	{
		printk("End bit error \n");
		return INT_STS_EBE;
	}
	else if ((error&INT_STS_SBE_BCI)==INT_STS_SBE_BCI)
	{
		printk("Start bit error \n");
		return INT_STS_SBE_BCI;
	}
	else if ((error&INT_STS_HLE)==INT_STS_HLE)
	{
		printk("Hardware locked write error \n");
		return INT_STS_HLE;
	}
	else if ((error&INT_STS_FRUN)==INT_STS_FRUN)
	{
		printk("FIFO underrun/overrun error \n");
		return INT_STS_FRUN;
	}
	else if ((error&INT_STS_HTO)==INT_STS_HTO)
	{
		printk("Data starvation by host timeout\n");
		return INT_STS_HTO;
	}
	else if ((error&INT_STS_DRTO_BDS)==INT_STS_DRTO_BDS)
	{
		printk("Data read timeout\n");
		return INT_STS_DRTO_BDS;
		}
	else if ((error&INT_STS_RTO_BAR)==INT_STS_RTO_BAR)
	{
		printk("Response timeout\n");
		return INT_STS_RTO_BAR;
	}
	else if ((error&INT_STS_DCRC)==INT_STS_DCRC)
	{
		printk("Data CRC error\n");
		return INT_STS_DCRC;
	}
	else if ((error&INT_STS_RCRC)==INT_STS_RCRC)
	{
		printk("Response CRC error\n");
		return INT_STS_RCRC;
	}
	else if ((error&INT_STS_RE)==INT_STS_RE)
	{
		printk("Response error\n");		
		return INT_STS_RE;
	}
	else 
	{
		printk("No error \n");
		return 0;
	}	
	isb();
	sync(emmc_port);
	return 0;
}

void rtkemmc_phase_tuning(struct rtkemmc_host *emmc_port,u32 mode,int flag)
{
	u32 TX_window=0;
	u32 RX_window=0;
	u32 TX1_window=0;
	int TX_fixed=0xff;
	int RX_fixed=0xff;
	int TX_best=0x0;
        int RX_best=0x0;
	int TX1_best=0x0;
	int i=0;
	u32 range=0;
	u16 state=0;

	if (mode == MODE_HS400)
		range = 0x10;
	else
		range = 0x20;

	if(suspend == 1 && (emmc_port->tx_tuning || emmc_port->rx_tuning) && emmc_port->mmc->card->cid.manfid != 0x13)
        {
		if(emmc_port->tx_tuning==0 && emmc_port->rx_tuning==1) {
			phase(emmc_port, VP0_saved, suspend_VP1);
		}
		else if(emmc_port->tx_tuning==1 && emmc_port->rx_tuning==0)
		{
			phase(emmc_port, suspend_VP0, VP1_saved);
		}
		else {
			phase(emmc_port, suspend_VP0, suspend_VP1);
		}
		sync(emmc_port);
		printk(KERN_ERR "suspend/resume: restore tx & rx phase: TX=0x%x, RX=0x%x\n", suspend_VP0, suspend_VP1);
		if(mode == MODE_HS200) {
			suspend = 0;
		}
		return;
	}
#ifdef PHASE_INHERITED
	host_card_stop(emmc_port);
	if (emmc_port->tx_tuning || emmc_port->rx_tuning){
		phase(emmc_port, (emmc_port->tx_tuning)?0xff:VP0_saved, (emmc_port->rx_tuning)?0xff:VP1_saved); 
		sync(emmc_port);
	}
  	else{
		phase(emmc_port, VP0_saved, VP1_saved); //VP0, VP1 phase
		sync(emmc_port);
		printk(KERN_ERR "Inherit bootcode tuning phase: TX=0x%x, RX=0x%x\n", VP0_saved, VP1_saved);
		return;
  	}
#else
	phase(emmc_port, 0, 0);	//VP0, VP1 phase
	sync(emmc_port);
#endif
	

#ifdef EMMC_PARAM_TEST
	TX_fixed = (readl(emmc_port->misc_membase + MISC_DUMMY1) & 0xff00) >> 8;
	RX_fixed = (readl(emmc_port->misc_membase + MISC_DUMMY1) & 0xff);
#endif	
	if (TX_fixed != 0xff) {
		phase(emmc_port, TX_fixed, 0xff);
		printk(KERN_ERR "++++++++++++++++++\nFIXED TX_PHASE = 0x%02x\n++++++++++++++++++\n", TX_fixed);
	}
	else if (emmc_port->tx_tuning) {
		if (mode == MODE_HS400 && flag==1)
                        printk(KERN_ERR "Start HS400 TX Tuning: \n");
                else if(flag==1)
                        printk(KERN_ERR "Start HS200 TX Tuning:\n");
                for(i=0;i<range;i++) {
                        phase(emmc_port, i, 0xff);
#ifdef DEBUG
                        printk("phase =0x%x \n", i);
#endif

                        if(rtkemmc_send_cmd13(emmc_port, &state) != 0)
                                TX_window= TX_window&(~(1<<i));
                        else
                                TX_window= TX_window|((1<<i));
                }
                TX_best = search_best(TX_window, range);
                if(flag==1)printk(KERN_ERR "TX_WINDOW = 0x%08x\n TX_best=0x%x\n", TX_window, TX_best);
                phase(emmc_port, TX_best, 0xff);
	}

	if (RX_fixed != 0xff) {
		phase(emmc_port, 0xff, RX_fixed);
		printk(KERN_ERR "++++++++++++++++++\nFIXED RX_PHASE = 0x%02x\n++++++++++++++++++\n", RX_fixed);
	}
	else if (emmc_port->rx_tuning) {
		if (mode == MODE_HS400 && flag==1)
                        printk(KERN_ERR "Start HS400 RX Tuning:\n");
                else if(flag==1)
                        printk(KERN_ERR "Start HS200 RX Tuning:\n ");
                for(i=0;i<range;i++) {
                        phase(emmc_port, 0xff, i);
#ifdef DEBUG
                        printk("phase =0x%x \n", i);
#endif
                        if(rtkemmc_send_cmd18(emmc_port, 1024, 0x100, 0) != 0)
                                RX_window= RX_window&(~(1<<i));
                        else
                                RX_window= RX_window|((1<<i));
                }
                RX_best = search_best(RX_window, range);
		suspend_VP1 = RX_best;
                if(flag==1)printk(KERN_ERR "RX_WINDOW = 0x%08x RX_best=0x%x\n", RX_window,RX_best);
                phase(emmc_port, 0xff, RX_best);
	}
	if (TX_fixed != 0xff) {
                phase(emmc_port, TX_fixed, 0xff);
                printk(KERN_ERR "++++++++++++++++++\nFIXED TX_PHASE = 0x%02x\n++++++++++++++++++\n", TX_fixed);
        }
        else if (emmc_port->tx_tuning) {
		TX1_window= TX_window;
                if (mode == MODE_HS400 && flag==1)
                        printk(KERN_ERR "Start HS400 TX Tuning2:\n");
                else if(flag==1)
                        printk(KERN_ERR "Start HS200 TX Tuning2:\n");
                for(i=0;i<range;i++) {
                        if(((TX_window>>i)&1)==1) {
                                phase(emmc_port, i, 0xff);
#ifdef DEBUG
                                printk("phase =0x%x \n", i);
#endif
                                if(rtkemmc_send_cmd25(emmc_port, 1024,0xfe,0) != 0)
                                        TX1_window= TX1_window&(~(1<<i));
                        }
                }
                TX1_best = search_best(TX1_window, range);
		suspend_VP0 = TX1_best;
                if(flag==1)printk(KERN_ERR "TX1_WINDOW = 0x%08x TX1_best=0x%x\n", TX1_window,TX1_best);
                phase(emmc_port, TX1_best, 0xff);
	}
	sync(emmc_port);
}
	
static int mmc_Tuning_HS200(struct rtkemmc_host *emmc_port,u32 mode){
	volatile int err=0;
	volatile int ret_state=0;
	struct mmc_host *host = emmc_port->mmc;
	struct mmc_host host_priv;
	struct mmc_card card_priv;
	u32 iobase = emmc_port->emmc_membase;
	unsigned long flags=0;
	u32 clk_drv, cmd_drv, data_drv, ds_drv;
	MMCPRINTF("%s \n", __func__);

    down_write(&cr_rw_sem);
	if (!g_bResuming)
		gCurrentBootMode = MODE_HS200;
	MMCPRINTF("[LY]sdr gCurrentBootMode =%d\n",gCurrentBootMode);

	rtkemmc_set_freq(emmc_port, 0xa6); //200Mhz
    rtkemmc_set_ip_div(emmc_port, EMMC_CLOCK_DIV_NON); // 200MHZ/1 = 200MHZ
#ifdef EMMC_PARAM_TEST
	if (readl(emmc_port->misc_membase + MISC_DUMMY2) != 0xffffffff){
		clk_drv = (readl(emmc_port->misc_membase + MISC_DUMMY2) & 0xff000000) >> 24;
		cmd_drv = (readl(emmc_port->misc_membase + MISC_DUMMY2) & 0x00ff0000) >> 16;
		data_drv = (readl(emmc_port->misc_membase + MISC_DUMMY2) & 0x0000ff00) >> 8;
		ds_drv = (readl(emmc_port->misc_membase + MISC_DUMMY2) & 0x000000ff);
		printk(KERN_ERR RED_BOLD"clk_drv = 0x%02x, cmd_drv = 0x%02x, data_drv = 0x%02x, ds_drv = 0x%02x"RESET, clk_drv, cmd_drv, data_drv, ds_drv);
		rtkemmc_set_pad_driving(emmc_port,clk_drv, cmd_drv, data_drv, ds_drv);
	}	
	else{
#endif		
		if (pddrive_nf_s2[0] != 0 )
			rtkemmc_set_pad_driving(emmc_port, pddrive_nf_s2[1], pddrive_nf_s2[2], pddrive_nf_s2[3], pddrive_nf_s2[4]);
		else
			rtkemmc_set_pad_driving(emmc_port,0xbb, 0xbb, 0xbb, 0xbb);
#ifdef EMMC_PARAM_TEST
	}
#endif

	rtkemmc_phase_tuning(emmc_port,mode,1);
	//mmc_flush_cache(host->card);
	sync(emmc_port);	
	mdelay(10);
    up_write(&cr_rw_sem);

	return 0;
}

static int mmc_Tuning_HS400(struct rtkemmc_host *emmc_port,u32 mode)
{
        MMCPRINTF("%s \n", __func__);
        down_write(&cr_rw_sem);
        if (!g_bResuming)
                gCurrentBootMode = MODE_HS400;
        MMCPRINTF("[LY]sdr gCurrentBootMode =%d\n",gCurrentBootMode);
        rtkemmc_set_freq(emmc_port, 0xa6); //200Mhz
        rtkemmc_set_ip_div(emmc_port, EMMC_CLOCK_DIV_NON); // 200MHZ/1 = 200MHZ

        if (pddrive_nf_s3[0] != 0 )
                rtkemmc_set_pad_driving(emmc_port, pddrive_nf_s3[1], pddrive_nf_s3[2], pddrive_nf_s3[3], pddrive_nf_s3[4]);
        else
                rtkemmc_set_pad_driving(emmc_port,0xff, 0x99, 0xaa, 0x33);

        rtkemmc_phase_tuning(emmc_port,mode,1);
	//mmc_flush_cache(emmc_port->mmc->card);
        sync(emmc_port);
        mdelay(10);
        up_write(&cr_rw_sem);
        printk(KERN_ERR "HS400 first stage: final phase=0x%x\n", readl(emmc_port->crt_membase + SYS_PLL_EMMC1));
        return 0;
}
	
static int mmc_Tuning_DDR50(struct rtkemmc_host *emmc_port){
	volatile int err=0;
	volatile int ret_state=0;
	struct mmc_host *host = emmc_port->mmc;
	struct mmc_host host_priv;
	struct mmc_card card_priv;
	u32 iobase = emmc_port->emmc_membase;
	unsigned long flags=0;

    down_write(&cr_rw_sem);
	if (!g_bResuming)	
		gCurrentBootMode = MODE_DDR;
	MMCPRINTF("[LY]sdr gCurrentBootMode =%d\n",gCurrentBootMode);

	rtkemmc_set_freq(emmc_port, 0x57); //100Mhz
    rtkemmc_set_ip_div(emmc_port, EMMC_CLOCK_DIV_NON); // 100MHZ/1 = 100MHZ

	sync(emmc_port);	
	udelay(100);
    up_write(&cr_rw_sem);

	return 0;
}

static int mmc_Tuning_SDR50(struct rtkemmc_host *emmc_port){			
	volatile int err=0;
	volatile int ret_state=0;
	struct mmc_host *host = emmc_port->mmc;
	struct mmc_host host_priv;
	struct mmc_card card_priv;
	u32 iobase = emmc_port->emmc_membase;
	unsigned long flags=0;

    down_write(&cr_rw_sem);
	if (!g_bResuming)	
		gCurrentBootMode = MODE_SDR;
	MMCPRINTF("[LY]sdr gCurrentBootMode =%d\n",gCurrentBootMode);

	rtkemmc_set_freq(emmc_port, 0x57); //100Mhz
    rtkemmc_set_ip_div(emmc_port, EMMC_CLOCK_DIV_2); // 100MHZ/2 = 50MHZ
	if (pddrive_nf_s0[0] != 0 )
		rtkemmc_set_pad_driving(emmc_port, pddrive_nf_s0[1], pddrive_nf_s0[2], pddrive_nf_s0[3], pddrive_nf_s0[4]);
	else
		rtkemmc_set_pad_driving(emmc_port,0x33, 0x33, 0x33, 0x33);

	sync(emmc_port);	
	udelay(100);
    up_write(&cr_rw_sem);

	return 0;
}

/****************************************************************
 *  error handling area
 * 
****************************************************************/
int error_handling(struct rtkemmc_host *emmc_port, unsigned int cmd_idx, unsigned int bIgnore)
{
	u32 iobase = emmc_port->emmc_membase;
    unsigned char sts1_val=0;
    int err=0;
	struct mmc_host *host = emmc_port->mmc;
    extern unsigned char g_ext_csd[];

    printk(KERN_INFO "%s : status1 val=%02x, cmd_idx=0x%02x, gCurrentBootMode=0x%02x\n", __func__, sts1_val,cmd_idx,gCurrentBootMode);
    host_card_stop(emmc_port);
	if (cmd_idx > MMC_SET_RELATIVE_ADDR)
	{
		polling_to_tran_state(emmc_port, cmd_idx, bIgnore);
	}

    printk(KERN_INFO "%s : status1 val=%02x, cmd_idx=0x%02x, gCurrentBootMode=0x%02x\n", __func__, sts1_val,cmd_idx,gCurrentBootMode);
    if (bIgnore)
    	return 0;
    return err;
}

int polling_to_tran_state(struct rtkemmc_host *emmc_port, int cmd_idx, int bIgnore)
{
        int err=1, retry_cnt=5;
        u8 ret_state=0;
        struct mmc_host *host = emmc_port->mmc;

        MMCPRINTF("%s : \n", __func__);
        sync(emmc_port);
        while(retry_cnt-- && err)
                err = rtkemmc_send_status(host->card,&ret_state,0,bIgnore);

        sync(emmc_port);

        if ((err)||(ret_state != STATE_TRAN)) {
                printk("--- cmd13 fail or ret_state not tran : 0x%08x---\n", ret_state);
		rtkemmc_stop_transmission(host->card,1);
                err = rtkemmc_wait_status(host->card,STATE_TRAN,0,bIgnore);
        }
        sync(emmc_port);
        return err;
}

int rtkemmc_send_cmd8(struct rtkemmc_host *emmc_port, unsigned int bIgnore)
{
	int ret_err=0;
    	struct sd_cmd_pkt cmd_info;
	struct mmc_host *host = emmc_port->mmc;
	volatile int err=1,retry_cnt=5;
	unsigned char cfg3=0,cfg1=0;
	int sts1_val=0;
	unsigned char *crd_ext_csd=NULL;
	u32 iobase = emmc_port->emmc_membase;
	struct mmc_data *data=NULL;
    	struct mmc_command *cmd=NULL;

	memset(&cmd_info, 0x00, sizeof(struct sd_cmd_pkt));
	
	crd_ext_csd = emmc_port->dma_paddr;
	if (crd_ext_csd == NULL)
	{
		printk(KERN_ERR "%s,%s : crd_ext_csd == NULL\n",DRIVER_NAME,__func__);
		return -5;
	}

	if (cmd_info.cmd == NULL)
	{
		cmd  = (struct mmc_command*) kmalloc(sizeof(struct mmc_command),GFP_KERNEL);
		memset(cmd, 0x00, sizeof(struct mmc_command));
		cmd_info.cmd  = (struct mmc_command*) cmd;
	}
	cmd_info.emmc_port = emmc_port;
	cmd_info.cmd->arg=0;
	cmd_info.cmd->opcode = MMC_SEND_EXT_CSD;
	cmd_info.rsp_len	 = 6;
	cmd_info.byte_count  = 0x200;
	cmd_info.block_count = 1;
	cmd_info.dma_buffer = crd_ext_csd;
	if (cmd_info.cmd->data == NULL)
	{
		data  = (struct mmc_data*) kmalloc(sizeof(struct mmc_data),GFP_KERNEL);
		memset(data, 0x00, sizeof(struct mmc_data));
		cmd_info.cmd->data = data;
		data->flags = MMC_DATA_READ;
	}
	else
		cmd_info.cmd->data->flags = MMC_DATA_READ;
	sync(emmc_port);
	MMCPRINTF("\n*** %s %s %d, cmdidx=0x%02x(%d), resp_type=0x%08x, host=0x%08x, card=0x%08x -------\n", __FILE__, __func__, __LINE__, cmd_info.cmd->opcode, cmd_info.cmd->opcode, cmd_info.cmd->flags, host, host->card);
	ret_err = SD_Stream_Cmd( EMMC_NORMALREAD, &cmd_info, bIgnore);
	if (ret_err)
	{
		if (bIgnore)
		{
			goto err8;
		}
		MMCPRINTF("[LY] status1 val=%02x\n", sts1_val);
		host_card_stop(emmc_port);
		polling_to_tran_state(emmc_port,MMC_SEND_EXT_CSD,1);
        }
err8:
	if (cmd)
	{
		cmd_info.cmd = NULL;
		kfree(cmd);
		cmd=NULL;
	}
	if (data)
	{
		//cmd_info->cmd->data = NULL;
		kfree(data);
		data=NULL;
	}
	sync(emmc_port);
	return ret_err;
}
int rtkemmc_send_cmd18(struct rtkemmc_host *emmc_port,int size, unsigned long addr, int flag)
{
	int ret_err=0;
    struct sd_cmd_pkt cmd_info;
	struct mmc_host *host = emmc_port->mmc;
	int sts1_val=0;
	unsigned char *crd_tmp_buffer=NULL;
	u32 iobase = emmc_port->emmc_membase;
	struct mmc_data *data=NULL;
    struct mmc_command *cmd=NULL;
	int i=0;
	unsigned long flags2;
	memset(&cmd_info, 0x00, sizeof(struct sd_cmd_pkt));


	crd_tmp_buffer = emmc_port->dma_paddr;
	if (crd_tmp_buffer == NULL)
	{
		printk(KERN_ERR "%s,%s : crd_tmp_buffer == NULL\n",DRIVER_NAME,__func__);
		return -5;
	}

	for(i=0;i<(2*0x200/4);i++)
	{
		*(u32 *)(gddr_dma_org+(i*4)) = (u32)((i+0x78*0x80)*0x13579bdf);
		//isb();
		//sync(emmc_port);
	}
	wmb();
	//__flush_dcache_area(gddr_dma_org, 2*0x200);
	//isb();
	//sync(emmc_port);

	if (cmd_info.cmd == NULL)
	{
		cmd  = (struct mmc_command*) kmalloc(sizeof(struct mmc_command),GFP_KERNEL);
		memset(cmd, 0x00, sizeof(struct mmc_command));
		cmd_info.cmd  = (struct mmc_command*) cmd;
	}
	cmd_info.emmc_port = emmc_port;
	cmd_info.cmd->arg = addr;
	cmd_info.cmd->opcode = MMC_READ_MULTIPLE_BLOCK;
	cmd_info.rsp_len	 = 6;
	cmd_info.byte_count  = 0x200;
	cmd_info.block_count = size/cmd_info.byte_count;
	cmd_info.dma_buffer = crd_tmp_buffer;
	if (cmd_info.cmd->data == NULL)
	{
		data  = (struct mmc_data*) kmalloc(sizeof(struct mmc_data),GFP_KERNEL);
		memset(data, 0x00, sizeof(struct mmc_data));
		cmd_info.cmd->data = data;
		data->flags = MMC_DATA_READ;
	}
	else
		cmd_info.cmd->data->flags = MMC_DATA_READ;
	MMCPRINTF("\n*** %s %s %d, cmdidx=0x%02x(%d), resp_type=0x%08x, host=0x%08x, card=0x%08x -------\n", __FILE__, __func__, __LINE__, cmd_info.cmd->opcode, cmd_info.cmd->opcode, cmd_info.cmd->flags, host, host->card);
	ret_err = SD_Stream_Cmd(EMMC_AUTOREAD1, &cmd_info, 1);
	if(flag==1) {
                memcpy(compare2,gddr_dma_org,size);
                //cp_sha256(gddr_dma_org, size, compare4);
        }

	rtk_lockapi_lock2(flags2, _at_("rtkemmc_send_cmd18"));
        if (readl(emmc_port->emmc_membase+EMMC_ISR) & ISR_DMA_DONE_INT)
                writel(0x2,emmc_port->emmc_membase+EMMC_ISR);
        rtk_lockapi_unlock2(flags2, _at_("rtkemmc_send_cmd18"));
	if (ret_err)
	{
		MMCPRINTF("[LY] status1 val=%02x\n", sts1_val);
		udelay(200);

		rtkemmc_stop_transmission(host->card, 1);
		
	/*	wait_done_timeout(emmc_port, (u32*)(emmc_port->emmc_membase + EMMC_STATUS), 0x200, 0x0);          //card is not busy
		isb();
		sync(emmc_port);

		rtk_lockapi_lock2(flags2, _at_("rtkemmc_send_cmd18"));
		if (readl(emmc_port + EMMC_TBBCNT) == 1024) {
			rtk_lockapi_unlock2(flags2, _at_("rtkemmc_send_cmd18"));
			wait_done_timeout(emmc_port, (u32*)(emmc_port->emmc_membase + EMMC_ISR), ISR_DMA_DONE_INT, ISR_DMA_DONE_INT);          //card is not busy
			isb();
			sync(emmc_port);
			rtk_lockapi_lock2(flags2, _at_("rtkemmc_send_cmd18"));
			rtkemmc_clr_int_sta();
			rtk_lockapi_unlock2(flags2, _at_("rtkemmc_send_cmd18"));

		}else{
			rtk_lockapi_unlock2(flags2, _at_("rtkemmc_send_cmd18"));
		}*/

		host_card_stop(emmc_port);
		polling_to_tran_state(emmc_port,MMC_READ_MULTIPLE_BLOCK,1);
	}
err18:
	if (cmd)
	{
		kfree(cmd);
		cmd_info.cmd = NULL;
		cmd=NULL;
	}
	if (data)
	{
		kfree(data);
		//cmd_info.cmd->data = NULL;
		data=NULL;
	}
	return ret_err;
}
int rtkemmc_send_cmd17(struct rtkemmc_host *emmc_port)
{
	int ret_err=0;
    	struct sd_cmd_pkt cmd_info;
	struct mmc_host *host = emmc_port->mmc;
	int sts1_val=0;
	unsigned char *crd_tmp_buffer=NULL;
	u32 iobase = emmc_port->emmc_membase;
	struct mmc_data *data=NULL;
    	struct mmc_command *cmd=NULL;
	int i=0;

	memset(&cmd_info, 0x00, sizeof(struct sd_cmd_pkt));

	crd_tmp_buffer = emmc_port->dma_paddr;
	if (crd_tmp_buffer == NULL)
	{
		printk(KERN_ERR "%s,%s : crd_tmp_buffer == NULL\n",DRIVER_NAME,__func__);
		return -5;
	}

	for(i=0;i<(0x200/4);i++)
	{
		*(u32 *)(gddr_dma_org+(i*4)) = (u32)((i+0x78*0x80)*0x13579bdf);
		//isb();
		//sync(emmc_port);
	}
	wmb();
	//__flush_dcache_area(gddr_dma_org, 0x200);
	//isb();
	//sync(emmc_port);

	if (cmd_info.cmd == NULL)
	{
		cmd  = (struct mmc_command*) kmalloc(sizeof(struct mmc_command),GFP_KERNEL);
		memset(cmd, 0x00, sizeof(struct mmc_command));
		cmd_info.cmd  = (struct mmc_command*) cmd;
	}
	cmd_info.emmc_port = emmc_port;
	cmd_info.cmd->arg = 0x100;
	cmd_info.cmd->opcode = MMC_READ_SINGLE_BLOCK;
	cmd_info.rsp_len	 = 6;
	cmd_info.byte_count  = 0x200;
	cmd_info.block_count = 1;
	cmd_info.dma_buffer = crd_tmp_buffer;
	if (cmd_info.cmd->data == NULL)
	{
		data  = (struct mmc_data*) kmalloc(sizeof(struct mmc_data),GFP_KERNEL);
		memset(data, 0x00, sizeof(struct mmc_data));
		cmd_info.cmd->data = data;
		data->flags = MMC_DATA_READ;
	}
	else
		cmd_info.cmd->data->flags = MMC_DATA_READ;
	MMCPRINTF("\n*** %s %s %d, cmdidx=0x%02x(%d), resp_type=0x%08x, host=0x%08x, card=0x%08x -------\n", __FILE__, __func__, __LINE__, cmd_info.cmd->opcode, cmd_info.cmd->opcode, cmd_info.cmd->flags, host, host->card);
	ret_err = SD_Stream_Cmd(EMMC_AUTOREAD1, &cmd_info, 1);
	if (ret_err)
	{
		MMCPRINTF("[LY] status1 val=%02x\n", sts1_val);
		rtkemmc_stop_transmission(host->card, 1);
		host_card_stop(emmc_port);
		polling_to_tran_state(emmc_port,MMC_READ_SINGLE_BLOCK,1);
	}
err17:
	if (cmd)
	{
		kfree(cmd);
		cmd_info.cmd = NULL;
		cmd=NULL;
	}
	if (data)
	{
		kfree(data);
		//cmd_info.cmd->data = NULL;
		data=NULL;
	}
	return ret_err;
}

int rtkemmc_send_cmd25(struct rtkemmc_host *emmc_port,int size, unsigned long addr, int flag)
{
	int ret_err=0,i=0;
    	struct sd_cmd_pkt cmd_info;
	struct mmc_host *host = emmc_port->mmc;
	int sts1_val=0;
	char *crd_tmp_buffer=NULL;
	u32 iobase = emmc_port->emmc_membase;
	struct mmc_data *data=NULL;
    	struct mmc_command *cmd=NULL;
	unsigned long flags=0;

	memset(&cmd_info, 0x00, sizeof(struct sd_cmd_pkt));
	
	crd_tmp_buffer = emmc_port->dma_paddr;
	if (crd_tmp_buffer == NULL)
	{
		printk(KERN_ERR "%s,%s : crd_ext_csd == NULL\n",DRIVER_NAME,__func__);
		return -5;
	}

	for(i=0;i<(2*0x200/4);i++)
	{
		if(GLOBAL==0x80000000) GLOBAL=0;
                else GLOBAL++;
                *(u32 *)(gddr_dma_org+(i*4)) = GLOBAL;
		//*(u32 *)(gddr_dma_org+(i*4)) = (u32)((i+0x78*0x80)*0x13579bdf);
		//isb();
		//sync(emmc_port);
	}
	wmb();
	//__flush_dcache_area(gddr_dma_org, 2*0x200);
	//isb();
	//sync(emmc_port);

	if (cmd_info.cmd == NULL)
	{
		cmd  = (struct mmc_command*) kmalloc(sizeof(struct mmc_command),GFP_KERNEL);
		memset(cmd, 0x00, sizeof(struct mmc_command));
		cmd_info.cmd  = (struct mmc_command*) cmd;
	}
	cmd_info.emmc_port = emmc_port;
	cmd_info.cmd->arg = addr;
	cmd_info.cmd->opcode = MMC_WRITE_MULTIPLE_BLOCK;
	cmd_info.rsp_len	 = 6;
	cmd_info.byte_count  = 0x200;
	cmd_info.block_count = size/cmd_info.byte_count;
	cmd_info.dma_buffer = crd_tmp_buffer;
	if (cmd_info.cmd->data == NULL)
	{
		data  = (struct mmc_data*) kmalloc(sizeof(struct mmc_data),GFP_KERNEL);
		memset(data, 0x00, sizeof(struct mmc_data));
		cmd_info.cmd->data = data;
		data->flags = MMC_DATA_WRITE;
	}
	else
		cmd_info.cmd->data->flags = MMC_DATA_WRITE;
	MMCPRINTF("\n*** %s %s %d, cmdidx=0x%02x(%d), resp_type=0x%08x, host=0x%08x, card=0x%08x , cmd=0x%08x, data=0x%08x-------\n", __FILE__, __func__, __LINE__, cmd_info.cmd->opcode, cmd_info.cmd->opcode, cmd_info.cmd->flags, host, host->card,cmd,data);
	ret_err = SD_Stream_Cmd(EMMC_AUTOWRITE1, &cmd_info, 1);
	if(flag==1) {
                memcpy(compare1,gddr_dma_org,size);
                //cp_sha256(gddr_dma_org, size, compare3);
        }
	if (ret_err)
	{
		MMCPRINTF("[LY] status1 val=%02x\n", sts1_val);
		host_card_stop(emmc_port);
		polling_to_tran_state(emmc_port,MMC_WRITE_MULTIPLE_BLOCK,1);
	}
err25:
	MMCPRINTF("\n*** %s %s %d, cmdidx=0x%02x(%d), resp_type=0x%08x, host=0x%08x, card=0x%08x , cmd=0x%08x, data=0x%08x-------\n", __FILE__, __func__, __LINE__, cmd_info.cmd->opcode, cmd_info.cmd->opcode, cmd_info.cmd->flags, host, host->card,cmd,data);
#if 1
	if (cmd)
	{
		cmd_info.cmd = NULL;
		kfree(cmd);
		cmd=NULL;
	}
	if (data)
	{
		//cmd_info.cmd->data = NULL;
		kfree(data);
		data=NULL;
	}
#endif
	sync(emmc_port);
	return ret_err;
}

int rtkemmc_send_cmd24(struct rtkemmc_host *emmc_port)
{
	int ret_err=0,i=0;
    	struct sd_cmd_pkt cmd_info;
	struct mmc_host *host = emmc_port->mmc;
	int sts1_val=0;
	char *crd_tmp_buffer=NULL;
	u32 iobase = emmc_port->emmc_membase;
	struct mmc_data *data=NULL;
    	struct mmc_command *cmd=NULL;
	unsigned long flags=0;

	memset(&cmd_info, 0x00, sizeof(struct sd_cmd_pkt));
	
	crd_tmp_buffer = emmc_port->dma_paddr;
	if (crd_tmp_buffer == NULL)
	{
		printk(KERN_ERR "%s,%s : crd_ext_csd == NULL\n",DRIVER_NAME,__func__);
		return -5;
	}

	for(i=0;i<(0x200/4);i++)
	{
		*(u32 *)(gddr_dma_org+(i*4)) = (u32)((i+0x78*0x80)*0x13579bdf);
		//isb();
		//sync(emmc_port);
	}
	wmb();
	//__flush_dcache_area(gddr_dma_org, 0x200);
	//isb();
	//sync(emmc_port);

	if (cmd_info.cmd == NULL)
	{
		cmd  = (struct mmc_command*) kmalloc(sizeof(struct mmc_command),GFP_KERNEL);
		memset(cmd, 0x00, sizeof(struct mmc_command));
		cmd_info.cmd  = (struct mmc_command*) cmd;
	}
	cmd_info.emmc_port = emmc_port;
	cmd_info.cmd->arg = 0xfe;
	cmd_info.cmd->opcode = MMC_WRITE_BLOCK;
	cmd_info.rsp_len	 = 6;
	cmd_info.byte_count  = 0x200;
	cmd_info.block_count = 1;
	cmd_info.dma_buffer = crd_tmp_buffer;
	if (cmd_info.cmd->data == NULL)
	{
		data  = (struct mmc_data*) kmalloc(sizeof(struct mmc_data),GFP_KERNEL);
		memset(data, 0x00, sizeof(struct mmc_data));
		cmd_info.cmd->data = data;
		data->flags = MMC_DATA_READ;
	}
	else
		cmd_info.cmd->data->flags = MMC_DATA_WRITE;
	MMCPRINTF("\n*** %s %s %d, cmdidx=0x%02x(%d), resp_type=0x%08x, host=0x%08x, card=0x%08x , cmd=0x%08x, data=0x%08x-------\n", __FILE__, __func__, __LINE__, cmd_info.cmd->opcode, cmd_info.cmd->opcode, cmd_info.cmd->flags, host, host->card,cmd,data);
	ret_err = SD_Stream_Cmd(EMMC_AUTOWRITE1, &cmd_info, 1);
	if (ret_err)
	{
		MMCPRINTF("[LY] status1 val=%02x\n", sts1_val);
		host_card_stop(emmc_port);
		polling_to_tran_state(emmc_port,MMC_WRITE_BLOCK,1);
	}
err24:
	MMCPRINTF("\n*** %s %s %d, cmdidx=0x%02x(%d), resp_type=0x%08x, host=0x%08x, card=0x%08x , cmd=0x%08x, data=0x%08x-------\n", __FILE__, __func__, __LINE__, cmd_info.cmd->opcode, cmd_info.cmd->opcode, cmd_info.cmd->flags, host, host->card,cmd,data);
#if 1
	if (cmd)
	{
		cmd_info.cmd = NULL;
		kfree(cmd);
		cmd=NULL;
	}
	if (data)
	{
		//cmd_info.cmd->data = NULL;
		kfree(data);
		data=NULL;
	}
#endif
	sync(emmc_port);
	return ret_err;
}

static int rw_test_tuning(struct rtkemmc_host *emmc_port,unsigned long emmc_blk_addr)
{
        int i,loop;
        rtkemmc_send_cmd25(emmc_port,DMA_ALLOC_LENGTH,/*0x26880*/emmc_blk_addr,1);
        rtkemmc_send_cmd18(emmc_port,DMA_ALLOC_LENGTH,/*0x26880*/emmc_blk_addr,1);

        for(i=0;i<DMA_ALLOC_LENGTH;i++) {
                if(compare1[i]!=compare2[i]) {
                        return 1;
                }
        }
        /*      for(i=0;i<16;i++) {
                        if(compare3[i]!=compare4[i]) {
                                return 1;
                        }
                }*/
        return 0;
}

static void rtkemmc_dqs_tuning(struct mmc_host *host)
{
        int i=0, j=0;
        struct rtkemmc_host *emmc_port;
        unsigned long emmc_real_size=0;
        unsigned long emmc_set_size=0;
        unsigned long dqs_tuning_blk_addr=0;
        unsigned long flags2;

        emmc_port = mmc_priv(host);
        mdelay(2);
	if(suspend == 1)
	{
		rtk_lockapi_lock2(flags2, _at_("rtkemmc_dqs_tuning"));
		if(emmc_port->dqs_tuning == 1) {
			writel(0x80 | suspend_dqs,emmc_port->emmc_membase + EMMC_DQS_CTRL1); //dqs dly tap
		}
		else {
			writel(0x80 | dqs_saved, emmc_port->emmc_membase + EMMC_DQS_CTRL1); //dqs dly tap
		}
		printk(KERN_ERR "suspend/resume: restore DQS=0x%x\n", readl(emmc_port->emmc_membase + EMMC_DQS_CTRL1));
		rtk_lockapi_unlock2(flags2, _at_("rtkemmc_dqs_tuning"));
		suspend = 0;
		return;
	}

#ifdef DQS_INHERITED
	if(emmc_port->dqs_tuning == 0) {
		rtk_lockapi_lock2(flags2, _at_("rtkemmc_dqs_tuning"));
		writel(0x80 | dqs_saved, emmc_port->emmc_membase + EMMC_DQS_CTRL1); //dqs dly tap
		printk(KERN_ERR "Inherit bootcode dqs: DQS=0x%x\n", readl(emmc_port->emmc_membase + EMMC_DQS_CTRL1));
		rtk_lockapi_unlock2(flags2, _at_("rtkemmc_dqs_tuning"));
		set_RTK_initial_flag(1);
		return;
	}
#endif
        down_write(&cr_rw_sem);
        g_bTuning = 1;

        emmc_real_size = host->card->ext_csd.sectors;
        if(emmc_real_size > 0x2200000 )    //32gb
                emmc_set_size = 0x3900000;
        else if( (emmc_real_size > 0x1200000) && (emmc_real_size < 0x2200000) ) //16gb
                emmc_set_size = 0x1c80000;
        else if( (emmc_real_size > 0xb40000) && (emmc_real_size < 0x1200000) )  //8gb
                emmc_set_size = 0xe40000;
        else
                emmc_set_size = 0x720000;     //4gb

        if(emmc_real_size-emmc_set_size>0x800) {
                dqs_tuning_blk_addr = emmc_set_size;    //non-used emmc space
        }
        else {
                dqs_tuning_blk_addr = 0x26880;  //evaluated free space
        }
        printk(KERN_ERR "emmc_real_blk_size=0x%llx, emmc_set_blk_size=0x%llx, dqs_tuning_blk_addr=0x%llx\n",emmc_real_size,emmc_set_size,dqs_tuning_blk_addr);

	for(i=0; i<0x20; i++) {
                if(j==9) break;
                rtk_lockapi_lock2(flags2, _at_("rtkemmc_dqs_tuning"));
                writel(0x80|(i<<1),emmc_port->emmc_membase + EMMC_DQS_CTRL1);
		printk(KERN_INFO "DQS windows tuning: i=0x%x, EMMC_DQS_CTRL1=0x%x ",i<<1,readl(emmc_port->emmc_membase + EMMC_DQS_CTRL1));
                rtk_lockapi_unlock2(flags2, _at_("rtkemmc_dqs_tuning"));
                rtkemmc_phase_tuning(emmc_port,MODE_HS400,0);
                if( rw_test_tuning(emmc_port, dqs_tuning_blk_addr)==0) {
                        j++;
                }
                else {
                        j=0;
                }
        }

        if(j!=9) {
                printk(KERN_ERR "Cannot find a proper dqs window...\n");
                rtk_lockapi_lock2(flags2, _at_("rtkemmc_dqs_tuning"));
                writel(0x88,emmc_port->emmc_membase + EMMC_DQS_CTRL1);
                rtk_lockapi_unlock2(flags2, _at_("rtkemmc_dqs_tuning"));
        }
        else {
                rtk_lockapi_lock2(flags2, _at_("rtkemmc_dqs_tuning"));
                writel(0x80|(readl(emmc_port->emmc_membase + EMMC_DQS_CTRL1)-8),emmc_port->emmc_membase + EMMC_DQS_CTRL1); //dqs dly tap
		suspend_dqs = readl(emmc_port->emmc_membase + EMMC_DQS_CTRL1);
                printk(KERN_ERR "DQS=0x%x\n", readl(emmc_port->emmc_membase + EMMC_DQS_CTRL1));
                rtk_lockapi_unlock2(flags2, _at_("rtkemmc_dqs_tuning"));
                rtkemmc_phase_tuning(emmc_port,MODE_HS400,1);
                printk(KERN_ERR "HS400: final phase=0x%x\n", readl(emmc_port->crt_membase + SYS_PLL_EMMC1));
        }
	//mmc_flush_cache(host->card);
        sync(emmc_port);
        mdelay(10);
        g_bTuning = 0;
        rtk_lockapi_lock2(flags2, _at_("rtkemmc_dqs_tuning"));
        printk(KERN_ERR "rtkemmc_dqs_tuning end EMMC_UHSREG=%x\n",readl(emmc_port->emmc_membase + EMMC_UHSREG));
        rtk_lockapi_unlock2(flags2, _at_("rtkemmc_dqs_tuning"));
        up_write(&cr_rw_sem);

        set_RTK_initial_flag(1);        //this flag is to use for sd card check for rcu install
}

void host_card_stop2(struct rtkemmc_host *emmc_port){
	printk(KERN_INFO "host_card_stop2 - start\n");
	volatile u32 reg;
	unsigned long flags2;

	rtk_lockapi_lock2(flags2, _at_("host_card_stop2"));

	rtkemmc_backup_registers(emmc_port);

	//CRT reset eMMC
	reg = readl(emmc_port->crt_membase + SYS_SOFT_RESET2);
	isb();
	sync(emmc_port);
	rtkemmc_writel(reg&0xfffff7ff, emmc_port->crt_membase + SYS_SOFT_RESET2);
	isb();
	sync(emmc_port);
//=========jim: modify the emmc reset flow from only 980000004 bit 11 to 98000004 bit 11 and 98000000c(common clk framework) bit 24 28 =========
	//reg = readl(emmc_port->crt_membase + SYS_CLOCK_ENABLE1);
        //isb();
        //sync(emmc_port);
        //rtkemmc_writel(reg&0xeeffffff, emmc_port->crt_membase + SYS_CLOCK_ENABLE1);
        //isb();
	clk_disable_unprepare(clk_en_emmc);
        clk_disable_unprepare(clk_en_emmc_ip);

        sync(emmc_port);

	//release CRT eMMC reset
	reg = readl(emmc_port->crt_membase + SYS_SOFT_RESET2);
	isb();
	sync(emmc_port);
	rtkemmc_writel(reg|0x00000800, emmc_port->crt_membase + SYS_SOFT_RESET2);
	isb();
	sync(emmc_port);
	
//===========jim modify the emmc reset flow from only 980000004 bit 11 to 98000004 bit 11 and 98000000c (common clk framework) bit 24 28 =========
	//reg = readl(emmc_port->crt_membase + SYS_CLOCK_ENABLE1);
        //isb();
        //sync(emmc_port);
        //rtkemmc_writel(reg|0x11000000, emmc_port->crt_membase + SYS_CLOCK_ENABLE1);
        //isb();
	clk_prepare_enable(clk_en_emmc);
        clk_prepare_enable(clk_en_emmc_ip);
        sync(emmc_port);

	if (get_rtd129x_cpu_revision() >= RTD129x_CHIP_REVISION_A01 ) {
                rtkemmc_writel( readl(emmc_port->emmc_membase + EMMC_DUMMY_SYS) ^ 0x40000000, emmc_port->emmc_membase + EMMC_DUMMY_SYS);
                isb();
                sync(emmc_port);
                udelay(200);
        }

	//DBG port
#ifdef DBG_PORT	
	rtkemmc_writel(0x5, emmc_port->emmc_membase + EMMC_MAIN2_DBG);
	rtkemmc_writel(0x7208,sb2_debug);
	rtkemmc_writel(0x2db7, emmc_port->emmc_membase + EMMC_DBG);
	isb();
	sync(emmc_port);
#endif

	rtkemmc_writel(gRegTbl.emmc_mux_pad0, emmc_port->emmc_membase + EMMC_muxpad0);
	rtkemmc_writel(gRegTbl.emmc_mux_pad1, emmc_port->emmc_membase + EMMC_muxpad1);
	rtkemmc_writel(gRegTbl.emmc_mux_pad2, emmc_port->emmc_membase + EMMC_muxpad2);
	rtkemmc_writel(gRegTbl.emmc_pfunc_nf1, emmc_port->emmc_membase + EMMC_PFUNC_NF1);
	rtkemmc_writel(gRegTbl.emmc_pfunc_cr, emmc_port->emmc_membase + EMMC_PFUNC_CR);
	rtkemmc_writel(gRegTbl.emmc_pdrive_nf1, emmc_port->emmc_membase + EMMC_PDRIVE_NF1);
	rtkemmc_writel(gRegTbl.emmc_pdrive_nf2, emmc_port->emmc_membase + EMMC_PDRIVE_NF2);
	rtkemmc_writel(gRegTbl.emmc_pdrive_nf3, emmc_port->emmc_membase + EMMC_PDRIVE_NF3);
	rtkemmc_writel(gRegTbl.emmc_ckgen_ctl, emmc_port->emmc_membase + EMMC_CKGEN_CTL);
	isb();
	sync(emmc_port);
	
	mmc_host_reset(emmc_port);
	
	rtkemmc_writel(gRegTbl.emmc_ctype, emmc_port->emmc_membase + EMMC_CTYPE);
	rtkemmc_writel(gRegTbl.emmc_uhsreg, emmc_port->emmc_membase + EMMC_UHSREG);
	rtkemmc_writel(gRegTbl.emmc_ddr_reg, emmc_port->emmc_membase + EMMC_DDR_REG);
	rtkemmc_writel(gRegTbl.emmc_card_thr_ctl, emmc_port->emmc_membase + EMMC_CARD_THR_CTL);
	if(gCurrentBootMode == MODE_HS400)
	{
		rtkemmc_writel(gRegTbl.emmc_dqs_ctrl1 | 0x80, emmc_port->emmc_membase + EMMC_DQS_CTRL1);
	}
	else {
		rtkemmc_writel(gRegTbl.emmc_dqs_ctrl1, emmc_port->emmc_membase + EMMC_DQS_CTRL1);
	}
	rtkemmc_writel(0, emmc_port->emmc_membase+EMMC_PAD_CTL); //PAD to 1.8v
	isb();
	sync(emmc_port);

	rtkemmc_writel(0, emmc_port->emmc_membase + EMMC_CLKENA); // 0x10, clk enable, disable clock
	isb();
	sync(emmc_port);

	rtkemmc_writel(0xa0202000, emmc_port->emmc_membase + EMMC_CMD); // 0x10, clk enable, disable clock
	isb();
	sync(emmc_port);

	rtk_lockapi_unlock2(flags2, _at_("host_card_stop2"));
	// 0x2c, wait for CIU to take the command
	wait_done_timeout(emmc_port, emmc_port->emmc_membase + EMMC_CMD, 0x80000000,0);

	rtk_lockapi_lock2(flags2, _at_("host_card_stop2"));
	rtkemmc_writel(gRegTbl.emmc_clk_div, emmc_port->emmc_membase + EMMC_CLKDIV);
	isb();
	sync(emmc_port);
	
	rtkemmc_writel(0xa0202000, emmc_port->emmc_membase + EMMC_CMD);  // 0x2c = start_cmd, upd_clk_reg_only, wait_prvdata_complete
	isb();
	sync(emmc_port);
	rtk_lockapi_unlock2(flags2, _at_("host_card_stop2"));
	
	// 0x2c, wait for CIU to take the command
	wait_done_timeout(emmc_port, emmc_port->emmc_membase + EMMC_CMD, 0x80000000,0);

	rtk_lockapi_lock2(flags2, _at_("host_card_stop2"));
	rtkemmc_writel(0x10001, emmc_port->emmc_membase + EMMC_CLKENA); // 0x10, clk enable, disable clock
	isb();
	sync(emmc_port);

	rtkemmc_writel(0xa0202000, emmc_port->emmc_membase + EMMC_CMD);  // 0x2c = start_cmd, upd_clk_reg_only, wait_prvdata_complete
	isb();
	sync(emmc_port);
	rtk_lockapi_unlock2(flags2, _at_("host_card_stop2"));
	
	// 0x2c, wait for CIU to take the command
	wait_done_timeout(emmc_port, emmc_port->emmc_membase + EMMC_CMD, 0x80000000,0);
	
	printk(KERN_INFO "host_card_stop2 - end\n");

	rtk_lockapi_lock2(flags2, _at_("host_card_stop2"));
	printk (KERN_INFO "muxpad1- 0x%08x\n", readl(emmc_port->emmc_membase + EMMC_muxpad1));
    rtk_lockapi_unlock2(flags2, _at_("host_card_stop2"));
}


void host_card_stop(struct rtkemmc_host *emmc_port){
	printk(KERN_INFO "host_card_stop - start\n");
	volatile u32 reg;
	unsigned long flags2;

	rtk_lockapi_lock2(flags2, _at_("host_card_stop"));

	rtkemmc_backup_registers(emmc_port);

	//CRT reset eMMC
	reg = readl(emmc_port->crt_membase + SYS_SOFT_RESET2);
	isb();
	sync(emmc_port);
	rtkemmc_writel(reg&0xfffff7ff, emmc_port->crt_membase + SYS_SOFT_RESET2);
	isb();
	sync(emmc_port);

//=========jim: modify the emmc reset flow from only 980000004 bit 11 to 98000004 bit 11 and 98000000c(common clk framework) bit 24 28 =========
        //reg = readl(emmc_port->crt_membase + SYS_CLOCK_ENABLE1);
        //isb();
        //sync(emmc_port);
        //rtkemmc_writel(reg&0xeeffffff, emmc_port->crt_membase + SYS_CLOCK_ENABLE1);
        //isb();
	clk_disable_unprepare(clk_en_emmc);
        clk_disable_unprepare(clk_en_emmc_ip);
        sync(emmc_port);

	//A01 98000450[1]: reset test_mux_main2 soft reset
	if (get_rtd129x_cpu_revision() >= RTD129x_CHIP_REVISION_A01 ) {
		reg = readl(emmc_port->crt_membase + SYS_DUMMY);
		isb();
		sync(emmc_port);
		rtkemmc_writel(reg&0xfffffffd, emmc_port->crt_membase + SYS_DUMMY);
		isb();
		sync(emmc_port);
	}

	//release CRT eMMC reset
	reg = readl(emmc_port->crt_membase + SYS_SOFT_RESET2);
	isb();
	sync(emmc_port);
	rtkemmc_writel(reg|0x00000800, emmc_port->crt_membase + SYS_SOFT_RESET2);
	isb();
	sync(emmc_port);
	
//===========jim modify the emmc reset flow from only 980000004 bit 11 to 98000004 bit 11 and 98000000c(common clk framework) bit 24 28 =========
        //reg = readl(emmc_port->crt_membase + SYS_CLOCK_ENABLE1);
        //isb();
        //sync(emmc_port);
        //rtkemmc_writel(reg|0x11000000, emmc_port->crt_membase + SYS_CLOCK_ENABLE1);
        //isb();
	clk_prepare_enable(clk_en_emmc);
        clk_prepare_enable(clk_en_emmc_ip);
        sync(emmc_port);

	//A01 98000450[1]: release test_mux_main2 soft reset
	if (get_rtd129x_cpu_revision() >= RTD129x_CHIP_REVISION_A01 ) {
		reg = readl(emmc_port->crt_membase + SYS_DUMMY);
		isb();
		sync(emmc_port);
		rtkemmc_writel(reg | 0x00000002, emmc_port->crt_membase + SYS_DUMMY);
		isb();
		sync(emmc_port);
	}

	if (get_rtd129x_cpu_revision() >= RTD129x_CHIP_REVISION_A01 ) {
                rtkemmc_writel( readl(emmc_port->emmc_membase + EMMC_DUMMY_SYS) ^ 0x40000000, emmc_port->emmc_membase + EMMC_DUMMY_SYS);
                isb();
                sync(emmc_port);
		udelay(200);
        }

	//DBG port
#ifdef DBG_PORT	
	rtkemmc_writel(0x5, emmc_port->emmc_membase + EMMC_MAIN2_DBG);
	rtkemmc_writel(0x7208,sb2_debug);
	rtkemmc_writel(0x2db7, emmc_port->emmc_membase + EMMC_DBG);
	isb();
	sync(emmc_port);
#endif

	rtkemmc_writel(gRegTbl.emmc_mux_pad0, emmc_port->emmc_membase + EMMC_muxpad0);
	rtkemmc_writel(gRegTbl.emmc_mux_pad1, emmc_port->emmc_membase + EMMC_muxpad1);
	rtkemmc_writel(gRegTbl.emmc_mux_pad2, emmc_port->emmc_membase + EMMC_muxpad2);
	rtkemmc_writel(gRegTbl.emmc_pfunc_nf1, emmc_port->emmc_membase + EMMC_PFUNC_NF1);
	rtkemmc_writel(gRegTbl.emmc_pfunc_cr, emmc_port->emmc_membase + EMMC_PFUNC_CR);
	rtkemmc_writel(gRegTbl.emmc_pdrive_nf1, emmc_port->emmc_membase + EMMC_PDRIVE_NF1);
	rtkemmc_writel(gRegTbl.emmc_pdrive_nf2, emmc_port->emmc_membase + EMMC_PDRIVE_NF2);
	rtkemmc_writel(gRegTbl.emmc_pdrive_nf3, emmc_port->emmc_membase + EMMC_PDRIVE_NF3);
	rtkemmc_writel(gRegTbl.emmc_pdrive_nf4, emmc_port->emmc_membase + EMMC_PDRIVE_NF4);
	rtkemmc_writel(gRegTbl.emmc_pdrive_cr0, emmc_port->emmc_membase + EMMC_PDRIVE_CR0);
	rtkemmc_writel(gRegTbl.emmc_pdrive_cr1, emmc_port->emmc_membase + EMMC_PDRIVE_CR1);
	rtkemmc_writel(gRegTbl.emmc_pdrive_sdio, emmc_port->emmc_membase + EMMC_PDRIVE_SDIO);
	rtkemmc_writel(gRegTbl.emmc_pdrive_sdio1, emmc_port->emmc_membase + EMMC_PDRIVE_SDIO1);
	rtkemmc_writel(gRegTbl.emmc_ckgen_ctl, emmc_port->emmc_membase + EMMC_CKGEN_CTL);
	isb();
	sync(emmc_port);
	
	mmc_host_reset(emmc_port);
	
	rtkemmc_writel(gRegTbl.emmc_ctype, emmc_port->emmc_membase + EMMC_CTYPE);
	rtkemmc_writel(gRegTbl.emmc_uhsreg, emmc_port->emmc_membase + EMMC_UHSREG);
	rtkemmc_writel(gRegTbl.emmc_ddr_reg, emmc_port->emmc_membase + EMMC_DDR_REG);
	rtkemmc_writel(gRegTbl.emmc_card_thr_ctl, emmc_port->emmc_membase + EMMC_CARD_THR_CTL);
	if(gCurrentBootMode == MODE_HS400)
	{
		rtkemmc_writel(gRegTbl.emmc_dqs_ctrl1|0x80, emmc_port->emmc_membase + EMMC_DQS_CTRL1);
	}
	else {
		rtkemmc_writel(gRegTbl.emmc_dqs_ctrl1, emmc_port->emmc_membase + EMMC_DQS_CTRL1);
	}
	rtkemmc_writel(0, emmc_port->emmc_membase+EMMC_PAD_CTL); //PAD to 1.8v
	isb();
	sync(emmc_port);

	rtkemmc_writel(0, emmc_port->emmc_membase + EMMC_CLKENA); // 0x10, clk enable, disable clock
	isb();
	sync(emmc_port);

	rtkemmc_writel(0xa0202000, emmc_port->emmc_membase + EMMC_CMD); // 0x10, clk enable, disable clock
	isb();
	sync(emmc_port);

	rtk_lockapi_unlock2(flags2, _at_("host_card_stop"));
	
	// 0x2c, wait for CIU to take the command
	wait_done_timeout(emmc_port, emmc_port->emmc_membase + EMMC_CMD, 0x80000000,0);

	rtk_lockapi_lock2(flags2, _at_("host_card_stop"));
	rtkemmc_writel(gRegTbl.emmc_clk_div, emmc_port->emmc_membase + EMMC_CLKDIV);
	isb();
	sync(emmc_port);
	
	rtkemmc_writel(0xa0202000, emmc_port->emmc_membase + EMMC_CMD);  // 0x2c = start_cmd, upd_clk_reg_only, wait_prvdata_complete
	isb();
	sync(emmc_port);
	rtk_lockapi_unlock2(flags2, _at_("host_card_stop"));
	
	// 0x2c, wait for CIU to take the command
	wait_done_timeout(emmc_port, emmc_port->emmc_membase + EMMC_CMD, 0x80000000,0);

	rtk_lockapi_lock2(flags2, _at_("host_card_stop"));
	rtkemmc_writel(0x10001, emmc_port->emmc_membase + EMMC_CLKENA); // 0x10, clk enable, disable clock
	isb();
	sync(emmc_port);

	rtkemmc_writel(0xa0202000, emmc_port->emmc_membase + EMMC_CMD);  // 0x2c = start_cmd, upd_clk_reg_only, wait_prvdata_complete
	isb();
	sync(emmc_port);
	rtk_lockapi_unlock2(flags2, _at_("host_card_stop"));
	
	// 0x2c, wait for CIU to take the command
	wait_done_timeout(emmc_port, emmc_port->emmc_membase + EMMC_CMD, 0x80000000,0);
	printk(KERN_INFO "host_card_stop - end\n");

	rtk_lockapi_lock2(flags2, _at_("host_card_stop"));
	printk (KERN_INFO "muxpad1- 0x%08x\n", readl(emmc_port->emmc_membase + EMMC_muxpad1));
    rtk_lockapi_unlock2(flags2, _at_("host_card_stop"));
}


/***************************************************************
 *  
 ***************************************************************/
static int rtkemmc_go_idle(struct mmc_card *card)
{
    struct mmc_command cmd;
    struct sd_cmd_pkt cmd_info;
    int err = 0;

    memset(&cmd, 0, sizeof(struct mmc_command));
    memset(&cmd_info, 0, sizeof(struct sd_cmd_pkt));

    down_write(&cr_rw_sem);
    bSendCmd0=1;
    set_cmd_info(card,&cmd,&cmd_info,
                 MMC_GO_IDLE_STATE,
                 0x00,
                 6);
    err = SD_SendCMDGetRSP_Cmd(&cmd_info,0);
    bSendCmd0=0;
    up_write(&cr_rw_sem);

    if(err){
        mmcmsg3(KERN_WARNING "%s: MMC_GO_IDLE fail\n",DRIVER_NAME);
    }
    return err;

}

static int rtkemmc_stop_transmission(struct mmc_card *card,int bIgnore)
{
    struct mmc_command cmd;
    struct sd_cmd_pkt cmd_info;
    int err = 0;
    int bMalloc=0;
    struct mmc_host * mmc = mmc_host_local;

    MMCPRINTF("%s : \n", __func__);

    memset(&cmd, 0, sizeof(struct mmc_command));
    memset(&cmd_info, 0, sizeof(struct sd_cmd_pkt));
    
    if (card == NULL)
    {
		bMalloc=1;
		card = (struct mmc_card*)kmalloc(sizeof(struct mmc_card),GFP_KERNEL);
		card->host = mmc;
    }

    set_cmd_info(card,&cmd,&cmd_info,
                 MMC_STOP_TRANSMISSION,
                 0x00,
                 6);
    err = SD_SendCMDGetRSP_Cmd(&cmd_info,bIgnore);

    if (bMalloc)
    {
	kfree(card);
	card = NULL;
    }
    if(err){
        mmcmsg3(KERN_WARNING "%s: MMC_STOP_TRANSMISSION fail\n",DRIVER_NAME);
    }
    return err;

}

static void set_cmd_info(struct mmc_card *card,struct mmc_command * cmd,
                         struct sd_cmd_pkt * cmd_info,u32 opcode,u32 arg,u8 rsp_para)
{
    memset(cmd, 0, sizeof(struct mmc_command));
    memset(cmd_info, 0, sizeof(struct sd_cmd_pkt));

    cmd->opcode         = opcode;
    cmd->arg            = arg;
    cmd_info->cmd       = cmd;
    cmd_info->emmc_port    = mmc_priv(card->host);
    cmd_info->rsp_len   = rsp_para;
}

static struct rtk_host_ops emmc_ops = {
    .func_irq       = NULL,
    .re_init_proc   = NULL,
    .card_det       = NULL,
    .card_power     = NULL,
	.chk_card_insert= rtkemmc_chk_card_insert,
	.set_crt_muxpad = rtkemmc_set_pin_mux,
	.set_clk        = NULL,
    .reset_card     = NULL,
    .reset_host     = NULL,
    .bus_speed_down = NULL,
    .get_cmdcode    = NULL,
    .get_r1_type    = NULL, 
    .chk_cmdcode    = rtkemmc_chk_cmdcode,
    .chk_r1_type    = NULL,
    .backup_regs    = rtkemmc_backup_registers,
    .restore_regs   = rtkemmc_restore_registers,
};

#ifdef CONFIG_MMC_RTKEMMC_HK_ATTR
static int mmc_send_data_cmd(unsigned int hc_cmd,
                        unsigned int cmd_arg,
                        unsigned int blk_cnt,
                        unsigned char * buffer)
{
    int err = 0;
    struct mmc_host * mmc = mmc_host_local;
    unsigned long flags;

    printk(KERN_ERR "cmd=%u, arg=0x%08x, size=0x%08x, buf=0x%p\n",
            hc_cmd,cmd_arg,blk_cnt,buffer);
    if(mmc){
        struct rtkemmc_host *emmc_port = mmc_priv(mmc);
        struct mmc_card *card = mmc->card;
        struct mmc_request mrq = {0};
        struct mmc_command cmd = {0};
	    struct mmc_data	data   = {0};
	    struct scatterlist sg;

        emmc_port = mmc_priv(mmc);
        mmc_claim_host(mmc);
        spin_lock_irqsave(&emmc_port->lock,flags);

        cmd.opcode = hc_cmd;
        cmd.arg = cmd_arg;
	    cmd.flags = MMC_RSP_R1 | MMC_CMD_ADTC;

        if(blk_cnt)
        {   //date info setting
    		data.sg = &sg;
    		data.sg_len = 1;
    		data.blksz = 512;
    		data.blocks = blk_cnt;

    		sg_init_one(data.sg, buffer, (blk_cnt<<9));

            if( hc_cmd == MMC_WRITE_MULTIPLE_BLOCK ||
                hc_cmd == MMC_WRITE_BLOCK )
            {
    	        data.flags = MMC_DATA_WRITE;
            }
            else if( hc_cmd == MMC_SEND_EXT_CSD ||
                     hc_cmd == MMC_READ_MULTIPLE_BLOCK ||
                     hc_cmd == MMC_READ_SINGLE_BLOCK )
            {
    		    data.flags = MMC_DATA_READ;
            }else{
                printk(KERN_ERR "not supported command");
                spin_unlock_irqrestore(&emmc_port->lock, flags);
                goto ERR_OUT;
            }

            mmc_set_data_timeout(&data, card);
        }

	    data.mrq = &mrq;
	    cmd.mrq = &mrq;
	    cmd.data = &data;
	    cmd.retries =3;

CMD_RETRY:
        mrq.data = &data;
        mrq.cmd = &cmd;

        if(emmc_port->mrq){
            emmc_port->mrq = NULL;
        }

        cmd.error = 0;
	    data.error = 0;

        spin_unlock_irqrestore(&emmc_port->lock, flags);
        mmc_wait_for_req(mmc, &mrq);
        spin_lock_irqsave(&emmc_port->lock,flags);

        err = cmd.error;

        if(err && cmd.retries){
            printk(KERN_ERR "%s(%u)last retry %d counter.\n",
                    __func__,__LINE__,cmd.retries);
            cmd.retries--;
            goto CMD_RETRY;
        }
        spin_unlock_irqrestore(&emmc_port->lock, flags);
        mmc_release_host(mmc);
    }else{
        err = -ENODEV;
    }

ERR_OUT:
    if(err)
        printk(KERN_ERR "err=%d\n",err);
    return err;
}

static void show_ext_csd(u8 *ext_csd)
{
    int i;
    for(i=0; i<512; i+=8){
        printk(KERN_ERR
            "[%03u]=%02x [%03u]=%02x [%03u]=%02x [%03u]=%02x "
            "[%03u]=%02x [%03u]=%02x [%03u]=%02x [%03u]=%02x\n",
            i,*(ext_csd+i),i+1,*(ext_csd+i+1),i+2,*(ext_csd+i+2),i+3,*(ext_csd+i+3),
            i+4,*(ext_csd+i+4),i+5,*(ext_csd+i+5),i+6,*(ext_csd+i+6),i+7,*(ext_csd+i+7));
    }
    printk(KERN_ERR "\n");
}

#define PF2_SHOW_EXT_CSD 0x01UL
#define PF2_FULL_PARAM   0x02UL
#define PF2_SET_EXT_CSD  0x04UL

#ifdef HACK_BOOT_PART_RW
/*
Note : this function not complete yet.
*/
static void hk_fill_bp(struct device *dev,
                       size_t p_count,
                       unsigned char *cr_param)
{
    u8 *emmc_buf;
    u32 param1;
    u32 param2;
    u32 param3;
    u32 buf_size;
    u8 part_config;
    struct mmc_host *host;
    struct mmc_card *card;

    printk(KERN_ERR "%s(%u)3013/08/15 17:30\n",__func__,__LINE__);

    host = dev_get_drvdata(dev);
    if(host)
        card = host->card;

    if(card == NULL){
        printk(KERN_ERR "card is not exist.\n");
        goto ERR_OUT;
    }

    rtkemmc_chk_param(&param1,8,cr_param);
    rtkemmc_chk_param(&param2,8,cr_param+9);
    rtkemmc_chk_param(&param3,8,cr_param+18);

    printk(KERN_ERR "param1=0x%x\n",param1); //enh_start_addr
    printk(KERN_ERR "param2=0x%x\n",param2); //enh_block_cnt
    printk(KERN_ERR "param3=0x%x\n",param3);

    buf_size = 512;
    part_config = card->ext_csd.part_config;

    if(host && card){
        int idx;
        printk(KERN_ERR "erased byte = 0x%x\n",card->erased_byte);
        printk(KERN_ERR "part_config=0x%x\n",part_config);
        printk(KERN_ERR "card's partition information\n");
        for (idx = 0; idx < card->nr_parts; idx++) {
	        if (card->part[idx].size) {
	            printk(KERN_ERR "\n===> \nPART %u; name:%s\n",
	                    idx,card->part[idx].name);
	            printk(KERN_ERR "    part_cfg = 0x%x; size:0x%x\n",
	                    card->part[idx].part_cfg,
	                    card->part[idx].size >> 9);
	            printk(KERN_ERR "    force_ro = 0x%x; area_type = 0x%x\n",
	                    card->part[idx].force_ro,
	                    card->part[idx].area_type);
                if(card->part[idx].part_cfg < EXT_CSD_PART_CONFIG_ACC_GP0)
                {
                    printk(KERN_ERR "cfg: EXT_CSD_PART_CONFIG_ACC_BOOT%x\n",
                        (card->part[idx].part_cfg)-EXT_CSD_PART_CONFIG_ACC_BOOT0);
                }else{
                    printk(KERN_ERR "cfg: EXT_CSD_PART_CONFIG_ACC_GP%u\n",
                        (card->part[idx].part_cfg)-EXT_CSD_PART_CONFIG_ACC_GP0);
                }
                if(card->part[idx].area_type == MMC_BLK_DATA_AREA_MAIN)
                    printk(KERN_ERR "type: MMC_BLK_DATA_AREA_MAIN\n");
                if(card->part[idx].area_type == MMC_BLK_DATA_AREA_BOOT)
                    printk(KERN_ERR "type: MMC_BLK_DATA_AREA_BOOT\n");
                if(card->part[idx].area_type == MMC_BLK_DATA_AREA_GP)
                    printk(KERN_ERR "type: MMC_BLK_DATA_AREA_GP\n");
	        }
        }
    }

    buf_size = 512 * 16;
    emmc_buf = kmalloc(buf_size, GFP_KERNEL);
    if(!emmc_buf){
        printk(KERN_ERR "emmc_buf is NULL\n");
        goto ERR_OUT;
    }

    memset(emmc_buf, 0, buf_size);

    printk(KERN_ERR "emmc_buf=0x%p\n",emmc_buf);

    mmc_send_data_cmd(MMC_SEND_EXT_CSD,
                      0,1,emmc_buf);

    printk(KERN_ERR "[EXT_CSD] :\n");
    show_ext_csd(emmc_buf);

    rtkemmc_switch_partition(card,BOOT1_PART);

    mmc_send_data_cmd(MMC_SEND_EXT_CSD,
                      0,1,emmc_buf);

    printk(KERN_ERR "{F} [EXT_CSD] :\n");
    show_ext_csd(emmc_buf);

    if(*(emmc_buf+EXT_CSD_PART_CONFIG) & 0x01){
        u32 boot_part_blk_cnt;

        boot_part_blk_cnt = (*(emmc_buf+EXT_CSD_BOOT_MULT))<<8;
        printk(KERN_ERR "have changed to boot partition 1. [%x block]\n",
                boot_part_blk_cnt);
    }

    kfree(emmc_buf);

ERR_OUT:
    return;
}
#endif

static void hk_set_wr_rel(struct device *dev,
                          size_t p_count,
                          unsigned char *cr_param)
{
    u8 *emmc_buf;
    u8 acc_mod;
    u8 index;
    u8 value;
    u8 i;
    u8 idx_lop;
    u8 cmd_set;
    u32 param1;
    u32 param2;
    u32 param3;
    u32 buf_size;
    struct mmc_host *host;
    struct mmc_card *card = NULL;

    printk(KERN_ERR "%s(%u)3013/08/15 17:30\n",__func__,__LINE__);

    if( p_count != 38){
        printk(KERN_ERR "Command format:\n"
               "    echo set_wr_rel=param1,param2,param3 > emmc_hacking;\n"
               "        param1[7:0] is value in byte you want to set.\n"
               "        param2 :\n"
               "            [bit0] TO show all ext_csd.\n"
               "            [bit1] Send CMD6. if this bit is set.\n"
               "                   param1 change to 32 bits parameter of argument of AMD6.\n"
               "        param3 must be 2379beef to make sure you want to do this.\n"
               "ex:\n"
               "    echo set_wr_rel=00000001,00000000,2379beef > emmc_hacking;\n"
               "        param1=0x00000001 enable user data area write reliability\n"
               "        param2=0x00020000 do not show all ext_csd.\n"
               "        param3=2379beef make sure that want to do this.\n"
               "\n"
               "    echo set_wr_rel=03a71f01,00000002,2379beef > emmc_hacking;\n"
               "        param1=0x03a71f01 set ext_cse[0xa7], value=0x1f, cmd_set=1\n"
               "        param2=0x00000002 send CMD6. param1 is argument in 32 bits.\n"
               "        param3=2379beef make sure that want to do this.\n" );
        goto ERR_OUT;
    }

    host = dev_get_drvdata(dev);
    if(host)
        card = host->card;

    // KWarning: checked ok by alexkh@realtek.com
    if(card == NULL){
        printk(KERN_ERR "card is not exist.\n");
        goto ERR_OUT;
    }

    rtkemmc_chk_param(&param1,8,cr_param);
    rtkemmc_chk_param(&param2,8,cr_param+9);
    rtkemmc_chk_param(&param3,8,cr_param+18);

    printk(KERN_ERR "param1=0x%x\n",param1);
    printk(KERN_ERR "param2=0x%x\n",param2);
    printk(KERN_ERR "param3=0x%x\n",param3);

    if(param2 & PF2_FULL_PARAM){
        printk(KERN_ERR "Send CMD6 alert\n");
        acc_mod = (u8)((param1>>24)&0xff);
        index   = (u8)((param1>>16)&0xff);
        value   = (u8)((param1>> 8)&0xff);
        cmd_set = (u8)(param1&0xff);
    }else{
        printk(KERN_ERR "Enable Write Reliability\n");
        acc_mod = MMC_SWITCH_MODE_WRITE_BYTE;
        index   = EXT_CSD_WR_REL_SET;
        value   = (u8)param1;
        cmd_set = 1;
    }

    printk(KERN_ERR "acc_mod=0x%x; index=%u; value=0x%x; cmd_set=0x%x\n",
            acc_mod,index,value,cmd_set);

    buf_size = 512;
    emmc_buf = kmalloc(buf_size, GFP_KERNEL);
    if(!emmc_buf){
        printk(KERN_ERR "emmc_buf is NULL\n");
        goto ERR_OUT;
    }
    //printk(KERN_ERR "emmc_buf=0x%p\n",emmc_buf);

    memset(emmc_buf, 0, 512);
    mmc_send_data_cmd(MMC_SEND_EXT_CSD,
                      0,1,emmc_buf);

    printk(KERN_ERR "[EXT_CSD] :\n");
    if(param2){
        if(param2 & PF2_SHOW_EXT_CSD)
            show_ext_csd(emmc_buf);
        if(param2 & PF2_FULL_PARAM){
            if( index==249 || index==242 ||
                index==212 || index==136 )
                idx_lop = 4;
            else if( index==157 || index==140)
                idx_lop = 3;
            else if( index==143)
                idx_lop = 12;
            else
                idx_lop = 1;

            for(i=0; i<idx_lop; i++)
                printk(KERN_ERR "    [%03u]=%02x\n",index+i,*(emmc_buf+index+i));
        }

    }else{
        printk(KERN_ERR "    [%03u]=%02x [%03u]=%02x\n",
                    166,*(emmc_buf+166),
                    167,*(emmc_buf+167));
    }

    if(index == EXT_CSD_WR_REL_SET){
        if((*(emmc_buf+EXT_CSD_WR_REL_PARAM) & 0x05) == 0x05 ){
            if(*(emmc_buf+EXT_CSD_PARTITION_SETTING_COMP) & 0x01){
                printk(KERN_ERR "This chip PARTITION configuration have completed\n");
                printk(KERN_ERR "  ENH_SATRT_ADDR = 0x%08x\n",
                          (*(emmc_buf+EXT_CSD_ENH_START_ADDR+3)<<24) |
                          (*(emmc_buf+EXT_CSD_ENH_START_ADDR+2)<<16) |
                          (*(emmc_buf+EXT_CSD_ENH_START_ADDR+1)<<8) |
                          (*(emmc_buf+EXT_CSD_ENH_START_ADDR)));
                printk(KERN_ERR "  ENH_SIZE_MULT  = 0x%06x\n",
                          (*(emmc_buf+EXT_CSD_ENH_SIZE_MULT+2)<<16) |
                          (*(emmc_buf+EXT_CSD_ENH_SIZE_MULT+1)<<8) |
                          (*(emmc_buf+EXT_CSD_ENH_SIZE_MULT)));
                printk(KERN_ERR "  PARTITION_ATTRIBUTE  = 0x%06x\n",
                        (*(emmc_buf+EXT_CSD_PARTITION_ATTRIBUTE)));
                printk(KERN_ERR "  WR_REL_SET  = 0x%x\n",
                        (*(emmc_buf+EXT_CSD_WR_REL_SET)));
                goto FINISH_OUT;
            }

        }else{
            printk(KERN_ERR "Device not support setting write reliability\n");
            printk(KERN_ERR "  WR_REL_PARAM = 0x%x\n",
                      *(emmc_buf+EXT_CSD_WR_REL_PARAM));
            goto FINISH_OUT;
        }
    }

    if(param3 == 0x2379beef)
        rtkemmc_switch(card,acc_mod,index,value,cmd_set);
    else{
        printk(KERN_ERR "param3 != 0x2379beef skip command.\n");
        goto FINISH_OUT;
    }

    memset(emmc_buf, 0, 512);
    mmc_send_data_cmd(MMC_SEND_EXT_CSD,
                      0,1,emmc_buf);

    printk(KERN_ERR "{F} [EXT_CSD] :\n");
    if(param2){
        if(param2 & PF2_SHOW_EXT_CSD)
            show_ext_csd(emmc_buf);
        if(param2 & PF2_FULL_PARAM){
            if( index==249 || index==242 ||
                index==212 || index==136 )
                idx_lop = 4;
            else if( index==157 || index==140)
                idx_lop = 3;
            else if( index==143)
                idx_lop = 12;
            else
                idx_lop = 1;

            for(i=0; i<idx_lop; i++)
                printk(KERN_ERR "    [%03u]=%02x\n",index+i,*(emmc_buf+index+i));
        }
    }else{
        printk(KERN_ERR "    [%03u]=%02x [%03u]=%02x\n",
                    166,*(emmc_buf+166),
                    167,*(emmc_buf+167));
    }

FINISH_OUT:
    kfree(emmc_buf);

ERR_OUT:
    return;

}

static void hk_red_ext_csd(struct device *dev,
                           size_t p_count,    //39
                           unsigned char *cr_param)
{
    u8 *emmc_buf;
    u32 param1; //target emmc address
    u32 param2; //block number
    u32 param3; //1: write; 0: read
    u32 buf_size ;
    struct mmc_host *host;
    struct mmc_card *card = NULL;

    printk(KERN_ERR "%s(%u)3013/08/15 17:30\n",__func__,__LINE__);
    host = dev_get_drvdata(dev);
    if(host)
        card = host->card;

    // KWarning: checked ok by alexkh@realtek.com
    if(card == NULL){
        printk(KERN_ERR "card is not exist.\n");
        goto ERR_OUT;
    }

    rtkemmc_chk_param(&param1,8,cr_param);
    rtkemmc_chk_param(&param2,8,cr_param+9);
    rtkemmc_chk_param(&param3,8,cr_param+18);

    printk(KERN_ERR "param1=0x%x\n",param1); //enh_start_addr
    printk(KERN_ERR "param2=0x%x\n",param2); //enh_block_cnt
    printk(KERN_ERR "param3=0x%x\n",param3);

    buf_size = 512;
    emmc_buf = kmalloc(buf_size, GFP_KERNEL);
    if(!emmc_buf){
        printk(KERN_ERR "emmc_buf is NULL\n");
        goto ERR_OUT;
    }
    printk(KERN_ERR "emmc_buf=0x%p\n",emmc_buf);

    memset(emmc_buf, 0, 512);
    mmc_send_data_cmd(MMC_SEND_EXT_CSD,
                      0,1,emmc_buf);

    printk(KERN_ERR "[EXT_CSD] :\n");

    if(param1 && !(param2 & PF2_SHOW_EXT_CSD)){
        u8 i;
        u8 item_cnt;
        for(i=0;i<4;i++){
            item_cnt = param1>>(i*8);
            printk(KERN_ERR "    [%03u]=%02x ",item_cnt,*(emmc_buf+item_cnt));
        }
        printk(KERN_ERR "\n");
    }else{
        show_ext_csd(emmc_buf);
    }
    kfree(emmc_buf);

ERR_OUT:
    return;
}

static void hk_set_enh_user_area(struct device *dev,
                                 size_t p_count,
                                 unsigned char *cr_param)
{
    u8 *emmc_buf;
    u32 param1;
    u32 param2;
    u32 param3;
    u32 buf_size ;
    struct mmc_host *host;
    struct mmc_card *card = NULL;

    printk(KERN_ERR "%s(%u)3013/08/15 17:30\n",__func__,__LINE__);
    host = dev_get_drvdata(dev);
    if(host)
        card = host->card;

    // KWarning: checked ok by alexkh@realtek.com
    if(card == NULL){
        printk(KERN_ERR "card is not exist.\n");
        goto ERR_OUT;
    }
    rtkemmc_chk_param(&param1,8,cr_param);
    rtkemmc_chk_param(&param2,8,cr_param+9);
    rtkemmc_chk_param(&param3,8,cr_param+18);

    printk(KERN_ERR "param1=0x%x\n",param1); //enh_start_addr
    printk(KERN_ERR "param2=0x%x\n",param2); //enh_block_cnt
    printk(KERN_ERR "param3=0x%x\n",param3);

    buf_size = 512;
    emmc_buf = kmalloc(buf_size, GFP_KERNEL);
    if(!emmc_buf){
        printk(KERN_ERR "emmc_buf is NULL\n");
        goto ERR_OUT;
    }
    printk(KERN_ERR "emmc_buf=0x%p\n",emmc_buf);

    memset(emmc_buf, 0, 512);
    mmc_send_data_cmd(MMC_SEND_EXT_CSD,
                      0,1,emmc_buf);

    printk(KERN_ERR "[EXT_CSD] :\n");
    if((*(emmc_buf+EXT_CSD_PARTITION_SUPPORT) &0x03) == 0x03){
        u32 i;
        u32 max_enh_size_mult;
        u32 enh_size_base_blk;
        u32 sec_cnt;
        u8 hc_wp_grp_size;
        u8 hc_erase_grp_size;

        hc_wp_grp_size = *(emmc_buf+EXT_CSD_HC_WP_GRP_SIZE);
        hc_erase_grp_size = *(emmc_buf+EXT_CSD_HC_ERASE_GRP_SIZE);

        sec_cnt = (*(emmc_buf+EXT_CSD_SEC_CNT+3)<<24) |
                  (*(emmc_buf+EXT_CSD_SEC_CNT+2)<<16) |
                  (*(emmc_buf+EXT_CSD_SEC_CNT+1)<<8) |
                  (*(emmc_buf+EXT_CSD_SEC_CNT));

        max_enh_size_mult = (*(emmc_buf+EXT_CSD_MAX_ENH_SIZE_MULT+2)<<16) |
                            (*(emmc_buf+EXT_CSD_MAX_ENH_SIZE_MULT+1)<<8) |
                            (*(emmc_buf+EXT_CSD_MAX_ENH_SIZE_MULT));

        enh_size_base_blk = (u32)hc_wp_grp_size *
                            (u32)hc_erase_grp_size * 1024;

        printk(KERN_ERR "  ERASE_GROUP_DEF = 0x%x\n",
                *(emmc_buf+EXT_CSD_ERASE_GROUP_DEF));
        printk(KERN_ERR "  HC_WP_GRP_SIZE = 0x%x\n",hc_wp_grp_size);
        printk(KERN_ERR "  HC_ERASE_GRP_SIZE = 0x%x\n",hc_erase_grp_size);
        printk(KERN_ERR "  SEC_CNT = 0x%x\n",sec_cnt);
        printk(KERN_ERR "  MAX_ENH_SIZE_MULT = 0x%x\n",max_enh_size_mult);
        printk(KERN_ERR "Enhanced User Data Area base=\n"
               "            %u kBytes(0x%x block)\n",
                (enh_size_base_blk>>1),enh_size_base_blk);
        printk(KERN_ERR "Enhanced User Data Area x Size=\n"
               "            ENH_SIZE_MULT x HC_WP_GRP_SIZE x\n"
               "            HC_ERASE_GRP_SIZE x 512 kBytes\n");

        if(*(emmc_buf+EXT_CSD_PARTITION_SETTING_COMP) & 0x01){
            printk(KERN_ERR "This chip PARTITION configuration have completed\n");
            printk(KERN_ERR "  ENH_SATRT_ADDR = 0x%08x\n",
                      (*(emmc_buf+EXT_CSD_ENH_START_ADDR+3)<<24) |
                      (*(emmc_buf+EXT_CSD_ENH_START_ADDR+2)<<16) |
                      (*(emmc_buf+EXT_CSD_ENH_START_ADDR+1)<<8) |
                      (*(emmc_buf+EXT_CSD_ENH_START_ADDR)));
            printk(KERN_ERR "  ENH_SIZE_MULT  = 0x%06x\n",
                      (*(emmc_buf+EXT_CSD_ENH_SIZE_MULT+2)<<16) |
                      (*(emmc_buf+EXT_CSD_ENH_SIZE_MULT+1)<<8) |
                      (*(emmc_buf+EXT_CSD_ENH_SIZE_MULT)));
            printk(KERN_ERR "  PARTITION_ATTRIBUTE  = 0x%06x\n",
                    (*(emmc_buf+EXT_CSD_PARTITION_ATTRIBUTE)));

            goto FINISH_OUT;
        }

        if(param2 > enh_size_base_blk){
            i = param2 % enh_size_base_blk;
            param2 = (param2/enh_size_base_blk);
            if(i)
                param2 += 1;
        }else{
            param2 = 1;
        }
        if(param2 > max_enh_size_mult)
            param2 = max_enh_size_mult;

        printk(KERN_ERR "ENH_SATRT_ADDR = 0x%08x\n",param1);
        printk(KERN_ERR "ENH_SIZE_MULT  = 0x%06x\n",param2);

        if(p_count == 38 && (param3 == 0x2379beef)){
            u32 part_switch_time;
            /* doing set enhance user data param to ext_csd */
            /* setting ENH_SATRT_ADDR */
            rtkemmc_switch(card,MMC_SWITCH_MODE_WRITE_BYTE,
                         EXT_CSD_ENH_START_ADDR,(u8)(param1&0xff),1);
            rtkemmc_switch(card,MMC_SWITCH_MODE_WRITE_BYTE,
                         EXT_CSD_ENH_START_ADDR+1,(u8)((param1>>8)& 0xff),1);
            rtkemmc_switch(card,MMC_SWITCH_MODE_WRITE_BYTE,
                         EXT_CSD_ENH_START_ADDR+2,(u8)((param1>>16)& 0xff),1);
            rtkemmc_switch(card,MMC_SWITCH_MODE_WRITE_BYTE,
                         EXT_CSD_ENH_START_ADDR+3,(u8)((param1>>24)& 0xff),1);
            /* setting ENH_SIZE_MULT */
            rtkemmc_switch(card,MMC_SWITCH_MODE_WRITE_BYTE,
                         EXT_CSD_ENH_SIZE_MULT,(u8)(param2& 0xff),1);
            rtkemmc_switch(card,MMC_SWITCH_MODE_WRITE_BYTE,
                         EXT_CSD_ENH_SIZE_MULT+1,(u8)((param2>>8)& 0xff),1);
            rtkemmc_switch(card,MMC_SWITCH_MODE_WRITE_BYTE,
                         EXT_CSD_ENH_SIZE_MULT+2,(u8)((param2>>16)& 0xff),1);
            /* settig user data area to enhance mode */
            rtkemmc_switch(card,MMC_SWITCH_MODE_WRITE_BYTE,
                         EXT_CSD_PARTITION_ATTRIBUTE,0x01,1);
            /* setting partition configuration complete */
            rtkemmc_switch(card,MMC_SWITCH_MODE_WRITE_BYTE,
                         EXT_CSD_PARTITION_SETTING_COMP,0x01,1);

            part_switch_time = *(emmc_buf+EXT_CSD_PART_SWITCH_TIME);
            printk(KERN_ERR "Partition Switch Time is %u0ms.\n",part_switch_time);
            mdelay(10*(part_switch_time+10));
        }else{
            printk(KERN_ERR "Command format:\n"
                   "    echo set_ehuser=param1,param2,param3 > emmc_hacking;\n"
                   "        param1 is enhance user data area start address in block(8 hex).\n"
                   "        param2 is enhance user data area size in block(8 hex).\n"
                   "        param3 must be 2379beef to make sure you want to do this.\n"
                   "ex:\n"
                   "    echo set_ehuser=00000000,00020000,2379beef > emmc_hacking;\n"
                   "        param1=0x00000000 EUDA start address is 0.\n"
                   "        param2=0x00020000 EUDA size is 0x2000.\n"
                   "        param3=2379beef make sure that want to do this.\n" );
        }

    }else{
        printk(KERN_ERR "device do not support enhance uaer data area.\n");
    }
FINISH_OUT:
    kfree(emmc_buf);

ERR_OUT:
    return;
}
#endif

/* ========================================================
 * blk_addr    : eMMC read/write target address, block base.
 * data_size   : tarnsfer data size, block base.
 * buffer      : DMA address
 * rw_mode     : fast read or ast write
 ========================================================== */
#define FAST_READ   0x1278
#define FAST_WRITE  0x3478
static int mmc_fast_rw( unsigned int blk_addr,
                   unsigned int data_size,
                   unsigned char * buffer,
                   unsigned int rw_mode )
{
    int err = 0;
    struct mmc_host * mmc = mmc_host_local;
    unsigned long flags;

    if(mmc){
        struct rtkemmc_host *emmc_port;
        struct mmc_card *card;
        struct mmc_request mrq = {0};
        struct mmc_command cmd = {0};
	    struct mmc_data	data   = {0};
	    struct scatterlist sg;

        emmc_port = mmc_priv(mmc);
        mmc_claim_host(mmc);
        spin_lock_irqsave(&emmc_port->lock,flags);

        card = mmc->card;

        if((emmc_port->rtflags & RTKCR_USER_PARTITION)==0){
            int try_loop = 3;
            do{
                err = 0;
                spin_unlock_irqrestore(&emmc_port->lock, flags);
                err = rtkemmc_switch_user_partition(card);
                spin_lock_irqsave(&emmc_port->lock,flags);

            }while(try_loop-- && err );
            if(err){
                spin_unlock_irqrestore(&emmc_port->lock, flags);
                mmc_release_host(mmc);
                goto ERR_OUT;
            }
            else
                emmc_port->rtflags |= RTKCR_USER_PARTITION;
        }

        if(data_size>1){

            if(rw_mode == FAST_WRITE)
                cmd.opcode = MMC_WRITE_MULTIPLE_BLOCK;
            else
                cmd.opcode = MMC_READ_MULTIPLE_BLOCK;

        }else{

            if(rw_mode == FAST_WRITE)
                cmd.opcode = MMC_WRITE_BLOCK;
            else
                cmd.opcode = MMC_READ_SINGLE_BLOCK;
        }

	    cmd.arg = blk_addr;
	    cmd.flags = MMC_RSP_R1 | MMC_CMD_ADTC;

        if(data_size)
        {   //date info setting
    		data.sg = &sg;
    		data.sg_len = 1;
    		data.blksz = 512;
    		data.blocks = data_size;

    		sg_init_one(data.sg, buffer, (data_size<<9));

            if(rw_mode == FAST_WRITE)
    	        data.flags = MMC_DATA_WRITE;
            else
    		    data.flags = MMC_DATA_READ;

            mmc_set_data_timeout(&data, card);
        }

	    data.mrq = &mrq;
	    cmd.mrq = &mrq;
	    cmd.data = &data;
        cmd.retries =5;

CMD_RETRY:
        mrq.data = &data;
        mrq.cmd = &cmd;

        if(emmc_port->mrq){
            emmc_port->mrq = NULL;
        }
        cmd.error = 0;
        data.error = 0;
        spin_unlock_irqrestore(&emmc_port->lock, flags);
        mmc_wait_for_req(mmc, &mrq);
        spin_lock_irqsave(&emmc_port->lock,flags);
        err = cmd.error;
        if((err==0) && (rw_mode==FAST_WRITE))
        {
            spin_unlock_irqrestore(&emmc_port->lock, flags);
            err = rtkemmc_wait_status(mmc->card,STATE_TRAN,0,0);
            spin_lock_irqsave(&emmc_port->lock,flags);
        }
        if(err && cmd.retries){
            printk("%s(%u)last retry %d counter.\n",
                    __func__,__LINE__,cmd.retries);
            cmd.retries--;
            goto CMD_RETRY;
        }
        spin_unlock_irqrestore(&emmc_port->lock, flags);
        mmc_release_host(mmc);
    }else{
        err = -ENODEV;
    }

ERR_OUT:
    if(err)
        printk("err=%d\n",err);
    return err;
}

#define MAX_XFER_BLK_A    0x100
#define MAX_XFER_BLK_B    0x400
static int mmc_fast_rw_loop(unsigned int blk_addr,
                            unsigned int data_size,
                            unsigned char * buffer,
                            unsigned int rw_mode )
{
    int err = 0;

    unsigned int tmp_addr   = blk_addr;
    unsigned int tmp_size   = data_size;
    unsigned int org_size   = data_size;
    unsigned char * tmp_buf = buffer;
    unsigned int max_xfer_blk;
    do{
        /* max 1M bytes read/write per transfer */

        max_xfer_blk = MAX_XFER_BLK_B;

        if(data_size > max_xfer_blk){
            tmp_size = max_xfer_blk;
        }else{
            tmp_size = data_size;
        }

        err = mmc_fast_rw(tmp_addr,tmp_size,tmp_buf,rw_mode);

        if(err)
            break;

        if(data_size > max_xfer_blk){
            tmp_addr    += max_xfer_blk;
            data_size   -= max_xfer_blk;
            tmp_buf     += (max_xfer_blk<<9);
        }else{
            data_size = 0;
        }

    }while(data_size);

    if(err == 0)
         return org_size-data_size;
    else
        return err;
}

int mmc_fast_read( unsigned int blk_addr,
                   unsigned int data_size,
                   unsigned char * buffer )
{
    int err = 0;
    err = mmc_fast_rw_loop(blk_addr,data_size,buffer,FAST_READ);
    return err;
}
EXPORT_SYMBOL(mmc_fast_read);

int mmc_fast_write( unsigned int blk_addr,
                    unsigned int data_size,
                    unsigned char * buffer )
{
    int err = 0;
    if(blk_addr<0x20000){
        printk("target small then save area.\n");
        err = -1;
    }
    err = mmc_fast_rw_loop(blk_addr,data_size,buffer,FAST_WRITE);
    return err;
}
EXPORT_SYMBOL(mmc_fast_write);

int mmc_send_cmd0(void)
{
    int err = 0;
    //struct mmc_host * mmc = mmc_host_local;
    unsigned long flags;
    struct mmc_host *host;
    struct mmc_card *card = NULL;
    struct mmc_host * mmc = mmc_host_local;

    printk("%s(%u)\n",__func__,__LINE__);

#if 1
    if(mmc){
        mmc_claim_host(mmc);
        card = mmc->card;
    }
    else
    {
    	printk("%s(%u) mmc == NULL\n",__func__,__LINE__);
	return -6; 
    }
#else
    host = dev_get_drvdata(dev);
    if(host)
        card = host->card;
#endif

    // KWarning: checked ok by alexkh@realtek.com
    if(card == NULL){
        printk(KERN_ERR "card is not exist.\n");
	err = -ENODEV;
        goto ERR_OUT;
    }
    err = rtkemmc_go_idle(card);

ERR_OUT:
    return -5;
}
EXPORT_SYMBOL(mmc_send_cmd0);

/* mmc device attribute *********************************************************** */
#define TEST_BLK_SIZE 10
void rtk_hexdump(const char *str, const void *buf, unsigned int length)
{
        unsigned int i;
        char *ptr = (char *)buf;

        if ((buf == NULL) || (length == 0)) {
                printk("NULL\n");
                return;
        }
        printk(str == NULL ? __func__ : str);
	printk(" (0x%08x)\n", (unsigned int)buf);

        for (i = 0; i < length; i++) {
		printk("0x%02x",(unsigned int)(ptr[i]));

                if ((i & 0xf) == 0xf)
                        printk("\n");
                else
                        printk(" ");
        }
        printk("\n");
}
EXPORT_SYMBOL(rtk_hexdump);

static ssize_t
cr_send_cmd0_dev_show(struct device *dev,
                     struct device_attribute *attr,
                     const char *buf,
                     size_t count)
{
    struct mmc_host * host = dev_get_drvdata(dev);

    printk("%s(%u)\n",__func__,__LINE__);
    return sprintf(buf, "send cmd0\n");
}
static ssize_t
cr_send_cmd0_dev_store(struct device *dev,
                     struct device_attribute *attr,
                     const char *buf,
                     size_t count)
{
	int err=0;

	//err = mmc_send_cmd0(dev);
	err = mmc_send_cmd0();
	return err;
}
DEVICE_ATTR(cr_send_cmd0, S_IRUGO | S_IWUSR,
            cr_send_cmd0_dev_show,cr_send_cmd0_dev_store);

static ssize_t
cr_fast_RW_dev_show(struct device *dev, struct device_attribute *attr,
        char *buf)
{
    struct mmc_host * host = dev_get_drvdata(dev);

    printk("%s(%u)\n",__func__,__LINE__);

    if(host && host->card){
        rtkemmc_switch_user_partition(host->card);
    }
    return sprintf(buf, "send SWITCH command\n");
}

static ssize_t
cr_fast_RW_dev_store(struct device *dev,
                     struct device_attribute *attr,
                     const char *buf,
                     size_t count)
{

    unsigned char *cr_param;


    printk("%s(%u)\n",__func__,__LINE__);
    printk("%s\n",buf);
    printk("count=%d\n",count);

    cr_param=(char *)rtkemmc_parse_token(buf,"cr_param");

    if(cr_param){
        u8 *emmc_buf;
        u32 param1; //target emmc address
        u32 param2; //block number
        u32 param3; //1: write; 0: read

        rtkemmc_chk_param(&param1,8,cr_param);
        rtkemmc_chk_param(&param2,8,cr_param+9);
        rtkemmc_chk_param(&param3,8,cr_param+18);
        printk("param1=0x%x param2=0x%x param3=0x%x\n",
                param1,param2,param3);

        emmc_buf = kmalloc(BYTE_CNT*param2, GFP_KERNEL);
        if(!emmc_buf){
            printk("emmc_buf is NULL\n");
            goto ERR_OUT;
        }

        printk("emmc_buf=0x%p\n",emmc_buf);

        if(param3 == 1){
            mmc_fast_write(param1, param2, emmc_buf );
        }else{
            mmc_fast_read(param1, param2, emmc_buf );
        }
        kfree(emmc_buf);
    }else{
        printk("have no parameter searched.\n");
    }

ERR_OUT:
    /*
    return value must be equare or big then "count"
    to finish this attribute
    */

    return count;
}
DEVICE_ATTR(cr_fast_RW, S_IRUGO | S_IWUSR,
            cr_fast_RW_dev_show,cr_fast_RW_dev_store);

static ssize_t
em_open_log_dev_show(struct device *dev, struct device_attribute *attr,
        char *buf)
{
    struct mmc_host * host = dev_get_drvdata(dev);
    struct rtkemmc_host *emmc_port = mmc_priv(host);

    if(emmc_port->rtflags & RTKCR_FOPEN_LOG){
        emmc_port->rtflags &= ~RTKCR_FOPEN_LOG;
    }else{
        emmc_port->rtflags |=  RTKCR_FOPEN_LOG;
    }

    return sprintf(buf, "%s log %s\n",
            DRIVER_NAME,
            (emmc_port->rtflags & RTKCR_FOPEN_LOG)?"open":"close");
}

static ssize_t
em_open_log_dev_store(struct device *dev,
                     struct device_attribute *attr,
                     const char *buf,
                     size_t count)
{

    printk("%s(%u)Not thing to do.\n",__func__,__LINE__);

    return count;
}
DEVICE_ATTR(em_open_log, S_IRUGO | S_IWUSR,
              em_open_log_dev_show,em_open_log_dev_store);

#ifdef CONFIG_MMC_RTKEMMC_HK_ATTR
static ssize_t
emmc_hacking_dev_show(struct device *dev, struct device_attribute *attr,
        char *buf)
{
    //struct mmc_host * host = dev_get_drvdata(dev);
    //struct mmc_card *card = host->card;

    printk(KERN_ERR "%s(%u)3013/08/12 10:55\n",__func__,__LINE__);

    printk(KERN_ERR "Supported hacking below:\n");
#ifdef HACK_BOOT_PART_RW
    printk(KERN_ERR "    fill_par    : fill data  to specific partition\n");
#endif
    printk(KERN_ERR "    set_wr_rel  : enable write reliability.\n");
    printk(KERN_ERR "    red_ext_csd : show ext_csd.\n");
    printk(KERN_ERR "    set_ehuser  : enable enhance user date area.\n");

    return sprintf(buf, "emmc_hacking_dev_show\n");
}

static ssize_t
emmc_hacking_dev_store(struct device *dev,
                     struct device_attribute *attr,
                     const char *buf,
                     size_t count)
{
    unsigned char *cr_param;

    printk(KERN_ERR "count=%d\n",count);

#ifdef HACK_BOOT_PART_RW
    cr_param=(char *)rtkemmc_parse_token(buf,"fill_par");
    if(cr_param){
        hk_fill_bp(dev,count,cr_param);
        goto FINISH_OUT;
    }
#endif //#ifdef HACK_BOOT_PART_RW

    cr_param=(char *)rtkemmc_parse_token(buf,"set_wr_rel");
    if(cr_param){
        hk_set_wr_rel(dev,count,cr_param);
        goto FINISH_OUT;
    }

    cr_param=(char *)rtkemmc_parse_token(buf,"red_ext_csd");
    if(cr_param){
        hk_red_ext_csd(dev,count,cr_param);
        goto FINISH_OUT;
    }

    cr_param=(char *)rtkemmc_parse_token(buf,"set_ehuser");
    if(cr_param){
        hk_set_enh_user_area(dev,count,cr_param);
        goto FINISH_OUT;
    }

    printk(KERN_ERR "have no match command!!\n");

FINISH_OUT:
    /*
    return value must be equare or big then "count"
    to finish this attribute
    */
    return count;
}
DEVICE_ATTR(emmc_hacking, S_IRUGO | S_IWUSR,
            emmc_hacking_dev_show,emmc_hacking_dev_store);

#endif

static ssize_t
emmc_info_dev_show(struct device *dev, struct device_attribute *attr,
        char *buf)
{
    struct mmc_host * host = dev_get_drvdata(dev);
    struct mmc_card *card = host->card;
    struct rtkemmc_host *emmc_port = mmc_priv(host);

    printk(KERN_INFO "%s(%u)\n",__func__,__LINE__);
    return sprintf(buf, "EMMC_STATUS=0x%08x SYS_PLL_EMMC1=0x%08x SYS_PLL_EMMC2=0x%08x \nSYS_PLL_EMMC3=0x%08x SYS_PLL_EMMC4=0x%08x EMMC_RINTSTS=0x%08x \nEMMC_IDSTS=0x%08x muxpad0=0x%08x muxpad1=0x%08x \nEMMC_PFUNC_NF1=0x%08x EMMC_PDRIVE_NF1=0x%08x EMMC_PDRIVE_NF2=0x%08x \nEMMC_PDRIVE_NF3=0x%08x EMMC_CTYPE=0x%08x EMMC_UHSREG=0x%08x \nEMMC_DDR_REG=0x%08x EMMC_CARD_THR_CTL=0x%08x EMMC_CLKDIV=0x%08x \nEMMC_CKGEN_CTL=0x%08x EMMC_DQS_CTRL1=0x%08x \n",readl(emmc_port->emmc_membase+EMMC_STATUS),readl(emmc_port->crt_membase + SYS_PLL_EMMC1),readl(emmc_port->crt_membase + SYS_PLL_EMMC2),readl(emmc_port->crt_membase + SYS_PLL_EMMC3),readl(emmc_port->crt_membase + SYS_PLL_EMMC4),readl(emmc_port->emmc_membase+EMMC_RINTSTS), readl(emmc_port->emmc_membase+EMMC_IDSTS), readl(emmc_port->emmc_membase+EMMC_muxpad0),readl(emmc_port->emmc_membase+EMMC_muxpad1),readl(emmc_port->emmc_membase+EMMC_PFUNC_NF1),readl(emmc_port->emmc_membase+EMMC_PDRIVE_NF1),readl(emmc_port->emmc_membase+EMMC_PDRIVE_NF2),readl(emmc_port->emmc_membase+EMMC_PDRIVE_NF3),readl(emmc_port->emmc_membase+EMMC_CTYPE),readl(emmc_port->emmc_membase+EMMC_UHSREG),readl(emmc_port->emmc_membase+EMMC_DDR_REG),readl(emmc_port->emmc_membase+EMMC_CARD_THR_CTL),readl(emmc_port->emmc_membase+EMMC_CLKDIV),readl(emmc_port->emmc_membase+EMMC_CKGEN_CTL),readl(emmc_port->emmc_membase+EMMC_DQS_CTRL1));
}

static ssize_t
emmc_info_dev_store(struct device *dev,
                     struct device_attribute *attr,
                     const char *buf,
                     size_t count)
{
    unsigned char *cr_param;

    printk(KERN_ERR "%s(%u)Nothing to do\n",__func__,__LINE__);

    /*
    return value must be equare or big then "count"
    to finish this attribute
    */
    return count;
}
DEVICE_ATTR(emmc_info, S_IRUGO | S_IWUSR,
            emmc_info_dev_show,emmc_info_dev_store);

static const struct of_device_id rtkemmc_ids[] = {
	{ .compatible = "Realtek,rtk1295-emmc" },
	{ /* Sentinel */ },
};
MODULE_DEVICE_TABLE(of, rtkemmc_ids);

static int rtkemmc_probe(struct platform_device *pdev)
{
    struct mmc_host *mmc = NULL;
    struct rtkemmc_host *emmc_port = NULL;
    struct resource *pResources = NULL;
    int ret = 0, irq = 0;
    int att_err;
    const u32 *prop;
    int err,size,speed_step=0;
    struct device_node *emmc_node = NULL;
    void __iomem *iobase;
    int i;
    int counter;	
    unsigned long flags2;

#ifdef EMMC_SHOUTDOWN_PROTECT
    u64 rtk_tmp_gpio;
#endif
    
    MMCPRINTF("\n");

    //set_RTK_initial_flag(1);
    emmc_node = pdev->dev.of_node;

    if (!emmc_node)
    	printk(KERN_ERR "%s : No emmc of_node found\n",DRIVER_NAME);
    else
    	printk(KERN_ERR "%s : emmc of_node found\n",DRIVER_NAME);


    mmc = mmc_alloc_host(sizeof(struct rtkemmc_host), &pdev->dev);


    if (!mmc) {
        ret = -ENOMEM;
        goto out;
    }
    mmc_host_local = mmc;

    emmc_port = mmc_priv(mmc);
    memset(emmc_port, 0, sizeof(struct rtkemmc_host));

    emmc_port->mmc = mmc;
    emmc_port->dev = &pdev->dev;
    emmc_port->ops = &emmc_ops;

    sema_init(&emmc_port->sem,1);
    sema_init(&emmc_port->sem_op_end,1);

    mmc->ocr_avail = MMC_VDD_30_31 | MMC_VDD_31_32 |
                     MMC_VDD_32_33 | MMC_VDD_33_34 | MMC_VDD_165_195;

    att_err = device_create_file(&pdev->dev, &dev_attr_cr_send_cmd0);
    att_err = device_create_file(&pdev->dev, &dev_attr_cr_fast_RW);
    att_err = device_create_file(&pdev->dev, &dev_attr_em_open_log);
    att_err = device_create_file(&pdev->dev, &dev_attr_emmc_info);

	/* Request IRQ */
    irq = irq_of_parse_and_map(emmc_node, 0);
    if (irq <= 0) {
		printk(KERN_ERR "%s : fail to parse of irq.\n",DRIVER_NAME);
		return -ENXIO;
    }
    MMCPRINTF(KERN_INFO "%s : IRQ = 0x%08x\n",DRIVER_NAME, irq);


	//map emmc registers
    pResources = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if (!pResources || irq < 0){
	printk(KERN_ERR "%s : fail to get resources or irq\n",DRIVER_NAME);
        return -ENXIO;
    }
    emmc_port->emmc_membase = devm_ioremap(&pdev->dev, pResources->start,
                                         resource_size(pResources));
    MMCPRINTF(KERN_INFO "%s : pResources 0= 0x%08x, emmc_iobase = 0x%08x, start = 0x%08x, size = 0x%08x\n",DRIVER_NAME, pResources,emmc_port->emmc_membase,pResources->start,resource_size(pResources));

	//map crt registers
	pResources = platform_get_resource(pdev, IORESOURCE_MEM, 1);
    if (!pResources || irq < 0){
	printk(KERN_ERR "%s : fail to get resources or irq\n",DRIVER_NAME);
        return -ENXIO;
    }
    emmc_port->crt_membase = devm_ioremap(&pdev->dev, pResources->start,
                                         resource_size(pResources));
    MMCPRINTF(KERN_INFO "%s : pResources 1= 0x%08x, crt_iobase = 0x%08x, start = 0x%08x, size = 0x%08x\n",DRIVER_NAME, pResources,emmc_port->crt_membase,pResources->start,resource_size(pResources));

	//map sb2 registers
	pResources = platform_get_resource(pdev, IORESOURCE_MEM, 2);
    if (!pResources || irq < 0){
	printk(KERN_ERR "%s : fail to get resources or irq\n",DRIVER_NAME);
        return -ENXIO;
    }
    emmc_port->sb2_membase = devm_ioremap(&pdev->dev, pResources->start,
                                         resource_size(pResources));
    MMCPRINTF(KERN_INFO "%s : pResources 2 0x%08x, sb2_iobase = 0x%08x, start = 0x%08x, size = 0x%08x\n",DRIVER_NAME, pResources,emmc_port->sb2_membase,pResources->start,resource_size(pResources));
	
	//map misc registers
#if defined(EMMC_LA_DEBUG_GPIO) || defined(EMMC_PARAM_TEST)
	pResources = platform_get_resource(pdev, IORESOURCE_MEM, 3);
    if (!pResources || irq < 0){
	printk(KERN_ERR "%s : fail to get resources or irq\n",DRIVER_NAME);
        return -ENXIO;
    }
    emmc_port->misc_membase = devm_ioremap(&pdev->dev, pResources->start,
                                         resource_size(pResources));
    MMCPRINTF(KERN_INFO "%s : pResources 3 0x%08x, misc_iobase = 0x%08x, start = 0x%08x, size = 0x%08x\n",DRIVER_NAME, pResources,emmc_port->misc_membase,pResources->start,resource_size(pResources));
#endif

#ifdef DBG_PORT	
	sb2_debug = devm_ioremap(&pdev->dev, 0x9801a954,4);
	//DBG port
	rtkemmc_writel(0x5, emmc_port->emmc_membase + EMMC_MAIN2_DBG);
	rtkemmc_writel(0x7208,sb2_debug);
	rtkemmc_writel(0x2db7, emmc_port->emmc_membase + EMMC_DBG);
	isb();
	sync(emmc_port);
#endif

    prop = of_get_property(pdev->dev.of_node, "speed-step", &size);
    if (prop)
    {
        	speed_step = of_read_number(prop, 1);
        	printk(KERN_INFO "[%s] get speed-step : %d \n",__func__,speed_step);
    } else 
    {
        	printk(KERN_ERR "[%s] get speed-step error !! %d \n",__func__,err);
    }

    prop = of_get_property(pdev->dev.of_node, "pddrive_nf_s0", &size);
    if (prop)
    {	
		if (size)
			counter = size / sizeof(u32);
		
		for (i=0; i<counter; i+=1) {
                	pddrive_nf_s0[i]= of_read_number(prop, 1 + i);
			/* KERN_ERR to let be happy */
        		printk(KERN_ERR "[%s] get driving s0 : 0x%x\n",__func__,pddrive_nf_s0[i]);   
		}
    } else 
    {
        	printk(KERN_INFO "[%s] no driving nf_s0 warning !! \n",__func__);
    }

    prop = of_get_property(pdev->dev.of_node, "pddrive_nf_s2", &size);
    if (prop)
    {
		if (size)
			counter = size / sizeof(u32);
	
		for (i=0; i<counter; i+=1) {
                       	pddrive_nf_s2[i]= of_read_number(prop, 1 + i);
			/* KERN_ERR to let be happy */
			printk(KERN_ERR "[%s] get driving s2 : 0x%x\n",__func__,pddrive_nf_s2[i]);
		}

    } else 
    {
        	printk(KERN_INFO "[%s] no driving nf_s2 warning !! \n",__func__);
    }	

	prop = of_get_property(pdev->dev.of_node, "pddrive_nf_s3", &size);
        if (prop) {
                if (size)
                        counter = size / sizeof(u32);

                for (i=0; i<counter; i+=1) {
                        pddrive_nf_s3[i]= of_read_number(prop, 1 + i);
                        /* KERN_ERR to let be happy */
                        printk(KERN_ERR "[%s] get driving s3 : 0x%x\n",__func__,pddrive_nf_s3[i]);
                }
        } else {
                printk(KERN_INFO "[%s] no driving nf_s3 warning !! \n",__func__);
        }
	
	prop = of_get_property(pdev->dev.of_node, "phase_tuning", &size);

	
	if (prop)
   	{   
	   	emmc_port->tx_tuning = of_read_number(prop, 1);
	   	emmc_port->rx_tuning = of_read_number(prop, 2);
		printk(KERN_ERR "[%s] get tx tuning switch : %u\n",__func__, emmc_port->tx_tuning);
		printk(KERN_ERR "[%s] get rx tuning switch : %u\n",__func__, emmc_port->rx_tuning);
   	}
	else 
   	{
		emmc_port->tx_tuning = 1;
		emmc_port->rx_tuning = 1;
		printk(KERN_INFO "[%s] no phase_tuning switch node !! \n",__func__);
   	}

	prop = of_get_property(pdev->dev.of_node, "dqs_tuning", &size);
	if (prop) {
		emmc_port->dqs_tuning = of_read_number(prop, 1);
		printk(KERN_ERR "[%s] get dqs tuning switch : %u\n",__func__, emmc_port->dqs_tuning);
	} else {
		emmc_port->dqs_tuning = 1;      //if we do not get this node, we should tune dqs value by kernel
		printk(KERN_INFO "[%s] no dqs_tuning switch node !! \n",__func__);
	}
	//rtkemmc_init(emmc_port);

	clk_en_emmc = clk_get(NULL, "clk_en_emmc");
	clk_en_emmc_ip = clk_get(NULL, "clk_en_emmc_ip");
	clk_cr = clk_get(NULL, "clk_en_cr");
	clk_prepare_enable(clk_en_emmc);
        clk_prepare_enable(clk_en_emmc_ip);
	clk_prepare_enable(clk_cr);

    mmc->caps = MMC_CAP_4_BIT_DATA
              | MMC_CAP_8_BIT_DATA
              | MMC_CAP_SD_HIGHSPEED
              | MMC_CAP_MMC_HIGHSPEED
              | MMC_CAP_NONREMOVABLE
	      | MMC_CAP_1_8V_DDR
	      | MMC_CAP_UHS_DDR50
	      | MMC_CAP_CMD23
	      | MMC_CAP_ERASE;
	 	

    switch(speed_step)
    {
	case 0: //sdr50
		mmc->caps &= ~(MMC_CAP_UHS_DDR50|MMC_CAP_1_8V_DDR);
		mmc->caps2 &= ~(MMC_CAP2_HS200_1_8V_SDR);
   		break;
	case 1: //ddr50
		mmc->caps2 &= ~(MMC_CAP2_HS200_1_8V_SDR);
		break;
	case 2: //hs200
		mmc->caps2 = MMC_CAP2_HS200_1_8V_SDR;
		break;
	case 3: //hs400
		mmc->caps2 = MMC_CAP2_HS200_1_8V_SDR|MMC_CAP2_HS400_1_8V;
		break;
    }
    if(rtk_emmc_bus_wid == 4 || rtk_emmc_bus_wid == 5){
        mmc->caps &= ~MMC_CAP_8_BIT_DATA;
    }

	mmc->caps2 |= MMC_CAP2_HC_ERASE_SZ;

    mmc->f_min = 300000;   	//300K
    mmc->f_max = 400000000; //400M    

    mmc->max_segs = 256; //128; 
    mmc->max_blk_size   = 512;

    mmc->max_blk_count  = 0x800; //0x400;

    mmc->max_seg_size   = mmc->max_blk_size * mmc->max_blk_count;
    mmc->max_req_size   = mmc->max_blk_size * mmc->max_blk_count;

    spin_lock_init(&emmc_port->lock);
    init_rwsem(&cr_rw_sem);
    tasklet_init(&emmc_port->req_end_tasklet, rtkemmc_req_end_tasklet,
		        (unsigned long)emmc_port);

    //Force enable dbg log
    #ifdef MMC_DBG
    emmc_port->rtflags |= RTKCR_FOPEN_LOG;
    #endif

    if(rtk_emmc_bus_wid == 9 || rtk_emmc_bus_wid == 5){
        emmc_port->rtflags |= RTKCR_FOPEN_LOG;
    }

    MMCPRINTF("\n");

  
    if (!emmc_port->emmc_membase) {
        printk(KERN_INFO "---- Realtek EMMC Controller Driver probe fail - nomem ----\n\n");
        ret = -ENOMEM;
        goto out;
    }

#ifdef ENABLE_EMMC_INT_MODE
    rtk_lockapi_lock2(flags2, _at_("rtkemmc_probe"));
    rtkemmc_hold_int_dec();       /* hold status interrupt */
    rtkemmc_clr_int_sta();
    rtk_lockapi_unlock2(flags2, _at_("rtkemmc_probe"));

    ret = request_irq(irq, rtkemmc_irq, IRQF_SHARED, DRIVER_NAME, emmc_port);   //rtkemmc_interrupt
    if (ret) {
        printk(KERN_ERR "%s: cannot assign irq %d\n", DRIVER_NAME, irq);
        goto out;
    } else{
        emmc_port->irq = irq;
    }
    setup_timer(&emmc_port->timer, rtkemmc_timeout_timer, (unsigned long)emmc_port);
#endif

    emmc_port->ops->set_crt_muxpad(emmc_port);
    if (emmc_port->ops->reset_card)
        emmc_port->ops->reset_card(emmc_port);
    emmc_port->ops->chk_card_insert(emmc_port);

    platform_set_drvdata(pdev, mmc);

    ret = mmc_add_host(mmc);
    if (ret)
        goto out;

    sync(emmc_port);
    memset((struct backupRegs*)&gRegTbl, 0x00, sizeof(struct backupRegs));
    gCurrentBootMode = MODE_SDR;
    g_crinit=0;
    gPreventRetry=0;
    g_bResuming=0;
    g_bTuning=0;

#ifdef RTK_EMMC_HEALTH_REPORT
	mmc->wr_emmc_size_cnt = 0;
        mmc->remainder = 0;
#endif

    printk(KERN_NOTICE "%s: %s driver initialized\n",
               mmc_hostname(mmc), DRIVER_NAME);
    return 0;

out:
    if (emmc_port) {
        if (emmc_port->irq)
            free_irq(emmc_port->irq, emmc_port);

        if (emmc_port->emmc_membase)
            iounmap(emmc_port->emmc_membase);
        if (emmc_port->crt_membase)
            iounmap(emmc_port->crt_membase);
    }
    if (pResources)
        release_resource(pResources);
    if (mmc)
        mmc_free_host(mmc);
    return ret;
}

static int __exit rtkemmc_remove(struct platform_device *pdev)
{
    struct mmc_host *mmc = platform_get_drvdata(pdev);
    MMCPRINTF("\n");
    device_remove_file(&pdev->dev, &dev_attr_cr_send_cmd0);
    device_remove_file(&pdev->dev, &dev_attr_cr_fast_RW);
    device_remove_file(&pdev->dev, &dev_attr_em_open_log);
    device_remove_file(&pdev->dev, &dev_attr_emmc_info);

#ifdef CONFIG_MMC_RTKEMMC_HK_ATTR
    device_remove_file(&pdev->dev, &dev_attr_emmc_hacking);
#endif

    if (mmc) {
        struct rtkemmc_host *emmc_port = mmc_priv(mmc);

        flush_scheduled_work();

        rtkemmc_free_dma_buf(emmc_port);

        mmc_remove_host(mmc);
        if(!mmc){
            printk("eMMC host have removed.\n");
            mmc_host_local = NULL;
        }
        free_irq(emmc_port->irq, emmc_port);

        del_timer_sync(&emmc_port->timer);
        iounmap(emmc_port->emmc_membase);
        iounmap(emmc_port->crt_membase);
		iounmap(emmc_port->sb2_membase);
#if defined(EMMC_LA_DEBUG_GPIO) || defined(EMMC_PARAM_TEST)				
		iounmap(emmc_port->misc_membase);
#endif
        release_resource(emmc_port->res);
        mmc_free_host(mmc);
    }
    platform_set_drvdata(pdev, NULL);
    return 0;
}


#ifdef CONFIG_PM
static int rtkemmc_suspend(struct platform_device *dev, pm_message_t state)
{
    struct platform_device *pdev = to_platform_device(dev);
    int ret = 0;
    struct rtkemmc_host *emmc_port=NULL;
    struct mmc_host *mmc = NULL;
    suspend = 1;
    mmc = mmc_host_local;
    emmc_port = mmc_priv(mmc);
    if(RTK_PM_STATE == PM_SUSPEND_STANDBY){
    	//For idle mode
    	printk(KERN_ERR "[%s] Enter %s Idle mode\n",DRIVER_NAME, __func__);
    	//rtkemmc_backup_registers(emmc_port);
    }else{
    	//For suspend mode
    	printk(KERN_ERR "[%s] Enter %s Suspend mode\n",DRIVER_NAME, __func__);
    }
    
    ret = pm_runtime_force_suspend(dev);
    printk(KERN_ERR "[%s] Exit %s\n",DRIVER_NAME,__func__);
    return ret;
}

static int rtkemmc_resume(struct platform_device *dev)
{
    struct platform_device *pdev = to_platform_device(dev);
    int ret = 0;
    struct mmc_host *mmc = NULL;
    struct rtkemmc_host *emmc_port=NULL;
    struct mmc_host *host = NULL;

    mmc = mmc_host_local;
    emmc_port = mmc_priv(mmc);
    if (!emmc_port)
	BUG();
    host = emmc_port->mmc;
    host->card->host = mmc;
    g_bResuming=1;

    if(RTK_PM_STATE == PM_SUSPEND_STANDBY){
    	//For idle mode
    	printk(KERN_ERR "[%s] Enter %s Idle mode\n",DRIVER_NAME, __func__);
    	//rtkemmc_restore_registers(emmc_port);
    }else{
    	//For suspend mode
    	printk(KERN_ERR "[%s] Enter %s Suspend mode\n",DRIVER_NAME, __func__);
    }
    if (!ret)
            ret = pm_runtime_force_resume(dev);

    rtkemmc_set_pin_mux(emmc_port);
    rtkemmc_chk_card_insert(emmc_port);
    sync(emmc_port);
    
	g_bResuming=0;
	init_completion(emmc_port->int_waiting);
    printk(KERN_ERR "[%s] Exit %s\n",DRIVER_NAME,__func__);

    return ret;
}
static const struct dev_pm_ops rtk_dev_pm_ops = {
        SET_SYSTEM_SLEEP_PM_OPS(rtkemmc_suspend,
                                rtkemmc_resume)
};
#endif
/*****************************************************************************************/
/* driver / device attache area                                                                                                               */
/*****************************************************************************************/
static struct platform_driver rtkemmc_driver = {
    .probe      = rtkemmc_probe,
    .remove     = __exit_p(rtkemmc_remove),
    .driver     = 
    {
            .name   = "rtkemmc",
            .owner  = THIS_MODULE,
	    .of_match_table = of_match_ptr(rtkemmc_ids),
#ifdef CONFIG_PM
	    .pm     = &rtk_dev_pm_ops
#endif
    },
};

static void rtkemmc_display_version (void)
{
    const __u8 *revision;
    const __u8 *date;
    const __u8 *time;
    char *running = (__u8 *)VERSION;

    strsep(&running, " ");
    strsep(&running, " ");
    revision = strsep(&running, " ");
    date = strsep(&running, " ");
    time = strsep(&running, " ");
    printk(BANNER " Rev:%s (%s %s)\n", revision, date, time);

#ifdef CONFIG_MMC_BLOCK_BOUNCE
    printk("%s: CONFIG_MMC_BLOCK_BOUNCE enable\n",DRIVER_NAME);
#else
    printk("%s: CONFIG_MMC_BLOCK_BOUNCE disable\n",DRIVER_NAME);
#endif

#ifdef CONFIG_SMP
    printk("%s: ##### CONFIG_SMP alert!! #####\n",DRIVER_NAME);
#else
    printk("%s: ##### CONFIG_SMP disable!! #####\n",DRIVER_NAME);
#endif
}

static int rtkemmc_set_bus_width(char * buf){
    /*
    get eMMC bus width setting by bootcode parameter, like below
    bootargs=console=ttyS0,115200 earlyprintk emmc_bus=8
    the keyword is "emmc_bus"
    the getted parameter is hex.
    example:
        emmc_bus=8
    */

    rtkemmc_chk_param(&rtk_emmc_bus_wid,1,buf+1);
    printk("%s: setting bus width is %u-bit\n",
                DRIVER_NAME,rtk_emmc_bus_wid);
    return 0;
}
module_platform_driver(rtkemmc_driver);

MODULE_AUTHOR("Louis Yang");
MODULE_DESCRIPTION("Realtek EMMC Host Controller driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:rtkemmc");

__setup("emmc_bus",rtkemmc_set_bus_width);
