/*
// $Id: $
// Copyright (c) 2005 DivX, Inc. http://www.divx.com/corporate
// All rights reserved.
//
// This software is the confidential and proprietary information of DivX
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivX, Inc.
*/
#ifndef _DIVXPORTABLEAPI_H
#define _DIVXPORTABLEAPI_H

/*------------------------------------------------------------------------------------------
 *
 *  LOCAL INCLUDES
 *
 *-----------------------------------------------------------------------------------------*/
#include "DivXPortableCryptoAPI.h"
#include "DivXInt.h"
#include "DivXNULL.h"


/*------------------------------------------------------------------------------------------
 *
 *  TYPE DECLARATIONS
 *
 *-----------------------------------------------------------------------------------------*/
typedef int32_t (*portableFPtr) ( uint32_t *handle );
struct moduleDescriptor_t
{
    portableFPtr initializeFPtr;
    portableFPtr finalizeFPtr;
    uint32_t handle;
};
typedef struct moduleDescriptor_t moduleDescriptor;


/*------------------------------------------------------------------------------------------
 *
 *  MACROS
 *
 *-----------------------------------------------------------------------------------------*/

#define DIVX_MAX_MODULES 10

/*
 *  USER MACROS:  Macros users should use to utilize this API.
 */
#define DIVX_PORTABLE_ADD_MODULE(name) { DivXPortable##name##Initialize, DivXPortable##name##Finalize, 0 },
#define DIVX_PORTABLE_MODULES_BEGIN moduleDescriptor PORTABLE_MODULES_ARRAY_NAME[DIVX_MAX_MODULES] = {
#define DIVX_PORTABLE_MODULES_END { NULL, NULL, 0 } };

/*
 *  UTILITY MACROS:  Macros users can use to help navigate the modules.
 */
#define PORTABLE_MODULES_ARRAY_NAME g_divxPortableModules
#define NUMBER_OF_PORTABLE_MODULES (sizeof(PORTABLE_MODULES_ARRAY_NAME)/sizeof(moduleDescriptor))

#define DECLARE_DIVX_PORTABLE_MODULE extern moduleDescriptor PORTABLE_MODULES_ARRAY_NAME[DIVX_MAX_MODULES];


/*------------------------------------------------------------------------------------------
 *
 *  FUNCTION DECLARATIONS
 *
 *-----------------------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

int32_t DivXPortableInitialize ( );
int32_t DivXPortableFinalize ( );

#ifdef __cplusplus
};
#endif

#endif

