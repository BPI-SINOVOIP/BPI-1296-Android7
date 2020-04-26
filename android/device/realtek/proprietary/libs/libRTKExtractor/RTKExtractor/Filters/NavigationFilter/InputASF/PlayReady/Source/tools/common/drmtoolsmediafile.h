/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRM_TOOLS_MEDIA_FILE_H__
#define __DRM_TOOLS_MEDIA_FILE_H__ 1

#include <drmtoolsasfparser.h>

typedef enum _tageDRM_MEDIA_FILE_TYPE
{
    eDRM_MEDIA_FILE_TYPE_UNKNOWN  = 0,
    eDRM_MEDIA_FILE_TYPE_ASF      = 1,
    eDRM_MEDIA_FILE_TYPE_ENVELOPE = 2
} eDRM_MEDIA_FILE_TYPE;

typedef struct _tagDRM_MEDIA_FILE_CONTEXT
{
    DRM_BOOL             fOpened;
    DRM_BOOL             fReadOnly;
    eDRM_MEDIA_FILE_TYPE eFileType;
    DRM_CONST_STRING     dstrFileName;
    DRM_BYTE             rgbDRMHeaderData[__CB_DECL( DRM_MAX_PLAYREADYOBJSIZE )];
    DRM_DWORD            cbDRMHeaderData;
    DRM_DWORD            ibDRMHeaderOffsetInFile;  /* Offset into the file where the DRM Header is */
    DRM_DECRYPT_CONTEXT  oDecryptContext;
    union
    {
        DRM_ENVELOPED_FILE_CONTEXT oEnvFile;
        DRM_TOOLS_ASF_CONTEXT      oAsfFile;
    } FormatContext;
} DRM_MEDIA_FILE_CONTEXT;

#define EMPTY_DRM_MEDIA_FILE_CONTEXT {                                \
                                        FALSE,                        \
                                        FALSE,                        \
                                        eDRM_MEDIA_FILE_TYPE_UNKNOWN, \
                                        EMPTY_DRM_STRING,             \
                                        { 0 },                        \
                                        0,                            \
                                        { 0 },                        \
                                        { 0 } }

DRM_RESULT DRM_CALL DRMTOOLS_OpenMediaFile( IN     DRM_CONST_STRING       *f_dstrFileName, 
                                               OUT DRM_MEDIA_FILE_CONTEXT *f_poMediaFile );

DRM_RESULT DRM_CALL DRMTOOLS_CloseMediaFile( IN OUT DRM_MEDIA_FILE_CONTEXT *f_poMediaFile );

DRM_RESULT DRM_CALL DRMTOOLS_SaveContentHeaderToFile( IN DRM_MEDIA_FILE_CONTEXT *f_poMediaFile,
                                                      IN DRM_CONST_STRING       *f_dstrOutFileName );


DRM_RESULT DRM_CALL DRMTOOLS_UpdatePlayReadyObject( IN DRM_MEDIA_FILE_CONTEXT *f_poMediaFile );

#endif /* __DRM_TOOLS_MEDIA_FILE_H__ */