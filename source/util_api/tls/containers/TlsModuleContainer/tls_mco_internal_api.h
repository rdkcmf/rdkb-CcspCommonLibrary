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

    module:	tls_mco_internal_api.h

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Tls Module
        Container Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/13/03    initial revision.

**********************************************************************/


#ifndef  _TLS_MCO_INTERNAL_API_
#define  _TLS_MCO_INTERNAL_API_


/***********************************************************
          FUNCTIONS IMPLEMENTED IN TLS_MCO_STATES.C
***********************************************************/

ANSC_HANDLE
TlsMcoGetTlsEnvController
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN TLS_MCO_EXTENSION.C
***********************************************************/

ANSC_STATUS
TlsMcoEnrollExtensionObjects
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsMcoManufactureExtensionObjects
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsMcoDestroyExtensionObjects
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN TLS_MCO_FEATURE.C
***********************************************************/

ANSC_STATUS
TlsMcoEnrollFeatureObjects
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsMcoManufactureFeatureObjects
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsMcoDestroyFeatureObjects
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN TLS_MCO_OPERATION.C
***********************************************************/

ANSC_STATUS
TlsMcoIgnite
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsMcoPutOut
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsMcoSwitch
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bSwitchOn
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN TLS_MCO_CONTROL.C
***********************************************************/

ANSC_STATUS
TlsMcoRegisterIfs
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsMcoDispatchIfs
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hIqcIf
    );


#endif
