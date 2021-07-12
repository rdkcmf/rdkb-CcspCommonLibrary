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

    module:	bmc2_comeo_process.c

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the exported functions provided by the Bmc2 Com Executor
        Object.

        *   Bmc2ComeoPreparePage
        *   Bmc2ComeoExecutePage

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        06/28/05    initial revision.

**********************************************************************/

#include "bmc2_comeo_global.h"
#include "safec_lib_common.h"

/**********************************************************************

    prototype:

        ANSC_HANDLE
        Bmc2ComeoPreparePage
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
                This context handle is transparent to the BMC2 COMEO
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
Bmc2ComeoPreparePage
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

#if defined(_BREE_SPO_USE_SRMO) || defined(_USE_PRECOMPILED_SCP_PAGES)
    /* all resources have been well cooked */

    PBMC2_COM_EXECUTOR_OBJECT       pMyObject   = (PBMC2_COM_EXECUTOR_OBJECT)hThisObject;
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

    PBMC2_COM_EXECUTOR_OBJECT       pMyObject   = (PBMC2_COM_EXECUTOR_OBJECT)hThisObject;
    PBREE_COOKED_BSP_PAGE           pCookedPage = NULL;
    char                            *pTemplate  = page_path;
    PBMC2_COMEO_READER_OBJECT       pReader;
    PBSP_TEMPLATE_ARCHIVE_OBJECT    pArchive;
    BOOL                            bSucc;
    PBSP_TEMPLATE_LIST_OBJECT       pList;
    PUCHAR                          pArcData    = NULL;
    ULONG                           ulArcDataSize;
    BOOL                            bNeedRelease= FALSE;

/*
#ifdef   _DEBUG
    printf("[Bmc2ComeoPreparePage]: cooking BSP page <%s> ...\n", page_path);
    fflush(stdout);
#endif
*/

    pArchive    = (PBSP_TEMPLATE_ARCHIVE_OBJECT)CreateBspEngArchiveComponent  (NULL, NULL, NULL);
    pList       = (PBSP_TEMPLATE_LIST_OBJECT   )CreateBspEngListComponent     (NULL, NULL, NULL);
    pReader     = (PBMC2_COMEO_READER_OBJECT   )CreateBmc2ComeoReaderComponent(NULL, NULL, NULL);

    if (!pArchive || !pList || !pReader)
    {
        goto TERMINATED;
    }

    pList->SetBspSoaIf((ANSC_HANDLE)pList, pMyObject->hBmc2SoaIf);

    pReader->OpenInternal((ANSC_HANDLE)pReader, pTemplate, (char *)page_data, page_size);

    if (pReader->Valid((ANSC_HANDLE)pReader))
    {
        pList->AddGroup((ANSC_HANDLE)pList, (ANSC_HANDLE)pReader, TRUE, pTemplate);

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

            /* free raw data buffer */
            AnscFreeMemory(page_data);
        }
    }
#endif

/*
#ifdef   _DEBUG
    printf("[Bmc2ComeoPreparePage]: BSP page <%s> has been cooked.\n", page_path);
    fflush(stdout);
#endif
*/

    return (ANSC_HANDLE)pCookedPage;
}


/**********************************************************************

    prototype:

        ANSC_STATUS
        Bmc2ComeoExecutePage
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBwrmCookedPage,
                ANSC_HANDLE                 hBmc2PecIf
            );

    description:

        This function is called to interpret a "cooked" BSP page.

    argument:   ANSC_HANDLE                 hThisObject
                This context handle is transparent to the BMC2 COMEO
                object.

                ANSC_HANDLE                 hBwrmCookedPage
                The handle to a "cooked" BSP page which returns
                by "PreparePage".

                ANSC_HANDLE                 hBmc2PecIf
                The handle to BMC2 PEC interface which will be
                used to access underlying objects.

    return:     status of the operation.

**********************************************************************/

