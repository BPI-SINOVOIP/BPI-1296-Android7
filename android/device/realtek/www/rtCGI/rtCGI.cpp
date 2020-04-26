#include "fcgi_stdio.h"	/* fcgi library; put it fist */
#include <stdlib.h>
#include <dirent.h>
#include <string.h>


#ifndef ANDROID 
	#include <sys/statvfs.h> 
	#include <sys/sem.h>
#else 
	#include <linux/sem.h> 
	#include <sys/vfs.h> 
	#define statvfs statfs
#endif 

#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>

#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>

#include <Application/AppClass/SetupClassAndroid.h>

#include "libcgi.h"
#include "response.h"
#include "remote.h"	// for remote control
#include "info.h"	// for query info
#include "setting.h"// for set/get setting item
#include "util.h"	// for utility
#include "fs.h"		// for file system /db
#ifdef USE_TOGO_MANAGER
#include "togo.h"	// for togo
#endif
#include "wifi.h"	// for wifi related
#include "rtCGI.h"

using namespace std;

/***********************************************************************
 **  CGI entry.
 **
 **  The CGI service "API" is defined as follows:
 **
 **  Input: [URL] http://......?id=xxx&action=xxx&...
 **			  	 id --> the main CGI function such as HDD,TimerRecord
 **			     action --> the subfunctions correspond to the "id"
 **				 ... --> other parameters maybe used correspond to action
 **
 **  Output: Response should always follow the JSON format
 **
 **
 **
 ************************************************************************/

int init(RtCgiMemVariables *pData)
{
	SetupClass::SetInstance(new SetupClassAndroid());

	return CGI_OK;	
}
 
int deInit(RtCgiMemVariables *pData)
{	
	return CGI_OK;
}

int handleHttpPostRequest(RtCgiMemVariables *pData)
{
    int id, retVal = CGI_OK;

	if ((id = cgi_get_id()) < 0)
		return CGI_FAIL;
		
	switch (id)
	{
		default:
			retVal = CGI_FAIL;			
	}
			
	return CGI_OK;
}

int handleHttpGetRequest(RtCgiMemVariables *pData)
{
    int id, retVal = CGI_OK;

	if ((id = cgi_get_id()) < 0)
		return CGI_FAIL;

	switch (id)
	{
		case ID_REMOTE:
		{					
			CGI_RESULT_T* pCgiRet = (CGI_RESULT_T*) malloc (sizeof(CGI_RESULT_T));
			memset(pCgiRet, 0, sizeof(CGI_RESULT_T));
			
			responseGeneralResult(pCgiRet, RESPONSE_TYPE_REMOTE, CGI_FAIL, "Please use http://[IP]/remote instead");
			dumpCgiResult(pCgiRet);
			// Move to remote.cgi
			retVal = CGI_FAIL;
		}
			break;
		case ID_QUERY_INFO:
		{
			retVal = handleQueryInfo();
		}
			break;
		case ID_GETSET_SETTING_ITEM:
		{
			retVal = handleGetSetSettingItem();
		}
			break;
		case ID_FSDB_REQUEST:
		{
			retVal = handleFSDBRequst();
		}
			break;
		case ID_INPUT_STRING:
		{
			//retVal = handleInputString();
		}
			break;
#ifdef USE_TOGO_MANAGER
		case ID_TOGO:
		{
			retVal = handleTogo();
		}
			break;
#endif
		case ID_WIFI:
		{
			retVal = handleWifi();
		}
			break;
		default:
			retVal = CGI_FAIL;			
	}
	
	return retVal;
}

int main(void)
{  
	RtCgiMemVariables data;

    /* Initilize */
    if (init(&data) == CGI_FAIL)
    	return -1;
    
    /* Response loop */
    while ( FCGI_Accept() >= 0) 
    {		
    	if (cgi_init() != CGI_OK) 
    		goto error_handler;		    	
		
		switch (cgi_get_method())
		{
			case METHOD_POST:
			{
				handleHttpPostRequest(&data);
			}
			break;
			
			case METHOD_GET:			
			default:
			{
				handleHttpGetRequest(&data);
			}
			break;
		}
				
error_handler:
		cgi_deinit();
				
	} /* while FCGI_Accept */          
	
	deInit(&data);
	/* End of Response */
	return 0;
}
