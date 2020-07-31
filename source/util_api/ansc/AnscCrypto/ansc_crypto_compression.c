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

    module:	ansc_crypto_compression.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the cryptography functions for the
        Crypto Object.

        *   AnscCryptoCompress
        *   AnscCryptoOutCompress
        *   AnscCryptoDeflateCompress
        *   AnscCryptoLzsCompress
        *   AnscCryptoV42bisCompress
        *   AnscCryptoZlibCompress
        *   AnscCryptoGzipCompress

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
        08/03/05    zhubin added zlib and zip support

**********************************************************************/


#include "ansc_crypto_global.h"
#include "zlib.h"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_STATUS
        AnscCryptoCompress
            (
                ULONG                       algorithm,
                PVOID                       plain,
                ULONG                       size,
                PVOID                       compact,
                PULONG                      pOutSize,
                ULONG                       mode,
                ULONG                       flag
            );

    description:

        This function performs cryptography computation.

    argument:   ULONG                       algorithm
                Specifies the cryptography algorithm to use.

                PVOID                       plain
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                ULONG                       size
                Specifies the size of the data buffer.

                PVOID                       compact
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                PULONG                      pOutSize,
                The buffer of output size;

                ULONG                       mode
                Specifies the cryptography parameter to be used.

                ULONG                       flag
                Specifies the cryptography parameter to be used.

    return:     the status of the operation

**********************************************************************/

