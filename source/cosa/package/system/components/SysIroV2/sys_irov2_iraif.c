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

    module:	sys_iro_iraif.c

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced interface functions
        of the Sys Info Repository Object.

        *   SysIroV2IraAcqWriteAccess
        *   SysIroV2IraRelWriteAccess
        *   SysIroV2IraAddSysFolder
        *   SysIroV2IraAddFolder
        *   SysIroV2IraAddFolder2
        *   SysIroV2IraDelFolder
        *   SysIroV2IraDelFolder2
        *   SysIroV2IraOpenFolder
        *   SysIroV2IraOpenFolder2
        *   SysIroV2IraCloseFolder
        *   SysIroV2IraClearFolder
        *   SysIroV2IraGenerateFolder
        *   SysIroV2IraPopulateFolder
        *   SysIroV2IraQueryFolder
        *   SysIroV2IraGetSubFolderCount
        *   SysIroV2IraEnumSubFolder
        *   SysIroV2IraTestSubFolder
        *   SysIroV2IraAddRecord
        *   SysIroV2IraAddRecord2
        *   SysIroV2IraDelRecord
        *   SysIroV2IraGetRecord
        *   SysIroV2IraSetRecord
        *   SysIroV2IraQueryRecord
        *   SysIroV2IraGetRecordCount
        *   SysIroV2IraEnumRecord
        *   SysIroV2IraTestRecord
        *   SysIroV2IraGetRfoUserContext
        *   SysIroV2IraSetRfoUserContext
        *   SysIroV2IraGetRfoUserReserved
        *   SysIroV2IraSetRfoUserReserved
        *   SysIroV2IraGetRfoFolderType
        *   SysIroV2IraSetRfoFolderType
        *   SysIroV2IraGetRfoPermission
        *   SysIroV2IraSetRfoPermission
        *   SysIroV2IraGetRfoRenderAttr
        *   SysIroV2IraSetRfoRenderAttr
        *   SysIroV2IraGetRroUserContext
        *   SysIroV2IraSetRroUserContext
        *   SysIroV2IraGetRroUserReserved
        *   SysIroV2IraSetRroUserReserved
        *   SysIroV2IraGetRroPermission
        *   SysIroV2IraSetRroPermission
        *   SysIroV2IraGetRroRenderAttr
        *   SysIroV2IraSetRroRenderAttr
        *   SysIroV2IraGetCurRecycleBin
        *   SysIroV2IraAttachRecycleBin
        *   SysIroV2IraDetachRecycleBin
        *   SysIroV2IraSortSubFolders
        *   SysIroV2IraSortRecords
        *   SysIroV2IraGetSysRamIf
        *   SysIroV2IraSetSysRamIf

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


#include "sys_irov2_global.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroV2IraAcqWriteAccess
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to acquire the repository access.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysIroV2IraAcqWriteAccess
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;
    PSYS_INFO_REPOSITORY_PROPERTY   pProperty    = (PSYS_INFO_REPOSITORY_PROPERTY)&pMyObject->Property;
    ULONG                           i            = 0;

    AnscAcquireLock(&pMyObject->AccessLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroV2IraRelWriteAccess
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to release the repository access.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysIroV2IraRelWriteAccess
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;
    PSYS_INFO_REPOSITORY_PROPERTY   pProperty    = (PSYS_INFO_REPOSITORY_PROPERTY)&pMyObject->Property;
    ULONG                           i            = 0;

    AnscReleaseLock(&pMyObject->AccessLock);

    return  ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysIroV2IraAddSysFolder
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pFolderName
            );

    description:

        This function is called to create a repository folder.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pFolderName
                Specifies the folder name to be created, this parameter
                MUST not be NULL.

    return:     handle of the newly created folder.

**********************************************************************/

ANSC_HANDLE
SysIroV2IraAddSysFolder
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pFolderName
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;

    return
        (PSYS_RFO_KEY)pMyObject->IraAddFolder2
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)NULL,
                pFolderName,
                SYS_RFO_PERMISSION_STANDARD,
                SYS_RFO_ACCESS_MODE_ADAPTIVE,
                SYS_REP_FOLDER_TYPE_STORAGE,
                (ANSC_HANDLE)NULL,
                (ANSC_HANDLE)NULL
            );
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysIroV2IraAddFolder
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCurFolder,
                char*                       pSubFolderName
            );

    description:

        This function is called to create a repository folder.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder under which the new folder
                shall be created. This parameter could be NULL.

                char*                       pSubFolderName
                Specifies the folder name to be created, this parameter
                MUST not be NULL.

    return:     handle of the newly created folder.

**********************************************************************/

ANSC_HANDLE
SysIroV2IraAddFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pSubFolderName
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;

    return
        (PSYS_RFO_KEY)pMyObject->IraAddFolder2
            (
                (ANSC_HANDLE)pMyObject,
                hCurFolder,
                pSubFolderName,
                SYS_RFO_PERMISSION_ALL,
                SYS_RFO_ACCESS_MODE_ADAPTIVE,
                SYS_REP_FOLDER_TYPE_STORAGE,
                (ANSC_HANDLE)NULL,
                (ANSC_HANDLE)NULL
            );
}


