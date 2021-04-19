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

    module:	slap_eco_uoaif.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced interface functions
        of the Slap Env Controller Object.

        *   SlapEcoUoaCreateContainer
        *   SlapEcoUoaDeleteContainer
        *   SlapEcoUoaAcqContainerAccess
        *   SlapEcoUoaRelContainerAccess
        *   SlapEcoUoaCreateObject
        *   SlapEcoUoaDeleteObject
        *   SlapEcoUoaDeleteObject2
        *   SlapEcoUoaInvokeObject
        *   SlapEcoUoaGetObjInsContext
        *   SlapEcoUoaSetObjInsContext
        *   SlapEcoUoaGetObjCallOption
        *   SlapEcoUoaAcqObjectAccess
        *   SlapEcoUoaRelObjectAccess
        *   SlapEcoUoaEnrollObjDescriptor
        *   SlapEcoUoaUnbindObjDescriptor
        *   SlapEcoUoaVerifyObjDescriptor
        *   SlapEcoUoaCreateProxyObject
        *   SlapEcoUoaEnrollProxyObject
        *   SlapEcoUoaUnbindProxyObject
        *   SlapEcoUoaVerifyProxyObject

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/04/03    initial revision.

**********************************************************************/


#include "slap_eco_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapEcoUoaCreateContainer
            (
                ANSC_HANDLE                 hThisObject,
                char*                       container_name,
                ULONG                       container_type
            );

    description:

        This function is called to create a Obj Container.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       container_name
                Specifies the name of the container object to be
                created.

                ULONG                       container_type
                Specifies the type of the container object to be
                created.

    return:     obj container.

**********************************************************************/

