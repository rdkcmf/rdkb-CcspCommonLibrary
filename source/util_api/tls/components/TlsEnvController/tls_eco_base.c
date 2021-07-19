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

    module:	tls_eco_base.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the TLS Env Controller Object.

        *   TlsEcoCreate
        *   TlsEcoRemove
        *   TlsEcoEnrollObjects
        *   TlsEcoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/18/03    initial revision.

**********************************************************************/


#include "tls_eco_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        TlsEcoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the TLS Env Controller Object and
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
TlsEcoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PTLS_ENV_CONTROLLER_OBJECT      pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PTLS_ENV_CONTROLLER_OBJECT)AnscAllocateMemory(sizeof(TLS_ENV_CONTROLLER_OBJECT));

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
    /* AnscCopyString(pBaseObject->Name, TLS_ENV_CONTROLLER_NAME); */

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = TLS_ENV_CONTROLLER_OID;
    pBaseObject->Create            = TlsEcoCreate;
    pBaseObject->Remove            = TlsEcoRemove;
    pBaseObject->EnrollObjects     = TlsEcoEnrollObjects;
    pBaseObject->Initialize        = TlsEcoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsEcoRemove
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
TlsEcoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_ENV_CONTROLLER_OBJECT      pMyObject          = (PTLS_ENV_CONTROLLER_OBJECT )hThisObject;
    PTLS_SESSION_MANAGER_OBJECT     pTlsSessionManager = (PTLS_SESSION_MANAGER_OBJECT)pMyObject->hTlsSessionManager;
    PTLS_CRYPTO_PROVIDER_OBJECT     pTlsCryptoProvider = (PTLS_CRYPTO_PROVIDER_OBJECT)pMyObject->hTlsCryptoProvider;
    PTLS_SCS_INTERFACE              pTlsScsIf          = (PTLS_SCS_INTERFACE         )pMyObject->hTlsScsIf;
    PTLS_MEC_INTERFACE              pTlsMecIf          = (PTLS_MEC_INTERFACE         )pMyObject->hTlsMecIf;

    pMyObject->Cancel((ANSC_HANDLE)pMyObject);
    pMyObject->Reset ((ANSC_HANDLE)pMyObject);

    if ( pTlsSessionManager )
    {
        pTlsSessionManager->Remove((ANSC_HANDLE)pTlsSessionManager);

        pMyObject->hTlsSessionManager = (ANSC_HANDLE)NULL;
    }

    if ( pTlsCryptoProvider )
    {
        pTlsCryptoProvider->Remove((ANSC_HANDLE)pTlsCryptoProvider);

        pMyObject->hTlsCryptoProvider = (ANSC_HANDLE)NULL;
    }

    if ( pTlsScsIf )
    {
        AnscFreeMemory(pTlsScsIf);

        pMyObject->hTlsScsIf = (ANSC_HANDLE)NULL;
    }

    if ( pTlsMecIf )
    {
        AnscFreeMemory(pTlsMecIf);

        pMyObject->hTlsMecIf = (ANSC_HANDLE)NULL;
    }

    AnscCoRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsEcoEnrollObjects
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
TlsEcoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_ENV_CONTROLLER_OBJECT      pMyObject          = (PTLS_ENV_CONTROLLER_OBJECT )hThisObject;
    PTLS_SESSION_MANAGER_OBJECT     pTlsSessionManager = (PTLS_SESSION_MANAGER_OBJECT)pMyObject->hTlsSessionManager;
    PTLS_CRYPTO_PROVIDER_OBJECT     pTlsCryptoProvider = (PTLS_CRYPTO_PROVIDER_OBJECT)pMyObject->hTlsCryptoProvider;
    PTLS_SCS_INTERFACE              pTlsScsIf          = (PTLS_SCS_INTERFACE         )pMyObject->hTlsScsIf;
    PTLS_MEC_INTERFACE              pTlsMecIf          = (PTLS_MEC_INTERFACE         )pMyObject->hTlsMecIf;
    errno_t                         rc                 = -1;

    if ( !pTlsSessionManager )
    {
        pTlsSessionManager =
            (PTLS_SESSION_MANAGER_OBJECT)TlsCreateSessionManager
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pTlsSessionManager )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hTlsSessionManager = (ANSC_HANDLE)pTlsSessionManager;
        }
    }

    if ( !pTlsCryptoProvider )
    {
        pTlsCryptoProvider =
            (PTLS_CRYPTO_PROVIDER_OBJECT)TlsCreateCryptoProvider
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pTlsCryptoProvider )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hTlsCryptoProvider = (ANSC_HANDLE)pTlsCryptoProvider;
        }
    }

    if ( !pTlsScsIf )
    {
        pTlsScsIf = (PTLS_SCS_INTERFACE)AnscAllocateMemory(sizeof(TLS_SCS_INTERFACE));

        if ( !pTlsScsIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hTlsScsIf = (ANSC_HANDLE)pTlsScsIf;
        }

        rc = strcpy_s(pTlsScsIf->Name, sizeof(pTlsScsIf->Name), TLS_SCS_INTERFACE_NAME);
        ERR_CHK(rc);

        pTlsScsIf->InterfaceId      = TLS_SCS_INTERFACE_ID;
        pTlsScsIf->hOwnerContext    = (ANSC_HANDLE)pMyObject;
        pTlsScsIf->Size             = sizeof(TLS_SCS_INTERFACE);

        pTlsScsIf->ConfigModule     = TlsEcoScsConfigModule;
        pTlsScsIf->CreateConnection = TlsEcoScsCreateConnection;
        pTlsScsIf->ConfigConnection = TlsEcoScsConfigConnection;
        pTlsScsIf->RemoveConnection = TlsEcoScsRemoveConnection;

        pTlsScsIf->StartConnection  = TlsEcoScsStartConnection;
        pTlsScsIf->CloseConnection  = TlsEcoScsCloseConnection;
        pTlsScsIf->QueryConnection  = TlsEcoScsQueryConnection;

        pTlsScsIf->RecvTlsMessage   = TlsEcoScsRecvTlsMessage;
        pTlsScsIf->SendAppMessage   = TlsEcoScsSendAppMessage;
    }

    if ( !pTlsMecIf )
    {
        pTlsMecIf = (PTLS_MEC_INTERFACE)AnscAllocateMemory(sizeof(TLS_MEC_INTERFACE));

        if ( !pTlsMecIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hTlsMecIf = (ANSC_HANDLE)pTlsMecIf;
        }

        rc = strcpy_s(pTlsMecIf->Name, sizeof(pTlsMecIf->Name), TLS_MEC_INTERFACE_NAME);
        ERR_CHK(rc);

        pTlsMecIf->InterfaceId          = TLS_MEC_INTERFACE_ID;
        pTlsMecIf->hOwnerContext        = (ANSC_HANDLE)pMyObject;
        pTlsMecIf->Size                 = sizeof(TLS_MEC_INTERFACE);

        pTlsMecIf->GetTlsSessionManager = TlsEcoMecGetTlsSessionManager;
        pTlsMecIf->GetTlsCryptoProvider = TlsEcoMecGetTlsCryptoProvider;
        pTlsMecIf->GetTlsHsmIf          = TlsEcoMecGetTlsHsmIf;
        pTlsMecIf->GetModuleParams      = TlsEcoMecGetModuleParams;
        pTlsMecIf->GetCertChain1        = TlsEcoMecGetCertChain1;
        pTlsMecIf->GetCertChain2        = TlsEcoMecGetCertChain2;
        pTlsMecIf->GetCertChain3        = TlsEcoMecGetCertChain3;
        pTlsMecIf->GetProfile           = TlsEcoMecGetProfile;
        pTlsMecIf->GetCACertByCert      = TlsEcoMecGetCACertByCert;
        pTlsMecIf->GetCACertByKey       = TlsEcoMecGetCACertByKey;
        pTlsMecIf->GetCACertByIssuer    = TlsEcoMecGetCACertByIssuer;
    }

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsEcoInitialize
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
TlsEcoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_ENV_CONTROLLER_OBJECT      pMyObject    = (PTLS_ENV_CONTROLLER_OBJECT)hThisObject;

    /*
     * Until you have to simulate C++ object-oriented progtlsming style with standard C, you don't
     * appreciate all the nice little things come with C++ language and all the dirty works that
     * have been done by the C++ compilers. Member initialization is one of these things. While in
     * C++ you don't have to initialize all the member fields inherited from the base class since
     * the compiler will do it for you, such is not the case with C.
     */
    AnscCoInitialize((ANSC_HANDLE)pMyObject);

    /*
     * Although we have initialized some of the member fields in the "create" member function, we
     * repeat the work here for completeness. While this simulation approach is pretty stupid from
     * a C++/Java progtlsmer perspective, it's the best we can get for universal embedded network
     * progtlsming. Before we develop our own operating system (don't expect that to happen any
     * time soon), this is the way things gonna be.
     */
    pMyObject->Oid                     = TLS_ENV_CONTROLLER_OID;
    pMyObject->Create                  = TlsEcoCreate;
    pMyObject->Remove                  = TlsEcoRemove;
    pMyObject->EnrollObjects           = TlsEcoEnrollObjects;
    pMyObject->Initialize              = TlsEcoInitialize;

    pMyObject->bActive                 = FALSE;

    pMyObject->GetProperty             = TlsEcoGetProperty;
    pMyObject->SetProperty             = TlsEcoSetProperty;
    pMyObject->ResetProperty           = TlsEcoResetProperty;
    pMyObject->Reset                   = TlsEcoReset;

    pMyObject->Engage                  = TlsEcoEngage;
    pMyObject->Cancel                  = TlsEcoCancel;

    pMyObject->Ssl2ToSsl3ClientHello   = TlsEcoSsl2ToSsl3ClientHello;

