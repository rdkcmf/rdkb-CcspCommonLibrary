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

    module:	ansc_obj_array_co_process.c

        For Object Array object,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Object Array Component Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        03/14/02    initial revision.

    ---------------------------------------------------------------

    last modified:  

**********************************************************************/


#include "ansc_uint_array_co_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscUintArrayGetSize
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to get number of items in array.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

    return:     the number of items.

**********************************************************************/

ULONG
AnscUIntArrayGetSize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_UINT_ARRAY_OBJECT         pUIntArray  = (PANSC_UINT_ARRAY_OBJECT)hThisObject;

    if (pUIntArray)
        return pUIntArray->ulItemCount;

    return 0;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscUintArraySetSize
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulNewSize,
                ULONG                       ulGrowBy
            )

    description:

        This function is called to set array size and growth size.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                ULONG                       ulNewSize
                The new size of array.

                ULONG                       ulGrowBy
                Growth size.

    return:     void.

**********************************************************************/

void
AnscUIntArraySetSize
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulNewSize,
        ULONG                       ulGrowBy
    )
{
    PANSC_UINT_ARRAY_OBJECT         pUIntArray  = (PANSC_UINT_ARRAY_OBJECT)hThisObject;

    /* enlarge the storage if neccessary */

    if (!pUIntArray)
        return;

    /* change the growth if it's not "-1" (0xFFFFFFFF) */
    if (ulGrowBy != (ULONG)-1)
    {
        pUIntArray->SetGrowth(hThisObject, ulGrowBy);
    }

    if (pUIntArray->ulMaxItemCount != ulNewSize)
    {
        PUINT                       pBuf;
        ULONG                       ulBufSize;

        ulBufSize   = sizeof(UINT) * ulNewSize;

        /* resize the storage to the given size */
        if (!pUIntArray->hStorage)
        {
            pBuf    = (PUINT)AnscAllocateMemory(ulBufSize);
        }
        else
        {
/*
            pBuf    = (PUINT)AnscReAllocMemory(pUIntArray->hStorage, ulBufSize);
*/

            pBuf = (PUINT)AnscMemUtilRealloc(pUIntArray->hStorage, pUIntArray->ulMaxItemCount * sizeof(UINT), ulBufSize);
        }

        pUIntArray->hStorage        = (ANSC_HANDLE)pBuf;
        pUIntArray->ulMaxItemCount  = ulNewSize;
        pUIntArray->ulItemCount     = ulNewSize;
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscUIntArrayRemoveAll
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to remove all items in array.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

    return:     void.

**********************************************************************/

void
AnscUIntArrayRemoveAll
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_UINT_ARRAY_OBJECT         pUIntArray  = (PANSC_UINT_ARRAY_OBJECT)hThisObject;

    if (!pUIntArray)
        return;

    /* we don't release the storage */
    pUIntArray->ulItemCount = 0;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_OBJECT_ARRAY_DATA
        AnscUIntArrayGetAt
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulIndex
            )

    description:

        This function is called to get array item.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                ULONG                       ulIndex
                The position of array item.

    return:     array item data.

**********************************************************************/

ANSC_OBJECT_ARRAY_DATA
AnscUIntArrayGetAt
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex
    )
{
    PANSC_UINT_ARRAY_OBJECT         pUIntArray  = (PANSC_UINT_ARRAY_OBJECT)hThisObject;

    if (pUIntArray && pUIntArray->ulItemCount > 0 && ulIndex < pUIntArray->ulItemCount)
    {
        PUINT                       pStorage;

        pStorage    = (PUINT)pUIntArray->hStorage;

#ifdef _64BIT_ARCH_SUPPORT_
        return (ANSC_OBJECT_ARRAY_DATA)(ULONG)pStorage[ulIndex];
#else
        return (ANSC_OBJECT_ARRAY_DATA)pStorage[ulIndex];
#endif
    }

    return (ANSC_OBJECT_ARRAY_DATA)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscUIntArraySetAt
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulIndex,
                ANSC_OBJECT_ARRAY_DATA      Data
            )

    description:

        This function is called to set array item at specified 
        position.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                ULONG                       ulIndex
                The position to set array item.

                ANSC_OBJECT_ARRAY_DATA      Data
                The array item to be set.

    return:     void.

