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

    module:	sys_iro_interface.h

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Sys Info Repository Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/02/02    initial revision.

**********************************************************************/


#ifndef  _SYS_IRO_INTERFACE_
#define  _SYS_IRO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "sys_properties.h"


/***********************************************************
       SYS INFO REPOSITORY COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  SYS_IRO_MAX_REP_ACCESS_COUNT               32

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SYSIRO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSIRO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_SYSIRO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSIRO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ANSC_STATUS
(*PFN_SYSIRO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_SYSIRO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_SYSIRO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSIRO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSIRO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSIRO_ACQ_ACCESS)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bReadAccess
    );

typedef  ANSC_STATUS
(*PFN_SYSIRO_REL_ACCESS)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bReadAccess
    );

/*
 * The Sys Info Repository Object is responsible for creating and managing a system-wide central
 * in-memory database. This database/repository is a tree-like structure, which consists of a set
 * of nodes at each level (only one at the root level). Each node may be associated with several
 * properties, which can be one of the pre-defined data types: integer, string, boolean, and
 * binary string.
 */
#define  SYS_INFO_REPOSITORY_CLASS_CONTENT                                                  \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    SYS_INFO_REPOSITORY_PROPERTY    Property;                                               \
    ANSC_HANDLE                     hIraIf;                                                 \
    ANSC_HANDLE                     hSysRamIf;                                              \
    ANSC_HANDLE                     hRepDriver;                                             \
    BOOL                            bActive;                                                \
    ANSC_SEMAPHORE                  AccessSemaphore;                                        \
    ANSC_LOCK                       AccessLock;                                             \
    ANSC_LOCK                       ThreadLock;                                             \
                                                                                            \
    PFN_SYSIRO_GET_IF               GetIraIf;                                               \
    PFN_SYSIRO_GET_CONTEXT          GetRepDriver;                                           \
                                                                                            \
    PFN_SYSIRO_GET_PROPERTY         GetProperty;                                            \
    PFN_SYSIRO_SET_PROPERTY         SetProperty;                                            \
    PFN_SYSIRO_RESET                ResetProperty;                                          \
    PFN_SYSIRO_RESET                Reset;                                                  \
                                                                                            \
    PFN_SYSIRO_ENGAGE               Engage;                                                 \
    PFN_SYSIRO_CANCEL               Cancel;                                                 \
    PFN_SYSIRO_ACQ_ACCESS           AcqAccess;                                              \
    PFN_SYSIRO_REL_ACCESS           RelAccess;                                              \
                                                                                            \
    PFN_SYSIRAIF_ACQ_ACCESS         IraAcqWriteAccess;                                      \
    PFN_SYSIRAIF_REL_ACCESS         IraRelWriteAccess;                                      \
                                                                                            \
    PFN_SYSIRAIF_ADDSYS_FOLDER      IraAddSysFolder;                                        \
    PFN_SYSIRAIF_ADD_FOLDER         IraAddFolder;                                           \
    PFN_SYSIRAIF_ADD_FOLDER2        IraAddFolder2;                                          \
    PFN_SYSIRAIF_DEL_FOLDER         IraDelFolder;                                           \
    PFN_SYSIRAIF_DEL_FOLDER2        IraDelFolder2;                                          \
                                                                                            \
    PFN_SYSIRAIF_OPEN_FOLDER        IraOpenFolder;                                          \
    PFN_SYSIRAIF_OPEN_FOLDER2       IraOpenFolder2;                                         \
    PFN_SYSIRAIF_CLOSE_FOLDER       IraCloseFolder;                                         \
    PFN_SYSIRAIF_CLEAR_FOLDER       IraClearFolder;                                         \
                                                                                            \
    PFN_SYSIRAIF_GENERATE           IraGenerateFolder;                                      \
    PFN_SYSIRAIF_POPULATE           IraPopulateFolder;                                      \
                                                                                            \
    PFN_SYSIRAIF_QUERY_FOLDER       IraQueryFolder;                                         \
    PFN_SYSIRAIF_GET_COUNT          IraGetSubFolderCount;                                   \
    PFN_SYSIRAIF_ENUM_FOLDER        IraEnumSubFolder;                                       \
    PFN_SYSIRAIF_TEST_FOLDER        IraTestSubFolder;                                       \
                                                                                            \
    PFN_SYSIRAIF_ADD_RECORD         IraAddRecord;                                           \
    PFN_SYSIRAIF_ADD_RECORD2        IraAddRecord2;                                          \
    PFN_SYSIRAIF_DEL_RECORD         IraDelRecord;                                           \
    PFN_SYSIRAIF_GET_RECORD         IraGetRecord;                                           \
    PFN_SYSIRAIF_SET_RECORD         IraSetRecord;                                           \
                                                                                            \
    PFN_SYSIRAIF_QUERY_RECORD       IraQueryRecord;                                         \
    PFN_SYSIRAIF_GET_COUNT          IraGetRecordCount;                                      \
    PFN_SYSIRAIF_ENUM_RECORD        IraEnumRecord;                                          \
    PFN_SYSIRAIF_TEST_RECORD        IraTestRecord;                                          \
                                                                                            \
    PFN_SYSIRAIF_GET_RFO_USER       IraGetRfoUserContext;                                   \
    PFN_SYSIRAIF_SET_RFO_USER       IraSetRfoUserContext;                                   \
    PFN_SYSIRAIF_GET_RFO_USER2      IraGetRfoUserReserved;                                  \
    PFN_SYSIRAIF_SET_RFO_USER2      IraSetRfoUserReserved;                                  \
    PFN_SYSIRAIF_GET_RFO_TYPE       IraGetRfoFolderType;                                    \
    PFN_SYSIRAIF_SET_RFO_TYPE       IraSetRfoFolderType;                                    \
    PFN_SYSIRAIF_GET_RFO_PERM       IraGetRfoPermission;                                    \
    PFN_SYSIRAIF_SET_RFO_PERM       IraSetRfoPermission;                                    \
    PFN_SYSIRAIF_GET_RFO_ATTR       IraGetRfoRenderAttr;                                    \
    PFN_SYSIRAIF_SET_RFO_ATTR       IraSetRfoRenderAttr;                                    \
                                                                                            \
    PFN_SYSIRAIF_GET_RRO_USER       IraGetRroUserContext;                                   \
    PFN_SYSIRAIF_SET_RRO_USER       IraSetRroUserContext;                                   \
    PFN_SYSIRAIF_GET_RRO_USER2      IraGetRroUserReserved;                                  \
    PFN_SYSIRAIF_SET_RRO_USER2      IraSetRroUserReserved;                                  \
    PFN_SYSIRAIF_GET_RRO_PERM       IraGetRroPermission;                                    \
    PFN_SYSIRAIF_SET_RRO_PERM       IraSetRroPermission;                                    \
    PFN_SYSIRAIF_GET_RRO_ATTR       IraGetRroRenderAttr;                                    \
    PFN_SYSIRAIF_SET_RRO_ATTR       IraSetRroRenderAttr;                                    \
                                                                                            \
    PFN_SYSIRAIF_GET_RBO            IraGetCurRecycleBin;                                    \
    PFN_SYSIRAIF_ATTACH_RBO         IraAttachRecycleBin;                                    \
    PFN_SYSIRAIF_DETACH_RBO         IraDetachRecycleBin;                                    \
                                                                                            \
    PFN_SYSIRAIF_SORT               IraSortSubFolders;                                      \
    PFN_SYSIRAIF_SORT               IraSortRecords;                                         \
                                                                                            \
    PFN_SYSIRAIF_GET_IF             IraGetSysRamIf;                                         \
    PFN_SYSIRAIF_SET_IF             IraSetSysRamIf;                                         \
    /* end of object class content */                                                       \

typedef  struct
_SYS_INFO_REPOSITORY_OBJECT
{
    SYS_INFO_REPOSITORY_CLASS_CONTENT
}
SYS_INFO_REPOSITORY_OBJECT,  *PSYS_INFO_REPOSITORY_OBJECT;

#define  ACCESS_SYS_INFO_REPOSITORY_OBJECT(p)       \
         ACCESS_CONTAINER(p, SYS_INFO_REPOSITORY_OBJECT, Linkage)


#endif
