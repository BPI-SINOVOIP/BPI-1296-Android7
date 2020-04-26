#ifndef __C_VCD_INPUT_PLUGIN_H__
#define __C_VCD_INPUT_PLUGIN_H__


#include <Filters/PrivateInfo.h>
#include <Filters/NavigationFilter/NavDef.h>
#include <Filters/NavigationFilter/NavPlugins.h>
#include <InbandAPI.h>
#include <Util.h>

#include "CVCDCore.h"
#include "CLowLevelTrans.h"

////////////////////////////////////////////////////////////////////////////////

#define VCD_DEFAULT_TURN_ON_PBC	1

#define VCD_RECOVERY_JUMP_STEP	32
#define CDDA_RIFF_HEADER_SIZE	44
#define CDDA_SECTOR_SIZE		2352

#define VCD_MIN_SCAN_SPEED		(30*256)
#define VCD_NORMAL_SPEED		256
#define VCD_INITIAL_PTS_FOR_I_ONLY_FORWARD	1000000
#define VCD_INITIAL_PTS_FOR_I_ONLY_REVERSE	100000000000000LL
#define VCD_PTS_INCREMENT_FOR_I_ONLY	90000

#define VCD_IN_STILL  				65535


////////////////////////////////////////////////////////////////////////////////

#define VCD_COMMAND(x)	{ printf x; }
#define VCD_NAV(x) 		//{ printf x; }
#define VCD_DEBUG(x) 	{ printf x; }
#define VCD_WARNING(x) { printf x; }
#define VCD_INFO(x)    { printf x; }
#define VCD_LOG(x)     { printf x; }


////////////////////////////////////////////////////////////////////////////////

#define VCD_SECTORS_PER_SEC		75	/* 75 sectors per second in VCD */
#define SVCD_SECTORS_PER_SEC	150	/* 150 sectors per second in SVCD */


////////////////////////////////////////////////////////////////////////////////

#define IS_PBC_ON(_this)  				((_this)->m_curLID != VCD_INVALID_ENTRY)
#define IS_AUDIO_CD(_this)				((_this)->m_vcdType == MEDIATYPE_CDDA)
#define IS_PLAYING_CDDA_TRACK(_this)	((_this)->m_curTrack >= (_this)->m_firstCDDATrack)
#define IS_CDDA_TRACK(track)			(track >= m_firstCDDATrack)
#define IS_TRICK_PLAY(_this)			((_this)->m_speed != VCD_NORMAL_SPEED)

class CInputVCD
{
	enum {
		/* we will send command by this order */
		CMD_FLUSH							=	(1L << 0),
		CMD_MEDIA_TYPE_CHANGE				=	(1L << 1),
		CMD_VIDEO_TARGET					= 	(1L << 2),
		CMD_AUDIO_TARGET					=	(1L << 3),
		CMD_SPU_TARGET						=	(1L << 4),
		CMD_NEW_SEGMENT						=	(1L << 5),
		CMD_VIDEO_VATR						=	(1L << 6),
		CMD_VIDEO_MIXER_RPTS				=	(1L << 7),
		CMD_VIDEO_BLEND						=	(1L << 8),
		CMD_VIDEO_DECODE					=	(1L << 9),
		CMD_VIDEO_ERROR_CONCEAL_LEVEL		=	(1L << 10),
		CMD_AUDIO_DATA_DISCONTINUITY		=	(1L << 11),
		CMD_VIDEO_END_OF_SEQUENCE			=	(1L << 12),
		CMD_VIDEO_VOBU_INFO					=	(1L << 13),
		CMD_END_OF_SEGMENT					=	(1L << 14),
		CMD_RESET_SPEED						=	(1L << 15),
		CMD_AUDIO_FORMAT					=	(1L << 16),
		CMD_NEW_AVSYNC_MODE					=	(1L << 17),
		CMD_RESET_REPEAT_MODE				=	(1L << 18),
		CMD_FATAL_ERROR						=	(1L << 19),
		CMD_EOS								=	(1L << 20),
	};
	
