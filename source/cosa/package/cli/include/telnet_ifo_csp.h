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

    module:	telnet_ifo_csp.h

        For TELNET,
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

        Kang Quan

    ---------------------------------------------------------------

    revision:

        06/09/05    initial revision.

**********************************************************************/


#ifndef  _TELNET_IFO_CSP_
#define  _TELNET_IFO_CSP_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"
#include "ansc_ifo_csp.h"


/***********************************************************
 TELNET CONFIGURATION SERVICE PROVIDER INTERFACE DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  TELNET_CSP_INTERFACE_NAME                  "telnetConfigurationServiceProviderIf"
#define  TELNET_CSP_INTERFACE_ID                    0

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_TELNETCSPIF_GET_IF)
    (
	    ANSC_HANDLE                 hThisObject
	);


typedef  ANSC_STATUS
(*PFN_TELNETCSPIF_SET_IF)
    (
	    ANSC_HANDLE                 hThisObject,
		ANSC_HANDLE                 value
	);


typedef  PUCHAR
(*PFN_TELNETCSPIF_GET_ADDR)
    (
        ANSC_HANDLE                 hThisObject
    );


typedef  ANSC_STATUS
(*PFN_TELNETCSPIF_SET_ADDR)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      value
    );


typedef  USHORT
(*PFN_TELNETCSPIF_GET_USHORT)
    (
        ANSC_HANDLE                 hThisObject
    );


typedef  ANSC_STATUS
(*PFN_TELNETCSPIF_SET_USHORT)
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      value
    );


typedef  ULONG
(*PFN_TELNETCSPIF_GET_ULONG)
    (
		ANSC_HANDLE					hThisObject
	);


typedef  ANSC_STATUS
(*PFN_TELNETCSPIF_SET_ULONG)
	(
		ANSC_HANDLE					hThisObject,
		ULONG                       value
	);


typedef  BOOL
(*PFN_TELNETCSPIF_GET_BOOL)
	(
		ANSC_HANDLE                 hThisObject
	);


typedef  ANSC_STATUS
(*PFN_TELNETCSPIF_SET_BOOL)
	(
		ANSC_HANDLE					hThisObject,
		BOOL                        flag
	);

typedef  PUCHAR
(*PFN_TELNETCSPIF_GET_PROMPT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession
    );

typedef  ANSC_STATUS
(*PFN_TELNETCSPIF_SET_PROMPT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        PUCHAR                      pString
    );

/*
 * As a general requirement, each module SHOULD provide an interface for external components to
 * configure its parameters and policies. Although the benefit of unifying the definition and usage
 * of such an interface is obvious, we DON'T want to impose any set of rules on the implementation.
 * Instead, we expect every module will implement its configuration interfaces independently.
 */
#define  TELNET_CSP_INTERFACE_CLASS_CONTENT                                                 \
    /* duplication of the base object class content */                                      \
    ANSC_CSP_INTERFACE_CLASS_CONTENT                                                        \
    /* start of object class content */                                                     \
    PFN_TELNETCSPIF_GET_IF          GetTspIf;                                               \
    PFN_TELNETCSPIF_SET_IF          SetTspIf;                                               \
	                                                                                        \
    PFN_TELNETCSPIF_GET_ADDR        GetHostAddress;                                         \
    PFN_TELNETCSPIF_SET_ADDR        SetHostAddress;                                         \
    PFN_TELNETCSPIF_GET_USHORT      GetHostPort;                                            \
    PFN_TELNETCSPIF_SET_USHORT      SetHostPort;                                            \
    PFN_TELNETCSPIF_GET_ULONG       GetServerType;                                          \
	PFN_TELNETCSPIF_SET_ULONG       SetServerType;                                          \
    PFN_TELNETCSPIF_GET_ULONG       GetServerMode;                                          \
	PFN_TELNETCSPIF_SET_ULONG       SetServerMode;                                          \
    PFN_TELNETCSPIF_GET_ULONG       GetServerOptions;                                       \
	PFN_TELNETCSPIF_SET_ULONG       SetServerOptions;                                       \
    PFN_TELNETCSPIF_GET_PROMPT      GetShellPrompt;                                         \
    PFN_TELNETCSPIF_SET_PROMPT      SetShellPrompt;                                         \
    /* end of object class content */                                                       \

typedef  struct
_TELNET_CSP_INTERFACE
{
    TELNET_CSP_INTERFACE_CLASS_CONTENT
}
TELNET_CSP_INTERFACE,  *PTELNET_CSP_INTERFACE;

#define  ACCESS_TELNET_CSP_INTERFACE(p)                \
         ACCESS_CONTAINER(p, TELNET_CSP_INTERFACE, Linkage)


#endif
