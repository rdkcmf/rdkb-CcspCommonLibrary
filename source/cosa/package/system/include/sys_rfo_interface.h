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

    module:	sys_rfo_interface.h

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Sys Repository Folder Objects.

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


#ifndef  _SYS_RFO_INTERFACE_
#define  _SYS_RFO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_lco_interface.h"
#include "ansc_lco_external_api.h"
#include "sys_properties.h"

#include "sys_ifo_rca.h"
#include "sys_ifo_rcg.h"
#include "sys_ifo_rcm.h"


/***********************************************************
      SYS REPOSITORY FOLDER COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  SYS_RFO_RFO_TABLE_SIZE                     8
#define  SYS_RFO_RRO_TABLE_SIZE                     4
#define  SYS_RFO_SCRATCH_PAD_SIZE                   128
#define  SYS_RFO_DEF_ARRAY_SIZE                     64

#define  SYS_RFO_SORTING_TYPE_BYTIME                1
#define  SYS_RFO_SORTING_TYPE_BYNAME                2

#define  SYS_RFO_SORTING_MODE_ASCENDING             1
#define  SYS_RFO_SORTING_MODE_DESCENDING            2

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SYSRFO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSRFO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_SYSRFO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSRFO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ULONG
(*PFN_SYSRFO_GET_RESERVED)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSRFO_SET_RESERVED)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       reserved
    );

typedef  ULONG
(*PFN_SYSRFO_GET_TIME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSRFO_SET_TIME)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulTime
    );

typedef  char*
(*PFN_SYSRFO_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSRFO_SET_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ULONG
(*PFN_SYSRFO_GET_TYPE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSRFO_SET_TYPE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       type
    );

typedef  ANSC_STATUS
(*PFN_SYSRFO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_SYSRFO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_SYSRFO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSRFO_ACQUIRE_AS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSRFO_RELEASE_AS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSRFO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSRFO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSRFO_DELETE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_SYSRFO_ISSUE_KEY)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulAccessMode
    );

typedef  ANSC_STATUS
(*PFN_SYSRFO_CLEAR_KEY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hKey
    );

typedef  ANSC_STATUS
(*PFN_SYSRFO_DEL_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSRFO_ADD_RFO_AE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSubFolder
    );

typedef  ANSC_STATUS
(*PFN_SYSRFO_DEL_RFO_AE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSubFolder
    );

typedef  ANSC_STATUS
(*PFN_SYSRFO_ADD_RRO_AE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRecord
    );

typedef  ANSC_STATUS
(*PFN_SYSRFO_DEL_RRO_AE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRecord
    );

typedef  ANSC_STATUS
(*PFN_SYSRFO_CLEAN_UP)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSRFO_SORT_RFOS)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bAscending
    );

typedef  ANSC_STATUS
(*PFN_SYSRFO_SORT_RROS)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bAscending
    );

typedef  char*
(*PFN_SYSRFO_QUERY_PATH)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pPathString
    );

typedef  BOOL
(*PFN_SYSRFO_IS_SOMETHING)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  BOOL
(*PFN_SYSRFO_IS_REMOVABLE)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bRecursive
    );

typedef  BOOL
(*PFN_SYSRFO_GRANT_ACCESS)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulAccessMode
    );

typedef  ULONG
(*PFN_SYSRFO_GET_COUNT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_SYSRFO_ADD_FOLDER)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pFolderName,
        ULONG                       ulPermission,
        ULONG                       ulFolderType,
        ANSC_HANDLE                 hRenderAttr,
        ANSC_HANDLE                 hReserved
    );

typedef  ANSC_STATUS
(*PFN_SYSRFO_DEL_FOLDER)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pFolderName,
        BOOL                        bRecursive,
        BOOL                        bForceful
    );

typedef  ANSC_HANDLE
(*PFN_SYSRFO_GET_FOLDER1)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex
    );

typedef  ANSC_HANDLE
(*PFN_SYSRFO_GET_FOLDER2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pFolderName
    );

typedef  ANSC_HANDLE
(*PFN_SYSRFO_ADD_RECORD)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRecordName,
        ULONG                       ulPermission,
        ULONG                       ulRecordType,
        ANSC_HANDLE                 hRenderAttr,
        PVOID                       pBuffer,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_SYSRFO_DEL_RECORD)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRecordName
    );

typedef  ANSC_HANDLE
(*PFN_SYSRFO_GET_RECORD1)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex
    );

typedef  ANSC_HANDLE
(*PFN_SYSRFO_GET_RECORD2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRecordName
    );

typedef  ANSC_STATUS
(*PFN_SYSRFO_SET_RECORD)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRecordName,
        ULONG                       ulRecordType,
        ANSC_HANDLE                 hRenderAttr,
        PVOID                       pBuffer,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_SYSRFO_GENERATE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSRFO_POPULATE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSRFO_OPEN_FIRST)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSRFO_CLOSE_LAST)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulFlags
    );

/*
 * Reassembling the typical operations related to a tree-node, the Sys Repository Folder Object
 * manages multiple sub folders and a set of records. Instead of deriving more-specific folder
 * object for each of the three folder types: storage, guarded, and dynamic. We simple encapsulate
 * all the necessary interfaces in this base object while still opens to the possibility that
 * internal/external modules may further implement special folders.
 */
