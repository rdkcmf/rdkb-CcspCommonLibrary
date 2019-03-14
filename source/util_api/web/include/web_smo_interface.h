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

    module:	web_smo_interface.h

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Web Site Manager Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/09/02    initial revision.

**********************************************************************/


#ifndef  _WEB_SMO_INTERFACE_
#define  _WEB_SMO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_cover3_interface.h"
#include "ansc_cover3_external_api.h"
#include "web_properties.h"

#include "http_ifo_wsp.h"
#include "web_ifo_hem.h"
#include "web_ifo_rpm.h"


/***********************************************************
   PLATFORM INDEPENDENT WEB SITE MANAGER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  WEB_SMO_MAJOR_VERSION                      1
#define  WEB_SMO_MINOR_VERSION                      1
#define  WEB_SMO_PRODUCT_NAME                       "Web Site Manager v2.0 - Cisco Systems, Inc."

#define  WEB_SMO_MPO_TABLE_SIZE                     16

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_WEBSMO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBSMO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_WEBSMO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBSMO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ULONG
(*PFN_WEBSMO_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBSMO_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    );

typedef  BOOL
(*PFN_WEBSMO_GET_BOOL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBSMO_SET_BOOL)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnabled
    );

typedef  char*
(*PFN_WEBSMO_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBSMO_SET_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ANSC_STATUS
(*PFN_WEBSMO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_WEBSMO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_WEBSMO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBSMO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBSMO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_WEBSMO_GET_VHO_BYNAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ANSC_HANDLE
(*PFN_WEBSMO_GET_VHO_BYPORT)
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      port,
        BOOL                        bDefault
    );

typedef  ANSC_HANDLE
(*PFN_WEBSMO_POP_VHO_BYNAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ANSC_HANDLE
(*PFN_WEBSMO_ADD_VHO_BYPROP)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVhoProperty
    );

typedef  ANSC_STATUS
(*PFN_WEBSMO_ADD_VHO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVho
    );

typedef  ANSC_STATUS
(*PFN_WEBSMO_DEL_VHO_BYNAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ANSC_STATUS
(*PFN_WEBSMO_DEL_VHO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVho
    );

typedef  ANSC_STATUS
(*PFN_WEBSMO_DEL_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBSMO_ASYNC_SERVE)
    (
        ANSC_HANDLE                 hAsyncReq
    );

typedef  ANSC_STATUS
(*PFN_WEBSMO_CONSTRUCT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ULONG                       ulStatusCode
    );

typedef  ANSC_STATUS
(*PFN_WEBSMO_ADD_HEADERS)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoRep,
        BOOL                        bCloseConnection
    );

typedef  BOOL
(*PFN_WEBSMO_WSP_ACCEPT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hClientInfo
    );

typedef  ANSC_STATUS
(*PFN_WEBSMO_WSP_NOTIFY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage,
        ULONG                       ulEvent
    );

typedef  ANSC_STATUS
(*PFN_WEBSMO_WSP_SERVE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWebsSession,
        ANSC_HANDLE                 hWebsTrans,
        ANSC_HANDLE                 hMessage
    );

/*
 * The HyperText Transfer Protocol (HTTP) is an application-level protocol for distributed, col-
 * laborative, hypermedia information systems. It is a generic, stateless, protocol which can be
 * used for many tasks beyond its use for hypertext, through extension of its request methods,
 * error codes and headers. A feature of HTTP is the typing and negotiation of data representation,
 * allowing systems to be built independently of the data being transferred.
 */
