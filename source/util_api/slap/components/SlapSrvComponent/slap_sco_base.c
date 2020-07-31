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

    module:	slap_sco_base.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Slap Srv Component Object.

        *   SlapScoCreate
        *   SlapScoRemove
        *   SlapScoEnrollObjects
        *   SlapScoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/06/03    initial revision.

**********************************************************************/


#include "slap_sco_global.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        SlapScoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Slap Srv Component Object and
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
SlapScoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_LIGHT_COMPONENT_OBJECT    pBaseObject  = NULL;
    PSLAP_SRV_COMPONENT_OBJECT      pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PSLAP_SRV_COMPONENT_OBJECT)AnscAllocateMemory(sizeof(SLAP_SRV_COMPONENT_OBJECT));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pBaseObject = (PANSC_LIGHT_COMPONENT_OBJECT)pMyObject;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = SLAP_SRV_COMPONENT_OID;
    pBaseObject->Create            = SlapScoCreate;
    pBaseObject->Remove            = SlapScoRemove;
    pBaseObject->EnrollObjects     = SlapScoEnrollObjects;
    pBaseObject->Initialize        = SlapScoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapScoRemove
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
SlapScoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_SRV_COMPONENT_OBJECT      pMyObject    = (PSLAP_SRV_COMPONENT_OBJECT)hThisObject;

    pMyObject->Cancel((ANSC_HANDLE)pMyObject);
    pMyObject->Reset ((ANSC_HANDLE)pMyObject);

    if ( pMyObject->ObjName )
    {
        AnscFreeMemory(pMyObject->ObjName);

        pMyObject->ObjName = NULL;
    }

    AnscLcoRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapScoEnrollObjects
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
SlapScoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_SRV_COMPONENT_OBJECT      pMyObject    = (PSLAP_SRV_COMPONENT_OBJECT)hThisObject;

    AnscLcoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapScoInitialize
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
SlapScoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_SRV_COMPONENT_OBJECT      pMyObject    = (PSLAP_SRV_COMPONENT_OBJECT)hThisObject;

    /*
     * Until you have to simulate C++ object-oriented programming style with standard C, you don't
     * appreciate all the nice little things come with C++ language and all the dirty works that
     * have been done by the C++ compilers. Member initialization is one of these things. While in
     * C++ you don't have to initialize all the member fields inherited from the base class since
     * the compiler will do it for you, such is not the case with C.
     */
    AnscLcoInitialize((ANSC_HANDLE)pMyObject);

    /*
     * Although we have initialized some of the member fields in the "create" member function, we
     * repeat the work here for completeness. While this simulation approach is pretty stupid from
     * a C++/Java programmer perspective, it's the best we can get for universal embedded network
     * programming. Before we develop our own operating system (don't expect that to happen any
     * time soon), this is the way things gonna be.
     */
    pMyObject->Oid                 = SLAP_SRV_COMPONENT_OID;
    pMyObject->Create              = SlapScoCreate;
    pMyObject->Remove              = SlapScoRemove;
    pMyObject->EnrollObjects       = SlapScoEnrollObjects;
    pMyObject->Initialize          = SlapScoInitialize;

    pMyObject->ObjName             = NULL;
    pMyObject->ObjType             = 0;
    pMyObject->ObjInstanceId       = 0;

    pMyObject->hInsContext         = (ANSC_HANDLE)NULL;
    pMyObject->hSlapObjContainer   = (ANSC_HANDLE)NULL;
    pMyObject->hSlapObjRecord      = (ANSC_HANDLE)NULL;
    pMyObject->hSlapObjEntity      = (ANSC_HANDLE)NULL;
    pMyObject->hSlapUoaIf          = (ANSC_HANDLE)NULL;
    pMyObject->hSlapBssIf          = (ANSC_HANDLE)NULL;
    pMyObject->hSlapOlaIf          = (ANSC_HANDLE)NULL;

    pMyObject->GetObjName2         = SlapScoGetObjName2;
    pMyObject->SetObjName          = SlapScoSetObjName;
    pMyObject->GetObjType          = SlapScoGetObjType;
    pMyObject->Reset               = SlapScoReset;

    pMyObject->Engage              = SlapScoEngage;
    pMyObject->Cancel              = SlapScoCancel;

    pMyObject->Match1              = SlapScoMatch1;
    pMyObject->InvokeDispatch      = SlapScoInvokeDispatch;
    pMyObject->ConvertParams       = SlapScoConvertParams;

    return  ANSC_STATUS_SUCCESS;
}