/**********************************************************************

    caller:     owner of this object

    prototype:

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

    description:

        This function is called to create a repository folder.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder under which the new folder
                shall be created. This parameter could be NULL.

                char*                       pSubFolderName
                Specifies the folder name to be created, this parameter
                MUST not be NULL.

                ULONG                       ulPermission
                Specifies the access permission for the new folder.

                ULONG                       ulAccessMode
                Specifies the access mode for the new folder.

                ULONG                       ulFolderType
                Specifies the folder type to be created.

                ANSC_HANDLE                 hRenderAttr
                Specifies the render attribute associated with the
                folder to be created.

                ANSC_HANDLE                 hReserved
                Specifies a folder-specific context handle.

    return:     handle of the newly created folder.

**********************************************************************/

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
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;
    PSYS_INFO_REPOSITORY_PROPERTY   pProperty    = (PSYS_INFO_REPOSITORY_PROPERTY)&pMyObject->Property;
    PSYS_IROV2_FOLDER_ENTRY         pCurFolder   = (PSYS_IROV2_FOLDER_ENTRY      )hCurFolder;
    PSYS_RAM_INTERFACE              pSysRamIf    = (PSYS_RAM_INTERFACE           )pMyObject->hSysRamIf;
    PSYS_IROV2_FOLDER_ENTRY         pSubFolder   = NULL;

    if ( !pMyObject->bActive )
    {
        return  (ANSC_HANDLE)NULL;
    }

    if ( TRUE )
    {
        pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, FALSE);

        pSubFolder =
            (PSYS_IROV2_FOLDER_ENTRY)pMyObject->AddFolder
                (
                    (ANSC_HANDLE)pMyObject,
                    pCurFolder,
                    pSubFolderName,
                    hReserved
                );

        if ( pSubFolder )
        {
            SysIroV2ReferenceFolderEntry(pSubFolder);
            SysIroV2FolderReleaseAccess (pSubFolder);
        }

        pMyObject->RelAccess((ANSC_HANDLE)pMyObject, FALSE);
    }

    if ( pSysRamIf )
    {
        returnStatus =
            pSysRamIf->Notify
                (
                    pSysRamIf->hOwnerContext,
                    hCurFolder,
                    SYS_RAM_EVENT_folderAdded
                );
    }

    return  (ANSC_HANDLE)pSubFolder;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroV2IraDelFolder
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCurFolder,
                char*                       pSubFolderName,
                BOOL                        bRecursive
            );

    description:

        This function is called to delete a repository folder.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder under which the specified
                folder shall be deleted. This parameter could be NULL.

                char*                       pSubFolderName
                Specifies the folder name to be deleted, this parameter
                MUST not be NULL.

                BOOL                        bRecursive
                Specifies whether the operation should be carried out
                recursively.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysIroV2IraDelFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pSubFolderName,
        BOOL                        bRecursive
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;
    PSYS_IROV2_FOLDER_ENTRY         pCurFolder   = (PSYS_IROV2_FOLDER_ENTRY      )hCurFolder;

    returnStatus =
        pMyObject->IraDelFolder2
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pCurFolder,
                pSubFolderName,
                bRecursive,
                FALSE
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroV2IraDelFolder2
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCurFolder,
                char*                       pSubFolderName,
                BOOL                        bRecursive,
                BOOL                        bForceful
            );

    description:

        This function is called to delete a repository folder
        regardless the permission associated this folder, all the sub-
        folders and individual records if bForceful is TRUE.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder under which the specified
                folder shall be deleted. This parameter could be NULL.

                char*                       pSubFolderName
                Specifies the folder name to be deleted, this parameter
                MUST not be NULL.

                BOOL                        bRecursive
                Specifies whether the operation should be carried out
                recursively.

                BOOL                        bForceful
                Specifies whether the operation should be forced even
                if it's not permitted by current permission.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysIroV2IraDelFolder2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pSubFolderName,
        BOOL                        bRecursive,
        BOOL                        bForceful
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;
    PSYS_IROV2_FOLDER_ENTRY         pCurFolder   = (PSYS_IROV2_FOLDER_ENTRY      )hCurFolder;
    PSYS_RAM_INTERFACE              pSysRamIf    = (PSYS_RAM_INTERFACE           )pMyObject->hSysRamIf;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }

    if ( TRUE )
    {
        pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, FALSE);

        returnStatus =
            pMyObject->DelFolder
                (
                    (ANSC_HANDLE)pMyObject,
                    pCurFolder,
                    pSubFolderName
                );

        pMyObject->RelAccess((ANSC_HANDLE)pMyObject, FALSE);
    }

    if ( pSysRamIf )
    {
        returnStatus =
            pSysRamIf->Notify
                (
                    pSysRamIf->hOwnerContext,
                    hCurFolder,
                    SYS_RAM_EVENT_folderDeleted
                );
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysIroV2IraOpenFolder
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCurFolder,
                char*                       pSubFolderName
            );

    description:

        This function is called to open a repository folder.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder under which the new folder
                shall be opened. This parameter could be NULL.

                char*                       pSubFolderName
                Specifies the folder name to be opened, this parameter
                MUST not be NULL.

    return:     handle of the newly opened folder.

**********************************************************************/

ANSC_HANDLE
SysIroV2IraOpenFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pSubFolderName
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;

    return
        (PSYS_RFO_KEY)pMyObject->IraOpenFolder2
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)hCurFolder,
                pSubFolderName,
                SYS_RFO_ACCESS_MODE_ADAPTIVE
            );
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysIroV2IraOpenFolder2
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCurFolder,
                char*                       pSubFolderName,
                ULONG                       ulAccessMode
            );

    description:

        This function is called to open a repository folder.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder under which the new folder
                shall be opened. This parameter could be NULL.

                char*                       pSubFolderName
                Specifies the folder name to be opened, this parameter
                MUST not be NULL.

                ULONG                       ulAccessMode
                Specifies the access mode for the new folder.

    return:     handle of the newly opened folder.

**********************************************************************/

