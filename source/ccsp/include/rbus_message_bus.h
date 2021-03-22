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
#ifndef CCSP_RBUS_INTERNAL_H
#define CCSP_RBUS_INTERNAL_H
#include <rbus-core/rbus_core.h>
#include <rbus-core/rbus_session_mgr.h>
#include <rtmessage/rtLog.h>

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
    RBUS_DATATYPE_DATETIME,          /** rbusDateTime_t structure for Date/Time */
    RBUS_DATATYPE_STRING,            /** null terminated C style string */
    RBUS_DATATYPE_BYTES,             /** byte array */
    RBUS_DATATYPE_PROPERTY,          /**< property instance */
    RBUS_DATATYPE_OBJECT,            /** object instance */
    RBUS_DATATYPE_NONE
} rbusNewDataType_t;

typedef struct _rbusTimeZone {
    int m_tzhour;
    int m_tzmin;
    int m_isWest;
} rbusTimeZone_t ;

typedef struct _rbusDateTime {
    struct tm       m_time;
    rbusTimeZone_t  m_tz;
} rbusDateTime_t;

int CcspBaseIf_evt_callback_rbus(const char * object_name,  const char * event_name, rbusMessage message, void * user_data);
void ccsp_handle_rbus_component_reply (void* bus_handle, rbusMessage msg, rbusNewDataType_t typeVal, enum dataType_e *pType, char** pStringValue);

#endif /* CCSP_RBUS_INTERNAL_H */
