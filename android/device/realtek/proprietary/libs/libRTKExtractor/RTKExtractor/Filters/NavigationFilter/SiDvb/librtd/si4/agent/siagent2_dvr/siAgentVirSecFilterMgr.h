
/*=======================================================================
 * Copyright c        Realtek Semiconductor Corporation, 2007
 * All rights reserved.
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * Module   : Multi-Instance siAgent2 (DVR version)
 * File   : siAgentVirSecFilterMgr.h
 *
 * Author   : palladio@realtek
 * Date   : 2007/05/11
 *
 *=======================================================================*/


#ifndef _SIAGENTVIRSECFILTERMGR_H_
#define _SIAGENTVIRSECFILTERMGR_H_

//don't remove this line
#define _EXPORT_SECTION_FILTER_MODULE

//#include <rtd_types.h>
#include <stdlib.h>
#include <stdio.h>
#include <system/si4/SITypes.h>



	typedef enum SEC_FILTER_OWNER_
	{
		SF_OWNER_NONE = 0x00,
		SF_OWNER_AGENT = 0x00000001,
		SF_OWNER_GEMSTAR = 0x00000002,
		SF_OWNER_CABOT = 0x00000003,
		//SF_OWNER_xx = 0x00000004,

	} SEC_FILTER_OWNER;

	#define VSFM_FILTER_HAS_OWNER(owner, bitfields) (((owner) & (bitfields)) == (bitfields))
	#define VSFM_FILTER_SET_OWNER(owner, bitfields)  ((owner) |= (bitfields) )

#define MAX_SF_HANDLE_COUNT 32
#define MAX_SF_MATCH_COUNT 32 //#of section filters a section can match at most

typedef struct _SF_HANDLE_PAIR {
	void* p; 				//pointer to SEC_FILTER_LAYER_P (SF_HANDLE)
	//unsigned int hwHandle; 	//h/w handle
} SF_HANDLE_PAIR;

typedef struct _SF_HANDLE_POOL {
	int count;
	SF_HANDLE_PAIR pair[MAX_SF_HANDLE_COUNT];  //allow at most MAX_SF_HANDLE_COUNT handles
} SF_HANDLE_POOL;

/**
	section filter interface
*/
typedef int (*SI_SF_CBF)(unsigned int sectionLeng, unsigned char* secBuf, unsigned short pid);

#define MAX_SF_PATTERN_SIZE 7
typedef struct _SecFilterPattern {
	unsigned short pid;
	unsigned char tid;
	unsigned char tidMask;		//
	//unsigned char* tails;
	unsigned char patSize;	//byte[0] is first byte after sectionLeng
	//it maybe programNum(sid),tsid,...
	unsigned char pat[MAX_SF_PATTERN_SIZE];
	//positive mask(bit set to 0 means don't care, 1:must match )
	unsigned char patPosMask[MAX_SF_PATTERN_SIZE];
} SecFilterPattern;


//struct SIAGENT_OBJECT_ ;
//real definition is in siAgentObject.h
typedef struct SIAGENT_OBJECT_ *P_SIAGENT_OBJECT;
//#define SIAGENT_OBJECT_P (struct SIAGENT_OBJECT_*)


void SiAgent_Enable_GetTableFilter( P_SIAGENT_OBJECT pSiAgentObj, unsigned short pid, unsigned char tid );
void SiAgent_Disable_GetTableFilter( P_SIAGENT_OBJECT pSiAgentObj, unsigned short pid, unsigned char tid );

void SiAgent_Enable_MonitorPatFilter( P_SIAGENT_OBJECT pSiAgentObj, unsigned short pid, unsigned char tid, unsigned short tsid, unsigned char version );
void SiAgent_Disable_MonitorPatFilter( P_SIAGENT_OBJECT pSiAgentObj, unsigned short pid, unsigned char tid, unsigned short tsid, unsigned char version );

