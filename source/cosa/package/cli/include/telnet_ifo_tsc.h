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

    module:	telnet_ifo_tsc.h

        For TELNET,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Telnet Session Control Objects.

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


#ifndef  _TELNET_IFO_TSC_
#define  _TELNET_IFO_TSC_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/***********************************************************
    TELNET SESSION CONTROL INTERFACE OBJECT DEFINITION
***********************************************************/

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_TELNETTSCIF_CLOSE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession
    );

typedef  ANSC_STATUS
(*PFN_TELNETTSPIF_OUTPUT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        PVOID                       pData,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_TELNETTSCIF_NOTIFY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ULONG                       ulCmdExecEvent,
        ANSC_HANDLE                 hReserved
    );

typedef  ANSC_STATUS
(*PFN_TELNETTSCIF_SEND_DM)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession
    );

typedef  ANSC_STATUS
(*PFN_TELNETTSCIF_SEND_GA)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession
    );

typedef  ANSC_STATUS
(*PFN_TELNETTSCIF_SEND_PROMPT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        BOOL                        bCrlf
    );

typedef  ANSC_STATUS
(*PFN_TELNETTSCIF_SEND_ECHO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        UCHAR                       ucCmdCode
    );

typedef  ANSC_STATUS
(*PFN_TELNETTSCIF_RING_BELL)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession
    );

typedef  ANSC_STATUS
(*PFN_TELNETTSCIF_SET_PROMPT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        PUCHAR                      pPrompt        
    );

typedef  ANSC_STATUS
(*PFN_TELNETTSCIF_SENDCH)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        UCHAR                       ucChar,
        ULONG                       ulCount
    );

/*
 * TELNET TSC interface provides routines to output data to client
 * and event notification.
 */
#define  TELNET_TSC_INTERFACE_CLASS_CONTENT                                                 \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_TELNETTSCIF_CLOSE           Close;                                                  \
    PFN_TELNETTSCIF_NOTIFY          Notify;                                                 \
                                                                                            \
    PFN_TELNETTSPIF_OUTPUT          Output;                                                 \
    PFN_TELNETTSCIF_SEND_PROMPT     SendShellPrompt;                                        \
    PFN_TELNETTSCIF_SET_PROMPT      SetShellPrompt;                                         \
    PFN_TELNETTSCIF_SENDCH          SendChar;                                               \
    PFN_TELNETTSCIF_RING_BELL       RingBell;                                               \
                                                                                            \
    PFN_TELNETTSCIF_SEND_DM         SendDataMark;                                           \
    PFN_TELNETTSCIF_SEND_GA         SendGoAhead;                                            \
    PFN_TELNETTSCIF_SEND_ECHO       SendEcho;                                               \
    /* end of object class content */                                                       \

typedef  struct
_TELNET_TSC_INTERFACE
{
    TELNET_TSC_INTERFACE_CLASS_CONTENT
}
TELNET_TSC_INTERFACE,  *PTELNET_TSC_INTERFACE;

#define  ACCESS_TELNET_TSC_INTERFACE(p)               \
         ACCESS_CONTAINER(p, TELNET_TSC_INTERFACE, Linkage)


#endif
