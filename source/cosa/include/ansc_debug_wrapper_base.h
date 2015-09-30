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

    module:	ansc_wrapper_base.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco System  , Inc., 1997 ~ 2001
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        data structures and basic types.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        01/15/01    initial revision.
        01/19/01    restructure the container object, add and
                    remove a few things
        04/01/01    add user mode wrapper functions

**********************************************************************/


#ifndef  _ANSC_WRAPPER_BASE_
#define  _ANSC_WRAPPER_BASE_


/***********************************************************
                  COMPILER SPECIFIC DEFINITIONS
***********************************************************/

/*
 * Macro for compilers:
 */
#if defined(__GNUC__)

  #define ANSC_FORCEINLINE                  static inline __attribute__((always_inline))

  #if __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ < 96)
    #define ansc_likely(x)                  (x)
    #define ansc_unlikely(x)                (x)
  #else
    #define ansc_likely(x)                  __builtin_expect(!!(x), 1)
    #define ansc_unlikely(x)                __builtin_expect(!!(x), 0)
  #endif
  
#elif defined(_MSC_VER)

  #define ANSC_FORCEINLINE                  __inline

  #pragma warning(disable: 4200)   /* zero-sized array in struct/union */
  #pragma warning(disable: 4201)   /* nameless struct/union */

  #define ansc_likely(x)                    (x)
  #define ansc_unlikely(x)                  (x)

#else 

  #define ANSC_FORCEINLINE           

#endif /* end of __GNUC__ */


/*
 * unreferenced parameters:
 */

#undef  UNREFERENCED_PARAMETER
#define UNREFERENCED_PARAMETER(_p_)         (_p_)


/***********************************************************
                  OS SPECIFIC DEFINITIONS
***********************************************************/

/*
 * Define the current operating system type. We can either do it here or in the makefile or the IDE.
 */

/*
 * For every operating system, we must include a header file which defines all the basic data types and
 * necessary system wrappers. Following is the list of header files must be included for Windows 9x.
 */
#ifdef   _ANSC_WINDOWS9X

    #ifdef   _ANSC_KERNEL

        #include "windows9x/kernel_base.h"
        #include "windows9x/kernel_runtime.h"
        #include "windows9x/kernel_protection.h"
        #include "windows9x/kernel_memory.h"
        #include "windows9x/kernel_string.h"
        #include "windows9x/kernel_debug.h"
        #include "windows9x/kernel_time.h"
        #include "windows9x/kernel_socket.h"
        #include "windows9x/kernel_task.h"
        #include "windows9x/kernel_file_io.h"
        #include "windows9x/kernel_packet.h"

    #else

        #include "windows9x/user_base.h"
        #include "windows9x/user_runtime.h"
        #include "windows9x/user_protection.h"
        #include "windows9x/user_memory.h"
        #include "windows9x/user_string.h"
        #include "windows9x/user_debug.h"
        #include "windows9x/user_time.h"
        #include "windows9x/user_socket.h"
        #include "windows9x/user_task.h"
        #include "windows9x/user_file_io.h"
        #include "windows9x/user_packet.h"

    #endif

#endif


/*
 * For every operating system, we must include a header file which defines all the basic data types and
 * necessary system wrappers. Following is the list of header files must be included for Windows NT.
 */
#ifdef   _ANSC_WINDOWSNT

    #ifdef   _ANSC_KERNEL

        #include "windowsnt/kernel_base.h"
        #include "windowsnt/kernel_runtime.h"
        #include "windowsnt/kernel_protection.h"
        #include "windowsnt/kernel_memory.h"
        #include "windowsnt/kernel_string.h"
        #include "windowsnt/kernel_debug.h"
        #include "windowsnt/kernel_time.h"
        #include "windowsnt/kernel_socket.h"
        #include "windowsnt/kernel_task.h"
        #include "windowsnt/kernel_file_io.h"
        #include "windowsnt/kernel_packet.h"

    #else

        #include "windowsnt/user_base.h"
        #include "windowsnt/user_runtime.h"
        #include "windowsnt/user_protection.h"
        #include "windowsnt/user_memory.h"
        #include "windowsnt/user_string.h"
        #include "windowsnt/user_debug.h"
        #include "windowsnt/user_time.h"
        #include "windowsnt/user_socket.h"
        #include "windowsnt/user_task.h"
        #include "windowsnt/user_file_io.h"
        #include "windowsnt/user_packet.h"

    #endif

