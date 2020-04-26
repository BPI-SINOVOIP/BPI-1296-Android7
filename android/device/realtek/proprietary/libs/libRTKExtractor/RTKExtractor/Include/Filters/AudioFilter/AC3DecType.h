
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

#ifndef __AC3_DEC_TYPE_H__
#define __AC3_DEC_TYPE_H__

//************************************************************************
// Definitions.
//************************************************************************

// compression scale factors (examples)
#define COMP_SCALE_FULL ((long)0x7fffffff)
#define COMP_SCALE_HALF ((long)0x40000000)
#define COMP_SCALE_NONE ((long)0x00000000)

// error codes
#define ERR_BAD_SYNCINFO	0x00000001
#define ERR_BAD_BSI			0x00000002
#define ERR_BAD_CRC			0x00000004
#define ERR_BAD_EXPONENT	0x00000008
#define ERR_BAD_MANTISSA	0x00000010
#define ERR_BAD_BITALLOC	0x00000020
#define ERR_CFG_OUTPUT		0x00010000

// exponent strategy
enum 
{
    REUSE,
    D15,
    D25,
    D45 
};

// delta bit allocation strategy
enum 
{
    DELTA_REUSE,
    DELTA_NEW,
    DELTA_NONE,
    DELTA_RSVD 
};

// Dolby surround mode
enum 
{
    DSURMOD_NONE,
    DSURMOD_OFF,
    DSURMOD_ON,
    DSURMOD_RSVD 
};

// dual mono downmix mode
enum
{
	DUAL_MONO_MIX,
	DUAL_MONO_L,
	DUAL_MONO_R,
	DUAL_MONO_STEREO
};


// compression mode
enum
{
	COMP_CUSTOM_A,
	COMP_CUSTOM_D,
	COMP_LINEOUT,
	COMP_RF
};


// audio coding mode
enum 
{
    MODE_11,
    MODE_10,
    MODE_20,
    MODE_30,
    MODE_21,
    MODE_31,
    MODE_22,
    MODE_32 
};

#define MODE_SURROUND MODE_11

const int mode2chnum[8] =
{ 2, 1, 2, 3, 3, 4, 4, 5 };


// LFE output mode
enum
{
	LFE_OFF,
	LFE_ON
};


//************************************************************************
// Type Definitions.
//************************************************************************

typedef enum 
{
	ENUM_AUDIO_DEC_AC3_CFG,
} ENUM_AUDIO_DEC_AC3_INFO_TYPE;


typedef struct 
{
	unsigned short		mode;
	unsigned short		LFEMode;
	unsigned short		dualmono;
	unsigned short		comprMode;
	long				scalehi;
	long				scalelo;
} AUDIO_DEC_AC3_CFG;

typedef struct 
{ 
	ENUM_AUDIO_DEC_AC3_INFO_TYPE	infoType;
	AUDIO_DEC_AC3_CFG				cfg;
} AUDIO_DEC_AC3_CFG_INFO;


#endif
