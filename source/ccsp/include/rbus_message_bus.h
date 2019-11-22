/*
* If not stated otherwise in this file or this component's Licenses.txt file the
* following copyright and licenses apply:
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

/**********************************************************************

        For Dbus base library Implementation,
        Common Component Software Platform (CCSP)

    ---------------------------------------------------------------

    environment:

            platform dependent

     ---------------------------------------------------------------

     author:

            Harikrishnan Raman

     ---------------------------------------------------------------

     revision:

            11/22/2019    initial revision.

**********************************************************************/

#ifdef _RBUS_NOT_REQ_
#define rtMessage                                         int
#define rbus_error_t                                      int
#define rtError                                           int
#define rbus_callback_t                                   int
#define METHOD_INVALID                                   "DUMMY"
#define METHOD_SETPARAMETERVALUES                        "DUMMY"
#define METHOD_GETPARAMETERVALUES                        "DUMMY"
#define METHOD_GETPARAMETERNAMES                         "DUMMY"
#define METHOD_SETPARAMETERATTRIBUTES                    "DUMMY"
#define METHOD_GETPARAMETERATTRIBUTES                    "DUMMY"
#define METHOD_COMMIT                                    "DUMMY"
#define METHOD_ADDTBLROW                                 "DUMMY"
#define METHOD_DELETETBLROW                              "DUMMY"
#define METHOD_RPC                                       "DUMMY"
#define METHOD_RESPONSE                                  "DUMMY"
#define METHOD_MAX                                       "DUMMY"
#define MAX_METHOD_NAME_LENGTH                           "DUMMY"
#define RBUS_SMGR_DESTINATION_NAME                       "DUMMY" 
#define RBUS_SMGR_METHOD_GET_CURRENT_SESSION_ID          "DUMMY" 
#define RBUS_SMGR_METHOD_REQUEST_SESSION_ID              "DUMMY" 
#define RBUS_SMGR_METHOD_END_SESSION                     "DUMMY"
#define RTMESSAGE_BUS_SUCCESS                               0
#define RT_OK                                               0
#define RT_ERROR                                            1
#define rtMessage_Create(a)                                 0
#define rtMessage_Release(a)                                0
#define rbus_openBrokerConnection(x)                        0
#define rbus_openBrokerConnection2(x, y)                    0
#define rbus_closeBrokerConnection(x)                       0
#define rbus_registerObj(x, y, z)                           0
#define rbus_unregisterObj(x)                               0
#define rbus_addElement(x, y)                               0
#define rbus_removeElement(x, y)                            0
#define rbus_registerMethod(x, y, z, a)                     0
#define rbus_unregisterMethod(x, y)                         0
#define rbus_registerMethodTable(x, y, z)                   0
#define rbus_unregisterMethodTable(x, y, z)                 0
#define rbus_invokeRemoteMethod(x, y, z, a, b)              0
#define rbus_invokeRemoteMethodAsync(x, y, z, a, b)         0
#define rbus_publishEvent(x, y, z)                          0
#define rbus_registerEvent(x, y)                            0
#define rbus_unregisterEvent(x, y)                          0
#define rbus_registerTimedUpdateEventCallback(x, y, z)      0
#define rbus_subscribeToEvent(x,  y, z, a)                  0
#define rbus_unsubscribeFromEvent(x, y)                     0
#define rbus_pushObj(x, y, z)                               0
#define rbus_pushObjNoAck(x, y)                             0
#define rbus_pullObj(x, y, z)                               0
#define rbus_subscribeToTimedUpdateEvents(x, y, a, b, c)    0
#define rbus_unsubscribeFromTimedUpdateEvents(x, y)         0
#define rbus_resolveWildcardDestination(x, y, z)            0
#define rbus_registeredComponents(a)                        0
#define rbus_GetElementsAddedByObject(a, b)                 0
#define rbus_AppendInt32(x, y)                              0
#define rbus_PopInt32(x, y)                                 0
#define rbus_AppendString(x, y)                             0
#define rbus_PopString(x, y)                                0
#else
#include <rbus-core/rbus_core.h>
#include <rbus-core/rbus_marshalling.h>
#include <rbus-core/rbus_session_mgr.h>
int CcspBaseIf_evt_callback_rbus(const char * object_name,  const char * event_name, rtMessage message, void * user_data);
#endif
          
