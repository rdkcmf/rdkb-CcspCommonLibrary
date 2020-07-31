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

    module:	http_bmoreq_exported_api.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the exported functions provided by the Http Bmo Request
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/20/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_BMOREQ_EXPORTED_API_
#define  _HTTP_BMOREQ_EXPORTED_API_


/***********************************************************
       FUNCTIONS IMPLEMENTED IN HTTP_BMOREQ_INTERFACE.C
***********************************************************/

ANSC_HANDLE
HttpCreateBmoReq
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN HTTP_BMOREQ_BASE.C
***********************************************************/

ANSC_HANDLE
HttpBmoReqCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );

ANSC_STATUS
HttpBmoReqRemove
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpBmoReqEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpBmoReqInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

char*
HttpBmoReqCgiGetFormContent
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpBmoReqCgiGetContentType
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pulMediaType,
        PULONG                      pulSubType
    );

char*
HttpBmoReqCgiGetBoundaryDelimiter
    (
        ANSC_HANDLE                 hThisObject
    );

#endif
