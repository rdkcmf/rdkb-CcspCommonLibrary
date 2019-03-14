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

    module:	tls_cco_interface.h

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the TLS Conn Controller Object.

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


#ifndef  _TLS_CCO_INTERFACE_
#define  _TLS_CCO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "tls_properties.h"

#include "tls_ifo_cbc.h"


/***********************************************************
   PLATFORM INDEPENDENT CONN CONTROLLER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_TLSCCO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TLSCCO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_TLSCCO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TLSCCO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ULONG
(*PFN_TLSCCO_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TLSCCO_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    );

typedef  ANSC_STATUS
(*PFN_TLSCCO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_TLSCCO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_TLSCCO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TLSCCO_ACQ_ACCESS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TLSCCO_REL_ACCESS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TLSCCO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TLSCCO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TLSCCO_CONFIG)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTlsConnParams
    );

typedef  ANSC_STATUS
(*PFN_TLSCCO_START)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TLSCCO_CLOSE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TLSCCO_RECV)
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hReserved
    );

typedef  ANSC_STATUS
(*PFN_TLSCCO_SEND)
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    );

/*
 * A TLS connection state is the operating environment of the TLS record protocol. It specifies a
 * compression algorithm, encryption algorithm, and MAC algorithm. In addition, the parameters for
 * these algorithms are known: the MAC secret and the bulk encryption keys and IVs for the
 * connection in both the read and the write directions.
 */
#define  TLS_CONN_CONTROLLER_CLASS_CONTENT                                                  \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    TLS_CONN_CONTROLLER_PROPERTY    Property;                                               \
    TLS_CONNECTION_PARAMS           ConnectionParams;                                       \
    ANSC_HANDLE                     hTbpPdo;                                                \
    ANSC_HANDLE                     hTlsRecordKeeper;                                       \
    ANSC_HANDLE                     hTlsHandShaker;                                         \
    ANSC_HANDLE                     hTlsAlertHandler;                                       \
    ANSC_HANDLE                     hTlsCipherSwitch;                                       \
    ANSC_HANDLE                     hTlsAppConnector;                                       \
    ANSC_HANDLE                     hTlsCbcIf;                                              \
    ANSC_HANDLE                     hTlsMecIf;                                              \
    ANSC_HANDLE                     hTlsTsaIf;                                              \
    BOOL                            bActive;                                                \
    BOOL                            bBroken;                                                \
    ANSC_LOCK                       AccessLock;                                             \
                                                                                            \
    PFN_TLSCCO_GET_PROPERTY         GetProperty;                                            \
    PFN_TLSCCO_SET_PROPERTY         SetProperty;                                            \
    PFN_TLSCCO_RESET                ResetProperty;                                          \
                                                                                            \
    PFN_TLSCCO_ACQ_ACCESS           AcqAccess;                                              \
    PFN_TLSCCO_REL_ACCESS           RelAccess;                                              \
    PFN_TLSCCO_ENGAGE               Engage;                                                 \
    PFN_TLSCCO_CANCEL               Cancel;                                                 \
    PFN_TLSCCO_CONFIG               Config;                                                 \
                                                                                            \
    PFN_TLSCCO_START                StartHandshake;                                         \
    PFN_TLSCCO_CLOSE                CloseSession;                                           \
                                                                                            \
    PFN_TLSCCO_RECV                 RecvTlsMessage;                                         \
    PFN_TLSCCO_SEND                 SendAppMessage;                                         \
    /* end of object class content */                                                       \

typedef  struct
_TLS_CONN_CONTROLLER_OBJECT
{
    TLS_CONN_CONTROLLER_CLASS_CONTENT
}
TLS_CONN_CONTROLLER_OBJECT,  *PTLS_CONN_CONTROLLER_OBJECT;

#define  ACCESS_TLS_CONN_CONTROLLER_OBJECT(p)       \
         ACCESS_CONTAINER(p, TLS_CONN_CONTROLLER_OBJECT, Linkage)


#endif
