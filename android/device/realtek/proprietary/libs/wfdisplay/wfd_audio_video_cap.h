#ifndef _WFD_AUDIO_VIDEO_CAP_H_
#define _WFD_AUDIO_VIDEO_CAP_H_

// here we define all the video modes used by WFD

/*
 * 2D video used by WFD
 */

// CEA list
#define    CEA0       0x00000001      // 640x480p60
#define    CEA1       0x00000002      // 720x480p60
#define    CEA2       0x00000004      // 720x480i60
#define    CEA3       0x00000008      // 720x576p50
#define    CEA4       0x00000010      // 720x576i50
#define    CEA5       0x00000020      // 1280x720p30
#define    CEA6       0x00000040      // 1280x720p60
#define    CEA7       0x00000080      // 1920x1080p30
#define    CEA8       0x00000100      // 1920x1080p60
#define    CEA9       0x00000200      // 1920x1080i60
#define    CEA10      0x00000400      // 1280x720p25
#define    CEA11      0x00000800      // 1280x720p50
#define    CEA12      0x00001000      // 1920x1080p25
#define    CEA13      0x00002000      // 1920x1080p50
#define    CEA14      0x00004000      // 1920x1080i50
#define    CEA15      0x00008000      // 1280x720p24
#define    CEA16      0x00010000      // 1920x1080p24
//CEA2
#define    CEA17      0x00020000      // 3840x2160 p24
#define    CEA18      0x00040000      // 3840x2160 p25
#define    CEA19      0x00080000      // 3840x2160 p30
#define    CEA20      0x00100000      // 3840x2160 p50
#define    CEA21      0x00200000      // 3840x2160 p60
#define    CEA22      0x00400000      // 4096x2160 p24
#define    CEA23      0x00800000      // 4096x2160 p25
#define    CEA24      0x01000000      // 4096x2160 p30
#define    CEA25      0x02000000      // 4096x2160 p50
#define    CEA26      0x04000000      // 4096x2160 p60



// VESA LIST
#define    VESA0      0x00000001      // 800x600p30
#define    VESA1      0x00000002      // 800x600p60
#define    VESA2      0x00000004      // 1024x768p30
#define    VESA3      0x00000008      // 1024x768p60
#define    VESA4      0x00000010      // 1152x864p30
#define    VESA5      0x00000020      // 1152x864p60
#define    VESA6      0x00000040      // 1280x768p30
#define    VESA7      0x00000080      // 1280x768p60
#define    VESA8      0x00000100      // 1280x800p30
#define    VESA9      0x00000200      // 1280x800p60
#define    VESA10     0x00000400      // 1360x768p30
#define    VESA11     0x00000800      // 1360x768p60
#define    VESA12     0x00001000      // 1366x768p30
#define    VESA13     0x00002000      // 1366x768p60
#define    VESA14     0x00004000      // 1280x1024p30
#define    VESA15     0x00008000      // 1280x1024p60
#define    VESA16     0x00010000      // 1400x1050p30
#define    VESA17     0x00020000      // 1400x1050p60
#define    VESA18     0x00040000      // 1440x900p30
#define    VESA19     0x00080000      // 1440x900p60
#define    VESA20     0x00100000      // 1600x900p30
#define    VESA21     0x00200000      // 1600x900p60
#define    VESA22     0x00400000      // 1600x1200p30
#define    VESA23     0x00800000      // 1600x1200p60
#define    VESA24     0x01000000      // 1680x1024p30
#define    VESA25     0x02000000      // 1680x1024p60
#define    VESA26     0x04000000      // 1680x1050p30
#define    VESA27     0x08000000      // 1680x1050p60
#define    VESA28     0x10000000      // 1920x1200p30
#define    VESA29     0x20000000      // 1920x1200p60
//R2
#define    VESA30 0x000040000000      // 2560x1440 p30
#define    VESA31 0x000080000000      // 2560x1440 p60
#define    VESA32 0x000100000000      // 2560x1600 p30
#define    VESA33 0x000200000000      // 2560x1600 p60

// HH list
#define    HH0        0x00000001      // 800x480p30
#define    HH1        0x00000002      // 800x480p60
#define    HH2        0x00000004      // 854x480p30
#define    HH3        0x00000008      // 854x480p60
#define    HH4        0x00000010      // 864x480p30
#define    HH5        0x00000020      // 864x480p60
#define    HH6        0x00000040      // 640x360p30
#define    HH7        0x00000080      // 640x360p60
#define    HH8        0x00000100      // 960x540p30
#define    HH9        0x00000200      // 960x540p60
#define    HH10       0x00000400      // 848x480p30
#define    HH11       0x00000800      // 848x480p60


// 3D video used by WFD

