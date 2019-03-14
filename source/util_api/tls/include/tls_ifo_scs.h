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

    module:	tls_ifo_scs.h

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the TLS Secure Communication Service Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/16/03    initial revision.

**********************************************************************/


#ifndef  _TLS_IFO_SCS_
#define  _TLS_IFO_SCS_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/***********************************************************
TLS SECURE COMMUNICATION SERVICE INTERFACE OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  TLS_SCS_INTERFACE_NAME                     "tlsSecureCommunicationService"
#define  TLS_SCS_INTERFACE_ID                       0

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_TLSSCSIF_CONFIG)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTlsModuleParams
    );

typedef  ANSC_HANDLE
(*PFN_TLSSCSIF_CREATE_CONN)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTlsTsaIf
    );

typedef  ANSC_STATUS
(*PFN_TLSSCSIF_CONFIG_CONN)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTlsConnection,
        ANSC_HANDLE                 hTlsConnParams
    );

typedef  ANSC_STATUS
(*PFN_TLSSCSIF_REMOVE_CONN)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTlsConnection
    );

typedef  ANSC_STATUS
(*PFN_TLSSCSIF_START_CONN)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTlsConnection
    );

typedef  ANSC_STATUS
(*PFN_TLSSCSIF_CLOSE_CONN)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTlsConnection
    );

typedef  BOOL
(*PFN_TLSSCSIF_QUERY_CONN)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTlsConnection
    );

typedef  ANSC_STATUS
(*PFN_TLSSCSIF_RECV)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTlsConnection,
        ANSC_HANDLE                 hMessageBdo
    );

typedef  ANSC_STATUS
(*PFN_TLSSCSIF_SEND)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTlsConnection,
        ANSC_HANDLE                 hMessageBdo
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
#define  TLS_SCS_INTERFACE_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_TLSSCSIF_CONFIG             ConfigModule;                                           \
    PFN_TLSSCSIF_CREATE_CONN        CreateConnection;                                       \
    PFN_TLSSCSIF_CONFIG_CONN        ConfigConnection;                                       \
    PFN_TLSSCSIF_REMOVE_CONN        RemoveConnection;                                       \
                                                                                            \
    PFN_TLSSCSIF_START_CONN         StartConnection;                                        \
    PFN_TLSSCSIF_CLOSE_CONN         CloseConnection;                                        \
    PFN_TLSSCSIF_QUERY_CONN         QueryConnection;                                        \
                                                                                            \
    PFN_TLSSCSIF_RECV               RecvTlsMessage;                                         \
    PFN_TLSSCSIF_SEND               SendAppMessage;                                         \
    /* end of object class content */                                                       \

typedef  struct
_TLS_SCS_INTERFACE
{
    TLS_SCS_INTERFACE_CLASS_CONTENT
}
TLS_SCS_INTERFACE,  *PTLS_SCS_INTERFACE;

#define  ACCESS_TLS_SCS_INTERFACE(p)                \
         ACCESS_CONTAINER(p, TLS_SCS_INTERFACE, Linkage)


#endif
