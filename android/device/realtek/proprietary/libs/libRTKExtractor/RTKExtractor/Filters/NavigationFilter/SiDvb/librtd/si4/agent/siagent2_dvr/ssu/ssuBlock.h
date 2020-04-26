
/*=======================================================================
 * Copyright c        Realtek Semiconductor Corporation, 2007
 * All rights reserved.
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * Module   : SSU
 * File   : ssuBlock.h
 *
 * Author   : palladio@realtek
 * Date   : 2007/05/29
 *
 *=======================================================================*/


#ifndef _SSUBLOCK_H_
#define _SSUBLOCK_H_



#include <string.h>
#include "ssuFunctionWrapper.h"

//#include <library/pmalloc/pmalloc.h> //for alloc large amount of memory



struct SSU_BLOCK_ ;
typedef struct SSU_BLOCK_       SSU_BLOCK_T, *SSU_BLOCK_P ;


struct SSU_BLOCK_ {
 UINT32 blockLen ;
 UINT8  blockData[0];
};

//SSU BLOCK METHODS

SSU_BLOCK_P SsuBlk_CreateSsuBlkObj(UINT8* secRawData, INT32 secRawDataLen, 
			UINT16 blockIdx, UINT16 blockSize, FILE* fp );


#ifdef _SSUBLOCK_C_
    #define _SSUBLOCK_C_DECLARE_

    #if 1

	#define  SSUBLOCK_MALLOC(size)   SI_AGENT_MALLOC((size))

	#define  SSUBLOCK_FREE(p)	SI_AGENT_FREE((p))
		
	#if 0	
	    #include <memCheck.h>

	    #define  SSUBLOCK_MALLOC(size)   Memcheck_Alloc(size)

	    #define  SSUBLOCK_FREE(p)        do{             \
	                                     Memcheck_Free(p) ;    \
	                                     (p) = NULL ; \
	                                    }while(0)
	#endif//0
    #else


      #ifdef _RUN_ON_TP_MODEL_
        #define SSUBLOCK_MALLOC(size)   malloc(size)

        #define SSUBLOCK_FREE(p)        do{             \
                                         free(p) ;    \
                                         (p) = NULL ; \
                                        }while(0)
      #else //_RUN_ON_TP_MODEL_
        #define SSUBLOCK_MALLOC(size)   pmalloc( mvdMemHeapPartitionHandle, size)

        #define SSUBLOCK_FREE(p)        do{             \
                                         pfree( mvdMemHeapPartitionHandle, p) ;    \
                                         (p) = NULL ; \
                                        }while(0)
      #endif //_RUN_ON_TP_MODEL_


    #endif


    #define _DEBUG_SSU_BLOCK_

    #define DBG_MSG_LEVEL_INFO    0
    #define DBG_MSG_LEVEL_WARNING 1
    #define DBG_MSG_LEVEL_ERROR   2

    #ifdef _DEBUG_SSU_BLOCK_

	#define SSU_BLOCK_DBG_MSG(x, format, args...) { \
			printf("[BLK]" format, ##args); \
			printf("\n"); \
		}
 	#if 0
           #define SSU_BLOCK_DBG_MSG(x, format, args...) { \
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
	#endif//0
    #else  //_DEBUG_SSU_BLOCK_
           #define SSU_BLOCK_DBG_MSG(x, format, args...)
    #endif //_DEBUG_SSU_BLOCK_


#else //_SSUBLOCK_C_
    #define _SSUBLOCK_C_DECLARE_ extern

#endif //_SSUBLOCK_C_


//_SSUBLOCK_C_DECLARE_ bool flagMvdMemoryReady ;
//_SSUBLOCK_C_DECLARE_ void *pMvdMemoryAddr ;
_SSUBLOCK_C_DECLARE_ void *mvdMemHeapPartitionHandle ;

#endif //_SSUBLOCK_H_



