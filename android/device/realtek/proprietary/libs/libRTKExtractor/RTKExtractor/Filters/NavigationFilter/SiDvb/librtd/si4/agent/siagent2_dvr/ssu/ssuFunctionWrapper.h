
/*=======================================================================
 * Copyright c        Realtek Semiconductor Corporation, 2007
 * All rights reserved.
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * Module   : SSU
 * File   : ssuFunctionWrapper.h
 *
 * Author   : palladio@realtek
 * Date   : 2007/04/24
 *
 *=======================================================================*/


#ifndef _SSUFUNCTIONWRAPPER_H_
#define _SSUFUNCTIONWRAPPER_H_


#define SSUDCR_INIT_ERROR_FILE_WRITE		(2)
#define SSUDCR_INIT_ERROR_FILE_FLOW 		(3)
#define SSUDCR_INIT_OK						(0)


//#include <library/sidb2/siInterface/siInterfaceHeader.h>
//#include "siAgentObject.h"
//#include "../siFunctionWrapper.h"
#include "../siAgentTypes.h"
#include <agent/SI_AgentMacros.h>
#include <system/CFramework/CFramework.h> //for SI_AGENT_MALLOC

//#define true TRUE
//#define false FALSE

#ifndef false
#define false 0
#endif

#ifndef true
#define true 1
#endif


#define TIMEOUT_SEC_SSU_DTR_UNT 60 /* [SSU spec] DVB-T:60sec, DVB-C:10sec, DVB-S:10sec */
#define TIMEOUT_SEC_SSU_DTR_DSI 60 /* [SSU spec] SSU DSI 5 sec */
#define TIMEOUT_SEC_SSU_DTR_DII 60 /* [SSU spec] SSU DSI 5 sec */
#define TIMEOUT_SEC_SSU_DTR_DDB  5 /* unknown */
#define TIMEOUT_SEC_SSU_DCR_DSI 60 /* [SSU spec] SSU DSI 5 sec */
#define TIMEOUT_SEC_SSU_DCR_DII 60 /* [SSU spec] SSU DSI 5 sec */
#define TIMEOUT_SEC_SSU_DCR_DDB 60 /* unknown */



#ifndef SI_TYPE_TS_LAYER
//#define SI_TYPE_TS_LAYER
//typedef	SI_DATABASE_TS_NODE* TABLE_TREE_TS_LAYER_P;
//typedef 	SIAGENT_OBJECT_*	SIAGENT_OBJECT_P;
#endif



/*
struct SI_IF_ES_INFO_ {
 UINT8        streamType;
 UINT16       esPid;
};
typedef struct SI_IF_ES_INFO_       SI_IF_ES_INFO_T, *SI_IF_ES_INFO_P;

struct SI_IF_ES_S_INFO_ {
 INT32              infoNum;
 SI_IF_ES_INFO_T    info[0];
};
typedef struct SI_IF_ES_S_INFO_       SI_IF_ES_S_INFO_T, *SI_IF_ES_S_INFO_P;

struct SI_IF_DESC_DATA_ {
 INT32      dataLen;
 UINT8      data[0];
};
typedef struct SI_IF_DESC_DATA_       SI_IF_DESC_DATA_T, *SI_IF_DESC_DATA_P;

struct SI_IF_DESC_S_INFO_ {
 INT32        num;
 UINT8        tag[0];
};
typedef struct SI_IF_DESC_S_INFO_     SI_IF_DESC_S_INFO_T, *SI_IF_DESC_S_INFO_P;

typedef void *(*SI_SECTIONCOLLECTOR_CBF)(UINT16, UINT8 *, INT32);

typedef enum {
 SI_SECTIONCOLLECTOR_CBF_TYPE_INSERT_SSU_SECTION,
 SI_SECTIONCOLLECTOR_CBF_TYPE_NUM__
} SI_SECTIONCOLLECTOR_CBF_TYPE ;
*/

//kjw: from siInterface.h
typedef struct SI_IF_PROGRAM_INFO_			SI_IF_PROGRAM_INFO_T, *SI_IF_PROGRAM_INFO_P ;
typedef struct SI_IF_PROGRAM_S_INFO_		SI_IF_PROGRAM_S_INFO_T, *SI_IF_PROGRAM_S_INFO_P ;
//typedef struct SI_IF_ES_INFO_				SI_IF_ES_INFO_T, *SI_IF_ES_INFO_P;
//typedef struct SI_IF_ES_S_INFO_				SI_IF_ES_S_INFO_T, *SI_IF_ES_S_INFO_P;
typedef struct SI_IF_DESC_S_INFO_			SI_IF_DESC_S_INFO_T, *SI_IF_DESC_S_INFO_P;
typedef struct SI_IF_DESC_DATA_				SI_IF_DESC_DATA_T, *SI_IF_DESC_DATA_P;


