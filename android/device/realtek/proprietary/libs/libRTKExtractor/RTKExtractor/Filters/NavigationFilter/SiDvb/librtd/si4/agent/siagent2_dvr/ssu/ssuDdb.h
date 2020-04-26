
/*=======================================================================
 * Copyright c        Realtek Semiconductor Corporation, 2007
 * All rights reserved.
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * Module   : SSU
 * File   : ssuDdb.h
 *
 * Author   : palladio@realtek
 * Date   : 2007/05/29
 *
 *=======================================================================*/


#ifndef _SSUDDB_H_
#define _SSUDDB_H_



#include <string.h>
#include "ssuFunctionWrapper.h"

struct SSU_DDB_ ;
typedef struct SSU_DDB_       SSU_DDB_T, *SSU_DDB_P ;


struct SSU_DDB_METHOD_ {
 UINT8  (*GetTableID)(SSU_DDB_P) ;
 UINT8  (*GetTableVersion)(SSU_DDB_P) ;
 UINT32 (*GetSectionLength)(SSU_DDB_P) ;
 UINT32 (*GetSectionCrc)(SSU_DDB_P) ;
 UINT32 (*GetDownloadId)(SSU_DDB_P) ;
 UINT16 (*GetModuleId)(SSU_DDB_P) ;
 UINT16 (*GetBlockNumber)(SSU_DDB_P) ;
};
typedef struct SSU_DDB_METHOD_       SSU_DDB_METHOD_T, *SSU_DDB_METHOD_P ;

struct SSU_DDB_ {
 SSU_DDB_METHOD_P method ;
 UINT32 tableRawDataLen ;
 UINT8  tableRawData[0] ;
};


//SSU DDB METHODS
UINT8  SsuDdb_GetTableID(SSU_DDB_P ssuDdb) ;
UINT8  SsuDdb_GetTableVersion(SSU_DDB_P ssuDdb) ;
UINT32 SsuDdb_GetSectionLength(SSU_DDB_P ssuDdb) ;
UINT32 SsuDdb_GetSectionCrc(SSU_DDB_P ssuDdb) ;
UINT32 SsuDdb_GetDownloadId(SSU_DDB_P ssuDdb) ;
UINT16 SsuDdb_GetModuleId(SSU_DDB_P ssuDdb) ;
UINT16 SsuDdb_GetBlockNumber(SSU_DDB_P ssuDdb) ;


SSU_DDB_P SsuDdb_CreateSsuDdbObj( UINT8* secRawData, INT32 secRawDataLen );
bool SsuDdb_CheckSsuDdbSyntax( UINT8* secRawData, INT32 secRawDataLen );
UINT32 SsuDdb_GetSecRawDataDownloadId(UINT8* secRawData) ;
UINT8 SsuDdb_GetSecRawDataModuleIdMsb(UINT8* secRawData) ;
UINT16 SsuDdb_GetSecRawDataModuleId(UINT8* secRawData) ;
UINT16 SsuDdb_GetSecRawDataBlockNumber(UINT8* secRawData) ;


#ifdef _SSUDDB_C_
    #define _SSUDDB_C_DECLARE_

    #if 1

	#define  SSUDDB_MALLOC(size)   SI_AGENT_MALLOC((size))

	#define  SSUDDB_FREE(p)	SI_AGENT_FREE((p))
		
	#if 0	
	    #include <memCheck.h>

	    #define  SSUDDB_MALLOC(size)   Memcheck_Alloc(size)

	    #define  SSUDDB_FREE(p)        do{             \
	                                     Memcheck_Free(p) ;    \
	                                     (p) = NULL ; \
	                                    }while(0)
	#endif//0

    #else

    #define SSUDDB_MALLOC(size)   malloc(size)

    #define SSUDDB_FREE(p)        do{             \
                                     free(p) ;    \
                                     (p) = NULL ; \
                                    }while(0)

    #endif


    #define _DEBUG_SSU_DDB_

    #define DBG_MSG_LEVEL_INFO    0
    #define DBG_MSG_LEVEL_WARNING 1
    #define DBG_MSG_LEVEL_ERROR   2

    #ifdef _DEBUG_SSU_DDB_
           #define SSU_DDB_DBG_MSG(x, format, args...) { \
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
    #else  //_DEBUG_SSU_DDB_
           #define SSU_DDB_DBG_MSG(x, format, args...)
    #endif //_DEBUG_SSU_DDB_


#else //_SSUDDB_C_
    #define _SSUDDB_C_DECLARE_ extern

#endif //_SSUDDB_C_

void SsuDdb_InitDdbMethod(void);
_SSUDDB_C_DECLARE_ SSU_DDB_METHOD_T ssuDdbMethod ;

#endif //_SSUDDB_H_



