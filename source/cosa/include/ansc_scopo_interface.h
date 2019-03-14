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

    module:	ansc_scopo_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for Sco Port Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        10/30/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_SCOPO_INTERFACE_
#define  _ANSC_SCOPO_INTERFACE_


/***********************************************************
      PORT OBJECT DEFINITION FOR SIMPLEX CONNECTOR
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_SCOPO_RELAY)
    (
        ANSC_HANDLE                 hPort,
        ANSC_HANDLE                 hRelayContext,
        PVOID                       buffer,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_SCOPO_RELEASE)
    (
        ANSC_HANDLE                 hPort,
        PVOID                       buffer,
        ULONG                       ulSize
    );

/*
 * Unlike the previous version of VPCom, in which the wrapper is part of the container, the
 * container is now 100% portable for different embedded platforms. The os-specific wrapper has to
 * be rewritten for each platform. The wrapper creates this wrapper object during initialization
 * and passes it to container.
 */
typedef  struct
_ANSC_SCO_PORT_OBJECT
{
    ANSC_HANDLE                     hPortContext;

    PFN_SCOPO_RELAY                 Relay;
    PFN_SCOPO_RELEASE               ReleaseBuffer;
}
ANSC_SCO_PORT_OBJECT,  *PANSC_SCO_PORT_OBJECT;


#endif
