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

    module:	sys_ifo_ira.h

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Sys Info Repository Access Interface
        Objects.

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


#ifndef  _SYS_IFO_IRA_
#define  _SYS_IFO_IRA_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/***********************************************************
       SYS INFO REPOSITORY ACCESS INTERFACE DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  SYS_IRA_INTERFACE_NAME                     "sysInfoRepositoryAccessIf"
#define  SYS_IRA_INTERFACE_ID                       0

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_SYSIRAIF_ACQ_ACCESS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSIRAIF_REL_ACCESS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_SYSIRAIF_ADDSYS_FOLDER)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pFolderName
    );

typedef  ANSC_HANDLE
(*PFN_SYSIRAIF_ADD_FOLDER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pSubFolderName
    );

typedef  ANSC_HANDLE
(*PFN_SYSIRAIF_ADD_FOLDER2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pSubFolderName,
        ULONG                       ulPermission,
        ULONG                       ulAccessMode,
        ULONG                       ulFolderType,
        ANSC_HANDLE                 hRenderAttr,
        ANSC_HANDLE                 hReserved
    );

typedef  ANSC_STATUS
(*PFN_SYSIRAIF_DEL_FOLDER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pSubFolderName,
        BOOL                        bRecursive
    );

typedef  ANSC_STATUS
(*PFN_SYSIRAIF_DEL_FOLDER2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pSubFolderName,
        BOOL                        bRecursive,
        BOOL                        bForceful
    );

typedef  ANSC_HANDLE
(*PFN_SYSIRAIF_OPEN_FOLDER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pSubFolderName
    );

typedef  ANSC_HANDLE
(*PFN_SYSIRAIF_OPEN_FOLDER2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pSubFolderName,
        ULONG                       ulAccessMode
    );

typedef  ANSC_STATUS
(*PFN_SYSIRAIF_CLOSE_FOLDER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    );

typedef  ANSC_STATUS
(*PFN_SYSIRAIF_CLEAR_FOLDER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    );

typedef  ANSC_STATUS
(*PFN_SYSIRAIF_GENERATE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    );

typedef  ANSC_STATUS
(*PFN_SYSIRAIF_POPULATE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    );

typedef  ANSC_STATUS
(*PFN_SYSIRAIF_QUERY_FOLDER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        PULONG                      pulTimestamp,
        PULONG                      pulPermission,
        PULONG                      pulFolderType,
        PULONG                      pulSubFolderCount,
        PULONG                      pulRecordCount,
        PULONG                      pulContentType,
        PANSC_HANDLE                phRenderAttr
    );

typedef  ULONG
(*PFN_SYSIRAIF_GET_COUNT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    );

typedef  ANSC_STATUS
(*PFN_SYSIRAIF_ENUM_FOLDER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ULONG                       ulIndex,
        char*                       pSubFolderName,
        PULONG                      pulNameSize
    );

typedef  BOOL
(*PFN_SYSIRAIF_TEST_FOLDER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pSubFolderName,
        ULONG                       ulAccessMode
    );

typedef  ANSC_STATUS
(*PFN_SYSIRAIF_ADD_RECORD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName,
        ULONG                       ulRecordType,
        PVOID                       pWriteBuffer,
        ULONG                       ulWriteSize
    );

typedef  ANSC_STATUS
(*PFN_SYSIRAIF_ADD_RECORD2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName,
        ULONG                       ulPermission,
        ULONG                       ulRecordType,
        ANSC_HANDLE                 hRenderAttr,
        PVOID                       pWriteBuffer,
        ULONG                       ulWriteSize
    );

typedef  ANSC_STATUS
(*PFN_SYSIRAIF_DEL_RECORD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName
    );

typedef  ANSC_STATUS
(*PFN_SYSIRAIF_GET_RECORD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName,
        PULONG                      pulRecordType,
        PANSC_HANDLE                phRenderAttr,
        PVOID                       pReadBuffer,
        PULONG                      pulReadSize
    );

typedef  ANSC_STATUS
(*PFN_SYSIRAIF_GET_RECORD_BY_INDEX)
    (
        ANSC_HANDLE         hThisObject,
        ANSC_HANDLE         hCurFolder,
        ULONG               ulIndex,
        CHAR                *pName, /* caller allocate */
        ULONG               *ulNameSize,
        ULONG               *ulRecType,
        ULONG               *ulContentType,
        ULONG               *ulAccess,
        UCHAR               **ppData, /* caller free */
        ULONG               *pDataSize
    );

typedef  ANSC_STATUS
(*PFN_SYSIRAIF_SET_RECORD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName,
        ULONG                       ulRecordType,
        ANSC_HANDLE                 hRenderAttr,
        PVOID                       pWriteBuffer,
        ULONG                       ulWriteSize
    );

