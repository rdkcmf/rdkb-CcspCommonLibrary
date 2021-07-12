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

    module:	bmc2_comeo_base.c

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the exported functions provided by the Bmc2 Com Executor
        Object.

        *   Bmc2ComeoCreate
        *   Bmc2ComeoRemove
        *   Bmc2ComeoEnrollObjects
        *   Bmc2ComeoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        06/28/05    initial revision.

**********************************************************************/


#include "bmc2_comeo_global.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        Bmc2ComeoCreate
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
Bmc2ComeoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PBMC2_COM_EXECUTOR_OBJECT       pMyObject    = NULL;
    errno_t    rc   = -1;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PBMC2_COM_EXECUTOR_OBJECT)AnscAllocateMemory(sizeof(BMC2_COM_EXECUTOR_OBJECT));

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
    rc = STRCPY_S_NOCLOBBER(pBaseObject->Name, sizeof(pBaseObject->Name), BMC2_COM_EXECUTOR_NAME);
    ERR_CHK(rc);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = BMC2_COM_EXECUTOR_OID;
    pBaseObject->Create            = Bmc2ComeoCreate;
    pBaseObject->Remove            = Bmc2ComeoRemove;
    pBaseObject->EnrollObjects     = Bmc2ComeoEnrollObjects;
    pBaseObject->Initialize        = Bmc2ComeoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ComeoRemove
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
Bmc2ComeoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_COM_EXECUTOR_OBJECT       pMyObject    = (PBMC2_COM_EXECUTOR_OBJECT)hThisObject;
    PBSPENG_SOA_INTERFACE           pBmc2SoaIf   = (PBSPENG_SOA_INTERFACE)pMyObject->hBmc2SoaIf;

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

    if (pBmc2SoaIf)
    {
        AnscFreeMemory(pBmc2SoaIf);
    }

    AnscCoRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ComeoEnrollObjects
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
Bmc2ComeoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_COM_EXECUTOR_OBJECT       pMyObject    = (PBMC2_COM_EXECUTOR_OBJECT)hThisObject;
    PBSPENG_SOA_INTERFACE           pBmc2SoaIf   = (PBSPENG_SOA_INTERFACE)pMyObject->hBmc2SoaIf;

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);
    errno_t   rc  = -1;

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

    if ( !pBmc2SoaIf )
    {
        pBmc2SoaIf = (PBSPENG_SOA_INTERFACE)AnscAllocateMemory(sizeof(BSPENG_SOA_INTERFACE));

        if ( !pBmc2SoaIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hBmc2SoaIf       = (ANSC_HANDLE)pBmc2SoaIf;
        }

        rc = STRCPY_S_NOCLOBBER(pBmc2SoaIf->Name, sizeof(pBmc2SoaIf->Name), BMC2_SOA_INTERFACE_NAME);
        ERR_CHK(rc);

        pBmc2SoaIf->InterfaceId         = BMC2_SOA_INTERFACE_ID;
        pBmc2SoaIf->hOwnerContext       = (ANSC_HANDLE)NULL;
        pBmc2SoaIf->Size                = sizeof(BSPENG_SOA_INTERFACE);

        pBmc2SoaIf->GetSlapObject       = Bmc2ComeoSoaGetSlapObject;
        pBmc2SoaIf->InvokeObject        = Bmc2ComeoSoaInvokeObject;
        pBmc2SoaIf->IsBuiltInObj        = Bmc2ComeoSoaIsBuiltInObject;
        pBmc2SoaIf->WriteAString        = Bmc2ComeoSoaWriteAString;
        pBmc2SoaIf->WriteBString        = Bmc2ComeoSoaWriteBString;
        pBmc2SoaIf->GetCookedPage       = Bmc2ComeoSoaGetCookedPage;
        pBmc2SoaIf->RetCookedPage       = Bmc2ComeoSoaRetCookedPage;
        pBmc2SoaIf->GetCookedPageData   = Bmc2ComeoSoaGetCookedPageData;
        pBmc2SoaIf->IsInterrupted       = Bmc2ComeoSoaIsInterrupted;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ComeoInitialize
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
Bmc2ComeoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_COM_EXECUTOR_OBJECT       pMyObject    = (PBMC2_COM_EXECUTOR_OBJECT)hThisObject;

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
    pMyObject->Oid                          = BMC2_COM_EXECUTOR_OID;
    pMyObject->Create                       = Bmc2ComeoCreate;
    pMyObject->Remove                       = Bmc2ComeoRemove;
    pMyObject->EnrollObjects                = Bmc2ComeoEnrollObjects;
    pMyObject->Initialize                   = Bmc2ComeoInitialize;

    pMyObject->GetBmc2EnvController         = Bmc2ComeoGetBmc2EnvController;
    pMyObject->SetBmc2EnvController         = Bmc2ComeoSetBmc2EnvController;

    pMyObject->GetProperty                  = Bmc2ComeoGetProperty;
    pMyObject->SetProperty                  = Bmc2ComeoSetProperty;
    pMyObject->ResetProperty                = Bmc2ComeoResetProperty;
    pMyObject->Reset                        = Bmc2ComeoReset;

    pMyObject->Engage                       = Bmc2ComeoEngage;
    pMyObject->Cancel                       = Bmc2ComeoCancel;
    pMyObject->SetupEnv                     = Bmc2ComeoSetupEnv;
    pMyObject->CloseEnv                     = Bmc2ComeoCloseEnv;

    pMyObject->PreparePage                  = Bmc2ComeoPreparePage;
    pMyObject->ExecutePage                  = Bmc2ComeoExecutePage;

    pMyObject->CreateBmc2SoaIf              = Bmc2ComeoCreateBmc2SoaIf;

    pMyObject->ResetProperty((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