ANSC_STATUS
Bmc2ComeoExecutePage
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBwrmCookedPage,
        ANSC_HANDLE                 hBmc2PecIf
    )
{
    PBMC2_COM_EXECUTOR_OBJECT       pMyObject       = (PBMC2_COM_EXECUTOR_OBJECT)hThisObject;
    ANSC_STATUS                     status          = ANSC_STATUS_SUCCESS;
    PBREE_COOKED_BSP_PAGE           pCookedPage     = (PBREE_COOKED_BSP_PAGE)hBwrmCookedPage;
    char                            *pTemplateName  = NULL;
    PBMC2_COMEO_READER_OBJECT       pReader         = NULL;
    BOOL                            bReaderOK       = FALSE;
    PBSP_TEMPLATE_LIST_OBJECT       pList           = NULL;
    PBSP_TEMPLATE_ARCHIVE_OBJECT    pArchive        = NULL;
    ANSC_HANDLE                     hBmc2SoaIf      = NULL;

    pReader = (PBMC2_COMEO_READER_OBJECT)CreateBmc2ComeoReaderComponent(NULL, NULL, NULL);

    if (!pReader)
    {
        goto OUT_OF_RESOURCES;
    }

    hBmc2SoaIf      = 
        pMyObject->CreateBmc2SoaIf
            (
                (ANSC_HANDLE)pMyObject,
                hBmc2PecIf
            );

    if ( !hBmc2SoaIf )
    {
        goto OUT_OF_RESOURCES;
    }

    pTemplateName   = pCookedPage->PagePath;

/*
#ifdef   _DEBUG
    printf("[Bmc2ComeoExecutePage]: executing BSP page <%s> ...\n", pTemplateName);
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

        /* set up BMC2 PEC interface */
        pList->SetBspSoaIf((ANSC_HANDLE)pList, hBmc2SoaIf);
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
            pTmpl->Run((ANSC_HANDLE)pTmpl, hThisObject, (ANSC_HANDLE)NULL, (ANSC_HANDLE)NULL, NULL);
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

    if ( hBmc2SoaIf )
    {
        AnscFreeMemory(hBmc2SoaIf);
    }

/*
#ifdef   _DEBUG
    printf("[Bmc2ComeoExecutePage]: done with BSP page <%s>.\n", pTemplateName);
    fflush(stdout);
#endif
*/

    return status;
}


/**********************************************************************

    prototype:

        ANSC_HANDLE
        Bmc2ComeoCreateBmc2SoaIf
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmc2PecIf
            )

    description:

        This function is called to create BSP SOA interface.

    argument:   ANSC_HANDLE                 hThisObject
                This context handle is transparent to the BMC2 COMEO
                object.

                ANSC_HANDLE                 hBmc2PecIf
                The handle to BMC2 PEC interface which will be
                used to access underlying objects.

    return:     status of the operation.

**********************************************************************/

ANSC_HANDLE
Bmc2ComeoCreateBmc2SoaIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmc2PecIf
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PBSPENG_SOA_INTERFACE           pBmc2SoaIf;
    errno_t   rc   = -1;

    pBmc2SoaIf = (PBSPENG_SOA_INTERFACE)AnscAllocateMemory(sizeof(BSPENG_SOA_INTERFACE));

    if ( !pBmc2SoaIf )
    {
        return  NULL;
    }

    rc = STRCPY_S_NOCLOBBER(pBmc2SoaIf->Name, sizeof(pBmc2SoaIf->Name), BMC2_SOA_INTERFACE_NAME);
    ERR_CHK(rc);

    pBmc2SoaIf->InterfaceId         = BMC2_SOA_INTERFACE_ID;
    pBmc2SoaIf->hOwnerContext       = (ANSC_HANDLE)hBmc2PecIf;
    pBmc2SoaIf->Size                = sizeof(BSPENG_SOA_INTERFACE);

    pBmc2SoaIf->GetSlapObject       = Bmc2ComeoSoaGetSlapObject;
    pBmc2SoaIf->InvokeObject        = Bmc2ComeoSoaInvokeObject;
    pBmc2SoaIf->IsBuiltInObj        = Bmc2ComeoSoaIsBuiltInObject;
    pBmc2SoaIf->WriteAString        = Bmc2ComeoSoaWriteAString;
    pBmc2SoaIf->WriteBString        = Bmc2ComeoSoaWriteBString;
    pBmc2SoaIf->GetCookedPage       = Bmc2ComeoSoaGetCookedPage;
    pBmc2SoaIf->RetCookedPage       = Bmc2ComeoSoaRetCookedPage;
    pBmc2SoaIf->GetCookedPageData   = Bmc2ComeoSoaGetCookedPageData;
    pBmc2SoaIf->IsInterrupted       = Bmc2ComeoSoaIsInterrupted;

    return (ANSC_HANDLE)pBmc2SoaIf;
}

