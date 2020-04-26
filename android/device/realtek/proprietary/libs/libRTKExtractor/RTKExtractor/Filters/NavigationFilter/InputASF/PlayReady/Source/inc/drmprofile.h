/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMPROFILE_H__
#define __DRMPROFILE_H__

#include <drmcommon.h>
#include <drmprofileconstants.h>
#include <drmtrace.h>

ENTER_PK_NAMESPACE;

/* Maxmimum size of cached sample buffer in bytes, should be a multiple of 16 bytes. */
#define MAX_PROFILE_BUFFER_SIZE     92160

/*
** Scope ID is made up of three components: Module ID (12 bits), Function ID (12 bits) and Block ID (7 bits)
** This Macro has been moved out from conditional compilation so that perftool can access it without turning
** ON profiling.
*/
#define PERF_SCOPE_ID( ModuleID, FuncID, BlockID )  ( ( ( ( ModuleID ) & 0x0fff ) << 19 ) + ( ( ( FuncID ) & 0x0fff ) << 7 ) + ( ( BlockID ) & 0x007f ) )

#if DRM_SUPPORT_PROFILING

    /* Flag to indicate whether to enable user data logging. */
    /* User data logging is not supported under aggregated scope sampling. */
    #define PROFILE_USER_DATA       0

    /* PROFILE_USE_SCOPE is defined to use normal scope sampling mechanism. */
    #define PROFILE_USE_SCOPE       1

    /* PROFILE_USE_AGG_SCOPE is defined to use aggregated scope sampling mechanism. */
    #define PROFILE_USE_AGG_SCOPE   0

    /* PROFILE_STACK_SIZE should be turned on to get the max. stack utilization by DRM APIs. */
    #define PROFILE_STACK_SIZE      0

    /* Note: One and only one of the above two definitions can be 1. */
    #if ( PROFILE_USE_SCOPE && PROFILE_USE_AGG_SCOPE ) || ( !PROFILE_USE_SCOPE && !PROFILE_USE_AGG_SCOPE )
        #error One and only one of PROFILE_USE_SCOPE and PROFILE_USE_AGG_SCOPE can be 1.
    #endif

    #if ( PROFILE_USER_DATA && !PROFILE_USE_SCOPE )
        #error User data logging is supported only under normal scope sampling.
    #endif

    #if ( PROFILE_USER_DATA && PROFILE_USE_AGG_SCOPE )
        #error User data logging is not supported under aggregated scope sampling.
    #endif

    /* Maximum levels of nested scope calls. */
    #define MAX_PROFILE_SCOPE_LEVEL     512

    /* Maximum number of aggregated scopes. */
    #define MAX_PROFILE_AGG_SCOPES      1024

    /*
    ** PERF_AGG_SCOPE_CONTEXT is used to store aggregated information of a scope.
    ** All PERF_AGG_SCOPE_CONTEXT structures are stored in global memory and
    ** are persisted at the end of profiling.
    */
    typedef struct _perf_scope_agg_context
    {
        /* Define detail level of logging for this scope. 0: do not check detail level. */
        DRM_UINT m_nDetailLevel;
        
        /* Define feature set this scope belongs to. 0: do not check feature set. */
        DRM_UINT m_nFeatureSet;

        /* Scope ID. */
        DRM_DWORD m_dwScopeID;
        
        /* Aggregated duration of the scope. */
        DRM_DWORD m_dwDuration;

        /* Total call count of the scope. */
        DRM_DWORD m_dwCallCount;

    } PERF_AGG_SCOPE_CONTEXT;

    /* PERF_GLOBAL_CONTEXT is the global context of the profiling machanism. */
    typedef struct _perf_global_context
    {
        /* Scope stack. */
        DRM_DWORD m_dwScopeID[ MAX_PROFILE_SCOPE_LEVEL ];
        
        /* Point to top of the scope stack. */
        DRM_UINT m_nCurrScope;

        /* Logging scopes with detail level < this value.  */
        DRM_UINT m_nDetailLevel;

        /* Logging scopes within a specific group of feature sets. */
        DRM_UINT m_nFeatureSet;

        /* Next PER_AGG_SCOPE_CONTEXT slot number to be assigned. */
        DRM_UINT m_nCurrAggScope;
 
        /* File handle for normal scope samples. */
        OEM_FILEHDL m_fp;
 
        /* File handle for aggregated scope samples. */
        OEM_FILEHDL m_fpAgg;
 
        /* Buffer to cache normal scope samples to minimize I/O. */
        DRM_BYTE m_bBuffer[ MAX_PROFILE_BUFFER_SIZE ];

        /* Next m_bBuffer slot to be assigned. */
        DRM_DWORD m_dwBufferIndex;

        /* Global aggregated scope context. */
        PERF_AGG_SCOPE_CONTEXT m_AggScopes[ MAX_PROFILE_AGG_SCOPES ];

        /* Flag indicating whether the profiling machanism is enable or not. */
        DRM_BOOL m_bRunning;

        /* Flag to prevent recursive profiling. */
        DRM_BOOL m_bInProfile;

        /* Time spent in writing profiling logs to the disk. */
        DRM_DWORD m_dwLogWriteDuration;

        #if PROFILE_USER_DATA    
            /* File handle for user data logging. */
            OEM_FILEHDL * m_fpUserData;

            /* Buffer to cache user data to minimize I/O. */
            DRM_BYTE m_bBufferUserData[ MAX_PROFILE_BUFFER_SIZE ];

            /* Next m_bBufferUserData slot to be assigned. */
            DRM_DWORD m_dwBufferUserDataIndex;
        #endif

        #if PROFILE_STACK_SIZE
            /* File handle for logging stack size data. */
            OEM_FILEHDL  m_fpStackSize;

            /* Max stack usage deteted so far for current PK API (scopelevel = 2). */
            DRM_DWORD m_dwMaxStackSize;

            /* Stack base - relative to this stack size will be calculated. */
            DRM_DWORD_PTR m_dwStackBase;

            /* Scope ID of the PK API leading to max stack usage. */
            DRM_DWORD m_dwMaxStackSizeScopeID;
 
            /* Max stack usage over all the PK APIs involved in a specific scenario. */
            DRM_DWORD m_dwMaxStackSizeinScenario;
        #endif

    } PERF_GLOBAL_CONTEXT;

    typedef struct _perf_scope_context
    {
        /* Define detail level of logging for this scope. 0: do not check detail level. */
        DRM_UINT m_nDetailLevel;

        /* Define feature set this scope belongs to. 0: do not check feature set. */
        DRM_UINT m_nFeatureSet;

        /* Scope ID. */
        DRM_DWORD m_dwScopeID;

        /* Parent scope ID. */
        DRM_DWORD m_dwParentScopeID;

        /* Duration of the scope. */
        DRM_DWORD m_dwDuration;
    
        #if PROFILE_USER_DATA
            /* Custom user data per context. */
            DRM_DWORD m_dwUserData1;
            
            DRM_DWORD m_dwUserData2;
            
            /* More user data can be defined afterwards. */
        #endif    

        /*
        ** Trace level of the current scope.
        ** If both profiling and tracing are enabled, the
        ** scope level tracing control will stay in the
        ** perf scope context.
        **/
        eTraceLevel m_eTraceLevel;
            
        /* Current component ID. */
        DRM_DWORD m_dwComponent;

    } PERF_SCOPE_CONTEXT;

