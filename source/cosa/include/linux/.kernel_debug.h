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

    module:	kernel_debug.h

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
        Xiong Jiang
        Ding Hua

    ---------------------------------------------------------------

    revision:

        01/19/01    Xuechen Yang    initial revision.
        08/19/03    Xiong Jiang     add debug levels, masks and short macros.
        04/26/2007  removed debug level/mask as this mechanism is in ANSC

**********************************************************************/


#ifndef  _KERNEL_DEBUG_
#define  _KERNEL_DEBUG_


/***********************************************************
        DEFINITION OF BASIC DATA TYPE AND STRUCTURES
***********************************************************/


/***********************************************************
       BASIC OPERATIONS BY MACROS AND INLINE FUNCTIONS
***********************************************************/

#define  KernelAssert(cond)                                                     \
            do                                                                  \
            {                                                                   \
                if (!(cond))                                                    \
                {                                                               \
                    printk                                                      \
                        (                                                       \
                            "ASSERT failure (" #cond ")\n  (line %d in %s)\n",  \
                            __LINE__, __FILE__);                                \
                    BUG();                                                      \
                }                                                               \
            } while(0)


#ifdef   _DEBUG
    #define  KernelTrace(arg...)                    printk(arg)
#else
    #define  KernelTrace(fmt, args...)
#endif

#define  KERNEL_TRACE_PLATFORM_MEMORY               FALSE
#define  KERNEL_TRACE_PLATFORM_TASK                 FALSE


#endif  /* _KERNEL_DEBUG_ */

