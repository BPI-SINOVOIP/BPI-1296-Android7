#ifndef __RTK_HWC_HWLAYER_VIDEO_BUFFER_H_
#define __RTK_HWC_HWLAYER_VIDEO_BUFFER_H_
#include <utils/BufferBase.h>
#include <utils/PLockBase.h>
#include <utils/DebugMessage.h>
#include <utils/HDCPMessage.h>
#include "CombinedManager.h"
#include <InbandAPI.h>
#include <utils/IonBuffer.h>

class VideoBuffer : public BufferBase,
    virtual public PLockBase::Client,
    virtual public DebugMessage::Client,
    virtual public HDCPMessage::Client,
    virtual public CombinedManager::Client
{
public:
    VideoBuffer();
    virtual ~VideoBuffer();
    virtual int prepareBuffer_();
    virtual void queueBuffer_();
    virtual void cancelBuffer_();
    virtual bool releaseBufferCheck();
    virtual void removeBuffer();
    virtual void dump(android::String8& buf, const char* prefix = NULL);

    static const int PLOCK_VERSION = 0x72746B33;
private:
    virtual void updateHdmiKey();
    virtual void releaseBuffer_();
    virtual void resetWin();

#define VP_PICTURE_MODE_MASK  0x0000ffff
#define VP_PICTURE_MODE_EXT_MASK 0xffff0000

    typedef enum
    {
        INTERLEAVED_TOP_FIELD = 0,  /* top    field data stored in even lines of a frame buffer */
        INTERLEAVED_BOT_FIELD,      /* bottom field data stored in odd  lines of a frame buffer */
        CONSECUTIVE_TOP_FIELD,      /* top    field data stored consecutlively in all lines of a field buffer */
        CONSECUTIVE_BOT_FIELD,      /* bottom field data stored consecutlively in all lines of a field buffer */
        CONSECUTIVE_FRAME,           /* progressive frame data stored consecutlively in all lines of a frame buffer */
        INTERLEAVED_TOP_FIELD_422,  /* top    field data stored in even lines of a frame buffer */
        INTERLEAVED_BOT_FIELD_422,      /* bottom field data stored in odd  lines of a frame buffer */
        CONSECUTIVE_TOP_FIELD_422,      /* top    field data stored consecutlively in all lines of a field buffer */
        CONSECUTIVE_BOT_FIELD_422,      /* bottom field data stored consecutlively in all lines of a field buffer */
        CONSECUTIVE_FRAME_422,        /* progressive frame with 4:2:2 chroma */
        TOP_BOTTOM_FRAME,            /* top field in the 0~height/2-1, bottom field in the height/2~height-1 in the frame */
        INTERLEAVED_TOP_BOT_FIELD,   /* one frame buffer contains one top and one bot field */
        INTERLEAVED_BOT_TOP_FIELD,   /* one frame buffer contains one bot and one top field */
        MPEG2_PIC_MODE_NOT_PROG,     /*yllin: for MPEG2 check pic mode usage */
        SOFTWARE_DEINTERLACED     /* frame data is calculated by software deinterlace */

    } VP_PICTURE_MODE ;

    typedef enum
    {
        VO_NV21_MASK = 0x1u<<16, //1:NV21 0:NV12
    } VP_PICTURE_MODE_EXT ;

    struct YUV_STATE {
        unsigned int version;
        unsigned int mode;
        unsigned int Y_addr;
        unsigned int U_addr;
        unsigned int pLock;
        unsigned int width;
        unsigned int height;
        unsigned int Y_pitch;
        unsigned int C_pitch;
        unsigned int RPTSH;
        unsigned int RPTSL;
        unsigned int PTSH;
        unsigned int PTSL;

        /* for send two interlaced fields in the same packet,
           valid only when mode is INTERLEAVED_TOP_BOT_FIELD or INTERLEAVED_BOT_TOP_FIELD*/
        unsigned int RPTSH2;
        unsigned int RPTSL2;
        unsigned int PTSH2;
        unsigned int PTSL2;

        unsigned int context;
        unsigned int pRefClock;  /* not used now */

        unsigned int pixelAR_hor; /* pixel aspect ratio hor, not used now */
        unsigned int pixelAR_ver; /* pixel aspect ratio ver, not used now */

        unsigned int Y_addr_Right; /* for mvc */
        unsigned int U_addr_Right; /* for mvc */
        unsigned int pLock_Right; /* for mvc */
        unsigned int mvc;		  /* 1: mvc */
        unsigned int subPicOffset;/* 3D Blu-ray dependent-view sub-picture plane offset metadata as defined in BD spec sec. 9.3.3.6.
                                     Valid only when Y_BufId_Right and C_BufId_Right are both valid */
        unsigned int pReceived; 		// fix bug 44329 by version 0x72746B30 'rtk0'
        unsigned int pReceived_Right;	// fix bug 44329 by version 0x72746B30 'rtk0'

        unsigned int fps;   // 'rtk1'
        unsigned int IsForceDIBobMode; // force vo use bob mode to do deinterlace, 'rtk2'.
        unsigned int lumaOffTblAddr;    // 'rtk3'
        unsigned int chromaOffTblAddr;  // 'rtk3'
        unsigned int lumaOffTblAddrR; /* for mvc, 'rtk3' */
        unsigned int chromaOffTblAddrR; /* for mvc, 'rtk3' */

        unsigned int bufBitDepth;   // 'rtk3'
        unsigned int bufFormat;     // 'rtk3', according to VO spec: 10bits Pixel Packing mode selection, "0": use 2 bytes to store 1 components. MSB justified. "1": use 4 bytes to store 3 components. LSB justified. 

        // VUI (Video Usability Information)
        unsigned int transferCharacteristics;   // 0:SDR, 1:HDR, 2:ST2084, 'rtk3'

        // Mastering display colour volume SEI, 'rtk3'
        unsigned int display_primaries_x0;
        unsigned int display_primaries_y0;
        unsigned int display_primaries_x1;
        unsigned int display_primaries_y1;
        unsigned int display_primaries_x2;
        unsigned int display_primaries_y2;
        unsigned int white_point_x;
        unsigned int white_point_y;
        unsigned int max_display_mastering_luminance;
        unsigned int min_display_mastering_luminance;

        /*for transcode interlaced feild use.*/ //'rtk4'
        unsigned int Y_addr_prev;   //'rtk4'
        unsigned int U_addr_prev;   //'rtk4'
        unsigned int Y_addr_next;   //'rtk4'
        unsigned int U_addr_next;   //'rtk4'
        unsigned int video_full_range_flag; //'rtk4'  default= 1
        unsigned int matrix_coefficients;   //'rtk4   default= 1

        /*for transcode interlaced feild use.*/  //'rtk5'
        unsigned int pLock_prev;
        unsigned int pReceived_prev;
        unsigned int pLock_next;
        unsigned int pReceived_next;

        unsigned int is_tch_video; // rtk-6
        tch_metadata technicolor_data;
        unsigned int pFrameBufferDbg;  //rtk7
        unsigned int pFrameBufferDbg_Right;
    } mYUVWin;

    enum VideoBufferFlags {
        FILLYUV         = 0x1U <<  0,
        QUEUE_BUFFER    = 0x1U <<  1,
    };
    unsigned int mFlags;
    unsigned long long mPTS[2];
    void initYuvState(struct YUV_STATE * picObj);
    IonBuffer * mDebugInfo;
};

#endif /* End of __RTK_HWC_HWLAYER_VIDEO_BUFFER_H_ */
