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

    module:	http_bmoreq_base.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Http Bmo Req Object.

        *   HttpBmoReqCreate
        *   HttpBmoReqRemove
        *   HttpBmoReqEnrollObjects
        *   HttpBmoReqInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/27/02    initial revision.

**********************************************************************/


#include "http_bmoreq_global.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        HttpBmoReqCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Http Bmo Req Object and
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
HttpBmoReqCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PHTTP_BMO_REQ_OBJECT            pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PHTTP_BMO_REQ_OBJECT)AnscAllocateMemory(sizeof(HTTP_BMO_REQ_OBJECT));

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
    /* AnscCopyString(pBaseObject->Name, HTTP_BMO_REQ_NAME); */

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = HTTP_BMO_REQ_OID;
    pBaseObject->Create            = HttpBmoReqCreate;
    pBaseObject->Remove            = HttpBmoReqRemove;
    pBaseObject->EnrollObjects     = HttpBmoReqEnrollObjects;
    pBaseObject->Initialize        = HttpBmoReqInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoReqRemove
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
HttpBmoReqRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT)hThisObject;
    PHTTP_CGI_INTERFACE             pCgiIf       = (PHTTP_CGI_INTERFACE )pMyObject->hCgiIf;
    PHTTP_RCP_INTERFACE             pRcpIf       = (PHTTP_RCP_INTERFACE )pMyObject->hRcpIf;

    pMyObject->Reset((ANSC_HANDLE)pMyObject);

    if ( pCgiIf )
    {
        AnscFreeMemory(pCgiIf);
    }

    if ( pRcpIf )
    {
        AnscFreeMemory(pRcpIf);
    }

    if ( pMyObject->RcpDpad )
    {
        AnscFreeMemory(pMyObject->RcpDpad);
    }

    HttpBmoRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoReqEnrollObjects
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
HttpBmoReqEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT)hThisObject;
    PHTTP_RCP_INTERFACE             pRcpIf       = (PHTTP_RCP_INTERFACE )pMyObject->hRcpIf;

#ifdef _ANSC_HTTP_CGI_USED_
    PHTTP_CGI_INTERFACE             pCgiIf       = (PHTTP_CGI_INTERFACE )pMyObject->hCgiIf;

    if ( !pCgiIf )
    {
        pCgiIf = (PHTTP_CGI_INTERFACE)AnscAllocateMemory(sizeof(HTTP_CGI_INTERFACE));

        if ( !pCgiIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hCgiIf = (ANSC_HANDLE)pCgiIf;
        }

        pCgiIf->hOwnerContext        = (ANSC_HANDLE)pMyObject;
        pCgiIf->Size                 = sizeof(HTTP_CGI_INTERFACE);

        pCgiIf->GetEnvVariableByName = HttpBmoReqCgiGetEnvVariableByName;
        pCgiIf->GetEnvVariableById   = HttpBmoReqCgiGetEnvVariableById;
        pCgiIf->GetAuthType          = HttpBmoReqCgiGetAuthType;
        pCgiIf->GetContentLength     = HttpBmoReqCgiGetContentLength;
        pCgiIf->GetContentType       = HttpBmoReqCgiGetContentType;
        pCgiIf->GetBoundaryDelimiter = HttpBmoReqCgiGetBoundaryDelimiter;
        pCgiIf->GetDocumentRoot      = HttpBmoReqCgiGetDocumentRoot;
        pCgiIf->GetPathInfo          = HttpBmoReqCgiGetPathInfo;
        pCgiIf->GetPathTranslated    = HttpBmoReqCgiGetPathTranslated;
        pCgiIf->GetQueryString       = HttpBmoReqCgiGetQueryString;
        pCgiIf->GetFormContent       = HttpBmoReqCgiGetFormContent;
        pCgiIf->GetRemoteAddr        = HttpBmoReqCgiGetRemoteAddr;
        pCgiIf->GetRemoteHost        = HttpBmoReqCgiGetRemoteHost;
        pCgiIf->GetRemoteUser        = HttpBmoReqCgiGetRemoteUser;
        pCgiIf->GetRequestMethod     = HttpBmoReqCgiGetRequestMethod;
        pCgiIf->GetScriptName        = HttpBmoReqCgiGetScriptName;
        pCgiIf->GetServerName        = HttpBmoReqCgiGetServerName;
        pCgiIf->GetServerPort        = HttpBmoReqCgiGetServerPort;
        pCgiIf->GetServerProtocol    = HttpBmoReqCgiGetServerProtocol;
        pCgiIf->GetServerSoftware    = HttpBmoReqCgiGetServerSoftware;
        pCgiIf->GetServerType        = HttpBmoReqCgiGetServerType;
        pCgiIf->GetHtmlFormInput     = HttpBmoReqCgiGetHtmlFormInput;
        pCgiIf->GetHtmlFormParam     = HttpBmoReqCgiGetHtmlFormParam;
        pCgiIf->GetHtmlFormParam2    = HttpBmoReqCgiGetHtmlFormParam2;
    }
