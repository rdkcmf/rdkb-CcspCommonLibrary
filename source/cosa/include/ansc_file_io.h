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

    module:	ansc_file_io.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2001
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros related to file/directory creation,
        removal and management. Instead of defining everything in a
        single file, we simply route every function and macro to
        the corresponding platform-dependent prototype definition.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/13/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_FILE_IO_
#define  _ANSC_FILE_IO_


/***********************************************************
            FUNCTION AND MACRO PROTOTYPE ROUTING
***********************************************************/

/*
 * The file system I/O functions usually differ significantly on different operating systems. But
 * fortunately, most of them provide similar APIs for programmers to create/open/read/write files
 * and file directories.
 */
#define  ANSC_FILE_TYPE_READ                        0x00000001
#define  ANSC_FILE_TYPE_WRITE                       0x00000002
#define  ANSC_FILE_TYPE_RDWR                        0x00000003
#define  ANSC_FILE_TYPE_RDGO                        0x00000004

#define  ANSC_FILE_MODE_READ                        0x00000001
#define  ANSC_FILE_MODE_WRITE                       0x00000002
#define  ANSC_FILE_MODE_RDWR                        0x00000003
#define  ANSC_FILE_MODE_CREATE                      0x00000004
#define  ANSC_FILE_MODE_TRUNC                       0x00000008
#define  ANSC_FILE_MODE_APPEND                      0x00000010
#ifdef  _ANSC_FILE_ZLIB_
#define  ANSC_FILE_MODE_ZLIB_COMPRESSED             0x00000020
#else
#define  ANSC_FILE_MODE_ZLIB_COMPRESSED             0
#endif

#define  ANSC_FILE_MAX_EXT_LEN                      6
#define  ANSC_FILE_EXT_ARRAY_SIZE                   17


#ifdef   _ANSC_KERNEL

    #define  ANSC_FILE_SEEK_SET                     KERNEL_FILE_SEEK_SET
    #define  ANSC_FILE_SEEK_CUR                     KERNEL_FILE_SEEK_CUR
    #define  ANSC_FILE_SEEK_END                     KERNEL_FILE_SEEK_END

    #define  ANSC_FILE_O_APPEND                     KERNEL_FILE_O_APPEND
    #define  ANSC_FILE_O_BINARY                     KERNEL_FILE_O_BINARY
    #define  ANSC_FILE_O_CREATE                     KERNEL_FILE_O_CREATE
    #define  ANSC_FILE_O_SHORT_LIVED                KERNEL_FILE_O_SHORT_LIVED
    #define  ANSC_FILE_O_TEMPORARY                  KERNEL_FILE_O_TEMPORARY
    #define  ANSC_FILE_O_EXCL                       KERNEL_FILE_O_EXCL
    #define  ANSC_FILE_O_RANDOM                     KERNEL_FILE_O_RANDOM
    #define  ANSC_FILE_O_RDONLY                     KERNEL_FILE_O_RDONLY
    #define  ANSC_FILE_O_RDWR                       KERNEL_FILE_O_RDWR
    #define  ANSC_FILE_O_SEQUENTIAL                 KERNEL_FILE_O_SEQUENTIAL
    #define  ANSC_FILE_O_TEXT                       KERNEL_FILE_O_TEXT
    #define  ANSC_FILE_O_TRUNC                      KERNEL_FILE_O_TRUNC
    #define  ANSC_FILE_O_WRONLY                     KERNEL_FILE_O_WRONLY

    #define  ANSC_GZIP_FILE_O_RDONLY                KERNEL_GZIP_FILE_O_RDONLY
    #define  ANSC_GZIP_FILE_O_WRONLY                KERNEL_GZIP_FILE_O_WRONLY

    #define  ANSC_FILE_S_IREAD                      KERNEL_FILE_S_IREAD
    #define  ANSC_FILE_S_IWRITE                     KERNEL_FILE_S_IWRITE

    #define  ANSC_FILE_PATH_SEP                     KERNEL_FILE_PATH_SEP

    #define  _ansc_open_file                        kernel_open_file
    #define  _ansc_close_file                       kernel_close_file
    #define  _ansc_read_file                        kernel_read_file
    #define  _ansc_write_file                       kernel_write_file
    #define  _ansc_seek_file                        kernel_seek_file
    #define  _ansc_copy_file                        kernel_copy_file
    #define  _ansc_delete_file                      kernel_delete_file
    /*#define  _ansc_rename_file                      kernel_rename_file*/
    #define  _ansc_get_file_size                    kernel_get_file_size
    /*#define  _ansc_get_file_stat                    kernel_get_file_stat*/
    #define  _ansc_is_directory                     kernel_is_directory


    /*#define  _ansc_create_directory                 kernel_create_directory*/
    /*#define  _ansc_delete_directory                 kernel_delete_directory*/
    /*#define  _ansc_copy_directory                   kernel_copy_directory*/
    /*#define  _ansc_move_file                        kernel_move_file*/

    #ifdef  _ANSC_FILE_GZIP_
    #define  _ansc_open_gzip_file                   kernel_open_gzip_file
    #define  _ansc_close_gzip_file                  kernel_close_gzip_file
    #define  _ansc_read_gzip_file                   kernel_read_gzip_file
    #define  _ansc_write_gzip_file                  kernel_write_gzip_file
    #define  _ansc_seek_gzip_file                   kernel_seek_gzip_file
    #endif

    #ifdef  _ANSC_FILE_SEARCH_
    #define  _ansc_find_first_file                  kernel_find_first_file
    #define  _ansc_find_next_file                   kernel_find_next_file
    #define  _ansc_find_close                       kernel_find_close
    #endif

