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

    module:	ansc_cbo_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for Circular Buffer Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        11/14/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_CBO_INTERFACE_
#define  _ANSC_CBO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"


/***********************************************************
   PLATFORM INDEPENDENT CIRCULAR BUFFER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  ANSC_CBO_FLAG_WRITE_OVERRIDE               0x00000001

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_CBO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_CBO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ULONG
(*PFN_CBO_GET_ULONG)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_CBO_SET_ULONG)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulValue
    );

typedef  ULONG
(*PFN_CBO_GET_POINTER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_CBO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_CBO_ALLOCATE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_CBO_FREE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_CBO_COPY_INTO)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulOffset1,
        ULONG                       ulOffset2,
        PVOID                       buffer,
        PULONG                      pulSize
    );

typedef  ANSC_STATUS
(*PFN_CBO_COPY_FROM)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulOffset1,
        ULONG                       ulOffset2,
        PVOID                       buffer,
        PULONG                      pulSize
    );

typedef  ULONG
(*PFN_CBO_GET_AREA_SIZE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulOffset1,
        ULONG                       ulOffset2
    );

typedef  ULONG
(*PFN_CBO_ADVANCE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulOffset,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_CBO_SYNC)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMinBehind,
        ULONG                       ulMaxBehind
    );

typedef  ANSC_STATUS
(*PFN_CBO_WRITE)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        PULONG                      pulSize
    );

typedef  ANSC_STATUS
(*PFN_CBO_READ)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        PULONG                      pulSize
    );

typedef  ANSC_STATUS
(*PFN_CBO_MAP)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        PULONG                      pulSize
    );

typedef  ANSC_STATUS
(*PFN_CBO_ACQUIRE)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppBuffer,
        PULONG                      pulSize
    );

typedef  ANSC_STATUS
(*PFN_CBO_RELEASE)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_CBO_ACQUIRE2)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulOffset,
        PVOID*                      ppBuffer,
        PULONG                      pulSize
    );

typedef  ANSC_STATUS
(*PFN_CBO_RELEASE2)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSize
    );

/*
 * Most policy configuration information is stored and transmitted in xml format. Compared to our
 * proprietary al_store based configuration format, xml not only provides more flexible and
 * powerful hierarchical structure, it can also be translated into other data format, such as html,
 * very easily via XSLT (eXtensible Stylesheet Language Transformation). This Xml Parser Object
 * takes a xml-based data stream as input, and exposes a read/write interface similar to al_store.
 */
#define  ANSC_CIRCULAR_BUFFER_CLASS_CONTENT                                                 \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    PVOID                           ScratchBuffer;                                          \
    ULONG                           BufferSize;                                             \
    ULONG                           Wpointer;                                               \
    ULONG                           Fpointer;                                               \
    ULONG                           Rpointer;                                               \
    ULONG                           WzoneSize;                                              \
    ULONG                           FzoneSize;                                              \
    ULONG                           RzoneSize;                                              \
    ULONG                           Flags;                                                  \
    ANSC_LOCK                       AccessLock;                                             \
                                                                                            \
    PFN_CBO_GET_ULONG               GetBufferSize;                                          \
    PFN_CBO_SET_ULONG               SetBufferSize;                                          \
    PFN_CBO_GET_ULONG               GetFlags;                                               \
    PFN_CBO_SET_ULONG               SetFlags;                                               \
    PFN_CBO_GET_POINTER             GetWpointer;                                            \
    PFN_CBO_GET_POINTER             GetFpointer;                                            \
    PFN_CBO_GET_POINTER             GetRpointer;                                            \
    PFN_CBO_GET_ULONG               GetWzoneSize;                                           \
    PFN_CBO_GET_ULONG               GetFzoneSize;                                           \
    PFN_CBO_GET_ULONG               GetRzoneSize;                                           \
    PFN_CBO_RESET                   Reset;                                                  \
                                                                                            \
    PFN_CBO_ALLOCATE                AllocateBuffer;                                         \
    PFN_CBO_FREE                    FreeBuffer;                                             \
    PFN_CBO_COPY_INTO               CopyIntoArea;                                           \
    PFN_CBO_COPY_FROM               CopyFromArea;                                           \
    PFN_CBO_GET_AREA_SIZE           GetAreaSize;                                            \
    PFN_CBO_ADVANCE                 AdvancePointer;                                         \
    PFN_CBO_SYNC                    SyncRpointer;                                           \
                                                                                            \
    PFN_CBO_WRITE                   Write;                                                  \
    PFN_CBO_ACQUIRE                 AcquireWb;                                              \
    PFN_CBO_RELEASE                 ReleaseWb;                                              \
    PFN_CBO_READ                    Read;                                                   \
    PFN_CBO_ACQUIRE                 AcquireRb;                                              \
    PFN_CBO_RELEASE                 ReleaseRb;                                              \
    PFN_CBO_MAP                     Map;                                                    \
    PFN_CBO_ACQUIRE2                AcquireMb;                                              \
    PFN_CBO_RELEASE2                ReleaseMb;                                              \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_CIRCULAR_BUFFER_OBJECT
{
    ANSC_CIRCULAR_BUFFER_CLASS_CONTENT
}
ANSC_CIRCULAR_BUFFER_OBJECT,  *PANSC_CIRCULAR_BUFFER_OBJECT;

#define  ACCESS_ANSC_CIRCULAR_BUFFER_OBJECT(p)      \
         ACCESS_CONTAINER(p, ANSC_CIRCULAR_BUFFER_OBJECT, Linkage)


#endif
