#ifndef __C_DTV_FILE_WRITER_H__
#define __C_DTV_FILE_WRITER_H__

#include <Filters/NavigationFilter/NavDef.h>
#include <Filters/GeneralIFO.h>

#include "CDtvFileEditor.h"
#ifdef ENABLE_DTV_ASYNC_RECORD
#include <UTIL/ulQueue.h>
#include <pthread.h>
#include <unistd.h>

typedef struct
{
	const unsigned char* pWriteBuf;
	int writeSize;
} DTV_WRITE_CMD_T;
typedef ulCircularQueue<DTV_WRITE_CMD_T> WriteCmdQueue;
#endif

struct IFO_MAIN_HEADER;

class CDtvFileWriter : public CDtvFileEditor
{
	friend class CInputDTV;

protected:

    osal_mutex_t* m_pNavMutex;
    
    int64_t       m_recClock;
    int64_t		  m_autoChapterClock;
    bool          m_bOwner;
    bool          m_bCircular;
    unsigned int  m_volumeFreeBlocks;
    unsigned int  m_writeErrorCount;
	int			  m_deliverDataSize;

#ifdef ENABLE_DTV_ASYNC_RECORD	
	pthread_t m_tid;
	osal_sem_t m_writeSem;
	volatile int m_bWriteQuit;
	WriteCmdQueue m_writeCmdQueue;
#endif	
public:

    enum DTV_FILE_WRITE_RESULT
    {
        WRITE_OK,
        WRITE_FAIL_VOLUME_FULL,
        WRITE_FAIL_IO_ERROR
    };

    CDtvFileWriter(osal_mutex_t* pNavMutex) 
    : m_pNavMutex(pNavMutex)
#ifdef ENABLE_DTV_ASYNC_RECORD    
    , m_writeCmdQueue(16, true)
#endif     
    {}

    // open file for recording and create or attach-to shared information (return true if succeeded, false if failed)
    bool Open(char* path, int handle, bool bCircular, int64_t maxFileSize, bool bLinkingList, unsigned char* pBuffer, unsigned int size);

    // close file when we're done with recording
    void Close(unsigned char* pBuffer, unsigned int size);

    // write (append) a block of media data to the recording file
    DTV_FILE_WRITE_RESULT Write(unsigned char* pData, unsigned int bytes);

    // get recording status
    HRESULT GetRecordStatus(NAVRECORDSTATUS* pStatus);

    // add new SI-State object which becomes effective from the current recording position
    void AddNewSiState(unsigned char* pStateData, unsigned int stateBytes);
    

	HRESULT BackupTMAP(unsigned int units);
	    

#ifdef ENABLE_DTV_ASYNC_RECORD	    
	//added by baili_feng
	HRESULT WriteAsync(unsigned char* pData, unsigned int bytes);

	static void* WriteThread(void* arg);
#endif	
};


#endif /*__C_DTV_FILE_WRITER_H__*/
