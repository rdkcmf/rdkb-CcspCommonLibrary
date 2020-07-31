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

    module:	slap_vco_touchararray.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced conversion functions
        of the Slap Var Converter Object.

        *   SlapVcoUint32ToUcharArray
        *   SlapVcoUint32ToUchar

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/02/04    initial revision.

**********************************************************************/


#include "slap_vco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_UCHAR_ARRAY*
        SlapVcoUint32ToUcharArray
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_UINT32                 var_uint32
            );

    description:

        This function is called to convert varUint32 to uchar_array.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_UINT32                 var_uint32
                Specifies the slap variable to be converted.

    return:     varUcharArray.

**********************************************************************/

SLAP_UCHAR_ARRAY*
SlapVcoUint32ToUcharArray
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32                 var_uint32
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    SLAP_UCHAR_ARRAY*               var_ucharArray = (SLAP_UCHAR_ARRAY*         )NULL;
    ULONG                           ulUcharCount   = sizeof(ULONG);

    var_ucharArray = (SLAP_UCHAR_ARRAY*)AnscAllocateMemory(sizeof(SLAP_UCHAR_ARRAY) + ulUcharCount);

    if ( !var_ucharArray )
    {
        return  NULL;
    }
    else
    {
        var_ucharArray->Size     = sizeof(SLAP_UCHAR_ARRAY) + ulUcharCount;
        var_ucharArray->VarCount = ulUcharCount;
        var_ucharArray->Syntax   = SLAP_VAR_SYNTAX_ucharArray;

        AnscWriteUlong(var_ucharArray->Array.arrayUchar, var_uint32);
    }

    return  var_ucharArray;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        SLAP_UCHAR_ARRAY*
        SlapVcoUint32ToUchar
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_UINT32                 var_uint32
            );

    description:

        This function is called to convert varUint32 to uchar_array.
        The difference is that this function must return a ucharArray
        with length 1.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_UINT32                 var_uint32
                Specifies the slap variable to be converted.

    return:     varUcharArray.

**********************************************************************/

SLAP_UCHAR_ARRAY*
SlapVcoUint32ToUchar
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UINT32                 var_uint32
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    SLAP_UCHAR_ARRAY*               var_ucharArray = (SLAP_UCHAR_ARRAY*         )NULL;
    ULONG                           ulUcharCount   = 1;

    var_ucharArray = (SLAP_UCHAR_ARRAY*)AnscAllocateMemory(sizeof(SLAP_UCHAR_ARRAY) + ulUcharCount);

    if ( !var_ucharArray )
    {
        return  NULL;
    }
    else
    {
        var_ucharArray->Size     = sizeof(SLAP_UCHAR_ARRAY) + ulUcharCount;
        var_ucharArray->VarCount = ulUcharCount;
        var_ucharArray->Syntax   = SLAP_VAR_SYNTAX_ucharArray;

        var_ucharArray->Array.arrayUchar[0] = (UCHAR)var_uint32;
    }

    return  var_ucharArray;
}