#endif


/*
 * For every operating system, we must include a header file which defines all the basic data types and
 * necessary system wrappers. Following is the list of header files must be included for Windows CE.
 */
#ifdef   _ANSC_WINDOWSCE

    #ifdef   _ANSC_KERNEL

        #include "windowsce/kernel_base.h"
        #include "windowsce/kernel_runtime.h"
        #include "windowsce/kernel_protection.h"
        #include "windowsce/kernel_memory.h"
        #include "windowsce/kernel_string.h"
        #include "windowsce/kernel_debug.h"
        #include "windowsce/kernel_time.h"
        #include "windowsce/kernel_socket.h"
        #include "windowsce/kernel_task.h"
        #include "windowsce/kernel_file_io.h"
        #include "windowsce/kernel_packet.h"

    #else

        #include "windowsce/user_base.h"
        #include "windowsce/user_runtime.h"
        #include "windowsce/user_protection.h"
        #include "windowsce/user_memory.h"
        #include "windowsce/user_string.h"
        #include "windowsce/user_debug.h"
        #include "windowsce/user_time.h"
        #include "windowsce/user_socket.h"
        #include "windowsce/user_task.h"
        #include "windowsce/user_file_io.h"
        #include "windowsce/user_packet.h"

    #endif

#endif


/*
 * For every operating system, we must include a header file which defines all the basic data types and
 * necessary system wrappers. Following is the list of header files must be included for Linux.
 */
#ifdef   _ANSC_LINUX

    #ifdef   _ANSC_KERNEL

        #include "linux/kernel_base.h"
        #include "linux/kernel_runtime.h"
        #include "linux/kernel_protection.h"
        #include "linux/kernel_memory.h"
        #include "linux/kernel_string.h"
        #include "linux/kernel_debug.h"
        #include "linux/kernel_time.h"
        #include "linux/kernel_socket.h"
        #include "linux/kernel_task.h"
        #include "linux/kernel_file_io.h"
        #include "linux/kernel_packet.h"

    #else

        #include "user_base.h"
//        #include "linux/user_runtime.h"
//        #include "linux/user_protection.h"
        #include "user_memory.h"
        #include "user_string.h"
        #include "user_debug.h"
        #include "user_time.h"
//        #include "linux/user_socket.h"
//        #include "linux/user_task.h"
//        #include "linux/user_file_io.h"
//        #include "linux/user_packet.h"

    #endif

#endif


/*
 * For every operating system, we must include a header file which defines all the basic data types and
 * necessary system wrappers. Following is the list of header files must be included for VxWorks.
 */
#ifdef   _ANSC_VXWORKS

    #ifdef   _ANSC_KERNEL

        #include "vxworks/kernel_base.h"
        #include "vxworks/kernel_runtime.h"
        #include "vxworks/kernel_protection.h"
        #include "vxworks/kernel_memory.h"
        #include "vxworks/kernel_string.h"
        #include "vxworks/kernel_debug.h"
        #include "vxworks/kernel_time.h"
        #include "vxworks/kernel_socket.h"
        #include "vxworks/kernel_task.h"
        #include "vxworks/kernel_file_io.h"
        #include "vxworks/kernel_packet.h"

    #else

        #include "vxworks/user_base.h"
        #include "vxworks/user_runtime.h"
        #include "vxworks/user_protection.h"
        #include "vxworks/user_memory.h"
        #include "vxworks/user_string.h"
        #include "vxworks/user_debug.h"
        #include "vxworks/user_time.h"
        #include "vxworks/user_socket.h"
        #include "vxworks/user_task.h"
        #include "vxworks/user_file_io.h"
        #include "vxworks/user_packet.h"

    #endif

