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

    module:	slap_vmo_mapping.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced mapping functions
        of the Slap Var Mapper Object.

        *   SlapVmoGetVarSyntax
        *   SlapVmoGetVarContentType

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/12/03    initial revision.

**********************************************************************/


#include "slap_vmo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SlapVmoGetVarSyntax
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name
            );

    description:

        This function is called to retrieve the SLAP syntax type.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the syntax name string to be matched.

    return:     syntax type.

**********************************************************************/

ULONG
SlapVmoGetVarSyntax
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    )
{
    PSLAP_VAR_MAPPER_OBJECT         pMyObject       = (PSLAP_VAR_MAPPER_OBJECT)hThisObject;
    PANSC_ATOM_TABLE_OBJECT         pAtoSyntax      = (PANSC_ATOM_TABLE_OBJECT)pMyObject->hAtoSyntax;
    PANSC_ATOM_DESCRIPTOR           pAtomDescriptor = (PANSC_ATOM_DESCRIPTOR  )NULL;

    pAtomDescriptor =
        (PANSC_ATOM_DESCRIPTOR)pAtoSyntax->GetAtomByName
            (
                (ANSC_HANDLE)pAtoSyntax,
                name
            );

    if ( !pAtomDescriptor )
    {
        return  SLAP_VAR_SYNTAX_other;
    }

    return  pAtomDescriptor->AtomId;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SlapVmoGetVarContentType
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name
            );

    description:

        This function is called to retrieve the SLAP content type.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the content name string to be matched.

    return:     content type.

**********************************************************************/

ULONG
SlapVmoGetVarContentType
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    )
{
    PSLAP_VAR_MAPPER_OBJECT         pMyObject       = (PSLAP_VAR_MAPPER_OBJECT)hThisObject;
    PANSC_ATOM_TABLE_OBJECT         pAtoContentType = (PANSC_ATOM_TABLE_OBJECT)pMyObject->hAtoContentType;
    PANSC_ATOM_DESCRIPTOR           pAtomDescriptor = (PANSC_ATOM_DESCRIPTOR  )NULL;

    pAtomDescriptor =
        (PANSC_ATOM_DESCRIPTOR)pAtoContentType->GetAtomByName
            (
                (ANSC_HANDLE)pAtoContentType,
                name
            );

    if ( !pAtomDescriptor )
    {
        return  SLAP_CONTENT_TYPE_UNSPECIFIED;
    }

    return  pAtomDescriptor->AtomId;
}
