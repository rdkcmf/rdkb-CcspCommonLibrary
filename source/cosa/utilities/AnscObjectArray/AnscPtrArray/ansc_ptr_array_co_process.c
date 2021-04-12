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

    module:	ansc_ptr_array_co_process.c

        For Ptr Array object,
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

        03/17/02    initial revision.

    ---------------------------------------------------------------

    last modified:  

**********************************************************************/


#include "ansc_ptr_array_co_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscPtrArrayGetSize
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to get items in array.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

    return:     the number of items.

**********************************************************************/

ULONG
AnscPtrArrayGetSize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_PTR_ARRAY_OBJECT          pPtrArray  = (PANSC_PTR_ARRAY_OBJECT)hThisObject;

    if (pPtrArray)
        return pPtrArray->ulItemCount;

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

        This function is called to set new size and grow size.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                ULONG                       ulNewSize
                The new array size.

                ULONG                       ulGrowBy
                The minimum growth size when array buffer needs to be
                enlarged.

    return:     void.

**********************************************************************/

void
AnscPtrArraySetSize
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulNewSize,
        ULONG                       ulGrowBy
    )
{
    PANSC_PTR_ARRAY_OBJECT          pPtrArray  = (PANSC_PTR_ARRAY_OBJECT)hThisObject;

    /* enlarge the storage if neccessary */

    if (!pPtrArray)
        return;

    /* change the growth if it's not "-1" (0xFFFFFFFF) */
    if (ulGrowBy != (ULONG)-1)
    {
        pPtrArray->SetGrowth(hThisObject, ulGrowBy);
    }

    if (pPtrArray->ulMaxItemCount != ulNewSize)
    {
        PANSC_PTR_ARRAY_OBJECT      pBuf;
        ULONG                       ulBufSize;

        ulBufSize   = ANSC_PTR_ARRAY_ITEM_SIZE * ulNewSize;

        /* resize the storage to the given size */
        if (!pPtrArray->hStorage)
        {
            pBuf    = (PANSC_PTR_ARRAY_OBJECT)AnscAllocateMemory(ulBufSize);
        }
        else
        {
/*
            pBuf    = (PANSC_PTR_ARRAY_OBJECT)AnscReAllocMemory(pPtrArray->hStorage, ulBufSize);
*/

            pBuf    = 
                (PANSC_PTR_ARRAY_OBJECT)AnscMemUtilRealloc
                    (
                        pPtrArray->hStorage, 
                        pPtrArray->ulMaxItemCount * ANSC_PTR_ARRAY_ITEM_SIZE, 
                        ulBufSize
                    );
        }

        pPtrArray->hStorage        = (ANSC_HANDLE)pBuf;
        pPtrArray->ulMaxItemCount  = ulNewSize;
        pPtrArray->ulItemCount     = ulNewSize;
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
AnscPtrArrayRemoveAll
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_PTR_ARRAY_OBJECT         pPtrArray  = (PANSC_PTR_ARRAY_OBJECT)hThisObject;

    if (!pPtrArray)
        return;

    /* we don't release the storage */
    pPtrArray->ulItemCount = 0;
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
                The position.

    return:     handle to array item.

**********************************************************************/

ANSC_OBJECT_ARRAY_DATA
AnscPtrArrayGetAt
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex
    )
{
    PANSC_PTR_ARRAY_OBJECT          pPtrArray  = (PANSC_PTR_ARRAY_OBJECT)hThisObject;

    if (pPtrArray && pPtrArray->ulItemCount > 0 && ulIndex < pPtrArray->ulItemCount)
    {
        PANSC_PTR_ARRAY_DATA     pStorage;

        pStorage    = (PANSC_PTR_ARRAY_DATA)pPtrArray->hStorage;

        return (ANSC_OBJECT_ARRAY_DATA)pStorage[ulIndex];
    }

    return (ANSC_OBJECT_ARRAY_DATA)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscPtrArraySetAt
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
AnscPtrArraySetAt
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex,
        ANSC_OBJECT_ARRAY_DATA      Data
    )
{
    PANSC_PTR_ARRAY_OBJECT          pPtrArray  = (PANSC_PTR_ARRAY_OBJECT)hThisObject;

    if (!pPtrArray || pPtrArray->ulMaxItemCount <= ulIndex)
        return;
    else
    {
        PANSC_PTR_ARRAY_DATA        pStorage;

        pStorage            = (PANSC_PTR_ARRAY_DATA)pPtrArray->hStorage;
        pStorage[ulIndex]   = (ANSC_PTR_ARRAY_DATA)Data;
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscPtrArrayInsertAt
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
AnscPtrArrayInsertAt
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex,
        ANSC_OBJECT_ARRAY_DATA      Data,
        ULONG                       ulCount
    )
{
    PANSC_PTR_ARRAY_OBJECT          pPtrArray  = (PANSC_PTR_ARRAY_OBJECT)hThisObject;

    if (!pPtrArray || ulIndex >= pPtrArray->ulItemCount)
        return;
    else
    {
        ULONG                       i;
        PANSC_PTR_ARRAY_DATA        pStorage;
  
        if (ulCount + pPtrArray->ulItemCount > pPtrArray->ulMaxItemCount)
        {
            /* resize the storage */
            pPtrArray->EnlargeStorage(hThisObject, pPtrArray->ulItemCount + ulCount);
        }

        if (ulCount + pPtrArray->ulItemCount > pPtrArray->ulMaxItemCount)
        {
            /* failed to enlarge the storage */
            return;
        }

        /* move items after the given index (inclusive) */
        pStorage    = (PANSC_PTR_ARRAY_DATA)pPtrArray->hStorage;
        for (i = pPtrArray->ulItemCount - 1; i >= ulIndex; i --)
        {
            pStorage[i + ulCount] = pStorage[i];
        }

        pPtrArray->ulItemCount += ulCount;

        for (i = 0; i < ulCount; i ++)
        {
            pStorage[i + ulIndex]   = (ANSC_PTR_ARRAY_DATA)Data;
        }
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscPtrArrayRemoveAt
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
AnscPtrArrayRemoveAt
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex,
        ULONG                       ulCount
    )
{
    UNREFERENCED_PARAMETER(ulCount);
    PANSC_PTR_ARRAY_OBJECT          pPtrArray  = (PANSC_PTR_ARRAY_OBJECT)hThisObject;

    if (!pPtrArray || ulIndex >= pPtrArray->ulItemCount)
        return;
    else
    {
        PANSC_PTR_ARRAY_DATA        pStorage;
        ULONG                       i;

        pStorage    = (PANSC_PTR_ARRAY_DATA)pPtrArray->hStorage;

        for (i = ulIndex; i < pPtrArray->ulItemCount; i ++)
        {
            pStorage[i] = pStorage[i + 1];
        }

        pPtrArray->ulItemCount --;
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscPtrArrayAdd
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
AnscPtrArrayAdd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_OBJECT_ARRAY_DATA      Data
    )
{
    PANSC_PTR_ARRAY_OBJECT          pPtrArray  = (PANSC_PTR_ARRAY_OBJECT)hThisObject;

    if (pPtrArray)
    {
        if (pPtrArray->ulItemCount + 1 > pPtrArray->ulMaxItemCount)
        {
            pPtrArray->EnlargeStorage(hThisObject, pPtrArray->ulItemCount + 1);
        }

        if (pPtrArray->ulItemCount + 1 <= pPtrArray->ulMaxItemCount)
        {
            PANSC_PTR_ARRAY_DATA    pStorage;

            pStorage = (PANSC_PTR_ARRAY_DATA)pPtrArray->hStorage;
            pStorage[pPtrArray->ulItemCount ++] = (ANSC_PTR_ARRAY_DATA)Data;

            return pPtrArray->ulItemCount;
        }
    }

    return 0xFFFFFFFF;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscPtrArrayEnlargeStorage
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
AnscPtrArrayEnlargeStorage
    (   
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulNewSize
    )
{
    PANSC_PTR_ARRAY_OBJECT          pPtrArray  = (PANSC_PTR_ARRAY_OBJECT)hThisObject;
    ULONG                           ulGrowth    = 1;
    ULONG                           ulNewMaxSize;

    /* enlarge the storage if neccessary */

    if (!pPtrArray)
        return;

    ulGrowth    = pPtrArray->ulGrowth;

    if (pPtrArray->ulMaxItemCount < ulNewSize)
    {
        PANSC_PTR_ARRAY_DATA        pBuf;
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

        ulBufSize   = ANSC_PTR_ARRAY_ITEM_SIZE * ulNewMaxSize;

        /* resize the storage to the given size */
        if (!pPtrArray->hStorage)
        {
            pBuf    = (PANSC_PTR_ARRAY_DATA)AnscAllocateMemory(ulBufSize);
        }
        else
        {
/*
            pBuf    = (PANSC_PTR_ARRAY_DATA)AnscReAllocMemory(pPtrArray->hStorage, ulBufSize);
*/
            pBuf    = 
                (PANSC_PTR_ARRAY_DATA)AnscMemUtilRealloc
                    (
                        pPtrArray->hStorage, 
                        pPtrArray->ulMaxItemCount * ANSC_PTR_ARRAY_ITEM_SIZE, 
                        ulBufSize
                    );
        }

        if (pBuf)
        {
            pPtrArray->hStorage        = (ANSC_HANDLE)pBuf;
            pPtrArray->ulMaxItemCount  = ulNewMaxSize;
        }
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG
        AnscPtrArrayFind
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
AnscPtrArrayFind
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_OBJECT_ARRAY_DATA      Data
    )
{
    PANSC_PTR_ARRAY_OBJECT          pPtrArray   = (PANSC_PTR_ARRAY_OBJECT)hThisObject;
    LONG                            Count       = pPtrArray->ulItemCount;
    LONG                            i;
    PANSC_PTR_ARRAY_DATA            pStorage    = (PANSC_PTR_ARRAY_DATA)pPtrArray->hStorage;

    for (i = 0; i < Count; i ++)
    {
        if ((ANSC_OBJECT_ARRAY_DATA)pStorage[i] == Data)
            return i;
    }

    return (LONG)-1;
}


