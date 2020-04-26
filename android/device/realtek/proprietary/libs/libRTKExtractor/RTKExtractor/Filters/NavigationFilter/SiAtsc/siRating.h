#ifndef _SI_RATING_H_
#define _SI_RATING_H_

#if defined(WIN32) | defined(DVR)
	#include "siTypes.h"
#else
	#include <si3/siTypes.h>
#endif


typedef enum _TEXT_ENCODING_CONVERT {

	TEXT_ENCODING_CONVERT_DEFAULT = 0,
	TEXT_ENCODING_CONVERT_TO_UTF8,
	TEXT_ENCODING_CONVERT_TO_UTF16
	
} TEXT_ENCODING_CONVERT;



BOOL SiRating_Init(TPMOD tpmod);
BOOL SiRating_DeInit(TPMOD tpmod);
BOOL SiRating_Reset(TPMOD tpmod);
BOOL SiRating_UpdateRrt(TPMOD tpmod, RRT_CONTENT **rrtContent, int size);
int SiRating_GetRrtSize(TPMOD tpmod, UINT8 regionId);

BOOL SiRating_GetString(TPMOD tpmod, SI_RATING_TEXT *ratingText, UINT8 regionId, UINT8 dimIndex, UINT8 valueIndex, BOOL useUtf8);
BOOL SiRating_GetMultiRating(TPMOD tpmod, SI_MULTI_RATING *multiRating, SI_CONTENT_ADVISORY *contentAdvisory, BOOL useUtf8);

BOOL SiRating_FreeString(SI_RATING_TEXT *ratingText);
BOOL SiRating_FreeMultiRating(SI_MULTI_RATING *multiRating);
BOOL SiRating_DupeMultiRating(SI_MULTI_RATING *dest, SI_MULTI_RATING *src);
BOOL SiRating_DupeMultiRatingEx(SI_MULTI_RATING *dest, SI_MULTI_RATING *src, TEXT_ENCODING_CONVERT encodingConvert);

BOOL SiRating_TableExist(TPMOD tpmod, UINT32 regionId);
BOOL SiRating_FreeDRRT(SI_DRRT_CONTENT *pDrrtContent);
BOOL SiRating_GetDRRT(TPMOD tpmod, SI_DRRT_CONTENT *pDrrtContent, UINT32 regionId, BOOL useUtf8);

#endif
