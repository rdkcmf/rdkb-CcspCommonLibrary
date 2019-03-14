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

    module:	tls_aho_interface.h

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the TLS Alert Handler Object.

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


#ifndef  _TLS_AHO_INTERFACE_
#define  _TLS_AHO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "tls_rco_interface.h"
#include "tls_rco_exported_api.h"
#include "tls_properties.h"


/***********************************************************
    PLATFORM INDEPENDENT ALERT HANDLER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_TLSAHO_RECV)
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_TLSAHO_SEND)
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       alert_level,
        UCHAR                       alert_descr
    );

/*
 * One of the content types supported by the TLS Record layer is the alert type. Alert messages
 * convey the severity of the message and a description of the alert. Alert messages with a level
 * of fatal result in the immediate termination of the connection. In this case, other connections
 * corresponding to the session may continue, but the session identifier must be invalidated,
 * preventing the failed session from being used to establish new connections. Like other messages,
 * alert messages are encrypted and compressed, as specified by the current connection state.
 */
#define  TLS_ALERT_HANDLER_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    TLS_RECORD_CLIENT_CLASS_CONTENT                                                         \
    /* start of object class content */                                                     \
    PFN_TLSAHO_RECV                 RecvAlert;                                              \
    PFN_TLSAHO_SEND                 SendAlert;                                              \
    /* end of object class content */                                                       \

typedef  struct
_TLS_ALERT_HANDLER_OBJECT
{
    TLS_ALERT_HANDLER_CLASS_CONTENT
}
TLS_ALERT_HANDLER_OBJECT,  *PTLS_ALERT_HANDLER_OBJECT;

#define  ACCESS_TLS_ALERT_HANDLER_OBJECT(p)         \
         ACCESS_CONTAINER(p, TLS_ALERT_HANDLER_OBJECT, Linkage)


#endif
