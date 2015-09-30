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

    module:	ansc_xsocket_interface.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2002
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This module implements the some platform-dependent and
        general utility functions related to xsocket manipulation.

        *   AnscStartupXsocketWrapper
        *   AnscCleanupXsocketWrapper
        *   AnscCreateXsocket

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

    caller:     component objects

    prototype:

        ANSC_STATUS
        AnscStartupXsocketWrapper
            (
                ANSC_HANDLE                 hOwnerContext
            )

    description:

        This function is called to start up the xsocket wrapper.

    argument:   ANSC_HANDLE                 hOwnerContext
                This context handle is transparent to the xsocket
                wrapper, it's only meanful to the caller.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscStartupXsocketWrapper
    (
        ANSC_HANDLE                 hOwnerContext
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;

    #ifdef  _ANSC_WINDOWSNT

        WORD                        version = 0x202;
        WSADATA                     wsaData;

        WSAStartup(version, &wsaData);

    #endif

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_STATUS
        AnscCleanupXsocketWrapper
            (
                ANSC_HANDLE                 hOwnerContext
            )

    description:

        This function is called to clean up the xsocket wrapper.

    argument:   ANSC_HANDLE                 hOwnerContext
                This context handle is transparent to the xsocket
                wrapper, it's only meanful to the caller.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscCleanupXsocketWrapper
    (
        ANSC_HANDLE                 hOwnerContext
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;

    #ifdef  _ANSC_WINDOWSNT

        WSACleanup();

    #endif

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     component objects

    prototype:

        ANSC_HANDLE
        AnscCreateXsocket
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            )

    description:

        This function is called to create a new xsocket object.

    argument:   ANSC_HANDLE                 hContainerContext
                This context handle is transparent to the object mapper
                wrapper, it's only meanful to the caller.

                ANSC_HANDLE                 hOwnerContext
                This context handle is transparent to the object mapper
                wrapper, it's only meanful to the caller.

                ANSC_HANDLE                 hAnscReserved
                This context handle is transparent to the object mapper
                wrapper, it's only meanful to the caller.

    return:     handle of the xsocket object.

**********************************************************************/

/*
 *  Some definitions in BSS conflicts with Linux socket definitions,
 *  thus, xsocket cannot be compiled together with _ANSC_SOCKET_LIBRARY in Linux
 */
#if      !defined(_ANSC_LINUX) || !defined(_ANSC_KERNEL) || !defined(_ANSC_SOCKET_LIBRARY_)

ANSC_HANDLE
AnscCreateXsocket
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PANSC_XSOCKET_OBJECT            pXsocketObject = NULL;

    return  AnscXsocketCreate(hContainerContext, hOwnerContext, hAnscReserved);
}

#endif
