//-----------------------------------------------------------------------------
// BDROM_NavigatorTypes_Internal.h
// Copyright (c) 2000 - 2004, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __BDROMNAVIGATORTYPESINTERNAL_H__
#define __BDROMNAVIGATORTYPESINTERNAL_H__

/// @file BDROM_NavigatorTypes_Internal.h
/// @brief Internal BDROM structures, enumerations and defines

#include "CommonTypes.h"
#include "DVDnUtilities.h"
//#include "time_utils.h"

// Doxygen documentation groups
//            group id					description
//            ---------------			---------------------------------------


/// @name BDROM Clip AV Stream level constants
//@{
// BDROM_EP_ENTRIES_PER_SP_SEGMENT
/// @brief Number of EP_map entries per SP Segment (BD+)
#define BDROM_EP_ENTRIES_PER_SP_SEGMENT				25
//@}

// BDROM_INVALID_PATH_ID
/// @brief Invalid Path Id
#define BDROM_INVALID_PATH_ID						0xFFFE

// BDROM_INVALID_SUBCLIP_ID
// Invalid SubClip_Id
#define BDROM_INVALID_SUBCLIP_ID					0xFFFE

// BDROM_INVALID_FONT_ID
// Invalid font_id
#define BDROM_INVALID_FONT_ID						0xFFFF

// Sound Ids
#define BDROM_SOUND_ID_NONE							0xFF

// BDROM_PLAYER_READ_SIZE
// Read size for the BDROM Nav. This is the number of bytes the player will
// read from disc each time its read() function is called
//
#define BDROM_PLAYER_READ_SIZE						61440			// 10 AUs. (i.e. 192 * 10 * 32)


// Invalid values for Media Sample metadata
// (for details on why these values have been chosen, see EMD_BD_DECRYPT_METADATA in \ThirdParty\Windows\CPSDK41\IEncryptedMetadataTypes.h)
#define BDROM_SAMPLEMD_INVALID_SPN					0xFFFFFFFF
#define BDROM_SAMPLEMD_INVALID_CLIP_ID				0x1FFFF
#define BDROM_SAMPLEMD_INVALID_PLAYITEM_ID			0x3FF
#define BDROM_SAMPLEMD_INVALID_SKB_ORDINAL			0x0



/// @class BDROM_RandomGen
/// @brief Random Number generator for the BDROM Modules. Generated random number using Linear Congruential Method.
///
class BDROM_RandomGen
{
private:
	// Arguments of the Linear Congruential Method, provided that
	// 1. 0 <= m_Mul < m_Mod;
	// 2. 0 <= m_Inc < m_Mod;
	// 3. m_Mul and m_Inc are relatively prime;
	// 4. m_Mul-1 is divisible by all prime factors of m_Mod;
	//   and
	// 5. m_Mul-1 is a multiple of 4 if m_Mod is a multiple of 4.
	//   Please refer to
	//       http://en.wikipedia.org/wiki/Linear_congruential_generator
	//   for more details.
	static const UInt32						m_Mod = 65536;					///< Modulus
	static const UInt32						m_Mul = 31489;					///< Multiplier
	static const UInt32						m_Inc = 12943;					///< Increment

public:
	// uiMin and uiMax are the min and max random number values to generate
	BDROM_RandomGen(UInt32 uiMin = 0, UInt32 uiMax = 0xFFFFFFFF)
	{
		SetRange(uiMin, uiMax);
	}

	void SetRange(UInt32 uiMin, UInt32 uiMax)
	{
		assert(uiMax >= uiMin);

		m_Min = uiMin;
		m_Max = uiMax;

		m_RangeSize = uiMax - uiMin + 1;

		// Reset random seed with system time
		m_Xn = ((UInt32)TickCount());
		m_Xn %= m_Mod; // convert to a value in [0 .. m_Mod-1]
	}

