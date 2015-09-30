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

    module:	kernel_socket.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco System  , Inc., 1997 ~ 2006
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file implements socket wrapper routines to redirect
        to native OS socket functionality:

        *   _ansc_select
        *   _ansc_socket_fd_get
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


#include "kernel_base.h"
#include "kernel_socket.h"
#include "kernel_debug.h"


#ifndef  _ANSC_SOCKET_LIBRARY_

/**********************************************************************
                        PATCH DEFINITIONS
**********************************************************************/

int
_ansc_select
    (
        int                     width,
        pansc_fd_set            pReadFds,
        pansc_fd_set            pWriteFds,
        pansc_fd_set            pExceptFds,
        struct timeval*         pTimeout
    )
{
    fd_set*                 rSet         = pReadFds   ? &(pReadFds->set)   : NULL;
    fd_set*                 wSet         = pWriteFds  ? &(pWriteFds->set)  : NULL;
    fd_set*                 eSet         = pExceptFds ? &(pExceptFds->set) : NULL;

    /*KernelTrace("_ansc_select is called.\n");*/

    return  select(width, rSet, wSet, eSet, pTimeout);
}


void
_ansc_socket_fd_get
    (
        pansc_fd_set            pSet,
        int*                    pSocket,
        int                     i
    )
{
    int                 iArraySize          = sizeof(pSet->set.fds_bits) / sizeof(fd_mask);
    int                 iArrayIndex         = 0;
    int                 iBitIndex           = 0;
    int                 iSetCount           = 0;
    int                 bFound              = 0;

    /*KernelTrace("iArraySize: %d, MFDBITS: %d\n", iArraySize, NFDBITS);*/

    for ( iArrayIndex = 0; iArrayIndex < iArraySize; iArrayIndex ++ )
    {
	    /*KernelTrace("%X\n", pSet->set.fds_bits[iArrayIndex]);*/

        for ( iBitIndex = 0; iBitIndex < NFDBITS; iBitIndex ++ )
        {
            if( pSet->set.fds_bits[iArrayIndex] & (1 << iBitIndex) )
            {
                if( iSetCount == i )
                {
                    bFound = 1;
                    break;
                }

                iSetCount ++;
            }
        }

        if ( bFound == 1 )
        {
			break;
	    }
    }

    /*KernelTrace("iSetCount: %d, iBitIndex: %d\n", iSetCount, iBitIndex);*/

    if ( bFound == 1)
    {
        *pSocket = iArrayIndex * NFDBITS + iBitIndex;
    }
    else
    {
        *pSocket = ANSC_SOCKET_INVALID_SOCKET;
    }

    return;
}


#endif

