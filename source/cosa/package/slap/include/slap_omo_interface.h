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

    module:	slap_omo_interface.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Slap Obj Mapper Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/04/03    initial revision.

**********************************************************************/


#ifndef  _SLAP_OMO_INTERFACE_
#define  _SLAP_OMO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "slap_properties.h"


/***********************************************************
          SLAP OBJ MAPPER COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SLAPOMO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPOMO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_SLAPOMO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPOMO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ANSC_STATUS
(*PFN_SLAPOMO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_SLAPOMO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_SLAPOMO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPOMO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPOMO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_SLAPOMO_MAK_OBO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTokenChain
    );

typedef  ANSC_HANDLE
(*PFN_SLAPOMO_GET_OBO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTokenChain
    );

typedef  ANSC_HANDLE
(*PFN_SLAPOMO_GET_OEO1)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    );

typedef  ANSC_HANDLE
(*PFN_SLAPOMO_GET_OEO2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTokenChain
    );

typedef  ANSC_STATUS
(*PFN_SLAPOMO_ADD_OEO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path,
        ANSC_HANDLE                 hObjEntity
    );

typedef  ANSC_STATUS
(*PFN_SLAPOMO_POP_OEO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    );

typedef  ANSC_STATUS
(*PFN_SLAPOMO_DEL_OEO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path,
        BOOL                        bFreeObjEntity
    );

typedef  ANSC_HANDLE
(*PFN_SLAPOMO_GET_OPO1)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    );

typedef  ANSC_HANDLE
(*PFN_SLAPOMO_GET_OPO2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTokenChain
    );

typedef  ANSC_STATUS
(*PFN_SLAPOMO_ADD_OPO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path,
        ANSC_HANDLE                 hObjProxy
    );

typedef  ANSC_STATUS
(*PFN_SLAPOMO_POP_OPO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    );

typedef  ANSC_STATUS
(*PFN_SLAPOMO_DEL_OPO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path,
        BOOL                        bFreeObjProxy
    );

/*
 * SLAP supports a hierarchical object name space, which can be extended to support new objects and
 * remote object invocation. SLAP Obj Mapper Object is designed to manage such name space by employ-
 * ing a tree-like structure to maintain all the registered OBJECT_DESCRIPTORS. This object is very
 * similar to WebResourceLocator object in many aspects while it also resembles AnscObjectMapper
 * object in some way.
 */
#define  SLAP_OBJ_MAPPER_CLASS_CONTENT                                                      \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    SLAP_OBJ_MAPPER_PROPERTY        Property;                                               \
    ANSC_HANDLE                     hRootObjBroker;                                         \
    BOOL                            bActive;                                                \
                                                                                            \
    PFN_SLAPOMO_GET_CONTEXT         GetRootObjBroker;                                       \
    PFN_SLAPOMO_GET_PROPERTY        GetProperty;                                            \
    PFN_SLAPOMO_SET_PROPERTY        SetProperty;                                            \
    PFN_SLAPOMO_RESET               ResetProperty;                                          \
    PFN_SLAPOMO_RESET               Reset;                                                  \
                                                                                            \
    PFN_SLAPOMO_ENGAGE              Engage;                                                 \
    PFN_SLAPOMO_CANCEL              Cancel;                                                 \
                                                                                            \
    PFN_SLAPOMO_MAK_OBO             MakObjBroker;                                           \
    PFN_SLAPOMO_GET_OBO             GetObjBroker;                                           \
    PFN_SLAPOMO_GET_OEO1            GetObjEntity1;                                          \
    PFN_SLAPOMO_GET_OEO2            GetObjEntity2;                                          \
    PFN_SLAPOMO_ADD_OEO             AddObjEntity;                                           \
    PFN_SLAPOMO_POP_OEO             PopObjEntity;                                           \
    PFN_SLAPOMO_DEL_OEO             DelObjEntity;                                           \
                                                                                            \
    PFN_SLAPOMO_GET_OPO1            GetObjProxy1;                                           \
    PFN_SLAPOMO_GET_OPO2            GetObjProxy2;                                           \
    PFN_SLAPOMO_ADD_OPO             AddObjProxy;                                            \
    PFN_SLAPOMO_POP_OPO             PopObjProxy;                                            \
    PFN_SLAPOMO_DEL_OPO             DelObjProxy;                                            \
    /* end of object class content */                                                       \

typedef  struct
_SLAP_OBJ_MAPPER_OBJECT
{
    SLAP_OBJ_MAPPER_CLASS_CONTENT
}
SLAP_OBJ_MAPPER_OBJECT,  *PSLAP_OBJ_MAPPER_OBJECT;

#define  ACCESS_SLAP_OBJ_MAPPER_OBJECT(p)           \
         ACCESS_CONTAINER(p, SLAP_OBJ_MAPPER_OBJECT, Linkage)


#endif