#if 0
    pMyObject->ScsConfigModule         = TlsEcoScsConfigModule;
    pMyObject->ScsCreateConnection     = TlsEcoScsCreateConnection;
    pMyObject->ScsConfigConnection     = TlsEcoScsConfigConnection;
    pMyObject->ScsRemoveConnection     = TlsEcoScsRemoveConnection;

    pMyObject->ScsStartConnection      = TlsEcoScsStartConnection;
    pMyObject->ScsCloseConnection      = TlsEcoScsCloseConnection;
    pMyObject->ScsQueryConnection      = TlsEcoScsQueryConnection;

    pMyObject->ScsRecvTlsMessage       = TlsEcoScsRecvTlsMessage;
    pMyObject->ScsSendAppMessage       = TlsEcoScsSendAppMessage;

    pMyObject->MecGetTlsSessionManager = TlsEcoMecGetTlsSessionManager;
    pMyObject->MecGetTlsCryptoProvider = TlsEcoMecGetTlsCryptoProvider;
    pMyObject->MecGetTlsHsmIf          = TlsEcoMecGetTlsHsmIf;
    pMyObject->MecGetModuleParams      = TlsEcoMecGetModuleParams;
    pMyObject->MecGetCertChain1        = TlsEcoMecGetCertChain1;
    pMyObject->MecGetCertChain2        = TlsEcoMecGetCertChain2;
    pMyObject->MecGetCertChain3        = TlsEcoMecGetCertChain3;
    pMyObject->MecGetProfile           = TlsEcoMecGetProfile;
    pMyObject->MecGetCACertByCert      = TlsEcoMecGetCACertByCert;
    pMyObject->MecGetCACertByKey       = TlsEcoMecGetCACertByKey;
    pMyObject->MecGetCACertByIssuer    = TlsEcoMecGetCACertByIssuer;
#endif

    /*
     * We shall initialize the object properties to the default values, which may be changed later
     * via the exposed member functions. If any of the future extensions needs to change the object
     * property, the following code also needs to be changed.
     */
    pMyObject->ResetProperty((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
