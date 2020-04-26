/*********************************************************************************/
/*                                                                               */
/* filename	:	SP_struct.h                                                        */
/*                                                                               */
/* Purpose	: sub-picture decoder data structure and                             */
/*            function prototype definitions                                     */
/*                                                                               */
/*                                                                               */
/*                                                                               */
/* Copyright (c) 2004 Realtek Semiconductor Corp. All rights reserved.           */
/*                                                                               */
/* No part of this document may be copied, reproduced,                           */
/* or transmitted in any form without the prior written                          */
/* consent of Realtek Semiconductor Corp. Hsinchu, Taiwan                        */
/*                                                                               */
/* Revision History:                                                             */
/*                                                                               */
/*      Date               By                  Description                       */
/* ============================================================================= */
/*  October, 2004   		PingHsiang Lu          Creation                          */
/*                                                                               */
/*                                                                               */
/*                                                                               */
/*********************************************************************************/
#ifndef _SP_DEF_H_
#define _SP_DEF_H_

#define B2N_16(x) \
 x = ((((x) & 0xff00) >> 8) | \
      (((x) & 0x00ff) << 8))

#define B2N_32(x) \
 x = ((((x) & 0xff000000) >> 24) | \
      (((x) & 0x00ff0000) >>  8) | \
      (((x) & 0x0000ff00) <<  8) | \
      (((x) & 0x000000ff) << 24))

#define SP_ADDR_MASK  0x1FFFFFFF /* to get physical address */

/* the maximum SPU size as per the DVD-Video Spec sec. 5.4.3.1 */
#define SP_MAX_SPU_SIZE  53220

/* the maximum size of DivX subtitle chuncks defined in the
   'DivX Subtitle Format Functional Specification' sec. 3.7.1 */
#define SP_MAX_DIVX_CHUNCK_SIZE  54272

#define SP_PALETTE_SIZE    16
#define SP_PXD_BUF_SIZE    ((SP_MAX_DIVX_CHUNCK_SIZE + 7) & ~7)
#define SP_CMD_BUF_SIZE    (((574-2+1) * (4 + 6*8) + 7) & ~7)
#define SP_MAX_PXD_BUFFERS 2
#define SP_MAX_CMD_BUFFERS 3

#define SP_ERROR  -1
#define SP_FAIL    0
#define SP_SUCCESS 1

#endif
