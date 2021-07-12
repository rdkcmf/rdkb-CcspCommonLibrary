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

        *   SysIroIraAcqWriteAccess
        *   SysIroIraRelWriteAccess
        *   SysIroIraAddSysFolder
        *   SysIroIraAddFolder
        *   SysIroIraAddFolder2
        *   SysIroIraDelFolder
        *   SysIroIraDelFolder2
        *   SysIroIraOpenFolder
        *   SysIroIraOpenFolder2
        *   SysIroIraCloseFolder
        *   SysIroIraClearFolder
        *   SysIroIraGenerateFolder
        *   SysIroIraPopulateFolder
        *   SysIroIraQueryFolder
        *   SysIroIraGetSubFolderCount
        *   SysIroIraEnumSubFolder
        *   SysIroIraTestSubFolder
        *   SysIroIraAddRecord
        *   SysIroIraAddRecord2
        *   SysIroIraDelRecord
        *   SysIroIraGetRecord
        *   SysIroIraSetRecord
        *   SysIroIraQueryRecord
        *   SysIroIraGetRecordCount
        *   SysIroIraEnumRecord
        *   SysIroIraTestRecord
        *   SysIroIraGetRfoUserContext
        *   SysIroIraSetRfoUserContext
        *   SysIroIraGetRfoUserReserved
        *   SysIroIraSetRfoUserReserved
        *   SysIroIraGetRfoFolderType
        *   SysIroIraSetRfoFolderType
        *   SysIroIraGetRfoPermission
        *   SysIroIraSetRfoPermission
        *   SysIroIraGetRfoRenderAttr
        *   SysIroIraSetRfoRenderAttr
        *   SysIroIraGetRroUserContext
        *   SysIroIraSetRroUserContext
        *   SysIroIraGetRroUserReserved
        *   SysIroIraSetRroUserReserved
        *   SysIroIraGetRroPermission
        *   SysIroIraSetRroPermission
        *   SysIroIraGetRroRenderAttr
        *   SysIroIraSetRroRenderAttr
        *   SysIroIraGetCurRecycleBin
        *   SysIroIraAttachRecycleBin
        *   SysIroIraDetachRecycleBin
        *   SysIroIraSortSubFolders
        *   SysIroIraSortRecords
        *   SysIroIraGetSysRamIf
        *   SysIroIraSetSysRamIf

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


