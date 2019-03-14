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

    module:	tls_eco_interface.h

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the TLS Env Controller Object.

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


#ifndef  _TLS_ECO_INTERFACE_
#define  _TLS_ECO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "tls_properties.h"

#include "tls_ifo_scs.h"
#include "tls_ifo_mec.h"


/***********************************************************
    PLATFORM INDEPENDENT ENV CONTROLLER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_TLSECO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TLSECO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_TLSECO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ULONG
(*PFN_TLSECO_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TLSECO_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    );

typedef  ANSC_STATUS
(*PFN_TLSECO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_TLSECO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_TLSECO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TLSECO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TLSECO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TLSECO_TRANSLATE)
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcMessage,
        ULONG                       ulSrcMsgSize,
        void*                       pDstMessage,
        PULONG                      pulDstMsgSize
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
#define  TLS_ENV_CONTROLLER_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    TLS_ENV_CONTROLLER_PROPERTY     Property;                                               \
    TLS_MODULE_PARAMS               ModuleParams;                                           \
    ANSC_HANDLE                     hTlsCryptoProvider;                                     \
    ANSC_HANDLE                     hTlsSessionManager;                                     \
    ANSC_HANDLE                     hTlsScsIf;                                              \
    ANSC_HANDLE                     hTlsMecIf;                                              \
    BOOL                            bActive;                                                \
                                                                                            \
    PFN_TLSECO_GET_PROPERTY         GetProperty;                                            \
    PFN_TLSECO_SET_PROPERTY         SetProperty;                                            \
    PFN_TLSECO_RESET                ResetProperty;                                          \
    PFN_TLSECO_RESET                Reset;                                                  \
                                                                                            \
    PFN_TLSECO_ENGAGE               Engage;                                                 \
    PFN_TLSECO_CANCEL               Cancel;                                                 \
                                                                                            \
    PFN_TLSECO_TRANSLATE            Ssl2ToSsl3ClientHello;                                  \
    /* end of object class content */                                                       \

typedef  struct
_TLS_ENV_CONTROLLER_OBJECT
{
    TLS_ENV_CONTROLLER_CLASS_CONTENT
}
TLS_ENV_CONTROLLER_OBJECT,  *PTLS_ENV_CONTROLLER_OBJECT;

#define  ACCESS_TLS_ENV_CONTROLLER_OBJECT(p)        \
         ACCESS_CONTAINER(p, TLS_ENV_CONTROLLER_OBJECT, Linkage)


#endif
