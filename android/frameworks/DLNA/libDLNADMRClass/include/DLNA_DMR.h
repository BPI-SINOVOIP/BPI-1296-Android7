#ifndef __VENUS_DLNA_DMR_HEADER_FILE__
#define __VENUS_DLNA_DMR_HEADER_FILE__

#include <semaphore.h>
#include <pthread.h>

#define LOG_NDEBUG 1
#ifndef LOG_TAG
#define LOG_TAG "DMR"
#endif

#include "DMR.h"
#include "ILibParsers.h"
#include "DMRCommon.h"
#include "CdsObject.h"
#include "CdsMediaClass.h"
#include "MimeTypes.h"
#include "ILibThreadPool.h"

#include "MediaType.h"

#define S_OK	0
#define E_FAIL	-1


#include <ne_session.h>
#include <ne_uri.h>
#include <ne_basic.h>
#include <ne_utils.h>
#include <ne_redirect.h>

typedef enum
{
	DMR_SUBR_NOTHING    = 0x0,
	DMR_SUBR_AUDIO      = 0x1,
	DMR_SUBR_VIDEO      = 0x2,
	DMR_SUBR_IMAGE      = 0x4,
	DMR_SUBR_PLAYLIST   = 0x8,
	DMR_SUBR_TEXT       = 0x10,
	DMR_SUBR_UNKNOWN    = 0x20,
}SUBRENDERER_TYPE;

typedef enum
{
	DLNA_DMR_CB_NONE = 0,
	DLNA_DMR_CB_QUERYFORCONNECT,
	DLNA_DMR_CB_PREPAREFORCONNECT,
	DLNA_DMR_CB_PREPAREFORDISCONNECT,
	DLNA_DMR_CB_SETBRIGHTNESS,
	DLNA_DMR_CB_SETCONTRAST,
	DLNA_DMR_CB_SHOWSTATUS,
	DLNA_DMR_CB_UPDATEINFO,
	DLNA_DMR_CB_SHOWVOLUMESTATUS,
	DLNA_DMR_CB_SHOWDIALOG,
	DLNA_DMR_CB_RESTART,
	DLNA_DMR_CB_EVENT,
} DLNA_DMR_CB_FUNC;

typedef enum
{
	DLNA_DMR_NOTIFY_ID_QUERYFORCONNECTION,
    DLNA_DMR_NOTIFY_ID_SETAVTRANSPORTURI,
    DLNA_DMR_NOTIFY_ID_LOADMEDIA,
	DLNA_DMR_NOTIFY_ID_PLAY,
	DLNA_DMR_NOTIFY_ID_PAUSE,
	DLNA_DMR_NOTIFY_ID_STOP,
	DLNA_DMR_NOTIFY_ID_SEEK,
	DLNA_DMR_NOTIFY_ID_SETRATE,
	DLNA_DMR_NOTIFY_ID_FACTORY_DEFAULT,
	DLNA_DMR_NOTIFY_ID_SETVOLUME,
	DLNA_DMR_NOTIFY_ID_SETMUTE,
	DLNA_DMR_NOTIFY_ID_SETCONTRAST,
	DLNA_DMR_NOTIFY_ID_SETBRIGHTNESS,
	DLNA_DMR_NOTIFY_ID_RESTART_DMR,
} DLNA_DMR_NOTIFY_ID;

typedef enum
{
	DMR_UPDATE_STATUS_CURRENTPLAYMODE,
	DMR_UPDATE_STATUS_CONTRAST,
	DMR_UPDATE_STATUS_BRIGHTNESS,
	DMR_UPDATE_STATUS_VOLUME,
	DMR_UPDATE_STATUS_MUTE,
	DMR_UPDATE_STATUS_TRANSPORTSTATE,
	DMR_UPDATE_STATUS_DURATION,
	DMR_UPDATE_STATUS_TIMEPOSITION,
	DMR_UPDATE_STATUS_SEEK,
} DMR_UPDATE_STATUS_TYPE;

