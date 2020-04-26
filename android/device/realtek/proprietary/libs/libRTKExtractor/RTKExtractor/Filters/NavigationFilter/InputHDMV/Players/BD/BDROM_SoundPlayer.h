//-----------------------------------------------------------------------------
// BDROM_SoundPlayer.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __BDROM_SOUNDPLAYER_H__
#define __BDROM_SOUNDPLAYER_H__

#include "ref_ptr.h"
//#include <map>
#include "BaseRenderer.h"
#include "BDROMNavigationCache.h"
#include "BDROM_NavigatorErrors.h"
#include "ModuleManager.h"

using namespace BDROM;

//_______________________________________________________________________________________________________________________________________
// Defines
//

// ID types
typedef UInt8		BDROMSoundID;			///< ID of a sound within a sound buffer
typedef UInt16		BDROMSoundBufferID;		///< ID of a sound buffer
//typedef UInt32		BDROMUniqueSoundID;		///< Unique SoundID created from a BDROMSoundBufferID and a BDROMSoundID

// maps
//typedef std::map<BDROMUniqueSoundID, SoundID>		SoundIdMap;	///< Maps Sound Player generated BDROMUniqueSoundID (Key) to renderer genereated SoundID (Value)
//typedef std::map<BDROMUniqueSoundID, SoundID>		RendererIdMap;	///< Maps renderer genereated SoundID (Key) to Sound Player generated BDROMUniqueSoundID (Value)

// Pre-defined soundbuffer_ids
//const BDROMSoundBufferID kBDROMSoundBufferId_Invalid			= 0xFFFF;	///< Invalid Id
//const BDROMSoundBufferID kBDROMSoundBufferId_All				= 0xFEEE;	///< All sound buffers (including Sound.bdmv)
//const BDROMSoundBufferID kBDROMSoundBufferId_AllButSoundBDMV	= 0xFDDD;	///< All sound buffers, except for Sound.bdmv
const BDROMSoundBufferID kBDROMSoundBufferId_SoundBDMV			= 0;		///< Sound.bdmv file on the BDROM disc

const SoundID kBDROMRendererSoundId_Invalid						= 0xFFFFFFFF;	///< Invalid Id that will never be returned by renderer
//const BDROMUniqueSoundID kBDROMUniqueSoundID_Invalid			= 0xFFFFFFFF;

// creates a unique BDROM SOUND ID for each sound from the given soundBufferId & soundId
//#define MAKE_UNIQUE_BDROM_SOUNDID(soundBufferId, soundId)		((soundBufferId << 16) | soundId)

// creates a soundBufferId from the unique BDROM SOUND ID
//#define MAKE_SOUNDBUFFERID(bdromUniqueSoundId)	((bdromUniqueSoundId >> 16) & 0xFFFF)

// creates a soundId from the unique BDROM SOUND ID
//#define MAKE_SOUNDID(bdromUniqueSoundId)		(bdromUniqueSoundId & 0xFFFF)

//_______________________________________________________________________________________________________________________________________
/// @class BDROM_SoundData
/// @brief Sound data pointer, size and attributes
///
struct BDROM_SoundData
{
	unsigned char*		pDataCachedAddr;			//noncached address
	unsigned long		pDataPhyAddr;	//pointer to phyAddr
	int					soundDataSize;
	int					channelNum;
	int					sampleRate;
	int					bitsPerSample;
	char				bIsBigEndian;
	char				refID;
};


//_______________________________________________________________________________________________________________________________________
/// @class BDROM_SoundBuffer
/// @brief Sound Buffer for 1 or more sound data in the same format as sound.bdmv
///
/// * Each BDROM_SoundBuffer has a unique soundbuffer_id, which is set by the BDROM_SoundPlayer when this sound buffer is added to
///   the BDROM_SoundPlayer.
/// * The creator of BDROM_SoundBuffer manages the memory that contains sound data.
///
#if 0
class BDROM_SoundBuffer
{
public:
	BDROM_SoundBuffer(UInt8 * pBuffer, UInt32 uiSize, BDROMSoundBufferID id);
	~BDROM_SoundBuffer();

	BDROMSoundBufferID GetId(void)								{ return m_Id; }

public:
	UInt8		GetNumberOfSounds(void) { return m_numberOfSounds; }
	SonicError	GetSound(BDROMSoundID uiSoundId, BDROM_SoundData &sound);
	//SonicError	IsSoundDataBE(BDROMSoundID uiSoundId, bool &soundDataBE);
	//SonicError	SetSoundDataBE(BDROMSoundID uiSoundId, bool soundDataBE);
	//void		ShowSoundDataBE();

private:
	BDROM_SoundBuffer();											// no default constructor
	BDROM_SoundBuffer(const BDROM_SoundBuffer&);					// no copying
	const BDROM_SoundBuffer& operator=(const BDROM_SoundBuffer&);	// no copying

private:
	BDROMSoundBufferID		m_Id;
	UInt8 *					m_pBuffer;
	UInt32					m_BufferSize;
	UInt8					m_numberOfSounds;
	//bool *					m_pSoundDataBE;
};

