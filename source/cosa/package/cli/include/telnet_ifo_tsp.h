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

    module:	telnet_ifo_tsp.h

        For TELNET,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Terminal Service Provider Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        06/09/05    initial revision.

**********************************************************************/


#ifndef  _TELNET_IFO_TSP_
#define  _TELNET_IFO_TSP_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/***********************************************************
    TELNET SERVICE PROVIDER INTERFACE OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
typedef  struct
_TELNET_TSP_CLIENT_INFO
{
    ANSC_IPV4_ADDRESS               Address;
    USHORT                          Port;
}
TELNET_TSP_CLIENT_INFO,  *PTELNET_TSP_CLIENT_INFO;

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  BOOL
(*PFN_TELNETTSPIF_ACCEPT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hClientInfo
    );

typedef  ANSC_STATUS
(*PFN_TELNETTSPIF_SERVE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTelnetSession,
        ANSC_HANDLE                 hExecutionProp,     /* _TELNET_CMD_EXECUTION_ENV */
        PVOID                       pCmd,
        ULONG                       ulCmdSize
    );

typedef  ANSC_STATUS
(*PFN_TELNETTSPIF_SIGNAL)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTelnetSession,
        ANSC_HANDLE                 hExecutionProp,
        ULONG                       ulTspSignal
    );

/*
 * TELNET Server is an application program that accepts connections in order to service requests by
 * sending back responses. Any given program may be capable of being both a client and a server;
 * our use of these terms refers only to the role being performed by the program for a particular
 * connection, rather than to the program's capabilities in general. Likewise, any server may act
 * as an origin server, proxy, gateway, or tunnel, switching behavior based on the nature of each
 * request.
 */
#define  TELNET_TSP_INTERFACE_CLASS_CONTENT                                                 \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_TELNETTSPIF_ACCEPT          Accept;                                                 \
    PFN_TELNETTSPIF_SERVE           Serve;                                                  \
    PFN_TELNETTSPIF_SIGNAL          Signal;                                                 \
    /* end of object class content */                                                       \

typedef  struct
_TELNET_TSP_INTERFACE
{
    TELNET_TSP_INTERFACE_CLASS_CONTENT
}
TELNET_TSP_INTERFACE,  *PTELNET_TSP_INTERFACE;

#define  ACCESS_TELNET_TSP_INTERFACE(p)               \
         ACCESS_CONTAINER(p, TELNET_TSP_INTERFACE, Linkage)


#endif
