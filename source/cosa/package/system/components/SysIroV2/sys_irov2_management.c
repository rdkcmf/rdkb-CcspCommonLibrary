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

    module:	sys_iro_management.c

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the data management functions 
        of the Sys Info Repository Object.

        *   SysIroV2AddFolder
        *   SysIroV2AddFolder2
        *   SysIroV2DelFolder
		*	SysIroV2DelFolder2
        *   SysIroV2ResetFolder
        *   SysIroV2GetFolderByName
        *   SysIroV2GetFolderByName2
        *   SysIroV2GetFolderByIndex
        *   SysIroV2AddRecord
        *   SysIroV2DelRecord
        *   SysIroV2SetRecord
        *   SysIroV2GetRecordByName
        *   SysIroV2GetRecordByIndex

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

        ANSC_HANDLE
        SysIroV2AddFolder
            (
                ANSC_HANDLE                 hThisObject,
                PSYS_IROV2_FOLDER_ENTRY     pCurFolder,
                char*                       pSubFolderName,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called to add a folder. The sub-folder name
        may be a path.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PSYS_IROV2_FOLDER_ENTRY     pCurFolder
                Specifies the current folder under which the new folder
                shall be created. This parameter could be NULL.

                char*                       pSubFolderName
                Specifies the folder name to be added.

                ANSC_HANDLE                 hReserved
                Specifies a folder-specific context handle.

    return:     handle of the new folder object.

**********************************************************************/

ANSC_HANDLE
SysIroV2AddFolder
    (
        ANSC_HANDLE                 hThisObject,
        PSYS_IROV2_FOLDER_ENTRY     pCurFolder,
        char*                       pSubFolderName,
        ANSC_HANDLE                 hReserved
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT   )hThisObject;
    PSYS_IROV2_FOLDER_ENTRY         pThisFolder  = (pCurFolder != NULL) ? pCurFolder : (PSYS_IROV2_FOLDER_ENTRY)pMyObject->hRootFolder;
    PSYS_IROV2_FOLDER_ENTRY         pNextFolder  = NULL;
    PANSC_TOKEN_CHAIN               pTokenChain  = NULL;
    PANSC_STRING_TOKEN              pTokenEntry  = NULL;

    AnscTrace("SysIroV2AddFolder -- %s %s  ...\n", pThisFolder->FolderName, pSubFolderName);

    pTokenChain =
        (PANSC_TOKEN_CHAIN)AnscTcAllocate
            (
                pSubFolderName,
                SYS_IROV2_PATH_SEPARATORS
            );

    if ( !pTokenChain )
    {
        pThisFolder = (PSYS_IROV2_FOLDER_ENTRY)NULL;

        goto  EXIT1;
    }
    else if ( AnscTcGetTokenCount(pTokenChain) == 0 )
    {
        pThisFolder = (PSYS_IROV2_FOLDER_ENTRY)NULL;

        goto  EXIT2;
    }

    /*
     * The specified folder name is the absolute path where the new folder should be created in the
     * repository tree. We first try to go as far as we can using specified folder name as the path
     * into tree, then creates a new branch based on the rest of the token chain.
     */
    SysIroV2FolderAcquireAccess(pThisFolder);

    while ( pTokenEntry = AnscTcUnlinkToken(pTokenChain) )
    {
        pNextFolder = 
            pMyObject->GetFolderByName2
                (
                    (ANSC_HANDLE)pMyObject,
                    pThisFolder,
                    pTokenEntry->Name
                );

        if ( pNextFolder )
        {
            SysIroV2FolderReleaseAccess(pThisFolder);

            AnscFreeMemory(pTokenEntry);
        }
        else
        {
            AnscTcPrefixToken(pTokenChain, pTokenEntry);

            break;
        }

        pThisFolder = pNextFolder;
    }

    if ( !pTokenEntry )
    {
        goto  EXIT2;
    }

    /*
     * We have found the Repository Folder Object whose name is the longest match for the specified
     * folder name. For each path segment that is left in the specified path name, we create a sub-
     * folder under each previously-created folder object starting with the current folder object.
     * While this approach has the benefit of saving other components from having to follow strict
     * layering rule by creating a folder every step along the way, it does introduce an ambiguous
     * situation: what folder-permission and folder-type shall we assign to the folders that are
     * created for the middle segments of the specified path? As you shall see below, we assign the
     * default permission and type to all the unspecified folders.
     */
    while ( pTokenEntry = AnscTcUnlinkToken(pTokenChain) )
    {
        pNextFolder =
            pMyObject->AddFolder2
                (
                    (ANSC_HANDLE)pMyObject,
                    pThisFolder,
                    pTokenEntry->Name,
                    (ANSC_HANDLE)NULL
                );

        if ( pNextFolder )
        {
            SysIroV2FolderReleaseAccess(pThisFolder);

            AnscFreeMemory(pTokenEntry);
        }
        else
        {
            AnscTcPrefixToken(pTokenChain, pTokenEntry);

            break;
        }

        pThisFolder = pNextFolder;
    }

    if ( pTokenEntry )
    {
        SysIroV2FolderReleaseAccess(pThisFolder);

        pThisFolder = (PSYS_IROV2_FOLDER_ENTRY)NULL;
    }

    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT2:

    AnscTcFree((ANSC_HANDLE)pTokenChain);

EXIT1:

    if ( pThisFolder == NULL )
    {
        AnscTrace("SysIroV2AddFolder exits -- failed to create the folder!\n");
    }

    return  (ANSC_HANDLE)pThisFolder;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysIroV2AddFolder2
            (
                ANSC_HANDLE                 hThisObject,
                PSYS_IROV2_FOLDER_ENTRY     pCurFolder,
                char*                       pSubFolderName,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called to add a folder under the current folder.
        The sub-folder name doesn't contain a path.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PSYS_IROV2_FOLDER_ENTRY     pCurFolder
                Specifies the current folder under which the new folder
                shall be created. This parameter could be NULL.

                char*                       pSubFolderName
                Specifies the folder name to be added.

                ANSC_HANDLE                 hReserved
                Specifies a folder-specific context handle.

    return:     handle of the new folder object.

**********************************************************************/

ANSC_HANDLE
SysIroV2AddFolder2
    (
        ANSC_HANDLE                 hThisObject,
        PSYS_IROV2_FOLDER_ENTRY     pCurFolder,
        char*                       pSubFolderName,
        ANSC_HANDLE                 hReserved
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT   )hThisObject;
    PSYS_IROV2_FOLDER_ENTRY         pNextFolder  = NULL;
    errno_t   rc = -1;

    pNextFolder =
        (PSYS_IROV2_FOLDER_ENTRY)
            AnscAllocateMemory
                (
                    sizeof(SYS_IROV2_FOLDER_ENTRY) + _ansc_strlen(pSubFolderName) + 1
                );

    if ( pNextFolder )
    {
        SysIroV2InitFolderEntry(pNextFolder);

        pNextFolder->hParentFolder  = (ANSC_HANDLE)pCurFolder;
        pNextFolder->UserReserved   = (ULONG)hReserved;
        rc = STRCPY_S_NOCLOBER(pNextFolder->FolderName, sizeof(pNextFolder->FolderName), pSubFolderName);
        ERR_CHK(rc);

        SysIroV2FolderAcquireAccess(pCurFolder);
        AnscQueuePushEntry         (&pCurFolder->SubFolderQueue, &pNextFolder->Linkage);
        SysIroV2FolderReleaseAccess(pCurFolder);

        SysIroV2FolderAcquireAccess(pNextFolder);
    }

    return  pNextFolder;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroV2DelFolder
            (
                ANSC_HANDLE                 hThisObject,
                PSYS_IROV2_FOLDER_ENTRY     pCurFolder,
                char*                       pSubFolderName
            );

    description:

        This function is called to delete a folder entry.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder under which the specified
                folder shall be deleted. This parameter could be NULL.

                char*                       pSubFolderName
                Specifies the folder name to be deleted, this parameter
                MUST not be NULL.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysIroV2DelFolder
    (
        ANSC_HANDLE                 hThisObject,
        PSYS_IROV2_FOLDER_ENTRY     pCurFolder,
        char*                       pSubFolderName
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT   )hThisObject;
    PSYS_IROV2_FOLDER_ENTRY         pThisFolder  = (pCurFolder != NULL) ? pCurFolder : (PSYS_IROV2_FOLDER_ENTRY)pMyObject->hRootFolder;
    PSYS_IROV2_FOLDER_ENTRY         pNextFolder  = NULL;

    AnscTrace("SysIroV2DelFolder -- %s %s  ...\n", pThisFolder->FolderName, pSubFolderName);

    pNextFolder =
        (PSYS_IROV2_FOLDER_ENTRY)pMyObject->GetFolderByName
            (
                (ANSC_HANDLE)pMyObject,
                pThisFolder,
                pSubFolderName
            );

    if ( !pNextFolder )
    {
        return  ANSC_STATUS_BAD_NAME;
    }

    if ( !SysIroV2IsFolderEntryRemovable(pNextFolder) )
    {
        SysIroV2FolderReleaseAccess(pNextFolder);

        AnscTrace("SysIroV2DelFolder -- the folder is not removable, RefCount = %lu...\n", pNextFolder->RefCount);

        return  ANSC_STATUS_ACCESS_DENIED;
    }
    else
    {
        pThisFolder = (PSYS_IROV2_FOLDER_ENTRY)pNextFolder->hParentFolder;

        SysIroV2FolderReleaseAccess(pNextFolder);

        if ( !pThisFolder )
        {
            return  ANSC_STATUS_ACCESS_DENIED;
        }
    }

    if ( TRUE )
    {
        SysIroV2FolderAcquireAccess(pThisFolder);
        AnscQueuePopEntryByLink    (&pThisFolder->SubFolderQueue, &pNextFolder->Linkage);
        SysIroV2FolderReleaseAccess(pThisFolder);

        pMyObject->DelFolder2      ((ANSC_HANDLE)pMyObject, pNextFolder);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroV2DelFolder2
            (
                ANSC_HANDLE                 hThisObject,
                PSYS_IROV2_FOLDER_ENTRY     pCurFolder
            );

    description:

        This function is called to delete a folder entry.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder under which the specified
                folder shall be deleted. This parameter could be NULL.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysIroV2DelFolder2
    (
        ANSC_HANDLE                 hThisObject,
        PSYS_IROV2_FOLDER_ENTRY     pCurFolder
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT   )hThisObject;
    PSYS_IROV2_FOLDER_ENTRY         pThisFolder  = pCurFolder;
    PSINGLE_LINK_ENTRY              pSListEntry  = NULL;
    PSYS_IROV2_FOLDER_ENTRY         pNextFolder  = NULL;
    PSYS_IROV2_RECORD_ENTRY         pRecord      = NULL;

    AnscTrace("SysIroV2DelFolder2 -- %s...\n", pThisFolder->FolderName);

    SysIroV2FolderAcquireAccess(pThisFolder);

    /*
     *  remove all records
     */
    pSListEntry = AnscQueuePopEntry(&pThisFolder->RecordQueue);

    while ( pSListEntry )
    {
        pRecord     = ACCESS_SYS_IROV2_RECORD_ENTRY(pSListEntry);
        pSListEntry = AnscQueuePopEntry            (&pThisFolder->RecordQueue);

        AnscTrace("SysIroV2DelFolder2 -- removing record %s.\n", pRecord->pRecordName);

        AnscFreeMemory(pRecord);
    }

    /*
     *  remove all sub-folders
     */
    pSListEntry = AnscQueuePopEntry(&pThisFolder->SubFolderQueue);

    while ( pSListEntry )
    {
        pNextFolder = ACCESS_SYS_IROV2_FOLDER_ENTRY(pSListEntry);
        pSListEntry = AnscQueuePopEntry            (&pThisFolder->SubFolderQueue);

        pMyObject->DelFolder2((ANSC_HANDLE)pMyObject, pNextFolder);
    }

    SysIroV2FolderReleaseAccess(pThisFolder);
    SysIroV2FreeFolderEntry    (pThisFolder);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroV2ResetFolder
            (
                ANSC_HANDLE                 hThisObject,
                PSYS_IROV2_FOLDER_ENTRY     pCurFolder
            );

    description:

        This function is called to delete a folder entry.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder under which the specified
                folder shall be deleted. This parameter could be NULL.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysIroV2ResetFolder
    (
        ANSC_HANDLE                 hThisObject,
        PSYS_IROV2_FOLDER_ENTRY     pCurFolder
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT   )hThisObject;
    PSYS_IROV2_FOLDER_ENTRY         pThisFolder  = pCurFolder;
    PSINGLE_LINK_ENTRY              pSListEntry  = NULL;
    PSYS_IROV2_FOLDER_ENTRY         pNextFolder  = NULL;
    PSYS_IROV2_RECORD_ENTRY         pRecord      = NULL;

    AnscTrace("SysIroV2ResetFolder -- %s...\n", pThisFolder->FolderName);

    /*
     *  remove all records
     */
    pSListEntry = AnscQueuePopEntry(&pThisFolder->RecordQueue);

    while ( pSListEntry )
    {
        pRecord     = ACCESS_SYS_IROV2_RECORD_ENTRY(pSListEntry);
        pSListEntry = AnscQueuePopEntry            (&pThisFolder->RecordQueue);

        AnscTrace("SysIroV2DelFolder2 -- removing record %s.\n", pRecord->pRecordName);

        AnscFreeMemory(pRecord);
    }

    /*
     *  remove all sub-folders
     */
    pSListEntry = AnscQueuePopEntry(&pThisFolder->SubFolderQueue);

    while ( pSListEntry )
    {
        pNextFolder = ACCESS_SYS_IROV2_FOLDER_ENTRY(pSListEntry);
        pSListEntry = AnscQueuePopEntry            (&pThisFolder->SubFolderQueue);

        pMyObject->DelFolder2((ANSC_HANDLE)pMyObject, pNextFolder);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysIroV2GetFolderByName
            (
                ANSC_HANDLE                 hThisObject,
                PSYS_IROV2_FOLDER_ENTRY     pCurFolder,
                char*                       pSubFolderName
            );

    description:

        This function is called to retrieve a folder entry by matching
        the folder name, which includes the path.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder under which the specified
                folder shall be deleted. This parameter could be NULL.

                char*                       pSubFolderName
                Specifies the folder name to be deleted, this parameter
                MUST not be NULL.

    return:     handle of the matched folder object.

**********************************************************************/

ANSC_HANDLE
SysIroV2GetFolderByName
    (
        ANSC_HANDLE                 hThisObject,
        PSYS_IROV2_FOLDER_ENTRY     pCurFolder,
        char*                       pSubFolderName
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT   )hThisObject;
    PSYS_IROV2_FOLDER_ENTRY         pThisFolder  = (pCurFolder != NULL) ? pCurFolder : (PSYS_IROV2_FOLDER_ENTRY)pMyObject->hRootFolder;
    PSYS_IROV2_FOLDER_ENTRY         pNextFolder  = NULL;
    PANSC_TOKEN_CHAIN               pTokenChain  = NULL;
    PANSC_STRING_TOKEN              pTokenEntry  = NULL;

    AnscTrace("SysIroV2GetFolderByName -- %s %s  ...\n", pThisFolder->FolderName, pSubFolderName);

    pTokenChain =
        (PANSC_TOKEN_CHAIN)AnscTcAllocate
            (
                pSubFolderName,
                SYS_IROV2_PATH_SEPARATORS
            );

    if ( !pTokenChain )
    {
        pThisFolder = (PSYS_IROV2_FOLDER_ENTRY)NULL;

        goto  EXIT1;
    }
    else if ( AnscTcGetTokenCount(pTokenChain) == 0 )
    {
        pThisFolder = (PSYS_IROV2_FOLDER_ENTRY)NULL;

        goto  EXIT2;
    }

    /*
     * The specified folder name is the absolute path where the new folder should be created in the
     * repository tree. We first try to go as far as we can using specified folder name as the path
     * into tree, then creates a new branch based on the rest of the token chain.
     */
    SysIroV2FolderAcquireAccess(pThisFolder);

    while ( pTokenEntry = AnscTcUnlinkToken(pTokenChain) )
    {
        pNextFolder = 
            pMyObject->GetFolderByName2
                (
                    (ANSC_HANDLE)pMyObject,
                    pThisFolder,
                    pTokenEntry->Name
                );

        if ( pNextFolder )
        {
            SysIroV2FolderReleaseAccess(pThisFolder);

            AnscFreeMemory(pTokenEntry);
        }
        else
        {
            AnscTcPrefixToken(pTokenChain, pTokenEntry);

            break;
        }

        pThisFolder = pNextFolder;
    }

    if ( pTokenEntry )
    {
        SysIroV2FolderReleaseAccess(pThisFolder);

        pThisFolder = (PSYS_IROV2_FOLDER_ENTRY)NULL;
    }

    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT2:

    AnscTcFree((ANSC_HANDLE)pTokenChain);

EXIT1:

    return  (ANSC_HANDLE)pThisFolder;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysIroV2GetFolderByName2
            (
                ANSC_HANDLE                 hThisObject,
                PSYS_IROV2_FOLDER_ENTRY     pCurFolder,
                char*                       pSubFolderName
            );

    description:

        This function is called to retrieve a folder entry by matching
        the folder name, which includes the path.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder under which the specified
                folder shall be deleted. This parameter could be NULL.

                char*                       pSubFolderName
                Specifies the folder name to be deleted, this parameter
                MUST not be NULL.

    return:     handle of the matched folder object.

**********************************************************************/

ANSC_HANDLE
SysIroV2GetFolderByName2
    (
        ANSC_HANDLE                 hThisObject,
        PSYS_IROV2_FOLDER_ENTRY     pCurFolder,
        char*                       pSubFolderName
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT   )hThisObject;
    PSYS_IROV2_FOLDER_ENTRY         pThisFolder  = pCurFolder;
    PSINGLE_LINK_ENTRY              pSListEntry  = NULL;
    PSYS_IROV2_FOLDER_ENTRY         pNextFolder  = NULL;

    /*AnscTrace("SysIroV2GetFolderByName2 -- %s %s...\n", pThisFolder->FolderName, pSubFolderName);*/

    pSListEntry = AnscQueueGetFirstEntry(&pThisFolder->SubFolderQueue);

    while ( pSListEntry )
    {
        pNextFolder = ACCESS_SYS_IROV2_FOLDER_ENTRY(pSListEntry);
        pSListEntry = AnscQueueGetNextEntry        (pSListEntry);

        if ( _ansc_strcmp(pNextFolder->FolderName, pSubFolderName) == 0 )
        {
            SysIroV2FolderAcquireAccess(pNextFolder);

            goto  EXIT;
        }
    }

    pNextFolder = NULL;

EXIT:

    if ( pNextFolder == NULL )
    {
        AnscTrace("SysIroV2GetFolderByName2 exits -- cannot find the folder %s %s!\n", pThisFolder->FolderName, pSubFolderName);
    }

    return  (ANSC_HANDLE)pNextFolder;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysIroV2GetFolderByIndex
            (
                ANSC_HANDLE                 hThisObject,
                PSYS_IROV2_FOLDER_ENTRY     pCurFolder,
                ULONG                       ulIndex
            );

    description:

        This function is called to retrieve a folder entry by matching
        the folder index.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder under which the specified
                folder shall be deleted. This parameter could be NULL.

                ULONG                       ulIndex
                Specifies the folder index to match.

    return:     handle of the matched folder object.

**********************************************************************/

ANSC_HANDLE
SysIroV2GetFolderByIndex
    (
        ANSC_HANDLE                 hThisObject,
        PSYS_IROV2_FOLDER_ENTRY     pCurFolder,
        ULONG                       ulIndex
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT   )hThisObject;
    PSYS_IROV2_FOLDER_ENTRY         pThisFolder  = pCurFolder;
    PSINGLE_LINK_ENTRY              pSListEntry  = NULL;
    PSYS_IROV2_FOLDER_ENTRY         pNextFolder  = NULL;
    ULONG                           ulCount      = 0;

    /*AnscTrace("SysIroV2GetFolderByIndex -- %s %lu...\n", pThisFolder->FolderName, ulIndex);*/

    pSListEntry = AnscQueueGetFirstEntry(&pThisFolder->SubFolderQueue);

    while ( pSListEntry )
    {
        pNextFolder = ACCESS_SYS_IROV2_FOLDER_ENTRY(pSListEntry);
        pSListEntry = AnscQueueGetNextEntry        (pSListEntry);

        if ( ulCount == ulIndex )
        {
            SysIroV2FolderAcquireAccess(pNextFolder);

            goto  EXIT;
        }
        else
        {
            ulCount++;
        }
    }

    pNextFolder = NULL;

EXIT:

    if ( pNextFolder == NULL )
    {
        AnscTrace("SysIroV2GetFolderByIndex exits -- cannot find the folder %s %lu!\n", pThisFolder->FolderName, ulIndex);
    }

    return  (ANSC_HANDLE)pNextFolder;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

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

                ULONG                       ulContentType
                Specifies the content type to be created.

                PVOID                       pWriteBuffer
                Specifies a record data value to be saved.

                ULONG                       ulWriteSize
                Specifies the size of the record data buffer.

    return:     status of operation.

**********************************************************************/

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
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT   )hThisObject;
    PSYS_IROV2_FOLDER_ENTRY         pThisFolder  = pCurFolder;
    PSYS_IROV2_RECORD_ENTRY         pRecord      = NULL;
    PSYS_IROV2_RECORD_DATA          pRecordData  = NULL;
    PSYS_IROV2_RECORD_ENTRY         pNewRecord   = NULL;
    BOOLEAN                         bNewRecord   = FALSE;
    errno_t    rc  = -1;

    AnscTrace("SysIroV2AddRecord -- %s %s  ...\n", pThisFolder->FolderName, pRecordName);

    SysIroV2FolderAcquireAccess(pThisFolder);

    pRecord = 
        (PSYS_IROV2_RECORD_ENTRY)pMyObject->GetRecordByName
            (
                (ANSC_HANDLE)pMyObject,
                pThisFolder,
                pRecordName
            );

    if ( !pRecord )
    {
        pRecord = 
            (PSYS_IROV2_RECORD_ENTRY)AnscAllocateMemory
                (
                    sizeof(SYS_IROV2_RECORD_ENTRY) + _ansc_strlen(pRecordName) + 1 + ulWriteSize
                );

        if ( pRecord == NULL )
        {
            returnStatus = ANSC_STATUS_RESOURCES;

            goto  EXIT;
        }
        else
        {
            bNewRecord = TRUE;
            SysRepositoryInitRecordEntry(pRecord);
        }
    }
    else if ( pRecord->ulDataSize < ulWriteSize )
    {
        /*
         *  replace the existing record with a newly allocated one
         */
        pNewRecord = 
            (PSYS_IROV2_RECORD_ENTRY)AnscAllocateMemory
                (
                    sizeof(SYS_IROV2_RECORD_ENTRY) + _ansc_strlen(pRecordName) + 1 + ulWriteSize
                );

        if ( pNewRecord == NULL )
        {
            returnStatus = ANSC_STATUS_RESOURCES;

            goto  EXIT;
        }
        else
        {
            SysRepositoryInitRecordEntry(pNewRecord);

            pNewRecord->hUserContext    = pRecord->hUserContext;
            pNewRecord->UserReserved    = pRecord->UserReserved;

            AnscQueuePushEntryBefore(&pThisFolder->RecordQueue, &pNewRecord->Linkage, &pRecord->Linkage);
            AnscQueuePopEntryByLink (&pThisFolder->RecordQueue, &pRecord->Linkage);
            SysIroV2FreeRecordEntry (pRecord);
            pRecord = pNewRecord;
        }
    }

    if ( TRUE )
    {
        pRecord->RecordType     = ulRecordType;
        pRecord->ContentType    = ulContentType;

        pRecord->ulDataSize     = ulWriteSize;
        pRecord->pRecordName    = &pRecord->DataAndName[ulWriteSize];
        rc = STRCPY_S_NOCLOBER(pRecord->pRecordName, sizeof(pRecord->pRecordName), pRecordName);
        ERR_CHK(rc);

        _ansc_memcpy(pRecord->DataAndName, pWriteBuffer, ulWriteSize);
    }

    if ( bNewRecord )
    {
        AnscQueuePushEntry(&pThisFolder->RecordQueue, &pRecord->Linkage);
    }

    returnStatus = ANSC_STATUS_SUCCESS;

EXIT:

    SysIroV2FolderReleaseAccess(pThisFolder);

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        AnscTrace("SysIroV2AddRecord exits -- %s %s failed, status = %d!\n", pThisFolder->FolderName, pRecordName, returnStatus);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysIroV2DelRecord
            (
                ANSC_HANDLE                 hThisObject,
                PSYS_IROV2_FOLDER_ENTRY     pCurFolder,
                char*                       pRecordName
            );

    description:

        This function is called to delete a repository record.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder under which the new record
                shall be added. This parameter MUST not be NULL.

                char*                       pRecordName
                Specifies the record name to be added, this parameter
                MUST not be NULL.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysIroV2DelRecord
    (
        ANSC_HANDLE                 hThisObject,
        PSYS_IROV2_FOLDER_ENTRY     pCurFolder,
        char*                       pRecordName
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT   )hThisObject;
    PSYS_IROV2_FOLDER_ENTRY         pThisFolder  = pCurFolder;
    PSYS_IROV2_RECORD_ENTRY         pRecord      = NULL;

    AnscTrace("SysIroV2DelRecord -- %s %s  ...\n", pThisFolder->FolderName, pRecordName);

    SysIroV2FolderAcquireAccess(pThisFolder);

    pRecord = 
        (PSYS_IROV2_RECORD_ENTRY)pMyObject->GetRecordByName
            (
                (ANSC_HANDLE)pMyObject,
                pThisFolder,
                pRecordName
            );

    if ( !pRecord )
    {
        returnStatus = ANSC_STATUS_CANT_FIND;

        goto  EXIT;
    }
    else
    {
        AnscQueuePopEntryByLink (&pThisFolder->RecordQueue, &pRecord->Linkage);
        SysIroV2FreeRecordEntry (pRecord);

        returnStatus = ANSC_STATUS_SUCCESS;
    }

EXIT:

    SysIroV2FolderReleaseAccess(pThisFolder);
    AnscTrace("SysIroV2DelRecord exits -- status = %d.\n", returnStatus);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

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

    description:

        This function is called to set a repository record.

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

                ULONG                       ulContentType
                Specifies the content type to be created.

                PVOID                       pWriteBuffer
                Specifies a record data value to be saved.

                ULONG                       ulWriteSize
                Specifies the size of the record data buffer.

    return:     status of operation.

**********************************************************************/

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
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT   )hThisObject;
    PSYS_IROV2_FOLDER_ENTRY         pThisFolder  = pCurFolder;
    PSYS_IROV2_RECORD_ENTRY         pRecord      = NULL;
    PSYS_IROV2_RECORD_DATA          pRecordData  = NULL;
    PSYS_IROV2_RECORD_ENTRY         pNewRecord   = NULL;
    errno_t   rc = -1;

    AnscTrace("SysIroV2SetRecord -- %s %s  ...\n", pThisFolder->FolderName, pRecordName);

    SysIroV2FolderAcquireAccess(pThisFolder);

    pRecord = 
        (PSYS_IROV2_RECORD_ENTRY)pMyObject->GetRecordByName
            (
                (ANSC_HANDLE)pMyObject,
                pThisFolder,
                pRecordName
            );

    if ( !pRecord )
    {
        returnStatus = ANSC_STATUS_CANT_FIND;

        goto  EXIT;
    }
    else if ( pRecord->ulDataSize < ulWriteSize )
    {
        /*
         *  replace the existing record with a newly allocated one
         */
        pNewRecord = 
            (PSYS_IROV2_RECORD_ENTRY)AnscAllocateMemory
                (
                    sizeof(SYS_IROV2_RECORD_ENTRY) + _ansc_strlen(pRecordName) + 1 + ulWriteSize
                );

        if ( pNewRecord == NULL )
        {
            returnStatus = ANSC_STATUS_RESOURCES;

            goto  EXIT;
        }
        else
        {
            SysRepositoryInitRecordEntry(pNewRecord);

            pNewRecord->hUserContext    = pRecord->hUserContext;
            pNewRecord->UserReserved    = pRecord->UserReserved;

            AnscQueuePushEntryBefore(&pThisFolder->RecordQueue, &pNewRecord->Linkage, &pRecord->Linkage);
            AnscQueuePopEntryByLink (&pThisFolder->RecordQueue, &pRecord->Linkage);
            SysIroV2FreeRecordEntry (pRecord);
            pRecord = pNewRecord;
        }
    }

    if ( TRUE )
    {
        pRecord->RecordType     = ulRecordType;
        pRecord->ContentType    = ulContentType;

        pRecord->ulDataSize     = ulWriteSize;
        pRecord->pRecordName    = &pRecord->DataAndName[ulWriteSize];
        rc = STRCPY_S_NOCLOBER(pRecord->pRecordName, sizeof(pRecord->pRecordName), pRecordName);
        ERR_CHK(rc);

        _ansc_memcpy(pRecord->DataAndName, pWriteBuffer, ulWriteSize);
    }

    returnStatus = ANSC_STATUS_SUCCESS;

EXIT:

    SysIroV2FolderReleaseAccess(pThisFolder);
    AnscTrace("SysIroV2SetRecord exits -- status = %d.\n", returnStatus);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysIroV2GetRecordByName
            (
                ANSC_HANDLE                 hThisObject,
                PSYS_IROV2_FOLDER_ENTRY     pCurFolder,
                char*                       pRecordName
            );

    description:

        This function is called to retrieve a repository record by
        matching the record name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder under which the new record
                shall be added. This parameter MUST not be NULL.

                char*                       pRecordName
                Specifies the record name to be added, this parameter
                MUST not be NULL.

    return:     handle of matched record.

**********************************************************************/

ANSC_HANDLE
SysIroV2GetRecordByName
    (
        ANSC_HANDLE                 hThisObject,
        PSYS_IROV2_FOLDER_ENTRY     pCurFolder,
        char*                       pRecordName
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT   )hThisObject;
    PSYS_IROV2_FOLDER_ENTRY         pThisFolder  = pCurFolder;
    PSINGLE_LINK_ENTRY              pSListEntry  = NULL;
    PSYS_IROV2_RECORD_ENTRY         pRecord      = NULL;

    /*AnscTrace("SysIroV2GetRecordByName -- %s %s  ...\n", pThisFolder->FolderName, pRecordName);*/

    SysIroV2FolderAcquireAccess(pThisFolder);

    pSListEntry = AnscQueueGetFirstEntry(&pThisFolder->RecordQueue);

    while ( pSListEntry )
    {
        pRecord     = ACCESS_SYS_IROV2_RECORD_ENTRY(pSListEntry);
        pSListEntry = AnscQueueGetNextEntry        (pSListEntry);

        if ( _ansc_strcmp(pRecord->pRecordName, pRecordName) == 0 )
        {
            goto  EXIT;
        }
    }

    pRecord = NULL;

EXIT:

    SysIroV2FolderReleaseAccess(pThisFolder);

    if ( pRecord == NULL )
    {
        AnscTrace("SysIroV2GetRecordByName exits -- cannot find the record %s %s!\n", pThisFolder->FolderName, pRecordName);
    }

    return  (ANSC_HANDLE)pRecord;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysIroV2GetRecordByIndex
            (
                ANSC_HANDLE                 hThisObject,
                PSYS_IROV2_FOLDER_ENTRY     pCurFolder,
                ULONG                       ulIndex
            );

    description:

        This function is called to retrieve a repository record by
        matching the record index.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder under which the new record
                shall be added. This parameter MUST not be NULL.

                ULONG                       ulIndex
                Specifies the record index to be matched.

    return:     handle of matched record.

**********************************************************************/

ANSC_HANDLE
SysIroV2GetRecordByIndex
    (
        ANSC_HANDLE                 hThisObject,
        PSYS_IROV2_FOLDER_ENTRY     pCurFolder,
        ULONG                       ulIndex
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_IROV2_OBJECT               pMyObject    = (PSYS_IROV2_OBJECT   )hThisObject;
    PSYS_IROV2_FOLDER_ENTRY         pThisFolder  = pCurFolder;
    PSINGLE_LINK_ENTRY              pSListEntry  = NULL;
    PSYS_IROV2_RECORD_ENTRY         pRecord      = NULL;
    ULONG                           ulCount      = 0;

    /*AnscTrace("SysIroV2GetRecordByIndex -- %s %lu  ...\n", pThisFolder->FolderName, ulIndex);*/

    SysIroV2FolderAcquireAccess(pThisFolder);

    pSListEntry = AnscQueueGetFirstEntry(&pThisFolder->RecordQueue);

    while ( pSListEntry )
    {
        pRecord     = ACCESS_SYS_IROV2_RECORD_ENTRY(pSListEntry);
        pSListEntry = AnscQueueGetNextEntry        (pSListEntry);

        if ( ulCount == ulIndex )
        {
            goto  EXIT;
        }
        else
        {
            ulCount ++;
        }
    }

    pRecord = NULL;

EXIT:

    SysIroV2FolderReleaseAccess(pThisFolder);

    if ( pRecord == NULL )
    {
        AnscTrace("SysIroV2GetRecordByIndex exits -- cannot find record %s %lu!\n", pThisFolder->FolderName, ulIndex);
    }

    return  (ANSC_HANDLE)pRecord;
}

