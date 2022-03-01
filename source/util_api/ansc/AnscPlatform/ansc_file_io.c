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

    module:	ansc_file_io.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the some platform-dependent and
        general utility functions related to file I/O.

        *   AnscCreateFile
        *   AnscOpenFile
        *   AnscCloseFile
        *   AnscReadFile
        *   AnscWriteFile
        *   AnscSeekFile
        *   AnscCopyFile
        *   AnscDeleteFile
        *   AnscRenameFile
        *   AnscGetFileSize
        *   AnscGetFileExt
        *   AnscGetFileExt2
        *   AnscGetFileProperty
        *   AnscGetMimeType
        *   AnscCreateDirectory
        *   AnscDeleteDirectory
        *   AnscCopyDirectory
        *   AnscMoveFile
        *   AnscCreateGzipFile
        *   AnscOpenGzipFile
        *   AnscCloseGzipFile
        *   AnscReadGzipFile
        *   AnscWriteGzipFile
        *   AnscSeekGzipFile
        *   AnscGetGzipFileSize
        *   AnscSearchFirstFile
        *   AnscSearchNextFile
        *   AnscCloseSearch

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/13/02    initial revision.

**********************************************************************/


#include "ansc_platform.h"
#include "ansc_external_storage.h"
#include "ansc_crypto_interface.h"
#include "ansc_crypto_external_api.h"
#include "ansc_crypto_internal_api.h"
#include "safec_lib_common.h"


ANSC_HANDLE
AnscCreateFile
    (
        char*                       name,
        ULONG                       mode,
        ULONG                       type
    )
{

    if ( !(mode & ANSC_FILE_MODE_CREATE) )
    {
        mode |= ANSC_FILE_MODE_CREATE;
    }

    return  AnscOpenFile(name, mode, type);
}


ANSC_HANDLE
AnscOpenFile
    (
        char*                       name,
        ULONG                       mode,
        ULONG                       type
    )
{
    PANSC_FILE_INFO                 pFileInfo    = NULL;
    int                             iFileHandle  = -1;
    int                             iFileFlags   = 0;
    int                             iFilePmode   = 0;
    errno_t                         rc           = -1;

    if ( ((mode & ANSC_FILE_MODE_READ ) != 0) &&
         ((mode & ANSC_FILE_MODE_WRITE) != 0) )
    {
        iFileFlags |= ANSC_FILE_O_RDWR;
    }
    else if ( (mode & ANSC_FILE_MODE_READ) != 0 )
    {
        iFileFlags |= ANSC_FILE_O_RDONLY;
    }
    else if ( (mode & ANSC_FILE_MODE_WRITE) != 0 )
    {
        iFileFlags |= ANSC_FILE_O_WRONLY;
    }

    if ( (mode & ANSC_FILE_MODE_CREATE) != 0 )
    {
        iFileFlags |= ANSC_FILE_O_CREATE;
    }

    if ( (mode & ANSC_FILE_MODE_TRUNC) != 0 )
    {
        iFileFlags |= ANSC_FILE_O_TRUNC;
    }

    if ( (mode & ANSC_FILE_MODE_APPEND) != 0 )
    {
        iFileFlags |= ANSC_FILE_O_APPEND;
    }

    iFileFlags |= ANSC_FILE_O_BINARY;

    if ( (type & ANSC_FILE_TYPE_READ) != 0 )
    {
        iFilePmode |= ANSC_FILE_S_IREAD;
    }

    if ( (type & ANSC_FILE_TYPE_WRITE) != 0 )
    {
        iFilePmode |= ANSC_FILE_S_IWRITE;
    }

    if ( (type & ANSC_FILE_TYPE_RDGO) != 0 )
    {
        iFilePmode |= ANSC_FILE_S_IRGO;
    }

    iFileHandle = _ansc_open_file(name, iFileFlags, iFilePmode);

    if ( iFileHandle == -1 )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pFileInfo = (PANSC_FILE_INFO)AnscAllocateMemory(sizeof(ANSC_FILE_INFO));

        if ( !pFileInfo )
        {
            _ansc_close_file(iFileHandle);

            return  (ANSC_HANDLE)NULL;
        }
        else if ( mode & ANSC_FILE_MODE_ZLIB_COMPRESSED )
        {
            pFileInfo->bZlibCompressed = TRUE;
        }
    }

    rc = strcpy_s(pFileInfo->Name, sizeof(pFileInfo->Name), name);
    ERR_CHK(rc);

    pFileInfo->Handle = iFileHandle;

    return  (ANSC_HANDLE)pFileInfo;
}


