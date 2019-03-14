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

    module:	http_ifo_wam.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Web Access Manager Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/25/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_IFO_WAM_
#define  _HTTP_IFO_WAM_


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
#define  HTTP_WAM_PMODE_RESERVED                    0
#define  HTTP_WAM_PMODE_PASS                        1
#define  HTTP_WAM_PMODE_PEND                        2
#define  HTTP_WAM_PMODE_DENY                        3

typedef  struct
_HTTP_WAM_CLIENT_INFO
{
    ANSC_IPV4_ADDRESS               Address;
    USHORT                          Port;
}
HTTP_WAM_CLIENT_INFO,  *PHTTP_WAM_CLIENT_INFO;

typedef  struct
_HTTP_WAM_SERVER_INFO
{
    char                            HostName[ANSC_DOMAIN_NAME_SIZE];
    USHORT                          Port;
}
HTTP_WAM_SERVER_INFO,  *PHTTP_WAM_SERVER_INFO;

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  BOOL
(*PFN_HTTPWAMIF_ACCEPT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hClientInfo
    );

typedef  ULONG
(*PFN_HTTPWAMIF_CONNECT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hServerInfo
    );

typedef  ANSC_STATUS
(*PFN_HTTPWAMIF_CLOSE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession
    );

/*
 * HTTP Proxy is an intermediary program which acts as both a server and a client for the purpose
 * of making requests on behalf of other clients. Requests are serviced internally or by passing
 * them on, with possible translation, to other servers. A proxy MUST implement both the client and
 * server requirements of the HTTP specification.
 */
#define  HTTP_WAM_INTERFACE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_HTTPWAMIF_ACCEPT            Accept;                                                 \
    PFN_HTTPWAMIF_CONNECT           Connect;                                                \
    PFN_HTTPWAMIF_CLOSE             Close;                                                  \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_WAM_INTERFACE
{
    HTTP_WAM_INTERFACE_CLASS_CONTENT
}
HTTP_WAM_INTERFACE,  *PHTTP_WAM_INTERFACE;

#define  ACCESS_HTTP_WAM_INTERFACE(p)               \
         ACCESS_CONTAINER(p, HTTP_WAM_INTERFACE, Linkage)


#endif
