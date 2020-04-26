/*********************************************************************************/
/*                                                                               */
/* DVBSP_Decode_struct.h                                                         */
/*                                                                               */
/* Purpose: DVB subtitling decoder structure definitions                         */
/*                                                                               */
/*                                                                               */
/*                                                                               */
/* Copyright (c) 2006 Realtek Semiconductor Corp. All rights reserved.           */
/*                                                                               */
/* No part of this document may be copied, reproduced,                           */
/* or transmitted in any form without the prior written                          */
/* consent of Realtek Semiconductor Corp. Hsinchu, Taiwan                        */
/*                                                                               */
/* Revision History:                                                             */
/*                                                                               */
/*      Date               By                  Description                       */
/* ============================================================================= */
/*    07/27/06         Shiyan Pan              Creation                          */
/*                                                                               */
/*                                                                               */
/*                                                                               */
/*********************************************************************************/
#ifndef _DVBSP_DECODE_STRUCT_H_
#define _DVBSP_DECODE_STRUCT_H_

//#define DVBSP_SOFTWARE_DECODE

//#include "rpc_common.h"
//#include "VP_comp.h"
//#include "VP_Utilities.h"
//#include "VTM_struct.h"
//#include "VBM_struct.h"
//#include "se_agent_struct.h"

#include "../sp_common/sp_common.h"
#include "rvsd_CommonFunc.h"

#include "DVBSP_Decode_def.h"

#define BDSP_ON_MIPS 0

#ifdef DVBSP_SOFTWARE_DECODE

typedef struct
{
#if SP_LITTLE_ENDIAN
    unsigned int dummy:8 ;
    unsigned int code_1:2 ;
    unsigned int run_length_3_10:3 ;
    unsigned int switch_1:1 ;
    unsigned int code_0:2 ;
    unsigned int reserved:16 ;
#else
    unsigned int reserved:16 ;
    unsigned int code_0:2 ;
    unsigned int switch_1:1 ;
    unsigned int run_length_3_10:3 ;
    unsigned int code_1:2 ;
    unsigned int dummy:8 ;
#endif
} DVBSP_2BIT_CODE_0 ;


typedef struct
{
#if SP_LITTLE_ENDIAN
    unsigned int dummy:4 ;
    unsigned int code:2 ;
    unsigned int run_length_12_17:4 ;
    unsigned int switch_3:2 ;
    unsigned int switch_2:1 ;
    unsigned int zero:3 ;
    unsigned int reserved:16 ;
#else
    unsigned int reserved:16 ;
    unsigned int zero:3 ;
    unsigned int switch_2:1 ;
    unsigned int switch_3:2 ;
    unsigned int run_length_12_17:4 ;
    unsigned int code:2 ;
    unsigned int dummy:4 ;
#endif
} DVBSP_2BIT_CODE_1 ;


typedef struct
{
#if SP_LITTLE_ENDIAN
    unsigned int code:2 ;
    unsigned int run_length_29_284:8 ;
    unsigned int switch_3:2 ;
    unsigned int switch_2:1 ;
    unsigned int zero:3 ;
    unsigned int reserved:16 ;
#else
    unsigned int reserved:16 ;
    unsigned int zero:3 ;
    unsigned int switch_2:1 ;
    unsigned int switch_3:2 ;
    unsigned int run_length_29_284:8 ;
    unsigned int code:2 ;
#endif
} DVBSP_2BIT_CODE_2 ;


typedef struct
{
#if SP_LITTLE_ENDIAN
    unsigned int dummy:12 ;
    unsigned int run_length_3_9:3 ;
    unsigned int switch_1:1 ;
    unsigned int code:4 ;
    unsigned int reserved:12 ;
#else
    unsigned int reserved:12 ;
    unsigned int code:4 ;
    unsigned int switch_1:1 ;
    unsigned int run_length_3_9:3 ;
    unsigned int dummy:12 ;
#endif
} DVBSP_4BIT_CODE_0 ;


typedef struct
{
#if SP_LITTLE_ENDIAN
    unsigned int dummy:8 ;
    unsigned int code:4 ;
    unsigned int run_length_4_7:2 ;
    unsigned int switch_2:1 ;
    unsigned int switch_1:1 ;
    unsigned int zero:4 ;
    unsigned int reserved:12 ;
#else
    unsigned int reserved:12 ;
    unsigned int zero:4 ;
    unsigned int switch_1:1 ;
    unsigned int switch_2:1 ;
    unsigned int run_length_4_7:2 ;
    unsigned int code:4 ;
    unsigned int dummy:8 ;
#endif
} DVBSP_4BIT_CODE_1 ;


