#ifndef __UTIL_H__
#define __UTIL_H__

/* UTIL - Include ALL Shared Utilities */
#include <stdio.h>

/* useful macro */
#define HAS_FLAG(flag, bit)    ((flag) & (bit))
#define SET_FLAG(flag, bit)    ((flag)|= (bit))
#define RESET_FLAG(flag, bit)  ((flag)&= (~(bit)))
#define XOR(p1, p2)            (((p1) && (!(p2))) || ((!(p1)) && (p2)))

/* debug utilities */
#include "UTIL/ulDebug.h"

#ifndef DEFAULT_WATCHDOG_TIMEOUT_THRESHOLD
#define INFINITY_WATCHDOG_TIMEOUT_THRESHOLD -1
#define DEFAULT_WATCHDOG_TIMEOUT_THRESHOLD  50
#define EXTENDED_WATCHDOG_TIMEOUT_THRESHOLD 100
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/mman.h>

long getRootVersion(void);
void setWatchDogTimeoutValue(int value);
void setTimeoutValue(int value);

int communicateToWatchDog(void);


void *getMemoryFromVideo(int *size);
void releaseMemoryFromVideo(void *ptr);

#ifdef THUNDER
char *Thunder_getlicense();
int GetThunderExitFlag();
void SetThunderExitFlag(int flag);
#endif

#ifdef VOOLETV
void GetVooleTVSpeed(double realtime_speed, double average_speed);
#endif

#ifdef PPTV
char *PPLive_getCustomID();
char *PPLive_getProgramID();
char *PPLive_getAuthNum();
#endif

#ifdef STREAM_PPS_IOPLUGIN
char *PPS_getOEM();
char *PPS_getProductName();
#endif

int getExtendedMID(char * string, int len);
int DecryptDataFromFile(char *inFileName, unsigned char *pKey, char **pDecrypted);
int GetParameterFromSystemParametersFile(char* str, char* par);
#ifdef __cplusplus
}
#endif




#ifdef __cplusplus

/* template data manipulators */
#include "UTIL/ulDArray.h"
#include "UTIL/ulQueue.h"

/* index shuffler */
#include "UTIL/shuffle.h"

/* discStreamFormat detection */
#include <StreamClass/EType.h>

typedef struct{
    ENUM_MEDIA_TYPE             majortype;
    ENUM_MEDIA_TYPE             subtype;
} DATA_MEDIA_TYPE;

typedef struct {
	int				mediaType;	/* MEDIA_VIDEO/MEDIA_AUDIO/MEDIA_IMAGE (refer to EType.h) */
	ENUM_MEDIA_TYPE fileType;	/* could be used as preDeterminedType for LoadMedia() */
	ENUM_MEDIA_TYPE ioType;		/* to indicate I/O method, depends on getDataFormat() */
} SUGGESTED_MEDIA_TYPE;

// copy from VideoRPC_System.x
typedef struct 
{
  long address ;   /* base address of a contiguous memory chunk */
  long size ;      /* size of the memory chunk */
} VIDEO_MEM_CHUNK_STRUCT;

typedef struct {
    long  numMemChunks;
    VIDEO_MEM_CHUNK_STRUCT memChunk[8] ;
} VIDEO_FREEMEMORY_STRUCT;

typedef enum {
    FILE_SYSTEM_TYPE_NONE,
    FILE_SYSTEM_TYPE_FAT,
    FILE_SYSTEM_TYPE_NTFS
} FileSystemType;

// Function logInfo: function put the "info" parameter to log
// info: NULL terminated string
void logInfo(char *info);

long print_time(char *str, int reset = 0);
int CheckDVDDisc(int profile);
bool GetFirstPossibleDvdDir(const char* pStrInitFolder, char* pResultBuf, int iResultBufSize);
int getDataFormat(char *pPath, DATA_MEDIA_TYPE *pMT);
bool isNetMediaType(char *media_path);
int VP_Request_Buffer(unsigned int width, unsigned int height);
int VP_Release_Buffer(int bufferID);
VIDEO_FREEMEMORY_STRUCT VP_VideoMemory_Config(int configMode);
HRESULT VideoOutCapture(int osdColorFormat, unsigned long phyAddr, u_short imgPitch, u_short startX, u_short startY, u_short width, u_short height);
/*!
@Return :
-1  : fail
2   : Successful
1   : already Exist
*/
int Mkdir(char *cdir,int mode);

int setWin32Hide(char *mount_point,char *name,bool isFolder);

int getFsFromFile(char *file,FileSystemType *fs_type);

bool IsHiddenFile(char* pPath, FileSystemType fsType);

bool SetHiddenAttr(char* pPath, FileSystemType fsType);

char *ufgets(char *s, int size, FILE *stream, char charset);

void GetSuggestedMediaType(char *pathname, SUGGESTED_MEDIA_TYPE &type, bool bGetDataFormat = false);

unsigned long GetVideoVersion(void);
unsigned long GetAudioVersion(void);

#if defined (SUPPORT_WMDRM_FLAG) || defined (SUPPORT_PLAYREADY_FLAG)
bool initWMDRM();
#endif

// Covert the string in pStrBuf to lower case.
// Created by Zack.
//
//  Parameters:
//      1. pStrBuf: The string buffer wants to be converted.
//
//  Return:
//      The original buffer pStrBuf with converted string.
// 
char* strlwr(char* pStrBuf);

// Covert the string in pStrBuf to upper case.
// Created by Zack.
//
//  Parameters:
//      1. pStrBuf: The string buffer wants to be converted.
//
//  Return:
//      The original buffer pStrBuf with converted string.
// 
char* strupr(char* pStrBuf);

#ifdef USE_SWF
typedef struct {
	bool		 isCompressed;
	int			 version;
	unsigned int fileLength;
	int			 width;
	int			 height;
	float		 frameRate;
	int			 frameCount;
} SWFFILEHEADER;

bool GetSWFFileHeader(char* file, SWFFILEHEADER &header);
#endif

#endif

#endif /*__UTIL_H__*/
