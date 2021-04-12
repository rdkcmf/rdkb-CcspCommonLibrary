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

    module:	bspeng_var_slap_conv.c

        For BSP Engine Var Component Object,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module provides utilities to do conversions between
        script variables and SLAP variables etc.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        07/18/03    initial revision.
        11/05/03    array support and conversion between 
                    SLAP array and BSP array.
                    Removed Bin's temporary solution for binary
                    data displaying.

**********************************************************************/


#include "bspeng_co_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BspEng_VC_Var2Slap
            (
                ANSC_HANDLE                 hVar,
                ANSC_HANDLE                 hSlapVar
            )

    description:

        This function is called to convert BSP variable to SLAP
        variable.

    argument:   ANSC_HANDLE                 hVar
                Handle to BSP variable.

                ANSC_HANDLE                 hSlapVar
                Handle to SLAP variable.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BspEng_VC_Var2Slap
    (
        ANSC_HANDLE                 hVar,
        ANSC_HANDLE                 hSlapVar
    )
{
    PBSP_TEMPLATE_VAR_OBJECT        pVar        = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    PSLAP_VARIABLE                  pSlapVar    = (PSLAP_VARIABLE)hSlapVar;
    ANSC_STATUS                     status      = ANSC_STATUS_SUCCESS;

    if (!hVar || !hSlapVar)
    {
        return ANSC_STATUS_NOT_SUPPORTED;
    }

    /* supposed SLAP object has been allocated before conversion */
    pSlapVar->Name          = NULL;
    pSlapVar->ContentType   = pVar->ContentType;
    pSlapVar->UsageType     = 0;

    switch (pVar->Type)
    {
        case    BspVar_Number:

            pSlapVar->Syntax            = SLAP_VAR_SYNTAX_TYPE_int;
            pSlapVar->Variant.varInt    = (SLAP_INT)pVar->Value.num;

            break;

        case    BspVar_UNumber:

            pSlapVar->Syntax            = SLAP_VAR_SYNTAX_TYPE_uint32;
            pSlapVar->Variant.varInt    = (SLAP_UINT32)pVar->Value.num;

            break;

        case    BspVar_String:

            {
                ULONG               ulSize  = pVar->Size;
                char                *pStr   = pVar->Value.str;
                SLAP_CHAR           *pNew   = NULL;

                if (ulSize == 0 && pStr)
                {
                    ulSize  = AnscSizeOfString(pStr);
                }
                else
                {
                    if (!pStr)
                    {
                        ulSize  = 0;
                    }
                }

                if (ulSize != 0)
                {
                    pNew    = (SLAP_CHAR *)AnscAllocateMemory((ulSize + 1) * sizeof(SLAP_CHAR));
                    if (pNew)
                    {
                        AnscCopyMemory(pNew, pStr, ulSize);
                        pNew[ulSize] = 0;   /* terminate the string anyway */
                    }
                }

                pSlapVar->Syntax            = SLAP_VAR_SYNTAX_TYPE_string;
                pSlapVar->Variant.varString = pNew;
            }

            break;

        case    BspVar_Object:
            
            if (pVar->Value.obj)
            {
                SlapCloneVariable(((PSLAP_VARIABLE)pVar->Value.obj), pSlapVar);
            }
            else
            {
                pSlapVar->Syntax            = SLAP_VAR_SYNTAX_TYPE_object;
                pSlapVar->Variant.varObject = NULL;
            }

            break;

        case    BspVar_Array:

            /*
             *  Try to convert BSP array to SLAP array item
             */
            {
                PBSP_TEMPLATE_ARRAY_ITEMS   pArrayItems;

                pArrayItems = &pVar->Value.arrayItems;

                /* we allow zero-element array to be passed in SLAP call */
                if (pArrayItems->numDims != 1)
                {
                    /* since SLAP doesn't support multiple-dimensional array */
                    status  = ANSC_STATUS_BAD_PARAMETER;
                }
                else
                {
                    if (pArrayItems->bBulkData)
                    {
                        status  = BspEng_VC_BulkData2Slap(hVar, hSlapVar);
                    }
                    else
                    {
                        status  = BspEng_VC_VarArray2Slap(hVar, hSlapVar);
                    }
                }
            }

            break;

        default:

            /* 
             * we don't need to care about other types since
             * we should already have got the value instead
             * of its reference. For type BspVar_Real, SLAP
             * current doesn't support it.
             */

            status  = ANSC_STATUS_FAILURE;

            break;
    
    }
    
    return status;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BspEng_VC_Slap2Var
            (
                ANSC_HANDLE                 hSlapVar,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to convert SLAP variable to 
        BSP variable.

    argument:   ANSC_HANDLE                 hSlapVar
                Handle to SLAP variable.

                ANSC_HANDLE                 hVar
                Handle to BSP variable.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BspEng_VC_Slap2Var
    (
        ANSC_HANDLE                 hSlapVar,
        ANSC_HANDLE                 hVar
    )
{
    PSLAP_VARIABLE                  pSlapVar    = (PSLAP_VARIABLE)hSlapVar;
    PBSP_TEMPLATE_VAR_OBJECT        pVar        = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    ANSC_STATUS                     status      = ANSC_STATUS_SUCCESS;
    PBSP_TEMPLATE_ARRAY_ITEMS       pArrayItems = NULL;
    PBSP_TEMPLATE_VAR_OBJECT        *pVars;
    ULONG                           i;

    if (!hVar || !hSlapVar)
    {
        return ANSC_STATUS_NOT_SUPPORTED;
    }

    pVar->Reset((ANSC_HANDLE)pVar);

    switch (pSlapVar->Syntax)
    {
        case    SLAP_VAR_SYNTAX_TYPE_void:

            pVar->Type      = BspVar_String;
            pVar->Value.str = NULL;
            pVar->Size      = 0;

            ACCESS_BSP_TEMPLATE_VAR_CLEAR_TEMP_FLAG(pVar);

            break;

        case    SLAP_VAR_SYNTAX_TYPE_bool:

            pVar->Type      = BspVar_Number;
            pVar->Value.num = pSlapVar->Variant.varBool;

            break;

        case    SLAP_VAR_SYNTAX_TYPE_int:

            pVar->Type      = BspVar_Number;
            pVar->Value.num = pSlapVar->Variant.varInt;

            break;

        case    SLAP_VAR_SYNTAX_TYPE_uint32:

            pVar->Type      = BspVar_UNumber;
            pVar->Value.num = pSlapVar->Variant.varUint32;

            break;

        case SLAP_VAR_SYNTAX_TYPE_string:

            {
                ULONG               ulSize  = 0;
                char                *pStr;

                if (!pSlapVar->Variant.varString)
                {
                    pStr    = NULL;
                }
                else
                {
                    ulSize  = AnscSizeOfString(pSlapVar->Variant.varString);

                    pStr    = (char *)AnscAllocateMemory(ulSize + 1);

                    if (pStr)
                    {
                        AnscCopyMemory(pStr, pSlapVar->Variant.varString, ulSize);
                        pStr[ulSize]    = '\0';
                    }
                }

                pVar->Type      = BspVar_String;
                pVar->Size      = ulSize;
                pVar->Value.str = pStr;

                ACCESS_BSP_TEMPLATE_VAR_CLEAR_TEMP_FLAG(pVar);
            }

            break;

        case    SLAP_VAR_SYNTAX_TYPE_boolArray:
            {
                PSLAP_BOOL_ARRAY    pBoolArray;

                pBoolArray  = (PSLAP_BOOL_ARRAY)pSlapVar->Variant.varBoolArray;
                status  = 
                    BspEng_PrepBspArrayVar
                        (
                            pVar, 
                            pBoolArray ? pBoolArray->VarCount : 0, 
                            BspArray_Bool
                        );

                if (status == ANSC_STATUS_SUCCESS)
                {
                    ULONG           ulSize  = pBoolArray ? pBoolArray->VarCount : 0;
                    pVars   = (PBSP_TEMPLATE_VAR_OBJECT *)pVar->Value.arrayItems.hVars;

                    for (i = 0; i < ulSize; i ++)
                    {
                        pVars[i]->Type      = BspVar_Number;
                        pVars[i]->Value.num = pBoolArray->Array.arrayBool[i];
                    }
                }
            }
            break;

        case    SLAP_VAR_SYNTAX_TYPE_intArray:
            {
                PSLAP_INT_ARRAY     pIntArray;

                pIntArray   = (PSLAP_BOOL_ARRAY)pSlapVar->Variant.varIntArray;
                status      = 
                    BspEng_PrepBspArrayVar
                        (
                            pVar, 
                            pIntArray ? pIntArray->VarCount : 0, 
                            BspArray_Int
                        );

                if (status == ANSC_STATUS_SUCCESS)
                {
                    ULONG           ulSize  = pIntArray ? pIntArray->VarCount : 0;
                    pVars   = (PBSP_TEMPLATE_VAR_OBJECT *)pVar->Value.arrayItems.hVars;

                    for (i = 0; i < ulSize; i ++)
                    {
                        pVars[i]->Type      = BspVar_Number;
                        pVars[i]->Value.num = pIntArray->Array.arrayInt[i];
                    }
                }
            }
            break;

        case    SLAP_VAR_SYNTAX_TYPE_stringArray:
            {
                PSLAP_STRING_ARRAY  pStringArray;

                pStringArray    = (PSLAP_STRING_ARRAY)pSlapVar->Variant.varStringArray;
                status          = 
                    BspEng_PrepBspArrayVar
                        (
                            pVar, 
                            pStringArray ? pStringArray->VarCount : 0, 
                            BspArray_String
                        );

                if (status == ANSC_STATUS_SUCCESS)
                {
                    ULONG           ulSize  = pStringArray ? pStringArray->VarCount : 0;
                    pVars   = (PBSP_TEMPLATE_VAR_OBJECT *)pVar->Value.arrayItems.hVars;

                    for (i = 0; i < ulSize; i ++)
                    {
                        pVars[i]->Type          = BspVar_String;

                        pVars[i]->Value.str    = (char *)AnscDupString((PUCHAR)pStringArray->Array.arrayString[i]);
                        pVars[i]->Size         = pVars[i]->Value.str ? AnscSizeOfString(pVars[i]->Value.str) : 0;

                        ACCESS_BSP_TEMPLATE_VAR_CLEAR_TEMP_FLAG(pVars[i]);
                    }
                }
            }
            break;

        case    SLAP_VAR_SYNTAX_TYPE_charArray:
            {
                PSLAP_CHAR_ARRAY    pCharArray;

                pCharArray  = (PSLAP_CHAR_ARRAY)pSlapVar->Variant.varCharArray;
                status      = 
                    BspEng_PrepBspArrayVar
                        (
                            pVar, 
                            pCharArray ? pCharArray->VarCount : 0, 
                            BspArray_Uchar
                        );

                if (status == ANSC_STATUS_SUCCESS)
                {
                    SLAP_CHAR       *pBulkData;
                    ULONG           ulSize  = pCharArray ? pCharArray->VarCount : 0;

                    pArrayItems = &pVar->Value.arrayItems;
                    pBulkData  = (SLAP_CHAR *)pArrayItems->hBulkData;

                    if (ulSize != 0)
                    {
                        AnscCopyMemory
                            (
                                pBulkData, 
                                pCharArray->Array.arrayChar, 
                                pCharArray->VarCount * sizeof(SLAP_CHAR)
                            );
                    }
                }
            }
            break;

        case    SLAP_VAR_SYNTAX_TYPE_ucharArray:
            {
                PSLAP_CHAR_ARRAY    pUCharArray;

                pUCharArray  = (PSLAP_UCHAR_ARRAY)pSlapVar->Variant.varCharArray;
                status      = 
                    BspEng_PrepBspArrayVar
                        (
                            pVar, 
                            pUCharArray ? pUCharArray->VarCount : 0, 
                            BspArray_Uchar
                        );

                if (status == ANSC_STATUS_SUCCESS)
                {
                    SLAP_CHAR       *pBulkData;
                    ULONG           ulSize  = pUCharArray ? pUCharArray->VarCount : 0;

                    pArrayItems = &pVar->Value.arrayItems;
                    pBulkData  = (SLAP_CHAR *)pArrayItems->hBulkData;

                    if (ulSize != 0)
                    {
                        AnscCopyMemory
                            (
                                pBulkData, 
                                pUCharArray->Array.arrayUchar, 
                                pUCharArray->VarCount * sizeof(SLAP_UCHAR)
                            );
                    }
                }
            }
            break;

        case    SLAP_VAR_SYNTAX_TYPE_uint32Array:
            {
                PSLAP_UINT32_ARRAY     pUint32Array;

                pUint32Array    = (PSLAP_UINT32_ARRAY)pSlapVar->Variant.varUint32Array;
                status          = 
                    BspEng_PrepBspArrayVar
                        (
                            pVar, 
                            pUint32Array ? pUint32Array->VarCount : 0, 
                            BspArray_Uint
                        );

                if (status == ANSC_STATUS_SUCCESS)
                {
                    ULONG           ulSize  = pUint32Array ? pUint32Array->VarCount : 0;
                    pVars   = (PBSP_TEMPLATE_VAR_OBJECT *)pVar->Value.arrayItems.hVars;

                    for (i = 0; i < ulSize; i ++)
                    {
                        pVars[i]->Type      = BspVar_UNumber;
                        pVars[i]->Value.num = pUint32Array->Array.arrayUint32[i];
                    }
                }
            }
            break;

        case    SLAP_VAR_SYNTAX_TYPE_ptrArray:
            {
                PSLAP_PTR_ARRAY     pPtrArray;

                pPtrArray   = (PSLAP_PTR_ARRAY)pSlapVar->Variant.varPtrArray;
                status      = 
                    BspEng_PrepBspArrayVar
                        (
                            pVar, 
                            pPtrArray ? pPtrArray->VarCount : 0, 
                            BspArray_Handle
                        );

                if (status == ANSC_STATUS_SUCCESS)
                {
                    ULONG           ulSize  = pPtrArray ? pPtrArray->VarCount : 0;
                    pVars   = (PBSP_TEMPLATE_VAR_OBJECT *)pVar->Value.arrayItems.hVars;

                    for (i = 0; i < ulSize; i ++)
                    {
                        pVars[i]->Type      = BspVar_UNumber;
                        pVars[i]->Value.num = (ULONG)pPtrArray->Array.arrayPtr[i];
                    }
                }
            }
            break;

        case    SLAP_VAR_SYNTAX_TYPE_handleArray:
            {
                PSLAP_HANDLE_ARRAY     pHandleArray;

                pHandleArray    = (PSLAP_HANDLE_ARRAY)pSlapVar->Variant.varHandleArray;
                status          = 
                    BspEng_PrepBspArrayVar
                        (
                            pVar, 
                            pHandleArray ? pHandleArray->VarCount : 0, 
                            BspArray_Handle
                        );

                if (status == ANSC_STATUS_SUCCESS)
                {
                    ULONG           ulSize  = pHandleArray ? pHandleArray->VarCount : 0;
                    pVars   = (PBSP_TEMPLATE_VAR_OBJECT *)pVar->Value.arrayItems.hVars;

                    for (i = 0; i < ulSize; i ++)
                    {
                        pVars[i]->Type      = BspVar_UNumber;
                        pVars[i]->Value.num = (ULONG)pHandleArray->Array.arrayHandle[i];
                    }
                }
            }
            break;

        case    SLAP_VAR_SYNTAX_TYPE_objectArray:
            {
                PSLAP_OBJECT_ARRAY     pObjectArray;

                pObjectArray    = (PSLAP_OBJECT_ARRAY)pSlapVar->Variant.varObjectArray;
                status          = 
                    BspEng_PrepBspArrayVar
                        (
                            pVar, 
                            pObjectArray ? pObjectArray->VarCount : 0, 
                            BspArray_Handle
                        );

                if (status == ANSC_STATUS_SUCCESS)
                {
                    ULONG           ulSize  = pObjectArray ? pObjectArray->VarCount : 0;
                    pVars   = (PBSP_TEMPLATE_VAR_OBJECT *)pVar->Value.arrayItems.hVars;

                    for (i = 0; i < ulSize; i ++)
                    {
                        pVars[i]->Type      = BspVar_UNumber;
                        pVars[i]->Value.num = (ULONG)pObjectArray->Array.arrayObject[i];
                    }
                }
            }
            break;

        default:

            /* 
             * other than basic types that BSP engine supports,
             * we should save the slap variable as an object.
             */

            pVar->Type  = BspVar_Object;

            if ( (pSlapVar->Syntax == SLAP_VAR_SYNTAX_TYPE_handle ||
                  pSlapVar->Syntax == SLAP_VAR_SYNTAX_TYPE_object) &&
                  pSlapVar->Variant.varHandle == NULL )
            {
                pVar->Value.obj = (PVOID)NULL;
            }
            else
            {
        		PSLAP_VARIABLE      pSlapVarObj = NULL;

        		SlapAllocVariable(pSlapVarObj);
        		pVar->Value.obj = pSlapVarObj;

                if (pVar->Value.obj)
                {
                    SlapCloneVariable(pSlapVar, ((PSLAP_VARIABLE)pVar->Value.obj));
                }
            }

            break;
    }

    pVar->ContentType   = pSlapVar->ContentType;
    pVar->SlapSyntax    = pSlapVar->Syntax;

    return status;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BspEng_PrepBspArrayVar
            (
                ANSC_HANDLE                 hArrayVar,
                ULONG                       ulItems,
                BSP_TEMPLATE_ARRAY_TYPE     arrayItemArray
            )

    description:

        This function is called to prepare array variable.

    argument:   ANSC_HANDLE                 hArrayVar
                Handle to BSP array variable.

                ULONG                       ulItems
                The number of array items.

                BSP_TEMPLATE_ARRAY_TYPE     arrayItemArray
                The type of array items.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BspEng_PrepBspArrayVar
    (
        ANSC_HANDLE                 hArrayVar,
        ULONG                       ulItems,
        BSP_TEMPLATE_ARRAY_TYPE     arrayItemType
    )
{
    PBSP_TEMPLATE_VAR_OBJECT        pVar        = (PBSP_TEMPLATE_VAR_OBJECT)hArrayVar;
    ANSC_STATUS                     status      = ANSC_STATUS_SUCCESS;
    ANSC_HANDLE                     hBulkData   = (ANSC_HANDLE)NULL;
    ULONG                           ulTotalSize = 0;
    BOOL                            bBulkData   = FALSE;

    pVar->Reset((ANSC_HANDLE)pVar);

    switch (arrayItemType)
    {
        case    BspArray_Uchar:
        case    BspArray_Char:

            bBulkData   = TRUE;

            break;
        default:
            break;
    }

    if (bBulkData)
    {
        ulTotalSize = BspEng_VC_GetBulkDataByteSize(ulItems, arrayItemType);

        if (ulTotalSize != 0)
        {
            hBulkData   = (ANSC_HANDLE)AnscAllocateMemory(ulTotalSize);
        }

        pVar->Type                          = BspVar_Array;
        pVar->Value.arrayItems.ulLen        = ulItems;
        pVar->Value.arrayItems.ulSize       = ulItems;
        pVar->Value.arrayItems.numDims      = 1;
        pVar->Value.arrayItems.Type         = arrayItemType;
        pVar->Value.arrayItems.hBulkData    = hBulkData;
        pVar->Value.arrayItems.bBulkData    = TRUE;
    }
    else
    {
        PBSP_TEMPLATE_VAR_OBJECT    *pVars      = NULL;
        ULONG                       i;

        if (ulItems != 0)
        {
            pVars   = 
                (PBSP_TEMPLATE_VAR_OBJECT *)AnscAllocateMemory
                    (
                        ulItems * sizeof(PBSP_TEMPLATE_VAR_OBJECT)
                    );
        }

        if (!pVars && ulItems != 0)
        {
            status  = ANSC_STATUS_RESOURCES;
        }
        else
        {
            pVar->Value.arrayItems.ulSize   = ulItems;

            for (i = 0; i < ulItems; i ++)
            {
                pVars[i]    = 
                    (PBSP_TEMPLATE_VAR_OBJECT)CreateBspEngVarComponent(NULL, NULL, NULL);

                /* if array size is too big, performance is bad */
                /* pVars[i]    = (PBSP_TEMPLATE_VAR_OBJECT)pRt->AcquireVariable((ANSC_HANDLE)pRt); */
            }
        }

        if (status == ANSC_STATUS_SUCCESS)
        {
            pVar->Type                          = BspVar_Array;
            pVar->Value.arrayItems.ulSize       = ulItems;
            pVar->Value.arrayItems.ulLen        = ulItems;
            pVar->Value.arrayItems.numDims      = 1;
            pVar->Value.arrayItems.Type         = arrayItemType;
            pVar->Value.arrayItems.hVars        = (ANSC_HANDLE)pVars;
            pVar->Value.arrayItems.bBulkData    = FALSE;
        }
    }

    return status;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BspEng_VC_PrepareSlapParamList
            (
                ANSC_HANDLE                 hSlapParamList,
                ANSC_HANDLE                 hVars,
                ULONG                       ulVars
            )

    description:

        This function is called to convert BSP variables and
        compile them into SLAP parameter list.

    argument:   ANSC_HANDLE                 hSlapParamList
                Handle to SLAP parameter list.

                ANSC_HANDLE                 hVars
                Handle to BSP variables.

                ULONG                       ulVars
                The number of BSP variables to be converted.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BspEng_VC_PrepareSlapParamList
    (
        ANSC_HANDLE                 hSlapParamList,
        ANSC_HANDLE                 hVars,
        ULONG                       ulVars
    )
{
    PSLAP_PARAMETER_LIST            pSlapParamList  = (PSLAP_PARAMETER_LIST)hSlapParamList;
    PBSP_TEMPLATE_VAR_OBJECT        *pVars          = (PBSP_TEMPLATE_VAR_OBJECT *)hVars;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;
    ULONG                           i;
    PBSP_TEMPLATE_VAR_OBJECT        pVar;
    PSLAP_VARIABLE                  pSlapVar;

    /* convert input parameters (script variables) to SLAP parameter list */
    if (!pSlapParamList || !pVars)
    {
        return ANSC_STATUS_FAILURE;
    }

    SlapInitParamList(pSlapParamList);

    for (i = 0; i < ulVars; i ++)
    {
        pVar        = pVars[i];
        pSlapVar    = &pSlapParamList->ParamArray[i];

        BspEng_VC_Var2Slap((ANSC_HANDLE)pVar, (ANSC_HANDLE)pSlapVar);
    }

    pSlapParamList->ParamCount  = ulVars;

    return status;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BspEng_VC_DeleteSlapParamList
            (
                ANSC_HANDLE                 hSlapParamList
            )

    description:

        This function is called to clean up SLAP parameter list.

    argument:   ANSC_HANDLE                 hSlapParamList
                Handle to BSP variable.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BspEng_VC_DeleteSlapParamList
    (
        ANSC_HANDLE                 hSlapParamList
    )
{
    PSLAP_PARAMETER_LIST            pSlapParamList  = (PSLAP_PARAMETER_LIST)hSlapParamList;

    /* 
     * This function doesn't release the resource occupied by
     * pSlapParamList itself. And caller needs to free that if
     * it was dynamically allocated.
     */

    SlapCleanParamList(pSlapParamList);

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BspEng_PreparePropertyParamList
            (
                ANSC_HANDLE                 hRuntime,
                ANSC_HANDLE                 hSlapParamList,
                BOOL                        bGetProperty,
                PUCHAR                      pPropertyName,
                ANSC_HANDLE                 hPropertyValue
            )

    description:

        This function is called to prepare property parameter list.

    argument:   ANSC_HANDLE                 hRuntime
                Handle to runtime object.

                ANSC_HANDLE                 hSlapParamList
                Handle to SLAP parameter list.

                ANSC_HANDLE                 bGetProperty
                If Get or Set will be invoked.

                PUCHAR                      pPropertyName
                Property name.

                ANSC_HANDLE                 hPropertyValue
                The property value.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BspEng_PreparePropertyParamList
    (
        ANSC_HANDLE                 hRuntime,
        ANSC_HANDLE                 hSlapParamList,
        BOOL                        bGetProperty,
        PUCHAR                      pPropertyName,
        ANSC_HANDLE                 hPropertyValue      /* local script variable */
    )
{
    /*
     * This function is called to prepare input parameter list.
     */
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt             = (PBSP_TEMPLATE_RUNTIME_OBJECT)hRuntime;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;
    PBSP_TEMPLATE_VAR_OBJECT        pVars[2];
    ULONG                           ulVars          = bGetProperty?1:2;
    PBSP_TEMPLATE_VAR_OBJECT        pName;

    if (!pPropertyName || !hSlapParamList)
    {
        return ANSC_STATUS_BAD_PARAMETER;
    }

    /* pName   = CreateBspEngVarComponent(NULL, NULL, NULL); */
    pName   = (PBSP_TEMPLATE_VAR_OBJECT)pRt->AcquireVariable((ANSC_HANDLE)pRt);

    if (!pName)
    {
        return ANSC_STATUS_RESOURCES;
    }

    pName->Type         = BspVar_String;
    pName->Value.str    = (PCHAR)pPropertyName;
    pName->bTemp        = TRUE;

    pVars[0]            = pName;
    pVars[1]            = hPropertyValue;

    status  = BspEng_VC_PrepareSlapParamList(hSlapParamList, (ANSC_HANDLE)pVars, ulVars);

    /* pName->Remove((ANSC_HANDLE)pName); */
    pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pName);

    return status;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BspEng_GetSlapVarCompareValue
            (
                ANSC_HANDLE                 hSlapVar,
                ANSC_HANDLE                 hCompareValue
            )

    description:

        This function is called to get compariable value from
        SLAP object.

    argument:   ANSC_HANDLE                 hSlapVar
                Handle to BSP variable.

                ANSC_HANDLE                 hCompareValue
                Handle to compariable value.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BspEng_GetSlapVarCompareValue
    (
        ANSC_HANDLE                 hSlapVar,
        ANSC_HANDLE                 hCompareValue
    )
{
    PSLAP_VARIABLE                  pSlapVar    = (PSLAP_VARIABLE)hSlapVar;
    PBSPENG_SLAP_VAR_COMPARE_VALUE  pValue      = (PBSPENG_SLAP_VAR_COMPARE_VALUE)hCompareValue;
    ANSC_STATUS                     status      = ANSC_STATUS_SUCCESS;

    if (!pSlapVar)
    {
        pValue->ulType          = BSPENG_SLAP_VAR_COMPARE_VALUE_TYPE_UINT32;
        pValue->value.uintValue = 0;

        return ANSC_STATUS_SUCCESS;
    }

    pValue->ulType          = BSPENG_SLAP_VAR_COMPARE_VALUE_TYPE_HANDLE;
    pValue->value.objValue  = NULL;

    switch (pSlapVar->Syntax)
    {
        case    SLAP_VAR_SYNTAX_TYPE_bool:

            pValue->ulType              = BSPENG_SLAP_VAR_COMPARE_VALUE_TYPE_UINT32;
            pValue->value.uintValue     = (SLAP_UINT32)pSlapVar->Variant.varBool;

            break;

        case    SLAP_VAR_SYNTAX_TYPE_int:

            pValue->ulType              = BSPENG_SLAP_VAR_COMPARE_VALUE_TYPE_INT;
            pValue->value.intValue      = (BOOL)pSlapVar->Variant.varInt;

            break;

        case    SLAP_VAR_SYNTAX_TYPE_string:

            pValue->ulType              = BSPENG_SLAP_VAR_COMPARE_VALUE_TYPE_STRING;
            pValue->value.stringValue   = pSlapVar->Variant.varString;

            break;

        case    SLAP_VAR_SYNTAX_TYPE_uint32:

            pValue->ulType              = BSPENG_SLAP_VAR_COMPARE_VALUE_TYPE_UINT32;
            pValue->value.uintValue     = pSlapVar->Variant.varUint32;

            break;

        default:

            /* this covers handle/object/array */

            pValue->ulType              = BSPENG_SLAP_VAR_COMPARE_VALUE_TYPE_UINT32;
            pValue->value.objValue      = pSlapVar->Variant.varHandle;

            break;
    }

    return status;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BspEng_VC_VarArray2Slap
            (
                ANSC_HANDLE                 hVar,
                ANSC_HANDLE                 hSlapVar
            )

    description:

        This function is called to convert BSP variable array to SLAP
        variable.

    argument:   ANSC_HANDLE                 hVar
                Handle to BSP variable.

                ANSC_HANDLE                 hSlapVar
                Handle to SLAP variable.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BspEng_VC_VarArray2Slap
    (
        ANSC_HANDLE                 hVar,
        ANSC_HANDLE                 hSlapVar
    )
{
    PBSP_TEMPLATE_VAR_OBJECT        pVar        = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    PSLAP_VARIABLE                  pSlapVar    = (PSLAP_VARIABLE)hSlapVar;
    ANSC_STATUS                     status      = ANSC_STATUS_SUCCESS;
    PBSP_TEMPLATE_ARRAY_ITEMS       pArrayItems;
    PBSP_TEMPLATE_VAR_OBJECT        *pVars;
    ULONG                           ulLen, i;
    BSP_TEMPLATE_ARRAY_TYPE         Type;

    pArrayItems = &pVar->Value.arrayItems;
    Type        = pArrayItems->Type;
    ulLen       = pArrayItems->ulLen;
    pVars       = (PBSP_TEMPLATE_VAR_OBJECT *)pArrayItems->hVars;

    if (pArrayItems->Type == BspArray_Variant)
    {
        /*
         *  we got to play a trick here. Since BSP
         *  writer doesn't set the expected type
         *  for slap array, we have to guess what
         *  type we should go with. For simplicity
         *  we just use the type of the first 
         *  item in BSP array.
         */
        ULONG                       ulArrayType = pVar->Type;

        if ( pVars && pVars[0] )
        {
            ulArrayType = pVars[0]->Type;
        }

        switch (ulArrayType)
        {
            case    BspVar_String:

                Type    = BspArray_String;

                break;

            case    BspVar_UNumber:

                Type    = BspArray_Uint;

                break;


            case    BspVar_Number:

                Type    = BspArray_Int;

                break;

            case    BspVar_Object:

                Type    = BspArray_Handle;

                break;
        }
    }

    switch (Type)
    {
        case    BspArray_Uint:
            
            {
                PSLAP_UINT32_ARRAY  pUint32Array;
                ULONG               ulTotalSize;

                ulTotalSize     = sizeof(SLAP_VAR_ARRAY) + ulLen * sizeof(SLAP_UINT32);
                pUint32Array    = 
                    (PSLAP_UINT32_ARRAY)AnscAllocateMemory(ulTotalSize);

                if (!pUint32Array)
                {
                    status  = ANSC_STATUS_RESOURCES;
                }
                else
                {
                    pSlapVar->Syntax                    = SLAP_VAR_SYNTAX_TYPE_uint32Array;
                    pSlapVar->Variant.varUint32Array    = pUint32Array;

                    pUint32Array->Size      = ulTotalSize;
                    pUint32Array->VarCount  = ulLen;
                    pUint32Array->Syntax    = pSlapVar->Syntax;

                    for (i = 0; i < ulLen; i ++)
                    {
                        pUint32Array->Array.arrayUint32[i]  = (SLAP_UINT32)pVars[i]->Value.num;
                    }
                }
            }
            
            break;

        case    BspArray_Int:
            
            {
                PSLAP_INT_ARRAY     pIntArray;
                ULONG               ulTotalSize;

                ulTotalSize     = sizeof(SLAP_VAR_ARRAY) + ulLen * sizeof(SLAP_INT);
                pIntArray    = 
                    (PSLAP_INT_ARRAY)AnscAllocateMemory(ulTotalSize);

                if (!pIntArray)
                {
                    status  = ANSC_STATUS_RESOURCES;
                }
                else
                {
                    pSlapVar->Syntax                    = SLAP_VAR_SYNTAX_TYPE_intArray;
                    pSlapVar->Variant.varUint32Array    = pIntArray;

                    pIntArray->Size      = ulTotalSize;
                    pIntArray->VarCount  = ulLen;
                    pIntArray->Syntax    = pSlapVar->Syntax;

                    for (i = 0; i < ulLen; i ++)
                    {
                        pIntArray->Array.arrayInt[i]    = (SLAP_INT)pVars[i]->Value.num;
                    }
                }
            }
            
            break;

        case    BspArray_String:
            
            {
                PSLAP_STRING_ARRAY  pStringArray;
                ULONG               ulTotalSize;

                ulTotalSize     = sizeof(SLAP_VAR_ARRAY) + ulLen * sizeof(SLAP_STRING);
                pStringArray    = 
                    (PSLAP_STRING_ARRAY)AnscAllocateMemory(ulTotalSize);

                if (!pStringArray)
                {
                    status  = ANSC_STATUS_RESOURCES;
                }
                else
                {
                    pSlapVar->Syntax                    = SLAP_VAR_SYNTAX_TYPE_stringArray;
                    pSlapVar->Variant.varStringArray    = pStringArray;

                    pStringArray->Size      = ulTotalSize;
                    pStringArray->VarCount  = ulLen;
                    pStringArray->Syntax    = pSlapVar->Syntax;

                    for (i = 0; i < ulLen; i ++)
                    {
                        /*CID: 67191 Dereference after null check*/
                        if (pVars && pVars[i]->Type == BspVar_String)
                        {
                            pStringArray->Array.arrayString[i]  = 
                                (SLAP_STRING)AnscCloneString(pVars[i]->Value.str);
                        }
                        else
                        {
                            pStringArray->Array.arrayString[i]  = (SLAP_STRING)NULL;
                        }
                    }
                }
            }
            
            break;

        case    BspArray_Uchar:
            
            {
                PSLAP_UCHAR_ARRAY   pUcharArray;
                ULONG               ulTotalSize;

                ulTotalSize     = sizeof(SLAP_VAR_ARRAY) + ulLen * sizeof(SLAP_UCHAR);
                pUcharArray     = 
                    (PSLAP_UCHAR_ARRAY)AnscAllocateMemory(ulTotalSize);

                if (!pUcharArray)
                {
                    status  = ANSC_STATUS_RESOURCES;
                }
                else
                {
                    pSlapVar->Syntax                    = SLAP_VAR_SYNTAX_TYPE_ucharArray;
                    pSlapVar->Variant.varUcharArray     = pUcharArray;

                    pUcharArray->Size       = ulTotalSize;
                    pUcharArray->VarCount   = ulLen;
                    pUcharArray->Syntax     = pSlapVar->Syntax;

                    for (i = 0; i < ulLen; i ++)
                    {
                        pUcharArray->Array.arrayUchar[i]= (SLAP_UCHAR)pVars[i]->Value.num;
                    }
                }
            }
            
            break;

        case    BspArray_Char:
            
            {
                PSLAP_CHAR_ARRAY    pCharArray;
                ULONG               ulTotalSize;

                ulTotalSize     = sizeof(SLAP_VAR_ARRAY) + ulLen * sizeof(SLAP_CHAR);
                pCharArray      = 
                    (PSLAP_CHAR_ARRAY)AnscAllocateMemory(ulTotalSize);

                if (!pCharArray)
                {
                    status  = ANSC_STATUS_RESOURCES;
                }
                else
                {
                    pSlapVar->Syntax                    = SLAP_VAR_SYNTAX_TYPE_charArray;
                    pSlapVar->Variant.varCharArray      = pCharArray;

                    pCharArray->Size        = ulTotalSize;
                    pCharArray->VarCount    = ulLen;
                    pCharArray->Syntax     = pSlapVar->Syntax;

                    for (i = 0; i < ulLen; i ++)
                    {
                        pCharArray->Array.arrayChar[i]  = (SLAP_CHAR)pVars[i]->Value.num;
                    }
                }
            }
            
            break;

        case    BspArray_Bool:
            
            {
                PSLAP_BOOL_ARRAY    pBoolArray;
                ULONG               ulTotalSize;

                ulTotalSize     = sizeof(SLAP_VAR_ARRAY) + ulLen * sizeof(SLAP_BOOL);
                pBoolArray      = 
                    (PSLAP_BOOL_ARRAY)AnscAllocateMemory(ulTotalSize);

                if (!pBoolArray)
                {
                    status  = ANSC_STATUS_RESOURCES;
                }
                else
                {
                    pSlapVar->Syntax                    = SLAP_VAR_SYNTAX_TYPE_boolArray;
                    pSlapVar->Variant.varBoolArray      = pBoolArray;

                    pBoolArray->Size        = ulTotalSize;
                    pBoolArray->VarCount    = ulLen;
                    pBoolArray->Syntax      = pSlapVar->Syntax;

                    for (i = 0; i < ulLen; i ++)
                    {
                        pBoolArray->Array.arrayBool[i]= (SLAP_BOOL)pVars[i]->Value.obj;
                    }
                }
            }
            
            break;

        case    BspArray_Ptr:

            {
                PSLAP_PTR_ARRAY     pPtrArray;
                ULONG               ulTotalSize;

                ulTotalSize     = sizeof(SLAP_VAR_ARRAY) + ulLen * sizeof(SLAP_PTR);
                pPtrArray       = 
                    (PSLAP_PTR_ARRAY)AnscAllocateMemory(ulTotalSize);

                if (!pPtrArray)
                {
                    status  = ANSC_STATUS_RESOURCES;
                }
                else
                {
                    pSlapVar->Syntax                    = SLAP_VAR_SYNTAX_TYPE_ptrArray;
                    pSlapVar->Variant.varPtrArray       = pPtrArray;

                    pPtrArray->Size         = ulTotalSize;
                    pPtrArray->VarCount     = ulLen;
                    pPtrArray->Syntax       = pSlapVar->Syntax;

                    for (i = 0; i < ulLen; i ++)
                    {
                        pPtrArray->Array.arrayPtr[i]  = (SLAP_PTR)pVars[i]->Value.obj;
                    }
                }
            }

            break;


        case    BspArray_Handle:
            
            {
                PSLAP_HANDLE_ARRAY  pHandleArray;
                ULONG               ulTotalSize;

                ulTotalSize     = sizeof(SLAP_VAR_ARRAY) + ulLen * sizeof(SLAP_HANDLE);
                pHandleArray       = 
                    (PSLAP_HANDLE_ARRAY)AnscAllocateMemory(ulTotalSize);

                if (!pHandleArray)
                {
                    status  = ANSC_STATUS_RESOURCES;
                }
                else
                {
                    pSlapVar->Syntax                    = SLAP_VAR_SYNTAX_TYPE_handleArray;
                    pSlapVar->Variant.varHandleArray    = pHandleArray;

                    pHandleArray->Size      = ulTotalSize;
                    pHandleArray->VarCount  = ulLen;
                    pHandleArray->Syntax    = pSlapVar->Syntax;

                    for (i = 0; i < ulLen; i ++)
                    {
                        pHandleArray->Array.arrayHandle[i]  = (SLAP_HANDLE)pVars[i]->Value.obj;
                    }
                }
            }
            
            break;

        case    BspArray_Object:

            {
                PSLAP_OBJECT_ARRAY  pObjectArray;
                ULONG               ulTotalSize;

                ulTotalSize     = sizeof(SLAP_VAR_ARRAY) + ulLen * sizeof(SLAP_OBJECT);
                pObjectArray       = 
                    (PSLAP_OBJECT_ARRAY)AnscAllocateMemory(ulTotalSize);

                if (!pObjectArray)
                {
                    status  = ANSC_STATUS_RESOURCES;
                }
                else
                {
                    pSlapVar->Syntax                    = SLAP_VAR_SYNTAX_TYPE_objectArray;
                    pSlapVar->Variant.varObjectArray    = pObjectArray;

                    pObjectArray->Size      = ulTotalSize;
                    pObjectArray->VarCount  = ulLen;
                    pObjectArray->Syntax    = pSlapVar->Syntax;

                    for (i = 0; i < ulLen; i ++)
                    {
                        pObjectArray->Array.arrayObject[i]  = (SLAP_OBJECT)pVars[i]->Value.num;
                    }
                }
            }

            break;
        default:
	    break;
    }

    return status;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BspEng_VC_BulkData2Slap
            (
                ANSC_HANDLE                 hVar,
                ANSC_HANDLE                 hSlapVar
            )

    description:

        This function is called to convert bulk data to SLAP
        variable.

    argument:   ANSC_HANDLE                 hVar
                Handle to BSP variable.

                ANSC_HANDLE                 hSlapVar
                Handle to SLAP variable.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BspEng_VC_BulkData2Slap
    (
        ANSC_HANDLE                 hVar,
        ANSC_HANDLE                 hSlapVar
    )
{
    PBSP_TEMPLATE_VAR_OBJECT        pVar        = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    PSLAP_VARIABLE                  pSlapVar    = (PSLAP_VARIABLE)hSlapVar;
    ANSC_STATUS                     status      = ANSC_STATUS_SUCCESS;
    PBSP_TEMPLATE_ARRAY_ITEMS       pArrayItems;
    ULONG                           ulLen, i;
    BSP_TEMPLATE_ARRAY_TYPE         Type;

    pArrayItems = &pVar->Value.arrayItems;
    ulLen       = pArrayItems->ulLen;
    Type        = pArrayItems->Type;

    if (pArrayItems->Type == BspArray_Variant || (ulLen != 0 && !pArrayItems->hBulkData))
    {
        /* unknown type, no idea how conversion is doing */
        return ANSC_STATUS_UNAPPLICABLE;
    }

    switch (Type)
    {
        case    BspArray_Uint:
            
            {
                PSLAP_UINT32_ARRAY  pUint32Array;
                ULONG               ulTotalSize;
                SLAP_UINT32         *pBulkData  = (SLAP_UINT32 *)pArrayItems->hBulkData;

                ulTotalSize     = sizeof(SLAP_VAR_ARRAY) + ulLen * sizeof(SLAP_UINT32);
                pUint32Array    = (PSLAP_UINT32_ARRAY)AnscAllocateMemory(ulTotalSize);

                if (!pUint32Array)
                {
                    status  = ANSC_STATUS_RESOURCES;
                }
                else
                {
                    pSlapVar->Syntax                    = SLAP_VAR_SYNTAX_TYPE_uint32Array;
                    pSlapVar->Variant.varUint32Array    = pUint32Array;

                    pUint32Array->Size      = ulTotalSize;
                    pUint32Array->VarCount  = ulLen;
                    pUint32Array->Syntax    = pSlapVar->Syntax;

                    if (ulLen != 0)
                    {
                        AnscCopyMemory(pUint32Array->Array.arrayUint32, pBulkData, ulLen * sizeof(SLAP_UINT32));
                    }
                }
            }
            
            break;

        case    BspArray_Int:
            
            {
                PSLAP_INT_ARRAY     pIntArray;
                ULONG               ulTotalSize;
                SLAP_INT            *pBulkData  = (SLAP_INT *)pArrayItems->hBulkData;

                ulTotalSize     = sizeof(SLAP_VAR_ARRAY) + ulLen * sizeof(SLAP_INT);
                pIntArray       = (PSLAP_INT_ARRAY)AnscAllocateMemory(ulTotalSize);

                if (!pIntArray)
                {
                    status  = ANSC_STATUS_RESOURCES;
                }
                else
                {
                    pSlapVar->Syntax                    = SLAP_VAR_SYNTAX_TYPE_intArray;
                    pSlapVar->Variant.varUint32Array    = pIntArray;

                    pIntArray->Size      = ulTotalSize;
                    pIntArray->VarCount  = ulLen;
                    pIntArray->Syntax    = pSlapVar->Syntax;

                    if (ulLen != 0)
                    {
                        AnscCopyMemory(pIntArray->Array.arrayInt, pBulkData, ulLen * sizeof(SLAP_INT));
                    }
                }
            }
            
            break;

        case    BspArray_String:
            
            {
                PSLAP_STRING_ARRAY  pStringArray;
                ULONG               ulTotalSize;
                SLAP_STRING         *pBulkData  = (SLAP_STRING *)pArrayItems->hBulkData;

                ulTotalSize     = sizeof(SLAP_VAR_ARRAY) + ulLen * sizeof(SLAP_STRING);
                pStringArray    = (PSLAP_STRING_ARRAY)AnscAllocateMemory(ulTotalSize);

                if (!pStringArray)
                {
                    status  = ANSC_STATUS_RESOURCES;
                }
                else
                {
                    pSlapVar->Syntax                    = SLAP_VAR_SYNTAX_TYPE_stringArray;
                    pSlapVar->Variant.varStringArray    = pStringArray;

                    pStringArray->Size      = ulTotalSize;
                    pStringArray->VarCount  = ulLen;
                    pStringArray->Syntax    = pSlapVar->Syntax;

                    for (i = 0; i < ulLen; i ++)
                    {
                        pStringArray->Array.arrayString[i]  = (SLAP_STRING)AnscCloneString(pBulkData[i]);
                    }
                }
            }
            
            break;

        case    BspArray_Uchar:
            
            {
                PSLAP_UCHAR_ARRAY   pUcharArray;
                ULONG               ulTotalSize;
                SLAP_UCHAR          *pBulkData  = (SLAP_UCHAR *)pArrayItems->hBulkData;

                ulTotalSize     = sizeof(SLAP_VAR_ARRAY) + ulLen * sizeof(SLAP_UCHAR);
                pUcharArray     = (PSLAP_UCHAR_ARRAY)AnscAllocateMemory(ulTotalSize);

                if (!pUcharArray)
                {
                    status  = ANSC_STATUS_RESOURCES;
                }
                else
                {
                    pSlapVar->Syntax                    = SLAP_VAR_SYNTAX_TYPE_ucharArray;
                    pSlapVar->Variant.varUcharArray     = pUcharArray;

                    pUcharArray->Size       = ulTotalSize;
                    pUcharArray->VarCount   = ulLen;
                    pUcharArray->Syntax     = pSlapVar->Syntax;

                    if (ulLen != 0)
                    {
                        AnscCopyMemory(pUcharArray->Array.arrayUchar, pBulkData, ulLen * sizeof(SLAP_UCHAR));
                    }
                }
            }
            
            break;

        case    BspArray_Char:
            
            {
                PSLAP_CHAR_ARRAY    pCharArray;
                ULONG               ulTotalSize;
                SLAP_CHAR           *pBulkData  = (SLAP_CHAR *)pArrayItems->hBulkData;

                ulTotalSize     = sizeof(SLAP_VAR_ARRAY) + ulLen * sizeof(SLAP_CHAR);
                pCharArray      = (PSLAP_CHAR_ARRAY)AnscAllocateMemory(ulTotalSize);

                if (!pCharArray)
                {
                    status  = ANSC_STATUS_RESOURCES;
                }
                else
                {
                    pSlapVar->Syntax                    = SLAP_VAR_SYNTAX_TYPE_charArray;
                    pSlapVar->Variant.varCharArray      = pCharArray;

                    pCharArray->Size        = ulTotalSize;
                    pCharArray->VarCount    = ulLen;
                    pCharArray->Syntax     = pSlapVar->Syntax;

                    if (ulLen != 0)
                    {
                        AnscCopyMemory(pCharArray->Array.arrayChar, pBulkData, ulLen * sizeof(SLAP_CHAR));
                    }
                }
            }
            
            break;

        case    BspArray_Bool:
            
            {
                PSLAP_BOOL_ARRAY    pBoolArray;
                ULONG               ulTotalSize;
                SLAP_BOOL           *pBulkData  = (SLAP_BOOL *)pArrayItems->hBulkData;

                ulTotalSize     = sizeof(SLAP_VAR_ARRAY) + ulLen * sizeof(SLAP_BOOL);
                pBoolArray      = (PSLAP_BOOL_ARRAY)AnscAllocateMemory(ulTotalSize);

                if (!pBoolArray)
                {
                    status  = ANSC_STATUS_RESOURCES;
                }
                else
                {
                    pSlapVar->Syntax                    = SLAP_VAR_SYNTAX_TYPE_boolArray;
                    pSlapVar->Variant.varBoolArray      = pBoolArray;

                    pBoolArray->Size        = ulTotalSize;
                    pBoolArray->VarCount    = ulLen;
                    pBoolArray->Syntax      = pSlapVar->Syntax;

                    if (ulLen != 0)
                    {
                        AnscCopyMemory(pBoolArray->Array.arrayBool, pBulkData, ulLen * sizeof(SLAP_BOOL));
                    }
                }
            }
            
            break;

        case    BspArray_Ptr:

            {
                PSLAP_PTR_ARRAY     pPtrArray;
                ULONG               ulTotalSize;
                SLAP_PTR            *pBulkData  = (SLAP_PTR *)pArrayItems->hBulkData;

                ulTotalSize     = sizeof(SLAP_VAR_ARRAY) + ulLen * sizeof(SLAP_PTR);
                pPtrArray       = (PSLAP_PTR_ARRAY)AnscAllocateMemory(ulTotalSize);

                if (!pPtrArray)
                {
                    status  = ANSC_STATUS_RESOURCES;
                }
                else
                {
                    pSlapVar->Syntax                    = SLAP_VAR_SYNTAX_TYPE_ptrArray;
                    pSlapVar->Variant.varPtrArray       = pPtrArray;

                    pPtrArray->Size         = ulTotalSize;
                    pPtrArray->VarCount     = ulLen;
                    pPtrArray->Syntax       = pSlapVar->Syntax;

                    if (ulLen != 0)
                    {
                        AnscCopyMemory(pPtrArray->Array.arrayPtr, pBulkData, ulLen * sizeof(SLAP_PTR));
                    }
                }
            }

            break;


        case    BspArray_Handle:
            
            {
                PSLAP_HANDLE_ARRAY  pHandleArray;
                ULONG               ulTotalSize;
                SLAP_HANDLE         *pBulkData  = (SLAP_HANDLE *)pArrayItems->hBulkData;

                ulTotalSize     = sizeof(SLAP_VAR_ARRAY) + ulLen * sizeof(SLAP_HANDLE);
                pHandleArray    = (PSLAP_HANDLE_ARRAY)AnscAllocateMemory(ulTotalSize);

                if (!pHandleArray)
                {
                    status  = ANSC_STATUS_RESOURCES;
                }
                else
                {
                    pSlapVar->Syntax                    = SLAP_VAR_SYNTAX_TYPE_handleArray;
                    pSlapVar->Variant.varHandleArray    = pHandleArray;

                    pHandleArray->Size      = ulTotalSize;
                    pHandleArray->VarCount  = ulLen;
                    pHandleArray->Syntax    = pSlapVar->Syntax;

                    if (ulLen != 0)
                    {
                        AnscCopyMemory(pHandleArray->Array.arrayHandle, pBulkData, ulLen * sizeof(SLAP_UINT32));
                    }
                }
            }
            
            break;

        case    BspArray_Object:

            {
                PSLAP_OBJECT_ARRAY  pObjectArray;
                ULONG               ulTotalSize;
                SLAP_OBJECT         *pBulkData  = (SLAP_OBJECT)pArrayItems->hBulkData;

                ulTotalSize     = sizeof(SLAP_VAR_ARRAY) + ulLen * sizeof(SLAP_OBJECT);
                pObjectArray    = (PSLAP_OBJECT_ARRAY)AnscAllocateMemory(ulTotalSize);

                if (!pObjectArray)
                {
                    status  = ANSC_STATUS_RESOURCES;
                }
                else
                {
                    pSlapVar->Syntax                    = SLAP_VAR_SYNTAX_TYPE_objectArray;
                    pSlapVar->Variant.varObjectArray    = pObjectArray;

                    pObjectArray->Size      = ulTotalSize;
                    pObjectArray->VarCount  = ulLen;
                    pObjectArray->Syntax    = pSlapVar->Syntax;

                    if (ulLen != 0)
                    {
                        AnscCopyMemory(pObjectArray->Array.arrayObject, pBulkData, ulLen * sizeof(SLAP_OBJECT));
                    }
                }
            }

            break;

        default:

            status  = ANSC_STATUS_UNAPPLICABLE;

            break;
    }

    return status;
}


ULONG
BspEng_VC_GetBulkDataByteSize
    (
        ULONG                       ulItems,
        BSP_TEMPLATE_ARRAY_TYPE     arrayItemType
    )
{
    ULONG                           ulTotalSize = 0;

    switch (arrayItemType)
    {
        case    BspArray_Uint:

            ulTotalSize = ulItems * sizeof(SLAP_UINT32);

            break;

        case    BspArray_Int:

            ulTotalSize = ulItems * sizeof(SLAP_INT);

            break;

        case    BspArray_String:

            ulTotalSize = ulItems * sizeof(SLAP_STRING);

            break;

        case    BspArray_Uchar:

            ulTotalSize = ulItems * sizeof(SLAP_UCHAR);

            break;

        case    BspArray_Char:

            ulTotalSize = ulItems * sizeof(SLAP_CHAR);

            break;

        case    BspArray_Bool:

            ulTotalSize = ulItems * sizeof(SLAP_BOOL);

            break;

        case    BspArray_Ptr:

            ulTotalSize = ulItems * sizeof(SLAP_PTR);

            break;

        case    BspArray_Handle:

            ulTotalSize = ulItems * sizeof(SLAP_HANDLE);

            break;

        case    BspArray_Object:

            ulTotalSize = ulItems * sizeof(SLAP_OBJECT);

            break;

        default:
            break;
    }

    return ulTotalSize;
}


