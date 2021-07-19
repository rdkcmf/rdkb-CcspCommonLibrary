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

    module:	web_vho_base.c

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Web Virtual Host Object.

        *   WebVhoCreate
        *   WebVhoRemove
        *   WebVhoEnrollObjects
        *   WebVhoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/11/02    initial revision.

**********************************************************************/


#include "web_vho_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        WebVhoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Web Virtual Host Object and
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
WebVhoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PWEB_VIRTUAL_HOST_OBJECT        pMyObject    = NULL;
    errno_t                         rc           = -1;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PWEB_VIRTUAL_HOST_OBJECT)AnscAllocateMemory(sizeof(WEB_VIRTUAL_HOST_OBJECT));

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
    rc = strcpy_s(pBaseObject->Name, sizeof(pBaseObject->Name), WEB_VIRTUAL_HOST_NAME);
    ERR_CHK(rc);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = WEB_VIRTUAL_HOST_OID;
    pBaseObject->Create            = WebVhoCreate;
    pBaseObject->Remove            = WebVhoRemove;
    pBaseObject->EnrollObjects     = WebVhoEnrollObjects;
    pBaseObject->Initialize        = WebVhoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebVhoRemove
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
WebVhoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_VIRTUAL_HOST_OBJECT        pMyObject         = (PWEB_VIRTUAL_HOST_OBJECT     )hThisObject;
    PWEB_RESOURCE_LOCATOR_OBJECT    pResourceLocator  = (PWEB_RESOURCE_LOCATOR_OBJECT )pMyObject->hResourceLocator;
    PWEB_LSM_INTERFACE              pLsmIf            = (PWEB_LSM_INTERFACE           )pMyObject->hLsmIf;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pSessionTdo       = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hSessionTdo;
    PANSC_TDO_CLIENT_OBJECT         pSessionTdoClient = (PANSC_TDO_CLIENT_OBJECT      )pMyObject->hSessionTdoClient;

    pMyObject->Cancel((ANSC_HANDLE)pMyObject);
    pMyObject->Reset ((ANSC_HANDLE)pMyObject);

    if ( pResourceLocator )
    {
        pResourceLocator->Remove((ANSC_HANDLE)pResourceLocator);

        pMyObject->hResourceLocator = (ANSC_HANDLE)NULL;
    }

    if ( pLsmIf )
    {
        AnscFreeMemory(pLsmIf);

        pMyObject->hLsmIf = (ANSC_HANDLE)NULL;
    }

    if ( pSessionTdo )
    {
        pSessionTdo->Remove((ANSC_HANDLE)pSessionTdo);
    }

    if ( pSessionTdoClient )
    {
        AnscFreeMemory(pSessionTdoClient);
    }

    AnscFreeLock(&pMyObject->SyncLock    );
    AnscFreeLock(&pMyObject->GsoTableLock);
    AnscCoRemove((ANSC_HANDLE)pMyObject  );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebVhoEnrollObjects
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
WebVhoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_VIRTUAL_HOST_OBJECT        pMyObject         = (PWEB_VIRTUAL_HOST_OBJECT     )hThisObject;
    PWEB_RESOURCE_LOCATOR_OBJECT    pResourceLocator  = (PWEB_RESOURCE_LOCATOR_OBJECT )pMyObject->hResourceLocator;
    PWEB_LSM_INTERFACE              pLsmIf            = (PWEB_LSM_INTERFACE           )pMyObject->hLsmIf;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pSessionTdo       = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hSessionTdo;
    PANSC_TDO_CLIENT_OBJECT         pSessionTdoClient = (PANSC_TDO_CLIENT_OBJECT      )pMyObject->hSessionTdoClient;
    errno_t                         rc                = -1;

    if ( !pResourceLocator )
    {
        pResourceLocator =
            (PWEB_RESOURCE_LOCATOR_OBJECT)WebCreateResourceLocator
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pResourceLocator )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hResourceLocator = (ANSC_HANDLE)pResourceLocator;
        }
    }

    if ( !pLsmIf )
    {
        pLsmIf = (PWEB_LSM_INTERFACE)AnscAllocateMemory(sizeof(WEB_LSM_INTERFACE));

        if ( !pLsmIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hLsmIf = (ANSC_HANDLE)pLsmIf;
        }

        rc = strcpy_s(pLsmIf->Name, sizeof(pLsmIf->Name), WEB_LSM_INTERFACE_NAME);
        ERR_CHK(rc);

        pLsmIf->InterfaceId    = WEB_LSM_INTERFACE_ID;
        pLsmIf->hOwnerContext  = (ANSC_HANDLE)pMyObject;
        pLsmIf->Size           = sizeof(WEB_LSM_INTERFACE);

        pLsmIf->NewContact     = WebVhoLsmNewContact;
        pLsmIf->ClassifyClient = WebVhoLsmClassifyClient;
        pLsmIf->ExpireSession  = WebVhoLsmExpireSession;
        pLsmIf->EndSession     = WebVhoLsmEndSession;
    }

    if ( !pSessionTdo )
    {
        pSessionTdo =
            (PANSC_TIMER_DESCRIPTOR_OBJECT)AnscCreateTimerDescriptor
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pSessionTdo )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hSessionTdo = (ANSC_HANDLE)pSessionTdo;
        }

        pSessionTdo->SetTimerType((ANSC_HANDLE)pSessionTdo, ANSC_TIMER_TYPE_PERIODIC     );
        pSessionTdo->SetInterval ((ANSC_HANDLE)pSessionTdo, WEB_VHO_SESSION_TIME_INTERVAL);
    }

    if ( !pSessionTdoClient )
    {
        pSessionTdoClient = (PANSC_TDO_CLIENT_OBJECT)AnscAllocateMemory(sizeof(ANSC_TDO_CLIENT_OBJECT));

        if ( !pSessionTdoClient )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hSessionTdoClient = (ANSC_HANDLE)pSessionTdoClient;
        }

        pSessionTdoClient->hClientContext = (ANSC_HANDLE)pMyObject;
        pSessionTdoClient->Invoke         = WebVhoSessionTdoInvoke;

        pSessionTdo->SetClient((ANSC_HANDLE)pSessionTdo, (ANSC_HANDLE)pSessionTdoClient);
    }

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebVhoInitialize
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
WebVhoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_VIRTUAL_HOST_OBJECT        pMyObject    = (PWEB_VIRTUAL_HOST_OBJECT)hThisObject;
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
    pMyObject->Oid               = WEB_VIRTUAL_HOST_OID;
    pMyObject->Create            = WebVhoCreate;
    pMyObject->Remove            = WebVhoRemove;
    pMyObject->EnrollObjects     = WebVhoEnrollObjects;
    pMyObject->Initialize        = WebVhoInitialize;

    pMyObject->hHfpIf            = (ANSC_HANDLE)NULL;
    pMyObject->bActive           = FALSE;

    pMyObject->GetHostName       = WebVhoGetHostName;
    pMyObject->SetHostName       = WebVhoSetHostName;
    pMyObject->GetHostPort       = WebVhoGetHostPort;
    pMyObject->SetHostPort       = WebVhoSetHostPort;
    pMyObject->GetHfpIf          = WebVhoGetHfpIf;
    pMyObject->SetHfpIf          = WebVhoSetHfpIf;
    pMyObject->GetLsmIf          = WebVhoGetLsmIf;
    pMyObject->SetLsmIf          = WebVhoSetLsmIf;
    pMyObject->IsDefaultHost     = WebVhoIsDefaultHost;

    pMyObject->GetProperty       = WebVhoGetProperty;
    pMyObject->SetProperty       = WebVhoSetProperty;
    pMyObject->ResetProperty     = WebVhoResetProperty;
    pMyObject->Reset             = WebVhoReset;

    pMyObject->SetHttpAuth       = WebVhoSetHttpAuth;
    pMyObject->SetHttpAuth2      = WebVhoSetHttpAuth2;

    pMyObject->AcquireAccess     = WebVhoAcquireAccess;
    pMyObject->ReleaseAccess     = WebVhoReleaseAccess;
    pMyObject->Engage            = WebVhoEngage;
    pMyObject->Cancel            = WebVhoCancel;
    pMyObject->MatchName         = WebVhoMatchName;
    pMyObject->SessionTdoInvoke  = WebVhoSessionTdoInvoke;

    pMyObject->AttachRoo         = WebVhoAttachRoo;
    pMyObject->DetachRoo         = WebVhoDetachRoo;
    pMyObject->DeleteRoo         = WebVhoDeleteRoo;

    pMyObject->IdentifyGso       = WebVhoIdentifyGso;
    pMyObject->GetGso            = WebVhoGetGso;
    pMyObject->AddGso            = WebVhoAddGso;
    pMyObject->DelGso            = WebVhoDelGso;
    pMyObject->DelAllGsos        = WebVhoDelAllGsos;

    pMyObject->Query             = WebVhoQuery;
    pMyObject->Process           = WebVhoProcess;
    pMyObject->Notify            = WebVhoNotify;
    pMyObject->ConstructResponse = WebVhoConstructResponse;
    pMyObject->AddHttpRepHeaders = WebVhoAddHttpRepHeaders;
    pMyObject->GetResourcePath   = WebVhoGetResourcePath;

    pMyObject->LsmNewContact     = WebVhoLsmNewContact;
    pMyObject->LsmClassifyClient = WebVhoLsmClassifyClient;
    pMyObject->LsmExpireSession  = WebVhoLsmExpireSession;
    pMyObject->LsmEndSession     = WebVhoLsmEndSession;

    AnscInitializeLock(&pMyObject->SyncLock);

    for ( i = 0; i < WEB_VHO_GSO_TABLE_SIZE; i++ )
    {
        AnscSListInitializeHeader(&pMyObject->GsoTable[i]);
    }
    AnscInitializeLock(&pMyObject->GsoTableLock);

    /*
     * We shall initialize the configuration properties to the default values, which may be changed
     * later via the set_property() member function. Note that this call may not guarantee a valid
     * and legtimate configuration.
     */
    pMyObject->ResetProperty((ANSC_HANDLE)pMyObject);

    if ( TRUE )
    {
        ULONG                       ulRandValue = AnscGetTickInMilliSeconds() * 1103515245 + 12345;

        /* Randomly pick up the starting global session id to be hard predictable */
        pMyObject->GlobalSidCount = (ulRandValue % 0x7FFFFFFF);
    }

    return  ANSC_STATUS_SUCCESS;
}