typedef enum
{
       DLNA_DMR_VOL_NONE = 0,
       DLNA_DMR_VOL_NUM,
       DLNA_DMR_VOL_MUTE,
}DLNA_DMR_VOL_STATUS;

typedef enum
{
	DMR_MESSAGE_DATA_TYPE_IGNORABLEFATALERROR,
} DMR_MESSAGE_DATA_TYPE;

struct DMRMessageData {
	DMR_MESSAGE_DATA_TYPE type;
	union {
		int fatal_error_code;
	} data;
};

typedef enum
{
	DLNA_DMR_RENDERERVAR_VOL_NUMBER,
	DLNA_DMR_RENDERERVAR_VOL_MUTE,
} DLNA_DMR_RENDERERVAR;

typedef void (*DMR_Init_Fn)(bool bInit);

class subRenderer {
friend class RTK_DLNA_DMR;
private:

public:
	SUBRENDERER_TYPE m_subRenderer_type;
	virtual ~subRenderer() {};

	virtual int preParse(char *filename, unsigned int *NumberOfTracks, char ***MediaTrackURI, SUBRENDERER_TYPE **MediaType, long *TotalTime,char ***ProtocalInfo = NULL) = 0;
	virtual int loadMedia(char *filename) = 0;
	virtual int Play(char *filename, int speed) = 0;
	virtual void Stop() = 0;
	virtual int Pause(bool Pause=true)= 0;
	virtual int SeekMediaPosition(int titleNum, long position){return E_FAIL;};
	virtual int SetRate(int rate){return E_FAIL;};
	virtual int GetResolution(int*x,int*y){return E_FAIL;};

	virtual void SetProtocolInfo(char *pinfo){};
	virtual void UpdateStatus(DMR_UPDATE_STATUS_TYPE type, long value){};

	SUBRENDERER_TYPE GetRendererType(){ return m_subRenderer_type;};
};

class mmRenderer : public subRenderer
{
private:
    long m_lInitSeekPosition;
    char *m_protocolInfo;
    char *m_pLoadURI;
    long m_lTotalTime;
    int m_Rate;

public:
    mmRenderer(SUBRENDERER_TYPE renderType);
    ~mmRenderer();

    int preParse(char *filename, unsigned int *NumberOfTracks, char ***MediaTrackURI, SUBRENDERER_TYPE **MediaType, long *TotalTime,char ***ProtocalInfo = NULL);
    int loadMedia(char *filename);
    int Play(char *filename, int speed);
    void Stop();
    int Pause(bool Pause=true);
    int SeekMediaPosition(int titleNum, long position);
    int SetRate(int rate);
    void SetProtocolInfo(char *pinfo);
    virtual void UpdateStatus(DMR_UPDATE_STATUS_TYPE type, long value);
};

class playlistRenderer: public subRenderer{
private:
	ne_session *m_pSess;
	ne_request *m_pReq;
	ne_uri     m_uri;
	off_t      m_curpos;
	off_t      m_contentlength;
	long m_Sockerror;
private:
	// always one file open
	int http_open(char *filename);
	int http_read(char *pBuffer, int size);
	void http_close();
	int getLineLength(char *pFileBuffer);
	int getListLength(char *pFileBuffer);
public:
	playlistRenderer();
	~playlistRenderer();

	int preParse(char *filename, unsigned int *NumberOfTracks, char ***MediaTrackURI, SUBRENDERER_TYPE **MediaType, long *TotalTime,char ***ProtocalInfo = NULL);
	int loadMedia(char *filename);
	int Play(char *filename, int speed){return S_OK;};
	void Stop(){};
	int Pause(bool Pause=true){return S_OK;};
};

class RTK_DLNA_DMR {

private:
	static void             *m_pDMR_MicroStack_Chain;
	static void             *m_pDMR_ILib_Pool;
	static char             *m_pDMR_ProtocolInfo;	// save in DMR.c, DMR_InternalState
	static char             *m_pDMR_mediaProtocolInfo;
	static char             *m_pDMR_mediaMimeTypeProtocol;
	static pthread_t         m_DMRthread;
	static pthread_t         m_DMRMonitorthread;
	static pthread_mutex_t      m_mutexSubRenderer;
	static bool              m_bIsInternalStop;
	static bool              m_bIsDMRBusy;
	static bool              m_bIsDMRChainAlive;
	static bool				 m_bIsDMRMonitorThreadRunning;

