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

    module:	slap_vho_array.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced array manipulation
        functions of the Slap Var Helper Object.

        *   SlapVhoAllocBoolArray
        *   SlapVhoAllocBoolArrayFromImcpVar
        *   SlapVhoAllocCharArray
        *   SlapVhoAllocCharArrayFromImcpVar
        *   SlapVhoAllocIntArray
        *   SlapVhoAllocIntArrayFromImcpVar
        *   SlapVhoAllocStringArray
        *   SlapVhoAllocStringArray2
        *   SlapVhoAllocStringArrayFromImcpVar
        *   SlapVhoAllocUcharArray
        *   SlapVhoAllocUcharArrayFromImcpVar
        *   SlapVhoAllocUint32Array
        *   SlapVhoAllocUint32ArrayFromImcpVar
        *   SlapVhoAllocPtrArray
        *   SlapVhoAllocPtrArrayFromImcpVar
        *   SlapVhoAllocHandleArray
        *   SlapVhoAllocHandleArrayFromImcpVar
        *   SlapVhoAllocObjectArray
        *   SlapVhoAllocObjectArrayFromImcpVar
        *   SlapVhoEqualVarArrays
        *   SlapVhoCloneVarArray
        *   SlapVhoCleanVarArray

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/25/05    initial revision.

**********************************************************************/


#include "slap_vho_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapVhoAllocBoolArray
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      var_data,
                ULONG                       var_length
            );

    description:

        This function is called to allocate a variable array.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      var_data
                Specifies the raw data of the array.

                ULONG                       var_length
                Specifies the length of the raw data.

    return:     variable array.

**********************************************************************/

ANSC_HANDLE
SlapVhoAllocBoolArray
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      var_data,
        ULONG                       var_length
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_BOOL_ARRAY                slap_array = NULL;
    ULONG                           var_count  = var_length / sizeof(SLAP_BOOL);
    ULONG                           total_size = 0;
    char*                           var_point  = (char*)var_data;
    ULONG                           jj         = 0;

    total_size = sizeof(SLAP_VAR_ARRAY) + sizeof(SLAP_BOOL) * var_count;
    slap_array = (PSLAP_BOOL_ARRAY)AnscAllocateMemory(total_size);

    if ( slap_array )
    {
        slap_array->Size     = total_size;
        slap_array->VarCount = var_count;
        slap_array->Syntax   = SLAP_VAR_SYNTAX_boolArray;

        if ( var_data )
        {
            for ( jj = 0; jj < var_count; jj++ )
            {
                slap_array->Array.arrayBool[jj] =
                    (SLAP_BOOL)AnscReadUlong(var_point);
                var_point                      += sizeof(SLAP_BOOL);
            }
        }
    }

    return  (ANSC_HANDLE)slap_array;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapVhoAllocBoolArrayFromImcpVar
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      imcp_var_data,
                ULONG                       imcp_var_length
            );

    description:

        This function is called to allocate a variable array from an
        IMCP variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      imcp_var_data
                Specifies the raw IMCP data of the array.

                ULONG                       imcp_var_length
                Specifies the length of the raw IMCP data.

    return:     variable array.

**********************************************************************/

ANSC_HANDLE
SlapVhoAllocBoolArrayFromImcpVar
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      imcp_var_data,
        ULONG                       imcp_var_length
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_BOOL_ARRAY                slap_array = NULL;
    ULONG                           var_count  = imcp_var_length / sizeof(ULONG);
    ULONG                           total_size = 0;
    char*                           var_point  = (char*)imcp_var_data;
    ULONG                           jj         = 0;

    total_size = sizeof(SLAP_VAR_ARRAY) + sizeof(SLAP_BOOL) * var_count;
    slap_array = (PSLAP_BOOL_ARRAY)AnscAllocateMemory(total_size);

    if ( slap_array )
    {
        slap_array->Size     = total_size;
        slap_array->VarCount = var_count;
        slap_array->Syntax   = SLAP_VAR_SYNTAX_boolArray;

        for ( jj = 0; jj < var_count; jj++ )
        {
            slap_array->Array.arrayBool[jj] =
                (SLAP_BOOL)AnscUlongFromNToH(AnscReadUlong(var_point));
            var_point                      += sizeof(ULONG);
        }
    }

    return  (ANSC_HANDLE)slap_array;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapVhoAllocCharArray
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      var_data,
                ULONG                       var_length
            );

    description:

        This function is called to allocate a variable array.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      var_data
                Specifies the raw data of the array.

                ULONG                       var_length
                Specifies the length of the raw data.

    return:     variable array.

