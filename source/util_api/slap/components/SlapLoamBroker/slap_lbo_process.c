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

    module:	slap_lbo_process.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the Slap Loam Broker Object.

        *   SlapLboAsyncCallTaskLoco
        *   SlapLboProcessCallAcqc
        *   SlapLboProcessCallRelc
        *   SlapLboProcessCallLoco
        *   SlapLboProcessCallDelo
        *   SlapLboProcessCallInvo
        *   SlapLboProcessCallAcqo
        *   SlapLboProcessCallRelo

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/03/03    initial revision.

**********************************************************************/


#include "slap_lbo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLboAsyncCallTaskLoco
            (
                ANSC_HANDLE                 hCallInfo
            );

    description:

        This function is invoked in a separate task, which is spawned
        by the ProcessCallLoco(), to perform object location call.

    argument:   ANSC_HANDLE                 hCallInfo
                This handle points to a SLAP_LBO_ASYNC_CALL_INFO data
                structure, which contains information for performing
                SLAP_LOCO request.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLboAsyncCallTaskLoco
    (
        ANSC_HANDLE                 hCallInfo
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PSLAP_LBO_ASYNC_CALL_INFO       pAsyncCallInfo      = (PSLAP_LBO_ASYNC_CALL_INFO )hCallInfo;
    PSLAP_LOAM_BROKER_OBJECT        pMyObject           = NULL;
    PANSC_LPC_CONNECTOR_OBJECT      pAnscLpcConnector   = NULL;
    PANSC_LPC_PARTY_ADDR            pPartyAddr          = NULL;
    void*                           pMessageBuffer      = (void*                     )NULL;
    ULONG                           ulBufferSize        = (ULONG                     )0;
    PIMCP_SLAP_CALL                 pImcpSlapCall       = (PIMCP_SLAP_CALL           )NULL;
    PIMCP_SLAP_LOCO_CALL            pImcpSlapLocoCall   = (PIMCP_SLAP_LOCO_CALL      )NULL;
    ULONG                           ulCallBuffSize      = (ULONG                     )0;
    ULONG                           ulCallDataSize      = (ULONG                     )0;
    PANSC_BUFFER_DESCRIPTOR         pImcpSlapAnswerBdo  = (PANSC_BUFFER_DESCRIPTOR   )NULL;
    PIMCP_SLAP_ANSWER               pImcpSlapAnswer     = (PIMCP_SLAP_ANSWER         )NULL;
    PIMCP_SLAP_LOCO_ANSWER          pImcpSlapLocoAnswer = (PIMCP_SLAP_LOCO_ANSWER    )NULL;
    ULONG                           ulLpcErrorCode      = (ULONG                     )ANSC_LPC_ERROR_noError;

    /* RDKB-6243, CID-24481; null checking on before use 
    ** NULL check of "pAsyncCallInfo" done on "EXIT0" before releasing semaphone
    ** the variable indicates that the variable may be NULL.
    */
    if(pAsyncCallInfo)
    {
        pMyObject           = (PSLAP_LOAM_BROKER_OBJECT  )pAsyncCallInfo->hThisObject;
        pPartyAddr          = (PANSC_LPC_PARTY_ADDR      )pAsyncCallInfo->hPartyAddr;

        if(!pMyObject || !pPartyAddr)
        {
            goto EXIT0;
        }

        pAnscLpcConnector   = (PANSC_LPC_CONNECTOR_OBJECT)pMyObject->hAnscLpcConnector;
        if(!pAnscLpcConnector)
        {
            goto EXIT0;
        }
    }
    else
    {
        goto  EXIT0;
    }

    if ( TRUE )
    {
        ulBufferSize   = sizeof(IMCP_SLAP_CALL) + sizeof(IMCP_SLAP_LOCO_CALL);
        ulBufferSize  += AnscSizeOfString(pAsyncCallInfo->ObjectName);
        ulBufferSize  += IMCP_REQUEST_BUFFER_OVERHEAD;
        pMessageBuffer = AnscAllocateMemory(ulBufferSize);

        if ( !pMessageBuffer )
        {
            returnStatus = ANSC_STATUS_RESOURCES;

            goto  EXIT0;
        }
        else
        {
            pImcpSlapCall     = (PIMCP_SLAP_CALL     )((ULONG)pMessageBuffer + IMCP_REQUEST_BUFFER_OVERHEAD);
            pImcpSlapLocoCall = (PIMCP_SLAP_LOCO_CALL)ImcpSlapCallGetData(pImcpSlapCall);
            ulCallDataSize    = (ULONG               )sizeof(IMCP_SLAP_LOCO_CALL) + AnscSizeOfString(pAsyncCallInfo->ObjectName);
            ulCallBuffSize    = (ULONG               )sizeof(IMCP_SLAP_CALL) + ulCallDataSize;
        }
    }

    if ( TRUE )
    {
        ImcpSlapCallSetType(pImcpSlapCall, IMCP_SLAP_CALL_TYPE_LOCO);
        ImcpSlapCallSetSize(pImcpSlapCall, ulCallDataSize          );
    }

    if ( TRUE )
    {
        ImcpSlapLocoCallSetObjectType(pImcpSlapLocoCall, pAsyncCallInfo->ObjectType);
        ImcpSlapLocoCallSetObjectName(pImcpSlapLocoCall, pAsyncCallInfo->ObjectName);
    }

    returnStatus =
        pAnscLpcConnector->ImcCallRequest
            (
                (ANSC_HANDLE)pAnscLpcConnector,
                pPartyAddr->PartyName,
                pImcpSlapCall,
                ulCallBuffSize,
                (ANSC_HANDLE*)&pImcpSlapAnswerBdo,
                &ulLpcErrorCode,
                0       /* using the default timeout value */
            );

    if ( (returnStatus   != ANSC_STATUS_SUCCESS   ) ||
         (ulLpcErrorCode != ANSC_LPC_ERROR_noError) )
    {
        goto  EXIT1;
    }
    else if ( !pImcpSlapAnswerBdo )
    {
        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT1;
    }
    else if ( AnscBdoGetBlockSize(pImcpSlapAnswerBdo) == 0 )
    {
        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT2;
    }
    else
    {
        pImcpSlapAnswer     = (PIMCP_SLAP_ANSWER     )AnscBdoGetBlock(pImcpSlapAnswerBdo);
        pImcpSlapLocoAnswer = (PIMCP_SLAP_LOCO_ANSWER)ImcpSlapAnswerGetData(pImcpSlapAnswer);
    }

    if ( ImcpSlapAnswerGetSize(pImcpSlapAnswer) < sizeof(IMCP_SLAP_LOCO_ANSWER) )
    {
        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT2;
    }
    else
    {
        returnStatus      = (ANSC_STATUS     )ImcpSlapLocoAnswerGetStatusCode(pImcpSlapLocoAnswer);
    }

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        goto  EXIT2;
    }
    else if ( (ImcpSlapAnswerGetSize(pImcpSlapAnswer) - sizeof(IMCP_SLAP_LOCO_ANSWER)) <= sizeof(IMCP_PARTY_INFO) )
    {
        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT2;
    }
    else
    {

        if ( *pAsyncCallInfo->phDstPartyAddr == (ANSC_HANDLE)NULL )
        {
            *pAsyncCallInfo->phDstPartyAddr = (ANSC_HANDLE)pPartyAddr;
        }
    }

    returnStatus = ANSC_STATUS_SUCCESS;

    goto  EXIT2;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT2:

    if ( pImcpSlapAnswerBdo )
    {
        AnscFreeBdo(pImcpSlapAnswerBdo);
    }

