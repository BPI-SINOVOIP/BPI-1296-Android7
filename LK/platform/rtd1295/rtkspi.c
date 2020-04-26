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
//#include <platform/rbus/sb2_reg.h>
#include <platform/io.h>
//#include <platform/sb2.h>
#include <platform/setting.h>
#include <debug.h>
#include <lib/heap.h>
#include <lib/bio.h>
#include <arch/ops.h>
#include <arch/arm64.h>
#include <arch/arm64/mmu.h>
#include "rtkspi.h"
#include "flash_spi_list.h"
#include <lib/console.h>

#define LOCAL_TRACE	0
#define DEBUG

// global variable
s_device_type *pDevice = NULL;
static bdev_t spi_dev;

#ifdef DEBUG
#define dbg_printf(...) printf(__VA_ARGS__)
#else
#define dbg_printf(...)
#endif

static uint32_t swap_endian(uint32_t input)
{
    uint32_t output;

    output = (input & 0xff000000) >> 24 |
             (input & 0x00ff0000) >> 8 |
             (input & 0x0000ff00) << 8 |
             (input & 0x000000ff) << 24;

    return output;
}

static void spi_switch_read_mode(void)
{
    writel(0x00000003, SB2_SFC_OPCODE); //switch flash to read mode
    writel(0x00000018, SB2_SFC_CTL); //command cycle
    ISB;
	sync();
}

static unsigned int get_max_erase_size(s_device_type *pDev)
{
    if (pDev->sec_256k_en) {
        return (256UL << 10);
    }
    else if (pDev->sec_64k_en) {
        return (64UL << 10); 
    }
    else if (pDev->sec_32k_en) {
        return (32UL << 10); 
    }
    else if (pDev->sec_4k_en) {
        return (4UL << 10); 
    }

    return 0;
}

static unsigned int get_min_erase_size(s_device_type *pDev)
{
    if (!pDev)
        return 0;

    if (pDev->sec_4k_en) {
        return (4UL << 10);
    }
    else if (pDev->sec_32k_en) {
        return (32UL << 10); 
    }
    else if (pDev->sec_64k_en) {
        return (64UL << 10); 
    }
    else if (pDev->sec_256k_en) {
        return (256UL << 10); 
    }

    return 0;
}

static ssize_t rtkspi_bdev_read(struct bdev *bdev, void *buf, off_t offset, size_t len)
{
	return rtk_spi_read(offset, len, buf);
}

static ssize_t rtkspi_bdev_read_block(struct bdev *bdev, void *buf, bnum_t block, uint count)
{
	return rtk_spi_read(block, count, buf);
}

static ssize_t rtkspi_bdev_write(struct bdev *bdev, const void *buf, off_t offset, size_t len)
{
	return rtk_spi_write(offset, len, buf);
}

static ssize_t rtkspi_bdev_write_block(struct bdev *bdev, const void *buf, bnum_t block, uint count)
{
	return rtk_spi_write(block, count, buf);
}

static ssize_t rtkspi_bdev_erase(struct bdev *bdev, off_t offset, size_t len)
{
    return rtk_spi_erase(offset, len);
}

static int rtkspi_register_init(void)
{
    uint8_t tmp;
    uint8_t *nor_addr = (uint8_t *)SPI_FLASH_BASE_ADDR;

	// configure serial flash controller
    writel(0x001A1307, SB2_SFC_CE);   // setup control edge
    mdelay(100);
    writel(0x00000000, SB2_SFC_WP);   // disable hardware potection
  
    // enable write status register
    writel(0x00000050, SB2_SFC_OPCODE);
    writel(0x00000000, SB2_SFC_CTL);
    ISB;
	sync();
    tmp = readb(nor_addr);
    (void)tmp;
    writeb(0x0, nor_addr);

    // write status register , no memory protection
    writel(0x00000001, SB2_SFC_OPCODE);
    writel(0x00000010, SB2_SFC_CTL);
    ISB;
	sync();  
    writeb(0x0, nor_addr);

	return 0;
}

int rtk_spi_identify(void)
{
    uint32_t id;
    uint32_t idx;
    uint8_t * nor_addr = (uint8_t *)SPI_FLASH_BASE_ADDR;

    // configure spi flash controller register
    writel(0x00000001, SB2_SFC_POS_LATCH);   // set serial flash controller latch data at rising edge
    writel(0x001A1307, SB2_SFC_CE);          // setup control edge

    // read manufacture & device ID
    writel(0x0000009f, SB2_SFC_OPCODE);
    writel(0x00000010, SB2_SFC_CTL);
    ISB;
	sync();

    id = swap_endian(readl(nor_addr));
    id >>= 8;
    printf("flash id [0x%x]\n", id);
    
    /* find the match flash brand */
    for (idx = 0; idx < DEV_SIZE_S; idx++) {
        if (s_device[idx].id == id) {
            pDevice = &s_device[idx];

            // show flash info
            printf("sector 256k en: %d\n", pDevice->sec_256k_en);
            printf("sector  64k en: %d\n", pDevice->sec_64k_en);
            printf("sector  32k en: %d\n", pDevice->sec_32k_en);
            printf("sector   4k en: %d\n", pDevice->sec_4k_en);
            printf("page program  : %d\n", pDevice->page_program);
            printf("max capacity  : 0x%08x\n", pDevice->size);
            printf("spi type name : %s\n", pDevice->string);

            return 0;
        }
    }

    return -1;
}

