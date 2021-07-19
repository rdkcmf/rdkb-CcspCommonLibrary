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

    module:	http_tro_states.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Http Trans Record Object.

        *   HttpTroGetWamIf
        *   HttpTroSetWamIf
        *   HttpTroGetSbcIf
        *   HttpTroSetSbcIf
        *   HttpTroGetCbcIf
        *   HttpTroSetCbcIf
        *   HttpTroGetPbcIf
        *   HttpTroSetPbcIf
        *   HttpTroGetHfpIf
        *   HttpTroSetHfpIf
        *   HttpTroGetBmoReq
        *   HttpTroSetBmoReq
        *   HttpTroGetBmoRep
        *   HttpTroSetBmoRep
        *   HttpTroGetClientSocket
        *   HttpTroSetClientSocket
        *   HttpTroGetServerSocket
        *   HttpTroSetServerSocket
        *   HttpTroGetSbcContext
        *   HttpTroSetSbcContext
        *   HttpTroGetCbcContext
        *   HttpTroSetCbcContext
        *   HttpTroGetServerName
        *   HttpTroSetServerName
        *   HttpTroGetClientAddr
        *   HttpTroGetServerAddr
        *   HttpTroGetClientPort
        *   HttpTroGetServerPort
        *   HttpTroGetSbcPmode
        *   HttpTroSetSbcPmode
        *   HttpTroGetCbcPmode
        *   HttpTroSetCbcPmode
        *   HttpTroGetTransState
        *   HttpTroSetTransState
        *   HttpTroReturn
        *   HttpTroReset

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/05/02    initial revision.

**********************************************************************/


#include "http_tro_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpTroGetWamIf
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

ANSC_HANDLE
HttpTroGetWamIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject    = (PHTTP_TRANS_RECORD_OBJECT)hThisObject;

    return  pMyObject->hWamIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpTroSetWamIf
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hInterface
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hInterface
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpTroSetWamIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject    = (PHTTP_TRANS_RECORD_OBJECT)hThisObject;

    pMyObject->hWamIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpTroGetSbcIf
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

ANSC_HANDLE
HttpTroGetSbcIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject    = (PHTTP_TRANS_RECORD_OBJECT)hThisObject;

    return  pMyObject->hSbcIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpTroSetSbcIf
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hInterface
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hInterface
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpTroSetSbcIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject    = (PHTTP_TRANS_RECORD_OBJECT)hThisObject;

    pMyObject->hSbcIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpTroGetCbcIf
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

ANSC_HANDLE
HttpTroGetCbcIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject    = (PHTTP_TRANS_RECORD_OBJECT)hThisObject;

    return  pMyObject->hCbcIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpTroSetCbcIf
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hInterface
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hInterface
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpTroSetCbcIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject    = (PHTTP_TRANS_RECORD_OBJECT)hThisObject;

    pMyObject->hCbcIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpTroGetPbcIf
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

ANSC_HANDLE
HttpTroGetPbcIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject    = (PHTTP_TRANS_RECORD_OBJECT)hThisObject;

    return  pMyObject->hPbcIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpTroSetPbcIf
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hInterface
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hInterface
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpTroSetPbcIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject    = (PHTTP_TRANS_RECORD_OBJECT)hThisObject;

    pMyObject->hPbcIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpTroGetHfpIf
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

ANSC_HANDLE
HttpTroGetHfpIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject    = (PHTTP_TRANS_RECORD_OBJECT)hThisObject;

    return  pMyObject->hHfpIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpTroSetHfpIf
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hInterface
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hInterface
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpTroSetHfpIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject    = (PHTTP_TRANS_RECORD_OBJECT)hThisObject;

    pMyObject->hHfpIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpTroGetBmoReq
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

ANSC_HANDLE
HttpTroGetBmoReq
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject    = (PHTTP_TRANS_RECORD_OBJECT)hThisObject;

    return  pMyObject->hBmoReq;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpTroSetBmoReq
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hMessage
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hMessage
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpTroSetBmoReq
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject    = (PHTTP_TRANS_RECORD_OBJECT)hThisObject;

    pMyObject->hBmoReq = hMessage;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpTroGetBmoRep
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

ANSC_HANDLE
HttpTroGetBmoRep
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject    = (PHTTP_TRANS_RECORD_OBJECT)hThisObject;

    return  pMyObject->hBmoRep;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpTroSetBmoRep
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hMessage
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hMessage
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpTroSetBmoRep
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject    = (PHTTP_TRANS_RECORD_OBJECT)hThisObject;

    pMyObject->hBmoRep = hMessage;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpTroGetClientSocket
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

ANSC_HANDLE
HttpTroGetClientSocket
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject    = (PHTTP_TRANS_RECORD_OBJECT)hThisObject;

    return  pMyObject->hClientSocket;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpTroSetClientSocket
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpTroSetClientSocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject    = (PHTTP_TRANS_RECORD_OBJECT)hThisObject;

    pMyObject->hClientSocket = hSocket;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpTroGetServerSocket
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

ANSC_HANDLE
HttpTroGetServerSocket
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject    = (PHTTP_TRANS_RECORD_OBJECT)hThisObject;

    return  pMyObject->hServerSocket;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpTroSetServerSocket
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpTroSetServerSocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject    = (PHTTP_TRANS_RECORD_OBJECT)hThisObject;

    pMyObject->hServerSocket = hSocket;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpTroGetSbcContext
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

