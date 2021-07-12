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

    module:	bwsp_sco_wsrif.c

        For Broadway Web Service Plane Implementation (BWSP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced interface functions
        of the Bwsp Site Creator Object.

        *   BwspScoWsrAttachWebHost
        *   BwspScoWsrDetachWebHost
        *   BwspScoWsrAttachServlet
        *   BwspScoWsrDetachServlet

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/02/03    initial revision.

**********************************************************************/


#include "bwsp_sco_global.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwspScoWsrAttachWebHost
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hHostDescr
            );

    description:

        This function is called to attach a portal host object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hHostDescr
                Specifies the descriptor of the portal host to be
                attached.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BwspScoWsrAttachWebHost
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHostDescr
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PBWSP_SITE_CREATOR_OBJECT       pMyObject    = (PBWSP_SITE_CREATOR_OBJECT    )hThisObject;
    PBWSP_SITE_CREATOR_PROPERTY     pProperty    = (PBWSP_SITE_CREATOR_PROPERTY  )&pMyObject->Property;
    PWEB_HEM_INTERFACE              pWebHemIf    = (PWEB_HEM_INTERFACE           )pMyObject->hWebHemIf;
    PHTTP_HELPER_CONTAINER_OBJECT   pHttpHco     = (PHTTP_HELPER_CONTAINER_OBJECT)pWebHemIf->GetHttpHco(pWebHemIf->hOwnerContext);
    PBWSP_WEB_HOST_DESCRIPTOR       pWebHostDesp = (PBWSP_WEB_HOST_DESCRIPTOR    )hHostDescr;
    PBWSP_PORTAL_HOST_OBJECT        pPortalHost  = (PBWSP_PORTAL_HOST_OBJECT     )NULL;
    BWSP_PORTAL_HOST_PROPERTY       bwspPhoProperty;
    WEB_LSM_INTERFACE               webLsmIf;
    errno_t   rc   = -1;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else
    {
        pPortalHost =
            (PBWSP_PORTAL_HOST_OBJECT)BwspCreatePortalHost
                (
                    pHttpHco,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );
    }

    if ( !pPortalHost )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    if ( TRUE )
    {
        AnscZeroMemory(&bwspPhoProperty, sizeof(BWSP_PORTAL_HOST_PROPERTY));

        rc = STRCPY_S_NOCLOBBER(bwspPhoProperty.HostName,  sizeof(bwspPhoProperty.HostName),    pWebHostDesp->HostName     );
        ERR_CHK(rc);
        rc = STRCPY_S_NOCLOBBER(bwspPhoProperty.VhoCookieName,  sizeof(bwspPhoProperty.VhoCookieName), pProperty   ->PhoCookieName);
        ERR_CHK(rc);
        rc = STRCPY_S_NOCLOBBER(bwspPhoProperty.LsmCookieName,  sizeof(bwspPhoProperty.LsmCookieName), pProperty   ->LsmCookieName);
        ERR_CHK(rc);

        bwspPhoProperty.HostPort               = pWebHostDesp->HostPort;
        bwspPhoProperty.bEnableSessionTracking = pWebHostDesp->bSessionTracking;
        bwspPhoProperty.bDefaultVirtualHost    = pWebHostDesp->bDefaultHost;
        bwspPhoProperty.SessionTimeOut         = pProperty   ->SessionTimeout;
        bwspPhoProperty.VhoCookieMaxAge        = pProperty   ->PhoCookieMaxAge;
        bwspPhoProperty.LsmCookieMaxAge        = pProperty   ->LsmCookieMaxAge;

        pPortalHost->SetProperty((ANSC_HANDLE)pPortalHost, (ANSC_HANDLE)&bwspPhoProperty);
    }

    if ( TRUE )
    {
        AnscZeroMemory(&webLsmIf, sizeof(WEB_LSM_INTERFACE));

        rc = STRCPY_S_NOCLOBBER(webLsmIf.Name, sizeof(webLsmIf.Name), WEB_LSM_INTERFACE_NAME);
        ERR_CHK(rc);

        webLsmIf.InterfaceId    = WEB_LSM_INTERFACE_ID;
        webLsmIf.hOwnerContext  = pWebHostDesp->hHostContext;
        webLsmIf.Size           = sizeof(WEB_LSM_INTERFACE);

        webLsmIf.NewContact     = (PFN_WEBLSMIF_NEW     )pWebHostDesp->NewContact;
        webLsmIf.ClassifyClient = (PFN_WEBLSMIF_CLASSIFY)pWebHostDesp->ClassifyClient;
        webLsmIf.ExpireSession  = (PFN_WEBLSMIF_EXPIRE  )pWebHostDesp->ExpireSession;
        webLsmIf.EndSession     = (PFN_WEBLSMIF_END     )pWebHostDesp->EndSession;

        pPortalHost->SetLsmIf((ANSC_HANDLE)pPortalHost, (ANSC_HANDLE)&webLsmIf);
    }

    if ( TRUE )
    {
        pPortalHost->Engage((ANSC_HANDLE)pPortalHost);
    }

    returnStatus =
        pWebHemIf->AttachWebVho
            (
                pWebHemIf->hOwnerContext,
                (ANSC_HANDLE)pPortalHost
            );

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        pPortalHost->Cancel((ANSC_HANDLE)pPortalHost);
        pPortalHost->Remove((ANSC_HANDLE)pPortalHost);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwspScoWsrDetachWebHost
            (
                ANSC_HANDLE                 hThisObject,
                char*                       host_name
            );

    description:

        This function is called to detach a portal host object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       host_name
                Specifies the name of the portal host to be detached.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BwspScoWsrDetachWebHost
    (
        ANSC_HANDLE                 hThisObject,
        char*                       host_name
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PBWSP_SITE_CREATOR_OBJECT       pMyObject    = (PBWSP_SITE_CREATOR_OBJECT    )hThisObject;
    PWEB_HEM_INTERFACE              pWebHemIf    = (PWEB_HEM_INTERFACE           )pMyObject->hWebHemIf;

    returnStatus =
        pWebHemIf->DeleteWebVho
            (
                pWebHemIf->hOwnerContext,
                host_name
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwspScoWsrAttachServlet
            (
                ANSC_HANDLE                 hThisObject,
                char*                       host_name,
                ANSC_HANDLE                 hServletDescr
            );

    description:

        This function is called to attach a web servlet object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       host_name
                Specifies the name of the portal host to which the web
                servlet is to be attached.

                ANSC_HANDLE                 hServletDescr
                Specifies the descriptor of the web servlet to be
                attached.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BwspScoWsrAttachServlet
    (
        ANSC_HANDLE                 hThisObject,
        char*                       host_name,
        ANSC_HANDLE                 hServletDescr
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PBWSP_SITE_CREATOR_OBJECT       pMyObject    = (PBWSP_SITE_CREATOR_OBJECT    )hThisObject;
    PWEB_HEM_INTERFACE              pWebHemIf    = (PWEB_HEM_INTERFACE           )pMyObject->hWebHemIf;
    PHTTP_HELPER_CONTAINER_OBJECT   pHttpHco     = (PHTTP_HELPER_CONTAINER_OBJECT)pWebHemIf->GetHttpHco(pWebHemIf->hOwnerContext);
    PBWSP_SERVLET_DESCRIPTOR        pServletDesp = (PBWSP_SERVLET_DESCRIPTOR     )hServletDescr;
    PBWSP_WEB_SERVLET_OBJECT        pWebServlet  = (PBWSP_WEB_SERVLET_OBJECT     )NULL;
    BWSP_WEB_SERVLET_PROPERTY       bwspWsoProperty;
    BWSP_WSH_INTERFACE              bwspWshIf;
    errno_t   rc   = -1;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else
    {
        pWebServlet =
            (PBWSP_WEB_SERVLET_OBJECT)BwspCreateWebServlet
                (
                    pHttpHco,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );
    }

    if ( !pWebServlet )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    if ( TRUE )
    {
        AnscZeroMemory(&bwspWsoProperty, sizeof(BWSP_WEB_SERVLET_PROPERTY));

        rc = STRCPY_S_NOCLOBBER(bwspWsoProperty.RootPath, sizeof(bwspWsoProperty.RootPath), pServletDesp->WebRootPath);
        ERR_CHK(rc);
        rc = STRCPY_S_NOCLOBBER(bwspWsoProperty.RegPath, sizeof(bwspWsoProperty.RegPath),  pServletDesp->ServletPath);
        ERR_CHK(rc);

        bwspWsoProperty.OwnerFlag  = pServletDesp->bMainPage   ? WEB_ROO_FLAG_STANDARD | WEB_ROO_FLAG_DEFAULT : WEB_ROO_FLAG_STANDARD;
        bwspWsoProperty.OwnerFlag |= pServletDesp->bUpnpSupport? WEB_ROO_FLAG_UPNP_SUPPORT: 0;
        bwspWsoProperty.OwnerType  = WEB_ROO_TYPE_GENERAL_CGI;
        bwspWsoProperty.AuthType   = WEB_ROO_AUTH_TYPE_NONE;

        pWebServlet->SetProperty((ANSC_HANDLE)pWebServlet, (ANSC_HANDLE)&bwspWsoProperty);
    }

    if ( TRUE )
    {
        AnscZeroMemory(&bwspWshIf, sizeof(BWSP_WSH_INTERFACE));

        rc = STRCPY_S_NOCLOBBER(bwspWshIf.Name, sizeof(bwspWshIf.Name), BWSP_WSH_INTERFACE_NAME);
        ERR_CHK(rc);

        bwspWshIf.InterfaceId   = BWSP_WSH_INTERFACE_ID;
        bwspWshIf.hOwnerContext = pServletDesp->hServletContext;
        bwspWshIf.Size          = sizeof(BWSP_WSH_INTERFACE);

        bwspWshIf.DoOptions     = (PFN_BWSPWSHIF_DO_OPTIONS    )pServletDesp->DoOptions;
        bwspWshIf.DoGet         = (PFN_BWSPWSHIF_DO_GET        )pServletDesp->DoGet;
        bwspWshIf.DoHead        = (PFN_BWSPWSHIF_DO_HEAD       )pServletDesp->DoHead;
        bwspWshIf.DoPost        = (PFN_BWSPWSHIF_DO_POST       )pServletDesp->DoPost;
        bwspWshIf.DoPut         = (PFN_BWSPWSHIF_DO_PUT        )pServletDesp->DoPut;
        bwspWshIf.DoDelete      = (PFN_BWSPWSHIF_DO_DELETE     )pServletDesp->DoDelete;
        bwspWshIf.DoTrace       = (PFN_BWSPWSHIF_DO_TRACE      )pServletDesp->DoTrace;
        bwspWshIf.DoConnect     = (PFN_BWSPWSHIF_DO_CONNECT    )pServletDesp->DoConnect;

        bwspWshIf.DoNotify      = (PFN_BWSPWSHIF_DO_NOTIFY     )pServletDesp->DoNotify;
        bwspWshIf.DoSearch      = (PFN_BWSPWSHIF_DO_SEARCH     )pServletDesp->DoSearch;
        bwspWshIf.DoMSearch     = (PFN_BWSPWSHIF_DO_M_SEARCH   )pServletDesp->DoMSearch;
        bwspWshIf.DoMPost       = (PFN_BWSPWSHIF_DO_M_POST     )pServletDesp->DoMPost;
        bwspWshIf.DoSubscribe   = (PFN_BWSPWSHIF_DO_SUBSCRIBE  )pServletDesp->DoSubscribe;
        bwspWshIf.DoUnsubscribe = (PFN_BWSPWSHIF_DO_UNSUBSCRIBE)pServletDesp->DoUnsubscribe;

        pWebServlet->SetBwspWshIf((ANSC_HANDLE)pWebServlet, (ANSC_HANDLE)&bwspWshIf);
    }

    if ( TRUE )
    {
        pWebServlet->Engage((ANSC_HANDLE)pWebServlet);
    }

    returnStatus =
        pWebHemIf->AttachWebRoo
            (
                pWebHemIf->hOwnerContext,
                host_name,
                (ANSC_HANDLE)pWebServlet
            );

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        pWebServlet->Cancel((ANSC_HANDLE)pWebServlet);
        pWebServlet->Remove((ANSC_HANDLE)pWebServlet);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwspScoWsrDetachServlet
            (
                ANSC_HANDLE                 hThisObject,
                char*                       host_name,
                char*                       path_name
            );

    description:

        This function is called to detach a web servlet object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.
                char

                char*                       host_name
                Specifies the name of the portal host to which the web
                servlet is to be detached.

                char*                       path_name
                Specifies the path of the web servlet to be detached.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BwspScoWsrDetachServlet
    (
        ANSC_HANDLE                 hThisObject,
        char*                       host_name,
        char*                       path_name
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PBWSP_SITE_CREATOR_OBJECT       pMyObject    = (PBWSP_SITE_CREATOR_OBJECT    )hThisObject;
    PWEB_HEM_INTERFACE              pWebHemIf    = (PWEB_HEM_INTERFACE           )pMyObject->hWebHemIf;

    returnStatus =
        pWebHemIf->DeleteWebRoo
            (
                pWebHemIf->hOwnerContext,
                host_name,
                path_name
            );

    return  returnStatus;
}
