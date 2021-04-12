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

    module:	sys_rdo_access.c

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced field-access functions
        of the Sys Repository Driver Object.

        *   SysRdoAddFolder1
        *   SysRdoAddFolder2
        *   SysRdoDelFolder1
        *   SysRdoDelFolder2
        *   SysRdoGetFolder1
        *   SysRdoGetFolder2
        *   SysRdoAddRecord
        *   SysRdoDelRecord
        *   SysRdoGetRecord
        *   SysRdoSetRecord

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


#include "sys_rdo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysRdoAddFolder1
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

                ULONG                       ulFolderType
                Specifies the folder type to be added.

                ANSC_HANDLE                 hRenderAttr
                Specifies the render attribute associated with the
                folder to be added.

                ANSC_HANDLE                 hReserved
                Specifies a folder-specific context handle.

    return:     handle of the new folder object.

**********************************************************************/

ANSC_HANDLE
SysRdoAddFolder1
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pFolderName,
        ULONG                       ulPermission,
        ULONG                       ulFolderType,
        ANSC_HANDLE                 hRenderAttr,
        ANSC_HANDLE                 hReserved
    )
{
    PSYS_REPOSITORY_DRIVER_OBJECT   pMyObject    = (PSYS_REPOSITORY_DRIVER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_FOLDER_OBJECT   pRootFolder  = (PSYS_REPOSITORY_FOLDER_OBJECT  )pMyObject->hRootFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pThisFolder  = NULL;

    pThisFolder =
        (PSYS_REPOSITORY_FOLDER_OBJECT)pMyObject->AddFolder2
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pRootFolder,
                pFolderName,
                ulPermission,
                ulFolderType,
                hRenderAttr,
                hReserved
            );

    return  (ANSC_HANDLE)pThisFolder;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysRdoAddFolder2
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCurFolder,
                char*                       pSubFolderName,
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

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder under which the new folder
                shall be created. This parameter could be NULL.

                char*                       pSubFolderName
                Specifies the folder name to be added.

                ULONG                       ulPermission
                Specifies the permission of the folder to be added.

                ULONG                       ulFolderType
                Specifies the folder type to be added.

                ANSC_HANDLE                 hRenderAttr
                Specifies the render attribute associated with the
                folder to be added.

                ANSC_HANDLE                 hReserved
                Specifies a folder-specific context handle.

    return:     handle of the new folder object.

**********************************************************************/

ANSC_HANDLE
SysRdoAddFolder2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pSubFolderName,
        ULONG                       ulPermission,
        ULONG                       ulFolderType,
        ANSC_HANDLE                 hRenderAttr,
        ANSC_HANDLE                 hReserved
    )
{
    PSYS_REPOSITORY_DRIVER_OBJECT   pMyObject    = (PSYS_REPOSITORY_DRIVER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_FOLDER_OBJECT   pRootFolder  = (PSYS_REPOSITORY_FOLDER_OBJECT  )pMyObject->hRootFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pThisFolder  = (hCurFolder != NULL)? (PSYS_REPOSITORY_FOLDER_OBJECT)hCurFolder : pRootFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pNextFolder  = NULL;
    PANSC_TOKEN_CHAIN               pTokenChain  = NULL;
    PANSC_STRING_TOKEN              pTokenEntry  = NULL;

    pTokenChain =
        (PANSC_TOKEN_CHAIN)AnscTcAllocate
            (
                pSubFolderName,
                SYS_RDO_PATH_SEPARATORS
            );

    if ( !pTokenChain || !pThisFolder) /*RDKB-5893 , CID-24357, added null check*/
    {
        pThisFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)NULL;

        goto  EXIT1;
    }
    else if ( AnscTcGetTokenCount(pTokenChain) == 0 )
    {
        pThisFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)NULL;

        goto  EXIT2;
    }

    /*
     * The specified folder name is the absolute path where the new folder should be created in the
     * repository tree. We first try to go as far as we can using specified folder name as the path
     * into tree, then creates a new branch based on the rest of the token chain.
     */
    pThisFolder->AcquireAccess((ANSC_HANDLE)pThisFolder);

    while (( pTokenEntry = AnscTcUnlinkToken(pTokenChain)) )
    {
        pNextFolder =
            pThisFolder->GetFolderByName
                (
                    (ANSC_HANDLE)pThisFolder,
                    pTokenEntry->Name
                );

        if ( pNextFolder )
        {
            pThisFolder->ReleaseAccess((ANSC_HANDLE)pThisFolder);

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
        if ( !pThisFolder )
        {
            goto  EXIT2;
        }
        else if ( (pThisFolder->GetPermission((ANSC_HANDLE)pThisFolder) != ulPermission) ||
                  (pThisFolder->GetFolderType((ANSC_HANDLE)pThisFolder) != ulFolderType) )
        {
            pThisFolder->ReleaseAccess((ANSC_HANDLE)pThisFolder);

            pThisFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)NULL;
        }

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
    while (( pTokenEntry = AnscTcUnlinkToken(pTokenChain)) )
    {
        if ( AnscTcGetTokenCount(pTokenChain) > 0 )
        {
            pNextFolder =
                pThisFolder->AddFolder
                    (
                        (ANSC_HANDLE)pThisFolder,
                        pTokenEntry->Name,
                        SYS_RFO_PERMISSION_ALL,
                        SYS_REP_FOLDER_TYPE_STORAGE,
                        (ANSC_HANDLE)NULL,
                        (ANSC_HANDLE)NULL
                    );
        }
        else
        {
            pNextFolder =
                pThisFolder->AddFolder
                    (
                        (ANSC_HANDLE)pThisFolder,
                        pTokenEntry->Name,
                        ulPermission,
                        ulFolderType,
                        hRenderAttr,
                        hReserved
                    );
        }

        if ( pNextFolder )
        {
            pThisFolder->ReleaseAccess((ANSC_HANDLE)pThisFolder);

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
        pThisFolder->ReleaseAccess((ANSC_HANDLE)pThisFolder);

        pThisFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)NULL;
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

        ANSC_STATUS
        SysRdoDelFolder1
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
SysRdoDelFolder1
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pFolderName,
        BOOL                        bRecursive,
        BOOL                        bForceful
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_REPOSITORY_DRIVER_OBJECT   pMyObject    = (PSYS_REPOSITORY_DRIVER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_FOLDER_OBJECT   pRootFolder  = (PSYS_REPOSITORY_FOLDER_OBJECT  )pMyObject->hRootFolder;

    returnStatus =
        pMyObject->DelFolder2
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pRootFolder,
                pFolderName,
                bRecursive,
                bForceful
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRdoDelFolder2
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCurFolder,
                char*                       pSubFolderName,
                BOOL                        bRecursive,
                BOOL                        bForceful
            );

    description:

        This function is called to delete a folder object.

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
SysRdoDelFolder2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pSubFolderName,
        BOOL                        bRecursive,
        BOOL                        bForceful
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_REPOSITORY_DRIVER_OBJECT   pMyObject    = (PSYS_REPOSITORY_DRIVER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_FOLDER_OBJECT   pThisFolder  = NULL;
    PSYS_REPOSITORY_FOLDER_OBJECT   pNextFolder  = NULL;

    pNextFolder =
        (PSYS_REPOSITORY_FOLDER_OBJECT)pMyObject->GetFolder2
            (
                (ANSC_HANDLE)pMyObject,
                hCurFolder,
                pSubFolderName
            );

    if ( !pNextFolder )
    {
        return  ANSC_STATUS_BAD_NAME;
    }
    else
    {
        pNextFolder->ReleaseAccess((ANSC_HANDLE)pNextFolder);
    }

    pThisFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pNextFolder->GetParentFolder((ANSC_HANDLE)pNextFolder);

    if ( !pThisFolder )
    {
        return  ANSC_STATUS_ACCESS_DENIED;
    }

    pThisFolder->AcquireAccess((ANSC_HANDLE)pThisFolder);

    returnStatus =
        pThisFolder->DelFolder
            (
                (ANSC_HANDLE)pThisFolder,
                pNextFolder->GetFolderName((ANSC_HANDLE)pNextFolder),
                bRecursive,
                bForceful
            );

    pThisFolder->ReleaseAccess((ANSC_HANDLE)pThisFolder);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysRdoGetFolder1
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
SysRdoGetFolder1
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pFolderName
    )
{
    PSYS_REPOSITORY_DRIVER_OBJECT   pMyObject    = (PSYS_REPOSITORY_DRIVER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_FOLDER_OBJECT   pRootFolder  = (PSYS_REPOSITORY_FOLDER_OBJECT  )pMyObject->hRootFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pThisFolder  = NULL;

    pThisFolder =
        (PSYS_REPOSITORY_FOLDER_OBJECT)pMyObject->GetFolder2
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pRootFolder,
                pFolderName
            );

    return  (ANSC_HANDLE)pThisFolder;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysRdoGetFolder2
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCurFolder,
                char*                       pSubFolderName
            );

    description:

        This function is called to retrieve a folder object by matching
        the folder name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCurFolder
                Specifies the current folder under which the new folder
                shall be opened. This parameter could be NULL.

                char*                       pSubFolderName
                Specifies the folder name to be opened, this parameter
                MUST not be NULL.

    return:     handle of the matched folder object.

**********************************************************************/

ANSC_HANDLE
SysRdoGetFolder2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCurFolder,
        char*                       pSubFolderName
    )
{
    PSYS_REPOSITORY_DRIVER_OBJECT   pMyObject    = (PSYS_REPOSITORY_DRIVER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_FOLDER_OBJECT   pRootFolder  = (PSYS_REPOSITORY_FOLDER_OBJECT  )pMyObject->hRootFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pThisFolder  = (hCurFolder != NULL)? (PSYS_REPOSITORY_FOLDER_OBJECT)hCurFolder : pRootFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pNextFolder  = NULL;
    PANSC_TOKEN_CHAIN               pTokenChain  = NULL;
    PANSC_STRING_TOKEN              pTokenEntry  = NULL;

    pTokenChain =
        (PANSC_TOKEN_CHAIN)AnscTcAllocate
            (
                pSubFolderName,
                SYS_RDO_PATH_SEPARATORS
            );

    if ( !pTokenChain )
    {
        pThisFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)NULL;

        goto  EXIT1;
    }
    else if ( AnscTcGetTokenCount(pTokenChain) == 0 )
    {
        pThisFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)NULL;

        goto  EXIT2;
    }

    pThisFolder->AcquireAccess((ANSC_HANDLE)pThisFolder);

    while ((pTokenEntry = AnscTcUnlinkToken(pTokenChain)) )
    {
        pNextFolder =
            pThisFolder->GetFolderByName
                (
                    (ANSC_HANDLE)pThisFolder,
                    pTokenEntry->Name
                );

        if ( pNextFolder )
        {
            pThisFolder->ReleaseAccess((ANSC_HANDLE)pThisFolder);

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
        pThisFolder->ReleaseAccess((ANSC_HANDLE)pThisFolder);

        pThisFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)NULL;
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
        SysRdoAddRecord
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
SysRdoAddRecord
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
    PSYS_REPOSITORY_DRIVER_OBJECT   pMyObject    = (PSYS_REPOSITORY_DRIVER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_FOLDER_OBJECT   pRootFolder  = (PSYS_REPOSITORY_FOLDER_OBJECT  )pMyObject->hRootFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pThisFolder  = pRootFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pNextFolder  = NULL;
    PSYS_REPOSITORY_RECORD_OBJECT   pThisRecord  = NULL;
    PANSC_TOKEN_CHAIN               pTokenChain  = NULL;
    PANSC_STRING_TOKEN              pTokenEntry  = NULL;

    pTokenChain =
        (PANSC_TOKEN_CHAIN)AnscTcAllocate
            (
                pRecordName,
                SYS_RDO_PATH_SEPARATORS
            );

    if ( !pTokenChain || !pThisFolder) /*RDKB-5893 , CID-24143, CID-24332, added null check*/
    {
        pThisRecord = (PSYS_REPOSITORY_RECORD_OBJECT)NULL;

        goto  EXIT;
    }
    else if ( AnscTcGetTokenCount(pTokenChain) == 0 )
    {
        pThisRecord = (PSYS_REPOSITORY_RECORD_OBJECT)NULL;

        goto  EXIT;
    }

    pThisFolder->AcquireAccess((ANSC_HANDLE)pThisFolder);

    while ( (pTokenEntry = AnscTcUnlinkToken(pTokenChain)) )
    {
        pNextFolder =
            pThisFolder->GetFolderByName
                (
                    (ANSC_HANDLE)pThisFolder,
                    pTokenEntry->Name
                );

        if ( pNextFolder )
        {
            pThisFolder->ReleaseAccess((ANSC_HANDLE)pThisFolder);

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
        pThisFolder->ReleaseAccess((ANSC_HANDLE)pThisFolder);

        pThisRecord = (PSYS_REPOSITORY_RECORD_OBJECT)NULL;

        goto  EXIT;
    }
    else if ( !pThisFolder )
    {
        pThisRecord = (PSYS_REPOSITORY_RECORD_OBJECT)NULL;

        goto  EXIT;
    }
    else if ( AnscTcGetTokenCount(pTokenChain) != 1 )
    {
        pThisFolder->ReleaseAccess((ANSC_HANDLE)pThisFolder);

        pThisRecord = (PSYS_REPOSITORY_RECORD_OBJECT)NULL;

        goto  EXIT;
    }
    else if ( (pThisFolder->GetPermission((ANSC_HANDLE)pThisFolder) & SYS_RFO_PERMISSION_RECORD_ADD) == 0 )
    {
        pThisFolder->ReleaseAccess((ANSC_HANDLE)pThisFolder);

        pThisRecord = (PSYS_REPOSITORY_RECORD_OBJECT)NULL;

        goto  EXIT;
    }
    else
    {
        pTokenEntry = AnscTcUnlinkToken(pTokenChain);
    }

    pThisRecord =
        (PSYS_REPOSITORY_RECORD_OBJECT)pThisFolder->AddRecord
            (
                (ANSC_HANDLE)pThisFolder,
                pTokenEntry->Name,
                ulPermission,
                ulRecordType,
                hRenderAttr,
                pBuffer,
                ulSize
            );

    pThisFolder->ReleaseAccess((ANSC_HANDLE)pThisFolder);

    AnscFreeMemory(pTokenEntry);


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT:

    if (pTokenChain)
        AnscTcFree((ANSC_HANDLE)pTokenChain);

    return  (ANSC_HANDLE)pThisRecord;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRdoDelRecord
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
SysRdoDelRecord
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRecordName
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_REPOSITORY_DRIVER_OBJECT   pMyObject    = (PSYS_REPOSITORY_DRIVER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_FOLDER_OBJECT   pRootFolder  = (PSYS_REPOSITORY_FOLDER_OBJECT  )pMyObject->hRootFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pThisFolder  = pRootFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pNextFolder  = NULL;
    PANSC_TOKEN_CHAIN               pTokenChain  = NULL;
    PANSC_STRING_TOKEN              pTokenEntry  = NULL;

    pTokenChain =
        (PANSC_TOKEN_CHAIN)AnscTcAllocate
            (
                pRecordName,
                SYS_RDO_PATH_SEPARATORS
            );

    if ( !pTokenChain || !pThisFolder) /*RDKB-5893 , CID-24410, CID-24446, added null check*/
    {
        returnStatus = ANSC_STATUS_BAD_NAME;

        goto  EXIT;
    }
    else if ( AnscTcGetTokenCount(pTokenChain) == 0 )
    {
        returnStatus = ANSC_STATUS_BAD_NAME;

        goto  EXIT;
    }

    pThisFolder->AcquireAccess((ANSC_HANDLE)pThisFolder);

    while ( (pTokenEntry = AnscTcUnlinkToken(pTokenChain)) )
    {
        pNextFolder =
            pThisFolder->GetFolderByName
                (
                    (ANSC_HANDLE)pThisFolder,
                    pTokenEntry->Name
                );

        if ( pNextFolder )
        {
            pThisFolder->ReleaseAccess((ANSC_HANDLE)pThisFolder);

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
        pThisFolder->ReleaseAccess((ANSC_HANDLE)pThisFolder);

        returnStatus = ANSC_STATUS_BAD_NAME;

        goto  EXIT;
    }
    else if ( !pThisFolder )
    {
        returnStatus = ANSC_STATUS_BAD_NAME;

        goto  EXIT;
    }
    else if ( AnscTcGetTokenCount(pTokenChain) != 1 )
    {
        pThisFolder->ReleaseAccess((ANSC_HANDLE)pThisFolder);

        returnStatus = ANSC_STATUS_BAD_NAME;

        goto  EXIT;
    }
    else if ( (pThisFolder->GetPermission((ANSC_HANDLE)pThisFolder) & SYS_RFO_PERMISSION_RECORD_DELETE) == 0 )
    {
        pThisFolder->ReleaseAccess((ANSC_HANDLE)pThisFolder);

        returnStatus = ANSC_STATUS_ACCESS_DENIED;

        goto  EXIT;
    }
    else
    {
        pTokenEntry = AnscTcUnlinkToken(pTokenChain);
    }

    returnStatus =
        pThisFolder->DelRecord
            (
                (ANSC_HANDLE)pThisFolder,
                pTokenEntry->Name
            );

    pThisFolder->ReleaseAccess((ANSC_HANDLE)pThisFolder);

    AnscFreeMemory(pTokenEntry);


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT:

    if (pTokenChain)
        AnscTcFree((ANSC_HANDLE)pTokenChain);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysRdoGetRecord
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
SysRdoGetRecord
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRecordName
    )
{
    PSYS_REPOSITORY_DRIVER_OBJECT   pMyObject    = (PSYS_REPOSITORY_DRIVER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_FOLDER_OBJECT   pRootFolder  = (PSYS_REPOSITORY_FOLDER_OBJECT  )pMyObject->hRootFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pThisFolder  = pRootFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pNextFolder  = NULL;
    PSYS_REPOSITORY_RECORD_OBJECT   pThisRecord  = NULL;
    PANSC_TOKEN_CHAIN               pTokenChain  = NULL;
    PANSC_STRING_TOKEN              pTokenEntry  = NULL;

    pTokenChain =
        (PANSC_TOKEN_CHAIN)AnscTcAllocate
            (
                pRecordName,
                SYS_RDO_PATH_SEPARATORS
            );

    if ( !pTokenChain )
    {
        pThisRecord = (PSYS_REPOSITORY_RECORD_OBJECT)NULL;

        goto  EXIT1;
    }
    else if ( AnscTcGetTokenCount(pTokenChain) == 0 )
    {
        pThisRecord = (PSYS_REPOSITORY_RECORD_OBJECT)NULL;

        goto  EXIT2;
    }

    /*CID -53152 Dereference before null check*/
    if ( !pThisFolder )
    {
        pThisRecord = (PSYS_REPOSITORY_RECORD_OBJECT)NULL;

        goto  EXIT2;
    }
    pThisFolder->AcquireAccess((ANSC_HANDLE)pThisFolder);

    while ( (pTokenEntry = AnscTcUnlinkToken(pTokenChain)) )
    {
        pNextFolder =
            pThisFolder->GetFolderByName
                (
                    (ANSC_HANDLE)pThisFolder,
                    pTokenEntry->Name
                );

        if ( pNextFolder )
        {
            pThisFolder->ReleaseAccess((ANSC_HANDLE)pThisFolder);

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
        pThisFolder->ReleaseAccess((ANSC_HANDLE)pThisFolder);

        pThisRecord = (PSYS_REPOSITORY_RECORD_OBJECT)NULL;

        goto  EXIT2;
    }
    else if ( !pThisFolder )
    {
        pThisRecord = (PSYS_REPOSITORY_RECORD_OBJECT)NULL;

        goto  EXIT2;
    }
    else if ( AnscTcGetTokenCount(pTokenChain) != 1 )
    {
        pThisFolder->ReleaseAccess((ANSC_HANDLE)pThisFolder);

        pThisRecord = (PSYS_REPOSITORY_RECORD_OBJECT)NULL;

        goto  EXIT2;
    }
    else
    {
        pTokenEntry = AnscTcUnlinkToken(pTokenChain);
    }

    pThisRecord =
        (PSYS_REPOSITORY_RECORD_OBJECT)pThisFolder->GetRecordByName
            (
                (ANSC_HANDLE)pThisFolder,
                pTokenEntry->Name
            );

    pThisFolder->ReleaseAccess((ANSC_HANDLE)pThisFolder);

    AnscFreeMemory(pTokenEntry);


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT2:

    AnscTcFree((ANSC_HANDLE)pTokenChain);

EXIT1:

    return  (ANSC_HANDLE)pThisRecord;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRdoSetRecord
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
SysRdoSetRecord
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRecordName,
        ULONG                       ulRecordType,
        ANSC_HANDLE                 hRenderAttr,
        PVOID                       pBuffer,
        ULONG                       ulSize
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_REPOSITORY_DRIVER_OBJECT   pMyObject    = (PSYS_REPOSITORY_DRIVER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_FOLDER_OBJECT   pRootFolder  = (PSYS_REPOSITORY_FOLDER_OBJECT  )pMyObject->hRootFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pThisFolder  = pRootFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pNextFolder  = NULL;
    PANSC_TOKEN_CHAIN               pTokenChain  = NULL;
    PANSC_STRING_TOKEN              pTokenEntry  = NULL;

    pTokenChain =
        (PANSC_TOKEN_CHAIN)AnscTcAllocate
            (
                pRecordName,
                SYS_RDO_PATH_SEPARATORS
            );

    if ( !pTokenChain )
    {
        returnStatus = ANSC_STATUS_BAD_NAME;

        goto  EXIT1;
    }
    else if ( AnscTcGetTokenCount(pTokenChain) == 0 )
    {
        returnStatus = ANSC_STATUS_BAD_NAME;

        goto  EXIT2;
    }
    /*CID: 59682 Dereference before null check*/
    if ( !pThisFolder )
    {
        returnStatus = ANSC_STATUS_BAD_NAME;
 
        goto  EXIT2;
    }

    pThisFolder->AcquireAccess((ANSC_HANDLE)pThisFolder);

    while ( (pTokenEntry = AnscTcUnlinkToken(pTokenChain)) )
    {
        pNextFolder =
            pThisFolder->GetFolderByName
                (
                    (ANSC_HANDLE)pThisFolder,
                    pTokenEntry->Name
                );

        if ( pNextFolder )
        {
            pThisFolder->ReleaseAccess((ANSC_HANDLE)pThisFolder);

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
        pThisFolder->ReleaseAccess((ANSC_HANDLE)pThisFolder);

        returnStatus = ANSC_STATUS_BAD_NAME;

        goto  EXIT2;
    }
    else if ( !pThisFolder )
    {
        returnStatus = ANSC_STATUS_BAD_NAME;

        goto  EXIT2;
    }
    else if ( AnscTcGetTokenCount(pTokenChain) != 1 )
    {
        pThisFolder->ReleaseAccess((ANSC_HANDLE)pThisFolder);

        returnStatus = ANSC_STATUS_BAD_NAME;

        goto  EXIT2;
    }
    else if ( (pThisFolder->GetPermission((ANSC_HANDLE)pThisFolder) & SYS_RFO_PERMISSION_RECORD_MODIFY) == 0 )
    {
        pThisFolder->ReleaseAccess((ANSC_HANDLE)pThisFolder);

        returnStatus = ANSC_STATUS_ACCESS_DENIED;

        goto  EXIT2;
    }
    else
    {
        pTokenEntry = AnscTcUnlinkToken(pTokenChain);
    }

    returnStatus =
        pThisFolder->SetRecord
            (
                (ANSC_HANDLE)pThisFolder,
                pTokenEntry->Name,
                ulRecordType,
                hRenderAttr,
                pBuffer,
                ulSize
            );

    pThisFolder->ReleaseAccess((ANSC_HANDLE)pThisFolder);

    AnscFreeMemory(pTokenEntry);


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT2:

    AnscTcFree((ANSC_HANDLE)pTokenChain);

EXIT1:

    return  returnStatus;
}
