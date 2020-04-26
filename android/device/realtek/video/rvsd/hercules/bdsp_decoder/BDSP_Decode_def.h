/*********************************************************************************/
/*                                                                               */
/* BDSP_Decode_def.h                                                            */
/*                                                                               */
/* Purpose: BD subtitling decoder constant macro definitions                    */
/*                                                                               */
/*                                                                               */
/*                                                                               */
/* Copyright (c) 2009 Realtek Semiconductor Corp. All rights reserved.           */
/*                                                                               */
/* No part of this document may be copied, reproduced,                           */
/* or transmitted in any form without the prior written                          */
/* consent of Realtek Semiconductor Corp. Hsinchu, Taiwan                        */
/*                                                                               */
/* Revision History:                                                             */
/*                                                                               */
/*      Date               By                  Description                       */
/* ============================================================================= */
/*    05/07/09         Bonds Lu                Creation                          */
/*                                                                               */
/*                                                                               */
/*                                                                               */
/*********************************************************************************/
#ifndef _BDSP_DECODE_DEF_H_
#define _BDSP_DECODE_DEF_H_

/****************
** debug usage **
****************/
//#define BDSP_DBG_DUMP_FRAME_START
//#define BDSP_PROFILE_EN
//#define BDSP_DBG_DUMP_ICQ
//#define BDSP_DBG_DUMP_PIC_EN   // debug: dump picture, Android.mk: RVSD_DBG_DUMP_BS = true

/****************
** main control *
****************/
//#define BDSP_REMOVE_BIT_FIELD
//#define BDSP_USING_ANOTHER_RLE    // exist bug
#define BDSP_USING_FFMPEG_RLE
//#define BDSP_ENABLE_4K
#define BDSP_SYNC_PTS_NEW
//#define WORKAROUND_DHCFAE1915
#define BDSP_MALLOC_RENDER_BUFFER_DYNMAIC
#define BDSP_USING_CACHE
//#define BDSP_USING_SECOPY   // in render, NO Good
#define BDSP_RLE_GET_BYTE

#define BDSP_SOFTWARE_DECODE
#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif

#ifdef BDSP_DBG_DUMP_PIC_EN
#define BDSP_DBG_DUMP_PIC(x) BDSP_dbg_dump_pic(x)
#else
#define BDSP_DBG_DUMP_PIC(x) 
#endif

#define BDSP_PROFILE_LOG SP_LOG

#define BDSP_MAX_NUM_RENDER_BUFS              3
#define BDSP_MAX_NUM_PAGE                     8  /* ??? */
#define BDSP_MAX_NUM_WINDOWS                  2
#define BDSP_MAX_NUM_CLUTS                    8
#define BDSP_MAX_NUM_OBJECTS                 64
#define BDSP_MAX_NUM_OBJECTS_PER_PAGE         2

#define BDSP_DEC_RING_BUF_MIN_EMPTINESS     0x4000

#define BDSP_DEC_SUCCESS   0

/* decode status */
#define BDSP_DEC_STATUS_BITSTREAM_UNDERFLOW        1

/* decode render status */
#define BDSP_DEC_RENDER_STATUS_NONE                0
#define BDSP_DEC_RENDER_STATUS_START               1
#define BDSP_DEC_RENDER_STATUS_BITBLIT             2


/* decode errors */
#define BDSP_ERR_UNKNOWN_WINDOW                   -1
#define BDSP_ERR_UNKNOWN_OBJECT                   -2
#define BDSP_ERR_UNKNOWN_CLUT                     -3
#define BDSP_ERR_UNDEFINED_PAGE_STATE             -4
#define BDSP_ERR_INVALID_OBJECT_FIELD             -5
#define BDSP_ERR_INVALID_WINDOW_FIELD             -6
#define BDSP_ERR_INVALID_PAGE_FIELD               -7
#define BDSP_ERR_INVALID_CLUT_ENTRY               -8
#define BDSP_ERR_INVALID_RENDER_PAGE              -9
#define BDSP_ERR_OBJECT_OVERFLOW                  -10
#define BDSP_ERR_WINDOW_OVERFLOW                  -11
#define BDSP_ERR_PIXEL_BUF_OVERFLOW               -12
#define BDSP_ERR_CLUT_OVERFLOW                    -13
#define BDSP_ERR_SE_TASK_BUFFER_OVERFLOW          -14

