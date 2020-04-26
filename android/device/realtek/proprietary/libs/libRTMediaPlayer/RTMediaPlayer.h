#ifndef ANDROID_RT_MEDIAPLAYER_H
#define ANDROID_RT_MEDIAPLAYER_H

#include <semaphore.h>
#include <pthread.h>
#include <utils/Vector.h>

#include <media/MediaPlayerInterface.h>

#include <RT_IPC/RT_IPC.h>

#include <media/stagefright/MediaErrors.h>

#include <transcode/RtkTranscodeCommon.h>

#include <Platform_Lib/SocketAVData/SocketAVData.h>


class RVSD;
class DataTransportServer;
//struct SOCKET_MEDIA_ATTRIBUTE;
#ifdef USE_RTK_VOUT_UTIL
class VoutUtil;
#endif

#define ENABLE_PREPARED_WITH_VIDEO_INFO
#define ENABLE_PREPARED_WITH_ALL_TRACK_INFO

namespace android
{

class IGraphicBufferProducer;
class RtkTranscodePlayer;
class RTMediaPlayerEventListener;

typedef enum {
    IDLE,
    PLAYING,
    PAUSE,
    LOADED,
    PREPARING,
    PLAYBACK_COMPLETE,
} PLAYER_STATE_e;

enum {
    ERROR_TRANSCODE_NOT_ENABLED     = MEDIA_ERROR_BASE - 2000,
    ERROR_TRANSCODE_NOT_CONFIGURED  = MEDIA_ERROR_BASE - 2001,
    ERROR_TRANSCODE_WRONG_STATE     = MEDIA_ERROR_BASE - 2002,
};

typedef enum {
    STREAM_DATA_TS = 0,
    STREAM_DATA_ES_VIDEO,
    STREAM_DATA_ES_AUDIO,
    STREAM_DATA_END
} STREAM_DATA_TYPE_e;

typedef enum {
    CONTENTMODE_NULL    = -1,
    CONTENTMODE_LETTERBOX,
    CONTENTMODE_FULL,
} RTK_PLAYER_CONTENTMODE_e;

// Keep NAVPROP_* in sync with MediaPlayer.java.
enum media_player_nav_property_ids {
    NAVPROP_SET_STEREOSCOPIC_OUTPUT_MODE = 1,
    NAVPROP_GET_PLAYBACK_STATUS = 2,
    NAVPROP_GET_MEDIA_TYPE = 3,
    NAVPROP_SET_STREAMING_IO_BUFFER_THRESHOLD = 4,
    NAVPROP_GET_BD_ISO_INFO = 5,
    NAVPROP_GET_BD_ISO_CHAPTER_INFO = 6,
    NAVPROP_MAX = 7
};
static const char* NaviPropIdNames[NAVPROP_MAX] = { 
    " ",// = 0
    "NAVPROP_SET_STEREOSCOPIC_OUTPUT_MODE",// = 1,
    "NAVPROP_GET_PLAYBACK_STATUS",// = 2,
    "NAVPROP_GET_MEDIA_TYPE",// = 3,
    "NAVPROP_SET_STREAMING_IO_BUFFER_THRESHOLD",// = 4,
    "NAVPROP_GET_BD_ISO_INFO",// = 5,
    "NAVPROP_GET_BD_ISO_CHAPTER_INFO"// = 6,
};
// Keep NAVI_* in sync with MediaPlayer.java.
enum player_navigation_control {
    NAVI_UP = 0,
    NAVI_DOWN = 1,
    NAVI_LEFT = 2,
    NAVI_RIGHT = 3,
    NAVI_ENTER = 4,
    NAVI_CHANGE_TITLE = 5,
    NAVI_CHANGE_CHAPTER = 6,
    NAVI_CHANGE_ANGLE = 7,
    NAVI_SWITCH_CHANNEL = 8,
};

enum media_player_audio_op_ids {
    AUDIO_OP_GET_TRACK = 1,
    AUDIO_OP_SET_TRACK = 2,
    AUDIO_OP_GET_CURRENT_INDEX = 3,
};

enum media_player_subtitle_op_ids {
    SUBTITLE_OP_GET_TRACK = 1,
    SUBTITLE_OP_SET_TRACK = 2,
    SUBTITLE_OP_GET_CURRENT_INDEX = 3,
};

enum media_player_bookmark_op_ids {
    BOOKMARK_OP_INIT = 0,
    BOOKMARK_OP_GET = 1,
    BOOKMARK_OP_SET = 2,
    BOOKMARK_OP_REMOVE = 3,
    BOOKMARK_OP_APPLY = 4,
    BOOKMARK_OP_MAX = 5
};

static const char* BookmarkOpName[BOOKMARK_OP_MAX] = {
    "BOOKMARK_OP_INIT",// = 0,
    "BOOKMARK_OP_GET",// = 1,
    "BOOKMARK_OP_SET",// = 2,
    "BOOKMARK_OP_REMOVE",// = 3,
    "BOOKMARK_OP_APPLY"// = 4,
};
// For transcode VO writeback flag used
enum transcode_vowriteback_flag {
    TRANSCODE_VOWRITE_BACK_ENABLE   = 0x01,
    TRANSCODE_VOWRITE_BACK_INCLUDE_OSD = 0x02,
    TRANSCODE_VOWRITE_BACK_INCLUDE_SUB = 0x04,
    TRANSCODE_VOWRITE_BACK_SHARE_BUFFER = 0x08,
};

enum {
    WORKER_CMD_NONE = 0,
    WORKER_CMD_GET_MEDIA_TYPE,
    WORKER_CMD_SET_SUBTITLE,
    WORKER_CMD_QUIT,
    WORKER_CMD_GET_VIDEO_TRACK_INFO,
    WORKER_CMD_GET_AUDIO_TRACK_INFO,
    WORKER_CMD_GET_SUBTITLE_TRACK_INFO,
    WORKER_CMD_INTERNAL_PAUSE,
    WORKER_CMD_INTERNAL_RESUME,
    WORKER_CMD_SIZE_OF_ENUM
};
typedef int WORKER_CMD_ID_e;

static const char* WorkerCmdNames[WORKER_CMD_SIZE_OF_ENUM] = { 
    "WORKER_CMD_NONE", // = 0
    "WORKER_CMD_GET_MEDIA_TYPE",// = 1
    "WORKER_CMD_SET_SUBTITLE",// = 2,
    "WORKER_CMD_QUIT",// = 3,
    "WORKER_CMD_GET_VIDEO_TRACK_INFO",// = 4,
    "WORKER_CMD_GET_AUDIO_TRACK_INFO",// = 5,
    "WORKER_CMD_GET_SUBTITLE_TRACK_INFO",// = 6,
    "WORKER_CMD_INTERNAL_PAUSE",// = 7,
    "WORKER_CMD_INTERNAL_RESUME"// = 8,
};

enum {
    TRACK_INFO_STATUS_UNINITIALIZED = 0,
    TRACK_INFO_STATUS_QUERY_VIDEO = 1,
    TRACK_INFO_STATUS_QUERY_AUDIO = (1 << 1),
    TRACK_INFO_STATUS_QUERY_SUBTITLE = (1 << 2),
    TRACK_INFO_STATUS_VIDEO_DONE = (1 << 3),
    TRACK_INFO_STATUS_AUDIO_DONE = (1 << 4),
    TRACK_INFO_STATUS_SUBTITLE_DONE = (1 << 5),
};
typedef int TRACK_INFO_STATUS_e;

#define WORKER_PARAM_SIZE (10)

typedef struct {
    WORKER_CMD_ID_e cmdId;
    int param[WORKER_PARAM_SIZE];
} WORKER_CMD_t;

// define the events for event callback
typedef enum {
    RT_PLAYER_FIRST_FRAME_READY,
} RT_PLAYER_EVT_e;

// define the event callback function type
typedef void (*RT_PLAYER_EVT_CB)(RT_PLAYER_EVT_e evt, void *handler, unsigned long param1, unsigned long param2);

typedef enum {
    SEND_COMMAND_OK,
    SEND_COMMAND_TIMEOUT,
    SEND_COMMAND_FAILED,
} SEND_COMMAND_STATE;

enum {
    SEND_COMMAND_PARM_TIMEMOUT_BLOCK = -1,
    SEND_COMMAND_PARM_TIMEMOUT_NOWAIT = 0
};

//Sync with MediaPlayServer.h
enum{
    RETURN_GASI_NOTREADY = -1,
    RETURN_GASI_NONE = -2,
};

enum {
    TRACK_INFO_STATUS_UNINIT = 0x55494e49,
    TRACK_INFO_STATUS_QUERY_COUNT = 0x51434e54,
    TRACK_INFO_STATUS_QUERY_ALL = 0x51414c4c,
    TRACK_INFO_STATUS_DONE = 0x444f4e45,
    TRACK_INFO_STATUS_ERROR = 0x45524f52
};
#define ENSURE_POSITION_ONE_BEFORE_BUFFER_FULL
#ifdef ENSURE_POSITION_ONE_BEFORE_BUFFER_FULL
typedef enum {
    FAKEPTS_STATE_INIT,
    FAKEPTS_STATE_SET,
    FAKEPTS_STATE_DONE,
    FAKEPTS_STATE_DIE
} FAKEPTS_STATE;
#endif

typedef struct _tagNAVLPCMINFO
{
    int             bitsPerSample;
    int             samplingRate;
    int             numChannels;
    int64_t 	fileSize;
} NAVLPCMINFO;

class RTMediaPlayer : public MediaPlayerHWInterface
{

public:
#ifdef USE_RTK_VOUT_UTIL
    RTMediaPlayer(bool bNeedInitRPC = true);
#else
    RTMediaPlayer();
#endif
    virtual             ~RTMediaPlayer();

