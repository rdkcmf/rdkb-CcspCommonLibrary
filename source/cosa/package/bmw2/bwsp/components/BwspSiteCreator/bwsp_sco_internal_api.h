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

    module:	bwsp_sco_internal_api.h

        For Broadway Web Service Plane Implementation (BWSP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Bwsp Site Creator
        Object.

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


#ifndef  _BWSP_SCO_INTERNAL_API_
#define  _BWSP_SCO_INTERNAL_API_


/***********************************************************
           FUNCTIONS IMPLEMENTED IN BWSP_SCO_STATES.C
***********************************************************/

ANSC_HANDLE
BwspScoGetWebHemIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BwspScoSetWebHemIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
BwspScoGetBwspWsrIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
BwspScoGetBwspCspIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BwspScoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
BwspScoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
BwspScoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BwspScoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN BWSP_SCO_OPERATION.C
***********************************************************/

ANSC_STATUS
BwspScoEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BwspScoCancel
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN BWSP_SCO_CSPIF.C
***********************************************************/

ANSC_STATUS
BwspScoCspConfigProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN BWSP_SCO_WSRIF.C
***********************************************************/

ANSC_STATUS
BwspScoWsrAttachWebHost
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHostDescr
    );

ANSC_STATUS
BwspScoWsrDetachWebHost
    (
        ANSC_HANDLE                 hThisObject,
        char*                       host_name
    );

ANSC_STATUS
BwspScoWsrAttachServlet
    (
        ANSC_HANDLE                 hThisObject,
        char*                       host_name,
        ANSC_HANDLE                 hServletDescr
    );

ANSC_STATUS
BwspScoWsrDetachServlet
    (
        ANSC_HANDLE                 hThisObject,
        char*                       host_name,
        char*                       path_name
    );


#endif
