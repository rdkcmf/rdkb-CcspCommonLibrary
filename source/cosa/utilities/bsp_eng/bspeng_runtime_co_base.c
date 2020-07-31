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

    module:	bspeng_runtime_co_base.c

        For BSP Engine Runtime Component Object,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic component object functions
        of the string Component Object.

        *   BspTemplateRuntimeCOCreate
        *   BspTemplateRuntimeCORemove

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
        BspTemplateRuntimeCOCreate
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
BspTemplateRuntimeCOCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hContainerContext);
    UNREFERENCED_PARAMETER(hOwnerContext);
    UNREFERENCED_PARAMETER(hAnscReserved);
    PBSP_TEMPLATE_RUNTIME_OBJECT    pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PBSP_TEMPLATE_RUNTIME_OBJECT)AnscAllocateMemory(sizeof(BSP_TEMPLATE_RUNTIME_OBJECT));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    pMyObject->Initialize                   = BspTemplateRuntimeCOInitialize;
    pMyObject->Remove                       = BspTemplateRuntimeCORemove;

    pMyObject->Initialize((ANSC_HANDLE)pMyObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BspTemplateRuntimeCORemove
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
BspTemplateRuntimeCORemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_RUNTIME_OBJECT    pMyObject    = (PBSP_TEMPLATE_RUNTIME_OBJECT)hThisObject;
    PBSP_TEMPLATE_VAR_OBJECT        pReturnVar   = (PBSP_TEMPLATE_VAR_OBJECT)pMyObject->hReturnValue;
    PBSP_TEMPLATE_STACK_OBJECT      pStack;
    PANSC_PTR_ARRAY_OBJECT          pPtrArray;

    pStack  = (PBSP_TEMPLATE_STACK_OBJECT)pMyObject->hStack;

    pStack->Remove((ANSC_HANDLE)pStack);

    pPtrArray = (PANSC_PTR_ARRAY_OBJECT)pMyObject->hExecute;

    if (pPtrArray)
    {
        ULONG                       ulTmplCount, i;
        PBSP_TEMPLATE_OBJECT        pTmpl;

        ulTmplCount = pPtrArray->GetSize((ANSC_HANDLE)pPtrArray);
        for (i = 0; i < ulTmplCount; i ++)
        {
        	pTmpl = (PBSP_TEMPLATE_OBJECT)pPtrArray->GetAt((ANSC_HANDLE)pPtrArray, i);
            pTmpl->Remove((ANSC_HANDLE)pTmpl);
        }

        pPtrArray->Remove((ANSC_HANDLE)pPtrArray);
    }

    pReturnVar->Remove((ANSC_HANDLE)pReturnVar);

    if (TRUE)
    {
        PBSP_TEMPLATE_VAR_POOL_ITEM pVarItem;
        PSINGLE_LINK_ENTRY          pEntry;
        PBSP_TEMPLATE_VAR_OBJECT    pVar;

        while (TRUE)
        {
            pEntry  = AnscSListPopEntry(&pMyObject->VariablePool);

            if (!pEntry)
            {
                break;
            }

            pVarItem = ACCESS_BSP_TEMPLATE_VAR_POOL_ITEM(pEntry);
            pVar     = pVarItem->pVar;

            pVar->Remove((ANSC_HANDLE)pVar);
            AnscFreeMemory(pVarItem);
        }
    }

    AnscFreeMemory((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
BspTemplateRuntimeCOInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_RUNTIME_OBJECT    pMyObject    = (PBSP_TEMPLATE_RUNTIME_OBJECT)hThisObject;
    PBSP_TEMPLATE_STACK_OBJECT      pStack       = NULL;

    if ( !pMyObject )
    {
        return ANSC_STATUS_SUCCESS;
    }

    pMyObject->Initialize                   = BspTemplateRuntimeCOInitialize;
    pMyObject->Remove                       = BspTemplateRuntimeCORemove;

    pMyObject->SetEnv                       = BspTemplateRuntimeSetEnv;
    pMyObject->SetWriter                    = BspTemplateRuntimeSetWriter;
    pMyObject->SetOutputset                 = BspTemplateRuntimeSetOutputset;
    pMyObject->GetStack                     = BspTemplateRuntimeGetStack;
    pMyObject->ClearReturnValue             = BspTemplateRuntimeClearReturnValue;
    pMyObject->SetReturnValue               = BspTemplateRuntimeSetReturnValue;
    pMyObject->GetReturnValue               = BspTemplateRuntimeGetReturnValue;

    pMyObject->AcquireVariable              = BspTemplateRuntimeAcquireVariable;
    pMyObject->ReleaseVariable              = BspTemplateRuntimeReleaseVariable;

    pMyObject->SetSlapParamRef              = BspTemplateRuntimeSetSlapParamRef;
    pMyObject->GetSlapParamRef              = BspTemplateRuntimeGetSlapParamRef;


    AnscInitializeSList(&pMyObject->VariablePool);

    /* create variable pool */
    if (TRUE)
    {
        PBSP_TEMPLATE_VAR_POOL_ITEM pVarItem;
        PBSP_TEMPLATE_VAR_OBJECT    pVar;
        ULONG                       i;

        for (i = 0; i < BSPENG_RUNTIME_INIT_VARIABLE_POOL_SIZE; i ++)
        {
            pVarItem    = (PBSP_TEMPLATE_VAR_POOL_ITEM)AnscAllocateMemory(sizeof(BSP_TEMPLATE_VAR_POOL_ITEM));
            pVar        = (PBSP_TEMPLATE_VAR_OBJECT)CreateBspEngVarComponent(NULL, (ANSC_HANDLE)pMyObject, NULL);

            if (pVar && pVarItem)
            {
                pVarItem->pVar      = pVar;
                pVarItem->bInUse    = FALSE;

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
            }
        }
    }

    /* create a stack object */
    pStack  = (PBSP_TEMPLATE_STACK_OBJECT)CreateBspEngStackComponent(NULL, pMyObject, NULL);

    pMyObject->hStack       = (ANSC_HANDLE)pStack;
    pMyObject->ulArgCount   = 0;
    pMyObject->ulNestCount  = 0;
    pMyObject->Stop         = 0;
    pMyObject->LoopLimit    = 1000;
    pMyObject->InExecute    = FALSE;

    pMyObject->hExecute     = AnscCreatePtrArrayComponent(NULL, NULL, NULL);
    pMyObject->hReturnValue = CreateBspEngVarComponent(NULL, NULL, NULL);

    return ANSC_STATUS_SUCCESS;
}


