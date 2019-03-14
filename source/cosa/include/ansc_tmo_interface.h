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

    module:	ansc_tmo_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Ansc Trunk Manager Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/01/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_TMO_INTERFACE_
#define  _ANSC_TMO_INTERFACE_


/***********************************************************
        GENERAL ANSC TRUNK MANAGER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the container object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_ANSCTMO_CREATE)
    (
        ANSC_HANDLE                 hOwnerContext
    );

typedef  ANSC_STATUS
(*PFN_ANSCTMO_REMOVE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ANSCTMO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_ANSCTMO_GET_TRUNK)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCircuit
    );

typedef  ANSC_STATUS
(*PFN_ANSCTMO_ADD_TRUNK)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTrunk
    );

typedef  ANSC_STATUS
(*PFN_ANSCTMO_DEL_TRUNK)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTrunk
    );

typedef  ANSC_STATUS
(*PFN_ANSCTMO_DEL_ALL_TRUNKS)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * The Component Objects are the extensions to the basic container. Any software module that is
 * shared between two or more extension objects or controller objects shall be implemented as a
 * Container Object. The container itself actually can leverage some of functionalities provided
 * by the Container Objects.
 */
#define  ANSC_TRUNK_MANAGER_CLASS_CONTENT                                                   \
    /* start of object class content */                                                     \
    SINGLE_LINK_ENTRY               Linkage;                                                \
    ANSC_HANDLE                     hOwnerContext;                                          \
                                                                                            \
    PFN_ANSCTMO_CREATE              Create;                                                 \
    PFN_ANSCTMO_REMOVE              Remove;                                                 \
    PFN_ANSCTMO_RESET               Reset;                                                  \
                                                                                            \
    PFN_ANSCTMO_GET_TRUNK           GetTrunk;                                               \
    PFN_ANSCTMO_ADD_TRUNK           AddTrunk;                                               \
    PFN_ANSCTMO_DEL_TRUNK           DelTrunk;                                               \
    PFN_ANSCTMO_DEL_ALL_TRUNKS      DelAllTrunks;                                           \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_TRUNK_MANAGER_OBJECT
{
    ANSC_TRUNK_MANAGER_CLASS_CONTENT
}
ANSC_TRUNK_MANAGER_OBJECT,  *PANSC_TRUNK_MANAGER_OBJECT;

#define  ACCESS_ANSC_TRUNK_MANAGER_OBJECT(p)        \
         ACCESS_CONTAINER(p, ANSC_TRUNK_MANAGER_OBJECT, Linkage)


/***********************************************************
          ANSC TRUNK MANAGER IPV4 OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the container object definition.
 */
#define  ANSC_TMO_IP4_SORTING_SRC_DST               0x00000001
#define  ANSC_TMO_IP4_SORTING_DST_SRC               0x00000002

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_ANSCTMOIP4O_EXTRACT_POOL)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTrunk
    );

typedef  ULONG
(*PFN_ANSCTMOIP4O_GET_GRAMMAR)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ANSCTMOIP4O_SET_GRAMMAR)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       grammar
    );

/*
 * Searching an address pair is much harder than searching a single address among a number of IP
 * address pools, which usually consists of address subnet pools and range pools. For this type
 * search, we usually devide the searching into two steps: first we identify the most matched pool
 * for either the source or destination address, then search another address within a much smaller
 * number of pools. So a hierarchical structure is needed.
 */
#define  ANSC_TMO_IP4_CLASS_CONTENT                                                         \
    /* duplication of the base object class content */                                      \
    ANSC_TRUNK_MANAGER_CLASS_CONTENT                                                        \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hAddrPoolManager;                                       \
    ULONG                           SortingGrammar;                                         \
                                                                                            \
    QUEUE_HEADER                    TrunkQueue;                                             \
    ANSC_LOCK                       TrunkQueueLock;                                         \
                                                                                            \
    PFN_ANSCTMOIP4O_EXTRACT_POOL    ExtractSrcAddrPool;                                     \
    PFN_ANSCTMOIP4O_EXTRACT_POOL    ExtractDstAddrPool;                                     \
                                                                                            \
    PFN_ANSCTMOIP4O_GET_GRAMMAR     GetSortingGrammar;                                      \
    PFN_ANSCTMOIP4O_SET_GRAMMAR     SetSortingGrammar;                                      \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_TMO_IP4_OBJECT
{
    ANSC_TMO_IP4_CLASS_CONTENT
}
ANSC_TMO_IP4_OBJECT,  *PANSC_TMO_IP4_OBJECT;

