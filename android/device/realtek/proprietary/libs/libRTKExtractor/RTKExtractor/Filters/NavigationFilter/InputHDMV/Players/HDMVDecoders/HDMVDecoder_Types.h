//-----------------------------------------------------------------------------
// HDMVDecoder_Types.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __HDMVDECODER_TYPES_H__
#define __HDMVDECODER_TYPES_H__

//#include "BDROM_NavigatorTypes.h"
#include "BDROMIndex.h"
#include "SonicCounter.h"
#include "TimeValue.h"


//--------------------------------------------------------------------------
// IGButtonCommands
/// @brief	Container to transport navigation commands from the IG Decoder to the Module Manager (via the Notification Event Manager)
///
struct IGButtonCommands
{
public:
	UInt16							mnumber_of_navigation_commands;
	BDROM::navigation_command	*	mnavigation_command;

	// unique ID for this set of commands. Set by the IG Decoder.
	UInt32							mBtnCmdSetId;

	// true if a SetButtonPage cmd from this cmd set was executed (to be set by module that executes these commands)
	bool							mSetButtonPageCmdExecuted;

public:
	IGButtonCommands()
	{
		mnumber_of_navigation_commands = 0;
		mnavigation_command = 0;
		mSetButtonPageCmdExecuted = false;
	}

	~IGButtonCommands()
	{
		if (mnavigation_command)
			delete [] mnavigation_command;
	}

	void CreateCommands(UInt16 uiNumCmds)
	{
		mnavigation_command = new BDROM::navigation_command[uiNumCmds];
		if (mnavigation_command)
			mnumber_of_navigation_commands = uiNumCmds;
	}
};

//--------------------------------------------------------------------------
// ButtonCmdsExecutionInfo
/// @brief	Information about button commands that were just executed
///
struct ButtonCmdsExecutionInfo
{
public:
	UInt32			mBtnCmdSetId;					// an unique id for the button command set just executed (id set by IG Decoder)
	bool			mSetButtonPageCmdExecuted;		// true if a SetButtonPage cmd was executed.
};



namespace BDROM_HDMVDecoder
{

const UInt32	kPageId_Invalid						= 0xFF;				// Invalid page id
const UInt32	kPageVersion_Invalid				= 0xFFFF;			// Invalid Page Version (valid values are 0x00 - 0xFF)

const UInt16	kButtonId_Invalid					= 0xFFFF;			// Invalid Button id
const UInt16	kMaxButtonsPerPage					= 255;				// max buttons allowed in a page
const UInt16	kButtonNumbericSelectVal_None		= 0xFFFF;			// No 'Numeric Select Value' for a button


// Error Base and Errors.
//
//    This value is the used to create errors in the range of 0x800040300 to 0x8000403FF
//    to make it unique in the BDROM Player, (and on windows, keep it within the range for FACILITY_ITF errors.)
//
const SInt32 kHDMVDec_ErrorBase = 0xA0040400;

enum
{
	// Success codes
	kHDMVDecErr_NoErr							= 0,							// Success

	// failure codes
	kHDMVDecErr_NotEnoughSpaceInEB				= (kHDMVDec_ErrorBase - 1),		// Not enough space in EB to fit PES payload
	kHDMVDecErr_NoSegmentsInEB					= (kHDMVDec_ErrorBase - 2),		// No more Segments available in EB.
	kHDMVDecErr_InvalidParam					= (kHDMVDec_ErrorBase - 3),		// Invalid parameter(s)
	kHDMVDecErr_NotEnoughMemory					= (kHDMVDec_ErrorBase - 4),		// Insufficient memory or a memory allocation failed
	kHDMVDecErr_InvalidState					= (kHDMVDec_ErrorBase - 5),		// Cannot perform requested operation in curr state (e.g., IG user op when paused)
	kHDMVDecErr_NotAvailable					= (kHDMVDec_ErrorBase - 6),		// operation/action not available
	kHDMVDecErr_MouseInputIgnored				= (kHDMVDec_ErrorBase - 7)		// no action taken (for mouse based input)
};


// MenuButtonInfo
/// @brief Describes a IG Menu Button
///
class MenuButtonInfo
{
public:
	MenuButtonInfo() : m_bValid(false), m_BtnId(kButtonId_Invalid), m_bUserClickable(false)
					 , m_Left(0), m_Top(0), m_Right(0), m_Bottom(0)
					 , m_BtnIdLeft(kButtonId_Invalid), m_BtnIdRight(kButtonId_Invalid)
					 , m_BtnIdTop(kButtonId_Invalid), m_BtnIdBottom(kButtonId_Invalid)													{ }
	~MenuButtonInfo()																													{ }	

