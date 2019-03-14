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

    module:	web_vho_interface.h

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Web Virtual Host Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/09/02    initial revision.

**********************************************************************/


#ifndef  _WEB_VHO_INTERFACE_
#define  _WEB_VHO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "web_properties.h"

#include "web_ifo_lsm.h"


/***********************************************************
   PLATFORM INDEPENDENT WEB VIRTUAL HOST OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  WEB_VHO_GSO_TABLE_SIZE                     128
#define  WEB_VHO_SESSION_TIME_INTERVAL              10 * 1000   /* in milli-seconds */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_WEBVHO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBVHO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_WEBVHO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBVHO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  char*
(*PFN_WEBVHO_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBVHO_SET_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  USHORT
(*PFN_WEBVHO_GET_PORT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBVHO_SET_PORT)
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usPort
    );

typedef  ULONG
(*PFN_WEBVHO_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBVHO_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    );

typedef  BOOL
(*PFN_WEBVHO_IS_SOMETHING)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBVHO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_WEBVHO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_WEBVHO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBVHO_ACQUIRE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBVHO_RELEASE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBVHO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBVHO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  BOOL
(*PFN_WEBVHO_MATCH_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ANSC_STATUS
(*PFN_WEBVHO_TDO_INVOKE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBVHO_ATTACH_ROO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRoo
    );

typedef  ANSC_STATUS
(*PFN_WEBVHO_DETACH_ROO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    );

typedef  ANSC_STATUS
(*PFN_WEBVHO_DELETE_ROO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    );

typedef  ANSC_HANDLE
(*PFN_WEBVHO_IDENTIFY_GSO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

typedef  ANSC_HANDLE
(*PFN_WEBVHO_GET_GSO)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSessionId,
        char*                       identifier,
        PUCHAR                      address,
        USHORT                      port
    );

typedef  ANSC_HANDLE
(*PFN_WEBVHO_ADD_GSO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       identifier,
        PUCHAR                      address,
        USHORT                      port
    );

typedef  ANSC_STATUS
(*PFN_WEBVHO_DEL_GSO)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSessionId,
        PUCHAR                      address,
        USHORT                      port
    );

typedef  ANSC_STATUS
(*PFN_WEBVHO_DEL_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ULONG
(*PFN_WEBVHO_QUERY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        PANSC_HANDLE                phQueryContext
    );

typedef  ANSC_STATUS
(*PFN_WEBVHO_PROCESS)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hQueryContext
    );

typedef  ANSC_STATUS
(*PFN_WEBVHO_NOTIFY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ULONG                       ulEvent
    );

typedef  ANSC_STATUS
(*PFN_WEBVHO_CONSTRUCT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ULONG                       ulStatusCode
    );

typedef  ANSC_STATUS
(*PFN_WEBVHO_ADD_HEADERS)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoRep
    );

typedef  ANSC_STATUS
(*PFN_WEBVHO_GET_RPATH)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hTokenChain,
        BOOL                        bReferred
    );

typedef  ANSC_STATUS
(*PFN_WEBVHO_SET_HTTP_AUTH)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path,
        ULONG                       http_auth_method,
        ANSC_HANDLE                 hWebAcmIf
    );

typedef  ANSC_STATUS
(*PFN_WEBVHO_SET_HTTP_AUTH2)
    (
		ANSC_HANDLE					hThisObject,
        char*                       path,
        ANSC_HANDLE                 hWebAcmIf,
		ULONG						http_auth_method,   
        ULONG                       alg,                
        BOOL                        bNoQop,
        ULONG                       NonceTimeout,       
        char*                       realm,              
        char*                       domain              
    );

/*
 * The HyperText Transfer Protocol (HTTP) is an application-level protocol for distributed, col-
 * laborative, hypermedia information systems. It is a generic, stateless, protocol which can be
 * used for many tasks beyond its use for hypertext, through extension of its request methods,
 * error codes and headers. A feature of HTTP is the typing and negotiation of data representation,
 * allowing systems to be built independently of the data being transferred.
 */
