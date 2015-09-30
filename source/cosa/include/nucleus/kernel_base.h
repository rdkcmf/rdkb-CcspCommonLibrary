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

        platform dependent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        01/19/01    initial revision.

**********************************************************************/


#ifndef  _KERNEL_BASE_
#define  _KERNEL_BASE_

#include  "kernel_base_custom_nucleus.h"

#include  <stdio.h>
#include  "nucleus.h"
#include  "inc\system.h"
#include  "p2port.h"
#include  "inc\ctypes.h"
#include  "h\p2types.h"
#include  "h\p2ibd.h"
#include  "h\p2iptype.h"
#include  "inc\la3_cfg.h"
#include  "p2proto.h"
#include  "PKTBUF.h"
#include  "sw_pkt.h"
#include  "p2ibd.h"
#include  "dev_scc.h"

/*
 * We shall include the basic platform definitions before defining our own.
 */

/***********************************************************
        DEFINITION OF BASIC DATA TYPE AND STRUCTURES
***********************************************************/

/*
 * We define some basic data types and structures.
 */
/* floating types have been disabled here in Nucleus 
typedef  double			DOUBLE,			*PDOUBLE;
typedef  float			FLOAT,			*PFLOAT;
*/
typedef  INT							*PINT;
typedef  unsigned char  			*PUCHAR;
typedef  unsigned short          *PUSHORT;
typedef  CHAR							*PCHAR;
typedef  SIGNED			LONG,			*PLONG;
typedef  UNSIGNED					*PULONG;
typedef  BOOL			BOOLEAN,		*PBOOLEAN, *PBOOL;
typedef  VOID							*PVOID;
typedef  double			DOUBLE,			*PDOUBLE;
typedef  float			FLOAT,			*PFLOAT;
typedef  short			SHORT,			*PSHORT;

typedef  NU_SEMAPHORE	KERNEL_LOCK,   *PKERNEL_LOCK;
typedef  NU_EVENT_GROUP KERNEL_EVENT,  *PKERNEL_EVENT;

#define  ANSC_SYSTEM_TICK			SYSTEM_OS_TIME_PER_TICK

#define  NU_MAX_NAME_SIZE			NU_MAX_NAME


/* FIX ME HERE! Need to initialize the following variable */
extern  PVOID		ghBisga_Memory;
extern  INT			sema_name;
extern  int			gTaskNumber;

#endif

