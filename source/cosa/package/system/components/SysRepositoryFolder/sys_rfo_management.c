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

    module:	sys_rfo_management.c

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the Sys Repository Folder Object.

        *   SysRfoDelAllFolders
        *   SysRfoDelAllRecords
        *   SysRfoAddRfoIntoArray
        *   SysRfoDelRfoFromArray
        *   SysRfoCleanUpRfoArray
        *   SysRfoAddRroIntoArray
        *   SysRfoDelRroFromArray
        *   SysRfoCleanUpRroArray

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


#include "sys_rfo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoDelAllFolders
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to delete all repository folders.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoDelAllFolders
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_FOLDER_OBJECT   pSubFolder   = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           i            = 0;

    AnscAcquireLock(&pMyObject->FolderTableLock);

    for ( i = 0; i < SYS_RFO_RFO_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscQueuePopEntry(&pMyObject->FolderTable[i]);

        while ( pSLinkEntry )
        {
            pSubFolder  = ACCESS_SYS_REPOSITORY_FOLDER_OBJECT(pSLinkEntry);
            pSLinkEntry = AnscQueuePopEntry(&pMyObject->FolderTable[i]);

            pSubFolder->AcquireAccess((ANSC_HANDLE)pSubFolder);
            pSubFolder->ReleaseAccess((ANSC_HANDLE)pSubFolder);
            pSubFolder->Remove       ((ANSC_HANDLE)pSubFolder);
        }
    }

    pMyObject->CleanUpRfoArray((ANSC_HANDLE)pMyObject);

    AnscReleaseLock(&pMyObject->FolderTableLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoDelAllRecords
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to delete all repository folders.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoDelAllRecords
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_RECORD_OBJECT   pRepRecord   = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           i            = 0;

    AnscAcquireLock(&pMyObject->RecordTableLock);

    for ( i = 0; i < SYS_RFO_RRO_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscQueuePopEntry(&pMyObject->RecordTable[i]);

        while ( pSLinkEntry )
        {
            pRepRecord  = ACCESS_SYS_REPOSITORY_RECORD_OBJECT(pSLinkEntry);
            pSLinkEntry = AnscQueuePopEntry(&pMyObject->RecordTable[i]);

            pRepRecord->Remove((ANSC_HANDLE)pRepRecord);
        }
    }

    pMyObject->CleanUpRroArray((ANSC_HANDLE)pMyObject);

    AnscReleaseLock(&pMyObject->RecordTableLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoAddRfoIntoArray
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSubFolder
            );

    description:

        This function is called to add a sub-folder object into the
        sorting array.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSubFolder
                Specifies the sub-folder object to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoAddRfoIntoArray
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSubFolder
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject       = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder      = (PSYS_REPOSITORY_FOLDER_OBJECT  )hSubFolder;
    PSYS_REPOSITORY_FOLDER_OBJECT   pTmpFolder      = (PSYS_REPOSITORY_FOLDER_OBJECT  )NULL;
    ANSC_HANDLE*                    pNewFolderArray = NULL;
    ULONG                           ulArrayNewLen   = 0;
    ULONG                           ulFolderIndex   = 0;
    ULONG                           i               = 0;
    ULONG                           j               = 0;

    if ( !pMyObject->FolderArray )
    {
        ulArrayNewLen   = pMyObject->FolderArrayMaxLen;
        pNewFolderArray = (ANSC_HANDLE*)AnscAllocateMemory(sizeof(ANSC_HANDLE) * ulArrayNewLen);

        if ( !pNewFolderArray )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->FolderArray       = pNewFolderArray;
            pMyObject->FolderArrayMaxLen = ulArrayNewLen;
            pMyObject->FolderArrayCurLen = 0;
        }
    }
    else if ( pMyObject->FolderArrayCurLen >= pMyObject->FolderArrayMaxLen )
    {
        ulArrayNewLen   = pMyObject->FolderArrayMaxLen + SYS_RFO_DEF_ARRAY_SIZE;
        pNewFolderArray = (ANSC_HANDLE*)AnscAllocateMemory(sizeof(ANSC_HANDLE) * ulArrayNewLen);

        if ( !pNewFolderArray )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            AnscCopyMemory
                (
                    pNewFolderArray,
                    pMyObject->FolderArray,
                    sizeof(ANSC_HANDLE) * pMyObject->FolderArrayCurLen
                );

            AnscFreeMemory(pMyObject->FolderArray);

            pMyObject->FolderArray       = pNewFolderArray;
            pMyObject->FolderArrayMaxLen = ulArrayNewLen;
        }
    }

    switch ( pMyObject->FolderSortingType )
    {
        case    SYS_RFO_SORTING_TYPE_BYTIME :

                ulFolderIndex                         = pMyObject->FolderArrayCurLen;
                pMyObject->FolderArray[ulFolderIndex] = (ANSC_HANDLE)pCurFolder;
                pMyObject->FolderArrayCurLen++;

                break;

        case    SYS_RFO_SORTING_TYPE_BYNAME :

                for ( i = 0; i < pMyObject->FolderArrayCurLen; i++ )
                {
                    pTmpFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pMyObject->FolderArray[i];

                    if ( _ansc_strcmp
                            (
                                pCurFolder->GetFolderName((ANSC_HANDLE)pCurFolder),
                                pTmpFolder->GetFolderName((ANSC_HANDLE)pTmpFolder)
                            ) < 0 )
                    {
                        break;
                    }
                }

                for ( j = pMyObject->FolderArrayCurLen; j > i ; j-- )
                {
                    pMyObject->FolderArray[j] = pMyObject->FolderArray[j - 1];
                }

                ulFolderIndex                         = i;
                pMyObject->FolderArray[ulFolderIndex] = (ANSC_HANDLE)pCurFolder;
                pMyObject->FolderArrayCurLen++;

                break;

        default :

                ulFolderIndex                         = pMyObject->FolderArrayCurLen;
                pMyObject->FolderArray[ulFolderIndex] = (ANSC_HANDLE)pCurFolder;
                pMyObject->FolderArrayCurLen++;

                break;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoDelRfoFromArray
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSubFolder
            );

    description:

        This function is called to delete a sub-folder object from the
        sorting array.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSubFolder
                Specifies the sub-folder object to be deleted.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoDelRfoFromArray
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSubFolder
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_FOLDER_OBJECT   pCurFolder   = (PSYS_REPOSITORY_FOLDER_OBJECT  )hSubFolder;
    ULONG                           i            = 0;
    ULONG                           j            = 0;

    if ( !pMyObject->FolderArray )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else if ( pMyObject->FolderArrayCurLen == 0 )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    for ( i = 0; i < pMyObject->FolderArrayCurLen; i++ )
    {
        if ( pMyObject->FolderArray[i] == (ANSC_HANDLE)pCurFolder )
        {
            break;
        }
    }

    if ( i >= pMyObject->FolderArrayCurLen )
    {
        return  ANSC_STATUS_CANT_FIND;
    }
    else
    {
        for ( j = i; j < (pMyObject->FolderArrayCurLen - 1); j++ )
        {
            pMyObject->FolderArray[j] = pMyObject->FolderArray[j + 1];
        }

        pMyObject->FolderArrayCurLen--;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoCleanUpRfoArray
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to clean up the sorting array.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoCleanUpRfoArray
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;

    if ( !pMyObject->FolderArray )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->FolderArrayCurLen = 0;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoAddRroIntoArray
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hRecord
            );

    description:

        This function is called to add a new record object into the
        sorting array.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hRecord
                Specifies the new record object to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoAddRroIntoArray
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRecord
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject       = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_RECORD_OBJECT   pCurRecord      = (PSYS_REPOSITORY_RECORD_OBJECT  )hRecord;
    PSYS_REPOSITORY_RECORD_OBJECT   pTmpRecord      = (PSYS_REPOSITORY_RECORD_OBJECT  )NULL;
    ANSC_HANDLE*                    pNewRecordArray = NULL;
    ULONG                           ulArrayNewLen   = 0;
    ULONG                           ulRecordIndex   = 0;
    ULONG                           i               = 0;
    ULONG                           j               = 0;

    if ( !pMyObject->RecordArray )
    {
        ulArrayNewLen   = pMyObject->RecordArrayMaxLen;
        pNewRecordArray = (ANSC_HANDLE*)AnscAllocateMemory(sizeof(ANSC_HANDLE) * ulArrayNewLen);

        if ( !pNewRecordArray )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->RecordArray       = pNewRecordArray;
            pMyObject->RecordArrayMaxLen = ulArrayNewLen;
            pMyObject->RecordArrayCurLen = 0;
        }
    }
    else if ( pMyObject->RecordArrayCurLen >= pMyObject->RecordArrayMaxLen )
    {
        ulArrayNewLen   = pMyObject->RecordArrayMaxLen + SYS_RFO_DEF_ARRAY_SIZE;
        pNewRecordArray = (ANSC_HANDLE*)AnscAllocateMemory(sizeof(ANSC_HANDLE) * ulArrayNewLen);

        if ( !pNewRecordArray )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            AnscCopyMemory
                (
                    pNewRecordArray,
                    pMyObject->RecordArray,
                    sizeof(ANSC_HANDLE) * pMyObject->RecordArrayCurLen
                );

            AnscFreeMemory(pMyObject->RecordArray);

            pMyObject->RecordArray       = pNewRecordArray;
            pMyObject->RecordArrayMaxLen = ulArrayNewLen;
        }
    }

    switch ( pMyObject->RecordSortingType )
    {
        case    SYS_RFO_SORTING_TYPE_BYTIME :

                ulRecordIndex                         = pMyObject->RecordArrayCurLen;
                pMyObject->RecordArray[ulRecordIndex] = (ANSC_HANDLE)pCurRecord;
                pMyObject->RecordArrayCurLen++;

                break;

        case    SYS_RFO_SORTING_TYPE_BYNAME :

                for ( i = 0; i < pMyObject->RecordArrayCurLen; i++ )
                {
                    pTmpRecord = (PSYS_REPOSITORY_RECORD_OBJECT)pMyObject->RecordArray[i];

                    if ( _ansc_strcmp
                            (
                                pCurRecord->GetRecordName((ANSC_HANDLE)pCurRecord),
                                pTmpRecord->GetRecordName((ANSC_HANDLE)pTmpRecord)
                            ) < 0 )
                    {
                        break;
                    }
                }

                for ( j = pMyObject->RecordArrayCurLen; j > i ; j-- )
                {
                    pMyObject->RecordArray[j] = pMyObject->RecordArray[j - 1];
                }

                ulRecordIndex                         = i;
                pMyObject->RecordArray[ulRecordIndex] = (ANSC_HANDLE)pCurRecord;
                pMyObject->RecordArrayCurLen++;

                break;

        default :

                ulRecordIndex                         = pMyObject->RecordArrayCurLen;
                pMyObject->RecordArray[ulRecordIndex] = (ANSC_HANDLE)pCurRecord;
                pMyObject->RecordArrayCurLen++;

                break;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoDelRroFromArray
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hRecord
            );

    description:

        This function is called to delete a record object from the
        sorting array.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hRecord
                Specifies the record object to be deleted.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoDelRroFromArray
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRecord
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_RECORD_OBJECT   pCurRecord   = (PSYS_REPOSITORY_RECORD_OBJECT  )hRecord;
    ULONG                           i            = 0;
    ULONG                           j            = 0;

    if ( !pMyObject->RecordArray )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else if ( pMyObject->RecordArrayCurLen == 0 )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    for ( i = 0; i < pMyObject->RecordArrayCurLen; i++ )
    {
        if ( pMyObject->RecordArray[i] == (ANSC_HANDLE)pCurRecord )
        {
            break;
        }
    }

    if ( i >= pMyObject->RecordArrayCurLen )
    {
        return  ANSC_STATUS_CANT_FIND;
    }
    else
    {
        for ( j = i; j < (pMyObject->RecordArrayCurLen - 1); j++ )
        {
            pMyObject->RecordArray[j] = pMyObject->RecordArray[j + 1];
        }

        pMyObject->RecordArrayCurLen--;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoCleanUpRroArray
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to clean up the sorting array.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoCleanUpRroArray
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject    = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;

    if ( !pMyObject->RecordArray )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->RecordArrayCurLen = 0;
    }

    return  ANSC_STATUS_SUCCESS;
}
