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

    module:	bspeng_stack_co_base.c

        For BSP Engine Stack Component Object,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic component object functions
        of the string Component Object.

        *   BspTemplateStackCOCreate
        *   BspTemplateStackCORemove

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
        BspTemplateStackCOCreate
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
BspTemplateStackCOCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hContainerContext);
    UNREFERENCED_PARAMETER(hAnscReserved);
    PBSP_TEMPLATE_STACK_OBJECT      pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PBSP_TEMPLATE_STACK_OBJECT)AnscAllocateMemory(sizeof(BSP_TEMPLATE_STACK_OBJECT));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    pMyObject->Initialize                   = BspTemplateStackCOInitialize;
    pMyObject->Remove                       = BspTemplateStackCORemove;
    pMyObject->hRuntime                     = hOwnerContext;

    pMyObject->Initialize((ANSC_HANDLE)pMyObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BspTemplateStackCORemove
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
BspTemplateStackCORemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_STACK_OBJECT      pMyObject    = (PBSP_TEMPLATE_STACK_OBJECT)hThisObject;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt          = (PBSP_TEMPLATE_RUNTIME_OBJECT)pMyObject->hRuntime;
    ULONG                           i;
    PBSP_TEMPLATE_VAR_OBJECT        pVar, *pVars;

    pVars   = (PBSP_TEMPLATE_VAR_OBJECT *)pMyObject->hVars;
    for (i = 0; i < pMyObject->Size; i ++)
    {
        pVar    = pVars[i];
        /* pVar->Remove((ANSC_HANDLE)pVar); */
        pRt->ReleaseVariable((ANSC_HANDLE)pRt, (ANSC_HANDLE)pVar);
    }

    AnscFreeMemory(pVars);

    AnscFreeMemory(pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
BspTemplateStackCOInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_STACK_OBJECT      pMyObject    = (PBSP_TEMPLATE_STACK_OBJECT)hThisObject;
    PBSP_TEMPLATE_RUNTIME_OBJECT    pRt          = NULL; /*RDKB-6141, CID-24279, require null check beofore use*/
    PBSP_TEMPLATE_VAR_OBJECT        *pVars       = NULL;
    PBSP_TEMPLATE_VAR_OBJECT        pVar         = NULL;
    ULONG                           i;

    if ( !pMyObject )
    {
        return ANSC_STATUS_FAILURE;
    }
    pRt          = (PBSP_TEMPLATE_RUNTIME_OBJECT)pMyObject->hRuntime;

    pMyObject->Add                          = BspTemplateStackAdd;
    pMyObject->GetAt                        = BspTemplateStackGetAt;
    pMyObject->GetCount                     = BspTemplateStackGetCount;
    pMyObject->GetBase                      = BspTemplateStackGetBase;
    pMyObject->GetTop                       = BspTemplateStackGetTop;
    pMyObject->GetState                     = BspTemplateStackGetState;
    pMyObject->NewFunction                  = BspTemplateStackNewFunction;
    pMyObject->DeleteFunction               = BspTemplateStackDeleteFunction;
    pMyObject->GetBufferAt                  = BspTemplateStackGetBufferAt;
    pMyObject->Dump                         = BspTemplateStackDump;

    pMyObject->Initialize                   = BspTemplateStackCOInitialize;
    pMyObject->Remove                       = BspTemplateStackCORemove;

    pMyObject->Base     = 0;
    pMyObject->Top      = 0;
    pMyObject->Size     = BSP_TEMPLATE_STACK_GROWTH;

    pVars   = 
        (PBSP_TEMPLATE_VAR_OBJECT *)AnscAllocateMemory
            (
                sizeof(PBSP_TEMPLATE_VAR_OBJECT) * pMyObject->Size
            );

    if (pVars)
    {
        for (i = 0; i < pMyObject->Size; i ++)
        {
            /* pVar        = (PBSP_TEMPLATE_VAR_OBJECT)CreateBspEngVarComponent(NULL, NULL, NULL); */
            pVar        = (PBSP_TEMPLATE_VAR_OBJECT)pRt->AcquireVariable((ANSC_HANDLE)pRt);

            pVar->bTemp = FALSE;
            pVars[i]    = pVar;
        }
    }

    pMyObject->hVars    = (ANSC_HANDLE)pVars;

    return ANSC_STATUS_SUCCESS;
}


