
/**
/////////////////////////////////////////////////////////////////////////////
//	@file AC3DecType.h
//
//	@brief Some type definition for audio ac3
//
//	@author Taro     10/8/2004
//
//	copyright (c)RealTek.   All rights reserved.
/////////////////////////////////////////////////////////////////////////////
*/

#ifndef __MPEG_DEC_TYPE_H__
#define __MPEG_DEC_TYPE_H__


//************************************************************************
// Definitions.
//************************************************************************


//************************************************************************
// Type Definitions.
//************************************************************************

typedef enum 
{
	ENUM_AUDIO_DEC_MPEG_CFG,
} ENUM_AUDIO_DEC_MPEG_INFO_TYPE;


typedef struct 
{
	unsigned short		mode;
	unsigned short		LFEMode;
} AUDIO_DEC_MPEG_CFG;

typedef struct 
{ 
	ENUM_AUDIO_DEC_MPEG_INFO_TYPE	infoType;
	AUDIO_DEC_MPEG_CFG				cfg;
} AUDIO_DEC_MPEG_CFG_INFO;


#endif
