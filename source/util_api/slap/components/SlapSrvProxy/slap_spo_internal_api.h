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

    module:	slap_spo_internal_api.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Slap Srv Proxy
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        10/06/03    initial revision.

**********************************************************************/


#ifndef  _SLAP_SPO_INTERNAL_API_
#define  _SLAP_SPO_INTERNAL_API_


/***********************************************************
          FUNCTIONS IMPLEMENTED IN SLAP_SPO_STATES.C
***********************************************************/

ULONG
SlapSpoGetCallTimeout
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapSpoSetCallTimeout
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       timeout
    );

char*
SlapSpoGetObjName
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapSpoSetObjName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

ULONG
SlapSpoGetObjType
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapSpoSetObjType
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       obj_type
    );

ULONG
SlapSpoGetObjMode
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapSpoSetObjMode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       obj_mode
    );

ANSC_HANDLE
SlapSpoGetSlapGoaObj
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapSpoSetSlapGoaObj
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSlapGoaObj
    );

ANSC_HANDLE
SlapSpoGetSlapUoaIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapSpoSetSlapUoaIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
SlapSpoGetSlapGoaIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapSpoSetSlapGoaIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
SlapSpoGetSlapPoaIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapSpoSetSlapPoaIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN SLAP_SPO_OPERATION.C
***********************************************************/

ANSC_STATUS
SlapSpoEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapSpoCancel
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
SlapSpoInvokeDispatch
    (
        ANSC_HANDLE                 hThisObject,
        char*                       method_name,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST**       params_out,
        SLAP_VARIABLE**             return_var
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN SLAP_SPO_UTILITIES.C
***********************************************************/

ANSC_STATUS
SlapSpoPoaToGoaParamList
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    );

ANSC_STATUS
SlapSpoGoaToPoaParamList
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    );

ANSC_STATUS
SlapSpoPoaToGoaVariable
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_VARIABLE*              slap_var
    );

ANSC_STATUS
SlapSpoGoaToPoaVariable
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_VARIABLE*              slap_var
    );


#endif
