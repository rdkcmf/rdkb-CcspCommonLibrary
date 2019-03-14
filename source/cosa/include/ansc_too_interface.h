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

    module:	ansc_too_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Task Organizer Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        12/04/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_TOO_INTERFACE_
#define  _ANSC_TOO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "ansc_time.h"


/***********************************************************
    PLATFORM INDEPENDENT TASK ORGANIZER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  ANSC_TOO_SCHEDULING_INTERVAL               30 * 1000       /* in milliseconds */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_TOO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TOO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TOO_START)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TOO_STOP)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TOO_INVOKE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TOO_REGISTER_TASK)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTaskEntry
    );

typedef  ANSC_STATUS
(*PFN_TOO_CANCEL_TASK)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTaskEntry
    );

typedef  ANSC_STATUS
(*PFN_TOO_SCHEDULE_TASK)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTaskEntry
    );

typedef  ANSC_STATUS
(*PFN_TOO_PREPARE_TASK)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTaskEntry
    );

/*
 * Most policy configuration information is stored and transmitted in xml format. Compared to our
 * proprietary al_store based configuration format, xml not only provides more flexible and
 * powerful hierarchical structure, it can also be translated into other data format, such as html,
 * very easily via XSLT (eXtensible Stylesheet Language Transformation). This Xml Parser Object
 * takes a xml-based data stream as input, and exposes a read/write interface similar to al_store.
 */
#define  ANSC_TASK_ORGANIZER_CLASS_CONTENT                                                  \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hWallClock;                                             \
    ANSC_HANDLE                     hSchedulingTimer;                                       \
    ANSC_HANDLE                     hTimerClient;                                           \
    BOOL                            bStarted;                                               \
                                                                                            \
    QUEUE_HEADER                    TeoQueue;                                               \
    ANSC_LOCK                       TeoQueueLock;                                           \
                                                                                            \
    PFN_TOO_GET_CONTEXT             GetWallClock;                                           \
    PFN_TOO_RESET                   Reset;                                                  \
                                                                                            \
    PFN_TOO_START                   Start;                                                  \
    PFN_TOO_STOP                    Stop;                                                   \
    PFN_TOO_INVOKE                  Invoke;                                                 \
                                                                                            \
    PFN_TOO_REGISTER_TASK           RegisterTask;                                           \
    PFN_TOO_CANCEL_TASK             CancelTask;                                             \
    PFN_TOO_SCHEDULE_TASK           ScheduleTask;                                           \
    PFN_TOO_PREPARE_TASK            PrepareTask;                                            \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_TASK_ORGANIZER_OBJECT
{
    ANSC_TASK_ORGANIZER_CLASS_CONTENT
}
ANSC_TASK_ORGANIZER_OBJECT,  *PANSC_TASK_ORGANIZER_OBJECT;

#define  ACCESS_ANSC_TASK_ORGANIZER_OBJECT(p)       \
         ACCESS_CONTAINER(p, ANSC_TASK_ORGANIZER_OBJECT, Linkage)


/***********************************************************
  PLATFORM INDEPENDENT TASK DESCRIPTOR OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object descriptor object definition.
 */
#define  ANSC_TASK_TYPE_ONCE                        1
#define  ANSC_TASK_TYPE_YEARLY                      2
#define  ANSC_TASK_TYPE_MONTHLY                     3
#define  ANSC_TASK_TYPE_WEEKLY                      4
#define  ANSC_TASK_TYPE_DAILY                       5
#define  ANSC_TASK_TYPE_HOURLY                      6

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_TEO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TEO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_STATUS
(*PFN_TEO_GET_TIME)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTime
    );

typedef  ANSC_STATUS
(*PFN_TEO_SET_TIME)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTime
    );

typedef  ULONG
(*PFN_TEO_GET_ULONG)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TEO_SET_ULONG)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulValue
    );

typedef  BOOL
(*PFN_TEO_GET_BOOL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TEO_SET_BOOL)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bValue
    );

typedef  ANSC_STATUS
(*PFN_TEO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TEO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TEO_INVOKE)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * Many extension or feature objects need to perform some background cleaning and maintenance work
 * in any container environment. This type of work can be implemented via separate low-priority
 * tasks or periodic timer-based callback functions, depending on the support of the underlying
 * real-time operating system. From our previous experience, timer callback implementation has more
 * os-support than background task based approach.
 */
#define  ANSC_TASK_ENTRY_CLASS_CONTENT                                                      \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hOrganizer;                                             \
    ANSC_HANDLE                     hClient;                                                \
    ANSC_LOCK                       AccessLock;                                             \
    ANSC_UNIVERSAL_TIME             WakeUpTime;                                             \
    ULONG                           TaskType;                                               \
    ULONG                           NextInvokeTime;                                         \
    ULONG                           CallCounter;                                            \
    BOOL                            bScheduled;                                             \
                                                                                            \
    PFN_TEO_GET_CONTEXT             GetOrganizer;                                           \
    PFN_TEO_SET_CONTEXT             SetOrganizer;                                           \
    PFN_TEO_GET_CONTEXT             GetClient;                                              \
    PFN_TEO_SET_CONTEXT             SetClient;                                              \
    PFN_TEO_GET_TIME                GetWakeUpTime;                                          \
    PFN_TEO_SET_TIME                SetWakeUpTime;                                          \
    PFN_TEO_GET_ULONG               GetTaskType;                                            \
    PFN_TEO_SET_ULONG               SetTaskType;                                            \
    PFN_TEO_GET_ULONG               GetNextInvokeTime;                                      \
    PFN_TEO_SET_ULONG               SetNextInvokeTime;                                      \
    PFN_TEO_GET_ULONG               GetCallCounter;                                         \
    PFN_TEO_SET_ULONG               SetCallCounter;                                         \
    PFN_TEO_GET_BOOL                GetState;                                               \
    PFN_TEO_SET_BOOL                SetState;                                               \
                                                                                            \
    PFN_TEO_ENGAGE                  Engage;                                                 \
    PFN_TEO_CANCEL                  Cancel;                                                 \
    PFN_TEO_INVOKE                  Invoke;                                                 \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_TASK_ENTRY_OBJECT
{
    ANSC_TASK_ENTRY_CLASS_CONTENT
}
ANSC_TASK_ENTRY_OBJECT,  *PANSC_TASK_ENTRY_OBJECT;

#define  ACCESS_ANSC_TASK_ENTRY_OBJECT(p)           \
         ACCESS_CONTAINER(p, ANSC_TASK_ENTRY_OBJECT, Linkage)


/***********************************************************
     CLIENT OBJECT DEFINITION FOR ANSC TASK DESCRIPTOR
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_TEOCO_INVOKE)
    (
        ANSC_HANDLE                 hClient
    );

/*
 * Unlike the previous version of VPCom, in which the wrapper is part of the container, the
 * container is now 100% portable for different embedded platforms. The os-specific wrapper has to
 * be rewritten for each platform. The wrapper creates this wrapper object during initialization
 * and passes it to container.
 */
typedef  struct
_ANSC_TEO_CLIENT_OBJECT
{
    ANSC_HANDLE                     hClientContext;

    PFN_TEOCO_INVOKE                Invoke;
}
ANSC_TEO_CLIENT_OBJECT,  *PANSC_TEO_CLIENT_OBJECT;


#endif
