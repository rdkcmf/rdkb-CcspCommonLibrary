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

    module:	kernel_file_io.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco System  , Inc., 1997 ~ 2001
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file implements the platform dependent routines.

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


#include "kernel_base.h"
#include "kernel_debug.h"
#include "kernel_file_io.h"


int
kernel_open_file
    (
        const char*                 filename,
        int                         oflag,
        int                         pmode
    )
{
    FILE*                           pFile       = NULL;
    char*                           pOpenMode   = NULL;

    /*
     *  we cannot care about the flags...
     */
    if ( oflag == KERNEL_FILE_O_RDONLY )
    {
        pOpenMode = "r";
    }
    else if ( oflag == KERNEL_FILE_O_WRONLY )
    {
        pOpenMode = "w";
    }
    else if ( oflag == KERNEL_FILE_O_RDWR )
    {
        pOpenMode = "r+";
    }
    else
    {
        KernelAssert(FALSE);
    }

    pFile = fopen(filename, pOpenMode);

    if ( pFile == NULL )
    {
        return  -1;
    }
    else
    {
        return  (int)pFile;
    }
}


int
kernel_close_file
    (
        int                         handle
    )
{
    FILE*                           pFile       = (FILE*)handle;

    return  fclose(pFile);
}


int
kernel_read_file
    (
        int                         handle,
        void*                       buffer,
        unsigned int                count
    )
{
    FILE*                           pFile       = (FILE*)handle;
    size_t                          iRead;

    iRead = fread(buffer, 1, count, pFile);

    return  iRead;
}


int
kernel_write_file
    (
        int                         handle,
        void*                       buffer,
        unsigned int                count
    )
{
    FILE*                           pFile       = (FILE*)handle;
    size_t                          iWritten;

    iWritten = fwrite(buffer, 1, count, pFile);

    return  iWritten;
}


long
kernel_seek_file
    (
        int                         handle,
        long                        offset,
        int                         origin
    )
{
    FILE*                           pFile       = (FILE*)handle;

    return  fseek(pFile, offset, origin);
}


long
kernel_get_file_size
    (
        int                         handle
    )
{
    FILE*                           pFile       = (FILE*)handle;

    /*
     *  well, that I don't know...
     */
    return  0;
}
