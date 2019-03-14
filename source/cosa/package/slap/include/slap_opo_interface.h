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

    module:	slap_opo_interface.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Slap Obj Proxy Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        10/06/03    initial revision.

**********************************************************************/


#ifndef  _SLAP_OPO_INTERFACE_
#define  _SLAP_OPO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_lco_interface.h"
#include "ansc_lco_external_api.h"

#include "slap_properties.h"


/***********************************************************
          SLAP OBJ ENTITY COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SLAPOPO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPOPO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_SLAPOPO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPOPO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ANSC_STATUS
(*PFN_SLAPOPO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_SLAPOPO_NEW_SPO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapGoaObj
    );

/*
 * The SLAP Obj Proxy Object is the place-holder for a SLAP SrvProxy Object. Its functionality is
 * similar to the Obj Entity Object except that the Obj Entity Object performs much more than just
 * a place-holder. Because the SrvProxy Objects are managed directly by the application module,
 * there's no need for OLM and pooling.
 */
#define  SLAP_OBJ_PROXY_CLASS_CONTENT                                                       \
    /* duplication of the base object class content */                                      \
    ANSCLCO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hSlapUoaIf;                                             \
    ANSC_HANDLE                     hObjDescriptor;                                         \
                                                                                            \
    PFN_SLAPOPO_SET_CONTEXT         SetObjDescriptor;                                       \
                                                                                            \
    PFN_SLAPOPO_NEW_SPO             NewSrvProxy;                                            \
    /* end of object class content */                                                       \

typedef  struct
_SLAP_OBJ_PROXY_OBJECT
{
    SLAP_OBJ_PROXY_CLASS_CONTENT
}
SLAP_OBJ_PROXY_OBJECT,  *PSLAP_OBJ_PROXY_OBJECT;

#define  ACCESS_SLAP_OBJ_PROXY_OBJECT(p)            \
         ACCESS_CONTAINER(p, SLAP_OBJ_PROXY_OBJECT, Linkage)


#endif
