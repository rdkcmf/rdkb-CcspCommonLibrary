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

    module:	slap_vho_conversion.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced variable conversion
        functions of the Slap Var Helper Object.

        *   SlapVhoImcpGetVarSize
        *   SlapVhoConvFromImcpVar
        *   SlapVhoConvToImcpVar
        *   SlapVhoConvFromSysRecordType
        *   SlapVhoConvToSysRecordType
        *   SlapVhoConvFromMoamType
        *   SlapVhoConvFromMib2Syntax
        *   SlapVhoConvFromMib2Var
        *   SlapVhoConvToMib2Var

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/26/05    initial revision.

**********************************************************************/


#include "slap_vho_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SlapVhoImcpGetVarSize
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 variable
            );

    description:

        This function is called to retrieve the variable size.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 variable
                Specifies the raw data of the array.

    return:     variable size.

**********************************************************************/

ULONG
SlapVhoImcpGetVarSize
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 variable
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_VARIABLE                  slap_var              = (PSLAP_VARIABLE)variable;
    ULONG                           imcp_var_size         = 0;
    ULONG                           ii                    = 0;
    SLAP_BOOL_ARRAY*                tmp_slap_bool_array   = NULL;
    SLAP_CHAR_ARRAY*                tmp_slap_char_array   = NULL;
    SLAP_INT_ARRAY*                 tmp_slap_int_array    = NULL;
    SLAP_STRING_ARRAY*              tmp_slap_string_array = NULL;
    SLAP_UCHAR_ARRAY*               tmp_slap_uchar_array  = NULL;
    SLAP_UINT32_ARRAY*              tmp_slap_uint32_array = NULL;
    SLAP_PTR_ARRAY*                 tmp_slap_ptr_array    = NULL;
    SLAP_HANDLE_ARRAY*              tmp_slap_handle_array = NULL;
    SLAP_OBJECT_ARRAY*              tmp_slap_object_array = NULL;
    SLAP_STRING                     tmp_slap_string       = NULL;

    imcp_var_size  = 0;
    imcp_var_size += sizeof(IMCP_SLAP_VARIABLE);

    switch ( slap_var->Syntax )
    {
        case    SLAP_VAR_SYNTAX_bool :

                imcp_var_size += sizeof(SLAP_BOOL);

                break;

        case    SLAP_VAR_SYNTAX_boolArray :

                if ( slap_var->Variant.varBoolArray )
                {
                    tmp_slap_bool_array = slap_var->Variant.varBoolArray;
                    imcp_var_size      +=
                        sizeof(SLAP_BOOL) * tmp_slap_bool_array->VarCount;
                }

                break;

        case    SLAP_VAR_SYNTAX_charArray :

                if ( slap_var->Variant.varCharArray )
                {
                    tmp_slap_char_array = slap_var->Variant.varCharArray;
                    imcp_var_size      +=
                        sizeof(SLAP_CHAR) * tmp_slap_char_array->VarCount;
                }

                break;

        case    SLAP_VAR_SYNTAX_int :

                imcp_var_size += sizeof(SLAP_INT);

                break;

        case    SLAP_VAR_SYNTAX_intArray :

                if ( slap_var->Variant.varIntArray )
                {
                    tmp_slap_int_array = slap_var->Variant.varIntArray;
                    imcp_var_size     +=
                        sizeof(SLAP_INT) * tmp_slap_int_array->VarCount;
                }

                break;

        case    SLAP_VAR_SYNTAX_string :

                if ( slap_var->Variant.varString )
                {
                    imcp_var_size += AnscSizeOfString(slap_var->Variant.varString);
                    imcp_var_size += 1;
                }

                break;

        case    SLAP_VAR_SYNTAX_stringArray :

                if ( slap_var->Variant.varStringArray )
                {
                    tmp_slap_string_array = slap_var->Variant.varStringArray;

                    for ( ii = 0; ii < tmp_slap_string_array->VarCount; ii++ )
                    {
                        tmp_slap_string =
                            tmp_slap_string_array->Array.arrayString[ii];

                        if ( tmp_slap_string )
                        {
                            imcp_var_size += AnscSizeOfString(tmp_slap_string) + 1;
                        }
                        else
                        {
                            imcp_var_size += 1;
                        }
                    }
                }

                break;

        case    SLAP_VAR_SYNTAX_ucharArray :

                if ( slap_var->Variant.varUcharArray )
                {
                    tmp_slap_uchar_array = slap_var->Variant.varUcharArray;
                    imcp_var_size       +=
                        sizeof(SLAP_UCHAR) * tmp_slap_uchar_array->VarCount;
                }

                break;

        case    SLAP_VAR_SYNTAX_uint32 :

                imcp_var_size += sizeof(SLAP_UINT32);

                break;

        case    SLAP_VAR_SYNTAX_uint32Array :

                if ( slap_var->Variant.varUint32Array )
                {
                    tmp_slap_uint32_array = slap_var->Variant.varUint32Array;
                    imcp_var_size        +=
                        sizeof(SLAP_UINT32) * tmp_slap_uint32_array->VarCount;
                }

                break;

        case    SLAP_VAR_SYNTAX_ptr :

                imcp_var_size += sizeof(SLAP_PTR);

                break;

        case    SLAP_VAR_SYNTAX_ptrArray :

                if ( slap_var->Variant.varPtrArray )
                {
                    tmp_slap_ptr_array = slap_var->Variant.varPtrArray;
                    imcp_var_size     +=
                        sizeof(SLAP_PTR) * tmp_slap_ptr_array->VarCount;
                }

                break;

        case    SLAP_VAR_SYNTAX_handle :

                imcp_var_size += sizeof(SLAP_HANDLE);

                break;

        case    SLAP_VAR_SYNTAX_handleArray :

                if ( slap_var->Variant.varHandleArray )
                {
                    tmp_slap_handle_array = slap_var->Variant.varHandleArray;
                    imcp_var_size        +=
                        sizeof(SLAP_HANDLE) * tmp_slap_handle_array->VarCount;
                }

                break;

        case    SLAP_VAR_SYNTAX_object :

                imcp_var_size += sizeof(SLAP_OBJECT);

                break;

        case    SLAP_VAR_SYNTAX_objectArray :

                if ( slap_var->Variant.varObjectArray )
                {
                    tmp_slap_object_array = slap_var->Variant.varObjectArray;
                    imcp_var_size        +=
                        sizeof(SLAP_OBJECT) * tmp_slap_object_array->VarCount;
                }

                break;

        default :

                break;
    }

    return  imcp_var_size;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        SlapVhoConvFromImcpVar
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 imcp_variable,
                ANSC_HANDLE                 slap_variable
            );

    description:

        This function is called to convert an IMCP variable to a SLAP
        variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 imcp_variable
                Specifies the IMCP variable to be converted.

                ANSC_HANDLE                 slap_variable
                Specifies the SLAP variable to be filled.

    return:     none.

