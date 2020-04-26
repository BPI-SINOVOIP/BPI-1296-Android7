
/*=======================================================================
 * Copyright c        Realtek Semiconductor Corporation, 2007
 * All rights reserved.
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * Module   : Multi-Instance siAgent2 (DVR version)
 * File   : siAgentInterface.h
 *
 * Author   : palladio@realtek
 * Date   : 2007/05/22
 *
 *=======================================================================*/


#ifndef _SIAGENTINTERFACE_H_
#define _SIAGENTINTERFACE_H_

//#include <rtd_types.h>
#include "siAgentStateMachine.h"
//#include "SITypes_local.h"



SIAGENT_OBJECT_P SiAgtIF_CreateSiAgentObject( char*          pAgentObjectName,//max. 15 chars + '\0'
                                              SI_HANDLE    si,
                                              SIDB_HANDLE    handleSidb,
                                              DEMUXER_HANDLE handleDemuxer );

void SiAgtIF_DeleteSiAgentObject( SIAGENT_OBJECT_P pSiAgentObj );



BOOL SiAgtIF_ScanChannel( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsNode );
BOOL SiAgtIF_ChangeChannel( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsNode, unsigned short programNumber );

BOOL SiAgtIF_ScanProgram( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsNode );
BOOL SiAgtIF_RecordProgram( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsNode, unsigned short programNumber );

#ifdef SI_DVB_ENABLE_SSU
BOOL SiAgtIF_ScanSSU( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsNode);
BOOL SiAgtIF_DownloadSSU( SIAGENT_OBJECT_P pSiAgentObj,  TABLE_TREE_TS_LAYER_P tsNode, SSU_SW_LOCATION_T *swInfo);
#endif//SI_DVB_ENABLE_SSU



SIDB_HANDLE SiAgtIF_GetSidbHandle( SIAGENT_OBJECT_P pSiAgentObj );
unsigned int      SiAgtIF_GetSiAgtObjId( SIAGENT_OBJECT_P pSiAgentObj );
void        SiAgtIF_Run( SIAGENT_OBJECT_P pSiAgentObj );
void        SiAgtIF_CheckAllSmIdle( SIAGENT_OBJECT_P pSiAgentObj, BOOL *pResult );
BOOL        SiAgtIF_StopAllTask( SIAGENT_OBJECT_P pSiAgentObj );
BOOL        SiAgtIF_RegisterCBF( SIAGENT_OBJECT_P pSiAgentObj, SI_AGENT_CBF cbf, SI_AGENT_CBF_TYPE type );

#ifdef REMOVE_GLOBAL_VARIABLE
void SiAgtIF_TpModelMutexInit(SIAGENT_OBJECT_P pSiAgentObj);
void SiAgtIF_TpModelMutexDestroy(SIAGENT_OBJECT_P pSiAgentObj);
void SiAgtIF_SiAgentTaskMutexInit(SIAGENT_OBJECT_P pSiAgentObj);
void SiAgtIF_SiAgentTaskMutexDestroy(SIAGENT_OBJECT_P pSiAgentObj);
#else /* else of REMOVE_GLOBAL_VARIABLE */
void SiAgtIF_TpModelMutexInit(void);
void SiAgtIF_TpModelMutexDestroy(void);
void SiAgtIF_SiAgentTaskMutexInit(void);
void SiAgtIF_SiAgentTaskMutexDestroy(void);
#endif /* end of REMOVE_GLOBAL_VARIABLE */

TABLE_TREE_TS_LAYER_P SiAgtIF_CreateTsNode( SIAGENT_OBJECT_P pSiAgentObj, unsigned int  frequency );
void SiAgtIF_SetCurrentTSNode( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );

int SiAgtIF_kbhit(void);

BOOL SiAgtIF_CheckPatExist( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );
PROGRAMS_INFO_P SiAgtIF_GetProgramsInfo( SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr );



#if 1
       #define SAIF_MALLOC(size)     SI_AGENT_MALLOC(size)
       #define SAIF_FREE(p) 		SI_AGENT_FREE((p))
    #else
       #include <memCheck.h>
       #define SAIF_MALLOC(size)     malloc(size)
       #define SAIF_FREE(p)          do{             \
                                        free(p) ;    \
                                        (p) = NULL ; \
                                       }while(0)
#endif


#ifdef _SIAGENTINTERFACE_C_
    #define _SIAGENTINTERFACE_C_DECLARE_



    #define _DEBUG_SIAGENT_INTERFACE_

    #define DBG_MSG_LEVEL_INFO    0
    #define DBG_MSG_LEVEL_WARNING 1
    #define DBG_MSG_LEVEL_ERROR   2

    #if 0 //def _DEBUG_SIAGENT_INTERFACE_
           #define SIAGENT_INTERFACE_DBG_MSG(x, format, args...) { \
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
    #else  //_DEBUG_SIAGENT_INTERFACE_
           #define SIAGENT_INTERFACE_DBG_MSG(x, format, args...)
    #endif //_DEBUG_SIAGENT_INTERFACE_


#else //_SIAGENTINTERFACE_C_
    #define _SIAGENTINTERFACE_C_DECLARE_ extern

#endif //_SIAGENTINTERFACE_C_




#endif //_SIAGENTINTERFACE_H_