EXIT1:

    if ( pMessageBuffer )
    {
        AnscFreeMemory(pMessageBuffer);
    }

EXIT0:

    if ( pAsyncCallInfo )
    {
        AnscReleaseSemaphore((ANSC_SEMAPHORE*)pAsyncCallInfo->hCallSemaphore, 1);

        AnscFreeMemory(pAsyncCallInfo);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLboProcessCallAcqc
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hLpcPartyAddr,
                void*                       pInputBuffer,
                ULONG                       ulInputSize,
                void**                      ppOutputBuffer,
                PULONG                      pulOutputSize,
                PULONG                      pulErrorCode
            );

    description:

        This function is called by the underlying LPC connector to
        dispatch a IMCP-based SLAP call.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hLpcPartyAddr
                Specifies the LPC party who initiated this call.

                void*                       pInputBuffer
                Specifies the input buffer of the call.

                ULONG                       ulInputSize
                Specifies the size of the input buffer.

                void**                      ppOutputBuffer
                Specifies the output buffer of the call to be returned.

                PULONG                      pulOutputSize
                Specifies the size of the output buffer to be returned.

                PULONG                      pulErrorCode
                Specifies the error code of the LPC operation.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLboProcessCallAcqc
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hLpcPartyAddr,
        void*                       pInputBuffer,
        ULONG                       ulInputSize,
        void**                      ppOutputBuffer,
        PULONG                      pulOutputSize,
        PULONG                      pulErrorCode
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hLpcPartyAddr);
    UNREFERENCED_PARAMETER(pInputBuffer);
    UNREFERENCED_PARAMETER(ulInputSize);

    *ppOutputBuffer = NULL;
    *pulOutputSize  = 0;
    *pulErrorCode   = ANSC_LPC_ERROR_invalidReqType;

    return  ANSC_STATUS_UNAPPLICABLE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLboProcessCallRelc
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hLpcPartyAddr,
                void*                       pInputBuffer,
                ULONG                       ulInputSize,
                void**                      ppOutputBuffer,
                PULONG                      pulOutputSize,
                PULONG                      pulErrorCode
            );

    description:

        This function is called by the underlying LPC connector to
        dispatch a IMCP-based SLAP call.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hLpcPartyAddr
                Specifies the LPC party who initiated this call.

                void*                       pInputBuffer
                Specifies the input buffer of the call.

                ULONG                       ulInputSize
                Specifies the size of the input buffer.

                void**                      ppOutputBuffer
                Specifies the output buffer of the call to be returned.

                PULONG                      pulOutputSize
                Specifies the size of the output buffer to be returned.

                PULONG                      pulErrorCode
                Specifies the error code of the LPC operation.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLboProcessCallRelc
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hLpcPartyAddr,
        void*                       pInputBuffer,
        ULONG                       ulInputSize,
        void**                      ppOutputBuffer,
        PULONG                      pulOutputSize,
        PULONG                      pulErrorCode
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hLpcPartyAddr);
    UNREFERENCED_PARAMETER(pInputBuffer);
    UNREFERENCED_PARAMETER(ulInputSize);

    *ppOutputBuffer = NULL;
    *pulOutputSize  = 0;
    *pulErrorCode   = ANSC_LPC_ERROR_invalidReqType;

    return  ANSC_STATUS_UNAPPLICABLE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLboProcessCallLoco
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hLpcPartyAddr,
                void*                       pInputBuffer,
                ULONG                       ulInputSize,
                void**                      ppOutputBuffer,
                PULONG                      pulOutputSize,
                PULONG                      pulErrorCode
            );

    description:

        This function is called by the underlying LPC connector to
        dispatch a IMCP-based SLAP call.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hLpcPartyAddr
                Specifies the LPC party who initiated this call.

                void*                       pInputBuffer
                Specifies the input buffer of the call.

                ULONG                       ulInputSize
                Specifies the size of the input buffer.

                void**                      ppOutputBuffer
                Specifies the output buffer of the call to be returned.

                PULONG                      pulOutputSize
                Specifies the size of the output buffer to be returned.

                PULONG                      pulErrorCode
                Specifies the error code of the LPC operation.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLboProcessCallLoco
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hLpcPartyAddr,
        void*                       pInputBuffer,
        ULONG                       ulInputSize,
        void**                      ppOutputBuffer,
        PULONG                      pulOutputSize,
        PULONG                      pulErrorCode
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PSLAP_LOAM_BROKER_OBJECT        pMyObject           = (PSLAP_LOAM_BROKER_OBJECT  )hThisObject;
    PANSC_LPC_CONNECTOR_OBJECT      pAnscLpcConnector   = (PANSC_LPC_CONNECTOR_OBJECT)pMyObject->hAnscLpcConnector;
    PANSC_LPC_PARTY_ADDR            pCallerPartyAddr    = (PANSC_LPC_PARTY_ADDR      )hLpcPartyAddr;
    PIMCP_SLAP_LOCO_CALL            pImcpSlapLocoCall   = (PIMCP_SLAP_LOCO_CALL      )pInputBuffer;
    void*                           pMessageBuffer      = (void*                     )NULL;
    ULONG                           ulBufferSize        = (ULONG                     )0;
    PIMCP_SLAP_ANSWER               pImcpSlapAnswer     = (PIMCP_SLAP_ANSWER         )NULL;
    PIMCP_SLAP_LOCO_ANSWER          pImcpSlapLocoAnswer = (PIMCP_SLAP_LOCO_ANSWER    )NULL;
    PIMCP_PARTY_INFO                pImcpPartyInfo      = (PIMCP_PARTY_INFO          )NULL;
    ULONG                           ulAnswerBuffSize    = (ULONG                     )0;
    ULONG                           ulAnswerDataSize    = (ULONG                     )0;
    ULONG                           ulObjectType        = (ULONG                     )0;
    char*                           pObjectName         = (char*                     )NULL;
    PSLAP_LBO_OBJECT_BINDING        pObjBinding         = (PSLAP_LBO_OBJECT_BINDING  )NULL;
    ANSC_STATUS                     asStatusCode        = (ANSC_STATUS               )ANSC_STATUS_SUCCESS;
    ULONG                           ulPartyAddrSize     = (ULONG                     )0;
    ULONG                           ulPartyAddrCount    = (ULONG                     )0;
    PANSC_LPC_PARTY_ADDR            pPartyAddr          = (PANSC_LPC_PARTY_ADDR      )NULL;
    PANSC_LPC_PARTY_ADDR            pPartyAddrFound     = (PANSC_LPC_PARTY_ADDR      )NULL;
    PANSC_LPC_PARTY_ADDR            pPartyAddrArray     = (PANSC_LPC_PARTY_ADDR      )NULL;
    ULONG                           ulActiveCalls       = (ULONG                     )0;
    PSLAP_LBO_ASYNC_CALL_INFO       pAsyncCallInfo      = (PSLAP_LBO_ASYNC_CALL_INFO )NULL;
    ULONG                           i                   = 0;
    ANSC_SEMAPHORE                  call_semaphore;

    *ppOutputBuffer = NULL;
    *pulOutputSize  = 0;
    *pulErrorCode   = ANSC_LPC_ERROR_noError;

    if ( ulInputSize <= sizeof(IMCP_SLAP_LOCO_CALL) )
    {
        *pulErrorCode = ANSC_LPC_ERROR_invalidReqData;

        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        ulObjectType = ImcpSlapLocoCallGetObjectType(pImcpSlapLocoCall);
        pObjectName  = (char*)AnscAllocateMemory(ulInputSize - sizeof(IMCP_SLAP_LOCO_CALL) + 1);

        if ( !pObjectName )
        {
            *pulErrorCode = ANSC_LPC_ERROR_resources;

            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            AnscCopyMemory
                (
                    pObjectName,
                    ImcpSlapLocoCallGetObjectName(pImcpSlapLocoCall),
                    ulInputSize - sizeof(IMCP_SLAP_LOCO_CALL)
                );
        }
    }

    /*
     * We should synchronize the invocation of simultaneous LOCO calls, that is, a LOCO call CANNOT
     * be started until the previous one completes. This has following benefits:
     *
     *      - reduce the number of threads may be created at once;
     *      - reduce the amount of IMCP traffic flow between processes;
     *      - avoid having to locate the same object twice.
     */
    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject);

    /*
     * To speed up the process of locating a SLAP object, we maintain a hash table interally for
     * all obj_name and party_name pairs. By centralizing the caching for name resolution, the time
     * and overhead involved in the communications between LPC parties are effectively reduced.
     * Note that the performance can be further improved if we maintain a separate cache for each
     * LPC party at the price of consuming much more memory.
     */
    pObjBinding =
        (PSLAP_LBO_OBJECT_BINDING)pMyObject->GetObjBinding
            (
                (ANSC_HANDLE)pMyObject,
                pObjectName
            );

    if ( pObjBinding )
    {
        pPartyAddrArray = NULL;
        pPartyAddrFound =
            (PANSC_LPC_PARTY_ADDR)pAnscLpcConnector->GetPartyAddr
                (
                    (ANSC_HANDLE)pAnscLpcConnector,
                    pObjBinding->PartyName
                );

        /*
         * If the associated 'party_addr' cannot be found, it usually means that the corresponding
         * LPC party has disconnected. We shall remove this 'obj_binding' and continue as if we had
         * no knowledge about this object whatsoever. Note that we should NEVER get here if the
         * underlying LPC connector works properly and informs us when a LPC party is disconnected.
         */
        if ( !pPartyAddrFound )
        {
            pMyObject->DelObjBinding((ANSC_HANDLE)pMyObject, pObjectName);
        }
    }
    else
    {
        pPartyAddrFound = NULL;
    }

    if ( !pPartyAddrFound )
    {
        /*
         * If the object name in question cannot be found in the cache, we need to 'broadcast' the
         * SLAP_LOCO REQUEST to all connected LPC parties (except the initiator and ourselves) and
         * examine the response from each party. Since the IMCP backbone doesn't offer any broad-
         * cast capability, we actually send a separate message to every LPC party.
         */
        ulActiveCalls    = 0;
        ulPartyAddrSize  = (ULONG)pAnscLpcConnector->GetPartyAddrSize((ANSC_HANDLE)pAnscLpcConnector);
        ulPartyAddrCount = (ULONG)0;
        pPartyAddrArray  = NULL;
        returnStatus     =
            pAnscLpcConnector->GetPartyArray
                (
                    (ANSC_HANDLE )pAnscLpcConnector,
                    (ANSC_HANDLE*)&pPartyAddrArray,
                    &ulPartyAddrCount
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            *pulErrorCode = ANSC_LPC_ERROR_internalError;

            goto  EXIT1;
        }
        else if ( ulPartyAddrCount == 0 )
        {
            *pulErrorCode = ANSC_LPC_ERROR_internalError;
            returnStatus  = ANSC_STATUS_INTERNAL_ERROR;

            goto  EXIT2;
        }
        else
        {
            AnscInitializeSemaphore(&call_semaphore, 0, ulPartyAddrCount);
        }

        for ( i = 0; i < ulPartyAddrCount; i++ )
        {
            pPartyAddr = (PANSC_LPC_PARTY_ADDR)((ULONG)pPartyAddrArray + i * ulPartyAddrSize);

            if ( strcasecmp(pPartyAddr->PartyName,pAnscLpcConnector->ImcGetPartyName((ANSC_HANDLE)pAnscLpcConnector)) == 0 )
            {
                continue;
            }
            else if ( pCallerPartyAddr &&
                      strcasecmp(pPartyAddr->PartyName,pCallerPartyAddr->PartyName) == 0)
            {
                continue;
            }
            else
            {
                pAsyncCallInfo = (PSLAP_LBO_ASYNC_CALL_INFO)AnscAllocateMemory(sizeof(SLAP_LBO_ASYNC_CALL_INFO));
            }

            if ( !pAsyncCallInfo )
            {
                continue;
            }
            else
            {
                pAsyncCallInfo->hThisObject    = (ANSC_HANDLE )pMyObject;
                pAsyncCallInfo->ObjectType     = (ULONG       )ulObjectType;
                pAsyncCallInfo->ObjectName     = (char*       )pObjectName;
                pAsyncCallInfo->hPartyAddr     = (ANSC_HANDLE )pPartyAddr;
                pAsyncCallInfo->hCallSemaphore = (ANSC_HANDLE )&call_semaphore;
                pAsyncCallInfo->phDstPartyAddr = (ANSC_HANDLE*)&pPartyAddrFound;
            }

            /*
             * Originally we dynamically spawn a separate task for each connected LPC Party: each
             * task sends LOCO request to the corresponding LPC Party and waits for the reply. It's
             * observed that such dynamic task creation might cause some weird problems on some
             * operating systems though the exact reason is still unknown. In an attempt to work
             * around the problem, we synchronize the LOCO request to each LPC Party.
             */
            /*
            returnStatus =
                AnscSpawnTask
                    (
                        pMyObject->AsyncCallTaskLoco,
                        (ANSC_HANDLE)pAsyncCallInfo,
                        "SlapLboAsyncCallTaskLoco"
                    );
            */
            returnStatus = pMyObject->AsyncCallTaskLoco((ANSC_HANDLE)pAsyncCallInfo);

            if ( returnStatus != ANSC_STATUS_SUCCESS )
            {
                /*
                 * Following statement is not needed for synchronized request...
                 */
                /*AnscFreeMemory(pAsyncCallInfo);*/

                continue;
            }
            else
            {
                ulActiveCalls++;
            }

            if ( pPartyAddrFound )
            {
                break;
            }
        }

        for ( i = 0; i < ulActiveCalls; i++ )
        {
            AnscAcquireSemaphore(&call_semaphore, 0xFFFFFFFF);
        }

        AnscFreeSemaphore(&call_semaphore);
    }

    /*
     * If any of the connected LPC party has responded with a positive answer, the 'pPartyAddrFound'
     * MUST now contain the corresponding handle of the target 'party_addr'; if all queries have
     * failed, this parameter will remain as NULL.
     */
    if ( pPartyAddrFound )
    {
        asStatusCode = ANSC_STATUS_SUCCESS;

        if ( !pObjBinding )
        {
            returnStatus =
                pMyObject->AddObjBinding
                    (
                        (ANSC_HANDLE)pMyObject,
                        pObjectName,
                        pPartyAddrFound->PartyName
                    );
        }
    }
    else
    {
        asStatusCode = ANSC_STATUS_CANT_FIND;
    }

    if ( TRUE )
    {
        ulBufferSize   = sizeof(IMCP_SLAP_ANSWER) + sizeof(IMCP_SLAP_LOCO_ANSWER);
        ulBufferSize  += (asStatusCode == ANSC_STATUS_SUCCESS)? sizeof(IMCP_PARTY_INFO) + AnscSizeOfString(pPartyAddrFound->PartyName) : 0;
        ulBufferSize  += IMCP_REPLY_BUFFER_OVERHEAD;
        pMessageBuffer = AnscAllocateMemory(ulBufferSize);

        if ( !pMessageBuffer )
        {
            *pulErrorCode = ANSC_LPC_ERROR_resources;
            returnStatus  = ANSC_STATUS_RESOURCES;

            goto  EXIT2;
        }
        else
        {
            pImcpSlapAnswer     = (PIMCP_SLAP_ANSWER     )((ULONG)pMessageBuffer + IMCP_REPLY_BUFFER_OVERHEAD);
            pImcpSlapLocoAnswer = (PIMCP_SLAP_LOCO_ANSWER)ImcpSlapAnswerGetData(pImcpSlapAnswer);
            pImcpPartyInfo      = (PIMCP_PARTY_INFO      )ImcpSlapLocoAnswerGetPartyInfo(pImcpSlapLocoAnswer);
            ulAnswerDataSize    = (ULONG                 )sizeof(IMCP_SLAP_LOCO_ANSWER);
            ulAnswerDataSize   += (ULONG                 )(asStatusCode == ANSC_STATUS_SUCCESS)? sizeof(IMCP_PARTY_INFO) + AnscSizeOfString(pPartyAddrFound->PartyName) : 0;
            ulAnswerBuffSize    = (ULONG                 )sizeof(IMCP_SLAP_ANSWER) + ulAnswerDataSize;
        }
    }

    if ( TRUE )
    {
        ImcpSlapAnswerSetSize(pImcpSlapAnswer, ulAnswerDataSize);
    }

    if ( TRUE )
    {
        ImcpSlapLocoAnswerSetStatusCode(pImcpSlapLocoAnswer, (ULONG)asStatusCode);

        if ( asStatusCode == ANSC_STATUS_SUCCESS )
        {
            ImcpPartyInfoSetPartyAddrValue(pImcpPartyInfo, pPartyAddrFound->PartyAddr.Value);
            ImcpPartyInfoSetPartyProtocol (pImcpPartyInfo, pPartyAddrFound->PartyProtocol  );
            ImcpPartyInfoSetPartyPort     (pImcpPartyInfo, pPartyAddrFound->PartyPort      );
            ImcpPartyInfoSetPartyName     (pImcpPartyInfo, pPartyAddrFound->PartyName      );
        }
    }

    *ppOutputBuffer = pImcpSlapAnswer;
    *pulOutputSize  = ulAnswerBuffSize;
    *pulErrorCode   = ANSC_LPC_ERROR_noError;
    returnStatus    = ANSC_STATUS_SUCCESS;

    goto  EXIT2;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT2:

    if ( pPartyAddrArray )
    {
        AnscFreeMemory(pPartyAddrArray);
    }

