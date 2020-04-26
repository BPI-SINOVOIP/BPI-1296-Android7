//-----------------------------------------------------------------------------
// SonicPBCE_Clips.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __SONICPBCE_CLIPS_H__
#define __SONICPBCE_CLIPS_H__

#include "rt_stream.h"
#include "SonicCriticalSection.h"
#include "BDROM_NavigatorTypes.h"
#include "BDROMNavigationCache.h"
#include "SonicPBCE_EventSink.h"
#include "BDROM_StreamControl.h"
#include "SonicPBCE_Registers.h"
#include "HDMVDecoder_Types.h"

#ifdef REALTEK_MODIFY
	#include <OSAL.h>
#else
	#include "BDROM_Settings.h"
#endif

//----------------------------------------------------------------
#define HDMV_RT_STREAM_DIRECT_IO_READ_SIZE	(BDROM_BYTES_IN_AU*2)
#define HDMV_MAX_READ_SIZE					(HDMV_RT_STREAM_DIRECT_IO_READ_SIZE*8)
#define HDMV_TS_BUFFER_SIZE					(HDMV_DIRECT_IO_BLOCK_SIZE+HDMV_MAX_READ_SIZE)

//----------------------------------------------------------------


//____________________________________________________________________________________________________________________________
// Clip Readers (and read-ahead buffers)
//

//--------------------------------------------------------------------------
// TSReader
// TS Reader interface
//
class TSReader
{
public:
	TSReader()				{}
	virtual ~TSReader()		{}

	// RePosition: Reposition read location. Should be called when a jump is needed
	virtual void       RePosition(UInt32 spn) = 0;

	// IsReady: Returns true if TSReader is ready to be used.
	virtual bool       IsReady(void) = 0;

	// Read: Read data from TS
	virtual SonicError Read(UInt32 readSPN, char * data, UInt32 bytesToRead, size_t & bytesRead)	= 0;
};


//--------------------------------------------------------------------------
// PassthruTSReader
// TS Reader implementation that always reads from drive.
//
class PassthruTSReader : public TSReader
{
public:
	PassthruTSReader(ref_ptr<BD_FS_rt_stream> pAVStream);
	~PassthruTSReader();

	void       RePosition(UInt32 spn);
	bool       IsReady(void);
	SonicError Read(UInt32 readSPN, char * data, UInt32 bytesToRead, size_t & bytesRead);

protected:
	ref_ptr<BD_FS_rt_stream>		m_pAVStream;
};


class BrowsableSlideshowBufferManager;

//--------------------------------------------------------------------------
// BrowsableSlideshowTSReader
// TS Reader implementation that should be used for reading MainTS/SubTS for Browsable Slideshow.
// This talks to a BrowsableSlideshowBufferManager to read data
//
class BrowsableSlideshowTSReader : public TSReader
{
public:
	enum TSType
	{
		MainTS,
		SubTS
	};

	BrowsableSlideshowTSReader(	BrowsableSlideshowTSReader::TSType type,
								ref_ptr<BD_FS_rt_stream> pAVStream,
								UInt32 SPNsInFile,
								UInt32 recRateBytesPerSec,
								BrowsableSlideshowBufferManager * pBufferMngr);
	~BrowsableSlideshowTSReader();

	void       RePosition(UInt32 spn);
	bool       IsReady(void);
	SonicError Read(UInt32 readSPN, char * data, UInt32 bytesToRead, size_t & bytesRead);

protected:
	TSType								m_TSType;
	BrowsableSlideshowBufferManager	*	m_pBufferMngr;
};