ANSC_HANDLE
SysIroV2IraOpenFolder2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pSubFolderName,
        ULONG                       ulAccessMode
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;
    PSYS_IROV2_FOLDER_ENTRY         pCurFolder   = (PSYS_IROV2_FOLDER_ENTRY      )hCurFolder;
    PSYS_IROV2_FOLDER_ENTRY         pSubFolder   = NULL;

    if ( !pMyObject->bActive )
    {
        return  (ANSC_HANDLE)NULL;
    }

    if ( TRUE )
    {
        pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, TRUE);

        pSubFolder =
            (PSYS_IROV2_FOLDER_ENTRY)pMyObject->GetFolderByName
                (
                    (ANSC_HANDLE)pMyObject,
                    pCurFolder,
                    pSubFolderName
                );

        if ( pSubFolder )
        {
            SysIroV2ReferenceFolderEntry(pSubFolder);
            SysIroV2FolderReleaseAccess (pSubFolder);
        }

        pMyObject->RelAccess((ANSC_HANDLE)pMyObject, TRUE);
    }

    return  (ANSC_HANDLE)pSubFolder;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroV2IraCloseFolder
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCurFolder
            );

    description:

        This function is called to close a repository folder.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder to be closed, this
                parameter MUST not be NULL.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysIroV2IraCloseFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;
    PSYS_IROV2_FOLDER_ENTRY         pCurFolder   = (PSYS_IROV2_FOLDER_ENTRY      )hCurFolder;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, TRUE);

    if ( pCurFolder )
    {
        SysIroV2FolderAcquireAccess   (pCurFolder);
        SysIroV2DereferenceFolderEntry(pCurFolder);
        SysIroV2FolderReleaseAccess   (pCurFolder);
    }

    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, TRUE);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroV2IraClearFolder
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCurFolder
            );

    description:

        This function is called to clear a repository folder.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder to be cleared, this
                parameter MUST not be NULL.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysIroV2IraClearFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;
    PSYS_INFO_REPOSITORY_PROPERTY   pProperty    = (PSYS_INFO_REPOSITORY_PROPERTY)&pMyObject->Property;
    PSYS_IROV2_FOLDER_ENTRY         pCurFolder   = (PSYS_IROV2_FOLDER_ENTRY      )hCurFolder;
    PSYS_RAM_INTERFACE              pSysRamIf    = (PSYS_RAM_INTERFACE           )pMyObject->hSysRamIf;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, FALSE);

    if ( pCurFolder )
    {
        SysIroV2FolderAcquireAccess(pCurFolder);
        pMyObject->ResetFolder     ((ANSC_HANDLE)pMyObject, pCurFolder);
        SysIroV2FolderReleaseAccess(pCurFolder);
    }

    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, FALSE);

    if ( pSysRamIf )
    {
        returnStatus =
            pSysRamIf->Notify
                (
                    pSysRamIf->hOwnerContext,
                    hCurFolder,
                    SYS_RAM_EVENT_folderCleared
                );
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroV2IraGenerateFolder
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCurFolder
            );

    description:

        This function is called to generate folder content.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder whose content to be
                generated, this parameter MUST not be NULL.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysIroV2IraGenerateFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;
    PSYS_RAM_INTERFACE              pSysRamIf    = (PSYS_RAM_INTERFACE           )pMyObject->hSysRamIf;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else
    {
        AnscTrace("*******************************************************\n");
        AnscTrace("** SysIroV2IraGenerateFolder should not be called!!! **\n");
        AnscTrace("*******************************************************\n");
    }

    if ( pSysRamIf )
    {
        returnStatus =
            pSysRamIf->Notify
                (
                    pSysRamIf->hOwnerContext,
                    hCurFolder,
                    SYS_RAM_EVENT_folderUpdated
                );
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroV2IraPopulateFolder
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCurFolder
            );

    description:

        This function is called to populate folder content.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder whose content to be
                populated, this parameter MUST not be NULL.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysIroV2IraPopulateFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;
    PSYS_RAM_INTERFACE              pSysRamIf    = (PSYS_RAM_INTERFACE           )pMyObject->hSysRamIf;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else
    {
        AnscTrace("*******************************************************\n");
        AnscTrace("** SysIroV2IraPopulateFolder should not be called!!! **\n");
        AnscTrace("*******************************************************\n");
    }

    if ( pSysRamIf )
    {
        returnStatus =
            pSysRamIf->Notify
                (
                    pSysRamIf->hOwnerContext,
                    hCurFolder,
                    SYS_RAM_EVENT_folderUpdated
                );
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

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

    description:

        This function is called to query a repository folder.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder to be queried, this
                parameter MUST not be NULL.

                PULONG                      pulTimestamp
                Returns the time at which the folder was created or
                last modified.

                PULONG                      pulPermission
                Returns the repository folder permission.

                PULONG                      pulFolderType
                Returns the repository folder type.

                PULONG                      pulSubFolderCount
                Returns the number of sub-folder objects.

                PULONG                      pulRecordCount
                Returns the number of repository record objects.

                PULONG                      pulContentType
                Returns the content type of the repository folder.

                PANSC_HANDLE                phRenderAttr
                Returns the associated render attribute.

    return:     status of operation.

**********************************************************************/

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
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;
    PSYS_IROV2_FOLDER_ENTRY         pCurFolder   = (PSYS_IROV2_FOLDER_ENTRY      )hCurFolder;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else if ( !pCurFolder )
    {
        return  ANSC_STATUS_INVALID_HANDLE;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, TRUE);
    SysIroV2FolderAcquireAccess(pCurFolder);

    if ( pulTimestamp )
    {
        AnscTrace("*******************************************************\n");
        AnscTrace("** SysIroV2IraQueryFolder -- pulTimestamp is set!!!  **\n");
        AnscTrace("*******************************************************\n");
    }

    if ( pulPermission )
    {
        *pulPermission = SYS_RFO_ACCESS_MODE_ALL;
    }

    if ( pulFolderType )
    {
        AnscTrace("*******************************************************\n");
        AnscTrace("** SysIroV2IraQueryFolder -- pulFolderType is set!!! **\n");
        AnscTrace("*******************************************************\n");
    }

    if ( pulSubFolderCount )
    {
        *pulSubFolderCount = AnscQueueQueryDepth(&pCurFolder->SubFolderQueue);
    }

    if ( pulRecordCount )
    {
        *pulRecordCount = AnscQueueQueryDepth(&pCurFolder->RecordQueue);
    }

    if ( pulContentType )
    {
        *pulContentType = SYS_FOLDER_CONTENT_DEFAULT;
    }

    if ( phRenderAttr )
    {
        AnscTrace("*******************************************************\n");
        AnscTrace("** SysIroV2IraQueryFolder -- phRenderAttr is set!!!  **\n");
        AnscTrace("*******************************************************\n");
    }

    SysIroV2FolderReleaseAccess(pCurFolder);
    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, TRUE);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SysIroV2IraGetSubFolderCount
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCurFolder
            );

    description:

        This function is called to retrieve the number of sub-folders
        under the specified folder.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder to be enumerated.

    return:     number of sub-folders.

**********************************************************************/

ULONG
SysIroV2IraGetSubFolderCount
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject     = (PSYS_IROV2_OBJECT            )hThisObject;
    PSYS_IROV2_FOLDER_ENTRY         pCurFolder    = (PSYS_IROV2_FOLDER_ENTRY      )hCurFolder;
    ULONG                           ulFolderCount = 0;

    if ( !pMyObject->bActive )
    {
        return  0;
    }
    else if ( !pCurFolder )
    {
        return  0;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, TRUE);
    SysIroV2FolderAcquireAccess(pCurFolder);

    ulFolderCount = AnscQueueQueryDepth(&pCurFolder->SubFolderQueue);

    SysIroV2FolderReleaseAccess(pCurFolder);
    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, TRUE);

    return  ulFolderCount;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroV2IraEnumSubFolder
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCurFolder,
                ULONG                       ulIndex,
                char*                       pSubFolderName,
                PULONG                      pulNameSize
            );

    description:

        This function is called to enumerate a sub-folder identified
        by its index.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder to be enumerated.

                ULONG                       ulIndex
                Specifies the index of the sub-folder to be enumerated.

                char*                       pSubFolderName
                Returns the name of the specified sub-folder.

                PULONG                      pulNameSize
                Returns the size of the sub-folder name.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysIroV2IraEnumSubFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ULONG                       ulIndex,
        char*                       pSubFolderName,
        PULONG                      pulNameSize
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;
    PSYS_IROV2_FOLDER_ENTRY         pCurFolder   = (PSYS_IROV2_FOLDER_ENTRY      )hCurFolder;
    PSYS_IROV2_FOLDER_ENTRY         pSubFolder   = NULL;
    errno_t   rc   = -1;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else if ( !pCurFolder )
    {
        return  ANSC_STATUS_INVALID_HANDLE;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, TRUE);

    pSubFolder =
        (PSYS_IROV2_FOLDER_ENTRY)pMyObject->GetFolderByIndex
            (
                (ANSC_HANDLE)pMyObject,
                pCurFolder,
                ulIndex
            );

    if ( !pSubFolder )
    {
        returnStatus = ANSC_STATUS_CANT_FIND;

        goto  EXIT1;
    }

    if ( *pulNameSize < AnscSizeOfString(pSubFolder->FolderName) )
    {
        *pulNameSize = AnscSizeOfString(pSubFolder->FolderName);

        returnStatus = ANSC_STATUS_BAD_SIZE;
    }
    else
    {

        rc = strcpy_s(pSubFolderName, *pulNameSize, pSubFolder->FolderName);
        ERR_CHK(rc);

         *pulNameSize = AnscSizeOfString(pSubFolder->FolderName);   
    }

    SysIroV2FolderReleaseAccess(pSubFolder);

    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, TRUE);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SysIroV2IraTestSubFolder
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCurFolder,
                char*                       pSubFolderName,
                ULONG                       ulAccessMode
            );

    description:

        This function is called to test a repository folder.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder under which the new folder
                shall be opened. This parameter could be NULL.

                char*                       pSubFolderName
                Specifies the folder name to be opened, this parameter
                MUST not be NULL.

                ULONG                       ulAccessMode
                Specifies the access mode for the new folder.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
