#ifndef __C_RECORD_FLOW_MANAGER_H__
#define __C_RECORD_FLOW_MANAGER_H__


#include <StreamClass/CFlowManager.h>
#include <StreamClass/CMediaType.h>

// muxer related
#include <Filters/CBaseMuxFilter.h>
#include <Filters/CDVDProgMux.h>
#include <Filters/CESMuxFilter.h>

// authoring filter related
#include <Filters/VRAuthoring.h>
#include <Filters/CFileAuthoring.h>
#include <Filters/spu_types.h>
#include <Filters/CRWAuthoring.h>
#include <Filters/CVIDEOAuthoring.h>


// for Thumbnail generation
#include <Filters/mpeg2enc.h>
#include <Flows/CPlaybackFlowManager.h>
#include <Platform_Lib/Graphics/user_cmd.h>
#include <Filters/NavigationFilter/NavDef.h>
#include <Filters/VideoDecoder/CMPEG2Decoder.h>
#include <Filters/NavigationFilter/CNavigationFilter.h>
 
// encoder related
class CVideoEncoderFilter;
#include <Filters/CAudioEncoderFilter.h>
#include <Filters/CAudioInputFilter.h>
#include <Filters/CPseudoAudioOutFilter.h>

//Disc Related 
#include <io/udfaccess/udf_if.h>

#define RECORD_FLOW_OUTPUT_SPACE    5
#define RECORD_FLOW_DISCERROR_THRESHOLD   10


#ifndef FLAG_SET
    #define IS_STATE(s, t)             ((s == t)? true:false)      
    #define IS_FLAG_SET(s)             ((s == true)? true:false)
    #define FLAG_SET(s)                (s = true)
    #define FLAG_CLEAR(s)              (s = false)
#endif    

typedef struct {
    CFlowManager*       pPlaybackFManager;
    CNavigationFilter*  pNavigatorFilter;
    CMPEG2Decoder*      pVDecoder;
    CReferenceClock*    pRefClock;
    INavControl*        pNavControl;
} THUMBNAIL_INFO;


typedef enum {
    RecordState_Stopped,
    RecordState_Paused,
    RecordState_EmergencyPaused,
    RecordState_Running,
    RecordState_None
} RECORD_STATE;

typedef enum {
    PLUS_RW_AUTHOR,
    DVD_VIDEO_AUTHOR,
    MINUS_RW_AUTHOR,
    FILE_AUTHOR,
    AVI_FILE_AUTHOR,
    AUTHOR_NONE
} AUTHOR_FILTER_TYPE;

typedef enum {
    DVD_PROG_MUXER,
    ES_MUXER,
    MUXER_NONE
} MUXER_FILTER_TYPE;

#define BASEFILTER_TO_PLUSRW_AUTHORFILTER(pFilter)    (dynamic_cast<CRWAuthoring*>(pFilter))    
#define BASEFILTER_TO_DVDVIDEO_AUTHORFILTER(pFilter)  (dynamic_cast<CVIDEOAuthoring*>(pFilter))    
#define BASEFILTER_TO_MINUSRW_AUTHORFILTER(pFilter)   (dynamic_cast<CVRAuthoring*>(pFilter))    
#define BASEFILTER_TO_FILE_AUTHORFILTER(pFilter)      (dynamic_cast<CFileAuthoring*>(pFilter)) 


class CRecordFlowManager: public CFlowManager{

public:
    CRecordFlowManager(bool bNeedRefClock=false, int editOnly=0);
    virtual ~CRecordFlowManager(void);

    HRESULT  AddFilter(IBaseFilter* pFilter, char* Id);
    HRESULT  RemoveFilter(IBaseFilter* pFilter);
    HRESULT  SetRecordFormat(MEDIA_TYPE *mt);

    HRESULT  Run(void);
    HRESULT  Stop(void);

    HRESULT  SetRate(long rate);

    HRESULT  StartEncode();
    HRESULT  PauseEncode();
    HRESULT  StopEncode(bool bNeedWaitThreeSecond = true);			
    HRESULT  ResumeEncode();		
    HRESULT  AutoPauseEncode(long isFinish);
            
