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

    module:	tls_smo_hsmif.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced interface functions
        of the TLS Session Manager Object.

        *   TlsSmoHsmSelHandshakeSession
        *   TlsSmoHsmGetHandshakeSession
        *   TlsSmoHsmAddHandshakeSession
        *   TlsSmoHsmDelHandshakeSession
        *   TlsSmoHsmDelAllHandshakeSessions

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/16/03    initial revision.

**********************************************************************/


#include "tls_smo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsSmoHsmSelHandshakeSession
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       host_id,
                ULONG                       peer_id,
                ULONG                       conn_end,
                ANSC_HANDLE                 hSessionState
            );

    description:

        This function is called to select a handshake session by
        matching the host IDs.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       host_id
                Specifies the host_id to be matched.

                ULONG                       peer_id
                Specifies the peer_id to be matched.

                ULONG                       conn_end
                Specifies the conn_end to be matched.

                ANSC_HANDLE                 hSessionState
                Specifies the session state to be filled.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsSmoHsmSelHandshakeSession
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       host_id,
        ULONG                       peer_id,
        ULONG                       conn_end,
        ANSC_HANDLE                 hSessionState
    )
{
    PTLS_SESSION_MANAGER_OBJECT     pMyObject     = (PTLS_SESSION_MANAGER_OBJECT  )hThisObject;
    PTLS_SESSION_ENTRY_OBJECT       pSessionEntry = (PTLS_SESSION_ENTRY_OBJECT    )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry   = (PSINGLE_LINK_ENTRY           )NULL;
    ULONG                           ulHashIndex   = AnscHashUlong(peer_id, TLS_SMO_SEO_TABLE_SIZE);

    AnscAcquireLock(&pMyObject->SeoTableLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->SeoTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pSessionEntry = ACCESS_TLS_SESSION_ENTRY_OBJECT(pSLinkEntry);
        pSLinkEntry   = AnscQueueGetNextEntry(pSLinkEntry);

        if ( pSessionEntry->Match1
                (
                    (ANSC_HANDLE)pSessionEntry,
                    host_id,
                    peer_id,
                    conn_end
                ) )
        {
            pSessionEntry->GetSessionState((ANSC_HANDLE)pSessionEntry, hSessionState);

            AnscReleaseLock(&pMyObject->SeoTableLock);

            return  ANSC_STATUS_SUCCESS;
        }
    }

    AnscReleaseLock(&pMyObject->SeoTableLock);

    return  ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsSmoHsmGetHandshakeSession
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       host_id,
                ULONG                       peer_id,
                PUCHAR                      session_id,
                ULONG                       session_id_size,
                ANSC_HANDLE                 hSessionState
            );

    description:

        This function is called to select a handshake session by
        matching the host IDs and session ID.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       host_id
                Specifies the host_id to be matched.

                ULONG                       peer_id
                Specifies the peer_id to be matched.

                PUCHAR                      session_id
                Specifies the session_id to be matched.

                ULONG                       session_id_size
                Specifies the size of the session_id to be matched.

                ANSC_HANDLE                 hSessionState
                Specifies the session state to be filled.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsSmoHsmGetHandshakeSession
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       host_id,
        ULONG                       peer_id,
        PUCHAR                      session_id,
        ULONG                       session_id_size,
        ANSC_HANDLE                 hSessionState
    )
{
    PTLS_SESSION_MANAGER_OBJECT     pMyObject     = (PTLS_SESSION_MANAGER_OBJECT  )hThisObject;
    PTLS_SESSION_ENTRY_OBJECT       pSessionEntry = (PTLS_SESSION_ENTRY_OBJECT    )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry   = (PSINGLE_LINK_ENTRY           )NULL;
    ULONG                           ulHashIndex   = AnscHashUlong(peer_id, TLS_SMO_SEO_TABLE_SIZE);

    AnscAcquireLock(&pMyObject->SeoTableLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->SeoTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pSessionEntry = ACCESS_TLS_SESSION_ENTRY_OBJECT(pSLinkEntry);
        pSLinkEntry   = AnscQueueGetNextEntry(pSLinkEntry);

        if ( pSessionEntry->Match2
                (
                    (ANSC_HANDLE)pSessionEntry,
                    host_id,
                    peer_id,
                    session_id,
                    session_id_size
                ) )
        {
            pSessionEntry->GetSessionState((ANSC_HANDLE)pSessionEntry, hSessionState);

            AnscReleaseLock(&pMyObject->SeoTableLock);

            return  ANSC_STATUS_SUCCESS;
        }
    }

    AnscReleaseLock(&pMyObject->SeoTableLock);

    return  ANSC_STATUS_CANT_FIND;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsSmoHsmAddHandshakeSession
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSessionState
            );

    description:

        This function is called to add a handshake session.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSessionState
                Specifies the session state to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsSmoHsmAddHandshakeSession
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSessionState
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PTLS_SESSION_MANAGER_OBJECT     pMyObject     = (PTLS_SESSION_MANAGER_OBJECT  )hThisObject;
    PTLS_SESSION_STATE              pSessionState = (PTLS_SESSION_STATE           )hSessionState;
    PTLS_SESSION_ENTRY_OBJECT       pSessionEntry = (PTLS_SESSION_ENTRY_OBJECT    )NULL;

    pSessionEntry =
        (PTLS_SESSION_ENTRY_OBJECT)TlsCreateSessionEntry
            (
                pMyObject->hContainerContext,
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)NULL
            );

    if ( !pSessionEntry )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pSessionEntry->SetSessionState((ANSC_HANDLE)pSessionEntry, (ANSC_HANDLE)pSessionState);
    }

    returnStatus = pMyObject->AddSession((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSessionEntry);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsSmoHsmDelHandshakeSession
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       host_id,
                ULONG                       peer_id,
                PUCHAR                      session_id,
                ULONG                       session_id_size
            );

    description:

        This function is called to delete a handshake session by
        matching the host IDs and session ID.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       host_id
                Specifies the host_id to be matched.

                ULONG                       peer_id
                Specifies the peer_id to be matched.

                PUCHAR                      session_id
                Specifies the session_id to be matched.

                ULONG                       session_id_size
                Specifies the size of the session_id to be matched.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsSmoHsmDelHandshakeSession
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       host_id,
        ULONG                       peer_id,
        PUCHAR                      session_id,
        ULONG                       session_id_size
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PTLS_SESSION_MANAGER_OBJECT     pMyObject     = (PTLS_SESSION_MANAGER_OBJECT  )hThisObject;
    
    returnStatus =
        pMyObject->DelSession
            (
                (ANSC_HANDLE)pMyObject,
                host_id,
                peer_id,
                session_id,
                session_id_size
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsSmoHsmDelAllHandshakeSessions
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to delete all handshake sessions.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsSmoHsmDelAllHandshakeSessions
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PTLS_SESSION_MANAGER_OBJECT     pMyObject     = (PTLS_SESSION_MANAGER_OBJECT  )hThisObject;
    
    returnStatus = pMyObject->DelAllSessions((ANSC_HANDLE)pMyObject);

    return  returnStatus;
}
