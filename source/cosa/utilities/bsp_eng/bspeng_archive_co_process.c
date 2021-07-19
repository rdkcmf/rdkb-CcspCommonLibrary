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

    module:	bspeng_archive_co_process.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This file implements serialization of lexical expression
        table.

        *   BspTemplateArchiveWriteInteger
        *   BspTemplateArchiveWriteUlong
        *   BspTemplateArchiveWriteDouble
        *   BspTemplateArchiveWriteWord
        *   BspTemplateArchiveWriteBoolean
        *   BspTemplateArchiveWriteByte
        *   BspTemplateArchiveWriteString
        *   BspTemplateArchiveWriteBinary
        *   BspTemplateArchiveReadInteger
        *   BspTemplateArchiveReadUlong
        *   BspTemplateArchiveReadDouble
        *   BspTemplateArchiveReadWord
        *   BspTemplateArchiveReadBoolean
        *   BspTemplateArchiveReadByte
        *   BspTemplateArchiveReadString
        *   BspTemplateArchiveReadBinary
        *   BspTemplateArchiveEnlargeStorage
        *   BspTemplateArchiveBigEnough
        *   BspTemplateArchiveGetRemainSize
        *   BspTemplateArchiveSetStorage
        *   BspTemplateArchiveReleaseStorage
        *   BspTemplateArchivePrepareLoading
        *   BspTemplateArchiveSave2File
        
    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        11/18/01    initial revision.

    ---------------------------------------------------------------

    last modified:

**********************************************************************/

#include "bspeng_co_global.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateArchiveWriteInteger
            (
                ANSC_HANDLE                 hThisObject,
                int                         Value
            )

    description:

        This function is to write an integer into archive object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                int                         Value

    return:     status of operation.

**********************************************************************/

BOOL
BspTemplateArchiveWriteInteger
    (
        ANSC_HANDLE                 hThisObject,
        int                         Value
    )
{
    PBSP_TEMPLATE_ARCHIVE_OBJECT    pArchive    = (PBSP_TEMPLATE_ARCHIVE_OBJECT)hThisObject;
    UCHAR                           Bytes[4];

    Bytes[0] = Value >> 24;
    Bytes[1] = (Value >> 16) & 0x000000FF;
    Bytes[2] = (Value >> 8) & 0x000000FF;
    Bytes[3] = Value & 0x000000FF;

    return pArchive->WriteBinary(hThisObject, Bytes, sizeof(Bytes)/sizeof(Bytes[0]));
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateArchiveWriteUlong
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       Value
            )

    description:

        This function is to write an ULONG into archive object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       Value

    return:     status of operation.

**********************************************************************/

