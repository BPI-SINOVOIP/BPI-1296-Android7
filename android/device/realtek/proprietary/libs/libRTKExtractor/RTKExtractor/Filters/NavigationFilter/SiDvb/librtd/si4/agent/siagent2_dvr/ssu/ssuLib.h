
/*=======================================================================
 * Copyright c        Realtek Semiconductor Corporation, 2007
 * All rights reserved.
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * Module   : SSU
 * File   : ssuLib.h
 *
 * Author   : palladio@realtek
 * Date   : 2007/05/02
 *
 *=======================================================================*/


#ifndef _SSULIB_H_
#define _SSULIB_H_

#include <system/si4/SITypes.h>
#include <system/si4/api/SI_Api.h>
#include "ssuDcr.h"
#include "ssuFunctionWrapper.h"



void SsuLib_InitSsuLibrary( SI*  pSiIf, UINT32 devOui,
                            UINT16 devHwModel, UINT16 devHwVersion,
                            UINT16 devSwModel, UINT16 devSwVersion,
                            CB_FP_CHECK_TARGET_DESC pCbfChkSmartCardDesc,
                            CB_FP_CHECK_TARGET_DESC pCbfChkMacAddrDesc,
                            CB_FP_CHECK_TARGET_DESC pCbfChkSerialNumDesc,
                            CB_FP_CHECK_TARGET_DESC pCbfChkIpAddrDesc,
                            CB_FP_CHECK_TARGET_DESC pCbfChkIpv6AddrDesc,
                            CB_FP_CHECK_TARGET_DESC pCbfChkPrivateDataDesc,
  				char* targetFilePath
	);

void SsuLib_DeInitSsuLibrary(SI*  pSiIf);

bool SsuLib_GetDtrInfo( SSU_DTR_INFO_P *ppGetDtrInfo );
bool SsuLib_GetSwLocation( UINT32 swPackIndex, SSU_SW_LOCATION_P *ppGetSwLocation );
bool SsuLib_GetSwUpdateReason( UINT32 swPackIndex, SW_UPDATE_REASON_P *ppGetSwUpdateReason );
bool SsuLib_GetEpSsuEventName( UINT32 swPackIndex, SSU_EVENT_NAME_P *ppGetSsuEventName );
bool SsuLib_GetEpSsuEventText( UINT32 swPackIndex, SSU_EVENT_TEXT_P *ppGetSsuEventText );
bool SsuLib_GetEpUpdateSwSchedule( UINT32 swPackIndex, SSU_SCHEDULE_P *ppGetUpdateSwSchedule );
bool SsuLib_GetEpUpdateSwInfo( UINT32 swPackIndex, SSU_UPDATE_SW_INFO_P *ppGetUpdateSwInfo );

#if 0
//[for target]
//memory(for siAgent2)
#define GET_MVD_MEMORY_SIZE (10*1024*1024)
void SsuLib_EnableMvdMemAccess( void );
void SsuLib_DisableMvdMemAccess( void );
void SsuLib_InitMvdMemHeapPartition( void );

//system(for AP)
void SsuLib_SystemReboot( void );
UINT32 SsuLib_GetSystemOui( void );
UINT16 SsuLib_GetSystemHwModelNum( void );
UINT16 SsuLib_GetSystemHwModelVer( void );
UINT16 SsuLib_GetSystemSwModelNum( void );
UINT16 SsuLib_GetSystemSwModelVer( void );

void SsuLib_OpenSwImage( void );
void SsuLib_CloseSwImage( void );
bool SsuLib_SwImageProgramFlash( UINT32 blockNum );
#endif//0

//flash program(for AP)
bool SsuLib_GetSwImageBlockNum( UINT32 *pGetBlockNum );//DDB number


#ifdef _SSULIB_C_
    #define _SSULIB_C_DECLARE_

    void SsuLib_InitAllSsuObjsMethod(void);

	ErrCode SsuLib_ProcessSsuSections(SI_DATABASE *pDatabase, unsigned int sectionLength,unsigned char** ppSectionRawBuffer, unsigned int pid);
	void SsuLib_RegSectionCollectorCBF(SI* si);

    #if 1

	#define  SSULIB_MALLOC(size)   SI_AGENT_MALLOC((size))

	#define  SSULIB_FREE(p)	SI_AGENT_FREE((p))
		
	#if 0	
	    #include <memCheck.h>

	    #define  SSULIB_MALLOC(size)   Memcheck_Alloc(size)

	    #define  SSULIB_FREE(p)        do{             \
	                                     Memcheck_Free(p) ;    \
	                                     (p) = NULL ; \
	                                    }while(0)
	#endif//0
    #else

    #define SSULIB_MALLOC(size)   malloc(size)

    #define SSULIB_FREE(p)        do{             \
                                     free(p) ;    \
                                     (p) = NULL ; \
                                    }while(0)

    #endif


    #define _DEBUG_SSU_LIB_

    #define DBG_MSG_LEVEL_INFO    0
    #define DBG_MSG_LEVEL_WARNING 1
    #define DBG_MSG_LEVEL_ERROR   2

    #ifdef _DEBUG_SSU_LIB_
           #define SSU_LIB_DBG_MSG(x, format, args...) { \
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
    #else  //_DEBUG_SSU_LIB_
           #define SSU_LIB_DBG_MSG(x, format, args...)
    #endif //_DEBUG_SSU_LIB_


#else //_SSULIB_C_
    #define _SSULIB_C_DECLARE_ extern

#endif //_SSULIB_C_




#endif //_SSULIB_H_



