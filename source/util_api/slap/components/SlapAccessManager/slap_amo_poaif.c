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

    module:	slap_amo_poaif.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced interface functions
        of the Slap Access Manager Object.

        *   SlapAmoPoaAcqObjectAccess
        *   SlapAmoPoaRelObjectAccess
        *   SlapAmoPoaCreateProxyObject
        *   SlapAmoPoaEnrollProxyObject
        *   SlapAmoPoaUnbindProxyObject
        *   SlapAmoPoaVerifyProxyObject

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        10/06/03    initial revision.

**********************************************************************/


#include "slap_amo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapAmoPoaAcqObjectAccess
            (
                ANSC_HANDLE                 hThisObject,
                char*                       obj_name
            );

    description:

        This function is called to acquire access to a runtime Srv
        Proxy Object by matching the obj_name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       obj_name
                Specifies the object name to be matched.

    return:     object handle.

**********************************************************************/

ANSC_HANDLE
SlapAmoPoaAcqObjectAccess
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    )
{
    PSLAP_ACCESS_MANAGER_OBJECT     pMyObject          = (PSLAP_ACCESS_MANAGER_OBJECT  )hThisObject;
    PSLAP_GOA_INTERFACE             pSlapGoaIf         = (PSLAP_GOA_INTERFACE          )pMyObject->hSlapGoaIf;
    PSLAP_POA_INTERFACE             pSlapPoaIf         = (PSLAP_POA_INTERFACE          )pMyObject->hSlapPoaIf;
    PSLAP_ENV_CONTROLLER_OBJECT     pSlapEnvController = (PSLAP_ENV_CONTROLLER_OBJECT  )pMyObject->hSlapEnvController;
    PSLAP_UOA_INTERFACE             pSlapUoaIf         = (PSLAP_UOA_INTERFACE          )pSlapEnvController->GetSlapUoaIf((ANSC_HANDLE)pSlapEnvController);
    ANSC_HANDLE                     hSlapGoaObject     = (ANSC_HANDLE                  )NULL;
    PSLAP_SRV_PROXY_OBJECT          pSlapSrvProxy      = (PSLAP_SRV_PROXY_OBJECT       )NULL;

    hSlapGoaObject =
        pSlapGoaIf->AcqObjectAccess
            (
                pSlapGoaIf->hOwnerContext,
                (ANSC_HANDLE)NULL,
                obj_name
            );

    if ( !hSlapGoaObject )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pSlapSrvProxy =
            (PSLAP_SRV_PROXY_OBJECT)pSlapUoaIf->CreateProxyObject
                (
                    pSlapUoaIf->hOwnerContext,
                    (ANSC_HANDLE)NULL,
                    obj_name,
                    hSlapGoaObject
                );
    }

    if ( !pSlapSrvProxy )
    {
        pSlapGoaIf->RelObjectAccess
                (
                    pSlapGoaIf->hOwnerContext,
                    hSlapGoaObject
                );

        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pSlapSrvProxy->SetObjMode  ((ANSC_HANDLE)pSlapSrvProxy, SLAP_SPO_OBJ_MODE_acquired);
        pSlapSrvProxy->SetSlapGoaIf((ANSC_HANDLE)pSlapSrvProxy, (ANSC_HANDLE)pSlapGoaIf   );
        pSlapSrvProxy->SetSlapPoaIf((ANSC_HANDLE)pSlapSrvProxy, (ANSC_HANDLE)pSlapPoaIf   );
    }

    return  (ANSC_HANDLE)pSlapSrvProxy;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapAmoPoaRelObjectAccess
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSlapObject
            );

    description:

        This function is called to release access to a runtime Srv
        Proxy Object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSlapObject
                Specifies the slap srv proxy object returned by the
                previous AcqObjectAccess() call.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapAmoPoaRelObjectAccess
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_SRV_PROXY_OBJECT          pSlapSrvProxy      = (PSLAP_SRV_PROXY_OBJECT       )hSlapObject;

    if ( !pSlapSrvProxy )
    {
        return  ANSC_STATUS_BAD_PARAMETER;
    }
    else
    {
        pSlapSrvProxy->Remove((ANSC_HANDLE)pSlapSrvProxy);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapAmoPoaCreateProxyObject
            (
                ANSC_HANDLE                 hThisObject,
                char*                       obj_name,
                ANSC_HANDLE                 hSlapGoaObj
            );

    description:

        This function is called to create a runtime Srv Proxy Object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       obj_name
                Specifies the object name to be created.

                ANSC_HANDLE                 hSlapGoaObj
                Specifies the runtime instance context to be associated
                with the newly created object.

    return:     object handle.

**********************************************************************/

ANSC_HANDLE
SlapAmoPoaCreateProxyObject
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name,
        ANSC_HANDLE                 hSlapGoaObj
    )
{
    PSLAP_ACCESS_MANAGER_OBJECT     pMyObject          = (PSLAP_ACCESS_MANAGER_OBJECT  )hThisObject;
    PSLAP_GOA_INTERFACE             pSlapGoaIf         = (PSLAP_GOA_INTERFACE          )pMyObject->hSlapGoaIf;
    PSLAP_POA_INTERFACE             pSlapPoaIf         = (PSLAP_POA_INTERFACE          )pMyObject->hSlapPoaIf;
    PSLAP_ENV_CONTROLLER_OBJECT     pSlapEnvController = (PSLAP_ENV_CONTROLLER_OBJECT  )pMyObject->hSlapEnvController;
    PSLAP_UOA_INTERFACE             pSlapUoaIf         = (PSLAP_UOA_INTERFACE          )pSlapEnvController->GetSlapUoaIf((ANSC_HANDLE)pSlapEnvController);
    PSLAP_SRV_PROXY_OBJECT          pSlapSrvProxy      = (PSLAP_SRV_PROXY_OBJECT       )NULL;

    pSlapSrvProxy =
        (PSLAP_SRV_PROXY_OBJECT)pSlapUoaIf->CreateProxyObject
            (
                pSlapUoaIf->hOwnerContext,
                (ANSC_HANDLE)NULL,
                obj_name,
                hSlapGoaObj
            );

    if ( !pSlapSrvProxy )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pSlapSrvProxy->SetObjMode  ((ANSC_HANDLE)pSlapSrvProxy, SLAP_SPO_OBJ_MODE_created);
        pSlapSrvProxy->SetSlapGoaIf((ANSC_HANDLE)pSlapSrvProxy, (ANSC_HANDLE)pSlapGoaIf  );
        pSlapSrvProxy->SetSlapPoaIf((ANSC_HANDLE)pSlapSrvProxy, (ANSC_HANDLE)pSlapPoaIf  );
    }

    return  (ANSC_HANDLE)pSlapSrvProxy;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapAmoPoaEnrollProxyObject
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
SlapAmoPoaEnrollProxyObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjDescr
    )
{
    PSLAP_ACCESS_MANAGER_OBJECT     pMyObject          = (PSLAP_ACCESS_MANAGER_OBJECT  )hThisObject;
    PSLAP_ENV_CONTROLLER_OBJECT     pSlapEnvController = (PSLAP_ENV_CONTROLLER_OBJECT  )pMyObject->hSlapEnvController;
    PSLAP_UOA_INTERFACE             pSlapUoaIf         = (PSLAP_UOA_INTERFACE          )pSlapEnvController->GetSlapUoaIf((ANSC_HANDLE)pSlapEnvController);

    return  pSlapUoaIf->EnrollProxyObject(pSlapUoaIf->hOwnerContext, hObjDescr);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapAmoPoaUnbindProxyObject
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
SlapAmoPoaUnbindProxyObject
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    )
{
    PSLAP_ACCESS_MANAGER_OBJECT     pMyObject          = (PSLAP_ACCESS_MANAGER_OBJECT  )hThisObject;
    PSLAP_ENV_CONTROLLER_OBJECT     pSlapEnvController = (PSLAP_ENV_CONTROLLER_OBJECT  )pMyObject->hSlapEnvController;
    PSLAP_UOA_INTERFACE             pSlapUoaIf         = (PSLAP_UOA_INTERFACE          )pSlapEnvController->GetSlapUoaIf((ANSC_HANDLE)pSlapEnvController);

    return  pSlapUoaIf->UnbindProxyObject(pSlapUoaIf->hOwnerContext, obj_name);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SlapAmoPoaVerifyProxyObject
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
SlapAmoPoaVerifyProxyObject
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    )
{
    PSLAP_ACCESS_MANAGER_OBJECT     pMyObject          = (PSLAP_ACCESS_MANAGER_OBJECT  )hThisObject;
    PSLAP_ENV_CONTROLLER_OBJECT     pSlapEnvController = (PSLAP_ENV_CONTROLLER_OBJECT  )pMyObject->hSlapEnvController;
    PSLAP_UOA_INTERFACE             pSlapUoaIf         = (PSLAP_UOA_INTERFACE          )pSlapEnvController->GetSlapUoaIf((ANSC_HANDLE)pSlapEnvController);

    return  pSlapUoaIf->VerifyProxyObject(pSlapUoaIf->hOwnerContext, obj_name);
}