BOOL
BspTemplateArchiveWriteUlong
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       Value
    )
{
    PBSP_TEMPLATE_ARCHIVE_OBJECT    pArchive    = (PBSP_TEMPLATE_ARCHIVE_OBJECT)hThisObject;

    return pArchive->WriteInteger(hThisObject, Value);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateArchiveWriteDouble
            (
                ANSC_HANDLE                 hThisObject,
                int                         Value
            )

    description:

        This function is to write a double into archive object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                BSP_TEMPLATE_DOUBLE         Value
                (DOUBLE                     Value)

    return:     status of operation.

**********************************************************************/

BOOL
BspTemplateArchiveWriteDouble
    (
        ANSC_HANDLE                 hThisObject,
#ifdef   _BSPENG_NO_DOUBLE
        BSP_TEMPLATE_DOUBLE         Value
#else
        DOUBLE                      Value
#endif
    )
{
    PBSP_TEMPLATE_ARCHIVE_OBJECT    pArchive    = (PBSP_TEMPLATE_ARCHIVE_OBJECT)hThisObject;
    char                            buf[32];

#ifdef   _BSPENG_NO_DOUBLE
    BSP_TEMPLATE_DOUBLE_TO_STRING(buf, (int)Value);
#else
    errno_t                         rc          = -1;
    rc = sprintf_s(buf, sizeof(buf), "%f", Value);
    if(rc < EOK)
    {
       ERR_CHK(rc);
    }
#endif

    return pArchive->WriteString(hThisObject, (PUCHAR)buf);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateArchiveWriteWord
            (
                ANSC_HANDLE                 hThisObject,
                int                         Value
            )

    description:

        This function is to write a word into archive object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                int                         Value

    return:     status of operation.

**********************************************************************/

BOOL
BspTemplateArchiveWriteWord
    (
        ANSC_HANDLE                 hThisObject,
        int                         Value
    )
{
    PBSP_TEMPLATE_ARCHIVE_OBJECT    pArchive    = (PBSP_TEMPLATE_ARCHIVE_OBJECT)hThisObject;
    UCHAR                           Bytes[2];

    Bytes[0] = (Value >> 8) & 0x00FF;
    Bytes[1] = Value & 0x00FF;

    return pArchive->WriteBinary(hThisObject, Bytes, sizeof(Bytes)/sizeof(Bytes[0]));
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateArchiveWriteBoolean
            (
                ANSC_HANDLE                 hThisObject,
                BOOL                        Value
            )

    description:

        This function is to write a Boolean into archive object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                BOOL                        Value

    return:     status of operation.

**********************************************************************/

BOOL
BspTemplateArchiveWriteBoolean
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        Value
    )
{
    PBSP_TEMPLATE_ARCHIVE_OBJECT    pArchive    = (PBSP_TEMPLATE_ARCHIVE_OBJECT)hThisObject;

    pArchive->WriteByte(hThisObject, (UCHAR)((Value)?1:0));

    return TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateArchiveWriteByte
            (
                ANSC_HANDLE                 hThisObject,
                UCHAR                       Value
            )

    description:

        This function is to write a UCHAR into archive object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                UCHAR                       Value

    return:     status of operation.

**********************************************************************/

BOOL
BspTemplateArchiveWriteByte
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       Value
    )
{
    PBSP_TEMPLATE_ARCHIVE_OBJECT    pArchive    = (PBSP_TEMPLATE_ARCHIVE_OBJECT)hThisObject;

    if (!pArchive->BigEnough(hThisObject, 1))
    {
        pArchive->EnlargeStorage(hThisObject, 0);
    }

    pArchive->pStorage[pArchive->CurPos ++] = Value;

    pArchive->ContentLen ++;

    return TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateArchiveWriteString
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      Value
            )

    description:

        This function is to write a string into archive object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      Value

    return:     status of operation.

**********************************************************************/

BOOL
BspTemplateArchiveWriteString
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      Value
    )
{
    PBSP_TEMPLATE_ARCHIVE_OBJECT    pArchive    = (PBSP_TEMPLATE_ARCHIVE_OBJECT)hThisObject;
    int                             nStrLen;
    BOOL                            bSucc;

    nStrLen = AnscSizeOfString((const char *)Value);

    /* write the length of the string first */
    bSucc = pArchive->WriteWord(hThisObject, nStrLen);

    if (!bSucc)
        return FALSE;
    else
        return pArchive->WriteBinary(hThisObject, Value, nStrLen);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateArchiveWriteBinary
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      pStream,
                ULONG                       StreamLen
            )

    description:

        This function is to write given binary stream
        into archive object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      pStream

                ULONG                       StreamLen
                The length of stream in bytes.

    return:     status of operation.

**********************************************************************/

BOOL
BspTemplateArchiveWriteBinary
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pStream,
        ULONG                       StreamLen
    )
{
    PBSP_TEMPLATE_ARCHIVE_OBJECT    pArchive    = (PBSP_TEMPLATE_ARCHIVE_OBJECT)hThisObject;
    ULONG                           i;

    if (!pArchive->BigEnough(hThisObject, StreamLen))
    {
        pArchive->EnlargeStorage(hThisObject, StreamLen);
    }

    for (i = 0; i < StreamLen; i ++)
    {
        pArchive->pStorage[pArchive->CurPos ++] = pStream[i];
    }

    pArchive->ContentLen += StreamLen;

    return TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateArchiveReadInteger
            (
                ANSC_HANDLE                 hThisObject,
                int                         *pValue
            )

    description:

        This function is to read an integer from archive object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                int                         *pValue

    return:     TRUE if operation succeeds, FALSE otherwise.

**********************************************************************/

BOOL
BspTemplateArchiveReadInteger
    (
        ANSC_HANDLE                 hThisObject,
        int                         *pValue
    )
{
    PBSP_TEMPLATE_ARCHIVE_OBJECT    pArchive    = (PBSP_TEMPLATE_ARCHIVE_OBJECT)hThisObject;
    int                             i, Val = 0;

    if (pArchive->GetRemainSize(hThisObject) < sizeof(int))
        return FALSE;

    for (i = 0; i < (int)sizeof(int); i ++)
    {
        Val <<= 8;
        Val += pArchive->pStorage[pArchive->CurPos ++];
    }

    *pValue = Val;
    return TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateArchiveReadInteger
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       *pValue
            )

    description:

        This function is to read a ULONG from archive object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       *pValue

    return:     TRUE if operation succeeds, FALSE otherwise.

**********************************************************************/

BOOL
BspTemplateArchiveReadUlong
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       *pValue
    )
{
    PBSP_TEMPLATE_ARCHIVE_OBJECT    pArchive    = (PBSP_TEMPLATE_ARCHIVE_OBJECT)hThisObject;

    return pArchive->ReadInteger(hThisObject, (int *)pValue);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateArchiveReadDouble
            (
                ANSC_HANDLE                 hThisObject,
                DOUBLE                      *pValue
            )

    description:

        This function is to read an integer from archive object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                BSP_TEMPLATE_DOUBLE         *pValue
                DOUBLE                      *pValue

    return:     TRUE if operation succeeds, FALSE otherwise.

**********************************************************************/

BOOL
BspTemplateArchiveReadDouble
    (
        ANSC_HANDLE                 hThisObject,
#ifdef   _BSPENG_NO_DOUBLE
        BSP_TEMPLATE_DOUBLE         *pValue
#else
        DOUBLE                      *pValue
#endif
    )
{
    PBSP_TEMPLATE_ARCHIVE_OBJECT    pArchive    = (PBSP_TEMPLATE_ARCHIVE_OBJECT)hThisObject;
    UCHAR                           buf[32];
    BOOL                            bSucc;

    bSucc = pArchive->ReadInteger(hThisObject, (int *)buf);

    if (bSucc)
    {
#ifdef   _BSPENG_NO_DOUBLE
        char                        *pEnd = NULL;
        BOOL                        bHasFrac = FALSE;
        BOOL                        *pHasFrac    = &bHasFrac;
       char                         **ppEnd = &pEnd;
        STRING_TO_BSP_TEMPLATE_DOUBLE(buf, pValue, ppEnd, pHasFrac);
#else
        *pValue = (DOUBLE)_ansc_atof(buf);
#endif
    }

    return bSucc;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateArchiveReadWord
            (
                ANSC_HANDLE                 hThisObject,
                int                         *pValue
            )

    description:

        This function is to read a word from archive object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                int                         *pValue

    return:     TRUE if operation succeeds, FALSE otherwise.

**********************************************************************/

BOOL
BspTemplateArchiveReadWord
    (
        ANSC_HANDLE                 hThisObject,
        int                         *pValue
    )
{
    PBSP_TEMPLATE_ARCHIVE_OBJECT    pArchive    = (PBSP_TEMPLATE_ARCHIVE_OBJECT)hThisObject;
    int                             i, Val = 0;

    if (pArchive->GetRemainSize(hThisObject) < 2)
        return FALSE;

    for (i = 0; i < 2; i ++)
    {
        Val <<= 8;
        Val += pArchive->pStorage[pArchive->CurPos ++];
    }

    *pValue = Val;
    return TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateArchiveReadBoolean
            (
                ANSC_HANDLE                 hThisObject,
                BOOL                        *pValue
            )

    description:

        This function is to read a Boolean from archive object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                BOOL                        *pValue

    return:     TRUE if operation succeeds, FALSE otherwise.

**********************************************************************/

BOOL
BspTemplateArchiveReadBoolean
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        *pValue
    )
{
    PBSP_TEMPLATE_ARCHIVE_OBJECT    pArchive    = (PBSP_TEMPLATE_ARCHIVE_OBJECT)hThisObject;
    BOOL                            Val;

    if (pArchive->GetRemainSize(hThisObject) < 1)
        return FALSE;

    Val = (BOOL)pArchive->pStorage[pArchive->CurPos ++];

    *pValue = Val;
    return TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateArchiveReadByte
            (
                ANSC_HANDLE                 hThisObject,
                UCHAR                       *pValue
            )

    description:

        This function is to read a UCHAR from archive object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                UCHAR                       *pValue

    return:     TRUE if operation succeeds, FALSE otherwise.

**********************************************************************/

BOOL
BspTemplateArchiveReadByte
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       *pValue
    )

{
    PBSP_TEMPLATE_ARCHIVE_OBJECT    pArchive    = (PBSP_TEMPLATE_ARCHIVE_OBJECT)hThisObject;

    return pArchive->ReadBinary(hThisObject, pValue, 1);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateArchiveReadString
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      *pValue
            )

    description:

        This function is to read a string from archive object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      *pValue

    return:     TRUE if operation succeeds, FALSE otherwise.

**********************************************************************/

BOOL
BspTemplateArchiveReadString
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      *pValue
    )
{
    PBSP_TEMPLATE_ARCHIVE_OBJECT    pArchive    = (PBSP_TEMPLATE_ARCHIVE_OBJECT)hThisObject;
    int                             nStrLen;
    BOOL                            bSucc;
    PUCHAR                          pString = (PUCHAR)NULL; /*RDKB-5933 , CID-24748, Initializing pointer*/

    bSucc = pArchive->ReadWord(hThisObject, &nStrLen);

    if (bSucc && nStrLen >= 0)
    {
        pString = (PUCHAR)AnscAllocateMemory((nStrLen+1) * sizeof(UCHAR));

        if (!pString)
        {
            return FALSE;
        }

        bSucc = pArchive->ReadBinary(hThisObject, (PUCHAR)pString, (ULONG)nStrLen);

        /* terminate the string */
        pString[nStrLen] = 0;

        if (!bSucc && pString)
        {
            AnscFreeMemory(pString);
            pString = NULL;
        }
    }

    *pValue = pString;

    return bSucc;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateArchiveReadBinary
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      pStream,
                ULONG                       StreamLen
            )

    description:

        This function is to read a binary stream from archive object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      pStream

                ULONG                       StreamLen
                The length of stream to read.

    return:     TRUE if operation succeeds, FALSE otherwise.

**********************************************************************/

BOOL
BspTemplateArchiveReadBinary
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pStream,
        ULONG                       StreamLen
    )
{
    PBSP_TEMPLATE_ARCHIVE_OBJECT    pArchive    = (PBSP_TEMPLATE_ARCHIVE_OBJECT)hThisObject;
    ULONG                           i;

    if (pArchive->GetRemainSize(hThisObject) < StreamLen)
        return FALSE;

    for (i = 0; i < StreamLen; i ++)
    {
        pStream[i] = pArchive->pStorage[pArchive->CurPos ++];
    }

    return TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateArchiveEnlargeStorage
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulIncAtLeast
            )

    description:

        This function is to englarge archive buffer by given amount.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulIncAtLeast
                The minimum size to be englarged.

    return:     void.

