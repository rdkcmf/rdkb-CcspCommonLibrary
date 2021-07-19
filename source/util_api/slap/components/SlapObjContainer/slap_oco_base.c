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

    module:	slap_oco_base.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Slap Obj Container Object.

        *   SlapOcoCreate
        *   SlapOcoRemove
        *   SlapOcoEnrollObjects
        *   SlapOcoInitialize

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


#include "slap_oco_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        SlapOcoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Slap Obj Container Object and
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
SlapOcoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PSLAP_OBJ_CONTAINER_OBJECT      pMyObject    = NULL;
    errno_t                         rc           = -1;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PSLAP_OBJ_CONTAINER_OBJECT)AnscAllocateMemory(sizeof(SLAP_OBJ_CONTAINER_OBJECT));

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
    rc = strcpy_s(pBaseObject->Name, sizeof(pBaseObject->Name), SLAP_OBJ_CONTAINER_NAME);
    ERR_CHK(rc);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = SLAP_OBJ_CONTAINER_OID;
    pBaseObject->Create            = SlapOcoCreate;
    pBaseObject->Remove            = SlapOcoRemove;
    pBaseObject->EnrollObjects     = SlapOcoEnrollObjects;
    pBaseObject->Initialize        = SlapOcoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOcoRemove
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
SlapOcoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_OBJ_CONTAINER_OBJECT      pMyObject    = (PSLAP_OBJ_CONTAINER_OBJECT)hThisObject;

    pMyObject->Reset((ANSC_HANDLE)pMyObject);

    AnscFreeLock(&pMyObject->ScoTableLock);
    AnscCoRemove((ANSC_HANDLE)pMyObject  );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOcoEnrollObjects
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
SlapOcoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_OBJ_CONTAINER_OBJECT      pMyObject    = (PSLAP_OBJ_CONTAINER_OBJECT)hThisObject;

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOcoInitialize
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
SlapOcoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_OBJ_CONTAINER_OBJECT      pMyObject    = (PSLAP_OBJ_CONTAINER_OBJECT)hThisObject;
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
    pMyObject->Oid                 = SLAP_OBJ_CONTAINER_OID;
    pMyObject->Create              = SlapOcoCreate;
    pMyObject->Remove              = SlapOcoRemove;
    pMyObject->EnrollObjects       = SlapOcoEnrollObjects;
    pMyObject->Initialize          = SlapOcoInitialize;

    pMyObject->hSlapUoaIf          = (ANSC_HANDLE)NULL;
    pMyObject->hSlapBssIf          = (ANSC_HANDLE)NULL;
    pMyObject->hSlapObjMapper      = (ANSC_HANDLE)NULL;
    pMyObject->ContainerName       = NULL;
    pMyObject->GlobalInsId         = 0;
    pMyObject->HashIndex           = 0;

    pMyObject->GetSlapUoaIf        = SlapOcoGetSlapUoaIf;
    pMyObject->SetSlapUoaIf        = SlapOcoSetSlapUoaIf;
    pMyObject->GetSlapBssIf        = SlapOcoGetSlapBssIf;
    pMyObject->SetSlapBssIf        = SlapOcoSetSlapBssIf;
    pMyObject->GetSlapObjMapper    = SlapOcoGetSlapObjMapper;
    pMyObject->SetSlapObjMapper    = SlapOcoSetSlapObjMapper;
    pMyObject->GetContainerName    = SlapOcoGetContainerName;
    pMyObject->SetContainerName    = SlapOcoSetContainerName;
    pMyObject->GetContainerType    = SlapOcoGetContainerType;
    pMyObject->SetContainerType    = SlapOcoSetContainerType;
    pMyObject->Reset               = SlapOcoReset;

    pMyObject->AssignInstanceId    = SlapOcoAssignInstanceId;
    pMyObject->GetSrvComponent     = SlapOcoGetSrvComponent;
    pMyObject->AddSrvComponent     = SlapOcoAddSrvComponent;
    pMyObject->DelSrvComponent     = SlapOcoDelSrvComponent;
    pMyObject->DelAllSrvComponents = SlapOcoDelAllSrvComponents;

    for ( i = 0; i < SLAP_OCO_SCO_TABLE_SIZE; i++ )
    {
        AnscQueueInitializeHeader(&pMyObject->ScoTable[i]);
    }
    AnscInitializeLock(&pMyObject->ScoTableLock);

    return  ANSC_STATUS_SUCCESS;
}
