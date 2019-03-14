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

    module:	bree_ifo_csp.h

        For BSP Run-time Execution Engine (BREE),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the BSP Run-time Execution Engine (BREE).

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        06/08/04    initial revision.

**********************************************************************/


#ifndef  _BREE_IFO_CSP_DEC_
#define  _BREE_IFO_CSP_DEC_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/***********************************************************
    BMW WEB CONTENT PROVIDER INTERFACE OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  BREE_CSP_INTERFACE_NAME                    "breeSpoCspIf"
#define  BREE_CSP_INTERFACE_ID                      0

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */

typedef  ANSC_STATUS
(*PFN_BREECSPIF_SET_ROOTPATH)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pWebRootPath
    );

typedef  PUCHAR
(*PFN_BREECSPIF_GET_ROOTPATH)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * For each Request Handler Object, there's WCP interface associated with it. The WCP interface
 * provides a general way for isolating the front-end web components from content authoring and
 * dynamic page generation. Instead of designing a unique interface for each handler, we design
 * this interface to be general enough to support all scenarios.
 */
#define  BREE_CSP_INTERFACE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_BREECSPIF_SET_ROOTPATH      SetWebRootPath;                                         \
    PFN_BREECSPIF_GET_ROOTPATH      GetWebRootPath;                                         \
    /* end of object class content */                                                       \

typedef  struct
_BREE_CSP_INTERFACE
{
    BREE_CSP_INTERFACE_CLASS_CONTENT
}
BREE_CSP_INTERFACE,  *PBREE_CSP_INTERFACE;

#define  ACCESS_BREE_CSP_INTERFACE(p)                \
         ACCESS_CONTAINER(p, BREE_CSP_INTERFACE, Linkage)


#endif
