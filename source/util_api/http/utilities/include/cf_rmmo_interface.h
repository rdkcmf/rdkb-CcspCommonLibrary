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


/**********************************************************************

    module:	cf_rmmo_interface.h

        For Content Filtering Reporting Message Management Object,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file includes all the header files required by
        Content Filtering Reporting Message Management Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan   

    ---------------------------------------------------------------

    revision:

        04/03/02    initial revision.

    ---------------------------------------------------------------

    last modified:  
    
**********************************************************************/

#ifndef  _CF_RMMO_INTERFACE_DEC
#define  _CF_RMMO_INTERFACE_DEC

#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"

/*
 * Define some const values that will be used in the object mapper object definition.
 */

#define  CF_RMMO_COMPONENT_NAME                     "CfRmmoComponent"
#define  CF_RMMO_COMPONENT_OID                      ANSC_OBJECT_OID_NULL
#define  CF_RMMO_COMPONENT_TYPE                     ANSC_OBJECT_TYPE_NULL

#define  CF_RMMO_QUERY_TIMEOUT_MS                   60000       /* 60 seconds */
#define  CF_REPORT_SERVER_SDO_TIMEOUT_VALUE         30          /* 30 seconds */

#define  CF_REPORT_MSG_DEFAULT_BUF_LEN              65536

#ifdef   _DEBUG
#define  CF_REPORT_REQ_MESSAGE_NUMBER               5
#else
#define  CF_REPORT_REQ_MESSAGE_NUMBER               50
#endif

#define  CF_REPOPRT_URL_MAX_LEN                     1024

#define  CF_REPORT_TASK_NAP_DURATION_MS             500         /* 500 mill seonds */
#define  CF_REPORT_SERVER_RETRY_TIMES               3

#define  CF_REPORT_MAX_FAILURE_SERVER_COUNT         3

#define  CF_REPORT_URL_CATEGORY_NOT_EXIST           "OK"

typedef  struct
_CF_REPORT_HISTORY_MSG
{
    SINGLE_LINK_ENTRY               Linkage;                
    PUCHAR                          pHistory;       /* XML encoded history report message */
    ULONG                           ulHistoryLen;   /* history report message length */
}
CF_REPORT_HISTORY_MSG, *PCF_REPORT_HISTORY_MSG;

#define  ACCESS_CF_HISTORY_MSG_OBJECT(p)            \
         ACCESS_CONTAINER(p, CF_REPORT_HISTORY_MSG, Linkage)


typedef  struct
_CF_REPORT_USER_INFO
{
    PUCHAR                          pAccountId;
    PUCHAR                          pAccountPassword;
    PUCHAR                          pUserName;
    PUCHAR                          pUserPassword;
    PUCHAR                          pDeviceId;
    PUCHAR                          pDeviceType;
    PUCHAR                          pClientVersion;
    /* A pointer array which contains "reporting history" object */
    ANSC_HANDLE                     HistoryArray;
    ANSC_LOCK                       HistoryArrayLock;
}
CF_REPORT_USER_INFO, *PCF_REPORT_USER_INFO;


typedef
void
(*PFN_CF_RMMO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef
ANSC_HANDLE
(*PFN_CF_RMMO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_STATUS
(*PFN_CF_RMMO_CREATE_TCP_CLIENT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_STATUS
(*PFN_CF_RMMO_ENGAGE_TCP_CLIENT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_STATUS
(*PFN_CF_RMMO_CREATE_TCP_WORKER)
    (   
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_CF_RMMO_ADD_SERVER)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pServerAddress,
        USHORT                      usServerPort
    );

typedef
void
(*PFN_CF_RMMO_START_SDO)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_CF_RMMO_SET_LOG_FILE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hLogFile
    );

typedef
void
(*PFN_CF_RMMO_CLEAR_USER_INFO)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_CF_RMMO_CLEAR_HISTORY_QUEUE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_CF_RMMO_ADD_USER)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      AccountId,
        PUCHAR                      AccountPass,
        PUCHAR                      UserName,
        PUCHAR                      UserPass,
        PUCHAR                      DeviceType,
        PUCHAR                      DeviceId,
        PUCHAR                      ClientVersion
    );

