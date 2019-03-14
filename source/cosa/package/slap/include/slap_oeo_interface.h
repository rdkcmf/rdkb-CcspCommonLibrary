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

    module:	slap_oeo_interface.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Slap Obj Entity Objects.

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


#ifndef  _SLAP_OEO_INTERFACE_
#define  _SLAP_OEO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_lco_interface.h"
#include "ansc_lco_external_api.h"

#include "slap_properties.h"
#include "slap_ifo_ola.h"


/***********************************************************
          SLAP OBJ ENTITY COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  SLAP_OEO_ORO_TABLE_SIZE                    4

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SLAPOEO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPOEO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_SLAPOEO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPOEO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ANSC_STATUS
(*PFN_SLAPOEO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPOEO_SETUP_TABLE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPOEO_REGISTER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCallDescriptor
    );

typedef  ANSC_HANDLE
(*PFN_SLAPOEO_GET_OPO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       property_name
    );

typedef  ANSC_STATUS
(*PFN_SLAPOEO_ADD_OPO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_SLAPOEO_DEL_OPO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       property_name
    );

typedef  ANSC_STATUS
(*PFN_SLAPOEO_DEL_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_SLAPOEO_GET_MDO1)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       method_name
    );

typedef  ANSC_HANDLE
(*PFN_SLAPOEO_GET_MDO2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       method_name,
        SLAP_PARAMETER_LIST*        param_list_input
    );

typedef  ANSC_STATUS
(*PFN_SLAPOEO_ADD_MDO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDescriptor
    );

typedef  ANSC_STATUS
(*PFN_SLAPOEO_DEL_MDO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       method_name
    );

typedef  ANSC_HANDLE
(*PFN_SLAPOEO_GET_ORO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjContainer,
        ULONG                       instance_id
    );

typedef  ANSC_HANDLE
(*PFN_SLAPOEO_NEW_ORO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjContainer,
        ANSC_HANDLE                 hInsContext
    );

typedef  ANSC_STATUS
(*PFN_SLAPOEO_DEL_ORO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjContainer,
        ANSC_HANDLE                 hSrvComponent,
        BOOL                        bForceful
    );

typedef  ANSC_HANDLE
(*PFN_SLAPOEO_ACQ_ORO)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPOEO_REL_ORO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjRecord
    );

typedef  ANSC_STATUS
(*PFN_SLAPOEO_RESOLVE1)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       call_description,
        SLAP_PARAMETER_LIST*        param_list
    );

typedef  ANSC_STATUS
(*PFN_SLAPOEO_RESOLVE2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       call_description,
        SLAP_PARAMETER_LIST*        param_list
    );

typedef  ANSC_STATUS
(*PFN_SLAPOEO_RESOLVE3)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       call_description,
        SLAP_VARIABLE*              return_var
    );

typedef  ULONG
(*PFN_SLAPOEO_RESOLVE4)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       method_name
    );

typedef  ULONG
(*PFN_SLAPOEO_RESOLVE5)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       method_name,
        SLAP_PARAMETER_LIST*        param_list
    );

/*
 * The SLAP Obj Entity Object manages the multiple active instances derived from the same object
 * template (registered as the Obj Descriptor). Its baseline functionality is to serve as a runtime
 * object locator given the object container (and the instance_id). Its advanced functionality may
 * include read/write access control, object pooling and so on.
 */
#define  SLAP_OBJ_ENTITY_CLASS_CONTENT                                                      \
    /* duplication of the base object class content */                                      \
    ANSCLCO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hSlapOlaIf;                                             \
    ANSC_HANDLE                     hSlapUoaIf;                                             \
    ANSC_HANDLE                     hSlapBssIf;                                             \
    ANSC_HANDLE                     hObjDescriptor;                                         \
                                                                                            \
    QUEUE_HEADER*                   OpoTable;                                               \
    ANSC_LOCK                       OpoTableLock;                                           \
    ULONG                           OpoTableSize;                                           \
    QUEUE_HEADER*                   MdoTable;                                               \
    ANSC_LOCK                       MdoTableLock;                                           \
    ULONG                           MdoTableSize;                                           \
                                                                                            \
    QUEUE_HEADER                    OroTable[SLAP_OEO_ORO_TABLE_SIZE];                      \
    ANSC_LOCK                       OroTableLock;                                           \
    SLIST_HEADER                    PooledOroSList;                                         \
    ANSC_LOCK                       PooledOroSListLock;                                     \
                                                                                            \
    PFN_SLAPOEO_SET_CONTEXT         SetObjDescriptor;                                       \
    PFN_SLAPOEO_RESET               Reset;                                                  \
                                                                                            \
    PFN_SLAPOEO_SETUP_TABLE         SetupOpoTable;                                          \
    PFN_SLAPOEO_SETUP_TABLE         SetupMdoTable;                                          \
    PFN_SLAPOEO_REGISTER            RegisterCall;                                           \
                                                                                            \
    PFN_SLAPOEO_GET_OPO             GetObjectProperty;                                      \
    PFN_SLAPOEO_ADD_OPO             AddObjectProperty;                                      \
    PFN_SLAPOEO_DEL_OPO             DelObjectProperty;                                      \
    PFN_SLAPOEO_DEL_ALL             DelAllObjectProperties;                                 \
                                                                                            \
    PFN_SLAPOEO_GET_MDO1            GetMethodDescriptor1;                                   \
    PFN_SLAPOEO_GET_MDO2            GetMethodDescriptor2;                                   \
    PFN_SLAPOEO_ADD_MDO             AddMethodDescriptor;                                    \
    PFN_SLAPOEO_DEL_MDO             DelMethodDescriptor;                                    \
    PFN_SLAPOEO_DEL_ALL             DelAllMethodDescriptors;                                \
                                                                                            \
    PFN_SLAPOEO_GET_ORO             GetObjRecord;                                           \
    PFN_SLAPOEO_NEW_ORO             NewObjRecord;                                           \
    PFN_SLAPOEO_DEL_ORO             DelObjRecord;                                           \
    PFN_SLAPOEO_DEL_ALL             DelAllObjRecords;                                       \
                                                                                            \
    PFN_SLAPOEO_ACQ_ORO             AcqPooledObjRecord;                                     \
    PFN_SLAPOEO_REL_ORO             RelPooledObjRecord;                                     \
    PFN_SLAPOEO_DEL_ALL             DelAllPooledObjRecords;                                 \
                                                                                            \
    PFN_SLAPOEO_RESOLVE1            ResolveCallDescrI;                                      \
    PFN_SLAPOEO_RESOLVE2            ResolveCallDescrO;                                      \
    PFN_SLAPOEO_RESOLVE3            ResolveCallDescrR;                                      \
    PFN_SLAPOEO_RESOLVE4            ResolveCallBridge;                                      \
    PFN_SLAPOEO_RESOLVE5            ResolveCallOption;                                      \
                                                                                            \
    PFN_SLAPOLAIF_GET_OPO           OlaGetObjectProperty;                                   \
    PFN_SLAPOLAIF_GET_MDO           OlaGetMethodDescriptor;                                 \
    /* end of object class content */                                                       \

typedef  struct
_SLAP_OBJ_ENTITY_OBJECT
{
    SLAP_OBJ_ENTITY_CLASS_CONTENT
}
SLAP_OBJ_ENTITY_OBJECT,  *PSLAP_OBJ_ENTITY_OBJECT;

#define  ACCESS_SLAP_OBJ_ENTITY_OBJECT(p)           \
         ACCESS_CONTAINER(p, SLAP_OBJ_ENTITY_OBJECT, Linkage)


#endif
