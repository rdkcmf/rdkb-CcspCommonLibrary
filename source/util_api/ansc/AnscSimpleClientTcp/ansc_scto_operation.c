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

    module:	ansc_scto_operation.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Ansc Simple Client Tcp Object.

        *   AnscSctoEngage
        *   AnscSctoCancel
        *   AnscSctoInitTlsClient

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        12/11/01    initial revision.

**********************************************************************/


#include "ansc_scto_global.h"
#include "safec_lib_common.h"

void Trace_Client_Server_address(char *ptr);

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscSctoEngage
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to engage the server daemon operation.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscSctoEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)hThisObject;
    PANSC_SCTO_WORKER_OBJECT        pWorker      = (PANSC_SCTO_WORKER_OBJECT      )pMyObject->hWorker;
    int                             s_error      = 0;
    ansc_socket_addr_in             ansc_client_addr;
    ansc_socket_addr_in             ansc_server_addr;
    xskt_socket_addr_in             xskt_client_addr;
    xskt_socket_addr_in             xskt_server_addr;

#ifdef _ANSC_IPV6_COMPATIBLE_
    ansc_addrinfo                   ansc_hints            = {0};
    ansc_addrinfo*                  pansc_server_addrinfo = NULL;
    ansc_addrinfo*                  pansc_client_addrinfo = NULL;
    xskt_addrinfo                   xskt_hints            = {0};
    xskt_addrinfo*                  pxskt_server_addrinfo = NULL, *pxskt_server_addrinfo_orig = NULL; //Modification for handling memory leak
    xskt_addrinfo*                  pxskt_client_addrinfo = NULL;
    USHORT                          usPort                = 0;
    char                            port[6] = {0};
    errno_t rc = -1;
