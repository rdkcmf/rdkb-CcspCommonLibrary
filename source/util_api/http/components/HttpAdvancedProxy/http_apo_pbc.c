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

    module:	http_apo_pbc.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2002
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This module implements the advanced interface functions
        of the Http Advanced Proxy Object.

        *   HttpApoPbcRecvReqHeaders
        *   HttpApoPbcRecvReqBody
        *   HttpApoPbcReqComplete
        *   HttpApoPbcRecvRepHeaders
        *   HttpApoPbcRecvRepBody
        *   HttpApoPbcRepComplete

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/04/02    initial revision.

**********************************************************************/


#include "http_apo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpApoPbcRecvReqHeaders
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSession,
                ANSC_HANDLE                 hMessage
            );

    description:

        This function is called to control the proxy's behavior.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSession
                Specifies the parameter for SBC object to process.

                ANSC_HANDLE                 hMessage
                Specifies the parameter for SBC object to process.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpApoPbcRecvReqHeaders
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession,
        ANSC_HANDLE                 hMessage
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_ADVANCED_PROXY_OBJECT     pMyObject    = (PHTTP_ADVANCED_PROXY_OBJECT  )hThisObject;
    PHTTP_ADVANCED_PROXY_PROPERTY   pProperty    = (PHTTP_ADVANCED_PROXY_PROPERTY)&pMyObject->Property;
    PHTTP_TRANS_RECORD_OBJECT       pTransRecord = (PHTTP_TRANS_RECORD_OBJECT    )hSession;
    PHTTP_BMO_REQ_OBJECT            pBmoReq      = (PHTTP_BMO_REQ_OBJECT         )hMessage;

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpApoPbcRecvReqBody
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSession,
                ANSC_HANDLE                 hMessage
            );

    description:

        This function is called to control the proxy's behavior.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSession
                Specifies the parameter for SBC object to process.

                ANSC_HANDLE                 hMessage
                Specifies the parameter for SBC object to process.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpApoPbcRecvReqBody
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession,
        ANSC_HANDLE                 hMessage
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_ADVANCED_PROXY_OBJECT     pMyObject    = (PHTTP_ADVANCED_PROXY_OBJECT  )hThisObject;
    PHTTP_ADVANCED_PROXY_PROPERTY   pProperty    = (PHTTP_ADVANCED_PROXY_PROPERTY)&pMyObject->Property;
    PHTTP_TRANS_RECORD_OBJECT       pTransRecord = (PHTTP_TRANS_RECORD_OBJECT    )hSession;
    PHTTP_BMO_REQ_OBJECT            pBmoReq      = (PHTTP_BMO_REQ_OBJECT         )hMessage;

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpApoPbcReqComplete
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSession
            );

    description:

        This function is called to control the proxy's behavior.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSession
                Specifies the parameter for SBC object to process.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpApoPbcReqComplete
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_ADVANCED_PROXY_OBJECT     pMyObject    = (PHTTP_ADVANCED_PROXY_OBJECT  )hThisObject;
    PHTTP_ADVANCED_PROXY_PROPERTY   pProperty    = (PHTTP_ADVANCED_PROXY_PROPERTY)&pMyObject->Property;
    PHTTP_TRANS_RECORD_OBJECT       pTransRecord = (PHTTP_TRANS_RECORD_OBJECT    )hSession;

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpApoPbcRecvRepHeaders
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSession,
                ANSC_HANDLE                 hMessage
            );

    description:

        This function is called to control the proxy's behavior.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSession
                Specifies the parameter for SBC object to process.

                ANSC_HANDLE                 hMessage
                Specifies the parameter for SBC object to process.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpApoPbcRecvRepHeaders
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession,
        ANSC_HANDLE                 hMessage
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_ADVANCED_PROXY_OBJECT     pMyObject    = (PHTTP_ADVANCED_PROXY_OBJECT  )hThisObject;
    PHTTP_ADVANCED_PROXY_PROPERTY   pProperty    = (PHTTP_ADVANCED_PROXY_PROPERTY)&pMyObject->Property;
    PHTTP_TRANS_RECORD_OBJECT       pTransRecord = (PHTTP_TRANS_RECORD_OBJECT    )hSession;
    PHTTP_BMO_REP_OBJECT            pBmoRep      = (PHTTP_BMO_REP_OBJECT         )hMessage;

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpApoPbcRecvRepBody
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSession,
                ANSC_HANDLE                 hMessage
            );

    description:

        This function is called to control the proxy's behavior.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSession
                Specifies the parameter for SBC object to process.

                ANSC_HANDLE                 hMessage
                Specifies the parameter for SBC object to process.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpApoPbcRecvRepBody
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession,
        ANSC_HANDLE                 hMessage
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_ADVANCED_PROXY_OBJECT     pMyObject    = (PHTTP_ADVANCED_PROXY_OBJECT  )hThisObject;
    PHTTP_ADVANCED_PROXY_PROPERTY   pProperty    = (PHTTP_ADVANCED_PROXY_PROPERTY)&pMyObject->Property;
    PHTTP_TRANS_RECORD_OBJECT       pTransRecord = (PHTTP_TRANS_RECORD_OBJECT    )hSession;
    PHTTP_BMO_REP_OBJECT            pBmoRep      = (PHTTP_BMO_REP_OBJECT         )hMessage;

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpApoPbcRepComplete
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSession
            );

    description:

        This function is called to control the proxy's behavior.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSession
                Specifies the parameter for SBC object to process.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpApoPbcRepComplete
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PHTTP_ADVANCED_PROXY_OBJECT     pMyObject    = (PHTTP_ADVANCED_PROXY_OBJECT  )hThisObject;
    PHTTP_ADVANCED_PROXY_PROPERTY   pProperty    = (PHTTP_ADVANCED_PROXY_PROPERTY)&pMyObject->Property;
    PHTTP_TRANS_RECORD_OBJECT       pTransRecord = (PHTTP_TRANS_RECORD_OBJECT    )hSession;

    return  returnStatus;
}