typedef
ANSC_HANDLE
(*PFN_CF_RMMO_FIND_USER_INFO)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      AccountId,
        PUCHAR                      UserName,
        PUCHAR                      DeviceType,
        PUCHAR                      DeviceId
    );

typedef
void
(*PFN_CF_RMMO_ADD_URL)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      AccountId,
        PUCHAR                      UserName,
        PUCHAR                      DeviceType,
        PUCHAR                      DeviceId,
        PUCHAR                      pUrl,
        BOOL                        bBlocked,
        PUCHAR                      pCategory,
        PANSC_UNIVERSAL_TIME        pTimeStamp
    );

typedef
void
(*PFN_CF_RMMO_BUILD_REPORT_REQ)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hUserInfo
    );

typedef
void
(*PFN_CF_RMMO_GET_SERVER_LIST)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlmo
    );


/*
 * As a standard practice, we encapsulate all functional and feature objects inside an Object
 * Container Object so it will function as a black-box implementation when we have to integrate
 * an Ansc component with other systems. However, that's not the only reason why we need to
 * define the Container Object: we also use container as the building block for constructing
 * larger systems.
 */
#define  CF_REPORTING_MSG_MGR_CLASS_CONTENT                                             \
    /* duplication of the base object class content */                                  \
    ANSCCO_CLASS_CONTENT                                                                \
    /* start of object class content */                                                 \
    ANSC_HANDLE                     hContext;                                           \
    ANSC_HANDLE                     hTcpClient;                                         \
    ANSC_HANDLE                     hTcpClientWorker;                                   \
    ANSC_HANDLE                     hReportSdo;                                         \
    ANSC_HANDLE                     hCurServerInfo;                                     \
    ANSC_HANDLE                     RRTimeoutEvent;                                     \
    BOOL                            bStarted;                                           \
    BOOL                            bTaskExited;                                        \
    ANSC_HANDLE                     hUserInfoArray;                                     \
    QUEUE_HEADER                    HistoryMsgQueue;                                    \
    ANSC_LOCK                       HistoryMsgQueueLock;                                \
    ANSC_HANDLE                     hLogFile;                                           \
    ANSC_HANDLE                     hSlmo;                                              \
    /* start of methods */                                                              \
    PFN_CF_RMMO_SET_CONTEXT         SetContext;                                         \
    PFN_CF_RMMO_GET_CONTEXT         GetContext;                                         \
    PFN_CF_RMMO_CREATE_TCP_CLIENT   CreateTcpClient;                                    \
    PFN_CF_RMMO_ENGAGE_TCP_CLIENT   EngageTcpClient;                                    \
    PFN_CF_RMMO_CREATE_TCP_WORKER   CreateTcpClientWorker;                              \
    PFN_CF_RMMO_ADD_SERVER          AddServer;                                          \
    PFN_CF_RMMO_START_SDO           StartSdo;                                           \
    PFN_CF_RMMO_SET_LOG_FILE        SetLogFile;                                         \
    PFN_CF_RMMO_CLEAR_USER_INFO     ClearUserInfo;                                      \
    PFN_CF_RMMO_CLEAR_HISTORY_QUEUE ClearHistoryMsgQueue;                               \
    PFN_CF_RMMO_ADD_USER            AddUser;                                            \
    PFN_CF_RMMO_ADD_URL             AddUrl;                                             \
    PFN_CF_RMMO_FIND_USER_INFO      FindUserInfo;                                       \
    PFN_CF_RMMO_BUILD_REPORT_REQ    BuildReportingReq;                                  \
    PFN_CF_RMMO_GET_SERVER_LIST     GetServerList;                                      \


/* Filtering Message Management Object (FMMO) */
typedef  struct
_CF_RMMO_OBJECT
{
    CF_REPORTING_MSG_MGR_CLASS_CONTENT
}
CF_RMMO_OBJECT,    *PCF_RMMO_OBJECT;


#define  ACCESS_CF_RMMO_OBJECT(p)            \
         ACCESS_CONTAINER(p, CF_RMMO_OBJECT, Linkage)

#endif

