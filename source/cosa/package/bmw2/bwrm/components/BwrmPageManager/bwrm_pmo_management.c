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

    module:	bwrm_pmo_management.c

        For Broadway Web Resource Manager Implementation (BWRM),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the Bwrm Page Manager Object.

        *   BwrmPmoGetPageCount
        *   BwrmPmoGetOldestPage
        *   BwrmPmoGetPage
        *   BwrmPmoAddPage
        *   BwrmPmoDelPage
        *   BwrmPmoDelAllPages

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/01/03    initial revision.

**********************************************************************/


#include "bwrm_pmo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        BwrmPmoGetPageCount
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the number of pages.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     page count.

**********************************************************************/

ULONG
BwrmPmoGetPageCount
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWRM_PAGE_MANAGER_OBJECT       pMyObject         = (PBWRM_PAGE_MANAGER_OBJECT    )hThisObject;
    ULONG                           ulPageCount       = 0;
    ULONG                           i                 = 0;

    AnscAcquireLock(&pMyObject->CpoTableLock);

    for ( i = 0; i < BWRM_PMO_CPO_TABLE_SIZE; i++ )
    {
        ulPageCount += AnscSListQueryDepth(&pMyObject->CpoTable[i]);
    }

    AnscReleaseLock(&pMyObject->CpoTableLock);

    return  ulPageCount;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BwrmPmoGetOldestPage
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the oldest Cooked Page
        Object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     cooked page.

**********************************************************************/

ANSC_HANDLE
BwrmPmoGetOldestPage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWRM_PAGE_MANAGER_OBJECT       pMyObject         = (PBWRM_PAGE_MANAGER_OBJECT    )hThisObject;
    PBWRM_COOKED_PAGE_OBJECT        pOldestCookedPage = (PBWRM_COOKED_PAGE_OBJECT     )NULL;
    PBWRM_COOKED_PAGE_OBJECT        pCookedPage       = (PBWRM_COOKED_PAGE_OBJECT     )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY           )NULL;
    ULONG                           ulOldestTimestamp = 0xFFFFFFFF;
    ULONG                           ulTempTimestamp   = 0;
    ULONG                           i                 = 0;

    AnscAcquireLock(&pMyObject->CpoTableLock);

    for ( i = 0; i < BWRM_PMO_CPO_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->CpoTable[i]);

        while ( pSLinkEntry )
        {
            pCookedPage     = ACCESS_BWRM_COOKED_PAGE_OBJECT(pSLinkEntry);
            pSLinkEntry     = AnscSListGetNextEntry(pSLinkEntry);
            ulTempTimestamp = pCookedPage->GetTimestamp((ANSC_HANDLE)pCookedPage);

            if ( ulTempTimestamp < ulOldestTimestamp )
            {
                ulOldestTimestamp = ulTempTimestamp;
                pOldestCookedPage = pCookedPage;
            }
        }
    }

    AnscReleaseLock(&pMyObject->CpoTableLock);

    return  (ANSC_HANDLE)pOldestCookedPage;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BwrmPmoGetPage
            (
                ANSC_HANDLE                 hThisObject,
                char*                       root_path,
                char*                       file_path
            );

    description:

        This function is called to retrieve a Cooked Page Object by
        matching the root_path and file_path.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       root_path
                Specifies the root_path to be matched.

                char*                       file_path
                Specifies the file_path to be matched.

    return:     cooked page.

**********************************************************************/

