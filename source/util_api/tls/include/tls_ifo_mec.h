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

    module:	tls_ifo_mec.h

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the TLS Module Element Controller Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/18/03    initial revision.

**********************************************************************/


#ifndef  _TLS_IFO_MEC_
#define  _TLS_IFO_MEC_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/***********************************************************
  TLS MODULE ELEMENT CONTROLLER INTERFACE OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  TLS_MEC_INTERFACE_NAME                     "tlsModuleElementController"
#define  TLS_MEC_INTERFACE_ID                       0

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_TLSMECIF_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_TLSMECIF_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_TLSMECIF_GET_PARAMS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_TLSMECIF_GET_CERT1)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       cert_bits,
        PUCHAR                      cert_id
    );

typedef  ANSC_HANDLE
(*PFN_TLSMECIF_GET_CERT2)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      cert_id
    );

typedef  ANSC_HANDLE
(*PFN_TLSMECIF_GET_CERT3)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       client_cert_type,
        ANSC_HANDLE                 authority_name,
        PUCHAR                      cert_id
    );

typedef  ANSC_HANDLE
(*PFN_TLSMECIF_GET_PROFILE)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      cert_id
    );

typedef  ANSC_HANDLE
(*PFN_TLSMECIF_GET_CERT4)
    (
        ANSC_HANDLE                 hThisObject,
        void*                       cert_data,
        ULONG                       cert_size
    );

typedef  ANSC_HANDLE
(*PFN_TLSMECIF_GET_CERT5)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 issuer_name,
        ANSC_HANDLE                 issuer_key
    );

typedef  ANSC_HANDLE
(*PFN_TLSMECIF_GET_CERT6)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 issuer_name,
        ANSC_HANDLE                 issuer_key_id,
        ANSC_HANDLE                 issuer_serial
    );

/*
 * As we have discovered recently in module development, it's useful to implement a backbone
 * environment access interface. This interface makes the resource and services available to all
 * the module elements from a central place. This mechanism greatly reduces the overhead for
 * populating the availability of certain internal/external resource within the module from top
 * down. The TLS_MEC interface serves as such a backbone access mechanism.
 */
#define  TLS_MEC_INTERFACE_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_TLSMECIF_GET_CONTEXT        GetTlsCryptoProvider;                                   \
    PFN_TLSMECIF_GET_CONTEXT        GetTlsSessionManager;                                   \
    PFN_TLSMECIF_GET_IF             GetTlsHsmIf;                                            \
    PFN_TLSMECIF_GET_PARAMS         GetModuleParams;                                        \
    PFN_TLSMECIF_GET_CERT1          GetCertChain1;                                          \
    PFN_TLSMECIF_GET_CERT2          GetCertChain2;                                          \
    PFN_TLSMECIF_GET_CERT3          GetCertChain3;                                          \
    PFN_TLSMECIF_GET_PROFILE        GetProfile;                                             \
    PFN_TLSMECIF_GET_CERT4          GetCACertByCert;                                        \
    PFN_TLSMECIF_GET_CERT5          GetCACertByKey;                                         \
    PFN_TLSMECIF_GET_CERT6          GetCACertByIssuer;                                      \
    /* end of object class content */                                                       \

typedef  struct
_TLS_MEC_INTERFACE
{
    TLS_MEC_INTERFACE_CLASS_CONTENT
}
TLS_MEC_INTERFACE,  *PTLS_MEC_INTERFACE;

#define  ACCESS_TLS_MEC_INTERFACE(p)                \
         ACCESS_CONTAINER(p, TLS_MEC_INTERFACE, Linkage)


#endif