	// Return a random number n, such that uiMin <= n <= uiMax
	UInt32 Random(void)
	{
		UInt32 uiXn1;

		// Apply the Linear Congruential Method
		uiXn1  = ((m_Mul * m_Xn) + m_Inc) % m_Mod;
		m_Xn   = uiXn1; // save the random value for next use.

		// Convert result to [min .. max] range.
		// It's obvious that m_Min <= uiXn1.
		// And uiXn1 <= m_Max, since 
		// uiXn1 <= floor((m_Mod-1)*m_RangeSize/m_Mod)+m_Min
		//       <= floor(m_RangeSize-m_RangeSize/m_Mod)+m_Min
		//       <  m_RangeSize+m_Min
		//       =  m_Max+1, 
		//          where floor(x) denotes the integer part of a non-negative real number x.
		// This guarantees that the result will actually be in the min-max range.
		uiXn1 = m_Xn*m_RangeSize/m_Mod + m_Min;

		return uiXn1;
	}

private:

	UInt32								m_RangeSize;					///< uiMax - uiMin + 1, used in the mapping from [0 .. m_Mod-1] _onto_ [uiMin .. uiMax]
	UInt32								m_Xn;							///< Last random value. Initially the Starting value(or random seed). [ 0 <= m_Xn <= m_Mod-1 ]
	UInt32								m_Min;							///< lower limit of range of random number to be generated
	UInt32								m_Max;							///< upper limit of range of random number to be generated
};


/// @typedef BDROM_Stream_RendererId
/// @brief ID associated with streams attached to Renderer
///
typedef enum tagBDROM_Stream_RendererId
{
	kBDROM_Stream_RendererId_PrimaryVideo		= 0,	///< Primary Video stream.
	kBDROM_Stream_RendererId_PrimaryAudio		= 1,	///< Primary Audio stream.
	kBDROM_Stream_RendererId_SecondaryVideo		= 2,	///< Secondary Video stream, typically PiP.
	kBDROM_Stream_RendererId_SecondaryAudio		= 3		///< Secondary Audio stream.
} BDROM_Stream_RendererId;


/// @enum BDROM_PlayListPlaybackType
/// @brief Type of PlayList playback mode
///
typedef enum tagBDROM_PlayListPlaybackType
{
	kBDROM_PlayListPlaybackType__Unknown			= 0,
	kBDROM_PlayListPlaybackType__Sequential			= 1,
	kBDROM_PlayListPlaybackType__Random				= 2,
	kBDROM_PlayListPlaybackType__Shuffle			= 3
} BDROM_PlayListPlaybackType;

/// @enum BDROM_ConnectionCondition
/// @brief Connection condition between PlayItems/SubPlayItems
///
typedef enum tagBDROM_ConnectionCondition
{
	kBDROM_ConnctCond__Unknown				= 0,
	kBDROM_ConnctCond__NonSeamless			= 1,
	kBDROM_ConnctCond__Seamless_5			= 5,
	kBDROM_ConnctCond__Seamless_6			= 6
} BDROM_ConnectionCondition;

const int kBDROMPCS_ClockIDBase	= 10;

typedef enum tagBDAV_EBDROMPrecisionClock
{
	eBDROMPC_SystemTime			= kBDROMPCS_ClockIDBase + 1,	// Clock that keeps running always. It is used to determine PAUSE_DELTA when PtsClock is paused and resumed
	eBDROMPC_ElapsedTitleTime	= kBDROMPCS_ClockIDBase + 2,	// Clock that tracks elapsed time of a playlist. Is paused when main path is paused.
	eBDROMPC_TitleTime			= kBDROMPCS_ClockIDBase + 3,	// ???
	eBDROMPC_MainPathPts		= kBDROMPCS_ClockIDBase + 4,	// Clock that tracks the PTS for the main-path. Is paused when main path is paused.
	eBDROMPC_SecondaryTime		= kBDROMPCS_ClockIDBase + 5,	// Clock for secondary video/path ???
	eBDROMPC_AsyncIGPts			= kBDROMPCS_ClockIDBase + 6,	// Clock for Async IG streams
	eBDROMPC_AsyncSubPathPTS	= kBDROMPCS_ClockIDBase + 7,	// Clock for Async SubPaths (like Async PiP, and Async Audio for browsable slideshow)
	eBDROMPC_LastClockID
} EBDROMPrecisionClock;

// BDROM_FontDesciption
//	Structure to store details of Fonts
//
struct BDROM_FontDesciption
{
	UInt32			mFontId;			// Font Id as specified in Clip Info
	char			mFontFile[6];		// Font name as specified in Clip Info