ANSC_HANDLE
BwrmPmoGetPage
    (
        ANSC_HANDLE                 hThisObject,
        char*                       root_path,
        char*                       file_path
    )
{
    PBWRM_PAGE_MANAGER_OBJECT       pMyObject         = (PBWRM_PAGE_MANAGER_OBJECT    )hThisObject;
    PBWRM_COOKED_PAGE_OBJECT        pCookedPage       = (PBWRM_COOKED_PAGE_OBJECT     )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY           )NULL;
    ULONG                           ulHashIndex       = (ULONG                        )AnscHashString(file_path, AnscSizeOfString(file_path), BWRM_PMO_CPO_TABLE_SIZE);
    ULONG                           ulCurTime         = AnscGetTickInSeconds();

    pMyObject->Timestamp = ulCurTime;

    AnscAcquireLock(&pMyObject->CpoTableLock);

    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->CpoTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pCookedPage = ACCESS_BWRM_COOKED_PAGE_OBJECT(pSLinkEntry);
        pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);

        if ( pCookedPage->MatchPath
                (
                    (ANSC_HANDLE)pCookedPage,
                    root_path,
                    file_path
                ) )
        {
            pCookedPage->SetTimestamp((ANSC_HANDLE)pCookedPage, ulCurTime);
            pCookedPage->IncRefCount ((ANSC_HANDLE)pCookedPage);

            AnscReleaseLock(&pMyObject->CpoTableLock);

            return  (ANSC_HANDLE)pCookedPage;
        }
    }

    AnscReleaseLock(&pMyObject->CpoTableLock);

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmPmoAddPage
            (
                ANSC_HANDLE                 hThisObject,
                char*                       root_path,
                char*                       file_path,
                ANSC_HANDLE                 hCookedPage
            );

    description:

        This function is called to add a Cooked Page Object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       root_path
                Specifies the root_path of the Cooked Page object.

                char*                       file_path
                Specifies the file_path of the Cooked Page object.

                ANSC_HANDLE                 hCookedPage
                Specifies the Cooked Page object to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BwrmPmoAddPage
    (
        ANSC_HANDLE                 hThisObject,
        char*                       root_path,
        char*                       file_path,
        ANSC_HANDLE                 hCookedPage
    )
{
    PBWRM_PAGE_MANAGER_OBJECT       pMyObject         = (PBWRM_PAGE_MANAGER_OBJECT    )hThisObject;
    PBWRM_PAGE_MANAGER_PROPERTY     pProperty         = (PBWRM_PAGE_MANAGER_PROPERTY  )&pMyObject->Property;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pCacheTimerObject = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hCacheTimerObject;
    PBWRM_COOKED_PAGE_OBJECT        pCookedPage       = (PBWRM_COOKED_PAGE_OBJECT     )hCookedPage;
    ULONG                           ulHashIndex       = (ULONG                        )AnscHashString(file_path, AnscSizeOfString(file_path), BWRM_PMO_CPO_TABLE_SIZE);
    ULONG                           ulPageCount       = 0;
    ULONG                           ulCurTime         = AnscGetTickInSeconds();

    pMyObject->Timestamp = ulCurTime;

    pCookedPage->SetRootPath ((ANSC_HANDLE)pCookedPage, root_path);
    pCookedPage->SetPagePath ((ANSC_HANDLE)pCookedPage, file_path);
    pCookedPage->SetTimestamp((ANSC_HANDLE)pCookedPage, ulCurTime);
    pCookedPage->IncRefCount ((ANSC_HANDLE)pCookedPage           );

    AnscAcquireLock   (&pMyObject->CpoTableLock);
    AnscSListPushEntry(&pMyObject->CpoTable[ulHashIndex], &pCookedPage->Linkage);
    AnscReleaseLock   (&pMyObject->CpoTableLock);

    if ( TRUE )
    {
        ulPageCount = pMyObject->GetPageCount((ANSC_HANDLE)pMyObject);

        if ( ulPageCount == 1 )
        {
            pCacheTimerObject->Start((ANSC_HANDLE)pCacheTimerObject);
        }
        else if ( ulPageCount > pProperty->CacheEntryCount )
        {
            pCookedPage = (PBWRM_COOKED_PAGE_OBJECT)pMyObject->GetOldestPage((ANSC_HANDLE)pMyObject);

            if ( pCookedPage && 0 == pCookedPage->GetRefCount((ANSC_HANDLE)pCookedPage) )
            {
                    pMyObject->DelPage
                        (
                            (ANSC_HANDLE)pMyObject,
                            pCookedPage->GetRootPath((ANSC_HANDLE)pCookedPage),
                            pCookedPage->GetPagePath((ANSC_HANDLE)pCookedPage)
                        );
            }
        }
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmPmoDelPage
            (
                ANSC_HANDLE                 hThisObject,
                char*                       root_path,
                char*                       file_path
            );

    description:

        This function is called to delete a Cooked Page Object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       root_path
                Specifies the root_path of the Cooked Page object.

                char*                       file_path
                Specifies the file_path of the Cooked Page object.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BwrmPmoDelPage
    (
        ANSC_HANDLE                 hThisObject,
        char*                       root_path,
        char*                       file_path
    )
{
    PBWRM_PAGE_MANAGER_OBJECT       pMyObject         = (PBWRM_PAGE_MANAGER_OBJECT    )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pCacheTimerObject = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hCacheTimerObject;
    PBWRM_COOKED_PAGE_OBJECT        pCookedPage       = (PBWRM_COOKED_PAGE_OBJECT     )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY           )NULL;
    ULONG                           ulHashIndex       = (ULONG                        )AnscHashString(file_path, AnscSizeOfString(file_path), BWRM_PMO_CPO_TABLE_SIZE);

    AnscAcquireLock(&pMyObject->CpoTableLock);

    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->CpoTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pCookedPage = ACCESS_BWRM_COOKED_PAGE_OBJECT(pSLinkEntry);
        pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);

        if ( pCookedPage->MatchPath
                (
                    (ANSC_HANDLE)pCookedPage,
                    root_path,
                    file_path
                ) )
        {
            AnscSListPopEntryByLink(&pMyObject->CpoTable[ulHashIndex], &pCookedPage->Linkage);

            pCookedPage->Remove((ANSC_HANDLE)pCookedPage);

            break;
        }
    }

    AnscReleaseLock(&pMyObject->CpoTableLock);

    if ( pMyObject->GetPageCount((ANSC_HANDLE)pMyObject) == 0 )
    {
        pCacheTimerObject->Stop((ANSC_HANDLE)pCacheTimerObject);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmPmoDelAllPages
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to delete all Cooked Page Objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BwrmPmoDelAllPages
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWRM_PAGE_MANAGER_OBJECT       pMyObject         = (PBWRM_PAGE_MANAGER_OBJECT    )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pCacheTimerObject = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hCacheTimerObject;
    PBWRM_COOKED_PAGE_OBJECT        pCookedPage       = (PBWRM_COOKED_PAGE_OBJECT     )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY           )NULL;
    ULONG                           i                 = 0;
    ULONG                           ulPageInUse       = 0;

    AnscAcquireLock(&pMyObject->CpoTableLock);

    for ( i = 0; i < BWRM_PMO_CPO_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscSListPopEntry(&pMyObject->CpoTable[i]);

        while ( pSLinkEntry )
        {
            pCookedPage = ACCESS_BWRM_COOKED_PAGE_OBJECT(pSLinkEntry);
            pSLinkEntry = AnscSListPopEntry(&pMyObject->CpoTable[i]);

            if ( 0 == pCookedPage->GetRefCount((ANSC_HANDLE)pCookedPage) )
            {
                pCookedPage->Remove((ANSC_HANDLE)pCookedPage);
            }
            else
            {
                ulPageInUse ++;
            }
        }
    }

    AnscReleaseLock(&pMyObject->CpoTableLock);

    if ( 0 == ulPageInUse )
    {
        pCacheTimerObject->Stop((ANSC_HANDLE)pCacheTimerObject);
    }

    return  ANSC_STATUS_SUCCESS;
}
