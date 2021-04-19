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

    module:	bspeng_var_co_process.c

        For BSP Engine Var Component Object,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the BSP engine var Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        05/14/03    initial revision.

    ---------------------------------------------------------------

    last modified:  

**********************************************************************/


#include "bspeng_co_global.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateVarIncrease
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to increase variable by one.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle to this object.

**********************************************************************/

ANSC_HANDLE
BspTemplateVarIncrease
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_VAR_OBJECT        pMyObject = (PBSP_TEMPLATE_VAR_OBJECT)hThisObject;

    switch (pMyObject->Type)
    {
    case BspVar_String:
        pMyObject->Value.num    = pMyObject->StrToNum(hThisObject);
        pMyObject->Type         = BspVar_Number;
        /* fall through */

    case BspVar_Number:
    case BspVar_UNumber:
        pMyObject->Value.num ++;
        break;

    case BspVar_Real:
#ifdef   _BSPENG_NO_DOUBLE
        pMyObject->Value.real += BSP_TEMPLATE_DOUBLE_SCALE;
#else
        pMyObject->Value.real ++;
#endif
        break;

    default:
        AnscTrace("Can't perform increment\r\n");
        break;
    }

    return hThisObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateVarDecrease
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to decrease variable by one.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle to this object.

**********************************************************************/

ANSC_HANDLE
BspTemplateVarDecrease
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_VAR_OBJECT            pMyObject = (PBSP_TEMPLATE_VAR_OBJECT)hThisObject;

    switch (pMyObject->Type)
    {
    case BspVar_String:
        pMyObject->Value.num    = pMyObject->StrToNum(hThisObject);
        pMyObject->Type         = BspVar_Number;
        /* fall through */

    case BspVar_Number:
    case BspVar_UNumber:
        pMyObject->Value.num --;
        break;

    case BspVar_Real:
#ifdef   _BSPENG_NO_DOUBLE
        pMyObject->Value.real -= BSP_TEMPLATE_DOUBLE_SCALE;
#else
        pMyObject->Value.real --;
#endif
        break;

    default:
        AnscTrace("Can't perform decrement\r\n");
        break;
    }

    return hThisObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG
        BspTemplateVarGetLong
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to get LONG value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     long value of the variable.

**********************************************************************/

LONG
BspTemplateVarGetLong
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_VAR_OBJECT        pMyObject = (PBSP_TEMPLATE_VAR_OBJECT)hThisObject;

    switch (pMyObject->Type)
    {
    case BspVar_String:
        if (pMyObject->Value.str)
            return AnscSizeOfString(pMyObject->Value.str);
        else
            return 0;
        break;

    case BspVar_Number:
    case BspVar_UNumber:
        return pMyObject->Value.num;
        break;

    case BspVar_Real:
#ifdef   _BSPENG_NO_DOUBLE
        return (LONG)BSP_TEMPLATE_DOUBLE_GET_INT(pMyObject->Value.real);
#else
        return (LONG)pMyObject->Value.real;
#endif
        break;

    case BspVar_Object:

        return (LONG)pMyObject->Value.obj;

        break;

    default:
        AnscTrace("Can't convert this value to long\r\n");
        break;
    }

    return 0;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateVarAssign
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to do variable assignment.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hVar
                The variable from which the value comes from.

    return:     handle to this object.

**********************************************************************/

ANSC_HANDLE
BspTemplateVarAssign
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_VAR_OBJECT        pMyObject = (PBSP_TEMPLATE_VAR_OBJECT)hThisObject;
    PBSP_TEMPLATE_VAR_OBJECT        pVar      = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    /*PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)pMyObject->hRuntime;*/
    BOOL                            bOK       = TRUE;

    if (!pVar)
    {
        /*
         * In case array access is out of boundary
         */

        pMyObject->Reset((ANSC_HANDLE)pMyObject);

        return hThisObject;
    }

	/* reset this object */
/*    if (pMyObject->Type != BspVar_Array || pMyObject->bTemp) */
    {
	    pMyObject->Reset((ANSC_HANDLE)pMyObject);
    }

    pMyObject->SlapSyntax   = pVar->SlapSyntax;
    pMyObject->ContentType  = pVar->ContentType;

    switch (pVar->Type)
    {
    case BspVar_String:
        if (pVar->bTemp || pVar->Size == 0)
        {
            if (pVar->bTemp)
            {
                pMyObject->Value.str    = pVar->Value.str;
                pMyObject->Size         = pVar->Size;
                pVar->Size              = 0;
            }
            else
            {
                pMyObject->Value.str    = AnscCloneString(pVar->Value.str);
                pMyObject->Size         = pVar->Size;
                pVar->Size              = 0;

                ACCESS_BSP_TEMPLATE_VAR_CLEAR_TEMP_FLAG(pMyObject);
            }
        }
        else
        {
            ULONG                   ulBufLen    = 0;

            if (pVar->Size == 0)
                ulBufLen    = AnscSizeOfString(pVar->Value.str) + 1;
            else
                ulBufLen    = pVar->Size + 1;

            pMyObject->Value.str    = (char *)AnscAllocateMemory(ulBufLen);
            if (pMyObject->Value.str)
            {
                AnscCopyMemory(pMyObject->Value.str, pVar->Value.str, pVar->Size);
                pMyObject->Value.str[ulBufLen - 1]   = 0;
                pMyObject->Size         = pVar->Size;
            }
            else
            {
                pMyObject->Size         = 0;
            }
            ACCESS_BSP_TEMPLATE_VAR_CLEAR_TEMP_FLAG(pMyObject);
        }
        break;

    case BspVar_Number:
    case BspVar_UNumber:
        pMyObject->Value.num    = pVar->Value.num;
        pVar->Size              = 0;
        break;

    case BspVar_Real:
        pMyObject->Value.real   = pVar->Value.real;
        pVar->Size              = 0;
        break;

    case BspVar_Object:
        pVar->Size              = 0;

        if (bOK && pVar->Value.obj)
        {
    	    PSLAP_VARIABLE          pSlapVarObj;

            SlapAllocVariable(pSlapVarObj);
	        pMyObject->Value.obj = pSlapVarObj;            

            if (pMyObject->Value.obj)
            {
                SlapCloneVariable(((PSLAP_VARIABLE)pVar->Value.obj), ((PSLAP_VARIABLE)pMyObject->Value.obj));
            }
        }
        break;

    case BspVar_Array:
        /* if (pMyObject->Type != BspVar_Array) */
        {
            /*
             *  we actually make a copy of the array, so the new variable
             *  doesnot share array items with another one. Since this
             *  variable is not declared as "array", we can set the
             *  bTemp flag as TRUE to indicate that this variable could be
             *  assigned to value with other type later on.
             */
            pMyObject->Type     = BspVar_Array;
            pMyObject->bTemp    = TRUE;
            pMyObject->Value.arrayItems = pVar->Value.arrayItems;

            pMyObject->Value.arrayItems.hVars   = NULL;
            pMyObject->Value.arrayItems.ulSize  = 0;
            pMyObject->Value.arrayItems.ulLen   = 0;
        }

        if (TRUE)
        {
            BOOL                    bCanAssign  = FALSE;
            ULONG                   i, ulDims;

            /*
             *  Check if such array assignment is allowed. 
             *  They must have the same dimensions except the first one.
             *  Of course the number of dimensions must also be the same.
             */
            if (pMyObject->Value.arrayItems.numDims == pVar->Value.arrayItems.numDims)
            {
                bCanAssign  = TRUE;

                ulDims      = pMyObject->Value.arrayItems.numDims;
                
                for (i = 1; i < ulDims; i ++)
                {
                    if (pMyObject->Value.arrayItems.Dims[i] != pVar->Value.arrayItems.Dims[i])
                    {
                        bCanAssign  = FALSE;
                        break;
                    }
                }
            }

            if (bCanAssign && !pVar->Value.arrayItems.bBulkData)
            {
                ULONG                       ulNewItems;
                ULONG                       ulItemsNow;
                PBSP_TEMPLATE_VAR_OBJECT    *pVars, *pVarsSrc;

                pVars       = (PBSP_TEMPLATE_VAR_OBJECT *)pMyObject->Value.arrayItems.hVars;
                pVarsSrc    = (PBSP_TEMPLATE_VAR_OBJECT *)pVar->Value.arrayItems.hVars;

                ulItemsNow  = pMyObject->Value.arrayItems.ulSize;
                for (i = 1; i < ulDims; i ++)
                {
                    ulItemsNow  *= pMyObject->Value.arrayItems.Dims[i];
                }

                ulNewItems  = pVar->Value.arrayItems.ulLen;

                for (i = 1; i < ulDims; i ++)
                {
                    ulNewItems *= pVar->Value.arrayItems.Dims[i];
                }

                /*
                 *  enlarge the array if neccessary
                 */
                if (pMyObject->Value.arrayItems.ulSize < pVar->Value.arrayItems.ulLen)
                {
                    PBSP_TEMPLATE_VAR_OBJECT    *pNewItems;

                    pNewItems   = 
                        (PBSP_TEMPLATE_VAR_OBJECT *)AnscAllocateMemory
                            (
                                ulNewItems * sizeof(PBSP_TEMPLATE_VAR_OBJECT)
                            );

                    if (!pNewItems)
                    {
                        ulNewItems  = 0;
                        bOK         = FALSE;
                    }
                    else
                    {
                        /*CID: 56594 Explicit null dereferenced*/
                        if ( pMyObject->Value.arrayItems.hVars ) 
                        {
                            AnscCopyMemory
                            (
                                pNewItems, 
                                pMyObject->Value.arrayItems.hVars, 
                                ulItemsNow * sizeof(PBSP_TEMPLATE_VAR_OBJECT)
                            );

                            AnscFreeMemory(pMyObject->Value.arrayItems.hVars);
                        }
                        for (i = ulItemsNow; i < ulNewItems; i ++)
                        {
                            pNewItems[i]    = 
                                (PBSP_TEMPLATE_VAR_OBJECT)CreateBspEngVarComponent(NULL, NULL, NULL);

                            /* if array size is too big, performance is bad */
                            /* pNewItems[i]    = (PBSP_TEMPLATE_VAR_OBJECT)pRt->AcquireVariable((ANSC_HANDLE)pRt); */
                        }

                        pMyObject->Value.arrayItems.hVars   = (ANSC_HANDLE)pNewItems;

                        pMyObject->Value.arrayItems.ulSize  = ulNewItems;

                        pVars   = pNewItems;
                    }
                }

                /* copy the values */
                /*CID:60250 Explicit null dereferenced*/
                if (pVars) {
                   for (i = 0; i < ulNewItems; i ++)
                   {
                     pVars[i]->Assign((ANSC_HANDLE)pVars[i], (ANSC_HANDLE)pVarsSrc[i]);
                   }
                }
                if (ulNewItems != 0)
                {
                    pMyObject->Value.arrayItems.ulLen   = pVar->Value.arrayItems.ulLen;
                }

                pMyObject->Value.arrayItems.Type    = pVar->Value.arrayItems.Type;

                break;
            }
            else
            {
                ANSC_STATUS                 status;
                ULONG                       ulTotalSize;
                BSP_TEMPLATE_ARRAY_TYPE     Type;
                PBSP_TEMPLATE_ARRAY_ITEMS   pFromArray, pToArray;

                pMyObject->Reset((ANSC_HANDLE)pMyObject);

                Type        = pVar->Value.arrayItems.Type;

                pFromArray  = &pVar->Value.arrayItems;

                status  = 
                    BspEng_PrepBspArrayVar
                        (
                            (ANSC_HANDLE)pMyObject, 
                            pFromArray->ulLen, 
                            Type
                        );

                if (status != ANSC_STATUS_SUCCESS)
                {
                    bOK = FALSE;
                }
                else
                {
                    pToArray    = &pMyObject->Value.arrayItems;

                    ulTotalSize = 
                        BspEng_VC_GetBulkDataByteSize
                            (
                                pFromArray->ulLen, 
                                Type
                            );

                    switch (Type)
                    {
                        case    BspArray_String:

                            {
                                ULONG       i;
                                SLAP_STRING *pFromStrings, *pToStrings;

                                pFromStrings    = (SLAP_STRING *)pFromArray->hBulkData;
                                pToStrings      = (SLAP_STRING *)pToArray->hBulkData;

                                for (i = 0; i < pFromArray->ulLen; i ++)
                                {
                                    pToStrings[i]   = AnscCloneString(pFromStrings[i]);
                                }
                            }

                            break;

                        default:
                        
                            if (ulTotalSize != 0)
                            {
                                AnscCopyMemory(pToArray->hBulkData, pFromArray->hBulkData, ulTotalSize);
                            }

                            break;
                    }

                    break;
                }
            }
        }
        /* fall through */

    default:
        AnscTrace("Can't perform assignment\r\n");
        bOK = FALSE;
        break;
    }

    if (bOK)
    {
        pMyObject->Type = pVar->Type;
    }

    return hThisObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateVarCreater
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to examine if this variable is
        GREATER than the specified variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hVar
                The variable to be compared.

    return:     comparison result.