#define    VIDEO_3D0   0x0000000000000001LL // 1920x(540+540)p24, top&bottom    
#define    VIDEO_3D1   0x0000000000000002LL // 1280x(360+360)p60, top&bottom
#define    VIDEO_3D2   0x0000000000000004LL // 1280x(360+360)p50, top&bottom
#define    VIDEO_3D3   0x0000000000000008LL // 1920x1080p24x2, frame sequential
#define    VIDEO_3D4   0x0000000000000010LL // 1280x720p60x2, frame sequential
#define    VIDEO_3D5   0x0000000000000020LL // 1280x720p30x2, frame sequential
#define    VIDEO_3D6   0x0000000000000040LL // 1280x720p50x2, frame sequential
#define    VIDEO_3D7   0x0000000000000080LL // 1280x720p25x2, frame sequential
#define    VIDEO_3D8   0x0000000000000100LL // 1920x(1080+45+1080)p24, frame packing
#define    VIDEO_3D9   0x0000000000000200LL // 1280x(720+30+720)p60, frame packing
#define    VIDEO_3D10  0x0000000000000400LL // 1280x(720+30+720)p30, frame packing
#define    VIDEO_3D11  0x0000000000000800LL // 1280x(720+30+720)p50, frame packing
#define    VIDEO_3D12  0x0000000000001000LL // 1280x(720+30+720)p25, frame packing
#define    VIDEO_3D13  0x0000000000002000LL // (960+960)x1080i60, side by side
#define    VIDEO_3D14  0x0000000000004000LL // (960+960)x1080i50, side by side
#define    VIDEO_3D15  0x0000000000008000LL // 640x(240+240)p60, top & bottom
#define    VIDEO_3D16  0x0000000000010000LL // (320+320)x480p60, side by side
#define    VIDEO_3D17  0x0000000000020000LL // 720x(240+240)p60, top & bottom
#define    VIDEO_3D18  0x0000000000040000LL // (360+360)x480p60, side by side
#define    VIDEO_3D19  0x0000000000080000LL // 720x(288+288)p50, top & bottom
#define    VIDEO_3D20  0x0000000000100000LL // (360+360)x576p50, side by side
#define    VIDEO_3D21  0x0000000000200000LL // 1280x(360+360)p24, top & bottom
#define    VIDEO_3D22  0x0000000000400000LL // (640+640)x720p24, side by side
#define    VIDEO_3D23  0x0000000000800000LL // 1280x(360+360)p25, top & bottom
#define    VIDEO_3D24  0x0000000001000000LL // (640+640)x720p25, side by side
#define    VIDEO_3D25  0x0000000002000000LL // 1280x(360+360)p30, top & bottom
#define    VIDEO_3D26  0x0000000004000000LL // (640+640)x720p30, side by side
#define    VIDEO_3D27  0x0000000008000000LL // 1920x(540+540)p30, top & bottom
#define    VIDEO_3D28  0x0000000010000000LL // 1920x(540+540)p50, top & bottom
#define    VIDEO_3D29  0x0000000020000000LL // 1920x(540+540)p60, top & bottom
#define    VIDEO_3D30  0x0000000040000000LL // (640+640)x720p50, side by side
#define    VIDEO_3D31  0x0000000080000000LL // (640+640)x720p60, side by side
#define    VIDEO_3D32  0x0000000100000000LL // (960+960)x1080p24, side by side
#define    VIDEO_3D33  0x0000000200000000LL // (960+960)x1080p50, side by side
#define    VIDEO_3D34  0x0000000400000000LL // (960+960)x1080p60, side by side
#define    VIDEO_3D35  0x0000000800000000LL // 1920x(1080+45+1080)p30, frame packing
#define    VIDEO_3D36  0x0000001000000000LL // 1920x(1080+45+1080)i50, frame packing
#define    VIDEO_3D37  0x0000002000000000LL // 1920x(1080+45+1080)i60, frame packing


// Audio formats used by WFD

// LPCM
#define    LPCM0       0x00000001    // lpcm, 44.1khz, 16bits, 2 channels
#define    LPCM1       0x00000002    // lpcm, 48khz, 16bits, 2 channels
// AACs
#define    AAC0        0x00000001    // aac, 48khz, 16bits, 2 channels
#define    AAC1        0x00000002    // aac, 48khz, 16bits, 4 channels
#define    AAC2        0x00000004    // aac, 48khz, 16bits, 6 channels
#define    AAC3        0x00000008    // aac, 48khz, 16bits, 8 channels
//AC3s
#define    AC30        0x00000001    // ac3, 48khz, 16bits, 2 channels
#define    AC31        0x00000002    // ac3, 48khz, 16bits, 4 channels
#define    AC32        0x00000004    // ac3, 48khz, 16bits, 6 channels

// other video parameters used by WFD

// video codec
#define    H264        0X01
#define    H265        0X02

// h264 profile
#define    H264_CBP    0x01
#define    H264_CHP    0x02
#define    H264_RHP    0x04
#define    H264_BP     0x08
#define    H264_MP     0x10
#define    H264_HP     0x20

// h265 profile
#define    H265_MP     0x01
#define    H265_MP_10     0x02

