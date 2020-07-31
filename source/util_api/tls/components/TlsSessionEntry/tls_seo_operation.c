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

    module:	tls_seo_operation.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the TLS Session Entry Object.

        *   TlsSeoMatch1

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


#include "tls_seo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        TlsSeoMatch1
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       host_id,
                ULONG                       peer_id,
                ULONG                       conn_end
            );

    description:

        This function is called to match the host IDs.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       host_id
                Specifies the host_id to be matched.

                ULONG                       peer_id
                Specifies the peer_id to be matched.

                ULONG                       conn_end
                Specifies the conn_end to be matched.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
TlsSeoMatch1
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       host_id,
        ULONG                       peer_id,
        ULONG                       conn_end
    )
{
    PTLS_SESSION_ENTRY_OBJECT       pMyObject       = (PTLS_SESSION_ENTRY_OBJECT)hThisObject;
    PTLS_SESSION_STATE              pSessionState   = (PTLS_SESSION_STATE       )&pMyObject->SessionState;
    PTLS_SECURITY_PARAMS            pSecurityParams = (PTLS_SECURITY_PARAMS     )&pSessionState->SecurityParams;

    if ( pSessionState->HostID != host_id )
    {
        return  FALSE;
    }
    else if ( pSessionState->PeerID != peer_id )
    {
        return  FALSE;
    }
    else if ( pSecurityParams->ConnectionEnd != conn_end )
    {
        return  FALSE;
    }

    return  TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        TlsSeoMatch2
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       host_id,
                ULONG                       peer_id,
                PUCHAR                      session_id,
                ULONG                       session_id_size
            );

    description:

        This function is called to match the host IDs and session ID.

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

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
TlsSeoMatch2
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       host_id,
        ULONG                       peer_id,
        PUCHAR                      session_id,
        ULONG                       session_id_size
    )
{
    PTLS_SESSION_ENTRY_OBJECT       pMyObject       = (PTLS_SESSION_ENTRY_OBJECT)hThisObject;
    PTLS_SESSION_STATE              pSessionState   = (PTLS_SESSION_STATE       )&pMyObject->SessionState;
    
    if ( pSessionState->HostID != host_id )
    {
        return  FALSE;
    }
    else if ( pSessionState->PeerID != peer_id )
    {
        return  FALSE;
    }
    else if ( pSessionState->SessionIDSize != session_id_size )
    {
        return  FALSE;
    }
    else if ( !AnscEqualMemory
                (
                    pSessionState->SessionID,
                    session_id,
                    session_id_size
                ) )
    {
        return  FALSE;
    }

    return  TRUE;
}
