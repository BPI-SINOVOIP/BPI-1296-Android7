#ifndef __GENERAL_IFO_H__
#define __GENERAL_IFO_H__

class CStateMap;
class CCGMSInfo;
class CChapterInfo;
class CPosTable;
#define FIXBUG20671

class CGeneralIFO
{
public:

    enum STATE_TYPE
    {
        STATE_TS,
        STATE_PS,
        STATE_TS_ATSC,
        STATE_TS_DVBT,
    };

	enum CGMS_TYPE
	{
		COPY_ONCE,
		COPY_NEVER,
		COPY_FREE,
		PLAY_NEVER,	
	};

#ifdef WIN32
	enum {
    CIRCULAR_MIN_UNITS   = (8192),
    CIRCULAR_GUARD_UNITS = (512),
   
    //
    // Shared Fields and Tables between Recording and Playback
    //
    //     - one absolute TMAP entry corresponds to 32 relative TMAP entries, each relative entry maps to one second in time
    //     - each block (absolute + relative) covers 32*1024 seconds, about 9.1 hours
    //     - maximal number of blocks are 32, which covers about 12 days in time, and uses about 1152K bytes memory
    //	   - extend one absolute entry to 64 for bug #12922 and one block length should be about 9.1*2 hr now.

    ABSOLUTE_TMAP_ENTRIES_PER_BLOCK = 1024,
    ABSOLUTE_TMAP_BYTES_PER_BLOCK   = ABSOLUTE_TMAP_ENTRIES_PER_BLOCK * sizeof(unsigned int),
    RELATIVE_ABSOLUTE_RATIO         = 64,
    RELATIVE_TMAP_ENTRIES_PER_BLOCK = ABSOLUTE_TMAP_ENTRIES_PER_BLOCK * RELATIVE_ABSOLUTE_RATIO,
    RELATIVE_TMAP_BYTES_PER_BLOCK   = RELATIVE_TMAP_ENTRIES_PER_BLOCK * sizeof(unsigned char),
    RELATIVE_EXTRA_BYTES_PER_BLOCK  = RELATIVE_TMAP_BYTES_PER_BLOCK >> 3,
    RELATIVE_UNIT_BYTES_PER_BLOCK   = RELATIVE_TMAP_BYTES_PER_BLOCK >> 3,
    MAX_TMAP_BLOCKS                 = 32,
    MAX_BYTES_PER_STATE             = 32 * 1024,
    MAX_NUM_OF_BACKUP_TMAP 				  = 512,
    MAX_PACKET_HEADER_LENGTH = 32,
    UNIT_SIZE_EXP = (12),
	};
#else
protected:

    const static int CIRCULAR_MIN_UNITS   = (8192);
    const static int CIRCULAR_GUARD_UNITS = (512);
   
    //
    // Shared Fields and Tables between Recording and Playback
    //
    //     - one absolute TMAP entry corresponds to 32 relative TMAP entries, each relative entry maps to one second in time
    //     - each block (absolute + relative) covers 32*1024 seconds, about 9.1 hours
    //     - maximal number of blocks are 32, which covers about 12 days in time, and uses about 1152K bytes memory
    //	   - extend one absolute entry to 64 for bug #12922 and one block length should be about 9.1*2 hr now.

    const static unsigned int ABSOLUTE_TMAP_ENTRIES_PER_BLOCK = 1024;
    const static unsigned int ABSOLUTE_TMAP_BYTES_PER_BLOCK   = ABSOLUTE_TMAP_ENTRIES_PER_BLOCK * sizeof(unsigned int);
    const static unsigned int RELATIVE_ABSOLUTE_RATIO         = 64;
    const static unsigned int RELATIVE_TMAP_ENTRIES_PER_BLOCK = ABSOLUTE_TMAP_ENTRIES_PER_BLOCK * RELATIVE_ABSOLUTE_RATIO;
    const static unsigned int RELATIVE_TMAP_BYTES_PER_BLOCK   = RELATIVE_TMAP_ENTRIES_PER_BLOCK * sizeof(unsigned char);
    const static unsigned int RELATIVE_EXTRA_BYTES_PER_BLOCK  = RELATIVE_TMAP_BYTES_PER_BLOCK >> 3;
    const static unsigned int RELATIVE_UNIT_BYTES_PER_BLOCK   = RELATIVE_TMAP_BYTES_PER_BLOCK >> 3;
    const static unsigned int MAX_TMAP_BLOCKS                 = 32;
    const static unsigned int MAX_BYTES_PER_STATE             = 32 * 1024;
    const static int MAX_NUM_OF_BACKUP_TMAP 				  = 512;
    const static unsigned int MAX_PACKET_HEADER_LENGTH = 32;
public:
    const static int UNIT_SIZE_EXP = (12); // 2 ^ 12 = 4096 bytes ////for devices with sector size 4k(refer to bug 18414)            
protected:
#endif
    struct TMAP_INDEX
    {
        unsigned int blk; // block index
        unsigned int abs; // absolute entry index
        unsigned int rtv; // relative entry index
    };

    struct ENTRY_RANGE
    {
        unsigned int startPos;
        unsigned int endPos;
    };

