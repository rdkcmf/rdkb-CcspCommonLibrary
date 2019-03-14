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

    module:	cf_slmo_interface.h

        For Server List Management Object (SLMO),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file includes all the header files required by
        Server Detection Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan   

    ---------------------------------------------------------------

    revision:

        04/23/02    initial revision.

    ---------------------------------------------------------------

    last modified:  
    
**********************************************************************/

#ifndef  _CF_SLMO_INTERFACE_DEC
#define  _CF_SLMO_INTERFACE_DEC

#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"

#include "http_ifo_bsp.h"

/*
 * Define some const values that will be used in the object mapper object definition.
 */

#define  CF_SLMO_COMPONENT_NAME                     "CfSlmoComponent"
#define  CF_SLMO_COMPONENT_OID                      ANSC_OBJECT_OID_NULL
#define  CF_SLMO_COMPONENT_TYPE                     ANSC_OBJECT_TYPE_NULL

#define  CF_SLMO_MAX_SERVER_LIST_URL                16

#define  CF_SLMO_HTTP_CLIENT_TIMEOUT_MS             (ULONG)-1

#define  CF_SLMO_SERVER_AS                          1
#define  CF_SLMO_SERVER_FS                          2
#define  CF_SLMO_SERVER_RS                          3

typedef  struct
_CF_SLMO_SERVER_INFO
{
    ANSC_IPV4_ADDRESS               IP_Addr;
    ULONG                           Port;
}
CF_SLMO_SERVER_INFO, *PCF_SLMO_SERVER_INFO;


typedef
void
(*PCF_SLMO_ADD_SERVER_LIST_URL)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pUrl
    );

typedef
void
(*PCF_SLMO_START)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PCF_SLMO_END)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
BOOL
(*PCF_SLMO_GOT_SERVER_LIST)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PCF_SLMO_CREATE_HTTP_CLIENT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PCF_SLMO_DELETE_HTTP_CLIENT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PCF_SLMO_SET_HFPIF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHfpIf
    );

typedef
void
(*PCF_SLMO_SETUP_BSPIF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
PCF_SLMO_SERVER_INFO
(*PCF_SLMO_GET_SERVER)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ServerType,
        ULONG                       Index
    );

typedef
ULONG
(*PCF_SLMO_GET_SERVER_COUNT)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ServerType
    );

typedef
void
(*PCF_SLMO_RANDOM_SERVER_ORDER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hServerList
    );

/*
 * As a standard practice, we encapsulate all functional and feature objects inside an Object
 * Container Object so it will function as a black-box implementation when we have to integrate
 * an Ansc component with other systems. However, that's not the only reason why we need to
 * define the Container Object: we also use container as the building block for constructing
 * larger systems.
 */
#define  CF_SERVER_LIST_MANAGEMENT_CLASS_CONTENT                                        \
    /* duplication of the base object class content */                                  \
    ANSCCO_CLASS_CONTENT                                                                \
    ANSC_HANDLE                     ASList;                                             \
    ANSC_HANDLE                     FSList;                                             \
    ANSC_HANDLE                     RSList;                                             \
    ANSC_EVENT                      GotServerListEvent;                                 \
    BOOL                            bTaskExited;                                        \
    BOOL                            bGotServerList;                                     \
    BOOL                            bStarted;                                           \
    PUCHAR                          ServerListUrl[CF_SLMO_MAX_SERVER_LIST_URL];         \
    ULONG                           NumServerListUrl;                                   \
    ANSC_HANDLE                     hHttpHelperContainer;                               \
    ANSC_HANDLE                     hHttpClient;                                        \
    ANSC_HANDLE                     hHfpIf;                                             \
    HTTP_BSP_INTERFACE              BspIf;                                              \
    PCF_SLMO_ADD_SERVER_LIST_URL    AddUrl;                                             \
    PCF_SLMO_START                  Start;                                              \
    PCF_SLMO_END                    End;                                                \
    PCF_SLMO_GOT_SERVER_LIST        GotServerList;                                      \
    PCF_SLMO_CREATE_HTTP_CLIENT     CreateHttpClient;                                   \
    PCF_SLMO_DELETE_HTTP_CLIENT     DeleteHttpClient;                                   \
    PCF_SLMO_SET_HFPIF              SetHfpIf;                                           \
    PCF_SLMO_SETUP_BSPIF            SetupBspIf;                                         \
    PCF_SLMO_GET_SERVER             GetServer;                                          \
    PCF_SLMO_GET_SERVER_COUNT       GetServerCount;                                     \
    PCF_SLMO_RANDOM_SERVER_ORDER    RandomServerOrder;                                  \


/* Filtering Message Management Object (FMMO) */
typedef  struct
_CF_SLMO_OBJECT
{
    CF_SERVER_LIST_MANAGEMENT_CLASS_CONTENT
}
CF_SLMO_OBJECT,    *PCF_SLMO_OBJECT;


#define  ACCESS_CF_SLMO_OBJECT(p)            \
         ACCESS_CONTAINER(p, CF_SLMO_OBJECT, Linkage)

#endif

