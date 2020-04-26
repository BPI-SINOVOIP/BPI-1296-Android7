#ifndef __C_FILE_AUTHOR_FILTER_H__
#define __C_FILE_AUTHOR_FILTER_H__

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include <OSAL.h>
#include <CStream.h>
#include <Filters/CAVIMuxFilter.h>
#include <Filters/CDataOutput.h>
#include <Filters/GeneralIFO.h>
#include <Filters/NavigationFilter/NavDef.h>

#ifndef HAS_FLAG
 #define HAS_FLAG(flag, bit)    ((flag) & (bit))
 #define SET_FLAG(flag, bit)    ((flag)|= (bit))
 #define RESET_FLAG(flag, bit)  ((flag)&= (~(bit)))
#endif 

#define FILEAUTH_OUTPUT_FILENAME       (0x00000001)
#define FILEAUTH_OUTPUT_FILENAME_RING  (0x00000002)
#define FILEAUTH_OUTPUT_FILEHANDLE     (0x00000010)
#define FILEAUTH_OUTPUT_SOCKETHANDLE   (0x00000020)

#define FILE_AUTHORING_RESERVED_FREE_BLK 5120   //5120*2K
#define FILE_AUTHORING_MAX_DISC_ERROR 10


typedef enum _tag_fileauth_outtype {
    FILEAUTH_FILE_WAV = 0,
    FILEAUTH_FILE_MP3,
    FILEAUTH_FILE_MPG,
    FILEAUTH_FILE_UNKNOWN
} ENUM_FILEAUTH_OUTPUT_TYPE;


class CFileAuthoringDataInputPin;
class CFileAuthoringInfoInputPin;
class CFileDataOutput;

class CFileAuthoring: public CBaseFilter {
    friend class CFileAuthoringDataInputPin;
    friend class CFileAuthoringInfoInputPin;
    friend class CFileDataOutput;

public:
    CFileAuthoring();
    ~CFileAuthoring(void);

    HRESULT     Pause(FILTER_STATE State);
    HRESULT     Run(void);
    HRESULT     Stop();

    HRESULT     Open(const char *path, bool isList, bool isRing = false, uint64_t ringSize=0);
    HRESULT     Open(int fd, bool isSocket, bool isRing = false, uint64_t ringSize=0);
    HRESULT     Close(void);
    HRESULT     Finalize();
    HRESULT     EditMP3ID3Tag(char *title, char *artist, char *album, int *year, char *comment, char *genre);
    HRESULT     GetRecordStatus(NAVRECORDSTATUS* pStatus);

    HRESULT     CanRecord(bool *value);
    HRESULT     GetTitleLength(int *nHour, int *nMin, 
                               int *nSec, int *nFrame);
    void        DropAllData();    
    static HRESULT     Delete(const char *file_name, int *out_errorCode=NULL);
    static HRESULT     Rename(const char *old_file_name, const char *new_file_name, int *out_errorCode=NULL);
    static HRESULT	   Protect(const char *file_name, int on, int *out_errorCode=NULL);

protected:  
    virtual HRESULT     ProcessPrivateInfo(BYTE *pInfo);

protected:

    CFileAuthoringDataInputPin   *m_pDataInputPin;
    CFileAuthoringInfoInputPin   *m_pInfoInputPin;
    CFileDataOutput              *m_pOutput;

    CMemListAllocator	          *m_pAllocator;
    int                          m_destType;
    uint64_t                     m_ringSize;

    unsigned int                 m_writePriority;
    uint64_t                     m_totalDiscSpace;
    uint64_t                     m_totalDataSize;
    uint32_t                     m_recordTotalFields;
    int                          m_videoFormat;
    uint32_t                     m_recSecond;

    char                         m_filePath[1024];
    int                          m_fileHandle; 
    CGeneralIFO                  m_generalIFO;
    

    __off64_t                    m_fileOffset;
    ENUM_FILEAUTH_OUTPUT_TYPE    m_authType;    
} ;

// END CLASS FILE_AUTHOR_FILTER

class CFileAuthoringDataInputPin : public CBaseInputPin
{
    friend class CFileAuthorFilter;

public:
    CFileAuthoringDataInputPin(const char *pId);
    ~CFileAuthoringDataInputPin();

    HRESULT Receive(IMediaSample* pSample);

    HRESULT QueryAccept(const MEDIA_TYPE *pMediaType);
    HRESULT ReceiveConnection(IPin *pConnector, const MEDIA_TYPE *pmt);
    HRESULT PrivateInfo(BYTE *pInfo, int nLength);

    HRESULT EndOfStream(unsigned int eventID);
};

class CFileAuthoringInfoInputPin : public CBaseInputPin
{
    friend class CDVDAuthorFilter;

public:
    CFileAuthoringInfoInputPin(const char *pId);
    ~CFileAuthoringInfoInputPin();

    HRESULT QueryAccept(const MEDIA_TYPE *pMediaType)	{ return S_OK; }
    HRESULT PrivateInfo(BYTE *pInfo, int nLength);
};

class CFileDataOutput : public CDataOutput
{
public:
    CFileDataOutput(unsigned int bufferSize);
    ~CFileDataOutput(void);

    int           setInitOffset(uint64_t offset);
    void          setSockeWrite(bool flag);
    void          setDropDataFlag(bool isDrop);

protected:
    bool          myDataOutput(void);
    
    uint64_t      m_fileOffset;
    int           m_failCount;
    bool          m_socketWrite;
    bool          m_bDropData;    
};

#endif //__C_FILE_AUTHOR_FILTER_H__

