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

    module:	bwsp_wso_base.c

        For Broadway Web Service Plane Implementation (BWSP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Bwsp Web Servlet Object.

        *   BwspWsoCreate
        *   BwspWsoRemove
        *   BwspWsoEnrollObjects
        *   BwspWsoInitialize

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


#include "bwsp_wso_global.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        BwspWsoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Bwsp Web Servlet Object and
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
BwspWsoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PBWSP_WEB_SERVLET_OBJECT        pMyObject    = NULL;
    errno_t   rc   = -1;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PBWSP_WEB_SERVLET_OBJECT)AnscAllocateMemory(sizeof(BWSP_WEB_SERVLET_OBJECT));

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
    rc = STRCPY_S_NOCLOBBER(pBaseObject->Name, sizeof(pBaseObject->Name), BWSP_WEB_SERVLET_NAME);
    ERR_CHK(rc);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = BWSP_WEB_SERVLET_OID;
    pBaseObject->Create            = BwspWsoCreate;
    pBaseObject->Remove            = BwspWsoRemove;
    pBaseObject->EnrollObjects     = BwspWsoEnrollObjects;
    pBaseObject->Initialize        = BwspWsoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwspWsoRemove
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
BwspWsoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWSP_WEB_SERVLET_OBJECT        pMyObject    = (PBWSP_WEB_SERVLET_OBJECT)hThisObject;
    PBWSP_WSH_INTERFACE             pBwspWshIf   = (PBWSP_WSH_INTERFACE     )pMyObject->hBwspWshIf;

    if ( pBwspWshIf )
    {
        AnscFreeMemory(pBwspWshIf);

        pMyObject->hBwspWshIf = (ANSC_HANDLE)NULL;
    }

    WebRooRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwspWsoEnrollObjects
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
BwspWsoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWSP_WEB_SERVLET_OBJECT        pMyObject    = (PBWSP_WEB_SERVLET_OBJECT)hThisObject;
    PBWSP_WSH_INTERFACE             pBwspWshIf   = (PBWSP_WSH_INTERFACE     )pMyObject->hBwspWshIf;
    errno_t  rc  = -1;

    if ( !pBwspWshIf )
    {
        pBwspWshIf = (PBWSP_WSH_INTERFACE)AnscAllocateMemory(sizeof(BWSP_WSH_INTERFACE));

        if ( !pBwspWshIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hBwspWshIf = (ANSC_HANDLE)pBwspWshIf;
        }

        rc = STRCPY_S_NOCLOBBER(pBwspWshIf->Name, sizeof(pBwspWshIf->Name), BWSP_WSH_INTERFACE_NAME);
        ERR_CHK(rc);

        pBwspWshIf->InterfaceId   = BWSP_WSH_INTERFACE_ID;
        pBwspWshIf->hOwnerContext = (ANSC_HANDLE)pMyObject;
        pBwspWshIf->Size          = sizeof(BWSP_WSH_INTERFACE);

        pBwspWshIf->DoOptions     = NULL;
        pBwspWshIf->DoGet         = NULL;
        pBwspWshIf->DoHead        = NULL;
        pBwspWshIf->DoPost        = NULL;
        pBwspWshIf->DoPut         = NULL;
        pBwspWshIf->DoDelete      = NULL;
        pBwspWshIf->DoTrace       = NULL;
        pBwspWshIf->DoConnect     = NULL;

        pBwspWshIf->DoNotify      = NULL;
        pBwspWshIf->DoSearch      = NULL;
        pBwspWshIf->DoMSearch     = NULL;
        pBwspWshIf->DoMPost       = NULL;
        pBwspWshIf->DoSubscribe   = NULL;
        pBwspWshIf->DoUnsubscribe = NULL;
    }

    WebRooEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwspWsoInitialize
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
BwspWsoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWSP_WEB_SERVLET_OBJECT        pMyObject    = (PBWSP_WEB_SERVLET_OBJECT)hThisObject;

    /*
     * Until you have to simulate C++ object-oriented programming style with standard C, you don't
     * appreciate all the nice little things come with C++ language and all the dirty works that
     * have been done by the C++ compilers. Member initialization is one of these things. While in
     * C++ you don't have to initialize all the member fields inherited from the base class since
     * the compiler will do it for you, such is not the case with C.
     */
    WebRooInitialize((ANSC_HANDLE)pMyObject);

    /*
     * Although we have initialized some of the member fields in the "create" member function, we
     * repeat the work here for completeness. While this simulation approach is pretty stupid from
     * a C++/Java programmer perspective, it's the best we can get for universal embedded network
     * programming. Before we develop our own operating system (don't expect that to happen any
     * time soon), this is the way things gonna be.
     */
    pMyObject->Oid           = BWSP_WEB_SERVLET_OID;
    pMyObject->Create        = BwspWsoCreate;
    pMyObject->Remove        = BwspWsoRemove;
    pMyObject->EnrollObjects = BwspWsoEnrollObjects;
    pMyObject->Initialize    = BwspWsoInitialize;

    pMyObject->ResetProperty = BwspWsoResetProperty;
    pMyObject->Reset         = BwspWsoReset;

    pMyObject->DoOptions     = BwspWsoDoOptions;
    pMyObject->DoGet         = BwspWsoDoGet;
    pMyObject->DoHead        = BwspWsoDoHead;
    pMyObject->DoPost        = BwspWsoDoPost;
    pMyObject->DoPut         = BwspWsoDoPut;
    pMyObject->DoDelete      = BwspWsoDoDelete;
    pMyObject->DoTrace       = BwspWsoDoTrace;
    pMyObject->DoConnect     = BwspWsoDoConnect;

    pMyObject->DoNotify      = BwspWsoDoNotify;
    pMyObject->DoSearch      = BwspWsoDoSearch;
    pMyObject->DoMSearch     = BwspWsoDoMSearch;
    pMyObject->DoMPost       = BwspWsoDoMPost;
    pMyObject->DoSubscribe   = BwspWsoDoSubscribe;
    pMyObject->DoUnsubscribe = BwspWsoDoUnsubscribe;

    pMyObject->GetBwspWshIf  = BwspWsoGetBwspWshIf;
    pMyObject->SetBwspWshIf  = BwspWsoSetBwspWshIf;

    /*
     * We shall initialize the configuration properties to the default values, which may be changed
     * later via the set_property() member function. Note that this call may not guarantee a valid
     * and legtimate configuration.
     */
    pMyObject->ResetProperty((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
