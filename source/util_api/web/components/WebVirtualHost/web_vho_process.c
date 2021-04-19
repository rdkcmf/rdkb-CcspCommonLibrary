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

    module:	web_vho_process.c

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the Web Virtual Host Object.

        *   WebVhoQuery
        *   WebVhoProcess
        *   WebVhoNotify
        *   WebVhoConstructResponse
        *   WebVhoAddHttpRepHeaders
        *   WebVhoGetResourcePath

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/11/02    initial revision.

**********************************************************************/


#include "web_vho_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        WebVhoQuery
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq,
                ANSC_HANDLE                 hBmoRep,
                PANSC_HANDLE                phQueryContext
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

                PANSC_HANDLE                phQueryContext
                Specifies the context handle to be passed to process().

    return:     HTTP status code.

**********************************************************************/

ULONG
WebVhoQuery
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        PANSC_HANDLE                phQueryContext
    )
{
    UNREFERENCED_PARAMETER(hBmoRep);
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PWEB_VIRTUAL_HOST_OBJECT        pMyObject          = (PWEB_VIRTUAL_HOST_OBJECT    )hThisObject;
    PWEB_RESOURCE_LOCATOR_OBJECT    pResourceLocator   = (PWEB_RESOURCE_LOCATOR_OBJECT)pMyObject->hResourceLocator;
    PHTTP_BMO_REQ_OBJECT            pBmoReq            = (PHTTP_BMO_REQ_OBJECT        )hBmoReq;
    PWEB_RESOURCE_OWNER_OBJECT      pResourceOwner     = NULL;
    ULONG                           ulHttpStatusCode   = HTTP_STATUS_OK;
    PANSC_TOKEN_CHAIN               pUriPathTokenChain = NULL;

    *phQueryContext    = (ANSC_HANDLE)NULL;
    pUriPathTokenChain = (PANSC_TOKEN_CHAIN)AnscAllocateMemory(sizeof(ANSC_TOKEN_CHAIN));

    if ( !pUriPathTokenChain )
    {
        ulHttpStatusCode = HTTP_STATUS_INTERNAL_SERVER_ERROR;

        goto  EXIT1;
    }
    else
    {
        AnscQueueInitializeHeader(&pUriPathTokenChain->TokensQueue);
    }

    if ( TRUE )
    {
        returnStatus =
            pMyObject->GetResourcePath
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pBmoReq,
                    (ANSC_HANDLE)pUriPathTokenChain,
                    FALSE
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            ulHttpStatusCode = HTTP_STATUS_INTERNAL_SERVER_ERROR;

            goto  EXIT2;
        }
    }

    pResourceOwner =
        (PWEB_RESOURCE_OWNER_OBJECT)pResourceLocator->GetResourceOwner2
            (
                (ANSC_HANDLE)pResourceLocator,
                (ANSC_HANDLE)pUriPathTokenChain
            );

    if ( !pResourceOwner )
    {
        AnscDelTokenChain((ANSC_HANDLE)pUriPathTokenChain);

        returnStatus =
            pMyObject->GetResourcePath
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pBmoReq,
                    (ANSC_HANDLE)pUriPathTokenChain,
                    TRUE
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            ulHttpStatusCode = HTTP_STATUS_INTERNAL_SERVER_ERROR;

            goto  EXIT2;
        }

        pResourceOwner =
            (PWEB_RESOURCE_OWNER_OBJECT)pResourceLocator->GetResourceOwner2
                (
                    (ANSC_HANDLE)pResourceLocator,
                    (ANSC_HANDLE)pUriPathTokenChain
                );

        if ( !pResourceOwner )
        {
            ulHttpStatusCode = HTTP_STATUS_NOT_FOUND;

            goto  EXIT2;
        }
    }

    if ( TRUE )
    {
        pBmoReq->SetRecvContext((ANSC_HANDLE)pBmoReq, (ANSC_HANDLE)pUriPathTokenChain);
    }

    ulHttpStatusCode = HTTP_STATUS_OK;
    *phQueryContext  = (ANSC_HANDLE)pResourceOwner;

    goto  EXIT1;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT2:

    AnscDelTokenChain((ANSC_HANDLE)pUriPathTokenChain);
    AnscFreeMemory   ((ANSC_HANDLE)pUriPathTokenChain);