// h264 level
#define    H264_LEVEL_31  0x01
#define    H264_LEVEL_32  0x02
#define    H264_LEVEL_4   0x04
#define    H264_LEVEL_41  0x08
#define    H264_LEVEL_42  0x10
#define    H264_LEVEL_5   0x20
#define    H264_LEVEL_51  0x40
#define    H264_LEVEL_52  0x80

// h265 level
 #define    H265_LEVEL_31  0X01
 #define    H265_LEVEL_4   0X02
 #define    H265_LEVEL_41  0X04
 #define    H265_LEVEL_5   0X08
#define    H265_LEVEL_51  0X10

// frame-control support
#define    FRAME_SKIP_SUPPORT    0x01
#define    MAX_SKIP_NO_LIMIT     0x00
#define    MAX_SKIP_1            0x02
#define    MAX_SKIP_MASK         0x0e
#define    FRAME_RATE_SUPPORT    0x10


/*
 * the following defines are for SIGMA required parameters
 */
#define SIGMA_VIDEO_FRAME_CONTROL           FRAME_SKIP_SUPPORT|MAX_SKIP_1|FRAME_RATE_SUPPORT

// mandatory video list
#define SIGMA_MANDATORY_CEA   CEA0|CEA1|CEA6
#define SIGMA_MANDATORY_VESA  0x00000000
#define SIGMA_MANDATORY_HH    0x00000000

// manatory sigma audio supported list
#define SIGMA_AUDIO_LPCM      LPCM0|LPCM1
#define SIMGA_AUDIO_AAC       0x00000000
#define SIGMA_AUDIO_AC3       0x00000000
#define SIGMA_AUDIO_DELAY     0x02


/*
 * For each chip, define its capability here
 */


/*
 * for SATURN, define chip's video capability
 */
#if 1
// the following parameter defines saturn's capability
#define NATIVE_MODE    0x40   // native resolution is (CEA, index 8) = 1080p60
#define NATIVE_MODE2   0x40   // native resolution is (CEA, index 16) = 1920x1080 p24
#define VIDEO_CODEC    H265   // support codec up to H.265
#define H264_PROFILE   H264_MP   // saturn supports chp 
#define H265_PROFILE   H265_MP     // support H.265 Main Profile
#define H264_LEVEL     H264_LEVEL_51 // saturn supports level up to 5.2
#define H265_LEVEL     H265_LEVEL_51  // support level up to 5.1
#define MAX_H_RESOLUTION  1920 // max horizontal resolution as codec is H.264 
#define MAX_V_RESOLUTION  1080 // max visual resolution as codec is H.264 
// video modes supported by saturn 
#define CEA_SUPPORT (CEA0|CEA1|CEA2|CEA3|CEA4|CEA5|CEA6|CEA7|CEA8|CEA9|CEA10|CEA11|CEA12|CEA13|CEA14|CEA15|CEA16)
#define CEA_SUPPORT2 (CEA_SUPPORT|CEA17|CEA18|CEA19)

#define VESA_SUPPORT (VESA0|VESA1|VESA2|VESA3|VESA4|VESA5|VESA6|VESA7|VESA8|VESA9|VESA10|VESA11|VESA12|VESA13|VESA14|VESA16|VESA18|VESA20|VESA22|VESA24|VESA26|VESA28)
#define VESA_SUPPORT2 (VESA_SUPPORT|VESA30|VESA32)

#define HH_SUPPORT (HH0|HH1|HH2|HH3|HH4|HH5|HH6|HH7|HH8|HH9|HH10|HH11)

#define VIDEO_LATENCY   0x02
#define VIDEO_LATENCY2  0x10
#define MIN_SLICE_SIZE        0x0000
#define SLICE_ENC_PARAMS      0x0000
#define SLICE_ENC_PARAMS2     0x001f

// video control param
#define VIDEO_FRAME_CONTROL    FRAME_SKIP_SUPPORT|MAX_SKIP_NO_LIMIT|FRAME_RATE_SUPPORT
#define NON_TRANSCODING 0x00

#define VIDEO_3D_SUPPORT  (VIDEO_3D0|VIDEO_3D1|VIDEO_3D2|VIDEO_3D13|VIDEO_3D14|VIDEO_3D15|VIDEO_3D16|VIDEO_3D17|VIDEO_3D18|VIDEO_3D19|VIDEO_3D20|VIDEO_3D21|VIDEO_3D22|VIDEO_3D23|VIDEO_3D24|VIDEO_3D25|VIDEO_3D26|VIDEO_3D27|VIDEO_3D28|VIDEO_3D29|VIDEO_3D30|VIDEO_3D31|VIDEO_3D32|VIDEO_3D33|VIDEO_3D34)

// audio supported by saturn
#define AUDIO_LPCM   (LPCM0|LPCM1)
#define AUDIO_AAC    (AAC0|AAC1|AAC2|AAC3)
#define AUDIO_AC3    (AC30|AC31|AC32)
#define AUDIO_DELAY  0x02
#endif  // IS_CHIP(SATURN)

#endif