#endif


/*
 * For every operating system, we must include a header file which defines all the basic data types and
 * necessary system wrappers. Following is the list of header files must be included for pSoS.
 */
#ifdef   _ANSC_PSOS

    #ifdef   _ANSC_KERNEL

        #include "psos/kernel_base.h"
        #include "psos/kernel_runtime.h"
        #include "psos/kernel_protection.h"
        #include "psos/kernel_memory.h"
        #include "psos/kernel_string.h"
        #include "psos/kernel_debug.h"
        #include "psos/kernel_time.h"
        #include "psos/kernel_socket.h"
        #include "psos/kernel_task.h"
        #include "psos/kernel_file_io.h"
        #include "psos/kernel_packet.h"

    #else

        #include "psos/user_base.h"
        #include "psos/user_runtime.h"
        #include "psos/user_protection.h"
        #include "psos/user_memory.h"
        #include "psos/user_string.h"
        #include "psos/user_debug.h"
        #include "psos/user_time.h"
        #include "psos/user_socket.h"
        #include "psos/user_task.h"
        #include "psos/user_file_io.h"
        #include "psos/user_packet.h"

    #endif

#endif


/*
 * For every operating system, we must include a header file which defines all the basic data types and
 * necessary system wrappers. Following is the list of header files must be included for pSoS.
 */
#ifdef   _ANSC_ATMOS

    #ifdef   _ANSC_KERNEL

        #include "atmos/kernel_base.h"
        #include "atmos/kernel_runtime.h"
        #include "atmos/kernel_protection.h"
        #include "atmos/kernel_memory.h"
        #include "atmos/kernel_string.h"
        #include "atmos/kernel_debug.h"
        #include "atmos/kernel_time.h"
        #include "atmos/kernel_socket.h"
        #include "atmos/kernel_task.h"
        #include "atmos/kernel_file_io.h"
        #include "atmos/kernel_packet.h"

    #else

        #include "atmos/user_base.h"
        #include "atmos/user_runtime.h"
        #include "atmos/user_protection.h"
        #include "atmos/user_memory.h"
        #include "atmos/user_string.h"
        #include "atmos/user_debug.h"
        #include "atmos/user_time.h"
        #include "atmos/user_socket.h"
        #include "atmos/user_task.h"
        #include "atmos/user_file_io.h"
        #include "atmos/user_packet.h"

    #endif

#endif


/*
 * For every operating system, we must include a header file which defines all the basic data types and
 * necessary system wrappers. Following is the list of header files must be included for pSoS.
 */
#ifdef   _ANSC_OSE

    #ifdef   _ANSC_KERNEL

        #include "ose/kernel_base.h"
        #include "ose/kernel_runtime.h"
        #include "ose/kernel_protection.h"
        #include "ose/kernel_memory.h"
        #include "ose/kernel_string.h"
        #include "ose/kernel_debug.h"
        #include "ose/kernel_time.h"
        #include "ose/kernel_socket.h"
        #include "ose/kernel_task.h"
        #include "ose/kernel_file_io.h"
        #include "ose/kernel_packet.h"

    #else

        #include "ose/user_base.h"
        #include "ose/user_runtime.h"
        #include "ose/user_protection.h"
        #include "ose/user_memory.h"
        #include "ose/user_string.h"
        #include "ose/user_debug.h"
        #include "ose/user_time.h"
        #include "ose/user_socket.h"
        #include "ose/user_task.h"
        #include "ose/user_file_io.h"
        #include "ose/user_packet.h"

    #endif

#endif


/***********************************************************
          DEFINITION OF BASIC UPPER AND LOWER LIMITS
***********************************************************/

/*
 * Some of the modules may share the same data structure and size limitation. We define them here
 * to eliminate the possibility of confiliction.
 */
