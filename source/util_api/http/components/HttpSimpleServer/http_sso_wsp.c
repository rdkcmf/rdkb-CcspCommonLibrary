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

    module:	http_sso_wsp.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2002
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This module implements the advanced interface functions
        of the Http Simple Server Object.

        *   HttpSsoWspAccept
        *   HttpSsoWspServe

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/08/02    initial revision.

**********************************************************************/


#include "http_sso_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        HttpSsoWspAccept
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hClientInfo
            );

    description:

        This function is called to control the server's behavior.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hClientInfo
                Specifies the parameter for WSP object to process.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
HttpSsoWspAccept
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hClientInfo
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_SERVER_OBJECT      pMyObject      = (PHTTP_SIMPLE_SERVER_OBJECT  )hThisObject;
    PHTTP_SIMPLE_SERVER_PROPERTY    pProperty      = (PHTTP_SIMPLE_SERVER_PROPERTY)&pMyObject->Property;
    PHTTP_WSP_CLIENT_INFO           pWspClientInfo = (PHTTP_WSP_CLIENT_INFO       )hClientInfo;

    return  TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpSsoWspServe
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hWebsSession,
                ANSC_HANDLE                 hWebsTrans,
                ANSC_HANDLE                 hMessage
            );

    description:

        This function is called to control the server's behavior.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hWebsSession
                Specifies the Web Session Object to be processed.

                ANSC_HANDLE                 hWebsTrans
                Specifies the Web Transaction Object to be processed.

                ANSC_HANDLE                 hMessage
                Specifies the Basic Message Object to be processed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpSsoWspServe
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWebsSession,
        ANSC_HANDLE                 hWebsTrans,
        ANSC_HANDLE                 hMessage
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_SIMPLE_SERVER_OBJECT      pMyObject    = (PHTTP_SIMPLE_SERVER_OBJECT  )hThisObject;
    PHTTP_SIMPLE_SERVER_PROPERTY    pProperty    = (PHTTP_SIMPLE_SERVER_PROPERTY)&pMyObject->Property;
    PHTTP_WEBS_SESSION_OBJECT       pWebsSession = (PHTTP_WEBS_SESSION_OBJECT   )hWebsSession;
    PHTTP_WEBS_TRANS_OBJECT         pWebsTrans   = (PHTTP_WEBS_TRANS_OBJECT     )hWebsTrans;
    PHTTP_BMO_REQ_OBJECT            pBmoReq      = (PHTTP_BMO_REQ_OBJECT        )hMessage;

    return  returnStatus;
}