	static char*             m_pSetAVURI;
	static SUBRENDERER_TYPE  m_SETAVType;
	static ENUM_MEDIA_TYPE   m_mediaType;
	static int				 m_dialogState;
	static unsigned int      m_NumberOfTracks;
	static unsigned int      m_CurrentTrack;
	static char**            m_ppMediaTrackURI;
	static char**			 m_ppProtocolInfo;
	static SUBRENDERER_TYPE* m_pMediaType;
	static long              m_totalTime;
	static struct CdsObject *m_pMediaMetadata;

	static DMR_PlayState m_PlayState;
	static DMR_MediaPlayMode m_PlayMode;
	static int m_Speed;
	// sem_t and sync ...

	static void          *m_pDMR_Monitor;

	static int m_ipAddressLength;
	static int *m_pIpAddressList;

	//FOR PCM usage
	static int 	nBitPerSample;
	static int  nChannel;
	static int 	nSampleRate;

private:
	// type 0 strExtension  1 ProtocolInfo
	static SUBRENDERER_TYPE RendererType(char *strExtensionProtocolInfo,int type = 0);
	static SUBRENDERER_TYPE RendererType(struct CdsObject *data);
//	static void UpdateDMRState();
	static void    playback_monitor(void *param);
	//static void    DMR_Network_Monitor(void *);
	static void*   DMR_Network_Monitor(void *);
	static void*   PlayTrackThread(void *);
	static void*   Run(void *arg);
	static inline bool checkDMRIsInternalCommand(){return m_bIsInternalStop;};
	static inline void SetDMRInternalCommandInit(){m_bIsInternalStop = true;};
	static inline void SetDMRInternalCommandDone(){m_bIsInternalStop = false;};
	static inline void SetDMRBlock()           {pthread_mutex_lock(&m_mutexSubRenderer);m_bIsDMRBusy      = true;};
	static inline void SetDMRUnBlock()         {m_bIsDMRBusy      = false;pthread_mutex_unlock(&m_mutexSubRenderer);};
public:
	static inline bool checkDMRIsBusy()        {return m_bIsDMRBusy;};

	// event functions R
public:
	static int RTKMR_AVTransport_Stop(DMR instance, void *session);
	static void RTKMR_AVTransport_PlayNext(DMR instance, void *session);
private:
	static int RTKMR_AVTransport_SetAVTransportURI(DMR instance, void *session, char *uri, struct CdsObject *data);
	static int RTKMR_AVTransport_SetNextAVTransportURI(DMR instance, void *session, char *uri, struct CdsObject *data);
	static int RTKMR_ConnectionManager_GetAVProtocolInfo(DMR instance, void *session, char **protocolInfo);
	static int RTKMR_AVTransport_Play(DMR instance, void *session, char *playSpeed);
	static int RTKMR_AVTransport_SeekTrack(DMR instance, void *session, unsigned int trackIndex);
	static int RTKMR_AVTransport_SeekTrackPosition(DMR instance, void *session, long position);
	static int RTKMR_AVTransport_SeekMediaPosition(DMR instance, void *session, long position);
	static int RTKMR_AVTransport_Next(DMR instance, void *session);
	static int RTKMR_AVTransport_Previous(DMR instance, void *session);
	static int RTKMR_RenderingControl_SelectPreset(DMR instance, void *session, char *presetName);

