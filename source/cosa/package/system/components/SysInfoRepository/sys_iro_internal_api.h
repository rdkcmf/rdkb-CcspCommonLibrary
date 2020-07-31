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

    module:	sys_iro_internal_api.h

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Sys Info Repository
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/04/02    initial revision.

**********************************************************************/


#ifndef  _SYS_IRO_INTERNAL_API_
#define  _SYS_IRO_INTERNAL_API_


/***********************************************************
           FUNCTIONS IMPLEMENTED IN SYS_IRO_STATES.C
***********************************************************/

ANSC_HANDLE
SysIroGetIraIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
SysIroGetRepDriver
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysIroGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
SysIroSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
SysIroResetProperty
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysIroReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN SYS_IRO_OPERATION.C
***********************************************************/

ANSC_STATUS
SysIroEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysIroCancel
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysIroAcqAccess
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bReadAccess
    );

ANSC_STATUS
SysIroRelAccess
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bReadAccess
    );


/***********************************************************
           FUNCTIONS IMPLEMENTED IN SYS_IRO_IRAIF.C
***********************************************************/

ANSC_STATUS
SysIroIraAcqWriteAccess
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysIroIraRelWriteAccess
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysIroIraAcqThreadLock
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysIroIraRelThreadLock
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
SysIroIraAddSysFolder
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pFolderName
    );

ANSC_HANDLE
SysIroIraAddFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pSubFolderName
    );

ANSC_HANDLE
SysIroIraAddFolder2
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

ANSC_STATUS
SysIroIraDelFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pSubFolderName,
        BOOL                        bRecursive
    );

ANSC_STATUS
SysIroIraDelFolder2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pSubFolderName,
        BOOL                        bRecursive,
        BOOL                        bForceful
    );

ANSC_HANDLE
SysIroIraOpenFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pSubFolderName
    );

ANSC_HANDLE
SysIroIraOpenFolder2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pSubFolderName,
        ULONG                       ulAccessMode
    );

ANSC_STATUS
SysIroIraCloseFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    );

ANSC_STATUS
SysIroIraClearFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    );

ANSC_STATUS
SysIroIraGenerateFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    );

ANSC_STATUS
SysIroIraPopulateFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    );

ANSC_STATUS
SysIroIraQueryFolder
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

ULONG
SysIroIraGetSubFolderCount
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    );

ANSC_STATUS
SysIroIraEnumSubFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ULONG                       ulIndex,
        char*                       pSubFolderName,
        PULONG                      pulNameSize
    );

BOOL
SysIroIraTestSubFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pSubFolderName,
        ULONG                       ulAccessMode
    );

ANSC_STATUS
SysIroIraAddRecord
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName,
        ULONG                       ulRecordType,
        PVOID                       pWriteBuffer,
        ULONG                       ulWriteSize
    );

ANSC_STATUS
SysIroIraAddRecord2
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

ANSC_STATUS
SysIroIraDelRecord
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName
    );

ANSC_STATUS
SysIroIraGetRecord
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName,
        PULONG                      pulRecordType,
        PANSC_HANDLE                phRenderAttr,
        PVOID                       pReadBuffer,
        PULONG                      pulReadSize
    );

ANSC_STATUS
SysIroIraGetRecordByIndex
    (
        ANSC_HANDLE         hThisObject,
        ANSC_HANDLE         hCurFolder,
        ULONG               ulIndex,
        CHAR                *pName, /* caller allocate */
        ULONG               *pNameSize,
        ULONG               *pRecType,
        ULONG               *pContentType,
        ULONG               *ulAccess,
        UCHAR               **pData, /* caller free */
        ULONG               *pDataSize
    );

ANSC_STATUS
SysIroIraSetRecord
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName,
        ULONG                       ulRecordType,
        ANSC_HANDLE                 hRenderAttr,
        PVOID                       pWriteBuffer,
        ULONG                       ulWriteSize
    );

ANSC_STATUS
SysIroIraQueryRecord
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

ULONG
SysIroIraGetRecordCount
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    );

ANSC_STATUS
SysIroIraEnumRecord
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ULONG                       ulIndex,
        char*                       pRecordName,
        PULONG                      pulNameSize,
        PULONG                      pulRecordType,
        PULONG                      pulDataSize
    );

BOOL
SysIroIraTestRecord
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName,
        ULONG                       ulAccessMode
    );

ANSC_HANDLE
SysIroIraGetRfoUserContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    );

ANSC_STATUS
SysIroIraSetRfoUserContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ANSC_HANDLE                 hUserContext
    );

ULONG
SysIroIraGetRfoUserReserved
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    );

ANSC_STATUS
SysIroIraSetRfoUserReserved
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ULONG                       reserved
    );

ULONG
SysIroIraGetRfoFolderType
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    );

ANSC_STATUS
SysIroIraSetRfoFolderType
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ULONG                       ulFolderType
    );

ULONG
SysIroIraGetRfoPermission
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    );

ANSC_STATUS
SysIroIraSetRfoPermission
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ULONG                       ulPermission
    );

ANSC_HANDLE
SysIroIraGetRfoRenderAttr
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    );

ANSC_STATUS
SysIroIraSetRfoRenderAttr
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ANSC_HANDLE                 hRenderAttr
    );

ANSC_HANDLE
SysIroIraGetRroUserContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName
    );

ANSC_STATUS
SysIroIraSetRroUserContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName,
        ANSC_HANDLE                 hUserContext
    );

ULONG
SysIroIraGetRroUserReserved
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName
    );

ANSC_STATUS
SysIroIraSetRroUserReserved
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName,
        ULONG                       reserved
    );

ULONG
SysIroIraGetRroPermission
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName
    );

ANSC_STATUS
SysIroIraSetRroPermission
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName,
        ULONG                       ulPermission
    );

ANSC_HANDLE
SysIroIraGetRroRenderAttr
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName
    );

ANSC_STATUS
SysIroIraSetRroRenderAttr
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName,
        ANSC_HANDLE                 hRenderAttr
    );

ANSC_HANDLE
SysIroIraGetCurRecycleBin
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    );

ANSC_HANDLE
SysIroIraAttachRecycleBin
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ANSC_HANDLE                 hRecycleBin
    );

ANSC_HANDLE
SysIroIraDetachRecycleBin
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    );

ANSC_STATUS
SysIroIraSortSubFolders
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ULONG                       ulSortingType,
        BOOL                        bAscending
    );

ANSC_STATUS
SysIroIraSortRecords
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ULONG                       ulSortingType,
        BOOL                        bAscending
    );

ANSC_HANDLE
SysIroIraGetSysRamIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysIroIraSetSysRamIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

#endif
