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

    module:	ansc_string_array_co_process.c

        For String Array object,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the String Array Component Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        03/26/02    initial revision.

    ---------------------------------------------------------------

    last modified:  

**********************************************************************/


#include "ansc_string_array_co_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscStringArrayGetSize
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
AnscStringArrayGetSize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_STRING_ARRAY_OBJECT         pStringArray  = (PANSC_STRING_ARRAY_OBJECT)hThisObject;

    if (pStringArray)
        return pStringArray->ulItemCount;

    return 0;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscStringArraySetSize
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to set array size.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

    return:     void.

**********************************************************************/

void
AnscStringArraySetSize
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulNewSize,
        ULONG                       ulGrowBy
    )
{
    PANSC_STRING_ARRAY_OBJECT         pStringArray  = (PANSC_STRING_ARRAY_OBJECT)hThisObject;

    /* enlarge the storage if neccessary */

    if (!pStringArray)
        return;

    /* change the growth if it's not "-1" (0xFFFFFFFF) */
    if (ulGrowBy != (ULONG)-1)
    {
        pStringArray->SetGrowth(hThisObject, ulGrowBy);
    }

    if (pStringArray->ulMaxItemCount != ulNewSize)
    {
        PUCHAR                      *pBuf;
        ULONG                       ulBufSize;

        ulBufSize   = sizeof(PUCHAR) * ulNewSize;

        /* resize the storage to the given size */
        if (!pStringArray->hStorage)
        {
            pBuf    = (PUCHAR *)AnscAllocateMemory(ulBufSize);
        }
        else
        {
/*
            pBuf    = (PUCHAR *)AnscReAllocMemory(pStringArray->hStorage, ulBufSize);
*/

            pBuf = (PUCHAR *)AnscMemUtilRealloc(pStringArray->hStorage, pStringArray->ulMaxItemCount * sizeof(PUCHAR), ulBufSize);
        }

        pStringArray->hStorage        = (ANSC_HANDLE)pBuf;
        pStringArray->ulMaxItemCount  = ulNewSize;
        pStringArray->ulItemCount     = ulNewSize;
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscStringArrayRemoveAll
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
AnscStringArrayRemoveAll
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_STRING_ARRAY_OBJECT       pStringArray  = (PANSC_STRING_ARRAY_OBJECT)hThisObject;
    PUCHAR                          *pStrArray, pStr;
    ULONG                           i;

    if (!pStringArray)
        return;

    /* we don't release the storage */

    pStrArray   = (PUCHAR *)pStringArray->hStorage;
    for (i = 0; i < pStringArray->ulItemCount; i ++)
    {
        pStr    = pStrArray[i];
        if (pStr)
        {
            AnscFreeMemory(pStr);
        }
    }

    pStringArray->ulItemCount = 0;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_OBJECT_ARRAY_DATA
        AnscStringArrayGetAt
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulIndex
            )

    description:

        This function is called to get array item at the specified 
        position.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                ULONG                       ulIndex

    return:     the array item.

**********************************************************************/

ANSC_OBJECT_ARRAY_DATA
AnscStringArrayGetAt
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex
    )
{
    PANSC_STRING_ARRAY_OBJECT       pStringArray  = (PANSC_STRING_ARRAY_OBJECT)hThisObject;

    if (pStringArray && pStringArray->ulItemCount > 0 && ulIndex < pStringArray->ulItemCount)
    {
        PUCHAR                      *pStorage;

        pStorage    = (PUCHAR *)pStringArray->hStorage;

        return (ANSC_OBJECT_ARRAY_DATA)pStorage[ulIndex];
    }

    return (ANSC_OBJECT_ARRAY_DATA)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscStringArraySetSize
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulIndex,
                ANSC_OBJECT_ARRAY_DATA      Data
            )

    description:

        This function is called to set given item in array at the
        specified position.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                ULONG                       ulIndex
                The position to put the array item.

                ANSC_OBJECT_ARRAY_DATA      Data
                Array data.

    return:     void.

**********************************************************************/

