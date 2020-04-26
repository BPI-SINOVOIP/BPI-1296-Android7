
/*=======================================================================
 * Copyright c        Realtek Semiconductor Corporation, 2007
 * All rights reserved.
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * Module   : SSU
 * File   : ssuModule.h
 *
 * Author   : palladio@realtek
 * Date   : 2007/05/29
 *
 *=======================================================================*/


#ifndef _SSUMODULE_H_
#define _SSUMODULE_H_




#include "ssuBlock.h"
#include "ssuFunctionWrapper.h"


struct SSU_MODULE_ ;
typedef struct SSU_MODULE_       SSU_MODULE_T, *SSU_MODULE_P ;


struct SSU_MODULE_METHOD_ {

};
typedef struct SSU_MODULE_METHOD_       SSU_MODULE_METHOD_T, *SSU_MODULE_METHOD_P ;

struct SSU_MODULE_ {
 SSU_MODULE_METHOD_P method ;
 UINT16      moduleId ;
 UINT32      moduleSize ;
 UINT32      blockNum ;
 UINT32      receivedBlockNum ;
 bool        flagHaveModuleCrc ;
 UINT32      checkedBlockNum ;
 bool        flagPassCheck ;
 UINT16      blockSize;
 SSU_BLOCK_P blocks[0];
};


//SSU MODULE METHODS


SSU_MODULE_P SsuModule_CreateSsuModuleObj( UINT16 moduleId, UINT32 moduleSize, UINT16 blockSize, bool flagHaveModuleCrc );


#ifdef _SSUMODULE_C_
    #define _SSUMODULE_C_DECLARE_

    #if 1

	#define  SSUMODULE_MALLOC(size)   SI_AGENT_MALLOC((size))

	#define  SSUMODULE_FREE(p)	SI_AGENT_FREE((p))
		
	#if 0	
	    #include <memCheck.h>

	    #define  SSUMODULE_MALLOC(size)   Memcheck_Alloc(size)

	    #define  SSUMODULE_FREE(p)        do{             \
	                                     Memcheck_Free(p) ;    \
	                                     (p) = NULL ; \
	                                    }while(0)
	#endif//0
    #else

    #define SSUMODULE_MALLOC(size)   malloc(size)

    #define SSUMODULE_FREE(p)        do{             \
                                     free(p) ;    \
                                     (p) = NULL ; \
                                    }while(0)

    #endif


    #define _DEBUG_SSU_MODULE_

    #define DBG_MSG_LEVEL_INFO    0
    #define DBG_MSG_LEVEL_WARNING 1
    #define DBG_MSG_LEVEL_ERROR   2

    #ifdef _DEBUG_SSU_MODULE_
           #define SSU_MODULE_DBG_MSG(x, format, args...) { \
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
    #else  //_DEBUG_SSU_MODULE_
           #define SSU_MODULE_DBG_MSG(x, format, args...)
    #endif //_DEBUG_SSU_MODULE_


#else //_SSUMODULE_C_
    #define _SSUMODULE_C_DECLARE_ extern

#endif //_SSUMODULE_C_

void SsuModule_InitModuleMethod(void);
_SSUMODULE_C_DECLARE_ SSU_MODULE_METHOD_T ssuModuleMethod ;


#endif //_SSUMODULE_H_



