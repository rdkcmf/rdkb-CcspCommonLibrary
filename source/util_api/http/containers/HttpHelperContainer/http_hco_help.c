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

    module:	http_hco_help.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-help functions
        of the Http Basic Message Object.

        *   HttpHcoGetHeaderId
        *   HttpHcoGetHeaderName
        *   HttpHcoGetMethodId
        *   HttpHcoGetMethodName
        *   HttpHcoGetCgiEnvId
        *   HttpHcoGetReasonPhrase
        *   HttpHcoGetExtMediaType
        *   HttpHcoGetExtSubType
        *   HttpHcoGetClockTime
        *   HttpHcoGetUtcTime

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/14/02    initial revision.

**********************************************************************/


#include "http_hco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpHcoGetHeaderId
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name
            );

    description:

        This function is called to retrieve a HTTP header field id.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the name of the header field.

    return:     header field id.

**********************************************************************/

ULONG
HttpHcoGetHeaderId
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    )
{
    PHTTP_HELPER_CONTAINER_OBJECT   pMyObject    = (PHTTP_HELPER_CONTAINER_OBJECT)hThisObject;
    PHTTP_ATO_HEADER_OBJECT         pAtoHeader   = (PHTTP_ATO_HEADER_OBJECT      )pMyObject->hAtoHeader;

    return  pAtoHeader->GetAtomCode((ANSC_HANDLE)pAtoHeader, name);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        HttpHcoGetHeaderName
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulHeaderId
            );

    description:

        This function is called to retrieve a HTTP header name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulHeaderId
                Specifies the header id of the header field.

    return:     header field name.

**********************************************************************/

char*
HttpHcoGetHeaderName
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulHeaderId
    )
{
    PHTTP_HELPER_CONTAINER_OBJECT   pMyObject    = (PHTTP_HELPER_CONTAINER_OBJECT)hThisObject;
    PHTTP_ATO_HEADER_OBJECT         pAtoHeader   = (PHTTP_ATO_HEADER_OBJECT      )pMyObject->hAtoHeader;

    return  pAtoHeader->GetAtomName((ANSC_HANDLE)pAtoHeader, ulHeaderId);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpHcoGetMethodId
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name
            );

    description:

        This function is called to retrieve a HTTP request method id.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the name of the request method.

    return:     request method id.

**********************************************************************/

ULONG
HttpHcoGetMethodId
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    )
{
    PHTTP_HELPER_CONTAINER_OBJECT   pMyObject    = (PHTTP_HELPER_CONTAINER_OBJECT)hThisObject;
    PHTTP_ATO_METHOD_OBJECT         pAtoMethod   = (PHTTP_ATO_METHOD_OBJECT      )pMyObject->hAtoMethod;

    return  pAtoMethod->GetAtomCode((ANSC_HANDLE)pAtoMethod, name);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        HttpHcoGetMethodName
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulMethodId
            );

    description:

        This function is called to retrieve a HTTP request method name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulMethodId
                Specifies the header id of the request method.

    return:     request method name.

**********************************************************************/

char*
HttpHcoGetMethodName
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMethodId
    )
{
    PHTTP_HELPER_CONTAINER_OBJECT   pMyObject    = (PHTTP_HELPER_CONTAINER_OBJECT)hThisObject;
    PHTTP_ATO_METHOD_OBJECT         pAtoMethod   = (PHTTP_ATO_METHOD_OBJECT      )pMyObject->hAtoMethod;

    return  pAtoMethod->GetAtomName((ANSC_HANDLE)pAtoMethod, ulMethodId);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpHcoGetCgiEnvId
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name
            );

    description:

        This function is called to retrieve a HTTP CGI environment id.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the name of the CGI environment variable.

    return:     CGI environment id.

**********************************************************************/

ULONG
HttpHcoGetCgiEnvId
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    )
{
    PHTTP_HELPER_CONTAINER_OBJECT   pMyObject    = (PHTTP_HELPER_CONTAINER_OBJECT)hThisObject;
    PHTTP_ATO_CGIENV_OBJECT         pAtoCgiEnv   = (PHTTP_ATO_CGIENV_OBJECT      )pMyObject->hAtoCgiEnv;

    return  pAtoCgiEnv->GetAtomCode((ANSC_HANDLE)pAtoCgiEnv, name);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        HttpHcoGetReasonPhrase
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       code
            );

    description:

        This function is called to retrieve a HTTP reason phrase.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       code
                Specifies the HTTP server response status code.

    return:     reason phrase.

