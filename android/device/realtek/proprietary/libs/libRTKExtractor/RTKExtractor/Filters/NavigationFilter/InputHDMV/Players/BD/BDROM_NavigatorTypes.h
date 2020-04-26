//-----------------------------------------------------------------------------
// BDROM_NavigatorTypes.h
// Copyright (c) 2000 - 2004, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __BDROM_NAVIGATORTYPES_H__
#define __BDROM_NAVIGATORTYPES_H__

#include "CommonTypes.h"
#include "DVDnUtilities.h"
//#include "BDROM_PlaybackInfo.h"
#include <string>
#include <vector>

// Doxygen documentation groups
//            group id					description
//            ---------------			---------------------------------------
/// @defgroup grp_BDROM_uop				BDROM UOPs

/// @defgroup grp_BDROM_attr			BDROM Stream Attributes



// BDROM_MAX_DISC_PLAYLISTS
/// @brief Maximum number of PlayLists (Real and Virtual playlists) combined allowed on a BDROM disc.
#define BDROM_MAX_DISC_PLAYLISTS				200

// BDROM_MAX_DISC_CLIPS
/// @brief Maximum number of Clips (Clips and Bridge-clips combined) allowed on a BDROM disc.
#define BDROM_MAX_DISC_CLIPS					200

// BDROM_MAX_DISC_PLAYITEMS
/// @brief Maximum number of PlayItems allowed on a BDROM disc.
#define BDROM_MAX_DISC_PLAYITEMS				2000

// BDROM_MAX_DISC_PLAYLISTMARKS
/// @brief Maximum number of PlayListMarks in a PlayList.
#define BDROM_MAX_DISC_PLAYLISTMARKS			1400
//@}


/// @name BDROM PlayList level constants
//@{
// BDROM_MAX_PL_PLAYITEMS
/// @brief Maximum number of PlayItems in a Playlist.
#define BDROM_MAX_PL_PLAYITEMS				255

// BDROM_MAX_PL_SUBPLAYITEMS
/// @brief Maximum number of Sub-PlayItems in a Playlist.
#define BDROM_MAX_PL_SUBPLAYITEMS			10

// BDROM_MAX_PL_PLAYLISTMARKS
/// @brief Maximum number of PlayListMarks in a PlayList.
#define BDROM_MAX_PL_PLAYLISTMARKS			100
//@}


// BDROM_MAX_AUX_BDROM_DIRS
/// @name BDROM text field lengths
//@{
// BDROM_MAX_DIR_NAME_LEN
/// @brief Maximum length of a Directory's name
#define BDROM_MAX_DIR_NAME_LEN				255

// BDROM_MAX_PL_NAME_LEN
/// @brief Maximum length of a PlayList's name
#define BDROM_MAX_PL_NAME_LEN				255

// BDROM_MAX_PL_DETAIL_LEN
/// @brief Maximum length of a PlayList's detail
#define BDROM_MAX_PL_DETAIL_LEN				1200

// BDROM_MAX_PL_MARK_NAME
/// @brief Maximum length of a PlayList Mark's name
#define BDROM_MAX_PL_MARK_NAME				24

// BDROM_MAX_PL_CHANNEL_NAME
/// @brief Maximum length of a PlayList Mark's name
#define BDROM_MAX_PL_CHANNEL_NAME			20
//@}


/// @name BDROM playback and scan speed ranges
//@{
// BDROM_MIN_FORWARD_PLAY_SPEED
/// @brief Playback rate below which the Navigator switches to Backward Scan (i.e., it will start to skip over data).
#define BDROM_MIN_FORWARD_PLAY_SPEED			0.0

// BDROM_NORMAL_FORWARD_PLAY_SPEED
/// @brief Normal 1x Playback rate.
#define BDROM_NORMAL_FORWARD_PLAY_SPEED		1.0

// BDROM_MAX_FORWARD_PLAY_SPEED
/// @brief Playback rate beyond which the Navigator switches to Forward Scan (i.e., it will start to skip over data).
#define BDROM_MAX_FORWARD_PLAY_SPEED			2.0
//@}


/// @name BDROM Transport Stream related constants
//@{
// BDROM_MPEG2_TS_PACKET_LEN
/// @brief Size of a BDROM MPEG-2 Transport Stream packet
#define BDROM_MPEG2_TS_PACKET_LEN			192

// BDROM_TS_PACKET_IN_AU
/// @brief Number of TS packet in an AU (Aligned Unit)
#define BDROM_TS_PACKET_IN_AU				32

// BDROM_BYTES_IN_AU
/// @brief Number of bytes in an Aligned Unit
#define BDROM_BYTES_IN_AU					(BDROM_TS_PACKET_IN_AU * BDROM_MPEG2_TS_PACKET_LEN)

// BDROM_NULL_PACKET_PID
/// @brief PID of a Null TS Packet
#define	BDROM_NULL_PACKET_PID				0x1FFF

// BDROM_INVALID_COUNTINUITY_COUNTER
/// @brief Invalid/Unknown continuity_counter
#define BDROM_INVALID_COUNTINUITY_COUNTER	0xFF
//@}


/// @name BDROM PlayList related constants
//@{
// BDROM_INVALID_PLAYLIST_NUM
/// @brief Invalid playlist_num
#define BDROM_INVALID_PLAYLIST_NUM			0xFFFF

// BDROM_INVALID_PLAYLIST_ID
/// @brief Invalid playlist_id
#define BDROM_INVALID_PLAYLIST_ID			0xFFFE

// BDROM_INVALID_PLAYITEM_NUM
/// @brief Invalid playitem_num
#define BDROM_INVALID_PLAYITEM_NUM			0xFFFF

// BDROM_INVALID_PLAYITEM_ID
/// @brief Invalid playitem_id
#define BDROM_INVALID_PLAYITEM_ID			0xFFFE

// BDROM_INVALID_CHAPTER_NUM
/// @brief Invalid chapter number
#define BDROM_INVALID_CHAPTER_NUM			0xFFFF

// BDROM_INVALID_MARK_NUM
/// @brief Invalid mark number
#define BDROM_INVALID_MARK_NUM				0xFFFF

// BDROM_INVALID_MARK_NUM
/// @brief Invalid mark number
#define BDROM_INVALID_MARK_ID				(BDROM_INVALID_MARK_NUM - 1)

// BDROM_INVALID_MEDIA_TIME
/// @brief Invalid Media Time or Playlist Elapsed Time
#define BDROM_INVALID_MEDIA_TIME			0xFFFFFFFF

// BDROM_INVALID_ANGLE_NUM
/// @brief Invalid Angle Number
#define BDROM_INVALID_ANGLE_NUM				0xFFFF

// BDROM_INFINITE_STILL_DURATION
/// @brief Infinite Still Duration
#define BDROM_INFINITE_STILL_DURATION		0xFFFFFFFF
//@}

/// @name BDROM stream related constants
//@{
// BDROM_MAX_SECONDARY_VIDEO_STREAMS
/// @brief maximum number of secondary video streams in a playitem
#define BDROM_MAX_SECONDARY_VIDEO_STREAMS				32

// BDROM_MAX_SECONDARY_AUDIO_STREAMS
/// @brief maximum number of secondary audio streams in a playitem
#define BDROM_MAX_SECONDARY_AUDIO_STREAMS				32

// BDROM_PRIMARY_AUDIO_STREAM_NONE
/// @brief Indicates 'No Primary Audio Stream'
#define BDROM_PRIMARY_AUDIO_STREAM_NONE					0xFF

// BDROM_SECONDARY_AUDIO_STREAM_NONE
/// @brief Indicates 'No Secondary Audio Stream'
#define BDROM_SECONDARY_AUDIO_STREAM_NONE				0xFF

// BDROM_SECONDARY_VIDEO_STREAM_NONE
/// @brief Indicates 'No Secondary Video Stream'
#define BDROM_SECONDARY_VIDEO_STREAM_NONE				0xFF

// BDROM_PGTEXTST_STREAM_NONE
/// @brief Indicates 'No [PiP] PG/TextST Stream'
#define BDROM_PGTEXTST_STREAM_NONE						0xFFF
//@}

// Disc Type
//
typedef enum tagBDROM_DiscType
{
	kBDROM_DiscType__None,
	kBDROM_DiscType__BDROM,
	kBDROM_DiscType__AVCHD
} BDROM_DiscType;

// Disc Creator
//
typedef enum tagBDROM_DiscCreator
{
	kBDROM_DiscCreator__Unknown,
	kBDROM_DiscCreator__MyDVD
} BDROM_DiscCreator;

// Disc Protection info (AACS, BD+, ...etc)
//
typedef UInt32						BDROM_ProtectionType;

// flags
const BDROM_ProtectionType			kBDROM_ProtectionType__AACS		= 0x01;		// Disc is AACS Encrypted
const BDROM_ProtectionType			kBDROM_ProtectionType__BDplus	= 0x02;		// Disc is BD+ protected
const BDROM_ProtectionType			kBDROM_ProtectionType__SKB		= 0x04;		// Disc has SKB (applicable only if AACS Encrypted)

// Disc Info
//
struct BDROM_DiscInfo
{
	BDROM_DiscInfo()
		: mDiscType(kBDROM_DiscType__None)
		, mDiscCreator(kBDROM_DiscCreator__Unknown)
		, mProtectionType(0)
	{
	}

	void Clear(void)
	{
		mDiscType       = kBDROM_DiscType__None;
		mDiscCreator    = kBDROM_DiscCreator__Unknown;
		mProtectionType = 0;
	}

	BDROM_DiscType			mDiscType;
	BDROM_DiscCreator		mDiscCreator;
	BDROM_ProtectionType	mProtectionType;		// see flags for more details of disc protection
};

// Player Settings
//
typedef enum tagBDROM_PlayerSetting
{
	kBDROM_PSet__Region,
	kBDROM_PSet__Country,
	kBDROM_PSet__ParentalLevel,
	kBDROM_PSet__AudioLanguage,
	kBDROM_PSet__PGTextSTLanguage,
	kBDROM_PSet__MenuDescriptionLanguage,
	kBDROM_PSet__PlayerProfile,
	kBDROM_PSet__PlayerVersion,
	kBDROM_PSet__BUDARoot,
	kBDROM_PSet__ADARoot,
	kBDROM_PSet__JavaSuspend
} BDROM_PlayerSetting;


// Player Capability
//
typedef enum tagBDROM_PlayerCapability
{
	kBDROM_PCap__AudioPlayback,				// Audio Decoder Capabilities: For values, see flags below
	kBDROM_PCap__ParentalLevelChange,		// Parental Level Change:  For values, see flags below
	kBDROM_PCap__CountryCodeChange,			// Country Code Change: For values, see flags below
	kBDROM_PCap__LanguageCodeChange,		// Language code change:  For values, see flags below
	kBDROM_PCap__AACSDiscPlayback,			// AACS Disc Capability:  For values, see flags below
	kBDROM_PCap__AVCHDDiscPlayback,			// AVCHD Disc Capability:  For values, see flags below
	kBDROM_PCap__BDMVDiscPlayback,			// BDMV Disc Capability:  For values, see flags below
	kBDROM_PCap__DriveExclusiveAccess		// Drive Exclusive Access:  For values, see flags below
} BDROM_PlayerCapability;

// Player Profile
//
typedef enum tagBDROM_PlayerProfile
{
	eProfile1_0 = 0,
	eProfile1_1 = 1,
	eProfile2_0 = 3,
	eProfile3_0 = 8
} BDROM_PlayerProfile;

// Player Settings Data
//
struct BDROM_PlayerSettingData
{
	// flag values to indicate validity of fields
	static const UInt32 kUInt32DataValid		= 0x00000001;
	static const UInt32 kStringDataValid		= 0x00000002;


	// flags to indicate which fields are valid
	//
	UInt32			mFlags;

	// UInt32 member used for
	//		kBDROM_PSet__Region, kBDROM_PSet__Country, kBDROM_PSet__ParentalLevel,
	//		kBDROM_PSet__AudioLanguage, kBDROM_PSet__PGTextSTLanguage, kBDROM_PSet__MenuDescriptionLanguage,
	//		kBDROM_PSet__PlayerProile, kBDROM_PSet__PlayerVersion
	//
	UInt32			mUInt32Data;

	// string used for
	//		kBDROM_PSet__BUDARoot
	//
	std::string		mStringData;
};

// ParentalLevelChangeRequestCallback
// Apps must derive from this class and override ParentalLevelChangeRequest() to get callbacks when a BD-J xlet attempts to
// increase parental level.
//
class ParentalLevelChangeRequestCallback
{
public:
	ParentalLevelChangeRequestCallback() {}
	virtual ~ParentalLevelChangeRequestCallback() {}	

	// ParentalLevelChangeRequest: request to the app to authorize parental level change to 'newParentalLevel'
	// return true to allow change.
	virtual bool ParentalLevelChangeRequest(UInt32 newParentalLevel) = 0;
};

// DriveExclusiveAccessChangeCallback
// Apps must derive from this class and override its functions to get callbacks when Exclusive Access is acquired
// and released.
//
class DriveExclusiveAccessChangeCallback
{
public:
	DriveExclusiveAccessChangeCallback() {}
	virtual ~DriveExclusiveAccessChangeCallback() {}	

	// AcquiringExclusiveAccess
	//	Called just before acquiring Exclusive Access to the drive.
	// Parameters:
	//	driveNumber [ in] : drive number. 1 - 26 for drive A - Z.
	//
	virtual void AcquiringExclusiveAccess(UInt32 driveNumber) = 0;

	// ReleasingExclusiveAccess
	//	Called just before releasing Exclusive Access to the drive.
	// Parameters:
	//	driveNumber [ in] : drive number. 1 - 26 for drive A - Z.
	//
	virtual void ReleasingExclusiveAccess(UInt32 driveNumber) = 0;
};

typedef enum tagHDMV_CommandType
{
	// NOTE: These values are used both to identify the command group a given command falls into and as a mask for
	// translating internal command information to the external OperatorInfo listed below.
	kHDMV_Notify_NavCommand_Type_Branch = 0x1F0F0000,
	kHDMV_Notify_NavCommand_Type_Compare = 0x1F000F00,
	kHDMV_Notify_NavCommand_Type_Set = 0x1F00000F
} HDMV_CommandType;

typedef enum tagHDMV_Operand_Location
{
	kHDMV_Notify_NavCommand_Operand_PSR,
	kHDMV_Notify_NavCommand_Operand_GPR,
	kHDMV_Notify_NavCommand_Operand_Immediate
} HDMV_Operand_Location;

