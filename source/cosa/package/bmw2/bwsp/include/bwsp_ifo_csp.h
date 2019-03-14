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

    module:	bwsp_ifo_csp.h

        For Broadway Web Service Plane Implementation (BWSP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Bwsp Configuration Service Provider (CSP)
        Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/02/03    initial revision.

**********************************************************************/


#ifndef  _BWSP_IFO_CSP_
#define  _BWSP_IFO_CSP_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_csp.h"


/***********************************************************
  BWSP CONFIGURATION SERVICE PROVIDER INTERFACE DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  BWSP_CSP_INTERFACE_NAME                    "bwspConfigurationServiceProviderIf"
#define  BWSP_CSP_INTERFACE_ID                      0

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_BWSPCSPIF_CONFIG)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

/*
 * The BWSP framework is built on top of the baseline ISC web server architecture. While the ISC
 * web server offers great flexibility via the generic Virtual Host Object (VHO) and Resource Owner
 * Object (ROO), which can be derived to encapsulate various type web servers, it requires the web
 * service developers to understand the ANSC/BISGA programming convention and methodology. Moreover,
 * it's not very flexible in supporting the DLL-type extension mechanism, which is supported by all
 * popular desktop operating systems. BWSP was designed to address all these issues.
 */
#define  BWSP_CSP_INTERFACE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_BWSPCSPIF_CONFIG            ConfigProperty;                                         \
    /* end of object class content */                                                       \

typedef  struct
_BWSP_CSP_INTERFACE
{
    BWSP_CSP_INTERFACE_CLASS_CONTENT
}
BWSP_CSP_INTERFACE,  *PBWSP_CSP_INTERFACE;

#define  ACCESS_BWSP_CSP_INTERFACE(p)               \
         ACCESS_CONTAINER(p, BWSP_CSP_INTERFACE, Linkage)


#endif
