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

    module:	kernel_protection.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco System  , Inc., 1997 ~ 2001
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file implements the platform dependent routines.

        *   KernelRealInitializeLock
        *   KernelRealAcquireLock
        *   KernelRealReleaseLock
        *   KernelRealFreeLock

        *   KernelInitializeEvent
        *   KernelSetEvent
        *   KernelResetEvent
        *   KernelPulseEvent
        *   KernelWaitEvent
        *   KernelFreeEvent

    ---------------------------------------------------------------

    environment:

        atmos 7.1   and later

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Hua Ding

    ---------------------------------------------------------------

    revision:

        01/19/01    initial revision.
        04/24/02    atmos port

**********************************************************************/

#include "quantum/as.h"
#include "kernel_base.h"
#include "kernel_debug.h"
#include "kernel_memory.h"
#include "kernel_protection.h"
#include "kernel_string.h"
#include "kernel_time.h"
#include "kernel_socket.h"
#include "kernel_task.h"

#include "quantum/as_timer.h"
#include "quantum/as_task.h"

/**********************************************************************
                           DEFINITIONS
**********************************************************************/

#define  USE_MSG_BASED_EVENT

#define  KERNEL_MAX_SEMAPHORE_NUM                   22000
#define  KERNEL_MAX_MESSAGE_NUM                     20

typedef  struct
_KERNEL_EVENT_WAIT
{
    ULONG                           NumberSignaled;
    ULONG                           NumberWaiting;
}
KERNEL_EVENT_WAIT,   *PKERNEL_EVENT_WAIT;


typedef struct
_KERNEL_EVENT_MSG
{
    ULONG                           NumberSignaled;
    KERNEL_SLIST_HEADER             TaskList;
}
KERNEL_EVENT_MSG, *PKERNEL_EVENT_MSG;


typedef struct
_KERNEL_FREE_MSG
{
    struct  _KERNEL_FREE_MSG   *Next;
    ATMOS_MESSAGE               Msg;
}
KERNEL_FREE_MSG, *PKERNEL_FREE_MSG;

#define  ACCESS_FREE_MSG(address)                   \
         ((PKERNEL_FREE_MSG)((PCHAR)(address) - (ULONG)(&((PKERNEL_FREE_MSG)0)->Msg)))


typedef struct
_KERNEL_TASK_LIST_ENTRY
{
    KERNEL_SLIST_ENTRY              Linkage;
    ATMOS_MQID                      TaskQid;
    char                            TaskName[KERNEL_TASK_NAME_LENGTH];
}
KERNEL_TASK_LIST_ENTRY, *PKERNEL_TASK_LIST_ENTRY;

#define  ACCESS_TASK_LIST_ENTRY(address)            \
         ((PKERNEL_TASK_LIST_ENTRY)((PCHAR)(address) - (ULONG)(&((PKERNEL_TASK_LIST_ENTRY)0)->Linkage)))


/**********************************************************************
                           GLOBAL VARIABLES
**********************************************************************/

static const char*          semaphore_name  = "/semaphore/bisgaSema";
static ULONG                gulSemaphoreId  = 0;
static BOOL                 bSemaphoreInit  = FALSE;
static PKERNEL_FREE_LOCK    pFreeLockHeader;
static ULONG                gulLockCount    = 0;

static BOOL                 bEventMsgInit   = FALSE;
static PKERNEL_FREE_MSG     pFreeMsgHeader; 

static KERNEL_LOCK          EventLock;

BOOL                        bEnableLock     = TRUE;

/**********************************************************************
    function prototype for LOCK and EVENT mechanism
**********************************************************************/

PKERNEL_SLIST_ENTRY
KernelSListPopEntry
    (
        PKERNEL_SLIST_HEADER        SListHeader
    );

BOOLEAN
KernelSListPopEntryByLink
    (
        PKERNEL_SLIST_HEADER        SListHeader,
        PKERNEL_SLIST_ENTRY         Entry
    );

/**********************************************************************
    Single link list operation functions
**********************************************************************/

#define  KernelSListGetFirstEntry(ListHead)         (ListHead)->Next.Next

PKERNEL_SLIST_ENTRY
KernelSListPopEntry
    (
        PKERNEL_SLIST_HEADER       SListHeader
    )
{
    PKERNEL_SLIST_ENTRY            FirstEntry = NULL;

    FirstEntry = (SListHeader)->Next.Next;

    if ( FirstEntry != NULL )
    {
        (SListHeader)->Next.Next = FirstEntry->Next;

        if ( FirstEntry->Next == NULL)
        {
            (SListHeader)->Last.Next = NULL;
        }
    }

    return  FirstEntry;
}

BOOLEAN
KernelSListPopEntryByLink
    (
        PKERNEL_SLIST_HEADER       SListHeader,
        PKERNEL_SLIST_ENTRY        Entry
    )
{
    PKERNEL_SLIST_ENTRY             NextEntry = NULL;
    BOOLEAN                         bFound    = FALSE;

    NextEntry = &SListHeader->Next;

    while ( NextEntry != NULL )
    {
        if ( NextEntry->Next == Entry )
        {
            NextEntry->Next = Entry->Next;

            bFound = TRUE;
            break;
        }

        NextEntry = NextEntry->Next;
    }

    if ( (SListHeader)->Last.Next == Entry )
    {
        if ( (SListHeader)->Next.Next == NULL )
        {
            (SListHeader)->Last.Next = NULL;
        }
        else
        {
            (SListHeader)->Last.Next = NextEntry;
        }
    }

    return  bFound;
}


/**********************************************************************
    Lock Implementation for ISOS 9.0 and later

    *   KernelRealInitializeLock
    *   KernelRealAcquireLock
    *   KernelRealReleaseLock
    *   KernelRealFreeLock
    *   KernelLockMessageHandler

**********************************************************************/

#ifdef  KERNEL_LOCK_USE_SEMAPHORE

