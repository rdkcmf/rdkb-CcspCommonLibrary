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

    module:	user_task.h

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
        10/06/01    separated from the ansc platform itself, one
                    for each specific platform

**********************************************************************/


#ifndef  _USER_TASK_
#define  _USER_TASK_


/***********************************************************
        DEFINITION OF BASIC DATA TYPE AND STRUCTURES
***********************************************************/

#define  UserGetProcessId()                         GetCurrentProcessId()
#define  UserGetCurTaskHandle()                     GetCurrentThreadId()
#define  UserCreateTask(f, s, p, h, n)              _beginthread(f, s, (void*)h)
#define  UserDeleteTask(h)
#define  UserEnterTask(p)
#define  UserLeaveTask()
#define  UserKillTask(h)

#define  UserTaskRoutine                            AnscTaskRoutine1
#define  UserPoolableTaskRoutine                    AnscTaskRoutine2
#define  UserJanitorTaskRoutine                     AnscTaskJanitorRoutine
#define  USER_DEFAULT_TASK_STACK_SIZE               0
#define  USER_DEFAULT_TASK_PRIORITY                 50
#define  USER_USE_TASK_POOL_MANAGER                 FALSE
#define  USER_USE_TASK_POOL_JANITOR                 FALSE
#define  USER_TASK_POOL_TYPE                        ANSC_TASK_POOL_TYPE_DYNAMIC
#define  USER_MAX_TASK_POOL_SIZE                    32

#define  USER_TASK_PRIORITY_HIGHEST                 25
#define  USER_TASK_PRIORITY_HIGH                    20
#define  USER_TASK_PRIORITY_NORMAL                  15
#define  USER_TASK_PRIORITY_LOW                     10
#define  USER_TASK_PRIORITY_LOWEST                  5


#endif
