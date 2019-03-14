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

    module:	beep_cso_interface.h

        For BSP Execution Environment Plane Implementation (BEEP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Beep Client Session Object.

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


#ifndef  _BEEP_CSO_INTERFACE_
#define  _BEEP_CSO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"


/***********************************************************
       BEEP CLIENT SESSION COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  BEEP_CSO_PCO_TABLE_SIZE                    32

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_BEEPCSO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPCSO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_BEEPCSO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPCSO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ULONG
(*PFN_BEEPCSO_GET_TIME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPCSO_SET_TIME)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       timeout
    );

typedef  char*
(*PFN_BEEPCSO_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPCSO_SET_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  BOOL
(*PFN_BEEPCSO_IS_SOMETHING)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPCSO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPCSO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPCSO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPCSO_SETUP_ENV)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPCSO_CLOSE_ENV)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_BEEPCSO_GET_PCO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       page_path
    );

typedef  ANSC_STATUS
(*PFN_BEEPCSO_ADD_PCO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPageContainer
    );

typedef  ANSC_STATUS
(*PFN_BEEPCSO_DEL_PCO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       page_path
    );

typedef  ANSC_STATUS
(*PFN_BEEPCSO_DEL_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPCSO_TERMINATE)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * Beep Client Session Objet is the generic representation of the web browsing session at the BEEP
 * level. This object doesn't manage user profile or perform anything related user authentication,
 * such functionalities should be encapsulated in the dynamic pages. Such separation offers a very
 * flexible framework, which can be used in radically different web site designs.
 */
#define  BEEP_CLIENT_SESSION_CLASS_CONTENT                                                  \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hBeepPemIf;                                             \
    ANSC_HANDLE                     hWebGeneralSession;                                     \
    ANSC_HANDLE                     hSlapContainerSession;                                  \
    ANSC_HANDLE                     hSlapBeepSession;                                       \
    BOOL                            bAuthenticated;                                         \
    BOOL                            bActive;                                                \
                                                                                            \
    SLIST_HEADER                    PcoTable[BEEP_CSO_PCO_TABLE_SIZE];                      \
    ANSC_LOCK                       PcoTableLock;                                           \
                                                                                            \
    PFN_BEEPCSO_GET_IF              GetBeepPemIf;                                           \
    PFN_BEEPCSO_SET_IF              SetBeepPemIf;                                           \
    PFN_BEEPCSO_GET_CONTEXT         GetWebGeneralSession;                                   \
    PFN_BEEPCSO_SET_CONTEXT         SetWebGeneralSession;                                   \
    PFN_BEEPCSO_GET_TIME            GetTimeout;                                             \
    PFN_BEEPCSO_SET_TIME            SetTimeout;                                             \
    PFN_BEEPCSO_GET_NAME            GetClientIdentifier;                                    \
    PFN_BEEPCSO_SET_NAME            SetClientIdentifier;                                    \
                                                                                            \
    PFN_BEEPCSO_IS_SOMETHING        IsClientUnknown;                                        \
    PFN_BEEPCSO_GET_CONTEXT         GetSlapContainerSession;                                \
    PFN_BEEPCSO_GET_CONTEXT         GetSlapBeepSession;                                     \
    PFN_BEEPCSO_RESET               Reset;                                                  \
                                                                                            \
    PFN_BEEPCSO_ENGAGE              Engage;                                                 \
    PFN_BEEPCSO_CANCEL              Cancel;                                                 \
    PFN_BEEPCSO_SETUP_ENV           SetupEnv;                                               \
    PFN_BEEPCSO_CLOSE_ENV           CloseEnv;                                               \
                                                                                            \
    PFN_BEEPCSO_GET_PCO             GetPageContainer;                                       \
    PFN_BEEPCSO_ADD_PCO             AddPageContainer;                                       \
    PFN_BEEPCSO_DEL_PCO             DelPageContainer;                                       \
    PFN_BEEPCSO_DEL_ALL             DelAllPageContainers;                                   \
                                                                                            \
    PFN_BEEPCSO_TERMINATE           Terminate;                                              \
    /* end of object class content */                                                       \

typedef  struct
_BEEP_CLIENT_SESSION_OBJECT
{
    BEEP_CLIENT_SESSION_CLASS_CONTENT
}
BEEP_CLIENT_SESSION_OBJECT,  *PBEEP_CLIENT_SESSION_OBJECT;

#define  ACCESS_BEEP_CLIENT_SESSION_OBJECT(p)       \
         ACCESS_CONTAINER(p, BEEP_CLIENT_SESSION_OBJECT, Linkage)


#endif