//--------------------------------------------------------------------------
// BrowsableSlideshowBufferManager
//	Manages read-ahead buffering for Browsable Slideshows. Uses its own thread to do the buffering
//	See BDROM Part 3-1 Annex E for more information on buffering for Browsable Slideshows.
//	This Buffer Manager uses 2 read-ahead buffers and a thread to drive the buffering.
//	It completely fills one buffer before switching to filling the other buffer.
//
#ifdef REALTEK_MODIFY_DISABLE_SONICTHREAD
class BrowsableSlideshowBufferManager
#else
class BrowsableSlideshowBufferManager : public SonicThread
#endif	
{
public:
	BrowsableSlideshowBufferManager();
	~BrowsableSlideshowBufferManager();

	enum BufferType
	{
		MainTSBuffer,
		SubTSBuffer
	};

	void Activate(BufferType bufType, ref_ptr<BD_FS_rt_stream> pAVStream, UInt32 SPNsInFile, UInt32 recRateBytesPerSec);
	void Deactivate(BufferType bufType);

	void       RePosition(BrowsableSlideshowTSReader::TSType TsType, UInt32 spn);
	SonicError Read(BrowsableSlideshowTSReader::TSType TsType, UInt32 readSPN, char * data, UInt32 bytesToRead, size_t & bytesRead);

	UInt32 GetNumberOfActiveBuffers(void);
	bool   IsBufferActive(BrowsableSlideshowTSReader::TSType TsType);
	bool   IsBufferReady(BrowsableSlideshowTSReader::TSType TsType);

#ifdef 	REALTEK_MODIFY_DISABLE_SONICTHREAD
	static void ThreadEntry(void* pInstance);
protected:
	// thread management
	Priority GetPriority(void)		{ return Priority_High; }
	SonicError Run(void);
#else
protected:
	// thread management
	virtual Priority GetPriority(void)		{ return Priority_High; }
	virtual SonicError Run(void);
	
#endif

protected:

	//--------------------------------------------------------------------------
	// ReadAheadBuffer
	//	The read-ahead buffer. Buffers data in either 'streaming' or 'pre-loaded' mode.
	//	Pre-loaded mode is used for clips that are small enough to be fit into the
	//	read-ahead buffer and don't require to be re-filled on a seek/re-position operation.
	//
	class ReadAheadBuffer
	{
	public:
		ReadAheadBuffer();
		~ReadAheadBuffer();

		static UInt32		READ_AHEAD_SIZE;

		bool Init(ref_ptr<BD_FS_rt_stream> pAVStream, UInt32 SPNsInFile, UInt32 recRateBytesPerSec, bool bMainTS);
		void DeInit(void);

		UInt8 *	GetBuffer(void)					{ return m_pReadAheadBuffer; }
		UInt32	GetBufferSize(void)				{ return m_ReadAheadBufSize; }

		bool	IsBufferActive(void)			{ return m_bBufferActive; }
		bool	IsBufferReady(void)				{ return m_bBufferReady; }

		bool	IsReadAheadNeeded(void)			{ return m_bBufferActive && m_bNeedsReadAhead; }
		void	DoReadAhead(void);

		void       RePosition(UInt32 spn);
		SonicError Read(UInt32 readSPN, char * data, UInt32 bytesToRead, size_t & bytesRead);

	protected:
		bool CreateBuffer(UInt32 size);
		void DestroyBuffer(void);

		bool IsBufferEmpty(void)		{ return ((m_pDataEnd == m_pDataStart) && (m_NumSPNsInBuf == 0)); }
		bool IsBufferFull(void)			{ return ((m_pDataEnd == m_pDataStart) && (m_NumSPNsInBuf == m_ReadAheadBufSizeInSPN)); }

		bool IsDataInBuffer(UInt32 spn, UInt32 bytes);

		SonicError GetReadabilityStatus(UInt32 readSPN, UInt32 bytesToRead);

		void AdvanceBufferPtr(UInt8* & ptr, UInt32 num)
		{
			UInt32 offset = (UInt32)(ptr - m_pReadAheadBuffer);
			if ((offset + num) < m_ReadAheadBufSize)
				ptr += num;
			else
				ptr = m_pReadAheadBuffer + ((offset + num) - m_ReadAheadBufSize);
		}

	protected:
		SonicCriticalSection		m_BufferLock;			// lock to be held when updating buffer state

		// debugging
		char						m_IdStr[7];

		// buffer
		UInt8 *						m_pReadAheadBuffer;		// The read-ahead buffer
		UInt32						m_ReadAheadBufSize;		// Size of Read-ahead buffer (in Bytes). Can be <= m_AllocatedSize
		UInt32						m_ReadAheadBufSizeInSPN;// Size of Read-ahead buffer (in SPNs).
		UInt32						m_AllocatedSize;		// Actual amount of memory allocated.

		// file
		ref_ptr<BD_FS_rt_stream>	m_pAVStream;
		UInt32						m_SPNsInFile;

		// data in buffer
		UInt32						m_FirstSPNinBuf;		// SPN of first TS packet in read-ahead buffer
		UInt32						m_NumSPNsInBuf;			// Number of SPNs in read-ahead buffer
		UInt8 *						m_pDataStart;			// First byte of data in buffer.
		UInt8 *						m_pDataEnd;				// 1 past Last byte of data in buffer

		// state
		enum BufferMode
		{
			eBM_Preloaded,		// all data is in buffer (happens only for files smaller than buffer size)
			eBM_Streaming		// need to keep streaming data from drive into buffer
		};

		bool						m_bBufferActive;		// Buffer in Use
		BufferMode					m_BufferMode;			// Mode in which buffer operates
		bool						m_bBufferReady;			// Indicates when buffer is ready to read from after a re-position
		bool						m_bNeedsReadAhead;		// Buffer to be filled
		UInt32						m_ReadAheadSPN;			// SPN to start read-ahead from

		bool						m_bReadAheadError;		// 'true' if read-ahead stopped due to a an error
		UInt32						m_ReadErrorStartSPN;	// start of SPN range that encountered error
		UInt32						m_ReadErrorEndSPN;		// end of SPN range that encountered error
	};


protected:
#ifdef REALTEK_MODIFY_DISABLE_SONICTHREAD
	char*					m_ThreadName;
#else	
	std::string				m_ThreadName;				// Name of this thread
#endif		
	bool					m_bExitThread;
	SonicSemaphore			m_semNewEvent;

	BufferType				m_BufferBeingReadAhead;

	ReadAheadBuffer			m_MainTSBuffer;
	ReadAheadBuffer			m_SubTSBuffer;
	
#ifdef REALTEK_MODIFY_DISABLE_SONICTHREAD
	CThread* 				m_pThread;
#endif	
};

//--------------------------------------------------------------------------
// TSReaderFactory
//	Returns appropriate TSReader for a give clip type
//
class TSReaderFactory
{
public:
	// Init for new playlist. To be called at start of new playlist
	static void PlaylistInit(void);

	// Final DeInit. To be called when PBCE is being destroyed
	static void DeInit(void);