    struct IFO_CONTEXT
    {
        bool           bCircular;                      // true if bit-stream is recorded into a circular buffer
        unsigned int   TMAPTimeUnit;
        unsigned int   preambleLength;                 // number of meta data bytes preceding media data
        unsigned int   unitsInBuffer;                  // the circular buffer capacity in units (not used if non-circular)
        unsigned int   recordPos;                      // the latest record position reported by recording
        unsigned int   headPos;                        // buffer head position
        unsigned int   tailPos;                        // buffer tail position (0 if non-circular)
        unsigned int   lastTMapEntryPos;               // position of the latest added TMAP entry
        unsigned int   recordElapsedSeconds;           // number of seconds elapsed since the beginning of recording
        unsigned int   secondsRecordedInBuffer;        // number of seconds recorded in buffer (circular or non-circular)
        unsigned int*  absTMAPBlocks[MAX_TMAP_BLOCKS]; // absolute TMAP blocks
        unsigned char* rtvTMAPBlocks[MAX_TMAP_BLOCKS]; // relative TMAP blocks
        unsigned char* rtvTMAPExtra[MAX_TMAP_BLOCKS];  // extra MSB bit for each relative TMAP entry
        unsigned char* rtvTMAPUnit[MAX_TMAP_BLOCKS];   // unit flag for each relative TMAP entry
        TMAP_INDEX     idxRec;                         // TMAP index of recording position
        TMAP_INDEX     idxPlay;                        // TMAP index of playback position
        TMAP_INDEX     idxRead;                        // TMAP index of read position
        TMAP_INDEX     idxTail;                        // TMAP index of circular buffer tail (not used if non-circular)
        CStateMap* 		pStateMap;               	
        CCGMSInfo*		pCGMSInfo;
		CChapterInfo*	pChapterInfo;

		int				unitSizeExp;

    } * m_pIFOContext;

    osal_mutex_t* m_pIFOMutex;

    CPosTable* m_pFutureTMAP;
    
    //
    // Local cache for faster look-up (from playback side)
    //

    ENTRY_RANGE  m_currEntry;       // range of currently played TMAP entry
    unsigned int m_currEntrySecond; // elapsed second of currently played TMAP entry
    ENTRY_RANGE  m_readEntry;       // range of currently read TMAP entry
    unsigned int m_readUnitsMax;    // max units per read

    //
    // Variables for IFO Read/Write Processing
    //

    enum { READING_WRITING_IDLE,

           READING_MAIN_HEADER,
           READING_ABS_HEADER,
           READING_ABS_TABLE,
           READING_RTV_HEADER,
           READING_RTV_TABLE,
           READING_RTV_EXTRA_HEADER,
           READING_RTV_EXTRA_TABLE,
           READING_RTV_UNIT_HEADER,
           READING_RTV_UNIT_TABLE,
           READING_CGMS_HEADER,
           READING_CGMS_TABLE,
           READING_CHAPTER_HEADER,
           READING_CHAPTER_TABLE,
           READING_STATE_HEADER,
           READING_STATE_SUB_HEADER,
           READING_STATE_BODY,
		   READING_UNIT_SIZE_EXP,
           READING_SIGNATURE,
           READING_COMPLETED,

           WRITING_MAIN_HEADER,
           WRITING_ABS_HEADER,
           WRITING_ABS_TABLE,
           WRITING_RTV_HEADER,
           WRITING_RTV_TABLE,
           WRITING_RTV_EXTRA_HEADER,
           WRITING_RTV_EXTRA_TABLE,
           WRITING_RTV_UNIT_HEADER,
           WRITING_RTV_UNIT_TABLE,
           WRITING_CGMS_HEADER,
           WRITING_CGMS_TABLE,
           WRITING_CHAPTER_HEADER,
           WRITING_CHAPTER_TABLE,
           WRITING_STATE_HEADER,
           WRITING_STATE_SUB_HEADER,
           WRITING_STATE_BODY,
		   WRITING_UNIT_SIZE_EXP,
           WRITING_SIGNATURE,
           WRITING_COMPLETED

    } m_ifoProcessingStep;

    unsigned int m_ifoRemainingBytes;
    unsigned int m_ifoBlockByteOffset;
    unsigned int m_ifoBlockIndex;

    unsigned int m_packetLength;
    unsigned int m_packetHeaderLength;

    unsigned char m_packetHeader[MAX_PACKET_HEADER_LENGTH];

    unsigned char m_headerBuffer[16];
    
    unsigned int  m_residueUnits;
    unsigned int  m_autoChapterDuration;

public:

    CGeneralIFO();

    void Init(void* pContext, int size, osal_mutex_t* pMutex, bool bCircular, unsigned int unitsInBuffer);
    void Attach(void* pContext, int size, osal_mutex_t* pMutex);

    static void context_destructor(void* context, int size);

    int GetPreambleLength();
    int GetContextSize() { return sizeof(IFO_CONTEXT); }

    //
    // Playback interface
    //

