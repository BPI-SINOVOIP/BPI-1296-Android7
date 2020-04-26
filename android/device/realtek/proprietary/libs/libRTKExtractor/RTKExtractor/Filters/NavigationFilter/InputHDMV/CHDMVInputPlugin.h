
#ifndef __C_HDMV_INPUT_PLUGIN_H__
#define __C_HDMV_INPUT_PLUGIN_H__

#include "BDROM_ApplicationEventSink.h"	
#include "SonicPBCE_Registers.h"
#include "BDROM_TimerManager.h"
#include "NotificationEventManager.h"
#include "BDROM_StreamControl.h"
#include "BDROM_SoundPlayer.h"
#include "SonicPBCE.h"
#include "BDROM_Navigator.h"
#include "BDROMNavigationCache.h"
#include "CERenderer.h"
#include "packetdef.h"
#include "BasePacketCallback.h"
#include "md_stream.h"
#include "BDROM_ElapsedTitleTimer.h"
#include "SonicPBCE_Clips.h"

#include <Filters/NavigationFilter/NavPlugins.h>
#include <Filters/NavigationFilter/SiEngine.h>
#include <OSAL.h>
#include <Filters/PrivateInfo.h>

#define IS_FLAG_ON(flag, bit)    ((flag) & (bit))
#define FLAG_ON(flag, bit)    ((flag)|= (bit))
#define FLAG_OFF(flag, bit)  ((flag)&= (~(bit)))



//__________________________________________________________________________
//
// Default settings
#define DEFAULT_PLAYER_REGION_CODE				BDROM_REGIONCODE_A					// Region A
#define DEFAULT_PLAYER_COUNTRY_CODE				0x5553								// 'US' Country Code
#define DEFAULT_PLAYER_PARENTAL_LEVEL			BDROM_PSR__PARENTAL_LEVEL__DEFAULT	// 0xFF (Least Restricted)
#define DEFAULT_PLAYER_PGTEXTST_DISPLAY			1									// PG/TextST On by default
#define DEFAULT_PLAYER_PARENTALLEVEL_CHANGE		0									// Player parental level cannot be changed
#define DEFAULT_PLAYER_LANGUAGECODE_CHANGE		0									// Player language code cannot be changed
#define DEFAULT_PLAYER_COUNTRYCODE_CHANGE		0									// Player country code cannot be changed
#define DEFAULT_PLAYER_AUDIO_LANGUAGE			"eng"								// English
#define DEFAULT_PLAYER_PGTEXTST_LANGUAGE		"eng"								// English
#define DEFAULT_PLAYER_MENU_LANGUAGE			"eng"								// English

//____________________________________________________
//	Resume info
//____________________________________________________
typedef struct {
	int titleNum;
	int chapterNum;
	int time;		//in sec
	int plNum;			//REALTEK_MODIFY_BUG_30406
	int audioStreamNum;	//REALTEK_MODIFY_BUG_28722	
	int spuStreamNum;	//REALTEK_MODIFY_BUG_28722	
	int domain;			//REALTEK_MODIFY_BUG_34448

} HDMV_NAV_STATE ;
	
//__________________________________________________________________________
//
class CInputHDMV : public BDROMAppEventSink
{
	enum { TARGET_VIDEO = 0,
           TARGET_AUDIO,
           TARGET_PG,
           TARGET_IG,
           TARGET_TEXTST,
           NUM_OF_TARGETS,
    };
    
    enum {
    	STATUS_AT_STOP_STATE 	= 1 << 0,	
    	STATUS_AT_PAUSE_STATE	= 1 << 1,
    	STATUS_USE_DIRECT_IO	= 1 << 2,
    	STATUS_WAIT_PRES_END	= 1 << 3,
    	STATUS_SYNC_WORD_MISS	= 1 << 4,
    };
    
