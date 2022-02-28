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

    module:	sys_rfo_access.c

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced field-access functions
        of the Sys Repository Folder Object.

        *   SysRfoQueryAbsPath
        *   SysRfoIsRemovable
        *   SysRfoGrantAccess
        *   SysRfoGetFolderCount
        *   SysRfoGetRecordCount
        *   SysRfoAddFolder
        *   SysRfoDelFolder
        *   SysRfoGetFolderByIndex
        *   SysRfoGetFolderByName
        *   SysRfoAddRecord
        *   SysRfoDelRecord
        *   SysRfoGetRecordByIndex
        *   SysRfoGetRecordByName
        *   SysRfoSetRecord
        *   SysRfoGenerateContent
        *   SysRfoPopulateContent
        *   SysRfoOpenFirst
        *   SysRfoCloseLast

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/05/02    initial revision.

**********************************************************************/


#include "sys_rfo_global.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        SysRfoQueryAbsPath
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pPathString
            );

    description:

        This function is called to retrieve the absolute repository
        path name for this folder object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pPathString
                Specifies the folder path string to be filled. This
                parameter could be NULL.

    return:     absoulte folder path.

**********************************************************************/

char*
SysRfoQueryAbsPath
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pPathString
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject     = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_FOLDER_PROPERTY pProperty     = (PSYS_REPOSITORY_FOLDER_PROPERTY)&pMyObject->Property;
    PSYS_REPOSITORY_FOLDER_OBJECT   pParentFolder = (PSYS_REPOSITORY_FOLDER_OBJECT  )pMyObject->hParentFolder;
    char*                           pFolderPath   = pMyObject->AbsolutePath;
    ULONG                           ulPathLen     = 0;
    errno_t   rc = -1;

    if ( !pFolderPath )
    {
        pMyObject->AbsolutePath = (char*)AnscAllocateMemory(ANSC_MAX_STRING_SIZE);
        pFolderPath             = pMyObject->AbsolutePath;

        if ( !pFolderPath )
        {
            return  NULL;
        }
    }
    else
    {
        AnscZeroMemory(pFolderPath, ANSC_MAX_STRING_SIZE);
    }

    if ( pParentFolder )
    {
            pParentFolder->QueryAbsPath
                (
                    (ANSC_HANDLE)pParentFolder,
                    pFolderPath
                );
    }

    ulPathLen                = AnscSizeOfString(pFolderPath);
    pFolderPath[ulPathLen++] = '/';

    rc = strcpy_s(&pFolderPath[ulPathLen], ANSC_MAX_STRING_SIZE, pProperty->FolderName);
    if(rc != EOK)
    {
        ERR_CHK(rc);
    }

    ulPathLen                = AnscSizeOfString(pFolderPath);
    pFolderPath[ulPathLen++] = 0;

    if ( pPathString )
    {
        rc = strcpy_s(pPathString, ANSC_MAX_STRING_SIZE, pFolderPath);
        if(rc != EOK)
        {
            ERR_CHK(rc);
        }

        if ( pMyObject->AbsolutePath )
        {
            AnscFreeMemory(pMyObject->AbsolutePath);

            pMyObject->AbsolutePath = NULL;
        }

        pFolderPath = pPathString;
    }

    return  pFolderPath;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SysRfoIsRemovable
            (
                ANSC_HANDLE                 hThisObject,
                BOOL                        bRecursive
            );

    description:

        This function is called to check whether the folder object
        can be removed safely.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                BOOL                        bRecursive
                Specifies whether the operation should be carried out
                recursively.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
SysRfoIsRemovable
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bRecursive
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_FOLDER_OBJECT   pSubFolder   = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    BOOL                            bRemovable   = TRUE;
    ULONG                           i            = 0;

    if ( pMyObject->RefCount != 0 )
    {
        return  FALSE;
    }
    else if ( pMyObject->GetFolderCount((ANSC_HANDLE)pMyObject) == 0 )
    {
        return  TRUE;
    }
    else if ( !bRecursive )
    {
        return  FALSE;
    }

    AnscAcquireLock(&pMyObject->FolderTableLock);

    for ( i = 0; i < SYS_RFO_RFO_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->FolderTable[i]);

        while ( pSLinkEntry )
        {
            pSubFolder  = ACCESS_SYS_REPOSITORY_FOLDER_OBJECT(pSLinkEntry);
            pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

            pSubFolder->AcquireAccess((ANSC_HANDLE)pSubFolder);
            bRemovable = pSubFolder->IsRemovable((ANSC_HANDLE)pSubFolder, bRecursive);
            pSubFolder->ReleaseAccess((ANSC_HANDLE)pSubFolder);

            if ( !bRemovable )
            {
                AnscReleaseLock(&pMyObject->FolderTableLock);

                return  FALSE;
            }
        }
    }

    AnscReleaseLock(&pMyObject->FolderTableLock);

    return  TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SysRfoGrantAccess
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulAccessMode
            );

    description:

        This function is called to grant access to a request.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulAccessMode
                Specifies the access mode associated with the request.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
SysRfoGrantAccess
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulAccessMode
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_FOLDER_PROPERTY pProperty    = (PSYS_REPOSITORY_FOLDER_PROPERTY)&pMyObject->Property;
    ULONG                           ulTmpMode    = ulAccessMode;

    if ( !(ulAccessMode & SYS_RFO_ACCESS_MODE_ADAPTIVE) )
    {
        ulTmpMode  = ulAccessMode & 0x0000FFFF;
        ulTmpMode &= pProperty->Permission;

        if ( ulTmpMode != (ulAccessMode & 0x0000FFFF) )
        {
            return  FALSE;
        }
    }

    if ( pMyObject->RefCount == 0 )
    {
        return  TRUE;
    }
    else if ( (ulAccessMode & SYS_RFO_ACCESS_MODE_EXCLUSIVE) != 0 )
    {
        return  FALSE;
    }
    else
    {
        /*
         * We should remove this "else" clause if the idea is to allow only one write/create/sync
         * operation at any time. By returning TRUE, we DON'T protect simultaneous read and write
         * access to the same folder.
         */
        return  TRUE;
    }

    return  FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SysRfoGetFolderCount
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the number of sub-folders.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     number of sub-folders.