#endif

    if ( pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else if ( !pWorker )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        /*
         * Just like any other socket-based ANSC object, we will create a separate async recv task
         * which is dedicated to receiving packets. This async recv task is controlled by 'bActive'
         * flag. What if at this moment right before we're about to enable the socket operation and
         * setting 'bActive' flag to TRUE, the old recv task created by the last call of Engage()
         * is still running? While it may not cause crash, but it certainly confuses the owner
         * object because all async recv tasks share the same worker interface. The most obvious
         * solution is to wait for previous recv task to exit before creating a new one.
         */
    	while ( pMyObject->EngineTaskCount != 0 )
        {
            AnscSleep(50);
        }

        pMyObject->bActive = TRUE;
        pMyObject->bClosed = FALSE;
    }

    /*
     * The underlying socket wrapper may require an explicit startup() call, such is the case on
     * Microsoft windows platforms. The wrapper initialization has to done for each task. On most
     * real-time operating systems, this call is not required.
     */
    if ( pMyObject->Mode & ANSC_SCTO_MODE_XSOCKET )
    {
        AnscStartupXsocketWrapper((ANSC_HANDLE)pMyObject);
    }
    else
    {
        AnscStartupSocketWrapper((ANSC_HANDLE)pMyObject);
    }

    #ifdef _ANSC_IPV6_COMPATIBLE_

    if ( pMyObject->Mode & ANSC_SCTO_MODE_XSOCKET )
    {
        xskt_hints.ai_family   = AF_UNSPEC;
        xskt_hints.ai_socktype = XSKT_SOCKET_STREAM;
        xskt_hints.ai_flags    = AI_CANONNAME;

        usPort = pMyObject->GetPeerPort((ANSC_HANDLE)pMyObject);
        rc = sprintf_s(port, sizeof(port), "%d", usPort);
        if(rc < EOK)
        {
            ERR_CHK(rc);
        }

        AnscTrace("!!! Peer Port: %s !!!\n", port);

        char * pPeerName = pMyObject->GetPeerName((ANSC_HANDLE)pMyObject);

        AnscTrace("Peer Name: %s!!!\n", pPeerName);

/*
        struct addrinfo hints,*res=NULL;
	memset(&hints,0,sizeof(hints));
	hints.ai_family=PF_UNSPEC;
	hints.ai_socktype=SOCK_DGRAM;
	hints.ai_protocol=IPPROTO_UDP;
	s_error=getaddrinfo("127.0.0.1","123",&hints,&res);
*/
/*
         s_error = _xskt_getaddrinfo
                (
                    "10.74.52.92",
                    port,
                    &xskt_hints,
                    &pxskt_server_addrinfo
                );

        AnscTrace("!!!!!! _xskt_getaddrinfo returns: %d %s !!!\n", s_error, gai_strerror(s_error));
*/
        if ( _xskt_getaddrinfo
                (
                    pMyObject->GetPeerName((ANSC_HANDLE)pMyObject), 
                    port,
                    &xskt_hints,
	            &pxskt_server_addrinfo_orig // Modification for handling memory leak
                ) ||
             _xskt_getaddrinfo
                (
                    "localhost", 
                    NULL,
                    &xskt_hints,
                    &pxskt_client_addrinfo
                )

            )
        {
            AnscTrace("!!! error 1 !!!\n");

            if ( pMyObject->Mode & ANSC_SCTO_MODE_NO_BSP_NOTIFY_CONN_ERR == 0 )
            {
                pWorker->Notify
	            	(
            		    pWorker->hWorkerContext,
            		    ANSC_SCTOWO_EVENT_SOCKET_ERROR,
            		    (ANSC_HANDLE)NULL
            		);
            }

            returnStatus = ANSC_STATUS_FAILURE;

            goto  EXIT1;
        }
	pxskt_server_addrinfo = pxskt_server_addrinfo_orig;

        AnscTrace("!!! after getaddrinfo !!!\n");
    }
    else
    {
        ansc_hints.ai_family   = AF_UNSPEC;
        ansc_hints.ai_socktype = ANSC_SOCKET_STREAM;
        ansc_hints.ai_flags    = AI_CANONNAME;

        usPort = pMyObject->GetPeerPort((ANSC_HANDLE)pMyObject);
        rc = sprintf_s(port, sizeof(port), "%d", usPort);
        if(rc < EOK)
        {
            ERR_CHK(rc);
        }

        if ( _ansc_getaddrinfo
                (
                    pMyObject->GetPeerName((ANSC_HANDLE)pMyObject), 
                    port,
                    &ansc_hints,
                    &pansc_server_addrinfo
                ) ||
             _ansc_getaddrinfo
                (
                    "localhost", 
                    NULL,
                    &ansc_hints,
                    &pansc_client_addrinfo
                )
            )
        {
            if ( pMyObject->Mode & ANSC_SCTO_MODE_NO_BSP_NOTIFY_CONN_ERR == 0 )
            {
                pWorker->Notify
	            	(
            		    pWorker->hWorkerContext,
		                ANSC_SCTOWO_EVENT_SOCKET_ERROR,
            	        (ANSC_HANDLE)NULL
	                );
            }

            returnStatus = ANSC_STATUS_FAILURE;

            goto  EXIT1;
        }
    }

