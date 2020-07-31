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

    module:	stun_sco_action.c

        For Simple Traversal of UDP Through NATs (STUN),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced action functions
        of the Stun Simple Client Object.

        *   StunScoPlayRole1
        *   StunScoDiscoverBinding
        *   StunScoDiscoverBindingTimeoutA
        *   StunScoDiscoverBindingTimeoutB
        *   StunScoMaintainBinding
        *   StunScoNotifyBindingChange
        *   StunScoAsyncDiscoverTask

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
        StunScoPlayRole1
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to engage the STUN client activity in
        the simplest form.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
StunScoPlayRole1
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject         = (PSTUN_SIMPLE_CLIENT_OBJECT    )hThisObject;
    PSTUN_SIMPLE_CLIENT_PROPERTY    pProperty         = (PSTUN_SIMPLE_CLIENT_PROPERTY  )&pMyObject->Property;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pStageTimerObj    = (PANSC_TIMER_DESCRIPTOR_OBJECT )pMyObject->hStageTimerObj;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pRetryTimerObj    = (PANSC_TIMER_DESCRIPTOR_OBJECT )pMyObject->hRetryTimerObj;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pProbeTimerObj    = (PANSC_TIMER_DESCRIPTOR_OBJECT )pMyObject->hProbeTimerObj;

    AnscTrace("StunScoPlayRole1 --- called.\n");

    /*
     * To make sure we have a clean start, we reset all internal states and timers.
     */
    pMyObject->Reset((ANSC_HANDLE)pMyObject);

    pStageTimerObj->Stop((ANSC_HANDLE)pStageTimerObj);
    pRetryTimerObj->Stop((ANSC_HANDLE)pRetryTimerObj);
    pProbeTimerObj->Stop((ANSC_HANDLE)pProbeTimerObj);

    /*
     * We use a stage timer to execute the STUN protocol.
     */
    pStageTimerObj->SetInterval((ANSC_HANDLE)pStageTimerObj, pProperty->MinKeepAliveInterval * 1000);
    pStageTimerObj->Start      ((ANSC_HANDLE)pStageTimerObj);

    /*
     * There're many different scenarios how the STUN protocol can be used to accomplish useful
     * tasks. This is at the discretion of the client. For each scenario, we will need to implement
     * a role function. In its simplest form, STUN client MUST:
     *
     *      - Determine the public IP address and port for the NAT binding on
     *        which it is listening for incoming UDP connection.
     *      - Discover the NAT binding timeout, and send STUN Binding Request
     *        messages at a rate necessary to keep alive this binding.
     *      - Listen for UDP connection messages and act on these messages when
     *        they arrive.
     */
    pMyObject->ClientStage = STUN_SCO_CLIENT_STAGE_Discovering;

    pMyObject->DiscoverBinding((ANSC_HANDLE)pMyObject);

    AnscTrace("StunScoPlayRole1 --- returned.\n");

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        StunScoDiscoverBinding
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to initiate the discover of the NAT
        binding.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
