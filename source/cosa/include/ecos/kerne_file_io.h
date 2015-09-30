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

    module:	kernel_file_io.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco System  , Inc., 1997 ~ 2006
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines some basic data types and
        structures on a particular platform.

    ---------------------------------------------------------------

    environment:

        eCos 1.3.1

    ---------------------------------------------------------------

    author:

        Hua Ding

    ---------------------------------------------------------------

    revision:

        05/01/06    initial revision.

**********************************************************************/


#ifndef  _KERNEL_FILE_IO_
#define  _KERNEL_FILE_IO_


/***********************************************************
        DEFINITION OF BASIC DATA TYPE AND STRUCTURES
***********************************************************/

#define O_RDONLY	0x0
#define O_WRONLY	0x1
#define O_RDWR		0x2
#define O_ACCMODE	0x3		/* Mask for O_RDONLY | O_WRONLY | O_RDWR */
#define O_NDELAY	0x4		/* Non-blocking I/O */
#define O_APPEND	0x8		/* append (writes guaranteed at the end) */
#define O_NONBLOCK	0x10	/* Non-blocking I/0, but different effect */
#define O_BINARY	0x20
#define O_CREAT		0x40
#define O_TRUNC		0x80
#define O_NOCTTY	0x100

#define SEEK_CUR    1
#define SEEK_END    2
#define SEEK_SET    0


#define  KERNEL_FILE_SEEK_SET                         SEEK_SET
#define  KERNEL_FILE_SEEK_CUR                         SEEK_CUR
#define  KERNEL_FILE_SEEK_END                         SEEK_END

#define  KERNEL_FILE_O_APPEND                         O_APPEND
#define  KERNEL_FILE_O_BINARY                         0                   /*unsupported*/
#define  KERNEL_FILE_O_CREATE                         O_CREAT
#define  KERNEL_FILE_O_SHORT_LIVED                    0                   /*unsupported*/
#define  KERNEL_FILE_O_TEMPORARY                      0                   /*unsupported*/
#define  KERNEL_FILE_O_EXCL                           O_EXCL
#define  KERNEL_FILE_O_RANDOM                         0                   /*unsupported*/
#define  KERNEL_FILE_O_RDONLY                         O_RDONLY
#define  KERNEL_FILE_O_RDWR                           O_RDWR
#define  KERNEL_FILE_O_SEQUENTIAL                     0                   /*unsupported*/
#define  KERNEL_FILE_O_TEXT                           0                   /*unsupported*/
#define  KERNEL_FILE_O_TRUNC                          O_TRUNC
#define  KERNEL_FILE_O_WRONLY                         O_WRONLY

#define  KERNEL_GZIP_FILE_O_RDONLY                    O_RDONLY
#define  KERNEL_GZIP_FILE_O_WRONLY                    O_RDWR

#define  KERNEL_FILE_S_IREAD                          0                   /*unsupported*/
#define  KERNEL_FILE_S_IWRITE                         0                   /*unsupported*/

#define  KERNEL_FILE_PATH_SEP                         '/'

#define  kernel_open_file                             open
#define  kernel_close_file                            close
#define  kernel_read_file                             read
#define  kernel_write_file                            write
#define  kernel_seek_file                             lseek
#define  kernel_copy_file(src, dst, fail_if_exists)   (0)
#define  kernel_delete_file(fname)                    (0)
#define  kernel_get_file_size(hFile)                  (0)

#define  kernel_open_gzip_file(f, oflag)              (0)
#define  kernel_close_gzip_file                       close
#define  kernel_read_gzip_file                        read
#define  kernel_write_gzip_file                       write
#define  kernel_seek_gzip_file                        lseek

#endif