#endif
//_______________________________________________________________________________________________________________________________________
/// @class BDROM_SoundPlayer
/// @brief Plays sounds from sound buffers
///
/// * Sound.bdmv is automatically loaded. Its soundbuffer_id is predefined to be kBDROMSoundBufferId_SoundBDMV.
///   If the platform that the BDROM player runs on loads Sound.bdmv, then BDROM_SoundPlayer's bLoadSoundBdmv param should be set to false.
///   If the platform does not load it, pass true for bLoadSoundBdmv, which will make this player load Sound.bdmv.
/// * All other sound buffers must first be added to the Sound Player (by calling AddSoundBuffer) before a sound can be played from it.
///
/// Use Case:
/// 01. Call Sound Player's AddSoundBuffer() to create a new sound buffer and retrieve its soundbuffer_id.
/// 02. Use PlaySound() to play sound, StopSound() to stop a sound before it has finished playing.
/// 03. Call DeleteSoundBuffer() to remove 1 or all sound buffer(s).
///
class BDROM_SoundPlayer
{
public:
	// construct BDROM_Player and tell it how to manage Sound.bdmv
	BDROM_SoundPlayer(ref_ptr<BaseRenderer> pRenderer, ref_ptr<BDROM_NavigationCache> pNavCache, bool bLoadSoundBdmv);
	~BDROM_SoundPlayer();

    void SetModuleManager(ref_ptr<ModuleManager> & pModuleManager) {  m_pModuleManager = pModuleManager; }

	// creates a new sound buffer. 
	//SonicError AddSoundBuffer(UInt8 * pBuffer, UInt32 uiSize, BDROMSoundBufferID & soundBufferId);
	// delete a single or all sound buffer(s)
	//void DeleteSoundBuffer(BDROMSoundBufferID soundBufferId);

	// Creates a sound buffer (either from sounds.bdmv or nav module, e.g. BD-J, created sound).
	void CreateSound(BDROMSoundBufferID soundBufferId, BDROMSoundID uiSoundId);
	// Plays a sound buffer (either from sounds.bdmv or nav module, e.g. BD-J, created sound).
	void PlaySound(BDROMSoundBufferID soundBufferId, BDROMSoundID uiSoundId);
	// Stops playback of a sound
	//void StopSound(BDROMSoundBufferID soundBufferId, BDROMSoundID uiSoundId);
	// Closes sound channel
	//void CloseSound(BDROMSoundBufferID soundBufferId, BDROMSoundID uiSoundId, bool eventProcessing=false);
	// Stops all sounds being played from a sound buffer
	//void StopSoundBuffer(BDROMSoundBufferID soundBufferId);
	// Stops and closes all opened sounds in a sound buffer
	//void CloseSoundBuffer(BDROMSoundBufferID soundBufferId, bool eventProcessing);
	// Closes all sound buffers
	//void CloseAllSoundBuffers(BDROMSoundBufferID soundBufferId);

	// Get duration of sound in milliseconds
	SonicError GetSoundDuration(BDROMSoundBufferID soundBufferId, BDROMSoundID uiSoundId, UInt32 & durationMs);

	SoundID GetRendererSoundId(BDROMSoundBufferID soundBufferId, BDROMSoundID uiSoundId);
	//BDROMUniqueSoundID GetBDROMUniqueSoundId(SoundID rendererSoundId);
	
	//void ShowBDROMSoundIdMap();
	//void ShowBDROMSoundBufferIdMap();

private:
	//BDROM_SoundPlayer();											// no default constructor
	//BDROM_SoundPlayer(const BDROM_SoundPlayer&);					// no copying
	//const BDROM_SoundPlayer& operator=(const BDROM_SoundPlayer&);	// no copying

	// retrieves pointer to sound buffer
	//BDROM_SoundBuffer * GetSoundBuffer(BDROMSoundBufferID soundBufferId);
	void 			ReadSoundBdmv(BD_FS_rt_stream * pSoundBDMVFile);
	//void			ParseSoundBuffer();
	//SonicError 		GetSound(BDROMSoundID uiSoundId, BDROM_SoundData & sound);
		
private:
	ref_ptr<BaseRenderer>				m_pRenderer;		///< Renderer API ptr
	ref_ptr<BDROM_NavigationCache>		m_pNavCache;		///< Navigation Cache
	ref_ptr<ModuleManager>              m_pModuleManager;

	BDROMSoundBufferID					m_ID;			///< Next SoundBuffer Id
	//char*								m_pSoundDataBE;	// -1 : unknown, 0 :little endian, 1: big endian
	
	bool								m_bLoadSoundBdmv;	///< true if Sound.bdmv is loaded by this player
	//unsigned char*						m_pSoundBdmvBufferCachedAddr;	///< cached addr of Buffer that contains Sound.bdmv.
	//unsigned char*						m_pSoundBdmvBuffer;	///< (noncached) Buffer that contains Sound.bdmv.
	//int									m_soundBdmvBufferSize;
	int									m_numberOfSounds;
	
	//long								m_phyAddrDiff;
	//SoundIdMap							m_SoundIdMap;		///< Maps BDROMUniqueSoundID to renderer returned soundIDs
	//RendererIdMap						m_RendererIdMap;	///< Maps renderer returned soundIDs to BDROMUniqueSoundIDs
	BDROM_SoundData*					m_pSoundData;
};

#endif
