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

    module:	http_bmoreq_cgi.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced cgi-interface functions
        of the Http Bmo Req Object.

        *   HttpBmoReqCgiGetEnvVariableByName
        *   HttpBmoReqCgiGetEnvVariableById
        *   HttpBmoReqCgiGetAuthType
        *   HttpBmoReqCgiGetContentLength
        *   HttpBmoReqCgiGetContentType
        *   HttpBmoReqCgiGetBoundaryDelimiter
        *   HttpBmoReqCgiGetDocumentRoot
        *   HttpBmoReqCgiGetPathInfo
        *   HttpBmoReqCgiGetPathTranslated
        *   HttpBmoReqCgiGetQueryString
        *   HttpBmoReqCgiGetFormContent
        *   HttpBmoReqCgiGetRemoteAddr
        *   HttpBmoReqCgiGetRemoteHost
        *   HttpBmoReqCgiGetRemoteUser
        *   HttpBmoReqCgiGetRequestMethod
        *   HttpBmoReqCgiGetScriptName
        *   HttpBmoReqCgiGetServerName
        *   HttpBmoReqCgiGetServerPort
        *   HttpBmoReqCgiGetServerProtocol
        *   HttpBmoReqCgiGetServerSoftware
        *   HttpBmoReqCgiGetServerType
        *   HttpBmoReqCgiGetHtmlFormInput
        *   HttpBmoReqCgiGetHtmlFormParam
        *   HttpBmoReqCgiGetHtmlFormParam2

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/14/02    initial revision.

**********************************************************************/


#include "http_bmoreq_global.h"

