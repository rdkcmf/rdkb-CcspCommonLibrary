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

    module:	web_roo_reply.c

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the Web Resource Owner Object.

        *   WebRooReplyReq
        *   WebRooReply300
        *   WebRooReply301
        *   WebRooReply302
        *   WebRooReply303
        *   WebRooReply304
        *   WebRooReply305
        *   WebRooReply307
        *   WebRooReply400
        *   WebRooReply403
        *   WebRooReply404
        *   WebRooReply405
        *   WebRooReply409
        *   WebRooReply410
        *   WebRooReply415
        *   WebRooReply417
        *   WebRooReply500
        *   WebRooReply501
        *   WebRooReply503
        *   WebRooReply505

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
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooReplyReq
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq,
                ANSC_HANDLE                 hBmoRep,
                ULONG                       ulStatusCode,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called to construct a reply message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the HTTP request message object.

                ANSC_HANDLE                 hBmoRep
                Specifies the HTTP response message object.

                ULONG                       ulStatusCode
                Specifies the server status code to be returned.

                ANSC_HANDLE                 hReserved
                Specifies the reply context specific for each status.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebRooReplyReq
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ULONG                       ulStatusCode,
        ANSC_HANDLE                 hReserved
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PWEB_RESOURCE_OWNER_OBJECT      pMyObject       = (PWEB_RESOURCE_OWNER_OBJECT   )hThisObject;
    PHTTP_HELPER_CONTAINER_OBJECT   pHttpHco        = (PHTTP_HELPER_CONTAINER_OBJECT)pMyObject->hContainerContext;
    PHTTP_BMO_REQ_OBJECT            pBmoReq         = (PHTTP_BMO_REQ_OBJECT         )hBmoReq;
    PHTTP_BMO_REP_OBJECT            pBmoRep         = (PHTTP_BMO_REP_OBJECT         )hBmoRep;
    PHTTP_RESPONSE_INFO             pRepInfo        = NULL;
    char*                           pRepPhrase      = pHttpHco->GetReasonPhrase((ANSC_HANDLE)pHttpHco, ulStatusCode);
    errno_t                         rc              = -1;

    if ( !pRepPhrase )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        pRepInfo = (PHTTP_RESPONSE_INFO)AnscAllocateMemory(sizeof(HTTP_RESPONSE_INFO));

        if ( !pRepInfo )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pRepInfo->MajorVersion = WEB_SMO_MAJOR_VERSION;
            pRepInfo->MinorVersion = WEB_SMO_MINOR_VERSION;
            pRepInfo->StatusCode   = ulStatusCode;

            rc = strcpy_s(pRepInfo->ReasonPhrase, sizeof(pRepInfo->ReasonPhrase), pRepPhrase);
            ERR_CHK(rc);
        }
    }

    returnStatus =
        pBmoRep->SetRepInfo
            (
                (ANSC_HANDLE)pBmoRep,
                (ANSC_HANDLE)pRepInfo
            );

    switch ( ulStatusCode )
    {
        case    HTTP_STATUS_MULTIPLE_CHOICES :

                returnStatus =
                    pMyObject->Reply300
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pBmoReq,
                            (ANSC_HANDLE)pBmoRep,
                            hReserved
                        );

                break;

        case    HTTP_STATUS_MOVED_PERMANENTLY :

                returnStatus =
                    pMyObject->Reply301
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pBmoReq,
                            (ANSC_HANDLE)pBmoRep,
                            hReserved
                        );

                break;

        case    HTTP_STATUS_FOUND :

                returnStatus =
                    pMyObject->Reply302
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pBmoReq,
                            (ANSC_HANDLE)pBmoRep,
                            hReserved
                        );

                break;

        case    HTTP_STATUS_SEE_OTHER :

                returnStatus =
                    pMyObject->Reply303
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pBmoReq,
                            (ANSC_HANDLE)pBmoRep,
                            hReserved
                        );

                break;

        case    HTTP_STATUS_NOT_MODIFIED :

                returnStatus =
                    pMyObject->Reply304
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pBmoReq,
                            (ANSC_HANDLE)pBmoRep,
                            hReserved
                        );

                break;

        case    HTTP_STATUS_USE_PROXY :

                returnStatus =
                    pMyObject->Reply305
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pBmoReq,
                            (ANSC_HANDLE)pBmoRep,
                            hReserved
                        );

                break;

        case    HTTP_STATUS_TEMP_REDIRECT :

                returnStatus =
                    pMyObject->Reply307
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pBmoReq,
                            (ANSC_HANDLE)pBmoRep,
                            hReserved
                        );

                break;

        case    HTTP_STATUS_BAD_REQUEST :

                returnStatus =
                    pMyObject->Reply400
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pBmoReq,
                            (ANSC_HANDLE)pBmoRep,
                            hReserved
                        );

                break;

        case    HTTP_STATUS_FORBIDDEN :

                returnStatus =
                    pMyObject->Reply403
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pBmoReq,
                            (ANSC_HANDLE)pBmoRep,
                            hReserved
                        );

                break;

        case    HTTP_STATUS_NOT_FOUND :

                returnStatus =
                    pMyObject->Reply404
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pBmoReq,
                            (ANSC_HANDLE)pBmoRep,
                            hReserved
                        );

                break;

        case    HTTP_STATUS_METHOD_NOT_ALLOWED :

                returnStatus =
                    pMyObject->Reply405
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pBmoReq,
                            (ANSC_HANDLE)pBmoRep,
                            hReserved
                        );

                break;

        case    HTTP_STATUS_CONFLICT :

                returnStatus =
                    pMyObject->Reply409
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pBmoReq,
                            (ANSC_HANDLE)pBmoRep,
                            hReserved
                        );

                break;

        case    HTTP_STATUS_GONE :

                returnStatus =
                    pMyObject->Reply410
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pBmoReq,
                            (ANSC_HANDLE)pBmoRep,
                            hReserved
                        );

                break;

        case    HTTP_STATUS_UNSUPPORTED_MEDIA :

                returnStatus =
                    pMyObject->Reply415
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pBmoReq,
                            (ANSC_HANDLE)pBmoRep,
                            hReserved
                        );

                break;

        case    HTTP_STATUS_EXPECTATION_FAILED :

                returnStatus =
                    pMyObject->Reply417
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pBmoReq,
                            (ANSC_HANDLE)pBmoRep,
                            hReserved
                        );

                break;

        case    HTTP_STATUS_INTERNAL_SERVER_ERROR :

                returnStatus =
                    pMyObject->Reply500
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pBmoReq,
                            (ANSC_HANDLE)pBmoRep,
                            hReserved
                        );

                break;

        case    HTTP_STATUS_NOT_IMPLEMENTED :

                returnStatus =
                    pMyObject->Reply501
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pBmoReq,
                            (ANSC_HANDLE)pBmoRep,
                            hReserved
                        );

                break;

        case    HTTP_STATUS_SERVICE_UNAVAILABLE :

                returnStatus =
                    pMyObject->Reply503
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pBmoReq,
                            (ANSC_HANDLE)pBmoRep,
                            hReserved
                        );

                break;

        case    HTTP_STATUS_VERSION_NOT_SUPPORTED :

                returnStatus =
                    pMyObject->Reply505
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pBmoReq,
                            (ANSC_HANDLE)pBmoRep,
                            hReserved
                        );

                break;

        default :

                break;
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooReply300
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq,
                ANSC_HANDLE                 hBmoRep,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called to construct a reply message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the HTTP request message object.

                ANSC_HANDLE                 hBmoRep
                Specifies the HTTP response message object.

                ANSC_HANDLE                 hReserved
                Specifies the reply context specific for each status.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebRooReply300
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hReserved
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hBmoReq);
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PHTTP_BMO_REP_OBJECT            pBmoRep          = (PHTTP_BMO_REP_OBJECT         )hBmoRep;
    PHTTP_HFO_LOCATION              pHttpHfoLocation = (PHTTP_HFO_LOCATION           )hReserved;

    /*
     * The "hReserved" parameter is a pointer to the http_hfo_location data structure, which
     * indicates the server's preferred URI for automatic redirection to the resource.
     */
    if ( pHttpHfoLocation )
    {
        returnStatus =
            pBmoRep->AddHeaderField
                (
                    (ANSC_HANDLE)pBmoRep,
                    (ANSC_HANDLE)pHttpHfoLocation
                );
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooReply301
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq,
                ANSC_HANDLE                 hBmoRep,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called to construct a reply message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the HTTP request message object.

                ANSC_HANDLE                 hBmoRep
                Specifies the HTTP response message object.

                ANSC_HANDLE                 hReserved
                Specifies the reply context specific for each status.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebRooReply301
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hReserved
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hBmoReq);
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PHTTP_BMO_REP_OBJECT            pBmoRep          = (PHTTP_BMO_REP_OBJECT         )hBmoRep;
    PHTTP_HFO_LOCATION              pHttpHfoLocation = (PHTTP_HFO_LOCATION           )hReserved;

    /*
     * The "hReserved" parameter is a pointer to the http_hfo_location data structure, which
     * indicates the server's preferred URI for automatic redirection to the resource.
     */
    if ( pHttpHfoLocation )
    {
        returnStatus =
            pBmoRep->AddHeaderField
                (
                    (ANSC_HANDLE)pBmoRep,
                    (ANSC_HANDLE)pHttpHfoLocation
                );
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooReply302
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq,
                ANSC_HANDLE                 hBmoRep,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called to construct a reply message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the HTTP request message object.

                ANSC_HANDLE                 hBmoRep
                Specifies the HTTP response message object.

                ANSC_HANDLE                 hReserved
                Specifies the reply context specific for each status.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebRooReply302
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hReserved
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hBmoReq);
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PHTTP_BMO_REP_OBJECT            pBmoRep          = (PHTTP_BMO_REP_OBJECT         )hBmoRep;
    PHTTP_HFO_LOCATION              pHttpHfoLocation = (PHTTP_HFO_LOCATION           )hReserved;

    /*
     * The "hReserved" parameter is a pointer to the http_hfo_location data structure, which
     * indicates the server's preferred URI for automatic redirection to the resource.
     */
    if ( pHttpHfoLocation )
    {
        returnStatus =
            pBmoRep->AddHeaderField
                (
                    (ANSC_HANDLE)pBmoRep,
                    (ANSC_HANDLE)pHttpHfoLocation
                );
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooReply303
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq,
                ANSC_HANDLE                 hBmoRep,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called to construct a reply message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the HTTP request message object.

                ANSC_HANDLE                 hBmoRep
                Specifies the HTTP response message object.

                ANSC_HANDLE                 hReserved
                Specifies the reply context specific for each status.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebRooReply303
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hReserved
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hBmoReq);
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PHTTP_BMO_REP_OBJECT            pBmoRep          = (PHTTP_BMO_REP_OBJECT         )hBmoRep;
    PHTTP_HFO_LOCATION              pHttpHfoLocation = (PHTTP_HFO_LOCATION           )hReserved;

    /*
     * The "hReserved" parameter is a pointer to the http_hfo_location data structure, which
     * indicates the server's preferred URI for automatic redirection to the resource.
     */
    if ( pHttpHfoLocation )
    {
        returnStatus =
            pBmoRep->AddHeaderField
                (
                    (ANSC_HANDLE)pBmoRep,
                    (ANSC_HANDLE)pHttpHfoLocation
                );
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooReply304
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq,
                ANSC_HANDLE                 hBmoRep,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called to construct a reply message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the HTTP request message object.

                ANSC_HANDLE                 hBmoRep
                Specifies the HTTP response message object.

                ANSC_HANDLE                 hReserved
                Specifies the reply context specific for each status.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebRooReply304
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hReserved
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hBmoReq);
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PHTTP_BMO_REP_OBJECT            pBmoRep          = (PHTTP_BMO_REP_OBJECT         )hBmoRep;
    PHTTP_HFO_LOCATION              pHttpHfoLocation = (PHTTP_HFO_LOCATION           )hReserved;

    /*
     * The "hReserved" parameter is a pointer to the http_hfo_location data structure, which
     * indicates the server's preferred URI for automatic redirection to the resource.
     */
    if ( pHttpHfoLocation )
    {
        returnStatus =
            pBmoRep->AddHeaderField
                (
                    (ANSC_HANDLE)pBmoRep,
                    (ANSC_HANDLE)pHttpHfoLocation
                );
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooReply305
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq,
                ANSC_HANDLE                 hBmoRep,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called to construct a reply message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the HTTP request message object.

                ANSC_HANDLE                 hBmoRep
                Specifies the HTTP response message object.

                ANSC_HANDLE                 hReserved
                Specifies the reply context specific for each status.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebRooReply305
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hReserved
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hBmoReq);
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PHTTP_BMO_REP_OBJECT            pBmoRep          = (PHTTP_BMO_REP_OBJECT         )hBmoRep;
    PHTTP_HFO_LOCATION              pHttpHfoLocation = (PHTTP_HFO_LOCATION           )hReserved;

    /*
     * The "hReserved" parameter is a pointer to the http_hfo_location data structure, which
     * indicates the server's preferred URI for automatic redirection to the resource.
     */
    if ( pHttpHfoLocation )
    {
        returnStatus =
            pBmoRep->AddHeaderField
                (
                    (ANSC_HANDLE)pBmoRep,
                    (ANSC_HANDLE)pHttpHfoLocation
                );
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooReply307
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq,
                ANSC_HANDLE                 hBmoRep,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called to construct a reply message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the HTTP request message object.

                ANSC_HANDLE                 hBmoRep
                Specifies the HTTP response message object.

                ANSC_HANDLE                 hReserved
                Specifies the reply context specific for each status.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebRooReply307
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hReserved
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hBmoReq);
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PHTTP_BMO_REP_OBJECT            pBmoRep          = (PHTTP_BMO_REP_OBJECT         )hBmoRep;
    PHTTP_HFO_LOCATION              pHttpHfoLocation = (PHTTP_HFO_LOCATION           )hReserved;

    /*
     * The "hReserved" parameter is a pointer to the http_hfo_location data structure, which
     * indicates the server's preferred URI for automatic redirection to the resource.
     */
    if ( pHttpHfoLocation )
    {
        returnStatus =
            pBmoRep->AddHeaderField
                (
                    (ANSC_HANDLE)pBmoRep,
                    (ANSC_HANDLE)pHttpHfoLocation
                );
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooReply400
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq,
                ANSC_HANDLE                 hBmoRep,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called to construct a reply message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the HTTP request message object.

                ANSC_HANDLE                 hBmoRep
                Specifies the HTTP response message object.

                ANSC_HANDLE                 hReserved
                Specifies the reply context specific for each status.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebRooReply400
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hReserved
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hBmoReq);
    UNREFERENCED_PARAMETER(hBmoRep);
    UNREFERENCED_PARAMETER(hReserved);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooReply403
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq,
                ANSC_HANDLE                 hBmoRep,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called to construct a reply message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the HTTP request message object.

                ANSC_HANDLE                 hBmoRep
                Specifies the HTTP response message object.

                ANSC_HANDLE                 hReserved
                Specifies the reply context specific for each status.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebRooReply403
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hReserved
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hBmoReq);
    UNREFERENCED_PARAMETER(hBmoRep);
    UNREFERENCED_PARAMETER(hReserved);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooReply404
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq,
                ANSC_HANDLE                 hBmoRep,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called to construct a reply message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the HTTP request message object.

                ANSC_HANDLE                 hBmoRep
                Specifies the HTTP response message object.

                ANSC_HANDLE                 hReserved
                Specifies the reply context specific for each status.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebRooReply404
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hReserved
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hBmoReq);
    UNREFERENCED_PARAMETER(hBmoRep);
    UNREFERENCED_PARAMETER(hReserved);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooReply405
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq,
                ANSC_HANDLE                 hBmoRep,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called to construct a reply message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the HTTP request message object.

                ANSC_HANDLE                 hBmoRep
                Specifies the HTTP response message object.

                ANSC_HANDLE                 hReserved
                Specifies the reply context specific for each status.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebRooReply405
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hReserved
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hBmoReq);
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PHTTP_BMO_REP_OBJECT            pBmoRep         = (PHTTP_BMO_REP_OBJECT         )hBmoRep;
    PHTTP_HFO_ALLOW                 pHttpHfoAllow   = (PHTTP_HFO_ALLOW              )hReserved;

    /*
     * The "hReserved" parameter is a pointer to the http_hfo_allow data structure, which contains
     * all the applicable methods to the requested URI.
     */
    if ( pHttpHfoAllow )
    {
        returnStatus =
            pBmoRep->AddHeaderField
                (
                    (ANSC_HANDLE)pBmoRep,
                    (ANSC_HANDLE)pHttpHfoAllow
                );
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooReply409
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq,
                ANSC_HANDLE                 hBmoRep,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called to construct a reply message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the HTTP request message object.

                ANSC_HANDLE                 hBmoRep
                Specifies the HTTP response message object.

                ANSC_HANDLE                 hReserved
                Specifies the reply context specific for each status.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebRooReply409
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hReserved
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hBmoReq);
    UNREFERENCED_PARAMETER(hBmoRep);
    UNREFERENCED_PARAMETER(hReserved);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooReply410
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq,
                ANSC_HANDLE                 hBmoRep,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called to construct a reply message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the HTTP request message object.

                ANSC_HANDLE                 hBmoRep
                Specifies the HTTP response message object.

                ANSC_HANDLE                 hReserved
                Specifies the reply context specific for each status.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebRooReply410
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hReserved
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hBmoReq);
    UNREFERENCED_PARAMETER(hBmoRep);
    UNREFERENCED_PARAMETER(hReserved);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooReply415
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq,
                ANSC_HANDLE                 hBmoRep,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called to construct a reply message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the HTTP request message object.

                ANSC_HANDLE                 hBmoRep
                Specifies the HTTP response message object.

                ANSC_HANDLE                 hReserved
                Specifies the reply context specific for each status.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebRooReply415
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hReserved
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hBmoReq);
    UNREFERENCED_PARAMETER(hBmoRep);
    UNREFERENCED_PARAMETER(hReserved);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooReply417
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq,
                ANSC_HANDLE                 hBmoRep,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called to construct a reply message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the HTTP request message object.

                ANSC_HANDLE                 hBmoRep
                Specifies the HTTP response message object.

                ANSC_HANDLE                 hReserved
                Specifies the reply context specific for each status.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebRooReply417
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hReserved
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hBmoReq);
    UNREFERENCED_PARAMETER(hBmoRep);
    UNREFERENCED_PARAMETER(hReserved);

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooReply500
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq,
                ANSC_HANDLE                 hBmoRep,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called to construct a reply message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the HTTP request message object.

                ANSC_HANDLE                 hBmoRep
                Specifies the HTTP response message object.

                ANSC_HANDLE                 hReserved
                Specifies the reply context specific for each status.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebRooReply500
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hReserved
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hBmoReq);
    UNREFERENCED_PARAMETER(hBmoRep);
    UNREFERENCED_PARAMETER(hReserved);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooReply501
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq,
                ANSC_HANDLE                 hBmoRep,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called to construct a reply message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the HTTP request message object.

                ANSC_HANDLE                 hBmoRep
                Specifies the HTTP response message object.

                ANSC_HANDLE                 hReserved
                Specifies the reply context specific for each status.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebRooReply501
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hReserved
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hBmoReq);
    UNREFERENCED_PARAMETER(hBmoRep);
    UNREFERENCED_PARAMETER(hReserved);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooReply503
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq,
                ANSC_HANDLE                 hBmoRep,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called to construct a reply message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the HTTP request message object.

                ANSC_HANDLE                 hBmoRep
                Specifies the HTTP response message object.

                ANSC_HANDLE                 hReserved
                Specifies the reply context specific for each status.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebRooReply503
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hReserved
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hBmoReq);
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PHTTP_BMO_REP_OBJECT            pBmoRep            = (PHTTP_BMO_REP_OBJECT         )hBmoRep;
    PHTTP_HFO_RETRY_AFTER           pHttpHfoRetryAfter = (PHTTP_HFO_RETRY_AFTER        )hReserved;

    /*
     * The "hReserved" parameter is a pointer to the http_hfo_retry_after data structure, which
     * contains the number of seconds that client SHOULD wait before submiting another request.
     */
    if ( pHttpHfoRetryAfter )
    {
        returnStatus =
            pBmoRep->AddHeaderField
                (
                    (ANSC_HANDLE)pBmoRep,
                    (ANSC_HANDLE)pHttpHfoRetryAfter
                );
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooReply505
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq,
                ANSC_HANDLE                 hBmoRep,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called to construct a reply message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the HTTP request message object.

                ANSC_HANDLE                 hBmoRep
                Specifies the HTTP response message object.

                ANSC_HANDLE                 hReserved
                Specifies the reply context specific for each status.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebRooReply505
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hReserved
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hBmoReq);
    UNREFERENCED_PARAMETER(hBmoRep);
    UNREFERENCED_PARAMETER(hReserved);

    return  ANSC_STATUS_SUCCESS;
}
