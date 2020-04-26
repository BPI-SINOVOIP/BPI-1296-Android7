/*********************************************************************************/
/*                                                                               */
/* DVBSP_Decode_def.h                                                            */
/*                                                                               */
/* Purpose: DVB subtitling decoder constant macro definitions                    */
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
#ifndef _DVBSP_DECODE_DEF_H_
#define _DVBSP_DECODE_DEF_H_

#define DVBSP_SOFTWARE_DECODE

#define DVBSP_H_NAN  0xffff

#define DVBSP_PICTURE_WIDTH   1920

#define DVBSP_MAX_TASK_LIFE_SPAN         5000000
#define DVBSP_MAX_NUM_RENDER_BUFS              2
#define DVBSP_MAX_NUM_REGIONS                 64
#define DVBSP_MAX_NUM_CLUTS                   16
#define DVBSP_MAX_NUM_PAGES                    8
#define DVBSP_MAX_NUM_REGIONS_PER_PAGE        32
#define DVBSP_MAX_NUM_OBJECTS_PER_REGION     128

#define DVBSP_DEC_RING_BUF_MIN_EMPTINESS    0x4000

#define DVBSP_DEC_SUCCESS   0

/* decode status */
#define DVBSP_DEC_STATUS_BITSTREAM_UNDERFLOW        1
#define DVBSP_DEC_STATUS_PIXEL_BUFFER               2

/* decode errors */
#define DVBSP_ERR_UNKNOWN_PAGE                     -1
#define DVBSP_ERR_UNKNOWN_REGION                   -2
#define DVBSP_ERR_UNKNOWN_OBJECT                   -3
#define DVBSP_ERR_UNKNOWN_CLUT                     -4
#define DVBSP_ERR_UNDEFINED_REGION_DEPTH           -5
#define DVBSP_ERR_UNDEFINED_PAGE_STATE             -6
#define DVBSP_ERR_INVALID_CLUT_ENTRY               -7
#define DVBSP_ERR_PAGE_OVERFLOW                    -8
#define DVBSP_ERR_REGION_OVERFLOW                  -9
#define DVBSP_ERR_PIXEL_BUF_OVERFLOW               -10
#define DVBSP_ERR_CLUT_OVERFLOW                    -11
#define DVBSP_ERR_UNSUPPORTED_OBJECT_CODING_METHOD -12
#define DVBSP_ERR_SE_TASK_BUFFER_OVERFLOW          -13
#define DVBSP_ERR_INVALID_DATA_TYPE                -14

/* pixel buffer size specified in the DVB subtitle decoder model (ETSI EN 300 743 v1.3.1 sec 5) */
#define DVBSP_REGION_BUFFER_SIZE       (320 * 1024)

/* segment sync byte */
#define DVBSP_SEGMENT_SYNC_BYTE   0x0f

/* segment type (ETSI EN 300 743 v1.2.1 Table 2) */
#define DVBSP_SEGMENT_PAGE_COMPOSITION        0x10
#define DVBSP_SEGMENT_REGION_COMPOSITION      0x11
#define DVBSP_SEGMENT_CLUT_DEFINITION         0x12
#define DVBSP_SEGMENT_OBJECT_DATA             0x13
#define DVBSP_SEGMENT_DISPLAY_DEFINITION      0x14
#define DVBSP_SEGMENT_END_OF_DISPLAY_SET      0x80
#define DVBSP_SEGMENT_STUFFING                0xff

/* page state (ETSI EN 300 743 v1.2.1 Table 3) */
#define DVBSP_PAGE_STATE_NORMAL_CASE           0
#define DVBSP_PAGE_STATE_ACQUISITION_POINT     1
#define DVBSP_PAGE_STATE_MODE_CHANGE           2

/* region pixel color depth (ETSI EN 300 743 v1.2.1 Table 5) */
#define DVBSP_PIXEL_DEPTH_2_BIT                1
#define DVBSP_PIXEL_DEPTH_4_BIT                2
#define DVBSP_PIXEL_DEPTH_8_BIT                3

/* object type (ETSI EN 300 743 v1.2.1 Table 6) */
#define DVBSP_OBJECT_TYPE_BITMAP               0
#define DVBSP_OBJECT_TYPE_CHARACTER            1
#define DVBSP_OBJECT_TYPE_CHARACTER_STRING     2

/* object provider (ETSI EN 300 743 v1.2.1 Table 7) */
#define DVBSP_OBJECT_PROVIDER_SP_STREAM        0
#define DVBSP_OBJECT_PROVIDER_ROM_IN_IRD       1

/* object coding method (ETSI EN 300 743 v1.2.1 Table 8) */
#define DVBSP_OBJECT_CODING_PIXEL              0x00
#define DVBSP_OBJECT_CODING_CHARACTER_STRING   0x01

/* check range: start inclusive, end exclusive */
#define S_oE(x, start, end) \
(   ((end) == (start) &&  ((x) == (start))) \
 || ((end)  > (start) &&  ((x) >= (start) && (x)  < (end)))  \
 || ((end)  < (start) && !((x)  < (start) && (x) >= (end))) \
)


#ifdef DVBSP_SOFTWARE_DECODE

#define DVBSP_MAX_NUM_TARGETS                256

/* pixel-data sub-block data_type (ETSI EN 300 743 v1.2.1 Table 9) */
#define DVBSP_PIXEL_TYPE_2_BIT                 0x10
#define DVBSP_PIXEL_TYPE_4_BIT                 0x11
#define DVBSP_PIXEL_TYPE_8_BIT                 0x12
#define DVBSP_PIXEL_TYPE_2_TO_4_BIT_MAP        0x20
#define DVBSP_PIXEL_TYPE_2_TO_8_BIT_MAP        0x21
#define DVBSP_PIXEL_TYPE_4_TO_8_BIT_MAP        0x22
#define DVBSP_PIXEL_TYPE_END_OF_LINE           0xF0

#else

/* size of bitstream wrap-around copy buffer */
#define DVBSP_LINEAR_BS_BUFFER_SIZE    (32  * 1024)

#endif

#endif
