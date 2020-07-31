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

    module:	http_wcto_operation.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Http Webc Trans Object.

        *   HttpWctoIsLastTrans
        *   HttpWctoDoLastTrans
        *   HttpWctoOpen
        *   HttpWctoClose
        *   HttpWctoAcquireAccess
        *   HttpWctoReleaseAccess

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        04/22/02    initial revision.

**********************************************************************/


#include "http_wcto_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        HttpWctoIsLastTrans
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

BOOL
HttpWctoIsLastTrans
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBC_TRANS_OBJECT         pMyObject    = (PHTTP_WEBC_TRANS_OBJECT)hThisObject;

    return  pMyObject->bLastTrans;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        HttpWctoDoLastTrans
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

BOOL
HttpWctoDoLastTrans
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBC_TRANS_OBJECT         pMyObject    = (PHTTP_WEBC_TRANS_OBJECT)hThisObject;
    PHTTP_BMO_REQ_OBJECT            pBmoReq      = (PHTTP_BMO_REQ_OBJECT   )pMyObject->hBmoReq;
    PHTTP_BMO_REP_OBJECT            pBmoRep      = (PHTTP_BMO_REP_OBJECT   )pMyObject->hBmoRep;

    if ( !pBmoReq->IsHttpVersion11((ANSC_HANDLE)pBmoReq) ||
         !pBmoRep->IsHttpVersion11((ANSC_HANDLE)pBmoRep) )
    {
        return  TRUE;
    }
    else if ( pBmoReq->IsCloseConnection((ANSC_HANDLE)pBmoReq) ||
              pBmoRep->IsCloseConnection((ANSC_HANDLE)pBmoRep) )
    {
        return  TRUE;
    }

    return  FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWctoOpen
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to enable session activities.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWctoOpen
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBC_TRANS_OBJECT         pMyObject        = (PHTTP_WEBC_TRANS_OBJECT       )hThisObject;
    PHTTP_SIMPLE_CLIENT_OBJECT      pSimpleClient    = (PHTTP_SIMPLE_CLIENT_OBJECT    )pMyObject->hOwnerContext;
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pTcpSimpleClient = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)pMyObject->hTcpSimpleClient;
    PHTTP_HFP_INTERFACE             pHfpIf           = (PHTTP_HFP_INTERFACE           )pMyObject->hHfpIf;
    PHTTP_TMH_INTERFACE             pTmhIf           = (PHTTP_TMH_INTERFACE           )pMyObject->hTmhIf;
    PHTTP_BMO_REQ_OBJECT            pBmoReq          = (PHTTP_BMO_REQ_OBJECT          )pMyObject->hBmoReq;
    PHTTP_BMO_REP_OBJECT            pBmoRep          = (PHTTP_BMO_REP_OBJECT          )pMyObject->hBmoRep;

    if ( !pBmoReq )
    {
        pBmoReq = (PHTTP_BMO_REQ_OBJECT)pSimpleClient->AcquireBmoReq((ANSC_HANDLE)pSimpleClient);

        if ( !pBmoReq )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hBmoReq = (ANSC_HANDLE)pBmoReq;
        }

        pBmoReq->SetHfpIf        ((ANSC_HANDLE)pBmoReq, (ANSC_HANDLE)pHfpIf              );
        pBmoReq->SetTmhIf        ((ANSC_HANDLE)pBmoReq, (ANSC_HANDLE)pTmhIf              );
        pBmoReq->SetTransactionId((ANSC_HANDLE)pBmoReq, 0                                );
        pBmoReq->SetClientAddr   ((ANSC_HANDLE)pBmoReq, pTcpSimpleClient->HostAddress.Dot);
        pBmoReq->SetClientPort   ((ANSC_HANDLE)pBmoReq, pTcpSimpleClient->HostPort       );
        pBmoReq->SetMboMode      ((ANSC_HANDLE)pBmoReq, HTTP_MBO_MODE_STORE              );
    }

    if ( !pBmoRep )
    {
        pBmoRep = (PHTTP_BMO_REP_OBJECT)pSimpleClient->AcquireBmoRep((ANSC_HANDLE)pSimpleClient);

        if ( !pBmoRep )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hBmoRep = (ANSC_HANDLE)pBmoRep;
        }

        pBmoRep->SetHfpIf        ((ANSC_HANDLE)pBmoRep, (ANSC_HANDLE)pHfpIf);
        pBmoRep->SetTmhIf        ((ANSC_HANDLE)pBmoRep, (ANSC_HANDLE)pTmhIf);
        pBmoRep->SetTransactionId((ANSC_HANDLE)pBmoRep, 0                  );
        pBmoRep->SetMboMode      ((ANSC_HANDLE)pBmoRep, HTTP_MBO_MODE_STORE);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWctoClose
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to disable session activities.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWctoClose
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBC_TRANS_OBJECT         pMyObject        = (PHTTP_WEBC_TRANS_OBJECT       )hThisObject;
    PHTTP_SIMPLE_CLIENT_OBJECT      pSimpleClient    = (PHTTP_SIMPLE_CLIENT_OBJECT    )pMyObject->hOwnerContext;
    PHTTP_BMO_REQ_OBJECT            pBmoReq          = (PHTTP_BMO_REQ_OBJECT          )pMyObject->hBmoReq;
    PHTTP_BMO_REP_OBJECT            pBmoRep          = (PHTTP_BMO_REP_OBJECT          )pMyObject->hBmoRep;

    if ( pBmoReq )
    {
        pMyObject->hBmoReq = (ANSC_HANDLE)NULL;

        pSimpleClient->ReleaseBmoReq
                (
                    (ANSC_HANDLE)pSimpleClient,
                    (ANSC_HANDLE)pBmoReq
                );
    }

    if ( pBmoRep )
    {
        pMyObject->hBmoRep = (ANSC_HANDLE)NULL;

        pSimpleClient->ReleaseBmoRep
                (
                    (ANSC_HANDLE)pSimpleClient,
                    (ANSC_HANDLE)pBmoRep
                );
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWctoAcquireAccess
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to acquire access to the session object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWctoAcquireAccess
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBC_TRANS_OBJECT         pMyObject     = (PHTTP_WEBC_TRANS_OBJECT   )hThisObject;
    
    AnscAcquireLock(&pMyObject->AccessLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpWctoReleaseAccess
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to release access to the session object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpWctoReleaseAccess
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_WEBC_TRANS_OBJECT         pMyObject     = (PHTTP_WEBC_TRANS_OBJECT   )hThisObject;
    AnscReleaseLock(&pMyObject->AccessLock);

    return  ANSC_STATUS_SUCCESS;
}