	// event function O
	static int RTKMR_AVTransport_SetPlayMode(DMR instance, void *session, DMR_MediaPlayMode playMode);
	static int RTKMR_AVTransport_Pause(DMR instance, void *session);
#if defined(INCLUDE_FEATURE_VOLUME)
	static int RTKMR_RenderingControl_SetVolume(DMR instance, void *session, unsigned char volume);
	static int RTKMR_RenderingControl_SetMute(DMR instance, void *session, int mute);
#endif /* INCLUDE_FEATURE_VOLUME*/
#if defined(INCLUDE_FEATURE_DISPLAY)
	static int RTKMR_RenderingControl_SetContrast(DMR instance, void *session, unsigned char contrast);
	static int RTKMR_RenderingControl_SetBrightness(DMR instance, void *session, unsigned char brightness);
#endif /* INCLUDE_FEATURE_DISPLAY */
	//self added


public:
	static struct _DMR *m_pDMR_MicroStack;
	static subRenderer *m_psubRenderer;

	RTK_DLNA_DMR(char *friendlyName, char *deviceUUID, DMR_Init_Fn pInitFunc = NULL);
	~RTK_DLNA_DMR();

	void Start();		// daemon-mode Start
	void Terminate();

	SUBRENDERER_TYPE GetRendererType();//{return m_psubRenderer->GetRendererType();};
	char*            GetRenderMediaFilename();
	char*            GetRenderMediaFullname();
	static long             GetRenderMediaFilesize();
	static long             GetRenderMediaDuration();
	int              GetRenderMediaResolutinoX();
	int              GetRenderMediaResolutionY();
	int              GetRenderMediaColorDepth();
	static int  GetPCMSampleRate(){ return nSampleRate;};
	static int  GetPCMBitPerSample(){ return nBitPerSample;};
	static int  GetPCMChannel(){ return nChannel;};
    static const char *GetMediaProtocolInfo(){ return (const char*)m_pDMR_mediaProtocolInfo; };
	static char *GetMediaMimeTypeProtocol(){ return m_pDMR_mediaMimeTypeProtocol;};
	static ENUM_MEDIA_TYPE GetMediaTpye(){return m_mediaType;};
	static bool CheckPlayContainerURI();
	static void SetDialogState(int dstate){m_dialogState = dstate;};
	static int GetDialogState(){return m_dialogState;};
	bool isURIAlive();
	static void SetDMRMessageData(DMRMessageData &msgData);
	bool GetDMRMessageData(DMRMessageData &msgData);
	void SetDMRRendererVar(DLNA_DMR_RENDERERVAR type, unsigned char value);
	DMR_PlayState GetRenderPlayState() { return m_PlayState; };
	static void UpdateDMRPlayState(DMR_PlayState state);

public:
	//register callback functions from AP
	void RegisterDlnaDmrCallbackFunc( int (*updateFuncPtr)(int, void*), void *pParam ,DLNA_DMR_CB_FUNC func_type);
	void DlnaDmrInternalStop();
	void DlnaDmrInternalPause(bool bUpdateState=false);
	void DlnaDmrInternalUnPause(bool bUpdateState=false);
	void DlnaDmrInternalPlayNextURI();
	int  DlnaDmrInternalSeek(int seconds);
	void DlnaDmrSyncRendererVar(unsigned char brightness, unsigned char contrast, unsigned char volume, bool mute);
//	void UnregisterDlnaDmrCallbackFunc( int (*updateFuncPtr)(int, void*), void *pParam ,DLNA_DMR_CB_FUNC  func_type);
private:
	// callback functions
    static int (*QueryForConnection)(int, void *);
    static int (*PrepareForConnection)(int, void *);
    static int (*PrepareForDisconnection)(int, void *);
	static int (*SetBrightness)(int , void*);
	static int (*SetContrast)(int, void *);
	static int (*ShowStatus)(int, void*);
	static int (*UpdateMediaInfo)(int, void*);
	static int (*ShowVolumeStatus)(int, void*);
	static int (*RestartDMR)(int, void*);
    static int (*EventNotify)(int, void*);
    static DMR_Init_Fn m_pfnInitialize;

public:
	static int (*ShowDialog)(int,void*);
	static int ExecEventNotify(DLNA_DMR_NOTIFY_ID id, void* pData = NULL);
	static void UpdateStatus(DMR_UPDATE_STATUS_TYPE type, long value);

private:
	static int UpdateSubRender(SUBRENDERER_TYPE renderType);
	static void DMR_Monitor(void *);
};

#endif
