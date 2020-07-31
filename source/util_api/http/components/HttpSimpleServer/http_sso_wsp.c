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

    module:	http_sso_wsp.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced interface functions
        of the Http Simple Server Object.

        *   HttpSsoWspAccept
        *   HttpSsoWspServe

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/08/02    initial revision.

**********************************************************************/


#include "http_sso_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        HttpSsoWspAccept
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hClientInfo
            );

    description:

        This function is called to control the server's behavior.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hClientInfo
                Specifies the parameter for WSP object to process.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
HttpSsoWspAccept
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hClientInfo
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hClientInfo);

    return  TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSsoWspServe
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hWebsSession,
                ANSC_HANDLE                 hWebsTrans,
                ANSC_HANDLE                 hMessage
            );

    description:

        This function is called to control the server's behavior.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hWebsSession
                Specifies the Web Session Object to be processed.

                ANSC_HANDLE                 hWebsTrans
                Specifies the Web Transaction Object to be processed.

                ANSC_HANDLE                 hMessage
                Specifies the Basic Message Object to be processed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSsoWspServe
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWebsSession,
        ANSC_HANDLE                 hWebsTrans,
        ANSC_HANDLE                 hMessage
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hWebsSession);
    UNREFERENCED_PARAMETER(hWebsTrans);
    UNREFERENCED_PARAMETER(hMessage);

    return  ANSC_STATUS_SUCCESS;
}
