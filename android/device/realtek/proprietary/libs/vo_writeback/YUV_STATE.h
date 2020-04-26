#ifndef YUV_STATE_H
#define YUV_STATE_H

typedef struct {
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
     *     valid only when mode is INTERLEAVED_TOP_BOT_FIELD or INTERLEAVED_BOT_TOP_FIELD*/
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
    unsigned int mvc;         /* 1: mvc */
    unsigned int subPicOffset;/* 3D Blu-ray dependent-view sub-picture plane offset metadata as defined in BD spec sec. 9.3.3.6.
                                 Valid only when Y_BufId_Right and C_BufId_Right are both valid */
    unsigned int pReceived;         // fix bug 44329 by version 0x72746B30 'rtk0'
    unsigned int pReceived_Right;   // fix bug 44329 by version 0x72746B30 'rtk0'

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
    unsigned int Y_addr_prev;
    unsigned int U_addr_prev;
    unsigned int Y_addr_next;
    unsigned int U_addr_next;

    unsigned int video_full_range_flag; //'rtk4'
    unsigned int matrix_coefficients; //'rtk4

    /*for transcode interlaced feild use.*/  //'rtk5'
    unsigned int pLock_prev;
    unsigned int pReceived_prev;
    unsigned int pLock_next;
    unsigned int pReceived_next;
} YUV_STATE;

#endif
