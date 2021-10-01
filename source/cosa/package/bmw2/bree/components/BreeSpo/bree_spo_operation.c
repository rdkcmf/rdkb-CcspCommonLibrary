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

    module:	bree_spo_operation.c

        For BSP Run-time Execution Engine (BREE) object,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the BREE object.

        *   BreeSpoPreparePage
        *   BreeSpoExecutePage
        *   BreeSpoPrepareStaticRes
        *   BreeSpoExecuteStaticRes
        *   BreeSpoEngage
        *   BreeSpoCancel

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


/**********************************************************************

    caller:     Broadway Web Resource Manager (BWRM)

    prototype:

        ANSC_HANDLE
        BreeSpoPreparePage
            (
                ANSC_HANDLE                 hThisObject,
                char*                       page_path,
                void*                       page_data,
                ULONG                       page_size
            )

    description:

        This function is called to "pre-compile" a BSP page, so next
        time we don't need to compile it again if it will be cached.

    argument:   ANSC_HANDLE                 hThisObject
                This context handle is transparent to the BREE SPO
                object.

                char*                       page_path
                Absolute path of this BSP file.

                void*                       page_data
                Raw data of the given BSP file.

                ULONG                       page_size
                Raw data size

    return:     handle of the "cooked" page.

**********************************************************************/

ANSC_HANDLE
BreeSpoPreparePage
    (
        ANSC_HANDLE                 hThisObject,
        char*                       page_path,
        void*                       page_data,
        ULONG                       page_size
    )
{
    /*
     * To provide flexibility and extendibility, system will only store
     * raw data of BSP pages and all resources. So this function is called
     * when a new page is going to be loaded.
     */
#if defined(_BREE_SPO_USE_SRMO) || defined(_USE_PRECOMPILED_BSP_PAGES)

    /* all resources have been well cooked */

    UNREFERENCED_PARAMETER(hThisObject);
    PBREE_COOKED_BSP_PAGE           pCookedPage = NULL;

    pCookedPage = (PBREE_COOKED_BSP_PAGE)BreeCreateCookedPageObj(NULL, NULL, NULL);

    if (pCookedPage)
    {
        pCookedPage->pArchivedBsp   = page_data;
        pCookedPage->ulSize         = page_size;
        pCookedPage->bDynamic       = TRUE;
    }
#else
    /*
     * the BSP page needs to be cooked
     */

    PBREE_SPO_OBJECT                pMyObject   = (PBREE_SPO_OBJECT)hThisObject;
    PBREE_COOKED_BSP_PAGE           pCookedPage = NULL;
    char                            *pTemplate  = page_path;
    PBREESPO_READER_OBJECT          pReader     = NULL;
    PBSP_TEMPLATE_ARCHIVE_OBJECT    pArchive    = NULL;
    BOOL                            bSucc;
    PBSP_TEMPLATE_LIST_OBJECT       pList       = NULL;
    PUCHAR                          pArcData    = NULL;
    ULONG                           ulArcDataSize;
    BOOL                            bNeedRelease= FALSE;
    ULONG                           ulLen       = 0;

    ulLen = AnscSizeOfString(page_path);

    if ( ulLen >= 4 && page_path[ulLen - 4] != '.' )    /* either ".bsp" or ".cbsp" */
    {
        /* The given page has been pre-compiled */
        pArcData        = page_data;
        ulArcDataSize   = page_size;
        bNeedRelease    = TRUE;

        page_data       = NULL;
        goto TERMINATED;
    }

#ifdef   _DEBUG
    AnscTrace("Start cooking BSP page <%s>, time = %d (ms) ...\n", page_path, AnscGetTickInMilliSeconds());
#endif

    pArchive    = (PBSP_TEMPLATE_ARCHIVE_OBJECT)CreateBspEngArchiveComponent(NULL, NULL, NULL);
    pList       = (PBSP_TEMPLATE_LIST_OBJECT   )CreateBspEngListComponent   (NULL, NULL, NULL);
    pReader     = (PBREESPO_READER_OBJECT      )CreateBreeSpoReaderComponent(NULL, NULL, NULL);

    if (!pArchive || !pList || !pReader)
    {
        goto TERMINATED;
    }

    pList->SetBspSoaIf((ANSC_HANDLE)pList, pMyObject->hBreeSoaIf);

    pReader->OpenInternal((ANSC_HANDLE)pReader, pTemplate, (char *)page_data, page_size);

    if (pReader->Valid((ANSC_HANDLE)pReader))
    {
#ifdef   _DEBUG
        AnscTrace("Parsing the BSP page <%s>, time = %d (ms) ...\n", page_path, AnscGetTickInMilliSeconds());
#endif

        pList->AddGroup((ANSC_HANDLE)pList, (ANSC_HANDLE)pReader, TRUE, pTemplate);

#ifdef   _DEBUG
        AnscTrace("Archiving BSP page <%s>, time = %d (ms) ...\n", page_path, AnscGetTickInMilliSeconds());
#endif

        bSucc = pList->Store((ANSC_HANDLE)pList, (ANSC_HANDLE)pArchive);
        if (bSucc)
        {
            pArcData    = pArchive->ExtractContent((ANSC_HANDLE)pArchive, &ulArcDataSize, &bNeedRelease);
        }
    }

TERMINATED:
    if (pList)
    {
        pList->Remove((ANSC_HANDLE)pList);
    }

    if (pReader)
    {
        pReader->Remove((ANSC_HANDLE)pReader);
    }

    if (pArchive)
    {
        pArchive->Remove((ANSC_HANDLE)pArchive);
    }

    if (pArcData && ulArcDataSize != 0)
    {
        pCookedPage = (PBREE_COOKED_BSP_PAGE)BreeCreateCookedPageObj(NULL, NULL, NULL);

        if (pCookedPage)
        {
            pCookedPage->pArchivedBsp   = pArcData;
            pCookedPage->ulSize         = ulArcDataSize;
            pCookedPage->bDynamic       = bNeedRelease;
        }
    }

    /* free raw data buffer */
    if ( page_data )
    {
        AnscFreeMemory(page_data);
    }
#endif

#ifdef   _DEBUG
    AnscTrace("BSP page %s has been cooked, time = %lu (ms) ...\n", page_path, AnscGetTickInMilliSeconds());
#endif

    return (ANSC_HANDLE)pCookedPage;
}


