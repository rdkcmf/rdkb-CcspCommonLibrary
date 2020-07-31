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

    module:	bwrm_eco_ramif.c

        For Broadway Web Resource Manager Implementation (BWRM),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced interface functions
        of the Bwrm Env Controller Object.

        *   BwrmEcoRamGetCacheNumber
        *   BwrmEcoRamSetCacheNumber
        *   BwrmEcoRamGetCacheSize
        *   BwrmEcoRamSetCacheSize
        *   BwrmEcoRamGetCacheTimeout
        *   BwrmEcoRamSetCacheTimeout
        *   BwrmEcoRamControlCache
        *   BwrmEcoRamGetRawPage
        *   BwrmEcoRamGetCookedPage
        *   BwrmEcoRamAddCookedPage
        *   BwrmEcoRamDelCookedPage
        *   BwrmEcoRamForcePageCache
        *   BwrmEcoRamIsPageCachedForced

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


#include "bwrm_eco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        BwrmEcoRamGetCacheNumber
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve cache property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     cache property.

**********************************************************************/

ULONG
BwrmEcoRamGetCacheNumber
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWRM_ENV_CONTROLLER_OBJECT     pMyObject        = (PBWRM_ENV_CONTROLLER_OBJECT  )hThisObject;
    PBWRM_PAGE_MANAGER_OBJECT       pBwrmPageManager = (PBWRM_PAGE_MANAGER_OBJECT    )pMyObject->hBwrmPageManager;

    return  pBwrmPageManager->GetCacheEntryCount((ANSC_HANDLE)pBwrmPageManager);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmEcoRamSetCacheNumber
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       cache_number
            );

    description:

        This function is called to configure cache property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       cache_number
                Specifies the cache property to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BwrmEcoRamSetCacheNumber
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       cache_number
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PBWRM_ENV_CONTROLLER_OBJECT     pMyObject        = (PBWRM_ENV_CONTROLLER_OBJECT  )hThisObject;
    PBWRM_PAGE_MANAGER_OBJECT       pBwrmPageManager = (PBWRM_PAGE_MANAGER_OBJECT    )pMyObject->hBwrmPageManager;

    returnStatus =
        pBwrmPageManager->SetCacheEntryCount
            (
                (ANSC_HANDLE)pBwrmPageManager,
                cache_number
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        BwrmEcoRamGetCacheSize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve cache property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     cache property.

**********************************************************************/

ULONG
BwrmEcoRamGetCacheSize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWRM_ENV_CONTROLLER_OBJECT     pMyObject        = (PBWRM_ENV_CONTROLLER_OBJECT  )hThisObject;
    PBWRM_PAGE_MANAGER_OBJECT       pBwrmPageManager = (PBWRM_PAGE_MANAGER_OBJECT    )pMyObject->hBwrmPageManager;

    return  pBwrmPageManager->GetCacheMemorySize((ANSC_HANDLE)pBwrmPageManager);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmEcoRamSetCacheSize
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       cache_size
            );

    description:

        This function is called to configure cache property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       cache_size
                Specifies the cache property to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BwrmEcoRamSetCacheSize
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       cache_size
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PBWRM_ENV_CONTROLLER_OBJECT     pMyObject        = (PBWRM_ENV_CONTROLLER_OBJECT  )hThisObject;
    PBWRM_PAGE_MANAGER_OBJECT       pBwrmPageManager = (PBWRM_PAGE_MANAGER_OBJECT    )pMyObject->hBwrmPageManager;

    returnStatus =
        pBwrmPageManager->SetCacheMemorySize
            (
                (ANSC_HANDLE)pBwrmPageManager,
                cache_size
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        BwrmEcoRamGetCacheTimeout
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve cache property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     cache property.

**********************************************************************/

ULONG
BwrmEcoRamGetCacheTimeout
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWRM_ENV_CONTROLLER_OBJECT     pMyObject        = (PBWRM_ENV_CONTROLLER_OBJECT  )hThisObject;
    PBWRM_PAGE_MANAGER_OBJECT       pBwrmPageManager = (PBWRM_PAGE_MANAGER_OBJECT    )pMyObject->hBwrmPageManager;

    return  pBwrmPageManager->GetCacheTimeout((ANSC_HANDLE)pBwrmPageManager);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmEcoRamSetCacheTimeout
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       cache_timeout
            );

    description:

        This function is called to configure cache property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       cache_timeout
                Specifies the cache property to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BwrmEcoRamSetCacheTimeout
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       cache_timeout
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PBWRM_ENV_CONTROLLER_OBJECT     pMyObject        = (PBWRM_ENV_CONTROLLER_OBJECT  )hThisObject;
    PBWRM_PAGE_MANAGER_OBJECT       pBwrmPageManager = (PBWRM_PAGE_MANAGER_OBJECT    )pMyObject->hBwrmPageManager;

    returnStatus =
        pBwrmPageManager->SetCacheTimeout
            (
                (ANSC_HANDLE)pBwrmPageManager,
                cache_timeout
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmEcoRamControlCache
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ctrl_code
            );

    description:

        This function is called to control the caching activity.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ctrl_code
                Specifies what type of control to be applied.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BwrmEcoRamControlCache
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ctrl_code
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PBWRM_ENV_CONTROLLER_OBJECT     pMyObject        = (PBWRM_ENV_CONTROLLER_OBJECT  )hThisObject;
    PBWRM_PAGE_MANAGER_OBJECT       pBwrmPageManager = (PBWRM_PAGE_MANAGER_OBJECT    )pMyObject->hBwrmPageManager;

    switch ( ctrl_code )
    {
        case    BWRM_RAM_CTRL_CODE_enable :

                returnStatus =
                    pBwrmPageManager->SetCacheEnabled
                        (
                            (ANSC_HANDLE)pBwrmPageManager,
                            TRUE
                        );

                break;

        case    BWRM_RAM_CTRL_CODE_disable :

                returnStatus =
                    pBwrmPageManager->SetCacheEnabled
                        (
                            (ANSC_HANDLE)pBwrmPageManager,
                            FALSE
                        );

                break;

        case    BWRM_RAM_CTRL_CODE_reset :

                returnStatus = pBwrmPageManager->DelAllPages((ANSC_HANDLE)pBwrmPageManager);

                break;

        default :

                returnStatus = ANSC_STATUS_UNAPPLICABLE;

                break;
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmEcoRamGetRawPage
            (
                ANSC_HANDLE                 hThisObject,
                char*                       root_path,
                char*                       page_path,
                void**                      page_data,
                PULONG                      page_size
            );

    description:

        This function is called to retrieve the raw page data.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       root_path
                Specifies root path of the page file to be retrieved.

                char*                       page_path
                Specifies file name of the page file to be retrieved.

                void**                      page_data
                Specifies the raw page data to be returned.

                PULONG                      page_size
                Specifies the raw page size to be returned.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BwrmEcoRamGetRawPage
    (
        ANSC_HANDLE                 hThisObject,
        char*                       root_path,
        char*                       page_path,
        void**                      page_data,
        PULONG                      page_size
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PBWRM_ENV_CONTROLLER_OBJECT     pMyObject        = (PBWRM_ENV_CONTROLLER_OBJECT  )hThisObject;
    PBWRM_FILE_MANAGER_OBJECT       pBwrmFileManager = (PBWRM_FILE_MANAGER_OBJECT    )pMyObject->hBwrmFileManager;

    returnStatus =
        pBwrmFileManager->GetFile
            (
                (ANSC_HANDLE)pBwrmFileManager,
                root_path,
                page_path,
                page_data,
                page_size
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmEcoRamGetCookedPage
            (
                ANSC_HANDLE                 hThisObject,
                char*                       root_path,
                char*                       page_path,
                ANSC_HANDLE*                phCookedPage
            );

    description:

        This function is called to retrieve the cooked page object from
        the cache.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       root_path
                Specifies root path of the page file to be retrieved.

                char*                       page_path
                Specifies file name of the page file to be retrieved.

                ANSC_HANDLE*                phCookedPage
                Specifies the cooked page object to be returned.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BwrmEcoRamGetCookedPage
    (
        ANSC_HANDLE                 hThisObject,
        char*                       root_path,
        char*                       page_path,
        ANSC_HANDLE*                phCookedPage
    )
{
    PBWRM_ENV_CONTROLLER_OBJECT     pMyObject        = (PBWRM_ENV_CONTROLLER_OBJECT  )hThisObject;
    PBWRM_PAGE_MANAGER_OBJECT       pBwrmPageManager = (PBWRM_PAGE_MANAGER_OBJECT    )pMyObject->hBwrmPageManager;

    *phCookedPage =
        pBwrmPageManager->GetPage
            (
                (ANSC_HANDLE)pBwrmPageManager,
                root_path,
                page_path
            );

    if ( !*phCookedPage )
    {
        return  ANSC_STATUS_CANT_FIND;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmEcoRamAddCookedPage
            (
                ANSC_HANDLE                 hThisObject,
                char*                       root_path,
                char*                       page_path,
                ANSC_HANDLE                 hCookedPage
            );

    description:

        This function is called to add the cooked page object into the
        cache.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       root_path
                Specifies root path of the page file to be added.

                char*                       page_path
                Specifies file name of the page file to be added.

                ANSC_HANDLE                 hCookedPage
                Specifies the cooked page object to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BwrmEcoRamAddCookedPage
    (
        ANSC_HANDLE                 hThisObject,
        char*                       root_path,
        char*                       page_path,
        ANSC_HANDLE                 hCookedPage
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PBWRM_ENV_CONTROLLER_OBJECT     pMyObject        = (PBWRM_ENV_CONTROLLER_OBJECT  )hThisObject;
    PBWRM_PAGE_MANAGER_OBJECT       pBwrmPageManager = (PBWRM_PAGE_MANAGER_OBJECT    )pMyObject->hBwrmPageManager;

    returnStatus =
        pBwrmPageManager->AddPage
            (
                (ANSC_HANDLE)pBwrmPageManager,
                root_path,
                page_path,
                hCookedPage
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmEcoRamDelCookedPage
            (
                ANSC_HANDLE                 hThisObject,
                char*                       root_path,
                char*                       page_path
            );

    description:

        This function is called to delete the cooked page object from
        the cache.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       root_path
                Specifies root path of the page file to be deleted.

                char*                       page_path
                Specifies file name of the page file to be deleted.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BwrmEcoRamDelCookedPage
    (
        ANSC_HANDLE                 hThisObject,
        char*                       root_path,
        char*                       page_path
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PBWRM_ENV_CONTROLLER_OBJECT     pMyObject        = (PBWRM_ENV_CONTROLLER_OBJECT  )hThisObject;
    PBWRM_PAGE_MANAGER_OBJECT       pBwrmPageManager = (PBWRM_PAGE_MANAGER_OBJECT    )pMyObject->hBwrmPageManager;

    returnStatus =
        pBwrmPageManager->DelPage
            (
                (ANSC_HANDLE)pBwrmPageManager,
                root_path,
                page_path
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BwrmEcoRamIsPageCachedForced
            (
                ANSC_HANDLE                 hThisObject,
                char*                       page_path
            );

    description:

        This function is called to check if the speicified page
        is forced to be cached.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       page_path
                Specifies file name of the page file to be queried.

    return:     status of operation.

**********************************************************************/

BOOL
BwrmEcoRamIsPageCachedForced
    (
        ANSC_HANDLE                 hThisObject,
        char*                       page_path
    )
{
    PBWRM_ENV_CONTROLLER_OBJECT     pMyObject        = (PBWRM_ENV_CONTROLLER_OBJECT  )hThisObject;
    PSINGLE_LINK_ENTRY              pSLinkEntry      = (PSINGLE_LINK_ENTRY         )NULL;
    PBWRM_ENV_CACHE_FORCED_PAGE     pCacheForcedPage = (PBWRM_ENV_CACHE_FORCED_PAGE)NULL;
    BOOL                            bForced          = FALSE;
    ULONG                           ulLen            = AnscSizeOfString(page_path);

    AnscAcquireLock(&pMyObject->AccessLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->ForcedCachedPages);

    while ( pSLinkEntry )
    {
        pCacheForcedPage = ACCESS_BWRM_ENV_CACHE_FORCED_PAGE(pSLinkEntry);
        pSLinkEntry      = AnscQueueGetNextEntry(pSLinkEntry);

        if ( ulLen == AnscSizeOfString(pCacheForcedPage->PagePath) &&
             AnscEqualString2(pCacheForcedPage->PagePath, page_path, ulLen, FALSE) )
        {
            bForced = TRUE;
            break;
        }
    }

    AnscReleaseLock(&pMyObject->AccessLock);

    return  bForced;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmEcoRamForcePageCache
            (
                ANSC_HANDLE                 hThisObject,
                char*                       page_path,
                BOOL                        bCacheForced
            );

    description:

        This function is called to set the specified page to be
        cached/uncached.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       page_path
                Specifies file name of the page file to be queried.

                BOOL                        bCacheForced

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BwrmEcoRamForcePageCache
    (
        ANSC_HANDLE                 hThisObject,
        char*                       page_path,
        BOOL                        bCachedForced
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PBWRM_ENV_CONTROLLER_OBJECT     pMyObject        = (PBWRM_ENV_CONTROLLER_OBJECT  )hThisObject;
    PSINGLE_LINK_ENTRY              pSLinkEntry      = (PSINGLE_LINK_ENTRY         )NULL;
    PBWRM_ENV_CACHE_FORCED_PAGE     pCacheForcedPage = (PBWRM_ENV_CACHE_FORCED_PAGE)NULL;
    BOOL                            bFound           = FALSE;
    ULONG                           ulLen            = AnscSizeOfString(page_path);

    AnscAcquireLock(&pMyObject->AccessLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->ForcedCachedPages);

    while ( pSLinkEntry )
    {
        pCacheForcedPage = ACCESS_BWRM_ENV_CACHE_FORCED_PAGE(pSLinkEntry);
        pSLinkEntry      = AnscQueueGetNextEntry(pSLinkEntry);

        if ( ulLen == AnscSizeOfString(pCacheForcedPage->PagePath) &&
             AnscEqualString2(pCacheForcedPage->PagePath, page_path, ulLen, FALSE) )
        {
            bFound = TRUE;
            break;
        }
    }

    if ( bFound && !bCachedForced )
    {
        /* remove it from the list */
        AnscQueuePopEntryByLink(&pMyObject->ForcedCachedPages, &pCacheForcedPage->Linkage);
    }
    else if ( bCachedForced && !bFound )
    {
        pCacheForcedPage = (PBWRM_ENV_CACHE_FORCED_PAGE)AnscAllocateMemory(sizeof(BWRM_ENV_CACHE_FORCED_PAGE));

        if ( pCacheForcedPage )
        {
            pCacheForcedPage->PagePath = AnscCloneString(page_path);

            if ( pCacheForcedPage->PagePath )
            {
                AnscQueuePushEntry(&pMyObject->ForcedCachedPages, &pCacheForcedPage->Linkage);
            }
            else
            {
                BwrmEnvRemoveCacheForcedPage(pCacheForcedPage);
            }
        }
    }

    AnscReleaseLock(&pMyObject->AccessLock);

    return  returnStatus;
}


