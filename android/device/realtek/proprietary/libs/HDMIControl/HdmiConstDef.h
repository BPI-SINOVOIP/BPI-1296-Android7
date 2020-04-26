#ifndef _HDMI_CONST_DEF_H_
#define _HDMI_CONST_DEF_H_

#include <stdlib.h>
#include <unistd.h>

#define ID_NAME_PARE(x) {x,#x}

#define EVENT_PLUG_IN                   (1)
#define EVENT_PLUG_OUT                  (0)

#define NO_OUTPUT_MODE                  (0)
#define DVI_OUTPUT_MODE                 (1)
#define HDMI_OUTPUT_MODE                (2)

#define EDID_DATA_LENGTH                (256)


// some mask define
#define HDMI_FREQ_SHIFT_MASK            (1<<0)
#define HDMI_3D_FREQ_SHIFT_MASK         (1<<1)

#define DEPTH_8BIT_MASK                 (1<<1)
#define DEPTH_10BIT_MASK                (1<<2)
#define DEPTH_12BIT_MASK                (1<<3)

#define COLOR_NONE                      (0xff)
#define COLOR_RGB444                    (   0)
#define COLOR_YUV422                    (   1)
#define COLOR_YUV444                    (   2)
#define COLOR_YUV420                    (   3)
#define COLOR_ANY                       (0xaa)

#define DEPTH_NONE                      (0xff)
#define DEPTH_8BIT                      (   8)
#define DEPTH_10BIT                     (  10)
#define DEPTH_12BIT                     (  12)

// sync with RtkVoutUtilManager.java
#define _3D_NULL                        (-1)
#define _3D_AUTO                        ( 0)
#define _3D_SIDE_BY_SIDE                ( 2)
#define _3D_TOP_BUTTON                  ( 3)

#define _3D_SUPPORT_MAEK                (1<<0)
#define _3D_FRAME_PACKING_MASK          (1<<1)
#define _3D_TOP_BOTTOM_MASK             (1<<2)
#define _3D_SIDE_BY_SIDE_MASK           (1<<3)

// used for getColorSet2, but is disabled now
#define GEN_COLOR_RST(color,depth)      ((color<<8)|depth)
#define GET_COLOR(color_rst)            (color_rst>>8)
#define GET_COLOR_DEPTH(color_rst)      (color_rst&0xff)

#define SUPPORT_DEPTH_8BIT(x)           ((x & DEPTH_8BIT_MASK))
#define SUPPORT_DEPTH_10BIT(x)          ((x & DEPTH_10BIT_MASK))
#define SUPPORT_DEPTH_12BIT(x)          ((x & DEPTH_12BIT_MASK))

#define EXTRA_SAVE_TO_FACTORY           (1<<0)      // save the result to factory
#define EXTRA_FORCE_OUTPUT_FORMAT       (1<<1)      // do not check the relationship of HDR mode , DV mode
#define EXTRA_IGNORE_CURRENT            (1<<2)      // set tv system and ignore if new output format is the same with current setting
#define EXTRA_USE_NOW_COLOR_IF_SDR      (1<<3)      // if TV is SDR TV, then do not change color mode
#define EXTRA_RESET_COLOR_MODE          (1<<4)      // reset color mode this time
#define EXTRA_RESET_TV_SYSTEM           (1<<5)      // reset TV system this time
#define EXTRA_TRY_TO_USE_CURRENT_COLOR  (1<<6)      // RtkHDMIManager2 set TV system, try to use current color mode

#define COLOR_POLICY_AUTO               (1)         // choice 8bit color mode only
#define COLOR_POLICY_DEEP_AUTO          (2)         // choice 12bit, 10bit color mode first.

// sync to RtkTVSystem.java
#define TV_SYSTEM_AUTO_MODE_OFF         (0)
#define TV_SYSTEM_AUTO_MODE_HDMI_EDID   (2)

#define TV_SYS_DEEPCOLOR_AUTO_DETECT    (  11)  // sync with RtkTVSystem
#define TV_SYS_COLORSPACE_AUTO_DETECT   (   0)
#define TV_SYS_COLOR_USE_DEFAULT        (0xab)  // told service reset to default value (by deep color property).
//#define TV_SYS_COLOR_MODE_MANUAL      (0xff)  // set in Service internally

// HDCP type def
#define HDCP_14                         (14)
#define HDCP_22                         (22)
#define HDCP_NONE                       ( 0)

