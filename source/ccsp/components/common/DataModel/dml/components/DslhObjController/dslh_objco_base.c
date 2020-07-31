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

    module:	dslh_objco_base.c

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Dslh Obj Controller Object.

        *   DslhObjcoCreate
        *   DslhObjcoRemove
        *   DslhObjcoEnrollObjects
        *   DslhObjcoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        10/03/2005    initial revision.
        09/22/2011    Bin added GetBulkParamValue apis;

**********************************************************************/


#include "dslh_objco_global.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        DslhObjcoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Dslh Obj Controller Object and
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
DslhObjcoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_LIGHT_COMPONENT_OBJECT    pBaseObject  = NULL;
    PDSLH_OBJ_CONTROLLER_OBJECT     pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PDSLH_OBJ_CONTROLLER_OBJECT)AnscAllocateMemory(sizeof(DSLH_OBJ_CONTROLLER_OBJECT));

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
    pBaseObject->Oid               = DSLH_OBJ_CONTROLLER_OID;
    pBaseObject->Create            = DslhObjcoCreate;
    pBaseObject->Remove            = DslhObjcoRemove;
    pBaseObject->EnrollObjects     = DslhObjcoEnrollObjects;
    pBaseObject->Initialize        = DslhObjcoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjcoRemove
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
DslhObjcoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_OBJ_CONTROLLER_OBJECT     pMyObject    = (PDSLH_OBJ_CONTROLLER_OBJECT)hThisObject;

    pMyObject->Reset((ANSC_HANDLE)pMyObject);

    AnscLcoRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjcoEnrollObjects
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
DslhObjcoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_OBJ_CONTROLLER_OBJECT     pMyObject    = (PDSLH_OBJ_CONTROLLER_OBJECT)hThisObject;

    AnscLcoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjcoInitialize
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
DslhObjcoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_OBJ_CONTROLLER_OBJECT     pMyObject    = (PDSLH_OBJ_CONTROLLER_OBJECT)hThisObject;

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
    pMyObject->Oid                  = DSLH_OBJ_CONTROLLER_OID;
    pMyObject->Create               = DslhObjcoCreate;
    pMyObject->Remove               = DslhObjcoRemove;
    pMyObject->EnrollObjects        = DslhObjcoEnrollObjects;
    pMyObject->Initialize           = DslhObjcoInitialize;

    pMyObject->hDslhObjRecord       = (ANSC_HANDLE)NULL;
    pMyObject->hDslhCpeController   = (ANSC_HANDLE)NULL;
    pMyObject->hInsContext          = (ANSC_HANDLE)NULL;
    pMyObject->bCommitted           = FALSE;
    pMyObject->hParentInsContext    = (ANSC_HANDLE)NULL;
    pMyObject->hParamArray          = (ANSC_HANDLE)NULL;
    pMyObject->bCommitting          = FALSE;

    pMyObject->GetInsContext        = DslhObjcoGetInsContext;
    pMyObject->CheckInstance        = DslhObjcoCheckInstance;
    pMyObject->CheckParamExistence  = DslhObjcoCheckParamExistence;
    pMyObject->CheckParamWritability= DslhObjcoCheckParamWritability;
    pMyObject->ValidateHierarchyInterface
                                    = DslhObjcoValidateHierarchyInterface;
    pMyObject->Reset                = DslhObjcoReset;
    pMyObject->GetParamValueByName  = NULL;

    pMyObject->Engage               = DslhObjcoEngage;
    pMyObject->Validate             = DslhObjcoValidate;
    pMyObject->Commit               = DslhObjcoCommit;
    pMyObject->Rollback             = DslhObjcoRollback;
    pMyObject->GetParamValueByName  = DslhObjcoGetParamValueByName;
    pMyObject->GetBulkParamValue    = DslhObjcoGetBulkParamValue;

    return  ANSC_STATUS_SUCCESS;
}
