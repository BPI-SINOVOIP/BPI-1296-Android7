
#ifndef	__RealtekSysHeaders_h
#define __RealtekSysHeaders_h

//#define NDEBUG
//#include "SystemHeaders_CPP_Includes.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <OSAL.h>




#include "CommonTypes.h"
#include "CommonSTLTypes.h"
#include "SystemHeaders_sal.h"

#undef NULL
#define NULL 0


#ifndef __mipsel__
#define __mipsel__
#endif

#define RENDERER_EXPORT


#define TickCount 			pli_getPTS
#define SetNameForDebugger 	pli_setThreadName
#define Sleep				osal_Sleep

#define REALTEK_DELETE_INSTANCE(_ptr)	if(_ptr){ delete _ptr; _ptr = 0;}
	

#define   MAKELPARAM(low, high)   ((DWORD)(((WORD)(low)) | (((DWORD)((WORD)(high))) << 16)))     
    

#endif

