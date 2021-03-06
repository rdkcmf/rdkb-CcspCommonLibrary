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

    description:

        This wrapper file defines some basic data types and
        structures on a particular platform.

    ---------------------------------------------------------------

    environment:

        OSE 4.2

    ---------------------------------------------------------------

    author:

        Hua Ding

    ---------------------------------------------------------------

    revision:

        07/31/02    initial revision.

**********************************************************************/


#ifndef  _KERNEL_FILE_IO_
#define  _KERNEL_FILE_IO_


/***********************************************************
        DEFINITION OF BASIC DATA TYPE AND STRUCTURES
***********************************************************/

#define  KERNEL_FILE_SEEK_SET                       SEEK_SET
#define  KERNEL_FILE_SEEK_CUR                       SEEK_CUR
#define  KERNEL_FILE_SEEK_END                       SEEK_END

#define  KERNEL_FILE_O_APPEND                       O_APPEND
#define  KERNEL_FILE_O_BINARY                       O_BINARY
#define  KERNEL_FILE_O_CREATE                       O_CREAT
#define  KERNEL_FILE_O_SHORT_LIVED                  O_SHORT_LIVED
#define  KERNEL_FILE_O_TEMPORARY                    O_TEMPORARY
#define  KERNEL_FILE_O_EXCL                         O_EXCL
#define  KERNEL_FILE_O_RANDOM                       O_RANDOM
#define  KERNEL_FILE_O_RDONLY                       O_RDONLY
#define  KERNEL_FILE_O_RDWR                         O_RDWR
#define  KERNEL_FILE_O_SEQUENTIAL                   O_SEQUENTIAL
#define  KERNEL_FILE_O_TEXT                         O_TEXT
#define  KERNEL_FILE_O_TRUNC                        O_TRUNC
#define  KERNEL_FILE_O_WRONLY                       O_WRONLY

#define  KERNEL_FILE_S_IREAD                        Error! Error!! Error!!!
#define  KERNEL_FILE_S_IWRITE                       Error! Error!! Error!!!

#define  KERNEL_FILE_PATH_SEP                       '/'

#define  KERNEL_open_file                           open
#define  KERNEL_close_file                          close
#define  KERNEL_read_file                           read
#define  KERNEL_write_file                          write
#define  KERNEL_seek_file                           fseek
#define  KERNEL_get_file_size                       Error! Error!! Error!!!

#endif
