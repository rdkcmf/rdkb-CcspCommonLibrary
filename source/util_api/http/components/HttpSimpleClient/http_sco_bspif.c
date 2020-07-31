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

    module:	http_sco_bspif.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced interface functions
        of the Http Simple Client Object.

        *   HttpScoBspPolish
        *   HttpScoBspBrowse
        *   HttpScoBspNotify

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        04/21/02    initial revision.

**********************************************************************/


#include "http_sco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpScoBspPolish
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq,
                ANSC_HANDLE                 hReqContext
            );

    description:

        This function is called to polish the client request message
        before sending out.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the request message object to be polished.

                ANSC_HANDLE                 hReqContext
                Specifies the request-specific context handle provided
                by the owner.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpScoBspPolish
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hReqContext
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hBmoReq);
    UNREFERENCED_PARAMETER(hReqContext);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpScoBspBrowse
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq,
                ANSC_HANDLE                 hBmoRep,
                ANSC_HANDLE                 hReqContext
            );

    description:

        This function is called to browse the response received from
        the web server.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the request message object that triggerred
                the response.

                ANSC_HANDLE                 hBmoRep
                Specifies the response message to be processed.

                ANSC_HANDLE                 hReqContext
                Specifies the request-specific context handle provided
                by the owner.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpScoBspBrowse
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hReqContext
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hBmoReq);
    UNREFERENCED_PARAMETER(hBmoRep);
    UNREFERENCED_PARAMETER(hReqContext);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpScoBspNotify
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulEvent,
                ULONG                       ulError,
                ANSC_HANDLE                 hReqContext,
                ANSC_HANDLE                 hBmoReq,
                ANSC_HANDLE                 hBmoRep
            );

    description:

        This function is called to notify the owner when something
        happens unexpectedly.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulEvent
                Specifies the nature of the event to be processed.

                ULONG                       ulError
                Specifies the error code associated with the event.

                ANSC_HANDLE                 hReqContext
                Specifies the request-specific context handle provided
                by the owner.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpScoBspNotify
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulEvent,
        ULONG                       ulError,
        ANSC_HANDLE                 hReqContext,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    )
{
   UNREFERENCED_PARAMETER(hThisObject);
   UNREFERENCED_PARAMETER(ulEvent);
   UNREFERENCED_PARAMETER(ulError);
   UNREFERENCED_PARAMETER(hReqContext); 
   UNREFERENCED_PARAMETER(hBmoReq);
   UNREFERENCED_PARAMETER(hBmoRep);
     
   return  ANSC_STATUS_SUCCESS;
}