**********************************************************************/

ULONG
SysRfoGetFolderCount
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject     = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    ULONG                           ulFolderCount = 0;
    ULONG                           i             = 0;

    AnscAcquireLock(&pMyObject->FolderTableLock);

    for ( i = 0; i < SYS_RFO_RFO_TABLE_SIZE; i++ )
    {
        ulFolderCount += AnscQueueQueryDepth(&pMyObject->FolderTable[i]);
    }

    AnscReleaseLock(&pMyObject->FolderTableLock);

    return  ulFolderCount;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SysRfoGetRecordCount
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the number of records.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     number of records.

**********************************************************************/

ULONG
SysRfoGetRecordCount
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject     = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    ULONG                           ulRecordCount = 0;
    ULONG                           i             = 0;

    AnscAcquireLock(&pMyObject->RecordTableLock);

    for ( i = 0; i < SYS_RFO_RRO_TABLE_SIZE; i++ )
    {
        ulRecordCount += AnscQueueQueryDepth(&pMyObject->RecordTable[i]);
    }

    AnscReleaseLock(&pMyObject->RecordTableLock);

    return  ulRecordCount;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

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

    description:

        This function is called to add a folder object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pFolderName
                Specifies the folder name to be added.

                ULONG                       ulPermission
                Specifies the permission of the folder to be added.

                ULONG                       ulType
                Specifies the folder type to be added.

                ANSC_HANDLE                 hRenderAttr
                Specifies the render attribute associated with the
                folder to be added.

                ANSC_HANDLE                 hReserved
                Specifies a folder-specific context handle.

    return:     handle of the new folder object.

**********************************************************************/

