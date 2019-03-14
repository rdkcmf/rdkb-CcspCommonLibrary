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

    module:	ansc_mco_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Module Container Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/12/02    initial revision.

**********************************************************************/


#ifndef  _ANSC_MCO_INTERFACE_
#define  _ANSC_MCO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_oco_interface.h"
#include "ansc_oco_external_api.h"


/***********************************************************
   PLATFORM INDEPENDENT MODULE CONTAINER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_MCO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_MCO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_MCO_GET_CARRIER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_MCO_SET_CARRIER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCarrier,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_MCO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_MCO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_MCO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_MCO_IGNITE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_MCO_PUTOUT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_MCO_SWITCH)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bSwitchOn
    );

typedef  ANSC_STATUS
(*PFN_MCO_ADD)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pIfName,
        ANSC_HANDLE                 hInterface
    );

typedef  ANSC_STATUS
(*PFN_MCO_DEL)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pIfName
    );

typedef  ANSC_HANDLE
(*PFN_MCO_QUERY)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pIfName
    );

typedef  ANSC_STATUS
(*PFN_MCO_REGISTER_IFS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_MCO_DISPATCH_IFS)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hIqcIf
    );

/*
 * As a standard practice, we encapsulate all functional and feature objects inside an Object
 * Container Object so it will function as a black-box implementation when we have to integrate
 * an Ansc component with other systems. However, that's not the only reason why we need to
 * define the Container Object: we also use container as the building block for constructing
 * larger systems.
 */
#define  ANSC_MODULE_CONTAINER_CLASS_CONTENT                                                \
    /* duplication of the base object class content */                                      \
    ANSC_OBJECT_CONTAINER_CLASS_CONTENT                                                     \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hQueryIfObject;                                         \
                                                                                            \
    PFN_MCO_GET_CONTEXT             GetQueryIfObject;                                       \
                                                                                            \
    PFN_MCO_IGNITE                  Ignite;                                                 \
    PFN_MCO_PUTOUT                  PutOut;                                                 \
    PFN_MCO_SWITCH                  Switch;                                                 \
                                                                                            \
    PFN_MCO_ADD                     AddModuleIf;                                            \
    PFN_MCO_DEL                     DelModuleIf;                                            \
    PFN_MCO_QUERY                   QueryModuleIf;                                          \
                                                                                            \
    PFN_MCO_REGISTER_IFS            RegisterIfs;                                            \
    PFN_MCO_DISPATCH_IFS            DispatchIfs;                                            \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_MODULE_CONTAINER_OBJECT
{
    ANSC_MODULE_CONTAINER_CLASS_CONTENT
}
ANSC_MODULE_CONTAINER_OBJECT,  *PANSC_MODULE_CONTAINER_OBJECT;

#define  ACCESS_ANSC_MODULE_CONTAINER_OBJECT(p)     \
         ACCESS_CONTAINER(p, ANSC_MODULE_CONTAINER_OBJECT, Linkage)


#endif
