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

    module:	stun_sco_states.c

        For Simple Traversal of UDP Through NATs (STUN),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Stun Simple Client Object.

        *   StunScoGetStunBsmIf
        *   StunScoSetStunBsmIf
        *   StunScoGetClientAddr
        *   StunScoSetClientAddr
        *   StunScoGetClientPort
        *   StunScoSetClientPort
        *   StunScoGetClientMode
        *   StunScoSetClientMode
        *   StunScoGetServerAddr
        *   StunScoSetServerAddr
        *   StunScoGetServerPort
        *   StunScoSetServerPort
        *   StunScoGetUsername
        *   StunScoSetUsername
        *   StunScoGetPassword
        *   StunScoSetPassword
        *   StunScoGetMinKeepAliveInterval
        *   StunScoSetMinKeepAliveInterval
        *   StunScoGetMaxKeepAliveInterval
        *   StunScoSetMaxKeepAliveInterval
        *   StunScoGetProperty
        *   StunScoSetProperty
        *   StunScoResetProperty
        *   StunScoReset

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/31/08    initial revision.

**********************************************************************/


#include "stun_sco_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        StunScoGetStunBsmIf
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
StunScoGetStunBsmIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject    = (PSTUN_SIMPLE_CLIENT_OBJECT  )hThisObject;

    return  pMyObject->hStunBsmIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        StunScoSetStunBsmIf
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
StunScoSetStunBsmIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject    = (PSTUN_SIMPLE_CLIENT_OBJECT  )hThisObject;

    pMyObject->hStunBsmIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        StunScoGetClientAddr
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
StunScoGetClientAddr
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject    = (PSTUN_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PSTUN_SIMPLE_CLIENT_PROPERTY    pProperty    = (PSTUN_SIMPLE_CLIENT_PROPERTY)&pMyObject->Property;

    return  pProperty->ClientAddr.Value;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        StunScoSetClientAddr
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulIp4Addr
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulIp4Addr
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
StunScoSetClientAddr
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIp4Addr
    )
{
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject    = (PSTUN_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PSTUN_SIMPLE_CLIENT_PROPERTY    pProperty    = (PSTUN_SIMPLE_CLIENT_PROPERTY)&pMyObject->Property;

    pProperty->ClientAddr.Value = ulIp4Addr;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        USHORT
        StunScoGetClientPort
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
StunScoGetClientPort
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject    = (PSTUN_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PSTUN_SIMPLE_CLIENT_PROPERTY    pProperty    = (PSTUN_SIMPLE_CLIENT_PROPERTY)&pMyObject->Property;

    return  pProperty->ClientPort;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        StunScoSetClientPort
            (
                ANSC_HANDLE                 hThisObject,
                USHORT                      usUdpPort
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       usUdpPort
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
StunScoSetClientPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usUdpPort
    )
{
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject    = (PSTUN_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PSTUN_SIMPLE_CLIENT_PROPERTY    pProperty    = (PSTUN_SIMPLE_CLIENT_PROPERTY)&pMyObject->Property;

    pProperty->ClientPort = usUdpPort;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        StunScoGetClientMode
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
StunScoGetClientMode
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject    = (PSTUN_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PSTUN_SIMPLE_CLIENT_PROPERTY    pProperty    = (PSTUN_SIMPLE_CLIENT_PROPERTY)&pMyObject->Property;

    return  pProperty->ClientMode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        StunScoSetClientMode
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulMode
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulMode
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
StunScoSetClientMode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    )
{
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject    = (PSTUN_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PSTUN_SIMPLE_CLIENT_PROPERTY    pProperty    = (PSTUN_SIMPLE_CLIENT_PROPERTY)&pMyObject->Property;

    pProperty->ClientMode = ulMode;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        StunScoGetServerAddr
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
StunScoGetServerAddr
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject    = (PSTUN_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PSTUN_SIMPLE_CLIENT_PROPERTY    pProperty    = (PSTUN_SIMPLE_CLIENT_PROPERTY)&pMyObject->Property;

    return  pProperty->ServerAddr.Value;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        StunScoSetServerAddr
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulIp4Addr
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulIp4Addr
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
StunScoSetServerAddr
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIp4Addr
    )
{
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject    = (PSTUN_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PSTUN_SIMPLE_CLIENT_PROPERTY    pProperty    = (PSTUN_SIMPLE_CLIENT_PROPERTY)&pMyObject->Property;

    pProperty->ServerAddr.Value = ulIp4Addr;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        USHORT
        StunScoGetServerPort
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
StunScoGetServerPort
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject    = (PSTUN_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PSTUN_SIMPLE_CLIENT_PROPERTY    pProperty    = (PSTUN_SIMPLE_CLIENT_PROPERTY)&pMyObject->Property;

    return  pProperty->ServerPort;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        StunScoSetServerPort
            (
                ANSC_HANDLE                 hThisObject,
                USHORT                      usUdpPort
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       usUdpPort
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
StunScoSetServerPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usUdpPort
    )
{
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject    = (PSTUN_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PSTUN_SIMPLE_CLIENT_PROPERTY    pProperty    = (PSTUN_SIMPLE_CLIENT_PROPERTY)&pMyObject->Property;

    pProperty->ServerPort = usUdpPort;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        StunScoGetUsername
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
StunScoGetUsername
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject    = (PSTUN_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PSTUN_SIMPLE_CLIENT_PROPERTY    pProperty    = (PSTUN_SIMPLE_CLIENT_PROPERTY)&pMyObject->Property;

    return  pProperty->Username;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        StunScoSetUsername
            (
                ANSC_HANDLE                 hThisObject,
                char*                       username
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       username
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
StunScoSetUsername
    (
        ANSC_HANDLE                 hThisObject,
        char*                       username
    )
{
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject    = (PSTUN_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PSTUN_SIMPLE_CLIENT_PROPERTY    pProperty    = (PSTUN_SIMPLE_CLIENT_PROPERTY)&pMyObject->Property;
    errno_t                         rc           = -1;

    AnscZeroMemory(pProperty->Username, STUN_MAX_USERNAME_SIZE);

    if ( username )
    {
        rc = strcpy_s(pProperty->Username, sizeof(pProperty->Username), username);
        ERR_CHK(rc);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        StunScoGetPassword
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
StunScoGetPassword
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject    = (PSTUN_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PSTUN_SIMPLE_CLIENT_PROPERTY    pProperty    = (PSTUN_SIMPLE_CLIENT_PROPERTY)&pMyObject->Property;

    return  pProperty->Password;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        StunScoSetPassword
            (
                ANSC_HANDLE                 hThisObject,
                char*                       password
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       password
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
StunScoSetPassword
    (
        ANSC_HANDLE                 hThisObject,
        char*                       password
    )
{
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject    = (PSTUN_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PSTUN_SIMPLE_CLIENT_PROPERTY    pProperty    = (PSTUN_SIMPLE_CLIENT_PROPERTY)&pMyObject->Property;
    errno_t                         rc           = -1;

    AnscZeroMemory(pProperty->Password, STUN_MAX_PASSWORD_SIZE);

    if ( password )
    {
        rc = strcpy_s(pProperty->Password, sizeof(pProperty->Password), password);
        ERR_CHK(rc);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        int
        StunScoGetMinKeepAliveInterval
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

int
StunScoGetMinKeepAliveInterval
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject    = (PSTUN_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PSTUN_SIMPLE_CLIENT_PROPERTY    pProperty    = (PSTUN_SIMPLE_CLIENT_PROPERTY)&pMyObject->Property;

    return  pProperty->MinKeepAliveInterval;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        StunScoSetMinKeepAliveInterval
            (
                ANSC_HANDLE                 hThisObject,
                int                         iSeconds
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                int                         iSeconds
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
StunScoSetMinKeepAliveInterval
    (
        ANSC_HANDLE                 hThisObject,
        int                         iSeconds
    )
{
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject    = (PSTUN_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PSTUN_SIMPLE_CLIENT_PROPERTY    pProperty    = (PSTUN_SIMPLE_CLIENT_PROPERTY)&pMyObject->Property;

    pProperty->MinKeepAliveInterval = iSeconds;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        int
        StunScoGetMaxKeepAliveInterval
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

int
StunScoGetMaxKeepAliveInterval
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject    = (PSTUN_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PSTUN_SIMPLE_CLIENT_PROPERTY    pProperty    = (PSTUN_SIMPLE_CLIENT_PROPERTY)&pMyObject->Property;

    return  pProperty->MaxKeepAliveInterval;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        StunScoSetMaxKeepAliveInterval
            (
                ANSC_HANDLE                 hThisObject,
                int                         iSeconds
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                int                         iSeconds
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
StunScoSetMaxKeepAliveInterval
    (
        ANSC_HANDLE                 hThisObject,
        int                         iSeconds
    )
{
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject    = (PSTUN_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PSTUN_SIMPLE_CLIENT_PROPERTY    pProperty    = (PSTUN_SIMPLE_CLIENT_PROPERTY)&pMyObject->Property;

    pProperty->MaxKeepAliveInterval = iSeconds;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        StunScoGetProperty
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hProperty
            );

    description:

        This function is called to retrieve object property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hProperty
                Specifies the property data structure to be filled.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
StunScoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject    = (PSTUN_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PSTUN_SIMPLE_CLIENT_PROPERTY    pProperty    = (PSTUN_SIMPLE_CLIENT_PROPERTY)&pMyObject->Property;

    *(PSTUN_SIMPLE_CLIENT_PROPERTY)hProperty = *pProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        StunScoSetProperty
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hProperty
            );

    description:

        This function is called to configure object property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hProperty
                Specifies the property data structure to be copied.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
StunScoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject    = (PSTUN_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PSTUN_SIMPLE_CLIENT_PROPERTY    pProperty    = (PSTUN_SIMPLE_CLIENT_PROPERTY)&pMyObject->Property;

    *pProperty = *(PSTUN_SIMPLE_CLIENT_PROPERTY)hProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        StunScoResetProperty
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to reset object property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
StunScoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject    = (PSTUN_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PSTUN_SIMPLE_CLIENT_PROPERTY    pProperty    = (PSTUN_SIMPLE_CLIENT_PROPERTY)&pMyObject->Property;

    AnscZeroMemory(pProperty, sizeof(STUN_SIMPLE_CLIENT_PROPERTY));

    pProperty->ClientAddr.Value     = 0;
    pProperty->ClientPort           = 0;
    pProperty->ServerAddr .Value    = 0;
    pProperty->ServerPort           = STUN_PORT;

    pProperty->MinKeepAliveInterval = STUN_MIN_KEEP_ALIVE_INTERVAL;
    pProperty->CurKeepAliveInterval = pProperty->MinKeepAliveInterval;
    pProperty->MaxKeepAliveInterval = -1;

    pProperty->ClientMode           = ANSC_SCUO_MODE_XSOCKET;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        StunScoReset
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
StunScoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject    = (PSTUN_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PSTUN_SIMPLE_CLIENT_PROPERTY    pProperty    = (PSTUN_SIMPLE_CLIENT_PROPERTY)&pMyObject->Property;

    StunInitBindingInfo((&pMyObject->BindingInfo ));
    StunInitBindingInfo((&pMyObject->BindingInfoA));
    AnscResetEvent     ( &pMyObject->SocketEventA );

    pProperty->CurKeepAliveInterval = pProperty->MinKeepAliveInterval;

    if ( pMyObject->LastSendMsg1 )
    {
        AnscFreeMemory(pMyObject->LastSendMsg1);

        pMyObject->LastSendMsg1 = NULL;
        pMyObject->LastMsgSize1 = 0;
    }

    pMyObject->ClientStage      = STUN_SCO_CLIENT_STAGE_None;
    pMyObject->ClientState      = STUN_SCO_CLIENT_STATE_Idle;
    pMyObject->SocketState1     = STUN_SCO_SOCKET_STATE_Idle;
    pMyObject->SocketStateA     = STUN_SCO_SOCKET_STATE_Idle;
    pMyObject->SocketStateB     = STUN_SCO_SOCKET_STATE_Idle;
    pMyObject->RetransInterval1 = STUN_MIN_RETRANSMISSION_INTERVAL;
    pMyObject->RetransTime1     = 0;
    pMyObject->LastSendMsg1     = NULL;
    pMyObject->LastMsgSize1     = 0;
    pMyObject->bMsgIntegrity1   = FALSE;
    pMyObject->bMsgIntegrityA   = FALSE;
    pMyObject->bMsgIntegrityB   = FALSE;

    return  ANSC_STATUS_SUCCESS;
}
