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

    module:	web_gso_states.c

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Web General Session Object.

        *   WebGsoGetHfpIf
        *   WebGsoSetHfpIf
        *   WebGsoGetActiveSocket
        *   WebGsoSetActiveSocket
        *   WebGsoGetSessionId
        *   WebGsoSetSessionId
        *   WebGsoGetClientAddr
        *   WebGsoSetClientAddr
        *   WebGsoGetClientPort
        *   WebGsoSetClientPort
        *   WebGsoGetLsmContext
        *   WebGsoSetLsmContext
        *   WebGsoGetLsmIdentifier
        *   WebGsoSetLsmIdentifier
        *   WebGsoGetLsmMaxAge
        *   WebGsoSetLsmMaxAge
        *   WebGsoGetLsmExpire
        *   WebGsoSetLsmExpire
        *   WebGsoReset

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/19/02    initial revision.

**********************************************************************/


#include "web_gso_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        WebGsoGetHfpIf
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

ANSC_HANDLE
WebGsoGetHfpIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_GENERAL_SESSION_OBJECT     pMyObject    = (PWEB_GENERAL_SESSION_OBJECT)hThisObject;

    return  pMyObject->hHfpIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebGsoSetHfpIf
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hInterface
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hInterface
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebGsoSetHfpIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PWEB_GENERAL_SESSION_OBJECT     pMyObject    = (PWEB_GENERAL_SESSION_OBJECT)hThisObject;

    pMyObject->hHfpIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        WebGsoGetActiveSocket
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

