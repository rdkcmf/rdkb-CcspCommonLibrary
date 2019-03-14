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

    module:	sys_rfo_internal_api.h

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Sys Repository
        Folder Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/06/02    initial revision.

**********************************************************************/


#ifndef  _SYS_RFO_INTERNAL_API_
#define  _SYS_RFO_INTERNAL_API_


/***********************************************************
           FUNCTIONS IMPLEMENTED IN SYS_RFO_STATES.C
***********************************************************/

ULONG
SysRfoGetBirthTime
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysRfoSetBirthTime
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulTime
    );

ANSC_HANDLE
SysRfoGetRenderAttr
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysRfoSetRenderAttr
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRenderAttr
    );

ANSC_HANDLE
SysRfoGetRecycleBin
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysRfoSetRecycleBin
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRecycleBin
    );

ANSC_HANDLE
SysRfoGetParentFolder
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysRfoSetParentFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFolder
    );

ANSC_HANDLE
SysRfoGetUserContext
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysRfoSetUserContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

ULONG
SysRfoGetUserReserved
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysRfoSetUserReserved
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       reserved
    );

ANSC_HANDLE
SysRfoGetRcaIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysRfoSetRcaIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
SysRfoGetRcgIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysRfoSetRcgIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
SysRfoGetRcmIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysRfoSetRcmIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

char*
SysRfoGetFolderName
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysRfoSetFolderName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

ULONG
SysRfoGetFolderType
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysRfoSetFolderType
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulType
    );

ULONG
SysRfoGetPermission
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysRfoSetPermission
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       permission
    );

ULONG
SysRfoGetContentType
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysRfoSetContentType
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulType
    );

ANSC_STATUS
SysRfoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
SysRfoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
SysRfoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysRfoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN SYS_RFO_OPERATION.C
***********************************************************/

ANSC_STATUS
SysRfoAcquireAccess
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysRfoReleaseAccess
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysRfoEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysRfoCancel
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysRfoDelete
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
SysRfoIssueKey
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulAccessMode
    );

ANSC_STATUS
SysRfoClearKey
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hKey
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN SYS_RFO_MANAGEMENT.C
***********************************************************/

ANSC_STATUS
SysRfoDelAllFolders
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysRfoDelAllRecords
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysRfoAddRfoIntoArray
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSubFolder
    );

ANSC_STATUS
SysRfoDelRfoFromArray
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSubFolder
    );

ANSC_STATUS
SysRfoCleanUpRfoArray
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysRfoAddRroIntoArray
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRecord
    );

ANSC_STATUS
SysRfoDelRroFromArray
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRecord
    );

ANSC_STATUS
SysRfoCleanUpRroArray
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN SYS_RFO_SORTING.C
***********************************************************/

ANSC_STATUS
SysRfoSortFolderByTime
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bAscending
    );

ANSC_STATUS
SysRfoSortFolderByName
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bAscending
    );

ANSC_STATUS
SysRfoSortRecordByTime
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bAscending
    );

ANSC_STATUS
SysRfoSortRecordByName
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bAscending
    );


/***********************************************************
           FUNCTIONS IMPLEMENTED IN SYS_RFO_ACCESS.C
***********************************************************/

char*
SysRfoQueryAbsPath
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pPathString
    );

BOOL
SysRfoIsRemovable
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bRecursive
    );

BOOL
SysRfoGrantAccess
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulAccessMode
    );

ULONG
SysRfoGetFolderCount
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
SysRfoGetRecordCount
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
SysRfoAddFolder
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pFolderName,
        ULONG                       ulPermission,
        ULONG                       ulFolderType,
        ANSC_HANDLE                 hRenderAttr,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
SysRfoDelFolder
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pFolderName,
        BOOL                        bRecursive,
        BOOL                        bForceful
    );

ANSC_HANDLE
SysRfoGetFolderByIndex
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex
    );

ANSC_HANDLE
SysRfoGetFolderByName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pFolderName
    );

ANSC_HANDLE
SysRfoAddRecord
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRecordName,
        ULONG                       ulPermission,
        ULONG                       ulRecordType,
        ANSC_HANDLE                 hRenderAttr,
        PVOID                       pBuffer,
        ULONG                       ulSize
    );

ANSC_STATUS
SysRfoDelRecord
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRecordName
    );

ANSC_HANDLE
SysRfoGetRecordByIndex
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex
    );

ANSC_HANDLE
SysRfoGetRecordByName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRecordName
    );

ANSC_STATUS
SysRfoSetRecord
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRecordName,
        ULONG                       ulRecordType,
        ANSC_HANDLE                 hRenderAttr,
        PVOID                       pBuffer,
        ULONG                       ulSize
    );

ANSC_STATUS
SysRfoGenerateContent
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysRfoPopulateContent
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysRfoOpenFirst
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysRfoCloseLast
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulFlags
    );


/***********************************************************
           FUNCTIONS IMPLEMENTED IN SYS_RFO_RCAIF.C
***********************************************************/

ANSC_STATUS
SysRfoRcaNotify
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFolder,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
SysRfoRcaReview
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFolder
    );


/***********************************************************
           FUNCTIONS IMPLEMENTED IN SYS_RFO_RCGIF.C
***********************************************************/

BOOL
SysRfoRcgNeedsUpdate
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFolder
    );

ANSC_STATUS
SysRfoRcgSynchronize
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFolder
    );


/***********************************************************
           FUNCTIONS IMPLEMENTED IN SYS_RFO_RCMIF.C
***********************************************************/

ANSC_STATUS
SysRfoRcmNotify
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFolder,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
SysRfoRcmInit
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFolder
    );

ANSC_STATUS
SysRfoRcmStop
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFolder
    );


#endif