int rtk_spi_init(void)
{
    arch_clean_invalidate_cache_range(MEMBASE, MEMSIZE);

    if (rtk_spi_identify())
        return -1;

    rtkspi_register_init();
    spi_switch_read_mode();

    bio_initialize_bdev(&spi_dev, BIO_SPI_NAME, SPI_BLOCK_SIZE, pDevice->size,
				0, NULL, BIO_FLAGS_NONE);
    spi_dev.read        = &rtkspi_bdev_read;
	spi_dev.read_block  = &rtkspi_bdev_read_block;
    spi_dev.write       = &rtkspi_bdev_write;
	spi_dev.write_block = &rtkspi_bdev_write_block;
    spi_dev.erase       = &rtkspi_bdev_erase;
	bio_register_device(&spi_dev);

    return 0;
}

unsigned int rtk_spi_erase(unsigned int nor_addr, unsigned int byte_size)
{
    volatile unsigned char tmp_sts;
    unsigned int start_address;
    unsigned int end_address;
    unsigned int curr_address;
    unsigned int tmp_cnt;
    unsigned int erase_size;

    unsigned int min_erase_size;
    unsigned int max_erase_size;

    nor_addr += SPI_FLASH_BASE_ADDR;

    dbg_printf("rtk_spi_erase: nor_addr = 0x%08x, byte_size = 0x%08x\n", nor_addr, byte_size);

    if (!pDevice) {
        printf("device not found\n");
        return -1;
    }
    
    min_erase_size = get_min_erase_size(pDevice);
    max_erase_size = get_max_erase_size(pDevice);

    if(!min_erase_size || !max_erase_size) {
    	printf("*** unknown SPI flash erase size, DO NOTHING ***\n");
    	return -1;
    }

    start_address = nor_addr;
    end_address = nor_addr + byte_size;

    dbg_printf("*** start addr : 0x%08x\n", start_address);
    dbg_printf("*** end addr : 0x%08x\n", end_address);

    // set aligment
    start_address &= ~(min_erase_size - 1);

    end_address   += (min_erase_size - 1);
    end_address   &= ~(min_erase_size - 1);

    dbg_printf("min erase size : 0x%08x\n", min_erase_size);
    dbg_printf("*** adjust start addr : 0x%08x\n", start_address);
    dbg_printf("*** adjust end addr : 0x%08x\n", end_address);

    //disable auto-prog
	writel(0x00000006, SB2_SFC_EN_WR);
	writel(0x00000005, SB2_SFC_WAIT_WR);
    ISB;
	sync();

	curr_address = start_address;

	while (curr_address < end_address) {
        
        // addr align max erase size
        if (((curr_address & (max_erase_size - 1)) == 0)  &&  
            ((curr_address + max_erase_size) <= end_address)) {
            erase_size = max_erase_size;
        }
        else {
            erase_size = min_erase_size;
        }
    
        printf("e");

        // write enable
        writel(0x00000006, SB2_SFC_OPCODE);
        mdelay(1); // work around for GD25Q80C_08Mbit
        writel(0x00000000, SB2_SFC_CTL);

        tmp_sts = readb(curr_address);

		do {
			if (erase_size == (256UL << 10)) {
                writel(0x000000d8, SB2_SFC_OPCODE);
                writel(0x00000008, SB2_SFC_CTL);
                ISB;
	            sync();
                tmp_sts = readb(curr_address);
                break;
            }
			else if (erase_size == (64UL << 10)) {
                writel(0x000000d8, SB2_SFC_OPCODE);
            	writel(0x00000008, SB2_SFC_CTL);
                ISB;
	            sync();
            	tmp_sts = readb(curr_address);
                break;
            }
			else if (erase_size == (32UL << 10)) {
            	writel(0x00000052, SB2_SFC_OPCODE);
                writel(0x00000008, SB2_SFC_CTL);
                ISB;
	            sync();
                tmp_sts = readb(curr_address);
                break;
            }
			else if (erase_size == (4UL << 10)) {
            	if (pDevice->id == PMC_4Mbit) {
                    writel(0x000000d7, SB2_SFC_OPCODE);
                }
                else {
                    writel(0x00000020, SB2_SFC_OPCODE);
                }
                writel(0x00000008, SB2_SFC_CTL);
                ISB;
	            sync();
                tmp_sts = readb(curr_address);
                break;
            }

		    printf("*** erase size(0x%08x) not support\n", erase_size);
            
			//enable auto-prog
			writel(0x00000106, SB2_SFC_EN_WR);
        	writel(0x00000105, SB2_SFC_WAIT_WR);
            ISB;
            sync();
            return -1;
            
		} while(0);

        // read status registers until write operation completed
        tmp_cnt = 0;
        do {
            if (!(tmp_cnt & 0x1FF)) {
            	tmp_cnt = 0;
            	printf("c");
            	mdelay(100);
            }
            
            tmp_cnt++;
            
            writel(0x00000005, SB2_SFC_OPCODE);
            writel(0x00000010, SB2_SFC_CTL);
            ISB;
            sync();
            tmp_sts = readb(curr_address);
            
        } while(tmp_sts & 0x1);

        curr_address += erase_size;
    }

    printf("\n");

	//enable auto-prog
	writel(0x00000106, SB2_SFC_EN_WR);
    writel(0x00000105, SB2_SFC_WAIT_WR);
    ISB;
	sync();

    spi_switch_read_mode();
    
    return end_address - start_address;
}