void
AnscStringArraySetAt
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex,
        ANSC_OBJECT_ARRAY_DATA      Data
    )
{
    PANSC_STRING_ARRAY_OBJECT       pStringArray  = (PANSC_STRING_ARRAY_OBJECT)hThisObject;

    if (!pStringArray || pStringArray->ulMaxItemCount <= ulIndex)
        return;
    else
    {
        PUCHAR                      *pStorage;
        char                      *pData, *pStr;

        pStorage    = (PUCHAR *)pStringArray->hStorage;

        pData       = Data;
        pStr = strdup(pData);
        if (! pStr)
        {
            return;
        }

        pStorage[ulIndex]   = (PUCHAR)pStr;
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscStringArrayInsertAt
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulIndex,
                ANSC_OBJECT_ARRAY_DATA      Data,
                ULONG                       ulCount
            )

    description:

        This function is called to insert given amount of array items
        at the specified position.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                ULONG                       ulIndex
                The position where to insert array items.

                ANSC_OBJECT_ARRAY_DATA      Data
                Array item data.

                ULONG                       ulCount
                The number of items to be inserted.

    return:     void.

**********************************************************************/

void
AnscStringArrayInsertAt
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex,
        ANSC_OBJECT_ARRAY_DATA      Data,
        ULONG                       ulCount
    )
{
    PANSC_STRING_ARRAY_OBJECT       pStringArray  = (PANSC_STRING_ARRAY_OBJECT)hThisObject;

    if (!pStringArray || ulIndex >= pStringArray->ulItemCount || Data == 0)
        return;
    else
    {
        ULONG                       i;
        PUCHAR                      *pStorage;
        char                      *pData, *pStr;
  
        if (ulCount + pStringArray->ulItemCount > pStringArray->ulMaxItemCount)
        {
            /* resize the storage */
            pStringArray->EnlargeStorage(hThisObject, pStringArray->ulItemCount + ulCount);
        }

        if (ulCount + pStringArray->ulItemCount > pStringArray->ulMaxItemCount)
        {
            /* failed to enlarge the storage */
            return;
        }

        /* move items after the given index (inclusive) */
        pStorage    = (PUCHAR *)pStringArray->hStorage;
        for (i = pStringArray->ulItemCount - 1; i >= ulIndex; i --)
        {
            pStorage[i + ulCount] = pStorage[i];
        }

        pStringArray->ulItemCount += ulCount;

        for (i = 0; i < ulCount; i ++)
        {
            pData       = Data;
            pStr = strdup(pData);
            if (! pStr)
            {
                return;
            }
            pStorage[i + ulIndex]   = (PUCHAR)pStr;
        }
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscStringArrayRemoveAt
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulIndex,
                ULONG                       ulCount
            )

    description:

        This function is called to remove the specified number of
        array items from the specified position.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                ULONG                       ulIndex
                Where to remove array items.

                ULONG                       ulCount
                The number of items to be removed.

    return:     void.

**********************************************************************/

void
AnscStringArrayRemoveAt
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex,
        ULONG                       ulCount
    )
{
    UNREFERENCED_PARAMETER(ulCount);
    PANSC_STRING_ARRAY_OBJECT       pStringArray  = (PANSC_STRING_ARRAY_OBJECT)hThisObject;

    if (!pStringArray || ulIndex >= pStringArray->ulItemCount)
        return;
    else
    {
        PUCHAR                      *pStorage;
        ULONG                       i;
        PUCHAR                      pStr;

        pStorage    = (PUCHAR *)pStringArray->hStorage;
        pStr        = pStorage[ulIndex];

        for (i = ulIndex; i < pStringArray->ulItemCount; i ++)
        {
            pStorage[i] = pStorage[i + 1];
        }

        AnscFreeMemory(pStr);
        pStringArray->ulItemCount --;
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscStringArrayAdd
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_OBJECT_ARRAY_DATA      Data
            )

    description:

        This function is called to add an array item.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                ANSC_OBJECT_ARRAY_DATA      Data
                Array item data.

    return:     the position this item is located.

**********************************************************************/

ULONG
AnscStringArrayAdd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_OBJECT_ARRAY_DATA      Data
    )
{
    PANSC_STRING_ARRAY_OBJECT       pStringArray  = (PANSC_STRING_ARRAY_OBJECT)hThisObject;

    if (pStringArray)
    {
        if (pStringArray->ulItemCount + 1 > pStringArray->ulMaxItemCount)
        {
            pStringArray->EnlargeStorage(hThisObject, pStringArray->ulItemCount + 1);
        }

        if (pStringArray->ulItemCount + 1 <= pStringArray->ulMaxItemCount)
        {
            PUCHAR                  *pStorage;
            char                  *pData, *pStr;

            pData       = Data;

            pStr = strdup(pData);
            if (pStr)
            {

                pStorage = (PUCHAR *)pStringArray->hStorage;
                pStorage[pStringArray->ulItemCount ++] = (PUCHAR)pStr;

                return pStringArray->ulItemCount;
            }
        }
    }

    return 0xFFFFFFFF;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscStringArrayEnlargeStorage
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulNewSize
            )

    description:

        This function is called to enlarge array size.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                ULONG                       ulNewSize
                The new size of the array.

    return:     void.

