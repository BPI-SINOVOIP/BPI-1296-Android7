/*
 *  Copyright (C) 2010 Realtek Semiconductors, All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __RTKSD_OPS_H
#define __RTKSD_OPS_H
#include <linux/completion.h>

void sync(struct rtkemmc_host *emmc_port);
void print_reg_sts(u32 cmd_idx, u32 rintsts);
void print_ip_desc(struct rtkemmc_host *emmc_port);

int rtkemmc_wait_opt_end(char*,struct rtkemmc_host *emmc_port,unsigned char cmdcode,unsigned int cmd_idx,unsigned char ignore_log);
void rtk_op_complete(struct rtkemmc_host *emmc_port);
char *rtkemmc_parse_token(const char *parsed_string, const char *token);
void rtkemmc_chk_param(u32 *pparam, u32 len, u8 *ptr);
void rtkemmc_set_mis_gpio(u32 gpio_num,u8 dir,u8 level);
void rtkemmc_set_iso_gpio(u32 gpio_num,u8 dir,u8 level);
int mmc_fast_write( unsigned int blk_addr,
                    unsigned int data_size,
                    unsigned char * buffer );

int mmc_fast_read( unsigned int blk_addr,
                   unsigned int data_size,
                   unsigned char * buffer );
struct completion* rtk_int_enable(struct rtkemmc_host *emmc_port, unsigned long msec);
int rtk_int_enable_and_waitfor(struct rtkemmc_host *emmc_port, u8 cmdcode, u32 cmd_idx, unsigned long msec, unsigned long dma_msec,u8 ignore_log);
int rtk_int_waitfor(struct rtkemmc_host *emmc_port, u8 cmdcode, u32 cmd_idx, unsigned long msec,unsigned long dma_msec,u8 ignore_log);
#endif

/* end of file */



