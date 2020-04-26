// Static Model

// 

#ifndef __Top_Package__CRWAuthoring__
#define __Top_Package__CRWAuthoring__

// Include files
#include <Filters/CDVDAuthoring.h>
#include <Filters/CVRMI.h>
#include <StreamClass/CPin.h>
#include <Filters/CDVDProgMux.h>

#define RWAUTHOR_MAX_VOBU 8

class CRWAuthoring : public CDVDAuthoring
{

public:
    CRWAuthoring(const char *pDVDPath, int editOnly=0, VIDEO_FORMAT videoFormat=VIDEO_FORMAT_NTSC, int writePriority = -1);
    ~CRWAuthoring();

    HRESULT Pause(FILTER_STATE State);

    HRESULT ProcessPrivateInfo(BYTE *pInfo);

    HRESULT getRecordInfo(IN int Tnum,
                          OUT uint32_t * begin_lbn,
                          OUT uint32_t * end_lbn,
                          OUT bool * isProtect); 

    HRESULT getRecordStartAddr(IN int Tnum, OUT uint32_t *begin_lbn);
    HRESULT SetRecordInfo(IN OUT int *Tnum);
    HRESULT CanRecord(OUT bool *value);
    HRESULT CanDubbing(IN int titleNum, IN int totalTitleNum, 
                       IN int totalChapterNum, IN int totalCellNum, 
                       IN int totalVOBUNum, IN uint32_t totalSpace, 
                       OUT bool *value, OUT uint8_t **FreeCellIDList, 
                       OUT uint32_t *beginLBA, OUT int *recordTitleNum);
    HRESULT deleteTitle(IN int title);
    HRESULT Realize();
    HRESULT Initialize(const char *lpctstrDVDPath);
    HRESULT Open(const char *lpctstrDVDPath, OUT RW_OPEN_ERROR_CODE *err=NULL);
    HRESULT close();
    HRESULT Rename(IN int title, 
                   IN char *name,  IN int size,
                   IN char *altname, IN int altsize, IN char alt_charset_code);
    HRESULT getLastRecordInfo(OUT uint32_t * begin_lbn, OUT uint32_t * end_lbn, 
                              OUT bool * isProtect);

    HRESULT protectTitle(IN int title, IN bool isProtect);
    HRESULT GetFreeCellID(unsigned int* pCellID);

    HRESULT SetMANID(unsigned int size, unsigned char* pMANID);

    HRESULT linkTitle(IN int title, IN int nextTitle);   
	
    HRESULT AddChapterMark(IN int titleNum , IN int chapterNum, 
                           IN int cellNum, IN unsigned int markVobuAddr);

    HRESULT DeleteChapterMark(IN int titleNum, IN int chapterNum);

    HRESULT GetTitleChapterInfo(IN int titleNumber, 
                                OUT int *pNumberOfChapters, OUT int *pEntryCellNumber,
                                OUT unsigned int *pEntryCellStartLBA);

    HRESULT SplitTitle(IN int titleNumber, IN int PGNumber, IN int cellNumber, 
                       IN unsigned int VOBUStartLBA);
    HRESULT GetSplitTitleProgress(OUT int *currBlock, OUT int *totalBlock);
    HRESULT GetAddChapterMarkProgress(OUT int *currBlock, OUT int *totalBlock);
    HRESULT AbortEditing();

    bool CanMergeTitle(IN int title);
    HRESULT MergeTitle(IN int title);
    HRESULT getVobFilebyAddr(IN int title, IN int32_t keyFrameAddr, 
                             OUT int *vtsn, OUT int *vtsttn, OUT uint32_t *modifyAddr);
    HRESULT RecordingUserData(IN int title, OUT char **rec_ud);
    bool CheckPhilipsDisc();
    HRESULT MergePhilipsDiscVOB();
    HRESULT InitDubbing();
    HRESULT generateDummyTitle(unsigned int titleSize);
    HRESULT DeleteAllTitles();
    HRESULT GetDiscInfo(int *discType, unsigned int *l0_capacity);

     //Extra VR Chapter editing APIs
    HRESULT AddVRChapterMark(IN int titleNum , IN int chapterNum,
                           IN int cellNum, IN unsigned int markVobuAddr, IN unsigned int videoFrame);

    HRESULT DeleteVRChapterMark(IN int titleNum , IN int chapterNum);
    HRESULT GetVRChapterInfo(IN int titleNumber, OUT int *pNumberOfChapters, 
                               OUT int *pIsHided, OUT int *pCellNumber, OUT unsigned int *pStartLBA);
    HRESULT ConfigureVRChapterMark(IN int titleNum, IN int chapterNum, IN int setHide);
    HRESULT DeleteAllVRChapterMark(IN int titleNum);
    HRESULT MakeDVDCompatible();
    bool CanMakeDVDComaptible();

protected:
private:
    int m_newTitleVTSnum;
    int m_newTitleVTSTTnum;
    char m_FreeCellId[MAX_VTS_CELL_N];
    uint32_t m_startLBA;
    uint32_t m_endLBA;
    int m_firstCell;
    int m_totalTiltes;
    uint32_t m_titleSize;
    unsigned int m_RecSpace;
    int m_startVRCHP;
    unsigned int m_totalChapters;
    unsigned int m_totalCells;
    unsigned int m_vobuSize[RWAUTHOR_MAX_VOBU];
    bool m_DubbingFlag;	
    unsigned char m_notifyFlag;
    unsigned char m_onlyInit;
    bool m_isOpened;
    
    //For Split Title and Add Chapter Mark
    int m_currBlk;
    int m_totalBlk;
    bool m_abortEdit;
	
private:
    void init();
    int processNVPack(IN unsigned char*pData, IN unsigned int VOBUAddr, 
                      IN unsigned int splitPosition, IN unsigned int newCellID, 
                      IN unsigned int newCellELTM);
    int processNVPack(IN unsigned char*pData, IN unsigned int VOBUAddr, 
                      IN unsigned int splitPosition1, IN unsigned int splitPosition2,
                      IN unsigned int newCellID, IN unsigned int newCellELTM);                      
    HRESULT InsertOnePTT(IN int titleNumber, IN int PGNumber, IN int entryCellNumber);
    HRESULT DoMultipleSplitCell(IN int titleNum, IN int totalNewCell, IN SPLITCELL *pResultNode);
};

// END CLASS DEFINITION CRWAuthoring

#endif // __Top_Package__CRWAuthoring__


