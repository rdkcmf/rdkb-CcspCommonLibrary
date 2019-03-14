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

    module:	tls_seo_interface.h

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the TLS Session Entry Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/16/03    initial revision.

**********************************************************************/


#ifndef  _TLS_SEO_INTERFACE_
#define  _TLS_SEO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "tls_properties.h"


/***********************************************************
    PLATFORM INDEPENDENT SESSION ENTRY OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_TLSSEO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_TLSSEO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_TLSSEO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  BOOL
(*PFN_TLSSEO_MATCH1)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       host_id,
        ULONG                       peer_id,
        ULONG                       conn_end
    );

typedef  BOOL
(*PFN_TLSSEO_MATCH2)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       host_id,
        ULONG                       peer_id,
        PUCHAR                      session_id,
        ULONG                       session_id_size
    );

/*
 * The cryptographic parameters of the session state are produced by the TLS Handshake Protocol,
 * which operates on top of the TLS Record Layer. When a TLS client and server first start
 * communicating, they agree on a protocol version, select cryptographic algorithms, optionally
 * authenticate each other, and use public-key encryption techniques to greate shared secrets. The
 * Session Entry Object is responsible for managing all successfully negotiated session states.
 */
#define  TLS_SESSION_ENTRY_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    TLS_SESSION_STATE               SessionState;                                           \
    ULONG                           Timestamp;                                              \
                                                                                            \
    PFN_TLSSEO_GET_PROPERTY         GetSessionState;                                        \
    PFN_TLSSEO_SET_PROPERTY         SetSessionState;                                        \
    PFN_TLSSEO_RESET                Reset;                                                  \
                                                                                            \
    PFN_TLSSEO_MATCH1               Match1;                                                 \
    PFN_TLSSEO_MATCH2               Match2;                                                 \
    /* end of object class content */                                                       \

typedef  struct
_TLS_SESSION_ENTRY_OBJECT
{
    TLS_SESSION_ENTRY_CLASS_CONTENT
}
TLS_SESSION_ENTRY_OBJECT,  *PTLS_SESSION_ENTRY_OBJECT;

#define  ACCESS_TLS_SESSION_ENTRY_OBJECT(p)         \
         ACCESS_CONTAINER(p, TLS_SESSION_ENTRY_OBJECT, Linkage)


#endif
