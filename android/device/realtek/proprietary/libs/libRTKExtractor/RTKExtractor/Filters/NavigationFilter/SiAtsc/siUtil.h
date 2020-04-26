#ifndef _SI_UTIL_H_
#define _SI_UTIL_H_

#if defined(DVR)
	#include "siTypes.h"
	#include <OSAL/PLI.h>
#elif defined(WIN32)
	#include "siTypes.h"
#else
	#include <si3/siTypes.h>
#endif

void SiUtil_PrintError(char *str, ...);
void SiUtil_PrintWarning(char *str, ...);
void SiUtil_PrintInfo(char *str, ...);
void SiUtil_Verify(char *str, ...);
void SiUtil_SendWinMsg(char *str, ...);
void SiUtil_Log(char *str, ...);

void SiUtil_Sleep(int mSec);

//void SiMalloc(void **p, int size) ;
void *SiRealloc(void* ptr, int size);
#define SI_FREE(pt)					if (pt) {free(pt); (pt) = NULL;}


int SiUtil_IsVideoScrambled(TPMOD tpmod, UINT16 pid);
BOOL SiUtil_GetDtvFreqTable(TPMOD tpmod, PHY_INFO **freqTable, int *tableSize);
BOOL SiUtil_InputTokenizer(INT8 *des1, INT8 *des2, INT8 *src, char token);
BOOL SiUtil_IsDaylightSaving(TPMOD tpmod, BOOL *pIsDaylightSaving, UINT32 *pDsTime);
BOOL SiUtil_ConvertIntToDateStr(UINT8* dateStr, int intTime);
BOOL SiUtil_ConvertIntToDate(SI_EVENT_DATETIME* date, int intTime);
void SiUtil_GetTimeStr(UINT8 *dateStr, UINT32 time);
int SiUtil_GetTid(UINT8 tid);
SI_LANG SiUtil_GetLangId(UINT8 *langStr);

#ifdef DVR
BOOL SiUtil_Utf16ToUtf8(UINT8 *srcBuf, int srcLen, UINT8 **dstBuf, int *dstBufLen);
BOOL SiUtil_Utf8ToUtf16(UINT8 *srcBuf, int srcLen, UINT8 **dstBuf, int *dstBufLen);
#endif

#endif

