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

        OSE 4.2

    ---------------------------------------------------------------

    author:

		Hua Ding

    ---------------------------------------------------------------

    revision:

        07/31/02    initial revision.

**********************************************************************/


#ifndef  _KERNEL_BASE_
#define  _KERNEL_BASE_

#include "kernel_base_custom_ose.h"

/*
 * We shall include the basic platform definitions before defining our own.
 */
#ifdef   USE_OSEDEF_H
#include "osedef.h"
#endif

#include "ose.h"

#include "assert.h"
/*#include "log.h"*/

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "inet.h"
#include "netdb.h"
#include "inet.sig"
#include "ineterr.h"
#include "time.h"

/*
 *  bring in bgi's definitions
 */
#include "macro.h"

/***********************************************************
        DEFINITION OF BASIC DATA TYPE AND STRUCTURES
***********************************************************/
/*
 *  endian definition
 */
#ifdef   LITTLE_ENDIAN

    #define _ANSC_LITTLE_ENDIAN_

#elif    defined(BIG_ENDIAN)

    #define _ANSC_BIG_ENDIAN_

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
typedef  LONG                       *PLONG;
typedef  ULONG                      *PULONG;
typedef  USHORT                     *PUSHORT;
typedef  BOOL                       *PBOOL;
typedef  BOOL                       BOOLEAN,    *PBOOLEAN;

typedef  double                     DOUBLE,     *PDOUBLE;
typedef  float                      FLOAT,      *PFLOAT;
typedef  int                        *PINT;

typedef  ULONG                      KERNEL_STATUS,  *PKERNEL_STATUS;
typedef  PVOID                      KERNEL_HANDLE,  *PKERNEL_HANDLE;

#define  KERNEL_HANDLE_NULL                         NULL

#ifndef  __inline
#define  __inline                   inline
#endif

#define  KERNEL_ARRAY_ZERO_SIZE

#endif
