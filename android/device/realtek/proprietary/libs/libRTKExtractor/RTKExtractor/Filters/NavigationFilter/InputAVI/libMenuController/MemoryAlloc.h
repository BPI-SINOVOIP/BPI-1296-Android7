/**
@file
@verbatim

$Id: MemoryAlloc.h 2614 2006-02-23 01:39:23Z sbramwell $

Copyright (c) 2002-2006 DivX, Inc. All rights reserved.

This software is the confidential and proprietary information of DivX,
Inc. and may be used only in accordance with the terms of your license from
DivX, Inc.

@endverbatim
*/

#ifndef LMC_MEMALLOC_H
#define LMC_MEMALLOC_H

typedef struct _MemBuffer
{
    unsigned char * mem;
    unsigned int size;
    unsigned int used;
    unsigned int alignment;
} MemBuffer;

void InitBuffer(MemBuffer *memPool,unsigned char* mem, unsigned int size, unsigned int alignment);
unsigned char* CloseBuffer(MemBuffer *memPool);
unsigned char* Alloc(MemBuffer *memPool, unsigned int size);

#endif //LMC_MEMALLOC_H
