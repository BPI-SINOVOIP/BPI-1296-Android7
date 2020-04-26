// Static Model

// 

#ifndef __CDVDAuthoring__
#define __CDVDAuthoring__


// Include files
#include <CStream.h>
#include <Filters/CDVDVideoTS.h>
//#include <Filters/IBaseAuthoring.h>
#include <Filters/CVRMI.h>
#include <Filters/CVTSI.h>
#include <Filters/CVMGI.h>

#include <Filters/CDataOutput.h>
#include <Filters/SpuGen.h>

// for create thumbnail playlist
#include <Filters/PrivateInfo.h>   
#include <Filters/NavigationFilter/NavDef.h>
#include <fcntl.h>
#include <unistd.h>
#include <Utility/ResourceMgr/ioResource.h>

namespace UDF{
#include <io/udfaccess/func.h>
#include <io/udfaccess/profile.h>
}

#include <io/dvd_path_define.h>
#include <Application/AppClass/setupdef.h>

struct _STINFO;

typedef struct {
    unsigned int hour;
    unsigned int minute;
    unsigned int second;
    unsigned int frame;
}TITLE_LENGTH;

typedef struct {
    int nTitleNo;
    int nOutputFormat;
    int nTargetWidth;
    int nTargetHeight;
    int nTargetX;
    int nTargetY;
    int nPitch;
    unsigned char *pTarget;
    unsigned char *pTargetChroma;
} plusrw_decode_keyframe_t;

typedef struct {
    int nKeyFrameNum;
    plusrw_decode_keyframe_t *pDecKeyFrame;
} plusrw_decode_multiple_keyframe_t;


typedef enum {
    RW_CONTENT_ERROR,
    RW_NO_CONTENT,
    RW_DISC_FULL,
    RW_OTHER
} RW_OPEN_ERROR_CODE;

class CDVDAuthoringDataInputPin;
class CDVDAuthoringInfoInputPin;

class CDVDDataOutput : public CDataOutput
{
public:
    CDVDDataOutput(unsigned int bufferSize);
    virtual ~CDVDDataOutput(void);
    void SetWritePriority(int priority);
    void SetDropDataFlag(bool isDrop);

protected:
    bool myDataOutput(void);
    int           m_writePriority;
    bool          m_bDropData;
    unsigned int  m_failCount;
};

class CDVDAuthoring : public CBaseFilter
{
    friend class CDVDAuthoringDataInputPin;
    friend class CDVDAuthoringInfoInputPin;
    friend class CDVDDataOutput;

public:
    CDVDAuthoring(const char *pDVDPath, int writePriority);
    ~CDVDAuthoring();

    HRESULT Run(void);
    HRESULT Stop();

    virtual HRESULT Open(const char *lpctstrDVDPath, OUT RW_OPEN_ERROR_CODE *err=NULL) = 0;
    virtual HRESULT Realize() = 0;
    virtual HRESULT Initialize(const char *lpctstrDVDPath)=0;

    void Serialize();
    void Deserialize();
    HRESULT Notify(EVCode EventCode, long ParamSize, long* pEventParams);
    HRESULT getTotalTitle(OUT int *Tnum);
    HRESULT getTotalNonEmptyTitle(OUT int *Tnum);
    virtual HRESULT getRecordInfo(IN int Tnum, OUT uint32_t * begin_lbn,
                                  OUT uint32_t * end_lbn, OUT bool * isProtect)=0; 
    virtual HRESULT getRecordStartAddr(IN int Tnum, OUT uint32_t *begin_lbn)=0;
    virtual HRESULT SetRecordInfo(IN OUT int *Tnum) = 0;
    virtual HRESULT CanRecord(OUT bool *value) = 0;
    virtual HRESULT deleteTitle(IN int title) = 0;
    HRESULT Finalize();
    char *getTitleName(IN int title);
    virtual HRESULT close() = 0;
    virtual HRESULT Rename(IN int title, 
                           IN char *name,  IN int size,
                           IN char *altname, IN int altsize, IN char alt_charset_code) = 0;
    HRESULT getLastRecordInfo(OUT uint32_t * begin_lbn, 
                              OUT uint32_t * end_lbn,
                              OUT bool * isProtect);
    virtual HRESULT protectTitle(IN int title, IN bool isProtect) = 0; 
    unsigned int GetTotalFreeChapters(int title);
    HRESULT GetKeyFrameInfo(int RecordingsNum,
                            int* pFrameNumInVOBU,
                            unsigned int * pVobuAddr);
    HRESULT SetKeyFrameInfo(int RecordingsNum,
                            int pFrameNumInVOBU,
                            unsigned int VobuAddr);
    HRESULT PrepareMenu(struct _STINFO *pStinfo, 
                        int pageNum, int numOfPages);
    HRESULT ManufacturerID(OUT char **man_id);
    HRESULT ManufacturerUserData(OUT char **man_ud);
    HRESULT isEmptyTitle(IN int title, OUT bool *value);
    HRESULT getTitleSize(IN int title, OUT unsigned int *spaceStr);

