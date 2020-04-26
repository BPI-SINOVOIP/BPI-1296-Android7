#ifndef __MPEG_PES_H__
#define __MPEG_PES_H__

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

#include <limits.h>

#define NAV_MAX_SEGMENT                 (512)

// ======================================================================================
// stream descriptor
// format: pppppppprrrrrrrrrrrrrrccffffffff
//         (1) p: media content (P)rovider
//         (2) r: (R)eserved for user define
//         (3) c: stream (C)ategories
//         (4) f: media specific stream (F)ormat
// ======================================================================================
#define MPEG_PES_DESC_NA                INT_MAX

// media content (P)rovider
#define MPEG_PES_PROVIDER_DVB_MASK      0x01000000

// stream (C)ategories
// (1) 0: VIDEO
// (2) 1: AUDIO
// (3) 2: SPU
// (4) 3: N/A
#define MPEG_PES_CATEGORY_VIDEO         0x00000000
#define MPEG_PES_CATEGORY_AUDIO         0x00000100
#define MPEG_PES_CATEGORY_SPU           0x00000200
#define MPEG_PES_CATEGORY_NA            0x00000300

#define MPEG_PES_INDEX_VIDEO            (MPEG_PES_CATEGORY_VIDEO>> 8)
#define MPEG_PES_INDEX_AUDIO            (MPEG_PES_CATEGORY_AUDIO>> 8)
#define MPEG_PES_INDEX_SPU              (MPEG_PES_CATEGORY_SPU>> 8)
#define MPEG_PES_INDEX_NA               (MPEG_PES_CATEGORY_NA>> 8)

// media specific stream (F)ormat
// #define MPEG_PES_VIDEO_AUTO             0x00            /* automatically detect video
                                                           // format by pes stream id */
#define MPEG_PES_VIDEO_11172            0x01            /* ISO/IEC 11172 Video */
#define MPEG_PES_VIDEO_13818            0x02            /* ISO/IEC 13818-2 Video */
#define MPEG_PES_VIDEO_NA               0xFF            /* Video format is not available */

// #define MPEG_PES_AUDIO_AUTO             0x20            /* automatically detect audio
                                                           // format by pes stream id */
#define MPEG_PES_AUDIO_11172            0x21            /* ISO/IEC 11172 Audio */
#define MPEG_PES_AUDIO_13818            0x22            /* ISO/IEC 13818-3 Audio */
#define MPEG_PES_AUDIO_AC3              0x23            /* AC3 */
#define MPEG_PES_AUDIO_LPCM             0x24            /* LPCM/PCM */
#define MPEG_PES_AUDIO_AAC              0x25            /* AAC */
#define MPEG_PES_AUDIO_DTS              0x26            /* DTS */
#define MPEG_PES_AUDIO_NA               0xFF            /* Video format is not available */

// #define MPEG_PES_SPU_AUTO               0x40            /* automatically detect subtitle
                                                           // format by pes stream id */
#define MPEG_PES_SPU_SUBTITLE           0x41            /* dvb subtitle */
#define MPEG_PES_SPU_NA                 0xFF            /* Subtitle format is not available */

#define MPEG_PES_FMT_NA                 0xFE
#define MPEG_PES_FMT_AUTO               0xFF

// ======================================================================================
//                          Some constant definition                         
// ======================================================================================
#define NAV_MAX_SEGMENT                 (512)

#ifdef NAV_MAX_SEGMENT
#define MAX_PES_BUFFER_NUMBER           NAV_MAX_SEGMENT
#else
#define MAX_PES_BUFFER_NUMBER           256
#endif

// ======================================================================================
//                          Some useful macro definition                         
// ======================================================================================
#define MPEG_PES_CATEGORY_GET(stream_descriptor)            \
    ((MPEG_PES_CATEGORY_NA & (stream_descriptor)))

#define MPEG_PES_INDEX_GET(stream_descriptor)            \
    ((MPEG_PES_CATEGORY_NA & (stream_descriptor))>> 8)

#define MPEG_PES_STREAM_FMT(stream_desc)        (0xff & (stream_desc))

#define MPEG_PES_CMD_NEW_SEG            0x00000001

typedef struct{
    BYTE*               p_pes_data;
    int                 len_pes_data;
    uint32_t            stream_descriptor;
    uint32_t            b_pes_header;
} pes_buf_t;

typedef struct{
    unsigned int        demux_cmd;
    pes_buf_t           p_pes_buf_array[MAX_PES_BUFFER_NUMBER];
    int                 n_pes_buf;
} pes_buf_list_t;

// ======================================================================================
// some useful macros         
// ======================================================================================
#define MAX(a, b)       (a)> (b)? (a): (b)
#define MIN(a, b)       (a)< (b)? (a): (b)

int
mpeg_pes_pts(
    int64_t*                    p_pts,
    unsigned char*              p_pes,
    int                         len_pes);

int
mpeg_pes_stream_desc(
    uint32_t*                   p_stream_desc,
    unsigned char*              p_pes,
    int                         len_pes);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // #ifndef __MPEG_PES_H__ 
