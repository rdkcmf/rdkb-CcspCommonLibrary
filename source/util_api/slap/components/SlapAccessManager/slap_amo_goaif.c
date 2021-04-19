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

    module:	slap_amo_goaif.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced interface functions
        of the Slap Access Manager Object.

        *   SlapAmoGoaGetAggregationMode
        *   SlapAmoGoaSetAggregationMode
        *   SlapAmoGoaGetLpcPartyPort
        *   SlapAmoGoaSetLpcPartyPort
        *   SlapAmoGoaGetLpcPartyAddr
        *   SlapAmoGoaSetLpcPartyAddr
        *   SlapAmoGoaGetLpcManagerAddr
        *   SlapAmoGoaSetLpcManagerAddr
        *   SlapAmoGoaCreateContainer
        *   SlapAmoGoaDeleteContainer
        *   SlapAmoGoaCreateObject
        *   SlapAmoGoaDeleteObject
        *   SlapAmoGoaInvokeObject
        *   SlapAmoGoaInvokeObject2
        *   SlapAmoGoaAttachObject
        *   SlapAmoGoaDetachObject
        *   SlapAmoGoaUnlinkObject
        *   SlapAmoGoaAcqObjectAccess
        *   SlapAmoGoaRelObjectAccess
        *   SlapAmoGoaFlushAllObjects
        *   SlapAmoGoaEnrollObjDescriptor
        *   SlapAmoGoaUnbindObjDescriptor
        *   SlapAmoGoaVerifyObjDescriptor
        *   SlapAmoGoaEnrollMobileObject
        *   SlapAmoGoaUnbindMobileObject
        *   SlapAmoGoaVerifyMobileObject

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/03/03    initial revision.

**********************************************************************/


#include "slap_amo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SlapAmoGoaGetAggregationMode
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve an object property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object property.

**********************************************************************/