    enum {
    	EVENT_FLUSH 		 			= 1 << 0,
		EVENT_FLUSH_AUDIO				= 1 << 1,
		EVENT_FLUSH_PG					= 1 << 2,
		EVENT_FLUSH_TEXTST				= 1 << 3,
    	EVENT_NEW_VIDEO_TYPE			= 1 << 4,
    	EVENT_NEW_DEMUX_TYPE			= 1 << 5,
    	EVENT_UPDATE_DEMUX_TARGET		= 1 << 6,
    	EVENT_UPDATE_PACKET_SIZE		= 1 << 7,
    	EVENT_NEW_SEGMENT				= 1 << 8,
    	EVENT_DECODE					= 1 << 9,
    	EVENT_VIDEO_BLEND_CMD			= 1 << 10,
    	EVENT_SPU_CONFIG				= 1 << 11,
    	EVENT_SEGMENT_END				= 1 << 12,
    	EVENT_RESET_SPEED				= 1 << 13,
    	EVENT_UPDATE_DEMUX_AUDIO_TARGET	= 1 << 14,
    	EVENT_VIDEO_VC1_SEQ_INFO		= 1 << 15,
		EVENT_EOS						= 1 << 16,
		EVENT_SPU_ON_OFF				= 1 << 17,
		EVENT_NAVIGATION_FAIL			= 1 << 18,
		EVENT_UNSUPPORT_FORMAT			= 1 << 19,
		EVENT_RESET_REPEAT_MODE			= 1 << 20,
		EVENT_UPDATE_AVSYNC_MODE		= 1 << 21,
		EVENT_VIDEO_USABILITY_INFO      = 1 << 22,
    };

	
private:
	//Load/Unload Media
	HRESULT Load(char* path, bool* pbContinue, NAVLOADFAILURE* pFailure);
    HRESULT LoadBDROMDirectory(char* path, bool* pbContinue);
    HRESULT CreateDisplayLayer(DisplayLayer*& pDisplayLayer, UInt32 nOrder);
	HRESULT DestroyDisplayLayer(DisplayLayer*& pDisplayLayer);
    HRESULT UnLoad();
    
    HRESULT GetMediaInfo(NAVMEDIAINFO* pMediaInfo);
    NAV_AVSYNC_MODE GetAVSyncMode();
    
    //read ts packets
    HRESULT Read(NAVBUF* pBuffer);
    HRESULT CheckMetaData(int srcStreamIndex, NAVBUF* pBuffer);
	bool HandleEvents(NAVBUF* pBuffer);
	

    HRESULT UpdatePlaybackPosition(PRESENTATION_POSITIONS* pPosition, NAVDEMUXPTSINFO* pDemuxPTSInfo);
    HRESULT PrivateDataFeedback(unsigned int id, unsigned char* data, unsigned int size, int64_t lastPTS);
    
    //User Operation
    HRESULT ExecUserCmd(NAV_CMD_ID id, void* cmdData, unsigned int cmdDataSize, unsigned int* pIsFlushRequired);
    HRESULT PlayTitle(int titleNum);
    HRESULT PlayChapter(int titleNum, int chapterNum);
    HRESULT PlayAtTime(int titleNum, int seconds);
    HRESULT ChangeSpuStream(int spuStreamIdx, NAV_DISPLAY_STATE displayStatus);
		
    //Set/Get Property
    HRESULT PropertySetGet(NAV_PROP_ID id, void* inData, unsigned int inDataSize, void* outData, unsigned int outDataSize, unsigned int* returnedSize);
    HRESULT GetAudioFormatFB(SI_CODEC_TYPE codecType, void* pOutData, int outDataSize);
	HRESULT GetAudioStatus(NAVAUDIOSTATUS* pAudioStatus);
	HRESULT GetSpuStatus(NAVSPICSTATUS* pSpuStatus);
	HRESULT GetVideoStatus(NAVVIDEOSTATUS* pVideoStatus);
	HRESULT GetNavState(HDMV_NAV_STATE* pState, unsigned int* pReturnSize);

	//For CERenderer Interfaces
	int RemovePID(int count, unsigned short* pidArray);   
	int InsertPID(int count, SI_FILTER_DESC* filterDescArray); 
	void FlushPin(SI_FILTER_TYPE type);
	void UpdatePBEndCallback(SI_FILTER_TYPE type, unsigned int pbEndCallback, int pbEndCBData, int pbEndCBOutputID);
	
	
	//Update Video/Audio/Spu/IG PID
	void UpdateTsDemuxTarget(NAVBUF* pBuffer);
	HRESULT DeliverPrivateInfoAccordingType(int target, SI_CODEC_TYPE codecType);
	HRESULT UpdateCurAVStream();
	int GetTargetIdxFromType(SI_FILTER_TYPE type);
	
	void OnFlush();
	
	inline void ResetRepeatMode()
	{														
		if(m_playbackStatus.repeatMode != NAV_REPEAT_NONE)				
		{
	#ifdef	REALTEK_MODIFY_REPEAT_TITLE  		
			ref_ptr<ModuleManager> pModuleManager = m_pBDROMNavigator->GetModuleManager();
        	pModuleManager->SetRepeatTitle(false);
    #endif    	
			m_playbackStatus.repeatMode = NAV_REPEAT_NONE;					
			FLAG_ON(m_events, EVENT_RESET_REPEAT_MODE );	
		}													
	}
		
