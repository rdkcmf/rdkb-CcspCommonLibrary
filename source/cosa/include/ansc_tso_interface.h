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

    module:	ansc_tso_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros related to timer scheduling operation.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        10/14/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_TSO_INTERFACE_
#define  _ANSC_TSO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "ansc_cover3_interface.h"
#include "ansc_cover3_external_api.h"


/***********************************************************
    PLATFORM INDEPENDENT TIMER SCHEDULER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  ANSC_TSO_DEF_SCHEDULING_UNIT               100     /* 100 milli-second instead of 10 ms */
#define  ANSC_TSO_MIN_SCHEDULING_UNIT               10      /* the smallest scheduling unit      */
#define  ANSC_TSO_TASK_CLEANUP_TIME                 5000

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_TSO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TSO_SCHEDULE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TSO_START)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TSO_STOP)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TSO_REGISTER_TIMER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTimerDescriptor
    );

typedef  ANSC_STATUS
(*PFN_TSO_CANCEL_TIMER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTimerDescriptor
    );

typedef  ANSC_STATUS
(*PFN_TSO_SCHEDULE_TIMER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTimerDescriptor
    );

/*
 * Most policy configuration information is stored and transmitted in xml format. Compared to our
 * proprietary al_store based configuration format, xml not only provides more flexible and
 * powerful hierarchical structure, it can also be translated into other data format, such as html,
 * very easily via XSLT (eXtensible Stylesheet Language Transformation). This Xml Parser Object
 * takes a xml-based data stream as input, and exposes a read/write interface similar to al_store.
 */
#define  ANSC_TIMER_SCHEDULER_CLASS_CONTENT                                                 \
    /* duplication of the base object class content */                                      \
    ANSCCO_VER3_CLASS_CONTENT                                                               \
    /* start of object class content */                                                     \
    ULONG                           SchedulingUnit;                                         \
    ANSC_HANDLE                     hCurInvokedTdo;                                         \
    BOOL                            bStarted;                                               \
                                                                                            \
    ANSC_EVENT                      SchedulerEvent;                                         \
    ANSC_TS_LOCK                    SyncTsLock;                                             \
    QUEUE_HEADER                    TdoQueue;                                               \
    ANSC_SPINLOCK                   TdoQueueSpinLock;                                       \
                                                                                            \
    PFN_TSO_RESET                   Reset;                                                  \
    PFN_TSO_SCHEDULE                ScheduleTask;                                           \
    PFN_TSO_START                   Start;                                                  \
    PFN_TSO_STOP                    Stop;                                                   \
                                                                                            \
    PFN_TSO_REGISTER_TIMER          RegisterTimer;                                          \
    PFN_TSO_CANCEL_TIMER            CancelTimer;                                            \
    PFN_TSO_SCHEDULE_TIMER          ScheduleTimer;                                          \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_TIMER_SCHEDULER_OBJECT
{
    ANSC_TIMER_SCHEDULER_CLASS_CONTENT
}
ANSC_TIMER_SCHEDULER_OBJECT,  *PANSC_TIMER_SCHEDULER_OBJECT;

#define  ACCESS_ANSC_TIMER_SCHEDULER_OBJECT(p)      \
         ACCESS_CONTAINER(p, ANSC_TIMER_SCHEDULER_OBJECT, Linkage)


/***********************************************************
  PLATFORM INDEPENDENT TIMER DESCRIPTOR OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object descriptor object definition.
 */
#define  ANSC_TIMER_TYPE_PERIODIC                   1
#define  ANSC_TIMER_TYPE_SPORADIC                   2

#define  ANSC_TIMER_MODE_asyncStop                  0x00000001

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_TDO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TDO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ULONG
(*PFN_TDO_GET_ULONG)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TDO_SET_ULONG)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulValue
    );

typedef  BOOL
(*PFN_TDO_GET_BOOL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TDO_SET_BOOL)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bValue
    );