typedef struct
{
#if SP_LITTLE_ENDIAN
    unsigned int dummy:4 ;
    unsigned int code:4 ;
    unsigned int run_length_9_24:4 ;
    unsigned int switch_3:2 ;
    unsigned int switch_2:1 ;
    unsigned int switch_1:1 ;
    unsigned int zero:4 ;
    unsigned int reserved:12 ;
#else
    unsigned int reserved:12 ;
    unsigned int zero:4 ;
    unsigned int switch_1:1 ;
    unsigned int switch_2:1 ;
    unsigned int switch_3:2 ;
    unsigned int run_length_9_24:4 ;
    unsigned int code:4 ;
    unsigned int dummy:4 ;
#endif
} DVBSP_4BIT_CODE_2 ;


typedef struct
{
#if SP_LITTLE_ENDIAN
    unsigned int code:4 ;
    unsigned int run_length_25_280:8 ;
    unsigned int switch_3:2 ;
    unsigned int switch_2:1 ;
    unsigned int switch_1:1 ;
    unsigned int zero:4 ;
    unsigned int reserved:12 ;
#else
    unsigned int reserved:12 ;
    unsigned int zero:4 ;
    unsigned int switch_1:1 ;
    unsigned int switch_2:1 ;
    unsigned int switch_3:2 ;
    unsigned int run_length_25_280:8 ;
    unsigned int code:4 ;
#endif
} DVBSP_4BIT_CODE_3 ;


typedef struct
{
#if SP_LITTLE_ENDIAN
    unsigned int dummy:8 ;
    unsigned int run_length_1_127:7 ;
    unsigned int switch_1:1 ;
    unsigned int code:8 ;
    unsigned int reserved:8 ;
#else
    unsigned int reserved:8 ;
    unsigned int code:8 ;
    unsigned int switch_1:1 ;
    unsigned int run_length_1_127:7 ;
    unsigned int dummy:8 ;
#endif
} DVBSP_8BIT_CODE_0 ;


typedef struct
{
#if SP_LITTLE_ENDIAN
    unsigned int code:8 ;
    unsigned int run_length_3_127:7 ;
    unsigned int switch_1:1 ;
    unsigned int zero:8 ;
    unsigned int reserved:8 ;
#else
    unsigned int reserved:8 ;
    unsigned int zero:8 ;
    unsigned int switch_1:1 ;
    unsigned int run_length_3_127:7 ;
    unsigned int code:8 ;
#endif
} DVBSP_8BIT_CODE_1 ;


typedef struct
{
#if 0
//#if SP_LITTLE_ENDIAN
    unsigned char  done:1 ;
    unsigned char  no_draw:1 ;
    unsigned char  copy_top:1 ;
    unsigned char  depth:2 ;
    unsigned char  reserved:3 ;
#else
    unsigned char  reserved:3 ;
    unsigned char  depth:2 ;
    unsigned char  copy_top:1 ;
    unsigned char  no_draw:1 ;
    unsigned char  done:1 ;
#endif
    unsigned char  offset ;
    unsigned short pitch ;    /* region pitch */
    unsigned short width ;    /* for end-of-line   detection */
    unsigned short height ;   /* for out-of-region detection */
    unsigned int   lineAddr ;
    unsigned int   blkAddr ;

} DVBSP_TARGET ;

#else

typedef struct
{
    unsigned int dst_pic_idx:2 ;
    unsigned int reserved2:1 ;
    unsigned int dst_y:12 ;
    unsigned int dst_x:12 ;
    unsigned int reserved1:1 ;
    unsigned int opcode:4 ;  /* must be b'1000 */

} DVBSP_SECMD_DSTPIC ;


typedef struct
{
    unsigned int reserved:15 ;
    unsigned int non_modifying_colour_flag:1 ;
    unsigned int pxd_length:16 ;

} DVBSP_SECMD_DECODE_WORD2 ;

#endif


typedef struct
{
    unsigned char   sync_byte ;
    unsigned char   segment_type ;
    unsigned short  page_id ;
    unsigned short  segment_length ;

} DVBSP_SEGMENT_HEADER ;


typedef struct
{
    unsigned char  CLUT_id ;
    unsigned char  version_number ;

    /* CLUTs, RGBA8888 format for each entry */
    unsigned int   colors_2_bit[  4] ;
    unsigned int   colors_4_bit[ 16] ;
    unsigned int   colors_8_bit[256] ;

} DVBSP_CLUT ;


