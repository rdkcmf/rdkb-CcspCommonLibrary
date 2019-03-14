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
        Ding Hua

    ---------------------------------------------------------------

    revision:

        01/30/06    initial revision.

**********************************************************************/


#ifndef  _SYS_IROV2_INTERNAL_API_
#define  _SYS_IROV2_INTERNAL_API_


/***********************************************************
           FUNCTIONS IMPLEMENTED IN SYS_IROV2_STATES.C
***********************************************************/

ANSC_HANDLE
SysIroV2GetIraIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysIroV2GetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
SysIroV2SetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
SysIroV2ResetProperty
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysIroV2Reset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN SYS_IROV2_OPERATION.C
***********************************************************/

ANSC_STATUS
SysIroV2Engage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysIroV2Cancel
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysIroV2AcqAccess
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bReadAccess
    );

ANSC_STATUS
SysIroV2RelAccess
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bReadAccess
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN SYS_IROV2_MANAGEMENT.C
***********************************************************/

ANSC_HANDLE
SysIroV2AddFolder
    (
        ANSC_HANDLE                 hThisObject,
        PSYS_IROV2_FOLDER_ENTRY     pCurFolder,
        char*                       pSubFolderName,
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
SysIroV2AddFolder2
    (
        ANSC_HANDLE                 hThisObject,
        PSYS_IROV2_FOLDER_ENTRY     pCurFolder,
        char*                       pSubFolderName,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
SysIroV2DelFolder
    (
        ANSC_HANDLE                 hThisObject,
        PSYS_IROV2_FOLDER_ENTRY     pCurFolder,
        char*                       pSubFolderName
    );

ANSC_STATUS
SysIroV2DelFolder2
    (
        ANSC_HANDLE                 hThisObject,
        PSYS_IROV2_FOLDER_ENTRY     pCurFolder
    );

ANSC_STATUS
SysIroV2ResetFolder
    (
        ANSC_HANDLE                 hThisObject,
        PSYS_IROV2_FOLDER_ENTRY     pCurFolder
    );

ANSC_HANDLE
SysIroV2GetFolderByName
    (
        ANSC_HANDLE                 hThisObject,
        PSYS_IROV2_FOLDER_ENTRY     pCurFolder,
        char*                       pSubFolderName
    );

ANSC_HANDLE
SysIroV2GetFolderByName2
    (
        ANSC_HANDLE                 hThisObject,
        PSYS_IROV2_FOLDER_ENTRY     pCurFolder,
        char*                       pSubFolderName
    );

ANSC_HANDLE
SysIroV2GetFolderByIndex
    (
        ANSC_HANDLE                 hThisObject,
        PSYS_IROV2_FOLDER_ENTRY     pCurFolder,
        ULONG                       ulIndex
    );

ANSC_STATUS
SysIroV2AddRecord
    (
        ANSC_HANDLE                 hThisObject,
        PSYS_IROV2_FOLDER_ENTRY     pCurFolder,
        char*                       pRecordName,
        ULONG                       ulRecordType,
        ULONG                       ulContentType,
        PVOID                       pWriteBuffer,
        ULONG                       ulWriteSize
    );

ANSC_STATUS
SysIroV2DelRecord
    (
        ANSC_HANDLE                 hThisObject,
        PSYS_IROV2_FOLDER_ENTRY     pCurFolder,
        char*                       pRecordName
    );

ANSC_STATUS
SysIroV2SetRecord
    (
        ANSC_HANDLE                 hThisObject,
        PSYS_IROV2_FOLDER_ENTRY     pCurFolder,
        char*                       pRecordName,
        ULONG                       ulRecordType,
        ULONG                       ulContentType,
        PVOID                       pWriteBuffer,
        ULONG                       ulWriteSize
    );

ANSC_HANDLE
SysIroV2GetRecordByName
    (
        ANSC_HANDLE                 hThisObject,
        PSYS_IROV2_FOLDER_ENTRY     pCurFolder,
        char*                       pRecordName
    );

ANSC_HANDLE
SysIroV2GetRecordByIndex
    (
        ANSC_HANDLE                 hThisObject,
        PSYS_IROV2_FOLDER_ENTRY     pCurFolder,
        ULONG                       ulIndex
    );

/***********************************************************
           FUNCTIONS IMPLEMENTED IN SYS_IROV2_IRAIF.C
***********************************************************/

ANSC_STATUS
SysIroV2IraAcqWriteAccess
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysIroV2IraRelWriteAccess
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
SysIroV2IraAddSysFolder
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pFolderName
    );

ANSC_HANDLE
SysIroV2IraAddFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pSubFolderName
    );

