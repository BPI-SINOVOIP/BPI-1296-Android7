
/*=======================================================================
 * Copyright c        Realtek Semiconductor Corporation, 2008
 * All rights reserved.
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * Module   : SIAGENT2
 * File   : siAgentCustomize.h
 *
 * Author   : palladio@realtek
 * Date   : 2008/01/16
 *
 *
 * modified by kjw for porting to pvr platform(2008/05/21)
 *
 *=======================================================================*/


#ifndef _SIAGENTCUSTOMIZE_H_
#define _SIAGENTCUSTOMIZE_H_

typedef enum _SIAGENT_TIMEOUT_TYPE
{
	SCAN_CH__PAT_TIMEOUT,


} SIAGENT_TIMEOUT_TYPE;


typedef struct _SIAGENT_SECTION_TIMEOUT
{
	unsigned int scanChPatTimeout ;
	unsigned int scanChPmtTimeout ;
	unsigned int scanChANitTimeout ;
	unsigned int scanChASdtTimeout ;

} SIAGENT_SECTION_TIMEOUT;

//timout setting

void Customize_ScanCh_Set_PAT_Timeout( unsigned int second );
void Customize_ScanCh_Set_PMT_Timeout(  unsigned int second );
void Customize_ScanCh_Set_aNIT_Timeout(  unsigned int second );
void Customize_ScanCh_Set_aSDT_Timeout(  unsigned int second );

 unsigned int Customize_ScanCh_Get_PAT_Timeout( void );
 unsigned int Customize_ScanCh_Get_PMT_Timeout( void );
 unsigned int Customize_ScanCh_Get_aNIT_Timeout( void );
 unsigned int Customize_ScanCh_Get_aSDT_Timeout( void );

void Customize_ScanCh_SetDefaultTimeoutValue( void );




#ifdef _SIAGENTCUSTOMIZE_C_
    #define _SIAGENTCUSTOMIZE_C_DECLARE_

    //#define _DEBUG_SIAGENTCUSTOMIZE_



    #if 0

        #include <memCheck.h>

        #define SC_MALLOC(size)   Memcheck_Alloc(size)

        #define SC_FREE(p)        do{             \
                                     Memcheck_Free(p) ;    \
                                     (p) = NULL ; \
                                    }while(0)

    #else

        #define SC_MALLOC(size)   malloc(size)

        #define SC_FREE(p)        do{             \
                                     free(p) ;    \
                                     (p) = NULL ; \
                                    }while(0)

    #endif



    #define DBG_MSG_LEVEL_INFO    0
    #define DBG_MSG_LEVEL_WARNING 1
    #define DBG_MSG_LEVEL_ERROR   2

    #ifdef _DEBUG_SIAGENTCUSTOMIZE_
           #define SC_DBG_MSG(x, format, args...) { \
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
    #else //#ifdef _DEBUG_SIAGENTCUSTOMIZE_
           #define SC_DBG_MSG(x, format, args...)
    #endif //#ifdef _DEBUG_SIAGENTCUSTOMIZE_

#else //_SIAGENTCUSTOMIZE_C_
    #define _SIAGENTCUSTOMIZE_C_DECLARE_ extern

#endif //_SIAGENTCUSTOMIZE_C_






#endif //_SIAGENTCUSTOMIZE_H_


