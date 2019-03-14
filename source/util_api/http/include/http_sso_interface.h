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

    module:	http_sso_interface.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Http Simple Server Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/07/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_SSO_INTERFACE_
#define  _HTTP_SSO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "http_properties.h"

#include "ansc_dsto_interface.h"
#include "ansc_dsto_external_api.h"
#include "ansc_dstowo_interface.h"


/***********************************************************
  PLATFORM INDEPENDENT HTTP SIMPLE SERVER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  HTTP_SSO_INITIAL_WSSO_NUMBER               32
#define  HTTP_SSO_INITIAL_WSTO_NUMBER               64
#define  HTTP_SSO_INITIAL_BMO_NUMBER                128

#define  HTTP_SSO_MODE_COMPACT                      0x00000001
#define  HTTP_SSO_MODE_TLS_ENABLED                  0x00000002
#define  HTTP_SSO_MODE_TLS_AUTH_CLIENT              0x00000004

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_HTTPSSO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPSSO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_HTTPSSO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPSSO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ULONG
(*PFN_HTTPSSO_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPSSO_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    );

typedef  ANSC_STATUS
(*PFN_HTTPSSO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_HTTPSSO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_HTTPSSO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPSSO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPSSO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_HTTPSSO_ACQUIRE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPSSO_RELEASE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPSSO_MANUFACTURE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPSSO_DESTROY)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  BOOL
(*PFN_HTTPSSO_WSP_ACCEPT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hClientInfo
    );

typedef  ANSC_STATUS
(*PFN_HTTPSSO_WSP_SERVE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWebsSession,
        ANSC_HANDLE                 hWebsTrans,
        ANSC_HANDLE                 hMessage
    );

/*
 * The HyperText Transfer Protocol (HTTP) is an application-level protocol for distributed, col-
 * laborative, hypermedia information systems. It is a generic, stateless, protocol which can be
 * used for many tasks beyond its use for hypertext, through extension of its request methods,
 * error codes and headers. A feature of HTTP is the typing and negotiation of data representation,
 * allowing systems to be built independently of the data being transferred.
 */
#define  HTTP_SIMPLE_SERVER_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    HTTP_SIMPLE_SERVER_PROPERTY     Property;                                               \
    ANSC_HANDLE                     hDaemonServer;                                          \
    ANSC_HANDLE                     hDstoWorker;                                            \
    ANSC_HANDLE                     hWspIf;                                                 \
    ANSC_HANDLE                     hHfpIf;                                                 \
    ANSC_HANDLE                     hFumIf;                                                 \
    ULONG                           ServerMode;                                             \
    ANSC_LOCK                       SyncLock;                                               \
    BOOL                            bActive;                                                \
                                                                                            \
    SLIST_HEADER                    WssoSList;                                              \
    ANSC_LOCK                       WssoSListLock;                                          \
    SLIST_HEADER                    WstoSList;                                              \
    ANSC_LOCK                       WstoSListLock;                                          \
    SLIST_HEADER                    BmoReqSList;                                            \
    ANSC_LOCK                       BmoReqSListLock;                                        \
    SLIST_HEADER                    BmoRepSList;                                            \
    ANSC_LOCK                       BmoRepSListLock;                                        \
                                                                                            \
    PFN_HTTPSSO_GET_IF              GetWspIf;                                               \
    PFN_HTTPSSO_SET_IF              SetWspIf;                                               \
    PFN_HTTPSSO_GET_IF              GetHfpIf;                                               \
    PFN_HTTPSSO_SET_IF              SetHfpIf;                                               \
    PFN_HTTPSSO_GET_IF              GetFumIf;                                               \
    PFN_HTTPSSO_SET_IF              SetFumIf;                                               \
    PFN_HTTPSSO_GET_MODE            GetServerMode;                                          \
    PFN_HTTPSSO_SET_MODE            SetServerMode;                                          \
    PFN_HTTPSSO_GET_PROPERTY        GetProperty;                                            \
    PFN_HTTPSSO_SET_PROPERTY        SetProperty;                                            \
    PFN_HTTPSSO_RESET               ResetProperty;                                          \
    PFN_HTTPSSO_RESET               Reset;                                                  \
                                                                                            \
    PFN_HTTPSSO_ENGAGE              Engage;                                                 \
    PFN_HTTPSSO_CANCEL              Cancel;                                                 \
                                                                                            \
    PFN_HTTPSSO_ACQUIRE             AcquireWsso;                                            \
    PFN_HTTPSSO_RELEASE             ReleaseWsso;                                            \
    PFN_HTTPSSO_MANUFACTURE         ManufactureWssoPool;                                    \
    PFN_HTTPSSO_DESTROY             DestroyWssoPool;                                        \
                                                                                            \
    PFN_HTTPSSO_ACQUIRE             AcquireWsto;                                            \
    PFN_HTTPSSO_RELEASE             ReleaseWsto;                                            \
    PFN_HTTPSSO_MANUFACTURE         ManufactureWstoPool;                                    \
    PFN_HTTPSSO_DESTROY             DestroyWstoPool;                                        \
                                                                                            \
    PFN_HTTPSSO_ACQUIRE             AcquireBmoReq;                                          \
    PFN_HTTPSSO_RELEASE             ReleaseBmoReq;                                          \
    PFN_HTTPSSO_MANUFACTURE         ManufactureBmoReqPool;                                  \
    PFN_HTTPSSO_DESTROY             DestroyBmoReqPool;                                      \
                                                                                            \
    PFN_HTTPSSO_ACQUIRE             AcquireBmoRep;                                          \
    PFN_HTTPSSO_RELEASE             ReleaseBmoRep;                                          \
    PFN_HTTPSSO_MANUFACTURE         ManufactureBmoRepPool;                                  \
    PFN_HTTPSSO_DESTROY             DestroyBmoRepPool;                                      \
                                                                                            \
    PFN_HTTPSSO_WSP_ACCEPT          WspAccept;                                              \
    PFN_HTTPSSO_WSP_SERVE           WspServe;                                               \
                                                                                            \
    PFN_DSTOWO_INIT                 WorkerInit;                                             \
    PFN_DSTOWO_UNLOAD               WorkerUnload;                                           \
                                                                                            \
    PFN_DSTOWO_ACCEPT               DstowoAccept;                                           \
    PFN_DSTOWO_SETOUT               DstowoSetOut;                                           \
    PFN_DSTOWO_REMOVE               DstowoRemove;                                           \
    PFN_DSTOWO_QUERY                DstowoQuery;                                            \
    PFN_DSTOWO_PROCESS              DstowoProcessSync;                                      \
    PFN_DSTOWO_PROCESS              DstowoProcessAsync;                                     \
    PFN_DSTOWO_COMPLETE             DstowoSendComplete;                                     \
    PFN_DSTOWO_NOTIFY               DstowoNotify;                                           \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_SIMPLE_SERVER_OBJECT
{
    HTTP_SIMPLE_SERVER_CLASS_CONTENT
}
HTTP_SIMPLE_SERVER_OBJECT,  *PHTTP_SIMPLE_SERVER_OBJECT;

#define  ACCESS_HTTP_SIMPLE_SERVER_OBJECT(p)        \
         ACCESS_CONTAINER(p, HTTP_SIMPLE_SERVER_OBJECT, Linkage)


#endif
