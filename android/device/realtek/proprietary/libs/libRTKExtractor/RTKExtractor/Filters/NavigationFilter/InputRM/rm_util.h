#ifndef __RM_UTIL_H__
#define __RM_UTIL_H__

#include <stdio.h>
#include <stdlib.h>
#include "rm_reader.h"

#define RM_BYTE_ALIGN_FOR_ODIRECT_OPEN	4096	//for devices with sector size 4k(refer to bug 18414)
#define RM_BUFSIZE        				(8*RM_BYTE_ALIGN_FOR_ODIRECT_OPEN) 	/* transcode io should be 32k */

int rm_unpack16(unsigned char* pBuf, unsigned char* pRetVal);
int rm_unpack32(BYTE* pBuf, unsigned char* pRetVal);

int rm_util_jump(rm_reader_t* pReader, rm_data_info_t* pDataInfo, long long offset);
int rm_util_read_to_buffer(rm_reader_t* pReader, rm_data_info_t* pDataInfo);
int rm_util_seek_from_buffer(rm_reader_t* pReader, rm_data_info_t* pDataInfo, int offset);
int rm_util_read_from_buffer(rm_reader_t* pReader, rm_data_info_t* pDataInfo, unsigned char* pBuffer, int size);
int rm_util_read_string_from_buffer(rm_reader_t* pReader, rm_data_info_t* pDataInfo, unsigned char* pBuffer, int size);
int rm_util_read_array_from_buffer(rm_reader_t* pReader, rm_data_info_t* pDataInfo, unsigned long numElem, unsigned long elemSize, void** ppArr);


void rm_util_two_bytes_swap(const unsigned char* pBuf, int size);


#endif //__RM_UTIL_H__