**********************************************************************/

void
BspTemplateArchiveEnlargeStorage
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIncAtLeast
    )
{
    PBSP_TEMPLATE_ARCHIVE_OBJECT    pArchive    = (PBSP_TEMPLATE_ARCHIVE_OBJECT)hThisObject;
    PUCHAR                          pNewStorage;
    ULONG                           ulNewSize;

    if (ulIncAtLeast > (ULONG)BSP_TEMPLATE_ARCHIVE_OBJECT_STORAGE_GROW)
    {
        ulNewSize   = ulIncAtLeast / BSP_TEMPLATE_ARCHIVE_OBJECT_STORAGE_GROW;
        ulNewSize   *= BSP_TEMPLATE_ARCHIVE_OBJECT_STORAGE_GROW;

        if (ulIncAtLeast % BSP_TEMPLATE_ARCHIVE_OBJECT_STORAGE_GROW)
        {
            ulNewSize   += BSP_TEMPLATE_ARCHIVE_OBJECT_STORAGE_GROW;
        }
    }
    else
    {
        ulNewSize   = BSP_TEMPLATE_ARCHIVE_OBJECT_STORAGE_GROW;
    }

    ulNewSize   += pArchive->StorageLen;

/*
    pNewStorage = AnscReAllocMemory
                    (
                        pArchive->pStorage, 
                        ulNewSize
                    );
*/

    pNewStorage = 
        AnscMemUtilRealloc
            (
                pArchive->pStorage, 
                pArchive->StorageLen, 
                ulNewSize
            );

    if (pNewStorage)
    {
        pArchive->pStorage      = pNewStorage;
        pArchive->StorageLen    = ulNewSize;
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateArchiveBigEnough
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       Size
            )

    description:

        This function is to check if archive buffer is big enough
        to hold given bytes.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       Size
                The expected size.

    return:     void.

**********************************************************************/

BOOL
BspTemplateArchiveBigEnough
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       Size
    )
{
    PBSP_TEMPLATE_ARCHIVE_OBJECT    pArchive    = (PBSP_TEMPLATE_ARCHIVE_OBJECT)hThisObject;

    return (pArchive->CurPos + Size <= pArchive->StorageLen);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateArchiveGetRemainSize
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is to get the remaining buffer size.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     remaining size of archive buffer.

**********************************************************************/

ULONG
BspTemplateArchiveGetRemainSize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_ARCHIVE_OBJECT    pArchive    = (PBSP_TEMPLATE_ARCHIVE_OBJECT)hThisObject;

    return (pArchive->ContentLen - pArchive->CurPos);
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateArchiveSetStorage
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      pStorage,
                ULONG                       StorageSize,
                BOOL                        NeedRelease
            )

    description:

        This function is to set archive storage.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      pStorage
                Archive buffer.

                ULONG                       StorageSize
                Archive buffer size.

                BOOL                        NeedRelease
                Indication if the buffer should be released afterwards.

    return:     void.

