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

    module:	http_wcto_base.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Http Webc Trans Object.

        *   HttpWctoCreate
        *   HttpWctoRemove
        *   HttpWctoEnrollObjects
        *   HttpWctoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        04/22/02    initial revision.

**********************************************************************/


#include "http_wcto_global.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        HttpWctoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Http Webc Trans Object and
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
HttpWctoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PHTTP_WEBC_TRANS_OBJECT         pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PHTTP_WEBC_TRANS_OBJECT)AnscAllocateMemory(sizeof(HTTP_WEBC_TRANS_OBJECT));

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
    /* AnscCopyString(pBaseObject->Name, HTTP_WEBC_TRANS_NAME); */

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = HTTP_WEBC_TRANS_OID;
    pBaseObject->Create            = HttpWctoCreate;
    pBaseObject->Remove            = HttpWctoRemove;
    pBaseObject->EnrollObjects     = HttpWctoEnrollObjects;
    pBaseObject->Initialize        = HttpWctoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWctoRemove
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function deswctoys the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWctoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBC_TRANS_OBJECT         pMyObject    = (PHTTP_WEBC_TRANS_OBJECT)hThisObject;
    PHTTP_TMH_INTERFACE             pTmhIf       = (PHTTP_TMH_INTERFACE    )pMyObject->hTmhIf;

    pMyObject->Reset((ANSC_HANDLE)pMyObject);

    if ( pTmhIf )
    {
        AnscFreeMemory(pTmhIf);
    }

    AnscFreeLock(&pMyObject->AccessLock);
    AnscCoRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWctoEnrollObjects
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
HttpWctoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBC_TRANS_OBJECT         pMyObject    = (PHTTP_WEBC_TRANS_OBJECT)hThisObject;
    PHTTP_TMH_INTERFACE             pTmhIf       = (PHTTP_TMH_INTERFACE      )pMyObject->hTmhIf;

    if ( !pTmhIf )
    {
        pTmhIf = (PHTTP_TMH_INTERFACE)AnscAllocateMemory(sizeof(HTTP_TMH_INTERFACE));

        if ( !pTmhIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hTmhIf = (ANSC_HANDLE)pTmhIf;
        }

        pTmhIf->hOwnerContext = (ANSC_HANDLE)pMyObject;
        pTmhIf->Size          = sizeof(HTTP_TMH_INTERFACE);
        pTmhIf->Notify        = HttpWctoTmhNotify;
        pTmhIf->Serialize     = HttpWctoTmhSerialize;
    }

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWctoInitialize
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
HttpWctoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBC_TRANS_OBJECT         pMyObject    = (PHTTP_WEBC_TRANS_OBJECT)hThisObject;

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
    pMyObject->Oid                = HTTP_WEBC_TRANS_OID;
    pMyObject->Create             = HttpWctoCreate;
    pMyObject->Remove             = HttpWctoRemove;
    pMyObject->EnrollObjects      = HttpWctoEnrollObjects;
    pMyObject->Initialize         = HttpWctoInitialize;

    pMyObject->hBspIf             = (ANSC_HANDLE)NULL;
    pMyObject->hHfpIf             = (ANSC_HANDLE)NULL;
    pMyObject->hWebcSession       = (ANSC_HANDLE)NULL;
    pMyObject->hBmoReq            = (ANSC_HANDLE)NULL;
    pMyObject->hBmoRep            = (ANSC_HANDLE)NULL;
    pMyObject->hBspReqContext     = (ANSC_HANDLE)NULL;
    pMyObject->hTcpSimpleClient   = (ANSC_HANDLE)NULL;
    pMyObject->TransState         = HTTP_WCTO_STATE_INITIALIZED;
    pMyObject->bLastTrans         = TRUE;
    pMyObject->AuthRetries        = 0;
    pMyObject->MaxAuthRetries     = HTTP_WCTO_DEFAULT_MAX_AUTH_RETRIES;

    pMyObject->GetBspIf           = HttpWctoGetBspIf;
    pMyObject->SetBspIf           = HttpWctoSetBspIf;
    pMyObject->GetHfpIf           = HttpWctoGetHfpIf;
    pMyObject->SetHfpIf           = HttpWctoSetHfpIf;

    pMyObject->GetWebcSession     = HttpWctoGetWebcSession;
    pMyObject->SetWebcSession     = HttpWctoSetWebcSession;
    pMyObject->GetBmoReq          = HttpWctoGetBmoReq;
    pMyObject->SetBmoReq          = HttpWctoSetBmoReq;
    pMyObject->GetBmoRep          = HttpWctoGetBmoRep;
    pMyObject->SetBmoRep          = HttpWctoSetBmoRep;
    pMyObject->GetTcpSimpleClient = HttpWctoGetTcpSimpleClient;
    pMyObject->SetTcpSimpleClient = HttpWctoSetTcpSimpleClient;

    pMyObject->GetTransState      = HttpWctoGetTransState;
    pMyObject->SetTransState      = HttpWctoSetTransState;
    pMyObject->Return             = HttpWctoReturn;
    pMyObject->Reset              = HttpWctoReset;

    pMyObject->IsLastTrans        = HttpWctoIsLastTrans;
    pMyObject->DoLastTrans        = HttpWctoDoLastTrans;
    pMyObject->Open               = HttpWctoOpen;
    pMyObject->Close              = HttpWctoClose;
    pMyObject->AcquireAccess      = HttpWctoAcquireAccess;
    pMyObject->ReleaseAccess      = HttpWctoReleaseAccess;

    pMyObject->Request            = HttpWctoRequest;
    pMyObject->DoOptions          = HttpWctoDoOptions;
    pMyObject->DoGet              = HttpWctoDoGet;
    pMyObject->DoHead             = HttpWctoDoHead;
    pMyObject->DoPost             = HttpWctoDoPost;
    pMyObject->DoPut              = HttpWctoDoPut;
    pMyObject->DoDelete           = HttpWctoDoDelete;
    pMyObject->DoTrace            = HttpWctoDoTrace;
    pMyObject->DoConnect          = HttpWctoDoConnect;

    pMyObject->Query              = HttpWctoQuery;
    pMyObject->Recv               = HttpWctoRecv;
    pMyObject->Send               = HttpWctoSend;
    pMyObject->Finish             = HttpWctoFinish;
    pMyObject->Abort              = HttpWctoAbort;

    pMyObject->TmhNotify          = HttpWctoTmhNotify;
    pMyObject->TmhSerialize       = HttpWctoTmhSerialize;

    AnscInitializeLock(&pMyObject->AccessLock);

    return  ANSC_STATUS_SUCCESS;
}
