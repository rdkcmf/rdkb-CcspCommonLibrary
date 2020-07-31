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

    module:	ansc_oco_base.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Ansc Object Container Object.

        *   AnscOcoCreate
        *   AnscOcoRemove
        *   AnscOcoEnrollObjects
        *   AnscOcoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        10/15/01    initial revision.

**********************************************************************/


#include "ansc_oco_global.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscOcoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Ansc Object Container Object and
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
AnscOcoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PANSC_OBJECT_CONTAINER_OBJECT   pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PANSC_OBJECT_CONTAINER_OBJECT)AnscAllocateMemory(sizeof(ANSC_OBJECT_CONTAINER_OBJECT));

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
    /* AnscCopyString(pBaseObject->Name, ANSC_OBJECT_CONTAINER_NAME); */

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = ANSC_OBJECT_CONTAINER_OID;
    pBaseObject->Create            = AnscOcoCreate;
    pBaseObject->Remove            = AnscOcoRemove;
    pBaseObject->EnrollObjects     = AnscOcoEnrollObjects;
    pBaseObject->Initialize        = AnscOcoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscOcoRemove
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
AnscOcoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_OBJECT_CONTAINER_OBJECT   pMyObject    = (PANSC_OBJECT_CONTAINER_OBJECT)hThisObject;

    if ( pMyObject->bEngaged )
    {
        pMyObject->Cancel((ANSC_HANDLE)pMyObject);
    }

    if ( pMyObject->hCarrierContext )
    {
        AnscFreeMemory(pMyObject->hCarrierContext);

        pMyObject->hCarrierContext = (ANSC_HANDLE)NULL;
    }

    pMyObject->Reset((ANSC_HANDLE)pMyObject);

    AnscCoRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscOcoEnrollObjects
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
AnscOcoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_OBJECT_CONTAINER_OBJECT   pMyObject    = (PANSC_OBJECT_CONTAINER_OBJECT)hThisObject;

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscOcoInitialize
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
AnscOcoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_OBJECT_CONTAINER_OBJECT   pMyObject    = (PANSC_OBJECT_CONTAINER_OBJECT)hThisObject;

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
    pMyObject->Oid                         = ANSC_OBJECT_CONTAINER_OID;
    pMyObject->Create                      = AnscOcoCreate;
    pMyObject->Remove                      = AnscOcoRemove;
    pMyObject->EnrollObjects               = AnscOcoEnrollObjects;
    pMyObject->Initialize                  = AnscOcoInitialize;

    pMyObject->hExternalObjectMapper       = (ANSC_HANDLE)NULL;
    pMyObject->hInternalObjectMapper       = (ANSC_HANDLE)NULL;
    pMyObject->hTimerScheduler             = (ANSC_HANDLE)NULL;
    pMyObject->hIpcPort                    = (ANSC_HANDLE)NULL;
    pMyObject->hCarrierContext             = (ANSC_HANDLE)NULL;
    pMyObject->OpMask                      = ANSC_OCO_OPMASK_OMO_EXTERNAL | ANSC_OCO_OPMASK_OMO_INTERNAL |
                                             ANSC_OCO_OPMASK_TSO          | ANSC_OCO_OPMASK_COLLAPSE;
    pMyObject->bEngaged                    = FALSE;

    pMyObject->GetExternalOmo              = AnscOcoGetExternalOmo;
    pMyObject->SetExternalOmo              = AnscOcoSetExternalOmo;
    pMyObject->GetInternalOmo              = AnscOcoGetInternalOmo;
    pMyObject->SetInternalOmo              = AnscOcoSetInternalOmo;
    pMyObject->GetTso                      = AnscOcoGetTso;
    pMyObject->SetTso                      = AnscOcoSetTso;
    pMyObject->GetIpcPort                  = AnscOcoGetIpcPort;
    pMyObject->SetIpcPort                  = AnscOcoSetIpcPort;
    pMyObject->GetCarrier                  = AnscOcoGetCarrier;
    pMyObject->SetCarrier                  = AnscOcoSetCarrier;
    pMyObject->GetOpMask                   = AnscOcoGetOpMask;
    pMyObject->SetOpMask                   = AnscOcoSetOpMask;
    pMyObject->Reset                       = AnscOcoReset;

    pMyObject->Engage                      = AnscOcoEngage;
    pMyObject->Cancel                      = AnscOcoCancel;
    pMyObject->SubEngage                   = AnscOcoSubEngage;
    pMyObject->SubCancel                   = AnscOcoSubCancel;
    pMyObject->EnrollAllObjects            = AnscOcoEnrollAllObjects;
    pMyObject->ManufactureAllObjects       = AnscOcoManufactureAllObjects;
    pMyObject->DestroyAllObjects           = AnscOcoDestroyAllObjects;

    pMyObject->EnrollExtensionObjects      = AnscOcoEnrollExtensionObjects;
    pMyObject->ManufactureExtensionObjects = AnscOcoManufactureExtensionObjects;
    pMyObject->DestroyExtensionObjects     = AnscOcoDestroyExtensionObjects;

    pMyObject->EnrollFeatureObjects        = AnscOcoEnrollFeatureObjects;
    pMyObject->ManufactureFeatureObjects   = AnscOcoManufactureFeatureObjects;
    pMyObject->DestroyFeatureObjects       = AnscOcoDestroyFeatureObjects;

    pMyObject->RegisterObject1             = AnscOcoRegisterObject1;
    pMyObject->RegisterObject2             = AnscOcoRegisterObject2;
    pMyObject->CreateObjectByName          = AnscOcoCreateObjectByName;
    pMyObject->CreateObjectByOid           = AnscOcoCreateObjectByOid;
    pMyObject->CreateObjectByType          = AnscOcoCreateObjectByType;
    pMyObject->GetObjectByRid              = AnscOcoGetObjectByRid;

    pMyObject->RegisterTimer               = AnscOcoRegisterTimer;
    pMyObject->CancelTimer                 = AnscOcoCancelTimer;

    return  ANSC_STATUS_SUCCESS;
}