	// Get TS Reader for a clip
	static ref_ptr<TSReader> GetTSReader(BDROM_ApplicationType clipApplType,
										 UInt32 recRateBytesPerSec,
										 ref_ptr<BD_FS_rt_stream> pAVStream,
										 UInt32 SPNsInFile);

protected:
	static BrowsableSlideshowBufferManager * GetBrowsableSlideshowBufferManager(void);

protected:
	static BrowsableSlideshowBufferManager * m_pBrowsableSlideshowBufferMngr;
};



//____________________________________________________________________________________________________________________________
// Clip and its state objects
//

//--------------------------------------------------------------------------
// BDROM_ClipState
// State of a PBCE_Clip
//
typedef enum tagPBCE_ClipState
{
										// --- state meaning ---					| --- allowed next state ---
	kPBCE_ClipState__Idle,				// uninitialized							| Pause (on loading a clip)
	kPBCE_ClipState__StartOfSegment,	// at new location							| Idle, StartOfSegment, Streaming
	kPBCE_ClipState__Streaming,			// streaming								| Idle, StartOfSegment, Paused, Error, EndOfSegment
	kPBCE_ClipState__Paused,			// streaming paused							| Idle, StartOfSegment, Streaming, Error, EndOfSegment
	kPBCE_ClipState__EndOfSegment,		// at end of segment (processing EOS)		| Stop, Idle, StartOfSegment
	kPBCE_ClipState__Stop,				// stopped streaming after EOS				| Idle, StartOfSegment
	kPBCE_ClipState__Error,				// stopped streaming clip due to an error	| Idle, StartOfSegment
	kPBCE_ClipState__ReadingPaused		// turned off clip reading (to use in transitions)	| Back to old state (must be cached before setting it to this state)
} PBCE_ClipState;


//--------------------------------------------------------------------------
// ClipStream
// PID info for a stream in the clip
//

class ClipStream
{
public:
	ClipStream()
	{
		Clear();
	}

public:
	UInt32		m_PID;				// stream PID
	bool		m_bDirty;			// true if m_PID has been changed (between 2 reads)

public:
	void SetPID(UInt32 PID)
	{
		m_PID = PID;
		m_bDirty = true;
	}
	UInt32 GetPID(void)			{ return m_PID; }

	void SetDirty(bool bDirty)	{ m_bDirty = bDirty; }
	bool IsDirty(void)			{ return m_bDirty; }

	void Clear(void)
	{
		m_PID = BDROM_NULL_PACKET_PID;
		m_bDirty = false;
	}
};


//--------------------------------------------------------------------------
// BDROM_SonicPBCE_ClipPath
// Path specific data of a Path attached to a Clip.
// There is one of these for every path that is pulling data out of a clip. As per BDROM spec, at most 2 paths will be pulling data from a single clip
//

class BDROM_SonicPBCE_ClipPath
{
public:
	BDROM_SonicPBCE_ClipPath()
	{
		Reset();
	}

public:
	bool								m_bPathActive;				// true if this path attached to clip is active

	ClipStream							m_PrimaryVideo;				// primary video stream
	ClipStream							m_PrimaryAudio;				// primary audio stream
	ClipStream							m_IG;						// IG PID
	ClipStream							m_PGText;					// PG/TextST or PiP PG/TextST PID
	bool								mbPGTextSTisText;			// true if m_PGText refers to a TextST stream PID
	ClipStream							m_SecondaryVideo;			// secondary video PID
	ClipStream							m_SecondaryAudio;			// secondary audio PID

	Time90K								m_pts90k_Start;				// start PTS (90 KHz) of the playtiem/subplayitem
	Time90K								m_pts90k_End;				// end PTS (90 KHz) of the playtiem/subplayitem
	Time90K								m_pts90k_PlaybackStart;		// playback start PTS (90 KHz) of the playtiem/subplayitem (>= start PTS, <= end PTS)

	UInt32								m_uiSPNStart;				// Start SPN of the Clip. May not be AU Aligned.
	UInt32								m_uiSPNEnd;					// End SPN of the Clip. May not be AU Aligned.

	PBCE_PathType						m_PBCEPathType;				// PBCE Path Type
	BDROM_SubPathType					m_SubPathType;				// SubPath type if PBCE Path Type is not Main Path

public:
	void Reset(void)
	{
		m_bPathActive = false;
		m_PrimaryVideo.Clear();
		m_PrimaryAudio.Clear();
		m_IG.Clear();
		m_PGText.Clear();
		m_SecondaryVideo.Clear();
		m_SecondaryAudio.Clear();
		mbPGTextSTisText = false;
		m_pts90k_Start = m_pts90k_PlaybackStart = 0;
		m_pts90k_End = 0xFFFFFFFF;
		m_PBCEPathType = kPBCE_PathType__Unknown;
		m_SubPathType  = kBDROM_SubPathType__Reserved_0;
	}
};


//--------------------------------------------------------------------------
// BDROM_SonicPBCE_ClipAngleChangeInfo
// Angle change information stored within a Clip
//

struct BDROM_SonicPBCE_ClipAngleChangeInfo
{
public:
	std::string		m_strClipFilename;			///< Name of the new angle's clip
	UInt32			m_uiStcId;					///< STC_id of the new angle clip streams
	bool			m_bSeamlessAngleChange;		///< true if seamless angle change is to be done
	bool			m_bDifferentAudio;			///< true if new angle can have diffent audio type for same PID. valid only if bSeamlessAngleChange is true.
	Time90K			m_pts90kTarget;				///< Target time to start playback at in the new angle. valid only if bSeamlessAngleChange is false
	UInt32			m_uiAngleNumber;			///< Target angle number

public:
	BDROM_SonicPBCE_ClipAngleChangeInfo()		{ Reset(); }