#define  ANSC_MAX_USERNAME_SIZE                     128
#define  ANSC_MAX_PASSWORD_SIZE                     128
#define  ANSC_PROPERTY_NAME_SIZE                    32
#define  ANSC_OBJECT_NAME_SIZE                      48
#define  ANSC_OBJ_IF_NAME_SIZE                      64
#define  ANSC_DOMAIN_NAME_SIZE                      256
#define  ANSC_FQDN_NAME_SIZE                        256
#define  ANSC_HOST_NAME_SIZE                        64
#define  ANSC_URI_PATH_SIZE                         128
#define  ANSC_URI_STRING_SIZE                       256
#define  ANSC_EMAIL_ADDRESS_SIZE                    128
#define  ANSC_MAX_STRING_SIZE                       1024
#define  ANSC_MAX_IPC_PARAMS_SIZE                   10240
#define  ANSC_MAX_IPC_RETURN_SIZE                   10240
#define  ANSC_MAX_OBJECT_NUMBER                     128
#define  ANSC_MAX_HEADER_SIZE                       256


/***********************************************************
              DEFINITION OF CONTAINER STATUS
***********************************************************/

/*
 * We define the basic status type as an unsigned integer. In the future, we may be able to encapsulate
 * the status in an object too.
 */
#ifdef   _ANSC_KERNEL

    #define  _ANSC_STATIC_                          _KERNEL_STATIC_
    #define  _ANSC_INLINE_                          _KERNEL_INLINE_

    typedef  ULONG                  ANSC_STATUS,     *PANSC_STATUS;
    typedef  PVOID                  ANSC_HANDLE,     *PANSC_HANDLE;
    typedef  KERNEL_LOCK            ANSC_LOCK,       *PANSC_LOCK;
    typedef  KERNEL_SPINLOCK        _ANSC_SPINLOCK_DEF;
    typedef  KERNEL_SEMAPHORE       ANSC_SEMAPHORE,  *PANSC_SEMAPHORE;

    #ifdef  _ANSC_SIMULATE_EVENT_
        typedef  ANSC_HANDLE        ANSC_EVENT,      *PANSC_EVENT;
    #else
        typedef  KERNEL_EVENT       ANSC_EVENT,      *PANSC_EVENT;
    #endif

    typedef  KERNEL_MEMORY_CACHE    ANSC_MEMORY_CACHE, *PANSC_MEMORY_CACHE;
#else

    #define  _ANSC_STATIC_                          _USER_STATIC_
    #define  _ANSC_INLINE_                          _USER_INLINE_

    typedef  ULONG                  ANSC_STATUS,     *PANSC_STATUS;
    typedef  PVOID                  ANSC_HANDLE,     *PANSC_HANDLE;
    typedef  USER_LOCK              ANSC_LOCK,       *PANSC_LOCK;
    typedef  USER_SPINLOCK          _ANSC_SPINLOCK_DEF;
    typedef  USER_SEMAPHORE         ANSC_SEMAPHORE,  *PANSC_SEMAPHORE;

    #ifdef  _ANSC_SIMULATE_EVENT_
        typedef  ANSC_HANDLE        ANSC_EVENT,      *PANSC_EVENT;
    #else
        typedef  USER_EVENT         ANSC_EVENT,      *PANSC_EVENT;
    #endif

    typedef  USER_MEMORY_CACHE      ANSC_MEMORY_CACHE, *PANSC_MEMORY_CACHE;
#endif


#ifdef   _ANSC_SPINLOCK_PROFILING_

#else
    typedef  _ANSC_SPINLOCK_DEF ANSC_SPINLOCK, *PANSC_SPINLOCK;
#endif


/***********************************************************
         DEFINITION OF POPULAR IP ADDRESS STRUCTURES
***********************************************************/

/*
 * Although IP address normally appears only in IP headers, many other protocols may contain or
 * refer to the use of IP address. So it's necessary to make IP address definitions globally
 * available instead of embedding it in the header file for IP.
 */