SysIroV2IraTestSubFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pSubFolderName,
        ULONG                       ulAccessMode
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;

    if ( !pMyObject->bActive )
    {
        return  FALSE;
    }
    else
    {
        AnscTrace("*******************************************************\n");
        AnscTrace("** SysIroV2IraTestSubFolder is called!!!             **\n");
        AnscTrace("*******************************************************\n");
    }

    return  FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

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

    description:

        This function is called to add a repository record.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder under which the new record
                shall be added. This parameter MUST not be NULL.

                char*                       pRecordName
                Specifies the record name to be added, this parameter
                MUST not be NULL.

                ULONG                       ulRecordType
                Specifies the record type to be created.

                PVOID                       pWriteBuffer
                Specifies a record data value to be saved.

                ULONG                       ulWriteSize
                Specifies the size of the record data buffer.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysIroV2IraAddRecord
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName,
        ULONG                       ulRecordType,
        PVOID                       pWriteBuffer,
        ULONG                       ulWriteSize
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;

    returnStatus =
        pMyObject->IraAddRecord2
            (
                (ANSC_HANDLE)pMyObject,
                hCurFolder,
                pRecordName,
                SYS_RRO_PERMISSION_ALL,
                ulRecordType,
                (ANSC_HANDLE)NULL,
                pWriteBuffer,
                ulWriteSize
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

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

    description:

        This function is called to add a repository record.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder under which the new record
                shall be added. This parameter MUST not be NULL.

                char*                       pRecordName
                Specifies the record name to be added, this parameter
                MUST not be NULL.

                ULONG                       ulPermission
                Specifies the record permission to be created.

                ULONG                       ulRecordType
                Specifies the record type to be created.

                ANSC_HANDLE                 hRenderAttr
                Specifies the render attribute associated with the
                record to be added.

                PVOID                       pWriteBuffer
                Specifies a record data value to be saved.

                ULONG                       ulWriteSize
                Specifies the size of the record data buffer.

    return:     status of operation.

**********************************************************************/

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
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;
    PSYS_INFO_REPOSITORY_PROPERTY   pProperty    = (PSYS_INFO_REPOSITORY_PROPERTY)&pMyObject->Property;
    PSYS_IROV2_FOLDER_ENTRY         pCurFolder   = (PSYS_IROV2_FOLDER_ENTRY      )hCurFolder;
    PSYS_RAM_INTERFACE              pSysRamIf    = (PSYS_RAM_INTERFACE           )pMyObject->hSysRamIf;
    PSYS_RRO_RENDER_ATTR            pRenderAttr  = (PSYS_RRO_RENDER_ATTR         )hRenderAttr;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else if ( !pCurFolder )
    {
        return  ANSC_STATUS_INVALID_HANDLE;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, FALSE);

    returnStatus =
        pMyObject->AddRecord
            (
                (ANSC_HANDLE)pMyObject,
                pCurFolder,
                pRecordName,
                ulRecordType,
                pRenderAttr ? pRenderAttr->ContentType : SYS_RECORD_CONTENT_DEFAULT,
                pWriteBuffer,
                ulWriteSize
            );

    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, FALSE);

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        return  returnStatus;
    }

    if ( pSysRamIf )
    {
        returnStatus =
            pSysRamIf->Notify
                (
                    pSysRamIf->hOwnerContext,
                    hCurFolder,
                    SYS_RAM_EVENT_recordAdded
                );
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroV2IraDelRecord
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCurFolder,
                char*                       pRecordName
            );

    description:

        This function is called to delete a repository record.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder under which the specified
                record shall be deleted. This parameter MUST not be
                NULL.

                char*                       pRecordName
                Specifies the record name to be deleted, this parameter
                MUST not be NULL.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysIroV2IraDelRecord
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;
    PSYS_INFO_REPOSITORY_PROPERTY   pProperty    = (PSYS_INFO_REPOSITORY_PROPERTY)&pMyObject->Property;
    PSYS_IROV2_FOLDER_ENTRY         pCurFolder   = (PSYS_IROV2_FOLDER_ENTRY      )hCurFolder;
    PSYS_RAM_INTERFACE              pSysRamIf    = (PSYS_RAM_INTERFACE           )pMyObject->hSysRamIf;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else if ( !pCurFolder )
    {
        return  ANSC_STATUS_INVALID_HANDLE;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, FALSE);

    returnStatus =
        pMyObject->DelRecord
            (
                (ANSC_HANDLE)pMyObject,
                pCurFolder,
                pRecordName
            );

    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, FALSE);

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        return  returnStatus;
    }

    if ( pSysRamIf )
    {
        returnStatus =
            pSysRamIf->Notify
                (
                    pSysRamIf->hOwnerContext,
                    hCurFolder,
                    SYS_RAM_EVENT_recordDeleted
                );
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

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

    description:

        This function is called to retrieve a repository record.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder under which the specified
                record shall be retrieved. This parameter MUST not be
                NULL.

                char*                       pRecordName
                Specifies the record name to be retrieved, this
                parameter MUST not be NULL.

                PULONG                      pulRecordType
                Returns the record type to be retrieved.

                PANSC_HANDLE                phRenderAttr
                Returns the render attribute associated with the record
                to be retrieved.

                PVOID                       pReadBuffer
                Specifies the data buffer where the record data value
                shall be placed.

                PULONG                      pulReadSize
                Specifies/returns the size of the record data buffer.

    return:     status of operation.

**********************************************************************/

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
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;
    PSYS_INFO_REPOSITORY_PROPERTY   pProperty    = (PSYS_INFO_REPOSITORY_PROPERTY)&pMyObject->Property;
    PSYS_IROV2_FOLDER_ENTRY         pCurFolder   = (PSYS_IROV2_FOLDER_ENTRY      )hCurFolder;
    PSYS_IROV2_RECORD_ENTRY         pCurRecord   = NULL;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else if ( !pCurFolder )
    {
        return  ANSC_STATUS_INVALID_HANDLE;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, TRUE);

    pCurRecord =
        (PSYS_IROV2_RECORD_ENTRY)pMyObject->GetRecordByName
            (
                (ANSC_HANDLE)pMyObject,
                pCurFolder,
                pRecordName
            );

    if ( !pCurRecord )
    {
        returnStatus = ANSC_STATUS_CANT_FIND;

        goto  EXIT1;
    }

    if ( pulRecordType )
    {
        *pulRecordType = pCurRecord->RecordType;
    }

    if ( phRenderAttr )
    {
        AnscTrace("*******************************************************\n");
        AnscTrace("** SysIroV2IraGetRecord   -- phRenderAttr is set!!!  **\n");
        AnscTrace("*******************************************************\n");
    }

    if ( !pReadBuffer || (*pulReadSize < pCurRecord->ulDataSize) )
    {
        *pulReadSize = pCurRecord->ulDataSize;

        returnStatus = ANSC_STATUS_BAD_SIZE;

        goto  EXIT1;
    }
    else
    {
        *pulReadSize = pCurRecord->ulDataSize;

        _ansc_memcpy(pReadBuffer, pCurRecord->DataAndName, pCurRecord->ulDataSize);
    }

    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, TRUE);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

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

    description:

        This function is called to configure a repository record.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder under which the new record
                shall be added. This parameter MUST not be NULL.

                char*                       pRecordName
                Specifies the record name to be added, this parameter
                MUST not be NULL.

                ULONG                       ulRecordType
                Specifies the record type to be created.

                ANSC_HANDLE                 hRenderAttr
                Specifies the render attribute associated with the
                record to be configured.

                PVOID                       pWriteBuffer
                Specifies a record data value to be saved.

                ULONG                       ulWriteSize
                Specifies the size of the record data buffer.

    return:     status of operation.

