#ifndef __C_AVIFILE_AUTHOR_FILTER_H__
#define __C_AVIFILE_AUTHOR_FILTER_H__

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <Filters/CFileAuthoring.h>

#define FILEAUTH_AVI_IDXARRAY_SIZE  131072
// total AVI file size
#define FILE_AUTHORING_AVI_FILE_SIZE     0x100000   //2G
// Index file's max size
#define FILE_AUTHORING_AVI_INDEX_SIZE     0x1FFF    //15.99MB
// the number of chunks which will be processed each time
// when collect each 64k*2, write thread will write to disc 
// (write to disc once every 100sec ...maybe)
#define FILEAUTH_AVI_IDXARRAY_PROCESS_UNIT 4096

class CAVIFileDataOutput;

class CAVIFileAuthoring: public CFileAuthoring {
    friend class CAVIFileDataOutput;

public:
    CAVIFileAuthoring(int writePriority=0);
    ~CAVIFileAuthoring(void);
    
    HRESULT     Open(const char *path, bool isRing, uint64_t ringSize);
    HRESULT     Pause(FILTER_STATE State);    
    HRESULT     Close();
    // @NOTICE: Finalize have to called before Close();
    HRESULT     Finalize();
    HRESULT     CanRecord(bool *value);
	
protected:  
    HRESULT     ProcessPrivateInfo(BYTE *pInfo);


public:
    int                          m_aviIdxArray[FILEAUTH_AVI_IDXARRAY_SIZE];
    int                          m_aviIdxIndexNow;
    int                          m_aviIdxIndexHead;
    bool                         m_writeAviIdxTable;
    int                          m_aviIdxHandle;   // index file's file handle
    off64_t                      m_aviIdxFileOffset; // index file's file offset   
    int                          m_aviOffsetNow;  // the field which write in the index file
    int                          m_totalChunkSize;
    int                          m_finishIdxTable;  // 0: none, 1: receive EOS, 2: write idx table ok
    int                          m_totalVideoFrames;
    int                          m_totalAudioFrames;
    int                          m_maxChunkSize;

} ;

// END CLASS FILE_AUTHOR_FILTER


class CAVIFileDataOutput : public CFileDataOutput
{
public:
    CAVIFileDataOutput(unsigned int bufferSize);
    ~CAVIFileDataOutput(void);

protected:
    bool          myDataOutput(void);
    char *        m_IndexContentAddr;
};

#endif //__C_FILE_AUTHOR_FILTER_H__

