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

    module:	slap_owo_base.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Slap Obj Wrapper Object.

        *   SlapOwoCreate
        *   SlapOwoRemove
        *   SlapOwoEnrollObjects
        *   SlapOwoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/04/03    initial revision.

**********************************************************************/


#include "slap_owo_global.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        SlapOwoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Slap Obj Wrapper Object and
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
SlapOwoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_LIGHT_COMPONENT_OBJECT    pBaseObject  = NULL;
    PSLAP_OBJ_WRAPPER_OBJECT        pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PSLAP_OBJ_WRAPPER_OBJECT)AnscAllocateMemory(sizeof(SLAP_OBJ_WRAPPER_OBJECT));

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
    pBaseObject->Oid               = SLAP_OBJ_WRAPPER_OID;
    pBaseObject->Create            = SlapOwoCreate;
    pBaseObject->Remove            = SlapOwoRemove;
    pBaseObject->EnrollObjects     = SlapOwoEnrollObjects;
    pBaseObject->Initialize        = SlapOwoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOwoRemove
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
SlapOwoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_OBJ_WRAPPER_OBJECT        pMyObject    = (PSLAP_OBJ_WRAPPER_OBJECT)hThisObject;

    pMyObject->Reset((ANSC_HANDLE)pMyObject);

    AnscLcoRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOwoEnrollObjects
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
SlapOwoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_OBJ_WRAPPER_OBJECT        pMyObject    = (PSLAP_OBJ_WRAPPER_OBJECT)hThisObject;

    AnscLcoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOwoInitialize
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
SlapOwoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_OBJ_WRAPPER_OBJECT        pMyObject    = (PSLAP_OBJ_WRAPPER_OBJECT)hThisObject;

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
    pMyObject->Oid             = SLAP_OBJ_WRAPPER_OID;
    pMyObject->Create          = SlapOwoCreate;
    pMyObject->Remove          = SlapOwoRemove;
    pMyObject->EnrollObjects   = SlapOwoEnrollObjects;
    pMyObject->Initialize      = SlapOwoInitialize;

    pMyObject->ObjType         = SLAP_OWO_OBJ_TYPE_uoaObject;
    pMyObject->ObjMode         = SLAP_OWO_OBJ_MODE_acquired;
    pMyObject->ObjName         = NULL;
    pMyObject->LpcPartyAddr    = (ANSC_HANDLE)NULL;
    pMyObject->LpcPartyName    = NULL;
    pMyObject->hWrappedObj     = (ANSC_HANDLE)NULL;
    pMyObject->hGoaContainer   = (ANSC_HANDLE)NULL;
    pMyObject->hSlapGoaIf      = (ANSC_HANDLE)NULL;

    pMyObject->SetObjName      = SlapOwoSetObjName;
    pMyObject->SetLpcPartyAddr = SlapOwoSetLpcPartyAddr;
    pMyObject->SetLpcPartyName = SlapOwoSetLpcPartyName;
    pMyObject->Reset           = SlapOwoReset;

    return  ANSC_STATUS_SUCCESS;
}