	void Reset(void)
	{
		m_strClipFilename = "";
		m_uiStcId = 0;
		m_bSeamlessAngleChange = false;
		m_bDifferentAudio = false;
		m_pts90kTarget = 0;
		m_uiAngleNumber = 0;
	}
};


//--------------------------------------------------------------------------
// Event
//
#ifdef WIN32
#include "WinEvent.h"
#define kPBCEEvent_INFINITE		INFINITE

class SonicPBCE_Event : WinEvent
{
public:
	SonicPBCE_Event() : WinEvent()						{}
	~SonicPBCE_Event()									{}

	bool Set()											{ return WinEvent::Set() == TRUE; }
	bool Reset()										{ return WinEvent::Reset() == TRUE; }
	bool Pulse()										{ return WinEvent::Pulse() == TRUE; }

	bool Wait(UInt32 ms = kPBCEEvent_INFINITE)			{ return WinEvent::Wait(ms) == TRUE; }
};

#else // WIN32

#define kPBCEEvent_INFINITE		0

class SonicPBCE_Event
{
public:
	SonicPBCE_Event()									{}
	~SonicPBCE_Event()									{}

	bool Set()											{ return true; }
	bool Reset()										{ return true; }
	bool Pulse()										{ return true; }

	bool Wait(UInt32 ms = kPBCEEvent_INFINITE)			{ /*ms;*/ return true; }
};
#endif // WIN32


//--------------------------------------------------------------------------
// HDMV Stream Extent information stored in Clip
//

class ClipHDMVStreamExtent : public HDMVStreamExtent
{
public:
	ClipHDMVStreamExtent() : HDMVStreamExtent(), m_bDirty(false), m_PID(BDROM_NULL_PACKET_PID) {}

	void SetExtent(UInt32 pid, HDMVStreamExtent & extent)
	{
		HDMVStreamExtent::SetExtent(extent);
		m_PID = pid;
		m_bDirty = true;
	}
	void SetExtent(UInt32 pid, HDMVStreamExtent * pExtent)
	{
		HDMVStreamExtent::SetExtent(*pExtent);
		m_PID = pid;
		m_bDirty = true;
	}
	HDMVStreamExtent & GetExtent(void)
	{
		return *this;
	}

	bool   IsDirty(void)			{ return m_bDirty; }
	UInt32 GetPID(void)				{ return m_PID; }
	void   SetDirty(bool bDirty)	{ m_bDirty = bDirty; }

protected:
	bool		m_bDirty;
	UInt32		m_PID;
};


//--------------------------------------------------------------------------
// BDROM_SonicPBCE_Clip
// A BDROM Playback Clip
//

class BDROM_SonicPBCE_PlayItem;

class BDROM_SonicPBCE_Clip : public rt_stream
{
public:
	BDROM_SonicPBCE_Clip(ref_ptr<SonicCriticalSection> pcsRead, BDROM_StreamSourceType sourceType, ref_ptr<BDROM_StreamControl> pStreamControl,
						 ref_ptr<BDROM_SonicPBCE_Registers> pRegisters, BDROM_SonicPBCE_EventSink * pEventSink, BDROM_DiscType discType,
					#ifdef REALTEK_MODIFY	 
						void*					pSettings);
					#else
						 ref_ptr<BDROM_Settings> pSettings);
					#endif	 
	virtual ~BDROM_SonicPBCE_Clip();

	// Init or re-init
	SonicError Init(ref_ptr<BDROM::BDROM_NavigationCache> pNavCache);
	static void SetReadSyncEvents(SonicPBCE_Event* pReadSyncStartEvent);
	void Reset(void);
	void Deinit(void);			// after a de-init, clip object cannot be re-used (only destructed).

	// clip load/un-load
	SonicError LoadClip(std::string strClipFilename);
	SonicError UnloadClip(void);

	void SpinUp(void);

	// set clip properties
	ref_ptr<BDROM::ClipInfo_clpi> GetClipInfo(void)			{ return m_pClipInfo_clpi; }
	/*
	void SetClipPID(PBCE_PathType pathType, BDROM_StreamType streamType, UInt32 uiPID);
	*/
	void SetClipSTCid(UInt32 STCid);
	void SetInOutTimes(PBCE_PathType pathType, Time90K pts90kStart, Time90K pts90kEnd);
	void SetSeamlessStart(bool bSeamless);		// previous clip seamless connected to this one
	void SetSeamlessStop(bool bSeamless);		// this clip will be seamlessly connected to next clip
	void SetStartOffsetPTS(SInt64 ts45kOffsetPTS);
	void SelectStream(PBCE_PathType pathType, BDROM_StreamType streamType, UInt32 uiPID, BDROM::stream_attributes * pStreamAttr,
					  bool bFlushOld = false, HDMVStreamExtent * pHdmvStreamExtent = 0);
	void StreamOnOff(BDROM_StreamType streamType, bool bOn, bool bIsText = false);