**********************************************************************/

BOOL
BspTemplateVarGreater
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_VAR_OBJECT        pMyObject = (PBSP_TEMPLATE_VAR_OBJECT)hThisObject;
    PBSP_TEMPLATE_VAR_OBJECT        pVar      = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    BSPENG_SLAP_VAR_COMPARE_VALUE   compVal;

    switch (pMyObject->Type)
    {
    case BspVar_String:
        switch (pVar->Type)
        {
        case BspVar_String:
            return _ansc_strcmp(pMyObject->Value.str, pVar->Value.str) > 0;
            break;

        case BspVar_Number:
        case BspVar_UNumber:
            {
                ULONG               ulLen   = 0;

                if (pMyObject->Value.str)
                {
                    if (pMyObject->Size)
                        ulLen   = pMyObject->Size;
                    else
                        ulLen   = AnscSizeOfString(pMyObject->Value.str);
                }

                return (LONG)ulLen > (LONG)pVar->Value.num;
            }
            break;

        case BspVar_Real:
            {
                ULONG               ulLen   = 0;

                if (pMyObject->Value.str)
                {
                    if (pMyObject->Size)
                        ulLen   = pMyObject->Size;
                    else
                        ulLen   = AnscSizeOfString(pMyObject->Value.str);
                }

#ifdef   _BSPENG_NO_DOUBLE
                return (LONG)ulLen > BSP_TEMPLATE_DOUBLE_GET_INT(pVar->Value.real);
#else
                return (double)ulLen > pVar->Value.real;
#endif
            }
            break;
        default:
            break;
        }
        break;

    case BspVar_Number:
    case BspVar_UNumber:
        switch (pVar->Type)
        {
        case BspVar_String:
            {
                ULONG               ulLen   = 0;

                if (pVar->Value.str)
                {
                    if (pVar->Size)
                        ulLen   = pVar->Size;
                    else
                        ulLen   = AnscSizeOfString(pVar->Value.str);
                }

                return (LONG)pMyObject->Value.num > (LONG)ulLen;
            }
            break;

        case BspVar_Number:
        case BspVar_UNumber:
            return pMyObject->Value.num > pVar->Value.num;
            break;

        case BspVar_Real:
#ifdef   _BSPENG_NO_DOUBLE
            return pMyObject->Value.num > (LONG)BSP_TEMPLATE_DOUBLE_GET_INT(pVar->Value.real);
#else
            return (double)pMyObject->Value.num > pVar->Value.real;
#endif
            break;

        case BspVar_Object:
            BspEng_GetSlapVarCompareValue((ANSC_HANDLE)pVar->Value.obj, (ANSC_HANDLE)&compVal);
            return (ULONG)pMyObject->Value.num > (ULONG)compVal.value.objValue;
            break;
        default:
            break;
        }
        break;

    case BspVar_Real:
        switch (pVar->Type)
        {
        case BspVar_String:
            {
                ULONG               ulLen   = 0;

                if (pVar->Value.str)
                {
                    if (pVar->Size)
                        ulLen   = pVar->Size;
                    else
                        ulLen   = AnscSizeOfString(pVar->Value.str);
                }

#ifdef   _BSPENG_NO_DOUBLE
                {
                    LONG            lInt;
                    ULONG           ulFrac;

                    lInt    = BSP_TEMPLATE_DOUBLE_GET_INT(pMyObject->Value.real);
                    ulFrac  = BSP_TEMPLATE_DOUBLE_GET_FRAC(pMyObject->Value.real);

                    if (lInt > (LONG)ulLen)
                        return TRUE;

                    if (lInt == (LONG)ulLen)
                    {
                        return ulFrac != 0;
                    }

                    return FALSE;
                }
#else
                return pMyObject->Value.real > (double)ulLen;
#endif
            }
            break;

        case BspVar_Number:
        case BspVar_UNumber:
#ifdef   _BSPENG_NO_DOUBLE
            {
                LONG            lInt;
                ULONG           ulFrac;

                lInt    = BSP_TEMPLATE_DOUBLE_GET_INT(pMyObject->Value.real);
                ulFrac  = BSP_TEMPLATE_DOUBLE_GET_FRAC(pMyObject->Value.real);

                if (lInt > (LONG)pVar->Value.num)
                    return TRUE;

                if (lInt == (LONG)pVar->Value.num)
                {
                    return ulFrac != 0;
                }

                return FALSE;
            }
#else
            return pMyObject->Value.real > (double)pVar->Value.num;
#endif
            break;

        case BspVar_Real:
            return pMyObject->Value.real > pVar->Value.real;
            break;
        default:
            break;
        }
        break;
    
    case BspVar_Object:
        switch (pVar->Type)
        {
        case BspVar_Number:
        case BspVar_UNumber:
                BspEng_GetSlapVarCompareValue((ANSC_HANDLE)pMyObject->Value.obj, (ANSC_HANDLE)&compVal);
                return 
                    ((compVal.ulType == BSPENG_SLAP_VAR_COMPARE_VALUE_TYPE_INT) || 
                    (compVal.ulType == BSPENG_SLAP_VAR_COMPARE_VALUE_TYPE_UINT32)) &&
                    ((ULONG)compVal.value.objValue > (ULONG)pVar->Value.num);
            break;

        case BspVar_Object:
            {
                ULONG               comp1, comp2;

                BspEng_GetSlapVarCompareValue((ANSC_HANDLE)pMyObject->Value.obj, (ANSC_HANDLE)&compVal);
                comp1 = (ULONG)compVal.value.objValue;

                BspEng_GetSlapVarCompareValue((ANSC_HANDLE)pVar->Value.obj, (ANSC_HANDLE)&compVal);
                comp2 = (ULONG)compVal.value.objValue;

                return comp1 > comp2;
            }
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    AnscTrace("Can't perform comparison\r\n");

    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateVarIncrease
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to examine if this variable is
        SMALLER than the specified one.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hVar

    return:     comparison result.

**********************************************************************/

BOOL
BspTemplateVarSmaller
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_VAR_OBJECT        pVar      = (PBSP_TEMPLATE_VAR_OBJECT)hVar;

    return pVar->Greater(hVar, hThisObject);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateVarIncrease
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to examine if this variable equals 
        to the specified one.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hVar

    return:     comparison result.

**********************************************************************/

BOOL
BspTemplateVarEqual
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_VAR_OBJECT        pMyObject = (PBSP_TEMPLATE_VAR_OBJECT)hThisObject;
    PBSP_TEMPLATE_VAR_OBJECT        pVar      = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    BSPENG_SLAP_VAR_COMPARE_VALUE   compVal;

    switch (pMyObject->Type)
    {
    case BspVar_String:
        switch (pVar->Type)
        {
        case BspVar_String:
            {
                if (!pMyObject->Value.str && !pVar->Value.str)
                    return TRUE;
                else
                    if (pMyObject->Value.str && pVar->Value.str)
                    {
                        return 
                            ( strcmp(pMyObject->Value.str, pVar->Value.str) == 0 ); 
                    }
                    else
                    {
                        ULONG       ulLen1  = 0;
                        ULONG       ulLen2  = 0;

                        if (pMyObject->Value.str)
                        {
                            if (pMyObject->Size)
                                ulLen1  = pMyObject->Size;
                            else
                                ulLen1  = AnscSizeOfString(pMyObject->Value.str);
                        }

                        if (pVar->Value.str)
                        {
                            if (pVar->Size)
                                ulLen2  = pVar->Size;
                            else
                                ulLen2  = AnscSizeOfString(pVar->Value.str);
                        }

                        return (ulLen1 == ulLen2);
                    }
            }
            break;

        case BspVar_Number:
        case BspVar_UNumber:
            {
                ULONG               ulLen   = 0;

                if (pMyObject->Value.str)
                {
                    if (pMyObject->Size)
                        ulLen   = pMyObject->Size;
                    else
                        ulLen   = AnscSizeOfString(pMyObject->Value.str);
                }

                return (LONG)ulLen == pVar->Value.num;
            }
            break;

        case BspVar_Real:
            {
                ULONG               ulLen;
                
                if (pMyObject->Size)
                    ulLen   = pMyObject->Size;
                else
                    ulLen   = AnscSizeOfString(pMyObject->Value.str);

#ifdef   _BSPENG_NO_DOUBLE
                {
                    LONG            lInt;
                    ULONG           ulFrac;

                    lInt    = BSP_TEMPLATE_DOUBLE_GET_INT(pVar->Value.real);
                    ulFrac  = BSP_TEMPLATE_DOUBLE_GET_FRAC(pVar->Value.real);

                    return (lInt == (LONG)ulLen && ulFrac == 0);
                }
#else
                return (LONG)ulLen == pVar->Value.real;
#endif
            }
            break;

            case BspVar_Object:
                BspEng_GetSlapVarCompareValue((ANSC_HANDLE)pVar->Value.obj, (ANSC_HANDLE)&compVal);
                return 
                    (compVal.ulType == BSPENG_SLAP_VAR_COMPARE_VALUE_TYPE_STRING) && 
                    (strcmp(compVal.value.stringValue, pMyObject->Value.str) == 0 );
                break;
        default:
            break;
        }
        break;

    case BspVar_Number:
    case BspVar_UNumber:
        switch (pVar->Type)
        {
        case BspVar_String:
            {
                ULONG               ulLen   = 0;

                if (pVar->Value.str)
                {
                    if (pVar->Size)
                        ulLen   = pVar->Size;
                    else
                        ulLen   = AnscSizeOfString(pVar->Value.str);
                }

                return pMyObject->Value.num == (LONG)ulLen;
            }
            break;

        case BspVar_Number:
        case BspVar_UNumber:
            return pMyObject->Value.num == pVar->Value.num;
            break;

        case BspVar_Real:
#ifdef   _BSPENG_NO_DOUBLE
            {
                LONG            lInt;
                ULONG           ulFrac;

                lInt    = BSP_TEMPLATE_DOUBLE_GET_INT(pVar->Value.real);
                ulFrac  = BSP_TEMPLATE_DOUBLE_GET_FRAC(pVar->Value.real);

                return ((LONG)pMyObject->Value.num == lInt && ulFrac == 0);
            }
#else
            return (double)pMyObject->Value.num == pVar->Value.real;
#endif
            break;

        case BspVar_Object:
            BspEng_GetSlapVarCompareValue((ANSC_HANDLE)pVar->Value.obj, (ANSC_HANDLE)&compVal);
            return (ULONG)pMyObject->Value.num == (ULONG)compVal.value.objValue;
            break;
        default:
            break;
        }
        break;

    case BspVar_Real:
        switch (pVar->Type)
        {
        case BspVar_String:
            {
                ULONG               ulLen   = 0;

                if (pVar->Value.str)
                {
                    if (pVar->Size)
                        ulLen   = pVar->Size;
                    else
                        ulLen   = AnscSizeOfString(pVar->Value.str);
                }

#ifdef   _BSPENG_NO_DOUBLE
            {
                LONG            lInt;
                ULONG           ulFrac;

                lInt    = BSP_TEMPLATE_DOUBLE_GET_INT(pMyObject->Value.real);
                ulFrac  = BSP_TEMPLATE_DOUBLE_GET_FRAC(pMyObject->Value.real);

                return ((LONG)ulLen == lInt && ulFrac == 0);
            }
#else
                return pMyObject->Value.real == (LONG)ulLen;
#endif
            }
            break;

        case BspVar_Number:
        case BspVar_UNumber:
#ifdef   _BSPENG_NO_DOUBLE
            {
                LONG            lInt;
                ULONG           ulFrac;

                lInt    = BSP_TEMPLATE_DOUBLE_GET_INT(pMyObject->Value.real);
                ulFrac  = BSP_TEMPLATE_DOUBLE_GET_FRAC(pMyObject->Value.real);

                return ((LONG)pVar->Value.num == lInt && ulFrac == 0);
            }
#else
            return pMyObject->Value.real == pVar->Value.num;
#endif
            break;

        case BspVar_Real:
            return pMyObject->Value.real == pVar->Value.real;
            break;
        default:
            break;
        }
        break;

    case BspVar_Object:
        switch (pVar->Type)
        {
        case BspVar_Number:
        case BspVar_UNumber:
                BspEng_GetSlapVarCompareValue((ANSC_HANDLE)pMyObject->Value.obj, (ANSC_HANDLE)&compVal);
                return 
                    ((compVal.ulType == BSPENG_SLAP_VAR_COMPARE_VALUE_TYPE_INT) || 
                    (compVal.ulType == BSPENG_SLAP_VAR_COMPARE_VALUE_TYPE_UINT32)) &&
                    ((ULONG)compVal.value.objValue == (ULONG)pVar->Value.num);
            break;

        case BspVar_String:
                BspEng_GetSlapVarCompareValue((ANSC_HANDLE)pMyObject->Value.obj, (ANSC_HANDLE)&compVal);
                return 
                    (compVal.ulType == BSPENG_SLAP_VAR_COMPARE_VALUE_TYPE_STRING) && 
                    (strcmp(compVal.value.stringValue, pVar->Value.str) == 0 );
            break;

        case BspVar_Object:
            {
                ULONG               comp1, comp2;

                BspEng_GetSlapVarCompareValue((ANSC_HANDLE)pMyObject->Value.obj, (ANSC_HANDLE)&compVal);
                comp1 = (ULONG)compVal.value.objValue;

                BspEng_GetSlapVarCompareValue((ANSC_HANDLE)pVar->Value.obj, (ANSC_HANDLE)&compVal);
                comp2 = (ULONG)compVal.value.objValue;

                return comp1 == comp2;
            }
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    AnscTrace("Can't perform comparison\r\n");

    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateVarPlus
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to do PLUS.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hVar
                The other variable for this operation.

    return:     handle to this object.

**********************************************************************/

ANSC_HANDLE
BspTemplateVarPlus
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_VAR_OBJECT        pMyObject = (PBSP_TEMPLATE_VAR_OBJECT)hThisObject;
    PBSP_TEMPLATE_VAR_OBJECT        pVar      = (PBSP_TEMPLATE_VAR_OBJECT)hVar;

    switch (pMyObject->Type)
    {
    case BspVar_String:
        pMyObject->Type         = BspVar_Number;
        pMyObject->Value.num    = pMyObject->StrToNum(hThisObject);
        /* fall through */

    case BspVar_Number:
    case BspVar_UNumber:
        switch (pVar->Type)
        {
        case BspVar_String:
            {
                ULONG               ulLen   = 0;

                if (pVar->Value.str)
                {
                    if (pVar->Size)
                        ulLen   = pVar->Size;
                    else
                        ulLen   = AnscSizeOfString(pVar->Value.str);
                }

                pMyObject->Value.num    += ulLen;
                return hThisObject;
            }
            break;

        case BspVar_Number:
        case BspVar_UNumber:
            pMyObject->Value.num    += pVar->Value.num;
            return hThisObject;
            break;

        case BspVar_Real:
#ifdef   _BSPENG_NO_DOUBLE
            pMyObject->Value.num    += BSP_TEMPLATE_DOUBLE_GET_INT(pVar->Value.real);
#else
            pMyObject->Value.num    += (LONG)pVar->Value.real;
#endif
            return hThisObject;
            break;
        default:
            break;
        }
        break;

    case BspVar_Real:
        switch (pVar->Type)
        {
        case BspVar_String:
            {
                ULONG               ulLen   = 0;

                if (pVar->Value.str)
                {
                    if (pVar->Size)
                        ulLen   = pVar->Size;
                    else
                        ulLen   = AnscSizeOfString(pVar->Value.str);
                }

#ifdef   _BSPENG_NO_DOUBLE
                pMyObject->Value.real   += (LONG)ulLen * BSP_TEMPLATE_DOUBLE_SCALE;
#else
                pMyObject->Value.real   += (LONG)ulLen;
#endif
                return hThisObject;
            }
            break;

        case BspVar_Number:
        case BspVar_UNumber:
#ifdef   _BSPENG_NO_DOUBLE
            pMyObject->Value.real   += pVar->Value.num * BSP_TEMPLATE_DOUBLE_SCALE;
#else
            pMyObject->Value.real   += pVar->Value.num;
#endif
            return hThisObject;
            break;

        case BspVar_Real:
            pMyObject->Value.real   += pVar->Value.real;
            return hThisObject;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    AnscTrace("Can't perform add\r\n");

    return hThisObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateVarMinus
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to do MINUS.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hVar
                The other variable for this operation.

    return:     handle to this object.

**********************************************************************/

ANSC_HANDLE
BspTemplateVarMinus
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_VAR_OBJECT        pMyObject = (PBSP_TEMPLATE_VAR_OBJECT)hThisObject;
    PBSP_TEMPLATE_VAR_OBJECT        pVar      = (PBSP_TEMPLATE_VAR_OBJECT)hVar;

    switch (pMyObject->Type)
    {
    case BspVar_String:
        pMyObject->Type         = BspVar_Number;
        pMyObject->Value.num    = pMyObject->StrToNum(hThisObject);
        /* fall through */

    case BspVar_Number:
    case BspVar_UNumber:
        switch (pVar->Type)
        {
        case BspVar_String:
            {
                ULONG               ulLen   = 0;

                if (pVar->Value.str)
                {
                    if (pVar->Size)
                        ulLen   = pVar->Size;
                    else
                        ulLen   = AnscSizeOfString(pVar->Value.str);
                }

                pMyObject->Value.num    -= (LONG)ulLen;
                return hThisObject;
            }
            break;

        case BspVar_Number:
        case BspVar_UNumber:
            pMyObject->Value.num    -= pVar->Value.num;
            return hThisObject;
            break;

        case BspVar_Real:
#ifdef   _BSPENG_NO_DOUBLE
            pMyObject->Value.num    -= BSP_TEMPLATE_DOUBLE_GET_INT(pVar->Value.real);
#else
            pMyObject->Value.num    -= (LONG)pVar->Value.real;
#endif
            return hThisObject;
            break;
        default:
            break;
        }
        break;

    case BspVar_Real:
        switch (pVar->Type)
        {
        case BspVar_String:
            {
                ULONG               ulLen   = 0;

                if (pVar->Value.str)
                {
                    if (pVar->Size)
                        ulLen   = pVar->Size;
                    else
                        ulLen   = AnscSizeOfString(pVar->Value.str);
                }

#ifdef   _BSPENG_NO_DOUBLE
                pMyObject->Value.real   -= (LONG)ulLen * BSP_TEMPLATE_DOUBLE_SCALE;
#else
                pMyObject->Value.real   -= (LONG)ulLen;
#endif

                return hThisObject;
            }
            break;

        case BspVar_Number:
        case BspVar_UNumber:
#ifdef   _BSPENG_NO_DOUBLE
            pMyObject->Value.real   -= pVar->Value.num * BSP_TEMPLATE_DOUBLE_SCALE;
#else
            pMyObject->Value.real   -= pVar->Value.num;
#endif
            return hThisObject;
            break;

        case BspVar_Real:
            pMyObject->Value.real   -= pVar->Value.real;
            return hThisObject;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    AnscTrace("Can't perform minus\r\n");

    return hThisObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateVarMultiply
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to do MULTIPLY.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hVar
                The other variable for this operation.

    return:     handle to this object.

**********************************************************************/

ANSC_HANDLE
BspTemplateVarMultiply
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_VAR_OBJECT        pMyObject = (PBSP_TEMPLATE_VAR_OBJECT)hThisObject;
    PBSP_TEMPLATE_VAR_OBJECT        pVar      = (PBSP_TEMPLATE_VAR_OBJECT)hVar;

    switch (pMyObject->Type)
    {
    case BspVar_String:
        pMyObject->Type         = BspVar_Number;
        pMyObject->Value.num    = pMyObject->StrToNum(hThisObject);
        /* fall through */

    case BspVar_Number:
    case BspVar_UNumber:
        switch (pVar->Type)
        {
        case BspVar_String:
            {
                ULONG               ulLen   = 0;

                if (pVar->Value.str)
                {
                    if (pVar->Size)
                        ulLen   = pVar->Size;
                    else
                        ulLen   = AnscSizeOfString(pVar->Value.str);
                }

                pMyObject->Value.num    *= (LONG)ulLen;
                return hThisObject;
            }
            break;

        case BspVar_Number:
        case BspVar_UNumber:
            pMyObject->Value.num    *= pVar->Value.num;
            return hThisObject;
            break;

        case BspVar_Real:
#ifdef   _BSPENG_NO_DOUBLE
            pMyObject->Value.num    *= 
                (LONG)BSP_TEMPLATE_DOUBLE_GET_INT(pVar->Value.real) / BSP_TEMPLATE_DOUBLE_SCALE;
#else
            pMyObject->Value.num    = (LONG)(pMyObject->Value.num * pVar->Value.real);
#endif
            return hThisObject;
            break;
        default:
            break;
        }
        break;

    case BspVar_Real:
        switch (pVar->Type)
        {
        case BspVar_String:
            {
                ULONG               ulLen   = 0;

                if (pVar->Value.str)
                {
                    if (pVar->Size)
                        ulLen   = pVar->Size;
                    else
                        ulLen   = AnscSizeOfString(pVar->Value.str);
                }

#ifdef   _BSPENG_NO_DOUBLE
                pMyObject->Value.real   *= (LONG)ulLen;
#else
                pMyObject->Value.real   *= (LONG)ulLen;
#endif
                return hThisObject;
            }
            break;

        case BspVar_Number:
        case BspVar_UNumber:
#ifdef   _BSPENG_NO_DOUBLE
            pMyObject->Value.real   *= pVar->Value.num;
#else
            pMyObject->Value.real   *= pVar->Value.num;
#endif
            return hThisObject;
            break;

        case BspVar_Real:
#ifdef   _BSPENG_NO_DOUBLE
            pMyObject->Value.real   = 
                pMyObject->Value.real * pVar->Value.real / BSP_TEMPLATE_DOUBLE_SCALE;
#else
            pMyObject->Value.real   *= pVar->Value.real;
#endif
            return hThisObject;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    AnscTrace("Can't perform multiply\r\n");

    return hThisObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateVarDivide
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to do DIVIDE.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hVar
                The other variable for this operation.

    return:     handle to this object.

**********************************************************************/

ANSC_HANDLE
BspTemplateVarDivide
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_VAR_OBJECT        pMyObject = (PBSP_TEMPLATE_VAR_OBJECT)hThisObject;
    PBSP_TEMPLATE_VAR_OBJECT        pVar      = (PBSP_TEMPLATE_VAR_OBJECT)hVar;

    switch (pMyObject->Type)
    {
    case BspVar_String:
        pMyObject->Type         = BspVar_Number;
        pMyObject->Value.num    = pMyObject->StrToNum(hThisObject);
        /* fall through */

    case BspVar_Number:
    case BspVar_UNumber:
        switch (pVar->Type)
        {
        case BspVar_String:
            {
                ULONG               ulLen   = 0;

                if (pVar->Value.str)
                {
                    if (pVar->Size)
                        ulLen   = pVar->Size;
                    else
                        ulLen   = AnscSizeOfString(pVar->Value.str);
                }

                if (ulLen != 0)
                    pMyObject->Value.num    /= ulLen;
                else
                    AnscTrace("Divide by 0, operation ignored\r\n");
                return hThisObject;
            }
            break;

        case BspVar_Number:
        case BspVar_UNumber:
            if (pVar->Value.num)
                pMyObject->Value.num    /= pVar->Value.num;
            else
                AnscTrace("Divide by 0, operation ignored\r\n");
            return hThisObject;
            break;

        case BspVar_Real:
            if ((LONG)pVar->Value.real != 0)
#ifdef   _BSPENG_NO_DOUBLE
                pMyObject->Value.num    = 
                pMyObject->Value.num * BSP_TEMPLATE_DOUBLE_SCALE / (LONG)pVar->Value.real;
#else
                pMyObject->Value.num    /= (LONG)pVar->Value.real;
#endif
            else
                AnscTrace("Divide by 0, operation ignored\r\n");
            return hThisObject;
            break;
        default:
            break;
        }
        break;

    case BspVar_Real:
        switch (pVar->Type)
        {
        case BspVar_String:
            {
                ULONG               ulLen   = 0;

                if (pVar->Value.str)
                {
                    if (pVar->Size)
                        ulLen   = pVar->Size;
                    else
                        ulLen   = AnscSizeOfString(pVar->Value.str);
                }

                if (ulLen != 0)
#ifdef   _BSPENG_NO_DOUBLE
                    pMyObject->Value.real   = pMyObject->Value.real / (LONG)ulLen;
#else
                    pMyObject->Value.real   /= (LONG)ulLen;
#endif
                else
                    AnscTrace("Divide by 0, operation ignored\r\n");

                return hThisObject;
            }
            break;

        case BspVar_Number:
        case BspVar_UNumber:
            if (pVar->Value.num)
#ifdef   _BSPENG_NO_DOUBLE
                pMyObject->Value.real   = pMyObject->Value.real / pVar->Value.num;
#else
                pMyObject->Value.real   /= pVar->Value.num;
#endif
            else
                AnscTrace("Divide by 0, operation ignored\r\n");
            return hThisObject;
            break;

        case BspVar_Real:
            if (pVar->Value.real != 0)
#ifdef   _BSPENG_NO_DOUBLE
                pMyObject->Value.real   = 
                pMyObject->Value.real * BSP_TEMPLATE_DOUBLE_SCALE / pVar->Value.real;
#else
                pMyObject->Value.real   /= pVar->Value.real;
#endif
            else
                AnscTrace("Divide by 0, operation ignored\r\n");
            return hThisObject;
            break;
	default:
            break;
        }
        break;
    default:
        break;
    }

    AnscTrace("Can't perform divide\r\n");

    return hThisObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateVarMod
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to do MODULO.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hVar
                The other variable for this operation.

    return:     handle to this object.

**********************************************************************/

ANSC_HANDLE
BspTemplateVarMod
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_VAR_OBJECT        pMyObject = (PBSP_TEMPLATE_VAR_OBJECT)hThisObject;
    PBSP_TEMPLATE_VAR_OBJECT        pVar      = (PBSP_TEMPLATE_VAR_OBJECT)hVar;

    switch (pMyObject->Type)
    {
    case BspVar_String:
        pMyObject->Value.num    = pMyObject->StrToNum(hThisObject);
        /* fall through */

    case BspVar_Real:
        if (pMyObject->Type == BspVar_Real)
            pMyObject->Value.num    = (LONG)pMyObject->Value.real;

        pMyObject->Type = BspVar_Number;
        /* fall through */

    case BspVar_Number:
    case BspVar_UNumber:
        switch (pVar->Type)
        {
        case BspVar_String:
            {
                ULONG               ulLen   = 0;

                if (pVar->Value.str)
                {
                    if (pVar->Size)
                        ulLen   = pVar->Size;
                    else
                        ulLen   = AnscSizeOfString(pVar->Value.str);
                }

                if (ulLen != 0)
                    pMyObject->Value.num    %= (LONG)ulLen;
                else
                    AnscTrace("Divide by 0, operation ignored\r\n");
                return hThisObject;
            }
            break;

        case BspVar_Number:
        case BspVar_UNumber:
            if (pVar->Value.num)
                pMyObject->Value.num    %= pVar->Value.num;
            else
                AnscTrace("Divide by 0, operation ignored\r\n");
            return hThisObject;
            break;

        case BspVar_Real:
            if (pVar->Value.real != 0)
#ifdef   _BSPENG_NO_DOUBLE
                pMyObject->Value.num    = 
                (pMyObject->Value.num * BSP_TEMPLATE_DOUBLE_SCALE) % (LONG)pVar->Value.real;
#else
                pMyObject->Value.num    %= (LONG)pVar->Value.real;
#endif
            else
                AnscTrace("Divide by 0, operation ignored\r\n");
            return hThisObject;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    AnscTrace("Can't perform mod\r\n");

    return hThisObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateVarStrCat
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to do STRCAT.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hVar
                The other variable for this operation.

    return:     handle to this object.

**********************************************************************/

ANSC_HANDLE
BspTemplateVarStrCat
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_VAR_OBJECT        pMyObject = (PBSP_TEMPLATE_VAR_OBJECT)hThisObject;
    PBSP_TEMPLATE_VAR_OBJECT        pVar      = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    ULONG                           ulLen = 0; /*RDKB-6142, CID-24705; initializing the variables*/
    char                            *pBuf = NULL;
    errno_t   rc = -1;

    switch (pMyObject->Type)
    {
    case BspVar_String:
        {
            if (pMyObject->Value.str)
            {
                if (pMyObject->Size)
                    ulLen   = pMyObject->Size;
                else
                   ulLen   = AnscSizeOfString(pMyObject->Value.str);
            }
            else
            {
                ulLen   = 0;
            }

            if (pMyObject->Size == 0)
            {
                pMyObject->Size = ulLen + 16;
                pBuf = (char *)AnscAllocateMemory(pMyObject->Size);
                if (!pBuf)
                {
                    pMyObject->Size = 0;
                }
                else
                {
                    if (pMyObject->Value.str && ulLen != 0)
                    {
                        AnscCopyString(pBuf, pMyObject->Value.str);
                    }
                    else
                    {
                        pBuf[0] = 0;
                    }
                }
                pMyObject->Value.str    = pBuf;
                ACCESS_BSP_TEMPLATE_VAR_CLEAR_TEMP_FLAG(pMyObject);
            }
        }
        break;

    case BspVar_Number:
    case BspVar_UNumber:
    case BspVar_Real:
        {
            pMyObject->Size = 16;
            pBuf    = (char *)AnscAllocateMemory(pMyObject->Size);

            if (!pBuf)
            {
                pMyObject->Size = 0;
            }
            else
            {
                if (pMyObject->Type == BspVar_Number){
                    rc = sprintf_s(pBuf,pMyObject->Size, "%ld", pMyObject->Value.num);
                    if(rc < EOK){
                        ERR_CHK(rc);
                    }
                    ulLen   = rc;
                }
                else
                if (pMyObject->Type == BspVar_UNumber){
                    rc = sprintf_s(pBuf,pMyObject->Size, "%u", (UINT)pMyObject->Value.num);
                    if(rc < EOK){
                        ERR_CHK(rc);
                    }
                    ulLen   = rc;
                }
                else
#ifdef   _BSPENG_NO_DOUBLE
                    ulLen   = BSP_TEMPLATE_DOUBLE_TO_STRING(pBuf, (int)pMyObject->Value.real);
#else
                    rc = sprintf_s(pBuf,pMyObject->Size, "%f", pMyObject->Value.real);
                    if(rc < EOK){
                        ERR_CHK(rc);
                    }
                    ulLen   = rc;
#endif
            }

            pMyObject->Value.str    = pBuf;
            pMyObject->Type         = BspVar_String;
            ACCESS_BSP_TEMPLATE_VAR_CLEAR_TEMP_FLAG(pMyObject);
        }
        break;

    case BspVar_Object:
        {
            pBuf    = BspTemplateEngGetSlapObjectString((ANSC_HANDLE)pMyObject->Value.obj);
            ulLen   = 0;

            if (pBuf)
            {
                ulLen   = AnscSizeOfString(pBuf);
            }

            pMyObject->Reset((ANSC_HANDLE)pMyObject);

            pMyObject->Type         = BspVar_String;
            pMyObject->Value.str    = pBuf;
            pMyObject->Size         = ulLen;
            ACCESS_BSP_TEMPLATE_VAR_CLEAR_TEMP_FLAG(pMyObject);

            pBuf    = NULL;
        }
        break;

    default:
        AnscTrace("Can't perform StrCat\r\n");
        return hThisObject;
        break;
    }

    switch (pVar->Type)
    {
    case BspVar_String:
        {
            ULONG                   ulLenStr   = 0;

            if (pVar->Value.str)
            {
                if (pVar->Size)
                    ulLenStr    = pVar->Size;
                else
                    ulLenStr    = AnscSizeOfString(pVar->Value.str);
            }

            pBuf    = pVar->Value.str;
            ulLen   += ulLenStr;
        }
        break;

    case BspVar_Number:
    case BspVar_UNumber:
    case BspVar_Real:
        pBuf    = (char *)AnscAllocateMemory(16);

        if (pBuf)
        {
            if (pVar->Type == BspVar_Number){
                rc = sprintf_s(pBuf,16, "%ld", pVar->Value.num);
                if(rc < EOK){
                    ERR_CHK(rc);
                }
                ulLen   += rc;
            }
            else
            if (pVar->Type == BspVar_UNumber){
                rc = sprintf_s(pBuf,16, "%u", (UINT)pVar->Value.num);
                if(rc < EOK){
                    ERR_CHK(rc);
                }
                ulLen   += rc;
            }
            else
#ifdef   _BSPENG_NO_DOUBLE
                ulLen   += BSP_TEMPLATE_DOUBLE_TO_STRING(pBuf, (int)pVar->Value.real);
#else
                rc = sprintf_s(pBuf,16, "%f", pVar->Value.real);
                if(rc < EOK){
                    ERR_CHK(rc);
                }
                ulLen   += rc;
#endif
        }

        break;

    case BspVar_Object:

        pBuf  = BspTemplateEngGetSlapObjectString(pVar->Value.obj);

        if (pBuf)
        {
            ulLen += AnscSizeOfString(pBuf);
        }

        break;

    default:
        AnscTrace("Can't perform StrCat\r\n");
        return hThisObject;
        break;
    }

    if (++ulLen > pMyObject->Size)
    {
/*
        pMyObject->Value.str    = (char *)AnscReAllocMemory(pMyObject->Value.str, ulLen);
*/
        pMyObject->Value.str    = 
            (char *)AnscMemUtilRealloc(pMyObject->Value.str, pMyObject->Size, ulLen);
    }

    if (pBuf && pMyObject->Value.str)
    {
        rc = strcat_s(pMyObject->Value.str, ulLen, pBuf);
        ERR_CHK(rc);
    }
    else
        if (!pMyObject->Value.str && pBuf)
        {
            pMyObject->Value.str    = (PCHAR)AnscDupString((PUCHAR)pBuf);
            pMyObject->Size         = AnscSizeOfString(pMyObject->Value.str);
        }

    if (pVar->Type != BspVar_String && pBuf)
    {
        AnscFreeMemory(pBuf);
    }

    pMyObject->Size = ulLen;
    ACCESS_BSP_TEMPLATE_VAR_CLEAR_TEMP_FLAG(pMyObject);

    return hThisObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateVarBitOr
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to do BIT OR.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hVar
                The other variable for this operation.

    return:     handle to this object.

**********************************************************************/

ANSC_HANDLE
BspTemplateVarBitOr
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_VAR_OBJECT        pMyObject = (PBSP_TEMPLATE_VAR_OBJECT)hThisObject;
    PBSP_TEMPLATE_VAR_OBJECT        pVar      = (PBSP_TEMPLATE_VAR_OBJECT)hVar;

    switch (pMyObject->Type)
    {
    case BspVar_Number:
    case BspVar_UNumber:
        switch (pVar->Type)
        {
        case BspVar_Number:
        case BspVar_UNumber:
            pMyObject->Value.num    |= pVar->Value.num;
            return hThisObject;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    AnscTrace("Can't perform | (bit or)\r\n");

    return hThisObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateVarBitAnd
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to do BIT AND.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hVar
                The other variable for this operation.

    return:     handle to this object.

**********************************************************************/

ANSC_HANDLE
BspTemplateVarBitAnd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_VAR_OBJECT        pMyObject = (PBSP_TEMPLATE_VAR_OBJECT)hThisObject;
    PBSP_TEMPLATE_VAR_OBJECT        pVar      = (PBSP_TEMPLATE_VAR_OBJECT)hVar;

    switch (pMyObject->Type)
    {
    case BspVar_Number:
    case BspVar_UNumber:
        switch (pVar->Type)
        {
        case BspVar_Number:
        case BspVar_UNumber:
            pMyObject->Value.num    &= pVar->Value.num;
            return hThisObject;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    AnscTrace("Can't perform & (bit and)\r\n");

    return hThisObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateVarBitXor
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to do BIT XOR.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hVar
                The other variable for this operation.

    return:     handle to this object.

**********************************************************************/

ANSC_HANDLE
BspTemplateVarBitXor
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_VAR_OBJECT        pMyObject = (PBSP_TEMPLATE_VAR_OBJECT)hThisObject;
    PBSP_TEMPLATE_VAR_OBJECT        pVar      = (PBSP_TEMPLATE_VAR_OBJECT)hVar;

    switch (pMyObject->Type)
    {
    case BspVar_Number:
    case BspVar_UNumber:
        switch (pVar->Type)
        {
        case BspVar_Number:
        case BspVar_UNumber:
            pMyObject->Value.num    ^= pVar->Value.num;
            return hThisObject;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    AnscTrace("Can't perform ^ (bit xor)\r\n");

    return hThisObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateVarShiftLeft
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to do SHIFT LEFT.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hVar
                The other variable for this operation.

    return:     handle to this object.

**********************************************************************/

ANSC_HANDLE
BspTemplateVarShiftLeft
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_VAR_OBJECT        pMyObject = (PBSP_TEMPLATE_VAR_OBJECT)hThisObject;
    PBSP_TEMPLATE_VAR_OBJECT        pVar      = (PBSP_TEMPLATE_VAR_OBJECT)hVar;

    switch (pMyObject->Type)
    {
    case BspVar_Number:
    case BspVar_UNumber:
        switch (pVar->Type)
        {
        case BspVar_Number:
        case BspVar_UNumber:
            pMyObject->Value.num    <<= pVar->Value.num;
            return hThisObject;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    AnscTrace("Can't perform << (shift left)\r\n");

    return hThisObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateVarShifeRight
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to do SHIFT RIGHT.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hVar
                The other variable for this operation.

    return:     handle to this object.

**********************************************************************/

ANSC_HANDLE
BspTemplateVarShiftRight
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_VAR_OBJECT        pMyObject = (PBSP_TEMPLATE_VAR_OBJECT)hThisObject;
    PBSP_TEMPLATE_VAR_OBJECT        pVar      = (PBSP_TEMPLATE_VAR_OBJECT)hVar;

    switch (pMyObject->Type)
    {
    case BspVar_Number:
    case BspVar_UNumber:
        switch (pVar->Type)
        {
        case BspVar_Number:
        case BspVar_UNumber:
            pMyObject->Value.num    >>= pVar->Value.num;
            return hThisObject;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    AnscTrace("Can't perform >> (shift right)\r\n");

    return hThisObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateVarPos
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to make this variable's value
        positive.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     void.

**********************************************************************/

void
BspTemplateVarPos
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_VAR_OBJECT        pMyObject = (PBSP_TEMPLATE_VAR_OBJECT)hThisObject;

    switch (pMyObject->Type)
    {
    case BspVar_String:
        pMyObject->Value.num    = pMyObject->StrToNum(hThisObject);
        pMyObject->Type         = BspVar_Number;
        break;

    case BspVar_Number:
        if (pMyObject->Value.num < 0)
            pMyObject->Value.num = -pMyObject->Value.num;
        break;

    case BspVar_UNumber: 
        /* do nothing here */
        break;

    case BspVar_Real:
#ifdef   _BSPENG_NO_DOUBLE
        if (BSP_TEMPLATE_DOUBLE_GET_INT(pMyObject->Value.real) < 0)
            BSP_TEMPATE_DOUBLE_SET
                (
                    pMyObject->Value.real, 
                    -BSP_TEMPLATE_DOUBLE_GET_INT(pMyObject->Value.real)
                );
#else
        if (pMyObject->Value.real < 0)
            pMyObject->Value.real   = -pMyObject->Value.real;
#endif
        break;

    default:
        AnscTrace("Can't positivize\r\n");
        break;
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateVarNeg
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to make this variable's value
        negative.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     void.

**********************************************************************/

void
BspTemplateVarNeg
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_VAR_OBJECT        pMyObject = (PBSP_TEMPLATE_VAR_OBJECT)hThisObject;

    switch (pMyObject->Type)
    {
    case BspVar_String:
        pMyObject->Value.num    = -(LONG)pMyObject->StrToNum(hThisObject);
        pMyObject->Type         = BspVar_Number;
        break;

    case BspVar_Number:
        pMyObject->Value.num = -pMyObject->Value.num;
        break;

    case BspVar_UNumber: 
        /* do nothing here */
        break;

    case BspVar_Real:
#ifdef   _BSPENG_NO_DOUBLE
        BSP_TEMPATE_DOUBLE_SET
            (
                pMyObject->Value.real, 
                -BSP_TEMPLATE_DOUBLE_GET_INT(pMyObject->Value.real)
            );
#else
        pMyObject->Value.real = -pMyObject->Value.real;
#endif
        break;

    default:
        AnscTrace("Can't positivize\r\n");
        break;
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        BspTemplateVarStrToNum
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to convert string to number.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     the number value of the string.

**********************************************************************/

ULONG
BspTemplateVarStrToNum
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_VAR_OBJECT        pMyObject = (PBSP_TEMPLATE_VAR_OBJECT)hThisObject;
    ULONG                           ulLen     = 0;

    if (pMyObject->Type == BspVar_String && pMyObject->Value.str)
    {
        if (pMyObject->Size)
            ulLen   = pMyObject->Size;
        else
            ulLen   = AnscSizeOfString(pMyObject->Value.str);
    }

    if (pMyObject->Size)
    {
        AnscFreeMemory(pMyObject->Value.str);
        pMyObject->Size         = 0;
        pMyObject->Value.str    = NULL;
    }

    return ulLen;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateVarReset
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to reset the variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     void.

**********************************************************************/

void
BspTemplateVarReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_VAR_OBJECT        pMyObject = (PBSP_TEMPLATE_VAR_OBJECT)hThisObject;
    /*PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)pMyObject->hRuntime;*/

    /*
    if (pMyObject->Size && !pMyObject->bTemp)
    */
    if (pMyObject->Type == BspVar_String && !pMyObject->bTemp)
	{
        if (pMyObject->Value.str)
        {
            AnscFreeMemory(pMyObject->Value.str);
        }
	}
    else
    if (pMyObject->Type == BspVar_Array)
    {
        PBSP_TEMPLATE_ARRAY_ITEMS   pItems;
        PBSP_TEMPLATE_VAR_OBJECT    *pVars, pVar;
        ULONG                       i;

        /*
         *  No matter bTemp is set on or off, we need to 
         *  release the memory allocated for array items.
         */
        pItems  = &pMyObject->Value.arrayItems;

        if (pItems->bBulkData)
        {
            if (pItems->Type == BspArray_String)
            {
                SLAP_STRING         *pString = (SLAP_STRING *)pItems->hBulkData;

                for (i = 0; i < pItems->ulLen; i ++)
                {
                    AnscFreeMemory(pString[i]);
                }
            }

            if (pItems->ulLen != 0 && pItems->hBulkData)
            {
                AnscFreeMemory(pItems->hBulkData);
            }
        }
        else
        {
            pVars   = (PBSP_TEMPLATE_VAR_OBJECT *)pItems->hVars;

            for (i = 0; i < pItems->ulSize; i ++)
            {
                pVar    = pVars[i];
                pVar->Remove((ANSC_HANDLE)pVar);

                /* if array size is too big, performance is bad */
                /* pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pVar); */
            }

            AnscFreeMemory(pVars);
        }
    }
    else
    if (pMyObject->Type == BspVar_Object && pMyObject->Value.obj)
    {
        SlapFreeVariable(((PSLAP_VARIABLE)pMyObject->Value.obj));
    }

    pMyObject->Type         = BspVar_Number;
    pMyObject->Value.num    = 0;
	pMyObject->bTemp		= TRUE;
    pMyObject->Size         = 0;

    pMyObject->SlapSyntax   = 0;
    pMyObject->ContentType  = 0;

    AnscZeroMemory(&pMyObject->Value.arrayItems, sizeof(pMyObject->Value.arrayItems));
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateVarSetContentType
            (
                ANSC_HANDLE                 hThisObject,
                char                        *pContentType
            )

    description:

        This function is called to set variable's content type.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char                        *pContentType
                The name of content type.

    return:     void.

**********************************************************************/

void
BspTemplateVarSetContentType
    (
        ANSC_HANDLE                 hThisObject,
        char                        *pContentType
    )
{
    PBSP_TEMPLATE_VAR_OBJECT        pMyObject = (PBSP_TEMPLATE_VAR_OBJECT)hThisObject;

    if (!pContentType)
    {
        return;
    }

    if (strcasecmp(pContentType, SLAP_CONTENT_NAME_MAPPED) == 0 )
    {
        pMyObject->ContentType  = SLAP_CONTENT_TYPE_MAPPED;
    }
    else
    if (strcasecmp(pContentType, SLAP_CONTENT_NAME_IP4_ADDR) == 0 )
    {
        pMyObject->ContentType  = SLAP_CONTENT_TYPE_IP4_ADDR;
    }
    else
    if (strcasecmp(pContentType, SLAP_CONTENT_NAME_MAC_ADDR) == 0 )
    {
        pMyObject->ContentType  = SLAP_CONTENT_TYPE_MAC_ADDR;
    }
    else
    if (strcasecmp(pContentType, SLAP_CONTENT_NAME_IP4_ADDR_LIST) == 0 )
    {
        pMyObject->ContentType  = SLAP_CONTENT_TYPE_IP4_ADDR_LIST;
    }
    else
    if (strcasecmp(pContentType, SLAP_CONTENT_NAME_TCPUDP_PORT) == 0 )
    {
        pMyObject->ContentType  = SLAP_CONTENT_TYPE_TCPUDP_PORT;
    }
    else
    if (strcasecmp(pContentType, SLAP_CONTENT_NAME_CALENDAR_TIME) == 0 )
    {
        pMyObject->ContentType  = SLAP_CONTENT_TYPE_CALENDAR_TIME;
    }
    else
    if (strcasecmp(pContentType, SLAP_CONTENT_NAME_PASSWORD) == 0 )
    {
        pMyObject->ContentType  = SLAP_CONTENT_TYPE_PASSWORD;
    }
    else
    if (strcasecmp(pContentType, SLAP_CONTENT_NAME_MASKED) == 0 )
    {
        pMyObject->ContentType  = SLAP_CONTENT_TYPE_MASKED;
    }
    else
    if (strcasecmp(pContentType, SLAP_CONTENT_NAME_OID_LIST) == 0 )
    {
        pMyObject->ContentType  = SLAP_CONTENT_TYPE_OID_LIST;
    }
    else
    if (strcasecmp(pContentType, SLAP_CONTENT_NAME_ASCII_STRING) == 0 )
    {
        pMyObject->ContentType  = SLAP_CONTENT_TYPE_ASCII_STRING;
    }
    else
    if (strcasecmp(pContentType, SLAP_CONTENT_NAME_HEX_STRING) == 0 )
    {
        pMyObject->ContentType  = SLAP_CONTENT_TYPE_HEX_STRING;
    }
    else
    if (strcasecmp(pContentType, SLAP_CONTENT_NAME_TABLE_2D) == 0 )
    {
        pMyObject->ContentType  = SLAP_CONTENT_TYPE_TABLE_2D;
    }
    else
    if (strcasecmp(pContentType, SLAP_CONTENT_NAME_TABLE_3D) == 0 )
    {
        pMyObject->ContentType  = SLAP_CONTENT_TYPE_TABLE_3D;
    }
    else
    if (strcasecmp(pContentType, SLAP_CONTENT_NAME_SIMPLE_CHART_PIE) == 0 )
    {
        pMyObject->ContentType  = SLAP_CONTENT_TYPE_SIMPLE_CHART_PIE;
    }
    else
    if (strcasecmp(pContentType, SLAP_CONTENT_NAME_SIMPLE_CHART_BAR) == 0 )
    {
        pMyObject->ContentType  = SLAP_CONTENT_TYPE_SIMPLE_CHART_BAR;
    }
    else
    if (strcasecmp(pContentType, SLAP_CONTENT_NAME_SIMPLE_GRAPH) == 0 )
    {
        pMyObject->ContentType  = SLAP_CONTENT_TYPE_SIMPLE_GRAPH;
    }
    else
    {
        pMyObject->ContentType  = SLAP_CONTENT_TYPE_UNSPECIFIED;
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char *
        BspTemplateVarGetContentType
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to get the variable's content type.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     the name of content type.

**********************************************************************/

char *
BspTemplateVarGetContentType
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_VAR_OBJECT        pMyObject = (PBSP_TEMPLATE_VAR_OBJECT)hThisObject;
    char                            *pType    = SLAP_CONTENT_NAME_UNSPECIFIED;

    switch (pMyObject->ContentType)
    {
        case    SLAP_CONTENT_TYPE_MAPPED:

            pType   = SLAP_CONTENT_NAME_MAPPED;

            break;

        case    SLAP_CONTENT_TYPE_IP4_ADDR:

            pType   = SLAP_CONTENT_NAME_IP4_ADDR;

            break;

        case    SLAP_CONTENT_TYPE_MAC_ADDR:

            pType   = SLAP_CONTENT_NAME_MAC_ADDR;

            break;

        case    SLAP_CONTENT_TYPE_IP4_ADDR_LIST:

            pType   = SLAP_CONTENT_NAME_IP4_ADDR_LIST;

            break;

        case    SLAP_CONTENT_TYPE_TCPUDP_PORT:

            pType   = SLAP_CONTENT_NAME_TCPUDP_PORT;

            break;

        case    SLAP_CONTENT_TYPE_CALENDAR_TIME:

            pType   = SLAP_CONTENT_NAME_CALENDAR_TIME;

            break;

        case    SLAP_CONTENT_TYPE_PASSWORD:

            pType   = SLAP_CONTENT_NAME_PASSWORD;

            break;

        case    SLAP_CONTENT_TYPE_MASKED:

            pType   = SLAP_CONTENT_NAME_MASKED;

            break;

        case    SLAP_CONTENT_TYPE_OID_LIST:

            pType   = SLAP_CONTENT_NAME_OID_LIST;

            break;

        case    SLAP_CONTENT_TYPE_ASCII_STRING:

            pType   = SLAP_CONTENT_NAME_ASCII_STRING;

            break;

        case    SLAP_CONTENT_TYPE_HEX_STRING:

            pType   = SLAP_CONTENT_NAME_HEX_STRING;

            break;

        case    SLAP_CONTENT_TYPE_TABLE_2D:

            pType   = SLAP_CONTENT_NAME_TABLE_2D;

            break;

        case    SLAP_CONTENT_TYPE_TABLE_3D:
            pType   = SLAP_CONTENT_NAME_TABLE_3D;

            break;

        case    SLAP_CONTENT_TYPE_SIMPLE_CHART_PIE:

            pType   = SLAP_CONTENT_NAME_SIMPLE_CHART_PIE;

            break;

        case    SLAP_CONTENT_TYPE_SIMPLE_CHART_BAR:

            pType   = SLAP_CONTENT_NAME_SIMPLE_CHART_BAR;

            break;

        case    SLAP_CONTENT_TYPE_SIMPLE_GRAPH:

            pType   = SLAP_CONTENT_NAME_SIMPLE_GRAPH;

            break;
    }

    return pType;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateVarSetContentType2
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulContentType
            )

    description:

        This function is called to set variable's content type.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulContentType
                The content type.

    return:     void.

**********************************************************************/

void
BspTemplateVarSetContentType2
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulContentType
    )
{
    PBSP_TEMPLATE_VAR_OBJECT        pMyObject = (PBSP_TEMPLATE_VAR_OBJECT)hThisObject;

    pMyObject->ContentType  = ulContentType;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BspTemplateVarGetBulkDataAt
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hArrayItem,
                ULONG                       ulIndex
            )

    description:

        This function is called to get the specified array item.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hArrayItem
                The array from which data will be retrieved.

                ULONG                       ulIndex
                The index to the item to be retrieved.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BspTemplateVarGetBulkDataAt
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hArrayItem,
        ULONG                       ulIndex
    )
{
    PBSP_TEMPLATE_VAR_OBJECT        pMyObject   = (PBSP_TEMPLATE_VAR_OBJECT)hThisObject;
    PBSP_TEMPLATE_ARRAY_ITEMS       pArrayItems = (PBSP_TEMPLATE_ARRAY_ITEMS)hArrayItem;
    ANSC_STATUS                     status      = ANSC_STATUS_SUCCESS;

    pMyObject->Reset((ANSC_HANDLE)pMyObject);

    /*RDKB-6142, CID-24350; null check before use*/
    if (!pArrayItems || ulIndex >= pArrayItems->ulLen)
    {
        return ANSC_STATUS_BAD_PARAMETER;
    }

    switch (pArrayItems->Type)
    {
        case    BspArray_Uint:

            {
                SLAP_UINT32         *pUintArray = (SLAP_UINT32 *)pArrayItems->hBulkData;

                pMyObject->Type         = BspVar_UNumber;
                pMyObject->Value.num    = pUintArray[ulIndex];
            }

            break;

        case    BspArray_Int:

            {
                SLAP_INT            *pIntArray = (SLAP_INT *)pArrayItems->hBulkData;

                pMyObject->Type         = BspVar_Number;
                pMyObject->Value.num    = pIntArray[ulIndex];
            }

            break;

        case    BspArray_String:

            {
                SLAP_STRING         *pStringArray = (SLAP_STRING *)pArrayItems->hBulkData;

                pMyObject->Type         = BspVar_String;
                pMyObject->Value.str    = AnscCloneString(pStringArray[ulIndex]);
                ACCESS_BSP_TEMPLATE_VAR_CLEAR_TEMP_FLAG(pMyObject);
            }

            break;

        case    BspArray_Uchar:

            {
                SLAP_UCHAR          *pUcharArray = (SLAP_UCHAR *)pArrayItems->hBulkData;

                pMyObject->Type         = BspVar_UNumber;
                pMyObject->Value.num    = (ULONG)pUcharArray[ulIndex];
            }

            break;

        case    BspArray_Char:

            {
                SLAP_CHAR           *pCharArray = (SLAP_CHAR *)pArrayItems->hBulkData;

                pMyObject->Type         = BspVar_UNumber;
                pMyObject->Value.num    = (ULONG)pCharArray[ulIndex];
            }

            break;

        case    BspArray_Bool:

            {
                SLAP_BOOL           *pBoolArray = (SLAP_BOOL *)pArrayItems->hBulkData;

                pMyObject->Type         = BspVar_UNumber;
                pMyObject->Value.num    = (ULONG)pBoolArray[ulIndex];
            }

            break;

        case    BspArray_Ptr:

            {
                SLAP_PTR            *pPtrArray = (SLAP_PTR *)pArrayItems->hBulkData;

                pMyObject->Type         = BspVar_UNumber;
                pMyObject->Value.num    = (ULONG)pPtrArray[ulIndex];
            }

            break;

        case    BspArray_Handle:

            {
                SLAP_HANDLE         *pHandleArray = (SLAP_HANDLE *)pArrayItems->hBulkData;

                pMyObject->Type         = BspVar_UNumber;
                pMyObject->Value.num    = (ULONG)pHandleArray[ulIndex];
            }

            break;

        case    BspArray_Object:

            {
                SLAP_OBJECT         *pObjectArray = (SLAP_OBJECT *)pArrayItems->hBulkData;

                pMyObject->Type         = BspVar_UNumber;
                pMyObject->Value.num    = (ULONG)pObjectArray[ulIndex];
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
        BspTemplateVarSetArraySize
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulNewSize
            )

    description:

        This function is called to resize the array.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulNewSize
                The new array size.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BspTemplateVarSetArraySize
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulNewSize
    )
{
    PBSP_TEMPLATE_VAR_OBJECT        pMyObject   = (PBSP_TEMPLATE_VAR_OBJECT)hThisObject;
    /*PBSP_TEMPLATE_RUNTIME_OBJECT    pRt         = (PBSP_TEMPLATE_RUNTIME_OBJECT)pMyObject->hRuntime;*/
    ANSC_STATUS                     status      = ANSC_STATUS_SUCCESS;
    PBSP_TEMPLATE_ARRAY_ITEMS       pArrayItems;
    ULONG                           i;

    if (pMyObject->Type != BspVar_Array)
    {
        return ANSC_STATUS_BAD_PARAMETER;
    }

    pArrayItems = &pMyObject->Value.arrayItems;

    if (pArrayItems->ulLen >= ulNewSize)
    {
        pArrayItems->ulLen  = ulNewSize;

        return ANSC_STATUS_SUCCESS;
    }

    if (pArrayItems->bBulkData)
    {
        ULONG                       ulTotalSize     = 0;
        ULONG                       ulNewTotalSize  = 0;
        PVOID                       pArray     = (PVOID)pArrayItems->hBulkData;
        PVOID                       pNewArray;

        switch (pArrayItems->Type)
        {
            case    BspArray_Uint:
                
                ulTotalSize     = pArrayItems->ulLen * sizeof(SLAP_UINT32);
                ulNewTotalSize  = ulNewSize * sizeof(SLAP_UINT32);

                break;

            case    BspArray_Int:
                
                ulTotalSize     = pArrayItems->ulLen * sizeof(SLAP_INT);
                ulNewTotalSize  = ulNewSize * sizeof(SLAP_INT);

                break;

            case    BspArray_String:
                
                ulTotalSize     = pArrayItems->ulLen * sizeof(SLAP_STRING);
                ulNewTotalSize  = ulNewSize * sizeof(SLAP_STRING);

                break;

            case    BspArray_Uchar:
                
                ulTotalSize     = pArrayItems->ulLen * sizeof(SLAP_UCHAR);
                ulNewTotalSize  = ulNewSize * sizeof(SLAP_UCHAR);

                break;

            case    BspArray_Char:
                
                ulTotalSize     = pArrayItems->ulLen * sizeof(SLAP_CHAR);
                ulNewTotalSize  = ulNewSize * sizeof(SLAP_CHAR);

                break;

            case    BspArray_Bool:
                
                ulTotalSize     = pArrayItems->ulLen * sizeof(SLAP_BOOL);
                ulNewTotalSize  = ulNewSize * sizeof(SLAP_BOOL);

                break;

            case    BspArray_Ptr:
                
                ulTotalSize     = pArrayItems->ulLen * sizeof(SLAP_PTR);
                ulNewTotalSize  = ulNewSize * sizeof(SLAP_PTR);

                break;

            case    BspArray_Handle:
                
                ulTotalSize     = pArrayItems->ulLen * sizeof(SLAP_HANDLE);
                ulNewTotalSize  = ulNewSize * sizeof(SLAP_HANDLE);

                break;

            case    BspArray_Object:
                
                ulTotalSize     = pArrayItems->ulLen * sizeof(SLAP_OBJECT);
                ulNewTotalSize  = ulNewSize * sizeof(SLAP_OBJECT);

                break;
            default:
            	break;
        }

        pNewArray   = (PVOID)AnscAllocateMemory(ulNewTotalSize);

        if (!pNewArray)
        {
            status  = ANSC_STATUS_RESOURCES;
        }
        else
        {
            pArrayItems->hBulkData  = (ANSC_HANDLE)pNewArray;
            pArrayItems->ulLen      = ulNewSize;

            AnscCopyMemory(pNewArray, pArray, ulTotalSize);
            AnscFreeMemory(pArray);
        }
    }
    else
    {
        PBSP_TEMPLATE_VAR_OBJECT    *pVars, *pOldVars;
        ULONG                       ulNewItems  = ulNewSize;
        ULONG                       ulOldItems  = pArrayItems->ulLen;

        /* grow array's size */
        pOldVars    = (PBSP_TEMPLATE_VAR_OBJECT *)pArrayItems->hVars;
        
        for (i = 1; i < pArrayItems->numDims; i ++)
        {
            ulOldItems  *= pArrayItems->Dims[i];
            ulNewItems  *= pArrayItems->Dims[i];
        }

        pVars   = 
            (PBSP_TEMPLATE_VAR_OBJECT *)AnscAllocateMemory
                (
                    ulNewItems * sizeof(PBSP_TEMPLATE_VAR_OBJECT)
                );

        if (pVars)
        {
            if (ulOldItems)
            {
                AnscCopyMemory
                    (
                        pVars, 
                        pOldVars, 
                        ulOldItems * sizeof(PBSP_TEMPLATE_VAR_OBJECT)
                    );
                AnscFreeMemory(pOldVars);
            }

            for (i = ulOldItems; i < ulNewItems; i ++)
            {
                pVars[i]    =
                    (PBSP_TEMPLATE_VAR_OBJECT)CreateBspEngVarComponent
                        (
                            NULL, 
                            NULL, 
                            NULL
                        );

                /* if array size is too big, performance is bad */
                /* pVars[i]    = (PBSP_TEMPLATE_VAR_OBJECT)pRt->AcquireVariable((ANSC_HANDLE)pRt); */
            }

            pArrayItems->hVars   = (ANSC_HANDLE)pVars;
            pArrayItems->ulSize  = ulNewSize;
            pArrayItems->ulLen   = ulNewSize;
        }
    }

    return status;
}


