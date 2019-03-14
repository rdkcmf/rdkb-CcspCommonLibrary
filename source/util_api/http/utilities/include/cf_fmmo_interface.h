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

    module:	cf_fmmo_interface.h

        For Filtering Message Management Object,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file includes all the header files required by
        Filtering Message Management Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan   

    ---------------------------------------------------------------

    revision:

        03/24/02    initial revision.

    ---------------------------------------------------------------

    last modified:  
    
**********************************************************************/

#ifndef  _CF_FMMO_INTERFACE_DEC
#define  _CF_FMMO_INTERFACE_DEC

#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"

/*
 * Define some const values that will be used in the object mapper object definition.
 */

#define  CF_FMMO_COMPONENT_NAME                     "CfFmmoComponent"
#define  CF_FMMO_COMPONENT_OID                      ANSC_OBJECT_OID_NULL
#define  CF_FMMO_COMPONENT_TYPE                     ANSC_OBJECT_TYPE_NULL

#if 0
#define  CF_FILTER_URL_HASH_TABLE_SIZE              1      /* cache bucket size */
#else
#define  CF_FILTER_URL_HASH_TABLE_SIZE              256     /* cache bucket size */
#endif

#define  CF_FILTER_URL_HASH_BUCKET_SIZE             8       /* maximum size of each bucket */

#define  CF_FILTER_SERVER_TIMEOUT_VALUE             60      /* 60 seconds */

#define  CF_FILTER_URL_BLOCK_BY_BLACK_LIST          "Domain-Black-List"

typedef  struct
_CF_FILTER_URL_CACHE_ENTITY
{
    PUCHAR                          Url;            /* request URL */
    PUCHAR                          Category;       /* request URL's category (might more than 1 category) */
    ULONG                           ulLastTime;     /* last time used by tick counts in second */
}
CF_FILTER_URL_CACHE_ENTITY, *PCF_FILTER_URL_CACHE_ENTITY;


typedef
void
(*PFN_CF_FMMO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef
void
(*PFN_CF_FMMO_QUERY_URL)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pUrl,
        ULONG                       ulUrlIndex
    );

typedef
ANSC_STATUS
(*PFN_CF_FMMO_QUERY_CACHE_URL)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pUrl,
        ULONG                       ulUrlIndex
    );

typedef
void
(*PFN_CF_FMMO_SET_SERVER)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pServerIP,
        USHORT                      ulServerPort
    );

typedef
void
(*PFN_CF_FMMO_SET_SHARED_TOKEN)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pSharedToken
    );

typedef
void
(*PFN_CF_FMMO_CACHE_URL)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pUrl,
        PUCHAR                      pCategory
    );

typedef
void
(*PFN_CF_FMMO_CLEAR_CACHE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_CF_FMMO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_STATUS
(*PFN_CF_FMMO_CREATE_UDP_CLIENT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_STATUS
(*PFN_CF_FMMO_ENGAGE_UDP_CLIENT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_STATUS
(*PFN_CF_FMMO_CREATE_UDP_WORKER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_CF_FMMO_SET_ACCOUNT_ID)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pAccountId
    );

typedef
void
(*PFN_CF_FMMO_SET_ACCOUNT_PASS)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pAccountPassword
    );

typedef
void
(*PFN_CF_FMMO_SET_USER_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pUserName
    );

typedef
void
(*PFN_CF_FMMO_SET_USER_PASS)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pUserPassword
    );

typedef
PUCHAR
(*PFN_CF_FMMO_GET_ACCOUNT_ID)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
PUCHAR
(*PFN_CF_FMMO_GET_ACCOUNT_PASS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
PUCHAR
(*PFN_CF_FMMO_GET_USER_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
PUCHAR
(*PFN_CF_FMMO_GET_USER_PASS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
PUCHAR
(*PFN_CF_FMMO_GET_SHARED_TOKEN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_CF_FMMO_SET_DEVICE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       DeviceType,
        PUCHAR                      pDeviceName,
        PUCHAR                      pDeviceId
    );

typedef
BOOL
(*PFN_CF_FMMO_HAS_BLOCK_CATEGORY)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pCategory
    );

typedef
void
(*PFN_CF_FMMO_CALC_USER_PASS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_CF_FMMO_SET_WHITE_LIST)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pWhiteList
    );

typedef
void
(*PFN_CF_FMMO_SET_DEFAULT_WHTLST)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pWhiteList
    );

