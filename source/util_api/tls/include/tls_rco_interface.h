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

    module:	tls_rco_interface.h

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the TLS Record Client Object.

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


#ifndef  _TLS_RCO_INTERFACE_
#define  _TLS_RCO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "tls_properties.h"


/***********************************************************
    PLATFORM INDEPENDENT RECORD CLIENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_TLSRCO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TLSRCO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TLSRCO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  BOOL
(*PFN_TLSRCO_ACCEPT)
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_TLSRCO_RECV)
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hReserved
    );

typedef  ANSC_STATUS
(*PFN_TLSRCO_SEND)
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    );

/*
 * Four record protocol clients are specified in RFC 2246: the handshake protocol, the alert
 * protocol, the change cipher spec protocol, and the application data protocol. In order to allow
 * extension of the TLS protocol, additional record types can be supported by the record protocol.
 * The TLS_CBC interface centralizes the connection control, including exception handling, session
 * management, and coordination.
 */
#define  TLS_RECORD_CLIENT_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    UCHAR                           RecordType;                                             \
    ANSC_HANDLE                     hTlsRecordKeeper;                                       \
    ANSC_HANDLE                     hTlsCbcIf;                                              \
    ANSC_HANDLE                     hTlsMecIf;                                              \
    ANSC_HANDLE                     hTlsTsaIf;                                              \
    BOOL                            bActive;                                                \
                                                                                            \
    PFN_TLSRCO_RESET                Reset;                                                  \
    PFN_TLSRCO_ENGAGE               Engage;                                                 \
    PFN_TLSRCO_CANCEL               Cancel;                                                 \
                                                                                            \
    PFN_TLSRCO_ACCEPT               Accept;                                                 \
    PFN_TLSRCO_RECV                 Recv;                                                   \
    PFN_TLSRCO_SEND                 Send;                                                   \
    /* end of object class content */                                                       \

typedef  struct
_TLS_RECORD_CLIENT_OBJECT
{
    TLS_RECORD_CLIENT_CLASS_CONTENT
}
TLS_RECORD_CLIENT_OBJECT,  *PTLS_RECORD_CLIENT_OBJECT;

#define  ACCESS_TLS_RECORD_CLIENT_OBJECT(p)         \
         ACCESS_CONTAINER(p, TLS_RECORD_CLIENT_OBJECT, Linkage)


#endif
