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

    module:	ansc_crypto_decompression.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the cryptography functions for the
        Crypto Object.

        *   AnscCryptoDecompress
        *   AnscCryptoOutDecompress
        *   AnscCryptoDeflateDecompress
        *   AnscCryptoLzsDecompress
        *   AnscCryptoV42bisDecompress
        *   AnscCryptoZlibDecompress
        *   AnscCryptoGzipDecompress

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Bin     Zhu

    ---------------------------------------------------------------

    revision:

        03/22/01    initial revision.
        08/03/05    zhubin added zlib and gzip support

**********************************************************************/


#include "ansc_crypto_global.h"
#include "zlib.h"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_STATUS
        AnscCryptoDecompress
            (
                ULONG                       algorithm,
                PVOID                       compact,
                ULONG                       size,
                PVOID                       plain,
                PULONG                      pOutSize
            );

    description:

        This function performs cryptography computation.

    argument:   ULONG                       algorithm
                Specifies the cryptography algorithm to use.

                PVOID                       compact
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                ULONG                       size
                Specifies the size of the data buffer.

                PVOID                       plain
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                PULONG                      pOutSize
                The buffer of output size;

    return:     the status of the operation.

**********************************************************************/

ANSC_STATUS
AnscCryptoDecompress
    (
        ULONG                       algorithm,
        PVOID                       compact,
        ULONG                       size,
        PVOID                       plain,
        PULONG                      pOutSize
    )
{
    ANSC_STATUS                     ulResult = 0;

    switch ( algorithm )
    {
        case    ANSC_CRYPTO_COMPRESSION_OUT :

                ulResult =
                    AnscCryptoOutDecompress
                        (
                            compact,
                            size,
                            plain,
                            pOutSize
                        );

                break;

        case    ANSC_CRYPTO_COMPRESSION_DEFLATE :

                ulResult =
                    AnscCryptoDeflateDecompress
                        (
                            compact,
                            size,
                            plain,
                            pOutSize
                        );

                break;

                /* remove LZS related APIs, two reasons:
                 * LZS is NOT free software
                 * LzsCompress/Decompress, as a IPSec compression option, is not used by vendors
                 */

#ifdef _ANSC_LZS_USED_
        case    ANSC_CRYPTO_COMPRESSION_LZS :

                ulResult =
                    AnscCryptoLzsDecompress
                        (
                            compact,
                            size,
                            plain,
                            pOutSize
                        );

                break;
#endif
        case    ANSC_CRYPTO_COMPRESSION_V42BIS :

                ulResult =
                    AnscCryptoV42bisDecompress
                        (
                            compact,
                            size,
                            plain,
                            pOutSize
                        );

                break;

        case    ANSC_CRYPTO_COMPRESSION_ZLIB :

                ulResult =
                    AnscCryptoZlibDecompress
                        (
                            compact,
                            size,
                            plain,
                            pOutSize
                        );

                break;

        case    ANSC_CRYPTO_COMPRESSION_GZIP :

                ulResult =
                    AnscCryptoGzipDecompress
                        (
                            compact,
                            size,
                            plain,
                            pOutSize
                        );

                break;

        default :

                ulResult = 0;

                break;
    }

    return  ulResult;
}


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_STATUS
        AnscCryptoOutDecompress
            (
                PVOID                       compact,
                ULONG                       size,
                PVOID                       plain,
                PULONG                      pOutSize
            );

    description:

        This function performs cryptography computation.

    argument:   PVOID                       compact
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                ULONG                       size
                Specifies the size of the data buffer.

                PVOID                       plain
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                PULONG                      pOutSize
                The buffer of output size;

    return:     the status of the operation.

**********************************************************************/

