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

/***************************
*  * start of copy from glibc
*   **************************/
struct tm
{
    int tm_sec;           /* Seconds. [0-60] (1 leap second) */
    int tm_min;           /* Minutes. [0-59] */
    int tm_hour;          /* Hours.   [0-23] */
    int tm_mday;          /* Day.     [1-31] */
    int tm_mon;           /* Month.   [0-11] */
    int tm_year;          /* Year - 1900.  */
    int tm_wday;          /* Day of week. [0-6] */
    int tm_yday;          /* Days in year.[0-365] */
    int tm_isdst;         /* DST.     [-1/0/1]*/

# ifdef __USE_BSD
    long int tm_gmtoff;       /* Seconds east of UTC.  */
    __const char *tm_zone;    /* Timezone abbreviation.  */
# else
    long int __tm_gmtoff;     /* Seconds east of UTC.  */
    __const char *__tm_zone;  /* Timezone abbreviation.  */
# endif
};


#define SECS_PER_HOUR   (60 * 60)
#define SECS_PER_DAY    (SECS_PER_HOUR * 24)

# define __isleap(year) \
  ((year) % 4 == 0 && ((year) % 100 != 0 || (year) % 400 == 0))

/* Compute the `struct tm' representation of *T,
   offset OFFSET seconds east of UTC,
   and store year, yday, mon, mday, wday, hour, min, sec into *TP.
   Return nonzero if successful.  */
__static_inline  int
__offtime
	(
		const time_t			*t,
		long int				offset,
		struct tm				*tp
	)
{
  const unsigned short int __mon_yday[2][13] =
  {
    /* Normal years.  */
    { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 },
    /* Leap years.  */
    { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 }
  };

  long int days, rem, y;
  const unsigned short int *ip;

  days = *t / SECS_PER_DAY;
  rem = *t % SECS_PER_DAY;
  rem += offset;
  while (rem < 0)
    {
      rem += SECS_PER_DAY;
      --days;
    }
  while (rem >= SECS_PER_DAY)
    {
      rem -= SECS_PER_DAY;
      ++days;
    }
  tp->tm_hour = rem / SECS_PER_HOUR;
  rem %= SECS_PER_HOUR;
  tp->tm_min = rem / 60;
  tp->tm_sec = rem % 60;
  /* January 1, 1970 was a Thursday.  */
  tp->tm_wday = (4 + days) % 7;
  if (tp->tm_wday < 0)
    tp->tm_wday += 7;
  y = 1970;

#define DIV(a, b) ((a) / (b) - ((a) % (b) < 0))
#define LEAPS_THRU_END_OF(y) (DIV (y, 4) - DIV (y, 100) + DIV (y, 400))

  while (days < 0 || days >= (__isleap (y) ? 366 : 365))
    {
      /* Guess a corrected year, assuming 365 days per year.  */
      long int yg = y + days / 365 - (days % 365 < 0);

      /* Adjust DAYS and Y to match the guessed year.  */
      days -= ((yg - y) * 365
	       + LEAPS_THRU_END_OF (yg - 1)
	       - LEAPS_THRU_END_OF (y - 1));
      y = yg;
    }
  tp->tm_year = y - 1900;
  if (tp->tm_year != y - 1900)
    return 0;
  tp->tm_yday = days;
  ip = __mon_yday[__isleap(y)];
  for (y = 11; days < (long int) ip[y]; --y)
    continue;
  days -= ip[y];
  tp->tm_mon = y;
  tp->tm_mday = days + 1;
  return 1;
}

#define KernelGetLocalTime KernelGetSystemTime
__static_inline  void
KernelGetSystemTime(KERNEL_SYSTEM_TIME*  pSystemTime)
{
    struct timeval  tv;
    time_t          timeNow;
    struct tm       ttmm;
    struct tm       *ptm;

    do_gettimeofday(&tv);
    timeNow = tv.tv_sec;

    ptm = &ttmm;
    __offtime(&timeNow, 0, ptm);

    pSystemTime->Year           = ptm->tm_year + 1900;
    pSystemTime->Month          = ptm->tm_mon + 1;
    pSystemTime->DayOfMonth     = ptm->tm_mday;
    pSystemTime->DayOfWeek      = ptm->tm_wday;
    pSystemTime->Hour           = ptm->tm_hour;
    pSystemTime->Minute         = ptm->tm_min;
    pSystemTime->Second         = ptm->tm_sec;
    pSystemTime->MilliSecond    = jiffies * 1000 / HZ;
    pSystemTime->bDayLightSaving= ptm->tm_isdst;
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

#ifdef  BCM_63XX
        /*
         *  BCM_6358
         */
        #define  KernelProfilingPccFactor            1

        #define  PERF_COUNTER_BASE 0xFF420000

        typedef struct BCM4350_register_config 
        {
            unsigned long global_control;		// 0x00
            #define PCE_enable (1<<31)
            #define PCSD_disable (1<<8)
            #define ModID(module) (module<<2)
            #define mod_BIU 1
            #define mod_Core 2
            #define mod_Data_Cache 4
            #define mod_Instruction_Cache 6
            #define mod_Readahead_Cache 0xb
            #define SetID(id) (id)
            #define mod_set_mask 0x3f
            unsigned long control[2];			// 0x04, 0x08
            #define BCM4350_COUNTER_EVENT(c,event)		(event  <<  (c%2 ? 18:2))
            #define BCM4350_COUNTER_ENABLE(c)			(1      <<  (c%2 ? 31:15))
            #define BCM4350_COUNTER_TPID(c,pid)			(pid    <<  (c%2 ? 29:13))
            #define BCM4350_COUNTER_AMID(c,amid)		(amid   <<  (c%2 ? 25:9))
            #define BCM4350_COUNTER_OVERFLOW(c)			(1    	<<	(c%2 ? 16:0))
            unsigned long unused;		// 0x0c
            long counter[4];			// 0x10, 0x14, 0x18, 0x1c
        } Perf_Control;
        #define PERFC ((volatile Perf_Control * const) PERF_COUNTER_BASE)

         __static_inline  void
         KernelProfilingStartPcc()
         {
            PERFC->global_control = PCE_enable;
	        PERFC->control[0] = PERFC->control[1] = 0x0;
            PERFC->counter[0] = -1;

            PERFC->control[0] |= BCM4350_COUNTER_EVENT(0, 0x12) | BCM4350_COUNTER_ENABLE(0);
         }

         __static_inline  void
         KernelProfilingStopPcc()
         {
            PERFC->global_control = 0;
         }

         __static_inline  ULONG
         KernelProfilingReadPcc()
         {
            return  (0xFFFFFFFF - PERFC->counter[0] + 1);
         }
#endif


#endif