	enum {
		STATUS_VCD_OPENED					=	(1L << 0),	// if initialized 
		STATUS_EOS_SENT						=	(1L << 1),	// if end of segment is sent
  		STATUS_SEQ_END_SENT					=	(1L << 2),	//if end of sequence end is sent
  		STATUS_AT_PAUSE_STATE				=	(1L << 3),	// True if at pause state
  		STATUS_AT_STOP_STATE				=	(1L << 4),	// True if at stop state
  		STATUS_DATA_ERROR_MARKED			=	(1L << 5),	
  		STATUS_SHOULD_RESET_FILE_POINTER	=	(1L << 6),
  		STATUS_VCD_IS_SLIDE_SHOW			=	(1L << 7),
  		STATUS_WAIT_FOR_AP_COMMAND			=	(1L << 8),
	};
	
	typedef enum {
		READ_BLOCK							= (1 << 0),
		READ_STILL_FRAME					= (1 << 1),
		READ_ERROR							= (1 << 2),
		READ_END							= (1 << 3),
		READ_WAIT							= (1 << 4),
		READ_COMMAND						= (1 << 5),
		READ_NAV_ERROR						= (1 << 6),
	} VCD_READ_RESULT;

	struct VCD_NAV_INFO	
	{ 
  		unsigned long  	curLSN;
		int				curLID;
		unsigned char	curTrack;
		char			playListDescIdx;
	};

	struct VCD_PLAY_ITEM_INFO 
	{
		char 			trackNumInsideFileName;	/* track num inside filename */
		unsigned long	startLSN;				/* start LSN */
  		unsigned long	size;			      
	
	};	
	
	typedef enum
	{
		/*Note: VCDINFO_ITEM_TYPE_TRACK, VCDINFO_ITEM_TYPE_ENTRY and VCDINFO_ITEM_TYPE_SEGMENT
  	    	    should be 1, 2 and 3 correspondingly.
  				See UpdateCommands() in CVCDNav.cpp
		*/
		VCD_ITEM_TYPE_NOTFOUND = 0,
		VCD_ITEM_TYPE_TRACK = 1,
		VCD_ITEM_TYPE_SEGMENT = 2,
		VCD_ITEM_TYPE_ENTRY = 3,
		VCD_ITEM_TYPE_LID,
		VCD_ITEM_TYPE_SPAREID2,
    
	} VCD_ITEM_TYPE;
  
	struct VCD_ITEM
	{
		unsigned short num;
		VCD_ITEM_TYPE  type;
	};
	
	
	
protected:	

	ENUM_MEDIA_TYPE	m_vcdType;
	
	int				m_RIFFHeader;
	int				m_sectorSize;  
	long			m_commands;	
	long			m_status;	
	
	osal_mutex_t* 	m_pMutex;
	CVCDCore*		m_pVCDCore;	
	
	/*****************************************************/
	/**		 information used for playback status 		**/
	/*****************************************************/
	
	char			m_interval;	
	int				m_elapsedTime;
	unsigned long	m_context;			// bit 0 - bit 7 : track number or playListDescIdx depending on PBC mode
  										// bit 8 - bit 31 : cur_lsn

	long			m_errorCode;
	
	int64_t			m_lastDemuxPTS;
	int64_t			m_lastPresentedPTS;
	int64_t			m_tickSincePresented;
	int64_t			m_lastReferenceClock;

	NAV_PLAYBACK_MODE	m_playbackMode;
	/*****************************************************/ 
	/**		 used for inband commands 					**/
	/*****************************************************/

	int64_t			 	m_videoInitCmd; 
	VIDEODECODECOMMAND	m_videoDecCmd;	
	VIDEOMIXERBLEND		m_videoBlendCmd;
	ENUM_MEDIA_TYPE 	m_audioType;	
	DEMUXTARGET			m_videoTarget;	
	DEMUXTARGET			m_audioTartet;	
	DEMUXTARGET			m_spuTarget;	
	AUDIOFORMAT			m_audioFormat;	
	VIDEODVDVOBUINFO	m_videoVOBUInfo;

	int					m_concealmentLevel;	
	unsigned int		m_vatr;			/* 	b0 		  : Film camera mode 
  											b1 		  : reserved
  											b2 		  : Source picture letterboxed 
  											b3  - b5  : Source picture resolution 
  											b6 		  : line21_switch_2 
  											b7 		  : line21_switch_1
  											b8  - b9  : Display mode
  											b10 - b11 : Aspect ratio 
  											b12 - b13 : TV system
  											b14 - b15 : Video compression mode */
  

	

