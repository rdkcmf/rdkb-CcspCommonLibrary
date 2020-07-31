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

    module:	slap_vco_uint32arrayto.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced conversion functions
        of the Slap Var Converter Object.

        *   SlapVcoUint32ArrayToMacAddr

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        12/16/03    initial revision.

**********************************************************************/


#include "slap_vco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_UCHAR_ARRAY*
        SlapVcoUint32ArrayToMacAddr
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_UINT32_ARRAY*          uint32_array
            );

    description:

        This function is called to convert varUint32Array to
        varUcharArray/mac_addr.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_UINT32_ARRAY*          uint32_array
                Specifies the slap uint32 array to be converted.

    return:     varUcharArray.

**********************************************************************/

SLAP_UCHAR_ARRAY*
SlapVcoUint32ArrayToMacAddr
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32_ARRAY*          uint32_array
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    SLAP_UCHAR_ARRAY*               var_ucharArray = (SLAP_UCHAR_ARRAY*         )NULL;
    ULONG                           i              = 0;

    var_ucharArray = (SLAP_UCHAR_ARRAY*)AnscAllocateMemory(sizeof(SLAP_UCHAR_ARRAY) + 6);

    if ( !var_ucharArray )
    {
        return  NULL;
    }
    else
    {
        var_ucharArray->Size     = sizeof(SLAP_UCHAR_ARRAY) + 6;
        var_ucharArray->VarCount = 6;
        var_ucharArray->Syntax   = SLAP_VAR_SYNTAX_ucharArray;
    }

    for ( i = 0; i < (ULONG)AnscGetMin2(uint32_array->VarCount, 6); i++ )
    {
        var_ucharArray->Array.arrayUchar[i] = (UCHAR)uint32_array->Array.arrayUint32[i];
    }

    return  var_ucharArray;
}
