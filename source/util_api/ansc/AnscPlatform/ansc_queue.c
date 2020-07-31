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

    module:	ansc_queue.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the some platform-dependent and
        general utility functions related to queue manipulation.

        *   AnscQueuePopEntry
        *   AnscQueuePopEntryByLink
        *   AnscQueueMoveEntryToStart
        *   AnscQueueMoveEntryToEnd
        *   AnscQueueSearchEntryByIndex
        *   AnscQueuePopEntryByIndex
        *   AnscQueuePushEntryFixNumber
        *   AnscQueuePushEntryBefore
        *   AnscQueuePushEntryAfter

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        04/01/01    initial revision.

**********************************************************************/


#include "ansc_global.h"

/*
PSINGLE_LINK_ENTRY
AnscQueuePopEntry
    (
        PQUEUE_HEADER               QueueHead
    )
{
    PSINGLE_LINK_ENTRY              FirstEntry = NULL;

    FirstEntry = (QueueHead)->Next.Next;

    if ( FirstEntry != NULL )
    {
        (QueueHead)->Next.Next = FirstEntry->Next;
        (QueueHead)->Depth--;

        if ( FirstEntry->Next == NULL)
        {
            (QueueHead)->Last.Next = NULL;
        }
    }

    return  FirstEntry;
}
*/

BOOLEAN
AnscQueuePopEntryByLink
    (
        PQUEUE_HEADER               QueueHead,
        PSINGLE_LINK_ENTRY          Entry
    )
{
    PSINGLE_LINK_ENTRY              NextEntry = NULL;
    BOOLEAN                         bFound    = FALSE;

    NextEntry = &QueueHead->Next;

    while ( NextEntry != NULL )
    {
        if ( NextEntry->Next == Entry )
        {
            NextEntry->Next = Entry->Next;
            (QueueHead)->Depth--;

            bFound = TRUE;
            break;
        }

        NextEntry = NextEntry->Next;
    }

    if ( (QueueHead)->Last.Next == Entry )
    {
        if ( (QueueHead)->Next.Next == NULL )
        {
            (QueueHead)->Last.Next = NULL;
        }
        else
        {
            (QueueHead)->Last.Next = NextEntry;
        }
    }

    return  bFound;
}


void
AnscQueueMoveEntryToStart
    (
        PQUEUE_HEADER               QueueHead,
        PSINGLE_LINK_ENTRY          Entry
    )
{
    AnscQueuePopEntryByLink  (QueueHead, Entry);
    AnscQueuePushEntryAtFront(QueueHead, Entry);
}


void
AnscQueueMoveEntryToEnd
    (
        PQUEUE_HEADER               QueueHead,
        PSINGLE_LINK_ENTRY          Entry
    )
{
    AnscQueuePopEntryByLink(QueueHead, Entry);
    AnscQueuePushEntry     (QueueHead, Entry);
}


PSINGLE_LINK_ENTRY
AnscQueueSearchEntryByIndex
    (
        PQUEUE_HEADER               QueueHead,
        ULONG                       Index
    )
{
    PSINGLE_LINK_ENTRY              NextEntry = NULL;
    ULONG                           i         = 0;

    NextEntry = &QueueHead->Next;

    for ( i = 0; i <= Index; i++ )
    {
        NextEntry = NextEntry->Next;

        if ( !NextEntry )
        {
            return  NULL;
        }
    }

    return  NextEntry;
}


PSINGLE_LINK_ENTRY
AnscQueuePopEntryByIndex
    (
        PQUEUE_HEADER               QueueHead,
        ULONG                       Index
    )
{
    PSINGLE_LINK_ENTRY              NextEntry = NULL;

    NextEntry = AnscQueueSearchEntryByIndex(QueueHead, Index);

    if ( NextEntry )
    {
        AnscQueuePopEntryByLink(QueueHead, NextEntry);
    }

    return  NextEntry;
}


PSINGLE_LINK_ENTRY
AnscQueuePushEntryFixNumber
    (
        PQUEUE_HEADER               QueueHead,
        PSINGLE_LINK_ENTRY          Entry,
        ULONG                       Max
    )
{
    PSINGLE_LINK_ENTRY              FirstEntry = NULL;

    AnscQueuePushEntry(QueueHead, Entry);

    if ( AnscQueueQueryDepth(QueueHead) > Max )
    {
        FirstEntry = AnscQueuePopEntry(QueueHead);

        return  FirstEntry;
    }

    return  NULL;
}


BOOLEAN
AnscQueuePushEntryBefore
    (
        PQUEUE_HEADER               QueueHead,
        PSINGLE_LINK_ENTRY          Entry,
        PSINGLE_LINK_ENTRY          beforeEntry
    )
{
    PSINGLE_LINK_ENTRY              LastEntry = NULL;
    BOOLEAN                         bFound    = FALSE;

    LastEntry = &QueueHead->Next;

    if ( LastEntry->Next == NULL )
    {
        (QueueHead)->Next.Next = Entry;
        (QueueHead)->Last.Next = Entry;
        Entry->Next            = NULL;

        QueueHead->Depth++;
    }
    else
    {
        while ( LastEntry != NULL )
        {
            if ( LastEntry->Next == beforeEntry )
            {
                LastEntry->Next = Entry;
                Entry->Next     = beforeEntry;

                (QueueHead)->Depth++;

                bFound = TRUE;
                break;
            }

            LastEntry = LastEntry->Next;
        }
    }

    if ( bFound && !beforeEntry )
    {
        (QueueHead)->Last.Next = Entry;
    }

    return  bFound;
}


VOID
AnscQueuePushEntryAfter
    (
        PQUEUE_HEADER               QueueHead,
        PSINGLE_LINK_ENTRY          Entry,
        PSINGLE_LINK_ENTRY          afterEntry
    )
{

    if ( !afterEntry )
    {
        AnscQueuePushEntryAtFront(QueueHead, Entry);

        return;
    }

    Entry->Next      = afterEntry->Next;
    afterEntry->Next = Entry;

    if ( (QueueHead)->Last.Next == afterEntry )
    {
        (QueueHead)->Last.Next = Entry;
    }

    QueueHead->Depth++;

    return;
}