#ifdef _ANSC_HTTP_CGI_USED_

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoReqCgiGetEnvVariableByName
            (
                ANSC_HANDLE                 hThisObject,
                char*                       varName,
                char*                       varValue,
                PULONG                      pulSize
            );

    description:

        This function is called to retrieve CGI environment variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       varName
                Specifies the variable name to be retrieved.

                char*                       varValue
                Specifies the variable value to be retrieved.

                PULONG                      pulSize
                Specifies the size of the buffer as the input parameter
                and the number of bytes been copied when returns.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoReqCgiGetEnvVariableByName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       varName,
        char*                       varValue,
        PULONG                      pulSize
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT         )hThisObject;
    PHTTP_HELPER_CONTAINER_OBJECT   pHttpHco     = (PHTTP_HELPER_CONTAINER_OBJECT)pMyObject->hContainerContext;
    ULONG                           ulCgiEnvId   = 0;

    ulCgiEnvId =
        pHttpHco->GetCgiEnvId
            (
                (ANSC_HANDLE)pHttpHco,
                varName
            );

    returnStatus =
    	HttpBmoReqCgiGetEnvVariableById
            (
                (ANSC_HANDLE)pMyObject,
                ulCgiEnvId,
                varValue,
                pulSize
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoReqCgiGetEnvVariableById
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulCgiVarId,
                char*                       varValue,
                PULONG                      pulSize
            );

    description:

        This function is called to retrieve CGI environment variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulCgiVarId
                Specifies the variable identifier to be retrieved.

                char*                       varValue
                Specifies the variable value to be retrieved.

                PULONG                      pulSize
                Specifies the size of the buffer as the input parameter
                and the number of bytes been copied when returns.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoReqCgiGetEnvVariableById
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCgiVarId,
        char*                       varValue,
        PULONG                      pulSize
    )
{
    ANSC_STATUS                     returnStatus          = ANSC_STATUS_SUCCESS;
    PHTTP_BMO_REQ_OBJECT            pMyObject             = (PHTTP_BMO_REQ_OBJECT         )hThisObject;
    PHTTP_HELPER_CONTAINER_OBJECT   pHttpHco              = (PHTTP_HELPER_CONTAINER_OBJECT)pMyObject->hContainerContext;
    PHTTP_RCP_INTERFACE             pRcpIf                = (PHTTP_RCP_INTERFACE          )pMyObject->hRcpIf;
    PHTTP_REQUEST_INFO              pReqInfo              = (PHTTP_REQUEST_INFO           )pMyObject->hReqInfo;
    PHTTP_HFO_AUTHORIZATION         pHttpHfoAuthorization = NULL;
    char*                           pHttpHfValue          = NULL;
    char*                           pHttpPath             = NULL;
    char*                           pHttpUser             = NULL;
    char*                           pHttpRequestMethod    = NULL;
    char*                           pScriptName           = NULL;
    char*                           pServerName           = NULL;
    char*                           pServerProtocol       = NULL;
    char*                           pServerSoftware       = NULL;
    ULONG                           ulContentLength       = 0;
    USHORT                          usServerPort          = HTTP_SERVER_PORT;
    char*                           pCgiEnvVarValue       = NULL;
    ULONG                           ulVarValueSize        =0;

    AnscZeroMemory(varValue, *pulSize);

    switch ( ulCgiVarId )
    {
        case    CGI_ENV_CODE_AUTH_TYPE :

                pHttpHfoAuthorization =
                    (PHTTP_HFO_AUTHORIZATION)pMyObject->GetHeaderField
                        (
                            (ANSC_HANDLE)pMyObject,
                            HTTP_HEADER_ID_AUTHORIZATION
                        );

                if ( !pHttpHfoAuthorization )
                {
                    pCgiEnvVarValue = HTTP_AUTH_TEXT_NONE;
                    ulVarValueSize  = AnscSizeOfString(pCgiEnvVarValue);
                }
                else if ( pHttpHfoAuthorization->Credential.AuthType == HTTP_AUTH_TYPE_BASIC )
                {
                    pCgiEnvVarValue = HTTP_AUTH_TEXT_BASIC;
                    ulVarValueSize  = AnscSizeOfString(pCgiEnvVarValue);
                }
                else if ( pHttpHfoAuthorization->Credential.AuthType == HTTP_AUTH_TYPE_DIGEST )
                {
                    pCgiEnvVarValue = HTTP_AUTH_TEXT_DIGEST;
                    ulVarValueSize  = AnscSizeOfString(pCgiEnvVarValue);
                }
                else
                {
                    pCgiEnvVarValue = HTTP_AUTH_TEXT_NONE;
                    ulVarValueSize  = AnscSizeOfString(pCgiEnvVarValue);
                }

                break;

        case    CGI_ENV_CODE_CONTENT_LENGTH :

                pHttpHfValue =
                    pMyObject->GetHeaderValueById
                        (
                            (ANSC_HANDLE)pMyObject,
                            HTTP_HEADER_ID_CONTENT_LENGTH
                        );

                if ( pHttpHfValue )
                {
                    pCgiEnvVarValue = pHttpHfValue;
                    ulVarValueSize  = AnscSizeOfString(pCgiEnvVarValue);
                }
                else
                {
                    ulContentLength = pMyObject->GetBodySize((ANSC_HANDLE)pMyObject);

                    AnscZeroMemory    (pMyObject->RcpIpad, pMyObject->RcpIpadSize);
                    AnscGetUlongString(ulContentLength,    pMyObject->RcpIpad    );

                    pCgiEnvVarValue = pMyObject->RcpIpad;
                    ulVarValueSize  = AnscSizeOfString(pCgiEnvVarValue);
                }

                break;

        case    CGI_ENV_CODE_CONTENT_TYPE :

                pHttpHfValue =
                    pMyObject->GetHeaderValueById
                        (
                            (ANSC_HANDLE)pMyObject,
                            HTTP_HEADER_ID_CONTENT_TYPE
                        );

                if ( pHttpHfValue )
                {
                    pCgiEnvVarValue = pHttpHfValue;
                    ulVarValueSize  = AnscSizeOfString(pCgiEnvVarValue);
                }

                break;

        case    CGI_ENV_CODE_DOCUMENT_ROOT :

                pHttpPath =
                    pRcpIf->GetDocumentRoot
                        (
                            pRcpIf->hOwnerContext,
                            (ANSC_HANDLE)pMyObject
                        );

                if ( pHttpPath )
                {
                    pCgiEnvVarValue = pHttpPath;
                    ulVarValueSize  = AnscSizeOfString(pCgiEnvVarValue);
                }

                break;

        case    CGI_ENV_CODE_PATH_INFO :

                pHttpPath =
                    pRcpIf->GetPathInfo
                        (
                            pRcpIf->hOwnerContext,
                            (ANSC_HANDLE)pMyObject
                        );

                if ( pHttpPath )
                {
                    pCgiEnvVarValue = pHttpPath;
                    ulVarValueSize  = AnscSizeOfString(pCgiEnvVarValue);
                }

                break;

        case    CGI_ENV_CODE_PATH_TRANSLATED :

                pHttpPath =
                    pRcpIf->GetPathTranslated
                        (
                            pRcpIf->hOwnerContext,
                            (ANSC_HANDLE)pMyObject
                        );

                if ( pHttpPath )
                {
                    pCgiEnvVarValue = pHttpPath;
                    ulVarValueSize  = AnscSizeOfString(pCgiEnvVarValue);
                }

                break;

        case    CGI_ENV_CODE_QUERY_STRING :

                pCgiEnvVarValue = pReqInfo->RequestUri.QueryParams;
                ulVarValueSize  = AnscSizeOfString(pCgiEnvVarValue);

                break;

        case    CGI_ENV_CODE_FORM_CONTENT :

                if ( pReqInfo->Method == HTTP_METHOD_GET )
                {
                    pCgiEnvVarValue = pReqInfo->RequestUri.QueryParams;
                    ulVarValueSize  = AnscSizeOfString(pCgiEnvVarValue);
                }
                else if ( pReqInfo->Method == HTTP_METHOD_POST )
                {
                    returnStatus =
                        pMyObject->CopyBodyFrom
                            (
                                (ANSC_HANDLE)pMyObject,
                                varValue,
                                pulSize
                            );

                    return  returnStatus;
                }

                break;

        case    CGI_ENV_CODE_REMOTE_ADDR :
        case    CGI_ENV_CODE_REMOTE_HOST :

                AnscZeroMemory     (pMyObject->RcpIpad,        pMyObject->RcpIpadSize);
                AnscGetIpAddrString(pMyObject->ClientAddr.Dot, pMyObject->RcpIpad    );

                pCgiEnvVarValue = pMyObject->RcpIpad;
                ulVarValueSize  = AnscSizeOfString(pCgiEnvVarValue);

                break;

        case    CGI_ENV_CODE_REMOTE_USER :

                pHttpUser =
                    pRcpIf->GetRemoteUser
                        (
                            pRcpIf->hOwnerContext,
                            (ANSC_HANDLE)pMyObject
                        );

                if ( pHttpUser )
                {
                    pCgiEnvVarValue = pHttpUser;
                    ulVarValueSize  = AnscSizeOfString(pCgiEnvVarValue);
                }

                break;

        case    CGI_ENV_CODE_REQUEST_METHOD :

                pHttpRequestMethod =
                    pHttpHco->GetMethodName
                        (
                            (ANSC_HANDLE)pHttpHco,
                            pReqInfo->Method
                        );

                if ( pHttpRequestMethod )
                {
                    pCgiEnvVarValue = pHttpRequestMethod;
                    ulVarValueSize  = AnscSizeOfString(pCgiEnvVarValue);
                }

                break;

        case    CGI_ENV_CODE_SCRIPT_NAME :

                pScriptName =
                    pRcpIf->GetScriptName
                        (
                            pRcpIf->hOwnerContext,
                            (ANSC_HANDLE)pMyObject
                        );

                if ( pScriptName )
                {
                    pCgiEnvVarValue = pScriptName;
                    ulVarValueSize  = AnscSizeOfString(pCgiEnvVarValue);
                }

                break;

        case    CGI_ENV_CODE_SERVER_NAME :

                pServerName =
                    pRcpIf->GetServerName
                        (
                            pRcpIf->hOwnerContext,
                            (ANSC_HANDLE)pMyObject
                        );

                if ( pServerName )
                {
                    pCgiEnvVarValue = pServerName;
                    ulVarValueSize  = AnscSizeOfString(pCgiEnvVarValue);
                }

                break;

        case    CGI_ENV_CODE_SERVER_PORT :

                usServerPort =
                    pRcpIf->GetServerPort
                        (
                            pRcpIf->hOwnerContext,
                            (ANSC_HANDLE)pMyObject
                        );

                AnscZeroMemory    (pMyObject->RcpIpad,  pMyObject->RcpIpadSize);
                AnscGetUlongString((ULONG)usServerPort, pMyObject->RcpIpad    );

                pCgiEnvVarValue = pMyObject->RcpIpad;
                ulVarValueSize  = AnscSizeOfString(pCgiEnvVarValue);

                break;

        case    CGI_ENV_CODE_SERVER_PROTOCOL :

                pServerProtocol =
                    pRcpIf->GetServerProtocol
                        (
                            pRcpIf->hOwnerContext,
                            (ANSC_HANDLE)pMyObject
                        );

                if ( pServerProtocol )
                {
                    pCgiEnvVarValue = pServerProtocol;
                    ulVarValueSize  = AnscSizeOfString(pCgiEnvVarValue);
                }

                break;

        case    CGI_ENV_CODE_SERVER_SOFTWARE :

                pServerSoftware =
                    pRcpIf->GetServerSoftware
                        (
                            pRcpIf->hOwnerContext,
                            (ANSC_HANDLE)pMyObject
                        );

                if ( pServerSoftware )
                {
                    pCgiEnvVarValue = pServerSoftware;
                    ulVarValueSize  = AnscSizeOfString(pCgiEnvVarValue);
                }

                break;

        case    CGI_ENV_CODE_HTTP_ACCEPT :
        case    CGI_ENV_CODE_HTTP_ACCEPT_CHARSET :
        case    CGI_ENV_CODE_HTTP_ACCEPT_ENCODING :
        case    CGI_ENV_CODE_HTTP_ACCEPT_LANGUAGE :
        case    CGI_ENV_CODE_HTTP_ACCEPT_RANGES :
        case    CGI_ENV_CODE_HTTP_AGE :
        case    CGI_ENV_CODE_HTTP_ALLOW :
        case    CGI_ENV_CODE_HTTP_AUTHORIZATION :
        case    CGI_ENV_CODE_HTTP_CACHE_CONTROL :
        case    CGI_ENV_CODE_HTTP_CONNECTION :
        case    CGI_ENV_CODE_HTTP_CONTENT_ENCODING :
        case    CGI_ENV_CODE_HTTP_CONTENT_LANGUAGE :
        case    CGI_ENV_CODE_HTTP_CONTENT_LENGTH :
        case    CGI_ENV_CODE_HTTP_CONTENT_LOCATION :
        case    CGI_ENV_CODE_HTTP_CONTENT_MD5 :
        case    CGI_ENV_CODE_HTTP_CONTENT_RANGE :
        case    CGI_ENV_CODE_HTTP_CONTENT_TYPE :
        case    CGI_ENV_CODE_HTTP_DATE :
        case    CGI_ENV_CODE_HTTP_ETAG :
        case    CGI_ENV_CODE_HTTP_EXPECT :
        case    CGI_ENV_CODE_HTTP_EXPIRES :
        case    CGI_ENV_CODE_HTTP_FROM :
        case    CGI_ENV_CODE_HTTP_HOST :
        case    CGI_ENV_CODE_HTTP_IF_MATCH :
        case    CGI_ENV_CODE_HTTP_IF_MODIFIED_SINCE :
        case    CGI_ENV_CODE_HTTP_IF_NONE_MATCH :
        case    CGI_ENV_CODE_HTTP_IF_RANGE :
        case    CGI_ENV_CODE_HTTP_IF_UNMODIFIED_SINCE :
        case    CGI_ENV_CODE_HTTP_LAST_MODIFIED :
        case    CGI_ENV_CODE_HTTP_LOCATION :
        case    CGI_ENV_CODE_HTTP_MAX_FORWARDS :
        case    CGI_ENV_CODE_HTTP_PRAGMA :
        case    CGI_ENV_CODE_HTTP_PROXY_AUTHENTICATE :
        case    CGI_ENV_CODE_HTTP_PROXY_AUTHORIZATION :
        case    CGI_ENV_CODE_HTTP_RANGE :
        case    CGI_ENV_CODE_HTTP_REFERER :
        case    CGI_ENV_CODE_HTTP_RETRY_AFTER :
        case    CGI_ENV_CODE_HTTP_SERVER :
        case    CGI_ENV_CODE_HTTP_TE :
        case    CGI_ENV_CODE_HTTP_TRAILER :
        case    CGI_ENV_CODE_HTTP_TRANSFER_ENCODING :
        case    CGI_ENV_CODE_HTTP_UPGRADE :
        case    CGI_ENV_CODE_HTTP_USER_AGENT :
        case    CGI_ENV_CODE_HTTP_VARY :
        case    CGI_ENV_CODE_HTTP_VIA :
        case    CGI_ENV_CODE_HTTP_WARNING :
        case    CGI_ENV_CODE_HTTP_WWW_AUTHENTICATE :
        case    CGI_ENV_CODE_HTTP_PROXY_CONNECTION :
        case    CGI_ENV_CODE_HTTP_COOKIE :
        case    CGI_ENV_CODE_HTTP_SET_COOKIE :

                pHttpHfValue =
                    pMyObject->GetHeaderValueById
                        (
                            (ANSC_HANDLE)pMyObject,
                            ulCgiVarId - CGI_ENV_CODE_HTTP
                        );

                if ( pHttpHfValue )
                {
                    pCgiEnvVarValue = pHttpHfValue;
                    ulVarValueSize  = AnscSizeOfString(pCgiEnvVarValue);
                }

                break;

        default :

                pCgiEnvVarValue = NULL;
                ulVarValueSize  = 0;

                break;

    }

    if ( !pCgiEnvVarValue || !ulVarValueSize )
    {
        return  ANSC_STATUS_NOT_SPECIFIED;
    }
    else if ( *pulSize < ulVarValueSize )
    {
        *pulSize = ulVarValueSize;

        return  ANSC_STATUS_BAD_SIZE;
    }
    else
    {
        *pulSize = ulVarValueSize;
    }

    AnscCopyMemory(varValue, pCgiEnvVarValue, ulVarValueSize);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpBmoReqCgiGetAuthType
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve CGI environment variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     CGI environment variable.

**********************************************************************/

ULONG
HttpBmoReqCgiGetAuthType
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject             = (PHTTP_BMO_REQ_OBJECT         )hThisObject;
    PHTTP_HFO_AUTHORIZATION         pHttpHfoAuthorization = NULL;
    ULONG                           ulAuthType            = HTTP_AUTH_TYPE_NONE;

    pHttpHfoAuthorization =
        (PHTTP_HFO_AUTHORIZATION)pMyObject->GetHeaderField
            (
                (ANSC_HANDLE)pMyObject,
                HTTP_HEADER_ID_AUTHORIZATION
            );

    if ( pHttpHfoAuthorization )
    {
        ulAuthType = pHttpHfoAuthorization->Credential.AuthType;
    }
    else
    {
        ulAuthType = HTTP_AUTH_TYPE_NONE;
    }

    return  ulAuthType;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpBmoReqCgiGetContentLength
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve CGI environment variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     CGI environment variable.

**********************************************************************/

ULONG
HttpBmoReqCgiGetContentLength
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject             = (PHTTP_BMO_REQ_OBJECT         )hThisObject;
    PHTTP_HFO_CONTENT_LENGTH        pHttpHfoContentLength = NULL;
    ULONG                           ulContentLength       = 0;

    pHttpHfoContentLength =
        (PHTTP_HFO_CONTENT_LENGTH)pMyObject->GetHeaderField
            (
                (ANSC_HANDLE)pMyObject,
                HTTP_HEADER_ID_CONTENT_LENGTH
            );

    if ( pHttpHfoContentLength )
    {
        ulContentLength = pHttpHfoContentLength->BodySize;
    }
    else
    {
        ulContentLength = pMyObject->GetBodySize((ANSC_HANDLE)pMyObject);
    }

    return  ulContentLength;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoReqCgiGetContentType
            (
                ANSC_HANDLE                 hThisObject,
                PULONG                      pulMediaType,
                PULONG                      pulSubType
            );

    description:

        This function is called to retrieve CGI environment variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PULONG                      pulMediaType
                Specifies the CGI environment variable to be filled.

                PULONG                      pulSubType
                Specifies the CGI environment variable to be filled.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpBmoReqCgiGetContentType
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pulMediaType,
        PULONG                      pulSubType
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject           = (PHTTP_BMO_REQ_OBJECT         )hThisObject;
    PHTTP_HFO_CONTENT_TYPE          pHttpHfoContentType = NULL;

    pHttpHfoContentType =
        (PHTTP_HFO_CONTENT_TYPE)pMyObject->GetHeaderField
            (
                (ANSC_HANDLE)pMyObject,
                HTTP_HEADER_ID_CONTENT_TYPE
            );

    if ( pHttpHfoContentType )
    {
        *pulMediaType = pHttpHfoContentType->MediaType;
        *pulSubType   = pHttpHfoContentType->SubType;
    }
    else
    {
        return  ANSC_STATUS_NOT_SPECIFIED;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        HttpBmoReqCgiGetBoundaryDelimiter
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve CGI environment variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     boundary delimiter.

**********************************************************************/

char*
HttpBmoReqCgiGetBoundaryDelimiter
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject           = (PHTTP_BMO_REQ_OBJECT         )hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf              = (PHTTP_HFP_INTERFACE          )pMyObject->hHfpIf;
    PHTTP_HFO_CONTENT_TYPE          pHttpHfoContentType = NULL;

    pHttpHfoContentType =
        (PHTTP_HFO_CONTENT_TYPE)pMyObject->GetHeaderField
            (
                (ANSC_HANDLE)pMyObject,
                HTTP_HEADER_ID_CONTENT_TYPE
            );

    if ( pHttpHfoContentType )
    {
        return  pHfpIf->GetHeaderParam
                    (
                        pHfpIf->hOwnerContext,
                        pHttpHfoContentType->Parameters,
                        "boundary"
                    );
    }

    return  NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        HttpBmoReqCgiGetDocumentRoot
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve CGI environment variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     CGI environment variable.

**********************************************************************/

char*
HttpBmoReqCgiGetDocumentRoot
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT         )hThisObject;
    PHTTP_RCP_INTERFACE             pRcpIf       = (PHTTP_RCP_INTERFACE          )pMyObject->hRcpIf;

    return  pRcpIf->GetDocumentRoot(pRcpIf->hOwnerContext, (ANSC_HANDLE)pMyObject);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        HttpBmoReqCgiGetPathInfo
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve CGI environment variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     CGI environment variable.

**********************************************************************/

char*
HttpBmoReqCgiGetPathInfo
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT         )hThisObject;
    PHTTP_RCP_INTERFACE             pRcpIf       = (PHTTP_RCP_INTERFACE          )pMyObject->hRcpIf;

    return  pRcpIf->GetPathInfo(pRcpIf->hOwnerContext, (ANSC_HANDLE)pMyObject);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        HttpBmoReqCgiGetPathTranslated
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve CGI environment variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     CGI environment variable.

