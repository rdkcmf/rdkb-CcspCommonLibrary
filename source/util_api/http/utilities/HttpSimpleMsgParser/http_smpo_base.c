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

    module:	http_smpo_base.c

        For HTTP Simple Message Parser (syntax parser),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic component object functions
        of the HTTP message parser component object.

        *   HttpSmpoCreate
        *   HttpSmpoRemove
        *   HttpSmpoEnrollObjects
        *   HttpSmpoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        02/22/02    initial revision.

    last modified:

        02/22/02

**********************************************************************/


#include "http_smpo_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        HttpSmpoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the http message parser Component object 
        and initializes the member variables and functions.

    argument:   ANSC_HANDLE                 hContainerContext
                This handle is used by the container object to interact
                with the outside world. It could be the real container
                or an target object.

                ANSC_HANDLE                 hOwnerContext
                This handle is passed in by the owner of this object.

                ANSC_HANDLE                 hAnscReserved
                This handle is passed in by the owner of this object.

    return:     newly created tokenizer component object.

**********************************************************************/

ANSC_HANDLE
HttpSmpoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PHTTP_SIMPLE_MSG_PARSER         pMyObject    = NULL;
    errno_t                         rc           = -1;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PHTTP_SIMPLE_MSG_PARSER)AnscAllocateMemory(sizeof(HTTP_SIMPLE_MSG_PARSER));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pBaseObject = (PANSC_COMPONENT_OBJECT)pMyObject;
    }

    /*
     * Initialize the common variables and functions for a component object.
     */
    rc = strcpy_s(pBaseObject->Name, sizeof(pBaseObject->Name), HTTP_SMPO_COMPONENT_NAME);
    ERR_CHK(rc);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = HTTP_SMPO_COMPONENT_OID;
    pBaseObject->Create            = HttpSmpoCreate;
    pBaseObject->Remove            = HttpSmpoRemove;
    pBaseObject->EnrollObjects     = HttpSmpoEnrollObjects;
    pBaseObject->Initialize        = HttpSmpoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSmpoRemove
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
HttpSmpoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_MSG_PARSER         pMyObject    = (PHTTP_SIMPLE_MSG_PARSER)hThisObject;

    if (pMyObject->hHfpIf)
    {
        PHTTP_HFP_INTERFACE         pHfpIf;

        pHfpIf = (PHTTP_HFP_INTERFACE)(pMyObject->hHfpIf);
        AnscFreeMemory(pHfpIf);
    }

    pMyObject->RemoveXTable((ANSC_HANDLE)pMyObject);

    AnscCoRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSmpoEnrollObjects
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
HttpSmpoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_MSG_PARSER         pMyObject    = (PHTTP_SIMPLE_MSG_PARSER)hThisObject;

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSmpoInitialize
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
HttpSmpoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_SIMPLE_MSG_PARSER         pMyObject     = (PHTTP_SIMPLE_MSG_PARSER)hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf;
    ULONG                           i;
    errno_t                         rc            = -1;

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

    pMyObject->Oid                         = HTTP_SMPO_COMPONENT_OID;
    pMyObject->Create                      = HttpSmpoCreate;
    pMyObject->Remove                      = HttpSmpoRemove;
    pMyObject->EnrollObjects               = HttpSmpoEnrollObjects;
    pMyObject->Initialize                  = HttpSmpoInitialize;

    pMyObject->ParseHeader                 = HttpSmpoParseHeader;
    pMyObject->ParseRequestLine            = HttpSmpoParseRequestLine;
    pMyObject->ParseStatusLine             = HttpSmpoParseStatusLine;
    pMyObject->ParseChunkedLine            = HttpSmpoParseChunkedLine;
    pMyObject->GetHeaderSize               = HttpSmpoGetHeaderSize;
    pMyObject->GetRequestLineSize          = HttpSmpoGetRequestLineSize;
    pMyObject->GetStatusLineSize           = HttpSmpoGetStatusLineSize;
    pMyObject->BuildHeader                 = HttpSmpoBuildHeader;
    pMyObject->BuildRequestLine            = HttpSmpoBuildRequestLine;
    pMyObject->BuildStatusLine             = HttpSmpoBuildStatusLine;

    pMyObject->GetHfpIf                    = HttpSmpoGetHfpIf;

    pMyObject->RegisterHeaderXInfo         = HttpSmpoRegisterHeaderXInfo;
    pMyObject->RemoveXTable                = HttpSmpoRemoveXTable;
    pMyObject->RegisterAllHeadersXInfo     = HttpSmpoRegisterAllHeadersXInfo;
    pMyObject->GetHeaderXInfo              = HttpSmpoGetHeaderXInfo;


    pMyObject->hHfpIf                      = NULL;
    pHfpIf = (PHTTP_HFP_INTERFACE)AnscAllocateMemory(sizeof(HTTP_HFP_INTERFACE));

    if (pHfpIf)
    {
        /* intialize the data HFP_interface */
        pHfpIf->ParseHeader                     = HttpSmpoParseHeader;
        pHfpIf->ParseRequestLine                = HttpSmpoParseRequestLine;
        pHfpIf->ParseStatusLine                 = HttpSmpoParseStatusLine;
        pHfpIf->ParseChunkedLine                = HttpSmpoParseChunkedLine;
        pHfpIf->ParseHttpUrl                    = HttpSmpoParseUrl;
        pHfpIf->ParseHttpUrlList                = HttpSmpoParseUrlList;
        pHfpIf->ParseQueryString                = HttpSmpoParseQueryString;
        pHfpIf->ParseHtmlFormInput              = HttpSmpoParseHtmlFormInput;
        pHfpIf->SaveFormPartAsFile              = HttpSmpoSaveFormPartAsFile;
        pHfpIf->GetFormDataNoFile               = HttpSmpoGetFormDataNoFile;
        pHfpIf->GetHeaderParam                  = HttpSmpoGetHeaderParam;
        pHfpIf->GetHeaderSize                   = HttpSmpoGetHeaderSize;
        pHfpIf->GetRequestLineSize              = HttpSmpoGetRequestLineSize;
        pHfpIf->GetStatusLineSize               = HttpSmpoGetStatusLineSize;
        pHfpIf->BuildHeader                     = HttpSmpoBuildHeader;
        pHfpIf->BuildRequestLine                = HttpSmpoBuildRequestLine;
        pHfpIf->BuildStatusLine                 = HttpSmpoBuildStatusLine;

        /* set the owner context and size of the structure */
        rc = strcpy_s(pHfpIf->Name, sizeof(pHfpIf->Name), HTTP_HFP_INTERFACE_NAME);
        ERR_CHK(rc);
        pHfpIf->hOwnerContext                   = hThisObject;
        pHfpIf->Size                            = sizeof(HTTP_HFP_INTERFACE);

        pMyObject->hHfpIf                       = (ANSC_HANDLE)pHfpIf;
    }

    for (i = 0; i < HTTP_SMPO_HEADER_TABLE_SIZE; i ++)
    {
        AnscInitializeSList(&pMyObject->HeaderXList[i]);
    }

    for (i = 0; i < HTTP_SMPO_MEDIA_TABLE_SIZE; i ++)
    {
        AnscInitializeSList(&pMyObject->MediaSubtypeList[i]);
    }

    pMyObject->RegisterAllHeadersXInfo((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


