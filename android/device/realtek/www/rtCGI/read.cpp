/*
 * read.cpp
 *
 *  Created on: Apr 12, 2014
 *      Author: charleslin
 */
//#define LOG_NDEBUG 0
#undef LOG_TAG
#define LOG_TAG "read.fcgi"
#include <utils/Log.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "fcgiapp.h"
#include "fcgimisc.h"

#include <binder/IServiceManager.h>
#include <binder/MemoryBase.h>
#include <binder/MemoryHeapBase.h>
#include <media/stagefright/foundation/ADebug.h>

#include "RtkIpc/RecordBufferService.h"

#define LISTENSOCK_FILENO 0
#define LISTENSOCK_FLAGS 0

using namespace android;

int quit = 0;

void sighandler(int num)
{
    ALOGW("got signal:%d\n", num);
    if(num == SIGINT || num == SIGTERM || num == SIGPIPE || num == SIGUSR1)
        quit = 1;
}

sp<IMemory> data;
sp<IMemory> info;
void *fifoproc(void *)
{
    uint32_t readPtr;
    int handle = 0;
    char fifofn[128];
    sprintf(fifofn, "/tmp/togo-%d", handle);
    int fifofd = open(fifofn, O_WRONLY);
    if(fifofd == -1){
        ALOGE("open %s failed:%s", fifofn, strerror(errno));
        return NULL;
    }


    /* clear IMemory will cause BpMemoryHeap binder transaction fail */
    //data.clear();
    //info.clear();
    if(data == NULL && info == NULL)
    {
        if(getRecordBuffer(data, info, handle) != NO_ERROR){
            ALOGE("Can't getRecordBuffer");
            return NULL;
        }

        ALOGD("data:%p:%d info:%p:%d", data->pointer(), data->size(), info->pointer(), info->size());
        CHECK(data->pointer() != NULL && data->size() > 0 && info->pointer() != NULL && info->size() > 0);
    }

    struct BufferInfo_t *bufInfo = (struct BufferInfo_t *)info->pointer();
    CHECK_EQ(bufInfo->magic, ASHMEM_RTK_MAGIC_NUMBER);

    readPtr = bufInfo->writePtr;

    //streaming loop
    quit = 0;
    while(!quit &&
          checkRecordBufferService() == true &&
          bufInfo->endOfStream == false){
        uint32_t writePtr = bufInfo->writePtr;
        int size;
        int cnt = 0;

        if(writePtr >= readPtr)
            size = writePtr - readPtr;
        else
            size = bufInfo->size - readPtr;

        if(size <= 0){
            usleep(100000);
            continue;
        }

        if (bufInfo->packet_size > 0)
        {
            int *current_size = (int *)((long)data->pointer() + (long)readPtr);

            if (current_size[0] > 0)
            {
                cnt = write(fifofd, (char *)data->pointer() + readPtr + 4, current_size[0]);
                readPtr = (readPtr + 1504) % bufInfo->size;
            }
            else
                ALOGI("!!!! size [%d] writePtr [0x%08x] readPtr[0x%08x] bufInfo->size[0x%08x] [%x %x %x %x ]!!!!", size, writePtr, readPtr, bufInfo->size, current_size[0], current_size[1], current_size[2], current_size[3]);
        }
        else
        {
            cnt = write(fifofd, (char *)data->pointer() + readPtr, size);
            readPtr = (readPtr + cnt) % bufInfo->size;
            bufInfo->readPtr = readPtr;
        }
        ALOGD("size:%d readPtr:%u writePtr:%u", cnt, readPtr, writePtr);
    }

    close(fifofd);
    ALOGD("exit: quit:%d alive:%d eos:%d", quit, checkRecordBufferService(), bufInfo->endOfStream);
    return NULL;
}

int main(int argc, char *argv[])
{
    ALOGV("Hey! I'm a %scgi process", FCGX_IsCGI() ? "" : "fast");

    int err = FCGX_Init(); /* call before Accept in multithreaded apps */
    if(err){
        ALOGE("FCGX_Init failed: %d", err);
        return 1;
    }

    FCGX_Request cgi;
    err = FCGX_InitRequest(&cgi, LISTENSOCK_FILENO, LISTENSOCK_FLAGS);
    if(err){
        ALOGE("FCGX_InitRequest failed: %d", err);
        return 2;
    }

    //signal(SIGINT, sighandler);
    //signal(SIGTERM, sighandler);
    //signal(SIGUSR1, sighandler);    //graceful process shutdown
    signal(SIGPIPE, sighandler);    //client aborts a connection

    while (1) { //fast cgi loop
      err = FCGX_Accept_r(&cgi);
      if(err){
          ALOGE("FCGX_Accept_r stopped: %d", err);
          break;
      }

      int handle = 0;
      char fifofn[128];
      sprintf(fifofn, "/tmp/togo-%d", handle);
      mkfifo(fifofn, 0644);

      FCGX_FPrintF(cgi.out, "X-Sendfile: %s\r\n", fifofn);
      FCGX_FPrintF(cgi.out, "Content-type: video/mp2t\r\n\r\n");
      pthread_t thread;
      pthread_create(&thread, NULL, fifoproc, NULL);
    }

    return 0;
}