	void Reset()
	{
		m_bValid = false;
		m_BtnId = kButtonId_Invalid;
		m_bUserClickable = false;
		m_Left = m_Top = m_Right = m_Bottom = 0;
		m_BtnIdLeft = m_BtnIdRight = m_BtnIdTop = m_BtnIdBottom = kButtonId_Invalid;
	}

	void SetValidity(bool bValid)											{ m_bValid = bValid; }
	void SetButtonInfo(UInt16 btnId, UInt16 numSelVal, bool bClickable)		{ m_BtnId = btnId; m_NumericSelectValue = numSelVal; m_bUserClickable = bClickable; }
	void SetBoundingRect(UInt16 l, UInt16 t, UInt16 r, UInt16 b)			{ m_Left = l; m_Top = t; m_Right = r; m_Bottom = b; }
	void SetAdjacencyInfo(UInt16 l, UInt16 r, UInt16 t, UInt16 b)			{ m_BtnIdLeft = l; m_BtnIdRight = r; m_BtnIdTop = t; m_BtnIdBottom = b; }

public:
	bool				m_bValid;	// true if the rest of the information in this object is valid (if true, it means button is visible too)

	UInt16				m_BtnId;

	// Indicates if the User can click on this button to activate it.
	// (these are buttons that have nav commands associated with them and are not auto-activate buttons)
	bool				m_bUserClickable;

	// This is the Number users can enter to select this button. kButtonNumbericSelectVal_None if none is provided
	UInt16				m_NumericSelectValue;

	// Button's bounding rectangle (in video resolution coordinates)
	UInt16				m_Left;
	UInt16				m_Top;
	UInt16				m_Right;
	UInt16				m_Bottom;

	// Adjacency details: Ids of the buttons the left/right/top/bottom of this button
	UInt16				m_BtnIdLeft;
	UInt16				m_BtnIdRight;
	UInt16				m_BtnIdTop;
	UInt16				m_BtnIdBottom;
};

// MenuPageButtonInfo
/// @brief Describes an IG Menu Page and buttons in it
///
class MenuPageButtonInfo
{
public:
	MenuPageButtonInfo() : m_refCount(1), m_bUsable(false), m_PageId(kPageId_Invalid), m_PageVer(0), m_SelBtnId(kButtonId_Invalid), m_NumBtns(0)	{ }
	~MenuPageButtonInfo()																															{ }

	void Reset()
	{
		m_bUsable  = false;
		m_PageId   = kPageId_Invalid;
		m_PageVer  = 0;
		m_NumBtns  = 0;
		m_SelBtnId = kButtonId_Invalid;
		for (UInt32 i = 0; i < kMaxButtonsPerPage; i++)
			m_Btns[i].Reset();
		m_lastX = m_lastY = 0;
	}

	void SetUsability(bool bUsable)										{ m_bUsable = bUsable; }

	void SetPageInfo(UInt8 pageId, UInt8 pageVer)						{ m_PageId = pageId; m_PageVer = pageVer; }
	void SetNumberOfButtons(UInt8 numBtns)								{ m_NumBtns = numBtns; }
	void SetSelectedButtonId(UInt16 btnId)								{ m_SelBtnId = btnId; }

	void SetButtonValidity(UInt8 idx, bool bValid)									{ m_Btns[idx].SetValidity(bValid); }
	void SetButtonInfo(UInt8 idx, UInt16 btnId, UInt16 numSelVal, bool bClickable)	{ m_Btns[idx].SetButtonInfo(btnId, numSelVal, bClickable); }
	void SetButtonBoundingRect(UInt8 idx, UInt16 l, UInt16 t, UInt16 r, UInt16 b)	{ m_Btns[idx].SetBoundingRect(l, t, r, b); }
	void SetButtonAdjacencyInfo(UInt8 idx, UInt16 l, UInt16 r, UInt16 t, UInt16 b)	{ m_Btns[idx].SetAdjacencyInfo(l, r, t, b); }

	bool IsInBtnRect(SInt32 x, SInt32 y, UInt8 i)
				{ return ((x >= m_Btns[i].m_Left) && (x <= m_Btns[i].m_Right) && (y >= m_Btns[i].m_Top) && (y <= m_Btns[i].m_Bottom)); }

	// control lifetime
	UInt32 AddRef()				{	return m_refCount.Increment(); }
	UInt32 Release()			{	assert(m_refCount.Count() > 0);
									UInt32 count = m_refCount.Decrement();
									if (count == 0)
										delete this;
									return count; 
								}
	inline UInt32 GetCount()	{	return m_refCount.Count(); }

public:
	SonicCounter		m_refCount;					// ref-count of this object

	bool				m_bUsable;					// indicates if the information in this class is usable for mouse input

	UInt8				m_PageId;					// Menu Page Id. 0x00 - 0xFE are valid page IDs. kPageId_Invalid (0xFF) when invalid
	UInt8				m_PageVer;					// Page version number

