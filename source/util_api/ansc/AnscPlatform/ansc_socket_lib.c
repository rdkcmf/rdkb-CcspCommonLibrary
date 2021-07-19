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

    module:	ansc_socket_lib.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the well-known bsd-compatible socket
        library functions.

        *   AnscSocketLibInitialize
        *   AnscSocketLibUnload
        *   AnscSocketLibAccept
        *   AnscSocketLibBind
        *   AnscSocketLibCloseSocket
        *   AnscSocketLibConnect
        *   AnscSocketLibGetHostByAddr
        *   AnscSocketLibGetHostByName
        *   AnscSocketLibGetHostName
        *   AnscSocketLibGetPeerName
        *   AnscSocketLibGetSockName
        *   AnscSocketLibGetSockOpt
        *   AnscSocketLibIoctlSocket
        *   AnscSocketLibInetAddr
        *   AnscSocketLibInetNtoa
        *   AnscSocketLibListen
        *   AnscSocketLibRecv
        *   AnscSocketLibRecvFrom
        *   AnscSocketLibSelect
        *   AnscSocketLibSend
        *   AnscSocketLibSendTo
        *   AnscSocketLibSetSockOpt
        *   AnscSocketLibShutdown
        *   AnscSocketLibSocket
        *   AnscSocketLibGetLastError
        *   AnscSocketLibFdsIsSet
        *   AnscSocketTlsInitialize
        *   AnscSocketTlsUnload
        *   AnscSocketTlsGetScsIf

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        04/15/02    initial revision.

**********************************************************************/


#include "ansc_global.h"
#include "bss_ifo_bsd.h"
#include "safec_lib_common.h"

#ifdef _ANSC_SOCKET_LIBRARY_
static  ANSC_HANDLE                 g_hBssBsdIf      = (ANSC_HANDLE)NULL;
static  BOOL                        g_bBssBsdStarted = FALSE;
#endif
static  ANSC_HANDLE                 g_hTlsScsIf      = (ANSC_HANDLE)NULL;
static  BOOL                        g_bTlsScsStarted = FALSE;

#define  AnscSocketLibSetLastError(last_error)                                              \
         {                                                                                  \
            PANSC_TASK_RECORD       task_record = NULL;                                     \
                                                                                            \
            task_record = (PANSC_TASK_RECORD)AnscGetCurTaskRecord();                        \
                                                                                            \
            if ( task_record )                                                              \
            {                                                                               \
                AnscTaskSetSsocketError(task_record, last_error);                           \
            }                                                                               \
         }

#define  AnscSocketLibValidate1(error_code)                                                 \
         {                                                                                  \
            if ( !g_bBssBsdStarted || !g_hBssBsdIf )                                        \
            {                                                                               \
                error_code = ANSC_SOCKET_ERROR_LIB_UNAVAILABLE;                             \
                                                                                            \
                AnscSocketLibSetLastError(error_code);                                      \
                                                                                            \
                return  ANSC_SOCKET_INVALID_SOCKET;                                         \
            }                                                                               \
         }

#define  AnscSocketLibValidate2(error_code)                                                 \
         {                                                                                  \
            if ( !g_bBssBsdStarted || !g_hBssBsdIf )                                        \
            {                                                                               \
                error_code = ANSC_SOCKET_ERROR_LIB_UNAVAILABLE;                             \
                                                                                            \
                AnscSocketLibSetLastError(error_code);                                      \
                                                                                            \
                return  ANSC_SOCKET_ERROR;                                                  \
            }                                                                               \
         }

#define  AnscSocketLibValidate3(error_code)                                                 \
         {                                                                                  \
            if ( !g_bBssBsdStarted || !g_hBssBsdIf )                                        \
            {                                                                               \
                error_code = ANSC_SOCKET_ERROR_LIB_UNAVAILABLE;                             \
                                                                                            \
                AnscSocketLibSetLastError(error_code);                                      \
                                                                                            \
                return  NULL;                                                               \
            }                                                                               \
         }


#ifdef  _ANSC_SOCKET_LIBRARY_