	inline void PackPrivateInfo(unsigned int infoId, void* infoData, unsigned int infoSize, NAVBUF* pBuffer)
	{
    	pBuffer->type              = NAVBUF_DATA;
    	pBuffer->data.channelIndex = 0;
    	pBuffer->data.startAddress = 0xFFFFFFFF;
    	pBuffer->data.payloadSize  = 0;
    	pBuffer->data.infoId       = infoId;
    	pBuffer->data.infoData     = (unsigned char*)infoData;
    	pBuffer->data.infoSize     = infoSize;
	}
	
	inline void PackEvent(unsigned int eventId, void* eventData, unsigned int eventSize, NAVBUF* pBuffer)
	{
    	pBuffer->type            = NAVBUF_EVENT;
    	pBuffer->event.eventId   = eventId;
    	pBuffer->event.eventData = (unsigned char*)eventData;
    	pBuffer->event.eventSize = eventSize;
	}
	
	
	
public:
	CInputHDMV(ENUM_MEDIA_TYPE majorType, osal_mutex_t* mutex);
	~CInputHDMV();
		
	//_________________________________________________________________________
	// input plugin export functions
	//_________________________________________________________________________
	
    static HRESULT loadMedia(void* pInstance, char* path, bool* pbContinue, NAVLOADFAILURE* pFailure);
    static HRESULT unloadMedia(void* pInstance);
    static HRESULT getMediaInfo(void* pInstance, NAVMEDIAINFO* pMediaInfo);
    static HRESULT read(void* pInstance, NAVBUF* pBuffer, unsigned int prohibitedChannelMask, NAV_STREAM_TYPE* channelFullnessInfo);
    static HRESULT updatePlaybackPosition(void* pInstance, PRESENTATION_POSITIONS* pPosition, NAVDEMUXPTSINFO* pDemuxPTSInfo);
    static HRESULT privateDataFeedback(void* pInstance, unsigned int id, unsigned char* data, unsigned int size, int64_t lastPTS);
    static HRESULT execUserCmd(void* pInstance, NAV_CMD_ID id, void* cmdData, unsigned int cmdDataSize, unsigned int* pIsFlushRequired);
    static HRESULT propertySetGet(void* pInstance, NAV_PROP_ID id, void* inData, unsigned int inDataSize, void* outData, unsigned int outDataSize, unsigned int* returnedSize);
    static HRESULT dispose(void* pInstance);	
	//static HRESULT identify(void* pInstance, char* path, unsigned char* streamBuffer, unsigned int streamBufferBytes, unsigned int streamBufferOffset);
    //    static HRESULT registerIOPlugin(void* pInstance, IOPLUGIN* ioPlugin, bool b_opening, bool b_direct_io, void* fh);

 
   	//_________________________________________________________________________
	// functions exported to CERenderer
	//_________________________________________________________________________
	
	static int removePID(unsigned int instanceID, int count, unsigned short* pidArray);   
	static int insertPID(unsigned int instanceID, int count, SI_FILTER_DESC* filterDescArray); 
	static void flushPin(unsigned int instanceID, SI_FILTER_TYPE type);
	static void updatePBEndCallback(unsigned int instanceID, SI_FILTER_TYPE type, unsigned int pbEndCallback, int pbEndCBData, int pbEndCBOutputID);

	//_________________________________________________________________________
	// NotificationEventSink
	//_________________________________________________________________________
	
	void NotifyEvent(BDROM_Event event);
	void UpdateVideoStatus(int numStream, BDROM_Stream* pStreamAttr);
	void UpdateAudioStatus(int numStream, BDROM_Stream* pStreamAttr);
	void UpdateSpuStatus(int numStream, BDROM_Stream* pStreamAttr);

private:
	osal_mutex_t   				m_BDNavMutex;
    osal_mutex_t*  				m_pNavMutex;
    BDROM_DiscType				m_discType;
    BDROM_Region				m_playerRegion;
    unsigned int				m_playerParentalLevel;
    unsigned int				m_playerCountry;
    char						m_playerAudioLanguage[4];
    char						m_playerMenuLanguage[4];
    char						m_playerPGTextSTLanguage[4];
    bool						m_bPlayerPGTextSTOn;
    bool						m_bParentalLevelChangeAllowed;
    bool						m_bLanguageCodeChangeAllowed;
    bool						m_bCountryCodeChangeAllowed;
    BDROM_PlayerProfile			m_PlayerProfile;
    