ANSC_STATUS
AnscCloseFile
    (
        ANSC_HANDLE                 hFile
    )
{
    PANSC_FILE_INFO                 pFileInfo    = (PANSC_FILE_INFO)hFile;

    if ( !pFileInfo )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        _ansc_close_file(pFileInfo->Handle);
    }

    AnscFreeMemory(pFileInfo);

    return  ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
AnscReadFile
    (
        ANSC_HANDLE                 hFile,
        PVOID                       buffer,
        PULONG                      pulSize
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_FILE_INFO                 pFileInfo    = (PANSC_FILE_INFO)hFile;
    int                             iReadSize    = (int            )*pulSize;
    int 			    iReturnsize  = 0;

    if ( !pFileInfo )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    if ( pFileInfo->bZlibCompressed )
    {
#ifdef _64BIT_ARCH_SUPPORT_
	ULONG uHandle = pFileInfo->Handle;
	iReturnsize  = _ansc_get_file_size((VOID*)uHandle);
#else
	iReturnsize  = _ansc_get_file_size((VOID*)pFileInfo->Handle);
#endif
	if ( iReturnsize < 0 )
	{
		return ANSC_STATUS_FAILURE;
	}
        ULONG                       ulFileSize      = (ULONG              )iReturnsize;
        /* CID:53804 Argument cannot be negative*/
        void*                       pCompressedFile = (void*              )AnscAllocateMemory(ulFileSize);

        if ( !pCompressedFile )
        {
            *pulSize = 0;

            return  ANSC_STATUS_RESOURCES;
        }

        do
        {
            iReadSize =
                _ansc_read_file
                    (
                        pFileInfo->Handle,
                        pCompressedFile,
                        (int)ulFileSize
                    );

            if ( iReadSize == -1 )
            {
                *pulSize     = 0;
                returnStatus = ANSC_STATUS_FAILURE;

                break;
            }

            returnStatus =
            	AnscCryptoZlibCompress
                    (
                        pCompressedFile,
                        (ULONG)iReadSize,
                        buffer,
                        pulSize
                    );

            break;
        }
        while ( FALSE );

        AnscFreeMemory(pCompressedFile);

        return  returnStatus;
    }
    else
    {
        iReadSize =
            _ansc_read_file
                (
                    pFileInfo->Handle,
                    buffer,
                    iReadSize
                );

        if ( iReadSize == -1 )
        {
            *pulSize = 0;

            return  ANSC_STATUS_FAILURE;
        }

        *pulSize = (ULONG)iReadSize;
    }

    return  ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
AnscWriteFile
    (
        ANSC_HANDLE                 hFile,
        PVOID                       buffer,
        PULONG                      pulSize
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_FILE_INFO                 pFileInfo    = (PANSC_FILE_INFO)hFile;
    int                             iWriteSize   = (int)*pulSize;

    if ( !pFileInfo )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    if ( pFileInfo->bZlibCompressed )
    {
        ULONG                       ulFileSize      = (ULONG              )(*pulSize * 110 / 100) + 16;
        void*                       pCompressedFile = (void*              )AnscAllocateMemory(ulFileSize);

        if ( !pCompressedFile )
        {
            *pulSize = 0;

            return  ANSC_STATUS_RESOURCES;
        }

        do
        {
            returnStatus =
            	AnscCryptoZlibCompress
                    (
                        buffer,
                        *pulSize,
                        pCompressedFile,
                        &ulFileSize
                    );

            if ( returnStatus != ANSC_STATUS_SUCCESS )
            {
                break;
            }

            iWriteSize =
                _ansc_write_file
                    (
                        pFileInfo->Handle,
                        pCompressedFile,
                        (int)ulFileSize
                    );

            if ( iWriteSize == -1 )
            {
                *pulSize     = 0;
                returnStatus = ANSC_STATUS_FAILURE;

                break;
            }

            break;
        }
        while ( FALSE );

        AnscFreeMemory(pCompressedFile);

        return  returnStatus;
    }
    else
    {
        iWriteSize =
            _ansc_write_file
                (
                    pFileInfo->Handle,
                    buffer,
                    iWriteSize
                );

        if ( iWriteSize == -1 )
        {
            *pulSize = 0;

            return  ANSC_STATUS_FAILURE;
        }

        *pulSize = (ULONG)iWriteSize;
    }

    return  ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
AnscSeekFile
    (
        ANSC_HANDLE                 hFile,
        ULONG                       ulOffset,
        int                         iPos
    )
{
    PANSC_FILE_INFO                 pFileInfo    = (PANSC_FILE_INFO)hFile;

    if ( !pFileInfo )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    return  (_ansc_seek_file(pFileInfo->Handle, (long)ulOffset, iPos) != -1)? ANSC_STATUS_SUCCESS : ANSC_STATUS_FAILURE;
}


ANSC_STATUS
AnscCopyFile
    (
        char*                       pSrcFile,
        char*                       pDstFile,
        BOOL                        bOverwriteDst
    )
{
    if ( _ansc_copy_file(pSrcFile, pDstFile, !bOverwriteDst) )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    return  ANSC_STATUS_FAILURE;
}


ANSC_STATUS
AnscDeleteFile
    (
        char*                       file_name
    )
{
    if ( _ansc_delete_file(file_name) )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    return  ANSC_STATUS_FAILURE;
}


ANSC_STATUS
AnscRenameFile
    (
        char*                       old_file_name,
        char*                       new_file_name
    )
{
    if ( _ansc_rename_file(old_file_name, new_file_name) )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    return  ANSC_STATUS_FAILURE;
}


ULONG
AnscGetFileSize
    (
        ANSC_HANDLE                 hFile
    )
{
    PANSC_FILE_INFO                 pFileInfo    = (PANSC_FILE_INFO)hFile;
    ULONG                           ulFileSize   = 0;
    int				    iReturnSize  = 0;
    if ( !pFileInfo )
    {
        return  0;
    }
    else if ( pFileInfo->bZlibCompressed )
    {
        int iReadSize = (int)0;

#ifdef _64BIT_ARCH_SUPPORT_
	ULONG uHandle = pFileInfo->Handle;
	iReturnSize = _ansc_get_file_size((VOID*)uHandle);
#else
	iReturnSize = _ansc_get_file_size((VOID*)pFileInfo->Handle);
#endif
	if ( iReturnSize < 0 )
	{
		return 0;
	}
        ULONG                       ulCompressedFileSize = (ULONG              )iReturnSize;
        /*CID:63954 Argument cannot be negative*/
        void*                       pCompressedFile      = (void*              )AnscAllocateMemory(ulCompressedFileSize);

        if ( !pCompressedFile )
        {
            return  ulCompressedFileSize;
        }

        do
        {
            iReadSize =
                _ansc_read_file
                    (
                        pFileInfo->Handle,
                        pCompressedFile,
                        (int)ulCompressedFileSize
                    );

            if ( (ULONG)iReadSize != ulCompressedFileSize )
            {

                break;
            }

            	AnscCryptoZlibDecompress
                    (
                        pCompressedFile,
                        (ULONG)iReadSize,
                        NULL,
                        &ulFileSize
                    );

            break;
        }
        while ( FALSE );

        AnscFreeMemory(pCompressedFile);

        /*
         * Reset the file pointer position to the beginning of the file...
         */
        _ansc_seek_file(pFileInfo->Handle, (long)0, ANSC_FILE_SEEK_SET);

        return  ulFileSize;
    }
    else
    {
	/*CID: 59651 Improper use of negative value*/
#ifdef _64BIT_ARCH_SUPPORT_
        ULONG uHandle = pFileInfo->Handle;
        iReturnSize = _ansc_get_file_size((VOID*)uHandle);
#else
        iReturnSize = _ansc_get_file_size((VOID*)pFileInfo->Handle);
#endif
        if(iReturnSize < 0)
	{
		return	0;
	}
        ulFileSize = (ULONG)iReturnSize;
    }

    return  ulFileSize;
}


char*
AnscGetFileExt
    (
        ANSC_HANDLE                 hFile
    )
{
    PANSC_FILE_INFO                 pFileInfo    = (PANSC_FILE_INFO)hFile;
    char*                           pTempChar    = NULL;
    ULONG                           ulCharCount  = 0;
    ULONG                           ulNameSize   = 0;

    if ( !pFileInfo )
    {
        return  NULL;
    }
    else
    {
        ulNameSize = AnscSizeOfString(pFileInfo->Name);
        pTempChar  = pFileInfo->Name;
        pTempChar += ulNameSize - 1;
    }

    while ( (*pTempChar != '.') && (ulNameSize > 0) && (ulCharCount <= ANSC_FILE_MAX_EXT_LEN) )
    {
        pTempChar  --;
        ulCharCount++;
        ulNameSize --;
    }

    if ( *pTempChar != '.' )
    {
        return  NULL;
    }
    else if ( ulCharCount == 0 )
    {
        return  NULL;
    }
    else
    {
        pTempChar++;
    }

    return  pTempChar;
}


char*
AnscGetFileExt2
    (
        char*                       file_name
    )
{
    char*                           pTempChar    = NULL;
    ULONG                           ulCharCount  = 0;
    ULONG                           ulNameSize   = 0;

    if ( TRUE )
    {
        ulNameSize = AnscSizeOfString(file_name);
        pTempChar  = file_name;
        pTempChar += ulNameSize - 1;
    }

    while ( (*pTempChar != '.') && (ulNameSize > 0) && (ulCharCount <= ANSC_FILE_MAX_EXT_LEN) )
    {
        pTempChar  --;
        ulCharCount++;
        ulNameSize --;
    }

    if ( *pTempChar != '.' )
    {
        return  NULL;
    }
    else if ( ulCharCount == 0 )
    {
        return  NULL;
    }
    else
    {
        pTempChar++;
    }

    return  pTempChar;
}

ANSC_HANDLE
AnscGetFileProperty
    (
        char*                       file_name
    )
{
    PANSC_FILE_PROPERTY             pAnscFileProperty = (PANSC_FILE_PROPERTY)AnscAllocateMemory(sizeof(ANSC_FILE_PROPERTY));

    if ( !pAnscFileProperty )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else if ( _ansc_get_file_stat(file_name, pAnscFileProperty) )
    {
        return  (ANSC_HANDLE)pAnscFileProperty;
    }
    else
    {
        AnscFreeMemory(pAnscFileProperty);
    }

    return  (ANSC_HANDLE)NULL;
}


ANSC_STATUS
AnscGetMimeType
    (
        ANSC_HANDLE                 hFile,
        PULONG                      pulMediaType,
        PULONG                      pulSubType
    )
{
    PANSC_FILE_INFO                 pFileInfo    = (PANSC_FILE_INFO)hFile;
    char*                           pFileExt     = NULL;
    ULONG                           i            = 0;

    *pulMediaType = IANA_MEDIA_TYPE_CODE_RESERVED;
    *pulSubType   = 0;

    if ( !pFileInfo )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        pFileExt = AnscGetFileExt(hFile);
    }

    if ( !pFileExt )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    for ( i = 0; i < ANSC_FILE_EXT_ARRAY_SIZE; i++ )
    {
        if ( strcasecmp(pFileExt,g_FileExtArray[i]) == 0 )
        {
            *pulMediaType = g_MediaTypeArray[i];
            *pulSubType   = g_SubTypeArray[i];

            return  ANSC_STATUS_SUCCESS;
        }
    }

    return  ANSC_STATUS_CANT_FIND;
}

ANSC_STATUS
AnscCreateDirectory
    (
        char*                       dir_name
    )
{
    return ( _ansc_create_directory(dir_name) ? ANSC_STATUS_SUCCESS : ANSC_STATUS_FAILURE );
}


ANSC_STATUS
AnscDeleteDirectory
    (
        char*                       dir_name
    )
{
    return ( _ansc_delete_directory(dir_name) ? ANSC_STATUS_SUCCESS : ANSC_STATUS_FAILURE );
}


ANSC_STATUS
AnscCopyDirectory
    (
        char*                       src_dir,
        char*                       dst_dir
    )
{
    return ( _ansc_copy_directory(src_dir, dst_dir) ? ANSC_STATUS_SUCCESS : ANSC_STATUS_FAILURE );
}


ANSC_STATUS
AnscMoveFile
    (
        char*                       src_file,
        char*                       dst_file
    )
{
    return ( _ansc_move_file(src_file, dst_file) ? ANSC_STATUS_SUCCESS : ANSC_STATUS_FAILURE );
}


#ifdef  _ANSC_FILE_GZIP_

ANSC_HANDLE
AnscCreateGzipFile
    (
        char*                       name,
        ULONG                       mode
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_FILE_INFO                 pFileInfo    = NULL;
    int                             iFileHandle  = -1;

    if ( !(mode & ANSC_FILE_MODE_CREATE) )
    {
        mode |= ANSC_FILE_MODE_CREATE;
    }

    return  AnscOpenGzipFile(name, mode);
}


ANSC_HANDLE
AnscOpenGzipFile
    (
        char*                       name,
        ULONG                       mode
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_FILE_INFO                 pFileInfo    = NULL;
    int                             iFileHandle  = -1;
    char*                           iFileFlags   = NULL;
    int                             iFilePmode   = 0;
    errno_t                         rc           = -1;

    if ( (mode & ANSC_FILE_MODE_READ) != 0 )
    {
        iFileFlags = ANSC_GZIP_FILE_O_RDONLY;
    }
    else if ( (mode & ANSC_FILE_MODE_WRITE) != 0 )
    {
        iFileFlags = ANSC_GZIP_FILE_O_WRONLY;
    }
    else
    {
        return (ANSC_HANDLE)NULL;
    }

    iFileHandle = (int)_ansc_open_gzip_file(name, iFileFlags);

    if ( iFileHandle == 0 )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pFileInfo = (PANSC_FILE_INFO)AnscAllocateMemory(sizeof(ANSC_FILE_INFO));

        if ( !pFileInfo )
        {
            _ansc_close_gzip_file(iFileHandle);

            return  (ANSC_HANDLE)NULL;
        }
    }

    rc = strcpy_s(pFileInfo->Name, sizeof(pFileInfo->Name), name);
    ERR_CHK(rc);

    pFileInfo->Handle = iFileHandle;

    return  (ANSC_HANDLE)pFileInfo;
}


