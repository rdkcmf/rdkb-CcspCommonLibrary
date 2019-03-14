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

    module:	http_ifo_pbc.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Proxy Behavior Controller Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/03/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_IFO_PBC_
#define  _HTTP_IFO_PBC_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/***********************************************************
 HTTP PROXY BEHAVIOR CONTROLLER INTERFACE OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_HTTPPBCIF_RECV1)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession,
        ANSC_HANDLE                 hMessage
    );

typedef  ANSC_STATUS
(*PFN_HTTPPBCIF_RECV2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession,
        ANSC_HANDLE                 hMessage
    );

typedef  ANSC_STATUS
(*PFN_HTTPPBCIF_COMPLETE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession
    );

/*
 * HTTP Proxy is an intermediary program which acts as both a server and a client for the purpose
 * of making requests on behalf of other clients. Recvs are serviced internally or by passing
 * them on, with possible translation, to other servers. A proxy MUST implement both the client and
 * server requirements of the HTTP specification.
 */
#define  HTTP_PBC_INTERFACE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_HTTPPBCIF_RECV1             RecvReqHeaders;                                         \
    PFN_HTTPPBCIF_RECV2             RecvReqBody;                                            \
    PFN_HTTPPBCIF_COMPLETE          ReqComplete;                                            \
                                                                                            \
    PFN_HTTPPBCIF_RECV1             RecvRepHeaders;                                         \
    PFN_HTTPPBCIF_RECV2             RecvRepBody;                                            \
    PFN_HTTPPBCIF_COMPLETE          RepComplete;                                            \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_PBC_INTERFACE
{
    HTTP_PBC_INTERFACE_CLASS_CONTENT
}
HTTP_PBC_INTERFACE,  *PHTTP_PBC_INTERFACE;

#define  ACCESS_HTTP_PBC_INTERFACE(p)               \
         ACCESS_CONTAINER(p, HTTP_PBC_INTERFACE, Linkage)


#endif