typedef enum tagHDMV_SetStream_Type
{
	kHDMV_Notify_NavCommand_SetStream_None,
	kHDMV_Notify_NavCommand_SetStream_Audio,
	kHDMV_Notify_NavCommand_SetStream_Video,
	kHDMV_Notify_NavCommand_SetStream_PGTextST,
	kHDMV_Notify_NavCommand_SetStream_IG,
	kHDMV_Notify_NavCommand_SetStream_Angle
} HDMV_SetStream_Type;

struct HDMV_OperandInfo
{
	HDMV_OperandInfo() { Reset(); }
	void Reset()
	{
		// Reset to default values
		mOperandValue = 0;
		mOperandLocation = kHDMV_Notify_NavCommand_Operand_Immediate;
		mOperandRegisterNumber = 0;
		mOperandStreamType = kHDMV_Notify_NavCommand_SetStream_None;
		mOperandDisplayGiven = false;
		mOperandDisplay = false;
		mOperandVideoSizeGiven = false;
		mOperandVideoSize = 0;
		mOperandPiPPGTextSTValidGiven = false;
		mOperandPiPPGTextSTValid = false;
	}

	UInt32					mOperandValue;				// The value in the operand
	HDMV_Operand_Location	mOperandLocation;			// The location of the operand--immediate, GPR or PSR
	UInt32					mOperandRegisterNumber;		// If GPR or PSR, the register number
	HDMV_SetStream_Type		mOperandStreamType;			// If a stream command, the type of stream
	bool					mOperandDisplayGiven;		// Indicates whether a display on/off value is provided
	bool					mOperandDisplay;			// Extra data used by some SetSystem commands, usually to turn display of a stream on or off
	bool					mOperandVideoSizeGiven;		// Indicates whether a video size is provided
	UInt32					mOperandVideoSize;			// Video size, if provided
	bool					mOperandPiPPGTextSTValidGiven; // Indicates whether a PiPPGTextSTValid flag value is provided
	bool					mOperandPiPPGTextSTValid;	// Flag indicating if PiP PGTextST Stream is used or if the PGTextST stream is enabled
};

typedef enum tagHDMV_OperatorInfo
{
	// These values are the result of ANDing one of the CommandType values with a 32 bit value containing
	// Subgroup and Operator information.

	// Branch--GoTo
	kHDMV_Notify_NavCommand_GoTo_Nop 						= 0x00000000,
	kHDMV_Notify_NavCommand_GoTo_GoTo 						= 0x00010000,
	kHDMV_Notify_NavCommand_GoTo_Break 						= 0x00020000,
	// Branch--Jump
	kHDMV_Notify_NavCommand_Jump_JumpObject 				= 0x01000000,
	kHDMV_Notify_NavCommand_Jump_JumpTitle 					= 0x01010000,
	kHDMV_Notify_NavCommand_Jump_CallObject 				= 0x01020000,
	kHDMV_Notify_NavCommand_Jump_CallTitle 					= 0x01030000,
	kHDMV_Notify_NavCommand_Jump_Resume 					= 0x01040000,
	// Branch--Play
	kHDMV_Notify_NavCommand_Play_PlayPL 					= 0x02000000,
	kHDMV_Notify_NavCommand_Play_PlayPLatPI 				= 0x02010000,
	kHDMV_Notify_NavCommand_Play_PlayPLatMK 				= 0x02020000,
	kHDMV_Notify_NavCommand_Play_TerminatePL 				= 0x02030000,
	kHDMV_Notify_NavCommand_Play_LinkPI 					= 0x02040000,
	kHDMV_Notify_NavCommand_Play_LinkMK 					= 0x02050000,
	// Compare
	kHDMV_Notify_NavCommand_Compare_BC 						= 0x08000100,	///< Bitwise Compare
	kHDMV_Notify_NavCommand_Compare_EQ 						= 0x08000200,	///< Equal
	kHDMV_Notify_NavCommand_Compare_NE 						= 0x08000300,	///< Not Equal
	kHDMV_Notify_NavCommand_Compare_GE 						= 0x08000400,	///< Greater than or Equal to
	kHDMV_Notify_NavCommand_Compare_GT 						= 0x08000500,	///< Greater than
	kHDMV_Notify_NavCommand_Compare_LE 						= 0x08000600,	///< Less than or Equal to
	kHDMV_Notify_NavCommand_Compare_LT 						= 0x08000700,	///< Less Than
	// Set--Set
	kHDMV_Notify_NavCommand_Set_Move 						= 0x10000001,
	kHDMV_Notify_NavCommand_Set_Swap 						= 0x10000002,
	kHDMV_Notify_NavCommand_Set_Add 						= 0x10000003,
	kHDMV_Notify_NavCommand_Set_Sub 						= 0x10000004,
	kHDMV_Notify_NavCommand_Set_Mul 						= 0x10000005,
	kHDMV_Notify_NavCommand_Set_Div 						= 0x10000006,
	kHDMV_Notify_NavCommand_Set_Mod							= 0x10000007,
	kHDMV_Notify_NavCommand_Set_Rnd							= 0x10000008,
	kHDMV_Notify_NavCommand_Set_And							= 0x10000009,
	kHDMV_Notify_NavCommand_Set_Or							= 0x1000000A,
	kHDMV_Notify_NavCommand_Set_Xor							= 0x1000000B,
	kHDMV_Notify_NavCommand_Set_BitSet						= 0x1000000C,
	kHDMV_Notify_NavCommand_Set_BitClear					= 0x1000000D,
	kHDMV_Notify_NavCommand_Set_ShiftLeft					= 0x1000000E,
	kHDMV_Notify_NavCommand_Set_ShiftRight					= 0x1000000F,
	// Set--SetSystem
	kHDMV_Notify_NavCommand_SetSystem_SetStream				= 0x11000001,
	kHDMV_Notify_NavCommand_SetSystem_SetNVTimer			= 0x11000002,
	kHDMV_Notify_NavCommand_SetSystem_SetButtonPage			= 0x11000003,
	kHDMV_Notify_NavCommand_SetSystem_EnableButton			= 0x11000004,
	kHDMV_Notify_NavCommand_SetSystem_DisableButton			= 0x11000005,
	kHDMV_Notify_NavCommand_SetSystem_SetSecondaryStream	= 0x11000006,
	kHDMV_Notify_NavCommand_SetSystem_PopUpMenuOff			= 0x11000007,
	kHDMV_Notify_NavCommand_SetSystem_StillOn				= 0x11000008,
	kHDMV_Notify_NavCommand_SetSystem_StillOff				= 0x11000009,
	kHDMV_Notify_NavCommand_SetSystem_SetMessageBDplus		= 0x1100000A,
	kHDMV_Notify_NavCommand_SetSystem_SetStatusBDplus		= 0x1100000B
} HDMV_OperatorInfo;

struct HDMV_CommandInfo
{
	void Clear()
	{
		mCommandType = kHDMV_Notify_NavCommand_Type_Branch;
		mOperator = kHDMV_Notify_NavCommand_GoTo_Nop;
		mOperands.clear();
	}

	HDMV_CommandType				mCommandType;		// In case client wants to organize or filter by type
	HDMV_OperatorInfo				mOperator;
	std::vector<HDMV_OperandInfo>	mOperands;
};

class NaviCommandCallback	// Derive from this class and override NaviCommandAvailable() to get callbacks when NaviCmds are executed
{
public:
	NaviCommandCallback() {}
	virtual ~NaviCommandCallback() {}	

	virtual void NaviCommandAvailable(HDMV_CommandInfo& commandInfo) = 0;
};

typedef UInt32 BDROM_PlayerCapabilityFlag;

// Audio Playback Capability Flags
//

// Flags for BDROM and AVCHD
const BDROM_PlayerCapabilityFlag	kBDROM_PCapFlag_AudioPlayback_LPCM_48_96_Stereo		= 0x0001;	// LPCM 48 & 96 KHz: Stereo Capable
const BDROM_PlayerCapabilityFlag	kBDROM_PCapFlag_AudioPlayback_LPCM_48_96_Surround	= 0x0002;	// LPCM 48 & 96 KHz: Surround Capable
const BDROM_PlayerCapabilityFlag	kBDROM_PCapFlag_AudioPlayback_LPCM_192_Incapable	= 0x0000;	// LPCM 192 KHz: Incapable
const BDROM_PlayerCapabilityFlag	kBDROM_PCapFlag_AudioPlayback_LPCM_192_Stereo		= 0x0004;	// LPCM 192 KHz: Stereo Capable
const BDROM_PlayerCapabilityFlag	kBDROM_PCapFlag_AudioPlayback_LPCM_192_Surround		= 0x0008;	// LPCM 192 KHz: Surround Capable

// Flags for AVCHD only
const BDROM_PlayerCapabilityFlag	kBDROM_PCapFlag_AudioPlayback_DD_Stereo				= 0x0010;	// Dolby Digital: Stereo Capable
const BDROM_PlayerCapabilityFlag	kBDROM_PCapFlag_AudioPlayback_DD_Surround			= 0x0020;	// Dolby Digital: Surround Capable

// Flags for BDROM and AVCHD
const BDROM_PlayerCapabilityFlag	kBDROM_PCapFlag_AudioPlayback_DDp_Indep_Stereo		= 0x0010;	// DD+ Independent Substream: Stereo Capable
const BDROM_PlayerCapabilityFlag	kBDROM_PCapFlag_AudioPlayback_DDp_Indep_Surround	= 0x0020;	// DD+ Independent Substream: Surround Capable
const BDROM_PlayerCapabilityFlag	kBDROM_PCapFlag_AudioPlayback_DDp_Dep_Incapable		= 0x0000;	// DD+ Dependent Substream: Incapable
const BDROM_PlayerCapabilityFlag	kBDROM_PCapFlag_AudioPlayback_DDp_Dep_Stereo		= 0x0040;	// DD+ Dependent Substream: Stereo Capable
const BDROM_PlayerCapabilityFlag	kBDROM_PCapFlag_AudioPlayback_DDp_Dep_Surround		= 0x0080;	// DD+ Dependent Substream: Surround Capable
const BDROM_PlayerCapabilityFlag	kBDROM_PCapFlag_AudioPlayback_DTSHD_Core_Stereo		= 0x0100;	// DTS-HD Core Substream: Stereo Capable
const BDROM_PlayerCapabilityFlag	kBDROM_PCapFlag_AudioPlayback_DTSHD_Core_Surround	= 0x0200;	// DTS-HD Core Substream: Surround Capable
const BDROM_PlayerCapabilityFlag	kBDROM_PCapFlag_AudioPlayback_DTSHD_Extn_Incapable	= 0x0000;	// DTS-HD Extension Substream: Incapable
const BDROM_PlayerCapabilityFlag	kBDROM_PCapFlag_AudioPlayback_DTSHD_Extn_Stereo		= 0x0400;	// DTS-HD Extension Substream: Stereo Capable
const BDROM_PlayerCapabilityFlag	kBDROM_PCapFlag_AudioPlayback_DTSHD_Extn_Surround	= 0x0800;	// DTS-HD Extension Substream: Surround Capable
const BDROM_PlayerCapabilityFlag	kBDROM_PCapFlag_AudioPlayback_DolbyL_DD_Stereo		= 0x1000;	// Dolby Lossless - Dolby Digital: Stereo Capable
const BDROM_PlayerCapabilityFlag	kBDROM_PCapFlag_AudioPlayback_DolbyL_DD_Surround	= 0x2000;	// Dolby Lossless - Dolby Digital: Surround Capable
const BDROM_PlayerCapabilityFlag	kBDROM_PCapFlag_AudioPlayback_DolbyL_MLP_Incapable	= 0x0000;	// Dolby Lossless - MLP: Incapable
const BDROM_PlayerCapabilityFlag	kBDROM_PCapFlag_AudioPlayback_DolbyL_MLP_Stereo		= 0x4000;	// Dolby Lossless - MLP: Stereo Capable
const BDROM_PlayerCapabilityFlag	kBDROM_PCapFlag_AudioPlayback_DolbyL_MLP_Surround	= 0x8000;	// Dolby Lossless - MLP: Surround Capable

// Parental level change Capability Flags
//
const BDROM_PlayerCapabilityFlag	kBDROM_PCapFlag_ParentalLevelChange_Prohibited		= 0;	// Not Allowed
const BDROM_PlayerCapabilityFlag	kBDROM_PCapFlag_ParentalLevelChange_Allowed			= 1;	// Allowed if application can get user to authorize it
const BDROM_PlayerCapabilityFlag	kBDROM_PCapFlag_ParentalLevelChange_Authorized		= 2;	// Authorize last requested parental level change
const BDROM_PlayerCapabilityFlag	kBDROM_PCapFlag_ParentalLevelChange_NotAuthorized	= 3;	// Authorize last requested parental level change

// Country Code change Capability Flags
//
const BDROM_PlayerCapabilityFlag	kBDROM_PCapFlag_CountryCodeChange_Prohibited	= 0;	// Not Allowed
const BDROM_PlayerCapabilityFlag	kBDROM_PCapFlag_CountryCodeChange_Allowed		= 1;	// Allowed

// Player Region Code change Capability Flags
//
const BDROM_PlayerCapabilityFlag	kBDROM_PCapFlag_RegionCodeChange_Prohibited		= 0;	// Not Allowed
const BDROM_PlayerCapabilityFlag	kBDROM_PCapFlag_RegionCodeChange_Allowed		= 1;	// Allowed

// Player Region Code change Capability Flags
//
const BDROM_PlayerCapabilityFlag	kBDROM_PCapFlag_LanguageCodeChange_Prohibited	= 0;	// Not Allowed
const BDROM_PlayerCapabilityFlag	kBDROM_PCapFlag_LanguageCodeChange_Allowed		= 1;	// Allowed

// AACS Disc Playback Capability Flags
//
const BDROM_PlayerCapabilityFlag	kBDROM_PCapFlag_AACSDiscPlayback_Prohibited		= 0;	// AACS protected disc playback not allowed
const BDROM_PlayerCapabilityFlag	kBDROM_PCapFlag_AACSDiscPlayback_Allowed		= 1;	// AACS protected disc playback allowed

