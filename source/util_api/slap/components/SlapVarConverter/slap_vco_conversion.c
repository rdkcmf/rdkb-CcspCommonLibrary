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

    module:	slap_vco_conversion.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced conversion functions
        of the Slap Var Converter Object.

        *   SlapVcoConvertVariable

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/13/03    initial revision.

**********************************************************************/


#include "slap_vco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapVcoConvertVariable
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_VARIABLE*              tpl_var,
                SLAP_VARIABLE*              tbc_var
            );

    description:

        This function is called to convert data variables.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_VARIABLE*              tpl_var
                Specifies the template variable to be used.

                SLAP_VARIABLE*              tbc_var
                Specifies the target variable to be translated.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapVcoConvertVariable
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_VARIABLE*              tpl_var,
        SLAP_VARIABLE*              tbc_var
    )
{
    PSLAP_VAR_CONVERTER_OBJECT      pMyObject    = (PSLAP_VAR_CONVERTER_OBJECT)hThisObject;
    int                             iTbcValue    = 0;
    ULONG                           ulTbcValue   = 0;
    char*                           pTbcValue    = NULL;
    PSLAP_VAR_ARRAY                 pTbcArray    = NULL;

    if ( tpl_var->Syntax == SLAP_VAR_SYNTAX_variable )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else if ( tpl_var->Syntax == tbc_var->Syntax )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else if ( tbc_var->Syntax == SLAP_VAR_SYNTAX_int )
    {
        if ( tpl_var->Syntax == SLAP_VAR_SYNTAX_uint32 )
        {
            tbc_var->Syntax = tpl_var->Syntax;

            return  ANSC_STATUS_SUCCESS;
        }

        switch ( tpl_var->ContentType )
        {
            case    SLAP_CONTENT_TYPE_UNSPECIFIED :

                    if ( tpl_var->Syntax == SLAP_VAR_SYNTAX_bool )
                    {
                        tbc_var->Syntax          = tpl_var->Syntax;
			/*CID: 53684 Assignment of overlapping memory*/
			int int_value            = tbc_var->Variant.varInt;
                        tbc_var->Variant.varBool = (SLAP_BOOL)int_value;
                    }
                    else if ( tpl_var->Syntax == SLAP_VAR_SYNTAX_uint32 )
                    {
                        tbc_var->Syntax            = tpl_var->Syntax;
			/*CID:71834 Assignment of overlapping memory*/
			int int_value              = tbc_var->Variant.varInt;
                        tbc_var->Variant.varUint32 = (SLAP_UINT32)int_value;
                    }
                    else if ( tpl_var->Syntax == SLAP_VAR_SYNTAX_string )
                    {
                        tbc_var->Syntax            = tpl_var->Syntax;
                        iTbcValue                  = tbc_var->Variant.varInt;
                        tbc_var->Variant.varString = (char*)AnscAllocateMemory(32);

                        if ( tbc_var->Variant.varString )
                        {
                            _ansc_itoa
                                (
                                    iTbcValue,
                                    tbc_var->Variant.varString,
                                    10
                                );
                        }
                    }
                    else
                    {
                        return  ANSC_STATUS_UNAPPLICABLE;
                    }

                    break;

            default :

                    return  ANSC_STATUS_UNAPPLICABLE;

                    break;
        }
    }
    else if ( tbc_var->Syntax == SLAP_VAR_SYNTAX_uint32 )
    {
        if ( tpl_var->Syntax == SLAP_VAR_SYNTAX_int )
        {
            tbc_var->Syntax = tpl_var->Syntax;

            return  ANSC_STATUS_SUCCESS;
        }

        switch ( tpl_var->ContentType )
        {
            case    SLAP_CONTENT_TYPE_UNSPECIFIED :

                    if ( tpl_var->Syntax == SLAP_VAR_SYNTAX_bool )
                    {
                        tbc_var->Syntax          = tpl_var->Syntax;
                        tbc_var->Variant.varBool = (SLAP_BOOL)tbc_var->Variant.varUint32;
                    }
                    else if ( tpl_var->Syntax == SLAP_VAR_SYNTAX_int )
                    {
			/* CID 71834 fix */
			SLAP_INT varIntTemp     = 0;
                        tbc_var->Syntax         = tpl_var->Syntax;
                        varIntTemp              = (SLAP_INT)tbc_var->Variant.varUint32;
			tbc_var->Variant.varInt = varIntTemp;
                    }
                    else if ( tpl_var->Syntax == SLAP_VAR_SYNTAX_string )
                    {
                        tbc_var->Syntax            = tpl_var->Syntax;
                        ulTbcValue                 = tbc_var->Variant.varUint32;
                        tbc_var->Variant.varString = (char*)AnscAllocateMemory(32);

                        if ( tbc_var->Variant.varString )
                        {
                            AnscGetUlongString
                                (
                                    ulTbcValue,
                                    tbc_var->Variant.varString
                                );
                        }
                    }
                    else
                    {
                        return  ANSC_STATUS_UNAPPLICABLE;
                    }

                    break;

            default :

                    return  ANSC_STATUS_UNAPPLICABLE;

                    break;
        }
    }
    else if ( tbc_var->Syntax == SLAP_VAR_SYNTAX_string )
    {
        pTbcValue = tbc_var->Variant.varString;

        if ( !pTbcValue )
        {
            return  ANSC_STATUS_UNAPPLICABLE;
        }

        switch ( tpl_var->ContentType )
        {
            case    SLAP_CONTENT_TYPE_UNSPECIFIED :

                    if ( tpl_var->Syntax == SLAP_VAR_SYNTAX_bool )
                    {
                        tbc_var->Syntax          = tpl_var->Syntax;
                        tbc_var->Variant.varBool =
                            pMyObject->StringToBool
                                (
                                    (ANSC_HANDLE)pMyObject,
                                    tbc_var->Variant.varString
                                );

                        AnscFreeMemory(pTbcValue);
                    }
                    else if ( tpl_var->Syntax == SLAP_VAR_SYNTAX_int )
                    {
                        tbc_var->Syntax         = tpl_var->Syntax;
                        tbc_var->Variant.varInt =
                            pMyObject->StringToInt
                                (
                                    (ANSC_HANDLE)pMyObject,
                                    tbc_var->Variant.varString
                                );

                        AnscFreeMemory(pTbcValue);
                    }
                    else if ( tpl_var->Syntax == SLAP_VAR_SYNTAX_ucharArray )
                    {
                        tbc_var->Syntax                = tpl_var->Syntax;
                        tbc_var->Variant.varUcharArray =
                            pMyObject->StringToUcharArray
                                (
                                    (ANSC_HANDLE)pMyObject,
                                    tbc_var->Variant.varString
                                );

                        AnscFreeMemory(pTbcValue);
                    }
                    else if ( tpl_var->Syntax == SLAP_VAR_SYNTAX_uint32 )
                    {
                        tbc_var->Syntax            = tpl_var->Syntax;
                        tbc_var->Variant.varUint32 =
                            pMyObject->StringToUint32
                                (
                                    (ANSC_HANDLE)pMyObject,
                                    tbc_var->Variant.varString
                                );

                        AnscFreeMemory(pTbcValue);
                    }
                    else
                    {
                        return  ANSC_STATUS_UNAPPLICABLE;
                    }

                    break;

            case    SLAP_CONTENT_TYPE_IP4_ADDR :
            case    SLAP_CONTENT_TYPE_SUBNET_MASK :

                    if ( tpl_var->Syntax == SLAP_VAR_SYNTAX_uint32 )
                    {
                        tbc_var->Syntax            = tpl_var->Syntax;
                        tbc_var->Variant.varUint32 =
                            pMyObject->StringToIp4Addr
                                (
                                    (ANSC_HANDLE)pMyObject,
                                    tbc_var->Variant.varString
                                );

                        AnscFreeMemory(pTbcValue);
                    }
                    else
                    {
                        return  ANSC_STATUS_UNAPPLICABLE;
                    }

                    break;


            case    SLAP_CONTENT_TYPE_MAC_ADDR :

                    if ( tpl_var->Syntax == SLAP_VAR_SYNTAX_ucharArray )
                    {
                        tbc_var->Syntax                = tpl_var->Syntax;
                        tbc_var->Variant.varUcharArray =
                            pMyObject->StringToMacAddr
                                (
                                    (ANSC_HANDLE)pMyObject,
                                    tbc_var->Variant.varString
                                );

                        AnscFreeMemory(pTbcValue);
                    }
                    else
                    {
                        return  ANSC_STATUS_UNAPPLICABLE;
                    }

                    break;

            case    SLAP_CONTENT_TYPE_OID_LIST :

                    if ( tpl_var->Syntax == SLAP_VAR_SYNTAX_uint32Array )
                    {
                        tbc_var->Syntax                 = tpl_var->Syntax;
                        tbc_var->Variant.varUint32Array =
                            pMyObject->StringToOidList
                                (
                                    (ANSC_HANDLE)pMyObject,
                                    tbc_var->Variant.varString
                                );

                        AnscFreeMemory(pTbcValue);
                    }
                    else
                    {
                        return  ANSC_STATUS_UNAPPLICABLE;
                    }

                    break;

            case    SLAP_CONTENT_TYPE_ASCII_STRING :

                    if ( tpl_var->Syntax == SLAP_VAR_SYNTAX_ucharArray )
                    {
                        tbc_var->Syntax                = tpl_var->Syntax;
                        tbc_var->Variant.varUcharArray =
                            pMyObject->StringToUcharArray2
                                (
                                    (ANSC_HANDLE)pMyObject,
                                    tbc_var->Variant.varString
                                );

                        AnscFreeMemory(pTbcValue);
                    }
                    else
                    {
                        return  ANSC_STATUS_UNAPPLICABLE;
                    }

                    break;

            case    SLAP_CONTENT_TYPE_HEX_STRING :

                    if ( tpl_var->Syntax == SLAP_VAR_SYNTAX_ucharArray )
                    {
                        tbc_var->Syntax                = tpl_var->Syntax;
                        tbc_var->Variant.varUcharArray =
                            pMyObject->StringToUcharArray
                                (
                                    (ANSC_HANDLE)pMyObject,
                                    tbc_var->Variant.varString
                                );

                        AnscFreeMemory(pTbcValue);
                    }
                    else
                    {
                        return  ANSC_STATUS_UNAPPLICABLE;
                    }

                    break;

            default :

                    return  ANSC_STATUS_UNAPPLICABLE;

                    break;
        }
    }
    else if ( tbc_var->Syntax == SLAP_VAR_SYNTAX_uint32Array )
    {
        pTbcArray = tbc_var->Variant.varUint32Array;

        if ( !pTbcArray )
        {
            return  ANSC_STATUS_UNAPPLICABLE;
        }

        switch ( tpl_var->ContentType )
        {
            case    SLAP_CONTENT_TYPE_UNSPECIFIED :

                    return  ANSC_STATUS_UNAPPLICABLE;

                    break;

            case    SLAP_CONTENT_TYPE_MAC_ADDR :

                    if ( tpl_var->Syntax == SLAP_VAR_SYNTAX_ucharArray )
                    {
                        tbc_var->Syntax                = tpl_var->Syntax;
                        tbc_var->Variant.varUcharArray =
                            pMyObject->Uint32ArrayToMacAddr
                                (
                                    (ANSC_HANDLE)pMyObject,
                                    tbc_var->Variant.varUint32Array
                                );

                        SlapFreeVarArray(pTbcArray);
                    }
                    else
                    {
                        return  ANSC_STATUS_UNAPPLICABLE;
                    }

                    break;

            default :

                    return  ANSC_STATUS_UNAPPLICABLE;

                    break;
        }
    }
    else
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    return  ANSC_STATUS_SUCCESS;
}