BOOLEAN
KernelRealInitializeLock
    (
        PKERNEL_LOCK                pLock,
        const char*                 pFileName,
        ULONG                       ulLineNumber
    )
{
    BITS                            critLevel;
    BOOLEAN                         retStatus       = TRUE;
    tASErr                          err;
    char                            cSemaName[32];

    if ( !bEnableLock )
    {
        return retStatus;
    }

    critLevel = atmos_startcritical();

    if ( !bSemaphoreInit )
    {
        int                 i;
        PKERNEL_FREE_LOCK   pFreeSema;

        pFreeLockHeader = NULL;

        for ( i = 0; i < KERNEL_MAX_SEMAPHORE_NUM; i++ )
        {
            if ( i % (KERNEL_MAX_SEMAPHORE_NUM / 10) == 0 )
            {
                KernelTrace2
                    (
                        KERNEL_DBG_LEVEL_TRACE_FLOW,
                        KERNEL_DBG_MASK_LOCK,
                        "KernelRealInitializeLock -- %03d%% finished!!\n",
                        (i * 100 / KERNEL_MAX_SEMAPHORE_NUM)
                    );
            }
            
            pFreeSema =
                (PKERNEL_FREE_LOCK)KernelAllocateMemory(sizeof(KERNEL_FREE_LOCK));

            if ( pFreeSema == NULL )
            {
                KernelTrace2
                    (
                        KERNEL_DBG_LEVEL_WARNING,
                        KERNEL_DBG_MASK_LOCK,
                        "KernelRealInitializeLock -- run out of memory!!\n"
                    );

                retStatus = FALSE;
                goto EXIT1;
            }

            pFreeSema->semaLock = NULL;
            pFreeSema->Next     = pFreeLockHeader;
            pFreeLockHeader     = pFreeSema;
        }

        KernelTrace2
            (
                KERNEL_DBG_LEVEL_TRACE_FLOW,
                KERNEL_DBG_MASK_LOCK,
                "KernelRealInitializeLock -- %03d%% finished!!\n",
                (i * 100 / KERNEL_MAX_SEMAPHORE_NUM)
            );

        bSemaphoreInit = TRUE;
    }

    if ( pLock == NULL)
    {
        KernelTrace2
            (
                KERNEL_DBG_LEVEL_WARNING,
                KERNEL_DBG_MASK_LOCK,
                "KernelRealInitializeLock -- NULL lock, file -- %s, line %lu!!\n",
                (pFileName ? pFileName : "No file name"),
                ulLineNumber
            );

        retStatus = FALSE;
        goto EXIT1; 
    }

    if ( !bSemaphoreInit || pFreeLockHeader == NULL )
    {
        KernelTrace2
            (
                KERNEL_DBG_LEVEL_WARNING,
                KERNEL_DBG_MASK_LOCK,
                "KernelRealInitializeLock -- run out of FREE LOCK, %u, task: %s!!\n",
                bSemaphoreInit,
                atmos_pcb_current_get_name()
            );

        retStatus = FALSE;
        goto EXIT1; 
    }

    gulLockCount   += 1;
    *pLock          = pFreeLockHeader;
    pFreeLockHeader = (*pLock)->Next;
    (*pLock)->Next  = NULL;

    atmos_endcritical(critLevel);

    if ( (*pLock)->semaLock == NULL )
    {
        sprintf(cSemaName, "%s%05u", semaphore_name, gulSemaphoreId);
        gulSemaphoreId += 1;

        err = as_SemaphoreNew
            (
                &(*pLock)->semaLock, 
                (const char*)cSemaName, 
                ""
            );
    
        if (kASEOK != err)
        {
            retStatus                = FALSE;

            KernelTrace2
                (
                    KERNEL_DBG_LEVEL_WARNING,
                    KERNEL_DBG_MASK_LOCK,
                    "KernelRealInitializeLock -- new semaphore failed!!\n"
                );

            return retStatus;
        }

        as_SemaphorePost((*pLock)->semaLock);
    }

EXIT1:
    atmos_endcritical(critLevel);
    return retStatus;
}


VOID
KernelRealAcquireLock
    (
        PKERNEL_LOCK                pLock,
        const char*                 pFileName,
        ULONG                       ulLineNumber
    )
{
    tASErr                          err;

    if ( !bEnableLock )
    {
        return;
    }

    KernelTrace2
        (
            KERNEL_DBG_LEVEL_TRACE_FLOW,
            KERNEL_DBG_MASK_LOCK,
            "KernelRealAcquireLock -- Lock: 0x%x, Task: %s.\n",
            pLock,
            atmos_pcb_current_get_name()
        );

    /*
     *  if the compiler doesn't do it in the right way -- if the first condition is satified,
     *  doesn't go the second one, we will need to switch to use AND.
     */
    if ( (pLock == NULL) || (*pLock == NULL) )
    {
        KernelTrace2
            (
                KERNEL_DBG_LEVEL_WARNING,
                KERNEL_DBG_MASK_LOCK,
                "KernelRealAcquireLock -- NULL lock, Task: %s, file -- %s, line %lu!!\n",
                atmos_pcb_current_get_name(),
                (pFileName ? pFileName : "No file name"),
                ulLineNumber
            );

        return;
    }

    as_SemaphoreWait((*pLock)->semaLock);

    KernelTrace2
        (
            KERNEL_DBG_LEVEL_TRACE_FLOW,
            KERNEL_DBG_MASK_LOCK,
            "KernelRealAcquireLock -- done, task = %s.\n",
            atmos_pcb_current_get_name()
        );

    return;
}


VOID
KernelRealReleaseLock
    (
        PKERNEL_LOCK                pLock,
        const char*                 pFileName,
        ULONG                       ulLineNumber
    )
{
    tASErr                          err;

    if ( !bEnableLock )
    {
        return;
    }

    KernelTrace2
        (
            KERNEL_DBG_LEVEL_TRACE_FLOW,
            KERNEL_DBG_MASK_LOCK,
            "KernelRealReleaseLock -- Lock: 0x%x, Task: %s.\n",
            pLock,
            atmos_pcb_current_get_name()
        );


    if ( pLock == NULL || *pLock == NULL )
    {
        KernelTrace2
            (
                KERNEL_DBG_LEVEL_WARNING,
                KERNEL_DBG_MASK_LOCK,
                "KernelRealReleaseLock -- NULL lock, Task: %s, file -- %s, line %lu!!\n",
                atmos_pcb_current_get_name(),
                (pFileName ? pFileName : "No file name"),
                ulLineNumber
            );

        return;
    }

    as_SemaphorePost((*pLock)->semaLock);

    KernelTrace2
        (
            KERNEL_DBG_LEVEL_TRACE_FLOW,
            KERNEL_DBG_MASK_LOCK,
            "KernelRealReleaseLock -- done, task = %s.\n",
            atmos_pcb_current_get_name()
        );

    return;
}


