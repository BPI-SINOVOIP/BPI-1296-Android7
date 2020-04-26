//#define LOG_NDEBUG 0
#undef LOG_TAG
#define LOG_TAG "togoOutCGI"

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <binder/MemoryBase.h>
#include <binder/MemoryHeapBase.h>
#include <binder/IServiceManager.h>
#include <utils/StrongPointer.h>
#include <RtkIpc/IRecordBuffer.h>
#include <cutils/log.h>
#include "ToGoService.h"

//pthread_t gStreamingThread;
static bool gbIsQuitStreaming = false;

namespace android {
struct StreamingThreadArgs {
	sp<IMemory> data;
	sp<IMemory> info;
};
}	// namespace android

using namespace android;

#define MAX_TOGO_INSTANCES	2
StreamingThreadArgs gThreadArgs;
/*
int getRecordBuffer(int handle, sp<IMemory> *data, sp<IMemory> *info)
{
	if (handle < 0 || handle >= MAX_TOGO_INSTANCES) {
		ALOGE("Invalid handle=%d.",handle);
		return -1;
	}

	static sp<IRecordBuffer> recordBuffer;
	
	if (recordBuffer == NULL)
	{
		sp<IServiceManager> sm = defaultServiceManager();
		sp<IBinder> binder;
		binder = sm->getService(String16("vendor.realtek.RecordBuffer"));
		if (binder != 0)
		{
			recordBuffer = IRecordBuffer::asInterface(binder);
		}
	}
	if (recordBuffer == NULL)
	{
		ALOGE("The buffer service is not published");
		return -1;
	}
	else
	{
		*data = recordBuffer->getBuffer(RECBUF_TYPE_DATA, handle);
		*info = recordBuffer->getBuffer(RECBUF_TYPE_INFO, handle);
		return 0;
	}
}
*/

int ParseCid(char *data, int dataLen)
{
	int cid = -1;
	int cidLen = 0;

	char* cidStr = strcasestr(data, "cid=");
	if (cidStr) {
		char* cidParam = cidStr + strlen("cid=");
		//ALOGV("cidStr=%s.cidParam=%s.",cidStr,cidParam);

		if ((cidParam+1) <= (data+dataLen)) {
			char* tmp = cidParam;
			while ((*tmp != '\0') && (*tmp != '&')) {
				//ALOGV("*tmp=%c.",*tmp);
				cidLen++;
				tmp++;
			}
			//ALOGV("cidLen=%d.",cidLen);

			if (cidLen == 1) {
				char tmp[2];
				memset(tmp, 0, sizeof(char)*2);
				strncpy(tmp, cidParam, cidLen);
				cid = atoi(tmp);

				if (cid != -1) {
					ToGoService *pToGo = ToGoService::GetInstance();										
					if (pToGo && (pToGo->isHandleLegal(cid) == 0)) {
						return cid;
					}
					else {
						ALOGE("Illegal cid=%d.",cid);
						return -1;
					}
					if (pToGo) {
						pToGo->ReleaseInstance();
					}								
				}	// if (cid != -1)
			}	// if (cidLen == 1)
		}	// if ((cidParam+1) <= (data+dataLen)
	}	// if (cidStr)

	return -1;
}

void SaveTogoPid(int cid)
{
	FILE* pFile = NULL;
	char filename[32];
	
	pid_t mypid = getpid();
	ALOGV("SaveTogoPid.cid=%d.mypid=%d.",cid,mypid);

	memset(filename, 0, sizeof(char)*32);
	snprintf(filename, 32, "/tmp/togopid-cid%d", cid);

	unlink(filename);
	
	if ((pFile = fopen(filename, "wb")) == NULL) {
		ALOGE("open %s NG.",filename);
		return;
	}
	fprintf(pFile, "%d", mypid);
	fclose(pFile);	
}

int GetTogoPid(int cid) {
	FILE* pFile = NULL;
	char buf[16];
	int mypid = -1;
	char filename[32];

	memset(filename, 0, sizeof(char)*32);
	snprintf(filename, 32, "/tmp/togopid-cid%d", cid);

	if ((pFile = fopen(filename, "rb")) == NULL) {
		ALOGE("open %s NG.",filename);
		return mypid;
	}
	memset(buf, 0, sizeof(char)*16);
	fread(buf, 1, 16, pFile);
	mypid = atoi(buf);

	ALOGV("GetTogoPid.mypid=%d.",mypid);
	unlink(filename);
	return mypid;
}

void KillStreamingProc(int cid)
{
	int mypid = GetTogoPid(cid);
	if (mypid != (-1)) {
		char cmd[16];
		memset(cmd, 0, sizeof(char)*16);
		sprintf(cmd, "kill -9 %d", mypid);
		ALOGV("exec cmd=%s.",cmd);
		system(cmd);
	}
}

