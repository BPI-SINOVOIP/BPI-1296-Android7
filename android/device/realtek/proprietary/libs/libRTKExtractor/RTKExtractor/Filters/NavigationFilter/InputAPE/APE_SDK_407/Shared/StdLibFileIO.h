//#ifdef IO_USE_STD_LIB_FILE_IO

#ifndef APE_STDLIBFILEIO_H
#define APE_STDLIBFILEIO_H

#include "IO.h"
//typedef const char * LPCTSTR;
typedef const wchar_t * LPCTSTR;
#define MAX_FULL_PATH 1024

class CStdLibFileIO : public CIO
{
public:

    // construction / destruction
    CStdLibFileIO();
    ~CStdLibFileIO();

    // open / close
    int Open(LPCTSTR pName);
    int Open(LPCTSTR pName, BOOL bDirectIO);
    int Close();
    
    // read / write
    int Read(void * pBuffer, unsigned int nBytesToRead, unsigned int * pBytesRead);
    int Write(const void * pBuffer, unsigned int nBytesToWrite, unsigned int * pBytesWritten);
    
    // seek
    int Seek(int nDistance, unsigned int nMoveMode);
    
    // other functions
    int SetEOF();

    // creation / destruction
    //int Create(const char * pName);
    int Create(LPCTSTR pName);
    int Delete();

    // attributes
    int GetPosition();
    int GetSize();
    //int GetName(char * pBuffer);
    int GetName(wchar_t * pBuffer);
    int GetHandle();

private:
    
    char m_cFileName[MAX_FULL_PATH];
    BOOL m_bReadOnly;
    FILE * m_pFile;
	BOOL m_bDirectIO;
};

#endif // #ifndef APE_STDLIBFILEIO_H

//#endif // #ifdef IO_USE_STD_LIB_FILE_IO