**********************************************************************/

void
SlapVhoConvFromImcpVar
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 imcp_variable,
        ANSC_HANDLE                 slap_variable
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PIMCP_SLAP_VARIABLE             imcp_var     = (PIMCP_SLAP_VARIABLE)imcp_variable;
    PSLAP_VARIABLE                  slap_var     = (PSLAP_VARIABLE     )slap_variable;
    PIMCP_SLAP_VAR_OBJECT           imcp_var_obj = NULL;

    SlapInitVariable(slap_var);

    slap_var->Name        = NULL;
    slap_var->ContentType = ImcpSlapVarGetContentType(imcp_var);
    slap_var->UsageType   = 0;
    slap_var->Syntax      = ImcpSlapVarGetSyntax(imcp_var);

    if ( ImcpSlapVarGetSyntax(imcp_var) == SLAP_VAR_SYNTAX_object )
    {
        if ( (ImcpSlapVarGetFlags(imcp_var) & IMCP_SLAP_VAR_FLAG_objType) &&
             (ImcpSlapVarGetFlags(imcp_var) & IMCP_SLAP_VAR_FLAG_objName) )
        {
            imcp_var_obj   = (PIMCP_SLAP_VAR_OBJECT)ImcpSlapVarGetData(imcp_var);
            slap_var->Name = AnscCloneString(ImcpSlapVarObjGetName(imcp_var_obj));
        }
    }

    switch ( slap_var->Syntax )
    {
        case    SLAP_VAR_SYNTAX_bool :

                slap_var->Variant.varBool =
                    AnscUlongFromNToH(AnscReadUlong(imcp_var->Data));

                break;

        case    SLAP_VAR_SYNTAX_boolArray :

                SlapAllocBoolArrayFromImcpVar
                    (
                        ImcpSlapVarGetData  (imcp_var),
                        ImcpSlapVarGetLength(imcp_var),
                        slap_var->Variant.varBoolArray
                    );

                break;

        case    SLAP_VAR_SYNTAX_charArray :

                SlapAllocCharArrayFromImcpVar
                    (
                        ImcpSlapVarGetData  (imcp_var),
                        ImcpSlapVarGetLength(imcp_var),
                        slap_var->Variant.varCharArray
                    );

                break;

        case    SLAP_VAR_SYNTAX_int :

                slap_var->Variant.varInt =
                    (SLAP_INT)AnscUlongFromNToH(AnscReadUlong(imcp_var->Data));

                break;

        case    SLAP_VAR_SYNTAX_intArray :

                SlapAllocIntArrayFromImcpVar
                    (
                        ImcpSlapVarGetData  (imcp_var),
                        ImcpSlapVarGetLength(imcp_var),
                        slap_var->Variant.varIntArray
                    );

                break;

        case    SLAP_VAR_SYNTAX_string :

                if ( ImcpSlapVarGetLength(imcp_var) == 0 )
                {
                    slap_var->Variant.varString = NULL;
                }
                else
                {
                    slap_var->Variant.varString =
                        (SLAP_STRING)AnscCloneString((char *)imcp_var->Data);
                };

                break;

        case    SLAP_VAR_SYNTAX_stringArray :

                SlapAllocStringArrayFromImcpVar
                    (
                        ImcpSlapVarGetData  (imcp_var),
                        ImcpSlapVarGetLength(imcp_var),
                        slap_var->Variant.varStringArray
                    );

                break;

        case    SLAP_VAR_SYNTAX_ucharArray :

                SlapAllocUcharArrayFromImcpVar
                    (
                        ImcpSlapVarGetData  (imcp_var),
                        ImcpSlapVarGetLength(imcp_var),
                        slap_var->Variant.varUcharArray
                    );

                break;

        case    SLAP_VAR_SYNTAX_uint32 :

                slap_var->Variant.varUint32 =
                    (SLAP_UINT32)AnscUlongFromNToH(AnscReadUlong(imcp_var->Data));

                break;

        case    SLAP_VAR_SYNTAX_uint32Array :

                SlapAllocUint32ArrayFromImcpVar
                    (
                        ImcpSlapVarGetData  (imcp_var),
                        ImcpSlapVarGetLength(imcp_var),
                        slap_var->Variant.varUint32Array
                    );

                break;

        case    SLAP_VAR_SYNTAX_ptr :

                slap_var->Variant.varPtr =
                    (SLAP_PTR)AnscUlongFromNToH(AnscReadUlong(imcp_var->Data));

                break;

        case    SLAP_VAR_SYNTAX_ptrArray :

                SlapAllocPtrArrayFromImcpVar
                    (
                        ImcpSlapVarGetData  (imcp_var),
                        ImcpSlapVarGetLength(imcp_var),
                        slap_var->Variant.varPtrArray
                    );

                break;

        case    SLAP_VAR_SYNTAX_handle :

                slap_var->Variant.varHandle =
                    (SLAP_HANDLE)AnscUlongFromNToH(AnscReadUlong(imcp_var->Data));

                break;

        case    SLAP_VAR_SYNTAX_handleArray :

                SlapAllocHandleArrayFromImcpVar
                    (
                        ImcpSlapVarGetData  (imcp_var),
                        ImcpSlapVarGetLength(imcp_var),
                        slap_var->Variant.varHandleArray
                    );

                break;

        case    SLAP_VAR_SYNTAX_object :

                slap_var->Variant.varObject =
                    (SLAP_OBJECT)AnscUlongFromNToH(AnscReadUlong(imcp_var->Data));

                break;

        case    SLAP_VAR_SYNTAX_objectArray :

                SlapAllocObjectArrayFromImcpVar
                    (
                        ImcpSlapVarGetData  (imcp_var),
                        ImcpSlapVarGetLength(imcp_var),
                        slap_var->Variant.varObjectArray
                    );

                break;

        default :

                break;
    }

    return;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        SlapVhoConvToImcpVar
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 slap_variable,
                ANSC_HANDLE                 imcp_variable
            );

    description:

        This function is called to convert a SLAP variable to an IMCP
        variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 slap_variable
                Specifies the SLAP variable to be converted.

                ANSC_HANDLE                 imcp_variable
                Specifies the IMCP variable to be filled.

    return:     none.

