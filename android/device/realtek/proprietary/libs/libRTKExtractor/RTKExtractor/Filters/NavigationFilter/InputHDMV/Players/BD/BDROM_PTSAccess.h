//-----------------------------------------------------------------------------
// BDROM_PTSAccess.h
// Copyright (c) 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __BDROM_PTSACCESS_H_
#define __BDROM_PTSACCESS_H_


//__________________________________________________________________________________________________________________________________________________
// BDROM_PTSAccess Interface
//

typedef enum tagBDROM_PTSType
{
	kBDROM_PTSType__MainPath,			// Main Path PTS
	kBDROM_PTSType__AsyncSubPath,		// Async Sub Path PTS (e.g. PTS of primary audio of a browsable slideshow, or Async PiP Path's PTS)
	kBDROM_PTSType__AsyncIG				// Async IG PTS (like for a popup menu path)
} BDROM_PTSType;


//--------------------------------------------------------------------------
/// @class BDROM_PTSAccess
/// @brief	Interface to get PTS of a BDROM Playback path
///
class BDROM_PTSAccess
{
public:
	BDROM_PTSAccess()				{}
	virtual ~BDROM_PTSAccess()		{}

	// returns current PTS as a 90 KHz value
	virtual bool GetCurrentPTS(BDROM_PTSType ptsType, Time90K & ts90kCurrentPTS)		 = 0;

	// returns current Playlist PTS as a 90 KHz value
	virtual bool GetCurrentPlaylistPTS(TimeValue & playlistPts90k)		 = 0;

	// returns current Playlist PTS as a 90 KHz value, and a platform PTS (0 if none available)
	virtual bool GetCurrentPlaylistPTS(TimeValue & playlistPts90k, TimeValue & platformPts90k)		 = 0;
};

#endif // __BDROM_PTSACCESS_H_