void
AnscSocketLibInitialize
    (
        ANSC_HANDLE                 hBssBsdIf
    )
{
    if ( !g_bBssBsdStarted || !g_hBssBsdIf )
    {
        g_hBssBsdIf      = hBssBsdIf;
        g_bBssBsdStarted = TRUE;
    }

    return;
}


void
AnscSocketLibUnload
    (
        void
    )
{
    g_hBssBsdIf      = (ANSC_HANDLE)NULL;
    g_bBssBsdStarted = FALSE;

    return;
}


ANSC_SOCKET
AnscSocketLibAccept
    (
        ANSC_SOCKET                 socket,
        ansc_socket_addr*           addr,
        int*                        addrlen
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PBSS_BSD_INTERFACE              pBssBsdIf       = (PBSS_BSD_INTERFACE)g_hBssBsdIf;
    PBSS_BSD_SOCKADDR               pBssBsdSockAddr = (PBSS_BSD_SOCKADDR )addr;
    BSS_BSD_SOCKET                  sAcceptedSocket = BSS_BSD_SOCKET_INVALID_SOCKET;
    ULONG                           ulErrorCode     = 0;

    AnscSocketLibValidate1(ulErrorCode);

    if ( *addrlen < sizeof(BSS_BSD_SOCKADDR) )
    {
        ulErrorCode = ANSC_SOCKET_ERROR_INVALID_LEN;

        AnscSocketLibSetLastError(ulErrorCode);

        return  BSS_BSD_SOCKET_INVALID_SOCKET;
    }
    else
    {
        *addrlen = sizeof(BSS_BSD_SOCKADDR);
    }

    sAcceptedSocket =
        pBssBsdIf->Accept
            (
                pBssBsdIf->hOwnerContext,
                (BSS_BSD_SOCKET)socket,
                (ANSC_HANDLE   )pBssBsdSockAddr,
                &ulErrorCode
            );

    AnscSocketLibSetLastError(ulErrorCode);

    return  (ANSC_SOCKET)sAcceptedSocket;
}


int
AnscSocketLibBind
    (
        ANSC_SOCKET                 socket,
        ansc_socket_addr*           name,
        int                         namelen
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PBSS_BSD_INTERFACE              pBssBsdIf       = (PBSS_BSD_INTERFACE)g_hBssBsdIf;
    PBSS_BSD_SOCKADDR               pBssBsdSockAddr = (PBSS_BSD_SOCKADDR )name;
    ULONG                           ulErrorCode     = 0;
    int                             returnValue     = ANSC_SOCKET_ERROR;

    AnscSocketLibValidate2(ulErrorCode);

    if ( namelen < sizeof(BSS_BSD_SOCKADDR) )
    {
        ulErrorCode = ANSC_SOCKET_ERROR_INVALID_LEN;

        AnscSocketLibSetLastError(ulErrorCode);

        return  BSS_BSD_SOCKET_INVALID_SOCKET;
    }

    returnStatus =
        pBssBsdIf->Bind
            (
                pBssBsdIf->hOwnerContext,
                (BSS_BSD_SOCKET)socket,
                (ANSC_HANDLE   )pBssBsdSockAddr,
                &ulErrorCode
            );

    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        returnValue = 0;
    }
    else
    {
        returnValue = ANSC_SOCKET_ERROR;
    }

    AnscSocketLibSetLastError(ulErrorCode);

    return  returnValue;
}


int
AnscSocketLibCloseSocket
    (
        ANSC_SOCKET                 socket
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PBSS_BSD_INTERFACE              pBssBsdIf    = (PBSS_BSD_INTERFACE)g_hBssBsdIf;
    ULONG                           ulErrorCode  = 0;
    int                             returnValue  = ANSC_SOCKET_ERROR;

    AnscSocketLibValidate2(ulErrorCode);

    returnStatus =
        pBssBsdIf->CloseSocket
            (
                pBssBsdIf->hOwnerContext,
                (BSS_BSD_SOCKET)socket,
                &ulErrorCode
            );

    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        returnValue = 0;
    }
    else
    {
        returnValue = ANSC_SOCKET_ERROR;
    }

    AnscSocketLibSetLastError(ulErrorCode);

    return  returnValue;
}


