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

    module:	web_roo_rcp.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2002
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This module implements the advanced rcp-interface functions
        of the Web Roo Object.

        *   WebRooRcpGetDocumentRoot
        *   WebRooRcpGetPathInfo
        *   WebRooRcpGetPathTranslated
        *   WebRooRcpGetRemoteUser
        *   WebRooRcpGetScriptName
        *   WebRooRcpGetServerName
        *   WebRooRcpGetServerPort
        *   WebRooRcpGetServerProtocol
        *   WebRooRcpGetServerSoftware

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/16/02    initial revision.

**********************************************************************/


#include "web_roo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        WebRooRcpGetDocumentRoot
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq
            );

    description:

        This function is called to retrieve CGI environment variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the associated request message object.

    return:     CGI environment variable.

**********************************************************************/

char*
WebRooRcpGetDocumentRoot
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PWEB_RESOURCE_OWNER_OBJECT      pMyObject    = (PWEB_RESOURCE_OWNER_OBJECT   )hThisObject;
    PWEB_RESOURCE_OWNER_PROPERTY    pProperty    = (PWEB_RESOURCE_OWNER_PROPERTY )&pMyObject->Property;
    PHTTP_HELPER_CONTAINER_OBJECT   pHttpHco     = (PHTTP_HELPER_CONTAINER_OBJECT)pMyObject->hContainerContext;
    PHTTP_BMO_REQ_OBJECT            pBmoReq      = (PHTTP_BMO_REQ_OBJECT         )hBmoReq;

    return  pProperty->RootPath;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        WebRooRcpGetPathInfo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq
            );

    description:

        This function is called to retrieve CGI environment variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the associated request message object.

    return:     CGI environment variable.

**********************************************************************/

char*
WebRooRcpGetPathInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PWEB_RESOURCE_OWNER_OBJECT      pMyObject       = (PWEB_RESOURCE_OWNER_OBJECT   )hThisObject;
    PWEB_RESOURCE_OWNER_PROPERTY    pProperty       = (PWEB_RESOURCE_OWNER_PROPERTY )&pMyObject->Property;
    PHTTP_HELPER_CONTAINER_OBJECT   pHttpHco        = (PHTTP_HELPER_CONTAINER_OBJECT)pMyObject->hContainerContext;
    PHTTP_BMO_REQ_OBJECT            pBmoReq         = (PHTTP_BMO_REQ_OBJECT         )hBmoReq;
    PANSC_TOKEN_CHAIN               pPathTokenChain = (PANSC_TOKEN_CHAIN            )pBmoReq->GetRecvContext((ANSC_HANDLE)pBmoReq);
    char*                           pPathInfo       = (char*                        )pBmoReq->RcpMpad;
    ULONG                           ulPathLen       = 0;
    PANSC_STRING_TOKEN              pPathToken      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = NULL;

    if ( !pPathTokenChain )
    {
        return NULL;
    }

    pSLinkEntry = AnscQueueGetFirstEntry(&pPathTokenChain->TokensQueue);

    while ( pSLinkEntry )
    {
        pPathToken   = ACCESS_ANSC_STRING_TOKEN(pSLinkEntry);
        pSLinkEntry  = AnscQueueGetNextEntry(pSLinkEntry);

        pPathInfo[ulPathLen++] = '/';

        AnscCopyString((char*)(pPathInfo + ulPathLen), pPathToken->Name);

        ulPathLen += AnscSizeOfString(pPathToken->Name);
    }

    pPathInfo[ulPathLen++] = 0;

    return  pPathInfo;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        WebRooRcpGetPathTranslated
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq
            );

    description:

        This function is called to retrieve CGI environment variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the associated request message object.

    return:     CGI environment variable.

**********************************************************************/

char*
WebRooRcpGetPathTranslated
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PWEB_RESOURCE_OWNER_OBJECT      pMyObject       = (PWEB_RESOURCE_OWNER_OBJECT   )hThisObject;
    PWEB_RESOURCE_OWNER_PROPERTY    pProperty       = (PWEB_RESOURCE_OWNER_PROPERTY )&pMyObject->Property;
    PHTTP_HELPER_CONTAINER_OBJECT   pHttpHco        = (PHTTP_HELPER_CONTAINER_OBJECT)pMyObject->hContainerContext;
    PHTTP_BMO_REQ_OBJECT            pBmoReq         = (PHTTP_BMO_REQ_OBJECT         )hBmoReq;
    PANSC_TOKEN_CHAIN               pPathTokenChain = (PANSC_TOKEN_CHAIN            )pBmoReq->GetRecvContext((ANSC_HANDLE)pBmoReq);
    char*                           pPathInfo       = (char*                        )pBmoReq->RcpBpad;
    ULONG                           ulPathLen       = 0;
    PANSC_STRING_TOKEN              pPathToken      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = NULL;

    AnscCopyString(pPathInfo, pProperty->RootPath);

    ulPathLen = AnscSizeOfString(pProperty->RootPath);

    if ( pPathInfo[ulPathLen - 1] == '/' )
    {
        ulPathLen--;
    }

    pSLinkEntry = AnscQueueGetFirstEntry(&pPathTokenChain->TokensQueue);

    while ( pSLinkEntry )
    {
        pPathToken   = ACCESS_ANSC_STRING_TOKEN(pSLinkEntry);
        pSLinkEntry  = AnscQueueGetNextEntry(pSLinkEntry);

        pPathInfo[ulPathLen++] = '/';

        AnscCopyString((char*)(pPathInfo + ulPathLen), pPathToken->Name);

        ulPathLen += AnscSizeOfString(pPathToken->Name);
    }

    pPathInfo[ulPathLen++] = 0;

    return  pPathInfo;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        WebRooRcpGetRemoteUser
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq
            );

    description:

        This function is called to retrieve CGI environment variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the associated request message object.

    return:     CGI environment variable.