typedef
void
(*PFN_CF_FMMO_ADD_TASK_REQ)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTaskReq
    );

typedef
ANSC_HANDLE
(*PFN_CF_FMMO_GET_TASK_REQ)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_CF_FMMO_CLEAR_TASK_QUEUE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_CF_FMMO_ADD_SERVER)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pServerAddress,
        USHORT                      usServerPort
    );

typedef
void
(*PFN_CF_FMMO_START_SDO)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_CF_FMMO_EMERGENT_START)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_CF_FMMO_SET_AUTH_MSG_MGR)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hAuthMsgMgr
    );

typedef
void
(*PFN_CF_FMMO_SET_DOMAIN_WHTLST)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pDomainWhiteList
    );

typedef
void
(*PFN_CF_FMMO_SET_DOMAIN_BLKLST)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pDomainBlackList
    );

typedef
BOOL
(*PFN_CF_FMMO_URL_IN_BLACKLIST)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pURL
    );

typedef
BOOL
(*PFN_CF_FMMO_URL_IN_WHITELIST)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pURL
    );

typedef
void
(*PFN_CF_FMMO_BLOCK_URL)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pURL,
        ULONG                       ulUrlIndex,
        PUCHAR                      pCategory
    );

typedef
void
(*PFN_CF_FMMO_UNBLOCK_URL)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pURL,
        ULONG                       ulUrlIndex
    );

typedef
void
(*PFN_CF_FMMO_SET_LOG_FILE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hLogFile
    );

typedef
void
(*PFN_CF_FMMO_SET_REPORT_MSG_MGR)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReportMsgMgr
    );

typedef
void
(*PFN_CF_FMMO_REPORT_URL)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pUrl,
        BOOL                        bBlocked,
        PUCHAR                      pCategory
    );

