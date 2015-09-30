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


#ifndef  _KERNEL_BASE_
#define  _KERNEL_BASE_

#include "kernel_base_custom_ecos.h"

/*
 * We shall include the basic platform definitions before defining our own.
 */
#include <pkgconf/kernel.h>
#include <pkgconf/libc.h>
#include <pkgconf/libm.h>

#include <cyg/infra/cyg_type.h>                     /* Common type definitions and support */
#include <cyg/kernel/kapi.h>

#include <cyg/infra/cyg_ass.h>                      /* Common assertion support */
#include <cyg/infra/diag.h>

#include <stdlib.h>
#include <stdio.h">

#include <network.h>

/***********************************************************
         DEFINITIONS SUPPOSEDLY TO BE IN MAKEFILE
***********************************************************/

#define  _ANSC_SIMULATE_EVENT_

/***********************************************************
                  BASELINE DEFINITIONS
***********************************************************/

#if      (_BYTE_ORDER == _LITTLE_ENDIAN)

    #define  _ANSC_LITTLE_ENDIAN_

#elif    (_BYTE_ORDER == _BIG_ENDIAN)

    #define  _ANSC_BIG_ENDIAN_

#else

    /*
     *  stop the compiler
     */
    #error   STOP!STOP!STOP! Endian preference must have been defined!

#endif

#define  ANSC_ZERO_ARRAY_SIZE                       0

/***********************************************************
        DEFINITION OF BASIC DATA TYPE AND STRUCTURES
***********************************************************/

#define  false                                      FALSE
#define  true                                       TRUE

typedef  CYG_WORD32                 ULONG,      *PULONG;
typedef  CYG_BYTE                   UCHAR,      *PUCHAR;
typedef  CYG_WORD16                 USHORT,     *PUSHORT;
typedef  cyg_bool                   BOOL,       *PBOOL;
typedef  cyg_bool                   BOOLEAN,    *PBOOLEAN;

typedef  cyg_sem_t                  KERNEL_LOCK,      *PKERNEL_LOCK;
typedef  ULONG                      KERNEL_SPINLOCK,  *PKERNEL_SPINLOCK;
typedef  cyg_sem_t                  KERNEL_SEMAPHORE, *PKERNEL_SEMAPHORE;

/*
 *  eCos condition variable doesn't exactly satisfy the need of Event and it explicitly requires a mutex
 *  It can still work, but since simulated event is used anyway, it is not implemented.
 */
typedef  struct
_KERNEL_EVENT
{
    cyg_cond_t                      Cond;
    cyg_mutex_t                     Mutex;
}
KERNEL_EVENT, *PKERNEL_EVENT;

typedef  void                       *PVOID;
typedef  char                       CHAR,       *PCHAR,   *LPSTR;
typedef  char                       *PSTR;
typedef  const char                 *PCSTR;
typedef  long                       LONG,       *PLONG;
typedef  ULONG                      ULONG64,    *PULONG64;

typedef  double                     DOUBLE,     *PDOUBLE;
typedef  float                      FLOAT,      *PFLOAT;
typedef  short                      SHORT,      *PSHORT;
typedef  int                        INT,        *PINT;
typedef  unsigned int               *PUINT;
typedef  unsigned char              BYTE,       *PBYTE;


/***********************************************************
        DEFINITION OF PLATFORM SPECIFIC CONSTANTS
***********************************************************/

#define  KERNEL_MILLISECONDS_PER_TICK               10

#endif

