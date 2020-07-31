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

    module:	bspeng_branch_co_process.c

        For BSP Engine Branch Component Object,
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
        BspTemplateBranchSetLeft
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hChildBranch
            )

    description:

        This function is called to set left branch.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hChildBranch
                Child branch object.

    return:     void.

**********************************************************************/

void
BspTemplateBranchSetLeft
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hChildBranch
    )
{
    PBSP_TEMPLATE_BRANCH_OBJECT     pMyObject = (PBSP_TEMPLATE_BRANCH_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     pLeft     = (PBSP_TEMPLATE_BRANCH_OBJECT)hChildBranch;

    if (!pLeft || pLeft->op != BspOp_None)
    {
        pMyObject->left.type    = BspBranch_eBranch;
        pMyObject->left.Value.b = hChildBranch;
    }
    else
    {
        switch (pMyObject->left.type        = pLeft->right.type)
        {
        case BspBranch_eString:
            pMyObject->left.Value.s         = (PCHAR)AnscDupString((PUCHAR)pLeft->right.Value.s);
            break;

        case BspBranch_eNumber:
            pMyObject->left.Value.n         = pLeft->right.Value.n;
            break;

        case BspBranch_eReal:
            pMyObject->left.Value.r         = pLeft->right.Value.r;
            break;

        case BspBranch_eVar:
            pMyObject->left.Value.n         = pLeft->right.Value.n;
            break;

        case BspBranch_eArrayDim:
            pMyObject->left.Value.a         = pLeft->right.Value.a;
            pLeft->right.Value.a.pBranch    = NULL;
            break;
        case BspBranch_eBranch:
        case BspBranch_eEmpty:
        case BspBranch_eArrayItem:
        case BspBranch_eApiParams:
        default:
            break;
        }

        BspTemplateBranchCORemove((ANSC_HANDLE)pLeft);
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateBranchSetRight
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hChildBranch
            )

    description:

        This function is called to set right branch.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hChildBranch
                Child branch object.

    return:     void.

**********************************************************************/

void
BspTemplateBranchSetRight
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hChildBranch
    )
{
    PBSP_TEMPLATE_BRANCH_OBJECT     pMyObject = (PBSP_TEMPLATE_BRANCH_OBJECT)hThisObject;
    PBSP_TEMPLATE_BRANCH_OBJECT     pRight    = (PBSP_TEMPLATE_BRANCH_OBJECT)hChildBranch;

    if (!pRight || pRight->op != BspOp_None)
    {
        pMyObject->right.type       = BspBranch_eBranch;
        pMyObject->right.Value.b    = hChildBranch;
    }
    else
    {
        switch (pMyObject->right.type = pRight->right.type)
        {
        case BspBranch_eString:
            pMyObject->right.Value.s        = (PCHAR)AnscDupString((PUCHAR)pRight->right.Value.s);
            break;

        case BspBranch_eNumber:
            pMyObject->right.Value.n        = pRight->right.Value.n;
            break;

        case BspBranch_eReal:
            pMyObject->right.Value.r        = pRight->right.Value.r;
            break;

        case BspBranch_eVar:
            pMyObject->right.Value.n        = pRight->right.Value.n;
            break;

        case BspBranch_eArrayDim:
        case BspBranch_eApiParams:
            pMyObject->right.Value.a        = pRight->right.Value.a;
            pRight->right.Value.a.pBranch   = NULL;
            pRight->right.Value.a.Count     = 0;
            break;
        case BspBranch_eBranch:
        case BspBranch_eEmpty:
        case BspBranch_eArrayItem:
        default:
            break;
        }

        BspTemplateBranchCORemove((ANSC_HANDLE)pRight);
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateBranchSetOp
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hChildBranch
            )

    description:

        This function is called to set operation code.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                BSP_TEMPLATE_OPERATOR       op
                Operation code.

    return:     void.

**********************************************************************/

void
BspTemplateBranchSetOp
    (
        ANSC_HANDLE                 hThisObject,
        BSP_TEMPLATE_OPERATOR       op
    )
{
    PBSP_TEMPLATE_BRANCH_OBJECT     pMyObject = (PBSP_TEMPLATE_BRANCH_OBJECT)hThisObject;

    pMyObject->op = op;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateBranchSetLeft
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hArchive
            )

    description:

        This function is called to save this object into archive.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hArchive
                Archive object.

    return:     status of operation.

**********************************************************************/

BOOL
BspTemplateBranchStore
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hArchive
    )
{
    PBSP_TEMPLATE_BRANCH_OBJECT     pMyObject = (PBSP_TEMPLATE_BRANCH_OBJECT)hThisObject;
    PBSP_TEMPLATE_ARCHIVE_OBJECT    pArchive  = (PBSP_TEMPLATE_ARCHIVE_OBJECT)hArchive;
    PBSP_TEMPLATE_BRANCH_DATA       pBrData;
    BOOL                            bSucc     = FALSE;
    SLIST_HEADER                    SList;
    PSINGLE_LINK_ENTRY              pEntry;
    PBSP_TEMPLATE_BRANCH_OBJECT     pBr;
    PBSP_TEMPLATE_ACV_ITEM          pAcvItem  = NULL;
    BOOL                            bBrData   = FALSE;

    AnscSListInitializeHeader(&SList);

    BSP_TEMPLATE_PUSH_ACV_ITEM(FALSE, pMyObject, &SList);

    while (TRUE)
    {
        pEntry = AnscSListPopEntry(&SList);
        if (!pEntry)
            break;

        pAcvItem    = ACCESS_TEMPLATE_ACV_ITEM(pEntry);
        pBr         = (PBSP_TEMPLATE_BRANCH_OBJECT)pAcvItem->pData;
        pBrData     = (PBSP_TEMPLATE_BRANCH_DATA)pAcvItem->pData;
        bBrData     = pAcvItem->bBranchData;
        AnscFreeMemory(pAcvItem);

        if (bBrData)
        {
            bSucc = 
                BspTemplateBranchStoreBranchData
                    (
                        (ANSC_HANDLE)pMyObject, 
                        &SList, 
                        hArchive, 
                        (ANSC_HANDLE)pBrData
                    );
        }
        else
        {
            /* write op code into archive */
            bSucc = pArchive->WriteUlong(hArchive, pBr->op);
            if (!bSucc)
                goto EXIT;

            /* write top op flag */
            bSucc = pArchive->WriteBoolean(hArchive, pBr->bTopOp);
            if (!bSucc)
                goto EXIT;

            bSucc = pArchive->WriteBoolean(hArchive, pBr->bObjSetProperty);
            if (!bSucc)
                goto EXIT;

            /* handle right branch */
            pBrData = &pBr->right;

            BSP_TEMPLATE_PUSH_ACV_ITEM(TRUE, pBrData, &SList);

            /* handle left branch */
            pBrData = &pBr->left;

            BSP_TEMPLATE_PUSH_ACV_ITEM(TRUE, pBrData, &SList);
        }
    }

EXIT:

    while (TRUE)
    {
        pEntry = AnscSListPopEntry(&SList);
        if (!pEntry)
            break;

        pAcvItem = ACCESS_TEMPLATE_ACV_ITEM(pEntry);
        AnscFreeMemory(pAcvItem);
    }

    return bSucc;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateBranchStoreBranchData
            (
                ANSC_HANDLE                 hThisObject,
                PSLIST_HEADER               pSListHeader,
                ANSC_HANDLE                 hArchive,
                ANSC_HANDLE                 hBranchData
            )

    description:

        This function is called to store branch data.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PSLIST_HEADER               pSListHeader
                Specifies the slist header that is used to queue
                branch objects during archiving in order to make
                the function non-recursive. Otherwise, long enough
                BSP pages may cause stack overflow.

                ANSC_HANDLE                 hArchive
                Archive object.

                ANSC_HANDLE                 hBranchData
                Branch data.

    return:     void.

**********************************************************************/

BOOL
BspTemplateBranchStoreBranchData
    (
        ANSC_HANDLE                 hThisObject,
        PSLIST_HEADER               pSListHeader,
        ANSC_HANDLE                 hArchive,
        ANSC_HANDLE                 hBranchData
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PBSP_TEMPLATE_ARCHIVE_OBJECT    pArchive  = (PBSP_TEMPLATE_ARCHIVE_OBJECT)hArchive;
    PBSP_TEMPLATE_BRANCH_DATA       pBrData   = (PBSP_TEMPLATE_BRANCH_DATA)hBranchData;
    PBSP_TEMPLATE_BRANCH_OBJECT     pBr, *pBranches;
    BOOL                            bSucc;
    PBSP_TEMPLATE_ARRAY_DATA        pArrayData;
    ULONG                           i, ulCount;

    /* left branch type */
    bSucc = pArchive->WriteByte(hArchive, (UCHAR)pBrData->type);
    if (!bSucc)
        return FALSE;

    switch (pBrData->type)
    {
    case BspBranch_eBranch:
        pBr     = (PBSP_TEMPLATE_BRANCH_OBJECT)pBrData->Value.b;
        bSucc   = pArchive->WriteBoolean(hArchive, pBr != NULL);
        if (!bSucc)
            break;

        if (pBr)
            BSP_TEMPLATE_PUSH_ACV_ITEM(FALSE, pBr, pSListHeader);

        bSucc = TRUE;

        break;

    case BspBranch_eString:
        bSucc   = pArchive->WriteString(hArchive, (PUCHAR)pBrData->Value.s);
        break;

    case BspBranch_eNumber:
    case BspBranch_eVar:
        bSucc   = pArchive->WriteUlong(hArchive, pBrData->Value.n);
        break;

    case BspBranch_eReal:
        bSucc   = pArchive->WriteDouble(hArchive, pBrData->Value.r);
        break;

    case BspBranch_eArrayDim:
    case BspBranch_eArrayItem:
    case BspBranch_eApiParams:
        pArrayData  = &pBrData->Value.a;
        ulCount     = pArrayData->Count;
        pBranches   = (PBSP_TEMPLATE_BRANCH_OBJECT *)pArrayData->pBranch;

        /* write amount of branches */
        bSucc = pArchive->WriteByte(hArchive, (UCHAR)ulCount);
        if (!bSucc)
            break;

        /* write branches */
        for (i = ulCount; i > 0; i --)
        {
            pBr = pBranches[i-1];

            bSucc = TRUE;
            if (pBr)
                BSP_TEMPLATE_PUSH_ACV_ITEM(FALSE, pBr, pSListHeader);

            if (!bSucc)
                break;
        }
        break;

    default:
        bSucc = TRUE;
        break;
    }

    return bSucc;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateBranchBranchLoad
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hArchive
            )

    description:

        This function is called to load branch from archive.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hArchive
                Archive object.

    return:     status of operation.

**********************************************************************/

BOOL
BspTemplateBranchLoad
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hArchive
    )
{
    PBSP_TEMPLATE_BRANCH_OBJECT     pMyObject = (PBSP_TEMPLATE_BRANCH_OBJECT)hThisObject;
    PBSP_TEMPLATE_ARCHIVE_OBJECT    pArchive  = (PBSP_TEMPLATE_ARCHIVE_OBJECT)hArchive;
    BOOL                            bSucc;
    ULONG                           ulValue;
    SLIST_HEADER                    SList;
    PSINGLE_LINK_ENTRY              pEntry;
    PBSP_TEMPLATE_BRANCH_OBJECT     pBr;
    PBSP_TEMPLATE_ACV_ITEM          pAcvItem  = NULL;
    BOOL                            bBrData   = FALSE;
    PBSP_TEMPLATE_BRANCH_DATA       pBrData;

    AnscSListInitializeHeader(&SList);

    BSP_TEMPLATE_PUSH_ACV_ITEM(FALSE, pMyObject, &SList);

    while (TRUE)
    {
        pEntry = AnscSListPopEntry(&SList);
        if (!pEntry)
            break;

        pAcvItem    = ACCESS_TEMPLATE_ACV_ITEM(pEntry);
        pBr         = (PBSP_TEMPLATE_BRANCH_OBJECT)pAcvItem->pData;
        pBrData     = (PBSP_TEMPLATE_BRANCH_DATA)pAcvItem->pData;
        bBrData     = pAcvItem->bBranchData;
        AnscFreeMemory(pAcvItem);

        if (bBrData)
        {
            bSucc = 
                BspTemplateBranchLoadBranchData
                    (
                        (ANSC_HANDLE)pMyObject,
                        &SList,
                        hArchive,
                        pBrData
                    );
        }
        else
        {
            /* read op code */
            bSucc   = pArchive->ReadUlong(hArchive, &ulValue);
            pBr->op = (BSP_TEMPLATE_OPERATOR)ulValue;
            if (!bSucc)
                return FALSE;

            /* read top op flag */
            bSucc   = pArchive->ReadBoolean(hArchive, &pBr->bTopOp);
            if (!bSucc)
                return FALSE;

            bSucc   = pArchive->ReadBoolean(hArchive, &pBr->bObjSetProperty);
            if (!bSucc)
                return FALSE;

            pBrData = &pBr->right;
            BSP_TEMPLATE_PUSH_ACV_ITEM(TRUE, pBrData, &SList);

            pBrData = &pBr->left;
            BSP_TEMPLATE_PUSH_ACV_ITEM(TRUE, pBrData, &SList);
        }
    }

    return TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateBranchLoadBranchData
            (
                ANSC_HANDLE                 hThisObject,
                PSLIST_HEADER               pSListHeader,
                ANSC_HANDLE                 hArchive,
                ANSC_HANDLE                 hBranchData
            )

    description:

        This function is called to set left branch.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PSLIST_HEADER               pSListHeader
                Specifies the slist header.

                ANSC_HANDLE                 hArchive
                Archive object.

                ANSC_HANDLE                 hBranchData
                Branch data object.

    return:     operation status.

**********************************************************************/

BOOL
BspTemplateBranchLoadBranchData
    (
        ANSC_HANDLE                 hThisObject,
        PSLIST_HEADER               pSListHeader,
        ANSC_HANDLE                 hArchive,
        ANSC_HANDLE                 hBranchData
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PBSP_TEMPLATE_ARCHIVE_OBJECT    pArchive  = (PBSP_TEMPLATE_ARCHIVE_OBJECT)hArchive;
    PBSP_TEMPLATE_BRANCH_DATA       pBrData   = (PBSP_TEMPLATE_BRANCH_DATA)hBranchData;
    PBSP_TEMPLATE_BRANCH_OBJECT     pBr, *pBranches;
    BOOL                            bSucc;
    ULONG                           i, ulCount;
    UCHAR                           ucType, ucCount;
    BOOL                            bExist;

    bSucc   = pArchive->ReadByte(hArchive, &ucType);
    if (!bSucc)
        return FALSE;

    pBrData->type   = (BSP_TEMPLATE_OPERATOR)ucType;

    switch (pBrData->type)
    {
    case BspBranch_eBranch:
        bSucc   = pArchive->ReadBoolean(hArchive, &bExist);
        if (!bSucc)
            break;

        if (!bExist)
            pBrData->Value.b    = (ANSC_HANDLE)NULL;
        else
        {
            pBr                 = 
                (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);

            pBrData->Value.b    = (ANSC_HANDLE)pBr;

            BSP_TEMPLATE_PUSH_ACV_ITEM(FALSE, pBr, pSListHeader);
            bSucc = TRUE;
        }
        break;

    case BspBranch_eString:
        bSucc   = pArchive->ReadString(hArchive, (PUCHAR *)&pBrData->Value.s);
        break;

    case BspBranch_eNumber:
    case BspBranch_eVar:
        bSucc   = pArchive->ReadUlong(hArchive, (PULONG)&pBrData->Value.n);
        break;

    case BspBranch_eReal:
        bSucc   = pArchive->ReadDouble(hArchive, &pBrData->Value.r);
        break;

    case BspBranch_eArrayDim:
    case BspBranch_eArrayItem:
    case BspBranch_eApiParams:

        bSucc       = pArchive->ReadByte(hArchive, &ucCount);
        if (!bSucc)
            return FALSE;

        ulCount     = (ULONG)ucCount;
        pBrData->Value.a.Count  = ulCount;

        if (ulCount != 0)
        {
            pBranches   = 
                (PBSP_TEMPLATE_BRANCH_OBJECT *)AnscAllocateMemory
                    (
                        ulCount * sizeof(BSP_TEMPLATE_BRANCH_OBJECT)
                    );

            if (!pBranches)
            {
                bSucc   = FALSE;
                break;
            }

            pBrData->Value.a.pBranch    = (void **)pBranches;

            for (i = 0; i < ulCount; i ++)
                pBranches[i]    = NULL;

            bSucc = TRUE;

            for (i = ulCount; i > 0; i --)
            {
                pBr = (PBSP_TEMPLATE_BRANCH_OBJECT)CreateBspEngBranchComponent(NULL, NULL, NULL);
                if (!pBr)
                {
                    bSucc = FALSE;
                    break;
                }
                pBranches[i-1]    = pBr;

                BSP_TEMPLATE_PUSH_ACV_ITEM(FALSE, pBr, pSListHeader);
            }
        }
        break;

    default:
        bSucc = TRUE;
        break;
    }

    return bSucc;
}

