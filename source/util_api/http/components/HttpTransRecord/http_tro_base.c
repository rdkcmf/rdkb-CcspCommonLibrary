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

    module:	http_tro_base.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Http Trans Record Object.

        *   HttpTroCreate
        *   HttpTroRemove
        *   HttpTroEnrollObjects
        *   HttpTroInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/05/02    initial revision.

**********************************************************************/


#include "http_tro_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        HttpTroCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Http Trans Record Object and
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
HttpTroCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PHTTP_TRANS_RECORD_OBJECT       pMyObject    = NULL;
    errno_t                         rc           = -1;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PHTTP_TRANS_RECORD_OBJECT)AnscAllocateMemory(sizeof(HTTP_TRANS_RECORD_OBJECT));

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
    rc = strcpy_s(pBaseObject->Name, sizeof(pBaseObject->Name), HTTP_TRANS_RECORD_NAME);
    ERR_CHK(rc);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = HTTP_TRANS_RECORD_OID;
    pBaseObject->Create            = HttpTroCreate;
    pBaseObject->Remove            = HttpTroRemove;
    pBaseObject->EnrollObjects     = HttpTroEnrollObjects;
    pBaseObject->Initialize        = HttpTroInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpTroRemove
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
HttpTroRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject    = (PHTTP_TRANS_RECORD_OBJECT)hThisObject;
    PHTTP_TMH_INTERFACE             pTmhClientIf = (PHTTP_TMH_INTERFACE      )pMyObject->hTmhClientIf;
    PHTTP_TMH_INTERFACE             pTmhServerIf = (PHTTP_TMH_INTERFACE      )pMyObject->hTmhServerIf;

    pMyObject->Reset((ANSC_HANDLE)pMyObject);

    if ( pTmhClientIf )
    {
        AnscFreeMemory(pTmhClientIf);
    }

    if ( pTmhServerIf )
    {
        AnscFreeMemory(pTmhServerIf);
    }

    AnscFreeLock(&pMyObject->AccessLock);
    AnscCoRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpTroEnrollObjects
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
HttpTroEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject    = (PHTTP_TRANS_RECORD_OBJECT)hThisObject;
    PHTTP_TMH_INTERFACE             pTmhClientIf = (PHTTP_TMH_INTERFACE      )pMyObject->hTmhClientIf;
    PHTTP_TMH_INTERFACE             pTmhServerIf = (PHTTP_TMH_INTERFACE      )pMyObject->hTmhServerIf;

    if ( !pTmhClientIf )
    {
        pTmhClientIf = (PHTTP_TMH_INTERFACE)AnscAllocateMemory(sizeof(HTTP_TMH_INTERFACE));

        if ( !pTmhClientIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hTmhClientIf = (ANSC_HANDLE)pTmhClientIf;
        }

        pTmhClientIf->hOwnerContext = (ANSC_HANDLE)pMyObject;
        pTmhClientIf->Size          = sizeof(HTTP_TMH_INTERFACE);
        pTmhClientIf->Notify        = HttpTroTmhClientNotify;
        pTmhClientIf->Serialize     = HttpTroTmhClientSerialize;
    }

    if ( !pTmhServerIf )
    {
        pTmhServerIf = (PHTTP_TMH_INTERFACE)AnscAllocateMemory(sizeof(HTTP_TMH_INTERFACE));

        if ( !pTmhServerIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hTmhServerIf = (ANSC_HANDLE)pTmhServerIf;
        }

        pTmhServerIf->hOwnerContext = (ANSC_HANDLE)pMyObject;
        pTmhServerIf->Size          = sizeof(HTTP_TMH_INTERFACE);
        pTmhServerIf->Notify        = HttpTroTmhServerNotify;
        pTmhServerIf->Serialize     = HttpTroTmhServerSerialize;
    }

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpTroInitialize
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
HttpTroInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject    = (PHTTP_TRANS_RECORD_OBJECT)hThisObject;

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
    pMyObject->Oid                = HTTP_TRANS_RECORD_OID;
    pMyObject->Create             = HttpTroCreate;
    pMyObject->Remove             = HttpTroRemove;
    pMyObject->EnrollObjects      = HttpTroEnrollObjects;
    pMyObject->Initialize         = HttpTroInitialize;

    pMyObject->ServerAddr.Value   = 0;
    pMyObject->ServerPort         = HTTP_SERVER_PORT;

    pMyObject->hWamIf             = (ANSC_HANDLE)NULL;
    pMyObject->hSbcIf             = (ANSC_HANDLE)NULL;
    pMyObject->hCbcIf             = (ANSC_HANDLE)NULL;
    pMyObject->hPbcIf             = (ANSC_HANDLE)NULL;
    pMyObject->hHfpIf             = (ANSC_HANDLE)NULL;
    pMyObject->hBmoReq            = (ANSC_HANDLE)NULL;
    pMyObject->hBmoRep            = (ANSC_HANDLE)NULL;
    pMyObject->hClientSocket      = (ANSC_HANDLE)NULL;
    pMyObject->hServerSocket      = (ANSC_HANDLE)NULL;
    pMyObject->hSbcContext        = (ANSC_HANDLE)NULL;
    pMyObject->hCbcContext        = (ANSC_HANDLE)NULL;
    pMyObject->HashIndex          = 0;
    pMyObject->SbcPmode           = HTTP_SBC_PMODE_RELAY1;
    pMyObject->CbcPmode           = HTTP_CBC_PMODE_RELAY1;
    pMyObject->TransState         = HTTP_TRO_STATE_INITIALIZED;
    pMyObject->bCloseConnection   = TRUE;

    pMyObject->GetWamIf           = HttpTroGetWamIf;
    pMyObject->SetWamIf           = HttpTroSetWamIf;
    pMyObject->GetSbcIf           = HttpTroGetSbcIf;
    pMyObject->SetSbcIf           = HttpTroSetSbcIf;
    pMyObject->GetCbcIf           = HttpTroGetCbcIf;
    pMyObject->SetCbcIf           = HttpTroSetCbcIf;
    pMyObject->GetPbcIf           = HttpTroGetPbcIf;
    pMyObject->SetPbcIf           = HttpTroSetPbcIf;
    pMyObject->GetHfpIf           = HttpTroGetHfpIf;
    pMyObject->SetHfpIf           = HttpTroSetHfpIf;

    pMyObject->GetBmoReq          = HttpTroGetBmoReq;
    pMyObject->SetBmoReq          = HttpTroSetBmoReq;
    pMyObject->GetBmoRep          = HttpTroGetBmoRep;
    pMyObject->SetBmoRep          = HttpTroSetBmoRep;
    pMyObject->GetClientSocket    = HttpTroGetClientSocket;
    pMyObject->SetClientSocket    = HttpTroSetClientSocket;
    pMyObject->GetServerSocket    = HttpTroGetServerSocket;
    pMyObject->SetServerSocket    = HttpTroSetServerSocket;
    pMyObject->GetSbcContext      = HttpTroGetSbcContext;
    pMyObject->SetSbcContext      = HttpTroSetSbcContext;
    pMyObject->GetCbcContext      = HttpTroGetCbcContext;
    pMyObject->SetCbcContext      = HttpTroSetCbcContext;

    pMyObject->GetServerName      = HttpTroGetServerName;
    pMyObject->SetServerName      = HttpTroSetServerName;
    pMyObject->GetClientAddr      = HttpTroGetClientAddr;
    pMyObject->GetServerAddr      = HttpTroGetServerAddr;
    pMyObject->GetClientPort      = HttpTroGetClientPort;
    pMyObject->GetServerPort      = HttpTroGetServerPort;

    pMyObject->GetSbcPmode        = HttpTroGetSbcPmode;
    pMyObject->SetSbcPmode        = HttpTroSetSbcPmode;
    pMyObject->GetCbcPmode        = HttpTroGetCbcPmode;
    pMyObject->SetCbcPmode        = HttpTroSetCbcPmode;
    pMyObject->GetTransState      = HttpTroGetTransState;
    pMyObject->SetTransState      = HttpTroSetTransState;

    pMyObject->Return             = HttpTroReturn;
    pMyObject->Reset              = HttpTroReset;

    pMyObject->Terminate          = HttpTroTerminate;
    pMyObject->Open               = HttpTroOpen;
    pMyObject->Close              = HttpTroClose;

    pMyObject->AcquireAccess      = HttpTroAcquireAccess;
    pMyObject->ReleaseAccess      = HttpTroReleaseAccess;

    pMyObject->QueryForClient     = HttpTroQueryForClient;
    pMyObject->RecvFromClient     = HttpTroRecvFromClient;
    pMyObject->FinishedByClient   = HttpTroFinishedByClient;
    pMyObject->SendToClient       = HttpTroSendToClient;
    pMyObject->ResumeTransaction  = HttpTroResumeTransaction;

    pMyObject->QueryForServer     = HttpTroQueryForServer;
    pMyObject->RecvFromServer     = HttpTroRecvFromServer;
    pMyObject->FinishedByServer   = HttpTroFinishedByServer;
    pMyObject->SendToServer       = HttpTroSendToServer;
    pMyObject->ConnectToServer    = HttpTroConnectToServer;
    pMyObject->SetUpConnection    = HttpTroSetUpConnection;

    pMyObject->TmhClientNotify    = HttpTroTmhClientNotify;
    pMyObject->TmhClientSerialize = HttpTroTmhClientSerialize;
    pMyObject->TmhServerNotify    = HttpTroTmhServerNotify;
    pMyObject->TmhServerSerialize = HttpTroTmhServerSerialize;

    AnscInitializeLock(&pMyObject->AccessLock);

    return  ANSC_STATUS_SUCCESS;
}