#else

    #define  ANSC_FILE_SEEK_SET                     USER_FILE_SEEK_SET
    #define  ANSC_FILE_SEEK_CUR                     USER_FILE_SEEK_CUR
    #define  ANSC_FILE_SEEK_END                     USER_FILE_SEEK_END

    #define  ANSC_FILE_O_APPEND                     USER_FILE_O_APPEND
    #define  ANSC_FILE_O_BINARY                     USER_FILE_O_BINARY
    #define  ANSC_FILE_O_CREATE                     USER_FILE_O_CREATE
    #define  ANSC_FILE_O_SHORT_LIVED                USER_FILE_O_SHORT_LIVED
    #define  ANSC_FILE_O_TEMPORARY                  USER_FILE_O_TEMPORARY
    #define  ANSC_FILE_O_EXCL                       USER_FILE_O_EXCL
    #define  ANSC_FILE_O_RANDOM                     USER_FILE_O_RANDOM
    #define  ANSC_FILE_O_RDONLY                     USER_FILE_O_RDONLY
    #define  ANSC_FILE_O_RDWR                       USER_FILE_O_RDWR
    #define  ANSC_FILE_O_SEQUENTIAL                 USER_FILE_O_SEQUENTIAL
    #define  ANSC_FILE_O_TEXT                       USER_FILE_O_TEXT
    #define  ANSC_FILE_O_TRUNC                      USER_FILE_O_TRUNC
    #define  ANSC_FILE_O_WRONLY                     USER_FILE_O_WRONLY

    #define  ANSC_GZIP_FILE_O_RDONLY                USER_GZIP_FILE_O_RDONLY
    #define  ANSC_GZIP_FILE_O_WRONLY                USER_GZIP_FILE_O_WRONLY

    #define  ANSC_FILE_S_IREAD                      USER_FILE_S_IREAD
    #define  ANSC_FILE_S_IWRITE                     USER_FILE_S_IWRITE
    #define  ANSC_FILE_S_IRGO                       USER_FILE_S_IRGO

    #define  ANSC_FILE_PATH_SEP                     USER_FILE_PATH_SEP

    #define  _ansc_open_file                        user_open_file
    #define  _ansc_close_file                       user_close_file
    #define  _ansc_read_file                        user_read_file
    #define  _ansc_write_file                       user_write_file
    #define  _ansc_seek_file                        user_seek_file
    #define  _ansc_copy_file                        user_copy_file
    #define  _ansc_delete_file                      user_delete_file
    #define  _ansc_rename_file                      user_rename_file
    #define  _ansc_get_file_size                    user_get_file_size
    #define  _ansc_get_file_stat                    user_get_file_stat
    #define  _ansc_is_directory                     user_is_directory

    #define  _ansc_create_directory                 user_create_directory
    #define  _ansc_delete_directory                 user_delete_directory
    #define  _ansc_copy_directory                   user_copy_directory
    #define  _ansc_move_file                        user_move_file

    #ifdef  _ANSC_FILE_GZIP_
    #define  _ansc_open_gzip_file                   user_open_gzip_file
    #define  _ansc_close_gzip_file                  user_close_gzip_file
    #define  _ansc_read_gzip_file                   user_read_gzip_file
    #define  _ansc_write_gzip_file                  user_write_gzip_file
    #define  _ansc_seek_gzip_file                   user_seek_gzip_file
    #endif

    #ifdef  _ANSC_FILE_SEARCH_
    #define  _ansc_find_first_file                  user_find_first_file
    #define  _ansc_find_next_file                   user_find_next_file
    #define  _ansc_find_close                       user_find_close
    #endif

#endif

/*
 * Instead of passing back and forth the OS-specific file handle directly, we use following data
 * structure to hold the file information to make the programming on ANSC platform more consistent.
 */
#define  ANSC_MAX_FILE_NAME_SIZE                    2048

typedef  struct
_ANSC_FILE_INFO
{
    SINGLE_LINK_ENTRY               Linkage;
    BOOL                            bZlibCompressed;
    char                            Name[ANSC_MAX_FILE_NAME_SIZE];
    int                             Handle;
}
ANSC_FILE_INFO,  *PANSC_FILE_INFO;

#define  ACCESS_ANSC_FILE_INFO(p)                   \
         ACCESS_CONTAINER(p, ANSC_FILE_INFO, Linkage)


/*
 * To support platform-independent file browsing, we need more definitions such as file attributes,
 * access time, modification time, creation time, so on and so forth. These contants and structures
 * are defined again in the associated user_file_io.h/kernel_file_io.h.
 */
#define  ANSC_FILE_ATTRIBUTE_Normal                 0x00000000
#define  ANSC_FILE_ATTRIBUTE_Directory              0x00000001
#define  ANSC_FILE_ATTRIBUTE_Hidden                 0x00000002
#define  ANSC_FILE_ATTRIBUTE_ReadOnly               0x00000004
#define  ANSC_FILE_ATTRIBUTE_System                 0x00000008

typedef  struct
_ANSC_FILE_PROPERTY
{
    ULONG                           Attributes;
    ULONG                           Size;
    ANSC_UNIVERSAL_TIME             AccessTime;
    ANSC_UNIVERSAL_TIME             ModificationTime;
    ANSC_UNIVERSAL_TIME             CreationTime;
}
ANSC_FILE_PROPERTY,  *PANSC_FILE_PROPERTY;


/***********************************************************
            FILE-RELATED HELP UTILITIES AND MACROS
***********************************************************/

/*
 * There're cases where we need to identify the media type of a particular disk file. For example:
 * in HTTP, many web server implementations simply map the request URI sent by client to a disk
 * file. In order to describe the requested entity, server needs to construct a "Content-Type" HTTP
 * header field along with the file content.
 */


#endif
