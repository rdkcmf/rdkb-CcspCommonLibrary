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

    module:	http_hco_interface.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Http Helper Container Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/14/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_HCO_INTERFACE_
#define  _HTTP_HCO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_oco_interface.h"
#include "ansc_oco_external_api.h"


/***********************************************************
   PLATFORM INDEPENDENT OBJECT CONTAINER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  HTTP_HELPER_CONTAINER_NAME                 "httpHelperContainer"
#define  HTTP_HELPER_CONTAINER_OID                  ANSC_OBJECT_OID_NULL
#define  HTTP_HELPER_CONTAINER_TYPE                 ANSC_OBJECT_TYPE_NULL

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_HTTPHCO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPHCO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ULONG
(*PFN_HTTPHCO_GET_CODE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  char*
(*PFN_HTTPHCO_GET_TEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       code
    );

typedef  ANSC_STATUS
(*PFN_HTTPHCO_GET_TIME)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTime
    );

/*
 * As a standard practice, we encapsulate all functional and feature objects inside an Object
 * Container Object so it will function as a black-box implementation when we have to integrate
 * an Ansc component with other systems. However, that's not the only reason why we need to
 * define the Container Object: we also use container as the building block for constructing
 * larger systems.
 */
#define  HTTP_HELPER_CONTAINER_CLASS_CONTENT                                                \
    /* duplication of the base object class content */                                      \
    ANSC_OBJECT_CONTAINER_CLASS_CONTENT                                                     \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hAtoStatus;                                             \
    ANSC_HANDLE                     hAtoHeader;                                             \
    ANSC_HANDLE                     hAtoMethod;                                             \
    ANSC_HANDLE                     hAtoCgiEnv;                                             \
    ANSC_HANDLE                     hAtoFxCat1;                                             \
    ANSC_HANDLE                     hAtoFxCat2;                                             \
    ANSC_HANDLE                     hSmartClock;                                            \
                                                                                            \
    PFN_HTTPHCO_GET_CONTEXT         GetAtoStatus;                                           \
    PFN_HTTPHCO_GET_CONTEXT         GetAtoHeader;                                           \
    PFN_HTTPHCO_GET_CONTEXT         GetAtoMethod;                                           \
    PFN_HTTPHCO_GET_CONTEXT         GetAtoCgiEnv;                                           \
    PFN_HTTPHCO_GET_CONTEXT         GetAtoFxCat1;                                           \
    PFN_HTTPHCO_GET_CONTEXT         GetAtoFxCat2;                                           \
    PFN_HTTPHCO_GET_CONTEXT         GetSmartClock;                                          \
                                                                                            \
    PFN_HTTPHCO_GET_CODE            GetHeaderId;                                            \
    PFN_HTTPHCO_GET_TEXT            GetHeaderName;                                          \
    PFN_HTTPHCO_GET_CODE            GetMethodId;                                            \
    PFN_HTTPHCO_GET_TEXT            GetMethodName;                                          \
    PFN_HTTPHCO_GET_CODE            GetCgiEnvId;                                            \
    PFN_HTTPHCO_GET_TEXT            GetReasonPhrase;                                        \
    PFN_HTTPHCO_GET_CODE            GetExtMediaType;                                        \
    PFN_HTTPHCO_GET_CODE            GetExtSubType;                                          \
    PFN_HTTPHCO_GET_TIME            GetClockTime;                                           \
    PFN_HTTPHCO_GET_TIME            GetUtcTime;                                             \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_HELPER_CONTAINER_OBJECT
{
    HTTP_HELPER_CONTAINER_CLASS_CONTENT
}
HTTP_HELPER_CONTAINER_OBJECT,  *PHTTP_HELPER_CONTAINER_OBJECT;

#define  ACCESS_HTTP_HELPER_CONTAINER_OBJECT(p)     \
         ACCESS_CONTAINER(p, HTTP_HELPER_CONTAINER_OBJECT, Linkage)


#endif
