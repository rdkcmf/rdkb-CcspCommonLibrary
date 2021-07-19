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

    module:	http_sco_request.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the request processing functions
        of the Http Simple Client Object.

        *   HttpScoRequest
        *   HttpScoRequest2
        *   HttpScoRequest3
        *   HttpScoDoOptions
        *   HttpScoDoGet
        *   HttpScoDoHead
        *   HttpScoDoPost
        *   HttpScoDoPut
        *   HttpScoDoDelete
        *   HttpScoDoTrace
        *   HttpScoDoConnect

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
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpScoRequest
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulMethod,
                ANSC_HANDLE                 hReqUri,
                ANSC_HANDLE                 hReqContext,
                BOOL                        bUseTls
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

                BOOL                        bUseTls
                Specifies whether SSL/TLS should be used for this HTTP
                session.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpScoRequest
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMethod,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext,
        BOOL                        bUseTls
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_SIMPLE_CLIENT_PROPERTY    pProperty    = (PHTTP_SIMPLE_CLIENT_PROPERTY)&pMyObject->Property;
    PHTTP_REQUEST_URI               pReqUri      = (PHTTP_REQUEST_URI           )hReqUri;
    PHTTP_WEBC_SESSION_OBJECT       pWebcSession = NULL;

    if ( pProperty->bReuseConnections )
    {
        pWebcSession =
            (PHTTP_WEBC_SESSION_OBJECT)pMyObject->MapWcso
                (
                    (ANSC_HANDLE)pMyObject,
                    pReqUri->HostName,
                    pReqUri->HostPort,
                    hReqContext,
                    bUseTls
                );
    }
    else
    {
        pWebcSession =
            (PHTTP_WEBC_SESSION_OBJECT)pMyObject->AddWcso
                (
                    (ANSC_HANDLE)pMyObject,
                    pReqUri->HostName,
                    pReqUri->HostPort,
                    hReqContext,
                    bUseTls
                );
    }

    if ( !pWebcSession )
    {
    	AnscTrace("HttpScoRequest - can't create web client session object!\n");
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pMyObject->hWebClientSession = (ANSC_HANDLE)pWebcSession;
    }

    returnStatus =
        pWebcSession->Request
            (
                (ANSC_HANDLE)pWebcSession,
                ulMethod,
                (ANSC_HANDLE)pReqUri,
                hReqContext
            );

    pWebcSession->ReleaseAccess((ANSC_HANDLE)pWebcSession);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpScoRequest2
            (
                ANSC_HANDLE                 hThisObject,
                char*                       request_line,
                ANSC_HANDLE                 hReqContext
            );

    description:

        This function is called to initiate a HTTP transaction.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       request_line
                Specifies the HTTP request line to be requested.

                ANSC_HANDLE                 hReqContext
                Specifies a request-specific context which will be
                passed back for browsing services.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpScoRequest2
    (
        ANSC_HANDLE                 hThisObject,
        char*                       request_line,
        ANSC_HANDLE                 hReqContext
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_HFP_INTERFACE             pHttpHfpIf   = (PHTTP_HFP_INTERFACE         )pMyObject->hHfpIf;
    PHTTP_REQUEST_INFO              pHttpReqInfo = (PHTTP_REQUEST_INFO          )NULL;
    char*                           pStdReqLine  = NULL;
    BOOL                            bApplyTls    = (BOOL                        )FALSE;
    errno_t                         rc           = -1;

    if(!request_line)
    {
        return ANSC_STATUS_FAILURE;
    }
    size_t len = AnscSizeOfString(request_line) + 2;
    pStdReqLine = (char *) AnscAllocateMemory(len);
    if ( !pStdReqLine )
    {
        return  ANSC_STATUS_RESOURCES;
    }

    rc = sprintf_s(pStdReqLine, len, "%s\r\n", request_line);
    if (rc < EOK)
    {
        ERR_CHK(rc);
        AnscFreeMemory(pStdReqLine);
        return ANSC_STATUS_FAILURE;
    }

    pHttpReqInfo =
        (PHTTP_REQUEST_INFO)pHttpHfpIf->ParseRequestLine
            (
                pHttpHfpIf->hOwnerContext,
                pStdReqLine,
                AnscSizeOfString(pStdReqLine)
            );

    AnscFreeMemory(pStdReqLine);

    if ( !pHttpReqInfo )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else if ( pHttpReqInfo->RequestUri.Flag & HTTP_URI_FLAG_HTTPS )
    {
        bApplyTls = TRUE;
    }
    else
    {
        bApplyTls = FALSE;
    }

    returnStatus =
        pMyObject->Request
            (
                (ANSC_HANDLE)pMyObject,
                (ULONG      )pHttpReqInfo->Method,
                (ANSC_HANDLE)&pHttpReqInfo->RequestUri,
                hReqContext,
                bApplyTls
            );

    AnscFreeMemory(pHttpReqInfo);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpScoRequest3
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulMethod,
                ANSC_HANDLE                 hReqUri,
                ANSC_HANDLE                 hReqContext,
                BOOL                        bUseTls
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

                BOOL                        bUseTls
                Specifies whether SSL/TLS should be used for this HTTP
                session.

                char*                       pUserName
                User name given for authentication.

                char*                       pPassword
                User's password for authentication.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpScoRequest3
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMethod,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext,
        BOOL                        bUseTls,
        char*                       pUserName,
        char*                       pPassword
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_SIMPLE_CLIENT_PROPERTY    pProperty    = (PHTTP_SIMPLE_CLIENT_PROPERTY)&pMyObject->Property;
    PHTTP_CAS_INTERFACE             pCasIf       = (PHTTP_CAS_INTERFACE         )pMyObject->hCasIf;
    PHTTP_REQUEST_URI               pReqUri      = (PHTTP_REQUEST_URI           )hReqUri;
    PHTTP_WEBC_SESSION_OBJECT       pWebcSession = NULL;

    if ( pProperty->bReuseConnections )
    {
        pWebcSession =
            (PHTTP_WEBC_SESSION_OBJECT)pMyObject->MapWcso
                (
                    (ANSC_HANDLE)pMyObject,
                    pReqUri->HostName,
                    pReqUri->HostPort,
                    hReqContext,
                    bUseTls
                );
    }
    else
    {
        pWebcSession =
            (PHTTP_WEBC_SESSION_OBJECT)pMyObject->AddWcso
                (
                    (ANSC_HANDLE)pMyObject,
                    pReqUri->HostName,
                    pReqUri->HostPort,
                    hReqContext,
                    bUseTls
                );
    }

    if ( !pWebcSession )
    {
    	AnscTrace("HttpScoRequest3 - can't create web client session object!\n");
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pMyObject->hWebClientSession = (ANSC_HANDLE)pWebcSession;
    }

    if ( pUserName && pPassword )
    {
        PHTTP_AUTH_CLIENT_OBJECT    pAuthClientObj  = NULL;

        pCasIf->EnableAuth(pCasIf->hOwnerContext, TRUE);

        pAuthClientObj  = (PHTTP_AUTH_CLIENT_OBJECT)pMyObject->GetClientAuthObj((ANSC_HANDLE)pMyObject);

        if ( pAuthClientObj )
        {
            if ( pMyObject->pAuthUserName )
            {
                AnscFreeMemory(pMyObject->pAuthUserName);
            }

            if ( pMyObject->pAuthPassword )
            {
                AnscFreeMemory(pMyObject->pAuthPassword);
            }

            pMyObject->pAuthUserName = AnscCloneString(pUserName);
            pMyObject->pAuthPassword = AnscCloneString(pPassword);

            if ( (ANSC_HANDLE)NULL == pMyObject->hAcmIf )
            {
                PHTTP_ACM_INTERFACE pAcmIf = (PHTTP_ACM_INTERFACE)AnscAllocateMemory(sizeof(HTTP_ACM_INTERFACE));

                if ( pAcmIf )
                {
                    pMyObject->hAcmIf     = (ANSC_HANDLE)pAcmIf;

                    pAcmIf->hOwnerContext = (ANSC_HANDLE)pMyObject;
                    pAcmIf->Size          = sizeof(HTTP_ACM_INTERFACE);

                    pAcmIf->GetCredential = HttpScoGetCredential;
                }
            }

            pAuthClientObj->SetAcmIf((ANSC_HANDLE)pAuthClientObj, pMyObject->hAcmIf);
        }
    }

    returnStatus =
        pWebcSession->Request
            (
                (ANSC_HANDLE)pWebcSession,
                ulMethod,
                (ANSC_HANDLE)pReqUri,
                hReqContext
            );

    pWebcSession->ReleaseAccess((ANSC_HANDLE)pWebcSession);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpScoDoOptions
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hReqUri,
                ANSC_HANDLE                 hReqContext,
                BOOL                        bUseTls
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

                BOOL                        bUseTls
                Specifies whether SSL/TLS should be used for this HTTP
                session.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpScoDoOptions
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext,
        BOOL                        bUseTls
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_REQUEST_URI               pReqUri      = (PHTTP_REQUEST_URI           )hReqUri;

    returnStatus =
        pMyObject->Request
            (
                (ANSC_HANDLE)pMyObject,
                HTTP_METHOD_CODE_OPTIONS,
                (ANSC_HANDLE)pReqUri,
                hReqContext,
                bUseTls
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpScoDoGet
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hReqUri,
                ANSC_HANDLE                 hReqContext,
                BOOL                        bUseTls
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

                BOOL                        bUseTls
                Specifies whether SSL/TLS should be used for this HTTP
                session.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpScoDoGet
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext,
        BOOL                        bUseTls
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_REQUEST_URI               pReqUri      = (PHTTP_REQUEST_URI           )hReqUri;

    returnStatus =
        pMyObject->Request
            (
                (ANSC_HANDLE)pMyObject,
                HTTP_METHOD_CODE_GET,
                (ANSC_HANDLE)pReqUri,
                hReqContext,
                bUseTls
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpScoDoHead
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hReqUri,
                ANSC_HANDLE                 hReqContext,
                BOOL                        bUseTls
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

                BOOL                        bUseTls
                Specifies whether SSL/TLS should be used for this HTTP
                session.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpScoDoHead
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext,
        BOOL                        bUseTls
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_REQUEST_URI               pReqUri      = (PHTTP_REQUEST_URI           )hReqUri;

    returnStatus =
        pMyObject->Request
            (
                (ANSC_HANDLE)pMyObject,
                HTTP_METHOD_CODE_HEAD,
                (ANSC_HANDLE)pReqUri,
                hReqContext,
                bUseTls
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpScoDoPost
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hReqUri,
                ANSC_HANDLE                 hReqContext,
                BOOL                        bUseTls
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

                BOOL                        bUseTls
                Specifies whether SSL/TLS should be used for this HTTP
                session.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpScoDoPost
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext,
        BOOL                        bUseTls
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_REQUEST_URI               pReqUri      = (PHTTP_REQUEST_URI           )hReqUri;

    returnStatus =
        pMyObject->Request
            (
                (ANSC_HANDLE)pMyObject,
                HTTP_METHOD_CODE_POST,
                (ANSC_HANDLE)pReqUri,
                hReqContext,
                bUseTls
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpScoDoPut
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hReqUri,
                ANSC_HANDLE                 hReqContext,
                BOOL                        bUseTls
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

                BOOL                        bUseTls
                Specifies whether SSL/TLS should be used for this HTTP
                session.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpScoDoPut
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext,
        BOOL                        bUseTls
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_REQUEST_URI               pReqUri      = (PHTTP_REQUEST_URI           )hReqUri;

    returnStatus =
        pMyObject->Request
            (
                (ANSC_HANDLE)pMyObject,
                HTTP_METHOD_CODE_PUT,
                (ANSC_HANDLE)pReqUri,
                hReqContext,
                bUseTls
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpScoDoDelete
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hReqUri,
                ANSC_HANDLE                 hReqContext,
                BOOL                        bUseTls
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

                BOOL                        bUseTls
                Specifies whether SSL/TLS should be used for this HTTP
                session.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpScoDoDelete
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext,
        BOOL                        bUseTls
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_REQUEST_URI               pReqUri      = (PHTTP_REQUEST_URI           )hReqUri;

    returnStatus =
        pMyObject->Request
            (
                (ANSC_HANDLE)pMyObject,
                HTTP_METHOD_CODE_DELETE,
                (ANSC_HANDLE)pReqUri,
                hReqContext,
                bUseTls
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpScoDoTrace
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hReqUri,
                ANSC_HANDLE                 hReqContext,
                BOOL                        bUseTls
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

                BOOL                        bUseTls
                Specifies whether SSL/TLS should be used for this HTTP
                session.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpScoDoTrace
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext,
        BOOL                        bUseTls
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_REQUEST_URI               pReqUri      = (PHTTP_REQUEST_URI           )hReqUri;

    returnStatus =
        pMyObject->Request
            (
                (ANSC_HANDLE)pMyObject,
                HTTP_METHOD_CODE_TRACE,
                (ANSC_HANDLE)pReqUri,
                hReqContext,
                bUseTls
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpScoDoConnect
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hReqUri,
                ANSC_HANDLE                 hReqContext,
                BOOL                        bUseTls
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

                BOOL                        bUseTls
                Specifies whether SSL/TLS should be used for this HTTP
                session.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpScoDoConnect
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext,
        BOOL                        bUseTls
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PHTTP_REQUEST_URI               pReqUri      = (PHTTP_REQUEST_URI           )hReqUri;

    returnStatus =
        pMyObject->Request
            (
                (ANSC_HANDLE)pMyObject,
                HTTP_METHOD_CODE_CONNECT,
                (ANSC_HANDLE)pReqUri,
                hReqContext,
                bUseTls
            );

    return  returnStatus;
}
