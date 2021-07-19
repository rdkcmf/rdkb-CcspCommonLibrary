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

    module:	slap_oeo_base.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Slap Obj Entity Object.

        *   SlapOeoCreate
        *   SlapOeoRemove
        *   SlapOeoEnrollObjects
        *   SlapOeoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/05/03    initial revision.

**********************************************************************/


#include "slap_oeo_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        SlapOeoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Slap Obj Entity Object and
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
SlapOeoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_LIGHT_COMPONENT_OBJECT    pBaseObject  = NULL;
    PSLAP_OBJ_ENTITY_OBJECT         pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PSLAP_OBJ_ENTITY_OBJECT)AnscAllocateMemory(sizeof(SLAP_OBJ_ENTITY_OBJECT));

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
    pBaseObject->Oid               = SLAP_OBJ_ENTITY_OID;
    pBaseObject->Create            = SlapOeoCreate;
    pBaseObject->Remove            = SlapOeoRemove;
    pBaseObject->EnrollObjects     = SlapOeoEnrollObjects;
    pBaseObject->Initialize        = SlapOeoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOeoRemove
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
SlapOeoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_OBJ_ENTITY_OBJECT         pMyObject    = (PSLAP_OBJ_ENTITY_OBJECT)hThisObject;
    PSLAP_OLA_INTERFACE             pSlapOlaIf   = (PSLAP_OLA_INTERFACE    )pMyObject->hSlapOlaIf;

    pMyObject->Reset((ANSC_HANDLE)pMyObject);

    if ( pSlapOlaIf )
    {
        AnscFreeMemory(pSlapOlaIf);

        pMyObject->hSlapOlaIf = (ANSC_HANDLE)NULL;
    }

    AnscFreeLock (&pMyObject->OpoTableLock      );
    AnscFreeLock (&pMyObject->MdoTableLock      );
    AnscFreeLock (&pMyObject->OroTableLock      );
    AnscFreeLock (&pMyObject->PooledOroSListLock);

    AnscLcoRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOeoEnrollObjects
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
SlapOeoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_OBJ_ENTITY_OBJECT         pMyObject    = (PSLAP_OBJ_ENTITY_OBJECT)hThisObject;
    PSLAP_OLA_INTERFACE             pSlapOlaIf   = (PSLAP_OLA_INTERFACE    )pMyObject->hSlapOlaIf;
    errno_t                         rc           = -1;

    if ( !pSlapOlaIf )
    {
        pSlapOlaIf = (PSLAP_OLA_INTERFACE)AnscAllocateMemory(sizeof(SLAP_OLA_INTERFACE));

        if ( !pSlapOlaIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hSlapOlaIf = (ANSC_HANDLE)pSlapOlaIf;
        }

        rc = strcpy_s(pSlapOlaIf->Name, sizeof(pSlapOlaIf->Name), SLAP_OLA_INTERFACE_NAME);
        ERR_CHK(rc);

        pSlapOlaIf->InterfaceId         = SLAP_OLA_INTERFACE_ID;
        pSlapOlaIf->hOwnerContext       = (ANSC_HANDLE)pMyObject;
        pSlapOlaIf->Size                = sizeof(SLAP_OLA_INTERFACE);

        pSlapOlaIf->GetObjectProperty   = SlapOeoOlaGetObjectProperty;
        pSlapOlaIf->GetMethodDescriptor = SlapOeoOlaGetMethodDescriptor;
    }

    AnscLcoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOeoInitialize
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
SlapOeoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_OBJ_ENTITY_OBJECT         pMyObject    = (PSLAP_OBJ_ENTITY_OBJECT)hThisObject;
    ULONG                           i            = 0;

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
    pMyObject->Oid                     = SLAP_OBJ_ENTITY_OID;
    pMyObject->Create                  = SlapOeoCreate;
    pMyObject->Remove                  = SlapOeoRemove;
    pMyObject->EnrollObjects           = SlapOeoEnrollObjects;
    pMyObject->Initialize              = SlapOeoInitialize;

    pMyObject->hSlapUoaIf              = (ANSC_HANDLE)NULL;
    pMyObject->hSlapBssIf              = (ANSC_HANDLE)NULL;
    pMyObject->hObjDescriptor          = (ANSC_HANDLE)NULL;
    pMyObject->OpoTable                = NULL;
    pMyObject->OpoTableSize            = SLAP_OBJ_DEF_OPO_TABLE_SIZE;
    pMyObject->MdoTable                = NULL;
    pMyObject->MdoTableSize            = SLAP_OBJ_DEF_MDO_TABLE_SIZE;

    pMyObject->SetObjDescriptor        = SlapOeoSetObjDescriptor;
    pMyObject->Reset                   = SlapOeoReset;

    pMyObject->SetupOpoTable           = SlapOeoSetupOpoTable;
    pMyObject->SetupMdoTable           = SlapOeoSetupMdoTable;
    pMyObject->RegisterCall            = SlapOeoRegisterCall;

    pMyObject->GetObjectProperty       = SlapOeoGetObjectProperty;
    pMyObject->AddObjectProperty       = SlapOeoAddObjectProperty;
    pMyObject->DelObjectProperty       = SlapOeoDelObjectProperty;
    pMyObject->DelAllObjectProperties  = SlapOeoDelAllObjectProperties;

    pMyObject->GetMethodDescriptor1    = SlapOeoGetMethodDescriptor1;
    pMyObject->GetMethodDescriptor2    = SlapOeoGetMethodDescriptor2;
    pMyObject->AddMethodDescriptor     = SlapOeoAddMethodDescriptor;
    pMyObject->DelMethodDescriptor     = SlapOeoDelMethodDescriptor;
    pMyObject->DelAllMethodDescriptors = SlapOeoDelAllMethodDescriptors;

    pMyObject->GetObjRecord            = SlapOeoGetObjRecord;
    pMyObject->NewObjRecord            = SlapOeoNewObjRecord;
    pMyObject->DelObjRecord            = SlapOeoDelObjRecord;
    pMyObject->DelAllObjRecords        = SlapOeoDelAllObjRecords;

    pMyObject->AcqPooledObjRecord      = SlapOeoAcqPooledObjRecord;
    pMyObject->RelPooledObjRecord      = SlapOeoRelPooledObjRecord;
    pMyObject->DelAllPooledObjRecords  = SlapOeoDelAllPooledObjRecords;

    pMyObject->ResolveCallDescrI       = SlapOeoResolveCallDescrI;
    pMyObject->ResolveCallDescrO       = SlapOeoResolveCallDescrO;
    pMyObject->ResolveCallDescrR       = SlapOeoResolveCallDescrR;
    pMyObject->ResolveCallBridge       = SlapOeoResolveCallBridge;
    pMyObject->ResolveCallOption       = SlapOeoResolveCallOption;

    pMyObject->OlaGetObjectProperty    = SlapOeoOlaGetObjectProperty;
    pMyObject->OlaGetMethodDescriptor  = SlapOeoOlaGetMethodDescriptor;

    AnscInitializeLock(&pMyObject->OpoTableLock);
    AnscInitializeLock(&pMyObject->MdoTableLock);

    for ( i = 0; i < SLAP_OEO_ORO_TABLE_SIZE; i++ )
    {
        AnscQueueInitializeHeader(&pMyObject->OroTable[i]);
    }
    AnscInitializeLock(&pMyObject->OroTableLock);

    AnscSListInitializeHeader(&pMyObject->PooledOroSList    );
    AnscInitializeLock       (&pMyObject->PooledOroSListLock);

    return  ANSC_STATUS_SUCCESS;
}