#define HDCP22_STATE_ENABLE             ( 0)
#define HDCP22_STATE_DISABLE            ( 1)
#define HDCP22_STATE_DISABLE_MANUAL     ( 2)
#define HDCP22_STATE_RESET_HDMI         ( 3)

// HDCP event
#define HDCP_EVENT_KEY_ERROR            (1<<0)
#define HDCP_EVENT_NEGO_ERROR           (1<<1)
#define HDCP_EVENT_SUCCESS              (1<<2)
#define HDCP_EVENT_RESET_HDMI           (1<<3)

// HDCP 1.4 driver sysfs state
#define HDCP_STATE_DISABLED                 0
#define HDCP_STATE_INIT                     1
#define HDCP_STATE_AUTH_1ST_STEP            2
#define HDCP_STATE_AUTH_2ND_STEP            3
#define HDCP_STATE_AUTH_3RD_STEP            4
#define HDCP_STATE_AUTH_FAIL_RESTARTING     5
#define HDCP_STATE_AUTH_FAILURE             6

#define HDMITX_STATE                    "/sys/devices/virtual/switch/hdmi/state"
#define HDCP_1X_EN_SYSFS                "/sys/devices/virtual/misc/hdcp/hdcp1x_en"
#define HDCP_1X_STATE_SYSFS             "/sys/devices/virtual/misc/hdcp/state"

#define HDCP_1X_KEY_PATH                "/tmp/_custom_param_h13"
#define HDCP_22_KEY_PATH                "/tmp/_custom_param_tx"

// property to query hdmi service status
#define PROPERTY_HDMI_START             "rtk.hdmi.service.start"
#define PROPERTY_DUAL_DISPLAY           "persist.rtk.hdmitx.dpdiffsource"

#define DUAL_DISPLAY_ON                 "on"
#define DUAL_DISPLAY_OFF                "off"

#define HIDL_OUTPUT_FORMAT_ASSIGN(x,y)      \
    {                                       \
        x.mode          = y.mode;           \
        x.vic           = y.vic;            \
        x.freq_shift    = y.freq_shift;     \
        x.color         = y.color;          \
        x.color_depth   = y.color_depth;    \
        x._3d_format    = y._3d_format;     \
        x.hdr           = y.hdr;            \
        x.reserved      = y.reserved;       \
    }

#define HIDL_OUTPUT_FORMAT_ASSIGN_PT(px,y)          \
    {                                               \
        px->mode            = y.mode;               \
        px->vic             = y.vic;                \
        px->freq_shift      = y.freq_shift;         \
        px->color           = y.color;              \
        px->color_depth     = y.color_depth;        \
        px->_3d_format      = y._3d_format;         \
        px->hdr             = y.hdr;                \
        px->reserved        = y.reserved;           \
    }

typedef uint8_t COLOR_TYPE;

// VIC index defined in HDMI spec
typedef enum {
    VIC_720_480P_60HZ       =   2,
    VIC_1280_720P_60HZ      =   4,
    VIC_1920_1080I_60HZ     =   5,
    VIC_720_480I_60HZ       =   6,      // NTSC
    VIC_1920_1080P_60HZ     =  16,
    VIC_720_576P_50HZ       =  17,
    VIC_1280_720P_50HZ      =  19,

    VIC_1920_1080I_50HZ     =  20,
    VIC_720_576I_50HZ       =  21,      // PAL
    VIC_1920_1080P_50HZ     =  31,
    VIC_1920_1080P_24HZ     =  32,
    VIC_1920_1080P_25HZ     =  33,
    VIC_1920_1080P_30HZ     =  34,

    VIC_3840_2160P_24HZ     =  93,
    VIC_3840_2160P_25HZ     =  94,
    VIC_3840_2160P_30HZ     =  95,
    VIC_3840_2160P_50HZ     =  96,
    VIC_3840_2160P_60HZ     =  97,

    VIC_4096_2160P_24HZ     =  98,
    VIC_4096_2160P_25HZ     =  99,
    VIC_4096_2160P_30HZ     = 100,
    VIC_4096_2160P_50HZ     = 101,
    VIC_4096_2160P_60HZ     = 102,
} HDMI_VIC_ENUM;

typedef struct _VIC_INFO {
    HDMI_VIC_ENUM   id;
    const char *    name;
} VIC_INFO;