ANSC_HANDLE
SysRfoAddFolder
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pFolderName,
        ULONG                       ulPermission,
        ULONG                       ulFolderType,
        ANSC_HANDLE                 hRenderAttr,
        ANSC_HANDLE                 hReserved
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_FOLDER_PROPERTY pProperty    = (PSYS_REPOSITORY_FOLDER_PROPERTY)&pMyObject->Property;
    PSYS_RCA_INTERFACE              pSelfRcaIf   = (PSYS_RCA_INTERFACE             )pMyObject->hSelfRcaIf;
    PSYS_RCM_INTERFACE              pSelfRcmIf   = (PSYS_RCM_INTERFACE             )pMyObject->hSelfRcmIf;
    PSYS_REPOSITORY_FOLDER_OBJECT   pSubFolder   = NULL;
    ULONG                           ulHashIndex  = AnscHashString(pFolderName, AnscSizeOfString(pFolderName), SYS_RFO_RFO_TABLE_SIZE);

    if ( AnscSizeOfString(pFolderName) >= SYS_MAX_FOLDER_NAME_SIZE )
    {
        return  (ANSC_HANDLE)NULL;
    }

    pSubFolder =
        (PSYS_REPOSITORY_FOLDER_OBJECT)pMyObject->GetFolderByName
            (
                (ANSC_HANDLE)pMyObject,
                pFolderName
            );

    if ( pSubFolder )
    {
        if ( (pSubFolder->GetFolderType((ANSC_HANDLE)pSubFolder) == ulFolderType) &&
             (pSubFolder->GetPermission((ANSC_HANDLE)pSubFolder) == ulPermission) )
        {
            return  (ANSC_HANDLE)pSubFolder;
        }
        else
        {
            pSubFolder->ReleaseAccess((ANSC_HANDLE)pSubFolder);

            return  (ANSC_HANDLE)NULL;
        }
    }
    else if ( (pProperty->Permission & SYS_RFO_PERMISSION_FOLDER_ADD) == 0 )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pSubFolder =
            (PSYS_REPOSITORY_FOLDER_OBJECT)SysCreateRepositoryFolder
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pSubFolder )
        {
            return  (ANSC_HANDLE)NULL;
        }
        else
        {
            pSubFolder->HashIndex = ulHashIndex;
            pSubFolder->Timestamp = AnscGetTickInSeconds();
        }
    }

    pSubFolder->SetBirthTime   ((ANSC_HANDLE)pSubFolder, ++pMyObject->GlobalFid);
    pSubFolder->SetRenderAttr  ((ANSC_HANDLE)pSubFolder, hRenderAttr           );
    pSubFolder->SetRecycleBin  ((ANSC_HANDLE)pSubFolder, (ANSC_HANDLE)NULL     );
    pSubFolder->SetParentFolder((ANSC_HANDLE)pSubFolder, (ANSC_HANDLE)pMyObject);
    pSubFolder->SetUserContext ((ANSC_HANDLE)pSubFolder, (ANSC_HANDLE)NULL     );
    pSubFolder->SetFolderName  ((ANSC_HANDLE)pSubFolder, pFolderName           );
    pSubFolder->SetFolderType  ((ANSC_HANDLE)pSubFolder, ulFolderType          );
    pSubFolder->SetPermission  ((ANSC_HANDLE)pSubFolder, ulPermission          );

    switch ( ulFolderType )
    {
        case    SYS_REP_FOLDER_TYPE_STORAGE :

                pSubFolder->SetRcaIf((ANSC_HANDLE)pSubFolder, (ANSC_HANDLE)NULL);
                pSubFolder->SetRcgIf((ANSC_HANDLE)pSubFolder, (ANSC_HANDLE)NULL);
                pSubFolder->SetRcmIf((ANSC_HANDLE)pSubFolder, (ANSC_HANDLE)NULL);

                break;

        case    SYS_REP_FOLDER_TYPE_GUARDED :

                pSubFolder->SetRcaIf((ANSC_HANDLE)pSubFolder, hReserved        );
                pSubFolder->SetRcgIf((ANSC_HANDLE)pSubFolder, (ANSC_HANDLE)NULL);
                pSubFolder->SetRcmIf((ANSC_HANDLE)pSubFolder, (ANSC_HANDLE)NULL);

                break;

        case    SYS_REP_FOLDER_TYPE_DYNAMIC :

                pSubFolder->SetRcaIf((ANSC_HANDLE)pSubFolder, (ANSC_HANDLE)NULL);
                pSubFolder->SetRcgIf((ANSC_HANDLE)pSubFolder, hReserved        );
                pSubFolder->SetRcmIf((ANSC_HANDLE)pSubFolder, (ANSC_HANDLE)NULL);

                break;

        case    SYS_REP_FOLDER_TYPE_SESSION :

                pSubFolder->SetRcaIf((ANSC_HANDLE)pSubFolder, (ANSC_HANDLE)NULL);
                pSubFolder->SetRcgIf((ANSC_HANDLE)pSubFolder, (ANSC_HANDLE)NULL);
                pSubFolder->SetRcmIf((ANSC_HANDLE)pSubFolder, hReserved        );

                break;

        default :

                pSubFolder->SetRcaIf((ANSC_HANDLE)pSubFolder, (ANSC_HANDLE)NULL);
                pSubFolder->SetRcgIf((ANSC_HANDLE)pSubFolder, (ANSC_HANDLE)NULL);
                pSubFolder->SetRcmIf((ANSC_HANDLE)pSubFolder, (ANSC_HANDLE)NULL);

                break;
    }

    if ( pProperty->FolderType == SYS_REP_FOLDER_TYPE_GUARDED )
    {
        returnStatus =
            pSelfRcaIf->Notify
                (
                    pSelfRcaIf->hOwnerContext,
                    (ANSC_HANDLE)pMyObject,
                    SYS_RCA_EVENT_FOLDER_ADDED,
                    (ANSC_HANDLE)pSubFolder
                );
    }
    else if ( pProperty->FolderType == SYS_REP_FOLDER_TYPE_SESSION )
    {
        returnStatus =
            pSelfRcmIf->Notify
                (
                    pSelfRcmIf->hOwnerContext,
                    (ANSC_HANDLE)pMyObject,
                    SYS_RCM_EVENT_FOLDER_ADDED,
                    (ANSC_HANDLE)pSubFolder
                );
    }

    if ( TRUE )
    {
        AnscAcquireLock(&pMyObject->FolderTableLock);

        AnscQueuePushEntry(&pMyObject->FolderTable[ulHashIndex], &pSubFolder->Linkage);
        pSubFolder->AcquireAccess((ANSC_HANDLE)pSubFolder);

        returnStatus =
            pMyObject->AddRfoIntoArray
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pSubFolder
                );

        AnscReleaseLock(&pMyObject->FolderTableLock);
    }
    CcspTraceDebug(("%lu.\n",returnStatus ));
    return  (ANSC_HANDLE)pSubFolder;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoDelFolder
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pFolderName,
                BOOL                        bRecursive,
                BOOL                        bForceful
            );

    description:

        This function is called to delete a folder object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pFolderName
                Specifies the folder name to be deleted.

                BOOL                        bRecursive
                Specifies whether the operation should be carried out
                recursively.

                BOOL                        bForceful
                Specifies whether the operation should be forced even
                if it's not permitted by current permission.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoDelFolder
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pFolderName,
        BOOL                        bRecursive,
        BOOL                        bForceful
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_FOLDER_PROPERTY pProperty    = (PSYS_REPOSITORY_FOLDER_PROPERTY)&pMyObject->Property;
    PSYS_RECYCLE_BIN_OBJECT         pRecycleBin  = (PSYS_RECYCLE_BIN_OBJECT        )pMyObject->hRecycleBin;
    PSYS_RCA_INTERFACE              pSelfRcaIf   = (PSYS_RCA_INTERFACE             )pMyObject->hSelfRcaIf;
    PSYS_RCM_INTERFACE              pSelfRcmIf   = (PSYS_RCM_INTERFACE             )pMyObject->hSelfRcmIf;
    PSYS_REPOSITORY_FOLDER_OBJECT   pSubFolder   = NULL;
    ULONG                           ulHashIndex  = 0;

    if ( (pProperty->Permission & SYS_RFO_PERMISSION_FOLDER_DELETE) == 0 )
    {
        return  ANSC_STATUS_ACCESS_DENIED;
    }
    else
    {
        pSubFolder =
            (PSYS_REPOSITORY_FOLDER_OBJECT)pMyObject->GetFolderByName
                (
                    (ANSC_HANDLE)pMyObject,
                    pFolderName
                );
    }

    if ( !pSubFolder )
    {
        return  ANSC_STATUS_BAD_NAME;
    }
    else if ( !bForceful && !(pSubFolder->GetPermission((ANSC_HANDLE)pSubFolder) & SYS_RFO_PERMISSION_DELETE) )
    {
        pSubFolder->ReleaseAccess((ANSC_HANDLE)pSubFolder);

        return  ANSC_STATUS_ACCESS_DENIED;
    }
    else if ( !pSubFolder->IsRemovable((ANSC_HANDLE)pSubFolder, bRecursive) )
    {
        pSubFolder->ReleaseAccess((ANSC_HANDLE)pSubFolder);

        return  ANSC_STATUS_ACCESS_DENIED;
    }
    else
    {
        ulHashIndex = pSubFolder->HashIndex;
    }

    pSubFolder->ReleaseAccess((ANSC_HANDLE)pSubFolder);

    if ( TRUE )
    {
        AnscAcquireLock(&pMyObject->FolderTableLock);

        AnscQueuePopEntryByLink(&pMyObject->FolderTable[ulHashIndex], &pSubFolder->Linkage);

        returnStatus =
            pMyObject->DelRfoFromArray
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pSubFolder
                );

        AnscReleaseLock(&pMyObject->FolderTableLock);
    }

    if ( pProperty->FolderType == SYS_REP_FOLDER_TYPE_GUARDED )
    {
        returnStatus =
            pSelfRcaIf->Notify
                (
                    pSelfRcaIf->hOwnerContext,
                    (ANSC_HANDLE)pMyObject,
                    SYS_RCA_EVENT_FOLDER_DELETED,
                    (ANSC_HANDLE)pSubFolder
                );
    }
    else if ( pProperty->FolderType == SYS_REP_FOLDER_TYPE_SESSION )
    {
        returnStatus =
            pSelfRcmIf->Notify
                (
                    pSelfRcmIf->hOwnerContext,
                    (ANSC_HANDLE)pMyObject,
                    SYS_RCM_EVENT_FOLDER_DELETED,
                    (ANSC_HANDLE)pSubFolder
                );
    }

    if ( pRecycleBin )
    {
        returnStatus =
            pRecycleBin->AddFolder
                (
                    (ANSC_HANDLE)pRecycleBin,
                    (ANSC_HANDLE)pSubFolder
                );
    }
    else
    {
        returnStatus = pSubFolder->Delete((ANSC_HANDLE)pSubFolder);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysRfoGetFolderByIndex
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulIndex
            );

    description:

        This function is called to retrieve a folder object by matching
        the folder index.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulIndex
                Specifies the folder index to be matched.

    return:     handle of the matched folder object.

