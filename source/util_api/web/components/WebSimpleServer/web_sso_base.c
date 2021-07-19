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

    module:	web_sso_base.c

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Web Simple Server Object.

        *   WebSsoCreate
        *   WebSsoRemove
        *   WebSsoEnrollObjects
        *   WebSsoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/10/02    initial revision.

**********************************************************************/


#include "web_sso_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        WebSsoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Web Simple Server Object and
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
WebSsoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PWEB_SIMPLE_SERVER_OBJECT       pMyObject    = NULL;
    errno_t                         rc           = -1;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PWEB_SIMPLE_SERVER_OBJECT)AnscAllocateMemory(sizeof(WEB_SIMPLE_SERVER_OBJECT));

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
    rc = strcpy_s(pBaseObject->Name, sizeof(pBaseObject->Name), WEB_SIMPLE_SERVER_NAME);
    ERR_CHK(rc);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = WEB_SIMPLE_SERVER_OID;
    pBaseObject->Create            = WebSsoCreate;
    pBaseObject->Remove            = WebSsoRemove;
    pBaseObject->EnrollObjects     = WebSsoEnrollObjects;
    pBaseObject->Initialize        = WebSsoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebSsoRemove
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
WebSsoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_SIMPLE_SERVER_OBJECT       pMyObject       = (PWEB_SIMPLE_SERVER_OBJECT)hThisObject;
    PWEB_SITE_MANAGER_OBJECT        pWebSiteManager = (PWEB_SITE_MANAGER_OBJECT )pMyObject->hWebSiteManager;
    PHTTP_HFP_INTERFACE             pHfpIf          = (PHTTP_HFP_INTERFACE      )pMyObject->hHfpIf;
    PWEB_CSP_INTERFACE              pCspIf          = (PWEB_CSP_INTERFACE       )pMyObject->hCspIf;
    PHTTP_FUM_INTERFACE             pFumIf          = (PHTTP_FUM_INTERFACE      )pMyObject->hFumIf;
    PHTTP_FUM_ENTITY                pEntity         = NULL;
    PSINGLE_LINK_ENTRY              pEntry          = NULL;

    pEntry = AnscSListPopEntry(&pMyObject->MdhSList);
    while ( pEntry )
    {
        pEntity = ACCESS_HTTP_FUM_ENTITY(pEntry);
        pEntry  = AnscSListPopEntry(&pMyObject->MdhSList);

        HttpFumEntityRemove(pEntity);
    }

    pMyObject->Cancel((ANSC_HANDLE)pMyObject);
    pMyObject->Reset ((ANSC_HANDLE)pMyObject);

    if ( pWebSiteManager )
    {
        pWebSiteManager->Remove((ANSC_HANDLE)pWebSiteManager);

        pMyObject->hWebSiteManager = (ANSC_HANDLE)NULL;
    }

    if ( pHfpIf )
    {
        AnscFreeMemory(pHfpIf);
    }

    if ( pCspIf )
    {
        AnscFreeMemory(pCspIf);
    }

    if ( pFumIf )
    {
        AnscFreeMemory(pFumIf);
    }

    AnscFreeLock(&pMyObject->MdhSListLock);

    AnscCoRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebSsoEnrollObjects
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
WebSsoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_SIMPLE_SERVER_OBJECT       pMyObject       = (PWEB_SIMPLE_SERVER_OBJECT)hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf          = (PHTTP_HFP_INTERFACE      )pMyObject->hHfpIf;
    PWEB_CSP_INTERFACE              pCspIf          = (PWEB_CSP_INTERFACE       )pMyObject->hCspIf;
    PHTTP_FUM_INTERFACE             pFumIf          = (PHTTP_FUM_INTERFACE      )pMyObject->hFumIf;
    errno_t                         rc              = -1;

    // Assume no support for WebSiteManager.
    /*if ( !pWebSiteManager )
    {
        pWebSiteManager =
            (PWEB_SITE_MANAGER_OBJECT)WebCreateSiteManager
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pWebSiteManager )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hWebSiteManager = (ANSC_HANDLE)pWebSiteManager;
        }
    }*/

    if ( !pHfpIf )
    {
        pHfpIf = (PHTTP_HFP_INTERFACE)AnscAllocateMemory(sizeof(HTTP_HFP_INTERFACE));

        if ( !pHfpIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hHfpIf = (ANSC_HANDLE)pHfpIf;
        }

        pHfpIf->hOwnerContext = (ANSC_HANDLE)NULL;
        pHfpIf->Size          = sizeof(HTTP_HFP_INTERFACE);
    }

    if ( !pCspIf )
    {
        pCspIf = (PWEB_CSP_INTERFACE)AnscAllocateMemory(sizeof(WEB_CSP_INTERFACE));

        if ( !pCspIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hCspIf = (ANSC_HANDLE)pCspIf;
        }

        rc = strcpy_s(pCspIf->Name, sizeof(pCspIf->Name), WEB_CSP_INTERFACE_NAME);
        ERR_CHK(rc);

        pCspIf->InterfaceId       = WEB_CSP_INTERFACE_ID;
        pCspIf->hOwnerContext     = (ANSC_HANDLE)pMyObject;
        pCspIf->Size              = sizeof(WEB_CSP_INTERFACE);

        pCspIf->GetCfgMode        = WebSsoCspGetCfgMode;
        pCspIf->IsReadOnly        = WebSsoCspIsReadOnly;
        pCspIf->IsSwitchOn        = WebSsoCspIsSwitchOn;
        pCspIf->Engage            = WebSsoCspEngage;
        pCspIf->Cancel            = WebSsoCspCancel;

        pCspIf->AddHttpDaemon     = WebSsoCspAddHttpDaemon;
        pCspIf->DelHttpDaemon     = WebSsoCspDelHttpDaemon;
        pCspIf->SetHttpServerMode = WebSsoCspSetHttpServerMode;
        pCspIf->SetPersistentConn = WebSsoCspSetPersistentConn;
        pCspIf->SetProductName    = WebSsoCspSetProductName;
    }

    if ( !pFumIf )
    {
        pFumIf = (PHTTP_FUM_INTERFACE)AnscAllocateMemory(sizeof(HTTP_FUM_INTERFACE));

        if ( !pFumIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hFumIf = (ANSC_HANDLE)pFumIf;
        }

        pFumIf->hOwnerContext = (ANSC_HANDLE)pMyObject;
        pFumIf->Size          = sizeof(HTTP_FUM_INTERFACE);
        pFumIf->RegMdhIf      = WebSsoRegisterMdhIf;
        pFumIf->GetMdhIf      = WebSsoGetMdhIf;
    }

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebSsoInitialize
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
WebSsoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_SIMPLE_SERVER_OBJECT       pMyObject    = (PWEB_SIMPLE_SERVER_OBJECT)hThisObject;
    ULONG                           i            = 0;

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
    pMyObject->Oid                  = WEB_SIMPLE_SERVER_OID;
    pMyObject->Create               = WebSsoCreate;
    pMyObject->Remove               = WebSsoRemove;
    pMyObject->EnrollObjects        = WebSsoEnrollObjects;
    pMyObject->Initialize           = WebSsoInitialize;

    pMyObject->ServerMode           = 0;
    pMyObject->bActive              = FALSE;

    pMyObject->GetSiteManager       = WebSsoGetSiteManager;
    pMyObject->GetHfpIf             = WebSsoGetHfpIf;
    pMyObject->SetHfpIf             = WebSsoSetHfpIf;
    pMyObject->GetFumIf             = WebSsoGetFumIf;
    pMyObject->GetCspIf             = WebSsoGetCspIf;
    pMyObject->GetServerMode        = WebSsoGetServerMode;
    pMyObject->SetServerMode        = WebSsoSetServerMode;
    pMyObject->GetProperty          = WebSsoGetProperty;
    pMyObject->SetProperty          = WebSsoSetProperty;
    pMyObject->ResetProperty        = WebSsoResetProperty;
    pMyObject->Reset                = WebSsoReset;

    pMyObject->Engage               = WebSsoEngage;
    pMyObject->Cancel               = WebSsoCancel;

    pMyObject->CspGetCfgMode        = WebSsoCspGetCfgMode;
    pMyObject->CspIsReadOnly        = WebSsoCspIsReadOnly;
    pMyObject->CspIsSwitchOn        = WebSsoCspIsSwitchOn;
    pMyObject->CspEngage            = WebSsoCspEngage;
    pMyObject->CspCancel            = WebSsoCspCancel;

    pMyObject->CspAddHttpDaemon     = WebSsoCspAddHttpDaemon;
    pMyObject->CspDelHttpDaemon     = WebSsoCspDelHttpDaemon;
    pMyObject->CspSetHttpServerMode = WebSsoCspSetHttpServerMode;
    pMyObject->CspSetPersistentConn = WebSsoCspSetPersistentConn;
    pMyObject->CspSetProductName    = WebSsoCspSetProductName;

    for ( i = 0; i < WEB_MAX_DAEMON_NUMBER; i++ )
    {
        pMyObject->hHttpSsoArray[i] = (ANSC_HANDLE)NULL;
    }

    /*
     * We shall initialize the configuration properties to the default values, which may be changed
     * later via the set_property() member function. Note that this call may not guarantee a valid
     * and legtimate configuration.
     */
    pMyObject->ResetProperty((ANSC_HANDLE)pMyObject);

    AnscSListInitializeHeader(&pMyObject->MdhSList);
    AnscInitializeLock       (&pMyObject->MdhSListLock);

    return  ANSC_STATUS_SUCCESS;
}
