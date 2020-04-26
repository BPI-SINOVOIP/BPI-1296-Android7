#ifndef __RT_MEDIACONFIG_H
#define __RT_MEDIACONFIG_H

#include <RT_MediaPlayClient/common.h>

#include <RT_IPC/RT_IPC.h>
#include <utils/Errors.h>

#if 0//defined(USE_DFB_SUPPORT) && defined(DFB_MULTI_APP)
#define MPCLIENT_ENABLE_FUSION
#endif

#ifdef MPCLIENT_ENABLE_FUSION
#include "Fusion/FusionCall.h"
#endif

namespace android {

class RT_MediaConfig
{
public:
	                     RT_MediaConfig();
    virtual             ~RT_MediaConfig();

	/***********************************************************
	* initCheck - setup a config connection to Server.
	***********************************************************/
    virtual status_t initCheck();
    
    /***********************************************************
	* setVORect - config display window.
	* @params - 
	* x [in] : 
	* y [in] : 
	* w [in] : width
	* h [in] : height
	* osdW [in] : 
	* osdH [in] : 
	* z [in] : video out layer. could be 1 or 2.
	* @ret - 
	***********************************************************/
    virtual status_t setVORect(int x, int y, int w, int h, int osdW, int osdH, int z = 1);
    
    /***********************************************************
	* deletePBInstance - delete playback instance
	* @params - 
	* name [in] : name of the playback instance going to be destroyed.
	*			  lenght of name should be 4.
	* @ret - 
	***********************************************************/
    virtual status_t deletePBInstance(const char* name);
    
    /***********************************************************
	* getPBInstance - get a named playback instance
	* @params - 
	* pbType [in] : playback flow type. 
	*				"PTAD" - audio only playback flow
	*      			"PTVD" - video only playback flow
	*      			"PTAV" - a general playback flow
	* name [in] : name of the playback instance.
	*			  lenght of name should be 4.
	* bsSize - bitstream buffer size. by default, 8M is used
	* cmdSize - command buffer size. by default, 256k is used
	* @ret - 
	***********************************************************/
    virtual status_t getPBInstance(const char* pbType, const char* name, long bsSize = RTCLIENT_BITSTREAM_BUF_SIZE, long cmdSize = RTCLIENT_COMMAND_BUF_SIZE);
    
    /***********************************************************
	* zoom - zoom video based on actual video size
	* @params - 
	* videoPlane [in] : video out layer. could 1 or 2.
	* zoomType [in] : 	0 - full size
	*			   		1 - original size
	* 				   	2 - double size
	* 				   if bigger than screen size, keep aspect ratio and fit the screen size. 					
	* @ret - 
	***********************************************************/
    virtual status_t zoom(long videoPlane, long zoomType);
    	    	
    /***********************************************************
	* execFunction - execute a script function
	* @params - 
	* pFunName [in] : name of script function.
	* pArgv [in] : a list of arguments. Each one is a string.
	* argc [in] : number of argments
	* retStrLen [out] : lenght of return string.
	* pRetStr [out] : a pointer to return string. 
	*				Caller should call releaseExecFuncResult() to de-allocate it.
	* @ret - 
	***********************************************************/
    virtual status_t execFunction(const char* pFunName, const char** pArgv, int argc, int& retStrLen, char*& pRetStr, bool bBySocket = true);
    
    /***********************************************************
	* releaseExecFuncResult - de-allocate the memory of the result of script function.
	* @params - 
	* pMemory [in] : a pointer to the memory to be de-allocated.
	* @ret - 
	***********************************************************/
	virtual status_t releaseExecFuncResult(char* pMemory);
	
	/***********************************************************
	* initFusionCheck - init linux fusion. 
	*					Be sure fusion is initialized before calling
	*					execFunction using linux fusion
	* @ret - 
	***********************************************************/
	virtual status_t initFusionCheck();
		
	virtual status_t memoryAllocate(int size,char **ppMem);
	
		
private:
	
    // copied from MediaPlayer class
    void xProcEntry(int checkedSeqNum  = -1);

    // copied from MediaPlayer class
    inline status_t xSendCmd(RT_CMD_ID cmdId, char* pInData, int inDataSize)
	{
		int seqNum;
		int retVal = m_ipc.sendCommand(cmdId, pInData, inDataSize, seqNum);
		if(retVal <= 0)
			return UNKNOWN_ERROR;

		// get result from server
		xProcEntry(seqNum);
		return NO_ERROR;
	}
	
private:
	
    RT_IPC			m_ipc;
    char* 			m_result;
    int				m_resultSize;
#ifndef NO_THREAD	
	pthread_mutex_t	 m_cmdLock;
#endif
#ifdef MPCLIENT_ENABLE_FUSION
	MPC_Fusion*		m_pFusion;
#endif	
};

}; //namespace android

#endif // __RT_MEDIACONFIG_H