VOID
KernelRealFreeLock
    (
        PKERNEL_LOCK                pLock,
        const char*                 pFileName,
        ULONG                       ulLineNumber
    )
{
    BITS                            critLevel;
    tASErr                          err;

    if ( !bEnableLock )
    {
        return;
    }

    KernelTrace2
        (
            KERNEL_DBG_LEVEL_TRACE_FLOW,
            KERNEL_DBG_MASK_LOCK,
            "KernelRealFreeLock -- task = %s.\n",
            atmos_pcb_current_get_name()
        );

    if ( pLock == NULL || *pLock == NULL )
    {
        KernelTrace2
            (
                KERNEL_DBG_LEVEL_WARNING,
                KERNEL_DBG_MASK_LOCK,
                "KernelRealFreeLock -- NULL lock: 0x%lX, task: %s, file -- %s, line %lu!!\n",
                pLock,
                atmos_pcb_current_get_name(),
                (pFileName ? pFileName : "No file name"),
                ulLineNumber
            );

        return;
    }

    critLevel = atmos_startcritical();

    if ( !bSemaphoreInit )
    {
        KernelTrace2
            (
                KERNEL_DBG_LEVEL_WARNING,
                KERNEL_DBG_MASK_LOCK,
                "KernelRealInitializeLock -- not initialized, task: %s, file -- %s, line %lu!!\n",
                atmos_pcb_current_get_name(),
                (pFileName ? pFileName : "No file name"),
                ulLineNumber
            );

        return;
    }

    gulLockCount   -= 1;
    (*pLock)->Next  = pFreeLockHeader;
    pFreeLockHeader = *pLock;
    *pLock          = NULL;

    atmos_endcritical(critLevel);

    KernelTrace2
        (
            KERNEL_DBG_LEVEL_TRACE_FLOW,
            KERNEL_DBG_MASK_LOCK,
            "KernelRealFreeLock -- done, task = %s.\n",
            atmos_pcb_current_get_name()
        );

    return;
}


ULONG
KernelGetLockCount(VOID)
{
    return gulLockCount;
}


#endif   /*KERNEL_LOCK_USE_SEMAPHORE*/


/**********************************************************************
    Event Implementation for ISOS 9.0 and later

    *   KernelInitializeEvent
    *   KernelSetEvent
    *   KernelResetEvent
    *   KernelPulseEvent
    *   KernelWaitEvent
    *   KernelFreeEvent
    *   KernelEventMessageHandler

**********************************************************************/

#ifdef   USE_MSG_BASED_EVENT

BOOLEAN
KernelInitializeEvent
    (
        PKERNEL_EVENT               pBaseEvent
    )
{
    PKERNEL_EVENT_MSG               pEvent          = (PKERNEL_EVENT_MSG)pBaseEvent;
    BITS                            critLevel;
    BOOLEAN                         retStatus       = TRUE;

    KernelTrace2
        (
            KERNEL_DBG_LEVEL_TRACE_FLOW,
            KERNEL_DBG_MASK_EVENT,
            "KernelInitializeEvent -- Handle: 0x%lX, Task: %s.\n",
            pEvent,
            atmos_pcb_current_get_name()
        );

    critLevel = atmos_startcritical();

    if ( !bEventMsgInit )
    {
        int                 i;
        PKERNEL_FREE_MSG    pTmpMsg;
    
        pFreeMsgHeader = NULL;

        for (i = 0; i < KERNEL_MAX_MESSAGE_NUM; i++)
        {
            pTmpMsg =
                (PKERNEL_FREE_MSG)KernelAllocateMemory(sizeof(KERNEL_FREE_MSG));

            pTmpMsg->Next   = pFreeMsgHeader;
            pFreeMsgHeader  = pTmpMsg;
        }

        /* KernelRealInitializeLock(&EventLock,  __FILE__, __LINE__); */

        bEventMsgInit = TRUE;
    }

    if ( pEvent == NULL)
    {
        KernelTrace2
            (
                KERNEL_DBG_LEVEL_WARNING,
                KERNEL_DBG_MASK_EVENT,
                "KernelInitializeEvent -- NULL Event!!\n"
            );

        retStatus = FALSE;
        goto EXIT1; 
    }

    pEvent->NumberSignaled = 0;
    KernelSListInitializeHeader(&pEvent->TaskList);

EXIT1:

    atmos_endcritical(critLevel);
    KernelTrace2
        (
            KERNEL_DBG_LEVEL_TRACE_FLOW,
            KERNEL_DBG_MASK_EVENT,
            "KernelInitializeEvent -- done, Handle: 0x%lX, Task: %s.\n",
            pEvent,
            atmos_pcb_current_get_name()
        );

    return retStatus;
}


