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

    module:	http_pso_base.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Http Proxy Session Object.

        *   HttpPsoCreate
        *   HttpPsoRemove
        *   HttpPsoEnrollObjects
        *   HttpPsoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/22/02    initial revision.

**********************************************************************/


#include "http_pso_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        HttpPsoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Http Proxy Session Object and
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
HttpPsoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PHTTP_PROXY_SESSION_OBJECT      pMyObject    = NULL;
    errno_t                         rc           = -1;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PHTTP_PROXY_SESSION_OBJECT)AnscAllocateMemory(sizeof(HTTP_PROXY_SESSION_OBJECT));

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
    rc = strcpy_s(pBaseObject->Name, sizeof(pBaseObject->Name), HTTP_PROXY_SESSION_NAME);
    ERR_CHK(rc);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = HTTP_PROXY_SESSION_OID;
    pBaseObject->Create            = HttpPsoCreate;
    pBaseObject->Remove            = HttpPsoRemove;
    pBaseObject->EnrollObjects     = HttpPsoEnrollObjects;
    pBaseObject->Initialize        = HttpPsoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoRemove
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
HttpPsoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject    = (PHTTP_PROXY_SESSION_OBJECT)hThisObject;
    PHTTP_TMH_INTERFACE             pTmhClientIf = (PHTTP_TMH_INTERFACE       )pMyObject->hTmhClientIf;
    PHTTP_TMH_INTERFACE             pTmhServerIf = (PHTTP_TMH_INTERFACE       )pMyObject->hTmhServerIf;

    pMyObject->Reset((ANSC_HANDLE)pMyObject);

    if ( pTmhClientIf )
    {
        AnscFreeMemory(pTmhClientIf);
    }

    if ( pTmhServerIf )
    {
        AnscFreeMemory(pTmhServerIf);
    }

    AnscFreeLock(&pMyObject->AccessLock     );
    AnscFreeLock(&pMyObject->BmoReqSListLock);
    AnscFreeLock(&pMyObject->BmoRepSListLock);
    AnscCoRemove((ANSC_HANDLE)pMyObject     );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoEnrollObjects
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
HttpPsoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject    = (PHTTP_PROXY_SESSION_OBJECT)hThisObject;
    PHTTP_TMH_INTERFACE             pTmhClientIf = (PHTTP_TMH_INTERFACE       )pMyObject->hTmhClientIf;
    PHTTP_TMH_INTERFACE             pTmhServerIf = (PHTTP_TMH_INTERFACE       )pMyObject->hTmhServerIf;

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
        pTmhClientIf->Notify        = HttpPsoTmhClientNotify;
        pTmhClientIf->Serialize     = HttpPsoTmhClientSerialize;
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
        pTmhServerIf->Notify        = HttpPsoTmhServerNotify;
        pTmhServerIf->Serialize     = HttpPsoTmhServerSerialize;
    }

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpPsoInitialize
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
HttpPsoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_PROXY_SESSION_OBJECT      pMyObject    = (PHTTP_PROXY_SESSION_OBJECT)hThisObject;

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
    pMyObject->Oid                = HTTP_PROXY_SESSION_OID;
    pMyObject->Create             = HttpPsoCreate;
    pMyObject->Remove             = HttpPsoRemove;
    pMyObject->EnrollObjects      = HttpPsoEnrollObjects;
    pMyObject->Initialize         = HttpPsoInitialize;

    pMyObject->ServerAddr.Value   = 0;
    pMyObject->ServerPort         = HTTP_SERVER_PORT;

    pMyObject->hWamIf             = (ANSC_HANDLE)NULL;
    pMyObject->hSbcIf             = (ANSC_HANDLE)NULL;
    pMyObject->hCbcIf             = (ANSC_HANDLE)NULL;
    pMyObject->hPbcIf             = (ANSC_HANDLE)NULL;
    pMyObject->hHfpIf             = (ANSC_HANDLE)NULL;
    pMyObject->hClientSocket      = (ANSC_HANDLE)NULL;
    pMyObject->hServerSocket      = (ANSC_HANDLE)NULL;
    pMyObject->hSbcContext        = (ANSC_HANDLE)NULL;
    pMyObject->hCbcContext        = (ANSC_HANDLE)NULL;
    pMyObject->HashIndex          = 0;
    pMyObject->SbcPmode           = HTTP_SBC_PMODE_RELAY1;
    pMyObject->CbcPmode           = HTTP_CBC_PMODE_RELAY1;
    pMyObject->SessionState       = HTTP_PSO_STATE_INITIALIZED;
    pMyObject->bEstablished       = FALSE;

    pMyObject->GetWamIf           = HttpPsoGetWamIf;
    pMyObject->SetWamIf           = HttpPsoSetWamIf;
    pMyObject->GetSbcIf           = HttpPsoGetSbcIf;
    pMyObject->SetSbcIf           = HttpPsoSetSbcIf;
    pMyObject->GetCbcIf           = HttpPsoGetCbcIf;
    pMyObject->SetCbcIf           = HttpPsoSetCbcIf;
    pMyObject->GetPbcIf           = HttpPsoGetPbcIf;
    pMyObject->SetPbcIf           = HttpPsoSetPbcIf;
    pMyObject->GetHfpIf           = HttpPsoGetHfpIf;
    pMyObject->SetHfpIf           = HttpPsoSetHfpIf;

    pMyObject->GetClientSocket    = HttpPsoGetClientSocket;
    pMyObject->SetClientSocket    = HttpPsoSetClientSocket;
    pMyObject->GetServerSocket    = HttpPsoGetServerSocket;
    pMyObject->SetServerSocket    = HttpPsoSetServerSocket;
    pMyObject->GetSbcContext      = HttpPsoGetSbcContext;
    pMyObject->SetSbcContext      = HttpPsoSetSbcContext;
    pMyObject->GetCbcContext      = HttpPsoGetCbcContext;
    pMyObject->SetCbcContext      = HttpPsoSetCbcContext;

    pMyObject->GetServerName      = HttpPsoGetServerName;
    pMyObject->SetServerName      = HttpPsoSetServerName;
    pMyObject->GetClientAddr      = HttpPsoGetClientAddr;
    pMyObject->GetServerAddr      = HttpPsoGetServerAddr;
    pMyObject->GetClientPort      = HttpPsoGetClientPort;
    pMyObject->GetServerPort      = HttpPsoGetServerPort;

    pMyObject->GetSbcPmode        = HttpPsoGetSbcPmode;
    pMyObject->SetSbcPmode        = HttpPsoSetSbcPmode;
    pMyObject->GetCbcPmode        = HttpPsoGetCbcPmode;
    pMyObject->SetCbcPmode        = HttpPsoSetCbcPmode;
    pMyObject->GetSessionState    = HttpPsoGetSessionState;
    pMyObject->SetSessionState    = HttpPsoSetSessionState;

    pMyObject->Return             = HttpPsoReturn;
    pMyObject->Reset              = HttpPsoReset;

    pMyObject->Open               = HttpPsoOpen;
    pMyObject->Close              = HttpPsoClose;

    pMyObject->AcquireAccess      = HttpPsoAcquireAccess;
    pMyObject->ReleaseAccess      = HttpPsoReleaseAccess;

    pMyObject->GetLastBmoReq      = HttpPsoGetLastBmoReq;
    pMyObject->GetLastBmoRep      = HttpPsoGetLastBmoRep;
    pMyObject->GetCurBmoReq       = HttpPsoGetCurBmoReq;
    pMyObject->GetCurBmoRep       = HttpPsoGetCurBmoRep;
    pMyObject->GetBmoReqByTid     = HttpPsoGetBmoReqByTid;
    pMyObject->GetBmoRepByTid     = HttpPsoGetBmoRepByTid;
    pMyObject->AddNewBmoReq       = HttpPsoAddNewBmoReq;
    pMyObject->AddNewBmoRep       = HttpPsoAddNewBmoRep;
    pMyObject->DelAllBmoReqs      = HttpPsoDelAllBmoReqs;
    pMyObject->DelAllBmoReps      = HttpPsoDelAllBmoReps;

    pMyObject->GetClientBuffer    = HttpPsoGetClientBuffer;
    pMyObject->QueryForClient     = HttpPsoQueryForClient;
    pMyObject->RecvFromClient     = HttpPsoRecvFromClient;
    pMyObject->FinishedByClient   = HttpPsoFinishedByClient;
    pMyObject->SendToClient       = HttpPsoSendToClient;
    pMyObject->AcceptClient       = HttpPsoAcceptClient;
    pMyObject->Authenticate       = HttpPsoAuthenticate;

    pMyObject->GetServerBuffer    = HttpPsoGetServerBuffer;
    pMyObject->QueryForServer     = HttpPsoQueryForServer;
    pMyObject->RecvFromServer     = HttpPsoRecvFromServer;
    pMyObject->FinishedByServer   = HttpPsoFinishedByServer;
    pMyObject->SendToServer       = HttpPsoSendToServer;
    pMyObject->ConnectToServer    = HttpPsoConnectToServer;

    pMyObject->TmhClientNotify    = HttpPsoTmhClientNotify;
    pMyObject->TmhClientSerialize = HttpPsoTmhClientSerialize;
    pMyObject->TmhServerNotify    = HttpPsoTmhServerNotify;
    pMyObject->TmhServerSerialize = HttpPsoTmhServerSerialize;

    AnscInitializeLock       (&pMyObject->AccessLock     );
    AnscSListInitializeHeader(&pMyObject->BmoReqSList    );
    AnscInitializeLock       (&pMyObject->BmoReqSListLock);
    AnscSListInitializeHeader(&pMyObject->BmoRepSList    );
    AnscInitializeLock       (&pMyObject->BmoRepSListLock);

    return  ANSC_STATUS_SUCCESS;
}