#include "sys_iro_global.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroIraAcqWriteAccess
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
SysIroIraAcqWriteAccess
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    /*ULONG                           i            = 0;

    
    AnscAcquireLock(&pMyObject->AccessLock);

    for ( i = 0; i < (SYS_IRO_MAX_REP_ACCESS_COUNT - 1); i++ )
    {
        AnscAcquireSemaphore(&pMyObject->AccessSemaphore, 0xFFFFFFFF);
    }

    AnscReleaseLock(&pMyObject->AccessLock);
    */

    AnscAcquireLock(&pMyObject->AccessLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroIraRelWriteAccess
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
SysIroIraRelWriteAccess
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;

    /*
    AnscReleaseSemaphore(&pMyObject->AccessSemaphore, SYS_IRO_MAX_REP_ACCESS_COUNT - 1);
    */

    AnscReleaseLock(&pMyObject->AccessLock);

    return  ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
SysIroIraAcqThreadLock
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;

    AnscAcquireLock(&pMyObject->ThreadLock);

    return  ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
SysIroIraRelThreadLock
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;

    AnscReleaseLock(&pMyObject->ThreadLock);

    return  ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysIroIraAddSysFolder
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
SysIroIraAddSysFolder
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pFolderName
    )
{
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pRfoKey      = NULL;

    pRfoKey =
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

    return  (ANSC_HANDLE)pRfoKey;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysIroIraAddFolder
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
SysIroIraAddFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pSubFolderName
    )
{
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pRfoKey      = NULL;

    pRfoKey =
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

    return  (ANSC_HANDLE)pRfoKey;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

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
    )
{
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_REPOSITORY_DRIVER_OBJECT   pRepDriver   = (PSYS_REPOSITORY_DRIVER_OBJECT)pMyObject->hRepDriver;
    PSYS_RFO_KEY                    pCurKey      = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_RAM_INTERFACE              pSysRamIf    = (PSYS_RAM_INTERFACE           )pMyObject->hSysRamIf;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder   = NULL;
    PSYS_REPOSITORY_FOLDER_OBJECT   pSubFolder   = NULL;
    PSYS_RFO_KEY                    pSubKey      = NULL;

    if ( !pMyObject->bActive )
    {
        return  (ANSC_HANDLE)NULL;
    }

    if ( pCurKey )
    {
        if ( !(pCurKey->AccessMode & SYS_RFO_ACCESS_MODE_FOLDER_ADD) )
        {
            return  (ANSC_HANDLE)NULL;
        }
        else
        {
            pCurFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pCurKey->hRepFolder;
        }

        pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, FALSE);

        pSubFolder =
            (PSYS_REPOSITORY_FOLDER_OBJECT)pRepDriver->AddFolder2
                (
                    (ANSC_HANDLE)pRepDriver,
                    (ANSC_HANDLE)pCurFolder,
                    pSubFolderName,
                    ulPermission,
                    ulFolderType,
                    hRenderAttr,
                    hReserved
                );

        pMyObject->RelAccess((ANSC_HANDLE)pMyObject, FALSE);
    }
    else
    {
        pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, FALSE);

        pSubFolder =
            (PSYS_REPOSITORY_FOLDER_OBJECT)pRepDriver->AddFolder1
                (
                    (ANSC_HANDLE)pRepDriver,
                    pSubFolderName,
                    ulPermission,
                    ulFolderType,
                    hRenderAttr,
                    hReserved
                );

        pMyObject->RelAccess((ANSC_HANDLE)pMyObject, FALSE);
    }

    if ( !pSubFolder )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pSubKey =
            (PSYS_RFO_KEY)pSubFolder->IssueKey
                (
                    (ANSC_HANDLE)pSubFolder,
                    ulAccessMode
                );

        pSubFolder->ReleaseAccess((ANSC_HANDLE)pSubFolder);
    }

    if ( pSysRamIf )
    {
            pSysRamIf->Notify
                (
                    pSysRamIf->hOwnerContext,
                    hCurFolder,
                    SYS_RAM_EVENT_folderAdded
                );
    }
    return  (ANSC_HANDLE)pSubKey;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroIraDelFolder
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
SysIroIraDelFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pSubFolderName,
        BOOL                        bRecursive
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pCurKey      = (PSYS_RFO_KEY                 )hCurFolder;

    returnStatus =
        pMyObject->IraDelFolder2
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pCurKey,
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
        SysIroIraDelFolder2
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
SysIroIraDelFolder2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pSubFolderName,
        BOOL                        bRecursive,
        BOOL                        bForceful
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_REPOSITORY_DRIVER_OBJECT   pRepDriver   = (PSYS_REPOSITORY_DRIVER_OBJECT)pMyObject->hRepDriver;
    PSYS_RFO_KEY                    pCurKey      = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_RAM_INTERFACE              pSysRamIf    = (PSYS_RAM_INTERFACE           )pMyObject->hSysRamIf;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder   = NULL;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }

    if ( pCurKey )
    {
        if ( !(pCurKey->AccessMode & SYS_RFO_ACCESS_MODE_FOLDER_DELETE) && !bForceful )
        {
            return  ANSC_STATUS_ACCESS_DENIED;
        }
        else
        {
            pCurFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pCurKey->hRepFolder;
        }

        pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, FALSE);

        returnStatus =
            pRepDriver->DelFolder2
                (
                    (ANSC_HANDLE)pRepDriver,
                    (ANSC_HANDLE)pCurFolder,
                    pSubFolderName,
                    bRecursive,
                    bForceful
                );

        pMyObject->RelAccess((ANSC_HANDLE)pMyObject, FALSE);
    }
    else
    {
        pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, FALSE);

        returnStatus =
            pRepDriver->DelFolder1
                (
                    (ANSC_HANDLE)pRepDriver,
                    pSubFolderName,
                    bRecursive,
                    bForceful
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
        SysIroIraOpenFolder
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
SysIroIraOpenFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pSubFolderName
    )
{
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pCurKey      = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_RFO_KEY                    pSubKey      = NULL;

    pSubKey =
        (PSYS_RFO_KEY)pMyObject->IraOpenFolder2
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pCurKey,
                pSubFolderName,
                SYS_RFO_ACCESS_MODE_ADAPTIVE
            );

    return  (ANSC_HANDLE)pSubKey;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysIroIraOpenFolder2
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
SysIroIraOpenFolder2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pSubFolderName,
        ULONG                       ulAccessMode
    )
{
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_REPOSITORY_DRIVER_OBJECT   pRepDriver   = (PSYS_REPOSITORY_DRIVER_OBJECT)pMyObject->hRepDriver;
    PSYS_RFO_KEY                    pCurKey      = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder   = NULL;
    PSYS_REPOSITORY_FOLDER_OBJECT   pSubFolder   = NULL;
    PSYS_RFO_KEY                    pSubKey      = NULL;

    if ( !pMyObject->bActive )
    {
        return  (ANSC_HANDLE)NULL;
    }

    if ( pCurKey )
    {
        if ( !(pCurKey->AccessMode & SYS_RFO_ACCESS_MODE_RETRIEVE) )
        {
            return  (ANSC_HANDLE)NULL;
        }
        else
        {
            pCurFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pCurKey->hRepFolder;
        }

        pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, TRUE);

        pSubFolder =
            (PSYS_REPOSITORY_FOLDER_OBJECT)pRepDriver->GetFolder2
                (
                    (ANSC_HANDLE)pRepDriver,
                    (ANSC_HANDLE)pCurFolder,
                    pSubFolderName
                );

        pMyObject->RelAccess((ANSC_HANDLE)pMyObject, TRUE);
    }
    else
    {
        pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, TRUE);

        pSubFolder =
            (PSYS_REPOSITORY_FOLDER_OBJECT)pRepDriver->GetFolder1
                (
                    (ANSC_HANDLE)pRepDriver,
                    pSubFolderName
                );

        pMyObject->RelAccess((ANSC_HANDLE)pMyObject, TRUE);
    }

    if ( !pSubFolder )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pSubKey =
            (PSYS_RFO_KEY)pSubFolder->IssueKey
                (
                    (ANSC_HANDLE)pSubFolder,
                    ulAccessMode
                );

        pSubFolder->ReleaseAccess((ANSC_HANDLE)pSubFolder);
    }

    return  (ANSC_HANDLE)pSubKey;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroIraCloseFolder
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
SysIroIraCloseFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pCurKey      = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder   = NULL;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, TRUE);

    if ( pCurKey )
    {
        pCurFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pCurKey->hRepFolder;

        pCurFolder->AcquireAccess((ANSC_HANDLE)pCurFolder);

        returnStatus =
            pCurFolder->ClearKey
                (
                    (ANSC_HANDLE)pCurFolder,
                    (ANSC_HANDLE)pCurKey
                );

        pCurFolder->ReleaseAccess((ANSC_HANDLE)pCurFolder);
    }

    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, TRUE);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroIraClearFolder
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
SysIroIraClearFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pCurKey      = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_RAM_INTERFACE              pSysRamIf    = (PSYS_RAM_INTERFACE           )pMyObject->hSysRamIf;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder   = NULL;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, FALSE);

    if ( pCurKey )
    {
        pCurFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pCurKey->hRepFolder;

        pCurFolder->AcquireAccess((ANSC_HANDLE)pCurFolder);
        pCurFolder->Reset        ((ANSC_HANDLE)pCurFolder);
        pCurFolder->ReleaseAccess((ANSC_HANDLE)pCurFolder);
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
        SysIroIraGenerateFolder
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
SysIroIraGenerateFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pCurKey      = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_RAM_INTERFACE              pSysRamIf    = (PSYS_RAM_INTERFACE           )pMyObject->hSysRamIf;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder   = NULL;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, FALSE);

    if ( pCurKey )
    {
        pCurFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pCurKey->hRepFolder;

        pCurFolder->AcquireAccess  ((ANSC_HANDLE)pCurFolder);
        pCurFolder->GenerateContent((ANSC_HANDLE)pCurFolder);
        pCurFolder->ReleaseAccess  ((ANSC_HANDLE)pCurFolder);
    }

    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, FALSE);

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
        SysIroIraPopulateFolder
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
SysIroIraPopulateFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pCurKey      = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_RAM_INTERFACE              pSysRamIf    = (PSYS_RAM_INTERFACE           )pMyObject->hSysRamIf;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder   = NULL;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, FALSE);

    if ( pCurKey )
    {
        pCurFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pCurKey->hRepFolder;

        pCurFolder->AcquireAccess  ((ANSC_HANDLE)pCurFolder);
        pCurFolder->PopulateContent((ANSC_HANDLE)pCurFolder);
        pCurFolder->ReleaseAccess  ((ANSC_HANDLE)pCurFolder);
    }

    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, FALSE);

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
    )
{
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pCurKey      = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder   = NULL;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else if ( !pCurKey )
    {
        return  ANSC_STATUS_INVALID_HANDLE;
    }
    else if ( !(pCurKey->AccessMode & SYS_RFO_ACCESS_MODE_RETRIEVE) )
    {
        return  ANSC_STATUS_ACCESS_DENIED;
    }
    else
    {
        pCurFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pCurKey->hRepFolder;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, TRUE);

    if ( pulTimestamp )
    {
        *pulTimestamp = pCurFolder->Timestamp;
    }

    if ( pulPermission )
    {
        *pulPermission = pCurFolder->GetPermission((ANSC_HANDLE)pCurFolder);
    }

    if ( pulFolderType )
    {
        *pulFolderType = pCurFolder->GetFolderType((ANSC_HANDLE)pCurFolder);
    }

    if ( pulSubFolderCount )
    {
        *pulSubFolderCount = pCurFolder->GetFolderCount((ANSC_HANDLE)pCurFolder);
    }

    if ( pulRecordCount )
    {
        *pulRecordCount = pCurFolder->GetRecordCount((ANSC_HANDLE)pCurFolder);
    }

    if ( pulContentType )
    {
        *pulContentType = pCurFolder->GetContentType((ANSC_HANDLE)pCurFolder);
    }

    if ( phRenderAttr )
    {
        *phRenderAttr = pCurFolder->GetRenderAttr((ANSC_HANDLE)pCurFolder);
    }

    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, TRUE);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SysIroIraGetSubFolderCount
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
SysIroIraGetSubFolderCount
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    )
{
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject     = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pCurKey       = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder    = NULL;
    ULONG                           ulFolderCount = 0;

    if ( !pMyObject->bActive )
    {
        return  0;
    }
    else if ( !pCurKey )
    {
        return  0;
    }
    else if ( !(pCurKey->AccessMode & SYS_RFO_ACCESS_MODE_RETRIEVE) )
    {
        return  0;
    }
    else
    {
        pCurFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pCurKey->hRepFolder;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, TRUE);
    ulFolderCount = pCurFolder->GetFolderCount((ANSC_HANDLE)pCurFolder);
    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, TRUE);

    return  ulFolderCount;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroIraEnumSubFolder
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
SysIroIraEnumSubFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ULONG                       ulIndex,
        char*                       pSubFolderName,
        PULONG                      pulNameSize
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pCurKey      = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder   = NULL;
    PSYS_REPOSITORY_FOLDER_OBJECT   pSubFolder   = NULL;
    char*                           pFolderName  = NULL;
    errno_t      rc  = -1;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else if ( !pCurKey )
    {
        return  ANSC_STATUS_INVALID_HANDLE;
    }
    else if ( !(pCurKey->AccessMode & SYS_RFO_ACCESS_MODE_RETRIEVE) )
    {
        return  ANSC_STATUS_ACCESS_DENIED;
    }
    else
    {
        pCurFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pCurKey->hRepFolder;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, TRUE);

    pSubFolder =
        (PSYS_REPOSITORY_FOLDER_OBJECT)pCurFolder->GetFolderByIndex
            (
                (ANSC_HANDLE)pCurFolder,
                ulIndex
            );

    if ( !pSubFolder )
    {
        returnStatus = ANSC_STATUS_CANT_FIND;

        goto  EXIT1;
    }
    else
    {
        pFolderName = pSubFolder->GetFolderName((ANSC_HANDLE)pSubFolder);
    }

    pSubFolder->ReleaseAccess((ANSC_HANDLE)pSubFolder);

    if ( *pulNameSize < AnscSizeOfString(pFolderName) )
    {
        *pulNameSize = AnscSizeOfString(pFolderName);

        returnStatus = ANSC_STATUS_BAD_SIZE;

        goto  EXIT1;
    }
    else
    {
        rc = strcpy_s(pSubFolderName, *pulNameSize, pFolderName);
        ERR_CHK(rc);

        *pulNameSize = AnscSizeOfString(pFolderName);
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

        BOOL
        SysIroIraTestSubFolder
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
SysIroIraTestSubFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pSubFolderName,
        ULONG                       ulAccessMode
    )
{
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_REPOSITORY_DRIVER_OBJECT   pRepDriver   = (PSYS_REPOSITORY_DRIVER_OBJECT)pMyObject->hRepDriver;
    PSYS_RFO_KEY                    pCurKey      = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder   = NULL;
    PSYS_REPOSITORY_FOLDER_OBJECT   pSubFolder   = NULL;
    BOOL                            bTestResult  = FALSE;

    if ( !pMyObject->bActive )
    {
        return  FALSE;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, TRUE);

    if ( pCurKey )
    {
        if ( !(pCurKey->AccessMode & SYS_RFO_ACCESS_MODE_RETRIEVE) )
        {
            bTestResult = FALSE;

            goto  EXIT1;
        }
        else
        {
            pCurFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pCurKey->hRepFolder;
        }

        pSubFolder =
            (PSYS_REPOSITORY_FOLDER_OBJECT)pRepDriver->GetFolder2
                (
                    (ANSC_HANDLE)pRepDriver,
                    (ANSC_HANDLE)pCurFolder,
                    pSubFolderName
                );
    }
    else
    {
        pSubFolder =
            (PSYS_REPOSITORY_FOLDER_OBJECT)pRepDriver->GetFolder1
                (
                    (ANSC_HANDLE)pRepDriver,
                    pSubFolderName
                );
    }

    if ( !pSubFolder )
    {
        bTestResult = FALSE;

        goto  EXIT1;
    }
    else
    {
        bTestResult =
            pSubFolder->GrantAccess
                (
                    (ANSC_HANDLE)pSubFolder,
                    ulAccessMode
                );

        pSubFolder->ReleaseAccess((ANSC_HANDLE)pSubFolder);
    }


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, TRUE);

    return  bTestResult;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

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
SysIroIraAddRecord
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
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pCurKey      = (PSYS_RFO_KEY                 )hCurFolder;

    returnStatus =
        pMyObject->IraAddRecord2
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pCurKey,
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
    )
{
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pCurKey      = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_RAM_INTERFACE              pSysRamIf    = (PSYS_RAM_INTERFACE           )pMyObject->hSysRamIf;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder   = NULL;
    PSYS_REPOSITORY_RECORD_OBJECT   pNewRecord   = NULL;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else if ( !pCurKey )
    {
        return  ANSC_STATUS_INVALID_HANDLE;
    }
    else if ( !(pCurKey->AccessMode & SYS_RFO_ACCESS_MODE_RECORD_ADD) )
    {
        return  ANSC_STATUS_ACCESS_DENIED;
    }
    else
    {
        pCurFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pCurKey->hRepFolder;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, FALSE);

    pNewRecord =
        (PSYS_REPOSITORY_RECORD_OBJECT)pCurFolder->AddRecord
            (
                (ANSC_HANDLE)pCurFolder,
                pRecordName,
                ulPermission,
                ulRecordType,
                hRenderAttr,
                pWriteBuffer,
                ulWriteSize
            );

    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, FALSE);

    if ( !pNewRecord )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    if ( pSysRamIf )
    {
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
        SysIroIraDelRecord
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
SysIroIraDelRecord
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pCurKey      = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_RAM_INTERFACE              pSysRamIf    = (PSYS_RAM_INTERFACE           )pMyObject->hSysRamIf;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder   = NULL;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else if ( !pCurKey )
    {
        return  ANSC_STATUS_INVALID_HANDLE;
    }
    else if ( !(pCurKey->AccessMode & SYS_RFO_ACCESS_MODE_RECORD_DELETE) )
    {
        return  ANSC_STATUS_ACCESS_DENIED;
    }
    else
    {
        pCurFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pCurKey->hRepFolder;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, FALSE);

    returnStatus =
        pCurFolder->DelRecord
            (
                (ANSC_HANDLE)pCurFolder,
                pRecordName
            );

    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, FALSE);

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
SysIroIraGetRecord
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
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pCurKey      = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder   = NULL;
    PSYS_REPOSITORY_RECORD_OBJECT   pCurRecord   = NULL;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else if ( !pCurKey )
    {
        return  ANSC_STATUS_INVALID_HANDLE;
    }
    else if ( !(pCurKey->AccessMode & SYS_RFO_ACCESS_MODE_RETRIEVE) )
    {
        return  ANSC_STATUS_ACCESS_DENIED;
    }
    else
    {
        pCurFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pCurKey->hRepFolder;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, TRUE);

    pCurRecord =
        (PSYS_REPOSITORY_RECORD_OBJECT)pCurFolder->GetRecordByName
            (
                (ANSC_HANDLE)pCurFolder,
                pRecordName
            );

    if ( !pCurRecord )
    {
        returnStatus = ANSC_STATUS_CANT_FIND;

        goto  EXIT1;
    }
    else if ( !(pCurRecord->GetPermission((ANSC_HANDLE)pCurRecord) & SYS_RRO_PERMISSION_RETRIEVE) )
    {
        returnStatus = ANSC_STATUS_ACCESS_DENIED;

        goto  EXIT1;
    }

    if ( pulRecordType )
    {
        *pulRecordType = pCurRecord->GetRecordType((ANSC_HANDLE)pCurRecord);
    }

    if ( phRenderAttr )
    {
        *phRenderAttr = pCurRecord->GetRenderAttr((ANSC_HANDLE)pCurRecord);
    }

    if ( !pReadBuffer || (*pulReadSize < pCurRecord->GetRecordSize((ANSC_HANDLE)pCurRecord)) )
    {
        *pulReadSize = pCurRecord->GetRecordSize((ANSC_HANDLE)pCurRecord);

        returnStatus = ANSC_STATUS_BAD_SIZE;

        goto  EXIT1;
    }

    returnStatus =
        pCurRecord->GetRecordData
            (
                (ANSC_HANDLE)pCurRecord,
                pReadBuffer,
                pulReadSize
            );


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, TRUE);

    return  returnStatus;
}

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
    )
{
    UNREFERENCED_PARAMETER(ulAccess);
    ANSC_STATUS                     returnStatus= ANSC_STATUS_SUCCESS;
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject   = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pCurKey     = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder  = NULL;
    PSYS_REPOSITORY_RECORD_OBJECT   pCurRecord  = NULL;
    CHAR                            *ptr;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else if ( !pCurKey )
    {
        return  ANSC_STATUS_INVALID_HANDLE;
    }
    else if ( !(pCurKey->AccessMode & SYS_RFO_ACCESS_MODE_RETRIEVE) )
    {
        return  ANSC_STATUS_ACCESS_DENIED;
    }
    else
    {
        pCurFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pCurKey->hRepFolder;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, TRUE);

    pCurRecord = 
        (PSYS_REPOSITORY_RECORD_OBJECT)pCurFolder->GetRecordByIndex
            (
                 (ANSC_HANDLE)pCurFolder, 
                 ulIndex
            );

    if ( !pCurRecord )
    {
        returnStatus = ANSC_STATUS_CANT_FIND;

        goto  EXIT1;
    }
    else if ( !(pCurRecord->GetPermission((ANSC_HANDLE)pCurRecord) & SYS_RRO_PERMISSION_RETRIEVE) )
    {
        returnStatus = ANSC_STATUS_ACCESS_DENIED;

        goto  EXIT1;
    }

    if (pName && pNameSize)
    {
        ptr = pCurRecord->GetRecordName((ANSC_HANDLE)pCurRecord);
        if (*pNameSize <= AnscSizeOfString(ptr))
        {
            *pNameSize = AnscSizeOfString(ptr) + 1;
            returnStatus = ANSC_STATUS_BAD_SIZE;

            goto  EXIT1;
        }
        snprintf(pName, *pNameSize, "%s", ptr);
    }

    if (pRecType)
    {
        *pRecType = pCurRecord->GetRecordType((ANSC_HANDLE)pCurRecord);
    }

    if (pContentType)
    {
        *pContentType = pCurRecord->GetContentType((ANSC_HANDLE)pCurRecord);
    }

    if (pData && pDataSize)
    {
        *pDataSize = (pCurRecord->GetRecordSize((ANSC_HANDLE)pCurRecord) + 1);

        *pData = AnscAllocateMemory(*pDataSize);
        if ((*pData) == NULL)
        {
            returnStatus = ANSC_STATUS_RESOURCES;

            goto  EXIT1;
        }

        returnStatus = 
            pCurRecord->GetRecordData((ANSC_HANDLE)pCurRecord, *pData, pDataSize);

        if (returnStatus != ANSC_STATUS_SUCCESS)
        {
            AnscFreeMemory(*pData);
        }
    }

EXIT1:
    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, TRUE);

    return returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

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
SysIroIraSetRecord
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
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pCurKey      = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_RAM_INTERFACE              pSysRamIf    = (PSYS_RAM_INTERFACE           )pMyObject->hSysRamIf;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder   = NULL;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else if ( !pCurKey )
    {
        return  ANSC_STATUS_INVALID_HANDLE;
    }
    else if ( !(pCurKey->AccessMode & SYS_RFO_ACCESS_MODE_RECORD_MODIFY) )
    {
        return  ANSC_STATUS_ACCESS_DENIED;
    }
    else
    {
        pCurFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pCurKey->hRepFolder;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, FALSE);

    returnStatus =
        pCurFolder->SetRecord
            (
                (ANSC_HANDLE)pCurFolder,
                pRecordName,
                ulRecordType,
                hRenderAttr,
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
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pCurKey      = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder   = NULL;
    PSYS_REPOSITORY_RECORD_OBJECT   pCurRecord   = NULL;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else if ( !pCurKey )
    {
        return  ANSC_STATUS_INVALID_HANDLE;
    }
    else if ( !(pCurKey->AccessMode & SYS_RFO_ACCESS_MODE_RETRIEVE) )
    {
        return  ANSC_STATUS_ACCESS_DENIED;
    }
    else
    {
        pCurFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pCurKey->hRepFolder;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, TRUE);

    pCurRecord =
        (PSYS_REPOSITORY_RECORD_OBJECT)pCurFolder->GetRecordByName
            (
                (ANSC_HANDLE)pCurFolder,
                pRecordName
            );

    if ( !pCurRecord )
    {
        returnStatus = ANSC_STATUS_CANT_FIND;

        goto  EXIT1;
    }

    if ( pulTimestamp )
    {
        *pulTimestamp = pCurRecord->Timestamp;
    }

    if ( pulPermission )
    {
        *pulPermission = pCurRecord->GetPermission((ANSC_HANDLE)pCurRecord);
    }

    if ( pulRecordType )
    {
        *pulRecordType = pCurRecord->GetRecordType((ANSC_HANDLE)pCurRecord);
    }

    if ( pulRecordSize )
    {
        *pulRecordSize = pCurRecord->GetRecordSize((ANSC_HANDLE)pCurRecord);
    }

    if ( pulContentType )
    {
        *pulContentType = pCurRecord->GetContentType((ANSC_HANDLE)pCurRecord);
    }

    if ( phRenderAttr )
    {
        *phRenderAttr = pCurRecord->GetRenderAttr((ANSC_HANDLE)pCurRecord);
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
        SysIroIraGetRecordCount
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
SysIroIraGetRecordCount
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    )
{
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject     = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pCurKey       = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder    = NULL;
    ULONG                           ulRecordCount = 0;

    if ( !pMyObject->bActive )
    {
        return  0;
    }
    else if ( !pCurKey )
    {
        return  0;
    }
    else if ( !(pCurKey->AccessMode & SYS_RFO_ACCESS_MODE_RETRIEVE) )
    {
        return  0;
    }
    else
    {
        pCurFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pCurKey->hRepFolder;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, TRUE);
    ulRecordCount = pCurFolder->GetRecordCount((ANSC_HANDLE)pCurFolder);
    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, TRUE);

    return  ulRecordCount;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroIraEnumRecord
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
SysIroIraEnumRecord
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
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject      = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pCurKey        = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder     = NULL;
    PSYS_REPOSITORY_RECORD_OBJECT   pCurRecord     = NULL;
    char*                           pCurRecordName = NULL;
    errno_t   rc = -1;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else if ( !pCurKey )
    {
        return  ANSC_STATUS_INVALID_HANDLE;
    }
    else if ( !(pCurKey->AccessMode & SYS_RFO_ACCESS_MODE_RETRIEVE) )
    {
        return  ANSC_STATUS_ACCESS_DENIED;
    }
    else
    {
        pCurFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pCurKey->hRepFolder;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, TRUE);

    pCurRecord =
        (PSYS_REPOSITORY_RECORD_OBJECT)pCurFolder->GetRecordByIndex
            (
                (ANSC_HANDLE)pCurFolder,
                ulIndex
            );

    if ( !pCurRecord )
    {
        returnStatus = ANSC_STATUS_CANT_FIND;

        goto  EXIT1;
    }
    else
    {
        pCurRecordName = pCurRecord->GetRecordName((ANSC_HANDLE)pCurRecord);
    }

    if ( *pulNameSize < AnscSizeOfString(pCurRecordName) )
    {
        *pulNameSize = AnscSizeOfString(pCurRecordName);

        returnStatus = ANSC_STATUS_BAD_SIZE;

        goto  EXIT1;
    }
    else
    {

        if ( pulRecordType )
        {
            *pulRecordType = pCurRecord->GetRecordType((ANSC_HANDLE)pCurRecord);
        }

        if ( pulDataSize )
        {
            *pulDataSize = pCurRecord->GetRecordSize((ANSC_HANDLE)pCurRecord);
        }

        rc = strcpy_s(pRecordName, *pulNameSize, pCurRecordName);
        ERR_CHK(rc);

        *pulNameSize = AnscSizeOfString(pCurRecordName);
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
        SysIroIraTestRecord
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
SysIroIraTestRecord
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName,
        ULONG                       ulAccessMode
    )
{
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pCurKey      = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder   = NULL;
    PSYS_REPOSITORY_RECORD_OBJECT   pCurRecord   = NULL;
    BOOL                            bTestResult  = FALSE;

    if ( !pMyObject->bActive )
    {
        return  FALSE;
    }
    else if ( !pCurKey )
    {
        return  FALSE;
    }
    else if ( !(pCurKey->AccessMode & SYS_RFO_ACCESS_MODE_RETRIEVE) )
    {
        return  FALSE;
    }
    else
    {
        pCurFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pCurKey->hRepFolder;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, TRUE);

    pCurRecord =
        (PSYS_REPOSITORY_RECORD_OBJECT)pCurFolder->GetRecordByName
            (
                (ANSC_HANDLE)pCurFolder,
                pRecordName
            );

    if ( !pCurRecord )
    {
        bTestResult = FALSE;

        goto  EXIT1;
    }
    else
    {
        bTestResult =
            pCurRecord->GrantAccess
                (
                    (ANSC_HANDLE)pCurRecord,
                    ulAccessMode
                );
    }


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, TRUE);

    return  bTestResult;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysIroIraGetRfoUserContext
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
SysIroIraGetRfoUserContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    )
{
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pCurKey      = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder   = NULL;
    ANSC_HANDLE                     hUserContext = NULL;

    if ( !pMyObject->bActive )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else if ( !pCurKey )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pCurFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pCurKey->hRepFolder;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, TRUE);
    hUserContext = pCurFolder->GetUserContext((ANSC_HANDLE)pCurFolder);
    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, TRUE);

    return  hUserContext;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroIraSetRfoUserContext
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
SysIroIraSetRfoUserContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ANSC_HANDLE                 hUserContext
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pCurKey      = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder   = NULL;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else if ( !pCurKey )
    {
        return  ANSC_STATUS_INVALID_HANDLE;
    }
    else
    {
        pCurFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pCurKey->hRepFolder;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, FALSE);

    returnStatus =
        pCurFolder->SetUserContext
            (
                (ANSC_HANDLE)pCurFolder,
                hUserContext
            );

    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, FALSE);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SysIroIraGetRfoUserReserved
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
SysIroIraGetRfoUserReserved
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    )
{
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pCurKey      = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder   = NULL;
    ULONG                           ulReserved   = 0;

    if ( !pMyObject->bActive )
    {
        return  0;
    }
    else if ( !pCurKey )
    {
        return  0;
    }
    else
    {
        pCurFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pCurKey->hRepFolder;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, TRUE);
    ulReserved = pCurFolder->GetUserReserved((ANSC_HANDLE)pCurFolder);
    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, TRUE);

    return  ulReserved;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroIraSetRfoUserReserved
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
SysIroIraSetRfoUserReserved
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ULONG                       reserved
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pCurKey      = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder   = NULL;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else if ( !pCurKey )
    {
        return  ANSC_STATUS_INVALID_HANDLE;
    }
    else
    {
        pCurFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pCurKey->hRepFolder;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, FALSE);

    returnStatus =
        pCurFolder->SetUserReserved
            (
                (ANSC_HANDLE)pCurFolder,
                reserved
            );

    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, FALSE);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SysIroIraGetRfoFolderType
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
SysIroIraGetRfoFolderType
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    )
{
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pCurKey      = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder   = NULL;
    ULONG                           ulFolderType = SYS_REP_FOLDER_TYPE_STORAGE;

    if ( !pMyObject->bActive )
    {
        return  SYS_REP_FOLDER_TYPE_STORAGE;
    }
    else if ( !pCurKey )
    {
        return  SYS_REP_FOLDER_TYPE_STORAGE;
    }
    else
    {
        pCurFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pCurKey->hRepFolder;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, TRUE);
    ulFolderType = pCurFolder->GetFolderType((ANSC_HANDLE)pCurFolder);
    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, TRUE);

    return  ulFolderType;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroIraSetRfoFolderType
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
SysIroIraSetRfoFolderType
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ULONG                       ulFolderType
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pCurKey      = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder   = NULL;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else if ( !pCurKey )
    {
        return  ANSC_STATUS_INVALID_HANDLE;
    }
    else
    {
        pCurFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pCurKey->hRepFolder;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, FALSE);

    returnStatus =
        pCurFolder->SetFolderType
            (
                (ANSC_HANDLE)pCurFolder,
                ulFolderType
            );

    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, FALSE);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SysIroIraGetRfoPermission
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
SysIroIraGetRfoPermission
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    )
{
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pCurKey      = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder   = NULL;
    ULONG                           ulPermission = SYS_RFO_PERMISSION_PROHIBITED;

    if ( !pMyObject->bActive )
    {
        return  SYS_RFO_PERMISSION_PROHIBITED;
    }
    else if ( !pCurKey )
    {
        return  SYS_RFO_PERMISSION_PROHIBITED;
    }
    else
    {
        pCurFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pCurKey->hRepFolder;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, TRUE);
    ulPermission = pCurFolder->GetPermission((ANSC_HANDLE)pCurFolder);
    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, TRUE);

    return  ulPermission;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroIraSetRfoPermission
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
SysIroIraSetRfoPermission
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ULONG                       ulPermission
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pCurKey      = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder   = NULL;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else if ( !pCurKey )
    {
        return  ANSC_STATUS_INVALID_HANDLE;
    }
    else
    {
        pCurFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pCurKey->hRepFolder;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, FALSE);

    returnStatus =
        pCurFolder->SetPermission
            (
                (ANSC_HANDLE)pCurFolder,
                ulPermission
            );

    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, FALSE);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysIroIraGetRfoRenderAttr
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
SysIroIraGetRfoRenderAttr
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    )
{
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pCurKey      = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder   = NULL;
    ANSC_HANDLE                     hRenderAttr  = NULL;

    if ( !pMyObject->bActive )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else if ( !pCurKey )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pCurFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pCurKey->hRepFolder;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, TRUE);
    hRenderAttr = pCurFolder->GetRenderAttr((ANSC_HANDLE)pCurFolder);
    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, TRUE);

    return  hRenderAttr;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroIraSetRfoRenderAttr
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
SysIroIraSetRfoRenderAttr
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ANSC_HANDLE                 hRenderAttr
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pCurKey      = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder   = NULL;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else if ( !pCurKey )
    {
        return  ANSC_STATUS_INVALID_HANDLE;
    }
    else
    {
        pCurFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pCurKey->hRepFolder;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, FALSE);

    returnStatus =
        pCurFolder->SetRenderAttr
            (
                (ANSC_HANDLE)pCurFolder,
                hRenderAttr
            );

    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, FALSE);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysIroIraGetRroUserContext
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
SysIroIraGetRroUserContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName
    )
{
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pCurKey      = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder   = NULL;
    PSYS_REPOSITORY_RECORD_OBJECT   pCurRecord   = NULL;
    ANSC_HANDLE                     hUserContext = NULL;

    if ( !pMyObject->bActive )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else if ( !pCurKey )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pCurFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pCurKey->hRepFolder;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, TRUE);

    pCurRecord =
        (PSYS_REPOSITORY_RECORD_OBJECT)pCurFolder->GetRecordByName
            (
                (ANSC_HANDLE)pCurFolder,
                pRecordName
            );

    if ( !pCurRecord )
    {
        hUserContext = (ANSC_HANDLE)NULL;
    }
    else
    {
        hUserContext = pCurRecord->GetUserContext((ANSC_HANDLE)pCurRecord);
    }

    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, TRUE);

    return  hUserContext;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroIraSetRroUserContext
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
SysIroIraSetRroUserContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName,
        ANSC_HANDLE                 hUserContext
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pCurKey      = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder   = NULL;
    PSYS_REPOSITORY_RECORD_OBJECT   pCurRecord   = NULL;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else if ( !pCurKey )
    {
        return  ANSC_STATUS_INVALID_HANDLE;
    }
    else
    {
        pCurFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pCurKey->hRepFolder;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, FALSE);

    pCurRecord =
        (PSYS_REPOSITORY_RECORD_OBJECT)pCurFolder->GetRecordByName
            (
                (ANSC_HANDLE)pCurFolder,
                pRecordName
            );

    if ( !pCurRecord )
    {
        returnStatus = ANSC_STATUS_BAD_NAME;
    }
    else
    {
        returnStatus =
            pCurRecord->SetUserContext
                (
                    (ANSC_HANDLE)pCurRecord,
                    hUserContext
                );
    }

    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, FALSE);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SysIroIraGetRroUserReserved
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
SysIroIraGetRroUserReserved
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName
    )
{
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pCurKey      = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder   = NULL;
    PSYS_REPOSITORY_RECORD_OBJECT   pCurRecord   = NULL;
    ULONG                           ulReserved   = 0;

    if ( !pMyObject->bActive )
    {
        return  0;
    }
    else if ( !pCurKey )
    {
        return  0;
    }
    else
    {
        pCurFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pCurKey->hRepFolder;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, TRUE);

    pCurRecord =
        (PSYS_REPOSITORY_RECORD_OBJECT)pCurFolder->GetRecordByName
            (
                (ANSC_HANDLE)pCurFolder,
                pRecordName
            );

    if ( !pCurRecord )
    {
        ulReserved = 0;
    }
    else
    {
        ulReserved = pCurRecord->GetUserReserved((ANSC_HANDLE)pCurRecord);
    }

    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, TRUE);

    return  ulReserved;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroIraSetRroUserReserved
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
SysIroIraSetRroUserReserved
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName,
        ULONG                       reserved
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pCurKey      = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder   = NULL;
    PSYS_REPOSITORY_RECORD_OBJECT   pCurRecord   = NULL;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else if ( !pCurKey )
    {
        return  ANSC_STATUS_INVALID_HANDLE;
    }
    else
    {
        pCurFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pCurKey->hRepFolder;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, FALSE);

    pCurRecord =
        (PSYS_REPOSITORY_RECORD_OBJECT)pCurFolder->GetRecordByName
            (
                (ANSC_HANDLE)pCurFolder,
                pRecordName
            );

    if ( !pCurRecord )
    {
        returnStatus = ANSC_STATUS_BAD_NAME;
    }
    else
    {
        returnStatus =
            pCurRecord->SetUserReserved
                (
                    (ANSC_HANDLE)pCurRecord,
                    reserved
                );
    }

    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, FALSE);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SysIroIraGetRroPermission
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
SysIroIraGetRroPermission
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName
    )
{
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pCurKey      = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder   = NULL;
    PSYS_REPOSITORY_RECORD_OBJECT   pCurRecord   = NULL;
    ULONG                           ulPermission = 0;

    if ( !pMyObject->bActive )
    {
        return  SYS_RRO_PERMISSION_PROHIBITED;
    }
    else if ( !pCurKey )
    {
        return  SYS_RRO_PERMISSION_PROHIBITED;
    }
    else
    {
        pCurFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pCurKey->hRepFolder;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, TRUE);

    pCurRecord =
        (PSYS_REPOSITORY_RECORD_OBJECT)pCurFolder->GetRecordByName
            (
                (ANSC_HANDLE)pCurFolder,
                pRecordName
            );

    if ( !pCurRecord )
    {
        ulPermission = SYS_RRO_PERMISSION_PROHIBITED;
    }
    else
    {
        ulPermission = pCurRecord->GetPermission((ANSC_HANDLE)pCurRecord);
    }

    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, TRUE);

    return  ulPermission;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroIraSetRroPermission
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
SysIroIraSetRroPermission
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName,
        ULONG                       ulPermission
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pCurKey      = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder   = NULL;
    PSYS_REPOSITORY_RECORD_OBJECT   pCurRecord   = NULL;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else if ( !pCurKey )
    {
        return  ANSC_STATUS_INVALID_HANDLE;
    }
    else
    {
        pCurFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pCurKey->hRepFolder;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, FALSE);

    pCurRecord =
        (PSYS_REPOSITORY_RECORD_OBJECT)pCurFolder->GetRecordByName
            (
                (ANSC_HANDLE)pCurFolder,
                pRecordName
            );

    if ( !pCurRecord )
    {
        returnStatus = ANSC_STATUS_BAD_NAME;
    }
    else
    {
        returnStatus =
            pCurRecord->SetPermission
                (
                    (ANSC_HANDLE)pCurRecord,
                    ulPermission
                );
    }

    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, FALSE);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysIroIraGetRroRenderAttr
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
SysIroIraGetRroRenderAttr
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName
    )
{
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pCurKey      = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder   = NULL;
    PSYS_REPOSITORY_RECORD_OBJECT   pCurRecord   = NULL;
    ANSC_HANDLE                     hRenderAttr  = NULL;

    if ( !pMyObject->bActive )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else if ( !pCurKey )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pCurFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pCurKey->hRepFolder;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, TRUE);

    pCurRecord =
        (PSYS_REPOSITORY_RECORD_OBJECT)pCurFolder->GetRecordByName
            (
                (ANSC_HANDLE)pCurFolder,
                pRecordName
            );

    if ( !pCurRecord )
    {
        hRenderAttr = (ANSC_HANDLE)NULL;
    }
    else
    {
        hRenderAttr = pCurRecord->GetRenderAttr((ANSC_HANDLE)pCurRecord);
    }

    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, TRUE);

    return  hRenderAttr;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroIraSetRroRenderAttr
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
SysIroIraSetRroRenderAttr
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pRecordName,
        ANSC_HANDLE                 hRenderAttr
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pCurKey      = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder   = NULL;
    PSYS_REPOSITORY_RECORD_OBJECT   pCurRecord   = NULL;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else if ( !pCurKey )
    {
        return  ANSC_STATUS_INVALID_HANDLE;
    }
    else
    {
        pCurFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pCurKey->hRepFolder;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, FALSE);

    pCurRecord =
        (PSYS_REPOSITORY_RECORD_OBJECT)pCurFolder->GetRecordByName
            (
                (ANSC_HANDLE)pCurFolder,
                pRecordName
            );

    if ( !pCurRecord )
    {
        returnStatus = ANSC_STATUS_BAD_NAME;
    }
    else
    {
        returnStatus =
            pCurRecord->SetRenderAttr
                (
                    (ANSC_HANDLE)pCurRecord,
                    hRenderAttr
                );
    }

    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, FALSE);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysIroIraGetCurRecycleBin
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
SysIroIraGetCurRecycleBin
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    )
{
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pCurKey      = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder   = NULL;
    ANSC_HANDLE                     hRecycleBin  = NULL;

    if ( !pMyObject->bActive )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else if ( !pCurKey )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pCurFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pCurKey->hRepFolder;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, TRUE);
    hRecycleBin = pCurFolder->GetRecycleBin((ANSC_HANDLE)pCurFolder);
    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, TRUE);

    return  hRecycleBin;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysIroIraAttachRecycleBin
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
SysIroIraAttachRecycleBin
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ANSC_HANDLE                 hRecycleBin
    )
{
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject      = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pCurKey        = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder     = NULL;
    PSYS_RECYCLE_BIN_OBJECT         pCurRecycleBin = NULL;

    if ( !pMyObject->bActive )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else if ( !pCurKey )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pCurFolder     = (PSYS_REPOSITORY_FOLDER_OBJECT)pCurKey->hRepFolder;
        pCurRecycleBin = (PSYS_RECYCLE_BIN_OBJECT      )pCurFolder->GetRecycleBin((ANSC_HANDLE)pCurFolder);
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, FALSE);

        pCurFolder->SetRecycleBin
            (
                (ANSC_HANDLE)pCurFolder,
                hRecycleBin
            );

    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, FALSE);
    return  (ANSC_HANDLE)pCurRecycleBin;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysIroIraDetachRecycleBin
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
SysIroIraDetachRecycleBin
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder
    )
{
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject      = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pCurKey        = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder     = NULL;
    PSYS_RECYCLE_BIN_OBJECT         pCurRecycleBin = NULL;

    if ( !pMyObject->bActive )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else if ( !pCurKey )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pCurFolder     = (PSYS_REPOSITORY_FOLDER_OBJECT)pCurKey->hRepFolder;
        pCurRecycleBin = (PSYS_RECYCLE_BIN_OBJECT      )pCurFolder->GetRecycleBin((ANSC_HANDLE)pCurFolder);
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, FALSE);

        pCurFolder->SetRecycleBin
            (
                (ANSC_HANDLE)pCurFolder,
                (ANSC_HANDLE)NULL
            );
    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, FALSE);

    return  (ANSC_HANDLE)pCurRecycleBin;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroIraSortSubFolders
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
SysIroIraSortSubFolders
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ULONG                       ulSortingType,
        BOOL                        bAscending
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pCurKey      = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder   = NULL;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else if ( !pCurKey )
    {
        return  ANSC_STATUS_INVALID_HANDLE;
    }
    else
    {
        pCurFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pCurKey->hRepFolder;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, FALSE);

    switch ( ulSortingType )
    {
        case    SYS_REPOSITORY_SORTING_BYTIME :

                returnStatus =
                    pCurFolder->SortFolderByTime
                        (
                            (ANSC_HANDLE)pCurFolder,
                            bAscending
                        );

                break;

        case    SYS_REPOSITORY_SORTING_BYNAME :

                returnStatus =
                    pCurFolder->SortFolderByName
                        (
                            (ANSC_HANDLE)pCurFolder,
                            bAscending
                        );

                break;

        default :

                returnStatus = ANSC_STATUS_UNAPPLICABLE;

                break;
    }

    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, FALSE);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroIraSortRecords
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
SysIroIraSortRecords
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        ULONG                       ulSortingType,
        BOOL                        bAscending
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;
    PSYS_RFO_KEY                    pCurKey      = (PSYS_RFO_KEY                 )hCurFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder   = NULL;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else if ( !pCurKey )
    {
        return  ANSC_STATUS_INVALID_HANDLE;
    }
    else
    {
        pCurFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pCurKey->hRepFolder;
    }

    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject, FALSE);

    switch ( ulSortingType )
    {
        case    SYS_REPOSITORY_SORTING_BYTIME :

                returnStatus =
                    pCurFolder->SortRecordByTime
                        (
                            (ANSC_HANDLE)pCurFolder,
                            bAscending
                        );

                break;

        case    SYS_REPOSITORY_SORTING_BYNAME :

                returnStatus =
                    pCurFolder->SortRecordByName
                        (
                            (ANSC_HANDLE)pCurFolder,
                            bAscending
                        );

                break;

        default :

                returnStatus = ANSC_STATUS_UNAPPLICABLE;

                break;
    }

    pMyObject->RelAccess((ANSC_HANDLE)pMyObject, FALSE);

    return  returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysIroIraGetSysRamIf
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
SysIroIraGetSysRamIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;

    return  pMyObject->hSysRamIf;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroIraSetSysRamIf
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
SysIroIraSetSysRamIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_INFO_REPOSITORY_OBJECT     pMyObject    = (PSYS_INFO_REPOSITORY_OBJECT  )hThisObject;

    pMyObject->hSysRamIf = hInterface;

    return returnStatus;   
}
