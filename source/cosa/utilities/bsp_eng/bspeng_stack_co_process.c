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

    module:	bspeng_stack_co_process.c

        For BSP Engine Stack Component Object,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the BSP engine stack Object.

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


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateStackAdd
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulCount
            )

    description:

        This function is called to add given count of variables
        on the stack object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulCount
                The number of variables to be added.

    return:     The first variabled newly added.

**********************************************************************/

ANSC_HANDLE
BspTemplateStackAdd
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCount
    )
{
    PBSP_TEMPLATE_STACK_OBJECT      pMyObject = (PBSP_TEMPLATE_STACK_OBJECT)hThisObject;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt       = (PBSP_TEMPLATE_RUNTIME_OBJECT)pMyObject->hRuntime;
    PBSP_TEMPLATE_VAR_OBJECT        *pVars, *pNewVars, pVar;
    ULONG                           i;

    pVars   = (PBSP_TEMPLATE_VAR_OBJECT *)pMyObject->hVars;

    if (pMyObject->Top + ulCount > pMyObject->Size)
    {
        ULONG                       ulCurSize, ulAdded;

        ulCurSize       = pMyObject->Size;
        pMyObject->Size = pMyObject->Top + ulCount + BSP_TEMPLATE_STACK_GROWTH;
        ulAdded         = pMyObject->Size - ulCurSize;

        pVars       = (PBSP_TEMPLATE_VAR_OBJECT *)pMyObject->hVars;

/*
        pNewVars    = 
            (PBSP_TEMPLATE_VAR_OBJECT *)
                AnscReAllocMemory(pVars, pMyObject->Size * sizeof(PBSP_TEMPLATE_VAR_OBJECT));
*/

        pNewVars    =
            (PBSP_TEMPLATE_VAR_OBJECT *)AnscMemUtilRealloc
                (
                    pVars, 
                    ulCurSize * sizeof(PBSP_TEMPLATE_VAR_OBJECT), 
                    pMyObject->Size * sizeof(PBSP_TEMPLATE_VAR_OBJECT)
                );

        if (pNewVars)
        {
            pVars               = pNewVars;
            pMyObject->hVars    = (ANSC_HANDLE)pVars;

            for (i = 0; i < ulAdded; i ++)
            {
                /* pVar     = (PBSP_TEMPLATE_VAR_OBJECT)CreateBspEngVarComponent(NULL, NULL, NULL); */
                pVar        = (PBSP_TEMPLATE_VAR_OBJECT)pRt->AcquireVariable((ANSC_HANDLE)pRt);

                pVar->bTemp = FALSE;
                pVars[ulCurSize + i]   = (ANSC_HANDLE)pVar;
            }
        }
    }

    pMyObject->Top += ulCount;

    for (i = 0; i < ulCount; i ++)
    {
        pVar = pVars[pMyObject->Top - ulCount + i];

        pVar->Reset((ANSC_HANDLE)pVar);
    }

    return pVars[pMyObject->Top - ulCount];
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateStackGetAt
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulIndex
            )

    description:

        This function is called to get the stack variable at
        the specified position.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulIndex
                The index to the desired stack variable.

    return:     handle to the stack variable.

**********************************************************************/

