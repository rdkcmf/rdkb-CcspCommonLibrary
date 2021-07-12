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

    module:	bree_spo_process.c

        For BSP Run-time Execution Engine (BREE) object,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the BREE object.

        *   BreeSpoSetBeepEngineDesc
        *   BreeSpoGetBeepEngineDesc
        *   BreeSpoCreateBeepEngineDesc
        *   BreeSpoSetStaticResEngineDesc
        *   BreeSpoGetStaticResEngineDesc
        *   BreeSpoCreateStaticResEngineDesc
        *   BreeSpoSetBeepSerIf
        *   BreeSpoSetBeepCspIf
        *   BreeSpoConvertPath
        *   BreeSpoSetResFilterMode
        *   BreeSpoRegResFilterDir
        *   BreeSpoRegResFilterDirExemption
        *   BreeSpoRegResFilterFile
        *   BreeSpoRegResFilterFileExemption
        *   BreeSpoGetFormalPagePath
        *   BreeSpoIsPagePathInFilterList
        *   BreeSpoPrepareDFName
        *   BreeSpoEnforceCache

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        07/22/03    initial revision.

**********************************************************************/


#include "bree_spo_global.h"
#include "safec_lib_common.h"

ANSC_STATUS
BreeSpoSetBeepEngineDesc
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PBREE_SPO_OBJECT                pMyObject   = (PBREE_SPO_OBJECT)hThisObject;

    pMyObject->hBeepEngineDesc  = hInterface;

    return ANSC_STATUS_SUCCESS;
}


ANSC_HANDLE
BreeSpoGetBeepEngineDesc
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBREE_SPO_OBJECT                pMyObject   = (PBREE_SPO_OBJECT)hThisObject;

    return pMyObject->hBeepEngineDesc;
}


ANSC_STATUS
BreeSpoCreateBeepEngineDesc
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBREE_SPO_OBJECT                pMyObject   = (PBREE_SPO_OBJECT)hThisObject;
    PBEEP_ENGINE_DESCRIPTOR         pDesc       = NULL;

    pDesc   = (PBEEP_ENGINE_DESCRIPTOR)AnscAllocateMemory(sizeof(BEEP_ENGINE_DESCRIPTOR));

    if (!pDesc)
    {
        return ANSC_STATUS_RESOURCES;
    }

    pDesc->EngineName       = BREE_SPO_ENGINE_NAME;
    pDesc->PageExtensions   = BREE_SPO_PAGE_EXTENSIONS;
#ifndef  _BMW2_BSP_PAGE_CACHING_ENABLED
    pDesc->EngineType       = 0;
#else
    pDesc->EngineType       = BEEP_ENGINE_TYPE_utilizeCache;
#endif

    pDesc->EngineType      |= BEEP_ENGINE_TYPE_resourceFilter;

    pDesc->bAggrWrites      = TRUE;

    pDesc->hEngineContext   = hThisObject;

    pDesc->PreparePage      = BreeSpoPreparePage;
    pDesc->ExecutePage      = BreeSpoExecutePage;
    pDesc->QualifyPage      = BreeSpoQualifyPage;
    pDesc->ConvertPath      = BreeSpoConvertPath;

    pMyObject->SetBeepEngineDesc(hThisObject, (ANSC_HANDLE)pDesc);

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
BreeSpoSetStaticResEngineDesc
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PBREE_SPO_OBJECT                pMyObject   = (PBREE_SPO_OBJECT)hThisObject;

    pMyObject->hStaticResEngineDesc = hInterface;

    return ANSC_STATUS_SUCCESS;
}


ANSC_HANDLE
BreeSpoGetStaticResEngineDesc
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBREE_SPO_OBJECT                pMyObject   = (PBREE_SPO_OBJECT)hThisObject;

    return pMyObject->hStaticResEngineDesc;
}


ANSC_STATUS
BreeSpoCreateStaticResEngineDesc
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBREE_SPO_OBJECT                pMyObject   = (PBREE_SPO_OBJECT)hThisObject;
    PBEEP_ENGINE_DESCRIPTOR         pDesc       = NULL;

    pDesc   = (PBEEP_ENGINE_DESCRIPTOR)AnscAllocateMemory(sizeof(BEEP_ENGINE_DESCRIPTOR));

    if (!pDesc)
    {
        return ANSC_STATUS_RESOURCES;
    }

    pDesc->EngineName       = BREE_SPO_STATIC_ENGINE_NAME;
    pDesc->PageExtensions   = BREE_SPO_STATIC_RES_EXTENSIONS;

