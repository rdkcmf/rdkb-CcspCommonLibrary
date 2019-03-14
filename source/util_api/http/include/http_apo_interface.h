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

    module:	http_apo_interface.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Http Advanced Proxy Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/04/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_APO_INTERFACE_
#define  _HTTP_APO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "http_properties.h"

#include "ansc_spto_interface.h"
#include "ansc_spto_external_api.h"
#include "ansc_sptowo_interface.h"


/***********************************************************
 PLATFORM INDEPENDENT HTTP ADVANCED PROXY OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  HTTP_APO_INITIAL_PSO_NUMBER                32
#define  HTTP_APO_INITIAL_TRO_NUMBER                64
#define  HTTP_APO_INITIAL_BMO_NUMBER                128

#define  HTTP_APO_MODE_TRANSPARENT                  0x00000001
#define  HTTP_APO_MODE_COMPACT                      0x00000002
#define  HTTP_APO_MODE_WAM_ENABLED                  0x00000010
#define  HTTP_APO_MODE_SBC_ENABLED                  0x00000020
#define  HTTP_APO_MODE_CBC_ENABLED                  0x00000040
#define  HTTP_APO_MODE_PBC_ENABLED                  0x00000080

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_HTTPAPO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPAPO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_HTTPAPO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPAPO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ULONG
(*PFN_HTTPAPO_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPAPO_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    );

typedef  ANSC_STATUS
(*PFN_HTTPAPO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_HTTPAPO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_HTTPAPO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPAPO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPAPO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_HTTPAPO_ACQUIRE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPAPO_RELEASE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPAPO_MANUFACTURE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPAPO_DESTROY)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  BOOL
(*PFN_HTTPAPO_WAM_ACCEPT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hClientInfo
    );

typedef  ULONG
(*PFN_HTTPAPO_WAM_CONNECT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hServerInfo
    );

typedef  ANSC_STATUS
(*PFN_HTTPAPO_WAM_CLOSE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession
    );

typedef  ULONG
(*PFN_HTTPAPO_SBC_REQUEST1)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession,
        ANSC_HANDLE                 hMessage
    );

typedef  ULONG
(*PFN_HTTPAPO_SBC_REQUEST2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession,
        ANSC_HANDLE                 hMessage
    );

typedef  ULONG
(*PFN_HTTPAPO_CBC_REPLY1)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession,
        ANSC_HANDLE                 hMessage
    );

typedef  ULONG
(*PFN_HTTPAPO_CBC_REPLY2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession,
        ANSC_HANDLE                 hMessage
    );

typedef  ANSC_STATUS
(*PFN_HTTPAPO_PBC_RECV1)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession,
        ANSC_HANDLE                 hMessage
    );

typedef  ANSC_STATUS
(*PFN_HTTPAPO_PBC_RECV2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession,
        ANSC_HANDLE                 hMessage
    );

typedef  ANSC_STATUS
(*PFN_HTTPAPO_PBC_COMPLETE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession
    );

/*
 * The HyperText Transfer Protocol (HTTP) is an application-level protocol for distributed, col-
 * laborative, hypermedia information systems. It is a generic, stateless, protocol which can be
 * used for many tasks beyond its use for hypertext, through extension of its request methods,
 * error codes and headers. A feature of HTTP is the typing and negotiation of data representation,
 * allowing systems to be built independently of the data being transferred.
 */
