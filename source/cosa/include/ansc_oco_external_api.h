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

    module:	ansc_oco_external_api.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the external functions provided by the Object Container.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        10/15/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_OCO_EXTERNAL_API_
#define  _ANSC_OCO_EXTERNAL_API_


/***********************************************************
        FUNCTIONS IMPLEMENTED IN ANSC_OCO_INTERFACE.C
***********************************************************/

ANSC_HANDLE
AnscCreateObjectContainer
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );


/***********************************************************
           FUNCTIONS IMPLEMENTED IN ANSC_OCO_BASE.C
***********************************************************/

ANSC_HANDLE
AnscOcoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );

ANSC_STATUS
AnscOcoRemove
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscOcoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscOcoInitialize
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN ANSC_OCO_OPERATION.C
***********************************************************/

ANSC_STATUS
AnscOcoEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscOcoCancel
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscOcoSubEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscOcoSubCancel
    (
        ANSC_HANDLE                 hThisObject
    );


#endif