unsigned int rtk_spi_write(unsigned int nor_addr, unsigned int byte_size, const unsigned char *buffer)
{
    unsigned int write_byte_cnt = 0;
    unsigned int dot_print = 0;

    nor_addr += SPI_FLASH_BASE_ADDR;

    dbg_printf("rtk_spi_write: nor_addr = 0x%08x, byte_size = 0x%08x, buffer = %p\n", nor_addr, byte_size, buffer);

    // add by angus
    writel(0x00000106, SB2_SFC_EN_WR);
    writel(0x00000105, SB2_SFC_WAIT_WR);
    writel(0x001A1307, SB2_SFC_CE);

    // issue write command
    writel(0x00000002, SB2_SFC_OPCODE);
    writel(0x00000018, SB2_SFC_CTL);
    
    ISB;
	sync();

    mdelay(2);

    while (byte_size--) {
        /* show '.' in console */
        if ((byte_size % 1024) == 1023) {
            printf(".");
            dot_print = 1;
        }

        arch_clean_invalidate_cache_range(PTR_U32(buffer), 1);

        writeb(*(volatile unsigned char *)buffer, nor_addr);

        arch_clean_invalidate_cache_range(PTR_U32(buffer), 1);

        buffer++;
        nor_addr++;
        write_byte_cnt++;
    }

    if (dot_print)
        printf("\n");

    spi_switch_read_mode();

    readb(SPI_FLASH_BASE_ADDR);

    return write_byte_cnt;
}

unsigned int rtk_spi_read(unsigned int nor_addr, unsigned int byte_size, unsigned char *buffer)
{
    unsigned int read_byte_cnt = 0;
    unsigned int dot_print = 0;

    nor_addr += SPI_FLASH_BASE_ADDR;

    dbg_printf("rtk_spi_read: nor_addr = 0x%08x, byte_size = 0x%08x, buffer = %p\n", nor_addr, byte_size, buffer);

    spi_switch_read_mode();

    while (byte_size--) {
        /* show '.' in console */
        if ((byte_size % 1024) == 1023) {
            printf(".");
            dot_print = 1;
        }

        arch_clean_invalidate_cache_range(PTR_U32(buffer), 1);

        *(volatile unsigned char *)buffer = readb(nor_addr);

        arch_clean_invalidate_cache_range(PTR_U32(buffer), 1);

        buffer++;
        nor_addr++;
        read_byte_cnt++;
    }

    if (dot_print)
        printf("\n");

	return read_byte_cnt;
}

void print_rtkspi_usage(void){
    unsigned int min_erase_size;

    min_erase_size = get_min_erase_size(pDevice);
    
	printf("rtkspi commands usage:\n");
	printf("rtkspi read nor_addr mem_addr byte_size\n");
	printf("rtkspi write nor_addr mem_addr byte_size\n");
	printf("rtkspi erase nor_addr byte_size (align %dK)\n", (min_erase_size >> 10));
}

int do_rtkspi(int argc, const cmd_args *argv)
{
    unsigned int ret = 0;
    unsigned int nor_addr, byte_size;
	unsigned char* mem_addr;
    
	if (argc != 5 && argc != 4) {	
		print_rtkspi_usage();
		return -1;
	}
    
	// read/write commands
	if (argc == 5){	
		nor_addr   = argv[2].u;
        mem_addr   = argv[3].p;
		byte_size  = argv[4].u;
        
		if (strncmp(argv[1].str, "read", 4) == 0) {
			ret = rtk_spi_read(nor_addr, byte_size, mem_addr);
			return ((ret != byte_size) ? -1 : 0);
		}
		else if (strncmp( argv[1].str, "write", 5) == 0) {
			ret = rtk_spi_write(nor_addr, byte_size, mem_addr);
			return ((ret != byte_size) ? -1 : 0);
		}
		else {
			print_rtkspi_usage();
		}
	}
	// erase command
	else if (argc == 4) {
		if (strncmp(argv[1].str, "erase", 5) != 0) {
			print_rtkspi_usage();
            return 0;
		}

        nor_addr = argv[2].u;
		byte_size = argv[3].u;
        ret = rtk_spi_erase(nor_addr, byte_size);
        return ret;
    }

    return 0;
}

STATIC_COMMAND_START
STATIC_COMMAND("rtkspi", "rtk spi commands", (console_cmd)&do_rtkspi)
STATIC_COMMAND_END(rtkspi);

