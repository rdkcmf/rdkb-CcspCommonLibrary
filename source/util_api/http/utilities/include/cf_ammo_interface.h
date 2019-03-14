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

    module:	cf_ammo_interface.h

        For Content Filtering Authentication Message Management Object,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file includes all the header files required by
        Content Filtering Authentication Message Management Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan   

    ---------------------------------------------------------------

    revision:

        03/29/02    initial revision.

    ---------------------------------------------------------------

    last modified:  
    
**********************************************************************/

#ifndef  _CF_AMMO_INTERFACE_DEC
#define  _CF_AMMO_INTERFACE_DEC

#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"

/*
 * Define some const values that will be used in the object mapper object definition.
 */

#define  CF_AMMO_COMPONENT_NAME                     "CfAmmoComponent"
#define  CF_AMMO_COMPONENT_OID                      ANSC_OBJECT_OID_NULL
#define  CF_AMMO_COMPONENT_TYPE                     ANSC_OBJECT_TYPE_NULL

#define  CF_AUTH_SERVER_SDO_TIMEOUT_VALUE           30          /* 30 seconds */
#define  CF_AMMO_QUERY_TIMEOUT_MS                   60000       /* 60 seconds */

#define  CF_AUTH_USER_BASE                          0
#define  CF_AUTH_USER_BEING_AUTHENTICATED           1
#define  CF_AUTH_USER_LEGITIMATE                    2
#define  CF_AUTH_USER_NOT_QUALIFIED                 3

#define  CF_AUTH_REP_DEFAULT_BUF_LEN                65536       /* 64K */

typedef
void
(*PFN_CF_AMMO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef
ANSC_HANDLE
(*PFN_CF_AMMO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_STATUS
(*PFN_CF_AMMO_CREATE_TCP_CLIENT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_STATUS
(*PFN_CF_AMMO_ENGAGE_TCP_CLIENT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_STATUS
(*PFN_CF_AMMO_CREATE_TCP_WORKER)
    (   
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_CF_AMMO_SET_ACCOUNT_ID)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pAccountId
    );

typedef
void
(*PFN_CF_AMMO_SET_ACCOUNT_PASS)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pAccountPassword
    );

typedef
void
(*PFN_CF_AMMO_SET_USER_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pUserName
    );

typedef
void
(*PFN_CF_AMMO_SET_USER_PASS)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 pUserPassword
    );

typedef
PUCHAR
(*PFN_CF_AMMO_GET_ACCOUNT_ID)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
PUCHAR
(*PFN_CF_AMMO_GET_ACCOUNT_PASS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
PUCHAR
(*PFN_CF_AMMO_GET_USER_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
PUCHAR
(*PFN_CF_AMMO_GET_USER_PASS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_CF_AMMO_SET_DEVICE)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pDeviceType,
        PUCHAR                      pDeviceName
    );

typedef
void
(*PFN_CF_AMMO_ADD_SERVER)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pServerAddress,
        USHORT                      usServerPort
    );

typedef
void
(*PFN_CF_AMMO_START_SDO)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_CF_AMMO_SET_CLIENT_VERSION)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pClientVersion
    );

typedef
void
(*PFN_CF_AMMO_SET_LAST_TIME)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       Year,
        ULONG                       Month,
        ULONG                       Day,
        ULONG                       Hour,
        ULONG                       Minute,
        ULONG                       Second
    );

typedef
void
(*PFN_CF_AMMO_SET_EDITION)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pEdition
    );

typedef
void
(*PFN_CF_AMMO_SET_LICENSE_TYPE)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pLicenseType
    );

typedef
void
(*PFN_CF_AMMO_AUTHENTICATE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_STATUS
(*PFN_CF_AMMO_PREPARE_AUTH_REQ)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_STATUS
(*PFN_CF_AMMO_PARSE_AUTH_REP)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pAPMsg,
        ULONG                       ulAPMsg
    );

typedef
ULONG
(*PFN_CF_AMMO_GET_AUTH_RESULT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
PUCHAR
(*PFN_CF_AMMO_GET_WHITE_LIST)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
PUCHAR
(*PFN_CF_AMMO_GET_SHARED_TOKEN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
PUCHAR
(*PFN_CF_AMMO_GET_DOMAIN_WHTLST)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
PUCHAR
(*PFN_CF_AMMO_GET_DOMAIN_BLKLST)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_CF_AMMO_SET_LOG_FILE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hLogFile
    );

typedef
PUCHAR
(*PFN_CF_AMMO_GET_DEVICE_TYPE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
PUCHAR
(*PFN_CF_AMMO_GET_DEVICE_ID)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
PUCHAR
(*PFN_CF_AMMO_GET_CLIENT_VERSION)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_CF_AMMO_GET_SERVER_LIST)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlmo
    );

