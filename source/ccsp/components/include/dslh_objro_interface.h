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

    module: dslh_objro_interface.h

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Dslh Obj Record Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Bin     Zhu

    ---------------------------------------------------------------

    revision:

        09/23/2005    initial revision.
        03/03/2009    Bin add "bFromAcs" for 2 apis
        01/18/2011    Bin added dynamically populate/unpopulate object apis;

**********************************************************************/


#ifndef  _DSLH_OBJRO_INTERFACE_
#define  _DSLH_OBJRO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "dslh_properties.h"

#include "dslh_ifo_rvq.h"


/***********************************************************
         DSLH OBJ RECORD COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  DSLH_OBJRO_VARRO_TABLE_SIZE                16

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_DSLHOBJRO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJRO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_DSLHOBJRO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJRO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  char*
(*PFN_DSLHOBJRO_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJRO_SET_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  BOOL
(*PFN_DSLHOBJRO_GET_BOOL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJRO_SET_BOOL)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnabled
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJRO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJRO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJRO_DESTROY)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJRO_ACTION)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJRO_POPULATE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJRO_POP_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pObjName
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJRO_CHANGED)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJRO_CREATED)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hChildObjController,
        ULONG                       ulInstanceNumber
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJRO_REMOVED)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulInstanceNumber
    );

typedef  ULONG
(*PFN_DSLHOBJRO_GET_COUNT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ULONG
(*PFN_DSLHOBJRO_GET_COUNT2)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bExcludeHidden
    );

typedef  ANSC_HANDLE
(*PFN_DSLHOBJRO_GET_OBJRO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ANSC_HANDLE
(*PFN_DSLHOBJRO_GET_OBJRO2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTokenChain
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJRO_ADD_OBJRO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjRecord
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJRO_DEL_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_DSLHOBJRO_GET_VARRO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJRO_ADD_VARRO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVarRecord
    );

typedef  ULONG
(*PFN_DSLHOBJRO_GET_TOTAL_NUM)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bFromAcs,
        BOOL                        bSelfIncluded
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJRO_GET_VALUES)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hParamValueArray,
        PULONG                      pulArraySize,
        BOOL                        bFromACS,
        unsigned int                writeID
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJRO_GET_INFOS)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hParamInfoArray,
        PULONG                      pulArraySize,
        BOOL                        bNextLevelOnly,
        BOOL                        bFromACS
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJRO_GET_ATTRIBS)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hParamAttribArray,
        PULONG                      pulArraySize,
        BOOL                        bFromACS
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJRO_SET_ATTRIBS)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSetParamAttrib
    );

typedef  BOOL
(*PFN_DSLHOBJRO_VERIFY)
    (
        ANSC_HANDLE                 hThisObject,
        char**                      ppFaultParamName
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJRO_COMMIT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJRO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJRO_ADD_CHILD)
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pulInstanceNumber
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJRO_DEL_CHILD)
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
#define  DSLH_OBJ_RECORD_CLASS_CONTENT                                                      \
    /* duplication of the base object class content */                                      \
    SINGLE_LINK_ENTRY               Linkage;                                                \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hParentObjRecord;                                       \
    ANSC_HANDLE                     hDslhCpeController;                                     \
    ANSC_HANDLE                     hDslhObjController;                                     \
    ANSC_HANDLE                     hDslhObjEntity;                                         \
    ANSC_HANDLE                     hIrepFoWmpDatabase;                                     \
    ANSC_HANDLE                     hDslhRvqIf;                                             \
    char*                           LastName;                                               \
    char*                           FullName;                                               \
    ULONG                           NextInstanceNumber;                                     \
                                                                                            \
    QUEUE_HEADER                    ObjroQueue;                                             \
    QUEUE_HEADER                    VarroTable;                                             \
                                                                                            \
    PFN_DSLHOBJRO_SET_CONTEXT       SetDslhObjEntity;                                       \
    PFN_DSLHOBJRO_SET_NAME          SetLastName;                                            \
    PFN_DSLHOBJRO_SET_NAME          SetFullName;                                            \
    PFN_DSLHOBJRO_DESTROY           Destroy;                                                \
    PFN_DSLHOBJRO_ACTION            Reset;                                                  \
                                                                                            \
    PFN_DSLHOBJRO_ACTION            Remove;                                                 \
                                                                                            \
    PFN_DSLHOBJRO_POPULATE          PopulateObjRecords;                                     \
    PFN_DSLHOBJRO_POPULATE          PopulateVarRecords;                                     \
    PFN_DSLHOBJRO_CREATED           ChildObjectCreated;                                     \
    PFN_DSLHOBJRO_REMOVED           ChildObjectRemoved;                                     \
    PFN_DSLHOBJRO_POP_NAME          PopulateObjRecordByName;                                \
    PFN_DSLHOBJRO_POP_NAME          DeleteObjRecordByName;                                  \
                                                                                            \
    PFN_DSLHOBJRO_GET_COUNT         GetObjRecordCount;                                      \
    PFN_DSLHOBJRO_GET_OBJRO         GetObjRecord;                                           \
    PFN_DSLHOBJRO_GET_OBJRO2        GetObjRecord2;                                          \
    PFN_DSLHOBJRO_ADD_OBJRO         AddObjRecord;                                           \
    PFN_DSLHOBJRO_DEL_ALL           DelAllObjRecords;                                       \
    PFN_DSLHOBJRO_GET_COUNT2        GetVarRecordCount;                                      \
    PFN_DSLHOBJRO_GET_VARRO         GetVarRecord;                                           \
    PFN_DSLHOBJRO_ADD_VARRO         AddVarRecord;                                           \
                                                                                            \
    PFN_DSLHOBJRO_GET_TOTAL_NUM     GetTotalParamNumber;                                    \
    PFN_DSLHOBJRO_GET_VALUES        GetAllParamValues;                                      \
    PFN_DSLHOBJRO_GET_INFOS         GetAllParamInfos;                                       \
    PFN_DSLHOBJRO_GET_ATTRIBS       GetAllParamAttribs;                                     \
    PFN_DSLHOBJRO_SET_ATTRIBS       SetAllParamAttribs;                                     \
                                                                                            \
    PFN_DSLHOBJRO_VERIFY            VerifyChanges;                                          \
    PFN_DSLHOBJRO_COMMIT            CommitChanges;                                          \
    PFN_DSLHOBJRO_COMMIT            CommitChanges2;                                         \
    PFN_DSLHOBJRO_CANCEL            CancelChanges;                                          \
    PFN_DSLHOBJRO_ADD_CHILD         AddChildObject;                                         \
    PFN_DSLHOBJRO_DEL_CHILD         DelChildObject;                                         \
                                                                                            \
    /* end of object class content */                                                       \

