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

/**********************************************************************
   Copyright [2014] [Cisco Systems, Inc.]
 
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
 
       http://www.apache.org/licenses/LICENSE-2.0
 
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
**********************************************************************/


#ifndef  _CF_REPORTING_MSG_CO_INTERFACE_DEC
#define  _CF_REPORTING_MSG_CO_INTERFACE_DEC

#include "cf_bmco_co_interface.h"

/* =============================================================================
 *          Reporting History Object
 * ========================================================================== */

#define  CF_REPORTING_HISTORY_MSG_COMPONENT_NAME    "CfReportingHistoryComponent"
#define  CF_REPORTING_HISTORY_MSG_COMPONENT_OID     ANSC_OBJECT_OID_NULL
#define  CF_REPORTING_HISTORY_COMPONENT_TYPE        ANSC_OBJECT_TYPE_NULL

/* definition of reporting type */
#define   UNKNOWN_REPORT                            -1
#define   HISTORY_REPORT                            0
#define   INTRUSION_REPORT                          1
#define   BLOCKING_REPORT                           2

#define  CUR_RR_VERSION                             1
#define  CUR_RP_VERSION                             1

#define  FIXED_TIME_LENGTH                          19 


typedef
ANSC_HANDLE
(*CF_RH_GET_DOMAIN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*CF_RH_GET_PAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*CF_RH_GET_CATEGORY)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*CF_RH_GET_TIME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
BOOL
(*CF_RH_IS_BLOCKED)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*CF_RH_SET_PROPERTIES)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 Url,
        ANSC_HANDLE                 Time,
        BOOL                        bBlocked,
        ANSC_HANDLE                 Category
    );

typedef
ANSC_HANDLE
(*CF_RH_CREATE_XML_HANDLE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
BOOL
(*CF_RH_PARSE_XML_NODE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hXmlDomNode
    );

/*
 * As a standard practice, we encapsulate all functional and feature objects inside an Object
 * Container Object so it will function as a black-box implementation when we have to integrate
 * an Ansc component with other systems. However, that's not the only reason why we need to
 * define the Container Object: we also use container as the building block for constructing
 * larger systems.
 */
#define  CF_REPORTING_HISTORY_CLASS_CONTENT                                             \
    /* duplication of the base object class content */                                  \
    ANSCCO_CLASS_CONTENT                                                                \
    /* start of object class content */                                                 \
    ANSC_HANDLE                     Domain;                                             \
    ANSC_HANDLE                     Page;                                               \
    ANSC_HANDLE                     Time;                                               \
    BOOL                            bBlocked;                                           \
    ANSC_HANDLE                     Category;                                           \
    /* methods */                                                                       \
    CF_RH_GET_DOMAIN                GetDomain;                                          \
    CF_RH_GET_PAGE                  GetPage;                                            \
    CF_RH_GET_CATEGORY              GetCategory;                                        \
    CF_RH_GET_TIME                  GetTime;                                            \
    CF_RH_IS_BLOCKED                IsBlocked;                                          \
    CF_RH_SET_PROPERTIES            SetProperties;                                      \
    CF_RH_CREATE_XML_HANDLE         CreateXmlHandle;                                    \
    CF_RH_PARSE_XML_NODE            ParseXmlNode;                                       \

  

/* content filtering basic message object  */
typedef  struct
_CF_REPORTING_HISTORY_OBJECT
{
    CF_REPORTING_HISTORY_CLASS_CONTENT
}
CF_REPORTING_HISTORY_OBJECT,    *PCF_REPORTING_HISTORY_OBJECT;


#define  ACCESS_CF_REPORTING_HISTORY_OBJECT(p)            \
         ACCESS_CONTAINER(p, CF_REPORTING_HISTORY_OBJECT, Linkage)


/* =============================================================================
 *          Reporting Request Object
 * ========================================================================== */

#define  CF_REPORTING_REQUEST_MSG_COMPONENT_NAME    "CfReportingRequestComponent"
#define  CF_REPORTING_REQUEST_MSG_COMPONENT_OID     ANSC_OBJECT_OID_NULL
#define  CF_REPORTING_REQUEST_COMPONENT_TYPE        ANSC_OBJECT_TYPE_NULL

/*
 *	Enum  the  Child Node Handle 
 */
typedef 
BOOL
(*EnumHistoryProc)
(
	PVOID 					        pData, 
    ANSC_HANDLE                     domain,
    ANSC_HANDLE                     page,
    ANSC_HANDLE                     time,
    BOOL                            bBlocked,
    ANSC_HANDLE                     category
);

typedef BOOLEAN 
(*EnumIntrusionProc)
(
	PVOID 					        pData, 
    ANSC_HANDLE                     type,
    ANSC_HANDLE                     description,
    ANSC_HANDLE                     time
);

typedef BOOLEAN 
(*EnumBlockingProc)
(
	PVOID 					        pData, 
    ANSC_HANDLE                     sourceIP,
	ULONG                           sourcePort,
	ANSC_HANDLE                     destIP,
	ULONG                           destPort,
	ANSC_HANDLE                     type,
	ANSC_HANDLE                     reason,
    ANSC_HANDLE                     time
);


typedef
ANSC_HANDLE
(*CF_RR_GET_DEVICE_TYPE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*CF_RR_GET_DEVICE_ID)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*CF_RR_GET_PASSWORD)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*CF_RR_GET_CLIENT_VERSION)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*CF_RR_GET_ACCOUNT_ID)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*CF_RR_GET_ACCOUNT_PASSWORD)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*CF_RR_GET_HISTORY_ARRAY)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*CF_RR_GET_INTRUSION_ARRAY)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*CF_RR_GET_BLOCKING_ARRAY)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*CF_RR_SET_DEVICE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 DeviceType,
        ANSC_HANDLE                 DeviceID
    );

