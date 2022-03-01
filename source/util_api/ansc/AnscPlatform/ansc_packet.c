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

    module:	ansc_packet.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the some platform-dependent and
        general utility functions related to packet processing.

        *   AnscAcquirePdo
        *   AnscReleasePdo
        *   AnscAllocatePdo
        *   AnscAllocatePdo2
        *   AnscCopyPdo
        *   AnscTransferPdo
        *   AnscClonePdo
        *   AnscFreePdo
        *   AnscFreePdo2
        *   AnscPdoClean
        *   AnscPdoCleanBdoQueue
        *   AnscPdoGetPacketSize
        *   AnscPdoGetBdoByOffset
        *   AnscPdoGetBufferByOffset
        *   AnscPdoCopyFrom
        *   AnscPdoGetPackingNumber
        *   AnscPdoCalChecksum
        *   AnscPdoConcatenate
        *   AnscPdoShrinkLeft
        *   AnscPdoShrinkRight
        *   AnscPdoShrinkMiddle
        *   AnscPdoBreakAt
        *   AnscPdoBreakAt2
        *   AnscPdoSplitAt
        *   AnscAcquireBdo
        *   AnscReleaseBdo
        *   AnscAllocateBdo
        *   AnscAllocateBdo2
        *   AnscCopyBdo
        *   AnscFreeBdo
        *   AnscFreeBdo2
        *   AnscFreeSonBdo
        *   AnscBdoClean
        *   AnscBdoCopyFrom
        *   AnscBdoBreakAt

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        01/25/02    initial revision.

**********************************************************************/


#include "ansc_global.h"

#ifdef   _ANSC_DIRTY_PDO_
#include "giad_definitions.h"
#include "ansc_crypto.h"
#include "dslg_definitions_datapath.h"
#endif
#include <sys/mman.h>


static  BOOL                        g_bPdoPoolingEnabled  = ANSC_ENABLE_PDO_POOLING;
static  BOOL                        g_bBdoPoolingEnabled  = ANSC_ENABLE_BDO_POOLING;
static  BOOL                        g_bPdoPoolInitialized = FALSE;
static  BOOL                        g_bBdoPoolInitialized = FALSE;
static  ULONG                       g_ulMaxPdoPoolSize    = ANSC_MAX_PDO_POOL_SIZE;
static  ULONG                       g_ulMaxBdoPoolSize    = ANSC_MAX_BDO_POOL_SIZE;
static  ULONG                       g_ulInitPdoPoolSize   = ANSC_MAX_PDO_POOL_SIZE/2;
static  ULONG                       g_ulInitBdoPoolSize   = ANSC_MAX_BDO_POOL_SIZE/2;


static  SLIST_HEADER                g_qPdoPoolList;
static  ANSC_SPINLOCK               g_qPdoPoolSpinLock;
static  SLIST_HEADER                g_qBdoPoolList;
static  ANSC_SPINLOCK               g_qBdoPoolSpinLock;

#ifdef _ANSC_TRACE_PACKET_
static  ULONG                       g_ulAllocPdo    = 0;
static  ULONG                       g_ulAllocBdo    = 0;
static  ULONG                       g_ulFreePdo     = 0;
static  ULONG                       g_ulFreeBdo     = 0;

/*
 * global data for profiling
 */

static   BOOLEAN                    gBdoTraceInit    = FALSE;
static   ANSC_SPINLOCK              gBdoTraceLock;
static   DLIST_HEADER               gBdoTraceList;

/*
 * static funciton:
 */
static
const char *
GetFileName
    (
        const char *                pPathName
    )
{
    const char *                    p;

    for( p = pPathName + AnscSizeOfString(pPathName); p > pPathName; p -- )
    {
        if( *(p-1) == '/' || *(p-1) == '\\' )
            break;
    }

    return p;
}

#endif /* _ANSC_TRACE_PACKET_ */


#ifndef _ANSC_TRACE_PACKET_

static
ANSC_STATUS
AnscReleasePdo
    (
        ANSC_HANDLE                 hPdo
    );

static
ANSC_STATUS
AnscFreePdo2
    (
        ANSC_HANDLE                 hPdo
    );

#else

static
ANSC_STATUS
AnscReleasePdoTrace
    (
        ANSC_HANDLE                 hPdo,
        const char *                pFileName,
        int                         iLineNo
    );

static
ANSC_STATUS
AnscFreePdo2Trace
    (
        ANSC_HANDLE                 hPdo,
        const char *                pFileName,
        int                         iLineNo
    );

#define AnscReleasePdo(hPdo)    \
            AnscReleasePdoTrace(hPdo, __FILE__, __LINE__)

#define AnscFreePdo2(hPdo)      \
            AnscFreePdo2Trace(hPdo, __FILE__, __LINE__)

#endif /* _ANSC_TRACE_PACKET_ */


static
ANSC_HANDLE
AnscAllocatePdo2
    (
        void
    );

static
ANSC_HANDLE
AnscAllocateBdo2
    (
        ULONG                       ulBufferSize,
        ULONG                       ulBlockOffset,
        ULONG                       ulBlockSize
    );


static
ANSC_HANDLE
AnscAcquirePdo
    (
        void
    )
{
    PSINGLE_LINK_ENTRY              pSLinkEntry = NULL;

    if ( g_bPdoPoolingEnabled )
    {
        if ( !g_bPdoPoolInitialized )
        {
            ULONG                   i;
            
            AnscSListInitializeHeader(&g_qPdoPoolList        );
            AnscInitializeSpinLock   (&g_qPdoPoolSpinLock);

            /*
             * pre-allocate the Pdo list: 
             */
            for( i = 0; i < g_ulInitPdoPoolSize; i ++ )
            {
                ANSC_HANDLE         hPdo;

                hPdo = AnscAllocatePdo2();
                if( ! hPdo )
                {
                    break;
                }
                AnscReleasePdo(hPdo);
            }

            g_bPdoPoolInitialized = TRUE;
        }

        AnscAcquireSpinLock(&g_qPdoPoolSpinLock);
        pSLinkEntry = AnscSListPopEntry(&g_qPdoPoolList);
        AnscReleaseSpinLock(&g_qPdoPoolSpinLock);

        if ( pSLinkEntry )
        {
            return  (ANSC_HANDLE)ACCESS_ANSC_PACKET_DESCRIPTOR(pSLinkEntry);
        }
    }

    return  AnscAllocatePdo2();
}


#ifndef _ANSC_TRACE_PACKET_
ANSC_STATUS
AnscReleasePdo
    (
        ANSC_HANDLE                 hPdo
    )
#else
ANSC_STATUS
AnscReleasePdoTrace
    (
        ANSC_HANDLE                 hPdo,
        const char *                pFileName,
        int                         iLineNo
    )
#endif
{
    PANSC_PACKET_DESCRIPTOR         pPdo      = (PANSC_PACKET_DESCRIPTOR)hPdo;
    PFN_PDO_FREE_ME                 pfnFreeMe = NULL;

    if ( !g_bPdoPoolingEnabled )
    {
        return  AnscFreePdo2(hPdo);
    }
    else
    {
        if ( pPdo->pfnFreeMe )
        {
            pfnFreeMe = pPdo->pfnFreeMe;
#ifndef _ANSC_TRACE_PACKET_
            AnscPdoClean((ANSC_HANDLE)pPdo);
#else
            AnscPdoCleanTrace((ANSC_HANDLE)pPdo, pFileName, iLineNo);
#endif

            pfnFreeMe(pPdo->hOwnerObject, (ANSC_HANDLE)pPdo);
        }
        else
        {
            /*
             * race condition may occur for the queue size, but it can only make the
             * pool a little bit over-limit and should not matter.
             */
            if ( AnscSListQueryDepth(&g_qPdoPoolList) >= g_ulMaxPdoPoolSize )
            {
#ifdef _ANSC_TRACE_PACKET_
                AnscTraceWarning(("@@ AnscPacket: Pdo pool over limit %d !!! -- Size limit %d.\n", 
                    g_ulFreePdo ++, g_ulMaxPdoPoolSize));
#endif /* _ANSC_TRACE_PACKET_ */
                AnscFreePdo2(hPdo);
            }
            else
            {
#ifndef _ANSC_TRACE_PACKET_
                AnscPdoClean((ANSC_HANDLE)pPdo);
#else
                AnscPdoCleanTrace((ANSC_HANDLE)pPdo, pFileName, iLineNo);
#endif

                AnscAcquireSpinLock(&g_qPdoPoolSpinLock);
                AnscSListPushEntry(&g_qPdoPoolList, &pPdo->Linkage);
                AnscReleaseSpinLock(&g_qPdoPoolSpinLock);
            }
        }
    }

    return  ANSC_STATUS_SUCCESS;
}


ANSC_HANDLE
AnscAllocatePdo
    (
        void
    )
{
    return  AnscAcquirePdo();
}


ANSC_HANDLE
AnscAllocatePdo2
    (
        void
    )
{
    PANSC_PACKET_DESCRIPTOR         pPdo         = NULL;

#ifdef _ANSC_TRACE_PACKET_
    AnscTraceWarning(("@@ AnscPacket: Allocating new Pdo %d.\n", ++g_ulAllocPdo));
#endif

    pPdo = (PANSC_PACKET_DESCRIPTOR)AnscAllocateMemory(sizeof(ANSC_PACKET_DESCRIPTOR));

    if ( !pPdo )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pPdo->FinalStatus     = ANSC_STATUS_SUCCESS;
        pPdo->Flags           = 0;
        pPdo->bSyncOnly       = FALSE;
        pPdo->bStealthy       = FALSE;
        pPdo->bQueuable       = FALSE;

        pPdo->CurHeaderBdo    = (ANSC_HANDLE)NULL;
        pPdo->CurHeaderBuffer = NULL;
        pPdo->CurHeaderOffset = 0;
        pPdo->ErrorCode       = 0;

        pPdo->hPktPassport    = (ANSC_HANDLE)NULL;
        pPdo->hPktSnpuPass    = (ANSC_HANDLE)NULL;
        pPdo->hOwnerObject    = (ANSC_HANDLE)NULL;
        pPdo->pfnFreeMe       = NULL;

        AnscQueueInitializeHeader(&pPdo->BdoQueue);
    }

    return  (ANSC_HANDLE)pPdo;
}


