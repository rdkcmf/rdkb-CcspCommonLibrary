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

    module:	web_gso_interface.h

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Web General Session Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/18/02    initial revision.

**********************************************************************/


#ifndef  _WEB_GSO_INTERFACE_
#define  _WEB_GSO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "web_properties.h"

#include "http_ifo_cgi.h"
#include "http_ifo_rcp.h"


/***********************************************************
 PLATFORM INDEPENDENT WEB GENERAL SESSION OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_WEBGSO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBGSO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_WEBGSO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBGSO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ULONG
(*PFN_WEBGSO_GET_ID)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBGSO_SET_ID)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSessionId
    );

typedef  PUCHAR
(*PFN_WEBGSO_GET_ADDR)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBGSO_SET_ADDR)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

typedef  USHORT
(*PFN_WEBGSO_GET_PORT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBGSO_SET_PORT)
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      port
    );

typedef  ULONG
(*PFN_WEBGSO_GET_TIME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBGSO_SET_TIME)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSeconds
    );

typedef  char*
(*PFN_WEBGSO_GET_STRING)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBGSO_SET_STRING)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pString
    );

typedef  ANSC_STATUS
(*PFN_WEBGSO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_WEBGSO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_WEBGSO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBGSO_ACQUIRE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBGSO_RELEASE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBGSO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBGSO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBGSO_TERMINATE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBGSO_LOCK)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBGSO_UNLOCK)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_WEBGSO_GET_ASINFO)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * Currently, HTTP servers respond to each client request without relating that request to previous
 * or subsequent requests; the technique allows clients and servers that wish to exchange state
 * information to place HTTP requests and responses within a larger context, which we term a
 * "session". This context might be used to create, for example, a "shopping cart", in which user
 * selections can be aggregated before purchase, or a magazine browsing system, in which a user's
 * previous reading effects which offerings are presented.
 */
#define  WEB_GENERAL_SESSION_CLASS_CONTENT                                                  \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hHfpIf;                                                 \
    ANSC_HANDLE                     hActiveSocket;                                          \
    ULONG                           SessionId;                                              \
    ULONG                           HashIndex;                                              \
    ULONG                           TimeStampInSec;                                         \
    ULONG                           LastReqAtInSec;                                         \
    ULONG                           LastRepAtInSec;                                         \
    ULONG                           RequestCount;                                           \
    ANSC_IPV4_ADDRESS               ClientAddr;                                             \
    USHORT                          ClientPort;                                             \
    ANSC_LOCK                       SyncLock;                                               \
    BOOL                            bActive;                                                \
    BOOL                            bLocked;                                                \
                                                                                            \
    ANSC_HANDLE                     hLsmContext;                                            \
    /*char                            LsmIdentifier[HTTP_MAX_COOKIE_VALUE_SIZE];*/          \
    char*                           LsmIdentifier;                                          \
    ULONG                           LsmMaxAge;                                              \
    ULONG                           LsmExpire;                                              \
    ULONG                           MaxLifespan;                                            \
                                                                                            \
    WEB_AUTH_SESSION_INFO           AuthSessInfo;                                           \
                                                                                            \
    PFN_WEBGSO_GET_IF               GetHfpIf;                                               \
    PFN_WEBGSO_SET_IF               SetHfpIf;                                               \
    PFN_WEBGSO_GET_CONTEXT          GetActiveSocket;                                        \
    PFN_WEBGSO_SET_CONTEXT          SetActiveSocket;                                        \
    PFN_WEBGSO_GET_ID               GetSessionId;                                           \
    PFN_WEBGSO_SET_ID               SetSessionId;                                           \
    PFN_WEBGSO_GET_ADDR             GetClientAddr;                                          \
    PFN_WEBGSO_SET_ADDR             SetClientAddr;                                          \
    PFN_WEBGSO_GET_PORT             GetClientPort;                                          \
    PFN_WEBGSO_SET_PORT             SetClientPort;                                          \
                                                                                            \
    PFN_WEBGSO_GET_CONTEXT          GetLsmContext;                                          \
    PFN_WEBGSO_SET_CONTEXT          SetLsmContext;                                          \
    PFN_WEBGSO_GET_STRING           GetLsmIdentifier;                                       \
    PFN_WEBGSO_SET_STRING           SetLsmIdentifier;                                       \
    PFN_WEBGSO_GET_TIME             GetLsmMaxAge;                                           \
    PFN_WEBGSO_SET_TIME             SetLsmMaxAge;                                           \
    PFN_WEBGSO_GET_TIME             GetLsmExpire;                                           \
    PFN_WEBGSO_SET_TIME             SetLsmExpire;                                           \
    PFN_WEBGSO_SET_TIME             SetMaxLifespan;                                         \
    PFN_WEBGSO_GET_TIME             GetMaxLifespan;                                         \
    PFN_WEBGSO_RESET                Reset;                                                  \
                                                                                            \
    PFN_WEBGSO_ACQUIRE              AcquireAccess;                                          \
    PFN_WEBGSO_RELEASE              ReleaseAccess;                                          \
    PFN_WEBGSO_ENGAGE               Engage;                                                 \
    PFN_WEBGSO_CANCEL               Cancel;                                                 \
    PFN_WEBGSO_TERMINATE            Terminate;                                              \
    PFN_WEBGSO_LOCK                 Lock;                                                   \
    PFN_WEBGSO_UNLOCK               Unlock;                                                 \
    PFN_WEBGSO_RESET                ResetTimestamp;                                         \
                                                                                            \
    PFN_WEBGSO_GET_ASINFO           GetAuthSessInfo;                                        \
    /* end of object class content */                                                       \

typedef  struct
_WEB_GENERAL_SESSION_OBJECT
{
    WEB_GENERAL_SESSION_CLASS_CONTENT
}
WEB_GENERAL_SESSION_OBJECT,  *PWEB_GENERAL_SESSION_OBJECT;

#define  ACCESS_WEB_GENERAL_SESSION_OBJECT(p)       \
         ACCESS_CONTAINER(p, WEB_GENERAL_SESSION_OBJECT, Linkage)


#endif