EXIT_PK_NAMESPACE;

    /* OEM function to retrieve global prifile context. */
    #include <oemprofile.h>

ENTER_PK_NAMESPACE;

    extern DRM_DWORD Perf_String_Data( DRM_CONST_STRING * pdstrIn );

    extern DRM_DWORD Perf_Binary_Data( DRM_BYTE * pbIn, DRM_DWORD dwSize );

    extern DRM_RESULT Perf_Log_Profile( PERF_SCOPE_CONTEXT * pScopeContext, DRM_UINT nScopeLevel );

    extern DRM_RESULT Perf_Enter_Scope( PERF_SCOPE_CONTEXT * pScopeContext );

    extern DRM_RESULT Perf_Leave_Scope( PERF_SCOPE_CONTEXT * pScopeContext );

    extern DRM_RESULT Perf_Enter_Agg_Scope( PERF_AGG_SCOPE_CONTEXT ** ppAggScopeContext, DRM_DWORD dwScopeID );

    extern DRM_RESULT Perf_Leave_Agg_Scope( PERF_AGG_SCOPE_CONTEXT * pAggScopeContext, DRM_DWORD dwDuration );

    extern DRM_RESULT Perf_Start_Profiling( DRM_VOID *pOEMContext, DRM_CONST_STRING *dstrPerfFileName );

    extern DRM_RESULT Perf_Stop_Profiling();

    extern DRM_RESULT Perf_Init_ProfileStack();

    /* Macro definitions. */

    /* Returns the performance counter's currrent count in microseconds. */    
#if DRM_NO_NATIVE_64_TYPES
    static DRM_UINT64 PERF_MICROSECONDS_PER_SECOND = DRM_UI64LITERAL( 0,1000000 );
    #define PERF_CURRENT_MICROSECOND_COUNT  \
        ( DRM_UI64Low32( DRM_UI64Div( DRM_UI64Mul( Oem_Profiling_QueryPerformanceCounter(), PERF_MICROSECONDS_PER_SECOND ), Oem_Profiling_QueryPerformanceFrequency() ) ) )
