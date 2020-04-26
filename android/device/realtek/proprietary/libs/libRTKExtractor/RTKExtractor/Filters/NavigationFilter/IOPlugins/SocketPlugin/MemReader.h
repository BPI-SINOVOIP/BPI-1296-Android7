
#ifndef __SHARED_MEMORY__READER_H__
#define __SHARED_MEMORY__READER_H__

#include <Platform_Lib/SocketAVData/SocketAVData.h>
#include <Filters/NavigationFilter/NavPlugins.h>

#define MEM_READER_BACKUP_BUF_SIZE (96*1024)

typedef struct _tagIOPlugin_Mem
{
	SOCKET_SHARED_MEM* pMemHeader;
	SOCKET_DATA_HEADER dataHeader;
	unsigned char*		pBuffer;
	long				offsetToNonCach;
	unsigned long		updatedReadPtr;
	long				remainSize;
	long				backupBufSize;
} IOPlugin_Mem;

int Mem_Reader_GetReadPtr(IOPlugin_Mem* pInstance, unsigned char** pBuffer, int size, NAVBUF* pEventReceived);
int Mem_Reader_Read(IOPlugin_Mem* pInstance, unsigned char* pBuffer, int size, NAVBUF* pEventReceived);
void Mem_Reader_Close(IOPlugin_Mem* pInstance);
void Mem_Reader_Reset(IOPlugin_Mem* pInstance);

#endif //__SHARED_MEMORY__READER_H__
