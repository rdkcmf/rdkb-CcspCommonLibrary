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

    module:	bspeng_runtime_co_process.c

        For BSP Engine Runtime Component Object,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the string Component Object.

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

        void
        BspTemplateRuntimeSetEnv
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hEnv
            )

    description:

        This function is called to set environment object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hEnv
                Environment object.

    return:     void.

**********************************************************************/

void
BspTemplateRuntimeSetEnv
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hEnv
    )
{
    PBSP_TEMPLATE_RUNTIME_OBJECT    pMyObject = (PBSP_TEMPLATE_RUNTIME_OBJECT)hThisObject;

    pMyObject->hEnv     = hEnv;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateRuntimeSetWriter
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hWriter
            )

    description:

        This function is called to set environment object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hWriter
                Writer object.

    return:     void.

**********************************************************************/

void
BspTemplateRuntimeSetWriter
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWriter
    )
{
    PBSP_TEMPLATE_RUNTIME_OBJECT    pMyObject = (PBSP_TEMPLATE_RUNTIME_OBJECT)hThisObject;

    pMyObject->hWriter  = hWriter;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateRuntimeSetOutputset
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hOutputset
            )

    description:

        This function is called to set environment object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hOutputset
                Output set object.

    return:     void.

**********************************************************************/

void
BspTemplateRuntimeSetOutputset
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOutputset
    )
{
    PBSP_TEMPLATE_RUNTIME_OBJECT    pMyObject = (PBSP_TEMPLATE_RUNTIME_OBJECT)hThisObject;

    pMyObject->hOutputSet   = hOutputset;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateRuntimeGetStack
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to get stack object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle to stack object.

**********************************************************************/

ANSC_HANDLE
BspTemplateRuntimeGetStack
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_RUNTIME_OBJECT    pMyObject = (PBSP_TEMPLATE_RUNTIME_OBJECT)hThisObject;

    return pMyObject->hStack;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateRuntimeClearReturnValue
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to clear return value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     void.

**********************************************************************/

void
BspTemplateRuntimeClearReturnValue
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_RUNTIME_OBJECT    pMyObject   = (PBSP_TEMPLATE_RUNTIME_OBJECT)hThisObject;
    PBSP_TEMPLATE_VAR_OBJECT        pVar        = (PBSP_TEMPLATE_VAR_OBJECT)pMyObject->hReturnValue;

    pVar->Reset((ANSC_HANDLE)pVar);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateRuntimeSetReturnValue
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hValue
            )

    description:

        This function is called to set environment object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hValue
                Return value.

    return:     status of operation.

**********************************************************************/

