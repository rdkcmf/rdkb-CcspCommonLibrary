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

    module:	ansc_mco_internal_api.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the external functions provided by the Module Container
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/12/02    initial revision.

**********************************************************************/


#ifndef  _ANSC_MCO_INTERNAL_API_
#define  _ANSC_MCO_INTERNAL_API_


/***********************************************************
          FUNCTIONS IMPLEMENTED IN ANSC_MCO_STATES.C
***********************************************************/

ANSC_HANDLE
AnscMcoGetQueryIfObject
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN ANSC_MCO_OPERATION.C
***********************************************************/

ANSC_STATUS
AnscMcoIgnite
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscMcoPutOut
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscMcoSwitch
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bSwitchOn
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN ANSC_MCO_QUERY.C
***********************************************************/

ANSC_STATUS
AnscMcoAddModuleIf
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pIfName,
        ANSC_HANDLE                 hInterface
    );

ANSC_STATUS
AnscMcoDelModuleIf
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pIfName
    );

ANSC_HANDLE
AnscMcoQueryModuleIf
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pIfName
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN ANSC_MCO_CONTROL.C
***********************************************************/

ANSC_STATUS
AnscMcoRegisterIfs
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscMcoDispatchIfs
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hIqcIf
    );


#endif
