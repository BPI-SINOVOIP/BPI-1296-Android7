#ifndef RT_CGI_H
#define RT_CGI_H

#include "libcgi.h"

/***** Version Info ****/
#define CGI_MAJOR_VER	0
#define CGI_MINOR_VER	1
/***** Version Info ****/

typedef struct _RT_CGI_MEM_VARIABLES_ {
	
	int fdRemote;	// file handle of remote key, Not used
		
} RtCgiMemVariables;


int init(RtCgiMemVariables *pData);
int deInit(RtCgiMemVariables *pData);
int handleHttpPostRequest(RtCgiMemVariables *pData);
int handleHttpGetRequest(RtCgiMemVariables *pData);



#endif // RT_CGI_H
