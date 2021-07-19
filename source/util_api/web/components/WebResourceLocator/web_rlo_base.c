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

    module:	web_rlo_base.c

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Web Resource Locator Object.

        *   WebRloCreate
        *   WebRloRemove
        *   WebRloEnrollObjects
        *   WebRloInitialize

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


#include "web_rlo_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        WebRloCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Web Resource Locator Object and
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
WebRloCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PWEB_RESOURCE_LOCATOR_OBJECT    pMyObject    = NULL;
    errno_t                         rc           = -1;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PWEB_RESOURCE_LOCATOR_OBJECT)AnscAllocateMemory(sizeof(WEB_RESOURCE_LOCATOR_OBJECT));

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
    rc = strcpy_s(pBaseObject->Name, sizeof(pBaseObject->Name), WEB_RESOURCE_LOCATOR_NAME);
    ERR_CHK(rc);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = WEB_RESOURCE_LOCATOR_OID;
    pBaseObject->Create            = WebRloCreate;
    pBaseObject->Remove            = WebRloRemove;
    pBaseObject->EnrollObjects     = WebRloEnrollObjects;
    pBaseObject->Initialize        = WebRloInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRloRemove
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
WebRloRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_RESOURCE_LOCATOR_OBJECT    pMyObject    = (PWEB_RESOURCE_LOCATOR_OBJECT)hThisObject;
    PWEB_URI_PATH_OBJECT            pRootUriPath = (PWEB_URI_PATH_OBJECT        )pMyObject->hRootUriPath;

    pMyObject->Cancel((ANSC_HANDLE)pMyObject);
    pMyObject->Reset ((ANSC_HANDLE)pMyObject);

    if ( pRootUriPath )
    {
        pRootUriPath->Remove((ANSC_HANDLE)pRootUriPath);

        pMyObject->hRootUriPath = (ANSC_HANDLE)NULL;
    }

    AnscCoRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRloEnrollObjects
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
WebRloEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_RESOURCE_LOCATOR_OBJECT    pMyObject    = (PWEB_RESOURCE_LOCATOR_OBJECT)hThisObject;
    PWEB_URI_PATH_OBJECT            pRootUriPath = (PWEB_URI_PATH_OBJECT        )pMyObject->hRootUriPath;

    if ( !pRootUriPath )
    {
        pRootUriPath =
            (PWEB_URI_PATH_OBJECT)WebCreateUriPath
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pRootUriPath )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hRootUriPath = (ANSC_HANDLE)pRootUriPath;
        }

        pRootUriPath->SetPathName     ((ANSC_HANDLE)pRootUriPath, WEB_RLO_PATH_SEPARATORS);
        pRootUriPath->SetResourceOwner((ANSC_HANDLE)pRootUriPath, (ANSC_HANDLE)NULL      );
    }

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRloInitialize
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
WebRloInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_RESOURCE_LOCATOR_OBJECT    pMyObject    = (PWEB_RESOURCE_LOCATOR_OBJECT)hThisObject;

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
    pMyObject->Oid               = WEB_RESOURCE_LOCATOR_OID;
    pMyObject->Create            = WebRloCreate;
    pMyObject->Remove            = WebRloRemove;
    pMyObject->EnrollObjects     = WebRloEnrollObjects;
    pMyObject->Initialize        = WebRloInitialize;

    pMyObject->hDefRoo           = (ANSC_HANDLE)NULL;
    pMyObject->bActive           = FALSE;

    pMyObject->GetRootUriPath    = WebRloGetRootUriPath;
    pMyObject->GetProperty       = WebRloGetProperty;
    pMyObject->SetProperty       = WebRloSetProperty;
    pMyObject->ResetProperty     = WebRloResetProperty;
    pMyObject->Reset             = WebRloReset;

    pMyObject->Engage            = WebRloEngage;
    pMyObject->Cancel            = WebRloCancel;

    pMyObject->MakUriPath        = WebRloMakUriPath;
    pMyObject->GetUriPath        = WebRloGetUriPath;
    pMyObject->GetResourceOwner1 = WebRloGetResourceOwner1;
    pMyObject->GetResourceOwner2 = WebRloGetResourceOwner2;
    pMyObject->AddResourceOwner  = WebRloAddResourceOwner;
    pMyObject->PopResourceOwner  = WebRloPopResourceOwner;
    pMyObject->DelResourceOwner  = WebRloDelResourceOwner;

    /*
     * We shall initialize the configuration properties to the default values, which may be changed
     * later via the set_property() member function. Note that this call may not guarantee a valid
     * and legtimate configuration.
     */
    pMyObject->ResetProperty((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
