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

    module:	tls_ifo_cbc.h

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the TLS Conn Behavior Controller Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/25/03    initial revision.

**********************************************************************/


#ifndef  _TLS_IFO_CBC_
#define  _TLS_IFO_CBC_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/***********************************************************
  TLS TRANSPORT SERVICE ADAPTOR INTERFACE OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  TLS_CBC_INTERFACE_NAME                     "tlsConnectionBehaviorController"
#define  TLS_CBC_INTERFACE_ID                       0

#define  TLS_CBC_EVENT_HANDSHAKE_SUCCEEDED          1
#define  TLS_CBC_EVENT_ALERT_RECEIVED               2
#define  TLS_CBC_EVENT_ALERT_GENERATED              3
#define  TLS_CBC_EVENT_CCS_RECEIVED                 4

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_TLSCBCIF_NOTIFY)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulEvent,
        ULONG                       ulError,
        ANSC_HANDLE                 hEventContext
    );

typedef  ANSC_HANDLE
(*PFN_TLSCBCIF_GET_PARAMS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TLSCBCIF_SET_ID)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      session_id,
        ULONG                       session_id_size
    );

typedef  ANSC_STATUS
(*PFN_TLSCBCIF_SET_BFLAG)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bflag
    );

typedef  ANSC_STATUS
(*PFN_TLSCBCIF_SET_ARRAY)
    (
        ANSC_HANDLE                 hThisObject,
        void*                       item_array,
        ULONG                       item_count
    );

typedef  ANSC_STATUS
(*PFN_TLSCBCIF_GEN_ALERT)
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       alert_level,
        UCHAR                       alert_descr
    );

typedef  ANSC_STATUS
(*PFN_TLSCBCIF_CHANGE)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * Four record protocol clients are specified in RFC 2246: the handshake protocol, the alert
 * protocol, the change cipher spec protocol, and the application data protocol. In order to allow
 * extension of the TLS protocol, additional record types can be supported by the record protocol.
 * The TLS_CBC interface centralizes the connection control, including exception handling, session
 * management, and coordination.
 */
#define  TLS_CBC_INTERFACE_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_TLSCBCIF_NOTIFY             NotifyEvent;                                            \
    PFN_TLSCBCIF_GET_PARAMS         GetConnParams;                                          \
    PFN_TLSCBCIF_SET_ID             SetSessionID;                                           \
    PFN_TLSCBCIF_SET_BFLAG          SetSessionSharing;                                      \
    PFN_TLSCBCIF_SET_BFLAG          SetQuickHandshake;                                      \
    PFN_TLSCBCIF_SET_ARRAY          SetCipherSuiteArray;                                    \
    PFN_TLSCBCIF_SET_ARRAY          SetCompressionArray;                                    \
    PFN_TLSCBCIF_GEN_ALERT          GenerateAlert;                                          \
    PFN_TLSCBCIF_CHANGE             ChangeCipherSpec;                                       \
    /* end of object class content */                                                       \

typedef  struct
_TLS_CBC_INTERFACE
{
    TLS_CBC_INTERFACE_CLASS_CONTENT
}
TLS_CBC_INTERFACE,  *PTLS_CBC_INTERFACE;

#define  ACCESS_TLS_CBC_INTERFACE(p)                \
         ACCESS_CONTAINER(p, TLS_CBC_INTERFACE, Linkage)


#endif
