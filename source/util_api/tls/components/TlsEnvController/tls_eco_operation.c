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

    module:	tls_eco_operation.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the TLS Env Controller Object.

        *   TlsEcoEngage
        *   TlsEcoCancel

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/18/03    initial revision.

**********************************************************************/


#include "tls_eco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsEcoEngage
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to engage the object activity.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsEcoEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_ENV_CONTROLLER_OBJECT      pMyObject          = (PTLS_ENV_CONTROLLER_OBJECT  )hThisObject;
    PTLS_SESSION_MANAGER_OBJECT     pTlsSessionManager = (PTLS_SESSION_MANAGER_OBJECT )pMyObject->hTlsSessionManager;

    if ( pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bActive = TRUE;
    }

    if ( TRUE )
    {
        pTlsSessionManager->hTlsMecIf       = (ANSC_HANDLE)pMyObject->hTlsMecIf;
        pTlsSessionManager->Engage     ((ANSC_HANDLE)pTlsSessionManager);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsEcoCancel
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to cancel the object activity.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsEcoCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_ENV_CONTROLLER_OBJECT      pMyObject          = (PTLS_ENV_CONTROLLER_OBJECT  )hThisObject;
    PTLS_SESSION_MANAGER_OBJECT     pTlsSessionManager = (PTLS_SESSION_MANAGER_OBJECT )pMyObject->hTlsSessionManager;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bActive = FALSE;
    }

    if ( TRUE )
    {
        pTlsSessionManager->Cancel((ANSC_HANDLE)pTlsSessionManager);
    }

    return  ANSC_STATUS_SUCCESS;
}
