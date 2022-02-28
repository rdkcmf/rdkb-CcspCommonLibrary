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

    module:	http_sco_base.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Http Simple Client Object.

        *   HttpScoCreate
        *   HttpScoRemove
        *   HttpScoEnrollObjects
        *   HttpScoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        04/20/02    initial revision.

**********************************************************************/


#include "http_sco_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        HttpScoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Http Simple Client Object and
        initializes the member variables and functions.

    argument:   ANSC_HANDLE                 hContainerContext
                This handle is used by the container object to interact
                with the outside world. It could be the real container
                or an target object.

                ANSC_HANDLE                 hOwnerContext
                This handle is passed in by the owner of this object.

                ANSC_HANDLE                 hAnscReserved
                This handle is passed in by the owner of this object.

    return:     newly created container object.

**********************************************************************/

ANSC_HANDLE
HttpScoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = NULL;
#ifndef _CCSP_CWMP_TCP_CONNREQ_HANDLER
    errno_t                         rc = -1;
#endif

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PHTTP_SIMPLE_CLIENT_OBJECT)AnscAllocateMemory(sizeof(HTTP_SIMPLE_CLIENT_OBJECT));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pBaseObject = (PANSC_COMPONENT_OBJECT)pMyObject;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
#ifndef _CCSP_CWMP_TCP_CONNREQ_HANDLER
    rc = strcpy_s(pBaseObject->Name, sizeof(pBaseObject->Name), HTTP_SIMPLE_CLIENT_NAME);
    ERR_CHK(rc);