struct SI_IF_PROGRAM_INFO_
{
	UINT16								programNumber;
	UINT16    							pid;
};

struct SI_IF_PROGRAM_S_INFO_
{
	UINT32								programInfoNumber;
	SI_IF_PROGRAM_INFO_T				programInfo[0];
};



struct SI_IF_DESC_S_INFO_
{
	INT32								num;
	UINT8								tag[0];
};

struct SI_IF_DESC_DATA_
{
	INT32								dataLen;
	UINT8								data[0];
};


//end of from siInterface.h

#define PMT_ES_RECORD_T      SI_IF_ES_INFO_T
#define PMT_ES_RECORD_P      SI_IF_ES_INFO_P


#define DESCRIPTOR_OBJECT_T  SI_IF_DESC_DATA_T
#define DESCRIPTOR_OBJECT_P  SI_IF_DESC_DATA_P

#define DESCRIPTOR_INFO_T    SI_IF_DESC_S_INFO_T
#define DESCRIPTOR_INFO_P    SI_IF_DESC_S_INFO_P
/*
struct PMT_ES_RECORD_ {
 UINT8        streamType;
 UINT16       esPid;
};
typedef struct PMT_ES_RECORD_       PMT_ES_RECORD_T, *PMT_ES_RECORD_P;

struct PMT_ES_INFO_ {
 INT32              infoNum;
 PMT_ES_RECORD_T    info[0];
};
typedef struct PMT_ES_INFO_       PMT_ES_INFO_T, *PMT_ES_INFO_P;

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


#if 0
PMT_ES_INFO_P       SsuFW_GetPmtEsInfo(SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, UINT16 programNumber);
DESCRIPTOR_OBJECT_P SsuFW_GetPmtLocalDesc(SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, UINT16 programNumber, INT32 esNumber, INT32 descNumber);
//DESCRIPTOR_OBJECT_P SsuFW_GetPmtGlobalDesc(SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, UINT16 programNumber, INT32 descNumber);
DESCRIPTOR_INFO_P   SsuFW_GetPmtLocalDescInfo(SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, UINT16 programNumber, INT32 esNumber);
DESCRIPTOR_INFO_P   SsuFW_GetPmtGlobalDescInfo(SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, UINT16 programNumber);
//void                SsuFW_RegSectionCollectorCBF(SIAGENT_OBJECT_P pSiAgentObj, SI_Collector_SectionCallback cbf, SI_SECTIONCOLLECTOR_CBF_TYPE type );
#endif//0
SSU_ES_INFO_P SsuFW_GetDataBroadCastIDInfo(SIAGENT_OBJECT_P pSiAgentObj, TABLE_TREE_TS_LAYER_P tsAddr, UINT16 programNumber );
void SsuFW_FreeDataBroadCastIDInfo(SIAGENT_OBJECT_P pSiAgentObj, SSU_ES_INFO_T **ppEsInfo);

unsigned long SsuFW_CheckSectionCrc(unsigned char *data, int len);




#ifdef _SSUFUNCTIONWRAPPER_C_
    #define _SSUFUNCTIONWRAPPER_C_DECLARE_



    #define _DEBUG_FUNCTION_WRAPPER_

    #define DBG_MSG_LEVEL_INFO    0
    #define DBG_MSG_LEVEL_WARNING 1
    #define DBG_MSG_LEVEL_ERROR   2

    #ifdef _DEBUG_FUNCTION_WRAPPER_
           #define FUNC_WRAPPER_DBG_MSG(x, format, args...) { \
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
    #else  //_DEBUG_FUNCTION_WRAPPER_
           #define FUNC_WRAPPER_DBG_MSG(x, format, args...)
    #endif //_DEBUG_FUNCTION_WRAPPER_


#else //_SSUFUNCTIONWRAPPER_C_
    #define _SSUFUNCTIONWRAPPER_C_DECLARE_ extern

#endif //_SSUFUNCTIONWRAPPER_C_






#endif //_SSUFUNCTIONWRAPPER_H_



