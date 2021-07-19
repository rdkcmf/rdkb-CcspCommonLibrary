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

    module:	web_roo_base.c

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Web Resource Owner Object.

        *   WebRooCreate
        *   WebRooRemove
        *   WebRooEnrollObjects
        *   WebRooInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/13/02    initial revision.

**********************************************************************/


#include "web_roo_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        WebRooCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Web Resource Owner Object and
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
WebRooCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PWEB_RESOURCE_OWNER_OBJECT      pMyObject    = NULL;
    errno_t                         rc           = -1;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PWEB_RESOURCE_OWNER_OBJECT)AnscAllocateMemory(sizeof(WEB_RESOURCE_OWNER_OBJECT));

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
    rc = strcpy_s(pBaseObject->Name, sizeof(pBaseObject->Name), WEB_RESOURCE_OWNER_NAME);
    ERR_CHK(rc);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = WEB_RESOURCE_OWNER_OID;
    pBaseObject->Create            = WebRooCreate;
    pBaseObject->Remove            = WebRooRemove;
    pBaseObject->EnrollObjects     = WebRooEnrollObjects;
    pBaseObject->Initialize        = WebRooInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooRemove
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
WebRooRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_RESOURCE_OWNER_OBJECT      pMyObject    = (PWEB_RESOURCE_OWNER_OBJECT)hThisObject;
    PHTTP_RCP_INTERFACE             pRcpIf       = (PHTTP_RCP_INTERFACE       )pMyObject->hRcpIf;

    pMyObject->Cancel((ANSC_HANDLE)pMyObject);
    pMyObject->Reset ((ANSC_HANDLE)pMyObject);

    if ( pRcpIf )
    {
        AnscFreeMemory(pRcpIf);
    }

    AnscFreeLock(&pMyObject->SyncLock  );
    AnscCoRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooEnrollObjects
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
WebRooEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_RESOURCE_OWNER_OBJECT      pMyObject    = (PWEB_RESOURCE_OWNER_OBJECT)hThisObject;
    PHTTP_RCP_INTERFACE             pRcpIf       = (PHTTP_RCP_INTERFACE       )pMyObject->hRcpIf;

    if ( !pRcpIf )
    {
        pRcpIf = (PHTTP_RCP_INTERFACE)AnscAllocateMemory(sizeof(HTTP_RCP_INTERFACE));

        if ( !pRcpIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hRcpIf = (ANSC_HANDLE)pRcpIf;
        }

        pRcpIf->hOwnerContext     = (ANSC_HANDLE)pMyObject;
        pRcpIf->Size              = sizeof(HTTP_RCP_INTERFACE);

        pRcpIf->GetDocumentRoot   = WebRooRcpGetDocumentRoot;
        pRcpIf->GetPathInfo       = WebRooRcpGetPathInfo;
        pRcpIf->GetPathTranslated = WebRooRcpGetPathTranslated;
        pRcpIf->GetRemoteUser     = WebRooRcpGetRemoteUser;
        pRcpIf->GetScriptName     = WebRooRcpGetScriptName;
        pRcpIf->GetServerName     = WebRooRcpGetServerName;
        pRcpIf->GetServerPort     = WebRooRcpGetServerPort;
        pRcpIf->GetServerProtocol = WebRooRcpGetServerProtocol;
        pRcpIf->GetServerSoftware = WebRooRcpGetServerSoftware;
    }

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooInitialize
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
WebRooInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_RESOURCE_OWNER_OBJECT      pMyObject    = (PWEB_RESOURCE_OWNER_OBJECT)hThisObject;

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
    pMyObject->Oid                  = WEB_RESOURCE_OWNER_OID;
    pMyObject->Create               = WebRooCreate;
    pMyObject->Remove               = WebRooRemove;
    pMyObject->EnrollObjects        = WebRooEnrollObjects;
    pMyObject->Initialize           = WebRooInitialize;

    pMyObject->hAuthServer          = NULL;
    pMyObject->hHfpIf               = (ANSC_HANDLE)NULL;
    pMyObject->bActive              = FALSE;

    pMyObject->GetAuthServer        = WebRooGetAuthServer;
    pMyObject->GetHfpIf             = WebRooGetHfpIf;
    pMyObject->SetHfpIf             = WebRooSetHfpIf;
    pMyObject->GetRcpIf             = WebRooGetRcpIf;
    pMyObject->GetPathName          = WebRooGetPathName;
    pMyObject->SetPathName          = WebRooSetPathName;
    pMyObject->IsDefaultOwner       = WebRooIsDefaultOwner;
    pMyObject->GetProperty          = WebRooGetProperty;
    pMyObject->SetProperty          = WebRooSetProperty;
    pMyObject->ResetProperty        = WebRooResetProperty;
    pMyObject->Reset                = WebRooReset;

    pMyObject->AcquireAccess        = WebRooAcquireAccess;
    pMyObject->ReleaseAccess        = WebRooReleaseAccess;
    pMyObject->Engage               = WebRooEngage;
    pMyObject->Cancel               = WebRooCancel;

    pMyObject->Query                = WebRooQuery;
    pMyObject->Process              = WebRooProcess;

    pMyObject->DoOptions            = WebRooDoOptions;
    pMyObject->DoGet                = WebRooDoGet;
    pMyObject->DoHead               = WebRooDoHead;
    pMyObject->DoPost               = WebRooDoPost;
    pMyObject->DoPut                = WebRooDoPut;
    pMyObject->DoDelete             = WebRooDoDelete;
    pMyObject->DoTrace              = WebRooDoTrace;
    pMyObject->DoConnect            = WebRooDoConnect;

    pMyObject->DoNotify             = WebRooDoNotify;
    pMyObject->DoSearch             = WebRooDoSearch;
    pMyObject->DoMSearch            = WebRooDoMSearch;
    pMyObject->DoMPost              = WebRooDoMPost;
    pMyObject->DoSubscribe          = WebRooDoSubscribe;
    pMyObject->DoUnsubscribe        = WebRooDoUnsubscribe;

    pMyObject->ReplyReq             = WebRooReplyReq;
    pMyObject->Reply300             = WebRooReply300;
    pMyObject->Reply301             = WebRooReply301;
    pMyObject->Reply302             = WebRooReply302;
    pMyObject->Reply303             = WebRooReply303;
    pMyObject->Reply304             = WebRooReply304;
    pMyObject->Reply305             = WebRooReply305;
    pMyObject->Reply307             = WebRooReply307;
    pMyObject->Reply400             = WebRooReply400;
    pMyObject->Reply403             = WebRooReply403;
    pMyObject->Reply404             = WebRooReply404;
    pMyObject->Reply405             = WebRooReply405;
    pMyObject->Reply409             = WebRooReply409;
    pMyObject->Reply410             = WebRooReply410;
    pMyObject->Reply415             = WebRooReply415;
    pMyObject->Reply417             = WebRooReply417;
    pMyObject->Reply500             = WebRooReply500;
    pMyObject->Reply501             = WebRooReply501;
    pMyObject->Reply503             = WebRooReply503;
    pMyObject->Reply505             = WebRooReply505;

    pMyObject->RcpGetDocumentRoot   = WebRooRcpGetDocumentRoot;
    pMyObject->RcpGetPathInfo       = WebRooRcpGetPathInfo;
    pMyObject->RcpGetPathTranslated = WebRooRcpGetPathTranslated;
    pMyObject->RcpGetRemoteUser     = WebRooRcpGetRemoteUser;
    pMyObject->RcpGetScriptName     = WebRooRcpGetScriptName;
    pMyObject->RcpGetServerName     = WebRooRcpGetServerName;
    pMyObject->RcpGetServerPort     = WebRooRcpGetServerPort;
    pMyObject->RcpGetServerProtocol = WebRooRcpGetServerProtocol;
    pMyObject->RcpGetServerSoftware = WebRooRcpGetServerSoftware;

    AnscInitializeLock(&pMyObject->SyncLock);

    /*
     * We shall initialize the configuration properties to the default values, which may be changed
     * later via the set_property() member function. Note that this call may not guarantee a valid
     * and legtimate configuration.
     */
    pMyObject->ResetProperty((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