**********************************************************************/

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
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;
    PSYS_INFO_REPOSITORY_PROPERTY   pProperty    = (PSYS_INFO_REPOSITORY_PROPERTY)&pMyObject->Property;
    PSYS_RAM_INTERFACE              pSysRamIf    = (PSYS_RAM_INTERFACE           )pMyObject->hSysRamIf;
    PSYS_IROV2_FOLDER_ENTRY         pCurFolder   = (PSYS_IROV2_FOLDER_ENTRY      )hCurFolder;
    PSYS_IROV2_RECORD_ENTRY         pCurRecord   = NULL;
    PSYS_RRO_RENDER_ATTR            pRenderAttr  = (PSYS_RRO_RENDER_ATTR         )hRenderAttr;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else if ( !pCurFolder )
    {
        return  ANSC_STATUS_INVALID_HANDLE;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, FALSE);

    returnStatus =
        pMyObject->SetRecord
            (
                (ANSC_HANDLE)pMyObject,
                pCurFolder,
                pRecordName,
                ulRecordType,
                pRenderAttr ? pRenderAttr->ContentType : SYS_RECORD_CONTENT_DEFAULT,
                pWriteBuffer,
                ulWriteSize
            );

    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, FALSE);

    if ( pSysRamIf )
    {
        returnStatus =
            pSysRamIf->Notify
                (
                    pSysRamIf->hOwnerContext,
                    hCurFolder,
                    SYS_RAM_EVENT_recordUpdated
                );
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

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

    description:

        This function is called to query a repository record.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder to be queried, this
                parameter MUST not be NULL.

                char*                       pRecordName
                Specifies the record name to be queried, this parameter
                MUST not be NULL.

                PULONG                      pulTimestamp
                Returns the time at which the folder was created or
                last modified.

                PULONG                      pulPermission
                Returns the repository record permission.

                PULONG                      pulRecordType
                Returns the repository record type.

                PULONG                      pulRecordSize
                Returns the size of the repository record.

                PULONG                      pulContentType
                Returns the content type of the repository record.

                PANSC_HANDLE                phRenderAttr
                Returns the associated render attribute.

    return:     status of operation.

**********************************************************************/

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
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;
    PSYS_INFO_REPOSITORY_PROPERTY   pProperty    = (PSYS_INFO_REPOSITORY_PROPERTY)&pMyObject->Property;
    PSYS_IROV2_FOLDER_ENTRY         pCurFolder   = (PSYS_IROV2_FOLDER_ENTRY      )hCurFolder;
    PSYS_IROV2_RECORD_ENTRY         pCurRecord   = NULL;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else if ( !pCurFolder )
    {
        return  ANSC_STATUS_INVALID_HANDLE;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, TRUE);

    pCurRecord =
        (PSYS_IROV2_RECORD_ENTRY)pMyObject->GetRecordByName
            (
                (ANSC_HANDLE)pMyObject,
                pCurFolder,
                pRecordName
            );

    if ( !pCurRecord )
    {
        returnStatus = ANSC_STATUS_CANT_FIND;

        goto  EXIT1;
    }

    if ( pulTimestamp )
    {
        AnscTrace("*******************************************************\n");
        AnscTrace("** SysIroV2IraQueryRecord -- pulTimestamp is set!!!  **\n");
        AnscTrace("*******************************************************\n");
    }

    if ( pulPermission )
    {
        *pulPermission = SYS_RRO_ACCESS_MODE_ALL;
    }

    if ( pulRecordType )
    {
        *pulRecordType = pCurRecord->RecordType;
    }

    if ( pulRecordSize )
    {
        *pulRecordSize = pCurRecord->ulDataSize;
    }

    if ( pulContentType )
    {
        *pulContentType = pCurRecord->ContentType;
    }

    if ( phRenderAttr )
    {
        AnscTrace("*******************************************************\n");
        AnscTrace("** SysIroV2IraQueryRecord -- phRenderAttr is set!!!  **\n");
        AnscTrace("*******************************************************\n");
    }

    returnStatus = ANSC_STATUS_SUCCESS;

    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, TRUE);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SysIroV2IraGetRecordCount
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCurFolder
            );

    description:

        This function is called to retrieve the number of records
        under the specified folder.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder to be enumerated.

    return:     number of records.

**********************************************************************/

ULONG
SysIroV2IraGetRecordCount
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject     = (PSYS_IROV2_OBJECT            )hThisObject;
    PSYS_INFO_REPOSITORY_PROPERTY   pProperty     = (PSYS_INFO_REPOSITORY_PROPERTY)&pMyObject->Property;
    PSYS_IROV2_FOLDER_ENTRY         pCurFolder    = (PSYS_IROV2_FOLDER_ENTRY      )hCurFolder;
    ULONG                           ulRecordCount = 0;

    if ( !pMyObject->bActive )
    {
        return  0;
    }
    else if ( !pCurFolder )
    {
        return  0;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, TRUE);
    ulRecordCount = AnscQueueQueryDepth(&pCurFolder->RecordQueue);
    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, TRUE);

    return  ulRecordCount;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroV2IraEnumRecord
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCurFolder,
                ULONG                       ulIndex,
                char*                       pRecordName,
                PULONG                      pulNameSize,
                PULONG                      pulRecordType
            );

    description:

        This function is called to enumerate a record identified
        by its index.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder to be enumerated.

                ULONG                       ulIndex
                Specifies the index of the record to be enumerated.

                char*                       pRecordName
                Returns the name of the specified record.

                PULONG                      pulNameSize
                Returns the size of the record name.

                PULONG                      pulRecordType
                Returns the type of the sspecified record.

                PULONG                      pulDataSize
                Returns the size of the record data value.

    return:     status of operation.

