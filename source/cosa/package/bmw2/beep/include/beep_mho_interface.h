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

    module:	beep_mho_interface.h

        For BSP Execution Environment Plane Implementation (BEEP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Beep Master Host Object.

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


#ifndef  _BEEP_MHO_INTERFACE_
#define  _BEEP_MHO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "beep_properties.h"


/***********************************************************
         BEEP MASTER HOST COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_BEEPMHO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPMHO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_BEEPMHO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPMHO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ANSC_STATUS
(*PFN_BEEPMHO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_BEEPMHO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_BEEPMHO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  char*
(*PFN_BEEPMHO_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPMHO_SET_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  USHORT
(*PFN_BEEPMHO_GET_PORT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPMHO_SET_PORT)
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      port
    );

typedef  ANSC_STATUS
(*PFN_BEEPMHO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPMHO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPMHO_NEW)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession
    );

typedef  ANSC_STATUS
(*PFN_BEEPMHO_CLASSIFY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession
    );

typedef  ANSC_STATUS
(*PFN_BEEPMHO_EXPIRE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession
    );

typedef  ANSC_STATUS
(*PFN_BEEPMHO_END)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession
    );

/*
 * Beep Master Host Objet is a derived instance of Bwsp Portal Host Object, though this derived
 * relationship is not very clear duo to the fact that (1) the BWSP_WSR interface separates the
 * internal Web Host implementation from the external modules; (2) the derived host objects only
 * provide session-tracking functionality.
 */
#define  BEEP_MASTER_HOST_CLASS_CONTENT                                                     \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    BEEP_MASTER_HOST_PROPERTY       Property;                                               \
    ANSC_HANDLE                     hBeepPemIf;                                             \
    ULONG                           ActiveClientCount;                                      \
    BOOL                            bActive;                                                \
                                                                                            \
    PFN_BEEPMHO_GET_IF              GetBeepPemIf;                                           \
    PFN_BEEPMHO_SET_IF              SetBeepPemIf;                                           \
    PFN_BEEPMHO_GET_NAME            GetHostName;                                            \
    PFN_BEEPMHO_SET_NAME            SetHostName;                                            \
    PFN_BEEPMHO_GET_PORT            GetHostPort;                                            \
    PFN_BEEPMHO_SET_PORT            SetHostPort;                                            \
                                                                                            \
    PFN_BEEPMHO_GET_PROPERTY        GetProperty;                                            \
    PFN_BEEPMHO_SET_PROPERTY        SetProperty;                                            \
    PFN_BEEPMHO_RESET               ResetProperty;                                          \
    PFN_BEEPMHO_RESET               Reset;                                                  \
                                                                                            \
    PFN_BEEPMHO_ENGAGE              Engage;                                                 \
    PFN_BEEPMHO_CANCEL              Cancel;                                                 \
                                                                                            \
    PFN_BEEPMHO_NEW                 NewContact;                                             \
    PFN_BEEPMHO_CLASSIFY            ClassifyClient;                                         \
    PFN_BEEPMHO_EXPIRE              ExpireSession;                                          \
    PFN_BEEPMHO_END                 EndSession;                                             \
    /* end of object class content */                                                       \

typedef  struct
_BEEP_MASTER_HOST_OBJECT
{
    BEEP_MASTER_HOST_CLASS_CONTENT
}
BEEP_MASTER_HOST_OBJECT,  *PBEEP_MASTER_HOST_OBJECT;

#define  ACCESS_BEEP_MASTER_HOST_OBJECT(p)          \
         ACCESS_CONTAINER(p, BEEP_MASTER_HOST_OBJECT, Linkage)


#endif
