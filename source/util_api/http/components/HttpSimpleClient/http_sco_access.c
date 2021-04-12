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

    module:	http_sco_access.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced object-access functions
        of the Http Simple Client Object.

        *   HttpScoMapWcso
        *   HttpScoGetWcso
        *   HttpScoAddWcso
        *   HttpScoDelAllWcsos

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        04/21/02    initial revision.

**********************************************************************/


#include "http_sco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpScoMapWcso
            (
                ANSC_HANDLE                 hThisObject,
                char*                       host,
                USHORT                      port,
                ANSC_HANDLE                 hReqContext,
                BOOL                        bUseTls
            );

    description:

        This function is called to retrieve a Webc Session Object by
        matching the host name and port number.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       host
                Specifies the host name of the target server.

                USHORT                      port
                Specifes the port number of the target server.

                ANSC_HANDLE                 hReqContext
                Specifies a request-specific context which will be
                passed back for browsing services.

                BOOL                        bUseTls
                Specifies whether SSL/TLS should be used for this HTTP
                session.

    return:     webc session object.

**********************************************************************/

ANSC_HANDLE
HttpScoMapWcso
    (
        ANSC_HANDLE                 hThisObject,
        char*                       host,
        USHORT                      port,
        ANSC_HANDLE                 hReqContext,
        BOOL                        bUseTls
    )
{
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_WEBC_SESSION_OBJECT       pWebcSession = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           ulHashIndex  = AnscHashString2(host, AnscSizeOfString(host), HTTP_SCO_WCSO_TABLE_SIZE);
    ULONG                           ulSessFlags;

    AnscAcquireLock(&pMyObject->WcsoTableLock);

    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->WcsoTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pWebcSession = ACCESS_HTTP_WEBC_SESSION_OBJECT(pSLinkEntry);
        pSLinkEntry  = AnscSListGetNextEntry(pSLinkEntry);

        if ( pWebcSession->bRemoveMe )
        {
            continue;
        }

        ulSessFlags = bUseTls? HTTP_WCSO_FLAG_tlsEnabled : 0;

        if ( pMyObject->ClientMode & HTTP_SCO_MODE_XSOCKET )
        {
            ulSessFlags |= HTTP_WCSO_FLAG_xsocketEnabled;
        }

        if ( pMyObject->ClientMode & HTTP_SCO_MODE_NOTIFY_ON_ALL_CONN_ONCE )
        {
            ulSessFlags |= HTTP_WCSO_FLAG_BspNotifyOnAllConnOnce;
        }

        if ( pWebcSession->MatchServer
                (
                    (ANSC_HANDLE)pWebcSession,
                    host,
                    port,
                    ulSessFlags
                ) )
        {
            pWebcSession->AcquireAccess((ANSC_HANDLE)pWebcSession);

            if ( pWebcSession->IsReadyToRequest((ANSC_HANDLE)pWebcSession) )
            {
                ANSC_IPV4_ADDRESS   ipAddr;

                AnscReleaseLock(&pMyObject->WcsoTableLock);

                ipAddr.Value    = pMyObject->GetCurPeerAddr((ANSC_HANDLE)pMyObject);

                if ( ipAddr.Value != 0 )
                {
                    pMyObject->ResetPeerAddrPick((ANSC_HANDLE)pMyObject, ipAddr.Value);
                }

                return  (ANSC_HANDLE)pWebcSession;
            }
            else
            {
                pWebcSession->ReleaseAccess((ANSC_HANDLE)pWebcSession);
            }
        }
    }

    AnscReleaseLock(&pMyObject->WcsoTableLock);

    return  pMyObject->AddWcso
                (
                    (ANSC_HANDLE)pMyObject,
                    host,
                    port,
                    hReqContext,
                    bUseTls
                );
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpScoGetWcso
            (
                ANSC_HANDLE                 hThisObject,
                char*                       host,
                USHORT                      port,
                ANSC_HANDLE                 hReqContext,
                BOOL                        bUseTls
            );

    description:

        This function is called to retrieve a Webc Session Object by
        matching the host name and port number.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       host
                Specifies the host name of the target server.

                USHORT                      port
                Specifes the port number of the target server.

                ANSC_HANDLE                 hReqContext
                Specifies a request-specific context which will be
                passed back for browsing services.

                BOOL                        bUseTls
                Specifies whether SSL/TLS should be used for this HTTP
                session.

    return:     webc session object.

**********************************************************************/

ANSC_HANDLE
HttpScoGetWcso
    (
        ANSC_HANDLE                 hThisObject,
        char*                       host,
        USHORT                      port,
        ANSC_HANDLE                 hReqContext,
        BOOL                        bUseTls
    )
{
    UNREFERENCED_PARAMETER(hReqContext);
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_WEBC_SESSION_OBJECT       pWebcSession = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           ulHashIndex  = AnscHashString2(host, AnscSizeOfString(host), HTTP_SCO_WCSO_TABLE_SIZE);
    ULONG                           ulSessFlags;

    AnscAcquireLock(&pMyObject->WcsoTableLock);

    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->WcsoTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pWebcSession = ACCESS_HTTP_WEBC_SESSION_OBJECT(pSLinkEntry);
        pSLinkEntry  = AnscSListGetNextEntry(pSLinkEntry);

        ulSessFlags = bUseTls? HTTP_WCSO_FLAG_tlsEnabled : 0;

        if ( pMyObject->ClientMode & HTTP_SCO_MODE_XSOCKET )
        {
            ulSessFlags |= HTTP_WCSO_FLAG_xsocketEnabled;
        }

        if ( pMyObject->ClientMode & HTTP_SCO_MODE_NOTIFY_ON_ALL_CONN_ONCE )
        {
            ulSessFlags |= HTTP_WCSO_FLAG_BspNotifyOnAllConnOnce;
        }

        if ( pWebcSession->MatchServer
                (
                    (ANSC_HANDLE)pWebcSession,
                    host,
                    port,
                    ulSessFlags
                ) )
        {
            pWebcSession->AcquireAccess((ANSC_HANDLE)pWebcSession);

            AnscReleaseLock(&pMyObject->WcsoTableLock);

            return  (ANSC_HANDLE)pWebcSession;
        }
    }

    AnscReleaseLock(&pMyObject->WcsoTableLock);

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpScoAddWcso
            (
                ANSC_HANDLE                 hThisObject,
                char*                       host,
                USHORT                      port,
                ANSC_HANDLE                 hReqContext,
                BOOL                        bUseTls
            );

    description:

        This function is called to add a new Webc Session Object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       host
                Specifies the host name of the target server.

                USHORT                      port
                Specifes the port number of the target server.

                ANSC_HANDLE                 hReqContext
                Specifies a request-specific context which will be
                passed back for browsing services.

                BOOL                        bUseTls
                Specifies whether SSL/TLS should be used for this HTTP
                session.

    return:     webc session object.

