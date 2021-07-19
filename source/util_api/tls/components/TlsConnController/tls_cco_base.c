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

    module:	tls_cco_base.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the TLS Conn Controller Object.

        *   TlsCcoCreate
        *   TlsCcoRemove
        *   TlsCcoEnrollObjects
        *   TlsCcoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/26/03    initial revision.

**********************************************************************/


#include "tls_cco_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        TlsCcoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the TLS Conn Controller Object and
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
TlsCcoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PTLS_CONN_CONTROLLER_OBJECT     pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PTLS_CONN_CONTROLLER_OBJECT)AnscAllocateMemory(sizeof(TLS_CONN_CONTROLLER_OBJECT));

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
    /* AnscCopyString(pBaseObject->Name, TLS_CONN_CONTROLLER_NAME); */

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = TLS_CONN_CONTROLLER_OID;
    pBaseObject->Create            = TlsCcoCreate;
    pBaseObject->Remove            = TlsCcoRemove;
    pBaseObject->EnrollObjects     = TlsCcoEnrollObjects;
    pBaseObject->Initialize        = TlsCcoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCcoRemove
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
TlsCcoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_CONN_CONTROLLER_OBJECT     pMyObject    = (PTLS_CONN_CONTROLLER_OBJECT)hThisObject;
    PANSC_PACKET_DESCRIPTOR         pTbpPdo      = (PANSC_PACKET_DESCRIPTOR    )pMyObject->hTbpPdo;
    PTLS_CBC_INTERFACE              pTlsCbcIf    = (PTLS_CBC_INTERFACE         )pMyObject->hTlsCbcIf;

    pMyObject->Cancel((ANSC_HANDLE)pMyObject);

    if ( pTbpPdo )
    {
        AnscFreePdo((ANSC_HANDLE)pTbpPdo);

        pMyObject->hTbpPdo = (ANSC_HANDLE)NULL;
    }

    if ( pTlsCbcIf )
    {
        AnscFreeMemory(pTlsCbcIf);

        pMyObject->hTlsCbcIf = (ANSC_HANDLE)NULL;
    }

    AnscFreeLock(&pMyObject->AccessLock);
    AnscCoRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCcoEnrollObjects
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
TlsCcoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_CONN_CONTROLLER_OBJECT     pMyObject    = (PTLS_CONN_CONTROLLER_OBJECT)hThisObject;
    PANSC_PACKET_DESCRIPTOR         pTbpPdo      = (PANSC_PACKET_DESCRIPTOR    )pMyObject->hTbpPdo;
    PTLS_CBC_INTERFACE              pTlsCbcIf    = (PTLS_CBC_INTERFACE         )pMyObject->hTlsCbcIf;
    errno_t                         rc           = -1;

    if ( !pTbpPdo )
    {
        pTbpPdo = (PANSC_PACKET_DESCRIPTOR)AnscAllocatePdo();

        if ( !pTbpPdo )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hTbpPdo = (ANSC_HANDLE)pTbpPdo;
        }
    }

    if ( !pTlsCbcIf )
    {
        pTlsCbcIf = (PTLS_CBC_INTERFACE)AnscAllocateMemory(sizeof(TLS_CBC_INTERFACE));

        if ( !pTlsCbcIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hTlsCbcIf = (ANSC_HANDLE)pTlsCbcIf;
        }

        rc = strcpy_s(pTlsCbcIf->Name, sizeof(pTlsCbcIf->Name), TLS_CBC_INTERFACE_NAME);
        ERR_CHK(rc);

        pTlsCbcIf->InterfaceId         = TLS_CBC_INTERFACE_ID;
        pTlsCbcIf->hOwnerContext       = (ANSC_HANDLE)pMyObject;
        pTlsCbcIf->Size                = sizeof(TLS_CBC_INTERFACE);

        pTlsCbcIf->NotifyEvent         = TlsCcoCbcNotifyEvent;
        pTlsCbcIf->GetConnParams       = TlsCcoCbcGetConnParams;
        pTlsCbcIf->SetSessionID        = TlsCcoCbcSetSessionID;
        pTlsCbcIf->SetSessionSharing   = TlsCcoCbcSetSessionSharing;
        pTlsCbcIf->SetQuickHandshake   = TlsCcoCbcSetQuickHandshake;
        pTlsCbcIf->SetCipherSuiteArray = TlsCcoCbcSetCipherSuiteArray;
        pTlsCbcIf->SetCompressionArray = TlsCcoCbcSetCompressionArray;
        pTlsCbcIf->GenerateAlert       = TlsCcoCbcGenerateAlert;
        pTlsCbcIf->ChangeCipherSpec    = TlsCcoCbcChangeCipherSpec;
    }

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCcoInitialize
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
TlsCcoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_CONN_CONTROLLER_OBJECT     pMyObject    = (PTLS_CONN_CONTROLLER_OBJECT)hThisObject;

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
    pMyObject->Oid                    = TLS_CONN_CONTROLLER_OID;
    pMyObject->Create                 = TlsCcoCreate;
    pMyObject->Remove                 = TlsCcoRemove;
    pMyObject->EnrollObjects          = TlsCcoEnrollObjects;
    pMyObject->Initialize             = TlsCcoInitialize;

    pMyObject->hTlsRecordKeeper       = (ANSC_HANDLE)NULL;
    pMyObject->hTlsHandShaker         = (ANSC_HANDLE)NULL;
    pMyObject->hTlsAlertHandler       = (ANSC_HANDLE)NULL;
    pMyObject->hTlsCipherSwitch       = (ANSC_HANDLE)NULL;
    pMyObject->hTlsAppConnector       = (ANSC_HANDLE)NULL;
    pMyObject->hTlsMecIf              = (ANSC_HANDLE)NULL;
    pMyObject->hTlsTsaIf              = (ANSC_HANDLE)NULL;
    pMyObject->bActive                = FALSE;
    pMyObject->bBroken                = FALSE;

    pMyObject->GetProperty            = TlsCcoGetProperty;
    pMyObject->SetProperty            = TlsCcoSetProperty;
    pMyObject->ResetProperty          = TlsCcoResetProperty;

    pMyObject->AcqAccess              = TlsCcoAcqAccess;
    pMyObject->RelAccess              = TlsCcoRelAccess;
    pMyObject->Engage                 = TlsCcoEngage;
    pMyObject->Cancel                 = TlsCcoCancel;
    pMyObject->Config                 = TlsCcoConfig;

    pMyObject->StartHandshake         = TlsCcoStartHandshake;
    pMyObject->CloseSession           = TlsCcoCloseSession;

    pMyObject->RecvTlsMessage         = TlsCcoRecvTlsMessage;
    pMyObject->SendAppMessage         = TlsCcoSendAppMessage;

#if 0
    pMyObject->CbcNotifyEvent         = TlsCcoCbcNotifyEvent;
    pMyObject->CbcGetConnParams       = TlsCcoCbcGetConnParams;
    pMyObject->CbcSetSessionID        = TlsCcoCbcSetSessionID;
    pMyObject->CbcSetSessionSharing   = TlsCcoCbcSetSessionSharing;
    pMyObject->CbcSetQuickHandshake   = TlsCcoCbcSetQuickHandshake;
    pMyObject->CbcSetCipherSuiteArray = TlsCcoCbcSetCipherSuiteArray;
    pMyObject->CbcSetCompressionArray = TlsCcoCbcSetCompressionArray;
    pMyObject->CbcGenerateAlert       = TlsCcoCbcGenerateAlert;
    pMyObject->CbcChangeCipherSpec    = TlsCcoCbcChangeCipherSpec;
#endif

    AnscInitializeLock(&pMyObject->AccessLock);

    /*
     * We shall initialize the object properties to the default values, which may be changed later
     * via the exposed member functions. If any of the future extensions needs to change the object
     * property, the following code also needs to be changed.
     */
    pMyObject->ResetProperty((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
