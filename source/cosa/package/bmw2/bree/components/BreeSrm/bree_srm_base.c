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

    module:	bree_srm_base.c

        For BREE Site Resource Manager Object,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic component object functions
        of the BREE Site Resource Manager Object.


    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        01/17/04    initial revision.

**********************************************************************/


#include "bree_srm_global.h"
#include "safec_lib_common.h"

PBREE_SRM_OBJECT                    g_pBreeSrmo     = NULL;

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        BreeSrmoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Bree Site Resource Manager
        Object and initializes the member variables and functions.

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
BreeSrmoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PBREE_SRM_OBJECT                pMyObject    = NULL;
    errno_t  rc  = -1;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PBREE_SRM_OBJECT)AnscAllocateMemory(sizeof(BREE_SRM_OBJECT));

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
    rc = STRCPY_S_NOCLOBBER(pBaseObject->Name, sizeof(pBaseObject->Name), BREE_SRMO_COMPONENT_NAME);
    ERR_CHK(rc);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = BREE_SRMO_COMPONENT_OID;
    pBaseObject->Create            = BreeSrmoCreate;
    pBaseObject->Remove            = BreeSrmoRemove;
    pBaseObject->EnrollObjects     = BreeSrmoEnrollObjects;
    pBaseObject->Initialize        = BreeSrmoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BreeSrmoRemove
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
BreeSrmoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBREE_SRM_OBJECT                pMyObject    = (PBREE_SRM_OBJECT)hThisObject;
    PBREE_SRM_RES_ITEM              pRes;
    ULONG                           i;
    PSINGLE_LINK_ENTRY              pEntry;

    for (i = 0; i < BREE_SRM_RES_TABLE_SIZE; i ++)
    {
        while (TRUE)
        {
            pEntry = AnscSListPopEntry(&pMyObject->ResList[i]);

            if (!pEntry)
            {
                break;
            }

            pRes = ACCESS_BREE_SRM_RES_ITEM_OBJECT(pEntry);

            AnscFreeMemory(pRes);
        }
    }

    AnscCoRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BreeSrmoEnrollObjects
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
BreeSrmoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBREE_SRM_OBJECT                pMyObject    = (PBREE_SRM_OBJECT)hThisObject;

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BreeSrmoInitialize
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
BreeSrmoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBREE_SRM_OBJECT                pMyObject    = (PBREE_SRM_OBJECT)hThisObject;
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
    pMyObject->Oid                          = BREE_SRMO_COMPONENT_OID;
    pMyObject->Create                       = BreeSrmoCreate;
    pMyObject->Remove                       = BreeSrmoRemove;
    pMyObject->EnrollObjects                = BreeSrmoEnrollObjects;
    pMyObject->Initialize                   = BreeSrmoInitialize;

    pMyObject->RegisterCookedResource       = BreeSrmoRegisterCookedResource;
    pMyObject->UnregisterCookedResource     = BreeSrmoUnregisterCookedResource;
    pMyObject->MapCookedResource            = BreeSrmoMapCookedResource;
    pMyObject->LoadCookedResources          = BreeSrmoLoadCookedResources;

    for (i = 0; i < BREE_SRM_RES_TABLE_SIZE; i ++)
    {
        AnscSListInitializeHeader(&pMyObject->ResList[i]);
    }

    pMyObject->LoadCookedResources((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}

