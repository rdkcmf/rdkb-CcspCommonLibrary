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

    module:	cont_filter_mco_interface.h

        For BroadWay Content Filtering Module Container,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Content Filtering Module 
        Container Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        09/10/02    initial revision.

**********************************************************************/


#ifndef  _CONT_FILTER_MCO_INTERFACE_
#define  _CONT_FILTER_MCO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_mco_interface.h"
#include "ansc_mco_external_api.h"


/***********************************************************
   PLATFORM INDEPENDENT OBJECT CONTAINER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  CONT_FILTER_MODULE_CONTAINER_NAME          "contFilterModuleContainer"
#define  CONT_FILTER_MODULE_CONTAINER_OID           ANSC_OBJECT_OID_NULL
#define  CONT_FILTER_MODULE_CONTAINER_TYPE          ANSC_OBJECT_TYPE_NULL


/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */

typedef  ANSC_HANDLE
(*PFN_MCO_CONT_FILTER_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_MCO_CONT_FILTER_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_MCO_CONT_FILTER_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_MCO_CONT_FILTER_GET_CARRIER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSessionContext
    );

typedef  ANSC_STATUS
(*PFN_MCO_CONT_FILTER_SET_CARRIER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSessionContext,
        ANSC_HANDLE                 hCarrier,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_MCO_CONT_FILTER_ACTION)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_MCO_CONT_FILTER_CUT)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * As a standard practice, we encapsulate all functional and feature objects inside an Object
 * Container Object so it will function as a black-box implementation when we have to integrate
 * an Ansc component with other systems. However, that's not the only reason why we need to
 * define the Container Object: we also use container as the building block for constructing
 * larger systems.
 */
#define CONT_FILTER_MODULE_CONTAINER_CLASS_CONTENT                                          \
    /* duplication of the base object class content */                                      \
    ANSC_MODULE_CONTAINER_CLASS_CONTENT                                                     \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hCfso;                                                  \
                                                                                            \
    PFN_MCO_CONT_FILTER_GET_CONTEXT GetCfso;                                                \
    /* end of object class content */                                                       \

typedef  struct
_CONT_FILTER_MODULE_CONTAINER_OBJECT
{
    CONT_FILTER_MODULE_CONTAINER_CLASS_CONTENT
}
CONT_FILTER_MODULE_CONTAINER_OBJECT,  *PCONT_FILTER_MODULE_CONTAINER_OBJECT;

#define  ACCESS_CONT_FILTER_MODULE_CONTAINER_OBJECT(p)      \
         ACCESS_CONTAINER(p, CONT_FILTER_MODULE_CONTAINER_OBJECT, Linkage)


#endif