	bool			mbFontLoaded;		// true if this font has been loaded
	std::string		mFontKey;			// Descriptor/Key for a loaded font (also to be set by caller)

	UInt8 *			mFontBuffer;		// pointer to memory where the font file data has been loaded
	UInt32			mFontBufferSize;	// Size of the font buffer

	BDROM_FontDesciption() : mFontId(BDROM_INVALID_FONT_ID), mbFontLoaded(false), mFontBuffer(0)		{ }
};

/// @typedef BDAV_SegmentAction
/// @brief Action to be taken when loading the next segment (Clip/PlayItem/PlayList/ATC Seq/STC Seq/Program Seq/SubPlayItem)
///
typedef enum tagBDAV_SegmentAction
{
	BDAV_SegmentAction_None,
	BDAV_SegmentAction_Load_Clip,
	BDAV_SegmentAction_Load_ProgSeq,
	BDAV_SegmentAction_Load_ATCSeq,
	BDAV_SegmentAction_Load_STCSeq,
	BDAV_SegmentAction_Load_SubPlayItem
} BDAV_SegmentAction;


typedef enum tagHDMV_NavCommand_Group
{
	kHDMV_NavCommand_Group_Branch = 0x00,
	kHDMV_NavCommand_Group_Compare = 0x01,
	kHDMV_NavCommand_Group_Set = 0x02
} HDMV_NavCommand_Group;

typedef enum tagHDMV_NavCommand_Branch
{
	kHDMV_NavCommand_Branch_Goto = 0x00,	// Program Control (Nop / Goto / Break)
	kHDMV_NavCommand_Branch_Jump = 0x01,	// Scenario Transition (Jump / Call / Resume)
	kHDMV_NavCommand_Branch_Play = 0x02		// Playback Execution (Play / Link)
} HDMV_NavCommand_Branch;

typedef enum tagHDMV_NavCommand_GoTo_Subgroup
{
	kHDMV_NavCommand_GoTo_Nop = 0x00,
	kHDMV_NavCommand_GoTo_GoTo = 0x01,
	kHDMV_NavCommand_GoTo_Break = 0x02
} HDMV_BHDMV_NavCommand_GoTo_Subgroup;

typedef enum tagHDMV_NavCommand_Jump_Subgroup
{
	kHDMV_NavCommand_Jump_JumpObject = 0x00,
	kHDMV_NavCommand_Jump_JumpTitle = 0x01,
	kHDMV_NavCommand_Jump_CallObject = 0x02,
	kHDMV_NavCommand_Jump_CallTitle = 0x03,
	kHDMV_NavCommand_Jump_Resume = 0x04
} HDMV_BHDMV_NavCommand_Jump_Subgroup;


typedef enum tagHDMV_NavCommand_Play_Subgroup
{
	kHDMV_NavCommand_Play_PlayPL = 0x00,
	kHDMV_NavCommand_Play_PlayPLatPI = 0x01,
	kHDMV_NavCommand_Play_PlayPLatMK = 0x02,
	kHDMV_NavCommand_Play_TerminatePL = 0x03,
	kHDMV_NavCommand_Play_LinkPI = 0x04,
	kHDMV_NavCommand_Play_LinkMK = 0x05,
} HDMV_BHDMV_NavCommand_Play_Subgroup;

typedef enum tagHDMV_NavCommand_Compare
{
	kHDMV_NavCommand_Compare_BC = 1,		///< Bitwise Compare
	kHDMV_NavCommand_Compare_EQ = 2,		///< Equal
	kHDMV_NavCommand_Compare_NE = 3,		///< Not Equal
	kHDMV_NavCommand_Compare_GE = 4,		///< Greater than or Equal to
	kHDMV_NavCommand_Compare_GT = 5,		///< Greater than
	kHDMV_NavCommand_Compare_LE = 6,		///< Less than or Equal to
	kHDMV_NavCommand_Compare_LT = 7			///< Less Than
} HDMV_NavCommand_Compare;

typedef enum tagHDMV_NavCommand_Set
{
	kHDMV_NavCommand_Set_Set = 0x00,
	kHDMV_NavCommand_Set_SetSystem = 0x01
} HDMV_NavCommand_Set;