BOOL
BspTemplateRuntimeSetReturnValue
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hValue
    )
{
    PBSP_TEMPLATE_RUNTIME_OBJECT    pMyObject   = (PBSP_TEMPLATE_RUNTIME_OBJECT)hThisObject;
    PBSP_TEMPLATE_VAR_OBJECT        pVar        = (PBSP_TEMPLATE_VAR_OBJECT)pMyObject->hReturnValue;

    pVar->Assign((ANSC_HANDLE)pVar, hValue);

    return TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateRuntimeGetReturnValue
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to get return value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle to return value.

**********************************************************************/

ANSC_HANDLE
BspTemplateRuntimeGetReturnValue
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_RUNTIME_OBJECT    pMyObject   = (PBSP_TEMPLATE_RUNTIME_OBJECT)hThisObject;

    return pMyObject->hReturnValue;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateRuntimeAcquireVariable
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to acquire a variable object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle to variable object.

**********************************************************************/

ANSC_HANDLE
BspTemplateRuntimeAcquireVariable
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_RUNTIME_OBJECT    pMyObject   = (PBSP_TEMPLATE_RUNTIME_OBJECT)hThisObject;
    PSINGLE_LINK_ENTRY              pEntry      = NULL;
    PBSP_TEMPLATE_VAR_POOL_ITEM     pVarItem;
    PBSP_TEMPLATE_VAR_OBJECT        pVar        = NULL;

    /* search available variable object */
    pEntry  = AnscSListGetFirstEntry(&pMyObject->VariablePool);

    while (pEntry)
    {
        pVarItem    = ACCESS_BSP_TEMPLATE_VAR_POOL_ITEM(pEntry);
        pVar        = pVarItem->pVar;

        if (!pVarItem->bInUse)
        {
            pVarItem->bInUse    = TRUE;

            break;
        }

        pEntry      = AnscSListGetNextEntry(pEntry);
    }

    if (!pEntry)
    {
        pVarItem    = (PBSP_TEMPLATE_VAR_POOL_ITEM)AnscAllocateMemory(sizeof(BSP_TEMPLATE_VAR_POOL_ITEM));
        pVar        = (PBSP_TEMPLATE_VAR_OBJECT)CreateBspEngVarComponent(NULL, (ANSC_HANDLE)pMyObject, NULL);

        if (pVar && pVarItem)
        {
            pVarItem->pVar      = pVar;
            pVarItem->bInUse    = TRUE;

            AnscSListPushEntry(&pMyObject->VariablePool, &pVarItem->Linkage);
        }
        else
        {
            if (pVarItem)
            {
                AnscFreeMemory(pVarItem);
            }

            if (pVar)
            {
                pVar->Remove((ANSC_HANDLE)pVar);
            }

            pVarItem    = NULL;
            pVar        = NULL;
        }
    }

    return (ANSC_HANDLE)pVar;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BspTemplateRuntimeReleaseVariable
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to acquire a variable object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hVar
                Handle to Variable object.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BspTemplateRuntimeReleaseVariable
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_RUNTIME_OBJECT    pMyObject   = (PBSP_TEMPLATE_RUNTIME_OBJECT)hThisObject;
    PSINGLE_LINK_ENTRY              pEntry      = NULL;
    PBSP_TEMPLATE_VAR_POOL_ITEM     pVarItem;
    PBSP_TEMPLATE_VAR_OBJECT        pVar        = NULL;

    /* search variable object in pool */
    pEntry  = AnscSListGetFirstEntry(&pMyObject->VariablePool);

    while (pEntry)
    {
        pVarItem    = ACCESS_BSP_TEMPLATE_VAR_POOL_ITEM(pEntry);
        pVar        = pVarItem->pVar;

        if (pVarItem->pVar == hVar)
        {
            pVarItem->bInUse    = FALSE;

            break;
        }

        pEntry      = AnscSListGetNextEntry(pEntry);
    }

    /* oops! anyway we take care of this variable */
    if (!pEntry)
    {
/*
        pVarItem    = (PBSP_TEMPLATE_VAR_POOL_ITEM)AnscAllocateMemory(sizeof(BSP_TEMPLATE_VAR_POOL_ITEM));

        if (pVarItem)
        {
            pVarItem->pVar      = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
            pVarItem->bInUse    = FALSE;

            AnscSListPushEntry(&pMyObject->VariablePool, &pVarItem->Linkage);
        }
        else
*/
        {
            pVar    = (PBSP_TEMPLATE_VAR_OBJECT)hVar;

            pVar->Remove((ANSC_HANDLE)pVar);

            return ANSC_STATUS_SUCCESS;
        }
    }

    /* reset this variable */
    pVar    = (PBSP_TEMPLATE_VAR_OBJECT)hVar;
    pVar->Reset((ANSC_HANDLE)pVar);

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
BspTemplateRuntimeClearSlapParamRef
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_RUNTIME_OBJECT    pMyObject   = (PBSP_TEMPLATE_RUNTIME_OBJECT)hThisObject;

    pMyObject->SlapParamRefCount    = 0;

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
BspTemplateRuntimeSetSlapParamRef
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE*                hParamRef,
        ULONG                       ulParamCount
    )
{
    PBSP_TEMPLATE_RUNTIME_OBJECT    pMyObject   = (PBSP_TEMPLATE_RUNTIME_OBJECT)hThisObject;
    ULONG                           ulCount     = ulParamCount;
    PBSP_TEMPLATE_VAR_OBJECT        *pVars      = (PBSP_TEMPLATE_VAR_OBJECT *)hParamRef;
    PBSP_TEMPLATE_VAR_OBJECT        *pRefs      = (PBSP_TEMPLATE_VAR_OBJECT *)pMyObject->SlapParamRef;

    if (ulCount > BSPENG_RUNTIME_MAX_SLAP_PARAMETER_REF)
    {
        ulCount = BSPENG_RUNTIME_MAX_SLAP_PARAMETER_REF;
    }

    if (ulCount != 0)
    {
        AnscCopyMemory(pRefs, pVars, sizeof(PBSP_TEMPLATE_VAR_OBJECT) * ulCount);
        pMyObject->SlapParamRefCount    = ulCount;
    }
    else
    {
        pMyObject->SlapParamRefCount    = 0;
    }

    return ANSC_STATUS_SUCCESS;
}


ANSC_HANDLE
BspTemplateRuntimeGetSlapParamRef
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pulParamRefCount
    )
{
    PBSP_TEMPLATE_RUNTIME_OBJECT    pMyObject   = (PBSP_TEMPLATE_RUNTIME_OBJECT)hThisObject;

    if (pulParamRefCount)
    {
        *pulParamRefCount   = pMyObject->SlapParamRefCount;
    }

    return (ANSC_HANDLE)pMyObject->SlapParamRef;
}