**********************************************************************/

void
AnscStringArrayEnlargeStorage
    (   
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulNewSize
    )
{
    PANSC_STRING_ARRAY_OBJECT       pStringArray  = (PANSC_STRING_ARRAY_OBJECT)hThisObject;
    ULONG                           ulGrowth    = 1;
    ULONG                           ulNewMaxSize;

    /* enlarge the storage if neccessary */

    if (!pStringArray)
        return;

    ulGrowth    = pStringArray->ulGrowth;

    if (pStringArray->ulMaxItemCount < ulNewSize)
    {
        PUCHAR                      *pBuf;
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

        ulBufSize   = sizeof(PUCHAR) * ulNewMaxSize;

        /* resize the storage to the given size */
        if (!pStringArray->hStorage)
        {
            pBuf    = (PUCHAR *)AnscAllocateMemory(ulBufSize);
        }
        else
        {
/*
            pBuf    = (PUCHAR *)AnscReAllocMemory(pStringArray->hStorage, ulBufSize);
*/
            pBuf = (PUCHAR *)AnscMemUtilRealloc(pStringArray->hStorage, pStringArray->ulMaxItemCount * sizeof(PUCHAR), ulBufSize);
        }

        if (pBuf)
        {
            pStringArray->hStorage        = (ANSC_HANDLE)pBuf;
            pStringArray->ulMaxItemCount  = ulNewMaxSize;
        }
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG
        AnscStringArrayArrayFind
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_OBJECT_ARRAY_DATA      Data
            )

    description:

        This function is called to find a specified item.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                ANSC_OBJECT_ARRAY_DATA      Data
                The array item data to be matched.

    return:     the position found, -1 if no match.

**********************************************************************/

LONG
AnscStringArrayFind
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_OBJECT_ARRAY_DATA      Data
    )
{
    PANSC_STRING_ARRAY_OBJECT       pStringArray  = (PANSC_STRING_ARRAY_OBJECT)hThisObject;
    LONG                            Count       = pStringArray->ulItemCount;
    LONG                            i;
    PANSC_STRING_ARRAY_FIND_DATA    pFindData   = (PANSC_STRING_ARRAY_FIND_DATA)Data;
    PUCHAR                          *pStrArray;
    PUCHAR                          pStrFind;
    ULONG                           ulStrFindLen;
    PUCHAR                          pStr;
    ULONG                           ulStrLen;
    BOOL                            bCaseSensitive, bEqual;

    pStrArray       = (PUCHAR *)pStringArray->hStorage;
    pStrFind        = pFindData->pStr;
    ulStrFindLen    = pFindData->ulStrLen;
    bCaseSensitive  = pFindData->bCaseSensitive;

    for (i = 0; i < Count; i ++)
    {
        pStr        = pStrArray[i];
        if (pStr)
        {
            ulStrLen    = AnscSizeOfString((const char *)pStr);

            if (ulStrLen == ulStrFindLen)
            {
                if (bCaseSensitive)
                {
                    bEqual  = AnscEqualMemory(pStr, pStrFind, ulStrLen);
                }
                else
                {
                    bEqual  = AnscEqualMemoryNoCase(pStr, pStrFind, ulStrLen);
                }

                if (bEqual)
                    return i;
            }
        }
    }

    return (LONG)-1;
}


