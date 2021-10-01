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

    module:	stun_sco_recv.c

        For Simple Traversal of UDP Through NATs (STUN),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced message-receiving
        functions of the Stun Simple Client Object.

        *   StunScoRecvBindingResponse1
        *   StunScoRecvBindingResponseA
        *   StunScoRecvBindingResponseB
        *   StunScoRecvBindingErrorResponse1
        *   StunScoRecvBindingErrorResponseA
        *   StunScoRecvBindingErrorResponseB
        *   StunScoVerifyMsgIntegrity

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
#include "ansc_crypto_external_api.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        StunScoRecvBindingResponse1
            (
                ANSC_HANDLE                 hThisObject,
                void*                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to process an incoming message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Specifies the incoming packet to be processed.

                ULONG                       ulSize
                Specifies the size of the incoming packet.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
StunScoRecvBindingResponse1
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    )
{
    ANSC_STATUS                     returnStatus             = ANSC_STATUS_SUCCESS;
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject                = (PSTUN_SIMPLE_CLIENT_OBJECT    )hThisObject;
    PSTUN_SIMPLE_CLIENT_PROPERTY    pProperty                = (PSTUN_SIMPLE_CLIENT_PROPERTY  )&pMyObject->Property;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pStageTimerObj           = (PANSC_TIMER_DESCRIPTOR_OBJECT )pMyObject->hStageTimerObj;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pRetryTimerObj           = (PANSC_TIMER_DESCRIPTOR_OBJECT )pMyObject->hRetryTimerObj;
    PSTUN_BSM_INTERFACE             pStunBsmIf               = (PSTUN_BSM_INTERFACE           )pMyObject->hStunBsmIf;
    PSTUN_HEADER                    pStunHeader              = (PSTUN_HEADER                  )buffer;
    PSTUN_ATTRIBUTE                 pStunAttribute           = (PSTUN_ATTRIBUTE               )NULL;
    PSTUN_ATTRIB_MAPPED_ADDRESS     pStunAttribMappedAddress = (PSTUN_ATTRIB_MAPPED_ADDRESS   )NULL;
    BOOL                            bBindingChanged          = FALSE;
    ANSC_IPV4_ADDRESS               MappedIpAddress          ;
    USHORT                          usMappedPort             = 0;
    BOOL                            bXorMappedAddress        = FALSE;

    AnscTrace("StunScoRecvBindingResponse1 --- received response, size = %lu.\n", ulSize);

    memset(&MappedIpAddress, 0, sizeof(ANSC_IPV4_ADDRESS));
    pRetryTimerObj->Stop((ANSC_HANDLE)pRetryTimerObj);

    switch ( pMyObject->ClientState )
    {
        case    STUN_SCO_CLIENT_STATE_Idle :

                break;

        case    STUN_SCO_CLIENT_STATE_Discovering :
        case    STUN_SCO_CLIENT_STATE_Maintaining :
        case    STUN_SCO_CLIENT_STATE_Notifying   :

                if ( pMyObject->SocketState1 != STUN_SCO_SOCKET_STATE_Requesting )
                {
                    break;
                }
                else if ( !AnscEqualMemory(pMyObject->TransID1, pStunHeader->TransactionID, 16) )
                {
                    break;
                }
                else if ( pMyObject->bMsgIntegrity1 &&
                         !pMyObject->VerifyMsgIntegrity
                            (
                                (ANSC_HANDLE)pMyObject,
                                buffer,
                                ulSize
                            ) )
                {
                    break;
                }

                AnscStunGetAttributeByType(pStunHeader, STUN_ATTRIB_TYPE_XOR_MAPPED_ADDRESS, pStunAttribute);
                if ( !pStunAttribute )
                {
                    AnscStunGetAttributeByType(pStunHeader, STUN_ATTRIB_TYPE_MAPPED_ADDRESS, pStunAttribute);
                }
                else
                {
                    bXorMappedAddress = TRUE;
                }

                if ( !pStunAttribute )
                {
                    break;
                }
                else
                {
                    pStunAttribMappedAddress = (PSTUN_ATTRIB_MAPPED_ADDRESS)pStunAttribute;

                    MappedIpAddress.Value    = AnscStunAttribMaGetAddressValue(pStunAttribMappedAddress);
                    usMappedPort             = AnscStunAttribMaGetPort        (pStunAttribMappedAddress);

                    if ( bXorMappedAddress )
                    {
                        ULONG                   ulMask = AnscReadUlong (pMyObject->TransID1);
                        USHORT                  usMask = AnscUshortFromNToH(AnscReadUshort(pMyObject->TransID1));

                        MappedIpAddress.Value ^= ulMask;
                        usMappedPort          ^= usMask;
                    }
                }

                if ( (pMyObject->BindingInfo.Ip4Addr.Value != MappedIpAddress.Value) ||
                     (pMyObject->BindingInfo.UdpPort       != usMappedPort) )
                {
                    bBindingChanged = TRUE;

                    pMyObject->BindingInfo.Ip4Addr.Value = MappedIpAddress.Value;
                    pMyObject->BindingInfo.UdpPort       = usMappedPort;

                    if ( (pMyObject->BindingInfo.Ip4Addr.Value != pProperty->ClientAddr.Value) ||
                         (pMyObject->BindingInfo.UdpPort       != pProperty->ClientPort      ) )
                    {
                        pMyObject->BindingInfo.bNatDetected = TRUE;
                    }

                    if ( pStunBsmIf )
                    {
                        returnStatus =
                            pStunBsmIf->Notify
                                (
                                    pStunBsmIf->hOwnerContext,
                                    STUN_BSM_EVENT_bindingChanged,
                                    (ANSC_HANDLE)&pMyObject->BindingInfo
                                );
                    }
                }

                /*
                 * Reset the state information.
                 */
                pMyObject->ClientState      = STUN_SCO_CLIENT_STATE_Idle;
                pMyObject->SocketState1     = STUN_SCO_SOCKET_STATE_Idle;
                pMyObject->RetransInterval1 = STUN_MIN_RETRANSMISSION_INTERVAL;
                pMyObject->RetransTime1     = 0;
                pMyObject->bMsgIntegrity1   = FALSE;

                if ( pMyObject->LastSendMsg1 )
                {
                    AnscFreeMemory(pMyObject->LastSendMsg1);

                    pMyObject->LastSendMsg1 = NULL;
                    pMyObject->LastMsgSize1 = 0;
                }

                /*
                 * Whenever the CPE detects a change to the NAT binding (as well as the first time
                 * the CPE determines the binding), it MUST immediately send a Binding Request
                 * message from the primary source port (the port on which the CPE is listening for
                 * UDP Connection Request messages) that includes the BINDING-CHANGE attribute.
                 */
                #ifdef  _DSLH_STUN_
                {
                    if ( bBindingChanged )
                    {
                        returnStatus = pMyObject->NotifyBindingChange((ANSC_HANDLE)pMyObject);
                    }
                }
                #endif

                /*
                 * If the current stage is 'discovering', we need to enter the next stage...
                 */
                if ( pMyObject->ClientStage == STUN_SCO_CLIENT_STAGE_Discovering )
                {
                    pStageTimerObj->Stop       ((ANSC_HANDLE)pStageTimerObj);
                    pStageTimerObj->SetInterval((ANSC_HANDLE)pStageTimerObj, 0);
                    pStageTimerObj->Start      ((ANSC_HANDLE)pStageTimerObj);
                }

                break;

        default :

                break;
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        StunScoRecvBindingResponseA
            (
                ANSC_HANDLE                 hThisObject,
                void*                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to process an incoming message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Specifies the incoming packet to be processed.

                ULONG                       ulSize
                Specifies the size of the incoming packet.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
StunScoRecvBindingResponseA
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    )
{
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject                = (PSTUN_SIMPLE_CLIENT_OBJECT    )hThisObject;
    PSTUN_HEADER                    pStunHeader              = (PSTUN_HEADER                  )buffer;
    PSTUN_ATTRIBUTE                 pStunAttribute           = (PSTUN_ATTRIBUTE               )NULL;
    PSTUN_ATTRIB_MAPPED_ADDRESS     pStunAttribMappedAddress = (PSTUN_ATTRIB_MAPPED_ADDRESS   )NULL;
    ANSC_IPV4_ADDRESS               MappedIpAddress          ;
    USHORT                          usMappedPort             = 0;
    BOOL                            bXorMappedAddress        = FALSE;

    AnscTrace("StunScoRecvBindingResponseA --- received response, size = %lu.\n", ulSize);

    memset(&MappedIpAddress, 0, sizeof(ANSC_IPV4_ADDRESS));

    if ( pMyObject->SocketStateA != STUN_SCO_SOCKET_STATE_Requesting &&
         pMyObject->SocketStateB != STUN_SCO_SOCKET_STATE_Requesting )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else if ( !AnscEqualMemory(pMyObject->TransIDA, pStunHeader->TransactionID, 16) )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else if ( pMyObject->bMsgIntegrityA &&
             !pMyObject->VerifyMsgIntegrity
                (
                    (ANSC_HANDLE)pMyObject,
                    buffer,
                    ulSize
                ) )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    AnscStunGetAttributeByType(pStunHeader, STUN_ATTRIB_TYPE_XOR_MAPPED_ADDRESS, pStunAttribute);
    if ( !pStunAttribute )
    {
        AnscStunGetAttributeByType(pStunHeader, STUN_ATTRIB_TYPE_MAPPED_ADDRESS, pStunAttribute);
    }
    else
    {
        bXorMappedAddress = TRUE;
    }

    if ( !pStunAttribute )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pStunAttribMappedAddress = (PSTUN_ATTRIB_MAPPED_ADDRESS)pStunAttribute;

        MappedIpAddress.Value    = AnscStunAttribMaGetAddressValue(pStunAttribMappedAddress);
        usMappedPort             = AnscStunAttribMaGetPort        (pStunAttribMappedAddress);

        if ( bXorMappedAddress )
        {
            ULONG                   ulMask = AnscReadUlong (pMyObject->TransIDA);
            USHORT                  usMask = AnscUshortFromNToH(AnscReadUshort(pMyObject->TransIDA));

            MappedIpAddress.Value ^= ulMask;
            usMappedPort          ^= usMask;
        }
    }

    if ( (pMyObject->BindingInfoA.Ip4Addr.Value != MappedIpAddress.Value ) ||
         (pMyObject->BindingInfoA.UdpPort       != usMappedPort) )
    {
        pMyObject->BindingInfoA.Ip4Addr.Value = MappedIpAddress.Value;
        pMyObject->BindingInfoA.UdpPort       = usMappedPort;
        pMyObject->BindingInfoA.bNatDetected  = TRUE;       /* cannot be determined since the request can be sent from either socket A or B */

        AnscTrace
            (
                "StunScoRecvBindingResponseA --- BindingInfoA changed, ip = 0x%.8x, port = %u.\n",
                pMyObject->BindingInfoA.Ip4Addr.Value,
                pMyObject->BindingInfoA.UdpPort
            );
    }

    /*
     * Reset the state information.
     */
    pMyObject->SocketStateA   = STUN_SCO_SOCKET_STATE_Idle;
    pMyObject->bMsgIntegrityA = FALSE;

    AnscSetEvent(&pMyObject->SocketEventA);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        StunScoRecvBindingResponseB
            (
                ANSC_HANDLE                 hThisObject,
                void*                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to process an incoming message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Specifies the incoming packet to be processed.

                ULONG                       ulSize
                Specifies the size of the incoming packet.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
StunScoRecvBindingResponseB
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(buffer);
    AnscTrace("StunScoRecvBindingResponseB --- received response, size = %lu.\n", ulSize);

    /*
     * We shouldn't receive anything on socket B...
     */

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        StunScoRecvBindingErrorResponse1
            (
                ANSC_HANDLE                 hThisObject,
                void*                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to process an incoming message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Specifies the incoming packet to be processed.

                ULONG                       ulSize
                Specifies the size of the incoming packet.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
StunScoRecvBindingErrorResponse1
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    )
{
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject            = (PSTUN_SIMPLE_CLIENT_OBJECT    )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pRetryTimerObj       = (PANSC_TIMER_DESCRIPTOR_OBJECT )pMyObject->hRetryTimerObj;
    PSTUN_BSM_INTERFACE             pStunBsmIf           = (PSTUN_BSM_INTERFACE           )pMyObject->hStunBsmIf;
    PSTUN_HEADER                    pStunHeader          = (PSTUN_HEADER                  )buffer;
    PSTUN_ATTRIBUTE                 pStunAttribute       = (PSTUN_ATTRIBUTE               )NULL;
    PSTUN_ATTRIB_ERROR_CODE         pStunAttribErrorCode = (PSTUN_ATTRIB_ERROR_CODE       )NULL;

    AnscTrace("StunScoRecvBindingErrorResponse1 --- received response, size = %lu.\n", ulSize);

    pRetryTimerObj->Stop((ANSC_HANDLE)pRetryTimerObj);

    switch ( pMyObject->ClientState )
    {
        case    STUN_SCO_CLIENT_STATE_Idle :

                break;

        case    STUN_SCO_CLIENT_STATE_Discovering :
        case    STUN_SCO_CLIENT_STATE_Maintaining :
        case    STUN_SCO_CLIENT_STATE_Notifying   :

                if ( pMyObject->SocketState1 != STUN_SCO_SOCKET_STATE_Requesting )
                {
                    break;
                }
                else if ( !AnscEqualMemory(pMyObject->TransID1, pStunHeader->TransactionID, 16) )
                {
                    break;
                }

                AnscStunGetAttributeByType(pStunHeader, STUN_ATTRIB_TYPE_ERROR_CODE, pStunAttribute);

                if ( !pStunAttribute )
                {
                    break;
                }
                else
                {
                    pStunAttribErrorCode = (PSTUN_ATTRIB_ERROR_CODE)pStunAttribute;
                }

                if ( AnscStunAttribEcGetErrorCode(pStunAttribErrorCode) == STUN_ERROR_CODE_Unauthorized )
                {
                    pMyObject->bMsgIntegrity1 = TRUE;

                    if ( pMyObject->ClientState == STUN_SCO_CLIENT_STATE_Discovering )
                    {
                        returnStatus = pMyObject->DiscoverBinding((ANSC_HANDLE)pMyObject);
                    }
                    else if ( pMyObject->ClientState == STUN_SCO_CLIENT_STATE_Maintaining )
                    {
                        returnStatus = pMyObject->MaintainBinding((ANSC_HANDLE)pMyObject);
                    }
                    else
                    {
                        returnStatus = pMyObject->NotifyBindingChange((ANSC_HANDLE)pMyObject);
                    }

                    break;
                }
                else
                {
                    if ( pStunBsmIf )
                    {
                        returnStatus =
                            pStunBsmIf->Notify
                                (
                                    pStunBsmIf->hOwnerContext,
                                    STUN_BSM_EVENT_errorReturned,
                                    (ANSC_HANDLE)pStunAttribErrorCode
                                );
                    }
                }

                /*
                 * Reset the state information.
                 */
                pMyObject->ClientState      = STUN_SCO_CLIENT_STATE_Idle;
                pMyObject->SocketState1     = STUN_SCO_SOCKET_STATE_Idle;
                pMyObject->RetransInterval1 = STUN_MIN_RETRANSMISSION_INTERVAL;
                pMyObject->RetransTime1     = 0;
                pMyObject->bMsgIntegrity1   = FALSE;

                if ( pMyObject->LastSendMsg1 )
                {
                    AnscFreeMemory(pMyObject->LastSendMsg1);

                    pMyObject->LastSendMsg1 = NULL;
                    pMyObject->LastMsgSize1 = 0;
                }

                break;

        default :

                break;
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        StunScoRecvBindingErrorResponseA
            (
                ANSC_HANDLE                 hThisObject,
                void*                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to process an incoming message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Specifies the incoming packet to be processed.

                ULONG                       ulSize
                Specifies the size of the incoming packet.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
StunScoRecvBindingErrorResponseA
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    )
{
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject            = (PSTUN_SIMPLE_CLIENT_OBJECT    )hThisObject;
    PSTUN_HEADER                    pStunHeader          = (PSTUN_HEADER                  )buffer;
    PSTUN_ATTRIBUTE                 pStunAttribute       = (PSTUN_ATTRIBUTE               )NULL;
    PSTUN_ATTRIB_ERROR_CODE         pStunAttribErrorCode = (PSTUN_ATTRIB_ERROR_CODE       )NULL;

    AnscTrace("StunScoRecvBindingErrorResponseA --- received response, size = %lu.\n", ulSize);

    if ( pMyObject->SocketStateA != STUN_SCO_SOCKET_STATE_Requesting )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else if ( !AnscEqualMemory(pMyObject->TransIDA, pStunHeader->TransactionID, 16) )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    AnscStunGetAttributeByType(pStunHeader, STUN_ATTRIB_TYPE_ERROR_CODE, pStunAttribute);

    if ( !pStunAttribute )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pStunAttribErrorCode = (PSTUN_ATTRIB_ERROR_CODE)pStunAttribute;
    }

    if ( AnscStunAttribEcGetErrorCode(pStunAttribErrorCode) == STUN_ERROR_CODE_Unauthorized )
    {
        pMyObject->bMsgIntegrityA = TRUE;

        returnStatus = pMyObject->DiscoverBindingTimeoutA((ANSC_HANDLE)pMyObject);

        return  returnStatus;
    }

    /*
     * Reset the state information.
     */
    pMyObject->SocketStateA   = STUN_SCO_SOCKET_STATE_Idle;
    pMyObject->bMsgIntegrityA = FALSE;

    AnscSetEvent(&pMyObject->SocketEventA);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        StunScoRecvBindingErrorResponseB
            (
                ANSC_HANDLE                 hThisObject,
                void*                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to process an incoming message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Specifies the incoming packet to be processed.

                ULONG                       ulSize
                Specifies the size of the incoming packet.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
StunScoRecvBindingErrorResponseB
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    )
{
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject            = (PSTUN_SIMPLE_CLIENT_OBJECT    )hThisObject;
    PSTUN_HEADER                    pStunHeader          = (PSTUN_HEADER                  )buffer;
    PSTUN_ATTRIBUTE                 pStunAttribute       = (PSTUN_ATTRIBUTE               )NULL;
    PSTUN_ATTRIB_ERROR_CODE         pStunAttribErrorCode = (PSTUN_ATTRIB_ERROR_CODE       )NULL;

    AnscTrace("StunScoRecvBindingErrorResponseB --- received response, size = %lu.\n", ulSize);

    if ( pMyObject->SocketStateB != STUN_SCO_SOCKET_STATE_Requesting )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else if ( !AnscEqualMemory(pMyObject->TransIDB, pStunHeader->TransactionID, 16) )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    AnscStunGetAttributeByType(pStunHeader, STUN_ATTRIB_TYPE_ERROR_CODE, pStunAttribute);

    if ( !pStunAttribute )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pStunAttribErrorCode = (PSTUN_ATTRIB_ERROR_CODE)pStunAttribute;
    }

    if ( AnscStunAttribEcGetErrorCode(pStunAttribErrorCode) == STUN_ERROR_CODE_Unauthorized )
    {
        pMyObject->bMsgIntegrityB = TRUE;

        returnStatus = pMyObject->DiscoverBindingTimeoutB((ANSC_HANDLE)pMyObject);

        return  returnStatus;
    }

    /*
     * Reset the state information.
     */
    pMyObject->SocketStateB   = STUN_SCO_SOCKET_STATE_Idle;
    pMyObject->bMsgIntegrityB = FALSE;

    AnscSetEvent(&pMyObject->SocketEventA);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        StunScoVerifyMsgIntegrity
            (
                ANSC_HANDLE                 hThisObject,
                void*                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to verify the integrity of an incoming
        message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Specifies the incoming packet to be processed.

                ULONG                       ulSize
                Specifies the size of the incoming packet.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
StunScoVerifyMsgIntegrity
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(ulSize);
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject               = (PSTUN_SIMPLE_CLIENT_OBJECT  )hThisObject;
    PSTUN_SIMPLE_CLIENT_PROPERTY    pProperty               = (PSTUN_SIMPLE_CLIENT_PROPERTY)&pMyObject->Property;
    PSTUN_HEADER                    pStunHeader             = (PSTUN_HEADER                )buffer;
    PSTUN_ATTRIBUTE                 pStunAttribute          = (PSTUN_ATTRIBUTE             )NULL;
    PSTUN_ATTRIB_MSG_INTEGRITY      pStunAttribMsgIntegrity = (PSTUN_ATTRIB_MSG_INTEGRITY  )NULL;
    ANSC_CRYPTO_HASH                crypto_hash;
    ANSC_CRYPTO_KEY                 crypto_key;

    AnscStunGetAttributeByType(pStunHeader, STUN_ATTRIB_TYPE_MSG_INTEGRITY, pStunAttribute);

    if ( !pStunAttribMsgIntegrity )
    {
        return  FALSE;
    }
    else
    {
        pStunAttribMsgIntegrity = (PSTUN_ATTRIB_MSG_INTEGRITY)pStunAttribute;
    }

    if ( AnscStunAttribMiGetLength(pStunAttribMsgIntegrity) != 20 )
    {
        return  FALSE;
    }
    else
    {
        crypto_key.KeyNumber   = 1;
        crypto_key.RoundNumber = 1;
        crypto_key.Length      = AnscSizeOfString(pProperty->Password);

        AnscCopyMemory
            (
                crypto_key.Value[0],
                pProperty->Password,
                AnscSizeOfString(pProperty->Password)
            );
    }

    AnscCryptoHmacSha1Digest
        (
            (void*)pStunHeader,
            (ULONG)((ULONG)pStunAttribMsgIntegrity - (ULONG)pStunHeader),
            &crypto_hash,
            &crypto_key
        );

    if ( !AnscEqualMemory
            (
                crypto_hash.Value,
                pStunAttribMsgIntegrity->Value,
                20
            ) )
    {
        return  FALSE;
    }

    return  TRUE;
}