static VIC_INFO sVICInfo[] = {
    ID_NAME_PARE(VIC_720_480P_60HZ),
    ID_NAME_PARE(VIC_1280_720P_60HZ),
    ID_NAME_PARE(VIC_1920_1080I_60HZ),
    ID_NAME_PARE(VIC_720_480I_60HZ),
    ID_NAME_PARE(VIC_1920_1080P_60HZ),
    ID_NAME_PARE(VIC_720_576P_50HZ),
    ID_NAME_PARE(VIC_1280_720P_50HZ),

    ID_NAME_PARE(VIC_1920_1080I_50HZ),
    ID_NAME_PARE(VIC_720_576I_50HZ),
    ID_NAME_PARE(VIC_1920_1080P_50HZ),
    ID_NAME_PARE(VIC_1920_1080P_24HZ),
    ID_NAME_PARE(VIC_1920_1080P_25HZ),
    ID_NAME_PARE(VIC_1920_1080P_30HZ),

    ID_NAME_PARE(VIC_3840_2160P_24HZ),
    ID_NAME_PARE(VIC_3840_2160P_25HZ),
    ID_NAME_PARE(VIC_3840_2160P_30HZ),
    ID_NAME_PARE(VIC_3840_2160P_50HZ),
    ID_NAME_PARE(VIC_3840_2160P_60HZ),

    ID_NAME_PARE(VIC_4096_2160P_24HZ),
    ID_NAME_PARE(VIC_4096_2160P_25HZ),
    ID_NAME_PARE(VIC_4096_2160P_30HZ),
    ID_NAME_PARE(VIC_4096_2160P_50HZ),
    ID_NAME_PARE(VIC_4096_2160P_60HZ),
};

typedef enum _HDMI_HDR_MODE {
    HDR_MODE_AUTO = 0, //HDR_MODE_OFF = 0,
    /* Dolby Vision */
    HDR_MODE_DV = 1,
    HDR_MODE_SDR = 2,
    HDR_MODE_GAMMA =3,
    /* HDR10 */
    HDR_MODE_HDR10 = 4,
    /* reserved future */
    HDR_MODE_FUTURE = 5,
    /* SDR/HDR by video */
    HDR_MODE_INPUT = 6,

    // DOVI Extension
    HDR_MODE_DV_LOW_LATENCY_12b_YUV422 = 7,
    HDR_MODE_DV_LOW_LATENCY_10b_YUV444 = 8,
    HDR_MODE_DV_LOW_LATENCY_10b_RGB444 = 9,
    HDR_MODE_DV_LOW_LATENCY_12b_YUV444 = 10,
    HDR_MODE_DV_LOW_LATENCY_12b_RGB444 = 11,
    HDR_MODE_DV_ON_INPUT = 12,
    HDR_MODE_DV_ON_LOW_LATENCY_12b422_INPUT = 13,

    HDR_MODE_CURRENT = 0xf0,
    HDR_MODE_FACTORY = 0xf1,
    HDR_MODE_NA = 0xf2,
} HDMI_HDR_MODE;

enum HDMI_3D_MODE {
    FORMAT_3D_OFF = 0,
    /* Frame Packing */
    FORMAT_3D_FP = 1,
    /* Side by side half */
    FORMAT_3D_SS = 2,
    /* Top and Buttom */
    FORMAT_3D_TB = 3,
};

typedef struct _HDMI_OUTPUT_FORMAT {
    uint8_t mode;
    uint8_t vic;
    uint8_t freq_shift;
    uint8_t color;
    uint8_t color_depth;
    uint8_t _3d_format;
    uint8_t hdr;
    uint8_t reserved;
} HDMI_OUTPUT_FORMAT;

typedef struct _HDMI_FORMAT_SUPPORT {
    uint8_t mVic;
    uint8_t mRgb444;
	uint8_t mYuv422;
	uint8_t mYuv444;
	uint8_t mYuv420;
	uint8_t mSupport_3d;
	uint8_t mSupport_fs;
    uint8_t mReserved;

    _HDMI_FORMAT_SUPPORT()
    : mVic(0),
      mRgb444(0),
      mYuv422(0),
      mYuv444(0),
      mYuv420(0),
      mSupport_3d(0),
      mSupport_fs(0),
      mReserved(0)
    {}

    _HDMI_FORMAT_SUPPORT(
        uint8_t vic,
        uint8_t rgb444,
        uint8_t yuv422,
        uint8_t yuv444,
        uint8_t yuv420,
        uint8_t support_3d,
        uint8_t support_fs,
        uint8_t reserved)
    : mVic(vic),
      mRgb444(rgb444),
      mYuv422(yuv422),
      mYuv444(yuv444),
      mYuv420(yuv420),
      mSupport_3d(support_3d),
      mSupport_fs(support_fs),
      mReserved(reserved)
    {}

} HDMI_FORMAT_SUPPORT;