#endif

    /*
     * To engage the Tcp Client, we need to perform following acts in the respective order:
     *
     *      (1) create the os-dependent socket
     *      (2) bind to the newly socket
     *      (3) connect to the specified  server address
     *      (4) allocate a buffer for receiving
     *      (5) spawn a separate thread and start receiving
     */
    if ( pMyObject->Mode & ANSC_SCTO_MODE_XSOCKET )
    {
#ifdef _ANSC_IPV6_COMPATIBLE_
        pMyObject->Socket = (ANSC_SOCKET)_xskt_socket(pxskt_server_addrinfo->ai_family, pxskt_server_addrinfo->ai_socktype, 0);
#else
        pMyObject->Socket = (ANSC_SOCKET)_xskt_socket(XSKT_SOCKET_AF_INET, XSKT_SOCKET_STREAM, 0);
#endif

        if ( (XSKT_SOCKET)pMyObject->Socket == XSKT_SOCKET_INVALID_SOCKET )
        {
            AnscTrace("!!!!!!!!!! _xskt_socket error !!!!!!!!!!\n");
            if ( pMyObject->Mode & (ANSC_SCTO_MODE_NO_BSP_NOTIFY_CONN_ERR == 0 ))
            {
                pWorker->Notify
	    			(
		    			pWorker->hWorkerContext,
			    		ANSC_SCTOWO_EVENT_SOCKET_ERROR,
				    	(ANSC_HANDLE)NULL
    				);
            }

            returnStatus = ANSC_STATUS_FAILURE;

            goto  EXIT1;
        }
        else  AnscTrace("Opening IPv4 socket Ok\n");
    }
    else
    {
#ifdef _ANSC_IPV6_COMPATIBLE_
        pMyObject->Socket = _ansc_socket(pansc_server_addrinfo->ai_family, pansc_server_addrinfo->ai_socktype, 0);
#else
        pMyObject->Socket = _ansc_socket(ANSC_SOCKET_AF_INET, ANSC_SOCKET_STREAM, 0);
#endif

        if ( pMyObject->Socket == ANSC_SOCKET_INVALID_SOCKET )
        {
            if ( pMyObject->Mode & (ANSC_SCTO_MODE_NO_BSP_NOTIFY_CONN_ERR == 0 ))
            {
                pWorker->Notify
	    			(
		    			pWorker->hWorkerContext,
			    		ANSC_SCTOWO_EVENT_SOCKET_ERROR,
				    	(ANSC_HANDLE)NULL
    				);
            }

            returnStatus = ANSC_STATUS_FAILURE;

            goto  EXIT1;
        }
    }
    /*
     * Normally we don't need to know which local network interface we shall bind to, and the
     * underlying operating system usually supports such notation as "any address".
     */