**********************************************************************/

char*
WebRooRcpGetRemoteUser
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PWEB_RESOURCE_OWNER_OBJECT      pMyObject    = (PWEB_RESOURCE_OWNER_OBJECT   )hThisObject;
    PWEB_RESOURCE_OWNER_PROPERTY    pProperty    = (PWEB_RESOURCE_OWNER_PROPERTY )&pMyObject->Property;
    PHTTP_HELPER_CONTAINER_OBJECT   pHttpHco     = (PHTTP_HELPER_CONTAINER_OBJECT)pMyObject->hContainerContext;
    PHTTP_BMO_REQ_OBJECT            pBmoReq      = (PHTTP_BMO_REQ_OBJECT         )hBmoReq;

    return  NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        WebRooRcpGetScriptName
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq
            );

    description:

        This function is called to retrieve CGI environment variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the associated request message object.

    return:     CGI environment variable.

**********************************************************************/

char*
WebRooRcpGetScriptName
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PWEB_RESOURCE_OWNER_OBJECT      pMyObject    = (PWEB_RESOURCE_OWNER_OBJECT   )hThisObject;
    PWEB_RESOURCE_OWNER_PROPERTY    pProperty    = (PWEB_RESOURCE_OWNER_PROPERTY )&pMyObject->Property;
    PHTTP_HELPER_CONTAINER_OBJECT   pHttpHco     = (PHTTP_HELPER_CONTAINER_OBJECT)pMyObject->hContainerContext;
    PHTTP_BMO_REQ_OBJECT            pBmoReq      = (PHTTP_BMO_REQ_OBJECT         )hBmoReq;

    return  pProperty->RegPath;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        WebRooRcpGetServerName
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq
            );

    description:

        This function is called to retrieve CGI environment variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the associated request message object.

    return:     CGI environment variable.

**********************************************************************/

char*
WebRooRcpGetServerName
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PWEB_RESOURCE_OWNER_OBJECT      pMyObject    = (PWEB_RESOURCE_OWNER_OBJECT   )hThisObject;
    PWEB_RESOURCE_OWNER_PROPERTY    pProperty    = (PWEB_RESOURCE_OWNER_PROPERTY )&pMyObject->Property;
    PHTTP_HELPER_CONTAINER_OBJECT   pHttpHco     = (PHTTP_HELPER_CONTAINER_OBJECT)pMyObject->hContainerContext;
    PHTTP_BMO_REQ_OBJECT            pBmoReq      = (PHTTP_BMO_REQ_OBJECT         )hBmoReq;

    return  pBmoReq->GetServerName((ANSC_HANDLE)pBmoReq);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        USHORT
        WebRooRcpGetServerPort
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq
            );

    description:

        This function is called to retrieve CGI environment variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the associated request message object.

    return:     CGI environment variable.

**********************************************************************/

USHORT
WebRooRcpGetServerPort
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PWEB_RESOURCE_OWNER_OBJECT      pMyObject    = (PWEB_RESOURCE_OWNER_OBJECT   )hThisObject;
    PWEB_RESOURCE_OWNER_PROPERTY    pProperty    = (PWEB_RESOURCE_OWNER_PROPERTY )&pMyObject->Property;
    PHTTP_HELPER_CONTAINER_OBJECT   pHttpHco     = (PHTTP_HELPER_CONTAINER_OBJECT)pMyObject->hContainerContext;
    PHTTP_BMO_REQ_OBJECT            pBmoReq      = (PHTTP_BMO_REQ_OBJECT         )hBmoReq;

    return  pBmoReq->GetDaemonPort((ANSC_HANDLE)pBmoReq);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        WebRooRcpGetServerProtocol
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq
            );

    description:

        This function is called to retrieve CGI environment variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the associated request message object.

    return:     CGI environment variable.

**********************************************************************/

char*
WebRooRcpGetServerProtocol
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PWEB_RESOURCE_OWNER_OBJECT      pMyObject    = (PWEB_RESOURCE_OWNER_OBJECT   )hThisObject;
    PWEB_RESOURCE_OWNER_PROPERTY    pProperty    = (PWEB_RESOURCE_OWNER_PROPERTY )&pMyObject->Property;
    PHTTP_HELPER_CONTAINER_OBJECT   pHttpHco     = (PHTTP_HELPER_CONTAINER_OBJECT)pMyObject->hContainerContext;
    PHTTP_BMO_REQ_OBJECT            pBmoReq      = (PHTTP_BMO_REQ_OBJECT         )hBmoReq;

    return  HTTP_CURRENT_VERSION;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        WebRooRcpGetServerSoftware
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq
            );

    description:

        This function is called to retrieve CGI environment variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the associated request message object.

    return:     CGI environment variable.

**********************************************************************/

char*
WebRooRcpGetServerSoftware
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PWEB_RESOURCE_OWNER_OBJECT      pMyObject    = (PWEB_RESOURCE_OWNER_OBJECT   )hThisObject;
    PWEB_RESOURCE_OWNER_PROPERTY    pProperty    = (PWEB_RESOURCE_OWNER_PROPERTY )&pMyObject->Property;
    PHTTP_HELPER_CONTAINER_OBJECT   pHttpHco     = (PHTTP_HELPER_CONTAINER_OBJECT)pMyObject->hContainerContext;
    PHTTP_BMO_REQ_OBJECT            pBmoReq      = (PHTTP_BMO_REQ_OBJECT         )hBmoReq;

    return  HTTP_SERVER_PRODUCT_NAME;
}