void SiAgent_Enable_GetPmtFilter( P_SIAGENT_OBJECT pSiAgentObj, unsigned short pid, unsigned char tid, unsigned short programNumber );
void SiAgent_Disable_GetPmtFilter( P_SIAGENT_OBJECT pSiAgentObj, unsigned short pid, unsigned char tid, unsigned short programNumber );

void SiAgent_Enable_MonitorPmtFilter( P_SIAGENT_OBJECT pSiAgentObj, unsigned short pid, unsigned char tid, unsigned short programNumber, unsigned char version );
void SiAgent_Disable_MonitorPmtFilter( P_SIAGENT_OBJECT pSiAgentObj, unsigned short pid, unsigned char tid, unsigned short programNumber, unsigned char version );

void SiAgent_Enable_MonitorTableFilter( P_SIAGENT_OBJECT pSiAgentObj, unsigned short pid, unsigned char tid, unsigned char version );
void SiAgent_Disable_MonitorTableFilter( P_SIAGENT_OBJECT pSiAgentObj, unsigned short pid, unsigned char tid, unsigned char version );

void SiAgent_Enable_GetApfEitFilter( P_SIAGENT_OBJECT pSiAgentObj, unsigned short pid, unsigned char tid, unsigned short serviceId, unsigned char sectionNumber );
void SiAgent_Disable_GetApfEitFilter( P_SIAGENT_OBJECT pSiAgentObj, unsigned short pid, unsigned char tid, unsigned short serviceId, unsigned char sectionNumber );

void SiAgent_Enable_MonitorApfEitFilter( P_SIAGENT_OBJECT pSiAgentObj, unsigned short pid, unsigned char tid, unsigned short serviceId, unsigned char sectionNumber, unsigned char version );
void SiAgent_Disable_MonitorApfEitFilter( P_SIAGENT_OBJECT pSiAgentObj, unsigned short pid, unsigned char tid, unsigned short serviceId, unsigned char sectionNumber, unsigned char version );


#ifdef ENABLE_SYSTEM_SOFTWARE_UPDATE
void SiAgent_Enable_GetDxiFilter(P_SIAGENT_OBJECT pSiAgentObj,  unsigned short pid, unsigned char tid, unsigned short id );
void SiAgent_Disable_GetDxiFilter(P_SIAGENT_OBJECT pSiAgentObj,  unsigned short pid, unsigned char tid, unsigned short id );
void SiAgent_Enable_MonitorDxiFilter(P_SIAGENT_OBJECT pSiAgentObj,  unsigned short pid, unsigned char tid, unsigned short id, unsigned char versionFlag );
void SiAgent_Disable_MonitorDxiFilter(P_SIAGENT_OBJECT pSiAgentObj,  unsigned short pid, unsigned char tid, unsigned short id, unsigned char versionFlag );
void SiAgent_Enable_GetSsuDdbFilter(P_SIAGENT_OBJECT pSiAgentObj,  unsigned short pid, unsigned char tid, unsigned char moduleIdMsb );
void SiAgent_Disable_GetSsuDdbFilter(P_SIAGENT_OBJECT pSiAgentObj,  unsigned short pid, unsigned char tid, unsigned char moduleIdMsb );
#endif//ENABLE_SYSTEM_SOFTWARE_UPDATE



/////////////////////   VSFM   ///////////////////////////

#if 1
typedef enum {
	  SEC_FLTR_FUNC_UNKNOWN          = 0,
	  SEC_FLTR_FUNC_GET_TABLE,
	  SEC_FLTR_FUNC_MONITOR_PAT,
	  SEC_FLTR_FUNC_GET_PMT,
	  SEC_FLTR_FUNC_MONITOR_PMT,
	  SEC_FLTR_FUNC_MONITOR_TABLE,
	  SEC_FLTR_FUNC_GET_APF_EIT,
	  SEC_FLTR_FUNC_MONITOR_APF_EIT,

	SEC_FLTR_FUNC_GET_DXI,
	SEC_FLTR_FUNC_MONITOR_DXI,
	SEC_FLTR_FUNC_GET_SSU_DDB,
	  
	  //kjw: si agent's default filter function value must smaller than SEC_FLTR_FUNC_8Byte_FILTER
	  //I sue this to speed up match -> ref VSFM_GetSectionFilterOwner() 
	SEC_FLTR_FUNC_8Byte_FILTER,
	SEC_FLTR_FUNC_CABOT_FILTER,
	SEC_FLTR_TOTAL_NUM,
} SEC_FILTER_FUNCTION ;

