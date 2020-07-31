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

    module:	tls_hsoclient_control.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced control functions
        of the TLS Hso Client Object.

        *   TlsHsoClientStart

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/01/03    initial revision.

**********************************************************************/


#include "tls_hsoclient_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsHsoClientStart
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
TlsHsoClientStart
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PTLS_HSO_CLIENT_OBJECT          pMyObject          = (PTLS_HSO_CLIENT_OBJECT     )hThisObject;
    PTLS_SESSION_STATE              pSessionState      = (PTLS_SESSION_STATE         )&pMyObject->SessionState;
    PTLS_SECURITY_PARAMS            pSecurityParams    = (PTLS_SECURITY_PARAMS       )&pSessionState->SecurityParams;
    PTLS_CBC_INTERFACE              pTlsCbcIf          = (PTLS_CBC_INTERFACE         )pMyObject->hTlsCbcIf;
    PTLS_MEC_INTERFACE              pTlsMecIf          = (PTLS_MEC_INTERFACE         )pMyObject->hTlsMecIf;
    PTLS_HSM_INTERFACE              pTlsHsmIf          = (PTLS_HSM_INTERFACE         )pTlsMecIf->GetTlsHsmIf         (pTlsMecIf->hOwnerContext);
    PTLS_CONNECTION_PARAMS          pConnParams        = (PTLS_CONNECTION_PARAMS     )pTlsCbcIf->GetConnParams       (pTlsCbcIf->hOwnerContext);

    if ( pMyObject->HsClientState != TLS_HSC_STATE_idle )
    {
        return  ANSC_STATUS_FAILURE;
    }
    else
    {
        pMyObject->Reset((ANSC_HANDLE)pMyObject);

        pSessionState->HostID          = pConnParams->HostID;
        pSessionState->PeerID          = pConnParams->PeerID;
        pSecurityParams->ConnectionEnd = TLS_CONNECTION_END_client;
    }

    /*
     * The client hello message includes a variable length session identifier. If not empty, the
     * value identifies a session between the same client and server whose security parameters the
     * client wishes to reuse. The session identifier may be from an earlier connection, this
     * connection, or another currently active connection.
     */
    if ( pConnParams->bQuickHandshake )
    {
        returnStatus =
            pTlsHsmIf->SelHandshakeSession
                (
                    pTlsHsmIf->hOwnerContext,
                    pConnParams->HostID,
                    pConnParams->PeerID,
                    TLS_CONNECTION_END_client,
                    (ANSC_HANDLE)pSessionState
                );

        if ( returnStatus == ANSC_STATUS_SUCCESS )
        {
            returnStatus =
                pTlsCbcIf->SetSessionID
                    (
                        pTlsCbcIf->hOwnerContext,
                        pSessionState->SessionID,
                        pSessionState->SessionIDSize
                    );

            pTlsCbcIf->SetQuickHandshake  (pTlsCbcIf->hOwnerContext, TRUE);
            pTlsCbcIf->SetCipherSuiteArray(pTlsCbcIf->hOwnerContext, &pSessionState->CipherSuite, 1);
            pTlsCbcIf->SetCompressionArray(pTlsCbcIf->hOwnerContext, &pSessionState->Compression, 1);
        }
        else
        {
            pTlsCbcIf->SetQuickHandshake(pTlsCbcIf->hOwnerContext, FALSE);
        }
    }

    /*
     * We intentionally delay sending the Client Hello message to give server a chance to send the
     * Hello Request message...
     */
    AnscSleep(1000);

    /*
     * The Client Hello Message may be sent unsolicitly or in reply to the Hello Request message
     * sent by server. Therefore, the SendClientHello() function call needs to be protected as a
     * critical section, which can only be executed by one task at any time.
     */
    AnscAcquireLock(&pMyObject->SyncLock);

    if ( pMyObject->HsClientState == TLS_HSC_STATE_idle )
    {
        returnStatus = pMyObject->SendClientHello((ANSC_HANDLE)pMyObject);
    }
    else
    {
        returnStatus = ANSC_STATUS_SUCCESS;
    }

    AnscReleaseLock(&pMyObject->SyncLock);

    return  returnStatus;
}
