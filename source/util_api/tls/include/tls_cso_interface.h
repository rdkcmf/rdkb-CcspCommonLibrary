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

    module:	tls_cso_interface.h

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the TLS Cipher Switch Object.

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


#ifndef  _TLS_CSO_INTERFACE_
#define  _TLS_CSO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "tls_rco_interface.h"
#include "tls_rco_exported_api.h"
#include "tls_properties.h"


/***********************************************************
    PLATFORM INDEPENDENT CIPHER SWITCH OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_TLSCSO_RECV)
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_TLSCSO_SEND)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * The change cipher spec protocol exists to signal transitions in ciphering strategies. The
 * protocol consists of a single message, which is encrypted and compressed under the current (not
 * the pending) connection state. The message consists of a single byte of value 1.
 *
 *      struct {
 *          enum { change_cipher_spec(1), (255) } type;
 *      } ChangeCipherSpec;
 *
 * The change cipher spec message is sent by both the client and server to notify the receiving
 * party that subsequent records will be protected under the newly negotiated CipherSpec and keys.
 * Reception of this message causes the receiver to instruct the Record Layer to immediately copy
 * the read pending state into the read current state. Immediately after sending this message, the
 * sender should instruct the record layer to make the write pending state the write active state.
 * The change cipher spec message is sent during the handshake after the security parameters have
 * been agreed upon, but before the verifying finished message is sent.
 */
#define  TLS_CIPHER_SWITCH_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    TLS_RECORD_CLIENT_CLASS_CONTENT                                                         \
    /* start of object class content */                                                     \
    PFN_TLSCSO_RECV                 RecvChangeCipherSpec;                                   \
    PFN_TLSCSO_SEND                 SendChangeCipherSpec;                                   \
    /* end of object class content */                                                       \

typedef  struct
_TLS_CIPHER_SWITCH_OBJECT
{
    TLS_CIPHER_SWITCH_CLASS_CONTENT
}
TLS_CIPHER_SWITCH_OBJECT,  *PTLS_CIPHER_SWITCH_OBJECT;

#define  ACCESS_TLS_CIPHER_SWITCH_OBJECT(p)         \
         ACCESS_CONTAINER(p, TLS_CIPHER_SWITCH_OBJECT, Linkage)


#endif