#ifdef   _DEBUG
    pDesc->EngineType       = BEEP_ENGINE_TYPE_wildcard;
#else
    pDesc->EngineType       = BEEP_ENGINE_TYPE_utilizeCache | BEEP_ENGINE_TYPE_wildcard;
#endif

    pDesc->EngineType      |= BEEP_ENGINE_TYPE_resourceFilter;

    pDesc->bAggrWrites      = FALSE;

    pDesc->hEngineContext   = hThisObject;

    pDesc->PreparePage      = BreeSpoPrepareStaticRes;
    pDesc->ExecutePage      = BreeSpoExecuteStaticRes;
    pDesc->QualifyPage      = BreeSpoQualifyPage;
    pDesc->ConvertPath      = BreeSpoConvertPath;

    pMyObject->SetStaticResEngineDesc(hThisObject, (ANSC_HANDLE)pDesc);

    return ANSC_STATUS_SUCCESS;
}


BOOL
BreeSpoQualifyPage
    (
        ANSC_HANDLE                 hThisObject,
        char*                       page_path,
        ULONG                       page_type
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    char                            *pFileName  = page_path;
    char                            *pExt;
    BOOL                            bQualified  = TRUE;

    pFileName   += AnscSizeOfString(pFileName);

    while (pFileName >= page_path)
    {
        if (*pFileName == '/' || *pFileName == '\\')
        {
            pFileName ++;

            break;
        }

        pFileName --;
    }

    if (pFileName < page_path)
    {
        pFileName   = page_path;
    }

    pExt    = _ansc_strchr(pFileName, '.');
    if (pExt)
    {
        pExt ++;
    }

    switch (page_type)
    {
        case    BEEP_PAGE_TYPE_serverPage:

            if (!pExt)
            {
                bQualified  = FALSE;
            }
            else
            {
                bQualified  = AnscEqualString2(pExt, BREE_SPO_PAGE_EXTENSIONS, AnscSizeOfString(BREE_SPO_PAGE_EXTENSIONS), FALSE);
            }

            break;

        case    BEEP_PAGE_TYPE_staticFile:
        default:

            if (!pExt)
            {
                bQualified  = TRUE;
            }
            else
            {
                bQualified  = !AnscEqualString2(pExt, BREE_SPO_PAGE_EXTENSIONS, AnscSizeOfString(BREE_SPO_PAGE_EXTENSIONS), FALSE);
            }

            break;

    }

    return bQualified;
}


ANSC_STATUS
BreeSpoSetBeepSerIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PBREE_SPO_OBJECT                pMyObject   = (PBREE_SPO_OBJECT)hThisObject;

    pMyObject->hBeepSerIf   = hInterface;

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
BreeSpoSetBeepCspIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PBREE_SPO_OBJECT                pMyObject   = (PBREE_SPO_OBJECT)hThisObject;

    pMyObject->hBeepCspIf   = hInterface;

    return ANSC_STATUS_SUCCESS;
}


