/*********************************************************************************/
/*                                                                               */
/* filename	:	SP_def.h                                       	                   */
/*                                                                               */
/* Purpose	: sub-picture decoder definitions                                    */
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
#ifndef _SP_STRUCT_H_
#define _SP_STRUCT_H_

//#include "VP_def.h"
//#include "VP_comp.h"
//#include "VP_Utilities.h"
//#include "VTM_struct.h"

#include "SP_def.h"
#include "rvsd_CommonFunc.h"

//#include "rpc_common.h"

/*************************/
/* Subpicture structures */
/*************************/
typedef enum
{
    FSTA_DSP = 0,
    STA_DSP,
    STP_DSP,
    SET_COLOR,
    SET_CONTR,
    SET_DAREA,    // 5
    SET_DSPXA,
    CHG_COLCON,
    ENABLE_HIGHLIGHT, // 8
    DISABLE_HIGHLIGHT,    // 9
    PALETTE_COMMAND,  // 10
    CMD_END=0xff
} SP_DCCMD ;


/* SPU structures */
typedef struct
{
    unsigned short SPU_SZ ;       // total size
    unsigned short SP_DCSQT_SA ;  // start address of DCSQT

} subPicUnitHeader ;


typedef struct
{
    unsigned short SP_DCSQ_STM ;     // Start Time of SP_DCSQ
    unsigned short SP_NXT_DCSQ_SA ;  // start address of the next SP_DCSQ

} spDCSQ ;


typedef struct
{
#if SP_LITTLE_ENDIAN
    unsigned char px1Color:4 ;
    unsigned char px2Color:4 ;
    unsigned char bgColor:4 ;
    unsigned char patColor:4 ;
#else
    unsigned char px2Color:4 ;
    unsigned char px1Color:4 ;
    unsigned char patColor:4 ;
    unsigned char bgColor:4 ;
#endif
} setCOLOR ;

typedef struct
{
#if SP_LITTLE_ENDIAN
    unsigned char px1Alpha:4 ;
    unsigned char px2Alpha:4 ;
    unsigned char bgAlpha:4 ;
    unsigned char patAlpha:4 ;
#else
    unsigned char px2Alpha:4 ;
    unsigned char px1Alpha:4 ;
    unsigned char patAlpha:4 ;
    unsigned char bgAlpha:4 ;
#endif
} setCONTR ;


typedef struct
{
#if SP_LITTLE_ENDIAN
    unsigned char startXUp :7 ; // 11 bit for unofficial HD-size DVD for DVIX SP
    unsigned char reserved1:1 ;
    unsigned char endXUp   :3 ;
    unsigned char reserved2:1 ;
    unsigned char startXLow:4 ;
    unsigned char endXLow     ;
    unsigned char startYUp :7 ;
    unsigned char reserved3:1 ;
    unsigned char endYUp   :3 ;
    unsigned char reserved4:1 ;
    unsigned char startYLow:4 ;
    unsigned char endYLow     ;
#else
    unsigned char reserved1:1 ;
    unsigned char startXUp :7 ; // 11 bit for unofficial HD-size DVD for DVIX SP
    unsigned char startXLow:4 ;
    unsigned char reserved2:1 ;
    unsigned char endXUp   :3 ;
    unsigned char endXLow     ;
    unsigned char reserved3:1 ;
    unsigned char startYUp :7 ;
    unsigned char startYLow:4 ;
    unsigned char reserved4:1 ;
    unsigned char endYUp   :3 ;
    unsigned char endYLow     ;
#endif
} setDAREA ;


typedef struct
{
    unsigned short staAddrOfTop ;
    unsigned short staAddrOfBottom ;

} setDSPXA ;


