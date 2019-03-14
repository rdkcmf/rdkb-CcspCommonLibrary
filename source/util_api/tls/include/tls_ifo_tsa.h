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

    module:	tls_ifo_tsa.h

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the TLS Transport Service Adaptor Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/17/03    initial revision.

**********************************************************************/


#ifndef  _TLS_IFO_TSA_
#define  _TLS_IFO_TSA_


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
#define  TLS_TSA_INTERFACE_NAME                     "tlsTransportServiceAdaptor"
#define  TLS_TSA_INTERFACE_ID                       0

#define  TLS_TSA_EVENT_CONNECTION_ESTABLISHED       1
#define  TLS_TSA_EVENT_CONNECTION_CLOSED            2
#define  TLS_TSA_EVENT_CONNECTION_TERMINATED        3
#define  TLS_TSA_EVENT_CONNECTION_TIMEOUT           4

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_TLSTSAIF_RECV)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessageBdo
    );

typedef  ANSC_STATUS
(*PFN_TLSTSAIF_SEND)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessageBdo
    );

typedef  ANSC_STATUS
(*PFN_TLSTSAIF_NOTIFY)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulEvent,
        ULONG                       ulError,
        ANSC_HANDLE                 hEventContext
    );

/*
 * The primary goal of the TLS Protocol is to provide privacy and data integrity between two
 * communicating applications. The protocol is composed of two layers: the TLS Record Protocol and
 * the TLS Handshake Protocol. At the lowest level, layered on top of some reliable transport
 * protocol (e.g., TCP[TCP]), is the TLS Record Protocol. The TLS Record Protocol provides
 * connection security that has two basic properties:
 *
 *      - The connection is private. Symmetric cryptography is used for data encryption
 *        (e.g., DES [DES], RC4 [RC4], etc.) The keys for this symmetric encryption are
 *        generated uniquely for each connection and are based on a secret negotiated by
 *        another protocol (such as the TLS Handshake Protocol). The Record Protocol can
 *        also be used without encryption.
 *
 *      - The connection is reliable. Message transport includes a message integrity
 *        check using a keyed MAC. Secure hash functions (e.g., SHA, MD5, etc.) are used
 *        for MAC computations. The Record Protocol can operate without a MAC, but is
 *        generally only used in this mode while another protocol is using the Record
 *        Protocol as a transport for negotiating security parameters.
 */
#define  TLS_TSA_INTERFACE_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_TLSTSAIF_RECV               RecvAppMessage;                                         \
    PFN_TLSTSAIF_SEND               SendTlsMessage;                                         \
    PFN_TLSTSAIF_NOTIFY             NotifyEvent;                                            \
    /* end of object class content */                                                       \

typedef  struct
_TLS_TSA_INTERFACE
{
    TLS_TSA_INTERFACE_CLASS_CONTENT
}
TLS_TSA_INTERFACE,  *PTLS_TSA_INTERFACE;

#define  ACCESS_TLS_TSA_INTERFACE(p)                \
         ACCESS_CONTAINER(p, TLS_TSA_INTERFACE, Linkage)


#endif