    ref_ptr<BDROM_SonicPBCE_Registers>	m_pBDROMRegisters;
    ref_ptr<NotificationEventManager>	m_pEventManager;
    ref_ptr<BDROM_StreamControl>		m_pBDROMStreamControl;
    ref_ptr<BDROM_NavigationCache>		m_pBDROMNavCache;
    ref_ptr<BDROM_SoundPlayer>			m_pSoundPlayer;
    ref_ptr<SonicPlaybackControlEngine> m_pPlaybackEngine;
    ref_ptr<BDROM_Navigator>			m_pBDROMNavigator;
    ref_ptr<CERenderer>					m_pRenderer;
    ref_ptr<BD_FS_Layer>				m_pBDFSLayer;
    ref_ptr<BDROM_TimerManager>			m_pTimerManager;
    ref_ptr<BDROM_ElapsedTitleTimer>	m_elapsedTitleTimer;
    ref_ptr<UserEventManager> 			m_pUserEventManager;
    
    DisplayLayer*				m_pIGDisplayLayer;
	DisplayLayer*				m_pTextSTDisplayLayer;
	
	long						m_demuxErrorCount;
    long						m_status;
    long						m_events;
   
    
    int64_t						m_curPresentPTS;
    int							m_waitCount;
    
    //____________________________________________________
    //	For trick play
    //____________________________________________________
    long 						m_speed;
    VIDEODVDVOBUINFO			m_vobuInfo;
    
    //____________________________________________________
    //	AV stream and meta stream
    //____________________________________________________
    unsigned char*				m_pTsBuffer;
    
    struct {
    	rt_stream*	pAVStream;	// to get ts packet
    	md_stream*	pMDStream;	// to get meta data
    	int			syncPos;
    	bool 		bSyncWordFound;
    } m_SrcStream[MAX_PBCE_CLIPS];
    
    
    int m_nextReadingStreamIdx;
    
    //____________________________________________________
    //	Inband Command
    //____________________________________________________
    VIDEODECODECOMMAND			m_videoDecCmd;
    VIDEOMIXERBLEND				m_videoBlendCmd;
    SPUCONFIG					m_spuConfig;
    VIDEOVC1SEQINFO 			m_VC1SeqInfo;
    
    SI_CODEC_TYPE				m_curVideoType;	//to change video decoder type
    //ENUM_MEDIA_TYPE				m_mediaType;		//to change demux type
    int							m_spicOnOff;
    int							m_tsPacketSize;
    DEMUXTARGET_TS				m_demuxTarget;
    DEMUXTARGET_TS_ENTRY 		m_demuxTargetEntries[SI_MAX_FILTER_NUMBER];
    DEMUXTARGET          		m_demuxTargetTsAudio;	/*  id : codecType
    												subId : sourceType ( MEDIATYPE_FILE or MEDIATYPE_STB)
    												subId2 : audio preference (only for those audio streams carrying packets with diff types)
    											 */
    											 
    // active targets of audio/video/subtitle/teletext
    struct {
        int  				 pid;
        bool 				 bNewTarget;
        SI_CODEC_TYPE		 codecType;
        ENUM_MEDIA_TYPE		 audioPreference;	//be meaningful only for TARGET_AUDIO
        
        PID_PRIVATE_DATA	 privateData;		 
		
    } m_activeTarget[NUM_OF_TARGETS];

	static const SI_FILTER_TYPE TargetToFilterTypeArr[NUM_OF_TARGETS];
	
	unsigned long m_dmallocmark;
	
	//____________________________________________________
    //	PlaybackStatus, Video/Audio/SPU status
    //____________________________________________________
	NAVPLAYBACKSTATUS	m_playbackStatus;
	NAVVIDEOSTATUS		m_videoStatus;
	NAVAUDIOSTATUS		m_audioStatus;
	NAVSPICSTATUS		m_spuStatus;
	volatile bool 		m_bItemInfoAvailable;	//indicate if video/audio/spu status is available
	
	long				m_fatalErrCode;
	ENUM_MEDIA_TYPE		m_unsupportedFormat;
	
	int					m_playItemId;
	int64_t				m_playItemOutTime45K;
	
	NAVMENUCONFIG		m_menuConfig;
	NAV_AVSYNC_MODE		m_avSyncMode;
	VIDEOUSABILITYINFO  m_videoUsability;
	
	unsigned long		m_mediaSignature[4];
	
	bool				m_bWaitSwitchToMiniBD;
	int                 m_playListId;
	int                 m_igPacketCount;
};

#endif //__C_HDMV_INPUT_PLUGIN_H__
