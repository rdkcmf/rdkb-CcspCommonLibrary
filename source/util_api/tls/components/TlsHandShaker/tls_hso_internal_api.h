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

    module:	tls_hso_internal_api.h

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the TLS Hand Shaker
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/28/03    initial revision.

**********************************************************************/


#ifndef  _TLS_HSO_INTERNAL_API_
#define  _TLS_HSO_INTERNAL_API_


/***********************************************************
          FUNCTIONS IMPLEMENTED IN TLS_HSO_CONTROL.C
***********************************************************/

ANSC_STATUS
TlsHsoStart
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsHsoAbort
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsHsoEstablish
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsHsoAgree
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bResumed
    );

ANSC_STATUS
TlsHsoCalKeys30
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsHsoCalKeys31
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsHsoCalMasterSecret30
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsHsoCalMasterSecret31
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsHsoChangeCipherW
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsHsoChangeCipherR
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN TLS_HSO_OPERATION.C
***********************************************************/

ANSC_STATUS
TlsHsoEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsHsoCancel
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsHsoSaveHandshakeMsg
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
TlsHsoSaveFragHandshakeMsg
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
TlsHsoStateTimerInvoke
    (
        ANSC_HANDLE                 hThisObject
    );


#endif
