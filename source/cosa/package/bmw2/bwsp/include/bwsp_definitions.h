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

    module:	bwsp_definitions.h

        For Broadway Web Service Plane Implementation (BWSP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This file defines the Bwsp element parameters that can be
        applied to the Bwsp Management Systems.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/02/03    initial revision.

**********************************************************************/


#ifndef  _BWSP_DEFINITIONS_
#define  _BWSP_DEFINITIONS_


/***********************************************************
    BWSP MANAGEMENT SCHEMA AND SITE STRUCTURE DEFINITION
***********************************************************/

/*
 * BMW2 prvoides an open and flexible framework that can be easily extended to support various web
 * management scenarios. One of the openness requirements is to allow the dynamic creation and
 * registration of Web Virtual Hosts. Since the Web Server module has inherent support for virtual
 * hosts, all we need to do here is to expose the interface.
 */
typedef  struct
_BWSP_WEB_HOST_DESCRIPTOR
{
    char*                           HostName;
    USHORT                          HostPort;
    BOOL                            bSessionTracking;
    BOOL                            bDefaultHost;
    ANSC_HANDLE                     hHostContext;

    void*                           NewContact;
    void*                           ClassifyClient;
    void*                           ExpireSession;
    void*                           EndSession;
}
BWSP_WEB_HOST_DESCRIPTOR,  *PBWSP_WEB_HOST_DESCRIPTOR;

/*
 * BMW2 prvoides an open and flexible framework that can be easily extended to support various web
 * management scenarios. One of the openness requirements is to allow the dynamic creation and
 * registration of Web Request Handlers (Resource Owners). Since the Web Server module has inherent
 * support for resource owners, all we need to do here is to expose the interface.
 */
typedef  struct
_BWSP_SERVLET_DESCRIPTOR
{
    char*                           WebRootPath;
    char*                           ServletPath;
    BOOL                            bMainPage;
    BOOL                            bUpnpSupport;
    ANSC_HANDLE                     hServletContext;

    void*                           DoOptions;
    void*                           DoGet;
    void*                           DoHead;
    void*                           DoPost;
    void*                           DoPut;
    void*                           DoDelete;
    void*                           DoTrace;
    void*                           DoConnect;

    void*                           DoNotify;
    void*                           DoSearch;
    void*                           DoMSearch;
    void*                           DoMPost;
    void*                           DoSubscribe;
    void*                           DoUnsubscribe;
}
BWSP_SERVLET_DESCRIPTOR,  *PBWSP_SERVLET_DESCRIPTOR;


#endif