**********************************************************************/

void
AnscUIntArraySetAt
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex,
        ANSC_OBJECT_ARRAY_DATA      Data
    )
{
    PANSC_UINT_ARRAY_OBJECT         pUIntArray  = (PANSC_UINT_ARRAY_OBJECT)hThisObject;

    if (!pUIntArray || pUIntArray->ulMaxItemCount <= ulIndex)
        return;
    else
    {
        PUINT                       pStorage;

        pStorage            = (PUINT)pUIntArray->hStorage;
#ifdef _64BIT_ARCH_SUPPORT_
        pStorage[ulIndex]   = (ULONG)Data;
#else
        pStorage[ulIndex]   = (UINT)Data;
#endif
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscUIntArrayInsertAt
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulIndex,
                ANSC_OBJECT_ARRAY_DATA      Data,
                ULONG                       ulCount
            )

    description:

        This function is called to insert an array item.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                ULONG                       ulIndex
                The position to insert array items.

                ANSC_OBJECT_ARRAY_DATA      Data
                Array item data.

                ULONG                       ulCount
                The number of items to be inserted.

    return:     void.

**********************************************************************/

void
AnscUIntArrayInsertAt
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex,
        ANSC_OBJECT_ARRAY_DATA      Data,
        ULONG                       ulCount
    )
{
    PANSC_UINT_ARRAY_OBJECT         pUIntArray  = (PANSC_UINT_ARRAY_OBJECT)hThisObject;

    if (!pUIntArray || ulIndex >= pUIntArray->ulItemCount)
        return;
    else
    {
        ULONG                       i;
        PUINT                       pStorage;
  
        if (ulCount + pUIntArray->ulItemCount > pUIntArray->ulMaxItemCount)
        {
            /* resize the storage */
            pUIntArray->EnlargeStorage(hThisObject, pUIntArray->ulItemCount + ulCount);
        }

        if (ulCount + pUIntArray->ulItemCount > pUIntArray->ulMaxItemCount)
        {
            /* failed to enlarge the storage */
            return;
        }

        /* move items after the given index (inclusive) */
        pStorage    = (PUINT)pUIntArray->hStorage;
        for (i = pUIntArray->ulItemCount - 1; i >= ulIndex; i --)
        {
            pStorage[i + ulCount] = pStorage[i];
        }

        pUIntArray->ulItemCount += ulCount;

        for (i = 0; i < ulCount; i ++)
        {
#ifdef _64BIT_ARCH_SUPPORT_
            pStorage[i + ulIndex]   = (ULONG)Data;
#else
            pStorage[i + ulIndex]   = (UINT)Data;
#endif
        }
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscUIntArrayRemoveAt
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulIndex,
                ULONG                       ulCount
            )

    description:

        This function is called to remove array items.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                ULONG                       ulIndex
                The starting position.

                ULONG                       ulCount
                The number of items to be removed.

    return:     void.

**********************************************************************/

void
AnscUIntArrayRemoveAt
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex,
        ULONG                       ulCount
    )
{
    UNREFERENCED_PARAMETER(ulCount);
    PANSC_UINT_ARRAY_OBJECT         pUIntArray  = (PANSC_UINT_ARRAY_OBJECT)hThisObject;

    if (!pUIntArray || ulIndex >= pUIntArray->ulItemCount)
        return;
    else
    {
        PUINT                       pStorage;
        ULONG                       i;

        pStorage    = (PUINT)pUIntArray->hStorage;

        for (i = ulIndex; i < pUIntArray->ulItemCount; i ++)
        {
            pStorage[i] = pStorage[i + 1];
        }

        pUIntArray->ulItemCount --;
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscUIntArrayAdd
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_OBJECT_ARRAY_DATA      Data
            )

    description:

        This function is called to add the specified data into array.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                ANSC_OBJECT_ARRAY_DATA      Data
                New item data.

    return:     the position of the new item.

**********************************************************************/

ULONG
AnscUIntArrayAdd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_OBJECT_ARRAY_DATA      Data
    )
{
    PANSC_UINT_ARRAY_OBJECT         pUIntArray  = (PANSC_UINT_ARRAY_OBJECT)hThisObject;

    if (pUIntArray)
    {
        if (pUIntArray->ulItemCount + 1 > pUIntArray->ulMaxItemCount)
        {
            pUIntArray->EnlargeStorage(hThisObject, pUIntArray->ulItemCount + 1);
        }

        if (pUIntArray->ulItemCount + 1 <= pUIntArray->ulMaxItemCount)
        {
            PUINT                   pStorage;

            pStorage = (PUINT)pUIntArray->hStorage;
#ifdef _64BIT_ARCH_SUPPORT_
            pStorage[pUIntArray->ulItemCount ++] = (ULONG)Data;
#else
            pStorage[pUIntArray->ulItemCount ++] = (UINT)Data;
#endif

            return pUIntArray->ulItemCount;
        }
    }

    return 0xFFFFFFFF;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscUIntArrayEnlargeStorage
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulNewSize
            )

    description:

        This function is called to get items in array.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                ULONG                       ulNewSize
                The new size of the array.

    return:     void.

