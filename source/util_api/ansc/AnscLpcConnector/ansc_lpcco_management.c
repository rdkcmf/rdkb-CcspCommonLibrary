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

    module:	ansc_lpcco_management.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the Ansc Lpc Connector Object.

        *   AnscLpccoGetPartyCount
        *   AnscLpccoGetPartyArray
        *   AnscLpccoGetPartyAddr
        *   AnscLpccoGetPartyAddr2
        *   AnscLpccoAddPartyAddr
        *   AnscLpccoDelPartyAddr
        *   AnscLpccoDelAllPartyAddrs
        *   AnscLpccoGetPendingCall
        *   AnscLpccoPopPendingCall
        *   AnscLpccoAddPendingCall
        *   AnscLpccoDelPendingCall
        *   AnscLpccoPopAllPendingCalls
        *   AnscLpccoDelAllPendingCalls

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/24/03    initial revision.

**********************************************************************/


#include "ansc_lpcco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscLpccoGetPartyCount
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the number of parties.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     party count.

**********************************************************************/

ULONG
AnscLpccoGetPartyCount
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_LPC_CONNECTOR_OBJECT      pMyObject    = (PANSC_LPC_CONNECTOR_OBJECT)hThisObject;
    ULONG                           ulPartyCount = 0;
    ULONG                           i            = 0;

    AnscAcquireLock(&pMyObject->PartyTableLock);

    for ( i = 0; i < ANSC_LPCCO_PARTY_TABLE_SIZE; i++ )
    {
        ulPartyCount += AnscQueueQueryDepth(&pMyObject->PartyTable[i]);
    }

    AnscReleaseLock(&pMyObject->PartyTableLock);

    return  ulPartyCount;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoGetPartyArray
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE*                pPartyArray,
                PULONG                      pulCount
            );

    description:

        This function is called to retrieve the array of parties.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE*                pParytArray
                Specifies the party array to be returned.

                PULONG                      pulCount
                Specifies the number of parties to be returned.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoGetPartyArray
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE*                pPartyArray,
        PULONG                      pulCount
    )
{
    PANSC_LPC_CONNECTOR_OBJECT      pMyObject       = (PANSC_LPC_CONNECTOR_OBJECT)hThisObject;
    PANSC_LPC_PARTY_ADDR            pPartyAddrArray = (PANSC_LPC_PARTY_ADDR      )NULL;
    PANSC_LPC_PARTY_ADDR            pPartyAddr      = (PANSC_LPC_PARTY_ADDR      )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = (PSINGLE_LINK_ENTRY        )NULL;
    ULONG                           ulPartyMaxCount = (ULONG                     )pMyObject->GetPartyCount   ((ANSC_HANDLE)pMyObject);
    ULONG                           ulPartyAddrSize = (ULONG                     )pMyObject->GetPartyAddrSize((ANSC_HANDLE)pMyObject);
    ULONG                           ulPartyCount    = 0;
    ULONG                           i               = 0;

    if ( ulPartyMaxCount == 0 )
    {
        *pPartyArray = (ANSC_HANDLE)NULL;
        *pulCount    = 0;

        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pPartyAddrArray = (PANSC_LPC_PARTY_ADDR)AnscAllocateMemory(ulPartyAddrSize * ulPartyMaxCount);

        if ( !pPartyAddrArray )
        {
            return  ANSC_STATUS_RESOURCES;
        }
    }

    AnscAcquireLock(&pMyObject->PartyTableLock);

    for ( i = 0; i < ANSC_LPCCO_PARTY_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->PartyTable[i]);

        while ( pSLinkEntry )
        {
            pPartyAddr  = ACCESS_ANSC_LPC_PARTY_ADDR(pSLinkEntry);
            pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

            AnscCopyMemory
                (
                    (void*)((ULONG)pPartyAddrArray + ulPartyCount * ulPartyAddrSize),
                    pPartyAddr,
                    pPartyAddr->StructSize
                );

            ulPartyCount++;

            if ( ulPartyCount >= ulPartyMaxCount )
            {
                break;
            }
        }

        if ( ulPartyCount >= ulPartyMaxCount )
        {
            break;
        }
    }

    AnscReleaseLock(&pMyObject->PartyTableLock);

    *pPartyArray = (ANSC_HANDLE)pPartyAddrArray;
    *pulCount    = ulPartyCount;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscLpccoGetPartyAddr
            (
                ANSC_HANDLE                 hThisObject,
                char*                       party_name
            );

    description:

        This function is called to retrieve a party addr.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       party_name
                Specifies the party name to be matched.

    return:     party addr.