	/*****************************************************/
	/**		used for IO 								**/
 	/*****************************************************/ 

	// Note : VCD(SVCD) has 75(150) sectors per second, so if we define VCD_READ_UNIT as 15 sectors,
	//		we don't need to take care the case that when fast forward/backward, we have already 
	//		read out the sectors which should be skipped.
  
	#define			VCD_READ_UNIT	1
	
	unsigned char	m_dataChunk[VCD_READ_UNIT*CDDA_SECTOR_SIZE];	
	char			m_dataChunkIdx;		
	char 			m_blockNum;	


	int				m_vcdFileHd;	// file handle
  
	/*****************************************************/
	/**		used for navigation							**/
 	/*****************************************************/
 	
	int				m_curLID;      	/* LID that play item is in. Implies PBC is on. */
//  PsdListDescriptor pxd;          /* If PBC is on, the relevant PSD/PLD */
	char            m_playListDescIdx;	/* current play list desc index of pxd. -1 if no index*/

	VCD_ITEM		m_playItem;		/* play-item, VCDPLAYER_BAD_ENTRY if none */
	VCD_ITEM		m_loopItem;     /* Where do we loop back to? Meaningful only in a selection list */
	int				m_loopCount;    /* # of times play-item has been played. Meaningful only in a selection list.*/
	unsigned char	m_curTrack;     /* current track number. */
	
	char			m_bSlideShow;
	
	unsigned short	m_nextEntry;    /* where to go if next is pressed, VCDPLAYER_BAD_ENTRY if none */
	unsigned short	m_prevEntry;    /* where to fo if prev is pressed, VCDPLAYER_BAD_ENTRY if none */
	unsigned short	m_returnEntry;  /* Entry index to use if return is pressed */
	unsigned short	m_defaultEntry; /* Default selection entry. */

	unsigned long	m_curLSN;      	/* LSN of where we are right now */
	unsigned long	m_endLSN;   	/* LSN of end of current entry/segment/track. */
	unsigned long	m_originLSN;    /* LSN of start of slider position. */
	unsigned long	m_trackLSN;     /* LSN of start track origin of track we are in. */
	unsigned long	m_trackEndLSN; /* LSN of end of current track/segment . */
  
	VCD_NAV_INFO		m_currNavState;	/* used for resume playback */
	NAV_REPEAT_MODE		m_repeatMode;	/* only support NAV_REPEAT_TITLE, NAV_REPEAT_CHAPTER */

	int					m_stillTime;	
	
	enum {
			STILL_IN_INFINITE_LOOP 		= -2,	//in infinite loop
			STILL_BUT_WAIT_TIME_UNKNOWN = -5,	//maybe a still but haven't read wait time yet
			STILL_NONE					= 0,	//not in still, 
			
	} ;
	int					m_inStill;	
  								/*  0 : not in still, 
                                   -2 : in infinite loop	
                                   -5 : maybe a still but haven't read wait time yet
                                  > 0 : number of seconds to wait */
    
    /*****************************************************/
	/**			 information used to AB repeat			**/  
	/*****************************************************/
	                              
	unsigned long m_segStartLSN;
	unsigned long m_segEndLSN;
	unsigned long m_segStartPos;
	
	/*****************************************************/
	/**			 information used to FF/FR 				**/  
	/*****************************************************/
	
	int		m_speed;	
	int		m_amountOfSkip;
	int		m_readSectorCount;
	int		m_ptsStep;				//ths incremental step of RPTS in VOBU info


	/*****************************************************/
	/**		information of media						**/
 	/*****************************************************/ 
 	
	char			m_pbcCap;			/* non-zero if it can be played with PBC ON, zero if it only can be played with PBC OFF */
	