**********************************************************************/

char*
HttpBmoReqCgiGetPathTranslated
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT         )hThisObject;
    PHTTP_RCP_INTERFACE             pRcpIf       = (PHTTP_RCP_INTERFACE          )pMyObject->hRcpIf;

    return  pRcpIf->GetPathTranslated(pRcpIf->hOwnerContext, (ANSC_HANDLE)pMyObject);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        HttpBmoReqCgiGetQueryString
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve CGI environment variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     CGI environment variable.

**********************************************************************/

char*
HttpBmoReqCgiGetQueryString
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT         )hThisObject;
    PHTTP_REQUEST_INFO              pReqInfo     = (PHTTP_REQUEST_INFO           )pMyObject->hReqInfo;
    
    return  pReqInfo->RequestUri.QueryParams;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        HttpBmoReqCgiGetFormContent
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve CGI environment variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     CGI environment variable.

**********************************************************************/

char*
HttpBmoReqCgiGetFormContent
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT         )hThisObject;
    PHTTP_REQUEST_INFO              pReqInfo     = (PHTTP_REQUEST_INFO           )pMyObject->hReqInfo;
    ULONG                           ulBodySize   = pMyObject->GetBodySize((ANSC_HANDLE)pMyObject);

    if ( pReqInfo->Method == HTTP_METHOD_CODE_GET )
    {
        return  pReqInfo->RequestUri.QueryParams;
    }
    else if ( pReqInfo->Method == HTTP_METHOD_CODE_POST )
    {
        if ( (ulBodySize == 0) )
        {
            return NULL;
        }
        else if ( (ulBodySize > HTTP_MAX_QUERY_STRING_SIZE) )
        {
            PHTTP_HFP_INTERFACE     pHfpIf       = (PHTTP_HFP_INTERFACE)pMyObject->hHfpIf;
            PHTTP_CGI_INTERFACE     pCgiIf       = (PHTTP_CGI_INTERFACE)pMyObject->hCgiIf;
            char*                   pBoundaryStr = pCgiIf->GetBoundaryDelimiter(pCgiIf->hOwnerContext);
            char*                   pFormData    = NULL;
            ULONG                   ulFormDataLen= 0;

            if ( !pBoundaryStr )
            {
                return NULL;
            }

            pFormData =
                pHfpIf->GetFormDataNoFile
                    (
                        pHfpIf->hOwnerContext,
                        pBoundaryStr,
                        pMyObject->hMessageBody,
                        &ulFormDataLen
                    );

            if ( pMyObject->RcpDpad )
            {
                AnscFreeMemory(pMyObject->RcpDpad);
            }

            pMyObject->RcpDpad      = pFormData;
            pMyObject->RcpDpadSize  = ulFormDataLen;

            return pMyObject->RcpDpad;
        }
        else if ( pMyObject->RcpDpadSize < (ulBodySize + 16) )
        {
            if ( pMyObject->RcpDpad )
            {
                AnscFreeMemory(pMyObject->RcpDpad);

                pMyObject->RcpDpadSize = 0;
            }

            pMyObject->RcpDpadSize = ulBodySize + 16;
            pMyObject->RcpDpad     = AnscAllocateMemory(pMyObject->RcpDpadSize);

            if ( !pMyObject->RcpDpad )
            {
                pMyObject->RcpDpadSize = 0;

                return  NULL;
            }
        }

        returnStatus =
            pMyObject->CopyBodyFrom
                (
                    (ANSC_HANDLE)pMyObject,
                    pMyObject->RcpDpad,
                    &ulBodySize
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            return  NULL;
        }
        else
        {
            pMyObject->RcpDpad[ulBodySize] = 0;
        }

        return  pMyObject->RcpDpad;
    }

    return  NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        PUCHAR
        HttpBmoReqCgiGetRemoteAddr
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve CGI environment variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     CGI environment variable.