EXIT1:

    pMyObject->RelAccess((ANSC_HANDLE)pMyObject);

    if ( pObjectName )
    {
        AnscFreeMemory(pObjectName);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLboProcessCallDelo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hLpcPartyAddr,
                void*                       pInputBuffer,
                ULONG                       ulInputSize,
                void**                      ppOutputBuffer,
                PULONG                      pulOutputSize,
                PULONG                      pulErrorCode
            );

    description:

        This function is called by the underlying LPC connector to
        dispatch a IMCP-based SLAP call.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hLpcPartyAddr
                Specifies the LPC party who initiated this call.

                void*                       pInputBuffer
                Specifies the input buffer of the call.

                ULONG                       ulInputSize
                Specifies the size of the input buffer.

                void**                      ppOutputBuffer
                Specifies the output buffer of the call to be returned.

                PULONG                      pulOutputSize
                Specifies the size of the output buffer to be returned.

                PULONG                      pulErrorCode
                Specifies the error code of the LPC operation.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLboProcessCallDelo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hLpcPartyAddr,
        void*                       pInputBuffer,
        ULONG                       ulInputSize,
        void**                      ppOutputBuffer,
        PULONG                      pulOutputSize,
        PULONG                      pulErrorCode
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hLpcPartyAddr);
    UNREFERENCED_PARAMETER(pInputBuffer);
    UNREFERENCED_PARAMETER(ulInputSize);

    *ppOutputBuffer = NULL;
    *pulOutputSize  = 0;
    *pulErrorCode   = ANSC_LPC_ERROR_invalidReqType;

    return  ANSC_STATUS_UNAPPLICABLE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLboProcessCallInvo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hLpcPartyAddr,
                void*                       pInputBuffer,
                ULONG                       ulInputSize,
                void**                      ppOutputBuffer,
                PULONG                      pulOutputSize,
                PULONG                      pulErrorCode
            );

    description:

        This function is called by the underlying LPC connector to
        dispatch a IMCP-based SLAP call.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hLpcPartyAddr
                Specifies the LPC party who initiated this call.

                void*                       pInputBuffer
                Specifies the input buffer of the call.

                ULONG                       ulInputSize
                Specifies the size of the input buffer.

                void**                      ppOutputBuffer
                Specifies the output buffer of the call to be returned.

                PULONG                      pulOutputSize
                Specifies the size of the output buffer to be returned.

                PULONG                      pulErrorCode
                Specifies the error code of the LPC operation.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLboProcessCallInvo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hLpcPartyAddr,
        void*                       pInputBuffer,
        ULONG                       ulInputSize,
        void**                      ppOutputBuffer,
        PULONG                      pulOutputSize,
        PULONG                      pulErrorCode
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hLpcPartyAddr);
    UNREFERENCED_PARAMETER(pInputBuffer);
    UNREFERENCED_PARAMETER(ulInputSize);

    *ppOutputBuffer = NULL;
    *pulOutputSize  = 0;
    *pulErrorCode   = ANSC_LPC_ERROR_invalidReqType;

    return  ANSC_STATUS_UNAPPLICABLE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLboProcessCallAcqo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hLpcPartyAddr,
                void*                       pInputBuffer,
                ULONG                       ulInputSize,
                void**                      ppOutputBuffer,
                PULONG                      pulOutputSize,
                PULONG                      pulErrorCode
            );

    description:

        This function is called by the underlying LPC connector to
        dispatch a IMCP-based SLAP call.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hLpcPartyAddr
                Specifies the LPC party who initiated this call.

                void*                       pInputBuffer
                Specifies the input buffer of the call.

                ULONG                       ulInputSize
                Specifies the size of the input buffer.

                void**                      ppOutputBuffer
                Specifies the output buffer of the call to be returned.

                PULONG                      pulOutputSize
                Specifies the size of the output buffer to be returned.

                PULONG                      pulErrorCode
                Specifies the error code of the LPC operation.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLboProcessCallAcqo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hLpcPartyAddr,
        void*                       pInputBuffer,
        ULONG                       ulInputSize,
        void**                      ppOutputBuffer,
        PULONG                      pulOutputSize,
        PULONG                      pulErrorCode
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hLpcPartyAddr);
    UNREFERENCED_PARAMETER(pInputBuffer);
    UNREFERENCED_PARAMETER(ulInputSize);

    *ppOutputBuffer = NULL;
    *pulOutputSize  = 0;
    *pulErrorCode   = ANSC_LPC_ERROR_invalidReqType;

    return  ANSC_STATUS_UNAPPLICABLE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLboProcessCallRelo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hLpcPartyAddr,
                void*                       pInputBuffer,
                ULONG                       ulInputSize,
                void**                      ppOutputBuffer,
                PULONG                      pulOutputSize,
                PULONG                      pulErrorCode
            );

    description:

        This function is called by the underlying LPC connector to
        dispatch a IMCP-based SLAP call.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hLpcPartyAddr
                Specifies the LPC party who initiated this call.

                void*                       pInputBuffer
                Specifies the input buffer of the call.

                ULONG                       ulInputSize
                Specifies the size of the input buffer.

                void**                      ppOutputBuffer
                Specifies the output buffer of the call to be returned.

                PULONG                      pulOutputSize
                Specifies the size of the output buffer to be returned.

                PULONG                      pulErrorCode
                Specifies the error code of the LPC operation.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLboProcessCallRelo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hLpcPartyAddr,
        void*                       pInputBuffer,
        ULONG                       ulInputSize,
        void**                      ppOutputBuffer,
        PULONG                      pulOutputSize,
        PULONG                      pulErrorCode
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hLpcPartyAddr);
    UNREFERENCED_PARAMETER(pInputBuffer);
    UNREFERENCED_PARAMETER(ulInputSize);

    *ppOutputBuffer = NULL;
    *pulOutputSize  = 0;
    *pulErrorCode   = ANSC_LPC_ERROR_invalidReqType;

    return  ANSC_STATUS_UNAPPLICABLE;
}