VOID
KernelSetEvent
    (
        PKERNEL_EVENT               pBaseEvent
    )
{
    PKERNEL_EVENT_MSG               pEvent          = (PKERNEL_EVENT_MSG)pBaseEvent;
    PKERNEL_SLIST_ENTRY             pSListEntry;
    PKERNEL_TASK_LIST_ENTRY         pTaskListEntry;
    BITS                            critLevel;

    KernelTrace2
        (
            KERNEL_DBG_LEVEL_TRACE_FLOW,
            KERNEL_DBG_MASK_EVENT,
            "KernelSetEvent -- Handle: 0x%x, Task: %s.\n",
            pEvent,
            atmos_pcb_current_get_name()
        );


    if ( pEvent == NULL)
    {
        KernelTrace2
            (
                KERNEL_DBG_LEVEL_WARNING,
                KERNEL_DBG_MASK_EVENT,
                "KernelSetEvent -- NULL Event, Task: %s!!\n",
                atmos_pcb_current_get_name()
            );

        return;
    }

    critLevel = atmos_startcritical();
    /*KernelRealAcquireLock(&EventLock,  __FILE__, __LINE__);*/

    pSListEntry = KernelSListPopEntry(&pEvent->TaskList);

    atmos_endcritical(critLevel);
    /*KernelRealReleaseLock(&EventLock,  __FILE__, __LINE__);*/

    if ( pSListEntry != NULL )
    {
        pTaskListEntry = ACCESS_TASK_LIST_ENTRY(pSListEntry);

        if ( bEventMsgInit && pFreeMsgHeader )
        {
            PKERNEL_FREE_MSG        pFreeMsg;

            critLevel = atmos_startcritical();
            /*KernelRealAcquireLock(&EventLock,  __FILE__, __LINE__);*/

            /*
             *  grab a pre-allocated message buffer
             */
            pFreeMsg = pFreeMsgHeader;

            if ( pFreeMsg != NULL )
            {
                MSG_D_KTX_AL_SET_EVENT(pSetEvent, &pFreeMsg->Msg);

                pFreeMsgHeader = pFreeMsg->Next;

                pSetEvent->EventHandle = (U32*)pEvent;

                atmos_endcritical(critLevel);
                /*KernelRealReleaseLock(&EventLock,  __FILE__, __LINE__);*/

                KernelTrace2
                    (
                        KERNEL_DBG_LEVEL_TRACE_FLOW,
                        KERNEL_DBG_MASK_EVENT,
                        "KernelSetEvent -- event 0x%lX, send message to qid = 0x%lX, task %s...\n",
                        pEvent,
                        pTaskListEntry->TaskQid,
                        pTaskListEntry->TaskName
                    );

                sendmessage
                    (
                        &pFreeMsg->Msg, 
                        MSG_N_KTX_AL_SET_EVENT, 
                        pTaskListEntry->TaskQid
                    );
            }
            else
            {
                KernelTrace2
                    (
                        KERNEL_DBG_LEVEL_WARNING,
                        KERNEL_DBG_MASK_EVENT,
                        "KernelSetEvent -- event 0x%lX, run out of free msg, %u.\n",
                        pEvent,
                        bEventMsgInit
                    );

                atmos_endcritical(critLevel);
                /*KernelRealReleaseLock(&EventLock,  __FILE__, __LINE__);*/
            }
        }
        else
        {
            KernelTrace2
                (
                    KERNEL_DBG_LEVEL_WARNING,
                    KERNEL_DBG_MASK_EVENT,
                    "KernelSetEvent -- event 0x%lX, run out of free msg, %u.\n",
                    pEvent,
                    bEventMsgInit
                );
        }

        KernelFreeMemory(pTaskListEntry);
    }
    else
    {
        critLevel = atmos_startcritical();
        /*KernelRealAcquireLock(&EventLock,  __FILE__, __LINE__);*/

        pEvent->NumberSignaled += 1;

        atmos_endcritical(critLevel);
        /*KernelRealReleaseLock(&EventLock,  __FILE__, __LINE__);*/
    }

    KernelTrace2
        (
            KERNEL_DBG_LEVEL_TRACE_FLOW,
            KERNEL_DBG_MASK_EVENT,
            "KernelSetEvent -- done, event = 0x%lX, task = %s.\n",
            pEvent,
            atmos_pcb_current_get_name()
        );

    return;
}


VOID
KernelResetEvent
    (
        PKERNEL_EVENT               pBaseEvent
    )
{
    PKERNEL_EVENT_MSG               pEvent          = (PKERNEL_EVENT_MSG)pBaseEvent;
    PKERNEL_TASK_LIST_ENTRY         pTaskListEntry;
    PKERNEL_SLIST_ENTRY             pSListEntry;
    BITS                            critLevel;

    KernelTrace2
        (
            KERNEL_DBG_LEVEL_TRACE_FLOW,
            KERNEL_DBG_MASK_EVENT,
            "KernelResetEvent -- Handle: 0x%lX, Task = %s.\n",
            pEvent,
            atmos_pcb_current_get_name()
        );


    if ( pEvent == NULL)
    {
        KernelTrace2
            (
                KERNEL_DBG_LEVEL_WARNING,
                KERNEL_DBG_MASK_EVENT,
                "KernelResetEvent -- NULL Event, Task = %s!!\n",
                atmos_pcb_current_get_name()
            );

        return;
    }

    critLevel = atmos_startcritical();
    /*KernelRealAcquireLock(&EventLock,  __FILE__, __LINE__);*/

    /*
     *  no action taken on the current waiting tasks -- they still pend there.
     *
    pSListEntry = KernelSListPopEntry(&pEvent->TaskList);
    while ( pSListEntry )
    {
        pTaskListEntry = ACCESS_TASK_LIST_ENTRY(pSListEntry);
        pSListEntry    = KernelSListPopEntry(&pEvent->TaskList);

        KernelFreeMemory(pTaskListEntry);
    }
     */

    pEvent->NumberSignaled = 0;

    atmos_endcritical(critLevel);
    /*KernelRealReleaseLock(&EventLock,  __FILE__, __LINE__);*/

    KernelTrace2
        (
            KERNEL_DBG_LEVEL_TRACE_FLOW,
            KERNEL_DBG_MASK_EVENT,
            "KernelResetEvent -- done, event = 0x%lX, task = %s.\n",
            pEvent,
            atmos_pcb_current_get_name()
        );

    return;
}