**********************************************************************/

void
SlapVhoConvToImcpVar
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 slap_variable,
        ANSC_HANDLE                 imcp_variable
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_VARIABLE                  slap_var              = (PSLAP_VARIABLE     )slap_variable;
    PIMCP_SLAP_VARIABLE             imcp_var              = (PIMCP_SLAP_VARIABLE)imcp_variable;
    ULONG                           ii                    = 0;
    SLAP_BOOL_ARRAY*                tmp_slap_bool_array   = NULL;
    SLAP_CHAR_ARRAY*                tmp_slap_char_array   = NULL;
    SLAP_INT_ARRAY*                 tmp_slap_int_array    = NULL;
    SLAP_STRING_ARRAY*              tmp_slap_string_array = NULL;
    SLAP_UCHAR_ARRAY*               tmp_slap_uchar_array  = NULL;
    SLAP_UINT32_ARRAY*              tmp_slap_uint32_array = NULL;
    SLAP_PTR_ARRAY*                 tmp_slap_ptr_array    = NULL;
    SLAP_HANDLE_ARRAY*              tmp_slap_handle_array = NULL;
    SLAP_OBJECT_ARRAY*              tmp_slap_object_array = NULL;
    SLAP_STRING                     tmp_slap_string       = NULL;

    ImcpSlapVarSetContentType(imcp_var, slap_var->ContentType);
    ImcpSlapVarSetSyntax     (imcp_var, slap_var->Syntax     );
    ImcpSlapVarSetLength     (imcp_var, 0                    );
    ImcpSlapVarSetFlags      (imcp_var, 0                    );

    switch ( slap_var->Syntax )
    {
        case    SLAP_VAR_SYNTAX_bool :

                ImcpSlapVarAppendUlong(imcp_var, slap_var->Variant.varBool);

                break;

        case    SLAP_VAR_SYNTAX_boolArray :

                if ( slap_var->Variant.varBoolArray )
                {
                    tmp_slap_bool_array = slap_var->Variant.varBoolArray;

                    for ( ii = 0; ii < tmp_slap_bool_array->VarCount; ii++ )
                    {
                        ImcpSlapVarAppendUlong
                            (
                                imcp_var,
                                tmp_slap_bool_array->Array.arrayBool[ii]
                            );
                    }
                }

                break;

        case    SLAP_VAR_SYNTAX_charArray :

                if ( slap_var->Variant.varCharArray )
                {
                    tmp_slap_char_array = slap_var->Variant.varCharArray;

                    for ( ii = 0; ii < tmp_slap_char_array->VarCount; ii++ )
                    {
                        ImcpSlapVarAppendUchar
                            (
                                imcp_var,
                                tmp_slap_char_array->Array.arrayChar[ii]
                            );
                    }
                }

                break;

        case    SLAP_VAR_SYNTAX_int :

                ImcpSlapVarAppendUlong(imcp_var, (ULONG)slap_var->Variant.varInt);

                break;

        case    SLAP_VAR_SYNTAX_intArray :

                if ( slap_var->Variant.varIntArray )
                {
                    tmp_slap_int_array = slap_var->Variant.varIntArray;

                    for ( ii = 0; ii < tmp_slap_int_array->VarCount; ii++ )
                    {
                        ImcpSlapVarAppendUlong
                            (
                                imcp_var,
                                (ULONG)tmp_slap_int_array->Array.arrayInt[ii]
                            );
                    }
                }

                break;

        case    SLAP_VAR_SYNTAX_string :

                if ( slap_var->Variant.varString )
                {
                    ImcpSlapVarAppendString(imcp_var, slap_var->Variant.varString);
                }

                break;

        case    SLAP_VAR_SYNTAX_stringArray :

                if ( slap_var->Variant.varStringArray )
                {
                    tmp_slap_string_array = slap_var->Variant.varStringArray;

                    for ( ii = 0; ii < tmp_slap_string_array->VarCount; ii++ )
                    {
                        tmp_slap_string =
                            tmp_slap_string_array->Array.arrayString[ii];

                        if ( tmp_slap_string )
                        {
                            ImcpSlapVarAppendString(imcp_var, tmp_slap_string);
                        }
                        else
                        {
                            ImcpSlapVarAppendUchar(imcp_var, 0);
                        }
                    }
                }

                break;

        case    SLAP_VAR_SYNTAX_ucharArray :

                if ( slap_var->Variant.varUcharArray )
                {
                    tmp_slap_uchar_array = slap_var->Variant.varUcharArray;

                    for ( ii = 0; ii < tmp_slap_uchar_array->VarCount; ii++ )
                    {
                        ImcpSlapVarAppendUchar
                            (
                                imcp_var,
                                tmp_slap_uchar_array->Array.arrayUchar[ii]
                            );
                    }
                }

                break;

        case    SLAP_VAR_SYNTAX_uint32 :

                ImcpSlapVarAppendUlong(imcp_var, slap_var->Variant.varUint32);

                break;

        case    SLAP_VAR_SYNTAX_uint32Array :

                if ( slap_var->Variant.varUint32Array )
                {
                    tmp_slap_uint32_array = slap_var->Variant.varUint32Array;

                    for ( ii = 0; ii < tmp_slap_uint32_array->VarCount; ii++ )
                    {
                        ImcpSlapVarAppendUlong
                            (
                                imcp_var,
                                tmp_slap_uint32_array->Array.arrayUint32[ii]
                            );
                    }
                }

                break;

        case    SLAP_VAR_SYNTAX_ptr :

                ImcpSlapVarAppendUlong
                    (
                        imcp_var,
                        (ULONG)slap_var->Variant.varPtr
                    );

                break;

        case    SLAP_VAR_SYNTAX_ptrArray :

                if ( slap_var->Variant.varPtrArray )
                {
                    tmp_slap_ptr_array = slap_var->Variant.varPtrArray;

                    for ( ii = 0; ii < tmp_slap_ptr_array->VarCount; ii++ )
                    {
                        ImcpSlapVarAppendUlong
                            (
                                imcp_var,
                                (ULONG)tmp_slap_ptr_array->Array.arrayPtr[ii]
                            );
                    }
                }

                break;

        case    SLAP_VAR_SYNTAX_handle :

                ImcpSlapVarAppendUlong
                    (
                        imcp_var,
                        (ULONG)slap_var->Variant.varHandle
                    );

                break;

        case    SLAP_VAR_SYNTAX_handleArray :

                if ( slap_var->Variant.varHandleArray )
                {
                    tmp_slap_handle_array = slap_var->Variant.varHandleArray;

                    for ( ii = 0; ii < tmp_slap_handle_array->VarCount; ii++ )
                    {
                        ImcpSlapVarAppendUlong
                            (
                                imcp_var,
                                tmp_slap_handle_array->Array.arrayHandle[ii]
                            );
                    }
                }

                break;

        case    SLAP_VAR_SYNTAX_object :

                ImcpSlapVarAppendUlong
                    (
                        imcp_var,
                        (ULONG)slap_var->Variant.varObject
                    );

                break;

        case    SLAP_VAR_SYNTAX_objectArray :

                if ( slap_var->Variant.varObjectArray )
                {
                    tmp_slap_object_array = slap_var->Variant.varObjectArray;

                    for ( ii = 0; ii < tmp_slap_object_array->VarCount; ii++ )
                    {
                        ImcpSlapVarAppendUlong
                            (
                                imcp_var,
                                tmp_slap_object_array->Array.arrayObject[ii]
                            );
                    }
                }

                break;

        default :

                break;
    }

    return;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SlapVhoConvFromSysRecordType
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       record_type
            );

    description:

        This function is called to convert the system record type to 
        SLAP object syntax.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       record_type
                Specifies the record type to be converted.

    return:     slap syntax.

