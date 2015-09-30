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

        platform dependent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/13/01    initial revision.

**********************************************************************/


#ifndef  _KERNEL_FILE_IO_
#define  _KERNEL_FILE_IO_


/***********************************************************
        DEFINITION OF BASIC DATA TYPE AND STRUCTURES
***********************************************************/

#define  KERNEL_FILE_SEEK_SET                       SEEK_SET
#define  KERNEL_FILE_SEEK_CUR                       SEEK_CUR
#define  KERNEL_FILE_SEEK_END                       SEEK_END

#define  KERNEL_FILE_O_APPEND                       _O_APPEND
#define  KERNEL_FILE_O_BINARY                       _O_BINARY
#define  KERNEL_FILE_O_CREATE                       _O_CREATE
#define  KERNEL_FILE_O_SHORT_LIVED                  _O_SHORT_LIVED
#define  KERNEL_FILE_O_TEMPORARY                    _O_TEMPORARY
#define  KERNEL_FILE_O_EXCL                         _O_EXCL
#define  KERNEL_FILE_O_RANDOM                       _O_RANDOM
#define  KERNEL_FILE_O_RDONLY                       _O_RDONLY
#define  KERNEL_FILE_O_RDWR                         _O_RDWR
#define  KERNEL_FILE_O_SEQUENTIAL                   _O_SEQUENTIAL
#define  KERNEL_FILE_O_TEXT                         _O_TEXT
#define  KERNEL_FILE_O_TRUNC                        _O_TRUNC
#define  KERNEL_FILE_O_WRONLY                       _O_WRONLY

#define  KERNEL_FILE_S_IREAD                        _S_IREAD
#define  KERNEL_FILE_S_IWRITE                       _S_IWRITE

#define  KERNEL_FILE_PATH_SEP                       '\'

#define  kernel_open_file                           _open
#define  kernel_close_file                          _close
#define  kernel_read_file                           _read
#define  kernel_write_file                          _write
#define  kernel_seek_file                           _lseek
#define  kernel_get_file_size                       _filelength


#endif
