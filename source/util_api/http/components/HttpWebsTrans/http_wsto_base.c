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

    module:	http_wsto_base.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Http Webs Trans Object.

        *   HttpWstoCreate
        *   HttpWstoRemove
        *   HttpWstoEnrollObjects
        *   HttpWstoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/08/02    initial revision.

**********************************************************************/


#include "http_wsto_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        HttpWstoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Http Webs Trans Object and
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
HttpWstoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PHTTP_WEBS_TRANS_OBJECT         pMyObject    = NULL;
    errno_t                         rc           = -1;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PHTTP_WEBS_TRANS_OBJECT)AnscAllocateMemory(sizeof(HTTP_WEBS_TRANS_OBJECT));

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
    rc = strcpy_s(pBaseObject->Name, sizeof(pBaseObject->Name), HTTP_WEBS_TRANS_NAME);
    ERR_CHK(rc);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = HTTP_WEBS_TRANS_OID;
    pBaseObject->Create            = HttpWstoCreate;
    pBaseObject->Remove            = HttpWstoRemove;
    pBaseObject->EnrollObjects     = HttpWstoEnrollObjects;
    pBaseObject->Initialize        = HttpWstoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWstoRemove
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function deswstoys the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWstoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBS_TRANS_OBJECT         pMyObject    = (PHTTP_WEBS_TRANS_OBJECT)hThisObject;
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
        HttpWstoEnrollObjects
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
HttpWstoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBS_TRANS_OBJECT         pMyObject    = (PHTTP_WEBS_TRANS_OBJECT)hThisObject;
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
        pTmhIf->Notify        = HttpWstoTmhNotify;
        pTmhIf->Serialize     = HttpWstoTmhSerialize;
    }

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWstoInitialize
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
HttpWstoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBS_TRANS_OBJECT         pMyObject    = (PHTTP_WEBS_TRANS_OBJECT)hThisObject;

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
    pMyObject->Oid            = HTTP_WEBS_TRANS_OID;
    pMyObject->Create         = HttpWstoCreate;
    pMyObject->Remove         = HttpWstoRemove;
    pMyObject->EnrollObjects  = HttpWstoEnrollObjects;
    pMyObject->Initialize     = HttpWstoInitialize;

    pMyObject->hWspIf         = (ANSC_HANDLE)NULL;
    pMyObject->hHfpIf         = (ANSC_HANDLE)NULL;
    pMyObject->hWebsSession   = (ANSC_HANDLE)NULL;
    pMyObject->hBmoReq        = (ANSC_HANDLE)NULL;
    pMyObject->hBmoRep        = (ANSC_HANDLE)NULL;
    pMyObject->hWebSocket     = (ANSC_HANDLE)NULL;
    pMyObject->TransState     = HTTP_WSTO_STATE_INITIALIZED;

    pMyObject->GetWspIf       = HttpWstoGetWspIf;
    pMyObject->SetWspIf       = HttpWstoSetWspIf;
    pMyObject->GetHfpIf       = HttpWstoGetHfpIf;
    pMyObject->SetHfpIf       = HttpWstoSetHfpIf;

    pMyObject->GetWebsSession = HttpWstoGetWebsSession;
    pMyObject->SetWebsSession = HttpWstoSetWebsSession;
    pMyObject->GetBmoReq      = HttpWstoGetBmoReq;
    pMyObject->SetBmoReq      = HttpWstoSetBmoReq;
    pMyObject->GetBmoRep      = HttpWstoGetBmoRep;
    pMyObject->SetBmoRep      = HttpWstoSetBmoRep;
    pMyObject->GetWebSocket   = HttpWstoGetWebSocket;
    pMyObject->SetWebSocket   = HttpWstoSetWebSocket;

    pMyObject->GetClientAddr  = HttpWstoGetClientAddr;
    pMyObject->GetClientPort  = HttpWstoGetClientPort;
    pMyObject->GetTransState  = HttpWstoGetTransState;
    pMyObject->SetTransState  = HttpWstoSetTransState;

    pMyObject->Return         = HttpWstoReturn;
    pMyObject->Reset          = HttpWstoReset;

    pMyObject->Open           = HttpWstoOpen;
    pMyObject->Close          = HttpWstoClose;

    pMyObject->AcquireAccess  = HttpWstoAcquireAccess;
    pMyObject->ReleaseAccess  = HttpWstoReleaseAccess;

    pMyObject->Query          = HttpWstoQuery;
    pMyObject->Recv           = HttpWstoRecv;
    pMyObject->Send           = HttpWstoSend;
    pMyObject->Finish         = HttpWstoFinish;

    pMyObject->TmhNotify      = HttpWstoTmhNotify;
    pMyObject->TmhSerialize   = HttpWstoTmhSerialize;

    AnscInitializeLock(&pMyObject->AccessLock);

    return  ANSC_STATUS_SUCCESS;
}