/**********************************************************************

    caller:     Broadway Web Resource Manager (BWRM)

    prototype:

        ANSC_STATUS
        BreeSpoExecutePage
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBwrmCookedPage,
                ANSC_HANDLE                 hBeepPecIf
            );

    description:

        This function is called to interpret a "cooked" BSP page.

    argument:   ANSC_HANDLE                 hThisObject
                This context handle is transparent to the BREE SPO
                object.

                ANSC_HANDLE                 hBwrmCookedPage
                The handle to a "cooked" BSP page which returns
                by "PreparePage".

                ANSC_HANDLE                 hBeepPecIf
                The handle to BEEP PEC interface which will be
                used to access underlying objects.

    return:     status of the operation.

**********************************************************************/

ANSC_STATUS
BreeSpoExecutePage
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBwrmCookedPage,
        ANSC_HANDLE                 hBeepPecIf
    )
{
    PBREE_SPO_OBJECT                pMyObject       = (PBREE_SPO_OBJECT)hThisObject;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;
    PBREE_COOKED_BSP_PAGE           pCookedPage     = (PBREE_COOKED_BSP_PAGE)hBwrmCookedPage;
    char                            *pTemplateName  = NULL;
    PBREESPO_READER_OBJECT          pReader         = NULL;
    BOOL                            bReaderOK       = FALSE;
    PBSP_TEMPLATE_LIST_OBJECT       pList           = NULL;
    PBSP_TEMPLATE_ARCHIVE_OBJECT    pArchive        = NULL;
    ANSC_HANDLE                     hBreeSoaIf      = NULL;

    pReader = (PBREESPO_READER_OBJECT)CreateBreeSpoReaderComponent(NULL, NULL, NULL);

    if (!pReader)
    {
        goto OUT_OF_RESOURCES;
    }

    hBreeSoaIf      = 
        pMyObject->CreateBreeSoaIf
            (
                (ANSC_HANDLE)pMyObject,
                hBeepPecIf
            );

    if ( !hBreeSoaIf )
    {
        goto OUT_OF_RESOURCES;
    }

    pTemplateName   = pCookedPage->PagePath;

/*
#ifdef   _DEBUG
    printf("[BreeSpoExecutePage]: executing BSP page <%s> ...\n", pTemplateName);
    fflush(stdout);
#endif
*/

    status          = 
        pReader->OpenInternal
            (
                (ANSC_HANDLE)pReader, 
                pTemplateName, 
                pCookedPage->pArchivedBsp, 
                pCookedPage->ulSize
            );

    bReaderOK       = status == ANSC_STATUS_SUCCESS;

    if (bReaderOK)
    {
        PBSP_TEMPLATE_OBJECT        pTmpl;

        pList = (PBSP_TEMPLATE_LIST_OBJECT)CreateBspEngListComponent(NULL, NULL, NULL);

        if (!pList)
        {
            goto OUT_OF_RESOURCES;
        }

        /* set up BEEP PEC interface */
        pList->SetBspSoaIf((ANSC_HANDLE)pList, hBreeSoaIf);
        pList->SetTemplateSource((ANSC_HANDLE)pList, FALSE);
        pList->UseArchive((ANSC_HANDLE)pList, TRUE);

        pArchive = (PBSP_TEMPLATE_ARCHIVE_OBJECT)CreateBspEngArchiveComponent(NULL, NULL, NULL);

        if (!pArchive)
        {
            goto OUT_OF_RESOURCES;
        }

        pArchive->SetStorage
            (
                (ANSC_HANDLE)pArchive, 
                pCookedPage->pArchivedBsp, 
                pCookedPage->ulSize, 
                FALSE
            );

        /* load "compiled" template into list object */
        pList->Load((ANSC_HANDLE)pList, (ANSC_HANDLE)pArchive);
        pArchive->Remove((ANSC_HANDLE)pArchive);

        pTmpl = pList->FindItem((ANSC_HANDLE)pList, pCookedPage->PagePath);

        if (!pTmpl)
        {
            AnscTrace(" can't find BSP page %s.\n", pCookedPage->PagePath);
        }
        else
        {
#ifdef   _DEBUG
            AnscTrace("Start running BSP page <%s>, time = %lu (ms) ...\n", pTmpl->pName, AnscGetTickInMilliSeconds());
#endif

            pTmpl->Run((ANSC_HANDLE)pTmpl, hThisObject, (ANSC_HANDLE)NULL, (ANSC_HANDLE)NULL, NULL);

#ifdef   _DEBUG
            AnscTrace("BSP page <%s> has been executed, time = %lu (ms).\n", pTmpl->pName, AnscGetTickInMilliSeconds());
#endif
        }

        pList->Remove((ANSC_HANDLE)pList);
    }

    pReader->Close((ANSC_HANDLE)pReader);
    pReader->Remove((ANSC_HANDLE)pReader);

    goto NO_PROBLEM;

OUT_OF_RESOURCES:

    if (pReader)
    {
        pReader->Remove((ANSC_HANDLE)pReader);
    }

    if (pList)
    {
        pList->Remove((ANSC_HANDLE)pList);
    }

    if (pArchive)
    {
        pArchive->Remove((ANSC_HANDLE)pArchive);
    }

NO_PROBLEM:

    if ( hBreeSoaIf )
    {
        AnscFreeMemory(hBreeSoaIf);
    }

/*
#ifdef   _DEBUG
    printf("[BreeSpoExecutePage]: done with BSP page <%s>.\n", pTemplateName);
    fflush(stdout);
#endif
*/

    return status;
}


