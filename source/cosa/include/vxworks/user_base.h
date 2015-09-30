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

    module:	user_base.h

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

        VxWorks 5.4

    ---------------------------------------------------------------

    author:

        Huaxiang Sun
        Hua Ding

    ---------------------------------------------------------------

    revision:

        01/19/01    initial revision.
        09/14/01    adapted to vxworks by Geng Yang

**********************************************************************/


#ifndef  _USER_BASE_
#define  _USER_BASE_

#include "user_base_custom_vxworks.h"

/*
 * We shall include the basic platform definitions before defining our own.
 */
#include "vxWorks.h"

/*#include "config.h"*/

#include "stdLib.h"
#include "stdio.h"
#include "ioLib.h"
#include "assert.h"
#include "strLib.h"
#include "time.h"
#include "inetLib.h"
#include "ftpLib.h"
#include "resolvLib.h"
#include "etherLib.h"
#include "selectLib.h"
#include "sockLib.h"
#include "netinet\ip.h"
#include "netinet/in.h"
#include "net\if_subr.h"
#include "taskLib.h"

#include "sysLib.h"
#include "tickLib.h"

/***********************************************************
         DEFINITIONS SUPPOSEDLY TO BE IN MAKEFILE
***********************************************************/

/*
 *  Event Simulation
 */
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

typedef  SEM_ID                     USER_LOCK,      *PUSER_LOCK;
typedef  USER_LOCK                  USER_SPINLOCK,  *PUSER_SPINLOCK;
typedef  SEM_ID                     USER_EVENT,     *PUSER_EVENT;
typedef  SEM_ID                     USER_SEMAPHORE, *PUSER_SEMAPHORE;

typedef  void                       *PVOID;
typedef  char                       CHAR,           *PCHAR,   *LPSTR;
typedef  char                       *PSTR;
typedef  const char                 *PCSTR;
typedef  UCHAR                      *PUCHAR;
typedef  long                       LONG,       *PLONG;
typedef  ULONG                      *PULONG;
typedef  ULONG                      ULONG64,        *PULONG64;
typedef  USHORT                     *PUSHORT;
typedef  UCHAR                      BOOLEAN,    *PBOOLEAN;

typedef  double                     DOUBLE,     *PDOUBLE;
typedef  float                      FLOAT,      *PFLOAT;
typedef  short                      SHORT,      *PSHORT;
typedef  int                        INT,        *PINT;
typedef  unsigned int               *PUINT;
typedef  BOOL                       *PBOOL;
typedef  unsigned char              BYTE,       *PBYTE;


#endif
