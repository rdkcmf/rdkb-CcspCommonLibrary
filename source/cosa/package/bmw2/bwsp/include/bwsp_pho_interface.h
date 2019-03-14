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

    module:	bwsp_pho_interface.h

        For Broadway Web Service Plane Implementation (BWSP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Bwsp Portal Host Object.

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


#ifndef  _BWSP_PHO_INTERFACE_
#define  _BWSP_PHO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "web_vho_interface.h"
#include "web_vho_exported_api.h"


/***********************************************************
   PLATFORM INDEPENDENT BWSP PORTAL HOST OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_BWSPPHO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BWSPPHO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_BWSPPHO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BWSPPHO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

/*
 * This object is derived from the Web Virtual Host Object, which is implemented by the baseline
 * Web module. Here we merely use this object as a placeholder and the starting point for laying
 * down the rest web-based management framework. This object MUST be explicitly registered to the
 * web server during environment setup.
 */
#define  BWSP_PORTAL_HOST_CLASS_CONTENT                                                     \
    /* duplication of the base object class content */                                      \
    WEB_VIRTUAL_HOST_CLASS_CONTENT                                                          \
    /* start of object class content */                                                     \
    /* end of object class content */                                                       \

typedef  struct
_BWSP_PORTAL_HOST_OBJECT
{
    BWSP_PORTAL_HOST_CLASS_CONTENT
}
BWSP_PORTAL_HOST_OBJECT,  *PBWSP_PORTAL_HOST_OBJECT;

#define  ACCESS_BWSP_PORTAL_HOST_OBJECT(p)          \
         ACCESS_CONTAINER(p, BWSP_PORTAL_HOST_OBJECT, Linkage)


#endif
