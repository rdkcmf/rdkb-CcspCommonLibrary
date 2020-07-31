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

    module:	slap_scostd_base.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Slap Sco Standard Object.

        *   SlapScoStdCreate
        *   SlapScoStdRemove
        *   SlapScoStdEnrollObjects
        *   SlapScoStdInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/08/03    initial revision.

**********************************************************************/


#include "slap_scostd_global.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        SlapScoStdCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Slap Sco Standard Object and
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
SlapScoStdCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_LIGHT_COMPONENT_OBJECT    pBaseObject  = NULL;
    PSLAP_SCO_STANDARD_OBJECT       pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PSLAP_SCO_STANDARD_OBJECT)AnscAllocateMemory(sizeof(SLAP_SCO_STANDARD_OBJECT));

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
    pBaseObject->Oid               = SLAP_SCO_STANDARD_OID;
    pBaseObject->Create            = SlapScoStdCreate;
    pBaseObject->Remove            = SlapScoStdRemove;
    pBaseObject->EnrollObjects     = SlapScoStdEnrollObjects;
    pBaseObject->Initialize        = SlapScoStdInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapScoStdRemove
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
SlapScoStdRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_SCO_STANDARD_OBJECT       pMyObject    = (PSLAP_SCO_STANDARD_OBJECT)hThisObject;

    SlapScoRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapScoStdEnrollObjects
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
SlapScoStdEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_SCO_STANDARD_OBJECT       pMyObject    = (PSLAP_SCO_STANDARD_OBJECT)hThisObject;

    SlapScoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapScoStdInitialize
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
SlapScoStdInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_SCO_STANDARD_OBJECT       pMyObject    = (PSLAP_SCO_STANDARD_OBJECT)hThisObject;

    /*
     * Until you have to simulate C++ object-oriented programming style with standard C, you don't
     * appreciate all the nice little things come with C++ language and all the dirty works that
     * have been done by the C++ compilers. Member initialization is one of these things. While in
     * C++ you don't have to initialize all the member fields inherited from the base class since
     * the compiler will do it for you, such is not the case with C.
     */
    SlapScoInitialize((ANSC_HANDLE)pMyObject);

    /*
     * Although we have initialized some of the member fields in the "create" member function, we
     * repeat the work here for completeness. While this simulation approach is pretty stupid from
     * a C++/Java programmer perspective, it's the best we can get for universal embedded network
     * programming. Before we develop our own operating system (don't expect that to happen any
     * time soon), this is the way things gonna be.
     */
    pMyObject->Oid             = SLAP_SCO_STANDARD_OID;
    pMyObject->Create          = SlapScoStdCreate;
    pMyObject->Remove          = SlapScoStdRemove;
    pMyObject->EnrollObjects   = SlapScoStdEnrollObjects;
    pMyObject->Initialize      = SlapScoStdInitialize;

    pMyObject->Reset           = SlapScoStdReset;
    pMyObject->InvokeDispatch  = SlapScoStdInvokeDispatch;

    pMyObject->CreateObject    = SlapScoStdCreateObject;
    pMyObject->CreateObject2   = SlapScoStdCreateObject2;
    pMyObject->DeleteObject    = SlapScoStdDeleteObject;
    pMyObject->CloneObject     = SlapScoStdCloneObject;

    pMyObject->ToString        = SlapScoStdToString;

    pMyObject->GetProperty     = SlapScoStdGetProperty;
    pMyObject->SetProperty     = SlapScoStdSetProperty;

    pMyObject->ImportMe        = SlapScoStdImportMe;
    pMyObject->ExportMe        = SlapScoStdExportMe;

    pMyObject->InvokeFixInput0 = SlapScoStdInvokeFixInput0;
    pMyObject->InvokeFixInput1 = SlapScoStdInvokeFixInput1;
    pMyObject->InvokeFixInput2 = SlapScoStdInvokeFixInput2;
    pMyObject->InvokeFixInput3 = SlapScoStdInvokeFixInput3;
    pMyObject->InvokeFixInput4 = SlapScoStdInvokeFixInput4;
    pMyObject->InvokeFixInput5 = SlapScoStdInvokeFixInput5;
    pMyObject->InvokeFixInput6 = SlapScoStdInvokeFixInput6;
    pMyObject->InvokeFixInput7 = SlapScoStdInvokeFixInput7;
    pMyObject->InvokeFixInput8 = SlapScoStdInvokeFixInput8;

    return  ANSC_STATUS_SUCCESS;
}