#endif

    if ( !pRcpIf )
    {
        pRcpIf = (PHTTP_RCP_INTERFACE)AnscAllocateMemory(sizeof(HTTP_RCP_INTERFACE));

        if ( !pRcpIf )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hRcpIf = (ANSC_HANDLE)pRcpIf;
        }

        pRcpIf->hOwnerContext        = (ANSC_HANDLE)pMyObject;
        pRcpIf->Size                 = sizeof(HTTP_RCP_INTERFACE);

        pRcpIf->GetDocumentRoot      = HttpBmoReqRcpGetDocumentRoot;
        pRcpIf->GetPathInfo          = HttpBmoReqRcpGetPathInfo;
        pRcpIf->GetPathTranslated    = HttpBmoReqRcpGetPathTranslated;
        pRcpIf->GetRemoteUser        = HttpBmoReqRcpGetRemoteUser;
        pRcpIf->GetScriptName        = HttpBmoReqRcpGetScriptName;
        pRcpIf->GetServerName        = HttpBmoReqRcpGetServerName;
        pRcpIf->GetServerPort        = HttpBmoReqRcpGetServerPort;
        pRcpIf->GetServerProtocol    = HttpBmoReqRcpGetServerProtocol;
        pRcpIf->GetServerSoftware    = HttpBmoReqRcpGetServerSoftware;
    }

    HttpBmoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpBmoReqInitialize
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
HttpBmoReqInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_BMO_REQ_OBJECT            pMyObject    = (PHTTP_BMO_REQ_OBJECT)hThisObject;

    /*
     * Until you have to simulate C++ object-oriented programming style with standard C, you don't
     * appreciate all the nice little things come with C++ language and all the dirty works that
     * have been done by the C++ compilers. Member initialization is one of these things. While in
     * C++ you don't have to initialize all the member fields inherited from the base class since
     * the compiler will do it for you, such is not the case with C.
     */
    HttpBmoInitialize((ANSC_HANDLE)pMyObject);

    /*
     * Although we have initialized some of the member fields in the "create" member function, we
     * repeat the work here for completeness. While this simulation approach is pretty stupid from
     * a C++/Java programmer perspective, it's the best we can get for universal embedded network
     * programming. Before we develop our own operating system (don't expect that to happen any
     * time soon), this is the way things gonna be.
     */
    pMyObject->Oid                     = HTTP_BMO_REQ_OID;
    pMyObject->Create                  = HttpBmoReqCreate;
    pMyObject->Remove                  = HttpBmoReqRemove;
    pMyObject->EnrollObjects           = HttpBmoReqEnrollObjects;
    pMyObject->Initialize              = HttpBmoReqInitialize;

    pMyObject->ClientAddr.Value        = 0;
    pMyObject->ClientPort              = 0;
    pMyObject->DaemonAddr.Value        = 0;
    pMyObject->DaemonPort              = 0;
    pMyObject->ServerMode              = 0;

    pMyObject->RcpDpad                 = NULL;
    pMyObject->RcpIpadSize             = HTTP_BMO_REQ_RCP_IPAD_SIZE;
    pMyObject->RcpSpadSize             = HTTP_BMO_REQ_RCP_SPAD_SIZE;
    pMyObject->RcpMpadSize             = HTTP_BMO_REQ_RCP_MPAD_SIZE;
    pMyObject->RcpBpadSize             = HTTP_BMO_REQ_RCP_BPAD_SIZE;
    pMyObject->RcpDpadSize             = 0;

    pMyObject->GetStartLineSize        = HttpBmoReqGetStartLineSize;
    pMyObject->PredictBodySize         = HttpBmoReqPredictBodySize;
    pMyObject->IsHttpVersion11         = HttpBmoReqIsHttpVersion11;

    pMyObject->DelStartLine            = HttpBmoReqDelStartLine;
    pMyObject->ParseStartLine          = HttpBmoReqParseStartLine;
    pMyObject->CopyStartLineFrom       = HttpBmoReqCopyStartLineFrom;

    pMyObject->Reset                   = HttpBmoReqReset;

    pMyObject->GetClientAddr           = HttpBmoReqGetClientAddr;
    pMyObject->SetClientAddr           = HttpBmoReqSetClientAddr;
    pMyObject->GetClientPort           = HttpBmoReqGetClientPort;
    pMyObject->SetClientPort           = HttpBmoReqSetClientPort;
    pMyObject->GetDaemonAddr           = HttpBmoReqGetDaemonAddr;
    pMyObject->SetDaemonAddr           = HttpBmoReqSetDaemonAddr;
    pMyObject->GetDaemonPort           = HttpBmoReqGetDaemonPort;
    pMyObject->SetDaemonPort           = HttpBmoReqSetDaemonPort;
    pMyObject->GetServerMode           = HttpBmoReqGetServerMode;
    pMyObject->SetServerMode           = HttpBmoReqSetServerMode;
    pMyObject->GetServerName           = HttpBmoReqGetServerName;
    pMyObject->GetServerPort           = HttpBmoReqGetServerPort;

    pMyObject->GetReqInfo              = HttpBmoReqGetReqInfo;
    pMyObject->GetMethod               = HttpBmoReqGetMethod;
    pMyObject->GetMethodName           = HttpBmoReqGetMethodName;
    pMyObject->GetMajorVersion         = HttpBmoReqGetMajorVersion;
    pMyObject->GetMinorVersion         = HttpBmoReqGetMinorVersion;
    pMyObject->GetUri                  = HttpBmoReqGetUri;

    pMyObject->SetReqInfo              = HttpBmoReqSetReqInfo;
    pMyObject->SetMethod               = HttpBmoReqSetMethod;
    pMyObject->SetMajorVersion         = HttpBmoReqSetMajorVersion;
    pMyObject->SetMinorVersion         = HttpBmoReqSetMinorVersion;
    pMyObject->SetUri                  = HttpBmoReqSetUri;

    pMyObject->GetRcpIf                = HttpBmoReqGetRcpIf;
    pMyObject->SetRcpIf                = HttpBmoReqSetRcpIf;

