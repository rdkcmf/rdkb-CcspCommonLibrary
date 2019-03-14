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

    module:	http_ifo_bsp.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Browsing Service Provider Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        04/21/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_IFO_BSP_
#define  _HTTP_IFO_BSP_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/***********************************************************
 HTTP BROWSING SERVICE PROVIDER INTERFACE OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  HTTP_BSP_INTERFACE_NAME                    "httpBrowsingServiceProviderIf"
#define  HTTP_BSP_INTERFACE_ID                      0

#define  HTTP_BSP_EVENT_BAD_REQUEST                 1
#define  HTTP_BSP_EVENT_CANNOT_RESOLVE_NAME         2
#define  HTTP_BSP_EVENT_SERVER_UNAVAILABLE          3
#define  HTTP_BSP_EVENT_NETWORK_FAILURE             4
#define  HTTP_BSP_EVENT_TIMEOUT                     5
#define  HTTP_BSP_EVENT_TLS_TIMEOUT                 6
#define  HTTP_BSP_EVENT_TLS_ERROR                   7
#define  HTTP_BSP_EVENT_HTTP_ERROR_STATUS           8
#define  HTTP_BSP_EVENT_SOCKET_CLOSED               9
#define  HTTP_BSP_EVENT_SESSION_RECYCLED            10

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_HTTPBSPIF_POLISH)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hReqContext
    );

typedef  ANSC_STATUS
(*PFN_HTTPBSPIF_BROWSE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hReqContext
    );

typedef  ANSC_STATUS
(*PFN_HTTPBSPIF_NOTIFY)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulEvent,
        ULONG                       ulError,
        ANSC_HANDLE                 hReqContext,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    );

/*
 * HTTP Server is an application program that accepts connections in order to service requests by
 * sending back responses. Any given program may be capable of being both a client and a server;
 * our use of these terms refers only to the role being performed by the program for a particular
 * connection, rather than to the program's capabilities in general. Likewise, any server may act
 * as an origin server, proxy, gateway, or tunnel, switching behavior based on the nature of each
 * request.
 */
#define  HTTP_BSP_INTERFACE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_HTTPBSPIF_POLISH            Polish;                                                 \
    PFN_HTTPBSPIF_BROWSE            Browse;                                                 \
    PFN_HTTPBSPIF_NOTIFY            Notify;                                                 \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_BSP_INTERFACE
{
    HTTP_BSP_INTERFACE_CLASS_CONTENT
}
HTTP_BSP_INTERFACE,  *PHTTP_BSP_INTERFACE;

#define  ACCESS_HTTP_BSP_INTERFACE(p)               \
         ACCESS_CONTAINER(p, HTTP_BSP_INTERFACE, Linkage)


#endif
