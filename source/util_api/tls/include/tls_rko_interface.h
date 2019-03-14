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

    module:	tls_rko_interface.h

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the TLS Record Keeper Object.

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


#ifndef  _TLS_RKO_INTERFACE_
#define  _TLS_RKO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "tls_properties.h"


/***********************************************************
    PLATFORM INDEPENDENT RECORD KEEPER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  TLS_RKO_RCO_TABLE_SIZE                     4

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_TLSRKO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_TLSRKO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_TLSRKO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TLSRKO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TLSRKO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TLSRKO_CHANGE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRecordState
    );

typedef  ANSC_HANDLE
(*PFN_TLSRKO_GET_RCO)
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       record_type
    );

typedef  ANSC_STATUS
(*PFN_TLSRKO_ADD_RCO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRecordClient
    );

typedef  ANSC_STATUS
(*PFN_TLSRKO_POP_RCO)
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       record_type
    );

typedef  ANSC_STATUS
(*PFN_TLSRKO_DEL_RCO)
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       record_type
    );

typedef  ANSC_STATUS
(*PFN_TLSRKO_DEL_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TLSRKO_RECV)
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hReserved
    );

typedef  ANSC_STATUS
(*PFN_TLSRKO_SEND)
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       record_type,
        void*                       buffer,
        ULONG                       ulSize
    );

/*
 * The TLS Record Protocol is a layered protocol. At each layer, messages may include fields for
 * length, description, and content. The Record Protocol takes messages to e transmitted, fragments
 * the data into manageable blocks, optionally compresses the data, applies a MAC, encrypts, and
 * transmits the result. Received data is decrypted, verified, decompressed, and reassembled, then
 * delivered to higher level clients.
 */
#define  TLS_RECORD_KEEPER_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    TLS_RECORD_KEEPER_PROPERTY      Property;                                               \
    TLS_RECORD_STATE                RecordStateW;                                           \
    TLS_RECORD_STATE                RecordStateR;                                           \
    ANSC_HANDLE                     hTlsCbcIf;                                              \
    ANSC_HANDLE                     hTlsMecIf;                                              \
    ANSC_HANDLE                     hTlsTsaIf;                                              \
    BOOL                            bActive;                                                \
                                                                                            \
    QUEUE_HEADER                    RcoTable[TLS_RKO_RCO_TABLE_SIZE];                       \
    ANSC_LOCK                       RcoTableLock;                                           \
                                                                                            \
    PFN_TLSRKO_GET_PROPERTY         GetProperty;                                            \
    PFN_TLSRKO_SET_PROPERTY         SetProperty;                                            \
    PFN_TLSRKO_RESET                ResetProperty;                                          \
    PFN_TLSRKO_RESET                Reset;                                                  \
                                                                                            \
    PFN_TLSRKO_ENGAGE               Engage;                                                 \
    PFN_TLSRKO_CANCEL               Cancel;                                                 \
    PFN_TLSRKO_CHANGE               ChangeStateW;                                           \
    PFN_TLSRKO_CHANGE               ChangeStateR;                                           \
                                                                                            \
    PFN_TLSRKO_GET_RCO              GetRecordClient;                                        \
    PFN_TLSRKO_ADD_RCO              AddRecordClient;                                        \
    PFN_TLSRKO_POP_RCO              PopRecordClient;                                        \
    PFN_TLSRKO_DEL_RCO              DelRecordClient;                                        \
    PFN_TLSRKO_DEL_ALL              DelAllRecordClients;                                    \
                                                                                            \
    PFN_TLSRKO_RECV                 Recv;                                                   \
    PFN_TLSRKO_SEND                 Send;                                                   \
    /* end of object class content */                                                       \

typedef  struct
_TLS_RECORD_KEEPER_OBJECT
{
    TLS_RECORD_KEEPER_CLASS_CONTENT
}
TLS_RECORD_KEEPER_OBJECT,  *PTLS_RECORD_KEEPER_OBJECT;

#define  ACCESS_TLS_RECORD_KEEPER_OBJECT(p)         \
         ACCESS_CONTAINER(p, TLS_RECORD_KEEPER_OBJECT, Linkage)


#endif
