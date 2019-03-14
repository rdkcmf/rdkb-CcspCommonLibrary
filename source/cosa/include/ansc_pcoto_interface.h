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

    module:	ansc_pcoto_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for PCO Transmitter Object.

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


#ifndef  _ANSC_PCOTO_INTERFACE_
#define  _ANSC_PCOTO_INTERFACE_


/***********************************************************
      TRANSMITTER OBJECT DEFINITION FOR PIPE CONNECTOR
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_PCOTO_SEND)
    (
        ANSC_HANDLE                 hTransmitter,
        ANSC_HANDLE                 hSendContext,
        PVOID                       buffer,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_PCOTO_RELEASE)
    (
        ANSC_HANDLE                 hTransmitter,
        PVOID                       buffer,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_PCOTO_SEND2)
    (
        ANSC_HANDLE                 hTransmitter,
        ANSC_HANDLE                 hAnscMsg
    );

typedef  ANSC_STATUS
(*PFN_PCOTO_RELEASE2)
    (
        ANSC_HANDLE                 hTransmitter,
        ANSC_HANDLE                 hAnscMsg
    );

/*
 * Unlike the previous version of VPCom, in which the wrapper is part of the container, the
 * container is now 100% portable for different embedded platforms. The os-specific wrapper has to
 * be rewritten for each platform. The wrapper creates this wrapper object during initialization
 * and passes it to container.
 */
typedef  struct
_ANSC_PCO_TRANSMITTER_OBJECT
{
    ANSC_HANDLE                     hTransmitterContext;

    PFN_PCOTO_SEND                  Send;
    PFN_PCOTO_RELEASE               ReleaseBuffer;
    PFN_PCOTO_SEND2                 Send2;
    PFN_PCOTO_RELEASE2              ReleaseBuffer2;
}
ANSC_PCO_TRANSMITTER_OBJECT,  *PANSC_PCO_TRANSMITTER_OBJECT;


#endif
