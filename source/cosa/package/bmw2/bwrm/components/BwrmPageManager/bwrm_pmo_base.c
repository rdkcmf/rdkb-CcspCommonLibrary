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

    module:	bwrm_pmo_base.c

        For Broadway Web Resource Manager Implementation (BWRM),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Bwrm Page Manager Object.

        *   BwrmPmoCreate
        *   BwrmPmoRemove
        *   BwrmPmoEnrollObjects
        *   BwrmPmoInitialize

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
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        BwrmPmoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Bwrm Page Manager Object and
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
BwrmPmoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PBWRM_PAGE_MANAGER_OBJECT       pMyObject    = NULL;
    errno_t   rc = -1;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PBWRM_PAGE_MANAGER_OBJECT)AnscAllocateMemory(sizeof(BWRM_PAGE_MANAGER_OBJECT));

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
    rc = STRCPY_S_NOCLOBBER(pBaseObject->Name, sizeof(pBaseObject->Name), BWRM_PAGE_MANAGER_NAME);
    ERR_CHK(rc);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = BWRM_PAGE_MANAGER_OID;
    pBaseObject->Create            = BwrmPmoCreate;
    pBaseObject->Remove            = BwrmPmoRemove;
    pBaseObject->EnrollObjects     = BwrmPmoEnrollObjects;
    pBaseObject->Initialize        = BwrmPmoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmPmoRemove
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
BwrmPmoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWRM_PAGE_MANAGER_OBJECT       pMyObject         = (PBWRM_PAGE_MANAGER_OBJECT    )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pCacheTimerObject = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hCacheTimerObject;
    PANSC_TDO_CLIENT_OBJECT         pCacheTimerClient = (PANSC_TDO_CLIENT_OBJECT      )pMyObject->hCacheTimerClient;

    pMyObject->Cancel((ANSC_HANDLE)pMyObject);
    pMyObject->Reset ((ANSC_HANDLE)pMyObject);

    if ( pCacheTimerObject )
    {
        pCacheTimerObject->Remove((ANSC_HANDLE)pCacheTimerObject);

        pMyObject->hCacheTimerObject = (ANSC_HANDLE)NULL;
    }

    if ( pCacheTimerClient )
    {
        AnscFreeMemory(pCacheTimerClient);

        pMyObject->hCacheTimerClient = (ANSC_HANDLE)NULL;
    }

    AnscFreeLock(&pMyObject->CpoTableLock);
    AnscCoRemove((ANSC_HANDLE)pMyObject  );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmPmoEnrollObjects
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
BwrmPmoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWRM_PAGE_MANAGER_OBJECT       pMyObject         = (PBWRM_PAGE_MANAGER_OBJECT    )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pCacheTimerObject = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hCacheTimerObject;
    PANSC_TDO_CLIENT_OBJECT         pCacheTimerClient = (PANSC_TDO_CLIENT_OBJECT      )pMyObject->hCacheTimerClient;

    if ( !pCacheTimerObject )
    {
        pCacheTimerObject =
            (PANSC_TIMER_DESCRIPTOR_OBJECT)AnscCreateTimerDescriptor
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pCacheTimerObject )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hCacheTimerObject = (ANSC_HANDLE)pCacheTimerObject;
        }

        pCacheTimerObject->SetTimerType((ANSC_HANDLE)pCacheTimerObject, ANSC_TIMER_TYPE_PERIODIC     );
        pCacheTimerObject->SetInterval ((ANSC_HANDLE)pCacheTimerObject, BWRM_PMO_CACHE_TIMER_INTERVAL);
    }

    if ( !pCacheTimerClient )
    {
        pCacheTimerClient = (PANSC_TDO_CLIENT_OBJECT)AnscAllocateMemory(sizeof(ANSC_TDO_CLIENT_OBJECT));

        if ( !pCacheTimerClient )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hCacheTimerClient = (ANSC_HANDLE)pCacheTimerClient;
        }

        pCacheTimerClient->hClientContext = (ANSC_HANDLE)pMyObject;
        pCacheTimerClient->Invoke         = BwrmPmoCacheTimerInvoke;

        pCacheTimerObject->SetClient((ANSC_HANDLE)pCacheTimerObject, (ANSC_HANDLE)pCacheTimerClient);
    }

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmPmoInitialize
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
BwrmPmoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWRM_PAGE_MANAGER_OBJECT       pMyObject    = (PBWRM_PAGE_MANAGER_OBJECT)hThisObject;
    ULONG                           i            = 0;

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
    pMyObject->Oid                = BWRM_PAGE_MANAGER_OID;
    pMyObject->Create             = BwrmPmoCreate;
    pMyObject->Remove             = BwrmPmoRemove;
    pMyObject->EnrollObjects      = BwrmPmoEnrollObjects;
    pMyObject->Initialize         = BwrmPmoInitialize;

    pMyObject->Timestamp          = AnscGetTickInSeconds();
    pMyObject->bActive            = FALSE;

    pMyObject->GetCacheEnabled    = BwrmPmoGetCacheEnabled;
    pMyObject->SetCacheEnabled    = BwrmPmoSetCacheEnabled;
    pMyObject->GetCacheEntryCount = BwrmPmoGetCacheEntryCount;
    pMyObject->SetCacheEntryCount = BwrmPmoSetCacheEntryCount;
    pMyObject->GetCacheMemorySize = BwrmPmoGetCacheMemorySize;
    pMyObject->SetCacheMemorySize = BwrmPmoSetCacheMemorySize;
    pMyObject->GetCacheTimeout    = BwrmPmoGetCacheTimeout;
    pMyObject->SetCacheTimeout    = BwrmPmoSetCacheTimeout;

    pMyObject->GetProperty        = BwrmPmoGetProperty;
    pMyObject->SetProperty        = BwrmPmoSetProperty;
    pMyObject->ResetProperty      = BwrmPmoResetProperty;
    pMyObject->Reset              = BwrmPmoReset;

    pMyObject->Engage             = BwrmPmoEngage;
    pMyObject->Cancel             = BwrmPmoCancel;
    pMyObject->CacheTimerInvoke   = BwrmPmoCacheTimerInvoke;

    pMyObject->GetPageCount       = BwrmPmoGetPageCount;
    pMyObject->GetOldestPage      = BwrmPmoGetOldestPage;
    pMyObject->GetPage            = BwrmPmoGetPage;
    pMyObject->AddPage            = BwrmPmoAddPage;
    pMyObject->DelPage            = BwrmPmoDelPage;
    pMyObject->DelAllPages        = BwrmPmoDelAllPages;

    for ( i = 0; i < BWRM_PMO_CPO_TABLE_SIZE; i++ )
    {
        AnscSListInitializeHeader(&pMyObject->CpoTable[i]);
    }
    AnscInitializeLock(&pMyObject->CpoTableLock);

    /*
     * We shall initialize the configuration properties to the default values, which may be changed
     * later via the set_property() member function. Note that this call may not guarantee a valid
     * and legtimate configuration.
     */
    pMyObject->ResetProperty((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
