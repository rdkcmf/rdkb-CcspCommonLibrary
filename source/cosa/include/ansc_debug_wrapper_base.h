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

#include <stdint.h>

/***********************************************************
                  COMPILER SPECIFIC DEFINITIONS
***********************************************************/

/*
 * Macro for compilers:
 */
#if defined(__GNUC__)

  #define ANSC_FORCEINLINE                  static inline __attribute__((always_inline))

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
 * For every operating system, we must include a header file which defines all the basic data types and
 * necessary system wrappers. Following is the list of header files must be included for Linux.
 */


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
            unsigned char           Dot[IPV4_ADDRESS_SIZE];                                 \
            uint32_t                Value;                                                  \
         }


/***********************************************************
          DEFINITION OF POPULAR DATA TYPE STRUCTURES
***********************************************************/

/*
 * There're places that we need to use a large integer which may require more than 32 bits to hold
 * its value. While some compilers may provide support for a 64-bit integer, we want to define our
 * own here to make it platform-independent.
 */
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
#if defined(_ANSC_LITTLE_ENDIAN_)

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

    #if defined(_ANSC_LITTLE_ENDIAN_)

        ANSC_FORCEINLINE
        USHORT
        AnscReadUshort(void * addr)
        {
            PUCHAR          pb = (PUCHAR)addr;

            if( ((ULONG)pb & 1) )
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

            if( ((ULONG)pb & 1) )
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

            if( ((ULONG)pb & 1) )
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
        
            if( ((ULONG)pb & 1) )
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

    #else

        ANSC_FORCEINLINE
        USHORT
        AnscReadUshort(void * addr)
        {
            PUCHAR          pb = (PUCHAR)addr;

            if( ((ULONG)pb & 1) )
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

            if( ((ULONG)pb & 1) )
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

            if( ((ULONG)pb & 1) )
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
        
            if( ((ULONG)pb & 1) )
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

    #endif

#else

    #define  AnscReadUshort(addr)               (*(PUSHORT)(addr))
    #define  AnscReadUlong(addr)                (*(PULONG )(addr))

    #define  AnscWriteUshort(addr, data)        *(PUSHORT)(addr) = data
    #define  AnscWriteUlong(addr, data)         *(PULONG )(addr) = data

#endif

#endif /* end of _ANSC_WRAPPER_BASE_ */