#define BDSP_DEC_MEM_SIZE (1024 * 12)

#define BDSP_RENDER_FRAME_NUM 3
#define BDSP_1920x1088 (0x200000)
#define BDSP_4096x2160 (0x872000)   // more 8KB

#define BDSP_MAX_PICTURE_WIDTH_1920                  1920
#define BDSP_MAX_PICTURE_HEIGHT_1088                 1088
#define BDSP_MAX_PICTURE_WIDTH_4096                  4096
#define BDSP_MAX_PICTURE_HEIGHT_2160                 2160

#ifdef BDSP_ENABLE_4K
#define BDSP_MAX_PICTURE_WIDTH                  4096
#define BDSP_MAX_PICTURE_HEIGHT                 2160
#define BDSP_RENDER_BUFFER_SIZE               (BDSP_MAX_PICTURE_WIDTH  * BDSP_MAX_PICTURE_HEIGHT)
#define VMM_SIZE_BD_SUBTITLE (BDSP_4096x2160 * BDSP_RENDER_FRAME_NUM)
#else
#define BDSP_MAX_PICTURE_WIDTH                  1920
#define BDSP_MAX_PICTURE_HEIGHT                 1088
#define BDSP_RENDER_BUFFER_SIZE               (BDSP_MAX_PICTURE_WIDTH  * BDSP_MAX_PICTURE_HEIGHT)
#define VMM_SIZE_BD_SUBTITLE (BDSP_1920x1088 * BDSP_RENDER_FRAME_NUM)
#endif

#define BDSP_TOTAL_OBJECT_BUFFER_SIZE       0x400000   // 3M  for OBJECT_BUFFER, 1M for bsLinear
#define BDSP_MAX_OBJECT_BUFFER_SIZE         0x300000

#if defined(BDSP_MALLOC_RENDER_BUFFER_DYNMAIC)
#define ION_BD_SIZE (BDSP_DEC_MEM_SIZE + BDSP_TOTAL_OBJECT_BUFFER_SIZE)
#else
//#define ION_BD_SIZE (10 * 1024 * 1024)  // 3pic
#define ION_BD_SIZE (BDSP_DEC_MEM_SIZE + BDSP_TOTAL_OBJECT_BUFFER_SIZE + VMM_SIZE_BD_SUBTITLE)  // 3pic
#endif

/* Segment type (Blue-ray Disc Read-Only Format Table 9-60) */
#define BDSP_SEGMENT_PALETTE_DEFINITION       0x14
#define BDSP_SEGMENT_OBJECT_DEFINITION        0x15
#define BDSP_SEGMENT_PRESENTATION_COMPOSITION 0x16
#define BDSP_SEGMENT_WINDOW_DEFINITION        0x17
#define BDSP_SEGMENT_END_OF_DISPLAY           0x80

/* Composition state (Blue-ray Disc Read-Only Format Table 9-62) */
#define BDSP_COMPOSITION_STATE_NORMAL_CASE        0
#define BDSP_COMPOSITION_STATE_ACQUISITION_POINT  1
#define BDSP_COMPOSITION_STATE_EPOCH_START        2
#define BDSP_COMPOSITION_STATE_EPOCH_CONTINUE     3

/* check range: start inclusive, end exclusive */
#define S_oE(x, start, end) \
(   ((end) == (start) &&  ((x) == (start))) \
 || ((end)  > (start) &&  ((x) >= (start) && (x)  < (end)))  \
 || ((end)  < (start) && !((x)  < (start) && (x) >= (end))) \
)


/* size of bitstream wrap-around copy buffer */
#define BDSP_LINEAR_BS_BUFFER_SIZE    (5 * 64 * 1024)

#ifdef BDSP_PROFILE_EN
#define BDSP_PROFILE_BF(tag) gProfile.start_pts[tag] = getPTS();
#define BDSP_PROFILE_AF(tag, tag_str) BDSP_PROFILE_LOG("%s, %d ms\n", tag_str \
    , ((int)(getPTS() - gProfile.start_pts[tag]))/90);

#else
#define BDSP_PROFILE_BF(tag) 
#define BDSP_PROFILE_AF(tag, tag_str) 
#endif

#endif