**********************************************************************/

ULONG
SlapVhoConvFromSysRecordType
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       record_type
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ULONG                           slap_syntax = SLAP_VAR_SYNTAX_other;

    switch ( record_type )
    {
        case    SYS_REP_RECORD_TYPE_SINT :

                slap_syntax = SLAP_VAR_SYNTAX_int;

                break;

        case    SYS_REP_RECORD_TYPE_UINT :

                slap_syntax = SLAP_VAR_SYNTAX_uint32;

                break;

        case    SYS_REP_RECORD_TYPE_BOOL :

                slap_syntax = SLAP_VAR_SYNTAX_bool;

                break;

        case    SYS_REP_RECORD_TYPE_ASTR :

                slap_syntax = SLAP_VAR_SYNTAX_string;

                break;

        case    SYS_REP_RECORD_TYPE_BSTR :

                slap_syntax = SLAP_VAR_SYNTAX_ucharArray;

                break;

        case    SYS_REP_RECORD_TYPE_HCXT :

                slap_syntax = SLAP_VAR_SYNTAX_handle;

                break;

        case    SYS_REP_RECORD_TYPE_ENUM :

                slap_syntax = SLAP_VAR_SYNTAX_uint32;

                break;

        default :

                slap_syntax = SLAP_VAR_SYNTAX_other;

                break;
    }

    return  slap_syntax;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SlapVhoConvToSysRecordType
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       slap_syntax
            );

    description:

        This function is called to convert the SLAP object syntax to 
        system record type.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       slap_syntax
                Specifies the SLAP object syntax to be converted.

    return:     slap syntax.

