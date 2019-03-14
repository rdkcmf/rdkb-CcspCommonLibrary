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

    module:	ansc_cover3_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the general Ansc CoVer3 Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/30/02    initial revision.

**********************************************************************/


#ifndef  _ANSC_COVER3_INTERFACE_
#define  _ANSC_COVER3_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"


/***********************************************************
        GENERAL ANSC COMPONENT OBJECT VER3 DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the container object definition.
 */
#define  ANSC_CO_VER3_NAME                          "coVer3"
#define  ANSC_CO_VER3_OID                           0xFFFFFFFF

#define  ANSC_COVER3_ENGINE_TASK_NAME               "cover3_engine_task"

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_ANSCCOVER3_SHUTDOWN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ANSCCOVER3_SPAWN)
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pTaskEntry,
        ANSC_HANDLE                 hTaskContext,
        char*                       name
    );

typedef  ANSC_STATUS
(*PFN_ANSCCOVER3_SPAWN2)
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pTaskEntry,
        ANSC_HANDLE                 hTaskContext,
        char*                       name,
        int                         priority
    );

typedef  ANSC_STATUS
(*PFN_ANSCCOVER3_SPAWN3)
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pTaskEntry,
        ANSC_HANDLE                 hTaskContext,
        char*                       name,
        int                         priority,
        ULONG                       stack_size
    );

typedef  ANSC_STATUS
(*PFN_ANSCCOVER3_ENGINE_TASK)
    (
        ANSC_HANDLE                 hTaskInfo
    );

/*
 * The Component Objects are the extensions to the basic container. Any software module that is
 * shared between two or more extension objects or controller objects shall be implemented as a
 * Container Object. The container itself actually can leverage some of functionalities provided
 * by the Container Objects.
 */
#define  ANSCCO_VER3_CLASS_CONTENT                                                          \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    BOOL                            bShuttingDown;                                          \
    ULONG                           EngineTaskCount;                                        \
    ANSC_LOCK                       EtoAccessLock;                                          \
                                                                                            \
    SLIST_HEADER                    TaskInfoSList;                                          \
    ANSC_LOCK                       TaskInfoSListLock;                                      \
                                                                                            \
    PFN_ANSCCOVER3_SHUTDOWN         Shutdown;                                               \
                                                                                            \
    PFN_ANSCCOVER3_SPAWN            SpawnTask;                                              \
    PFN_ANSCCOVER3_SPAWN2           SpawnTask2;                                             \
    PFN_ANSCCOVER3_SPAWN3           SpawnTask3;                                             \
    PFN_ANSCCOVER3_ENGINE_TASK      EngineTask;                                             \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_CO_VER3_OBJECT
{
    ANSCCO_VER3_CLASS_CONTENT
}
ANSC_CO_VER3_OBJECT,  *PANSC_CO_VER3_OBJECT;

#define  ACCESS_ANSC_CO_VER3_OBJECT(p)              \
         ACCESS_CONTAINER(p, ANSC_CO_VER3_OBJECT, Linkage)


/***********************************************************
         COVER3 ENGINE TASK OBJECT ENTRY DEFINITION
***********************************************************/

/*
 * We have simplified the task-spawning process by mandating the one and only input parameter for
 * each engine task to be the object itself. The task name is fixed too. However, we do have to
 * remember the entry point for the newly created task.
 */
typedef  ANSC_STATUS
(*PFN_ANSC_COVER3_TASK_ROUTINE)
    (
        ANSC_HANDLE                 hTaskContext
    );

typedef  struct
_ANSC_COVER3_TASK_INFO
{
    SINGLE_LINK_ENTRY               Linkage;
    ANSC_HANDLE                     hTaskContext;
    PFN_ANSC_COVER3_TASK_ROUTINE    Routine;
}
ANSC_COVER3_TASK_INFO,  *PANSC_COVER3_TASK_INFO;

#define  ACCESS_ANSC_COVER3_TASK_INFO(p)            \
         ACCESS_CONTAINER(p, ANSC_COVER3_TASK_INFO, Linkage)


#endif
