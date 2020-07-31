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

    module:	ansc_xsink_base.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic construction and removal
        functions of the General Xsink Object.

        *   AnscXsinkCreate
        *   AnscXsinkRemove

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/28/02    initial revision.

**********************************************************************/


#include "ansc_xsocket_global.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscXsinkCreate
            (
                ANSC_HANDLE                 hOwnerContext
            );

    description:

        This function constructs the General Xsink Object and
        initializes the member variables and functions.

    argument:   ANSC_HANDLE                 hOwnerContext
                This handle is passed in by the owner of this object.

    return:     newly created xsocket object.

**********************************************************************/

ANSC_HANDLE
AnscXsinkCreate
    (
        ANSC_HANDLE                 hOwnerContext
    )
{
    PANSC_XSINK_OBJECT              pMyObject    = NULL;

    pMyObject = (PANSC_XSINK_OBJECT)AnscAllocateMemory(sizeof(ANSC_XSINK_OBJECT));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    pMyObject->hOwnerContext  = hOwnerContext;
    pMyObject->hXsocketObject = (ANSC_HANDLE)NULL;

    pMyObject->Create         = AnscXsinkCreate;
    pMyObject->Remove         = AnscXsinkRemove;

    pMyObject->GetXsocket     = AnscXsinkGetXsocket;
    pMyObject->SetXsocket     = AnscXsinkSetXsocket;
    pMyObject->Attach         = AnscXsinkAttach;
    pMyObject->Detach         = AnscXsinkDetach;

    pMyObject->GetRecvBuffer  = AnscXsinkGetRecvBuffer;
    pMyObject->Accept         = AnscXsinkAccept;
    pMyObject->Recv           = AnscXsinkRecv;
    pMyObject->Close          = AnscXsinkClose;
    pMyObject->Abort          = AnscXsinkAbort;

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXsinkRemove
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function destroys the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXsinkRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_XSINK_OBJECT              pMyObject    = (PANSC_XSINK_OBJECT)hThisObject;

    AnscFreeMemory(pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