**********************************************************************/

ULONG
SlapVhoConvToSysRecordType
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       slap_syntax
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ULONG                           record_type = SYS_REP_RECORD_TYPE_ASTR;

    switch ( slap_syntax )
    {
        case    SLAP_VAR_SYNTAX_bool :

                record_type = SYS_REP_RECORD_TYPE_BOOL;

                break;

        case    SLAP_VAR_SYNTAX_int :

                record_type = SYS_REP_RECORD_TYPE_SINT;

                break;

        case    SLAP_VAR_SYNTAX_string :

                record_type = SYS_REP_RECORD_TYPE_ASTR;

                break;

        case    SLAP_VAR_SYNTAX_ucharArray :

                record_type = SYS_REP_RECORD_TYPE_BSTR;

                break;

        case    SLAP_VAR_SYNTAX_uint32 :

                record_type = SYS_REP_RECORD_TYPE_UINT;

                break;

        case    SLAP_VAR_SYNTAX_handle :

                record_type = SYS_REP_RECORD_TYPE_HCXT;

                break;

        default :

                break;
    }

    return  record_type;
}


#ifdef   _SLAP_VAR_CONVERTER_SUPPORT_MOAM

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SlapVhoConvFromMoamType
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       moam_type
            );

    description:

        This function is called to convert the MOAM variable type to 
        SLAP object syntax.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       moam_type
                Specifies the MOAM variable type to be converted.

    return:     slap syntax.