ANSC_STATUS
AnscCloseGzipFile
    (
        ANSC_HANDLE                 hFile
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_FILE_INFO                 pFileInfo    = (PANSC_FILE_INFO)hFile;

    if ( !pFileInfo )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        _ansc_close_gzip_file(pFileInfo->Handle);
    }

    AnscFreeMemory(pFileInfo);

    return  ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
AnscReadGzipFile
    (
        ANSC_HANDLE                 hFile,
        PVOID                       buffer,
        PULONG                      pulSize
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_FILE_INFO                 pFileInfo    = (PANSC_FILE_INFO)hFile;
    int                             iReadSize    = (int)*pulSize;

    if ( !pFileInfo )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    iReadSize =
        _ansc_read_gzip_file
            (
                pFileInfo->Handle,
                buffer,
                iReadSize
            );

    if ( iReadSize == -1 )
    {
        *pulSize = 0;

        return  ANSC_STATUS_FAILURE;
    }

    *pulSize = (ULONG)iReadSize;

    return  ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
AnscWriteGzipFile
    (
        ANSC_HANDLE                 hFile,
        PVOID                       buffer,
        PULONG                      pulSize
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_FILE_INFO                 pFileInfo    = (PANSC_FILE_INFO)hFile;
    int                             iWriteSize   = (int)*pulSize;

    if ( !pFileInfo )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    iWriteSize =
        _ansc_write_gzip_file
            (
                pFileInfo->Handle,
                buffer,
                iWriteSize
            );

    if ( iWriteSize == -1 )
    {
        *pulSize = 0;

        return  ANSC_STATUS_FAILURE;
    }

    *pulSize = (ULONG)iWriteSize;

    return  ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
AnscSeekGzipFile
    (
        ANSC_HANDLE                 hFile,
        ULONG                       ulOffset,
        int                         iPos
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_FILE_INFO                 pFileInfo    = (PANSC_FILE_INFO)hFile;

    if ( !pFileInfo )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else if ( ulOffset != 0xFFFFFFFF )
    {
        _ansc_seek_gzip_file(pFileInfo->Handle, (long)ulOffset, iPos);
    }

    return  ANSC_STATUS_SUCCESS;
}


ULONG
AnscGetGzipFileSize
    (
        ANSC_HANDLE                 hFile
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_FILE_INFO                 pFileInfo    = (PANSC_FILE_INFO)hFile;
    ULONG                           ulFileSize   = 0;

    if ( !pFileInfo )
    {
        return  0;
    }
    else
    {
        int                         bytes = 0;
        int                         pos   = _ansc_seek_gzip_file(pFileInfo->Handle, 0L, ANSC_FILE_SEEK_CUR);
        static char                 buf[4096];      /* I'm nervous about 4K allocation on stack, will change it */

        while ( (bytes = _ansc_read_gzip_file(pFileInfo->Handle, buf, 4096)) > 0 )
        {
            ulFileSize += bytes;
        }

        _ansc_seek_gzip_file(pFileInfo->Handle, pos, ANSC_FILE_SEEK_SET);
    }

    return  ulFileSize;
}

#endif


#ifdef  _ANSC_FILE_SEARCH_

ANSC_HANDLE
AnscSearchFirstFile
    (
        char*                       dir_name,
        char*                       target_file_name,   /* can be wildcard, cannot be NULL */
        PBOOL                       pbDirectory,
        char*                       first_file_name
    )
{
    ANSC_HANDLE                     hSearchContext  = NULL;
	/*printf("in ansc_file_io.c, AnscSearchFirstFile begin\n");*/

    if ( _ansc_find_first_file
            (
                dir_name,
                target_file_name,
                &hSearchContext,
                (char *)pbDirectory,
                first_file_name
            ) )
    {
		/*printf("in ansc_file_io.c, AnscSearchFirstFile end 1 hSearchContext: %d\n", hSearchContext);*/
        return  hSearchContext;
    }

	/*printf("in ansc_file_io.c, AnscSearchFirstFile end 2 hSearchContext: %d\n", hSearchContext);*/
    return  NULL;
}


BOOL
AnscSearchNextFile
    (
        ANSC_HANDLE                 hSearchContext,
        PBOOL                       pbDirectory,
        char*                       file_name
    )
{
	/*printf("in ansc_file_io.c before _ansc_find_next_file\n");*/
    if ( _ansc_find_next_file
            (
                hSearchContext,
                (char *)pbDirectory,
                file_name
            ) )
    {
		/*printf("in ansc_file_io.c after _ansc_find_next_file return true file_name: %s\n", file_name);*/
        return  TRUE;
    }

	/*printf("in ansc_file_io.c after _ansc_find_next_file return false file_name: %s\n", file_name);*/
    return  FALSE;
}


void
AnscCloseSearch
    (
        ANSC_HANDLE                 hSearchContext
    )
{
    _ansc_find_close(hSearchContext);
}

#endif