ANSC_HANDLE
AnscCopyPdo
    (
        ANSC_HANDLE                 hPdo
    )
{
    PANSC_PACKET_DESCRIPTOR         pSrcPdo      = (PANSC_PACKET_DESCRIPTOR)hPdo;
    PANSC_PACKET_DESCRIPTOR         pDstPdo      = (PANSC_PACKET_DESCRIPTOR)AnscAllocatePdo();
    PANSC_BUFFER_DESCRIPTOR         pDstBdo      = NULL;
    ULONG                           ulPacketSize = AnscPdoGetPacketSize((ANSC_HANDLE)pSrcPdo);

    if ( !pDstPdo )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else if ( ulPacketSize > 0 )
    {
        pDstBdo =
            (PANSC_BUFFER_DESCRIPTOR)AnscAllocateBdo
                (
                    ulPacketSize + ANSC_PDO_MAX_HEAD_ROOM + ANSC_PDO_MAX_TAIL_ROOM,
                    ANSC_PDO_MAX_HEAD_ROOM,
                    ulPacketSize
                );

        if ( !pDstBdo )
        {
            AnscFreePdo((ANSC_HANDLE)pDstPdo);

            return  (ANSC_HANDLE)NULL;
        }
        else
        {
            AnscPdoAttachBdoAtFront(pDstPdo, pDstBdo);
        }
    }
    else
    {
        pDstBdo = NULL;
    }

    pDstPdo->FinalStatus     = pSrcPdo->FinalStatus;
    pDstPdo->Flags           = 0;
    pDstPdo->bSyncOnly       = FALSE;
    pDstPdo->bStealthy       = pSrcPdo->bStealthy;
    pDstPdo->bQueuable       = FALSE;

    pDstPdo->CurHeaderBdo    = pDstBdo? AnscPdoGetBdoByOffset   ((ANSC_HANDLE)pDstPdo, pSrcPdo->CurHeaderOffset) : NULL;
    pDstPdo->CurHeaderBuffer = pDstBdo? AnscPdoGetBufferByOffset((ANSC_HANDLE)pDstPdo, pSrcPdo->CurHeaderOffset) : NULL;
    pDstPdo->CurHeaderOffset = pSrcPdo->CurHeaderOffset;
    pDstPdo->ErrorCode       = pSrcPdo->ErrorCode;

    pDstPdo->hPktPassport    = (ANSC_HANDLE)NULL;
    pDstPdo->hPktSnpuPass    = (ANSC_HANDLE)NULL;
    pDstPdo->hOwnerObject    = (ANSC_HANDLE)NULL;
    pDstPdo->pfnFreeMe       = NULL;

    AnscCopyMemory(pDstPdo->T2SpecificPad, pSrcPdo->T2SpecificPad, ANSC_PDO_INFO_PAD_SIZE );
    AnscCopyMemory(pDstPdo->Layer2InfoPad, pSrcPdo->Layer2InfoPad, ANSC_PDO_INFO_PAD_SIZE );
    AnscCopyMemory(pDstPdo->Layer3InfoPad, pSrcPdo->Layer3InfoPad, ANSC_PDO_INFO_PAD_SIZE );
    AnscCopyMemory(pDstPdo->Layer4InfoPad, pSrcPdo->Layer4InfoPad, ANSC_PDO_INFO_PAD_SIZE );
    AnscCopyMemory(pDstPdo->ModelsInfoPad, pSrcPdo->ModelsInfoPad, ANSC_PDO_INFO_PAD_SIZE2);

    if ( pDstBdo )
    {
        AnscPdoCopyFrom
            (
                (ANSC_HANDLE)pSrcPdo,
                AnscBdoGetBlock(pDstBdo),
                &ulPacketSize,
                0
            );
    }

    return  (ANSC_HANDLE)pDstPdo;
}


ANSC_HANDLE
AnscTransferPdo
    (
        ANSC_HANDLE                 hPdo
    )
{
    PANSC_PACKET_DESCRIPTOR         pSrcPdo      = (PANSC_PACKET_DESCRIPTOR)hPdo;
    PANSC_PACKET_DESCRIPTOR         pDstPdo      = NULL;

    if ( pSrcPdo->bSyncOnly )
    {
        return  (ANSC_HANDLE)AnscCopyPdo((ANSC_HANDLE)pSrcPdo);
    }
    else
    {
        pDstPdo = (PANSC_PACKET_DESCRIPTOR)AnscAllocatePdo();
    }

    if ( !pDstPdo )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pDstPdo->FinalStatus     = pSrcPdo->FinalStatus;
        pDstPdo->Flags           = 0;
        pDstPdo->bSyncOnly       = FALSE;
        pDstPdo->bStealthy       = pSrcPdo->bStealthy;
        pDstPdo->bQueuable       = FALSE;

        pDstPdo->CurHeaderBdo    = pSrcPdo->CurHeaderBdo;
        pDstPdo->CurHeaderBuffer = pSrcPdo->CurHeaderBuffer;
        pDstPdo->CurHeaderOffset = pSrcPdo->CurHeaderOffset;
        pDstPdo->ErrorCode       = pSrcPdo->ErrorCode;

        pDstPdo->hPktPassport    = (ANSC_HANDLE)NULL;
        pDstPdo->hPktSnpuPass    = (ANSC_HANDLE)NULL;
        pDstPdo->hOwnerObject    = (ANSC_HANDLE)NULL;
        pDstPdo->pfnFreeMe       = NULL;

        AnscCopyMemory(pDstPdo->T2SpecificPad, pSrcPdo->T2SpecificPad, ANSC_PDO_INFO_PAD_SIZE );
        AnscCopyMemory(pDstPdo->Layer2InfoPad, pSrcPdo->Layer2InfoPad, ANSC_PDO_INFO_PAD_SIZE );
        AnscCopyMemory(pDstPdo->Layer3InfoPad, pSrcPdo->Layer3InfoPad, ANSC_PDO_INFO_PAD_SIZE );
        AnscCopyMemory(pDstPdo->Layer4InfoPad, pSrcPdo->Layer4InfoPad, ANSC_PDO_INFO_PAD_SIZE );
        AnscCopyMemory(pDstPdo->ModelsInfoPad, pSrcPdo->ModelsInfoPad, ANSC_PDO_INFO_PAD_SIZE2);
    }

    pDstPdo->BdoQueue = pSrcPdo->BdoQueue;

    AnscQueueInitializeHeader(&pSrcPdo->BdoQueue);

    pSrcPdo->CurHeaderBdo    = (ANSC_HANDLE)NULL;
    pSrcPdo->CurHeaderBuffer = NULL;
    pSrcPdo->CurHeaderOffset = 0;

    return  (ANSC_HANDLE)pDstPdo;
}


ANSC_HANDLE
AnscClonePdo
    (
        ANSC_HANDLE                 hPdo
    )
{
    PANSC_PACKET_DESCRIPTOR         pSrcPdo      = (PANSC_PACKET_DESCRIPTOR)hPdo;
    PANSC_PACKET_DESCRIPTOR         pDstPdo      = NULL;

    /*
     * This function allocates an empty packet descriptor, but preserve all layer information. This
     * function is useful in places where we need to maintain a copy of the layer information but
     * don't want to store the packet data.
     */
    pDstPdo = (PANSC_PACKET_DESCRIPTOR)AnscAllocatePdo();

    if ( !pDstPdo )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pDstPdo->FinalStatus     = pSrcPdo->FinalStatus;
        pDstPdo->Flags           = 0;
        pDstPdo->bSyncOnly       = FALSE;
        pDstPdo->bStealthy       = pSrcPdo->bStealthy;
        pDstPdo->bQueuable       = FALSE;

        pDstPdo->CurHeaderBdo    = NULL;
        pDstPdo->CurHeaderBuffer = NULL;
        pDstPdo->CurHeaderOffset = 0;
        pDstPdo->ErrorCode       = pSrcPdo->ErrorCode;

        pDstPdo->hPktPassport    = (ANSC_HANDLE)NULL;
        pDstPdo->hPktSnpuPass    = (ANSC_HANDLE)NULL;
        pDstPdo->hOwnerObject    = (ANSC_HANDLE)NULL;
        pDstPdo->pfnFreeMe       = NULL;

        AnscCopyMemory(pDstPdo->T2SpecificPad, pSrcPdo->T2SpecificPad, ANSC_PDO_INFO_PAD_SIZE );
        AnscCopyMemory(pDstPdo->Layer2InfoPad, pSrcPdo->Layer2InfoPad, ANSC_PDO_INFO_PAD_SIZE );
        AnscCopyMemory(pDstPdo->Layer3InfoPad, pSrcPdo->Layer3InfoPad, ANSC_PDO_INFO_PAD_SIZE );
        AnscCopyMemory(pDstPdo->Layer4InfoPad, pSrcPdo->Layer4InfoPad, ANSC_PDO_INFO_PAD_SIZE );
        AnscCopyMemory(pDstPdo->ModelsInfoPad, pSrcPdo->ModelsInfoPad, ANSC_PDO_INFO_PAD_SIZE2);
    }

    AnscQueueInitializeHeader(&pDstPdo->BdoQueue);

    return  (ANSC_HANDLE)pDstPdo;
}


#ifndef _ANSC_TRACE_PACKET_
ANSC_STATUS
AnscFreePdo
    (
        ANSC_HANDLE                 hPdo
    )
{
    return  AnscReleasePdo(hPdo);
}
#else
ANSC_STATUS
AnscFreePdoTrace
    (
        ANSC_HANDLE                 hPdo,
        const char *                pFileName,
        int                         iLineNo
    )
{
    return AnscReleasePdoTrace(hPdo, pFileName, iLineNo);
}
#endif /* _ANSC_TRACE_PACKET_ */


#ifndef _ANSC_TRACE_PACKET_
ANSC_STATUS
AnscFreePdo2
    (
        ANSC_HANDLE                 hPdo
    )
#else
ANSC_STATUS
AnscFreePdo2Trace
    (
        ANSC_HANDLE                 hPdo,
        const char *                pFileName,
        int                         iLineNo
    )
#endif
{
    PANSC_PACKET_DESCRIPTOR         pPdo         = (PANSC_PACKET_DESCRIPTOR)hPdo;

#ifndef _ANSC_TRACE_PACKET_
    AnscPdoCleanBdoQueue(hPdo);
#else
    AnscPdoCleanBdoQueueTrace(hPdo, pFileName, iLineNo);
#endif

    if ( pPdo->pfnFreeMe )
    {
        pPdo->pfnFreeMe(pPdo->hOwnerObject, (ANSC_HANDLE)pPdo);
    }
    else
    {
        AnscFreeMemory(pPdo);
    }

    return  ANSC_STATUS_SUCCESS;
}


#ifndef _ANSC_TRACE_PACKET_
void
AnscPdoClean
    (
        ANSC_HANDLE                 hPdo
    )
#else
void
AnscPdoCleanTrace
    (
        ANSC_HANDLE                 hPdo,
        const char *                pFileName,
        int                         iLineNo
    )
