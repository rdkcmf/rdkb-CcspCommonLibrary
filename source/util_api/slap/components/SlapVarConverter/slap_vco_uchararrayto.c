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

    module:	slap_vco_uchararrayto.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced conversion functions
        of the Slap Var Converter Object.

        *   SlapVcoUcharArrayToUint32

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

        SLAP_UINT32
        SlapVcoUcharArrayToUint32
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_UCHAR_ARRAY*           var_uchar_array
            );

    description:

        This function is called to convert string to varUint.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_UCHAR_ARRAY*           var_uchar_array
                Specifies the slap variable string to be converted.

    return:     varUint32.

**********************************************************************/

SLAP_UINT32
SlapVcoUcharArrayToUint32
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_UCHAR_ARRAY*           var_uchar_array
    )
{
    UNREFERENCED_PARAMETER(hThisObject);

    if ( !var_uchar_array )
    {
        return  0;
    }

    switch ( var_uchar_array->VarCount )
    {
        case    sizeof(UCHAR) :

                return  (ULONG)var_uchar_array->Array.arrayUchar[0];

                break;

        case    sizeof(USHORT) :

                return  AnscReadUshort(var_uchar_array->Array.arrayUchar);

                break;

        case    sizeof(ULONG) :

                return  AnscReadUlong(var_uchar_array->Array.arrayUchar);

                break;

        default :

                return  0;
    }

    return  0;
}
