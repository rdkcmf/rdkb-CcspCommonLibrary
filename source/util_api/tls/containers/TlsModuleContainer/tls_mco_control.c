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

    module:	tls_mco_control.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced control functions
        of the Tls Module Container Object.

        *   TlsMcoRegisterIfs
        *   TlsMcoDispatchIfs

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/13/03    initial revision.

**********************************************************************/


#include "tls_mco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsMcoRegisterIfs
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to register all the runtime interfaces.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsMcoRegisterIfs
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_MODULE_CONTAINER_OBJECT    pMyObject         = (PTLS_MODULE_CONTAINER_OBJECT)hThisObject;
    PTLS_ENV_CONTROLLER_OBJECT      pTlsEnvController = (PTLS_ENV_CONTROLLER_OBJECT  )pMyObject->hTlsEnvController;
    PTLS_SCS_INTERFACE              pTlsScsIf         = (PTLS_SCS_INTERFACE          )pTlsEnvController->hTlsScsIf;
    PTLS_MEC_INTERFACE              pTlsMecIf         = (PTLS_MEC_INTERFACE          )pTlsEnvController->hTlsMecIf;

    pMyObject->AddModuleIf
        (
            (ANSC_HANDLE)pMyObject,
            pTlsScsIf->Name,
            (ANSC_HANDLE)pTlsScsIf
        );

    pMyObject->AddModuleIf
        (
            (ANSC_HANDLE)pMyObject,
            pTlsMecIf->Name,
            (ANSC_HANDLE)pTlsMecIf
        );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsMcoDispatchIfs
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hIqcIf
            );

    description:

        This function is called to dispatch all the interface objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hIqcIf
                Specifies the Interface Query Controller Interface.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsMcoDispatchIfs
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hIqcIf
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hIqcIf);
    return  ANSC_STATUS_SUCCESS;
}