BOOL
BreeSpoConvertPath

    (
        ANSC_HANDLE                 hThisObject,
        char*                       org_page_path,
        char**                      new_page_path
    )
{
    PBREE_SPO_OBJECT                pMyObject       = (PBREE_SPO_OBJECT)hThisObject;
    ULONG                           FilterMode      = pMyObject->ResFilterMode;
    char                            *pFName;
    BOOL                            bPathChanged    = FALSE;

    pFName  = (char *)pMyObject->GetFormalPagePath((ANSC_HANDLE)pMyObject, (PUCHAR)org_page_path);

    if (!pFName)
    {
        *new_page_path  = AnscCloneString(BREE_SPO_BAD_PAGE_PATH_NAME);
        bPathChanged    = TRUE;

        goto DONE;
    }

    if (FilterMode == BREE_SPO_RES_FILTER_Off)
    {
        bPathChanged    = FALSE;

        goto DONE;
    }

    /* check the page path against exemption list first */
    if (pMyObject->IsPagePathInFilterList((ANSC_HANDLE)pMyObject, (PUCHAR)pFName, TRUE))
    {
        if (FilterMode == BREE_SPO_RES_FILTER_ProtectExcept)
        {
            *new_page_path  = AnscCloneString((char *)pMyObject->SupervisorPage);
            bPathChanged    = TRUE;

            goto DONE;
        }

        if (FilterMode == BREE_SPO_RES_FILTER_ProtectOnly)
        {
            bPathChanged    = FALSE;

            goto DONE;
        }
    }

    /* check the page path against filter list then */
    if (pMyObject->IsPagePathInFilterList((ANSC_HANDLE)pMyObject, (PUCHAR)pFName, FALSE))
    {
        if (FilterMode == BREE_SPO_RES_FILTER_ProtectOnly)
        {
            *new_page_path  = AnscCloneString((char *)pMyObject->SupervisorPage);
            bPathChanged    = TRUE;

            goto DONE;
        }

        if (FilterMode == BREE_SPO_RES_FILTER_ProtectExcept)
        {
            bPathChanged    = FALSE;

            goto DONE;
        }
    }

    if (FilterMode == BREE_SPO_RES_FILTER_ProtectOnly)
    {
        bPathChanged    = FALSE;

        goto DONE;
    }
    else if (FilterMode == BREE_SPO_RES_FILTER_ProtectExcept)
    {
        *new_page_path  = AnscCloneString((char *)pMyObject->SupervisorPage);
        bPathChanged    = TRUE;

        goto DONE;
    }
    else
    {
        bPathChanged    = FALSE;

        goto DONE;
    }

DONE:
    if (pFName)
    {
        AnscFreeMemory(pFName);
    }

    return bPathChanged;
}


ANSC_STATUS
BreeSpoConfigResFilter
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ResFilterMode,
        PUCHAR                      pSupervisorPage
    )
{
    PBREE_SPO_OBJECT                pMyObject   = (PBREE_SPO_OBJECT)hThisObject;
    ANSC_STATUS                     status      = ANSC_STATUS_FAILURE;

    switch (ResFilterMode)
    {
    case    BREE_SPO_RES_FILTER_Off:
    case    BREE_SPO_RES_FILTER_ProtectExcept:
    case    BREE_SPO_RES_FILTER_ProtectOnly:

        pMyObject->ResFilterMode    = ResFilterMode;

        if (pMyObject->SupervisorPage)
        {
            AnscFreeMemory(pMyObject->SupervisorPage);
        }

        if (!pSupervisorPage)
        {
            pMyObject->SupervisorPage   = NULL;
        }
        else
        {
            pMyObject->SupervisorPage   = (PUCHAR)AnscCloneString((char *)pSupervisorPage);
        }

        status                      = ANSC_STATUS_SUCCESS;

        break;
    }

    return status;
}