#endif
{
    PANSC_PACKET_DESCRIPTOR         pPdo         = (PANSC_PACKET_DESCRIPTOR)hPdo;
    ULONG                           ulPdoSize    = sizeof(ANSC_PACKET_DESCRIPTOR);
    ULONG*                          pTempUlong   = (ULONG*)pPdo;
    UCHAR*                          pTempUchar   = (UCHAR*)pPdo;
    ULONG                           i            = 0;

#ifndef _ANSC_TRACE_PACKET_
    AnscPdoCleanBdoQueue(hPdo);
#else
    AnscPdoCleanBdoQueueTrace(hPdo, pFileName, iLineNo);
#endif

#ifdef  _ANSC_DIRTY_PDO_
    {
        PGIAD_PACKET_INFO           pGiadPacketInfo = (PGIAD_PACKET_INFO)AnscPdoGetModelsInfo(pPdo);
        PDSLG_PACKET_INFO           pDslgPacketInfo = (PDSLG_PACKET_INFO)AnscPdoGetT2Specific(pPdo);

        pPdo->FinalStatus                   = ANSC_STATUS_SUCCESS;

        pPdo->Flags                         = 0;
        pPdo->bSyncOnly                     = FALSE;

        pPdo->hPktPassport                  = (ANSC_HANDLE)NULL;
        pPdo->hPktSnpuPass                  = (ANSC_HANDLE)NULL;

        pGiadPacketInfo->ProcessHistory     = 0;
        pGiadPacketInfo->ProcessFlags       = 0;
        pGiadPacketInfo->DefRouteMetric     = 0;
        pGiadPacketInfo->OrgSrcIpAddr.Value = 0;
        pGiadPacketInfo->OrgSrcPort         = 0;
        pGiadPacketInfo->OrgDstIpAddr.Value = 0;
        pGiadPacketInfo->OrgDstPort         = 0;
        pGiadPacketInfo->PolicyRuleId       = 0;
        pGiadPacketInfo->TargetTrunk        = (ANSC_HANDLE)NULL;
        pGiadPacketInfo->hTrunkContext1     = (ANSC_HANDLE)NULL;
        pGiadPacketInfo->hTrunkContext2     = (ANSC_HANDLE)NULL;

        pDslgPacketInfo->OrgPacketFormat    = 0;
        pDslgPacketInfo->ProcessFlags       = 0;
    }
#else
    {
        for ( i = 0; i < ulPdoSize / 4; i++ )
        {
            pTempUlong[i] = 0;
        }

        if ( (ulPdoSize % 4) > 0 )  /* impossible since we didn't pack the structure */
        {
            pTempUchar = (PUCHAR)((ULONG)pTempUchar + i * sizeof(ULONG));

            for ( i = 0; i < ulPdoSize % 4; i++ )
            {
                pTempUchar[i] = 0;
            }
        }

        /*
        pPdo->FinalStatus     = ANSC_STATUS_SUCCESS;
        pPdo->CurHeaderBdo    = (ANSC_HANDLE)NULL;
        pPdo->CurHeaderBuffer = NULL;
        pPdo->CurHeaderOffset = 0;
        pPdo->ErrorCode       = 0;
        pPdo->hPktPassport    = (ANSC_HANDLE)NULL;

        AnscZeroMemory(pPdo->Layer2InfoPad, ANSC_PDO_INFO_PAD_SIZE );
        AnscZeroMemory(pPdo->Layer3InfoPad, ANSC_PDO_INFO_PAD_SIZE );
        AnscZeroMemory(pPdo->Layer4InfoPad, ANSC_PDO_INFO_PAD_SIZE );
        AnscZeroMemory(pPdo->ErrorsInfoPad, ANSC_PDO_INFO_PAD_SIZE );
        AnscZeroMemory(pPdo->ModelsInfoPad, ANSC_PDO_INFO_PAD_SIZE2);
        */
    }
#endif
}

#ifndef _ANSC_TRACE_PACKET_
void
AnscPdoCleanBdoQueue
    (
        ANSC_HANDLE                 hPdo
    )
#else
void
AnscPdoCleanBdoQueueTrace
    (
        ANSC_HANDLE                 hPdo,
        const char *                pFileName,
        int                         iLineNo
    )
#endif
{
    PANSC_PACKET_DESCRIPTOR         pPdo         = (PANSC_PACKET_DESCRIPTOR)hPdo;
    PANSC_BUFFER_DESCRIPTOR         pBdo;
    PSINGLE_LINK_ENTRY              pSLinkEntry;

    pSLinkEntry = AnscQueueGetFirstEntry(&pPdo->BdoQueue);

    while ( pSLinkEntry )
    {
        pBdo        = ACCESS_ANSC_BUFFER_DESCRIPTOR(pSLinkEntry);
        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

#ifndef _ANSC_TRACE_PACKET_
        AnscFreeBdo(pBdo);
#else
        AnscFreeBdoTrace(pBdo, pFileName, iLineNo);
#endif
    }

    AnscQueueInitializeHeader(&pPdo->BdoQueue);
}


ULONG
AnscPdoGetPacketSize
    (
        ANSC_HANDLE                 hPdo
    )
{
    PANSC_PACKET_DESCRIPTOR         pPdo         = (PANSC_PACKET_DESCRIPTOR)hPdo;
    PANSC_BUFFER_DESCRIPTOR         pBdo         = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           ulPacketSize = 0;

    pSLinkEntry = AnscQueueGetFirstEntry(&pPdo->BdoQueue);

    while ( pSLinkEntry )
    {
        pBdo        = ACCESS_ANSC_BUFFER_DESCRIPTOR(pSLinkEntry);
        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

        ulPacketSize += AnscBdoGetBlockSize(pBdo);
    }

    return  ulPacketSize;
}


ANSC_HANDLE
AnscPdoGetBdoByOffset
    (
        ANSC_HANDLE                 hPdo,
        ULONG                       ulOffset
    )
{
    PANSC_PACKET_DESCRIPTOR         pPdo         = (PANSC_PACKET_DESCRIPTOR)hPdo;
    PANSC_BUFFER_DESCRIPTOR         pBdo         = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           ulCurOffset  = 0;
    BOOL                            bBdoFound    = FALSE;

    pSLinkEntry = AnscQueueGetFirstEntry(&pPdo->BdoQueue);

    while ( pSLinkEntry )
    {
        pBdo        = ACCESS_ANSC_BUFFER_DESCRIPTOR(pSLinkEntry);
        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

        if ( (ulCurOffset + AnscBdoGetBlockSize(pBdo)) > ulOffset )
        {
            bBdoFound = TRUE;

            break;
        }
        else
        {
            ulCurOffset += AnscBdoGetBlockSize(pBdo);
        }
    }

    if ( !bBdoFound )
    {
        return  (ANSC_HANDLE)NULL;
    }

    return  (ANSC_HANDLE)pBdo;
}


PVOID
AnscPdoGetBufferByOffset
    (
        ANSC_HANDLE                 hPdo,
        ULONG                       ulOffset
    )
{
    PANSC_PACKET_DESCRIPTOR         pPdo         = (PANSC_PACKET_DESCRIPTOR)hPdo;
    PANSC_BUFFER_DESCRIPTOR         pBdo         = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           ulCurOffset  = 0;
    BOOL                            bBdoFound    = FALSE;

    pSLinkEntry = AnscQueueGetFirstEntry(&pPdo->BdoQueue);

    while ( pSLinkEntry )
    {
        pBdo        = ACCESS_ANSC_BUFFER_DESCRIPTOR(pSLinkEntry);
        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

        if ( (ulCurOffset + AnscBdoGetBlockSize(pBdo)) > ulOffset )
        {
            bBdoFound = TRUE;

            break;
        }
        else
        {
            ulCurOffset += AnscBdoGetBlockSize(pBdo);
        }
    }

    if ( !bBdoFound )
    {
        return  NULL;
    }

    return  (PVOID)((ULONG)AnscBdoGetBlock(pBdo) + (ulOffset - ulCurOffset));
}


ANSC_STATUS
AnscPdoCopyFrom
    (
        ANSC_HANDLE                 hPdo,
        PVOID                       pDstBuffer,
        PULONG                      pulCopySize,
        ULONG                       ulOffset
    )
{
    PANSC_PACKET_DESCRIPTOR         pPdo           = (PANSC_PACKET_DESCRIPTOR)hPdo;
    PANSC_BUFFER_DESCRIPTOR         pBdo           = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry    = NULL;
    ULONG                           ulPacketOffset = 0;
    ULONG                           ulBufferOffset = 0;
    ULONG                           ulLeftSize     = *pulCopySize;
    ULONG                           ulCopiedSize   = 0;
    ULONG                           ulTbcSize      = 0;
    BOOL                            bFirstBdoFound = FALSE;

    if ( ulOffset > AnscPdoGetPacketSize((ANSC_HANDLE)pPdo) )
    {
        return  ANSC_STATUS_BAD_OFFSET;
    }
    else if ( ulLeftSize == 0 )
    {
        return  ANSC_STATUS_BAD_SIZE;
    }

    pSLinkEntry = AnscQueueGetFirstEntry(&pPdo->BdoQueue);

    while ( pSLinkEntry && (ulLeftSize > 0) )
    {
        pBdo        = ACCESS_ANSC_BUFFER_DESCRIPTOR(pSLinkEntry);
        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

        if ( !bFirstBdoFound )
        {
            if ( (ulPacketOffset + AnscBdoGetBlockSize(pBdo)) <= ulOffset )
            {
                ulPacketOffset += AnscBdoGetBlockSize(pBdo);

                continue;
            }
            else
            {
                bFirstBdoFound = TRUE;
                ulBufferOffset = ulOffset - ulPacketOffset;
            }
        }
        else
        {
            ulBufferOffset = 0;
        }

        ulTbcSize   = AnscBdoGetBlockSize(pBdo) - ulBufferOffset;
        ulTbcSize   = (ulTbcSize <= ulLeftSize)? ulTbcSize : ulLeftSize;
        ulLeftSize -= ulTbcSize;

        AnscBdoCopyFrom
            (
                (ANSC_HANDLE)pBdo,
                (PVOID)((ULONG)pDstBuffer + ulCopiedSize),
                &ulTbcSize,
                ulBufferOffset
            );

        ulCopiedSize += ulTbcSize;
    }

    *pulCopySize = ulCopiedSize;

    return  ANSC_STATUS_SUCCESS;
}