typedef  struct
_DSLH_OBJ_RECORD_OBJECT
{
    DSLH_OBJ_RECORD_CLASS_CONTENT
}
DSLH_OBJ_RECORD_OBJECT,  *PDSLH_OBJ_RECORD_OBJECT;

#define  ACCESS_DSLH_OBJ_RECORD_OBJECT(p)           \
         ACCESS_CONTAINER(p, DSLH_OBJ_RECORD_OBJECT, Linkage)


/***********************************************************
       DSLH OBJ RECORD CHANGE LOG PARAMETER DEFINITION
***********************************************************/

/*
 * On successful receipt of a SetParameterValues RPC, the CPE MUST apply the changes to each of the
 * specified Parameters immediately and atomically. The order of Parameters listed in the Parameter-
 * List has no significance - the application of value changes to the CPE MUST be independent from
 * the order in which they are listed.
 */
typedef  struct
_DSLH_OBJRO_CHANGE_LOG_ENTRY
{
    SINGLE_LINK_ENTRY               Linkage;
    ANSC_HANDLE                     hVarRecord;
    SLAP_VARIABLE*                  NewValue;
}
DSLH_OBJRO_CHANGE_LOG_ENTRY, *PDSLH_OBJRO_CHANGE_LOG_ENTRY;

#define  ACCESS_DSLH_OBJRO_CHANGE_LOG_ENTRY(p)      \
         ACCESS_CONTAINER(p, DSLH_OBJRO_CHANGE_LOG_ENTRY, Linkage)


#endif
