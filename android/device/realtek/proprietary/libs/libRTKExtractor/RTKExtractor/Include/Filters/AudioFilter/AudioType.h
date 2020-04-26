    

/**
///////////////////////////////////////////////////////////////////////////////
//	@file AudioType.h
//
//	@brief Some type definition for audio filters
//
//	@author Taro     08/30/2004
//
//	copyright (c)RealTek.   All rights reserved.
/////////////////////////////////////////////////////////////////////////////////
*/

#ifndef __AUDIO_TYPE_H__
#define __AUDIO_TYPE_H__


/////////////////////////////////////////////////////////////////////////////////
// Definitions.
/////////////////////////////////////////////////////////////////////////////////

#define DELETE(x)  { if(x!=NULL) delete x; x=NULL; }

#ifndef max
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b)    (((a) < (b)) ? (a) : (b))
#endif


//#define AUDIO_DEC_INPUT_BUF_SIZE (1024*30)
//#define AUDIO_DEC_OUTPUT_BUF_SIZE (1024*280)

#define AUDIO_UNDEFINED_AGENTID -1

#define AUDIO_DEC_INPUT_BUF_SIZE (32768)
// need to be multiple of 8, a transaction of DMA is 8 bytes
#define AUDIO_DEC_OUTPUT_BUF_SIZE (51200)  

#define AUDIO_DEC_OUTPIN	8

#define CH0 0
#define CH1 1
#define CH2 2
#define CH3 3
#define CH4 4
#define CH5 5
#define CH6 6
#define CH7 7
#define CH8 8
#define CH9 9

//************************************************************************
// Type definitions.
//************************************************************************

typedef enum
{
//	AUDIO_DECODER,
	AUDIO_ENCODER,
	AUDIO_MPEG_DECODER,
	AUDIO_AC3_DECODER,
	AUDIO_GENERAL_DECODER
		
} AUDIO_FILTER_TYPE;

///////////////////////////////////////////////////////////////

typedef struct 
{
	int								chnum;
	int								samplebit;
	int								samplerate;
} AUDIO_PCM_FORMAT;


// private info

typedef enum 
{
	ENUM_AUDIO_PCM_FORMAT,
	ENUM_AUDIO_FRAME_SIZE
} ENUM_AUDIO_INFO_TYPE;


typedef struct 
{
	ENUM_AUDIO_INFO_TYPE 			infoType;
	AUDIO_PCM_FORMAT				pcmFormat;
} AUDIO_INFO_PCM_FORMAT;


typedef struct 
{
	ENUM_AUDIO_INFO_TYPE 			infoType;
	int								frameSampleNum;
} AUDIO_INFO_FRAME_SAMPLE_NUM;


#endif // #ifndef __AUDIO_TYPE_H__
