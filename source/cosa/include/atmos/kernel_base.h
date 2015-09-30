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

        Cisco System  , Inc., 1997 ~ 2002
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines some basic data types and
        structures on a particular platform.

    ---------------------------------------------------------------

    environment:

        atmos 7.1 or later

    ---------------------------------------------------------------

    author:

        Xuechen Yang
		Hua Ding

    ---------------------------------------------------------------

    revision:

        01/19/01    initial revision.
        04/24/02    atmos port

**********************************************************************/


#ifndef  _KERNEL_BASE_
#define  _KERNEL_BASE_

#include "kernel_base_custom_atmos.h"

/*
 * We shall include the basic platform definitions before defining our own.
 */
#include "config/al_common_atmos.h"                 /* Includes config.h */
#include "kernel.h"
#include "errno.h"
#include "messages.h"
/* Standard ANSI includes */
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "assert.h"

/* Unix compatibility */
#include "netinet/in.h"
#include "arpa/inet.h"
#include "net/if.h"

/* From tcp_ip module */
#include "ipfilter.h"

#include "xx.ext"
#include "std.h"

/*
 *  for regular dynamic memory allocations/deallocations
 */
#include "pool.h"

/* Misc. */
#include "command.h"
#include "timelib.h"
#include "time.h"

/***********************************************************
                  BASELINE DEFINITIONS
***********************************************************/

#define  ANSC_ZERO_ARRAY_SIZE                       0

/***********************************************************
        DEFINITION OF BASIC DATA TYPE AND STRUCTURES
***********************************************************/

/*
 * We define some basic data types and structures.
 */
typedef  void                       VOID,       *PVOID;
typedef  char                       CHAR,       *PCHAR,   *LPSTR;
typedef  char                       *PSTR;
typedef  const char                 *PCSTR;
typedef  unsigned char              UCHAR,      *PUCHAR;
typedef  double                     DOUBLE,     *PDOUBLE;
typedef  float                      FLOAT,      *PFLOAT;
typedef  short                      SHORT,      *PSHORT;
typedef  int                        INT,        *PINT;
typedef  long                       LONG,       *PLONG;
typedef  unsigned long              ULONG,      *PULONG;
typedef  unsigned short             USHORT,     *PUSHORT;
typedef  UCHAR                      BOOLEAN,    *PBOOLEAN;
typedef  BOOL                       *PBOOL;
typedef  unsigned int               UINT,       *PUINT;
typedef  BYTE                       *PBYTE;

/*
 *  DH  need revisit, ANSC wrapper need to be consistent with this
 */
typedef  ULONG                      KERNEL_STATUS,  *PKERNEL_STATUS;
typedef  PVOID                      KERNEL_HANDLE,  *PKERNEL_HANDLE;

#define  KERNEL_HANDLE_NULL                         NULL

#ifndef  __inline
    #define  __inline                   inline
#endif


/***********************************************************
          DEFINITIONS FOR SINGLY LINKED LIST
***********************************************************/

typedef struct
_KERNEL_SLIST_ENTRY
{
    struct _KERNEL_SLIST_ENTRY*     Next;
}
KERNEL_SLIST_ENTRY, *PKERNEL_SLIST_ENTRY;

typedef struct
_KERNEL_SLIST_HEADER
{
    KERNEL_SLIST_ENTRY              Next;
    KERNEL_SLIST_ENTRY              Last;
}
KERNEL_SLIST_HEADER, *PKERNEL_SLIST_HEADER;

#define KernelSListInitializeHeader(SListHeader)                    \
        {                                                           \
            (SListHeader)->Next.Next = NULL;                        \
            (SListHeader)->Last.Next = NULL;                        \
        }

#define KernelSListPushEntry(SListHeader, Entry)                    \
        {                                                           \
            PKERNEL_SLIST_ENTRY     LastEntry;                      \
                                                                    \
            LastEntry     = (SListHeader)->Last.Next;               \
            (Entry)->Next = NULL;                                   \
                                                                    \
            if ( LastEntry == NULL )                                \
            {                                                       \
                (SListHeader)->Next.Next = (Entry);                 \
            }                                                       \
            else                                                    \
            {                                                       \
                LastEntry->Next = (Entry);                          \
            }                                                       \
                                                                    \
            (SListHeader)->Last.Next = (Entry);                     \
        }


#endif
