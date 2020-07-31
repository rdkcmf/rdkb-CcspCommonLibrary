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

    module:	tls_rko_base.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the TLS Record Keeper Object.

        *   TlsRkoCreate
        *   TlsRkoRemove
        *   TlsRkoEnrollObjects
        *   TlsRkoInitialize

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


#include "tls_rko_global.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        TlsRkoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the TLS Record Keeper Object and
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
TlsRkoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PTLS_RECORD_KEEPER_OBJECT       pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PTLS_RECORD_KEEPER_OBJECT)AnscAllocateMemory(sizeof(TLS_RECORD_KEEPER_OBJECT));

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
    /* AnscCopyString(pBaseObject->Name, TLS_RECORD_KEEPER_NAME); */

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = TLS_RECORD_KEEPER_OID;
    pBaseObject->Create            = TlsRkoCreate;
    pBaseObject->Remove            = TlsRkoRemove;
    pBaseObject->EnrollObjects     = TlsRkoEnrollObjects;
    pBaseObject->Initialize        = TlsRkoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsRkoRemove
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
TlsRkoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_RECORD_KEEPER_OBJECT       pMyObject    = (PTLS_RECORD_KEEPER_OBJECT)hThisObject;

    pMyObject->Cancel((ANSC_HANDLE)pMyObject);
    pMyObject->Reset ((ANSC_HANDLE)pMyObject);

    AnscFreeLock(&pMyObject->RcoTableLock);
    AnscCoRemove((ANSC_HANDLE)pMyObject  );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsRkoEnrollObjects
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
TlsRkoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    AnscCoEnrollObjects((ANSC_HANDLE)hThisObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsRkoInitialize
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
TlsRkoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_RECORD_KEEPER_OBJECT       pMyObject    = (PTLS_RECORD_KEEPER_OBJECT)hThisObject;
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
    pMyObject->Oid                 = TLS_RECORD_KEEPER_OID;
    pMyObject->Create              = TlsRkoCreate;
    pMyObject->Remove              = TlsRkoRemove;
    pMyObject->EnrollObjects       = TlsRkoEnrollObjects;
    pMyObject->Initialize          = TlsRkoInitialize;

    pMyObject->hTlsCbcIf           = (ANSC_HANDLE)NULL;
    pMyObject->hTlsMecIf           = (ANSC_HANDLE)NULL;
    pMyObject->hTlsTsaIf           = (ANSC_HANDLE)NULL;
    pMyObject->bActive             = FALSE;

    pMyObject->GetProperty         = TlsRkoGetProperty;
    pMyObject->SetProperty         = TlsRkoSetProperty;
    pMyObject->ResetProperty       = TlsRkoResetProperty;
    pMyObject->Reset               = TlsRkoReset;

    pMyObject->Engage              = TlsRkoEngage;
    pMyObject->Cancel              = TlsRkoCancel;
    pMyObject->ChangeStateW        = TlsRkoChangeStateW;
    pMyObject->ChangeStateR        = TlsRkoChangeStateR;

    pMyObject->GetRecordClient     = TlsRkoGetRecordClient;
    pMyObject->AddRecordClient     = TlsRkoAddRecordClient;
    pMyObject->PopRecordClient     = TlsRkoPopRecordClient;
    pMyObject->DelRecordClient     = TlsRkoDelRecordClient;
    pMyObject->DelAllRecordClients = TlsRkoDelAllRecordClients;

    pMyObject->Recv                = TlsRkoRecv;
    pMyObject->Send                = TlsRkoSend;

    for ( i = 0; i < TLS_RKO_RCO_TABLE_SIZE; i++ )
    {
        AnscQueueInitializeHeader(&pMyObject->RcoTable[i]);
    }
    AnscInitializeLock(&pMyObject->RcoTableLock);

    /*
     * We shall initialize the object properties to the default values, which may be changed later
     * via the exposed member functions. If any of the future extensions needs to change the object
     * property, the following code also needs to be changed.
     */
    pMyObject->ResetProperty((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
