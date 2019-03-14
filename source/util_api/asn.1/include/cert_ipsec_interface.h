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

    module:	cert_ipsec_interface.h

        For BroadWay Service Delivery System (Bisga)

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the CertIpsecManager Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        03/17/2004    initial revision.

**********************************************************************/


#ifndef  _CERT_IPSEC_MGR_INTERFACE_
#define  _CERT_IPSEC_MGR_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"

#ifndef  CERT_IPSEC_MANAGER_OID
#define  CERT_IPSEC_MANAGER_OID                    0x0038
#endif

#ifndef  CERT_IPSEC_MANAGER_NAME
#define  CERT_IPSEC_MANAGER_NAME                   "certIpsecManager"
#endif

typedef  struct
_CERT_IPSEC_MANAGER_PROPERTY
{
    ULONG                           uValue;
}
CERT_IPSEC_MANAGER_PROPERTY,  *PCERT_IPSEC_MANAGER_PROPERTY;

/***********************************************************
       CERT IPSEC MANAGER COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_CERTIPC_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_CERTIPC_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_CERTIPC_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_CERTIPC_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ANSC_STATUS
(*PFN_CERTIPC_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_CERTIPC_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_CERTIPC_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_CERTIPC_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_CERTIPC_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_CERTIPC_SETUP)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_CERTIPC_CLOSE)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * The CertIpsecManager object is responsible for initiating the SLAP environment for all the
 * policy management objects. It also provides an interface all SLAP objects to access persistent
 * configuration that is stored in the Sys_Registry. Any changes made by the user via the web
 * interface will be saved into the registry.
 */
#define  CERT_IPSEC_MANAGER_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    CERT_IPSEC_MANAGER_PROPERTY     Property;                                               \
    ANSC_HANDLE                     hSlapPoaIf;                                             \
    ANSC_HANDLE                     hSlapGoaIf;                                             \
    ANSC_HANDLE                     hSlapDefContainer;                                      \
    ANSC_HANDLE                     hCertIpsecIf;                                           \
    ANSC_HANDLE                     hPoamCertMgr;                                           \
    BOOL                            bActive;                                                \
                                                                                            \
    PFN_CERTIPC_GET_IF              GetCertIpsecIf;                                         \
                                                                                            \
    PFN_CERTIPC_GET_IF              GetSlapPoaIf;                                           \
    PFN_CERTIPC_SET_IF              SetSlapPoaIf;                                           \
    PFN_CERTIPC_GET_IF              GetSlapGoaIf;                                           \
    PFN_CERTIPC_SET_IF              SetSlapGoaIf;                                           \
    PFN_CERTIPC_GET_CONTEXT         GetSlapDefContainer;                                    \
    PFN_CERTIPC_SET_CONTEXT         SetSlapDefContainer;                                    \
                                                                                            \
    PFN_CERTIPC_GET_PROPERTY        GetProperty;                                            \
    PFN_CERTIPC_SET_PROPERTY        SetProperty;                                            \
    PFN_CERTIPC_RESET               ResetProperty;                                          \
    PFN_CERTIPC_RESET               Reset;                                                  \
                                                                                            \
    PFN_CERTIPC_ENGAGE              Engage;                                                 \
    PFN_CERTIPC_CANCEL              Cancel;                                                 \
    PFN_CERTIPC_SETUP               SetupEnv;                                               \
    PFN_CERTIPC_CLOSE               CloseEnv;                                               \
                                                                                            \
    /* end of object class content */                                                       \

typedef  struct
_CERT_IPSEC_MANAGER_OBJECT
{
    CERT_IPSEC_MANAGER_CLASS_CONTENT
}
CERT_IPSEC_MANAGER_OBJECT,  *PCERT_IPSEC_MANAGER_OBJECT;

#define  ACCESS_CERT_IPSEC_MANAGER_OBJECT(p)       \
         ACCESS_CONTAINER(p, CERT_IPSEC_MANAGER_OBJECT, Linkage)


#endif
