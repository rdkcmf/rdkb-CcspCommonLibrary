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

    module:	bwsp_wso_do_stuff_upnp.c

        For Broadway Web Service Plane Implementation (BWSP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the Bwsp Web Servlet Object.

        *   BwspWsoDoNotify
        *   BwspWsoDoSearch
        *   BwspWsoDoMSearch
        *   BwspWsoDoMPost
        *   BwspWsoDoSubscribe
        *   BwspWsoDoUnsubscribe

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


#include "bwsp_wso_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwspWsoDoNotify
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
BwspWsoDoNotify
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

    if ( pBwspWshIf->DoNotify )
    {
        returnStatus =
            pBwspWshIf->DoNotify
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
        BwspWsoDoSearch
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
BwspWsoDoSearch
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

    if ( pBwspWshIf->DoSearch )
    {
        returnStatus =
            pBwspWshIf->DoSearch
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
        BwspWsoDoMSearch
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
BwspWsoDoMSearch
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

    if ( pBwspWshIf->DoMSearch )
    {
        returnStatus =
            pBwspWshIf->DoMSearch
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
        BwspWsoDoMPost
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
BwspWsoDoMPost
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

    if ( pBwspWshIf->DoMPost )
    {
        returnStatus =
            pBwspWshIf->DoMPost
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
        BwspWsoDoSubscribe
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
BwspWsoDoSubscribe
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

    if ( pBwspWshIf->DoSubscribe )
    {
        returnStatus =
            pBwspWshIf->DoSubscribe
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
        BwspWsoDoUnsubscribe
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
BwspWsoDoUnsubscribe
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

    if ( pBwspWshIf->DoUnsubscribe )
    {
        returnStatus =
            pBwspWshIf->DoUnsubscribe
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