//	void SetAssociatedPlayItem(BDROM_SonicPBCE_PlayItem* playItem);
	void SetPreLoad(bool bPreLoad);
	void SetMainPathClip(bool bMainPathClip);
	void SetPathType(PBCE_PathType pathType, BDROM_SubPathType subPathType = kBDROM_SubPathType__Reserved_0);
	void AttachPath(PBCE_PathType pathType);
	void DetachPath(PBCE_PathType pathType);

	// special function used only for AVCHD menus
	void SetMenuStream(bool bIsMenuStream)			{ m_bIsMenuStream = bIsMenuStream; }
	bool GetMenuStream(void)						{ return m_bIsMenuStream; }

	// for seamlessly connected clips, this call tells the clip that is now being played
	void ClipIsPlaying(void);
	bool IsClipEnded(void);

	// get clip properties
	UInt32 GetClipPID(PBCE_PathType pathType, BDROM_StreamType streamType);
	SonicError GetStreamAttributes(UInt32 uiPID, BDROM_Stream & streamAttr);
	SonicError GetCurrentVideoAttributes(BDROM_VideoAttributes & vatr);
	SonicError GetCurrentAudioAttributes(BDROM_AudioAttributes & aatr);
	BDROM_ApplicationType GetApplicationType(void);
	bool GetPreLoad(void);
	bool IsMainPathClip(void);
	PBCE_ClipState GetState(void)											{ return m_State; }

	// set playitem info (used *ONLY* for setting playitem id in media sample properties)
	void SetSampleMetadataPlayitemId(SInt32 playitemId)						{ m_PlayitemId = playitemId; }

	// clip start, pause, stop, search
	SonicError StartAtTime(PBCE_PathType pathType, Time90K pts90kTime);
	SonicError Stop(void);
	SonicError Start(void);
	SonicError PauseReading(void);
	SonicError ResumeReading(void);
	SonicError PauseOn(void);
	bool       IsPaused(void);
	SonicError PauseOff(void);
	bool       IsRateChangeAllowed(void);
	SonicError SetRate(BDROM_PlayDirection direction, double dSpeed);
//	SonicError GetRate(BDROM_PlayDirection & direction, double & dSpeed);

	// angle change
	SonicError ChangeAngle(std::string strClipFilename, UInt32 uiStcId, bool bSeamlessAngleChange, bool bDifferentAudio, Time90K pts90kTarget, UInt32 uiAngleNumber);

public: // ------- Dell Demo specific DS Filter requires these functions --------
	void GetMainPathSegmentTimes(Time90K & pts90kSegmentStart, Time90K & pts90kSegmentEnd, Time90K & pts90kTimeBase, Time90K & ts90kElapsedTime);
	void GetCurrentStreamPIDs(UInt32 & uiAudioPid, UInt32 & uiVideoPid);

