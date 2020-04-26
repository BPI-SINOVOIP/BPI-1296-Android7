/*
 *  directtv_decoder.c - direct tv decoder for RTK irda driver
 *
 * Copyright (C) 2017 Realtek Semiconductor Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/io.h>
#include "ir_sw_decoder.h"

unsigned char encode_mode=0;
unsigned char encode_num=0;

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

static int directtv_length_decode(unsigned int len_h, unsigned int len_l, unsigned int encode_mode)
{
	unsigned int len_total;

	len_total = len_h + len_l;

	if(encode_mode==0) {
		if(len_total < 5 || len_total > 13)
			return -1;
		else if(len_total<=7)
			return 0;
		else if(len_total<=10 && len_h<len_l)
			return 1;
		else if(len_total<=10 && len_h>len_l)
			return 2;
		else
			return 3;
	} else {
		if(len_total < 5 || len_total >10)
			return -1;
		else if(len_total <= 7)
			return 0;
		else
			return 1;
	}
}

static int directtv_checksum(int code)
{
	int checksum;
	int temp = code >> 4;

	checksum = ((temp&0xC0)>>6)*7 + ((temp&0x30)>>4)*5 + ((temp&0xC)>>2)*3 + (temp&0x3);
	checksum = checksum & 0xF;
	if(checksum == (code&0xF))
		return 1;
	else
		return 0;
}

int raw_directtv_decoder(struct ir_sw_decoder *pdec, unsigned int *fifoval, int fifolv)
{
	unsigned int length_low=0, length_high=0;
	int code;
	unsigned int fifoptr=0, bitptr=32;

	pdec->codenum=0;

	while(1) {
		length_high = length_low = 0;
		length_high = get_bit_count(fifoval, fifolv, &fifoptr, &bitptr, 0);
		length_low = get_bit_count(fifoval, fifolv, &fifoptr, &bitptr, 1);
		if( bitptr==0 && fifoptr>=fifolv ) {
			if(pdec->fifostate==3 || pdec->fifostate==1) {
				pdec->fifostate = pdec->symbol_cnt = pdec->symbol = encode_mode = 0;
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
					if(pdec->length_hb>13 && pdec->length_lb>=5 && pdec->length_lb<=8) {
						pdec->symbol_cnt = pdec->symbol = encode_mode = 0;
						encode_num = 8;
						pdec->fifostate = 2;
					} else if(pdec->length_hb>=10 && pdec->length_hb<=14 && pdec->length_lb>=2 && pdec->length_lb<=4) {
						pdec->symbol_cnt = pdec->symbol = 0;
						encode_mode = 1;
						encode_num = 11;
						pdec->fifostate = 2;
					}
					else
						pdec->fifostate = 0;
				}
				pdec->length_hb = pdec->length_lb = 0;
				if(pdec->fifostate==2)
					goto decode_start;
			}
			if(length_high>13 && length_low>=5 && length_low<=8) {
				pdec->symbol_cnt = pdec->symbol = encode_mode = 0;
				encode_num = 8;
				pdec->fifostate = 2;
			} else if(length_high>=10 && length_high<14 && length_low>=2 && length_low<=4) {
				pdec->symbol_cnt = pdec->symbol = 0;
				encode_num = 11;
				encode_mode = 1;
				pdec->fifostate = 2;
			}
			else
				pdec->fifostate = 0;
		}
		else if(pdec->fifostate>=2 && pdec->symbol_cnt<=encode_num) {
			if(pdec->fifostate==3) {
				pdec->fifostate = 2;
				if(length_high==0 || length_low==0 || pdec->length_hb==0 || pdec->length_lb==0) {
					length_low += pdec->length_lb;
					length_high += pdec->length_hb;
				} else {
					code = directtv_length_decode(pdec->length_hb, pdec->length_lb, encode_mode);
					if(code < 0)
						pdec->fifostate = pdec->symbol_cnt = pdec->symbol = 0;
					else {
						if(encode_mode==0)
							pdec->symbol = pdec->symbol | code << (encode_num-1-pdec->symbol_cnt++)*2;
						else
							pdec->symbol = pdec->symbol | code << (encode_num-1-pdec->symbol_cnt++);
					}
				}
				pdec->length_hb = pdec->length_lb = 0;
			}
			if(pdec->symbol_cnt < encode_num) {
				code = directtv_length_decode(length_high, length_low, encode_mode);
				if(code < 0)
					pdec->fifostate = pdec->symbol_cnt = pdec->symbol = 0;
				else {
					if(encode_mode==0)
						pdec->symbol = pdec->symbol | code << (encode_num-1-pdec->symbol_cnt++)*2;
					else
						pdec->symbol = pdec->symbol | code << (encode_num-1-pdec->symbol_cnt++);
				}
			}
			if(pdec->symbol_cnt==encode_num) {
				if(encode_mode==0) {
					if(directtv_checksum(pdec->symbol))
						pdec->valid_code[pdec->codenum++] = (pdec->symbol & 0xFF0) >> 4;
				} else
					pdec->valid_code[pdec->codenum++] = pdec->symbol;
				pdec->fifostate = pdec->symbol = pdec->symbol_cnt = 0;
			}
		}
	}
	if(pdec->fifostate==2 || ((length_high>0 && length_low==0) || (length_high>=10)) ) {
		pdec->length_hb = length_high;
		pdec->length_lb = length_low;
		pdec->fifostate += 1;
	}
	if(pdec->codenum>0)
		return pdec->valid_code[pdec->codenum-1];
	else
		return -1;
}