ANSC_HANDLE
HttpTroGetSbcContext
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject    = (PHTTP_TRANS_RECORD_OBJECT)hThisObject;

    return  pMyObject->hSbcContext;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpTroSetSbcContext
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hContext
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hContext
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpTroSetSbcContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject    = (PHTTP_TRANS_RECORD_OBJECT)hThisObject;

    pMyObject->hSbcContext = hContext;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpTroGetCbcContext
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

ANSC_HANDLE
HttpTroGetCbcContext
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject    = (PHTTP_TRANS_RECORD_OBJECT)hThisObject;

    return  pMyObject->hCbcContext;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpTroSetCbcContext
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hContext
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hContext
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpTroSetCbcContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject    = (PHTTP_TRANS_RECORD_OBJECT)hThisObject;

    pMyObject->hCbcContext = hContext;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        HttpTroGetServerName
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

char*
HttpTroGetServerName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject     = (PHTTP_TRANS_RECORD_OBJECT     )hThisObject;
    
    return  pMyObject->ServerName;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpTroSetServerName
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpTroSetServerName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject    = (PHTTP_TRANS_RECORD_OBJECT)hThisObject;
    errno_t                         rc           = -1;

    AnscZeroMemory(pMyObject->ServerName, ANSC_DOMAIN_NAME_SIZE);
    rc = strcpy_s(pMyObject->ServerName, sizeof(pMyObject->ServerName), name);
    ERR_CHK(rc);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        PUCHAR
        HttpTroGetClientAddr
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

PUCHAR
HttpTroGetClientAddr
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject     = (PHTTP_TRANS_RECORD_OBJECT     )hThisObject;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pClientSocket = (PANSC_DAEMON_SOCKET_TCP_OBJECT)pMyObject->hClientSocket;
    
    if ( !pClientSocket )
    {
        return  NULL;
    }

    return  pClientSocket->GetPeerAddress((ANSC_HANDLE)pClientSocket);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        PUCHAR
        HttpTroGetServerAddr
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

PUCHAR
HttpTroGetServerAddr
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject     = (PHTTP_TRANS_RECORD_OBJECT     )hThisObject;
    
    return  pMyObject->ServerAddr.Dot;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        USHORT
        HttpTroGetClientPort
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

USHORT
HttpTroGetClientPort
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject     = (PHTTP_TRANS_RECORD_OBJECT     )hThisObject;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pClientSocket = (PANSC_DAEMON_SOCKET_TCP_OBJECT)pMyObject->hClientSocket;
    
   if ( !pClientSocket )
    {
        return  0;
    }

    return  pClientSocket->GetPeerPort((ANSC_HANDLE)pClientSocket);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        USHORT
        HttpTroGetServerPort
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

USHORT
HttpTroGetServerPort
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject     = (PHTTP_TRANS_RECORD_OBJECT     )hThisObject;
    
    return  pMyObject->ServerPort;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpTroGetSbcPmode
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

ULONG
HttpTroGetSbcPmode
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject    = (PHTTP_TRANS_RECORD_OBJECT)hThisObject;

    return  pMyObject->SbcPmode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpTroSetSbcPmode
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulPmode
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulPmode
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpTroSetSbcPmode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulPmode
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject    = (PHTTP_TRANS_RECORD_OBJECT)hThisObject;

    pMyObject->SbcPmode = ulPmode;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpTroGetCbcPmode
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

ULONG
HttpTroGetCbcPmode
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject    = (PHTTP_TRANS_RECORD_OBJECT)hThisObject;

    return  pMyObject->CbcPmode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpTroSetCbcPmode
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulPmode
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulPmode
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpTroSetCbcPmode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulPmode
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject    = (PHTTP_TRANS_RECORD_OBJECT)hThisObject;

    pMyObject->CbcPmode = ulPmode;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpTroGetTransState
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

ULONG
HttpTroGetTransState
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject    = (PHTTP_TRANS_RECORD_OBJECT)hThisObject;

    return  pMyObject->TransState;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpTroSetTransState
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulState
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulState
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpTroSetTransState
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulState
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject    = (PHTTP_TRANS_RECORD_OBJECT)hThisObject;

    pMyObject->TransState = ulState;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpTroReturn
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to return object back to the global
        object pool.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpTroReturn
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PHTTP_TRANS_RECORD_OBJECT       pMyObject      = (PHTTP_TRANS_RECORD_OBJECT  )hThisObject;
    PHTTP_ADVANCED_PROXY_OBJECT     pAdvancedProxy = (PHTTP_ADVANCED_PROXY_OBJECT)pMyObject->hOwnerContext;

    returnStatus =
        pAdvancedProxy->ReleaseTro
            (
                (ANSC_HANDLE)pAdvancedProxy,
                (ANSC_HANDLE)pMyObject
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpTroReset
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to reset object states.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpTroReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_TRANS_RECORD_OBJECT       pMyObject    = (PHTTP_TRANS_RECORD_OBJECT)hThisObject;

    pMyObject->hClientSocket    = (ANSC_HANDLE)NULL;
    pMyObject->hServerSocket    = (ANSC_HANDLE)NULL;
    pMyObject->hSbcContext      = (ANSC_HANDLE)NULL;
    pMyObject->hCbcContext      = (ANSC_HANDLE)NULL;
    pMyObject->HashIndex        = 0;
    pMyObject->SbcPmode         = HTTP_SBC_PMODE_RELAY2;
    pMyObject->CbcPmode         = HTTP_CBC_PMODE_RELAY2;
    pMyObject->TransState       = HTTP_TRO_STATE_INITIALIZED;
    pMyObject->bCloseConnection = TRUE;

    return  ANSC_STATUS_SUCCESS;
}
