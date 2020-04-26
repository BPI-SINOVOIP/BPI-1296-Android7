#ifndef _REALTEK_PLAYER_MEDIA_TYPE_H_
#define _REALTEK_PLAYER_MEDIA_TYPE_H_

/**
 * copy from DvdPlayer source src/Include/StreamClass/EType.h
 */
typedef enum {
    MEDIATYPE_None,                     // 0, not available

//  Major Type:                         Description
    MEDIATYPE_AnalogAudio,              // 1, Analog audio.
    MEDIATYPE_AnalogVideo,              // 2, Analog video.
    MEDIATYPE_Audio,                    // 3, Audio.
    MEDIATYPE_Video,                    // 4, Video.
    MEDIATYPE_AUXLine21Data,            // 5, Line 21 data. Used by closed captions.
    MEDIATYPE_DVD_ENCRYPTED_PACK,       // 6, Encrypted DVD data
    MEDIATYPE_MPEG2_PES,                // 7, MPEG-2 PES packets.
    MEDIATYPE_ScriptCommand,            // 8, Data is a script command, used by closed captions.
    MEDIATYPE_Stream,                   // 9, Byte stream with no time stamps.
    MEDIATYPE_Text,                     // 10, Text.
    MEDIATYPE_Image,                    // 11, Image
    MEDIATYPE_Directory,                // 12, Direcotory
    MEDIATYPE_NEPTUNE_TP,               // 13

//  Audio Subtype:                      Description
    MEDIASUBTYPE_PCM,                   // 14, PCM audio.(big endian)
    MEDIASUBTYPE_PCM_LITTLE_ENDIAN,     // 15, PCM audio.(little endian)
    MEDIASUBTYPE_MPEG1Packet,           // 16, MPEG1 Audio packet.
    MEDIASUBTYPE_MPEG1ES,               // 17, MPEG1 Audio Payload (Elementary Stream).
    MEDIASUBTYPE_MPEG2_AUDIO,           // 18, MPEG-2 audio data
    MEDIASUBTYPE_DVD_LPCM_AUDIO,        // 19, DVD audio data
    MEDIASUBTYPE_MLP_AUDIO,             // 20, trueHD
    MEDIASUBTYPE_DOLBY_AC3,             // 21, Dolby data
    MEDIASUBTYPE_DOLBY_AC3_SPDIF,       // 22, Dolby AC3 over SPDIF.
    MEDIASUBTYPE_MPEG_AUDIO,            // 23, general MPEG audio
    MEDIASUBTYPE_DTS,                   // 24, DTS audio
    MEDIASUBTYPE_DTS_HD,                // 25, DTS-HD audio
    MEDIASUBTYPE_DTS_HD_EXT,            // 26, DTS-HD extension sub stream
    MEDIASUBTYPE_DTS_HD_CORE,           // 27, DTS_HD core sub stream
    MEDIASUBTYPE_DDP,                   // 28, Dolby Digital Plus audio
    MEDIASUBTYPE_SDDS,                  // 29, SDDS audio
    MEDIASUBTYPE_DV,                    // 30, DV audio
    MEDIASUBTYPE_AAC,                   // 31, AAC(Advanced Audio Coding)
    MEDIASUBTYPE_RAW_AAC,               // 32, AAC without header
    MEDIASUBTYPE_OGG_AUDIO,             // 33, OGG vorbis
    MEDIASUBTYPE_WMA,                   // 34, WMA audio
    MEDIASUBTYPE_WMAPRO,                // 35, WMAPRO audio
    MEDIASUBTYPE_MP3,                   // 36, MP3 file
    MEDIASUBTYPE_MP4,                   // 37, MP4 aac file
    MEDIASUBTYPE_LATM_AAC,              // 38, LATM/LOAS AAC file
    MEDIASUBTYPE_WAVE,                  // 39, WAVE audio
    MEDIASUBTYPE_AIFF,                  // 40, AIFF audio
    MEDIASUBTYPE_RTP,                   // 41, RTP
    MEDIASUBTYPE_APE,                   // 42, APE, pure audio

    MEDIASUBTYPE_DVD_SUB_PICTURE,           // 43, DVD-Video subpicture
    MEDIASUBTYPE_DIVX_SUB_PICTURE,          // 44, DivX Subtitles
    MEDIASUBTYPE_DVB_SUBTITLE_SUB_PICTURE,  // 45, DVB Subtitles
    MEDIASUBTYPE_DVB_TT_SUB_PICTURE,        // 46, DVB Teletext
    MEDIASUBTYPE_ATSC_CC_SUB_PICTURE,       // 47, ATSC close caption
    MEDIASUBTYPE_BLURAY_PGS_SUB_PICTURE,    // 48, Blu-ray PGS

// Video Subtype:                       Description
    MEDIASUBTYPE_DIVX3_VIDEO,           // 49, DivX 3.11 Video
    MEDIASUBTYPE_DIVX_VIDEO,            // 50, DivX 4/5 Video
    MEDIASUBTYPE_MPEG4_VIDEO,           // 51, MPEG-4 Video
    MEDIASUBTYPE_MJPEG_VIDEO,           // 52, Motion JPEG
    MEDIASUBTYPE_DVSD_VIDEO,            // 53, DV(Standard Definition)Video
    MEDIASUBTYPE_H264_VIDEO,            // 54
    MEDIASUBTYPE_H263_VIDEO,            // 55
    MEDIASUBTYPE_VP6_VIDEO,             // 56
    MEDIASUBTYPE_VC1_VIDEO,             // 57
    MEDIASUBTYPE_VC1_ADVANCED_VIDEO,    // 58
    MEDIASUBTYPE_VC1_WINDOWSMEDIA_VIDEO,// 59
    MEDIASUBTYPE_AVS_VIDEO,             // 60

//  DVD type/Subtype:                        Description
    MEDIATYPE_DVD,                      // 61
    MEDIATYPE_SVCD,                     // 62
    MEDIATYPE_VCD,                      // 63
    MEDIATYPE_CDDA,                     // 64
    MEDIATYPE_FILE,                     // 65
    MEDIATYPE_PLAYLIST,                 // 66
    MEDIATYPE_NET,                      // 67
    MEDIATYPE_MMS,                      // 68
    MEDIATYPE_TRANSCODE,                // 69
    MEDIATYPE_DV,                       // 70
    MEDIATYPE_DV_FILE,                  // 71
    MEDIATYPE_STB,                      // 72
    MEDIATYPE_FILESET,                  // 73
    MEDIATYPE_FILELINK,                 // 74
    MEDIATYPE_RTSP,                     // 75
    MEDIATYPE_RTP,                      // 76, hongjian_shen
    MEDIATYPE_PPS,                      // 77,
    MEDIATYPE_BESTV,                    // 78,
    MEDIATYPE_VOOLETV,                  // 79,
    MEDIATYPE_THUNDER_REMOTE,           // 80,
    MEDIATYPE_THUNDER,                  // 81,
    MEDIATYPE_CUSTOM_1,                 // 82,
    MEDIATYPE_CUSTOM_2,                 // 83
    MEDIATYPE_CUSTOM_3,                 // 84

    MEDIASUBTYPE_MPEG2_VIDEO,           // 85, MPEG-2 video
    MEDIASUBTYPE_DVD_SUBPICTURE,        // 86, DVD Subpicture
    MEDIASUBTYPE_DVD_RTRW_PLUS,         // 87, DVD +RW
    MEDIASUBTYPE_DVD_RTRW_MINUS,        // 88, DVD -VR
    MEDIASUBTYPE_DVD_RTRW_MINUS_HD,     // 89, DVD -VR on HDD (as special case)
    MEDIASUBTYPE_DVD_VIDEO,             // 90, Recordble DVD -VIDEO
    MEDIASUBTYPE_DVD_VIDEO_ROM,         // 91, DVD ROM
    MEDIASUBTYPE_DVD_RTRW_STILL,        // 92, DVD -VR Still Picture
    MEDIASUBTYPE_DVD_MENU,              // 93, DVD Menu VOB
    MEDIASUBTYPE_SVCD,                  // 94, Recordable SVCD
    MEDIASUBTYPE_SVCD_ROM,              // 95, SVCD Read Only
    MEDIASUBTYPE_VCD,                   // 96, VCD Recordable
    MEDIASUBTYPE_VCD_ROM,               // 97, VCD Read Only
    MEDIASUBTYPE_CDDA,                  // 98, Recordable CDDA
    MEDIASUBTYPE_CDDA_ROM,              // 99, CDDA Read Only
    MEDIASUBTYPE_AVCHD,                 // 100, AVCHD
    MEDIASUBTYPE_AVCHD_ROM,             // 101, AVCHD Read Only
    MEDIASUBTYPE_HTTP,                  // 102,
    MEDIASUBTYPE_MMS,                   // 103,
    MEDIASUBTYPE_TRANSCODE,             // 104,
    MEDIASUBTYPE_MPEG_PROGRAM,          // 105,
    MEDIASUBTYPE_STB_1394,              // 106, Richard
    MEDIASUBTYPE_FILESET,               // 107,
    MEDIASUBTYPE_FILELINK,              // 108

//  Line 21 Subtype:                    Description
    MEDIASUBTYPE_Line21_BytePair,       // 109, Line 21 data as byte pairs
    MEDIASUBTYPE_Line21_GOPPacket,      // 110, Line 21 data in DVD GOP Packet
    MEDIASUBTYPE_Line21_VBIRawData,     // 111, Line 21 data in raw VBI format

//  MPEG-1 Subtypes:                    Description
    MEDIASUBTYPE_MPEG1Audio,            // 112, MPEG audio
    MEDIASUBTYPE_MPEG1System,           // 113, MPEG system
    MEDIASUBTYPE_MPEG1SystemStream,     // 114, Obsolete. Do not use.
    MEDIASUBTYPE_MPEG1Video,            // 115, MPEG video
    MEDIASUBTYPE_MPEG1VideoCD,          // 116, MPEG video CD
    MEDIASUBTYPE_MPEG1SuperVCD,         // 117, MPEG SVCD

//  MPEG-2 Subtypes:                    Description
    MEDIASUBTYPE_MPEG2_PROGRAM,         // 118, Program stream
    MEDIASUBTYPE_MPEG2_TRANSPORT,       // 119, Transport stream (TS), with 188-byte packets

//  Subpicture Subtypes:                Description
    MEDIASUBTYPE_AI44,                  // 120, For subpicture and text overlays
    MEDIASUBTYPE_IAK2,                  // 121, For subpicture and text overlays,
                                        //     2 bits indexed, constant alpha and single color key
    MEDIASUBTYPE_IAK4,                  // 122, For subpicture and text overlays,
                                        //     4 bits indexed, constant alpha and single color key
    MEDIASUBTYPE_IAK8,                  // 123, For subpicture and text overlays,
                                        //     8 bits indexed, constant alpha and single color key.
                                        //     16 programmable palete.
//  External Subtitles
    MEDIASUBTYPE_SRT,                   // 124,
    MEDIASUBTYPE_SSA,                   // 125,
    MEDIASUBTYPE_ASS,                   // 126,
    MEDIASUBTYPE_TXT,                   // 127

//  Analog Video Subtypes:              Description
    MEDIASUBTYPE_AnalogVideo_NTSC,      // 128, NTSC
    MEDIASUBTYPE_AnalogVideo_PAL,       // 129, PAL (will be extended)

//  Image Subtypes
    MEDIASUBTYPE_IMAGE_JPEG,            // 130, JPEG Images
    MEDIASUBTYPE_IMAGE_BITMAP,          // 131, BMP Images
    MEDIASUBTYPE_IMAGE_GIF,             // 132, GIF Images
    MEDIASUBTYPE_IMAGE_TIFF,            // 133, TIFF Images
    MEDIASUBTYPE_IMAGE_PNG,             // 134, PNG Images
    MEDIASUBTYPE_IMAGE_DNG,             // 135, DNG Images



    MEDIASUBTYPE_MPEG2_AUDIO_WITH_EXTENSION,    // 136
    MEDIASUBTYPE_MPEG2_AUDIO_WITHOUT_EXTENSION, //137


    MEDIASUBTYPE_PPSRM,         // 138, PPS RM
    MEDIASUBTYPE_PPSWMA,        // 139, PPS WMA

    MEDIASUBTYPE_BESTVRM,       // 140, PPS RM
    MEDIASUBTYPE_BESTVWMA,      // 141, PPS WMA
    MEDIASUBTYPE_BESTVMP4,      // 142

    // Real Media
    MEDIASUBTYPE_RM,            // 143
    MEDIASUBTYPE_RV,            // 144, real video
    MEDIASUBTYPE_RA_COOK,       // 145, RealAudio 8 Low Bit Rate(cook)
    MEDIASUBTYPE_RA_ATRC,       // 146, RealAudio 8 Hight Bit Rate(atrc)
    MEDIASUBTYPE_RA_RAAC,       // 147, AAC(raac)
    MEDIASUBTYPE_RA_SIPR,       // 148, RealAudio Voice(sipr)
    MEDIASUBTYPE_RA_LSD,        // 149, RealAudio Lossless

    MEDIASUBTYPE_ADPCM,         // 150, ADPCM audio
    MEDIASUBTYPE_FLAC,          // 151
    MEDIASUBTYPE_ULAW,          // 152
    MEDIASUBTYPE_ALAW,          // 153

// AVI Container subtype (using DivX specification)
    MEDIASUBTYPE_DMF_0,         // 154, Traditional AVI 1.0
    MEDIASUBTYPE_DMF_1,         // 155, DivX Ultra
    MEDIASUBTYPE_DMF_2,         // 156, OpenDML 2.0

    MEDIASUBTYPE_MP4_VIDEO,     // 157, mp4 video file

//  File Container Type
    MEDIATYPE_Interleaved,      // 158, Interleaved audio and video, aka AVI (Type1 DV AVI will use MEDIATYPE_DV)
    MEDIATYPE_FLASHVIDEO,       // 159, Macromedia Flash Video
    MEDIATYPE_MATROSKA,         // 160, Matroska Container (MKV)
    MEDIASUBTYPE_MATROSKA_DIVX_ENHANCED,    // 161,
    MEDIATYPE_ASF,              // 162, ASF/WMV/WMA file

// HDMV lpcm audio type
    MEDIASUBTYPE_PCM_HDMV,      // 163

    //used for blue-ray
    MEDIASUBTYPE_HDMV_MLP_AUDIO,    // 164, trueHD audio
    MEDIASUBTYPE_HDMV_DOLBY_AC3,    // 165
    MEDIASUBTYPE_HDMV_DDP,          // 166
    MEDIASUBTYPE_HDMV_DTS,          // 167
    MEDIASUBTYPE_HDMV_DTS_HD,       // 168

    MEDIATYPE_FLASH,                // 169

    MEDIATYPE_PHOTO_ALBUM,          // 170

    MEDIATYPE_NRD,                  // 171
    MEDIASUBTYPE_NRD_2XX,           // 172

    MEDIASUBTYPE_BD,                // 173
    MEDIASUBTYPE_BD_ROM,            // 174

    MEDIASUBTYPE_VP8_VIDEO,         // 175

    MEDIATYPE_DIRECT_CONNECT,       // 176

    MEDIATYPE_HLS,                  // 177, HTTP Live Streaming

    MEDIATYPE_CONFERENCE,           // 178,

    MEDIASUBTYPE_AMRWB_AUDIO,       // 179
    MEDIASUBTYPE_AMRNB_AUDIO,       // 180

    //Used for Skype
    MEDIASUBTYPE_SILK_AUDIO,        // 181
    MEDIASUBTYPE_G729_AUDIO,        // 182

    MEDIASUBTYPE_MVC_DEPENDENT_VIDEO, //183

    //used for camera display
    MEDIATYPE_CAMERA,               // 184
    MEDIASUBTYPE_YUV_VIDEO,         // 185

    MEDIASUBTYPE_APE_AUDIO,         // 186

    MEDIASUBTYPE_WMV7_VIDEO,        // 187
    MEDIASUBTYPE_WMV8_VIDEO,        // 188

    MEDIATYPE_SOCKET,           // 189
    MEDIATYPE_SNDA,             // 190

    MEDIASUBTYPE_IMAGE_JPS,     //191 3d photo
    MEDIASUBTYPE_IMAGE_MPO,     //192 3d photo

    MEDIASUBTYPE_NRD_3XX,       //193
    MEDIATYPE_OMADRMFILE,       //194 omadrm file
    MEDIASUBTYPE_OMADRMMP4,     //195 omadrm mp4
    MEDIASUBTYPE_OMADRMASF,     //196 omadrm asf
    MEDIATYPE_SERVERIO,                //197, client deliver us content via net

    MEDIASUBTYPE_RA_RA1,        //198
    MEDIASUBTYPE_FJPEG_VIDEO,       //199

    MEDIASUBTYPE_ALAC,          // 200 for m4a

    MEDIATYPE_PPTV,             // 201 for PPTV

    MEDIATYPE_UDP,              // 202. UDP
    MEDIATYPE_MIRACAST_SOCKET,  // 203 for Miracast

    MEDIASUBTYPE_InputDummyNET, //204 for IMS
    MEDIATYPE_DUMMYFILESET,     //205
    MEDIASUBTYPE_VOD,   //206
    MEDIASUBTYPE_CNLIVE,    //207
    MEDIATYPE_LIVESOCKET,   //208

    // video type add for H265
    MEDIASUBTYPE_H265_VIDEO,    //209
	MEDIASUBTYPE_VP9_VIDEO,		//210
    MEDIASUBTYPE_DRA,                   // 211, DRA audio
    MEDIASUBTYPE_OGG_OPUS,              // 212, OGG opus

	// video type for ring buffer 2
	MEDIASUBTYPE_ENHANCEMENT_LAYER_VIDEO, //213
    
    MEDIASUBTYPE_HDMV_MLP_AUDIO_ATMOS,    // 214, trueHD audio with atmos
} ENUM_MEDIA_TYPE;

#endif // _REALTEK_PLAYER_MEDIA_TYPE_H_
