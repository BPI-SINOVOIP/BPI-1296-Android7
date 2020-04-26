#ifndef __S_TYPE_H__
#define __S_TYPE_H__

#include <OSAL/Types.h>
#include "EType.h"

#define MAX_STREAM_NAME_LENGTH          128
#define MAX_FILTER_NAME_LENGTH          MAX_STREAM_NAME_LENGTH
#define MAX_PIN_NAME_LENGTH             MAX_STREAM_NAME_LENGTH


class IFlowManager;
class IBaseFilter;

typedef struct{
    char                        name[MAX_FILTER_NAME_LENGTH];
    IFlowManager*               pFM;
} FILTER_INFO;

typedef struct{
    IBaseFilter*                pFilter;
    PIN_DIRECTION               dir;
    char                        achName[MAX_PIN_NAME_LENGTH];
} PIN_INFO;

typedef struct{
    BYTE*                       pBuffer;
    ULONG                       nLen;
} Buffer_T;


typedef struct{
    ENUM_MEDIA_TYPE             majortype;
    ENUM_MEDIA_TYPE             subtype;
    long                        lSampleSize;
    FORMAT_TYPE                 formatType;
    Buffer_T                    format;

    // BYTE                        *pbFormat;
    // ULONG                       cbFormat;
} MEDIA_TYPE;

typedef struct{
    MEDIA_TYPE                  mediaType;
    long                        unitPerSecond;
} MEDIA_INFO;

typedef struct{
    // Buffer_T                    BeginBuf;
    BYTE*                       BeginBuf;
    BYTE*                       pNonCachedBuffer;
    unsigned long               bufferPhyAddr;
    Buffer_T                    ActualBuf;
} SAMPLE_INFO;

typedef struct{
    long                        numberOfBuffers;
    long                        bufferSize;
    long                        align;
    long                        prefix;
    // ALLOCATOR_STYLE             style;
} ALLOCATOR_PROPERTIES;

typedef unsigned long                  REFERENCE_TIME;

typedef long                    EVId;

typedef struct{ // all parameter use 256 as 1X, MinSkip 15x256 means skip
  long                        MaxSpeed;
  long                        MaxReverseSpeed;
  long                        MinSkip;          
  long                        MaxSkip;
  long                        MinReverseSkip;
  long                        MaxReverseSkip;
} FILTER_SEEKING_CAP;

template <class T>
struct Array_T{
    T*                          pArray;
    int                         nElement;
};

typedef struct{
    void*                       pInfo;
    long                        nLen;
} VoidInfo_S;

#endif
