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

    module:	tls_hsoserver_control.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced control functions
        of the TLS Hso Server Object.

        *   TlsHsoServerStart

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/05/03    initial revision.

**********************************************************************/


#include "tls_hsoserver_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsHsoServerStart
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to start the handshake process.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsHsoServerStart
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_HSO_SERVER_OBJECT          pMyObject          = (PTLS_HSO_SERVER_OBJECT     )hThisObject;
    PTLS_SESSION_STATE              pSessionState      = (PTLS_SESSION_STATE         )&pMyObject->SessionState;
    PTLS_SECURITY_PARAMS            pSecurityParams    = (PTLS_SECURITY_PARAMS       )&pSessionState->SecurityParams;
    PTLS_CBC_INTERFACE              pTlsCbcIf          = (PTLS_CBC_INTERFACE         )pMyObject->hTlsCbcIf;
    PTLS_CONNECTION_PARAMS          pConnParams        = (PTLS_CONNECTION_PARAMS     )pTlsCbcIf->GetConnParams       (pTlsCbcIf->hOwnerContext);

    if ( pMyObject->HsServerState != TLS_HSS_STATE_idle )
    {
        return  ANSC_STATUS_FAILURE;
    }
    else
    {
        pMyObject->Reset((ANSC_HANDLE)pMyObject);

        pSessionState->HostID          = pConnParams->HostID;
        pSessionState->PeerID          = pConnParams->PeerID;
        pSecurityParams->ConnectionEnd = TLS_CONNECTION_END_server;
    }

    return pMyObject->SendHelloRequest((ANSC_HANDLE)pMyObject);
}
