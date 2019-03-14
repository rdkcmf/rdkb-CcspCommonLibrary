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

    module:	tls_smo_interface.h

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the TLS Session Manager Object.

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


#ifndef  _TLS_SMO_INTERFACE_
#define  _TLS_SMO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "tls_properties.h"

#include "tls_ifo_hsm.h"


/***********************************************************
   PLATFORM INDEPENDENT SESSION MANAGER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  TLS_SMO_PATROL_TIMER_INTERVAL              5 * 60 * 1000   /* in milliseconds */
#define  TLS_SMO_SESSION_TIMEOUT                    3600            /* in seconds      */
#define  TLS_SMO_SEO_TABLE_SIZE                     8

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_TLSSMO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TLSSMO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_TLSSMO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TLSSMO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ULONG
(*PFN_TLSSMO_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TLSSMO_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    );

typedef  ANSC_STATUS
(*PFN_TLSSMO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_TLSSMO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_TLSSMO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TLSSMO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TLSSMO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TLSSMO_INVOKE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_TLSSMO_GET_SEO)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       host_id,
        ULONG                       peer_id,
        PUCHAR                      session_id,
        ULONG                       session_id_size
    );

typedef  ANSC_STATUS
(*PFN_TLSSMO_ADD_SEO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession
    );

typedef  ANSC_STATUS
(*PFN_TLSSMO_DEL_SEO)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       host_id,
        ULONG                       peer_id,
        PUCHAR                      session_id,
        ULONG                       session_id_size
    );

typedef  ANSC_STATUS
(*PFN_TLSSMO_DEL_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * The cryptographic parameters of the session state are produced by the TLS Handshake Protocol,
 * which operates on top of the TLS Record Layer. When a TLS client and server first start
 * communicating, they agree on a protocol version, select cryptographic algorithms, optionally
 * authenticate each other, and use public-key encryption techniques to greate shared secrets. The
 * Session Manager Object is responsible for managing all successfully negotiated session states.
 */
#define  TLS_SESSION_MANAGER_CLASS_CONTENT                                                  \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    TLS_SESSION_MANAGER_PROPERTY    Property;                                               \
    ANSC_HANDLE                     hPatrolTimer;                                           \
    ANSC_HANDLE                     hTimerClient;                                           \
    ANSC_HANDLE                     hTlsHsmIf;                                              \
    ANSC_HANDLE                     hTlsMecIf;                                              \
    BOOL                            bActive;                                                \
                                                                                            \
    QUEUE_HEADER                    SeoTable[TLS_SMO_SEO_TABLE_SIZE];                       \
    ANSC_LOCK                       SeoTableLock;                                           \
                                                                                            \
    PFN_TLSSMO_GET_PROPERTY         GetProperty;                                            \
    PFN_TLSSMO_SET_PROPERTY         SetProperty;                                            \
    PFN_TLSSMO_RESET                ResetProperty;                                          \
    PFN_TLSSMO_RESET                Reset;                                                  \
                                                                                            \
    PFN_TLSSMO_ENGAGE               Engage;                                                 \
    PFN_TLSSMO_CANCEL               Cancel;                                                 \
    PFN_TLSSMO_INVOKE               PatrolTimerInvoke;                                      \
                                                                                            \
    PFN_TLSSMO_GET_SEO              GetSession;                                             \
    PFN_TLSSMO_ADD_SEO              AddSession;                                             \
    PFN_TLSSMO_DEL_SEO              DelSession;                                             \
    PFN_TLSSMO_DEL_ALL              DelAllSessions;                                         \
                                                                                            \
    /* end of object class content */                                                       \

typedef  struct
_TLS_SESSION_MANAGER_OBJECT
{
    TLS_SESSION_MANAGER_CLASS_CONTENT
}
TLS_SESSION_MANAGER_OBJECT,  *PTLS_SESSION_MANAGER_OBJECT;

#define  ACCESS_TLS_SESSION_MANAGER_OBJECT(p)       \
         ACCESS_CONTAINER(p, TLS_SESSION_MANAGER_OBJECT, Linkage)


#endif