**********************************************************************/

ANSC_HANDLE
SlapVhoAllocCharArray
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      var_data,
        ULONG                       var_length
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_CHAR_ARRAY                slap_array = NULL;
    ULONG                           var_count  = var_length / sizeof(SLAP_CHAR);
    ULONG                           total_size = 0;
    char*                           var_point  = (char*)var_data;
    ULONG                           jj         = 0;

    total_size = sizeof(SLAP_VAR_ARRAY) + sizeof(SLAP_CHAR) * var_count;
    slap_array = (PSLAP_CHAR_ARRAY)AnscAllocateMemory(total_size);

    if ( slap_array )
    {
        slap_array->Size     = total_size;
        slap_array->VarCount = var_count;
        slap_array->Syntax   = SLAP_VAR_SYNTAX_charArray;

        if ( var_data )
        {
            for ( jj = 0; jj < var_count; jj++ )
            {
                slap_array->Array.arrayChar[jj] = (SLAP_CHAR)*var_point;
                var_point                      += sizeof(SLAP_CHAR);
            }
        }
    }

    return  (ANSC_HANDLE)slap_array;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapVhoAllocCharArrayFromImcpVar
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      imcp_var_data,
                ULONG                       imcp_var_length
            );

    description:

        This function is called to allocate a variable array from an
        IMCP variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      imcp_var_data
                Specifies the raw IMCP data of the array.

                ULONG                       imcp_var_length
                Specifies the length of the raw IMCP data.

    return:     variable array.

**********************************************************************/

ANSC_HANDLE
SlapVhoAllocCharArrayFromImcpVar
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      imcp_var_data,
        ULONG                       imcp_var_length
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_CHAR_ARRAY                slap_array = NULL;
    ULONG                           var_count  = imcp_var_length / sizeof(SLAP_CHAR);
    ULONG                           total_size = 0;
    char*                           var_point  = (char*)imcp_var_data;
    ULONG                           jj         = 0;

    total_size = sizeof(SLAP_VAR_ARRAY) + sizeof(SLAP_CHAR) * var_count;
    slap_array = (PSLAP_CHAR_ARRAY)AnscAllocateMemory(total_size);

    if ( slap_array )
    {
        slap_array->Size     = total_size;
        slap_array->VarCount = var_count;
        slap_array->Syntax   = SLAP_VAR_SYNTAX_charArray;

        for ( jj = 0; jj < var_count; jj++ )
        {
            slap_array->Array.arrayChar[jj] = (SLAP_CHAR)*var_point;
            var_point                      += sizeof(SLAP_CHAR);
        }
    }

    return  (ANSC_HANDLE)slap_array;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapVhoAllocIntArray
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      var_data,
                ULONG                       var_length
            );

    description:

        This function is called to allocate a variable array.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      var_data
                Specifies the raw data of the array.

                ULONG                       var_length
                Specifies the length of the raw data.

    return:     variable array.

**********************************************************************/

ANSC_HANDLE
SlapVhoAllocIntArray
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      var_data,
        ULONG                       var_length
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_INT_ARRAY                 slap_array = NULL;
    ULONG                           var_count  = var_length / sizeof(SLAP_INT);
    ULONG                           total_size = 0;
    char*                           var_point  = (char*)var_data;
    ULONG                           jj         = 0;

    total_size = sizeof(SLAP_VAR_ARRAY) + sizeof(SLAP_INT) * var_count;
    slap_array = (PSLAP_INT_ARRAY)AnscAllocateMemory(total_size);

    if ( slap_array )
    {
        slap_array->Size     = total_size;
        slap_array->VarCount = var_count;
        slap_array->Syntax   = SLAP_VAR_SYNTAX_intArray;

        if ( var_data )
        {
            for ( jj = 0; jj < var_count; jj++ )
            {
                slap_array->Array.arrayInt[jj] =
                    (SLAP_INT)AnscReadUlong(var_point);
                var_point                     += sizeof(SLAP_INT);
            }
        }
    }

    return  (ANSC_HANDLE)slap_array;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapVhoAllocIntArrayFromImcpVar
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      imcp_var_data,
                ULONG                       imcp_var_length
            );

    description:

        This function is called to allocate a variable array from an
        IMCP variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      imcp_var_data
                Specifies the raw IMCP data of the array.

                ULONG                       imcp_var_length
                Specifies the length of the raw IMCP data.

    return:     variable array.

