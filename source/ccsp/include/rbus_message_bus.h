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
#define MESSAGE_FIELD_PAYLOAD                            "DUMMY"
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
#define rbus_findMatchingObjects(x, y, z)                   0
#define rbus_registeredComponents(a)                        0
#define rbus_GetElementsAddedByObject(a, b)                 0
#define rbus_AppendInt32(x, y)                              0
#define rbus_PopInt32(x, y)                                 0
#define rbus_AppendString(x, y)                             0
#define rbus_PopString(x, y)                                0
#define rbus_SetString(x,y,z)                               0
#define rbus_GetString(x,y,z)                               0
#define ccsp_handle_rbus_component_reply(a, b, c, d)        0
#define RBUS_RETURN_CODE_SUCCESS                            0
#define RBUS_RETURN_CODE_FAILURE                            1
#define rbusNewDataType_t                                   int
#define RBUS_DATATYPE_BOOLEAN                               0x500
#define RBUS_DATATYPE_STRING                                0x50E

#else
#include <rbus-core/rbus_core.h>
#include <rbus-core/rbus_marshalling.h>
#include <rbus-core/rbus_session_mgr.h>
#include <rbus-core/rtLog.h>

#define RBUS_RETURN_CODE_SUCCESS 0
#define RBUS_RETURN_CODE_FAILURE 1

typedef enum _rbus_data_type_t {
    RBUS_DATATYPE_BOOLEAN  = 0x500,  /** stdbool true or false */
    RBUS_DATATYPE_CHAR,              /** char of size 1 byte*/
    RBUS_DATATYPE_BYTE,              /** unsigned char */
    RBUS_DATATYPE_INT8,              /** 8 bit int */
    RBUS_DATATYPE_UINT8,             /** 8 bit unsigned int */
    RBUS_DATATYPE_INT16,             /** 16 bit int */
    RBUS_DATATYPE_UINT16,            /** 16 bit unsigned int */
    RBUS_DATATYPE_INT32,             /** 32 bit int */
    RBUS_DATATYPE_UINT32,            /** 32 bit unsigned int */
    RBUS_DATATYPE_INT64,             /** 64 bit int */
    RBUS_DATATYPE_UINT64,            /** 64 bit unsigned int */
    RBUS_DATATYPE_SINGLE,            /** 32 bit float */
    RBUS_DATATYPE_DOUBLE,            /** 64 bit float */
    RBUS_DATATYPE_DATETIME,          /** timeval num secs/msecs from epoch */
    RBUS_DATATYPE_STRING,            /** null terminated C style string */
    RBUS_DATATYPE_BYTES,             /** byte array */
    RBUS_DATATYPE_PROPERTY,          /**< property instance */
    RBUS_DATATYPE_OBJECT,            /** object instance */
    RBUS_DATATYPE_NONE
} rbusNewDataType_t;



int CcspBaseIf_evt_callback_rbus(const char * object_name,  const char * event_name, rtMessage message, void * user_data);
static int telemetry_send_signal_rbus(const char * destination, const char * method, rtMessage request, void * user_data, rtMessage *response, const rtMessageHeader* hdr);
void ccsp_handle_rbus_component_reply (void* bus_handle, rtMessage msg, rbusNewDataType_t typeVal, enum dataType_e *pType, char** pStringValue);
#endif
          
