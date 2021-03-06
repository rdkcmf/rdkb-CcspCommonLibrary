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


#ifndef  _USER_BASE_
#define  _USER_BASE_

#include "user_base_custom_windowsnt.h"

/*
 * We shall include the basic platform definitions before defining our own.
 */
#define  WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <windows.h>
#include <process.h>
#include <stddef.h>
#include <stdlib.h>
#include <conio.h>
#include <io.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <assert.h>


/***********************************************************
        DEFINITION OF BASIC DATA TYPE AND STRUCTURES
***********************************************************/

#define  _USER_STATIC_                          static
#define  _USER_INLINE_                          __inline

/*
 * We define some basic data types and structures.
 */
typedef  double                     DOUBLE,         *PDOUBLE;
typedef  float                      FLOAT,          *PFLOAT;
typedef  short                      SHORT,          *PSHORT;
typedef  int                        INT,            *PINT;
typedef  long                       LONG,           *PLONG;

typedef  HANDLE                     USER_LOCK,      *PUSER_LOCK;

#ifdef  _ANSC_WIN32_USE_MUTEX_AS_SPINLOCK_

    typedef  HANDLE  USER_SPINLOCK,   *PUSER_SPINLOCK;

#else /*_ANSC_WIN32_USE_MUTEX_AS_SPINLOCK_ */

    typedef struct _USER_SPINLOCK
    {
        long        lLock;
    }
    USER_SPINLOCK,   *PUSER_SPINLOCK;

#endif /* _ANSC_WIN32_USE_MUTEX_AS_SPINLOCK_ */

typedef  HANDLE                     USER_SEMAPHORE, *PUSER_SEMAPHORE;
typedef  HANDLE                     USER_EVENT,     *PUSER_EVENT;

typedef  struct
_USER_MEMORY_CACHE
{
    ULONG                           ulBlockSize;
}
USER_MEMORY_CACHE,  *PUSER_MEMORY_CACHE;


#define  ANSC_ZERO_ARRAY_SIZE                       0
#define  false                                      FALSE
#define  true                                       TRUE


#endif

