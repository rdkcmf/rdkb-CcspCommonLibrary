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

    module:	slap_oco_interface.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Slap Obj Container Objects.

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


#ifndef  _SLAP_OCO_INTERFACE_
#define  _SLAP_OCO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "slap_properties.h"


/***********************************************************
        SLAP OBJ CONTAINER COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  SLAP_OCO_SCO_TABLE_SIZE                    64

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SLAPOCO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPOCO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_SLAPOCO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPOCO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  char*
(*PFN_SLAPOCO_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPOCO_SET_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       container_name
    );

typedef  ULONG
(*PFN_SLAPOCO_GET_TYPE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPOCO_SET_TYPE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       container_type
    );

typedef  ANSC_STATUS
(*PFN_SLAPOCO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPOCO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPOCO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ULONG
(*PFN_SLAPOCO_ASSIGN_ID)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

typedef  ANSC_HANDLE
(*PFN_SLAPOCO_GET_SCO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name,
        ULONG                       instance_id
    );

typedef  ANSC_STATUS
(*PFN_SLAPOCO_ADD_SCO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvComponent
    );

typedef  ANSC_STATUS
(*PFN_SLAPOCO_DEL_SCO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvComponent
    );

typedef  ANSC_STATUS
(*PFN_SLAPOCO_DEL_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * SLAP supports a hierarchical object name space, which can be extended to support new objects and
 * remote object invocation. The SLAP Obj Container Object is different from some other container
 * objects, such as AnscObjectContainer and AnscModuleContainer. The SLAP Obj Container allows the
 * other service modules to create a runtime environment where the life-cycle and behavior of the
 * service logic objects can be controlled and managed in a unified manner.
 */
#define  SLAP_OBJ_CONTAINER_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hSlapUoaIf;                                             \
    ANSC_HANDLE                     hSlapBssIf;                                             \
    ANSC_HANDLE                     hSlapObjMapper;                                         \
    char*                           ContainerName;                                          \
    ULONG                           ContainerType;                                          \
    ULONG                           GlobalInsId;                                            \
    ULONG                           HashIndex;                                              \
                                                                                            \
    QUEUE_HEADER                    ScoTable[SLAP_OCO_SCO_TABLE_SIZE];                      \
    ANSC_LOCK                       ScoTableLock;                                           \
                                                                                            \
    PFN_SLAPOCO_GET_IF              GetSlapUoaIf;                                           \
    PFN_SLAPOCO_SET_IF              SetSlapUoaIf;                                           \
    PFN_SLAPOCO_GET_IF              GetSlapBssIf;                                           \
    PFN_SLAPOCO_SET_IF              SetSlapBssIf;                                           \
    PFN_SLAPOCO_GET_CONTEXT         GetSlapObjMapper;                                       \
    PFN_SLAPOCO_SET_CONTEXT         SetSlapObjMapper;                                       \
    PFN_SLAPOCO_GET_NAME            GetContainerName;                                       \
    PFN_SLAPOCO_SET_NAME            SetContainerName;                                       \
    PFN_SLAPOCO_GET_TYPE            GetContainerType;                                       \
    PFN_SLAPOCO_SET_TYPE            SetContainerType;                                       \
    PFN_SLAPOCO_RESET               Reset;                                                  \
                                                                                            \
    PFN_SLAPOCO_ASSIGN_ID           AssignInstanceId;                                       \
    PFN_SLAPOCO_GET_SCO             GetSrvComponent;                                        \
    PFN_SLAPOCO_ADD_SCO             AddSrvComponent;                                        \
    PFN_SLAPOCO_DEL_SCO             DelSrvComponent;                                        \
    PFN_SLAPOCO_DEL_ALL             DelAllSrvComponents;                                    \
    /* end of object class content */                                                       \

typedef  struct
_SLAP_OBJ_CONTAINER_OBJECT
{
    SLAP_OBJ_CONTAINER_CLASS_CONTENT
}
SLAP_OBJ_CONTAINER_OBJECT,  *PSLAP_OBJ_CONTAINER_OBJECT;

#define  ACCESS_SLAP_OBJ_CONTAINER_OBJECT(p)        \
         ACCESS_CONTAINER(p, SLAP_OBJ_CONTAINER_OBJECT, Linkage)


#endif
