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

    module:	ansc_ifo_csp.h

        For Advanced Networking Service Container (ANSC),
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

        05/13/02    initial revision.

**********************************************************************/


#ifndef  _ANSC_IFO_CSP_
#define  _ANSC_IFO_CSP_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/***********************************************************
  ANSC CONFIGURATION SERVICE PROVIDER INTERFACE DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  ANSC_CSP_INTERFACE_NAME                    "anscConfigurationServiceProviderIf"
#define  ANSC_CSP_INTERFACE_ID                      0

#define  ANSC_CFG_MODE_READ                         0x00000001
#define  ANSC_CFG_MODE_WRITE                        0x00000002
#define  ANSC_CFG_MODE_RDWR                         0x00000003

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ULONG
(*PFN_ANSCCSPIF_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  BOOL
(*PFN_ANSCCSPIF_IS_SOMETHING)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ANSCCSPIF_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ANSCCSPIF_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * As a general requirement, each module SHOULD provide an interface for external components to
 * configure its parameters and policies. Although the benefit of unifying the definition and usage
 * of such an interface is obvious, we DON'T want to impose any set of rules on the implementation.
 * Instead, we expect every module will implement its configuration interfaces independently.
 */
#define  ANSC_CSP_INTERFACE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_ANSCCSPIF_GET_MODE          GetCfgMode;                                             \
    PFN_ANSCCSPIF_IS_SOMETHING      IsReadOnly;                                             \
    PFN_ANSCCSPIF_IS_SOMETHING      IsSwitchOn;                                             \
    PFN_ANSCCSPIF_ENGAGE            Engage;                                                 \
    PFN_ANSCCSPIF_CANCEL            Cancel;                                                 \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_CSP_INTERFACE
{
    ANSC_CSP_INTERFACE_CLASS_CONTENT
}
ANSC_CSP_INTERFACE,  *PANSC_CSP_INTERFACE;

#define  ACCESS_ANSC_CSP_INTERFACE(p)               \
         ACCESS_CONTAINER(p, ANSC_CSP_INTERFACE, Linkage)


#endif