**********************************************************************/

ANSC_HANDLE
SlapVhoAllocIntArrayFromImcpVar
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      imcp_var_data,
        ULONG                       imcp_var_length
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_INT_ARRAY                 slap_array = NULL;
    ULONG                           var_count  = imcp_var_length / sizeof(SLAP_INT);
    ULONG                           total_size = 0;
    char*                           var_point  = (char*)imcp_var_data;
    ULONG                           jj         = 0;

    total_size = sizeof(SLAP_VAR_ARRAY) + sizeof(SLAP_INT) * var_count;
    slap_array = (PSLAP_INT_ARRAY)AnscAllocateMemory(total_size);

    if ( slap_array )
    {
        slap_array->Size     = total_size;
        slap_array->VarCount = var_count;
        slap_array->Syntax   = SLAP_VAR_SYNTAX_intArray;

        for ( jj = 0; jj < var_count; jj++ )
        {
            slap_array->Array.arrayInt[jj] =
                (SLAP_INT)AnscUlongFromNToH(AnscReadUlong(var_point));
            var_point                     += sizeof(ULONG);
        }
    }

    return  (ANSC_HANDLE)slap_array;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapVhoAllocStringArray
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      var_data,
                ULONG                       var_length
            );

    description:

        This function is called to allocate a variable array.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      var_data
                Specifies the raw data of the array.

                ULONG                       var_length
                Specifies the length of the raw data.

    return:     variable array.

**********************************************************************/

ANSC_HANDLE
SlapVhoAllocStringArray
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      var_data,
        ULONG                       var_length
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_STRING_ARRAY              slap_array = NULL;
    ULONG                           var_count  = 0;
    ULONG                           total_size = 0;
    char*                           var_point  = (char*)var_data;
    ULONG                           jj         = 0;

    for ( jj = 0; jj < var_length; jj++ )
    {
        if ( var_point[jj] == 0 )
        {
            var_count++;
        }
    }

    total_size = sizeof(SLAP_VAR_ARRAY) + sizeof(SLAP_STRING) * var_count;
    slap_array = (PSLAP_STRING_ARRAY)AnscAllocateMemory(total_size);

    if ( slap_array )
    {
        slap_array->Size     = total_size;
        slap_array->VarCount = var_count;
        slap_array->Syntax   = SLAP_VAR_SYNTAX_stringArray;

        if ( var_data )
        {
            for ( jj = 0; jj < var_count; jj++ )
            {
                slap_array->Array.arrayString[jj] =
                    (SLAP_STRING)AnscCloneString(var_point);
                var_point                        += AnscSizeOfString(var_point) + 1;
            }
        }
    }

    return  (ANSC_HANDLE)slap_array;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapVhoAllocStringArray2
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       var_count
            );

    description:

        This function is called to allocate a variable array.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       var_count
                Specifies the number of the raw data unit.

    return:     variable array.

**********************************************************************/

ANSC_HANDLE
SlapVhoAllocStringArray2
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       var_count
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_STRING_ARRAY              slap_array = NULL;
    ULONG                           total_size = 0;
    ULONG                           jj         = 0;

    total_size = sizeof(SLAP_VAR_ARRAY) + sizeof(SLAP_STRING) * var_count;
    slap_array = (PSLAP_STRING_ARRAY)AnscAllocateMemory(total_size);

    if ( slap_array )
    {
        slap_array->Size     = total_size;
        slap_array->VarCount = var_count;
        slap_array->Syntax   = SLAP_VAR_SYNTAX_stringArray;

        for ( jj = 0; jj < var_count; jj++ )
        {
            slap_array->Array.arrayString[jj] = (SLAP_STRING)NULL;
        }
    }

    return  (ANSC_HANDLE)slap_array;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapVhoAllocStringArrayFromImcpVar
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      imcp_var_data,
                ULONG                       imcp_var_length
            );

    description:

        This function is called to allocate a variable array from an
        IMCP variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      imcp_var_data
                Specifies the raw IMCP data of the array.

                ULONG                       imcp_var_length
                Specifies the length of the raw IMCP data.

    return:     variable array.

