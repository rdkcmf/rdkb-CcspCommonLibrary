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

    module:	http_spo_sbc.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced interface functions
        of the Http Simple Proxy Object.

        *   HttpSpoSbcRequestHeaders
        *   HttpSpoSbcRequestBody

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/26/02    initial revision.

**********************************************************************/


#include "http_spo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpSpoSbcRequestHeaders
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSession,
                ANSC_HANDLE                 hMessage
            );

    description:

        This function is called to control the proxy's behavior.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSession
                Specifies the parameter for SBC object to process.

                ANSC_HANDLE                 hMessage
                Specifies the parameter for SBC object to process.

    return:     process mode.

**********************************************************************/

ULONG
HttpSpoSbcRequestHeaders
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession,
        ANSC_HANDLE                 hMessage
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hSession);
    UNREFERENCED_PARAMETER(hMessage);

    return  HTTP_SBC_PMODE_RELAY2;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpSpoSbcRequestBody
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSession,
                ANSC_HANDLE                 hMessage
            );

    description:

        This function is called to control the proxy's behavior.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSession
                Specifies the parameter for SBC object to process.

                ANSC_HANDLE                 hMessage
                Specifies the parameter for SBC object to process.

    return:     process mode.

**********************************************************************/

ULONG
HttpSpoSbcRequestBody
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession,
        ANSC_HANDLE                 hMessage
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hSession);
    UNREFERENCED_PARAMETER(hMessage);

    return  HTTP_SBC_PMODE_RELAY2;
}
