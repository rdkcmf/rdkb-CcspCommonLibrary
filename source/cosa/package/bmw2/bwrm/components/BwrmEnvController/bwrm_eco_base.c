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

    module:	bwrm_eco_base.c

        For Broadway Web Resource Manager Implementation (BWRM),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Bwrm Env Controller Object.

        *   BwrmEcoCreate
        *   BwrmEcoRemove
        *   BwrmEcoEnrollObjects
        *   BwrmEcoInitialize

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
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        BwrmEcoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Bwrm Env Controller Object and
        initializes the member variables and functions.

    argument:   ANSC_HANDLE                 hContainerContext
                This handle is used by the container object to interact
                with the outside world. It could be the real container
                or an target object.

                ANSC_HANDLE                 hOwnerContext
                This handle is passed in by the owner of this object.

                ANSC_HANDLE                 hAnscReserved
                This handle is passed in by the owner of this object.

    return:     newly created container object.

**********************************************************************/

ANSC_HANDLE
BwrmEcoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PBWRM_ENV_CONTROLLER_OBJECT     pMyObject    = NULL;
    errno_t   rc  = -1;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PBWRM_ENV_CONTROLLER_OBJECT)AnscAllocateMemory(sizeof(BWRM_ENV_CONTROLLER_OBJECT));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pBaseObject = (PANSC_COMPONENT_OBJECT)pMyObject;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    rc = STRCPY_S_NOCLOBBER(pBaseObject->Name, sizeof(pBaseObject->Name), BWRM_ENV_CONTROLLER_NAME);
    ERR_CHK(rc);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = BWRM_ENV_CONTROLLER_OID;
    pBaseObject->Create            = BwrmEcoCreate;
    pBaseObject->Remove            = BwrmEcoRemove;
    pBaseObject->EnrollObjects     = BwrmEcoEnrollObjects;
    pBaseObject->Initialize        = BwrmEcoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmEcoRemove
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function destroys the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BwrmEcoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWRM_ENV_CONTROLLER_OBJECT     pMyObject        = (PBWRM_ENV_CONTROLLER_OBJECT)hThisObject;
    PBWRM_RAM_INTERFACE             pBwrmRamIf       = (PBWRM_RAM_INTERFACE        )pMyObject->hBwrmRamIf;
    PBWRM_FILE_MANAGER_OBJECT       pBwrmFileManager = (PBWRM_FILE_MANAGER_OBJECT  )pMyObject->hBwrmFileManager;
    PBWRM_PAGE_MANAGER_OBJECT       pBwrmPageManager = (PBWRM_PAGE_MANAGER_OBJECT  )pMyObject->hBwrmPageManager;
    PSINGLE_LINK_ENTRY              pSLinkEntry      = (PSINGLE_LINK_ENTRY         )NULL;
    PBWRM_ENV_CACHE_FORCED_PAGE     pCacheForcedPage = (PBWRM_ENV_CACHE_FORCED_PAGE)NULL;

    pMyObject->Cancel((ANSC_HANDLE)pMyObject);
    pMyObject->Reset ((ANSC_HANDLE)pMyObject);

    if ( pBwrmRamIf )
    {
        AnscFreeMemory(pBwrmRamIf);

        pMyObject->hBwrmRamIf = (ANSC_HANDLE)NULL;
    }

    if ( pBwrmFileManager )
    {
        pBwrmFileManager->Remove((ANSC_HANDLE)pBwrmFileManager);

        pMyObject->hBwrmFileManager = (ANSC_HANDLE)NULL;
    }

    if ( pBwrmPageManager )
    {
        pBwrmPageManager->Remove((ANSC_HANDLE)pBwrmPageManager);

        pMyObject->hBwrmPageManager = (ANSC_HANDLE)NULL;
    }

    pSLinkEntry = AnscQueuePopEntry(&pMyObject->ForcedCachedPages);

    while ( pSLinkEntry )
    {
        pCacheForcedPage = ACCESS_BWRM_ENV_CACHE_FORCED_PAGE(pSLinkEntry);
        pSLinkEntry    = AnscQueuePopEntry(&pMyObject->ForcedCachedPages);

        BwrmEnvRemoveCacheForcedPage(pCacheForcedPage);
    }

    AnscReleaseLock(&pMyObject->AccessLock);

    AnscFreeLock(&pMyObject->AccessLock);

    AnscCoRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmEcoEnrollObjects
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function enrolls all the objects required by this object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BwrmEcoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWRM_ENV_CONTROLLER_OBJECT     pMyObject        = (PBWRM_ENV_CONTROLLER_OBJECT)hThisObject;
    PBWRM_RAM_INTERFACE             pBwrmRamIf       = (PBWRM_RAM_INTERFACE        )pMyObject->hBwrmRamIf;
    PBWRM_FILE_MANAGER_OBJECT       pBwrmFileManager = (PBWRM_FILE_MANAGER_OBJECT  )pMyObject->hBwrmFileManager;
    PBWRM_PAGE_MANAGER_OBJECT       pBwrmPageManager = (PBWRM_PAGE_MANAGER_OBJECT  )pMyObject->hBwrmPageManager;
    errno_t   rc = -1;

    if ( !pBwrmRamIf )
    {
        pBwrmRamIf = (PBWRM_RAM_INTERFACE)AnscAllocateMemory(sizeof(BWRM_RAM_INTERFACE));

        if ( !pBwrmRamIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hBwrmRamIf = (ANSC_HANDLE)pBwrmRamIf;
        }

        rc = STRCPY_S_NOCLOBBER(pBwrmRamIf->Name, sizeof(pBwrmRamIf->Name), BWRM_RAM_INTERFACE_NAME);
        ERR_CHK(rc);

        pBwrmRamIf->InterfaceId     = BWRM_RAM_INTERFACE_ID;
        pBwrmRamIf->hOwnerContext   = (ANSC_HANDLE)pMyObject;
        pBwrmRamIf->Size            = sizeof(BWRM_RAM_INTERFACE);

        pBwrmRamIf->GetCacheNumber      = BwrmEcoRamGetCacheNumber;
        pBwrmRamIf->SetCacheNumber      = BwrmEcoRamSetCacheNumber;
        pBwrmRamIf->GetCacheSize        = BwrmEcoRamGetCacheSize;
        pBwrmRamIf->SetCacheSize        = BwrmEcoRamSetCacheSize;
        pBwrmRamIf->GetCacheTimeout     = BwrmEcoRamGetCacheTimeout;
        pBwrmRamIf->SetCacheTimeout     = BwrmEcoRamSetCacheTimeout;
        pBwrmRamIf->ControlCache        = BwrmEcoRamControlCache;

        pBwrmRamIf->GetRawPage          = BwrmEcoRamGetRawPage;
        pBwrmRamIf->GetCookedPage       = BwrmEcoRamGetCookedPage;
        pBwrmRamIf->AddCookedPage       = BwrmEcoRamAddCookedPage;
        pBwrmRamIf->DelCookedPage       = BwrmEcoRamDelCookedPage;

        pBwrmRamIf->ForcePageCache      = BwrmEcoRamForcePageCache;
        pBwrmRamIf->IsPageCacheForced   = BwrmEcoRamIsPageCachedForced;
    }

    if ( !pBwrmFileManager )
    {
        pBwrmFileManager =
            (PBWRM_FILE_MANAGER_OBJECT)BwrmCreateFileManager
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pBwrmFileManager )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hBwrmFileManager = (ANSC_HANDLE)pBwrmFileManager;
        }
    }

    if ( !pBwrmPageManager )
    {
        pBwrmPageManager =
            (PBWRM_PAGE_MANAGER_OBJECT)BwrmCreatePageManager
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pBwrmPageManager )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hBwrmPageManager = (ANSC_HANDLE)pBwrmPageManager;
        }
    }

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmEcoInitialize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function first calls the initialization member function
        of the base class object to set the common member fields
        inherited from the base class. It then initializes the member
        fields that are specific to this object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BwrmEcoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWRM_ENV_CONTROLLER_OBJECT     pMyObject    = (PBWRM_ENV_CONTROLLER_OBJECT)hThisObject;

    /*
     * Until you have to simulate C++ object-oriented programming style with standard C, you don't
     * appreciate all the nice little things come with C++ language and all the dirty works that
     * have been done by the C++ compilers. Member initialization is one of these things. While in
     * C++ you don't have to initialize all the member fields inherited from the base class since
     * the compiler will do it for you, such is not the case with C.
     */
    AnscCoInitialize((ANSC_HANDLE)pMyObject);

    /*
     * Although we have initialized some of the member fields in the "create" member function, we
     * repeat the work here for completeness. While this simulation approach is pretty stupid from
     * a C++/Java programmer perspective, it's the best we can get for universal embedded network
     * programming. Before we develop our own operating system (don't expect that to happen any
     * time soon), this is the way things gonna be.
     */
    pMyObject->Oid                = BWRM_ENV_CONTROLLER_OID;
    pMyObject->Create             = BwrmEcoCreate;
    pMyObject->Remove             = BwrmEcoRemove;
    pMyObject->EnrollObjects      = BwrmEcoEnrollObjects;
    pMyObject->Initialize         = BwrmEcoInitialize;

    pMyObject->bActive            = FALSE;

    pMyObject->GetBwrmRamIf       = BwrmEcoGetBwrmRamIf;
    pMyObject->GetBwrmPageManager = BwrmEcoGetBwrmPageManager;
    pMyObject->GetBwrmFileManager = BwrmEcoGetBwrmFileManager;

    pMyObject->GetProperty        = BwrmEcoGetProperty;
    pMyObject->SetProperty        = BwrmEcoSetProperty;
    pMyObject->ResetProperty      = BwrmEcoResetProperty;
    pMyObject->Reset              = BwrmEcoReset;

    pMyObject->Engage             = BwrmEcoEngage;
    pMyObject->Cancel             = BwrmEcoCancel;

    pMyObject->RamGetCacheNumber  = BwrmEcoRamGetCacheNumber;
    pMyObject->RamSetCacheNumber  = BwrmEcoRamSetCacheNumber;
    pMyObject->RamGetCacheSize    = BwrmEcoRamGetCacheSize;
    pMyObject->RamSetCacheSize    = BwrmEcoRamSetCacheSize;
    pMyObject->RamGetCacheTimeout = BwrmEcoRamGetCacheTimeout;
    pMyObject->RamSetCacheTimeout = BwrmEcoRamSetCacheTimeout;
    pMyObject->RamControlCache    = BwrmEcoRamControlCache;

    pMyObject->RamGetRawPage      = BwrmEcoRamGetRawPage;
    pMyObject->RamGetCookedPage   = BwrmEcoRamGetCookedPage;
    pMyObject->RamAddCookedPage   = BwrmEcoRamAddCookedPage;
    pMyObject->RamDelCookedPage   = BwrmEcoRamDelCookedPage;

    pMyObject->IsPageCacheForced  = BwrmEcoRamIsPageCachedForced;
    pMyObject->ForcePageCache     = BwrmEcoRamForcePageCache;

    AnscQueueInitializeHeader(&pMyObject->ForcedCachedPages);
    AnscInitializeLock       (&pMyObject->AccessLock);

    /*
     * We shall initialize the configuration properties to the default values, which may be changed
     * later via the set_property() member function. Note that this call may not guarantee a valid
     * and legtimate configuration.
     */
    pMyObject->ResetProperty((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