typedef struct _HDMITVCapInfo {
    int32_t mConnected;
    int32_t mEotf;
    int32_t mDV;
    int32_t mMetaDataLen;
    int32_t mMaxLuminance;
    int32_t mAvgLuminance;
    int32_t mMinLuminance;
} HDMITVCapInfo;

// display mode define
enum INTERFACE_TYPE {
	ANALOG_AND_DIGITAL = 0,
	ANALOG_ONLY = 1,
	DIGITAL_ONLY = 2,
	DISPLAY_PORT_ONLY = 3,
	HDMI_AND_DISPLAY_PORT_SAME_SOURCE = 4,
	HDMI_AND_DISPLAY_PORT_DIFF_SOURCE = 5,
	DISPLAY_PORT_AND_CVBS_SAME_SOURCE = 6,
	HDMI_AND_DP_DIFF_SOURCE_WITH_CVBS = 7,
    // defined but not use
	FORCE_DP_OFF = 8,
};

typedef enum INTERFACE_TYPE INTERFACE_TYPE;

// DP related
typedef struct _INTERFACE_TYPE_MAP {
    INTERFACE_TYPE mType;
    bool mHDMI;
    bool mDP;
    bool mDiffSource;
} INTERFACE_TYPE_MAP;

static INTERFACE_TYPE_MAP sInterfaceTypeMap[] = {
    {HDMI_AND_DISPLAY_PORT_DIFF_SOURCE,true,true,true},
    {HDMI_AND_DISPLAY_PORT_SAME_SOURCE,true,true,false},
    {ANALOG_AND_DIGITAL,true,false,true},
    {ANALOG_AND_DIGITAL,true,false,false},
    {DISPLAY_PORT_ONLY,false,true,true},
    {DISPLAY_PORT_ONLY,false,true,false},
    {ANALOG_AND_DIGITAL,false,false,true},
    {ANALOG_AND_DIGITAL,false,false,false},
};

static inline const char * getColorSpaceString(int32_t color)
{
    switch(color) {
    case COLOR_RGB444:
        return "RGB444";
    case COLOR_YUV422:
        return "YUV422";
    case COLOR_YUV444:
        return "YUV444";
    case COLOR_YUV420:
        return "YUV420";
    case COLOR_NONE:
        return "None";
    case COLOR_ANY:
        return "Any";
    default:
        return "COLOR_UNKNOWN";
    }
}

static inline const char * get3DModeString(enum HDMI_3D_MODE mode)
{
    switch(mode) {
    case FORMAT_3D_OFF:
        return "FORMAT_3D_OFF";
    case FORMAT_3D_FP:
        return "FORMAT_3D_FP";
    case FORMAT_3D_SS:
        return "FORMAT_3D_SS";
    case FORMAT_3D_TB:
        return "FORMAT_3D_TB";
    default:
        return "3D_NA";
    }
}

static inline const char * getHDRModeString(HDMI_HDR_MODE mode)
{
    switch(mode) {
    case HDR_MODE_CURRENT:
        return "CURRENT";
    case HDR_MODE_FACTORY:
        return "FACTORY";
    case HDR_MODE_AUTO: //HDR_MODE_OFF:
        return "AUTO";
    case HDR_MODE_DV:
        return "DV";
    case HDR_MODE_SDR:
        return "SDR";
    case HDR_MODE_GAMMA:
        return "GAMMA";
    case HDR_MODE_HDR10:
        return "HDR10";
    case HDR_MODE_FUTURE:
        return "FUTURE";
    case HDR_MODE_INPUT:
        return "INPUT";
    case HDR_MODE_DV_LOW_LATENCY_12b_YUV422:
        return "DV_LOW_LATENCY_12b_YUV422";
    case HDR_MODE_DV_LOW_LATENCY_10b_YUV444:
        return "DV_LOW_LATENCY_10b_YUV444";
    case HDR_MODE_DV_LOW_LATENCY_10b_RGB444:
        return "DV_LOW_LATENCY_10b_RGB444";
    case HDR_MODE_DV_LOW_LATENCY_12b_YUV444:
        return "DV_LOW_LATENCY_12b_YUV444";
    case HDR_MODE_DV_LOW_LATENCY_12b_RGB444:
        return "DV_LOW_LATENCY_12b_RGB444";
    case HDR_MODE_DV_ON_INPUT:
        return "HDR_MODE_DV_ON_INPUT";
    case HDR_MODE_DV_ON_LOW_LATENCY_12b422_INPUT:
        return "HDR_MODE_DV_ON_LOW_LATENCY_12b422_INPUT";
    default:
        return "HDR_UNKNOWN";
    }
}

