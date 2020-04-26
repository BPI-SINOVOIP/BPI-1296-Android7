
/*=======================================================================
 * Copyright c        Realtek Semiconductor Corporation, 2007
 * All rights reserved.
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * Module   : SSU
 * File   : ssuDii.h
 *
 * Author   : palladio@realtek
 * Date   : 2007/05/29
 *
 *=======================================================================*/


#ifndef _SSUDII_H_
#define _SSUDII_H_



#include <string.h>
#include "ssuFunctionWrapper.h"


struct SSU_DII_ ;
typedef struct SSU_DII_       SSU_DII_T, *SSU_DII_P ;

/*
struct DESCRIPTOR_OBJECT_ {
 INT32      dataLen;
 UINT8      data[0];
};
typedef struct DESCRIPTOR_OBJECT_       DESCRIPTOR_OBJECT_T, *DESCRIPTOR_OBJECT_P;

struct DESCRIPTOR_INFO_ {
 INT32        num;
 UINT8        tag[0];
};
typedef struct DESCRIPTOR_INFO_     DESCRIPTOR_INFO_T, *DESCRIPTOR_INFO_P;
*/

//DII Query Struct

struct SSU_DII_MODULE_RECORD_ {
 UINT16 moduleId ;
 UINT32 moduleSize ;
 bool flagHaveModuleCrc ;
};
typedef struct SSU_DII_MODULE_RECORD_       SSU_DII_MODULE_RECORD_T, *SSU_DII_MODULE_RECORD_P ;

struct SSU_DII_MODULE_INFO_ {
 UINT32 downloadId ;
 UINT16 blockSize ;
 UINT32 moduleNumber ;
 SSU_DII_MODULE_RECORD_T moduleRecords[0] ;
};
typedef struct SSU_DII_MODULE_INFO_       SSU_DII_MODULE_INFO_T, *SSU_DII_MODULE_INFO_P ;

//DII Object Struct

struct SSU_DII_METHOD_ {
 UINT8  (*GetTableID)(SSU_DII_P) ;
 UINT8  (*GetTableVersion)(SSU_DII_P) ;
 UINT32 (*GetSectionLength)(SSU_DII_P) ;
 UINT32 (*GetSectionCrc)(SSU_DII_P) ;
 UINT32 (*GetDownloadId)(SSU_DII_P) ;
 UINT8  (*GetModuleIdMsb)(SSU_DII_P) ;
 UINT32 (*GetTransactionId)(SSU_DII_P) ;
 UINT32 (*GetSsuModuleNumber)(SSU_DII_P) ;
 SSU_DII_MODULE_INFO_P (*GetModuleInfo)(SSU_DII_P);
 UINT16 (*GetBlockSize)(SSU_DII_P) ;
 DESCRIPTOR_INFO_P (*GetModuleInfoDescInfo)( SSU_DII_P, UINT32 ) ;
 DESCRIPTOR_OBJECT_P (*GetModuleInfoDesc)( SSU_DII_P, UINT32, UINT32 ) ;
};
typedef struct SSU_DII_METHOD_       SSU_DII_METHOD_T, *SSU_DII_METHOD_P ;

struct SSU_DII_ {
 SSU_DII_METHOD_P method ;
 UINT32 tableRawDataLen ;
 UINT8  tableRawData[0] ;
};


//SSU DII METHODS
UINT8  SsuDii_GetTableID(SSU_DII_P ssuDii) ;
UINT8  SsuDii_GetTableVersion(SSU_DII_P ssuDii) ;
UINT32 SsuDii_GetSectionLength(SSU_DII_P ssuDii) ;
UINT32 SsuDii_GetSectionCrc(SSU_DII_P ssuDii) ;
UINT32 SsuDii_GetDownloadId(SSU_DII_P ssuDii) ;
UINT8  SsuDii_GetModuleIdMsb(SSU_DII_P ssuDii) ;
UINT32 SsuDii_GetTransactionId(SSU_DII_P ssuDii) ;
UINT32 SsuDii_GetSsuModuleNumber(SSU_DII_P ssuDii) ;
SSU_DII_MODULE_INFO_P SsuDii_GetModuleInfo(SSU_DII_P ssuDii) ;
UINT16 SsuDii_GetBlockSize(SSU_DII_P ssuDii) ;
DESCRIPTOR_INFO_P SsuDii_GetModuleInfoDescInfo(SSU_DII_P ssuDii, UINT32 moduleNum) ;
DESCRIPTOR_OBJECT_P SsuDii_GetModuleInfoDesc(SSU_DII_P ssuDii, UINT32 moduleNum, UINT32 descNum) ;

SSU_DII_P SsuDii_CreateSsuDiiObj( UINT8* secRawData, INT32 secRawDataLen );
bool SsuDii_CheckSsuDiiSyntax( UINT8* secRawData, INT32 secRawDataLen );
UINT32 SsuDii_GetSecRawDataTransactionId(UINT8* secRawData) ;
UINT32 SsuDii_GetSecRawDataCrc(UINT8* secRawData) ;
UINT8  SsuDii_GetSecRawDataSecVer(UINT8* secRawData) ;


#ifdef _SSUDII_C_
    #define _SSUDII_C_DECLARE_

    #if 1

	#define SSUDII_MALLOC(size)   SI_AGENT_MALLOC((size))

	#define SSUDII_FREE(p)	SI_AGENT_FREE((p))
		
	#if 0	
	    #include <memCheck.h>

	    #define SSUDII_MALLOC(size)   Memcheck_Alloc(size)

	    #define SSUDII_FREE(p)        do{             \
	                                     Memcheck_Free(p) ;    \
	                                     (p) = NULL ; \
	                                    }while(0)
	#endif//0
    #else

    #define SSUDII_MALLOC(size)   malloc(size)

    #define SSUDII_FREE(p)        do{             \
                                     free(p) ;    \
                                     (p) = NULL ; \
                                    }while(0)

    #endif


    #define _DEBUG_SSU_DII_

    #define DBG_MSG_LEVEL_INFO    0
    #define DBG_MSG_LEVEL_WARNING 1
    #define DBG_MSG_LEVEL_ERROR   2

    #ifdef _DEBUG_SSU_DII_
           #define SSU_DII_DBG_MSG(x, format, args...) { \
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
    #else  //_DEBUG_SSU_DII_
           #define SSU_DII_DBG_MSG(x, format, args...)
    #endif //_DEBUG_SSU_DII_


#else //_SSUDII_C_
    #define _SSUDII_C_DECLARE_ extern

#endif //_SSUDII_C_

void SsuDii_InitDiiMethod(void);
_SSUDII_C_DECLARE_ SSU_DII_METHOD_T ssuDiiMethod ;


#endif //_SSUDII_H_



