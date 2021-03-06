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

    module:	slap_bmc2termo_interface.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Slap Bmc2 Terminal Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/24/05    initial revision.

**********************************************************************/


#ifndef  _SLAP_BMC2TERMO_INTERFACE_
#define  _SLAP_BMC2TERMO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "slap_scogal_interface.h"
#include "slap_scogal_exported_api.h"


/***********************************************************
       SLAP BMC2 TERMINAL COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  SLAP_BMC2TERMO_UOA_NAME                    "Services.Bmc2.Terminal"

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_BMC2TERMO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2TERMO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_BMC2TERMO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2TERMO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ANSC_STATUS
(*PFN_BMC2TERMO_TERMINATE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2TERMO_ENTER)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pDomainName
    );

typedef  char*
(*PFN_BMC2TERMO_GET_USERNAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2TERMO_SET_USERNAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pUsername
    );

/*
 * The Slap Bmc2 Terminal Object is created when the Shell program creates a new Virtual Terminal.
 * It's usually associated with a unique Telnet client or a console session. Currently this object
 * doesn't support functions other than Terminate() and Lock()/Unlock(). We may add more features
 * for terminal control in the future. For example: we can dedicate a portion of the screen to
 * display the statistics and performance data.
 */
#define  SLAP_BMC2_TERMINAL_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    SLAP_SCO_GALLERY_CLASS_CONTENT                                                          \
    /* start of object class content */                                                     \
    PFN_BMC2TERMO_TERMINATE         Terminate;                                              \
    PFN_BMC2TERMO_ENTER             EnterDomain;                                            \
    PFN_BMC2TERMO_GET_USERNAME      GetUsername;                                            \
    PFN_BMC2TERMO_SET_USERNAME      SetUsername;                                            \
    /* end of object class content */                                                       \

typedef  struct
_SLAP_BMC2_TERMINAL_OBJECT
{
    SLAP_BMC2_TERMINAL_CLASS_CONTENT
}
SLAP_BMC2_TERMINAL_OBJECT,  *PSLAP_BMC2_TERMINAL_OBJECT;

#define  ACCESS_SLAP_BMC2_TERMINAL_OBJECT(p)        \
         ACCESS_CONTAINER(p, SLAP_BMC2_TERMINAL_OBJECT, Linkage)
#define  ACCESS_SLAP_BMC2_TERMINAL_FIELD(f)         \
         ACCESS_FIELD_OFFSET(SLAP_BMC2_TERMINAL_OBJECT, f)


#endif
