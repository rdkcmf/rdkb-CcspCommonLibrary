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

    module:	tls_mco_operation.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Tls Module Container Object.

        *   TlsMcoIgnite
        *   TlsMcoPutOut
        *   TlsMcoSwitch

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
        TlsMcoIgnite
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to engage operations for encapsulated
        component objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsMcoIgnite
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_MODULE_CONTAINER_OBJECT    pMyObject         = (PTLS_MODULE_CONTAINER_OBJECT)hThisObject;

    if ( TRUE )
    {
        pMyObject->RegisterIfs((ANSC_HANDLE)pMyObject);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsMcoPutOut
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to cancel operations for encapsulated
        component objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsMcoPutOut
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsMcoSwitch
            (
                ANSC_HANDLE                 hThisObject,
                BOOL                        bSwitchOn
            );

    description:

        This function is called to turn on/off the module as a whole.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                BOOL                        bSwitchOn
                Specifies whether to turn on or off the module.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsMcoSwitch
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bSwitchOn
    )
{
    PTLS_MODULE_CONTAINER_OBJECT    pMyObject         = (PTLS_MODULE_CONTAINER_OBJECT)hThisObject;
    PTLS_ENV_CONTROLLER_OBJECT      pTlsEnvController = (PTLS_ENV_CONTROLLER_OBJECT  )pMyObject->hTlsEnvController;
    PTLS_SCS_INTERFACE              pTlsScsIf         = (PTLS_SCS_INTERFACE          )pTlsEnvController->hTlsScsIf;

    if ( bSwitchOn )
    {
        pTlsEnvController->Engage((ANSC_HANDLE)pTlsEnvController);

        if ( TRUE )
        {
            AnscSocketTlsInitialize((ANSC_HANDLE)pTlsScsIf);
        }
    }
    else
    {
        if ( TRUE )
        {
            AnscSocketTlsUnload();
        }

        pTlsEnvController->Cancel((ANSC_HANDLE)pTlsEnvController);
    }

    return  ANSC_STATUS_SUCCESS;
}
