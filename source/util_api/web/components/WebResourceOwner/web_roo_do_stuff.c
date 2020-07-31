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

    module:	web_roo_do_stuff.c

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the Web Resource Owner Object.

        *   WebRooDoOptions
        *   WebRooDoGet
        *   WebRooDoHead
        *   WebRooDoPost
        *   WebRooDoPut
        *   WebRooDoDelete
        *   WebRooDoTrace
        *   WebRooDoConnect

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/18/02    initial revision.

**********************************************************************/


#include "web_roo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooDoOptions
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
WebRooDoOptions
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PWEB_RESOURCE_OWNER_OBJECT      pMyObject       = (PWEB_RESOURCE_OWNER_OBJECT  )hThisObject;
    PHTTP_BMO_REQ_OBJECT            pBmoReq         = (PHTTP_BMO_REQ_OBJECT        )hBmoReq;
    PHTTP_BMO_REP_OBJECT            pBmoRep         = (PHTTP_BMO_REP_OBJECT        )hBmoRep;

    returnStatus =
        pMyObject->ReplyReq
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pBmoReq,
                (ANSC_HANDLE)pBmoRep,
                HTTP_STATUS_OK,
                (ANSC_HANDLE)NULL
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooDoGet
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
WebRooDoGet
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PWEB_RESOURCE_OWNER_OBJECT      pMyObject       = (PWEB_RESOURCE_OWNER_OBJECT  )hThisObject;
    PHTTP_BMO_REQ_OBJECT            pBmoReq         = (PHTTP_BMO_REQ_OBJECT        )hBmoReq;
    PHTTP_BMO_REP_OBJECT            pBmoRep         = (PHTTP_BMO_REP_OBJECT        )hBmoRep;

    returnStatus =
        pMyObject->ReplyReq
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pBmoReq,
                (ANSC_HANDLE)pBmoRep,
                HTTP_STATUS_OK,
                (ANSC_HANDLE)NULL
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooDoHead
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
WebRooDoHead
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PWEB_RESOURCE_OWNER_OBJECT      pMyObject       = (PWEB_RESOURCE_OWNER_OBJECT  )hThisObject;
    PHTTP_BMO_REQ_OBJECT            pBmoReq         = (PHTTP_BMO_REQ_OBJECT        )hBmoReq;
    PHTTP_BMO_REP_OBJECT            pBmoRep         = (PHTTP_BMO_REP_OBJECT        )hBmoRep;

    returnStatus =
        pMyObject->ReplyReq
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pBmoReq,
                (ANSC_HANDLE)pBmoRep,
                HTTP_STATUS_OK,
                (ANSC_HANDLE)NULL
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooDoPost
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
WebRooDoPost
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PWEB_RESOURCE_OWNER_OBJECT      pMyObject       = (PWEB_RESOURCE_OWNER_OBJECT  )hThisObject;
    PHTTP_BMO_REQ_OBJECT            pBmoReq         = (PHTTP_BMO_REQ_OBJECT        )hBmoReq;
    PHTTP_BMO_REP_OBJECT            pBmoRep         = (PHTTP_BMO_REP_OBJECT        )hBmoRep;

    returnStatus =
        pMyObject->ReplyReq
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pBmoReq,
                (ANSC_HANDLE)pBmoRep,
                HTTP_STATUS_OK,
                (ANSC_HANDLE)NULL
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooDoPut
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
WebRooDoPut
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PWEB_RESOURCE_OWNER_OBJECT      pMyObject       = (PWEB_RESOURCE_OWNER_OBJECT  )hThisObject;
    PHTTP_BMO_REQ_OBJECT            pBmoReq         = (PHTTP_BMO_REQ_OBJECT        )hBmoReq;
    PHTTP_BMO_REP_OBJECT            pBmoRep         = (PHTTP_BMO_REP_OBJECT        )hBmoRep;
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
        WebRooDoDelete
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
WebRooDoDelete
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PWEB_RESOURCE_OWNER_OBJECT      pMyObject       = (PWEB_RESOURCE_OWNER_OBJECT  )hThisObject;
    PHTTP_BMO_REQ_OBJECT            pBmoReq         = (PHTTP_BMO_REQ_OBJECT        )hBmoReq;
    PHTTP_BMO_REP_OBJECT            pBmoRep         = (PHTTP_BMO_REP_OBJECT        )hBmoRep;
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
        WebRooDoTrace
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
WebRooDoTrace
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PWEB_RESOURCE_OWNER_OBJECT      pMyObject       = (PWEB_RESOURCE_OWNER_OBJECT  )hThisObject;
    PHTTP_BMO_REQ_OBJECT            pBmoReq         = (PHTTP_BMO_REQ_OBJECT        )hBmoReq;
    PHTTP_BMO_REP_OBJECT            pBmoRep         = (PHTTP_BMO_REP_OBJECT        )hBmoRep;
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
        WebRooDoConnect
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
WebRooDoConnect
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PWEB_RESOURCE_OWNER_OBJECT      pMyObject       = (PWEB_RESOURCE_OWNER_OBJECT  )hThisObject;
    PHTTP_BMO_REQ_OBJECT            pBmoReq         = (PHTTP_BMO_REQ_OBJECT        )hBmoReq;
    PHTTP_BMO_REP_OBJECT            pBmoRep         = (PHTTP_BMO_REP_OBJECT        )hBmoRep;
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
