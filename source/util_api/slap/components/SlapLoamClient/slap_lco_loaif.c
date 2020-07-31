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

    module:	slap_lco_loaif.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced interface functions
        of the Slap Loam Client Object.

        *   SlapLcoLoaAcqContainerAccess
        *   SlapLcoLoaRelContainerAccess
        *   SlapLcoLoaLocateObject
        *   SlapLcoLoaDeleteObject
        *   SlapLcoLoaInvokeObject
        *   SlapLcoLoaAcqObjectAccess
        *   SlapLcoLoaRelObjectAccess
        *   SlapLcoLoaRelObjectAccess2
        *   SlapLcoLoaFlushAllObjects
        *   SlapLcoLoaEnrollMobileObject
        *   SlapLcoLoaUnbindMobileObject
        *   SlapLcoLoaVerifyMobileObject

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/30/03    initial revision.

**********************************************************************/


#include "slap_lco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapLcoLoaAcqContainerAccess
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 lpc_party_addr,
                char*                       container_name
            );

    description:

        This function is called to acquire access to a runtime
        container Object maintains by the specified the LPC party.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 lpc_party_addr
                Specifies the address of the LPC party to which the
                request is to be sent.

                char*                       container_name
                Specifies the container name to be matched.

    return:     object handle.

**********************************************************************/