#define  IPV4_ADDRESS_SIZE                          4
#define  IPV6_ADDRESS_SIZE                          16

#ifdef   _ANSC_IPV6
    #define  IP_ADDRESS_SIZE                        IPV6_ADDRESS_SIZE
#else
    #define  IP_ADDRESS_SIZE                        IPV4_ADDRESS_SIZE
#endif

#define  IP_ADDRESS_LENGTH                          IP_ADDRESS_SIZE

/*
 * While we're trying really hard to smooth the procedure of switch-over from IPv4 to IPv4, there
 * are many places where using the IP address as an integer for comparision and calculation is much
 * easier than array-based operation.
 */
#define  ANSC_IPV4_ADDRESS                                                                  \
         union                                                                              \
         {                                                                                  \
            UCHAR                   Dot[IPV4_ADDRESS_SIZE];                                 \
            ULONG                   Value;                                                  \
         }

#define  ANSC_IPV4_MASK                                                                     \
         union                                                                              \
         {                                                                                  \
            UCHAR                   Dot[IPV4_ADDRESS_SIZE];                                 \
            ULONG                   Value;                                                  \
         }


/***********************************************************
          DEFINITION OF POPULAR DATA TYPE STRUCTURES
***********************************************************/

/*
 * There're places that we need to use a large integer which may require more than 32 bits to hold
 * its value. While some compilers may provide support for a 64-bit integer, we want to define our
 * own here to make it platform-independent.
 */
typedef  UCHAR   ANSC_UINT08,  *PANSC_UINT08;
typedef  USHORT  ANSC_UINT16,  *PANSC_UINT16;
typedef  ULONG   ANSC_UINT32,  *PANSC_UINT32;

typedef  struct
_ANSC_UINT64
{
    ULONG                           HiPart;
    ULONG                           LoPart;
}
ANSC_UINT64,  *PANSC_UINT64;

#define  AnscEqualUint64(uint64_v1, uint64_v2)      \
         ( (uint64_v1->HiPart == uint64_v2->HiPart) && (uint64_v1->LoPart == uint64_v2->LoPart) )
#define  AnscInitUint64(uint64_v)                   \
         { uint64_v->HiPart = 0; uint64_v->LoPart = 0; }

#define  AnscIncUint64(uint64_v)                                                            \
         {                                                                                  \
            uint64_v->LoPart++;                                                             \
                                                                                            \
            if ( uint64_v->LoPart == 0 )                                                    \
            {                                                                               \
                uint64_v->HiPart++;                                                         \
            }                                                                               \
         }

#define  AnscAddToUint64(uint64_v, uint32_v)                                                \
         {                                                                                  \
            uint64_v->LoPart += uint32_v;                                                   \
                                                                                            \
            if ( uint64_v->LoPart < uint32_v )                                              \
            {                                                                               \
                uint64_v->HiPart++;                                                         \
            }                                                                               \
         }


/***********************************************************
             DEFINITION OF BYTES SWAPPING MACROS
***********************************************************/

/*
 * These macros and inline functions shield the rest of the container from dealing with the byte-order
 * issue caused by different endian types supported by different cpu.
 */
#ifdef   _ANSC_LITTLE_ENDIAN_

    #define  AnscMemoryFromNToH                     AnscSwapMemory
    #define  AnscUlongFromNToH                      AnscSwapUlong
    #define  AnscUshortFromNToH                     AnscSwapUshort
    #define  AnscMemoryFromHToN                     AnscSwapMemory
    #define  AnscUlongFromHToN                      AnscSwapUlong
    #define  AnscUshortFromHToN                     AnscSwapUshort

#else

    #define  AnscMemoryFromNToH
    #define  AnscUlongFromNToH
    #define  AnscUshortFromNToH
    #define  AnscMemoryFromHToN
    #define  AnscUlongFromHToN
    #define  AnscUshortFromHToN

#endif


/*
 * Some very basic macros to facilitate the daily programming tasks.
 */