ANSC_STATUS
BreeSpoRegResFilterDir
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pName
    )
{
    PBREE_SPO_OBJECT                pMyObject   = (PBREE_SPO_OBJECT)hThisObject;
    PBREE_SPO_RES_FILTER            pFilter     = NULL;

    if (!pName)
    {
        return ANSC_STATUS_BAD_PARAMETER;
    }

    pFilter = (PBREE_SPO_RES_FILTER)AnscAllocateMemory(sizeof(BREE_SPO_RES_FILTER));

    if (!pFilter)
    {
        return ANSC_STATUS_RESOURCES;
    }

    pFilter->bDirectory         = TRUE;
    pFilter->pDFName            = pMyObject->PrepareDFName((ANSC_HANDLE)pMyObject, pName);

    AnscSListPushEntry(&pMyObject->ResFilterList, &pFilter->Linkage);

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
BreeSpoRegResFilterDirExemption
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pName
    )
{
    PBREE_SPO_OBJECT                pMyObject   = (PBREE_SPO_OBJECT)hThisObject;
    PBREE_SPO_RES_FILTER            pFilter     = NULL;

    if (!pName)
    {
        return ANSC_STATUS_BAD_PARAMETER;
    }

    pFilter = (PBREE_SPO_RES_FILTER)AnscAllocateMemory(sizeof(BREE_SPO_RES_FILTER));

    if (!pFilter)
    {
        return ANSC_STATUS_RESOURCES;
    }

    pFilter->bDirectory         = TRUE;
    pFilter->pDFName            = pMyObject->PrepareDFName((ANSC_HANDLE)pMyObject, pName);

    AnscSListPushEntry(&pMyObject->ResFilterListExemption, &pFilter->Linkage);

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
BreeSpoRegResFilterFile
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pName
    )
{
    PBREE_SPO_OBJECT                pMyObject   = (PBREE_SPO_OBJECT)hThisObject;
    PBREE_SPO_RES_FILTER            pFilter     = NULL;

    if (!pName)
    {
        return ANSC_STATUS_BAD_PARAMETER;
    }

    pFilter = (PBREE_SPO_RES_FILTER)AnscAllocateMemory(sizeof(BREE_SPO_RES_FILTER));

    if (!pFilter)
    {
        return ANSC_STATUS_RESOURCES;
    }

    pFilter->bDirectory         = FALSE;
    pFilter->pDFName            = pMyObject->PrepareDFName((ANSC_HANDLE)pMyObject, pName);

    AnscSListPushEntryAtBack(&pMyObject->ResFilterList, &pFilter->Linkage);

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
BreeSpoRegResFilterFileExemption
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pName
    )
{
    PBREE_SPO_OBJECT                pMyObject   = (PBREE_SPO_OBJECT)hThisObject;
    PBREE_SPO_RES_FILTER            pFilter     = NULL;

    if (!pName)
    {
        return ANSC_STATUS_BAD_PARAMETER;
    }

    pFilter = (PBREE_SPO_RES_FILTER)AnscAllocateMemory(sizeof(BREE_SPO_RES_FILTER));

    if (!pFilter)
    {
        return ANSC_STATUS_RESOURCES;
    }

    pFilter->bDirectory         = FALSE;
    pFilter->pDFName            = pMyObject->PrepareDFName((ANSC_HANDLE)pMyObject, pName);

    if (pFilter->pDFName)
    {
        AnscStrLower(pFilter->pDFName);
    }

    AnscSListPushEntryAtBack(&pMyObject->ResFilterListExemption, &pFilter->Linkage);

    return ANSC_STATUS_SUCCESS;
}


PUCHAR
BreeSpoGetFormalPagePath
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pPagePath
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PUCHAR                          pFName      = NULL;
    ULONG                           ulPathLen   = pPagePath ? AnscSizeOfString((const char *)pPagePath) : 0;
    ULONG                           i;
    USHORT                          pathSegPos[BREE_SPO_MAX_PAGE_PATH_SEG];
    USHORT                          pathSegLen[BREE_SPO_MAX_PAGE_PATH_SEG];
    ULONG                           pathSegNum  = 0;
    PUCHAR                          pNextSlash;
    PUCHAR                          pCurPath;
    PUCHAR                          pLast;
    ULONG                           ulCurPos;

    if (ulPathLen == 0)
    {
        return NULL;
    }

    pLast       = pPagePath + ulPathLen - 1;
    while (*pPagePath == '/' && pPagePath <= pLast)
    {
        pPagePath ++;
    }

    if (pPagePath > pLast)
    {
        return NULL;
    }

    pFName  = (PUCHAR)AnscAllocateMemory(ulPathLen + 1);

    if (!pFName)
    {
        return NULL;
    }

    pLast       = pPagePath + AnscSizeOfString((const char *)pPagePath) - 1;
    pCurPath    = pPagePath;
    
    while (pCurPath && pCurPath <= pLast)
    {
        pNextSlash  = (PUCHAR)_ansc_strchr((const char *)pCurPath, '/');

        if (!pNextSlash)
        {
            pathSegPos[pathSegNum]  = pCurPath - pPagePath;
            pathSegLen[pathSegNum]  = pLast - pCurPath + 1;
            pathSegNum ++;

            break;
        }
        else
        {
            ULONG                   segLen  = pNextSlash - pCurPath;

            if (segLen == 1 && *pCurPath == '.')
            {
                /* it's "./", do nothing for this segment */
            }
            else if (segLen == 2 && *pCurPath == '.' && *(pCurPath + 1) == '.')
            {
                if (pathSegNum == 0)
                {
                    /* trying access resource out of web root sub-tree */
                    AnscFreeMemory(pFName);

                    return NULL;
                }
                else
                {
                    if (pathSegNum == 1 && pathSegLen[0] == 0)
                    {
                        AnscFreeMemory(pFName);

                        return NULL;
                    }
                    
                    pathSegNum --;
                }
            }
            else
            {
                pathSegPos[pathSegNum]  = pCurPath - pPagePath;
                pathSegLen[pathSegNum]  = pNextSlash - pCurPath;
                pathSegNum ++;
            }

            /* check next segment */
            pCurPath    = pNextSlash + 1;
        }
    }

    ulCurPos    = 0;

    /* construct the formal path */
    for (i = 0; i < pathSegNum; i ++)
    {
        AnscCopyMemory
            (
                pFName + ulCurPos, 
                pPagePath + pathSegPos[i],
                pathSegLen[i]
            );

        ulCurPos += pathSegLen[i];

        if (i < pathSegNum - 1)
        {
            pFName[ulCurPos ++] = '/';
        }
    }

    pFName[ulCurPos]    = 0;

    return pFName;
}