typedef
void
(*CF_RR_SET_PASSWORD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 Password
    );

typedef
void
(*CF_RR_SET_CLIENT_VERSION)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 ClientVersion
    );

typedef
void
(*CF_RR_SET_ACCOUNT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 AccountID,
        ANSC_HANDLE                 AccountPassword
    );

typedef
void
(*CF_RR_SET_ARRAY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHistoryArray
    );

typedef
BOOL
(*CF_RR_ENUM_HISTORY)
    (
        ANSC_HANDLE                 hThisObject,
        EnumHistoryProc             Proc,
        PVOID                       pData
    );

typedef
BOOL
(*CF_RR_ENUM_INTRUSION)
    (
        ANSC_HANDLE                 hThisObject,
        EnumIntrusionProc           Proc,
        PVOID                       pData
    );

typedef
BOOL
(*CF_RR_ENUM_BLOCKING)
    (
        ANSC_HANDLE                 hThisObject,
        EnumBlockingProc            Proc,
        PVOID                       pData
    );

typedef
LONG
(*CF_RR_GET_REPORTING_TYPE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*CF_RR_REMOVE_HISTORY_ARRAY)
    (
        ANSC_HANDLE                 hThisObject
    );


/*
 * As a standard practice, we encapsulate all functional and feature objects inside an Object
 * Container Object so it will function as a black-box implementation when we have to integrate
 * an Ansc component with other systems. However, that's not the only reason why we need to
 * define the Container Object: we also use container as the building block for constructing
 * larger systems.
 */
#define  CF_REPORTING_REQUEST_CLASS_CONTENT                                             \
    /* duplication of the base object class content */                                  \
    CF_BASIC_MESSAGE_CONTENT_CLASS_CONTENT                                              \
    /* start of object class content */                                                 \
    ANSC_HANDLE                     DeviceType;                                         \
    ANSC_HANDLE                     DeviceID;                                           \
    ANSC_HANDLE                     Password;                                           \
    ANSC_HANDLE                     ClientVersion;                                      \
    ANSC_HANDLE                     AccountID;                                          \
    ANSC_HANDLE                     AccountPassword;                                    \
    BOOL                            bCreateArray;                                       \
    /* three pointer arrays */                                                          \
    ANSC_HANDLE                     HistoryArray;                                       \
    ANSC_HANDLE                     IntrusionArray;                                     \
    ANSC_HANDLE                     BlockingArray;                                      \
    /* methods */                                                                       \
    CF_RR_GET_DEVICE_TYPE           GetDeviceType;                                      \
    CF_RR_GET_DEVICE_ID             GetDeviceID;                                        \
    CF_RR_GET_PASSWORD              GetPassword;                                        \
    CF_RR_GET_CLIENT_VERSION        GetClientVersion;                                   \
    CF_RR_GET_ACCOUNT_ID            GetAccountID;                                       \
    CF_RR_GET_ACCOUNT_PASSWORD      GetAccountPassword;                                 \
    CF_RR_GET_HISTORY_ARRAY         GetHistoryArray;                                    \
    CF_RR_GET_INTRUSION_ARRAY       GetIntrusionArray;                                  \
    CF_RR_GET_BLOCKING_ARRAY        GetBlockingArray;                                   \
    CF_RR_SET_DEVICE                SetDevice;                                          \
    CF_RR_SET_PASSWORD              SetPassword;                                        \
    CF_RR_SET_CLIENT_VERSION        SetClientVersion;                                   \
    CF_RR_SET_ACCOUNT               SetAccount;                                         \
    CF_RR_SET_ARRAY                 SetArray;                                           \
    CF_RR_ENUM_HISTORY              EnumHistory;                                        \
    CF_RR_ENUM_INTRUSION            EnumIntrusion;                                      \
    CF_RR_ENUM_BLOCKING             EnumBlock;                                          \
    CF_RR_GET_REPORTING_TYPE        GetReportingType;                                   \
    CF_RR_REMOVE_HISTORY_ARRAY      RemoveHistoryArray;                                 \

  

