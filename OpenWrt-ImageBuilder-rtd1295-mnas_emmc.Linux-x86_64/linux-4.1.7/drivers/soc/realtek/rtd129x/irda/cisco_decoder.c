/*
 *  cisco_decoder.c - cisco decoder for RTK irda driver
 *
 * Copyright (C) 2017 Realtek Semiconductor Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/io.h>

#include "venus_ir.h"
#include "ir_sw_decoder.h"

static int get_bit_count(unsigned int *fifo, unsigned int number, unsigned int *ptr, unsigned int *bptr, int polar)
{
	unsigned int bit_cnt = 0;

	if(polar != 0 && polar != 1)
		return -1;

	while(1) {
		if(*bptr == 0) {
			(*ptr)++;
			if( *ptr>=number )
				return bit_cnt;
			(*bptr) = 32;
		} else {
			if( test_bit((*bptr)-1, (unsigned long *)(fifo+*ptr)) ) {
				if(polar == 1) {
					(*bptr)--;
					bit_cnt++;
				} else
					break;
			} else {
				if(polar == 0) {
					(*bptr)--;
					bit_cnt++;
				} else
					break;
			}
		}
	}
	return bit_cnt;
}

static int cisco_length_decode(unsigned int len_h, unsigned int len_l)
{
	unsigned int len_total;

	len_total = len_h + len_l;

	if(len_total > 6 && len_total < 10)
		return 0;
	else if(len_total > 15 && len_total < 19)
		return 1;
	else
		return -1;
}

int raw_cisco_decoder(struct ir_sw_decoder *pdec, unsigned int *fifoval, int fifolv)
{
	unsigned int length_low=0, length_high=0;
	unsigned int fifoptr=0, bitptr=32;
	static unsigned int lastsymbol;
	int code;

	pdec->codenum=0;
	
	while(1) {
		length_high = length_low = 0;
		length_high = get_bit_count(fifoval, fifolv, &fifoptr, &bitptr, 0);
		length_low = get_bit_count(fifoval, fifolv, &fifoptr, &bitptr, 1);
		if( bitptr==0 && fifoptr>=fifolv ) {
			if(pdec->fifostate==3 || pdec->fifostate==1) {
				pdec->fifostate = pdec->symbol_cnt = pdec->symbol = 0;
				pdec->length_hb = pdec->length_lb = 0;
			}
			break;
		}
decode_start:
		if(pdec->fifostate<2) {
			if(pdec->fifostate == 1) {
				if(length_high==0 || length_low==0 || pdec->length_lb==0 || pdec->length_hb==0) {
					length_low += pdec->length_lb;
					length_high += pdec->length_hb;
				} else {
					if((pdec->length_hb>=15 && pdec->length_hb<=19)) {
					//	if(pdec->length_lb>=10 && pdec->length_lb <=13) {
					//		pdec->symbol = lastsymbol;
					//		pdec->symbol_cnt = 16;
					//	} else if(pdec->length_lb>=21 && pdec->length_lb<=24)
						if(pdec->length_lb>=15 && pdec->length_lb<=19)
							pdec->symbol_cnt = pdec->symbol = 0;
						pdec->fifostate = 2;
					} else
						pdec->fifostate = 0;
				}
				pdec->length_hb = pdec->length_lb = 0;
				if(pdec->fifostate==2)
					goto decode_start;
			}
			if((length_high>=15 && length_high<=19)) {
//				if(length_low>=10 && length_low <=13) {
//					pdec->symbol = lastsymbol;
//					pdec->symbol_cnt = 16;
//					pdec->fifostate = 2;
//					goto decode_start;
//				} else if(length_low>=21 && length_low<=24)
				if(length_low>=15 && length_low<=19)
					pdec->symbol_cnt = pdec->symbol = 0;
				pdec->fifostate = 2;
			} else
				pdec->fifostate = 0;
		}
		else if(pdec->fifostate>=2 && pdec->symbol_cnt<=22) {
			if(pdec->fifostate==3) {
				pdec->fifostate = 2;
				if(length_high==0 || length_low==0 || pdec->length_hb==0 || pdec->length_lb==0) {
					length_low += pdec->length_lb;
					length_high += pdec->length_hb;
				} else {
					code = cisco_length_decode(pdec->length_hb, pdec->length_lb);
					if(code < 0)
						pdec->fifostate = pdec->symbol_cnt = pdec->symbol = 0;
					else
						pdec->symbol = pdec->symbol | code << (21-pdec->symbol_cnt++);
				}
				pdec->length_hb = pdec->length_lb = 0;
			}
			if(pdec->symbol_cnt < 22) {
				code = cisco_length_decode(length_high, length_low);
				if(code < 0)
					pdec->fifostate = pdec->symbol_cnt = pdec->symbol = 0;
				else
					pdec->symbol = pdec->symbol | code << (21-pdec->symbol_cnt++);
			}
			if(pdec->symbol_cnt==22) {
				lastsymbol = pdec->symbol & 0xffffff;
				pdec->valid_code[pdec->codenum++] = pdec->symbol & 0xffffff;
				pdec->fifostate = pdec->symbol = pdec->symbol_cnt = 0;
			}
		}
	}
	if(pdec->fifostate==2 || (length_high>0 && length_low==0) || length_high>13 ) {
		pdec->length_hb = length_high;
		pdec->length_lb = length_low;
		pdec->fifostate += 1;
	}
	if(pdec->codenum>0)
		return pdec->valid_code[pdec->codenum-1];
	else
		return -1;
}

