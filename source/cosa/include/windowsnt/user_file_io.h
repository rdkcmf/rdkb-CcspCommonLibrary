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

    module:	user_file_io.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco System  , Inc., 1997 ~ 2002
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines some basic data types and
        structures on a particular platform.

    ---------------------------------------------------------------

    environment:

        platform dependent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/13/01    initial revision.

**********************************************************************/


#ifndef  _USER_FILE_IO_
#define  _USER_FILE_IO_

#include <time.h>
#include <direct.h>

/***********************************************************
        DEFINITION OF BASIC DATA TYPE AND STRUCTURES
***********************************************************/

#define  USER_FILE_ATTRIBUTE_Normal                 0x00000000
#define  USER_FILE_ATTRIBUTE_Directory              0x00000001
#define  USER_FILE_ATTRIBUTE_Hidden                 0x00000002
#define  USER_FILE_ATTRIBUTE_ReadOnly               0x00000004
#define  USER_FILE_ATTRIBUTE_System                 0x00000008

typedef  struct
_USER_FILE_PROPERTY
{
    ULONG                           Attributes;
    ULONG                           Size;
    USER_SYSTEM_TIME                AccessTime;
    USER_SYSTEM_TIME                ModificationTime;
    USER_SYSTEM_TIME                CreationTime;
}
USER_FILE_PROPERTY,  *PUSER_FILE_PROPERTY;


#define  USER_FILE_SEEK_SET                         SEEK_SET
#define  USER_FILE_SEEK_CUR                         SEEK_CUR
#define  USER_FILE_SEEK_END                         SEEK_END

#define  USER_FILE_O_APPEND                         _O_APPEND
#define  USER_FILE_O_BINARY                         _O_BINARY
#define  USER_FILE_O_CREATE                         _O_CREAT
#define  USER_FILE_O_SHORT_LIVED                    _O_SHORT_LIVED
#define  USER_FILE_O_TEMPORARY                      _O_TEMPORARY
#define  USER_FILE_O_EXCL                           _O_EXCL
#define  USER_FILE_O_RANDOM                         _O_RANDOM
#define  USER_FILE_O_RDONLY                         _O_RDONLY
#define  USER_FILE_O_RDWR                           _O_RDWR
#define  USER_FILE_O_SEQUENTIAL                     _O_SEQUENTIAL
#define  USER_FILE_O_TEXT                           _O_TEXT
#define  USER_FILE_O_TRUNC                          _O_TRUNC
#define  USER_FILE_O_WRONLY                         _O_WRONLY

#define  USER_GZIP_FILE_O_RDONLY                    _O_RDONLY
#define  USER_GZIP_FILE_O_WRONLY                    _O_WRONLY

#define  USER_FILE_S_IREAD                          _S_IREAD
#define  USER_FILE_S_IWRITE                         _S_IWRITE

#define  USER_FILE_PATH_SEP                         '\\'

#define  user_open_file                             _open
#define  user_close_file                            _close
#define  user_read_file                             _read
#define  user_write_file                            _write
#define  user_seek_file                             _lseek
#define  user_copy_file(src, dst, fail_if_exists)   CopyFile(src, dst, fail_if_exists)
#define  user_get_file_size                         _filelength

#define  user_open_gzip_file(f, oflag)              _open(f, oflag | _O_CREAT | _O_BINARY, _S_IREAD | _S_IWRITE)
#define  user_close_gzip_file                       _close
#define  user_read_gzip_file                        _read
#define  user_write_gzip_file                       _write
#define  user_seek_gzip_file                        _lseek

#define  user_create_directory                      mkdir

#define  USER_FILE_OP_CMD_BUF_SIZE                  540

__inline  BOOL
user_delete_file
    (
        char*                       file_name
    )
{
    DWORD                       dwAttr;               
    dwAttr = GetFileAttributes((LPCTSTR)file_name);       
    if ( dwAttr & FILE_ATTRIBUTE_READONLY )           
    {                                                 
        dwAttr &= ~FILE_ATTRIBUTE_READONLY;           
        SetFileAttributes((LPCTSTR)file_name, dwAttr);    
    }                                                 
    return DeleteFile(file_name);                                
}


__inline  BOOL
user_is_directory
    (
        char*                       path_name
    )
{
    struct  _stat                   file_stat;

    if ( _stat(path_name, &file_stat) == -1 )
    {
        return  FALSE;
    }
    else if ( file_stat.st_mode & _S_IFDIR )
    {
        return  TRUE;
    }

    return  FALSE;
}


