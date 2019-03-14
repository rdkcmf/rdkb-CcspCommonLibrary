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

    module:	cf_sso_interface.h

        For Content Filtering (CF),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Content Filtering 
        Simple Object.

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


#ifndef  _CONT_FILTER_SSO_INTERFACE_
#define  _CONT_FILTER_SSO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"

#include "cont_filter_ifo_csp.h"

#define  CONT_FILTER_SIMPLE_OBJECT_NAME             "contFilterSimpleObject"

#define  CONT_FILTER_SIMPLE_OBJECT_OID              0

/***********************************************************
    PLATFORM INDEPENDENT SIMPLE SERVER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  CF_MAX_ACCOUNT_ID                          16
#define  CF_MAX_ACCOUNT_PASSWORD                    64
#define  CF_MAX_USER_NAME                           32
#define  CF_MAX_USER_PASSWORD                       64
#define  CF_MAX_DEVICE_NAME                         64
#define  CF_MAX_CLIENT_VERSION                      16
#define  CF_MAX_DEVICE_ID                           128
#define  CF_MAX_DEVICE_TYPE                         8

#define  CF_CLIENT_EDITION                          "HttpProxyCF"

#define  CF_DEVICE_TYPE_PC                          1
#define  CF_DEVICE_TYPE_SG                          2

#define  CF_DEVICE_NAME_PC                          "PC"
#define  CF_DEVICE_NAME_SG                          "SG"

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_CFSSO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_CFSSO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_CFSSO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_CFSSO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ANSC_STATUS
(*PFN_CFSSO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_CFSSO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_CFSSO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_CFSSO_RESET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * As a standard practice, we encapsulate all functional and feature objects inside an Object
 * Container Object so it will function as a black-box implementation when we have to integrate
 * an Ansc component with other systems. However, that's not the only reason why we need to
 * define the Container Object: we also use container as the building block for constructing
 * larger systems.
 */
#define  CONT_FILTER_SIMPLE_OBJECT_CLASS_CONTENT                                            \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    UCHAR                           AccountId[CF_MAX_ACCOUNT_ID];                           \
    UCHAR                           AccountPswd[CF_MAX_ACCOUNT_PASSWORD];                   \
    UCHAR                           UserName[CF_MAX_USER_NAME];                             \
    UCHAR                           UserPswd[CF_MAX_USER_PASSWORD];                         \
    UCHAR                           DeviceName[CF_MAX_DEVICE_NAME];                         \
    UCHAR                           ClientVersion[CF_MAX_CLIENT_VERSION];                   \
    UCHAR                           DeviceId[CF_MAX_DEVICE_ID];                             \
    UCHAR                           DeviceType[CF_MAX_DEVICE_TYPE];                                           \
                                                                                            \
    ANSC_HANDLE                     hCfCspIf;                                               \
    ANSC_HANDLE                     hHttpWamIf;                                             \
    ANSC_HANDLE                     hHttpSbcIf;                                             \
    ANSC_HANDLE                     hHttpCbcIf;                                             \
    ANSC_HANDLE                     hHttpPbcIf;                                             \
    ANSC_HANDLE                     hHttpHfpIf;                                             \
                                                                                            \
    ANSC_HANDLE                     hCfSmo;                                                 \
    ANSC_HANDLE                     hSlmo;                                                  \
    ANSC_HANDLE                     hAmmo;                                                  \
    ANSC_HANDLE                     hFmmo;                                                  \
    ANSC_HANDLE                     hRmmo;                                                  \
                                                                                            \
    ANSC_HANDLE                     hAdvHttpProxy;                                          \
    ANSC_HANDLE                     hHttpHelperContainer;                                   \
                                                                                            \
    BOOL                            bActive;                                                \
                                                                                            \
    PFN_CFSSO_RESET                 Reset;                                                  \
    PFN_CFSSO_RESET_PROPERTY        ResetProperty;                                          \
                                                                                            \
    PFN_CFSSO_GET_IF                GetCfCspIf;                                             \
    PFN_CFSSO_SET_IF                SetHttpHfpIf;                                           \
                                                                                            \
    PFN_CFSSO_ENGAGE                Engage;                                                 \
    PFN_CFSSO_CANCEL                Cancel;                                                 \
                                                                                            \
    PFN_ANSCCSPIF_GET_MODE          CspGetCfgMode;                                          \
    PFN_ANSCCSPIF_IS_SOMETHING      CspIsReadOnly;                                          \
    PFN_ANSCCSPIF_IS_SOMETHING      CspIsSwitchOn;                                          \
    PFN_ANSCCSPIF_ENGAGE            CspEngage;                                              \
    PFN_ANSCCSPIF_CANCEL            CspCancel;                                              \
                                                                                            \
    /* end of object class content */                                                       \

typedef  struct
_CONT_FILTER_SIMPLE_OBJECT
{
    CONT_FILTER_SIMPLE_OBJECT_CLASS_CONTENT
}
CONT_FILTER_SIMPLE_OBJECT,  *PCONT_FILTER_SIMPLE_OBJECT;

#define  ACCESS_CONT_FILTER_SIMPLE_OBJECT(p)       \
         ACCESS_CONTAINER(p, CONT_FILTER_SIMPLE_OBJECT, Linkage)


#endif
