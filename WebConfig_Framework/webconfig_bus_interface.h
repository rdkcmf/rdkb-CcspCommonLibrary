/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2015 RDK Management
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


#ifndef  _COSA_WEBCONFIG_BUS_INTERFACE_H
#define  _COSA_WEBCONFIG_BUS_INTERFACE_H

#include<rbus.h>
#include "webconfig_framework.h"
#ifdef WBCFG_MULTI_COMP_SUPPORT

void EventRegister(char* EventName);
int subscribeToEvent(char* EventName);
void UnregisterFromEvent(char* EventName);
void sendDataToEvent(char* event_name , char* data);
void eventRegisterSlave();
void eventRegisterMaster();

void UnSubscribeFromEvent(char* EventName);

void multiComp_callbk_rbus(rbusHandle_t handle, rbusEvent_t const* event, rbusEventSubscription_t* subscription);
rbusError_t eventSubHandler(rbusHandle_t handle, rbusEventSubAction_t action, const char* eventName, rbusFilter_t filter, int32_t interval, bool* autoPublish);
void multiComp_callbk_rbus(rbusHandle_t handle, rbusEvent_t const* event, rbusEventSubscription_t* subscription);

#endif

#endif