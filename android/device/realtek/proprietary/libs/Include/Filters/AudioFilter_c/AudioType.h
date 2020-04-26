

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

#if defined(WIN32)
typedef unsigned char		u_char;
typedef unsigned short		u_short;
typedef unsigned int		u_int;
typedef unsigned long		u_long;
typedef signed __int64		int64_t;
typedef unsigned __int64	u_int64_t;
#endif

#include "AudioRPCBaseDS_data.h"

/////////////////////////////////////////////////////////////////////////////////
// Definitions.
/////////////////////////////////////////////////////////////////////////////////

#ifdef DELETE
#undef DELETE
#endif

#define DELETE(x)  { if(x!=NULL) delete x; x=NULL; }

//#define AUDIO_DEC_INPUT_BUF_SIZE (1024*30)
//#define AUDIO_DEC_OUTPUT_BUF_SIZE (1024*280)

#define AUDIO_UNDEFINED_AGENTID -1

#define AUDIO_DEC_INPUT_BUF_SIZE (256*1024)
//[70KB explanation]: need to be multiple of 8, a transaction of DMA is 8 bytes
//increase DEC_OUTPUT_BUFSIZE to 70KB for HWSRC 4X downsample requirement.
//[74KB explanation]: fix bug#30026 (by yllin)
//-HWSRC 4X downsample, need 0x10000 bytes for prebuffering.
//-OGG set min(out_ring_size) requirement per frame = 0x2000 bytes.
//-to make sure OGG can fill out ring to 0x10000 bytes, the min(out_ring_size) = (0x12000)+1 bytes
//-so we set out ring size = 74*1024 = 0x12800 bytes.
#if IS_CHIP(SATURN)
#define AUDIO_DEC_OUTPUT_BUF_SIZE (74*1024)//(70*1024)(64*1024)
#else
#define AUDIO_DEC_OUTPUT_BUF_SIZE (64*1024)
#endif

#define AUDIO_PP_OUTPUT_BUF_SIZE (64*1024)
#define AUDIO_IN_OUTPUT_BUF_SIZE (64*1024)
#define AUDIO_AOUT_OUTPUT_BUF_SIZE (64*1024)

#if IS_CHIP(MARS) || IS_CHIP(JUPITER)|| IS_CHIP(SATURN)
#define AUDIO_MAX_CHNUM  8
#else
#define AUDIO_MAX_CHNUM  6
#endif

#define AUDIO_DEC_OUTPIN		AUDIO_MAX_CHNUM
#define AUDIO_PP_OUTPIN			AUDIO_MAX_CHNUM
#define AUDIO_IN_OUTPIN			2
#define AUDIO_AOUT_OUTPIN		4
#define AUDIO_PSEUDO_OUT_PIN	2



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
	ENUM_AUDIO_DEC_EXT_BS,
	ENUM_AUDIO_DEC_CFG,
	ENUM_AUDIO_ERROR_STATUS,
	ENUM_AUDIO_RPC_PRIVATE_INFO,

} ENUM_AUDIO_INFO_TYPE;


typedef struct
{
	ENUM_AUDIO_INFO_TYPE	infoType;
	int						exist;
} AUDIO_INFO_EXT_BS;

typedef struct
{
	ENUM_AUDIO_INFO_TYPE	infoType;
	AUDIO_DEC_CFG			cfg;
} AUDIO_INFO_DEC_CFG;


typedef struct
{
	ENUM_AUDIO_INFO_TYPE	infoType;
	long				err;
} AUDIO_INFO_ERR_STATUS;


///////////////////////////////////////////////////////////////

#endif // #ifndef __AUDIO_TYPE_H__
