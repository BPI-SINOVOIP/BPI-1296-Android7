#ifndef _SI_DESCRIPTOR_H_
#define _SI_DESCRIPTOR_H_

#if defined(WIN32) | defined(DVR)
	#include "siTypes.h"
#else
	#include <si3/siTypes.h>
#endif




void SiDesc_FreeMultiString(SI_MULTIPLE_STRING *mString);
void SiDesc_FreeContentAdvisory(SI_CONTENT_ADVISORY *contentAdvisory);
void SiDesc_FreeCaption(SI_CAPTION_SERVICE *captionService);
void SiDesc_FreeLocation(SI_LOCATION *location);
void SiDesc_FreeCategory(SI_CATEGORICAL_CODE *categoricalCode);
void SiDesc_FreeAc3Audio(SI_AC3 *ac3Audio);
void SiDesc_FreeTimeShifted(SI_TIMESHIFTED_SERVICE_LIST *serviceList);

BOOL SiDesc_ParseMultiString(const UINT8* buf, INT16 len, SI_MULTIPLE_STRING* mString);

void SiDesc_Ca(const UINT8 *buf, UINT16 dLoopLen, CAT_CONTENT *catContent);
BOOL SiDesc_Location(const UINT8 *buf, UINT16 dLoopLen, SI_LOCATION *location);
BOOL SiDesc_ExtendedName(const UINT8 *buf, UINT16 dLoopLen, SI_MULTIPLE_STRING *mString);
BOOL SiDesc_Caption(const UINT8 *buf, UINT16 dLoopLen, SI_CAPTION_SERVICE *captionService);
BOOL SiDesc_ContentAdvisory(const UINT8 *buf, UINT16 dLoopLen, SI_CONTENT_ADVISORY *contentAdvisory);
BOOL SiDesc_Ac3Audio(const UINT8 *buf, UINT16 dLoopLen, SI_AC3 *ac3Audio);
BOOL SiDesc_CategoricalCode(const UINT8 *buf, UINT16 dLoopLen, SI_CATEGORICAL_CODE *categoricalCode);
BOOL SiDesc_ComponentName(const UINT8 *buf, UINT16 dLoopLen, SI_MULTIPLE_STRING *mString);
BOOL SiDesc_Language(const UINT8 *buf, UINT16 dLoopLen, char *langStr, int *type);
BOOL SiDesc_TimeShifted(const UINT8 *buf, UINT16 dLoopLen, SI_TIMESHIFTED_SERVICE_LIST *serviceList);

#endif
