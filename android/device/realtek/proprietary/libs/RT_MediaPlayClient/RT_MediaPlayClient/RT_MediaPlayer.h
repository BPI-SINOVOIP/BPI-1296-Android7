#ifndef __MEDIAPLAYCLIENT_RT_MEDIAPLAYER_H
#define __MEDIAPLAYCLIENT_RT_MEDIAPLAYER_H

#ifndef NO_THREAD
#include <semaphore.h>
#endif

#include <RT_MediaPlayClient/UserEventQueue.h>
#include <RT_IPC/RT_IPC.h>
#include <utils/Errors.h>

namespace android {

typedef enum 
{
	NORMAL_FLOW = 1,
	AUDIO_ONLY_FLOW,
	VIDEO_ONLY_FLOW,
		
} PLAYBACK_FLOW_TYPE;
	
typedef struct 
{
	int numStreams;
	int type;
	int aspectRatioX;
	int aspectRatioY;
	int frameSizeWidth;
	int frameSizeHeight;
	int frameRate;
	int bitRate;
	
} VIDEO_STREAM_INFO;

typedef struct 
{
	int numStreams;
	int currentStreamNum;
	int type;
	int numChannels;
	int bitsPerSample;
	int samplingRate;
	
} AUDIO_STREAM_INFO;

typedef struct 
{
	int numStreams;
	int currentStreamNum;
	unsigned long languageCode;
	
} SPU_STREAM_INFO;

class RT_MediaPlayer 
{
public:
	/**********************************************************
	* constructure
	* @params -
	* z 		: Server side provides 2 display layers, 1 and 2. 
	*			  specify to what layer you want to display.
	* flowType	: specify what kind of flow you want to build, 
	*			  video only, audio only or a general flow.
	* flowName	: If you have already create a playback flow (by RTMediaConfig::getPBInstance)
	*			  and want to reuse it, set that flow's name.
	* bsSize	: bitstream buffer size
	* cmdSize	: command buffer size
	* Note - if flowName is set, flowType, bsSize and cmdSize should be the same as those 
	*		 when your create the reusable flow.
	***********************************************************/
                        RT_MediaPlayer(int z = 1, PLAYBACK_FLOW_TYPE flowType = NORMAL_FLOW, char* flowName = 0, long bsSize =RTCLIENT_BITSTREAM_BUF_SIZE, long cmdSize =RTCLIENT_COMMAND_BUF_SIZE);
    virtual             ~RT_MediaPlayer();
    
	/***********************************************************
	* initCheck - setup a playback connection to Server.
	***********************************************************/
    virtual status_t    initCheck();
    
    /***********************************************************
	* setDataSource - send media file url.
	* @params - 
	* url [in] : media file url
	* @ret - NO_ERROR if media is loaded successfully and BAD_VALUE if fail.
	***********************************************************/
    virtual status_t    setDataSource(const char *url);
    
    /***********************************************************
	* start - start/resume playback flow .
	*
	***********************************************************/
    virtual status_t    start();
    
    /***********************************************************
	* stop - stop playback flow .
	* @params -
	* bVideoDisappear [in] : set 1 to clear screen when stop flow.
	***********************************************************/
    virtual status_t    stop(int bVideoDisappear = 0);
    
    /***********************************************************
	* pause - pause playback flow .
	* @params -
	* bVideoDisappear [in] : set 1 to clear screen when pause flow.
	***********************************************************/
    virtual status_t    pause(int bVideoDisappear = 0);
    
    /***********************************************************
	* seekTo - seek to a new time position.
	* @params -
	* sec [in] : time position (in seconds)
	***********************************************************/
    virtual status_t    seekTo(int sec);
    
    /***********************************************************
	* getCurrentPosition - get current playback status.
	* @params -
	* sec [out] : current playback time position (in seconds)
	* pCurrentPTS [out] : current playback postion in PTS (90k)
	***********************************************************/
    virtual status_t    getCurrentPosition(int& sec, long long* pCurrentPTS = 0);
    
    /***********************************************************
	* getDuration - get total time.
	* @params -
	* sec [out] : total time (in seconds)
	***********************************************************/
    virtual status_t    getDuration(int& sec);
    
    /***********************************************************
	* reset - unload media.
	***********************************************************/
    virtual status_t    reset();
    
    /***********************************************************
	* setLooping - set repeat or not.
	* @params -
	* loop [in] : 1 means repeat playing and 0 to cancel repeat 
	***********************************************************/
    virtual status_t    setLooping(int loop);
    
