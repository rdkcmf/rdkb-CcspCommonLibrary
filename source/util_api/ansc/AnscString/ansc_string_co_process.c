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

    module:	ansc_string_co_process.c

        For string,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2001
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the string Component Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        03/13/02    initial revision.

    ---------------------------------------------------------------

    last modified:  

**********************************************************************/


#include "ansc_string_co_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscStringCOSetString
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      pValue
            )

    description:

        This function is called to set string value.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                PUCHAR                      pValue
                The string value.

    return:     void.

**********************************************************************/

void
AnscStringCOSetString
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pValue              /* ZERO-terminated string */
    )
{
    PANSC_STRING_OBJECT             pStringObj  = (PANSC_STRING_OBJECT)hThisObject;
    ULONG                           ulLen;
    ULONG                           ulCopySize;

    if (!pStringObj || !pValue)
        return;

    ulLen   = AnscSizeOfString(pValue);
    
    /* buffer is not big enough */
    if (pStringObj->ulValueBufferLen <= ulLen)
    {
        pStringObj->Resize(hThisObject, ulLen + 1);
    }

    ulCopySize  = ulLen;
    if (ulCopySize >= pStringObj->ulValueBufferLen)
    {
        /* have to trunk the string */
        ulCopySize  = pStringObj->ulValueBufferLen - 1;
    }

    pStringObj->SetBuffer(hThisObject, pValue, ulCopySize);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscStringCOSetBuffer
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      pBuffer,
                ULONG                       ulBufferLen
            )

    description:

        This function is called to set string value.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                PUCHAR                      pBuffer
                The buffer of string value.

                ULONG                       ulBufferLen
                The size of buffer.

    return:     void.

**********************************************************************/

void
AnscStringCOSetBuffer
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pBuffer,            /* no need to ZERO-terminated and 0s are allowed in the buffer */
        ULONG                       ulBufferLen
    )
  