**********************************************************************/

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
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject      = (PSYS_IROV2_OBJECT            )hThisObject;
    PSYS_INFO_REPOSITORY_PROPERTY   pProperty      = (PSYS_INFO_REPOSITORY_PROPERTY)&pMyObject->Property;
    PSYS_IROV2_FOLDER_ENTRY         pCurFolder     = (PSYS_IROV2_FOLDER_ENTRY      )hCurFolder;
    PSYS_IROV2_RECORD_ENTRY         pCurRecord     = NULL;
    errno_t    rc   = -1;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else if ( !pCurFolder )
    {
        return  ANSC_STATUS_INVALID_HANDLE;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, TRUE);

    pCurRecord =
        (PSYS_IROV2_RECORD_ENTRY)pMyObject->GetRecordByIndex
            (
                (ANSC_HANDLE)pMyObject,
                pCurFolder,
                ulIndex
            );

    if ( !pCurRecord )
    {
        returnStatus = ANSC_STATUS_CANT_FIND;

        goto  EXIT1;
    }

    if ( *pulNameSize < AnscSizeOfString(pCurRecord->pRecordName) )
    {
        *pulNameSize = AnscSizeOfString(pCurRecord->pRecordName);

        returnStatus = ANSC_STATUS_BAD_SIZE;

        goto  EXIT1;
    }
    else
    {

        if ( pulRecordType )
        {
            *pulRecordType = pCurRecord->RecordType;
        }

        if ( pulDataSize )
        {
            *pulDataSize = pCurRecord->ulDataSize;
        }

        rc = strcpy_s(pRecordName, *pulNameSize, pCurRecord->pRecordName);
        ERR_CHK(rc);

        *pulNameSize = AnscSizeOfString(pCurRecord->pRecordName);
    }

    returnStatus = ANSC_STATUS_SUCCESS;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, TRUE);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SysIroV2IraTestRecord
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCurFolder,
                char*                       pRecordName,
                ULONG                       ulAccessMode
            );

    description:

        This function is called to test a repository record.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder under which the new record
                shall be added. This parameter MUST not be NULL.

                char*                       pRecordName
                Specifies the record name to be added, this parameter
                MUST not be NULL.

                ULONG                       ulAccessMode
                Specifies the access mode should be granted.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
SysIroV2IraTestRecord
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName,
        ULONG                       ulAccessMode
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;

    AnscTrace("*******************************************************\n");
    AnscTrace("** SysIroV2IraTestRecord is called!!!                **\n");
    AnscTrace("*******************************************************\n");

    return  FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysIroV2IraGetRfoUserContext
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCurFolder
            );

    description:

        This function is called to retrieve certain property associated
        with a repository folder.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder on which the operation
                should be carried out.

    return:     property value.

**********************************************************************/

ANSC_HANDLE
SysIroV2IraGetRfoUserContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;
    PSYS_IROV2_FOLDER_ENTRY         pCurFolder   = (PSYS_IROV2_FOLDER_ENTRY      )hCurFolder;
    ANSC_HANDLE                     hUserContext = NULL;

    if ( !pMyObject->bActive )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else if ( !pCurFolder )
    {
        return  (ANSC_HANDLE)NULL;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, TRUE);
    hUserContext = pCurFolder->hUserContext;
    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, TRUE);

    return  hUserContext;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroV2IraSetRfoUserContext
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCurFolder,
                ANSC_HANDLE                 hUserContext
            );

    description:

        This function is called to modify certain property associated
        with a repository folder.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder on which the operation
                should be carried out.

                ANSC_HANDLE                 hUserContext
                Specifies the property value to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysIroV2IraSetRfoUserContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ANSC_HANDLE                 hUserContext
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;
    PSYS_IROV2_FOLDER_ENTRY         pCurFolder   = (PSYS_IROV2_FOLDER_ENTRY      )hCurFolder;
    PSYS_RAM_INTERFACE              pSysRamIf    = (PSYS_RAM_INTERFACE           )pMyObject->hSysRamIf;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else if ( !pCurFolder )
    {
        return  ANSC_STATUS_INVALID_HANDLE;
    }
    else
    {
        AnscTrace("SysIroV2IraSetRfoUserContext -- %s...\n", pCurFolder->FolderName);
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, FALSE);

    pCurFolder->hUserContext = hUserContext;

    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, FALSE);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SysIroV2IraGetRfoUserReserved
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCurFolder
            );

    description:

        This function is called to retrieve certain property associated
        with a repository folder.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder on which the operation
                should be carried out.

    return:     property value.

**********************************************************************/

ULONG
SysIroV2IraGetRfoUserReserved
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;
    PSYS_INFO_REPOSITORY_PROPERTY   pProperty    = (PSYS_INFO_REPOSITORY_PROPERTY)&pMyObject->Property;
    PSYS_IROV2_FOLDER_ENTRY         pCurFolder   = (PSYS_IROV2_FOLDER_ENTRY      )hCurFolder;
    ULONG                           ulReserved   = 0;

    if ( !pMyObject->bActive )
    {
        return  0;
    }
    else if ( !pCurFolder )
    {
        return  0;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, TRUE);
    ulReserved = pCurFolder->UserReserved;
    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, TRUE);

    return  ulReserved;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroV2IraSetRfoUserReserved
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCurFolder,
                ULONG                       reserved
            );

    description:

        This function is called to modify certain property associated
        with a repository folder.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder on which the operation
                should be carried out.

                ULONG                       reserved
                Specifies the property value to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysIroV2IraSetRfoUserReserved
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ULONG                       reserved
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;
    PSYS_IROV2_FOLDER_ENTRY         pCurFolder   = (PSYS_IROV2_FOLDER_ENTRY      )hCurFolder;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else if ( !pCurFolder )
    {
        return  ANSC_STATUS_INVALID_HANDLE;
    }
    else
    {
        AnscTrace("SysIroV2IraSetRfoUserReserved -- %s...\n", pCurFolder->FolderName);
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, FALSE);

    pCurFolder->UserReserved = reserved;

    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, FALSE);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SysIroV2IraGetRfoFolderType
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCurFolder
            );

    description:

        This function is called to retrieve certain property associated
        with a repository folder.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder on which the operation
                should be carried out.

    return:     property value.

**********************************************************************/

