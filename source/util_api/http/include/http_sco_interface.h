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

    module:	http_sco_interface.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Http Simple Client Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        04/20/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_SCO_INTERFACE_
#define  _HTTP_SCO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "http_properties.h"

#include "http_ifo_hfp.h"
#include "http_ifo_bsp.h"
#include "http_ifo_acm.h"


/***********************************************************
  PLATFORM INDEPENDENT HTTP SIMPLE CLIENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  HTTP_SCO_INITIAL_WCSO_NUMBER               32
#define  HTTP_SCO_INITIAL_WCTO_NUMBER               64
#define  HTTP_SCO_INITIAL_BMO_NUMBER                128

#define  HTTP_SCO_MODE_COMPACT                      0x00000001
#define  HTTP_SCO_MODE_XSOCKET                      0x00000002
#define  HTTP_SCO_MODE_NOTIFY_ON_ALL_CONN_ONCE      0x00000004

#define  HTTP_SCO_WCSO_TABLE_SIZE                   64
#define  HTTP_SCO_SESSION_TIME_INTERVAL             3 * 1000    /* in milli-seconds */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_HTTPSCO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPSCO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_HTTPSCO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPSCO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ULONG
(*PFN_HTTPSCO_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPSCO_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    );

typedef  char*
(*PFN_HTTPSCO_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPSCO_SET_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ANSC_STATUS
(*PFN_HTTPSCO_SET_TIMEOUT)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulTimeout
    );

typedef  ANSC_STATUS
(*PFN_HTTPSCO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_HTTPSCO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_HTTPSCO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPSCO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPSCO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPSCO_TDO_INVOKE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_HTTPSCO_ACQUIRE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPSCO_RELEASE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPSCO_MANUFACTURE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPSCO_DESTROY)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPSCO_REQUEST)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMethod,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext,
        BOOL                        bUseTls
    );

typedef  ANSC_STATUS
(*PFN_HTTPSCO_REQUEST2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       request_line,
        ANSC_HANDLE                 hReqContext
    );

typedef  ANSC_STATUS
(*PFN_HTTPSCO_REQUEST3)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMethod,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext,
        BOOL                        bUseTls,
        char*                       pUserName,
        char*                       pPassword
    );

typedef  ANSC_STATUS
(*PFN_HTTPSCO_DO_OPTIONS)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext,
        BOOL                        bUseTls
    );

typedef  ANSC_STATUS
(*PFN_HTTPSCO_DO_GET)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext,
        BOOL                        bUseTls
    );

typedef  ANSC_STATUS
(*PFN_HTTPSCO_DO_HEAD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext,
        BOOL                        bUseTls
    );

typedef  ANSC_STATUS
(*PFN_HTTPSCO_DO_POST)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext,
        BOOL                        bUseTls
    );

typedef  ANSC_STATUS
(*PFN_HTTPSCO_DO_PUT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext,
        BOOL                        bUseTls
    );

typedef  ANSC_STATUS
(*PFN_HTTPSCO_DO_DELETE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext,
        BOOL                        bUseTls
    );

typedef  ANSC_STATUS
(*PFN_HTTPSCO_DO_TRACE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext,
        BOOL                        bUseTls
    );

typedef  ANSC_STATUS
(*PFN_HTTPSCO_DO_CONNECT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext,
        BOOL                        bUseTls
    );

typedef  ANSC_HANDLE
(*PFN_HTTPSCO_MAP_WCSO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       host,
        USHORT                      port,
        ANSC_HANDLE                 hReqContext,
        BOOL                        bUseTls
    );

typedef  ANSC_HANDLE
(*PFN_HTTPSCO_GET_WCSO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       host,
        USHORT                      port,
        ANSC_HANDLE                 hReqContext,
        BOOL                        bUseTls
    );

typedef  ANSC_HANDLE
(*PFN_HTTPSCO_ADD_WCSO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       host,
        USHORT                      port,
        ANSC_HANDLE                 hReqContext,
        BOOL                        bUseTls
    );

typedef  ANSC_STATUS
(*PFN_HTTPSCO_DEL_ALL_WCSOS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPSCO_SET_PEER_ADDRS)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPeerAddresses,
        PBOOL                       pbSameAsCurrent
    );