typedef  ANSC_STATUS
(*PFN_TDO_ACQUIRE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TDO_RELEASE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TDO_START)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TDO_STOP)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TDO_INVOKE)
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
#define  ANSC_TIMER_DESCRIPTOR_CLASS_CONTENT_NO_PROF                                        \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hScheduler;                                             \
    ANSC_HANDLE                     hClient;                                                \
    ANSC_SPINLOCK                   AccessSpinLock;                                         \
    ULONG                           StartTime;                                              \
    ULONG                           Timestamp;                                              \
    ULONG                           TimerType;                                              \
    ULONG                           TimerMode;                                              \
    ULONG                           Interval;                                               \
    ULONG                           TimeToFire;                                             \
    ULONG                           CallCounter;                                            \
    BOOL                            bScheduled;                                             \
                                                                                            \
    PFN_TDO_GET_CONTEXT             GetScheduler;                                           \
    PFN_TDO_SET_CONTEXT             SetScheduler;                                           \
    PFN_TDO_GET_CONTEXT             GetClient;                                              \
    PFN_TDO_SET_CONTEXT             SetClient;                                              \
    PFN_TDO_GET_ULONG               GetStartTime;                                           \
    PFN_TDO_SET_ULONG               SetStartTime;                                           \
    PFN_TDO_GET_ULONG               GetTimestamp;                                           \
    PFN_TDO_SET_ULONG               SetTimestamp;                                           \
    PFN_TDO_GET_ULONG               GetTimerType;                                           \
    PFN_TDO_SET_ULONG               SetTimerType;                                           \
    PFN_TDO_GET_ULONG               GetTimerMode;                                           \
    PFN_TDO_SET_ULONG               SetTimerMode;                                           \
    PFN_TDO_GET_ULONG               GetInterval;                                            \
    PFN_TDO_SET_ULONG               SetInterval;                                            \
    PFN_TDO_GET_ULONG               GetTimeToFire;                                          \
    PFN_TDO_SET_ULONG               SetTimeToFire;                                          \
    PFN_TDO_GET_ULONG               GetCallCounter;                                         \
    PFN_TDO_SET_ULONG               SetCallCounter;                                         \
    PFN_TDO_GET_BOOL                GetState;                                               \
    PFN_TDO_SET_BOOL                SetState;                                               \
                                                                                            \
    PFN_TDO_ACQUIRE                 AcquireAccess;                                          \
    PFN_TDO_RELEASE                 ReleaseAccess;                                          \
    PFN_TDO_START                   Start;                                                  \
    PFN_TDO_STOP                    Stop;                                                   \
    PFN_TDO_INVOKE                  Invoke;                                                 \
    /* end of object class content */                                                       \

#ifndef _ANSC_TDO_PROFILING_

    #define ANSC_TIMER_DESCRIPTOR_CLASS_CONTENT                     \
                ANSC_TIMER_DESCRIPTOR_CLASS_CONTENT_NO_PROF

#else

    #define ANSC_TIMER_DESCRIPTOR_CLASS_CONTENT                     \
                ANSC_TIMER_DESCRIPTOR_CLASS_CONTENT_NO_PROF         \
                DOUBLE_LINK_ENTRY               TdoProfileLink;     \
                char                            TdoInitFile[32];    \
                int                             TdoInitLine;        \
                ULONG                           TdoFlags;           \
                ULONG                           CurrTimestamp;      \
                ULONG                           CurrElapsedTime;    \
                ULONG                           LongestTime;        \
                ULONG                           TotalHits;          \
                ULONG                           TotalHitTime;       \
                

#endif /* _ANSC_TDO_PROFILING_ */

typedef  struct
_ANSC_TIMER_DESCRIPTOR_OBJECT
{
    ANSC_TIMER_DESCRIPTOR_CLASS_CONTENT
}
ANSC_TIMER_DESCRIPTOR_OBJECT,  *PANSC_TIMER_DESCRIPTOR_OBJECT;

#define  ACCESS_ANSC_TIMER_DESCRIPTOR_OBJECT(p)     \
         ACCESS_CONTAINER(p, ANSC_TIMER_DESCRIPTOR_OBJECT, Linkage)


/***********************************************************
     CLIENT OBJECT DEFINITION FOR ANSC TIMER DESCRIPTOR
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_TDOCO_INVOKE)
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
_ANSC_TDO_CLIENT_OBJECT
{
    ANSC_HANDLE                     hClientContext;

    PFN_TDOCO_INVOKE                Invoke;
}
ANSC_TDO_CLIENT_OBJECT,  *PANSC_TDO_CLIENT_OBJECT;


#endif
