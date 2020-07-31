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

    module:	sys_rfo_sorting.c

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced sorting functions
        of the Sys Repository Folder Object.

        *   SysRfoSortFolderByTime
        *   SysRfoSortFolderByName
        *   SysRfoSortRecordByTime
        *   SysRfoSortRecordByName

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/23/02    initial revision.

**********************************************************************/


#include "sys_rfo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoSortFolderByTime
            (
                ANSC_HANDLE                 hThisObject,
                BOOL                        bAscending
            );

    description:

        This function is called to sort the folder array.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                BOOL                        bAscending
                Specifies whether the order is up or down.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoSortFolderByTime
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bAscending
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject       = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_FOLDER_OBJECT   pTbsFolder      = (PSYS_REPOSITORY_FOLDER_OBJECT  )NULL;
    PSYS_REPOSITORY_FOLDER_OBJECT   pTmpFolder      = (PSYS_REPOSITORY_FOLDER_OBJECT  )NULL;
    ANSC_HANDLE*                    pTmpFolderArray = NULL;
    ULONG                           ulTmpCount      = 0;
    ULONG                           i               = 0;
    ULONG                           j               = 0;
    ULONG                           k               = 0;

    if ( bAscending )
    {
        pMyObject->FolderSortingMode = SYS_RFO_SORTING_MODE_ASCENDING;
    }
    else
    {
        pMyObject->FolderSortingMode = SYS_RFO_SORTING_MODE_DESCENDING;
    }

    if ( pMyObject->FolderSortingType == SYS_RFO_SORTING_TYPE_BYTIME )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->FolderSortingType = SYS_RFO_SORTING_TYPE_BYTIME;
    }

    if ( !pMyObject->FolderArray || (pMyObject->FolderArrayCurLen == 0) )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pTmpFolderArray = (ANSC_HANDLE*)AnscAllocateMemory(sizeof(ANSC_HANDLE) * pMyObject->FolderArrayMaxLen);

        if ( !pTmpFolderArray )
        {
            return  ANSC_STATUS_RESOURCES;
        }
    }

    AnscAcquireLock(&pMyObject->FolderTableLock);

    for ( k = 0; k < pMyObject->FolderArrayCurLen; k++ )
    {
        pTbsFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pMyObject->FolderArray[k];

        for ( i = 0; i < ulTmpCount; i++ )
        {
            pTmpFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pTmpFolderArray[i];

            if ( pTbsFolder->GetBirthTime((ANSC_HANDLE)pTbsFolder) < pTmpFolder->GetBirthTime((ANSC_HANDLE)pTmpFolder) )
            {
                break;
            }
        }

        for ( j = ulTmpCount; j > i ; j-- )
        {
            pTmpFolderArray[j] = pTmpFolderArray[j - 1];
        }

        pTmpFolderArray[i] = (ANSC_HANDLE)pTbsFolder;
        ulTmpCount++;
    }

    AnscFreeMemory(pMyObject->FolderArray);

    pMyObject->FolderArray = pTmpFolderArray;

    AnscReleaseLock(&pMyObject->FolderTableLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoSortFolderByName
            (
                ANSC_HANDLE                 hThisObject,
                BOOL                        bAscending
            );

    description:

        This function is called to sort the folder array.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                BOOL                        bAscending
                Specifies whether the order is up or down.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoSortFolderByName
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bAscending
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject       = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_FOLDER_OBJECT   pTbsFolder      = (PSYS_REPOSITORY_FOLDER_OBJECT  )NULL;
    PSYS_REPOSITORY_FOLDER_OBJECT   pTmpFolder      = (PSYS_REPOSITORY_FOLDER_OBJECT  )NULL;
    ANSC_HANDLE*                    pTmpFolderArray = NULL;
    ULONG                           ulTmpCount      = 0;
    ULONG                           i               = 0;
    ULONG                           j               = 0;
    ULONG                           k               = 0;

    if ( bAscending )
    {
        pMyObject->FolderSortingMode = SYS_RFO_SORTING_MODE_ASCENDING;
    }
    else
    {
        pMyObject->FolderSortingMode = SYS_RFO_SORTING_MODE_DESCENDING;
    }

    if ( pMyObject->FolderSortingType == SYS_RFO_SORTING_TYPE_BYNAME )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->FolderSortingType = SYS_RFO_SORTING_TYPE_BYNAME;
    }

    if ( !pMyObject->FolderArray || (pMyObject->FolderArrayCurLen == 0) )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pTmpFolderArray = (ANSC_HANDLE*)AnscAllocateMemory(sizeof(ANSC_HANDLE) * pMyObject->FolderArrayMaxLen);

        if ( !pTmpFolderArray )
        {
            return  ANSC_STATUS_RESOURCES;
        }
    }

    AnscAcquireLock(&pMyObject->FolderTableLock);

    for ( k = 0; k < pMyObject->FolderArrayCurLen; k++ )
    {
        pTbsFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pMyObject->FolderArray[k];

        for ( i = 0; i < ulTmpCount; i++ )
        {
            pTmpFolder = (PSYS_REPOSITORY_FOLDER_OBJECT)pTmpFolderArray[i];

            if ( _ansc_strcmp
                    (
                        pTbsFolder->GetFolderName((ANSC_HANDLE)pTbsFolder),
                        pTmpFolder->GetFolderName((ANSC_HANDLE)pTmpFolder)
                    ) < 0 )
            {
                break;
            }
        }

        for ( j = ulTmpCount; j > i ; j-- )
        {
            pTmpFolderArray[j] = pTmpFolderArray[j - 1];
        }

        pTmpFolderArray[i] = (ANSC_HANDLE)pTbsFolder;
        ulTmpCount++;
    }

    AnscFreeMemory(pMyObject->FolderArray);

    pMyObject->FolderArray = pTmpFolderArray;

    AnscReleaseLock(&pMyObject->FolderTableLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoSortRecordByTime
            (
                ANSC_HANDLE                 hThisObject,
                BOOL                        bAscending
            );

    description:

        This function is called to sort the record array.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                BOOL                        bAscending
                Specifies whether the order is up or down.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoSortRecordByTime
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bAscending
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject       = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_RECORD_OBJECT   pTbsRecord      = (PSYS_REPOSITORY_RECORD_OBJECT  )NULL;
    PSYS_REPOSITORY_RECORD_OBJECT   pTmpRecord      = (PSYS_REPOSITORY_RECORD_OBJECT  )NULL;
    ANSC_HANDLE*                    pTmpRecordArray = NULL;
    ULONG                           ulTmpCount      = 0;
    ULONG                           i               = 0;
    ULONG                           j               = 0;
    ULONG                           k               = 0;

    if ( bAscending )
    {
        pMyObject->RecordSortingMode = SYS_RFO_SORTING_MODE_ASCENDING;
    }
    else
    {
        pMyObject->RecordSortingMode = SYS_RFO_SORTING_MODE_DESCENDING;
    }

    if ( pMyObject->RecordSortingType == SYS_RFO_SORTING_TYPE_BYTIME )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->RecordSortingType = SYS_RFO_SORTING_TYPE_BYTIME;
    }

    if ( !pMyObject->RecordArray || (pMyObject->RecordArrayCurLen == 0) )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pTmpRecordArray = (ANSC_HANDLE*)AnscAllocateMemory(sizeof(ANSC_HANDLE) * pMyObject->RecordArrayMaxLen);

        if ( !pTmpRecordArray )
        {
            return  ANSC_STATUS_RESOURCES;
        }
    }

    AnscAcquireLock(&pMyObject->RecordTableLock);

    for ( k = 0; k < pMyObject->RecordArrayCurLen; k++ )
    {
        pTbsRecord = (PSYS_REPOSITORY_RECORD_OBJECT)pMyObject->RecordArray[k];

        for ( i = 0; i < ulTmpCount; i++ )
        {
            pTmpRecord = (PSYS_REPOSITORY_RECORD_OBJECT)pTmpRecordArray[i];

            if ( pTbsRecord->GetBirthTime((ANSC_HANDLE)pTbsRecord) < pTmpRecord->GetBirthTime((ANSC_HANDLE)pTmpRecord) )
            {
                break;
            }
        }

        for ( j = ulTmpCount; j > i ; j-- )
        {
            pTmpRecordArray[j] = pTmpRecordArray[j - 1];
        }

        pTmpRecordArray[i] = (ANSC_HANDLE)pTbsRecord;
        ulTmpCount++;
    }

    AnscFreeMemory(pMyObject->RecordArray);

    pMyObject->RecordArray = pTmpRecordArray;

    AnscReleaseLock(&pMyObject->RecordTableLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRfoSortRecordByName
            (
                ANSC_HANDLE                 hThisObject,
                BOOL                        bAscending
            );

    description:

        This function is called to sort the record array.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                BOOL                        bAscending
                Specifies whether the order is up or down.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRfoSortRecordByName
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bAscending
    )
{
    PSYS_REPOSITORY_FOLDER_OBJECT   pMyObject       = (PSYS_REPOSITORY_FOLDER_OBJECT  )hThisObject;
    PSYS_REPOSITORY_RECORD_OBJECT   pTbsRecord      = (PSYS_REPOSITORY_RECORD_OBJECT  )NULL;
    PSYS_REPOSITORY_RECORD_OBJECT   pTmpRecord      = (PSYS_REPOSITORY_RECORD_OBJECT  )NULL;
    ANSC_HANDLE*                    pTmpRecordArray = NULL;
    ULONG                           ulTmpCount      = 0;
    ULONG                           i               = 0;
    ULONG                           j               = 0;
    ULONG                           k               = 0;

    if ( bAscending )
    {
        pMyObject->RecordSortingMode = SYS_RFO_SORTING_MODE_ASCENDING;
    }
    else
    {
        pMyObject->RecordSortingMode = SYS_RFO_SORTING_MODE_DESCENDING;
    }

    if ( pMyObject->RecordSortingType == SYS_RFO_SORTING_TYPE_BYNAME )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->RecordSortingType = SYS_RFO_SORTING_TYPE_BYNAME;
    }

    if ( !pMyObject->RecordArray || (pMyObject->RecordArrayCurLen == 0) )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pTmpRecordArray = (ANSC_HANDLE*)AnscAllocateMemory(sizeof(ANSC_HANDLE) * pMyObject->RecordArrayMaxLen);

        if ( !pTmpRecordArray )
        {
            return  ANSC_STATUS_RESOURCES;
        }
    }

    AnscAcquireLock(&pMyObject->RecordTableLock);

    for ( k = 0; k < pMyObject->RecordArrayCurLen; k++ )
    {
        pTbsRecord = (PSYS_REPOSITORY_RECORD_OBJECT)pMyObject->RecordArray[k];

        for ( i = 0; i < ulTmpCount; i++ )
        {
            pTmpRecord = (PSYS_REPOSITORY_RECORD_OBJECT)pTmpRecordArray[i];

            if ( _ansc_strcmp
                    (
                        pTbsRecord->GetRecordName((ANSC_HANDLE)pTbsRecord),
                        pTmpRecord->GetRecordName((ANSC_HANDLE)pTmpRecord)
                    ) < 0 )
            {
                break;
            }
        }

        for ( j = ulTmpCount; j > i ; j-- )
        {
            pTmpRecordArray[j] = pTmpRecordArray[j - 1];
        }

        pTmpRecordArray[i] = (ANSC_HANDLE)pTbsRecord;
        ulTmpCount++;
    }

    AnscFreeMemory(pMyObject->RecordArray);

    pMyObject->RecordArray = pTmpRecordArray;

    AnscReleaseLock(&pMyObject->RecordTableLock);

    return  ANSC_STATUS_SUCCESS;
}