    virtual status_t    initCheck();
    using MediaPlayerBase::setDataSource; //avoid "xxx hides overloaded virtual function"
    virtual status_t    setDataSource(const sp<IMediaHTTPService> &httpService, const char *url, const KeyedVector<String8, String8> *headers = NULL);
    virtual status_t    setDataSource(const char *url, const KeyedVector<String8, String8> *headers = NULL);
    virtual status_t    setDataSource(int fd, int64_t offset, int64_t length);
    // pass the buffered ISurfaceTexture to the media player service
    virtual status_t    setVideoSurfaceTexture(
        const sp<IGraphicBufferProducer> &bufferProducer);
    virtual status_t    prepare();

    virtual status_t    prepareAsync();
    virtual status_t    start();
    virtual status_t    stop();
    virtual status_t    pause();
    virtual bool        isPlaying() {
        return (m_state==PLAYING);    //need mutex protected?
    }
    virtual status_t    seekTo(int msec);
    virtual status_t    setStartTime(int msec);
    virtual status_t    getCurrentPosition(int *msec);
    virtual status_t    getVideoInfo(int *width, int *height);
    virtual status_t    getPlaybackPTS(long long *presentAudioPTS, long long *presentVideoPTS,
                                       long long *demuxAudioPTS, long long *demuxVideoPTS);
    virtual status_t    getDuration(int *msec);
    virtual status_t    fastforward(int speed);
    virtual status_t    fastrewind(int speed);
    virtual int    *getSubtitleInfo(int /*streamNum*/) {
        return NULL;
    };
    virtual status_t    setSubtitleInfo(int streamNum,int enable,int textEncoding,int textColor,int fontSize,int syncTime,int offset);
    virtual status_t    setAutoScanExtSubtitle(int autoScan);
    virtual status_t    setExtSubtitlePath(const char *path);
    virtual int         *getAudioTrackInfo(int /*streamNum*/) {
        return NULL;
    };
    virtual status_t    setAudioTrackInfo(int streamNum);
    virtual status_t    execSetGetNavProperty(int propertyID,int inDataSize,int outDataOffset,unsigned long *phyAddr);
    virtual status_t    deliverNaviControlCmd(int action, int target);
    virtual status_t    reset();
    virtual status_t    setLooping(int loop);
    virtual player_type playerType() {
        return REALTEK_PLAYER;
    }
    virtual status_t    setParameter(int key, const Parcel &request);
    virtual status_t    getParameter(int key, Parcel *reply);

