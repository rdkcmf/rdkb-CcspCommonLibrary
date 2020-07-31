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

    module:	sys_rbo_management.c

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the Sys Recycle Bin Object.

        *   SysRboDelAllFolders
        *   SysRboDelAllRecords

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

        ANSC_STATUS
        SysRboDelAllFolders
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
SysRboDelAllFolders
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_RECYCLE_BIN_OBJECT         pMyObject    = (PSYS_RECYCLE_BIN_OBJECT  )hThisObject;
    PSYS_REPOSITORY_FOLDER_OBJECT   pSubFolder   = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->FolderQueueLock);

    pSLinkEntry = AnscQueuePopEntry(&pMyObject->FolderQueue);

    while ( pSLinkEntry )
    {
        pSubFolder  = ACCESS_SYS_REPOSITORY_FOLDER_OBJECT(pSLinkEntry);
        pSLinkEntry = AnscQueuePopEntry(&pMyObject->FolderQueue);

        pSubFolder->AcquireAccess((ANSC_HANDLE)pSubFolder);
        pSubFolder->ReleaseAccess((ANSC_HANDLE)pSubFolder);
        pSubFolder->Remove       ((ANSC_HANDLE)pSubFolder);
    }

    AnscReleaseLock(&pMyObject->FolderQueueLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysRboDelAllRecords
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
SysRboDelAllRecords
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSYS_RECYCLE_BIN_OBJECT         pMyObject    = (PSYS_RECYCLE_BIN_OBJECT  )hThisObject;
    PSYS_REPOSITORY_RECORD_OBJECT   pRepRecord   = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->RecordQueueLock);

    pSLinkEntry = AnscQueuePopEntry(&pMyObject->RecordQueue);

    while ( pSLinkEntry )
    {
        pRepRecord  = ACCESS_SYS_REPOSITORY_RECORD_OBJECT(pSLinkEntry);
        pSLinkEntry = AnscQueuePopEntry(&pMyObject->RecordQueue);

        pRepRecord->Remove((ANSC_HANDLE)pRepRecord);
    }

    AnscReleaseLock(&pMyObject->RecordQueueLock);

    return  ANSC_STATUS_SUCCESS;
}