// AVCHD Disc Playback Capability Flags
//
const BDROM_PlayerCapabilityFlag	kBDROM_PCapFlag_AVCHDDiscPlayback_Prohibited	= 0;	// AVCHD Disc playback not allowed
const BDROM_PlayerCapabilityFlag	kBDROM_PCapFlag_AVCHDDiscPlayback_Allowed		= 1;	// AVCHD Disc playback allowed

// BDMV Disc Playback Capability Flags
//
const BDROM_PlayerCapabilityFlag	kBDROM_PCapFlag_BDMVDiscPlayback_Prohibited		= 0;	// BDMV Disc playback not allowed
const BDROM_PlayerCapabilityFlag	kBDROM_PCapFlag_BDMVDiscPlayback_Allowed		= 1;	// BDMV Disc playback allowed

// Drive Exclusive Access Flags
//
const BDROM_PlayerCapabilityFlag	kBDROM_PCapFlag_DriveExclusiveAccess_Off		= 0;	// Player should NOT get exclusive access to drive
const BDROM_PlayerCapabilityFlag	kBDROM_PCapFlag_DriveExclusiveAccess_On			= 1;	// Player should get exclusive access to drive


// BDROM_UserEventCode
// Codes for User Events that can result in with Annex D User Operations and Key Events
// (shown here with associated User Opeartion and BDJ Key Code)
//
typedef enum tagBDROM_UserEventCode
{
	// User Event								Associated User Operation		Associated BDJ Key Code
	// ---------------------------------		----------------------------	-------------------------------
	kBDROM_UE__MenuCall,					//	MenuCall						-
	kBDROM_UE__TitleSearch,					//	TitleSearch						-
	kBDROM_UE__Resume,						//	Resume							-
	kBDROM_UE__PlayFirstPlaybackTitle,		//	PlayFirstPlaybackTitle			-
	kBDROM_UE__Stop,						//	Stop							kBDROM_VK__STOP
	kBDROM_UE__Play,						//	PlayFirst../PauseOff/StillOff	kBDROM_VK__PLAY
	kBDROM_UE__ChapterSearch,				//	ChapterSearch					-
	kBDROM_UE__TimeSearch,					//	TimeSearch						-
	kBDROM_UE__SkipToNextPoint,				//	SkipToNextPoint					kBDROM_VK__TRACK_NEXT
	kBDROM_UE__SkipToPreviousPoint,			//	SkipBackToPreviousPoint			kBDROM_VK__TRACK_PREV
	kBDROM_UE__Pause,						//	PauseOn/PauseOff				kBDROM_VK__PAUSE
	kBDROM_UE__StillOff,					//	StillOff						kBDROM_VK__STILL_OFF
	kBDROM_UE__ForwardPlay,					//	ForwardPlay						kBDROM_VK__FAST_FWD
	kBDROM_UE__BackwardPlay,				//	BackwardPlay					kBDROM_VK__REWIND
	kBDROM_UE__PrimaryAudioStreamChange,	//	PrimaryAudioStreamChange		-
	kBDROM_UE__AngleChange,					//	AngleChange						-
	kBDROM_UE__PGTextSTEnableDisable,		//	PGTextSTEnableDisable			kBDROM_VK__PG_TEXTST_ENABLE_DISABLE
	kBDROM_UE__PGTextSTStreamChange,		//	PGTextSTStreamChange			-
	kBDROM_UE__SecondaryVideoEnableDisable,	//	SecondaryVideoEnableDisable		kBDROM_VK__SECONDARY_VIDEO_ENABLE_DISABLE
	kBDROM_UE__SecondaryVideoStreamChange,	//	SecondaryVideoStreamChange		-
	kBDROM_UE__SecondaryAudioEnableDisable,	//	SecondaryAudioEnableDisable		kBDROM_VK__SECONDARY_AUDIO_ENABLE_DISABLE
	kBDROM_UE__SecondaryAudioStreamChange,	//	SecondaryAudioStreamChange		-
	kBDROM_UE__PiPPGTextSTStreamChange,		//	PiPPGTextSTStreamChange			-
	kBDROM_UE__TextSubtitleStyleChange,		//	TextSubtitleStyleChange			-
	kBDROM_UE__Up,							//	MoveUpSelectedButton			kBDROM_VK__UP
	kBDROM_UE__Down,						//	MoveDownSelectedButton			kBDROM_VK__DOWN
	kBDROM_UE__Left,						//	MoveLeftSelectedButton			kBDROM_VK__LEFT
	kBDROM_UE__Right,						//	MoveRightSelectedButton			kBDROM_VK__RIGHT
	kBDROM_UE__SelectButton,				//	SelectButton					-
	kBDROM_UE__Enter,						//	ActivateButton					kBDROM_VK__ENTER
	kBDROM_UE__SelectButtonAndActivate,		//	SelectButtonAndActivate			-
	kBDROM_UE__PopUpOn,						//	PopUpOn							kBDROM_VK__POPUP_MENU
	kBDROM_UE__PopUpOff,					//	PopUpOff						kBDROM_VK__POPUP_MENU
	kBDROM_UE__PopUpMenu,					//	PopUpOn/PopUpOff				kBDROM_VK__POPUP_MENU
	kBDROM_UE__ColoredKey,					//	-								kBDROM_VK__COLOR_KEY{0 - 3}
	kBDROM_UE__NumberKey,					//	-								kBDROM_VK__{0-9}

	kBDROM_UE__NumberOfEvents,				//  *NOT* and event. This is just to keep track of number of total number of Events
	kBDROM_UE__None				= 0xFFFF	//	Default
} BDROM_UserEventCode;

// BDROM_KeyEventCode
// Codes for Key Events (as specified in the BDROM Spec)
//
typedef enum tagBDROM_KeyEventCode
{
	kBDROM_VK__STOP,
	kBDROM_VK__PLAY,
	kBDROM_VK__TRACK_NEXT,
	kBDROM_VK__TRACK_PREV,
	kBDROM_VK__PAUSE,
	kBDROM_VK__STILL_OFF,
	kBDROM_VK__FAST_FWD,
	kBDROM_VK__REWIND,
	kBDROM_VK__PG_TEXTST_ENABLE_DISABLE,
	kBDROM_VK__SECONDARY_VIDEO_ENABLE_DISABLE,
	kBDROM_VK__SECONDARY_AUDIO_ENABLE_DISABLE,
	kBDROM_VK__UP,
	kBDROM_VK__DOWN,
	kBDROM_VK__LEFT,
	kBDROM_VK__RIGHT,
	kBDROM_VK__ENTER,
	kBDROM_VK__POPUP_MENU,
	kBDROM_VK__COLOR_KEY_0,
	kBDROM_VK__COLOR_KEY_1,
	kBDROM_VK__COLOR_KEY_2,
	kBDROM_VK__COLOR_KEY_3,
	kBDROM_VK__0,
	kBDROM_VK__1,
	kBDROM_VK__2,
	kBDROM_VK__3,
	kBDROM_VK__4,
	kBDROM_VK__5,
	kBDROM_VK__6,
	kBDROM_VK__7,
	kBDROM_VK__8,
	kBDROM_VK__9
} BDROM_KeyEventCode;

// BDROM_KeyState
// Key State for a Key Event
//
typedef enum tagBDROM_KeyState
{
	kBDROM_KS__KeyDown,								// Key Pressed
	kBDROM_KS__KeyUp								// Key Released
} BDROM_KeyState;

// BDROM_UserEventResponseType
// Types of possible responses to a user event
//
typedef enum tagBDROM_UserEventResponseType
{
	kBDROM_ResponseType_None,						// User Operation
	kBDROM_ResponseType_UserOperation,				// User Operation
	kBDROM_ResponseType_KeyEvent					// Key Event
} BDROM_UserEventResponseType;

// BDROM_UserEventResponse
// Response for a single user event
//
struct BDROM_UserEventResponse
{
public:
	BDROM_UserEventCode				mUserEvent;		//        User Event: _Play, _Up, _Number, etc
	BDROM_UserEventResponseType		mResponseType;	//  Type of Response: _UserOperation / _KeyEvent
	bool							mbMasked;		// Masked/Prohibited: true / false  (if false, this is a Masked Event or Prohibited User Operation)

public:
	BDROM_UserEventResponse() : mUserEvent(kBDROM_UE__None), mResponseType(kBDROM_ResponseType_None), mbMasked(true)	{ }
	void Reset(void)											{ mUserEvent = kBDROM_UE__None; mResponseType = kBDROM_ResponseType_None; mbMasked = true; }
};

// kNumUserEvents
// Number of User Events in User Event Response Table
//
const UInt32 kNumUserEvents					= (UInt32)kBDROM_UE__NumberOfEvents;

// BDROM_UserEventResponseTable
// Contains responses types for all relevant user events
//
struct BDROM_UserEventResponseTable
{
public:
	BDROM_UserEventResponse			mUserEventResponse[kNumUserEvents];	// user event responses

public:
	// Reset: clears table. All entries become undefined
	void Reset(void)		{ for (UInt32 i = 0; i < kNumUserEvents; i++)	mUserEventResponse[i].Reset(); }

	// Set: sets values for a given user action
	bool Set(BDROM_UserEventCode ec, BDROM_UserEventResponseType rt, bool m)
			{	if ((UInt32)ec < kNumUserEvents)
				{
					mUserEventResponse[ec].mUserEvent = ec;	mUserEventResponse[ec].mResponseType = rt;	mUserEventResponse[ec].mbMasked = m;
					return true;
				}
				return false;
			}

	// Get: gets values for a given user action
	bool Get(BDROM_UserEventCode ec, BDROM_UserEventResponseType & rt, bool & m)
			{	if ((UInt32)ec < kNumUserEvents)
				{
					rt = mUserEventResponse[ec].mResponseType;		m = mUserEventResponse[ec].mbMasked;
					return true;
				}
				else
				{
					rt = kBDROM_ResponseType_None;	m = false;
					return false;
				}
			}
	bool Get(UInt32 index, BDROM_UserEventResponse & uer)
			{	if (index < kNumUserEvents)
				{
					uer = mUserEventResponse[index];
					return true;
				}
				else
				{
					uer.Reset();
					return false;
				}
			}

	// SetResponseType: Sets 'Response Type' property of a User Event Response
	void SetResponseType(BDROM_UserEventCode ec, BDROM_UserEventResponseType rt)
			{	if ((UInt32)ec < kNumUserEvents)
					mUserEventResponse[ec].mResponseType = rt;
			}

	// SetResponseType: Sets 'Response Type' property of a User Event Response
	BDROM_UserEventResponseType GetResponseType(BDROM_UserEventCode ec)
			{	if ((UInt32)ec < kNumUserEvents)
					return mUserEventResponse[ec].mResponseType;
				return kBDROM_ResponseType_None;
			}

	// SetMasked: Sets masked/prohibited property of a User Event Response
	void SetMasked(BDROM_UserEventCode ec, bool m)
			{	if ((UInt32)ec < kNumUserEvents)
					mUserEventResponse[ec].mbMasked = m;
			}

	// IsMasked: returns masked/prohibited property of a User Event Response
	bool IsMasked(BDROM_UserEventCode ec)
			{	if ((UInt32)ec < kNumUserEvents)
					return mUserEventResponse[ec].mbMasked;
				return true;
			}

	// IsEqual: compares two sets of entries. Returns true if they are equal/false if they are not.
	bool IsEqual(BDROM_UserEventResponse & l, BDROM_UserEventResponse & r)
			{	return ((l.mUserEvent == r.mUserEvent) && (l.mResponseType == r.mResponseType) && (l.mbMasked == r.mbMasked)); }
};


/// @enum BDROM_PlayerState
/// @brief BDROM Player State
///
typedef enum tagBDROM_PlayerState
{
	kBDROM_PlayerState__NoDisc,					// No disc has been loaded
	kBDROM_PlayerState__Stop,					// Playback stopped
	kBDROM_PlayerState__Play,					// Playback (even if disc has been removed but Disc-Unbound Java apps are running)
	kBDROM_PlayerState__Pause,					// Paused
	kBDROM_PlayerState__Still,					// In a Still
	kBDROM_PlayerState__Scanning				// Scanning (fast forward or reverse)
} BDROM_PlayerState;

/// @enum BDROM_Region
/// @brief BDROM Region Code
///
typedef enum tagBDROM_Region
{
	kBDROM_Region_Unknown	= 0x00,
	kBDROM_Region_A			= 0x01,
	kBDROM_Region_B			= 0x02,
	kBDROM_Region_C			= 0x04
} BDROM_Region;

/// @struct BDROMDiscId
/// @brief 128 bit Disc Identifier and 32 bit Organization Identifier (which are in id.bdmv)
///
struct BDROMDiscId
{
	UInt32		mOrganizationId;		///< Organization_ID
	UInt32		mDiscId[4];				///< Disc_ID
};

/// @typedef BDROM_Domain
/// @brief BDROM Navigation Domains.
///
typedef enum tagBDROM_Domain 
{
	kBDROM_Domain__Stop,			///< Stop domain: playback is stopped.
	kBDROM_Domain__Menu,			///< Menu domain: Title 0 (Top Menu) is being played.
	kBDROM_Domain__FirstPlay,		///< FirstPlay domain: Title 0xFFFF (FirstPlay) is being played.
	kBDROM_Domain__Title,			///< Title domain: Any other Title being played.
} BDROM_Domain;

/// @typedef BDROM_ObjectType
/// @brief Type of BDROM TopMenu/FirstPlayback/Title Object - HDMV or BD-J.
///
typedef enum tagBDROM_ObjectType
{
	kBDROM_ObjectType__Reserved0	= 0,		///< Reserved
	kBDROM_ObjectType__HDMV			= 1,		///< HDMV type of object
	kBDROM_ObjectType__BDJ			= 2,		///< BD-J type of object
	kBDROM_ObjectType__Reserved3	= 3,		///< Reserved
	kBDROM_ObjectType__Unknown		= 0xFF		///< Unknown
} BDROM_ObjectType;

/// @enum BDROM_TitleType
/// @brief Type of HDMV/BDJ Title - Movie or Interactive
///
typedef enum tagBDROM_TitleType
{
	kBDROM_TitleType__HDMV_Movie			= 0,
	kBDROM_TitleType__HDMV_Interactive		= 1,
	kBDROM_TitleType__BDJ_Movie				= 2,
	kBDROM_TitleType__BDJ_Interactive		= 3,
	kBDROM_TitleType__Unknown				= 0xFF
} BDROM_TitleType;