#ifndef _ANSC_IPV6_COMPATIBLE_
    if ( pMyObject->Mode & ANSC_SCTO_MODE_XSOCKET )
    {
        xskt_client_addr.sin_family = XSKT_SOCKET_AF_INET;
        xskt_client_addr.sin_port   = _xskt_htons(pMyObject->HostPort);

        if (pMyObject->bSocketBindToDevice && *(pMyObject->SocketDeviceName)) 
        {
            if (_xskt_setsocketopt
                    (
                         pMyObject->Socket, 
                         XSKT_SOCKET_SOL_SOCKET, 
                         XSKT_SOCKET_SO_BINDTODEVICE, 
                         pMyObject->SocketDeviceName, 
                         _ansc_strlen(pMyObject->SocketDeviceName) + 1
                    )
                < 0)
            {
                perror("setsockopt-SOL_SOCKET-SO_BINDTODEVICE");
                returnStatus = ANSC_STATUS_FAILURE;
                goto EXIT2;
            }
        }
        //        fprintf(stderr, "<RT XSKT> Binding socket to Device '%s'.\n", pMyObject->SocketDeviceName); 

        if ( pMyObject->HostAddress.Value == 0 )
        {
            ((pansc_socket_addr_in)&xskt_client_addr)->sin_addr.s_addr = XSKT_SOCKET_ANY_ADDRESS;
        }
        else
        {
            ((pansc_socket_addr_in)&xskt_client_addr)->sin_addr.s_addr = pMyObject->HostAddress.Value;
        }

        if ( _xskt_bind((XSKT_SOCKET)pMyObject->Socket, (xskt_socket_addr*)&xskt_client_addr, sizeof(xskt_client_addr)) != 0 )
        {
            AnscTrace("!!!!!!!!!! _xskt_bind error: socket=%d, error=%d !!!!!!!!!!\n", (XSKT_SOCKET)pMyObject->Socket, errno);
            Trace_Client_Server_address(((xskt_socket_addr*)(&xskt_client_addr))->sa_data);

            perror("_xskt_bind error");

            if ( pMyObject->Mode & (ANSC_SCTO_MODE_NO_BSP_NOTIFY_CONN_ERR == 0 ))
            {
                pWorker->Notify
	    			(
		    			pWorker->hWorkerContext,
			    		ANSC_SCTOWO_EVENT_SOCKET_ERROR,
				    	(ANSC_HANDLE)NULL
    				);
            }
            returnStatus = ANSC_STATUS_FAILURE;

            goto  EXIT2;
        }
    }
    else
    {
        ansc_client_addr.sin_family = ANSC_SOCKET_AF_INET;
        ansc_client_addr.sin_port   = _ansc_htons(pMyObject->HostPort);

        if (pMyObject->bSocketBindToDevice && *(pMyObject->SocketDeviceName)) 
        {
            if (_xskt_setsocketopt
                    (
                         pMyObject->Socket, 
                         ANSC_SOCKET_SOL_SOCKET, 
                         ANSC_SOCKET_SO_BINDTODEVICE, 
                         pMyObject->SocketDeviceName, 
                         _ansc_strlen(pMyObject->SocketDeviceName) + 1
                    )
                < 0)
            {
                perror("setsockopt-SOL_SOCKET-SO_BINDTODEVICE");
                returnStatus = ANSC_STATUS_FAILURE;
                goto EXIT2;
            }
        }
        // fprintf(stderr, "<RT AnscSKT> Binding socket to Device '%s'.\n", pMyObject->SocketDeviceName); 


        if ( pMyObject->HostAddress.Value == 0 )
        {
            ansc_client_addr.sin_addr.s_addr = ANSC_SOCKET_ANY_ADDRESS;
        }
        else
        {
            ansc_client_addr.sin_addr.s_addr = pMyObject->HostAddress.Value;
        }

        if ( _ansc_bind(pMyObject->Socket, (ansc_socket_addr*)&ansc_client_addr, sizeof(ansc_client_addr)) != 0 )
        {
            AnscTrace("!!!!!!!!!! _ansc_bind error: socket=%d, error=%d !!!!!!!!!!\n", (XSKT_SOCKET)pMyObject->Socket, errno);
            Trace_Client_Server_address(((xskt_socket_addr*)(&ansc_client_addr))->sa_data);

            perror("_ansc_bind error");

            if ( pMyObject->Mode & (ANSC_SCTO_MODE_NO_BSP_NOTIFY_CONN_ERR == 0 ))
            {
                pWorker->Notify
	    			(
		    			pWorker->hWorkerContext,
			    		ANSC_SCTOWO_EVENT_SOCKET_ERROR,
				    	(ANSC_HANDLE)NULL
    				);
            }

            returnStatus = ANSC_STATUS_FAILURE;

            goto  EXIT2;
        }
    }
