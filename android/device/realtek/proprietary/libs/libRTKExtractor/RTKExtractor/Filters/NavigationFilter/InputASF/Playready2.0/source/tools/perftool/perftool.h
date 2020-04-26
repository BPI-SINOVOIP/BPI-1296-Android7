/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/
#define STRSAFE_LIB
#define STRSAFE_NO_DEPRECATE
#include <strsafe.h>
#include <drmcommon.h>
#include <drmutilities.h>
#include <drmcrt.h>
#include <drmcontextsizes.h>
#include <drmmanager.h>
#include <stdio.h>
#include <drmcmdlnpars.h>
#include <drmprofileconstants.h>
#include <drmprofile.h>
#include <drmtoolsconstants.h>

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#define MAX_LINE_SIZE 256
/*The size MAX_MOD and MAX_FUNC will have to be changed whenever additional perf counters are added
to drmprofileconstants.h*/
#define MAX_MOD 200
#define MAX_FUNC 150

typedef struct _perf_scope_node
{
    DRM_DWORD m_dwModuleID;
    DRM_DWORD m_dwFunctionID;
    DRM_DWORD m_dwDuration;
    DRM_DWORD m_dwScopeLevel;
    struct _perf_scope_node *m_pChild;
    struct _perf_scope_node *m_pParent;
    struct _perf_scope_node *m_pPeer; 
    DRM_BOOL m_bRunning;
} PERF_SCOPE_NODE;

typedef struct _level_node
{
    DRM_LONG nLevel;
    PERF_SCOPE_NODE *m_pNodeList;
    struct _level_node *m_pNextLevelNode;
    struct _level_node *m_pPreviousLevelNode;
} LEVEL_NODE;

typedef struct _agg_duration
{
    DRM_DWORD m_dwRPM;
    DRM_DWORD m_dwChildRPM;
}AGG_DURATION;


DRM_RESULT GetIndex(const DRM_CHAR *pStr,DRM_DWORD *pdwResult);
DRM_RESULT ProcessConfigFile(DRM_CONST_STRING dstrConfigFile, DRM_CONST_STRING dstrTestCaseID);
DRM_RESULT OutputTree(PERF_SCOPE_NODE *pStartNode);
DRM_RESULT PrintNode(PERF_SCOPE_NODE *pNode, DRM_LONG n_indent);
DRM_RESULT InsertNode(LEVEL_NODE *pParentLevel, PERF_SCOPE_NODE *pCurrNode, DRM_DWORD dwParentModuleID, DRM_DWORD dwParentFunctionID );
DRM_RESULT CreatePerfScopeNode(PERF_SCOPE_NODE **ppCurrNode, DRM_DWORD dwModuleID, DRM_DWORD dwFunctionID, DRM_DWORD dwDuration, DRM_DWORD dwScopeLevel);
DRM_RESULT GetScopeLevelNode(LEVEL_NODE **pCurrLevel, DRM_DWORD dwScopeLevel);
DRM_RESULT Cleanup(DRM_VOID);
DRM_RESULT ParseAndSave();
DRM_RESULT GetPerfCounter(DRM_SCOPE **pPerfCounter, DRM_DWORD dwModuleIndex, DRM_DWORD dwFunctionIndex);
DRM_RESULT ParseStackLog();
static void PackedCharsToNative( DRM_CHAR *f_pPackedString, DRM_DWORD cch );


#ifdef __cplusplus
}
#endif