    unsigned int GetRecordPos();
    unsigned int GetMaxReadUnits(unsigned int readPos);
    unsigned int GetPlaybackElapsedSeconds(unsigned int presentPos);
    unsigned int GetRecordElapsedSeconds();
    void UpdateStateAndSegment(unsigned int readPos, unsigned int* pSegBegin, unsigned int* pSegEnd,
                               void (*callback)(void* instance, STATE_TYPE type, void* state, int bytes), void* pInstance);
    bool GotoSecond(int second, unsigned int* pNewReadPos);
    void GotoOldestPosition(unsigned int* pNewReadPos);
    bool GotoNewestPosition(unsigned int* pNewReadPos, bool bHead);
    void ResetPosition();
    bool GetTimePosition(unsigned int second, unsigned int* pPosition);
	bool GetState(int bytes, unsigned char* pState, unsigned int* pReturnedSize);
	
    //
    // Recording interface
    //

    void PrepareRecording();
    void SuspendRecording();
    void FinalizeRecording();
    void AddNewEntryToTMAP(bool bRecording = 1);
    void AddFutureEntryToTMAP(unsigned int diff);
    void AdvanceRecordPosition(unsigned int units);

    void GetRecordStatus(unsigned int* pElapsedTime,
                         unsigned int* pRecordedTime,
                         unsigned int* pRecordedBlocks,
                         unsigned int* pTotalBufferBlocks);

    void SetPreambleLength(unsigned int preambleLength);

	void AddNewState(STATE_TYPE type, void* state, int bytes);
	
	void BackupTMAP(unsigned int units);
    //
    // IFO Read/Write interface
    //

    void ConfigPacketizer(unsigned int   packetLength,       // (0) means no packetization is requested
                          unsigned int   packetHeaderLength,
                          unsigned char* pPacketHeader);     // the packet header pattern (must be fixed)

    void StartIfoRead(void* pContext, int size, osal_mutex_t* pMutex);
    void StartIfoWrite();
    bool SendBufferFromReading(unsigned int bytes, unsigned char* buffer, bool* pbCompleted);
    bool PrepareBufferForWriting(unsigned int bufferSize, unsigned char* buffer, unsigned int* pDataBytes);

	
		
    //
    // CGMS 
    //

    bool AddCGMSMark(CGMS_TYPE type);
    
    //
    // Chapter Editing Interfaces
    //

	bool InsertChapter(unsigned int time, unsigned int* pChapterIndex,
						unsigned int* pChapterLength);
						
	bool RemoveAllChapters ();
	bool RemoveChapter(unsigned int chapterIdx, unsigned int* pChapterTime,
						unsigned int* pChapterLength);
	
	int  QueryChapterTotalNumber ();
	bool QueryChapter(unsigned int chapterIdx, unsigned int* pChapterTime,
					  unsigned int* pChapterLength, bool* pIsHidden, 
					  unsigned int* pChapterStartPos, bool* pbSeamless, unsigned int* pChapterVOBUNum);

	bool ShowChapter(unsigned int chapterIdx, bool bShow);
	void ConfigureAutoChapter(unsigned int chapterDuration);
	unsigned int GetAutoChapterDuration();
	
	
	//
	// Edinting API
	//
	bool DivideTitle(unsigned int time, CGeneralIFO* pCGIFO);
	bool MergeTitle(CGeneralIFO* pCGIFO);
	bool ABCopy(CGeneralIFO* pCGIFO, unsigned int startTime, unsigned int endTime);
	bool ABCut(unsigned int startTime, unsigned int endTime);
		
	//
	// Dubbing API
	//
	int GetVOBUNum();
	unsigned int GetTitleSize();
	unsigned int GetTimeDurationPerVOBU();
	bool CheckCanDubbing();
	bool GetVOBUSize(unsigned int vobuIdx, unsigned int* pVOBUSize);
	
	//
	// for unit test
	//
	void DumpIFOContext();
	
	int GetUnitSizeEXP()	{ return m_pIFOContext->unitSizeExp; }
	
protected:

    //
    // Private Helpers
    //

 	
	bool QueryChapterImpl(int tableIdx, unsigned int* pChapterTime, unsigned int* pChapterLength,
						bool* pIsHidden, unsigned int* pChapterStartPos, 
						bool* pbSeamless, unsigned int* pChapterVOBUNum);

    void GotoSecondImpl(int second, unsigned int* pNewReadPos, 
						unsigned int* pCurrEntrySecond, unsigned int* pReadUnitsMax, 
						ENTRY_RANGE* pCurrEntry, ENTRY_RANGE* pReadEntry, 
						TMAP_INDEX* pIdxPlay, TMAP_INDEX* pIdxRead);
						
    void UpdateEntryRange(ENTRY_RANGE* pRange, TMAP_INDEX* pIndex);
    bool MoveIndex(TMAP_INDEX* pIndex, int steps);
    void RemoveUnusedStates();
    void CalculateSignature(unsigned char *signature);

    inline unsigned int GetTMAPEntryNum(TMAP_INDEX* pTMAPIdx);
    inline unsigned int GetRTV(unsigned int blk, unsigned int idx);
    inline void SetRTV(unsigned int blk, unsigned int idx, unsigned int unit, unsigned int value);
};


#endif /*__GENERAL_IFO_H__*/