/// @typedef BDROM_TitleAccessType
/// @brief Access Type of BDROM Title.
/// (Note1): If title number is 0 or 0xFFFF, the title number shall not be displayed on BD-ROM
///          Players user interface.
typedef enum tagBDROM_TitleAccessType
{
	kBDROM_TitleAccessType__Permitted					= 0,		///< Title Search (UO) into this title is permitted. A corresponding
																	///< title number value may be displayed on BD-ROM Players User
																	///< Interface except the case of (Note1) top.

	kBDROM_TitleAccessType__ProhibitedAndDisplay		= 2,		///< Title Search (UO) into this title is prohibited. A corresponding
																	///< title number value may be displayed on BD-ROM Players User
																	///< Interface except the case of (Note1) top.

	kBDROM_TitleAccessType__ProhibitedAndNotDisplay		= 3,		///< Title search(UO) into this title is prohibited. A corresponding
																	///< title number value shall not be displayed on BD-ROM Players
																	///< User Interface.

	kBDROM_TitleAccessType__Reserved					= 1,		///< Reserved
	kBDROM_TitleAccessType__Unknown						= 0xFF		///< Unknown
} BDROM_TitleAccessType;

/// @enum BDROM_ApplicationType
/// @brief Application_type defined in BDROM spec
///
typedef enum tagBDROM_ApplicationType
{
	kBDROM_ApplType__Reserved						= 0,

	// MainPath types
	kBDROM_ApplType__MainPath_Movie					= 1,
	kBDROM_ApplType__MainPath_TimeBasedSlideshow	= 2,
	kBDROM_ApplType__MainPath_BrowsableSlideshow	= 3,

	// SubPath types
	kBDROM_ApplType__SubPath_BrowsableSlideshow		= 4,
	kBDROM_ApplType__SubPath_IG_Menu				= 5,
	kBDROM_ApplType__SubPath_TextSubtitle			= 6,
	kBDROM_ApplType__SubPath_1orMore_ES				= 7
} BDROM_ApplicationType;

// Special Title, chapter and playitem numbers (not all defined by spec)
//
static UInt16 const kBDROM_TN_TopMenu		= 0;		///< Top Menu. (defined by BDROM spec)
static UInt16 const kBDROM_TN_FirstPlay		= 0xFFFF;	///< First Play. (defined by BDROM spec)
static UInt16 const kBDROM_TN_Resume		= 0xFFFE;	///< Indicates a Resume operation (initiated by BD-J app)
static UInt16 const kBDROM_TN_MinTitle		= 1;		///< Lowest Title number (defined by BDROM spec)
static UInt16 const kBDROM_TN_MaxTitle		= 999;		///< Lowest Title number (defined by BDROM spec)
static UInt16 const kBDROM_TN_Current		= 0xFEEE;	///< Current Title.
static UInt16 const kBDROM_TN_None			= 0xFDDD;	///< None (as would be when in Stop state)

/// @enum BDROM_PlayList_PlaybackType
/// @brief Playback type of a playlist
///
typedef enum tagBDROM_PlayList_PlaybackType
{
	kBDROM_PL_PlayType__Reserved		= 0,
	kBDROM_PL_PlayType__Sequential		= 1,
	kBDROM_PL_PlayType__Random			= 2,
	kBDROM_PL_PlayType__Shuffle			= 3
} BDROM_PlayList_PlaybackType;

/// @typedef BDROM_CharSet
/// @brief BDROM Character Set defines. All other values are reserved for future use.
///
typedef enum tagBDROM_CharSet
{
	kBDROM_CharSet_Reserved				= 0x00,		///< Reserved
	kBDROM_CharSet_Unicode_2_UTF8		= 0x01,		///< Character Set: Unicode 2.0 (ISO 10646-1), Character Encoding Scheme: UTF8
	kBDROM_CharSet_Unicode_2_UTF16_BE	= 0x02,		///< Character Set: Unicode 2.0 (ISO 10646-1), Character Encoding Scheme: UTF16 Big-Endian
	kBDROM_CharSet_Shift_JIS_Japanese	= 0x03,		///< Character Set: Shift JIS (Japanese)
	kBDROM_CharSet_KSC_5601_Korean		= 0x04,		///< Character Set: KSC 5601-1987 incl KSC 5653 for roman character (Koream), Character Encoding Scheme: EUC-KR
	kBDROM_CharSet_GB18030_Chinese		= 0x05,		///< Character Set: GB18030-2000 (Chinese)
	kBDROM_CharSet_GB2312_Chinese		= 0x06,		///< Character Set: GB2312 (Chinese), Character Encoding Scheme: CN-GB
	kBDROM_CharSet_BIG5_Chinese			= 0x07		///< Character Set: BIG5 (Chinese)
} BDROM_CharSet;

/// @typedef BDROM_MarkType
/// @brief BDROM Menu and Mark types.
///

typedef enum tagBDROM_MarkType
{
	// Mark Type used a default value / initializer
	kBDROM_MarkType_Invalid				= 0,

	// Mark Types that are defined in the spec. A PlayList Mark can be one of these two.
	kBDROM_MarkType_EntryMark			= 1,	
	kBDROM_MarkType_LinkPoint			= 2,

	// Mark Types defined by usage. Nav Cmds can cause jump to any mark (LinkMark), but user operations are limited to entry marks (ChapterMark)
	kBDROM_MarkType_ChapterMark			= kBDROM_MarkType_EntryMark,
	kBDROM_MarkType_LinkMark			= (kBDROM_MarkType_EntryMark | kBDROM_MarkType_LinkPoint)
} BDROM_MarkType;

/// @typedef BDROM_UOP
/// @brief List of all BDROM UOPs (i.e. BDROM_UOP_Flags).
/// @ingroup grp_BDROM_uop
///
#define BDROM_UOP_PROHIBITED				1
#define BDROM_UOP_ALLOWED					0


/// @name Defines for default/invalid values not defined by BDROM spec
//@{
// BDROM_AUDIO_BITS_PER_SAMPLE_NOINFO
/// No information about bits-per-audio-sample
#define BDROM_AUDIO_BITS_PER_SAMPLE_NOINFO		0xFF
//@}


/// @typedef BDROM_StreamCodingTypes
/// @brief BDROM Audio and Video stream encoding types. All other values are 'reserved for future use'
/// @ingroup grp_BDROM_attr
///
typedef enum tagBDROM_StreamCodingTypes
{
	kBDROM_StreamCodingType_Unknown						= 0x00,
//	kBDROM_StreamCodingType_MPEG1Video					= 0x01,

	// Primary/Secondary Video
	//
	kBDROM_StreamCodingType_MPEG2Video					= 0x02,		
	kBDROM_StreamCodingType_MPEG4AVCVideo				= 0x1B,
	kBDROM_StreamCodingType_VC1Video					= 0xEA,

//	kBDROM_StreamCodingType_MPEG1Audio					= 0x03,		
//	kBDROM_StreamCodingType_MPEG2Audio					= 0x04,		
//	kBDROM_StreamCodingType_13818Private				= 0x05,		
//	kBDROM_StreamCodingType_SubTitle					= 0x06,		
//	kBDROM_StreamCodingType_MHEG						= 0x07,		
//	kBDROM_StreamCodingType_DSM							= 0x08,	
//	kBDROM_StreamCodingType_H_222_1						= 0x09,		
//	kBDROM_StreamCodingType_13818_6_A					= 0x0A,		
//	kBDROM_StreamCodingType_13818_6_B					= 0x0B,		
//	kBDROM_StreamCodingType_13818_6_C					= 0x0C,		
//	kBDROM_StreamCodingType_13818_6_D					= 0x0D,		
//	kBDROM_StreamCodingType_13818_1_Aux					= 0x0E,		
//	kBDROM_StreamCodingType_MPEG2AudioAAC				= 0x0F,		

	// Primary Audio
	//
	kBDROM_StreamCodingType_LPCMAudio					= 0x80,		
	kBDROM_StreamCodingType_AC3Audio					= 0x81,
	kBDROM_StreamCodingType_DTSAudio					= 0x82,
	kBDROM_StreamCodingType_DolbyLosslessAudio			= 0x83,
	kBDROM_StreamCodingType_DolbyPlusAudio				= 0x84,
	kBDROM_StreamCodingType_DTS_HDAudio_exceptXLL		= 0x85,
	kBDROM_StreamCodingType_DTS_HDAudio_XLL				= 0x86,

	// Secondary Audio
	//
	kBDROM_StreamCodingType_DolbyPlus_SecondaryAudio	= 0xA1,
	kBDROM_StreamCodingType_DTS_HD_SecondaryAudio		= 0xA2,

	// PG, IG and TextST
	//
	kBDROM_StreamCodingType_PresentationGraphics		= 0x90,
	kBDROM_StreamCodingType_InteractiveGraphics			= 0x91,
	kBDROM_StreamCodingType_TextSubtitle				= 0x92

//	kBDROM_StreamCodingType_Tip							= 0xC3
} BDROM_StreamCodingTypes;

/// @typedef BDROM_AudioPresentationType
/// @brief BDROM audio presentation types. All other values are 'reserved for future use'
/// @ingroup grp_BDROM_attr
///
typedef enum tagBDROM_AudioPresentationType
{
	/// Single Mono channel, or
	/// Single Mono channel for both core substream and extension substream of DTS-HD, or 
	/// Single Mono channel for both Dolby Digital audio and MLP audio stream of the Dolby Lossless audio stream 
	kBDROM_AudioPres_SingleMono				= 1,

	kBDROM_AudioPres_Reserved_1				= 2,	///< Reserved

	/// Stereo (2-channel), or
	/// Stereo (2-channel) for both core substream and extension substream of DTS-HD, or 
	/// Stereo (2-channel) for both Dolby Digital audio and MLP audio stream of the Dolby Lossless audio stream 
	kBDROM_AudioPres_Stereo					= 3,

	kBDROM_AudioPres_Reserved_2				= 4,	///< Reserved
	kBDROM_AudioPres_Reserved_3				= 5,	///< Reserved

	/// Multi-channel, or
	/// Multi-channel for both core substream and extension substream of DTS-HD, or 
	/// Multi-channel for both Independent Substream-0 and Dependent Substream-0 of the Dolby Digital Plus audio stream
	/// Multi-channel for both Dolby Digital audio and MLP audio stream of the Dolby Lossless audio stream 
	kBDROM_AudioPres_MultiChannel			= 6,

	kBDROM_AudioPres_Reserved_4				= 7,	///< Reserved
	kBDROM_AudioPres_Reserved_5				= 8,	///< Reserved
	kBDROM_AudioPres_Reserved_6				= 9,	///< Reserved
	kBDROM_AudioPres_Reserved_7				= 10,	///< Reserved
	kBDROM_AudioPres_Reserved_8				= 11,	///< Reserved

	/// Stereo (2-channel) for core substream and Multichannel for extension substream of DTS-HD, or 
	/// Stereo (2-channel) for Dolby Digital audio and Multichannel for MLP audio stream of the Dolby Lossless audio stream 
	kBDROM_AudioPres_Stereo_or_Multichannel	= 12,

	kBDROM_AudioPres_Reserved_9				= 13,	///< Reserved
	kBDROM_AudioPres_Reserved_10			= 14,	///< Reserved
	kBDROM_AudioPres_NoInfo					= 15	///< Reserved
} BDROM_AudioPresentationType;

/// @typedef BDROM_AudioSamplingFreq
/// @brief BDROM audio sampling freqencies. All other values are 'reserved for future use'
/// @ingroup grp_BDROM_attr
///

typedef enum tagBDROM_AudioSamplingFreq
{
	kBDROM_AudioSamplingFreq_48KHz		= 1,
//	kBDROM_AudioSamplingFreq_44_1KHz		= 2,
//	kBDROM_AudioSamplingFreq_32KHz		= 3,
	kBDROM_AudioSamplingFreq_96KHz		= 4,
	kBDROM_AudioSamplingFreq_192KHz		= 5,
	kBDROM_AudioSamplingFreq_NoInfo		= 15
} BDROM_AudioSamplingFreq;

/// @typedef BDROM_AudioChannels
/// @brief Number of Audio channels.
/// @ingroup grp_BDROM_attr
///
typedef enum tagBDROM_AudioChannels
{
	kBDROM_AudioChannels_reserved		= 0,	///< reserved value.
	kBDROM_AudioChannels_1				= 1,	///< 1 Channel
	kBDROM_AudioChannels_2				= 2,	///< 2 Channels
	kBDROM_AudioChannels_3				= 3,	///< 3 Channels
	kBDROM_AudioChannels_4				= 4,	///< 4 Channels
	kBDROM_AudioChannels_5				= 5,	///< 5 Channels
	kBDROM_AudioChannels_6				= 6,	///< 5.1 / 6 Channels
	kBDROM_AudioChannels_7				= 7,	///< 7 Channels - Is this Possible/Valid?
	kBDROM_AudioChannels_8				= 8,	///< 7.1 / 8 Channels
	kBDROM_AudioChannels_Unknown			= 0xFF,	///< Number of channels is unknown
} BDROM_AudioChannels;

/// @typedef BDROM_AudioType
///
typedef enum tagBDROM_AudioSource
{
	kBDROM_InteractiveAudio = 1,
	kBDROM_OverallAudio = 2,
	kBDROM_PrimaryAudio = 3,
	kBDROM_SecondaryAudio = 4
} BDROM_AudioSource;

/// @typedef BDROM_VideoFormat
/// @brief Video format (480i/480p/...). All other values are 'reserved for future use'
/// @ingroup grp_BDROM_attr
///
typedef enum tagBDROM_VideoFormat
{
	kBDROM_VideoFormat_480i		= 1,	///< 480i [ ITU-R BT.601-4 ]
	kBDROM_VideoFormat_576i		= 2,	///< 576i [ ITU-R BT.601-4 ]
	kBDROM_VideoFormat_480p		= 3,	///< 480p [ SMPTE 293M ]
	kBDROM_VideoFormat_1080i	= 4,	///< 1080i [ SMPTE 274M ]
	kBDROM_VideoFormat_720p		= 5,	///< 720p [ SMPTE 296M ]
	kBDROM_VideoFormat_1080p	= 6,	///< 1080 [ SMPTE 274M ]
	kBDROM_VideoFormat_576p		= 7,	///< 576p [ ITU-R BT.1358 ]
	kBDROM_VideoFormat_NoInfo	= 15	///< No information.
} BDROM_VideoFormat;


/// @typedef BDROM_VideoFrameRate
/// @brief BDROM video frame rate. All other values are 'reserved for future use'
/// @ingroup grp_BDROM_attr
///

