//---------------------------------------------------------------------------
//File: CAudioEncoder.h
//
//Desc: Header file for DVD-VENUS Program Audio Encoder Filter Class 
//      interface define
//
//Author: 
//
//Copyright (c)RealTek.   All rights reserved.

//Just includes the defination of the info structure, which is needed by the 
//DVD Program Mux Filter.
#ifndef __C_AUDIO_ENCODER_TYPE_H__
#define __C_AUDIO_ENCODER_TYPE_H__

#ifndef AERPC
typedef enum  _ENUM_DVD_AUDIO_ENCODER_OUTPUT_INFO_TYPE{
	AUDIOENCODER_AudioGEN,
	AUDIOENCODER_AudioFrameInfo,
	AUDIOENCODER_TotalInfo
}ENUM_DVD_AUDIO_ENCODER_OUTPUT_INFO_TYPE;

/* The AUDIOENCODER_AudioGEN is sent out once to the down stream
	filter with Output Pin's API DeliverPrivateInfo() after 
	the Start() is finished. */
typedef struct  _AudioElemGenInfo {
    ENUM_DVD_AUDIO_ENCODER_OUTPUT_INFO_TYPE infoType;

	/* Audio encoder type, MPEG, AC3 or LPCM */
	/* Can be MEDIASUBTYPE_MPEG1ES, MEDIASUBTYPE_DVD_LPCM_AUDIO, 
	   MEDIASUBTYPE_DOLBY_AC3, or MEDIASUBTYPE_DOLBY_AC3_SPDIF */
    ENUM_MEDIA_TYPE audioEncoderType;

	/* This field indicates the compressed audio rate. 
	    For example, 256000 bps AC3 audio stream*/
    unsigned int audioRate;

	/* This field indicates the audio sampling frequency.
	   Currently only 48000 and 44100 sampling are supported by Mux filter */
    unsigned int samplingFreq;

	/* This field indicates the audio encoder encoding mode,
	   such as dual mono, stereo or mono. */
    unsigned int audioEncodingMode;
} AUDIO_ELEM_GEN_INFO;

/* The AUDIOENCODER_AudioFrameInfo is sent out to the down stream
	filter with Output Pin's API DeliverPrivateInfo() right before the data
	(IMediaSample). Normally one media sample contains one encoded audio 
	frame data. In case the audio frame is too small(see LPCM), one media 
	sample may contain multiple audio frame data. */
typedef struct  _AudioElemFrameInfo {
    ENUM_DVD_AUDIO_ENCODER_OUTPUT_INFO_TYPE infoType;

	/* The upper 32 bit of the 64 bit PTS corresponding to the first sample
	of the current audio frame */
    unsigned int PTShigh;

	/* The lower 32 bit of the 64 bit PTS corresponding to the first sample
	of the current audio frame */
    unsigned int PTSlow;

	/* The number for audio frames for the current media sample. */
    unsigned char numOfFrames;
} AUDIO_ELEM_FFRAME_INFO;

#endif
#endif