ANSC_STATUS
AnscCryptoCompress
    (
        ULONG                       algorithm,
        PVOID                       plain,
        ULONG                       size,
        PVOID                       compact,
        PULONG                      pOutSize,
        ULONG                       mode,
        ULONG                       flag
    )
{
    ANSC_STATUS                     ulResult = 0;

    switch ( algorithm )
    {
        case    ANSC_CRYPTO_COMPRESSION_OUT :

                ulResult =
                    AnscCryptoOutCompress
                        (
                            plain,
                            size,
                            compact,
                            pOutSize,
                            mode,
                            flag
                        );

                break;

        case    ANSC_CRYPTO_COMPRESSION_DEFLATE :

                ulResult =
                    AnscCryptoDeflateCompress
                        (
                            plain,
                            size,
                            compact,
                            pOutSize,
                            mode,
                            flag
                        );

                break;

                /* remove LZS related APIs, two reasons:
                 * LZS is NOT free software
                 * LzsCompress/Decompress, as a IPSec compression option, is not used by vendors
                 */

#ifdef _ANSC_LZS_USED_
        case    ANSC_CRYPTO_COMPRESSION_LZS :

                ulResult =
                    AnscCryptoLzsCompress
                        (
                            plain,
                            size,
                            compact,
                            pOutSize,
                            mode,
                            flag
                        );

                break;
#endif
        case    ANSC_CRYPTO_COMPRESSION_V42BIS :

                ulResult =
                    AnscCryptoV42bisCompress
                        (
                            plain,
                            size,
                            compact,
                            pOutSize,
                            mode,
                            flag
                        );

                break;

        case    ANSC_CRYPTO_COMPRESSION_ZLIB :

                ulResult =
                    AnscCryptoZlibCompress
                        (
                            plain,
                            size,
                            compact,
                            pOutSize
                        );

                break;

        case    ANSC_CRYPTO_COMPRESSION_GZIP :

                ulResult =
                    AnscCryptoGzipCompress
                        (
                            plain,
                            size,
                            compact,
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
        AnscCryptoOutCompress
            (
                PVOID                       plain,
                ULONG                       size,
                PVOID                       compact,
                PULONG                      pOutSize,
                ULONG                       mode,
                ULONG                       flag
            );

    description:

        This function performs cryptography computation.

    argument:   PVOID                       plain
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                ULONG                       size
                Specifies the size of the data buffer.

                PVOID                       compact
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                PULONG                      pOutSize,
                The buffer of output size;

                ULONG                       mode
                Specifies the cryptography parameter to be used.

                ULONG                       flag
                Specifies the cryptography parameter to be used.

    return:     the status of the operation

**********************************************************************/

ANSC_STATUS
AnscCryptoOutCompress
    (
        PVOID                       plain,
        ULONG                       size,
        PVOID                       compact,
        PULONG                      pOutSize,
        ULONG                       mode,
        ULONG                       flag
    )
{
    UNREFERENCED_PARAMETER(plain);
    UNREFERENCED_PARAMETER(compact);
    UNREFERENCED_PARAMETER(pOutSize);
    UNREFERENCED_PARAMETER(mode);
    UNREFERENCED_PARAMETER(flag);
    return  size;
}


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_STATUS
        AnscCryptoDeflateCompress
            (
                PVOID                       plain,
                ULONG                       size,
                PVOID                       compact,
                PULONG                      pOutSize,
                ULONG                       mode,
                ULONG                       flag
            );

    description:

        This function performs cryptography computation.

    argument:   PVOID                       plain
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                ULONG                       size
                Specifies the size of the data buffer.

                PVOID                       compact
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                PULONG                      pOutSize,
                The buffer of output size;

                ULONG                       mode
                Specifies the cryptography parameter to be used.

                ULONG                       flag
                Specifies the cryptography parameter to be used.

    return:     the status of the operation

**********************************************************************/

ANSC_STATUS
AnscCryptoDeflateCompress
    (
        PVOID                       plain,
        ULONG                       size,
        PVOID                       compact,
        PULONG                      pOutSize,
        ULONG                       mode,
        ULONG                       flag
    )
{
    UNREFERENCED_PARAMETER(plain);
    UNREFERENCED_PARAMETER(compact);
    UNREFERENCED_PARAMETER(pOutSize);
    UNREFERENCED_PARAMETER(mode);
    UNREFERENCED_PARAMETER(flag);
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
        AnscCryptoLzsCompress
            (
                PVOID                       plain,
                ULONG                       size,
                PVOID                       compact,
                PULONG                      pOutSize,
                ULONG                       mode,
                ULONG                       flag
            );

    description:

        This function performs cryptography computation.

    argument:   PVOID                       plain
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                ULONG                       size
                Specifies the size of the data buffer.

                PVOID                       compact
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                PULONG                      pOutSize,
                The buffer of output size;

                ULONG                       mode
                Specifies the cryptography parameter to be used.

                ULONG                       flag
                Specifies the cryptography parameter to be used.

    return:     the status of the operation

**********************************************************************/

ANSC_STATUS
AnscCryptoLzsCompress
    (
        PVOID                       plain,
        ULONG                       size,
        PVOID                       compact,
        PULONG                      pOutSize,
        ULONG                       mode,
        ULONG                       flag
    )
{
    PVOID                           pCompressedBuffer      = NULL;
    PVOID                           pCompressedData        = NULL;
    ULONG                           ulCompressedBufferSize = 0;
    ULONG                           ulCompressedDataSize   = 0;
    PVOID                           pScratchRam            = NULL;

    /*
     * dynamically allocate memory for holding the compressed data
     * note: I don't know if the implementation of LZS I have can deal with the situation that the input and output
     * buffer are the same address, for safe, we allocate intermediate memory for holding the compressed data
     */
    ulCompressedBufferSize = size + size / 8 + 256;
    ulCompressedDataSize   = ulCompressedBufferSize;
    pCompressedBuffer      = AnscAllocateMemory(ulCompressedBufferSize);
    pCompressedData        = pCompressedBuffer;

    /*
     * dynamically allocate memory for holding the LZS history data
     */
    pScratchRam = AnscAllocateMemory(LZS_HISTORY_SIZE);

    /*
     * perform LZS compression
     */
    LZS_InitHistory(pScratchRam);

    LZS_Compress
        (
            (unsigned char**)&plain,
            (unsigned char**)&pCompressedBuffer,
            &size,
            &ulCompressedDataSize,
            pScratchRam,
            (USHORT)(LZS_SOURCE_FLUSH | LZS_DEST_FLUSH | mode),
            (USHORT)flag
        );

    ulCompressedDataSize = ulCompressedBufferSize - ulCompressedDataSize;

    /*
     * copy the compressed data into the output buffer provided by the COMP engine
     */
    AnscCopyMemory
        (
            compact,
            pCompressedData,
            ulCompressedDataSize
        );

    /*
     * don't forget to flush the allocated intermediate memory buffer
     */
    AnscFreeMemory(pCompressedData);
    AnscFreeMemory(pScratchRam);

    /*
     * return the size of compressed result to caller
     */
    *pOutSize =  ulCompressedDataSize;

    return ANSC_STATUS_SUCCESS;
}

#endif

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_STATUS
        AnscCryptoV42bisCompress
            (
                PVOID                       plain,
                ULONG                       size,
                PVOID                       compact,
                PULONG                      pOutSize,
                ULONG                       mode,
                ULONG                       flag
            );

    description:

        This function performs cryptography computation.

    argument:   PVOID                       plain
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                ULONG                       size
                Specifies the size of the data buffer.

                PVOID                       compact
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                PULONG                      pOutSize,
                The buffer of output size;

                ULONG                       mode
                Specifies the cryptography parameter to be used.

                ULONG                       flag
                Specifies the cryptography parameter to be used.

    return:     the status of the operation

**********************************************************************/

ANSC_STATUS
AnscCryptoV42bisCompress
    (
        PVOID                       plain,
        ULONG                       size,
        PVOID                       compact,
        PULONG                      pOutSize,
        ULONG                       mode,
        ULONG                       flag
    )
{
    UNREFERENCED_PARAMETER(plain);
    UNREFERENCED_PARAMETER(compact);
    UNREFERENCED_PARAMETER(pOutSize);
    UNREFERENCED_PARAMETER(mode);
    UNREFERENCED_PARAMETER(flag);
    return  size;
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_STATUS
        AnscCryptoZlibCompress
            (
                PVOID                       plain,
                ULONG                       size,
                PVOID                       compact,
                PULONG                      pOutSize
            );

    description:

        This function performs cryptography computation.

    argument:   PVOID                       plain
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                ULONG                       size
                Specifies the size of the data buffer.

                PVOID                       compact
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                PULONG                      pOutSize,
                The buffer of output size;

    return:     the status of the operation

**********************************************************************/

ANSC_STATUS
AnscCryptoZlibCompress
    (
        PVOID                       plain,
        ULONG                       size,
        PVOID                       compact,
        PULONG                      pOutSize
    )
{

#ifndef _NO_ANSC_ZLIB_

    LONG                            errCode;

    if( plain == NULL || compact == NULL || size == 0 || pOutSize == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    errCode = compress(compact, pOutSize, plain, size);

    if( errCode != 0)
    {
        return ANSC_STATUS_FAILURE;
    }

#endif
    UNREFERENCED_PARAMETER(plain);
    UNREFERENCED_PARAMETER(size);
    UNREFERENCED_PARAMETER(compact);
    UNREFERENCED_PARAMETER(pOutSize);
    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_STATUS
        AnscCryptoGzipCompress
            (
                PVOID                       plain,
                ULONG                       size,
                PVOID                       compact,
                PULONG                      pOutSize
            );

    description:

        This function performs cryptography computation.

    argument:

                PVOID                       plain
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                ULONG                       size
                Specifies the size of the data buffer.

                PVOID                       compact
                Specifies the data buffer to which the cryptography
                algorithm is to be applied.

                PULONG                      pOutSize,
                The buffer of output size;

    return:     the status of the operation

**********************************************************************/

ANSC_STATUS
AnscCryptoGzipCompress
    (
        PVOID                       plain,
        ULONG                       size,
        PVOID                       compact,
        PULONG                      pOutSize
    )
{

#ifdef _ANSC_GZIP_USED_

    ULONG                           length = 0;

    if( plain == NULL || compact == NULL || size == 0 || pOutSize == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    length = gz_al_compress(plain, size, compact, *pOutSize);

    if( length == 0)
    {
        return ANSC_STATUS_FAILURE;
    }

    *pOutSize = length;

#else
    UNREFERENCED_PARAMETER(plain);
    UNREFERENCED_PARAMETER(size);
    UNREFERENCED_PARAMETER(compact);
    UNREFERENCED_PARAMETER(pOutSize);
    AnscTrace("WARNING: GZIP is disabled!!!\n");
#endif

    return ANSC_STATUS_SUCCESS;
}
