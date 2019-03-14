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

    module:	tls_seo_internal_api.h

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the TLS Session Entry
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/16/03    initial revision.

**********************************************************************/


#ifndef  _TLS_SEO_INTERNAL_API_
#define  _TLS_SEO_INTERNAL_API_


/***********************************************************
          FUNCTIONS IMPLEMENTED IN TLS_SEO_STATES.C
***********************************************************/

ANSC_STATUS
TlsSeoGetSessionState
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSessionState
    );

ANSC_STATUS
TlsSeoSetSessionState
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSessionState
    );

ANSC_STATUS
TlsSeoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN TLS_SEO_OPERATION.C
***********************************************************/

BOOL
TlsSeoMatch1
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       host_id,
        ULONG                       peer_id,
        ULONG                       conn_end
    );

BOOL
TlsSeoMatch2
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       host_id,
        ULONG                       peer_id,
        PUCHAR                      session_id,
        ULONG                       session_id_size
    );


#endif