protected:
	// clip loading
	SonicError LoadNewClip(std::string strClipFilename);

	// returns ptr to a info about path attached to clip
	BDROM_SonicPBCE_ClipPath * GetClipPath(PBCE_PathType pathType)
	{
		if (pathType == kPBCE_PathType__MainPath)
			return &m_MainPathStream;
		else
			return &m_SubPathStream;
	}

	// stream attributes related functions
	void ExtractVideoAttributes(BDROM::StreamInfo * pStreamInfo, BDROM_VideoAttributes * pVideoAttr)
	{
		assert(pStreamInfo);
		assert(pVideoAttr);
		pVideoAttr->codingType  = (BDROM_StreamCodingTypes)((UInt32)pStreamInfo->mStreamCodingInfo.mstream_coding_type);
		pVideoAttr->videoFormat = (BDROM_VideoFormat)      ((UInt32)pStreamInfo->mStreamCodingInfo.mvideo_format_table.mvideo_format);
		pVideoAttr->aspectRatio = (BDROM_VideoAspectRatio) ((UInt32)pStreamInfo->mStreamCodingInfo.mvideo_format_table.maspect_ratio);
		pVideoAttr->frameRate   = (BDROM_VideoFrameRate)   ((UInt32)pStreamInfo->mStreamCodingInfo.mvideo_format_table.mframe_rate);
		pVideoAttr->bLine21Data = (pStreamInfo->mStreamCodingInfo.mvideo_format_table.mcc_flag == 1);
	}

	void ExtractAudioAttributes(BDROM::StreamInfo * pStreamInfo, BDROM_AudioAttributes * pAudioAttr)
	{
		assert(pStreamInfo);
		assert(pAudioAttr);
		pAudioAttr->codingType       = (BDROM_StreamCodingTypes)    ((UInt32)pStreamInfo->mStreamCodingInfo.mstream_coding_type);
		pAudioAttr->samplingFreq     = (BDROM_AudioSamplingFreq)    ((UInt32)pStreamInfo->mStreamCodingInfo.maudio_format_table.msampling_frequency);
		pAudioAttr->presentationType = (BDROM_AudioPresentationType)((UInt32)pStreamInfo->mStreamCodingInfo.maudio_format_table.maudio_presentation_type);
		pAudioAttr->language[0]      = pStreamInfo->mStreamCodingInfo.maudio_format_table.maudio_language_code[1];
		pAudioAttr->language[1]      = pStreamInfo->mStreamCodingInfo.maudio_format_table.maudio_language_code[2];
		pAudioAttr->language[2]      = pStreamInfo->mStreamCodingInfo.maudio_format_table.maudio_language_code[3];
#if 1
		// let demux determine correct channel info
		pAudioAttr->audioChannels    = kBDROM_AudioChannels_Unknown;
#else
		if (pAudioAttr->presentationType == BDROM_AudioPres_SingleMono)
			pAudioAttr->audioChannels    = kBDROM_AudioChannels_1;
		else if (pAudioAttr->presentationType == BDROM_AudioPres_Stereo)
			pAudioAttr->audioChannels    = kBDROM_AudioChannels_2;
		else
			pAudioAttr->audioChannels    = kBDROM_AudioChannels_Unknown;
#endif
		pAudioAttr->bitsPerSample    = BDROM_AUDIO_BITS_PER_SAMPLE_NOINFO;
	}

	void ExtractPGAttributes(BDROM::StreamInfo * pStreamInfo, BDROM_GraphicsAttributes * pGraphicsAttr)
	{
		assert(pStreamInfo);
		assert(pGraphicsAttr);
		pGraphicsAttr->language[0] = pStreamInfo->mStreamCodingInfo.mPG_format_table.mPG_language_code[1];
		pGraphicsAttr->language[1] = pStreamInfo->mStreamCodingInfo.mPG_format_table.mPG_language_code[2];
		pGraphicsAttr->language[2] = pStreamInfo->mStreamCodingInfo.mPG_format_table.mPG_language_code[3];
	}

	void ExtractIGAttributes(BDROM::StreamInfo * pStreamInfo, BDROM_GraphicsAttributes * pGraphicsAttr)
	{
		assert(pStreamInfo);
		assert(pGraphicsAttr);
		pGraphicsAttr->language[0] = pStreamInfo->mStreamCodingInfo.mIG_format_table.mIG_language_code[1];
		pGraphicsAttr->language[1] = pStreamInfo->mStreamCodingInfo.mIG_format_table.mIG_language_code[2];
		pGraphicsAttr->language[2] = pStreamInfo->mStreamCodingInfo.mIG_format_table.mIG_language_code[3];
	}

	void ExtractTextSTAttributes(BDROM::StreamInfo * pStreamInfo, BDROM_GraphicsAttributes * pGraphicsAttr)
	{
		assert(pStreamInfo);
		assert(pGraphicsAttr);
		pGraphicsAttr->charCode    = (BDROM_CharSet)(UInt32)pStreamInfo->mStreamCodingInfo.mtextST_format_table.mcharacter_code;
		pGraphicsAttr->language[0] = pStreamInfo->mStreamCodingInfo.mtextST_format_table.mtextST_language_code[1];
		pGraphicsAttr->language[1] = pStreamInfo->mStreamCodingInfo.mtextST_format_table.mtextST_language_code[2];
		pGraphicsAttr->language[2] = pStreamInfo->mStreamCodingInfo.mtextST_format_table.mtextST_language_code[3];
	}

	BDROM_StreamDestType GetDestinationType(BDROM_StreamType streamType, bool bIsText = false);
	void SetDestinationFormat(BDROM_StreamDestType destType, BDROM_StreamType streamType, UInt32 uiPID, BDROM::stream_attributes * pStreamAttr);
	void SetVideoDestinationFormat(BDROM_StreamDestType destType, BDROM_StreamCodingTypes codingType,
								   BDROM_VideoFrameRate frameRate, BDROM_VideoAspectRatio aspectRatio);
	void SetAudioDestinationFormat(BDROM_StreamDestType destType, BDROM_StreamCodingTypes codingType,
								   BDROM_AudioPresentationType presentationType, BDROM_AudioSamplingFreq samplingFreq, UInt32 uiPID);
	void SetGraphicsDestinationFormat(BDROM_StreamDestType destType, BDROM_StreamCodingTypes codingType);
	void ConfigureLPCMDownMixCoeffs(BDROM_StreamSourceType sourceType, UInt16 pid);

	// clip playback setup
	SonicError PrepareClipForPlayback(void);
	SonicError DetermineEPMapSearchPID(void);
	SonicError DetermineEPMapForClip(void);

	// playback and stream control
	void       Flush(void);

	// EP_map related functions: SPN->map entry, SP Segment Num calc, determining seamless angle change point, etc.
	SonicError SPNtoEPEntries(UInt32 uiSPN, UInt32 & uiEPcoarseNum, UInt32 & uiEPfineNum);
	SonicError UpdateEPEntries(void);
	SonicError GetNextAngleChangePointEPMapEntries(UInt32 & uiEPcoarseNum, UInt32 & uiEPfineNum);
	SonicError GetNextAngleChangePointLocation(UInt32 & uiAngleChangePointSPN, Time90K & pts90kAngleChangePointPTS);
	SonicError GetAngleChangePointEPMapEntries(Time90K ts90kPTS, UInt32 & uiEPcoarseNum, UInt32 & uiEPfineNum);
	UInt32     GetSPSegmentId(UInt32 uiEPFineEntryNum);
	SonicError GetSPSegmentEndSPN(UInt32 uiSPSegmentId, UInt32 & uiSPSegmentEndSPN);
	SonicError PTS_to_SPN(Time90K pts90kTime, bool bAlignToLowerEntry, UInt32 & spn);

	// angle change
	SonicError ChangeAngle(UInt32 uiAngleNumber);
	SonicError ChangeAngle(BDROM_SonicPBCE_ClipAngleChangeInfo & angleChangeInfo);

	// scanning
	bool       IsScanRate(BDROM_PlayDirection direction, double dSpeed);
	SonicError ChangeRate(void);				// initiates a scan operation or cancels a scan operation (i.e, reverts to play). Should be called from read()
	SonicError JumpToNextOrPrevIFrame(void);	// Jumps to next/prev I-frame based on curr direction. Should be called from read()
	SonicError LocateNextIFrame(UInt32 uiNextFrameOffset = 1);		// locates next I-frame. Should be called from JumpToNextOrPrevIFrame()
	SonicError LocatePrevIFrame(UInt32 uiPrevFrameOffset = 1);		// locates prev I-frame. Should be called from JumpToNextOrPrevIFrame()

	//REALTEK_MODIFY
	SonicError GotoNextScanSegment();
	SonicError LocateIFrame(UInt64 targetPTS, UInt32& EPCoarseEntryNum, UInt32& EPFineEntryNum, UInt32& IFrameStartSPN, UInt32& IFrameEndSPN, long long& IFramePTS);

	// sync with read
	void SyncWithRead_Begin(void);
	void SyncWithRead_End(void);