#define  HTTP_ADVANCED_PROXY_CLASS_CONTENT                                                  \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    HTTP_ADVANCED_PROXY_PROPERTY    Property;                                               \
    ANSC_HANDLE                     hSimpleProxy;                                           \
    ANSC_HANDLE                     hSptoWorker;                                            \
    ANSC_HANDLE                     hWamIf;                                                 \
    ANSC_HANDLE                     hSbcIf;                                                 \
    ANSC_HANDLE                     hCbcIf;                                                 \
    ANSC_HANDLE                     hPbcIf;                                                 \
    ANSC_HANDLE                     hHfpIf;                                                 \
    ULONG                           ProxyMode;                                              \
    ANSC_LOCK                       SyncLock;                                               \
    BOOL                            bActive;                                                \
                                                                                            \
    SLIST_HEADER                    PsoSList;                                               \
    ANSC_LOCK                       PsoSListLock;                                           \
    SLIST_HEADER                    TroSList;                                               \
    ANSC_LOCK                       TroSListLock;                                           \
    SLIST_HEADER                    BmoReqSList;                                            \
    ANSC_LOCK                       BmoReqSListLock;                                        \
    SLIST_HEADER                    BmoRepSList;                                            \
    ANSC_LOCK                       BmoRepSListLock;                                        \
                                                                                            \
    PFN_HTTPAPO_GET_IF              GetWamIf;                                               \
    PFN_HTTPAPO_SET_IF              SetWamIf;                                               \
    PFN_HTTPAPO_GET_IF              GetSbcIf;                                               \
    PFN_HTTPAPO_SET_IF              SetSbcIf;                                               \
    PFN_HTTPAPO_GET_IF              GetCbcIf;                                               \
    PFN_HTTPAPO_SET_IF              SetCbcIf;                                               \
    PFN_HTTPAPO_GET_IF              GetPbcIf;                                               \
    PFN_HTTPAPO_SET_IF              SetPbcIf;                                               \
    PFN_HTTPAPO_GET_IF              GetHfpIf;                                               \
    PFN_HTTPAPO_SET_IF              SetHfpIf;                                               \
    PFN_HTTPAPO_GET_MODE            GetProxyMode;                                           \
    PFN_HTTPAPO_SET_MODE            SetProxyMode;                                           \
                                                                                            \
    PFN_HTTPAPO_GET_PROPERTY        GetProperty;                                            \
    PFN_HTTPAPO_SET_PROPERTY        SetProperty;                                            \
    PFN_HTTPAPO_RESET               ResetProperty;                                          \
    PFN_HTTPAPO_RESET               Reset;                                                  \
                                                                                            \
    PFN_HTTPAPO_ENGAGE              Engage;                                                 \
    PFN_HTTPAPO_CANCEL              Cancel;                                                 \
                                                                                            \
    PFN_HTTPAPO_ACQUIRE             AcquirePso;                                             \
    PFN_HTTPAPO_RELEASE             ReleasePso;                                             \
    PFN_HTTPAPO_MANUFACTURE         ManufacturePsoPool;                                     \
    PFN_HTTPAPO_DESTROY             DestroyPsoPool;                                         \
                                                                                            \
    PFN_HTTPAPO_ACQUIRE             AcquireTro;                                             \
    PFN_HTTPAPO_RELEASE             ReleaseTro;                                             \
    PFN_HTTPAPO_MANUFACTURE         ManufactureTroPool;                                     \
    PFN_HTTPAPO_DESTROY             DestroyTroPool;                                         \
                                                                                            \
    PFN_HTTPAPO_ACQUIRE             AcquireBmoReq;                                          \
    PFN_HTTPAPO_RELEASE             ReleaseBmoReq;                                          \
    PFN_HTTPAPO_MANUFACTURE         ManufactureBmoReqPool;                                  \
    PFN_HTTPAPO_DESTROY             DestroyBmoReqPool;                                      \
                                                                                            \
    PFN_HTTPAPO_ACQUIRE             AcquireBmoRep;                                          \
    PFN_HTTPAPO_RELEASE             ReleaseBmoRep;                                          \
    PFN_HTTPAPO_MANUFACTURE         ManufactureBmoRepPool;                                  \
    PFN_HTTPAPO_DESTROY             DestroyBmoRepPool;                                      \
                                                                                            \
    PFN_HTTPAPO_WAM_ACCEPT          WamAccept;                                              \
    PFN_HTTPAPO_WAM_CONNECT         WamConnect;                                             \
    PFN_HTTPAPO_WAM_CLOSE           WamClose;                                               \
                                                                                            \
    PFN_HTTPAPO_SBC_REQUEST1        SbcRequestHeaders;                                      \
    PFN_HTTPAPO_SBC_REQUEST2        SbcRequestBody;                                         \
                                                                                            \
    PFN_HTTPAPO_CBC_REPLY1          CbcReplyHeaders;                                        \
    PFN_HTTPAPO_CBC_REPLY2          CbcReplyBody;                                           \
                                                                                            \
    PFN_HTTPAPO_PBC_RECV1           PbcRecvReqHeaders;                                      \
    PFN_HTTPAPO_PBC_RECV2           PbcRecvReqBody;                                         \
    PFN_HTTPAPO_PBC_COMPLETE        PbcReqComplete;                                         \
    PFN_HTTPAPO_PBC_RECV1           PbcRecvRepHeaders;                                      \
    PFN_HTTPAPO_PBC_RECV2           PbcRecvRepBody;                                         \
    PFN_HTTPAPO_PBC_COMPLETE        PbcRepComplete;                                         \
                                                                                            \
    PFN_SPTOWO_INIT                 WorkerInit;                                             \
    PFN_SPTOWO_UNLOAD               WorkerUnload;                                           \
                                                                                            \
    PFN_SPTOWO_ACCEPT               PrvwoAccept;                                            \
    PFN_SPTOWO_SETOUT               PrvwoSetOut;                                            \
    PFN_SPTOWO_REMOVE               PrvwoRemove;                                            \
    PFN_SPTOWO_QUERY                PrvwoQuery;                                             \
    PFN_SPTOWO_PROCESS              PrvwoProcessSync;                                       \
    PFN_SPTOWO_PROCESS              PrvwoProcessAsync;                                      \
    PFN_SPTOWO_COMPLETE             PrvwoSendComplete;                                      \
    PFN_SPTOWO_NOTIFY               PrvwoNotify;                                            \
                                                                                            \
    PFN_SPTOWO_ACCEPT               PubwoAccept;                                            \
    PFN_SPTOWO_REMOVE               PubwoRemove;                                            \
    PFN_SPTOWO_QUERY                PubwoQuery;                                             \
    PFN_SPTOWO_PROCESS              PubwoProcessSync;                                       \
    PFN_SPTOWO_PROCESS              PubwoProcessAsync;                                      \
    PFN_SPTOWO_COMPLETE             PubwoSendComplete;                                      \
    PFN_SPTOWO_NOTIFY               PubwoNotify;                                            \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_ADVANCED_PROXY_OBJECT
{
    HTTP_ADVANCED_PROXY_CLASS_CONTENT
}
HTTP_ADVANCED_PROXY_OBJECT,  *PHTTP_ADVANCED_PROXY_OBJECT;

#define  ACCESS_HTTP_ADVANCED_PROXY_OBJECT(p)       \
         ACCESS_CONTAINER(p, HTTP_ADVANCED_PROXY_OBJECT, Linkage)


#endif