ULONG
SlapAmoGoaGetAggregationMode
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_ACCESS_MANAGER_OBJECT     pMyObject          = (PSLAP_ACCESS_MANAGER_OBJECT  )hThisObject;
    PSLAP_ACCESS_MANAGER_PROPERTY   pProperty          = (PSLAP_ACCESS_MANAGER_PROPERTY)&pMyObject->Property;

    return  pProperty->AggregationMode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapAmoGoaSetAggregationMode
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       mode
            );

    description:

        This function is called to configure an object property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       mode
                Specifies the object property to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapAmoGoaSetAggregationMode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       mode
    )
{
    PSLAP_ACCESS_MANAGER_OBJECT     pMyObject          = (PSLAP_ACCESS_MANAGER_OBJECT  )hThisObject;
    PSLAP_ACCESS_MANAGER_PROPERTY   pProperty          = (PSLAP_ACCESS_MANAGER_PROPERTY)&pMyObject->Property;

    pProperty->AggregationMode = mode;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        USHORT
        SlapAmoGoaGetLpcPartyPort
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve an object property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object property.

**********************************************************************/

USHORT
SlapAmoGoaGetLpcPartyPort
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_ACCESS_MANAGER_OBJECT     pMyObject          = (PSLAP_ACCESS_MANAGER_OBJECT  )hThisObject;
    PSLAP_ACCESS_MANAGER_PROPERTY   pProperty          = (PSLAP_ACCESS_MANAGER_PROPERTY)&pMyObject->Property;

    return  pProperty->LpcPartyPort;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapAmoGoaSetLpcPartyPort
            (
                ANSC_HANDLE                 hThisObject,
                USHORT                      port
            );

    description:

        This function is called to configure an object property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                USHORT                      port
                Specifies the object property to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapAmoGoaSetLpcPartyPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      port
    )
{
    PSLAP_ACCESS_MANAGER_OBJECT     pMyObject          = (PSLAP_ACCESS_MANAGER_OBJECT  )hThisObject;
    PSLAP_ACCESS_MANAGER_PROPERTY   pProperty          = (PSLAP_ACCESS_MANAGER_PROPERTY)&pMyObject->Property;

    pProperty->LpcPartyPort = port;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SlapAmoGoaGetLpcPartyAddr
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve an object property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object property.

**********************************************************************/

ULONG
SlapAmoGoaGetLpcPartyAddr
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_ACCESS_MANAGER_OBJECT     pMyObject          = (PSLAP_ACCESS_MANAGER_OBJECT  )hThisObject;
    PSLAP_ACCESS_MANAGER_PROPERTY   pProperty          = (PSLAP_ACCESS_MANAGER_PROPERTY)&pMyObject->Property;

    return  pProperty->MyAddress.Value;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapAmoGoaSetLpcPartyAddr
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ipaddr
            );

    description:

        This function is called to configure an object property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                USHORT                      port
                Specifies the object property to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapAmoGoaSetLpcPartyAddr
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ipaddr
    )
{
    PSLAP_ACCESS_MANAGER_OBJECT     pMyObject          = (PSLAP_ACCESS_MANAGER_OBJECT  )hThisObject;
    PSLAP_ACCESS_MANAGER_PROPERTY   pProperty          = (PSLAP_ACCESS_MANAGER_PROPERTY)&pMyObject->Property;

    pProperty->MyAddress.Value = ipaddr;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SlapAmoGoaGetLpcManagerAddr
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve an object property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object property.

**********************************************************************/

ULONG
SlapAmoGoaGetLpcManagerAddr
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_ACCESS_MANAGER_OBJECT     pMyObject          = (PSLAP_ACCESS_MANAGER_OBJECT  )hThisObject;
    PSLAP_ACCESS_MANAGER_PROPERTY   pProperty          = (PSLAP_ACCESS_MANAGER_PROPERTY)&pMyObject->Property;

    return  pProperty->ManagerAddress.Value;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapAmoGoaSetLpcManagerAddr
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ipaddr
            );

    description:

        This function is called to configure an object property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                USHORT                      port
                Specifies the object property to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapAmoGoaSetLpcManagerAddr
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ipaddr
    )
{
    PSLAP_ACCESS_MANAGER_OBJECT     pMyObject          = (PSLAP_ACCESS_MANAGER_OBJECT  )hThisObject;
    PSLAP_ACCESS_MANAGER_PROPERTY   pProperty          = (PSLAP_ACCESS_MANAGER_PROPERTY)&pMyObject->Property;

    pProperty->ManagerAddress.Value = ipaddr;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapAmoGoaCreateContainer
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
SlapAmoGoaCreateContainer
    (
        ANSC_HANDLE                 hThisObject,
        char*                       container_name,
        ULONG                       container_type
    )
{
    PSLAP_ACCESS_MANAGER_OBJECT     pMyObject          = (PSLAP_ACCESS_MANAGER_OBJECT  )hThisObject;
    PSLAP_ACCESS_MANAGER_PROPERTY   pProperty          = (PSLAP_ACCESS_MANAGER_PROPERTY)&pMyObject->Property;
    PSLAP_ENV_CONTROLLER_OBJECT     pSlapEnvController = (PSLAP_ENV_CONTROLLER_OBJECT  )pMyObject->hSlapEnvController;
    PSLAP_UOA_INTERFACE             pSlapUoaIf         = (PSLAP_UOA_INTERFACE          )NULL;
    ANSC_HANDLE                     hSlapContainer     = (ANSC_HANDLE                  )NULL;
    PSLAP_OBJ_WRAPPER_OBJECT        pSlapObjWrapper    = (PSLAP_OBJ_WRAPPER_OBJECT     )NULL;

    if ( !pSlapEnvController || !(pProperty->AggregationMode & SLAP_GOA_MODE_thisProcess) )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pSlapUoaIf = (PSLAP_UOA_INTERFACE)pSlapEnvController->GetSlapUoaIf((ANSC_HANDLE)pSlapEnvController);
    }

    hSlapContainer =
        pSlapUoaIf->CreateContainer
            (
                pSlapUoaIf->hOwnerContext,
                container_name,
                container_type
            );

    if ( !hSlapContainer )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pSlapObjWrapper =
            (PSLAP_OBJ_WRAPPER_OBJECT)SlapCreateOwoUoac
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );
    }

    if ( !pSlapObjWrapper )
    {
        pSlapUoaIf->DeleteContainer
                (
                    pSlapUoaIf->hOwnerContext,
                    hSlapContainer
                );

        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pSlapObjWrapper->ObjType       = SLAP_OWO_OBJ_TYPE_uoaContainer;
        pSlapObjWrapper->ObjMode       = SLAP_OWO_OBJ_MODE_created;
        pSlapObjWrapper->hWrappedObj   = hSlapContainer;
        pSlapObjWrapper->hGoaContainer = (ANSC_HANDLE)NULL;
        pSlapObjWrapper->hSlapGoaIf    = pMyObject->hSlapGoaIf;

        pSlapObjWrapper->SetObjName     ((ANSC_HANDLE)pSlapObjWrapper, container_name   );
        pSlapObjWrapper->SetLpcPartyAddr((ANSC_HANDLE)pSlapObjWrapper, (ANSC_HANDLE)NULL);
        pSlapObjWrapper->SetLpcPartyName((ANSC_HANDLE)pSlapObjWrapper, NULL             );
    }

    return  (ANSC_HANDLE)pSlapObjWrapper;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapAmoGoaDeleteContainer
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
SlapAmoGoaDeleteContainer
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapContainer
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PSLAP_ACCESS_MANAGER_OBJECT     pMyObject          = (PSLAP_ACCESS_MANAGER_OBJECT  )hThisObject;
    PSLAP_ACCESS_MANAGER_PROPERTY   pProperty          = (PSLAP_ACCESS_MANAGER_PROPERTY)&pMyObject->Property;
    PSLAP_ENV_CONTROLLER_OBJECT     pSlapEnvController = (PSLAP_ENV_CONTROLLER_OBJECT  )pMyObject->hSlapEnvController;
    PSLAP_UOA_INTERFACE             pSlapUoaIf         = (PSLAP_UOA_INTERFACE          )NULL;
    PSLAP_OBJ_WRAPPER_OBJECT        pSlapObjWrapper    = (PSLAP_OBJ_WRAPPER_OBJECT     )hSlapContainer;
    ANSC_HANDLE                     hSlapWrappedObj    = (ANSC_HANDLE                  )NULL;

    if ( !pSlapEnvController || !(pProperty->AggregationMode & SLAP_GOA_MODE_thisProcess) )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        pSlapUoaIf = (PSLAP_UOA_INTERFACE)pSlapEnvController->GetSlapUoaIf((ANSC_HANDLE)pSlapEnvController);
    }

    if ( !pSlapObjWrapper )
    {
        return  ANSC_STATUS_BAD_PARAMETER;
    }
    else if ( pSlapObjWrapper->ObjType != SLAP_OWO_OBJ_TYPE_uoaContainer )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        hSlapWrappedObj = pSlapObjWrapper->hWrappedObj;
    }

    pSlapObjWrapper->Remove((ANSC_HANDLE)pSlapObjWrapper);

    returnStatus =
        pSlapUoaIf->DeleteContainer
            (
                pSlapUoaIf->hOwnerContext,
                hSlapWrappedObj
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapAmoGoaCreateObject
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
SlapAmoGoaCreateObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapContainer,
        char*                       obj_name,
        ANSC_HANDLE                 hInsContext
    )
{
    PSLAP_ACCESS_MANAGER_OBJECT     pMyObject          = (PSLAP_ACCESS_MANAGER_OBJECT  )hThisObject;
    PSLAP_ACCESS_MANAGER_PROPERTY   pProperty          = (PSLAP_ACCESS_MANAGER_PROPERTY)&pMyObject->Property;
    PSLAP_ENV_CONTROLLER_OBJECT     pSlapEnvController = (PSLAP_ENV_CONTROLLER_OBJECT  )pMyObject->hSlapEnvController;
    PSLAP_UOA_INTERFACE             pSlapUoaIf         = (PSLAP_UOA_INTERFACE          )NULL;
    ANSC_HANDLE                     hSlapObject        = (ANSC_HANDLE                  )NULL;
    PSLAP_OBJ_WRAPPER_OBJECT        pSlapObjWrapperC   = (PSLAP_OBJ_WRAPPER_OBJECT     )hSlapContainer;
    PSLAP_OBJ_WRAPPER_OBJECT        pSlapObjWrapperO   = (PSLAP_OBJ_WRAPPER_OBJECT     )NULL;

    if ( !pSlapEnvController || !(pProperty->AggregationMode & SLAP_GOA_MODE_thisProcess) )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pSlapUoaIf = (PSLAP_UOA_INTERFACE)pSlapEnvController->GetSlapUoaIf((ANSC_HANDLE)pSlapEnvController);
    }

    if ( !pSlapObjWrapperC )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else if ( pSlapObjWrapperC->ObjType != SLAP_OWO_OBJ_TYPE_uoaContainer ) /*RDKB-6242, CID-24346, NULL check before use*/
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        hSlapObject =
            pSlapUoaIf->CreateObject
                (
                    pSlapUoaIf->hOwnerContext,
                    pSlapObjWrapperC->hWrappedObj,
                    obj_name,
                    hInsContext
                );
    }

    if ( !hSlapObject )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pSlapObjWrapperO =
            (PSLAP_OBJ_WRAPPER_OBJECT)SlapCreateOwoUoao
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );
    }

    if ( !pSlapObjWrapperO )
    {
        pSlapUoaIf->DeleteObject
                (
                    pSlapUoaIf->hOwnerContext,
                    hSlapObject
                );

        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pSlapObjWrapperO->ObjType       = SLAP_OWO_OBJ_TYPE_uoaObject;
        pSlapObjWrapperO->ObjMode       = SLAP_OWO_OBJ_MODE_created;
        pSlapObjWrapperO->hWrappedObj   = hSlapObject;
        pSlapObjWrapperO->hGoaContainer = (ANSC_HANDLE)NULL;
        pSlapObjWrapperO->hSlapGoaIf    = pMyObject->hSlapGoaIf;

        pSlapObjWrapperO->SetObjName     ((ANSC_HANDLE)pSlapObjWrapperO, obj_name         );
        pSlapObjWrapperO->SetLpcPartyAddr((ANSC_HANDLE)pSlapObjWrapperO, (ANSC_HANDLE)NULL);
        pSlapObjWrapperO->SetLpcPartyName((ANSC_HANDLE)pSlapObjWrapperO, NULL             );
    }

    pMyObject->GoaAttachObject
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pSlapObjWrapperO,
                (ANSC_HANDLE)pSlapObjWrapperC
            );

    return  (ANSC_HANDLE)pSlapObjWrapperO;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapAmoGoaDeleteObject
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
SlapAmoGoaDeleteObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PSLAP_ACCESS_MANAGER_OBJECT     pMyObject          = (PSLAP_ACCESS_MANAGER_OBJECT  )hThisObject;
    PSLAP_ACCESS_MANAGER_PROPERTY   pProperty          = (PSLAP_ACCESS_MANAGER_PROPERTY)&pMyObject->Property;
    PSLAP_ENV_CONTROLLER_OBJECT     pSlapEnvController = (PSLAP_ENV_CONTROLLER_OBJECT  )pMyObject->hSlapEnvController;
    PSLAP_LOAM_CLIENT_OBJECT        pSlapLoamClient    = (PSLAP_LOAM_CLIENT_OBJECT     )pMyObject->hSlapLoamClient;
    PSLAP_UOA_INTERFACE             pSlapUoaIf         = (PSLAP_UOA_INTERFACE          )NULL;
    PSLAP_LOA_INTERFACE             pSlapLoaIf         = (PSLAP_LOA_INTERFACE          )NULL;
    PSLAP_OBJ_WRAPPER_OBJECT        pSlapObjWrapper    = (PSLAP_OBJ_WRAPPER_OBJECT     )hSlapObject;
    ULONG                           ulOwoObjType       = (ULONG                        )SLAP_OWO_OBJ_TYPE_uoaObject;

    if ( !pSlapObjWrapper )
    {
        return  ANSC_STATUS_BAD_PARAMETER;
    }
    else
    {
        ulOwoObjType = pSlapObjWrapper->ObjType;
    }

    if ( (ulOwoObjType != SLAP_OWO_OBJ_TYPE_uoaObject) &&
         (ulOwoObjType != SLAP_OWO_OBJ_TYPE_loaObject) )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else if ( ulOwoObjType == SLAP_OWO_OBJ_TYPE_uoaObject )
    {
        if ( !pSlapEnvController || !(pProperty->AggregationMode & SLAP_GOA_MODE_thisProcess) )
        {
            return  ANSC_STATUS_UNAPPLICABLE;
        }
        else
        {
            pSlapUoaIf = (PSLAP_UOA_INTERFACE)pSlapEnvController->GetSlapUoaIf((ANSC_HANDLE)pSlapEnvController);
        }

        returnStatus =
            pSlapUoaIf->DeleteObject
                (
                    pSlapUoaIf->hOwnerContext,
                    pSlapObjWrapper->hWrappedObj
                );
    }
    else
    {
        if ( !pSlapLoamClient || !(pProperty->AggregationMode & SLAP_GOA_MODE_imcpClient) )
        {
            return  ANSC_STATUS_UNAPPLICABLE;
        }
        else
        {
            pSlapLoaIf = (PSLAP_LOA_INTERFACE)pSlapLoamClient->GetSlapLoaIf((ANSC_HANDLE)pSlapLoamClient);
        }

        returnStatus =
            pSlapLoaIf->DeleteObject
                (
                    pSlapLoaIf->hOwnerContext,
                    pSlapObjWrapper->LpcPartyAddr,
                    pSlapObjWrapper->hWrappedObj
                );
    }

    if ( TRUE )
    {
        pMyObject->GoaDetachObject
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pSlapObjWrapper
            );

        pSlapObjWrapper->Remove((ANSC_HANDLE)pSlapObjWrapper);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapAmoGoaInvokeObject
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSlapObject,
                ANSC_HANDLE                 hSlapGoaRtc,
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

                ANSC_HANDLE                 hSlapGoaRtc
                Specifies the slap GOA runtime container to which all
                runtime objects will be attached. The caller may set
                this parameter to NULL if it doesn't want SLAP_GAO to
                handle life-cycle management for SLAP objects.

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
SlapAmoGoaInvokeObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject,
        ANSC_HANDLE                 hSlapGoaRtc,
        char*                       method_name,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST**       params_out,
        SLAP_VARIABLE**             return_var
    )
{
    PSLAP_ACCESS_MANAGER_OBJECT     pMyObject    = (PSLAP_ACCESS_MANAGER_OBJECT  )hThisObject;

    return  pMyObject->GoaInvokeObject2
                (
                    (ANSC_HANDLE)pMyObject,
                    hSlapObject,
                    hSlapGoaRtc,
                    method_name,
                    params_in,
                    params_out,
                    return_var,
                    0                       /* using the timeout value */
                );
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapAmoGoaInvokeObject2
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSlapObject,
                ANSC_HANDLE                 hSlapGoaRtc,
                char*                       method_name,
                SLAP_PARAMETER_LIST*        params_in,
                SLAP_PARAMETER_LIST**       params_out,
                SLAP_VARIABLE**             return_var,
                ULONG                       ulTimeout
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

                ANSC_HANDLE                 hSlapGoaRtc
                Specifies the slap GOA runtime container to which all
                runtime objects will be attached. The caller may set
                this parameter to NULL if it doesn't want SLAP_GAO to
                handle life-cycle management for SLAP objects.

                char*                       method_name
                Specifies the object method to be invoked.

                SLAP_PARAMETER_LIST*        params_in
                Specifies the list of input parameters to be passed.

                SLAP_PARAMETER_LIST**       params_out
                Specifies the list of output parameters to be returned.

                SLAP_VARIABLE**             return_var
                Specifies the variable to be returned.

                ULONG                       ulTimeout
                Specifies the timeout value for this call.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapAmoGoaInvokeObject2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject,
        ANSC_HANDLE                 hSlapGoaRtc,
        char*                       method_name,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST**       params_out,
        SLAP_VARIABLE**             return_var,
        ULONG                       ulTimeout
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PSLAP_ACCESS_MANAGER_OBJECT     pMyObject          = (PSLAP_ACCESS_MANAGER_OBJECT  )hThisObject;
    PSLAP_ACCESS_MANAGER_PROPERTY   pProperty          = (PSLAP_ACCESS_MANAGER_PROPERTY)&pMyObject->Property;
    PSLAP_ENV_CONTROLLER_OBJECT     pSlapEnvController = (PSLAP_ENV_CONTROLLER_OBJECT  )pMyObject->hSlapEnvController;
    PSLAP_LOAM_CLIENT_OBJECT        pSlapLoamClient    = (PSLAP_LOAM_CLIENT_OBJECT     )pMyObject->hSlapLoamClient;
    PSLAP_UOA_INTERFACE             pSlapUoaIf         = (PSLAP_UOA_INTERFACE          )NULL;
    PSLAP_LOA_INTERFACE             pSlapLoaIf         = (PSLAP_LOA_INTERFACE          )NULL;
    PSLAP_OBJ_WRAPPER_OBJECT        pSlapObjWrapper    = (PSLAP_OBJ_WRAPPER_OBJECT     )hSlapObject;
    ULONG                           ulOwoObjType       = (ULONG                        )SLAP_OWO_OBJ_TYPE_uoaObject;
    ULONG                           ulCallOptions      = (ULONG                        )0;

    /*
     * There're certain object methods should not be simply relayed to the target logic object. For
     * example, the external modules may try to invoke method "Remove" to delete the target object.
     * Handling of such methods should be taken care by GOA interface.
     */
    if ( strcasecmp(method_name, "Remove") == 0 &&
         (params_in             != NULL              ) &&
         (params_in->ParamCount == 0                 ) )
    {
        returnStatus =
            pMyObject->GoaDeleteObject
                (
                    (ANSC_HANDLE)pMyObject,
                    hSlapObject
                );

        return  returnStatus;
    }

    if ( !pSlapObjWrapper )
    {
        return  ANSC_STATUS_BAD_PARAMETER;
    }
    else
    {
        ulOwoObjType = pSlapObjWrapper->ObjType;
    }

    if ( (ulOwoObjType != SLAP_OWO_OBJ_TYPE_uoaObject) &&
         (ulOwoObjType != SLAP_OWO_OBJ_TYPE_loaObject) )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else if ( ulOwoObjType == SLAP_OWO_OBJ_TYPE_uoaObject )
    {
        if ( !pSlapEnvController || !(pProperty->AggregationMode & SLAP_GOA_MODE_thisProcess) )
        {
            return  ANSC_STATUS_UNAPPLICABLE;
        }
        else
        {
            pSlapUoaIf    = (PSLAP_UOA_INTERFACE)pSlapEnvController->GetSlapUoaIf((ANSC_HANDLE)pSlapEnvController);
            ulCallOptions =
                pSlapUoaIf->GetObjCallOption
                    (
                        pSlapUoaIf->hOwnerContext,
                        pSlapObjWrapper->hWrappedObj,
                        method_name,
                        params_in
                    );
        }

        if ( !(ulCallOptions & SLAP_CALL_OPTION_skipGoaTranslation) )
        {
            if ( params_in )
            {
                returnStatus =
                    pMyObject->GoaToUoaParamList
                        (
                            (ANSC_HANDLE)pMyObject,
                            params_in
                        );

                if ( returnStatus != ANSC_STATUS_SUCCESS )
                {
                    return  returnStatus;
                }
            }
        }

        returnStatus =
            pSlapUoaIf->InvokeObject
                (
                    pSlapUoaIf->hOwnerContext,
                    pSlapObjWrapper->hWrappedObj,
                    method_name,
                    params_in,
                    params_out,
                    return_var
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            return  returnStatus;
        }
        else if ( !(ulCallOptions & SLAP_CALL_OPTION_skipGoaTranslation) )
        {
            if ( params_out && *params_out )
            {
                returnStatus =
                    pMyObject->UoaToGoaParamList
                        (
                            (ANSC_HANDLE)pMyObject,
                            *params_out,
                            hSlapGoaRtc
                        );

                if ( returnStatus != ANSC_STATUS_SUCCESS )
                {
                    return  returnStatus;
                }
            }

            if ( return_var && *return_var )
            {
                returnStatus =
                    pMyObject->UoaToGoaVariable
                        (
                            (ANSC_HANDLE)pMyObject,
                            *return_var,
                            hSlapGoaRtc
                        );

                if ( returnStatus != ANSC_STATUS_SUCCESS )
                {
                    return  returnStatus;
                }
            }
        }
    }
    else
    {
        if ( !pSlapLoamClient || !(pProperty->AggregationMode & SLAP_GOA_MODE_imcpClient) )
        {
            return  ANSC_STATUS_UNAPPLICABLE;
        }
        else
        {
            pSlapLoaIf = (PSLAP_LOA_INTERFACE)pSlapLoamClient->GetSlapLoaIf((ANSC_HANDLE)pSlapLoamClient);
        }

        if ( params_in )
        {
            returnStatus =
                pMyObject->GoaToLoaParamList
                    (
                        (ANSC_HANDLE)pMyObject,
                        params_in
                    );

            if ( returnStatus != ANSC_STATUS_SUCCESS )
            {
                return  returnStatus;
            }
        }

        returnStatus =
            pSlapLoaIf->InvokeObject
                (
                    pSlapLoaIf->hOwnerContext,
                    pSlapObjWrapper->LpcPartyAddr,
                    pSlapObjWrapper->hWrappedObj,
                    method_name,
                    params_in,
                    params_out,
                    return_var,
                    TRUE,
                    ulTimeout
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            return  returnStatus;
        }
        else
        {
            if ( params_out && *params_out )
            {
                returnStatus =
                    pMyObject->LoaToGoaParamList
                        (
                            (ANSC_HANDLE)pMyObject,
                            pSlapObjWrapper->LpcPartyAddr,
                            *params_out,
                            hSlapGoaRtc
                        );

                if ( returnStatus != ANSC_STATUS_SUCCESS )
                {
                    return  returnStatus;
                }
            }

            if ( return_var && *return_var )
            {
                returnStatus =
                    pMyObject->LoaToGoaVariable
                        (
                            (ANSC_HANDLE)pMyObject,
                            pSlapObjWrapper->LpcPartyAddr,
                            *return_var,
                            hSlapGoaRtc
                        );

                if ( returnStatus != ANSC_STATUS_SUCCESS )
                {
                    return  returnStatus;
                }
            }
        }
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapAmoGoaAttachObject
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSlapObject,
                ANSC_HANDLE                 hSlapContainer
            );

    description:

        This function is called to attach a GOA object to the specified
        GOA container.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSlapObject
                Specifies the GOA object that is to be attached.

                ANSC_HANDLE                 hSlapContainer
                Specifies the GOA container to which the object is to
                be attached.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapAmoGoaAttachObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject,
        ANSC_HANDLE                 hSlapContainer
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PSLAP_ACCESS_MANAGER_OBJECT     pMyObject          = (PSLAP_ACCESS_MANAGER_OBJECT  )hThisObject;
    PSLAP_OBJ_WRAPPER_OBJECT        pSlapObjWrapperO   = (PSLAP_OBJ_WRAPPER_OBJECT     )hSlapObject;
    PSLAP_OBJ_WRAPPER_OBJECT        pSlapObjWrapperC   = (PSLAP_OBJ_WRAPPER_OBJECT     )hSlapContainer;
    PSLAP_OWO_UOAC_OBJECT           pSlapOwoUoac       = (PSLAP_OWO_UOAC_OBJECT        )pSlapObjWrapperC;

    if ( !pSlapObjWrapperO || !pSlapObjWrapperC )
    {
        return  ANSC_STATUS_BAD_PARAMETER;
    }
    else if ( pSlapObjWrapperC->ObjType != SLAP_OWO_OBJ_TYPE_uoaContainer )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else if ( pSlapObjWrapperO->ObjMode != SLAP_OWO_OBJ_MODE_created )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        returnStatus =
            pMyObject->GoaDetachObject
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pSlapObjWrapperO
                );
    }

    returnStatus =
        pSlapOwoUoac->AttachObjWrapper
            (
                (ANSC_HANDLE)pSlapOwoUoac,
                (ANSC_HANDLE)pSlapObjWrapperO
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapAmoGoaDetachObject
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSlapObject
            );

    description:

        This function is called to detach a GOA object from its
        associated GOA container.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSlapObject
                Specifies the GOA object that is to be detached.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapAmoGoaDetachObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PSLAP_OBJ_WRAPPER_OBJECT        pSlapObjWrapperO   = (PSLAP_OBJ_WRAPPER_OBJECT     )hSlapObject;
    PSLAP_OBJ_WRAPPER_OBJECT        pSlapObjWrapperC   = (PSLAP_OBJ_WRAPPER_OBJECT     )NULL;
    PSLAP_OWO_UOAC_OBJECT           pSlapOwoUoac       = (PSLAP_OWO_UOAC_OBJECT        )NULL;

    if ( !pSlapObjWrapperO )
    {
        return  ANSC_STATUS_BAD_PARAMETER;
    }
    else
    {
        pSlapObjWrapperC = (PSLAP_OBJ_WRAPPER_OBJECT)pSlapObjWrapperO->hGoaContainer;
        pSlapOwoUoac     = (PSLAP_OWO_UOAC_OBJECT   )pSlapObjWrapperC;
    }

    if ( !pSlapObjWrapperC ||
         (pSlapObjWrapperC->ObjType != SLAP_OWO_OBJ_TYPE_uoaContainer) )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    returnStatus =
        pSlapOwoUoac->DetachObjWrapper
            (
                (ANSC_HANDLE)pSlapOwoUoac,
                (ANSC_HANDLE)pSlapObjWrapperO
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapAmoGoaUnlinkObject
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSlapObject
            );

    description:

        This function is called to unlink a GOA object from its
        associated GOA container.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSlapObject
                Specifies the GOA object that is to be unlinked.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapAmoGoaUnlinkObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PSLAP_OBJ_WRAPPER_OBJECT        pSlapObjWrapperO   = (PSLAP_OBJ_WRAPPER_OBJECT     )hSlapObject;
    PSLAP_OBJ_WRAPPER_OBJECT        pSlapObjWrapperC   = (PSLAP_OBJ_WRAPPER_OBJECT     )NULL;
    PSLAP_OWO_UOAC_OBJECT           pSlapOwoUoac       = (PSLAP_OWO_UOAC_OBJECT        )NULL;

    if ( !pSlapObjWrapperO )
    {
        return  ANSC_STATUS_BAD_PARAMETER;
    }
    else
    {
        pSlapObjWrapperC = (PSLAP_OBJ_WRAPPER_OBJECT)pSlapObjWrapperO->hGoaContainer;
        pSlapOwoUoac     = (PSLAP_OWO_UOAC_OBJECT   )pSlapObjWrapperC;
    }

    if ( !pSlapObjWrapperC ||
         (pSlapObjWrapperC->ObjType != SLAP_OWO_OBJ_TYPE_uoaContainer) )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    returnStatus =
        pSlapOwoUoac->RemoveObjWrapper
            (
                (ANSC_HANDLE)pSlapOwoUoac,
                (ANSC_HANDLE)pSlapObjWrapperO
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapAmoGoaAcqObjectAccess
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
SlapAmoGoaAcqObjectAccess
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapContainer,
        char*                       obj_name
    )
{
    #ifdef  _ANSC_SLAP_LPC_
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    char*                           pLpcPartyName      = (char*                        )NULL;
    #endif
    PSLAP_ACCESS_MANAGER_OBJECT     pMyObject          = (PSLAP_ACCESS_MANAGER_OBJECT  )hThisObject;
    PSLAP_ACCESS_MANAGER_PROPERTY   pProperty          = (PSLAP_ACCESS_MANAGER_PROPERTY)&pMyObject->Property;
    PSLAP_ENV_CONTROLLER_OBJECT     pSlapEnvController = (PSLAP_ENV_CONTROLLER_OBJECT  )pMyObject->hSlapEnvController;
    PSLAP_UOA_INTERFACE             pSlapUoaIf         = (PSLAP_UOA_INTERFACE          )NULL;
    PSLAP_OBJ_WRAPPER_OBJECT        pSlapObjWrapperC   = (PSLAP_OBJ_WRAPPER_OBJECT     )hSlapContainer;
    PSLAP_OBJ_WRAPPER_OBJECT        pSlapObjWrapperO   = (PSLAP_OBJ_WRAPPER_OBJECT     )NULL;
    ANSC_HANDLE                     hSlapObject        = (ANSC_HANDLE                  )NULL;
    ULONG                           ulOwoObjType       = (ULONG                        )SLAP_OWO_OBJ_TYPE_uoaContainer;

    /*
     * The SLAP_GOA interface greatly simplifies the access to any SLAP object, whether it's owned
     * by this process or any other local process. But doing so is at the price of giving up some
     * of the flexibitilies offered by the SLAP_LOA interface. For example, SLAP_GOA doesn't allow
     * upper layer to acquire access to a remote SLAP container while SLAP_LOA supports it. Such
     * behavior is intentional because SLAP_GOA tries to hide the complexity associated with the
     * container management.
     */
    if ( !pSlapObjWrapperC )
    {
        if ( pSlapEnvController && (pProperty->AggregationMode & SLAP_GOA_MODE_thisProcess) )
        {
            pSlapUoaIf  = (PSLAP_UOA_INTERFACE)pSlapEnvController->GetSlapUoaIf((ANSC_HANDLE)pSlapEnvController);
            hSlapObject =
                pSlapUoaIf->AcqObjectAccess
                    (
                        pSlapUoaIf->hOwnerContext,
                        (ANSC_HANDLE)NULL,
                        obj_name
                    );

            if ( hSlapObject )
            {
                pSlapObjWrapperO =
                    (PSLAP_OBJ_WRAPPER_OBJECT)SlapCreateOwoUoao
                        (
                            pMyObject->hContainerContext,
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)NULL
                        );

                if ( !pSlapObjWrapperO )
                {
                    pSlapUoaIf->RelObjectAccess
                            (
                                pSlapUoaIf->hOwnerContext,
                                hSlapObject
                            );

                    return  (ANSC_HANDLE)NULL;
                }
                else
                {
                    pSlapObjWrapperO->ObjType       = SLAP_OWO_OBJ_TYPE_uoaObject;
                    pSlapObjWrapperO->ObjMode       = SLAP_OWO_OBJ_MODE_acquired;
                    pSlapObjWrapperO->hWrappedObj   = hSlapObject;
                    pSlapObjWrapperO->hGoaContainer = (ANSC_HANDLE)NULL;
                    pSlapObjWrapperO->hSlapGoaIf    = pMyObject->hSlapGoaIf;

                    pSlapObjWrapperO->SetObjName     ((ANSC_HANDLE)pSlapObjWrapperO, obj_name         );
                    pSlapObjWrapperO->SetLpcPartyAddr((ANSC_HANDLE)pSlapObjWrapperO, (ANSC_HANDLE)NULL);
                    pSlapObjWrapperO->SetLpcPartyName((ANSC_HANDLE)pSlapObjWrapperO, NULL             );
                }

                return  (ANSC_HANDLE)pSlapObjWrapperO;
            }
        }


        #ifdef  _ANSC_SLAP_LPC_

        if ( pSlapLoamClient && (pProperty->AggregationMode & SLAP_GOA_MODE_imcpClient) )
        {
            pSlapLoaIf   = (PSLAP_LOA_INTERFACE)pSlapLoamClient->GetSlapLoaIf((ANSC_HANDLE)pSlapLoamClient);
            returnStatus =
                pSlapLoaIf->LocateObject
                    (
                        pSlapLoaIf->hOwnerContext,
                        obj_name,
                        &pLpcPartyName,
                        &hLpcPartyAddr
                    );

            if ( returnStatus != ANSC_STATUS_SUCCESS )
            {
                return  (ANSC_HANDLE)NULL;
            }
            else if ( !pLpcPartyName || !hLpcPartyAddr )
            {
                goto  EXIT1;
            }

            hSlapObject =
                pSlapLoaIf->AcqObjectAccess
                    (
                        pSlapLoaIf->hOwnerContext,
                        hLpcPartyAddr,
                        (ANSC_HANDLE)NULL,
                        obj_name
                    );

            if ( hSlapObject )
            {
                pSlapObjWrapperO =
                    (PSLAP_OBJ_WRAPPER_OBJECT)SlapCreateOwoLoao
                        (
                            pMyObject->hContainerContext,
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)NULL
                        );

                if ( !pSlapObjWrapperO )
                {
                    returnStatus =
                        pSlapLoaIf->RelObjectAccess
                            (
                                pSlapLoaIf->hOwnerContext,
                                hLpcPartyAddr,
                                hSlapObject
                            );

                    goto  EXIT1;
                }
                else
                {
                    pSlapObjWrapperO->ObjType       = SLAP_OWO_OBJ_TYPE_loaObject;
                    pSlapObjWrapperO->ObjMode       = SLAP_OWO_OBJ_MODE_acquired;
                    pSlapObjWrapperO->hWrappedObj   = hSlapObject;
                    pSlapObjWrapperO->hGoaContainer = (ANSC_HANDLE)NULL;
                    pSlapObjWrapperO->hSlapGoaIf    = pMyObject->hSlapGoaIf;

                    pSlapObjWrapperO->SetObjName     ((ANSC_HANDLE)pSlapObjWrapperO, obj_name     );
                    pSlapObjWrapperO->SetLpcPartyAddr((ANSC_HANDLE)pSlapObjWrapperO, hLpcPartyAddr);
                    pSlapObjWrapperO->SetLpcPartyName((ANSC_HANDLE)pSlapObjWrapperO, pLpcPartyName);

                    if ( pLpcPartyName )
                    {
                        AnscFreeMemory(pLpcPartyName);
                    }
                }

                return  (ANSC_HANDLE)pSlapObjWrapperO;
            }
        }

        #endif

        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        ulOwoObjType = pSlapObjWrapperC->ObjType;

        if ( (ulOwoObjType != SLAP_OWO_OBJ_TYPE_uoaContainer) &&
             (ulOwoObjType != SLAP_OWO_OBJ_TYPE_loaContainer) )
        {
            return  (ANSC_HANDLE)NULL;
        }
        else if ( ulOwoObjType == SLAP_OWO_OBJ_TYPE_uoaContainer )
        {
            if ( !pSlapEnvController || !(pProperty->AggregationMode & SLAP_GOA_MODE_thisProcess) )
            {
                return  (ANSC_HANDLE)NULL;
            }
            else
            {
                pSlapUoaIf  = (PSLAP_UOA_INTERFACE)pSlapEnvController->GetSlapUoaIf((ANSC_HANDLE)pSlapEnvController);
                hSlapObject =
                    pSlapUoaIf->AcqObjectAccess
                        (
                            pSlapUoaIf->hOwnerContext,
                            pSlapObjWrapperC->hWrappedObj,
                            obj_name
                        );

                if ( hSlapObject )
                {
                    pSlapObjWrapperO =
                        (PSLAP_OBJ_WRAPPER_OBJECT)SlapCreateOwoUoao
                            (
                                pMyObject->hContainerContext,
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)NULL
                            );

                    if ( !pSlapObjWrapperO )
                    {
                        pSlapUoaIf->RelObjectAccess
                                (
                                    pSlapUoaIf->hOwnerContext,
                                    hSlapObject
                                );

                        return  (ANSC_HANDLE)NULL;
                    }
                    else
                    {
                        pSlapObjWrapperO->ObjType       = SLAP_OWO_OBJ_TYPE_uoaObject;
                        pSlapObjWrapperO->ObjMode       = SLAP_OWO_OBJ_MODE_acquired;
                        pSlapObjWrapperO->hWrappedObj   = hSlapObject;
                        pSlapObjWrapperO->hGoaContainer = (ANSC_HANDLE)NULL;
                        pSlapObjWrapperO->hSlapGoaIf    = pMyObject->hSlapGoaIf;

                        pSlapObjWrapperO->SetObjName     ((ANSC_HANDLE)pSlapObjWrapperO, obj_name         );
                        pSlapObjWrapperO->SetLpcPartyAddr((ANSC_HANDLE)pSlapObjWrapperO, (ANSC_HANDLE)NULL);
                        pSlapObjWrapperO->SetLpcPartyName((ANSC_HANDLE)pSlapObjWrapperO, NULL             );
                    }

                    return  (ANSC_HANDLE)pSlapObjWrapperO;
                }
            }

            #ifdef  _ANSC_SLAP_LPC_

            if ( pSlapLoamClient && (pProperty->AggregationMode & SLAP_GOA_MODE_imcpClient) )
            {
                pSlapLoaIf   = (PSLAP_LOA_INTERFACE)pSlapLoamClient->GetSlapLoaIf((ANSC_HANDLE)pSlapLoamClient);
                returnStatus =
                    pSlapLoaIf->LocateObject
                        (
                            pSlapLoaIf->hOwnerContext,
                            obj_name,
                            &pLpcPartyName,
                            &hLpcPartyAddr
                        );

                if ( returnStatus != ANSC_STATUS_SUCCESS )
                {
                    return  (ANSC_HANDLE)NULL;
                }
                else if ( !pLpcPartyName || !hLpcPartyAddr )
                {
                    goto  EXIT1;
                }

                hSlapObject =
                    pSlapLoaIf->AcqObjectAccess
                        (
                            pSlapLoaIf->hOwnerContext,
                            hLpcPartyAddr,
                            (ANSC_HANDLE)NULL,
                            obj_name
                        );

                if ( hSlapObject )
                {
                    pSlapObjWrapperO =
                        (PSLAP_OBJ_WRAPPER_OBJECT)SlapCreateOwoLoao
                            (
                                pMyObject->hContainerContext,
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)NULL
                            );

                    if ( !pSlapObjWrapperO )
                    {
                        returnStatus =
                            pSlapLoaIf->RelObjectAccess
                                (
                                    pSlapLoaIf->hOwnerContext,
                                    hLpcPartyAddr,
                                    hSlapObject
                                );

                        goto  EXIT1;
                    }
                    else
                    {
                        pSlapObjWrapperO->ObjType       = SLAP_OWO_OBJ_TYPE_loaObject;
                        pSlapObjWrapperO->ObjMode       = SLAP_OWO_OBJ_MODE_acquired;
                        pSlapObjWrapperO->hWrappedObj   = hSlapObject;
                        pSlapObjWrapperO->hGoaContainer = (ANSC_HANDLE)NULL;
                        pSlapObjWrapperO->hSlapGoaIf    = pMyObject->hSlapGoaIf;

                        pSlapObjWrapperO->SetObjName     ((ANSC_HANDLE)pSlapObjWrapperO, obj_name     );
                        pSlapObjWrapperO->SetLpcPartyAddr((ANSC_HANDLE)pSlapObjWrapperO, hLpcPartyAddr);
                        pSlapObjWrapperO->SetLpcPartyName((ANSC_HANDLE)pSlapObjWrapperO, pLpcPartyName);

                        if ( pLpcPartyName )
                        {
                            AnscFreeMemory(pLpcPartyName);
                        }
                    }

                    return  (ANSC_HANDLE)pSlapObjWrapperO;
                }
            }

            #endif

            return  (ANSC_HANDLE)NULL;
        }
        else
        {
            #ifdef  _ANSC_SLAP_LPC_

            if ( !pSlapLoamClient || !(pProperty->AggregationMode & SLAP_GOA_MODE_imcpClient) )
            {
                return  (ANSC_HANDLE)NULL;
            }
            else
            {
                pSlapLoaIf    = (PSLAP_LOA_INTERFACE)pSlapLoamClient->GetSlapLoaIf((ANSC_HANDLE)pSlapLoamClient);
                pLpcPartyName = (char*              )pSlapObjWrapperC->LpcPartyName;
                hLpcPartyAddr = (ANSC_HANDLE        )pSlapObjWrapperC->LpcPartyAddr;
            }

            hSlapObject =
                pSlapLoaIf->AcqObjectAccess
                    (
                        pSlapLoaIf->hOwnerContext,
                        hLpcPartyAddr,
                        pSlapObjWrapperC->hWrappedObj,
                        obj_name
                    );

            if ( hSlapObject )
            {
                pSlapObjWrapperO =
                    (PSLAP_OBJ_WRAPPER_OBJECT)SlapCreateOwoLoao
                        (
                            pMyObject->hContainerContext,
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)NULL
                        );

                if ( !pSlapObjWrapperO )
                {
                    returnStatus =
                        pSlapLoaIf->RelObjectAccess
                            (
                                pSlapLoaIf->hOwnerContext,
                                hLpcPartyAddr,
                                hSlapObject
                            );

                    return  (ANSC_HANDLE)NULL;
                }
                else
                {
                    pSlapObjWrapperO->ObjType       = SLAP_OWO_OBJ_TYPE_loaObject;
                    pSlapObjWrapperO->ObjMode       = SLAP_OWO_OBJ_MODE_acquired;
                    pSlapObjWrapperO->hWrappedObj   = hSlapObject;
                    pSlapObjWrapperO->hGoaContainer = (ANSC_HANDLE)NULL;
                    pSlapObjWrapperO->hSlapGoaIf    = pMyObject->hSlapGoaIf;

                    pSlapObjWrapperO->SetObjName     ((ANSC_HANDLE)pSlapObjWrapperO, obj_name     );
                    pSlapObjWrapperO->SetLpcPartyAddr((ANSC_HANDLE)pSlapObjWrapperO, hLpcPartyAddr);
                    pSlapObjWrapperO->SetLpcPartyName((ANSC_HANDLE)pSlapObjWrapperO, pLpcPartyName);
                }

                return  (ANSC_HANDLE)pSlapObjWrapperO;
            }

            #endif
        }

        return  (ANSC_HANDLE)NULL;
    }

    return  (ANSC_HANDLE)NULL;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/