#endif

    /*
     * As a Tcp client application, we now try to connect the network server, whose address is
     * specified by the "peer address" and "peer port" fields.
     */
    if ( pMyObject->Mode & ANSC_SCTO_MODE_XSOCKET )
    {
#ifdef _ANSC_IPV6_COMPATIBLE_
        BOOL                        bNoConn = TRUE;
#endif

        _ansc_memset(&xskt_server_addr, 0, sizeof(xskt_server_addr));
    	xskt_server_addr.sin_family                                 = XSKT_SOCKET_AF_INET;
        xskt_server_addr.sin_addr.s_addr  = pMyObject->PeerAddress.Value;
        xskt_server_addr.sin_port                                   = _xskt_htons(pMyObject->PeerPort);

#ifdef _ANSC_IPV6_COMPATIBLE_
        while ( bNoConn && pxskt_server_addrinfo )
        {
            if ( _xskt_connect((XSKT_SOCKET)pMyObject->Socket, pxskt_server_addrinfo->ai_addr, pxskt_server_addrinfo->ai_addrlen) != 0 )
            {
                pxskt_server_addrinfo = pxskt_server_addrinfo->ai_next;     /* try next ip address */
            }
            else
            {
                bNoConn = FALSE;
                break;
            }
        }

        if ( bNoConn )
#else
            if ( _xskt_connect((XSKT_SOCKET)pMyObject->Socket, (xskt_socket_addr*)&xskt_server_addr, sizeof(xskt_server_addr)) != 0 )
#endif
        {
            AnscTrace("!!!!!!!!!! _xskt_connect error: socket=%d, error=%d !!!!!!!!!!\n", (XSKT_SOCKET)pMyObject->Socket, errno);
            Trace_Client_Server_address(((xskt_socket_addr*)(&xskt_server_addr))->sa_data);

            perror("_xskt_connect error");

            s_error = _xskt_get_last_error();

            if ( pMyObject->Mode & (ANSC_SCTO_MODE_NO_BSP_NOTIFY_CONN_ERR == 0 ))
            {
                pWorker->Notify
	    			(
		    			pWorker->hWorkerContext,
			    		ANSC_SCTOWO_EVENT_SOCKET_TIMEOUT,
				    	(ANSC_HANDLE)NULL
    				);
            }
            returnStatus = ANSC_STATUS_FAILURE;

            goto  EXIT2;
        }
    }
    else
    {
        _ansc_memset(&ansc_server_addr, 0, sizeof(ansc_server_addr));
        ansc_server_addr.sin_family      = ANSC_SOCKET_AF_INET;
        ansc_server_addr.sin_addr.s_addr = pMyObject->PeerAddress.Value;
        ansc_server_addr.sin_port        = _ansc_htons(pMyObject->PeerPort);

#ifdef _ANSC_IPV6_COMPATIBLE_
        if ( _ansc_connect(pMyObject->Socket, pansc_server_addrinfo->ai_addr, pansc_server_addrinfo->ai_addrlen) != 0 )
#else
        if ( _ansc_connect(pMyObject->Socket, (ansc_socket_addr*)&ansc_server_addr, sizeof(ansc_server_addr)) != 0 )
#endif
        {
            s_error = _ansc_get_last_error();

            AnscTrace("!!! Connect error: %d, %s !!!\n", s_error, strerror(s_error));

            if ( pMyObject->Mode & (ANSC_SCTO_MODE_NO_BSP_NOTIFY_CONN_ERR == 0 ))
            {
                pWorker->Notify
				    (
			    		pWorker->hWorkerContext,
		    			ANSC_SCTOWO_EVENT_SOCKET_TIMEOUT,
	    				(ANSC_HANDLE)NULL
    				);
            }

            returnStatus = ANSC_STATUS_FAILURE;

            goto  EXIT2;
        }
    }

    /*
     * We have gone so far that all socket operations succeeded, we want to allocate a buffer that
     * is big enough for any incoming message.
     */
    if ( !pMyObject->RecvBuffer && !(pMyObject->Mode & ANSC_SCTO_MODE_FOREIGN_BUFFER) )
    {
        pMyObject->RecvBuffer     = AnscAllocateMemory(pMyObject->RecvBufferSize);
        pMyObject->RecvPacketSize = 0;
        pMyObject->RecvOffset     = 0;

        if ( !pMyObject->RecvBuffer )
        {
            returnStatus = ANSC_STATUS_RESOURCES;

            goto  EXIT2;
        }
    }

    pMyObject->RecvBytesCount = 0;
    pMyObject->SendBytesCount = 0;
    pMyObject->LastRecvAt     = AnscGetTickInSeconds();
    pMyObject->LastSendAt     = AnscGetTickInSeconds();

    /*
     * If the compilation option '_ANSC_SOCKET_TLS_LAYER_' is enabled, we can simply let the ANSC
     * socket layer to perform the SSL/TLS functionality; otherwise, we need to prepare for doing
     * SSL/TLS internally.
     */
    if ( pMyObject->Mode & ANSC_SCTO_MODE_TLS_ENABLED )
    {
#ifdef _ANSC_USE_OPENSSL_
        pMyObject->bTlsEnabled = TRUE;
        if ( !openssl_init(SSL_CLIENT_CALLS) )
        {
            AnscTrace("AnscSctoEngage - openssl_init() failed!\n");
            returnStatus = ANSC_STATUS_FAILURE;
            goto  EXIT2;
        }

#else
        #ifdef  _ANSC_SOCKET_TLS_LAYER_
        {
            _ansc_en_usetls(pMyObject->Socket);

            pMyObject->bTlsEnabled = FALSE;
        }
        #else
        {
            pMyObject->hTlsScsIf   = (pMyObject->hTlsScsIf != NULL)? pMyObject->hTlsScsIf : AnscSocketTlsGetScsIf();
            pMyObject->bTlsEnabled = TRUE;
        }
        #endif
#endif
    }

    /*
     * To save the worker object from having to deal with blocking/non-blocking/async receiving
     * functions provided by underlying socket layer, we create a separate task to do that.
     */
    returnStatus =
        pMyObject->SpawnTask3
            (
                (ANSC_HANDLE)pMyObject,
                (void*      )pMyObject->RecvTask,
                (ANSC_HANDLE)pMyObject,
                ANSC_SCTO_RECV_TASK_NAME,
                USER_DEFAULT_TASK_PRIORITY,
                11*USER_DEFAULT_TASK_STACK_SIZE

            );
