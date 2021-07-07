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

    module:	http_wcso_request.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the request processing functions
        of the Http Webc Session Object.

        *   HttpWcsoConnect
        *   HttpWcsoRequest
        *   HttpWcsoDoOptions
        *   HttpWcsoDoGet
        *   HttpWcsoDoHead
        *   HttpWcsoDoPost
        *   HttpWcsoDoPut
        *   HttpWcsoDoDelete
        *   HttpWcsoDoTrace
        *   HttpWcsoDoConnect
        *   HttpWcsoConnectPeer

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        04/22/02    initial revision.

**********************************************************************/


#include "http_wcso_global.h"
#include "ansc_xsocket_external_api.h"

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWcsoConnect
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to establish a TCP connection with the
        specified web server.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWcsoConnect
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PHTTP_WEBC_SESSION_OBJECT       pMyObject        = (PHTTP_WEBC_SESSION_OBJECT     )hThisObject;
    PHTTP_WEBC_SESSION_PROPERTY     pProperty        = (PHTTP_WEBC_SESSION_PROPERTY   )&pMyObject->Property;
    PHTTP_BSP_INTERFACE             pBspIf           = (PHTTP_BSP_INTERFACE           )pMyObject->hBspIf;
    PHTTP_SIMPLE_CLIENT_OBJECT      pHttpSco         = (PHTTP_SIMPLE_CLIENT_OBJECT    )pMyObject->hOwnerContext;
    HTTP_SCO_HOST_ADDRESSES         PeerAddr;
    BOOL                            bSameAddresses   = FALSE;
    ANSC_IPV4_ADDRESS               ipAddr;
    memset(&ipAddr, 1, sizeof(ANSC_IPV4_ADDRESS));
    AnscStartupXsocketWrapper((ANSC_HANDLE)pMyObject);

    AnscZeroMemory(&PeerAddr, sizeof(HTTP_SCO_HOST_ADDRESSES));
    PeerAddr.NumPeers   = HTTP_SCO_MAX_RESOLVED_HOST;

    if ( pProperty->SessionFlags & HTTP_WCSO_FLAG_xsocketEnabled )
    {
        XsktResolveHostName2(pProperty->PeerName, ((PULONG)PeerAddr.PeerAddresses), (PeerAddr.NumPeers));
    }
    else
    {
        AnscResolveHostName2(pProperty->PeerName, ((PULONG)PeerAddr.PeerAddresses), (PeerAddr.NumPeers));
    }

#ifndef _ANSC_IPV6_COMPATIBLE_
    if ( PeerAddr.NumPeers == 0 )
    {
        returnStatus =
            pBspIf->Notify
                (
                    pBspIf->hOwnerContext,
                    HTTP_BSP_EVENT_CANNOT_RESOLVE_NAME,
                    0,
                    pMyObject->hBspReqContext,
                    NULL,
                    NULL
                );

        pMyObject->SessionState = HTTP_WCSO_STATE_FINISHED;

        return  ANSC_STATUS_BAD_NAME;
    }
    else
    {
        pHttpSco->SetPeerAddresses((ANSC_HANDLE)pHttpSco, (ANSC_HANDLE)&PeerAddr, &bSameAddresses);

        if ( bSameAddresses )
        {
            ipAddr.Value    = pHttpSco->GetCurPeerAddr((ANSC_HANDLE)pHttpSco);
            pHttpSco->ResetPeerAddrPick((ANSC_HANDLE)pHttpSco, ipAddr.Value);
        }
        else
        {
            pHttpSco->GetNextPeerAddr((ANSC_HANDLE)pHttpSco, &ipAddr.Value);
        }
    }
