#ifndef _SI_SECTION_H_
#define _SI_SECTION_H_

#if defined(WIN32) | defined(DVR)
	#include "siTypes.h"
	#include "siCache.h"
#else
	#include <si3/siTypes.h>
	#include <si3/siCache.h>
#endif




void SiSection_Free(PARSED_SECTION *parsedSection);
PARSED_SECTION *SiSection_Parse(TPMOD tpmod, SECTION_BUF *sectionBuf);
BOOL SiSection_IsValid(UINT8 *section);

#endif