ANSC_HANDLE
SlapEcoUoaCreateContainer
    (
        ANSC_HANDLE                 hThisObject,
        char*                       container_name,
        ULONG                       container_type
    )
{
    PSLAP_ENV_CONTROLLER_OBJECT     pMyObject         = (PSLAP_ENV_CONTROLLER_OBJECT)hThisObject;
    PSLAP_OBJ_CONTAINER_OBJECT      pSlapObjContainer = (PSLAP_OBJ_CONTAINER_OBJECT )NULL;

    if ( !container_name )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pSlapObjContainer =
            (PSLAP_OBJ_CONTAINER_OBJECT)SlapCreateObjContainer
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );
    }

    if ( !pSlapObjContainer )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pSlapObjContainer->SetSlapUoaIf    ((ANSC_HANDLE)pSlapObjContainer, pMyObject->hSlapUoaIf    );
        pSlapObjContainer->SetSlapBssIf    ((ANSC_HANDLE)pSlapObjContainer, pMyObject->hSlapBssIf    );
        pSlapObjContainer->SetSlapObjMapper((ANSC_HANDLE)pSlapObjContainer, pMyObject->hSlapObjMapper);
        pSlapObjContainer->SetContainerName((ANSC_HANDLE)pSlapObjContainer, container_name           );
        pSlapObjContainer->SetContainerType((ANSC_HANDLE)pSlapObjContainer, container_type           );
    }

    if ( TRUE )
    {
        pMyObject->AddSlapContainer((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSlapObjContainer);
    }

    return  (ANSC_HANDLE)pSlapObjContainer;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapEcoUoaDeleteContainer
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSlapContainer
            );

    description:

        This function is called to delete a Obj Container.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSlapContainer
                Specifies the container object returned by the previous
                CreateContainer() call.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapEcoUoaDeleteContainer
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapContainer
    )
{
    PSLAP_ENV_CONTROLLER_OBJECT     pMyObject         = (PSLAP_ENV_CONTROLLER_OBJECT)hThisObject;
    PSLAP_OBJ_CONTAINER_OBJECT      pSlapObjContainer = (PSLAP_OBJ_CONTAINER_OBJECT )hSlapContainer;

    if ( !pSlapObjContainer )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    if ( TRUE )
    {
        pMyObject->DelSlapContainer2((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSlapObjContainer);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapEcoUoaAcqContainerAccess
            (
                ANSC_HANDLE                 hThisObject,
                char*                       container_name
            );

    description:

        This function is called to acquire access to a runtime
        container Object by matching the container_name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       container_name
                Specifies the container name to be matched.

    return:     object handle.

**********************************************************************/

ANSC_HANDLE
SlapEcoUoaAcqContainerAccess
    (
        ANSC_HANDLE                 hThisObject,
        char*                       container_name
    )
{
    PSLAP_ENV_CONTROLLER_OBJECT     pMyObject         = (PSLAP_ENV_CONTROLLER_OBJECT)hThisObject;
    PSLAP_OBJ_CONTAINER_OBJECT      pSlapObjContainer = (PSLAP_OBJ_CONTAINER_OBJECT )NULL;

    if ( !container_name )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pSlapObjContainer =
            (PSLAP_OBJ_CONTAINER_OBJECT)pMyObject->GetSlapContainer 
                (
                    (ANSC_HANDLE)pMyObject,
                    container_name
                );
    }

    if ( !pSlapObjContainer )
    {
        return  (ANSC_HANDLE)NULL;
    }

    return  (ANSC_HANDLE)pSlapObjContainer;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapEcoUoaRelContainerAccess
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSlapContainer
            );

    description:

        This function is called to release access to a runtime
        container Object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSlapContainer
                Specifies the slap container object returned by the
                previous CreateContainer() or AcqContainerAccess() call.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapEcoUoaRelContainerAccess
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapContainer
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hSlapContainer);
    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapEcoUoaCreateObject
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSlapContainer,
                char*                       obj_name,
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to create a runtime Obj Record Object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSlapContainer
                Specifies the container object returned by the previous
                CreateContainer() call.

                char*                       obj_name
                Specifies the object name to be created.

                ANSC_HANDLE                 hInsContext
                Specifies the runtime instance context to be associated
                with the newly created object.

    return:     object handle.

**********************************************************************/

ANSC_HANDLE
SlapEcoUoaCreateObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapContainer,
        char*                       obj_name,
        ANSC_HANDLE                 hInsContext
    )
{
    PSLAP_ENV_CONTROLLER_OBJECT     pMyObject         = (PSLAP_ENV_CONTROLLER_OBJECT)hThisObject;
    PSLAP_OBJ_MAPPER_OBJECT         pSlapObjMapper    = (PSLAP_OBJ_MAPPER_OBJECT    )pMyObject->hSlapObjMapper;
    PSLAP_OBJ_CONTAINER_OBJECT      pSlapObjContainer = (PSLAP_OBJ_CONTAINER_OBJECT )hSlapContainer;
    PSLAP_OBJ_ENTITY_OBJECT         pSlapObjEntity    = (PSLAP_OBJ_ENTITY_OBJECT    )NULL;
    PSLAP_OBJ_RECORD_OBJECT         pSlapObjRecord    = (PSLAP_OBJ_RECORD_OBJECT    )NULL;

    if ( !obj_name || (AnscSizeOfString(obj_name) == 0) )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else if ( !pSlapObjContainer )
    {
        pSlapObjContainer = (PSLAP_OBJ_CONTAINER_OBJECT)pMyObject->hSlapDefContainer;

        if ( !pSlapObjContainer )
        {
            return  (ANSC_HANDLE)NULL;
        }
    }

    pSlapObjEntity =
        (PSLAP_OBJ_ENTITY_OBJECT)pSlapObjMapper->GetObjEntity1
            (
                (ANSC_HANDLE)pSlapObjMapper,
                obj_name
            );

    if ( !pSlapObjEntity )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pSlapObjRecord =
            (PSLAP_OBJ_RECORD_OBJECT)pSlapObjEntity->NewObjRecord
                (
                    (ANSC_HANDLE)pSlapObjEntity,
                    (ANSC_HANDLE)pSlapObjContainer,
                    hInsContext
                );
    }

    if ( !pSlapObjRecord )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pSlapObjRecord->IncRef   ((ANSC_HANDLE)pSlapObjRecord);
        pSlapObjRecord->RelAccess((ANSC_HANDLE)pSlapObjRecord);
    }

    return  (ANSC_HANDLE)pSlapObjRecord;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapEcoUoaDeleteObject
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSlapObject
            );

    description:

        This function is called to delete a runtime Obj Record Object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSlapObject
                Specifies the slap obj record object returned by the
                previous CreateObject() or AcqObjectAccess() call.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapEcoUoaDeleteObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject
    )
{
   UNREFERENCED_PARAMETER(hThisObject);
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PSLAP_OBJ_CONTAINER_OBJECT      pSlapObjContainer = (PSLAP_OBJ_CONTAINER_OBJECT )NULL;
    PSLAP_OBJ_ENTITY_OBJECT         pSlapObjEntity    = (PSLAP_OBJ_ENTITY_OBJECT    )NULL;
    PSLAP_OBJ_RECORD_OBJECT         pSlapObjRecord    = (PSLAP_OBJ_RECORD_OBJECT    )hSlapObject;
    PSLAP_SRV_COMPONENT_OBJECT      pSlapSrvComponent = (PSLAP_SRV_COMPONENT_OBJECT )NULL;

    if ( !pSlapObjRecord )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        pSlapObjContainer = (PSLAP_OBJ_CONTAINER_OBJECT)pSlapObjRecord->hSlapObjContainer;
        pSlapObjEntity    = (PSLAP_OBJ_ENTITY_OBJECT   )pSlapObjRecord->hSlapObjEntity;
        pSlapSrvComponent = (PSLAP_SRV_COMPONENT_OBJECT)pSlapObjRecord->hSlapSrvComponent;
    }

    if ( !pSlapObjEntity )
    {
        pSlapObjRecord->Remove((ANSC_HANDLE)pSlapObjRecord);

        return  ANSC_STATUS_INTERNAL_ERROR;
    }
    else
    {
        if ( pSlapObjRecord->GetRef((ANSC_HANDLE)pSlapObjRecord) > 0 )
        {
            pSlapObjRecord->AcqAccess((ANSC_HANDLE)pSlapObjRecord);
            pSlapObjRecord->DecRef   ((ANSC_HANDLE)pSlapObjRecord);
            pSlapObjRecord->RelAccess((ANSC_HANDLE)pSlapObjRecord);
        }

        returnStatus =
            pSlapObjEntity->DelObjRecord
                (
                    (ANSC_HANDLE)pSlapObjEntity,
                    (ANSC_HANDLE)pSlapObjContainer,
                    (ANSC_HANDLE)pSlapSrvComponent,
                    FALSE
                );
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapEcoUoaDeleteObject2
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSlapObject
            );

    description:

        This function is called to delete a runtime Obj Record Object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSlapObject
                Specifies the slap obj record object returned by the
                previous CreateObject() or AcqObjectAccess() call.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapEcoUoaDeleteObject2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PSLAP_OBJ_CONTAINER_OBJECT      pSlapObjContainer = (PSLAP_OBJ_CONTAINER_OBJECT )NULL;
    PSLAP_OBJ_ENTITY_OBJECT         pSlapObjEntity    = (PSLAP_OBJ_ENTITY_OBJECT    )NULL;
    PSLAP_OBJ_RECORD_OBJECT         pSlapObjRecord    = (PSLAP_OBJ_RECORD_OBJECT    )hSlapObject;
    PSLAP_SRV_COMPONENT_OBJECT      pSlapSrvComponent = (PSLAP_SRV_COMPONENT_OBJECT )NULL;

    if ( !pSlapObjRecord )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        pSlapObjContainer = (PSLAP_OBJ_CONTAINER_OBJECT)pSlapObjRecord->hSlapObjContainer;
        pSlapObjEntity    = (PSLAP_OBJ_ENTITY_OBJECT   )pSlapObjRecord->hSlapObjEntity;
        pSlapSrvComponent = (PSLAP_SRV_COMPONENT_OBJECT)pSlapObjRecord->hSlapSrvComponent;
    }

    if ( !pSlapObjEntity )
    {
        pSlapObjRecord->Remove((ANSC_HANDLE)pSlapObjRecord);

        return  ANSC_STATUS_INTERNAL_ERROR;
    }
    else
    {
        returnStatus =
            pSlapObjEntity->DelObjRecord
                (
                    (ANSC_HANDLE)pSlapObjEntity,
                    (ANSC_HANDLE)pSlapObjContainer,
                    (ANSC_HANDLE)pSlapSrvComponent,
                    TRUE
                );
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapEcoUoaInvokeObject
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSlapObject,
                char*                       method_name,
                SLAP_PARAMETER_LIST*        params_in,
                SLAP_PARAMETER_LIST**       params_out,
                SLAP_VARIABLE**             return_var
            );

    description:

        This function is called to invoke a runtime Srv Component
        Object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSlapObject
                Specifies the slap obj record object returned by the
                previous CreateObject() or AcqObjectAccess() call.

                char*                       method_name
                Specifies the object method to be invoked.

                SLAP_PARAMETER_LIST*        params_in
                Specifies the list of input parameters to be passed.

                SLAP_PARAMETER_LIST**       params_out
                Specifies the list of output parameters to be returned.

                SLAP_VARIABLE**             return_var
                Specifies the variable to be returned.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapEcoUoaInvokeObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject,
        char*                       method_name,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST**       params_out,
        SLAP_VARIABLE**             return_var
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PSLAP_ENV_CONTROLLER_OBJECT     pMyObject         = (PSLAP_ENV_CONTROLLER_OBJECT)hThisObject;
    PSLAP_OBJ_RECORD_OBJECT         pSlapObjRecord    = (PSLAP_OBJ_RECORD_OBJECT    )hSlapObject;
    PSLAP_SRV_COMPONENT_OBJECT      pSlapSrvComponent = (PSLAP_SRV_COMPONENT_OBJECT )pSlapObjRecord->hSlapSrvComponent;
    ULONG                           ulObjType         = (ULONG                      )0;
    errno_t                         rc                = -1;

    pMyObject->bCallPending         = TRUE;
    pMyObject->PendingCallTimestamp = AnscGetTickInSeconds();

    rc = strcpy_s(pMyObject->PendingCallName, sizeof(pMyObject->PendingCallName), method_name);
    ERR_CHK(rc);

    /*
     * There're certain object methods should not be simply relayed to the target logic object. For
     * example, the external modules may try to invoke method "Remove" to delete the target object.
     * Handling of such methods should be taken care by UOA interface.
     */
    if ( strcasecmp(method_name, "Remove") == 0 &&
         (params_in             != NULL              ) &&
         (params_in->ParamCount == 0                 ) )
    {
        returnStatus =
            pMyObject->UoaDeleteObject
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pSlapObjRecord
                );

        pMyObject->bCallPending = FALSE;

        return  returnStatus;
    }

    if ( !pSlapSrvComponent )
    {
        pMyObject->bCallPending = FALSE;

        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        ulObjType = pSlapSrvComponent->ObjType;

        if ( ulObjType & SLAP_OBJ_TYPE_serializedCalls )
        {
            pSlapObjRecord->AcqAccess((ANSC_HANDLE)pSlapObjRecord);
        }

        returnStatus =
            pSlapSrvComponent->InvokeDispatch
                (
                    (ANSC_HANDLE)pSlapSrvComponent,
                    method_name,
                    params_in,
                    params_out,
                    return_var
                );

        if ( ulObjType & SLAP_OBJ_TYPE_serializedCalls )
        {
            pSlapObjRecord->RelAccess((ANSC_HANDLE)pSlapObjRecord);
        }
    }

    pMyObject->bCallPending = FALSE;

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapEcoUoaGetObjInsContext
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSlapObject
            );

    description:

        This function is called to retrieve the ins_context associated
        with the SLAP object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSlapObject
                Specifies the slap obj record object returned by the
                previous CreateObject() or AcqObjectAccess() call.

    return:     ins context.