ANSC_HANDLE
SlapLcoLoaAcqContainerAccess
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 lpc_party_addr,
        char*                       container_name
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PSLAP_LOAM_CLIENT_OBJECT        pMyObject           = (PSLAP_LOAM_CLIENT_OBJECT  )hThisObject;
    PANSC_LPC_CONNECTOR_OBJECT      pAnscLpcConnector   = (PANSC_LPC_CONNECTOR_OBJECT)pMyObject->hAnscLpcConnector;
    void*                           pMessageBuffer      = (void*                     )NULL;
    ULONG                           ulBufferSize        = (ULONG                     )0;
    PIMCP_SLAP_CALL                 pImcpSlapCall       = (PIMCP_SLAP_CALL           )NULL;
    PIMCP_SLAP_ACQC_CALL            pImcpSlapAcqcCall   = (PIMCP_SLAP_ACQC_CALL      )NULL;
    ULONG                           ulCallBuffSize      = (ULONG                     )0;
    ULONG                           ulCallDataSize      = (ULONG                     )0;
    PANSC_BUFFER_DESCRIPTOR         pImcpSlapAnswerBdo  = (PANSC_BUFFER_DESCRIPTOR   )NULL;
    PIMCP_SLAP_ANSWER               pImcpSlapAnswer     = (PIMCP_SLAP_ANSWER         )NULL;
    PIMCP_SLAP_ACQC_ANSWER          pImcpSlapAcqcAnswer = (PIMCP_SLAP_ACQC_ANSWER    )NULL;
    ULONG                           ulLpcErrorCode      = (ULONG                     )ANSC_LPC_ERROR_noError;
    ANSC_HANDLE                     hLpcSlapContainer   = (ANSC_HANDLE               )NULL;

    if ( !pMyObject->bActive )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else if ( !lpc_party_addr || !container_name )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        ulBufferSize   = sizeof(IMCP_SLAP_CALL) + sizeof(IMCP_SLAP_ACQC_CALL);
        ulBufferSize  += AnscSizeOfString(container_name);
        ulBufferSize  += IMCP_REQUEST_BUFFER_OVERHEAD;
        pMessageBuffer = AnscAllocateMemory(ulBufferSize);

        if ( !pMessageBuffer )
        {
            return  (ANSC_HANDLE)NULL;
        }
        else
        {
            pImcpSlapCall     = (PIMCP_SLAP_CALL     )((ULONG)pMessageBuffer + IMCP_REQUEST_BUFFER_OVERHEAD);
            pImcpSlapAcqcCall = (PIMCP_SLAP_ACQC_CALL)ImcpSlapCallGetData(pImcpSlapCall);
            ulCallDataSize    = (ULONG               )sizeof(IMCP_SLAP_ACQC_CALL) + AnscSizeOfString(container_name);
            ulCallBuffSize    = (ULONG               )sizeof(IMCP_SLAP_CALL) + ulCallDataSize;
        }
    }

    if ( TRUE )
    {
        ImcpSlapCallSetType(pImcpSlapCall, IMCP_SLAP_CALL_TYPE_ACQC);
        ImcpSlapCallSetSize(pImcpSlapCall, ulCallDataSize          );
    }

    if ( TRUE )
    {
        ImcpSlapAcqcCallSetContainerType(pImcpSlapAcqcCall, 0             );
        ImcpSlapAcqcCallSetContainerName(pImcpSlapAcqcCall, container_name);
    }

    returnStatus =
        pAnscLpcConnector->ImcCallRequest2
            (
                (ANSC_HANDLE)pAnscLpcConnector,
                lpc_party_addr,
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
        goto  EXIT1;
    }
    else if ( AnscBdoGetBlockSize(pImcpSlapAnswerBdo) == 0 )
    {
        goto  EXIT2;
    }
    else
    {
        pImcpSlapAnswer     = (PIMCP_SLAP_ANSWER     )AnscBdoGetBlock(pImcpSlapAnswerBdo);
        pImcpSlapAcqcAnswer = (PIMCP_SLAP_ACQC_ANSWER)ImcpSlapAnswerGetData(pImcpSlapAnswer);
    }

    if ( ImcpSlapAnswerGetSize(pImcpSlapAnswer) != sizeof(IMCP_SLAP_ACQC_ANSWER) )
    {
        goto  EXIT2;
    }
    else
    {
        hLpcSlapContainer = (ANSC_HANDLE)ImcpSlapAcqcAnswerGetContainerId(pImcpSlapAcqcAnswer);
    }


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

    return  hLpcSlapContainer;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLcoLoaRelContainerAccess
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 lpc_party_addr,
                ANSC_HANDLE                 hSlapContainer
            );

    description:

        This function is called to release access to a runtime
        container Object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 lpc_party_addr
                Specifies the address of the LPC party to which the
                request is to be sent.

                ANSC_HANDLE                 hSlapContainer
                Specifies the slap container object returned by the
                previous CreateContainer() or AcqContainerAccess() call.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLcoLoaRelContainerAccess
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 lpc_party_addr,
        ANSC_HANDLE                 hSlapContainer
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PSLAP_LOAM_CLIENT_OBJECT        pMyObject           = (PSLAP_LOAM_CLIENT_OBJECT  )hThisObject;
    PANSC_LPC_CONNECTOR_OBJECT      pAnscLpcConnector   = (PANSC_LPC_CONNECTOR_OBJECT)pMyObject->hAnscLpcConnector;
    void*                           pMessageBuffer      = (void*                     )NULL;
    ULONG                           ulBufferSize        = (ULONG                     )0;
    PIMCP_SLAP_CALL                 pImcpSlapCall       = (PIMCP_SLAP_CALL           )NULL;
    PIMCP_SLAP_RELC_CALL            pImcpSlapRelcCall   = (PIMCP_SLAP_RELC_CALL      )NULL;
    ULONG                           ulCallBuffSize      = (ULONG                     )0;
    ULONG                           ulCallDataSize      = (ULONG                     )0;
    PANSC_BUFFER_DESCRIPTOR         pImcpSlapAnswerBdo  = (PANSC_BUFFER_DESCRIPTOR   )NULL;
    PIMCP_SLAP_ANSWER               pImcpSlapAnswer     = (PIMCP_SLAP_ANSWER         )NULL;
    PIMCP_SLAP_RELC_ANSWER          pImcpSlapRelcAnswer = (PIMCP_SLAP_RELC_ANSWER    )NULL;
    ULONG                           ulLpcErrorCode      = (ULONG                     )ANSC_LPC_ERROR_noError;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else if ( !lpc_party_addr || !hSlapContainer )
    {
        return  ANSC_STATUS_BAD_PARAMETER;
    }
    else
    {
        ulBufferSize   = sizeof(IMCP_SLAP_CALL) + sizeof(IMCP_SLAP_RELC_CALL);
        ulBufferSize  += IMCP_REQUEST_BUFFER_OVERHEAD;
        pMessageBuffer = AnscAllocateMemory(ulBufferSize);

        if ( !pMessageBuffer )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pImcpSlapCall     = (PIMCP_SLAP_CALL     )((ULONG)pMessageBuffer + IMCP_REQUEST_BUFFER_OVERHEAD);
            pImcpSlapRelcCall = (PIMCP_SLAP_RELC_CALL)ImcpSlapCallGetData(pImcpSlapCall);
            ulCallDataSize    = (ULONG               )sizeof(IMCP_SLAP_RELC_CALL);
            ulCallBuffSize    = (ULONG               )sizeof(IMCP_SLAP_CALL) + ulCallDataSize;
        }
    }

    if ( TRUE )
    {
        ImcpSlapCallSetType(pImcpSlapCall, IMCP_SLAP_CALL_TYPE_RELC);
        ImcpSlapCallSetSize(pImcpSlapCall, ulCallDataSize          );
    }

    if ( TRUE )
    {
        ImcpSlapRelcCallSetContainerId(pImcpSlapRelcCall, (ULONG)hSlapContainer);
    }

    returnStatus =
        pAnscLpcConnector->ImcCallRequest2
            (
                (ANSC_HANDLE)pAnscLpcConnector,
                lpc_party_addr,
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
        pImcpSlapRelcAnswer = (PIMCP_SLAP_RELC_ANSWER)ImcpSlapAnswerGetData(pImcpSlapAnswer);
    }

    if ( ImcpSlapAnswerGetSize(pImcpSlapAnswer) != sizeof(IMCP_SLAP_RELC_ANSWER) )
    {
        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT2;
    }
    else
    {
        returnStatus = (ANSC_STATUS)ImcpSlapRelcAnswerGetStatusCode(pImcpSlapRelcAnswer);
    }


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

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLcoLoaLocateObject
            (
                ANSC_HANDLE                 hThisObject,
                char*                       obj_name,
                char**                      lpc_party_name,
                ANSC_HANDLE*                lpc_party_addr
            );

    description:

        This function is called to locate a runtime Obj Record Object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       obj_name
                Specifies the object name to be located.

                char**                      lpc_party_name
                Specifies the LPC party name to be returned.

                ANSC_HANDLE*                lpc_party_addr
                Specifies the address of the LPC party to be retrieved.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLcoLoaLocateObject
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name,
        char**                      lpc_party_name,
        ANSC_HANDLE*                lpc_party_addr
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PSLAP_LOAM_CLIENT_OBJECT        pMyObject           = (PSLAP_LOAM_CLIENT_OBJECT  )hThisObject;
    PANSC_LPC_CONNECTOR_OBJECT      pAnscLpcConnector   = (PANSC_LPC_CONNECTOR_OBJECT)pMyObject->hAnscLpcConnector;
    PSLAP_LOA_OBJECT_STUB           pObjectStub         = (PSLAP_LOA_OBJECT_STUB     )NULL;
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
    PIMCP_PARTY_INFO                pRepliedPartyInfo   = (PIMCP_PARTY_INFO          )NULL;
    ULONG                           ulPartyInfoSize     = (ULONG                     )0;
    char*                           pPartyName          = (char*                     )NULL;
    PANSC_LPC_PARTY_ADDR            pPartyAddr          = (PANSC_LPC_PARTY_ADDR      )NULL;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else if ( !obj_name || !lpc_party_name || !lpc_party_addr )
    {
        return  ANSC_STATUS_BAD_PARAMETER;
    }
    else
    {
        pObjectStub =
            (PSLAP_LOA_OBJECT_STUB)pMyObject->GetObjectStubByName
                (
                    (ANSC_HANDLE)pMyObject,
                    obj_name
                );
    }

    if ( pObjectStub )
    {
        *lpc_party_name = AnscCloneString(pObjectStub->LpcPartyName);
        *lpc_party_addr = pObjectStub->hLpcPartyAddr;

        SlapUnlockLoaObjectStub(pObjectStub);

        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        *lpc_party_name = NULL;
        *lpc_party_addr = NULL;

        ulBufferSize   = sizeof(IMCP_SLAP_CALL) + sizeof(IMCP_SLAP_LOCO_CALL);
        ulBufferSize  += AnscSizeOfString(obj_name);
        ulBufferSize  += IMCP_REQUEST_BUFFER_OVERHEAD;
        pMessageBuffer = AnscAllocateMemory(ulBufferSize);

        if ( !pMessageBuffer )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pImcpSlapCall     = (PIMCP_SLAP_CALL     )((ULONG)pMessageBuffer + IMCP_REQUEST_BUFFER_OVERHEAD);
            pImcpSlapLocoCall = (PIMCP_SLAP_LOCO_CALL)ImcpSlapCallGetData(pImcpSlapCall);
            ulCallDataSize    = (ULONG               )sizeof(IMCP_SLAP_LOCO_CALL) + AnscSizeOfString(obj_name);
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
        ImcpSlapLocoCallSetObjectType(pImcpSlapLocoCall, 0       );
        ImcpSlapLocoCallSetObjectName(pImcpSlapLocoCall, obj_name);
    }

    returnStatus =
        pAnscLpcConnector->ImcCallRequest
            (
                (ANSC_HANDLE)pAnscLpcConnector,
                ANSC_LPC_MANAGER_PARTY_NAME,
                pImcpSlapCall,
                ulCallBuffSize,
                (ANSC_HANDLE*)&pImcpSlapAnswerBdo,
                &ulLpcErrorCode,
                0       /* using the default timeout value */
            );

    if ( (returnStatus   != ANSC_STATUS_SUCCESS   ) ||
         (ulLpcErrorCode != ANSC_LPC_ERROR_noError) )
    {
       /* AnscTrace
            (
                "\n !! SlapLoaIf.LocateObject() failed: obj_name = %s !!\n",
                obj_name
            );
	*/
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
        pRepliedPartyInfo = (PIMCP_PARTY_INFO)ImcpSlapLocoAnswerGetPartyInfo (pImcpSlapLocoAnswer);
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
        ulPartyInfoSize = ImcpSlapAnswerGetSize(pImcpSlapAnswer) - sizeof(IMCP_SLAP_LOCO_ANSWER);
        pPartyName      = (char*)AnscAllocateMemory(ulPartyInfoSize - sizeof(IMCP_PARTY_INFO) + 1);

        if ( !pPartyName )
        {
            returnStatus = ANSC_STATUS_RESOURCES;

            goto  EXIT2;
        }
        else
        {
            AnscCopyMemory
                (
                    pPartyName,
                    ImcpPartyInfoGetPartyName(pRepliedPartyInfo),
                    ulPartyInfoSize - sizeof(IMCP_PARTY_INFO)
                );
        }

        pPartyAddr =
            pAnscLpcConnector->GetPartyAddr
                (
                    (ANSC_HANDLE)pAnscLpcConnector,
                    pPartyName
                );

        if ( !pPartyAddr )
        {
            pPartyAddr = (PANSC_LPC_PARTY_ADDR)AnscAllocateMemory(sizeof(ANSC_LPC_PARTY_ADDR));

            if ( !pPartyAddr )
            {
                returnStatus = ANSC_STATUS_RESOURCES;

                goto  EXIT3;
            }
            else
            {
                AnscCopyMemory
                    (
                        pPartyAddr->PartyName,
                        ImcpPartyInfoGetPartyName(pRepliedPartyInfo),
                        ulPartyInfoSize - sizeof(IMCP_PARTY_INFO)
                    );

                pPartyAddr->StructSize      = sizeof(ANSC_LPC_PARTY_ADDR);
                pPartyAddr->RefCount        = 0;
                pPartyAddr->Timestamp       = 0;
                pPartyAddr->PartyState      = 0;
                pPartyAddr->PartyType       = ANSC_LPC_PARTY_TYPE_client;
                pPartyAddr->PartyAddr.Value = ImcpPartyInfoGetPartyAddrValue(pRepliedPartyInfo);
                pPartyAddr->PartyProtocol   = ImcpPartyInfoGetPartyProtocol (pRepliedPartyInfo);
                pPartyAddr->PartyPort       = ImcpPartyInfoGetPartyPort     (pRepliedPartyInfo);
                pPartyAddr->PartySocket     = (ANSC_HANDLE)NULL;
                pPartyAddr->PartyReserved   = (ANSC_HANDLE)NULL;

                returnStatus =
                    pAnscLpcConnector->AddPartyAddr
                        (
                            (ANSC_HANDLE)pAnscLpcConnector,
                            (ANSC_HANDLE)pPartyAddr
                        );

                if ( returnStatus != ANSC_STATUS_SUCCESS )
                {
                    AnscFreeMemory(pPartyAddr);

                    pPartyAddr = NULL;
                }
            }
        }

        *lpc_party_name = pPartyName;
        *lpc_party_addr = (ANSC_HANDLE)pPartyAddr;
    }

    returnStatus = ANSC_STATUS_SUCCESS;

    goto  EXIT2;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT3:

    if ( pPartyName )
    {
        AnscFreeMemory(pPartyName);
    }

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

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLcoLoaDeleteObject
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 lpc_party_addr,
                ANSC_HANDLE                 hSlapObject
            );

    description:

        This function is called to delete a runtime Obj Record Object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 lpc_party_addr
                Specifies the address of the LPC party to which the
                request is to be sent.

                ANSC_HANDLE                 hSlapObject
                Specifies the slap obj record object returned by the
                previous AcqObjectAccess() call.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLcoLoaDeleteObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 lpc_party_addr,
        ANSC_HANDLE                 hSlapObject
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PSLAP_LOAM_CLIENT_OBJECT        pMyObject           = (PSLAP_LOAM_CLIENT_OBJECT   )hThisObject;
    PSLAP_LOAM_CLIENT_PROPERTY      pProperty           = (PSLAP_LOAM_CLIENT_PROPERTY )&pMyObject->Property;
    PSLAP_ENV_CONTROLLER_OBJECT     pSlapEnvController  = (PSLAP_ENV_CONTROLLER_OBJECT)pMyObject->hSlapEnvController;
    PANSC_LPC_CONNECTOR_OBJECT      pAnscLpcConnector   = (PANSC_LPC_CONNECTOR_OBJECT )pMyObject->hAnscLpcConnector;
    PSLAP_UOA_INTERFACE             pSlapUoaIf          = (PSLAP_UOA_INTERFACE        )NULL;
    PSLAP_OBJ_AGENT_OBJECT          pSlapObjAgent       = (PSLAP_OBJ_AGENT_OBJECT     )hSlapObject;
    ANSC_HANDLE                     hClonedObj          = (ANSC_HANDLE                )NULL;
    ULONG                           ulOaoObjMode        = (ULONG                      )0;
    void*                           pMessageBuffer      = (void*                      )NULL;
    ULONG                           ulBufferSize        = (ULONG                      )0;
    PIMCP_SLAP_CALL                 pImcpSlapCall       = (PIMCP_SLAP_CALL            )NULL;
    PIMCP_SLAP_DELO_CALL            pImcpSlapDeloCall   = (PIMCP_SLAP_DELO_CALL       )NULL;
    ULONG                           ulCallBuffSize      = (ULONG                      )0;
    ULONG                           ulCallDataSize      = (ULONG                      )0;
    PANSC_BUFFER_DESCRIPTOR         pImcpSlapAnswerBdo  = (PANSC_BUFFER_DESCRIPTOR    )NULL;
    PIMCP_SLAP_ANSWER               pImcpSlapAnswer     = (PIMCP_SLAP_ANSWER          )NULL;
    PIMCP_SLAP_DELO_ANSWER          pImcpSlapDeloAnswer = (PIMCP_SLAP_DELO_ANSWER     )NULL;
    ULONG                           ulLpcErrorCode      = (ULONG                      )ANSC_LPC_ERROR_noError;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else if ( !lpc_party_addr || !hSlapObject )
    {
        return  ANSC_STATUS_BAD_PARAMETER;
    }

    /*
     * If the mobility support has been turned on, SlapLoamClient MUST encapsulate the original
     * SLAP object in a SlapObjAgent object.
     */
    if ( pProperty->bMobilityOn )
    {
        pSlapUoaIf   = (PSLAP_UOA_INTERFACE)pSlapEnvController->GetSlapUoaIf((ANSC_HANDLE)pSlapEnvController);
        ulOaoObjMode = pSlapObjAgent->ObjMode;
        hSlapObject  = pSlapObjAgent->hOriginalObj;
        hClonedObj   = pSlapObjAgent->hClonedObj;

        if ( !hSlapObject )
        {
            return  ANSC_STATUS_INTERNAL_ERROR;
        }
        else
        {
            pSlapObjAgent->Remove((ANSC_HANDLE)pSlapObjAgent);
        }

        switch ( ulOaoObjMode )
        {
            case    SLAP_OAO_OBJ_MODE_relay :

                    break;

            case    SLAP_OAO_OBJ_MODE_clone :

                    if ( hClonedObj )
                    {
                        returnStatus =
                            pSlapUoaIf->DeleteObject
                                (
                                    pSlapUoaIf->hOwnerContext,
                                    hClonedObj
                                );
                    }

                    break;

            case    SLAP_OAO_OBJ_MODE_local :

                    returnStatus =
                        pSlapUoaIf->DeleteObject
                            (
                                pSlapUoaIf->hOwnerContext,
                                hSlapObject
                            );

                    return  ANSC_STATUS_SUCCESS;

                    break;

            default :

                    break;
        }
    }

    if ( TRUE )
    {
        ulBufferSize   = sizeof(IMCP_SLAP_CALL) + sizeof(IMCP_SLAP_DELO_CALL);
        ulBufferSize  += IMCP_REQUEST_BUFFER_OVERHEAD;
        pMessageBuffer = AnscAllocateMemory(ulBufferSize);

        if ( !pMessageBuffer )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pImcpSlapCall     = (PIMCP_SLAP_CALL     )((ULONG)pMessageBuffer + IMCP_REQUEST_BUFFER_OVERHEAD);
            pImcpSlapDeloCall = (PIMCP_SLAP_DELO_CALL)ImcpSlapCallGetData(pImcpSlapCall);
            ulCallDataSize    = (ULONG               )sizeof(IMCP_SLAP_DELO_CALL);
            ulCallBuffSize    = (ULONG               )sizeof(IMCP_SLAP_CALL) + ulCallDataSize;
        }
    }

    if ( TRUE )
    {
        ImcpSlapCallSetType(pImcpSlapCall, IMCP_SLAP_CALL_TYPE_DELO);
        ImcpSlapCallSetSize(pImcpSlapCall, ulCallDataSize          );
    }

    if ( TRUE )
    {
        ImcpSlapDeloCallSetObjectId(pImcpSlapDeloCall, (ULONG)hSlapObject);
    }

    returnStatus =
        pAnscLpcConnector->ImcCallRequest2
            (
                (ANSC_HANDLE)pAnscLpcConnector,
                lpc_party_addr,
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
        pImcpSlapDeloAnswer = (PIMCP_SLAP_DELO_ANSWER)ImcpSlapAnswerGetData(pImcpSlapAnswer);
    }

    if ( ImcpSlapAnswerGetSize(pImcpSlapAnswer) != sizeof(IMCP_SLAP_DELO_ANSWER) )
    {
        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT2;
    }
    else
    {
        returnStatus = (ANSC_STATUS)ImcpSlapDeloAnswerGetStatusCode(pImcpSlapDeloAnswer);
    }


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

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLcoLoaInvokeObject
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 lpc_party_addr,
                ANSC_HANDLE                 hSlapObject,
                char*                       method_name,
                SLAP_PARAMETER_LIST*        params_in,
                SLAP_PARAMETER_LIST**       params_out,
                SLAP_VARIABLE**             return_var,
                BOOL                        bTranslate,
                ULONG                       ulTimeout
            );

    description:

        This function is called to invoke a runtime Srv Component
        Object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 lpc_party_addr
                Specifies the address of the LPC party to which the
                request is to be sent.

                ANSC_HANDLE                 hSlapObject
                Specifies the slap obj record object returned by the
                previous AcqObjectAccess() call.

                char*                       method_name
                Specifies the object method to be invoked.

                SLAP_PARAMETER_LIST*        params_in
                Specifies the list of input parameters to be passed.

                SLAP_PARAMETER_LIST**       params_out
                Specifies the list of output parameters to be returned.

                SLAP_VARIABLE**             return_var
                Specifies the variable to be returned.

                BOOL                        bTranslate
                Specifies whether the LOA translation should be applied.

                ULONG                       ulTimeout
                Specifies the timeout value for this call.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLcoLoaInvokeObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 lpc_party_addr,
        ANSC_HANDLE                 hSlapObject,
        char*                       method_name,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST**       params_out,
        SLAP_VARIABLE**             return_var,
        BOOL                        bTranslate,
        ULONG                       ulTimeout
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PSLAP_LOAM_CLIENT_OBJECT        pMyObject           = (PSLAP_LOAM_CLIENT_OBJECT   )hThisObject;
    PSLAP_LOAM_CLIENT_PROPERTY      pProperty           = (PSLAP_LOAM_CLIENT_PROPERTY )&pMyObject->Property;
    PSLAP_ENV_CONTROLLER_OBJECT     pSlapEnvController  = (PSLAP_ENV_CONTROLLER_OBJECT)pMyObject->hSlapEnvController;
    PANSC_LPC_CONNECTOR_OBJECT      pAnscLpcConnector   = (PANSC_LPC_CONNECTOR_OBJECT )pMyObject->hAnscLpcConnector;
    PSLAP_UOA_INTERFACE             pSlapUoaIf          = (PSLAP_UOA_INTERFACE        )NULL;
    PSLAP_OBJ_AGENT_OBJECT          pSlapObjAgent       = (PSLAP_OBJ_AGENT_OBJECT     )hSlapObject;
    ULONG                           ulOaoObjMode        = (ULONG                      )0;
    void*                           pMessageBuffer      = (void*                      )NULL;
    ULONG                           ulBufferSize        = (ULONG                      )0;
    ULONG                           ulImcpVarArraySize  = (ULONG                      )0;
    ULONG                           ulImcpVarSize       = (ULONG                      )0;
    ULONG                           ulImcpVarCount      = (ULONG                      )0;
    ULONG                           ulImcpVarIndex      = (ULONG                      )0;
    PIMCP_SLAP_CALL                 pImcpSlapCall       = (PIMCP_SLAP_CALL            )NULL;
    PIMCP_SLAP_INVO_CALL            pImcpSlapInvoCall   = (PIMCP_SLAP_INVO_CALL       )NULL;
    PIMCP_SLAP_VARIABLE             pImcpSlapVar        = (PIMCP_SLAP_VARIABLE        )NULL;
    ULONG                           ulCallBuffSize      = (ULONG                      )0;
    ULONG                           ulCallDataSize      = (ULONG                      )0;
    PANSC_BUFFER_DESCRIPTOR         pImcpSlapAnswerBdo  = (PANSC_BUFFER_DESCRIPTOR    )NULL;
    PIMCP_SLAP_ANSWER               pImcpSlapAnswer     = (PIMCP_SLAP_ANSWER          )NULL;
    PIMCP_SLAP_INVO_ANSWER          pImcpSlapInvoAnswer = (PIMCP_SLAP_INVO_ANSWER     )NULL;
    ULONG                           ulInvoReturnFlag    = (ULONG                      )0;
    BOOL                            bValidSlapAnswer    = (BOOL                       )TRUE;
    PSLAP_PARAMETER_LIST            pOutputParamList    = (PSLAP_PARAMETER_LIST       )NULL;
    PSLAP_VARIABLE                  pReturnedVar        = (PSLAP_VARIABLE             )NULL;
    ULONG                           ulLpcErrorCode      = (ULONG                      )ANSC_LPC_ERROR_noError;
    ULONG                           i                   = 0;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else if ( !lpc_party_addr || !hSlapObject || !method_name || !params_in || !params_out || !return_var )
    {
        return  ANSC_STATUS_BAD_PARAMETER;
    }
    else
    {
        *params_out = NULL;
        *return_var = NULL;
    }

    /*
     * If the mobility support has been turned on, SlapLoamClient MUST encapsulate the original
     * SLAP object in a SlapObjAgent object.
     */
    if ( bTranslate && pProperty->bMobilityOn )
    {
        pSlapUoaIf   = (PSLAP_UOA_INTERFACE)pSlapEnvController->GetSlapUoaIf((ANSC_HANDLE)pSlapEnvController);
        ulOaoObjMode = pSlapObjAgent->ObjMode;
        hSlapObject  = pSlapObjAgent->hOriginalObj;

        if ( !hSlapObject )
        {
            return  ANSC_STATUS_INTERNAL_ERROR;
        }

        switch ( ulOaoObjMode )
        {
            case    SLAP_OAO_OBJ_MODE_relay :

                    break;

            case    SLAP_OAO_OBJ_MODE_clone :
            case    SLAP_OAO_OBJ_MODE_local :

                    if ( params_in )
                    {
                        returnStatus =
                            pMyObject->LoaToUoaParamList
                                (
                                    (ANSC_HANDLE)pMyObject,
                                    params_in
                                );

                        if ( returnStatus != ANSC_STATUS_SUCCESS )
                        {
                            return  returnStatus;
                        }
                    }

                    if ( ulOaoObjMode == SLAP_OAO_OBJ_MODE_clone )
                    {
                        pSlapObjAgent->bDirty = TRUE;

                        returnStatus =
                            pSlapUoaIf->InvokeObject
                                (
                                    pSlapUoaIf->hOwnerContext,
                                    pSlapObjAgent->hClonedObj,
                                    method_name,
                                    params_in,
                                    params_out,
                                    return_var
                                );
                    }
                    else
                    {
                        returnStatus =
                            pSlapUoaIf->InvokeObject
                                (
                                    pSlapUoaIf->hOwnerContext,
                                    hSlapObject,
                                    method_name,
                                    params_in,
                                    params_out,
                                    return_var
                                );
                    }

                    if ( returnStatus != ANSC_STATUS_SUCCESS )
                    {
                        return  returnStatus;
                    }
                    else
                    {
                        if ( params_out && *params_out )
                        {
                            returnStatus =
                                pMyObject->UoaToLoaParamList
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        *params_out
                                    );

                            if ( returnStatus != ANSC_STATUS_SUCCESS )
                            {
                                return  returnStatus;
                            }
                        }

                        if ( return_var && *return_var )
                        {
                            returnStatus =
                                pMyObject->UoaToLoaVariable
                                    (
                                        (ANSC_HANDLE)pMyObject,
                                        *return_var
                                    );

                            if ( returnStatus != ANSC_STATUS_SUCCESS )
                            {
                                return  returnStatus;
                            }
                        }
                    }

                    return  returnStatus;

                    break;

            default :

                    break;
        }
    }

    if ( TRUE )
    {
        for ( i = 0; i < params_in->ParamCount; i++ )
        {
            SlapImcpGetVarSize((&params_in->ParamArray[i]), ulImcpVarSize);

            ulImcpVarArraySize += ulImcpVarSize;
        }

        ulBufferSize   = sizeof(IMCP_SLAP_CALL) + sizeof(IMCP_SLAP_INVO_CALL);
        ulBufferSize  += AnscSizeOfString(method_name) + ulImcpVarArraySize;
        ulBufferSize  += IMCP_REQUEST_BUFFER_OVERHEAD;
        pMessageBuffer = AnscAllocateMemory(ulBufferSize);

        if ( !pMessageBuffer )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pImcpSlapCall     = (PIMCP_SLAP_CALL     )((ULONG)pMessageBuffer + IMCP_REQUEST_BUFFER_OVERHEAD);
            pImcpSlapInvoCall = (PIMCP_SLAP_INVO_CALL)ImcpSlapCallGetData(pImcpSlapCall);
            ulCallDataSize    = (ULONG               )sizeof(IMCP_SLAP_INVO_CALL) + AnscSizeOfString(method_name) + ulImcpVarArraySize;
            ulCallBuffSize    = (ULONG               )sizeof(IMCP_SLAP_CALL) + ulCallDataSize;
        }
    }

    if ( TRUE )
    {
        ImcpSlapCallSetType(pImcpSlapCall, IMCP_SLAP_CALL_TYPE_INVO);
        ImcpSlapCallSetSize(pImcpSlapCall, ulCallDataSize          );
    }

    if ( TRUE )
    {
        ImcpSlapInvoCallSetObjectId      (pImcpSlapInvoCall, (ULONG)hSlapObject           );
        ImcpSlapInvoCallSetMethodNameSize(pImcpSlapInvoCall, AnscSizeOfString(method_name));
        ImcpSlapInvoCallSetVarArraySize  (pImcpSlapInvoCall, ulImcpVarArraySize           );
        ImcpSlapInvoCallSetMethodName    (pImcpSlapInvoCall, method_name                  );

        if ( TRUE )
        {
            pImcpSlapVar = NULL;

            for ( i = 0; i < params_in->ParamCount; i++ )
            {
                if ( pImcpSlapVar )
                {
                    pImcpSlapVar = ImcpSlapVarGetNextVar(pImcpSlapVar);
                }
                else
                {
                    pImcpSlapVar = ImcpSlapInvoCallGetFirstVar(pImcpSlapInvoCall);
                }

                SlapConvToImcpVar((&params_in->ParamArray[i]), pImcpSlapVar);
            }
        }
    }

    /*
     * While SLAP_LOA transparently brige the method invocation to another process via IMCP, it
     * also introduces some significant overhead. The transmission overhead is very noticeable when
     * an operation requires several remote invocations. To improve the performance for remote
     * object access, we have thought of two approaches:
     *
     *      (1) minimize the number of IMCP packets that have to be transmitted by pack-
     *          ing multiple SLAP calls into one message. This approach sounds like a
     *          feasible solution since remote invocations usually are made in a bunch
     *          especially for policy management tasks. However, This approach requires
     *          awareness at application level and a sophisticated mechanism to control
     *          the logic flow of the method access, which is something we don't want to
     *          get into at this moment;
     *
     *      (2) convert as many SLAP_LOA calls into SLAP_UOA calls as we can by creating
     *          a mirrored SLAP object in the local UOA environment for every 'mobile'
     *          LOA object. A 'mobile' SLAP object is such an object that any of its ins-
     *          tance can be freely moved from one UOA process to another without compro-
     *          mising its functionality. Because SLAP objects are implemented in C, the
     *          mobility only applies to the data member of the object (unlike Java).
     *
     * It appears that (2) is a good solution provided that we can encapsulate most SLAP objects as
     * 'mobile' objects. Another factor that may affect the performance of mobile objects is the
     * average number of invocations associated with each 'mobile' object. This number has to be
     * bigger than 3 in order to justify this solution. We SHOULD provide an option to allow the
     * upper layer to selectively enable/disable 'mobility' for certain objects.
     */
    if ( bTranslate && pProperty->bMobilityOn )
    {
        returnStatus =
            pMyObject->LoaToLpcImcpInvoCall
                (
                    (ANSC_HANDLE)pMyObject,
                    lpc_party_addr,
                    pImcpSlapInvoCall
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            goto  EXIT1;
        }
    }

    returnStatus =
        pAnscLpcConnector->ImcCallRequest2
            (
                (ANSC_HANDLE)pAnscLpcConnector,
                lpc_party_addr,
                pImcpSlapCall,
                ulCallBuffSize,
                (ANSC_HANDLE*)&pImcpSlapAnswerBdo,
                &ulLpcErrorCode,
                ulTimeout
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
        pImcpSlapInvoAnswer = (PIMCP_SLAP_INVO_ANSWER)ImcpSlapAnswerGetData(pImcpSlapAnswer);
    }

    if ( ImcpSlapAnswerGetSize(pImcpSlapAnswer) < sizeof(IMCP_SLAP_INVO_ANSWER) )
    {
        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT2;
    }
    else
    {
        ImcpSlapInvoAnswerValidate(pImcpSlapInvoAnswer, ImcpSlapAnswerGetSize(pImcpSlapAnswer), bValidSlapAnswer);
    }

    if ( !bValidSlapAnswer )
    {
        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT2;
    }
    else
    {
        returnStatus     = (ANSC_STATUS)ImcpSlapInvoAnswerGetStatusCode(pImcpSlapInvoAnswer);
        ulInvoReturnFlag = (ULONG      )ImcpSlapInvoAnswerGetReturnFlag(pImcpSlapInvoAnswer);
        ulImcpVarIndex   = 0;

        ImcpSlapInvoAnswerGetVarCount(pImcpSlapInvoAnswer, ulImcpVarCount);
    }

    if ( ulImcpVarCount == 0 )
    {
        goto  EXIT2;
    }

    /*
     * While SLAP_LOA transparently brige the method invocation to another process via IMCP, it
     * also introduces some significant overhead. The transmission overhead is very noticeable when
     * an operation requires several remote invocations. To improve the performance for remote
     * object access, we have thought of two approaches:
     *
     *      (1) minimize the number of IMCP packets that have to be transmitted by pack-
     *          ing multiple SLAP calls into one message. This approach sounds like a
     *          feasible solution since remote invocations usually are made in a bunch
     *          especially for policy management tasks. However, This approach requires
     *          awareness at application level and a sophisticated mechanism to control
     *          the logic flow of the method access, which is something we don't want to
     *          get into at this moment;
     *
     *      (2) convert as many SLAP_LOA calls into SLAP_UOA calls as we can by creating
     *          a mirrored SLAP object in the local UOA environment for every 'mobile'
     *          LOA object. A 'mobile' SLAP object is such an object that any of its ins-
     *          tance can be freely moved from one UOA process to another without compro-
     *          mising its functionality. Because SLAP objects are implemented in C, the
     *          mobility only applies to the data member of the object (unlike Java).
     *
     * It appears that (2) is a good solution provided that we can encapsulate most SLAP objects as
     * 'mobile' objects. Another factor that may affect the performance of mobile objects is the
     * average number of invocations associated with each 'mobile' object. This number has to be
     * bigger than 3 in order to justify this solution. We SHOULD provide an option to allow the
     * upper layer to selectively enable/disable 'mobility' for certain objects.
     */
    if ( bTranslate && pProperty->bMobilityOn )
    {
        returnStatus =
            pMyObject->LpcToLoaImcpInvoAnswer
                (
                    (ANSC_HANDLE)pMyObject,
                    lpc_party_addr,
                    pImcpSlapInvoAnswer
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            goto  EXIT2;
        }
    }

    /*
     * The responder may choose return any combination of the 'returned variable' and the 'output
     * parameter list'. We use the 'ReturnFlag' to differentiate between different situations. Note
     * that the caller is responsible for releasing the memory for the 'returned variable' and the
     * output parameter list'.
     */
    if ( ulInvoReturnFlag & IMCP_SLAP_INVO_FLAG_returnedValue )
    {
        pImcpSlapVar = ImcpSlapInvoAnswerGetFirstVar(pImcpSlapInvoAnswer);
        pReturnedVar = (PSLAP_VARIABLE)AnscAllocateMemory(sizeof(SLAP_VARIABLE));

        if ( !pReturnedVar )
        {
            returnStatus = ANSC_STATUS_RESOURCES;

            goto  EXIT2;
        }
        else
        {
            SlapInitVariable(pReturnedVar);

            pReturnedVar->Name        = NULL;
            pReturnedVar->ContentType = 0;
            pReturnedVar->UsageType   = 0;

            SlapConvFromImcpVar(pImcpSlapVar, pReturnedVar);
        }

        ulImcpVarIndex++;
    }
    else
    {
        pReturnedVar = NULL;
    }

    if ( ulInvoReturnFlag & IMCP_SLAP_INVO_FLAG_outputParamList )
    {
        pOutputParamList = (PSLAP_PARAMETER_LIST)AnscAllocateMemory(sizeof(SLAP_PARAMETER_LIST));

        if ( !pOutputParamList )
        {
            returnStatus = ANSC_STATUS_RESOURCES;

            goto  EXIT3;
        }

        for ( i = ulImcpVarIndex; i < ulImcpVarCount; i++ )
        {
            ImcpSlapInvoAnswerGetVarByIndex(pImcpSlapInvoAnswer, pImcpSlapVar, i);

            SlapConvFromImcpVar(pImcpSlapVar, (&pOutputParamList->ParamArray[i - ulImcpVarIndex]));

            pOutputParamList->ParamCount++;
        }
    }
    else
    {
        pOutputParamList = NULL;
    }

    *params_out = pOutputParamList;
    *return_var = pReturnedVar;

    goto  EXIT2;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT3:

    if ( pReturnedVar )
    {
        SlapFreeVariable(pReturnedVar);
    }

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

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapLcoLoaAcqObjectAccess
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 lpc_party_addr,
                ANSC_HANDLE                 hSlapContainer,
                char*                       obj_name
            );

    description:

        This function is called to acquire access to a runtime Obj
        Record Object by matching the obj_name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 lpc_party_addr
                Specifies the address of the LPC party to which the
                request is to be sent.

                ANSC_HANDLE                 hSlapContainer
                Specifies the container object returned by the previous
                CreateContainer() call.

                char*                       obj_name
                Specifies the object name to be matched.

    return:     object handle.