#ifdef _ANSC_USE_OPENSSL_
    if ( pMyObject->bTlsEnabled )
    {
   	    SSL *ssl = NULL;
   	    ssl = openssl_connect (pMyObject->Socket, (void *)&pMyObject->hostNames);
	    if ( !ssl )
		{

	        pMyObject->bTlsConnected = FALSE;
	        returnStatus = ANSC_STATUS_FAILURE;

	        //PANSC_SCTO_WORKER_OBJECT        pWorker      = (PANSC_SCTO_WORKER_OBJECT)pMyObject->hWorker;
            if ( pMyObject->Mode & (ANSC_SCTO_MODE_NO_BSP_NOTIFY_CONN_ERR == 0 ))
            {
                pWorker->Notify
                    (
                        pWorker->hWorkerContext,
                        ANSC_SCTOWO_EVENT_TLS_ERROR,
                        (ANSC_HANDLE)0L
                    );
            }
		}
	    else
	    {
			s_error = openssl_validate_certificate (pMyObject->Socket, pMyObject->HostName, ssl, SSL_CLIENT_CALLS, pMyObject->hostNames.peerVerify);
		    if ( s_error == 0 )
		    {
		        AnscTrace("AnscSctoEngage - openssl_validate_certificate() failed %p.\n", ssl);

		        //PANSC_SCTO_WORKER_OBJECT        pWorker      = (PANSC_SCTO_WORKER_OBJECT      )pMyObject->hWorker;
                if ( pMyObject->Mode & (ANSC_SCTO_MODE_NO_BSP_NOTIFY_CONN_ERR == 0) )
                {
	                pWorker->Notify
	                    (
	                        pWorker->hWorkerContext,
	                        ANSC_SCTOWO_EVENT_TLS_ERROR,
	                        (ANSC_HANDLE)NULL
	                    );
                }
		        returnStatus = ANSC_STATUS_FAILURE;
		    }

		    pMyObject->hTlsConnection = ssl;
		    pMyObject->bTlsConnected = TRUE;
	    }
    }
#else
    /*
     * If SSL/TLS is enabled, we should complete TLS handshake before returning from Engage(). This
     * behavior allows a single consistent API to used between this object and the worker object.
     * Since the handshake will take multiple messages to complete, we need to block the current
     * task until being notified by the TLS module.
     */
    if ( pMyObject->bTlsEnabled && pMyObject->hTlsScsIf )
    {
        pMyObject->bTlsConnected = FALSE;

        pMyObject->InitTlsClient((ANSC_HANDLE)pMyObject);
    }