**********************************************************************/

ANSC_HANDLE
SysRfoGetFolderByIndex
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_FOLDER_OBJECT   pSubFolder   = NULL;
    ULONG                           ulRfoIndex   = ulIndex;

    if ( ulIndex >= pMyObject->FolderArrayCurLen )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        if ( pMyObject->FolderSortingMode == SYS_RFO_SORTING_MODE_ASCENDING )
        {
            ulRfoIndex = ulIndex;
        }
        else
        {
            ulRfoIndex = pMyObject->FolderArrayCurLen - 1 - ulIndex;
        }

        AnscAcquireLock(&pMyObject->FolderTableLock);
        pSubFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pMyObject->FolderArray[ulRfoIndex];
        pSubFolder->AcquireAccess((ANSC_HANDLE)pSubFolder);
        AnscReleaseLock(&pMyObject->FolderTableLock);

        return  (ANSC_HANDLE)pSubFolder;
    }

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysRfoGetFolderByName
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pFolderName
            );

    description:

        This function is called to retrieve a folder object by matching
        the folder name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pFolderName
                Specifies the folder name to be matched.

    return:     handle of the matched folder object.

**********************************************************************/

ANSC_HANDLE
SysRfoGetFolderByName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pFolderName
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_FOLDER_OBJECT   pSubFolder   = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           ulHashIndex  = AnscHashString(pFolderName, AnscSizeOfString(pFolderName), SYS_RFO_RFO_TABLE_SIZE);

    AnscAcquireLock(&pMyObject->FolderTableLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->FolderTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pSubFolder  = ACCESS_SYS_REPOSITORY_FOLDER_OBJECT(pSLinkEntry);
        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

        if ( strcmp(pFolderName,pSubFolder->GetFolderName((ANSC_HANDLE)pSubFolder)) == 0 )
        {
            pSubFolder->AcquireAccess((ANSC_HANDLE)pSubFolder);

            AnscReleaseLock(&pMyObject->FolderTableLock);

            return  (ANSC_HANDLE)pSubFolder;
        }
    }

    AnscReleaseLock(&pMyObject->FolderTableLock);

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

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

    description:

        This function is called to add a record object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pRecordName
                Specifies the record name to be added.

                ULONG                       ulPermission
                Specifies the record permission to be created.

                ULONG                       ulRecordType
                Specifies the record type to be added.

                ANSC_HANDLE                 hRenderAttr
                Specifies the render attribute associated with the
                record to be added.

                PVOID                       pBuffer
                Specifies the record data value to be added.

                ULONG                       ulSize
                Specifies the size of the record data value.

    return:     handle of the new record object.