VOID
KernelPulseEvent
    (
        PKERNEL_EVENT               pBaseEvent
    )
{
    PKERNEL_EVENT_MSG               pEvent          = (PKERNEL_EVENT_MSG)pBaseEvent;
    PKERNEL_TASK_LIST_ENTRY         pTaskListEntry;
    PKERNEL_SLIST_ENTRY             pSListEntry;
    BITS                            critLevel;

    KernelTrace2
        (
            KERNEL_DBG_LEVEL_TRACE_FLOW,
            KERNEL_DBG_MASK_EVENT,
            "KernelPulseEvent -- Handle = 0x%lX, Task = %s.\n",
            pEvent,
            atmos_pcb_current_get_name()
        );

    if ( pEvent == NULL)
    {
        KernelTrace2
            (
                KERNEL_DBG_LEVEL_WARNING,
                KERNEL_DBG_MASK_EVENT,
                "KernelPulseEvent -- NULL Event, Task: %s!!\n",
                atmos_pcb_current_get_name()
            );

        return;
    }

    critLevel = atmos_startcritical();
    /*KernelRealAcquireLock(&EventLock,  __FILE__, __LINE__);*/

    pSListEntry = KernelSListPopEntry(&pEvent->TaskList);

    atmos_endcritical(critLevel);
    /*KernelRealReleaseLock(&EventLock,  __FILE__, __LINE__);*/

    while ( pSListEntry )
    {
        pTaskListEntry = ACCESS_TASK_LIST_ENTRY(pSListEntry);

        if ( (pTaskListEntry->TaskQid != atmos_qid_get_current()) 
             &&  bEventMsgInit && pFreeMsgHeader )
        {
            PKERNEL_FREE_MSG        pFreeMsg;

            critLevel = atmos_startcritical();
            /*KernelRealAcquireLock(&EventLock,  __FILE__, __LINE__);*/

            /*
             *  grab a pre-allocated message buffer
             */
            pFreeMsg = pFreeMsgHeader;

            if ( pFreeMsg != NULL )
            {
                MSG_D_KTX_AL_SET_EVENT(pSetEvent, &pFreeMsg->Msg);

                pFreeMsgHeader = pFreeMsg->Next;

                pSetEvent->EventHandle = (U32*)pEvent;

                atmos_endcritical(critLevel);
                /*KernelRealReleaseLock(&EventLock,  __FILE__, __LINE__);*/

                KernelTrace2
                    (
                        KERNEL_DBG_LEVEL_TRACE_FLOW,
                        KERNEL_DBG_MASK_EVENT,
                        "KernelPulseEvent -- event 0x%lX, send message to qid = 0x%lX, task %s...\n",
                        pEvent,
                        pTaskListEntry->TaskQid,
                        pTaskListEntry->TaskName
                    );

                sendmessage
                    (
                        &pFreeMsg->Msg, 
                        MSG_N_KTX_AL_SET_EVENT, 
                        pTaskListEntry->TaskQid
                    );
            }
            else
            {
                KernelTrace2
                    (
                        KERNEL_DBG_LEVEL_WARNING,
                        KERNEL_DBG_MASK_EVENT,
                        "KernelPulseEvent -- event 0x%lX, run out of free msg, %u.\n",
                        pEvent,
                        bEventMsgInit
                    );

                atmos_endcritical(critLevel);
                /*KernelRealReleaseLock(&EventLock,  __FILE__, __LINE__);*/
            }
        }
        else
        {
            if ( !pFreeMsgHeader || !bEventMsgInit )
            {
                KernelTrace2
                    (
                        KERNEL_DBG_LEVEL_WARNING,
                        KERNEL_DBG_MASK_EVENT,
                        "KernelPulseEvent -- event 0x%lX, run out of free msg, %u.\n",
                        pEvent,
                        bEventMsgInit
                    );
            }
        }

        KernelFreeMemory(pTaskListEntry);

        critLevel = atmos_startcritical();
        /*KernelRealAcquireLock(&EventLock,  __FILE__, __LINE__);*/

        pSListEntry = KernelSListPopEntry(&pEvent->TaskList);

        atmos_endcritical(critLevel);
        /*KernelRealReleaseLock(&EventLock,  __FILE__, __LINE__);*/
    }

    critLevel = atmos_startcritical();
    /*KernelRealAcquireLock(&EventLock,  __FILE__, __LINE__);*/
    
    pEvent->NumberSignaled = 0;
    
    atmos_endcritical(critLevel);
    /*KernelRealReleaseLock(&EventLock,  __FILE__, __LINE__);*/

    KernelTrace2
        (
            KERNEL_DBG_LEVEL_TRACE_FLOW,
            KERNEL_DBG_MASK_EVENT,
            "KernelPulseEvent -- event 0x%lX, done, task = %s.\n",
            pEvent,
            atmos_pcb_current_get_name()
        );

    return;
}