public:
	//-----------------------------------------------------------------------------------
	// name rt_Stream Overrides
	//

	virtual SonicError get_err(void) const;
	virtual SonicError read(char *data, size_t nbytes, size_t *bytes_read);

protected:
	static const int IEndPositionSPNOffsets[8];	///< upper limits of I_tp_size for a given I_end_position_offset expressed as SPN

	BDROM_StreamSourceType					m_StreamSourceType;			///< type of BDROM_StreamSource that this clip is
	ref_ptr<BDROM_StreamControl>			m_pStreamControl;			///< Pointer to Stream Controller
	BDROM_SonicPBCE_EventSink *				m_pEventSink;				///< Event sink for this clip
	ref_ptr<BDROM_SonicPBCE_Registers>		m_pRegisters;				///< Ptr to PSRs
	BDROM_DiscType							m_discType;					///< Disc Type: BDROM or AVCHD

	ref_ptr<BDROM::BDROM_NavigationCache>	m_pNavCache;
	ref_ptr<BDROM::ClipInfo_clpi>			m_pClipInfo_clpi;
	ref_ptr<BD_FS_rt_stream>				m_pAVStream;				///< Main-Path Clip file
	ref_ptr<TSReader>						m_pTSReader;				///< TSReader object used to read data from clip
	UInt32									m_uiSPNsInFile;				///< File size in SPNs
	ref_ptr<BDROM::HDMV_LPCM_down_mix_coefficient>	m_LPCMDownMixCoeffData;		///< HDMV_LPCM_dowm_mix_coefficient in Clip Extension Data
	UInt8                                   m_Coeffs[18];

	BDROM::stream_PID_entry *				m_pStreamPIDEntry;			///< pointer to table PIDs in the EP_map for the current Program Sequence. 
	BDROM::EP_map_for_one_stream *			m_pEPMapForOneStream;		///< pointer to table of EP entries in the EP_map for the current stream.
	UInt32									m_uiEPmapSearchPID;			///< PID used to determine which EP_map_for_one_stream to use
	UInt32									m_EPStreamType;				///< EP_map stream type
	UInt32									m_STCid;					///< STC_id used to determine which EP_map_for_one_stream to use
	UInt32									m_uiATCSeqIdx;				///< Index (1 based) of ATC Sequence being used.
	UInt32									m_uiSTCSeqIdx;				///< Index (1 based) of STC Sequence being used.

	UInt32									m_uiEPCoarseEntryNum;		///< 1 based EP_coarse index. Corresponds to current read location. 0 if read loc < SPN_F_EP
	UInt32									m_uiEPFineEntryNum;			///< 1 based EP_fine index. Corresponds to current read location. 0 if read loc < SPN_F_EP

	// Data Members for Segment-Keyed Discs
	UInt32									m_uiSPSegmentId;			///< SP Segment ID (0 ... n) of current read location.
	UInt32									m_uiNumSPSegments;			///< Number of SP Segments in Clip.
	UInt32									m_uiSPSegmentEndSPN;		///< last SPN of current SP Segment (the SP segment number in m_uiSPSegmentId)

	ref_ptr<SonicCriticalSection>			m_pcsRead;					///< Serializes Nav file opening/reading and AV File reading functions.

	UInt32									m_PrerollBytesRead;			///< number of bytes read in pre-roll mode
	UInt32									m_PrerollSize;				///< max number of bytes to be read in pre-roll mode
	PBCE_ClipState							m_State;					///< Clip streaming State
	PBCE_ClipState							m_StateBeforeReadingPaused;	///< state backed up in this when reading is paused
	SonicError								m_LastError;				///< error code when stream is in Error state
	bool									m_bEndOfStream;				///< true if at EOS (i.e. state is EndOfSegment or has gone from EndOfSegment to Stop)

	BDROM_SonicPBCE_ClipPath				m_MainPathStream;
	BDROM_SonicPBCE_ClipPath				m_SubPathStream;
	bool									m_bSeamlessStart;			///< true if clip start is seamless (i.e., prev-->curr clip/angle transition seamless)
	bool									m_bSeamlessStop;			///< true if clip end is seamless (i.e., curr-->next clip transition seamless)
	SInt64									m_ts45kStartOffsetPTS;		///< PTS (45kHz) to be added to stream PTS to (a) make its start seamless, or (b) sync subpath to main path (e.g. at Async PiP start)

	UInt32									m_uiSPNStart;				///< Start SPN of the Clip (lowest of Start SPNs of all attached Paths). May not be AU Aligned.
	UInt32									m_uiSPNEnd;					///< End SPN of the Clip (highest of End SPNs of all attached Paths). May not be AU Aligned.
	UInt32									m_uiSPNCurrent;				///< Current SPN (i.e., current read location). AU Aligned.
	Time90K									m_pts90kStart;				///< PTS of start location (set when a 'new location' is set)
	Time90K									m_ts90kElapsedStart;		///< Elapsed time of the start location (as per main-path, or sub-path if no main-path is attcahed)

	bool									m_bPreLoad;					///< Indicates if clip is meant to be preloaded
	bool									m_bMainPathClip;			///< Indicates if clip is part of the MainPath

	ClipHDMVStreamExtent					m_IgStreamExtent;
	ClipHDMVStreamExtent					m_PgStreamExtent;
	ClipHDMVStreamExtent					m_TxtStreamExtent;

	// angle change
	bool									m_bAngleChangePending;		///< An angle change is pending. This is set to true only for Seamless angle changes
	BDROM_SonicPBCE_ClipAngleChangeInfo		m_AngleChangeInfo;			///< Information needed to change angle
	UInt32									m_uiSPNNextAngleChangePt;	///< SPN of next angle change point in current clip. Exit_SPN = m_uiSPNNextAngleChangePt - 1.
	UInt32									m_CurrentAngle;				///< Current Angle number

	// scanning
	bool									m_bRateChangePending;		///< true if a rate change needs to be done
	BDROM_PlayDirection						m_PendingDirection;			///< pending scan operation's direction of playback/scan
	double									m_dPendingSpeed;			///< pending scan operation's Playback/Scan speed
	bool									m_bScanning;				///< true if we are scanning
	BDROM_PlayDirection						m_Direction;				///< Direction of playback/scan
	double									m_dSpeed;					///< Current Playback/Scan speed
	bool									m_bAtIFrameStart;			///< true if we are about to read a new I-frame
	UInt32									m_uiIFrameEndSPN;			///< Last possible SPN of current I frame (only meaningful during scan)
	Time90K									m_ts90kScanPos;				///< Running time position of seeking, used to manage seeks instead of using start time of each coarse/fine pair as a starting point
	Time90K									m_ts90kToSkip;
	
	// members used to synch non-thread safe HDR calls like configure output with a read
	static volatile bool					m_bSyncWithReadRequired;	// if true, tells read() that it needs to signal m_semReadStart and wait on it to be signalled back
	static SonicPBCE_Event				*	m_pReadSyncStartEvent;		// signal used to begin sync with read

	// AACS/BD+ sample metadata properties
	SInt32									m_PlayitemId;				// playitem id. defaults to -1. Used *ONLY* for setting media sample properties.
	SInt32									m_ClipId;					// clip id. defaults to -1. Used *ONLY* for setting media sample properties.

	// special AVCHD stuff
	bool									m_bIsMenuStream;			// true if this is clip for a AVCHD menu

	//REALTEK_MODIFY_USE_DIRECT_IO
	unsigned char*							m_pTsBuffer;
};