int
AnscSocketLibConnect
    (
        ANSC_SOCKET                 socket,
        ansc_socket_addr*           name,
        int                         namelen
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PBSS_BSD_INTERFACE              pBssBsdIf       = (PBSS_BSD_INTERFACE)g_hBssBsdIf;
    PBSS_BSD_SOCKADDR               pBssBsdSockAddr = (PBSS_BSD_SOCKADDR )name;
    ULONG                           ulErrorCode     = 0;
    int                             returnValue     = ANSC_SOCKET_ERROR;

    AnscSocketLibValidate2(ulErrorCode);

    if ( namelen < sizeof(BSS_BSD_SOCKADDR) )
    {
        ulErrorCode = ANSC_SOCKET_ERROR_INVALID_LEN;

        AnscSocketLibSetLastError(ulErrorCode);

        return  BSS_BSD_SOCKET_INVALID_SOCKET;
    }

    returnStatus =
        pBssBsdIf->Connect
            (
                pBssBsdIf->hOwnerContext,
                (BSS_BSD_SOCKET)socket,
                (ANSC_HANDLE   )pBssBsdSockAddr,
                &ulErrorCode
            );

    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        returnValue = 0;
    }
    else
    {
        returnValue = ANSC_SOCKET_ERROR;
    }

    AnscSocketLibSetLastError(ulErrorCode);

    return  returnValue;
}


ansc_hostent*
AnscSocketLibGetHostByAddr
    (
        char*                       addr,
        int                         len,
        int                         type
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PBSS_BSD_INTERFACE              pBssBsdIf      = (PBSS_BSD_INTERFACE)g_hBssBsdIf;
    PBSS_BSD_HOSTENT                pBssBsdHostent = NULL;
    ULONG                           ulErrorCode    = 0;
    int                             returnValue    = ANSC_SOCKET_ERROR;

    AnscSocketLibValidate3(ulErrorCode);

    pBssBsdHostent =
        (PBSS_BSD_HOSTENT)pBssBsdIf->GetHostByAddr
            (
                pBssBsdIf->hOwnerContext,
                addr,
                len,
                type,
                &ulErrorCode
            );

    AnscSocketLibSetLastError(ulErrorCode);

    return  (ansc_hostent*)pBssBsdHostent;
}


ansc_hostent*
AnscSocketLibGetHostByName
    (
        char*                       name
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PBSS_BSD_INTERFACE              pBssBsdIf      = (PBSS_BSD_INTERFACE)g_hBssBsdIf;
    PBSS_BSD_HOSTENT                pBssBsdHostent = NULL;
    ULONG                           ulErrorCode    = 0;
    int                             returnValue    = ANSC_SOCKET_ERROR;

    AnscSocketLibValidate3(ulErrorCode);

    pBssBsdHostent =
        (PBSS_BSD_HOSTENT)pBssBsdIf->GetHostByName
            (
                pBssBsdIf->hOwnerContext,
                name,
                &ulErrorCode
            );

    AnscSocketLibSetLastError(ulErrorCode);

    return  (ansc_hostent*)pBssBsdHostent;
}


int
AnscSocketLibGetHostName
    (
        char*                       name,
        int                         namelen
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PBSS_BSD_INTERFACE              pBssBsdIf    = (PBSS_BSD_INTERFACE)g_hBssBsdIf;
    char*                           pHostName    = NULL;
    ULONG                           ulErrorCode  = 0;
    int                             returnValue  = ANSC_SOCKET_ERROR;
    errno_t                         rc           = -1;

    AnscSocketLibValidate2(ulErrorCode);

    pHostName =
        pBssBsdIf->GetHostName
            (
                pBssBsdIf->hOwnerContext,
                &ulErrorCode
            );

    if ( !pHostName )
    {
        returnValue = ANSC_SOCKET_ERROR;
    }
    else if ( AnscSizeOfString(pHostName) > (ULONG)namelen )
    {
        returnValue = ANSC_SOCKET_ERROR;
        ulErrorCode = ANSC_SOCKET_ERROR_INVALID_LEN;
    }
    else
    {
        returnValue = 0;

        rc = strcpy_s(name, sizeof(name), pHostName);
        ERR_CHK(rc);
    }

    AnscSocketLibSetLastError(ulErrorCode);

    return  returnValue;
}