#ifdef _ANSC_HTTP_CGI_USED_
    pMyObject->GetFormContent          = HttpBmoReqGetFormContent;
    pMyObject->GenFormContent          = HttpBmoReqGenFormContent;
    pMyObject->GetMappedPath           = HttpBmoReqGetMappedPath;
    pMyObject->SetMappedPath           = HttpBmoReqSetMappedPath;
    pMyObject->GetOriginalPagePath     = HttpBmoReqGetOriginalPagePath;
    pMyObject->SetOriginalPagePath     = HttpBmoReqSetOriginalPagePath;
    pMyObject->GetFilteredPagePath     = HttpBmoReqGetFilteredPagePath;
    pMyObject->SetFilteredPagePath     = HttpBmoReqSetFilteredPagePath;

    pMyObject->GetCgiIf                = HttpBmoReqGetCgiIf;
    pMyObject->CgiGetEnvVariableByName = HttpBmoReqCgiGetEnvVariableByName;
    pMyObject->CgiGetEnvVariableById   = HttpBmoReqCgiGetEnvVariableById;
    pMyObject->CgiGetAuthType          = HttpBmoReqCgiGetAuthType;
    pMyObject->CgiGetContentLength     = HttpBmoReqCgiGetContentLength;
    pMyObject->CgiGetContentType       = HttpBmoReqCgiGetContentType;
    pMyObject->CgiGetBoundaryDelimiter = HttpBmoReqCgiGetBoundaryDelimiter;
    pMyObject->CgiGetDocumentRoot      = HttpBmoReqCgiGetDocumentRoot;
    pMyObject->CgiGetPathInfo          = HttpBmoReqCgiGetPathInfo;
    pMyObject->CgiGetPathTranslated    = HttpBmoReqCgiGetPathTranslated;
    pMyObject->CgiGetQueryString       = HttpBmoReqCgiGetQueryString;
    pMyObject->CgiGetFormContent       = HttpBmoReqCgiGetFormContent;
    pMyObject->CgiGetRemoteAddr        = HttpBmoReqCgiGetRemoteAddr;
    pMyObject->CgiGetRemoteHost        = HttpBmoReqCgiGetRemoteHost;
    pMyObject->CgiGetRemoteUser        = HttpBmoReqCgiGetRemoteUser;
    pMyObject->CgiGetRequestMethod     = HttpBmoReqCgiGetRequestMethod;
    pMyObject->CgiGetScriptName        = HttpBmoReqCgiGetScriptName;
    pMyObject->CgiGetServerName        = HttpBmoReqCgiGetServerName;
    pMyObject->CgiGetServerPort        = HttpBmoReqCgiGetServerPort;
    pMyObject->CgiGetServerProtocol    = HttpBmoReqCgiGetServerProtocol;
    pMyObject->CgiGetServerSoftware    = HttpBmoReqCgiGetServerSoftware;
    pMyObject->CgiGetServerType        = HttpBmoReqCgiGetServerType;
    pMyObject->CgiGetHtmlFormInput     = HttpBmoReqCgiGetHtmlFormInput;
    pMyObject->CgiGetHtmlFormParam     = HttpBmoReqCgiGetHtmlFormParam;
    pMyObject->CgiGetHtmlFormParam2    = HttpBmoReqCgiGetHtmlFormParam2;