EXIT1:

    return  ulHttpStatusCode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebVhoProcess
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq,
                ANSC_HANDLE                 hBmoRep,
                ANSC_HANDLE                 hQueryContext
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

                ANSC_HANDLE                 hQueryContext
                Specifies the context handle passed by query().

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebVhoProcess
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hQueryContext
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PWEB_VIRTUAL_HOST_OBJECT        pMyObject          = (PWEB_VIRTUAL_HOST_OBJECT    )hThisObject;
    PHTTP_BMO_REQ_OBJECT            pBmoReq            = (PHTTP_BMO_REQ_OBJECT        )hBmoReq;
    PHTTP_BMO_REP_OBJECT            pBmoRep            = (PHTTP_BMO_REP_OBJECT        )hBmoRep;
    PWEB_RESOURCE_OWNER_OBJECT      pResourceOwner     = (PWEB_RESOURCE_OWNER_OBJECT  )hQueryContext;
    PANSC_TOKEN_CHAIN               pUriPathTokenChain = (PANSC_TOKEN_CHAIN           )pBmoReq->GetRecvContext((ANSC_HANDLE)pBmoReq);
    PWEB_GENERAL_SESSION_OBJECT     pSession           = NULL;

    pSession =
        (PWEB_GENERAL_SESSION_OBJECT)pMyObject->IdentifyGso
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pBmoReq
            );

    pBmoReq->SetRecvContext((ANSC_HANDLE)pBmoReq, (ANSC_HANDLE)pUriPathTokenChain);
    pBmoRep->SetRecvContext((ANSC_HANDLE)pBmoRep, (ANSC_HANDLE)pSession          );
    pBmoRep->SetWebHost    ((ANSC_HANDLE)pBmoRep, (ANSC_HANDLE)pMyObject         );

    if ( pSession )
    {
        pSession->LastReqAtInSec = AnscGetTickInSecondsAbs();

        /*
         * The 'SendContext' of the HttpBmoReq object was set to the associated AnscDaemonSocket
         * object by WebSiteManager object. The session object needs this reference to lock/unlock
         * the socket if necessary.
         */
        pSession->SetActiveSocket((ANSC_HANDLE)pSession, pBmoReq->GetSendContext((ANSC_HANDLE)pBmoReq));
    }

    returnStatus =
        pResourceOwner->Process
            (
                (ANSC_HANDLE)pResourceOwner,
                (ANSC_HANDLE)pBmoReq,
                (ANSC_HANDLE)pBmoRep
            );

    if ( !(pBmoRep->ProgressBits & HTTP_BMOREP_PROGRESS_hostHeadersAdded) )
    {
        returnStatus =
            pMyObject->AddHttpRepHeaders
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pBmoRep
                );
    }

    if ( pSession )
    {
        pSession->LastRepAtInSec = AnscGetTickInSecondsAbs();
        pSession->RequestCount++;

        pSession->ReleaseAccess((ANSC_HANDLE)pSession);

        /* We don't need to do session removal here, let timer thread do it

        ulCurTime = AnscGetTickInSeconds();

        if ( pSession->MaxLifespan != 0 )
        {
            bExpired = WEB_SEESION_EXPIRED(pSession->TimeStampInSec, ulCurTime, pSession->MaxLifespan);
        }

        if (!bExpired)
        {
            bExpired = ( pSession->GetLsmMaxAge((ANSC_HANDLE)pSession) == 0 );
        }

        if ( bExpired )
        {
            returnStatus =
                pMyObject->DelGso
                    (
                        (ANSC_HANDLE)pMyObject,
                        pSession->GetSessionId((ANSC_HANDLE)pSession),
                        pBmoReq->GetClientAddr((ANSC_HANDLE)pBmoReq),
                        pBmoReq->GetClientPort((ANSC_HANDLE)pBmoReq)
                    );
        }
        else if ( pBmoRep->GetCode((ANSC_HANDLE)pBmoRep) >= HTTP_STATUS_BAD_REQUEST )
        {
            if ( pSession->RequestCount < 3 && pBmoRep->GetCode((ANSC_HANDLE)pBmoRep) != HTTP_STATUS_UNAUTHORIZED )
            {
                returnStatus =
                    pMyObject->DelGso
                        (
                            (ANSC_HANDLE)pMyObject,
                            pSession->GetSessionId((ANSC_HANDLE)pSession),
                            pBmoReq->GetClientAddr((ANSC_HANDLE)pBmoReq),
                            pBmoReq->GetClientPort((ANSC_HANDLE)pBmoReq)
                        );
            }
        }

        if ( pSession->RequestCount > 3 && pBmoRep->GetCode((ANSC_HANDLE)pBmoRep) == HTTP_STATUS_UNAUTHORIZED )
        {
            returnStatus =
                pMyObject->DelGso
                    (
                        (ANSC_HANDLE)pMyObject,
                        pSession->GetSessionId((ANSC_HANDLE)pSession),
                        pBmoReq->GetClientAddr((ANSC_HANDLE)pBmoReq),
                        pBmoReq->GetClientPort((ANSC_HANDLE)pBmoReq)
                    );
        }
        */
    }

    pBmoReq->SetRecvContext((ANSC_HANDLE)pBmoReq, (ANSC_HANDLE)NULL);
    pBmoRep->SetRecvContext((ANSC_HANDLE)pBmoRep, (ANSC_HANDLE)NULL);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebVhoNotify
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq,
                ULONG                       ulEvent
            );

    description:

        This function is called to process a web request event.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the HTTP request message object.

                ULONG                       ulEvent
                Specifies the HTTP event to be processed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebVhoNotify
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ULONG                       ulEvent
    )
{
    UNREFERENCED_PARAMETER(ulEvent);
    PWEB_VIRTUAL_HOST_OBJECT        pMyObject    = (PWEB_VIRTUAL_HOST_OBJECT  )hThisObject;
    PHTTP_BMO_REQ_OBJECT            pBmoReq      = (PHTTP_BMO_REQ_OBJECT      )hBmoReq;
    PWEB_GENERAL_SESSION_OBJECT     pSession     = NULL;

    pSession =
        (PWEB_GENERAL_SESSION_OBJECT)pMyObject->IdentifyGso
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pBmoReq
            );

    if ( pSession )
    {
        pSession->LastReqAtInSec = AnscGetTickInSecondsAbs();

        pSession->ReleaseAccess((ANSC_HANDLE)pSession);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebVhoConstructResponse
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq,
                ANSC_HANDLE                 hBmoRep,
                ULONG                       ulStatusCode
            );

    description:

        This function is called to construct a web server resposne.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the HTTP request message object.

                ANSC_HANDLE                 hBmoRep
                Specifies the HTTP response message object.

                ULONG                       ulStatusCode
                Specifies the server status code to be returned.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebVhoConstructResponse
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ULONG                       ulStatusCode
    )
{
    UNREFERENCED_PARAMETER(hBmoReq);
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PWEB_VIRTUAL_HOST_OBJECT        pMyObject    = (PWEB_VIRTUAL_HOST_OBJECT     )hThisObject;
    PHTTP_HELPER_CONTAINER_OBJECT   pHttpHco     = (PHTTP_HELPER_CONTAINER_OBJECT)pMyObject->hContainerContext;
    PHTTP_BMO_REP_OBJECT            pBmoRep      = (PHTTP_BMO_REP_OBJECT         )hBmoRep;
    PHTTP_RESPONSE_INFO             pRepInfo     = NULL;
    char*                           pRepPhrase   = pHttpHco->GetReasonPhrase((ANSC_HANDLE)pHttpHco, ulStatusCode);
    errno_t                         rc           = -1;

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

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebVhoAddHttpRepHeaders
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoRep
            );

    description:

        This function is called to add some general response headers.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoRep
                Specifies the HTTP response message object.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebVhoAddHttpRepHeaders
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoRep
    )
{
    PWEB_VIRTUAL_HOST_OBJECT        pMyObject         = (PWEB_VIRTUAL_HOST_OBJECT     )hThisObject;
    PWEB_VIRTUAL_HOST_PROPERTY      pProperty         = (PWEB_VIRTUAL_HOST_PROPERTY   )&pMyObject->Property;
    PHTTP_HELPER_CONTAINER_OBJECT   pHttpHco          = (PHTTP_HELPER_CONTAINER_OBJECT)pMyObject->hContainerContext;
    PHTTP_BMO_REP_OBJECT            pBmoRep           = (PHTTP_BMO_REP_OBJECT         )hBmoRep;
    PWEB_GENERAL_SESSION_OBJECT     pSession          = (PWEB_GENERAL_SESSION_OBJECT  )pBmoRep->GetRecvContext((ANSC_HANDLE)pBmoRep);
    PHTTP_HFO_SET_COOKIE            pHttpHfoSetCookie1= NULL;
    PHTTP_HFO_SET_COOKIE            pHttpHfoSetCookie2= NULL;
    PHTTP_COOKIE_CONTENT            pVhoCookieContent = NULL;
    PHTTP_COOKIE_CONTENT            pLsmCookieContent = NULL;
    ULONG                           ulTimeInSeconds   = 0;
    ANSC_UNIVERSAL_TIME             curCalendarTime;
    char*                           pLsmId            = NULL;
    errno_t                         rc                = -1;

    if ( !pProperty->bEnableSessionTracking )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else if ( !pSession )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pHttpHco->GetClockTime
                (
                    (ANSC_HANDLE)pHttpHco,
                    (ANSC_HANDLE)&curCalendarTime
                );

        ulTimeInSeconds = AnscCalendarToSecond((ANSC_HANDLE)&curCalendarTime);
    }

    /*
     * The origin server initiates a session, if it so desires. (Note that "session" here does not
     * refer to a persistent network connection but to a logical session created from HTTP requests
     * and responses. The presence or absence of a persistent connection should have no effect on
     * the use of cookie-derived sessions). To initiate a session, the origin server returns an
     * extra response header to the client, Set-Cookie. The syntax for the Set-Cookie response
     * header is
     *
     *      set-cookie      =       "Set-Cookie:" cookies
     *      cookies         =       1#cookie
     *      cookie          =       NAME "=" VALUE *(";" cookie-av)
     *      NAME            =       attr
     *      VALUE           =       value
     *      cookie-av       =       "Comment" "=" value
     *                      |       "Domain" "=" value
     *                      |       "Max-Age" "=" value
     *                      |       "Path" "=" value
     *                      |       "Secure"
     *                      |       "Version" "=" 1*DIGIT
     *
     * Informally, the Set-Cookie response header comprises the token Set-Cookie:, followed by a
     * comma-separated list of one or more cookies. Each cookie begins with a NAME=VALUE pair,
     * followed by zero or more semi-colon-separated attribute-value pairs. The syntax for
     * attribute-value pairs was shown earlier. The specific attributes and the semantics of their
     * values follows. The NAME=VALUE attribute-value pair must come first in each cookie. The
     * others, if present, can occur in any order. If an attribute appears more than once in a
     * cookie, the behavior is undefined.
     */

    /*
     * Create multiple Set-Cookie headers instead of putting both cookies into one header for
     * compatibility problems.
     */

    pLsmId = pSession->GetLsmIdentifier((ANSC_HANDLE)pSession);

    pHttpHfoSetCookie1 = (PHTTP_HFO_SET_COOKIE)AnscAllocateMemory(sizeof(HTTP_HFO_SET_COOKIE) + sizeof(HTTP_COOKIE_CONTENT) + 16);

    if ( pLsmId )
    {
        pHttpHfoSetCookie2 = (PHTTP_HFO_SET_COOKIE)AnscAllocateMemory(sizeof(HTTP_HFO_SET_COOKIE) + sizeof(HTTP_COOKIE_CONTENT) + 16);
    }

    if ( !pHttpHfoSetCookie1 || (!pHttpHfoSetCookie2 && pLsmId) )
    {
        if ( pHttpHfoSetCookie1 )
        {
            AnscFreeMemory(pHttpHfoSetCookie1);
        }

        if ( pHttpHfoSetCookie2 )
        {
            AnscFreeMemory(pHttpHfoSetCookie2);
        }

        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pHttpHfoSetCookie1->HashIndex   = 0;
        pHttpHfoSetCookie1->Flags       = 0;
        pHttpHfoSetCookie1->HeaderId    = HTTP_HEADER_ID_SET_COOKIE;
        pHttpHfoSetCookie1->CookieCount = 1;

        if ( pHttpHfoSetCookie2 )
        {
            pHttpHfoSetCookie2->HashIndex   = 0;
            pHttpHfoSetCookie2->Flags       = 0;
            pHttpHfoSetCookie2->HeaderId    = HTTP_HEADER_ID_SET_COOKIE;
            pHttpHfoSetCookie2->CookieCount = 1;
    
            pLsmCookieContent = &pHttpHfoSetCookie2->CookieArray[0];
        }

        pVhoCookieContent = &pHttpHfoSetCookie1->CookieArray[0];
    }

    if ( TRUE )
    {
        pVhoCookieContent->bSecure         = FALSE;
        pVhoCookieContent->bPermanent      = FALSE;
        pVhoCookieContent->Version         = HTTP_CURRENT_COOKIE_VERSION;
        pVhoCookieContent->MaxAgeInSeconds = pProperty->VhoCookieMaxAge;

        if ( pVhoCookieContent->bPermanent )
        {
            AnscSecondToCalendar
                (
                    ulTimeInSeconds + pVhoCookieContent->MaxAgeInSeconds,
                    (ANSC_HANDLE)&pVhoCookieContent->Expires
                );
        }

        rc = strcpy_s(pVhoCookieContent->Name, sizeof(pVhoCookieContent->Name), pProperty->VhoCookieName);
        ERR_CHK(rc);
        rc = strcpy_s(pVhoCookieContent->Path, sizeof(pVhoCookieContent->Path), HTTP_ROOT_URI_PATH      );
        ERR_CHK(rc);

        AnscGetUlongString
            (
                pSession->GetSessionId((ANSC_HANDLE)pSession),
                pVhoCookieContent->Value
            );
    }

    if ( pLsmId )
    {
        pLsmCookieContent->bSecure         = FALSE;
        pLsmCookieContent->bPermanent      = FALSE;
        pLsmCookieContent->Version         = HTTP_CURRENT_COOKIE_VERSION;
        pLsmCookieContent->MaxAgeInSeconds = pSession->GetLsmMaxAge((ANSC_HANDLE)pSession);

        if ( pLsmCookieContent->bPermanent )
        {
            AnscSecondToCalendar
                (
                    ulTimeInSeconds + pLsmCookieContent->MaxAgeInSeconds,
                    (ANSC_HANDLE)&pLsmCookieContent->Expires
                );
        }

        rc = strcpy_s(pLsmCookieContent->Name, sizeof(pLsmCookieContent->Name), pProperty->LsmCookieName);
        ERR_CHK(rc);
        rc = strcpy_s(pLsmCookieContent->Path, sizeof(pLsmCookieContent->Path), HTTP_ROOT_URI_PATH      );
        ERR_CHK(rc);

        if ( pLsmId )
        {
        rc = strcpy_s
            (
                pLsmCookieContent->Value,
                sizeof(pLsmCookieContent->Value),
                pLsmId
            );
        ERR_CHK(rc);
        }
    }

    pBmoRep->AddHeaderField
            (
                (ANSC_HANDLE)pBmoRep,
                (ANSC_HANDLE)pHttpHfoSetCookie1
            );

    if ( pLsmId )
    {
        pBmoRep->AddHeaderField
                (
                    (ANSC_HANDLE)pBmoRep,
                    (ANSC_HANDLE)pHttpHfoSetCookie2
                );
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebVhoGetResourcePath
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq,
                ANSC_HANDLE                 hTokenChain,
                BOOL                        bReferred
            );

    description:

        This function is called to construct the URI path for the
        requested resource.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the HTTP request message object.

                ANSC_HANDLE                 hTokenChain
                Specifies the path token chain that is used to hold
                all the path tokens.

                BOOL                        bReferred
                Specifies how the resource path should be constructed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebVhoGetResourcePath
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hTokenChain,
        BOOL                        bReferred
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PHTTP_BMO_REQ_OBJECT            pBmoReq            = (PHTTP_BMO_REQ_OBJECT      )hBmoReq;
    PHTTP_REQUEST_INFO              pReqInfo           = (PHTTP_REQUEST_INFO        )pBmoReq->GetReqInfo    ((ANSC_HANDLE)pBmoReq);
    PHTTP_HFO_REFERER               pHttpHfoReferer    = (PHTTP_HFO_REFERER         )pBmoReq->GetHeaderField((ANSC_HANDLE)pBmoReq, HTTP_HEADER_ID_REFERER);
    PHTTP_HFO_HOST                  pHttpHfoHost       = (PHTTP_HFO_HOST            )pBmoReq->GetHeaderField((ANSC_HANDLE)pBmoReq, HTTP_HEADER_ID_HOST   );
    PANSC_TOKEN_CHAIN               pUriPathTokenChain = (PANSC_TOKEN_CHAIN         )hTokenChain;
    PANSC_STRING_TOKEN              pUriPathToken      = NULL;
    ULONG                           i                  = 0;
    errno_t                         rc                 = -1;

    if ( !bReferred )
    {
        for ( i = 1; i < pReqInfo->RequestUri.PathLevel; i++ )
        {
            pUriPathToken = (PANSC_STRING_TOKEN)AnscAllocateMemory(sizeof(ANSC_STRING_TOKEN));

            if ( !pUriPathToken )
            {
                return  ANSC_STATUS_RESOURCES;
            }
            else
            {
                rc = strcpy_s(pUriPathToken->Name, sizeof(pUriPathToken->Name), pReqInfo->RequestUri.PathArray[i]);
                ERR_CHK(rc);

                AnscQueuePushEntry(&pUriPathTokenChain->TokensQueue, &pUriPathToken->Linkage);
            }
        }

        return  ANSC_STATUS_SUCCESS;
    }

    /*
     * We have to construct a token chain which encapsulate the complete absolute path for the
     * resource being requested. The existence of "Referer" header makes the whole situation even
     * more complicated: we have to consider the case where the requested URI is relative path to
     * the previously requested page, whose URI is now encapsulated by the "Referer" header.
     */
    while ( pHttpHfoReferer && (pReqInfo->RequestUri.Type == HTTP_URI_TYPE_ABS_PATH) )
    {
        if ( pHttpHfoHost )
        {
            if ( strcasecmp(pHttpHfoReferer->ReferrerUri.HostName,pHttpHfoHost->HostName) == 0 )
            {
                break;
            }
        }
        else if ( pReqInfo->RequestUri.Type == HTTP_URI_TYPE_ABSOLUTE_URI )
        {
            if ( !strcasecmp(pHttpHfoReferer->ReferrerUri.HostName,pReqInfo->RequestUri.HostName) == 0 )
            {
                break;
            }
        }
        else
        {
            break;
        }

        if ( pHttpHfoReferer->ReferrerUri.PathLevel <= 1 )
        {
            break;
        }
        else if ( pReqInfo->RequestUri.PathLevel <= 1 )
        {
            break;
        }

        /*
         * By now, we have every reason to believe that the current request URI is a relative path
         * to the referred URI. Since the Resource Locator can only deal with absolute path, we
         * need to construct the complete path token chain.
         */
        for ( i = 1; i < pHttpHfoReferer->ReferrerUri.PathLevel; i++ )
        {
            pUriPathToken = (PANSC_STRING_TOKEN)AnscAllocateMemory(sizeof(ANSC_STRING_TOKEN));

            if ( !pUriPathToken )
            {
                return  ANSC_STATUS_RESOURCES;
            }
            else
            {
                rc = strcpy_s(pUriPathToken->Name, sizeof(pUriPathToken->Name), pHttpHfoReferer->ReferrerUri.PathArray[i]);
                ERR_CHK(rc);

                AnscQueuePushEntry(&pUriPathTokenChain->TokensQueue, &pUriPathToken->Linkage);
            }
        }

        break;
    }

    for ( i = 1; i < pReqInfo->RequestUri.PathLevel; i++ )
    {
        pUriPathToken = (PANSC_STRING_TOKEN)AnscAllocateMemory(sizeof(ANSC_STRING_TOKEN));

        if ( !pUriPathToken )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            rc = strcpy_s(pUriPathToken->Name, sizeof(pUriPathToken->Name), pReqInfo->RequestUri.PathArray[i]);
            ERR_CHK(rc);

            AnscQueuePushEntry(&pUriPathTokenChain->TokensQueue, &pUriPathToken->Linkage);
        }
    }

    return  ANSC_STATUS_SUCCESS;
}
