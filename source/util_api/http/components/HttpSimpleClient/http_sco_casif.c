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

    module:	http_sco_casif.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced interface functions
        of the Http Simple Client Object.

        *   HttpScoCasEnableAuth
        *   HttpScoCasIsAuthEnabled
        *   HttpScoCasProcessRequest
        *   HttpScoCasProcessResponse
        *   HttpScoCasClearAuthInfo

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        10/11/05    initial revision.

**********************************************************************/


#include "http_sco_global.h"


#if 0
static void
HttpScoSetCookie
    (
        ANSC_HANDLE                 hHfpIf,
        ANSC_HANDLE                 hResponse,
        ANSC_HANDLE                 hRequest
    );
#endif


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpScoCasEnableAuth
            (
                ANSC_HANDLE                 hThisObject,
                BOOL                        bEnable
            )

    description:

        This function is called to enable HTTP client authentication.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                BOOL                        bEnable
                TRUE to enable client authentication, disable the 
                service if it is FALSE.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpScoCasEnableAuth
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnable
    )
{
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject       = (PHTTP_SIMPLE_CLIENT_OBJECT)hThisObject;
    PHTTP_AUTH_CLIENT_OBJECT        pClientAuthObj  = (PHTTP_AUTH_CLIENT_OBJECT  )pMyObject->hClientAuthObj;

    if ( !pClientAuthObj )
    {
        if ( bEnable )
        {
            pClientAuthObj  = (PHTTP_AUTH_CLIENT_OBJECT)HttpCreateAuthco(NULL, (ANSC_HANDLE)pMyObject, NULL);

            pMyObject->hClientAuthObj   = (ANSC_HANDLE)pClientAuthObj;

            if ( !pClientAuthObj )
            {
                status  = ANSC_STATUS_RESOURCES;
            }
            else
            {
                pClientAuthObj->Enable((ANSC_HANDLE)pClientAuthObj, TRUE);
            }
        }
    }
    else
    {
        if ( !bEnable )
        {
            pClientAuthObj->Enable((ANSC_HANDLE)pClientAuthObj, FALSE);
            pMyObject->hClientAuthObj   = (ANSC_HANDLE)NULL;
            pClientAuthObj->Remove((ANSC_HANDLE)pClientAuthObj);
        }
        else
        {
            pClientAuthObj->Enable((ANSC_HANDLE)pClientAuthObj, TRUE);
        }
    }

    return status;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        HttpScoCasIsAuthEnabled
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to return if client authentication
        is enabled.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     TRUE if it is enabled, FALSE otherwise.

**********************************************************************/

BOOL
HttpScoCasIsAuthEnabled
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject       = (PHTTP_SIMPLE_CLIENT_OBJECT)hThisObject;
    PHTTP_AUTH_CLIENT_OBJECT        pClientAuthObj  = (PHTTP_AUTH_CLIENT_OBJECT  )pMyObject->hClientAuthObj;

    if ( !pClientAuthObj )
    {
        return FALSE;
    }
    else
    {
        return pClientAuthObj->IsEnabled((ANSC_HANDLE)pClientAuthObj);
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpScoCasProcessRequest
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hRequest
            )

    description:

        This function is called to process HTTP response object before
        indicating to SSP through BSP interface.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hRequest
                HTTP request object.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpScoCasProcessRequest
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRequest
    )
{
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject       = (PHTTP_SIMPLE_CLIENT_OBJECT)hThisObject;
    PHTTP_AUTH_CLIENT_OBJECT        pClientAuthObj  = (PHTTP_AUTH_CLIENT_OBJECT  )pMyObject->hClientAuthObj;
    PHTTP_CAS_INTERFACE             pCasIf          = (PHTTP_CAS_INTERFACE       )pMyObject->hCasIf;

    if ( !pCasIf->IsAuthEnabled(pCasIf->hOwnerContext) )
    {
        return ANSC_STATUS_SUCCESS;
    }

    status = pClientAuthObj->ProcessReq((ANSC_HANDLE)pClientAuthObj, hRequest);

    return status;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpScoCasProcessResponse
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hRequest,
                ANSC_HANDLE                 hResponse,
                ANSC_HANDLE                 hWebcTransObj
            )

    description:

        This function is called to process HTTP response object before
        indicating to SSP through BSP interface.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hRequest
                HTTP request object.

                ANSC_HANDLE                 hRespones
                HTTP response object.

                ANSC_HANDLE                 hWebcTransObj
                HTTP Webc Trans object.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpScoCasProcessResponse
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRequest,
        ANSC_HANDLE                 hResponse,
        ANSC_HANDLE                 hWebcTransObj
    )
{
    UNREFERENCED_PARAMETER(hWebcTransObj);
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject       = (PHTTP_SIMPLE_CLIENT_OBJECT)hThisObject;
    PHTTP_AUTH_CLIENT_OBJECT        pClientAuthObj  = (PHTTP_AUTH_CLIENT_OBJECT  )pMyObject->hClientAuthObj;
    PHTTP_CAS_INTERFACE             pCasIf          = (PHTTP_CAS_INTERFACE       )pMyObject->hCasIf;
    
    if ( !pCasIf->IsAuthEnabled(pCasIf->hOwnerContext) )
    {
        return ANSC_STATUS_SUCCESS;
    }

    status = pClientAuthObj->ProcessRep((ANSC_HANDLE)pClientAuthObj, hRequest, hResponse);

    if ( status == ANSC_STATUS_DO_IT_AGAIN )
    {
        /* 
         * we will let application handle authentication instead of 
         * doing it here since some HTTP servers may close the connection
         * right away.
         */
        status = ANSC_STATUS_SUCCESS;

#if 0 
        status =
            pBspIf->Browse
                (
                    pBspIf->hOwnerContext,
                    hRequest,
                    hResponse,
                    pWebcTrans->hBspReqContext
                );

        /* send the request again */
        status = pCasIf->ProcessRequest(pCasIf->hOwnerContext, hRequest);

        if ( status == ANSC_STATUS_SUCCESS )
        {
            HttpScoSetCookie(pMyObject->hHfpIf, hResponse, hRequest);

            status = 
                pWebcTrans->Send
                    (
                        (ANSC_HANDLE)pWebcTrans,
                        hRequest,
                        HTTP_WCTO_SFLAG_HEADERS | HTTP_WCTO_SFLAG_BODY
                    );

            if ( status == ANSC_STATUS_SUCCESS )
            {
                /* the response should not indicate through BSP interface */
                status = ANSC_STATUS_DO_IT_AGAIN;
            }
        }
#endif
    }

    return status;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpScoCasClearAuthInfo
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to clear all cached authentication
        info - this call enforces re-authentication to server to
        reflect changes to authentication scheme, username and
        password etc.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpScoCasClearAuthInfo
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject       = (PHTTP_SIMPLE_CLIENT_OBJECT)hThisObject;
    PHTTP_AUTH_CLIENT_OBJECT        pClientAuthObj  = (PHTTP_AUTH_CLIENT_OBJECT  )pMyObject->hClientAuthObj;

    if ( pClientAuthObj )
    {
        pClientAuthObj->ClearAllAuthInfo((ANSC_HANDLE)pClientAuthObj);
    }

    return status;
}