#define  AnscGetMax2(a, b)                          (a > b)? a : b
#define  AnscGetMin2(a, b)                          (a > b)? b : a


/*
 * If we pack the data members in a stucture on one-byte boundary, it will cause some problem for
 * accessing them on certain platforms depending on the underlying hardware architecture. If the
 * option below is enabled, any field in a packet will be accessed byte-by-byte, regardless actual
 * size of the data member.
 */
#ifdef  _ANSC_BYTE_ACCESS_

    #ifdef _ANSC_NO_HALFWORD_ACCESS_
        #error "Need half-word access support!"
    #endif /* _ANSC_NO_HALFWORD_SUPPORT_ */

    #ifdef  _ANSC_LITTLE_ENDIAN_

        ANSC_FORCEINLINE
        USHORT
        AnscReadUshort(void * addr)
        {
            PUCHAR          pb = (PUCHAR)addr;

            if( ansc_unlikely((ULONG)pb & 1) )
            {
                return (USHORT)*pb + ((USHORT)*(pb + 1) << 8);
            }
            else
            {
                return *(PUSHORT)pb;
            }
        }

        ANSC_FORCEINLINE
        ULONG
        AnscReadUlong(void * addr)
        {
            PUCHAR          pb = (PUCHAR)addr;

            if( ansc_unlikely((ULONG)pb & 1) )
            {
                return (ULONG)*pb + ((ULONG)*(PUSHORT)(pb+1) << 8) + ((ULONG)*(pb+3) << 24);
            }

            if( ((ULONG)pb & 3) )
            {
                return (ULONG)*(PUSHORT)pb + ((ULONG)*(PUSHORT)(pb+2) << 16);
            }

            return *(PULONG)pb;
        }
        
        ANSC_FORCEINLINE
        void
        AnscWriteUshort(void * addr, USHORT u16)
        {
            PUCHAR          pb = (PUCHAR)addr;

            if( ansc_unlikely((ULONG)pb & 1) )
            {
                *pb             = (UCHAR)u16;
                *(pb+1)         = (UCHAR)(u16 >> 8);
            }
            else
            {
                *(PUSHORT)pb    = u16;
            }
        }
        
        ANSC_FORCEINLINE
        void
        AnscWriteUlong(void * addr, ULONG u32)
        {
            PUCHAR          pb = (PUCHAR)addr;
        
            if( ansc_unlikely((ULONG)pb & 1) )
            {
                *pb             = (UCHAR )u32;   
                *(PUSHORT)(pb+1)= (USHORT)(u32 >> 8);
                *(pb+3)         = (UCHAR )(u32 >> 24);
                return;
            }
            
            if( ((ULONG)pb & 3) )
            {
                *(PUSHORT)pb    = (USHORT)u32;
                *(PUSHORT)(pb+2)= (USHORT)(u32 >> 16);
                return;
            }

            *(PULONG)pb = u32;
        }

        #define  AnscEqualUshort(a, b)                                                      \
                 ( (*((PUCHAR)&a) == *((PUCHAR)&b)) && (*((PUCHAR)&a + 1) == *((PUCHAR)&b + 1)) )
        #define  AnscEqualUlong(a, b)                                                       \
                 ( (*((PUCHAR)&a) == *((PUCHAR)&b)) && (*((PUCHAR)&a + 1) == *((PUCHAR)&b + 1)) && (*((PUCHAR)&a + 2) == *((PUCHAR)&b + 2)) && (*((PUCHAR)&a + 3) == *((PUCHAR)&b + 3)) )

        #define  AnscUcharToUshort(f_uchar)                                                 \
                 (USHORT)(f_uchar)

    #else

        ANSC_FORCEINLINE
        USHORT
        AnscReadUshort(void * addr)
        {
            PUCHAR          pb = (PUCHAR)addr;

            if( ansc_unlikely((ULONG)pb & 1) )
            {
                return ((USHORT)*pb << 8) + (USHORT)*(pb+1);
            }
            else
            {
                return *(PUSHORT)pb;
            }
        }

        ANSC_FORCEINLINE
        ULONG
        AnscReadUlong(void * addr)
        {
            PUCHAR          pb = (PUCHAR)addr;

            if( ansc_unlikely((ULONG)pb & 1) )
            {
                return ((ULONG)*(pb) << 24) + ((ULONG)*(PUSHORT)(pb+1) << 8) + (ULONG)*(pb+3);
            }

            if( ((ULONG)pb & 3) )
            {
                return ((ULONG)*(PUSHORT)(pb) << 16) + (ULONG)*(PUSHORT)(pb+2);
            }

            return *(PULONG)pb;
        }
        
        ANSC_FORCEINLINE
        void
        AnscWriteUshort(void * addr, USHORT u16)
        {
            PUCHAR          pb = (PUCHAR)addr;

            if( ansc_unlikely((ULONG)pb & 1) )
            {
                *pb             = (UCHAR)(u16 >> 8);
                *(pb+1)         = (UCHAR)u16;
            }
            else
            {
                *(PUSHORT)pb    = u16;
            }
        }
        
        ANSC_FORCEINLINE
        void
        AnscWriteUlong(void * addr, ULONG u32)
        {
            PUCHAR          pb = (PUCHAR)addr;
        
            if( ansc_unlikely((ULONG)pb & 1) )
            {
                *pb             = (UCHAR )(u32 >> 24);   
                *(PUSHORT)(pb+1)= (USHORT)(u32 >> 8);
                *(pb+3)         = (UCHAR )u32;
                return;
            }
            
            if( ((ULONG)pb & 3) )
            {
                *(PUSHORT)pb    = (USHORT)(u32 >> 16);
                *(PUSHORT)(pb+2)= (USHORT)u32;
                return;
            }

            *(PULONG)pb = u32;
        }


        #define  AnscEqualUshort(a, b)                                                      \
                 ( (*((PUCHAR)&a) == *((PUCHAR)&b)) && (*((PUCHAR)&a + 1) == *((PUCHAR)&b + 1)) )
        #define  AnscEqualUlong(a, b)                                                       \
                 ( (*((PUCHAR)&a) == *((PUCHAR)&b)) && (*((PUCHAR)&a + 1) == *((PUCHAR)&b + 1)) && (*((PUCHAR)&a + 2) == *((PUCHAR)&b + 2)) && (*((PUCHAR)&a + 3) == *((PUCHAR)&b + 3)) )

        #define  AnscUcharToUshort(f_uchar)                                                 \
                 (USHORT)(f_uchar) * 0x100

    #endif