    HRESULT  SetChapterPeriod(unsigned int period,VIDEO_FORMAT TVformat = VIDEO_FORMAT_NTSC);
        
    HRESULT  GenerateTitleMenu(bool drawEmptyTitles); 
    HRESULT  setMenuStyle(bool hasThumbnail) { return S_OK; }
    HRESULT  getMenuStyle(OUT bool *hasThumbnail) { return S_OK; }
    HRESULT  ShowThumbnail(IN plusrw_decode_multiple_keyframe_t *pMultiKeyFrame);    
    HRESULT  setOverWriteSeq(int numOfTitles, uint8_t *seqList) { return S_OK; }
    HRESULT  setNumOfTitlesPerPage(int numOfTitles) { return S_OK; }    
    HRESULT  CreatePlaybackFlow();
    HRESULT  TearDownPlaybackFlow();
    
    HRESULT  SemGive();
    IReferenceClock* GetInputRefClock() { return m_pInputRefClock; } 
    HRESULT  GetRecordPosition(unsigned int *second, unsigned int *frame);
    HRESULT  IssueDropAllData();
    HRESULT  SendCommand(char *command);
    
    bool  HasDefaultHandling(EVCode evCode);
    void  HandleDefaultEvents(EVCode evCode, long*  pParams, long   paramSize);
    bool  HandleCriticalEvents(EVCode evCode, long*  pParams, long   paramSize);

private:
    static void deliverData_loop(void* Data);
    
    // Filter related
    CBaseMuxFilter*             m_pMux;    
    CBaseFilter*                m_pAuthor;
    AUTHOR_FILTER_TYPE          m_authorType;
    MUXER_FILTER_TYPE           m_muxerType;
    
    CVideoEncoderFilter*        m_pVideoEncoder;   
    CAudioEncoderFilter*        m_pAudioEncoder;
    CAudioInputFilter*          m_pAudioIn;
    CPseudoAudioOutFilter*      m_pPseudoAudioOut;
    
    RECORD_STATE                m_state;
    osal_sem_t                  m_RecordingStopFlag; 
    osal_sem_t                  m_SemRdStatus;
       
    // cell/chapter related   
    int                         m_EditOnly;
    VIDEO_FORMAT                m_TVFormat;
    unsigned int                m_CellPeriod;
    unsigned int                m_ChapterPeriod;
    unsigned int                m_RecordPeriod;
    MEDIA_TYPE                  m_RecordFormat;
    unsigned int                m_CellFieldCount;
    unsigned int                m_ChapterFieldCount;
    unsigned int                m_CellClosed;
    unsigned int                m_ChapterClosed;
    //For dual layer record
    unsigned int                m_LayerBoundary;
    int                         m_StartRLBN;
    
    // for thumbnail decoding and VMGI sequence table
    STINFO                      m_Stinfo;
    int                         m_numOfSeq;
    int8_t                      m_preSeqEntry;   //previous sequence entry
    uint8_t                     m_seqList[99];   //overwrite sequence table    
    THUMBNAIL_INFO              m_thumbnailInfo;
    bool                        m_bPlayFlowCreate;
    
    // for encoder deliver
    CThread*                    m_pDeliverThread;
    osal_sem_t                  m_SemDeliver;


    // This ref closk is designed for A/V input, which is used to deal with the different
    // Clock between system(video) and audio
    IReferenceClock*            m_pInputRefClock;

    // Pause/Resume/EmergencyPause related
    int                         m_EmergencyPauseCnt;
    bool                        m_PrepareEmergencyPauseFlag;
    int64_t                     m_EmergencyPauseTime;
    int64_t                     m_startEncodeTime;
    int64_t                     m_pauseEncodeTime;    
    int64_t                     m_resumeEncodeTime;
    
    // record error situation flag related
    bool                        m_bRecordFullFlag;    
    bool                        m_bDiscErrorFlag;    
    int                         m_discFullEventCount;    
    int                         m_transcodeStopCount;
    
};

#endif