typedef struct
{
#if 0
//#if SP_LITTLE_ENDIAN
    unsigned int horizontal_position:12 ;
    unsigned int provider_flag:2 ;
    unsigned int type:2 ;
    unsigned int object_id:16 ;
    unsigned int background_pixel_code:8 ;
    unsigned int foreground_pixel_code:8 ;
    unsigned int vertical_position:12;
    unsigned int reserved:4 ;
#else
    unsigned int object_id:16 ;
    unsigned int type:2 ;
    unsigned int provider_flag:2 ;
    unsigned int horizontal_position:12 ;
    unsigned int reserved:4 ;
    unsigned int vertical_position:12;
    unsigned int foreground_pixel_code:8 ;
    unsigned int background_pixel_code:8 ;
#endif
} DVBSP_OBJECT_INSTANCE ;


typedef struct
{
#if SP_LITTLE_ENDIAN_SPECIAL
    unsigned int   reserved:3 ;
    unsigned int   fill_flag:1 ;
    unsigned int   version_number:4 ;
    unsigned int   CLUT_id:8 ;
    unsigned int   region_id:8 ;
    //////////////////////////////////////////////////////////////////////////
    unsigned int   height:16 ;
    unsigned int   width:16 ;
    //////////////////////////////////////////////////////////////////////////
    unsigned int   reserved2:2 ;
    unsigned int   _2_bit_pixel_code:2 ;
    unsigned int   _4_bit_pixel_code:4 ;
    unsigned int   _8_bit_pixel_code:8 ;
    unsigned int   reserved1:2 ;
    unsigned int   depth:3 ;
    unsigned int   level_of_compatibility:3 ;
#else
    unsigned int   region_id:8 ;
    unsigned int   CLUT_id:8 ;

    unsigned int   version_number:4 ;
    unsigned int   fill_flag:1 ;
    unsigned int   reserved:3 ;
    //////////////////////////////////////////////////////////////////////////
    unsigned int   width:16 ;
    unsigned int   height:16 ;
    //////////////////////////////////////////////////////////////////////////
    unsigned int   level_of_compatibility:3 ;
    unsigned int   depth:3 ;
    unsigned int   reserved1:2 ;
    unsigned int   _8_bit_pixel_code:8 ;
    unsigned int   _4_bit_pixel_code:4 ;
    unsigned int   _2_bit_pixel_code:2 ;
    unsigned int   reserved2:2 ;
#endif
    unsigned short pitch ;
    unsigned short objects ;  /* number of listed objects in this region */
    unsigned int   pixelBuf ; /* pixel buffer start address */

    /* object instance list */
    DVBSP_OBJECT_INSTANCE  object[DVBSP_MAX_NUM_OBJECTS_PER_REGION] ;

} DVBSP_REGION ;


typedef struct
{
    unsigned char  region_id ;
    unsigned short horizontal_address ;
    unsigned short vertical_address ;

} DVBSP_REGION_INSTANCE ;


typedef struct
{
#if 0
//#if SP_LITTLE_ENDIAN
    unsigned int   reserved:2 ;
    unsigned int   state:2 ;
    unsigned int   version_number:4 ;
    unsigned int   time_out:8 ;
    unsigned int   page_id:16 ;
#else
    unsigned int   page_id:16 ;
    unsigned int   time_out:8 ;
    unsigned int   version_number:4 ;
    unsigned int   state:2 ;
    unsigned int   reserved:2 ;
#endif

    unsigned short regions ;  /* number of listed region instances in this page */
    unsigned short height ;   /* the height of the smallest window enclosing all regions in the page */

    unsigned short display_width ;
    unsigned short display_height ;
    unsigned short display_hor_min ;
    unsigned short display_hor_max ;
    unsigned short display_ver_min ;
    unsigned short display_ver_max ;

    /* region instance list */
    DVBSP_REGION_INSTANCE  region[DVBSP_MAX_NUM_REGIONS_PER_PAGE] ;

} DVBSP_PAGE ;


typedef struct
{
#if 0
//#if SP_LITTLE_ENDIAN
    unsigned char decode:1 ;
    unsigned char svc_acquired:1 ;
    unsigned char reserved:6 ;
#else
    unsigned char reserved:6 ;
    unsigned char svc_acquired:1 ;
    unsigned char decode:1 ;
#endif

} DVBSP_DECODE_STATUS ;


