/*
 * If not stated otherwise in this file or this component's Licenses.txt file
 * the following copyright and licenses apply:
 *
 * Copyright 2019 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#ifndef __CCSP_RBUS_VALUE_CHANGE_H
#define __CCSP_RBUS_VALUE_CHANGE_H

#include <stdint.h>
#include <rbus-core/rbus_message.h>

typedef struct _rbusFilter* rbusFilter_t;

#ifdef __cplusplus
extern "C" {
#endif

int Ccsp_RbusValueChange_Subscribe(
    void* handle, 
    const char* listener, 
    const char* parameter, 
    rbusMessage payload);

int Ccsp_RbusValueChange_Unsubscribe(
    void* handle,
    const char* listener, 
    const char* parameter, 
    rbusMessage payload);

int Ccsp_RbusValueChange_Close(
    void* handle);

#ifdef __cplusplus
}
#endif
#endif
