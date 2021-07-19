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

    module:	http_wsso_base.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Http Webs Session Object.

        *   HttpWssoCreate
        *   HttpWssoRemove
        *   HttpWssoEnrollObjects
        *   HttpWssoInitialize

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


#include "http_wsso_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        HttpWssoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Http Webs Session Object and
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
HttpWssoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PHTTP_WEBS_SESSION_OBJECT       pMyObject    = NULL;
    errno_t                         rc           = -1;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PHTTP_WEBS_SESSION_OBJECT)AnscAllocateMemory(sizeof(HTTP_WEBS_SESSION_OBJECT));

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
    rc = strcpy_s(pBaseObject->Name, sizeof(pBaseObject->Name), HTTP_WEBS_SESSION_NAME);
    ERR_CHK(rc);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = HTTP_WEBS_SESSION_OID;
    pBaseObject->Create            = HttpWssoCreate;
    pBaseObject->Remove            = HttpWssoRemove;
    pBaseObject->EnrollObjects     = HttpWssoEnrollObjects;
    pBaseObject->Initialize        = HttpWssoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWssoRemove
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
HttpWssoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBS_SESSION_OBJECT       pMyObject    = (PHTTP_WEBS_SESSION_OBJECT)hThisObject;

    pMyObject->Reset((ANSC_HANDLE)pMyObject);

    AnscFreeEvent(&pMyObject->ServeEvent   );
    AnscFreeLock (&pMyObject->AccessLock   );
    AnscFreeLock (&pMyObject->WstoSListLock);
    AnscCoRemove ((ANSC_HANDLE)pMyObject   );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWssoEnrollObjects
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
HttpWssoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBS_SESSION_OBJECT       pMyObject    = (PHTTP_WEBS_SESSION_OBJECT)hThisObject;

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWssoInitialize
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
HttpWssoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBS_SESSION_OBJECT       pMyObject    = (PHTTP_WEBS_SESSION_OBJECT)hThisObject;

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
    pMyObject->Oid             = HTTP_WEBS_SESSION_OID;
    pMyObject->Create          = HttpWssoCreate;
    pMyObject->Remove          = HttpWssoRemove;
    pMyObject->EnrollObjects   = HttpWssoEnrollObjects;
    pMyObject->Initialize      = HttpWssoInitialize;

    pMyObject->hWspIf          = (ANSC_HANDLE)NULL;
    pMyObject->hHfpIf          = (ANSC_HANDLE)NULL;
    pMyObject->hWebSocket      = (ANSC_HANDLE)NULL;
    pMyObject->SessionState    = HTTP_WSSO_STATE_INITIALIZED;
    pMyObject->ServeCount      = 0;

    pMyObject->GetWspIf        = HttpWssoGetWspIf;
    pMyObject->SetWspIf        = HttpWssoSetWspIf;
    pMyObject->GetHfpIf        = HttpWssoGetHfpIf;
    pMyObject->SetHfpIf        = HttpWssoSetHfpIf;

    pMyObject->GetWebSocket    = HttpWssoGetWebSocket;
    pMyObject->SetWebSocket    = HttpWssoSetWebSocket;
    pMyObject->GetSessionState = HttpWssoGetSessionState;
    pMyObject->SetSessionState = HttpWssoSetSessionState;

    pMyObject->Return          = HttpWssoReturn;
    pMyObject->Reset           = HttpWssoReset;

    pMyObject->Open            = HttpWssoOpen;
    pMyObject->Close           = HttpWssoClose;

    pMyObject->AcquireAccess   = HttpWssoAcquireAccess;
    pMyObject->ReleaseAccess   = HttpWssoReleaseAccess;
    pMyObject->EnterWspServe   = HttpWssoEnterWspServe;
    pMyObject->LeaveWspServe   = HttpWssoLeaveWspServe;
    pMyObject->ClearWspServe   = HttpWssoClearWspServe;
    pMyObject->CloseConnection = HttpWssoCloseConnection;

    pMyObject->GetEndWsto      = HttpWssoGetEndWsto;
    pMyObject->GetCurWsto      = HttpWssoGetCurWsto;
    pMyObject->AddNewWsto      = HttpWssoAddNewWsto;
    pMyObject->DelAllWstos     = HttpWssoDelAllWstos;

    pMyObject->Query           = HttpWssoQuery;
    pMyObject->Recv            = HttpWssoRecv;
    pMyObject->Finish          = HttpWssoFinish;
    pMyObject->Accept          = HttpWssoAccept;

    AnscInitializeEvent      (&pMyObject->ServeEvent   );
    AnscResetEvent           (&pMyObject->ServeEvent   );

    AnscInitializeLock       (&pMyObject->AccessLock   );
    AnscSListInitializeHeader(&pMyObject->WstoSList    );
    AnscInitializeLock       (&pMyObject->WstoSListLock);

    return  ANSC_STATUS_SUCCESS;
}
