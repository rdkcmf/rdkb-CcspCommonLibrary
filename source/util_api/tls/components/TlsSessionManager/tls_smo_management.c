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

    module:	tls_smo_management.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the TLS Session Manager Object.

        *   TlsSmoGetSession
        *   TlsSmoAddSession
        *   TlsSmoDelSession
        *   TlsSmoDelAllSessions

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

        ANSC_HANDLE
        TlsSmoGetSession
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       host_id,
                ULONG                       peer_id,
                PUCHAR                      session_id,
                ULONG                       session_id_size
            );

    description:

        This function is called to retrieve a Session Entry Object by
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

    return:     session entry.

**********************************************************************/

ANSC_HANDLE
TlsSmoGetSession
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       host_id,
        ULONG                       peer_id,
        PUCHAR                      session_id,
        ULONG                       session_id_size
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
            AnscReleaseLock(&pMyObject->SeoTableLock);

            return  (ANSC_HANDLE)pSessionEntry;
        }
    }

    AnscReleaseLock(&pMyObject->SeoTableLock);

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsSmoAddSession
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSession
            );

    description:

        This function is called to add a Session Entry Object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSession
                Specifies the session entry to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsSmoAddSession
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession
    )
{
    PTLS_SESSION_MANAGER_OBJECT     pMyObject     = (PTLS_SESSION_MANAGER_OBJECT  )hThisObject;
    PTLS_SESSION_ENTRY_OBJECT       pSessionEntry = (PTLS_SESSION_ENTRY_OBJECT    )hSession;
    ULONG                           ulHashIndex   = AnscHashUlong(pSessionEntry->SessionState.PeerID, TLS_SMO_SEO_TABLE_SIZE);

    pSessionEntry->Timestamp        = AnscGetTickInSeconds();

    AnscAcquireLock   (&pMyObject->SeoTableLock);
    AnscQueuePushEntry(&pMyObject->SeoTable[ulHashIndex], &pSessionEntry->Linkage);
    AnscReleaseLock   (&pMyObject->SeoTableLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsSmoDelSession
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       host_id,
                ULONG                       peer_id,
                PUCHAR                      session_id,
                ULONG                       session_id_size
            );

    description:

        This function is called to delete a Session Entry Object by
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
TlsSmoDelSession
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       host_id,
        ULONG                       peer_id,
        PUCHAR                      session_id,
        ULONG                       session_id_size
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
            AnscQueuePopEntryByLink(&pMyObject->SeoTable[ulHashIndex], &pSessionEntry->Linkage);

            pSessionEntry->Remove((ANSC_HANDLE)pSessionEntry);

            break;
        }
    }

    AnscReleaseLock(&pMyObject->SeoTableLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsSmoDelAllSessions
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to delete all Session Entry Objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsSmoDelAllSessions
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_SESSION_MANAGER_OBJECT     pMyObject     = (PTLS_SESSION_MANAGER_OBJECT  )hThisObject;
    PTLS_SESSION_ENTRY_OBJECT       pSessionEntry = (PTLS_SESSION_ENTRY_OBJECT    )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry   = (PSINGLE_LINK_ENTRY           )NULL;
    ULONG                           i             = 0;

    AnscAcquireLock(&pMyObject->SeoTableLock);

    for ( i = 0; i < TLS_SMO_SEO_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscQueuePopEntry(&pMyObject->SeoTable[i]);

        while ( pSLinkEntry )
        {
            pSessionEntry = ACCESS_TLS_SESSION_ENTRY_OBJECT(pSLinkEntry);
            pSLinkEntry   = AnscQueuePopEntry(&pMyObject->SeoTable[i]);

            pSessionEntry->Remove((ANSC_HANDLE)pSessionEntry);
        }
    }

    AnscReleaseLock(&pMyObject->SeoTableLock);

    return  ANSC_STATUS_SUCCESS;
}
