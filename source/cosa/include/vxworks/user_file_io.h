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

        VxWorks 5.4

    ---------------------------------------------------------------

    author:

        Huaxiang Sun
        Hua Ding

    ---------------------------------------------------------------

    revision:

        03/13/01    initial revision.

**********************************************************************/


#ifndef  _USER_FILE_IO_
#define  _USER_FILE_IO_


/***********************************************************
        DEFINITION OF BASIC DATA TYPE AND STRUCTURES
***********************************************************/

#define  USER_FILE_SEEK_SET                         SEEK_SET
#define  USER_FILE_SEEK_CUR                         SEEK_CUR
#define  USER_FILE_SEEK_END                         SEEK_END

#define  USER_FILE_O_APPEND                         O_APPEND
#define  USER_FILE_O_BINARY                         0                   /*unsupported*/
#define  USER_FILE_O_CREATE                         O_CREAT
#define  USER_FILE_O_SHORT_LIVED                    0                   /*unsupported*/
#define  USER_FILE_O_TEMPORARY                      0                   /*unsupported*/
#define  USER_FILE_O_EXCL                           O_EXCL
#define  USER_FILE_O_RANDOM                         0                   /*unsupported*/
#define  USER_FILE_O_RDONLY                         O_RDONLY
#define  USER_FILE_O_RDWR                           O_RDWR
#define  USER_FILE_O_SEQUENTIAL                     0                   /*unsupported*/
#define  USER_FILE_O_TEXT                           0                   /*unsupported*/
#define  USER_FILE_O_TRUNC                          O_TRUNC
#define  USER_FILE_O_WRONLY                         O_WRONLY

#define  USER_GZIP_FILE_O_RDONLY                    O_RDONLY
#define  USER_GZIP_FILE_O_WRONLY                    O_RDWR

#define  USER_FILE_S_IREAD                          0                   /*unsupported*/
#define  USER_FILE_S_IWRITE                         0                   /*unsupported*/

#define  USER_FILE_PATH_SEP                         '/'

#define  user_open_file                             open
#define  user_close_file                            close
#define  user_read_file                             read
#define  user_write_file                            write
#define  user_seek_file                             lseek
#define  user_copy_file(src, dst, fail_if_exists)   (0)
#define  user_delete_file(fname)                    (0)
#define  user_get_file_size(hFile)                  (0)

#define  user_open_gzip_file(f, oflag)              (0)
#define  user_close_gzip_file                       close
#define  user_read_gzip_file                        read
#define  user_write_gzip_file                       write
#define  user_seek_gzip_file                        lseek

#endif

