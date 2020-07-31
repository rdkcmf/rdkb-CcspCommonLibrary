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

    module:	ansc_byte_array_co_process.c

        For Byte Array object,
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

        06/03/02    initial revision.

    ---------------------------------------------------------------

    last modified:  

**********************************************************************/


#include "ansc_byte_array_co_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscByteArrayGetSize
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
AnscByteArrayGetSize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_BYTE_ARRAY_OBJECT         pByteArray  = (PANSC_BYTE_ARRAY_OBJECT)hThisObject;

    if (pByteArray)
        return pByteArray->ulItemCount;

    return 0;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscPtrArraySetSize
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
AnscByteArraySetSize
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulNewSize,
        ULONG                       ulGrowBy
    )
{
    PANSC_BYTE_ARRAY_OBJECT         pByteArray  = (PANSC_BYTE_ARRAY_OBJECT)hThisObject;

    /* enlarge the storage if neccessary */

    if (!pByteArray)
        return;

    /* change the growth if it's not "-1" (0xFFFFFFFF) */
    if (ulGrowBy != (ULONG)-1)
    {
        pByteArray->SetGrowth(hThisObject, ulGrowBy);
    }

    if (pByteArray->ulMaxItemCount != ulNewSize)
    {
        PBYTE                       pBuf;
        ULONG                       ulBufSize;

        ulBufSize   = sizeof(BYTE) * ulNewSize;

        /* resize the storage to the given size */
        if (!pByteArray->hStorage)
        {
            pBuf    = (PBYTE)AnscAllocateMemory(ulBufSize);
        }
        else
        {
/*
            pBuf    = (PBYTE)AnscReAllocMemory(pByteArray->hStorage, ulBufSize);
*/

            pBuf = (PBYTE)AnscMemUtilRealloc(pByteArray->hStorage, pByteArray->ulMaxItemCount * sizeof(BYTE), ulBufSize);
        }

        pByteArray->hStorage        = (ANSC_HANDLE)pBuf;
        pByteArray->ulMaxItemCount  = ulNewSize;
        pByteArray->ulItemCount     = ulNewSize;
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscPtrArrayRemoveAll
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
AnscByteArrayRemoveAll
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_BYTE_ARRAY_OBJECT         pByteArray  = (PANSC_BYTE_ARRAY_OBJECT)hThisObject;

    if (!pByteArray)
        return;

    /* we don't release the storage */
    pByteArray->ulItemCount = 0;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_OBJECT_ARRAY_DATA
        AnscPtrArrayGetAt
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
AnscByteArrayGetAt
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex
    )
{
    PANSC_BYTE_ARRAY_OBJECT         pByteArray  = (PANSC_BYTE_ARRAY_OBJECT)hThisObject;

    if (pByteArray && pByteArray->ulItemCount > 0 && ulIndex < pByteArray->ulItemCount)
    {
        PBYTE                       pStorage;

        pStorage    = (PBYTE)pByteArray->hStorage;

        return (ANSC_OBJECT_ARRAY_DATA)(intptr_t)pStorage[ulIndex];
    }

    return (ANSC_OBJECT_ARRAY_DATA)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscByteArraySetAt
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
AnscByteArraySetAt
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex,
        ANSC_OBJECT_ARRAY_DATA      Data
    )
{
    PANSC_BYTE_ARRAY_OBJECT         pByteArray  = (PANSC_BYTE_ARRAY_OBJECT)hThisObject;

    if (!pByteArray || pByteArray->ulMaxItemCount <= ulIndex)
        return;
    else
    {
        PBYTE                       pStorage;

        pStorage            = (PBYTE)pByteArray->hStorage;
        pStorage[ulIndex]   = (BYTE)(intptr_t)Data;
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscByteArrayInsertAt
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
AnscByteArrayInsertAt
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex,
        ANSC_OBJECT_ARRAY_DATA      Data,
        ULONG                       ulCount
    )
{
    PANSC_BYTE_ARRAY_OBJECT         pByteArray  = (PANSC_BYTE_ARRAY_OBJECT)hThisObject;

    if (!pByteArray || ulIndex >= pByteArray->ulItemCount)
        return;
    else
    {
        ULONG                       i;
        PBYTE                       pStorage;
  
        if (ulCount + pByteArray->ulItemCount >= pByteArray->ulMaxItemCount)
        {
            /* resize the storage */
            pByteArray->EnlargeStorage(hThisObject, pByteArray->ulItemCount + ulCount);
        }

        if (ulCount + pByteArray->ulItemCount > pByteArray->ulMaxItemCount)
        {
            /* failed to enlarge the storage */
            return;
        }

        /* move items after the given index (inclusive) */
        pStorage    = (PBYTE)pByteArray->hStorage;
        for (i = pByteArray->ulItemCount - 1; i >= ulIndex; i --)
        {
            pStorage[i + ulCount] = pStorage[i];
        }

        pByteArray->ulItemCount += ulCount;

        for (i = 0; i < ulCount; i ++)
        {
            pStorage[i + ulIndex]   = (BYTE)(intptr_t)Data;
        }
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscByteArrayRemoveAt
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
AnscByteArrayRemoveAt
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex,
        ULONG                       ulCount
    )
{
    UNREFERENCED_PARAMETER(ulCount);
    PANSC_BYTE_ARRAY_OBJECT         pByteArray  = (PANSC_BYTE_ARRAY_OBJECT)hThisObject;

    if (!pByteArray || ulIndex >= pByteArray->ulItemCount)
        return;
    else
    {
        PBYTE                       pStorage;
        ULONG                       i;

        pStorage    = (PBYTE)pByteArray->hStorage;

        for (i = ulIndex; i < pByteArray->ulItemCount; i ++)
        {
            pStorage[i] = pStorage[i + 1];
        }

        pByteArray->ulItemCount --;
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscByteArrayAdd
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
AnscByteArrayAdd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_OBJECT_ARRAY_DATA      Data
    )
{
    PANSC_BYTE_ARRAY_OBJECT         pByteArray  = (PANSC_BYTE_ARRAY_OBJECT)hThisObject;

    if (pByteArray)
    {
        if (pByteArray->ulItemCount + 1 >= pByteArray->ulMaxItemCount)
        {
            pByteArray->EnlargeStorage(hThisObject, pByteArray->ulItemCount + 1);
        }

        if (pByteArray->ulItemCount + 1 <= pByteArray->ulMaxItemCount)
        {
            PBYTE                   pStorage;

            pStorage = (PBYTE)pByteArray->hStorage;
            pStorage[pByteArray->ulItemCount ++] = (BYTE)(intptr_t)Data;

            return pByteArray->ulItemCount;
        }
    }

    return 0xFFFFFFFF;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscByteArrayEnlargeStorage
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
AnscByteArrayEnlargeStorage
    (   
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulNewSize
    )
{
    PANSC_BYTE_ARRAY_OBJECT         pByteArray  = (PANSC_BYTE_ARRAY_OBJECT)hThisObject;
    ULONG                           ulGrowth    = 1;
    ULONG                           ulNewMaxSize;

    /* enlarge the storage if neccessary */

    if (!pByteArray || pByteArray->bForeignBuffer)
    {
        AnscTrace("AnscByteArrayEnlargeStorage - failed to enlarge storage, foreign buffer may be used, or NULL pointer specified.\n");
        return;
    }

    ulGrowth    = pByteArray->ulGrowth;

    if (pByteArray->ulMaxItemCount <= ulNewSize)
    {
        PBYTE                       pBuf;
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

        ulBufSize   = sizeof(BYTE) * ulNewMaxSize;

        /* resize the storage to the given size */
        if (!pByteArray->hStorage)
        {
            pBuf    = (PBYTE)AnscAllocateMemory(ulBufSize);
        }
        else
        {
/*
            pBuf    = (PBYTE)AnscReAllocMemory(pByteArray->hStorage, ulBufSize); 
*/

            pBuf = (PUCHAR)AnscMemUtilRealloc(pByteArray->hStorage, pByteArray->ulMaxItemCount, ulNewMaxSize);
        }

        if (pBuf)
        {
            pByteArray->hStorage        = (ANSC_HANDLE)pBuf;
            pByteArray->ulMaxItemCount  = ulNewMaxSize;
        }
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG
        AnscByteArrayFind
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
AnscByteArrayFind
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_OBJECT_ARRAY_DATA      Data
    )
{
    PANSC_BYTE_ARRAY_OBJECT         pByteArray  = (PANSC_BYTE_ARRAY_OBJECT)hThisObject;
    LONG                            Count       = pByteArray->ulItemCount;
    LONG                            i;
    PBYTE                           pStorage    = (PBYTE)pByteArray->hStorage;
    BYTE                            uSample     = (BYTE)(intptr_t)Data;

    for (i = 0; i < Count; i ++)
    {
        if (pStorage[i] == uSample)
            return i;
    }

    return (LONG)-1;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscByteArrayGetBuffer
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulIndex
            )

    description:

        This function is called to return array buffer from the
        specified position.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                ULONG                       ulIndex
                The starting position of buffer.

    return:     handle to the buffer.

**********************************************************************/

ANSC_HANDLE
AnscByteArrayGetBuffer
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex
    )
{
    PANSC_BYTE_ARRAY_OBJECT         pByteArray  = (PANSC_BYTE_ARRAY_OBJECT)hThisObject;

    if (pByteArray && pByteArray->ulItemCount > 0 && ulIndex < pByteArray->ulItemCount)
    {
        PBYTE                       pStorage;

        pStorage    = (PBYTE)pByteArray->hStorage;

        return (ANSC_HANDLE)&pStorage[ulIndex];
    }

    return (ANSC_HANDLE)NULL;
}



/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        AnscByteArraySetBuffer
            (
                ANSC_HANDLE                 hThisObject,
                char                        *pBuf,
                ULONG                       ulSize,
                BOOL                        bForeignBuffer
            )

    description:

        This function is called to set array items with given buffer.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                char                        *pBuf
                String buffer to copy from.

                ULONG                       ulSize
                The size of buffer.

    return:     status of operation.

**********************************************************************/

BOOL
AnscByteArraySetBuffer
    (
        ANSC_HANDLE                 hThisObject,
        char                        *pBuf,
        ULONG                       ulSize,
        BOOL                        bForeignBuffer
    )
{
    PANSC_BYTE_ARRAY_OBJECT         pByteArray  = (PANSC_BYTE_ARRAY_OBJECT)hThisObject;

    if ( bForeignBuffer )
    {
        pByteArray->bForeignBuffer = TRUE;
        pByteArray->hStorage       = (ANSC_HANDLE)pBuf;
        pByteArray->ulMaxItemCount = ulSize;
    }
    else
    {
        if (pByteArray->ulMaxItemCount < ulSize)
        {
            pByteArray->EnlargeStorage(hThisObject, ulSize);

            if (pByteArray->ulMaxItemCount < ulSize)
            {
                return FALSE;
            }
        }

        AnscCopyMemory(pByteArray->hStorage, pBuf, ulSize);

        pByteArray->bForeignBuffer = FALSE;
    }

    pByteArray->ulItemCount = ulSize;

    return TRUE;
}