#define  ACCESS_ANSC_TMO_IP4_OBJECT(p)              \
         ACCESS_CONTAINER(p, ANSC_TMO_IP4_OBJECT, Linkage)


/***********************************************************
           ANSC TRUNK MANAGER TCP OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the container object definition.
 */
#define  ANSC_TMO_TCP_SORTING_SRC_DST               0x00000001
#define  ANSC_TMO_TCP_SORTING_DST_SRC               0x00000002

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_ANSCTMOTCPO_EXTRACT_POOL)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTrunk
    );

typedef  ULONG
(*PFN_ANSCTMOTCPO_GET_GRAMMAR)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ANSCTMOTCPO_SET_GRAMMAR)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       grammar
    );

/*
 * Searching a port number pair is much harder than searching a single port among a number of Tcp
 * port number pools, which usually consists of port number range pools. For this type, we usually
 * devide the searching into two steps: first we identify the most matched pool for either the
 * source or destination port number, then search another address within a much smaller number of
 * pools. So a hierarchical structure is needed.
 */
#define  ANSC_TMO_TCP_CLASS_CONTENT                                                         \
    /* duplication of the base object class content */                                      \
    ANSC_TRUNK_MANAGER_CLASS_CONTENT                                                        \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hAddrPoolManager;                                       \
    ULONG                           SortingGrammar;                                         \
                                                                                            \
    QUEUE_HEADER                    TrunkQueue;                                             \
    ANSC_LOCK                       TrunkQueueLock;                                         \
                                                                                            \
    PFN_ANSCTMOTCPO_EXTRACT_POOL    ExtractSrcAddrPool;                                     \
    PFN_ANSCTMOTCPO_EXTRACT_POOL    ExtractDstAddrPool;                                     \
                                                                                            \
    PFN_ANSCTMOTCPO_GET_GRAMMAR     GetSortingGrammar;                                      \
    PFN_ANSCTMOTCPO_SET_GRAMMAR     SetSortingGrammar;                                      \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_TMO_TCP_OBJECT
{
    ANSC_TMO_TCP_CLASS_CONTENT
}
ANSC_TMO_TCP_OBJECT,  *PANSC_TMO_TCP_OBJECT;

#define  ACCESS_ANSC_TMO_TCP_OBJECT(p)              \
         ACCESS_CONTAINER(p, ANSC_TMO_TCP_OBJECT, Linkage)


/***********************************************************
           ANSC TRUNK MANAGER UDP OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the container object definition.
 */
#define  ANSC_TMO_UDP_SORTING_SRC_DST               0x00000001
#define  ANSC_TMO_UDP_SORTING_DST_SRC               0x00000002

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_ANSCTMOUDPO_EXTRACT_POOL)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTrunk
    );

typedef  ULONG
(*PFN_ANSCTMOUDPO_GET_GRAMMAR)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ANSCTMOUDPO_SET_GRAMMAR)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       grammar
    );

/*
 * Searching a port number pair is much harder than searching a single port among a number of Tcp
 * port number pools, which usually consists of port number range pools. For this type, we usually
 * devide the searching into two steps: first we identify the most matched pool for either the
 * source or destination port number, then search another address within a much smaller number of
 * pools. So a hierarchical structure is needed.
 */
#define  ANSC_TMO_UDP_CLASS_CONTENT                                                         \
    /* duplication of the base object class content */                                      \
    ANSC_TRUNK_MANAGER_CLASS_CONTENT                                                        \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hAddrPoolManager;                                       \
    ULONG                           SortingGrammar;                                         \
                                                                                            \
    QUEUE_HEADER                    TrunkQueue;                                             \
    ANSC_LOCK                       TrunkQueueLock;                                         \
                                                                                            \
    PFN_ANSCTMOUDPO_EXTRACT_POOL    ExtractSrcAddrPool;                                     \
    PFN_ANSCTMOUDPO_EXTRACT_POOL    ExtractDstAddrPool;                                     \
                                                                                            \
    PFN_ANSCTMOUDPO_GET_GRAMMAR     GetSortingGrammar;                                      \
    PFN_ANSCTMOUDPO_SET_GRAMMAR     SetSortingGrammar;                                      \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_TMO_UDP_OBJECT
{
    ANSC_TMO_UDP_CLASS_CONTENT
}
ANSC_TMO_UDP_OBJECT,  *PANSC_TMO_UDP_OBJECT;