**********************************************************************/

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
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_FOLDER_PROPERTY pProperty    = (PSYS_REPOSITORY_FOLDER_PROPERTY)&pMyObject->Property;
    PSYS_RCA_INTERFACE              pSelfRcaIf   = (PSYS_RCA_INTERFACE             )pMyObject->hSelfRcaIf;
    PSYS_RCM_INTERFACE              pSelfRcmIf   = (PSYS_RCM_INTERFACE             )pMyObject->hSelfRcmIf;
    PSYS_REPOSITORY_RECORD_OBJECT   pRepRecord   = NULL;
    ULONG                           ulHashIndex  = AnscHashString(pRecordName, AnscSizeOfString(pRecordName), SYS_RFO_RRO_TABLE_SIZE);

    pRepRecord =
        (PSYS_REPOSITORY_RECORD_OBJECT)pMyObject->GetRecordByName
            (
                (ANSC_HANDLE)pMyObject,
                pRecordName
            );

    if ( pRepRecord )
    {
        if ( (pRepRecord->GetRecordType((ANSC_HANDLE)pRepRecord) != ulRecordType) ||
             (pRepRecord->GetPermission((ANSC_HANDLE)pRepRecord) != ulPermission) )
        {
            return  (ANSC_HANDLE)NULL;
        }
        else if ( (pProperty->Permission & SYS_RFO_PERMISSION_RECORD_MODIFY) == 0 )
        {
            return  (ANSC_HANDLE)NULL;
        }
        else if ( !(pRepRecord->GetPermission((ANSC_HANDLE)pRepRecord) & SYS_RRO_PERMISSION_MODIFY) )
        {
            return  (ANSC_HANDLE)NULL;
        }
        else
        {
            returnStatus =
                pRepRecord->SetRecordData
                    (
                        (ANSC_HANDLE)pRepRecord,
                        pBuffer,
                        ulSize
                    );

            if ( pProperty->FolderType == SYS_REP_FOLDER_TYPE_GUARDED )
            {
                returnStatus =
                    pSelfRcaIf->Notify
                        (
                            pSelfRcaIf->hOwnerContext,
                            (ANSC_HANDLE)pMyObject,
                            SYS_RCA_EVENT_RECORD_MODIFIED,
                            (ANSC_HANDLE)pRepRecord
                        );
            }
            else if ( pProperty->FolderType == SYS_REP_FOLDER_TYPE_SESSION )
            {
                returnStatus =
                    pSelfRcmIf->Notify
                        (
                            pSelfRcmIf->hOwnerContext,
                            (ANSC_HANDLE)pMyObject,
                            SYS_RCM_EVENT_RECORD_MODIFIED,
                            (ANSC_HANDLE)pRepRecord
                        );
            }

            return  (ANSC_HANDLE)pRepRecord;
        }
    }
    else if ( (pProperty->Permission & SYS_RFO_PERMISSION_RECORD_ADD) == 0 )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        switch ( ulRecordType )
        {
            case    SYS_REP_RECORD_TYPE_SINT :

                    pRepRecord =
                        (PSYS_REPOSITORY_RECORD_OBJECT)SysCreateRroSint
                            (
                                pMyObject->hContainerContext,
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)NULL
                            );

                    break;

            case    SYS_REP_RECORD_TYPE_UINT :

                    pRepRecord =
                        (PSYS_REPOSITORY_RECORD_OBJECT)SysCreateRroUint
                            (
                                pMyObject->hContainerContext,
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)NULL
                            );

                    break;

            case    SYS_REP_RECORD_TYPE_BOOL :

                    pRepRecord =
                        (PSYS_REPOSITORY_RECORD_OBJECT)SysCreateRroBool
                            (
                                pMyObject->hContainerContext,
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)NULL
                            );

                    break;

            case    SYS_REP_RECORD_TYPE_ASTR :

                    pRepRecord =
                        (PSYS_REPOSITORY_RECORD_OBJECT)SysCreateRroAstr
                            (
                                pMyObject->hContainerContext,
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)NULL
                            );

                    break;

            case    SYS_REP_RECORD_TYPE_BSTR :

                    pRepRecord =
                        (PSYS_REPOSITORY_RECORD_OBJECT)SysCreateRroBstr
                            (
                                pMyObject->hContainerContext,
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)NULL
                            );

                    break;

            case    SYS_REP_RECORD_TYPE_HCXT :

                    pRepRecord =
                        (PSYS_REPOSITORY_RECORD_OBJECT)SysCreateRroHcxt
                            (
                                pMyObject->hContainerContext,
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)NULL
                            );

                    break;

            case    SYS_REP_RECORD_TYPE_ENUM :

                    pRepRecord =
                        (PSYS_REPOSITORY_RECORD_OBJECT)SysCreateRroEnum
                            (
                                pMyObject->hContainerContext,
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)NULL
                            );

                    break;

            default :

                    pRepRecord =
                        (PSYS_REPOSITORY_RECORD_OBJECT)SysCreateRepositoryRecord
                            (
                                pMyObject->hContainerContext,
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)NULL
                            );

                    break;
        }

        if ( !pRepRecord )
        {
            return  (ANSC_HANDLE)NULL;
        }
        else
        {
            pRepRecord->HashIndex = ulHashIndex;
            pRepRecord->Timestamp = AnscGetTickInSeconds();
        }
    }

    pRepRecord->SetBirthTime   ((ANSC_HANDLE)pRepRecord, ++pMyObject->GlobalRid);
    pRepRecord->SetRenderAttr  ((ANSC_HANDLE)pRepRecord, hRenderAttr           );
    pRepRecord->SetParentFolder((ANSC_HANDLE)pRepRecord, (ANSC_HANDLE)pMyObject);
    pRepRecord->SetUserContext ((ANSC_HANDLE)pRepRecord, (ANSC_HANDLE)NULL     );
    pRepRecord->SetRecordName  ((ANSC_HANDLE)pRepRecord, pRecordName           );
    pRepRecord->SetRecordType  ((ANSC_HANDLE)pRepRecord, ulRecordType          );
    pRepRecord->SetPermission  ((ANSC_HANDLE)pRepRecord, ulPermission          );

    returnStatus =
        pRepRecord->SetRecordData
            (
                (ANSC_HANDLE)pRepRecord,
                pBuffer,
                ulSize
            );

    if ( pProperty->FolderType == SYS_REP_FOLDER_TYPE_GUARDED )
    {
        returnStatus =
            pSelfRcaIf->Notify
                (
                    pSelfRcaIf->hOwnerContext,
                    (ANSC_HANDLE)pMyObject,
                    SYS_RCA_EVENT_RECORD_ADDED,
                    (ANSC_HANDLE)pRepRecord
                );
    }
    else if ( pProperty->FolderType == SYS_REP_FOLDER_TYPE_SESSION )
    {
        returnStatus =
            pSelfRcmIf->Notify
                (
                    pSelfRcmIf->hOwnerContext,
                    (ANSC_HANDLE)pMyObject,
                    SYS_RCM_EVENT_RECORD_ADDED,
                    (ANSC_HANDLE)pRepRecord
                );
    }

    if ( TRUE )
    {
        AnscAcquireLock(&pMyObject->RecordTableLock);

        AnscQueuePushEntry(&pMyObject->RecordTable[ulHashIndex], &pRepRecord->Linkage);

        returnStatus =
            pMyObject->AddRroIntoArray
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pRepRecord
                );

        AnscReleaseLock(&pMyObject->RecordTableLock);
    }
    CcspTraceDebug(("%lu.\n",returnStatus));
    return  (ANSC_HANDLE)pRepRecord;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoDelRecord
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pRecordName
            );

    description:

        This function is called to delete a record object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pRecordName
                Specifies the record name to be deleted.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoDelRecord
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRecordName
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_FOLDER_PROPERTY pProperty    = (PSYS_REPOSITORY_FOLDER_PROPERTY)&pMyObject->Property;
    PSYS_RECYCLE_BIN_OBJECT         pRecycleBin  = (PSYS_RECYCLE_BIN_OBJECT        )pMyObject->hRecycleBin;
    PSYS_RCA_INTERFACE              pSelfRcaIf   = (PSYS_RCA_INTERFACE             )pMyObject->hSelfRcaIf;
    PSYS_RCM_INTERFACE              pSelfRcmIf   = (PSYS_RCM_INTERFACE             )pMyObject->hSelfRcmIf;
    PSYS_REPOSITORY_RECORD_OBJECT   pRepRecord   = NULL;
    ULONG                           ulHashIndex  = 0;

    if ( (pProperty->Permission & SYS_RFO_PERMISSION_RECORD_DELETE) == 0 )
    {
        return  ANSC_STATUS_ACCESS_DENIED;
    }
    else
    {
        pRepRecord =
            (PSYS_REPOSITORY_RECORD_OBJECT)pMyObject->GetRecordByName
                (
                    (ANSC_HANDLE)pMyObject,
                    pRecordName
                );
    }

    if ( !pRepRecord )
    {
        return  ANSC_STATUS_BAD_NAME;
    }
    else if ( !(pRepRecord->GetPermission((ANSC_HANDLE)pRepRecord) & SYS_RRO_PERMISSION_DELETE) )
    {
        return  ANSC_STATUS_ACCESS_DENIED;
    }
    else
    {
        ulHashIndex = pRepRecord->HashIndex;
    }

    if ( TRUE )
    {
        AnscAcquireLock(&pMyObject->RecordTableLock);

        AnscQueuePopEntryByLink(&pMyObject->RecordTable[ulHashIndex], &pRepRecord->Linkage);

        returnStatus =
            pMyObject->DelRroFromArray
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pRepRecord
                );

        AnscReleaseLock(&pMyObject->RecordTableLock);
    }

    if ( pProperty->FolderType == SYS_REP_FOLDER_TYPE_GUARDED )
    {
        returnStatus =
            pSelfRcaIf->Notify
                (
                    pSelfRcaIf->hOwnerContext,
                    (ANSC_HANDLE)pMyObject,
                    SYS_RCA_EVENT_RECORD_DELETED,
                    (ANSC_HANDLE)pRepRecord
                );
    }
    else if ( pProperty->FolderType == SYS_REP_FOLDER_TYPE_SESSION )
    {
        returnStatus =
            pSelfRcmIf->Notify
                (
                    pSelfRcmIf->hOwnerContext,
                    (ANSC_HANDLE)pMyObject,
                    SYS_RCM_EVENT_RECORD_DELETED,
                    (ANSC_HANDLE)pRepRecord
                );
    }

    if ( pRecycleBin )
    {
        returnStatus =
            pRecycleBin->AddRecord
                (
                    (ANSC_HANDLE)pRecycleBin,
                    (ANSC_HANDLE)pRepRecord
                );
    }
    else
    {
        pRepRecord->Remove((ANSC_HANDLE)pRepRecord);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysRfoGetRecordByIndex
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulIndex
            );

    description:

        This function is called to retrieve a record object by matching
        the record index.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulIndex
                Specifies the record index to be matched.

    return:     handle of the matched record object.

