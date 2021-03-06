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

    module:	bwrm_pmo_exported_api.h

        For Broadway Web Resource Manager Implementation (BWRM),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the exported functions provided by the Bwrm Page Manager
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/07/03    initial revision.

**********************************************************************/


#ifndef  _BWRM_PMO_EXPORTED_API_
#define  _BWRM_PMO_EXPORTED_API_


/***********************************************************
        FUNCTIONS IMPLEMENTED IN BWRM_PMO_INTERFACE.C
***********************************************************/

ANSC_HANDLE
BwrmCreatePageManager
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );


/***********************************************************
           FUNCTIONS IMPLEMENTED IN BWRM_PMO_BASE.C
***********************************************************/

ANSC_HANDLE
BwrmPmoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );

ANSC_STATUS
BwrmPmoRemove
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BwrmPmoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BwrmPmoInitialize
    (
        ANSC_HANDLE                 hThisObject
    );


#endif