/* content filtering basic message object  */
typedef  struct
_CF_REPORTING_REQUEST_OBJECT
{
    CF_REPORTING_REQUEST_CLASS_CONTENT
}
CF_REPORTING_REQUEST_OBJECT,    *PCF_REPORTING_REQUEST_OBJECT;


#define  ACCESS_CF_REPORTING_REQUEST_OBJECT(p)            \
         ACCESS_CONTAINER(p, CF_REPORTING_REQUEST_OBJECT, Linkage)


/* =============================================================================
 *          Reporting History Object
 * ========================================================================== */

#define  CF_REPORTING_RESPONSE_MSG_COMPONENT_NAME   "CfReportingResponseComponent"
#define  CF_REPORTING_RESPONSE_MSG_COMPONENT_OID    ANSC_OBJECT_OID_NULL
#define  CF_REPORTING_RESPONSE_COMPONENT_TYPE       ANSC_OBJECT_TYPE_NULL

typedef
ANSC_HANDLE
(*CF_RP_GET_SERVER_VERSION)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
BOOL
(*CF_RP_IS_OK)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*CF_RP_GET_FAILURE_INFO)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*CF_RP_SET_SERVER_VERSION)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 ServerVersion
    );

typedef
void
(*CF_RP_SET_SUCCESS_STATUS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*CF_RP_SET_FAILURE_STATUS)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 FailureInfo
    );


/*
 * As a standard practice, we encapsulate all functional and feature objects inside an Object
 * Container Object so it will function as a black-box implementation when we have to integrate
 * an Ansc component with other systems. However, that's not the only reason why we need to
 * define the Container Object: we also use container as the building block for constructing
 * larger systems.
 */
#define  CF_REPORTING_RESPONSE_CLASS_CONTENT                                            \
    /* duplication of the base object class content */                                  \
    CF_BASIC_MESSAGE_CONTENT_CLASS_CONTENT                                              \
    /* start of object class content */                                                 \
    ANSC_HANDLE                     ServerVersion;                                            \
    BOOL                            bStatus;                                            \
    ANSC_HANDLE                     FailureInfo;                                        \
    /* methods */                                                                       \
    CF_RP_GET_SERVER_VERSION        GetServerVersion;                                   \
    CF_RP_IS_OK                     IsOK;                                               \
    CF_RP_GET_FAILURE_INFO          GetFailureInfo;                                     \
    CF_RP_SET_SERVER_VERSION        SetServerVersion;                                   \
    CF_RP_SET_SUCCESS_STATUS        SetSuccessStatus;                                   \
    CF_RP_SET_FAILURE_STATUS        SetFailureStatus;                                   \

  

/* content filtering basic message object  */
typedef  struct
_CF_REPORTING_RESPONSE_OBJECT
{
    CF_REPORTING_RESPONSE_CLASS_CONTENT
}
CF_REPORTING_RESPONSE_OBJECT,    *PCF_REPORTING_RESPONSE_OBJECT;


#define  ACCESS_CF_REPORTING_RESPONSE_OBJECT(p)            \
         ACCESS_CONTAINER(p, CF_REPORTING_RESPONSE_OBJECT, Linkage)


#endif

