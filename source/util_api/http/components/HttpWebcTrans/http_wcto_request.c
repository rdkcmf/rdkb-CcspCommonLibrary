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

    module:	http_wcto_request.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the request processing functions
        of the Http Webc Trans Object.

        *   HttpWctoRequest
        *   HttpWctoDoOptions
        *   HttpWctoDoGet
        *   HttpWctoDoHead
        *   HttpWctoDoPost
        *   HttpWctoDoPut
        *   HttpWctoDoDelete
        *   HttpWctoDoTrace
        *   HttpWctoDoConnect

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


#include "http_wcto_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWctoRequest
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
HttpWctoRequest
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMethod,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PHTTP_WEBC_TRANS_OBJECT         pMyObject         = (PHTTP_WEBC_TRANS_OBJECT       )hThisObject;
    PHTTP_SIMPLE_CLIENT_OBJECT      pSimpleClient     = (PHTTP_SIMPLE_CLIENT_OBJECT    )pMyObject->hOwnerContext;
    PHTTP_BSP_INTERFACE             pBspIf            = (PHTTP_BSP_INTERFACE           )pMyObject->hBspIf;
    PHTTP_BMO_REQ_OBJECT            pBmoReq           = (PHTTP_BMO_REQ_OBJECT          )pMyObject->hBmoReq;
    PHTTP_CAS_INTERFACE             pCasIf            = (PHTTP_CAS_INTERFACE           )pSimpleClient->GetCasIf((ANSC_HANDLE)pSimpleClient);
    PHTTP_REQUEST_URI               pReqUri           = (PHTTP_REQUEST_URI             )hReqUri;
    PHTTP_REQUEST_INFO              pReqInfo          = NULL;
    PHTTP_HFO_USER_AGENT            pHttpHfoUserAgent = NULL;
    PHTTP_HFO_HOST                  pHttpHfoHost      = NULL;

    if ( pMyObject->TransState != HTTP_WCTO_STATE_INITIALIZED )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        pReqInfo = (PHTTP_REQUEST_INFO)AnscAllocateMemory(sizeof(HTTP_REQUEST_INFO));

        if ( !pReqInfo )
        {
            return  ANSC_STATUS_RESOURCES;
        }
    }

    pMyObject->hBspReqContext = hReqContext;

    if ( TRUE )
    {
        pReqInfo->Method       = ulMethod;
        pReqInfo->MajorVersion = 1;
        pReqInfo->MinorVersion = 1;
        pReqInfo->RequestUri   = *pReqUri;
    }

    pBmoReq->Reset     ((ANSC_HANDLE)pBmoReq);
    pBmoReq->SetMboMode((ANSC_HANDLE)pBmoReq, HTTP_MBO_MODE_STORE  );
    pBmoReq->SetReqInfo((ANSC_HANDLE)pBmoReq, (ANSC_HANDLE)pReqInfo);

    /*
     * The User-Agent request-header field contains information about the user agent originating
     * the request. This is for statistical purposes, the tracing of protocol violations, and
     * automated recognition of user agents for the sake of tailoring responses to avoid particular
     * user agent limitations. User agents SHOULD include this field with requests. The field can
     * contain multiple product tokens and comments identifying the agent and any subproducts which
     * form a significant part of the user agent. By convention, the product tokens are listed in
     * order of their significance for identifying the application.
     *
     *      User-Agent = "User-Agent" ":" 1*( product | comment )
     *
     * Example:
     *
     *      User-Agent: CERN-LineMode/2.15 libwww/2.17b3
     */
    pHttpHfoUserAgent = (PHTTP_HFO_USER_AGENT)AnscAllocateMemory(sizeof(HTTP_HFO_USER_AGENT));

    if ( !pHttpHfoUserAgent )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pHttpHfoUserAgent->HashIndex = 0;
        pHttpHfoUserAgent->Flags     = 0;
        pHttpHfoUserAgent->HeaderId  = HTTP_HEADER_ID_USER_AGENT;

        AnscCopyString(pHttpHfoUserAgent->ProductToken, pSimpleClient->GetProductName((ANSC_HANDLE)pSimpleClient));
    }

    returnStatus =
        pBmoReq->AddHeaderField
            (
                (ANSC_HANDLE)pBmoReq,
                (ANSC_HANDLE)pHttpHfoUserAgent
            );

    /*
     * The Host request-header field specifies the Internet host and port number of the resource
     * being requested, as obtained from the original URI given by the user or referring resource
     * (generally an HTTP URL). The Host field value MUST represent the naming authority of the
     * origin server or gateway given by the original URL. This allows the origin server or gateway
     * to differentiate between internally-ambiguous URLs, such as the root "/" URL of a server for
     * multiple host names on a single IP address.
     *
     *      Host = "Host" ":" host [ ":" port ] ;
     *
     * A "host" without any trailing port information implies the default port for the service
     * requested (e.g., "80" for an HTTP URL). For example, a request on the origin server for
     *
     *      <http://www.w3.org/pub/WWW/>
     *
     * would properly include:
     *
     *      GET /pub/WWW/ HTTP/1.1
     *      Host: www.w3.org
     *
     * A client MUST include a Host header field in all HTTP/1.1 request messages. If the requested
     * URI does not include an Internet host name for the service being requested, then the Host
     * header field MUST be given with an empty value. An HTTP/1.1 proxy MUST ensure that any
     * request message it forwards does contain an appropriate Host header field that identifies
     * the service being requested by the proxy. All Internet-based HTTP/1.1 servers MUST respond
     * with a 400 (Bad Request) status code to any HTTP/1.1 request message which lacks a Host
     * header field.
     */
    pHttpHfoHost = (PHTTP_HFO_HOST)AnscAllocateMemory(sizeof(HTTP_HFO_HOST));

    if ( !pHttpHfoHost )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pHttpHfoHost->HashIndex = 0;
        pHttpHfoHost->Flags     = 0;
        pHttpHfoHost->HeaderId  = HTTP_HEADER_ID_HOST;

        AnscCopyString(pHttpHfoHost->HostName, pReqUri->HostName);