**********************************************************************/

ANSC_HANDLE
SlapVhoAllocStringArrayFromImcpVar
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      imcp_var_data,
        ULONG                       imcp_var_length
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_STRING_ARRAY              slap_array = NULL;
    ULONG                           var_count  = 0;
    ULONG                           total_size = 0;
    char*                           var_point  = (char*)imcp_var_data;
    ULONG                           jj         = 0;

    for ( jj = 0; jj < imcp_var_length; jj++ )
    {
        if ( var_point[jj] == 0 )
        {
            var_count++;
        }
    }

    total_size = sizeof(SLAP_VAR_ARRAY) + sizeof(SLAP_STRING) * var_count;
    slap_array = (PSLAP_STRING_ARRAY)AnscAllocateMemory(total_size);

    if ( slap_array )
    {
        slap_array->Size     = total_size;
        slap_array->VarCount = var_count;
        slap_array->Syntax   = SLAP_VAR_SYNTAX_stringArray;

        for ( jj = 0; jj < var_count; jj++ )
        {
            slap_array->Array.arrayString[jj] =
                (SLAP_STRING)AnscCloneString(var_point);
            var_point                        += AnscSizeOfString(var_point) + 1;
        }
    }

    return  (ANSC_HANDLE)slap_array;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapVhoAllocUcharArray
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      var_data,
                ULONG                       var_length
            );

    description:

        This function is called to allocate a variable array.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      var_data
                Specifies the raw data of the array.

                ULONG                       var_length
                Specifies the length of the raw data.

    return:     variable array.

**********************************************************************/

ANSC_HANDLE
SlapVhoAllocUcharArray
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      var_data,
        ULONG                       var_length
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_UCHAR_ARRAY               slap_array = NULL;
    ULONG                           var_count  = var_length / sizeof(SLAP_UCHAR);
    ULONG                           total_size = 0;
    char*                           var_point  = (char*)var_data;
    ULONG                           jj         = 0;

    total_size = sizeof(SLAP_VAR_ARRAY) + sizeof(SLAP_UCHAR) * var_count;
    slap_array = (PSLAP_UCHAR_ARRAY)AnscAllocateMemory(total_size);

    if ( slap_array )
    {
        slap_array->Size     = total_size;
        slap_array->VarCount = var_count;
        slap_array->Syntax   = SLAP_VAR_SYNTAX_ucharArray;

        if ( var_data )
        {
            for ( jj = 0; jj < var_count; jj++ )
            {
                slap_array->Array.arrayUchar[jj] = (SLAP_UCHAR)*var_point;
                var_point                       += sizeof(SLAP_UCHAR);
            }
        }
    }

    return  (ANSC_HANDLE)slap_array;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapVhoAllocUcharArrayFromImcpVar
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      imcp_var_data,
                ULONG                       imcp_var_length
            );

    description:

        This function is called to allocate a variable array from an
        IMCP variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      imcp_var_data
                Specifies the raw IMCP data of the array.

                ULONG                       imcp_var_length
                Specifies the length of the raw IMCP data.

    return:     variable array.

**********************************************************************/

ANSC_HANDLE
SlapVhoAllocUcharArrayFromImcpVar
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      imcp_var_data,
        ULONG                       imcp_var_length
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_UCHAR_ARRAY               slap_array = NULL;
    ULONG                           var_count  = imcp_var_length / sizeof(SLAP_UCHAR);
    ULONG                           total_size = 0;
    char*                           var_point  = (char*)imcp_var_data;
    ULONG                           jj         = 0;

    total_size = sizeof(SLAP_VAR_ARRAY) + sizeof(SLAP_UCHAR) * var_count;
    slap_array = (PSLAP_UCHAR_ARRAY)AnscAllocateMemory(total_size);

    if ( slap_array )
    {
        slap_array->Size     = total_size;
        slap_array->VarCount = var_count;
        slap_array->Syntax   = SLAP_VAR_SYNTAX_ucharArray;

        for ( jj = 0; jj < var_count; jj++ )
        {
            slap_array->Array.arrayUchar[jj] = (SLAP_UCHAR)*var_point;
            var_point                       += sizeof(SLAP_UCHAR);
        }
    }

    return  (ANSC_HANDLE)slap_array;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapVhoAllocUint32Array
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      var_data,
                ULONG                       var_length
            );

    description:

        This function is called to allocate a variable array.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      var_data
                Specifies the raw data of the array.

                ULONG                       var_length
                Specifies the length of the raw data.

    return:     variable array.

