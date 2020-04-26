
#ifndef __DTV_PARAM_H__
#define __DTV_PARAM_H__

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

#include <stdint.h>

#define DTV_SIGNATURE           "RTKDTV PARAM"
#define DTV_SIGNATURE_LENGTH    (sizeof(DTV_SIGNATURE)- 1)
#define DTV_FILE_DEFAULT_DVB_T  "dvb_t.default"

#define DTV_PARAM_VERSION       (0x00000000)

// one nibble
#define DTV_SYSTEM_MAJOR_DVB    (0x0)
#define DTV_SYSTEM_MAJOR_ATSC   (0x1)
#define DTV_SYSTEM_MAJOR_ISDB   (0x2)

// one nibble
#define DTV_SYSTEM_MINOR_C      (0x0)
#define DTV_SYSTEM_MINOR_H      (0x1)
#define DTV_SYSTEM_MINOR_S      (0x2)
#define DTV_SYSTEM_MINOR_S2     (0x3)
#define DTV_SYSTEM_MINOR_T      (0x4)

#define DTV_MAKE_SYSTEM_ID(major, minor)        (((major)<< 4) | (minor))

typedef struct _dtv_param_table_header_t{
    char        dtv_signature[DTV_SIGNATURE_LENGTH];
    uint32_t    dtv_version_number              : 32;
    uint32_t    dtv_system_id_major             :  4;
    uint32_t    dtv_system_id_minor             :  4;
    uint32_t    reserved                        : 24;
    uint32_t    loop_length                     : 32;
} dtv_param_table_header_t;

typedef struct _dtv_dvb_loop_header_t{
    uint32_t    dtv_dvb_frequency_start         : 32;
    uint32_t    dtv_dvb_frequency_end           : 32;
    uint32_t    dtv_dvb_bandwidth               :  8;
    uint32_t    reserved                        : 24;
} dtv_dvb_loop_header_t;

typedef struct _dtv_atsc_loop_header_t{
    uint32_t    dtv_atsc_channel_id             : 32;
    uint32_t    dtv_atsc_frequency              : 32;
} dtv_atsc_loop_header_t;

int
dtv_param_file_valid(
    const char*                 p_filename);

int
dtv_param_file_read(
    char*                       p_param,
    int*                        p_param_length,
    const char*                 p_filename);

int
dtv_param_dump(
    char*                       p_param,
    int                         param_length);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // #ifndef __DTV_PARAM_H__