/**********************************************************************

    caller:     Broadway Web Resource Manager (BWRM)

    prototype:

        ANSC_HANDLE
        BreeSpoPrepareStaticRes
            (
                ANSC_HANDLE                 hThisObject,
                char*                       page_path,
                void*                       page_data,
                ULONG                       page_size
            )

    description:

        This function is called to "pre-compile" a BSP page, so next
        time we don't need to compile it again if it will be cached.

    argument:   ANSC_HANDLE                 hThisObject
                This context handle is transparent to the BREE SPO
                object.

                char*                       page_path
                Absolute path of this BSP file.

                void*                       page_data
                Raw data of the given BSP file.

                ULONG                       page_size
                Raw data size

    return:     handle of the "cooked" page.

**********************************************************************/

ANSC_HANDLE
BreeSpoPrepareStaticRes
    (
        ANSC_HANDLE                 hThisObject,
        char*                       page_path,
        void*                       page_data,
        ULONG                       page_size
    )
{
    UNREFERENCED_PARAMETER(page_path);
    UNREFERENCED_PARAMETER(hThisObject);
    /*
     * To provide flexibility and extendibility, system will only store
     * raw data of BSP pages and all resources. So this function is called
     * when a new page is going to be loaded.
     */

    PBREE_COOKED_BSP_PAGE           pCookedPage = NULL;

/*
#ifdef   _DEBUG
    printf("[BreeSpoExecutePage]: cooking static resource <%s>.\n", page_path);
    fflush(stdout);
#endif
*/

    if (page_data && page_size != 0)
    {
        pCookedPage = (PBREE_COOKED_BSP_PAGE)BreeCreateCookedPageObj(NULL, NULL, NULL);

        if (pCookedPage)
        {
            pCookedPage->pArchivedBsp   = page_data;
            pCookedPage->ulSize         = page_size;
            pCookedPage->bDynamic       = TRUE;         /* BREE is in charge of releasing the resources */
        }
    }

/*
#ifdef   _DEBUG
    printf("[BreeSpoExecutePage]: static resource <%s> has been cooked.\n", page_path);
    fflush(stdout);
#endif
*/

    return (ANSC_HANDLE)pCookedPage;
}