#define  SYS_REPOSITORY_FOLDER_CLASS_CONTENT                                                \
    /* duplication of the base object class content */                                      \
    ANSCLCO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    SYS_REPOSITORY_FOLDER_PROPERTY  Property;                                               \
    ULONG                           HashIndex;                                              \
    ULONG                           GlobalFid;                                              \
    ULONG                           GlobalRid;                                              \
    ULONG                           BirthTime;                                              \
    ULONG                           Timestamp;                                              \
    ANSC_HANDLE                     hRenderAttr;                                            \
    ANSC_HANDLE                     hRecycleBin;                                            \
    ANSC_HANDLE                     hParentFolder;                                          \
    ANSC_HANDLE                     hUserContext;                                           \
    ULONG                           UserReserved;                                           \
    ANSC_HANDLE                     hOpenRcaIf;                                             \
    ANSC_HANDLE                     hOpenRcgIf;                                             \
    ANSC_HANDLE                     hOpenRcmIf;                                             \
    ANSC_HANDLE                     hSelfRcaIf;                                             \
    ANSC_HANDLE                     hSelfRcgIf;                                             \
    ANSC_HANDLE                     hSelfRcmIf;                                             \
    ULONG                           CurAccessMode;                                          \
    ULONG                           RefCount;                                               \
    ANSC_TS_LOCK                    AccessTsLock;                                           \
    BOOL                            bActive;                                                \
                                                                                            \
    char*                           AbsolutePath;                                           \
                                                                                            \
    ULONG                           FolderSortingType;                                      \
    ULONG                           FolderSortingMode;                                      \
    ANSC_HANDLE*                    FolderArray;                                            \
    ULONG                           FolderArrayMaxLen;                                      \
    ULONG                           FolderArrayCurLen;                                      \
    QUEUE_HEADER                    FolderTable[SYS_RFO_RFO_TABLE_SIZE];                    \
    ANSC_LOCK                       FolderTableLock;                                        \
                                                                                            \
    ULONG                           RecordSortingType;                                      \
    ULONG                           RecordSortingMode;                                      \
    ANSC_HANDLE*                    RecordArray;                                            \
    ULONG                           RecordArrayMaxLen;                                      \
    ULONG                           RecordArrayCurLen;                                      \
    QUEUE_HEADER                    RecordTable[SYS_RFO_RRO_TABLE_SIZE];                    \
    ANSC_LOCK                       RecordTableLock;                                        \
                                                                                            \
    PFN_SYSRFO_GET_TIME             GetBirthTime;                                           \
    PFN_SYSRFO_SET_TIME             SetBirthTime;                                           \
    PFN_SYSRFO_GET_CONTEXT          GetRenderAttr;                                          \
    PFN_SYSRFO_SET_CONTEXT          SetRenderAttr;                                          \
    PFN_SYSRFO_GET_CONTEXT          GetRecycleBin;                                          \
    PFN_SYSRFO_SET_CONTEXT          SetRecycleBin;                                          \
    PFN_SYSRFO_GET_CONTEXT          GetParentFolder;                                        \
    PFN_SYSRFO_SET_CONTEXT          SetParentFolder;                                        \
    PFN_SYSRFO_GET_CONTEXT          GetUserContext;                                         \
    PFN_SYSRFO_SET_CONTEXT          SetUserContext;                                         \
    PFN_SYSRFO_GET_RESERVED         GetUserReserved;                                        \
    PFN_SYSRFO_SET_RESERVED         SetUserReserved;                                        \
    PFN_SYSRFO_GET_IF               GetRcaIf;                                               \
    PFN_SYSRFO_SET_IF               SetRcaIf;                                               \
    PFN_SYSRFO_GET_IF               GetRcgIf;                                               \
    PFN_SYSRFO_SET_IF               SetRcgIf;                                               \
    PFN_SYSRFO_GET_IF               GetRcmIf;                                               \
    PFN_SYSRFO_SET_IF               SetRcmIf;                                               \
                                                                                            \
    PFN_SYSRFO_GET_NAME             GetFolderName;                                          \
    PFN_SYSRFO_SET_NAME             SetFolderName;                                          \
    PFN_SYSRFO_GET_TYPE             GetFolderType;                                          \
    PFN_SYSRFO_SET_TYPE             SetFolderType;                                          \
    PFN_SYSRFO_GET_TYPE             GetPermission;                                          \
    PFN_SYSRFO_SET_TYPE             SetPermission;                                          \
    PFN_SYSRFO_GET_TYPE             GetContentType;                                         \
    PFN_SYSRFO_SET_TYPE             SetContentType;                                         \
                                                                                            \
    PFN_SYSRFO_GET_PROPERTY         GetProperty;                                            \
    PFN_SYSRFO_SET_PROPERTY         SetProperty;                                            \
    PFN_SYSRFO_RESET                ResetProperty;                                          \
    PFN_SYSRFO_RESET                Reset;                                                  \
                                                                                            \
    PFN_SYSRFO_ACQUIRE_AS           AcquireAccess;                                          \
    PFN_SYSRFO_RELEASE_AS           ReleaseAccess;                                          \
    PFN_SYSRFO_ENGAGE               Engage;                                                 \
    PFN_SYSRFO_CANCEL               Cancel;                                                 \
    PFN_SYSRFO_DELETE               Delete;                                                 \
    PFN_SYSRFO_ISSUE_KEY            IssueKey;                                               \
    PFN_SYSRFO_CLEAR_KEY            ClearKey;                                               \
                                                                                            \
    PFN_SYSRFO_DEL_ALL              DelAllFolders;                                          \
    PFN_SYSRFO_DEL_ALL              DelAllRecords;                                          \
    PFN_SYSRFO_ADD_RFO_AE           AddRfoIntoArray;                                        \
    PFN_SYSRFO_DEL_RFO_AE           DelRfoFromArray;                                        \
    PFN_SYSRFO_CLEAN_UP             CleanUpRfoArray;                                        \
    PFN_SYSRFO_ADD_RRO_AE           AddRroIntoArray;                                        \
    PFN_SYSRFO_DEL_RRO_AE           DelRroFromArray;                                        \
    PFN_SYSRFO_CLEAN_UP             CleanUpRroArray;                                        \
                                                                                            \
    PFN_SYSRFO_SORT_RFOS            SortFolderByTime;                                       \
    PFN_SYSRFO_SORT_RFOS            SortFolderByName;                                       \
    PFN_SYSRFO_SORT_RROS            SortRecordByTime;                                       \
    PFN_SYSRFO_SORT_RROS            SortRecordByName;                                       \
                                                                                            \
    PFN_SYSRFO_QUERY_PATH           QueryAbsPath;                                           \
    PFN_SYSRFO_IS_REMOVABLE         IsRemovable;                                            \
    PFN_SYSRFO_GRANT_ACCESS         GrantAccess;                                            \
    PFN_SYSRFO_GET_COUNT            GetFolderCount;                                         \
    PFN_SYSRFO_GET_COUNT            GetRecordCount;                                         \
                                                                                            \
    PFN_SYSRFO_ADD_FOLDER           AddFolder;                                              \
    PFN_SYSRFO_DEL_FOLDER           DelFolder;                                              \
    PFN_SYSRFO_GET_FOLDER1          GetFolderByIndex;                                       \
    PFN_SYSRFO_GET_FOLDER2          GetFolderByName;                                        \
                                                                                            \
    PFN_SYSRFO_ADD_RECORD           AddRecord;                                              \
    PFN_SYSRFO_DEL_RECORD           DelRecord;                                              \
    PFN_SYSRFO_GET_RECORD1          GetRecordByIndex;                                       \
    PFN_SYSRFO_GET_RECORD2          GetRecordByName;                                        \
    PFN_SYSRFO_SET_RECORD           SetRecord;                                              \
                                                                                            \
    PFN_SYSRFO_GENERATE             GenerateContent;                                        \
    PFN_SYSRFO_POPULATE             PopulateContent;                                        \
    PFN_SYSRFO_OPEN_FIRST           OpenFirst;                                              \
    PFN_SYSRFO_CLOSE_LAST           CloseLast;                                              \
                                                                                            \
    PFN_SYSRCAIF_NOTIFY             RcaNotify;                                              \
    PFN_SYSRCAIF_REVIEW             RcaReview;                                              \
                                                                                            \
    PFN_SYSRCGIF_IS_SOMETHING       RcgNeedsUpdate;                                         \
    PFN_SYSRCGIF_SYNCHRONIZE        RcgSynchronize;                                         \
                                                                                            \
    PFN_SYSRCMIF_NOTIFY             RcmNotify;                                              \
    PFN_SYSRCMIF_INIT               RcmInit;                                                \
    PFN_SYSRCMIF_STOP               RcmStop;                                                \
    /* end of object class content */                                                       \

typedef  struct
_SYS_REPOSITORY_FOLDER_OBJECT
{
    SYS_REPOSITORY_FOLDER_CLASS_CONTENT
}
SYS_REPOSITORY_FOLDER_OBJECT,  *PSYS_REPOSITORY_FOLDER_OBJECT;

#define  ACCESS_SYS_REPOSITORY_FOLDER_OBJECT(p)     \
         ACCESS_CONTAINER(p, SYS_REPOSITORY_FOLDER_OBJECT, Linkage)


#endif