#endif

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
    	goto  EXIT2;
    }

    goto FREE_ADDRINFO;

    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT2:

	AnscTrace("AnscSctoEngage - failed with status %lu, socket error %d!\n", returnStatus, s_error);

    if ( pMyObject->Mode & ANSC_SCTO_MODE_XSOCKET )
    {
        _xskt_closesocket((XSKT_SOCKET)pMyObject->Socket);
    }
    else
    {
        _ansc_closesocket(pMyObject->Socket);
    }

EXIT1:

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        pMyObject->bActive = FALSE;

        pMyObject->Reset((ANSC_HANDLE)pMyObject);
    }

FREE_ADDRINFO: // Handling memory leak
#ifdef _ANSC_IPV6_COMPATIBLE_
    if ( pxskt_server_addrinfo_orig )
    {
        _xskt_freeaddrinfo(pxskt_server_addrinfo_orig);
    }

    if ( pxskt_client_addrinfo )
    {
        _xskt_freeaddrinfo(pxskt_client_addrinfo);
    }

    if ( pansc_server_addrinfo )
    {
        _ansc_freeaddrinfo(pansc_server_addrinfo);
    }

    if ( pansc_client_addrinfo )
    {
        _ansc_freeaddrinfo(pansc_client_addrinfo);
    } 
#endif

    return  returnStatus;
}

void Trace_Client_Server_address(char *ptr)
{
    int j;
    char s[256];
    char stmp[16];
    s[0] = '\0';
    errno_t rc = -1;

    for(j=0; j<13; j++) {
        rc = sprintf_s(stmp, sizeof(stmp), "%.2x:", *(ptr++));
        if(rc < EOK)
        {
            ERR_CHK(rc);
        }
        rc = strcat_s(s, sizeof(s), stmp);
        ERR_CHK(rc);
    }
    rc = sprintf_s(stmp, sizeof(stmp), "%.2x", *ptr);
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }
    rc = strcat_s(s, sizeof(s), stmp);
    ERR_CHK(rc);
    AnscTrace("!!!!!!!!!! _xskt_bind error: server/client_addr=%s\n", s);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscSctoCancel
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to cancel the server daemon operation.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscSctoCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)hThisObject;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
    	pMyObject->bShuttingDown = TRUE;
        pMyObject->bActive = FALSE;
        pMyObject->bClosed = TRUE;

        while ( pMyObject->EngineTaskCount != 0 )
        {
            AnscSleep(500);
        }

    }

#ifdef _ANSC_USE_OPENSSL_
    if ( pMyObject->hTlsConnection )
    {
        SSL *ssl = (SSL *) pMyObject->hTlsConnection;
    	AnscTrace ("SSL connection %p is closed.\n", ssl);
        SSL_shutdown (ssl);
        SSL_free (ssl);
    }
#else
    if ( pMyObject->bTlsEnabled && pMyObject->hTlsScsIf )
    {
        PTLS_SCS_INTERFACE              pTlsScsIf    = (PTLS_SCS_INTERFACE            )pMyObject->hTlsScsIf;

        AnscSetEvent(&pMyObject->TlsConnEvent);

        if ( pMyObject->hTlsConnection )
        {
            if ( pMyObject->bTlsConnected )
            {
                    pTlsScsIf->CloseConnection
                        (
                            pTlsScsIf->hOwnerContext,
                            pMyObject->hTlsConnection
                        );
            }

                pTlsScsIf->RemoveConnection
                    (
                        pTlsScsIf->hOwnerContext,
                        pMyObject->hTlsConnection
                    );
        }

        pMyObject->bTlsConnected = FALSE;
    }
