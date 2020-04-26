#ifndef __C_IOCACHE_IF_H__
#define __C_IOCACHE_IF_H__

#include <io/IOCache_buffer.h>
#include <Utility/hash.h>
#include <OSAL.h>

#include <io/dvd_input.h>
#include <io/dvd_reader.h>

#ifdef __cplusplus
extern "C" {
#endif

/* interface to dvdaccess */
int DVDCacheInit(dvd_reader_t *dvd, int readCache,  int BufferNum);
int DVDCacheRelease(dvd_reader_t *dvd, int readCache,  int BufferNum);
int DVDCacheRelease_by_handle(int pCacheHandle, int readCache,  int BufferNum);
int DVDCacheBufRelease(dvd_file_t *dvd_file, __off64_t offset_2k, unsigned char *pBuf);
unsigned char *DVDCacheBufRequest(dvd_file_t *dvd_file, __off64_t offset_2k);
int DVDCacheFlushDevice(dvd_reader_t *dvd, int readCache);
int DVDCacheReadBlocks( dvd_file_t *dvd_file, __off64_t offset_2k,
			      size_t block_count, unsigned char **ppData);
#if 0
int DVDCacheWriteVOBBlocks( dvd_file_t *dvd_file,
                              size_t block_count, unsigned char *pData);
ssize_t DVDCacheWriteVROBlocks( dvd_file_t *dvd_file, unsigned char *data, size_t block_count );
#endif /* 0 */

char DVDCacheSetDirty(dvd_reader_t *dvd, int readCache);

#ifdef __cplusplus
}
#endif

#endif