**********************************************************************/

ANSC_HANDLE
SlapVhoAllocUint32Array
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      var_data,
        ULONG                       var_length
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_UINT32_ARRAY              slap_array = NULL;
    ULONG                           var_count  = var_length / sizeof(ULONG);
    ULONG                           total_size = 0;
    char*                           var_point  = (char*)var_data;
    ULONG                           jj         = 0;

    total_size = sizeof(SLAP_VAR_ARRAY) + sizeof(SLAP_UINT32) * var_count;
    slap_array = (PSLAP_UINT32_ARRAY)AnscAllocateMemory(total_size);

    if ( slap_array )
    {
        slap_array->Size     = total_size;
        slap_array->VarCount = var_count;
        slap_array->Syntax   = SLAP_VAR_SYNTAX_uint32Array;

        if ( var_data )
        {
            for ( jj = 0; jj < var_count; jj++ )
            {
                slap_array->Array.arrayUint32[jj] =
                    (SLAP_UINT32)AnscReadUlong(var_point);
                var_point                        += sizeof(SLAP_UINT32);
            }
        }
    }

    return  (ANSC_HANDLE)slap_array;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapVhoAllocUint32ArrayFromImcpVar
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      imcp_var_data,
                ULONG                       imcp_var_length
            );

    description:

        This function is called to allocate a variable array from an
        IMCP variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      imcp_var_data
                Specifies the raw IMCP data of the array.

                ULONG                       imcp_var_length
                Specifies the length of the raw IMCP data.

    return:     variable array.

**********************************************************************/

ANSC_HANDLE
SlapVhoAllocUint32ArrayFromImcpVar
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      imcp_var_data,
        ULONG                       imcp_var_length
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_UINT32_ARRAY              slap_array = NULL;
    ULONG                           var_count  = imcp_var_length / sizeof(ULONG);
    ULONG                           total_size = 0;
    char*                           var_point  = (char*)imcp_var_data;
    ULONG                           jj         = 0;

    total_size = sizeof(SLAP_VAR_ARRAY) + sizeof(SLAP_UINT32) * var_count;
    slap_array = (PSLAP_UINT32_ARRAY)AnscAllocateMemory(total_size);

    if ( slap_array )
    {
        slap_array->Size     = total_size;
        slap_array->VarCount = var_count;
        slap_array->Syntax   = SLAP_VAR_SYNTAX_uint32Array;

        for ( jj = 0; jj < var_count; jj++ )
        {
            slap_array->Array.arrayUint32[jj] =
                (SLAP_UINT32)AnscUlongFromNToH(AnscReadUlong(var_point));
            var_point                        += sizeof(ULONG);
        }
    }

    return  (ANSC_HANDLE)slap_array;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapVhoAllocPtrArray
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      var_data,
                ULONG                       var_length
            );

    description:

        This function is called to allocate a variable array.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      var_data
                Specifies the raw data of the array.

                ULONG                       var_length
                Specifies the length of the raw data.

    return:     variable array.

**********************************************************************/

ANSC_HANDLE
SlapVhoAllocPtrArray
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      var_data,
        ULONG                       var_length
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_PTR_ARRAY                 slap_array = NULL;
    ULONG                           var_count  = var_length / sizeof(ULONG);
    ULONG                           total_size = 0;
    char*                           var_point  = (char*)var_data;
    ULONG                           jj         = 0;

    total_size = sizeof(SLAP_VAR_ARRAY) + sizeof(SLAP_PTR) * var_count;
    slap_array = (PSLAP_PTR_ARRAY)AnscAllocateMemory(total_size);

    if ( slap_array )
    {
        slap_array->Size     = total_size;
        slap_array->VarCount = var_count;
        slap_array->Syntax   = SLAP_VAR_SYNTAX_ptrArray;

        if ( var_data )
        {
            for ( jj = 0; jj < var_count; jj++ )
            {
                slap_array->Array.arrayPtr[jj] =
                    (SLAP_PTR)AnscReadUlong(var_point);
                var_point                     += sizeof(SLAP_PTR);
            }
        }
    }

    return  (ANSC_HANDLE)slap_array;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapVhoAllocPtrArrayFromImcpVar
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      imcp_var_data,
                ULONG                       imcp_var_length
            );

    description:

        This function is called to allocate a variable array from an
        IMCP variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      imcp_var_data
                Specifies the raw IMCP data of the array.

                ULONG                       imcp_var_length
                Specifies the length of the raw IMCP data.

    return:     variable array.