**********************************************************************/

PUCHAR
HttpBmoReqCgiGetRemoteAddr
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT         )hThisObject;
    
    return  pMyObject->ClientAddr.Dot;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        HttpBmoReqCgiGetRemoteHost
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve CGI environment variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     CGI environment variable.

**********************************************************************/

char*
HttpBmoReqCgiGetRemoteHost
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT         )hThisObject;
    
    AnscZeroMemory     (pMyObject->RcpIpad,        pMyObject->RcpIpadSize);
    AnscGetIpAddrString(pMyObject->ClientAddr.Dot, pMyObject->RcpIpad    );

    return  pMyObject->RcpIpad;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        HttpBmoReqCgiGetRemoteUser
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve CGI environment variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     CGI environment variable.

**********************************************************************/

char*
HttpBmoReqCgiGetRemoteUser
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT         )hThisObject;
    PHTTP_RCP_INTERFACE             pRcpIf       = (PHTTP_RCP_INTERFACE          )pMyObject->hRcpIf;

    return  pRcpIf->GetRemoteUser(pRcpIf->hOwnerContext, (ANSC_HANDLE)pMyObject);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpBmoReqCgiGetRequestMethod
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve CGI environment variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     CGI environment variable.

**********************************************************************/

ULONG
HttpBmoReqCgiGetRequestMethod
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT         )hThisObject;
    PHTTP_REQUEST_INFO              pReqInfo     = (PHTTP_REQUEST_INFO           )pMyObject->hReqInfo;

    return  pReqInfo->Method;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        HttpBmoReqCgiGetScriptName
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve CGI environment variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     CGI environment variable.

