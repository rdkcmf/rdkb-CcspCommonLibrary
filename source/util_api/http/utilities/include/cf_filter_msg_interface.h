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


#ifndef  _CF_FILTER_MSG_CO_INTERFACE_DEC
#define  _CF_FILTER_MSG_CO_INTERFACE_DEC

#include "cf_bmco_co_interface.h"

/* =============================================================================
 *          Filtering Request Message Object
 * ========================================================================== */

#define  CF_FR_MSG_COMPONENT_NAME                   "CfFRMsgComponent"
#define  CF_FR_MSG_COMPONENT_OID                    ANSC_OBJECT_OID_NULL
#define  CF_FR_MSG_COMPONENT_TYPE                   ANSC_OBJECT_TYPE_NULL

typedef
ANSC_HANDLE
(*PFN_CF_FR_GET_DEVICE_TYPE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_CF_FR_GET_PASSWORD)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_CF_FR_GET_CLIENT_VERSION)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_CF_FR_GET_URL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ULONG
(*PFN_CF_FR_GET_INDEX)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_CF_FR_SET_DEVICE_TYPE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 DeviceType
    );

typedef
void
(*PFN_CF_FR_SET_PASSWORD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 Password
    );

typedef
void
(*PFN_CF_FR_SET_CLIENT_VERSION)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 ClientVersion
    );

typedef
void
(*PFN_CF_FR_SET_URL)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 URL
    );


typedef
void
(*PFN_CF_FR_SET_INDEX)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       Index
    );

/*
 * As a standard practice, we encapsulate all functional and feature objects inside an Object
 * Container Object so it will function as a black-box implementation when we have to integrate
 * an Ansc component with other systems. However, that's not the only reason why we need to
 * define the Container Object: we also use container as the building block for constructing
 * larger systems.
 */
#define  CF_FR_MESSAGE_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                  \
    CF_BASIC_MESSAGE_CONTENT_CLASS_CONTENT                                              \
    /* start of object class content */                                                 \
    ANSC_HANDLE                     DeviceType;                                         \
    ANSC_HANDLE                     Password;                                           \
    ANSC_HANDLE                     ClientVersion;                                      \
    ANSC_HANDLE                     Url;                                                \
    ULONG                           Index;                                              \
    /* methods */                                                                       \
    PFN_CF_FR_GET_DEVICE_TYPE       GetDeviceType;                                      \
    PFN_CF_FR_GET_PASSWORD          GetPassword;                                        \
    PFN_CF_FR_GET_CLIENT_VERSION    GetClientVersion;                                   \
    PFN_CF_FR_GET_URL               GetUrl;                                             \
    PFN_CF_FR_GET_INDEX             GetIndex;                                           \
    PFN_CF_FR_SET_DEVICE_TYPE       SetDeviceType;                                      \
    PFN_CF_FR_SET_PASSWORD          SetPassword;                                        \
    PFN_CF_FR_SET_CLIENT_VERSION    SetClientVersion;                                   \
    PFN_CF_FR_SET_URL               SetUrl;                                             \
    PFN_CF_FR_SET_INDEX             SetIndex;                                           \



/* filtering response message object  */
typedef  struct
_CF_FR_MESSAGE_OBJECT
{
    CF_FR_MESSAGE_CLASS_CONTENT
}
CF_FR_MESSAGE_OBJECT,    *PCF_FR_MESSAGE_OBJECT;


#define  ACCESS_CF_FR_MESSAGE_OBJECT(p)            \
         ACCESS_CONTAINER(p, CF_FR_MESSAGE_OBJECT, Linkage)



/* =============================================================================
 *          Filtering Response Message Object
 * ========================================================================== */

#define  CF_FP_MSG_COMPONENT_NAME                   "CfFPMsgComponent"
#define  CF_FP_MSG_COMPONENT_OID                    ANSC_OBJECT_OID_NULL
#define  CF_FP_MSG_COMPONENT_TYPE                   ANSC_OBJECT_TYPE_NULL


typedef
ULONG
(*PFN_CF_FP_GET_INDEX)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
BOOL
(*PFN_CF_FP_IS_BLOCKED)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_CF_FP_GET_CATEGORY)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_CF_FP_SET_INDEX)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       Index
    );

typedef
void
(*PFN_CF_FP_SET_BLOCKED)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bBlocked
    );

typedef
void
(*PFN_CF_FP_SET_CATEGORY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 Category
    );


/*
 * As a standard practice, we encapsulate all functional and feature objects inside an Object
 * Container Object so it will function as a black-box implementation when we have to integrate
 * an Ansc component with other systems. However, that's not the only reason why we need to
 * define the Container Object: we also use container as the building block for constructing
 * larger systems.
 */
#define  CF_FP_MESSAGE_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                  \
    CF_BASIC_MESSAGE_CONTENT_CLASS_CONTENT                                              \
    /* start of object class content */                                                 \
    ULONG                           Index;                                              \
    BOOL                            bBlocked;                                           \
    ANSC_HANDLE                     Category;                                           \
    /* methods */                                                                       \
    PFN_CF_FP_GET_INDEX             GetIndex;                                           \
    PFN_CF_FP_IS_BLOCKED            IsBlocked;                                          \
    PFN_CF_FP_GET_CATEGORY          GetCategory;                                        \
    PFN_CF_FP_SET_INDEX             SetIndex;                                           \
    PFN_CF_FP_SET_BLOCKED           SetBlocked;                                         \
    PFN_CF_FP_SET_CATEGORY          SetCategory;                                        \



/* filtering response message object  */
typedef  struct
_CF_FP_MESSAGE_OBJECT
{
    CF_FP_MESSAGE_CLASS_CONTENT
}
CF_FP_MESSAGE_OBJECT,    *PCF_FP_MESSAGE_OBJECT;


#define  ACCESS_CF_FP_MESSAGE_OBJECT(p)            \
         ACCESS_CONTAINER(p, CF_FP_MESSAGE_OBJECT, Linkage)


#endif