typedef  BOOL
(*PFN_HTTPSCO_GET_NEXT_ADDR)
    (
        ANSC_HANDLE                 hThisObject,
        uint32_t                   *pPeerAddr
    );

typedef  ULONG
(*PFN_HTTPSCO_GET_CUR_ADDR)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPSCO_RESET_ADDR_PICK)
    (
        ANSC_HANDLE                 hThisObject,
        uint32_t                    ipAddr
    );

typedef  ANSC_STATUS
(*PFN_HTTPSCO_GET_HOSTNAMES)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHostNames
    );

typedef  ANSC_STATUS
(*PFN_HTTPSCO_SET_HOSTNAMES)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHostNames
    );


/*
 * The HyperText Transfer Protocol (HTTP) is an application-level protocol for distributed, col-
 * laborative, hypermedia information systems. It is a generic, stateless, protocol which can be
 * used for many tasks beyond its use for hypertext, through extension of its request methods,
 * error codes and headers. A feature of HTTP is the typing and negotiation of data representation,
 * allowing systems to be built independently of the data being transferred.
 */
#define  HTTP_SIMPLE_CLIENT_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    HTTP_SIMPLE_CLIENT_PROPERTY     Property;                                               \
    ANSC_HANDLE                     hBspIf;                                                 \
    ANSC_HANDLE                     hHfpIf;                                                 \
    ANSC_HANDLE                     hSessionTdo;                                            \
    ANSC_HANDLE                     hSessionTdoClient;                                      \
    ULONG                           ClientMode;                                             \
    ANSC_HANDLE                     hCasIf;                                                 \
    ANSC_HANDLE                     hClientAuthObj;                                         \
    BOOL                            bActive;                                                \
    char*                           pAuthUserName;                                          \
    char*                           pAuthPassword;                                          \
    ANSC_HANDLE                     hAcmIf;                                                 \
    ANSC_HANDLE                     hWebClientSession;                                      \
    ULONG			    SessionIdleTimeout;					    \
    /* used to verify peer */								    \
    HTTP_SCO_HOST_NAMES		    hostNames;	            	    			    \
                                                                                            \
    SLIST_HEADER                    WcsoTable[HTTP_SCO_WCSO_TABLE_SIZE];                    \
    ANSC_LOCK                       WcsoTableLock;                                          \
                                                                                            \
    SLIST_HEADER                    WcsoSList;                                              \
    ANSC_LOCK                       WcsoSListLock;                                          \
    SLIST_HEADER                    WctoSList;                                              \
    ANSC_LOCK                       WctoSListLock;                                          \
    SLIST_HEADER                    BmoReqSList;                                            \
    ANSC_LOCK                       BmoReqSListLock;                                        \
    SLIST_HEADER                    BmoRepSList;                                            \
    ANSC_LOCK                       BmoRepSListLock;                                        \
                                                                                            \
    HTTP_SCO_HOST_ADDRESSES         PeerAddresses;                                          \
    ULONG                           LastPicked;                                             \
                                                                                            \
    PFN_HTTPSCO_GET_IF              GetBspIf;                                               \
    PFN_HTTPSCO_SET_IF              SetBspIf;                                               \
    PFN_HTTPSCO_GET_IF              GetHfpIf;                                               \
    PFN_HTTPSCO_SET_IF              SetHfpIf;                                               \
    PFN_HTTPSCO_GET_IF              GetCasIf;                                               \
    PFN_HTTPSCO_GET_CONTEXT         GetClientAuthObj;                                       \
    PFN_HTTPSCO_GET_MODE            GetClientMode;                                          \
    PFN_HTTPSCO_SET_MODE            SetClientMode;                                          \
    PFN_HTTPSCO_GET_NAME            GetProductName;                                         \
    PFN_HTTPSCO_SET_NAME            SetProductName;                                         \
    PFN_HTTPSCO_GET_HOSTNAMES       GetHostNames;                                          \
    PFN_HTTPSCO_SET_HOSTNAMES       SetHostNames;                                          \
    PFN_HTTPSCO_SET_TIMEOUT	    SetSessionIdleTimeout;			    	    \
    PFN_HTTPSCO_GET_PROPERTY        GetProperty;                                            \
    PFN_HTTPSCO_SET_PROPERTY        SetProperty;                                            \
    PFN_HTTPSCO_RESET               ResetProperty;                                          \
    PFN_HTTPSCO_RESET               Reset;                                                  \
                                                                                            \
    PFN_HTTPSCO_ENGAGE              Engage;                                                 \
    PFN_HTTPSCO_CANCEL              Cancel;                                                 \
    PFN_HTTPSCO_TDO_INVOKE          SessionTdoInvoke;                                       \
                                                                                            \
    PFN_HTTPSCO_ACQUIRE             AcquireWcso;                                            \
    PFN_HTTPSCO_RELEASE             ReleaseWcso;                                            \
    PFN_HTTPSCO_MANUFACTURE         ManufactureWcsoPool;                                    \
    PFN_HTTPSCO_DESTROY             DestroyWcsoPool;                                        \
                                                                                            \
    PFN_HTTPSCO_ACQUIRE             AcquireWcto;                                            \
    PFN_HTTPSCO_RELEASE             ReleaseWcto;                                            \
    PFN_HTTPSCO_MANUFACTURE         ManufactureWctoPool;                                    \
    PFN_HTTPSCO_DESTROY             DestroyWctoPool;                                        \
                                                                                            \
    PFN_HTTPSCO_ACQUIRE             AcquireBmoReq;                                          \
    PFN_HTTPSCO_RELEASE             ReleaseBmoReq;                                          \
    PFN_HTTPSCO_MANUFACTURE         ManufactureBmoReqPool;                                  \
    PFN_HTTPSCO_DESTROY             DestroyBmoReqPool;                                      \
                                                                                            \
    PFN_HTTPSCO_ACQUIRE             AcquireBmoRep;                                          \
    PFN_HTTPSCO_RELEASE             ReleaseBmoRep;                                          \
    PFN_HTTPSCO_MANUFACTURE         ManufactureBmoRepPool;                                  \
    PFN_HTTPSCO_DESTROY             DestroyBmoRepPool;                                      \
                                                                                            \
    PFN_HTTPSCO_REQUEST             Request;                                                \
    PFN_HTTPSCO_REQUEST2            Request2;                                               \
    PFN_HTTPSCO_REQUEST3            Request3;                                               \
    PFN_HTTPSCO_DO_OPTIONS          DoOptions;                                              \
    PFN_HTTPSCO_DO_GET              DoGet;                                                  \
    PFN_HTTPSCO_DO_HEAD             DoHead;                                                 \
    PFN_HTTPSCO_DO_POST             DoPost;                                                 \
    PFN_HTTPSCO_DO_PUT              DoPut;                                                  \
    PFN_HTTPSCO_DO_DELETE           DoDelete;                                               \
    PFN_HTTPSCO_DO_TRACE            DoTrace;                                                \
    PFN_HTTPSCO_DO_CONNECT          DoConnect;                                              \
                                                                                            \
    PFN_HTTPSCO_MAP_WCSO            MapWcso;                                                \
    PFN_HTTPSCO_GET_WCSO            GetWcso;                                                \
    PFN_HTTPSCO_ADD_WCSO            AddWcso;                                                \
    PFN_HTTPSCO_DEL_ALL_WCSOS       DelAllWcsos;                                            \
                                                                                            \
    PFN_HTTPBSPIF_POLISH            BspPolish;                                              \
    PFN_HTTPBSPIF_BROWSE            BspBrowse;                                              \
    PFN_HTTPBSPIF_NOTIFY            BspNotify;                                              \
                                                                                            \
    PFN_HTTPSCO_SET_PEER_ADDRS      SetPeerAddresses;                                       \
    PFN_HTTPSCO_GET_NEXT_ADDR       GetNextPeerAddr;                                        \
    PFN_HTTPSCO_GET_CUR_ADDR        GetCurPeerAddr;                                         \
    PFN_HTTPSCO_RESET_ADDR_PICK     ResetPeerAddrPick;                                      \
                                                                                            \
    PFN_HTTPACMIF_GET_CREDENTIAL    GetCredential;                                          \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_SIMPLE_CLIENT_OBJECT
{
    HTTP_SIMPLE_CLIENT_CLASS_CONTENT
}
HTTP_SIMPLE_CLIENT_OBJECT,  *PHTTP_SIMPLE_CLIENT_OBJECT;

#define  ACCESS_HTTP_SIMPLE_CLIENT_OBJECT(p)        \
         ACCESS_CONTAINER(p, HTTP_SIMPLE_CLIENT_OBJECT, Linkage)


#endif
