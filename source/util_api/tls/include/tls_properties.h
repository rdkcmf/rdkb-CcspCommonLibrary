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

    module:	tls_properties.h

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This file defines the configuration parameters that can be
        applied to the TLS Systems.

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


#ifndef  _TLS_PROPERTIES_
#define  _TLS_PROPERTIES_


/*
 * We have to use some of the constant values defined in the TLS message definitions, so we
 * include the header file here.
 */
#include  "tls_definitions.h"


/***********************************************************
   TLS ENV CONTROLLER CONFIGURATION PARAMETERS DEFINITION
***********************************************************/

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
typedef  struct
_TLS_ENV_CONTROLLER_PROPERTY
{
    ULONG                           Dummy;
}
TLS_ENV_CONTROLLER_PROPERTY,  *PTLS_ENV_CONTROLLER_PROPERTY;


/***********************************************************
   TLS SESSION MANAGER CONFIGURATION PARAMETERS DEFINITION
***********************************************************/

/*
 * The cryptographic parameters of the session state are produced by the TLS Handshake Protocol,
 * which operates on top of the TLS Record Layer. When a TLS client and server first start
 * communicating, they agree on a protocol version, select cryptographic algorithms, optionally
 * authenticate each other, and use public-key encryption techniques to greate shared secrets. The
 * Session Manager Object is responsible for managing all successfully negotiated session states.
 */
typedef  struct
_TLS_SESSION_MANAGER_PROPERTY
{
    ULONG                           Dummy;
}
TLS_SESSION_MANAGER_PROPERTY,  *PTLS_SESSION_MANAGER_PROPERTY;


/***********************************************************
   TLS CONN CONTROLLER CONFIGURATION PARAMETERS DEFINITION
***********************************************************/

/*
 * A TLS connection state is the operating environment of the TLS record protocol. It specifies a
 * compression algorithm, encryption algorithm, and MAC algorithm. In addition, the parameters for
 * these algorithms are known: the MAC secret and the bulk encryption keys and IVs for the
 * connection in both the read and the write directions.
 */
typedef  struct
_TLS_CONN_CONTROLLER_PROPERTY
{
    ULONG                           Dummy;
}
TLS_CONN_CONTROLLER_PROPERTY,  *PTLS_CONN_CONTROLLER_PROPERTY;


/***********************************************************
    TLS RECORD KEEPER CONFIGURATION PARAMETERS DEFINITION
***********************************************************/

/*
 * The TLS Record Protocol is a layered protocol. At each layer, messages may include fields for
 * length, description, and content. The Record Protocol takes messages to e transmitted, fragments
 * the data into manageable blocks, optionally compresses the data, applies a MAC, encrypts, and
 * transmits the result. Received data is decrypted, verified, decompressed, and reassembled, then
 * delivered to higher level clients.
 */
typedef  struct
_TLS_RECORD_KEEPER_PROPERTY
{
    ULONG                           Dummy;
}
TLS_RECORD_KEEPER_PROPERTY,  *PTLS_RECORD_KEEPER_PROPERTY;


#endif
