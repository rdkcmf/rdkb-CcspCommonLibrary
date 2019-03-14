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

    module:	slap_scogal_internal_api.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Slap Sco Gallery
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/15/03    initial revision.

**********************************************************************/


#ifndef  _SLAP_SCOGAL_INTERNAL_API_
#define  _SLAP_SCOGAL_INTERNAL_API_


/***********************************************************
        FUNCTIONS IMPLEMENTED IN SLAP_SCOGAL_STATES.C
***********************************************************/

ANSC_HANDLE
SlapScoGalGetSlapGoaIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
SlapScoGalGetOlmContainer
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
      FUNCTIONS IMPLEMENTED IN SLAP_SCOGAL_COLLECTION.C
***********************************************************/

SLAP_UINT32
SlapScoGalGetCount
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
      FUNCTIONS IMPLEMENTED IN SLAP_SCOGAL_MANAGEMENT.C
***********************************************************/

SLAP_VARIABLE*
SlapScoGalGetVariable
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    );

ANSC_STATUS
SlapScoGalSetVariable
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    );

ANSC_STATUS
SlapScoGalAddVariable
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    );

ANSC_STATUS
SlapScoGalDelVariable
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    );

ANSC_STATUS
SlapScoGalDelAllVariables
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
      FUNCTIONS IMPLEMENTED IN SLAP_SCOGAL_MEMBERSHIP.C
***********************************************************/

ANSC_STATUS
SlapScoGalJoinGallery
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_VARIABLE*              gallery_var
    );

ANSC_STATUS
SlapScoGalQuitGallery
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_VARIABLE*              gallery_var
    );


#endif