typedef struct
{
#if SP_LITTLE_ENDIAN
    unsigned int chgTerLiNum:10 ;
    unsigned int reserved2:2 ;
    unsigned int numOfChg:4 ;
    unsigned int chgStaLiNum:10 ;
    unsigned int reserved1:6 ;
#else
    unsigned int reserved1:6 ;
    unsigned int chgStaLiNum:10 ;
    unsigned int numOfChg:4 ;
    unsigned int reserved2:2 ;
    unsigned int chgTerLiNum:10 ;
#endif
} lnCTLI ;


typedef struct
{
#if SP_LITTLE_ENDIAN
    unsigned short chgStaPxNum:10 ;
    unsigned short reserved:6 ;
    unsigned short color ;
    unsigned short alpha ;
#else
    unsigned short reserved:6 ;
    unsigned short chgStaPxNum:10 ;
    unsigned short color ;
    unsigned short alpha ;
#endif
} pxCTLI ;

typedef struct
{
    unsigned char reserved:4 ;
    unsigned char hd:1 ;
    unsigned char palette:1 ;
    unsigned char hdrDone:1 ;
    unsigned char decode:1 ;

} SP_DEC_STATUS ;

/**
 *  \brief This struct will be delivered from sub-picture decoder to vout.
 */
typedef struct
{
    SP_STREAM_TYPE streamType ;
    long long      PTS ;

    VP_BUFFER     *pxd_poolId_addr ;
    VP_BUFFER     *cmd_poolId_addr ;

    unsigned int   topPxd_Addr ;
    unsigned int   botPxd_Addr ;
    unsigned int   cmd_Addr ;
    unsigned short top_num ;   // number of LN_CTLI in the top    field
    unsigned short bot_num ;   // number of LN_CTLI in the bottom field
    unsigned short ctrl_cmd ;  // DCCMD, bit 0 is FSTA_DSP, bit 1 is STA_DSP ...etc
    // bit  8: highlight enable
    // bit  9: highlight disable
    // bit 10: palette command

    VP_WINDOW  dispWin ;

    setCOLOR   dftColor ;
    setCONTR   dftContrast ;

    setCOLOR   hlColor ;
    setCONTR   hlContrast ;
    VP_WINDOW  hlWindow ;

    unsigned int palette[SP_PALETTE_SIZE] ;

} VP_OBJ_DVD_SP ;


typedef struct
{
    unsigned int   L_BS_Base ;   	/* bit stream base  address in byte units */
    unsigned int   L_BS_Limit ;  	/* bit stream limit address in byte units */
    unsigned int   L_BS_Write ;  	/* bit stream current write pointer in byte units */
    unsigned int   L_BS_Read ;  	/* bit stream current read  pointer in byte units */
    unsigned int  *L_BS_pWrPtr ;
    unsigned int  *L_BS_pRdPtr ;

    /* fields for inband command queue */
    VP_INBAND_RING_BUF  S_ICQ ;

    subPicUnitHeader SPUHeader ;
    unsigned short   DCSQ_SA ;

    SP_STREAM_TYPE  streamType ;
    unsigned char  pxd_offset ; /* dummy byte (0~7 byte) to satisfy DMA moving for PXD buffer */

    SP_DEC_STATUS  status ;     /* decoder status */
    VP_OBJ_DVD_SP  spInfo ;     /* sub-picture info. */

    long long      PTM ;    /* current PTM */

    /* pixel and command and buffers */
    VP_BUFFER  pxd_buffers[SP_MAX_PXD_BUFFERS] ;
    VP_BUFFER  cmd_buffers[SP_MAX_CMD_BUFFERS] ;

    unsigned long long PXDBuf[SP_PXD_BUF_SIZE/sizeof(unsigned long long) * SP_MAX_PXD_BUFFERS] ;
    unsigned long long CMDBuf[SP_CMD_BUF_SIZE/sizeof(unsigned long long) * SP_MAX_CMD_BUFFERS] ;

} SP_DECODE ;


/* function Prototypes */
int SP_Decoder_Create (SP_INFO* pSpInfo, VP_COMPONENT *, int) ;

#endif
