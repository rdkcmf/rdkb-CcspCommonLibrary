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

    module:	kernel_base.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco System  , Inc., 1997 ~ 2001
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines some basic data types and
        structures on a particular platform.

    ---------------------------------------------------------------

    environment:

        pSOS 2.5

    ---------------------------------------------------------------

    author:

        Xuechen Yang
		Hua Ding

    ---------------------------------------------------------------

    revision:

        01/19/01    initial revision.
		10/15/01	pSOS port

**********************************************************************/


#ifndef  _KERNEL_BASE_
#define  _KERNEL_BASE_

#include "kernel_base_custom_psos.h"

/*
 * We shall include the basic platform definitions before defining our own.
 */
#include <bsp.h>
#include <bspfuncs.h>
#include <pna.h>
/*#include <pna_mib.h>*/
#include <psos.h>
#include <board.h>

/* Standard ANSI includes */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <memory.h>

/* POSIX header files */
#include <time.h>

/* name resolution */
#include <rescfg.h>


#include "sys_conf.h"


/***********************************************************
        DEFINITION OF BASIC DATA TYPE AND STRUCTURES
***********************************************************/

/*DH  TBD   these two definitions are OS/Processor dependent, need revisit

/*
 * These macros and inline functions shield the rest of the container from dealing with the byte-order
 * issue caused by different endian types supported by different cpu.
 */
#if      BSP_LITTLE_ENDIAN == YES

    #define  _ANSC_LITTLE_ENDIAN_

#elif    BSP_LITTLE_ENDIAN == NO

#else

/*
 *  stop the compiler
 */
#error   STOP!STOP!STOP! Endian preference must have been defined!

#endif


/*
 * We define some basic data types and structures.
 */
typedef  void                       VOID,       *PVOID;
typedef  char                       *PCHAR,     *LPSTR;
typedef  char                       *PSTR;
typedef  const char                 *PCSTR;
typedef  UCHAR                      *PUCHAR;
typedef  long                       LONG,       *PLONG;
typedef  ULONG                      *PULONG;
typedef  USHORT                     *PUSHORT;
typedef  UCHAR                      BOOLEAN,    *PBOOLEAN;

#ifndef  BOOL
typedef  ULONG                      BOOL;
#endif

/*
 *  DH  need revisit, ANSC wrapper need to be consistent with this
 */
typedef  ULONG                      KERNEL_STATUS,  *PKERNEL_STATUS;
typedef  PVOID                      KERNEL_HANDLE,  *PKERNEL_HANDLE;
typedef  ULONG                      KERNEL_LOCK,    *PKERNEL_LOCK;
typedef  ULONG                      KERNEL_EVENT,   *PKERNEL_EVENT;

#ifndef  __inline
#define  __inline                   inline
#endif

#endif