    virtual status_t    setVideoScalingMode(int mode);
    virtual status_t    invoke(const Parcel& request, Parcel *reply);
    virtual status_t    getMetadata(const SortedVector<media::Metadata::Type>& ids, Parcel *records);

    virtual status_t    setVolume(float /*leftVolume*/, float /*rightVolume*/) {
        assert(0);
        return NO_ERROR;
    }
    virtual status_t    setAudioStreamType(audio_stream_type_t streamType);
    virtual status_t    setDataPathNotLoad(const char *url, const KeyedVector<String8, String8> *headers = NULL);
    virtual int         getState() {
        return m_state;
    };
    virtual int checkLoadMedia();
    virtual int getVideoHeight();
    virtual int getVideoWidth();
    virtual void setVideoDisapper(bool disapper);
    virtual int SetMediaAttr(const SOCKET_MEDIA_ATTRIBUTE* pAttr);

    /**
     * @brief Set the position and size of video display window
     * @parm  x         - the top left position (x-axis)
     *        y         - the top left position (y-axis)
     *        width     - the width of video display window
     *        height    - the height of video display window
     * @return 0  - succeed
     *         -1 - fail
     */
    virtual int SetVideoWindow(int x, int y, int width, int height);

    /**
     * @brief Set the display ratio
     * @parm  contentMode - letterbox/full. Default is full
     * @return null
     */
    virtual void SetContentMode(/*RTK_PLAYER_CONTENTMODE_e*/ int contentMode);