typedef enum tagBDROM_VideoFrameRate
{
	kBDROM_VideoFrameRate_Reserved1 = 0,	///< Reserved
	kBDROM_VideoFrameRate_23_976	= 1,	///< 23.976... fps
	kBDROM_VideoFrameRate_24		= 2,	///< 24 fps
	kBDROM_VideoFrameRate_25		= 3,	///< 25 fps
	kBDROM_VideoFrameRate_29_97		= 4,	///< 29.97... fps
	kBDROM_VideoFrameRate_Reserved2 = 5,	///< Reserved
	kBDROM_VideoFrameRate_50		= 6,	///< 50 fps
	kBDROM_VideoFrameRate_59_94		= 7,	///< 59.94 fps
	kBDROM_VideoFrameRate_NoInfo	= 15	///< Reserved
} BDROM_VideoFrameRate;


/// @typedef BDROM_VideoAspectRatio
/// @brief BDROM video aspect ratio. All other values are 'reserved for future use'
/// @ingroup grp_BDROM_attr
///
typedef enum tagBDROM_VideoAspectRatio
{
	kBDROM_VideoAspectRatio_4_3		= 2,	///< 4:3 aspect ratio.
	kBDROM_VideoAspectRatio_16_9	= 3,	///< 16:9 aspect ratio.
	kBDROM_VideoAspectRatio_2_21_1	= 4,	///< 2.21:1 aspect ratio.
	kBDROM_VideoAspectRatio_NoInfo	= 15,	///< No information.
} BDROM_VideoAspectRatio;


/// @typedef BDROM_PlayList_EntryPointType
/// @brief A PlayList entry point type. See tagBDROM_PlayList_EntryPointType
///
typedef enum tagBDROM_PlayList_EntryPointType
{
	kBDROM_PlayListEntryPointType_Any,			///< Any kind of entry point.
	kBDROM_PlayListEntryPointType_PlayItem,		///< A PlayItem.
	kBDROM_PlayListEntryPointType_PlayListMark	///< A Mark.
} BDROM_PlayList_EntryPointType;

/// @typedef BDROM_SequenceType
/// @brief Sequence Types
///
typedef enum tagBDROM_SequenceType
{
	kBDROM_SequenceType_ATC,			///< STC Sequence.
	kBDROM_SequenceType_STC,			///< ATC Sequence.
	kBDROM_SequenceType_Program		///< Program Sequence.
} BDROM_SequenceType;

/// @struct BDROM_Dir_ResumeInfo
/// @brief BDROM Directory Resume information.
///
struct BDROM_Dir_ResumeInfo
{
	bool	bResumeValid;				///< Indicates if resume is possible
	char	resumePlayListName[11];		///< Name of PlayList file in which playback will resume
};

/// @struct BDROM_PL_MarkInfo
/// @brief Information about a valid PlayList Mark.
///



struct BDROM_PL_MarkInfo
{
	BDROM_MarkType		markType;	
	UInt32				uiMarkPlayItem;	
	
	bool				bTimestampValid;		///< Indicates that the uiMarkTimestamp field is valid.
	DVDTimeStampType	tsMarkTimeStamp;		///< Timestamp of the mark.
	UInt32				uiMarkTimestamp;		///< Timestamp of the mark. This field is NOT obsolete, it provides better accuracy than the needless conversion to and from DVDTimeStampType.
	bool				bDurationValid;			///< Indicates that the uiMarkDuration field is valid.
	DVDTimeStampType	tsMarkDuration;
	UInt32				uiMarkDuration;		
	UInt32				uiEntry_ES_PID;
};



/// @struct BDROM_PL_ResumeInfo
/// @brief PlayList's Resume information.
///
struct BDROM_PL_ResumeInfo
{
	bool				bResumeValid;	///< Indicates if resume is possible.
	BDROM_PL_MarkInfo	resumeMark;		///< Resume Mark that contains information about the resume location.
};

/// @struct BDROM_PL_AppInfo
/// @brief PlayList's Application information
///
struct BDROM_PL_AppInfo
{
	DVDTimeStampType		plDuration;			///< PlayList's duration.
};

/// BDROM_PL_Info
/// @brief PlayList's Id, number of playitems and number of subplayitems.
struct BDROM_PL_PLInfo
{
	UInt32 uiPlayListId;
	UInt32 uiNumOfPlayItems;
	UInt32 uiNumOfSubPlayItems;
};

/// @struct BDROM_Location
/// @brief BDROM Location description
///
struct BDROM_Location
{
	BDROM_Domain	domain;				///< BDROM Domain.

	UInt32			uiPlayListNum;		///< PlayList number. Valid only if domain is not BDROM_Domain_Stop.
	UInt32			uiPlayItemNum;		///< PlayItem number.
	UInt32			uiChapterNum;		///< Chapter number.
	bool			bElapsedTimeValid;	///< Indicates if 'elapsedTime' is valid.
	Time90K			elapsedTime;		///< Elapsed time (in 45KHz) since start of PlayList. AKA media time.

	BDROM_Location()	{	Reset(); }
	void Reset(void)	{	domain = kBDROM_Domain__Stop;  uiPlayListNum = BDROM_INVALID_PLAYLIST_NUM;  uiPlayItemNum = BDROM_INVALID_PLAYITEM_NUM;
							uiChapterNum  = BDROM_INVALID_CHAPTER_NUM;  bElapsedTimeValid = false;  elapsedTime = 0;
						}
};



/// @struct BDROM_AudioAttributes
/// @brief Attributes of a BDROM audio stream
/// @ingroup grp_BDROM_attr
///

typedef UInt8 BDROM_LanguageCode[3];

struct BDROM_AudioAttributes
{
	BDROM_StreamCodingTypes		codingType;
	BDROM_AudioPresentationType	presentationType;
	BDROM_AudioSamplingFreq		samplingFreq;
	BDROM_AudioChannels			audioChannels;
	UInt32						bitsPerSample;	
	BDROM_LanguageCode			language;	
};

/// @struct BDROM_ISRC
/// @brief ISRC of a BDROM stream
/// @ingroup grp_BDROM_attr
///
struct BDROM_ISRC
{
	char country_code[2];
	char copyright_holder[3];
	char recording_year[2];
	char recording_number[5];
};

/// @struct BDROM_GraphicsAttributes
/// @brief Attributes of a BDROM graphics stream
/// @ingroup grp_BDROM_attr
///
struct BDROM_GraphicsAttributes
{	
	BDROM_StreamCodingTypes	codingType;
	BDROM_CharSet			charCode;				///< only valid for TestST streams
	BDROM_LanguageCode		language;		
	BDROM_ISRC				isrc;		
};

/// @typedef BDROM_OutputStreamType
/// @brief Type of streams output by the Navigator / Playback Control Engine
///
typedef enum tagBDROM_OutputStreamType
{
	kBDROM_OutputStreamType__Unknown		= 0,	///< unspecified/unknown
	kBDROM_OutputStreamType__Main			= 1,	///< Main Path stream (Primary Video)
	kBDROM_OutputStreamType__Sub			= 2,	///< Sub Path stream (Secondary Video (PiP), or Primary Audio for browsable slideshow)
	kBDROM_OutputStreamType__SubPreload_1	= 3,	///< Sub Path stream for preloading (PiP/IG/PG/TextST/Audio subpath)
	kBDROM_OutputStreamType__SubPreload_2	= 4		///< Sub Path stream for preloading (PiP/IG/PG/TextST/Audio subpath)
} BDROM_OutputStreamType;

/// @def kBDROM_StreamNum__None
/// Stream Number indicating 'No Stream'
///
#define kBDROM_StreamNum__None				0

/// @typedef BDROM_StreamType
/// @brief Type of BDROM stream
/// @ingroup grp_BDROM_attr
///
typedef enum tagBDROM_StreamType
{
	kBDROM_StreamType_Unknown,			///< Unknown stream type.
	kBDROM_StreamType_PrimaryVideo,		///< Primary Video stream.
	kBDROM_StreamType_SecondaryVideo,	///< Secondary Video stream, typically PiP.
	kBDROM_StreamType_PrimaryAudio,		///< Primary Audio stream.
	kBDROM_StreamType_SecondaryAudio,	///< Secondary Audio stream.
	kBDROM_StreamType_IG,				///< Interactive Graphics stream.
	kBDROM_StreamType_PGTextST,			///< Presentation Graphics stream.
	kBDROM_StreamType_PiP_PGTextST,		///< Picture-in-Picture Presentation Graphics stream.
} BDROM_StreamType;

/// @struct BDROM_VideoAttributes
/// @brief Attributes of a BDROM video stream
/// @ingroup grp_BDROM_attr
///

struct BDROM_VideoAttributes
{
	BDROM_StreamCodingTypes		codingType;		
	BDROM_VideoFormat			videoFormat;
	BDROM_VideoFrameRate		frameRate;	
	BDROM_VideoAspectRatio		aspectRatio;	
	bool						bLine21Data;		
};

/// @struct BDROM_Stream
/// @brief Description of a BDROM stream in a MPEG-2 TS or DV clip
/// @ingroup grp_BDROM_attr
///
struct BDROM_Stream
{
	/// @name Stream identifying information
	//@{
	BDROM_StreamType			streamType;	///< Type of stream - audio/video. contents of the attribute section should be interpreted based on this type.
	UInt16						pid;		///< PID of the stream. Valid only if stream is in a MPEG-2 TS clip. for streams in DV clips, it is set to 0x1FFF (NULL PID)
	//@}

	/// @name Stream attributes Union
	/// @brief Union of possible stream attributes
	//@{
	union
	{
		BDROM_AudioAttributes		aatr;		///< Audio attributes. Valid only if streamType is Audio.
		BDROM_VideoAttributes		vatr;		///< Video attributes. Valid only if streamType is Video.
		BDROM_GraphicsAttributes	gatr;		///< Graphics attributes. Valid if streamType is IG, PG or Text Subtitle.
	};
	//@}
};

/// @typedef BDROM_PlayDirection
/// @brief BDROM Play Directions, i.e. Forward and Backward.
///
typedef enum tagBDROM_PlayDirection
{
	kBDROM_UnknownPlayDirection		= 0,
	kBDROM_ForwardPlay				= 1,
	kBDROM_BackwardPlay				= 2
} BDROM_PlayDirection;


//__________________________________________________________________________________________________________________________________________________
// BDROM Events
//

// BDROM Event codes are made up of a category and an event.
//       | Category        | Event           |
//       | byte 3 | byte 2 | byte 1 | byte 0 |
//
// Both the Category and the Event are 16 bit masks. Total number of events possible : 256.
//


//--------------------------------------------------------------------------
// Event Categories
//
typedef enum tagBDROM_EventCategory
{
	kBDROM_ECat__Playlist					= 0x0001,		// PlayList/PlayItem related events
	kBDROM_ECat__Title						= 0x0002,		// Title related events
	kBDROM_ECat__Time						= 0x0004,		// Time related events
	kBDROM_ECat__IG							= 0x0008,		// IG related events
	kBDROM_ECat__Dom						= 0x0010,		// Domain related events
	kBDROM_ECat__Disc						= 0x0020,		// Disc related events
	kBDROM_ECat__Prot						= 0x0040,		// Content Protection (AACS, BD+) related events
	kBDROM_ECat__UOP						= 0x0080,		// UOP related events
	kBDROM_ECat__Stream						= 0x0100,		// stream related events
	kBDROM_ECat__NaviCmd					= 0x0110,		// NaviCmds events
	kBDROM_ECat__PSR						= 0x0120,		// PSR events
	kBDROM_ECat__GPR						= 0x0140,		// GPR events
	kBDROM_ECat__PG							= 0x0180,		// PG related events
} BDROM_EventCategory;

// Event codes creation macro
//
#define MAKE_BDROM_EVENT(cat, ev)		((cat << 16) | ev)


// -----------------
// Fatal Error Codes - Major and Minor
typedef enum tagBDROM_FatalErrorMajorCode
{
	kBDROM_FatalErrMajor__DiscReadError,
	kBDROM_FatalErrMajor__PlaybackError,
} BDROM_FatalErrorMajorCode;

typedef enum tagBDROM_FatalErrorMinorCode
{
	// Minor code for kBDROM_FatalErrorMajor__DiscReadError
	kBDROM_FatalErrMinor__IndexFile,
	kBDROM_FatalErrMinor__MovieObjectFile,
	kBDROM_FatalErrMinor__PlayListFile,
	kBDROM_FatalErrMinor__ClipInfoFile,
	kBDROM_FatalErrMinor__StreamFile,
	kBDROM_FatalErrMinor__SoundFile,

	// Minor code for kBDROM_FatalErrMajor__PlaybackError
	kBDROM_FatalErrMinor__UnexpectedError,			// Unexpected/Unknown error.
	kBDROM_FatalErrMinor__PrimaryVMRConnectError,	// VMR for Primary Video connection/re-connection failed
	kBDROM_FatalErrMinor__SecondaryVMRConnectError,	// VMR for Secondary Video connection/re-connection failed
	kBDROM_FatalErrMinor__TitlePermissionError,
	kBDROM_FatalErrMinor__PlayItemLoadError,		//REALTEK_MODIFY
	kBDROM_FatalErrMinor__UnsupportFormatError,		//REALTEK_MODIFY
} BDROM_FatalErrorMinorCode;

