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

    module: user_base.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines some basic data types and
        structures on a particular platform.

    ---------------------------------------------------------------

    environment:

        platform dependent
        Linux user mode

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Geng Yang
        Ying Lou

    ---------------------------------------------------------------

    revision:

        01/19/01    initial revision.
        09/14/01    adapted to linux kernel by Geng Yang
        04/03/02    adapted to linux user by Ying Lou

**********************************************************************/


#ifndef  _USER_BASE_
#define  _USER_BASE_

#include "user_base_custom_linux.h"

/*
 * We shall include the basic platform definitions before defining our own.
 */
#define  LINUX
/* #define  _ANSC_SIMULATE_EVENT_ */

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/ipc.h>
#ifdef _BUILD_ANDROID
#include <linux/sem.h>
#include <linux/shm.h>
#else
#include <sys/sem.h>
#include <sys/shm.h>
#endif
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdarg.h>
#if !defined(_ANSC_LINUX_NO_NCURSES)
    /* for console apps */
    #include <ncurses.h>
#endif

/***********************************************************
        DEFINITION OF BASIC DATA TYPE AND STRUCTURES
***********************************************************/

#define  __static_inline                        static __inline__
#define  _USER_STATIC_                          static
#define  _USER_INLINE_                          __inline__

/*
 * We define some basic data types and structures.
 */
typedef  pthread_mutex_t                        USER_LOCK, *PUSER_LOCK;
typedef  sem_t                                  USER_SEMAPHORE,  *PUSER_SEMAPHORE;
typedef  USER_LOCK                              USER_SPINLOCK;

typedef  void                       VOID,           *PVOID;
typedef  char                       CHAR,           *PCHAR,   *LPSTR;
typedef  int                        INT,            *PINT;
typedef  unsigned int               UINT,           *PUINT;
typedef  char                       *PSTR;
typedef  const char                 *PCSTR;
typedef  unsigned char              BYTE,           * PBYTE;
typedef  unsigned char              UCHAR,          *PUCHAR;
typedef  long                       LONG,           *PLONG;
typedef  unsigned long              ULONG,          *PULONG;
typedef  short                      SHORT,          *PSHORT;
typedef  unsigned short             USHORT,         *PUSHORT;
typedef  double                     DOUBLE,         *PDOUBLE;
typedef  float                      FLOAT,          *PFLOAT;
typedef  UCHAR                      BOOLEAN,        *PBOOLEAN;
typedef  UCHAR                      BOOL,           *PBOOL;
typedef  ULONG                      ULONG64,        *PULONG64;
/*typedef             int                 XSKT_SOCKET,            *PXSKT_SOCKET;*/

/*
#ifndef  bool
typedef  UCHAR bool;
#endif
 */
#ifndef FALSE
#define  FALSE                  0
#endif
#define  false                  0
#ifndef TRUE
#define  TRUE                   1
#endif
#define  true                   1

extern key_t pubEventKey;

#define  ANSC_ZERO_ARRAY_SIZE                       0

#ifndef _BUILD_ANDROID
union semun
{
    int                             val;
    struct semid_ds*                buf;
    unsigned short int*             array;
    struct seminfo *                __buf;
};

typedef struct _al_event
{
    int                             eventId;
    key_t                           keyId;
} USER_EVENT, *PUSER_EVENT;

typedef struct _sem_event
{
    int                             inited;
    sem_t                           sem;
} SEM_EVENT, *PSEM_EVENT;
#else
typedef struct _al_event
{
    pthread_mutex_t msg_mutex;
    pthread_cond_t  msg_threshold_cv;
} USER_EVENT, *PUSER_EVENT , SEM_EVENT, *PSEM_EVENT;
#endif


typedef  struct
_USER_MEMORY_CACHE
{
    ULONG                           ulBlockSize;
}
USER_MEMORY_CACHE,  *PUSER_MEMORY_CACHE;


#ifdef ARCH_x86um
#include "linux_x86um.h"
#endif

#endif