**********************************************************************/

void
BspTemplateArchiveSetStorage
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pStorage,
        ULONG                       StorageSize,
        BOOL                        NeedRelease     /* indicating whether this buffer needs to release */
    )
{
    PBSP_TEMPLATE_ARCHIVE_OBJECT    pArchive    = (PBSP_TEMPLATE_ARCHIVE_OBJECT)hThisObject;

    pArchive->ReleaseStorage(hThisObject);

    pArchive->pStorage           = pStorage;
    pArchive->StorageLen         = StorageSize;
    pArchive->ContentLen         = StorageSize;
    pArchive->NeedRelease        = NeedRelease;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateArchiveReleaseStorage
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is to release archive storage.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     void.

**********************************************************************/

void
BspTemplateArchiveReleaseStorage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_ARCHIVE_OBJECT    pArchive    = (PBSP_TEMPLATE_ARCHIVE_OBJECT)hThisObject;

    if (pArchive->pStorage && pArchive->NeedRelease)
    {
        AnscFreeMemory(pArchive->pStorage);
    }

    pArchive->pStorage           = NULL;
    pArchive->CurPos             = 0;
    pArchive->StorageLen         = 0;
    pArchive->NeedRelease        = FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateArchivePrepareLoading
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is to prepare loading templates from
        archive buffer.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     void.

**********************************************************************/

void
BspTemplateArchivePrepareLoading
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_ARCHIVE_OBJECT    pArchive    = (PBSP_TEMPLATE_ARCHIVE_OBJECT)hThisObject;

    pArchive->CurPos = 0;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateArchiveSaveToFile
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      pFileName,
                ULONG                       ulMode
            )

    description:

        This function is to read an integer from archive object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      pFileName
                Output file name.

                ULONG                       ulMode
                Indicates how templates will be saved.
                0 --- binary
                1 --- comma delimited

    return:     status of operation.

