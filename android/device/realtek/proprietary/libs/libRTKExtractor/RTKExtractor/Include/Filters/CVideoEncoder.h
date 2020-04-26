//---------------------------------------------------------------------------
//File: CVideoEncoder.h
//
//Desc: Header file for DVD-VENUS Program Video Encoder Filter Class 
//      interface define
//
//Author: 
//
//Copyright (c)RealTek.   All rights reserved.

//Just includes the defination of the info structure, which is needed by the 
//DVD Program Mux Filter.

#include "StreamClass/EType.h"

#ifndef __C_VIDEO_ENCODER_FILTER_H__
#define __C_VIDEO_ENCODER_FILTER_H__

#ifndef VERPC
typedef enum  _ENUM_DVD_VIDEO_ENCODER_OUTINPUT_INFO_TYPE{
	VIDEOENCODER_VideoGEN,
	VIDEOENCODER_VideoFrameInfo,
	VIDEOENCODER_TotalInfo
}ENUM_DVD_VIDEO_ENCODER_OUTPUT_INFO_TYPE;

typedef enum _ENUM_VIDEO_ENCODER_RC_MODE{
	VIDEOENCODER_CBR,
	VIDEOENCODER_VBR,
	VIDEOENCODER_CVBR,
	VIDEOENCODER_TotalRCMode
}ENUM_VIDEO_ENCODER_RC_MODE;

/* The VIDEOENCODER_VideoGEN is sent out once to the down stream
	filter with Output Pin's API DeliverPrivateInfo() after 
	the Start() is finished. */
typedef struct  _VideoElemGenInfo {
	/* Should be set to VIDEOENCODER_VideoGEN */
    ENUM_DVD_VIDEO_ENCODER_OUTPUT_INFO_TYPE infoType;

	/* Videe Input Source(Analog, DV or MPEG Decoder) */
	/* Can be MEDIATYPE_AnalogVideo, MEDIATYPE_Interleaved, or 
	MEDIATYPE_MPEG2_PES */
    ENUM_MEDIA_TYPE videoInputSource;

	/* Video Encoder type (MPEG1 or MPEG2). */
	/* Can be MEDIASUBTYPE_MPEG2_VIDEO, MEDIASUBTYPE_MPEG1Video */
    ENUM_MEDIA_TYPE encoderType;

	/* Video format(NTSC/PAL) */
	/* Can be MEDIASUBTYPE_AnalogVideo_NTSC or MEDIASUBTYPE_AnalogVideo_PAL */
    ENUM_MEDIA_TYPE TVSystemFormat;

	/*Horizontal resolution in the encoded stream. */
    ENUM_VIDEO_HORIZONTAL_RESOLUTION horizontalResolusion; 

	/* Video Encoder rate control mode, i.e., CBR, VBR_CBG, VBR or CVBR. */
    ENUM_VIDEO_ENCODER_RC_MODE videoRCMode;

	/* The STD buffer size. Normally 229K bytes for MPEG2 and 46K bytes 
	for MPEG1 encoder. Application can change it to improve the video 
	quality, if necessary. */
    unsigned int video_STD_buffer_size;

	/* The init STD buffer size. Default is 95% of the STD buffer size. */
    unsigned int init_STD_buffer_bullness;

	/* Encoded video stream bit rate, bits per second. */
    unsigned int videoBitRate;

	/* The GOP structure fo M/N. 3/15 for NTSC and 3/12 for PAL by default. */
    unsigned char gop_M;
    unsigned char gop_N;

	/* the number of GOPs per VOBU. Normally one GOP per VOBU. */
    unsigned char numOfGOPsPerVOBU;
} VIDEO_ELEM_GEN_INFO;


/* The VIDEOENCODER_VideoFrameInfo is sent out to the down stream
	filter with Output Pin's API DeliverPrivateInfo() right before the data
	(IMediaSample). Normally one media sample contains one encoded video 
	frame data. */
typedef struct  _VideoElemFrameInfo {
	/* Should be set to  VIDEOENCODER_VideoFrameInfo. */
    ENUM_DVD_VIDEO_ENCODER_OUTPUT_INFO_TYPE infoType;

	/* The picture number in the display order, starting form 0; */
    unsigned int pictureNumber;

	/* The type of picture, i.e., I, B, or P */
    ENUM_VIDEO_FRAME_TYPE pictureType;

	/* The upper 32 bit of the 64 bit PTS. */
    unsigned int PTShigh;

	/* The lower 32 bit of the 64 bit PTS. */
    unsigned int PTSlow;

	/* The upper 32 bit of the 64 bit DTS. */
    unsigned int DTShigh;

	/* The lower 32 bit of the 64 bit DTS. */
    unsigned int DTSlow;

	/* The original VBI/WSS data for the current frame. */
    unsigned int VBIData;

	/* The STD fullness for the current video frame. */
    unsigned int VBVfullness;

	/* The number of fields for the current video frame, 2 or 3. */
    unsigned char numOfField;

	/*The current video frame is the Paused video frame, in display order. */
    unsigned char pausedVideoFrame;

	/* The current video frame is a key frame candidate or not. */
    unsigned char newVOBUStart;

	/* If the frame is I frame, NewVOBUStart will indicate a new 
	VOBU start or not. */
    unsigned char KeyFrame;    

	/* If EOS is at the head of the picture. May not be used. */
	unsigned char EOSatHead;
} VIDEO_ELEM_FRAME_INFO;

#endif
#endif