**********************************************************************/

ANSC_HANDLE
AnscLpccoGetPartyAddr
    (
        ANSC_HANDLE                 hThisObject,
        char*                       party_name
    )
{
    PANSC_LPC_CONNECTOR_OBJECT      pMyObject    = (PANSC_LPC_CONNECTOR_OBJECT)hThisObject;
    PANSC_LPC_PARTY_ADDR            pPartyAddr   = (PANSC_LPC_PARTY_ADDR      )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = (PSINGLE_LINK_ENTRY        )NULL;
    ULONG                           ulHashIndex  = AnscHashString(party_name, AnscSizeOfString(party_name), ANSC_LPCCO_PARTY_TABLE_SIZE);

    AnscAcquireLock(&pMyObject->PartyTableLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->PartyTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pPartyAddr  = ACCESS_ANSC_LPC_PARTY_ADDR(pSLinkEntry);
        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

        if ( strcasecmp(pPartyAddr->PartyName,party_name) == 0 )
        {
            AnscReleaseLock(&pMyObject->PartyTableLock);

            return  (ANSC_HANDLE)pPartyAddr;
        }
    }

    AnscReleaseLock(&pMyObject->PartyTableLock);

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscLpccoGetPartyAddr2
            (
                ANSC_HANDLE                 hThisObject,
                char*                       party_name
            );

    description:

        This function is called to retrieve a party addr. This call is
        different from GetPartyAddr() in that this function acquires a
        reference count.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       party_name
                Specifies the party name to be matched.

    return:     party addr.

**********************************************************************/

ANSC_HANDLE
AnscLpccoGetPartyAddr2
    (
        ANSC_HANDLE                 hThisObject,
        char*                       party_name
    )
{
    PANSC_LPC_CONNECTOR_OBJECT      pMyObject    = (PANSC_LPC_CONNECTOR_OBJECT)hThisObject;
    PANSC_LPC_PARTY_ADDR            pPartyAddr   = (PANSC_LPC_PARTY_ADDR      )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = (PSINGLE_LINK_ENTRY        )NULL;
    ULONG                           ulHashIndex  = AnscHashString(party_name, AnscSizeOfString(party_name), ANSC_LPCCO_PARTY_TABLE_SIZE);

    AnscAcquireLock(&pMyObject->PartyTableLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->PartyTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pPartyAddr  = ACCESS_ANSC_LPC_PARTY_ADDR(pSLinkEntry);
        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

        if ( strcasecmp(pPartyAddr->PartyName,party_name) == 0 )
        {
            pPartyAddr->RefCount++;

            AnscReleaseLock(&pMyObject->PartyTableLock);

            return  (ANSC_HANDLE)pPartyAddr;
        }
    }

    AnscReleaseLock(&pMyObject->PartyTableLock);

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoAddPartyAddr
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 party_addr
            );

    description:

        This function is called to add a party addr.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 party_addr
                Specifies the party addr to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoAddPartyAddr
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 party_addr
    )
{
    PANSC_LPC_CONNECTOR_OBJECT      pMyObject    = (PANSC_LPC_CONNECTOR_OBJECT)hThisObject;
    PANSC_LPC_PARTY_ADDR            pPartyAddr   = (PANSC_LPC_PARTY_ADDR      )party_addr;
    char*                           pPartyName   = (char*                     )pPartyAddr->PartyName;
    ULONG                           ulHashIndex  = AnscHashString(pPartyName, AnscSizeOfString(pPartyName), ANSC_LPCCO_PARTY_TABLE_SIZE);

    if ( pMyObject->GetPartyAddr
            (
                (ANSC_HANDLE)pMyObject,
                pPartyName
            ) != (ANSC_HANDLE)NULL )
    {
        AnscTrace
            (
                "[AnscLpccoAddPartyAddr] collison found, failed to add party <%s> from %d.%d.%d.%d (%d)\n",
                pPartyAddr->PartyName,
                pPartyAddr->PartyAddr.Dot[0],
                pPartyAddr->PartyAddr.Dot[1],
                pPartyAddr->PartyAddr.Dot[2],
                pPartyAddr->PartyAddr.Dot[3],
                pPartyAddr->PartyPort
            );

        return  ANSC_STATUS_COLLISION;
    }

    AnscAcquireLock   (&pMyObject->PartyTableLock);
    AnscQueuePushEntry(&pMyObject->PartyTable[ulHashIndex], &pPartyAddr->Linkage);
    AnscReleaseLock   (&pMyObject->PartyTableLock);

    AnscTrace
        (
            "[AnscLpccoAddPartyAddr] party <%s> from %d.%d.%d.%d (%d) has been added.\n",
            pPartyAddr->PartyName,
            pPartyAddr->PartyAddr.Dot[0],
            pPartyAddr->PartyAddr.Dot[1],
            pPartyAddr->PartyAddr.Dot[2],
            pPartyAddr->PartyAddr.Dot[3],
            pPartyAddr->PartyPort
        );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoDelPartyAddr
            (
                ANSC_HANDLE                 hThisObject,
                char*                       party_name
            );

    description:

        This function is called to delete a party addr.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       party_name
                Specifies the party name to be matched.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoDelPartyAddr
    (
        ANSC_HANDLE                 hThisObject,
        char*                       party_name
    )
{
    PANSC_LPC_CONNECTOR_OBJECT      pMyObject    = (PANSC_LPC_CONNECTOR_OBJECT)hThisObject;
    PANSC_LPC_PARTY_ADDR            pPartyAddr   = (PANSC_LPC_PARTY_ADDR      )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = (PSINGLE_LINK_ENTRY        )NULL;
    ULONG                           ulHashIndex  = AnscHashString(party_name, AnscSizeOfString(party_name), ANSC_LPCCO_PARTY_TABLE_SIZE);

    AnscAcquireLock(&pMyObject->PartyTableLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->PartyTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pPartyAddr  = ACCESS_ANSC_LPC_PARTY_ADDR(pSLinkEntry);
        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

        if ( strcasecmp(pPartyAddr->PartyName,party_name) == 0 )
        {
            AnscQueuePopEntryByLink(&pMyObject->PartyTable[ulHashIndex], &pPartyAddr->Linkage);

            AnscFreeMemory(pPartyAddr);

            break;
        }
    }

    AnscReleaseLock(&pMyObject->PartyTableLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoDelAllPartyAddrs
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to delete all party addrs.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoDelAllPartyAddrs
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_LPC_CONNECTOR_OBJECT      pMyObject    = (PANSC_LPC_CONNECTOR_OBJECT)hThisObject;
    PANSC_LPC_PARTY_ADDR            pPartyAddr   = (PANSC_LPC_PARTY_ADDR      )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = (PSINGLE_LINK_ENTRY        )NULL;
    ULONG                           i            = 0;

    AnscAcquireLock(&pMyObject->PartyTableLock);

    for ( i = 0; i < ANSC_LPCCO_PARTY_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscQueuePopEntry(&pMyObject->PartyTable[i]);

        while ( pSLinkEntry )
        {
            pPartyAddr  = ACCESS_ANSC_LPC_PARTY_ADDR(pSLinkEntry);
            pSLinkEntry = AnscQueuePopEntry(&pMyObject->PartyTable[i]);

            AnscFreeMemory(pPartyAddr);
        }
    }

    AnscReleaseLock(&pMyObject->PartyTableLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscLpccoGetPendingCall
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       msg_type,
                ULONG                       seq_number
            );

    description:

        This function is called to retrieve a pending call.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       msg_type
                Specifies the message type to be matched.

                ULONG                       seq_number
                Specifies the seq number to be matched.

    return:     pending call.

**********************************************************************/

ANSC_HANDLE
AnscLpccoGetPendingCall
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       msg_type,
        ULONG                       seq_number
    )
{
    PANSC_LPC_CONNECTOR_OBJECT      pMyObject    = (PANSC_LPC_CONNECTOR_OBJECT)hThisObject;
    PANSC_LPCCO_PENDING_CALL        pPendingCall = (PANSC_LPCCO_PENDING_CALL  )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = (PSINGLE_LINK_ENTRY        )NULL;

    AnscAcquireLock(&pMyObject->PcallQueueLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->PcallQueue);

    while ( pSLinkEntry )
    {
        pPendingCall = ACCESS_ANSC_LPCCO_PENDING_CALL(pSLinkEntry);
        pSLinkEntry  = AnscQueueGetNextEntry(pSLinkEntry);

        if ( (pPendingCall->ImcpMsgType   == msg_type  ) &&
             (pPendingCall->CallSeqNumber == seq_number) )
        {
            AnscReleaseLock(&pMyObject->PcallQueueLock);

            return  (ANSC_HANDLE)pPendingCall;
        }
    }

    AnscReleaseLock(&pMyObject->PcallQueueLock);

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscLpccoPopPendingCall
            (
                ANSC_HANDLE                 hThisObject,
                char*                       party_name,
                ULONG                       msg_type,
                ULONG                       seq_number,
                ULONG                       error_code,
                ANSC_HANDLE                 output_bdo
            );

    description:

        This function is called to pop up a pending call.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       party_name
                Specifies the party name to be matched.

                ULONG                       msg_type
                Specifies the message type to be matched.

                ULONG                       seq_number
                Specifies the seq number to be matched.

                ULONG                       error_code
                Specifies the error code associated with the call.

                ANSC_HANDLE                 output_bdo
                Specifies the output buffer descriptor returned.

    return:     pending call.

**********************************************************************/

ANSC_HANDLE
AnscLpccoPopPendingCall
    (
        ANSC_HANDLE                 hThisObject,
        char*                       party_name,
        ULONG                       msg_type,
        ULONG                       seq_number,
        ULONG                       error_code,
        ANSC_HANDLE                 output_bdo
    )
{
    PANSC_LPC_CONNECTOR_OBJECT      pMyObject    = (PANSC_LPC_CONNECTOR_OBJECT)hThisObject;
    PANSC_LPCCO_PENDING_CALL        pPendingCall = (PANSC_LPCCO_PENDING_CALL  )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = (PSINGLE_LINK_ENTRY        )NULL;

    AnscAcquireLock(&pMyObject->PcallQueueLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->PcallQueue);

    while ( pSLinkEntry )
    {
        pPendingCall = ACCESS_ANSC_LPCCO_PENDING_CALL(pSLinkEntry);
        pSLinkEntry  = AnscQueueGetNextEntry(pSLinkEntry);

        if ( (pPendingCall->ImcpMsgType   == msg_type  ) &&
             (pPendingCall->CallSeqNumber == seq_number) )
        {
            if ( party_name && pPendingCall->PartyName )
            {
                if ( !strcasecmp(party_name,pPendingCall->PartyName) == 0 )
                {
                    continue;
                }
            }

            AnscQueuePopEntryByLink(&pMyObject->PcallQueue, &pPendingCall->Linkage);

            AnscReleaseLock(&pMyObject->PcallQueueLock);

            if ( TRUE )
            {
                pPendingCall->CallError     = error_code;
                pPendingCall->CallOutputBdo = output_bdo;

                AnscSetEvent(pPendingCall->CallEvent);
            }

            return  (ANSC_HANDLE)pPendingCall;
        }
    }

    AnscReleaseLock(&pMyObject->PcallQueueLock);

    if ( output_bdo )
    {
        AnscFreeBdo(output_bdo);
    }

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoAddPendingCall
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hPendingCall
            );

    description:

        This function is called to add a pending call.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hPendingCall
                Specifies the pending call to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoAddPendingCall
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPendingCall
    )
{
    PANSC_LPC_CONNECTOR_OBJECT      pMyObject    = (PANSC_LPC_CONNECTOR_OBJECT)hThisObject;
    PANSC_LPCCO_PENDING_CALL        pPendingCall = (PANSC_LPCCO_PENDING_CALL  )hPendingCall;

    AnscAcquireLock   (&pMyObject->PcallQueueLock);
    AnscQueuePushEntry(&pMyObject->PcallQueue, &pPendingCall->Linkage);
    AnscReleaseLock   (&pMyObject->PcallQueueLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoDelPendingCall
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       msg_type,
                ULONG                       seq_number
            );

    description:

        This function is called to delete a pending call.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       msg_type
                Specifies the message type to be matched.

                ULONG                       seq_number
                Specifies the seq number to be matched.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoDelPendingCall
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       msg_type,
        ULONG                       seq_number
    )
{
    PANSC_LPC_CONNECTOR_OBJECT      pMyObject    = (PANSC_LPC_CONNECTOR_OBJECT)hThisObject;
    PANSC_LPCCO_PENDING_CALL        pPendingCall = (PANSC_LPCCO_PENDING_CALL  )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = (PSINGLE_LINK_ENTRY        )NULL;

    AnscAcquireLock(&pMyObject->PcallQueueLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->PcallQueue);

    while ( pSLinkEntry )
    {
        pPendingCall = ACCESS_ANSC_LPCCO_PENDING_CALL(pSLinkEntry);
        pSLinkEntry  = AnscQueueGetNextEntry(pSLinkEntry);

        if ( (pPendingCall->ImcpMsgType   == msg_type  ) &&
             (pPendingCall->CallSeqNumber == seq_number) )
        {
            AnscQueuePopEntryByLink(&pMyObject->PcallQueue, &pPendingCall->Linkage);

            AnscSetEvent  (pPendingCall->CallEvent);
            AnscFreeMemory(pPendingCall);

            break;
        }
    }

    AnscReleaseLock(&pMyObject->PcallQueueLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoPopAllPendingCalls
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       error_code
            );

    description:

        This function is called to pop up all pending calls.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       error_code
                Specifies the error code associated with the call.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoPopAllPendingCalls
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       error_code
    )
{
    PANSC_LPC_CONNECTOR_OBJECT      pMyObject    = (PANSC_LPC_CONNECTOR_OBJECT)hThisObject;
    PANSC_LPCCO_PENDING_CALL        pPendingCall = (PANSC_LPCCO_PENDING_CALL  )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = (PSINGLE_LINK_ENTRY        )NULL;

    AnscAcquireLock(&pMyObject->PcallQueueLock);

    pSLinkEntry = AnscQueuePopEntry(&pMyObject->PcallQueue);

    while ( pSLinkEntry )
    {
        pPendingCall = ACCESS_ANSC_LPCCO_PENDING_CALL(pSLinkEntry);
        pSLinkEntry  = AnscQueuePopEntry(&pMyObject->PcallQueue);

        if ( TRUE )
        {
            pPendingCall->CallError     = error_code;
            pPendingCall->CallOutputBdo = (ANSC_HANDLE)NULL;

            AnscSetEvent(pPendingCall->CallEvent);
        }
    }

    AnscReleaseLock(&pMyObject->PcallQueueLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoDelAllPendingCalls
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to delete all pending calls.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoDelAllPendingCalls
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_LPC_CONNECTOR_OBJECT      pMyObject    = (PANSC_LPC_CONNECTOR_OBJECT)hThisObject;
    PANSC_LPCCO_PENDING_CALL        pPendingCall = (PANSC_LPCCO_PENDING_CALL  )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = (PSINGLE_LINK_ENTRY        )NULL;

    AnscAcquireLock(&pMyObject->PcallQueueLock);

    pSLinkEntry = AnscQueuePopEntry(&pMyObject->PcallQueue);

    while ( pSLinkEntry )
    {
        pPendingCall = ACCESS_ANSC_LPCCO_PENDING_CALL(pSLinkEntry);
        pSLinkEntry  = AnscQueuePopEntry(&pMyObject->PcallQueue);

        AnscSetEvent  (pPendingCall->CallEvent);
        AnscFreeMemory(pPendingCall);
    }

    AnscReleaseLock(&pMyObject->PcallQueueLock);

    return  ANSC_STATUS_SUCCESS;
}