typedef
void
(*PFN_CF_FMMO_GET_SERVER_LIST)
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
#define  CF_FILTER_MSG_MGR_CLASS_CONTENT                                                \
    /* duplication of the base object class content */                                  \
    ANSCCO_CLASS_CONTENT                                                                \
    /* start of object class content */                                                 \
    ANSC_HANDLE                     hContext;                                           \
    ANSC_HANDLE                     hFilterSdo;                                         \
    ANSC_HANDLE                     hCurServerInfo;                                     \
    ANSC_LOCK                       UrlCacheLock;                                       \
    PCF_FILTER_URL_CACHE_ENTITY     UrlCache[CF_FILTER_URL_HASH_TABLE_SIZE][CF_FILTER_URL_HASH_BUCKET_SIZE];            \
    ANSC_LOCK                       TaskReqQueueLock;                                   \
    QUEUE_HEADER                    TaskReqQueue;                                       \
    BOOL                            bTaskActive;                                        \
    ANSC_HANDLE                     hUdpClient;                                         \
    ANSC_HANDLE                     hUdpClientWorker;                                   \
    PUCHAR                          AccountId;                                          \
    PUCHAR                          AccountPassword;                                    \
    PUCHAR                          UserName;                                           \
    PUCHAR                          UserPassword;                                       \
    /* white list (string list)  */                                                     \
    ANSC_HANDLE                     DefaultWhiteList;                                   \
    ANSC_HANDLE                     WhiteList;                                          \
    BOOL                            bUseDefaultWhiteList;                               \
    ANSC_HANDLE                     hAuthMsgMgr;                                        \
    /* the token is got from AP and it's used to  do a simple authorization.*/          \
    PUCHAR                          SharedToken;                                        \
    ULONG                           DeviceType;                                         \
    PUCHAR                          DeviceName;                                         \
    PUCHAR                          DeviceId;                                           \
    ANSC_HANDLE                     hCryptoObj;                                         \
    /* domain white & black list (string lists) */                                      \
    ANSC_HANDLE                     DomainWhiteList;                                    \
    ANSC_HANDLE                     DomainBlackList;                                    \
    BOOL                            bTaskExited;                                        \
    ANSC_HANDLE                     hLogFile;                                           \
    ANSC_HANDLE                     hReportMsgMgr;                                      \
    ANSC_HANDLE                     hNtpClock;                                          \
    ANSC_HANDLE                     hSlmo;                                              \
    /* start of methods */                                                              \
    PFN_CF_FMMO_SET_CONTEXT         SetContext;                                         \
    PFN_CF_FMMO_SET_SERVER          SetServer;                                          \
    PFN_CF_FMMO_QUERY_URL           QueryUrl;                                           \
    PFN_CF_FMMO_CACHE_URL           CacheUrl;                                           \
    PFN_CF_FMMO_QUERY_CACHE_URL     QueryUrlFromCache;                                  \
    PFN_CF_FMMO_CLEAR_CACHE         ClearCache;                                         \
    PFN_CF_FMMO_SET_SHARED_TOKEN    SetSharedToken;                                     \
    PFN_CF_FMMO_GET_CONTEXT         GetContext;                                         \
    PFN_CF_FMMO_CREATE_UDP_CLIENT   CreateUdpClient;                                    \
    PFN_CF_FMMO_ENGAGE_UDP_CLIENT   EngageUdpClient;                                    \
    PFN_CF_FMMO_CREATE_UDP_WORKER   CreateUdpClientWorker;                              \
    PFN_CF_FMMO_SET_ACCOUNT_ID      SetAccountId;                                       \
    PFN_CF_FMMO_SET_ACCOUNT_PASS    SetAccountPassword;                                 \
    PFN_CF_FMMO_SET_USER_NAME       SetUserName;                                        \
    PFN_CF_FMMO_SET_USER_PASS       SetUserPassword;                                    \
    PFN_CF_FMMO_GET_ACCOUNT_ID      GetAccountId;                                       \
    PFN_CF_FMMO_GET_ACCOUNT_PASS    GetAccountPassword;                                 \
    PFN_CF_FMMO_GET_USER_NAME       GetUserName;                                        \
    PFN_CF_FMMO_GET_USER_PASS       GetUserPassword;                                    \
    PFN_CF_FMMO_GET_SHARED_TOKEN    GetSharedToken;                                     \
    PFN_CF_FMMO_SET_DEVICE          SetDevice;                                          \
    PFN_CF_FMMO_HAS_BLOCK_CATEGORY  HasBlockedCategory;                                 \
    PFN_CF_FMMO_CALC_USER_PASS      CalcUserPassword;                                   \
    PFN_CF_FMMO_SET_WHITE_LIST      SetWhiteList;                                       \
    PFN_CF_FMMO_SET_DEFAULT_WHTLST  SetDefaultWhiteList;                                \
    PFN_CF_FMMO_ADD_TASK_REQ        AddTaskReq;                                         \
    PFN_CF_FMMO_GET_TASK_REQ        GetTaskReq;                                         \
    PFN_CF_FMMO_CLEAR_TASK_QUEUE    ClearTaskQueue;                                     \
    PFN_CF_FMMO_ADD_SERVER          AddServer;                                          \
    PFN_CF_FMMO_START_SDO           StartSdo;                                           \
    PFN_CF_FMMO_EMERGENT_START      EmergentStart;                                      \
    PFN_CF_FMMO_SET_AUTH_MSG_MGR    SetAuthMsgMgr;                                      \
    PFN_CF_FMMO_SET_DOMAIN_WHTLST   SetDomainWhiteList;                                 \
    PFN_CF_FMMO_SET_DOMAIN_BLKLST   SetDomainBlackList;                                 \
    PFN_CF_FMMO_URL_IN_BLACKLIST    IsUrlInBlackList;                                   \
    PFN_CF_FMMO_URL_IN_WHITELIST    IsUrlInWhiteList;                                   \
    PFN_CF_FMMO_BLOCK_URL           BlockUrl;                                           \
    PFN_CF_FMMO_UNBLOCK_URL         UnblockUrl;                                         \
    PFN_CF_FMMO_SET_LOG_FILE        SetLogFile;                                         \
    PFN_CF_FMMO_SET_REPORT_MSG_MGR  SetReportMsgMgr;                                    \
    PFN_CF_FMMO_REPORT_URL          ReportUrl;                                          \
    PFN_CF_FMMO_GET_SERVER_LIST     GetServerList;                                      \


/* Filtering Message Management Object (FMMO) */
typedef  struct
_CF_FMMO_OBJECT
{
    CF_FILTER_MSG_MGR_CLASS_CONTENT
}
CF_FMMO_OBJECT,    *PCF_FMMO_OBJECT;


#define  ACCESS_CF_FMMO_OBJECT(p)            \
         ACCESS_CONTAINER(p, CF_FMMO_OBJECT, Linkage)

#endif

