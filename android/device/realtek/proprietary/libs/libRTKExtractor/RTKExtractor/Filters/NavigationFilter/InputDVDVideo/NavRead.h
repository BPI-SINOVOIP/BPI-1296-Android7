#ifndef __DVD_VIDEO_NAV_READ_H__
#define __DVD_VIDEO_NAV_READ_H__

#include "common.h"
#include <Filters/CDVDVideoTS.h>

#define MAX_SECTORS_PER_READ 16
#define DVDV_SECTOR_SIZE     2048

class CDVDVideoReader
{
public:

    CDVDVideoReader() : m_pReader(NULL) {}
    virtual ~CDVDVideoReader() { Close(); }

    inline bool IsOpen() { return (m_pReader != NULL); }
    inline bool IsODD()  { return (IsOpen() && m_pReader->b_udf_fs_11 != 0); }

    // open reader for media access, init VMG and TS (time-shift info)
    HRESULT Open(char* path, osal_mutex_t* mutex, unsigned long* pPhyAddrOffset);

    // release reader, free VMG/VTS/VOB/TS
    HRESULT Close();

    vmg_t* LockVMG();         // Lock VMG
    void UnlockVMG();         // Unlock VMG
    vts_t* LockVTS(int vtsN); // Lock VTS
    void UnlockVTS();         // Unlock VTS
    ts_handle_t* LockTS();    // Lock TS
    void UnlockTS();          // Unlock TS

    void UnlockAll();

    // read VOB data blocks
    HRESULT ReadBlocks(unsigned char** ppBuffer, DVDVideoState* pState, uint32_t address, int blocks);

    HRESULT ReadMediaSignature(unsigned char *signature);

    int ReadCurrCGMS(int64_t offset);
    int QueryCGMS();


protected:

    HRESULT InitReader(char* path); // open reader or share the instance exposed by authoring
    HRESULT InitVMG();              // load VMG or share the instance exposed by authoring
    HRESULT InitVTS(int vtsN);      // load VTS or share the instance exposed by authoring
    HRESULT InitTS();               // create TS or share the instance exposed by authoring
    HRESULT LoadVMG();              // load VMG from media
    HRESULT LoadVTS(int vtsN);      // load VTS from media

    dvd_info_t* m_pReader;
    CDVDVideoTS   m_tsManager;

    // parameters for VMG/VTS/TS sharing/locking
    vmg_t*        m_pVMG;
    vts_t*        m_pVTS;
    ts_handle_t*  m_pTS;
    osal_mutex_t* m_pVMGLock;
    osal_mutex_t* m_pVTSLock;
    osal_mutex_t* m_pTSLock;
    osal_mutex_t* m_pMutex;
    int           m_InitedVTSN; // remember which VTS IFO is currently inited and exposed
    int           m_lockStatus; // remember whether VMG/VTS/TS lock is held

    enum { LOCKED_VMG = 0x00000001,
           LOCKED_VTS = 0x00000002,
           LOCKED_TS  = 0x00000004  };

    // parameters for VOB reading
    dvd_file_t*    m_pVOBFile;
    DVDVideoDomain m_currDomain;
    int            m_currVTSN;
    uint32_t       m_currAddress;
    int            m_currBlocks;

#if defined(USE_IOCACHE)
    dvd_file_t*    m_pCacheVOBFile;
    int            m_bReadCache;
    int            m_CacheBufferNumber;
    __off64_t            m_CacheOffset;
    int            m_CacheBufferSize;
    unsigned char* m_pCacheBuf;
#elif defined(VOB_CACHE_DATA)
    //
#else
    unsigned char* m_pReadBuffer;
#endif
};

#endif //__DVD_VIDEO_NAV_READ_H__