#else
    #define PERF_MICROSECONDS_PER_SECOND 1000000.0
    #define PERF_CURRENT_MICROSECOND_COUNT  \
        ( ( DRM_DWORD )( ( ( double )Oem_Profiling_QueryPerformanceCounter() ) * ( PERF_MICROSECONDS_PER_SECOND / ( ( double )Oem_Profiling_QueryPerformanceFrequency() ) ) ) )
#endif /* DRM_NO_NATIVE_64_TYPES */

    /* Macros used for normal scope sampling. */
    #define PERF_SCOPE_DETAIL_LEVEL_X( DetailLevel)         ScopeContext.m_nDetailLevel = ( DetailLevel );

    #define PERF_SCOPE_FEATURE_SET_X( FeatureSet)           ScopeContext.m_nFeatureSet = ( FeatureSet );

    #define PERF_ENTER_SCOPE_X( ModuleID, FuncID, BlockID ) { \
                                                            PERF_SCOPE_CONTEXT ScopeContext = { 0 }; \
                                                            ScopeContext.m_dwScopeID = PERF_SCOPE_ID( ( ModuleID ), ( FuncID ), ( BlockID ) ); \
                                                            ScopeContext.m_nDetailLevel = 1; \
                                                            ScopeContext.m_nFeatureSet = 0; \
                                                            ScopeContext.m_eTraceLevel = TRACE_FATAL; \
                                                            Perf_Enter_Scope(&ScopeContext); \
                                                            DRM_TRACE_ENTER_SCOPE( TRACE_DEFAULT_ROBUST, ScopeContext.m_dwScopeID );

    #define PERF_LEAVE_SCOPE_X                              Perf_Leave_Scope( &ScopeContext );  \
                                                            DRM_TRACE_LEAVE_SCOPE( TRACE_DEFAULT_ROBUST, ScopeContext.m_dwScopeID, dr ); \
                                                            }

    #define PERF_ENTER_SCOPE( ModuleID, FuncID )            PERF_ENTER_SCOPE_X( ModuleID, FuncID, 1 )

    #define PERF_LEAVE_SCOPE                                PERF_LEAVE_SCOPE_X;

    /* Macros used for aggregated scope sampling. */
    #define PERF_AGG_SCOPE_DETAIL_LEVEL_X( DetailLevel )    pAggScopeContext->m_nDetailLevel = ( DetailLevel );

    #define PERF_AGG_SCOPE_FEATURE_SET_X( FeatureSet )      pAggScopeContext->m_nFeatureSet = ( FeatureSet );

    #define PERF_ENTER_AGG_SCOPE_X( ModuleID, FuncID, BlockID ) { \
                                                            static PERF_AGG_SCOPE_CONTEXT * pAggScopeContext = NULL; \
                                                            DRM_DWORD dwPerfAggEnterTick = PERF_CURRENT_MICROSECOND_COUNT; \
                                                            Perf_Enter_Agg_Scope( &pAggScopeContext, PERF_SCOPE_ID( ( ModuleID ), ( FuncID ), ( BlockID ) ) );

    #define PERF_LEAVE_AGG_SCOPE_X                          dwPerfAggEnterTick = PERF_CURRENT_MICROSECOND_COUNT - dwPerfAggEnterTick; \
                                                            Perf_Leave_Agg_Scope( pAggScopeContext, dwPerfAggEnterTick ); \
                                                            }

    #define PERF_ENTER_AGG_SCOPE( ModuleID, FuncID )        PERF_ENTER_AGG_SCOPE_X( ModuleID, FuncID, 1 )

    #define PERF_LEAVE_AGG_SCOPE                            PERF_LEAVE_AGG_SCOPE_X;

    /* Below are macros that are used externally by the code to be profiled. */

    /* Macros used for global context setup. */
    #define DRM_PROFILING_GLOBAL_DETAIL_LEVEL( DetailLevel )    \
                                                            Oem_Profiling_GetProfileContext()->m_nDetailLevel = ( DetailLevel );

    #define DRM_PROFILING_GLOBAL_FEATURE_SET( FeatureSet )  Oem_Profiling_GetProfileContext()->m_nFeatureSet = ( FeatureSet );

    #define DRM_PROFILING_STRING_HASH( pstrIn )             ( Perf_String_Data( ( DRM_CONST_STRING * )pstrIn ) )

    #define DRM_PROFILING_BINARY_HASH( pbIn, dwSize )       ( Perf_Binary_Data( pbIn, dwSize ) )

    #if PROFILE_USER_DATA

        #define DRM_PROFILING_USER_DATA_1( UserData )       ScopeContext.m_dwUserData1 = ( UserData );

        #define DRM_PROFILING_USER_DATA_2( UserData )       ScopeContext.m_dwUserData2 = ( UserData );

    #else 

        #define DRM_PROFILING_USER_DATA_1( UserData )

        #define DRM_PROFILING_USER_DATA_2( UserData )

    #endif  /* PROFILE_USER_DATA */

    /* Define generic macros used in the source code. */
    #if PROFILE_USE_SCOPE

        #define DRM_PROFILING_DETAIL_LEVEL( DetailLevel )   PERF_SCOPE_DETAIL_LEVEL_X( DetailLevel )

        #define DRM_PROFILING_FEATURE_SET( FeatureSet )     PERF_SCOPE_FEATURE_SET_X( FeatureSet )

        #define DRM_PROFILING_ENTER_SCOPE_X                 PERF_ENTER_SCOPE_X

        #define DRM_PROFILING_LEAVE_SCOPE_X                 PERF_LEAVE_SCOPE_X

        #define DRM_PROFILING_ENTER_SCOPE                   PERF_ENTER_SCOPE

        #define DRM_PROFILING_LEAVE_SCOPE                   PERF_LEAVE_SCOPE

    #endif  /* PROFILE_USE_SCOPE */

    #if PROFILE_USE_AGG_SCOPE

        #define DRM_PROFILING_DETAIL_LEVEL( DetailLevel )   PERF_AGG_SCOPE_DETAIL_LEVEL_X(DetailLevel)

        #define DRM_PROFILING_FEATURE_SET( FeatureSet )     PERF_AGG_SCOPE_FEATURE_SET_X(FeatureSet)

        #define DRM_PROFILING_ENTER_SCOPE_X                 PERF_ENTER_AGG_SCOPE_X

        #define DRM_PROFILING_LEAVE_SCOPE_X                 PERF_LEAVE_AGG_SCOPE_X

        #define DRM_PROFILING_ENTER_SCOPE                   PERF_ENTER_AGG_SCOPE

        #define DRM_PROFILING_LEAVE_SCOPE                   PERF_LEAVE_AGG_SCOPE

    #endif  /* PROFILE_USE_AGG_SCOPE */

    #define DRM_Start_Profiling( x, y ) Perf_Start_Profiling( x, y )

    #define DRM_Stop_Profiling()        Perf_Stop_Profiling()

    #define DRM_Init_ProfileStack()     Perf_Init_ProfileStack()