#endif
    returnStatus = pMyObject->ConnectPeer((ANSC_HANDLE)pMyObject, ipAddr.Value);

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        pMyObject->ConnectionFailed((ANSC_HANDLE)pMyObject);

        pMyObject->SessionState = HTTP_WCSO_STATE_FINISHED;

        return  ANSC_STATUS_FAILURE;
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWcsoRequest
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulMethod,
                ANSC_HANDLE                 hReqUri,
                ANSC_HANDLE                 hReqContext
            );

    description:

        This function is called to initiate a HTTP transaction.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulMethod
                Specifies the HTTP request method to be conducted.

                ANSC_HANDLE                 hReqUri
                Specifies the HTTP uri to which the request is to be
                targetted.

                ANSC_HANDLE                 hReqContext
                Specifies a request-specific context which will be
                passed back for browsing services.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWcsoRequest
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMethod,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_WEBC_SESSION_OBJECT       pMyObject    = (PHTTP_WEBC_SESSION_OBJECT  )hThisObject;
    PHTTP_REQUEST_URI               pReqUri      = (PHTTP_REQUEST_URI          )hReqUri;
    PHTTP_WEBC_TRANS_OBJECT         pWebcTrans   = NULL;
    PHTTP_REQUEST_URI               pHttpReqUri  = NULL;

    if ( pMyObject->SessionState != HTTP_WCSO_STATE_SERVER_CONNECTED )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        pWebcTrans = (PHTTP_WEBC_TRANS_OBJECT)pMyObject->GetCurWcto((ANSC_HANDLE)pMyObject);
    }

    if ( !pWebcTrans )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else if ( pWebcTrans->GetTransState((ANSC_HANDLE)pWebcTrans) != HTTP_WCTO_STATE_INITIALIZED )
    {
        pWebcTrans->ReleaseAccess((ANSC_HANDLE)pWebcTrans);

        return  ANSC_STATUS_UNAPPLICABLE;
    }

    pHttpReqUri  = (PHTTP_REQUEST_URI)AnscAllocateMemory(sizeof(HTTP_REQUEST_URI));

    if ( !pHttpReqUri )
    {
    	AnscTrace("HttpWcsoRequest - can't create http request URI!\n");
        return ANSC_STATUS_RESOURCES;
    }

    *pHttpReqUri    = *pReqUri;

    if ( pMyObject->hReqUri )
    {
        AnscFreeMemory(pMyObject->hReqUri);
    }

    pMyObject->hReqUri      = (ANSC_HANDLE)pHttpReqUri;
    pMyObject->ReqMethod    = ulMethod;

    pMyObject->Timestamp    = AnscGetTickInSeconds();
    pMyObject->bRemoveMe    = FALSE;

    returnStatus =
        pWebcTrans->Request
            (
                (ANSC_HANDLE)pWebcTrans,
                ulMethod,
                (ANSC_HANDLE)pReqUri,
                hReqContext
            );

    pWebcTrans->ReleaseAccess((ANSC_HANDLE)pWebcTrans);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWcsoDoOptions
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hReqUri,
                ANSC_HANDLE                 hReqContext
            );

    description:

        This function is called to initiate a HTTP transaction.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hReqUri
                Specifies the HTTP uri to which the request is to be
                targetted.

                ANSC_HANDLE                 hReqContext
                Specifies a request-specific context which will be
                passed back for browsing services.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWcsoDoOptions
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_WEBC_SESSION_OBJECT       pMyObject    = (PHTTP_WEBC_SESSION_OBJECT  )hThisObject;
    PHTTP_REQUEST_URI               pReqUri      = (PHTTP_REQUEST_URI          )hReqUri;

    returnStatus =
        pMyObject->Request
            (
                (ANSC_HANDLE)pMyObject,
                HTTP_METHOD_CODE_OPTIONS,
                (ANSC_HANDLE)pReqUri,
                hReqContext
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWcsoDoGet
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hReqUri,
                ANSC_HANDLE                 hReqContext
            );

    description:

        This function is called to initiate a HTTP transaction.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hReqUri
                Specifies the HTTP uri to which the request is to be
                targetted.

                ANSC_HANDLE                 hReqContext
                Specifies a request-specific context which will be
                passed back for browsing services.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWcsoDoGet
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_WEBC_SESSION_OBJECT       pMyObject    = (PHTTP_WEBC_SESSION_OBJECT  )hThisObject;
    PHTTP_REQUEST_URI               pReqUri      = (PHTTP_REQUEST_URI          )hReqUri;

    returnStatus =
        pMyObject->Request
            (
                (ANSC_HANDLE)pMyObject,
                HTTP_METHOD_CODE_GET,
                (ANSC_HANDLE)pReqUri,
                hReqContext
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWcsoDoHead
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hReqUri,
                ANSC_HANDLE                 hReqContext
            );

    description:

        This function is called to initiate a HTTP transaction.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hReqUri
                Specifies the HTTP uri to which the request is to be
                targetted.

                ANSC_HANDLE                 hReqContext
                Specifies a request-specific context which will be
                passed back for browsing services.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWcsoDoHead
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_WEBC_SESSION_OBJECT       pMyObject    = (PHTTP_WEBC_SESSION_OBJECT  )hThisObject;
    PHTTP_REQUEST_URI               pReqUri      = (PHTTP_REQUEST_URI          )hReqUri;

    returnStatus =
        pMyObject->Request
            (
                (ANSC_HANDLE)pMyObject,
                HTTP_METHOD_CODE_HEAD,
                (ANSC_HANDLE)pReqUri,
                hReqContext
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWcsoDoPost
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hReqUri,
                ANSC_HANDLE                 hReqContext
            );

    description:

        This function is called to initiate a HTTP transaction.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hReqUri
                Specifies the HTTP uri to which the request is to be
                targetted.

                ANSC_HANDLE                 hReqContext
                Specifies a request-specific context which will be
                passed back for browsing services.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWcsoDoPost
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_WEBC_SESSION_OBJECT       pMyObject    = (PHTTP_WEBC_SESSION_OBJECT  )hThisObject;
    PHTTP_REQUEST_URI               pReqUri      = (PHTTP_REQUEST_URI          )hReqUri;

    returnStatus =
        pMyObject->Request
            (
                (ANSC_HANDLE)pMyObject,
                HTTP_METHOD_CODE_POST,
                (ANSC_HANDLE)pReqUri,
                hReqContext
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWcsoDoPut
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hReqUri,
                ANSC_HANDLE                 hReqContext
            );

    description:

        This function is called to initiate a HTTP transaction.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hReqUri
                Specifies the HTTP uri to which the request is to be
                targetted.

                ANSC_HANDLE                 hReqContext
                Specifies a request-specific context which will be
                passed back for browsing services.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWcsoDoPut
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_WEBC_SESSION_OBJECT       pMyObject    = (PHTTP_WEBC_SESSION_OBJECT  )hThisObject;
    PHTTP_REQUEST_URI               pReqUri      = (PHTTP_REQUEST_URI          )hReqUri;

    returnStatus =
        pMyObject->Request
            (
                (ANSC_HANDLE)pMyObject,
                HTTP_METHOD_CODE_PUT,
                (ANSC_HANDLE)pReqUri,
                hReqContext
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWcsoDoDelete
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hReqUri,
                ANSC_HANDLE                 hReqContext
            );

    description:

        This function is called to initiate a HTTP transaction.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hReqUri
                Specifies the HTTP uri to which the request is to be
                targetted.

                ANSC_HANDLE                 hReqContext
                Specifies a request-specific context which will be
                passed back for browsing services.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWcsoDoDelete
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_WEBC_SESSION_OBJECT       pMyObject    = (PHTTP_WEBC_SESSION_OBJECT  )hThisObject;
    PHTTP_REQUEST_URI               pReqUri      = (PHTTP_REQUEST_URI          )hReqUri;

    returnStatus =
        pMyObject->Request
            (
                (ANSC_HANDLE)pMyObject,
                HTTP_METHOD_CODE_DELETE,
                (ANSC_HANDLE)pReqUri,
                hReqContext
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWcsoDoTrace
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hReqUri,
                ANSC_HANDLE                 hReqContext
            );

    description:

        This function is called to initiate a HTTP transaction.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hReqUri
                Specifies the HTTP uri to which the request is to be
                targetted.

                ANSC_HANDLE                 hReqContext
                Specifies a request-specific context which will be
                passed back for browsing services.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWcsoDoTrace
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_WEBC_SESSION_OBJECT       pMyObject    = (PHTTP_WEBC_SESSION_OBJECT  )hThisObject;
    PHTTP_REQUEST_URI               pReqUri      = (PHTTP_REQUEST_URI          )hReqUri;

    returnStatus =
        pMyObject->Request
            (
                (ANSC_HANDLE)pMyObject,
                HTTP_METHOD_CODE_TRACE,
                (ANSC_HANDLE)pReqUri,
                hReqContext
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWcsoDoConnect
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hReqUri,
                ANSC_HANDLE                 hReqContext
            );

    description:

        This function is called to initiate a HTTP transaction.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hReqUri
                Specifies the HTTP uri to which the request is to be
                targetted.

                ANSC_HANDLE                 hReqContext
                Specifies a request-specific context which will be
                passed back for browsing services.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWcsoDoConnect
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_WEBC_SESSION_OBJECT       pMyObject    = (PHTTP_WEBC_SESSION_OBJECT  )hThisObject;
    PHTTP_REQUEST_URI               pReqUri      = (PHTTP_REQUEST_URI          )hReqUri;

    returnStatus =
        pMyObject->Request
            (
                (ANSC_HANDLE)pMyObject,
                HTTP_METHOD_CODE_CONNECT,
                (ANSC_HANDLE)pReqUri,
                hReqContext
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWcsoConnectPeer
            (
                ANSC_HANDLE                 hThisObject,
                uint32_t                    ipAddrToConnect
            );

    description:

        This function is called to establish a TCP connection with the
        specified web server.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                uint32_t                    ipAddrToConnect
                The peer to connect.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWcsoConnectPeer
    (
        ANSC_HANDLE                 hThisObject,
        uint32_t                    ipAddrToConnect
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PHTTP_WEBC_SESSION_OBJECT       pMyObject        = (PHTTP_WEBC_SESSION_OBJECT     )hThisObject;
    PHTTP_WEBC_SESSION_PROPERTY     pProperty        = (PHTTP_WEBC_SESSION_PROPERTY   )&pMyObject->Property;
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pTcpSimpleClient = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)pMyObject->hTcpSimpleClient;
    PHTTP_SIMPLE_CLIENT_OBJECT      pHttpSco         = (PHTTP_SIMPLE_CLIENT_OBJECT    )pMyObject->hOwnerContext;
    ULONG                           ulSctoMode       = ANSC_SCTO_MODE_FOREIGN_BUFFER;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp      = NULL;
    BOOL                            bGotPeerAddr     = TRUE;
    ANSC_IPV4_ADDRESS               PeerAddr;
    PANSC_BUFFER_DESCRIPTOR         pRecvBdo         = (ANSC_HANDLE)NULL;
    uint32_t                        ipAddr           = ipAddrToConnect;

TRY_NEXT:
    if ( ipAddr == 0 )
    {
        goto NO_MORE_TRY;
    }

    PeerAddr.Value  = ipAddr;

    pRecvBdo     = (PANSC_BUFFER_DESCRIPTOR)pTcpSimpleClient->GetBufferContext((ANSC_HANDLE)pTcpSimpleClient);
    pTcpSimpleClient->SetBufferContext((ANSC_HANDLE)pTcpSimpleClient, NULL, 0, NULL);

    if ( pRecvBdo )
    {
        AnscReleaseBdo(pRecvBdo);
        pRecvBdo    = NULL;
    }

    returnStatus = pTcpSimpleClient->Cancel((ANSC_HANDLE)pTcpSimpleClient);

    if ( pProperty->SessionFlags & HTTP_WCSO_FLAG_tlsEnabled )
    {
        ulSctoMode |= ANSC_SCTO_MODE_TLS_ENABLED;
    }

    if ( pProperty->SessionFlags & HTTP_WCSO_FLAG_xsocketEnabled )
    {
        ulSctoMode |= ANSC_SCTO_MODE_XSOCKET;
    }

    pTcpSimpleClient->SetHostAddress   ((ANSC_HANDLE)pTcpSimpleClient, pProperty->HostAddr.Dot   );
    pTcpSimpleClient->SetHostPort      ((ANSC_HANDLE)pTcpSimpleClient, pProperty->HostPort       );
    pTcpSimpleClient->SetPeerName      ((ANSC_HANDLE)pTcpSimpleClient, (PUCHAR)pProperty->PeerName       );
    pTcpSimpleClient->SetPeerAddress   ((ANSC_HANDLE)pTcpSimpleClient, PeerAddr.Dot              );
    pTcpSimpleClient->SetPeerPort      ((ANSC_HANDLE)pTcpSimpleClient, pProperty->PeerPort       );
    pTcpSimpleClient->SetMaxMessageSize((ANSC_HANDLE)pTcpSimpleClient, HTTP_WCSO_MAX_MESSAGE_SIZE);
    pTcpSimpleClient->SetMode          ((ANSC_HANDLE)pTcpSimpleClient, ulSctoMode                );

    //    fprintf(stderr, "<RT> %s: HTTP Client DeviceName = '%s'\n", __FUNCTION__, pHttpSco ? pHttpSco->Property.BindToDevice.DeviceName : "NULL");
    if( pHttpSco && pHttpSco->Property.BindToDevice.DeviceName[0] &&
        AnscSizeOfString(pHttpSco->Property.BindToDevice.DeviceName) < ANSC_OBJ_IF_NAME_SIZE)
    {
        pTcpSimpleClient->SetSocketBindToDevice ((ANSC_HANDLE)pTcpSimpleClient, TRUE);
        pTcpSimpleClient->SetSocketDeviceName   ((ANSC_HANDLE)pTcpSimpleClient, pHttpSco->Property.BindToDevice.DeviceName);
    }
    else 
    {
        pTcpSimpleClient->SetSocketBindToDevice ((ANSC_HANDLE)pTcpSimpleClient, FALSE);
        pTcpSimpleClient->SetSocketDeviceName   ((ANSC_HANDLE)pTcpSimpleClient, "");
    }

    returnStatus = pTcpSimpleClient->Engage((ANSC_HANDLE)pTcpSimpleClient);

NO_MORE_TRY:
    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        if ( bGotPeerAddr )
        {
            BOOL                    bSucc   = FALSE;

            bSucc   = pHttpSco->GetNextPeerAddr((ANSC_HANDLE)pHttpSco, &ipAddr);

            if ( bSucc && ipAddr != 0 )
            {
                pMyObject->SessionState = HTTP_WCSO_STATE_INITIALIZED;

                goto TRY_NEXT;
            }
        }

        return  ANSC_STATUS_FAILURE;
    }
    else
    {
        pMyObject->SessionState = HTTP_WCSO_STATE_SERVER_CONNECTED;
    }

    pBufferDesp =
        (PANSC_BUFFER_DESCRIPTOR)AnscAllocateBdo
            (
                HTTP_WCSO_MAX_MESSAGE_SIZE,
                0,
                0
            );

    if ( pBufferDesp )
    {
        pTcpSimpleClient->SetBufferContext
            (
                (ANSC_HANDLE)pTcpSimpleClient,
                AnscBdoGetEndOfBlock(pBufferDesp),
                AnscBdoGetLeftSize  (pBufferDesp),
                (ANSC_HANDLE)pBufferDesp
            );
    }

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWcsoConnectionFailed
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to handle connection failure.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWcsoConnectionFailed
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBC_SESSION_OBJECT       pMyObject        = (PHTTP_WEBC_SESSION_OBJECT     )hThisObject;
    PHTTP_BSP_INTERFACE             pBspIf           = (PHTTP_BSP_INTERFACE           )pMyObject->hBspIf;
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pTcpSimpleClient = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)pMyObject->hTcpSimpleClient;

    if ( pTcpSimpleClient->LastEvent != 0 )
    {
        switch ( pTcpSimpleClient->LastEvent )
        {
            case    ANSC_SCTOWO_EVENT_TLS_TIMEOUT :

                    pBspIf->Notify
                            (
                                pBspIf->hOwnerContext,
                                HTTP_BSP_EVENT_TLS_TIMEOUT,
                                pTcpSimpleClient->LastError,
                                pMyObject->hBspReqContext,
                                NULL,
                                NULL
                            );

                    break;

            case    ANSC_SCTOWO_EVENT_TLS_ERROR :

                    pBspIf->Notify
                            (
                                pBspIf->hOwnerContext,
                                HTTP_BSP_EVENT_TLS_ERROR,
                                pTcpSimpleClient->LastError,
                                pMyObject->hBspReqContext,
                                NULL,
                                NULL
                            );

                    break;

            default :

                    pBspIf->Notify
                            (
                                pBspIf->hOwnerContext,
                                HTTP_BSP_EVENT_SERVER_UNAVAILABLE,
                                pTcpSimpleClient->LastError,
                                pMyObject->hBspReqContext,
                                NULL,
                                NULL
                            );

                    break;
        }
    }
    /* TLS related events (more specific) are now reported directly, */
    /* comment this out to avoid double-event posting */
    /*
    else
    {
        returnStatus =
            pBspIf->Notify
                (
                    pBspIf->hOwnerContext,
                    HTTP_BSP_EVENT_SERVER_UNAVAILABLE,
                    0,
                    pMyObject->hBspReqContext,
                    NULL,
                    NULL
                );
    }
    */

    pMyObject->SessionState = HTTP_WCSO_STATE_FINISHED;

    return  ANSC_STATUS_SUCCESS;
}


