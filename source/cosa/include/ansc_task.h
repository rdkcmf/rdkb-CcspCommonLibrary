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

    module:	ansc_task.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros related to task creation, removal and
        management. Instead of defining everything in a single file,
        we simply route every function and macro to the
        corresponding platform- dependent prototype definition.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        01/01/01    initial revision.
        01/19/01    restructure the container object, add and
                    remove a few things
        04/01/01    add user mode wrapper functions
        10/06/01    separated from the ansc platform itself, one
                    for each specific platform

**********************************************************************/


#ifndef  _ANSC_TASK_
#define  _ANSC_TASK_

#include "ansc_wrapper_base.h"
#include "ansc_common_structures.h"

/***********************************************************
            FUNCTION AND MACRO PROTOTYPE ROUTING
***********************************************************/

/*
 * According to the current platform definition, we route the definition to the corresponding
 * header files.
 */
#define  ANSC_TASK_POOL_TYPE_STATIC                 1
#define  ANSC_TASK_POOL_TYPE_DYNAMIC                2

#define  ANSC_TASK_SLEEP_INTERVAL                   10
#define  ANSC_TASK_CLEAN_INTERVAL                   200
#define  ANSC_TASK_BREAK_INTERVAL                   20

#define  ANSC_TASK_RECORD_TABLE_SIZE                131

#define  AnscTaskRelinquish()                       AnscSleep(0)



    #define  AnscGetProcessId                       UserGetProcessId
    #define  AnscGetCurTaskHandle                   UserGetCurTaskHandle
    #define  AnscCreateTask                         UserCreateTask
    #define  AnscDeleteTask                         UserDeleteTask
    #define  AnscEnterTask                          UserEnterTask
    #define  AnscLeaveTask                          UserLeaveTask
    #define  AnscKillTask                           UserKillTask

    #define  AnscActiveTaskRoutine                  UserTaskRoutine
    #define  AnscPoolableTaskRoutine                UserPoolableTaskRoutine
    #define  AnscJanitorTaskRoutine                 UserJanitorTaskRoutine
    #define  ANSC_DEFAULT_TASK_STACK_SIZE           USER_DEFAULT_TASK_STACK_SIZE
    #define  ANSC_DEFAULT_TASK_PRIORITY             USER_DEFAULT_TASK_PRIORITY
    #define  ANSC_USE_TASK_POOL_MANAGER             USER_USE_TASK_POOL_MANAGER
    #define  ANSC_USE_TASK_POOL_JANITOR             USER_USE_TASK_POOL_JANITOR
    #define  ANSC_CUR_TASK_POOL_TYPE                USER_TASK_POOL_TYPE
    #define  ANSC_MAX_TASK_POOL_SIZE                USER_MAX_TASK_POOL_SIZE

    #define  ANSC_TASK_PRIORITY_HIGHEST             USER_TASK_PRIORITY_HIGHEST
    #define  ANSC_TASK_PRIORITY_HIGH                USER_TASK_PRIORITY_HIGH
    #define  ANSC_TASK_PRIORITY_NORMAL              USER_TASK_PRIORITY_NORMAL
    #define  ANSC_TASK_PRIORITY_LOW                 USER_TASK_PRIORITY_LOW
    #define  ANSC_TASK_PRIORITY_LOWEST              USER_TASK_PRIORITY_LOWEST



/***********************************************************
    DEFINITION OF MULTI-TASK RELATED WRAPPER FUNCTIONS
***********************************************************/

/*
 * There're many places in the embedded programming need multi-task support. For example, session
 * table maintenance, traffic scheduling and socket operation all require separate threads. While
 * different operating systems have different multi-task support, we're trying to provide a unified
 * programming interface to our service delivery platform.
 */
#define  ANSC_TASK_SCRATCH_PAD_SIZE1                64
#define  ANSC_TASK_SCRATCH_PAD_SIZE2                320
#define  ANSC_TASK_SUO_TABLE_SIZE                   16

typedef  ANSC_STATUS
(*PFN_ANSC_TASK_ROUTINE)
    (
        ANSC_HANDLE                 hTaskContext
    );

typedef  struct
_ANSC_TASK_RECORD
{
    SINGLE_LINK_ENTRY               Linkage;
    ULONG                           RefCount;
#ifdef  _ANSC_DEBUG_TASK_
    int volatile                    bEnabled;
    int volatile                    bCaptured;
    char                            CreateFile[32];
    int                             CreateLine;
#endif /* _ANSC_DEBUG_TASK_ */
    char                            Name[ANSC_OBJECT_NAME_SIZE];
    BOOL                            bAnscOwned;
    BOOL                            bSuspended;
    ULONG                           Parent;
    ULONG                           Handle;
    ANSC_SPINLOCK                   AccessSpinLock;
    PFN_ANSC_TASK_ROUTINE           Routine;
    ANSC_HANDLE                     hContext;

    ULONG                           GeneralError;
    ULONG                           SsocketError;
    char                            ScratchPad[ANSC_TASK_SCRATCH_PAD_SIZE1];
    char                            SsocketPad[ANSC_TASK_SCRATCH_PAD_SIZE2];

    QUEUE_HEADER                    SuoTable  [ANSC_TASK_SUO_TABLE_SIZE   ];
}
ANSC_TASK_RECORD,  *PANSC_TASK_RECORD;

#define  ACCESS_ANSC_TASK_RECORD(p)                 \
         ACCESS_CONTAINER(p, ANSC_TASK_RECORD, Linkage)

PANSC_TASK_RECORD
AnscAllocateTaskRecord1
    (
        PCSTR                       pcszName
    );

void
AnscFreeTaskRecord
    (
        PANSC_TASK_RECORD           pTaskRecord
    );

#define  AnscAllocateTaskRecord(name, task_record)  do { task_record = AnscAllocateTaskRecord1(name); }while(0)

#define  AnscTaskGetGeneralError(tr)                ( tr->GeneralError )
#define  AnscTaskGetSsocketError(tr)                ( tr->SsocketError )
#define  AnscTaskGetScratchPad(tr)                  ( tr->ScratchPad   )
#define  AnscTaskGetSsocketPad(tr)                  ( tr->SsocketPad   )

#define  AnscTaskSetGeneralError(tr, code)          ( tr->GeneralError = code )
#define  AnscTaskSetSsocketError(tr, code)          ( tr->SsocketError = code )

/*
 * It's not enough to just maintain one static scratch pad for all modules to share. Unlike other
 * similar situations, such as the layered info pad in packet descriptor, the execution within the
 * task may involve multiple modules. We use following data structure to encapsulate the module-
 * specific information. Note that ANSC is NOT responsible for releasing the memory allocated and
 * associated with each storage unit, so the owner must explicitly remove them.
 */
typedef  struct
_ANSC_TASK_STORAGE_UNIT
{
    SINGLE_LINK_ENTRY               Linkage;
    ULONG                           HashIndex;
    char                            Name[ANSC_OBJECT_NAME_SIZE];
    ANSC_HANDLE                     hSuoContext;
}
ANSC_TASK_STORAGE_UNIT,  *PANSC_TASK_STORAGE_UNIT;

#define  ACCESS_ANSC_TASK_STORAGE_UNIT(p)           \
         ACCESS_CONTAINER(p, ANSC_TASK_STORAGE_UNIT, Linkage)


#endif
