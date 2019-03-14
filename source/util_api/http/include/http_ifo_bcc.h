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

    module:	http_ifo_bcc.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Body Content Carrier Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/01/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_IFO_BCC_
#define  _HTTP_IFO_BCC_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/***********************************************************
    HTTP BODY CONTENT CARRIER INTERFACE OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  HTTP_BCC_EVENT_ARRIVED_BODY_PART           1
#define  HTTP_BCC_EVENT_ARRIVED_BODY                2
#define  HTTP_BCC_EVENT_COMPLETED                   3
#define  HTTP_BCC_EVENT_OVER_PACKED                 4

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_HTTPBCCIF_GET_HEADER)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulHeaderId
    );

typedef  ANSC_STATUS
(*PFN_HTTPBCCIF_ADD_HEADER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHfo
    );

typedef  ULONG
(*PFN_HTTPBCCIF_PREDICT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPBCCIF_NOTIFY)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    );

typedef  ANSC_STATUS
(*PFN_HTTPBCCIF_SERIALIZE)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hSerializeContext
    );

/*
 * HTTP Proxy is an intermediary program which acts as both a server and a client for the purpose
 * of making requests on behalf of other clients. Requests are serviced internally or by passing
 * them on, with possible translation, to other servers. A proxy MUST implement both the client and
 * server requirements of the HTTP specification.
 */
#define  HTTP_BCC_INTERFACE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_HTTPBCCIF_GET_HEADER        GetHeaderField;                                         \
    PFN_HTTPBCCIF_ADD_HEADER        AddHeaderField;                                         \
    PFN_HTTPBCCIF_PREDICT           PredictBodySize;                                        \
    PFN_HTTPBCCIF_NOTIFY            Notify;                                                 \
    PFN_HTTPBCCIF_SERIALIZE         Serialize;                                              \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_BCC_INTERFACE
{
    HTTP_BCC_INTERFACE_CLASS_CONTENT
}
HTTP_BCC_INTERFACE,  *PHTTP_BCC_INTERFACE;

#define  ACCESS_HTTP_BCC_INTERFACE(p)               \
         ACCESS_CONTAINER(p, HTTP_BCC_INTERFACE, Linkage)


#endif