**********************************************************************/

char*
HttpBmoReqCgiGetScriptName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT         )hThisObject;
    PHTTP_RCP_INTERFACE             pRcpIf       = (PHTTP_RCP_INTERFACE          )pMyObject->hRcpIf;

    return  pRcpIf->GetScriptName(pRcpIf->hOwnerContext, (ANSC_HANDLE)pMyObject);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        HttpBmoReqCgiGetServerName
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve CGI environment variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     CGI environment variable.

**********************************************************************/

char*
HttpBmoReqCgiGetServerName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT         )hThisObject;
    PHTTP_RCP_INTERFACE             pRcpIf       = (PHTTP_RCP_INTERFACE          )pMyObject->hRcpIf;

    return  pRcpIf->GetServerName(pRcpIf->hOwnerContext, (ANSC_HANDLE)pMyObject);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        USHORT
        HttpBmoReqCgiGetServerPort
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve CGI environment variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     CGI environment variable.

**********************************************************************/

USHORT
HttpBmoReqCgiGetServerPort
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT         )hThisObject;
    PHTTP_RCP_INTERFACE             pRcpIf       = (PHTTP_RCP_INTERFACE          )pMyObject->hRcpIf;

    return  pRcpIf->GetServerPort(pRcpIf->hOwnerContext, (ANSC_HANDLE)pMyObject);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        HttpBmoReqCgiGetServerProtocol
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve CGI environment variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     CGI environment variable.