#ifdef _ANSC_IPV6_COMPATIBLE_
        if ( _ansc_strchr(pHttpHfoHost->HostName, ':') )
        {
            _ansc_sprintf(pHttpHfoHost->HostName, "%s%s%s", "[", pReqUri->HostName, "]");
        }
#endif
        pHttpHfoHost->HostPort = pReqUri->HostPort;
    }

    returnStatus =
        pBmoReq->AddHeaderField
            (
                (ANSC_HANDLE)pBmoReq,
                (ANSC_HANDLE)pHttpHfoHost
            );

    /*
     * This object is only responsible for constructing the most basic Http request message while
     * the BSP object is one for configuring the advanced headers, such as Accept-Language or the
     * message body. Before letting BSP polish the message, there might be some method-specific
     * header fields need to be added.
     */
    switch ( ulMethod )
    {
        case    HTTP_METHOD_CODE_OPTIONS :

                returnStatus =
                    pMyObject->DoOptions
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pBmoReq
                        );

                break;

        case    HTTP_METHOD_CODE_GET :

                returnStatus =
                    pMyObject->DoGet
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pBmoReq
                        );

                break;

        case    HTTP_METHOD_CODE_HEAD :

                returnStatus =
                    pMyObject->DoHead
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pBmoReq
                        );

                break;

        case    HTTP_METHOD_CODE_POST :

                returnStatus =
                    pMyObject->DoPost
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pBmoReq
                        );

                break;

        case    HTTP_METHOD_CODE_PUT :

                returnStatus =
                    pMyObject->DoPut
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pBmoReq
                        );

                break;

        case    HTTP_METHOD_CODE_DELETE :

                returnStatus =
                    pMyObject->DoDelete
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pBmoReq
                        );

                break;

        case    HTTP_METHOD_CODE_TRACE :

                returnStatus =
                    pMyObject->DoTrace
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pBmoReq
                        );

                break;

        case    HTTP_METHOD_CODE_CONNECT :

                returnStatus =
                    pMyObject->DoConnect
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pBmoReq
                        );

                break;

        default :

                break;
    }

    returnStatus =
        pBspIf->Polish
            (
                pBspIf->hOwnerContext,
                (ANSC_HANDLE)pBmoReq,
                pMyObject->hBspReqContext
            );

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        return  returnStatus;
    }
    else
    {
        /* process the request for client authentication */
        returnStatus = 
            pCasIf->ProcessRequest
                (
                    pCasIf->hOwnerContext,
                    (ANSC_HANDLE)pBmoReq
                );

        if ( returnStatus == ANSC_STATUS_SUCCESS )
        {
            returnStatus =
                pMyObject->Send
                    (
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)pBmoReq,
                        HTTP_WCTO_SFLAG_HEADERS | HTTP_WCTO_SFLAG_BODY
                    );

            if ( returnStatus == ANSC_STATUS_SUCCESS )
            {
                pMyObject->TransState = HTTP_WCTO_STATE_REQ_SENT;
            }
        }
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWctoDoOptions
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq
            );

    description:

        This function is called to initiate a HTTP transaction.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the HTTP request message object to be
                processed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWctoDoOptions
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hBmoReq);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWctoDoGet
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq
            );

    description:

        This function is called to initiate a HTTP transaction.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the HTTP request message object to be
                processed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWctoDoGet
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hBmoReq);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWctoDoHead
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq
            );

    description:

        This function is called to initiate a HTTP transaction.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the HTTP request message object to be
                processed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWctoDoHead
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hBmoReq);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWctoDoPost
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq
            );

    description:

        This function is called to initiate a HTTP transaction.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the HTTP request message object to be
                processed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWctoDoPost
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hBmoReq);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWctoDoPut
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq
            );

    description:

        This function is called to initiate a HTTP transaction.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the HTTP request message object to be
                processed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWctoDoPut
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hBmoReq);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWctoDoDelete
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq
            );

    description:

        This function is called to initiate a HTTP transaction.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the HTTP request message object to be
                processed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWctoDoDelete
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hBmoReq);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWctoDoTrace
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq
            );

    description:

        This function is called to initiate a HTTP transaction.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the HTTP request message object to be
                processed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWctoDoTrace
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hBmoReq);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWctoDoConnect
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq
            );

    description:

        This function is called to initiate a HTTP transaction.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the HTTP request message object to be
                processed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWctoDoConnect
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hBmoReq);

    return  ANSC_STATUS_SUCCESS;
}