**********************************************************************/

char*
HttpHcoGetReasonPhrase
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       code
    )
{
    PHTTP_HELPER_CONTAINER_OBJECT   pMyObject    = (PHTTP_HELPER_CONTAINER_OBJECT)hThisObject;
    PHTTP_ATO_STATUS_OBJECT         pAtoStatus   = (PHTTP_ATO_STATUS_OBJECT      )pMyObject->hAtoStatus;

    return  pAtoStatus->GetAtomName((ANSC_HANDLE)pAtoStatus, code);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpHcoGetExtMediaType
            (
                ANSC_HANDLE                 hThisObject,
                char*                       file_ext
            );

    description:

        This function is called to retrieve the media type corresponds
        to the specified file extension.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       file_ext
                Specifies the file extension to be examined.

    return:     media type.

**********************************************************************/

ULONG
HttpHcoGetExtMediaType
    (
        ANSC_HANDLE                 hThisObject,
        char*                       file_ext
    )
{
    PHTTP_HELPER_CONTAINER_OBJECT   pMyObject    = (PHTTP_HELPER_CONTAINER_OBJECT)hThisObject;
    PHTTP_ATO_FXCAT1_OBJECT         pAtoFxCat1   = (PHTTP_ATO_FXCAT1_OBJECT      )pMyObject->hAtoFxCat1;
    char                            lcase_str[32];

    AnscZeroMemory    (lcase_str, 32                                   );
    AnscStringFromUToL(file_ext,  lcase_str, AnscSizeOfString(file_ext));

    return  pAtoFxCat1->GetAtomCode((ANSC_HANDLE)pAtoFxCat1, lcase_str);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpHcoGetExtSubType
            (
                ANSC_HANDLE                 hThisObject,
                char*                       file_ext
            );

    description:

        This function is called to retrieve the sub type corresponds
        to the specified file extension.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       file_ext
                Specifies the file extension to be examined.

    return:     sub type.

**********************************************************************/

ULONG
HttpHcoGetExtSubType
    (
        ANSC_HANDLE                 hThisObject,
        char*                       file_ext
    )
{
    PHTTP_HELPER_CONTAINER_OBJECT   pMyObject    = (PHTTP_HELPER_CONTAINER_OBJECT)hThisObject;
    PHTTP_ATO_FXCAT2_OBJECT         pAtoFxCat2   = (PHTTP_ATO_FXCAT2_OBJECT      )pMyObject->hAtoFxCat2;
    char                            lcase_str[32];

    AnscZeroMemory    (lcase_str, 32                                   );
    AnscStringFromUToL(file_ext,  lcase_str, AnscSizeOfString(file_ext));

    return  pAtoFxCat2->GetAtomCode((ANSC_HANDLE)pAtoFxCat2, lcase_str);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpHcoGetClockTime
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hTime
            );

    description:

        This function is called to retrieve the system clock time.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hTime
                Specifies the universal time structure to filled out.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpHcoGetClockTime
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTime
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_HELPER_CONTAINER_OBJECT   pMyObject    = (PHTTP_HELPER_CONTAINER_OBJECT)hThisObject;
    PANSC_SMART_CLOCK_OBJECT        pSmartClock  = (PANSC_SMART_CLOCK_OBJECT     )pMyObject->hSmartClock;

    returnStatus =
        pSmartClock->GetClockTime
            (
                (ANSC_HANDLE)pSmartClock,
                hTime
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpHcoGetUtcTime
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hTime
            );

    description:

        This function is called to retrieve the system clock time.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hTime
                Specifies the universal time structure to filled out.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpHcoGetUtcTime
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTime
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_HELPER_CONTAINER_OBJECT   pMyObject    = (PHTTP_HELPER_CONTAINER_OBJECT)hThisObject;
    PANSC_SMART_CLOCK_OBJECT        pSmartClock  = (PANSC_SMART_CLOCK_OBJECT     )pMyObject->hSmartClock;

    returnStatus =
        pSmartClock->GetUtcTime
            (
                (ANSC_HANDLE)pSmartClock,
                hTime
            );

    return  returnStatus;
}