#else  //org code
typedef enum {
	  SEC_FLTR_FUNC_UNKNOWN          = 0,
	  SEC_FLTR_FUNC_GET_TABLE        = 1,
	  SEC_FLTR_FUNC_MONITOR_PAT      = 2,
	  SEC_FLTR_FUNC_GET_PMT          = 3,
	  SEC_FLTR_FUNC_MONITOR_PMT      = 4,
	  SEC_FLTR_FUNC_MONITOR_TABLE    = 5,
	  SEC_FLTR_FUNC_GET_APF_EIT      = 6,
	  SEC_FLTR_FUNC_MONITOR_APF_EIT  = 7,

	SEC_FLTR_FUNC_GET_DXI 	   = 8,
	SEC_FLTR_FUNC_MONITOR_DXI    = 9,
	SEC_FLTR_FUNC_GET_SSU_DDB    = 10,
	  
	  //kjw: si agent's default filter function value must smaller than SEC_FLTR_FUNC_8Byte_FILTER
	  //I sue this to speed up match -> ref VSFM_GetSectionFilterOwner() 
	SEC_FLTR_FUNC_8Byte_FILTER  = 11,
	SEC_FLTR_FUNC_CABOT_FILTER = 12,

	SEC_FLTR_TOTAL_NUM,
} SEC_FILTER_FUNCTION ;
#endif//

	struct ARG_GET_TABLE_ {
	  unsigned short  pid :13 ;
	  unsigned char   tid     ;
	  unsigned char   secFilterIndex ; //for remove section filter
	};
	typedef struct ARG_GET_TABLE_        ARG_GET_TABLE_T ;
	struct ARG_MONITOR_PAT_ {
	  unsigned short  pid :13 ;
	  unsigned char   tid     ;
	  unsigned short  tsid    ;
	  unsigned char   version ;
	  unsigned char   secFilterIndex ;
	};
	typedef struct ARG_MONITOR_PAT_        ARG_MONITOR_PAT_T ;
	struct ARG_GET_PMT_ {
	  unsigned short  pid :13 ;
	  unsigned char   tid     ;
	  unsigned short  programNumber ;
	  unsigned char   secFilterIndex ;
	};
	typedef struct ARG_GET_PMT_        ARG_GET_PMT_T ;
	struct ARG_MONITOR_PMT_ {
	  unsigned short  pid :13 ;
	  unsigned char   tid     ;
	  unsigned short  programNumber ;
	  unsigned char   version ;
	  unsigned char   secFilterIndex ;
	};
	typedef struct ARG_MONITOR_PMT_        ARG_MONITOR_PMT_T ;
	struct ARG_MONITOR_TABLE_ {
	  unsigned short  pid :13 ;
	  unsigned char   tid     ;
	  unsigned char   version ;
	  unsigned char   secFilterIndex ;
	};
	typedef struct ARG_MONITOR_TABLE_        ARG_MONITOR_TABLE_T ;
	struct ARG_GET_APF_EIT_ {
	  unsigned short  pid :13 ;
	  unsigned char   tid     ;
	  unsigned short  serviceId ;
	  unsigned char   sectionNumber ;
	  unsigned char   secFilterIndex ;
	};
	typedef struct ARG_GET_APF_EIT_        ARG_GET_APF_EIT_T ;
	struct ARG_MONITOR_APF_EIT_ {
	  unsigned short  pid :13 ;
	  unsigned char   tid     ;
	  unsigned short  serviceId ;
	  unsigned char   sectionNumber ;
	  unsigned char   version ;
	  unsigned char   secFilterIndex ;
	};
	typedef struct ARG_MONITOR_APF_EIT_        ARG_MONITOR_APF_EIT_T ;
	typedef struct _SecFilterPattern ARG_8Byte_FILTER_T;
	typedef CabotFilter 				ARG_Cabot_FILTER_T;

	#if 0
	typedef struct ARG_8Byte_FILTER_T_ {
		unsigned short  pid;
		unsigned char tid;	//0xff mean don't care
		//unsigned char* tails;
		unsigned char patSize;
		//byte[0] is first byte after sectionLeng
		//it maybe programNum(sid),tsid,...
		unsigned char pat[8];
		//positive mask(bit set to 0 means don't care)
		unsigned char patPosMask[8];
	} ARG_8Byte_FILTER_T;
	#endif//0

