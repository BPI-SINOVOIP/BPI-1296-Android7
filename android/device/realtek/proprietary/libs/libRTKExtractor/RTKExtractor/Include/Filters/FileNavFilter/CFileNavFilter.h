#ifndef __C_FILE_NAV_FILTER_H__
#define __C_FILE_NAV_FILTER_H__

#include <OSAL.h>
#include <CStream.h>
#include <StreamClass/CMemAllocator.h>
#include <io/dvr_ifo_types.h>
#include <io/dvr_ifo_access.h>
#include <Filters/CDVDProgMux.h>
#include <Filters/FileNavFilter/CFileNavPin.h>
#include <Filters/rtr_vmgi.h>
#include <UTIL/ulDebug.h>
#include <Utility/database/ebase.h>
#include <Utility/ResourceMgr/ioResource.h>


#define MAX_PATH_LEN            256
#define FIRST_SEND_VOBU_NUMS    240

#define	PACK_SZ                 (2*1024)
#define MAX_READ_BLOCK_NUMBER   32 
#define _READ_BLOCK_SIZE        (MAX_READ_BLOCK_NUMBER * PACK_SZ)

#define TVS_525_60              0
#define TVS_625_50              1 

#define DVD_DUBBING_DATA_QUEUE_SIZE   256

#define DVD_AUTO_CHAPTER_SECOND  (4*60)

typedef struct dubbing_data_t {
    uint32_t    PGnum;
    uint16_t    startMVOBnum;
    uint16_t    endMVOBnum;
} dubbing_data_t;

typedef struct c_eltm_t {
    uint8_t    i8Hour;
    uint8_t    i8Min;
    uint8_t    i8Sec;
    uint8_t    i8Frame;
    uint8_t    i8tc_flag;
} c_eltm_t;

typedef enum{
  DVD_DUBBING_READ_NONE,
  DVD_DUBBING_READ_START,
  DVD_DUBBING_READ_DONE,
  DVD_DUBBING_READ_ABORT
}DVD_DUBBING_READ_STATUS;

typedef enum{
  DVD_DUBBING_PROCESS_NONE,
  DVD_DUBBING_PROCESS_START,
  DVD_DUBBING_PROCESS_DONE,
  DVD_DUBBING_PROCESS_END
}DVD_DUBBING_PROCESS_STATUS;

class CFileNavFilter: public CBaseFilter {

friend class COutFileNavPin;

public:
    CFileNavFilter(void);
    ~CFileNavFilter(void);
                                
    HRESULT     Stop(void);
    HRESULT     Pause(FILTER_STATE State);
    HRESULT     Run(void);
    HRESULT     Abort(void);

    HRESULT     Open(const char *path);
    HRESULT     Close(void);

    uint32_t    SetDubbingPGNo(IN uint32_t totalPGnum, IN uint32_t* pPGnumAry);
    bool        SetProperty(IN uint8_t* pCellIDList, IN uint32_t startAddr);
    bool        GetRecordStatus(OUT uint8_t *cellNums, OUT uint8_t *chapterNums, OUT uint32_t *totalVOBUNs);
    uint32_t    GetCurrentDubbingSize()	    { return m_VOBsz; }
	
		
protected:
    static void FileNavThreadEntry(void* pData);
    static void FileNavReadThreadEntry(void* pData);
    bool        ThreadFileNav(void);
    int         ReadDataThread(void);
    
private:
    void        xSetAudioBitRate(uint8_t i8Bitrate, unsigned int *bitsPerSecond);
    void        xSetAudioType(uint8_t i8ACM, ENUM_MEDIA_TYPE *type);
    void        xSetGenInfo(PROG_STREAM_GEN_INFO *GenInfo, uint8_t M_VOB_STIn);
    void        xGetVROAddr(uint16_t i16MVOBNo, uint32_t *pi32VROStartBlockNo, 
                            uint32_t *pi32VROEndBlockNo);    
    bool        xSetVOB_Cell_id(BYTE* pNavPKT, uint32_t navStartAddr, uint8_t i8CellNo, 
                                bool bIsStartNav, m_vobi_t *pM_VOBI, 
                                uint32_t startVOBUn, uint32_t endVOBUn, uint32_t currVOBUn);
    void        xSetTVSystem(uint8_t TVSystem);
    uint32_t    xCheckDubbingPGs();
    uint16_t    xGetPGStartCellNo(uint16_t PGnum);
    bool        xCheckSTIdata(uint8_t i8M_VOB_STIN);
    uint32_t    xGetMVOBsize(uint16_t targetMVOBnum);
    bool        xRead_Blocks(uint32_t offset, int block_count, unsigned char *data);
    int         xCal_C_ELTM(BYTE* pPTR);
    int         xSave_C_ELTM(BYTE* pPTR);
    void        xSet_VOBU_SRI(BYTE* pNavPKT, m_vobi_t *pM_VOBI,
                              uint32_t startVOBUn, uint32_t endVOBUn, uint32_t currVOBUn);  
    int32_t     xGetTotalSCells(IN int cellN); 
    bool        xGetSCellInfo(IN int cellN, IN int chapter, 
                                OUT uint32_t *startVOBUn, OUT uint32_t *endVOBUn);
 
protected:   
    COutFileNavPin*            m_pOutPin;
    COutFileNavInfoPin*        m_pOutInfoPin;
    CThread*                   m_pThread;
    CThread*                   m_pReadThread;
    osal_sem_t                 m_DataSem;

    unsigned int               m_writePriority;
    DVD_DUBBING_READ_STATUS    m_readThreadStatus;
    DVD_DUBBING_PROCESS_STATUS m_processThreadStatus;

    CMemListAllocator*	        m_pAllocator;
    IMediaSample*               m_pDataArray[DVD_DUBBING_DATA_QUEUE_SIZE];
    uint32_t                    m_DataInIndex;
    uint32_t                    m_DataOutIndex;

    char                        m_FilePath[MAX_PATH_LEN];  
    CRTR_VMGI*                  m_pRtrVmgi;    
    dubbing_data_t              m_pDubbingData;
    bool                        m_bEndOfStream;
    bool                        m_bIsSendEndEvent;	
    uint8_t                     m_TVSystem;
    uint32_t                    m_LastVOBEndAddr;    
    uint8_t*                    m_pCell_id_List;
    uint8_t                     m_curCellIDidx;
    uint32_t                    m_VOBsz;	// count current dubbing size in every title
    unsigned char               *m_tmp_data;
    unsigned char               *m_ptr1;	
    c_eltm_t                    m_C_ELTM;    
    uint32_t                    m_NavPktStartAddr;
} ;

#endif //__C_FILE_NAV_FILTER_H__