**********************************************************************/

ULONG
SlapVhoConvFromMoamType
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       moam_type
    )
{
    ULONG                           slap_syntax = SLAP_VAR_SYNTAX_other;

    switch ( moam_type )
    {
        case    MOAM_OBJ_TYPE_SINT :

                slap_syntax = SLAP_VAR_SYNTAX_int;

                break;

        case    MOAM_OBJ_TYPE_UINT :

                slap_syntax = SLAP_VAR_SYNTAX_uint32;

                break;

        case    MOAM_OBJ_TYPE_LINT :

                slap_syntax = SLAP_VAR_SYNTAX_uint32Array;

                break;

        case    MOAM_OBJ_TYPE_BSTR :
        case    MOAM_OBJ_TYPE_BUFF :

                slap_syntax = SLAP_VAR_SYNTAX_ucharArray;

                break;

        case    MOAM_OBJ_TYPE_OID :

                slap_syntax = SLAP_VAR_SYNTAX_uint32Array;

                break;

        default :

                slap_syntax = SLAP_VAR_SYNTAX_other;

                break;
    }

    return  slap_syntax;
}

#endif /* _SLAP_VAR_CONVERTER_SUPPORT_MOAM */


#ifdef   _SLAP_VAR_CONVERTER_SUPPORT_MIB2

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SlapVhoConvFromMib2Syntax
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       mib2_syntax
            );

    description:

        This function is called to convert the MIB2 variable syntax to 
        SLAP object syntax.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       mib2_syntax
                Specifies the MIB2 variable syntax to be converted.

    return:     slap syntax.

**********************************************************************/

ULONG
SlapVhoConvFromMib2Syntax
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       mib2_syntax
    )
{
    ULONG                           slap_syntax = SLAP_VAR_SYNTAX_other;

    switch ( mib2_syntax )
    {
        case    MIB2_OBJ_SYNTAX_INTEGER     :
        case    MIB2_OBJ_SYNTAX_TestAndIncr :
        case    MIB2_OBJ_SYNTAX_RowStatus   :

                slap_syntax = SLAP_VAR_SYNTAX_int;

                break;

        case    MIB2_OBJ_SYNTAX_OCTET_STRING :
        case    MIB2_OBJ_SYNTAX_NamedBits    :
        case    MIB2_OBJ_SYNTAX_Opaque       :

                slap_syntax = SLAP_VAR_SYNTAX_ucharArray;

                break;

        case    MIB2_OBJ_SYNTAX_OID :

                slap_syntax = SLAP_VAR_SYNTAX_uint32Array;

                break;

        case    MIB2_OBJ_SYNTAX_IpAddress :

                slap_syntax = SLAP_VAR_SYNTAX_uint32;

                break;

        case    MIB2_OBJ_SYNTAX_Counter32  :
        case    MIB2_OBJ_SYNTAX_Gauge32    :
        case    MIB2_OBJ_SYNTAX_TimeTicks  :
        case    MIB2_OBJ_SYNTAX_Unsigned32 :

                slap_syntax = SLAP_VAR_SYNTAX_uint32;

                break;

        case    MIB2_OBJ_SYNTAX_Counter64 :

                slap_syntax = SLAP_VAR_SYNTAX_uint32Array;

                break;

        case    MIB2_OBJ_SYNTAX_TABLE       :
        case    MIB2_OBJ_SYNTAX_ROW_INDEX   :
        case    MIB2_OBJ_SYNTAX_ROW_AUGMENT :

                slap_syntax = SLAP_VAR_SYNTAX_object;

                break;

        default :

                slap_syntax = SLAP_VAR_SYNTAX_other;

                break;
    }

    return  slap_syntax;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        SlapVhoConvFromMib2Var
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 mib2_variable,
                ANSC_HANDLE                 slap_variable
            );

    description:

        This function is called to convert a MIB2 variable to a SLAP
        variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 mib2_variable
                Specifies the MIB2 variable to be converted.

                ANSC_HANDLE                 slap_variable
                Specifies the SLAP variable to be filled.

    return:     none.

**********************************************************************/