**********************************************************************/

ANSC_HANDLE
SlapEcoUoaGetObjInsContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_OBJ_RECORD_OBJECT         pSlapObjRecord    = (PSLAP_OBJ_RECORD_OBJECT    )hSlapObject;
    PSLAP_SRV_COMPONENT_OBJECT      pSlapSrvComponent = (PSLAP_SRV_COMPONENT_OBJECT )pSlapObjRecord->hSlapSrvComponent;

    return  pSlapSrvComponent->hInsContext;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapEcoUoaSetObjInsContext
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSlapObject,
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to configure the ins_context associated
        with the SLAP object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSlapObject
                Specifies the slap obj record object returned by the
                previous CreateObject() or AcqObjectAccess() call.

                ANSC_HANDLE                 hInsContext
                Specifies the ins context to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapEcoUoaSetObjInsContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject,
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_OBJ_RECORD_OBJECT         pSlapObjRecord    = (PSLAP_OBJ_RECORD_OBJECT    )hSlapObject;
    PSLAP_SRV_COMPONENT_OBJECT      pSlapSrvComponent = (PSLAP_SRV_COMPONENT_OBJECT )pSlapObjRecord->hSlapSrvComponent;

    pSlapSrvComponent->hInsContext = hInsContext;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SlapEcoUoaGetObjCallOption
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSlapObject,
                char*                       method_name,
                SLAP_PARAMETER_LIST*        params_in
            );

    description:

        This function is called to retrieve the call option associated
        with the specified method of the SLAP object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSlapObject
                Specifies the slap obj record object returned by the
                previous CreateObject() or AcqObjectAccess() call.

                char*                       method_name
                Specifies the method whose call option will be returned.

                SLAP_PARAMETER_LIST*        params_in
                Specifies the input parameter list for the method.

    return:     call option.

