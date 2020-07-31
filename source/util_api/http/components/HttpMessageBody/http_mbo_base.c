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

    module:	http_mbo_base.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Http Message Body Object.

        *   HttpMboCreate
        *   HttpMboRemove
        *   HttpMboEnrollObjects
        *   HttpMboInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/01/02    initial revision.

**********************************************************************/


#include "http_mbo_global.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        HttpMboCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Http Message Body Object and
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
HttpMboCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PHTTP_MESSAGE_BODY_OBJECT       pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PHTTP_MESSAGE_BODY_OBJECT)AnscAllocateMemory(sizeof(HTTP_MESSAGE_BODY_OBJECT));

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
    /* AnscCopyString(pBaseObject->Name, HTTP_MESSAGE_BODY_NAME); */

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = HTTP_MESSAGE_BODY_OID;
    pBaseObject->Create            = HttpMboCreate;
    pBaseObject->Remove            = HttpMboRemove;
    pBaseObject->EnrollObjects     = HttpMboEnrollObjects;
    pBaseObject->Initialize        = HttpMboInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpMboRemove
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
HttpMboRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_MESSAGE_BODY_OBJECT       pMyObject    = (PHTTP_MESSAGE_BODY_OBJECT)hThisObject;

    pMyObject->Reset((ANSC_HANDLE)pMyObject);

    AnscFreeLock(&pMyObject->BdoQueueLock);
    AnscCoRemove((ANSC_HANDLE)pMyObject  );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpMboEnrollObjects
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
HttpMboEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_MESSAGE_BODY_OBJECT       pMyObject    = (PHTTP_MESSAGE_BODY_OBJECT)hThisObject;

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpMboInitialize
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
HttpMboInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_MESSAGE_BODY_OBJECT       pMyObject    = (PHTTP_MESSAGE_BODY_OBJECT)hThisObject;

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
    pMyObject->Oid             = HTTP_MESSAGE_BODY_OID;
    pMyObject->Create          = HttpMboCreate;
    pMyObject->Remove          = HttpMboRemove;
    pMyObject->EnrollObjects   = HttpMboEnrollObjects;
    pMyObject->Initialize      = HttpMboInitialize;

    pMyObject->hHfpIf          = (ANSC_HANDLE)NULL;
    pMyObject->hBccIf          = (ANSC_HANDLE)NULL;
    pMyObject->State           = HTTP_MBO_STATE_EMPTY;
    pMyObject->Mode            = HTTP_MBO_MODE_STORE;
    pMyObject->TransferSize    = 0;
    pMyObject->EntitySize      = 0;
    pMyObject->ArrivedSize     = 0;

    pMyObject->hFumIf          = (ANSC_HANDLE)NULL;
    pMyObject->MPState         = HTTP_MBO_MP_STATE_INIT;
    pMyObject->hLastMPBdo      = NULL;
    pMyObject->pMPFieldName    = NULL;

    pMyObject->GetHfpIf        = HttpMboGetHfpIf;
    pMyObject->SetHfpIf        = HttpMboSetHfpIf;
    pMyObject->GetBccIf        = HttpMboGetBccIf;
    pMyObject->SetBccIf        = HttpMboSetBccIf;
    pMyObject->SetFumIf        = HttpMboSetFumIf;
    pMyObject->GetState        = HttpMboGetState;
    pMyObject->SetState        = HttpMboSetState;
    pMyObject->GetMode         = HttpMboGetMode;
    pMyObject->SetMode         = HttpMboSetMode;
    pMyObject->Reset           = HttpMboReset;

    pMyObject->GetBodySize     = HttpMboGetBodySize;
    pMyObject->GetBodySize2    = HttpMboGetBodySize2;
    pMyObject->GetTransferSize = HttpMboGetTransferSize;
    pMyObject->GetEntitySize   = HttpMboGetEntitySize;
    pMyObject->GetArrivedSize  = HttpMboGetArrivedSize;
    pMyObject->AppendBodyBdo   = HttpMboAppendBodyBdo;
    pMyObject->AppendBodyFile  = HttpMboAppendBodyFile;
    pMyObject->ClearBody       = HttpMboClearBody;
    pMyObject->CopyBodyFrom    = HttpMboCopyBodyFrom;
    pMyObject->CopyBodyInto    = HttpMboCopyBodyInto;
    pMyObject->SaveAsBody      = HttpMboSaveAsBody;
    pMyObject->SaveBodyAsFile  = HttpMboSaveBodyAsFile;

    pMyObject->Examine         = HttpMboExamine;
    pMyObject->Process         = HttpMboProcess;
    pMyObject->CloseUp         = HttpMboCloseUp;
    pMyObject->Output          = HttpMboOutput;
    pMyObject->RemoveCoding    = HttpMboRemoveCoding;

    pMyObject->ProcessMPData   = HttpMboProcessMPData;

    AnscQueueInitializeHeader(&pMyObject->BdoQueue    );
    AnscInitializeLock       (&pMyObject->BdoQueueLock);

    return  ANSC_STATUS_SUCCESS;
}
