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

    module:	bree_spo_base.c

        For BSP Run-time Execution Engine (BREE) object,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the BREE Object.

        *   BreeSpoCreate
        *   BreeSpoRemove
        *   BreeSpoEnrollObjects
        *   BreeSpoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        07/22/03    initial revision.

**********************************************************************/


#include "bree_spo_global.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        BreeSpoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Ansc Daemon Engine Tcp Object and
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
BreeSpoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PBREE_SPO_OBJECT                pMyObject    = NULL;
    errno_t   rc  = -1;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PBREE_SPO_OBJECT)AnscAllocateMemory(sizeof(BREE_SPO_OBJECT));

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
    rc = STRCPY_S_NOCLOBBER(pBaseObject->Name, sizeof(pBaseObject->Name), BREE_SPO_COMPONENT_NAME);
    ERR_CHK(rc);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = BREE_SPO_COMPONENT_OID;
    pBaseObject->Create            = BreeSpoCreate;
    pBaseObject->Remove            = BreeSpoRemove;
    pBaseObject->EnrollObjects     = BreeSpoEnrollObjects;
    pBaseObject->Initialize        = BreeSpoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BreeSpoRemove
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
BreeSpoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBREE_SPO_OBJECT                pMyObject    = (PBREE_SPO_OBJECT)hThisObject;
    PSINGLE_LINK_ENTRY              pSListEntry;
    PBREE_SPO_RES_FILTER            pFilter;
    PBSPENG_SOA_INTERFACE           pBreeSoaIf   = (PBSPENG_SOA_INTERFACE)pMyObject->hBreeSoaIf;

    if (pMyObject->hBeepEngineDesc)
    {
        AnscFreeMemory(pMyObject->hBeepEngineDesc);
    }

    if (pMyObject->hStaticResEngineDesc)
    {
        AnscFreeMemory(pMyObject->hStaticResEngineDesc);
    }

    /* clean up filter list */
    while (TRUE)
    {
        pSListEntry = AnscSListPopEntry(&pMyObject->ResFilterList);

        if (!pSListEntry)
        {
            break;
        }

        pFilter = ACCESS_BREE_SPO_RES_FILTER(pSListEntry);

        if (pFilter->pDFName)
        {
            AnscFreeMemory(pFilter->pDFName);
        }

        AnscFreeMemory(pFilter);
    }

    /* clean up filter exemption list */
    while (TRUE)
    {
        pSListEntry = AnscSListPopEntry(&pMyObject->ResFilterListExemption);

        if (!pSListEntry)
        {
            break;
        }

        pFilter = ACCESS_BREE_SPO_RES_FILTER(pSListEntry);

        if (pFilter->pDFName)
        {
            AnscFreeMemory(pFilter->pDFName);
        }

        AnscFreeMemory(pFilter);
    }

    if (pMyObject->SupervisorPage)
    {
        AnscFreeMemory(pMyObject->SupervisorPage);
    }

#ifdef   _BREE_SPO_USE_SRMO

    if (g_pBreeSrmo)
    {
        PBREE_SRM_OBJECT            pBreeSrmo = g_pBreeSrmo;

        g_pBreeSrmo = NULL;

        pBreeSrmo->Remove((ANSC_HANDLE)pBreeSrmo);
    }

#ifdef   _ANSC_FILE_ZLIB_
    if ( g_pCryptoObj )
    {
        g_pCryptoObj->Remove((ANSC_HANDLE)g_pCryptoObj);
    }
#endif

#endif

    if (pBreeSoaIf)
    {
        AnscFreeMemory(pBreeSoaIf);
    }

    AnscCoRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BreeSpoEnrollObjects
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
BreeSpoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBREE_SPO_OBJECT                pMyObject    = (PBREE_SPO_OBJECT)hThisObject;
    PBSPENG_SOA_INTERFACE           pBreeSoaIf   = (PBSPENG_SOA_INTERFACE)pMyObject->hBreeSoaIf;

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);
    errno_t  rc  = -1;

#ifdef   _BREE_SPO_USE_SRMO
    if (!g_pBreeSrmo)
    {
        g_pBreeSrmo = 
            (PBREE_SRM_OBJECT)BreeCreateSrmo
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );
    }

#ifdef   _ANSC_FILE_ZLIB_
    if ( !g_pCryptoObj )
    {
        g_pCryptoObj  = (PANSC_CRYPTO_OBJECT)AnscCreateCrypto((ANSC_HANDLE)pMyObject);
    }
#endif