**********************************************************************/

char*
HttpBmoReqCgiGetServerProtocol
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT         )hThisObject;
    PHTTP_RCP_INTERFACE             pRcpIf       = (PHTTP_RCP_INTERFACE          )pMyObject->hRcpIf;

    return  pRcpIf->GetServerProtocol(pRcpIf->hOwnerContext, (ANSC_HANDLE)pMyObject);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        HttpBmoReqCgiGetServerSoftware
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve CGI environment variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     CGI environment variable.

**********************************************************************/

char*
HttpBmoReqCgiGetServerSoftware
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT         )hThisObject;
    PHTTP_RCP_INTERFACE             pRcpIf       = (PHTTP_RCP_INTERFACE          )pMyObject->hRcpIf;

    return  pRcpIf->GetServerSoftware(pRcpIf->hOwnerContext, (ANSC_HANDLE)pMyObject);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        HttpBmoReqCgiGetServerType
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve CGI environment variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     CGI environment variable.

**********************************************************************/

char*
HttpBmoReqCgiGetServerType
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT         )hThisObject;
    
    if ( pMyObject->ServerMode & HTTP_SSO_MODE_TLS_ENABLED )
    {
        return  "https";
    }

    return  "http";
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpBmoReqCgiGetHtmlFormInput
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve HTML form input data.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     form input data.

