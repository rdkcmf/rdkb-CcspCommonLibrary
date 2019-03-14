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

    module:	beep_pco_interface.h

        For BSP Execution Environment Plane Implementation (BEEP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Beep Page Container Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/07/03    initial revision.

**********************************************************************/


#ifndef  _BEEP_PCO_INTERFACE_
#define  _BEEP_PCO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"

#include "beep_ifo_pec.h"


/***********************************************************
       BEEP PAGE CONTAINER COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  BEEP_PCO_ORO_TABLE_SIZE                    32

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_BEEPPCO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPPCO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_BEEPPCO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPPCO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  char*
(*PFN_BEEPPCO_GET_PATH)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPPCO_SET_PATH)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       page_path
    );

typedef  ULONG
(*PFN_BEEPPCO_GET_TIME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPPCO_SET_TIME)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulTimestamp
    );

typedef  BOOL
(*PFN_BEEPPCO_GET_BOOL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPPCO_SET_BOOL)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnabled
    );

typedef  ANSC_STATUS
(*PFN_BEEPPCO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ULONG
(*PFN_BEEPPCO_GET_REF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ULONG
(*PFN_BEEPPCO_INC_REF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ULONG
(*PFN_BEEPPCO_DEC_REF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPPCO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPPCO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPPCO_SETUP_ENV)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPPCO_CLOSE_ENV)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  BOOL
(*PFN_BEEPPCO_MATCH_PATH)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       page_path
    );

typedef  ANSC_STATUS
(*PFN_BEEPPCO_TERMINATE)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * Beep Page Container Objet must be instantiated for every page execution request. This object
 * provides a generic BEEP/SLAP object access interface to the dynamic page execution engines. Not
 * only it inherits the application and session level SLAP containers from other objects, it also
 * creates the page level container, which constructs the operational environment for the Page
 * object.
 */
#define  BEEP_PAGE_CONTAINER_CLASS_CONTENT                                                  \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hBeepPemIf;                                             \
    ANSC_HANDLE                     hSlapContainerPage;                                     \
    ANSC_HANDLE                     hSlapBeepPage;                                          \
    char*                           PagePath;                                               \
    ULONG                           Timestamp;                                              \
    BOOL                            bPersistent;                                            \
    BOOL                            bActive;                                                \
    ULONG                           RefCount;                                               \
                                                                                            \
    PFN_BEEPPCO_GET_IF              GetBeepPemIf;                                           \
    PFN_BEEPPCO_SET_IF              SetBeepPemIf;                                           \
    PFN_BEEPPCO_GET_CONTEXT         GetSlapContainerPage;                                   \
    PFN_BEEPPCO_GET_CONTEXT         GetSlapBeepPage;                                        \
    PFN_BEEPPCO_GET_PATH            GetPagePath;                                            \
    PFN_BEEPPCO_SET_PATH            SetPagePath;                                            \
    PFN_BEEPPCO_GET_TIME            GetTimestamp;                                           \
    PFN_BEEPPCO_SET_TIME            SetTimestamp;                                           \
    PFN_BEEPPCO_GET_BOOL            GetPersistent;                                          \
    PFN_BEEPPCO_SET_BOOL            SetPersistent;                                          \
    PFN_BEEPPCO_RESET               Reset;                                                  \
                                                                                            \
    PFN_BEEPPCO_GET_REF             GetRef;                                                 \
    PFN_BEEPPCO_INC_REF             IncRef;                                                 \
    PFN_BEEPPCO_DEC_REF             DecRef;                                                 \
    PFN_BEEPPCO_ENGAGE              Engage;                                                 \
    PFN_BEEPPCO_CANCEL              Cancel;                                                 \
    PFN_BEEPPCO_SETUP_ENV           SetupEnv;                                               \
    PFN_BEEPPCO_CLOSE_ENV           CloseEnv;                                               \
                                                                                            \
    PFN_BEEPPCO_MATCH_PATH          MatchPagePath;                                          \
    PFN_BEEPPCO_TERMINATE           Terminate;                                              \
    /* end of object class content */                                                       \

typedef  struct
_BEEP_PAGE_CONTAINER_OBJECT
{
    BEEP_PAGE_CONTAINER_CLASS_CONTENT
}
BEEP_PAGE_CONTAINER_OBJECT,  *PBEEP_PAGE_CONTAINER_OBJECT;

#define  ACCESS_BEEP_PAGE_CONTAINER_OBJECT(p)       \
         ACCESS_CONTAINER(p, BEEP_PAGE_CONTAINER_OBJECT, Linkage)


#endif