BOOL
BreeSpoIsPagePathInFilterList
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pPagePath,
        BOOL                        bExemptionList
    )
{
    PBREE_SPO_OBJECT                pMyObject   = (PBREE_SPO_OBJECT)hThisObject;
    PSINGLE_LINK_ENTRY              pSListEntry;
    PBREE_SPO_RES_FILTER            pFilter;
    PSLIST_HEADER                   pList;
    PUCHAR                          pPath;
    BOOL                            bInList     = FALSE;
    PUCHAR                          pSeg;
    PUCHAR                          pPageSeg;
    ULONG                           ulPageSegLen;
    PUCHAR                          pNextSlash;
    PUCHAR                          pPathLast;

    pPath   = (PUCHAR)AnscCloneString((char *)pPagePath);
    if (!pPath)
    {
        return FALSE;
    }

    pPath       = AnscStrLower(pPath);
    pPathLast   = pPath + AnscSizeOfString((const char *)pPath) - 1;

    pList   = bExemptionList ? &pMyObject->ResFilterListExemption : &pMyObject->ResFilterList;

    pSListEntry = AnscSListGetFirstEntry(pList);

    while (pSListEntry)
    {
        pFilter     = ACCESS_BREE_SPO_RES_FILTER(pSListEntry);
        pSListEntry = AnscSListGetNextEntry(pSListEntry);

        pSeg        = pFilter->pDFName;
        pPageSeg    = pPath;

        while (TRUE)
        {
            if (!pPageSeg && pSeg)
            {
                break;
            }

            if (!pSeg)
            {
                if (pFilter->bDirectory || !pPageSeg)
                {
                    bInList = TRUE;
                    goto FINISHED;
                }
                else
                {
                    /* try next filter */
                    break;
                }
            }
            else
            {
                pNextSlash  = (PUCHAR)_ansc_strchr((const char *)pPageSeg, '/');

                if (!pNextSlash)
                {
                    ulPageSegLen    = pPathLast - pPageSeg + 1;
                }
                else
                {
                    ulPageSegLen    = pNextSlash - pPageSeg;
                }

                if (AnscSizeOfString((const char *)pSeg) != ulPageSegLen)
                {
                    /* try next filter */
                    break;
                }

                if (!AnscEqualMemory(pSeg, pPageSeg, ulPageSegLen))
                {
                    /* try next filter */
                    break;
                }
            }

            /* get next segment of page path */
            pNextSlash  = (PUCHAR)_ansc_strchr((const char *)pPageSeg, '/');
            if (!pNextSlash)
            {
                pPageSeg    = NULL;
            }
            else
            {
                pPageSeg    = pNextSlash + 1;

                if (pPageSeg > pPathLast)
                {
                    pPageSeg    = NULL;
                }
            }

            /* get next segment of filter */
            pSeg    = pSeg + AnscSizeOfString((const char *)pSeg) + 1;
            if (*pSeg == 0)
            {
                pSeg    = NULL;
            }
        }
    }

FINISHED:
    AnscFreeMemory(pPath);

    return bInList;
}