// ----------------
// Disc Protection related Error/Warning codes
typedef enum tagBDROM_DiscProtectionFailureCode
{
	kBDROM_DPFC__Unknown,							// AACS/COPP/...: failed, but cause of failure is not known
	kBDROM_DPFC__CoppCertFailure,					// COPP: COPP cert in display driver is expired or revoked.
	kBDROM_DPFC__ContentProtectFailure,				// COPP: General COPP failure (see from NV drivers in Clone mode).
	//kBDROM_DPFC__AuthenticationModuleMissing,		// AACS: One (or more) modules/dlls needed for AACS Authentication are missing.
	kBDROM_DPFC__COPPNotSupportedInDriver,			// COPP: (Graphics) Driver does not support COPP, atleast not in this mode (e.g. Clone Mode).
	kBDROM_DPFC__NoHDCPOnDigitalOut,				// COPP: Digital Out has no HDCP capability
	kBDROM_DPFC__ImageConstrainedToken,             // COPP: ICT flag is set, will decimate teh image
    kBDROM_DPFC__DigitalOnlyToken,					// COPP: DOT flag set for this content
	res2, //kBDROM_DPFC__UnsecureProcess,			// removed - will never be seen by CinePlyer
	kBDROM_DPFC__OutputPinChanged,					// COPP diagnostic - ignroe in shipping product
	kBDROM_DPFC__UnknownOutput,						// COPP: Cannot detect output being used.
	kBDROM_DPFC__AACSAuthFailed,					// AACS: the AACS codes on the disc are not valid
	kBDROM_DPFC__NoCoppOnExtendedMonitor,			// COPP: Secondary Monitor has no COPP or COPP status unknown (assmuing unprotected)
	kBDROM_DPFC__DigitalRepeaterOnOutput,			// COPP: A digital repeater has been detected by COPP
	kBDROM_DPFC__IctAnalogNoDownRez,       			// COPP: ICT flag without down rez support (error)
    kBDROM_DPFC__AcpProtectFailed,                  // COPP: ACP analog copy protection failed.
    kBDROM_DPFC__CgmsaProtectFailed,                // COPP: Copy Generation Management System - Analog failed.
    kBDROM_DPFC__GuestUpdateFailed,                 // AACS: CRL/DRL update failed due to guest user restrictions
    kBDROM_DPFC__VldAccelerationCancelled,          // UAB: VLD Acceleration cancelled due to UAB protection failure.
    kBDROM_DPFC__CloneMode,                         // COPP: Detected clone mode on display
    kBDROM_DPFC__UnsecureOutput                     // COPP: some other COPP error

} BDROM_DiscProtectionFailureCode;

// -----------------
// Warning Codes - Major and Minor
typedef enum tagBDROM_WarningMajorCode
{
	kBDROM_WarningMajor__PlaybackError,
} BDROM_WarningMajorCode;

typedef enum tagBDROM_WarningMinorCode
{
	// Minor code for kBDROM_FatalErrMajor__PlaybackError
	kBDROM_WarningMinor__SecondaryVMRConnectError,	// VMR for Secondary Video connection/re-connection failed
} BDROM_WarningMinorCode;


//--------------------------------------------------------------------------
// Macros related to the BDROM Events
//
#define SET_TITLESTARTDETAILS_PARAMS(bdrom_event, ttlNum, ttlType, objType, keyIntrTblPresent, keyIntrTbl)	\
			{																								\
				bdrom_event.param1.mUInt32 = (ttlNum & 0x0000FFFF);											\
				if (ttlType != kBDROM_TitleType__Unknown)													\
					bdrom_event.param1.mUInt32 |= (0x00080000 | (((UInt32)ttlType & 0x0000007F) << 16));	\
				if (objType != kBDROM_ObjectType__Unknown)													\
					bdrom_event.param1.mUInt32 |= (0x00800000 | (((UInt32)objType & 0x0000007F) << 20));	\
				if (keyIntrTblPresent)																		\
					bdrom_event.param1.mUInt32 |= 0x80000000;												\
				bdrom_event.param2.mUInt32 = keyIntrTbl;													\
			}

#define GET_TITLESTARTDETAILS_PARAMS(bdrom_event, ttlNum, ttlType, objType, keyIntrTblPresent, keyIntrTbl)	\
			{																								\
				ttlType = kBDROM_TitleType__Unknown; objType = kBDROM_ObjectType__Unknown; keyIntrTbl = 0;	\
				ttlNum  = (bdrom_event.param1.mUInt32 & 0x0000FFFF);										\
				if (bdrom_event.param1.mUInt32 & 0x00080000)												\
					ttlType = (BDROM_TitleType)((bdrom_event.param1.mUInt32 & 0x00070000) >> 16);			\
				if (bdrom_event.param1.mUInt32 & 0x00800000)												\
					objType = (BDROM_ObjectType)((bdrom_event.param1.mUInt32 & 0x00700000) >> 20);			\
				keyIntrTblPresent = ((bdrom_event.param1.mUInt32 & 0x80000000) != 0);						\
				if (keyIntrTblPresent)																		\
					keyIntrTbl = bdrom_event.param2.mUInt32;												\
			}

