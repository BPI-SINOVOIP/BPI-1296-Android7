#ifndef _VIDEO_FILTER_INSTANCE_MAP_H_
#define _VIDEO_FILTER_INSTANCE_MAP_H_

#include <StreamClass/CFilter.h>
#include <VideoRPCBaseDS_data.h>

// Define max value of Video Filters to Agents mapping 
#define VIDEO_PROCESSOR_MAP_SIZE 20

#ifdef  __cplusplus
extern "C" {
#endif

void VP_AddToFilterMap(CBaseFilter* pFilter, long instanceID);
CBaseFilter* VP_MapToFilter(long instanceID);
void VP_DeleteFromFilterMap(long instanceID);

#ifdef  __cplusplus
}
#endif

#endif // _VIDEO_FILTER_INSTANCE_MAP_H_