#define  WEB_SITE_MANAGER_CLASS_CONTENT                                                     \
    /* duplication of the base object class content */                                      \
    ANSCCO_VER3_CLASS_CONTENT                                                               \
    /* start of object class content */                                                     \
    WEB_SITE_MANAGER_PROPERTY       Property;                                               \
    ANSC_HANDLE                     hWspIf;                                                 \
    ANSC_HANDLE                     hHemIf;                                                 \
    ANSC_HANDLE                     hRpmIf;                                                 \
    ANSC_HANDLE                     hHfpIf;                                                 \
    BOOL                            bActive;                                                \
                                                                                            \
    QUEUE_HEADER                    VhoQueue;                                               \
    ANSC_LOCK                       VhoQueueLock;                                           \
    QUEUE_HEADER                    MpoTable[WEB_SMO_MPO_TABLE_SIZE];                       \
    ANSC_LOCK                       MpoTableLock;                                           \
                                                                                            \
    PFN_WEBSMO_GET_IF               GetWspIf;                                               \
    PFN_WEBSMO_GET_IF               GetHemIf;                                               \
    PFN_WEBSMO_GET_IF               GetRpmIf;                                               \
    PFN_WEBSMO_GET_IF               GetHfpIf;                                               \
    PFN_WEBSMO_SET_IF               SetHfpIf;                                               \
    PFN_WEBSMO_GET_BOOL             GetPersistentConn;                                      \
    PFN_WEBSMO_SET_BOOL             SetPersistentConn;                                      \
    PFN_WEBSMO_GET_NAME             GetProductName;                                         \
    PFN_WEBSMO_SET_NAME             SetProductName;                                         \
    PFN_WEBSMO_GET_PROPERTY         GetProperty;                                            \
    PFN_WEBSMO_SET_PROPERTY         SetProperty;                                            \
    PFN_WEBSMO_RESET                ResetProperty;                                          \
    PFN_WEBSMO_RESET                Reset;                                                  \
                                                                                            \
    PFN_WEBSMO_ENGAGE               Engage;                                                 \
    PFN_WEBSMO_CANCEL               Cancel;                                                 \
    PFN_WEBSMO_ENGAGE               EngageAllHosts;                                         \
    PFN_WEBSMO_CANCEL               CancelAllHosts;                                         \
                                                                                            \
    PFN_WEBSMO_GET_VHO_BYNAME       GetHostByName;                                          \
    PFN_WEBSMO_GET_VHO_BYPORT       GetHostByPort;                                          \
    PFN_WEBSMO_POP_VHO_BYNAME       PopHostByName;                                          \
    PFN_WEBSMO_ADD_VHO_BYPROP       AddHostByProperty;                                      \
    PFN_WEBSMO_ADD_VHO              AddHost;                                                \
    PFN_WEBSMO_DEL_VHO_BYNAME       DelHostByName;                                          \
    PFN_WEBSMO_DEL_VHO              DelHost;                                                \
    PFN_WEBSMO_DEL_ALL              DelAllHosts;                                            \
                                                                                            \
    PFN_WEBSMO_ASYNC_SERVE          AsyncServeTask;                                         \
    PFN_WEBSMO_CONSTRUCT            ConstructResponse;                                      \
    PFN_WEBSMO_ADD_HEADERS          AddHttpRepHeaders;                                      \
                                                                                            \
    PFN_WEBSMO_WSP_ACCEPT           WspAccept;                                              \
    PFN_WEBSMO_WSP_NOTIFY           WspNotify;                                              \
    PFN_WEBSMO_WSP_SERVE            WspServe;                                               \
                                                                                            \
    PFN_WEBHEMIF_ATTACH1            HemAttachWebVho;                                        \
    PFN_WEBHEMIF_DETACH1            HemDetachWebVho;                                        \
    PFN_WEBHEMIF_DELETE1            HemDeleteWebVho;                                        \
    PFN_WEBHEMIF_ATTACH2            HemAttachWebRoo;                                        \
    PFN_WEBHEMIF_DETACH2            HemDetachWebRoo;                                        \
    PFN_WEBHEMIF_DELETE2            HemDeleteWebRoo;                                        \
                                                                                            \
    PFN_WEBHEMIF_GET_VERSION        HemGetHttpVersion;                                      \
    PFN_WEBHEMIF_GET_OBJ            HemGetHttpHco;                                          \
    PFN_WEBHEMIF_GET_IF             HemGetHttpHfpIf;                                        \
    PFN_WEBHEMIF_REGISTER_IF        HemRegisterHfpIf;                                       \
                                                                                            \
    PFN_WEBRPMIF_GET_MAPPING        RpmGetMappedPage;                                       \
    PFN_WEBRPMIF_ADD_MAPPING        RpmAddMappedPath;                                       \
    PFN_WEBRPMIF_DEL_MAPPING        RpmDelMappedPath;                                       \
    PFN_WEBRPMIF_DEL_ALL            RpmDelAllMappedPaths;                                   \
    /* end of object class content */                                                       \