typedef
BOOL
(*PFN_CF_AMMO_HAS_REPORTING)
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
#define  CF_AUTH_MSG_MGR_CLASS_CONTENT                                                \
    /* duplication of the base object class content */                                  \
    ANSCCO_CLASS_CONTENT                                                                \
    /* start of object class content */                                                 \
    ANSC_HANDLE                     hContext;                                           \
    ANSC_HANDLE                     hTcpClient;                                         \
    ANSC_HANDLE                     hTcpClientWorker;                                   \
    PUCHAR                          AccountId;                                          \
    PUCHAR                          AccountPassword;                                    \
    PUCHAR                          UserName;                                           \
    PUCHAR                          UserPassword;                                       \
    PUCHAR                          Edition;                                            \
    PUCHAR                          LicenseType;                                        \
    PUCHAR                          DeviceName;                                         \
    PUCHAR                          DeviceType;                                         \
    PUCHAR                          ClientVersion;                                      \
    ANSC_HANDLE                     hCryptoObj;                                         \
    ANSC_HANDLE                     hAuthSdo;                                           \
    ANSC_HANDLE                     hAuthRequestMsg;                                    \
    PUCHAR                          pARMsg;                                             \
    ULONG                           ulARMsgLen;                                         \
    ANSC_HANDLE                     hAuthResponseMsg;                                   \
    ANSC_UNIVERSAL_TIME             LastTime;                                           \
    ANSC_HANDLE                     hCurServerInfo;                                     \
    ANSC_HANDLE                     ARMsgTimeoutEvent;                                  \
    BOOL                            bAuthenticating;                                    \
    BOOL                            bStarted;                                           \
    BOOL                            bTaskExited;                                        \
    ANSC_HANDLE                     hLogFile;                                           \
    ANSC_HANDLE                     hSlmo;                                              \
    /* start of methods */                                                              \
    PFN_CF_AMMO_SET_CONTEXT         SetContext;                                         \
    PFN_CF_AMMO_GET_CONTEXT         GetContext;                                         \
    PFN_CF_AMMO_CREATE_TCP_CLIENT   CreateTcpClient;                                    \
    PFN_CF_AMMO_ENGAGE_TCP_CLIENT   EngageTcpClient;                                    \
    PFN_CF_AMMO_CREATE_TCP_WORKER   CreateTcpClientWorker;                              \
    PFN_CF_AMMO_SET_ACCOUNT_ID      SetAccountId;                                       \
    PFN_CF_AMMO_SET_ACCOUNT_PASS    SetAccountPassword;                                 \
    PFN_CF_AMMO_SET_USER_NAME       SetUserName;                                        \
    PFN_CF_AMMO_SET_USER_PASS       SetUserPassword;                                    \
    PFN_CF_AMMO_GET_ACCOUNT_ID      GetAccountId;                                       \
    PFN_CF_AMMO_GET_ACCOUNT_PASS    GetAccountPassword;                                 \
    PFN_CF_AMMO_GET_USER_NAME       GetUserName;                                        \
    PFN_CF_AMMO_GET_USER_PASS       GetUserPassword;                                    \
    PFN_CF_AMMO_SET_DEVICE          SetDevice;                                          \
    PFN_CF_AMMO_ADD_SERVER          AddServer;                                          \
    PFN_CF_AMMO_START_SDO           StartSdo;                                           \
    PFN_CF_AMMO_SET_CLIENT_VERSION  SetClientVersion;                                   \
    PFN_CF_AMMO_SET_LAST_TIME       SetLastTime;                                        \
    PFN_CF_AMMO_SET_EDITION         SetEdition;                                         \
    PFN_CF_AMMO_SET_LICENSE_TYPE    SetLicenseType;                                     \
    PFN_CF_AMMO_AUTHENTICATE        Authenticate;                                       \
    PFN_CF_AMMO_PREPARE_AUTH_REQ    PrepareAuthRequest;                                 \
    PFN_CF_AMMO_PARSE_AUTH_REP      ParseAuthResponse;                                  \
    PFN_CF_AMMO_GET_AUTH_RESULT     GetAuthResult;                                      \
    PFN_CF_AMMO_GET_WHITE_LIST      GetWhiteList;                                       \
    PFN_CF_AMMO_GET_SHARED_TOKEN    GetSharedToken;                                     \
    PFN_CF_AMMO_GET_DOMAIN_WHTLST   GetDomainWhiteList;                                 \
    PFN_CF_AMMO_GET_DOMAIN_BLKLST   GetDomainBlackList;                                 \
    PFN_CF_AMMO_SET_LOG_FILE        SetLogFile;                                         \
    PFN_CF_AMMO_GET_DEVICE_TYPE     GetDeviceType;                                      \
    PFN_CF_AMMO_GET_DEVICE_ID       GetDeviceId;                                        \
    PFN_CF_AMMO_GET_CLIENT_VERSION  GetClientVersion;                                   \
    PFN_CF_AMMO_GET_SERVER_LIST     GetServerList;                                      \
    PFN_CF_AMMO_HAS_REPORTING       HasReporting;                                       \




/* Filtering Message Management Object (FMMO) */
typedef  struct
_CF_AMMO_OBJECT
{
    CF_AUTH_MSG_MGR_CLASS_CONTENT
}
CF_AMMO_OBJECT,    *PCF_AMMO_OBJECT;


#define  ACCESS_CF_AMMO_OBJECT(p)            \
         ACCESS_CONTAINER(p, CF_AMMO_OBJECT, Linkage)

#endif

