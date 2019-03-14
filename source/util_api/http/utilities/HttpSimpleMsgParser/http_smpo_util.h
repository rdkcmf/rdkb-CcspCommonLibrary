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

    module:	http_smpo_util.h

        For HTTP Simple Message Parser (syntax parser),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the HTTP message parser
        which are called for generating and constructing HTTP 
        message.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        02/24/02    initial revision.

    ---------------------------------------------------------------

    last modified:  
    
        02/24/02

**********************************************************************/

#ifndef  _HTTP_SMPO_UTIL
#define  _HTTP_SMPO_UTIL


/*
 *  Define the following constants to be used when finding multipart
 *  boundary string in a list of BDOs. One thing we should take care
 *  is --- the boundary character might be located cross the BDO's
 *  boundary. So, at least 2 BDOs have to be combined together to
 *  search for boundary. 
 */
#define  HTTP_SMPO_BODY_MSG_BDO_MAX_SIZE            1460
#define  HTTP_SMPO_BODY_MSG_MAX_SIZE                4 * HTTP_SMPO_BODY_MSG_BDO_MAX_SIZE

/* --------------------------------------------------------------------
 *                      Parsing APIs
 * ----------------------------------------------------------------- */

ANSC_STATUS
HttpSmpoLocateFieldData
    (
        ANSC_HANDLE                 hHttpMbo, 
        char*                       param_name,
        char*                       pBoundary,
        ANSC_HANDLE                 *phStartField,
        PULONG                      pulFieldOffset,
        ANSC_HANDLE                 *phStartBdo,
        PULONG                      pulStartOffset,
        ANSC_HANDLE                 *phEndBdo,
        PULONG                      pulEndOffset
    );

ANSC_STATUS
HttpSmpoSaveFieldData
    (
        ANSC_HANDLE                 hThisObject,
        char*                       file_name,
        BOOL                        bFlushBody,
        ANSC_HANDLE                 hStartField,
        ULONG                       ulFieldOffset,
        ANSC_HANDLE                 hStartBdo,
        ULONG                       ulStartOffset,
        ANSC_HANDLE                 hEndBdo,
        ULONG                       ulEndOffset
    );

BOOL
HttpSmpoFindNextBoundary
    (
        ANSC_HANDLE                 hHttpMbo,
        ANSC_HANDLE                 hBdo,
        ULONG                       ulOffset,
        char*                       pBoundary,
        ANSC_HANDLE                 *phBdo,
        PULONG                      pulOffset
    );

BOOL
HttpSmpoGetFormField
    (
        ANSC_HANDLE                 hHttpMbo,
        char*                       pFieldName,
        ANSC_HANDLE                 hBdo,
        ULONG                       ulOffset,
        char*                       pBoundary,
        ANSC_HANDLE                 *phBdo,
        PULONG                      pulOffset
    );

PUCHAR
HttpSmpoCopyFormData
    (
        PUCHAR                      pBuf,
        PULONG                      pulBufSize,
        PULONG                      pulDataLen,
        PUCHAR                      pData,
        ULONG                       ulDataLen
    );

ANSC_STATUS
HttpSmpoCopyFormData2
    (
        ANSC_HANDLE                 hHttpMbo,
        PUCHAR*                     ppBuf,
        PULONG                      pulBufSize,
        PULONG                      pulDataLen,
        char*                       pBoundary,
        ANSC_HANDLE                 hStartBdo,
        ULONG                       ulStartOffset,
        ANSC_HANDLE                 hEndBdo,
        ULONG                       ulEndOffset
    );

#endif
