/*
 * IRecordBuffer.h
 *  Created on: 19 mars 2010  Author: Zingo Andersen
 *  License: Public Domain (steal and use what you like)
 *
 * Buffer classes to handle the binder communication  */

#ifndef IRECORDBUFFER_H_
#define IRECORDBUFFER_H_

#include <utils/RefBase.h>
#include <binder/IInterface.h>
#include <binder/Parcel.h>
#include <binder/IMemory.h>
#include <utils/Timers.h>

namespace android {

/* Current BufferInfo size is 32 Byte defined in recordcamera.ccp */
struct BufferInfo_t {
    uint32_t magic;
    uint32_t size;
    uint32_t writePtr;
    uint32_t packet_size;   // When we used RTP over UDP, need read this packet_size everytime
    bool endOfStream;
    //uint32_t beginAddr;
    //uint32_t bufferID;
    //uint32_t numOfReadPtr;
    //uint32_t reserve2;
    //uint32_t reserve3;
    uint32_t readPtr;//[4];
    //int32_t fileOffset;
    //int32_t requestedFileOffset;
    //int32_t bSeekable;
};

enum {
    RECBUF_TYPE_DATA,
    RECBUF_TYPE_INFO
};

#define ASHMEM_RTK_MAGIC_NUMBER		0x1d938eba

class IRecordBuffer: public IInterface
{
public:
  DECLARE_META_INTERFACE(RecordBuffer);
  virtual sp<IMemory>               getBuffer(int id, int handle) = 0;
  virtual bool                      isAlive() = 0;
};

/* --- Server side --- */

class BnRecordBuffer: public BnInterface<IRecordBuffer>
{
public:
  virtual status_t    onTransact( uint32_t code,
                                  const Parcel& data,
                                  Parcel* reply,
                                  uint32_t flags = 0);
};

sp<IRecordBuffer> getRecordBufferAgent(void);
int getRecordBuffer(sp<IMemory> &data, sp<IMemory> &info, int handle = 0);
bool checkRecordBufferService();

}; // namespace android

#endif /* IRECORDBUFFER_H_ */