__inline  BOOL
user_rename_file
    (
        char*                       old_file_name,
        char*                       new_file_name
    )
{
    return  MoveFile(old_file_name, new_file_name);
}


__inline  BOOL
user_get_file_stat
    (
        char*                       file_name,
        HANDLE                      h_file_property
    )
{
    PUSER_FILE_PROPERTY             pFileProperty = (PUSER_FILE_PROPERTY)h_file_property;
    struct _stat                    fst;
    int                             ret;
    struct tm*                      tm;

    ret = _stat(file_name, &fst);

    if ( ret == 0 )
    {
        tm = localtime(&fst.st_atime);
        pFileProperty->AccessTime.Year                  = (USHORT)(tm->tm_year + 1900);
        pFileProperty->AccessTime.Month                 = (USHORT)(tm->tm_mon + 1);
        pFileProperty->AccessTime.DayOfMonth            = (USHORT)tm->tm_mday;
        pFileProperty->AccessTime.Hour                  = (USHORT)tm->tm_hour;
        pFileProperty->AccessTime.Minute                = (USHORT)tm->tm_min;
        pFileProperty->AccessTime.Second                = (USHORT)tm->tm_sec;
        pFileProperty->AccessTime.MilliSecond           = (USHORT)0;
        pFileProperty->AccessTime.DayOfWeek             = (USHORT)tm->tm_wday;
        pFileProperty->AccessTime.bDayLightSaving       = tm->tm_isdst;

        tm = localtime(&fst.st_mtime);
        pFileProperty->ModificationTime.Year            = (USHORT)(tm->tm_year + 1900);
        pFileProperty->ModificationTime.Month           = (USHORT)(tm->tm_mon + 1);
        pFileProperty->ModificationTime.DayOfMonth      = (USHORT)tm->tm_mday;
        pFileProperty->ModificationTime.Hour            = (USHORT)tm->tm_hour;
        pFileProperty->ModificationTime.Minute          = (USHORT)tm->tm_min;
        pFileProperty->ModificationTime.Second          = (USHORT)tm->tm_sec;
        pFileProperty->ModificationTime.MilliSecond     = (USHORT)0;
        pFileProperty->ModificationTime.DayOfWeek       = (USHORT)tm->tm_wday;
        pFileProperty->ModificationTime.bDayLightSaving = tm->tm_isdst;

        tm = localtime(&fst.st_ctime);
        pFileProperty->CreationTime.Year                = (USHORT)(tm->tm_year + 1900);
        pFileProperty->CreationTime.Month               = (USHORT)(tm->tm_mon + 1);
        pFileProperty->CreationTime.DayOfMonth          = (USHORT)tm->tm_mday;
        pFileProperty->CreationTime.Hour                = (USHORT)tm->tm_hour;
        pFileProperty->CreationTime.Minute              = (USHORT)tm->tm_min;
        pFileProperty->CreationTime.Second              = (USHORT)tm->tm_sec;
        pFileProperty->CreationTime.MilliSecond         = (USHORT)0;
        pFileProperty->CreationTime.DayOfWeek           = (USHORT)tm->tm_wday;
        pFileProperty->CreationTime.bDayLightSaving     = tm->tm_isdst;

        pFileProperty->Size         = fst.st_size;
        pFileProperty->Attributes   = 0;

        if ( fst.st_mode & _S_IFDIR )
        {
            pFileProperty->Attributes |= USER_FILE_ATTRIBUTE_Directory;
        }

        if ( fst.st_mode & _S_IFREG )
        {
            pFileProperty->Attributes |= USER_FILE_ATTRIBUTE_Normal;
        }

        if ( fst.st_mode & _S_IREAD )
        {
            pFileProperty->Attributes |= USER_FILE_ATTRIBUTE_ReadOnly;
        }
    }

    return  (ret == 0);
}


__inline  BOOL
user_move_file
    (
        char*                       srcFile,
        char*                       dstFile
    )
{
    return MoveFile(srcFile, dstFile);
}