#define  ACCESS_ANSC_TMO_UDP_OBJECT(p)              \
         ACCESS_CONTAINER(p, ANSC_TMO_UDP_OBJECT, Linkage)


/***********************************************************
           ANSC TRUNK MANAGER ESP OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the container object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_ANSCTMOESPO_EXTRACT_POOL)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTrunk
    );

/*
 * Searching a port number pair is much harder than searching a single port among a number of Tcp
 * port number pools, which usually consists of port number range pools. For this type, we usually
 * devide the searching into two steps: first we identify the most matched pool for either the
 * source or destination port number, then search another address within a much smaller number of
 * pools. So a hierarchical structure is needed.
 */
#define  ANSC_TMO_ESP_CLASS_CONTENT                                                         \
    /* duplication of the base object class content */                                      \
    ANSC_TRUNK_MANAGER_CLASS_CONTENT                                                        \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hAddrPoolManager;                                       \
                                                                                            \
    QUEUE_HEADER                    TrunkQueue;                                             \
    ANSC_LOCK                       TrunkQueueLock;                                         \
                                                                                            \
    PFN_ANSCTMOESPO_EXTRACT_POOL    ExtractAddrPool;                                        \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_TMO_ESP_OBJECT
{
    ANSC_TMO_ESP_CLASS_CONTENT
}
ANSC_TMO_ESP_OBJECT,  *PANSC_TMO_ESP_OBJECT;

#define  ACCESS_ANSC_TMO_ESP_OBJECT(p)              \
         ACCESS_CONTAINER(p, ANSC_TMO_ESP_OBJECT, Linkage)


/***********************************************************
            ANSC TRUNK MANAGER AH OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the container object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_ANSCTMOAHO_EXTRACT_POOL)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTrunk
    );

/*
 * Searching a port number pair is much harder than searching a single port among a number of Tcp
 * port number pools, which usually consists of port number range pools. For this type, we usually
 * devide the searching into two steps: first we identify the most matched pool for either the
 * source or destination port number, then search another address within a much smaller number of
 * pools. So a hierarchical structure is needed.
 */
#define  ANSC_TMO_AH_CLASS_CONTENT                                                          \
    /* duplication of the base object class content */                                      \
    ANSC_TRUNK_MANAGER_CLASS_CONTENT                                                        \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hAddrPoolManager;                                       \
                                                                                            \
    QUEUE_HEADER                    TrunkQueue;                                             \
    ANSC_LOCK                       TrunkQueueLock;                                         \
                                                                                            \
    PFN_ANSCTMOAHO_EXTRACT_POOL     ExtractAddrPool;                                        \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_TMO_AH_OBJECT
{
    ANSC_TMO_AH_CLASS_CONTENT
}
ANSC_TMO_AH_OBJECT,  *PANSC_TMO_AH_OBJECT;

#define  ACCESS_ANSC_TMO_AH_OBJECT(p)               \
         ACCESS_CONTAINER(p, ANSC_TMO_AH_OBJECT, Linkage)


/***********************************************************
         ANSC TRUNK MANAGER IPCOMP OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the container object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_ANSCTMOIPCOMPO_EXTRACT_POOL)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTrunk
    );

/*
 * Searching a port number pair is much harder than searching a single port among a number of Tcp
 * port number pools, which usually consists of port number range pools. For this type, we usually
 * devide the searching into two steps: first we identify the most matched pool for either the
 * source or destination port number, then search another address within a much smaller number of
 * pools. So a hierarchical structure is needed.
 */
#define  ANSC_TMO_IPCOMP_CLASS_CONTENT                                                      \
    /* duplication of the base object class content */                                      \
    ANSC_TRUNK_MANAGER_CLASS_CONTENT                                                        \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hAddrPoolManager;                                       \
                                                                                            \
    QUEUE_HEADER                    TrunkQueue;                                             \
    ANSC_LOCK                       TrunkQueueLock;                                         \
                                                                                            \
    PFN_ANSCTMOIPCOMPO_EXTRACT_POOL ExtractAddrPool;                                        \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_TMO_IPCOMP_OBJECT
{
    ANSC_TMO_IPCOMP_CLASS_CONTENT
}
ANSC_TMO_IPCOMP_OBJECT,  *PANSC_TMO_IPCOMP_OBJECT;

#define  ACCESS_ANSC_TMO_IPCOMP_OBJECT(p)           \
         ACCESS_CONTAINER(p, ANSC_TMO_IPCOMP_OBJECT, Linkage)


#endif
