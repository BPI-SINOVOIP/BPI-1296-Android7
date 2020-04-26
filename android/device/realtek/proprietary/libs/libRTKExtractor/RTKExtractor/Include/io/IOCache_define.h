#ifndef __C_IOCACHE_DEFINE_H__
#define __C_IOCACHE_DEFINE_H__

#include <io/udfaccess/func.h>
#include <io/udfaccess/profile.h>
#include <io/IOCache.h>
#include <io/IOCache_buffer.h>

#define IOCACHE_MIN_CACHE_BUFFER_NUM 16
#define DVD_BLOCK_LEN 2048
//#define KEY_SHIFT 10 //for 1k
#define KEY_SHIFT 4 // for 16
#define BOUNDARY_of_IOCACHE_from_2K(x) ((((x)/IOCACHE_SIZE_2K)*IOCACHE_SIZE_2K)*DVD_BLOCK_LEN)

static inline __off64_t Cache_to_BufOffset(IOCACHE *pCache, __off64_t offset)
{
  if (Cache_to_Device(pCache)->deviceType)  // dvdreader_t->b_udf_fs_11
    return ((unsigned long long)(((fobject *)(pCache->fileHandle))->n->loc))<<32L | (offset & 0xffffffff);
  else
    return  offset;
}

static inline unsigned long long Cache_to_Key(IOCACHE *pCache, __off64_t offset)
{
  if (Cache_to_Device(pCache)->deviceType) // dvdreader_t->b_udf_fs_11
    return (((unsigned long long)(((fobject *)(pCache->fileHandle))->n->loc))<<32L | (offset & 0xffffffff))>>KEY_SHIFT;
  else
    return (((unsigned long long)(pCache->fileHandle & 0xffff))<<48L | (offset&0xffffffffffffL))>>KEY_SHIFT;
}

static inline unsigned long long Buf_to_Key(IOCACHE_DEVICE *pDevice, IOCACHE_BUFFER *pBuf)
{
  if (pDevice->deviceType) // dvdreader_t->b_udf_fs_11
    return pBuf->offset_2k>>KEY_SHIFT;
  else
    return (((unsigned long long)(pBuf->fileHandle & 0xffff))<<48L | (pBuf->offset_2k&0xffffffffffffL))>>KEY_SHIFT;
}

#endif
