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

    module:	slap_vmo_operation.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Slap Var Mapper Object.

        *   SlapVmoInitAtoSyntax
        *   SlapVmoInitAtoContentType

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Bin Zhu

    ---------------------------------------------------------------

    revision:

        07/12/03    initial revision.
        09/30/2010    Bin added ipv6_list support.

**********************************************************************/


#include "slap_vmo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapVmoInitAtoSyntax
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to initialize the atom table for
        SLAP object syntax.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapVmoInitAtoSyntax
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_VAR_MAPPER_OBJECT         pMyObject    = (PSLAP_VAR_MAPPER_OBJECT)hThisObject;
    PANSC_ATOM_TABLE_OBJECT         pAtoSyntax   = (PANSC_ATOM_TABLE_OBJECT)pMyObject->hAtoSyntax;

    pAtoSyntax->AddAtomByName
        (
            (ANSC_HANDLE)pAtoSyntax,
            SLAP_VAR_SYNTAX_NAME_other,
            SLAP_VAR_SYNTAX_TYPE_other,
            (ANSC_HANDLE)0
        );

    pAtoSyntax->AddAtomByName
        (
            (ANSC_HANDLE)pAtoSyntax,
            SLAP_VAR_SYNTAX_NAME_void,
            SLAP_VAR_SYNTAX_TYPE_void,
            (ANSC_HANDLE)0
        );

    pAtoSyntax->AddAtomByName
        (
            (ANSC_HANDLE)pAtoSyntax,
            SLAP_VAR_SYNTAX_NAME_bool,
            SLAP_VAR_SYNTAX_TYPE_bool,
            (ANSC_HANDLE)0
        );

    pAtoSyntax->AddAtomByName
        (
            (ANSC_HANDLE)pAtoSyntax,
            SLAP_VAR_SYNTAX_NAME_boolArray,
            SLAP_VAR_SYNTAX_TYPE_boolArray,
            (ANSC_HANDLE)0
        );

    pAtoSyntax->AddAtomByName
        (
            (ANSC_HANDLE)pAtoSyntax,
            SLAP_VAR_SYNTAX_NAME_charArray,
            SLAP_VAR_SYNTAX_TYPE_charArray,
            (ANSC_HANDLE)0
        );

    pAtoSyntax->AddAtomByName
        (
            (ANSC_HANDLE)pAtoSyntax,
            SLAP_VAR_SYNTAX_NAME_int,
            SLAP_VAR_SYNTAX_TYPE_int,
            (ANSC_HANDLE)0
        );

    pAtoSyntax->AddAtomByName
        (
            (ANSC_HANDLE)pAtoSyntax,
            SLAP_VAR_SYNTAX_NAME_intArray,
            SLAP_VAR_SYNTAX_TYPE_intArray,
            (ANSC_HANDLE)0
        );

    pAtoSyntax->AddAtomByName
        (
            (ANSC_HANDLE)pAtoSyntax,
            SLAP_VAR_SYNTAX_NAME_string,
            SLAP_VAR_SYNTAX_TYPE_string,
            (ANSC_HANDLE)0
        );

    pAtoSyntax->AddAtomByName
        (
            (ANSC_HANDLE)pAtoSyntax,
            SLAP_VAR_SYNTAX_NAME_stringArray,
            SLAP_VAR_SYNTAX_TYPE_stringArray,
            (ANSC_HANDLE)0
        );

    pAtoSyntax->AddAtomByName
        (
            (ANSC_HANDLE)pAtoSyntax,
            SLAP_VAR_SYNTAX_NAME_ucharArray,
            SLAP_VAR_SYNTAX_TYPE_ucharArray,
            (ANSC_HANDLE)0
        );

    pAtoSyntax->AddAtomByName
        (
            (ANSC_HANDLE)pAtoSyntax,
            SLAP_VAR_SYNTAX_NAME_uint32,
            SLAP_VAR_SYNTAX_TYPE_uint32,
            (ANSC_HANDLE)0
        );

    pAtoSyntax->AddAtomByName
        (
            (ANSC_HANDLE)pAtoSyntax,
            SLAP_VAR_SYNTAX_NAME_uint32Array,
            SLAP_VAR_SYNTAX_TYPE_uint32Array,
            (ANSC_HANDLE)0
        );

    pAtoSyntax->AddAtomByName
        (
            (ANSC_HANDLE)pAtoSyntax,
            SLAP_VAR_SYNTAX_NAME_ptr,
            SLAP_VAR_SYNTAX_TYPE_ptr,
            (ANSC_HANDLE)0
        );

    pAtoSyntax->AddAtomByName
        (
            (ANSC_HANDLE)pAtoSyntax,
            SLAP_VAR_SYNTAX_NAME_ptrArray,
            SLAP_VAR_SYNTAX_TYPE_ptrArray,
            (ANSC_HANDLE)0
        );

    pAtoSyntax->AddAtomByName
        (
            (ANSC_HANDLE)pAtoSyntax,
            SLAP_VAR_SYNTAX_NAME_handle,
            SLAP_VAR_SYNTAX_TYPE_handle,
            (ANSC_HANDLE)0
        );

    pAtoSyntax->AddAtomByName
        (
            (ANSC_HANDLE)pAtoSyntax,
            SLAP_VAR_SYNTAX_NAME_handleArray,
            SLAP_VAR_SYNTAX_TYPE_handleArray,
            (ANSC_HANDLE)0
        );

    pAtoSyntax->AddAtomByName
        (
            (ANSC_HANDLE)pAtoSyntax,
            SLAP_VAR_SYNTAX_NAME_object,
            SLAP_VAR_SYNTAX_TYPE_object,
            (ANSC_HANDLE)0
        );

    pAtoSyntax->AddAtomByName
        (
            (ANSC_HANDLE)pAtoSyntax,
            SLAP_VAR_SYNTAX_NAME_objectArray,
            SLAP_VAR_SYNTAX_TYPE_objectArray,
            (ANSC_HANDLE)0
        );

    pAtoSyntax->AddAtomByName
        (
            (ANSC_HANDLE)pAtoSyntax,
            SLAP_VAR_SYNTAX_NAME_variable,
            SLAP_VAR_SYNTAX_TYPE_variable,
            (ANSC_HANDLE)0
        );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapVmoInitAtoContentType
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to initialize the atom table for
        SLAP content type.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapVmoInitAtoContentType
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_VAR_MAPPER_OBJECT         pMyObject       = (PSLAP_VAR_MAPPER_OBJECT)hThisObject;
    PANSC_ATOM_TABLE_OBJECT         pAtoContentType = (PANSC_ATOM_TABLE_OBJECT)pMyObject->hAtoContentType;

    pAtoContentType->AddAtomByName
        (
            (ANSC_HANDLE)pAtoContentType,
            SLAP_CONTENT_NAME_UNSPECIFIED,
            SLAP_CONTENT_TYPE_UNSPECIFIED,
            (ANSC_HANDLE)0
        );

    pAtoContentType->AddAtomByName
        (
            (ANSC_HANDLE)pAtoContentType,
            SLAP_CONTENT_NAME_MAPPED,
            SLAP_CONTENT_TYPE_MAPPED,
            (ANSC_HANDLE)0
        );

    pAtoContentType->AddAtomByName
        (
            (ANSC_HANDLE)pAtoContentType,
            SLAP_CONTENT_NAME_IP4_ADDR,
            SLAP_CONTENT_TYPE_IP4_ADDR,
            (ANSC_HANDLE)0
        );

    pAtoContentType->AddAtomByName
        (
            (ANSC_HANDLE)pAtoContentType,
            SLAP_CONTENT_NAME_IP4_ADDR_WILDCARD,
            SLAP_CONTENT_TYPE_IP4_ADDR_WILDCARD,
            (ANSC_HANDLE)0
        );

    pAtoContentType->AddAtomByName
        (
            (ANSC_HANDLE)pAtoContentType,
            SLAP_CONTENT_NAME_MAC_ADDR,
            SLAP_CONTENT_TYPE_MAC_ADDR,
            (ANSC_HANDLE)0
        );

    pAtoContentType->AddAtomByName
        (
            (ANSC_HANDLE)pAtoContentType,
            SLAP_CONTENT_NAME_IP6_ADDR,
            SLAP_CONTENT_TYPE_IP6_ADDR,
            (ANSC_HANDLE)0
        );

    pAtoContentType->AddAtomByName
        (
            (ANSC_HANDLE)pAtoContentType,
            SLAP_CONTENT_NAME_IP6_ADDR_LIST,
            SLAP_CONTENT_TYPE_IP6_ADDR_LIST,
            (ANSC_HANDLE)0
        );


    pAtoContentType->AddAtomByName
        (
            (ANSC_HANDLE)pAtoContentType,
            SLAP_CONTENT_NAME_IP4_ADDR_LIST,
            SLAP_CONTENT_TYPE_IP4_ADDR_LIST,
            (ANSC_HANDLE)0
        );

    pAtoContentType->AddAtomByName
        (
            (ANSC_HANDLE)pAtoContentType,
            SLAP_CONTENT_NAME_MAC_ADDR_LIST,
            SLAP_CONTENT_TYPE_MAC_ADDR_LIST,
            (ANSC_HANDLE)0
        );

    pAtoContentType->AddAtomByName
        (
            (ANSC_HANDLE)pAtoContentType,
            SLAP_CONTENT_NAME_TCPUDP_PORT,
            SLAP_CONTENT_TYPE_TCPUDP_PORT,
            (ANSC_HANDLE)0
        );

    pAtoContentType->AddAtomByName
        (
            (ANSC_HANDLE)pAtoContentType,
            SLAP_CONTENT_NAME_CALENDAR_TIME,
            SLAP_CONTENT_TYPE_CALENDAR_TIME,
            (ANSC_HANDLE)0
        );

    pAtoContentType->AddAtomByName
        (
            (ANSC_HANDLE)pAtoContentType,
            SLAP_CONTENT_NAME_PASSWORD,
            SLAP_CONTENT_TYPE_PASSWORD,
            (ANSC_HANDLE)0
        );

    pAtoContentType->AddAtomByName
        (
            (ANSC_HANDLE)pAtoContentType,
            SLAP_CONTENT_NAME_MASKED,
            SLAP_CONTENT_TYPE_MASKED,
            (ANSC_HANDLE)0
        );

    pAtoContentType->AddAtomByName
        (
            (ANSC_HANDLE)pAtoContentType,
            SLAP_CONTENT_NAME_OID_LIST,
            SLAP_CONTENT_TYPE_OID_LIST,
            (ANSC_HANDLE)0
        );

    pAtoContentType->AddAtomByName
        (
            (ANSC_HANDLE)pAtoContentType,
            SLAP_CONTENT_NAME_ASCII_STRING,
            SLAP_CONTENT_TYPE_ASCII_STRING,
            (ANSC_HANDLE)0
        );

    pAtoContentType->AddAtomByName
        (
            (ANSC_HANDLE)pAtoContentType,
            SLAP_CONTENT_NAME_HEX_STRING,
            SLAP_CONTENT_TYPE_HEX_STRING,
            (ANSC_HANDLE)0
        );

    pAtoContentType->AddAtomByName
        (
            (ANSC_HANDLE)pAtoContentType,
            SLAP_CONTENT_NAME_SUBNET_MASK,
            SLAP_CONTENT_TYPE_SUBNET_MASK,
            (ANSC_HANDLE)0
        );

    pAtoContentType->AddAtomByName
        (
            (ANSC_HANDLE)pAtoContentType,
            SLAP_CONTENT_NAME_URL_STRING,
            SLAP_CONTENT_TYPE_URL_STRING,
            (ANSC_HANDLE)0
        );

    pAtoContentType->AddAtomByName
        (
            (ANSC_HANDLE)pAtoContentType,
            SLAP_CONTENT_NAME_TABLE_2D,
            SLAP_CONTENT_TYPE_TABLE_2D,
            (ANSC_HANDLE)0
        );

    pAtoContentType->AddAtomByName
        (
            (ANSC_HANDLE)pAtoContentType,
            SLAP_CONTENT_NAME_TABLE_3D,
            SLAP_CONTENT_TYPE_TABLE_3D,
            (ANSC_HANDLE)0
        );

    pAtoContentType->AddAtomByName
        (
            (ANSC_HANDLE)pAtoContentType,
            SLAP_CONTENT_NAME_SIMPLE_CHART_PIE,
            SLAP_CONTENT_TYPE_SIMPLE_CHART_PIE,
            (ANSC_HANDLE)0
        );

    pAtoContentType->AddAtomByName
        (
            (ANSC_HANDLE)pAtoContentType,
            SLAP_CONTENT_NAME_SIMPLE_CHART_BAR,
            SLAP_CONTENT_TYPE_SIMPLE_CHART_BAR,
            (ANSC_HANDLE)0
        );

    pAtoContentType->AddAtomByName
        (
            (ANSC_HANDLE)pAtoContentType,
            SLAP_CONTENT_NAME_SIMPLE_GRAPH,
            SLAP_CONTENT_TYPE_SIMPLE_GRAPH,
            (ANSC_HANDLE)0
        );

    return  ANSC_STATUS_SUCCESS;
}
