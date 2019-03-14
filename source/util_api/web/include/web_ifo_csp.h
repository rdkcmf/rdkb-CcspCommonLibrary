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

    module:	web_ifo_csp.h

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Configuration Service Provider Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/15/02    initial revision.

**********************************************************************/


#ifndef  _WEB_IFO_CSP_
#define  _WEB_IFO_CSP_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_csp.h"


/***********************************************************
   WEB CONFIGURATION SERVICE PROVIDER INTERFACE DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  WEB_CSP_INTERFACE_NAME                     "webConfigurationServiceProviderIf"
#define  WEB_CSP_INTERFACE_ID                       0

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_WEBCSPIF_ADD_DAEMON)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDaemonInfo
    );

typedef  ANSC_STATUS
(*PFN_WEBCSPIF_DEL_DAEMON)
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usDaemonPort
    );

typedef  ANSC_STATUS
(*PFN_WEBCSPIF_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    );

typedef  ANSC_STATUS
(*PFN_WEBCSPIF_SET_BOOL)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnabled
    );

typedef  ANSC_STATUS
(*PFN_WEBCSPIF_SET_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pProductName
    );

/*
 * As a general requirement, each module SHOULD provide an interface for external components to
 * configure its parameters and policies. Although the benefit of unifying the definition and usage
 * of such an interface is obvious, we DON'T want to impose any set of rules on the implementation.
 * Instead, we expect every module will implement its configuration interfaces independently.
 */
#define  WEB_CSP_INTERFACE_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    ANSC_CSP_INTERFACE_CLASS_CONTENT                                                        \
    /* start of object class content */                                                     \
    PFN_WEBCSPIF_ADD_DAEMON         AddHttpDaemon;                                          \
    PFN_WEBCSPIF_DEL_DAEMON         DelHttpDaemon;                                          \
    PFN_WEBCSPIF_SET_MODE           SetHttpServerMode;                                      \
    PFN_WEBCSPIF_SET_BOOL           SetPersistentConn;                                      \
    PFN_WEBCSPIF_SET_NAME           SetProductName;                                         \
    /* end of object class content */                                                       \

typedef  struct
_WEB_CSP_INTERFACE
{
    WEB_CSP_INTERFACE_CLASS_CONTENT
}
WEB_CSP_INTERFACE,  *PWEB_CSP_INTERFACE;

#define  ACCESS_WEB_CSP_INTERFACE(p)               \
         ACCESS_CONTAINER(p, WEB_CSP_INTERFACE, Linkage)


#endif