ANSC_STATUS
AnscCryptoOutDecompress
    (
        PVOID                       compact,
        ULONG                       size,
        PVOID                       plain,
        PULONG                      pOutSize
    )
{
    UNREFERENCED_PARAMETER(compact);
    UNREFERENCED_PARAMETER(plain);
    UNREFERENCED_PARAMETER(pOutSize);
    return  size;
}


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_STATUS
        AnscCryptoDeflateDecompress
            (
                PVOID                       compact,
                ULONG                       size,
                PVOID                       plain,
                PULONG                      pOutSize
            );

    description:

        This function performs cryptography computation.

    argument:   PVOID                       compact
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                ULONG                       size
                Specifies the size of the data buffer.

                PVOID                       plain
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                PULONG                      pOutSize
                The buffer of output size;

    return:     the status of the operation.

**********************************************************************/

ANSC_STATUS
AnscCryptoDeflateDecompress
    (
        PVOID                       compact,
        ULONG                       size,
        PVOID                       plain,
        PULONG                      pOutSize
    )
{
    UNREFERENCED_PARAMETER(compact);
    UNREFERENCED_PARAMETER(plain);
    UNREFERENCED_PARAMETER(pOutSize);
    return  size;
}

/* remove LZS related APIs, two reasons:
 * LZS is NOT free software
 * LzsCompress/Decompress, as a IPSec compression option, is not used by vendors
 */

# ifdef _ANSC_LZS_USED_

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_STATUS
        AnscCryptoLzsDecompress
            (
                PVOID                       compact,
                ULONG                       size,
                PVOID                       plain,
                PULONG                      pOutSize
            );

    description:

        This function performs cryptography computation.

    argument:   PVOID                       compact
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                ULONG                       size
                Specifies the size of the data buffer.

                PVOID                       plain
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                PULONG                      pOutSize
                The buffer of output size;

    return:     the status of the operation.

**********************************************************************/

ANSC_STATUS
AnscCryptoLzsDecompress
    (
        PVOID                       compact,
        ULONG                       size,
        PVOID                       plain,
        PULONG                      pOutSize
    )
{
    PVOID                           pDecompressedBuffer      = NULL;
    PVOID                           pDecompressedData        = NULL;
    ULONG                           ulDecompressedBufferSize = 0;
    ULONG                           ulDecompressedDataSize   = 0;
    PVOID                           pScratchRam              = NULL;

    /*
     * dynamically allocate memory for holding the decompressed data
     * note: I don't know if the implementation of LZS I have can deal with the situation that the input and output
     * buffer are the same address, for safe, we allocate intermediate memory for holding the decompressed data
     */
    ulDecompressedBufferSize = ANSC_MAX_RAW_BUFFER_SIZE + 256;
    ulDecompressedDataSize   = ulDecompressedBufferSize;
    pDecompressedBuffer      = AnscAllocateMemory(ulDecompressedBufferSize);
    pDecompressedData        = pDecompressedBuffer;

    /*
     * dynamically allocate memory for holding the LZS history data
     */
    pScratchRam = AnscAllocateMemory(LZS_HISTORY_SIZE);

    /*
     * perform LZS decompression
     */
    LZS_InitHistory(pScratchRam);

    LZS_Decompress
        (
            (unsigned char**)&compact,
            (unsigned char**)&pDecompressedBuffer,
            &size,
            &ulDecompressedDataSize,
            pScratchRam,
            LZS_RESET
        );

    ulDecompressedDataSize = ulDecompressedBufferSize - ulDecompressedDataSize;

    /*
     * copy the decompressed data into the output buffer provided by the COMP engine
     */
    AnscCopyMemory
        (
            plain,
            pDecompressedData,
            ulDecompressedDataSize
        );

    /*
     * don't forget to flush the allocated intermediate memory buffer
     */
    AnscFreeMemory(pDecompressedData);
    AnscFreeMemory(pScratchRam);

    /*
     * return the size of decompressed result to caller
     */
    *pOutSize = ulDecompressedDataSize;

    return ANSC_STATUS_SUCCESS;
}

