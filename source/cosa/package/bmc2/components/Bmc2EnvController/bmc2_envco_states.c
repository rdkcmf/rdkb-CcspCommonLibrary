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

    module:	bmc2_envco_states.c

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Bmc2 Env Controller Object.

        *   Bmc2EnvcoGetBmc2SccIf
        *   Bmc2EnvcoGetBmc2ComExecutor
        *   Bmc2EnvcoGetBwrmEnvController
        *   Bmc2EnvcoGetBwrmRamIf
        *   Bmc2EnvcoGetSlapGoaIf
        *   Bmc2EnvcoSetSlapGoaIf
        *   Bmc2EnvcoGetSlapUoaIf
        *   Bmc2EnvcoSetSlapUoaIf
        *   Bmc2EnvcoGetSlapContainerServices
        *   Bmc2EnvcoSetSlapContainerServices
        *   Bmc2EnvcoSetPseudoTermDevName
        *   Bmc2EnvcoGetProperty
        *   Bmc2EnvcoSetProperty
        *   Bmc2EnvcoResetProperty
        *   Bmc2EnvcoReset

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/24/05    initial revision.

**********************************************************************/


#include "bmc2_envco_global.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        Bmc2EnvcoGetBmc2SccIf
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
Bmc2EnvcoGetBmc2SccIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_ENV_CONTROLLER_OBJECT     pMyObject    = (PBMC2_ENV_CONTROLLER_OBJECT  )hThisObject;

    return  pMyObject->hBmc2SccIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        Bmc2EnvcoGetBmc2ComExecutor
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
Bmc2EnvcoGetBmc2ComExecutor
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_ENV_CONTROLLER_OBJECT     pMyObject    = (PBMC2_ENV_CONTROLLER_OBJECT  )hThisObject;

    return  pMyObject->hBmc2ComExecutor;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        Bmc2EnvcoGetBwrmEnvController
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
Bmc2EnvcoGetBwrmEnvController
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_ENV_CONTROLLER_OBJECT     pMyObject    = (PBMC2_ENV_CONTROLLER_OBJECT  )hThisObject;

    return  pMyObject->hBwrmEnvController;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        Bmc2EnvcoGetBwrmRamIf
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
Bmc2EnvcoGetBwrmRamIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_ENV_CONTROLLER_OBJECT     pMyObject          = (PBMC2_ENV_CONTROLLER_OBJECT  )hThisObject;
    PBWRM_ENV_CONTROLLER_OBJECT     pBwrmEnvController = (PBWRM_ENV_CONTROLLER_OBJECT  )pMyObject->hBwrmEnvController;

    return  pBwrmEnvController->GetBwrmRamIf((ANSC_HANDLE)pBwrmEnvController);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        Bmc2EnvcoGetSlapGoaIf
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
Bmc2EnvcoGetSlapGoaIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_ENV_CONTROLLER_OBJECT     pMyObject    = (PBMC2_ENV_CONTROLLER_OBJECT  )hThisObject;

    return  pMyObject->hSlapGoaIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2EnvcoSetSlapGoaIf
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hInterface
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hInterface
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2EnvcoSetSlapGoaIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PBMC2_ENV_CONTROLLER_OBJECT     pMyObject    = (PBMC2_ENV_CONTROLLER_OBJECT  )hThisObject;

    pMyObject->hSlapGoaIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        Bmc2EnvcoGetSlapUoaIf
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
Bmc2EnvcoGetSlapUoaIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_ENV_CONTROLLER_OBJECT     pMyObject    = (PBMC2_ENV_CONTROLLER_OBJECT  )hThisObject;

    return  pMyObject->hSlapUoaIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2EnvcoSetSlapUoaIf
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hInterface
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hInterface
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2EnvcoSetSlapUoaIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PBMC2_ENV_CONTROLLER_OBJECT     pMyObject    = (PBMC2_ENV_CONTROLLER_OBJECT  )hThisObject;

    pMyObject->hSlapUoaIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        Bmc2EnvcoGetSlapContainerServices
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
Bmc2EnvcoGetSlapContainerServices
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_ENV_CONTROLLER_OBJECT     pMyObject    = (PBMC2_ENV_CONTROLLER_OBJECT  )hThisObject;

    return  pMyObject->hSlapContainerServices;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2EnvcoSetSlapContainerServices
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hContainer
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hContainer
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2EnvcoSetSlapContainerServices
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContainer
    )
{
    PBMC2_ENV_CONTROLLER_OBJECT     pMyObject    = (PBMC2_ENV_CONTROLLER_OBJECT  )hThisObject;

    pMyObject->hSlapContainerServices = hContainer;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        Bmc2EnvcoSetPseudoTermDevName
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pPseudoTermDevName
            )

    description:

        This function is called to set object property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pPseudoTermDevName
                Specifies the property data structure to be filled.

    return:     status of operation.