ULONG
AnscPdoGetPackingNumber
    (
        ANSC_HANDLE                 hPdo,
        ULONG                       ulOffset,
        ULONG                       ulSize
    )
{
    PANSC_PACKET_DESCRIPTOR         pPdo            = (PANSC_PACKET_DESCRIPTOR)hPdo;
    PANSC_BUFFER_DESCRIPTOR         pBdo            = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = NULL;
    ULONG                           ulPacketOffset  = 0;
    ULONG                           ulBufferOffset  = 0;
    ULONG                           ulLeftSize      = ulSize;
    ULONG                           ulCopiedSize    = 0;
    ULONG                           ulTbcSize       = 0;
    ULONG                           ulPackingNumber = 0;
    USHORT                          usBoundaryWord  = 0;
    BOOL                            bOddBytes       = FALSE;
    BOOL                            bFirstBdoFound  = FALSE;

    if ( ulOffset > AnscPdoGetPacketSize((ANSC_HANDLE)pPdo) )
    {
        return  0;
    }
    else if ( ulLeftSize == 0 )
    {
        return  0;
    }

    pSLinkEntry = AnscQueueGetFirstEntry(&pPdo->BdoQueue);

    while ( pSLinkEntry && (ulLeftSize > 0) )
    {
        pBdo        = ACCESS_ANSC_BUFFER_DESCRIPTOR(pSLinkEntry);
        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

        if ( !bFirstBdoFound )
        {
            if ( (ulPacketOffset + AnscBdoGetBlockSize(pBdo)) <= ulOffset )
            {
                ulPacketOffset += AnscBdoGetBlockSize(pBdo);

                continue;
            }
            else
            {
                bFirstBdoFound = TRUE;
                ulBufferOffset = ulOffset - ulPacketOffset;
            }
        }
        else if ( AnscBdoGetBlockSize(pBdo) == 0 )
        {
            continue;
        }
        else
        {
            ulBufferOffset = 0;
        }

        /*
         * If the previous buffer has left one byte to consume, we need to combine it with the
         * first byte of this buffer and add them (as a single WORD) to the packing number.
         */
        if ( bOddBytes )
        {
            *(PUCHAR)((ULONG)&usBoundaryWord + 1) = *(PUCHAR)((ULONG)pBdo->Buffer + pBdo->Offset + ulBufferOffset);
            ulPackingNumber                      += (ULONG)usBoundaryWord;
            ulBufferOffset++;

            bOddBytes     = FALSE;
            ulCopiedSize += 2;
            ulLeftSize   -= 2;
        }

        ulTbcSize   = AnscBdoGetBlockSize(pBdo) - ulBufferOffset;
        ulTbcSize   = (ulTbcSize <= ulLeftSize)? ulTbcSize : ulLeftSize;
        ulLeftSize -= ulTbcSize;

        /*
         * If the current buffer has an odd number of bytes, we need to take out the last byte and
         * combine it with the first byte of the next buffer.
         */
        if ( ulLeftSize > 0 )
        {
            if ( (ulTbcSize % 2) != 0 )
            {
                bOddBytes                = TRUE;
                usBoundaryWord           = 0;
                *(PUCHAR)&usBoundaryWord = *(PUCHAR)((ULONG)pBdo->Buffer + pBdo->Offset + ulBufferOffset + ulTbcSize - 1);
                ulTbcSize               -= 1;
                ulLeftSize              += 1;
            }
        }

        if ( ulTbcSize == 0 )
        {
            continue;
        }

        ulPackingNumber +=
            (ULONG)AnscGetPackingNumber
                (
                    (PVOID)((ULONG)pBdo->Buffer + pBdo->Offset + ulBufferOffset),
                    ulTbcSize
                );

        ulCopiedSize += ulTbcSize;
    }

    return  ulPackingNumber;
}


USHORT
AnscPdoCalChecksum
    (
        ANSC_HANDLE                 hPdo,
        ULONG                       ulOffset,
        ULONG                       ulSize
    )
{
    PANSC_PACKET_DESCRIPTOR         pPdo           = (PANSC_PACKET_DESCRIPTOR)hPdo;
    PANSC_BUFFER_DESCRIPTOR         pBdo           = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry    = NULL;
    ULONG                           ulPacketOffset = 0;
    ULONG                           ulBufferOffset = 0;
    ULONG                           ulLeftSize     = ulSize;
    ULONG                           ulCopiedSize   = 0;
    ULONG                           ulTbcSize      = 0;
    BOOL                            bFirstBdoFound = FALSE;
    ULONG                           ulChecksum     = 0;

    if ( ulOffset > AnscPdoGetPacketSize((ANSC_HANDLE)pPdo) )
    {
        return  0;
    }
    else if ( ulLeftSize == 0 )
    {
        return  0;
    }

    pSLinkEntry = AnscQueueGetFirstEntry(&pPdo->BdoQueue);

    while ( pSLinkEntry && (ulLeftSize > 0) )
    {
        pBdo        = ACCESS_ANSC_BUFFER_DESCRIPTOR(pSLinkEntry);
        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

        if ( !bFirstBdoFound )
        {
            if ( (ulPacketOffset + AnscBdoGetBlockSize(pBdo)) <= ulOffset )
            {
                ulPacketOffset += AnscBdoGetBlockSize(pBdo);

                continue;
            }
            else
            {
                bFirstBdoFound = TRUE;
                ulBufferOffset = ulOffset - ulPacketOffset;
            }
        }
        else
        {
            ulBufferOffset = 0;
        }

        ulTbcSize   = AnscBdoGetBlockSize(pBdo) - ulBufferOffset;
        ulTbcSize   = (ulTbcSize <= ulLeftSize)? ulTbcSize : ulLeftSize;
        ulLeftSize -= ulTbcSize;

        ulChecksum +=
            (ULONG)AnscGetPackingNumber
                (
                    (PVOID)((ULONG)pBdo->Buffer + pBdo->Offset + ulBufferOffset),
                    ulTbcSize
                );

        ulCopiedSize += ulTbcSize;
    }

    while ( ulChecksum >> 16 )
    {
        ulChecksum = (ulChecksum & 0xFFFF) + (ulChecksum >> 16);
    }

    return  ~((USHORT)ulChecksum);
}


ANSC_STATUS
AnscPdoConcatenate
    (
        ANSC_HANDLE                 hPdo1,
        ANSC_HANDLE                 hPdo2
    )
{
    PANSC_PACKET_DESCRIPTOR         pPdo1        = (PANSC_PACKET_DESCRIPTOR)hPdo1;
    PANSC_PACKET_DESCRIPTOR         pPdo2        = (PANSC_PACKET_DESCRIPTOR)hPdo2;
    PANSC_BUFFER_DESCRIPTOR         pBdo         = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    if ( AnscQueueQueryDepth(&pPdo2->BdoQueue) == 0 )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    pSLinkEntry = AnscQueuePopEntry(&pPdo2->BdoQueue);

    while ( pSLinkEntry )
    {
        pBdo        = ACCESS_ANSC_BUFFER_DESCRIPTOR(pSLinkEntry);
        pSLinkEntry = AnscQueuePopEntry(&pPdo2->BdoQueue);

        AnscPdoAttachBdoAtBack(pPdo1, pBdo);
    }

    return  ANSC_STATUS_SUCCESS;
}

#ifndef _ANSC_TRACE_PACKET_
ANSC_STATUS
AnscPdoShrinkLeft
    (
        ANSC_HANDLE                 hPdo,
        ULONG                       ulSize
    )
#else
ANSC_STATUS
AnscPdoShrinkLeftTrace
    (
        ANSC_HANDLE                 hPdo,
        ULONG                       ulSize,
        const char *                pFileName,
        int                         iLineNo
    )
#endif
{
    PANSC_PACKET_DESCRIPTOR         pPdo         = (PANSC_PACKET_DESCRIPTOR)hPdo;
    PANSC_BUFFER_DESCRIPTOR         pBdo         = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           ulCurOffset  = 0;
    ULONG                           ulPacketSize = AnscPdoGetPacketSize((ANSC_HANDLE)pPdo);
    ULONG                           ulLeftSize   = 0;
    BOOL                            bBdoFound    = FALSE;

    if ( ulPacketSize <= ulSize )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        ulLeftSize = ulPacketSize - ulSize;
    }

    pSLinkEntry = AnscQueueGetFirstEntry(&pPdo->BdoQueue);

    while ( pSLinkEntry )
    {
        pBdo        = ACCESS_ANSC_BUFFER_DESCRIPTOR(pSLinkEntry);
        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

        if ( bBdoFound )
        {
            AnscQueuePopEntryByLink(&pPdo->BdoQueue, &pBdo->Linkage);
#ifndef _ANSC_TRACE_PACKET_
            AnscFreeBdo((ANSC_HANDLE)pBdo);
#else
            AnscFreeBdoTrace((ANSC_HANDLE)pBdo, pFileName, iLineNo);
#endif
        }
        else if ( (ulCurOffset + AnscBdoGetBlockSize(pBdo)) < ulLeftSize )
        {
            ulCurOffset += AnscBdoGetBlockSize(pBdo);
        }
        else if ( (ulCurOffset + AnscBdoGetBlockSize(pBdo)) == ulLeftSize )
        {
            bBdoFound = TRUE;
        }
        else
        {
            AnscBdoShrinkLeft(pBdo, ulCurOffset + AnscBdoGetBlockSize(pBdo) - ulLeftSize);

            bBdoFound = TRUE;
        }
    }

    return  ANSC_STATUS_SUCCESS;
}

#ifndef _ANSC_TRACE_PACKET_
ANSC_STATUS
AnscPdoShrinkRight
    (
        ANSC_HANDLE                 hPdo,
        ULONG                       ulSize
    )
#else
ANSC_STATUS
AnscPdoShrinkRightTrace
    (
        ANSC_HANDLE                 hPdo,
        ULONG                       ulSize,
        const char *                pFileName,
        int                         iLineNo
    )
#endif
{
    PANSC_PACKET_DESCRIPTOR         pPdo         = (PANSC_PACKET_DESCRIPTOR)hPdo;
    PANSC_BUFFER_DESCRIPTOR         pBdo         = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           ulCurOffset  = 0;
    BOOL                            bBdoFound    = FALSE;

    pSLinkEntry = AnscQueueGetFirstEntry(&pPdo->BdoQueue);

    while ( pSLinkEntry )
    {
        pBdo        = ACCESS_ANSC_BUFFER_DESCRIPTOR(pSLinkEntry);
        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

        if ( (ulCurOffset + AnscBdoGetBlockSize(pBdo)) <= ulSize )
        {
            ulCurOffset += AnscBdoGetBlockSize(pBdo);

            AnscQueuePopEntryByLink(&pPdo->BdoQueue, &pBdo->Linkage);
#ifndef _ANSC_TRACE_PACKET_
            AnscFreeBdo((ANSC_HANDLE)pBdo);
#else
            AnscFreeBdoTrace((ANSC_HANDLE)pBdo, pFileName, iLineNo);
#endif
        }
        else
        {
            bBdoFound = TRUE;

            break;
        }
    }

    if ( !bBdoFound )
    {
        return  ANSC_STATUS_BAD_SIZE;
    }
    else
    {
        AnscBdoShrinkRight(pBdo, ulSize - ulCurOffset);
    }

    return  ANSC_STATUS_SUCCESS;
}


#ifndef _ANSC_TRACE_PACKET_
ANSC_STATUS
AnscPdoBreakAt
    (
        ANSC_HANDLE                 hPdo,
        ULONG                       ulOffset
    )
