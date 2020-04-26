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
#include "rtknand.h"
#include <lib/console.h>
#include <lib/sysparam.h>
#include "reg.h"

//cloud add
#include "flash_nand_list.h"


#define LOCAL_TRACE	0

#define HS200_PHASE_INHERITED

#ifdef DEBUG
#define dbg_printf(...) printf(__VA_ARGS__)
#else
#define dbg_printf(...)
#endif

#define PAGE_TMP_ADDR		0x01200000
#define SPARE_DMA_ADDR		0x016fff00
#define SPARE_DMA_ADDR_SRAM		0x80005f00
#define DMA_ADDR_SRAM_TMP           0x80007200
#define DMA_ADDR_DDR_TMP           0x300000


#define READ_RETRY
#define READ_RETRY_UNICORN
#define BAD_BLOCK_PROTECT    1

#define PHYS(addr) 	((unsigned int)(addr))
/************************************************************************
 *  Public variables
 ************************************************************************/
unsigned int g_PageSize;
unsigned int g_BlockSize;
unsigned int g_PagePerBlock;
unsigned int g_BlockPerFlash;
unsigned int g_Max_Page;
unsigned int g_Total_block;

/************************************************************************
 *  Static variables
 ************************************************************************/
static unsigned int s_PageSize;
//static unsigned int s_RetryPageSize;
static unsigned int s_BlockSize;
//static unsigned int s_PagePerBlock;
//static unsigned int s_BlockPerFlash;
static unsigned int s_FlashID4Bytes;
//static unsigned int s_hwsettingPageAddr;
static unsigned char s_FlashParamHynix[4];
static unsigned char s_FlashParamAddrHynix[4] = {0xa7, 0xad, 0xae, 0xaf};
static unsigned int  s_FlashRetryCountHynix;
static unsigned int  s_FlashParamTmp;


#ifdef READ_RETRY_UNICORN
static int read_retry_toshiba_cnt[7]={0x00, 0x00, 0x04, 0x7c, 0x78, 0x74, 0x08};

/*
static int read_retry_toshiba_cnt_new[8][5]={
        {0x00, 0x00, 0x00, 0x00, 0x00},
        {0x04, 0x04, 0x7c, 0x7e, 0x00},
        {0x00, 0x7c, 0x78, 0x78, 0x00},
        {0x7c, 0x76, 0x74, 0x72, 0x00},
        {0x08, 0x08, 0x00, 0x00, 0x00},
        {0x0b, 0x7e, 0x76, 0x74, 0x00},
        {0x10, 0x76, 0x72, 0x70, 0x00},
        {0x02, 0x7c, 0x7e, 0x70, 0x00}
};
*/
static int read_retry_toshiba_cnt_new_15nm[8][5]={
        {0x00, 0x00, 0x00, 0x00, 0x00},
        {0x04, 0x04, 0x00, 0x00, 0x00},
        {0x08, 0x04, 0x04, 0x04, 0x00},
        {0x7c, 0x7c, 0x7c, 0x78, 0x00},
        {0x78, 0x78, 0x78, 0x74, 0x00},
        {0x78, 0x76, 0x76, 0x74, 0x00},
        {0x78, 0x76, 0x76, 0x72, 0x00},
        {0x78, 0x76, 0x76, 0x72, 0x00},
       // {0x78, 0x76, 0x76, 0x72, 0x00},
       // {0x78, 0x76, 0x76, 0x72, 0x00}
};
#endif //READ_RETRY_UNICORN

unsigned char temp_buf[4096] ,write_buf[4096] ;
unsigned int temp_spare[4];
nandecctype_t sys_nand_type;

#define PTR_U32(_p)   ((unsigned int)(unsigned long)(_p))

// FOR BBT USE
/* Global Variables */
unsigned int RBA=0;
#define	BB_INIT	0xFFFE
#define	RB_INIT	0xFFFD
#define BB_DIE_INIT	0xEEEE
#define RB_DIE_INIT	BB_DIE_INIT

#define BB1_BLOCK   1
#define BB2_BLOCK    2
#define	BBT_TAG	0xBB
#define	BBT_SCAN_RETRY_CNT 	2



int RBA_PERCENT = 5;
int BAD_BLOK_NUM = 0;

BB_t *bbt;
unsigned char numchips =0;

//
int nf_read(unsigned int start_page, unsigned char *buf, unsigned int page_cnt, unsigned int *spare);
ssize_t nf_write(unsigned int start_page, unsigned char *buf, unsigned int size, unsigned int spare, unsigned char doWriteSkip);
int nf_erase_phys_block(unsigned int  page_addr);
static void nf_set_random_col_addr(unsigned int data_col, unsigned int spare_col);
//static unsigned int nf_get_spare(void);
unsigned int nf_get_spare(void);
unsigned int nf_read_page_c_dma_2K(unsigned int page_no, unsigned int dest_addr);
static void nf_set_read_retrial_hynix(void);
void nf_restore_read_retrial(void);
static void nf_set_feature_micron(int P1);
void nf_set_pre_condition_toshiba(void);
void nf_set_feature_toshiba_new_15nm(int P1);
void nf_set_ext_feature_toshiba(void);
void nf_set_terminate_toshiba(void);
void nf_set_feature_toshiba(int P1);
static unsigned int nf_read_page_with_retrial_c(unsigned int page_no, unsigned int dest_addr);
//FOR BIO

static bdev_t nand_dev;
static ssize_t rtknand_bdev_read_block(struct bdev *bdev, void *buf, bnum_t block, uint count);
static ssize_t rtknand_bdev_write_block(struct bdev *bdev, const void *_buf, bnum_t block, uint count);
static ssize_t rtknand_bdev_read(struct bdev *bdev, void *buf, off_t offset, size_t len);
static ssize_t rtknand_bdev_write(struct bdev *bdev, const void *buf, off_t offset, size_t len);
static int rtknand_bdev_ioctl(struct bdev *, int request, void *argp);


void set_nand_pin_mux(void)
{
	//1295
	//set default i/f to nf
	//REG32(SYS_muxpad0) = 0x55555555;
	   writel(0x55555555,SYS_muxpad0);
}
static char nf_is_random_mode(void)
{
//	return REG32(NF_RND_RW_EN) & 0x1;
return readl(NF_RND_RW_EN) & 0x1;
}

//TODO: how to detect 6/12 bits ??
static unsigned int nf_is_6bit_ecc(void)
{
    return ((readl(NF_ECC_SEL) & 0xF) == 0);
}

static unsigned int nf_is_12bit_ecc(void)
{
    return ((readl(NF_ECC_SEL) & 0xF) == 1);
}

static unsigned int nf_is_16bit_ecc(void)
{
	return (((readl(NF_ECC_SEL) & 0xF)>>1)==0x07);
}


static unsigned int nf_is_24bit_ecc(void)
{
	return (((readl(NF_ECC_SEL) & 0xF)>>1)==0x05);
}

static unsigned int nf_is_40bit_ecc(void)
{
	return (((readl(NF_ECC_SEL) & 0xF)>>1)==0x01);
}

static unsigned int nf_is_43bit_ecc(void)
{
	return (((readl(NF_ECC_SEL) & 0xF)>>1)==0x02);
}

static unsigned int nf_is_65bit_ecc(void)
{
	return (((readl(NF_ECC_SEL) & 0xF)>>1)==0x03);
}

static unsigned int nf_is_72bit_ecc(void)
{
	return (((readl(NF_ECC_SEL) & 0xF)>>1)==0x04);
}

void polling_done(const unsigned int addr, const unsigned int mask, const unsigned int value)
{
	while ((readl(addr) & mask) != value)
			;
}


/*
 * issue RESET command to nand flash
 */
int polling_timeout(const unsigned int addr, const unsigned int mask,
	const unsigned int value, unsigned int timeout)
{
	if (timeout == 0) {	// no timeout, use polling_done instead
		polling_done(addr, mask, value);
		return 0;
	}

	while (((readl(addr) & mask) != value) && timeout) {
		timeout--;
	}

	return (timeout == 0 ? ERR_TIMEOUT : 0);
}



static int nf_reset(void)
{
	//REG32(NF_CHIP_EN) = NAND_CHIP_SELECT;
	writel(NAND_CHIP_SELECT,NF_CHIP_EN);

	#ifdef ROM_DEBUG
	prints("NAND_CHIP_SELECT = ");
	print_hex(NAND_CHIP_SELECT);
	prints("\n");
	#endif
/*
	REG32(NF_T3) = 0x2;
	REG32(NF_T2) = 0x5;
	REG32(NF_T1) = 0x2;
*/
	writel(0x2,NF_T3);
	writel(0x5,NF_T2);
	writel(0x2,NF_T1);


	//REG32(NF_MULTI_CH_MODE) = 0x0;
		writel(0x0,NF_MULTI_CH_MODE);
  //	REG32(NF_PP_RDY) = 0x0;
		writel(0x0,NF_PP_RDY);
	// set 'reset' command
	//REG32(NF_CMD1) = 0xff;
		writel(0xff,NF_CMD1);
	// issue command
	//REG32(NF_ND_CTL) = 0x80;
		writel(0x80,NF_ND_CTL);

	// polling go bit to until go bit de-assert (command completed)
	if (polling_timeout(NF_ND_CTL, 0x80, 0, 0x10000)) {

#ifdef ROM_INFO
		prints("nf_reset timeout\n");
#endif

		return ERR_NF_RESET_FAIL;	// time-out
	}

	// check ready/busy pin of NAND flash
	if (polling_timeout(NF_ND_CTL, 0x40, 0x40, 0x10000)) {

#ifdef ROM_INFO
		prints("nf_reset timeout\n");
#endif

		return ERR_NF_RESET_FAIL;	// time-out
	}

	return 0;
}


static void nf_init(void)
{
	//REG32(NF_T1) = 0x2;
	//REG32(NF_T2) = 0x2;
	//REG32(NF_T3) = 0x2;
	//REG32(NF_DELAY_CNT) = 0x9;
//	REG32(NF_SPR_DDR_CTL) &= ~0x40000000;	// disable spare to DDR
 writel(readl(NF_SPR_DDR_CTL)&(~0x40000000),NF_SPR_DDR_CTL);
	/* set random access mode registers if enabled */
	if (nf_is_random_mode()) {

#ifdef ROM_DEBUG
		prints("random access mode\n");
#endif

		_REG32(NF_RND_CMD1) = 0x5;
		_REG32(NF_RND_CMD2) = 0xe0;
		_REG32(NF_RND_DATASTR_COL_H) = 0;
		_REG32(NF_RND_SPR_STR_COL_H) = 0x8;
		_REG32(NF_RND_SPR_STR_COL_L) = 0;


	}
}


static int nf_read_id(unsigned int *id)
{
	unsigned int data ,data2;
	unsigned char loacl_id[6] ={0};
#ifdef ENV_SIMULATION
	return 0;
#endif

	//Set data transfer count, data transfer count must be 0x200 at auto mode
	//Set SRAM path and access mode
	/*
	REG32(NF_DATA_TL0) = 5;
	REG32(NF_DATA_TL1) = 0x80;	// transfer mode
	*/
	//writel(5,NF_DATA_TL0);
	writel(6,NF_DATA_TL0);  // ID length 6
	writel(0x80,NF_DATA_TL1);

	//Set PP
	/*
	REG32(NF_PP_RDY) = 0;		// through PP
	REG32(NF_PP_CTL0) = 0x01;
	REG32(NF_PP_CTL1) = 0;
	*/
	writel(0,NF_PP_RDY);
	writel(0x01,NF_PP_CTL0);
	writel(0,NF_PP_CTL1);
	//Set command
	/*
	REG32(NF_CMD1) = 0x90;
	REG32(NF_ND_CTL) = 0x80;
	*/
	writel(0x90,NF_CMD1);
	writel(0x80,NF_ND_CTL);

	if (polling_timeout(NF_ND_CTL, 0x80, 0, 0x10000)) {

#ifdef ROM_DEBUG
		prints("nf_read_id timeout\n");
#endif

		return ERR_NF_READ_ID_FAIL;
	}

	//Set address
	/*
	REG32(NF_PAGE_ADR0) = 0;
	REG32(NF_PAGE_ADR2) = 0x7<<5;
	REG32(NF_ND_CTL) = 0x81;
	*/
	writel(0,NF_PAGE_ADR0);
	writel(0,NF_PAGE_ADR1);	//clear
	writel(0x7<<5,NF_PAGE_ADR2);
	writel(0x81,NF_ND_CTL);
	if (polling_timeout(NF_ND_CTL, 0x80, 0, 0x10000)) {

#ifdef ROM_DEBUG
		prints("nf_read_id timeout\n");
#endif

		return ERR_NF_READ_ID_FAIL;
	}

	//Enable MultiByteRead XFER mode

	//REG32(NF_ND_CTL) = 0x84; // start tranfer
	writel(0x84,NF_ND_CTL);
	if (polling_timeout(NF_ND_CTL, 0x80, 0, 0x10000)) {

#ifdef ROM_DEBUG
		prints("nf_read_id timeout\n");
#endif

		return ERR_NF_READ_ID_FAIL;
	}

	//Reset PP
//	REG32(NF_PP_CTL0) = 0x2;
	writel(0x2,NF_PP_CTL0);

	//Move data to DRAM from SRAM
	/*
	REG32(NF_SRAM_CTL) = 0x30; //set 0x30 to enable bit[5] ()map_sel, due to out nand list table are implemented for little endian
	data = REG32(NF_PAGE_ADR0);
	REG32(NF_SRAM_CTL) = 0x00;	//# no omitted
  */
	writel(0x30,NF_SRAM_CTL);
	data = readl(NF_PAGE_ADR0);
	data2 = readl(NF_PAGE_ADR1);

	writel(0x00,NF_SRAM_CTL);

	*id = data;

	printf("data2 = %x \n",data2);

	//#ifdef ROM_DEBUG
	printf("nf_read_id0~3 = %x \n",data);

	loacl_id[4] = data2 & 0xff;
	loacl_id[5] = (data2 >> 8) & 0xff;
	printf("nf_read_id4 = %x  , id 5 = %x \n ",loacl_id[4],loacl_id[5]);


	return 0;
}
static int nf_scan_id(void)
{
	unsigned int i;
	char buf[128];

	printf("s_FlashID4Bytes =%x ",s_FlashID4Bytes);
	for (i = 0; n_device[i].id; i++){



			if ( n_device[i].id==s_FlashID4Bytes )
			{

				_REG32(NF_T1) = n_device[i].t1;
				_REG32(NF_T2) = n_device[i].t2;
				_REG32(NF_T3) = n_device[i].t3;

				if ( s_FlashID4Bytes != HY27UT084G2M ){
					_REG32( NF_MULTI_CH_MODE) = 0x20;
				}


						//mtd->PartNum = nand_device[i].name;
						#if 0
						device_size = n_device[i].size;
						chip_size = n_device[i].chipsize;
						page_size = n_device[i].PageSize;
						block_size = n_device[i].BlockSize;
						oob_size = n_device[i].OobSize;
						num_chips = n_device[i].num_chips;
						isLastPage = n_device[i].isLastPage;
                                          #else

						s_BlockSize = n_device[i].BlockSize;
						s_PageSize = n_device[i].PageSize;

						g_BlockSize = n_device[i].BlockSize;
						g_PageSize = n_device[i].PageSize;
						g_PagePerBlock = s_BlockSize/s_PageSize ;///g_BlockSize / g_PageSize;
						g_Max_Page= n_device[i].size/g_PageSize;
						g_Total_block = n_device[i].size/g_BlockSize;
						#endif

	/*
						REG_WRITE_U32( REG_TIME_PARA1,n_device[i].t1);
						REG_WRITE_U32( REG_TIME_PARA2,n_device[i].t2);
						REG_WRITE_U32( REG_TIME_PARA3,n_device[i].t3);
*/
						printf("index(%d) , id=0x%x, device_size=0x%llx, page_size=0x%x, block_size=0x%x, \n",	i,n_device[i].id,n_device[i].size,n_device[i].PageSize,n_device[i].BlockSize);
						/* prepare nand info for linux kernel */
						sprintf(buf, "nandinfo=id:%x,ds:%llu,ps:%d,bs:%d,t1:%d,t2:%d,t3:%d,eb:%d", n_device[i].id, n_device[i].size, n_device[i].PageSize, n_device[i].BlockSize, n_device[i].t1, n_device[i].t2, n_device[i].t3, n_device[i].ecc_bit);
						sysparam_remove("nand_indo");
						sysparam_add("nand_info", buf, strlen(buf) + 1);

						return 0;

				}

			}
	return -1;  //scan fail

}
static unsigned int alogin_to_block_boundary(unsigned int page_addr)
{
	unsigned int block;

	block = page_addr / g_PagePerBlock;
	if (page_addr % g_PagePerBlock)
		block++;

	return block * g_PagePerBlock;
}


