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

    module:	tls_hsoclient_interface.h

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the TLS Hso Client Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/26/03    initial revision.

**********************************************************************/


#ifndef  _TLS_HSOCLIENT_INTERFACE_
#define  _TLS_HSOCLIENT_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "tls_hso_interface.h"
#include "tls_hso_exported_api.h"
#include "tls_properties.h"


/***********************************************************
      PLATFORM INDEPENDENT HSO CLIENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  TLS_HSC_STATE_idle                         0
#define  TLS_HSC_STATE_sentClientHello              1
#define  TLS_HSC_STATE_waitServerHello              2
#define  TLS_HSC_STATE_waitCertificate              3
#define  TLS_HSC_STATE_waitServerKeyExchange        4
#define  TLS_HSC_STATE_waitCertRequest              5
#define  TLS_HSC_STATE_waitServerHelloDone          6
#define  TLS_HSC_STATE_sentCertificate              7
#define  TLS_HSC_STATE_sentClientKeyExchange        8
#define  TLS_HSC_STATE_sentCertVerify               9
#define  TLS_HSC_STATE_sentFinished                 10
#define  TLS_HSC_STATE_waitFinished                 11

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_TLSHSOCLIENT_RECV)
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_TLSHSOCLIENT_SEND)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * The TLS Handshake Protocol consists of a suite of three sub-protocols which are used to allow
 * peers to agree upon security parameters for the record layer, authenticate themselves, instan-
 * tiate negotiated security parameters, and report error conditions to each other. The Handshake
 * Protocol is responsible for negotiating a session, which consists of the following items:
 *
 *      session identifier
 *          An arbitrary byte sequence chosen by the server to identify an active or
 *          resumable session state.
 *      peer certificate
 *          X509v3 certificate of the peer. This element of the state may be null.
 *      compression method
 *          The algorithm used to compress data prior to encryption.
 *      cipher spec
 *          Specifies the bulk data encryption algorithm (such as null, DES, etc.) and
 *          a MAC algorithm (such as MD5 or SHA). It also defines cryptographic
 *          attributes such as the hash_size.
 *      master secret
 *          48-byte secret shared between the client and server.
 *      is resumable
 *          A flag indicating whether the session can be used to initiate new connections.
 *
 * These items are then used to create security parameters for use by the Record Layer when
 * protecting application data. Many connections can be instantiated using the same session through
 * the resumption feature of the TLS Handshake Protocol.
 */
#define  TLS_HSO_CLIENT_CLASS_CONTENT                                                       \
    /* duplication of the base object class content */                                      \
    TLS_HAND_SHAKER_CLASS_CONTENT                                                           \
    /* start of object class content */                                                     \
    ULONG                           HsClientState;                                          \
                                                                                            \
    PFN_TLSHSOCLIENT_RECV           RecvHelloRequest;                                       \
    PFN_TLSHSOCLIENT_RECV           RecvServerHello;                                        \
    PFN_TLSHSOCLIENT_RECV           RecvCertificate;                                        \
    PFN_TLSHSOCLIENT_RECV           RecvServerKeyExchange;                                  \
    PFN_TLSHSOCLIENT_RECV           RecvCertRequest;                                        \
    PFN_TLSHSOCLIENT_RECV           RecvServerHelloDone;                                    \
    PFN_TLSHSOCLIENT_RECV           RecvFinished;                                           \
                                                                                            \
    PFN_TLSHSOCLIENT_SEND           SendClientHello;                                        \
    PFN_TLSHSOCLIENT_SEND           SendCertificate;                                        \
    PFN_TLSHSOCLIENT_SEND           SendClientKeyExchange;                                  \
    PFN_TLSHSOCLIENT_SEND           SendCertVerify;                                         \
    PFN_TLSHSOCLIENT_SEND           SendFinished;                                           \
    /* end of object class content */                                                       \

typedef  struct
_TLS_HSO_CLIENT_OBJECT
{
    TLS_HSO_CLIENT_CLASS_CONTENT
}
TLS_HSO_CLIENT_OBJECT,  *PTLS_HSO_CLIENT_OBJECT;

#define  ACCESS_TLS_HSO_CLIENT_OBJECT(p)            \
         ACCESS_CONTAINER(p, TLS_HSO_CLIENT_OBJECT, Linkage)


#endif
