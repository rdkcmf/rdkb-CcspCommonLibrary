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

    module:	dslh_objeo_interface.h

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Dslh Obj Entity Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Bin Zhu

    ---------------------------------------------------------------

    revision:

        09/23/2005    initial revision.
        01/11/2011    Bin added "GetVarEntity";
        01/18/2011    Bin added dynamically populate/unpopulate object apis;
        02/07/2011    Bin added several functions to support "UnregisterObject";

**********************************************************************/


#ifndef  _DSLH_OBJEO_INTERFACE_
#define  _DSLH_OBJEO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_lco_interface.h"
#include "ansc_lco_external_api.h"
#include "dslh_properties.h"


/***********************************************************
         DSLH OBJ ENTITY COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  DSLH_OBJEO_VAREO_TABLE_SIZE                16

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_DSLHOBJEO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJEO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_DSLHOBJEO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJEO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  char*
(*PFN_DSLHOBJEO_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJEO_SET_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  BOOL
(*PFN_DSLHOBJEO_GET_BOOL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJEO_SET_BOOL)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnabled
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJEO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJEO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJEO_ENABLE)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnabled
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJEO_ACTION)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_DSLHOBJEO_GET_OBJEO2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTokenChain
    );

typedef  ANSC_HANDLE
(*PFN_DSLHOBJEO_GET_OBJEO3)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_DSLHOBJEO_GET_VARE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJEO_ADD_OBJEO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjEntity
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJEO_DEL_OBJEO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pObjName
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJEO_ADD_VAREO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVarEntity
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJEO_DEL_VAREO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pVarName
    );

typedef  ULONG
(*PFN_DSLHOBJEO_GET_COUNT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ULONG
(*PFN_DSLHOBJEO_GET_COUNT2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pPartialPath
    );

typedef  ANSC_HANDLE
(*PFN_DSLHOBJEO_GET_OBJRO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName,
        SLAP_VARIABLE*              pParamValue
    );

typedef  ANSC_HANDLE
(*PFN_DSLHOBJEO_GET_OBJRO2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pPartialPath,
        ULONG                       ulIndex
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJEO_ADD_OBJRO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjRecord
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJEO_DEL_OBJRO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjRecord
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJEO_REG_ACT)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pObjName
    );

/*
 * The CWMP (CPE WAN Management Protocol) parameter management model is implemented by aggregating
 * the data-oriented service calls provided by different SLAP objects (which may be located in diff-
 * erent processes). Even though the real configuration parameters are managed in a distributed
 * fashion, it's useful to maintain a centralized parameter database in a tree-like structure. For
 * example, this database will maintain the attribute and access control information for each para-
 * meter while the target module doesn't have to be aware of it.
 */
#define  DSLH_OBJ_ENTITY_CLASS_CONTENT                                                      \
    /* start of object class content */                                                     \
    SINGLE_LINK_ENTRY               Linkage;                                                \
    ANSC_HANDLE                     hParentObjEntity;                                       \
    DSLH_CWMP_OBJECT_DESCR*         ObjDescr;                                               \
    char*                           LastName;                                               \
    BOOL                            bForbidNotify;                                          \
                                                                                            \
    ULONG                           InsUpdateTimestamp;                                     \
    QUEUE_HEADER                    ObjInsQueue;                                            \
    QUEUE_HEADER                    ObjeoQueue;                                             \
    QUEUE_HEADER                    VareoTable;                                             \
    PFN_DSLHOBJEO_ENABLE            EnableForbidNotify;                                     \
    PFN_DSLHOBJEO_ACTION            Reset;                                                  \
    PFN_DSLHOBJEO_ACTION            Remove;                                                 \
                                                                                            \
    PFN_DSLHOBJEO_GET_OBJEO2        GetObjEntity2;                                          \
    PFN_DSLHOBJEO_GET_OBJEO3        GetObjEntity3;                                          \
    PFN_DSLHOBJEO_ADD_OBJEO         AddObjEntity;                                           \
    PFN_DSLHOBJEO_DEL_OBJEO         DelObjEntity;                                           \
    PFN_DSLHOBJEO_ADD_VAREO         AddVarEntity;                                           \
    PFN_DSLHOBJEO_DEL_VAREO         DelVarEntity;                                           \
    PFN_DSLHOBJEO_GET_VARE          GetVarEntity;                                           \
                                                                                            \
    PFN_DSLHOBJEO_GET_COUNT         GetObjRecordCount;                                      \
    PFN_DSLHOBJEO_GET_COUNT2        GetObjRecordCount2;                                     \
    PFN_DSLHOBJEO_GET_OBJRO         GetObjRecord;                                           \
    PFN_DSLHOBJEO_GET_OBJRO2        GetObjRecord2;                                          \
    PFN_DSLHOBJEO_ADD_OBJRO         AddObjRecord;                                           \
    PFN_DSLHOBJEO_DEL_OBJRO         DelObjRecord;                                           \
    PFN_DSLHOBJEO_REG_ACT           PopulateObjRecordByName;                                \
    PFN_DSLHOBJEO_REG_ACT           DeleteObjRecordByName;                                  \
    /* end of object class content */                                                       \

typedef  struct
_DSLH_OBJ_ENTITY_OBJECT
{
    DSLH_OBJ_ENTITY_CLASS_CONTENT
}
DSLH_OBJ_ENTITY_OBJECT,  *PDSLH_OBJ_ENTITY_OBJECT;

#define  ACCESS_DSLH_OBJ_ENTITY_OBJECT(p)           \
         ACCESS_CONTAINER(p, DSLH_OBJ_ENTITY_OBJECT, Linkage)


/***********************************************************
        DSLH OBJ ENTITY RUNTIME INSTANCE DEFINITION
***********************************************************/

/*
 * Each ObjEntity may spawn multiple runtime instances (i.e. ObjRecord). It's very useful to keep
 * track of the runtime instance creation and removal in ObjEntity so we can implement advanced
 * features like identifying a object based on the value of a particular parameter.
 */
typedef  struct
_DSLH_OBJEO_INSTANCE
{
    SINGLE_LINK_ENTRY               Linkage;
    ANSC_HANDLE                     hObjRecord;
}
DSLH_OBJEO_INSTANCE,  *PDSLH_OBJEO_INSTANCE;

#define  ACCESS_DSLH_OBJEO_INSTANCE(p)              \
         ACCESS_CONTAINER(p, DSLH_OBJEO_INSTANCE, Linkage)


#endif
