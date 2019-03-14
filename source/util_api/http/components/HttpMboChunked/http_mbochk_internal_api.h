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

    module:	http_mbochk_internal_api.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Http Mbo Chunked
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/01/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_MBOCHK_INTERNAL_API_
#define  _HTTP_MBOCHK_INTERNAL_API_


/***********************************************************
        FUNCTIONS IMPLEMENTED IN HTTP_MBOCHK_STATES.C
***********************************************************/

ANSC_STATUS
HttpMboChkReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN HTTP_MBOCHK_ACCESS.C
***********************************************************/

ULONG
HttpMboChkGetEntitySize
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
HttpMboChkGetArrivedSize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpMboChkAppendBodyBdo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo
    );

ULONG
HttpMboChkGetExpectedSize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpMboChkAppendChunkData
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo,
        BOOL                        bHasCrlf
    );

ANSC_STATUS
HttpMboChkCopyBodyFrom
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        PULONG                      pulSize
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN HTTP_MBOCHK_PROCESS.C
***********************************************************/

ULONG
HttpMboChkExamine
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpMboChkProcess
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo
    );

ANSC_STATUS
HttpMboChkCloseUp
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo
    );

ANSC_STATUS
HttpMboChkRemoveCoding
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpMboChkProcessChunkData
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo
    );

ANSC_STATUS
HttpMboChkProcessTrailer
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo,
        PBOOL                       pbBccIfNotified
    );


/***********************************************************
      FUNCTIONS IMPLEMENTED IN HTTP_BMOCHK_MANAGEMENT.C
***********************************************************/

ANSC_STATUS
HttpMboChkClearChunkInfos
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
HttpMboChkGetCurChunkInfo
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpMboChkAddChunkInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hChunkInfo,
        char*                       pRawChunkLine,
        ULONG                       ulLineSize
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN HTTP_BMOCHK_TRAILER.C
***********************************************************/

ANSC_STATUS
HttpMboChkAppendTrailer
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRawChunkLine,
        ULONG                       ulLineSize
    );

ANSC_STATUS
HttpMboChkParseTrailer
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRawChunkLine,
        ULONG                       ulLineSize
    );


#endif