int
AnscSocketLibGetPeerName
    (
        ANSC_SOCKET                 socket,
        ansc_socket_addr*           name,
        int*                        namelen
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PBSS_BSD_INTERFACE              pBssBsdIf       = (PBSS_BSD_INTERFACE)g_hBssBsdIf;
    PBSS_BSD_SOCKADDR               pBssBsdSockAddr = (PBSS_BSD_SOCKADDR )name;
    ULONG                           ulErrorCode     = 0;
    int                             returnValue     = ANSC_SOCKET_ERROR;

    AnscSocketLibValidate2(ulErrorCode);

    if ( *namelen < sizeof(BSS_BSD_SOCKADDR) )
    {
        ulErrorCode = ANSC_SOCKET_ERROR_INVALID_LEN;

        AnscSocketLibSetLastError(ulErrorCode);

        return  BSS_BSD_SOCKET_INVALID_SOCKET;
    }
    else
    {
        *namelen = sizeof(BSS_BSD_SOCKADDR);
    }

    returnStatus =
        pBssBsdIf->GetPeerName
            (
                pBssBsdIf->hOwnerContext,
                (BSS_BSD_SOCKET)socket,
                (ANSC_HANDLE   )pBssBsdSockAddr,
                &ulErrorCode
            );

    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        returnValue = 0;
    }
    else
    {
        returnValue = ANSC_SOCKET_ERROR;
    }

    AnscSocketLibSetLastError(ulErrorCode);

    return  returnValue;
}


int
AnscSocketLibGetSockName
    (
        ANSC_SOCKET                 socket,
        ansc_socket_addr*           name,
        int*                        namelen
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PBSS_BSD_INTERFACE              pBssBsdIf       = (PBSS_BSD_INTERFACE)g_hBssBsdIf;
    PBSS_BSD_SOCKADDR               pBssBsdSockAddr = (PBSS_BSD_SOCKADDR )name;
    ULONG                           ulErrorCode     = 0;
    int                             returnValue     = ANSC_SOCKET_ERROR;

    AnscSocketLibValidate2(ulErrorCode);

    if ( *namelen < sizeof(BSS_BSD_SOCKADDR) )
    {
        ulErrorCode = ANSC_SOCKET_ERROR_INVALID_LEN;

        AnscSocketLibSetLastError(ulErrorCode);

        return  BSS_BSD_SOCKET_INVALID_SOCKET;
    }
    else
    {
        *namelen = sizeof(BSS_BSD_SOCKADDR);
    }

    returnStatus =
        pBssBsdIf->GetSockName
            (
                pBssBsdIf->hOwnerContext,
                (BSS_BSD_SOCKET)socket,
                (ANSC_HANDLE   )pBssBsdSockAddr,
                &ulErrorCode
            );

    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        returnValue = 0;
    }
    else
    {
        returnValue = ANSC_SOCKET_ERROR;
    }

    AnscSocketLibSetLastError(ulErrorCode);

    return  returnValue;
}


int
AnscSocketLibGetSockOpt
    (
        ANSC_SOCKET                 socket,
        int                         level,
        int                         optname,
        char*                       optval,
        int*                        optlen
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PBSS_BSD_INTERFACE              pBssBsdIf    = (PBSS_BSD_INTERFACE)g_hBssBsdIf;
    ULONG                           ulErrorCode  = 0;
    int                             returnValue  = ANSC_SOCKET_ERROR;

    AnscSocketLibValidate2(ulErrorCode);

    returnStatus =
        pBssBsdIf->GetSockOpt
            (
                pBssBsdIf->hOwnerContext,
                (BSS_BSD_SOCKET)socket,
                level,
                optname,
                optval,
                optlen,
                &ulErrorCode
            );

    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        returnValue = 0;
    }
    else
    {
        returnValue = ANSC_SOCKET_ERROR;
    }

    AnscSocketLibSetLastError(ulErrorCode);

    return  returnValue;
}


