
/*=======================================================================
 * Copyright c        Realtek Semiconductor Corporation, 2007
 * All rights reserved.
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * Module   : SSU
 * File   : ssuGroup.h
 *
 * Author   : palladio@realtek
 * Date   : 2007/05/29
 *
 *=======================================================================*/


#ifndef _SSUGROUP_H_
#define _SSUGROUP_H_




#include "ssuModule.h"
#include "ssuDii.h"

struct SSU_GROUP_ ;
typedef struct SSU_GROUP_       SSU_GROUP_T, *SSU_GROUP_P ;


struct SSU_GROUP_METHOD_ {

};
typedef struct SSU_GROUP_METHOD_       SSU_GROUP_METHOD_T, *SSU_GROUP_METHOD_P ;

struct SSU_GROUP_ {
 SSU_GROUP_METHOD_P method ;
 UINT32  groupSize ;//sum of modules' size(from DII, not from DSI's group info)
 UINT32  moduleNum ;
 UINT32  totalBlockNum ;
 UINT32  receivedBlockNum ;
 UINT32  checkedBlockNum ;
 UINT32  receivedDdbCountForTimeoutDetect ;
 bool    flagPassCheck ;
 SSU_MODULE_P modules[0];
};



//SSU GROUP METHODS


SSU_GROUP_P SsuGp_CreateSsuGpObj( SSU_DII_MODULE_INFO_P pSsuDiiModuleInfo );
void SsuGp_DeleteSsuGpObj( SSU_GROUP_P *ppSsuGroup );


#ifdef _SSUGROUP_C_
    #define _SSUGROUP_C_DECLARE_

   #if 1

	#define  SSUGROUP_MALLOC(size)   SI_AGENT_MALLOC((size))

	#define  SSUGROUP_FREE(p)	SI_AGENT_FREE((p))
		
	#if 0	
	    #include <memCheck.h>

	    #define  SSUGROUP_MALLOC(size)   Memcheck_Alloc(size)

	    #define  SSUGROUP_FREE(p)        do{             \
	                                     Memcheck_Free(p) ;    \
	                                     (p) = NULL ; \
	                                    }while(0)
	#endif//0
    #else

    #define SSUGROUP_MALLOC(size)   malloc(size)

    #define SSUGROUP_FREE(p)        do{             \
                                     free(p) ;    \
                                     (p) = NULL ; \
                                    }while(0)

    #endif


    #define _DEBUG_SSU_GROUP_

    #define DBG_MSG_LEVEL_INFO    0
    #define DBG_MSG_LEVEL_WARNING 1
    #define DBG_MSG_LEVEL_ERROR   2

    #ifdef _DEBUG_SSU_GROUP_
           #define SSU_GROUP_DBG_MSG(x, format, args...) { \
                    switch(x) { \
                    case DBG_MSG_LEVEL_INFO   : printf("\033[1;32m%s() \033[m", __FUNCTION__); \
                        printf("[INFO][%4d]: ", __LINE__); break; \
                    case DBG_MSG_LEVEL_WARNING: printf("\033[1;33m%s() \033[m", __FUNCTION__); \
                        printf("[WARNING][%4d]: ", __LINE__); break; \
                    case DBG_MSG_LEVEL_ERROR  : printf("\033[1;31m%s() \033[m", __FUNCTION__); \
                        printf("[ERROR][%4d]: ", __LINE__); break; \
                    default: break; } \
                    printf(format, ##args); \
                    printf("\n"); \
                   }
    #else  //_DEBUG_SSU_GROUP_
           #define SSU_GROUP_DBG_MSG(x, format, args...)
    #endif //_DEBUG_SSU_GROUP_


#else //_SSUGROUP_C_
    #define _SSUGROUP_C_DECLARE_ extern

#endif //_SSUGROUP_C_


void SsuGp_InitGpMethod(void);
_SSUGROUP_C_DECLARE_ SSU_GROUP_METHOD_T ssuGroupMethod ;


#endif //_SSUGROUP_H_