StunScoDiscoverBinding
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus              = ANSC_STATUS_SUCCESS;
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject                 = (PSTUN_SIMPLE_CLIENT_OBJECT    )hThisObject;
    PSTUN_SIMPLE_CLIENT_PROPERTY    pProperty                 = (PSTUN_SIMPLE_CLIENT_PROPERTY  )&pMyObject->Property;
    PSTUN_HEADER                    pStunHeader               = (PSTUN_HEADER                  )NULL;
    PSTUN_ATTRIBUTE                 pStunAttribNext           = (PSTUN_ATTRIBUTE               )NULL;
    PSTUN_ATTRIB_USERNAME           pStunAttribUsername       = (PSTUN_ATTRIB_USERNAME         )NULL;
    PSTUN_ATTRIB_CONN_REQ_BINDING   pStunAttribConnReqBinding = (PSTUN_ATTRIB_CONN_REQ_BINDING )NULL;
    PSTUN_ATTRIB_MSG_INTEGRITY      pStunAttribMsgIntegrity   = (PSTUN_ATTRIB_MSG_INTEGRITY    )NULL;
    ULONG                           ulStunMsgLength           = (ULONG                         )0;
    ULONG                           ulUsernameBytes           = (ULONG                         )0;
    ANSC_CRYPTO_HASH                crypto_hash;
    ANSC_CRYPTO_KEY                 crypto_key;

    /*
     * For the purpose of binding discovery, the client sends a STUN Binding Request to a server,
     * without any flags set in the CHANGE-REQUEST attribute, and without the RESPONSE-ADDRESS
     * attribute. This causes the server to send the response back to the address and port that the
     * request came from.
     */
    pStunHeader = (PSTUN_HEADER)AnscAllocateMemory(STUN_MAX_MSG_SIZE);

    if ( !pStunHeader )
    {
        return  ANSC_STATUS_RESOURCES;
    }

    /*
     * The transaction ID is a 128 bit identifier. It also serves as salt to randomize the request
     * and the response. All responses carry the same identifier as the request they correspond to.
     */
    AnscWriteUlong(&pMyObject->TransID1[ 0], (ULONG)pMyObject           );
    AnscWriteUlong(&pMyObject->TransID1[ 4], AnscGetTickInMilliSeconds());
    AnscWriteUlong(&pMyObject->TransID1[ 8], pProperty->ClientAddr.Value);
    AnscWriteUlong(&pMyObject->TransID1[12], pProperty->ClientAddr.Value);

    AnscCryptoMd5Digest(pMyObject->TransID1, 16, &crypto_hash     );
    AnscCopyMemory        (pMyObject->TransID1, crypto_hash.Value, 16);

    AnscStunSetMessageType  (pStunHeader, STUN_MSG_TYPE_BindingRequest);
    AnscStunSetMessageLength(pStunHeader, 0                           );
    AnscStunSetTransactionID(pStunHeader, pMyObject->TransID1         );

    /*
     * The USERNAME attribute is used for message integrity. It serves as a means to identify the
     * shared secret used in the message integrity check. The USERNAME is always present in a
     * Shared Secret Response, along with the PASSWORD. It is optionally present in a Binding
     * Request when message integrity is used.
     *
     * The value of USERNAME is a variable length opaque value. Its length MUST be a multiple of 4
     * (measured in bytes) in order to guarantee alignment of attributes on word boundaries.
     */
    pStunAttribNext = (PSTUN_ATTRIBUTE)pStunHeader->Attributes;

    if ( AnscSizeOfString(pProperty->Username) > 0 )
    {
        pStunAttribUsername = (PSTUN_ATTRIB_USERNAME)pStunAttribNext;

        if ( AnscSizeOfString(pProperty->Username) % 4 != 0 )
        {
            ulUsernameBytes = (AnscSizeOfString(pProperty->Username) / 4 + 1) * 4;
        }
        else
        {
            ulUsernameBytes = AnscSizeOfString(pProperty->Username);
        }

        AnscStunAttribUnSetType  (pStunAttribUsername, STUN_ATTRIB_TYPE_USERNAME);
        AnscStunAttribUnSetLength(pStunAttribUsername, (USHORT)ulUsernameBytes  );
        AnscStunAttribUnSetValue (pStunAttribUsername, pProperty->Username      );

        pStunAttribNext  = AnscStunAttribGetNextAttrib(pStunAttribUsername);
        ulStunMsgLength += AnscStunAttribGetSize      (pStunAttribUsername);
    }

    /*
     * A CPE MUST include the CONNECTION-REQUEST-BINDING attribute in every Binding Request message
     * whose source address and port are the address and port on which it is listening for UDP
     * Connection Request messages. In all other Binding Request messages, the CPE MUST NOT include
     * this attribute.
     */
    #ifdef  _DSLH_STUN_
    {
        pStunAttribConnReqBinding = (PSTUN_ATTRIB_CONN_REQ_BINDING)pStunAttribNext;

        AnscStunAttribCrbSetAll(pStunAttribConnReqBinding);

        pStunAttribNext  = AnscStunAttribGetNextAttrib(pStunAttribConnReqBinding);
        ulStunMsgLength += AnscStunAttribGetSize      (pStunAttribConnReqBinding);
    }
    #endif

    if ( pMyObject->bMsgIntegrity1 )
    {
        pStunAttribMsgIntegrity = (PSTUN_ATTRIB_MSG_INTEGRITY)pStunAttribNext;

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

            AnscCryptoHmacSha1Digest
                (
                    (void*)pStunHeader,
                    (ULONG)((ULONG)pStunAttribMsgIntegrity - (ULONG)pStunHeader),
                    &crypto_hash,
                    &crypto_key
                );
        }

        AnscStunAttribMiSetType  (pStunAttribMsgIntegrity, STUN_ATTRIB_TYPE_MSG_INTEGRITY);
        AnscStunAttribMiSetLength(pStunAttribMsgIntegrity, 20                            );
        AnscStunAttribMiSetValue (pStunAttribMsgIntegrity, crypto_hash.Value             );

        pStunAttribNext  = AnscStunAttribGetNextAttrib(pStunAttribMsgIntegrity);
        ulStunMsgLength += AnscStunAttribGetSize      (pStunAttribMsgIntegrity);
    }

    if ( ulStunMsgLength > 0 )
    {
        AnscStunSetMessageLength(pStunHeader, (USHORT)ulStunMsgLength);
    }

    pMyObject->ClientState = STUN_SCO_CLIENT_STATE_Discovering;
    returnStatus           =
        pMyObject->SendBindingRequest1
            (
                (ANSC_HANDLE)pMyObject,
                pStunHeader,
                AnscStunGetMessageSize(pStunHeader)
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        StunScoDiscoverBindingTimeoutA
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to initiate the discover of the life
        time of the NAT binding.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
StunScoDiscoverBindingTimeoutA
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus            = ANSC_STATUS_SUCCESS;
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject               = (PSTUN_SIMPLE_CLIENT_OBJECT    )hThisObject;
    PSTUN_SIMPLE_CLIENT_PROPERTY    pProperty               = (PSTUN_SIMPLE_CLIENT_PROPERTY  )&pMyObject->Property;
    PSTUN_HEADER                    pStunHeader             = (PSTUN_HEADER                  )NULL;
    PSTUN_ATTRIBUTE                 pStunAttribNext         = (PSTUN_ATTRIBUTE               )NULL;
    PSTUN_ATTRIB_USERNAME           pStunAttribUsername     = (PSTUN_ATTRIB_USERNAME         )NULL;
    PSTUN_ATTRIB_MSG_INTEGRITY      pStunAttribMsgIntegrity = (PSTUN_ATTRIB_MSG_INTEGRITY    )NULL;
    ULONG                           ulStunMsgLength         = (ULONG                         )0;
    ULONG                           ulUsernameBytes         = (ULONG                         )0;
    ANSC_CRYPTO_HASH                crypto_hash;
    ANSC_CRYPTO_KEY                 crypto_key;

    /*
     * For the purpose of binding discovery, the client sends a STUN Binding Request to a server,
     * without any flags set in the CHANGE-REQUEST attribute, and without the RESPONSE-ADDRESS
     * attribute. This causes the server to send the response back to the address and port that the
     * request came from.
     */
    pStunHeader = (PSTUN_HEADER)AnscAllocateMemory(STUN_MAX_MSG_SIZE);

    if ( !pStunHeader )
    {
        return  ANSC_STATUS_RESOURCES;
    }

    /*
     * The transaction ID is a 128 bit identifier. It also serves as salt to randomize the request
     * and the response. All responses carry the same identifier as the request they correspond to.
     */
    AnscWriteUlong(&pMyObject->TransIDA[ 0], (ULONG)pMyObject           );
    AnscWriteUlong(&pMyObject->TransIDA[ 4], AnscGetTickInMilliSeconds());
    AnscWriteUlong(&pMyObject->TransIDA[ 8], pProperty->ClientAddr.Value);
    AnscWriteUlong(&pMyObject->TransIDA[12], pProperty->ClientAddr.Value);

    AnscCryptoMd5Digest(pMyObject->TransIDA, 16, &crypto_hash     );
    AnscCopyMemory        (pMyObject->TransIDA, crypto_hash.Value, 16);

    AnscStunSetMessageType  (pStunHeader, STUN_MSG_TYPE_BindingRequest);
    AnscStunSetMessageLength(pStunHeader, 0                           );
    AnscStunSetTransactionID(pStunHeader, pMyObject->TransIDA         );

    /*
     * The USERNAME attribute is used for message integrity. It serves as a means to identify the
     * shared secret used in the message integrity check. The USERNAME is always present in a
     * Shared Secret Response, along with the PASSWORD. It is optionally present in a Binding
     * Request when message integrity is used.
     *
     * The value of USERNAME is a variable length opaque value. Its length MUST be a multiple of 4
     * (measured in bytes) in order to guarantee alignment of attributes on word boundaries.
     */
    pStunAttribNext = (PSTUN_ATTRIBUTE)pStunHeader->Attributes;

    if ( AnscSizeOfString(pProperty->Username) > 0 )
    {
        pStunAttribUsername = (PSTUN_ATTRIB_USERNAME)pStunAttribNext;

        if ( AnscSizeOfString(pProperty->Username) % 4 != 0 )
        {
            ulUsernameBytes = (AnscSizeOfString(pProperty->Username) / 4 + 1) * 4;
        }
        else
        {
            ulUsernameBytes = AnscSizeOfString(pProperty->Username);
        }

        AnscStunAttribUnSetType  (pStunAttribUsername, STUN_ATTRIB_TYPE_USERNAME);
        AnscStunAttribUnSetLength(pStunAttribUsername, (USHORT)ulUsernameBytes  );
        AnscStunAttribUnSetValue (pStunAttribUsername, pProperty->Username      );

        pStunAttribNext  = AnscStunAttribGetNextAttrib(pStunAttribUsername);
        ulStunMsgLength += AnscStunAttribGetSize      (pStunAttribUsername);
    }

    if ( pMyObject->bMsgIntegrityA )
    {
        pStunAttribMsgIntegrity = (PSTUN_ATTRIB_MSG_INTEGRITY)pStunAttribNext;

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

            AnscCryptoHmacSha1Digest
                (
                    (void*)pStunHeader,
                    (ULONG)((ULONG)pStunAttribMsgIntegrity - (ULONG)pStunHeader),
                    &crypto_hash,
                    &crypto_key
                );
        }

        AnscStunAttribMiSetType  (pStunAttribMsgIntegrity, STUN_ATTRIB_TYPE_MSG_INTEGRITY);
        AnscStunAttribMiSetLength(pStunAttribMsgIntegrity, 20                            );
        AnscStunAttribMiSetValue (pStunAttribMsgIntegrity, crypto_hash.Value             );

        pStunAttribNext  = AnscStunAttribGetNextAttrib(pStunAttribMsgIntegrity);
        ulStunMsgLength += AnscStunAttribGetSize      (pStunAttribMsgIntegrity);
    }

    if ( ulStunMsgLength > 0 )
    {
        AnscStunSetMessageLength(pStunHeader, (USHORT)ulStunMsgLength);
    }

    returnStatus =
        pMyObject->SendBindingRequestA
            (
                (ANSC_HANDLE)pMyObject,
                pStunHeader,
                AnscStunGetMessageSize(pStunHeader)
            );

    AnscFreeMemory(pStunHeader);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        StunScoDiscoverBindingTimeoutB
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to initiate the discover of the life
        time of the NAT binding.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