//____________________________________________________________________________________________________________________________
// Clip Manager
//

#define MAX_PBCE_CLIPS					4

//--------------------------------------------------------------------------
// PBCE_ClipAssocEntry
// Entry to associate 
//
struct PBCE_ClipAssocEntry
{
public:
	std::string							m_strClipFilename;
	BDROM_SonicPBCE_Clip *				m_pClip;
	ref_ptr<rt_stream>					m_pClipRefPtr;
	SInt32								m_refCount;

	PBCE_ClipAssocEntry() { Reset(); }

private:
	void Reset(void)
	{
		m_strClipFilename.assign("zzzzz", 5);
		m_refCount = 0;
	}
};


//--------------------------------------------------------------------------
// BDROM_SonicPBCE_ClipManager
// Manages (Loads/Unloads) clips
//
class BDROM_SonicPBCE_ClipManager
{
public:
	BDROM_SonicPBCE_ClipManager(ref_ptr<SonicCriticalSection> pcsRead, ref_ptr<BDROM_StreamControl> pStreamControl,
								ref_ptr<BDROM_SonicPBCE_Registers> pRegisters, BDROM_SonicPBCE_EventSink * pEventSink,
								BDROM_DiscType discType, 
						#ifdef REALTEK_MODIFY	 
								void*					pSettings);
						#else		
								ref_ptr<BDROM_Settings> pSettings);
						#endif		
	~BDROM_SonicPBCE_ClipManager();

	// Init, Re-init
	SonicError Init(ref_ptr<BDROM::BDROM_NavigationCache> pNavCache);
	void Reset(void);

	SonicError LoadClip(PBCE_PathType pathType, std::string strClipFilename, BDROM_SonicPBCE_Clip* & pClip);
	SonicError UnloadClip(BDROM_SonicPBCE_Clip * pClip);

	ref_ptr<rt_stream> GetClipAsStreamSource(PBCE_PathType pathType);
	BDROM_SonicPBCE_Clip * GetClip(PBCE_PathType pathType);

	std::string GetClipName(PBCE_PathType pathType);

protected:
	PBCE_ClipAssocEntry					m_ClipTable[MAX_PBCE_CLIPS];

	SonicPBCE_Event					*	m_pReadSyncStartEvent;		// signal used to begin sync with read
};

void GetVideoFrameSize(BDROM_VideoFormat videoFormat, UInt32 & width, UInt32 & height);

#endif
