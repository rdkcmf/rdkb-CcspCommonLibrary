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

    module:	http_psover2_base.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Http Pso Ver2 Object.

        *   HttpPsoVer2Create
        *   HttpPsoVer2Remove
        *   HttpPsoVer2EnrollObjects
        *   HttpPsoVer2Initialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/04/02    initial revision.

**********************************************************************/


#include "http_psover2_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        HttpPsoVer2Create
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Http Pso Ver2 Object and
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
HttpPsoVer2Create
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PHTTP_PSO_VER2_OBJECT           pMyObject    = NULL;
    errno_t                         rc           = -1;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PHTTP_PSO_VER2_OBJECT)AnscAllocateMemory(sizeof(HTTP_PSO_VER2_OBJECT));

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
    rc = strcpy_s(pBaseObject->Name, sizeof(pBaseObject->Name), HTTP_PSO_VER2_NAME);
    ERR_CHK(rc);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = HTTP_PSO_VER2_OID;
    pBaseObject->Create            = HttpPsoVer2Create;
    pBaseObject->Remove            = HttpPsoVer2Remove;
    pBaseObject->EnrollObjects     = HttpPsoVer2EnrollObjects;
    pBaseObject->Initialize        = HttpPsoVer2Initialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoVer2Remove
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
HttpPsoVer2Remove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PSO_VER2_OBJECT           pMyObject    = (PHTTP_PSO_VER2_OBJECT)hThisObject;

    pMyObject->Reset((ANSC_HANDLE)pMyObject);

    AnscFreeLock(&pMyObject->AccessLock  );
    AnscFreeLock(&pMyObject->TroSListLock);
    AnscCoRemove((ANSC_HANDLE)pMyObject  );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoVer2EnrollObjects
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
HttpPsoVer2EnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PSO_VER2_OBJECT           pMyObject    = (PHTTP_PSO_VER2_OBJECT)hThisObject;

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoVer2Initialize
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
HttpPsoVer2Initialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PSO_VER2_OBJECT           pMyObject    = (PHTTP_PSO_VER2_OBJECT)hThisObject;

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
    pMyObject->Oid                = HTTP_PSO_VER2_OID;
    pMyObject->Create             = HttpPsoVer2Create;
    pMyObject->Remove             = HttpPsoVer2Remove;
    pMyObject->EnrollObjects      = HttpPsoVer2EnrollObjects;
    pMyObject->Initialize         = HttpPsoVer2Initialize;

    pMyObject->hWamIf             = (ANSC_HANDLE)NULL;
    pMyObject->hSbcIf             = (ANSC_HANDLE)NULL;
    pMyObject->hCbcIf             = (ANSC_HANDLE)NULL;
    pMyObject->hPbcIf             = (ANSC_HANDLE)NULL;
    pMyObject->hHfpIf             = (ANSC_HANDLE)NULL;
    pMyObject->hClientSocket      = (ANSC_HANDLE)NULL;
    pMyObject->hSbcContext        = (ANSC_HANDLE)NULL;
    pMyObject->hCbcContext        = (ANSC_HANDLE)NULL;
    pMyObject->HashIndex          = 0;
    pMyObject->SbcPmode           = HTTP_SBC_PMODE_RELAY1;
    pMyObject->CbcPmode           = HTTP_CBC_PMODE_RELAY1;
    pMyObject->SessionState       = HTTP_PSOVER2_STATE_INITIALIZED;

    pMyObject->GetWamIf           = HttpPsoVer2GetWamIf;
    pMyObject->SetWamIf           = HttpPsoVer2SetWamIf;
    pMyObject->GetSbcIf           = HttpPsoVer2GetSbcIf;
    pMyObject->SetSbcIf           = HttpPsoVer2SetSbcIf;
    pMyObject->GetCbcIf           = HttpPsoVer2GetCbcIf;
    pMyObject->SetCbcIf           = HttpPsoVer2SetCbcIf;
    pMyObject->GetPbcIf           = HttpPsoVer2GetPbcIf;
    pMyObject->SetPbcIf           = HttpPsoVer2SetPbcIf;
    pMyObject->GetHfpIf           = HttpPsoVer2GetHfpIf;
    pMyObject->SetHfpIf           = HttpPsoVer2SetHfpIf;

    pMyObject->GetClientSocket    = HttpPsoVer2GetClientSocket;
    pMyObject->SetClientSocket    = HttpPsoVer2SetClientSocket;
    pMyObject->GetSbcContext      = HttpPsoVer2GetSbcContext;
    pMyObject->SetSbcContext      = HttpPsoVer2SetSbcContext;
    pMyObject->GetCbcContext      = HttpPsoVer2GetCbcContext;
    pMyObject->SetCbcContext      = HttpPsoVer2SetCbcContext;

    pMyObject->GetSbcPmode        = HttpPsoVer2GetSbcPmode;
    pMyObject->SetSbcPmode        = HttpPsoVer2SetSbcPmode;
    pMyObject->GetCbcPmode        = HttpPsoVer2GetCbcPmode;
    pMyObject->SetCbcPmode        = HttpPsoVer2SetCbcPmode;
    pMyObject->GetSessionState    = HttpPsoVer2GetSessionState;
    pMyObject->SetSessionState    = HttpPsoVer2SetSessionState;

    pMyObject->Return             = HttpPsoVer2Return;
    pMyObject->Reset              = HttpPsoVer2Reset;

    pMyObject->Open               = HttpPsoVer2Open;
    pMyObject->Close              = HttpPsoVer2Close;

    pMyObject->AcquireAccess      = HttpPsoVer2AcquireAccess;
    pMyObject->ReleaseAccess      = HttpPsoVer2ReleaseAccess;

    pMyObject->AskTroBySocket     = HttpPsoVer2AskTroBySocket;
    pMyObject->PopTroBySocket     = HttpPsoVer2PopTroBySocket;
    pMyObject->GetLastTro         = HttpPsoVer2GetLastTro;
    pMyObject->GetCurTro          = HttpPsoVer2GetCurTro;
    pMyObject->AddNewTro          = HttpPsoVer2AddNewTro;
    pMyObject->DelAllTros         = HttpPsoVer2DelAllTros;

    pMyObject->QueryForClient     = HttpPsoVer2QueryForClient;
    pMyObject->RecvFromClient     = HttpPsoVer2RecvFromClient;
    pMyObject->FinishedByClient   = HttpPsoVer2FinishedByClient;
    pMyObject->AcceptClient       = HttpPsoVer2AcceptClient;
    pMyObject->Authenticate       = HttpPsoVer2Authenticate;

    AnscInitializeLock       (&pMyObject->AccessLock  );
    AnscSListInitializeHeader(&pMyObject->TroSList    );
    AnscInitializeLock       (&pMyObject->TroSListLock);

    return  ANSC_STATUS_SUCCESS;
}