#endif

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_STATUS
        AnscCryptoV42bisDecompress
            (
                PVOID                       compact,
                ULONG                       size,
                PVOID                       plain,
                PULONG                      pOutSize
            );

    description:

        This function performs cryptography computation.

    argument:   PVOID                       compact
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                ULONG                       size
                Specifies the size of the data buffer.

                PVOID                       plain
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                PULONG                      pOutSize
                The buffer of output size;

    return:     the status of the operation.

**********************************************************************/

ANSC_STATUS
AnscCryptoV42bisDecompress
    (
        PVOID                       compact,
        ULONG                       size,
        PVOID                       plain,
        PULONG                      pOutSize
    )
{
    UNREFERENCED_PARAMETER(compact);
    UNREFERENCED_PARAMETER(plain);
    UNREFERENCED_PARAMETER(pOutSize);
    return  size;
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_STATUS
        AnscCryptoZlibDecompress
            (
                PVOID                       compact,
                ULONG                       size,
                PVOID                       plain,
                PULONG                      pOutSize
            );

    description:

        This function performs cryptography computation.

    argument:   PVOID                       compact
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                ULONG                       size
                Specifies the size of the data buffer.

                PVOID                       plain
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                PULONG                      pOutSize
                The buffer of output size;

    return:     the status of the operation.

**********************************************************************/

ANSC_STATUS
AnscCryptoZlibDecompress
    (
        PVOID                       compact,
        ULONG                       size,
        PVOID                       plain,
        PULONG                      pOutSize
    )
{
#ifndef _NO_ANSC_ZLIB_

    LONG                            errCode;

    if( plain == NULL || compact == NULL || size == 0 || pOutSize == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    errCode = uncompress(plain, pOutSize, compact, size);

    if( errCode != 0)
    {
        return ANSC_STATUS_FAILURE;
    }

#else
    UNREFERENCED_PARAMETER(compact);
    UNREFERENCED_PARAMETER(size);
    UNREFERENCED_PARAMETER(plain);
    UNREFERENCED_PARAMETER(pOutSize);
#endif
    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_STATUS
        AnscCryptoGzipDecompress
            (
                PVOID                       compact,
                ULONG                       size,
                PVOID                       plain,
                PULONG                      pOutSize
            );

    description:

        This function performs cryptography computation.

    argument:   PVOID                       compact
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                ULONG                       size
                Specifies the size of the data buffer.

                PVOID                       plain
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                PULONG                      pOutSize
                The buffer of output size;

    return:     the status of the operation.

**********************************************************************/

ANSC_STATUS
AnscCryptoGzipDecompress
    (
        PVOID                       compact,
        ULONG                       size,
        PVOID                       plain,
        PULONG                      pOutSize
    )
{
#ifdef _ANSC_GZIP_USED_

    ULONG                           plainLen   = 0;

    if( compact == NULL || size <= 14 || pOutSize == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    if( gz_isvalid(compact,size) != 0)
    {
        AnscTrace("It's an invalid gzip content\n");

        return ANSC_STATUS_FAILURE;
    }

    plainLen = gz_getplainlength(compact,size);

    if( *pOutSize < plainLen)
    {
        *pOutSize = plainLen;

        return ANSC_STATUS_RESOURCES;
    }

    *pOutSize = plainLen;

    if( plain == NULL)
    {
        return ANSC_STATUS_SUCCESS;
    }

    plainLen= gz_al_decompress(compact,size, plain, *pOutSize);

    if( plainLen == 0)
    {
        return ANSC_STATUS_FAILURE;
    }

    *pOutSize = plainLen;

#else
    UNREFERENCED_PARAMETER(compact);
    UNREFERENCED_PARAMETER(size);
    UNREFERENCED_PARAMETER(plain);
    UNREFERENCED_PARAMETER(pOutSize);

    AnscTrace("WARNING: GZIP is disabled!!!\n");
#endif


    return ANSC_STATUS_SUCCESS;
}