{
    PANSC_STRING_OBJECT             pStringObj  = (PANSC_STRING_OBJECT)hThisObject;
    ULONG                           ulCopySize;

    if (!pStringObj || !pBuffer || ulBufferLen == 0)
        return;

    /* buffer is not big enough */
    if (pStringObj->ulValueBufferLen <= ulBufferLen)
    {
        pStringObj->Resize(hThisObject, ulBufferLen + 1);
    }

    ulCopySize  = ulBufferLen;
    if (ulCopySize >= pStringObj->ulValueBufferLen)
        ulCopySize = pStringObj->ulValueBufferLen - 1;

    if (ulCopySize != 0)
    {
        AnscCopyMemory(pStringObj->pValue, pBuffer, ulCopySize);
        pStringObj->ulValueLen          = ulCopySize;
        pStringObj->pValue[ulCopySize]  = 0;            /* terminate the string anyway */
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscStringCOCopy
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hString
            )

    description:

        This function is called to copy string.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                ANSC_HANDLE                 hString
                The string to copy from.

    return:     void.

**********************************************************************/

void
AnscStringCOCopy
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hString             /* another string to be copied */
    )
{
    PANSC_STRING_OBJECT             pStringObj  = (PANSC_STRING_OBJECT)hThisObject;
    PANSC_STRING_OBJECT             pSrcString  = (PANSC_STRING_OBJECT)hString;

    if (!pStringObj | !pSrcString)
        return;
    else
    {
        PUCHAR                      pSrcBuf;
        ULONG                       ulSrcBufLen;

        pSrcBuf     = pSrcString->GetString(hString);
        ulSrcBufLen = pSrcString->GetLength(hString);

        pStringObj->SetBuffer(hThisObject, pSrcBuf, ulSrcBufLen);
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscStringCOGetLength
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to get the length of string value.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

    return:     the length of string.

**********************************************************************/

ULONG
AnscStringCOGetLength
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_STRING_OBJECT             pStringObj  = (PANSC_STRING_OBJECT)hThisObject;

    if (!pStringObj)
        return 0;
    else
        return pStringObj->ulValueLen;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        AnscStringCOIsEmpty
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to examine if the string is empty

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

    return:     TRUE if it's empty, FALSE otherwise.

**********************************************************************/

BOOL
AnscStringCOIsEmpty
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_STRING_OBJECT             pStringObj  = (PANSC_STRING_OBJECT)hThisObject;

    if (!pStringObj)
        return TRUE;
    else
        return (pStringObj->ulValueLen == 0 || !pStringObj->pValue);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscStringCOEmpty
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to empty the string.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

    return:     void.

**********************************************************************/

void
AnscStringCOEmpty
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_STRING_OBJECT             pStringObj  = (PANSC_STRING_OBJECT)hThisObject;

    /* 
     * a trick here, we don't release the memory because this function
     * is generally called before user want to do something.
     */

    if (pStringObj)
    {
        pStringObj->ulValueLen  = 0;
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        UCHAR
        AnscStringCOGetAt
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulIndex
            )

    description:

        This function is called to the character at specified position.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                ULONG                       ulIndex
                The index of the character.

    return:     the character.

**********************************************************************/

UCHAR
AnscStringCOGetAt
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex
    )
{
    PANSC_STRING_OBJECT             pStringObj  = (PANSC_STRING_OBJECT)hThisObject;
    
    if (!pStringObj || ulIndex >= pStringObj->ulValueLen)
        return (UCHAR)0;
    else
    {
        return pStringObj->pValue[ulIndex];
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscStringCOSetAt
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulIndex,
                UCHAR                       uChar
            )

    description:

        This function is called to put the character in the string
        at specified position.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                ULONG                       ulIndex
                The index position in string.

                UCHAR                       uChar
                Character to put.

    return:     void.

**********************************************************************/

void
AnscStringCOSetAt
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex,
        UCHAR                       uChar
    )
{
    PANSC_STRING_OBJECT             pStringObj  = (PANSC_STRING_OBJECT)hThisObject;

    if (!pStringObj || ulIndex >= pStringObj->ulValueLen)
        return;
    else
    {
        pStringObj->pValue[ulIndex] = uChar;
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        PUCHAR
        AnscStringCOGetString
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to get string value.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

    return:     the string value.

**********************************************************************/

PUCHAR
AnscStringCOGetString
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_STRING_OBJECT             pStringObj  = (PANSC_STRING_OBJECT)hThisObject;

    if (!pStringObj)
        return NULL;
    else
        return pStringObj->pValue;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        AnscStringCOEqual
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      pString
            )

    description:

        This function is called to examine if it equals to given
        string.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                PUCHAR                      pString
                The string to be compared.

    return:     TRUE if they are equal, FALSE otherwise.

**********************************************************************/

BOOL
AnscStringCOEqual
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pString
    )
{
    PANSC_STRING_OBJECT             pStringObj  = (PANSC_STRING_OBJECT)hThisObject;
    ULONG                           ulStrLen;

    if (!pString || !pStringObj || !pStringObj->pValue || pStringObj->ulValueLen == 0)
        return FALSE;

    ulStrLen    = AnscSizeOfString(pString);
    
    if (ulStrLen != pStringObj->ulValueLen)
        return FALSE;

    return AnscEqualMemory(pStringObj->pValue, pString, ulStrLen);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscStringCOEqualNoCase
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      pString
            )

    description:

        This function is called to examine if string value
        equals to the given string in case insensitive.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                PUCHAR                      pValue
                The string value.

    return:     void.

**********************************************************************/

BOOL
AnscStringCOEqualNoCase
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pString
    )
{
    PANSC_STRING_OBJECT             pStringObj  = (PANSC_STRING_OBJECT)hThisObject;
    ULONG                           ulStrLen;

    if (!pString || !pStringObj || !pStringObj->pValue || pStringObj->ulValueLen == 0)
        return FALSE;

    ulStrLen    = AnscSizeOfString(pString);
    
    if (ulStrLen != pStringObj->ulValueLen)
        return FALSE;

    return AnscEqualMemoryNoCase(pStringObj->pValue, pString, ulStrLen);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscStringCOLeft
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulCount
            )

    description:

        This function is called to get left substring with
        specified number of characters.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                ULONG                       ulCount
                The number of characters.

    return:     handle the new string object.

