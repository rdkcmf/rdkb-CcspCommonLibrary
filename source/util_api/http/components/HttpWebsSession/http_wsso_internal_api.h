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

    module:	http_wsso_internal_api.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Http Webs Session
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/08/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_WSSO_INTERNAL_API_
#define  _HTTP_WSSO_INTERNAL_API_


/***********************************************************
         FUNCTIONS IMPLEMENTED IN HTTP_WSSO_STATES.C
***********************************************************/

ANSC_HANDLE
HttpWssoGetWspIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWssoSetWspIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
HttpWssoGetHfpIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWssoSetHfpIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
HttpWssoGetWebSocket
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWssoSetWebSocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

ULONG
HttpWssoGetSessionState
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWssoSetSessionState
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulState
    );

ANSC_STATUS
HttpWssoReturn
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWssoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN HTTP_WSSO_OPERATION.C
***********************************************************/

ANSC_STATUS
HttpWssoOpen
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWssoClose
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWssoAcquireAccess
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWssoReleaseAccess
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWssoEnterWspServe
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWssoLeaveWspServe
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWssoClearWspServe
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWssoCloseConnection
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN HTTP_WSSO_MANAGEMENT.C
***********************************************************/

ANSC_HANDLE
HttpWssoGetEndWsto
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
HttpWssoGetCurWsto
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
HttpWssoAddNewWsto
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWssoDelAllWstos
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN HTTP_WSSO_PROCESS.C
***********************************************************/

ULONG
HttpWssoQuery
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    );

ANSC_STATUS
HttpWssoRecv
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    );

ANSC_STATUS
HttpWssoFinish
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    );

BOOL
HttpWssoAccept
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReserved
    );


#endif