ANSC_HANDLE
SysIroV2IraAddFolder2
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
SysIroV2IraDelFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pSubFolderName,
        BOOL                        bRecursive
    );

ANSC_STATUS
SysIroV2IraDelFolder2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pSubFolderName,
        BOOL                        bRecursive,
        BOOL                        bForceful
    );

ANSC_HANDLE
SysIroV2IraOpenFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pSubFolderName
    );

ANSC_HANDLE
SysIroV2IraOpenFolder2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pSubFolderName,
        ULONG                       ulAccessMode
    );

ANSC_STATUS
SysIroV2IraCloseFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    );

ANSC_STATUS
SysIroV2IraClearFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    );

ANSC_STATUS
SysIroV2IraGenerateFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    );

ANSC_STATUS
SysIroV2IraPopulateFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    );

ANSC_STATUS
SysIroV2IraQueryFolder
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
SysIroV2IraGetSubFolderCount
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    );

ANSC_STATUS
SysIroV2IraEnumSubFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ULONG                       ulIndex,
        char*                       pSubFolderName,
        PULONG                      pulNameSize
    );

BOOL
SysIroV2IraTestSubFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pSubFolderName,
        ULONG                       ulAccessMode
    );

ANSC_STATUS
SysIroV2IraAddRecord
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName,
        ULONG                       ulRecordType,
        PVOID                       pWriteBuffer,
        ULONG                       ulWriteSize
    );

ANSC_STATUS
SysIroV2IraAddRecord2
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
SysIroV2IraDelRecord
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName
    );

ANSC_STATUS
SysIroV2IraGetRecord
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
SysIroV2IraSetRecord
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
SysIroV2IraQueryRecord
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
SysIroV2IraGetRecordCount
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    );

ANSC_STATUS
SysIroV2IraEnumRecord
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
SysIroV2IraTestRecord
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName,
        ULONG                       ulAccessMode
    );

ANSC_HANDLE
SysIroV2IraGetRfoUserContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    );

ANSC_STATUS
SysIroV2IraSetRfoUserContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ANSC_HANDLE                 hUserContext
    );

ULONG
SysIroV2IraGetRfoUserReserved
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    );

ANSC_STATUS
SysIroV2IraSetRfoUserReserved
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ULONG                       reserved
    );

ULONG
SysIroV2IraGetRfoFolderType
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    );

ANSC_STATUS
SysIroV2IraSetRfoFolderType
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ULONG                       ulFolderType
    );

ULONG
SysIroV2IraGetRfoPermission
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    );

ANSC_STATUS
SysIroV2IraSetRfoPermission
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ULONG                       ulPermission
    );

ANSC_HANDLE
SysIroV2IraGetRfoRenderAttr
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    );

ANSC_STATUS
SysIroV2IraSetRfoRenderAttr
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ANSC_HANDLE                 hRenderAttr
    );

ANSC_HANDLE
SysIroV2IraGetRroUserContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName
    );

ANSC_STATUS
SysIroV2IraSetRroUserContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName,
        ANSC_HANDLE                 hUserContext
    );

ULONG
SysIroV2IraGetRroUserReserved
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName
    );

ANSC_STATUS
SysIroV2IraSetRroUserReserved
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName,
        ULONG                       reserved
    );

ULONG
SysIroV2IraGetRroPermission
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName
    );

ANSC_STATUS
SysIroV2IraSetRroPermission
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName,
        ULONG                       ulPermission
    );

ANSC_HANDLE
SysIroV2IraGetRroRenderAttr
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName
    );

ANSC_STATUS
SysIroV2IraSetRroRenderAttr
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName,
        ANSC_HANDLE                 hRenderAttr
    );

ANSC_HANDLE
SysIroV2IraGetCurRecycleBin
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    );

ANSC_HANDLE
SysIroV2IraAttachRecycleBin
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ANSC_HANDLE                 hRecycleBin
    );

ANSC_HANDLE
SysIroV2IraDetachRecycleBin
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    );

ANSC_STATUS
SysIroV2IraSortSubFolders
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ULONG                       ulSortingType,
        BOOL                        bAscending
    );

ANSC_STATUS
SysIroV2IraSortRecords
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ULONG                       ulSortingType,
        BOOL                        bAscending
    );

ANSC_HANDLE
SysIroV2IraGetSysRamIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SysIroV2IraSetSysRamIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

#endif
