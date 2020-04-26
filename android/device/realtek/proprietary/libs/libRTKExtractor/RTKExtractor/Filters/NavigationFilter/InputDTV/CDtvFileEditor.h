#ifndef __C_DTV_FILE_EDITOR_H__
#define __C_DTV_FILE_EDITOR_H__

#include <Filters/NavigationFilter/NavPlugins.h>
#include <Filters/GeneralIFO.h>
#include "InfoHeader.h"
#include <io/GeneralFileAccess/file_access.h>
class CDtvFileEditor
{
public:
	CDtvFileEditor()
		: m_ioInternalHandle(0), m_pSrcPathName(0), m_pBuffer(0)
	{
		//printf("[%s %d] m_pBuffer = %p, m_pSrcPathName = %p\n", __FILE__, __LINE__, m_pBuffer, m_pSrcPathName);
	}
		
	~CDtvFileEditor();
	
	void SetPathName(char* path);
			
	bool LoadIFOContext(IOPLUGIN* pIOPlugin, void* fh, unsigned char* pBuffer, int size, CGeneralIFO* pCIFO, void* pIfoContext, osal_mutex_t* pIfoMutex, ENUM_GFILE_TYPE* pIOType);		
		
	
	// query byte offset corresponding to the elapsed second
    HRESULT GetTimePosition(unsigned int second, int64_t* pByteOffset);

	
    HRESULT ABFastCopy(char* pDesPathName, unsigned int startSec, unsigned int endSec);
    HRESULT StoreIFO(int ioType, CGeneralIFO* pGeneralIFO, int pFH, unsigned char* pBuffer, unsigned int size);
		
protected :
	    
	HRESULT GetTargetFileIFO(void* fh, CGeneralIFO* pTargetGIFO, osal_mutex_t* pMutex, ENUM_GFILE_TYPE* pIOType, bool bCreateIFOIfNeeded = 1);
	
	int	 OpenTargetFile(char* pDesPathName, CGeneralIFO* pTargetGIFO, osal_mutex_t* pMutex, ENUM_GFILE_TYPE* pIOType);
    void PreparePreamble(unsigned char* pBuffer, unsigned int size, IFO_MAIN_HEADER* pHdr);
	
	    
protected:
	
	CGeneralIFO        m_generalIFO;
    
  	int			   	   m_ioInternalHandle;
  	ENUM_GFILE_TYPE	   m_ioType; 
  	char*			   m_pSrcPathName;
  	unsigned char*     m_pBuffer; 
  	
};

#endif /*__C_DTV_FILE_EDITOR_H__*/
