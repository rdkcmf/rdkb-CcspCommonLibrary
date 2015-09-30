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

        Cisco System  , Inc., 1997 ~ 2007
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines some basic data types and
        structures on a particular platform.

    ---------------------------------------------------------------

    environment:

        platform dependent
        vxworks

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        01/19/01    initial revision.
        14/09/01    adapted to linux by Geng Yang

**********************************************************************/


#ifndef  _KERNEL_BASE_
#define  _KERNEL_BASE_

#include "kernel_base_custom_linux.h"

/*
 * We shall include the basic platform definitions before defining our own.
 */
#define  LINUX
#define  _ANSC_NON_PREEMPTIVE_

#include <linux/module.h>
#include <linux/types.h>
#include <linux/ctype.h>
#include <linux/kernel.h>
/*It doesn't seem the functions defined in this header file are referenced*/
/*#include <linux/tqueue.h>*/
#include <linux/skbuff.h>
#include <linux/fs.h>
#include <linux/netdevice.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/net.h>
#include <linux/in.h>
#include <linux/unistd.h>
#include <linux/string.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/spinlock.h>
#include <linux/slab.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/if_arp.h>
#include <linux/if_packet.h>
#include <linux/posix_types.h>
#include <linux/poll.h>
#include <linux/personality.h>
#include <net/sock.h>
#include <asm/uaccess.h>
#include <asm/system.h>
#include <asm/hardirq.h>
#include <asm/checksum.h>

/***********************************************************
                     TYPE DEFINITIONS
***********************************************************/

#ifndef _ANSC_DEFINED_VOID
    typedef  void                   VOID;
#endif

#ifndef _ANSC_DEFINED_PVOID
    typedef  void                   *PVOID;
#endif

#ifndef _ANSC_DEFINED_CHAR
    typedef  char                   CHAR;
#endif

#ifndef _ANSC_DEFINED_PCHAR
    typedef  char                   *PCHAR;
#endif

#ifndef _ANSC_DEFINED_INT
    typedef  int                    INT;
#endif

#ifndef _ANSC_DEFINED_PSTR
    typedef  char                   *PSTR;
#endif

#ifndef _ANSC_DEFINED_LPSTR
    typedef  char                   *LPSTR;
#endif

#ifndef _ANSC_DEFINED_PCSTR
    typedef  const char             *PCSTR;
#endif

#ifndef _ANSC_DEFINED_UCHAR
    typedef  unsigned char	    	UCHAR;
#endif

#ifndef _ANSC_DEFINED_PUCHAR
    typedef  unsigned char	    	*PUCHAR;
#endif

#ifndef _ANSC_DEFINED_LONG
    typedef  long                   LONG;
#endif

#ifndef _ANSC_DEFINED_PLONG
    typedef  long                   *PLONG;
#endif

#ifndef _ANSC_DEFINED_ULONG
    typedef  unsigned long	    	ULONG;
#endif

#ifndef _ANSC_DEFINED_PULONG
    typedef  unsigned long	    	*PULONG;
#endif

#ifndef _ANSC_DEFINED_USHORT
    typedef  unsigned short	    	USHORT;
#endif

#ifndef _ANSC_DEFINED_PUSHORT
    typedef  unsigned short	    	*PUSHORT;
#endif

#ifndef _ANSC_DEFINED_BOOLEAN
    typedef  UCHAR                  BOOLEAN;
#endif

#ifndef _ANSC_DEFINED_PBOOLEAN
    typedef  UCHAR                  *PBOOLEAN;
#endif

#ifndef _ANSC_DEFINED_BOOL
    typedef  UCHAR	            	BOOL;
#endif

#ifndef _ANSC_DEFINED_PBOOL
    typedef  UCHAR	            	*PBOOL;
#endif

#ifndef _ANSC_DEFINED_bool
    typedef  BOOL	            	bool;
#endif

#ifndef _ANSC_DEFINED_DOUBLE
    typedef  long                   DOUBLE;
#endif

#ifndef _ANSC_DEFINED_PDOUBLE
    typedef  long                   *PDOUBLE;
#endif

#ifndef _ANSC_DEFINED_SHORT
    typedef  short                  SHORT;
#endif

#ifndef _ANSC_DEFINED_PSHORT
    typedef  short                  *PSHORT;
#endif

#ifndef _ANSC_DEFINED_UINT
    typedef  unsigned int           UINT;
#endif

#ifndef _ANSC_DEFINED_PUINT
    typedef  unsigned int           *PUINT;
#endif

#ifndef _ANSC_DEFINED_BYTE
    typedef  unsigned char          BYTE;
#endif

#ifndef _ANSC_DEFINED_PBYTE
    typedef  unsigned char          *PBYTE;
#endif

#ifndef _ANSC_DEFINED_ULONG64
    typedef  ULONG                  ULONG64;
#endif

#ifndef _ANSC_DEFINED_PULONG64
    typedef  ULONG                  *PULONG64;
#endif

#ifndef _ANSC_DEFINED_FALSE
    #define  FALSE                  0
#endif

#ifndef _ANSC_DEFINED_false
    #define  false                  0
#endif

#ifndef _ANSC_DEFINED_TRUE
    #define  TRUE                   1
#endif

#ifndef _ANSC_DEFINED_true
    #define  true                   1
#endif


/***********************************************************
        DEFINITION OF BASIC DATA TYPE AND STRUCTURES
***********************************************************/

#ifndef _ANSC_LINUX_2_6_
    #define  __inline	        	                __inline__
#else
    #define  __inline	        	                __inline__
#endif

#define  __static_inline                            static __inline__
#define  _KERNEL_STATIC_                            static
#define  _KERNEL_INLINE_                            __inline__

/*
 * We define some basic data types and structures.
 */
typedef struct semaphore            KERNEL_LOCK,        *PKERNEL_LOCK;
typedef  spinlock_t		            KERNEL_SPINLOCK,    *PKERNEL_SPINLOCK;
typedef  KERNEL_LOCK			    KERNEL_SEMAPHORE,	*PKERNEL_SEMAPHORE;

typedef struct _KERNEL_EVENT
{
    wait_queue_head_t       wq;
	atomic_t                status;
} KERNEL_EVENT, *PKERNEL_EVENT;


typedef  kmem_cache_t               KERNEL_MEMORY_CACHE, *PKERNEL_MEMORY_CACHE;


#define  ANSC_ZERO_ARRAY_SIZE                       0


#endif

