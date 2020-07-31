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

    module:	bwsp_wso_do_stuff.c

        For Broadway Web Service Plane Implementation (BWSP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the Bwsp Web Servlet Object.

        *   BwspWsoDoOptions
        *   BwspWsoDoGet
        *   BwspWsoDoHead
        *   BwspWsoDoPost
        *   BwspWsoDoPut
        *   BwspWsoDoDelete
        *   BwspWsoDoTrace
        *   BwspWsoDoConnect

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/02/03    initial revision.

**********************************************************************/


#include "bwsp_wso_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwspWsoDoOptions
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
BwspWsoDoOptions
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PBWSP_WEB_SERVLET_OBJECT        pMyObject       = (PBWSP_WEB_SERVLET_OBJECT   )hThisObject;
    PBWSP_WSH_INTERFACE             pBwspWshIf      = (PBWSP_WSH_INTERFACE        )pMyObject->hBwspWshIf;
    PHTTP_BMO_REQ_OBJECT            pBmoReq         = (PHTTP_BMO_REQ_OBJECT       )hBmoReq;
    PHTTP_BMO_REP_OBJECT            pBmoRep         = (PHTTP_BMO_REP_OBJECT       )hBmoRep;

    if ( pBwspWshIf->DoOptions )
    {
        returnStatus =
            pBwspWshIf->DoOptions
                (
                    pBwspWshIf->hOwnerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pBmoReq,
                    (ANSC_HANDLE)pBmoRep
                );
    }
    else
    {
        returnStatus =
            pMyObject->ReplyReq
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pBmoReq,
                    (ANSC_HANDLE)pBmoRep,
                    HTTP_STATUS_OK,
                    (ANSC_HANDLE)NULL
                );
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwspWsoDoGet
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
BwspWsoDoGet
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PBWSP_WEB_SERVLET_OBJECT        pMyObject       = (PBWSP_WEB_SERVLET_OBJECT   )hThisObject;
    PBWSP_WSH_INTERFACE             pBwspWshIf      = (PBWSP_WSH_INTERFACE        )pMyObject->hBwspWshIf;
    PHTTP_BMO_REQ_OBJECT            pBmoReq         = (PHTTP_BMO_REQ_OBJECT       )hBmoReq;
    PHTTP_BMO_REP_OBJECT            pBmoRep         = (PHTTP_BMO_REP_OBJECT       )hBmoRep;

    if ( pBwspWshIf->DoGet )
    {
        returnStatus =
            pBwspWshIf->DoGet
                (
                    pBwspWshIf->hOwnerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pBmoReq,
                    (ANSC_HANDLE)pBmoRep
                );
    }
    else
    {
        returnStatus =
            pMyObject->ReplyReq
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pBmoReq,
                    (ANSC_HANDLE)pBmoRep,
                    HTTP_STATUS_OK,
                    (ANSC_HANDLE)NULL
                );
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwspWsoDoHead
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
BwspWsoDoHead
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PBWSP_WEB_SERVLET_OBJECT        pMyObject       = (PBWSP_WEB_SERVLET_OBJECT   )hThisObject;
    PBWSP_WSH_INTERFACE             pBwspWshIf      = (PBWSP_WSH_INTERFACE        )pMyObject->hBwspWshIf;
    PHTTP_BMO_REQ_OBJECT            pBmoReq         = (PHTTP_BMO_REQ_OBJECT       )hBmoReq;
    PHTTP_BMO_REP_OBJECT            pBmoRep         = (PHTTP_BMO_REP_OBJECT       )hBmoRep;

    if ( pBwspWshIf->DoHead )
    {
        returnStatus =
            pBwspWshIf->DoHead
                (
                    pBwspWshIf->hOwnerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pBmoReq,
                    (ANSC_HANDLE)pBmoRep
                );
    }
    else
    {
        returnStatus =
            pMyObject->ReplyReq
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pBmoReq,
                    (ANSC_HANDLE)pBmoRep,
                    HTTP_STATUS_OK,
                    (ANSC_HANDLE)NULL
                );
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwspWsoDoPost
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
BwspWsoDoPost
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PBWSP_WEB_SERVLET_OBJECT        pMyObject       = (PBWSP_WEB_SERVLET_OBJECT   )hThisObject;
    PBWSP_WSH_INTERFACE             pBwspWshIf      = (PBWSP_WSH_INTERFACE        )pMyObject->hBwspWshIf;
    PHTTP_BMO_REQ_OBJECT            pBmoReq         = (PHTTP_BMO_REQ_OBJECT       )hBmoReq;
    PHTTP_BMO_REP_OBJECT            pBmoRep         = (PHTTP_BMO_REP_OBJECT       )hBmoRep;
    PHTTP_HFO_ALLOW                 pHttpHfoAllow   = NULL;
    ULONG                           j               = 0;

    if ( pBwspWshIf->DoPost )
    {
        returnStatus =
            pBwspWshIf->DoPost
                (
                    pBwspWshIf->hOwnerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pBmoReq,
                    (ANSC_HANDLE)pBmoRep
                );
    }
    else
    {
        pHttpHfoAllow = (PHTTP_HFO_ALLOW)AnscAllocateMemory(sizeof(HTTP_HFO_ALLOW));

        if ( !pHttpHfoAllow )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pHttpHfoAllow->HashIndex        = 0;
            pHttpHfoAllow->Flags            = 0;
            pHttpHfoAllow->HeaderId         = HTTP_HEADER_ID_ALLOW;

            j                               = 0;
            pHttpHfoAllow->MethodArray[j++] = HTTP_METHOD_CODE_OPTIONS;
            pHttpHfoAllow->MethodArray[j++] = HTTP_METHOD_CODE_GET;
            pHttpHfoAllow->MethodArray[j++] = HTTP_METHOD_CODE_HEAD;

            if ( pBwspWshIf->DoPost )
            {
                pHttpHfoAllow->MethodArray[j++] = HTTP_METHOD_CODE_POST;
            }

            if ( pBwspWshIf->DoPut )
            {
                pHttpHfoAllow->MethodArray[j++] = HTTP_METHOD_CODE_PUT;
            }

            if ( pBwspWshIf->DoDelete )
            {
                pHttpHfoAllow->MethodArray[j++] = HTTP_METHOD_CODE_DELETE;
            }

            if ( pBwspWshIf->DoTrace )
            {
                pHttpHfoAllow->MethodArray[j++] = HTTP_METHOD_CODE_TRACE;
            }

            if ( pBwspWshIf->DoConnect )
            {
                pHttpHfoAllow->MethodArray[j++] = HTTP_METHOD_CODE_CONNECT;
            }

            pHttpHfoAllow->MethodCount = j;
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
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwspWsoDoPut
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
BwspWsoDoPut
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PBWSP_WEB_SERVLET_OBJECT        pMyObject       = (PBWSP_WEB_SERVLET_OBJECT   )hThisObject;
    PBWSP_WSH_INTERFACE             pBwspWshIf      = (PBWSP_WSH_INTERFACE        )pMyObject->hBwspWshIf;
    PHTTP_BMO_REQ_OBJECT            pBmoReq         = (PHTTP_BMO_REQ_OBJECT       )hBmoReq;
    PHTTP_BMO_REP_OBJECT            pBmoRep         = (PHTTP_BMO_REP_OBJECT       )hBmoRep;
    PHTTP_HFO_ALLOW                 pHttpHfoAllow   = NULL;
    ULONG                           j               = 0;

    if ( pBwspWshIf->DoPut )
    {
        returnStatus =
            pBwspWshIf->DoPut
                (
                    pBwspWshIf->hOwnerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pBmoReq,
                    (ANSC_HANDLE)pBmoRep
                );
    }
    else
    {
        pHttpHfoAllow = (PHTTP_HFO_ALLOW)AnscAllocateMemory(sizeof(HTTP_HFO_ALLOW));

        if ( !pHttpHfoAllow )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pHttpHfoAllow->HashIndex        = 0;
            pHttpHfoAllow->Flags            = 0;
            pHttpHfoAllow->HeaderId         = HTTP_HEADER_ID_ALLOW;

            j                               = 0;
            pHttpHfoAllow->MethodArray[j++] = HTTP_METHOD_CODE_OPTIONS;
            pHttpHfoAllow->MethodArray[j++] = HTTP_METHOD_CODE_GET;
            pHttpHfoAllow->MethodArray[j++] = HTTP_METHOD_CODE_HEAD;

            if ( pBwspWshIf->DoPost )
            {
                pHttpHfoAllow->MethodArray[j++] = HTTP_METHOD_CODE_POST;
            }

            if ( pBwspWshIf->DoPut )
            {
                pHttpHfoAllow->MethodArray[j++] = HTTP_METHOD_CODE_PUT;
            }

            if ( pBwspWshIf->DoDelete )
            {
                pHttpHfoAllow->MethodArray[j++] = HTTP_METHOD_CODE_DELETE;
            }

            if ( pBwspWshIf->DoTrace )
            {
                pHttpHfoAllow->MethodArray[j++] = HTTP_METHOD_CODE_TRACE;
            }

            if ( pBwspWshIf->DoConnect )
            {
                pHttpHfoAllow->MethodArray[j++] = HTTP_METHOD_CODE_CONNECT;
            }

            pHttpHfoAllow->MethodCount = j;
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
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwspWsoDoDelete
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
BwspWsoDoDelete
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PBWSP_WEB_SERVLET_OBJECT        pMyObject       = (PBWSP_WEB_SERVLET_OBJECT   )hThisObject;
    PBWSP_WSH_INTERFACE             pBwspWshIf      = (PBWSP_WSH_INTERFACE        )pMyObject->hBwspWshIf;
    PHTTP_BMO_REQ_OBJECT            pBmoReq         = (PHTTP_BMO_REQ_OBJECT       )hBmoReq;
    PHTTP_BMO_REP_OBJECT            pBmoRep         = (PHTTP_BMO_REP_OBJECT       )hBmoRep;
    PHTTP_HFO_ALLOW                 pHttpHfoAllow   = NULL;
    ULONG                           j               = 0;

    if ( pBwspWshIf->DoDelete )
    {
        returnStatus =
            pBwspWshIf->DoDelete
                (
                    pBwspWshIf->hOwnerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pBmoReq,
                    (ANSC_HANDLE)pBmoRep
                );
    }
    else
    {
        pHttpHfoAllow = (PHTTP_HFO_ALLOW)AnscAllocateMemory(sizeof(HTTP_HFO_ALLOW));

        if ( !pHttpHfoAllow )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pHttpHfoAllow->HashIndex        = 0;
            pHttpHfoAllow->Flags            = 0;
            pHttpHfoAllow->HeaderId         = HTTP_HEADER_ID_ALLOW;

            j                               = 0;
            pHttpHfoAllow->MethodArray[j++] = HTTP_METHOD_CODE_OPTIONS;
            pHttpHfoAllow->MethodArray[j++] = HTTP_METHOD_CODE_GET;
            pHttpHfoAllow->MethodArray[j++] = HTTP_METHOD_CODE_HEAD;

            if ( pBwspWshIf->DoPost )
            {
                pHttpHfoAllow->MethodArray[j++] = HTTP_METHOD_CODE_POST;
            }

            if ( pBwspWshIf->DoPut )
            {
                pHttpHfoAllow->MethodArray[j++] = HTTP_METHOD_CODE_PUT;
            }

            if ( pBwspWshIf->DoDelete )
            {
                pHttpHfoAllow->MethodArray[j++] = HTTP_METHOD_CODE_DELETE;
            }

            if ( pBwspWshIf->DoTrace )
            {
                pHttpHfoAllow->MethodArray[j++] = HTTP_METHOD_CODE_TRACE;
            }

            if ( pBwspWshIf->DoConnect )
            {
                pHttpHfoAllow->MethodArray[j++] = HTTP_METHOD_CODE_CONNECT;
            }

            pHttpHfoAllow->MethodCount = j;
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
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwspWsoDoTrace
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
BwspWsoDoTrace
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PBWSP_WEB_SERVLET_OBJECT        pMyObject       = (PBWSP_WEB_SERVLET_OBJECT   )hThisObject;
    PBWSP_WSH_INTERFACE             pBwspWshIf      = (PBWSP_WSH_INTERFACE        )pMyObject->hBwspWshIf;
    PHTTP_BMO_REQ_OBJECT            pBmoReq         = (PHTTP_BMO_REQ_OBJECT       )hBmoReq;
    PHTTP_BMO_REP_OBJECT            pBmoRep         = (PHTTP_BMO_REP_OBJECT       )hBmoRep;
    PHTTP_HFO_ALLOW                 pHttpHfoAllow   = NULL;
    ULONG                           j               = 0;

    if ( pBwspWshIf->DoTrace )
    {
        returnStatus =
            pBwspWshIf->DoTrace
                (
                    pBwspWshIf->hOwnerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pBmoReq,
                    (ANSC_HANDLE)pBmoRep
                );
    }
    else
    {
        pHttpHfoAllow = (PHTTP_HFO_ALLOW)AnscAllocateMemory(sizeof(HTTP_HFO_ALLOW));

        if ( !pHttpHfoAllow )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pHttpHfoAllow->HashIndex        = 0;
            pHttpHfoAllow->Flags            = 0;
            pHttpHfoAllow->HeaderId         = HTTP_HEADER_ID_ALLOW;

            j                               = 0;
            pHttpHfoAllow->MethodArray[j++] = HTTP_METHOD_CODE_OPTIONS;
            pHttpHfoAllow->MethodArray[j++] = HTTP_METHOD_CODE_GET;
            pHttpHfoAllow->MethodArray[j++] = HTTP_METHOD_CODE_HEAD;

            if ( pBwspWshIf->DoPost )
            {
                pHttpHfoAllow->MethodArray[j++] = HTTP_METHOD_CODE_POST;
            }

            if ( pBwspWshIf->DoPut )
            {
                pHttpHfoAllow->MethodArray[j++] = HTTP_METHOD_CODE_PUT;
            }

            if ( pBwspWshIf->DoDelete )
            {
                pHttpHfoAllow->MethodArray[j++] = HTTP_METHOD_CODE_DELETE;
            }

            if ( pBwspWshIf->DoTrace )
            {
                pHttpHfoAllow->MethodArray[j++] = HTTP_METHOD_CODE_TRACE;
            }

            if ( pBwspWshIf->DoConnect )
            {
                pHttpHfoAllow->MethodArray[j++] = HTTP_METHOD_CODE_CONNECT;
            }

            pHttpHfoAllow->MethodCount = j;
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
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwspWsoDoConnect
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
BwspWsoDoConnect
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PBWSP_WEB_SERVLET_OBJECT        pMyObject       = (PBWSP_WEB_SERVLET_OBJECT   )hThisObject;
    PBWSP_WSH_INTERFACE             pBwspWshIf      = (PBWSP_WSH_INTERFACE        )pMyObject->hBwspWshIf;
    PHTTP_BMO_REQ_OBJECT            pBmoReq         = (PHTTP_BMO_REQ_OBJECT       )hBmoReq;
    PHTTP_BMO_REP_OBJECT            pBmoRep         = (PHTTP_BMO_REP_OBJECT       )hBmoRep;
    PHTTP_HFO_ALLOW                 pHttpHfoAllow   = NULL;
    ULONG                           j               = 0;

    if ( pBwspWshIf->DoConnect )
    {
        returnStatus =
            pBwspWshIf->DoConnect
                (
                    pBwspWshIf->hOwnerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pBmoReq,
                    (ANSC_HANDLE)pBmoRep
                );
    }
    else
    {
        pHttpHfoAllow = (PHTTP_HFO_ALLOW)AnscAllocateMemory(sizeof(HTTP_HFO_ALLOW));

        if ( !pHttpHfoAllow )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pHttpHfoAllow->HashIndex        = 0;
            pHttpHfoAllow->Flags            = 0;
            pHttpHfoAllow->HeaderId         = HTTP_HEADER_ID_ALLOW;

            j                               = 0;
            pHttpHfoAllow->MethodArray[j++] = HTTP_METHOD_CODE_OPTIONS;
            pHttpHfoAllow->MethodArray[j++] = HTTP_METHOD_CODE_GET;
            pHttpHfoAllow->MethodArray[j++] = HTTP_METHOD_CODE_HEAD;

            if ( pBwspWshIf->DoPost )
            {
                pHttpHfoAllow->MethodArray[j++] = HTTP_METHOD_CODE_POST;
            }

            if ( pBwspWshIf->DoPut )
            {
                pHttpHfoAllow->MethodArray[j++] = HTTP_METHOD_CODE_PUT;
            }

            if ( pBwspWshIf->DoDelete )
            {
                pHttpHfoAllow->MethodArray[j++] = HTTP_METHOD_CODE_DELETE;
            }

            if ( pBwspWshIf->DoTrace )
            {
                pHttpHfoAllow->MethodArray[j++] = HTTP_METHOD_CODE_TRACE;
            }

            if ( pBwspWshIf->DoConnect )
            {
                pHttpHfoAllow->MethodArray[j++] = HTTP_METHOD_CODE_CONNECT;
            }

            pHttpHfoAllow->MethodCount = j;
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
    }

    return  returnStatus;
}
