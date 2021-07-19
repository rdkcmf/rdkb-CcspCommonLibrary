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
#include "safec_lib_common.h"


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
    UNREFERENCED_PARAMETER(hBmoReq);
    PWEB_RESOURCE_OWNER_OBJECT      pMyObject    = (PWEB_RESOURCE_OWNER_OBJECT   )hThisObject;
    PWEB_RESOURCE_OWNER_PROPERTY    pProperty    = (PWEB_RESOURCE_OWNER_PROPERTY )&pMyObject->Property;

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
    UNREFERENCED_PARAMETER(hThisObject);
    PHTTP_BMO_REQ_OBJECT            pBmoReq         = (PHTTP_BMO_REQ_OBJECT         )hBmoReq;
    PANSC_TOKEN_CHAIN               pPathTokenChain = (PANSC_TOKEN_CHAIN            )pBmoReq->GetRecvContext((ANSC_HANDLE)pBmoReq);
    char*                           pPathInfo       = (char*                        )pBmoReq->RcpMpad;
    ULONG                           ulPathLen       = 0;
    PANSC_STRING_TOKEN              pPathToken      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = NULL;
    errno_t                         rc              = -1;

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

        /* pPathInfo is a pointer, of size HTTP_BMO_REQ_RCP_MPAD_SIZE bytes */
        rc = strcpy_s((char*)(pPathInfo + ulPathLen), (HTTP_BMO_REQ_RCP_MPAD_SIZE - ulPathLen), pPathToken->Name);
        ERR_CHK(rc);

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
    PWEB_RESOURCE_OWNER_OBJECT      pMyObject       = (PWEB_RESOURCE_OWNER_OBJECT   )hThisObject;
    PWEB_RESOURCE_OWNER_PROPERTY    pProperty       = (PWEB_RESOURCE_OWNER_PROPERTY )&pMyObject->Property;
    PHTTP_BMO_REQ_OBJECT            pBmoReq         = (PHTTP_BMO_REQ_OBJECT         )hBmoReq;
    PANSC_TOKEN_CHAIN               pPathTokenChain = (PANSC_TOKEN_CHAIN            )pBmoReq->GetRecvContext((ANSC_HANDLE)pBmoReq);
    char*                           pPathInfo       = (char*                        )pBmoReq->RcpBpad;
    ULONG                           ulPathLen       = 0;
    PANSC_STRING_TOKEN              pPathToken      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = NULL;
    errno_t                         rc              = -1;

    /*pPathInfo is a pointer pointing to data of size HTTP_BMO_REQ_RCP_BPAD_SIZE bytes*/
    rc = strcpy_s(pPathInfo, HTTP_BMO_REQ_RCP_BPAD_SIZE, pProperty->RootPath);
    ERR_CHK(rc);

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

        rc = strcpy_s((char*)(pPathInfo + ulPathLen), (HTTP_BMO_REQ_RCP_BPAD_SIZE - ulPathLen), pPathToken->Name);
        ERR_CHK(rc);

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
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hBmoReq);
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
    UNREFERENCED_PARAMETER(hBmoReq);
    PWEB_RESOURCE_OWNER_OBJECT      pMyObject    = (PWEB_RESOURCE_OWNER_OBJECT   )hThisObject;
    PWEB_RESOURCE_OWNER_PROPERTY    pProperty    = (PWEB_RESOURCE_OWNER_PROPERTY )&pMyObject->Property;

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
    UNREFERENCED_PARAMETER(hThisObject);
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
    UNREFERENCED_PARAMETER(hThisObject);
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
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hBmoReq);

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
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hBmoReq);

    return  HTTP_SERVER_PRODUCT_NAME;
}