ANSC_HANDLE
BspTemplateStackGetAt
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex
    )
{
    PBSP_TEMPLATE_STACK_OBJECT      pMyObject = (PBSP_TEMPLATE_STACK_OBJECT)hThisObject;
    PBSP_TEMPLATE_VAR_OBJECT        *pVars;

    pVars = (PBSP_TEMPLATE_VAR_OBJECT *)pMyObject->hVars;

    if ((int)ulIndex < 0 || ulIndex >= pMyObject->Top)
        return NULL;

    return pVars[ulIndex];
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        BspTemplateStackGetCount
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to get the number of variables
        on the stack.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     the number of stack variables.

**********************************************************************/

ULONG
BspTemplateStackGetCount
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_STACK_OBJECT      pMyObject = (PBSP_TEMPLATE_STACK_OBJECT)hThisObject;

    return pMyObject->Top - pMyObject->Base;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        BspTemplateStackGetBase
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to get base position of stack.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     base position.

**********************************************************************/

ULONG
BspTemplateStackGetBase
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_STACK_OBJECT      pMyObject = (PBSP_TEMPLATE_STACK_OBJECT)hThisObject;

    return pMyObject->Base;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        BspTemplateStackGetTop
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to get the top position.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     top position.

**********************************************************************/

ULONG
BspTemplateStackGetTop
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_STACK_OBJECT      pMyObject = (PBSP_TEMPLATE_STACK_OBJECT)hThisObject;

    return pMyObject->Top;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateStackGetState
            (
                ANSC_HANDLE                 hThisObject,
                PBSP_TEMPLATE_STATE         pState
            )

    description:

        This function is called to get state of the stack.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PBSP_TEMPLATE_STATE         pState
                Stack state.

    return:     void.

**********************************************************************/

void
BspTemplateStackGetState
    (
        ANSC_HANDLE                 hThisObject,
        PBSP_TEMPLATE_STATE         pState
    )
{
    PBSP_TEMPLATE_STACK_OBJECT      pMyObject = (PBSP_TEMPLATE_STACK_OBJECT)hThisObject;

    if (pState)
    {
        pState->base    = pMyObject->Base;
        pState->top     = pMyObject->Top;
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateStackNewFunction
            (
                ANSC_HANDLE                 hThisObject,
                PBSP_TEMPLATE_STATE         pState
            )

    description:

        This function is called to change stack state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PBSP_TEMPLATE_STATE         pState
                New state to be set.

    return:     void.

**********************************************************************/

void
BspTemplateStackNewFunction
    (
        ANSC_HANDLE                 hThisObject,
        PBSP_TEMPLATE_STATE         pState
    )
{
    PBSP_TEMPLATE_STACK_OBJECT      pMyObject = (PBSP_TEMPLATE_STACK_OBJECT)hThisObject;

    if (pState)
        pMyObject->Base = pState->top;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateStackAdd
            (
                ANSC_HANDLE                 hThisObject,
                PBSP_TEMPLATE_STATE         pState
            )

    description:

        This function is called to adjust stack state when a
        template or procedure has been executed.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PBSP_TEMPLATE_STATE         pState
                Stack state to be restored.

    return:     void.

**********************************************************************/

void
BspTemplateStackDeleteFunction
    (
        ANSC_HANDLE                 hThisObject,
        PBSP_TEMPLATE_STATE         pState
    )
{
    PBSP_TEMPLATE_STACK_OBJECT      pMyObject = (PBSP_TEMPLATE_STACK_OBJECT)hThisObject;
    PBSP_TEMPLATE_VAR_OBJECT        *pVars, pVar;
    ULONG                           i;

    if (!pState)
        return;

    pVars   = (PBSP_TEMPLATE_VAR_OBJECT *)pMyObject->hVars;
    for (i = pMyObject->Base; i < pMyObject->Top; i ++)
    {
        pVar    = pVars[i];

		/* reset the variable */
        pVar->Reset((ANSC_HANDLE)pVar);
    }

    pMyObject->Base = pState->base;
    pMyObject->Top  = pState->top;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateStackGetBufferAt
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulIndex
            )

    description:

        This function is called to get variables from specified
        position.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulIndex
                The position where desired variables start.

    return:     handle to desired stack variables.

**********************************************************************/

ANSC_HANDLE
BspTemplateStackGetBufferAt

    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex
    )
{
    PBSP_TEMPLATE_STACK_OBJECT      pMyObject = (PBSP_TEMPLATE_STACK_OBJECT)hThisObject;
    PBSP_TEMPLATE_VAR_OBJECT        *pVars;

    if ((int)ulIndex < 0 || ulIndex >= pMyObject->Top)
        return NULL;

    pVars   = (PBSP_TEMPLATE_VAR_OBJECT *)pMyObject->hVars;

    return (ANSC_HANDLE)(&pVars[ulIndex]);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateStackDump
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to dump the stack variables.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     void.

**********************************************************************/

void
BspTemplateStackDump
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_STACK_OBJECT      pMyObject = (PBSP_TEMPLATE_STACK_OBJECT)hThisObject;
    PBSP_TEMPLATE_VAR_OBJECT        *pVars, pVal;
    ULONG                           i, ulCount;

    ulCount = pMyObject->Top;
    pVars   = (PBSP_TEMPLATE_VAR_OBJECT *)pMyObject->hVars;

    for (i = 0; i < ulCount; i ++)
    {
        AnscTrace("%4lu: ", i);

        pVal = pVars[i];

        switch (pVal->Type)
        {
        case BspVar_Number:
            AnscTrace("[Number]: %ld", pVal->Value.num);
            break;

        case BspVar_UNumber:
            AnscTrace("[Unsigned Number]: %ld", pVal->Value.num);
            break;

        case BspVar_Real:
            AnscTrace("[Real]: %.8ld", pVal->Value.real);
            break;

        case BspVar_String:
            AnscTrace("[String]: %s", pVal->Value.str);
            break;

        case BspVar_Object:
            AnscTrace("[Object]: 0x%p", pVal->Value.obj);
            break;

        case BspVar_Ref:
            AnscTrace("[Variable Ref]: %ld", pVal->Value.num);
            break;

        case BspVar_Array:
            AnscTrace
                (
                    "[Array]: max len = %lu, len = %lu", 
                    pVal->Value.arrayItems.ulSize, 
                    pVal->Value.arrayItems.ulLen
                );
            break;

        default:
            AnscTrace("[unknown type]");
            break;
        }

        AnscTrace("\r\n");
    }
}