#ifdef ENABLE_SYSTEM_SOFTWARE_UPDATE
    struct ARG_GET_DXI_ {
      unsigned short  pid :13 ;
      unsigned char	  tid     ;
      unsigned short  id :15  ;
      unsigned char   secFilterIndex ;
    };
    typedef struct ARG_GET_DXI_        ARG_GET_DXI_T ;
    struct ARG_MONITOR_DXI_ {
      unsigned short  pid :13 ;
      unsigned char   tid     ;
      unsigned short  id :15  ;
      unsigned char   versionFlag :1 ;
      unsigned char   secFilterIndex ;
    };
    typedef struct ARG_MONITOR_DXI_        ARG_MONITOR_DXI_T ;
    struct ARG_GET_SSU_DDB_ {
      unsigned short  pid :13 ;
      unsigned char   tid;
      unsigned char   moduleIdMsb;
      unsigned char   secFilterIndex ;
    };
    typedef struct ARG_GET_SSU_DDB_        ARG_GET_SSU_DDB_T;

#endif//ENABLE_SYSTEM_SOFTWARE_UPDATE


struct SEC_FILTER_LAYER_ {
	  struct SEC_FILTER_LAYER_  *fd ;
	  struct SEC_FILTER_LAYER_  *bk ;
	  SEC_FILTER_FUNCTION secFilterFunction ;

	  SEC_FILTER_OWNER owner;

	  unsigned char  referenceCount ;

	  union {
	    ARG_GET_TABLE_T       argGetTable         ;
	    ARG_MONITOR_PAT_T     argMonitorPat       ;
	    ARG_GET_PMT_T         argGetPmt           ;
	    ARG_MONITOR_PMT_T     argMonitorPmt       ;
	    ARG_MONITOR_TABLE_T   argMonitorTable     ;
	    ARG_GET_APF_EIT_T     argGetApfEit        ;
	    ARG_MONITOR_APF_EIT_T argMonitorApfEit    ;
  	    ARG_8Byte_FILTER_T 	argGSecFilter;	//Gemstar section filter
  	    ARG_Cabot_FILTER_T	argCabotFilter;
#ifdef ENABLE_SYSTEM_SOFTWARE_UPDATE
	  ARG_GET_DXI_T 			argGetDxi;
	  ARG_MONITOR_DXI_T 		argMonitorDxi;
	  ARG_GET_SSU_DDB_T 	argGetSsuDdb;
#endif//ENABLE_SYSTEM_SOFTWARE_UPDATE

	  };
	};
	typedef struct SEC_FILTER_LAYER_        SEC_FILTER_LAYER_T, *SEC_FILTER_LAYER_P ;

	struct SEC_FILTER_LAYER_HD_ {
	  struct SEC_FILTER_LAYER_HD_  *fd ;
	  struct SEC_FILTER_LAYER_HD_  *bk ;
	};
	typedef struct SEC_FILTER_LAYER_HD_        SEC_FILTER_LAYER_HD_T, *SEC_FILTER_LAYER_HD_P ;

	struct PID_LAYER_ {
	  struct PID_LAYER_  *fd ;
	  struct PID_LAYER_  *bk ;
	  unsigned short     pid :13 ;
	  SEC_FILTER_LAYER_HD_T secFilterLinkList ;
	};
	typedef struct PID_LAYER_       PID_LAYER_T, *PID_LAYER_P ;

	struct PID_LAYER_HD_ {
	  struct PID_LAYER_HD_  *fd ;
	  struct PID_LAYER_HD_  *bk ;
	};
	typedef struct PID_LAYER_HD_       PID_LAYER_HD_T, *PID_LAYER_HD_P ;


	//for store filter list(if a section match multiple filters~)
	typedef struct _SEC_FILTER_LIST {
		int count;
		SEC_FILTER_LAYER_P list[MAX_SF_MATCH_COUNT];  //allow at most MAX_SF_HANDLE_COUNT handles
	} SEC_FILTER_LIST;