int
AnscSocketLibIoctlSocket
    (
        ANSC_SOCKET                 socket,
        long                        cmd,
        unsigned long*              argp
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PBSS_BSD_INTERFACE              pBssBsdIf    = (PBSS_BSD_INTERFACE)g_hBssBsdIf;
    ULONG                           ulErrorCode  = 0;
    int                             returnValue  = ANSC_SOCKET_ERROR;

    AnscSocketLibValidate2(ulErrorCode);

    returnStatus =
        pBssBsdIf->IoctlSocket
            (
                pBssBsdIf->hOwnerContext,
                (BSS_BSD_SOCKET)socket,
                cmd,
                argp,
                &ulErrorCode
            );

    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        returnValue = 0;
    }
    else
    {
        returnValue = ANSC_SOCKET_ERROR;
    }

    AnscSocketLibSetLastError(ulErrorCode);

    return  returnValue;
}


ULONG
AnscSocketLibInetAddr
    (
        char*                       cp
    )
{
    ULONG                           ulStringSize  = AnscSizeOfString(cp);
    ULONG                           ulMaxStrSize  = 3 * 4 + 3;
    ULONG                           ulDigitCount  = 0;
    char*                           pTempAddrStr  = cp;
    ULONG                           i             = 0;
    ULONG                           j             = 0;
    char                            digitString[16];
    ANSC_IPV4_ADDRESS               ipAddress;

    if ( ulStringSize > ulMaxStrSize )
    {
        return  BSS_BSD_INADDR_NONE;
    }
    else if ( (cp[0] == '.') || (cp[ulStringSize - 1] == '.') )
    {
        return  BSS_BSD_INADDR_NONE;
    }

    for ( i = 0; i < IPV4_ADDRESS_SIZE; i++ )
    {
        j = 0;

        AnscZeroMemory(digitString, 16);

        while ( (*pTempAddrStr != '.') && (*pTempAddrStr != 0) )
        {
            digitString[j] = *pTempAddrStr;

            pTempAddrStr++;
            j           ++;
        }

        ipAddress.Dot[i] = (UCHAR)AnscGetStringUlong(digitString);

        if ( *pTempAddrStr == '.' )
        {
            pTempAddrStr++;
        }
        else if ( i != (IPV4_ADDRESS_SIZE - 1) )
        {
            return  BSS_BSD_INADDR_NONE;
        }
    }

    return  ipAddress.Value;
}


char*
AnscSocketLibInetNtoa
    (
        ansc_in_addr                in
    )
{
    ULONG                           ulStringSize = 0;
    PANSC_TASK_RECORD               pTaskRecord  = (PANSC_TASK_RECORD)AnscGetCurTaskRecord();
    char*                           pScratchPad  = NULL;
    char*                           pTempAddrStr = NULL;
    ULONG                           i            = 0;
    ANSC_IPV4_ADDRESS               ipAddress;

    if ( !pTaskRecord )
    {
        return  NULL;
    }
    else
    {
        ipAddress.Value = in.s_addr;
        pScratchPad     = AnscTaskGetSsocketPad(pTaskRecord);
        pTempAddrStr    = pScratchPad;

        AnscZeroMemory(pScratchPad, ANSC_TASK_SCRATCH_PAD_SIZE2);
    }

    for ( i = 0; i < IPV4_ADDRESS_SIZE; i++ )
    {
        AnscGetUlongString((ULONG)ipAddress.Dot[i], pTempAddrStr);

        pTempAddrStr += AnscSizeOfString(pTempAddrStr);

        if ( i < (IPV4_ADDRESS_SIZE - 1) )
        {
            *pTempAddrStr = '.';

            pTempAddrStr++;
        }
    }

    return  pScratchPad;
}