static unsigned int nf_read_page_with_retrial_c_2K(unsigned int page_no, unsigned int dest_addr)
{
	unsigned int  res=1;
	unsigned int  retry_count;
	unsigned char flash_manufacturer;
    unsigned char flash_model;

   printf("nf_read_page_with_retrial_c_2K start 0x %x \n",page_no);
	// normal read operation
	res = nf_read_page_c_dma_2K(page_no, dest_addr);

	if (res == 0)
		return res;	// read success

#ifdef READ_RETRY_UNICORN
#ifdef READ_RETRY

	// read failed, check flash ECC type
	if (nf_is_6bit_ecc() || nf_is_12bit_ecc())
        return res; // 6-bit or 12-bit ECC flash (no need to read retrial)

#ifdef ROM_INFO
	prints("read page ");
	print_hex(page_no);
	prints(" error\n");
#endif

	// check flash manufacturer
	flash_manufacturer = s_FlashID4Bytes & 0xff;
        flash_model = (s_FlashID4Bytes & 0xff0000)>>16;
	switch (flash_manufacturer) {
		case 0xad:	// Hynix
			for (retry_count = 0; retry_count < 6; retry_count++) {

#ifdef ROM_INFO
				prints("T");
#endif

				nf_set_read_retrial_hynix();
				res = nf_read_page_c_dma_2K(page_no, dest_addr);
				if (res == 0) {

#ifdef ROM_INFO
					prints("Y\n");
#endif
					nf_restore_read_retrial();

					return res;	// read success
				}
#ifdef ROM_INFO
				prints("X\n");
#endif
			}

			nf_restore_read_retrial();
			break;

		case 0x2c:	// Micron

                       if(flash_model == 0x44){    //Micron read-retry only for 40 bit
        			for (retry_count = 1; retry_count <= 7; retry_count++) {
#ifdef ROM_DEBUG
        				prints("Mr\n");
#endif
        				nf_set_feature_micron(retry_count);

        				res = nf_read_page_c_dma_2K(page_no, dest_addr);
        				if (res == 0) {
                                                nf_set_feature_micron(0);	// set to 0.
        					break;
        				}
        			}
        			nf_set_feature_micron(0);	// set to 0.
			}
			break;

                case 0x98:      //TOSHIBA
#ifdef ROM_DEBUG
                         prints("read retry toshiba\n");
#endif
                        nf_set_pre_condition_toshiba();

                        if(flash_model == 0x94){    //for TC58TEG6DDL 15nm
#ifdef ROM_DEBUG
                                prints("TC58TEG6DDL\n");
#endif
                                for (retry_count = 0; retry_count <= 10; retry_count++) {
                                            nf_set_feature_toshiba_new_15nm(retry_count);

                                        nf_set_ext_feature_toshiba();
        				res = nf_read_page_c_dma_2K(page_no, dest_addr);
        				if (res == 0) {
                                                nf_set_feature_toshiba_new_15nm(0);
                                                nf_set_terminate_toshiba();
        					break;
        				}
        			}
                                nf_set_feature_toshiba_new_15nm(0);
                                nf_set_terminate_toshiba();

                        }else{
#ifdef ROM_DEBUG
                                prints("toshiba normal\n");
#endif
                                for (retry_count = 1; retry_count <= 6; retry_count++) {
                                            nf_set_feature_toshiba(read_retry_toshiba_cnt[retry_count]);

                                        nf_set_ext_feature_toshiba();
        				res = nf_read_page_c_dma_2K(page_no, dest_addr);
        				if (res == 0) {
                                                nf_set_feature_toshiba(0);
                                                nf_set_terminate_toshiba();
        					break;
        				}
        			}

                                nf_set_feature_toshiba(0);
                                nf_set_terminate_toshiba();
                        }
                        break;

		default:	// unknown (or not supported) manufacturer

#ifdef ROM_INFO
			prints("unknown");
#endif

			break;
	}

#ifdef ROM_INFO
	prints("\n");
#endif
#endif	// #ifdef READ_RETRY
#endif //ifdef READ_RETRY_UNICORN

 printf("nf_read_page_with_retrial_c_2K finish \n");
	return res;
}