    dvd_info_t* getDVDHandle();
	    		   
    virtual HRESULT ProcessPrivateInfo(BYTE *pInfo) = 0;
    HRESULT ClearUpdateTitleMenu();
    bool AskIfMenuHasThumbnail();
    bool AskIfDrawEmptyTitlesOnMenu();
    int getNumOfTitlesPerPage();
    char *getBackgroundPicture();
    void getIfoPalette(uint32_t *palette);
    HRESULT CreateBackgroundDecodePlaylist(IN FILE *fd, 
                                           IN unsigned char *pTarget,
                                           IN unsigned char *pTargetChroma);
    HRESULT CreateMenuThumbnailList(IN FILE *fd, 
                                    IN unsigned char *pTarget, IN unsigned char *pTargetChroma, 
                                    IN int pageNum, IN int numOfPages);
    HRESULT CreateOSDThumbnailList(IN char *filename, 
                                   IN plusrw_decode_multiple_keyframe_t *pMultiKeyFrame);
    virtual HRESULT getVobFilebyAddr(IN int title, IN int32_t keyFrameAddr, 
                                     OUT int *vtsn, OUT int *vtsttn, OUT uint32_t *modifyAddr)=0;

    HRESULT GetTitleLength(IN int titleNum, OUT TITLE_LENGTH *pLength);
    HRESULT GetTVSystem(OUT bool *isNTSC);

    virtual HRESULT DeleteAllTitles() = 0;
    int GetWritePriority();

    HRESULT SetRecordingQuality(IN ENUM_REC_QUALITY quality);
    HRESULT GetRecordingQuality(IN int titleNum, OUT ENUM_REC_QUALITY * quality);

    void DropAllData();

    HRESULT ExecuteCommandLine(char* command);
	

protected:
    CMemListAllocator *m_pAllocator;
    dvd_info_t *m_pDVD;
    CVRMI *m_pVrmi;
    dvd_file_t *m_pVobFile;
    CVMGI *m_pVmgi;
    osal_mutex_t* m_pVmgiLock;
    CVTSI *m_pVtsi[3];
    osal_mutex_t* m_pVtsiLock[3];
    int m_newTitle;
    int m_lastTitle;	
    CDVDAuthoringDataInputPin *m_pDataInputPin;
    CDVDAuthoringInfoInputPin *m_pInfoInputPin;
    CDVDDataOutput *m_pOutput;
    CDVDVideoTS *m_pDVDCommon;
    ts_handle_t *m_pTSInfo;
    osal_mutex_t* m_pTSLock;

    int m_videoFormat;
    unsigned int m_totalDataSize;
    unsigned int m_recordTotalFields;
    UDF::dinfo *m_discinfo;
    int m_disctypeflag;
    unsigned int m_fullThreshold; // if remain free space is less than this value,
                                  // means we have to take this disc as full state !
    bool m_mergeVobFileFlag;
    ENUM_REC_QUALITY m_newRecQuality;
    // resource manage
    RESOURCE_NAME      m_ioDevice;
protected:
    HRESULT UpdateMenu();
    HRESULT getTitleSizeString(IN int title, OUT char *spaceStr);
    int MapToNonEmptyTitleNumber(IN int orgTnum);
private:

};

// END CLASS DEFINITION CDVDAuthoring

class CDVDAuthoringDataInputPin : public CBaseInputPin
{
    friend class CDVDAuthoring;

public:
    CDVDAuthoringDataInputPin(const char *pId);
    ~CDVDAuthoringDataInputPin();

    HRESULT Receive(IMediaSample* pSample);
    HRESULT ReceiveConnection(IPin* pConnector, const MEDIA_TYPE* pMediaType);

    HRESULT QueryAccept(const MEDIA_TYPE *pMediaType);
    HRESULT PrivateInfo(BYTE *pInfo, int nLength);

    HRESULT EndOfStream(unsigned int eventID);
};

class CDVDAuthoringInfoInputPin : public CBaseInputPin
{
    friend class CDVDAuthoring;

public:
    CDVDAuthoringInfoInputPin(const char *pId);
    ~CDVDAuthoringInfoInputPin();

    HRESULT QueryAccept(const MEDIA_TYPE *pMediaType)	{ return S_OK; }
    HRESULT PrivateInfo(BYTE *pInfo, int nLength);
};

#endif // __CDVDAuthoring__