int
AnscSocketLibListen
    (
        ANSC_SOCKET                 socket,
        int                         backlog
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PBSS_BSD_INTERFACE              pBssBsdIf    = (PBSS_BSD_INTERFACE)g_hBssBsdIf;
    ULONG                           ulErrorCode  = 0;
    int                             returnValue  = ANSC_SOCKET_ERROR;

    AnscSocketLibValidate2(ulErrorCode);

    returnStatus =
        pBssBsdIf->Listen
            (
                pBssBsdIf->hOwnerContext,
                (BSS_BSD_SOCKET)socket,
                backlog,
                &ulErrorCode
            );

    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        returnValue = 0;
    }
    else
    {
        returnValue = ANSC_SOCKET_ERROR;
    }

    AnscSocketLibSetLastError(ulErrorCode);

    return  returnValue;
}


int
AnscSocketLibRecv
    (
        ANSC_SOCKET                 socket,
        char*                       buf,
        int                         len,
        int                         flags
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PBSS_BSD_INTERFACE              pBssBsdIf    = (PBSS_BSD_INTERFACE)g_hBssBsdIf;
    ULONG                           ulRecvSize   = (ULONG             )len;
    ULONG                           ulErrorCode  = 0;
    int                             returnValue  = ANSC_SOCKET_ERROR;

    AnscSocketLibValidate2(ulErrorCode);

    ulRecvSize   = (ULONG)len;
    returnStatus =
        pBssBsdIf->Recv
            (
                pBssBsdIf->hOwnerContext,
                (BSS_BSD_SOCKET)socket,
                (PVOID         )buf,
                (PULONG        )&ulRecvSize,
                (ULONG         )flags,
                &ulErrorCode
            );

    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        returnValue = (int)ulRecvSize;
    }
    else
    {
        returnValue = ANSC_SOCKET_ERROR;
    }

    AnscSocketLibSetLastError(ulErrorCode);

    return  returnValue;
}


int
AnscSocketLibRecvFrom
    (
        ANSC_SOCKET                 socket,
        char*                       buf,
        int                         len,
        int                         flags,
        ansc_socket_addr*           from,
        int*                        fromlen
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PBSS_BSD_INTERFACE              pBssBsdIf       = (PBSS_BSD_INTERFACE)g_hBssBsdIf;
    PBSS_BSD_SOCKADDR               pBssBsdSockAddr = (PBSS_BSD_SOCKADDR )from;
    ULONG                           ulRecvSize      = (ULONG             )len;
    ULONG                           ulErrorCode     = 0;
    int                             returnValue     = ANSC_SOCKET_ERROR;

    AnscSocketLibValidate2(ulErrorCode);

    if ( *fromlen < sizeof(BSS_BSD_SOCKADDR) )
    {
        ulErrorCode = ANSC_SOCKET_ERROR_INVALID_LEN;

        AnscSocketLibSetLastError(ulErrorCode);

        return  BSS_BSD_SOCKET_INVALID_SOCKET;
    }
    else
    {
        *fromlen = sizeof(BSS_BSD_SOCKADDR);
    }

    ulRecvSize   = (ULONG)len;
    returnStatus =
        pBssBsdIf->RecvFrom
            (
                pBssBsdIf->hOwnerContext,
                (BSS_BSD_SOCKET)socket,
                (PVOID         )buf,
                (PULONG        )&ulRecvSize,
                (ULONG         )flags,
                (ANSC_HANDLE   )pBssBsdSockAddr,
                &ulErrorCode
            );

    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        returnValue = ulRecvSize;
    }
    else
    {
        returnValue = ANSC_SOCKET_ERROR;
    }

    AnscSocketLibSetLastError(ulErrorCode);

    return  returnValue;
}


