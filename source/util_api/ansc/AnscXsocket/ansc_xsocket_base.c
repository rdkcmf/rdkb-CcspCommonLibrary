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

    module:	ansc_xsocket_base.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic construction and removal
        functions of the General Xsocket Object.

        *   AnscXsocketCreate
        *   AnscXsocketRemove
        *   AnscXsocketEnrollObjects
        *   AnscXsocketInitialize
        *   AnscXsocketShutdown

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        04/04/01    initial revision.
        04/02/02    modify the xsocket object interface, to drive it
                    from the AnscCoVer3 object.
        05/28/02    cloned from the basic ansc object.

**********************************************************************/


#include "ansc_xsocket_global.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscXsocketCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the General Xsocket Object and
        initializes the member variables and functions.

    argument:   ANSC_HANDLE                 hContainerContext
                This handle is used by the container object to interact
                with the outside world. It could be the real container
                or an target object.

                ANSC_HANDLE                 hOwnerContext
                This handle is passed in by the owner of this object.

                ANSC_HANDLE                 hAnscReserved
                This handle is passed in by the owner of this object.

    return:     newly created xsocket object.

**********************************************************************/

ANSC_HANDLE
AnscXsocketCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PANSC_XSOCKET_OBJECT            pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PANSC_XSOCKET_OBJECT)AnscAllocateMemory(sizeof(ANSC_XSOCKET_OBJECT));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pBaseObject = (PANSC_COMPONENT_OBJECT)pMyObject;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    /* AnscCopyString(pBaseObject->Name, ANSC_XSOCKET_NAME); */

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = ANSC_SOCKET_OID;
    pBaseObject->Create            = AnscXsocketCreate;
    pBaseObject->Remove            = AnscXsocketRemove;
    pBaseObject->EnrollObjects     = AnscXsocketEnrollObjects;
    pBaseObject->Initialize        = AnscXsocketInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXsocketRemove
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
AnscXsocketRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_XSOCKET_OBJECT            pMyObject           = (PANSC_XSOCKET_OBJECT)hThisObject;
    xskt_addrinfo*                  pxskt_ori_addrinfo  = (xskt_addrinfo*      )pMyObject->pOriPeerAddrInfo;
    xskt_addrinfo*                  pxskt_peer_addrinfo = (xskt_addrinfo*      )pMyObject->pPeerAddrInfo;
    xskt_addrinfo*                  pxskt_host_addrinfo = (xskt_addrinfo*      )pMyObject->pHostAddrInfo;
    PANSC_XSINK_OBJECT              pXsinkObject        = (PANSC_XSINK_OBJECT  )pMyObject->hXsinkObject;

    pMyObject->Close((ANSC_HANDLE)pMyObject);

    if ( pxskt_ori_addrinfo )
    {
        _xskt_freeaddrinfo(pxskt_ori_addrinfo);

        pMyObject->pOriPeerAddrInfo = NULL;
    }

    if ( pxskt_peer_addrinfo )
    {
        _xskt_freeaddrinfo(pxskt_peer_addrinfo);

        pMyObject->pPeerAddrInfo = NULL;
    }

    if ( pxskt_host_addrinfo )
    {
        _xskt_freeaddrinfo(pxskt_host_addrinfo);

        pMyObject->pHostAddrInfo = NULL;
    }

    if ( pXsinkObject )
    {
        pXsinkObject->hXsocketObject = (ANSC_HANDLE)NULL;
    }

    pMyObject->hXsinkObject = (ANSC_HANDLE)NULL;

    AnscCoVer3Remove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXsocketEnrollObjects
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function enrolls all the objects required by this object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXsocketEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_XSOCKET_OBJECT            pMyObject    = (PANSC_XSOCKET_OBJECT)hThisObject;
    PANSC_XSINK_OBJECT              pXsinkHolder = (PANSC_XSINK_OBJECT  )pMyObject->hXsinkHolder;

    if ( !pXsinkHolder )
    {
        pXsinkHolder = (PANSC_XSINK_OBJECT)AnscXsinkCreate((ANSC_HANDLE)pMyObject);

        if ( !pXsinkHolder )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hXsinkHolder = (ANSC_HANDLE)pXsinkHolder;
        }

        pXsinkHolder->hXsocketObject = (ANSC_HANDLE)pMyObject;
    }

    AnscCoVer3EnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXsocketInitialize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function initializes the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXsocketInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_XSOCKET_OBJECT            pMyObject    = (PANSC_XSOCKET_OBJECT)hThisObject;

    /*
     * Until you have to simulate C++ object-oriented programming style with standard C, you don't
     * appreciate all the nice little things come with C++ language and all the dirty works that
     * have been done by the C++ compilers. Member initialization is one of these things. While in
     * C++ you don't have to initialize all the member fields inherited from the base class since
     * the compiler will do it for you, such is not the case with C.
     */
    AnscCoVer3Initialize((ANSC_HANDLE)pMyObject);

    /*
     * Although we have initialized some of the member fields in the "create" member function, we
     * repeat the work here for completeness. While this simulation approach is pretty stupid from
     * a C++/Java programmer perspective, it's the best we can get for universal embedded network
     * programming. Before we develop our own operating system (don't expect that to happen any
     * time soon), this is the way things gonna be.
     */
    pMyObject->Oid              = ANSC_SOCKET_OID;
    pMyObject->Create           = AnscXsocketCreate;
    pMyObject->Remove           = AnscXsocketRemove;
    pMyObject->EnrollObjects    = AnscXsocketEnrollObjects;
    pMyObject->Initialize       = AnscXsocketInitialize;
    pMyObject->Shutdown         = AnscXsocketShutdown;

    pMyObject->Type             = ANSC_XSOCKET_TYPE_TCP;
    pMyObject->Mode             = ANSC_XSOCKET_MODE_ASYNC_CLOSE;
    pMyObject->hXsinkObject     = (ANSC_HANDLE)NULL;
    pMyObject->bClosed          = TRUE;

    pMyObject->GetType          = AnscXsocketGetType;
    pMyObject->SetType          = AnscXsocketSetType;
    pMyObject->GetMode          = AnscXsocketGetMode;
    pMyObject->SetMode          = AnscXsocketSetMode;
    pMyObject->GetIpProtocol    = AnscXsocketGetIpProtocol;
    pMyObject->SetIpProtocol    = AnscXsocketSetIpProtocol;
    pMyObject->GetHostAddress   = AnscXsocketGetHostAddress;
    pMyObject->SetHostAddress   = AnscXsocketSetHostAddress;
    pMyObject->GetHostName      = AnscXsocketGetHostName;
    pMyObject->SetHostName      = AnscXsocketSetHostName;
    pMyObject->GetHostPort      = AnscXsocketGetHostPort;
    pMyObject->SetHostPort      = AnscXsocketSetHostPort;
    pMyObject->GetHostAddrInfo  = AnscXsocketGetHostAddrInfo;
    pMyObject->SetHostAddrInfo  = AnscXsocketSetHostAddrInfo;
    pMyObject->GetPeerAddress   = AnscXsocketGetPeerAddress;
    pMyObject->SetPeerAddress   = AnscXsocketSetPeerAddress;
    pMyObject->GetPeerName      = AnscXsocketGetPeerName;
    pMyObject->SetPeerName      = AnscXsocketSetPeerName;
    pMyObject->GetPeerPort      = AnscXsocketGetPeerPort;
    pMyObject->SetPeerPort      = AnscXsocketSetPeerPort;
    pMyObject->GetPeerAddrInfo  = AnscXsocketGetPeerAddrInfo;
    pMyObject->SetPeerAddrInfo  = AnscXsocketSetPeerAddrInfo;
    pMyObject->GetTransportType = AnscXsocketGetTransportType;
    pMyObject->SetTransportType = AnscXsocketSetTransportType;
    pMyObject->GetXsink         = AnscXsocketGetXsink;
    pMyObject->SetXsink         = AnscXsocketSetXsink;

    pMyObject->ResolveAddr      = AnscXsocketResolveAddr;
    pMyObject->ApplyDSCP        = AnscXsocketApplyDSCP;
    pMyObject->Bind             = AnscXsocketBind;
    pMyObject->Connect          = AnscXsocketConnect;
    pMyObject->Listen           = AnscXsocketListen;
    pMyObject->Send             = AnscXsocketSend;
    pMyObject->Send2            = AnscXsocketSend2;
    pMyObject->Open             = AnscXsocketOpen;
    pMyObject->Close            = AnscXsocketClose;

    pMyObject->AcceptTask       = AnscXsocketAcceptTask;
    pMyObject->RecvTask         = AnscXsocketRecvTask;

    AnscInitializeLock(&pMyObject->OpLock);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXsocketShutdown
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function really destroys the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXsocketShutdown
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_XSOCKET_OBJECT            pMyObject    = (PANSC_XSOCKET_OBJECT)hThisObject;
    PANSC_XSINK_OBJECT              pXsinkHolder = (PANSC_XSINK_OBJECT  )pMyObject->hXsinkHolder;

    if ( pXsinkHolder )
    {
        pXsinkHolder->Remove((ANSC_HANDLE)pXsinkHolder);

        pMyObject->hXsinkHolder = (ANSC_HANDLE)NULL;
    }

    AnscFreeLock      (&pMyObject->OpLock    );
    AnscCoVer3Shutdown((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
