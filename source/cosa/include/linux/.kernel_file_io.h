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

        Linux kernel

    ---------------------------------------------------------------

    author:

        Geng Yang

    ---------------------------------------------------------------

    revision:

        03/13/01    initial revision.

**********************************************************************/


#ifndef  _KERNEL_FILE_IO_
#define  _KERNEL_FILE_IO_


/***********************************************************
        DEFINITION OF BASIC DATA TYPE AND STRUCTURES
***********************************************************/

#ifndef  SEEK_SET
    #define  SEEK_SET    0
#endif
#ifndef  SEEK_CUR
    #define  SEEK_CUR    1
#endif
#ifndef  SEEK_END
    #define  SEEK_END    2
#endif


#define  KERNEL_FILE_SEEK_SET                         SEEK_SET
#define  KERNEL_FILE_SEEK_CUR                         SEEK_CUR
#define  KERNEL_FILE_SEEK_END                         SEEK_END

#define  KERNEL_FILE_O_APPEND                         O_APPEND
#define  KERNEL_FILE_O_BINARY                         0
#define  KERNEL_FILE_O_CREATE                         O_CREAT
#define  KERNEL_FILE_O_SHORT_LIVED                    O_SHORT_LIVED
#define  KERNEL_FILE_O_TEMPORARY                      O_TEMPORARY
#define  KERNEL_FILE_O_EXCL                           O_EXCL
#define  KERNEL_FILE_O_RANDOM                         O_RANDOM
#define  KERNEL_FILE_O_RDONLY                         O_RDONLY
#define  KERNEL_FILE_O_RDWR                           O_RDWR
#define  KERNEL_FILE_O_SEQUENTIAL                     O_SEQUENTIAL
#define  KERNEL_FILE_O_TEXT                           O_TEXT
#define  KERNEL_FILE_O_TRUNC                          O_TRUNC
#define  KERNEL_FILE_O_WRONLY                         O_WRONLY

#define  KERNEL_FILE_S_IREAD                          O_RDONLY
#define  KERNEL_FILE_S_IWRITE                         O_WRONLY

#define  KERNEL_FILE_PATH_SEP                         '/'

struct file * 
kernel_open         (char *name, int iFileFlags, int iFilePmode);
int  kernel_close   (struct file *filp);
/*
 * linux kernel has implemented this function
 * int  kernel_read    (struct file *file, unsigned long offset, char * addr, unsigned long count);
 */
int  kernel_write   (struct file *filp, unsigned long offset,   char * addr, unsigned long count);
int  kernel_lseek   (struct file *filp, int offset,             int whence);
int  kernel_filelen (struct file *filp);
int  kernel_delete  (char *name);

#define  kernel_open_file(name, iFileFlags, iFilePmode)                                 \
         (int)kernel_open(name, iFileFlags, iFilePmode)

#define  kernel_close_file(handle)                                                      \
         kernel_close((struct file *)(handle))

#define  kernel_read_file(handle, buf, size)                                            \
         kernel_read((struct file *)(handle), 0, buf, size);

#define  kernel_write_file(handle, buf, size)                                           \
         kernel_write((struct file *)(handle), 0, buf, size);

#define  kernel_seek_file                             kernel_lseek
#define  kernel_get_file_size                         kernel_filelen

#define  kernel_copy_file             
#define  kernel_delete_file                           kernel_delete
#endif