ULONG
SysIroV2IraGetRfoFolderType
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;

    return  SYS_REP_FOLDER_TYPE_STORAGE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroV2IraSetRfoFolderType
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCurFolder,
                ULONG                       ulFolderType
            );

    description:

        This function is called to modify certain property associated
        with a repository folder.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder on which the operation
                should be carried out.

                ULONG                       ulFolderType
                Specifies the property value to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysIroV2IraSetRfoFolderType
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ULONG                       ulFolderType
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SysIroV2IraGetRfoPermission
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCurFolder
            );

    description:

        This function is called to retrieve certain property associated
        with a repository folder.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder on which the operation
                should be carried out.

    return:     property value.

**********************************************************************/

ULONG
SysIroV2IraGetRfoPermission
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;

    return  SYS_RFO_ACCESS_MODE_ALL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroV2IraSetRfoPermission
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCurFolder,
                ULONG                       ulPermission
            );

    description:

        This function is called to modify certain property associated
        with a repository folder.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder on which the operation
                should be carried out.

                ULONG                       ulPermission
                Specifies the property value to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysIroV2IraSetRfoPermission
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ULONG                       ulPermission
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysIroV2IraGetRfoRenderAttr
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCurFolder
            );

    description:

        This function is called to retrieve certain property associated
        with a repository folder.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder on which the operation
                should be carried out.

    return:     property value.

**********************************************************************/

ANSC_HANDLE
SysIroV2IraGetRfoRenderAttr
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;

    AnscTrace("*******************************************************\n");
    AnscTrace("** SysIroV2IraGetRfoRenderAttr is called!!!          **\n");
    AnscTrace("*******************************************************\n");

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroV2IraSetRfoRenderAttr
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCurFolder,
                ANSC_HANDLE                 hRenderAttr
            );

    description:

        This function is called to modify certain property associated
        with a repository folder.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder on which the operation
                should be carried out.

                ANSC_HANDLE                 hRenderAttr
                Specifies the property value to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysIroV2IraSetRfoRenderAttr
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ANSC_HANDLE                 hRenderAttr
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysIroV2IraGetRroUserContext
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCurFolder,
                char*                       pRecordName
            );

    description:

        This function is called to retrieve certain property associated
        with a repository record.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder on which the operation
                should be carried out.

                char*                       pRecordName
                Specifies the name of the record on which the operation
                should be carried out.

    return:     property value.

**********************************************************************/

ANSC_HANDLE
SysIroV2IraGetRroUserContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;
    PSYS_IROV2_FOLDER_ENTRY         pCurFolder   = (PSYS_IROV2_FOLDER_ENTRY      )hCurFolder;
    PSYS_IROV2_RECORD_ENTRY         pCurRecord   = NULL;
    ANSC_HANDLE                     hUserContext = NULL;

    if ( !pMyObject->bActive )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else if ( !pCurFolder )
    {
        return  (ANSC_HANDLE)NULL;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, TRUE);

    pCurRecord =
        (PSYS_IROV2_RECORD_ENTRY)pMyObject->GetRecordByName
            (
                (ANSC_HANDLE)pMyObject,
                pCurFolder,
                pRecordName
            );

    if ( !pCurRecord )
    {
        hUserContext = (ANSC_HANDLE)NULL;
    }
    else
    {
        hUserContext = pCurRecord->hUserContext;
    }

    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, TRUE);

    return  hUserContext;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroV2IraSetRroUserContext
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCurFolder,
                char*                       pRecordName,
                ANSC_HANDLE                 hUserContext
            );

    description:

        This function is called to modify certain property associated
        with a repository record.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder on which the operation
                should be carried out.

                char*                       pRecordName
                Specifies the name of the record on which the operation
                should be carried out.

                ANSC_HANDLE                 hUserContext
                Specifies the property value to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysIroV2IraSetRroUserContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName,
        ANSC_HANDLE                 hUserContext
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;
    PSYS_RAM_INTERFACE              pSysRamIf    = (PSYS_RAM_INTERFACE           )pMyObject->hSysRamIf;
    PSYS_IROV2_FOLDER_ENTRY         pCurFolder   = (PSYS_IROV2_FOLDER_ENTRY      )hCurFolder;
    PSYS_IROV2_RECORD_ENTRY         pCurRecord   = NULL;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else if ( !pCurFolder )
    {
        return  ANSC_STATUS_INVALID_HANDLE;
    }
    else
    {
        AnscTrace("SysIroV2IraSetRroUserContext -- %s, %s...\n", pCurFolder->FolderName, pRecordName);
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, FALSE);

    pCurRecord =
        (PSYS_IROV2_RECORD_ENTRY)pMyObject->GetRecordByName
            (
                (ANSC_HANDLE)pMyObject,
                pCurFolder,
                pRecordName
            );

    if ( !pCurRecord )
    {
        returnStatus = ANSC_STATUS_BAD_NAME;
    }
    else
    {
        pCurRecord->hUserContext = hUserContext;
    }

    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, FALSE);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SysIroV2IraGetRroUserReserved
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCurFolder,
                char*                       pRecordName
            );

    description:

        This function is called to retrieve certain property associated
        with a repository record.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder on which the operation
                should be carried out.

                char*                       pRecordName
                Specifies the name of the record on which the operation
                should be carried out.

    return:     property value.

**********************************************************************/

ULONG
SysIroV2IraGetRroUserReserved
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;
    PSYS_IROV2_FOLDER_ENTRY         pCurFolder   = (PSYS_IROV2_FOLDER_ENTRY      )hCurFolder;
    PSYS_IROV2_RECORD_ENTRY         pCurRecord   = NULL;
    ULONG                           ulReserved   = 0;

    if ( !pMyObject->bActive )
    {
        return  0;
    }
    else if ( !pCurFolder )
    {
        return  0;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, TRUE);

    pCurRecord =
        (PSYS_IROV2_RECORD_ENTRY)pMyObject->GetRecordByName
            (
                (ANSC_HANDLE)pMyObject,
                pCurFolder,
                pRecordName
            );

    if ( !pCurRecord )
    {
        ulReserved = 0;
    }
    else
    {
        ulReserved = pCurRecord->UserReserved;
    }

    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, TRUE);

    return  ulReserved;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroV2IraSetRroUserReserved
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCurFolder,
                char*                       pRecordName,
                ULONG                       reserved
            );

    description:

        This function is called to modify certain property associated
        with a repository record.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder on which the operation
                should be carried out.

                char*                       pRecordName
                Specifies the name of the record on which the operation
                should be carried out.

                ULONG                       reserved
                Specifies the property value to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysIroV2IraSetRroUserReserved
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName,
        ULONG                       reserved
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;
    PSYS_IROV2_FOLDER_ENTRY         pCurFolder   = (PSYS_IROV2_FOLDER_ENTRY      )hCurFolder;
    PSYS_IROV2_RECORD_ENTRY         pCurRecord   = NULL;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else if ( !pCurFolder )
    {
        return  ANSC_STATUS_INVALID_HANDLE;
    }
    else
    {
        AnscTrace("SysIroV2IraSetRroUserReserved -- %s, %s...\n", pCurFolder->FolderName, pRecordName);
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, FALSE);

    pCurRecord =
        (PSYS_IROV2_RECORD_ENTRY)pMyObject->GetRecordByName
            (
                (ANSC_HANDLE)pMyObject,
                pCurFolder,
                pRecordName
            );

    if ( !pCurRecord )
    {
        returnStatus = ANSC_STATUS_BAD_NAME;
    }
    else
    {
        pCurRecord->UserReserved = reserved;
    }

    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, FALSE);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SysIroV2IraGetRroPermission
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCurFolder,
                char*                       pRecordName
            );

    description:

        This function is called to retrieve certain property associated
        with a repository record.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder on which the operation
                should be carried out.

                char*                       pRecordName
                Specifies the name of the record on which the operation
                should be carried out.

    return:     property value.