#endif

    pMyObject->RcpGetDocumentRoot      = HttpBmoReqRcpGetDocumentRoot;
    pMyObject->RcpGetPathInfo          = HttpBmoReqRcpGetPathInfo;
    pMyObject->RcpGetPathTranslated    = HttpBmoReqRcpGetPathTranslated;
    pMyObject->RcpGetRemoteUser        = HttpBmoReqRcpGetRemoteUser;
    pMyObject->RcpGetScriptName        = HttpBmoReqRcpGetScriptName;
    pMyObject->RcpGetServerName        = HttpBmoReqRcpGetServerName;
    pMyObject->RcpGetServerPort        = HttpBmoReqRcpGetServerPort;
    pMyObject->RcpGetServerProtocol    = HttpBmoReqRcpGetServerProtocol;
    pMyObject->RcpGetServerSoftware    = HttpBmoReqRcpGetServerSoftware;

    pMyObject->GetAuthInfo             = HttpBmoReqGetAuthInfo;
    pMyObject->GetAuthUser             = HttpBmoReqGetAuthUser;
    pMyObject->GetAuthScheme           = HttpBmoReqGetAuthScheme;
    pMyObject->GetAuthRealm            = HttpBmoReqGetAuthRealm;
    pMyObject->GetAuthDigestUri        = HttpBmoReqGetAuthDigestUri;

    return  ANSC_STATUS_SUCCESS;
}