#ifdef  _ANSC_SLAP_LPC_
EXIT1:

    if ( pLpcPartyName )
    {
        AnscFreeMemory(pLpcPartyName);
    }

    return  (ANSC_HANDLE)pSlapObjWrapperO;
#endif
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapAmoGoaRelObjectAccess
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
SlapAmoGoaRelObjectAccess
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PSLAP_ACCESS_MANAGER_OBJECT     pMyObject          = (PSLAP_ACCESS_MANAGER_OBJECT  )hThisObject;
    PSLAP_ACCESS_MANAGER_PROPERTY   pProperty          = (PSLAP_ACCESS_MANAGER_PROPERTY)&pMyObject->Property;
    PSLAP_ENV_CONTROLLER_OBJECT     pSlapEnvController = (PSLAP_ENV_CONTROLLER_OBJECT  )pMyObject->hSlapEnvController;
    PSLAP_UOA_INTERFACE             pSlapUoaIf         = (PSLAP_UOA_INTERFACE          )NULL;
    PSLAP_OBJ_WRAPPER_OBJECT        pSlapObjWrapper    = (PSLAP_OBJ_WRAPPER_OBJECT     )hSlapObject;
    ULONG                           ulOwoObjType       = (ULONG                        )SLAP_OWO_OBJ_TYPE_uoaObject;

    if ( !pSlapObjWrapper )
    {
        return  ANSC_STATUS_BAD_PARAMETER;
    }
    else
    {
        ulOwoObjType = pSlapObjWrapper->ObjType;
    }

    if ( (ulOwoObjType != SLAP_OWO_OBJ_TYPE_uoaObject) &&
         (ulOwoObjType != SLAP_OWO_OBJ_TYPE_loaObject) )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else if ( ulOwoObjType == SLAP_OWO_OBJ_TYPE_uoaObject )
    {
        if ( !pSlapEnvController || !(pProperty->AggregationMode & SLAP_GOA_MODE_thisProcess) )
        {
            return  ANSC_STATUS_UNAPPLICABLE;
        }
        else
        {
            pSlapUoaIf = (PSLAP_UOA_INTERFACE)pSlapEnvController->GetSlapUoaIf((ANSC_HANDLE)pSlapEnvController);
        }

        returnStatus =
            pSlapUoaIf->RelObjectAccess
                (
                    pSlapUoaIf->hOwnerContext,
                    pSlapObjWrapper->hWrappedObj
                );
    }
    else
    {
        #ifdef  _ANSC_SLAP_LPC_

        if ( !pSlapLoamClient || !(pProperty->AggregationMode & SLAP_GOA_MODE_imcpClient) )
        {
            return  ANSC_STATUS_UNAPPLICABLE;
        }
        else
        {
            pSlapLoaIf = (PSLAP_LOA_INTERFACE)pSlapLoamClient->GetSlapLoaIf((ANSC_HANDLE)pSlapLoamClient);
        }

        returnStatus =
            pSlapLoaIf->RelObjectAccess
                (
                    pSlapLoaIf->hOwnerContext,
                    pSlapObjWrapper->LpcPartyAddr,
                    pSlapObjWrapper->hWrappedObj
                );

        #else

        return  ANSC_STATUS_UNAPPLICABLE;

        #endif
    }

    if ( TRUE )
    {
        pMyObject->GoaDetachObject
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pSlapObjWrapper
            );

        pSlapObjWrapper->Remove((ANSC_HANDLE)pSlapObjWrapper);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapAmoGoaFlushAllObjects
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to flush all remote objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapAmoGoaFlushAllObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_ACCESS_MANAGER_OBJECT     pMyObject          = (PSLAP_ACCESS_MANAGER_OBJECT  )hThisObject;
    PSLAP_ACCESS_MANAGER_PROPERTY   pProperty          = (PSLAP_ACCESS_MANAGER_PROPERTY)&pMyObject->Property;
    PSLAP_LOAM_CLIENT_OBJECT        pSlapLoamClient    = (PSLAP_LOAM_CLIENT_OBJECT     )pMyObject->hSlapLoamClient;
    PSLAP_LOA_INTERFACE             pSlapLoaIf         = (PSLAP_LOA_INTERFACE          )NULL;

    if ( !pSlapLoamClient || !(pProperty->AggregationMode & SLAP_GOA_MODE_imcpClient) )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        pSlapLoaIf = (PSLAP_LOA_INTERFACE)pSlapLoamClient->GetSlapLoaIf((ANSC_HANDLE)pSlapLoamClient);
    }

    return  pSlapLoaIf->FlushAllObjects(pSlapLoaIf->hOwnerContext);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapAmoGoaEnrollObjDescriptor
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
SlapAmoGoaEnrollObjDescriptor
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjDescr
    )
{
    PSLAP_ACCESS_MANAGER_OBJECT     pMyObject          = (PSLAP_ACCESS_MANAGER_OBJECT  )hThisObject;
    PSLAP_ACCESS_MANAGER_PROPERTY   pProperty          = (PSLAP_ACCESS_MANAGER_PROPERTY)&pMyObject->Property;
    PSLAP_ENV_CONTROLLER_OBJECT     pSlapEnvController = (PSLAP_ENV_CONTROLLER_OBJECT  )pMyObject->hSlapEnvController;
    PSLAP_UOA_INTERFACE             pSlapUoaIf         = (PSLAP_UOA_INTERFACE          )NULL;

    if ( !pSlapEnvController || !(pProperty->AggregationMode & SLAP_GOA_MODE_thisProcess) )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        pSlapUoaIf = (PSLAP_UOA_INTERFACE)pSlapEnvController->GetSlapUoaIf((ANSC_HANDLE)pSlapEnvController);
    }

    return  pSlapUoaIf->EnrollObjDescriptor(pSlapUoaIf->hOwnerContext, hObjDescr);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapAmoGoaUnbindObjDescriptor
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
SlapAmoGoaUnbindObjDescriptor
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    )
{
    PSLAP_ACCESS_MANAGER_OBJECT     pMyObject          = (PSLAP_ACCESS_MANAGER_OBJECT  )hThisObject;
    PSLAP_ACCESS_MANAGER_PROPERTY   pProperty          = (PSLAP_ACCESS_MANAGER_PROPERTY)&pMyObject->Property;
    PSLAP_ENV_CONTROLLER_OBJECT     pSlapEnvController = (PSLAP_ENV_CONTROLLER_OBJECT  )pMyObject->hSlapEnvController;
    PSLAP_UOA_INTERFACE             pSlapUoaIf         = (PSLAP_UOA_INTERFACE          )NULL;

    if ( !pSlapEnvController || !(pProperty->AggregationMode & SLAP_GOA_MODE_thisProcess) )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        pSlapUoaIf = (PSLAP_UOA_INTERFACE)pSlapEnvController->GetSlapUoaIf((ANSC_HANDLE)pSlapEnvController);
    }

    return  pSlapUoaIf->UnbindObjDescriptor(pSlapUoaIf->hOwnerContext, obj_name);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SlapAmoGoaVerifyObjDescriptor
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
SlapAmoGoaVerifyObjDescriptor
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    )
{
    PSLAP_ACCESS_MANAGER_OBJECT     pMyObject          = (PSLAP_ACCESS_MANAGER_OBJECT  )hThisObject;
    PSLAP_ACCESS_MANAGER_PROPERTY   pProperty          = (PSLAP_ACCESS_MANAGER_PROPERTY)&pMyObject->Property;
    PSLAP_ENV_CONTROLLER_OBJECT     pSlapEnvController = (PSLAP_ENV_CONTROLLER_OBJECT  )pMyObject->hSlapEnvController;
    PSLAP_UOA_INTERFACE             pSlapUoaIf         = (PSLAP_UOA_INTERFACE          )NULL;

    if ( !pSlapEnvController || !(pProperty->AggregationMode & SLAP_GOA_MODE_thisProcess) )
    {
        return  FALSE;
    }
    else
    {
        pSlapUoaIf = (PSLAP_UOA_INTERFACE)pSlapEnvController->GetSlapUoaIf((ANSC_HANDLE)pSlapEnvController);
    }

    return  pSlapUoaIf->VerifyObjDescriptor(pSlapUoaIf->hOwnerContext, obj_name);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapAmoGoaEnrollMobileObject
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
SlapAmoGoaEnrollMobileObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjDescr
    )
{
    PSLAP_ACCESS_MANAGER_OBJECT     pMyObject          = (PSLAP_ACCESS_MANAGER_OBJECT  )hThisObject;
    PSLAP_ACCESS_MANAGER_PROPERTY   pProperty          = (PSLAP_ACCESS_MANAGER_PROPERTY)&pMyObject->Property;
    PSLAP_LOAM_CLIENT_OBJECT        pSlapLoamClient    = (PSLAP_LOAM_CLIENT_OBJECT     )pMyObject->hSlapLoamClient;
    PSLAP_LOA_INTERFACE             pSlapLoaIf         = (PSLAP_LOA_INTERFACE          )NULL;

    if ( !pSlapLoamClient || !(pProperty->AggregationMode & SLAP_GOA_MODE_imcpClient) )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        pSlapLoaIf = (PSLAP_LOA_INTERFACE)pSlapLoamClient->GetSlapLoaIf((ANSC_HANDLE)pSlapLoamClient);
    }

    return  pSlapLoaIf->EnrollMobileObject(pSlapLoaIf->hOwnerContext, hObjDescr);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapAmoGoaUnbindMobileObject
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
SlapAmoGoaUnbindMobileObject
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    )
{
    PSLAP_ACCESS_MANAGER_OBJECT     pMyObject          = (PSLAP_ACCESS_MANAGER_OBJECT  )hThisObject;
    PSLAP_ACCESS_MANAGER_PROPERTY   pProperty          = (PSLAP_ACCESS_MANAGER_PROPERTY)&pMyObject->Property;
    PSLAP_LOAM_CLIENT_OBJECT        pSlapLoamClient    = (PSLAP_LOAM_CLIENT_OBJECT     )pMyObject->hSlapLoamClient;
    PSLAP_LOA_INTERFACE             pSlapLoaIf         = (PSLAP_LOA_INTERFACE          )NULL;

    if ( !pSlapLoamClient || !(pProperty->AggregationMode & SLAP_GOA_MODE_imcpClient) )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        pSlapLoaIf = (PSLAP_LOA_INTERFACE)pSlapLoamClient->GetSlapLoaIf((ANSC_HANDLE)pSlapLoamClient);
    }

    return  pSlapLoaIf->UnbindMobileObject(pSlapLoaIf->hOwnerContext, obj_name);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SlapAmoGoaVerifyMobileObject
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
SlapAmoGoaVerifyMobileObject
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    )
{
    PSLAP_ACCESS_MANAGER_OBJECT     pMyObject          = (PSLAP_ACCESS_MANAGER_OBJECT  )hThisObject;
    PSLAP_ACCESS_MANAGER_PROPERTY   pProperty          = (PSLAP_ACCESS_MANAGER_PROPERTY)&pMyObject->Property;
    PSLAP_LOAM_CLIENT_OBJECT        pSlapLoamClient    = (PSLAP_LOAM_CLIENT_OBJECT     )pMyObject->hSlapLoamClient;
    PSLAP_LOA_INTERFACE             pSlapLoaIf         = (PSLAP_LOA_INTERFACE          )NULL;

    if ( !pSlapLoamClient || !(pProperty->AggregationMode & SLAP_GOA_MODE_imcpClient) )
    {
        return  FALSE;
    }
    else
    {
        pSlapLoaIf = (PSLAP_LOA_INTERFACE)pSlapLoamClient->GetSlapLoaIf((ANSC_HANDLE)pSlapLoamClient);
    }

    return  pSlapLoaIf->VerifyMobileObject(pSlapLoaIf->hOwnerContext, obj_name);
}
