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

    module:	ansc_apmo_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Ansc Address Pool Manager Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/01/01    initial revision.
        08/02/01    single address search may result number of
                    matched pools; so the returned value from the
                    member function get_pool() must be modified.

**********************************************************************/


#ifndef  _ANSC_APMO_INTERFACE_
#define  _ANSC_APMO_INTERFACE_


/***********************************************************
     GENERAL ANSC ADDRESS POOL MANAGER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the container object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_ANSCAPMO_CREATE)
    (
        ANSC_HANDLE                 hOwnerContext
    );

typedef  ANSC_STATUS
(*PFN_ANSCAPMO_REMOVE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ANSCAPMO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ULONG
(*PFN_ANSCAPMO_GET_COUNT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_ANSCAPMO_MATCH_POOL)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPool
    );

typedef  ANSC_HANDLE
(*PFN_ANSCAPMO_GET_POOL)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSelector
    );

typedef  ANSC_STATUS
(*PFN_ANSCAPMO_ADD_POOL)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPool
    );

typedef  ANSC_STATUS
(*PFN_ANSCAPMO_DEL_POOL)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPool
    );

typedef  ANSC_STATUS
(*PFN_ANSCAPMO_DEL_ALL_POOLS)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * The Component Objects are the extensions to the basic container. Any software module that is
 * shared between two or more extension objects or controller objects shall be implemented as a
 * Container Object. The container itself actually can leverage some of functionalities provided
 * by the Container Objects.
 */
#define  ANSC_ADDR_POOL_MANAGER_CLASS_CONTENT                                               \
    /* start of object class content */                                                     \
    SINGLE_LINK_ENTRY               Linkage;                                                \
    ANSC_HANDLE                     hOwnerContext;                                          \
                                                                                            \
    PFN_ANSCAPMO_CREATE             Create;                                                 \
    PFN_ANSCAPMO_REMOVE             Remove;                                                 \
    PFN_ANSCAPMO_RESET              Reset;                                                  \
                                                                                            \
    PFN_ANSCAPMO_GET_COUNT          GetPoolCount;                                           \
    PFN_ANSCAPMO_MATCH_POOL         MatchPool;                                              \
                                                                                            \
    PFN_ANSCAPMO_GET_POOL           GetPool;                                                \
    PFN_ANSCAPMO_ADD_POOL           AddPool;                                                \
    PFN_ANSCAPMO_DEL_POOL           DelPool;                                                \
    PFN_ANSCAPMO_DEL_ALL_POOLS      DelAllPools;                                            \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_ADDR_POOL_MANAGER_OBJECT
{
    ANSC_ADDR_POOL_MANAGER_CLASS_CONTENT
}
ANSC_ADDR_POOL_MANAGER_OBJECT,  *PANSC_ADDR_POOL_MANAGER_OBJECT;

#define  ACCESS_ANSC_ADDR_POOL_MANAGER_OBJECT(p)    \
         ACCESS_CONTAINER(p, ANSC_ADDR_POOL_MANAGER_OBJECT, Linkage)


/***********************************************************
      ANSC ADDRESS POOL MANAGER IPV4 OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the container object definition.
 */
#define  ANSC_APMOIP4O_MAX_INDEX_COUNT              32

typedef  struct
_ANSC_APMO_IP4_INDEX
{
    ULONG                           Index;
    ANSC_HANDLE                     hArea;
}
ANSC_APMO_IP4_INDEX,  *PANSC_APMO_IP4_INDEX;

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ULONG
(*PFN_ANSCAPMOIP4O_GET_COUNT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ANSCAPMOIP4O_SET_COUNT)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCount
    );

typedef  ANSC_STATUS
(*PFN_ANSCAPMOIP4O_DEL_ALL_AREAS)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * Searching an address pair is much harder than searching a single address among a number of IP
 * address pools, which usually consists of address subnet pools and range pools. For this type
 * search, we usually devide the searching into two steps: first we identify the most matched pool
 * for either the source or destination address, then search another address within a much smaller
 * number of pools. So a hierarchical structure is needed.
 */
