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

    module:	web_roo_do_stuff_upnp.c

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2005
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the Web Resource Owner Object.

        *   WebRooDoNotify
        *   WebRooDoSearch
        *   WebRooDoMSearch
        *   WebRooDoMPost
        *   WebRooDoSubscribe
        *   WebRooDoUnsubscribe

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/02/05    initial revision.

**********************************************************************/


#include "web_roo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooDoNotify
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq,
                ANSC_HANDLE                 hBmoRep
            );

    description:

        This function is called to process a web request message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the HTTP request message object.

                ANSC_HANDLE                 hBmoRep
                Specifies the HTTP response message object.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebRooDoNotify
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PWEB_RESOURCE_OWNER_OBJECT      pMyObject       = (PWEB_RESOURCE_OWNER_OBJECT  )hThisObject;
    PWEB_RESOURCE_OWNER_PROPERTY    pProperty       = (PWEB_RESOURCE_OWNER_PROPERTY)&pMyObject->Property;
    PHTTP_RCP_INTERFACE             pRcpIf          = (PHTTP_RCP_INTERFACE         )pMyObject->hRcpIf;
    PHTTP_BMO_REQ_OBJECT            pBmoReq         = (PHTTP_BMO_REQ_OBJECT        )hBmoReq;
    PHTTP_BMO_REP_OBJECT            pBmoRep         = (PHTTP_BMO_REP_OBJECT        )hBmoRep;
    PHTTP_REQUEST_INFO              pReqInfo        = (PHTTP_REQUEST_INFO          )pBmoReq->GetReqInfo    ((ANSC_HANDLE)pBmoReq);
    PANSC_TOKEN_CHAIN               pPathTokenChain = (PANSC_TOKEN_CHAIN           )pBmoReq->GetRecvContext((ANSC_HANDLE)pBmoReq);
    PWEB_GENERAL_SESSION_OBJECT     pSession        = (PWEB_GENERAL_SESSION_OBJECT )pBmoRep->GetRecvContext((ANSC_HANDLE)pBmoRep);
    PHTTP_HFO_ALLOW                 pHttpHfoAllow   = NULL;

    pHttpHfoAllow = (PHTTP_HFO_ALLOW)AnscAllocateMemory(sizeof(HTTP_HFO_ALLOW));

    if ( !pHttpHfoAllow )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pHttpHfoAllow->HashIndex      = 0;
        pHttpHfoAllow->Flags          = 0;
        pHttpHfoAllow->HeaderId       = HTTP_HEADER_ID_ALLOW;

        pHttpHfoAllow->MethodCount    = 4;
        pHttpHfoAllow->MethodArray[0] = HTTP_METHOD_CODE_OPTIONS;
        pHttpHfoAllow->MethodArray[1] = HTTP_METHOD_CODE_GET;
        pHttpHfoAllow->MethodArray[2] = HTTP_METHOD_CODE_HEAD;
        pHttpHfoAllow->MethodArray[3] = HTTP_METHOD_CODE_POST;
    }

    returnStatus =
        pMyObject->ReplyReq
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pBmoReq,
                (ANSC_HANDLE)pBmoRep,
                HTTP_STATUS_METHOD_NOT_ALLOWED,
                (ANSC_HANDLE)pHttpHfoAllow
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooDoSearch
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq,
                ANSC_HANDLE                 hBmoRep
            );

    description:

        This function is called to process a web request message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the HTTP request message object.

                ANSC_HANDLE                 hBmoRep
                Specifies the HTTP response message object.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebRooDoSearch
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PWEB_RESOURCE_OWNER_OBJECT      pMyObject       = (PWEB_RESOURCE_OWNER_OBJECT  )hThisObject;
    PWEB_RESOURCE_OWNER_PROPERTY    pProperty       = (PWEB_RESOURCE_OWNER_PROPERTY)&pMyObject->Property;
    PHTTP_RCP_INTERFACE             pRcpIf          = (PHTTP_RCP_INTERFACE         )pMyObject->hRcpIf;
    PHTTP_BMO_REQ_OBJECT            pBmoReq         = (PHTTP_BMO_REQ_OBJECT        )hBmoReq;
    PHTTP_BMO_REP_OBJECT            pBmoRep         = (PHTTP_BMO_REP_OBJECT        )hBmoRep;
    PHTTP_REQUEST_INFO              pReqInfo        = (PHTTP_REQUEST_INFO          )pBmoReq->GetReqInfo    ((ANSC_HANDLE)pBmoReq);
    PANSC_TOKEN_CHAIN               pPathTokenChain = (PANSC_TOKEN_CHAIN           )pBmoReq->GetRecvContext((ANSC_HANDLE)pBmoReq);
    PWEB_GENERAL_SESSION_OBJECT     pSession        = (PWEB_GENERAL_SESSION_OBJECT )pBmoRep->GetRecvContext((ANSC_HANDLE)pBmoRep);
    PHTTP_HFO_ALLOW                 pHttpHfoAllow   = NULL;

    pHttpHfoAllow = (PHTTP_HFO_ALLOW)AnscAllocateMemory(sizeof(HTTP_HFO_ALLOW));

    if ( !pHttpHfoAllow )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pHttpHfoAllow->HashIndex      = 0;
        pHttpHfoAllow->Flags          = 0;
        pHttpHfoAllow->HeaderId       = HTTP_HEADER_ID_ALLOW;

        pHttpHfoAllow->MethodCount    = 4;
        pHttpHfoAllow->MethodArray[0] = HTTP_METHOD_CODE_OPTIONS;
        pHttpHfoAllow->MethodArray[1] = HTTP_METHOD_CODE_GET;
        pHttpHfoAllow->MethodArray[2] = HTTP_METHOD_CODE_HEAD;
        pHttpHfoAllow->MethodArray[3] = HTTP_METHOD_CODE_POST;
    }

    returnStatus =
        pMyObject->ReplyReq
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pBmoReq,
                (ANSC_HANDLE)pBmoRep,
                HTTP_STATUS_METHOD_NOT_ALLOWED,
                (ANSC_HANDLE)pHttpHfoAllow
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooDoMSearch
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq,
                ANSC_HANDLE                 hBmoRep
            );

    description:

        This function is called to process a web request message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the HTTP request message object.

                ANSC_HANDLE                 hBmoRep
                Specifies the HTTP response message object.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebRooDoMSearch
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PWEB_RESOURCE_OWNER_OBJECT      pMyObject       = (PWEB_RESOURCE_OWNER_OBJECT  )hThisObject;
    PWEB_RESOURCE_OWNER_PROPERTY    pProperty       = (PWEB_RESOURCE_OWNER_PROPERTY)&pMyObject->Property;
    PHTTP_RCP_INTERFACE             pRcpIf          = (PHTTP_RCP_INTERFACE         )pMyObject->hRcpIf;
    PHTTP_BMO_REQ_OBJECT            pBmoReq         = (PHTTP_BMO_REQ_OBJECT        )hBmoReq;
    PHTTP_BMO_REP_OBJECT            pBmoRep         = (PHTTP_BMO_REP_OBJECT        )hBmoRep;
    PHTTP_REQUEST_INFO              pReqInfo        = (PHTTP_REQUEST_INFO          )pBmoReq->GetReqInfo    ((ANSC_HANDLE)pBmoReq);
    PANSC_TOKEN_CHAIN               pPathTokenChain = (PANSC_TOKEN_CHAIN           )pBmoReq->GetRecvContext((ANSC_HANDLE)pBmoReq);
    PWEB_GENERAL_SESSION_OBJECT     pSession        = (PWEB_GENERAL_SESSION_OBJECT )pBmoRep->GetRecvContext((ANSC_HANDLE)pBmoRep);
    PHTTP_HFO_ALLOW                 pHttpHfoAllow   = NULL;

    pHttpHfoAllow = (PHTTP_HFO_ALLOW)AnscAllocateMemory(sizeof(HTTP_HFO_ALLOW));

    if ( !pHttpHfoAllow )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pHttpHfoAllow->HashIndex      = 0;
        pHttpHfoAllow->Flags          = 0;
        pHttpHfoAllow->HeaderId       = HTTP_HEADER_ID_ALLOW;

        pHttpHfoAllow->MethodCount    = 4;
        pHttpHfoAllow->MethodArray[0] = HTTP_METHOD_CODE_OPTIONS;
        pHttpHfoAllow->MethodArray[1] = HTTP_METHOD_CODE_GET;
        pHttpHfoAllow->MethodArray[2] = HTTP_METHOD_CODE_HEAD;
        pHttpHfoAllow->MethodArray[3] = HTTP_METHOD_CODE_POST;
    }

    returnStatus =
        pMyObject->ReplyReq
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pBmoReq,
                (ANSC_HANDLE)pBmoRep,
                HTTP_STATUS_METHOD_NOT_ALLOWED,
                (ANSC_HANDLE)pHttpHfoAllow
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooDoMPost
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq,
                ANSC_HANDLE                 hBmoRep
            );

    description:

        This function is called to process a web request message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the HTTP request message object.

                ANSC_HANDLE                 hBmoRep
                Specifies the HTTP response message object.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebRooDoMPost
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PWEB_RESOURCE_OWNER_OBJECT      pMyObject       = (PWEB_RESOURCE_OWNER_OBJECT  )hThisObject;
    PWEB_RESOURCE_OWNER_PROPERTY    pProperty       = (PWEB_RESOURCE_OWNER_PROPERTY)&pMyObject->Property;
    PHTTP_RCP_INTERFACE             pRcpIf          = (PHTTP_RCP_INTERFACE         )pMyObject->hRcpIf;
    PHTTP_BMO_REQ_OBJECT            pBmoReq         = (PHTTP_BMO_REQ_OBJECT        )hBmoReq;
    PHTTP_BMO_REP_OBJECT            pBmoRep         = (PHTTP_BMO_REP_OBJECT        )hBmoRep;
    PHTTP_REQUEST_INFO              pReqInfo        = (PHTTP_REQUEST_INFO          )pBmoReq->GetReqInfo    ((ANSC_HANDLE)pBmoReq);
    PANSC_TOKEN_CHAIN               pPathTokenChain = (PANSC_TOKEN_CHAIN           )pBmoReq->GetRecvContext((ANSC_HANDLE)pBmoReq);
    PWEB_GENERAL_SESSION_OBJECT     pSession        = (PWEB_GENERAL_SESSION_OBJECT )pBmoRep->GetRecvContext((ANSC_HANDLE)pBmoRep);
    PHTTP_HFO_ALLOW                 pHttpHfoAllow   = NULL;

    pHttpHfoAllow = (PHTTP_HFO_ALLOW)AnscAllocateMemory(sizeof(HTTP_HFO_ALLOW));

    if ( !pHttpHfoAllow )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pHttpHfoAllow->HashIndex      = 0;
        pHttpHfoAllow->Flags          = 0;
        pHttpHfoAllow->HeaderId       = HTTP_HEADER_ID_ALLOW;

        pHttpHfoAllow->MethodCount    = 4;
        pHttpHfoAllow->MethodArray[0] = HTTP_METHOD_CODE_OPTIONS;
        pHttpHfoAllow->MethodArray[1] = HTTP_METHOD_CODE_GET;
        pHttpHfoAllow->MethodArray[2] = HTTP_METHOD_CODE_HEAD;
        pHttpHfoAllow->MethodArray[3] = HTTP_METHOD_CODE_POST;
    }

    returnStatus =
        pMyObject->ReplyReq
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pBmoReq,
                (ANSC_HANDLE)pBmoRep,
                HTTP_STATUS_METHOD_NOT_ALLOWED,
                (ANSC_HANDLE)pHttpHfoAllow
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooDoSubscribe
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq,
                ANSC_HANDLE                 hBmoRep
            );

    description:

        This function is called to process a web request message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the HTTP request message object.

                ANSC_HANDLE                 hBmoRep
                Specifies the HTTP response message object.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebRooDoSubscribe
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PWEB_RESOURCE_OWNER_OBJECT      pMyObject       = (PWEB_RESOURCE_OWNER_OBJECT  )hThisObject;
    PWEB_RESOURCE_OWNER_PROPERTY    pProperty       = (PWEB_RESOURCE_OWNER_PROPERTY)&pMyObject->Property;
    PHTTP_RCP_INTERFACE             pRcpIf          = (PHTTP_RCP_INTERFACE         )pMyObject->hRcpIf;
    PHTTP_BMO_REQ_OBJECT            pBmoReq         = (PHTTP_BMO_REQ_OBJECT        )hBmoReq;
    PHTTP_BMO_REP_OBJECT            pBmoRep         = (PHTTP_BMO_REP_OBJECT        )hBmoRep;
    PHTTP_REQUEST_INFO              pReqInfo        = (PHTTP_REQUEST_INFO          )pBmoReq->GetReqInfo    ((ANSC_HANDLE)pBmoReq);
    PANSC_TOKEN_CHAIN               pPathTokenChain = (PANSC_TOKEN_CHAIN           )pBmoReq->GetRecvContext((ANSC_HANDLE)pBmoReq);
    PWEB_GENERAL_SESSION_OBJECT     pSession        = (PWEB_GENERAL_SESSION_OBJECT )pBmoRep->GetRecvContext((ANSC_HANDLE)pBmoRep);
    PHTTP_HFO_ALLOW                 pHttpHfoAllow   = NULL;

    pHttpHfoAllow = (PHTTP_HFO_ALLOW)AnscAllocateMemory(sizeof(HTTP_HFO_ALLOW));

    if ( !pHttpHfoAllow )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pHttpHfoAllow->HashIndex      = 0;
        pHttpHfoAllow->Flags          = 0;
        pHttpHfoAllow->HeaderId       = HTTP_HEADER_ID_ALLOW;

        pHttpHfoAllow->MethodCount    = 4;
        pHttpHfoAllow->MethodArray[0] = HTTP_METHOD_CODE_OPTIONS;
        pHttpHfoAllow->MethodArray[1] = HTTP_METHOD_CODE_GET;
        pHttpHfoAllow->MethodArray[2] = HTTP_METHOD_CODE_HEAD;
        pHttpHfoAllow->MethodArray[3] = HTTP_METHOD_CODE_POST;
    }

    returnStatus =
        pMyObject->ReplyReq
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pBmoReq,
                (ANSC_HANDLE)pBmoRep,
                HTTP_STATUS_METHOD_NOT_ALLOWED,
                (ANSC_HANDLE)pHttpHfoAllow
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooDoUnsubscribe
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq,
                ANSC_HANDLE                 hBmoRep
            );

    description:

        This function is called to process a web request message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the HTTP request message object.

                ANSC_HANDLE                 hBmoRep
                Specifies the HTTP response message object.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebRooDoUnsubscribe
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PWEB_RESOURCE_OWNER_OBJECT      pMyObject       = (PWEB_RESOURCE_OWNER_OBJECT  )hThisObject;
    PWEB_RESOURCE_OWNER_PROPERTY    pProperty       = (PWEB_RESOURCE_OWNER_PROPERTY)&pMyObject->Property;
    PHTTP_RCP_INTERFACE             pRcpIf          = (PHTTP_RCP_INTERFACE         )pMyObject->hRcpIf;
    PHTTP_BMO_REQ_OBJECT            pBmoReq         = (PHTTP_BMO_REQ_OBJECT        )hBmoReq;
    PHTTP_BMO_REP_OBJECT            pBmoRep         = (PHTTP_BMO_REP_OBJECT        )hBmoRep;
    PHTTP_REQUEST_INFO              pReqInfo        = (PHTTP_REQUEST_INFO          )pBmoReq->GetReqInfo    ((ANSC_HANDLE)pBmoReq);
    PANSC_TOKEN_CHAIN               pPathTokenChain = (PANSC_TOKEN_CHAIN           )pBmoReq->GetRecvContext((ANSC_HANDLE)pBmoReq);
    PWEB_GENERAL_SESSION_OBJECT     pSession        = (PWEB_GENERAL_SESSION_OBJECT )pBmoRep->GetRecvContext((ANSC_HANDLE)pBmoRep);
    PHTTP_HFO_ALLOW                 pHttpHfoAllow   = NULL;

    pHttpHfoAllow = (PHTTP_HFO_ALLOW)AnscAllocateMemory(sizeof(HTTP_HFO_ALLOW));

    if ( !pHttpHfoAllow )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pHttpHfoAllow->HashIndex      = 0;
        pHttpHfoAllow->Flags          = 0;
        pHttpHfoAllow->HeaderId       = HTTP_HEADER_ID_ALLOW;

        pHttpHfoAllow->MethodCount    = 4;
        pHttpHfoAllow->MethodArray[0] = HTTP_METHOD_CODE_OPTIONS;
        pHttpHfoAllow->MethodArray[1] = HTTP_METHOD_CODE_GET;
        pHttpHfoAllow->MethodArray[2] = HTTP_METHOD_CODE_HEAD;
        pHttpHfoAllow->MethodArray[3] = HTTP_METHOD_CODE_POST;
    }

    returnStatus =
        pMyObject->ReplyReq
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pBmoReq,
                (ANSC_HANDLE)pBmoRep,
                HTTP_STATUS_METHOD_NOT_ALLOWED,
                (ANSC_HANDLE)pHttpHfoAllow
            );

    return  returnStatus;
}