**********************************************************************/

ANSC_HANDLE
SlapVhoAllocPtrArrayFromImcpVar
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      imcp_var_data,
        ULONG                       imcp_var_length
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_PTR_ARRAY                 slap_array = NULL;
    ULONG                           var_count  = imcp_var_length / sizeof(ULONG);
    ULONG                           total_size = 0;
    char*                           var_point  = (char*)imcp_var_data;
    ULONG                           jj         = 0;

    total_size = sizeof(SLAP_VAR_ARRAY) + sizeof(SLAP_PTR) * var_count;
    slap_array = (PSLAP_PTR_ARRAY)AnscAllocateMemory(total_size);

    if ( slap_array )
    {
        slap_array->Size     = total_size;
        slap_array->VarCount = var_count;
        slap_array->Syntax   = SLAP_VAR_SYNTAX_ptrArray;

        for ( jj = 0; jj < var_count; jj++ )
        {
            slap_array->Array.arrayPtr[jj] =
                (SLAP_PTR)AnscUlongFromNToH(AnscReadUlong(var_point));
            var_point                     += sizeof(ULONG);
        }
    }

    return  (ANSC_HANDLE)slap_array;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapVhoAllocHandleArray
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      var_data,
                ULONG                       var_length
            );

    description:

        This function is called to allocate a variable array.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      var_data
                Specifies the raw data of the array.

                ULONG                       var_length
                Specifies the length of the raw data.

    return:     variable array.

**********************************************************************/

ANSC_HANDLE
SlapVhoAllocHandleArray
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      var_data,
        ULONG                       var_length
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_HANDLE_ARRAY              slap_array = NULL;
    ULONG                           var_count  = var_length / sizeof(ULONG);
    ULONG                           total_size = 0;
    char*                           var_point  = (char*)var_data;
    ULONG                           jj         = 0;

    total_size = sizeof(SLAP_VAR_ARRAY) + sizeof(SLAP_HANDLE) * var_count;
    slap_array = (PSLAP_HANDLE_ARRAY)AnscAllocateMemory(total_size);

    if ( slap_array )
    {
        slap_array->Size     = total_size;
        slap_array->VarCount = var_count;
        slap_array->Syntax   = SLAP_VAR_SYNTAX_handleArray;

        if ( var_data )
        {
            for ( jj = 0; jj < var_count; jj++ )
            {
                slap_array->Array.arrayHandle[jj] =
                    (SLAP_HANDLE)AnscReadUlong(var_point);
                var_point                        += sizeof(SLAP_HANDLE);
            }
        }
    }

    return  (ANSC_HANDLE)slap_array;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapVhoAllocHandleArrayFromImcpVar
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      imcp_var_data,
                ULONG                       imcp_var_length
            );

    description:

        This function is called to allocate a variable array from an
        IMCP variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      imcp_var_data
                Specifies the raw IMCP data of the array.

                ULONG                       imcp_var_length
                Specifies the length of the raw IMCP data.

    return:     variable array.

**********************************************************************/

ANSC_HANDLE
SlapVhoAllocHandleArrayFromImcpVar
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      imcp_var_data,
        ULONG                       imcp_var_length
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_HANDLE_ARRAY              slap_array = NULL;
    ULONG                           var_count  = imcp_var_length / sizeof(ULONG);
    ULONG                           total_size = 0;
    char*                           var_point  = (char*)imcp_var_data;
    ULONG                           jj         = 0;

    total_size = sizeof(SLAP_VAR_ARRAY) + sizeof(SLAP_HANDLE) * var_count;
    slap_array = (PSLAP_HANDLE_ARRAY)AnscAllocateMemory(total_size);

    if ( slap_array )
    {
        slap_array->Size     = total_size;
        slap_array->VarCount = var_count;
        slap_array->Syntax   = SLAP_VAR_SYNTAX_handleArray;

        for ( jj = 0; jj < var_count; jj++ )
        {
            slap_array->Array.arrayHandle[jj] =
                (SLAP_HANDLE)AnscUlongFromNToH(AnscReadUlong(var_point));
            var_point                        += sizeof(ULONG);
        }
    }

    return  (ANSC_HANDLE)slap_array;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapVhoAllocObjectArray
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      var_data,
                ULONG                       var_length
            );

    description:

        This function is called to allocate a variable array.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      var_data
                Specifies the raw data of the array.

                ULONG                       var_length
                Specifies the length of the raw data.

    return:     variable array.

