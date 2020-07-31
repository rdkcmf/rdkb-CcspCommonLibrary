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

    module:	bspeng_var_co_base.c

        For BSP Engine Var Component Object,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic component object functions
        of the string Component Object.

        *   BspTemplateVarCOCreate
        *   BspTemplateVarCORemove

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        05/14/03    initial revision.

**********************************************************************/


#include "bspeng_co_global.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        BspTemplateVarCOCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the String Component object and
        initializes the member variables and functions.

    argument:   ANSC_HANDLE                 hContainerContext
                This handle is used by the container object to interact
                with the outside world. It could be the real container
                or an target object.

                ANSC_HANDLE                 hOwnerContext
                This handle is passed in by the owner of this object.

                ANSC_HANDLE                 hAnscReserved
                This handle is passed in by the owner of this object.

    return:     newly created string component object.

**********************************************************************/

ANSC_HANDLE
BspTemplateVarCOCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hContainerContext);
    UNREFERENCED_PARAMETER(hAnscReserved);
    PBSP_TEMPLATE_VAR_OBJECT        pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PBSP_TEMPLATE_VAR_OBJECT)AnscAllocateMemory(sizeof(BSP_TEMPLATE_VAR_OBJECT));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    pMyObject->Remove                       = BspTemplateVarCORemove;
    pMyObject->Initialize                   = BspTemplateVarCOInitialize;

    pMyObject->Initialize((ANSC_HANDLE)pMyObject);

    pMyObject->hRuntime     = hOwnerContext;

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BspTemplateVarCORemove
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function destroys the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BspTemplateVarCORemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_VAR_OBJECT        pMyObject    = (PBSP_TEMPLATE_VAR_OBJECT)hThisObject;

/*
    if (pMyObject->Size && !pMyObject->bTemp)
        AnscFreeMemory(pMyObject->Value.str);
    else
    if (pMyObject->Type == BspVar_ArrayRef)
    {
        PBSP_TEMPLATE_VAR_ARRAY_REF pArrayRef;

        pArrayRef   = (PBSP_TEMPLATE_VAR_ARRAY_REF)pMyObject->Value.ref;

        if (pArrayRef)
            AnscFreeMemory(pArrayRef);
    }
    else
    if (pMyObject->Type == BspVar_Object)
    {
        SlapFreeVariable(((PSLAP_VARIABLE)pMyObject->Value.obj));
    }
*/

    pMyObject->Reset((ANSC_HANDLE)pMyObject);

    AnscFreeMemory(pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
BspTemplateVarCOInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_VAR_OBJECT        pMyObject    = (PBSP_TEMPLATE_VAR_OBJECT)hThisObject;

    if ( !pMyObject )
    {
        return ANSC_STATUS_FAILURE;
    }

    pMyObject->Increase                     = BspTemplateVarIncrease;
    pMyObject->Decrease                     = BspTemplateVarDecrease;
    pMyObject->GetLong                      = BspTemplateVarGetLong;
    pMyObject->Assign                       = BspTemplateVarAssign;
    pMyObject->Greater                      = BspTemplateVarGreater;
    pMyObject->Equal                        = BspTemplateVarEqual;
    pMyObject->Plus                         = BspTemplateVarPlus;
    pMyObject->Minus                        = BspTemplateVarMinus;
    pMyObject->Multiply                     = BspTemplateVarMultiply;
    pMyObject->Divide                       = BspTemplateVarDivide;
    pMyObject->Mod                          = BspTemplateVarMod;
    pMyObject->StrCat                       = BspTemplateVarStrCat;
    pMyObject->BitOr                        = BspTemplateVarBitOr;
    pMyObject->BitAnd                       = BspTemplateVarBitAnd;
    pMyObject->BitXor                       = BspTemplateVarBitXor;
    pMyObject->ShiftLeft                    = BspTemplateVarShiftLeft;
    pMyObject->ShiftRight                   = BspTemplateVarShiftRight;
    pMyObject->Pos                          = BspTemplateVarPos;
    pMyObject->Neg                          = BspTemplateVarNeg;
    pMyObject->StrToNum                     = BspTemplateVarStrToNum;
    pMyObject->Reset                        = BspTemplateVarReset;
    pMyObject->GetContentType               = BspTemplateVarGetContentType;
    pMyObject->SetContentType               = BspTemplateVarSetContentType;
    pMyObject->SetContentType2              = BspTemplateVarSetContentType2;
    pMyObject->GetBulkDataAt                = BspTemplateVarGetBulkDataAt;
    pMyObject->SetArraySize                 = BspTemplateVarSetArraySize;

    pMyObject->Remove                       = BspTemplateVarCORemove;
    pMyObject->Initialize                   = BspTemplateVarCOInitialize;

    pMyObject->Type             = BspVar_Number;
    pMyObject->Value.num        = 0;
    pMyObject->Size             = 0;
    pMyObject->bTemp            = TRUE;

    return ANSC_STATUS_SUCCESS;
}


