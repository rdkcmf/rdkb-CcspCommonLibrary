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

    module:	beep_eco_interface.h

        For BSP Execution Environment Plane Implementation (BEEP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Beep Env Controller Objects.

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


#ifndef  _BEEP_ECO_INTERFACE_
#define  _BEEP_ECO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"

#include "beep_properties.h"
#include "beep_ifo_csp.h"
#include "beep_ifo_pem.h"
#include "beep_ifo_ser.h"


/***********************************************************
        BEEP ENV CONTROLLER COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  BEEP_ECO_MAX_MHO_NUMBER                    16

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_BEEPECO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPECO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_BEEPECO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPECO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ANSC_STATUS
(*PFN_BEEPECO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_BEEPECO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_BEEPECO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPECO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPECO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPECO_SETUP_ENV)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPECO_CLOSE_ENV)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * The Beep Env Controller Object is responsible for creating the runtime environment, performing
 * system initialization, invoke a few key system components, and tearing down the system. While
 * it doesn't provide the real binary-level separation and runtime environment management, such as
 * beep object loading, the concept of "Environment Control" is really important.
 */
#define  BEEP_ENV_CONTROLLER_CLASS_CONTENT                                                  \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    BEEP_ENV_CONTROLLER_PROPERTY    Property;                                               \
    ANSC_HANDLE                     hWebHemIf;                                              \
    ANSC_HANDLE                     hBwspWsrIf;                                             \
    ANSC_HANDLE                     hBwrmRamIf;                                             \
    ANSC_HANDLE                     hSlapGoaIf;                                             \
    ANSC_HANDLE                     hSlapUoaIf;                                             \
    ANSC_HANDLE                     hBeepCspIf;                                             \
    ANSC_HANDLE                     hBeepPemIf;                                             \
    ANSC_HANDLE                     hBeepSerIf;                                             \
    BOOL                            bActive;                                                \
                                                                                            \
    ULONG                           BeepMhoCount;                                           \
    ANSC_HANDLE                     BeepMhoArray[BEEP_ECO_MAX_MHO_NUMBER];                  \
    ANSC_HANDLE                     BeepMsoArray[BEEP_ECO_MAX_MHO_NUMBER];                  \
                                                                                            \
    ANSC_HANDLE                     hSlapContainerApp;                                      \
    ANSC_HANDLE                     hSlapContainerServices;                                 \
    ANSC_HANDLE                     hSlapBeepApp;                                           \
    ANSC_HANDLE                     hSlapBeepServer;                                        \
    ANSC_HANDLE                     hSlapVarEntity;                                         \
    ANSC_HANDLE                     hSlapVarConverter;                                      \
                                                                                            \
    PFN_BEEPECO_GET_IF              GetWebHemIf;                                            \
    PFN_BEEPECO_SET_IF              SetWebHemIf;                                            \
    PFN_BEEPECO_GET_IF              GetBwspWsrIf;                                           \
    PFN_BEEPECO_SET_IF              SetBwspWsrIf;                                           \
    PFN_BEEPECO_GET_IF              GetBwrmRamIf;                                           \
    PFN_BEEPECO_SET_IF              SetBwrmRamIf;                                           \
    PFN_BEEPECO_GET_IF              GetSlapGoaIf;                                           \
    PFN_BEEPECO_SET_IF              SetSlapGoaIf;                                           \
    PFN_BEEPECO_GET_IF              GetSlapUoaIf;                                           \
    PFN_BEEPECO_SET_IF              SetSlapUoaIf;                                           \
    PFN_BEEPECO_GET_IF              GetBeepCspIf;                                           \
    PFN_BEEPECO_GET_IF              GetBeepPemIf;                                           \
    PFN_BEEPECO_GET_IF              GetBeepSerIf;                                           \
                                                                                            \
    PFN_BEEPECO_GET_PROPERTY        GetProperty;                                            \
    PFN_BEEPECO_SET_PROPERTY        SetProperty;                                            \
    PFN_BEEPECO_RESET               ResetProperty;                                          \
    PFN_BEEPECO_RESET               Reset;                                                  \
                                                                                            \
    PFN_BEEPECO_ENGAGE              Engage;                                                 \
    PFN_BEEPECO_CANCEL              Cancel;                                                 \
    PFN_BEEPECO_SETUP_ENV           SetupEnv;                                               \
    PFN_BEEPECO_CLOSE_ENV           CloseEnv;                                               \
                                                                                            \
    PFN_BEEPCSPIF_GET_SERVER        CspGetVirtualServer;                                    \
    PFN_BEEPCSPIF_ADD_SERVER        CspAddVirtualServer;                                    \
    PFN_BEEPCSPIF_ADD_SERVER2       CspAddVirtualServer2;                                   \
    PFN_BEEPCSPIF_DEL_SERVER        CspDelVirtualServer;                                    \
    PFN_BEEPCSPIF_DEL_ALL           CspDelAllVirtualServers;                                \
                                                                                            \
    PFN_BEEPCSPIF_GET_NAME          CspGetHostName;                                         \
    PFN_BEEPCSPIF_SET_NAME          CspSetHostName;                                         \
    PFN_BEEPCSPIF_GET_NAME          CspGetWebRootPath;                                      \
    PFN_BEEPCSPIF_SET_NAME          CspSetWebRootPath;                                      \
    PFN_BEEPCSPIF_GET_NAME          CspGetServletPath;                                      \
    PFN_BEEPCSPIF_SET_NAME          CspSetServletPath;                                      \
    PFN_BEEPCSPIF_GET_NAME          CspGetDefPagePath;                                      \
    PFN_BEEPCSPIF_SET_NAME          CspSetDefPagePath;                                      \
    PFN_BEEPCSPIF_ADD_BINDING       CspAddPageBinding;                                      \
                                                                                            \
    PFN_BEEPPEMIF_GET_IF            PemGetBeepCspIf;                                        \
    PFN_BEEPPEMIF_GET_IF2           PemGetBeepRmgIf;                                        \
    PFN_BEEPPEMIF_GET_IF            PemGetWebHemIf;                                         \
    PFN_BEEPPEMIF_GET_IF            PemGetBwspWsrIf;                                        \
    PFN_BEEPPEMIF_GET_IF            PemGetBwrmRamIf;                                        \
    PFN_BEEPPEMIF_GET_IF            PemGetSlapGoaIf;                                        \
    PFN_BEEPPEMIF_GET_IF            PemGetSlapUoaIf;                                        \
    PFN_BEEPPEMIF_GET_IF3           PemGetBeepPesIf;                                        \
                                                                                            \
    PFN_BEEPPEMIF_GET_CONTEXT       PemGetSlapContainerApp;                                 \
    PFN_BEEPPEMIF_GET_CONTEXT       PemGetSlapContainerServices;                            \
    PFN_BEEPPEMIF_SET_CONTEXT       PemSetSlapContainerServices;                            \
    PFN_BEEPPEMIF_GET_CONTEXT       PemGetSlapBeepApp;                                      \
    PFN_BEEPPEMIF_GET_CONTEXT       PemGetSlapBeepServer;                                   \
    PFN_BEEPPEMIF_GET_CONTEXT2      PemGetBeepServiceBean;                                  \
                                                                                            \
    PFN_BEEPSERIF_ATTACH            SerAttachEngine;                                        \
    PFN_BEEPSERIF_DETACH            SerDetachEngine;                                        \
    /* end of object class content */                                                       \

typedef  struct
_BEEP_ENV_CONTROLLER_OBJECT
{
    BEEP_ENV_CONTROLLER_CLASS_CONTENT
}
BEEP_ENV_CONTROLLER_OBJECT,  *PBEEP_ENV_CONTROLLER_OBJECT;

#define  ACCESS_BEEP_ENV_CONTROLLER_OBJECT(p)       \
         ACCESS_CONTAINER(p, BEEP_ENV_CONTROLLER_OBJECT, Linkage)


#endif