**********************************************************************/

void
Bmc2EnvcoSetPseudoTermDevName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pPseudoTermDevName
    )
{
    PBMC2_ENV_CONTROLLER_OBJECT     pMyObject    = (PBMC2_ENV_CONTROLLER_OBJECT  )hThisObject;

    if ( pMyObject->pPseudoTermDevName )
    {
        AnscFreeMemory(pMyObject->pPseudoTermDevName);
    }

    pMyObject->pPseudoTermDevName = AnscCloneString(pPseudoTermDevName);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2EnvcoGetProperty
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hProperty
            );

    description:

        This function is called to retrieve object property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hProperty
                Specifies the property data structure to be filled.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2EnvcoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PBMC2_ENV_CONTROLLER_OBJECT     pMyObject    = (PBMC2_ENV_CONTROLLER_OBJECT  )hThisObject;
    PBMC2_ENV_CONTROLLER_PROPERTY   pProperty    = (PBMC2_ENV_CONTROLLER_PROPERTY)&pMyObject->Property;

    *(PBMC2_ENV_CONTROLLER_PROPERTY)hProperty = *pProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2EnvcoSetProperty
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hProperty
            );

    description:

        This function is called to configure object property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hProperty
                Specifies the property data structure to be copied.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2EnvcoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PBMC2_ENV_CONTROLLER_OBJECT     pMyObject    = (PBMC2_ENV_CONTROLLER_OBJECT  )hThisObject;
    PBMC2_ENV_CONTROLLER_PROPERTY   pProperty    = (PBMC2_ENV_CONTROLLER_PROPERTY)&pMyObject->Property;

    *pProperty = *(PBMC2_ENV_CONTROLLER_PROPERTY)hProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2EnvcoResetProperty
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to reset object property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2EnvcoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_ENV_CONTROLLER_OBJECT     pMyObject    = (PBMC2_ENV_CONTROLLER_OBJECT  )hThisObject;
    PBMC2_ENV_CONTROLLER_PROPERTY   pProperty    = (PBMC2_ENV_CONTROLLER_PROPERTY)&pMyObject->Property;
    errno_t   rc  = -1;

    AnscZeroMemory(pProperty,                    sizeof(BMC2_ENV_CONTROLLER_PROPERTY));
    rc = STRCPY_S_NOCLOBBER(pProperty->RootPath, sizeof(pProperty->RootPath),  BMC2_DEF_ROOT_PATH                  );
    ERR_CHK(rc);
    rc = STRCPY_S_NOCLOBBER(pProperty->RootDomainCommand, sizeof(pProperty->RootDomainCommand), BMC2_DEF_ROOT_DOMAIN_COMMAND        );
    ERR_CHK(rc);

    pProperty->bMultiUserCtrl = TRUE;
    pProperty->bCacheScpPages = TRUE;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2EnvcoReset
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
Bmc2EnvcoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_ENV_CONTROLLER_OBJECT     pMyObject    = (PBMC2_ENV_CONTROLLER_OBJECT  )hThisObject;

    pMyObject->DelAllComTerminals     ((ANSC_HANDLE)pMyObject);
    pMyObject->DelAllCommandProperties((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