__inline  BOOL
user_copy_directory
    (
        char*                       srcDir,
        char*                       dstDir
    )
{
    char                            cmd[USER_FILE_OP_CMD_BUF_SIZE];
    int                             ret;
    char*                           pLastPath = srcDir + strlen(srcDir) - 1;

    while ( *pLastPath != '\\' && pLastPath >= srcDir )
    {
        pLastPath --;
    }

    if ( pLastPath < srcDir )
    {
        return FALSE;
    }

    pLastPath ++;
    if ( *(dstDir + strlen(dstDir) - 1) == '\\' )
    {
        dstDir[strlen(dstDir) - 1] = 0;
    }

    _ansc_sprintf(cmd, "xcopy \"%s\" \"%s\"\\\"%s\" /E /Q /Y /C /I /H", srcDir, dstDir, pLastPath);

    ret = system(cmd);

    return ret == 0;
}


__inline  BOOL
user_delete_directory
    (
        char*                       dir
    )
{
    char                            cmd[USER_FILE_OP_CMD_BUF_SIZE];
    int                             ret;

    _ansc_sprintf(cmd, "rmdir \"%s\" /S /Q", dir);

    ret = system(cmd);

    return ret == 0;
}


__inline  BOOL
user_find_first_file
    (
        char*                       dir_name,
        char*                       tar_file_name,
        HANDLE*                     ph_find_context,
        BOOL*                       pb_directory,
        char*                       first_file_name
    )
{
    HANDLE                          hSearchContext  = (HANDLE)NULL;
    ULONG                           ulFileNameSize  = strlen(dir_name) + strlen(tar_file_name) + 4;
    char*                           pTargetFileName = NULL;
    WIN32_FIND_DATA*                pWin32FindData  = NULL;
    BOOL                            bResult         = FALSE;

    *ph_find_context = (HANDLE)NULL;
    *pb_directory    = FALSE;

    pTargetFileName = (char*           )malloc(ulFileNameSize         );
    pWin32FindData  = (WIN32_FIND_DATA*)malloc(sizeof(WIN32_FIND_DATA));

    if ( !pTargetFileName || !pWin32FindData )
    {
        goto  EXIT1;
    }
    else
    {
        memset(pTargetFileName, 0, ulFileNameSize         );
        memset(pWin32FindData,  0, sizeof(WIN32_FIND_DATA));
    }

    strcpy(pTargetFileName, dir_name);
    if ( pTargetFileName[strlen(pTargetFileName) - 1] != '\\' )
    {
        strcat(pTargetFileName, "\\");
    }
    strcat(pTargetFileName, tar_file_name);

    hSearchContext =
        FindFirstFile
            (
                pTargetFileName,
                pWin32FindData
            );

    if ( hSearchContext == INVALID_HANDLE_VALUE )
    {
        goto  EXIT1;
    }
    else if ( (strlen(pWin32FindData->cFileName) + 1) > 2048 )
    {
        goto  EXIT1;
    }
    else
    {
        strcpy(first_file_name, pWin32FindData->cFileName);
    }

    bResult          = TRUE;
    *ph_find_context = hSearchContext;
    *pb_directory    = pWin32FindData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;


EXIT1:

    if ( pTargetFileName )
    {
        free(pTargetFileName);
    }

    if ( pWin32FindData )
    {
        free(pWin32FindData);
    }

    return  bResult;
}


__inline  BOOL
user_find_next_file
    (
        HANDLE                      h_find_context,
        BOOL*                       pb_directory,
        char*                       next_file_name
    )
{
    WIN32_FIND_DATA*                pWin32FindData = NULL;
    BOOL                            bResult        = FALSE;

    *pb_directory = FALSE;

    pWin32FindData = (WIN32_FIND_DATA*)malloc(sizeof(WIN32_FIND_DATA));

    if ( !pWin32FindData )
    {
        goto  EXIT1;
    }
    else
    {
        memset(pWin32FindData,  0, sizeof(WIN32_FIND_DATA));
    }

    if ( !FindNextFile
            (
                h_find_context,
                pWin32FindData
            ) )
    {
        goto  EXIT1;
    }
    else if ( (strlen(pWin32FindData->cFileName) + 1) > 2048 )
    {
        goto  EXIT1;
    }
    else
    {
        strcpy(next_file_name, pWin32FindData->cFileName);
    }

    bResult       = TRUE;
    *pb_directory = pWin32FindData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;


EXIT1:

    if ( pWin32FindData )
    {
        free(pWin32FindData);
    }

    return  bResult;
}


__inline  void
user_find_close
    (
        HANDLE                      h_find_context
    )
{
    FindClose(h_find_context);
}


#endif