	char			m_numAVTracks;	
	char			m_numCDDATracks;
	unsigned short  m_numSegments;	
	//unsigned short	m_numLIDs;	
	//unsigned int	m_numEntries;	
 
  
	char	 		m_firstCDDATrack;
	char			m_avTrackFormat[VCD_MAX_PATH_LEN];	
	char			m_cddaTrackFormat[VCD_MAX_PATH_LEN];
	char			m_segmentFormat[VCD_MAX_PATH_LEN];	

  
	VCD_PLAY_ITEM_INFO*	m_pTrack; 	// num of track is m_numAVTracks + m_numCDDATracks;
	VCD_PLAY_ITEM_INFO*	m_pEntry; 	// num of entry is m_numEntries;
	

protected:
	//////////////////////////////////////////////////////////////
	// 					VCDIO									//
	//////////////////////////////////////////////////////////////
	void			GetSectorSize(int size);
	void			SetPlayItemInfo(int* pIndex, char num, struct dirent **namelist, char* pPath, const char* pFormat);
	unsigned char	InitTracks(char* pSourceName);
	unsigned char	initSegments(char* pSourceName);
	unsigned long	GetEntrySecNum(unsigned int entryNum);
	unsigned char	VCDOpen(char* pSourceName) ;
	void			VCDClose();
	
	

	//////////////////////////////////////////////////////////////
	// 					VCDNav									//
	//////////////////////////////////////////////////////////////
	HRESULT StartPlayback(unsigned char bTurnOnPBC);
	HRESULT Play(VCD_ITEM item, char playListDescIdx = 0);
	HRESULT	PlaySingleItem(VCD_ITEM item);
	HRESULT OpenFile();
	HRESULT SetPlayItem(VCD_ITEM item);
	HRESULT MovePlayItem(PSD_LIST_DESC* pPXD, int itemStep);
	void	UpdateNavButtonInfo();
	
	CInputVCD::VCD_READ_RESULT PBCOffUpdateNav();
	CInputVCD::VCD_READ_RESULT PBCOnUpdateNav();
	CInputVCD::VCD_READ_RESULT ReadNextLSNSector(unsigned char **pReadData, unsigned int* pReadSize) ;
	unsigned char	IdentifyCDDAFormat(unsigned char* buffer, int bufferSize);
	unsigned char 	ReadOneSector(unsigned char **pBuf);
	void 			UpdateEntry(unsigned short offset, unsigned short* pEntry, const char* pLabel);
	void 			SyncLSN(unsigned char bAlwaysSync);
	void 			UpdateCommands(VCD_ITEM item);
	void 			ClassifyItem(unsigned short itemNum, VCD_ITEM* pItem);
	int 			ReadSectorsToBuf(unsigned char **pBuf);
	int 			GetSegmentSize(unsigned short num);
	
	//////////////////////////////////////////////////////////////
	// 					InputVCD									//
	//////////////////////////////////////////////////////////////
	
	HRESULT GetPlaybackStatus(NAVPLAYBACKSTATUS* status);
	HRESULT GetVideoStatus(NAVVIDEOSTATUS* status);
	HRESULT GetAudioStatus(NAVAUDIOSTATUS* status);
	HRESULT GetDiscStatus(NAVDISCSTATUS* status);
	HRESULT GetTitleStatus(NAVTITLESTATUS* status);
	HRESULT GetNavState(void* state, unsigned int size, unsigned int* returnedSize);
	HRESULT SetNavState(void* state, unsigned int size);
	HRESULT Load(char* path, bool* pbContinue);
	HRESULT UpdatePosition(PRESENTATION_POSITIONS* pPosition, NAVDEMUXPTSINFO* pDemuxPTSInfo);
	HRESULT ExecUserCmd(NAV_CMD_ID id, void* cmdData, unsigned int cmdDataSize, unsigned int* pIsFlushRequired);
	HRESULT PlayNextPrevChapter(unsigned short playEntry, unsigned char bNextChapter);
	HRESULT UserCmdPlayItem(VCD_ITEM item);
	HRESULT SendCommand(NAVBUF* pBuffer);
	HRESULT Read(NAVBUF* pBuffer);
	HRESULT PlayAtTime(int chapterNum, unsigned int sec);
		
	void PackEvent(unsigned int eventId, void* eventData, unsigned int eventSize, NAVBUF* pBuffer);			
	void PackPrivateInfo(unsigned int infoId, void* infoData, unsigned int infoSize, NAVBUF* pBuffer, void* payloadData = NULL, unsigned int payloadSize = 0);
	int GetPresentationTime();
	
	// ============================== reset current nav state ===========================
	inline void SetCurrNavState()
	{														
  		m_currNavState.curLID 		  = m_curLID;			
    	m_currNavState.curLSN 		  = m_curLSN;			
    	m_currNavState.curTrack		  = m_curTrack;			
    	m_currNavState.playListDescIdx = m_playListDescIdx;	
    	m_elapsedTime				  = GetPresentationTime();
	}
	
