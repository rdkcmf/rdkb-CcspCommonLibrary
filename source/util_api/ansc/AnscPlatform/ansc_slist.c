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

    module:	ansc_slist.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the some platform-dependent and
        general utility functions related to slist manipulation.

        *   AnscSListPopEntry
        *   AnscSListPopEntryByLink
        *   AnscSListReplaceEntry
        *   AnscSListGetLastEntry
        *   AnscSListPushEntryFixNumber
        *   AnscSListSearchEntryByIndex
        *   AnscSListPushEntryByIndex

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
AnscSListPopEntry
    (
        PSLIST_HEADER               ListHead
    )
{
    PSINGLE_LINK_ENTRY              FirstEntry = NULL;

    FirstEntry = (ListHead)->Next.Next;

    if ( FirstEntry != NULL )
    {
        (ListHead)->Next.Next = FirstEntry->Next;
        (ListHead)->Depth--;
    }

    return  FirstEntry;
}
*/

BOOLEAN
AnscSListPopEntryByLink
    (
        PSLIST_HEADER               ListHead,
        PSINGLE_LINK_ENTRY          Entry
    )
{
    PSINGLE_LINK_ENTRY              NextEntry = NULL;
    BOOLEAN                         bFound    = FALSE;

    NextEntry = &ListHead->Next;

    while ( NextEntry != NULL )
    {
        if ( NextEntry->Next == Entry )
        {
            NextEntry->Next = Entry->Next;
            (ListHead)->Depth--;

            bFound = TRUE;
            break;
        }

        NextEntry = NextEntry->Next;
    }

    return  bFound;
}


VOID
AnscSListReplaceEntry
    (
        PSLIST_HEADER               ListHead,
        PSINGLE_LINK_ENTRY          NewEntry,
        PSINGLE_LINK_ENTRY          OldEntry
    )
{
    PSINGLE_LINK_ENTRY              NextEntry = NULL;

    NextEntry = &ListHead->Next;

    while ( NextEntry != NULL )
    {
        if ( NextEntry->Next == OldEntry )
        {
            NextEntry->Next = NewEntry;
            NewEntry->Next  = OldEntry->Next;

            break;
        }

        NextEntry = NextEntry->Next;
    }
}


PSINGLE_LINK_ENTRY
AnscSListGetLastEntry
    (
        PSLIST_HEADER               ListHead
    )
{
    PSINGLE_LINK_ENTRY              NextEntry = NULL;
    PSINGLE_LINK_ENTRY              LastEntry = NULL;

    NextEntry = ListHead->Next.Next;
    LastEntry = NextEntry;

    while ( NextEntry )
    {
        LastEntry = NextEntry;
        NextEntry = LastEntry->Next;
    }

    return  LastEntry;
}


PSINGLE_LINK_ENTRY
AnscSListPushEntryFixNumber
    (
        PSLIST_HEADER               ListHead,
        PSINGLE_LINK_ENTRY          Entry,
        ULONG                       Max
    )
{
    PSINGLE_LINK_ENTRY              LastEntry = NULL;

    AnscSListPushEntry(ListHead, Entry);

    if ( AnscSListQueryDepth(ListHead) > Max )
    {
        LastEntry = AnscSListGetLastEntry(ListHead);

        AnscSListPopEntryByLink(ListHead, LastEntry);

        return  LastEntry;
    }

    return  NULL;
}


PSINGLE_LINK_ENTRY
AnscSListSearchEntryByIndex
    (
        PSLIST_HEADER               ListHead,
        ULONG                       Index
    )
{
    PSINGLE_LINK_ENTRY              NextEntry = NULL;
    ULONG                           i         = 0;

    NextEntry = &ListHead->Next;

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
AnscSListPushEntryByIndex
    (
        PSLIST_HEADER               ListHead,
        PSINGLE_LINK_ENTRY          Entry,
        ULONG                       Index
    )
{
    PSINGLE_LINK_ENTRY              NextEntry = NULL;
    ULONG                           i         = 0;

    if ( Index >= (ULONG)ListHead->Depth )
    {
        AnscSListPushEntryAtBack(ListHead, Entry);

        return  NULL;
    }

    NextEntry = &ListHead->Next;

    for ( i = 0; i < Index; i++ )
    {
        NextEntry = NextEntry->Next;
    }

    Entry->Next     = NextEntry->Next;
    NextEntry->Next = Entry;
    ListHead->Depth++;

    return  NextEntry;
}