**********************************************************************/

ANSC_HANDLE
SlapVhoAllocObjectArray
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      var_data,
        ULONG                       var_length
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_OBJECT_ARRAY              slap_array = NULL;
    ULONG                           var_count  = var_length / sizeof(ULONG);
    ULONG                           total_size = 0;
    char*                           var_point  = (char*)var_data;
    ULONG                           jj         = 0;

    total_size = sizeof(SLAP_VAR_ARRAY) + sizeof(SLAP_OBJECT) * var_count;
    slap_array = (PSLAP_OBJECT_ARRAY)AnscAllocateMemory(total_size);

    if ( slap_array )
    {
        slap_array->Size     = total_size;
        slap_array->VarCount = var_count;
        slap_array->Syntax   = SLAP_VAR_SYNTAX_objectArray;

        if ( var_data )
        {
            for ( jj = 0; jj < var_count; jj++ )
            {
                slap_array->Array.arrayObject[jj] =
                    (SLAP_OBJECT)AnscReadUlong(var_point);
                var_point                        += sizeof(SLAP_OBJECT);
            }
        }
    }

    return  (ANSC_HANDLE)slap_array;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapVhoAllocObjectArrayFromImcpVar
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      imcp_var_data,
                ULONG                       imcp_var_length
            );

    description:

        This function is called to allocate a variable array from an
        IMCP variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      imcp_var_data
                Specifies the raw IMCP data of the array.

                ULONG                       imcp_var_length
                Specifies the length of the raw IMCP data.

    return:     variable array.

**********************************************************************/

ANSC_HANDLE
SlapVhoAllocObjectArrayFromImcpVar
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      imcp_var_data,
        ULONG                       imcp_var_length
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_OBJECT_ARRAY              slap_array = NULL;
    ULONG                           var_count  = imcp_var_length / sizeof(ULONG);
    ULONG                           total_size = 0;
    char*                           var_point  = (char*)imcp_var_data;
    ULONG                           jj         = 0;

    total_size = sizeof(SLAP_VAR_ARRAY) + sizeof(SLAP_OBJECT) * var_count;
    slap_array = (PSLAP_OBJECT_ARRAY)AnscAllocateMemory(total_size);

    if ( slap_array )
    {
        slap_array->Size     = total_size;
        slap_array->VarCount = var_count;
        slap_array->Syntax   = SLAP_VAR_SYNTAX_objectArray;

        for ( jj = 0; jj < var_count; jj++ )
        {
            slap_array->Array.arrayObject[jj] =
                (SLAP_OBJECT)AnscUlongFromNToH(AnscReadUlong(var_point));
            var_point                        += sizeof(ULONG);
        }
    }

    return  (ANSC_HANDLE)slap_array;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        SlapVhoEqualVarArrays
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 slap_array1,
                ANSC_HANDLE                 slap_array2
            );

    description:

        This function is called to compare variable arrays.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 slap_array1
                Specifies the first array to be examined.

                ANSC_HANDLE                 slap_array2
                Specifies the second array to be examined.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
