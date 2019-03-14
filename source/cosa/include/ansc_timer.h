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

    module:	ansc_timer.h

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
        03/16/07    reimplemented in straight C

**********************************************************************/


#ifndef  _ANSC_TIMER_
#define  _ANSC_TIMER_


/***********************************************************
  PLATFORM INDEPENDENT TIMER DESCRIPTOR OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  ANSC_DEF_SCHEDULING_UNIT                   1000    /* 1 second instead of 10 ms   */
#define  ANSC_MIN_SCHEDULING_UNIT                   10      /* the smallest scheduling unit*/
#define  ANSC_TASK_CLEANUP_TIME                     5000

/*
 * Define some const values that will be used in the object descriptor object definition.
 */
#define  ANSC_TIMER_TYPE_PERIODIC                   1
#define  ANSC_TIMER_TYPE_SPORADIC                   2

/*
 * Many extension or feature objects need to perform some background cleaning and maintenance work
 * in any container environment. This type of work can be implemented via separate low-priority
 * tasks or periodic timer-based callback functions, depending on the support of the underlying
 * real-time operating system. From our previous experience, timer callback implementation has more
 * os-support than background task based approach.
 */
typedef  ANSC_STATUS
(*PFN_TDOCO_INVOKE)
    (
        ANSC_HANDLE                 hInvokeContext
    );

typedef  struct
_ANSC_TIMER_DESCRIPTOR
{
    SINGLE_LINK_ENTRY               Linkage;
    ANSC_LOCK                       AccessLock;
    ULONG                           StartTime;
    ULONG                           Timestamp;
    ULONG                           TimerType;
    ULONG                           Interval;
    ULONG                           TimeToFire;
    BOOL                            bScheduled;

    ANSC_HANDLE                     hInvokeContext;
    PFN_TDOCO_INVOKE                Invoke;
}
ANSC_TIMER_DESCRIPTOR,  *PANSC_TIMER_DESCRIPTOR;

#define  ACCESS_ANSC_TIMER_DESCRIPTOR(p)            \
         ACCESS_CONTAINER(p, ANSC_TIMER_DESCRIPTOR, Linkage)


#endif
