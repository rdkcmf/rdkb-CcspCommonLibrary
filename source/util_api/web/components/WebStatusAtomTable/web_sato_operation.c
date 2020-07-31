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

    module:	web_sato_operation.c

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Web Status Atom Table Object.

        *   WebSatoCreatePhraseTable
        *   WebSatoGetReasonPhrase

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


#include "web_sato_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebSatoCreatePhraseTable
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function creates the atom table for HTTP status phrase.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebSatoCreatePhraseTable
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_STATUS_ATOM_TABLE_OBJECT   pMyObject    = (PWEB_STATUS_ATOM_TABLE_OBJECT)hThisObject;

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_STATUS_CONTINUE,
            HTTP_PHRASE_CONTINUE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_STATUS_SWITCHING_PROTOCOL,
            HTTP_PHRASE_SWITCHING_PROTOCOL,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_STATUS_OK,
            HTTP_PHRASE_OK,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_STATUS_CREATED,
            HTTP_PHRASE_CREATED,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_STATUS_ACCEPTED,
            HTTP_PHRASE_ACCEPTED,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_STATUS_NON_AUTHORITATIVE,
            HTTP_PHRASE_NON_AUTHORITATIVE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_STATUS_NO_CONTENT,
            HTTP_PHRASE_NO_CONTENT,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_STATUS_RESET_CONTENT,
            HTTP_PHRASE_RESET_CONTENT,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_STATUS_PARTIAL_CONTENT,
            HTTP_PHRASE_PARTIAL_CONTENT,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_STATUS_MULTIPLE_CHOICES,
            HTTP_PHRASE_MULTIPLE_CHOICES,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_STATUS_MOVED_PERMANENTLY,
            HTTP_PHRASE_MOVED_PERMANENTLY,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_STATUS_FOUND,
            HTTP_PHRASE_FOUND,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_STATUS_SEE_OTHER,
            HTTP_PHRASE_SEE_OTHER,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_STATUS_NOT_MODIFIED,
            HTTP_PHRASE_NOT_MODIFIED,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_STATUS_USE_PROXY,
            HTTP_PHRASE_USE_PROXY,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_STATUS_TEMP_REDIRECT,
            HTTP_PHRASE_TEMP_REDIRECT,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_STATUS_BAD_REQUEST,
            HTTP_PHRASE_BAD_REQUEST,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_STATUS_UNAUTHORIZED,
            HTTP_PHRASE_UNAUTHORIZED,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_STATUS_PAYMENT_REQUIRED,
            HTTP_PHRASE_PAYMENT_REQUIRED,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_STATUS_FORBIDDEN,
            HTTP_PHRASE_FORBIDDEN,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_STATUS_NOT_FOUND,
            HTTP_PHRASE_NOT_FOUND,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_STATUS_METHOD_NOT_ALLOWED,
            HTTP_PHRASE_METHOD_NOT_ALLOWED,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_STATUS_NOT_ACCEPTABLE,
            HTTP_PHRASE_NOT_ACCEPTABLE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_STATUS_PROXY_AUTH_REQUIRED,
            HTTP_PHRASE_PROXY_AUTH_REQUIRED,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_STATUS_REQUEST_TIMEOUT,
            HTTP_PHRASE_REQUEST_TIMEOUT,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_STATUS_CONFLICT,
            HTTP_PHRASE_CONFLICT,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_STATUS_GONE,
            HTTP_PHRASE_GONE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_STATUS_LENGTH_REQUIRED,
            HTTP_PHRASE_LENGTH_REQUIRED,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_STATUS_PRECON_FAILED,
            HTTP_PHRASE_PRECON_FAILED,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_STATUS_ENTITY_TOO_LARGE,
            HTTP_PHRASE_ENTITY_TOO_LARGE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_STATUS_URI_TOO_LONG,
            HTTP_PHRASE_URI_TOO_LONG,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_STATUS_UNSUPPORTED_MEDIA,
            HTTP_PHRASE_UNSUPPORTED_MEDIA,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_STATUS_RANGE_NOT_SATISFIABLE,
            HTTP_PHRASE_RANGE_NOT_SATISFIABLE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_STATUS_EXPECTATION_FAILED,
            HTTP_PHRASE_EXPECTATION_FAILED,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_STATUS_INTERNAL_SERVER_ERROR,
            HTTP_PHRASE_INTERNAL_SERVER_ERROR,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_STATUS_NOT_IMPLEMENTED,
            HTTP_PHRASE_NOT_IMPLEMENTED,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_STATUS_BAD_GATEWAY,
            HTTP_PHRASE_BAD_GATEWAY,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_STATUS_SERVICE_UNAVAILABLE,
            HTTP_PHRASE_SERVICE_UNAVAILABLE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_STATUS_GATEWAY_TIMEOUT,
            HTTP_PHRASE_GATEWAY_TIMEOUT,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_STATUS_VERSION_NOT_SUPPORTED,
            HTTP_PHRASE_VERSION_NOT_SUPPORTED,
            (ANSC_HANDLE)NULL
        );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        WebSatoGetReasonPhrase
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulCode
            );

    description:

        This function is called to retrieve the reason phrase for
        the corresponding status code.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulCode
                Specifies the status code to be matched.

    return:     reason phrase.

**********************************************************************/

char*
WebSatoGetReasonPhrase
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCode
    )
{
    PWEB_STATUS_ATOM_TABLE_OBJECT   pMyObject       = (PWEB_STATUS_ATOM_TABLE_OBJECT)hThisObject;
    PANSC_ATOM_DESCRIPTOR           pAtomDescriptor = NULL;

    pAtomDescriptor =
        (PANSC_ATOM_DESCRIPTOR)pMyObject->GetAtomById
            (
                (ANSC_HANDLE)pMyObject,
                ulCode
            );

    if ( !pAtomDescriptor )
    {
        return  NULL;
    }

    return  pAtomDescriptor->AtomName;
}