    /**
     * @brief Set the display ratio
     * @parm  contentMode - letterbox/full. Default is full
     * @return null
     */
    virtual void SetContentMode(RTK_PLAYER_CONTENTMODE_e contentMode);

    /**
     * @brief display video
     * @parn  null
     * @return 0  - succeed
     *         -1 - fail
     */
    virtual int VideoShow();

    /**
     * @brief hide video
     * @parm  null
     * @return 0  - succeed
     *         -1 - fail
     */
    virtual int VideoHide();

    /**
    * @return 0 - successfully.
    *        -1 - failed.
    **/
    virtual int writeData(STREAM_DATA_TYPE_e type, unsigned char* pBuffer, unsigned int nSize, uint64_t timestamp);

    /**
     * drop data in buffer.
     */
    virtual status_t dropData(long iFrameOnly = 0);

    /**
    * send notice to stop dropping data
    */
    virtual status_t sendFlush(long iFrameOnly = 0);

    /**
    * get buffer status of DataTransportServer
    **/
    virtual int getBufferStatus(long *total, long *fullness);

    /**
    * get the flag to check if the 1st frame has been decoded or not
    **/
    virtual bool checkFirstFrameReady();   
	

    /*
    APIs for get/set audio balance.
    1:left channel, 2: right channel, 3: stereo
     */
    virtual bool setAudioBalance(int balance);
    virtual int getAudioBalance();

    /**
     * @brief  register event callback function.
     * @param  pcb - callback function
     *         handler -
     * @return  none.
     */
    virtual void registerEventCallback(RT_PLAYER_EVT_CB pcb, void* handler);

    // APIs for transcode
    /**
     * Initialize transcode manager if bEnable is true; otherwise
     * stop transcoding and release transcode manger.
     *
     * When it is enabled, the default state of transcoding is pause.
     */
    virtual status_t    setTranscodeMode(bool bEnable);

    // APIs for transcode set VO writeback
    // Enable
    /**
     * Set Transcode VO writeback, for control
     */
    virtual status_t setTranscodeVOWriteback(const int32_t flag);

    // APIs for transcode set OSD status
    // Enable
    /**
     * Set Transcode OSD status, for control
     */
    virtual status_t setTranscodeOSDStatus(const int32_t flag);

    // APIs for LayerOrder
    /**
     * Set LayerOrder value
     */
    virtual status_t setLayerOrderValue(const uint32_t value);

    // APIs for g_play_speed
    /**
     * Set g_play_speed value
     */
    virtual status_t setPlaySpeedValue(const int32_t nSpeed);

    /**
     * Start trancoding.
     * Before calling it, you need to complete the configuration of output protocol
     * and target a/v format.
     */
    virtual status_t    startTranscode();

