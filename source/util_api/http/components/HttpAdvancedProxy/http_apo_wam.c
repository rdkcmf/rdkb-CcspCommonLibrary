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

    module:	http_apo_wam.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced interface functions
        of the Http Advanced Proxy Object.

        *   HttpApoWamAccept
        *   HttpApoWamConnect
        *   HttpApoWamClose

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/04/02    initial revision.

**********************************************************************/


#include "http_apo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        HttpApoWamAccept
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hClientInfo
            );

    description:

        This function is called to control the proxy's behavior.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hClientInfo
                Specifies the parameter for WAM object to process.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
HttpApoWamAccept
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

        ULONG
        HttpApoWamConnect
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hServerInfo
            );

    description:

        This function is called to control the proxy's behavior.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hServerInfo
                Specifies the parameter for WAM object to process.

    return:     process mode.

**********************************************************************/

ULONG
HttpApoWamConnect
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hServerInfo
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hServerInfo);

    return  HTTP_WAM_PMODE_PASS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpApoWamClose
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSession
            );

    description:

        This function is called to control the proxy's behavior.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSession
                Specifies the parameter for WAM object to process.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpApoWamClose
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hSession);

    return  ANSC_STATUS_SUCCESS;
}
