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

    module:	slap_lso_process.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the Slap Loam Server Object.

        *   SlapLsoProcessCallAcqc
        *   SlapLsoProcessCallRelc
        *   SlapLsoProcessCallLoco
        *   SlapLsoProcessCallDelo
        *   SlapLsoProcessCallInvo
        *   SlapLsoProcessCallAcqo
        *   SlapLsoProcessCallRelo

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/02/03    initial revision.

**********************************************************************/


#include "slap_lso_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLsoProcessCallAcqc
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
SlapLsoProcessCallAcqc
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
    UNREFERENCED_PARAMETER(hLpcPartyAddr);
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PSLAP_LOAM_SERVER_OBJECT        pMyObject           = (PSLAP_LOAM_SERVER_OBJECT  )hThisObject;
    PSLAP_UOA_INTERFACE             pSlapUoaIf          = (PSLAP_UOA_INTERFACE       )pMyObject->hSlapUoaIf;
    PIMCP_SLAP_ACQC_CALL            pImcpSlapAcqcCall   = (PIMCP_SLAP_ACQC_CALL      )pInputBuffer;
    void*                           pMessageBuffer      = (void*                     )NULL;
    ULONG                           ulBufferSize        = (ULONG                     )0;
    PIMCP_SLAP_ANSWER               pImcpSlapAnswer     = (PIMCP_SLAP_ANSWER         )NULL;
    PIMCP_SLAP_ACQC_ANSWER          pImcpSlapAcqcAnswer = (PIMCP_SLAP_ACQC_ANSWER    )NULL;
    ULONG                           ulAnswerBuffSize    = (ULONG                     )0;
    ULONG                           ulAnswerDataSize    = (ULONG                     )0;
    char*                           pContainerName      = (char*                     )NULL;
    ANSC_HANDLE                     hSlapContainer      = (ANSC_HANDLE               )NULL;

    *ppOutputBuffer = NULL;
    *pulOutputSize  = 0;
    *pulErrorCode   = ANSC_LPC_ERROR_noError;

    if ( ulInputSize <= sizeof(IMCP_SLAP_ACQC_CALL) )
    {
        *pulErrorCode = ANSC_LPC_ERROR_invalidReqData;

        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        pContainerName = (char*)AnscAllocateMemory(ulInputSize - sizeof(IMCP_SLAP_ACQC_CALL) + 1);

        if ( !pContainerName )
        {
            *pulErrorCode = ANSC_LPC_ERROR_resources;

            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            AnscCopyMemory
                (
                    pContainerName,
                    ImcpSlapAcqcCallGetContainerName(pImcpSlapAcqcCall),
                    ulInputSize - sizeof(IMCP_SLAP_ACQC_CALL)
                );
        }
    }

    hSlapContainer =
        pSlapUoaIf->AcqContainerAccess
            (
                pSlapUoaIf->hOwnerContext,
                pContainerName
            );

    if ( TRUE )
    {
        ulBufferSize   = sizeof(IMCP_SLAP_ANSWER) + sizeof(IMCP_SLAP_ACQC_ANSWER);
        ulBufferSize  += IMCP_REPLY_BUFFER_OVERHEAD;
        pMessageBuffer = AnscAllocateMemory(ulBufferSize);

        if ( !pMessageBuffer )
        {
            *pulErrorCode = ANSC_LPC_ERROR_resources;
            returnStatus  = ANSC_STATUS_RESOURCES;

            goto  EXIT1;
        }
        else
        {
            pImcpSlapAnswer     = (PIMCP_SLAP_ANSWER     )((ULONG)pMessageBuffer + IMCP_REPLY_BUFFER_OVERHEAD);
            pImcpSlapAcqcAnswer = (PIMCP_SLAP_ACQC_ANSWER)ImcpSlapAnswerGetData(pImcpSlapAnswer);
            ulAnswerDataSize    = (ULONG                 )sizeof(IMCP_SLAP_ACQC_ANSWER);
            ulAnswerBuffSize    = (ULONG                 )sizeof(IMCP_SLAP_ANSWER) + ulAnswerDataSize;
        }
    }

    if ( TRUE )
    {
        ImcpSlapAnswerSetSize(pImcpSlapAnswer, ulAnswerDataSize);
    }

    if ( TRUE )
    {
        ImcpSlapAcqcAnswerSetContainerId(pImcpSlapAcqcAnswer, (ULONG)hSlapContainer);
    }

    *ppOutputBuffer = pImcpSlapAnswer;
    *pulOutputSize  = ulAnswerBuffSize;
    *pulErrorCode   = ANSC_LPC_ERROR_noError;
    returnStatus    = ANSC_STATUS_SUCCESS;

    goto  EXIT1;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    if ( pContainerName )
    {
        AnscFreeMemory(pContainerName);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLsoProcessCallRelc
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
SlapLsoProcessCallRelc
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
    UNREFERENCED_PARAMETER(hLpcPartyAddr);
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PSLAP_LOAM_SERVER_OBJECT        pMyObject           = (PSLAP_LOAM_SERVER_OBJECT  )hThisObject;
    PSLAP_UOA_INTERFACE             pSlapUoaIf          = (PSLAP_UOA_INTERFACE       )pMyObject->hSlapUoaIf;
    PIMCP_SLAP_RELC_CALL            pImcpSlapRelcCall   = (PIMCP_SLAP_RELC_CALL      )pInputBuffer;
    void*                           pMessageBuffer      = (void*                     )NULL;
    ULONG                           ulBufferSize        = (ULONG                     )0;
    PIMCP_SLAP_ANSWER               pImcpSlapAnswer     = (PIMCP_SLAP_ANSWER         )NULL;
    PIMCP_SLAP_RELC_ANSWER          pImcpSlapRelcAnswer = (PIMCP_SLAP_RELC_ANSWER    )NULL;
    ULONG                           ulAnswerBuffSize    = (ULONG                     )0;
    ULONG                           ulAnswerDataSize    = (ULONG                     )0;
    ANSC_STATUS                     asStatusCode        = (ANSC_STATUS               )ANSC_STATUS_SUCCESS;
    ANSC_HANDLE                     hSlapContainer      = (ANSC_HANDLE               )NULL;

    *ppOutputBuffer = NULL;
    *pulOutputSize  = 0;
    *pulErrorCode   = ANSC_LPC_ERROR_noError;

    if ( ulInputSize != sizeof(IMCP_SLAP_RELC_CALL) )
    {
        *pulErrorCode = ANSC_LPC_ERROR_invalidReqData;

        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        hSlapContainer = (ANSC_HANDLE)ImcpSlapRelcCallGetContainerId(pImcpSlapRelcCall);
    }

    asStatusCode =
        pSlapUoaIf->RelContainerAccess
            (
                pSlapUoaIf->hOwnerContext,
                hSlapContainer
            );

    if ( TRUE )
    {
        ulBufferSize   = sizeof(IMCP_SLAP_ANSWER) + sizeof(IMCP_SLAP_RELC_ANSWER);
        ulBufferSize  += IMCP_REPLY_BUFFER_OVERHEAD;
        pMessageBuffer = AnscAllocateMemory(ulBufferSize);

        if ( !pMessageBuffer )
        {
            *pulErrorCode = ANSC_LPC_ERROR_resources;
            returnStatus  = ANSC_STATUS_RESOURCES;

            goto  EXIT1;
        }
        else
        {
            pImcpSlapAnswer     = (PIMCP_SLAP_ANSWER     )((ULONG)pMessageBuffer + IMCP_REPLY_BUFFER_OVERHEAD);
            pImcpSlapRelcAnswer = (PIMCP_SLAP_RELC_ANSWER)ImcpSlapAnswerGetData(pImcpSlapAnswer);
            ulAnswerDataSize    = (ULONG                 )sizeof(IMCP_SLAP_RELC_ANSWER);
            ulAnswerBuffSize    = (ULONG                 )sizeof(IMCP_SLAP_ANSWER) + ulAnswerDataSize;
        }
    }

    if ( TRUE )
    {
        ImcpSlapAnswerSetSize(pImcpSlapAnswer, ulAnswerDataSize);
    }

    if ( TRUE )
    {
        ImcpSlapRelcAnswerSetStatusCode(pImcpSlapRelcAnswer, (ULONG)asStatusCode);
    }

    *ppOutputBuffer = pImcpSlapAnswer;
    *pulOutputSize  = ulAnswerBuffSize;
    *pulErrorCode   = ANSC_LPC_ERROR_noError;
    returnStatus    = ANSC_STATUS_SUCCESS;

    goto  EXIT1;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLsoProcessCallLoco
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
SlapLsoProcessCallLoco
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
    UNREFERENCED_PARAMETER(hLpcPartyAddr);
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PSLAP_LOAM_SERVER_OBJECT        pMyObject           = (PSLAP_LOAM_SERVER_OBJECT  )hThisObject;
    PANSC_LPC_CONNECTOR_OBJECT      pAnscLpcConnector   = (PANSC_LPC_CONNECTOR_OBJECT)pMyObject->hAnscLpcConnector;
    PSLAP_UOA_INTERFACE             pSlapUoaIf          = (PSLAP_UOA_INTERFACE       )pMyObject->hSlapUoaIf;
    PIMCP_SLAP_LOCO_CALL            pImcpSlapLocoCall   = (PIMCP_SLAP_LOCO_CALL      )pInputBuffer;
    void*                           pMessageBuffer      = (void*                     )NULL;
    ULONG                           ulBufferSize        = (ULONG                     )0;
    PIMCP_SLAP_ANSWER               pImcpSlapAnswer     = (PIMCP_SLAP_ANSWER         )NULL;
    PIMCP_SLAP_LOCO_ANSWER          pImcpSlapLocoAnswer = (PIMCP_SLAP_LOCO_ANSWER    )NULL;
    PIMCP_PARTY_INFO                pImcpPartyInfo      = (PIMCP_PARTY_INFO          )NULL;
    ULONG                           ulAnswerBuffSize    = (ULONG                     )0;
    ULONG                           ulAnswerDataSize    = (ULONG                     )0;
    char*                           pObjectName         = (char*                     )NULL;
    ANSC_STATUS                     asStatusCode        = (ANSC_STATUS               )ANSC_STATUS_SUCCESS;
    char*                           pLpcPartyName       = (char*                     )NULL;
    PANSC_LPC_PARTY_ADDR            pLpcPartyAddr       = (PANSC_LPC_PARTY_ADDR      )NULL;

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
        pObjectName = (char*)AnscAllocateMemory(ulInputSize - sizeof(IMCP_SLAP_LOCO_CALL) + 1);

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

    if ( pSlapUoaIf->VerifyObjDescriptor
            (
                pSlapUoaIf->hOwnerContext,
                pObjectName
            ) )
    {
        asStatusCode  = ANSC_STATUS_SUCCESS;
        pLpcPartyName = pAnscLpcConnector->ImcGetPartyName((ANSC_HANDLE)pAnscLpcConnector);

        if ( !pLpcPartyName )
        {
            *pulErrorCode = ANSC_LPC_ERROR_internalError;

            return  ANSC_STATUS_INTERNAL_ERROR;
        }
        else
        {
            pLpcPartyAddr =
                pAnscLpcConnector->GetPartyAddr
                    (
                        (ANSC_HANDLE)pAnscLpcConnector,
                        pLpcPartyName
                    );
        }

        if ( !pLpcPartyAddr )
        {
            *pulErrorCode = ANSC_LPC_ERROR_internalError;

            return  ANSC_STATUS_INTERNAL_ERROR;
        }
    }
    else
    {
        asStatusCode = ANSC_STATUS_CANT_FIND;
    }

    if ( TRUE )
    {
        ulBufferSize   = sizeof(IMCP_SLAP_ANSWER) + sizeof(IMCP_SLAP_LOCO_ANSWER);
        ulBufferSize  += (asStatusCode == ANSC_STATUS_SUCCESS)? sizeof(IMCP_PARTY_INFO) + AnscSizeOfString(pLpcPartyName) : 0;
        ulBufferSize  += IMCP_REPLY_BUFFER_OVERHEAD;
        pMessageBuffer = AnscAllocateMemory(ulBufferSize);

        if ( !pMessageBuffer )
        {
            *pulErrorCode = ANSC_LPC_ERROR_resources;
            returnStatus  = ANSC_STATUS_RESOURCES;

            goto  EXIT1;
        }
        else
        {
            pImcpSlapAnswer     = (PIMCP_SLAP_ANSWER     )((ULONG)pMessageBuffer + IMCP_REPLY_BUFFER_OVERHEAD);
            pImcpSlapLocoAnswer = (PIMCP_SLAP_LOCO_ANSWER)ImcpSlapAnswerGetData(pImcpSlapAnswer);
            pImcpPartyInfo      = (PIMCP_PARTY_INFO      )ImcpSlapLocoAnswerGetPartyInfo(pImcpSlapLocoAnswer);
            ulAnswerDataSize    = (ULONG                 )sizeof(IMCP_SLAP_LOCO_ANSWER);
            ulAnswerDataSize   += (ULONG                 )(asStatusCode == ANSC_STATUS_SUCCESS)? sizeof(IMCP_PARTY_INFO) + AnscSizeOfString(pLpcPartyName) : 0;
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
            ImcpPartyInfoSetPartyAddrValue(pImcpPartyInfo, pLpcPartyAddr->PartyAddr.Value);
            ImcpPartyInfoSetPartyProtocol (pImcpPartyInfo, pLpcPartyAddr->PartyProtocol  );
            ImcpPartyInfoSetPartyPort     (pImcpPartyInfo, pLpcPartyAddr->PartyPort      );
            ImcpPartyInfoSetPartyName     (pImcpPartyInfo, pLpcPartyAddr->PartyName      );
        }
    }

    *ppOutputBuffer = pImcpSlapAnswer;
    *pulOutputSize  = ulAnswerBuffSize;
    *pulErrorCode   = ANSC_LPC_ERROR_noError;
    returnStatus    = ANSC_STATUS_SUCCESS;

    goto  EXIT1;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

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
        SlapLsoProcessCallDelo
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
SlapLsoProcessCallDelo
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
    UNREFERENCED_PARAMETER(hLpcPartyAddr);
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PSLAP_LOAM_SERVER_OBJECT        pMyObject           = (PSLAP_LOAM_SERVER_OBJECT  )hThisObject;
    PSLAP_UOA_INTERFACE             pSlapUoaIf          = (PSLAP_UOA_INTERFACE       )pMyObject->hSlapUoaIf;
    PIMCP_SLAP_DELO_CALL            pImcpSlapDeloCall   = (PIMCP_SLAP_DELO_CALL      )pInputBuffer;
    void*                           pMessageBuffer      = (void*                     )NULL;
    ULONG                           ulBufferSize        = (ULONG                     )0;
    PIMCP_SLAP_ANSWER               pImcpSlapAnswer     = (PIMCP_SLAP_ANSWER         )NULL;
    PIMCP_SLAP_DELO_ANSWER          pImcpSlapDeloAnswer = (PIMCP_SLAP_DELO_ANSWER    )NULL;
    ULONG                           ulAnswerBuffSize    = (ULONG                     )0;
    ULONG                           ulAnswerDataSize    = (ULONG                     )0;
    ANSC_STATUS                     asStatusCode        = (ANSC_STATUS               )ANSC_STATUS_SUCCESS;
    ANSC_HANDLE                     hSlapObject         = (ANSC_HANDLE               )NULL;

    *ppOutputBuffer = NULL;
    *pulOutputSize  = 0;
    *pulErrorCode   = ANSC_LPC_ERROR_noError;

    if ( ulInputSize != sizeof(IMCP_SLAP_DELO_CALL) )
    {
        *pulErrorCode = ANSC_LPC_ERROR_invalidReqData;

        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        hSlapObject = (ANSC_HANDLE)ImcpSlapDeloCallGetObjectId(pImcpSlapDeloCall);
    }

    asStatusCode =
        pSlapUoaIf->DeleteObject
            (
                pSlapUoaIf->hOwnerContext,
                hSlapObject
            );

    if ( TRUE )
    {
        ulBufferSize   = sizeof(IMCP_SLAP_ANSWER) + sizeof(IMCP_SLAP_DELO_ANSWER);
        ulBufferSize  += IMCP_REPLY_BUFFER_OVERHEAD;
        pMessageBuffer = AnscAllocateMemory(ulBufferSize);

        if ( !pMessageBuffer )
        {
            *pulErrorCode = ANSC_LPC_ERROR_resources;
            returnStatus  = ANSC_STATUS_RESOURCES;

            goto  EXIT1;
        }
        else
        {
            pImcpSlapAnswer     = (PIMCP_SLAP_ANSWER     )((ULONG)pMessageBuffer + IMCP_REPLY_BUFFER_OVERHEAD);
            pImcpSlapDeloAnswer = (PIMCP_SLAP_DELO_ANSWER)ImcpSlapAnswerGetData(pImcpSlapAnswer);
            ulAnswerDataSize    = (ULONG                 )sizeof(IMCP_SLAP_DELO_ANSWER);
            ulAnswerBuffSize    = (ULONG                 )sizeof(IMCP_SLAP_ANSWER) + ulAnswerDataSize;
        }
    }

    if ( TRUE )
    {
        ImcpSlapAnswerSetSize(pImcpSlapAnswer, ulAnswerDataSize);
    }

    if ( TRUE )
    {
        ImcpSlapDeloAnswerSetStatusCode(pImcpSlapDeloAnswer, (ULONG)asStatusCode);
    }

    *ppOutputBuffer = pImcpSlapAnswer;
    *pulOutputSize  = ulAnswerBuffSize;
    *pulErrorCode   = ANSC_LPC_ERROR_noError;
    returnStatus    = ANSC_STATUS_SUCCESS;

    goto  EXIT1;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLsoProcessCallInvo
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
SlapLsoProcessCallInvo
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
    UNREFERENCED_PARAMETER(hLpcPartyAddr);
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PSLAP_LOAM_SERVER_OBJECT        pMyObject           = (PSLAP_LOAM_SERVER_OBJECT  )hThisObject;
    PSLAP_LOAM_SERVER_PROPERTY      pProperty           = (PSLAP_LOAM_SERVER_PROPERTY)&pMyObject->Property;
    PSLAP_UOA_INTERFACE             pSlapUoaIf          = (PSLAP_UOA_INTERFACE       )pMyObject->hSlapUoaIf;
    PIMCP_SLAP_INVO_CALL            pImcpSlapInvoCall   = (PIMCP_SLAP_INVO_CALL      )pInputBuffer;
    void*                           pMessageBuffer      = (void*                     )NULL;
    ULONG                           ulBufferSize        = (ULONG                     )0;
    PIMCP_SLAP_ANSWER               pImcpSlapAnswer     = (PIMCP_SLAP_ANSWER         )NULL;
    PIMCP_SLAP_INVO_ANSWER          pImcpSlapInvoAnswer = (PIMCP_SLAP_INVO_ANSWER    )NULL;
    ULONG                           ulAnswerBuffSize    = (ULONG                     )0;
    ULONG                           ulAnswerDataSize    = (ULONG                     )0;
    char*                           pMethodName         = (char*                     )NULL;
    char*                           pObjName            = (char*                     )NULL;
    ULONG                           ulObjType           = (ULONG                     )0;
    ANSC_HANDLE                     hSlapObject         = (ANSC_HANDLE               )NULL;
    ULONG                           ulImcpVarCount      = (ULONG                     )0;
    ULONG                           ulImcpVarArraySize  = (ULONG                     )0;
    ULONG                           ulImcpVarSize       = (ULONG                     )0;
    PIMCP_SLAP_VARIABLE             pImcpSlapVar        = (PIMCP_SLAP_VARIABLE       )NULL;
    PSLAP_PARAMETER_LIST            pInputParamList     = (PSLAP_PARAMETER_LIST      )NULL;
    PSLAP_PARAMETER_LIST            pOutputParamList    = (PSLAP_PARAMETER_LIST      )NULL;
    PSLAP_PARAMETER_LIST            pExportParamList    = (PSLAP_PARAMETER_LIST      )NULL;
    PSLAP_VARIABLE                  pReturnedVar        = (PSLAP_VARIABLE            )NULL;
    ANSC_STATUS                     asStatusCode        = (ANSC_STATUS               )ANSC_STATUS_SUCCESS;
    PSLAP_OBJ_RECORD_OBJECT         pSlapObjRecord      = (PSLAP_OBJ_RECORD_OBJECT   )NULL;
    PSLAP_SRV_COMPONENT_OBJECT      pSlapSrvComponent   = (PSLAP_SRV_COMPONENT_OBJECT)NULL;
    PSLAP_SCO_STANDARD_OBJECT       pSlapScoStandard    = (PSLAP_SCO_STANDARD_OBJECT )NULL;
    ULONG                           ulReturnFlag        = (ULONG                     )0;
    BOOL                            bValidSlapCall      = (BOOL                      )TRUE;
    BOOL                            bAutoExportObj      = (BOOL                      )FALSE;
    ULONG                           i                   = 0;

    *ppOutputBuffer = NULL;
    *pulOutputSize  = 0;
    *pulErrorCode   = ANSC_LPC_ERROR_noError;

    ImcpSlapInvoCallValidate(pImcpSlapInvoCall, ulInputSize, bValidSlapCall);

    if ( !bValidSlapCall )
    {
        *pulErrorCode = ANSC_LPC_ERROR_invalidReqData;

        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        hSlapObject = (ANSC_HANDLE)ImcpSlapInvoCallGetObjectId(pImcpSlapInvoCall);
        pMethodName = (char*      )AnscAllocateMemory(ImcpSlapInvoCallGetMethodNameSize(pImcpSlapInvoCall) + 1);

        if ( !hSlapObject )
        {
            *pulErrorCode = ANSC_LPC_ERROR_invalidReqData;
            returnStatus  = ANSC_STATUS_UNAPPLICABLE;

            goto  EXIT1;
        }
        else if ( !pMethodName )
        {
            *pulErrorCode = ANSC_LPC_ERROR_resources;

            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            AnscCopyMemory
                (
                    pMethodName,
                    ImcpSlapInvoCallGetMethodName(pImcpSlapInvoCall),
                    ImcpSlapInvoCallGetMethodNameSize(pImcpSlapInvoCall)
                );
        }
    }

    if ( TRUE )
    {
        pInputParamList = (PSLAP_PARAMETER_LIST)AnscAllocateMemory(sizeof(SLAP_PARAMETER_LIST));

        if ( !pInputParamList )
        {
            *pulErrorCode = ANSC_LPC_ERROR_resources;
            returnStatus  = ANSC_STATUS_RESOURCES;

            goto  EXIT1;
        }
        else
        {
            ImcpSlapInvoCallGetVarCount(pImcpSlapInvoCall, ulImcpVarCount);

            pInputParamList->ParamCount = ulImcpVarCount;

            for ( i = 0; i < ulImcpVarCount; i++ )
            {
                ImcpSlapInvoCallGetVarByIndex(pImcpSlapInvoCall, pImcpSlapVar, i);

                SlapConvFromImcpVar(pImcpSlapVar, (&pInputParamList->ParamArray[i]));
            }
        }
    }

    ulImcpVarCount     = 0;
    ulImcpVarArraySize = 0;
    asStatusCode       =
        pSlapUoaIf->InvokeObject
            (
                pSlapUoaIf->hOwnerContext,
                hSlapObject,
                pMethodName,
                pInputParamList,
                &pOutputParamList,
                &pReturnedVar
            );

    if ( asStatusCode == ANSC_STATUS_SUCCESS )
    {
        if ( pReturnedVar )
        {
            SlapImcpGetVarSize(pReturnedVar, ulImcpVarSize);

            /*
             * To support the encapsulation of 'mobile' object, we need some special treatment for
             * 'object' variables: in addition to transmit the object handle, we must also encapsu-
             * late the 'obj_name' and 'obj_type'.
             */
            if ( pReturnedVar->Syntax == SLAP_VAR_SYNTAX_object )
            {
                pSlapObjRecord = (PSLAP_OBJ_RECORD_OBJECT)pReturnedVar->Variant.varObject;

                if ( pSlapObjRecord )
                {
                    pSlapSrvComponent = (PSLAP_SRV_COMPONENT_OBJECT)pSlapObjRecord->hSlapSrvComponent;
                    pObjName          = pSlapSrvComponent->ObjName;
                    ulObjType         = pSlapSrvComponent->ObjType;

                    ulImcpVarSize += sizeof(ULONG) + AnscSizeOfString(pObjName) + 1;

                    /*
                     * If the object is a mobile object and the 'bMobilityOn' flag is enabled, we
                     * shall transfer the object data in the same payload. This will reduce the
                     * number of Request/Reply exchanges between the LOAM_CLIENT and LOAM_SERVER
                     * modules.
                     */
                    if ( (ulObjType & SLAP_OBJ_TYPE_mobile) && pProperty->bMobilityOn && !pOutputParamList )
                    {
                        bAutoExportObj = TRUE;
                    }
                }
            }

            ulImcpVarCount     += 1;
            ulImcpVarArraySize += ulImcpVarSize;
            ulReturnFlag       |= IMCP_SLAP_INVO_FLAG_returnedValue;

            /*
             * We can only transfer the object data when there's no output parameters. The current
             * encoding/decoding mechanism in IMCP is not sufficient to support auto-transfer for
             * object or object-array variables included in the output parameter list.
             */
            if ( bAutoExportObj )
            {
                pSlapScoStandard = (PSLAP_SCO_STANDARD_OBJECT)pSlapSrvComponent;
                returnStatus     =
                    pSlapScoStandard->ExportMe
                        (
                            (ANSC_HANDLE)pSlapScoStandard,
                            &pExportParamList
                        );

                if ( (returnStatus == ANSC_STATUS_SUCCESS) && pExportParamList )
                {
                    for ( i = 0; i < pExportParamList->ParamCount; i++ )
                    {
                        SlapImcpGetVarSize((&pExportParamList->ParamArray[i]), ulImcpVarSize);

                        ulImcpVarCount     += 1;
                        ulImcpVarArraySize += ulImcpVarSize;
                    }

                    ulReturnFlag |= IMCP_SLAP_INVO_FLAG_returnedObjData;
                }
            }
        }

        if ( pOutputParamList )
        {
            for ( i = 0; i < pOutputParamList->ParamCount; i++ )
            {
                SlapImcpGetVarSize((&pOutputParamList->ParamArray[i]), ulImcpVarSize);

                /*
                 * To support the encapsulation of 'mobile' object, we need some special treatment
                 * for 'object' variables: in addition to transmit the object handle, we must also
                 * encapsulate the 'obj_name' and 'obj_type'.
                 */
                if ( pOutputParamList->ParamArray[i].Syntax == SLAP_VAR_SYNTAX_object )
                {
                    pSlapObjRecord = (PSLAP_OBJ_RECORD_OBJECT)pOutputParamList->ParamArray[i].Variant.varObject;

                    if ( pSlapObjRecord )
                    {
                        pSlapSrvComponent = (PSLAP_SRV_COMPONENT_OBJECT)pSlapObjRecord->hSlapSrvComponent;
                        pObjName          = pSlapSrvComponent->ObjName;
                        ulObjType         = pSlapSrvComponent->ObjType;

                        ulImcpVarSize += sizeof(ULONG) + AnscSizeOfString(pObjName) + 1;
                    }
                }

                ulImcpVarCount     += 1;
                ulImcpVarArraySize += ulImcpVarSize;
            }

            ulReturnFlag |= IMCP_SLAP_INVO_FLAG_outputParamList;
        }
    }
    else
    {
        ulImcpVarArraySize = 0;
    }

    if ( TRUE )
    {
        ulBufferSize   = sizeof(IMCP_SLAP_ANSWER) + sizeof(IMCP_SLAP_INVO_ANSWER);
        ulBufferSize  += ulImcpVarArraySize;
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
            pImcpSlapInvoAnswer = (PIMCP_SLAP_INVO_ANSWER)ImcpSlapAnswerGetData(pImcpSlapAnswer);
            ulAnswerDataSize    = (ULONG                 )sizeof(IMCP_SLAP_INVO_ANSWER);
            ulAnswerDataSize   += (ULONG                 )ulImcpVarArraySize;
            ulAnswerBuffSize    = (ULONG                 )sizeof(IMCP_SLAP_ANSWER) + ulAnswerDataSize;
        }
    }

    if ( TRUE )
    {
        ImcpSlapAnswerSetSize(pImcpSlapAnswer, ulAnswerDataSize);
    }

    if ( TRUE )
    {
        ImcpSlapInvoAnswerSetStatusCode  (pImcpSlapInvoAnswer, (ULONG)asStatusCode);
        ImcpSlapInvoAnswerSetReturnFlag  (pImcpSlapInvoAnswer, ulReturnFlag       );
        ImcpSlapInvoAnswerSetVarArraySize(pImcpSlapInvoAnswer, ulImcpVarArraySize );

        if ( (asStatusCode       == ANSC_STATUS_SUCCESS) &&
             (ulImcpVarArraySize >  0                  ) )
        {
            pImcpSlapVar = ImcpSlapInvoAnswerGetFirstVar(pImcpSlapInvoAnswer);

            if ( pReturnedVar )
            {
                SlapConvToImcpVar(pReturnedVar, pImcpSlapVar);

                /*
                 * To support the encapsulation of 'mobile' object, we need some special treatment
                 * for 'object' variables: in addition to transmit the object handle, we must also
                 * encapsulate the 'obj_name' and 'obj_type'.
                 */
                if ( pReturnedVar->Syntax == SLAP_VAR_SYNTAX_object )
                {
                    pSlapObjRecord = (PSLAP_OBJ_RECORD_OBJECT)pReturnedVar->Variant.varObject;

                    if ( pSlapObjRecord )
                    {
                        pSlapSrvComponent = (PSLAP_SRV_COMPONENT_OBJECT)pSlapObjRecord->hSlapSrvComponent;
                        pObjName          = pSlapSrvComponent->ObjName;
                        ulObjType         = pSlapSrvComponent->ObjType;

                        ImcpSlapVarSetFlags    (pImcpSlapVar, IMCP_SLAP_VAR_FLAG_objType | IMCP_SLAP_VAR_FLAG_objName);
                        ImcpSlapVarAppendUlong (pImcpSlapVar, ulObjType                                              );
                        ImcpSlapVarAppendString(pImcpSlapVar, pObjName                                               );
                    }
                }

                pImcpSlapVar = ImcpSlapVarGetNextVar(pImcpSlapVar);

                /*
                 * We can only transfer the object data when there's no output parameters. The
                 * current encoding/decoding mechanism in IMCP is not sufficient to support auto-
                 * transfer for object or object-array variables included in the output parameter
                 * list.
                 */
                if ( bAutoExportObj && pExportParamList )
                {
                    for ( i = 0; i < pExportParamList->ParamCount; i++ )
                    {
                        SlapConvToImcpVar((&pExportParamList->ParamArray[i]), pImcpSlapVar);

                        pImcpSlapVar = ImcpSlapVarGetNextVar(pImcpSlapVar);
                    }
                }
            }

            if ( pOutputParamList )
            {
                for ( i = 0; i < pOutputParamList->ParamCount; i++ )
                {
                    SlapConvToImcpVar((&pOutputParamList->ParamArray[i]), pImcpSlapVar);

                    /*
                     * To support the encapsulation of 'mobile' object, we need some special treat-
                     * ment for 'object' variables: in addition to transmit the object handle, we
                     * must also encapsulate the 'obj_name' and 'obj_type'.
                     */
                    if ( pOutputParamList->ParamArray[i].Syntax == SLAP_VAR_SYNTAX_object )
                    {
                        pSlapObjRecord = (PSLAP_OBJ_RECORD_OBJECT)pOutputParamList->ParamArray[i].Variant.varObject;

                        if ( pSlapObjRecord )
                        {
                            pSlapSrvComponent = (PSLAP_SRV_COMPONENT_OBJECT)pSlapObjRecord->hSlapSrvComponent;
                            pObjName          = pSlapSrvComponent->ObjName;
                            ulObjType         = pSlapSrvComponent->ObjType;

                            ImcpSlapVarSetFlags    (pImcpSlapVar, IMCP_SLAP_VAR_FLAG_objType | IMCP_SLAP_VAR_FLAG_objName);
                            ImcpSlapVarAppendUlong (pImcpSlapVar, ulObjType                                              );
                            ImcpSlapVarAppendString(pImcpSlapVar, pObjName                                               );
                        }
                    }

                    pImcpSlapVar = ImcpSlapVarGetNextVar(pImcpSlapVar);
                }
            }
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

    if ( pReturnedVar )
    {
        SlapFreeVariable(pReturnedVar);
    }

    if ( pOutputParamList )
    {
        SlapFreeParamList(pOutputParamList);
    }

    if ( pInputParamList )
    {
        SlapFreeParamList(pInputParamList);
    }

    if ( pExportParamList )
    {
        SlapFreeParamList(pExportParamList);
    }

EXIT1:

    if ( pMethodName )
    {
        AnscFreeMemory(pMethodName);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLsoProcessCallAcqo
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
SlapLsoProcessCallAcqo
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
    UNREFERENCED_PARAMETER(hLpcPartyAddr);
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PSLAP_LOAM_SERVER_OBJECT        pMyObject           = (PSLAP_LOAM_SERVER_OBJECT  )hThisObject;
    PSLAP_UOA_INTERFACE             pSlapUoaIf          = (PSLAP_UOA_INTERFACE       )pMyObject->hSlapUoaIf;
    PIMCP_SLAP_ACQO_CALL            pImcpSlapAcqoCall   = (PIMCP_SLAP_ACQO_CALL      )pInputBuffer;
    void*                           pMessageBuffer      = (void*                     )NULL;
    ULONG                           ulBufferSize        = (ULONG                     )0;
    PIMCP_SLAP_ANSWER               pImcpSlapAnswer     = (PIMCP_SLAP_ANSWER         )NULL;
    PIMCP_SLAP_ACQO_ANSWER          pImcpSlapAcqoAnswer = (PIMCP_SLAP_ACQO_ANSWER    )NULL;
    ULONG                           ulAnswerBuffSize    = (ULONG                     )0;
    ULONG                           ulAnswerDataSize    = (ULONG                     )0;
    char*                           pObjectName         = (char*                     )NULL;
    ANSC_HANDLE                     hSlapContainer      = (ANSC_HANDLE               )NULL;
    ANSC_HANDLE                     hSlapObject         = (ANSC_HANDLE               )NULL;

    *ppOutputBuffer = NULL;
    *pulOutputSize  = 0;
    *pulErrorCode   = ANSC_LPC_ERROR_noError;

    if ( ulInputSize <= sizeof(IMCP_SLAP_ACQO_CALL) )
    {
        *pulErrorCode = ANSC_LPC_ERROR_invalidReqData;

        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        hSlapContainer = (ANSC_HANDLE)ImcpSlapAcqoCallGetContainerId(pImcpSlapAcqoCall);
        pObjectName    = (char*      )AnscAllocateMemory(ulInputSize - sizeof(IMCP_SLAP_ACQO_CALL) + 1);

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
                    ImcpSlapAcqoCallGetObjectName(pImcpSlapAcqoCall),
                    ulInputSize - sizeof(IMCP_SLAP_ACQO_CALL)
                );
        }
    }

    hSlapObject =
        pSlapUoaIf->AcqObjectAccess
            (
                pSlapUoaIf->hOwnerContext,
                hSlapContainer,
                pObjectName
            );

    if ( TRUE )
    {
        ulBufferSize   = sizeof(IMCP_SLAP_ANSWER) + sizeof(IMCP_SLAP_ACQO_ANSWER);
        ulBufferSize  += IMCP_REPLY_BUFFER_OVERHEAD;
        pMessageBuffer = AnscAllocateMemory(ulBufferSize);

        if ( !pMessageBuffer )
        {
            *pulErrorCode = ANSC_LPC_ERROR_resources;
            returnStatus  = ANSC_STATUS_RESOURCES;

            goto  EXIT1;
        }
        else
        {
            pImcpSlapAnswer     = (PIMCP_SLAP_ANSWER     )((ULONG)pMessageBuffer + IMCP_REPLY_BUFFER_OVERHEAD);
            pImcpSlapAcqoAnswer = (PIMCP_SLAP_ACQO_ANSWER)ImcpSlapAnswerGetData(pImcpSlapAnswer);
            ulAnswerDataSize    = (ULONG                 )sizeof(IMCP_SLAP_ACQO_ANSWER);
            ulAnswerBuffSize    = (ULONG                 )sizeof(IMCP_SLAP_ANSWER) + ulAnswerDataSize;
        }
    }

    if ( TRUE )
    {
        ImcpSlapAnswerSetSize(pImcpSlapAnswer, ulAnswerDataSize);
    }

    if ( TRUE )
    {
        ImcpSlapAcqoAnswerSetObjectId(pImcpSlapAcqoAnswer, (ULONG)hSlapObject);
    }

    *ppOutputBuffer = pImcpSlapAnswer;
    *pulOutputSize  = ulAnswerBuffSize;
    *pulErrorCode   = ANSC_LPC_ERROR_noError;
    returnStatus    = ANSC_STATUS_SUCCESS;

    goto  EXIT1;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

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
        SlapLsoProcessCallRelo
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
SlapLsoProcessCallRelo
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
    UNREFERENCED_PARAMETER(hLpcPartyAddr);
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PSLAP_LOAM_SERVER_OBJECT        pMyObject           = (PSLAP_LOAM_SERVER_OBJECT  )hThisObject;
    PSLAP_UOA_INTERFACE             pSlapUoaIf          = (PSLAP_UOA_INTERFACE       )pMyObject->hSlapUoaIf;
    PIMCP_SLAP_RELO_CALL            pImcpSlapReloCall   = (PIMCP_SLAP_RELO_CALL      )pInputBuffer;
    void*                           pMessageBuffer      = (void*                     )NULL;
    ULONG                           ulBufferSize        = (ULONG                     )0;
    PIMCP_SLAP_ANSWER               pImcpSlapAnswer     = (PIMCP_SLAP_ANSWER         )NULL;
    PIMCP_SLAP_RELO_ANSWER          pImcpSlapReloAnswer = (PIMCP_SLAP_RELO_ANSWER    )NULL;
    ULONG                           ulAnswerBuffSize    = (ULONG                     )0;
    ULONG                           ulAnswerDataSize    = (ULONG                     )0;
    ANSC_STATUS                     asStatusCode        = (ANSC_STATUS               )ANSC_STATUS_SUCCESS;
    ANSC_HANDLE                     hSlapObject         = (ANSC_HANDLE               )NULL;

    *ppOutputBuffer = NULL;
    *pulOutputSize  = 0;
    *pulErrorCode   = ANSC_LPC_ERROR_noError;

    if ( ulInputSize != sizeof(IMCP_SLAP_RELO_CALL) )
    {
        *pulErrorCode = ANSC_LPC_ERROR_invalidReqData;

        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        hSlapObject = (ANSC_HANDLE)ImcpSlapReloCallGetObjectId(pImcpSlapReloCall);
    }

    asStatusCode =
        pSlapUoaIf->RelObjectAccess
            (
                pSlapUoaIf->hOwnerContext,
                hSlapObject
            );

    if ( TRUE )
    {
        ulBufferSize   = sizeof(IMCP_SLAP_ANSWER) + sizeof(IMCP_SLAP_RELO_ANSWER);
        ulBufferSize  += IMCP_REPLY_BUFFER_OVERHEAD;
        pMessageBuffer = AnscAllocateMemory(ulBufferSize);

        if ( !pMessageBuffer )
        {
            *pulErrorCode = ANSC_LPC_ERROR_resources;
            returnStatus  = ANSC_STATUS_RESOURCES;

            goto  EXIT1;
        }
        else
        {
            pImcpSlapAnswer     = (PIMCP_SLAP_ANSWER     )((ULONG)pMessageBuffer + IMCP_REPLY_BUFFER_OVERHEAD);
            pImcpSlapReloAnswer = (PIMCP_SLAP_RELO_ANSWER)ImcpSlapAnswerGetData(pImcpSlapAnswer);
            ulAnswerDataSize    = (ULONG                 )sizeof(IMCP_SLAP_RELO_ANSWER);
            ulAnswerBuffSize    = (ULONG                 )sizeof(IMCP_SLAP_ANSWER) + ulAnswerDataSize;
        }
    }

    if ( TRUE )
    {
        ImcpSlapAnswerSetSize(pImcpSlapAnswer, ulAnswerDataSize);
    }

    if ( TRUE )
    {
        ImcpSlapReloAnswerSetStatusCode(pImcpSlapReloAnswer, (ULONG)asStatusCode);
    }

    *ppOutputBuffer = pImcpSlapAnswer;
    *pulOutputSize  = ulAnswerBuffSize;
    *pulErrorCode   = ANSC_LPC_ERROR_noError;
    returnStatus    = ANSC_STATUS_SUCCESS;

    goto  EXIT1;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    return  returnStatus;
}