#define  ANSC_APMO_IP4_CLASS_CONTENT                                                        \
    /* duplication of the base object class content */                                      \
    ANSC_ADDR_POOL_MANAGER_CLASS_CONTENT                                                    \
    /* start of object class content */                                                     \
    ULONG                           MaxIndexCount;                                          \
    ULONG                           CurIndexCount;                                          \
    ULONG                           IndexBufferSize;                                        \
    PVOID                           IndexBuffer;                                            \
                                                                                            \
    QUEUE_HEADER                    PoolQueue;                                              \
    ANSC_LOCK                       PoolQueueLock;                                          \
    QUEUE_HEADER                    AreaQueue;                                              \
    ANSC_LOCK                       AreaQueueLock;                                          \
                                                                                            \
    PFN_ANSCAPMOIP4O_GET_COUNT      GetMaxIndexCount;                                       \
    PFN_ANSCAPMOIP4O_SET_COUNT      SetMaxIndexCount;                                       \
    PFN_ANSCAPMOIP4O_GET_COUNT      GetCurIndexCount;                                       \
                                                                                            \
    PFN_ANSCAPMOIP4O_DEL_ALL_AREAS  DelAllAreas;                                            \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_APMO_IP4_OBJECT
{
    ANSC_APMO_IP4_CLASS_CONTENT
}
ANSC_APMO_IP4_OBJECT,  *PANSC_APMO_IP4_OBJECT;

#define  ACCESS_ANSC_APMO_IP4_OBJECT(p)             \
         ACCESS_CONTAINER(p, ANSC_APMO_IP4_OBJECT, Linkage)


/***********************************************************
       ANSC ADDRESS POOL MANAGER TCP OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the container object definition.
 */
#define  ANSC_APMOTCPO_MAX_INDEX_COUNT              32

typedef  struct
_ANSC_APMO_TCP_INDEX
{
    ULONG                           Index;
    ANSC_HANDLE                     hArea;
}
ANSC_APMO_TCP_INDEX,  *PANSC_APMO_TCP_INDEX;

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ULONG
(*PFN_ANSCAPMOTCPO_GET_COUNT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ANSCAPMOTCPO_SET_COUNT)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCount
    );

typedef  ANSC_STATUS
(*PFN_ANSCAPMOTCPO_DEL_ALL_AREAS)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * Searching a port number pair is much harder than searching a single port among a number of Tcp
 * port number pools, which usually consists of port number range pools. For this type, we usually
 * devide the searching into two steps: first we identify the most matched pool for either the
 * source or destination port number, then search another address within a much smaller number of
 * pools. So a hierarchical structure is needed.
 */
#define  ANSC_APMO_TCP_CLASS_CONTENT                                                        \
    /* duplication of the base object class content */                                      \
    ANSC_ADDR_POOL_MANAGER_CLASS_CONTENT                                                    \
    /* start of object class content */                                                     \
    ULONG                           MaxIndexCount;                                          \
    ULONG                           CurIndexCount;                                          \
    ULONG                           IndexBufferSize;                                        \
    PVOID                           IndexBuffer;                                            \
                                                                                            \
    QUEUE_HEADER                    PoolQueue;                                              \
    ANSC_LOCK                       PoolQueueLock;                                          \
    QUEUE_HEADER                    AreaQueue;                                              \
    ANSC_LOCK                       AreaQueueLock;                                          \
                                                                                            \
    PFN_ANSCAPMOTCPO_GET_COUNT      GetMaxIndexCount;                                       \
    PFN_ANSCAPMOTCPO_SET_COUNT      SetMaxIndexCount;                                       \
    PFN_ANSCAPMOTCPO_GET_COUNT      GetCurIndexCount;                                       \
                                                                                            \
    PFN_ANSCAPMOTCPO_DEL_ALL_AREAS  DelAllAreas;                                            \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_APMO_TCP_OBJECT
{
    ANSC_APMO_TCP_CLASS_CONTENT
}
ANSC_APMO_TCP_OBJECT,  *PANSC_APMO_TCP_OBJECT;

#define  ACCESS_ANSC_APMO_TCP_OBJECT(p)             \
         ACCESS_CONTAINER(p, ANSC_APMO_TCP_OBJECT, Linkage)


/***********************************************************
       ANSC ADDRESS POOL MANAGER UDP OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the container object definition.
 */
#define  ANSC_APMOUDPO_MAX_INDEX_COUNT              32

typedef  struct
_ANSC_APMO_UDP_INDEX
{
    ULONG                           Index;
    ANSC_HANDLE                     hArea;
}
ANSC_APMO_UDP_INDEX,  *PANSC_APMO_UDP_INDEX;

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ULONG
(*PFN_ANSCAPMOUDPO_GET_COUNT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ANSCAPMOUDPO_SET_COUNT)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCount
    );