#define  WEB_VIRTUAL_HOST_CLASS_CONTENT                                                     \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    WEB_VIRTUAL_HOST_PROPERTY       Property;                                               \
    ANSC_HANDLE                     hResourceLocator;                                       \
    ANSC_HANDLE                     hHfpIf;                                                 \
    ANSC_HANDLE                     hLsmIf;                                                 \
    ANSC_HANDLE                     hSessionTdo;                                            \
    ANSC_HANDLE                     hSessionTdoClient;                                      \
    ULONG                           GlobalSidCount;                                         \
    ANSC_LOCK                       SyncLock;                                               \
    BOOL                            bActive;                                                \
                                                                                            \
    SLIST_HEADER                    GsoTable[WEB_VHO_GSO_TABLE_SIZE];                       \
    ANSC_LOCK                       GsoTableLock;                                           \
                                                                                            \
    PFN_WEBVHO_GET_NAME             GetHostName;                                            \
    PFN_WEBVHO_SET_NAME             SetHostName;                                            \
    PFN_WEBVHO_GET_PORT             GetHostPort;                                            \
    PFN_WEBVHO_SET_PORT             SetHostPort;                                            \
    PFN_WEBVHO_GET_IF               GetHfpIf;                                               \
    PFN_WEBVHO_SET_IF               SetHfpIf;                                               \
    PFN_WEBVHO_GET_IF               GetLsmIf;                                               \
    PFN_WEBVHO_SET_IF               SetLsmIf;                                               \
    PFN_WEBVHO_IS_SOMETHING         IsDefaultHost;                                          \
                                                                                            \
    PFN_WEBVHO_GET_PROPERTY         GetProperty;                                            \
    PFN_WEBVHO_SET_PROPERTY         SetProperty;                                            \
    PFN_WEBVHO_RESET                ResetProperty;                                          \
    PFN_WEBVHO_RESET                Reset;                                                  \
                                                                                            \
    PFN_WEBVHO_ACQUIRE              AcquireAccess;                                          \
    PFN_WEBVHO_RELEASE              ReleaseAccess;                                          \
    PFN_WEBVHO_ENGAGE               Engage;                                                 \
    PFN_WEBVHO_CANCEL               Cancel;                                                 \
    PFN_WEBVHO_MATCH_NAME           MatchName;                                              \
    PFN_WEBVHO_TDO_INVOKE           SessionTdoInvoke;                                       \
                                                                                            \
    PFN_WEBVHO_ATTACH_ROO           AttachRoo;                                              \
    PFN_WEBVHO_DETACH_ROO           DetachRoo;                                              \
    PFN_WEBVHO_DELETE_ROO           DeleteRoo;                                              \
                                                                                            \
    PFN_WEBVHO_IDENTIFY_GSO         IdentifyGso;                                            \
    PFN_WEBVHO_GET_GSO              GetGso;                                                 \
    PFN_WEBVHO_ADD_GSO              AddGso;                                                 \
    PFN_WEBVHO_DEL_GSO              DelGso;                                                 \
    PFN_WEBVHO_DEL_ALL              DelAllGsos;                                             \
                                                                                            \
    PFN_WEBVHO_QUERY                Query;                                                  \
    PFN_WEBVHO_PROCESS              Process;                                                \
    PFN_WEBVHO_NOTIFY               Notify;                                                 \
    PFN_WEBVHO_CONSTRUCT            ConstructResponse;                                      \
    PFN_WEBVHO_ADD_HEADERS          AddHttpRepHeaders;                                      \
    PFN_WEBVHO_GET_RPATH            GetResourcePath;                                        \
                                                                                            \
    PFN_WEBLSMIF_NEW                LsmNewContact;                                          \
    PFN_WEBLSMIF_CLASSIFY           LsmClassifyClient;                                      \
    PFN_WEBLSMIF_EXPIRE             LsmExpireSession;                                       \
    PFN_WEBLSMIF_END                LsmEndSession;                                          \
                                                                                            \
    PFN_WEBVHO_SET_HTTP_AUTH        SetHttpAuth;                                            \
    PFN_WEBVHO_SET_HTTP_AUTH2       SetHttpAuth2;                                           \
    /* end of object class content */                                                       \

typedef  struct
_WEB_VIRTUAL_HOST_OBJECT
{
    WEB_VIRTUAL_HOST_CLASS_CONTENT
}
WEB_VIRTUAL_HOST_OBJECT,  *PWEB_VIRTUAL_HOST_OBJECT;

#define  ACCESS_WEB_VIRTUAL_HOST_OBJECT(p)          \
         ACCESS_CONTAINER(p, WEB_VIRTUAL_HOST_OBJECT, Linkage)


#endif
