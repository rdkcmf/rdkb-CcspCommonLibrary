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

    module:	tls_smo_base.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the TLS Session Manager Object.

        *   TlsSmoCreate
        *   TlsSmoRemove
        *   TlsSmoEnrollObjects
        *   TlsSmoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/16/03    initial revision.

**********************************************************************/


#include "tls_smo_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        TlsSmoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the TLS Session Manager Object and
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
TlsSmoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PTLS_SESSION_MANAGER_OBJECT     pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PTLS_SESSION_MANAGER_OBJECT)AnscAllocateMemory(sizeof(TLS_SESSION_MANAGER_OBJECT));

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
    /* AnscCopyString(pBaseObject->Name, TLS_SESSION_MANAGER_NAME); */

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = TLS_SESSION_MANAGER_OID;
    pBaseObject->Create            = TlsSmoCreate;
    pBaseObject->Remove            = TlsSmoRemove;
    pBaseObject->EnrollObjects     = TlsSmoEnrollObjects;
    pBaseObject->Initialize        = TlsSmoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsSmoRemove
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
TlsSmoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_SESSION_MANAGER_OBJECT     pMyObject    = (PTLS_SESSION_MANAGER_OBJECT  )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pPatrolTimer = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hPatrolTimer;
    PANSC_TDO_CLIENT_OBJECT         pTimerClient = (PANSC_TDO_CLIENT_OBJECT      )pMyObject->hTimerClient;
    PTLS_HSM_INTERFACE              pTlsHsmIf    = (PTLS_HSM_INTERFACE           )pMyObject->hTlsHsmIf;

    pMyObject->Cancel((ANSC_HANDLE)pMyObject);
    pMyObject->Reset ((ANSC_HANDLE)pMyObject);

    if ( pPatrolTimer )
    {
        pPatrolTimer->Remove((ANSC_HANDLE)pPatrolTimer);
    }

    if ( pTimerClient )
    {
        AnscFreeMemory(pTimerClient);
    }

    if ( pTlsHsmIf )
    {
        AnscFreeMemory(pTlsHsmIf);

        pMyObject->hTlsHsmIf = (ANSC_HANDLE)NULL;
    }

    AnscFreeLock(&pMyObject->SeoTableLock);
    AnscCoRemove((ANSC_HANDLE)pMyObject  );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsSmoEnrollObjects
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
TlsSmoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_SESSION_MANAGER_OBJECT     pMyObject    = (PTLS_SESSION_MANAGER_OBJECT  )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pPatrolTimer = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hPatrolTimer;
    PANSC_TDO_CLIENT_OBJECT         pTimerClient = (PANSC_TDO_CLIENT_OBJECT      )pMyObject->hTimerClient;
    PTLS_HSM_INTERFACE              pTlsHsmIf    = (PTLS_HSM_INTERFACE           )pMyObject->hTlsHsmIf;
    errno_t                         rc           = -1;

    if ( !pPatrolTimer )
    {
        pPatrolTimer =
            (PANSC_TIMER_DESCRIPTOR_OBJECT)AnscCreateTimerDescriptor
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pPatrolTimer )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hPatrolTimer = (ANSC_HANDLE)pPatrolTimer;
        }

        pPatrolTimer->SetTimerType((ANSC_HANDLE)pPatrolTimer, ANSC_TIMER_TYPE_PERIODIC     );
        pPatrolTimer->SetInterval ((ANSC_HANDLE)pPatrolTimer, TLS_SMO_PATROL_TIMER_INTERVAL);
    }

    if ( !pTimerClient )
    {
        pTimerClient = (PANSC_TDO_CLIENT_OBJECT)AnscAllocateMemory(sizeof(ANSC_TDO_CLIENT_OBJECT));

        if ( !pTimerClient )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hTimerClient = (ANSC_HANDLE)pTimerClient;
        }

        pTimerClient->hClientContext = (ANSC_HANDLE)pMyObject;
        pTimerClient->Invoke         = TlsSmoPatrolTimerInvoke;

        pPatrolTimer->SetClient((ANSC_HANDLE)pPatrolTimer, (ANSC_HANDLE)pTimerClient);
    }

    if ( !pTlsHsmIf )
    {
        pTlsHsmIf = (PTLS_HSM_INTERFACE)AnscAllocateMemory(sizeof(TLS_HSM_INTERFACE));

        if ( !pTlsHsmIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hTlsHsmIf = (ANSC_HANDLE)pTlsHsmIf;
        }

        rc = strcpy_s(pTlsHsmIf->Name, sizeof(pTlsHsmIf->Name), TLS_HSM_INTERFACE_NAME);
        ERR_CHK(rc);

        pTlsHsmIf->InterfaceId             = TLS_HSM_INTERFACE_ID;
        pTlsHsmIf->hOwnerContext           = (ANSC_HANDLE)pMyObject;
        pTlsHsmIf->Size                    = sizeof(TLS_HSM_INTERFACE);

        pTlsHsmIf->SelHandshakeSession     = TlsSmoHsmSelHandshakeSession;
        pTlsHsmIf->GetHandshakeSession     = TlsSmoHsmGetHandshakeSession;
        pTlsHsmIf->AddHandshakeSession     = TlsSmoHsmAddHandshakeSession;
        pTlsHsmIf->DelHandshakeSession     = TlsSmoHsmDelHandshakeSession;
        pTlsHsmIf->DelAllHandshakeSessions = TlsSmoHsmDelAllHandshakeSessions;
    }

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsSmoInitialize
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
TlsSmoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_SESSION_MANAGER_OBJECT     pMyObject    = (PTLS_SESSION_MANAGER_OBJECT)hThisObject;
    ULONG                           i            = 0;

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
    pMyObject->Oid                        = TLS_SESSION_MANAGER_OID;
    pMyObject->Create                     = TlsSmoCreate;
    pMyObject->Remove                     = TlsSmoRemove;
    pMyObject->EnrollObjects              = TlsSmoEnrollObjects;
    pMyObject->Initialize                 = TlsSmoInitialize;

    pMyObject->hTlsMecIf                  = (ANSC_HANDLE)NULL;
    pMyObject->bActive                    = FALSE;

    pMyObject->GetProperty                = TlsSmoGetProperty;
    pMyObject->SetProperty                = TlsSmoSetProperty;
    pMyObject->ResetProperty              = TlsSmoResetProperty;
    pMyObject->Reset                      = TlsSmoReset;

    pMyObject->Engage                     = TlsSmoEngage;
    pMyObject->Cancel                     = TlsSmoCancel;
    pMyObject->PatrolTimerInvoke          = TlsSmoPatrolTimerInvoke;

    pMyObject->GetSession                 = TlsSmoGetSession;
    pMyObject->AddSession                 = TlsSmoAddSession;
    pMyObject->DelSession                 = TlsSmoDelSession;
    pMyObject->DelAllSessions             = TlsSmoDelAllSessions;

#if 0
    pMyObject->HsmSelHandshakeSession     = TlsSmoHsmSelHandshakeSession;
    pMyObject->HsmGetHandshakeSession     = TlsSmoHsmGetHandshakeSession;
    pMyObject->HsmAddHandshakeSession     = TlsSmoHsmAddHandshakeSession;
    pMyObject->HsmDelHandshakeSession     = TlsSmoHsmDelHandshakeSession;
    pMyObject->HsmDelAllHandshakeSessions = TlsSmoHsmDelAllHandshakeSessions;
#endif

    for ( i = 0; i < TLS_SMO_SEO_TABLE_SIZE; i++ )
    {
        AnscQueueInitializeHeader(&pMyObject->SeoTable[i]);
    }
    AnscInitializeLock(&pMyObject->SeoTableLock);

    /*
     * We shall initialize the object properties to the default values, which may be changed later
     * via the exposed member functions. If any of the future extensions needs to change the object
     * property, the following code also needs to be changed.
     */
    pMyObject->ResetProperty((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