typedef enum tagHDMV_NavCommand_Set_Subgroup
{
	kHDMV_NavCommand_Set_Move = 0x01,
	kHDMV_NavCommand_Set_Swap = 0x02,
	kHDMV_NavCommand_Set_Add = 0x03,
	kHDMV_NavCommand_Set_Sub = 0x04,
	kHDMV_NavCommand_Set_Mul = 0x05,
	kHDMV_NavCommand_Set_Div = 0x06,
	kHDMV_NavCommand_Set_Mod = 0x07,
	kHDMV_NavCommand_Set_Rnd = 0x08,
	kHDMV_NavCommand_Set_And = 0x09,
	kHDMV_NavCommand_Set_Or = 0x0A,
	kHDMV_NavCommand_Set_Xor = 0x0B,
	kHDMV_NavCommand_Set_BitSet = 0x0C,
	kHDMV_NavCommand_Set_BitClear = 0x0D,
	kHDMV_NavCommand_Set_ShiftLeft = 0x0E,
	kHDMV_NavCommand_Set_ShiftRight = 0x0F
} HDMV_NavCommand_Set_Subgroup;

// HDMV_NavCommand_SetSystem_Subgroup
/// @brief SetSystem Navigation Command opcodes
///
typedef enum tagHDMV_NavCommand_SetSystem_Subgroup
{
	kHDMV_NavCommand_SetSystem_SetStream			= 0x01,
	kHDMV_NavCommand_SetSystem_SetNVTimer			= 0x02,
	kHDMV_NavCommand_SetSystem_SetButtonPage		= 0x03,
	kHDMV_NavCommand_SetSystem_EnableButton			= 0x04,
	kHDMV_NavCommand_SetSystem_DisableButton		= 0x05,
	kHDMV_NavCommand_SetSystem_SetSecondaryStream	= 0x06,
	kHDMV_NavCommand_SetSystem_PopUpMenuOff			= 0x07,
	kHDMV_NavCommand_SetSystem_StillOn				= 0x08,
	kHDMV_NavCommand_SetSystem_StillOff				= 0x09,
	kHDMV_NavCommand_SetSystem_SetMessageBDplus		= 0x10,
	kHDMV_NavCommand_SetSystem_SetStatusBDplus		= 0x11
} HDMV_NavCommand_SetSystem_Subgroup;


//--------------------------------------------------------------------------
// Event
//
#ifdef WIN32

#include "WinEvent.h"

#define kBDROMNavEvent_INFINITE		INFINITE

class BDROMNavEvent : WinEvent
{
public:
	BDROMNavEvent() : WinEvent()						{}
	~BDROMNavEvent()									{}

	bool Set()											{ return WinEvent::Set() == TRUE; }
	bool Reset()										{ return WinEvent::Reset() == TRUE; }
	bool Pulse()										{ return false; }	// PulseEvent Win32 API is 'Unreliable' per Win32 documentation.

	bool Wait(UInt32 ms = kBDROMNavEvent_INFINITE)		{ return WinEvent::Wait(ms) == TRUE; }
};

#else // WIN32

#include "SonicCriticalSection.h"
#include "SonicSemaphore.h"

#define kBDROMNavEvent_INFINITE		0xFFFFFFFF

class BDROMNavEvent
{
public:
	BDROMNavEvent()										{}
	~BDROMNavEvent()									{}

	bool Set()											
	{
		SonicAutoCriticalSection csLock(&csEvtLock);
		Reset();
		semEvtBlocker.Signal();
		return true;
	}
	bool Reset()										
	{
		SonicAutoCriticalSection csLock(&csEvtLock);
		while (semEvtBlocker.Wait(0));	// try-wait till semaphore count is back to 0
		return true;
	}
	bool Pulse()										{ return false; }

	bool Wait(UInt32 ms = kBDROMNavEvent_INFINITE)		
	{
		if (ms == kBDROMNavEvent_INFINITE)
			semEvtBlocker.Wait();
		else
			semEvtBlocker.Wait(ms);
		return true;
	}

private:
	SonicCriticalSection	csEvtLock;
	SonicSemaphore			semEvtBlocker;
};
#endif // WIN32

#endif
