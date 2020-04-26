/*=======================================================================
 * Copyright c				Realtek Semiconductor Corporation, 2007     *
 * All rights reserved.                                                 *
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
 *
 * Module		: C Framework
 *
 * Author		: ss@realtek.com.tw
 * Date			: 2007/04
 *
 *=======================================================================*/

#ifndef _MEMCHECK_H_
#define _MEMCHECK_H_

#include <stdlib.h>


//--------------------------------------------------------------------
// Configs
//--------------------------------------------------------------------
//#define _MEMCHECK_PRINT_CMDS				1

#define MEMCHECK_ALLOCTION_LIST_NUM		64
#define MEMCHECK_MAX_FILENAME_LEN		16

//#define MEMCHECK_ALLOCTION_LIST_NUM		256
//#define MEMCHECK_MAX_FILENAME_LEN		32


// since this shall always be TRUE, this module will not deal with the
// situation in non-word-alignment systems
#define MEMCHECK_WORD_ALIGNMENT

//--------------------------------------------------------------------
// Function Interface
//--------------------------------------------------------------------
#define CF_Memory_Alloc(modID, i_Size)	_CF_Memory_Alloc(modID, i_Size, __FILE__, __LINE__);
#define CF_Memory_Free(modID, p_Addr)	_CF_Memory_Free (modID, p_Addr, __FILE__, __LINE__);


//--------------------------------------------------------------------
// Definition
//--------------------------------------------------------------------
typedef struct CFMemoryAllocation		CFMemoryAllocation;
struct CFMemoryAllocation
{
	CFMemoryAllocation					*next;
	int									size;
	CF_MODID							moduleID;
	char								fileName[MEMCHECK_MAX_FILENAME_LEN];
	int									lineNumber;
	unsigned char						buf[0];
} ;

typedef struct CFMemoryAllocationList	CFMemoryAllocationList;
struct CFMemoryAllocationList
{
	CFMemoryAllocation					*ma;
	unsigned int						allocatedSize;
	unsigned int						allocatedNum;
	unsigned int						freeNum;
};

typedef struct CFMemoryAllocationCollection	CFMemoryAllocationCollection;
struct CFMemoryAllocationCollection
{
	CFMemoryAllocationList					allocationLists[MEMCHECK_ALLOCTION_LIST_NUM];
};

typedef enum
{
	CF_M_TRACELEVEL_LISTALL=0,
	CF_M_TRACELEVEL_PROFILING=1,
	CF_M_TRACELEVEL_SIMPLE=2,

} CF_M_TRACELEVEL;


//--------------------------------------------------------------------
// Functions
//--------------------------------------------------------------------

void CF_Memory_Init(
	void 								*(*allocFunc)(size_t),
	void 								(*freeFunc)(void*)
);

void CF_Memory_DeInit();

// use MemCheck_Alloc(size)
void *_CF_Memory_Alloc
(
	CF_MODID							moduleID,
	int									size,
	char								*fileName,
	int									lineNumber
);

// use MemCheck_Free(*addr)
void _CF_Memory_Free
(
	CF_MODID							moduleID,
	void								*addr,
	char								*fileName,
	int									lineNumber
);

void CF_Memory_GetMemoryUsage(int *allocNum, int *freeNum, int *totalSize);

void CF_Memory_ReportMemory(CF_M_TRACELEVEL outputLevel);

#endif // _MEMCHECK_H_