	// ============================== get play time type ===========================
	inline const char* GetPlayItemType(int type)
	{
		static const char* array[6] = { "VCD_ITEM_TYPE_NOTFOUND",
    									"VCD_ITEM_TYPE_TRACK",
    									"VCD_ITEM_TYPE_SEGMENT",
    									"VCD_ITEM_TYPE_ENTRY",
    									"VCD_ITEM_TYPE_LID",
    									"VCD_ITEM_TYPE_SPAREID2",
									};
		//if(IS_PBC_ON(this))	return "VCDINFO_ITEM_TYPE_LID";
		
		return (type >= 0 && type < 6) ? array[type] : "VCD_ITEM_TYPE_NOTFOUND";
	}

	// ============================== check if reset speed ===========================
	inline void ResetSpeed()
	{														
		if(!IS_AUDIO_CD(this)) 		
		{													
			SET_FLAG(m_commands, CMD_RESET_SPEED);			
			SET_FLAG(m_commands, CMD_FLUSH);				
			RESET_FLAG(m_commands, CMD_VIDEO_VOBU_INFO);	
			m_speed	= VCD_NORMAL_SPEED;			
		}													
	}
	// ============================== reset repeat mode =======================
	inline void ResetRepeatChapter()
	{														
		if(m_repeatMode == NAV_REPEAT_CHAPTER)				
		{													
			m_repeatMode = NAV_REPEAT_NONE;					
			SET_FLAG(m_commands, CMD_RESET_REPEAT_MODE );	
		}													
	}
	
	inline void ResetRepeatMode()
	{														
		if(m_repeatMode != NAV_REPEAT_NONE)				
		{													
			m_repeatMode = NAV_REPEAT_NONE;					
			SET_FLAG(m_commands, CMD_RESET_REPEAT_MODE );	
		}													
	}
	// ============================== check if sequence end is sent ===========================

	inline unsigned char SendSequenceEnd()
	{
		if(!HAS_FLAG(m_status,	STATUS_SEQ_END_SENT))
		{															
			SET_FLAG(m_commands, CMD_VIDEO_END_OF_SEQUENCE );	
			SET_FLAG(m_status, STATUS_SEQ_END_SENT);		
			return 1;
		}
		return 0;
	}
			
public :
	CInputVCD(ENUM_MEDIA_TYPE majorType, osal_mutex_t* mutex) 
	{
		m_pMutex = mutex; 
		m_vcdType = majorType;
		m_vcdFileHd = -1;
	
		m_pVCDCore = 0;
    	m_pTrack = m_pEntry = NULL;
  		
	}
	
	~CInputVCD() {	VCDClose(); }
	
	// input plugin functions
    static HRESULT loadMedia(void* pInstance, char* path, bool* pbContinue, NAVLOADFAILURE* pFailure);
    static HRESULT unloadMedia(void* pInstance);
    static HRESULT getMediaInfo(void* pInstance, NAVMEDIAINFO* pMediaInfo);
    static HRESULT readBuf(void* pInstance, NAVBUF* pBuffer, unsigned int prohibitedChannelMask, NAV_STREAM_TYPE* channelFullnessInfo);
    static HRESULT updatePlaybackPosition(void* pInstance, PRESENTATION_POSITIONS* pPosition, NAVDEMUXPTSINFO* pDemuxPTSInfo);
    static HRESULT privateDataFeedback(void* pInstance, unsigned int id, unsigned char* data, unsigned int size, int64_t lastPTS);
    static HRESULT execUserCmd(void* pInstance, NAV_CMD_ID id, void* cmdData, unsigned int cmdDataSize, unsigned int* pIsFlushRequired);
    static HRESULT propertySetGet(void* pInstance, NAV_PROP_ID id, void* inData, unsigned int inDataSize, void* outData, unsigned int outDataSize, unsigned int* returnedSize);
    static HRESULT dispose(void* pInstance);	
    
    static int		CDDATrackFilter(const struct dirent* file);
    static int		AVTrackFilter(const struct dirent* file);
};

#endif //#define __C_VCD_INPUT_PLUGIN_H__
