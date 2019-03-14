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

    module:	cont_filter_ifo_csp.h

        For Content Filtering (CF),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Configuration Service Provider Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        09/10/02    initial revision.

**********************************************************************/


#ifndef  _CONT_FILTER_IFO_CSP_
#define  _CONT_FILTER_IFO_CSP_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_csp.h"


/***********************************************************
  DHCPS CONFIGURATION SERVICE PROVIDER INTERFACE DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  CONT_FILTER_CSP_INTERFACE_NAME             "contFilterConfigurationServiceProviderIf"
#define  CONT_FILTER_CSP_INTERFACE_ID               0

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_CFCSPIF_SET_ACCOUNT_ID)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pAccountId
    );

typedef  ANSC_STATUS
(*PFN_CFCSPIF_SET_ACCOUNT_PSWD)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pAccountPswd
    );

typedef  ANSC_STATUS
(*PFN_CFCSPIF_SET_USER_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pUserName
    );

typedef  ANSC_STATUS
(*PFN_CFCSPIF_SET_USER_PSWD)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pUserPswd
    );

typedef  ANSC_STATUS
(*PFN_CFCSPIF_SET_DEVICE_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pDeviceName
    );

typedef  ANSC_STATUS
(*PFN_CFCSPIF_SET_CLIENT_VERSION)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pVersion
    );

typedef  ANSC_STATUS
(*PFN_CFCSPIF_SET_DEVICE_ID)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pDeviceId
    );

typedef  ANSC_STATUS
(*PFN_CFCSPIF_SET_DEVICE_TYPE)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pDeviceType
    );

/*
 * As a general requirement, each module SHOULD provide an interface for external components to
 * configure its parameters and policies. Although the benefit of unifying the definition and usage
 * of such an interface is obvious, we DON'T want to impose any set of rules on the implementation.
 * Instead, we expect every module will implement its configuration interfaces independently.
 */
#define  CONT_FILTER_CSP_INTERFACE_CLASS_CONTENT                                            \
    /* duplication of the base object class content */                                      \
    ANSC_CSP_INTERFACE_CLASS_CONTENT                                                        \
    /* start of object class content */                                                     \
    PFN_CFCSPIF_SET_ACCOUNT_ID      SetAccountId;                                           \
    PFN_CFCSPIF_SET_ACCOUNT_PSWD    SetAccountPassword;                                     \
    PFN_CFCSPIF_SET_USER_NAME       SetUserName;                                            \
    PFN_CFCSPIF_SET_USER_PSWD       SetUserPassword;                                        \
    PFN_CFCSPIF_SET_DEVICE_NAME     SetDeviceName;                                          \
    PFN_CFCSPIF_SET_DEVICE_ID       SetDeviceId;                                            \
    PFN_CFCSPIF_SET_DEVICE_TYPE     SetDeviceType;                                          \
    PFN_CFCSPIF_SET_CLIENT_VERSION  SetClientVersion;                                       \
    /* end of object class content */                                                       \

typedef  struct
_CONT_FILTER_CSP_INTERFACE
{
    CONT_FILTER_CSP_INTERFACE_CLASS_CONTENT
}
CONT_FILTER_CSP_INTERFACE,  *PCONT_FILTER_CSP_INTERFACE;

#define  ACCESS_CONT_FILTER_CSP_INTERFACE(p)              \
         ACCESS_CONTAINER(p, CONT_FILTER_CSP_INTERFACE, Linkage)


#endif