#else

    #ifdef  _ANSC_LITTLE_ENDIAN_

        #define  AnscReadUshort(addr)               (*(PUSHORT)(addr))
        #define  AnscReadUlong(addr)                (*(PULONG )(addr))

        #define  AnscWriteUshort(addr, data)        *(PUSHORT)(addr) = data
        #define  AnscWriteUlong(addr, data)         *(PULONG )(addr) = data

        #define  AnscEqualUshort(a, b)              ( a == b )
        #define  AnscEqualUlong(a, b)               ( a == b )

        #define  AnscUcharToUshort(f_uchar)         (USHORT)(f_uchar)

    #else

        #define  AnscReadUshort(addr)               (*(PUSHORT)(addr))
        #define  AnscReadUlong(addr)                (*(PULONG )(addr))

        #define  AnscWriteUshort(addr, data)        *(PUSHORT)(addr) = data
        #define  AnscWriteUlong(addr, data)         *(PULONG )(addr) = data

        #define  AnscEqualUshort(a, b)              ( a == b )
        #define  AnscEqualUlong(a, b)               ( a == b )

        #define  AnscUcharToUshort(f_uchar)         (USHORT)(f_uchar) * 0x100

    #endif

#endif

#endif /* end of _ANSC_WRAPPER_BASE_ */

