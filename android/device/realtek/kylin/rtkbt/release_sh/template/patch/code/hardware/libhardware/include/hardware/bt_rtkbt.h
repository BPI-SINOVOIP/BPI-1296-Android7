/*
 * Copyright (C) 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef ANDROID_INCLUDE_BT_RTKBT_H
#define ANDROID_INCLUDE_BT_RTKBT_H
#include <errno.h>
#include <stdint.h>
#include <strings.h>
#include <sys/cdefs.h>
#include <sys/types.h>

#include "hardware/bluetooth.h"

__BEGIN_DECLS
typedef int (* rtkbt_GenericEvent_func)(int id, int event, unsigned char * data, int len);
typedef int (* rtkbt_GenericCommand_func)(int id, int command, unsigned char * data, int len);
typedef int (* rtkbt_GetFeature_func)(int id);

/** RTKBT callbacks */
typedef struct {
    size_t size;
    rtkbt_GenericEvent_func onGenericEvent;
} rtkbt_callbacks_t;

/** RTKBT interface. */
typedef struct {
    size_t size;
    bt_status_t (*init)(rtkbt_callbacks_t* callbacks);
    void (*cleanup)( void );
    rtkbt_GetFeature_func GetFeature;
    rtkbt_GenericCommand_func GenericCommand;
} rtkbt_interface_t;

__END_DECLS

#endif