#else
ANSC_STATUS
AnscPdoBreakAtTrace
    (
        ANSC_HANDLE                 hPdo,
        ULONG                       ulOffset,
        const char *                pFileName,
        int                         iLineNo
    )
#endif
{
    PANSC_PACKET_DESCRIPTOR         pPdo            = (PANSC_PACKET_DESCRIPTOR)hPdo;
    PANSC_BUFFER_DESCRIPTOR         pParentBdo      = (PANSC_BUFFER_DESCRIPTOR)AnscPdoGetBdoByOffset((ANSC_HANDLE)pPdo, ulOffset);
    PANSC_BUFFER_DESCRIPTOR         pSonBdo1        = NULL;
    PANSC_BUFFER_DESCRIPTOR         pSonBdo2        = NULL;
    PVOID                           pBreakPoint     = AnscPdoGetBufferByOffset((ANSC_HANDLE)pPdo, ulOffset);
    ULONG                           ulBdo1BlockSize = 0;
    ULONG                           ulBdo2BlockSize = 0;

    if ( !pParentBdo )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else if ( pBreakPoint == AnscBdoGetBlock(pParentBdo) )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pSonBdo1 = AnscAllocateBdo(0, 0, 0);
        pSonBdo2 = AnscAllocateBdo(0, 0, 0);

        if ( !pSonBdo1 || !pSonBdo2 )
        {
            return  ANSC_STATUS_RESOURCES;
        }
    }

    ulBdo1BlockSize        = (ULONG)pBreakPoint - (ULONG)AnscBdoGetBlock(pParentBdo);
    ulBdo2BlockSize        = pParentBdo->BlockSize - ulBdo1BlockSize;

    pSonBdo1->bOwner       = FALSE;
    pSonBdo1->Buffer       = pParentBdo->Buffer;
    pSonBdo1->BufferSize   = ulBdo1BlockSize + pParentBdo->Offset;
    pSonBdo1->Offset       = pParentBdo->Offset;
    pSonBdo1->BlockSize    = ulBdo1BlockSize;
    pSonBdo1->RefCount     = 0;
    pSonBdo1->Reserved1    = pParentBdo->Reserved1;
    pSonBdo1->hOwnerObject = (ANSC_HANDLE)pParentBdo;
    pSonBdo1->pfnFreeMe    = (PFN_BDO_FREE_ME)AnscFreeSonBdo;

    AnscCopyMemory(pSonBdo1->OsSpecific, pParentBdo->OsSpecific, ANSC_BDO_OS_SPECIFIC_SIZE);

    pSonBdo2->bOwner       = FALSE;
    pSonBdo2->Buffer       = pBreakPoint;
    pSonBdo2->BufferSize   = ulBdo2BlockSize;
    pSonBdo2->Offset       = 0;
    pSonBdo2->BlockSize    = ulBdo2BlockSize;
    pSonBdo2->RefCount     = 0;
    pSonBdo2->Reserved1    = pParentBdo->Reserved1;
    pSonBdo2->hOwnerObject = (ANSC_HANDLE)pParentBdo;
    pSonBdo2->pfnFreeMe    = (PFN_BDO_FREE_ME)AnscFreeSonBdo;

    AnscCopyMemory(pSonBdo2->OsSpecific, pParentBdo->OsSpecific, ANSC_BDO_OS_SPECIFIC_SIZE);

    AnscBdoIncRefCount(pParentBdo, 2);

#ifdef _ANSC_TRACE_PACKET_
    {
        PANSC_BUFFER_REFCOUNT_TRACE pRef;
        pRef = (PANSC_BUFFER_REFCOUNT_TRACE)AnscAllocateMemory(sizeof(ANSC_BUFFER_REFCOUNT_TRACE));
        if( pRef )
        {
            _ansc_strncpy(pRef->RefFile, GetFileName(pFileName), sizeof(pRef->RefFile));
            pRef->RefFile[sizeof(pRef->RefFile)-1] = '\0';
            pRef->RefLine       = iLineNo;
            pRef->RefCountInc   = 2;
            pRef->RefCount      = pParentBdo->RefCount;
            AnscAcquireSpinLock(&gBdoTraceLock);
            AnscDListPushEntryRight(&pParentBdo->RefHead, &pRef->Link);
            AnscReleaseSpinLock(&gBdoTraceLock);
        }
    }
#endif

    AnscPdoInsertBdoAfter(pPdo, pSonBdo2, pParentBdo);
    AnscPdoInsertBdoAfter(pPdo, pSonBdo1, pParentBdo);
    AnscPdoDetachBdo     (pPdo, pParentBdo);

    return  ANSC_STATUS_SUCCESS;
}


#ifndef _ANSC_TRACE_PACKET_    
ANSC_STATUS
AnscPdoBreakAt2
    (
        ANSC_HANDLE                 hPdo,
        ULONG                       ulOffset
    )
#else
ANSC_STATUS
AnscPdoBreakAt2Trace
    (
        ANSC_HANDLE                 hPdo,
        ULONG                       ulOffset,
        const char *                pFileName,
        int                         iLineNo
    )
#endif 
{
    PANSC_PACKET_DESCRIPTOR         pPdo            = (PANSC_PACKET_DESCRIPTOR)hPdo;
    PANSC_BUFFER_DESCRIPTOR         pParentBdo      = (PANSC_BUFFER_DESCRIPTOR)AnscPdoGetBdoByOffset((ANSC_HANDLE)pPdo, ulOffset);
    PANSC_BUFFER_DESCRIPTOR         pGranmaBdo      = (PANSC_BUFFER_DESCRIPTOR)NULL;
    PANSC_BUFFER_DESCRIPTOR         pSonBdo1        = NULL;
    PANSC_BUFFER_DESCRIPTOR         pSonBdo2        = NULL;
    PVOID                           pBreakPoint     = AnscPdoGetBufferByOffset((ANSC_HANDLE)pPdo, ulOffset);
    ULONG                           ulBdo1BlockSize = 0;
    ULONG                           ulBdo2BlockSize = 0;

    if ( !pParentBdo )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else if ( pBreakPoint == AnscBdoGetBlock(pParentBdo) )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    if ( !pParentBdo->bOwner && pParentBdo->hOwnerObject && (pParentBdo->pfnFreeMe == AnscFreeSonBdo) )
    {
        pGranmaBdo = (PANSC_BUFFER_DESCRIPTOR)pParentBdo->hOwnerObject;
    }
    else
    {
        pGranmaBdo = (PANSC_BUFFER_DESCRIPTOR)NULL;
    }

    if ( !pGranmaBdo )
    {
#ifndef _ANSC_TRACE_PACKET_        
        return  AnscPdoBreakAt(hPdo, ulOffset);
#else
        return  AnscPdoBreakAtTrace(hPdo, ulOffset, pFileName, iLineNo);
#endif
    }
    else
    {
        pSonBdo1 = AnscAllocateBdo(0, 0, 0);
        pSonBdo2 = AnscAllocateBdo(0, 0, 0);

        if ( !pSonBdo1 || !pSonBdo2 )
        {
            return  ANSC_STATUS_RESOURCES;
        }
    }


    ulBdo1BlockSize        = (ULONG)pBreakPoint - (ULONG)AnscBdoGetBlock(pParentBdo);
    ulBdo2BlockSize        = pParentBdo->BlockSize - ulBdo1BlockSize;

    pSonBdo1->bOwner       = FALSE;
    pSonBdo1->Buffer       = pParentBdo->Buffer;
    pSonBdo1->BufferSize   = ulBdo1BlockSize + pParentBdo->Offset;
    pSonBdo1->Offset       = pParentBdo->Offset;
    pSonBdo1->BlockSize    = ulBdo1BlockSize;
    pSonBdo1->RefCount     = 0;
    pSonBdo1->Reserved1    = pParentBdo->Reserved1;
    pSonBdo1->hOwnerObject = (ANSC_HANDLE)pGranmaBdo;   /* attach to granma, not mother */
    pSonBdo1->pfnFreeMe    = (PFN_BDO_FREE_ME)AnscFreeSonBdo;

    AnscCopyMemory(pSonBdo1->OsSpecific, pParentBdo->OsSpecific, ANSC_BDO_OS_SPECIFIC_SIZE);

    pSonBdo2->bOwner       = FALSE;
    pSonBdo2->Buffer       = pBreakPoint;
    pSonBdo2->BufferSize   = ulBdo2BlockSize;
    pSonBdo2->Offset       = 0;
    pSonBdo2->BlockSize    = ulBdo2BlockSize;
    pSonBdo2->RefCount     = 0;
    pSonBdo2->Reserved1    = pParentBdo->Reserved1;
    pSonBdo2->hOwnerObject = (ANSC_HANDLE)pGranmaBdo;   /* attach to granma, not mother */
    pSonBdo2->pfnFreeMe    = (PFN_BDO_FREE_ME)AnscFreeSonBdo;

    AnscCopyMemory(pSonBdo2->OsSpecific, pParentBdo->OsSpecific, ANSC_BDO_OS_SPECIFIC_SIZE);

    AnscBdoIncRefCount(pGranmaBdo, 2);  /* increase reference count of granma, not mother */

#ifdef _ANSC_TRACE_PACKET_
    {
        PANSC_BUFFER_REFCOUNT_TRACE pRef;
        pRef = (PANSC_BUFFER_REFCOUNT_TRACE)AnscAllocateMemory(sizeof(ANSC_BUFFER_REFCOUNT_TRACE));
        if( pRef )
        {
            _ansc_strncpy(pRef->RefFile, GetFileName(pFileName), sizeof(pRef->RefFile));
            pRef->RefFile[sizeof(pRef->RefFile)-1] = '\0';
            pRef->RefLine       = iLineNo;
            pRef->RefCountInc   = 2;
            pRef->RefCount      = pGranmaBdo->RefCount;
            AnscAcquireSpinLock(&gBdoTraceLock);
            AnscDListPushEntryRight(&pGranmaBdo->RefHead, &pRef->Link);
            AnscReleaseSpinLock(&gBdoTraceLock);
        }
    }
#endif

    AnscPdoInsertBdoAfter(pPdo, pSonBdo2, pParentBdo);
    AnscPdoInsertBdoAfter(pPdo, pSonBdo1, pParentBdo);
    AnscPdoDetachBdo     (pPdo, pParentBdo);

    /*
     * Don't need mother anymore; granma is the mother ....
     */
    AnscFreeBdo((ANSC_HANDLE)pParentBdo);

    return  ANSC_STATUS_SUCCESS;
}


#ifndef _ANSC_TRACE_PACKET_ 
ANSC_STATUS
AnscPdoSplitAt
    (
        ANSC_HANDLE                 hPdo1,
        ANSC_HANDLE                 hPdo2,
        ULONG                       ulOffset
    )
#else
ANSC_STATUS
AnscPdoSplitAtTrace
    (
        ANSC_HANDLE                 hPdo1,
        ANSC_HANDLE                 hPdo2,
        ULONG                       ulOffset,
        const char *                pFileName,
        int                         iLineNo
    )