    /***********************************************************
	* changeRate - change playback flow speed.
	* @params -
	* rate [in] : Forward 1x is 256 and backward 1x is -256.
	 			  flow rate can be derived from speed by rate = 256 * speed.
	 			  For example, 
	 			  fast forward : speed = 2x, rate = 256 * 2 = 512.
	 			  slow forward : speed = 3/4, rate = 256 * 3/4 = 192.
	 			  fast rewind  : speed = -1.5x, rate = 256 * -1.5 = -384.
	 			  slow rewind  : speed = -0.5x, rate = 256 * -0.5 = -128.
	 	
	***********************************************************/
	virtual status_t    changeRate(int rate);
    
    /***********************************************************
	* setVolume - change volume.
	* @params -
	* volume [in] : from 0 to 31. 0 is max while 31 is min
	***********************************************************/
	virtual status_t    setVolume(int volume);
	
	/***********************************************************
	* getVolume - get current volume.
	* @params -
	* volume [out] : from 0 to 31. 
	***********************************************************/
	virtual status_t    getVolume(long& volume);
	
	/***********************************************************
	* setMute - mute/unmute audio.
	* @params -
	* bMute [in] : 1 to mute while 0 to un-mute audio. 
	***********************************************************/
	virtual status_t    setMute(int bMute);
	
	/***********************************************************
	* getMute - get current audio mute state.
	* @params -
	* bMute [out] : 1 means mute while 0 means un-mute
	***********************************************************/
	virtual status_t    getMute(int& bMute);
	
	/***********************************************************
	* getBufFullness - get current fullness in each buffer.
	* @params -
	* videoFullness [out] : fullnessof video buffer (in bytes)
	* audioFullness [out] : fullness of audio buffer (in bytes)
	* pIOFullness [out] : fullness of io input buffer (in bytes)
	***********************************************************/
	virtual status_t    getBufFullness(long& videoFullness, long& audioFullness, long* pIOFullness = 0);
	
	/***********************************************************
	* getContiguousMem - get a contiguous memory.
	* @params -
	* size [in] : size of the request memory
	* phyAddr [out] : physical address of the memory. 0 means out of memory
	* pVirAddr [out] : virtual address of the memory
	* bDataBuffer [in] : 1 means this memory is used to transport bitstream.
	*                    the beginning of the memory will be configured as SOCKET_SHARED_MEM
	***********************************************************/
	virtual status_t    getContiguousMem(int size, unsigned long& phyAddr, unsigned char*& pVirAddr, bool bDataBuffer);
	
	/***********************************************************
	* getContiguousMem - free contiguous memory.
	* @params -
	* phyAddr [in] : physical address of the memory.  
	***********************************************************/
	virtual status_t    freeContiguousMem(unsigned long phyAddr);
	
	
	/***********************************************************
	* getHWSecret - get hardware secret key.
	* @params -
	* pSecret [out] : 32-bytes secret key
	***********************************************************/
	virtual status_t	getHWSecret(unsigned char* pSecret);
	
	/***********************************************************
	* dropData - This is used in the case if A/V data is delivered 
	*			 from client side(by unix socket or share memory) 
	*			 and want to flush the data in the buffer.
	* @params -
	* dataType [in] : what kind of data you want to flush.
	*					0 : all data, 1 : video data, 2 : audio data, 3 : spic data
	* stamp [in] : like a serial number and it should be the same as
	*              that in SOCKET_INFO_FLUSH you deliver in data buffer later.
	***********************************************************/
	virtual status_t 	dropData(int dataType, int stamp, int bResetKey = 0);
	
	/***********************************************************
	* registerData - register feedback data id. If succeed, server side 
	*				 will periodically feedback data associated with id 
	*				 to the shared memory specified by parameter phyAddr.
	* @params -
	* id [in] : data id
	*			1 - playback status
	*				data structure {
	 					long      elapsedTime; //in sec
	 					long long elapsedPTS;  //in 90k
	 				}
	*
	* phyAddr [in] : physical address of this memory
	* memSize [in] : shared memory size
	***********************************************************/
	virtual status_t	registerData(long id, unsigned long phyAddr, long memSize);
	
