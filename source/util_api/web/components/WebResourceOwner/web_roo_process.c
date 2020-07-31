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

    module:	web_roo_process.c

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the Web Resource Owner Object.

        *   WebRooQuery
        *   WebRooProcess

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/13/02    initial revision.

**********************************************************************/


#include "web_roo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        WebRooQuery
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq,
                ANSC_HANDLE                 hBmoRep
            );

    description:

        This function is called to examine a web request message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the HTTP request message object.

                ANSC_HANDLE                 hBmoRep
                Specifies the HTTP response message object.

    return:     HTTP status code.

**********************************************************************/

ULONG
WebRooQuery
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hBmoReq);
    UNREFERENCED_PARAMETER(hBmoRep);
    return  HTTP_STATUS_OK;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRooProcess
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
WebRooProcess
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
    PWEB_AUTH_SERVER_OBJECT         pAuthServer     = (PWEB_AUTH_SERVER_OBJECT     )pMyObject->hAuthServer;
    BOOL                            bAuthenticated  = TRUE;

    pBmoReq->SetRcpIf     ((ANSC_HANDLE)pBmoReq, (ANSC_HANDLE)pRcpIf   );
    pBmoRep->SetWebServlet((ANSC_HANDLE)pBmoReq, (ANSC_HANDLE)pMyObject);

    if ( pReqInfo->MajorVersion != HTTP_CURRENT_MAJOR_VERSION )
    {
        returnStatus =
            pMyObject->ReplyReq
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pBmoReq,
                    (ANSC_HANDLE)pBmoRep,
                    HTTP_STATUS_VERSION_NOT_SUPPORTED,
                    (ANSC_HANDLE)NULL
                );

        goto  EXIT1;
    }

    if ( pAuthServer )
    {
        returnStatus = 
            pAuthServer->AuthenticateReq
                (
                    (ANSC_HANDLE)pAuthServer,
                    (ANSC_HANDLE)pBmoReq,
                    (ANSC_HANDLE)pBmoRep
                );

        bAuthenticated  = ( returnStatus == ANSC_STATUS_SUCCESS );
        returnStatus = ANSC_STATUS_SUCCESS;
    }

    if ( bAuthenticated )
    {
        switch ( pBmoReq->GetMethod((ANSC_HANDLE)pBmoReq) )
        {
            case    HTTP_METHOD_CODE_OPTIONS :

                    returnStatus =
                        pMyObject->DoOptions
                            (
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)pBmoReq,
                                (ANSC_HANDLE)pBmoRep
                            );

                    break;

            case    HTTP_METHOD_CODE_GET :

                    returnStatus =
                        pMyObject->DoGet
                            (
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)pBmoReq,
                                (ANSC_HANDLE)pBmoRep
                            );

                    break;

            case    HTTP_METHOD_CODE_HEAD :

                    returnStatus =
                        pMyObject->DoHead
                            (
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)pBmoReq,
                                (ANSC_HANDLE)pBmoRep
                            );

                    break;

            case    HTTP_METHOD_CODE_POST :

                    returnStatus =
                        pMyObject->DoPost
                            (
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)pBmoReq,
                                (ANSC_HANDLE)pBmoRep
                            );

                    break;

            case    HTTP_METHOD_CODE_PUT :

                    returnStatus =
                        pMyObject->DoPut
                            (
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)pBmoReq,
                                (ANSC_HANDLE)pBmoRep
                            );

                    break;

            case    HTTP_METHOD_CODE_DELETE :

                    returnStatus =
                        pMyObject->DoDelete
                            (
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)pBmoReq,
                                (ANSC_HANDLE)pBmoRep
                            );

                    break;

            case    HTTP_METHOD_CODE_TRACE :

                    returnStatus =
                        pMyObject->DoTrace
                            (
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)pBmoReq,
                                (ANSC_HANDLE)pBmoRep
                            );

                    break;

            case    HTTP_METHOD_CODE_CONNECT :

                    returnStatus =
                        pMyObject->DoConnect
                            (
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)pBmoReq,
                                (ANSC_HANDLE)pBmoRep
                            );

                    break;

            default :

                    if ( pProperty->OwnerFlag & WEB_ROO_FLAG_UPNP_SUPPORT )
                    {
                        if ( pBmoReq->GetMethod((ANSC_HANDLE)pBmoReq) == HTTP_METHOD_CODE_NOTIFY )
                        {
                            returnStatus =
                                pMyObject->DoNotify
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        (ANSC_HANDLE)pBmoReq,
                                        (ANSC_HANDLE)pBmoRep
                                    );

                            break;
                        }
                        else if ( pBmoReq->GetMethod((ANSC_HANDLE)pBmoReq) == HTTP_METHOD_CODE_SEARCH )
                        {
                            returnStatus =
                                pMyObject->DoSearch
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        (ANSC_HANDLE)pBmoReq,
                                        (ANSC_HANDLE)pBmoRep
                                    );

                            break;
                        }
                        else if ( pBmoReq->GetMethod((ANSC_HANDLE)pBmoReq) == HTTP_METHOD_CODE_M_SEARCH )
                        {
                            returnStatus =
                                pMyObject->DoMSearch
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        (ANSC_HANDLE)pBmoReq,
                                        (ANSC_HANDLE)pBmoRep
                                    );

                            break;
                        }
                        else if ( pBmoReq->GetMethod((ANSC_HANDLE)pBmoReq) == HTTP_METHOD_CODE_M_POST )
                        {
                            returnStatus =
                                pMyObject->DoMPost
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        (ANSC_HANDLE)pBmoReq,
                                        (ANSC_HANDLE)pBmoRep
                                    );

                            break;
                        }
                        else if ( pBmoReq->GetMethod((ANSC_HANDLE)pBmoReq) == HTTP_METHOD_CODE_SUBSCRIBE )
                        {
                            returnStatus =
                                pMyObject->DoSubscribe
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        (ANSC_HANDLE)pBmoReq,
                                        (ANSC_HANDLE)pBmoRep
                                    );

                            break;
                        }
                        else if ( pBmoReq->GetMethod((ANSC_HANDLE)pBmoReq) == HTTP_METHOD_CODE_UNSUBSCRIBE )
                        {
                            returnStatus =
                                pMyObject->DoUnsubscribe
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        (ANSC_HANDLE)pBmoReq,
                                        (ANSC_HANDLE)pBmoRep
                                    );

                            break;
                        }
                    }

                    returnStatus =
                        pMyObject->ReplyReq
                            (
                                (ANSC_HANDLE)pMyObject,
                                (ANSC_HANDLE)pBmoReq,
                                (ANSC_HANDLE)pBmoRep,
                                HTTP_STATUS_NOT_IMPLEMENTED,
                                (ANSC_HANDLE)NULL
                            );

                    break;
        }
    }

    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    AnscDelTokenChain((ANSC_HANDLE)pPathTokenChain);
    AnscFreeMemory   ((ANSC_HANDLE)pPathTokenChain);

    return  returnStatus;
}
