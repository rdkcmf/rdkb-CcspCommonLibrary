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


#ifndef  _KERNEL_DEBUG_
#define  _KERNEL_DEBUG_


/***********************************************************
            DEFINITION OF DEBUG LEVEL/MASK
***********************************************************/
/*
 * debug Level and Mask definitions.
 */
#define  KERNEL_DBG_LEVEL_DEATH                     1               /*system is going to die*/
#define  KERNEL_DBG_LEVEL_CRITICAL                  2               /*system is unable to perform its functionality*/
#define  KERNEL_DBG_LEVEL_ERROR                     4               /*handled errors*/
#define  KERNEL_DBG_LEVEL_WARNING                   6               /*warning about abnormal situation*/
#define  KERNEL_DBG_LEVEL_TEST                      8               /*for testing */
#define  KERNEL_DBG_LEVEL_TRACE_FLOW                10              /*trace showing the processing flow, could be routine entries*/
#define  KERNEL_DBG_LEVEL_TRACE_ENTRY               11              /*trace for regular routine entries*/
#define  KERNEL_DBG_LEVEL_TRACE_INFO                12              /*informational trace in regular routines*/
#define  KERNEL_DBG_LEVEL_TRACE_TRIVIA_ENTRY        13              /*trace for trivia routine entries*/
#define  KERNEL_DBG_LEVEL_VERBOSE_FLOW              18              /*usually show the main traffic flow, but too verbose*/
#define  KERNEL_DBG_LEVEL_VERBOSE                   24              /*say whatever you like, level 1*/
#define  KERNEL_DBG_LEVEL_VERBOSE2                  26              /*say whatever you like, level 2*/
#define  KERNEL_DBG_LEVEL_DUMPING                   32              /*dumping everything*/

#define  KERNEL_DBG_MASK_ALL                        0xFFFFFFFF

#define  KERNEL_DBG_MASK_LEGACY                     0x00000001      /*for the trace statement without specifying level/mask*/
#define  KERNEL_DBG_MASK_GENERIC                    0x00000002      /*to the one doesn't fall into any other categories*/
#define  KERNEL_DBG_MASK_MEMORY                     0x00000004      /*for memory tracking*/

/***********************************************************
        ROUTE DEBUG LEVEL/MASK DEFINITIONS TO ANSC
***********************************************************/

#define  ANSC_DBG_LEVEL_DEATH                       KERNEL_DBG_LEVEL_DEATH
#define  ANSC_DBG_LEVEL_CRITICAL                    KERNEL_DBG_LEVEL_CRITICAL
#define  ANSC_DBG_LEVEL_ERROR                       KERNEL_DBG_LEVEL_ERROR
#define  ANSC_DBG_LEVEL_WARNING                     KERNEL_DBG_LEVEL_WARNING
#define  ANSC_DBG_LEVEL_TEST                        KERNEL_DBG_LEVEL_TEST
#define  ANSC_DBG_LEVEL_TRACE_FLOW                  KERNEL_DBG_LEVEL_TRACE_FLOW
#define  ANSC_DBG_LEVEL_TRACE_ENTRY                 KERNEL_DBG_LEVEL_TRACE_ENTRY
#define  ANSC_DBG_LEVEL_TRACE_INFO                  KERNEL_DBG_LEVEL_TRACE_INFO
#define  ANSC_DBG_LEVEL_TRACE_TRIVIA_ENTRY          KERNEL_DBG_LEVEL_TRACE_TRIVIA_ENTRY
#define  ANSC_DBG_LEVEL_VERBOSE_FLOW                KERNEL_DBG_LEVEL_VERBOSE_FLOW
#define  ANSC_DBG_LEVEL_VERBOSE                     KERNEL_DBG_LEVEL_VERBOSE
#define  ANSC_DBG_LEVEL_VERBOSE2                    KERNEL_DBG_LEVEL_VERBOSE2
#define  ANSC_DBG_LEVEL_DUMPING                     KERNEL_DBG_LEVEL_DUMPING
                                                   
#define  ANSC_DBG_MASK_ALL                          KERNEL_DBG_MASK_ALL
                                                   
#define  ANSC_DBG_MASK_LEGACY                       KERNEL_DBG_MASK_LEGACY
#define  ANSC_DBG_MASK_GENERIC                      KERNEL_DBG_MASK_GENERIC
#define  ANSC_DBG_MASK_MEMORY                       KERNEL_DBG_MASK_MEMORY

/***********************************************************
       BASIC OPERATIONS BY MACROS AND INLINE FUNCTIONS
***********************************************************/

/*
 *  force ansc to go with debug macros, so that we can gain the control
 */
#define  _DEBUG

#define  DEBUG

#ifdef   DEBUG

    #define  KERNEL_CHECKED_BUILD
    #define  ANSC_CHECKED_BUILD

    #define  KernelTrace
    #define  KernelTrace2

    /*#define  KernelAssert(cond)                     assert(cond)*/
    void ose_assertion_failed(const char *expr, const char *file, unsigned line);
    #define  KernelAssert(exp)                      ( (exp) != 0 ? (void)0 : ose_assertion_failed(#exp, __FILE__, __LINE__) )

#else

    #define  KERNEL_FREE_BUILD
    #define  ANSC_FREE_BUILD

    #define  KernelTrace
    #define  KernelTrace2
    #define  KernelAssert(cond)

#endif

#define  KERNEL_TRACE_PLATFORM_MEMORY               FALSE
#define  KERNEL_TRACE_PLATFORM_TASK                 FALSE

#endif