#endif

{
    PANSC_PACKET_DESCRIPTOR         pPdo1        = (PANSC_PACKET_DESCRIPTOR)hPdo1;
    PANSC_PACKET_DESCRIPTOR         pPdo2        = (PANSC_PACKET_DESCRIPTOR)hPdo2;
    PANSC_BUFFER_DESCRIPTOR         pBdo         = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           ulCurOffset  = 0;
    ULONG                           ulPacketSize = AnscPdoGetPacketSize((ANSC_HANDLE)pPdo1);
    BOOL                            bBdoFound    = FALSE;

    if ( ulPacketSize <= ulOffset )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    
#ifndef _ANSC_TRACE_PACKET_
    AnscPdoBreakAt((ANSC_HANDLE)pPdo1, ulOffset);
#else
    AnscPdoBreakAtTrace((ANSC_HANDLE)pPdo1, ulOffset, pFileName, iLineNo);
#endif

    pSLinkEntry = AnscQueueGetFirstEntry(&pPdo1->BdoQueue);

    while ( pSLinkEntry )
    {
        pBdo        = ACCESS_ANSC_BUFFER_DESCRIPTOR(pSLinkEntry);
        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

        if ( bBdoFound )
        {
            AnscQueuePopEntryByLink(&pPdo1->BdoQueue, &pBdo->Linkage);
            AnscQueuePushEntry     (&pPdo2->BdoQueue, &pBdo->Linkage);
        }
        else if ( (ulCurOffset + AnscBdoGetBlockSize(pBdo)) < ulOffset )
        {
            ulCurOffset += AnscBdoGetBlockSize(pBdo);
        }
        else if ( (ulCurOffset + AnscBdoGetBlockSize(pBdo)) == ulOffset )
        {
            bBdoFound = TRUE;
        }
        else
        {
            return  ANSC_STATUS_UNAPPLICABLE;
        }
    }

    pPdo2->FinalStatus     = pPdo1->FinalStatus;
    pPdo2->Flags           = pPdo1->Flags;
    pPdo2->bSyncOnly       = FALSE;
    pPdo2->bStealthy       = pPdo1->bStealthy;

    pPdo2->CurHeaderBdo    = AnscPdoGetBdoByOffset   ((ANSC_HANDLE)pPdo2, 0);
    pPdo2->CurHeaderBuffer = AnscPdoGetBufferByOffset((ANSC_HANDLE)pPdo2, 0);
    pPdo2->CurHeaderOffset = 0;
    pPdo2->ErrorCode       = pPdo1->ErrorCode;

    pPdo2->hPktPassport    = (ANSC_HANDLE)NULL;
    pPdo2->hOwnerObject    = (ANSC_HANDLE)NULL;
    pPdo2->pfnFreeMe       = NULL;

    return  ANSC_STATUS_SUCCESS;
}

/*
ANSC_STATUS
AnscPdoShrinkMiddle
    (
        ANSC_HANDLE                 hPdo,
        ULONG                       ulOffset,
        ULONG                       ulSize
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_PACKET_DESCRIPTOR         pPdo         = (PANSC_PACKET_DESCRIPTOR)hPdo;
    PANSC_BUFFER_DESCRIPTOR         pBdo         = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           ulCurOffset  = 0;
    ULONG                           ulPacketSize = AnscPdoGetPacketSize((ANSC_HANDLE)pPdo);
    BOOL                            bBdoFound    = FALSE;

    if ( ulOffset >= ulPacketSize )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else if ( (ulOffset + ulSize) > ulPacketSize )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else if ( (ulOffset + ulSize) == ulPacketSize )
    {
        return  AnscPdoShrinkLeft((ANSC_HANDLE)pPdo, ulSize);
    }

    return  ANSC_STATUS_NOT_SUPPORTED;
}
*/

static
ANSC_HANDLE
AnscAcquireBdo
    (
        void
    )
{
    PSINGLE_LINK_ENTRY              pSLinkEntry = NULL;

    if ( g_bBdoPoolingEnabled )
    {
        if ( !g_bBdoPoolInitialized )
        {
            ULONG                   i;
            
            AnscSListInitializeHeader(&g_qBdoPoolList        );
            AnscInitializeSpinLock   (&g_qBdoPoolSpinLock);

            /*
             * pre-allocate the Bdo list: 
             */
            for( i = 0; i < g_ulInitBdoPoolSize; i ++ )
            {
                ANSC_HANDLE         hBdo;

                hBdo = AnscAllocateBdo2(0, 0, 0);
                if( ! hBdo )
                {
                    break;
                }
                AnscReleaseBdo(hBdo);
            }

            g_bBdoPoolInitialized = TRUE;
        }

        AnscAcquireSpinLock(&g_qBdoPoolSpinLock);
        pSLinkEntry = AnscSListPopEntry(&g_qBdoPoolList);
        AnscReleaseSpinLock(&g_qBdoPoolSpinLock);

        if ( pSLinkEntry )
        {
            return  (ANSC_HANDLE)ACCESS_ANSC_BUFFER_DESCRIPTOR(pSLinkEntry);
        }
    }
    else
    {
        if( ! g_bBdoPoolInitialized )
        {
            AnscInitializeSpinLock   (&g_qBdoPoolSpinLock);
            g_bBdoPoolInitialized = TRUE;
        }
    }

    return  AnscAllocateBdo2(0, 0 ,0);
}


ANSC_STATUS
AnscReleaseBdo
    (
        ANSC_HANDLE                 hBdo
    )
{
    PANSC_BUFFER_DESCRIPTOR         pBdo = (PANSC_BUFFER_DESCRIPTOR)hBdo;

#ifdef _ANSC_TRACE_PACKET_
    AnscAcquireSpinLock(&gBdoTraceLock);
    AnscDListPopEntryByLink(&gBdoTraceList, &pBdo->AllocLink);
    AnscReleaseSpinLock(&gBdoTraceLock);
#endif /* _ANSC_TRACE_PACKET_ */


    if ( !g_bBdoPoolingEnabled )
    {
        AnscFreeMemory(pBdo);

        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        /*
         * race condition may occur for the queue size, but it can only make the
         * pool a little bit over-limit and should not matter.
         */
        if ( AnscSListQueryDepth(&g_qBdoPoolList) >= g_ulMaxBdoPoolSize )
        {
#ifdef _ANSC_TRACE_PACKET_
            AnscTraceWarning(("@@ AnscPacket: Bdo Pool over limit %d !!! -- Size limit %d.\n", 
                 ++g_ulFreeBdo, g_ulMaxBdoPoolSize));
#endif 
            AnscFreeMemory(pBdo);
        }
        else
        {
            AnscBdoClean((ANSC_HANDLE)pBdo);

            AnscAcquireSpinLock(&g_qBdoPoolSpinLock);
            AnscSListPushEntry(&g_qBdoPoolList, &pBdo->Linkage);
            AnscReleaseSpinLock(&g_qBdoPoolSpinLock);
        }
    }

    return  ANSC_STATUS_SUCCESS;
}


ANSC_HANDLE
#ifndef _ANSC_TRACE_PACKET_
AnscAllocateBdo
    (
        ULONG                       ulBufferSize,
        ULONG                       ulBlockOffset,
        ULONG                       ulBlockSize
    )
#else
AnscAllocateBdoTrace
    (
        ULONG                       ulBufferSize,
        ULONG                       ulBlockOffset,
        ULONG                       ulBlockSize,
        const char *                pFileName,
        int                         iLineNo
    )
#endif

{
    PANSC_BUFFER_DESCRIPTOR         pBdo         = NULL;

    pBdo = (PANSC_BUFFER_DESCRIPTOR)AnscAcquireBdo();

    if ( !pBdo )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pBdo->BufferSize = ulBufferSize;
        pBdo->Offset     = ulBlockOffset;
        pBdo->BlockSize  = ulBlockSize;
    }

    if ( ulBufferSize > 0 )
    {
        pBdo->bOwner     = TRUE;
        pBdo->Buffer     = AnscAllocateMemory(ulBufferSize + 16);
        pBdo->bUseMmap   = FALSE;
        pBdo->BufferSize = ulBufferSize;
        pBdo->Offset     = ulBlockOffset;
        pBdo->BlockSize  = ulBlockSize;

        if ( !pBdo->Buffer )
        {
            AnscReleaseBdo(pBdo);

            return  (ANSC_HANDLE)NULL;
        }
    }

#ifdef _ANSC_TRACE_PACKET_
    {
        ULONG                       count;
        
        _ansc_strncpy(pBdo->AllocFile, GetFileName(pFileName), sizeof(pBdo->AllocFile));
        pBdo->AllocFile[sizeof(pBdo->AllocFile)-1] = '\0';
        pBdo->AllocLine = iLineNo;
        AnscDListInitializeHeader(&pBdo->RefHead);
        pBdo->TmpFile   = NULL;
        pBdo->TmpLine   = -1;
        if( ! gBdoTraceInit )
        {
            AnscInitializeSpinLock(&gBdoTraceLock);
            AnscDListInitializeHeader(&gBdoTraceList);
            gBdoTraceInit = TRUE;
        }

        AnscAcquireSpinLock(&gBdoTraceLock);
        AnscDListPushEntryRight(&gBdoTraceList, &pBdo->AllocLink);
        count = AnscDListQueryDepth(&gBdoTraceList);
        AnscReleaseSpinLock(&gBdoTraceLock);

        if( 0 == (count & 1023) )
        {
            AnscBdoTracePrint(0);
        }
    }
#endif /* _ANSC_TRACE_PACKET_ */

    return  (ANSC_HANDLE)pBdo;
}


ANSC_HANDLE
AnscAllocateBdoWithMmap
    (
        ULONG                       ulBufferSize,
        ULONG                       ulBlockOffset,
        ULONG                       ulBlockSize
    )
{
    PANSC_BUFFER_DESCRIPTOR         pBdo         = NULL;

    pBdo = (PANSC_BUFFER_DESCRIPTOR)AnscAcquireBdo();

    if ( !pBdo )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pBdo->BufferSize = ulBufferSize;
        pBdo->Offset     = ulBlockOffset;
        pBdo->BlockSize  = ulBlockSize;
    }

    if ( ulBufferSize > 0 )
    {
        pBdo->bOwner     = TRUE;
        pBdo->Buffer     = mmap(NULL, ulBufferSize + 16, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
        pBdo->bUseMmap   = TRUE;
        pBdo->BufferSize = ulBufferSize;
        pBdo->Offset     = ulBlockOffset;
        pBdo->BlockSize  = ulBlockSize;
        
        if ( !pBdo->Buffer )
        {
            AnscReleaseBdo(pBdo);

            return  (ANSC_HANDLE)NULL;
        }
    }

    return  (ANSC_HANDLE)pBdo;
}


