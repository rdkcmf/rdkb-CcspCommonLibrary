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

    module:	slap_eco_operation.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Slap Env Controller Object.

        *   SlapEcoEngage
        *   SlapEcoCancel
        *   SlapEcoSetupEnv

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


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapEcoEngage
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to engage the object activities.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapEcoEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PSLAP_ENV_CONTROLLER_OBJECT     pMyObject      = (PSLAP_ENV_CONTROLLER_OBJECT  )hThisObject;
    PSLAP_OBJ_MAPPER_OBJECT         pSlapObjMapper = (PSLAP_OBJ_MAPPER_OBJECT      )pMyObject->hSlapObjMapper;

    if ( pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    if ( TRUE )
    {
        pSlapObjMapper->Engage((ANSC_HANDLE)pSlapObjMapper);
    }

    returnStatus = pMyObject->SetupEnv((ANSC_HANDLE)pMyObject);

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        return  returnStatus;
    }
    else
    {
        pMyObject->bActive = TRUE;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapEcoCancel
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to cancel the object activities.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapEcoCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_ENV_CONTROLLER_OBJECT     pMyObject      = (PSLAP_ENV_CONTROLLER_OBJECT  )hThisObject;
    PSLAP_OBJ_MAPPER_OBJECT         pSlapObjMapper = (PSLAP_OBJ_MAPPER_OBJECT      )pMyObject->hSlapObjMapper;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bActive = FALSE;
    }

    pSlapObjMapper->Cancel((ANSC_HANDLE)pSlapObjMapper);

    pMyObject->DelAllSlapContainers((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapEcoSetupEnv
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to setup the runtime environment.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapEcoSetupEnv
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PSLAP_ENV_CONTROLLER_OBJECT     pMyObject      = (PSLAP_ENV_CONTROLLER_OBJECT)hThisObject;
    PSLAP_UOA_INTERFACE             pSlapUoaIf     = (PSLAP_UOA_INTERFACE        )pMyObject->hSlapUoaIf;
    ANSC_HANDLE                     hSysSlapOcoCtl = (ANSC_HANDLE                )NULL;
    ANSC_HANDLE                     hSysSlapOcoUtl = (ANSC_HANDLE                )NULL;
    ANSC_HANDLE                     hSysSlapOcoCfg = (ANSC_HANDLE                )NULL;
    PSLAP_OBJECT_DESCRIPTOR         pObjDescriptor = (PSLAP_OBJECT_DESCRIPTOR    )NULL;

    hSysSlapOcoCtl = pSlapUoaIf->CreateContainer(pSlapUoaIf->hOwnerContext, SLAP_SYS_CONTAINER_CONTROL,       SLAP_CONTAINER_TYPE_SYSTEM);
    hSysSlapOcoUtl = pSlapUoaIf->CreateContainer(pSlapUoaIf->hOwnerContext, SLAP_SYS_CONTAINER_UTILITY,       SLAP_CONTAINER_TYPE_SYSTEM);
    hSysSlapOcoCfg = pSlapUoaIf->CreateContainer(pSlapUoaIf->hOwnerContext, SLAP_SYS_CONTAINER_CONFIGURATION, SLAP_CONTAINER_TYPE_SYSTEM);

    if ( !hSysSlapOcoCtl || !hSysSlapOcoUtl || !hSysSlapOcoCfg )
    {
        return  ANSC_STATUS_FAILURE;
    }

    if ( TRUE )
    {
        pObjDescriptor = (PSLAP_OBJECT_DESCRIPTOR)SlapVeoGetSlapObjDescriptor(pMyObject->hContainerContext);
        returnStatus   =
            pSlapUoaIf->EnrollObjDescriptor
                (
                    pSlapUoaIf->hOwnerContext,
                    (ANSC_HANDLE)pObjDescriptor
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            return  returnStatus;
        }
    }

    if ( TRUE )
    {
        pObjDescriptor = (PSLAP_OBJECT_DESCRIPTOR)SlapVmoGetSlapObjDescriptor(pMyObject->hContainerContext);
        returnStatus   =
            pSlapUoaIf->EnrollObjDescriptor
                (
                    pSlapUoaIf->hOwnerContext,
                    (ANSC_HANDLE)pObjDescriptor
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            return  returnStatus;
        }

        pSlapUoaIf->CreateObject
                (
                    pSlapUoaIf->hOwnerContext,
                    hSysSlapOcoUtl,
                    pObjDescriptor->ObjName,
                    (ANSC_HANDLE)NULL
                );
    }

    if ( TRUE )
    {
        pObjDescriptor = (PSLAP_OBJECT_DESCRIPTOR)SlapVcoGetSlapObjDescriptor(pMyObject->hContainerContext);
        returnStatus   =
            pSlapUoaIf->EnrollObjDescriptor
                (
                    pSlapUoaIf->hOwnerContext,
                    (ANSC_HANDLE)pObjDescriptor
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            return  returnStatus;
        }

        pSlapUoaIf->CreateObject
                (
                    pSlapUoaIf->hOwnerContext,
                    hSysSlapOcoUtl,
                    pObjDescriptor->ObjName,
                    (ANSC_HANDLE)NULL
                );
    }

    return  ANSC_STATUS_SUCCESS;
}