static inline const char * getEDIDModeStr(int32_t edidMode)
{
    switch(edidMode) {
    case TV_SYSTEM_AUTO_MODE_HDMI_EDID:
        return "TV_SYSTEM_AUTO_MODE_HDMI_EDID";
    case TV_SYSTEM_AUTO_MODE_OFF:
        return "TV_SYSTEM_AUTO_MODE_OFF";
    default:
        return "EDID_MODE_ERROR";
    }
}

static inline const char * getHDCPEventStr(int32_t event)
{
    switch(event) {
    case HDCP_EVENT_KEY_ERROR:
        return "HDCP_EVENT_KEY_ERROR";
    case HDCP_EVENT_NEGO_ERROR:
        return "HDCP_EVENT_NEGO_ERROR";
    case HDCP_EVENT_SUCCESS:
        return "HDCP_EVENT_SUCCESS";
    default:
        return "HDCP_EVENT_NA";
    }
}

static inline int32_t _readHDCP1xEnable(char *buf, int32_t len)
{
    FILE *fd = fopen(HDCP_1X_EN_SYSFS, "r");

    if(fd != NULL) {
        fread(buf,1,len,fd);
        fclose(fd);
        return 1;
    }else{
        return -1;
    }
}

static inline int32_t _readHDMIPlugged()
{
    FILE *fd = fopen(HDMITX_STATE, "r");
    int32_t tx_plugged = 0;

    if(fd != NULL) {
        char buf[32]={'\0'};
        fread(buf,1,32,fd);
        tx_plugged = atoi(buf);
        fclose(fd);
    }

    return tx_plugged;
}

static inline int32_t _HDCP_CkeckIfKeyExist()
{
    if(access(HDCP_1X_KEY_PATH,R_OK) != 0) {
        return -1;
    }else{
        return 1;
    }
}

static inline int32_t _HDCP22_CkeckIfKeyExist()
{
    if(access(HDCP_22_KEY_PATH,R_OK) != 0) {
        return -1;
    }else{
        return 1;
    }
}

static inline int32_t _readHDCP1xState()
{
    char buf[32]={0};
    FILE *fd = fopen(HDCP_1X_STATE_SYSFS, "r");

    if(fd != NULL) {
        fread(buf,1,32,fd);
        int val = atoi(buf);
        fclose(fd);
        return val;
    }else{
        return -1;
    }
}

static inline const char * _getInterfaceTypeString(INTERFACE_TYPE type)
{
    switch (type) {
    case ANALOG_AND_DIGITAL:
        return "ANALOG_AND_DIGITAL";
    case ANALOG_ONLY:
        return "ANALOG_ONLY";
    case DIGITAL_ONLY:
        return "DIGITAL_ONLY";
    case DISPLAY_PORT_ONLY:
        return "DISPLAY_PORT_ONLY";
    case HDMI_AND_DISPLAY_PORT_SAME_SOURCE:
        return "HDMI_AND_DISPLAY_PORT_SAME_SOURCE";
    case HDMI_AND_DISPLAY_PORT_DIFF_SOURCE:
        return "HDMI_AND_DISPLAY_PORT_DIFF_SOURCE";
    case DISPLAY_PORT_AND_CVBS_SAME_SOURCE:
        return "DISPLAY_PORT_AND_CVBS_SAME_SOURCE";
    case HDMI_AND_DP_DIFF_SOURCE_WITH_CVBS:
        return "HDMI_AND_DP_DIFF_SOURCE_WITH_CVBS";
    case FORCE_DP_OFF:
        return "FORCE_DP_OFF";
    default:
        return "INTERFACE_TYPE_ERROR";
    }
}

#endif
