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

    module:	ansc_dlist.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the some platform-dependent and
        general utility functions related to dlist manipulation.

        *   AnscDListGetEntryByIndex
        *   AnscDListPushEntryLeft
        *   AnscDListPushEntryRight
        *   AnscDListPushEntryAtIndex
        *   AnscDListPushEntryLeftFixNumber
        *   AnscDListPopEntryLeft
        *   AnscDListPopEntryRight
        *   AnscDListPopEntryByIndex
        *   AnscDListPopEntryByLink

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/23/01    initial revision.

**********************************************************************/


#include "ansc_global.h"


PDOUBLE_LINK_ENTRY
AnscDListGetEntryByIndex
    (
        PDLIST_HEADER               pDListHeader,
        ULONG                       ulIndex
    )
{
    PDOUBLE_LINK_ENTRY              pEntry = pDListHeader->Heads.Next;
    ULONG                           count  = 0;

    while ( pEntry && (count < ulIndex) )
    {
        pEntry = pEntry->Next;
        count++;
    }

    return  pEntry;
}


void
AnscDListPushEntryLeft
    (
        PDLIST_HEADER               pDListHeader,
        PDOUBLE_LINK_ENTRY          pEntry
    )
{
    PDOUBLE_LINK_ENTRY              pHeadEntry = pDListHeader->Heads.Next;
    PDOUBLE_LINK_ENTRY              pTailEntry = pDListHeader->Heads.Last;

    if ( pHeadEntry )
    {
        pHeadEntry->Last = pEntry;
    }

    pEntry->Next             = pHeadEntry;
    pEntry->Last             = NULL;
    pDListHeader->Heads.Next = pEntry;

    if ( !pTailEntry )
    {
        pDListHeader->Heads.Last = pEntry;
    }

    pDListHeader->Depth++;
}


void
AnscDListPushEntryRight
    (
        PDLIST_HEADER               pDListHeader,
        PDOUBLE_LINK_ENTRY          pEntry
    )
{
    PDOUBLE_LINK_ENTRY              pHeadEntry = pDListHeader->Heads.Next;
    PDOUBLE_LINK_ENTRY              pTailEntry = pDListHeader->Heads.Last;

    if ( pTailEntry )
    {
        pTailEntry->Next = pEntry;
    }

    pEntry->Next             = NULL;
    pEntry->Last             = pTailEntry;
    pDListHeader->Heads.Last = pEntry;

    if ( !pHeadEntry )
    {
        pDListHeader->Heads.Next = pEntry;
    }

    pDListHeader->Depth++;
}


void
AnscDListPushEntryAtIndex
    (
        PDLIST_HEADER               pDListHeader,
        PDOUBLE_LINK_ENTRY          pEntry,
        ULONG                       ulIndex
    )
{
    PDOUBLE_LINK_ENTRY              pLastEntry = NULL;
    PDOUBLE_LINK_ENTRY              pHeadEntry = pDListHeader->Heads.Next;
    PDOUBLE_LINK_ENTRY              pTailEntry = pDListHeader->Heads.Last;
    PDOUBLE_LINK_ENTRY              pNextEntry = NULL;

    if ( ulIndex > pDListHeader->Depth )
    {
        return;
    }
    else if ( ulIndex == 0 )
    {
        pLastEntry = NULL;
        pNextEntry = pHeadEntry->Next;
    }
    else if ( ulIndex == pDListHeader->Depth )
    {
        pLastEntry = pTailEntry;
        pNextEntry = NULL;
    }
    else
    {
        pLastEntry = AnscDListGetEntryByIndex(pDListHeader, ulIndex - 1);
        pNextEntry = pLastEntry->Next;
    }

    pEntry->Last = pLastEntry;
    pEntry->Next = pNextEntry;

    if ( pLastEntry )
    {
        pLastEntry->Next = pEntry;
    }
    else
    {
        pDListHeader->Heads.Next = pEntry;
    }

    if ( pNextEntry )
    {
        pNextEntry->Last = pEntry;
    }
    else
    {
        pDListHeader->Heads.Last = pEntry;
    }

    pDListHeader->Depth++;
}