**********************************************************************/

ANSC_HANDLE
HttpScoAddWcso
    (
        ANSC_HANDLE                 hThisObject,
        char*                       host,
        USHORT                      port,
        ANSC_HANDLE                 hReqContext,
        BOOL                        bUseTls
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_BSP_INTERFACE             pBspIf       = (PHTTP_BSP_INTERFACE         )pMyObject->hBspIf;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE         )pMyObject->hHfpIf;
    PHTTP_WEBC_SESSION_OBJECT       pWebcSession = (PHTTP_WEBC_SESSION_OBJECT   )pMyObject->AcquireWcso((ANSC_HANDLE)pMyObject);
    /*CID: 164307 Dereference before null check*/
    if ( !pWebcSession )
    {
    	AnscTrace("HttpScoAddWcso - can't acquire web client session object!\n");
        return  (ANSC_HANDLE)NULL;
    }

    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pTcpClient   = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)pWebcSession->hTcpSimpleClient;
    ULONG                           ulHashIndex  = AnscHashString2(host, AnscSizeOfString(host), HTTP_SCO_WCSO_TABLE_SIZE);
    ULONG                           ulWebcSessionMode   = bUseTls? HTTP_WCSO_FLAG_tlsEnabled : 0;

    if ( pMyObject->ClientMode & HTTP_SCO_MODE_XSOCKET )
    {
         ulWebcSessionMode   |= HTTP_WCSO_FLAG_xsocketEnabled;
    }

    if ( pMyObject->ClientMode & HTTP_SCO_MODE_NOTIFY_ON_ALL_CONN_ONCE )
    {
        ulWebcSessionMode   |= HTTP_WCSO_FLAG_BspNotifyOnAllConnOnce;
    }

    pWebcSession->hBspReqContext = hReqContext;
    pWebcSession->Timestamp      = AnscGetTickInSeconds();
    pWebcSession->HashIndex      = ulHashIndex;

    pWebcSession->SetPeerName    ((ANSC_HANDLE)pWebcSession, host               );
    pWebcSession->SetPeerPort    ((ANSC_HANDLE)pWebcSession, port               );
    pWebcSession->SetHostPort    ((ANSC_HANDLE)pWebcSession, 0                  );
    pWebcSession->SetSessionFlags((ANSC_HANDLE)pWebcSession, ulWebcSessionMode  );
    pWebcSession->SetBspIf       ((ANSC_HANDLE)pWebcSession, (ANSC_HANDLE)pBspIf);
    pWebcSession->SetHfpIf       ((ANSC_HANDLE)pWebcSession, (ANSC_HANDLE)pHfpIf);

    if( pTcpClient )
    {
	pTcpClient->SetHostNames ((ANSC_HANDLE)pTcpClient, (ANSC_HANDLE)&pMyObject->hostNames);
    }

    returnStatus = pWebcSession->Open((ANSC_HANDLE)pWebcSession);

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        pWebcSession->Return((ANSC_HANDLE)pWebcSession);

        return  (ANSC_HANDLE)NULL;
    }

    AnscAcquireLock   (&pMyObject->WcsoTableLock);
    AnscSListPushEntry(&pMyObject->WcsoTable[ulHashIndex], &pWebcSession->Linkage);
    pWebcSession->AcquireAccess((ANSC_HANDLE)pWebcSession);
    AnscReleaseLock   (&pMyObject->WcsoTableLock);

    return  (ANSC_HANDLE)pWebcSession;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpScoDelAllWcsos
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to return all Webc Session Objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpScoDelAllWcsos
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_WEBC_SESSION_OBJECT       pWebcSession = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           i            = 0;

    AnscAcquireLock(&pMyObject->WcsoTableLock);

    for ( i = 0; i < HTTP_SCO_WCSO_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscSListPopEntry(&pMyObject->WcsoTable[i]);

        while ( pSLinkEntry )
        {
            pWebcSession = ACCESS_HTTP_WEBC_SESSION_OBJECT(pSLinkEntry);
            pSLinkEntry  = AnscSListPopEntry(&pMyObject->WcsoTable[i]);

            pWebcSession->AcquireAccess((ANSC_HANDLE)pWebcSession);
            pWebcSession->ReleaseAccess((ANSC_HANDLE)pWebcSession);
            pWebcSession->Close        ((ANSC_HANDLE)pWebcSession);
            pWebcSession->Return       ((ANSC_HANDLE)pWebcSession);
        }
    }

    AnscReleaseLock(&pMyObject->WcsoTableLock);

    return  ANSC_STATUS_SUCCESS;
}