#endif

    if ( pMyObject->Mode & ANSC_SCTO_MODE_XSOCKET )
    {
        if ( (XSKT_SOCKET)pMyObject->Socket != XSKT_SOCKET_INVALID_SOCKET )
        {
            _xskt_shutdown   (((XSKT_SOCKET)pMyObject->Socket), XSKT_SOCKET_SD_RECV);
            _xskt_closesocket(((XSKT_SOCKET)pMyObject->Socket));

            pMyObject->Socket = (ANSC_SOCKET)XSKT_SOCKET_INVALID_SOCKET;
        }
    }
    else
    {
        if ( pMyObject->Socket != ANSC_SOCKET_INVALID_SOCKET )
        {
            _ansc_shutdown   (pMyObject->Socket, ANSC_SOCKET_SD_RECV);
            _ansc_closesocket(pMyObject->Socket);

            pMyObject->Socket = ANSC_SOCKET_INVALID_SOCKET;
        }
    }

    /*
     * The underlying socket wrapper may require an explicit cleanup() call, such is the case on
     * Microsoft windows platforms. The wrapper initialization has to done for each task. On most
     * real-time operating systems, this call is not required.
     */
    if ( pMyObject->Mode & ANSC_SCTO_MODE_XSOCKET )
    {
        AnscCleanupXsocketWrapper((ANSC_HANDLE)pMyObject);
    }
    else
    {
        AnscCleanupSocketWrapper((ANSC_HANDLE)pMyObject);
    }

    pMyObject->Reset((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}

#ifndef _ANSC_USE_OPENSSL_

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscSctoInitTlsClient
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to initialize TLS client operation.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscSctoInitTlsClient
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)hThisObject;
    PANSC_SCTO_WORKER_OBJECT        pWorker      = (PANSC_SCTO_WORKER_OBJECT      )pMyObject->hWorker;
    PTLS_SCS_INTERFACE              pTlsScsIf    = (PTLS_SCS_INTERFACE            )pMyObject->hTlsScsIf;
    PTLS_TSA_INTERFACE              pTlsTsaIf    = (PTLS_TSA_INTERFACE            )pMyObject->hTlsTsaIf;
    TLS_CONNECTION_PARAMS           tlsConnParams;

    if ( !pMyObject->bTlsEnabled || !pTlsScsIf )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    if ( TRUE )
    {
        TlsInitConnParams((&tlsConnParams));

        tlsConnParams.bSessionSharing  = TRUE;
        tlsConnParams.bQuickHandshake  = TRUE;
        tlsConnParams.bReqCertificate  = FALSE;
        tlsConnParams.bTlsClient       = TRUE;

        tlsConnParams.HostID           = pMyObject->HostAddress.Value;
        tlsConnParams.PeerID           = pMyObject->PeerAddress.Value;
        tlsConnParams.SessionIDSize    = 0;

        tlsConnParams.CipherSuiteCount = 0;
        tlsConnParams.CompressionCount = 0;
    }

    pMyObject->hTlsConnection =
        pTlsScsIf->CreateConnection
            (
                pTlsScsIf->hOwnerContext,
                (ANSC_HANDLE)pTlsTsaIf
            );

    if ( !pMyObject->hTlsConnection )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        returnStatus =
            pTlsScsIf->ConfigConnection
                (
                    pTlsScsIf->hOwnerContext,
                    pMyObject->hTlsConnection,
                    (ANSC_HANDLE)&tlsConnParams
                );
    }

    AnscResetEvent(&pMyObject->TlsConnEvent);

    returnStatus =
        pTlsScsIf->StartConnection
            (
                pTlsScsIf->hOwnerContext,
                pMyObject->hTlsConnection
            );

    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        AnscWaitEvent(&pMyObject->TlsConnEvent, 0xFFFFFFFF);
    }

    if ( !pMyObject->bTlsConnected )
    {
        returnStatus =
            pTlsScsIf->RemoveConnection
                (
                    pTlsScsIf->hOwnerContext,
                    pMyObject->hTlsConnection
                );

        pMyObject->hTlsConnection = (ANSC_HANDLE)NULL;

        returnStatus = ANSC_STATUS_FAILURE;
    }
    else
    {
        returnStatus = ANSC_STATUS_SUCCESS;
    }

    return  returnStatus;
}
#endif