SlapVhoEqualVarArrays
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 slap_array1,
        ANSC_HANDLE                 slap_array2
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_VAR_ARRAY                 array1        = (PSLAP_VAR_ARRAY)slap_array1;
    PSLAP_VAR_ARRAY                 array2        = (PSLAP_VAR_ARRAY)slap_array2;
    BOOL                            b_equal_array = TRUE;
    ULONG                           jj            = 0;

    b_equal_array = TRUE;

    do
    {
        if ( (array1->Size     != array2->Size    ) ||
             (array1->VarCount != array2->VarCount) ||
             (array1->Syntax   != array2->Syntax  ) )
        {
            b_equal_array = FALSE;

            break;
        }

        for ( jj = 0; jj < array1->VarCount; jj++ )
        {
            switch ( array1->Syntax )
            {
                case    SLAP_VAR_SYNTAX_boolArray :

                        b_equal_array =
                            (array1->Array.arrayBool[jj] ==
                             array2->Array.arrayBool[jj]);

                        break;

                case    SLAP_VAR_SYNTAX_charArray :

                        b_equal_array =
                            (array1->Array.arrayChar[jj] ==
                             array2->Array.arrayChar[jj]);

                        break;

                case    SLAP_VAR_SYNTAX_intArray :

                        b_equal_array =
                            (array1->Array.arrayInt[jj] ==
                             array2->Array.arrayInt[jj]);

                        break;

                case    SLAP_VAR_SYNTAX_stringArray :

                        if ( array1->Array.arrayString[jj] &&
                             array2->Array.arrayString[jj] )
                        {
                            b_equal_array =
                                strcmp(array1->Array.arrayString[jj],array2->Array.arrayString[jj]) == 0 ;
                        }
                        else if ( !array1->Array.arrayString[jj] &&
                                  !array2->Array.arrayString[jj] )
                        {
                            b_equal_array = TRUE;
                        }
                        else
                        {
                            b_equal_array = FALSE;
                        }

                        break;

                case    SLAP_VAR_SYNTAX_ucharArray :

                        b_equal_array =
                            (array1->Array.arrayUchar[jj] ==
                             array2->Array.arrayUchar[jj]);

                        break;

                case    SLAP_VAR_SYNTAX_uint32Array :

                        b_equal_array =
                            (array1->Array.arrayUint32[jj] ==
                             array2->Array.arrayUint32[jj]);

                        break;

                case    SLAP_VAR_SYNTAX_ptrArray :

                        b_equal_array =
                            (array1->Array.arrayPtr[jj] ==
                             array2->Array.arrayPtr[jj]);

                        break;

                case    SLAP_VAR_SYNTAX_handleArray :

                        b_equal_array =
                            (array1->Array.arrayHandle[jj] ==
                             array2->Array.arrayHandle[jj]);

                        break;

                case    SLAP_VAR_SYNTAX_objectArray :

                        b_equal_array =
                            (array1->Array.arrayObject[jj] ==
                             array2->Array.arrayObject[jj]);

                        break;

                default :

                        b_equal_array = FALSE;

                        break;
            }

            if ( !b_equal_array )
            {
                break;
            }
        }

        break;
    }
    while ( FALSE );

    return  b_equal_array;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapVhoCloneVarArray
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 slap_array
            );

    description:

        This function is called to clone a variable array.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 slap_array
                Specifies the array to be cloned.

    return:     cloned array.

**********************************************************************/

ANSC_HANDLE
SlapVhoCloneVarArray
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 slap_array
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_VAR_ARRAY                 src_array = (PSLAP_VAR_ARRAY)slap_array;
    PSLAP_VAR_ARRAY                 dst_array = (PSLAP_VAR_ARRAY)NULL;
    ULONG                           kk        = 0;

    dst_array = (PSLAP_VAR_ARRAY)AnscAllocateMemory(src_array->Size);

    if ( dst_array )
    {
        dst_array->Size     = src_array->Size;
        dst_array->VarCount = src_array->VarCount;
        dst_array->Syntax   = src_array->Syntax;

        if ( src_array->Syntax == SLAP_VAR_SYNTAX_stringArray )
        {
            for ( kk = 0; kk < src_array->VarCount; kk++ )
            {
                dst_array->Array.arrayString[kk] =
                    AnscCloneString(src_array->Array.arrayString[kk]);
            }
        }
        else
        {
            AnscCopyMemory(dst_array, src_array, src_array->Size);
        }
    }

    return  (ANSC_HANDLE)dst_array;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        SlapVhoCleanVarArray
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 slap_array
            );

    description:

        This function is called to clean a variable array.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 slap_array
                Specifies the array to be clean.

    return:     none.

**********************************************************************/

void
SlapVhoCleanVarArray
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 slap_array
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_VAR_ARRAY                 tbc_array = (PSLAP_VAR_ARRAY)slap_array;
    ULONG                           kk        = 0;

    if ( tbc_array->Syntax == SLAP_VAR_SYNTAX_stringArray )
    {
        for ( kk = 0; kk < tbc_array->VarCount; kk++ )
        {
            if ( tbc_array->Array.arrayString[kk] )
            {
                AnscFreeMemory(tbc_array->Array.arrayString[kk]);

                tbc_array->Array.arrayString[kk] = NULL;
            }
        }
    }

    return;
}