PUCHAR
BreeSpoPrepareDFName
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pPagePath
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PUCHAR                          pDFName = NULL;
    ULONG                           ulLen, i;
    errno_t    rc  = -1;

    /* this function assumes there's no double slashes "//" in the page path */

    ulLen   = AnscSizeOfString((const char *)pPagePath);
    pDFName = (PUCHAR)AnscAllocateMemory(ulLen + 2);

    if (pDFName)
    {
        if (ulLen > 1 && *pPagePath == '/')
        {
            pPagePath ++;
            ulLen --;
        }

        if (ulLen == 0)
        {
            pDFName[0]  = 0;
            pDFName[1]  = 0;
        }
        else
        {
            rc = strcpy_s((char*)pDFName, (ulLen + 2), (char*)pPagePath);
            ERR_CHK(rc);
        }

        AnscStrLower(pDFName);

        /* replace slash with NULL character */
        for (i = 0; i < ulLen; i ++)
        {
            if (*(pDFName + i) == '/')
            {
                *(pDFName + i) = 0;
            }
        }

        pDFName[ulLen]  = 0;
        pDFName[ulLen + 1]  = 0;
    }

    return pDFName;
}


ANSC_STATUS
BreeSpoEnforceCache
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBeepPecIf,
        ULONG                       ulSeconds
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PBEEP_PEC_INTERFACE             pBeepPecIf      = (PBEEP_PEC_INTERFACE)hBeepPecIf;
    ANSC_HANDLE                     hSlapBeepResp   = NULL;
    SLAP_PARAMETER_LIST             paramList;
    SLAP_PARAMETER_LIST             *outList        = NULL;
    PSLAP_VARIABLE                  pParam;
    PSLAP_VARIABLE                  pRetVal = NULL;
    ULONG                           ulPecStatus;
    char                            value[48];
    errno_t    rc  = -1;

    hSlapBeepResp  = pBeepPecIf->GetResponse(pBeepPecIf->hOwnerContext);

    if (!hSlapBeepResp)
    {
        return ANSC_STATUS_INTERNAL_ERROR;
    }

    if (TRUE)
    {
        rc = sprintf_s(value, sizeof(value), "max-age=%d", (int)ulSeconds);
        if(rc < EOK)
        {
            ERR_CHK(rc);
            return ANSC_STATUS_INTERNAL_ERROR;
        }

        paramList.ParamCount    = 2;

        pParam  = &paramList.ParamArray[0];
        pParam->Syntax  = SLAP_VAR_SYNTAX_TYPE_string;
        pParam->Variant.varString   = (SLAP_CHAR *)HTTP_HEADER_NAME_CACHE_CONTROL;

        pParam  = &paramList.ParamArray[1];
        pParam->Syntax  = SLAP_VAR_SYNTAX_TYPE_string;
        pParam->Variant.varString   = (SLAP_CHAR *)value;

        /* invoke "AddHeader" method of Response object to output the given string */
        pBeepPecIf->InvokeObject
            (
                pBeepPecIf->hOwnerContext,
                hSlapBeepResp,
                (char *)"AddHeader",
                &paramList,
                &outList,
                &pRetVal,
                &ulPecStatus
            );

        if (outList)
        {
            SlapFreeParamList(outList);
            outList = NULL;
        }

        if (pRetVal)
        {
            SlapFreeVariable(pRetVal);
            pRetVal = NULL;
        }
    }

    if (TRUE)
    {
        ANSC_UNIVERSAL_TIME         timeNow;
        char                        *pWDay = NULL, *pMonth = NULL;
        ULONG                       ulExpTime;

        AnscGetSystemTime(&timeNow);

        ulExpTime   = AnscCalendarToSecond((ANSC_HANDLE)&timeNow);
        ulExpTime  += ulSeconds;
        
        AnscSecondToCalendar(ulExpTime, (ANSC_HANDLE)&timeNow);

        switch (timeNow.DayOfWeek)
        {
            default:

                    pWDay   = "Sun";

                    break;

            case    1:

                    pWDay   = "Mon";

                    break;

            case    2:

                    pWDay   = "Tue";

                    break;

            case    3:

                    pWDay   = "Wed";

                    break;

            case    4:

                    pWDay   = "Thu";

                    break;

            case    5:

                    pWDay   = "Fri";

                    break;

            case    6:

                    pWDay   = "Sat";

                    break;
        }

        switch (timeNow.Month)
        {
            case    1:

                    pMonth  = "Jan";

                    break;

            case    2:

                    pMonth  = "Feb";

                    break;

            case    3:

                    pMonth  = "Mar";

                    break;

            case    4:

                    pMonth  = "Apr";

                    break;

            case    5:

                    pMonth  = "May";

                    break;

            case    6:

                    pMonth  = "Jun";

                    break;

            case    7:

                    pMonth  = "Jul";

                    break;

            case    8:

                    pMonth  = "Aug";

                    break;

            case    9:

                    pMonth  = "Sep";

                    break;

            case    10:

                    pMonth  = "Oct";

                    break;

            case    11:

                    pMonth  = "Nov";

                    break;

            case    12:
            default:

                    pMonth  = "Dec";

                    break;

        }

        rc = sprintf_s
            (
                value, sizeof(value),
                "%s, %.2d %s %d %.2d:%.2d:%.2d GMT", 
                pWDay, 
                timeNow.DayOfMonth,
                pMonth,
                timeNow.Year,
                timeNow.Hour,
                timeNow.Minute,
                timeNow.Second
            );
        if(rc < EOK)
        {
            ERR_CHK(rc);
            return ANSC_STATUS_INTERNAL_ERROR;
        }

        paramList.ParamCount    = 2;

        pParam  = &paramList.ParamArray[0];
        pParam->Syntax  = SLAP_VAR_SYNTAX_TYPE_string;
        pParam->Variant.varString   = (SLAP_CHAR *)HTTP_HEADER_NAME_EXPIRES;

        pParam  = &paramList.ParamArray[1];
        pParam->Syntax  = SLAP_VAR_SYNTAX_TYPE_string;
        pParam->Variant.varString   = (SLAP_CHAR *)value;

        /* invoke "AddHeader" method of Response object to output the given string */
        pBeepPecIf->InvokeObject
            (
                pBeepPecIf->hOwnerContext,
                hSlapBeepResp,
                (char *)"AddHeader",
                &paramList,
                &outList,
                &pRetVal,
                &ulPecStatus
            );

        if (outList)
        {
            SlapFreeParamList(outList);
            outList = NULL;
        }

        if (pRetVal)
        {
            SlapFreeVariable(pRetVal);
            pRetVal = NULL;
        }
    }

    return ANSC_STATUS_SUCCESS;
}