#if 0
static void
HttpScoSetCookie
    (
        ANSC_HANDLE                 hHfpIf,
        ANSC_HANDLE                 hResponse,
        ANSC_HANDLE                 hRequest
    )
{
    PHTTP_HFP_INTERFACE             pHttpHfpIf  = (PHTTP_HFP_INTERFACE)hHfpIf;
    PHTTP_BMO_REP_OBJECT            pResponse   = (PHTTP_BMO_REP_OBJECT)hResponse;
    PHTTP_BMO_REQ_OBJECT            pRequest    = (PHTTP_BMO_REQ_OBJECT)hRequest;
    char*                           pSetCookie  = NULL;
    BOOL                            bSetCookie2 = FALSE;
    ULONG                           ulCount1    = 0;
    ULONG                           ulCount2    = 0;
    PHTTP_HEADER_FIELD              pHttpHfo     = NULL;

    pRequest->DelHeaderField((ANSC_HANDLE)pRequest, HTTP_HEADER_ID_COOKIE);

    while ( TRUE )
    {
        pSetCookie = pResponse->GetHeaderValueByName2((ANSC_HANDLE)pResponse, "Set-Cookie2", ulCount1++);
        if ( !pSetCookie )
        {
            pSetCookie = pResponse->GetHeaderValueByName2((ANSC_HANDLE)pResponse, "Set-Cookie", ulCount2++);
            bSetCookie2 = TRUE;
        }

        if ( !pSetCookie )
        {
            break;
        }
        else
        {
            char*                       pCookie = NULL;
            ULONG                       ulCookieSize    = AnscSizeOfString(pSetCookie) + 64;

            pCookie = (char *)AnscAllocateMemory(ulCookieSize);

            if ( pCookie )
            {
                PHTTP_HFO_SET_COOKIE    pHfoSetCookie = NULL;

                if ( bSetCookie2 )
                {
                    _ansc_sprintf(pCookie, "Set-Cookie2: %s", pSetCookie);
                }
                else
                {
                    _ansc_sprintf(pCookie, "Set-Cookie: %s", pSetCookie);
                }

                pHfoSetCookie = (PHTTP_HFO_SET_COOKIE)pHttpHfpIf->ParseHeader(pHttpHfpIf->hOwnerContext, pCookie, AnscSizeOfString(pCookie));

                if ( pHfoSetCookie )
                {
                    char*               pCookieValue = NULL;

                    pHfoSetCookie->HeaderId = HTTP_HEADER_ID_COOKIE;
                    pHfoSetCookie->Flags &= ~HTTP_FIELD_FLAG_VALUE_PRESENT;
                    pHfoSetCookie->Flags &= ~HTTP_FIELD_FLAG_LINE_PRESENT;

                    ulCookieSize = pHttpHfpIf->GetHeaderSize(pHttpHfpIf->hOwnerContext, (ANSC_HANDLE)pHfoSetCookie);
                    pHttpHfpIf->BuildHeader(pHttpHfpIf->hOwnerContext, (ANSC_HANDLE)pHfoSetCookie, pCookie, ulCookieSize);
                    pCookie[ulCookieSize] = 0;

                    if ( pCookie )
                    {
                        pRequest->SetHeaderValueByName((ANSC_HANDLE)pRequest, "Cookie", pCookie + 8);
                    }

                    AnscFreeMemory(pHfoSetCookie);
                }

                AnscFreeMemory(pCookie);
            }
        }
    }
}
#endif 

