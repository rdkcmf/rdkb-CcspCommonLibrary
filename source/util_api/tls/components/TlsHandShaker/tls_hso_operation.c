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

    module:	tls_hso_operation.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the TLS Hand Shaker Object.

        *   TlsHsoEngage
        *   TlsHsoCancel
        *   TlsHsoSaveHandshakeMsg
        *   TlsHsoSaveFragHandshakeMsg
        *   TlsHsoStateTimerInvoke

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/06/03    initial revision.

**********************************************************************/


#include "tls_hso_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsHsoEngage
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to engage the object activity.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsHsoEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    TlsRcoEngage((ANSC_HANDLE)hThisObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsHsoCancel
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to cancel the object activity.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsHsoCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    TlsRcoCancel((ANSC_HANDLE)hThisObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsHsoSaveHandshakeMsg
            (
                ANSC_HANDLE                 hThisObject,
                void*                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to save the handshake message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                void*                       buffer
                Specifies the handshake message buffer to be saved.

                ULONG                       ulSize
                Specifies the size of the message to be saved.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsHsoSaveHandshakeMsg
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    )
{
    PTLS_HAND_SHAKER_OBJECT         pMyObject       = (PTLS_HAND_SHAKER_OBJECT)hThisObject;
    void*                           pNewMsgBuffer   = (void*                  )NULL;
    ULONG                           ulNewBufferSize = (ULONG                  )0;

    if ( !pMyObject->MsgBuffer )
    {
        pMyObject->MsgOffset     = 0;
        pMyObject->MsgBufferSize = TLS_HSO_DEF_MSG_BUFFER_SIZE;
        pMyObject->MsgBuffer     = AnscAllocateMemory(pMyObject->MsgBufferSize);

        if ( !pMyObject->MsgBuffer )
        {
            return  ANSC_STATUS_RESOURCES;
        }
    }

    if ( (pMyObject->MsgBufferSize - pMyObject->MsgOffset) < ulSize )
    {
        ulNewBufferSize = AnscGetMax2(pMyObject->MsgBufferSize * 2, pMyObject->MsgBufferSize + ulSize);
        pNewMsgBuffer   = AnscAllocateMemory(ulNewBufferSize);

        if ( !pNewMsgBuffer )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            AnscCopyMemory
                (
                    pNewMsgBuffer,
                    pMyObject->MsgBuffer,
                    pMyObject->MsgOffset
                );

            AnscFreeMemory(pMyObject->MsgBuffer);
        }

        pMyObject->MsgBuffer     = pNewMsgBuffer;
        pMyObject->MsgBufferSize = ulNewBufferSize;
    }

    AnscCopyMemory
        (
            (PVOID)((ULONG)pMyObject->MsgBuffer + pMyObject->MsgOffset),
            buffer,
            ulSize
        );

    pMyObject->MsgOffset += ulSize;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsHsoSaveFragHandshakeMsg
            (
                ANSC_HANDLE                 hThisObject,
                void*                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to save the fragmented handshake
        message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                void*                       buffer
                Specifies the handshake message buffer to be saved.

                ULONG                       ulSize
                Specifies the size of the message to be saved.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsHsoSaveFragHandshakeMsg
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    )
{
    PTLS_HAND_SHAKER_OBJECT         pMyObject       = (PTLS_HAND_SHAKER_OBJECT)hThisObject;
    void*                           pNewMsgBuffer   = (void*                  )NULL;
    ULONG                           ulNewBufferSize = (ULONG                  )0;

    if ( !pMyObject->FragMsgBuffer )
    {
        pMyObject->FragMsgOffset     = 0;
        pMyObject->FragMsgBufferSize = TLS_HSO_DEF_MSG_BUFFER_SIZE;
        pMyObject->FragMsgBuffer     = AnscAllocateMemory(pMyObject->FragMsgBufferSize);

        if ( !pMyObject->FragMsgBuffer )
        {
            return  ANSC_STATUS_RESOURCES;
        }
    }

    if ( (pMyObject->FragMsgBufferSize - pMyObject->FragMsgOffset) < ulSize )
    {
        ulNewBufferSize = AnscGetMax2(pMyObject->FragMsgBufferSize * 2, pMyObject->FragMsgBufferSize + ulSize);
        pNewMsgBuffer   = AnscAllocateMemory(ulNewBufferSize);

        if ( !pNewMsgBuffer )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            AnscCopyMemory
                (
                    pNewMsgBuffer,
                    pMyObject->FragMsgBuffer,
                    pMyObject->FragMsgOffset
                );

            AnscFreeMemory(pMyObject->FragMsgBuffer);
        }

        pMyObject->FragMsgBuffer     = pNewMsgBuffer;
        pMyObject->FragMsgBufferSize = ulNewBufferSize;
    }

    AnscCopyMemory
        (
            (PVOID)((ULONG)pMyObject->FragMsgBuffer + pMyObject->FragMsgOffset),
            buffer,
            ulSize
        );

    pMyObject->FragMsgOffset += ulSize;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsHsoStateTimerInvoke
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called whenever the state timer is fired.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsHsoStateTimerInvoke
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return  ANSC_STATUS_SUCCESS;
}
