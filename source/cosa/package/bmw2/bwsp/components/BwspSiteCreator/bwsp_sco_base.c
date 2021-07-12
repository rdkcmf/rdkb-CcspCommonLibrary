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

    module:	bwsp_sco_base.c

        For Broadway Web Service Plane Implementation (BWSP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Bwsp Site Creator Object.

        *   BwspScoCreate
        *   BwspScoRemove
        *   BwspScoEnrollObjects
        *   BwspScoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/02/03    initial revision.

**********************************************************************/


#include "bwsp_sco_global.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        BwspScoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Bwsp Site Creator Object and
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
BwspScoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PBWSP_SITE_CREATOR_OBJECT       pMyObject    = NULL;
    errno_t   rc = -1;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PBWSP_SITE_CREATOR_OBJECT)AnscAllocateMemory(sizeof(BWSP_SITE_CREATOR_OBJECT));

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
    rc = STRCPY_S_NOCLOBBER(pBaseObject->Name, sizeof(pBaseObject->Name), BWSP_SITE_CREATOR_NAME);
    ERR_CHK(rc);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = BWSP_SITE_CREATOR_OID;
    pBaseObject->Create            = BwspScoCreate;
    pBaseObject->Remove            = BwspScoRemove;
    pBaseObject->EnrollObjects     = BwspScoEnrollObjects;
    pBaseObject->Initialize        = BwspScoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwspScoRemove
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
BwspScoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWSP_SITE_CREATOR_OBJECT       pMyObject    = (PBWSP_SITE_CREATOR_OBJECT)hThisObject;
    PBWSP_WSR_INTERFACE             pBwspWsrIf   = (PBWSP_WSR_INTERFACE      )pMyObject->hBwspWsrIf;
    PBWSP_CSP_INTERFACE             pBwspCspIf   = (PBWSP_CSP_INTERFACE      )pMyObject->hBwspCspIf;

    pMyObject->Cancel((ANSC_HANDLE)pMyObject);
    pMyObject->Reset ((ANSC_HANDLE)pMyObject);

    if ( pBwspWsrIf )
    {
        AnscFreeMemory(pBwspWsrIf);
    }

    if ( pBwspCspIf )
    {
        AnscFreeMemory(pBwspCspIf);
    }

    AnscCoRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwspScoEnrollObjects
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
BwspScoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWSP_SITE_CREATOR_OBJECT       pMyObject    = (PBWSP_SITE_CREATOR_OBJECT)hThisObject;
    PBWSP_WSR_INTERFACE             pBwspWsrIf   = (PBWSP_WSR_INTERFACE      )pMyObject->hBwspWsrIf;
    PBWSP_CSP_INTERFACE             pBwspCspIf   = (PBWSP_CSP_INTERFACE      )pMyObject->hBwspCspIf;
    errno_t  rc = -1;

    if ( !pBwspWsrIf )
    {
        pBwspWsrIf = (PBWSP_WSR_INTERFACE)AnscAllocateMemory(sizeof(BWSP_WSR_INTERFACE));

        if ( !pBwspWsrIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hBwspWsrIf = (ANSC_HANDLE)pBwspWsrIf;
        }

        rc = STRCPY_S_NOCLOBBER(pBwspWsrIf->Name, sizeof(pBwspWsrIf->Name), BWSP_WSR_INTERFACE_NAME);
        ERR_CHK(rc);

        pBwspWsrIf->InterfaceId   = BWSP_WSR_INTERFACE_ID;
        pBwspWsrIf->hOwnerContext = (ANSC_HANDLE)pMyObject;
        pBwspWsrIf->Size          = sizeof(BWSP_WSR_INTERFACE);

        pBwspWsrIf->AttachWebHost = BwspScoWsrAttachWebHost;
        pBwspWsrIf->DetachWebHost = BwspScoWsrDetachWebHost;
        pBwspWsrIf->AttachServlet = BwspScoWsrAttachServlet;
        pBwspWsrIf->DetachServlet = BwspScoWsrDetachServlet;
    }

    if ( !pBwspCspIf )
    {
        pBwspCspIf = (PBWSP_CSP_INTERFACE)AnscAllocateMemory(sizeof(BWSP_CSP_INTERFACE));

        if ( !pBwspCspIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hBwspCspIf = (ANSC_HANDLE)pBwspCspIf;
        }

        rc = STRCPY_S_NOCLOBBER(pBwspCspIf->Name, sizeof(pBwspCspIf->Name), BWSP_CSP_INTERFACE_NAME);
        ERR_CHK(rc);

        pBwspCspIf->InterfaceId    = BWSP_CSP_INTERFACE_ID;
        pBwspCspIf->hOwnerContext  = (ANSC_HANDLE)pMyObject;
        pBwspCspIf->Size           = sizeof(BWSP_CSP_INTERFACE);

        pBwspCspIf->ConfigProperty = BwspScoCspConfigProperty;
    }

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwspScoInitialize
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
BwspScoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWSP_SITE_CREATOR_OBJECT       pMyObject    = (PBWSP_SITE_CREATOR_OBJECT)hThisObject;

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
    pMyObject->Oid               = BWSP_SITE_CREATOR_OID;
    pMyObject->Create            = BwspScoCreate;
    pMyObject->Remove            = BwspScoRemove;
    pMyObject->EnrollObjects     = BwspScoEnrollObjects;
    pMyObject->Initialize        = BwspScoInitialize;

    pMyObject->hWebHemIf         = (ANSC_HANDLE)NULL;
    pMyObject->bActive           = FALSE;

    pMyObject->GetWebHemIf       = BwspScoGetWebHemIf;
    pMyObject->SetWebHemIf       = BwspScoSetWebHemIf;
    pMyObject->GetBwspWsrIf      = BwspScoGetBwspWsrIf;
    pMyObject->GetBwspCspIf      = BwspScoGetBwspCspIf;

    pMyObject->GetProperty       = BwspScoGetProperty;
    pMyObject->SetProperty       = BwspScoSetProperty;
    pMyObject->ResetProperty     = BwspScoResetProperty;
    pMyObject->Reset             = BwspScoReset;

    pMyObject->Engage            = BwspScoEngage;
    pMyObject->Cancel            = BwspScoCancel;

    pMyObject->CspConfigProperty = BwspScoCspConfigProperty;

    pMyObject->WsrAttachWebHost  = BwspScoWsrAttachWebHost;
    pMyObject->WsrDetachWebHost  = BwspScoWsrDetachWebHost;
    pMyObject->WsrAttachServlet  = BwspScoWsrAttachServlet;
    pMyObject->WsrDetachServlet  = BwspScoWsrDetachServlet;

    /*
     * We shall initialize the configuration properties to the default values, which may be changed
     * later via the set_property() member function. Note that this call may not guarantee a valid
     * and legtimate configuration.
     */
    pMyObject->ResetProperty((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