#endif

    if ( !pBreeSoaIf )
    {
        pBreeSoaIf = (PBSPENG_SOA_INTERFACE)AnscAllocateMemory(sizeof(BSPENG_SOA_INTERFACE));

        if ( !pBreeSoaIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hBreeSoaIf       = (ANSC_HANDLE)pBreeSoaIf;
        }

        rc = STRCPY_S_NOCLOBBER(pBreeSoaIf->Name, sizeof(pBreeSoaIf->Name), BREE_SOA_INTERFACE_NAME);
        ERR_CHK(rc);

        pBreeSoaIf->InterfaceId         = BREE_SOA_INTERFACE_ID;
        pBreeSoaIf->hOwnerContext       = (ANSC_HANDLE)NULL;
        pBreeSoaIf->Size                = sizeof(BSPENG_SOA_INTERFACE);

        pBreeSoaIf->GetSlapObject       = BreeSpoSoaGetSlapObject;
        pBreeSoaIf->InvokeObject        = BreeSpoSoaInvokeObject;
        pBreeSoaIf->IsBuiltInObj        = BreeSpoSoaIsBuiltInObject;
        pBreeSoaIf->WriteAString        = BreeSpoSoaWriteAString;
        pBreeSoaIf->WriteBString        = BreeSpoSoaWriteBString;
        pBreeSoaIf->GetCookedPage       = BreeSpoSoaGetCookedPage;
        pBreeSoaIf->RetCookedPage       = BreeSpoSoaRetCookedPage;
        pBreeSoaIf->GetCookedPageData   = BreeSpoSoaGetCookedPageData;
        pBreeSoaIf->IsInterrupted       = BreeSpoSoaIsInterrupted;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BreeSpoInitialize
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
BreeSpoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBREE_SPO_OBJECT                pMyObject    = (PBREE_SPO_OBJECT)hThisObject;

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
    pMyObject->Oid                          = BREE_SPO_COMPONENT_OID;
    pMyObject->Create                       = BreeSpoCreate;
    pMyObject->Remove                       = BreeSpoRemove;
    pMyObject->EnrollObjects                = BreeSpoEnrollObjects;
    pMyObject->Initialize                   = BreeSpoInitialize;

    pMyObject->Engage                       = BreeSpoEngage;
    pMyObject->Cancel                       = BreeSpoCancel;
    pMyObject->SetBeepSerIf                 = BreeSpoSetBeepSerIf;
    pMyObject->SetBeepCspIf                 = BreeSpoSetBeepCspIf;
    
    pMyObject->SetBeepEngineDesc            = BreeSpoSetBeepEngineDesc;
    pMyObject->GetBeepEngineDesc            = BreeSpoGetBeepEngineDesc;
    pMyObject->CreateBeepEngineDesc         = BreeSpoCreateBeepEngineDesc;
    pMyObject->PreparePage                  = BreeSpoPreparePage;
    pMyObject->ExecutePage                  = BreeSpoExecutePage;

    pMyObject->CreateStaticResEngineDesc    = BreeSpoCreateStaticResEngineDesc;
    pMyObject->SetStaticResEngineDesc       = BreeSpoSetStaticResEngineDesc;
    pMyObject->GetStaticResEngineDesc       = BreeSpoGetStaticResEngineDesc;
    pMyObject->PrepareStaticRes             = BreeSpoPrepareStaticRes;
    pMyObject->ExecuteStatisRes             = BreeSpoExecuteStaticRes;

    pMyObject->QualifyPage                  = BreeSpoQualifyPage;

    pMyObject->ConfigResFilter              = BreeSpoConfigResFilter;
    pMyObject->RegResFilterDir              = BreeSpoRegResFilterDir;
    pMyObject->RegResFilterDirExemption     = BreeSpoRegResFilterDirExemption;
    pMyObject->RegResFilterFile             = BreeSpoRegResFilterFile;
    pMyObject->RegResFilterFileExemption    = BreeSpoRegResFilterFileExemption;
    pMyObject->GetFormalPagePath            = BreeSpoGetFormalPagePath;
    pMyObject->IsPagePathInFilterList       = BreeSpoIsPagePathInFilterList;
    pMyObject->PrepareDFName                = BreeSpoPrepareDFName;

    pMyObject->EnforceCache                 = BreeSpoEnforceCache;
    pMyObject->CreateBreeSoaIf              = BreeSpoCreateBreeSoaIf;

    pMyObject->ResFilterMode                = BREE_SPO_RES_FILTER_Off;
    pMyObject->SupervisorPage               = NULL;

    AnscSListInitializeHeader(&pMyObject->ResFilterList);
    AnscSListInitializeHeader(&pMyObject->ResFilterListExemption);

    /* create BEEP engine descriptor for BREE */
    pMyObject->CreateBeepEngineDesc(hThisObject);

    /* create BEEP engine descriptor for BREE */
    pMyObject->CreateStaticResEngineDesc(hThisObject);

    return  ANSC_STATUS_SUCCESS;
}