StunScoDiscoverBindingTimeoutB
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus               = ANSC_STATUS_SUCCESS;
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject                  = (PSTUN_SIMPLE_CLIENT_OBJECT    )hThisObject;
    PSTUN_SIMPLE_CLIENT_PROPERTY    pProperty                  = (PSTUN_SIMPLE_CLIENT_PROPERTY  )&pMyObject->Property;
    PSTUN_HEADER                    pStunHeader                = (PSTUN_HEADER                  )NULL;
    PSTUN_ATTRIBUTE                 pStunAttribNext            = (PSTUN_ATTRIBUTE               )NULL;
    PSTUN_ATTRIB_USERNAME           pStunAttribUsername        = (PSTUN_ATTRIB_USERNAME         )NULL;
    PSTUN_ATTRIB_RESPONSE_ADDRESS   pStunAttribResponseAddress = (PSTUN_ATTRIB_RESPONSE_ADDRESS )NULL;
    PSTUN_ATTRIB_MSG_INTEGRITY      pStunAttribMsgIntegrity    = (PSTUN_ATTRIB_MSG_INTEGRITY    )NULL;
    ULONG                           ulStunMsgLength            = (ULONG                         )0;
    ULONG                           ulUsernameBytes            = (ULONG                         )0;
    ANSC_CRYPTO_HASH                crypto_hash;
    ANSC_CRYPTO_KEY                 crypto_key;

    /*
     * For the purpose of binding discovery, the client sends a STUN Binding Request to a server,
     * without any flags set in the CHANGE-REQUEST attribute, and without the RESPONSE-ADDRESS
     * attribute. This causes the server to send the response back to the address and port that the
     * request came from.
     */
    pStunHeader = (PSTUN_HEADER)AnscAllocateMemory(STUN_MAX_MSG_SIZE);

    if ( !pStunHeader )
    {
        return  ANSC_STATUS_RESOURCES;
    }

    /*
     * The transaction ID is a 128 bit identifier. It also serves as salt to randomize the request
     * and the response. All responses carry the same identifier as the request they correspond to.
     */
    AnscWriteUlong(&pMyObject->TransIDA[ 0], (ULONG)pMyObject           );
    AnscWriteUlong(&pMyObject->TransIDA[ 4], AnscGetTickInMilliSeconds());
    AnscWriteUlong(&pMyObject->TransIDA[ 8], pProperty->ClientAddr.Value);
    AnscWriteUlong(&pMyObject->TransIDA[12], pProperty->ClientAddr.Value);

    AnscCryptoMd5Digest(pMyObject->TransIDA, 16, &crypto_hash     );
    AnscCopyMemory        (pMyObject->TransIDA, crypto_hash.Value, 16);

    AnscStunSetMessageType  (pStunHeader, STUN_MSG_TYPE_BindingRequest);
    AnscStunSetMessageLength(pStunHeader, 0                           );
    AnscStunSetTransactionID(pStunHeader, pMyObject->TransIDA         );

    /*
     * The USERNAME attribute is used for message integrity. It serves as a means to identify the
     * shared secret used in the message integrity check. The USERNAME is always present in a
     * Shared Secret Response, along with the PASSWORD. It is optionally present in a Binding
     * Request when message integrity is used.
     *
     * The value of USERNAME is a variable length opaque value. Its length MUST be a multiple of 4
     * (measured in bytes) in order to guarantee alignment of attributes on word boundaries.
     */
    pStunAttribNext = (PSTUN_ATTRIBUTE)pStunHeader->Attributes;

    if ( AnscSizeOfString(pProperty->Username) > 0 )
    {
        pStunAttribUsername = (PSTUN_ATTRIB_USERNAME)pStunAttribNext;

        if ( AnscSizeOfString(pProperty->Username) % 4 != 0 )
        {
            ulUsernameBytes = (AnscSizeOfString(pProperty->Username) / 4 + 1) * 4;
        }
        else
        {
            ulUsernameBytes = AnscSizeOfString(pProperty->Username);
        }

        AnscStunAttribUnSetType  (pStunAttribUsername, STUN_ATTRIB_TYPE_USERNAME);
        AnscStunAttribUnSetLength(pStunAttribUsername, (USHORT)ulUsernameBytes  );
        AnscStunAttribUnSetValue (pStunAttribUsername, pProperty->Username      );

        pStunAttribNext  = AnscStunAttribGetNextAttrib(pStunAttribUsername);
        ulStunMsgLength += AnscStunAttribGetSize      (pStunAttribUsername);
    }

    /*
     * The RESPONSE-ADDRESS needs to be set to the mapped IP address and port number of the socket
     * A. This will create a new binding on the NAT, and cause the STUN server to send a Binding
     * Response that would match the old binding, if it still exists.
     */
    {
        pStunAttribResponseAddress = (PSTUN_ATTRIB_RESPONSE_ADDRESS)pStunAttribNext;

        AnscStunAttribRaSetType        (pStunAttribResponseAddress, STUN_ATTRIB_TYPE_RESPONSE_ADDRESS                            );
        AnscStunAttribRaSetLength      (pStunAttribResponseAddress, sizeof(STUN_ATTRIB_RESPONSE_ADDRESS) - sizeof(STUN_ATTRIBUTE));
        AnscStunAttribRaSetReserved    (pStunAttribResponseAddress, 0                                                            );
        AnscStunAttribRaSetFamily      (pStunAttribResponseAddress, 1                                                            );
        AnscStunAttribRaSetPort        (pStunAttribResponseAddress, pMyObject->BindingInfoA.UdpPort                              );
        AnscStunAttribRaSetAddressValue(pStunAttribResponseAddress, pMyObject->BindingInfoA.Ip4Addr.Value                        );

        pStunAttribNext  = AnscStunAttribGetNextAttrib(pStunAttribResponseAddress);
        ulStunMsgLength += AnscStunAttribGetSize      (pStunAttribResponseAddress);
    }

    if ( pMyObject->bMsgIntegrityB )
    {
        pStunAttribMsgIntegrity = (PSTUN_ATTRIB_MSG_INTEGRITY)pStunAttribNext;

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

            AnscCryptoHmacSha1Digest
                (
                    (void*)pStunHeader,
                    (ULONG)((ULONG)pStunAttribMsgIntegrity - (ULONG)pStunHeader),
                    &crypto_hash,
                    &crypto_key
                );
        }

        AnscStunAttribMiSetType  (pStunAttribMsgIntegrity, STUN_ATTRIB_TYPE_MSG_INTEGRITY);
        AnscStunAttribMiSetLength(pStunAttribMsgIntegrity, 20                            );
        AnscStunAttribMiSetValue (pStunAttribMsgIntegrity, crypto_hash.Value             );

        pStunAttribNext  = AnscStunAttribGetNextAttrib(pStunAttribMsgIntegrity);
        ulStunMsgLength += AnscStunAttribGetSize      (pStunAttribMsgIntegrity);
    }

    if ( ulStunMsgLength > 0 )
    {
        AnscStunSetMessageLength(pStunHeader, (USHORT)ulStunMsgLength);
    }

    /* reinitialize binding info before sending out this binding request */
    StunInitBindingInfo((&pMyObject->BindingInfoA));

    returnStatus =
        pMyObject->SendBindingRequestB
            (
                (ANSC_HANDLE)pMyObject,
                pStunHeader,
                AnscStunGetMessageSize(pStunHeader)
            );

    AnscFreeMemory(pStunHeader);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        StunScoMaintainBinding
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to maintain the NAT binding.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
StunScoMaintainBinding
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus              = ANSC_STATUS_SUCCESS;
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject                 = (PSTUN_SIMPLE_CLIENT_OBJECT    )hThisObject;
    PSTUN_SIMPLE_CLIENT_PROPERTY    pProperty                 = (PSTUN_SIMPLE_CLIENT_PROPERTY  )&pMyObject->Property;
    PSTUN_HEADER                    pStunHeader               = (PSTUN_HEADER                  )NULL;
    PSTUN_ATTRIBUTE                 pStunAttribNext           = (PSTUN_ATTRIBUTE               )NULL;
    PSTUN_ATTRIB_USERNAME           pStunAttribUsername       = (PSTUN_ATTRIB_USERNAME         )NULL;
    PSTUN_ATTRIB_CONN_REQ_BINDING   pStunAttribConnReqBinding = (PSTUN_ATTRIB_CONN_REQ_BINDING )NULL;
    PSTUN_ATTRIB_MSG_INTEGRITY      pStunAttribMsgIntegrity   = (PSTUN_ATTRIB_MSG_INTEGRITY    )NULL;
    ULONG                           ulStunMsgLength           = (ULONG                         )0;
    ULONG                           ulUsernameBytes           = (ULONG                         )0;
    ANSC_CRYPTO_HASH                crypto_hash;
    ANSC_CRYPTO_KEY                 crypto_key;

    /*
     * For the purpose of binding discovery, the client sends a STUN Binding Request to a server,
     * without any flags set in the CHANGE-REQUEST attribute, and without the RESPONSE-ADDRESS
     * attribute. This causes the server to send the response back to the address and port that the
     * request came from.
     */
    pStunHeader = (PSTUN_HEADER)AnscAllocateMemory(STUN_MAX_MSG_SIZE);

    if ( !pStunHeader )
    {
        return  ANSC_STATUS_RESOURCES;
    }

    /*
     * The transaction ID is a 128 bit identifier. It also serves as salt to randomize the request
     * and the response. All responses carry the same identifier as the request they correspond to.
     */
    AnscWriteUlong(&pMyObject->TransID1[ 0], (ULONG)pMyObject           );
    AnscWriteUlong(&pMyObject->TransID1[ 4], AnscGetTickInMilliSeconds());
    AnscWriteUlong(&pMyObject->TransID1[ 8], pProperty->ClientAddr.Value);
    AnscWriteUlong(&pMyObject->TransID1[12], pProperty->ClientAddr.Value);

    AnscCryptoMd5Digest(pMyObject->TransID1, 16, &crypto_hash     );
    AnscCopyMemory        (pMyObject->TransID1, crypto_hash.Value, 16);

    AnscStunSetMessageType  (pStunHeader, STUN_MSG_TYPE_BindingRequest);
    AnscStunSetMessageLength(pStunHeader, 0                           );
    AnscStunSetTransactionID(pStunHeader, pMyObject->TransID1         );

    /*
     * The USERNAME attribute is used for message integrity. It serves as a means to identify the
     * shared secret used in the message integrity check. The USERNAME is always present in a
     * Shared Secret Response, along with the PASSWORD. It is optionally present in a Binding
     * Request when message integrity is used.
     *
     * The value of USERNAME is a variable length opaque value. Its length MUST be a multiple of 4
     * (measured in bytes) in order to guarantee alignment of attributes on word boundaries.
     */
    pStunAttribNext = (PSTUN_ATTRIBUTE)pStunHeader->Attributes;

    if ( AnscSizeOfString(pProperty->Username) > 0 )
    {
        pStunAttribUsername = (PSTUN_ATTRIB_USERNAME)pStunAttribNext;

        if ( AnscSizeOfString(pProperty->Username) % 4 != 0 )
        {
            ulUsernameBytes = (AnscSizeOfString(pProperty->Username) / 4 + 1) * 4;
        }
        else
        {
            ulUsernameBytes = AnscSizeOfString(pProperty->Username);
        }

        AnscStunAttribUnSetType  (pStunAttribUsername, STUN_ATTRIB_TYPE_USERNAME);
        AnscStunAttribUnSetLength(pStunAttribUsername, (USHORT)ulUsernameBytes  );
        AnscStunAttribUnSetValue (pStunAttribUsername, pProperty->Username      );

        pStunAttribNext  = AnscStunAttribGetNextAttrib(pStunAttribUsername);
        ulStunMsgLength += AnscStunAttribGetSize      (pStunAttribUsername);
    }

    /*
     * A CPE MUST include the CONNECTION-REQUEST-BINDING attribute in every Binding Request message
     * whose source address and port are the address and port on which it is listening for UDP
     * Connection Request messages. In all other Binding Request messages, the CPE MUST NOT include
     * this attribute.
     */
    #ifdef  _DSLH_STUN_
    {
        pStunAttribConnReqBinding = (PSTUN_ATTRIB_CONN_REQ_BINDING)pStunAttribNext;

        AnscStunAttribCrbSetAll(pStunAttribConnReqBinding);

        pStunAttribNext  = AnscStunAttribGetNextAttrib(pStunAttribConnReqBinding);
        ulStunMsgLength += AnscStunAttribGetSize      (pStunAttribConnReqBinding);
    }
    #endif

    if ( pMyObject->bMsgIntegrity1 )
    {
        pStunAttribMsgIntegrity = (PSTUN_ATTRIB_MSG_INTEGRITY)pStunAttribNext;

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

            AnscCryptoHmacSha1Digest
                (
                    (void*)pStunHeader,
                    (ULONG)((ULONG)pStunAttribMsgIntegrity - (ULONG)pStunHeader),
                    &crypto_hash,
                    &crypto_key
                );
        }

        AnscStunAttribMiSetType  (pStunAttribMsgIntegrity, STUN_ATTRIB_TYPE_MSG_INTEGRITY);
        AnscStunAttribMiSetLength(pStunAttribMsgIntegrity, 20                            );
        AnscStunAttribMiSetValue (pStunAttribMsgIntegrity, crypto_hash.Value             );

        pStunAttribNext  = AnscStunAttribGetNextAttrib(pStunAttribMsgIntegrity);
        ulStunMsgLength += AnscStunAttribGetSize      (pStunAttribMsgIntegrity);
    }

    if ( ulStunMsgLength > 0 )
    {
        AnscStunSetMessageLength(pStunHeader, (USHORT)ulStunMsgLength);
    }

    pMyObject->ClientState = STUN_SCO_CLIENT_STATE_Maintaining;
    returnStatus           =
        pMyObject->SendBindingRequest1
            (
                (ANSC_HANDLE)pMyObject,
                pStunHeader,
                AnscStunGetMessageSize(pStunHeader)
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        StunScoNotifyBindingChange
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to notify the binding has changed.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
StunScoNotifyBindingChange
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus              = ANSC_STATUS_SUCCESS;
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject                 = (PSTUN_SIMPLE_CLIENT_OBJECT    )hThisObject;
    PSTUN_SIMPLE_CLIENT_PROPERTY    pProperty                 = (PSTUN_SIMPLE_CLIENT_PROPERTY  )&pMyObject->Property;
    PSTUN_HEADER                    pStunHeader               = (PSTUN_HEADER                  )NULL;
    PSTUN_ATTRIBUTE                 pStunAttribNext           = (PSTUN_ATTRIBUTE               )NULL;
    PSTUN_ATTRIB_USERNAME           pStunAttribUsername       = (PSTUN_ATTRIB_USERNAME         )NULL;
    PSTUN_ATTRIB_CONN_REQ_BINDING   pStunAttribConnReqBinding = (PSTUN_ATTRIB_CONN_REQ_BINDING )NULL;
    PSTUN_ATTRIB_BINDING_CHANGE     pStunAttribBindingChange  = (PSTUN_ATTRIB_BINDING_CHANGE   )NULL;
    PSTUN_ATTRIB_MSG_INTEGRITY      pStunAttribMsgIntegrity   = (PSTUN_ATTRIB_MSG_INTEGRITY    )NULL;
    ULONG                           ulStunMsgLength           = (ULONG                         )0;
    ULONG                           ulUsernameBytes           = (ULONG                         )0;
    ANSC_CRYPTO_HASH                crypto_hash;
    ANSC_CRYPTO_KEY                 crypto_key;

    /*
     * For the purpose of binding discovery, the client sends a STUN Binding Request to a server,
     * without any flags set in the CHANGE-REQUEST attribute, and without the RESPONSE-ADDRESS
     * attribute. This causes the server to send the response back to the address and port that the
     * request came from.
     */
    pStunHeader = (PSTUN_HEADER)AnscAllocateMemory(STUN_MAX_MSG_SIZE);

    if ( !pStunHeader )
    {
        return  ANSC_STATUS_RESOURCES;
    }

    /*
     * The transaction ID is a 128 bit identifier. It also serves as salt to randomize the request
     * and the response. All responses carry the same identifier as the request they correspond to.
     */
    AnscWriteUlong(&pMyObject->TransID1[ 0], (ULONG)pMyObject           );
    AnscWriteUlong(&pMyObject->TransID1[ 4], AnscGetTickInMilliSeconds());
    AnscWriteUlong(&pMyObject->TransID1[ 8], pProperty->ClientAddr.Value);
    AnscWriteUlong(&pMyObject->TransID1[12], pProperty->ClientAddr.Value);

    AnscCryptoMd5Digest(pMyObject->TransID1, 16, &crypto_hash     );
    AnscCopyMemory        (pMyObject->TransID1, crypto_hash.Value, 16);

    AnscStunSetMessageType  (pStunHeader, STUN_MSG_TYPE_BindingRequest);
    AnscStunSetMessageLength(pStunHeader, 0                           );
    AnscStunSetTransactionID(pStunHeader, pMyObject->TransID1         );

    /*
     * The USERNAME attribute is used for message integrity. It serves as a means to identify the
     * shared secret used in the message integrity check. The USERNAME is always present in a
     * Shared Secret Response, along with the PASSWORD. It is optionally present in a Binding
     * Request when message integrity is used.
     *
     * The value of USERNAME is a variable length opaque value. Its length MUST be a multiple of 4
     * (measured in bytes) in order to guarantee alignment of attributes on word boundaries.
     */
    pStunAttribNext = (PSTUN_ATTRIBUTE)pStunHeader->Attributes;

    if ( AnscSizeOfString(pProperty->Username) > 0 )
    {
        pStunAttribUsername = (PSTUN_ATTRIB_USERNAME)pStunAttribNext;

        if ( AnscSizeOfString(pProperty->Username) % 4 != 0 )
        {
            ulUsernameBytes = (AnscSizeOfString(pProperty->Username) / 4 + 1) * 4;
        }
        else
        {
            ulUsernameBytes = AnscSizeOfString(pProperty->Username);
        }

        AnscStunAttribUnSetType  (pStunAttribUsername, STUN_ATTRIB_TYPE_USERNAME);
        AnscStunAttribUnSetLength(pStunAttribUsername, (USHORT)ulUsernameBytes  );
        AnscStunAttribUnSetValue (pStunAttribUsername, pProperty->Username      );

        pStunAttribNext  = AnscStunAttribGetNextAttrib(pStunAttribUsername);
        ulStunMsgLength += AnscStunAttribGetSize      (pStunAttribUsername);
    }

    /*
     * A CPE MUST include the CONNECTION-REQUEST-BINDING attribute in every Binding Request message
     * whose source address and port are the address and port on which it is listening for UDP
     * Connection Request messages. In all other Binding Request messages, the CPE MUST NOT include
     * this attribute.
     */
    #ifdef  _DSLH_STUN_
    {
        pStunAttribBindingChange = (PSTUN_ATTRIB_BINDING_CHANGE)pStunAttribNext;

        AnscStunAttribBcSetAll(pStunAttribBindingChange);

        pStunAttribNext  = AnscStunAttribGetNextAttrib(pStunAttribBindingChange);
        ulStunMsgLength += AnscStunAttribGetSize      (pStunAttribBindingChange);
    }
    #endif

    /*
     * Whenever the CPE detects a change to the NAT binding (as well as the first time the CPE
     * determines the binding), it MUST immediately send a Binding Request message from the primary
     * source port (the port on which the CPE is listening for UDP Connection Request messages)
     * that includes the BINDING-CHANGE attribute.
     */
    #ifdef  _DSLH_STUN_
    {
        pStunAttribConnReqBinding = (PSTUN_ATTRIB_CONN_REQ_BINDING)pStunAttribNext;

        AnscStunAttribCrbSetAll(pStunAttribConnReqBinding);

        pStunAttribNext  = AnscStunAttribGetNextAttrib(pStunAttribConnReqBinding);
        ulStunMsgLength += AnscStunAttribGetSize      (pStunAttribConnReqBinding);
    }
    #endif

    if ( pMyObject->bMsgIntegrity1 )
    {
        pStunAttribMsgIntegrity = (PSTUN_ATTRIB_MSG_INTEGRITY)pStunAttribNext;

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

            AnscCryptoHmacSha1Digest
                (
                    (void*)pStunHeader,
                    (ULONG)((ULONG)pStunAttribMsgIntegrity - (ULONG)pStunHeader),
                    &crypto_hash,
                    &crypto_key
                );
        }

        AnscStunAttribMiSetType  (pStunAttribMsgIntegrity, STUN_ATTRIB_TYPE_MSG_INTEGRITY);
        AnscStunAttribMiSetLength(pStunAttribMsgIntegrity, 20                            );
        AnscStunAttribMiSetValue (pStunAttribMsgIntegrity, crypto_hash.Value             );

        pStunAttribNext  = AnscStunAttribGetNextAttrib(pStunAttribMsgIntegrity);
        ulStunMsgLength += AnscStunAttribGetSize      (pStunAttribMsgIntegrity);
    }

    if ( ulStunMsgLength > 0 )
    {
        AnscStunSetMessageLength(pStunHeader, (USHORT)ulStunMsgLength);
    }

    pMyObject->ClientState = STUN_SCO_CLIENT_STATE_Notifying;
    returnStatus           =
        pMyObject->SendBindingRequest1
            (
                (ANSC_HANDLE)pMyObject,
                pStunHeader,
                AnscStunGetMessageSize(pStunHeader)
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        StunScoAsyncDiscoverTask
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to discover the life time of the NAT
        binding in a separate task.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
StunScoAsyncDiscoverTask
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject         = (PSTUN_SIMPLE_CLIENT_OBJECT    )hThisObject;
    PSTUN_SIMPLE_CLIENT_PROPERTY    pProperty         = (PSTUN_SIMPLE_CLIENT_PROPERTY  )&pMyObject->Property;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pProbeTimerObj    = (PANSC_TIMER_DESCRIPTOR_OBJECT )pMyObject->hProbeTimerObj;
    PANSC_SIMPLE_CLIENT_UDP_OBJECT  pSimpleClientUdpA = (PANSC_SIMPLE_CLIENT_UDP_OBJECT)pMyObject->hSimpleClientUdpA;
    PANSC_SIMPLE_CLIENT_UDP_OBJECT  pSimpleClientUdpB = (PANSC_SIMPLE_CLIENT_UDP_OBJECT)pMyObject->hSimpleClientUdpB;
    USHORT                          usUdpPortA        = (USHORT                        )STUN_DISCOVER_PORT_BASE - 1;
    ULONG                           ulPreTimestamp    = (ULONG                         )AnscGetTickInSeconds();
    int                             nTimeoutRoof      = (pProperty->MaxKeepAliveInterval > 0)? pProperty->MaxKeepAliveInterval : 0;
    int                             nTimeoutFloor     = pProperty->MinKeepAliveInterval;
    int                             nTimerDelay       = nTimeoutFloor;
    BOOL                            bRoofDetermined   = (nTimeoutRoof > 0)? TRUE : FALSE;
    int                             nProbeInterval    = 0;

    AnscTrace("StunScoAsyncDiscoverTask --- start running ...\n");

    pMyObject->AsyncTaskCount++;

    StunInitBindingInfo((&pMyObject->BindingInfoA));
    AnscResetEvent     ( &pMyObject->SocketEventA );

    /*
     * STUN can be used to discover the lifetimes of the bindings created by the NAT. In many cases,
     * the client will need to refresh the binding, either through a new STUN request, or an
     * application packet, in order for the application to continue to use the binding. By discover-
     * ing the binding lifetime, the client can determine how frequently it needs to refresh.
     */
    pSimpleClientUdpB->SetHostAddress   ((ANSC_HANDLE)pSimpleClientUdpB, pProperty->ClientAddr.Dot);
    pSimpleClientUdpB->SetHostPort      ((ANSC_HANDLE)pSimpleClientUdpB, 0                        );
    pSimpleClientUdpB->SetPeerAddress   ((ANSC_HANDLE)pSimpleClientUdpB, pProperty->ServerAddr.Dot);
    pSimpleClientUdpB->SetPeerPort      ((ANSC_HANDLE)pSimpleClientUdpB, pProperty->ServerPort    );
    pSimpleClientUdpB->SetMaxMessageSize((ANSC_HANDLE)pSimpleClientUdpB, STUN_MAX_MSG_SIZE        );
    pSimpleClientUdpB->SetMode          ((ANSC_HANDLE)pSimpleClientUdpB, pProperty->ClientMode    );
    pSimpleClientUdpB->Engage           ((ANSC_HANDLE)pSimpleClientUdpB);

    /*
     * The specific procedures by which the CPE uses Binding Requests from the secondary source
     * port to determine the binding timeout is highlighted below:
     *
     *      1. Choose a new port for socket A.
     *      2. Send a Binding Request from socket A.
     *      3. Expect to receive a Binding Response for step 2.
     *      4. Sleep for a while (current keepalive interval).
     *      5. Send a new Binding Request from socket B with RESPONSE-ADDRESS set to A.
     *      6. Expect to receive a Binding Response on socket A for step 5.
     *      7. If response is received, increase the current keepalive interval and go to 1.
     *      8. If no response is received, froze the current keepalive interval.
     */
    while ( pMyObject->bActive && ( !bRoofDetermined || nTimeoutFloor+1 < nTimeoutRoof ) )
    {
        AnscTrace
            (
                "StunScoAsyncDiscoverTask --- Timeout floor = %d, roof = %d, interval probing = %d.\n",
                nTimeoutFloor,
                nTimeoutRoof,
                nTimerDelay
            );

        usUdpPortA = (USHORT)STUN_DISCOVER_PORT_BASE - 1;

        /*
         * STEP 1: Choose a new port for socket A.
         */
        do
        {
            usUdpPortA ++;
            pSimpleClientUdpA->SetHostAddress   ((ANSC_HANDLE)pSimpleClientUdpA, pProperty->ClientAddr.Dot);
            pSimpleClientUdpA->SetHostPort      ((ANSC_HANDLE)pSimpleClientUdpA, usUdpPortA               );
            pSimpleClientUdpA->SetPeerAddress   ((ANSC_HANDLE)pSimpleClientUdpA, pProperty->ServerAddr.Dot);
            pSimpleClientUdpA->SetPeerPort      ((ANSC_HANDLE)pSimpleClientUdpA, pProperty->ServerPort    );
            pSimpleClientUdpA->SetMaxMessageSize((ANSC_HANDLE)pSimpleClientUdpA, STUN_MAX_MSG_SIZE        );
            pSimpleClientUdpA->SetMode          ((ANSC_HANDLE)pSimpleClientUdpA, pProperty->ClientMode    );
            returnStatus = pSimpleClientUdpA->Engage((ANSC_HANDLE)pSimpleClientUdpA);
        }
        while ( returnStatus != ANSC_STATUS_SUCCESS );

        /*
         * STEP 2: Send a Binding Request from socket A.
         */
        AnscResetEvent(&pMyObject->SocketEventA);

        pMyObject->DiscoverBindingTimeoutA((ANSC_HANDLE)pMyObject);

        AnscTrace("StunScoAsyncDiscoverTask --- binding request sent out through port A(%u).\n", usUdpPortA);

        /*
         * STEP 3: Expect to receive a Binding Response for step 2.
         *
         * According to RFC3489 dictates, at time 9500ms, if no
         * response is received, this transaction fails.
         */
        AnscWaitEvent(&pMyObject->SocketEventA, 9500);

        if ( pMyObject->BindingInfoA.Ip4Addr.Value == 0 )
        {
            /*
             * We've failed to acquire a new NAT binding on socket A, abort...
             */

            AnscTrace("StunScoAsyncDiscoverTask --- no response received for request sent through socket A!\n");

            break;
        }

        /*
         * STEP 4: Sleep for a while (current keepalive interval).
         */
        ulPreTimestamp = (ULONG)AnscGetTickInSeconds();

        while ( ((AnscGetTickInSeconds() - ulPreTimestamp) < (ULONG)nTimerDelay) && pMyObject->bActive )
        {
            AnscSleep(1000);
        }

        if ( !pMyObject->bActive )
        {
            break;
        }

        AnscTrace("StunScoAsyncDiscoverTask --- sending binding request through port B to see if interval %d is okay...\n", nTimerDelay);

        /*
         * STEP 5: Send a new Binding Request from socket B with RESPONSE-ADDRESS set to A.
         */
        AnscResetEvent(&pMyObject->SocketEventA);

        pMyObject->DiscoverBindingTimeoutB((ANSC_HANDLE)pMyObject);

        /*
         * STEP 6: Expect to receive a Binding Response on socket A for step 5.
         *
         * According to RFC3489 dictates, at time 9500ms, if no
         * response is received, this transaction fails.
         */
        AnscWaitEvent(&pMyObject->SocketEventA, 9500);

        if ( pMyObject->BindingInfoA.Ip4Addr.Value == 0 )
        {
            /*
             * We've failed to receive the response on socket A, the current timer delay is too big,
             * try smaller interval
             */
            AnscTrace("StunScoAsyncDiscoverTask --- interval %d seems too long!\n", nTimerDelay);

            bRoofDetermined = TRUE;
            nTimeoutRoof    = nTimerDelay;
            nTimerDelay     = (nTimeoutFloor + nTimeoutRoof)/2;

            continue;
        }

        /*
         * STEP 7: If response is received, increase the current keepalive interval and go to 1.
         */

        /*
         * Clean up the socket state and re-run the test.
         */

        /* be conservative and give us some room for retransmission described in RFC 3489 - 9500ms */
        nProbeInterval = nTimerDelay - 10;
        if ( nProbeInterval < pProperty->MinKeepAliveInterval )
        {
            nProbeInterval = pProperty->MinKeepAliveInterval;
        }

        AnscTrace("StunScoAsyncDiscoverTask --- probe timer interval is set to %d seconds.\n", nTimerDelay);
        pProbeTimerObj->SetInterval((ANSC_HANDLE)pProbeTimerObj, (ULONG)nProbeInterval * 1000);

        pSimpleClientUdpA->Cancel((ANSC_HANDLE)pSimpleClientUdpA);

        nTimeoutFloor = nTimerDelay;

        if ( !bRoofDetermined )
        {
            nTimeoutRoof = 2 * nTimeoutFloor;

            if ( pProperty->MaxKeepAliveInterval > 0 && nTimeoutRoof > pProperty->MaxKeepAliveInterval )
            {
                nTimeoutRoof    = pProperty->MaxKeepAliveInterval;
                bRoofDetermined = TRUE;
            }
        }

        nTimerDelay = (nTimeoutFloor + nTimeoutRoof)/2;
    }

    pSimpleClientUdpA->Cancel((ANSC_HANDLE)pSimpleClientUdpA);
    pSimpleClientUdpB->Cancel((ANSC_HANDLE)pSimpleClientUdpB);

    pMyObject->AsyncTaskCount--;

    AnscTrace("StunScoAsyncDiscoverTask --- terminated, final probe interval set = %d\n", nProbeInterval);

    return  returnStatus;
}