ANSC_HANDLE
WebGsoGetActiveSocket
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_GENERAL_SESSION_OBJECT     pMyObject    = (PWEB_GENERAL_SESSION_OBJECT)hThisObject;

    return  pMyObject->hActiveSocket;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebGsoSetActiveSocket
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebGsoSetActiveSocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    )
{
    PWEB_GENERAL_SESSION_OBJECT     pMyObject    = (PWEB_GENERAL_SESSION_OBJECT)hThisObject;

    pMyObject->hActiveSocket = hSocket;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        WebGsoGetSessionId
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
WebGsoGetSessionId
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_GENERAL_SESSION_OBJECT     pMyObject    = (PWEB_GENERAL_SESSION_OBJECT)hThisObject;

    return  pMyObject->SessionId;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebGsoSetSessionId
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulSessionId
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulSessionId
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebGsoSetSessionId
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSessionId
    )
{
    PWEB_GENERAL_SESSION_OBJECT     pMyObject    = (PWEB_GENERAL_SESSION_OBJECT)hThisObject;

    pMyObject->SessionId = ulSessionId;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        PUCHAR
        WebGsoGetClientAddr
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

PUCHAR
WebGsoGetClientAddr
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_GENERAL_SESSION_OBJECT     pMyObject    = (PWEB_GENERAL_SESSION_OBJECT)hThisObject;

    return  pMyObject->ClientAddr.Dot;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebGsoSetClientAddr
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      address
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      address
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebGsoSetClientAddr
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    )
{
    PWEB_GENERAL_SESSION_OBJECT     pMyObject    = (PWEB_GENERAL_SESSION_OBJECT)hThisObject;

    pMyObject->ClientAddr.Value = *(PULONG)address;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        USHORT
        WebGsoGetClientPort
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

USHORT
WebGsoGetClientPort
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_GENERAL_SESSION_OBJECT     pMyObject    = (PWEB_GENERAL_SESSION_OBJECT)hThisObject;

    return  pMyObject->ClientPort;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebGsoSetClientPort
            (
                ANSC_HANDLE                 hThisObject,
                USHORT                      port
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                USHORT                      port
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebGsoSetClientPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      port
    )
{
    PWEB_GENERAL_SESSION_OBJECT     pMyObject    = (PWEB_GENERAL_SESSION_OBJECT)hThisObject;

    pMyObject->ClientPort = port;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        WebGsoGetLsmContext
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

ANSC_HANDLE
WebGsoGetLsmContext
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_GENERAL_SESSION_OBJECT     pMyObject    = (PWEB_GENERAL_SESSION_OBJECT)hThisObject;

    return  pMyObject->hLsmContext;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebGsoSetLsmContext
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hContext
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hContext
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebGsoSetLsmContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    )
{
    PWEB_GENERAL_SESSION_OBJECT     pMyObject    = (PWEB_GENERAL_SESSION_OBJECT)hThisObject;

    pMyObject->hLsmContext = hContext;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        WebGsoGetLsmIdentifier
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
WebGsoGetLsmIdentifier
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_GENERAL_SESSION_OBJECT     pMyObject    = (PWEB_GENERAL_SESSION_OBJECT)hThisObject;

    return  pMyObject->LsmIdentifier;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebGsoSetLsmIdentifier
            (
                ANSC_HANDLE                 hThisObject,
                char*                       identifier
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       identifier
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebGsoSetLsmIdentifier
    (
        ANSC_HANDLE                 hThisObject,
        char*                       identifier
    )
{
    PWEB_GENERAL_SESSION_OBJECT     pMyObject    = (PWEB_GENERAL_SESSION_OBJECT)hThisObject;

    /*
    AnscZeroMemory(pMyObject->LsmIdentifier, HTTP_MAX_COOKIE_VALUE_SIZE);
    AnscCopyString(pMyObject->LsmIdentifier, identifier                );
    */

    if ( pMyObject->LsmIdentifier )
    {
        AnscFreeMemory(pMyObject->LsmIdentifier);
    }

    pMyObject->LsmIdentifier = identifier ? AnscCloneString(identifier) : NULL;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        WebGsoGetLsmMaxAge
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
WebGsoGetLsmMaxAge
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_GENERAL_SESSION_OBJECT     pMyObject    = (PWEB_GENERAL_SESSION_OBJECT)hThisObject;

    return  pMyObject->LsmMaxAge;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebGsoSetLsmMaxAge
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulSeconds
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulSeconds
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebGsoSetLsmMaxAge
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSeconds
    )
{
    PWEB_GENERAL_SESSION_OBJECT     pMyObject    = (PWEB_GENERAL_SESSION_OBJECT)hThisObject;

    pMyObject->LsmMaxAge = ulSeconds;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        WebGsoGetLsmExpire
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
WebGsoGetLsmExpire
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_GENERAL_SESSION_OBJECT     pMyObject    = (PWEB_GENERAL_SESSION_OBJECT)hThisObject;

    return  pMyObject->LsmExpire;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebGsoSetLsmExpire
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulSeconds
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulSeconds
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebGsoSetLsmExpire
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSeconds
    )
{
    PWEB_GENERAL_SESSION_OBJECT     pMyObject    = (PWEB_GENERAL_SESSION_OBJECT)hThisObject;

    pMyObject->LsmExpire = ulSeconds;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        WebGsoGetMaxLifespan
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
WebGsoGetMaxLifespan
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_GENERAL_SESSION_OBJECT     pMyObject    = (PWEB_GENERAL_SESSION_OBJECT)hThisObject;

    return  pMyObject->MaxLifespan;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebGsoSetLsmExpire
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulSeconds
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulSeconds
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebGsoSetMaxLifespan
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSeconds
    )
{
    PWEB_GENERAL_SESSION_OBJECT     pMyObject    = (PWEB_GENERAL_SESSION_OBJECT)hThisObject;

    pMyObject->MaxLifespan = ulSeconds;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebGsoReset
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to reset object states.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebGsoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_GENERAL_SESSION_OBJECT     pMyObject    = (PWEB_GENERAL_SESSION_OBJECT)hThisObject;

    pMyObject->SessionId      = 0;
    pMyObject->HashIndex      = 0;
    pMyObject->TimeStampInSec = AnscGetTickInSecondsAbs();
    pMyObject->LastReqAtInSec = pMyObject->TimeStampInSec;
    pMyObject->LastRepAtInSec = pMyObject->TimeStampInSec;
    pMyObject->RequestCount   = 0;
    pMyObject->MaxLifespan    = 0;

    if ( pMyObject->LsmIdentifier )
    {
        AnscFreeMemory(pMyObject->LsmIdentifier);
        pMyObject->LsmIdentifier = NULL;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        WebGsoGetHfpIf
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

ANSC_HANDLE
WebGsoGetAuthSessInfo
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_GENERAL_SESSION_OBJECT     pMyObject    = (PWEB_GENERAL_SESSION_OBJECT)hThisObject;

    return &pMyObject->AuthSessInfo;
}