typedef  struct
_WEB_SITE_MANAGER_OBJECT
{
    WEB_SITE_MANAGER_CLASS_CONTENT
}
WEB_SITE_MANAGER_OBJECT,  *PWEB_SITE_MANAGER_OBJECT;

#define  ACCESS_WEB_SITE_MANAGER_OBJECT(p)          \
         ACCESS_CONTAINER(p, WEB_SITE_MANAGER_OBJECT, Linkage)


/***********************************************************
         WEB RESOURCE PATH MAPPING ENTRY DEFINITION
***********************************************************/

/*
 * The WEB_RPM interface allows a web resource to be mapped to one or more different URL paths.
 * Before locating the target ResourceOwner object, the VirtualHost will map the request URL con-
 * tained in the HTTP request message to its original web resource location. For example, the UPnP
 * module utilizes the same BSP for all service controls, and each service has its own contrl URL.
 */
typedef  struct
_WEB_SMO_MAPPED_PATH
{
    SINGLE_LINK_ENTRY               Linkage;
    char*                           ReqUrlPath;
    char*                           TargetPage;
}
WEB_SMO_MAPPED_PATH,  *PWEB_SMO_MAPPED_PATH;

#define  ACCESS_WEB_SMO_MAPPED_PATH(p)              \
         ACCESS_CONTAINER(p, WEB_SMO_MAPPED_PATH, Linkage)

#define  WebSmoCleanMappedPath(mapped_path)                                                 \
         {                                                                                  \
            if ( mapped_path->ReqUrlPath )                                                  \
            {                                                                               \
                AnscFreeMemory(mapped_path->ReqUrlPath);                                    \
                                                                                            \
                mapped_path->ReqUrlPath = NULL;                                             \
            }                                                                               \
                                                                                            \
            if ( mapped_path->TargetPage )                                                  \
            {                                                                               \
                AnscFreeMemory(mapped_path->TargetPage);                                    \
                                                                                            \
                mapped_path->TargetPage = NULL;                                             \
            }                                                                               \
         }

#define  WebSmoFreeMappedPath(mapped_path)                                                  \
         {                                                                                  \
            WebSmoCleanMappedPath(mapped_path);                                             \
            AnscFreeMemory       (mapped_path);                                             \
         }


/***********************************************************
    PLATFORM INDEPENDENT WEB SMO ASYNC REQUEST DEFINITION
***********************************************************/

/*
 * In order to pipeline multiple concurrent requests sent by one or more HTTP clients, we support
 * asynchronous processing for HTTP requests. If the asynchronous mode is enabled, we spawn a
 * separate task for every request. While this approach improves the responsiveness of the web
 * server and provides better isolation for request handling, it requires more system resources.
 */
typedef  struct
_WEB_SMO_ASYNC_REQUEST
{
    ANSC_HANDLE                     hThisObject;
    ANSC_HANDLE                     hWebsSession;
    ANSC_HANDLE                     hWebsTrans;
    ANSC_HANDLE                     hReqMessage;
}
WEB_SMO_ASYNC_REQUEST,  *PWEB_SMO_ASYNC_REQUEST;


#endif
