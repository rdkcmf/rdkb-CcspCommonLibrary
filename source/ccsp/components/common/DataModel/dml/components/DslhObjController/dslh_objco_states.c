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

    module: dslh_objco_states.c

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Dslh Obj Controller Object.

        *   DslhObjcoGetInsContext
        *   DslhObjcoCheckInstance
        *   DslhObjcoCheckParamExistence
        *   DslhObjcoCheckParamWritability
        *   DslhObjcoValidateHierarchyInterface
        *   DslhObjcoReset

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        10/03/05    initial revision.
        12/14/06    Bin got rid of unnecessary Get/Set apis;

**********************************************************************/

#include "dslh_objco_global.h"

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        DslhObjcoGetInsContext
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the Instance Context Handle.
        Most time the "hInsContext" will be returned, but sometime the
        objController has to rewrite it.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     the handle;

**********************************************************************/

ANSC_HANDLE
DslhObjcoGetInsContext
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_OBJ_CONTROLLER_OBJECT     pMyObject    = (PDSLH_OBJ_CONTROLLER_OBJECT)hThisObject;

    return pMyObject->hInsContext;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        DslhObjcoCheckInstance
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        If the instance's existence is dynamic, the ObjController has the
        opportunity to let us know by rewriting this api.
        If it returns FALSE, this object won't be populated.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:  true or false;

**********************************************************************/
BOOL
DslhObjcoCheckInstance
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_OBJ_CONTROLLER_OBJECT     pMyObject    = (PDSLH_OBJ_CONTROLLER_OBJECT)hThisObject;
    PDSLH_TR69_INTERFACE            pInterface   = (PDSLH_TR69_INTERFACE)pMyObject->hDslhTr69If;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord   = (PDSLH_OBJ_RECORD_OBJECT)pMyObject->hDslhObjRecord;

    if( pInterface == NULL)
    {
        return TRUE;
    }

    if( pInterface->bUseFullName)
    {
        if( pInterface->Methods.MethodWithFullName.CheckInstance != NULL)
        {
            return pInterface->Methods.MethodWithFullName.CheckInstance(pMyObject->hInsContext, pObjRecord->FullName);
        }
    }
    else
    {
        if( pInterface->Methods.MethodWithLastName.CheckInstance != NULL)
        {
            return pInterface->Methods.MethodWithLastName.CheckInstance(pMyObject->hInsContext);
        }
    }

    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        DslhObjcoCheckParamExistence
            (
                ANSC_HANDLE                 hThisObject,
                PCHAR                       pParamName
            );

    description:

        If certain parameters' existence is dynamic, the ObjController has the
        opportunity to let us know by rewriting this api.
        If it returns FALSE, this parameter won't be populated.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:  true or false;

**********************************************************************/
BOOL
DslhObjcoCheckParamExistence
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pParamName
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(pParamName);
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        DslhObjcoCheckParamWritability
            (
                ANSC_HANDLE                 hThisObject,
                PCHAR                       pParamName
            );

    description:

        If certain parameters' writability is dynamic, the ObjController has the
        opportunity to let us know by rewriting this api.
        "DSLH_OBJCONTROLLER_PARAM_WRITE_NOTCHANGE" will be returned by default and 
        the writability in entity's descriptor will be used.
        If the returned value is other than that, the returned access will be used.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PCHAR                       pParamName,
                The parameter name

    return:  the writability value

**********************************************************************/
ULONG
DslhObjcoCheckParamWritability
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pParamName
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(pParamName);
    return DSLH_OBJCONTROLLER_PARAM_WRITE_NOTCHANGE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        DslhObjcoValidateHierarchyInterface
            (
                ANSC_HANDLE                 hThisObject,
                PCHAR                       pHierarchName,
                ULONG                       uType
            );

    description:

        This function validates a full hierarchical name with certain type. 

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PCHAR                       pHierarchName,
                The full hierarchical name of the interface

                ULONG                       uType
                The interface type

    return:  true or false;

**********************************************************************/
BOOL
DslhObjcoValidateHierarchyInterface
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pHierarchName,
        ULONG                       uType
    )
{
    UNREFERENCED_PARAMETER(pHierarchName);
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(uType);
    /*PDSLH_OBJ_CONTROLLER_OBJECT     pMyObject			= (PDSLH_OBJ_CONTROLLER_OBJECT)hThisObject;*/
	/*PDSLH_CPE_CONTROLLER_OBJECT		pDslhCpeController	= (PDSLH_CPE_CONTROLLER_OBJECT)pMyObject->hDslhCpeController;*/

    return TRUE/*pDslhCpeController->ValidateHierarchyInterface(pDslhCpeController, pHierarchName, uType)*/;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhObjcoReset
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to reset object states.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhObjcoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_OBJ_CONTROLLER_OBJECT     pMyObject    = (PDSLH_OBJ_CONTROLLER_OBJECT)hThisObject;

    pMyObject->hParentInsContext = (ANSC_HANDLE)NULL;
    pMyObject->hInsContext       = (ANSC_HANDLE)NULL;

    return  ANSC_STATUS_SUCCESS;
}