void
KernelWaitEvent
    (
        PKERNEL_EVENT               pBaseEvent,
        ULONG                       ulMilliSeconds
    )
{
    PKERNEL_EVENT_MSG               pEvent          = (PKERNEL_EVENT_MSG)pBaseEvent;
    PKERNEL_SLIST_ENTRY             pSavedSListEntry= (PKERNEL_SLIST_ENTRY)NULL;
    PKERNEL_TASK_LIST_ENTRY         pTaskListEntry;
    PKERNEL_FREE_MSG                pFreeMsg;
    BITS                            critLevel;
    BOOLEAN                         bInfinite       = FALSE;
    BOOLEAN                         bLoop           = TRUE;
    CHAR*                           cTaskName;
    ATMOS_MESSAGE                   TimerMsg;
    ATMOS_MESSAGE*                  pWaitingMsg;
    WORD                            wRtnCode;
    int                             iPhase          = 0;
    char                            NameBuffer[16]  = "Insuffient";
    char                            NameBuffer2[16] = "Insuffient";

    cTaskName = (char*)atmos_pcb_current_get_name();

    KernelTrace2
        (
            KERNEL_DBG_LEVEL_TRACE_FLOW,
            KERNEL_DBG_MASK_EVENT,
            "KernelWaitEvent -- event: 0x%lX, task = %s, time = %u.\n",
            pEvent,
            cTaskName,
            ulMilliSeconds
        );

    if ( ulMilliSeconds == 0 || ulMilliSeconds == 0xFFFFFFFF )
    {
        bInfinite = TRUE;
    }

    if ( pEvent == NULL)
    {
        KernelTrace2
            (
                KERNEL_DBG_LEVEL_WARNING,
                KERNEL_DBG_MASK_EVENT,
                "KernelWaitEvent -- NULL Event, Task: %s!!\n",
                cTaskName
            );

        return;
    }

    critLevel = atmos_startcritical();
    /*KernelRealAcquireLock(&EventLock,  __FILE__, __LINE__);*/
    
    if ( pEvent->NumberSignaled )
    {
        pEvent->NumberSignaled -= 1;
        bLoop = FALSE;
    }
    else
    {
        pTaskListEntry          = (PKERNEL_TASK_LIST_ENTRY)KernelAllocateMemory(sizeof(KERNEL_TASK_LIST_ENTRY));
        pTaskListEntry->TaskQid = atmos_qid_get_current();

    #if defined(KERNEL_CHECKED_BUILD)
        KernelCopyString(pTaskListEntry->TaskName, cTaskName);
    #endif

        KernelSListPushEntry(&pEvent->TaskList, &pTaskListEntry->Linkage);
        pSavedSListEntry = &pTaskListEntry->Linkage;
    }

    atmos_endcritical(critLevel);
    /*KernelRealReleaseLock(&EventLock,  __FILE__, __LINE__);*/

    if ( bLoop && !bInfinite )
    {
        timer_ms_timeout(&TimerMsg, ulMilliSeconds);
    }

    while ( bLoop )
    {
        KernelTrace2
            (
                KERNEL_DBG_LEVEL_TRACE_FLOW,
                KERNEL_DBG_MASK_EVENT,
                "KernelWaitEvent -- waiting ..., Event = 0x%lx, Task = %s.\n",
                pEvent,
                cTaskName
            );

        pWaitingMsg = awaitmessage();

        switch ( pWaitingMsg->code )
        {
            case    MSG_N_KTX_AL_SET_EVENT:
                {
                    MSG_D_KTX_AL_SET_EVENT(pSetEvent, pWaitingMsg);

                    if ( pSetEvent->EventHandle == (U32*)pEvent ) 
                    {
                        bLoop = FALSE;

                        if ( !bInfinite )
                        {
                            bInfinite = TRUE;

                            if ( pollspecificmessage(&TimerMsg) )
                            {
                                KernelTrace2
                                    (
                                        KERNEL_DBG_LEVEL_TRACE_FLOW,
                                        KERNEL_DBG_MASK_EVENT,
                                        "KernelWaitEvent -- event = 0x%lX, task = %s, timer message is on the way...\n",
                                        pEvent,
                                        cTaskName
                                    );

                                awaitspecificmessage(&TimerMsg);

                                KernelTrace2
                                    (
                                        KERNEL_DBG_LEVEL_TRACE_FLOW,
                                        KERNEL_DBG_MASK_EVENT,
                                        "KernelWaitEvent -- event = 0x%lX, task = %s, cleaned the timer message.\n",
                                        pEvent,
                                        cTaskName
                                    );
                            }
                            else
                            {
                                KernelTrace2
                                    (
                                        KERNEL_DBG_LEVEL_TRACE_FLOW,
                                        KERNEL_DBG_MASK_EVENT,
                                        "KernelWaitEvent -- event = 0x%lX, task = %s, cancel the timer message...\n",
                                        pEvent,
                                        cTaskName
                                    );

                                wRtnCode = timer_cancel(&TimerMsg);

                                KernelTrace2
                                    (
                                        KERNEL_DBG_LEVEL_TRACE_FLOW,
                                        KERNEL_DBG_MASK_EVENT,
                                        "KernelWaitEvent -- event = 0x%lX, task = %s, done with the timer message cancellation, error code = %u, %s.\n",
                                        pEvent,
                                        cTaskName,
                                        wRtnCode,
                                        pollmessage() ? "new message pending" : "no new messages"
                                    );

                                /* Found or not (wRtnCode == ENOTFOUND), the original message should have come back */
                                if ( pollspecificmessage(&TimerMsg) )
                                {
                                    awaitspecificmessage(&TimerMsg);

                                    KernelTrace2
                                        (
                                            KERNEL_DBG_LEVEL_TRACE_FLOW,
                                            KERNEL_DBG_MASK_EVENT,
                                            "KernelWaitEvent -- event = 0x%lX, task = %s, cleaned the timer message.\n",
                                            pEvent,
                                            cTaskName
                                        );
                                }
                                else
                                {
                                    KernelTrace2
                                        (
                                            KERNEL_DBG_LEVEL_WARNING,
                                            KERNEL_DBG_MASK_EVENT,
                                            "KernelWaitEvent -- event = 0x%lX, task = %s, Timer message didn't come back!!!\n",
                                            pEvent,
                                            cTaskName
                                        );
                                }
                            }
                        }
                    }
                    else
                    {
                        KernelTrace2
                            (
                                KERNEL_DBG_LEVEL_WARNING,
                                KERNEL_DBG_MASK_EVENT,
                                "KernelWaitEvent -- event = 0x%lX, qid = 0x%lX, task = %s, received bad SetEvent msg, event handle = 0x%lX, from task %s!!\n",
                                pEvent,
                                atmos_qid_get_current(),
                                cTaskName,
                                pSetEvent->EventHandle,
                                atmos_qid_get_name(pWaitingMsg->src, NameBuffer, sizeof(NameBuffer))
                            );
                    }

                    critLevel = atmos_startcritical();
                    /*KernelRealAcquireLock(&EventLock,  __FILE__, __LINE__);*/

                    pFreeMsg = ACCESS_FREE_MSG(pWaitingMsg);

                    pFreeMsg->Next = pFreeMsgHeader;
                    pFreeMsgHeader = pFreeMsg;

                    atmos_endcritical(critLevel);
                    /*KernelRealReleaseLock(&EventLock,  __FILE__, __LINE__);*/

                    iPhase = 1;

                    break;
                }

            case    MSG_R_TIMER_MSWAIT:
                {
                    BOOLEAN         bFound;

                    bLoop = FALSE;

                    KernelTrace2
                        (
                            KERNEL_DBG_LEVEL_TRACE_INFO,
                            KERNEL_DBG_MASK_EVENT,
                            "KernelWaitEvent -- event = 0x%lX, task = %s, timed-out...\n",
                            pEvent,
                            cTaskName
                        );

                    critLevel = atmos_startcritical();
                    /*KernelRealAcquireLock(&EventLock,  __FILE__, __LINE__);*/

                    bFound = 
                        KernelSListPopEntryByLink(&pEvent->TaskList, pSavedSListEntry);
                    if ( bFound )
                    {
                        pTaskListEntry = ACCESS_TASK_LIST_ENTRY(pSavedSListEntry);
                        if ( pTaskListEntry->TaskQid == atmos_qid_get_current() )
                        {
                            KernelFreeMemory(pTaskListEntry);
                        }
                        else
                        {
                            KernelSListPushEntry(&pEvent->TaskList, pSavedSListEntry);
                        }
                    }

                    atmos_endcritical(critLevel);
                    /*KernelRealReleaseLock(&EventLock,  __FILE__, __LINE__);*/

                    iPhase = 2;

                    break;
                }

            default:
                {
                    KernelTrace2
                        (
                            KERNEL_DBG_LEVEL_WARNING,
                            KERNEL_DBG_MASK_EVENT,
                            "KernelWaitEvent -- event = 0x%lX, qid = 0x%lX, task = %s, received unknown msg: 0x%X, src: 0x%X(%s), dst: 0x%X(%s)!\n",
                            pEvent,
                            atmos_qid_get_current(),
                            cTaskName,
                            pWaitingMsg->code,
                            pWaitingMsg->src,
                            atmos_qid_get_name(pWaitingMsg->src, NameBuffer, sizeof(NameBuffer)),
                            pWaitingMsg->dest,
                            atmos_qid_get_name(pWaitingMsg->dest, NameBuffer2, sizeof(NameBuffer2))
                        );

                    break;
                }
        }
    }

    return;
}