typedef enum
{
	VSFM_OK,
	VSFM_ERR,
	VSFM_SET_SF_ERR,
	VSFM_NOT_FIND_PID,
	VSFM_NOT_FIND_SF, //not find this section filter
	VSFM_INVALID_HANDLE,
	VSFM_SECTION_NOT_MATCH,
} VSFM_ERRCODE;


//#ifdef _EXPORT_SECTION_FILTER_MODULE
//following lock mechanism "maybe" needed only when _EXPORT_SECTION_FILTER_MODULE is enabled
#define VSFM_INIT_MUTEX(ag)				pthread_mutex_init(&((ag)->vsfmMutex), NULL);
#define VSFM_LOCK(ag)				{ pthread_mutex_lock(&((ag)->vsfmMutex)); }
#define VSFM_UNLOCK(ag)			{ pthread_mutex_unlock(&((ag)->vsfmMutex));}
#define VSFM_DESTROY_MUTEX(ag)			pthread_mutex_destroy(&((ag)->vsfmMutex));





	#if 0
	#define VSFM_W_LOCK(vsfm) do { VSFM_LOCK_MUTEX(vsfm->mutex); } while(0)
	#define VSFM_W_UNLOCK(vsfm)	do { VSFM_UNLOCK_MUTEX(vsfm->mutex);} while(0)
	#define VSFM_R_LOCK(vsfm)	do { VSFM_LOCK_MUTEX(vsfm->mutex); } while(0)
	#define VSFM_R_UNLOCK(vsfm)	do { VSFM_UNLOCK_MUTEX(vsfm->mutex);} while(0)
	#endif//


int VSFM_Init(P_SIAGENT_OBJECT pSiAgentObj);

//ErrCode SI_RegisterSF_Module(SI *pSI,  SF_MODULE_HANDLE* moduleHandle);
//ErrCode SI_UnRegisterSF_Module(SI  *pSI,  SF_MODULE_HANDLE*  );
VSFM_ERRCODE VSFM_AddSectionFilter(SI*, SF_MODULE_HANDLE mh, SEC_FILTER_FUNCTION sfType, void  *sfPat, SF_HANDLE* sfHandle);
VSFM_ERRCODE VSFM_RemoveSectionFilter(SI*, SF_HANDLE* sfHandle);
VSFM_ERRCODE VSFM_GetSectionFilterOwner(SI*,  unsigned int pid, unsigned char* rawData, unsigned int secLeng, unsigned int* owner);
VSFM_ERRCODE VSFM_GetMatchedSectionFilterList(SI* si,  unsigned int pid, unsigned char* rawData, unsigned int secLeng, unsigned int* owner, SEC_FILTER_LIST* list);
VSFM_ERRCODE VSFM_Reset(P_SIAGENT_OBJECT pSiAgentObj);

void VSFM_Mutex_Lock(SI* si );
void VSFM_Mutex_Unlock(SI* si);

	

//SEC_FILTER_LAYER_P SiAgent_Enable_KByteFilter( P_SIAGENT_OBJECT pSiAgentObj, SecFilterPattern* pat, SEC_FILTER_OWNER owner, SF_HANDLE* sfHandle);
//void SiAgent_Disable_KByteFilter( P_SIAGENT_OBJECT pSiAgentObj, SecFilterPattern* pat, SEC_FILTER_OWNER owner)
//VSFM_ERRCODE SiAgent_Disable_KByteFilter( P_SIAGENT_OBJECT pSiAgentObj, SEC_FILTER_LAYER_P sf);