	/***********************************************************
	* setProperty - set client side property
	* @params -
	* enable [in] : 1 means enable while 0 means disable.
	* property [in] : bit mask of client property. If bit is on, 
	* 		          it means the associate property is enabled or disabled.
	*				  PROP_FEEDBACK_ERROR             = 1 << 0, Server side will feedback error code and won't stop playback flow
	*				  PROP_EASE_REBUF_REQUIREMENT     = 1 << 1,		      
	***********************************************************/
	virtual status_t 	setProperty(int enable, int property);

	/***********************************************************
	* getVideoStreamInfo - get vide stream info.
	* @params -
	* pInfo [out] : vide stream info
	***********************************************************/
	virtual status_t    getVideoStreamInfo(VIDEO_STREAM_INFO* pInfo);
	
	/***********************************************************
	* getAudioStreamInfo - get audio stream info.
	* @params -
	* pInfo [out] : audio stream info
	***********************************************************/
	virtual status_t    getAudioStreamInfo(AUDIO_STREAM_INFO* pInfo);
	
	/***********************************************************
	* getSpuStreamInfo - get spu stream info.
	* @params -
	* pInfo [out] : spu stream info
	***********************************************************/
	virtual status_t    getSpuStreamInfo(SPU_STREAM_INFO* pInfo);
	
	/***********************************************************
	* setSpuStream - change spu stream .
	* @params -
	* streamNum [in] : spu stream number
	* bShow [in] : 1 means enable while 0 means disable
	* textEncoding [in] : set character encoding. About encoding map, read TEXT_ENCODING_TABLE in doc.txt.
	* textColor [in] : set color of character. About color map, read TEXT_COLOR_TABLE in doc.txt.
	* fondSize [in] : set size of character. min is 12 while max is 76.
	***********************************************************/
	virtual status_t 	setSpuStream(long streamNum, long bShow, long textEncoding=1, long textColor= 0, long fontSize = 20);
		
public:
	/***********************************************************
	* GetEvent - get the events which notifyed from Server side. 
	*            some event may have event data like CMD_FATAL_ERROR.
	* @params -
	* eventId [out] : please see RT_CMD_ID
	* pEventDataSize [out] : event data size
	* pEventData [out] : event data
	* inDataSize [in] : size of pEventData
	***********************************************************/
	inline status_t GetEvent(int& eventId, int* pEventDataSize = 0, char* pEventData = 0, int inDataSize = 0) 
	{
		#ifdef NO_THREAD
			xProcEntry();
		#endif
		return m_EventQueue.DeQueue(eventId, pEventDataSize, pEventData, inDataSize) ? NO_ERROR : TIMED_OUT;
	}
	
	/***********************************************************
	* flushEventQueue - flush event queue
	***********************************************************/
	inline status_t	flushEventQueue()
	{
		m_EventQueue.FlushQueue();
		return NO_ERROR;
	}
		
private:
	inline status_t xSendCmd(RT_CMD_ID cmdId, char* pInData, int inDataSize, bool bBlocking)
	{	
		int seqNum;
		int retVal = m_ipc.sendCommand(cmdId, pInData, inDataSize, seqNum);
		if(retVal <= 0)
			return UNKNOWN_ERROR;
		
		#ifdef NO_THREAD
			printf("[%s %d] seqNum = %d\n", __FILE__, __LINE__, seqNum);
			xProcEntry(seqNum);	
		#else
			if(bBlocking)		
				sem_wait(&m_semToWaitResponse);
		#endif
	
		return NO_ERROR;
	}
	
	
	void xProcEntry(int checkedSeqNum  = -1);
    
    static void* 	procEntry(void* pInstance);
    
    
private:
    volatile enum { UNKNOWN = -1,
    				FAIL = 0,
    				SUCCESS = 1
    			} m_load;
    			
    bool 			m_bPlaying;
    int				m_duration;
    
#ifndef NO_THREAD 
    sem_t			 m_semToWaitResponse;
    sem_t			 m_semToUpdateResult;
    pthread_t		 m_thread;
    pthread_mutex_t	 m_cmdLock;
#endif
    volatile bool	m_bAskToStopThread;
    
    RT_IPC			m_ipc;
    char* 			m_result;
	int				m_resultSize;
	
	EventQueue		m_EventQueue;
	int				m_z;
	PLAYBACK_FLOW_TYPE m_flowType;
	char*			m_flowName;
	long			m_bsSize;
	long			m_cmdSize;
};

}; //namespace android

#endif // __MEDIAPLAYCLIENT_RT_MEDIAPLAYER_H
