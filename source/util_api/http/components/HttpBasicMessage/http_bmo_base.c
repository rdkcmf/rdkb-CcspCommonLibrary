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

    module:	http_bmo_base.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Http Basic Message Object.

        *   HttpBmoCreate
        *   HttpBmoRemove
        *   HttpBmoEnrollObjects
        *   HttpBmoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/26/02    initial revision.

**********************************************************************/


#include "http_bmo_global.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        HttpBmoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Http Basic Message Object and
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
HttpBmoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PHTTP_BASIC_MESSAGE_OBJECT)AnscAllocateMemory(sizeof(HTTP_BASIC_MESSAGE_OBJECT));

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
    /* AnscCopyString(pBaseObject->Name, HTTP_BASIC_MESSAGE_NAME); */

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = HTTP_BASIC_MESSAGE_OID;
    pBaseObject->Create            = HttpBmoCreate;
    pBaseObject->Remove            = HttpBmoRemove;
    pBaseObject->EnrollObjects     = HttpBmoEnrollObjects;
    pBaseObject->Initialize        = HttpBmoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoRemove
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
HttpBmoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject    = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
    PHTTP_BCC_INTERFACE             pBccIf       = (PHTTP_BCC_INTERFACE       )pMyObject->hBccIf;

    pMyObject->Reset((ANSC_HANDLE)pMyObject);

    if ( pBccIf )
    {
        AnscFreeMemory(pBccIf);
    }

    AnscFreeLock(&pMyObject->HfoTableLock);
    AnscCoRemove((ANSC_HANDLE)pMyObject  );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoEnrollObjects
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
HttpBmoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject    = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
    PHTTP_BCC_INTERFACE             pBccIf       = (PHTTP_BCC_INTERFACE       )pMyObject->hBccIf;

    if ( !pBccIf )
    {
        pBccIf = (PHTTP_BCC_INTERFACE)AnscAllocateMemory(sizeof(HTTP_BCC_INTERFACE));

        if ( !pBccIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hBccIf = (ANSC_HANDLE)pBccIf;
        }

        pBccIf->hOwnerContext   = (ANSC_HANDLE)pMyObject;
        pBccIf->Size            = sizeof(HTTP_BCC_INTERFACE);

        pBccIf->GetHeaderField  = HttpBmoBccGetHeaderField;
        pBccIf->AddHeaderField  = HttpBmoBccAddHeaderField;
        pBccIf->PredictBodySize = HttpBmoBccPredictBodySize;
        pBccIf->Notify          = HttpBmoBccNotify;
        pBccIf->Serialize       = HttpBmoBccSerialize;
    }

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoInitialize
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
HttpBmoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BASIC_MESSAGE_OBJECT      pMyObject    = (PHTTP_BASIC_MESSAGE_OBJECT)hThisObject;
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
    pMyObject->Oid                  = HTTP_BASIC_MESSAGE_OID;
    pMyObject->Create               = HttpBmoCreate;
    pMyObject->Remove               = HttpBmoRemove;
    pMyObject->EnrollObjects        = HttpBmoEnrollObjects;
    pMyObject->Initialize           = HttpBmoInitialize;

    pMyObject->hHfpIf               = (ANSC_HANDLE)NULL;
    pMyObject->hTmhIf               = (ANSC_HANDLE)NULL;
    pMyObject->hFumIf               = (ANSC_HANDLE)NULL;
    pMyObject->hRecvContext         = (ANSC_HANDLE)NULL;
    pMyObject->hSendContext         = (ANSC_HANDLE)NULL;
    pMyObject->hHttpWebsTrans       = (ANSC_HANDLE)NULL;
    pMyObject->hWebServer           = (ANSC_HANDLE)NULL;
    pMyObject->hWebHost             = (ANSC_HANDLE)NULL;
    pMyObject->hWebServlet          = (ANSC_HANDLE)NULL;
    pMyObject->hMessageBody         = (ANSC_HANDLE)NULL;
    pMyObject->hHeaderBdo           = (ANSC_HANDLE)NULL;
    pMyObject->State                = HTTP_BMO_STATE_EMPTY;
    pMyObject->MboMode              = HTTP_MBO_MODE_RELAY;
    pMyObject->TransactionId        = 0;
    pMyObject->PadSize1             = HTTP_BMO_SCRATCH_PAD1_SIZE;
    pMyObject->PadSize2             = HTTP_BMO_SCRATCH_PAD2_SIZE;

    pMyObject->GetHfpIf             = HttpBmoGetHfpIf;
    pMyObject->SetHfpIf             = HttpBmoSetHfpIf;
    pMyObject->GetTmhIf             = HttpBmoGetTmhIf;
    pMyObject->SetTmhIf             = HttpBmoSetTmhIf;
    pMyObject->GetFumIf             = HttpBmoGetFumIf;
    pMyObject->SetFumIf             = HttpBmoSetFumIf;
    pMyObject->GetRecvContext       = HttpBmoGetRecvContext;
    pMyObject->SetRecvContext       = HttpBmoSetRecvContext;
    pMyObject->GetSendContext       = HttpBmoGetSendContext;
    pMyObject->SetSendContext       = HttpBmoSetSendContext;
    pMyObject->GetHttpWebsTrans     = HttpBmoGetHttpWebsTrans;
    pMyObject->SetHttpWebsTrans     = HttpBmoSetHttpWebsTrans;
    pMyObject->GetWebServer         = HttpBmoGetWebServer;
    pMyObject->SetWebServer         = HttpBmoSetWebServer;
    pMyObject->GetWebHost           = HttpBmoGetWebHost;
    pMyObject->SetWebHost           = HttpBmoSetWebHost;
    pMyObject->GetWebServlet        = HttpBmoGetWebServlet;
    pMyObject->SetWebServlet        = HttpBmoSetWebServlet;
    pMyObject->GetState             = HttpBmoGetState;
    pMyObject->SetState             = HttpBmoSetState;
    pMyObject->GetMboMode           = HttpBmoGetMboMode;
    pMyObject->SetMboMode           = HttpBmoSetMboMode;
    pMyObject->GetTransactionId     = HttpBmoGetTransactionId;
    pMyObject->SetTransactionId     = HttpBmoSetTransactionId;
    pMyObject->GetWebSessionId      = HttpBmoGetWebSessionId;
    pMyObject->SetWebSessionId      = HttpBmoSetWebSessionId;

    pMyObject->Return               = HttpBmoReturn;
    pMyObject->Reset                = HttpBmoReset;

    pMyObject->GetStartLineSize     = HttpBmoGetStartLineSize;
    pMyObject->GetHeadersSize       = HttpBmoGetHeadersSize;
    pMyObject->PredictBodySize      = HttpBmoPredictBodySize;
    pMyObject->GetBodySize          = HttpBmoGetBodySize;
    pMyObject->GetBodySize2         = HttpBmoGetBodySize2;
    pMyObject->GetMessageSize       = HttpBmoGetMessageSize;
    pMyObject->GetPtmForHeaders     = HttpBmoGetPtmForHeaders;
    pMyObject->GetPtmForBody        = HttpBmoGetPtmForBody;

    pMyObject->GetHeaderBdo         = HttpBmoGetHeaderBdo;
    pMyObject->SetHeaderBdo         = HttpBmoSetHeaderBdo;

    pMyObject->IsProxyKeepAlive     = HttpBmoIsProxyKeepAlive;
    pMyObject->IsCloseConnection    = HttpBmoIsCloseConnection;
    pMyObject->IsConnKeepAlive      = HttpBmoIsConnKeepAlive;
    pMyObject->IsChunkedCoding      = HttpBmoIsChunkedCoding;
    pMyObject->IsHttpVersion11      = HttpBmoIsHttpVersion11;

    pMyObject->GetHeaderValueById   = HttpBmoGetHeaderValueById;
    pMyObject->GetHeaderValueById2  = HttpBmoGetHeaderValueById2;
    pMyObject->GetHeaderValueByName = HttpBmoGetHeaderValueByName;
    pMyObject->GetHeaderValueByName2= HttpBmoGetHeaderValueByName2;
    pMyObject->SetHeaderValueByName = HttpBmoSetHeaderValueByName;

    pMyObject->DelStartLine         = HttpBmoDelStartLine;
    pMyObject->ParseStartLine       = HttpBmoParseStartLine;
    pMyObject->ClearHeaders         = HttpBmoClearHeaders;
    pMyObject->ParseHeaders         = HttpBmoParseHeaders;
    pMyObject->CopyStartLineFrom    = HttpBmoCopyStartLineFrom;
    pMyObject->CopyHeadersFrom      = HttpBmoCopyHeadersFrom;
    pMyObject->GetHeaderField       = HttpBmoGetHeaderField;
    pMyObject->GetHeaderField2      = HttpBmoGetHeaderField2;
    pMyObject->AddHeaderField       = HttpBmoAddHeaderField;
    pMyObject->DelHeaderField       = HttpBmoDelHeaderField;
    pMyObject->DelAllHfos           = HttpBmoDelAllHfos;

    pMyObject->ClearBody            = HttpBmoClearBody;
    pMyObject->CopyBodyFrom         = HttpBmoCopyBodyFrom;
    pMyObject->CopyBodyInto         = HttpBmoCopyBodyInto;
    pMyObject->SaveAsBody           = HttpBmoSaveAsBody;
    pMyObject->SaveAsMbo            = HttpBmoSaveAsMbo;
    pMyObject->CreateMessageBody    = HttpBmoCreateMessageBody;
    pMyObject->AppendBody           = HttpBmoAppendBody;
    pMyObject->AppendBodyBdo        = HttpBmoAppendBodyBdo;