ANSC_HANDLE
AnscAllocateBdo2
    (
        ULONG                       ulBufferSize,
        ULONG                       ulBlockOffset,
        ULONG                       ulBlockSize
    )
{
    PANSC_BUFFER_DESCRIPTOR         pBdo         = NULL;

#ifdef _ANSC_TRACE_PACKET_
    if( g_ulAllocBdo == g_ulMaxBdoPoolSize  )
    {
        AnscTraceWarning(("@@ AnscPacket: Allocating new Bdo %d.\n", ++g_ulAllocBdo));
    }
#endif

    pBdo = (PANSC_BUFFER_DESCRIPTOR)AnscAllocateMemory(sizeof(ANSC_BUFFER_DESCRIPTOR));

    if ( !pBdo )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pBdo->bOwner       = FALSE;
        pBdo->Buffer       = NULL;
        pBdo->bUseMmap     = FALSE;
        pBdo->BufferSize   = ulBufferSize;
        pBdo->Offset       = ulBlockOffset;
        pBdo->BlockSize    = ulBlockSize;
        pBdo->RefCount     = 0;
        pBdo->Reserved1    = 0;

        pBdo->hOwnerObject = (ANSC_HANDLE)NULL;
        pBdo->pfnFreeMe    = NULL;
    }

    if ( ulBufferSize > 0 )
    {
        pBdo->bOwner     = TRUE;
        pBdo->Buffer     = AnscAllocateMemory(ulBufferSize + 16);
        pBdo->BufferSize = ulBufferSize;
        pBdo->Offset     = ulBlockOffset;
        pBdo->BlockSize  = ulBlockSize;

        if ( !pBdo->Buffer )
        {
            AnscFreeMemory(pBdo);

            return  (ANSC_HANDLE)NULL;
        }
    }

    return  (ANSC_HANDLE)pBdo;
}

#ifndef _ANSC_TRACE_PACKET_
ANSC_HANDLE
AnscCopyBdo
    (
        ANSC_HANDLE                 hBdo
    )
#else
ANSC_HANDLE
AnscCopyBdoTrace
    (
        ANSC_HANDLE                 hBdo,
        const char *                pFileName,
        int                         iLineNo
    )
#endif
{
    PANSC_BUFFER_DESCRIPTOR         pSrcBdo      = (PANSC_BUFFER_DESCRIPTOR)hBdo;
#ifndef _ANSC_TRACE_PACKET_
    PANSC_BUFFER_DESCRIPTOR         pDstBdo      = (PANSC_BUFFER_DESCRIPTOR)AnscAllocateBdo(0, 0, 0);
#else
    PANSC_BUFFER_DESCRIPTOR         pDstBdo      = (PANSC_BUFFER_DESCRIPTOR)AnscAllocateBdoTrace(0, 0, 0, pFileName, iLineNo);
#endif

    if ( !pDstBdo )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        *pDstBdo = *pSrcBdo;

        pDstBdo->bOwner       = FALSE;
        pDstBdo->hOwnerObject = (ANSC_HANDLE)NULL;
        pDstBdo->pfnFreeMe    = NULL;
    }

    return  (ANSC_HANDLE)pDstBdo;
}


#ifndef _ANSC_TRACE_PACKET_
ANSC_STATUS
AnscFreeBdo
    (
        ANSC_HANDLE                 hBdo
    )
#else
ANSC_STATUS
AnscFreeBdoTrace
    (
        ANSC_HANDLE                 hBdo,
        const char *                pFileName,
        int                         iLineNo
    )
#endif
{
    PANSC_BUFFER_DESCRIPTOR         pBdo = (PANSC_BUFFER_DESCRIPTOR)hBdo;


#ifdef _ANSC_TRACE_PACKET_
    if( pBdo->RefCount > 0 )
    {
        PANSC_BUFFER_REFCOUNT_TRACE pRef;
        pRef = (PANSC_BUFFER_REFCOUNT_TRACE)AnscAllocateMemory(sizeof(ANSC_BUFFER_REFCOUNT_TRACE));
        if( pRef )
        {
            _ansc_strncpy(pRef->RefFile, GetFileName(pFileName), sizeof(pRef->RefFile));
            pRef->RefFile[sizeof(pRef->RefFile)-1] = '\0';
            pRef->RefLine       = iLineNo;
            pRef->RefCountInc   = -1;
            pRef->RefCount      = pBdo->RefCount - 1;
            AnscAcquireSpinLock(&gBdoTraceLock);
            AnscDListPushEntryRight(&pBdo->RefHead, &pRef->Link);
            AnscReleaseSpinLock(&gBdoTraceLock);
        }
    }
#endif

    AnscAcquireSpinLock(&g_qBdoPoolSpinLock);

    if ( pBdo->RefCount > 0 )
    {
        pBdo->RefCount--;

        if ( pBdo->RefCount > 0 )
        {
            AnscReleaseSpinLock(&g_qBdoPoolSpinLock);

            return  ANSC_STATUS_SUCCESS;
        }
    }

    AnscReleaseSpinLock(&g_qBdoPoolSpinLock);

#ifdef _ANSC_TRACE_PACKET_
    {
        PDOUBLE_LINK_ENTRY  pEnt;
        
        for( pEnt = AnscDListGetHeadEntryLeft(&pBdo->RefHead); pEnt; )
        {
            PANSC_BUFFER_REFCOUNT_TRACE     pRef;
            
            pRef = ACCESS_CONTAINER(pEnt, ANSC_BUFFER_REFCOUNT_TRACE, Link);
            pEnt = AnscDListGetEntryRight(pEnt);

            AnscFreeMemory(pRef);
        }

        pBdo->TmpFile = pFileName;
        pBdo->TmpLine = iLineNo;
    }
#endif

    if ( pBdo->pfnFreeMe )
    {
        pBdo->pfnFreeMe(pBdo->hOwnerObject, (ANSC_HANDLE)pBdo);

        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        if ( pBdo->bOwner && pBdo->Buffer )
        {
            if (pBdo->bUseMmap)
            {
                munmap(pBdo->Buffer, pBdo->BufferSize+16);
            }
            else
            {
                AnscFreeMemory(pBdo->Buffer);
            }
        }
    }

    return  AnscReleaseBdo(hBdo);
}

/*
ANSC_STATUS
AnscFreeBdo2
    (
        ANSC_HANDLE                 hBdo
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_BUFFER_DESCRIPTOR         pBdo         = (PANSC_BUFFER_DESCRIPTOR)hBdo;

    AnscAcquireSpinLock(&g_qBdoPoolSpinLock);

    if ( pBdo->RefCount > 0 )
    {
        pBdo->RefCount--;

        if ( pBdo->RefCount > 0 )
        {
            AnscReleaseSpinLock(&g_qBdoPoolSpinLock);

            return  ANSC_STATUS_SUCCESS;
        }
    }

    AnscReleaseSpinLock(&g_qBdoPoolSpinLock);

    if ( pBdo->pfnFreeMe )
    {
        pBdo->pfnFreeMe(pBdo->hOwnerObject, (ANSC_HANDLE)pBdo);
    }
    else
    {
        if ( pBdo->bOwner && pBdo->Buffer )
        {
            AnscFreeMemory(pBdo->Buffer);
        }

        AnscFreeMemory(pBdo);
    }

    return  ANSC_STATUS_SUCCESS;
}
*/

ANSC_STATUS
AnscFreeSonBdo
    (
        ANSC_HANDLE                 hParentBdo,
        ANSC_HANDLE                 hSonBdo
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_BUFFER_DESCRIPTOR         pParentBdo   = (PANSC_BUFFER_DESCRIPTOR)hParentBdo;
    PANSC_BUFFER_DESCRIPTOR         pSonBdo      = (PANSC_BUFFER_DESCRIPTOR)hSonBdo;

    /*
    AnscFreeMemory(pSonBdo);

    returnStatus = AnscFreeBdo((ANSC_HANDLE)pParentBdo);
    */
#ifdef _ANSC_TRACE_PACKET_
    AnscAcquireSpinLock(&gBdoTraceLock);
    AnscDListPopEntryByLink(&gBdoTraceList, &pSonBdo->AllocLink);
    AnscReleaseSpinLock(&gBdoTraceLock);

    AnscFreeBdoTrace((ANSC_HANDLE)pParentBdo, pSonBdo->TmpFile, pSonBdo->TmpLine);
    pSonBdo->TmpFile = NULL;
    pSonBdo->TmpLine = -1;
#endif /* _ANSC_TRACE_PACKET_ */

    if ( !g_bBdoPoolingEnabled )
    {
        AnscFreeMemory(pSonBdo);
    }
    else
    {
        /*
         * race condition may occur for the queue size, but it can only make the
         * pool a little bit over-limit and should not matter.
         */
        if ( AnscSListQueryDepth(&g_qBdoPoolList) >= g_ulMaxBdoPoolSize )
        {
#ifdef _ANSC_TRACE_PACKET_
            AnscTraceWarning(("@@ AnscPacket: Bdo pool over limit %d !!! -- Size limt %d.\n", 
                ++g_ulFreeBdo, g_ulMaxBdoPoolSize));
#endif
            AnscFreeMemory(pSonBdo);
        }
        else
        {
            AnscBdoClean((ANSC_HANDLE)pSonBdo);

            AnscAcquireSpinLock(&g_qBdoPoolSpinLock);
            AnscSListPushEntry(&g_qBdoPoolList, &pSonBdo->Linkage);
            AnscReleaseSpinLock(&g_qBdoPoolSpinLock);
        }
    }

#ifndef _ANSC_TRACE_PACKET_
    returnStatus = AnscFreeBdo((ANSC_HANDLE)pParentBdo);
#endif

    return  returnStatus;
}


