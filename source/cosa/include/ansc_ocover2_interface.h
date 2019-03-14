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

    module:	ansc_ocover2_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Ansc OcoVer2 Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        12/07/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_OCOVER2_INTERFACE_
#define  _ANSC_OCOVER2_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ocover1_interface.h"
#include "ansc_ocover1_external_api.h"


/***********************************************************
   PLATFORM INDEPENDENT OBJECT CONTAINER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_OCOVER2_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_OCOVER2_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_STATUS
(*PFN_OCOVER2_REGISTER_TASK)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTask
    );

typedef  ANSC_STATUS
(*PFN_OCOVER2_CANCEL_TASK)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTask
    );

/*
 * As a standard practice, we encapsulate all functional and feature objects inside an Object
 * Container Object so it will function as a black-box implementation when we have to integrate
 * an Ansc component with other systems. However, that's not the only reason why we need to
 * define the Container Object: we also use container as the building block for constructing
 * larger systems.
 */
#define  ANSC_OCO_VER2_CLASS_CONTENT                                                        \
    /* duplication of the base object class content */                                      \
    ANSC_OCO_VER1_CLASS_CONTENT                                                             \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hTaskOrganizer;                                         \
                                                                                            \
    PFN_OCOVER2_GET_CONTEXT         GetToo;                                                 \
    PFN_OCOVER2_SET_CONTEXT         SetToo;                                                 \
                                                                                            \
    PFN_OCOVER2_REGISTER_TASK       RegisterTask;                                           \
    PFN_OCOVER2_CANCEL_TASK         CancelTask;                                             \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_OCO_VER2_OBJECT
{
    ANSC_OCO_VER2_CLASS_CONTENT
}
ANSC_OCO_VER2_OBJECT,  *PANSC_OCO_VER2_OBJECT;

#define  ACCESS_ANSC_OCO_VER2_OBJECT(p)             \
         ACCESS_CONTAINER(p, ANSC_OCO_VER2_OBJECT, Linkage)


#endif
