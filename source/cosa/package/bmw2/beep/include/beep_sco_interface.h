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

    module:	beep_sco_interface.h

        For BSP Execution Environment Plane Implementation (BEEP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Beep Srv Controller Object.

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


#ifndef  _BEEP_SCO_INTERFACE_
#define  _BEEP_SCO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"


/***********************************************************
       BEEP SRV CONTROLLER COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_BEEPSCO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPSCO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_BEEPSCO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPSCO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ANSC_STATUS
(*PFN_BEEPSCO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_BEEPSCO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_BEEPSCO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPSCO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPSCO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_BEEPSCO_GET_SBO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       file_ext
    );

typedef  ANSC_HANDLE
(*PFN_BEEPSCO_GET_SBO2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       page_path
    );

typedef  ANSC_STATUS
(*PFN_BEEPSCO_ADD_SBO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hServiceBean
    );

typedef  ANSC_STATUS
(*PFN_BEEPSCO_DEL_SBO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       engine_name
    );

typedef  ANSC_STATUS
(*PFN_BEEPSCO_DEL_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  char*
(*PFN_BEEPSCO_GET_PBO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       relative_path
    );

typedef  ANSC_STATUS
(*PFN_BEEPSCO_ADD_PBO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       relative_path,
        char*                       mapped_page
    );

typedef  ANSC_STATUS
(*PFN_BEEPSCO_DEL_PBO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       relative_path
    );

typedef  ANSC_STATUS
(*PFN_BEEPSCO_PROCESS)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHttpBmoReq,
        ANSC_HANDLE                 hHttpBmoRep
    );

/*
 * Beep Srv Controller Objet is responsible for managing multiple runtime Page Execution Engines,
 * which are instantiated by the Beep Service Bean Object. This object dispatches the HTTP requests
 * solely based on the file extensions (e.g., .bsp, .html, .xml and so on). The owner object is
 * responsible for breaking down a registered engine into multiple service beans.
 */
#define  BEEP_SRV_CONTROLLER_CLASS_CONTENT                                                  \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    BEEP_SRV_CONTROLLER_PROPERTY    Property;                                               \
    ANSC_HANDLE                     hAtoFileExt;                                            \
    ANSC_HANDLE                     hBeepPemIf;                                             \
    ANSC_HANDLE                     hDefServiceBean;                                        \
    BOOL                            bActive;                                                \
                                                                                            \
    QUEUE_HEADER                    SboQueue;                                               \
    ANSC_LOCK                       SboQueueLock;                                           \
    QUEUE_HEADER                    PboQueue;                                               \
    ANSC_LOCK                       PboQueueLock;                                           \
                                                                                            \
    PFN_BEEPSCO_GET_IF              GetBeepPemIf;                                           \
    PFN_BEEPSCO_SET_IF              SetBeepPemIf;                                           \
    PFN_BEEPSCO_GET_PROPERTY        GetProperty;                                            \
    PFN_BEEPSCO_SET_PROPERTY        SetProperty;                                            \
    PFN_BEEPSCO_RESET               ResetProperty;                                          \
    PFN_BEEPSCO_RESET               Reset;                                                  \
                                                                                            \
    PFN_BEEPSCO_ENGAGE              Engage;                                                 \
    PFN_BEEPSCO_CANCEL              Cancel;                                                 \
                                                                                            \
    PFN_BEEPSCO_GET_SBO             GetServiceBean;                                         \
    PFN_BEEPSCO_GET_SBO2            GetServiceBean2;                                        \
    PFN_BEEPSCO_ADD_SBO             AddServiceBean;                                         \
    PFN_BEEPSCO_DEL_SBO             DelServiceBean;                                         \
    PFN_BEEPSCO_DEL_ALL             DelAllServiceBeans;                                     \
                                                                                            \
    PFN_BEEPSCO_GET_PBO             GetPageBinding;                                         \
    PFN_BEEPSCO_ADD_PBO             AddPageBinding;                                         \
    PFN_BEEPSCO_DEL_PBO             DelPageBinding;                                         \
    PFN_BEEPSCO_DEL_ALL             DelAllPageBindings;                                     \
                                                                                            \
    PFN_BEEPSCO_PROCESS             Process;                                                \
    /* end of object class content */                                                       \

typedef  struct
_BEEP_SRV_CONTROLLER_OBJECT
{
    BEEP_SRV_CONTROLLER_CLASS_CONTENT
}
BEEP_SRV_CONTROLLER_OBJECT,  *PBEEP_SRV_CONTROLLER_OBJECT;

#define  ACCESS_BEEP_SRV_CONTROLLER_OBJECT(p)       \
         ACCESS_CONTAINER(p, BEEP_SRV_CONTROLLER_OBJECT, Linkage)


/***********************************************************
         BEEP SRV CONTROLLER PAGE BINDING DEFINITION
***********************************************************/

/*
 * Although Web Server supports an extensive layering structure via the Virtual Host and Resource
 * Owner entities, we have combined these two objects into one layer in BMW2 architecture. While it
 * simplifies the configuration and packaging of the BMW2, we lose the flexibility to differentiate
 * requests by the request URL within the servlet. Page-binding is workaround for this problem.
 */
typedef  struct
_BEEP_SCO_PAGE_BINDING
{
    SINGLE_LINK_ENTRY               Linkage;
    char*                           RelativePath;
    char*                           MappedPage;
}
BEEP_SCO_PAGE_BINDING,  *PBEEP_SCO_PAGE_BINDING;

#define  ACCESS_BEEP_SCO_PAGE_BINDING(p)            \
         ACCESS_CONTAINER(p, BEEP_SCO_PAGE_BINDING, Linkage)

#define  BeepScoFreePageBinding(page_binding)                                               \
         {                                                                                  \
            if ( page_binding->RelativePath )                                               \
            {                                                                               \
                AnscFreeMemory(page_binding->RelativePath);                                 \
                                                                                            \
                page_binding->RelativePath = NULL;                                          \
            }                                                                               \
                                                                                            \
            if ( page_binding->MappedPage )                                                 \
            {                                                                               \
                AnscFreeMemory(page_binding->MappedPage);                                   \
                                                                                            \
                page_binding->MappedPage = NULL;                                            \
            }                                                                               \
                                                                                            \
            AnscFreeMemory(page_binding);                                                   \
         }


#endif