    /**
     * Pause transcoding.
     */
    virtual status_t    pauseTranscode();

    /**
     * Stop transcoding. It would stop transcode maanger and release it.
     * If you want to restart it again, you need to
     *   call setTranscodeMode to initialize it then call startTranscode to
     *   start it.
     */
    virtual status_t    stopTranscode();

    /**
     * Set the output protocol of TS stream.
     * It must be called before calling startTranscode; otherwise it
     * would return ERROR_TRANSCODE_WRONG_STATE.
     */
    virtual status_t    setOutputFormat(RtkTranscodeConf &conf);

    /**
     * New API for MediaPlayer interface
     */
    virtual status_t    setOutputFormat(int outputType,
                                int outputFormat,
                                int fileDesc,
                                const char *fileName,
                                sp<IMemory> &ashmemData,
                                sp<IMemory> &ashmemInfo,
                                const char *udpIp,
                                int udpPort,
                                bool rtpHeader);

    /**
     * Configure the A/V format of target media (output media stream).
     * It must be called before calling startTranscode; othewise it
     * would return ERROR_TRANSCODE_WRONG_STATE.
     */
    virtual status_t    configureTargetFormat(AV_FORMAT_CONFIG &targetFmt);

    /**
     * New API for MediaPlayer interface
     */
    virtual status_t    configureTargetFormat(  // video
                                        int width,
                                        int height,
                                        int fps,
                                        int v_bitrate,
                                        int iframeInterval,
                                        int rateControl,
                                        int aspectRatio,
                                        int interlace,
                                        int shareWBbuf,
                                        // audio
                                        int channelCount,
                                        int channelMode,
                                        int sampleRate,
                                        int a_bitrate);

    /**
     * register your ALooper hanlder.
     * It should be called before calling startTranscode; otherwise it
     * would return ERROR_TRANSCODE_WRONG_STATE.
     */
    virtual status_t    registerStatusHandler(sp<AHandler> &handlerId);

    /**
    * set video low delary mode. Be sure turn it off when terminate playback.
    */
    virtual status_t setVideoLowDelayMode(long mode);

    virtual status_t setVideoErrorLevel(long errConcealmentLevel, long errNotificationLevel);
	
    virtual status_t setMediaAttributes();

    /**
    * set video low Disable Reorder. Be sure turn it off when terminate playback.
    */
    virtual status_t SetVideoDisableReorder(int mode);


    /**
     * Set focus
     * @params -
     * focus[in] : 0 means UnFocus and 1 mean Focus. Focus would
     *             allow audio to be connected to AO; otherwise audio
     *             would not be connected to AO (in other words, it would
     *             be muted). If there is subtitle displayed, it would have
     *             the same effect as audio.
     * @return - NO_ERROR
     */
    status_t            setFocus(int focus);

    /**
     * Set setVideoDynamicScale
     * @brief - 4K scale need api for 1295 kylin
     * @params -
     * control[in] : 0 = Off, 1 = On
     * width[in] : target video width when control = 1
     * height[in] : target video height when control = 1
     * @return - NO_ERROR
     */
    status_t setVideoDynamicScale(int control = 0, int width = 0, int height = 0);

    void notify(int msg, int ext1, int ext2);
#if defined(__LINARO_SDK__)
    status_t setTranscodeMode2(bool bEnable,int32_t mOutputFormat);
#endif
private:
    typedef struct VIDEO_INFO {
        int type;
        int aspectRatioX;
        int aspectRatioY;
        int width;
        int height;
        float fps;
        int bps;
        int isProg;
        int isMVC;
        int isHDR;
        int isCompressBufferMode;
        int offsetTable_ysize;
        int offsetTable_csize;
    } VIDEO_INFO_t;
    typedef struct AUDIO_INFO {
        int idx;
        int type;
        int channelCount;
        int bps;
        int sampleRate;
        int pid;
        int langCode;
    } AUDIO_INFO_t;
    typedef struct SUBTITLE_INFO {
        int idx;
        int langCode;
        int from; // (EMBEDDED, TS_DVB, TS_TT, EXTERNAL)
        int type;
        char extLang[16];
    } SUBTITLE_INFO_t;
    struct NAVI_INFO {
        int numTitles;
        int currentTitle;
        int numChapters;
        int currentChapter;
        int numAngles;
        int currentAngle;
        int domain;
    };