**********************************************************************/

ANSC_HANDLE
SysRfoGetRecordByIndex
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_RECORD_OBJECT   pRepRecord   = NULL;
    ULONG                           ulRroIndex   = ulIndex;

    if ( ulIndex >= pMyObject->RecordArrayCurLen )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        if ( pMyObject->RecordSortingMode == SYS_RFO_SORTING_MODE_ASCENDING )
        {
            ulRroIndex = ulIndex;
        }
        else
        {
            ulRroIndex = pMyObject->RecordArrayCurLen - 1 - ulIndex;
        }

        AnscAcquireLock(&pMyObject->RecordTableLock);
        pRepRecord = (PSYS_REPOSITORY_RECORD_OBJECT)pMyObject->RecordArray[ulRroIndex];
        AnscReleaseLock(&pMyObject->RecordTableLock);

        return  (ANSC_HANDLE)pRepRecord;
    }

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysRfoGetRecordByName
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pRecordName
            );

    description:

        This function is called to retrieve a record object by matching
        the record name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pRecordName
                Specifies the record name to be matched.

    return:     handle of the matched record object.

**********************************************************************/

ANSC_HANDLE
SysRfoGetRecordByName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRecordName
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_RECORD_OBJECT   pRepRecord   = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           ulHashIndex  = AnscHashString(pRecordName, AnscSizeOfString(pRecordName), SYS_RFO_RRO_TABLE_SIZE);

    AnscAcquireLock(&pMyObject->RecordTableLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->RecordTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pRepRecord  = ACCESS_SYS_REPOSITORY_RECORD_OBJECT(pSLinkEntry);
        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

        if ( strcmp(pRecordName,pRepRecord->GetRecordName((ANSC_HANDLE)pRepRecord) ) == 0 )
        {
            AnscReleaseLock(&pMyObject->RecordTableLock);

            return  (ANSC_HANDLE)pRepRecord;
        }
    }

    AnscReleaseLock(&pMyObject->RecordTableLock);

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

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

    description:

        This function is called to configure a record object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pRecordName
                Specifies the record name to be configured.

                ULONG                       ulRecordType
                Specifies the record type to be configured.

                ANSC_HANDLE                 hRenderAttr
                Specifies the render attribute associated with the
                record to be configured.

                PVOID                       pBuffer
                Specifies the record data value to be configured.

                ULONG                       ulSize
                Specifies the size of the record data value.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoSetRecord
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRecordName,
        ULONG                       ulRecordType,
        ANSC_HANDLE                 hRenderAttr,
        PVOID                       pBuffer,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(ulRecordType);
    UNREFERENCED_PARAMETER(hRenderAttr);
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject      = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_FOLDER_PROPERTY pProperty      = (PSYS_REPOSITORY_FOLDER_PROPERTY)&pMyObject->Property;
    PSYS_RCA_INTERFACE              pSelfRcaIf     = (PSYS_RCA_INTERFACE             )pMyObject->hSelfRcaIf;
    PSYS_RCM_INTERFACE              pSelfRcmIf     = (PSYS_RCM_INTERFACE             )pMyObject->hSelfRcmIf;
    PSYS_REPOSITORY_RECORD_OBJECT   pRepRecord     = NULL;
    PSYS_RRO_BOOL_OBJECT            pRepRecordBool = NULL;
    PSYS_RRO_SINT_OBJECT            pRepRecordSint = NULL;
    PSYS_RRO_RENDER_ATTR            pRroRenderAttr = NULL;

    if ( (pProperty->Permission & SYS_RFO_PERMISSION_RECORD_MODIFY) == 0 )
    {
        return  ANSC_STATUS_ACCESS_DENIED;
    }
    else
    {
        pRepRecord =
            (PSYS_REPOSITORY_RECORD_OBJECT)pMyObject->GetRecordByName
                (
                    (ANSC_HANDLE)pMyObject,
                    pRecordName
                );
    }

    if ( !pRepRecord )
    {
        return  ANSC_STATUS_BAD_NAME;
    }
    else if ( !(pRepRecord->GetPermission((ANSC_HANDLE)pRepRecord) & SYS_RRO_PERMISSION_MODIFY) )
    {
        return  ANSC_STATUS_ACCESS_DENIED;
    }

    pRroRenderAttr = pRepRecord->GetRenderAttr((ANSC_HANDLE)pRepRecord);
    returnStatus   =
        pRepRecord->SetRecordData
            (
                (ANSC_HANDLE)pRepRecord,
                pBuffer,
                ulSize
            );

    if ( pRroRenderAttr )
    {
        switch ( pRroRenderAttr->ContentType )
        {
            case    SYS_RECORD_CONTENT_BUTTON :

                    if ( pRepRecord->GetRecordType((ANSC_HANDLE)pRepRecord) == SYS_REP_RECORD_TYPE_BOOL )
                    {
                        pRepRecordBool = (PSYS_RRO_BOOL_OBJECT)pRepRecord;

                        if ( pRepRecordBool->GetRecordValue((ANSC_HANDLE)pRepRecordBool) )
                        {
                            if ( pRroRenderAttr->ActionCall )
                            {
                                returnStatus =
                                    pRroRenderAttr->ActionCall
                                        (
                                            pRroRenderAttr->ActionContext,
                                            1
                                        );
                            }
                        }
                    }

                    break;

            case    SYS_RECORD_CONTENT_SWITCH :

                    if ( pRepRecord->GetRecordType((ANSC_HANDLE)pRepRecord) == SYS_REP_RECORD_TYPE_SINT )
                    {
                        pRepRecordSint = (PSYS_RRO_SINT_OBJECT)pRepRecord;

                        if ( pRepRecordSint->GetRecordValue((ANSC_HANDLE)pRepRecordSint) != 0 )
                        {
                            if ( pRroRenderAttr->ActionCall )
                            {
                                returnStatus =
                                    pRroRenderAttr->ActionCall
                                        (
                                            pRroRenderAttr->ActionContext,
                                            pRepRecordSint->GetRecordValue((ANSC_HANDLE)pRepRecordSint)
                                        );
                            }
                        }
                    }

                    break;

            default :

                    break;
        }
    }

    if ( pProperty->FolderType == SYS_REP_FOLDER_TYPE_GUARDED )
    {
        returnStatus =
            pSelfRcaIf->Notify
                (
                    pSelfRcaIf->hOwnerContext,
                    (ANSC_HANDLE)pMyObject,
                    SYS_RCA_EVENT_RECORD_MODIFIED,
                    (ANSC_HANDLE)pRepRecord
                );
    }
    else if ( pProperty->FolderType == SYS_REP_FOLDER_TYPE_SESSION )
    {
        returnStatus =
            pSelfRcmIf->Notify
                (
                    pSelfRcmIf->hOwnerContext,
                    (ANSC_HANDLE)pMyObject,
                    SYS_RCM_EVENT_RECORD_MODIFIED,
                    (ANSC_HANDLE)pRepRecord
                );
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoGenerateContent
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called when the content of the folder object
        needs to be generated.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoGenerateContent
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_FOLDER_PROPERTY pProperty    = (PSYS_REPOSITORY_FOLDER_PROPERTY)&pMyObject->Property;
    PSYS_RCG_INTERFACE              pSelfRcgIf   = (PSYS_RCG_INTERFACE             )pMyObject->hSelfRcgIf;
    PSYS_RCM_INTERFACE              pSelfRcmIf   = (PSYS_RCM_INTERFACE             )pMyObject->hSelfRcmIf;

    if ( pProperty->FolderType == SYS_REP_FOLDER_TYPE_DYNAMIC )
    {
        pMyObject->Timestamp = AnscGetTickInSeconds();
        returnStatus         =
            pSelfRcgIf->Synchronize
                (
                    pSelfRcgIf->hOwnerContext,
                    (ANSC_HANDLE)pMyObject
                );
    }
    else if ( pProperty->FolderType == SYS_REP_FOLDER_TYPE_SESSION )
    {
        pMyObject->Timestamp = AnscGetTickInSeconds();
        returnStatus         =
            pSelfRcmIf->Init
                (
                    pSelfRcmIf->hOwnerContext,
                    (ANSC_HANDLE)pMyObject
                );
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoPopulateContent
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called when the content of the folder object
        needs to be populated.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoPopulateContent
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_FOLDER_PROPERTY pProperty    = (PSYS_REPOSITORY_FOLDER_PROPERTY)&pMyObject->Property;
    PSYS_RCA_INTERFACE              pSelfRcaIf   = (PSYS_RCA_INTERFACE             )pMyObject->hSelfRcaIf;
    PSYS_RCM_INTERFACE              pSelfRcmIf   = (PSYS_RCM_INTERFACE             )pMyObject->hSelfRcmIf;

    if ( pProperty->FolderType == SYS_REP_FOLDER_TYPE_GUARDED )
    {
        if ( TRUE )
        {
            pMyObject->Timestamp = AnscGetTickInSeconds();
            returnStatus         =
                pSelfRcaIf->Review
                    (
                        pSelfRcaIf->hOwnerContext,
                        (ANSC_HANDLE)pMyObject
                    );
        }
    }
    else if ( pProperty->FolderType == SYS_REP_FOLDER_TYPE_SESSION )
    {
        if ( TRUE )
        {
            pMyObject->Timestamp = AnscGetTickInSeconds();
            returnStatus         =
                pSelfRcmIf->Stop
                    (
                        pSelfRcmIf->hOwnerContext,
                        (ANSC_HANDLE)pMyObject
                    );
        }
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoOpenFirst
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called when the folder is opened for the first
        time in history.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoOpenFirst
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_FOLDER_PROPERTY pProperty    = (PSYS_REPOSITORY_FOLDER_PROPERTY)&pMyObject->Property;
    PSYS_RCG_INTERFACE              pSelfRcgIf   = (PSYS_RCG_INTERFACE             )pMyObject->hSelfRcgIf;
    PSYS_RCM_INTERFACE              pSelfRcmIf   = (PSYS_RCM_INTERFACE             )pMyObject->hSelfRcmIf;

    if ( pProperty->FolderType == SYS_REP_FOLDER_TYPE_DYNAMIC )
    {
        pMyObject->Timestamp = AnscGetTickInSeconds();
        returnStatus         =
            pSelfRcgIf->Synchronize
                (
                    pSelfRcgIf->hOwnerContext,
                    (ANSC_HANDLE)pMyObject
                );
    }
    else if ( pProperty->FolderType == SYS_REP_FOLDER_TYPE_SESSION )
    {
        pMyObject->Timestamp = AnscGetTickInSeconds();
        returnStatus         =
            pSelfRcmIf->Init
                (
                    pSelfRcmIf->hOwnerContext,
                    (ANSC_HANDLE)pMyObject
                );
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoCloseLast
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulFlags
            );

    description:

        This function is called when the folder is closed by the last
        user who is done accessing the folder.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulFlags
                Specifies the flags that need to be applied to the
                folder object.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoCloseLast
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulFlags
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_FOLDER_PROPERTY pProperty    = (PSYS_REPOSITORY_FOLDER_PROPERTY)&pMyObject->Property;
    PSYS_RCA_INTERFACE              pSelfRcaIf   = (PSYS_RCA_INTERFACE             )pMyObject->hSelfRcaIf;
    PSYS_RCM_INTERFACE              pSelfRcmIf   = (PSYS_RCM_INTERFACE             )pMyObject->hSelfRcmIf;

    if ( pProperty->FolderType == SYS_REP_FOLDER_TYPE_GUARDED )
    {
        if ( (ulFlags & SYS_RFO_CLOSE_FLAG_SYNC) != 0 )
        {
            pMyObject->Timestamp = AnscGetTickInSeconds();
            returnStatus         =
                pSelfRcaIf->Review
                    (
                        pSelfRcaIf->hOwnerContext,
                        (ANSC_HANDLE)pMyObject
                    );
        }
    }
    else if ( pProperty->FolderType == SYS_REP_FOLDER_TYPE_SESSION )
    {
        if ( (ulFlags & SYS_RFO_CLOSE_FLAG_SYNC) != 0 )
        {
            pMyObject->Timestamp = AnscGetTickInSeconds();
            returnStatus         =
                pSelfRcmIf->Stop
                    (
                        pSelfRcmIf->hOwnerContext,
                        (ANSC_HANDLE)pMyObject
                    );
        }
    }

    if ( (ulFlags & SYS_RFO_CLOSE_FLAG_CLEAN) != 0 )
    {
        pMyObject->Reset((ANSC_HANDLE)pMyObject);
    }

    return  returnStatus;
}
