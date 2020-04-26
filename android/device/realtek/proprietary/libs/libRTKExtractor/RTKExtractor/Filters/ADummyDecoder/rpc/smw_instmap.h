

#ifndef _MIDDLEWARE_INSTANCE_MAP_H_
#define _MIDDLEWARE_INSTANCE_MAP_H_

#include "Filters/AudioFilter_c/CAudioFilter.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define ADDTO_FILTERMAP(filter,instanceID)			AddToFilterMap(filter,instanceID)
#define DELETEFROM_FILTERMAP(instanceID)			DeleteFromFilterMap(instanceID)

void			AddToFilterMap(CAudioFilter* pFilter, long instanceID);
void			DeleteFromFilterMap(long instanceID);
CAudioFilter*	MapToFilter(long instanceID);

#ifdef  __cplusplus
}
#endif

#endif











