PDOUBLE_LINK_ENTRY
AnscDListPushEntryLeftFixNumber
    (
        PDLIST_HEADER               pDListHeader,
        PDOUBLE_LINK_ENTRY          pEntry,
        ULONG                       Max
    )
{
    PDOUBLE_LINK_ENTRY              LastEntry = NULL;

    AnscDListPushEntryLeft(pDListHeader, pEntry);

    if ( AnscDListQueryDepth(pDListHeader) > Max )
    {
        LastEntry = AnscDListPopEntryRight(pDListHeader);

        return  LastEntry;
    }

    return  NULL;
}


PDOUBLE_LINK_ENTRY
AnscDListPopEntryLeft
    (
        PDLIST_HEADER               pDListHeader
    )
{
    PDOUBLE_LINK_ENTRY              pHeadEntry = pDListHeader->Heads.Next;
    PDOUBLE_LINK_ENTRY              pNextEntry = NULL;

    if ( !pHeadEntry )
    {
        return  NULL;
    }
    else
    {
        pNextEntry = pHeadEntry->Next;
    }

    pDListHeader->Heads.Next = pNextEntry;
    pDListHeader->Depth--;

    if ( pNextEntry )
    {
        pNextEntry->Last = NULL;
    }
    else
    {
        pDListHeader->Heads.Last = NULL;
    }

    return  pHeadEntry;
}


PDOUBLE_LINK_ENTRY
AnscDListPopEntryRight
    (
        PDLIST_HEADER               pDListHeader
    )
{
    PDOUBLE_LINK_ENTRY              pTailEntry = pDListHeader->Heads.Last;
    PDOUBLE_LINK_ENTRY              pLastEntry = NULL;

    if ( !pTailEntry )
    {
        return  NULL;
    }
    else
    {
        pLastEntry = pTailEntry->Last;
    }

    pDListHeader->Heads.Last = pLastEntry;
    pDListHeader->Depth--;

    if ( pLastEntry )
    {
        pLastEntry->Next = NULL;
    }
    else
    {
        pDListHeader->Heads.Next = NULL;
    }

    return  pTailEntry;
}


PDOUBLE_LINK_ENTRY
AnscDListPopEntryByIndex
    (
        PDLIST_HEADER               pDListHeader,
        ULONG                       ulIndex
    )
{
    PDOUBLE_LINK_ENTRY              pEntry = AnscDListGetEntryByIndex(pDListHeader, ulIndex);

    if ( !pEntry )
    {
        return  NULL;
    }
    else
    {
        AnscDListPopEntryByLink(pDListHeader, pEntry);
    }

    return  pEntry;
}


BOOLEAN
AnscDListPopEntryByLink
    (
        PDLIST_HEADER               pDListHeader,
        PDOUBLE_LINK_ENTRY          pEntry
    )
{
    PDOUBLE_LINK_ENTRY              pHeadEntry = pDListHeader->Heads.Next;
    PDOUBLE_LINK_ENTRY              pTailEntry = pDListHeader->Heads.Last;
    PDOUBLE_LINK_ENTRY              pPrevEntry = pEntry->Last;
    PDOUBLE_LINK_ENTRY              pNextEntry = pEntry->Next;

    if ( pEntry == pHeadEntry )
    {
        pDListHeader->Heads.Next = pEntry->Next;
    }

    if ( pEntry == pTailEntry )
    {
        pDListHeader->Heads.Last = pEntry->Last;
    }

    if ( pPrevEntry )
    {
        pPrevEntry->Next = pNextEntry;
    }

    if ( pNextEntry )
    {
        pNextEntry->Last = pPrevEntry;
    }

    pDListHeader->Depth--;

    return  TRUE;
}
