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

    module:	http_bmo_internal_api.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Http Basic Message
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/26/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_BMO_INTERNAL_API_
#define  _HTTP_BMO_INTERNAL_API_


/***********************************************************
          FUNCTIONS IMPLEMENTED IN HTTP_BMO_STATES.C
***********************************************************/

ANSC_HANDLE
HttpBmoGetHfpIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpBmoSetHfpIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
HttpBmoGetTmhIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpBmoSetTmhIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
HttpBmoGetFumIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpBmoSetFumIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
HttpBmoGetRecvContext
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpBmoSetRecvContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

ANSC_HANDLE
HttpBmoGetSendContext
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpBmoSetSendContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

ANSC_HANDLE
HttpBmoGetHttpWebsTrans
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpBmoSetHttpWebsTrans
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWebsTrans
    );

ANSC_HANDLE
HttpBmoGetWebServer
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpBmoSetWebServer
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hServer
    );

ANSC_HANDLE
HttpBmoGetWebHost
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpBmoSetWebHost
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHost
    );

ANSC_HANDLE
HttpBmoGetWebServlet
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpBmoSetWebServlet
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hServlet
    );

ULONG
HttpBmoGetState
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpBmoSetState
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulState
    );

ULONG
HttpBmoGetMboMode
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpBmoSetMboMode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    );

ULONG
HttpBmoGetTransactionId
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpBmoSetTransactionId
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       id
    );

ULONG
HttpBmoGetWebSessionId
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpBmoSetWebSessionId
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       id
    );

ANSC_STATUS
HttpBmoReturn
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN HTTP_BMO_ACCESS.C
***********************************************************/

ULONG
HttpBmoGetStartLineSize
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
HttpBmoGetHeadersSize
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
HttpBmoPredictBodySize
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
HttpBmoGetBodySize
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
HttpBmoGetBodySize2
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
HttpBmoGetMessageSize
    (
        ANSC_HANDLE                 hThisObject
    );

BOOL
HttpBmoGetPtmForHeaders
    (
        ANSC_HANDLE                 hThisObject
    );

BOOL
HttpBmoGetPtmForBody
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
HttpBmoGetHeaderBdo
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpBmoSetHeaderBdo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo
    );

BOOL
HttpBmoIsProxyKeepAlive
    (
        ANSC_HANDLE                 hThisObject
    );

BOOL
HttpBmoIsCloseConnection
    (
        ANSC_HANDLE                 hThisObject
    );

BOOL
HttpBmoIsConnKeepAlive
    (
        ANSC_HANDLE                 hThisObject
    );

BOOL
HttpBmoIsChunkedCoding
    (
        ANSC_HANDLE                 hThisObject
    );

BOOL
HttpBmoIsHttpVersion11
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN HTTP_BMO_HEADER.C
***********************************************************/

char*
HttpBmoGetHeaderValueById
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulHeaderId
    );

char*
HttpBmoGetHeaderValueById2
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulHeaderId,
        ULONG                       ulIndex
    );

char*
HttpBmoGetHeaderValueByName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

char*
HttpBmoGetHeaderValueByName2
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        ULONG                       ulIndex
    );

ANSC_STATUS
HttpBmoSetHeaderValueByName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        char*                       value
    );

ANSC_STATUS
HttpBmoDelStartLine
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpBmoParseStartLine
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpBmoClearHeaders
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpBmoParseHeaders
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpBmoCopyStartLineFrom
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        PULONG                      pulSize
    );

ANSC_STATUS
HttpBmoCopyHeadersFrom
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        PULONG                      pulSize
    );

ANSC_HANDLE
HttpBmoGetHeaderField
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulHeaderId
    );

ANSC_HANDLE
HttpBmoGetHeaderField2
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulHeaderId,
        ULONG                       ulIndex
    );

ANSC_STATUS
HttpBmoAddHeaderField
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHfo
    );

ANSC_STATUS
HttpBmoDelHeaderField
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulHeaderId
    );

ANSC_STATUS
HttpBmoDelAllHfos
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
           FUNCTIONS IMPLEMENTED IN HTTP_BMO_BODY.C
***********************************************************/

ANSC_STATUS
HttpBmoClearBody
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpBmoCopyBodyFrom
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        PULONG                      pulSize
    );

ANSC_STATUS
HttpBmoCopyBodyInto
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        PULONG                      pulSize
    );

ANSC_STATUS
HttpBmoSaveAsBody
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpBmoSaveAsMbo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMbo
    );

ANSC_HANDLE
HttpBmoCreateMessageBody
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpBmoAppendBody
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpBmoAppendBodyBdo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo
    );

ANSC_STATUS
HttpBmoAppendBodyFile
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pFileName,
        int                         iRangeFrom,
        int                         iRangeTo
    );

ANSC_STATUS
HttpBmoSaveBodyAsFile
    (
        ANSC_HANDLE                 hThisObject,
        char*                       file_name,
        BOOL                        bFlushBody
    );

ANSC_STATUS
HttpBmoSaveFormAsFile
    (
        ANSC_HANDLE                 hThisObject,
        char*                       param_name,
        char*                       file_name,
        BOOL                        bFlushBody
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN HTTP_BMO_PROCESS.C
***********************************************************/

ULONG
HttpBmoExamine
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpBmoProcess
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo
    );

ANSC_STATUS
HttpBmoCloseUp
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBdo
    );

ANSC_STATUS
HttpBmoProduce
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        PULONG                      pulSize
    );

ANSC_STATUS
HttpBmoOutputHeaders
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSerializeContext
    );

ANSC_STATUS
HttpBmoOutputBody
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSerializeContext
    );

void
HttpBmoSendComplete
    (
        ANSC_HANDLE                 hThisObject
    );

/***********************************************************
           FUNCTIONS IMPLEMENTED IN HTTP_BMO_BCC.C
***********************************************************/

ANSC_HANDLE
HttpBmoBccGetHeaderField
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulHeaderId
    );

ANSC_STATUS
HttpBmoBccAddHeaderField
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHfo
    );

ULONG
HttpBmoBccPredictBodySize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpBmoBccNotify
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
HttpBmoBccSerialize
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hSerializeContext
    );


#endif