	struct NAVI_BD_ISO_INFO
	{
		int numItems;
		unsigned int duration[200]; // MAX 100 titles
	};

    void xProcEntry();
    void xWorkerProcEntry();
    void xCheckPrepared();
    void xGetPlaybackPts();

    void internalPause();
    void internalResume();

    static void* procEntry(void* pInstance);
    static void* workerProcEntry(void* pInstance);
    static void* checkPreparedEntry(void* pInstance);
    static void* getPlaybackPtsEntry(void* pInstance);
    int processPBConfigCommand(const KeyedVector<String8, String8> *headers = NULL);
    status_t handleBookmarkOperation(const int opID, const Parcel &request, Parcel *reply);
    status_t handleSubtitleOperation(const Parcel &request, Parcel *reply);
    status_t handleAudioOperation(const Parcel &request, Parcel *reply);

    status_t setTvSystem(const int width, const int height, const int framerate, const bool isProgressive);
    status_t setLanguages(const int osd_lang, const int menu_lang, const int sub_lang, const int sub_text_encoding,
                          const int audio_lang, const int minikbd_lang = 0);
    status_t setVideoScaledSize(int width, int height);
    status_t getTrackInfo(Parcel *reply);
    status_t getVideoTrackInfo(Parcel *reply);
    status_t getAudioTrackInfo(Parcel *reply);
    status_t getSubtitleTrackInfo(Parcel *reply);

    inline void writeAudioTrackInfoToParcel(AUDIO_INFO_t info, Parcel *reply);
    inline void writeVideoTrackInfoToParcel(VIDEO_INFO_t info, Parcel *reply);
    inline void writeSubtitleTrackInfoToParcel(SUBTITLE_INFO_t info, Parcel *reply);

    bool _getSubtitleTrackInfo(int streamNum, SUBTITLE_INFO_t *pSubtitleInfo);
    bool _getAudioTrackInfo(int streamNum, AUDIO_INFO_t *pAudioInfo);
    bool _getVideoTrackInfo(VIDEO_INFO_t *pVideoInfo);

    bool _processBookmarkContentPath(const char *pInPath, int inPathLen, char **ppOutPath);

    status_t _handleNavProperty(const Parcel &request, Parcel *reply);

    // for transcode
    /**
     * Get the audio decoder agent ID from DvdPlayer
     */
    long getAudioDecoderAgentID();

    /**
     * Set the agent ID to RVSD.
     */
    status_t registerDvdPlayerAgentID(const sp<AHandler> &handlerId);

    /**
     * Initialize Transcode Manager.
     */
    status_t setupTranscodeManager();
    #if defined(__LINARO_SDK__)
    status_t setupTranscodeManager2(int32_t mOutputFormat);
    #endif

    double _now_ms();

    void updateVideoRectWin();

    void _doDelayedOperations();

    //for audio redirect
    void setupAudioRedirect();

    void flushAudioRedirect();

    void stopAudioRedirect();

    void restartAudioRedirect();

    // for RT_PLAYER_EVT_e event notification
    void _notifyEvent(RT_PLAYER_EVT_e evt);

    //
    void _enableErrorFeedback();

    void addWorkerCmd(WORKER_CMD_t* pCmd);
    WORKER_CMD_t getWorkerCmd();

    /**
     * Sent the command to DvdPlayer
     * timeOutMS:
     *    0: no wait, -1: block, others
     */
    SEND_COMMAND_STATE sendCommand(RT_CMD_ID cmdID, char* pInData, int inDataSize, int timeOut=0);
    void sendCommandDone(int seqNum);

    /**
     * Add seqnum to m_aSeqNumtoCheck
     * seqnum:
     *    the sequence number to be added.
     * return:
     *    int value : the index of the sequence number exist.
     *    UNDEFILE_SEQNUM_INDEX : m_aSeqNumtoCheck is full or add fail.
     */
    int addSeqNumtoChecklist(int seqnum);

