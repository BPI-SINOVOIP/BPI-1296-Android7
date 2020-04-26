#include "fcgi_stdio.h"	/* fcgi library; put it fist */
#undef LOG_TAG
#define LOG_TAG "remote.fcgi"
#include <utils/Log.h>

#include <stdlib.h>
#include <string.h>

#include <sys/time.h>
#include <unistd.h>
#include <linux/input.h>

#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>

#include "remote.h"
#include "response.h"

static void write_key_event(int fdRemote, int keycode, int downOrUp)
/* downOrUp: 1 -> down, 0 -> up */
{
    struct input_event event_init, event, event_end;
    memset(&event_init, 0, sizeof(event_init));
    memset(&event, 0, sizeof(event));
    memset(&event_end, 0, sizeof(event_end));
    gettimeofday(&event_init.time, NULL);
    event_init.type = EV_MSC;
    event_init.code = MSC_SCAN;
    event_init.value = 500000;
    gettimeofday(&event.time, NULL);
    event.type = EV_KEY;
    event.code = keycode;
    event.value = downOrUp;
    gettimeofday(&event_end.time, NULL);
    event_end.type = EV_SYN;
    event_end.code = SYN_REPORT;
    event_end.value = 0;
    //ALOGD("before write evt_code:%d, pressed:%d.\n", event.code, downOrUp);
    write(fdRemote, &event_init, sizeof(event_init)); // init 
    write(fdRemote, &event, sizeof(event));// key event
    write(fdRemote, &event_end, sizeof(event_end));// Show
    //sleep(1);// wait
    ALOGD("after write evt_code:%d, pressed:%d.\n", event.code, downOrUp);
}

int handleRemoteKey(int fdRemote)
{
	char *cmd_type = NULL;
	int keycode;

	CGI_RESULT_T* pCgiRet = (CGI_RESULT_T*) malloc (sizeof(CGI_RESULT_T));	
	memset(pCgiRet, 0, sizeof(CGI_RESULT_T));

	pCgiRet->type = CGI_RESULT_REMOTE;	
    pCgiRet->RemoteResult.mResponseType = RESPONSE_TYPE_REMOTE;
	
	do
	{	
		if (fdRemote == -1) 
		{
			pCgiRet->RemoteResult.mRet = CGI_FAIL;
			snprintf(pCgiRet->RemoteResult.mErrStr, MAX_SIZE_ERROR_STR, "Permission not allowed");
			break;
		}
		
		if (cgi_get_svalue("command", &cmd_type) != CGI_OK)
		{
			pCgiRet->RemoteResult.mRet = CGI_FAIL;
			snprintf(pCgiRet->RemoteResult.mErrStr,MAX_SIZE_ERROR_STR, "Invalid command");
			break;
		}
				
		keycode = cmd_string2id(cmd_type, tbl_CommandSet, SIZEOF_ARRAY(tbl_CommandSet));
		ALOGD("trans cmd_type:%s to key_code=%d\n", cmd_type, keycode);
		if (keycode == -1)
		{
			pCgiRet->RemoteResult.mRet = CGI_FAIL;
			snprintf(pCgiRet->RemoteResult.mErrStr, MAX_SIZE_ERROR_STR, "Not support keycode: %d", keycode);
			break;
		}		

		write_key_event(fdRemote, keycode, 1);	// pressed
		write_key_event(fdRemote, keycode, 0);	// released

		// success
		pCgiRet->RemoteResult.mRet = CGI_OK;
		
	} while(0);

	dumpCgiResult(pCgiRet); 	

	SAFE_FREE(pCgiRet);	
	SAFE_FREE(cmd_type);
	
	return CGI_OK;		
}


int main(void)
{  
	int fdRemote;	// file handle of remote key

	fdRemote = open(DEV_REMOTE, O_RDWR);
	if (fdRemote == -1) {
		printf("Error open mouse:%s\n", strerror(errno));
		return CGI_FAIL;
	}
    
    /* Response loop */
    while ( FCGI_Accept() >= 0) 
    {		
    	if (cgi_init() != CGI_OK) 
    		goto error_handler;		    	
		
		switch (cgi_get_method())
		{			
			case METHOD_POST:
			{
				// not supported
			}
			case METHOD_GET:			
			default:
			{
				if (cgi_get_id() == ID_REMOTE)
				{
					handleRemoteKey(fdRemote);
				}
			}
			break;
		}
				
error_handler:
		cgi_deinit();
				
	} /* while FCGI_Accept */          
		
	close(fdRemote);
	
	/* End of Response */
	return 0;
}

