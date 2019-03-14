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

    module:	web_upo_internal_api.h

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Web Uri Path
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/12/02    initial revision.

**********************************************************************/


#ifndef  _WEB_UPO_INTERNAL_API_
#define  _WEB_UPO_INTERNAL_API_


/***********************************************************
           FUNCTIONS IMPLEMENTED IN WEB_UPO_STATES.C
***********************************************************/

BOOL
WebUpoIsRemoveable
    (
        ANSC_HANDLE                 hThisObject
    );

char*
WebUpoGetPathName
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebUpoSetPathName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    );

ANSC_HANDLE
WebUpoGetResourceOwner
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebUpoSetResourceOwner
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hResourceOwner
    );

ANSC_STATUS
WebUpoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN WEB_UPO_MANAGEMENT.C
***********************************************************/

ANSC_HANDLE
WebUpoGetUriPath
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    );

ANSC_HANDLE
WebUpoAddUriPath
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    );

ANSC_STATUS
WebUpoDelUriPath
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    );

ANSC_STATUS
WebUpoDelAllUriPaths
    (
        ANSC_HANDLE                 hThisObject
    );


#endif