typedef  ANSC_STATUS
(*PFN_SYSIRAIF_QUERY_RECORD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName,
        PULONG                      pulTimestamp,
        PULONG                      pulPermission,
        PULONG                      pulRecordType,
        PULONG                      pulRecordSize,
        PULONG                      pulContentType,
        PANSC_HANDLE                phRenderAttr
    );

typedef  ANSC_STATUS
(*PFN_SYSIRAIF_ENUM_RECORD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ULONG                       ulIndex,
        char*                       pRecordName,
        PULONG                      pulNameSize,
        PULONG                      pulRecordType,
        PULONG                      pulDataSize
    );

typedef  BOOL
(*PFN_SYSIRAIF_TEST_RECORD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName,
        ULONG                       ulAccessMode
    );

typedef  ANSC_HANDLE
(*PFN_SYSIRAIF_GET_RFO_USER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    );

typedef  ANSC_STATUS
(*PFN_SYSIRAIF_SET_RFO_USER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ANSC_HANDLE                 hUserContext
    );

typedef  ULONG
(*PFN_SYSIRAIF_GET_RFO_USER2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    );

typedef  ANSC_STATUS
(*PFN_SYSIRAIF_SET_RFO_USER2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ULONG                       reserved
    );

typedef  ULONG
(*PFN_SYSIRAIF_GET_RFO_TYPE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    );

typedef  ANSC_STATUS
(*PFN_SYSIRAIF_SET_RFO_TYPE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ULONG                       ulFolderType
    );

typedef  ULONG
(*PFN_SYSIRAIF_GET_RFO_PERM)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    );

typedef  ANSC_STATUS
(*PFN_SYSIRAIF_SET_RFO_PERM)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ULONG                       ulPermission
    );

typedef  ANSC_HANDLE
(*PFN_SYSIRAIF_GET_RFO_ATTR)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    );

typedef  ANSC_STATUS
(*PFN_SYSIRAIF_SET_RFO_ATTR)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ANSC_HANDLE                 hRenderAttr
    );

typedef  ANSC_HANDLE
(*PFN_SYSIRAIF_GET_RRO_USER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName
    );

typedef  ANSC_STATUS
(*PFN_SYSIRAIF_SET_RRO_USER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName,
        ANSC_HANDLE                 hUserContext
    );

typedef  ULONG
(*PFN_SYSIRAIF_GET_RRO_USER2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName
    );

typedef  ANSC_STATUS
(*PFN_SYSIRAIF_SET_RRO_USER2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName,
        ULONG                       reserved
    );

typedef  ULONG
(*PFN_SYSIRAIF_GET_RRO_PERM)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName
    );

typedef  ANSC_STATUS
(*PFN_SYSIRAIF_SET_RRO_PERM)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName,
        ULONG                       ulPermission
    );

typedef  ANSC_HANDLE
(*PFN_SYSIRAIF_GET_RRO_ATTR)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName
    );

typedef  ANSC_STATUS
(*PFN_SYSIRAIF_SET_RRO_ATTR)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName,
        ANSC_HANDLE                 hRenderAttr
    );

typedef  ANSC_HANDLE
(*PFN_SYSIRAIF_GET_RBO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    );

typedef  ANSC_HANDLE
(*PFN_SYSIRAIF_ATTACH_RBO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ANSC_HANDLE                 hRecycleBin
    );

typedef  ANSC_HANDLE
(*PFN_SYSIRAIF_DETACH_RBO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    );

typedef  ANSC_STATUS
(*PFN_SYSIRAIF_SORT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ULONG                       ulSortingType,
        BOOL                        bAscending
    );

typedef  ANSC_HANDLE
(*PFN_SYSIRAIF_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSIRAIF_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHandle
    );

/*
 * As part of the effort to minimize the integration cost and unify the development of different
 * system components, we divide the product development into three stages: component development,
 * packaging, and integration. We encapsulate all the system modules inside a system package object
 * which exposes following API functions. These functions are used by customers for customization
 * and final integration.
 */