VOID
KernelFreeEvent
    (
        PKERNEL_EVENT               pBaseEvent
    )
{
    PKERNEL_EVENT_MSG               pEvent          = (PKERNEL_EVENT_MSG)pBaseEvent;
    BITS                            critLevel;
    PKERNEL_TASK_LIST_ENTRY         pTaskListEntry;
    PKERNEL_SLIST_ENTRY             pSListEntry;

    KernelTrace2
        (
            KERNEL_DBG_LEVEL_TRACE_FLOW,
            KERNEL_DBG_MASK_EVENT,
            "KernelFreeEvent -- event: 0x%lX, task = %s.\n",
            pEvent,
            atmos_pcb_current_get_name()
        );

    if ( pEvent == NULL)
    {
        KernelTrace2
            (
                KERNEL_DBG_LEVEL_WARNING,
                KERNEL_DBG_MASK_EVENT,
                "KernelFreeEvent -- NULL Event, Task: %s!!\n",
                atmos_pcb_current_get_name()
            );

        return;
    }

    critLevel = atmos_startcritical();
    /* KernelRealAcquireLock(&EventLock,  __FILE__, __LINE__); */

    if ( KernelSListGetFirstEntry(&pEvent->TaskList) != NULL )
    {
        KernelTrace2
            (
                KERNEL_DBG_LEVEL_TEST,
                KERNEL_DBG_MASK_EVENT,
                "KernelFreeEvent -- event: 0x%lX, task = %s, there are tasks waiting...\n",
                pEvent,
                atmos_pcb_current_get_name()
            );

        atmos_endcritical(critLevel);
        /* KernelRealReleaseLock(&EventLock,  __FILE__, __LINE__); */

        KernelPulseEvent(pBaseEvent);

        critLevel = atmos_startcritical();
        /* KernelRealAcquireLock(&EventLock,  __FILE__, __LINE__); */
    }

    pSListEntry = KernelSListPopEntry(&pEvent->TaskList);

    while ( pSListEntry )
    {
        pTaskListEntry = ACCESS_TASK_LIST_ENTRY(pSListEntry);
        pSListEntry    = KernelSListPopEntry(&pEvent->TaskList);

        KernelTrace2
            (
                KERNEL_DBG_LEVEL_TEST,
                KERNEL_DBG_MASK_EVENT,
                "KernelFreeEvent -- event: 0x%lX, task = %s, throw out the waiting task %s, qid = 0x%lX...\n",
                pEvent,
                atmos_pcb_current_get_name(),
                pTaskListEntry->TaskName,
                pTaskListEntry->TaskQid
            );

        KernelFreeMemory(pTaskListEntry);
    }

    pEvent->NumberSignaled = 0;
    KernelSListInitializeHeader(&pEvent->TaskList);

    atmos_endcritical(critLevel);
    /* KernelRealReleaseLock(&EventLock,  __FILE__, __LINE__); */

    KernelTrace2
        (
            KERNEL_DBG_LEVEL_TRACE_FLOW,
            KERNEL_DBG_MASK_EVENT,
            "KernelFreeEvent -- Done, event: 0x%lX, task = %s.\n",
            pEvent,
            atmos_pcb_current_get_name()
        );

    return;
}


#else   /* use wait based event */


BOOLEAN
KernelInitializeEvent
    (
        PKERNEL_EVENT               pBaseEvent
    )
{
    PKERNEL_EVENT_WAIT              pEvent          = (PKERNEL_EVENT_WAIT)pBaseEvent;

    pEvent->NumberSignaled  = 0;
    pEvent->NumberWaiting   = 0;

    return  TRUE;
}


VOID
KernelSetEvent
    (
        PKERNEL_EVENT               pBaseEvent
    )
{
    PKERNEL_EVENT_WAIT              pEvent          = (PKERNEL_EVENT_WAIT)pBaseEvent;
    BITS                            critLevel;

    critLevel = atmos_startcritical();

    KernelTrace2
        (
            KERNEL_DBG_LEVEL_TRACE_FLOW,
            KERNEL_DBG_MASK_GENERIC,
            "KernelSetEvent -- signaled = %lu, waiting = %lu, task = %s.\n",
            pEvent->NumberSignaled,
            pEvent->NumberWaiting,
            atmos_pcb_current_get_name()
        );

    if (pEvent->NumberWaiting == 0)
    {
        pEvent->NumberSignaled = 1;
    }
    else
    {
        pEvent->NumberSignaled ++;
    }

    atmos_endcritical(critLevel);

    KernelTrace2
        (
            KERNEL_DBG_LEVEL_TRACE_FLOW,
            KERNEL_DBG_MASK_GENERIC,
            "KernelSetEvent -- done, task = %s.\n",
            atmos_pcb_current_get_name()
        );

    /*
     *  let other waiters digest this state change
     */
    atmos_round_robin();

    return;
}