ANSC_HANDLE
BreeSpoCreateBreeSoaIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBeepPecIf
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PBSPENG_SOA_INTERFACE           pBreeSoaIf;
    errno_t   rc = -1;

    pBreeSoaIf = (PBSPENG_SOA_INTERFACE)AnscAllocateMemory(sizeof(BSPENG_SOA_INTERFACE));

    if ( !pBreeSoaIf )
    {
        return  NULL;
    }

    rc = STRCPY_S_NOCLOBBER(pBreeSoaIf->Name, sizeof(pBreeSoaIf->Name), BREE_SOA_INTERFACE_NAME);
    ERR_CHK(rc);

    pBreeSoaIf->InterfaceId         = BREE_SOA_INTERFACE_ID;
    pBreeSoaIf->hOwnerContext       = (ANSC_HANDLE)hBeepPecIf;
    pBreeSoaIf->Size                = sizeof(BSPENG_SOA_INTERFACE);

    pBreeSoaIf->GetSlapObject       = BreeSpoSoaGetSlapObject;
    pBreeSoaIf->InvokeObject        = BreeSpoSoaInvokeObject;
    pBreeSoaIf->IsBuiltInObj        = BreeSpoSoaIsBuiltInObject;
    pBreeSoaIf->WriteAString        = BreeSpoSoaWriteAString;
    pBreeSoaIf->WriteBString        = BreeSpoSoaWriteBString;
    pBreeSoaIf->GetCookedPage       = BreeSpoSoaGetCookedPage;
    pBreeSoaIf->RetCookedPage       = BreeSpoSoaRetCookedPage;
    pBreeSoaIf->GetCookedPageData   = BreeSpoSoaGetCookedPageData;
    pBreeSoaIf->IsInterrupted       = BreeSpoSoaIsInterrupted;

    return (ANSC_HANDLE)pBreeSoaIf;
}



