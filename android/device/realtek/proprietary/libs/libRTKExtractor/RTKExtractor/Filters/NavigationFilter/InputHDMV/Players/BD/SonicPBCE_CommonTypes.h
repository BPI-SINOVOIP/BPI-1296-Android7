//-----------------------------------------------------------------------------
// SonicPBCE_CommonTypes.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __SONICPBCE_COMMONTYPES_H__
#define __SONICPBCE_COMMONTYPES_H__

//___________________________________________________________________________________________________________________________________________________
//
// BDROM Spec based common defines, typedefs, structures
//


// BDROM_SubPathType
// SubPath_type defined in BDROM spec
//
typedef enum tagBDROM_SubPathType
{
	kBDROM_SubPathType__Reserved_0							= 0,
	kBDROM_SubPathType__Reserved_1							= 1,
	kBDROM_SubPathType__PrimaryAudio_BrowsableSlideshow		= 2,
	kBDROM_SubPathType__IG_Menu								= 3,
	kBDROM_SubPathType__TextSubtitle						= 4,
	kBDROM_SubPathType__OutOfMux_Sync_ES_or_PiP				= 5,
	kBDROM_SubPathType__OutOfMux_Async_PiP					= 6,
	kBDROM_SubPathType__InMux_Sync_PiP						= 7
} BDROM_SubPathType;


// BDROM_StreamEntryType
// Item type of Audio/Video/PG/IG/Txt stream entry in the STN_table - PlayItem or SubPlayItem
//
typedef enum tagBDROM_StreamEntryType
{
	kBDROM_StreamEntryType__Reserved				= 0,
	kBDROM_StreamEntryType__PlayItem				= 1,
	kBDROM_StreamEntryType__SubPlayItem				= 2,	///< Elementary Stream of Clip used by SubPath with subpath_type of 2, 3, 4, 5, or 6
	kBDROM_StreamEntryType__SubPlayItem_InMux_PiP	= 3		///< Elementary Stream of Clip used by SubPath with subpath_type 7 (In-Mux Synchronous PiP)
} BDROM_StreamEntryType;


// BDROM_StillType
// Type of still at the end of a playitem
//
typedef enum tagBDROM_StillType
{
	kBDROM_StillType__None						= 0,
	kBDROM_StillType__TimedStill				= 1,
	kBDROM_StillType__InfiniteStill				= 2
} BDROM_StillType;


//___________________________________________________________________________________________________________________________________________________
//
// PBCE specific common defines, typedefs, structures
//

// PBCE_PathType
// Type of Path - Main or Sub
//
typedef enum tagPBCE_PathType
{
	kPBCE_PathType__Unknown,

	// mainpath
	kPBCE_PathType__MainPath,				///< MainPath

	// subpaths
	kPBCE_PathType__SubPath_InMux,			///< In-Mux SubPath
	kPBCE_PathType__SubPath_OutOfMux,		///< OutOfMux SubPath
	kPBCE_PathType__SubPreload_IG,			///< Preload IG SubPath
	kPBCE_PathType__SubPreload_Text			///< Preload TextST SubPath
} PBCE_PathType;


// PBCE_PathHandle
// Uniquely identifies PBCE_Paths
//
typedef enum tagPBCE_PathHandle
{
	kPBCE_PathHandle__Invalid			= 0,
	kPBCE_PathHandle__Main				= 1,
	kPBCE_PathHandle__Sub				= 2,
	kPBCE_PathHandle__Sub_PiP			= 3,
	kPBCE_PathHandle__Sub_Preload_Txt	= 4,
	kPBCE_PathHandle__Sub_Preload_IG	= 5
} PBCE_PathHandle;


// PBCE_ItemType
// Type of Item - PlayItem or SubPlayItem
//
typedef enum tagPBCE_ItemType
{
	kPBCE_ItemType__PlayItem,
	kPBCE_ItemType__SubPlayItem
} PBCE_ItemType;


#define BDROM_PBCE__CURRENT_STREAM					0					// current stream number (for any stream type)

// BDROM_SPN_INVALID
// Invalid SPN
// 
#define BDROM_SPN_INVALID							0xFFFFFFFF

// BDROM_SPN_END_OF_FILE
// End-of-File SPN
// 
#define BDROM_SPN_END_OF_FILE						0xFFFFFFFE


// PBCE_ClipHandle
// Uniquely identifies the PBCE_Clip objects
//
typedef enum tagPBCE_ClipHandle
{
	kPBCE_ClipHandle__Invalid			= 0,
	kPBCE_ClipHandle__Main				= 1,
	kPBCE_ClipHandle__Sub				= 2,
	kPBCE_ClipHandle__Sub_Preload_1		= 3,
	kPBCE_ClipHandle__Sub_Preload_2		= 4,
} PBCE_ClipHandle;

#endif
