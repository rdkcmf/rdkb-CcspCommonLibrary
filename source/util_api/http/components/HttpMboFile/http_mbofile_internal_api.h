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

    module:	http_mbofile_internal_api.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Http Mbo File
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/18/05    initial revision.

**********************************************************************/


#ifndef  _HTTP_MBOFILE_INTERNAL_API_
#define  _HTTP_MBOFILE_INTERNAL_API_


/***********************************************************
        FUNCTIONS IMPLEMENTED IN HTTP_MBOFILE_ACCESS.C
***********************************************************/

ULONG
HttpMboFileGetTransferSize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpMboFileAppendBodyBdo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo
    );

ANSC_STATUS
HttpMboFileAppendBodyFile
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pFileName,
        int                         iRangeFrom,
        int                         iRangeTo
    );

ANSC_STATUS
HttpMboFileClearBody
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpMboFileCopyBodyFrom
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        PULONG                      pulSize
    );

ANSC_STATUS
HttpMboFileCopyBodyInto
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        PULONG                      pulSize
    );

ANSC_STATUS
HttpMboFileSaveAsBody
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpMboFileSaveBodyAsFile
    (
        ANSC_HANDLE                 hThisObject,
        char*                       file_name,
        BOOL                        bFlushBody
    );

ANSC_STATUS
HttpMboFileSaveFormAsFile
    (
        ANSC_HANDLE                 hThisObject,
        char*                       param_name,
        char*                       file_name,
        BOOL                        bFlushBody
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN HTTP_MBOFILE_PROCESS.C
***********************************************************/

ANSC_STATUS
HttpMboFileOutput
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSerializeContext
    );


#endif
