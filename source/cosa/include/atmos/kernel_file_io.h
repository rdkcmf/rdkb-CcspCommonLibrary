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

        Cisco System  , Inc., 1997 ~ 2002
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines some basic data types and
        structures on a particular platform.

    ---------------------------------------------------------------

    environment:

        atmos 7.1   and later

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Hua Ding

    ---------------------------------------------------------------

    revision:

        01/19/01    initial revision.
        04/24/02    atmos port

**********************************************************************/


#ifndef  _KERNEL_FILE_IO_
#define  _KERNEL_FILE_IO_


/***********************************************************
        DEFINITION OF BASIC DATA TYPE AND STRUCTURES
***********************************************************/

#define  KERNEL_FILE_SEEK_SET                       SEEK_SET
#define  KERNEL_FILE_SEEK_CUR                       SEEK_CUR
#define  KERNEL_FILE_SEEK_END                       SEEK_END

#define  KERNEL_FILE_O_APPEND                       0                   /*unsupported*/
#define  KERNEL_FILE_O_BINARY                       0                   /*unsupported*/
#define  KERNEL_FILE_O_CREATE                       0                   /*unsupported*/
#define  KERNEL_FILE_O_SHORT_LIVED                  0                   /*unsupported*/
#define  KERNEL_FILE_O_TEMPORARY                    0                   /*unsupported*/
#define  KERNEL_FILE_O_EXCL                         0                   /*unsupported*/
#define  KERNEL_FILE_O_RANDOM                       0                   /*unsupported*/
#define  KERNEL_FILE_O_RDONLY                       O_RDONLY
#define  KERNEL_FILE_O_RDWR                         O_RDWR
#define  KERNEL_FILE_O_SEQUENTIAL                   0                   /*unsupported*/
#define  KERNEL_FILE_O_TEXT                         0                   /*unsupported*/
#define  KERNEL_FILE_O_TRUNC                        0                   /*unsupported*/
#define  KERNEL_FILE_O_WRONLY                       O_WRONLY

#define  KERNEL_FILE_S_IREAD                        0                   /*unsupported*/
#define  KERNEL_FILE_S_IWRITE                       0                   /*unsupported*/

#define  KERNEL_FILE_PATH_SEP                       '/'

int
kernel_open_file
    (
        const char*                 filename,
        int                         oflag,
        int                         pmode
    );

int
kernel_close_file
    (
        int                         handle
    );


int
kernel_read_file
    (
        int                         handle,
        void*                       buffer,
        unsigned int                count
    );

int
kernel_write_file
    (
        int                         handle,
        void*                       buffer,
        unsigned int                count
    );

long
kernel_seek_file
    (
        int                         handle,
        long                        offset,
        int                         origin
    );

long
kernel_get_file_size
    (
        int                         handle
    );


#endif