**********************************************************************/

void
AnscUIntArrayEnlargeStorage
    (   
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulNewSize
    )
{
    PANSC_UINT_ARRAY_OBJECT         pUIntArray  = (PANSC_UINT_ARRAY_OBJECT)hThisObject;
    ULONG                           ulGrowth    = 1;
    ULONG                           ulNewMaxSize;

    /* enlarge the storage if neccessary */

    if (!pUIntArray)
        return;

    ulGrowth    = pUIntArray->ulGrowth;

    if (pUIntArray->ulMaxItemCount < ulNewSize)
    {
        PUINT                       pBuf;
        ULONG                       ulBufSize;

        ulNewMaxSize    = ulNewSize;
        if (ulNewMaxSize % ulGrowth)
            ulNewMaxSize    = (ulNewMaxSize/ulGrowth + 1) * ulGrowth;
        else
        {
            ulNewMaxSize    = ulNewMaxSize;

            if (ulNewMaxSize == ulNewSize)
            {
                ulNewMaxSize    += ulGrowth;
            }
        }

        ulBufSize   = sizeof(UINT) * ulNewMaxSize;

        /* resize the storage to the given size */
        if (!pUIntArray->hStorage)
        {
            pBuf    = (PUINT)AnscAllocateMemory(ulBufSize);
        }
        else
        {
/*
            pBuf    = (PUINT)AnscReAllocMemory(pUIntArray->hStorage, ulBufSize);
*/

            pBuf    = (PUINT)AnscMemUtilRealloc(pUIntArray->hStorage, pUIntArray->ulMaxItemCount * sizeof(UINT), ulBufSize);
        }

        if (pBuf)
        {
            pUIntArray->hStorage        = (ANSC_HANDLE)pBuf;
            pUIntArray->ulMaxItemCount  = ulNewMaxSize;
        }
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG
        AnscUIntArrayFind
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_OBJECT_ARRAY_DATA      Data
            )

    description:

        This function is called to find given data in array.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

    return:     the position found, -1 if no match.

**********************************************************************/

LONG
AnscUIntArrayFind
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_OBJECT_ARRAY_DATA      Data
    )
{
    PANSC_UINT_ARRAY_OBJECT         pUIntArray  = (PANSC_UINT_ARRAY_OBJECT)hThisObject;
    LONG                            Count       = pUIntArray->ulItemCount;
    LONG                            i;
    PUINT                           pStorage    = (PUINT)pUIntArray->hStorage;
#ifdef _64BIT_ARCH_SUPPORT_
    UINT                            uSample     = (ULONG)Data;
#else
    UINT                            uSample     = (UINT)Data;
#endif

    for (i = 0; i < Count; i ++)
    {
        if (pStorage[i] == uSample)
            return i;
    }

    return (LONG)-1;
}