void* StreamingThreadProc(void* args)
{
	ALOGV("enter StreamingThreadProc.stdout=%p.",stdout);
	uint32_t readPtr = 0;

	StreamingThreadArgs* pArgs = (StreamingThreadArgs*)args;
	struct BufferInfo_t* bufInfo = (struct BufferInfo_t *)(pArgs->info->pointer());
	sp<IMemory> data = pArgs->data;

	readPtr = bufInfo->writePtr;
	 
	while (!gbIsQuitStreaming) {
		uint32_t writePtr = bufInfo->writePtr;
		int size = 0;
		 
		if (writePtr >= readPtr) {
			size = writePtr - readPtr;
		}
		else {
			size = bufInfo->size - readPtr;
		}
		
		if (size <= 0){
			usleep(100000);
			continue;
		}

		if (bufInfo->packet_size > 0) {
			int *current_size = (int *)((char *)data->pointer() + readPtr);
		 
			if (current_size[0] > 0) {
				int cnt = fwrite((char *)data->pointer() + readPtr + 4 , 1, current_size[0], stdout);
				readPtr = (readPtr + 1504) % bufInfo->size;
			}
			else {
				ALOGE("!!!! size [%d] writePtr [%p] readPtr[%p] bufInfo->size[%p] [%x %x %x %x ]!!!!", size, writePtr, readPtr, bufInfo->size, current_size[0], current_size[1], current_size[2], current_size[3]);
			}
		}
		else {
			//ALOGV("before write data.ptr=%p.size=%d.",((char *)data->pointer() + readPtr),size);
			int cnt = fwrite((char *)data->pointer() + readPtr, 1, size, stdout);
			//ALOGV("after write data.ptr=%p.size=%d.cnt=%d.",((char *)data->pointer() + readPtr),size,cnt);
			readPtr = (readPtr + cnt) % bufInfo->size;
		}
	}
	
	//pthread_detach(pthread_self());
	gbIsQuitStreaming = false;
	ALOGV("leave StreamingThreadProc.");
	return NULL;
}

bool StartStreaming(int handle)
{
	ALOGV("enter StartStreaming.handle=%d.stdout=%p.",handle,stdout);
	
	sp<IMemory> data = NULL;
	sp<IMemory> info = NULL;

	if (getRecordBuffer(data, info, handle) != 0) {
		ALOGE("getRecordBuffer() NG.");
		return false;
	}
	ALOGE("data=%p.info=%p.",data.get(),info.get());

	struct BufferInfo_t *bufInfo = (struct BufferInfo_t *)info->pointer();
	if (bufInfo->magic != ASHMEM_RTK_MAGIC_NUMBER) {
		ALOGE("magic number is wrong.number=%d.ASHMEM_RTK_MAGIC_NUMBER=%d.",bufInfo->magic,ASHMEM_RTK_MAGIC_NUMBER);
		return false;
	}

	gbIsQuitStreaming = false;

	fprintf(stdout, "Content-type: video/mp2t\n\n");
	
	gThreadArgs.data = data;
	gThreadArgs.info = info;
	//pthread_create(&gStreamingThread, NULL, StreamingThreadProc, &gThreadArgs);
	StreamingThreadProc((void*)(&gThreadArgs));
	return true;
}

int main(void)
{
	//fprintf(stdout, "Content-type: text/html\n\n");
	//fprintf(stdout, "Hello, CGI!\n");
	//fprintf(stdout, "Content-type: video/mp2t\n\n");
	
	while (1) {
		char* data = getenv("QUERY_STRING");
		if (data != NULL) {
			int dataLen = strlen(data);
			//ALOGV("data=%s.dataLen=%d.",data,dataLen);

			int cmdLen = 0;
			char* cmdStr = strcasestr(data, "cmd=");
			if (cmdStr) {

				char* cmdParam = cmdStr + strlen("cmd=");
				//ALOGV("cmdStr=%s.cmdParam=%s.",cmdStr,cmdParam);

				if ((cmdParam+4) <= (data+dataLen)) {
					char* tmp = cmdParam;
					while ((*tmp != '\0') && (*tmp != '&')) {
						//ALOGV("*tmp=%c.",*tmp);
						cmdLen++;
						tmp++;
					}
					//ALOGV("cmdLen=%d.",cmdLen);

					if (cmdLen != 0) {
						int cid = -1;
						if (strncmp(cmdParam, "start", cmdLen) == 0) {
							ALOGV("cmd meets start");

							cid = ParseCid(data, dataLen);
							if (cid != (-1)) {
								SaveTogoPid(cid);
								if (StartStreaming(cid) == false) {
									ALOGE("StartStreaming() NG.");
									break;
								}
								else {
									ALOGV("start OK.cid=%d.",cid);
								}
							}
							else {
								ALOGE("No cid or incorrect!");
								break;
							}
						}	// if (strncmp(cmdParam, "start", cmdLen)
						else if (strncmp(cmdParam, "stop", cmdLen) == 0) {
							ALOGV("cmd meets stop");

							cid = ParseCid(data, dataLen);
							if (cid != (-1)) {
								KillStreamingProc(cid);
								ALOGV("stop OK.cid=%d.",cid);
								return 0;
							}
							else {
								ALOGE("No cid or incorrect!");
								break;
							}
						}	// else if (strncmp(cmdParam, "stop", cmdLen)
					}	// if (cmdLen != 0)
				}	// if ((cmdParam+5) <= (data+dataLen))
			}	// if (cmdStr)
		}
		else {
			ALOGV("No params.");
			return 0;
		}
		usleep(100000);
	}
	
	return 0;
}



