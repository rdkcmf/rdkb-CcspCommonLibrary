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

    module:	sys_rbo_access.c

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced field-access functions
        of the Sys Recycle Bin Object.

        *   SysRboGetFolderCount
        *   SysRboGetRecordCount
        *   SysRboAddFolder
        *   SysRboGetFolderByIndex
        *   SysRboAddRecord
        *   SysRboGetRecordByIndex

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/21/02    initial revision.

**********************************************************************/


#include "sys_rbo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SysRboGetFolderCount
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
SysRboGetFolderCount
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_RECYCLE_BIN_OBJECT         pMyObject     = (PSYS_RECYCLE_BIN_OBJECT  )hThisObject;
    ULONG                           ulFolderCount = 0;

    AnscAcquireLock(&pMyObject->FolderQueueLock);
    ulFolderCount = AnscQueueQueryDepth(&pMyObject->FolderQueue);
    AnscReleaseLock(&pMyObject->FolderQueueLock);

    return  ulFolderCount;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SysRboGetRecordCount
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
SysRboGetRecordCount
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_RECYCLE_BIN_OBJECT         pMyObject     = (PSYS_RECYCLE_BIN_OBJECT  )hThisObject;
    ULONG                           ulRecordCount = 0;

    AnscAcquireLock(&pMyObject->RecordQueueLock);
    ulRecordCount = AnscQueueQueryDepth(&pMyObject->RecordQueue);
    AnscReleaseLock(&pMyObject->RecordQueueLock);

    return  ulRecordCount;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRboAddFolder
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hFolder
            );

    description:

        This function is called to add a folder object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hFolder
                Specifies a folder object to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRboAddFolder
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFolder
    )
{
    PSYS_RECYCLE_BIN_OBJECT         pMyObject    = (PSYS_RECYCLE_BIN_OBJECT      )hThisObject;
    PSYS_REPOSITORY_FOLDER_OBJECT   pRepFolder   = (PSYS_REPOSITORY_FOLDER_OBJECT)hFolder;

    pRepFolder->Timestamp = AnscGetTickInSeconds();

    AnscAcquireLock   (&pMyObject->FolderQueueLock);
    AnscQueuePushEntry(&pMyObject->FolderQueue, &pRepFolder->Linkage);
    AnscReleaseLock   (&pMyObject->FolderQueueLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysRboGetFolderByIndex
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
SysRboGetFolderByIndex
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex
    )
{
    PSYS_RECYCLE_BIN_OBJECT         pMyObject    = (PSYS_RECYCLE_BIN_OBJECT      )hThisObject;
    PSYS_REPOSITORY_FOLDER_OBJECT   pRepFolder   = (PSYS_REPOSITORY_FOLDER_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    if ( ulIndex >= AnscQueueQueryDepth(&pMyObject->FolderQueue) )
    {
        return  (ANSC_HANDLE)NULL;
    }

    AnscAcquireLock(&pMyObject->FolderQueueLock);
    pSLinkEntry = AnscQueueSearchEntryByIndex(&pMyObject->FolderQueue, ulIndex);
    AnscReleaseLock(&pMyObject->FolderQueueLock);

    if ( pSLinkEntry )
    {
        pRepFolder = ACCESS_SYS_REPOSITORY_FOLDER_OBJECT(pSLinkEntry);
    }

    return  (ANSC_HANDLE)pRepFolder;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRboAddRecord
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hRecord
            );

    description:

        This function is called to add a record object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hRecord
                Specifies a record object to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysRboAddRecord
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRecord
    )
{
    PSYS_RECYCLE_BIN_OBJECT         pMyObject    = (PSYS_RECYCLE_BIN_OBJECT      )hThisObject;
    PSYS_REPOSITORY_RECORD_OBJECT   pRepRecord   = (PSYS_REPOSITORY_RECORD_OBJECT)hRecord;

    pRepRecord->Timestamp = AnscGetTickInSeconds();

    AnscAcquireLock   (&pMyObject->RecordQueueLock);
    AnscQueuePushEntry(&pMyObject->RecordQueue, &pRepRecord->Linkage);
    AnscReleaseLock   (&pMyObject->RecordQueueLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysRboGetRecordByIndex
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
SysRboGetRecordByIndex
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex
    )
{
    PSYS_RECYCLE_BIN_OBJECT         pMyObject    = (PSYS_RECYCLE_BIN_OBJECT      )hThisObject;
    PSYS_REPOSITORY_RECORD_OBJECT   pRepRecord   = (PSYS_REPOSITORY_RECORD_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    if ( ulIndex >= AnscQueueQueryDepth(&pMyObject->RecordQueue) )
    {
        return  (ANSC_HANDLE)NULL;
    }

    AnscAcquireLock(&pMyObject->RecordQueueLock);
    pSLinkEntry = AnscQueueSearchEntryByIndex(&pMyObject->RecordQueue, ulIndex);
    AnscReleaseLock(&pMyObject->RecordQueueLock);

    if ( pSLinkEntry )
    {
        pRepRecord = ACCESS_SYS_REPOSITORY_RECORD_OBJECT(pSLinkEntry);
    }

    return  (ANSC_HANDLE)pRepRecord;
}