void
SlapVhoConvFromMib2Var
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 mib2_variable,
        ANSC_HANDLE                 slap_variable
    )
{
    PMIB2_VARIABLE                  mib2_var   = (PMIB2_VARIABLE)mib2_variable;
    PSLAP_VARIABLE                  slap_var   = (PSLAP_VARIABLE)slap_variable;
    ULONG                           array_size = 0;

    SlapInitVariable(slap_var);

    switch ( mib2_var->Syntax )
    {
        case    MIB2_OBJ_SYNTAX_INTEGER     :
        case    MIB2_OBJ_SYNTAX_TestAndIncr :
        case    MIB2_OBJ_SYNTAX_RowStatus   :

                slap_var->Syntax         = SLAP_VAR_SYNTAX_int;
                slap_var->Variant.varInt = mib2_var->Param.Integer;

                break;

        case    MIB2_OBJ_SYNTAX_OCTET_STRING :
        case    MIB2_OBJ_SYNTAX_NamedBits    :
        case    MIB2_OBJ_SYNTAX_Opaque       :

                slap_var->Syntax = SLAP_VAR_SYNTAX_ucharArray;

                if ( mib2_var->Param.OctetString )
                {
                    array_size  = sizeof(SLAP_UCHAR_ARRAY);
                    array_size += mib2_var->Param.OctetString->ArraySize;

                    slap_var->Variant.varUcharArray =
                        (SLAP_UCHAR_ARRAY*)AnscAllocateMemory(array_size);

                    if ( slap_var->Variant.varUcharArray )
                    {
                        slap_var->Variant.varUcharArray->Size     = array_size;
                        slap_var->Variant.varUcharArray->VarCount =
                            mib2_var->Param.OctetString->Length;
                        slap_var->Variant.varUcharArray->Syntax   =
                            SLAP_VAR_SYNTAX_ucharArray;

                        AnscCopyMemory
                            (
                                slap_var->Variant.varUcharArray->Array.arrayUchar,
                                mib2_var->Param.OctetString->Octets,
                                mib2_var->Param.OctetString->ArraySize
                            );
                    }
                }
                else
                {
                    slap_var->Variant.varUcharArray = NULL;
                }

                break;

        case    MIB2_OBJ_SYNTAX_OID :

                slap_var->Syntax = SLAP_VAR_SYNTAX_uint32Array;

                if ( mib2_var->Param.Oid )
                {
                    array_size  = sizeof(SLAP_UINT32_ARRAY);
                    array_size += mib2_var->Param.Oid->ArraySize;

                    slap_var->Variant.varUint32Array =
                        (SLAP_UINT32_ARRAY*)AnscAllocateMemory(array_size);

                    if ( slap_var->Variant.varUint32Array )
                    {
                        slap_var->Variant.varUint32Array->Size     = array_size;
                        slap_var->Variant.varUint32Array->VarCount =
                            mib2_var->Param.Oid->Length;
                        slap_var->Variant.varUint32Array->Syntax   =
                            SLAP_VAR_SYNTAX_uint32Array;

                        AnscCopyMemory
                            (
                                slap_var->Variant.varUint32Array->Array.arrayUint32,
                                mib2_var->Param.Oid->Identifiers,
                                mib2_var->Param.Oid->ArraySize
                            );
                    }
                }
                else
                {
                    slap_var->Variant.varUint32Array = NULL;
                }

                break;

        case    MIB2_OBJ_SYNTAX_IpAddress :

                slap_var->Syntax            = SLAP_VAR_SYNTAX_uint32;
                slap_var->Variant.varUint32 = mib2_var->Param.IpAddress;

                break;

        case    MIB2_OBJ_SYNTAX_Counter32  :
        case    MIB2_OBJ_SYNTAX_Gauge32    :
        case    MIB2_OBJ_SYNTAX_TimeTicks  :
        case    MIB2_OBJ_SYNTAX_Unsigned32 :

                slap_var->Syntax            = SLAP_VAR_SYNTAX_uint32;
                slap_var->Variant.varUint32 = mib2_var->Param.Unsigned32;

                break;

        case    MIB2_OBJ_SYNTAX_Counter64 :

                slap_var->Syntax = SLAP_VAR_SYNTAX_uint32Array;

                if ( mib2_var->Param.Counter64 )
                {
                    array_size  = sizeof(SLAP_UINT32_ARRAY);
                    array_size += sizeof(ULONG) * 2;

                    slap_var->Variant.varUint32Array =
                        (SLAP_UINT32_ARRAY*)AnscAllocateMemory(array_size);

                    if ( slap_var->Variant.varUint32Array )
                    {
                        slap_var->Variant.varUint32Array->Size     = array_size;
                        slap_var->Variant.varUint32Array->VarCount = 2;
                        slap_var->Variant.varUint32Array->Syntax   =
                            SLAP_VAR_SYNTAX_uint32Array;

                        AnscCopyMemory
                            (
                                slap_var->Variant.varUint32Array->Array.arrayUint32,
                                mib2_var->Param.Counter64,
                                sizeof(ULONG) * 2
                            );
                    }
                }
                else
                {
                    slap_var->Variant.varUint32Array = NULL;
                }

                break;

        case    MIB2_OBJ_SYNTAX_TABLE       :
        case    MIB2_OBJ_SYNTAX_ROW_INDEX   :
        case    MIB2_OBJ_SYNTAX_ROW_AUGMENT :

                slap_var->Syntax            = SLAP_VAR_SYNTAX_object;
                slap_var->Variant.varObject = (SLAP_OBJECT)NULL;

                break;

        default :

                slap_var->Syntax = SLAP_VAR_SYNTAX_other;

                break;
    }

    return;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        SlapVhoConvToMib2Var
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 slap_variable,
                ANSC_HANDLE                 mib2_variable,
                ULONG                       mib2_syntax
            );

    description:

        This function is called to convert a SLAP variable to a MIB2
        variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 slap_variable
                Specifies the SLAP variable to be converted.

                ANSC_HANDLE                 mib2_variable
                Specifies the MIB2 variable to be filled.

                ULONG                       mib2_syntax
                Specifies the syntax of the MIB2 variable.

    return:     none.