int
AnscSocketLibSelect
    (
        int                         nfds,
        ansc_fd_set*                readfds,
        ansc_fd_set*                writefds,
        ansc_fd_set*                exceptfds,
        ansc_timeval*               timeout
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PBSS_BSD_INTERFACE              pBssBsdIf      = (PBSS_BSD_INTERFACE)g_hBssBsdIf;
    PBSS_BSD_FDSET                  pBssBsdRdFdset = (PBSS_BSD_FDSET    )readfds;
    PBSS_BSD_FDSET                  pBssBsdWrFdset = (PBSS_BSD_FDSET    )writefds;
    PBSS_BSD_FDSET                  pBssBsdExFdset = (PBSS_BSD_FDSET    )exceptfds;
    PBSS_BSD_TIMEVAL                pBssBsdTimeval = (PBSS_BSD_TIMEVAL  )timeout;
    ULONG                           ulErrorCode    = 0;
    int                             returnValue    = ANSC_SOCKET_ERROR;

    AnscSocketLibValidate2(ulErrorCode);

    returnStatus =
        pBssBsdIf->Select
            (
                pBssBsdIf->hOwnerContext,
                nfds,
                (ANSC_HANDLE)pBssBsdRdFdset,
                (ANSC_HANDLE)pBssBsdWrFdset,
                (ANSC_HANDLE)pBssBsdExFdset,
                (ANSC_HANDLE)pBssBsdTimeval,
                &ulErrorCode
            );

    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        returnValue = 0;

        if ( pBssBsdRdFdset )
        {
            returnValue += (int)pBssBsdRdFdset->fd_count;
        }

        if ( pBssBsdWrFdset )
        {
            returnValue += (int)pBssBsdWrFdset->fd_count;
        }

        if ( pBssBsdExFdset )
        {
            returnValue += (int)pBssBsdExFdset->fd_count;
        }
    }
    else
    {
        returnValue = ANSC_SOCKET_ERROR;
    }

    AnscSocketLibSetLastError(ulErrorCode);

    return  returnValue;
}


int
AnscSocketLibSend
    (
        ANSC_SOCKET                 socket,
        char*                       buf,
        int                         len,
        int                         flags
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PBSS_BSD_INTERFACE              pBssBsdIf    = (PBSS_BSD_INTERFACE)g_hBssBsdIf;
    ULONG                           ulSendSize   = (ULONG             )len;
    ULONG                           ulErrorCode  = 0;
    int                             returnValue  = ANSC_SOCKET_ERROR;

    AnscSocketLibValidate2(ulErrorCode);

    ulSendSize   = (ULONG)len;
    returnStatus =
        pBssBsdIf->Send
            (
                pBssBsdIf->hOwnerContext,
                (BSS_BSD_SOCKET)socket,
                (PVOID         )buf,
                (PULONG        )&ulSendSize,
                (ULONG         )flags,
                &ulErrorCode
            );

    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        returnValue = (int)ulSendSize;
    }
    else
    {
        returnValue = ANSC_SOCKET_ERROR;
    }

    AnscSocketLibSetLastError(ulErrorCode);

    return  returnValue;
}


int
AnscSocketLibSendTo
    (
        ANSC_SOCKET                 socket,
        char*                       buf,
        int                         len,
        int                         flags,
        ansc_socket_addr*           to,
        int                         tolen
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PBSS_BSD_INTERFACE              pBssBsdIf       = (PBSS_BSD_INTERFACE)g_hBssBsdIf;
    PBSS_BSD_SOCKADDR               pBssBsdSockAddr = (PBSS_BSD_SOCKADDR )to;
    ULONG                           ulSendSize      = (ULONG             )len;
    ULONG                           ulErrorCode     = 0;
    int                             returnValue     = ANSC_SOCKET_ERROR;

    AnscSocketLibValidate2(ulErrorCode);

    if ( tolen < sizeof(BSS_BSD_SOCKADDR) )
    {
        ulErrorCode = ANSC_SOCKET_ERROR_INVALID_LEN;

        AnscSocketLibSetLastError(ulErrorCode);

        return  BSS_BSD_SOCKET_INVALID_SOCKET;
    }

    ulSendSize   = (ULONG)len;
    returnStatus =
        pBssBsdIf->SendTo
            (
                pBssBsdIf->hOwnerContext,
                (BSS_BSD_SOCKET)socket,
                (PVOID         )buf,
                (PULONG        )&ulSendSize,
                (ULONG         )flags,
                (ANSC_HANDLE   )pBssBsdSockAddr,
                &ulErrorCode
            );

    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        returnValue = ulSendSize;
    }
    else
    {
        returnValue = ANSC_SOCKET_ERROR;
    }

    AnscSocketLibSetLastError(ulErrorCode);

    return  returnValue;
}