**********************************************************************/

ANSC_HANDLE
AnscStringCOLeft
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCount
    )
{
    PANSC_STRING_OBJECT             pStringObj  = (PANSC_STRING_OBJECT)hThisObject;
    ULONG                           ulLen;

    if (!pStringObj || ulCount == 0)
        return (ANSC_HANDLE)NULL;
    else
    {
        ulLen   = ulCount;

        if (ulLen >= pStringObj->ulValueLen)
            ulLen = pStringObj->ulValueLen;

        if (ulLen == 0)
            return (ANSC_HANDLE)NULL;
        else
        {
            PUCHAR                  pSrcBuf;
            PANSC_STRING_OBJECT     pNewStringObj;

            pSrcBuf = pStringObj->GetString(hThisObject);

            pNewStringObj = (PANSC_STRING_OBJECT)AnscCreateStringComponent(NULL, NULL, NULL);

            if (pNewStringObj)
            {
                pNewStringObj->SetBuffer((ANSC_HANDLE)pNewStringObj, pSrcBuf, ulLen);
            }

            return (ANSC_HANDLE)pNewStringObj;
        }
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscStringCORight
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulCount
            )

    description:

        This function is called to get the specified number of 
        string from the right side.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                ULONG                       ulCount
                The number of bytes.

    return:     the handle to the new string.

**********************************************************************/

ANSC_HANDLE
AnscStringCORight
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCount
    )
{
    PANSC_STRING_OBJECT             pStringObj  = (PANSC_STRING_OBJECT)hThisObject;
    ULONG                           ulLen;

    if (!pStringObj || ulCount == 0)
        return (ANSC_HANDLE)NULL;
    else
    {
        ulLen   = ulCount;

        if (ulLen >= pStringObj->ulValueLen)
            ulLen = pStringObj->ulValueLen;

        if (ulLen == 0)
            return (ANSC_HANDLE)NULL;
        else
        {
            PUCHAR                  pSrcBuf;
            PANSC_STRING_OBJECT     pNewStringObj;

            pSrcBuf = pStringObj->GetString(hThisObject);

            pNewStringObj = (PANSC_STRING_OBJECT)AnscCreateStringComponent(NULL, NULL, NULL);

            if (pNewStringObj)
            {
                pSrcBuf += (pStringObj->ulValueLen - ulLen);
                pNewStringObj->SetBuffer((ANSC_HANDLE)pNewStringObj, pSrcBuf, ulLen);
            }

            return (ANSC_HANDLE)pNewStringObj;
        }
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG
        AnscStringCOFindChar
            (
                ANSC_HANDLE                 hThisObject,
                UCHAR                       uChar
            )

    description:

        This function is called to set string value.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                UCHAR                       uChar
                The character to be searched.

    return:     position found, -1 if no match.

**********************************************************************/

LONG
AnscStringCOFindChar
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       uChar
    )
{
    PANSC_STRING_OBJECT             pStringObj  = (PANSC_STRING_OBJECT)hThisObject;

    if (!pStringObj || !pStringObj->pValue || pStringObj->ulValueLen == 0)
        return (LONG)-1;
    else
    {
        ULONG                       i;

        for (i = 0; i < pStringObj->ulValueLen; i ++)
        {
            if (pStringObj->pValue[i] == uChar)
                return (LONG)i;
        }

        return (LONG)-1;
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG
        AnscStringCOFindString
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      pString
            )

    description:

        This function is called to search the given string.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                PUCHAR                      pString
                The string to be searched.

    return:     the position found, -1 if no match.

**********************************************************************/

LONG
AnscStringCOFindString
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pString
    )
{
    PANSC_STRING_OBJECT             pStringObj  = (PANSC_STRING_OBJECT)hThisObject;
    if (!pStringObj || !pString || !pStringObj->pValue || pStringObj->ulValueLen == 0)
        return (LONG)-1;
    else
    {
        PUCHAR                      pSrcString;
        PUCHAR                      pPos;

        pSrcString  = pStringObj->GetString(hThisObject);

        pPos        = AnscStrStr(pSrcString, pString);
        if (!pPos)
            return (LONG)-1;
        else
            return (LONG)(pPos - pSrcString);
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscStringCOUpper
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to make string value upper case.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

    return:     void.

**********************************************************************/

void
AnscStringCOUpper
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_STRING_OBJECT             pStringObj  = (PANSC_STRING_OBJECT)hThisObject;

    if (pStringObj)
    {
        ULONG                       i;
        UCHAR                       uChar;

        for (i = 0; i < pStringObj->ulValueLen; i ++)
        {
            uChar   = pStringObj->pValue[i];

            if (uChar >= 'a' && uChar <= 'z')
            {
                uChar                   = 'A' + (uChar - 'a');
                pStringObj->pValue[i]   = uChar;
            }
        }
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscStringCOLower
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to make string value lower case.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

    return:     void.

**********************************************************************/

void
AnscStringCOLower
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_STRING_OBJECT             pStringObj  = (PANSC_STRING_OBJECT)hThisObject;

    if (pStringObj)
    {
        ULONG                       i;
        UCHAR                       uChar;

        for (i = 0; i < pStringObj->ulValueLen; i ++)
        {
            uChar   = pStringObj->pValue[i];

            if (uChar >= 'A' && uChar <= 'Z')
            {
                uChar                   = 'a' + (uChar - 'A');
                pStringObj->pValue[i]   = uChar;
            }
        }
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscStringCOResize
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulNewSize
            )

    description:

        This function is called to resize string buffer.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                ULONG                       ulNewSize
                The new size of string buffer.

    return:     void.

**********************************************************************/

void
AnscStringCOResize
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulNewSize
    )
{
    PANSC_STRING_OBJECT             pStringObj  = (PANSC_STRING_OBJECT)hThisObject;
    /* resize current string buffer to the given size */

    if (pStringObj && pStringObj->ulValueBufferLen < ulNewSize)
    {
        PUCHAR                      pBuf;

        /* increase the buffer size s */
        if (ulNewSize - pStringObj->ulValueBufferLen < ANSC_STRING_RESIZE_MIN_INC)
        {
            ulNewSize   = pStringObj->ulValueBufferLen + ANSC_STRING_RESIZE_MIN_INC;
        }

#if 0
        if (!pStringObj->pValue)
            pBuf = (PUCHAR)AnscAllocateMemory(ulNewSize);
        else
        {
            ULONG                   ulCopySize;

            pBuf = (PUCHAR)AnscAllocateMemory(ulNewSize);

            if (pBuf)
            {
                ulCopySize  = pStringObj->ulValueBufferLen;
                if (ulCopySize >= ulNewSize)
                    ulCopySize  = ulNewSize - 1;

                AnscCopyMemory(pBuf, pStringObj->pValue, ulCopySize);
                pBuf[ulCopySize]    = 0;
            }
        }

        if (pBuf)
        {
            if (pStringObj->pValue)
            {
                AnscFreeMemory(pStringObj->pValue);
            }

            pStringObj->pValue              = pBuf;
            pStringObj->ulValueBufferLen    = ulNewSize;
        }
#endif

        if (!pStringObj->pValue)
        {
            pBuf = (PUCHAR)AnscAllocateMemory(ulNewSize);

            pStringObj->pValue              = pBuf;
            pStringObj->ulValueBufferLen    = ulNewSize;
        }
        else
        {
/*
            pBuf = (PUCHAR)AnscReAllocMemory(pStringObj->pValue, ulNewSize);
*/
            
            pBuf = (PUCHAR)AnscMemUtilRealloc(pStringObj->pValue, pStringObj->ulValueBufferLen, ulNewSize);

            pStringObj->pValue              = pBuf;
            pStringObj->ulValueBufferLen    = ulNewSize;
        }
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscStringCOTrimLeftChar
            (
                ANSC_HANDLE                 hThisObject,
                UCHAR                       uChar
            )

    description:

        This function is called to trim down the specified character
        from left.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                UCHAR                       uChar
                The character to trim down.

    return:     void.

**********************************************************************/

void
AnscStringCOTrimLeftChar
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       uChar
    )
{
    PANSC_STRING_OBJECT             pStringObj  = (PANSC_STRING_OBJECT)hThisObject;

    if (pStringObj)
    {
        ULONG                       ulStrLen, i, pos;
        PUCHAR                      pValue;

        pValue      = pStringObj->pValue;
        ulStrLen    = pStringObj->ulValueLen;

        for (i = 0; i < ulStrLen; i ++)
        {
            if (pValue[i] != uChar)
                break;
        }

        if (i != 0)
        {
            pos = i;
            for (; i < ulStrLen; i ++)
            {
                pValue[i - pos] = pValue[i];
            }

            ulStrLen    -= pos;

            pStringObj->ulValueLen  = ulStrLen;
        }
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscStringCOTrimLeftString
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      pTrimStr
            )

    description:

        This function is called to trim down the given string
        from the left of this string object.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                PUCHAR                      pTrimStr
                The string to be trimmed down.

    return:     void.

**********************************************************************/

void
AnscStringCOTrimLeftString
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pTrimStr
    )
{
    PANSC_STRING_OBJECT             pStringObj  = (PANSC_STRING_OBJECT)hThisObject;
    PUCHAR                          pValue;
    ULONG                           ulStrLen;
    ULONG                           TrimLen;
    PUCHAR                          pLeftStr;

    if (pStringObj && pTrimStr)
    {
        pValue      = pStringObj->pValue;
        ulStrLen    = pStringObj->ulValueLen;

        TrimLen     = AnscSizeOfString(pTrimStr);
        if (TrimLen > ulStrLen || TrimLen == 0)
            return;

        pLeftStr    = pValue;

        if (AnscEqualMemory(pLeftStr, pTrimStr, TrimLen))
        {
            ULONG                   i;

            for (i = TrimLen; i < ulStrLen; i ++)
            {
                pValue[i - TrimLen] = pValue[i];
            }

            pStringObj->ulValueLen          -= TrimLen;
            pValue[pStringObj->ulValueLen]  = 0;
        }
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscStringCOTrimRightChar
            (
                ANSC_HANDLE                 hThisObject,
                UCHAR                       uChar
            )

    description:

        This function is called to trim down the given character
        from right side of this string.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                UCHAR                       uChar
                Character to be trimmed down.

    return:     void.

**********************************************************************/

void
AnscStringCOTrimRightChar
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       uChar
    )
{
    PANSC_STRING_OBJECT             pStringObj  = (PANSC_STRING_OBJECT)hThisObject;

    if (pStringObj && pStringObj->ulValueLen != 0)
    {
        ULONG                       ulStrLen;
        PUCHAR                      pValue;
        LONG                        i;

        pValue      = pStringObj->pValue;
        ulStrLen    = pStringObj->ulValueLen;

        for (i = (LONG)ulStrLen - 1; i >= 0; i --)
        {
            if (pValue[i] != uChar)
                break;
        }

        if (i >= 0)
        {
            if (i != (LONG)ulStrLen - 1)
            {
                pStringObj->ulValueLen  = i + 1;
                pValue[i + 1]           = 0;
            }
        }
        else
        {
            pValue[0]               = 0;
            pStringObj->ulValueLen  = 0;
        }
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscStringCOSetString
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      pTrimStr
            )

    description:

        This function is called to trim down the given string from
        the right side of this string.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                PUCHAR                      pTrimStr
                The string to be trimmed down.

    return:     void.

**********************************************************************/

void
AnscStringCOTrimRightString
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pTrimStr
    )
{
    PANSC_STRING_OBJECT             pStringObj  = (PANSC_STRING_OBJECT)hThisObject;
    PUCHAR                          pValue;
    ULONG                           ulStrLen;
    ULONG                           TrimLen;
    PUCHAR                          pRightStr;

    if (pStringObj && pTrimStr)
    {
        pValue      = pStringObj->pValue;
        ulStrLen    = pStringObj->ulValueLen;

        TrimLen     = AnscSizeOfString(pTrimStr);
        if (TrimLen > ulStrLen || TrimLen == 0)
            return;

        pRightStr   = pValue + (ulStrLen - TrimLen);

        if (AnscEqualMemory(pRightStr, pTrimStr, TrimLen))
        {
            *pRightStr  = 0;
            pStringObj->ulValueLen  -= TrimLen;
        }
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscStringCOReplaceChar
            (
                ANSC_HANDLE                 hThisObject,
                UCHAR                       chOld,
                UCHAR                       chNew
            )

    description:

        This function is called to replace the character chOld with
        chNew.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                UCHAR                       chOld
                The replaced character.

                UCHAR                       chNew
                The character to replace with.

    return:     the number of characters replaced.

**********************************************************************/

ULONG
AnscStringCOReplaceChar
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       chOld,
        UCHAR                       chNew
    )
{
    PANSC_STRING_OBJECT             pStringObj  = (PANSC_STRING_OBJECT)hThisObject;
    PUCHAR                          pValue;
    ULONG                           ulValueLen, i, ulCount;

    ulValueLen  = pStringObj->GetLength((ANSC_HANDLE)pStringObj);
    if (ulValueLen == 0)
        return 0;

    pValue  = pStringObj->GetString((ANSC_HANDLE)pStringObj);
    ulCount = 0;

    for (i = 0; i < ulValueLen; i ++)
    {
        if (pValue[i] == chOld)
        {
            ulCount ++;
            pValue[i]   = chNew;
        }
    }

    return ulCount;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscStringCOReplaceString
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      pOldStr,
                PUCHAR                      pNewStr
            )

    description:

        This function is called to replace string pOldString with
        pNewStr.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                PUCHAR                      pOldStr
                The string to be replaced.

                PUCHAR                      pNewStr
                The string to replace with.

    return:     the number of replacement.

**********************************************************************/

ULONG
AnscStringCOReplaceString
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pOldStr,
        PUCHAR                      pNewStr
    )
{
    PANSC_STRING_OBJECT             pStringObj  = (PANSC_STRING_OBJECT)hThisObject;
    ULONG                           ulCount;
    PANSC_STRING_OBJECT             pNewString, pLeft, pRight;
    LONG                            pos;
    ULONG                           ulRight;
    ULONG                           ulOldStrLen;
    BOOL                            bRemoveString;

    if (!pOldStr)
        return 0;

    pNewString  = NULL;

    pNewStr     = NULL;
    ulCount     = 0;
    pos         = pStringObj->FindString((ANSC_HANDLE)pStringObj, pOldStr);

    ulOldStrLen     = AnscSizeOfString(pOldStr);
    bRemoveString   = FALSE;

    while (pos >= 0)
    {
        /* replace the string */
        ulCount ++;

        if (!pNewString)
        {
            pNewString  = (PANSC_STRING_OBJECT)AnscCreateStringComponent(NULL, NULL, NULL);

            if (!pNewString)
                return 0;
        }

        pLeft = pRight = NULL;

        if (pos != 0)
        {
            pLeft   = pStringObj->Left((ANSC_HANDLE)pStringObj, pos);
        }

        ulRight = pStringObj->GetLength((ANSC_HANDLE)pStringObj) - pos - ulOldStrLen;

        if (ulRight != 0)
        {
            pRight  = pStringObj->Right((ANSC_HANDLE)pStringObj, ulRight);
        }

        if (pLeft)
        {
            /* concat the left part */
            pNewString->Concat((ANSC_HANDLE)pNewString, (ANSC_HANDLE)pLeft);
            pLeft->Remove((ANSC_HANDLE)pLeft);
        }

        if (pNewStr)
        {
            /* add replace string */
            pNewString->AppendString((ANSC_HANDLE)pNewString, pNewStr);
        }

        if (bRemoveString)
        {
            pStringObj->Remove((ANSC_HANDLE)pStringObj);
        }

        pStringObj      = pRight;
        bRemoveString   = TRUE;
        
        pos = pStringObj->FindString((ANSC_HANDLE)pStringObj, pOldStr);
    }
    
    return ulCount;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscStringCOAppendBuffer
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      pBuffer,
                ULONG                       ulBufferLen
            )

    description:

        This function is called to append the given buffer to the
        string value of this object.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                PUCHAR                      pBuffer
                The buffer to be appended.

                ULONG                       ulBufferLen
                The buffer length.

    return:     void.

**********************************************************************/

void
AnscStringCOAppendBuffer
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pBuffer,
        ULONG                       ulBufferLen
    )
{
    PANSC_STRING_OBJECT             pStringObj  = (PANSC_STRING_OBJECT)hThisObject;
    ULONG                           ulNewValueLen, ulValueLen;
    PUCHAR                          pValue;

    if (!pBuffer || ulBufferLen == 0)
        return;

    ulValueLen      = pStringObj->GetLength((ANSC_HANDLE)pStringObj);
    ulNewValueLen   = ulBufferLen + ulValueLen + 1;

    pStringObj->Resize((ANSC_HANDLE)pStringObj, ulNewValueLen);

    if (ulNewValueLen <= pStringObj->ulValueBufferLen)
    {
        pValue  = pStringObj->pValue;
        AnscCopyMemory(pValue + ulValueLen, pBuffer, ulBufferLen);

        pValue[ulNewValueLen - 1]   = 0;

        pStringObj->ulValueLen  = ulNewValueLen - 1;
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscStringCOAppendString
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      pValue
            )

    description:

        This function is called to append the given string.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                PUCHAR                      pValue
                The string value.

    return:     void.

**********************************************************************/

void
AnscStringCOAppendString
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pValue
    )
{
    PANSC_STRING_OBJECT             pStringObj  = (PANSC_STRING_OBJECT)hThisObject;
    ULONG                           ulValueLen;

    if (pValue)
    {
        ulValueLen  = AnscSizeOfString(pValue);
        pStringObj->AppendBuffer((ANSC_HANDLE)pStringObj, pValue, ulValueLen);
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscStringCOConcat
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      pValue
            )

    description:

        This function is called to concatenate this string
        with given one.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                PUCHAR                      pValue
                The string value to be concatenated.

    return:     void.

**********************************************************************/

void
AnscStringCOConcat
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 String
    )
{
    PANSC_STRING_OBJECT             pStringObj  = (PANSC_STRING_OBJECT)hThisObject;
    PANSC_STRING_OBJECT             pStrConcat  = (PANSC_STRING_OBJECT)String;
    PUCHAR                          pValue;
    ULONG                           ulValueLen;

    if (!pStrConcat)
        return;

    pValue      = pStrConcat->GetString((ANSC_HANDLE)pStrConcat);
    ulValueLen  = pStrConcat->GetLength((ANSC_HANDLE)pStrConcat);

    pStringObj->AppendBuffer((ANSC_HANDLE)pStringObj, pValue, ulValueLen);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char *
        AnscStringCOExtractBuffer
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to extract string buffer.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

    return:     the string buffer.

**********************************************************************/

char *
AnscStringCOExtractBuffer
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_STRING_OBJECT             pStringObj  = (PANSC_STRING_OBJECT)hThisObject;
    char                            *pBuf;

    pBuf    = pStringObj->pValue;

    pStringObj->pValue              = NULL;
    pStringObj->ulValueLen          = 0;
    pStringObj->ulValueBufferLen    = 0;

    return pBuf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscStringCOAppendStringN
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      pValue,
                ULONG                       ulTimes
            )

    description:

        This function is called to append given string ulTimes times.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                PUCHAR                      pValue
                The string value.

                ULONG                       ulTimes
                Times to repeat.

    return:     void.

**********************************************************************/

void
AnscStringCOAppendStringN
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pValue,
        ULONG                       ulTimes
    )
{
    PANSC_STRING_OBJECT             pStringObj  = (PANSC_STRING_OBJECT)hThisObject;
    ULONG                           i;

    for (i = 0; i < ulTimes; i ++)
    {
        pStringObj->AppendString(hThisObject, pValue);
    }
}