/**********************************************************************

    caller:     Broadway Web Resource Manager (BWRM)

    prototype:

        ANSC_STATUS
        BreeSpoExecuteStaticRes
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBwrmCookedPage,
                ANSC_HANDLE                 hBeepPecIf
            );

    description:

        This function is called to interpret a "cooked" BSP page.

    argument:   ANSC_HANDLE                 hThisObject
                This context handle is transparent to the BREE SPO
                object.

                ANSC_HANDLE                 hBwrmCookedPage
                The handle to a "cooked" BSP page which returns
                by "PreparePage".

                ANSC_HANDLE                 hBeepPecIf
                The handle to BEEP PEC interface which will be
                used to access underlying objects.

    return:     status of the operation.

**********************************************************************/

ANSC_STATUS
BreeSpoExecuteStaticRes
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBwrmCookedPage,
        ANSC_HANDLE                 hBeepPecIf
    )
{
    PBREE_SPO_OBJECT                pMyObject       = (PBREE_SPO_OBJECT)hThisObject;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;
    PBREE_COOKED_BSP_PAGE           pCookedPage     = (PBREE_COOKED_BSP_PAGE)hBwrmCookedPage;
    PBEEP_PEC_INTERFACE             pBeepPecIf      = (PBEEP_PEC_INTERFACE)hBeepPecIf;
    ANSC_HANDLE                     hResponse;
    SLAP_PARAMETER_LIST             paramList;
    SLAP_PARAMETER_LIST             *outList        = NULL;
    PSLAP_VARIABLE                  pParam;
    PSLAP_VARIABLE                  pRetVal = NULL;
    ULONG                           ulPecStatus;
    ANSC_HANDLE                     hBreeSoaIf      = (ANSC_HANDLE)NULL;

/*
#ifdef   _DEBUG
    printf("[BreeSpoExecutePage]: writing static resource into Repsonse object ...\n");
    fflush(stdout);
#endif
*/

    hBreeSoaIf  = 
        pMyObject->CreateBreeSoaIf
            (
                (ANSC_HANDLE)pMyObject,
                hBeepPecIf
            );

    if ( !hBreeSoaIf )
    {
        return ANSC_STATUS_RESOURCES;
    }

    hResponse   = pBeepPecIf->GetResponse(pBeepPecIf->hOwnerContext);

    /* call "Write" method of "Response" object to output page content */
    paramList.ParamCount        = 2;

    pParam                      = &paramList.ParamArray[0];
    AnscZeroMemory(pParam, sizeof(SLAP_VARIABLE));
    pParam->Syntax              = SLAP_VAR_SYNTAX_ptr;
    pParam->Variant.varPtr      = (SLAP_PTR)pCookedPage->pArchivedBsp;

    pParam                      = &paramList.ParamArray[1];
    AnscZeroMemory(pParam, sizeof(SLAP_VARIABLE));
    pParam->Syntax              = SLAP_VAR_SYNTAX_TYPE_uint32;
    pParam->Variant.varUint32   = pCookedPage->ulSize;

    /* invoke "BinaryWrite1" method of Response object to output the given string */
    pBeepPecIf->InvokeObject
        (
            pBeepPecIf->hOwnerContext,
            hResponse,
            (char *)"BinaryWrite1",
            &paramList,
            &outList,
            &pRetVal,
            &ulPecStatus
        );

    SlapCleanParamList((&paramList));    

    if (outList)
    {
        SlapFreeParamList(outList);
    }

    if (pRetVal)
    {
        SlapFreeVariable(pRetVal);
    }

/*
#ifdef   _DEBUG
    printf("[BreeSpoExecutePage]: static resource has been written to Repsonse object.\n");
    fflush(stdout);
#endif
*/

    /* hard-coded cache expiration timeout for static resources */
    pMyObject->EnforceCache((ANSC_HANDLE)pMyObject, hBeepPecIf, 3600);

    AnscFreeMemory(hBreeSoaIf);

    return status;
}


