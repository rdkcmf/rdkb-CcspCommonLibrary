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

    module:	bspeng_branch_co_base.c

        For Template Engine Branch Component Object,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic component object functions
        of the string Component Object.

        *   BspTemplateBranchCOCreate
        *   BspTemplateBranchCORemove

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
        BspTemplateBranchCOCreate
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
BspTemplateBranchCOCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hContainerContext);
    UNREFERENCED_PARAMETER(hOwnerContext);
    UNREFERENCED_PARAMETER(hAnscReserved);
    PBSP_TEMPLATE_BRANCH_OBJECT     pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PBSP_TEMPLATE_BRANCH_OBJECT)AnscAllocateMemory(sizeof(BSP_TEMPLATE_BRANCH_OBJECT));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    BspTemplateBranchCOInitialize((ANSC_HANDLE)pMyObject);
    
    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BspTemplateBranchCORemove
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
BspTemplateBranchCORemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_BRANCH_OBJECT     pMyObject    = (PBSP_TEMPLATE_BRANCH_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     pBr;
    BSP_TEMPLATE_BRANCH_TYPE        type;
    PBSP_TEMPLATE_ACV_ITEM          pAcvItem  = NULL;
    PBSP_TEMPLATE_BRANCH_OBJECT     pBrObject;
    SLIST_HEADER                    SList;
    PSINGLE_LINK_ENTRY              pEntry;

    AnscSListInitializeHeader(&SList);

    BSP_TEMPLATE_PUSH_ACV_ITEM(FALSE, pMyObject, &SList);

    while (TRUE)
    {
        pEntry = AnscSListPopEntry(&SList);
        if (!pEntry)
            break;

        pAcvItem    = ACCESS_TEMPLATE_ACV_ITEM(pEntry);
        pBrObject   = (PBSP_TEMPLATE_BRANCH_OBJECT)pAcvItem->pData;
        AnscFreeMemory(pAcvItem);

        /* right branch */
        type = pBrObject->right.type;

        switch (type)
        {
        case BspBranch_eEmpty:
        case BspBranch_eNumber:
        case BspBranch_eReal:
        case BspBranch_eVar:
        case BspBranch_eBranch:
            pBr = (PBSP_TEMPLATE_BRANCH_OBJECT)pBrObject->right.Value.b;
            if (pBr)
            {
                BSP_TEMPLATE_PUSH_ACV_ITEM(FALSE, pBr, &SList);
            }
            break;

        case BspBranch_eString:
            if (pBrObject->right.Value.s)
                AnscFreeMemory(pBrObject->right.Value.s);
            break;

        case BspBranch_eArrayDim:
        case BspBranch_eArrayItem:
        case BspBranch_eApiParams:
            {
                PBSP_TEMPLATE_BRANCH_OBJECT *pBranches;
                ULONG                   Count, i;

                Count       = pBrObject->right.Value.a.Count;
                pBranches   = (PBSP_TEMPLATE_BRANCH_OBJECT *)pBrObject->right.Value.a.pBranch;

                if (pBranches)
                {
                    for (i = 0; i < Count; i ++)
                    {
                        pBr = pBranches[i];
                        BSP_TEMPLATE_PUSH_ACV_ITEM(FALSE, pBr, &SList);
                    }
                }

                AnscFreeMemory(pBrObject->right.Value.a.pBranch);
            }
            break;
        }

        /* left branch */
        type = pBrObject->left.type;

        switch (type)
        {
        case BspBranch_eEmpty:
        case BspBranch_eNumber:
        case BspBranch_eReal:
        case BspBranch_eVar:
        case BspBranch_eBranch:
            pBr = (PBSP_TEMPLATE_BRANCH_OBJECT)pBrObject->left.Value.b;
            if (pBr)
                BSP_TEMPLATE_PUSH_ACV_ITEM(FALSE, pBr, &SList);
            break;

        case BspBranch_eString:
            if (pBrObject->left.Value.s)
                AnscFreeMemory(pBrObject->left.Value.s);
            break;

        case BspBranch_eArrayDim:
        case BspBranch_eArrayItem:
        case BspBranch_eApiParams:
            {
                PBSP_TEMPLATE_BRANCH_OBJECT *pBranches;
                ULONG                   Count, i;

                Count       = pBrObject->left.Value.a.Count;
                pBranches   = (PBSP_TEMPLATE_BRANCH_OBJECT *)pBrObject->left.Value.a.pBranch;

                for (i = 0; i < Count; i ++)
                {
                    pBr = pBranches[i];
                    BSP_TEMPLATE_PUSH_ACV_ITEM(FALSE, pBr, &SList);
                }

                AnscFreeMemory(pBrObject->left.Value.a.pBranch);
            }
            break;
        }

        AnscFreeMemory(pBrObject);
    }

    return  ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
BspTemplateBranchCOInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_BRANCH_OBJECT     pMyObject    = (PBSP_TEMPLATE_BRANCH_OBJECT)hThisObject;

    if ( !pMyObject )
    {
        return ANSC_STATUS_FAILURE;
    }

    pMyObject->left.type        = pMyObject->right.type = BspBranch_eEmpty;
    pMyObject->op               = BspOp_None;
    pMyObject->bTopOp           = FALSE;
    pMyObject->bObjSetProperty  = FALSE;
    
    return ANSC_STATUS_SUCCESS;
}


