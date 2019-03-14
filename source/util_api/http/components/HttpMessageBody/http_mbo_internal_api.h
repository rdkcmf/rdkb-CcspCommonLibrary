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

    module:	http_mbo_internal_api.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Http Message Body
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


#ifndef  _HTTP_MBO_INTERNAL_API_
#define  _HTTP_MBO_INTERNAL_API_


/***********************************************************
          FUNCTIONS IMPLEMENTED IN HTTP_MBO_STATES.C
***********************************************************/

ANSC_HANDLE
HttpMboGetHfpIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpMboSetHfpIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
HttpMboGetBccIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpMboSetBccIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_STATUS
HttpMboSetFumIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ULONG
HttpMboGetState
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpMboSetState
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulState
    );

ULONG
HttpMboGetMode
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpMboSetMode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulState
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN HTTP_MBO_ACCESS.C
***********************************************************/

ULONG
HttpMboGetBodySize
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
HttpMboGetBodySize2
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
HttpMboGetTransferSize
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
HttpMboGetEntitySize
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
HttpMboGetArrivedSize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpMboAppendBodyBdo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo
    );

ANSC_STATUS
HttpMboAppendBodyFile
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pFileName,
        int                         iRangeFrom,
        int                         iRangeTo
    );

ANSC_STATUS
HttpMboClearBody
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpMboCopyBodyFrom
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        PULONG                      pulSize
    );

ANSC_STATUS
HttpMboCopyBodyInto
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        PULONG                      pulSize
    );

ANSC_STATUS
HttpMboSaveAsBody
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpMboSaveBodyAsFile
    (
        ANSC_HANDLE                 hThisObject,
        char*                       file_name,
        BOOL                        bFlushBody
    );

ANSC_STATUS
HttpMboSaveFormAsFile
    (
        ANSC_HANDLE                 hThisObject,
        char*                       param_name,
        char*                       file_name,
        BOOL                        bFlushBody
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN HTTP_MBO_PROCESS.C
***********************************************************/

ULONG
HttpMboExamine
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpMboProcess
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo
    );

ANSC_STATUS
HttpMboCloseUp
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo
    );

ANSC_STATUS
HttpMboOutput
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSerializeContext
    );

ANSC_STATUS
HttpMboRemoveCoding
    (
        ANSC_HANDLE                 hThisObject
    );

/***********************************************************
         FUNCTIONS IMPLEMENTED IN HTTP_MBO_UTIL.C
***********************************************************/

ANSC_STATUS
HttpMboProcessMPData
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo
    );


#endif