SEC_FILTER_LAYER_P SiAgent_Enable_OtherFilter( P_SIAGENT_OBJECT pSiAgentObj, SEC_FILTER_FUNCTION type, void* pat, SEC_FILTER_OWNER owner, SF_HANDLE* sfHandle);
//void SiAgent_Disable_KByteFilter( P_SIAGENT_OBJECT pSiAgentObj, SecFilterPattern* pat, SEC_FILTER_OWNER owner)
VSFM_ERRCODE SiAgent_Disable_OtherFilter( P_SIAGENT_OBJECT pSiAgentObj, SEC_FILTER_LAYER_P sf);




#undef P_SIAGENT_OBJECT

//#endif//_EXPORT_SECTION_FILTER_MODULE







#ifdef _SIAGENTVIRSECFILTERMGR_C_
    #define _SIAGENTVIRSECFILTERMGR_C_DECLARE_

	static PID_LAYER_P SiAgent_VSFM_SearchPidLayer( P_SIAGENT_OBJECT pSiAgentObj, unsigned short pid );
	static SEC_FILTER_LAYER_P SiAgent_VSFM_SearchSecFilterLayer( P_SIAGENT_OBJECT pSiAgentObj, PID_LAYER_P pPidLayer, SEC_FILTER_LAYER_P pSearchSecFilter );
	static PID_LAYER_P SiAgent_VSFM_CreatePidLayer( P_SIAGENT_OBJECT pSiAgentObj, unsigned short pid );

	//#ifdef _DEBUG_SIAGENT_MEM_
	#if 1

		#define VSFM_MALLOC(size)  SI_AGENT_MALLOC(size)
 		#define VSFM_FREE(p)   SI_AGENT_FREE((p))

	    #if 0
	       #include <memCheck.h>
	       #define VSFM_MALLOC(size)     Memcheck_Alloc(size)
	       #define VSFM_FREE(p)          do{                   \
	                                        Memcheck_Free(p) ; \
	                                        (p) = NULL ;       \
	                                       }while(0)
	    #endif//0
	#else
		#define VSFM_MALLOC(size)     malloc(size)
		#define VSFM_FREE(p)          do{             \
                                        free(p) ;    \
                                        (p) = NULL ; \
                                       }while(0)
	#endif//_DEBUG_SIAGENT_MEM_



    #define DBG_MSG_LEVEL_INFO    0
    #define DBG_MSG_LEVEL_WARNING 1
    #define DBG_MSG_LEVEL_ERROR   2

    #if 0 //def _DEBUG_SIAGENT_VSFM_
           #define SIAGENT_VSFM_DBG_MSG(x, format, args...) { \
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
    #else  //_DEBUG_SIAGENT_VSFM_
           #define SIAGENT_VSFM_DBG_MSG(x, format, args...)
    #endif //_DEBUG_SIAGENT_VSFM_

    #ifdef _OTHER_VSFM_DEBUG_
	#define OTHER_VSFM_DBG_MSG(x, format, args...) { \
                    switch(x) { \
                    case DBG_MSG_LEVEL_WARNING: printf("\033[1;33m%s() \033[m", __FUNCTION__); \
                        printf("[WARNING][%4d]: ", __LINE__); break; \
                    case DBG_MSG_LEVEL_ERROR  : printf("\033[1;31m%s() \033[m", __FUNCTION__); \
                        printf("[ERROR][%4d]: ", __LINE__); break; \
                    default: break; } \
                    printf(format, ##args); \
                    printf("\n"); \
                   }
    #else//_OTHER_VSFM_DEBUG_
		#define OTHER_VSFM_DBG_MSG(x, format, args...)
    #endif//_OTHER_VSFM_DEBUG_



#else //_SIAGENTVIRSECFILTERMGR_C_
    #define _SIAGENTVIRSECFILTERMGR_C_DECLARE_ extern

#endif //_SIAGENTVIRSECFILTERMGR_C_




#endif //_SIAGENTVIRSECFILTERMGR_H_