    /**
     * Get the index of seqnum at m_aSeqNumtoCheck
     * seqnum:
     *    the sequence number to be checked.
     * return:
     *    int value : the index of the sequence number exist.
     *    UNDEFILE_SEQNUM_INDEX : seqnum is not exist in m_aSeqNumtoCheck.
     */
    int getSeqNumIndx(int seqnum);

    /**
     * initialization actions after first frame ready and played.
     *
     */
    void initAfterFirstFrameReady();
    void __getVideoTrackInfo();
    void handleGVSIresult(char *readBuf, int retValSize);
    void __getAudioTrackInfo();
    void handleGASIresult(char *readBuf, int retValSize);
    void __getSubtitleTrackInfo();
    void handleGSSIresult(char *readBuf, int retValSize);

    /**
      * input : int state that to transfer to string.
      * output : char *
      * NOTE : The returned char * memory space is allocated by malloc.
      *        Users should **free** memory if you don't use anymore.
      */
    char* getStateString(int state);
    void setState(int *m_state, int state);
    bool isQuerying(int *m_state);

#ifdef ENABLE_1195_CC_FLOW_NO_DECODE
    /**
     * CC Data Transport.
     *
     */
    void CCDataTransport_init();
    void CCDataTransport_finish();
    void xccProcEntry();
    static void* ccProcEntry(void* pInstance);
	void CCDataTransport_Enable(bool bEnable);
#endif

private:

    volatile enum {
        UNKNOWN = -1,
        FAIL = 0,
        SUCCESS = 1
    } m_load;

    volatile int m_state;

    int			m_duration;
    int			m_elapsedTime;

    bool        m_needTobeWakeup;
    sem_t		m_semToWaitResponse;
    pthread_t		m_thread;
    pthread_t		m_workerThread; // Thread to perform some internal operations
    pthread_mutex_t	m_cmdLock;
    pthread_mutex_t	m_ResetLock;
    Mutex           m_workerCmdLock;
    Condition       m_workerCondition;
    volatile bool	m_bAskToStopThread;

#ifdef ENABLE_1195_CC_FLOW_NO_DECODE
    pthread_t		m_ccThread; // Thread to perform cc transport
    volatile bool	m_bAskccStop;
    volatile bool	m_bccQuited;
#endif

    RT_IPC		m_ipc;
    audio_stream_type_t m_streamType;
    int			m_uop;
    bool		m_connectClosed;
    bool        m_workerQuited;
    bool		m_seeking;
    long long   m_seekDoneTime;
    volatile bool	m_bPrepared;
    pthread_t		m_checkPreparedThread;
    int m_trackCount;
    int m_videoTrackCount;
    int m_audioTrackCount;
    int m_audioTrackCurrentIndex;
    int m_subtitleTrackCount;
    int m_subtitleTrackCurrentIndex;
    VIDEO_INFO_t m_VideoTrackInfo;
    AUDIO_INFO_t m_AudioTrackInfo;
    SUBTITLE_INFO_t m_SubtitleTrackInfo;
    struct NAVI_INFO m_NaviInfo;
	struct NAVI_BD_ISO_INFO m_NaviBdIsoInfo;
    int m_MediaMajorType;
    int m_MediaSubType;
    int m_autoScanExtSubtitle;
    volatile int m_iVideoDisappear; // 1: disappear, 0: appear

    struct {
        int x;
        int y;
        int width;
        int height;
    } m_VideoWindowRect;

    bool            m_getRealPlaybackPts;
    pthread_t       m_getPlaybackPtsThread;
    struct {
        int64_t presentAudioPTS;
        int64_t presentVideoPTS;
        int64_t demuxAudioPTS;
        int64_t demuxVideoPTS;
    } m_PlaybackPtsInfo;

    bool m_playAtv;

    unsigned long m_psTempAddr;

    unsigned long m_psAddr;
    bool		m_getMetadataCalled;
    bool m_getRealTimeElapsedTime;
    int m_frameRate;

    unsigned long m_psSetGetNavAddr;
#ifdef ENABLE_RT_WIFIDISPLAY
    void *m_wfdCB;
#endif
    //int  m_navStateSize;
    //char m_navState[4096];
    int  m_bookmarkResult;

    char m_currentContentPath[CMD_BUF_SIZE];
    bool m_loadmediafail;

