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

    module:	http_bmoreq_auth.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Http Bmo Req Object.

        *   HttpBmoReqGetStartLineSize


    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        11/14/06    initial revision.

**********************************************************************/


#include "http_bmoreq_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoReqGetAuthInfo
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_STATUS
HttpBmoReqGetAuthInfo
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT)hThisObject;

    if ( !pMyObject->hAuthInfo )
    {
        PHTTP_AUTH_OBJECT           pHttpAuthObj = NULL;

        pHttpAuthObj = (PHTTP_AUTH_OBJECT)HttpCreateAutho(NULL, NULL, NULL);

        if ( !pHttpAuthObj )
        {
            return ANSC_STATUS_RESOURCES;
        }

        pMyObject->hAuthInfo = 
            (ANSC_HANDLE)pHttpAuthObj->GetAuthInfo
                (
                    (ANSC_HANDLE)pHttpAuthObj, 
                    (ANSC_HANDLE)pMyObject
                );

        pHttpAuthObj->Remove((ANSC_HANDLE)pHttpAuthObj);
    }

    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        HttpBmoReqGetAuthUser
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

char*
HttpBmoReqGetAuthUser
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject   = (PHTTP_BMO_REQ_OBJECT)hThisObject;
    PHTTP_AUTHO_INFO                pAuthInfo   = (PHTTP_AUTHO_INFO    )pMyObject->hAuthInfo;

    if ( !pAuthInfo )
    {
        pMyObject->GetAuthInfo((ANSC_HANDLE)pMyObject);
        pAuthInfo   = (PHTTP_AUTHO_INFO)pMyObject->hAuthInfo;
    }

    return pAuthInfo ? (char*)pAuthInfo->pUserName : NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpBmoReqGetAuthScheme
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ULONG
HttpBmoReqGetAuthScheme
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject   = (PHTTP_BMO_REQ_OBJECT)hThisObject;
    PHTTP_AUTHO_INFO                pAuthInfo   = (PHTTP_AUTHO_INFO    )pMyObject->hAuthInfo;

    if ( !pAuthInfo )
    {
        pMyObject->GetAuthInfo((ANSC_HANDLE)pMyObject);
        pAuthInfo   = (PHTTP_AUTHO_INFO)pMyObject->hAuthInfo;

    }

    return pAuthInfo ? pAuthInfo->AuthType : HTTP_AUTH_TYPE_NONE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char *
        HttpBmoReqGetAuthRealm
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

char*
HttpBmoReqGetAuthRealm
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject   = (PHTTP_BMO_REQ_OBJECT)hThisObject;
    PHTTP_AUTHO_INFO                pAuthInfo   = (PHTTP_AUTHO_INFO    )pMyObject->hAuthInfo;

    if ( !pAuthInfo )
    {
        pMyObject->GetAuthInfo((ANSC_HANDLE)pMyObject);
        pAuthInfo   = (PHTTP_AUTHO_INFO)pMyObject->hAuthInfo;
    }

    return pAuthInfo ? (char*)pAuthInfo->pRealm : NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char *
        HttpBmoReqGetAuthDigestUri
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

char*
HttpBmoReqGetAuthDigestUri
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject   = (PHTTP_BMO_REQ_OBJECT)hThisObject;
    PHTTP_AUTHO_INFO                pAuthInfo   = (PHTTP_AUTHO_INFO    )pMyObject->hAuthInfo;

    if ( !pAuthInfo )
    {
        pMyObject->GetAuthInfo((ANSC_HANDLE)pMyObject);
        pAuthInfo   = (PHTTP_AUTHO_INFO)pMyObject->hAuthInfo;
    }

    return (pAuthInfo && pAuthInfo->pDigest) ? (char*)pAuthInfo->pDigest->pDigestUri : NULL;
}