#else   /* DRM_SUPPORT_PROFILING */

    #define DRM_PROFILING_USER_DATA_1( UserData )

    #define DRM_PROFILING_USER_DATA_2( UserData )

    #define DRM_PROFILING_DETAIL_LEVEL( DetailLevel )

    #define DRM_PROFILING_FEATURE_SET( FeatureSet )

    #define DRM_PROFILING_ENTER_SCOPE_X( ModuleID, FuncID, BlockID )    

    #define DRM_PROFILING_LEAVE_SCOPE_X()

    #if DRM_SUPPORT_TRACING

        typedef struct _trace_scope_context
        {
            /* Scope ID. */
            DRM_DWORD m_dwScopeID;

            /* Trace level of the current scope. */
            eTraceLevel m_eTraceLevel;
            
            /* Current component ID. */
            DRM_DWORD m_dwComponent;

        } TRACE_SCOPE_CONTEXT;

        #define DRM_PROFILING_ENTER_SCOPE( ModuleID, FuncID )       { \
                                                                    TRACE_SCOPE_CONTEXT ScopeContext = { 0 }; \
                                                                    ScopeContext.m_dwScopeID = PERF_SCOPE_ID( ( ModuleID ), ( FuncID ), 1 ); \
                                                                    ScopeContext.m_eTraceLevel = TRACE_FATAL; \
                                                                    DRM_TRACE_ENTER_SCOPE( TRACE_DEFAULT_ROBUST, ScopeContext.m_dwScopeID );

        #define DRM_PROFILING_LEAVE_SCOPE                           DRM_TRACE_LEAVE_SCOPE( TRACE_DEFAULT_ROBUST, ScopeContext.m_dwScopeID, dr ); }

    #else

        #define DRM_PROFILING_ENTER_SCOPE( ModuleID, FuncID )

        #define DRM_PROFILING_LEAVE_SCOPE

    #endif  /* DRM_SUPPORT_TRACING */

    #define DRM_PROFILING_GLOBAL_DETAIL_LEVEL( DetailLevel )

    #define DRM_PROFILING_GLOBAL_FEATURE_SET( FeatureSet )

    #define DRM_PROFILING_STRING_HASH( pstrIn )

    #define DRM_PROFILING_BINARY_HASH( pstrIn )

    #define DRM_Start_Profiling( x, y ) DRM_SUCCESS

    #define DRM_Stop_Profiling()        DRM_SUCCESS

    #define DRM_Init_ProfileStack()     DRM_SUCCESS

#endif /* DRM_SUPPORT_PROFILING */

EXIT_PK_NAMESPACE;

#endif    /* __DBMPROFILE_H__ */