#ifdef _ANSC_HTTP_FILE_USED_
    pMyObject->AppendBodyFile       = HttpBmoAppendBodyFile;
    pMyObject->SaveBodyAsFile       = HttpBmoSaveBodyAsFile;
    pMyObject->SaveFormAsFile       = HttpBmoSaveFormAsFile;
#endif

    pMyObject->Examine              = HttpBmoExamine;
    pMyObject->Process              = HttpBmoProcess;
    pMyObject->CloseUp              = HttpBmoCloseUp;
    pMyObject->Produce              = HttpBmoProduce;
    pMyObject->OutputHeaders        = HttpBmoOutputHeaders;
    pMyObject->OutputBody           = HttpBmoOutputBody;
    pMyObject->SendComplete         = HttpBmoSendComplete;

    pMyObject->BccGetHeaderField    = HttpBmoBccGetHeaderField;
    pMyObject->BccAddHeaderField    = HttpBmoBccAddHeaderField;
    pMyObject->BccPredictBodySize   = HttpBmoBccPredictBodySize;
    pMyObject->BccNotify            = HttpBmoBccNotify;
    pMyObject->BccSerialize         = HttpBmoBccSerialize;

    for ( i = 0; i < HTTP_BMO_HFO_TABLE_SIZE; i++ )
    {
        AnscQueueInitializeHeader(&pMyObject->HfoTable[i]);
    }
    AnscInitializeLock(&pMyObject->HfoTableLock);

    return  ANSC_STATUS_SUCCESS;
}