**********************************************************************/

ULONG
SysIroV2IraGetRroPermission
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;

    AnscTrace("*******************************************************\n");
    AnscTrace("** SysIroV2IraGetRroPermission is called!!!          **\n");
    AnscTrace("*******************************************************\n");

    return  SYS_RRO_ACCESS_MODE_ALL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroV2IraSetRroPermission
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCurFolder,
                char*                       pRecordName,
                ULONG                       ulPermission
            );

    description:

        This function is called to modify certain property associated
        with a repository record.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder on which the operation
                should be carried out.

                char*                       pRecordName
                Specifies the name of the record on which the operation
                should be carried out.

                ULONG                       ulPermission
                Specifies the property value to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysIroV2IraSetRroPermission
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName,
        ULONG                       ulPermission
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;

    AnscTrace("*******************************************************\n");
    AnscTrace("** SysIroV2IraSetRroPermission is called!!!          **\n");
    AnscTrace("*******************************************************\n");

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysIroV2IraGetRroRenderAttr
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCurFolder,
                char*                       pRecordName
            );

    description:

        This function is called to retrieve certain property associated
        with a repository record.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder on which the operation
                should be carried out.

                char*                       pRecordName
                Specifies the name of the record on which the operation
                should be carried out.

    return:     property value.

**********************************************************************/

ANSC_HANDLE
SysIroV2IraGetRroRenderAttr
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;

    AnscTrace("*******************************************************\n");
    AnscTrace("** SysIroV2IraGetRroRenderAttr is called!!!          **\n");
    AnscTrace("*******************************************************\n");

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroV2IraSetRroRenderAttr
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCurFolder,
                char*                       pRecordName,
                ANSC_HANDLE                 hRenderAttr
            );

    description:

        This function is called to modify certain property associated
        with a repository record.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder on which the operation
                should be carried out.

                char*                       pRecordName
                Specifies the name of the record on which the operation
                should be carried out.

                ANSC_HANDLE                 hRenderAttr
                Specifies the property value to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysIroV2IraSetRroRenderAttr
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName,
        ANSC_HANDLE                 hRenderAttr
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;

    AnscTrace("*******************************************************\n");
    AnscTrace("** SysIroV2IraSetRroRenderAttr is called!!!          **\n");
    AnscTrace("*******************************************************\n");

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysIroV2IraGetCurRecycleBin
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCurFolder
            );

    description:

        This function is called to retrieve certain property associated
        with a repository folder.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder on which the operation
                should be carried out.

    return:     property value.

**********************************************************************/

ANSC_HANDLE
SysIroV2IraGetCurRecycleBin
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;

    AnscTrace("*******************************************************\n");
    AnscTrace("** SysIroV2IraGetCurRecycleBin is called!!!          **\n");
    AnscTrace("*******************************************************\n");

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysIroV2IraAttachRecycleBin
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCurFolder,
                ANSC_HANDLE                 hRecycleBin
            );

    description:

        This function is called to modify certain property associated
        with a repository folder.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder on which the operation
                should be carried out.

                ANSC_HANDLE                 hRecycleBin
                Specifies the property value to be configured.

    return:     old property value.

**********************************************************************/

ANSC_HANDLE
SysIroV2IraAttachRecycleBin
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ANSC_HANDLE                 hRecycleBin
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject      = (PSYS_IROV2_OBJECT            )hThisObject;

    AnscTrace("*******************************************************\n");
    AnscTrace("** SysIroV2IraAttachRecycleBin is called!!!          **\n");
    AnscTrace("*******************************************************\n");

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysIroV2IraDetachRecycleBin
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCurFolder
            );

    description:

        This function is called to modify certain property associated
        with a repository folder.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder on which the operation
                should be carried out.

    return:     old property value.

**********************************************************************/

ANSC_HANDLE
SysIroV2IraDetachRecycleBin
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject      = (PSYS_IROV2_OBJECT            )hThisObject;

    AnscTrace("*******************************************************\n");
    AnscTrace("** SysIroV2IraDetachRecycleBin is called!!!          **\n");
    AnscTrace("*******************************************************\n");

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroV2IraSortSubFolders
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCurFolder,
                ULONG                       ulSortingType,
                BOOL                        bAscending
            );

    description:

        This function is called to sort all the sub-folder objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder on which the operation
                should be carried out.

                ULONG                       ulSortingType
                Specifies how the sorting should be done.

                BOOL                        bAscending
                Specifies how the sorting should be done.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysIroV2IraSortSubFolders
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ULONG                       ulSortingType,
        BOOL                        bAscending
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;
    PSYS_IROV2_FOLDER_ENTRY         pCurFolder   = (PSYS_IROV2_FOLDER_ENTRY      )hCurFolder;

    AnscTrace("*******************************************************\n");
    AnscTrace("** SysIroV2IraSortSubFolders is called on folder %s!!!\n", pCurFolder->FolderName);
    AnscTrace("*******************************************************\n");

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroV2IraSortRecords
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCurFolder,
                ULONG                       ulSortingType,
                BOOL                        bAscending
            );

    description:

        This function is called to sort all the record objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder on which the operation
                should be carried out.

                ULONG                       ulSortingType
                Specifies how the sorting should be done.

                BOOL                        bAscending
                Specifies how the sorting should be done.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysIroV2IraSortRecords
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ULONG                       ulSortingType,
        BOOL                        bAscending
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;

    AnscTrace("*******************************************************\n");
    AnscTrace("** SysIroV2IraSortRecords is called!!!               **\n");
    AnscTrace("*******************************************************\n");

    return  returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysIroV2IraGetSysRamIf
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to get the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle of the state.

**********************************************************************/

ANSC_HANDLE
SysIroV2IraGetSysRamIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;

    return  pMyObject->hSysRamIf;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroV2IraSetSysRamIf
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hInterface
            );

    description:

        This function is called to set the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hInterface
                Specifies the handle to set.

    return:     operation status.

**********************************************************************/

ANSC_STATUS
SysIroV2IraSetSysRamIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT            )hThisObject;

    pMyObject->hSysRamIf = hInterface;

    return returnStatus;   
}
