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

    module:	slap_vho_variable.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced variable conversion
        functions of the Slap Var Helper Object.

        *   SlapVhoEqualVariables
        *   SlapVhoCloneVariable
        *   SlapVhoCleanVariable

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

        BOOL
        SlapVhoEqualVariables
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 slap_variable1,
                ANSC_HANDLE                 slap_variable2
            );

    description:

        This function is called to compare two variables.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 slap_variable1
                Specifies the first variable to be compared.

                ANSC_HANDLE                 slap_variable2
                Specifies the second variable to be compared.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
SlapVhoEqualVariables
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 slap_variable1,
        ANSC_HANDLE                 slap_variable2
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_VARIABLE                  var1        = (PSLAP_VARIABLE)slap_variable1;
    PSLAP_VARIABLE                  var2        = (PSLAP_VARIABLE)slap_variable2;
    BOOL                            b_equal_var = TRUE;

    do
    {
        if ( var1->Syntax != var2->Syntax )
        {
            b_equal_var = FALSE;

            break;
        }

        switch ( var1->Syntax )
        {
            case    SLAP_VAR_SYNTAX_bool :

                    b_equal_var =
                        (var1->Variant.varBool == var2->Variant.varBool);

                    break;

            case    SLAP_VAR_SYNTAX_boolArray :

                    SlapEqualVarArrays
                        (
                            var1->Variant.varBoolArray,
                            var2->Variant.varBoolArray,
                            b_equal_var
                        );

                    break;

            case    SLAP_VAR_SYNTAX_charArray :

                    SlapEqualVarArrays
                        (
                            var1->Variant.varCharArray,
                            var2->Variant.varCharArray,
                            b_equal_var
                        );

                    break;

            case    SLAP_VAR_SYNTAX_int :

                    b_equal_var =
                        (var1->Variant.varInt == var2->Variant.varInt);

                    break;

            case    SLAP_VAR_SYNTAX_intArray :

                    SlapEqualVarArrays
                        (
                            var1->Variant.varIntArray,
                            var2->Variant.varIntArray,
                            b_equal_var
                        );

                    break;

            case    SLAP_VAR_SYNTAX_string :

                    if ( var1->Variant.varString && var2->Variant.varString )
                    {
                        b_equal_var =
                            ( strcmp(var1->Variant.varString,var2->Variant.varString) == 0 );
                    }
                    else if ( !var1->Variant.varString && !var2->Variant.varString )
                    {
                        b_equal_var = TRUE;
                    }
                    else
                    {
                        b_equal_var = FALSE;
                    }

                    break;

            case    SLAP_VAR_SYNTAX_stringArray :

                    SlapEqualVarArrays
                        (
                            var1->Variant.varStringArray,
                            var2->Variant.varStringArray,
                            b_equal_var
                        );

                    break;

            case    SLAP_VAR_SYNTAX_ucharArray :

                    SlapEqualVarArrays
                        (
                            var1->Variant.varUcharArray,
                            var2->Variant.varUcharArray,
                            b_equal_var
                        );

                    break;

            case    SLAP_VAR_SYNTAX_uint32 :

                    b_equal_var =
                        (var1->Variant.varUint32 == var2->Variant.varUint32);

                    break;

            case    SLAP_VAR_SYNTAX_uint32Array :

                    SlapEqualVarArrays
                        (
                            var1->Variant.varUint32Array,
                            var2->Variant.varUint32Array,
                            b_equal_var
                        );

                    break;

            case    SLAP_VAR_SYNTAX_ptr :

                    b_equal_var =
                        (var1->Variant.varPtr == var2->Variant.varPtr);

                    break;

            case    SLAP_VAR_SYNTAX_ptrArray :

                    SlapEqualVarArrays
                        (
                            var1->Variant.varPtrArray,
                            var2->Variant.varPtrArray,
                            b_equal_var
                        );

                    break;

            case    SLAP_VAR_SYNTAX_handle :

                    b_equal_var =
                        (var1->Variant.varHandle == var2->Variant.varHandle);

                    break;

            case    SLAP_VAR_SYNTAX_handleArray :

                    SlapEqualVarArrays
                        (
                            var1->Variant.varHandleArray,
                            var2->Variant.varHandleArray,
                            b_equal_var
                        );

                    break;

            case    SLAP_VAR_SYNTAX_object :

                    b_equal_var =
                        (var1->Variant.varObject == var2->Variant.varObject);

                    break;

            case    SLAP_VAR_SYNTAX_objectArray :

                    SlapEqualVarArrays
                        (
                            var1->Variant.varObjectArray,
                            var2->Variant.varObjectArray,
                            b_equal_var
                        );

                    break;

            default :

                    b_equal_var = FALSE;

                    break;
        }

        break;
    }
    while ( FALSE );

    return  b_equal_var;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        SlapVhoCloneVariable
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 src_variable,
                ANSC_HANDLE                 dst_variable
            );

    description:

        This function is called to clone a variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 src_variable
                Specifies the SLAP variable to be cloned.

                ANSC_HANDLE                 dst_variable
                Specifies the SLAP variable to be filed.

    return:     none.