#define  SYS_IRA_INTERFACE_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_SYSIRAIF_ACQ_ACCESS         AcqWriteAccess;                                         \
    PFN_SYSIRAIF_REL_ACCESS         RelWriteAccess;                                         \
    PFN_SYSIRAIF_ACQ_ACCESS         AcqThreadLock;                                         \
    PFN_SYSIRAIF_REL_ACCESS         RelThreadLock;                                         \
                                                                                            \
    PFN_SYSIRAIF_ADDSYS_FOLDER      AddSysFolder;                                           \
    PFN_SYSIRAIF_ADD_FOLDER         AddFolder;                                              \
    PFN_SYSIRAIF_ADD_FOLDER2        AddFolder2;                                             \
    PFN_SYSIRAIF_DEL_FOLDER         DelFolder;                                              \
    PFN_SYSIRAIF_DEL_FOLDER2        DelFolder2;                                             \
                                                                                            \
    PFN_SYSIRAIF_OPEN_FOLDER        OpenFolder;                                             \
    PFN_SYSIRAIF_OPEN_FOLDER2       OpenFolder2;                                            \
    PFN_SYSIRAIF_CLOSE_FOLDER       CloseFolder;                                            \
    PFN_SYSIRAIF_CLEAR_FOLDER       ClearFolder;                                            \
                                                                                            \
    PFN_SYSIRAIF_GENERATE           GenerateFolder;                                         \
    PFN_SYSIRAIF_POPULATE           PopulateFolder;                                         \
                                                                                            \
    PFN_SYSIRAIF_QUERY_FOLDER       QueryFolder;                                            \
    PFN_SYSIRAIF_GET_COUNT          GetSubFolderCount;                                      \
    PFN_SYSIRAIF_ENUM_FOLDER        EnumSubFolder;                                          \
    PFN_SYSIRAIF_TEST_FOLDER        TestSubFolder;                                          \
                                                                                            \
    PFN_SYSIRAIF_ADD_RECORD         AddRecord;                                              \
    PFN_SYSIRAIF_ADD_RECORD2        AddRecord2;                                             \
    PFN_SYSIRAIF_DEL_RECORD         DelRecord;                                              \
    PFN_SYSIRAIF_GET_RECORD         GetRecord;                                              \
    PFN_SYSIRAIF_GET_RECORD_BY_INDEX         GetRecordByIndex;                                              \
    PFN_SYSIRAIF_SET_RECORD         SetRecord;                                              \
                                                                                            \
    PFN_SYSIRAIF_QUERY_RECORD       QueryRecord;                                            \
    PFN_SYSIRAIF_GET_COUNT          GetRecordCount;                                         \
    PFN_SYSIRAIF_ENUM_RECORD        EnumRecord;                                             \
    PFN_SYSIRAIF_TEST_RECORD        TestRecord;                                             \
                                                                                            \
    PFN_SYSIRAIF_GET_RFO_USER       GetRfoUserContext;                                      \
    PFN_SYSIRAIF_SET_RFO_USER       SetRfoUserContext;                                      \
    PFN_SYSIRAIF_GET_RFO_USER2      GetRfoUserReserved;                                     \
    PFN_SYSIRAIF_SET_RFO_USER2      SetRfoUserReserved;                                     \
    PFN_SYSIRAIF_GET_RFO_TYPE       GetRfoFolderType;                                       \
    PFN_SYSIRAIF_SET_RFO_TYPE       SetRfoFolderType;                                       \
    PFN_SYSIRAIF_GET_RFO_PERM       GetRfoPermission;                                       \
    PFN_SYSIRAIF_SET_RFO_PERM       SetRfoPermission;                                       \
    PFN_SYSIRAIF_GET_RFO_ATTR       GetRfoRenderAttr;                                       \
    PFN_SYSIRAIF_SET_RFO_ATTR       SetRfoRenderAttr;                                       \
                                                                                            \
    PFN_SYSIRAIF_GET_RRO_USER       GetRroUserContext;                                      \
    PFN_SYSIRAIF_SET_RRO_USER       SetRroUserContext;                                      \
    PFN_SYSIRAIF_GET_RRO_USER2      GetRroUserReserved;                                     \
    PFN_SYSIRAIF_SET_RRO_USER2      SetRroUserReserved;                                     \
    PFN_SYSIRAIF_GET_RRO_PERM       GetRroPermission;                                       \
    PFN_SYSIRAIF_SET_RRO_PERM       SetRroPermission;                                       \
    PFN_SYSIRAIF_GET_RRO_ATTR       GetRroRenderAttr;                                       \
    PFN_SYSIRAIF_SET_RRO_ATTR       SetRroRenderAttr;                                       \
                                                                                            \
    PFN_SYSIRAIF_GET_RBO            GetCurRecycleBin;                                       \
    PFN_SYSIRAIF_ATTACH_RBO         AttachRecycleBin;                                       \
    PFN_SYSIRAIF_DETACH_RBO         DetachRecycleBin;                                       \
                                                                                            \
    PFN_SYSIRAIF_SORT               SortSubFolders;                                         \
    PFN_SYSIRAIF_SORT               SortRecords;                                            \
                                                                                            \
    PFN_SYSIRAIF_GET_IF             GetSysRamIf;                                            \
    PFN_SYSIRAIF_SET_IF             SetSysRamIf;                                            \
    /* end of object class content */                                                       \

typedef  struct
_SYS_IRA_INTERFACE
{
    SYS_IRA_INTERFACE_CLASS_CONTENT
}
SYS_IRA_INTERFACE,  *PSYS_IRA_INTERFACE;

#define  ACCESS_SYS_IRA_INTERFACE(p)                \
         ACCESS_CONTAINER(p, SYS_IRA_INTERFACE, Linkage)


#endif
