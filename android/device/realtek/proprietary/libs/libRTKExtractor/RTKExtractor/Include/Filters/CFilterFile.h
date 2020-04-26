#ifndef __C_FILTER_FILE_H__
#define __C_FILTER_FILE_H__

// #include "../StreamClass/IStream.h"
#include <StreamClass/CFilter.h>
#include <StreamClass/CMemAllocator.h>

class   IPin;
class   COutPinFileRead;
class   CInPinFileWrite;
class   CInPinAdd;
class   COutPinAdd;
class   CThread;

class CFilterFile: public CBaseFilter{
public:
                                CFilterFile(
                                    char*                   pFileName,
				    char*                   pFileMode
                                );
    virtual                     ~CFilterFile(void);
protected:
    FILE*                       m_pFileHandle;
    char*                       m_pFileName;
    char*                       m_pFileMode;
};

class CFilterFileRead: public CFilterFile{
friend class                    COutPinFileRead;
public:
                                CFilterFileRead(
                                    char*                   pFileName,
                                    char*                   pFileMode
                                );
    virtual                     ~CFilterFileRead(void);

    HRESULT                     Stop(void);
    HRESULT                     Pause(
                                    FILTER_STATE            State
                                );
    HRESULT                     Run(void);


protected:
    // re-write this function for user's purpose
    static void                 ThreadReadFile(
                                    void*                   pData
                                );
    virtual bool                ReadFromFile(void);

    virtual bool                RingReadFromFile(void) PURE;
    virtual bool                ListReadFromFile(void) PURE;

protected:   
    COutPinFileRead*            m_pOutPin;
    CThread*                    m_pThread;
};

class CFilterFileWrite: public CFilterFile{
friend class                    CInPinFileWrite;
public:
                                CFilterFileWrite(
                                    char*                   pFileName,
                                    char*                   pFileMode,
                                    int                     BufferSize
                                );
    virtual                     ~CFilterFileWrite(void);

    HRESULT                     Stop(void);
    HRESULT                     Pause(
                                    FILTER_STATE            State
                                );
    HRESULT                     Run(void);

    bool                        WriteFile(
                                    IMediaSample*           pIMSample
                                );
protected:
    virtual bool                ThreadWriteFile(void) PURE;

    static void                 FileWriteThread(
                                    void*                   pData
                                );
protected:
    CInPinFileWrite*            m_pInPin;
    CThread*                    m_pThread;
    IMemAllocator*              m_pAllocator;
    VoidArrSem                  m_DataQueue;
};

#endif