**********************************************************************/

void
SlapVhoConvToMib2Var
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 slap_variable,
        ANSC_HANDLE                 mib2_variable,
        ULONG                       mib2_syntax
    )
{
    PSLAP_VARIABLE                  slap_var = (PSLAP_VARIABLE)slap_variable;
    PMIB2_VARIABLE                  mib2_var = (PMIB2_VARIABLE)mib2_variable;

    mib2_var->Name   = NULL;
    mib2_var->Syntax = mib2_syntax;

    switch ( mib2_var->Syntax )
    {
        case    MIB2_OBJ_SYNTAX_INTEGER     :
        case    MIB2_OBJ_SYNTAX_TestAndIncr :
        case    MIB2_OBJ_SYNTAX_RowStatus   :

                mib2_var->Param.Integer = slap_var->Variant.varInt;

                break;

        case    MIB2_OBJ_SYNTAX_OCTET_STRING :
        case    MIB2_OBJ_SYNTAX_NamedBits    :
        case    MIB2_OBJ_SYNTAX_Opaque       :

                if ( slap_var->Variant.varUcharArray )
                {
                    mib2_var->Param.OctetString =
                        (ANSC_ASN1_OCTET_STRING*)AnscAsn1AllocOctetString
                            (
                                slap_var->Variant.varUcharArray->Array.arrayUchar,
                                slap_var->Variant.varUcharArray->VarCount
                            );
                }
                else
                {
                    mib2_var->Param.OctetString = NULL;
                }

                break;

        case    MIB2_OBJ_SYNTAX_OID :

                if ( slap_var->Variant.varUint32Array )
                {
                    mib2_var->Param.Oid =
                        (ANSC_ASN1_OID*)AnscAsn1AllocOid
                            (
                                slap_var->Variant.varUint32Array->Array.arrayUint32,
                                slap_var->Variant.varUint32Array->VarCount
                            );
                }
                else
                {
                    mib2_var->Param.Oid = NULL;
                }

                break;

        case    MIB2_OBJ_SYNTAX_IpAddress :

                mib2_var->Param.IpAddress = slap_var->Variant.varUint32;

                break;

        case    MIB2_OBJ_SYNTAX_Counter32  :
        case    MIB2_OBJ_SYNTAX_Gauge32    :
        case    MIB2_OBJ_SYNTAX_TimeTicks  :
        case    MIB2_OBJ_SYNTAX_Unsigned32 :

                mib2_var->Param.Unsigned32 = slap_var->Variant.varUint32;

                break;

        case    MIB2_OBJ_SYNTAX_Counter64 :

                if ( (slap_var->Variant.varUint32Array          != NULL) &&
                     (slap_var->Variant.varUint32Array->VarCount == 2  ) )
                {
                    mib2_var->Param.Counter64 =
                        (ANSC_UINT64*)AnscAllocateMemory(sizeof(ANSC_UINT64));

                    if ( mib2_var->Param.Counter64 )
                    {
                        mib2_var->Param.Counter64->HiPart =
                            slap_var->Variant.varUint32Array->Array.arrayUint32[0];
                        mib2_var->Param.Counter64->LoPart =
                            slap_var->Variant.varUint32Array->Array.arrayUint32[1];
                    }
                }
                else
                {
                    mib2_var->Param.Counter64 = NULL;
                }

                break;

        case    MIB2_OBJ_SYNTAX_TABLE       :
        case    MIB2_OBJ_SYNTAX_ROW_INDEX   :
        case    MIB2_OBJ_SYNTAX_ROW_AUGMENT :

                break;

        default :

                break;
    }

    return;
}

#endif /* _SLAP_VAR_CONVERTER_SUPPORT_MIB2 */