**********************************************************************/

ANSC_HANDLE
HttpBmoReqCgiGetHtmlFormInput
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT         )hThisObject;
    PHTTP_FORM_CONTENT_OBJECT       pFormContent = (PHTTP_FORM_CONTENT_OBJECT    )pMyObject->hFormContent;

    if ( !pFormContent )
    {
        returnStatus = pMyObject->GenFormContent((ANSC_HANDLE)pMyObject);
        pFormContent = (PHTTP_FORM_CONTENT_OBJECT)pMyObject->hFormContent;

        if ( (returnStatus != ANSC_STATUS_SUCCESS) || !pFormContent )
        {
            return  (ANSC_HANDLE)NULL;
        }
    }

    return  pFormContent->GetFormInput((ANSC_HANDLE)pFormContent);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        HttpBmoReqCgiGetHtmlFormParam
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name
            );

    description:

        This function is called to retrieve HTML form element data.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the HTML form element to be retrieved.

    return:     form element data.

**********************************************************************/

char*
HttpBmoReqCgiGetHtmlFormParam
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT         )hThisObject;
    PHTTP_FORM_CONTENT_OBJECT       pFormContent = (PHTTP_FORM_CONTENT_OBJECT    )pMyObject->hFormContent;

    if ( !pFormContent )
    {
        returnStatus = pMyObject->GenFormContent((ANSC_HANDLE)pMyObject);
        pFormContent = (PHTTP_FORM_CONTENT_OBJECT)pMyObject->hFormContent;

        if ( (returnStatus != ANSC_STATUS_SUCCESS) || !pFormContent )
        {
            return  NULL;
        }
    }

    return  pFormContent->GetFormParam
                (
                    (ANSC_HANDLE)pFormContent,
                    name
                );
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void*
        HttpBmoReqCgiGetHtmlFormParam2
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name,
                PULONG                      pulSize
            );

    description:

        This function is called to retrieve HTML form element data.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the HTML form element to be retrieved.

                PULONG                      pulSize
                Specifies the size of the element to be returned.

    return:     form element data.

**********************************************************************/

void*
HttpBmoReqCgiGetHtmlFormParam2
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        PULONG                      pulSize
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT         )hThisObject;
    PHTTP_FORM_CONTENT_OBJECT       pFormContent = (PHTTP_FORM_CONTENT_OBJECT    )pMyObject->hFormContent;

    if ( !pFormContent )
    {
        returnStatus = pMyObject->GenFormContent((ANSC_HANDLE)pMyObject);
        pFormContent = (PHTTP_FORM_CONTENT_OBJECT)pMyObject->hFormContent;

        if ( (returnStatus != ANSC_STATUS_SUCCESS) || !pFormContent )
        {
            *pulSize = 0;

            return  NULL;
        }
    }

    return  pFormContent->GetFormParam2
                (
                    (ANSC_HANDLE)pFormContent,
                    name,
                    pulSize
                );
}

#endif