**********************************************************************/

void
SlapVhoCloneVariable
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 src_variable,
        ANSC_HANDLE                 dst_variable
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_VARIABLE                  src_var = (PSLAP_VARIABLE)src_variable;
    PSLAP_VARIABLE                  dst_var = (PSLAP_VARIABLE)dst_variable;

    SlapCleanVariable(dst_var);

    dst_var->Name        = (src_var->Name) ? AnscCloneString(src_var->Name) : NULL;
    dst_var->ContentType = src_var->ContentType;
    dst_var->UsageType   = src_var->UsageType;
    dst_var->Syntax      = src_var->Syntax;

    switch ( src_var->Syntax )
    {
        case    SLAP_VAR_SYNTAX_bool :

                dst_var->Variant.varBool = src_var->Variant.varBool;

                break;

        case    SLAP_VAR_SYNTAX_boolArray :

                if ( src_var->Variant.varBoolArray )
                {
                    SlapCloneVarArray
                        (
                            src_var->Variant.varBoolArray,
                            dst_var->Variant.varBoolArray
                        );
                }
                else
                {
                    dst_var->Variant.varBoolArray = NULL;
                }

                break;

        case    SLAP_VAR_SYNTAX_charArray :

                if ( src_var->Variant.varCharArray )
                {
                    SlapCloneVarArray
                        (
                            src_var->Variant.varCharArray,
                            dst_var->Variant.varCharArray
                        );
                }
                else
                {
                    dst_var->Variant.varCharArray = NULL;
                }

                break;

        case    SLAP_VAR_SYNTAX_int :

                dst_var->Variant.varInt = src_var->Variant.varInt;

                break;

        case    SLAP_VAR_SYNTAX_intArray :

                if ( src_var->Variant.varIntArray )
                {
                    SlapCloneVarArray
                        (
                            src_var->Variant.varIntArray,
                            dst_var->Variant.varIntArray
                        );
                }
                else
                {
                    dst_var->Variant.varIntArray = NULL;
                }

                break;

        case    SLAP_VAR_SYNTAX_string :

                if ( src_var->Variant.varString )
                {
                    dst_var->Variant.varString =
                        AnscCloneString
                            (
                                src_var->Variant.varString
                            );
                }
                else
                {
                    dst_var->Variant.varString = NULL;
                }

                break;

        case    SLAP_VAR_SYNTAX_stringArray :

                if ( src_var->Variant.varStringArray )
                {
                    SlapCloneVarArray
                        (
                            src_var->Variant.varStringArray,
                            dst_var->Variant.varStringArray
                        );
                }
                else
                {
                    dst_var->Variant.varStringArray = NULL;
                }

                break;

        case    SLAP_VAR_SYNTAX_ucharArray :

                if ( src_var->Variant.varUcharArray )
                {
                    SlapCloneVarArray
                        (
                            src_var->Variant.varUcharArray,
                            dst_var->Variant.varUcharArray
                        );
                }
                else
                {
                    dst_var->Variant.varUcharArray = NULL;
                }

                break;

        case    SLAP_VAR_SYNTAX_uint32 :

                dst_var->Variant.varUint32 = src_var->Variant.varUint32;

                break;

        case    SLAP_VAR_SYNTAX_uint32Array :

                if ( src_var->Variant.varUint32Array )
                {
                    SlapCloneVarArray
                        (
                            src_var->Variant.varUint32Array,
                            dst_var->Variant.varUint32Array
                        );
                }
                else
                {
                    dst_var->Variant.varUint32Array = NULL;
                }

                break;

        case    SLAP_VAR_SYNTAX_ptr :

                dst_var->Variant.varPtr = src_var->Variant.varPtr;

                break;

        case    SLAP_VAR_SYNTAX_ptrArray :

                if ( src_var->Variant.varPtrArray )
                {
                    SlapCloneVarArray
                        (
                            src_var->Variant.varPtrArray,
                            dst_var->Variant.varPtrArray
                        );
                }
                else
                {
                    dst_var->Variant.varPtrArray = NULL;
                }

                break;

        case    SLAP_VAR_SYNTAX_handle :

                dst_var->Variant.varHandle = src_var->Variant.varHandle;

                break;

        case    SLAP_VAR_SYNTAX_handleArray :

                if ( src_var->Variant.varHandleArray )
                {
                    SlapCloneVarArray
                        (
                            src_var->Variant.varHandleArray,
                            dst_var->Variant.varHandleArray
                        );
                }
                else
                {
                    dst_var->Variant.varHandleArray = NULL;
                }

                break;

        case    SLAP_VAR_SYNTAX_object :

                dst_var->Variant.varObject = src_var->Variant.varObject;

                break;

        case    SLAP_VAR_SYNTAX_objectArray :

                if ( src_var->Variant.varObjectArray )
                {
                    SlapCloneVarArray
                        (
                            src_var->Variant.varObjectArray,
                            dst_var->Variant.varObjectArray
                        );
                }
                else
                {
                    dst_var->Variant.varObjectArray = NULL;
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

        void
        SlapVhoCleanVariable
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 slap_variable
            );

    description:

        This function is called to clean a variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 slap_variable
                Specifies the SLAP variable to be cleaned.

    return:     none.

**********************************************************************/

void
SlapVhoCleanVariable
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 slap_variable
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PSLAP_VARIABLE                  tbc_var = (PSLAP_VARIABLE)slap_variable;

    if ( tbc_var->Name )
    {
        AnscFreeMemory(tbc_var->Name);

        tbc_var->Name = NULL;
    }

    switch ( tbc_var->Syntax )
    {
        case    SLAP_VAR_SYNTAX_bool :

                break;

        case    SLAP_VAR_SYNTAX_boolArray :

                if ( tbc_var->Variant.varBoolArray )
                {
                    SlapFreeVarArray(tbc_var->Variant.varBoolArray);
                }

                break;

        case    SLAP_VAR_SYNTAX_charArray :

                if ( tbc_var->Variant.varCharArray )
                {
                    SlapFreeVarArray(tbc_var->Variant.varCharArray);
                }

                break;

        case    SLAP_VAR_SYNTAX_int :

                break;

        case    SLAP_VAR_SYNTAX_intArray :

                if ( tbc_var->Variant.varIntArray )
                {
                    SlapFreeVarArray(tbc_var->Variant.varIntArray);
                }

                break;

        case    SLAP_VAR_SYNTAX_string :

                if ( tbc_var->Variant.varString )
                {
                    AnscFreeMemory(tbc_var->Variant.varString);
                }

                break;

        case    SLAP_VAR_SYNTAX_stringArray :

                if ( tbc_var->Variant.varStringArray )
                {
                    SlapFreeVarArray(tbc_var->Variant.varStringArray);
                }

                break;

        case    SLAP_VAR_SYNTAX_ucharArray :

                if ( tbc_var->Variant.varUcharArray )
                {
                    SlapFreeVarArray(tbc_var->Variant.varUcharArray);
                }

                break;

        case    SLAP_VAR_SYNTAX_uint32 :

                break;

        case    SLAP_VAR_SYNTAX_uint32Array :

                if ( tbc_var->Variant.varUint32Array )
                {
                    SlapFreeVarArray(tbc_var->Variant.varUint32Array);
                }

                break;

        case    SLAP_VAR_SYNTAX_ptr :

                break;

        case    SLAP_VAR_SYNTAX_ptrArray :

                if ( tbc_var->Variant.varPtrArray )
                {
                    SlapFreeVarArray(tbc_var->Variant.varPtrArray);
                }

                break;

        case    SLAP_VAR_SYNTAX_handle :

                break;

        case    SLAP_VAR_SYNTAX_handleArray :

                if ( tbc_var->Variant.varHandleArray )
                {
                    SlapFreeVarArray(tbc_var->Variant.varHandleArray);
                }

                break;

        case    SLAP_VAR_SYNTAX_object :

                break;

        case    SLAP_VAR_SYNTAX_objectArray :

                if ( tbc_var->Variant.varObjectArray )
                {
                    SlapFreeVarArray(tbc_var->Variant.varObjectArray);
                }

                break;

        default :

                break;
    }

    return;
}