    RVSD *mRvsd;
#ifdef USE_RTK_VOUT_UTIL
    bool m_config_v1;
    VoutUtil *mVoutUtil;
    long m_VideoPlaneInstanceID; // we need to get it from DvdPlayer ?
#endif

    // for transcode
    enum {
        TRANSCODE_CONFIGURED_TARGET_AV_FORMAT = 0x1,
        TRANSCODE_CONFIGURED_OUTPUT_PROTOCOL  = 0x2,
    };
    sp<RtkTranscodePlayer> mTranscodeManager;
    sp<ALooper> mTranscodeManagerLooper;

    bool m_AudioRedirect;

    bool m_bTranscodeMode;
    int32_t m_TranscodeConfiguration;
    long m_audioDecAgentId;

    int m_audioBalance; // 1:left channel, 2: right channel, 3: stereo

    bool m_DelaySetContentMode;
    bool m_DelaySetVideoWin;
    bool m_ShowVideoNextRun;
    bool m_bFirstFrame;
    bool m_bRvsdReady;
    RTK_PLAYER_CONTENTMODE_e m_PreferredContentMode;
    sp<RTMediaPlayerEventListener> mListener;
    bool m_ChangeSpeedResult;
    bool m_bGetDuration;
    bool m_DeliverNaviControlCmdResult;
    RT_PLAYER_EVT_CB m_pEventCb;
    void *m_pEventHandler;
    Vector<WORKER_CMD_t> m_workerQueue;
    Vector<AUDIO_INFO_t> m_audioTrackInfoList;
    Vector<VIDEO_INFO_t> m_videoTrackInfoList;
    Vector<SUBTITLE_INFO_t> m_subtitleTrackInfoList;
    Vector<VIDEO_INFO_t> m_videoTrackInfoList_internal;
    Vector<AUDIO_INFO_t> m_audioTrackInfoList_internal;
    Vector<SUBTITLE_INFO_t> m_subtitleTrackInfoList_internal;
    TRACK_INFO_STATUS_e m_trackInfoStatus;
private: // for Contiguous memory used
    struct ContiguousMemInfo {
        unsigned long phyAddr;
        int size;
        int sharefd;
        void *pMemory;
        bool isReady;
    } m_ContiguousMemInfo;

    DataTransportServer	*m_pDataTransportServer;

    status_t    getContiguousMem(int size, ContiguousMemInfo &memInfo);
    status_t    freeContiguousMem(ContiguousMemInfo &memInfo);
    bool		m_bIsAllocateMemoryDone;

    AV_FORMAT_CONFIG mTargetFormat; //used by transcoding
    int mVideoScalingMode;
    int m_nCurrentCmdSeqNum;
    pthread_mutex_t	m_lockSeqNum;
#define MAX_SEQNUM_COUNT (5)
#define UNDEFINE_SEQNUM (-1)
#define UNDEFINE_SEQNUM_INDEX (-1)
    int m_aSeqNumtoCheck[MAX_SEQNUM_COUNT];
    bool m_bIsinitAfterFirstFrameReady;
    int m_GetVideoInfostate;
    int m_GetAudioInfostate;
    int m_AudioCountToQuery;
    int m_GetSubtitleInfostate;
    int m_SubtitleCountToQuery;
    bool m_bIsTrackInfoReported;

private:
    NAVLPCMINFO *m_pNAVLPCMInfo;
#ifdef ENSURE_POSITION_ONE_BEFORE_BUFFER_FULL
    FAKEPTS_STATE m_nFakePts;
    bool m_FirstEPTMRunned;
#endif
    int m_videoTrackCount_internal;
    int m_audioTrackCount_internal;
    int m_audioTrackCurrentIndex_internal;
    int m_subtitleTrackCount_internal;
    int m_subtitleTrackCurrentIndex_internal;
    VIDEO_INFO_t m_VideoTrackInfo_internal;
    AUDIO_INFO_t m_AudioTrackInfo_internal;
    SUBTITLE_INFO_t m_SubtitleTrackInfo_internal;
};

}; // namespace android

#endif // ANDROID_RT_MEDIAPLAYER_H
