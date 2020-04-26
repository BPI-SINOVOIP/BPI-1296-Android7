#include <utils/Log.h>
#include <string.h>
#include <media/stagefright/MediaDefs.h>

namespace android {

static const char videoDivx3[] = "DIVX3";
//static const char videoDivx[] = "DIVX";
static const char videoMpeg4[] = "MPEG4";
static const char videoMjpeg[] = "MJPEG";
//static const char videoDv[] = "DV";
static const char videoH264[] = "H264";
static const char videoH263[] = "H263";
static const char videoVp6[] = "VP6";
static const char videoVc1[] = "VC1";
//static const char videoVc1Advence[] = "VC1 Advance";
//static const char videoVc1WindowsMedia[] = "VC1 Windows Media";
static const char videoMpeg2[] = "MPEG2";
//static const char videoMpeg1[] = "MPEG1";
static const char videoRm[] = "RM";
static const char videoMp4[] = "MP4 Video";
static const char videoFlv[] = "FLV";
//static const char videoMkv[] = "MKV";
static const char videoAvs[] = "AVS";
static const char videoWmv[] = "WMV";
//static const char videoWmv7[] = "WMV7";
//static const char videoWmv8[] = "WMV8";
static const char videoVp8[] = "VP8";
static const char videoVp9[] = "VP9";
static const char videoH265[] = "H.265";
//static const char videoUnknown[] = "unknown";

const char* getVideoTypeString(const char *mimeType)
{
    const char *pRet = NULL;
    if(strcasecmp(mimeType, MEDIA_MIMETYPE_VIDEO_VP8) == 0)
        pRet = videoVp8;
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_VIDEO_VP9) == 0)
        pRet = videoVp9;
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_VIDEO_AVC) == 0)
        pRet = videoH264;
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_VIDEO_HEVC) == 0)
        pRet = videoH265;
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_VIDEO_MPEG4) == 0)
        pRet = videoMpeg4;
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_VIDEO_H263) == 0)
        pRet = videoH263;
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_VIDEO_MPEG2) == 0)
        pRet = videoMpeg2;
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_VIDEO_RAW) == 0)
        pRet = "RAW";
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_VIDEO_VC1) == 0)
        pRet = videoVc1;
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_VIDEO_RV) == 0)
        pRet = videoRm;
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_VIDEO_DIVX3) == 0)
        pRet = videoDivx3;
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_VIDEO_VP6) == 0)
        pRet = videoVp6;
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_VIDEO_AVS) == 0)
        pRet = videoAvs;
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_VIDEO_MJPEG) == 0)
        pRet = videoMjpeg;
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_VIDEO_WVC1) == 0)
        pRet = "WVC1";
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_VIDEO_WMV3) == 0)
        pRet = videoWmv;
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_VIDEO_WMV) == 0)
        pRet = videoWmv;
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_VIDEO_MP43) == 0)
        pRet = videoMp4;
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_VIDEO_FLV) == 0)
        pRet = videoFlv;
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_VIDEO_RV30) == 0)
        pRet = videoRm;
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_VIDEO_RV40) == 0)
        pRet = videoRm;
    else
        pRet = mimeType;
    return pRet;
}
}
