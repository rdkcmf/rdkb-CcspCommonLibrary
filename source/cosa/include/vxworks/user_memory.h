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

    module:	user_memory.h

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

        VxWorks 5.4

    ---------------------------------------------------------------

    author:

        Huaxiang Sun
        Hua Ding

    ---------------------------------------------------------------

    revision:

        01/19/01    initial revision.

**********************************************************************/


#ifndef  _USER_MEMORY_
#define  _USER_MEMORY_


/***********************************************************
        DEFINITION OF BASIC DATA TYPE AND STRUCTURES
***********************************************************/
/*#define  _USER_USE_MEMORY_TRACKING_*/

#define  _USER_MEMORY_TRACKING_METHOD_1_

/***********************************************************
       BASIC OPERATIONS BY MACROS AND INLINE FUNCTIONS
***********************************************************/

/*
 *  prototype
 *
PVOID
RealUserAllocateMemory
    (
        ULONG                       ulSize
    );
 */
#define  RealUserAllocateMemory(ulSize)             malloc(ulSize)

/*
 *  prototype
 *
VOID
RealUserFreeMemory
    (
        PVOID                       pMem
    );
 */
#define  RealUserFreeMemory(pMem)                   free(pMem)


/*
 *  prototype
 *
PVOID
RealUserReAllocMemory
    (
        PVOID                       pMem,
        ULONG                       ulSize
    );
 */
#define  RealUserReAllocMemory(pMem, ulSize)        realloc(pMem, ulSize)


/*
 *  not supported
 *
 *  if AnscAllocateMemory2/AnscFreeMemory2 are enabled, we do need to
 *  implement the mechanism to track memory usage
 *
 *  prototype
 *
ULONG
RealUserGetMemorySize
    (
        PVOID                       pMem
    );
 */
#define  RealUserGetMemorySize(pMem)                0


#ifdef   _USER_USE_MEMORY_TRACKING_

    #if     defined(_USER_MEMORY_TRACKING_METHOD_1_)

        PVOID
        __UserAllocateMemory
            (
                ULONG                       ulSize,
                PCSTR                       pFileName,
                ULONG                       LineNumber
            );

        VOID
        __UserFreeMemory
            (
                PVOID                       pMem,
                PCSTR                       pFileName,
                ULONG                       LineNumber
            );

        PVOID
        __UserReAllocMemory
            (
                PVOID                       pMem,
                ULONG                       ulSize,
                PCSTR                       pFileName,
                ULONG                       LineNumber
            );

        #define  UserAllocateMemory(ulSize)             __UserAllocateMemory(ulSize, __FILE__, __LINE__)
        #define  UserFreeMemory(pMem)                   __UserFreeMemory(pMem, __FILE__, __LINE__)
        #define  UserReAllocMemory(pMem, ulSize)        __UserReAllocMemory(pMem, ulSize, __FILE__, __LINE__)

        ULONG
        UserGetMemorySize
            (
                PVOID                       pMem
            );

    #elif   defined(_USER_MEMORY_TRACKING_METHOD_2_)

        PVOID
        UserAllocateMemory
            (
                ULONG                       ulSize
            );

        VOID
        UserFreeMemory
            (
                PVOID                       pMem
            );

        PVOID
        __UserReAllocMemory
            (
                PVOID                       pMem,
                ULONG                       ulSize,
                PCSTR                       pFileName,
                ULONG                       LineNumber
            );

        #define  UserReAllocMemory(pMem, ulSize)        __UserReAllocMemory(pMem, ulSize, __FILE__, __LINE__)

        ULONG
        UserGetMemorySize
            (
                PVOID                       pMem
            );

    #elif   defined(_USER_MEMORY_TRACKING_METHOD_3_)

        PVOID
        __UserAllocateMemory
            (
                ULONG                       ulSize,
                PCSTR                       pFileName,
                ULONG                       LineNumber
            );

        VOID
        __UserFreeMemory
            (
                PVOID                       pMem,
                PCSTR                       pFileName,
                ULONG                       LineNumber
            );

        PVOID
        __UserReAllocMemory
            (
                PVOID                       pMem,
                ULONG                       ulSize,
                PCSTR                       pFileName,
                ULONG                       LineNumber
            );

        #define  UserAllocateMemory(ulSize)             __UserAllocateMemory(ulSize, __FILE__, __LINE__)
        #define  UserFreeMemory(pMem)                   __UserFreeMemory(pMem, __FILE__, __LINE__)
        #define  UserReAllocMemory(pMem, ulSize)        __UserReAllocMemory(pMem, ulSize, __FILE__, __LINE__)

        ULONG
        UserGetMemorySize
            (
                PVOID                       pMem
            );

    #endif

#else

    PVOID
    UserAllocateMemory
        (
            ULONG                       ulSize
        );

    #define  UserFreeMemory                         RealUserFreeMemory
    #define  UserReAllocMemory                      RealUserReAllocMemory
    #define  UserGetMemorySize                      RealUserGetMemorySize

#endif

PVOID
UserResizeMemory
    (
        PVOID                       pMem,
        ULONG                       ulSize
    );


/*
 *  prototype
 *
VOID
UserCopyMemory
    (
        PVOID                       pDest,
        PVOID                       pSrc,
        ULONG                       ulSize
    );
 */
#define  UserCopyMemory(pDest, pSrc, ulSize)        memcpy(pDest, pSrc, ulSize)


/*
 *  prototype
 *
VOID
UserZeroMemory
    (
        PVOID                       pMem,
        ULONG                       ulSize
    );
 */
#define  UserZeroMemory(pMem, ulSize)               memset(pMem, 0, ulSize)


/*
 *  prototype
 *
BOOLEAN
UserEqualMemory
    (
        PVOID                       pMem1,
        PVOID                       pMem2,
        ULONG                       ulSize
    );
 */
#define  UserEqualMemory(pMem1, pMem2, ulSize)      (memcmp(pMem1, pMem2, ulSize) == 0)

#define  UserGetFreeMemory()                        0xFFFFFFFF

#endif
