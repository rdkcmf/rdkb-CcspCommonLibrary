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

    module:	web_gso_base.c

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Web General Session Object.

        *   WebGsoCreate
        *   WebGsoRemove
        *   WebGsoEnrollObjects
        *   WebGsoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/19/02    initial revision.

**********************************************************************/


#include "web_gso_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        WebGsoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Web General Session Object and
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
WebGsoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PWEB_GENERAL_SESSION_OBJECT     pMyObject    = NULL;
    errno_t                         rc           = -1;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PWEB_GENERAL_SESSION_OBJECT)AnscAllocateMemory(sizeof(WEB_GENERAL_SESSION_OBJECT));

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
    rc = strcpy_s(pBaseObject->Name, sizeof(pBaseObject->Name), WEB_GENERAL_SESSION_NAME);
    ERR_CHK(rc);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = WEB_GENERAL_SESSION_OID;
    pBaseObject->Create            = WebGsoCreate;
    pBaseObject->Remove            = WebGsoRemove;
    pBaseObject->EnrollObjects     = WebGsoEnrollObjects;
    pBaseObject->Initialize        = WebGsoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebGsoRemove
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
WebGsoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_GENERAL_SESSION_OBJECT     pMyObject    = (PWEB_GENERAL_SESSION_OBJECT)hThisObject;

    pMyObject->Cancel((ANSC_HANDLE)pMyObject);
    pMyObject->Reset ((ANSC_HANDLE)pMyObject);

    AnscFreeLock(&pMyObject->SyncLock  );
    AnscCoRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebGsoEnrollObjects
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
WebGsoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_GENERAL_SESSION_OBJECT     pMyObject    = (PWEB_GENERAL_SESSION_OBJECT)hThisObject;

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebGsoInitialize
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
WebGsoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_GENERAL_SESSION_OBJECT     pMyObject    = (PWEB_GENERAL_SESSION_OBJECT)hThisObject;

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
    pMyObject->Oid              = WEB_GENERAL_SESSION_OID;
    pMyObject->Create           = WebGsoCreate;
    pMyObject->Remove           = WebGsoRemove;
    pMyObject->EnrollObjects    = WebGsoEnrollObjects;
    pMyObject->Initialize       = WebGsoInitialize;

    pMyObject->hHfpIf           = (ANSC_HANDLE)NULL;
    pMyObject->hActiveSocket    = (ANSC_HANDLE)NULL;
    pMyObject->SessionId        = 0;
    pMyObject->HashIndex        = 0;
    pMyObject->TimeStampInSec   = AnscGetTickInSecondsAbs();
    pMyObject->LastReqAtInSec   = pMyObject->TimeStampInSec;
    pMyObject->LastRepAtInSec   = pMyObject->TimeStampInSec;
    pMyObject->RequestCount     = 0;
    pMyObject->ClientAddr.Value = 0;
    pMyObject->ClientPort       = 0;
    pMyObject->bActive          = FALSE;
    pMyObject->bLocked          = FALSE;

    /* CID 277590 fix */
    AnscZeroMemory(&pMyObject->AuthSessInfo, sizeof(WEB_AUTH_SESSION_INFO));

    pMyObject->hLsmContext      = (ANSC_HANDLE)NULL;
    pMyObject->LsmMaxAge        = WEB_DEF_LSM_COOKIE_MAX_AGE;
    pMyObject->LsmExpire        = WEB_DEF_SESSION_TIMEOUT;
    pMyObject->MaxLifespan      = 0;

    pMyObject->GetHfpIf         = WebGsoGetHfpIf;
    pMyObject->SetHfpIf         = WebGsoSetHfpIf;
    pMyObject->GetActiveSocket  = WebGsoGetActiveSocket;
    pMyObject->SetActiveSocket  = WebGsoSetActiveSocket;
    pMyObject->GetSessionId     = WebGsoGetSessionId;
    pMyObject->SetSessionId     = WebGsoSetSessionId;
    pMyObject->GetClientAddr    = WebGsoGetClientAddr;
    pMyObject->SetClientAddr    = WebGsoSetClientAddr;
    pMyObject->GetClientPort    = WebGsoGetClientPort;
    pMyObject->SetClientPort    = WebGsoSetClientPort;

    pMyObject->GetLsmContext    = WebGsoGetLsmContext;
    pMyObject->SetLsmContext    = WebGsoSetLsmContext;
    pMyObject->GetLsmIdentifier = WebGsoGetLsmIdentifier;
    pMyObject->SetLsmIdentifier = WebGsoSetLsmIdentifier;
    pMyObject->GetLsmMaxAge     = WebGsoGetLsmMaxAge;
    pMyObject->SetLsmMaxAge     = WebGsoSetLsmMaxAge;
    pMyObject->GetLsmExpire     = WebGsoGetLsmExpire;
    pMyObject->SetLsmExpire     = WebGsoSetLsmExpire;
    pMyObject->GetMaxLifespan   = WebGsoGetMaxLifespan;
    pMyObject->SetMaxLifespan   = WebGsoSetMaxLifespan;
    pMyObject->Reset            = WebGsoReset;

    pMyObject->AcquireAccess    = WebGsoAcquireAccess;
    pMyObject->ReleaseAccess    = WebGsoReleaseAccess;
    pMyObject->Engage           = WebGsoEngage;
    pMyObject->Cancel           = WebGsoCancel;
    pMyObject->Terminate        = WebGsoTerminate;
    pMyObject->Lock             = WebGsoLock;
    pMyObject->Unlock           = WebGsoUnlock;
    pMyObject->ResetTimestamp   = WebGsoResetTimestamp;

    pMyObject->GetAuthSessInfo  = WebGsoGetAuthSessInfo;

    AnscInitializeLock(&pMyObject->SyncLock);

    return  ANSC_STATUS_SUCCESS;
}