	UInt16				m_SelBtnId;					// currently selected button's button id (update caused by GC after btn sel)

	SInt32				m_lastX;					// previous x position of mouse ptr
	SInt32				m_lastY;					// previous y position of mouse ptr

	UInt8				m_NumBtns;					// Number of buttons in page (max 255 - same as number of BOGs)
	MenuButtonInfo		m_Btns[kMaxButtonsPerPage];	// Details of all currently valid buttons in the page (i.e., 0 or 1 per BOG)
};

}; // namespace BDROM_HDMVDecoder


//--------------------------------------------------------------------------
// HDMV Stream Playlist Location
// Location in main path.
//

class HDMVStreamPlaylistLocation
{
public:
	HDMVStreamPlaylistLocation()
		: m_PlaylistPts90k(0), m_PlatformPts(0), m_bPlatformPtsValid(false)
	{
	}
	HDMVStreamPlaylistLocation(TimeValue playlistPts90k)
		: m_PlaylistPts90k(playlistPts90k), m_PlatformPts(0), m_bPlatformPtsValid(false)
	{
	}
	HDMVStreamPlaylistLocation(TimeValue playlistPts90k, TimeValue platformPts)
		: m_PlaylistPts90k(playlistPts90k), m_PlatformPts(platformPts), m_bPlatformPtsValid(true)
	{
	}

	bool operator==(HDMVStreamPlaylistLocation location)
	{
		return (m_PlaylistPts90k == location.m_PlaylistPts90k);
	}
	bool operator!=(HDMVStreamPlaylistLocation location)
	{
		return (m_PlaylistPts90k != location.m_PlaylistPts90k);
	}
	bool operator<=(HDMVStreamPlaylistLocation location)
	{
		return (m_PlaylistPts90k <= location.m_PlaylistPts90k);
	}
	bool operator>=(HDMVStreamPlaylistLocation location)
	{
		return (m_PlaylistPts90k >= location.m_PlaylistPts90k);
	}
	bool operator<(HDMVStreamPlaylistLocation location)
	{
		return (m_PlaylistPts90k < location.m_PlaylistPts90k);
	}
	bool operator>(HDMVStreamPlaylistLocation location)
	{
		return (m_PlaylistPts90k > location.m_PlaylistPts90k);
	}

public:
	TimeValue	m_PlaylistPts90k;		// 90K PTS Media Time
	TimeValue	m_PlatformPts;			// Platform PTS associated with this point. Valid only if bPlatformPtsValid is true;
	bool		m_bPlatformPtsValid;	// validity of platformPts
};

//--------------------------------------------------------------------------
// HDMV Stream Extent
// Information on the extent of main path that a HDMV stream should be synced to.
//

class HDMVStreamExtent
{
public:
	HDMVStreamExtent()
		: m_InPts90k(0), m_OutPts90k(0)
	{
	}
	HDMVStreamExtent(HDMVStreamPlaylistLocation inLoc, HDMVStreamPlaylistLocation outLoc, TimeValue inPts90k, TimeValue outPts90k)
		: m_InLocation(inLoc), m_OutLocation(outLoc), m_InPts90k(inPts90k), m_OutPts90k(outPts90k)
	{
	}

	void SetExtent(HDMVStreamPlaylistLocation inLoc, HDMVStreamPlaylistLocation outLoc, TimeValue inPts90k, TimeValue outPts90k)
	{
		m_InLocation  = inLoc;
		m_OutLocation = outLoc;
		m_InPts90k    = inPts90k;
		m_OutPts90k   = outPts90k;
	}
	void SetExtent(HDMVStreamExtent & extent)
	{
		m_InLocation  = extent.m_InLocation;
		m_OutLocation = extent.m_OutLocation;
		m_InPts90k    = extent.m_InPts90k;
		m_OutPts90k   = extent.m_OutPts90k;
	}

	bool operator==(HDMVStreamExtent extent)
	{
		return ((m_InLocation == extent.m_InLocation) && (m_OutLocation == extent.m_OutLocation) &&
				(m_InPts90k == extent.m_InPts90k) && (m_OutPts90k = extent.m_OutPts90k));
	}

public:
	HDMVStreamPlaylistLocation	m_InLocation;	// Location in Main Path where this HDMV stream's start is synced to
	HDMVStreamPlaylistLocation	m_OutLocation;	// Location in Main Path where this HDMV Stream's end is synced to
	TimeValue					m_InPts90k;		// 90KHz IN pts of playitem/subplayitem that has the HDMV Stream
	TimeValue					m_OutPts90k;	// 90KHz OUT pts of playitem/subplayitem that has the HDMV Stream
};


#endif // __HDMVDECODER_TYPES_H__
