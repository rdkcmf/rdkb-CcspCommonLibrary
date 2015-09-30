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

    module:	kernel_time.h

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

        01/24/01    initial revision.

**********************************************************************/


#ifndef  _KERNEL_TIME_
#define  _KERNEL_TIME_


/***********************************************************
        DEFINITION OF BASIC DATA TYPE AND STRUCTURES
***********************************************************/

/*
 * Following data structure is used by objects to require time/date information from either the
 * underlying operationg system, or from the utility objects.
 */
typedef  struct
_KERNEL_SYSTEM_TIME
{
    BOOL                            bDayLightSaving;
    USHORT                          Year;
    USHORT                          Month;
    USHORT                          DayOfMonth;
    USHORT                          DayOfWeek;
    USHORT                          Hour;
    USHORT                          Minute;
    USHORT                          Second;
    USHORT                          MilliSecond;
}
KERNEL_SYSTEM_TIME,  *PKERNEL_SYSTEM_TIME;

#define  KERNEL_TIME_SOURCE_SYS                       TRUE
#define  KERNEL_TIME_SOURCE_NTP                       FALSE

/* This has to be declared somewhere else */
extern wait_queue_head_t ALCWVPN_WQ;

/***********************************************************
       BASIC OPERATIONS BY MACROS AND INLINE FUNCTIONS
***********************************************************/

__static_inline VOID
KernelSleepInMilliSeconds(ULONG milliSecond)
{
	current->state = TASK_INTERRUPTIBLE;
	interruptible_sleep_on_timeout(&ALCWVPN_WQ, milliSecond * HZ / 1000);
	current->state = TASK_RUNNING;
}

__static_inline  ULONG
KernelGetTickInSeconds()
{
    return jiffies/HZ;
}

__static_inline  ULONG
KernelGetTickInMilliSeconds()
{
    return jiffies * (1000 / HZ);
}

__static_inline  ULONG
KernelGetTickInMicroSeconds()
{
    return jiffies * (1000000 / HZ);
}

__static_inline  void
KernelGetTickInMicroSeconds64
    (
        ULONG*                      hi_part,
        ULONG*                      lo_part
    )
{
    *hi_part = 0;
    *lo_part = jiffies * (1000000 / HZ);
}

#define KernelGetLocalTime KernelGetSystemTime
__static_inline  void
KernelGetSystemTime(KERNEL_SYSTEM_TIME*  pSystemTime)
{
# not support
    return;
}

#define SECONDS_FROM_1900_TO_1970        2208988800UL
__static_inline unsigned long
KernelGetSystemTimeInSeconds()
{
    struct  timeval tv;

    do_gettimeofday(&tv);
    return (unsigned long)tv.tv_sec + SECONDS_FROM_1900_TO_1970;
}

#define KernelGetUtcSeconds KernelGetSystemTimeInSeconds
#define KernelGetTickInSecondsAbs KernelGetTickInSeconds
#define KernelGetTickInMilliSecondsAbs KernelGetTickInMilliSeconds

#define KernelSetTzString
#define KernelGetTzOffset


/***********************************************************
                        PROFILING
***********************************************************/

#ifdef  PPC85XX
        /*
         * PowerQuicc III
         */
        #define  KernelProfilingPccFactor           1

        __static_inline  void
        KernelProfilingStartPcc()
        {
            ULONG                           ulCtrlValue = 0;

            ulCtrlValue = 0x80000000;
            asm volatile("mtpmr 400,%0" : : "r" (ulCtrlValue));

            ulCtrlValue = 1 << 16;
            asm volatile("mtpmr 144,%0" : : "r" (ulCtrlValue));

            ulCtrlValue = 0;
            asm volatile("mtpmr 16,%0" : : "r" (ulCtrlValue));
            asm volatile("mtpmr 400,%0" : : "r" (ulCtrlValue));
        }

        __static_inline  void
        KernelProfilingStopPcc()
        {
            ULONG                           ulCtrlValue = 0;

            ulCtrlValue = 0x80000000;
            asm volatile("mtpmr 400,%0" : : "r" (ulCtrlValue));
        }

        __static_inline  ULONG
        KernelProfilingReadPcc()
        {
            ULONG                           ulPmc0      = 0;

            asm volatile("mfpmr %0, 16" : "=r" (ulPmc0) :);

            return  ulPmc0;
        }
#endif

#ifdef  IXP42X
        /*
         * IXP425
         */
        #define  KernelProfilingPccFactor           1

        __static_inline  void
        KernelProfilingStartPcc()
        {
            register ULONG                  ulCtrlValue = 0;

            /*
             *  enable/reset Clock Counter via PMNC -- coprocessor 14
             */

            /* read in the current PMNC */
            __asm volatile("mrc\tp14, 0, %0, c0, c1, 0" : "=r" (ulCtrlValue));

            /* clear Clock Counter interrupt */
            ulCtrlValue &= ~0x40;

            /* clear Clock Counter divider */
            ulCtrlValue &= ~0x8;

            /* reset Clock Counter */
            ulCtrlValue |= 0x4;

            /* enable Clock Counter (actually all counters) */
            ulCtrlValue |= 0x1;
            
            /* write PMNC */
            __asm("mcr\tp14, 0, %0, c0, c1, 0" : : "r" (ulCtrlValue));

        }

        /*
         * do not have to do anything to stop Clock Counter
         */
        #define  KernelProfilingStopPcc()

        __static_inline  ULONG
        KernelProfilingReadPcc()
        {
            register ULONG                  _value_;

            /*move to ccnt (clk counter) register from coprocessor 14 and store in value
             */
            __asm volatile("mrc\tp14, 0, %0, c1, c1, 0" : "=r" (_value_));

            return  _value_;
        }
#endif



#endif