**********************************************************************/

ANSC_HANDLE
SlapLcoLoaAcqObjectAccess
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 lpc_party_addr,
        ANSC_HANDLE                 hSlapContainer,
        char*                       obj_name
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PSLAP_LOAM_CLIENT_OBJECT        pMyObject           = (PSLAP_LOAM_CLIENT_OBJECT  )hThisObject;
    PSLAP_LOAM_CLIENT_PROPERTY      pProperty           = (PSLAP_LOAM_CLIENT_PROPERTY)&pMyObject->Property;
    PANSC_LPC_CONNECTOR_OBJECT      pAnscLpcConnector   = (PANSC_LPC_CONNECTOR_OBJECT)pMyObject->hAnscLpcConnector;
    PSLAP_OBJ_AGENT_OBJECT          pSlapObjAgent       = (PSLAP_OBJ_AGENT_OBJECT    )NULL;
    PSLAP_LOA_OBJECT_STUB           pObjectStub         = (PSLAP_LOA_OBJECT_STUB     )NULL;
    PANSC_LPC_PARTY_ADDR            pPartyAddr          = (PANSC_LPC_PARTY_ADDR      )lpc_party_addr;
    void*                           pMessageBuffer      = (void*                     )NULL;
    ULONG                           ulBufferSize        = (ULONG                     )0;
    PIMCP_SLAP_CALL                 pImcpSlapCall       = (PIMCP_SLAP_CALL           )NULL;
    PIMCP_SLAP_ACQO_CALL            pImcpSlapAcqoCall   = (PIMCP_SLAP_ACQO_CALL      )NULL;
    ULONG                           ulCallBuffSize      = (ULONG                     )0;
    ULONG                           ulCallDataSize      = (ULONG                     )0;
    PANSC_BUFFER_DESCRIPTOR         pImcpSlapAnswerBdo  = (PANSC_BUFFER_DESCRIPTOR   )NULL;
    PIMCP_SLAP_ANSWER               pImcpSlapAnswer     = (PIMCP_SLAP_ANSWER         )NULL;
    PIMCP_SLAP_ACQO_ANSWER          pImcpSlapAcqoAnswer = (PIMCP_SLAP_ACQO_ANSWER    )NULL;
    ULONG                           ulLpcErrorCode      = (ULONG                     )ANSC_LPC_ERROR_noError;
    ANSC_HANDLE                     hLpcSlapObject      = (ANSC_HANDLE               )NULL;

    if ( !pMyObject->bActive )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else if ( !lpc_party_addr || !obj_name )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pObjectStub =
            (PSLAP_LOA_OBJECT_STUB)pMyObject->GetObjectStubByName
                (
                    (ANSC_HANDLE)pMyObject,
                    obj_name
                );
    }

    if ( pObjectStub )
    {
        hLpcSlapObject = pObjectStub->hLoaSlapObject;

        pObjectStub->RefCount    += 1;
        pObjectStub->LastAccessAt = AnscGetTickInSeconds();

        SlapUnlockLoaObjectStub(pObjectStub);

        /*
         * If the mobility support has been turned on, SlapLoamClient MUST encapsulate the original
         * SLAP object in a SlapObjAgent object.
         */
        if ( pProperty->bMobilityOn )
        {
            pSlapObjAgent =
                (PSLAP_OBJ_AGENT_OBJECT)SlapCreateObjAgent
                    (
                        pMyObject->hContainerContext,
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)NULL
                    );

            if ( !pSlapObjAgent )
            {
                return  (ANSC_HANDLE)NULL;
            }
            else
            {
                pSlapObjAgent->ObjType      = 0;
                pSlapObjAgent->ObjMode      = SLAP_OAO_OBJ_MODE_relay;
                pSlapObjAgent->hOriginalObj = hLpcSlapObject;
                pSlapObjAgent->hClonedObj   = (ANSC_HANDLE)NULL;
                pSlapObjAgent->bDirty       = FALSE;

                pSlapObjAgent->SetObjName((ANSC_HANDLE)pSlapObjAgent, obj_name);
            }

            hLpcSlapObject = (ANSC_HANDLE)pSlapObjAgent;
        }

        return  hLpcSlapObject;
    }
    else
    {
        ulBufferSize   = sizeof(IMCP_SLAP_CALL) + sizeof(IMCP_SLAP_ACQO_CALL);
        ulBufferSize  += AnscSizeOfString(obj_name);
        ulBufferSize  += IMCP_REQUEST_BUFFER_OVERHEAD;
        pMessageBuffer = AnscAllocateMemory(ulBufferSize);

        if ( !pMessageBuffer )
        {
            return  (ANSC_HANDLE)NULL;
        }
        else
        {
            pImcpSlapCall     = (PIMCP_SLAP_CALL     )((ULONG)pMessageBuffer + IMCP_REQUEST_BUFFER_OVERHEAD);
            pImcpSlapAcqoCall = (PIMCP_SLAP_ACQO_CALL)ImcpSlapCallGetData(pImcpSlapCall);
            ulCallDataSize    = (ULONG               )sizeof(IMCP_SLAP_ACQO_CALL) + AnscSizeOfString(obj_name);
            ulCallBuffSize    = (ULONG               )sizeof(IMCP_SLAP_CALL) + ulCallDataSize;
        }
    }

    if ( TRUE )
    {
        ImcpSlapCallSetType(pImcpSlapCall, IMCP_SLAP_CALL_TYPE_ACQO);
        ImcpSlapCallSetSize(pImcpSlapCall, ulCallDataSize          );
    }

    if ( TRUE )
    {
        ImcpSlapAcqoCallSetContainerId(pImcpSlapAcqoCall, (ULONG)hSlapContainer);
        ImcpSlapAcqoCallSetObjectName (pImcpSlapAcqoCall, obj_name             );
    }

    returnStatus =
        pAnscLpcConnector->ImcCallRequest2
            (
                (ANSC_HANDLE)pAnscLpcConnector,
                lpc_party_addr,
                pImcpSlapCall,
                ulCallBuffSize,
                (ANSC_HANDLE*)&pImcpSlapAnswerBdo,
                &ulLpcErrorCode,
                0       /* using the default timeout value */
            );

    if ( (returnStatus   != ANSC_STATUS_SUCCESS   ) ||
         (ulLpcErrorCode != ANSC_LPC_ERROR_noError) )
    {/*
        AnscTrace
            (
                "\n !! SlapLoaIf.AcqObjectAccess() failed: party_name = %s, obj_name = %s !!\n",
                pPartyAddr->PartyName,
                obj_name
            );
	*/
        goto  EXIT1;
    }
    else if ( !pImcpSlapAnswerBdo )
    {
        goto  EXIT1;
    }
    else if ( AnscBdoGetBlockSize(pImcpSlapAnswerBdo) == 0 )
    {
        goto  EXIT2;
    }
    else
    {
        pImcpSlapAnswer     = (PIMCP_SLAP_ANSWER     )AnscBdoGetBlock(pImcpSlapAnswerBdo);
        pImcpSlapAcqoAnswer = (PIMCP_SLAP_ACQO_ANSWER)ImcpSlapAnswerGetData(pImcpSlapAnswer);
    }

    if ( ImcpSlapAnswerGetSize(pImcpSlapAnswer) != sizeof(IMCP_SLAP_ACQO_ANSWER) )
    {
        goto  EXIT2;
    }
    else
    {
        hLpcSlapObject = (ANSC_HANDLE)ImcpSlapAcqoAnswerGetObjectId(pImcpSlapAcqoAnswer);
    }

    if ( TRUE )
    {
        returnStatus =
            pMyObject->AddObjectStub
                (
                    (ANSC_HANDLE)pMyObject,
                    hLpcSlapObject,
                    obj_name,
                    pPartyAddr->PartyName,
                    lpc_party_addr
                );
    }

    /*
     * If the mobility support has been turned on, SlapLoamClient MUST encapsulate the original
     * SLAP object in a SlapObjAgent object.
     */
    if ( pProperty->bMobilityOn )
    {
        pSlapObjAgent =
            (PSLAP_OBJ_AGENT_OBJECT)SlapCreateObjAgent
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pSlapObjAgent )
        {
            /*
             * We SHOULD release the object either explicitly via a remote method invocation or by
             * setting the reference count to zero; otherwise, this object will never be released
             * because the reference count will never reach zero.
             */
            pObjectStub =
                (PSLAP_LOA_OBJECT_STUB)pMyObject->GetObjectStubByHandle
                    (
                        (ANSC_HANDLE)pMyObject,
                        hLpcSlapObject
                    );

            if ( pObjectStub )
            {
                pObjectStub->RefCount    -= 1;
                pObjectStub->LastAccessAt = AnscGetTickInSeconds();

                SlapUnlockLoaObjectStub(pObjectStub);
            }

            hLpcSlapObject = (ANSC_HANDLE)NULL;

            goto  EXIT2;
        }
        else
        {
            pSlapObjAgent->ObjType      = 0;
            pSlapObjAgent->ObjMode      = SLAP_OAO_OBJ_MODE_relay;
            pSlapObjAgent->hOriginalObj = hLpcSlapObject;
            pSlapObjAgent->hClonedObj   = (ANSC_HANDLE)NULL;
            pSlapObjAgent->bDirty       = FALSE;

            pSlapObjAgent->SetObjName((ANSC_HANDLE)pSlapObjAgent, obj_name);
        }

        hLpcSlapObject = (ANSC_HANDLE)pSlapObjAgent;
    }


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

    return  hLpcSlapObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLcoLoaRelObjectAccess
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 lpc_party_addr,
                ANSC_HANDLE                 hSlapObject
            );

    description:

        This function is called to release access to a runtime Obj
        Record Object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 lpc_party_addr
                Specifies the address of the LPC party to which the
                request is to be sent.

                ANSC_HANDLE                 hSlapObject
                Specifies the slap obj record object returned by the
                previous AcqObjectAccess() call.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLcoLoaRelObjectAccess
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 lpc_party_addr,
        ANSC_HANDLE                 hSlapObject
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PSLAP_LOAM_CLIENT_OBJECT        pMyObject           = (PSLAP_LOAM_CLIENT_OBJECT   )hThisObject;
    PSLAP_LOAM_CLIENT_PROPERTY      pProperty           = (PSLAP_LOAM_CLIENT_PROPERTY )&pMyObject->Property;
    PSLAP_ENV_CONTROLLER_OBJECT     pSlapEnvController  = (PSLAP_ENV_CONTROLLER_OBJECT)pMyObject->hSlapEnvController;
    PANSC_LPC_CONNECTOR_OBJECT      pAnscLpcConnector   = (PANSC_LPC_CONNECTOR_OBJECT )pMyObject->hAnscLpcConnector;
    PSLAP_UOA_INTERFACE             pSlapUoaIf          = (PSLAP_UOA_INTERFACE        )NULL;
    PSLAP_OBJ_AGENT_OBJECT          pSlapObjAgent       = (PSLAP_OBJ_AGENT_OBJECT     )hSlapObject;
    ANSC_HANDLE                     hClonedObj          = (ANSC_HANDLE                )NULL;
    ULONG                           ulOaoObjMode        = (ULONG                      )0;
    PSLAP_LOA_OBJECT_STUB           pObjectStub         = (PSLAP_LOA_OBJECT_STUB      )NULL;
    void*                           pMessageBuffer      = (void*                      )NULL;
    ULONG                           ulBufferSize        = (ULONG                      )0;
    PIMCP_SLAP_CALL                 pImcpSlapCall       = (PIMCP_SLAP_CALL            )NULL;
    PIMCP_SLAP_RELO_CALL            pImcpSlapReloCall   = (PIMCP_SLAP_RELO_CALL       )NULL;
    ULONG                           ulCallBuffSize      = (ULONG                      )0;
    ULONG                           ulCallDataSize      = (ULONG                      )0;
    PANSC_BUFFER_DESCRIPTOR         pImcpSlapAnswerBdo  = (PANSC_BUFFER_DESCRIPTOR    )NULL;
    PIMCP_SLAP_ANSWER               pImcpSlapAnswer     = (PIMCP_SLAP_ANSWER          )NULL;
    PIMCP_SLAP_RELO_ANSWER          pImcpSlapReloAnswer = (PIMCP_SLAP_RELO_ANSWER     )NULL;
    ULONG                           ulLpcErrorCode      = (ULONG                      )ANSC_LPC_ERROR_noError;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else if ( !lpc_party_addr || !hSlapObject )
    {
        return  ANSC_STATUS_BAD_PARAMETER;
    }

    /*
     * If the mobility support has been turned on, SlapLoamClient MUST encapsulate the original
     * SLAP object in a SlapObjAgent object.
     */
    if ( pProperty->bMobilityOn )
    {
        pSlapUoaIf   = (PSLAP_UOA_INTERFACE)pSlapEnvController->GetSlapUoaIf((ANSC_HANDLE)pSlapEnvController);
        ulOaoObjMode = pSlapObjAgent->ObjMode;
        hSlapObject  = pSlapObjAgent->hOriginalObj;
        hClonedObj   = pSlapObjAgent->hClonedObj;

        if ( !hSlapObject )
        {
            return  ANSC_STATUS_INTERNAL_ERROR;
        }
        else
        {
            pSlapObjAgent->Remove((ANSC_HANDLE)pSlapObjAgent);
        }

        switch ( ulOaoObjMode )
        {
            case    SLAP_OAO_OBJ_MODE_relay :

                    break;

            case    SLAP_OAO_OBJ_MODE_clone :

                    /*
                     * We SHOULD never get here since the dynamically-created objects can only be
                     * deleted instead of being released. This will cause the cloned object to
                     * become an orphaned object, which will eventually be removed when the mobile
                     * container is deleted.
                     */
                    if ( hClonedObj )
                    {
                        returnStatus =
                            pSlapUoaIf->RelObjectAccess
                                (
                                    pSlapUoaIf->hOwnerContext,
                                    hClonedObj
                                );
                    }

                    break;

            case    SLAP_OAO_OBJ_MODE_local :

                    /*
                     * We SHOULD never get here since the dynamically-created objects can only be
                     * deleted instead of being released. This will cause the cloned object to
                     * become an orphaned object, which will eventually be removed when the mobile
                     * container is deleted.
                     */
                    returnStatus =
                        pSlapUoaIf->RelObjectAccess
                            (
                                pSlapUoaIf->hOwnerContext,
                                hSlapObject
                            );

                    return  ANSC_STATUS_SUCCESS;

                    break;

            default :

                    break;
        }
    }

    if ( TRUE )
    {
        pObjectStub =
            (PSLAP_LOA_OBJECT_STUB)pMyObject->GetObjectStubByHandle
                (
                    (ANSC_HANDLE)pMyObject,
                    hSlapObject
                );
    }

    if ( pObjectStub )
    {
        pObjectStub->RefCount    -= 1;
        pObjectStub->LastAccessAt = AnscGetTickInSeconds();

        SlapUnlockLoaObjectStub(pObjectStub);

        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        ulBufferSize   = sizeof(IMCP_SLAP_CALL) + sizeof(IMCP_SLAP_RELO_CALL);
        ulBufferSize  += IMCP_REQUEST_BUFFER_OVERHEAD;
        pMessageBuffer = AnscAllocateMemory(ulBufferSize);

        if ( !pMessageBuffer )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pImcpSlapCall     = (PIMCP_SLAP_CALL     )((ULONG)pMessageBuffer + IMCP_REQUEST_BUFFER_OVERHEAD);
            pImcpSlapReloCall = (PIMCP_SLAP_RELO_CALL)ImcpSlapCallGetData(pImcpSlapCall);
            ulCallDataSize    = (ULONG               )sizeof(IMCP_SLAP_RELO_CALL);
            ulCallBuffSize    = (ULONG               )sizeof(IMCP_SLAP_CALL) + ulCallDataSize;
        }
    }

    if ( TRUE )
    {
        ImcpSlapCallSetType(pImcpSlapCall, IMCP_SLAP_CALL_TYPE_RELO);
        ImcpSlapCallSetSize(pImcpSlapCall, ulCallDataSize          );
    }

    if ( TRUE )
    {
        ImcpSlapReloCallSetObjectId(pImcpSlapReloCall, (ULONG)hSlapObject);
    }

    returnStatus =
        pAnscLpcConnector->ImcCallRequest2
            (
                (ANSC_HANDLE)pAnscLpcConnector,
                lpc_party_addr,
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
        pImcpSlapReloAnswer = (PIMCP_SLAP_RELO_ANSWER)ImcpSlapAnswerGetData(pImcpSlapAnswer);
    }

    if ( ImcpSlapAnswerGetSize(pImcpSlapAnswer) != sizeof(IMCP_SLAP_RELO_ANSWER) )
    {
        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT2;
    }
    else
    {
        returnStatus = (ANSC_STATUS)ImcpSlapReloAnswerGetStatusCode(pImcpSlapReloAnswer);
    }


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

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLcoLoaRelObjectAccess2
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 lpc_party_addr,
                ANSC_HANDLE                 hSlapObject
            );

    description:

        This function is called to release access to a runtime Obj
        Record Object. This difference between this function and the
        above RelObjectAccess() is that this function is called
        internally to directly release a remote object. Hence no LOA
        translation or caching is applied.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 lpc_party_addr
                Specifies the address of the LPC party to which the
                request is to be sent.

                ANSC_HANDLE                 hSlapObject
                Specifies the slap obj record object returned by the
                previous AcqObjectAccess() call.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLcoLoaRelObjectAccess2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 lpc_party_addr,
        ANSC_HANDLE                 hSlapObject
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PSLAP_LOAM_CLIENT_OBJECT        pMyObject           = (PSLAP_LOAM_CLIENT_OBJECT   )hThisObject;
    PANSC_LPC_CONNECTOR_OBJECT      pAnscLpcConnector   = (PANSC_LPC_CONNECTOR_OBJECT )pMyObject->hAnscLpcConnector;
    void*                           pMessageBuffer      = (void*                      )NULL;
    ULONG                           ulBufferSize        = (ULONG                      )0;
    PIMCP_SLAP_CALL                 pImcpSlapCall       = (PIMCP_SLAP_CALL            )NULL;
    PIMCP_SLAP_RELO_CALL            pImcpSlapReloCall   = (PIMCP_SLAP_RELO_CALL       )NULL;
    ULONG                           ulCallBuffSize      = (ULONG                      )0;
    ULONG                           ulCallDataSize      = (ULONG                      )0;
    PANSC_BUFFER_DESCRIPTOR         pImcpSlapAnswerBdo  = (PANSC_BUFFER_DESCRIPTOR    )NULL;
    PIMCP_SLAP_ANSWER               pImcpSlapAnswer     = (PIMCP_SLAP_ANSWER          )NULL;
    PIMCP_SLAP_RELO_ANSWER          pImcpSlapReloAnswer = (PIMCP_SLAP_RELO_ANSWER     )NULL;
    ULONG                           ulLpcErrorCode      = (ULONG                      )ANSC_LPC_ERROR_noError;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else if ( !lpc_party_addr || !hSlapObject )
    {
        return  ANSC_STATUS_BAD_PARAMETER;
    }

    if ( TRUE )
    {
        ulBufferSize   = sizeof(IMCP_SLAP_CALL) + sizeof(IMCP_SLAP_RELO_CALL);
        ulBufferSize  += IMCP_REQUEST_BUFFER_OVERHEAD;
        pMessageBuffer = AnscAllocateMemory(ulBufferSize);

        if ( !pMessageBuffer )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pImcpSlapCall     = (PIMCP_SLAP_CALL     )((ULONG)pMessageBuffer + IMCP_REQUEST_BUFFER_OVERHEAD);
            pImcpSlapReloCall = (PIMCP_SLAP_RELO_CALL)ImcpSlapCallGetData(pImcpSlapCall);
            ulCallDataSize    = (ULONG               )sizeof(IMCP_SLAP_RELO_CALL);
            ulCallBuffSize    = (ULONG               )sizeof(IMCP_SLAP_CALL) + ulCallDataSize;
        }
    }

    if ( TRUE )
    {
        ImcpSlapCallSetType(pImcpSlapCall, IMCP_SLAP_CALL_TYPE_RELO);
        ImcpSlapCallSetSize(pImcpSlapCall, ulCallDataSize          );
    }

    if ( TRUE )
    {
        ImcpSlapReloCallSetObjectId(pImcpSlapReloCall, (ULONG)hSlapObject);
    }

    returnStatus =
        pAnscLpcConnector->ImcCallRequest2
            (
                (ANSC_HANDLE)pAnscLpcConnector,
                lpc_party_addr,
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
        pImcpSlapReloAnswer = (PIMCP_SLAP_RELO_ANSWER)ImcpSlapAnswerGetData(pImcpSlapAnswer);
    }

    if ( ImcpSlapAnswerGetSize(pImcpSlapAnswer) != sizeof(IMCP_SLAP_RELO_ANSWER) )
    {
        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT2;
    }
    else
    {
        returnStatus = (ANSC_STATUS)ImcpSlapReloAnswerGetStatusCode(pImcpSlapReloAnswer);
    }


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

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLcoLoaFlushAllObjects
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to release all the cached LOA objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLcoLoaFlushAllObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PSLAP_LOAM_CLIENT_OBJECT        pMyObject          = (PSLAP_LOAM_CLIENT_OBJECT   )hThisObject;

    pMyObject->DelAllObjectStubs((ANSC_HANDLE)pMyObject);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLcoLoaEnrollMobileObject
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hObjDescr
            );

    description:

        This function is called to register a object descriptor.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hObjDescr
                Specifies the object descriptor to be registered.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLcoLoaEnrollMobileObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjDescr
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PSLAP_LOAM_CLIENT_OBJECT        pMyObject          = (PSLAP_LOAM_CLIENT_OBJECT   )hThisObject;
    PSLAP_LOAM_CLIENT_PROPERTY      pProperty          = (PSLAP_LOAM_CLIENT_PROPERTY )&pMyObject->Property;
    PSLAP_ENV_CONTROLLER_OBJECT     pSlapEnvController = (PSLAP_ENV_CONTROLLER_OBJECT)pMyObject->hSlapEnvController;
    PSLAP_UOA_INTERFACE             pSlapUoaIf         = (PSLAP_UOA_INTERFACE        )NULL;

    if ( pProperty->bMobilityOn && pSlapEnvController )
    {
        pSlapUoaIf = (PSLAP_UOA_INTERFACE)pSlapEnvController->GetSlapUoaIf((ANSC_HANDLE)pSlapEnvController);

        returnStatus =
            pSlapUoaIf->EnrollObjDescriptor
                (
                    pSlapUoaIf->hOwnerContext,
                    hObjDescr
                );
    }
    else
    {
        returnStatus = ANSC_STATUS_UNAPPLICABLE;
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLcoLoaUnbindMobileObject
            (
                ANSC_HANDLE                 hThisObject,
                char*                       obj_name
            );

    description:

        This function is called to unbind a object descriptor.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       obj_name
                Specifies the obj_name to be unbound.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLcoLoaUnbindMobileObject
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PSLAP_LOAM_CLIENT_OBJECT        pMyObject          = (PSLAP_LOAM_CLIENT_OBJECT   )hThisObject;
    PSLAP_LOAM_CLIENT_PROPERTY      pProperty          = (PSLAP_LOAM_CLIENT_PROPERTY )&pMyObject->Property;
    PSLAP_ENV_CONTROLLER_OBJECT     pSlapEnvController = (PSLAP_ENV_CONTROLLER_OBJECT)pMyObject->hSlapEnvController;
    PSLAP_UOA_INTERFACE             pSlapUoaIf         = (PSLAP_UOA_INTERFACE        )NULL;

    if ( pProperty->bMobilityOn && pSlapEnvController )
    {
        pSlapUoaIf = (PSLAP_UOA_INTERFACE)pSlapEnvController->GetSlapUoaIf((ANSC_HANDLE)pSlapEnvController);

        returnStatus =
            pSlapUoaIf->UnbindObjDescriptor
                (
                    pSlapUoaIf->hOwnerContext,
                    obj_name
                );
    }
    else
    {
        returnStatus = ANSC_STATUS_UNAPPLICABLE;
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SlapLcoLoaVerifyMobileObject
            (
                ANSC_HANDLE                 hThisObject,
                char*                       obj_name
            );

    description:

        This function is called to verify a object descriptor.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       obj_name
                Specifies the obj_name to be unbound.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
SlapLcoLoaVerifyMobileObject
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    )
{
    PSLAP_LOAM_CLIENT_OBJECT        pMyObject          = (PSLAP_LOAM_CLIENT_OBJECT   )hThisObject;
    PSLAP_LOAM_CLIENT_PROPERTY      pProperty          = (PSLAP_LOAM_CLIENT_PROPERTY )&pMyObject->Property;
    PSLAP_ENV_CONTROLLER_OBJECT     pSlapEnvController = (PSLAP_ENV_CONTROLLER_OBJECT)pMyObject->hSlapEnvController;
    PSLAP_UOA_INTERFACE             pSlapUoaIf         = (PSLAP_UOA_INTERFACE        )NULL;
    BOOL                            bVerifyResult      = FALSE;

    if ( pProperty->bMobilityOn && pSlapEnvController )
    {
        pSlapUoaIf = (PSLAP_UOA_INTERFACE)pSlapEnvController->GetSlapUoaIf((ANSC_HANDLE)pSlapEnvController);

        bVerifyResult =
            pSlapUoaIf->VerifyObjDescriptor
                (
                    pSlapUoaIf->hOwnerContext,
                    obj_name
                );
    }
    else
    {
        bVerifyResult = FALSE;
    }

    return  bVerifyResult;
}
