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

    module:	http_ifo_sbc.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Server Behavior Controller Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/20/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_IFO_SBC_
#define  _HTTP_IFO_SBC_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/***********************************************************
 HTTP SERVER BEHAVIOR CONTROLLER INTERFACE OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  HTTP_SBC_PMODE_RESERVED                    0
#define  HTTP_SBC_PMODE_RELAY1                      1   /* only relay this transaction        */
#define  HTTP_SBC_PMODE_RELAY2                      2   /* relay all transactions from now on */
#define  HTTP_SBC_PMODE_STORE                       3
#define  HTTP_SBC_PMODE_QUEUE                       4
#define  HTTP_SBC_PMODE_RESET                       5
#define  HTTP_SBC_PMODE_CLOSE                       6

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ULONG
(*PFN_HTTPSBCIF_REQUEST1)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession,
        ANSC_HANDLE                 hMessage
    );

typedef  ULONG
(*PFN_HTTPSBCIF_REQUEST2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession,
        ANSC_HANDLE                 hMessage
    );

/*
 * HTTP Proxy is an intermediary program which acts as both a server and a client for the purpose
 * of making requests on behalf of other clients. Requests are serviced internally or by passing
 * them on, with possible translation, to other servers. A proxy MUST implement both the client and
 * server requirements of the HTTP specification.
 */
#define  HTTP_SBC_INTERFACE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_HTTPSBCIF_REQUEST1          RequestHeaders;                                         \
    PFN_HTTPSBCIF_REQUEST2          RequestBody;                                            \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_SBC_INTERFACE
{
    HTTP_SBC_INTERFACE_CLASS_CONTENT
}
HTTP_SBC_INTERFACE,  *PHTTP_SBC_INTERFACE;

#define  ACCESS_HTTP_SBC_INTERFACE(p)               \
         ACCESS_CONTAINER(p, HTTP_SBC_INTERFACE, Linkage)


#endif