typedef  ANSC_STATUS
(*PFN_ANSCAPMOUDPO_DEL_ALL_AREAS)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * Searching a port number pair is much harder than searching a single port among a number of Udp
 * port number pools, which usually consists of port number range pools. For this type, we usually
 * devide the searching into two steps: first we identify the most matched pool for either the
 * source or destination port number, then search another address within a much smaller number of
 * pools. So a hierarchical structure is needed.
 */
#define  ANSC_APMO_UDP_CLASS_CONTENT                                                        \
    /* duplication of the base object class content */                                      \
    ANSC_ADDR_POOL_MANAGER_CLASS_CONTENT                                                    \
    /* start of object class content */                                                     \
    ULONG                           MaxIndexCount;                                          \
    ULONG                           CurIndexCount;                                          \
    ULONG                           IndexBufferSize;                                        \
    PVOID                           IndexBuffer;                                            \
                                                                                            \
    QUEUE_HEADER                    PoolQueue;                                              \
    ANSC_LOCK                       PoolQueueLock;                                          \
    QUEUE_HEADER                    AreaQueue;                                              \
    ANSC_LOCK                       AreaQueueLock;                                          \
                                                                                            \
    PFN_ANSCAPMOUDPO_GET_COUNT      GetMaxIndexCount;                                       \
    PFN_ANSCAPMOUDPO_SET_COUNT      SetMaxIndexCount;                                       \
    PFN_ANSCAPMOUDPO_GET_COUNT      GetCurIndexCount;                                       \
                                                                                            \
    PFN_ANSCAPMOUDPO_DEL_ALL_AREAS  DelAllAreas;                                            \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_APMO_UDP_OBJECT
{
    ANSC_APMO_UDP_CLASS_CONTENT
}
ANSC_APMO_UDP_OBJECT,  *PANSC_APMO_UDP_OBJECT;

#define  ACCESS_ANSC_APMO_UDP_OBJECT(p)             \
         ACCESS_CONTAINER(p, ANSC_APMO_UDP_OBJECT, Linkage)


/***********************************************************
       ANSC ADDRESS POOL MANAGER SPI OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the container object definition.
 */
#define  ANSC_APMOSPIO_MAX_INDEX_COUNT              32

typedef  struct
_ANSC_APMO_SPI_INDEX
{
    ULONG                           Index;
    ANSC_HANDLE                     hArea;
}
ANSC_APMO_SPI_INDEX,  *PANSC_APMO_SPI_INDEX;

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ULONG
(*PFN_ANSCAPMOSPIO_GET_COUNT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ANSCAPMOSPIO_SET_COUNT)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCount
    );

typedef  ANSC_STATUS
(*PFN_ANSCAPMOSPIO_DEL_ALL_AREAS)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * Searching a port number pair is much harder than searching a single port among a number of Spi
 * port number pools, which usually consists of port number range pools. For this type, we usually
 * devide the searching into two steps: first we identify the most matched pool for either the
 * source or destination port number, then search another address within a much smaller number of
 * pools. So a hierarchical structure is needed.
 */
#define  ANSC_APMO_SPI_CLASS_CONTENT                                                        \
    /* duplication of the base object class content */                                      \
    ANSC_ADDR_POOL_MANAGER_CLASS_CONTENT                                                    \
    /* start of object class content */                                                     \
    ULONG                           MaxIndexCount;                                          \
    ULONG                           CurIndexCount;                                          \
    ULONG                           IndexBufferSize;                                        \
    PVOID                           IndexBuffer;                                            \
                                                                                            \
    QUEUE_HEADER                    PoolQueue;                                              \
    ANSC_LOCK                       PoolQueueLock;                                          \
    QUEUE_HEADER                    AreaQueue;                                              \
    ANSC_LOCK                       AreaQueueLock;                                          \
                                                                                            \
    PFN_ANSCAPMOSPIO_GET_COUNT      GetMaxIndexCount;                                       \
    PFN_ANSCAPMOSPIO_SET_COUNT      SetMaxIndexCount;                                       \
    PFN_ANSCAPMOSPIO_GET_COUNT      GetCurIndexCount;                                       \
                                                                                            \
    PFN_ANSCAPMOSPIO_DEL_ALL_AREAS  DelAllAreas;                                            \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_APMO_SPI_OBJECT
{
    ANSC_APMO_SPI_CLASS_CONTENT
}
ANSC_APMO_SPI_OBJECT,  *PANSC_APMO_SPI_OBJECT;

#define  ACCESS_ANSC_APMO_SPI_OBJECT(p)             \
         ACCESS_CONTAINER(p, ANSC_APMO_SPI_OBJECT, Linkage)


#endif