typedef struct
{
    DVBSP_DECODE_STATUS  status ;

    unsigned short pages ;    /* number of active pages   in an epoch */
    unsigned short regions ;  /* number of active regions in an epoch */
    unsigned short CLUTs ;    /* number of active CLUTs   in an epoch */

    /* bitstream pointers */
    unsigned int   bsBase ;
    unsigned int   bsLimit ;
    unsigned int   bsCurr ;
    unsigned int   bsWrite ;
    unsigned long *bsWrPtr ;
    unsigned long *bsRdPtr ;

    int   render_page_id ;

    VP_INBAND_RING_BUF  S_ICQ ;  /* in-band command queue */

    long long  PTS ;


    DVBSP_PAGE     page  [DVBSP_MAX_NUM_PAGES  ] ;
    DVBSP_REGION   region[DVBSP_MAX_NUM_REGIONS] ;
    DVBSP_CLUT     CLUT  [DVBSP_MAX_NUM_CLUTS  ] ;

    /* region/render buffers */
    VP_BUFFER     *renderBuf ;
    VP_BUFFER      renderBufPool[DVBSP_MAX_NUM_RENDER_BUFS] ;
    unsigned int   regionBufOffset ;
#if defined(__LP64__)
    unsigned long   regionBuf ;
    unsigned long   pageBuf ;
#else
    unsigned int   regionBuf ;
    unsigned int   pageBuf ;
#endif

#ifdef DVBSP_SOFTWARE_DECODE
    /* for object decoding */
    unsigned short obj_dec_x_offset ; /* x_offset of the current decoded position */
    unsigned short obj_dec_y_offset ; /* y_offset of the current decoded position */
    unsigned short targets ;

    /* color map-tables */
    unsigned char *colorMap[3][3] ;
    unsigned char  color_map[3*(4+16+256)] ;

    DVBSP_TARGET   target[DVBSP_MAX_NUM_TARGETS] ;

#else

    unsigned int   SE_task_id ;
    long long      bsLinear[DVBSP_LINEAR_BS_BUFFER_SIZE / sizeof(long long)] ;

#endif

    /* phoenix */
    unsigned int   bsCurr_C ;
    SP_INFO *pSpInfo;

} DVBSP_DECODE ;


/* function prototypes */

unsigned int DVBSP_YUVT2RGBA (int, int, int, int) ;

int  DVBSP_Reset                       (DVBSP_DECODE *) ;
int  DVBSP_Init                        (DVBSP_DECODE *, unsigned char) ;
int  DVBSP_Find_Page                   (DVBSP_DECODE *, unsigned int) ;
int  DVBSP_Find_CLUT                   (DVBSP_DECODE *, unsigned int) ;
int  DVBSP_Find_Region                 (DVBSP_DECODE *, unsigned char) ;
int  DVBSP_ResetVE                     (DVBSP_DECODE *, unsigned int) ;
int  DVBSP_SetBitstreamPtr             (DVBSP_DECODE *, unsigned int) ;
int  DVBSP_GetProcessedLength          (DVBSP_DECODE *, unsigned int) ;
int  DVBSP_GetFullness                 (DVBSP_DECODE *) ;
int  DVBSP_Scan_InBandCommand          (DVBSP_DECODE *) ;
int  DVBSP_Decode_Segment              (DVBSP_DECODE *) ;
int  DVBSP_Decode_Page                 (DVBSP_DECODE *, unsigned int, unsigned int) ;
int  DVBSP_Decode_Region               (DVBSP_DECODE *, unsigned int, unsigned int) ;
int  DVBSP_Decode_CLUT                 (DVBSP_DECODE *, unsigned int, unsigned int) ;
int  DVBSP_Decode_Object               (DVBSP_DECODE *, unsigned int, unsigned int) ;
int  DVBSP_Decode_DisplayWin           (DVBSP_DECODE *, unsigned int, unsigned int) ;
int  DVBSP_Render_Region               (DVBSP_DECODE *, unsigned int, DVBSP_REGION *) ;

#ifdef DVBSP_SOFTWARE_DECODE
int  DVBSP_Decode_Pixel_Data           (DVBSP_DECODE *) ;
int  DVBSP_Decode_PixelCodeString_2bit (DVBSP_DECODE *) ;
int  DVBSP_Decode_PixelCodeString_4bit (DVBSP_DECODE *) ;
int  DVBSP_Decode_PixelCodeString_8bit (DVBSP_DECODE *) ;
int  DVBSP_Draw_Pixel                  (DVBSP_DECODE *, unsigned char, int, int) ;
#endif

int DVBSP_Decoder_Create (SP_INFO *, VP_COMPONENT *, int) ;
void DVBSP_SwapPacketToVO(VIDEO_VO_OBJ_DVB_SP_REGION* regionObj, int regionIdx, int last_in_page);

#endif