#endif

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = HTTP_SIMPLE_CLIENT_OID;
    pBaseObject->Create            = HttpScoCreate;
    pBaseObject->Remove            = HttpScoRemove;
    pBaseObject->EnrollObjects     = HttpScoEnrollObjects;
    pBaseObject->Initialize        = HttpScoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpScoRemove
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
HttpScoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject         = (PHTTP_SIMPLE_CLIENT_OBJECT   )hThisObject;
    PHTTP_BSP_INTERFACE             pBspIf            = (PHTTP_BSP_INTERFACE          )pMyObject->hBspIf;
    PHTTP_HFP_INTERFACE             pHfpIf            = (PHTTP_HFP_INTERFACE          )pMyObject->hHfpIf;
    PHTTP_CAS_INTERFACE             pCasIf            = (PHTTP_CAS_INTERFACE          )pMyObject->hCasIf;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pSessionTdo       = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hSessionTdo;
    PANSC_TDO_CLIENT_OBJECT         pSessionTdoClient = (PANSC_TDO_CLIENT_OBJECT      )pMyObject->hSessionTdoClient;
    PHTTP_AUTH_CLIENT_OBJECT        pClientAuthObj    = (PHTTP_AUTH_CLIENT_OBJECT     )pMyObject->hClientAuthObj;
    PHTTP_ACM_INTERFACE             pAcmIf            = (PHTTP_ACM_INTERFACE          )pMyObject->hAcmIf;

    pMyObject->Cancel((ANSC_HANDLE)pMyObject);
    pMyObject->Reset ((ANSC_HANDLE)pMyObject);

    if ( pBspIf )
    {
        AnscFreeMemory(pBspIf);
    }

    if ( pHfpIf )
    {
        AnscFreeMemory(pHfpIf);
    }

    if ( pCasIf )
    {
        AnscFreeMemory(pCasIf);
    }

    if ( pSessionTdo )
    {
        pSessionTdo->Remove((ANSC_HANDLE)pSessionTdo);
    }

    if ( pSessionTdoClient )
    {
        AnscFreeMemory(pSessionTdoClient);
    }

    if ( pClientAuthObj )
    {
        pClientAuthObj->Remove((ANSC_HANDLE)pClientAuthObj);
    }

    if ( pAcmIf )
    {
        AnscFreeMemory(pAcmIf);
    }

    if ( pMyObject->pAuthUserName )
    {
        AnscFreeMemory(pMyObject->pAuthUserName);
    }

    if ( pMyObject->pAuthPassword )
    {
        AnscFreeMemory(pMyObject->pAuthPassword);
    }

    if ( pMyObject->hostNames.peerVerify )
    {
	if ( pMyObject->hostNames.hostNames && pMyObject->hostNames.numHosts > 0 )
	{
	    int i = 0;
	    for ( i = 0; i < pMyObject->hostNames.numHosts; i++)
	    {
		if(pMyObject->hostNames.hostNames[i])
		{
		    AnscFreeMemory(pMyObject->hostNames.hostNames[i]);
		    pMyObject->hostNames.hostNames[i] = NULL;
		}
	    }
	    AnscFreeMemory(pMyObject->hostNames.hostNames );
	    pMyObject->hostNames.hostNames = NULL;
	}
    }

    AnscFreeLock(&pMyObject->WcsoTableLock  );
    AnscFreeLock(&pMyObject->WcsoSListLock  );
    AnscFreeLock(&pMyObject->WctoSListLock  );
    AnscFreeLock(&pMyObject->BmoReqSListLock);
    AnscFreeLock(&pMyObject->BmoRepSListLock);

    AnscCoRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpScoEnrollObjects
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function enrolls all the objects required by this object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpScoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject         = (PHTTP_SIMPLE_CLIENT_OBJECT   )hThisObject;
    PHTTP_BSP_INTERFACE             pBspIf            = (PHTTP_BSP_INTERFACE          )pMyObject->hBspIf;
    PHTTP_HFP_INTERFACE             pHfpIf            = (PHTTP_HFP_INTERFACE          )pMyObject->hHfpIf;
    PHTTP_CAS_INTERFACE             pCasIf            = (PHTTP_CAS_INTERFACE          )pMyObject->hCasIf;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pSessionTdo       = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hSessionTdo;
    PANSC_TDO_CLIENT_OBJECT         pSessionTdoClient = (PANSC_TDO_CLIENT_OBJECT      )pMyObject->hSessionTdoClient;

    if ( !pBspIf )
    {
        pBspIf = (PHTTP_BSP_INTERFACE)AnscAllocateMemory(sizeof(HTTP_BSP_INTERFACE));

        if ( !pBspIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hBspIf = (ANSC_HANDLE)pBspIf;
        }

        pBspIf->hOwnerContext = (ANSC_HANDLE)pMyObject;
        pBspIf->Size          = sizeof(HTTP_BSP_INTERFACE);
        pBspIf->Polish        = HttpScoBspPolish;
        pBspIf->Browse        = HttpScoBspBrowse;
        pBspIf->Notify        = HttpScoBspNotify;
    }

    if ( !pHfpIf )
    {
        pHfpIf = (PHTTP_HFP_INTERFACE)AnscAllocateMemory(sizeof(HTTP_HFP_INTERFACE));

        if ( !pHfpIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            /* Initialize the data HFP_interface */
            pHfpIf->ParseHeader                     = AnscHttpParseHeader;
            pHfpIf->ParseRequestLine                = AnscHttpParseRequestLine;
            pHfpIf->ParseStatusLine                 = AnscHttpParseStatusLine;
            pHfpIf->ParseChunkedLine                = AnscHttpParseChunkedLine;
            pHfpIf->ParseHttpUrl                    = AnscHttpParseUrl;
            pHfpIf->ParseQueryString                = AnscHttpParseQueryString;
            pHfpIf->GetHeaderParam                  = AnscHttpGetHeaderParam;
            pHfpIf->GetHeaderSize                   = AnscHttpGetHeaderSize;
            pHfpIf->GetStatusLineSize               = AnscHttpGetStatusLineSize;
            pHfpIf->GetRequestLineSize              = AnscHttpGetRequestLineSize;
            pHfpIf->BuildHeader                     = AnscHttpBuildHeader;
            pHfpIf->BuildRequestLine                = AnscHttpBuildRequestLine;
            pHfpIf->BuildStatusLine                 = AnscHttpBuildStatusLine;

            /* set the owner context and size of the structure */
            pHfpIf->hOwnerContext                   = hThisObject;
            pHfpIf->Size                            = sizeof(HTTP_HFP_INTERFACE);

            /* register minimum header info */
            AnscHttpRegisterAllHeadersXInfo();
            pMyObject->hHfpIf = (ANSC_HANDLE)pHfpIf;
        }
#ifndef _CCSP_CWMP_TCP_CONNREQ_HANDLER
        pHfpIf->hOwnerContext = (ANSC_HANDLE)NULL;
        pHfpIf->Size          = sizeof(HTTP_HFP_INTERFACE);
#endif
    }

    if ( !pCasIf )
    {
        pCasIf = (PHTTP_CAS_INTERFACE)AnscAllocateMemory(sizeof(HTTP_CAS_INTERFACE));

        if ( !pCasIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hCasIf   = (ANSC_HANDLE)pCasIf;
        }

        pCasIf->hOwnerContext   = (ANSC_HANDLE)pMyObject;
        pCasIf->Size            = sizeof(HTTP_CAS_INTERFACE);

        pCasIf->EnableAuth      = HttpScoCasEnableAuth;
        pCasIf->IsAuthEnabled   = HttpScoCasIsAuthEnabled;
        pCasIf->ProcessRequest  = HttpScoCasProcessRequest;
        pCasIf->ProcessResponse = HttpScoCasProcessResponse;
        pCasIf->ClearAuthInfo   = HttpScoCasClearAuthInfo;
    }

    if ( !pSessionTdo )
    {
        pSessionTdo =
            (PANSC_TIMER_DESCRIPTOR_OBJECT)AnscCreateTimerDescriptor
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pSessionTdo )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hSessionTdo = (ANSC_HANDLE)pSessionTdo;
        }

        pSessionTdo->SetTimerType((ANSC_HANDLE)pSessionTdo, ANSC_TIMER_TYPE_PERIODIC      );
        pSessionTdo->SetInterval ((ANSC_HANDLE)pSessionTdo, HTTP_SCO_SESSION_TIME_INTERVAL);
    }

    if ( !pSessionTdoClient )
    {
        pSessionTdoClient = (PANSC_TDO_CLIENT_OBJECT)AnscAllocateMemory(sizeof(ANSC_TDO_CLIENT_OBJECT));

        if ( !pSessionTdoClient )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hSessionTdoClient = (ANSC_HANDLE)pSessionTdoClient;
        }

        pSessionTdoClient->hClientContext = (ANSC_HANDLE)pMyObject;
        pSessionTdoClient->Invoke         = HttpScoSessionTdoInvoke;

        pSessionTdo->SetClient((ANSC_HANDLE)pSessionTdo, (ANSC_HANDLE)pSessionTdoClient);
    }

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpScoInitialize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function first calls the initialization member function
        of the base class object to set the common member fields
        inherited from the base class. It then initializes the member
        fields that are specific to this object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpScoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_CLIENT_OBJECT      pMyObject    = (PHTTP_SIMPLE_CLIENT_OBJECT)hThisObject;
    ULONG                           i            = 0;

    /*
     * Until you have to simulate C++ object-oriented programming style with standard C, you don't
     * appreciate all the nice little things come with C++ language and all the dirty works that
     * have been done by the C++ compilers. Member initialization is one of these things. While in
     * C++ you don't have to initialize all the member fields inherited from the base class since
     * the compiler will do it for you, such is not the case with C.
     */
    AnscCoInitialize((ANSC_HANDLE)pMyObject);

    /*
     * Although we have initialized some of the member fields in the "create" member function, we
     * repeat the work here for completeness. While this simulation approach is pretty stupid from
     * a C++/Java programmer perspective, it's the best we can get for universal embedded network
     * programming. Before we develop our own operating system (don't expect that to happen any
     * time soon), this is the way things gonna be.
     */
    pMyObject->Oid                   = HTTP_SIMPLE_CLIENT_OID;
    pMyObject->Create                = HttpScoCreate;
    pMyObject->Remove                = HttpScoRemove;
    pMyObject->EnrollObjects         = HttpScoEnrollObjects;
    pMyObject->Initialize            = HttpScoInitialize;

    pMyObject->ClientMode            = HTTP_SCO_MODE_COMPACT;
    pMyObject->bActive               = FALSE;
    pMyObject->hWebClientSession     = NULL;
	pMyObject->SessionIdleTimeout	 = 0;

    pMyObject->GetBspIf              = HttpScoGetBspIf;
    pMyObject->SetBspIf              = HttpScoSetBspIf;
    pMyObject->GetHfpIf              = HttpScoGetHfpIf;
    pMyObject->SetHfpIf              = HttpScoSetHfpIf;
    pMyObject->GetCasIf              = HttpScoGetCasIf;
    pMyObject->GetClientAuthObj      = HttpScoGetClientAuthObj;
    pMyObject->GetClientMode         = HttpScoGetClientMode;
    pMyObject->SetClientMode         = HttpScoSetClientMode;
    pMyObject->GetProductName        = HttpScoGetProductName;
    pMyObject->SetProductName        = HttpScoSetProductName;
    pMyObject->GetHostNames	     = HttpScoGetHostNames;
    pMyObject->SetHostNames	     = HttpScoSetHostNames;
    pMyObject->SetSessionIdleTimeout = HttpScoSetSessionIdleTimeout;

    pMyObject->GetProperty           = HttpScoGetProperty;
    pMyObject->SetProperty           = HttpScoSetProperty;
    pMyObject->ResetProperty         = HttpScoResetProperty;
    pMyObject->Reset                 = HttpScoReset;

    pMyObject->GetCredential         = HttpScoGetCredential;

    pMyObject->Engage                = HttpScoEngage;
    pMyObject->Cancel                = HttpScoCancel;
    pMyObject->SessionTdoInvoke      = HttpScoSessionTdoInvoke;

    pMyObject->AcquireWcso           = HttpScoAcquireWcso;
    pMyObject->ReleaseWcso           = HttpScoReleaseWcso;
    pMyObject->ManufactureWcsoPool   = HttpScoManufactureWcsoPool;
    pMyObject->DestroyWcsoPool       = HttpScoDestroyWcsoPool;

    pMyObject->AcquireWcto           = HttpScoAcquireWcto;
    pMyObject->ReleaseWcto           = HttpScoReleaseWcto;
    pMyObject->ManufactureWctoPool   = HttpScoManufactureWctoPool;
    pMyObject->DestroyWctoPool       = HttpScoDestroyWctoPool;

    pMyObject->AcquireBmoReq         = HttpScoAcquireBmoReq;
    pMyObject->ReleaseBmoReq         = HttpScoReleaseBmoReq;
    pMyObject->ManufactureBmoReqPool = HttpScoManufactureBmoReqPool;
    pMyObject->DestroyBmoReqPool     = HttpScoDestroyBmoReqPool;

    pMyObject->AcquireBmoRep         = HttpScoAcquireBmoRep;
    pMyObject->ReleaseBmoRep         = HttpScoReleaseBmoRep;
    pMyObject->ManufactureBmoRepPool = HttpScoManufactureBmoRepPool;
    pMyObject->DestroyBmoRepPool     = HttpScoDestroyBmoRepPool;

    pMyObject->Request               = HttpScoRequest;
    pMyObject->Request2              = HttpScoRequest2;
    pMyObject->Request3              = HttpScoRequest3;
    pMyObject->DoOptions             = HttpScoDoOptions;
    pMyObject->DoGet                 = HttpScoDoGet;
    pMyObject->DoHead                = HttpScoDoHead;
    pMyObject->DoPost                = HttpScoDoPost;
    pMyObject->DoPut                 = HttpScoDoPut;
    pMyObject->DoDelete              = HttpScoDoDelete;
    pMyObject->DoTrace               = HttpScoDoTrace;
    pMyObject->DoConnect             = HttpScoDoConnect;

    pMyObject->MapWcso               = HttpScoMapWcso;
    pMyObject->GetWcso               = HttpScoGetWcso;
    pMyObject->AddWcso               = HttpScoAddWcso;
    pMyObject->DelAllWcsos           = HttpScoDelAllWcsos;

    pMyObject->BspPolish             = HttpScoBspPolish;
    pMyObject->BspBrowse             = HttpScoBspBrowse;
    pMyObject->BspNotify             = HttpScoBspNotify;

    pMyObject->SetPeerAddresses      = HttpScoSetPeerAddresses;
    pMyObject->GetNextPeerAddr       = HttpScoGetNextPeerAddr;
    pMyObject->GetCurPeerAddr        = HttpScoGetCurPeerAddr;
    pMyObject->ResetPeerAddrPick     = HttpScoResetPeerAddrPick;


    for ( i = 0; i < HTTP_SCO_WCSO_TABLE_SIZE; i++ )
    {
        AnscSListInitializeHeader(&pMyObject->WcsoTable[i]);
    }
    AnscInitializeLock(&pMyObject->WcsoTableLock);

    AnscSListInitializeHeader(&pMyObject->WcsoSList      );
    AnscInitializeLock       (&pMyObject->WcsoSListLock  );
    AnscSListInitializeHeader(&pMyObject->WctoSList      );
    AnscInitializeLock       (&pMyObject->WctoSListLock  );

    AnscSListInitializeHeader(&pMyObject->BmoReqSList    );
    AnscInitializeLock       (&pMyObject->BmoReqSListLock);
    AnscSListInitializeHeader(&pMyObject->BmoRepSList    );
    AnscInitializeLock       (&pMyObject->BmoRepSListLock);

    /*
     * We shall initialize the configuration properties to the default values, which may be changed
     * later via the set_property() member function. Note that this call may not guarantee a valid
     * and legtimate configuration.
     */
    pMyObject->ResetProperty((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