**********************************************************************/

ULONG
SlapEcoUoaGetObjCallOption
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject,
        char*                       method_name,
        SLAP_PARAMETER_LIST*        params_in
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_OBJ_RECORD_OBJECT         pSlapObjRecord    = (PSLAP_OBJ_RECORD_OBJECT    )hSlapObject;
    PSLAP_OBJ_ENTITY_OBJECT         pSlapObjEntity    = (PSLAP_OBJ_ENTITY_OBJECT    )pSlapObjRecord->hSlapObjEntity;

    if ( !pSlapObjEntity )
    {
        return  0;
    }

    return  pSlapObjEntity->ResolveCallOption
                (
                    (ANSC_HANDLE)pSlapObjEntity,
                    method_name,
                    params_in
                );
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapEcoUoaAcqObjectAccess
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSlapContainer,
                char*                       obj_name
            );

    description:

        This function is called to acquire access to a runtime Obj
        Record Object by matching the obj_name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSlapContainer
                Specifies the container object returned by the previous
                CreateContainer() call.

                char*                       obj_name
                Specifies the object name to be matched.

    return:     object handle.

**********************************************************************/

ANSC_HANDLE
SlapEcoUoaAcqObjectAccess
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapContainer,
        char*                       obj_name
    )
{
    PSLAP_ENV_CONTROLLER_OBJECT     pMyObject         = (PSLAP_ENV_CONTROLLER_OBJECT)hThisObject;
    PSLAP_OBJ_MAPPER_OBJECT         pSlapObjMapper    = (PSLAP_OBJ_MAPPER_OBJECT    )pMyObject->hSlapObjMapper;
    PSLAP_OBJ_CONTAINER_OBJECT      pSlapObjContainer = (PSLAP_OBJ_CONTAINER_OBJECT )hSlapContainer;
    PSLAP_OBJ_ENTITY_OBJECT         pSlapObjEntity    = (PSLAP_OBJ_ENTITY_OBJECT    )NULL;
    PSLAP_OBJ_RECORD_OBJECT         pSlapObjRecord    = (PSLAP_OBJ_RECORD_OBJECT    )NULL;

    if ( !obj_name )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else if ( !pSlapObjContainer )
    {
        pSlapObjContainer = (PSLAP_OBJ_CONTAINER_OBJECT)pMyObject->hSlapDefContainer;

        if ( !pSlapObjContainer )
        {
            return  (ANSC_HANDLE)NULL;
        }
    }

    pSlapObjEntity =
        (PSLAP_OBJ_ENTITY_OBJECT)pSlapObjMapper->GetObjEntity1
            (
                (ANSC_HANDLE)pSlapObjMapper,
                obj_name
            );

    if ( !pSlapObjEntity )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pSlapObjRecord =
            (PSLAP_OBJ_RECORD_OBJECT)pSlapObjEntity->GetObjRecord
                (
                    (ANSC_HANDLE)pSlapObjEntity,
                    (ANSC_HANDLE)pSlapObjContainer,
                    0
                );
    }

    if ( !pSlapObjRecord )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pSlapObjRecord->IncRef   ((ANSC_HANDLE)pSlapObjRecord);
        pSlapObjRecord->RelAccess((ANSC_HANDLE)pSlapObjRecord);
    }

    return  (ANSC_HANDLE)pSlapObjRecord;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapEcoUoaRelObjectAccess
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSlapObject
            );

    description:

        This function is called to release access to a runtime Obj
        Record Object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSlapObject
                Specifies the slap obj record object returned by the
                previous CreateObject() or AcqObjectAccess() call.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapEcoUoaRelObjectAccess
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_OBJ_RECORD_OBJECT         pSlapObjRecord    = (PSLAP_OBJ_RECORD_OBJECT    )hSlapObject;

    pSlapObjRecord->AcqAccess((ANSC_HANDLE)pSlapObjRecord);
    pSlapObjRecord->DecRef   ((ANSC_HANDLE)pSlapObjRecord);
    pSlapObjRecord->RelAccess((ANSC_HANDLE)pSlapObjRecord);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapEcoUoaEnrollObjDescriptor
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hObjDescr
            );

    description:

        This function is called to register a object descriptor.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hObjDescr
                Specifies the object descriptor to be registered.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapEcoUoaEnrollObjDescriptor
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjDescr
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PSLAP_ENV_CONTROLLER_OBJECT     pMyObject      = (PSLAP_ENV_CONTROLLER_OBJECT)hThisObject;
    PSLAP_OBJ_MAPPER_OBJECT         pSlapObjMapper = (PSLAP_OBJ_MAPPER_OBJECT    )pMyObject->hSlapObjMapper;
    PSLAP_OBJECT_DESCRIPTOR         pObjDescriptor = (PSLAP_OBJECT_DESCRIPTOR    )hObjDescr;
    PSLAP_OBJ_ENTITY_OBJECT         pSlapObjEntity = (PSLAP_OBJ_ENTITY_OBJECT    )NULL;

    pSlapObjEntity =
        (PSLAP_OBJ_ENTITY_OBJECT)SlapCreateObjEntity
            (
                pMyObject->hContainerContext,
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)NULL
            );

    if ( !pSlapObjEntity )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pSlapObjEntity->hSlapUoaIf = pMyObject->hSlapUoaIf;
        pSlapObjEntity->hSlapBssIf = pMyObject->hSlapBssIf;

        pSlapObjEntity->SetObjDescriptor((ANSC_HANDLE)pSlapObjEntity, (ANSC_HANDLE)pObjDescriptor);
    }

    returnStatus =
        pSlapObjMapper->AddObjEntity
            (
                (ANSC_HANDLE)pSlapObjMapper,
                pObjDescriptor->ObjName,
                (ANSC_HANDLE)pSlapObjEntity
            );

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        pSlapObjEntity->Remove((ANSC_HANDLE)pSlapObjEntity);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapEcoUoaUnbindObjDescriptor
            (
                ANSC_HANDLE                 hThisObject,
                char*                       obj_name
            );

    description:

        This function is called to unbind a object descriptor.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       obj_name
                Specifies the obj_name to be unbound.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapEcoUoaUnbindObjDescriptor
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PSLAP_ENV_CONTROLLER_OBJECT     pMyObject      = (PSLAP_ENV_CONTROLLER_OBJECT)hThisObject;
    PSLAP_OBJ_MAPPER_OBJECT         pSlapObjMapper = (PSLAP_OBJ_MAPPER_OBJECT    )pMyObject->hSlapObjMapper;

    returnStatus =
        pSlapObjMapper->DelObjEntity
            (
                (ANSC_HANDLE)pSlapObjMapper,
                obj_name,
                TRUE
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SlapEcoUoaVerifyObjDescriptor
            (
                ANSC_HANDLE                 hThisObject,
                char*                       obj_name
            );

    description:

        This function is called to verify a object descriptor.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       obj_name
                Specifies the obj_name to be unbound.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
SlapEcoUoaVerifyObjDescriptor
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    )
{
    PSLAP_ENV_CONTROLLER_OBJECT     pMyObject      = (PSLAP_ENV_CONTROLLER_OBJECT)hThisObject;
    PSLAP_OBJ_MAPPER_OBJECT         pSlapObjMapper = (PSLAP_OBJ_MAPPER_OBJECT    )pMyObject->hSlapObjMapper;
    PSLAP_OBJ_ENTITY_OBJECT         pSlapObjEntity = (PSLAP_OBJ_ENTITY_OBJECT    )NULL;

    pSlapObjEntity =
        (PSLAP_OBJ_ENTITY_OBJECT)pSlapObjMapper->GetObjEntity1
            (
                (ANSC_HANDLE)pSlapObjMapper,
                obj_name
            );

    if ( !pSlapObjEntity )
    {
        return  FALSE;
    }

    return  TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapEcoUoaCreateProxyObject
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSlapContainer,
                char*                       obj_name,
                ANSC_HANDLE                 hSlapGoaObj
            );

    description:

        This function is called to create a runtime Srv Proxy Object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSlapContainer
                Specifies the container object returned by the previous
                CreateContainer() call.

                char*                       obj_name
                Specifies the object name to be created.

                ANSC_HANDLE                 hSlapGoaObj
                Specifies the runtime instance context to be associated
                with the newly created object.

    return:     object handle.