static int nf_scan_block(unsigned int page_addr, const unsigned int magic, unsigned int limit, unsigned int *target_addr)
{
	unsigned int i;		// current page address
	unsigned int spare;
	unsigned char local_buf[g_PageSize]  ;

       arch_clean_invalidate_cache_range(PTR_U32(&spare), 4);
       printf("nf_scan_block  0x%x  spare =0x%x\n",page_addr,spare );
	// reset col address in ramdom mode
	if (nf_is_random_mode()) {
		nf_set_random_col_addr(0, g_PageSize);
	}

	// disable spare to ddr
	_REG32(NF_SPR_DDR_CTL) &= ~0x40000000;	// disable spare to DDR

	// align to block boundary
	for (i = alogin_to_block_boundary(page_addr); limit > 0; i += g_PagePerBlock, limit--) {
		// read 1st page of block
#ifdef ROM_DEBUG
			prints("page_no:");
			print_hex(i);
			prints("\n");
#endif
              printf("scan page =0x%x times = 0x%x ",i ,limit );
		//if (nf_read_to_PP_first_with_retrial_c(0, i, 512)) {
		//if (nf_read_page_c_dma(i, DMA_ADDR_SRAM_TMP)) {//temp dma securm ram addr
		//if (nf_read_page_c_dma_2K(i, DMA_ADDR_SRAM_TMP)) {//temp dma securm ram addr

	//	if(nf_read_page_with_retrial_c_2K(i, DMA_ADDR_SRAM_TMP)){
	if(nf_read_page_with_retrial_c(i,  (unsigned long)local_buf)){
		//if (nf_read_page_c(i, 0x3000000)) {
                    printf("read error \n");
#ifdef ROM_DEBUG
			prints("read_to_PP(0, ");
			print_hex(i);
			prints(") ECC error\n");
#endif
			continue;	// ECC error, go to next block
		}

		// get spare from sram
		//spare = nf_get_spare() & 0xFF;
		spare = nf_get_spare()&0xFFFF;

//#ifdef ROM_DEBUG
	printf(" read_to_PP(0,%x) spare =0x%x  \n",magic,spare);

//#endif



		if (spare == magic) {

//#ifdef ROM_DEBUG
			printf(" nf_scan_block  magic =0x%x  starts from page  0x%x \n",magic,i);

//#endif
			*target_addr = i;
                      printf("nf_scan_block sucess page = 0x%x  \n",i);
			return 0;
		}
	}
 	printf("nf_scan_block FAIL  \n");
	return -1;
}


 static int rtk_block_isbad(  unsigned int  check_page)
{

#if 1
	unsigned int block_status_p1;

#if 0
	if ( isLastPage ){
		page = block*ppb + (ppb-1);
		if ( this->read_oob (mtd, chipnr, page, oob_size, this->g_oobbuf) ){
			printk ("%s: read_oob page=%d failed\n", __FUNCTION__, page);
			return 1;
		}
		block_status_p1 = this->g_oobbuf[0];
#if Nand_Block_Isbad_Slow_Version
		page = block*ppb + (ppb-2);
		if ( this->read_oob (mtd, chipnr, page, oob_size, this->g_oobbuf) ){
			printk ("%s: read_oob page=%d failed\n", __FUNCTION__, page);
			return 1;
		}
		block_status_p2 = this->g_oobbuf[0];
		//debug_nand("[1]block_status_p1=0x%x, block_status_p2=0x%x\n", block_status_p1, block_status_p2);
#endif
//	}else{
#endif
/*
		if ( this->read_oob (mtd, chipnr, page, oob_size, this->g_oobbuf) ){
			printk ("%s: read_oob page=%d failed\n", __FUNCTION__, page);
			return 1;
		}
		block_status_p1 = this->g_oobbuf[0];
*/
		//check_page = check_page/g_PageSize;
		nf_read(check_page, temp_buf, 1, temp_spare);

//	}
	block_status_p1 =  temp_spare[0];
	//printf("rtk_block_isbad  spare data = %x , %x ,%x, %x \n",temp_spare[0],temp_spare[1],temp_spare[2],temp_spare[3]);
	//if (page == 0xf80)    block_status_p1 = 0x55;

	if ( block_status_p1 != 0xffffffff){
		printf ("WARNING: : error page =0x%x , block_status_p1=0x%x\n",check_page, block_status_p1);
		return -1;
	}
#endif
	return 0;
}

#define BBTScan_Start_block  128

static int scan_last_die_BB(void)
{
	unsigned int addr;
	unsigned int start_page = 0;

	unsigned int table_index=0;
	unsigned int remap_count = 0;
       unsigned int i, j;


       RBA = g_Total_block * RBA_PERCENT/100;
	bbt = malloc(RBA *sizeof(BB_t));
	//unsigned char  block_status[g_Total_block];
	unsigned char  block_status[g_Total_block];

       printf("scan_last_die_BB start  RBA =0x%x  block_status =0x%lx g_Total_block=0x%x\n",RBA,sizeof(block_status),g_Total_block);
//	arch_clean_invalidate_cache_range(PTR_U32(block_status), g_Total_block);
//	arch_clean_invalidate_cache_range(PTR_U32(bbt), sizeof(BB_t)*RBA);

       memset (block_status, 0, g_Total_block );
	start_page = g_PagePerBlock *BBTScan_Start_block ;//for fsbl, tee...
      #if 0
//erase befor scan
for( addr=start_page; addr<g_Max_Page; addr+=g_PagePerBlock ){
					nf_erase_phys_block(addr);
			}
	#endif
	//
	for( addr=start_page; addr<g_Max_Page; addr+=g_PagePerBlock ){
		if ( rtk_block_isbad( addr) ){
			//int bb = addr >> this->phys_erase_shift;
			int bb = addr/g_PagePerBlock;
			block_status[bb] = 0xff;
		BAD_BLOK_NUM++; // check bb number
		}
	}

       int remap_block[g_Total_block];

	for ( i=0; i<RBA; i++){
		if ( block_status[(g_Total_block-1)-i] == 0x00){
			remap_block[remap_count] = (g_Total_block-1)-i;
			remap_count++;
		}
	}
       printf("remap_cont =0x%x ", remap_count);
	if (remap_count<RBA+1){
		for (j=remap_count+1; j<RBA+1; j++)
			remap_block[j-1] = RB_INIT;
	}
       printf("g_Total_block-RBA =0x%x \n ",g_Total_block-RBA);
	for ( i=0; i<(g_Total_block-RBA); i++){
		if (table_index >= RBA) {
			printf("BB number exceeds RBA. \n");
			break;
		}
		if (block_status[i] == 0xff){  //bad block
			bbt[table_index].bad_block = i;
			bbt[table_index].BB_die = numchips-1;
			bbt[table_index].remap_block = remap_block[table_index];
			bbt[table_index].RB_die = numchips-1;
			table_index++;
		}
	}

	       printf("table_index =0x%x \n ",table_index);

	for( i=table_index; table_index<RBA; table_index++){
		bbt[table_index].bad_block = BB_INIT;
		bbt[table_index].BB_die = BB_DIE_INIT;
		bbt[table_index].remap_block = remap_block[table_index];
		bbt[table_index].RB_die = numchips-1;
	}

//	RTK_FLUSH_CACHE((unsigned long) this->bbt, sizeof(BB_t)*RBA);

// BB TABLE
/*
       for (i=0;i<RBA;i++)
	{
       printf("scan_last_die_BB finish   bbt[0x%x].bad_block =%x ,  remap_block =%x\n",i,bbt[i].bad_block ,bbt[i].remap_block);

       }
       */
	return 0;

}
static int rtk_create_bbt(unsigned int page)
{

	int rc = 0;
	unsigned int *temp_BBT = 0;
	unsigned char  mem_page_num, page_counter=0;
	//unsigned int i;


	if(page==(BB1_BLOCK*g_PagePerBlock))
		printf("[rtk_create_bbt] nand driver creates B1 !!page =0x %x \n",page);
	else if(page==(BB2_BLOCK*g_PagePerBlock))
		printf("[rtk_create_bbt] nand driver creates B2 !!page =0x %x \n",page);
	else {
		printf("[rtk_create_bbt] abort creating BB on page %x !!\n", page);
		return  -1;
	}

  	mem_page_num = (sizeof(BB_t)*RBA + g_PageSize-1 )/g_PageSize;
	printf("mem_page_num  =%x \n",mem_page_num);
	//temp_BBT = kmalloc( mem_page_num*g_PageSize, GFP_KERNEL );
	/*
	if ( !temp_BBT ){
		printk("%s: Error, no enough memory for temp_BBT\n",__FUNCTION__);
		return -ENOMEM;
	}
	*/
	temp_BBT = malloc(mem_page_num*g_PageSize);
	memset( temp_BBT, 0xff, mem_page_num*g_PageSize);

	if (nf_erase_phys_block(page) ){
		printf("[rtk_create_bbt ]erase block %d failure !!\n", page);
		rc =  -1;
		goto EXIT;
	}
	temp_spare[0]= BBT_TAG;

	memcpy( (void*)temp_BBT, bbt, sizeof(BB_t)*RBA );

	arch_clean_invalidate_cache_range(PTR_U32(temp_BBT), g_PageSize);
	arch_clean_invalidate_cache_range(PTR_U32(temp_spare), 4);
/*
	for (i=0;i<RBA;i++)
	{
       printf("scan_last_die_BB finish   bbt[0x%x].bad_block =%x ,  remap_block =%x  temp_BBT[0x%x] =%x,bbt[0x%x].RB_die  =0x%x  \n",i,bbt[i].bad_block ,bbt[i].remap_block,i,temp_BBT[i],i,bbt[i].RB_die);

       }
    */
	while( mem_page_num>0 ){
		if (nf_write(page+page_counter,(unsigned char *)(temp_BBT+page_counter*g_PageSize),1,temp_spare[0],0)){
				printf("[rtk_create_bbt] write BBT B%d page %d failure!!\n",page ==0?0:1, page+page_counter);
				rc =  -1;
				goto EXIT;
		}
		page_counter++;
		mem_page_num--;
	}

EXIT:


       printf("[rtk_create_bbt] create sucess page =0x%x \n",page );
       return rc;
}
void read_bbt_from_nand(unsigned int bb_block)
{

#if 1
        unsigned int temp_len ;
	RBA = g_Total_block * RBA_PERCENT/100;
       temp_len =  (sizeof(BB_t)*RBA + g_PageSize-1 )/g_PageSize;
	bbt = malloc(RBA *sizeof(BB_t));
	nf_read(bb_block*g_PagePerBlock,(unsigned char *)bbt ,temp_len , temp_spare );
	#if 0
		printf(" bbt addr =  0x%p \n",bbt);
    printf(" rtknand_bdev_read bad BB_die[0] = 0x%x   \n", bbt[0].BB_die );
		printf(" rtknand_bdev_read bad bad_block[0] = 0x%x   \n", bbt[0].bad_block );
		printf(" rtknand_bdev_read bad bad_block[0] = 0x%x   \n", bbt[0].RB_die );
		printf(" rtknand_bdev_read bad bad_block[0] = 0x%x   \n", bbt[0].remap_block );
       #endif
#endif

}
void rtk_nand_scan_bbt(void)
{
      unsigned int  TAR_PAGE =0;
      int res ;


	printf("rtk_nand_scan_bbt \n");

	res = nf_scan_block(BB1_BLOCK*g_PagePerBlock,BBT_TAG,BBT_SCAN_RETRY_CNT,&TAR_PAGE);

	if (res ==0)
	{
	      printf("FIND BBT1 SUCESS  page = 0x%x ",  TAR_PAGE);
		  read_bbt_from_nand(BB1_BLOCK);
	}
	else
	{
		res = nf_scan_block(BB2_BLOCK*g_PagePerBlock,BBT_TAG,BBT_SCAN_RETRY_CNT,&TAR_PAGE);

	        	if (res ==0)
			{
			        read_bbt_from_nand(BB2_BLOCK);
	      			printf("FIND BBT2 SUCESS  page = 0x%x ",  TAR_PAGE);
			}
			else
			{
                            scan_last_die_BB();
				printf("rtk_nand_scan_bbt  fail , NO BB1 & BB2 \n");
				rtk_create_bbt(BB1_BLOCK*g_PagePerBlock);
				rtk_create_bbt(BB2_BLOCK*g_PagePerBlock);

			}
	}

#if 0
	int rc = 0;
	__u8 isbbt_b1, isbbt_b2;
	u8 *temp_BBT=0;
	u8 *temp_cloud=0;
	//u8 *TEMP_data=0 ,u8 *TEMP_spare=0 ;
	u8 mem_page_num, page_counter=0;

	int i = 0;

	printf("rtk_nand_scan_bbt \n");


	rc = this->read_ecc_page(mtd, 0, ppb, this->g_databuf, this->g_oobbuf, CP_NF_NONE,0,0);
	//printk("read_ecc_page rc = %x , oob = 0x%x\n", rc, this->g_oobbuf[0]);

	isbbt_b1 = this->g_oobbuf[0];
	if ( !rc ){
		if ( isbbt_b1 == BBT_TAG ){
			printk("[%s] have created bbt B1, just loads it !!\n", __FUNCTION__);
			memcpy( temp_BBT, this->g_databuf, page_size );
			page_counter++;
			mem_page_num--;

			while( mem_page_num>0 ){
				if ( this->read_ecc_page(mtd, 0, ppb+page_counter, this->g_databuf, this->g_oobbuf, CP_NF_NONE,0,0) ){
					printk("[%s] load bbt B1 error!!\n", __FUNCTION__);
					kfree(temp_BBT);
					return -1;
				}
				memcpy( temp_BBT+page_counter*page_size, this->g_databuf, page_size );
				page_counter++;
				mem_page_num--;
			}
			memcpy( this->bbt, temp_BBT, sizeof(BB_t)*RBA );
		}
		else{
			printk("[%s] read BBT B1 tags fails, try to load BBT B2\n", __FUNCTION__);
			rc = this->read_ecc_page(mtd, 0, ppb<<1, this->g_databuf, this->g_oobbuf, CP_NF_NONE,0,0);
			printk("read_ecc_page rc = %x , oob = 0x%x\n", rc, this->g_oobbuf[0]);
			isbbt_b2 = this->g_oobbuf[0];
			if ( !rc ){
				if ( isbbt_b2 == BBT_TAG ){
					printk("[%s] have created bbt B2, just loads it !!\n", __FUNCTION__);
					memcpy( temp_BBT, this->g_databuf, page_size );
					page_counter++;
					mem_page_num--;

					while( mem_page_num>0 ){
						if ( this->read_ecc_page(mtd, 0, 2*ppb+page_counter, this->g_databuf, this->g_oobbuf, CP_NF_NONE,0,0) ){
							printk("[%s] load bbt B2 error!!\n", __FUNCTION__);
							kfree(temp_BBT);
							return -1;
						}
						memcpy( temp_BBT+page_counter*page_size, this->g_databuf, page_size );
						page_counter++;
						mem_page_num--;
					}
					memcpy( this->bbt, temp_BBT, sizeof(BB_t)*RBA );
				}else{
					printk("[%s] read BBT B2 tags fails, nand driver will creat BBT B1 and B2\n", __FUNCTION__);
					rtk_create_bbt(mtd, ppb);
					rtk_create_bbt(mtd, ppb<<1);
  				#if 1
				this->read_ecc_page(mtd, 0, 2*ppb+page_counter, this->g_databuf, this->g_oobbuf, CP_NF_NONE,0,0);
	       		for (cntt=0;cntt<10;cntt++)
	       		{

       				printk("after  rtk_create_bbt    finish  TEMP_data[%d] = 0x%x  spare =%x  \n",cntt,this->g_databuf[cntt], this->g_oobbuf[cntt]);
	       		}

			       #endif

				}
			}else{
				printk("[%s] read BBT B2 with HW ECC fails, nand driver will creat BBT B1\n", __FUNCTION__);
				rtk_create_bbt(mtd, ppb);


			}
		}
	}else{
		printk("[%s] read BBT B1 with HW ECC error, try to load BBT B2\n", __FUNCTION__);
		rc = this->read_ecc_page(mtd, 0, ppb<<1, this->g_databuf, this->g_oobbuf, CP_NF_NONE,0,0);
		isbbt_b2 = this->g_oobbuf[0];
		if ( !rc ){
			if ( isbbt_b2 == BBT_TAG ){
				printk("[%s] have created bbt B1, just loads it !!\n", __FUNCTION__);
				memcpy( temp_BBT, this->g_databuf, page_size );
				page_counter++;
				mem_page_num--;

				while( mem_page_num>0 ){
					if ( this->read_ecc_page(mtd, 0, 2*ppb+page_counter, this->g_databuf, this->g_oobbuf, CP_NF_NONE,0,0) ){
						printk("[%s] load bbt B2 error!!\n", __FUNCTION__);
						kfree(temp_BBT);
						return -1;
					}
					memcpy( temp_BBT+page_counter*page_size, this->g_databuf, page_size );
					page_counter++;
					mem_page_num--;
				}
				memcpy( this->bbt, temp_BBT, sizeof(BB_t)*RBA );
			}else{
				printk("[%s] read BBT B2 tags fails, nand driver will creat BBT B2\n", __FUNCTION__);
				rtk_create_bbt(mtd, ppb<<1);


			}
		}else{
			printk("[%s] read BBT B1 and B2 with HW ECC fails\n", __FUNCTION__);
			kfree(temp_BBT);
			return -1;
		}
	}

	dump_BBT(mtd);

	if (temp_BBT)
		kfree(temp_BBT);
      #if 1

	if ( this->read_ecc_page(mtd, 0, 1*64, this->g_databuf ,  this->g_oobbuf, CP_NF_NONE,0,0) ){
						printk("read fail  error!!\n");

						return -1;
					}


	       for (cntt=0;cntt<10;cntt++)
	       {

       		printk("check rtk_nand_scan_bbt    finish  TEMP_data[%d] = 0x%x  spare =%x  \n",i,this->g_databuf[cntt], this->g_oobbuf[cntt]);
	       }
              this->read_ecc_page(mtd, 0, 2*64, this->g_databuf ,  this->g_oobbuf, CP_NF_NONE,0,0);



	       for (cntt=0;cntt<10;cntt++)
	       {

       		printk("check rtk_nand_scan_bbt    finish  TEMP_data[%d] = 0x%x  spare =%x  \n",i,this->g_databuf[cntt], this->g_oobbuf[cntt]);
	       }


              this->read_ecc_page(mtd, 0, 62*64, this->g_databuf ,  this->g_oobbuf, CP_NF_NONE,0,0);
	       for (cntt=0;cntt<10;cntt++)
	       {

       		printk("check rtk_nand_scan_bbt   finish  TEMP_data[%d] = 0x%x  spare =%x  \n",i,this->g_databuf[cntt], this->g_oobbuf[cntt]);
	       }



		this->read_ecc_page(mtd, 0, 0x700, this->g_databuf  ,  this->g_oobbuf, CP_NF_NONE,0,0);
	       for (cntt=0;cntt<10;cntt++)
	       {

       		printk("check rtk_nand_scan_bbt   finish  TEMP_data[%d] = 0x%x  spare =%x  \n",i,this->g_databuf[cntt],this->g_oobbuf[cntt]);
	       }

      #endif

	return rc;
#endif
}

/************************************************************************
 *
 *                          nf_write
 *  Description :
 *  -------------
 *  write data into NAND flash
 *
 *  Parameters :
 *  ------------
 *  'device',       	IN,    variable of type, n_device_type.
 *  'start_page',  	IN,    start page address to write
 *  'buf', 	        IN,    pointer for buffer of data to be written
 *  'size',       	IN,    number of bytes to write
 *
 *  Return values :
 *  ---------------
 *  '-1': device is NULL or write beyond flash or write failed
 *
 ************************************************************************/
int nf_initial(void)
{


	int res = 0;
	//extern nandecctype_t sys_nand_type;
       unsigned int tmp_reg = 0 ;
//   	unsigned int i , j;
	//unsigned int temp_spare[4];
	//unsigned int  j;


	arch_clean_invalidate_cache_range(MEMBASE, MEMSIZE);

       printf("nf_initial \n");
	   /*
	   memset((void *)temp_buf,0xc,2048);
	      memset((void *)write_buf,0xaa,2048);
       memset((void *)temp_spare,0x55,16);
       */
	// set flash info.
	/*
	s_PageSize = g_PageSize;
	s_BlockSize = g_BlockSize;
	s_PagePerBlock = g_PagePerBlock;
	s_BlockPerFlash = g_BlockPerFlash;
*/
    //REG32(0x98000038) = 0x4;
    tmp_reg = readl(SYS_NF_CKSEL);
    tmp_reg &= ~0xf;
    tmp_reg |= 0x5;
   // REG32(SYS_NF_CKSEL) = tmp_reg;   //set to 10.8Mhz
   writel(tmp_reg,SYS_NF_CKSEL);

	// set pinmux
	set_nand_pin_mux();
	sys_nand_type = NAND_ECC_TYPE_6BIT;
	// set ECC.
	//REG32(NF_ECC_SEL) = sys_nand_type;
	   writel(sys_nand_type,NF_ECC_SEL);

	//set random access mode
	if (nf_is_6bit_ecc() || nf_is_12bit_ecc())
		//REG32(NF_RND_RW_EN) = 1;
		writel(1,NF_RND_RW_EN);
	else
		//REG32(NF_RND_RW_EN) = 0;
		writel(0,NF_RND_RW_EN);

	/* reset & init flash */
	nf_init();

	/*
	if (res = nf_reset())
		return res;
	*/
	res = nf_reset();

	if(res)  return res;

/*
	REG32(NF_T3) = 0x2;
	REG32(NF_T2) = 0x5;
	REG32(NF_T1) = 0x2;
*/
		writel(0x2,NF_T3);
		writel(0x5,NF_T2);
		writel(0x2,NF_T1);

   // REG32(NF_MULTI_CH_MODE) = 0x0;

   		writel(0x0,NF_MULTI_CH_MODE);



	// to avoid identifying fail with empty flash, remove it.
	/* identify flash */
	//if (res = nf_identify())
	//	return res;

	/* read id */
	s_FlashID4Bytes = 0;

	res = nf_read_id(&s_FlashID4Bytes);
	if (res !=0)
	{
	       printf("NAND READ ID FIAL \n");
	 	return res ; // scan ID fail
       }
    //  printf("6 byte ID  =%x , %x \n  ",(unsigned int )(0xffffffff&s_FlashID4Bytes),(unsigned int )((s_FlashID4Bytes>>32)&0xffffffff));
//	if (res = nf_read_id(&s_FlashID4Bytes))
//        return res;
       res =  nf_scan_id();

       if (res !=0)
	{
	       printf("NAND SACN ID FIAL \n");
	 	return res ; // scan ID fail
       }
	rtk_nand_scan_bbt();


#if 0
	nf_read(0x1a*64,(unsigned char *)temp_buf ,1 , temp_spare );
	for (j=0;j<30;j++)
	{
	 printf("block 0x1a  temp_buf[0x%x]=%x \n",j,temp_buf[j]);
	}
	nf_read(1*64,(unsigned char *)temp_buf ,1 , temp_spare );
	for (j=0;j<30;j++)
	{
	 printf("block 0x1  temp_buf[0x%x]=%x \n",j,temp_buf[j]);
	}


	for (i=0;i<4;i++)
	{
	 printf("block 0x1  spare_buf[0x%x]=%x \n",i,temp_spare[i]);
	}


	nf_read(0x1a*64,(unsigned char *)temp_buf ,1 , temp_spare );
	for (j=0;j<30;j++)
	{
	 printf("block 0x1a  temp_buf[0x%x]=%x \n",j,temp_buf[j]);
	}


	for (i=0;i<4;i++)
	{
	 printf("block 0x1a  spare_buf[0x%x]=%x \n",i,temp_spare[i]);
	}


       nf_erase_phys_block(22*64);


	/*
       arch_clean_invalidate_cache_range(PTR_U32(write_buf), g_PageSize);
	arch_clean_invalidate_cache_range(PTR_U32(temp_spare), 4);
*/

	memset((void *)write_buf,0x78,2048);

	arch_clean_invalidate_cache_range(PTR_U32(write_buf), g_PageSize);
	arch_clean_invalidate_cache_range(PTR_U32(temp_spare), 4);
	//memset(write_buf,0xee,50);

	temp_spare[0]=0xcd;



	nf_write(22*64,(unsigned char *)write_buf,1,temp_spare[0],0);


	nf_read(22*64,(unsigned char *)temp_buf ,1 , temp_spare );

	for (i=0;i<20;i++)
	{
	 printf("block 22  data[0x%x]=%x \n",i,temp_buf[i]);
	}
	 printf("block 22  spare =%x \n",temp_spare[0]);



       for (j=0;j<63;j++)
       {
	nf_read(j*64,(unsigned char *)temp_buf ,1 , temp_spare );
	for (i=0;i<30;i++)
	{
	 printf("block 0x%x  temp_buf[0x%x]=%x \n",j,i,temp_buf[i]);
	}


	for (i=0;i<4;i++)
	{
	 printf("block 0x%x  spare_buf[0x%x]=%x \n",j,i,temp_spare[i]);
	}
       }

#endif


	bio_initialize_bdev(&nand_dev, BIO_NAND_NAME, g_BlockSize, g_Total_block,
				0, NULL, BIO_FLAGS_NONE);
	nand_dev.read_block = rtknand_bdev_read_block;
	nand_dev.write_block = rtknand_bdev_write_block;

	nand_dev.read = rtknand_bdev_read;
	nand_dev.write = rtknand_bdev_write;
	nand_dev.ioctl = rtknand_bdev_ioctl;
	bio_register_device(&nand_dev);
/*
	mmc_dev.read_block = rtkemmc_bdev_read_block;
	mmc_dev.write_block = rtkemmc_bdev_write_block;
	bio_register_device(&mmc_dev);
	*/
	return res;

}

int addr_secure_ram(unsigned int addr)
{
	return (addr & 0x80000000);
}

void nf_wait_done(unsigned int *addr, unsigned int mask, unsigned int value){

  while ( ((*addr)& mask) != value ){
  	//if(n++>0x10000){
			//printk("Time out \n");
			//prints("to\n");
			//return;
    // }
			asm("nop");
	}
}


unsigned int nf_read_page_c_dma_2K(unsigned int page_no, unsigned int dest_addr)
{
	unsigned int unit_len;
    int retry_allone = 0;
    int isMLC = 0;
    int j;

			//prints("dest_addr:");
			//print_hex(dest_addr);
			//prints("\n");

            		//prints("page_no:");
			//print_hex(page_no);
			//prints("\n");
			printf("nf_read_page_c_dma_2K start page =0x%x ",page_no );

retry:
	if (nf_is_6bit_ecc() || nf_is_12bit_ecc()) {
		unit_len = 512;
		//enable random access
		_REG32(NF_RND_RW_EN) = 1;
	}
	else {
        isMLC = 1;
        unit_len = 1024;
        if (retry_allone == 1){
            _REG32(REG_RMZ_CTRL) = 0;
		    //REG32(NF_BLANK_CHK) = 0x5;    //not used xnor, just disable randomizer
		    _REG32(NF_BLANK_CHK) = 0x1; // blank check
		}
        else{


                for(j=0;j<6;j++){
                    _REG32(REG_RMZ_SEED_CTRL) = j;
	            //REG32(REG_RMZ_SEED_L) = 0x6e;
	            //REG32(REG_RMZ_SEED_H) = 0xa3;

                    _REG32(REG_RMZ_SEED_L) = (page_no>>(j * 0x10));
	            _REG32(REG_RMZ_SEED_H) = (page_no>>(j * 0x10 + 8));
                }

            _REG32(REG_RMZ_SEED_CTRL) = 0x10;
		    _REG32(REG_RMZ_CTRL) = 1;
            //REG32(NF_RND_RW_EN) = 0;
        }
	}

	_REG32(NF_DATA_TL0) = unit_len & 0xff;
	_REG32(NF_DATA_TL1) = (unit_len >> 8) | 0x80;	// set length1, access_mode = 1

	// set page_len (in unit)
	_REG32(NF_PAGE_LEN) = 0x800 / unit_len;

	// set PP
	_REG32(NF_PP_RDY) = 0x80;	//NAND --> PP --> DRAM
	_REG32(NF_PP_CTL0) = 0;
	_REG32(NF_PP_CTL1) = 0;

	// enable blank check
	_REG32(NF_BLANK_CHK) = 0x1;

	// set command
	_REG32(NF_CMD1) = 0x00;
	_REG32(NF_CMD2) = 0x30;
	_REG32(NF_CMD3) = 0x70;

	_REG32(NF_RND_CMD1) = 0x5;
	_REG32(NF_RND_CMD2) = 0xe0;

	// set address
	_REG32(NF_PAGE_ADR0) =  (page_no & 0xff);
	_REG32(NF_PAGE_ADR1) =  (page_no >> 8 ) & 0xff;
	_REG32(NF_PAGE_ADR2) = ((page_no >> 16) & 0x1f) | (0x1 << 5);	// addr_mode = 001
	_REG32(NF_PAGE_ADR3) = ((page_no >> 21) & 0x7) << 5;
	_REG32(NF_COL_ADR0)  = 0;
	_REG32(NF_COL_ADR1)  = 0;

	//Set random address spare address
	// already set it outside.
	//WRITE_REG_INT32U(NF_RND_SPR_STR_COL_L_reg, (( 0xff ) & ( col_addr>> 0 )));				//Set spare address[7:0]
	//WRITE_REG_INT32U(NF_RND_SPR_STR_COL_H_reg, pagesize*sector_size);				//Set spare address[15:8]

	//Set random address spare address
	//REG32(NF_RND_SPR_STR_COL_L)=0;				//Set spare address[7:0]
	//REG32(NF_RND_SPR_STR_COL_H)=32;				//Set spare address[15:8]

	// set ECC
#ifndef ENV_SIMULATION
	//REG32(NF_MULTI_CH_MODE) = 0x20;
	_REG32(NF_MULTI_CH_MODE) = 0x0;
#endif
	_REG32(NF_ECC_STOP) = 0x80;	// set ecc_n_stop
	if(addr_secure_ram(dest_addr)){
            //prints("2K SRAM spare\n");
	    _REG32(NF_SPR_DDR_CTL) = 0x40000000 | (SPARE_DMA_ADDR_SRAM >> 3);
        }else{
            //prints("2K DDR spare\n");
	    _REG32(NF_SPR_DDR_CTL) = 0x40000000 | (SPARE_DMA_ADDR >> 3);
        }
sync();
	// set DMA
	//REG32(NF_DMA_ADR) = PHYS(dest_addr) >> 3;	// 8 bytes unit
	_REG32(NF_DMA_ADR) = dest_addr >> 3;	// 8 bytes unit
	_REG32(NF_DMA_LEN) = 0x800 / 0x200;	// 512B unit

    if(addr_secure_ram(dest_addr)){
        //prints("2K SRAM\n");
	_REG32(NF_DMA_CONF) = 0x13;
    }else{
        //prints("2K DDR\n");
        _REG32(NF_DMA_CONF) = 0x3;
    }

#ifdef ENABLE_MMU
	sys_dcache_flush_all();
#endif
	sync();
//prints("2K auto trig \n");
	// enable Auto mode
	_REG32(NF_AUTO_TRIG) = 0x82;	// read in 2K page size

    nf_wait_done((void *)(unsigned long)NF_AUTO_TRIG, 0x80, 0x00);					//Wait auto mode done
	//while( REG32(NF_AUTO_TRIG) & 0x80 )
		//;
//prints("2K dma conf\n");
    #ifdef ENABLE_MMU
	sys_dcache_flush_all();
#endif
	sync();
	// wait DMA done
	nf_wait_done((void *)(unsigned long )NF_DMA_CONF, 0x01, 0x00);					//Wait auto mode done
	//while( REG32(NF_DMA_CONF) & 0x1 )
		//;

    // return OK if all data bit is 1 (page is not written yet)
    if ((_REG32(NF_BLANK_CHK) & 0x8)==0){
        #ifdef ROM_DEBUG
                prints("==>all one\n");
        #endif
		return 0;
    }
    else if (_REG32(NF_ECC_STATE) & 0x8) {
		#ifdef ROM_DEBUG
			prints("2K HW ecc error \n");
		#endif
        if ((isMLC) && (retry_allone == 0)){
            retry_allone++;
            goto retry;
        }
		return ERR_NF_READ_ECC_FAIL;	// ECC not correctable
	}
	else
		printf("nf_read_page_c_dma_2K finish page =0x%x ",page_no );
		return 0;
}


unsigned int nf_read_page_c_dma(unsigned int page_no, unsigned int dest_addr)
{
	unsigned int unit_len;
    int retry_allone = 0;
    int isMLC = 0;
    int j;

	ISB;
	sync();
			//prints("dest_addr:");
			//print_hex(dest_addr);
			//prints("\n");

//printf("nf_read_page_c_dma \n ");
retry:
	if (nf_is_6bit_ecc() || nf_is_12bit_ecc()) {
		unit_len = 512;
		//enable random access
		//REG32(NF_RND_RW_EN) = 1;
		writel(1,NF_RND_RW_EN);
		//    printf("SLC  \n ");
	}
	else {
		//    printf("MLC  \n ");
        isMLC = 1;
        unit_len = 1024;
        if (retry_allone == 1){
                    _REG32(REG_RMZ_CTRL) = 0;

		    //REG32(NF_BLANK_CHK) = 0x5; //not used xnor, just disable randomizer
		    _REG32(NF_BLANK_CHK) = 0x1;

		}
        else{


                for(j=0;j<6;j++){
                   // REG32(REG_RMZ_SEED_CTRL) = j;
                    	writel(j,REG_RMZ_SEED_CTRL);
	            //REG32(REG_RMZ_SEED_L) = 0x6e;
	            //REG32(REG_RMZ_SEED_H) = 0xa3;

                    //REG32(REG_RMZ_SEED_L) = (page_no>>(j * 0x10));
                    writel((page_no>>(j * 0x10)),REG_RMZ_SEED_L);
	           // REG32(REG_RMZ_SEED_H) = (page_no>>(j * 0x10 + 8));
	              writel((page_no>>(j * 0x10 + 8)),REG_RMZ_SEED_H);

                }

           _REG32(REG_RMZ_SEED_CTRL) = 0x10;

		   _REG32(REG_RMZ_CTRL) = 1;

        }
	}

	//REG32(NF_DATA_TL0) = unit_len & 0xff;
	writel((unit_len & 0xff),NF_DATA_TL0);
//	REG32(NF_DATA_TL1) = (unit_len >> 8) | 0x80;	// set length1, access_mode = 1
	writel(((unit_len >> 8) | 0x80),NF_DATA_TL1);
	// set page_len (in unit)
	//REG32(NF_PAGE_LEN) = s_PageSize / unit_len;
	writel((s_PageSize / unit_len),NF_PAGE_LEN);


	// set PP

	_REG32(NF_PP_RDY) = 0x80;	//NAND --> PP --> DRAM
	_REG32(NF_PP_CTL0) = 0;
	_REG32(NF_PP_CTL1) = 0;




	// enable blank check
	_REG32(NF_BLANK_CHK) = 0x1;

	// set command

	_REG32(NF_CMD1) = 0x00;
	_REG32(NF_CMD2) = 0x30;
	_REG32(NF_CMD3) = 0x70;





	_REG32(NF_RND_CMD1) = 0x5;
	_REG32(NF_RND_CMD2) = 0xe0;



	// set address

	_REG32(NF_PAGE_ADR0) =  (page_no & 0xff);
	_REG32(NF_PAGE_ADR1) =  (page_no >> 8 ) & 0xff;
	_REG32(NF_PAGE_ADR2) = ((page_no >> 16) & 0x1f) | (0x1 << 5);	// addr_mode = 001
	_REG32(NF_PAGE_ADR3) = ((page_no >> 21) & 0x7) << 5;
	_REG32(NF_COL_ADR0)  = 0;
	_REG32(NF_COL_ADR1)  = 0;




	//Set random address spare address
	// already set it outside.
	//WRITE_REG_INT32U(NF_RND_SPR_STR_COL_L_reg, (( 0xff ) & ( col_addr>> 0 )));				//Set spare address[7:0]
	//WRITE_REG_INT32U(NF_RND_SPR_STR_COL_H_reg, pagesize*sector_size);				//Set spare address[15:8]

	// set ECC
#ifndef ENV_SIMULATION
	//REG32(NF_MULTI_CH_MODE) = 0x20;
	_REG32(NF_MULTI_CH_MODE) = 0x0;

#endif
	_REG32(NF_ECC_STOP) = 0x80;	// set ecc_n_stop


	if(addr_secure_ram(dest_addr)){
            //prints("SRAM spare\n");
	  _REG32(NF_SPR_DDR_CTL) = 0x60000000 | (SPARE_DMA_ADDR_SRAM >> 3);

        }else{
            //prints("DDR spare\n");
	    _REG32(NF_SPR_DDR_CTL) = 0x60000000 | (SPARE_DMA_ADDR >> 3);

        }

	// set DMA
	//REG32(NF_DMA_ADR) = PHYS(dest_addr) >> 3;	// 8 bytes unit
	_REG32(NF_DMA_ADR) = dest_addr >> 3;	// 8 bytes unit
	_REG32(NF_DMA_LEN) = s_PageSize / 0x200;	// 512B unit


    if(addr_secure_ram(dest_addr)){
        //prints("SRAM\n");
	_REG32(NF_DMA_CONF) = 0x13;
    }else{
        //prints("DDR\n");
        _REG32(NF_DMA_CONF) = 0x3;
    }

#ifdef ENABLE_MMU
	sys_dcache_flush_all();
#endif
	sync();

	// enable Auto mode
	_REG32(NF_AUTO_TRIG) = 0x82;	// read in 2K page size
	while( _REG32(NF_AUTO_TRIG) & 0x80 )
		;

	// wait DMA done
	while( _REG32(NF_DMA_CONF) & 0x1 )
		;


    // return OK if all data bit is 1 (page is not written yet)
    if ((_REG32(NF_BLANK_CHK) & 0x8)==0){

           //     printf("==>all one\n");

		return 0;
    }
    else if (_REG32(NF_ECC_STATE) & 0x8) {

			printf("HW ecc error \n");

        if ((isMLC) && (retry_allone == 0)){
            retry_allone++;
            goto retry;
        }
		return ERR_NF_READ_ECC_FAIL;	// ECC not correctable
	}
	else
	{
	//printf("read OK \n");
		return 0;
	}
}

static void nf_get_param_hynix(void)
{
    int i;

	// Set data transfer count[7:0]
	_REG32(NF_DATA_TL0) = 0x1;

	// Set data transfer count[13:8] and access mode
	_REG32(NF_DATA_TL1) = 0x80;

	// Set not using PP
	_REG32(NF_PP_RDY) = 0;

	// Set PP starting address[9:8], PP reset and PP enable
	_REG32(NF_PP_CTL0) = 0x1;

	// Set PP starting assdress[7:0]
	_REG32(NF_PP_CTL1) = 0;

	// Set CMD1
	_REG32(NF_CMD1) = 0x37;

	// Enable transfer, eanble ECC and set transfer mode (transfer command)
	_REG32(NF_ND_CTL) = 0x80;

	// Wait transfer done
	while (_REG32(NF_ND_CTL) & 0x80);

	for (i = 0; i < 4; i++) {
		// Transfer address
		_REG32(NF_PAGE_ADR0) = s_FlashParamAddrHynix[i];

		// Set address mode (only one address cycle)
		_REG32(NF_PAGE_ADR2) = 0x7 << 5;

		// Enable transfer, eanble ECC and set transfer mode (transfer address)
		_REG32(NF_ND_CTL) = 0x80 | 0x1;

		// Wait transfer done
		while (_REG32(NF_ND_CTL) & 0x80);

		// Enable transfer, eanble ECC and set transfer mode (read single bye data)
		_REG32(NF_ND_CTL) = 0x80 | 0x2;

		// Wait transfer done
		while (_REG32(NF_ND_CTL) & 0x80);

		// Get data
		s_FlashParamHynix[i] = _REG32(NF_DATA) & 0xff;
	}
	if (i != 4) // loop no completed
	{
		//faultDetect();
	}


	// Reset PP
	_REG32(NF_PP_CTL0) = 0x2;
}

static void nf_set_param_hynix(const unsigned char new_param[4])
{
    int i;

	// Set data transfer count[7:0]
	_REG32(NF_DATA_TL0) = 0x1;

	// Set data transfer count[13:8] and access mode
	_REG32(NF_DATA_TL1) = 0x80;

	// Set CMD1
	_REG32(NF_CMD1) = 0x36;

	// Enable transfer, eanble ECC and set transfer mode (transfer command)
	_REG32(NF_ND_CTL) = 0x80;

	// Wait transfer done
	while (_REG32(NF_ND_CTL) & 0x80);

	// Set not using PP
	_REG32(NF_PP_RDY) = 0;

	for (i = 0; i < 4; i++) {
		// Transfer address
		_REG32(NF_PAGE_ADR0) = s_FlashParamAddrHynix[i];

		// Set address mode (only one address cycle)
		_REG32(NF_PAGE_ADR2) = 0x7 << 5;

		// Enable transfer, eanble ECC and set transfer mode (transfer address)
		_REG32(NF_ND_CTL) = 0x80 | 0x1;

		// Wait transfer done
		while (_REG32(NF_ND_CTL) & 0x80);

		// Reset PP
		_REG32(NF_PP_RDY)=0;
		_REG32(NF_PP_CTL0) = 0x2;//Alexchang

		// Enable direct access SRAM
		_REG32(NF_SRAM_CTL) = 0x30;

		// Set data
	//	REG8(NF_REG_BASE) = new_param[i];
    _REG32(NF_REG_BASE) = new_param[i];
		// Disable direct access SRAM
		_REG32(NF_SRAM_CTL) = 0;

		// Set PP starting address[9:8], PP reset and PP enable
		_REG32(NF_PP_CTL0) = 0x1;

		//Set PP starting address[9:8]
		_REG32(NF_PP_CTL0) = 0;

		// Enable transfer, eanble ECC and set transfer mode (multiple bytes of data write)
		_REG32(NF_ND_CTL) = 0x80 | 0x3;

		// Wait transfer done
		while (_REG32(NF_ND_CTL) & 0x80);
	}
	if (i != 4) // loop no completed
	{
	//	faultDetect();
	}

	// Set CMD1
	_REG32(NF_CMD1) = 0x16;

	// Enable transfer, eanble ECC and set transfer mode
	_REG32(NF_ND_CTL) = 0x80;

	// Wait transfer done
	while (_REG32(NF_ND_CTL) & 0x80);
}


static void nf_set_read_retrial_hynix(void)
{
    unsigned int  temp;
	unsigned char new_param[4];

	// check parameter data is ready
	if ( (s_FlashParamHynix[0] == 0) && (s_FlashParamHynix[1] == 0)
		&& (s_FlashParamHynix[2] == 0) && (s_FlashParamHynix[3] == 0) ) {

		if (s_FlashParamTmp == 0) {
			// register doest not contain parameter, read from flash
			nf_get_param_hynix();	// only once

				// also save parameter to register
				s_FlashParamTmp = (s_FlashParamHynix[0] << 24) |
					(s_FlashParamHynix[1] << 16) |
					(s_FlashParamHynix[2] <<  8) |
					s_FlashParamHynix[3];
#ifdef ROM_INFO
			prints("param from flash: ");
			hexdump(s_FlashParamHynix, 4);
#endif

		}
		else {
			// get parameter from register
			temp = s_FlashParamTmp;

#ifdef ROM_INFO
			prints("param in reg:");
			print_hex(temp);
			prints("\n");
#endif

			s_FlashParamHynix[0] = (temp >> 24) & 0xff;
			s_FlashParamHynix[1] = (temp >> 16) & 0xff;
			s_FlashParamHynix[2] = (temp >>  8) & 0xff;
			s_FlashParamHynix[3] = temp & 0xff;
		}

		// reset retry count
		s_FlashRetryCountHynix = 0;
	}

	temp = s_FlashRetryCountHynix % 6;

	switch(temp) {
		case 0:	// 1st retry
			new_param[0] = s_FlashParamHynix[0];
			new_param[1] = s_FlashParamHynix[1] + 0x6;
			new_param[2] = s_FlashParamHynix[2] + 0xa;
			new_param[3] = s_FlashParamHynix[3] + 0x6;
			break;

		case 1:	// 2nd retry
			new_param[0] = 0;
			new_param[1] = s_FlashParamHynix[1] - 0x3;
			new_param[2] = s_FlashParamHynix[2] - 0x7;
			new_param[3] = s_FlashParamHynix[3] - 0x8;
			break;

		case 2:	// 3rd retry
			new_param[0] = 0;
			new_param[1] = s_FlashParamHynix[1] - 0x6;
			new_param[2] = s_FlashParamHynix[2] - 0xd;
			new_param[3] = s_FlashParamHynix[3] - 0xf;
			break;

		case 3:	// 4th retry
			new_param[0] = 0;
			new_param[1] = s_FlashParamHynix[1] - 0x9;
			new_param[2] = s_FlashParamHynix[2] - 0x14;
			new_param[3] = s_FlashParamHynix[3] - 0x17;
			break;

		case 4:	// 5th retry
			new_param[0] = 0;
			new_param[1] = 0;
			new_param[2] = s_FlashParamHynix[2] - 0x1a;
			new_param[3] = s_FlashParamHynix[3] - 0x1e;
			break;

		case 5:	// 6th retry
			new_param[0] = 0;
			new_param[1] = 0;
			new_param[2] = s_FlashParamHynix[2] - 0x20;
			new_param[3] = s_FlashParamHynix[3] - 0x25;
			break;

		default:
			return;	// should not happen
	}

	nf_set_param_hynix(new_param);
	s_FlashRetryCountHynix++;	// increment retry count

#ifdef ROM_INFO
	prints("param now: ");
	hexdump(new_param, 4);
#endif
}
static int nf_restore_read_retrial_hynix(void)
{
	// check parameter data is ready
	if ( (s_FlashParamHynix[0] == 0) && (s_FlashParamHynix[1] == 0)
		&& (s_FlashParamHynix[2] == 0) && (s_FlashParamHynix[3] == 0) )
		return 0;	// no need to restore (since do not get parameter yet)

	nf_set_param_hynix(s_FlashParamHynix);
	s_FlashRetryCountHynix = 0;
	return 1;
}


void nf_restore_read_retrial(void)
{
	unsigned char flash_manufacturer;
	//int res;

	if (nf_is_24bit_ecc() == 0)
		return;		// not 24-bit ECC flash, no need to restore

	if (s_FlashID4Bytes == 0)
		return;		// cannot identify flash manufacturer

	// check flash manufacturer
	flash_manufacturer = s_FlashID4Bytes & 0xff;
	//res = 0;
	switch (flash_manufacturer) {
		case 0xad:	// Hynix
			 nf_restore_read_retrial_hynix();
			break;

		default:

#ifdef ROM_INFO
			prints("unknown");
#endif

			return;
	}

#ifdef ROM_INFO
	if (res)
		printf("\nrestored\n");
#endif
}

static void nf_set_feature_micron(int P1)
{
	_REG32(NF_DATA_TL0) = 0x4;	//Set data transfer count[7:0]
	_REG32(NF_DATA_TL1) = 0x80;	//Set data transfer count[13:8], SRAM path and access mode

	//Set PP
	_REG32(NF_PP_RDY) = 0;	//Data read to DRAM from NAND through PP
	_REG32(NF_PP_CTL0) = 1;	//Set PP starting address[9:8], PP reset and PP enable
	_REG32(NF_PP_CTL1) = 0;	//Set PP starting assdress[7:0]

	//Set command
	_REG32(NF_CMD1) = 0xEF;	//Set CMD1
	_REG32(NF_ND_CTL) = 0x80;	//Enable Xfer, eanble ECC and set transfer mode
	polling_timeout(NF_ND_CTL, 0x80, 0x00, 0);	//Wait xfer done

	//Set address,
	_REG32(NF_PAGE_ADR0) = 0x89;
	_REG32(NF_PAGE_ADR2) = 0xE0;
	_REG32(NF_ND_CTL) = 0x81;	//Enable Xfer, eanble ECC and set transfer mode
	polling_timeout(NF_ND_CTL, 0x80, 0x00, 0);	//Wait xfer done

	//Reset PP
	_REG32(NF_PP_RDY) = 0;	//Data read to DRAM from NAND through PP
	_REG32(NF_PP_CTL0) = 0x2;

	//Set data
	_REG32(NF_SRAM_CTL) = 0x30;	//Enable direct access SRAM
	_REG32(NF_PAGE_ADR0) = P1;	//Set data
	_REG32(NF_SRAM_CTL) = 0;		//Disable direct access SRAM

	//Set PP
	_REG32(NF_PP_CTL0) = 0x1;
	_REG32(NF_PP_CTL0) = 0;
	_REG32(NF_ND_CTL) = 0x83;	//Enable Xfer, eanble ECC and set transfer mode
	polling_timeout(NF_ND_CTL, 0xc0, 0x40, 0);	//Wait xfer done
}


void nf_set_pre_condition_toshiba(void){

	_REG32(NF_CMD1) = 0x5C;	//Set CMD1
	_REG32(NF_ND_CTL) = 0x80;	//Enable Xfer, eanble ECC and set transfer mode
	polling_timeout(NF_ND_CTL, 0x80, 0x00, 0);	//Wait xfer done

    	_REG32(NF_CMD1) = 0xC5;	//Set CMD1
	_REG32(NF_ND_CTL) = 0x80;	//Enable Xfer, eanble ECC and set transfer mode
	polling_timeout(NF_ND_CTL, 0x80, 0x00, 0);	//Wait xfer done
}


void nf_set_feature_toshiba_new_15nm(int P1)
{

    int i = 0;

            _REG32(0x98000038) = 0xf;
            _REG32(NF_T3) = 0x2;
            _REG32(NF_T3) = 0x2;
            _REG32(NF_T3) = 0x2;

    for(i=4;i<9;i++){
	//REG32(NF_DATA_TL0) = 0x4;	//Set data transfer count[7:0]
	//REG32(NF_DATA_TL1) = 0x80;	//Set data transfer count[13:8], SRAM path and access mode

	//Set PP
	//REG32(NF_PP_RDY) = 0;	//Data read to DRAM from NAND through PP
	//REG32(NF_PP_CTL0) = 1;	//Set PP starting address[9:8], PP reset and PP enable
	//REG32(NF_PP_CTL1) = 0;	//Set PP starting assdress[7:0]

	//Set command
	_REG32(NF_CMD1) = 0x55;	//Set CMD1
	_REG32(NF_ND_CTL) = 0x80;	//Enable Xfer, eanble ECC and set transfer mode
	polling_timeout(NF_ND_CTL, 0x80, 0x00, 0);	//Wait xfer done

	//Set address,
	if(i==8)
                _REG32(NF_PAGE_ADR0) = 0x0d;
        else
	        _REG32(NF_PAGE_ADR0) = i;
	_REG32(NF_PAGE_ADR2) = 0xE0;
	_REG32(NF_ND_CTL) = 0x81;	//Enable Xfer, eanble ECC and set transfer mode
	polling_timeout(NF_ND_CTL, 0x80, 0x00, 0);	//Wait xfer done

	//Reset PP
	_REG32(NF_PP_RDY) = 0;	//Data read to DRAM from NAND through PP
	_REG32(NF_PP_CTL0) = 0x2;

	//Set data
	_REG32(NF_SRAM_CTL) = 0x30;	//Enable direct access SRAM
	_REG32(NF_PAGE_ADR0) = read_retry_toshiba_cnt_new_15nm[P1][i-4];	//Set data
	_REG32(NF_SRAM_CTL) = 0;		//Disable direct access SRAM


	// Set data transfer count[7:0]
	_REG32(NF_DATA_TL0) = 0x1;

	// Set data transfer count[13:8] and access mode
	_REG32(NF_DATA_TL1) = 0x80;

	//Set PP
	_REG32(NF_PP_CTL0) = 0x1;
	//REG32(NF_PP_CTL0) = 0;
	_REG32(NF_ND_CTL) = 0x83;	//Enable Xfer, eanble ECC and set transfer mode
	polling_timeout(NF_ND_CTL, 0xc0, 0x40, 0);	//Wait xfer done
	}

            _REG32(0x98000038) = 0x4;
            _REG32(NF_T3) = 0x2;
            _REG32(NF_T3) = 0x2;
            _REG32(NF_T3) = 0x2;

        //if(P1==4){
            	//REG32(NF_CMD1) = 0xb3;	//Set CMD1
        	//REG32(NF_ND_CTL) = 0x80;	//Enable Xfer, eanble ECC and set transfer mode
        	//polling_timeout(NF_ND_CTL, 0x80, 0x00, 0);	//Wait xfer done
        //}
}
void nf_set_ext_feature_toshiba(void){

	_REG32(NF_CMD1) = 0x26;	//Set CMD1
	_REG32(NF_ND_CTL) = 0x80;	//Enable Xfer, eanble ECC and set transfer mode
	polling_timeout(NF_ND_CTL, 0x80, 0x00, 0);	//Wait xfer done

    	_REG32(NF_CMD1) = 0x5d;	//Set CMD1
	_REG32(NF_ND_CTL) = 0x80;	//Enable Xfer, eanble ECC and set transfer mode
	polling_timeout(NF_ND_CTL, 0x80, 0x00, 0);	//Wait xfer done
}

// add read-retry for TOSHIBA
void nf_set_terminate_toshiba(void){

	_REG32(NF_CMD1) = 0xff;	//Set CMD1
	_REG32(NF_ND_CTL) = 0x80;	//Enable Xfer, eanble ECC and set transfer mode
	polling_timeout(NF_ND_CTL, 0x80, 0x00, 0);	//Wait xfer done

}

void nf_set_feature_toshiba(int P1)
{

    int i = 0;

            _REG32(0x98000038) = 0xf;
            _REG32(NF_T3) = 0x2;
            _REG32(NF_T3) = 0x2;
            _REG32(NF_T3) = 0x2;

    for(i=4;i<8;i++){
	//REG32(NF_DATA_TL0) = 0x4;	//Set data transfer count[7:0]
	//REG32(NF_DATA_TL1) = 0x80;	//Set data transfer count[13:8], SRAM path and access mode

	//Set PP
	//REG32(NF_PP_RDY) = 0;	//Data read to DRAM from NAND through PP
	//REG32(NF_PP_CTL0) = 1;	//Set PP starting address[9:8], PP reset and PP enable
	//REG32(NF_PP_CTL1) = 0;	//Set PP starting assdress[7:0]

	//Set command
	_REG32(NF_CMD1) = 0x55;	//Set CMD1
	_REG32(NF_ND_CTL) = 0x80;	//Enable Xfer, eanble ECC and set transfer mode
	polling_timeout(NF_ND_CTL, 0x80, 0x00, 0);	//Wait xfer done

	//Set address,
	_REG32(NF_PAGE_ADR0) = i;
	_REG32(NF_PAGE_ADR2) = 0xE0;
	_REG32(NF_ND_CTL) = 0x81;	//Enable Xfer, eanble ECC and set transfer mode
	polling_timeout(NF_ND_CTL, 0x80, 0x00, 0);	//Wait xfer done

	//Reset PP
	_REG32(NF_PP_RDY) = 0;	//Data read to DRAM from NAND through PP
	_REG32(NF_PP_CTL0) = 0x2;

	//Set data
	_REG32(NF_SRAM_CTL) = 0x30;	//Enable direct access SRAM
	_REG32(NF_PAGE_ADR0) = P1;	//Set data
	_REG32(NF_SRAM_CTL) = 0;		//Disable direct access SRAM


	// Set data transfer count[7:0]
	_REG32(NF_DATA_TL0) = 0x1;

	// Set data transfer count[13:8] and access mode
	_REG32(NF_DATA_TL1) = 0x80;

	//Set PP
	_REG32(NF_PP_CTL0) = 0x1;
	//REG32(NF_PP_CTL0) = 0;
	_REG32(NF_ND_CTL) = 0x83;	//Enable Xfer, eanble ECC and set transfer mode
	polling_timeout(NF_ND_CTL, 0xc0, 0x40, 0);	//Wait xfer done
	}

            _REG32(0x98000038) = 0x4;
            _REG32(NF_T3) = 0x2;
            _REG32(NF_T3) = 0x2;
            _REG32(NF_T3) = 0x2;
}


static unsigned int nf_read_page_with_retrial_c(unsigned int page_no, unsigned int dest_addr)
{
	unsigned int  res;
	unsigned int  retry_count;
	unsigned char flash_manufacturer;
  unsigned char flash_model;

	// normal read operation
	//printf("nf_read_page_with_retrial_c \n");
	res = nf_read_page_c_dma(page_no, dest_addr);

	if (res == 0)
		return res;	// read success

#ifdef READ_RETRY_UNICORN
#ifdef READ_RETRY

	// read failed, check flash ECC type
	if (nf_is_6bit_ecc() || nf_is_12bit_ecc())
        return res; // 6-bit or 12-bit ECC flash (no need to read retrial)

#ifdef ROM_INFO
	prints("read page ");
	print_hex(page_no);
	prints(" error\n");
#endif

	// check flash manufacturer
	flash_manufacturer = s_FlashID4Bytes & 0xff;
        flash_model = (s_FlashID4Bytes & 0xff0000)>>16;
	switch (flash_manufacturer) {
		case 0xad:	// Hynix
			for (retry_count = 0; retry_count < 6; retry_count++) {

#ifdef ROM_INFO
				prints("T");
#endif

				nf_set_read_retrial_hynix();
				res = nf_read_page_c_dma(page_no, dest_addr);
				if (res == 0) {

#ifdef ROM_INFO
					prints("Y\n");
#endif
					nf_restore_read_retrial();

					return res;	// read success
				}
#ifdef ROM_INFO
				prints("X\n");
#endif
			}
			if (retry_count != 6) // loop no completed
			{
				//faultDetect();
			}
			nf_restore_read_retrial();
			break;

		case 0x2c:	// Micron

                       if(flash_model == 0x44){    //Micron read-retry only for 40 bit
        			for (retry_count = 1; retry_count <= 7; retry_count++) {
#if 1
        				printf("Mr\n");
#endif
        				nf_set_feature_micron(retry_count);

        				res = nf_read_page_c_dma(page_no, dest_addr);
        				if (res == 0) {
                                                nf_set_feature_micron(0);	// set to 0.
        					break;
        				}
        			}
        			nf_set_feature_micron(0);	// set to 0.
			}
			break;

                case 0x98:      //TOSHIBA
#ifdef ROM_DEBUG
                         prints("read retry toshiba\n");
#endif
                        nf_set_pre_condition_toshiba();

                        if(flash_model == 0x94){    //for TC58TEG6DDL 15nm
#ifdef ROM_DEBUG
                                prints("TC58TEG6DDL\n");
#endif
                                for (retry_count = 0; retry_count <= 10; retry_count++) {
                                            nf_set_feature_toshiba_new_15nm(retry_count);

                                        nf_set_ext_feature_toshiba();
        				res = nf_read_page_c_dma(page_no, dest_addr);
        				if (res == 0) {
                                                nf_set_feature_toshiba_new_15nm(0);
                                                nf_set_terminate_toshiba();
        					break;
        				}
        			}

                                nf_set_feature_toshiba_new_15nm(0);
                                nf_set_terminate_toshiba();

                        }else{
#ifdef ROM_DEBUG
                                prints("toshiba normal\n");
#endif
                                for (retry_count = 1; retry_count <= 6; retry_count++) {
                                            nf_set_feature_toshiba(read_retry_toshiba_cnt[retry_count]);

                                        nf_set_ext_feature_toshiba();
        				res = nf_read_page_c_dma(page_no, dest_addr);
        				if (res == 0) {
                                                nf_set_feature_toshiba(0);
                                                nf_set_terminate_toshiba();
        					break;
        				}
        			}

                                nf_set_feature_toshiba(0);
                                nf_set_terminate_toshiba();
                        }
                        break;

		default:	// unknown (or not supported) manufacturer

#ifdef ROM_INFO
			prints("unknown");
#endif

			break;
	}

#ifdef ROM_INFO
	prints("\n");
#endif
#endif	// #ifdef READ_RETRY
#endif //ifdef READ_RETRY_UNICORN
	return res;
}


static int nf_spare_ddr_enabled(void)
{
	return ((_REG32(NF_SPR_DDR_CTL) >> 30) & 0x1);
}


//static unsigned int nf_get_sram(const int mapping, const unsigned int offset)
unsigned int nf_get_sram(const int mapping, const unsigned int offset)
{
	unsigned int data;
       //cloud add
//	arch_clean_invalidate_cache_range(PTR_U32(&data), 4);
	//printf(" clean data =%x \n ",data);
	//cloud aad finsih
	if (nf_spare_ddr_enabled()) {
		//printf("SPARE from DDR \n");
		data = (_REG32(NF_SPR_DDR_CTL) & 0x1fffffff) << 3;	// physical address
		data = _REG32(data + offset);
	}
	else {
		// configure spare area data in PP (16 byte: 6 byte for user-defined, 10 byte for ECC)
		_REG32(NF_PP_RDY) = 0; // disable read_by_pp
		_REG32(NF_SRAM_CTL) = 0x30 | mapping; // enable direct access to PP

		data = _REG32(NF_PAGE_ADR0 + offset);

		_REG32(NF_SRAM_CTL) = 0x0; // disable direct access

			//printf("SPARE from REG reg 0 = 0x%x   0x%x  , 0x%x , 0x%x \n", _REG32(NF_PAGE_ADR0),_REG32(NF_PAGE_ADR1),_REG32(NF_PAGE_ADR2),_REG32(NF_PAGE_ADR3));
	}
//printf("data =%x \n ",data);
#ifdef ROM_DEBUG
	prints("{");
	print_hex(data);
	prints("}");
#endif
	return data;
}



//static unsigned int nf_get_spare(void)
unsigned int nf_get_spare(void)
{
	// modify by angus, we need while 4 bytes of spare (2 bytes signautre & 2 bytes page serial no.)
	return (unsigned int) (nf_get_sram(0x4, 0)/* & 0xff*/);
}




//int nf_read(unsigned int start_page, unsigned char *buf, unsigned int size, unsigned int *spare)
int nf_read(unsigned int start_page, unsigned char *buf, unsigned int page_cnt, unsigned int *spare)
{
	unsigned int stop_page;
	int res;

   	arch_clean_invalidate_cache_range(PTR_U32(buf), page_cnt*g_PageSize);
	arch_clean_invalidate_cache_range(PTR_U32(spare), 4);
	 //cloud modify 20161215
	arch_invalidate_cache_range(PTR_U32(SPARE_DMA_ADDR), 4);
	// validate arguments (size should be aligned to page size boundary)
	/*
	if ( (buf == NULL)
		|| (start_page > g_PagePerBlock * g_BlockPerFlash)
		|| (size & (g_PageSize - 1))
		|| (size == 0) )
        return (-1);
        */
  	if ( (buf == NULL)
		|| (start_page > g_Max_Page)
		|| (page_cnt == 0) )
        return (-1);
	/*
	// do not allow write past end of flash
	stop_page = start_page + size / g_PageSize;
	if (stop_page > g_PagePerBlock * g_BlockPerFlash)
        return (-1);
      */

	stop_page = start_page+ page_cnt;
      	if (stop_page > g_Max_Page)
        return (-1);
      // clear DMA data
      //_REG32(SPARE_DMA_ADDR ) = 0xffffffff;
//     arch_invalidate_cache_range(PTR_U32(SPARE_DMA_ADDR), 4);

    while (start_page < stop_page)
	{
		res = nf_read_page_with_retrial_c(start_page, (unsigned long)buf);
		if (res){
//#ifdef ROM_INFO
            printf("nf_read_res: %x \n ",res);

//#endif
			return (-1);
        }


		if (spare) {
			*spare = nf_get_spare();
			//arch_clean_invalidate_cache_range(PTR_U32(spare), 4);
		}

		buf += g_PageSize;
		start_page++;
	}
	if (start_page < stop_page) // loop no completed
	{
		//faultDetect();
	}

	return 0;
}


static unsigned int nf_set_spare(unsigned int spare, unsigned int offset)
{
	unsigned char i, count, increment;
	unsigned int unit_len;

	// we always use dma for writing.

    //printf("nf_set_spare  REG_SPR_DDR_CTL =%x SPARE_DMA_ADDR =0x%x \n ",_REG32(NF_SPR_DDR_CTL),_REG32(SPARE_DMA_ADDR) );
	_REG32(NF_SPR_DDR_CTL) = 0x60000000 | ((SPARE_DMA_ADDR+offset) >> 3);
	//cloud test
	//arch_clean_invalidate_cache_range(PTR_U32(SPARE_DMA_ADDR), 4);

	if (nf_is_6bit_ecc() || nf_is_12bit_ecc())
		unit_len = 512;
	else
		unit_len = 1024;

	if (nf_spare_ddr_enabled()) {

		//REG32(NF_SPR_DDR_CTL) = 0x60000000 | (PHYS(SPARE_DMA_ADDR+offset) >> 3);
		//rtd_outl(SPARE_DMA_ADDR + offset, spare);
		//rtd_outl(SPARE_DMA_ADDR, spare);

		if (nf_is_6bit_ecc() || nf_is_12bit_ecc())
			increment = 8;	// 6B for software (8B spacing)
		else
			increment = 4;	// 4B for software

		// spare/data: 6B/512B based
		count = s_PageSize / unit_len;
		for (i = 0; i < count; i++) {
			// (only 6 byte is for software)
			_REG32(SPARE_DMA_ADDR + i*increment) = spare;
			//rtd_outl(SPARE_DMA_ADDR + i*increment, spare);
			//REG8(SPARE_DMA_ADDR + i*increment + 4) = 0xff;
			//REG8(SPARE_DMA_ADDR + i*increment + 5) = 0xff;
		}
		if (i != count) // loop no completed
		{
			//faultDetect();
		}
		// it work OK
              arch_clean_invalidate_cache_range(PTR_U32(SPARE_DMA_ADDR), 4);
		//printf("DDR ENABLE SPARE_DMA_ADDR =0x%x \n ",_REG32(SPARE_DMA_ADDR) );

	}
	else {
		_REG32(NF_PP_RDY) = 0; // disable read_by_pp
		_REG32(NF_SRAM_CTL) = 0x30 | 0x4; // enable direct access to PP

		_REG32(NF_PAGE_ADR0 + offset) = spare;

		_REG32(NF_SRAM_CTL) = 0x0; // disable direct access
	}

	return 0;
}

static void nf_set_random_col_addr(unsigned int data_col, unsigned int spare_col)
{
#ifdef ROM_DEBUG
		prints("set random col(");
		print_hex(data_col);
		prints(", ");
		print_hex(spare_col);
		prints(")\n");
#endif

	_REG32(NF_RND_DATASTR_COL_H) = data_col >> 8;
	_REG32(NF_RND_SPR_STR_COL_H) = spare_col >> 8;
	_REG32(NF_RND_SPR_STR_COL_L) = spare_col & 0xff;
}


static int nf_write_phys_page( unsigned int page_no, unsigned int src_addr)
{
	unsigned int temp;
	unsigned int data_unit;

	//printf("nf_write_phys_page ~~!!!\n");
	// determine read unit size
	if (nf_is_6bit_ecc() || nf_is_12bit_ecc()) {
		data_unit = 512;
        //printf("nf_is_SLC  src_addr =0x%x  \n",src_addr);
		if (nf_is_random_mode()) {
			_REG32(NF_RND_CMD1) = 0x85;
			nf_set_random_col_addr(0, s_PageSize);
		}
		_REG32(NF_BLANK_CHK) = 0x1;		// enable blank check
	}
	else {
        //printf("nf_is_MLC\n");
		data_unit = 1024;

		_REG32(REG_RMZ_CTRL) = 0x1;
                _REG32(REG_RMZ_SEED_CTRL) = 0x0;
		_REG32(NF_RND_RW_EN) &= ~0x1;	// disable_random_mode
		_REG32(NF_BLANK_CHK) = 0x1;		// enable blank check
	}

	// Set chip select
	_REG32(NF_CHIP_EN) = NAND_CHIP_SELECT;

	//Set data transfer count, data transfer count must be 0x200 at auto mode
	//Set SRAM path and access mode
	_REG32(NF_DATA_TL0) = data_unit & 0xff;
	_REG32(NF_DATA_TL1) = (data_unit >> 8) & 0x3f;	// transfer mode

	//Set page length at auto mode
	_REG32(NF_PAGE_LEN) = s_PageSize / data_unit;	//Set page length (in unit)

	//Set PP
	_REG32(NF_PP_RDY) = 0x0;	//NAND --> PP --> DRAM
	_REG32(NF_PP_CTL0) = 0x0;
	_REG32(NF_PP_CTL1) = 0;

	//Set table sram
	_REG32(NF_LOW_PWR) = 0;

	//Set command
	_REG32(NF_CMD1) = 0x80;
	_REG32(NF_CMD2) = 0x10;
	_REG32(NF_CMD3) = 0x70;

	//Set address
	_REG32(NF_PAGE_ADR0) =  page_no & 0xff;
	_REG32(NF_PAGE_ADR1) =  page_no >> 8 ;
	_REG32(NF_PAGE_ADR2) = ((page_no >>16 ) & 0x1f) | (0x1 << 5);
	_REG32(NF_PAGE_ADR3) = ((page_no >> 21) & 0x7) << 5;
	_REG32(NF_COL_ADR0)  = 0;
	_REG32(NF_COL_ADR1)  = 0;

	//Set ECC
	_REG32(NF_MULTI_CH_MODE) = 0x20;
	_REG32(NF_ECC_STOP) = 0x80;

	_REG32(NF_SPR_DDR_CTL) = 0x60000000 | ((SPARE_DMA_ADDR) >> 3);

	//Set DMA
	_REG32(NF_DMA_ADR) = PHYS(src_addr) >> 3;	// 8 bytes unit
	_REG32(NF_DMA_LEN) = s_PageSize / 0x200;	// DMA transfer unit is 512 bytes
	_REG32(NF_DMA_CONF) = 0x01;

	sync();

	//Enable Auto mode
	_REG32(NF_AUTO_TRIG) = 0x80|(s_PageSize == 512 ? 0x0 : 0x1);	//0x1: write in 2K page size
	//printf("SET command wait finish 11111111111 !!! \n");
	while( _REG32(NF_AUTO_TRIG) & 0x80 )
		;

	//Wait DMA done
	//printf("SET command wait finish 222222222222 !!! \n");
	while( _REG32(NF_DMA_CONF) & 0x1 );

	// wait ready.
	polling_timeout(NF_ND_CTL, 0x40, 0x40, 0);

	// execute command3 register and wait for executed completion
	_REG32(NF_POLL_STATUS) = (0x6<<1) | 0x1;
	//printf("SET command wait finish 33333333333333333 !!! \n");
	while ( _REG32(NF_POLL_STATUS) & 0x1)
		;

	temp = _REG32(NF_DATA) & 0x1;

	if (temp == 0) {
		/* show '.' in console */
		printf(".");
		return 0;
	}
	/* show '!' in console */
	printf("!(%x)" ,page_no);

	return -1;
}

#if 1
 ssize_t nf_write(unsigned int start_page, unsigned char *buf, unsigned int page_cnt, unsigned int spare, unsigned char doWriteSkip)
{
	unsigned int stop_page;
	int res;
       unsigned int i = 0 ;
	unsigned char isNeedWrite = 1;

   //    local_buf = malloc(page_cnt*g_PageSize);
    //printf("nf_write  start_page =0x%x , g_PagePerBlock =0x%x , g_BlockPerFlash =0x%x \n  ", start_page , g_PagePerBlock ,  g_BlockPerFlash  );

	arch_clean_invalidate_cache_range(PTR_U32(buf), g_PageSize * page_cnt);
	arch_clean_invalidate_cache_range(PTR_U32(&spare), 4);
/*
	memcpy(local_buf,buf,page_cnt*g_PageSize);
	memcpy(&local_spare,&spare,4);
*/
	//arch_clean_invalidate_cache_range(PTR_U32(&spare), 4);

  	if ( (buf == NULL)
		|| (start_page > g_Max_Page)
		|| (page_cnt == 0) )
        return (-1);

	// do not allow write past end of flash
	//stop_page = start_page + size / g_PageSize;

	stop_page = start_page+ page_cnt;
      	if (stop_page > g_Max_Page)
        return (-1);

	//printf("nf_write write start !!!!!!!! start_page = 0x%x  , stop_page = 0x%x  \n",start_page,stop_page);
	while (start_page < stop_page)
	{
		//prints("nf_write: start_page=0x");
		//print_hex(*start_page);
		//prints("\n");

        if (doWriteSkip){
            //if there are all 0xff in a whole page, skip this page
            for(i = 0; i < g_PageSize; i++){
                if(buf[i] != 0xff){
                    isNeedWrite = 1;
                    break;
                }
            }
            if(i == g_PageSize){
                //prints("s\n");
                isNeedWrite = 0;
            }
        }

        if (isNeedWrite){
            //prints("w\n");
    		nf_set_spare(spare, 0);
    		//res = nf_write_phys_page(*start_page, (unsigned long)buf);
    		//printf("nf_write buffer addr =%x \n ", buf[0] );
    		res = nf_write_phys_page(start_page, (unsigned long)buf);
    		if (res)
    			return (-1);
        }
		buf += g_PageSize;
		//*start_page = *start_page + 1;
		start_page = start_page + 1;
	}
	arch_clean_invalidate_cache_range(PTR_U32(buf), g_PageSize);
	arch_clean_invalidate_cache_range(PTR_U32(&spare), 4);

	/*
	if (*start_page < stop_page) // loop no completed
	{
		//faultDetect();
	}
	*/
	return 0;
}
#else
//int nf_write(unsigned int start_page, unsigned char *buf, unsigned int size, unsigned int spare, unsigned char doWriteSkip)
int nf_write(unsigned int start_page, unsigned char *buf, unsigned int page_cnt, unsigned int spare, unsigned char doWriteSkip)
{
	unsigned int stop_page;
	int res;
       unsigned int i = 0 ;
	unsigned char isNeedWrite = 1;
	//unsigned char local_buf[2048];
	unsigned char *local_buf =0 ;
	unsigned int local_spare;
       local_buf = malloc(page_cnt*g_PageSize);
	printf("nf_write  start_page =0x%x , g_PagePerBlock =0x%x , g_BlockPerFlash =0x%x \n  ", start_page , g_PagePerBlock ,  g_BlockPerFlash  );

	arch_clean_invalidate_cache_range(PTR_U32(local_buf), g_PageSize);
	arch_clean_invalidate_cache_range(PTR_U32(&local_spare), 4);

	memcpy(local_buf,buf,page_cnt*g_PageSize);
	memcpy(&local_spare,&spare,4);

	//arch_clean_invalidate_cache_range(PTR_U32(&spare), 4);

  	if ( (buf == NULL)
		|| (start_page > g_Max_Page)
		|| (page_cnt == 0) )
        return (-1);

	// do not allow write past end of flash
	//stop_page = start_page + size / g_PageSize;

	stop_page = start_page+ page_cnt;
      	if (stop_page > g_Max_Page)
        return (-1);

	printf("nf_write write start !!!!!!!! start_page = 0x%x  , stop_page = 0x%x  \n",start_page,stop_page);
	while (start_page < stop_page)
	{
		//prints("nf_write: start_page=0x");
		//print_hex(*start_page);
		//prints("\n");

        if (doWriteSkip){
            //if there are all 0xff in a whole page, skip this page
            for(i = 0; i < g_PageSize; i++){
                if(local_buf[i] != 0xff){
                    isNeedWrite = 1;
                    break;
                }
            }
            if(i == g_PageSize){
                //prints("s\n");
                isNeedWrite = 0;
            }
        }

        if (isNeedWrite){
            //prints("w\n");
    		nf_set_spare(local_spare, 0);
    		//res = nf_write_phys_page(*start_page, (unsigned long)buf);
    		printf("nf_write buffer addr =%x \n ", local_buf[0] );
    			res = nf_write_phys_page(start_page, (unsigned long)local_buf);
    		if (res)
    			return (-1);
        }
		local_buf += g_PageSize;
		//*start_page = *start_page + 1;
		start_page = start_page + 1;
	}
	/*
	if (*start_page < stop_page) // loop no completed
	{
		//faultDetect();
	}
	*/
	return 0;
}
#endif
int nf_erase_phys_block(unsigned int  page_addr)
{
	unsigned int  temp;

	// Set chip select
	_REG32(NF_CHIP_EN) = NAND_CHIP_SELECT;

	//Set command
	_REG32(NF_CMD1) = 0x60;		//Set CMD1
	_REG32(NF_CMD2) = 0xd0;		//Set CMD2
	_REG32(NF_CMD3) = 0x70;		//Set CMD3

	//Set address
	//note. page_addr[5:0] is ignored to be truncated as block
	_REG32(NF_PAGE_ADR0) =  page_addr & 0xff;
	_REG32(NF_PAGE_ADR1) =  page_addr >> 8;
	_REG32(NF_PAGE_ADR2) = ((page_addr >>16 ) & 0x1f) | (4 << 5);
	_REG32(NF_PAGE_ADR3) = ((page_addr >> 21) & 0x7) << 5;
	_REG32(NF_COL_ADR0)  = 0;
	_REG32(NF_COL_ADR1)  = 0;


	//Set ECC: Set HW no check ECC, no_wait_busy & edo.
#ifndef ENV_SIMULATION
	_REG32(NF_MULTI_CH_MODE) = 0x3 << 4;
#else
	_REG32(NF_MULTI_CH_MODE) = 0x1 << 4;
#endif

	//Enable Auto mode: Set and enable auto mode
	// and wait until auto mode done
	_REG32(NF_AUTO_TRIG) = 0x8a;

	while ( _REG32(NF_AUTO_TRIG) & 0x80)
		;

	// wait ready.
	polling_timeout(NF_ND_CTL, 0x40, 0x40, 0);

	// execute command3 register and wait for executed completion
	_REG32(NF_POLL_STATUS) = (0x6<<1) | 0x1;
	while ( _REG32(NF_POLL_STATUS) & 0x1)
		;

	temp = _REG32(NF_DATA) & 0x1;


	if (temp == 0) {
		printf("/(%x)", page_addr/g_PagePerBlock);
		return 0;
	}
	/* show 'X' in console */
	printf("X(%x) ", page_addr/g_PagePerBlock);
	return -1;
}


static ssize_t rtknand_bdev_read_block(struct bdev *bdev, void *buf, bnum_t block, uint count)
{
		int res;
		ssize_t read_block_cnt;
		unsigned int i,j;
    //  if(BAD_BLOK_NUM ==0) //no bad block
      if  (bbt[0].bad_block==BB_INIT )  // no bad block
			{
				res =nf_read(block*g_PagePerBlock, buf,count*g_PagePerBlock, temp_spare);
				read_block_cnt = count ;
				if(res ==0) return  read_block_cnt ;//block count
				else           return 1;
			}
			else
			{
				read_block_cnt =0;
				//BAD_BLOK_NUM =0;
				for (i=block ; i<(block+count);i++)
				{
						for(j=0;j<RBA;j++)
						{
								if(bbt[j].bad_block == i)   // bad block remap
								{
								//bad block
								res = nf_read(bbt[j].remap_block*g_PagePerBlock,( unsigned char*)(buf+((i-block)*g_BlockSize)), 1*g_PagePerBlock,temp_spare);
								if(res ==0) read_block_cnt++;
								break;
								}
								else if (bbt[j].bad_block == BB_INIT)  // not bad block ,read directly
								{
									res = nf_read(i*g_PagePerBlock,( unsigned char*)(buf+((i-block)*g_BlockSize)), 1*g_PagePerBlock,temp_spare);
									if(res ==0) read_block_cnt++;
									break;
								}
							}
				}
         return read_block_cnt ;
	}

	//int nf_read(unsigned int start_page, unsigned char *buf, unsigned int page_cnt, unsigned int *spare)
}
//static ssize_t rtknand_bdev_read_byte(struct bdev *bdev, void *buf, bnum_t byte_offset, uint count)
static ssize_t rtknand_bdev_read(struct bdev *bdev, void *buf, off_t offset, size_t len)
{
 	int res;
       ssize_t read_byte_cnt;
   	unsigned int i,j;
	unsigned int page_addr,page_num;
	unsigned int block_start,block_end;
	unsigned int page_shift ,read_page_cnt;
    //  if(BAD_BLOK_NUM ==0) //no bad block
      //printf(" rtknand_bdev_read   len =%lx  \n",len);
      if  (bbt[0].bad_block==BB_INIT )  // no bad block
      {
			       //printf("case1 \n");
				page_addr = 	 offset/g_PageSize ;
				page_num =  len/g_PageSize;
				 //printf("page_addr  =%x ,page_num=%x \n",page_addr,page_num);

				if ( (len%g_PageSize) != 0)  page_num = page_num+1;
				res =nf_read(page_addr, buf,page_num, temp_spare);
		read_byte_cnt =page_num *g_PageSize ;
		if(res ==0) return  read_byte_cnt ;//block count
		else           return 1;
		}
		else   //bad block exist
		{
		//printf("case2 \n");
	       read_byte_cnt =0;
   		block_start = offset/g_BlockSize ;
		block_end = block_start +(len/g_BlockSize);
	       page_shift = offset/g_PageSize;
		page_num =  len/g_PageSize;
		if(len%g_BlockSize !=0)  block_end ++;

		 //printf("block_start  =%x ,block_end=%x \n",block_start,block_end);

		for (i=block_start ; i<(block_end);i++)
		{
			for(j=0;j<RBA;j++)
			{
				if(bbt[j].bad_block == i)   // bad block remap
				{

					//bad block
                                   #if 1

					printf("this is bad  j =%x ,i=%x \n",j,i);
                  			page_addr = 	 offset/g_PageSize ;
					page_num =  len/g_PageSize;
			 	       if ( (len%g_PageSize) != 0)  page_num = page_num+1;

			              if (i==block_start)  //start
					{

					 read_page_cnt = ((block_start+1)*g_PagePerBlock-(page_addr));
					 if (read_page_cnt >  page_num)  read_page_cnt = page_num;
					 page_shift = page_addr -(block_start*g_PagePerBlock);
					 res = nf_read((bbt[j].remap_block*g_PagePerBlock)+page_shift,buf,read_page_cnt ,temp_spare);
			              }
					else if (i==(block_end-1))  //end
					{
					read_page_cnt =page_addr+page_num -(i*g_PagePerBlock);
					 res = nf_read(bbt[j].remap_block*g_PagePerBlock,buf,read_page_cnt ,temp_spare);
					}
					else
					{
					read_page_cnt =g_PagePerBlock;
					 res = nf_read(bbt[j].remap_block*g_PagePerBlock,buf,read_page_cnt ,temp_spare);
					}


			  		//res = nf_read(page_addr,buf, page_num,temp_spare);
					 if(res ==0)
					 {
					 buf=buf+(read_page_cnt*g_PageSize);
					 read_byte_cnt =  read_byte_cnt+(read_page_cnt*g_PageSize);
					 }
			 		break;
					#else


					page_num = (len%g_BlockSize)/g_PageSize;
					if ((len%g_BlockSize)%g_PageSize)  page_num = page_num +1;
					res = nf_read((bbt[j].remap_block*g_PagePerBlock)+(page_shift),buf,page_num ,temp_spare);
					if(res ==0) read_byte_cnt = (page_num*g_PageSize);
			  		break;
					#endif
                   		}
				else if (bbt[j].bad_block == BB_INIT)  // not bad block ,read directly
				{

			 		//printf(" not bad block ,read directly \n");
                  			page_addr = 	 offset/g_PageSize ;
					page_num =  len/g_PageSize;
			 	       if ( (len%g_PageSize) != 0)  page_num = page_num+1;

			              if (i==block_start)  //start
					{
					 read_page_cnt = ((block_start+1)*g_PagePerBlock-(page_addr));
					 if (read_page_cnt >  page_num)  read_page_cnt = page_num;
					 res = nf_read(page_addr,buf,read_page_cnt ,temp_spare);
			              }
					else if (i==(block_end-1))  //end
					{
					read_page_cnt =page_addr+page_num -(i*g_PagePerBlock);
					 res = nf_read(i*g_PagePerBlock,buf,read_page_cnt ,temp_spare);
					}
					else
					{
					read_page_cnt =g_PagePerBlock;
					 res = nf_read(i*g_PagePerBlock,buf,read_page_cnt ,temp_spare);
					}


			  		//res = nf_read(page_addr,buf, page_num,temp_spare);
					 if(res ==0)
					 {
					 buf=buf+(read_page_cnt*g_PageSize);
					 read_byte_cnt =  read_byte_cnt+(read_page_cnt*g_PageSize);
					 }
			 		break;
				}

                   }

               }



         return read_byte_cnt ;
	}

	//int nf_read(unsigned int start_page, unsigned char *buf, unsigned int page_cnt, unsigned int *spare)
}

 static ssize_t rtknand_bdev_write_block(struct bdev *bdev, const void *_buf, bnum_t block, uint count)
{
	int res;
	   ssize_t write_block_cnt;
   unsigned int i,j;
   if(bbt[0].bad_block==BB_INIT )  // no bad block
   {
           //erase
           for(i=block ;i<(block+count);i++)
           {
          		res =  nf_erase_phys_block(i*g_PagePerBlock);
	   		if(res !=0) return res ;
           }
          temp_spare[0]=0xffffffff;
	   res = nf_write(block*g_PagePerBlock,( unsigned char*)_buf, count*g_PagePerBlock,temp_spare[0],0);
	   write_block_cnt = count ;
          if(res ==0) return  write_block_cnt ;//block count
          else           return 1;  //read fail

	//int nf_write(unsigned int start_page, unsigned char *buf, unsigned int page_cnt, unsigned int spare, unsigned char doWriteSkip)
   }
   else
   {
              write_block_cnt =0;
   		//BAD_BLOK_NUM =0;
               for (i=block ; i<(block+count);i++)
               {


		     for(j=0;j<RBA;j++)
                   {
                   	if(bbt[j].bad_block == i)   // bad block remap
                   	{
                   	 //bad block
                   	res =  nf_erase_phys_block(bbt[j].remap_block*g_PagePerBlock);
	   		if(res !=0) return res ;
                      res = nf_write(bbt[j].remap_block*g_PagePerBlock,( unsigned char*)(_buf+((i-block)*g_BlockSize)), 1*g_PagePerBlock,temp_spare[0],0);
			 if(res ==0) write_block_cnt++;
			  break;
                   	}
			else if (bbt[j].bad_block == BB_INIT)  // not bad block ,read directly
			{
			  res =  nf_erase_phys_block(i*g_PagePerBlock);
	   		  if(res !=0) return res ;
			  res = nf_write(i*g_PagePerBlock,( unsigned char*)(_buf+((i-block)*g_BlockSize)), 1*g_PagePerBlock,temp_spare[0],0);
			 if(res ==0) write_block_cnt++;
			 break;
			}

                   }

               }



         return write_block_cnt ;
   }
}

#if BAD_BLOCK_PROTECT
void add_bbt_block(unsigned int bad_bb)
{
	unsigned int j;
	for(j=0;j<RBA;j++)
	{
		if(bbt[j].bad_block==BB_INIT )  // no bad block
		{
			bbt[j].bad_block = bad_bb;
		}
	}
}
	#endif
 //static ssize_t rtknand_bdev_write_byte(struct bdev *bdev, const void *_buf, bnum_t byte_offset, uint count)
 static ssize_t rtknand_bdev_write(struct bdev *bdev, const void *buf, off_t offset, size_t len)
 {
		ssize_t write_byte_cnt =0;
		int res;
		unsigned int i,j;
		unsigned int page_addr,page_num;
		unsigned int block_start,block_end;
	#if BAD_BLOCK_PROTECT
	unsigned int bad_flag=0;
	#endif
		block_start = offset/g_BlockSize;
		block_end =   block_start+(len/g_BlockSize);
		if ((len%g_BlockSize))  block_end ++;
   if(bbt[0].bad_block==BB_INIT )  // no bad block
   {
//erase
		for(i=block_start ;i<(block_end);i++)
		{
			res =  nf_erase_phys_block(i*g_PagePerBlock);
			if(res !=0)
			{
				  printf("bad block found !!!! \n");
			#if BAD_BLOCK_PROTECT
			bad_flag = 1 ;
			add_bbt_block(i);
			#else
			return res ;
			#endif
			}
		}
		#if BAD_BLOCK_PROTECT
		if (bad_flag == 1)
		{
			rtk_create_bbt(BB1_BLOCK*g_PagePerBlock);
			rtk_create_bbt(BB2_BLOCK*g_PagePerBlock);
			bad_flag =0;
		}
  #endif
   }
		if(bbt[0].bad_block==BB_INIT )  // no bad block
		{

	   page_addr = offset/g_PageSize;
	   page_num = len / g_PageSize;
	   if ( (len%g_PageSize) != 0)  page_num = page_num+1;
          temp_spare[0]=0xffffffff;
	   res = nf_write(page_addr,( unsigned char*)buf, page_num,temp_spare[0],0);
	   //write_byte_cnt = len ;
	   write_byte_cnt = page_num*g_PageSize;
          if(res ==0) return  write_byte_cnt ;//byte count
          else           return 1;  //read fail
	//int nf_write(unsigned int start_page, unsigned char *buf, unsigned int page_cnt, unsigned int spare, unsigned char doWriteSkip)
		}
   else
   {
			write_byte_cnt =0;
			//BAD_BLOK_NUM =0;
			for (i=block_start ; i<block_end;i++)
			{
		  		for(j=0;j<RBA;j++)
					{
						if(bbt[j].bad_block == i)   // bad block remap
            {
							res =  nf_erase_phys_block(bbt[j].remap_block*g_PagePerBlock);
							if(res !=0) return res ;
							if(( (i ==(block_start))&&(g_BlockSize>len))||((i ==(block_end))&&(len%g_BlockSize !=0)))   //read size not reach block size
							{
									page_num = (len%g_BlockSize)/g_PageSize;
									if ((len%g_BlockSize)%g_PageSize)  page_num = page_num +1;
									res = nf_write(bbt[j].remap_block*g_PagePerBlock,( unsigned char*)(buf+((i-block_start)*g_BlockSize)),page_num ,temp_spare[0],0);
									if(res ==0) write_byte_cnt = write_byte_cnt + (page_num*g_PageSize);
              }
							else
							{
								//bad block
								res = nf_write(bbt[j].remap_block*g_PagePerBlock,( unsigned char*)(buf+((i-block_start)*g_BlockSize)), 1*g_PagePerBlock,temp_spare[0],0);
								if(res ==0) write_byte_cnt=write_byte_cnt + g_BlockSize;
							}
							break;
             }
							else if (bbt[j].bad_block == BB_INIT)  // not bad block ,read directly
							{
			  			res =  nf_erase_phys_block(i*g_PagePerBlock);
			if(res !=0)  //return res ;
			  {
				#if BAD_BLOCK_PROTECT
					bad_flag = 1 ;
					add_bbt_block(i);
				#else
					return res ;
				#endif
	   		  }
							if(((i ==(block_start))&&(g_BlockSize>len))||((i ==(block_end))&&(len%g_BlockSize !=0)))   //read size not reach block size
							{
								page_num = (len%g_BlockSize)/g_PageSize;
								if ((len%g_BlockSize)%g_PageSize)  page_num = page_num +1;
								res = nf_write(i*g_PagePerBlock,( unsigned char*)(buf+((i-block_start)*g_BlockSize)),page_num ,temp_spare[0],0);
								if(res ==0) write_byte_cnt = write_byte_cnt + (page_num*g_PageSize);
							}
							else
							{
							res = nf_write(i*g_PagePerBlock,( unsigned char*)(buf+((i-block_start)*g_BlockSize)), 1*g_PagePerBlock,temp_spare[0],0);
							if(res ==0) write_byte_cnt= write_byte_cnt+g_BlockSize;
							}
							break;
						}
						}
               }
 				#if BAD_BLOCK_PROTECT
				if(bad_flag == 1)
				{
					rtk_create_bbt(BB1_BLOCK*g_PagePerBlock);
					rtk_create_bbt(BB2_BLOCK*g_PagePerBlock);
					bad_flag =0;
				}
 				#endif
				return write_byte_cnt ;
   }
 }
static int rtknand_bdev_ioctl(struct bdev *bdev, int request, void *argp)
{
	return g_BlockSize;
}

void print_rtknand_usage(void)
{
	printf("page_size: 0x%x, block_size: 0x%x, page_per_block:0x%x, total_block: 0x%x\n",
		g_PageSize, g_BlockSize, g_PagePerBlock, g_Total_block);

	printf("rtknand commands usage:\n");
	printf("rtknand read mem_addr page_addr page_count\n");
	printf("rtknand write mem_addr page_addr page_count\n");
	printf("rtknand erase blk_addr [ end_blk_addr ]\n");
}

int do_rtknand(int argc, const cmd_args *argv)
{
	unsigned int ret = 0;
	unsigned int i = 0;

	unsigned int blk_addr, end_blk_addr, page_addr, page_count;
	unsigned char *mem_addr;

	if (argc != 5 && argc != 4 && argc != 3) {
		print_rtknand_usage();
		return -1;
	}

	// read/write commands
	if (argc == 5) {
	    mem_addr   = argv[2].p;
		page_addr   = argv[3].u;
		page_count  = argv[4].u;

		if (strncmp(argv[1].str, "read", 4) == 0) {
			ret = nf_read(page_addr, mem_addr, page_count, temp_spare);
			return ret;
		}
		else if (strncmp(argv[1].str, "write", 5) == 0) {
			temp_spare[0] = 0xffffffff;
			ret = nf_write(page_addr, mem_addr, page_count, temp_spare[0], 0);
			return ret;
		}
		else {
			print_rtknand_usage();
		}
	}
	// erase command
	else if (argc == 3 || argc == 4) {
		blk_addr = argv[2].u;

		if (argc == 4) {
			end_blk_addr = argv[3].u;
		}

		if (strncmp(argv[1].str, "erase", 5) != 0) {
			print_rtknand_usage();
			return -1;
		}
		else {
			if (argc == 3 ) {
				ret = nf_erase_phys_block(blk_addr*g_PagePerBlock);
				printf("\n");
				return ret;
			}
			else if (argc == 4) {
				for (i = blk_addr; i <= end_blk_addr; i ++) {
					ret = nf_erase_phys_block(i*g_PagePerBlock);
					if (ret) {
						printf("\n");
						return ret;
					}
				}
				printf("\n");
				return 0;
			}
		}
	}

	return -1;
}


STATIC_COMMAND_START
STATIC_COMMAND("rtknand", "rtk nand commands", (console_cmd)&do_rtknand)
STATIC_COMMAND_END(rtknand);