ANSC_STATUS
BreeSpoEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBREE_SPO_OBJECT                pMyObject       = (PBREE_SPO_OBJECT)hThisObject;
    PBEEP_SER_INTERFACE             pBeepSerIf      = (PBEEP_SER_INTERFACE)pMyObject->hBeepSerIf;

    if (!pMyObject->bActive)
    {
        pBeepSerIf->AttachEngine(pBeepSerIf->hOwnerContext, pMyObject->hBeepEngineDesc);
        pBeepSerIf->AttachEngine(pBeepSerIf->hOwnerContext, pMyObject->hStaticResEngineDesc);

        pMyObject->bActive  = TRUE;
    }

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
BreeSpoCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBREE_SPO_OBJECT                pMyObject       = (PBREE_SPO_OBJECT)hThisObject;
    PBEEP_SER_INTERFACE             pBeepSerIf      = (PBEEP_SER_INTERFACE)pMyObject->hBeepSerIf;

    if (pMyObject->bActive)
    {
        pBeepSerIf->DetachEngine(pBeepSerIf->hOwnerContext, BREE_SPO_ENGINE_NAME);
        pBeepSerIf->DetachEngine(pBeepSerIf->hOwnerContext, BREE_SPO_STATIC_ENGINE_NAME);

        pMyObject->bActive  = FALSE;
    }

    return ANSC_STATUS_SUCCESS;
}