**********************************************************************/

ANSC_HANDLE
SlapEcoUoaCreateProxyObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapContainer,
        char*                       obj_name,
        ANSC_HANDLE                 hSlapGoaObj
    )
{
    UNREFERENCED_PARAMETER(hSlapContainer);
    PSLAP_ENV_CONTROLLER_OBJECT     pMyObject         = (PSLAP_ENV_CONTROLLER_OBJECT)hThisObject;
    PSLAP_OBJ_MAPPER_OBJECT         pSlapObjMapper    = (PSLAP_OBJ_MAPPER_OBJECT    )pMyObject->hSlapObjMapper;
    PSLAP_OBJ_PROXY_OBJECT          pSlapObjProxy     = (PSLAP_OBJ_PROXY_OBJECT     )NULL;
    PSLAP_SRV_PROXY_OBJECT          pSlapSrvProxy     = (PSLAP_SRV_PROXY_OBJECT     )NULL;

    if ( !obj_name || (AnscSizeOfString(obj_name) == 0) )
    {
        return  (ANSC_HANDLE)NULL;
    }

    pSlapObjProxy =
        (PSLAP_OBJ_PROXY_OBJECT)pSlapObjMapper->GetObjProxy1
            (
                (ANSC_HANDLE)pSlapObjMapper,
                obj_name
            );

    if ( !pSlapObjProxy )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pSlapSrvProxy =
            (PSLAP_SRV_PROXY_OBJECT)pSlapObjProxy->NewSrvProxy
                (
                    (ANSC_HANDLE)pSlapObjProxy,
                    hSlapGoaObj
                );
    }

    if ( !pSlapSrvProxy )
    {
        return  (ANSC_HANDLE)NULL;
    }

    return  (ANSC_HANDLE)pSlapSrvProxy;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapEcoUoaEnrollProxyObject
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hObjDescr
            );

    description:

        This function is called to register a object descriptor.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hObjDescr
                Specifies the object descriptor to be registered.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapEcoUoaEnrollProxyObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjDescr
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PSLAP_ENV_CONTROLLER_OBJECT     pMyObject      = (PSLAP_ENV_CONTROLLER_OBJECT)hThisObject;
    PSLAP_OBJ_MAPPER_OBJECT         pSlapObjMapper = (PSLAP_OBJ_MAPPER_OBJECT    )pMyObject->hSlapObjMapper;
    PSLAP_OBJECT_DESCRIPTOR         pObjDescriptor = (PSLAP_OBJECT_DESCRIPTOR    )hObjDescr;
    PSLAP_OBJ_PROXY_OBJECT          pSlapObjProxy  = (PSLAP_OBJ_PROXY_OBJECT     )NULL;

    pSlapObjProxy =
        (PSLAP_OBJ_PROXY_OBJECT)SlapCreateObjProxy
            (
                pMyObject->hContainerContext,
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)NULL
            );

    if ( !pSlapObjProxy )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pSlapObjProxy->hSlapUoaIf = pMyObject->hSlapUoaIf;

        pSlapObjProxy->SetObjDescriptor((ANSC_HANDLE)pSlapObjProxy, (ANSC_HANDLE)pObjDescriptor);
    }

    returnStatus =
        pSlapObjMapper->AddObjProxy
            (
                (ANSC_HANDLE)pSlapObjMapper,
                pObjDescriptor->ObjName,
                (ANSC_HANDLE)pSlapObjProxy
            );

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        pSlapObjProxy->Remove((ANSC_HANDLE)pSlapObjProxy);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapEcoUoaUnbindProxyObject
            (
                ANSC_HANDLE                 hThisObject,
                char*                       obj_name
            );

    description:

        This function is called to unbind a object descriptor.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       obj_name
                Specifies the obj_name to be unbound.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapEcoUoaUnbindProxyObject
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PSLAP_ENV_CONTROLLER_OBJECT     pMyObject      = (PSLAP_ENV_CONTROLLER_OBJECT)hThisObject;
    PSLAP_OBJ_MAPPER_OBJECT         pSlapObjMapper = (PSLAP_OBJ_MAPPER_OBJECT    )pMyObject->hSlapObjMapper;

    returnStatus =
        pSlapObjMapper->DelObjProxy
            (
                (ANSC_HANDLE)pSlapObjMapper,
                obj_name,
                TRUE
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SlapEcoUoaVerifyProxyObject
            (
                ANSC_HANDLE                 hThisObject,
                char*                       obj_name
            );

    description:

        This function is called to verify a object descriptor.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       obj_name
                Specifies the obj_name to be unbound.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
SlapEcoUoaVerifyProxyObject
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    )
{
    PSLAP_ENV_CONTROLLER_OBJECT     pMyObject      = (PSLAP_ENV_CONTROLLER_OBJECT)hThisObject;
    PSLAP_OBJ_MAPPER_OBJECT         pSlapObjMapper = (PSLAP_OBJ_MAPPER_OBJECT    )pMyObject->hSlapObjMapper;
    PSLAP_OBJ_PROXY_OBJECT          pSlapObjProxy  = (PSLAP_OBJ_PROXY_OBJECT     )NULL;

    pSlapObjProxy =
        (PSLAP_OBJ_PROXY_OBJECT)pSlapObjMapper->GetObjProxy1
            (
                (ANSC_HANDLE)pSlapObjMapper,
                obj_name
            );

    if ( !pSlapObjProxy )
    {
        return  FALSE;
    }

    return  TRUE;
}
