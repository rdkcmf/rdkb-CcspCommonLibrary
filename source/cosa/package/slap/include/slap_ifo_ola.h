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

    module:	slap_ifo_ola.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Slap Object Logic Access Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/11/03    initial revision.

**********************************************************************/


#ifndef  _SLAP_IFO_OLA_
#define  _SLAP_IFO_OLA_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"

#include "slap_definitions.h"


/***********************************************************
        SLAP OBJECT LOGIC ACCESS INTERFACE DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  SLAP_OLA_INTERFACE_NAME                    "slapObjectLogicAccessIf"
#define  SLAP_OLA_INTERFACE_ID                      0

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SLAPOLAIF_GET_OPO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       property_name
    );

typedef  ANSC_HANDLE
(*PFN_SLAPOLAIF_GET_MDO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       method_name,
        SLAP_PARAMETER_LIST*        param_list_input
    );

/*
 * Instead of managing object logic properties (i.e., Object Property and Method Descriptor) in
 * every instance of the same Srv Component Object, we centralize the OLM (Object Logic Management)
 * in SLAP Obj Entity Object. By centralizing the logic management, we greatly reduce the memory
 * and runtime execution overhead associated with the object instantiation process.
 */
#define  SLAP_OLA_INTERFACE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_SLAPOLAIF_GET_OPO           GetObjectProperty;                                      \
    PFN_SLAPOLAIF_GET_MDO           GetMethodDescriptor;                                    \
    /* end of object class content */                                                       \

typedef  struct
_SLAP_OLA_INTERFACE
{
    SLAP_OLA_INTERFACE_CLASS_CONTENT
}
SLAP_OLA_INTERFACE,  *PSLAP_OLA_INTERFACE;

#define  ACCESS_SLAP_OLA_INTERFACE(p)               \
         ACCESS_CONTAINER(p, SLAP_OLA_INTERFACE, Linkage)


#endif