//--------------------------------------------------------------------------
// BDROM_EventCode
/// @brief	BDROM Event Codes.
///
typedef enum tagBDROM_EventCode
{
	kBDROM_EC__None									= 0,

	// kBDROM_EC__UnsupportedProfile2Feature:
	//      param 1: -reserved for future use-
	//      param 2: -reserved for future use-
	//  Description: A certain Profile 2 feature is not supported
	//       Source: Any BDROM Navigation Component
	//         Dest: Application
	//
	kBDROM_EC__UnsupportedProfile2Feature			= MAKE_BDROM_EVENT(kBDROM_ECat__Disc, 0x0001),

	// kBDROM_EC__DiscProtectionError:
	//      param 1: (mUInt32) A BDROM_DiscProtectionFailureCode enum value
	//      param 2: (mUInt32) Severity Code.
	//                         0: Warning
	//                         1: Fatal Error
	//  Description: The disc is AACS protected and a Authentication/AACS/BD+/COPP/.. error has occured.
	//       Source: Whichever Nav component initiates AACS Authentication
	//         Dest: Application
	//
	kBDROM_EC__DiscProtectionError					= MAKE_BDROM_EVENT(kBDROM_ECat__Disc, 0x0002),

	// kBDROM_EC__FatalError:
	//      param 1: (mUInt32) Major Code - See BDROM_FatalErrorMajorCode enum for possible values
	//      param 2: (mUInt32) Minor Code - See BDROM_FatalErrorMinorCode enum for possible values
	//  Description: A fatal error has occured and playback will be stopped.
	//       Source: Nav component that encountered the fatal error
	//         Dest: (a) Module Manager, so it can stop playback
	//               (b) Application
	//
	kBDROM_EC__FatalError							= MAKE_BDROM_EVENT(kBDROM_ECat__Disc, 0x0003),

	// kBDROM_EC_DM__NewDomain:
	//      param 1: (mUInt32) Domain Type - A BDROM_Domain enum value
	//      param 2: -
	//  Description: Msg to indicate that player has entered a new domain
	//       Source: Module Manager
	//         Dest: Application
	//
	kBDROM_EC_DM__NewDomain							= MAKE_BDROM_EVENT(kBDROM_ECat__Disc, 0x0004),

	// kBDROM_EC_ParentalLevelChangeRequest:
	//      param 1: (mUInt32) New Parental Level
	//      param 2: -
	//  Description: Msg to ask App to authorize a Parental Level change by BDJ xlet.
	//       Source: PBCE
	//         Dest: Application
	//
	kBDROM_EC__ParentalLevelChangeRequest			= MAKE_BDROM_EVENT(kBDROM_ECat__Disc, 0x0005),

	// kBDROM_EC_ParentalLevelChanged:
	//      param 1: (mUInt32) New Parental Level
	//      param 2: -
	//  Description: Msg to indicate that Parental Level has been changed BDJ xlet
	//       Source: PBCE
	//         Dest: Application
	//
	kBDROM_EC__ParentalLevelChanged					= MAKE_BDROM_EVENT(kBDROM_ECat__Disc, 0x0006),

	// kBDROM_EC_CountryCodeChanged:
	//      param 1: (mUInt32) New Country Code
	//      param 2: -
	//  Description: Msg to indicate that Country Code has been changed by BDJ xlet
	//       Source: PBCE
	//         Dest: Application
	//
	kBDROM_EC__CountryCodeChanged					= MAKE_BDROM_EVENT(kBDROM_ECat__Disc, 0x0007),

	// kBDROM_EC_LanguageCodeChanged:
	//      param 1: (mUInt32) Language Type - A BDROM_Domain enum value
    //                   1 : Audio Language
    //                   2 : PG/TextST Language
    //                   3 : Menu Description Language
	//      param 2: (mUInt32) Language Code
	//  Description: Msg to indicate that player has entered a new domain
	//       Source: PBCE
	//         Dest: Application
	//
	kBDROM_EC__LanguageCodeChanged					= MAKE_BDROM_EVENT(kBDROM_ECat__Disc, 0x0008),

	// kBDROM_EC__Warning:
	//      param 1: (mUInt32) Major Code - See BDROM_WarningMajorCode enum for possible values
	//      param 2: (mUInt32) Minor Code - See BDROM_WarningMinorCode enum for possible values
	//  Description: A warning raised by the BDROM Player. Playback will continue but with some degradation.
	//       Source: Nav component that encountered the warning
	//         Dest: (a) Application
	//
	kBDROM_EC__Warning								= MAKE_BDROM_EVENT(kBDROM_ECat__Disc, 0x0009),

	// kBDROM_EC__DriveEjectPressed:
	//      param 1: -
	//      param 2: -
	//  Description: Drive's Eject button pressed (by user). Only for drive in use (& locked by Nav)
	//       Source: Nav component that detects this
	//         Dest: (a) Application
	//
	kBDROM_EC__DriveEjectPressed					= MAKE_BDROM_EVENT(kBDROM_ECat__Disc, 0x000A),

	// kBDROM_EC_PL__PlayListStart:
	//      param 1: (mUInt32) Playlist Number
	//      param 2: -
	//  Description: Playlist has started.
	//       Source: PBCE (Playlist Manager)
	//         Dest: (a) User Event Manager
	//               (b) Application
	//
	kBDROM_EC_PL__PlayListStart						= MAKE_BDROM_EVENT(kBDROM_ECat__Playlist, 0x0001),

	// kBDROM_EC_PL__PlayListEnd:
	//      param 1: -
	//      param 2: -
	//  Description: Current Playlist has ended.
	//       Source: PBCE (Playlist Manager)
	//         Dest: (a) Module Manager (so it can send END_OF_PLAYLIST event to BDJ module)
	//               (b) User Event Manager
	//
	kBDROM_EC_PL__PlayListEnd						= MAKE_BDROM_EVENT(kBDROM_ECat__Playlist, 0x0002),

	// kBDROM_EC_PL__PlayItemStart:
	//      param 1: (mUInt32) Playitem Number
	//      param 2: -
	//  Description: A Playitem in current playlist has started.
	//       Source: PBCE (Playlist Manager)
	//         Dest: (a) Module Manager
	//               (b) Application
	//
	kBDROM_EC_PL__PlayItemStart						= MAKE_BDROM_EVENT(kBDROM_ECat__Playlist, 0x0003),

	// kBDROM_EC_PL__PlayItemEnd:
	//      param 1: -
	//      param 2: -
	//  Description: A Playitem in current playlist has ended.
	//       Source: PBCE (Playlist Manager)
	//         Dest: -none- (yet)
	//
	kBDROM_EC_PL__PlayItemEnd						= MAKE_BDROM_EVENT(kBDROM_ECat__Playlist, 0x0004),

	// kBDROM_EC_PL__AngleChange:
	//      param 1: (mUInt32) Angle number
	//      param 2: -
	//  Description: Current Angle has been changed.
	//       Source: PBCE (PBCE_Clip)
	//         Dest: Application
	//
	kBDROM_EC_PL__AngleChange						= MAKE_BDROM_EVENT(kBDROM_ECat__Playlist, 0x0005),

	// kBDROM_EC_PL__AnglesAvailable:
	//      param 1: (mUInt32) Number of Angles available
	//      param 2: -
	//  Description: Number of available angles has changed.
	//       Source: PBCE (Playlist Manager)
	//         Dest: Application
	//
	kBDROM_EC_PL__AnglesAvailable					= MAKE_BDROM_EVENT(kBDROM_ECat__Playlist, 0x0006),

	// kBDROM_EC_PL__StillOn:
	//      param 1: (mUInt32) Still duration. BDROM_INFINITE_STILL_DURATION for Infinite stills.
	//      param 2: -
	//  Description: A Still operation has begun.
	//       Source: PBCE (Main Path)
	//         Dest: (a) User Event Manager (so it can prohibit Pause during a still)
	//               (b) Application
	//               (c) BDJ Module
	//
	kBDROM_EC_PL__StillOn							= MAKE_BDROM_EVENT(kBDROM_ECat__Playlist, 0x0007),

	// kBDROM_EC_PL__StillOff:
	//      param 1: -
	//      param 2: -
	//  Description: A Still operation has ended.
	//       Source: PBCE (Main Path)
	//         Dest: (a) User Event Manager
	//               (b) Application
	//               (c) BDJ Module
	//
	kBDROM_EC_PL__StillOff							= MAKE_BDROM_EVENT(kBDROM_ECat__Playlist, 0x0008),

	// kBDROM_EC_PL__RateChange:
	//      param 1: (mUInt32) a BDROM_PlayDirection enum value to indicate new direction
	//      param 2: (mUInt32) playback speed calculated as "abs(rate) * 1000".
	//  Description: Playback speed (& possibly direction) has changed
	//         Note: E.g. params for 2x is [BDROM_ForrwardPlay, 2000], and for -1.5x is [BDROM_BackwardPlay, 1500]
	//       Source: PBCE (PBCE_Clip)
	//         Dest: Application
	//
	kBDROM_EC_PL__RateChange						= MAKE_BDROM_EVENT(kBDROM_ECat__Playlist, 0x0009),

	// kBDROM_EC_PL__PlaybackMark:
	//      param 1: (mUInt32) PL_mark_id of the PlayList Mark passed. 
	//      param 2: -
	//  Description: Playback has passed a playlist mark of either type Entry Mark or Link Point.
	//       Source: PBCE (Main Path)
	//         Dest: BDJ Module
	//
	kBDROM_EC_PL__PlaybackMark						= MAKE_BDROM_EVENT(kBDROM_ECat__Playlist, 0x000A),

	// kBDROM_EC_PL__DataStarved:
	//      param 1: - 
	//      param 2: -
	//  Description: Playback has it a disabled VFS clip in a progressive playlist.
	//       Source: PBCE (Main Path)
	//         Dest: BDJ Module
	//
	kBDROM_EC_PL__DataStarved						= MAKE_BDROM_EVENT(kBDROM_ECat__Playlist, 0x000B),
	
	kBDROM_EC_PL__EnterTerminationState				= MAKE_BDROM_EVENT(kBDROM_ECat__Playlist, 0x000C),
	
	// kBDROM_EC_PL__PrimaryAudioChange:
	//      param 1: (mUInt32) Primary Audio stream number. BDROM_PRIMARY_AUDIO_STREAM_NONE if no Primary Audio is selected.
	//      param 2: -
	//  Description: Primary Audio Stream selected/changed
	//       Source: PBCE (Playlist Manager)
	//         Dest: (a) Module Manager
	//               (b) Application
	//
	kBDROM_EC_PL__PrimaryAudioChange				= MAKE_BDROM_EVENT(kBDROM_ECat__Stream, 0x0001),

	// kBDROM_EC_PL__SecondaryAudioChange:
	//      param 1: (mUInt32) Secondary Audio stream number. BDROM_SECONDARY_AUDIO_STREAM_NONE if no Secondary Audio is selected.
	//      param 2: (mUInt32) Secondary Audio presentation state. 1: enabled, 0: disabled
	//  Description: Secondary Audio Stream selected/changed
	//       Source: PBCE (Playlist Manager)
	//         Dest: (a) Module Manager
	//               (b) Application
	//
	kBDROM_EC_PL__SecondaryAudioChange				= MAKE_BDROM_EVENT(kBDROM_ECat__Stream, 0x0002),

	// kBDROM_EC_PL__SecondaryVideoChange:
	//      param 1: (mUInt32) Secondary Video stream number. BDROM_SECONDARY_VIDEO_STREAM_NONE if no Secondary Video is selected.
	//      param 2: (mUInt32) Secondary Video presentation state. 1: enabled, 0: disabled
	//  Description: Secondary Video Stream selected/changed
	//       Source: PBCE (Playlist Manager)
	//         Dest: Application
	//
	kBDROM_EC_PL__SecondaryVideoChange				= MAKE_BDROM_EVENT(kBDROM_ECat__Stream, 0x0003),

	// kBDROM_EC_PL__PGTextSTChange:
	//      param 1: (mUInt32) PG/TextST or PiP PG/TextST stream number. BDROM_PGTEXTST_STREAM_NONE if no stream selected.
	//      param 2: (mUInt32) Stream type & Presentation state.
	//                         bit 0: Stream type.          0= PG,  1= Text Subtitle
	//                         bit 1: Stream Display State. 0= Off, 1= On
	//  Description: PG/TextST or PiP PG/TextST Stream selected/changed
	//       Source: PBCE (Playlist Manager)
	//         Dest: (a) Module Manager
	//               (b) Application
	//
	kBDROM_EC_PL__PGTextSTChange					= MAKE_BDROM_EVENT(kBDROM_ECat__Stream, 0x0004),

	// kBDROM_EC_PL__SecondaryAudioAvailable:
	//      param 1: (mUInt32) Number of Secondary Audio streams available.
	//      param 2: (mUInt32) Secondary Audio stream selection basis. Secondary Audio is selected based on either Primary Audio or Secondary Video.
    //                   1 : Secondary Audio stream is selected based on Secondary Video. This means that secondary audio cannot be
	//                       independently changed; it will get changed based on secondary video selection.
    //                   2 : Secondary Audio can be independently selected, but its selection is based on the current Primary Audio
	//                       stream, i.e., only streams that are allowed to be combined with the current Primary Audio stream can be selected.
	//  Description: Secondary Audio Stream availability
	//       Source: PBCE (Playlist Manager)
	//         Dest: Application
	//
	kBDROM_EC_PL__SecondaryAudioAvailable			= MAKE_BDROM_EVENT(kBDROM_ECat__Stream, 0x0005),

	// kBDROM_EC_PL__SecondaryVideoAvailable:
	//      param 1: (mUInt32) Number of Secondary Video streams available.
	//      param 2: -
	//  Description: Secondary Video Stream availability
	//       Source: PBCE (Playlist Manager)
	//         Dest: Application
	//
	kBDROM_EC_PL__SecondaryVideoAvailable			= MAKE_BDROM_EVENT(kBDROM_ECat__Stream, 0x0006),

	// kBDROM_EC_PL__PrimaryVideoResolutionChanged:
	//      param 1: (mUInt32) Video Width
	//      param 2: (mUInt32) Video Width
	//  Description: Primary Video Resolution has changed
	//       Source: PBCE (VideoSizeManager)
	//         Dest: ModuleManager
	//
	kBDROM_EC_PL__PrimaryVideoResolutionChanged		= MAKE_BDROM_EVENT(kBDROM_ECat__Stream, 0x0007),

	// kBDROM_EC_PL__SecondaryVideoResolutionChanged:
	//      param 1: (mUInt32) Video Width
	//      param 2: (mUInt32) Video Width
	//  Description: Secondary Video Resolution has changed
	//       Source: PBCE (VideoSizeManager)
	//         Dest: ModuleManager
	//
	kBDROM_EC_PL__SecondaryVideoResolutionChanged	= MAKE_BDROM_EVENT(kBDROM_ECat__Stream, 0x0008),

	
	// kBDROM_EC_PL__SecondaryVideoAvailable:
	//      param 1: (mUInt32) Number of Secondary Video streams available.
	//      param 2: -
	//  Description: Secondary Video Stream availability
	//       Source: PBCE (Playlist Manager)
	//         Dest: Application
	//
	kBDROM_EC_PL__SubtitleAvailable					= MAKE_BDROM_EVENT(kBDROM_ECat__Stream, 0x0009),
	
	// kBDROM_EC_PL__InteractiveAudioEnd:
	//      param 1: (mUInt32) SoundID
	//      param 2: -
	//  Description: Interactive Audio playback has ended
	//       Source: Sound Manager
	//         Dest: Application
	//
	kBDROM_EC_PL__InteractiveAudioEnd   			= MAKE_BDROM_EVENT(kBDROM_ECat__Stream, 0x000a),

	// kBDROM_EC_PL__MediaSelectEvent:
	//      param 1: (mUInt32) MediaSelect stream numbers (part-1)
	//      			bit 00-06 : Primary Video stream number (if valid, will always be 1).
	//      			bit    07 : Primary Video stream number valid.
	//      			bit 08-14 : Primary Audio stream number (0: None, 1-32: stream num).
	//					bit    15 : Primary Audio stream number valid.
	//					bit 16-22 : Secondary Video stream number (0: None, 1-32: stream num).
	//					bit    23 : Secondary Video stream number valid
	//      			bit 24-30 : Secondary Audio stream number (0: none, 1-32: stream num).
	//      			bit    31 : Secondary Audio stream number valid.
	//      param 2: (mUInt32) MediaSelect stream numbers (part-2)
	//      			bit 00-14 : Primary Subtitle (aka PG/TextST) stream number (0: None, 1-255 stream num)
	//      			bit    15 : Primary Subtitle (aka PG/TextST) stream number valid.
	//      			bit 16-30 : PiP Subtitle (aka PiP PG/TextST) stream number (0: None, 1-255 stream num)
	//      			bit    31 : PiP Subtitle (aka PiP PG/TextST) stream number valid.
	//				 Only one of the two Subtitle numbers will be valid.
	//  Description: Trigger for generating javax.tv.media.MediaSelectEvent
	//       Source: PBCE
	//         Dest: Module Manager (meant for BD-J Module only)
	//
	kBDROM_EC_PL__MediaSelectEvent   				= MAKE_BDROM_EVENT(kBDROM_ECat__Stream, 0x000b),

	//REALTEK_MODIFY
	//kBDROM_EC_PL__SubtitleStatus:
	//		param 1: (mUInt32) total number of subtitle streams
	//		param 2: (mUInt32) a pointer to the stream attribute array 
	//		 		Note : it is a dynamically allocate memory and should be
	//						de-allocate after event sent.
	//  Description: Send subtitle stream attribute
	//       Source: PBCE (Playlist Mgr)
	//         Dest: Application
	//
	kBDROM_EC_PL__SubtitleStatus					= MAKE_BDROM_EVENT(kBDROM_ECat__Stream, 0x000C),
	
	//REALTEK_MODIFY
	//kBDROM_EC_PL__AudioStatus:
	//		param 1: (mUInt32) total number of primary audio streams
	//		param 2: (mUInt32) a pointer to the stream attribute array 
	//		 		Note : it is a dynamically allocate memory and should be
	//						de-allocate after event sent.
	//  Description: Send primary audio stream attribute
	//       Source: PBCE (Playlist Mgr)
	//         Dest: Application
	//
	kBDROM_EC_PL__AudioStatus						= MAKE_BDROM_EVENT(kBDROM_ECat__Stream, 0x000D),
	
	//REALTEK_MODIFY
	//kBDROM_EC_PL__VideoStatus:
	//		param 1: (mUInt32) total number of primary video streams
	//		param 2: (mUInt32) a pointer to the stream attribute array 
	//		 		Note : it is a dynamically allocate memory and should be
	//						de-allocate after event sent.
	//  Description: Send primary video stream attribute
	//       Source: PBCE (Playlist Mgr)
	//         Dest: Application
	//
	kBDROM_EC_PL__VideoStatus						= MAKE_BDROM_EVENT(kBDROM_ECat__Stream, 0x000E),
	
	// kBDROM_EC_TT__TitleStartDetails:
	//      param 1: (mUInt32) Title Number, Title Type, Object Type, and Key Interest Table presence flag
	//                  bits 00-15: Title Number
	//                  bits 16-17: Title Type
	//                  bit  19   : flag to indicate if bits 16-17 contain Title Type
	//                  bits 20-21: Object Type
	//                  bit  23   : flag to indicate if bits 20-21 contain Object Type
	//                  bit  31   : flag to indicate if param 2 has a Key Interest Table
	//      param 2: (mUint32) Key Interest Table. (if bit 21 of param1 is '1')
	//        Notes: Use SET_TITLESTARTDETAILS_PARAMS() and GET_TITLESTARTDETAILS_PARAMS() to set and get params for this event
	//  Description: Msg to indicate that a new Title has been loaded, along with information about the title
	//       Source: Module Manager
	//         Dest: User Event Manager (so it can update UOPs and UER Table)
	//
	kBDROM_EC_TT__TitleStartDetails					= MAKE_BDROM_EVENT(kBDROM_ECat__Title, 0x0001),

	// kBDROM_EC_TT__TitleStart:
	//      param 1: (mUInt32) Title Number
	//      param 2: (mUint32) Title Type
	//  Description: Msg to indicate that a new Title has been loaded, along with information about the title
	//       Source: User Event Manager
	//         Dest: Application
	//
	kBDROM_EC_TT__TitleStart						= MAKE_BDROM_EVENT(kBDROM_ECat__Title, 0x0002),

	// kBDROM_EC_TT__TitleEnd:
	//      param 1: -
	//      param 2: -
	//  Description: Msg to indicate that a current Title has been unloaded.
	//       Source: Module Manager
	//         Dest: User Event Manager (so it can update UOPs and UER Table)
	//
	kBDROM_EC_TT__TitleEnd							= MAKE_BDROM_EVENT(kBDROM_ECat__Title, 0x0003),

	// kBDROM_EC_TT__ChapterStart:
	//      param 1: (mUInt32) Chapter number. If this is BDROM_INVALID_CHAPTER_NUM, it means that the current title does not have any Chapters,
	//               as would be the case for Interactive Titles.
	//      param 2: -
	//  Description: New Chapter has begun.
	//       Source: PBCE (Playlist Manager)
	//         Dest: (a) Module Manager
	//               (b) Application
	//
	kBDROM_EC_TT__ChapterStart						= MAKE_BDROM_EVENT(kBDROM_ECat__Title, 0x0004),

	// kBDROM_EC_TT__ChapterEnd:
	//      param 1: -
	//      param 2: -
	//  Description: Current Chapter has ended.
	//       Source: PBCE (Playlist Manager)
	//         Dest: -none- (yet)
	//
	kBDROM_EC_TT__ChapterEnd						= MAKE_BDROM_EVENT(kBDROM_ECat__Title, 0x0005),

	// kBDROM_EC_TT__ResumeAvailability:
	//      param 1: (mUInt32) Availability of a Resume operation. 1: Resume available, 0: Resume not available
	//      param 2: -
	//  Description: Availability of a Resume user operation has changed.
	//       Source: Module Manager
	//         Dest: Application
	//
	kBDROM_EC_TT__ResumeAvailability				= MAKE_BDROM_EVENT(kBDROM_ECat__Title, 0x0006),

	// kBDROM_EC_TT__MOBJorBDJOStart:
	//      param 1: -
	//      param 2: -
	//  Description: New Movie Object / BDJO has been loaded.
	//      *NOTE* : Should be send only *after* kBDROM_EC_TT__TitleStartDetails is sent.
	//       Source: Module Manager
	//         Dest: User Event Manager
	//
	kBDROM_EC_TT__MOBJorBDJOStart						= MAKE_BDROM_EVENT(kBDROM_ECat__Title, 0x0007),

	//REALTEK_MODIFY
	// kBDROM_EC_TT__TotalChapterNum:
	//      param 1: Total chapter numer
	//      param 2: -
	//  Description: New playlist is loaded, should update total chapter number
	//      Source: PBCE (Playlist Manager)
	//		Dest : Application
	//
	kBDROM_EC_TT__TotalChapterNum					= MAKE_BDROM_EVENT(kBDROM_ECat__Title, 0x0008),

	kBDROM_EC_TM__ElapsedTime						= MAKE_BDROM_EVENT(kBDROM_ECat__Time, 0x0001),

	// kBDROM_EC_TM__PlaylistTime:
	//      param 1: (mTime) Playlist time (aka media time) as 90KHz value.
	//      param 2: -
	//  Description: Playlist time (elapsed time in current playlist)
	//       Source: If use of PTS callbacks has been enabled, PtsCallbackHandler. Otherwise PBCE.
	//         Dest: Module Manager
	//
	kBDROM_EC_TM__PlaylistTime						= MAKE_BDROM_EVENT(kBDROM_ECat__Time, 0x0002),

	// kBDROM_EC_TM__StillTimerElapsed:
	//      param 1: -
	//      param 2: -
	//  Description: Timed Still duration has elapsed
	//       Source: Timed-Still Timer
	//         Dest: PBCE
	//
	kBDROM_EC_TM__StillTimerElapsed					= MAKE_BDROM_EVENT(kBDROM_ECat__Time, 0x0003),

	// kBDROM_EC_TM__NavTimerElapsed:
	//      param 1: (mUInt32) Movie Object Id to jump to
	//      param 2: -
	//  Description: Nav Timer has elapsed
	//       Source: Nav Timer
	//         Dest: Module Manager
	//
	kBDROM_EC_TM__NavTimerElapsed					= MAKE_BDROM_EVENT(kBDROM_ECat__Time, 0x0004),

	// kBDROM_EC_TM__TimePulse:
	//      param 1: -
	//      param 2: -
	//  Description: Trigger to PBCE (so it can process things like current time, chapter, etc. and send regular events to mod mgr, app,...)
	//       Source: Title Timer
	//         Dest: PBCE
	//
	kBDROM_EC_TM__TimePulse							= MAKE_BDROM_EVENT(kBDROM_ECat__Time, 0x0005),

	// kBDROM_EC_TM_NavTimerTick:
	//      param 1: -
	//      param 2: -
	//  Description: Trigger a PSR9 decrement 
	//       Source: Nav Timer
	//         Dest: Module Manager
	kBDROM_EC_TM__NavTimerTick						= MAKE_BDROM_EVENT(kBDROM_ECat__Time, 0x0006),

	// kBDROM_EC_TM__StopTime:
	//      param 1: -
	//      param 2: -
	//  Description: Stop time. 
	//       Source: PBCE
	//         Dest: Module Manager
	//
	kBDROM_EC_TM__StopTime							= MAKE_BDROM_EVENT(kBDROM_ECat__Time, 0x0007),

	// kBDROM_EC_TM__ElapsedTimeInSec:
	//      param 1: (mUInt32) Elapsed time in sec
	//      param 2: -
	//  Description: Elapsed time. Sent only for Movie titles.
	//       Source: PBCE
	//         Dest: Application
	//
	kBDROM_EC_TM__ElapsedTimeInSec					= MAKE_BDROM_EVENT(kBDROM_ECat__Time, 0x0008),

	// kBDROM_EC_TM__CurrentPlaylistDurationInSec:
	//      param 1: (mUInt32) current play list duration 
	//      param 2: -
	//  Description: 
	//       Source: PBCE
	//         Dest: Application
	//
	kBDROM_EC_TM__CurrentPlaylistDurationInSec		= MAKE_BDROM_EVENT(kBDROM_ECat__Time, 0x0009),
	
	// kBDROM_EC_TM__ChapterStartTimeInSec:
	//      param 1: (mUInt32) chapter number 
	//      param 2: (mUInt32) chapter start time in sec
	//  Description: chapter start time. 
	//       Source: PBCE
	//         Dest: Application
	//
	kBDROM_EC_TM__ChapterStartTimeInSec				= MAKE_BDROM_EVENT(kBDROM_ECat__Time, 0x000A),
	
	// kBDROM_EC_TM__ChapterDurationInSec:
	//      param 1: (mUInt32) chapter number 
	//      param 2: (mUInt32) chapter duration in sec
	//  Description: chapter duration. 
	//       Source: PBCE
	//         Dest: Application
	//
	kBDROM_EC_TM__ChapterDurationInSec				= MAKE_BDROM_EVENT(kBDROM_ECat__Time, 0x000B),

	// kBDROM_EC_TM__ElapsedTime:
	//      param 1: (mTime) Elapsed time in HH:MM:SS format
	//      param 2: -
	//  Description: Elapsed time. Sent only for Movie titles.
	//       Source: PBCE
	//         Dest: Application
	//
	
	// kBDROM_EC_IG__ButtonCommands:
	//      param 1: (mpVoid) A IGButtonCommands* that points to an array of button commands. Receiver of event must free this memory.
	//      param 2: -
	//  Description: Button Commands to be executed
	//       Source: IG Decoder (GC)
	//         Dest: Module Manager
	//
	kBDROM_EC_IG__ButtonCommands					= MAKE_BDROM_EVENT(kBDROM_ECat__IG, 0x0001),

	// kBDROM_EC_IG__PopUpMenuAvailable:
	//      param 1: -
	//      param 2: -
	//  Description: A PopUp Menu is available
	//       Source: PlayList Manager
	//         Dest: Application
	//
	kBDROM_EC_IG__PopUpMenuAvailable				= MAKE_BDROM_EVENT(kBDROM_ECat__IG, 0x0002),

	// kBDROM_EC_IG__MenuPageDisplay:
	//      param 1: (mUInt32) Id of Menu Page. Valid page ids are 0x00 - 0xFE.  A value of 0xFF means menu has been removed from display.
	//      param 2: (mUInt32) Page Width and Height. Upper 16 bits contain width, lower 16 contain height.
	//  Description: Menu Page Information sent when a page is displayed or removed.
	//       Source: IG Decoder (GC)
	//         Dest: Application
	//
	kBDROM_EC_IG__MenuPageDisplay					= MAKE_BDROM_EVENT(kBDROM_ECat__IG, 0x0003),

	// kBDROM_EC_IG__ButtonChange:
	//      param 1: (mUInt32) Button number
	//      param 2: -
	//  Description: A new button has been Selected
	//       Source: IG Decoder (GC)
	//         Dest: Application
	//
	kBDROM_EC_IG__ButtonChange						= MAKE_BDROM_EVENT(kBDROM_ECat__IG, 0x0004),

	//REALTEK_MODIFY
	//kBDROM_EC_IG__TotalButtonNum
	//      param 1: (mUInt32) Total Button number
	//      param 2: -
	//  Description: total button number in this page
	//       Source: IG Decoder (GC)
	//         Dest: Application
	//
	kBDROM_EC_IG__TotalButtonNum					= MAKE_BDROM_EVENT(kBDROM_ECat__IG, 0x0005),

	// kBDROM_EC_CP__AACSAuthStageCompleted:		(this event is not being used and may never be used)
	//      param 1: (mUInt32) AACS Stage
	//      param 2: -
	//  Description: AACS Authentication stage complete
	//       Source: AACS_DiscAccess
	//         Dest: Application
	//
	kBDROM_EC_CP__AACSAuthStageCompleted			= MAKE_BDROM_EVENT(kBDROM_ECat__Prot, 0x0001),

	// kBDROM_EC_CP__AACSAuthCompleted:
	//      param 1: (mUInt32) SonicError value. Success/Error code of AACS authentication
	//      param 2: -
	//  Description: AACS Authentication completed/failed
	//       Source: AACS_DiscAccess
	//         Dest: AACS Authentication Listener
	//
	kBDROM_EC_CP__AACSAuthCompleted					= MAKE_BDROM_EVENT(kBDROM_ECat__Prot, 0x0002),

	// kBDROM_EC_CP__SMDisconnected:
	//      param 1: -
	//      param 2: -
	//  Description: Security Module has been disconnected
	//       Source: AACS_DiscAccess
	//         Dest: ?
	//
	kBDROM_EC_CP__SMDisconnected					= MAKE_BDROM_EVENT(kBDROM_ECat__Prot, 0x0004),

	// kBDROM_EC_CP__SMDestroyed:
	//      param 1: -
	//      param 2: -
	//  Description: Security Module has been destroyed
	//       Source: AACS_DiscAccess
	//         Dest: ?
	//
	kBDROM_EC_CP__SMDestroyed						= MAKE_BDROM_EVENT(kBDROM_ECat__Prot, 0x0005),

	// kBDROM_EC_CP__BDplusMessage:
	//      param 1: message value stored in PSR 102
	//      param 2: -
	//  Description: Message from BD+ Module
	//       Source: ContentProtectionManager
	//         Dest: Module Manager
	//
	kBDROM_EC_CP__BDplusMessage						= MAKE_BDROM_EVENT(kBDROM_ECat__Prot, 0x0006),

	// kBDROM_EC_CP__BDplusStatus:
	//      param 1: status value stored in PSR 104
	//      param 2: -
	//  Description: Status from BD+ Module
	//       Source: ContentProtectionManager
	//         Dest: Module Manager
	//
	kBDROM_EC_CP__BDplusStatus						= MAKE_BDROM_EVENT(kBDROM_ECat__Prot, 0x0007),

	// kBDROM_EC_UO__PlayList:
	//		param 1: (mpVoid) UO_mask_table* value.
	//		param 2: -
	//	Description: Playlist UOPs
	//         Note: This event is sent after the kBDROM_EC_PL__PlayListStart event is sent
	//		 Source: PBCE (Playlist Manager)
	//		   Dest: User Event Manager (so it can update Player UOPs and User Event Response Table)
	//
	kBDROM_EC_UO__PlayList							= MAKE_BDROM_EVENT(kBDROM_ECat__UOP, 0x0001),

	// kBDROM_EC_UO__PlayItem:
	//		param 1: (mpVoid) UO_mask_table* value.
	//		param 2: -
	//	Description: Playitem UOPs
	//         Note: This event is sent after the kBDROM_EC_PL__PlayItemStart event is sent
	//		 Source: PBCE (Playlist Manager)
	//		   Dest: User Event Manager (so it can update Player UOPs and User Event Response Table)
	//
	kBDROM_EC_UO__PlayItem							= MAKE_BDROM_EVENT(kBDROM_ECat__UOP, 0x0002),

	// kBDROM_EC_UO__Page:
	//		param 1: (mUOPs) Page UOPs. Value only if param2 is 1.
	//		param 2: (mUInt32) Page display state. 1: Page on, 0: Page off
	//	Description: Menu Page UOPs
	//		 Source: IG Decoder (GC)
	//		   Dest: User Event Manager (so it can update Player UOPs and User Event Response Table)
	//
	kBDROM_EC_UO__Page								= MAKE_BDROM_EVENT(kBDROM_ECat__UOP, 0x0003),

	// kBDROM_EC_UO__PlayerUOPs:
	//		param 1: (mUOPs) Player UOPs. This is the combined UOPS created from Terminal Info, playlist UOPs, playitem UOPs, and current menu page UOPs, if any.
	//		param 2: -
	//	Description: Player UOPs
	//		 Source: User Event Manager
	//		   Dest: Application
	//
	kBDROM_EC_UO__PlayerUOPs						= MAKE_BDROM_EVENT(kBDROM_ECat__UOP, 0x0004),

	// kBDROM_EC_UO__KeyInterestTable:
	//		param 1: (mUInt32) Key Interest Table flags (32 bits - as in the spec)
	//		param 2: (mUInt32) Set/Remove Table. 1: Set new Key Interest Table, 0: Remove Key Interest Table
	//	Description: Msg to indicate that a new Key Interest Table is available, or that no Key Interest Table is to be used
	//		 Source: Module Manager
	//		   Dest: User Event Manager (so it can update User Event Response Table and send kBDROM_EC_UO__NewUserEventResponseTable event to app)
	//
	kBDROM_EC_UO__KeyInterestTable					= MAKE_BDROM_EVENT(kBDROM_ECat__UOP, 0x0005),

	// kBDROM_EC_UO__NewUserEventResponseTable:
	//		param 1: -
	//		param 2: -
	//	Description: Msg to indicate that a new User Event Response Table is available.
	//		 Source: User Event Manager
	//		   Dest: Application (so it can call GetUserEventResponseTable() to retrieve the new User Event Response Table)
	//
	kBDROM_EC_UO__NewUserEventResponseTable			= MAKE_BDROM_EVENT(kBDROM_ECat__UOP, 0x0006),

	// EC_UO__UserOperationMasked:
	//		param 1: Index of attempted UserOperation
	//		param 2: -
	//	Description: Msg to indicate that a user tried to perform a masked user operation.
	//		 Source: Module Manager
	//		   Dest: BDJModule -- so UOMaskedEvent can be provided to listeners for such events
	//
	kBDROM_EC_UO__UserOperationMasked				= MAKE_BDROM_EVENT(kBDROM_ECat__UOP, 0x0007),

	//kBDROM_EC_ST__NaviCommandExecuted:
	//		param 1: (mpVoid) Reference to NaviCommand info.  
	//		param 2:   
	//	Description: Msg to indicate that a NaviCmd has occurred.
	//		 Source: HDMV Module Manager
	//		  Dest: Application
	kBDROM_EC_ST__NaviCommandExecuted				= MAKE_BDROM_EVENT(kBDROM_ECat__NaviCmd, 0x0001),

	// kBDROM_EC_ST__PSRSet
	//		param 1: - (mUInt32) PSR Number.  
	//		param 2: - (mUInt32) the value of the PSR.
	//	Description: Msg to indicate that a PSR value has changed.
	//		 Source: PBCE (Playlist Manager)
	//		  Dest: Application
	kBDROM_EC_ST__PSRSet							= MAKE_BDROM_EVENT(kBDROM_ECat__PSR, 0x0001),

	// kBDROM_EC_ST__GPRSet
	//		param 1: - (mUInt32) GPR Number.  
	//		param 2: - (mUInt32) the value of the GPR.
	//	Description: Msg to indicate that a GPR value has changed.
	//		 Source: PBCE (Playlist Manager)
	//		   Dest: Application
	kBDROM_EC_ST__GPRSet							= MAKE_BDROM_EVENT(kBDROM_ECat__GPR, 0x0001),

	//REALTEK_MODIFY
	//kBDROM_EC_PG__Flush
	//      param 1: -
	//      param 2: -
	//  Description: To flush current PG stream
	//       Source: PG Decoder
	//         Dest: Application
	//
	kBDROM_EC_PG__Flush							= MAKE_BDROM_EVENT(kBDROM_ECat__PG, 0x0001),
	
	//REALTEK_MODIFY
	//kBDROM_EC_PG__OnOff
	//      param 1: (mUInt32) 1 means On and 0 means Off
	//      param 2: -
	//  Description: To enable/disable PG display
	//       Source: PG Decoder
	//         Dest: Application
	//
	kBDROM_EC_PG__OnOff							= MAKE_BDROM_EVENT(kBDROM_ECat__PG, 0x0002),
	
} BDROM_EventCode;


//--------------------------------------------------------------------------
// BDROM_Event
/// @brief	A BDROM Event
///
struct BDROM_Event
{
public:
	BDROM_EventCode			mEventCode;

	union
	{
		UInt32				mUInt32;
		void *				mpVoid;
		DVDTimeStampType	mTime;
		UInt8				mUOPs[8];
		double				mDouble;
	} param1;


	union
	{
		UInt32				mUInt32;
	} param2;

public:
	BDROM_Event()					: mEventCode(kBDROM_EC__None)		{ param1.mUInt32 = 0; param2.mUInt32 = 0; }
	BDROM_Event(BDROM_EventCode ec)	: mEventCode(ec)					{ param1.mUInt32 = 0; param2.mUInt32 = 0; }
};


typedef enum tagBDROM_ChPosInTitle
{
	kBDROM_ChPosInTitle__None			= 0x00,
	kBDROM_ChPosInTitle__FirstChapter	= 0x01,
	kBDROM_ChPosInTitle__LastChapter	= 0x02,
} BDROM_ChPosInTitle;


#endif // __BDROM_NAVIGATORTYPES_H__