**********************************************************************/

BOOL
BspTemplateArchiveSaveToFile
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pFileName,
        ULONG                       ulMode
    )
{
#ifdef   _BSP_TEMPLATE_ENGINE_USING_EXTERNAL_FILE
    PBSP_TEMPLATE_ARCHIVE_OBJECT    pArchive    = (PBSP_TEMPLATE_ARCHIVE_OBJECT)hThisObject;
    ANSC_HANDLE                     hFile;
    ULONG                           ulFileMode, ulFileType;

    ulFileMode  = ANSC_FILE_MODE_CREATE | ANSC_FILE_MODE_WRITE | ANSC_FILE_MODE_TRUNC;
    ulFileType  = ANSC_FILE_TYPE_WRITE;

    hFile       = AnscOpenFile((char *)pFileName, ulFileMode, ulFileType);

    if (hFile)
    {
        ULONG                       total = 0;
        ULONG                       block = 1024;
        ULONG                       bytesWritten;

        if (ulMode == BSP_TEMPLATE_ARCHIVE_BINARY)
        {
            while (total <= pArchive->ContentLen)
            {
                if (block > pArchive->ContentLen - total)
                    block = pArchive->ContentLen - total;
                else
                    block = 1024;

                bytesWritten = block;
                AnscWriteFile(hFile, (PVOID)(pArchive->pStorage + total), &bytesWritten);

                if (bytesWritten == 0)
                    break;

                total += bytesWritten;
            }
        }
        else
            if (ulMode == BSP_TEMPLATE_ARCHIVE_COMMA_SEPARATED)
            {
                char                buf[1024];
                int                 size;
                ULONG               ulSize;

                while (total <= pArchive->ContentLen)
                {
                    size = 
                        sprintf_s
                            (
                                buf, sizeof(buf), "%d,%s", 
                                pArchive->pStorage[total], 
                                (total % 40 == 0 && total != 0)?"\n":""
                            );
                    if(size < 0)
                    {
                        ERR_CHK(size);
                    }
		    total++;
                    ulSize  = size;
                    AnscWriteFile(hFile, buf, &ulSize);
                }
            }
            else
            {
                /* Unsupported mode */
            }

        AnscCloseFile(hFile);
    }
#endif

    return TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        PUCHAR
        BspTemplateArchiveExtractContent
            (
                ANSC_HANDLE                 hThisObject,
                PULONG                      pulContentLen,
                PBOOL                       pbNeedRelease
           )

    description:

        This function is to extract the content of archived 
        templates.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PULONG                      pulContentLen
                The content length.

                PBOOL                       pbNeedRelease
                Indication if the buffer should be released.

    return:     buffer containing the content.

**********************************************************************/

PUCHAR
BspTemplateArchiveExtractContent
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pulContentLen,
        PBOOL                       pbNeedRelease
   )
{
    PBSP_TEMPLATE_ARCHIVE_OBJECT    pArchive    = (PBSP_TEMPLATE_ARCHIVE_OBJECT)hThisObject;

    *pulContentLen  = pArchive->ContentLen;
    *pbNeedRelease  = pArchive->NeedRelease;

    /* 
     * Set "NeedRelease" flag to FALSE. The caller to this function
     * should be responsible for releasing resource that might need
     * to be released. Check the return value of "pbNeedRelease". If
     * the this value is "TRUE", you need to free resource, otherwise,
     * you don't.
     */
    pArchive->NeedRelease   = FALSE;

    return pArchive->pStorage;
}


