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

    module:	bwrm_cpo_base.c

        For Broadway Web Resource Manager Implementation (BWRM),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Bwrm Cooked Page Object.

        *   BwrmCpoCreate
        *   BwrmCpoRemove
        *   BwrmCpoEnrollObjects
        *   BwrmCpoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/02/03    initial revision.

**********************************************************************/


#include "bwrm_cpo_global.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        BwrmCpoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Bwrm Cooked Page Object and
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
BwrmCpoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PBWRM_COOKED_PAGE_OBJECT        pMyObject    = NULL;
    errno_t   rc   = -1;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PBWRM_COOKED_PAGE_OBJECT)AnscAllocateMemory(sizeof(BWRM_COOKED_PAGE_OBJECT));

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
    rc = STRCPY_S_NOCLOBBER(pBaseObject->Name, sizeof(pBaseObject->Name), BWRM_COOKED_PAGE_NAME);
    ERR_CHK(rc);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = BWRM_COOKED_PAGE_OID;
    pBaseObject->Create            = BwrmCpoCreate;
    pBaseObject->Remove            = BwrmCpoRemove;
    pBaseObject->EnrollObjects     = BwrmCpoEnrollObjects;
    pBaseObject->Initialize        = BwrmCpoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmCpoRemove
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
BwrmCpoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWRM_COOKED_PAGE_OBJECT        pMyObject    = (PBWRM_COOKED_PAGE_OBJECT)hThisObject;

    AnscFreeLock(&pMyObject->AccessLock);

    pMyObject->Reset((ANSC_HANDLE)pMyObject);

    AnscCoRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmCpoEnrollObjects
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
BwrmCpoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWRM_COOKED_PAGE_OBJECT        pMyObject    = (PBWRM_COOKED_PAGE_OBJECT)hThisObject;

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwrmCpoInitialize
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
BwrmCpoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWRM_COOKED_PAGE_OBJECT        pMyObject    = (PBWRM_COOKED_PAGE_OBJECT)hThisObject;

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
    pMyObject->Oid              = BWRM_COOKED_PAGE_OID;
    pMyObject->Create           = BwrmCpoCreate;
    pMyObject->Remove           = BwrmCpoRemove;
    pMyObject->EnrollObjects    = BwrmCpoEnrollObjects;
    pMyObject->Initialize       = BwrmCpoInitialize;

    pMyObject->RootPath         = NULL;
    pMyObject->PagePath         = NULL;
    pMyObject->hOwnerReserved   = NULL;
    pMyObject->hPageContext     = NULL;
    pMyObject->Timestamp        = AnscGetTickInSeconds();
    pMyObject->RefCount         = 0;
    AnscInitializeLock(&pMyObject->AccessLock);
    pMyObject->GetRootPath      = BwrmCpoGetRootPath;
    pMyObject->SetRootPath      = BwrmCpoSetRootPath;
    pMyObject->GetPagePath      = BwrmCpoGetPagePath;
    pMyObject->SetPagePath      = BwrmCpoSetPagePath;
    pMyObject->GetOwnerReserved = BwrmCpoGetOwnerReserved;
    pMyObject->SetOwnerReserved = BwrmCpoSetOwnerReserved;
    pMyObject->GetPageContext   = BwrmCpoGetPageContext;
    pMyObject->SetPageContext   = BwrmCpoSetPageContext;
    pMyObject->GetTimestamp     = BwrmCpoGetTimestamp;
    pMyObject->SetTimestamp     = BwrmCpoSetTimestamp;

    pMyObject->IncRefCount      = BwrmCpoIncRefCount;
    pMyObject->DecRefCount      = BwrmCpoDecRefCount;
    pMyObject->GetRefCount      = BwrmCpoGetRefCount;

    pMyObject->Reset            = BwrmCpoReset;

    pMyObject->MatchPath        = BwrmCpoMatchPath;

    return  ANSC_STATUS_SUCCESS;
}
