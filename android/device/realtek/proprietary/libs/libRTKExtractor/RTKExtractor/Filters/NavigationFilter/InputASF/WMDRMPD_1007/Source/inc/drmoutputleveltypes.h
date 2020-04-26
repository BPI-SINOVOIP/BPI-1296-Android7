/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRM_OUTPUTLEVELTYPES_H__
#define __DRM_OUTPUTLEVELTYPES_H__

#ifdef __cplusplus
extern "C" {
#endif

enum DRM_OPL_CALLBACK_TYPE
{
    DRM_PLAY_OPL_CALLBACK = 0x1,
    DRM_COPY_OPL_CALLBACK = 0x2,
    DRM_INCLUSION_LIST_CALLBACK = 0x3
};

#define DRM_MAX_INCLUSION_GUIDS 20

typedef DRM_RESULT (DRM_API* DRMPFNOUTPUTLEVELSCALLBACK)( 
    IN const DRM_VOID  *f_pvOutputLevelsData,
    IN       DRM_DWORD  f_dwCallbackType,
    IN const DRM_VOID  *f_pv );

/*
**  When recieving this struct via a callback you can determine which GUIDs in the list are usabale by checking the 
**  rgfInclusionListValid[dwChainDepth] array to see which ones are TRUE.  Those that are true means that
**  rgInclusionList[x] is usable where rgfInclusionListValid[dwChainDepth][x] is TRUE.
*/
typedef struct __tagDRM_INCLUSION_LIST_CALLBACK_STRUCT
{
    DRM_GUID  rgInclusionList       [DRM_MAX_INCLUSION_GUIDS];
    DRM_BOOL  rgfInclusionListValid [DRM_MAX_INCLUSION_GUIDS][DRM_MAX_LICENSE_CHAIN_DEPTH];
    DRM_DWORD dwChainDepth;
} DRM_INCLUSION_LIST_CALLBACK_STRUCT;

#ifndef DRM_OPL_TYPES
#define DRM_OPL_TYPES 1

typedef struct __tagDRM_MINIMUM_OUTPUT_PROTECTION_LEVELS
{
    DRM_WORD wCompressedDigitalVideo;
    DRM_WORD wUncompressedDigitalVideo;
    DRM_WORD wAnalogVideo;
    DRM_WORD wCompressedDigitalAudio;
    DRM_WORD wUncompressedDigitalAudio;
    
} DRM_MINIMUM_OUTPUT_PROTECTION_LEVELS;

typedef struct __tagDRM_OPL_OUTPUT_IDS
{
    DRM_WORD  cIds;
    DRM_GUID  *rgIds;
    
} DRM_OPL_OUTPUT_IDS;

typedef struct __tagDRM_VIDEO_OUTPUT_PROTECTION
{
    DRM_GUID guidId;
    DRM_BYTE bConfigData;

} DRM_VIDEO_OUTPUT_PROTECTION;

typedef struct __tagDRM_VIDEO_OUTPUT_PROTECTION_IDS
{
    DRM_WORD cEntries;
    DRM_VIDEO_OUTPUT_PROTECTION *rgVop;

} DRM_VIDEO_OUTPUT_PROTECTION_IDS;

typedef struct __tagDRM_PLAY_OPL
{
    DRM_MINIMUM_OUTPUT_PROTECTION_LEVELS minOPL;
    DRM_OPL_OUTPUT_IDS                   oplIdReserved;
    DRM_VIDEO_OUTPUT_PROTECTION_IDS      vopi;

} DRM_PLAY_OPL;

typedef struct __tagDRM_COPY_OPL
{
    DRM_WORD           wMinimumCopyLevel;
    DRM_OPL_OUTPUT_IDS oplIdIncludes;
    DRM_OPL_OUTPUT_IDS oplIdExcludes;
    
} DRM_COPY_OPL;

#endif


#ifdef __cplusplus
}
#endif


#endif /* __DRM_OUTPUTLEVELS_H__ */
