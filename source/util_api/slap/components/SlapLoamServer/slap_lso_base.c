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

    module:	slap_lso_base.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Slap Loam Server Object.

        *   SlapLsoCreate
        *   SlapLsoRemove
        *   SlapLsoEnrollObjects
        *   SlapLsoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/02/03    initial revision.

**********************************************************************/


#include "slap_lso_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        SlapLsoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Slap Loam Server Object and
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
SlapLsoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PSLAP_LOAM_SERVER_OBJECT        pMyObject    = NULL;
    errno_t                         rc           = -1;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PSLAP_LOAM_SERVER_OBJECT)AnscAllocateMemory(sizeof(SLAP_LOAM_SERVER_OBJECT));

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
    rc = strcpy_s(pBaseObject->Name, sizeof(pBaseObject->Name), SLAP_LOAM_SERVER_NAME);
    ERR_CHK(rc);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = SLAP_LOAM_SERVER_OID;
    pBaseObject->Create            = SlapLsoCreate;
    pBaseObject->Remove            = SlapLsoRemove;
    pBaseObject->EnrollObjects     = SlapLsoEnrollObjects;
    pBaseObject->Initialize        = SlapLsoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLsoRemove
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
SlapLsoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_LOAM_SERVER_OBJECT        pMyObject    = (PSLAP_LOAM_SERVER_OBJECT)hThisObject;
    PANSC_CPC_INTERFACE             pAnscCpcIf   = (PANSC_CPC_INTERFACE     )pMyObject->hAnscCpcIf;

    pMyObject->Cancel((ANSC_HANDLE)pMyObject);
    pMyObject->Reset ((ANSC_HANDLE)pMyObject);

    if ( pAnscCpcIf )
    {
        AnscFreeMemory(pAnscCpcIf);

        pMyObject->hAnscCpcIf = (ANSC_HANDLE)NULL;
    }

    AnscCoRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLsoEnrollObjects
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
SlapLsoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_LOAM_SERVER_OBJECT        pMyObject    = (PSLAP_LOAM_SERVER_OBJECT)hThisObject;
    PANSC_CPC_INTERFACE             pAnscCpcIf   = (PANSC_CPC_INTERFACE     )pMyObject->hAnscCpcIf;
    errno_t                         rc           = -1;

    if ( !pAnscCpcIf )
    {
        pAnscCpcIf = (PANSC_CPC_INTERFACE)AnscAllocateMemory(sizeof(ANSC_CPC_INTERFACE));

        if ( !pAnscCpcIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hAnscCpcIf = (ANSC_HANDLE)pAnscCpcIf;
        }

        rc = strcpy_s(pAnscCpcIf->Name, sizeof(pAnscCpcIf->Name), ANSC_CPC_INTERFACE_NAME);
        ERR_CHK(rc);

        pAnscCpcIf->InterfaceId      = ANSC_CPC_INTERFACE_ID;
        pAnscCpcIf->hOwnerContext    = (ANSC_HANDLE)pMyObject;
        pAnscCpcIf->Size             = sizeof(ANSC_CPC_INTERFACE);

        pAnscCpcIf->CallDispatch     = SlapLsoCpcCallDispatch;
        pAnscCpcIf->FreeOutputBuffer = SlapLsoCpcFreeOutputBuffer;
        pAnscCpcIf->NotifyEvent      = SlapLsoCpcNotifyEvent;
    }

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLsoInitialize
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
SlapLsoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_LOAM_SERVER_OBJECT        pMyObject    = (PSLAP_LOAM_SERVER_OBJECT)hThisObject;
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
    pMyObject->Oid                 = SLAP_LOAM_SERVER_OID;
    pMyObject->Create              = SlapLsoCreate;
    pMyObject->Remove              = SlapLsoRemove;
    pMyObject->EnrollObjects       = SlapLsoEnrollObjects;
    pMyObject->Initialize          = SlapLsoInitialize;

    pMyObject->hAnscLpcConnector   = (ANSC_HANDLE)NULL;
    pMyObject->hSlapUoaIf          = (ANSC_HANDLE)NULL;
    pMyObject->bActive             = FALSE;

    pMyObject->GetAnscCpcIf        = SlapLsoGetAnscCpcIf;
    pMyObject->GetAnscLpcConnector = SlapLsoGetAnscLpcConnector;
    pMyObject->SetAnscLpcConnector = SlapLsoSetAnscLpcConnector;
    pMyObject->GetSlapUoaIf        = SlapLsoGetSlapUoaIf;
    pMyObject->SetSlapUoaIf        = SlapLsoSetSlapUoaIf;
    pMyObject->GetMobilityOn       = SlapLsoGetMobilityOn;
    pMyObject->SetMobilityOn       = SlapLsoSetMobilityOn;

    pMyObject->GetProperty         = SlapLsoGetProperty;
    pMyObject->SetProperty         = SlapLsoSetProperty;
    pMyObject->ResetProperty       = SlapLsoResetProperty;
    pMyObject->Reset               = SlapLsoReset;

    pMyObject->Engage              = SlapLsoEngage;
    pMyObject->Cancel              = SlapLsoCancel;
    pMyObject->SetupEnv            = SlapLsoSetupEnv;

    pMyObject->ProcessCallAcqc     = SlapLsoProcessCallAcqc;
    pMyObject->ProcessCallRelc     = SlapLsoProcessCallRelc;
    pMyObject->ProcessCallLoco     = SlapLsoProcessCallLoco;
    pMyObject->ProcessCallDelo     = SlapLsoProcessCallDelo;
    pMyObject->ProcessCallInvo     = SlapLsoProcessCallInvo;
    pMyObject->ProcessCallAcqo     = SlapLsoProcessCallAcqo;
    pMyObject->ProcessCallRelo     = SlapLsoProcessCallRelo;

    pMyObject->CpcCallDispatch     = SlapLsoCpcCallDispatch;
    pMyObject->CpcFreeOutputBuffer = SlapLsoCpcFreeOutputBuffer;
    pMyObject->CpcNotifyEvent      = SlapLsoCpcNotifyEvent;


    /*
     * We shall initialize the configuration properties to the default values, which may be changed
     * later via the set_property() member function. Note that this call may not guarantee a valid
     * and legtimate configuration.
     */
    pMyObject->ResetProperty((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