VOID
KernelResetEvent
    (
        PKERNEL_EVENT               pBaseEvent
    )
{
    PKERNEL_EVENT_WAIT              pEvent          = (PKERNEL_EVENT_WAIT)pBaseEvent;
    BITS                            critLevel;

    critLevel = atmos_startcritical();

    KernelTrace2
        (
            KERNEL_DBG_LEVEL_VERBOSE_FLOW,
            KERNEL_DBG_MASK_GENERIC,
            "KernelResetEvent -- signaled = %lu, waiting = %lu, task = %s.\n",
            pEvent->NumberSignaled,
            pEvent->NumberWaiting,
            atmos_pcb_current_get_name()
        );

    pEvent->NumberSignaled = 0;

    atmos_endcritical(critLevel);

    KernelTrace2
        (
            KERNEL_DBG_LEVEL_VERBOSE_FLOW,
            KERNEL_DBG_MASK_GENERIC,
            "KernelResetEvent -- done, task = %s.\n",
            atmos_pcb_current_get_name()
        );

    return;
}


VOID
KernelPulseEvent
    (
        PKERNEL_EVENT               pBaseEvent
    )
{
    PKERNEL_EVENT_WAIT              pEvent          = (PKERNEL_EVENT_WAIT)pBaseEvent;
    BITS                            critLevel;

    critLevel = atmos_startcritical();

    KernelTrace2
        (
            KERNEL_DBG_LEVEL_TRACE_FLOW,
            KERNEL_DBG_MASK_GENERIC,
            "KernelPulseEvent -- signaled = %lu, waiting = %lu, task = %s.\n",
            pEvent->NumberSignaled,
            pEvent->NumberWaiting,
            atmos_pcb_current_get_name()
        );

    pEvent->NumberSignaled = pEvent->NumberWaiting;

    atmos_endcritical(critLevel);

    KernelTrace2
        (
            KERNEL_DBG_LEVEL_TRACE_FLOW,
            KERNEL_DBG_MASK_GENERIC,
            "KernelPulseEvent -- done, task = %s.\n",
            atmos_pcb_current_get_name()
        );

    /*
     *  let other waiters digest this state change
     */
    atmos_round_robin();

    return;
}


void
KernelWaitEvent
    (
        PKERNEL_EVENT               pBaseEvent,
        ULONG                       ulMilliSeconds
    )
{
    PKERNEL_EVENT_WAIT              pEvent          = (PKERNEL_EVENT_WAIT)pBaseEvent;
    BITS                            critLevel;
    ULONG                           startTimeStamp  = 0;
    BOOLEAN                         bInfinite       = FALSE;

    if ( ulMilliSeconds == 0 || ulMilliSeconds == 0xFFFFFFFF )
    {
        bInfinite = TRUE;
    }

    startTimeStamp = KernelGetTickInMilliSeconds();

    critLevel = atmos_startcritical();

    if (pEvent->NumberSignaled > 0)
    {
        KernelTrace2
            (
                KERNEL_DBG_LEVEL_TRACE_FLOW,
                KERNEL_DBG_MASK_GENERIC,
                "KernelWaitEvent -- signaled = %lu, waiting = %lu, task = %s, done.\n",
                pEvent->NumberSignaled,
                pEvent->NumberWaiting,
                atmos_pcb_current_get_name()
            );

        pEvent->NumberSignaled --;

        atmos_endcritical(critLevel);

        /*return  KERNEL_STATUS_SUCCESS;*/
        return;
    }

    pEvent->NumberWaiting ++;

    atmos_endcritical(critLevel);

    /*
     *  let others take the time slice
     */
    /*atmos_round_robin();*/
    as_TimerWait(1);

    /*
     *  wait until time-out
     */
    while ( bInfinite || (KernelGetTickInMilliSeconds() - startTimeStamp) < ulMilliSeconds )
    {
        ULONG                       ulPrevTs    = 0;

        critLevel = atmos_startcritical();

        if (pEvent->NumberSignaled > 0)
        {
            pEvent->NumberSignaled --;
            pEvent->NumberWaiting  --;

            atmos_endcritical(critLevel);

            KernelTrace2
                (
                    KERNEL_DBG_LEVEL_TRACE_FLOW,
                    KERNEL_DBG_MASK_GENERIC,
                    "KernelWaitEvent -- signaled, task = %s.\n",
                    atmos_pcb_current_get_name()
                );

            /*return  KERNEL_STATUS_SUCCESS;*/
            return;
        }

        atmos_endcritical(critLevel);

        if ((KernelGetTickInMilliSeconds() - ulPrevTs) >= 500)
        {
            KernelTrace2
                (
                    KERNEL_DBG_LEVEL_VERBOSE_FLOW,
                    KERNEL_DBG_MASK_GENERIC,
                    "KernelWaitEvent -- waiting ..., signaled = %lu, waiting = %lu, task = %s.\n",
                    pEvent->NumberSignaled,
                    pEvent->NumberWaiting,
                    atmos_pcb_current_get_name()
                );

            ulPrevTs = KernelGetTickInMilliSeconds();
        }

        /*
         *  let others take the time slice
         */
        /*atmos_round_robin();*/
        as_TimerWait(1);
    }

    KernelTrace2
        (
            KERNEL_DBG_LEVEL_VERBOSE_FLOW,
            KERNEL_DBG_MASK_GENERIC,
            "KernelWaitEvent -- timed-out, task = %s.\n",
            atmos_pcb_current_get_name()
        );

    critLevel = atmos_startcritical();

    pEvent->NumberWaiting  --;

    atmos_endcritical(critLevel);

    /*return  KERNEL_STATUS_TIMEOUT;*/
    return;
}


VOID
KernelFreeEvent
    (
        PKERNEL_EVENT               pBaseEvent
    )
{
    PKERNEL_EVENT_WAIT              pEvent          = (PKERNEL_EVENT_WAIT)pBaseEvent;
    BITS                            critLevel;

    critLevel = atmos_startcritical();

    pEvent->NumberSignaled  = 0;
    pEvent->NumberWaiting   = 0;

    atmos_endcritical(critLevel);

    return;
}

#endif