int
AnscSocketLibSetSockOpt
    (
        ANSC_SOCKET                 socket,
        int                         level,
        int                         optname,
        char*                       optval,
        int                         optlen
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PBSS_BSD_INTERFACE              pBssBsdIf    = (PBSS_BSD_INTERFACE)g_hBssBsdIf;
    ULONG                           ulErrorCode  = 0;
    int                             returnValue  = ANSC_SOCKET_ERROR;

    AnscSocketLibValidate2(ulErrorCode);

    returnStatus =
        pBssBsdIf->SetSockOpt
            (
                pBssBsdIf->hOwnerContext,
                (BSS_BSD_SOCKET)socket,
                level,
                optname,
                optval,
                optlen,
                &ulErrorCode
            );

    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        returnValue = 0;
    }
    else
    {
        returnValue = ANSC_SOCKET_ERROR;
    }

    AnscSocketLibSetLastError(ulErrorCode);

    return  returnValue;
}


int
AnscSocketLibShutdown
    (
        ANSC_SOCKET                 socket,
        int                         how
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PBSS_BSD_INTERFACE              pBssBsdIf    = (PBSS_BSD_INTERFACE)g_hBssBsdIf;
    ULONG                           ulErrorCode  = 0;
    int                             returnValue  = ANSC_SOCKET_ERROR;

    AnscSocketLibValidate2(ulErrorCode);

    returnStatus =
        pBssBsdIf->Shutdown
            (
                pBssBsdIf->hOwnerContext,
                (BSS_BSD_SOCKET)socket,
                how,
                &ulErrorCode
            );

    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        returnValue = 0;
    }
    else
    {
        returnValue = ANSC_SOCKET_ERROR;
    }

    AnscSocketLibSetLastError(ulErrorCode);

    return  returnValue;
}


ANSC_SOCKET
AnscSocketLibSocket
    (
        int                         af,
        int                         type,
        int                         protocol
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PBSS_BSD_INTERFACE              pBssBsdIf    = (PBSS_BSD_INTERFACE)g_hBssBsdIf;
    BSS_BSD_SOCKET                  sNewSocket   = BSS_BSD_SOCKET_INVALID_SOCKET;
    ULONG                           ulErrorCode  = 0;

    AnscSocketLibValidate1(ulErrorCode);

    sNewSocket =
        pBssBsdIf->Socket
            (
                pBssBsdIf->hOwnerContext,
                af,
                type,
                protocol,
                &ulErrorCode
            );

    AnscSocketLibSetLastError(ulErrorCode);

    return  (ANSC_SOCKET)sNewSocket;
}


int
AnscSocketLibGetLastError
    (
        void
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_TASK_RECORD               pTaskRecord  = (PANSC_TASK_RECORD)AnscGetCurTaskRecord();
    ULONG                           ulErrorCode  = 0;

    if ( pTaskRecord )
    {
        ulErrorCode = AnscTaskGetSsocketError(pTaskRecord);
    }

    return  (int)ulErrorCode;
}


int
AnscSocketLibFdsIsSet
    (
        ANSC_SOCKET                 socket,
        ansc_fd_set*                fds
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PBSS_BSD_FDSET                  pBssBsdFdset = (PBSS_BSD_FDSET)fds;
    ULONG                           i            = 0;

    for ( i = 0; i < pBssBsdFdset->fd_count; i++ )
    {
        if ( pBssBsdFdset->fd_array[i] == (BSS_BSD_SOCKET)socket )
        {
            return  1;
        }
    }

    return  0;
}


#endif


void
AnscSocketTlsInitialize
    (
        ANSC_HANDLE                 hTlsScsIf
    )
{
    if ( !g_bTlsScsStarted || !g_hTlsScsIf )
    {
        g_hTlsScsIf      = hTlsScsIf;
        g_bTlsScsStarted = TRUE;
    }

    return;
}


void
AnscSocketTlsUnload
    (
        void
    )
{
    g_hTlsScsIf      = (ANSC_HANDLE)NULL;
    g_bTlsScsStarted = FALSE;

    return;
}


ANSC_HANDLE
AnscSocketTlsGetScsIf
    (
        void
    )
{
    return  g_hTlsScsIf;
}