ANSC_STATUS
AnscBdoClean
    (
        ANSC_HANDLE                 hBdo
    )
{
    PANSC_BUFFER_DESCRIPTOR         pBdo       = (PANSC_BUFFER_DESCRIPTOR)hBdo;

    /*
    for ( i = 0; i < ulBdoSize / 4; i++ )
    {
        pTempUlong[i] = 0;
    }

    if ( (ulBdoSize % 4) > 0 )
    {
        pTempUchar = (PUCHAR)((ULONG)pTempUchar + i * sizeof(ULONG));

        for ( i = 0; i < ulBdoSize % 4; i++ )
        {
            pTempUchar[i] = 0;
        }
    }
    */

    /*
    AnscZeroMemory(pBdo, sizeof(ANSC_BUFFER_DESCRIPTOR));

    pBdo->bOwner       = FALSE;
    pBdo->Buffer       = NULL;
    pBdo->BufferSize   = 0;
    pBdo->Offset       = 0;
    pBdo->BlockSize    = 0;
    pBdo->RefCount     = 0;
    pBdo->Reserved1    = 0;

    pBdo->hOwnerObject = (ANSC_HANDLE)NULL;
    pBdo->pfnFreeMe    = NULL;
    */

    /*
     * Lightwight cleaning...
     */
    pBdo->bOwner    = FALSE;
    pBdo->RefCount  = 0;
    pBdo->Reserved1 = 0;
    pBdo->pfnFreeMe = NULL;

    return  ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
AnscBdoCopyFrom
    (
        ANSC_HANDLE                 hBdo,
        PVOID                       pDstBuffer,
        PULONG                      pulCopySize,
        ULONG                       ulOffset
    )
{
    PANSC_BUFFER_DESCRIPTOR         pBdo         = (PANSC_BUFFER_DESCRIPTOR)hBdo;
    ULONG                           ulTbcSize    = *pulCopySize;

    if ( ulOffset >= pBdo->BlockSize )
    {
        return  ANSC_STATUS_BAD_OFFSET;
    }
    else if ( ulTbcSize == 0 )
    {
        return  ANSC_STATUS_BAD_SIZE;
    }
    else
    {
        ulTbcSize = (ulTbcSize > (pBdo->BlockSize - ulOffset))? (pBdo->BlockSize - ulOffset) : ulTbcSize;
    }

    AnscCopyMemory
        (
            pDstBuffer,
            (PVOID)((ULONG)pBdo->Buffer + pBdo->Offset + ulOffset),
            ulTbcSize
        );

    *pulCopySize = ulTbcSize;

    return  ANSC_STATUS_SUCCESS;
}

#ifndef _ANSC_TRACE_PACKET_
ANSC_STATUS
AnscBdoBreakAt
    (
        ANSC_HANDLE                 hBdo,
        ULONG                       ulOffset,
        PANSC_HANDLE                phHeadBdo,
        PANSC_HANDLE                phTailBdo
    )
#else
ANSC_STATUS
AnscBdoBreakAtTrace
    (
        ANSC_HANDLE                 hBdo,
        ULONG                       ulOffset,
        PANSC_HANDLE                phHeadBdo,
        PANSC_HANDLE                phTailBdo,
        const char *                pFileName,
        int                         iLineNo
    )
#endif
    
{
    PANSC_BUFFER_DESCRIPTOR         pParentBdo      = (PANSC_BUFFER_DESCRIPTOR)hBdo;
    PANSC_BUFFER_DESCRIPTOR         pHeadBdo        = NULL;
    PANSC_BUFFER_DESCRIPTOR         pTailBdo        = NULL;
    PVOID                           pBreakPoint     = (PVOID)((ULONG)pParentBdo->Buffer + pParentBdo->Offset + ulOffset);
    ULONG                           ulHeadBlockSize = 0;
    ULONG                           ulTailBlockSize = 0;

    if ( ulOffset >= pParentBdo->BlockSize )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else if ( ulOffset == 0 )
    {
        *phHeadBdo = (ANSC_HANDLE)pParentBdo;
        *phTailBdo = (ANSC_HANDLE)NULL;

        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
#ifndef _ANSC_TRACE_PACKET_      
        pHeadBdo = AnscAllocateBdo(0, 0, 0);
        pTailBdo = AnscAllocateBdo(0, 0, 0);
#else
        pHeadBdo = AnscAllocateBdoTrace(0, 0, 0, pFileName, iLineNo);
        pTailBdo = AnscAllocateBdoTrace(0, 0, 0, pFileName, iLineNo);
#endif
        if ( !pHeadBdo || !pTailBdo )
        {
            *phHeadBdo = (ANSC_HANDLE)NULL;
            *phTailBdo = (ANSC_HANDLE)NULL;

            return  ANSC_STATUS_RESOURCES;
        }
    }

    ulHeadBlockSize        = ulOffset;
    ulTailBlockSize        = pParentBdo->BlockSize - ulHeadBlockSize;

    pHeadBdo->bOwner       = FALSE;
    pHeadBdo->Buffer       = pParentBdo->Buffer;
    pHeadBdo->BufferSize   = ulHeadBlockSize + pParentBdo->Offset;
    pHeadBdo->Offset       = pParentBdo->Offset;
    pHeadBdo->BlockSize    = ulHeadBlockSize;
    pHeadBdo->RefCount     = 0;
    pHeadBdo->Reserved1    = pParentBdo->Reserved1;
    pHeadBdo->hOwnerObject = (ANSC_HANDLE)pParentBdo;
    pHeadBdo->pfnFreeMe    = (PFN_BDO_FREE_ME)AnscFreeSonBdo;

    AnscCopyMemory(pHeadBdo->OsSpecific, pParentBdo->OsSpecific, ANSC_BDO_OS_SPECIFIC_SIZE);

    pTailBdo->bOwner       = FALSE;
    pTailBdo->Buffer       = pBreakPoint;
    pTailBdo->BufferSize   = ulTailBlockSize;
    pTailBdo->Offset       = 0;
    pTailBdo->BlockSize    = ulTailBlockSize;
    pTailBdo->RefCount     = 0;
    pTailBdo->Reserved1    = pParentBdo->Reserved1;
    pTailBdo->hOwnerObject = (ANSC_HANDLE)pParentBdo;
    pTailBdo->pfnFreeMe    = (PFN_BDO_FREE_ME)AnscFreeSonBdo;

    AnscCopyMemory(pTailBdo->OsSpecific, pParentBdo->OsSpecific, ANSC_BDO_OS_SPECIFIC_SIZE);

    AnscBdoIncRefCount(pParentBdo, 2);

    *phHeadBdo = (ANSC_HANDLE)pHeadBdo;
    *phTailBdo = (ANSC_HANDLE)pTailBdo;

    return  ANSC_STATUS_SUCCESS;
}


#ifdef _ANSC_TRACE_PACKET_


#define GnpsEdoLinuxBdoFreeMe       1
#define ssp_DslgCmdFreeBdoBuf       2
#define DslgProfilingEdoEouFreeMe   3



void
AnscBdoTracePrint
    (
        int                         type
    )
{
    PDOUBLE_LINK_ENTRY              pEntry;
    PANSC_BUFFER_DESCRIPTOR         pBdo;
    ULONG                           ulIndex = 0, ulMax;

    AnscTraceError(("######### Total BDO Count:  %d. ###########\n",AnscDListQueryDepth(&gBdoTraceList)));
    if( type == 0 )
    {
        return;        
    }

    ulMax = (type == 1) ? 1024 : (ULONG)-1;

    AnscTraceError(("No   Address   File       Line   RefCount  Reserved1  bOwner  pfnFreeMe  FreeMename\n"));

    AnscAcquireSpinLock(&gBdoTraceLock);

    for( pEntry = AnscDListGetHeadEntryLeft(&gBdoTraceList); pEntry; ulIndex ++ )
    {
        PDOUBLE_LINK_ENTRY          pEntRef;
        const   char *              name;
        ULONG                       i;

        if( ulIndex >= ulMax )
        {
            break;
        }
            
        pBdo    = ACCESS_CONTAINER(pEntry, ANSC_BUFFER_DESCRIPTOR, AllocLink);
        pEntry  = AnscDListGetEntryRight(pEntry);


        if( pBdo->pfnFreeMe == GnpsEdoLinuxBdoFreeMe )
        {
            name = "GnpsEdoLinuxBdoFreeMe";
        }
        else if(pBdo->pfnFreeMe == ssp_DslgCmdFreeBdoBuf )
        {
            name = "ssp_DslgCmdFreeBdoBuf";
        }
        else if(pBdo->pfnFreeMe == DslgProfilingEdoEouFreeMe )
        {
            name = "DslgProfilingEdoEouFreeMe";
        }
        else
        {
            name = "Unknown_function";
        }

        AnscTraceError
            ((
                "%6d   0x%08x  %24s  %6d   %4d  0x%08x  %1d  0x%08x  %24s  \n",
                ulIndex,
                pBdo,
                pBdo->AllocFile,
                pBdo->AllocLine,
                pBdo->RefCount,
                pBdo->Reserved1,
                pBdo->bOwner,
                pBdo->pfnFreeMe,
                name
            ));

        for( pEntRef = AnscDListGetHeadEntryLeft(&pBdo->RefHead), i = 0; pEntRef; i ++ )
        {
            PANSC_BUFFER_REFCOUNT_TRACE pRef;

            pRef = ACCESS_CONTAINER(pEntRef, ANSC_BUFFER_REFCOUNT_TRACE, Link);
            pEntRef = AnscDListGetEntryRight(pEntRef);

            AnscTraceError
                ((
                    "%6d   0x%08x  %24s  %6d   %4d    %8d\n",
                    ulIndex,
                    i,
                    pRef->RefFile,
                    pRef->RefLine,
                    pRef->RefCountInc,
                    pRef->RefCount
                ));
        }
    }    

    AnscReleaseSpinLock(&gBdoTraceLock);

}


#endif /* _ANSC_TRACE_PACKET_ */


void
AnscPacketCleanup
    (
        void
    )
{
    PSINGLE_LINK_ENTRY              pEnt;

    /*
     * free pdo pool
     */
    AnscAcquireSpinLock(&g_qPdoPoolSpinLock);

    for( pEnt = AnscSListGetFirstEntry(&g_qPdoPoolList); pEnt; )
    {
        PANSC_PACKET_DESCRIPTOR     pPdo;

        pPdo = (PANSC_PACKET_DESCRIPTOR)ACCESS_ANSC_PACKET_DESCRIPTOR(pEnt); 
        pEnt = AnscSListGetNextEntry(pEnt);

        AnscFreePdo2(pPdo);
    }
    AnscSListInitializeHeader(&g_qPdoPoolList);
    AnscReleaseSpinLock(&g_qPdoPoolSpinLock);
    AnscFreeSpinLock(&g_qPdoPoolSpinLock);
    g_bPdoPoolInitialized = FALSE;


    /*
     * free bdo pool
     */
    AnscAcquireSpinLock(&g_qBdoPoolSpinLock);

    for( pEnt = AnscSListGetFirstEntry(&g_qBdoPoolList); pEnt; )
    {
        PANSC_BUFFER_DESCRIPTOR     pBdo;

        pBdo = (PANSC_BUFFER_DESCRIPTOR)ACCESS_ANSC_BUFFER_DESCRIPTOR(pEnt); 
        pEnt = AnscSListGetNextEntry(pEnt);

        AnscFreeMemory(pBdo);
    }
    AnscSListInitializeHeader(&g_qBdoPoolList);
    AnscReleaseSpinLock(&g_qBdoPoolSpinLock);
    AnscFreeSpinLock(&g_qBdoPoolSpinLock);
    g_bBdoPoolInitialized = FALSE;

#ifdef _ANSC_TRACE_PACKET_    
    g_ulAllocPdo    = 0;
    g_ulAllocBdo    = 0;
    g_ulFreePdo     = 0;
    g_ulFreeBdo     = 0;
#endif

}
