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

    module:	http_properties.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This file defines the configuration parameters that can be
        applied to the Http Systems.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/20/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_PROPERTIES_
#define  _HTTP_PROPERTIES_


/*
 * We have to use some of the constant values defined in the Http message definitions, so we
 * include the header file here.
 */
#include  "http_definitions.h"


/***********************************************************
    HTTP SIMPLE PROXY CONFIGURATION PARAMETERS DEFINITION
***********************************************************/

/*
 * The HyperText Transfer Protocol (HTTP) is an application-level protocol for distributed, col-
 * laborative, hypermedia information systems. It is a generic, stateless, protocol which can be
 * used for many tasks beyond its use for hypertext, through extension of its request methods,
 * error codes and headers. A feature of HTTP is the typing and negotiation of data representation,
 * allowing systems to be built independently of the data being transferred.
 */
#define  HTTP_PROXY_TYPE_SMALL                      1
#define  HTTP_PROXY_TYPE_MEDIUM                     2
#define  HTTP_PROXY_TYPE_BIG                        3
#define  HTTP_PROXY_TYPE_GIANT                      4

#define  HTTP_SPROXY_CONN_NUMBER                    0x0000007F      /* 128      */
#define  HTTP_MPROXY_CONN_NUMBER                    0x00004FFF      /* 4096     */
#define  HTTP_BPROXY_CONN_NUMBER                    0x0000FFFF      /* 65536    */
#define  HTTP_GPROXY_CONN_NUMBER                    0x00FFFFFF      /* 16777215 */

#define  HTTP_SPROXY_ENGINE_NUMBER                  2
#define  HTTP_MPROXY_ENGINE_NUMBER                  4
#define  HTTP_BPROXY_ENGINE_NUMBER                  16
#define  HTTP_GPROXY_ENGINE_NUMBER                  64

#define  HTTP_SPROXY_SOCKET_NUMBER                  128
#define  HTTP_MPROXY_SOCKET_NUMBER                  512
#define  HTTP_BPROXY_SOCKET_NUMBER                  2048
#define  HTTP_GPROXY_SOCKET_NUMBER                  4096

/*
 * A significant difference between HTTP/1.1 and earlier versions of HTTP is that persistent co-
 * nnetions are the default behavior of any HTTP connection. That is, unless otherwise indicated,
 * the client SHOULD assme that the server will maintain a persistent connection, even after error
 * responses from the server.
 *
 * The HTTP Simple Proxy Object creates a unique outbound session for each inbound HTTP session:
 * the closing of either session will bring down the other. The proxy itself does NOT contribute
 * to the maintainence of the persistent connection.
 */
#define  HTTP_SPO_MAX_MESSAGE_SIZE                  8192
#define  HTTP_SPO_SOCKET_TIMEOUT                    30              /* in seconds */
#define  HTTP_SPO_SOCKET_TTC                        2               /* in seconds */
#define  HTTP_SPO_RECV_BUFFER_ROOM                  256

typedef  struct
_HTTP_SIMPLE_PROXY_PROPERTY
{
    ANSC_IPV4_ADDRESS               HostAddress;
    USHORT                          HostPort;
    ULONG                           ProxyType;

    BOOL                            bChainedProxy;
    ANSC_IPV4_ADDRESS               NextProxyAddress;
    USHORT                          NextProxyPort;
}
HTTP_SIMPLE_PROXY_PROPERTY,  *PHTTP_SIMPLE_PROXY_PROPERTY;

/*
 * A significant difference between HTTP/1.1 and earlier versions of HTTP is that persistent co-
 * nnetions are the default behavior of any HTTP connection. That is, unless otherwise indicated,
 * the client SHOULD assme that the server will maintain a persistent connection, even after error
 * responses from the server.
 *
 * Unlike the Simple Proxy, HTTP Advanced Proxy Object was designed to meet a set of more sophis-
 * ticated HTTP proxy requirments, including:
 *
 *      $ providing cached response directly to the clients
 *      $ enable outbound session sharing if multiple clients is requesting the same URL
 *      $ maintain inbound and outbound sessions separately for better efficiency
 */
#define  HTTP_APO_MAX_MESSAGE_SIZE                  8192
#define  HTTP_APO_SOCKET_TIMEOUT                    30              /* in seconds */
#define  HTTP_APO_SOCKET_TTC                        3               /* in seconds */
#define  HTTP_APO_RECV_BUFFER_ROOM                  256

typedef  struct
_HTTP_ADVANCED_PROXY_PROPERTY
{
    ANSC_IPV4_ADDRESS               HostAddress;
    USHORT                          HostPort;
    ULONG                           ProxyType;

    BOOL                            bChainedProxy;
    ANSC_IPV4_ADDRESS               NextProxyAddress;
    USHORT                          NextProxyPort;
}
HTTP_ADVANCED_PROXY_PROPERTY,  *PHTTP_ADVANCED_PROXY_PROPERTY;


/***********************************************************
   HTTP SIMPLE SERVER CONFIGURATION PARAMETERS DEFINITION
***********************************************************/

/*
 * The HyperText Transfer Protocol (HTTP) is an application-level protocol for distributed, col-
 * laborative, hypermedia information systems. It is a generic, stateless, protocol which can be
 * used for many tasks beyond its use for hypertext, through extension of its request methods,
 * error codes and headers. A feature of HTTP is the typing and negotiation of data representation,
 * allowing systems to be built independently of the data being transferred.
 */
#define  HTTP_SERVER_TYPE_SMALL                     1
#define  HTTP_SERVER_TYPE_MEDIUM                    2
#define  HTTP_SERVER_TYPE_BIG                       3
#define  HTTP_SERVER_TYPE_GIANT                     4

#define  HTTP_SSERVER_ENGINE_NUMBER                 2
#define  HTTP_MSERVER_ENGINE_NUMBER                 4
#define  HTTP_BSERVER_ENGINE_NUMBER                 16
#define  HTTP_GSERVER_ENGINE_NUMBER                 64

#define  HTTP_SSERVER_SOCKET_NUMBER                 128
#define  HTTP_MSERVER_SOCKET_NUMBER                 512
#define  HTTP_BSERVER_SOCKET_NUMBER                 2048
#define  HTTP_GSERVER_SOCKET_NUMBER                 4096

#define  HTTP_SERVER_MODE_memoryConserving          0x00000001
#define  HTTP_SERVER_MODE_useXsocket                0x00000002

/*
 * HTTP Server is an application program that accepts connections in order to service requests by
 * sending back responses. Any given program may be capable of being both a client and a server;
 * our use of these terms refers only to the role being performed by the program for a particular
 * connection, rather than to the program's capabilities in general. Likewise, any server may act
 * as an origin server, proxy, gateway, or tunnel, switching behavior based on the nature of each
 * request.
 */
#define  HTTP_SSO_MAX_MESSAGE_SIZE                  128 * 1024          /* 128KB      */
#define  HTTP_SSO_MAX_HEADERS_SIZE                  4   * 1024          /* 4KB        */
#define  HTTP_SSO_SOCKET_TIMEOUT                    30                  /* in seconds */
#define  HTTP_SSO_SOCKET_TTC                        3                   /* in seconds */
#define  HTTP_SSO_RECV_BUFFER_ROOM                  2048                /* 2KB        */
#define  HTTP_SSO_DEF_BODY_SIZE_CAP                 4096 * 1024         /* 4MB        */
#define  HTTP_SSO_MAX_BODY_SIZE_CAP                 1024 * 1024 * 24    /* 24MB       */

typedef  struct
_HTTP_SIMPLE_SERVER_PROPERTY
{
    ANSC_IPV4_ADDRESS               HostAddress;
    USHORT                          HostPort;
    ULONG                           ServerType;
    ULONG                           ServerMode;
    ULONG                           ReqBodySizeCap;                 /* ignored if 'memoryConserving' is not set */
    ULONG                           MaxBodySizeCap;                 /* ignored if 'memoryConserving' is not set */
}
HTTP_SIMPLE_SERVER_PROPERTY,  *PHTTP_SIMPLE_SERVER_PROPERTY;

/***********************************************************
   HTTP SIMPLE CLIENT CONFIGURATION PARAMETERS DEFINITION
***********************************************************/

/*
 * The HyperText Transfer Protocol (HTTP) is an application-level protocol for distributed, col-
 * laborative, hypermedia information systems. It is a generic, stateless, protocol which can be
 * used for many tasks beyond its use for hypertext, through extension of its request methods,
 * error codes and headers. A feature of HTTP is the typing and negotiation of data representation,
 * allowing systems to be built independently of the data being transferred.
 */

#define  HTTP_MAX_PRODUCT_NAME_SIZE                 128
#define  HTTP_SCO_PRODUCT_NAME                      "Ansc Http Client v2.0 - Cisco Systems, Inc."

#define  HTTP_SCO_MHOST_MODE_SEQUENT                0
#define  HTTP_SCO_MHOST_MODE_RANDOM                 1

#define  HTTP_MAX_DEVICE_NAME_SIZE                  64

typedef struct 
_HTTP_BIND_TO_DEVICE_PROPERTY
{
    // BOOL                         bHostBindToDevice;
    char                            DeviceName[HTTP_MAX_DEVICE_NAME_SIZE];
}
HTTP_BIND_TO_DEVICE_PROPERTY, *PHTTP_BIND_TO_DEVICE_PROPERTY;

typedef  struct
_HTTP_SIMPLE_CLIENT_PROPERTY
{
    BOOL                            bReuseConnections;
    char                            ProductName[HTTP_MAX_PRODUCT_NAME_SIZE];
    ULONG                           SPMode;         /* how to pick up server if multiple hosts resolved */

    HTTP_BIND_TO_DEVICE_PROPERTY    BindToDevice;
}
HTTP_SIMPLE_CLIENT_PROPERTY,  *PHTTP_SIMPLE_CLIENT_PROPERTY;


/***********************************************************
   HTTP CLIENT PEER ADDRESSES DEFINITION
***********************************************************/

/*
 * The HyperText Transfer Protocol (HTTP) is an application-level protocol for distributed, col-
 * laborative, hypermedia information systems. It is a generic, stateless, protocol which can be
 * used for many tasks beyond its use for hypertext, through extension of its request methods,
 * error codes and headers. A feature of HTTP is the typing and negotiation of data representation,
 * allowing systems to be built independently of the data being transferred.
 */
#define  HTTP_SCO_MAX_RESOLVED_HOST                 8

typedef  struct
_HTTP_SCO_HOST_ADDRESSES
{
    ANSC_IPV4_ADDRESS               PeerAddresses[HTTP_SCO_MAX_RESOLVED_HOST];
    ULONG                           NumPeers;
    UCHAR                           PeerPickFlags[HTTP_SCO_MAX_RESOLVED_HOST];
}
HTTP_SCO_HOST_ADDRESSES,    *PHTTP_SCO_HOST_ADDRESSES;


/***********************************************************
    HTTP WEBC SESSION CONFIGURATION PARAMETERS DEFINITION
***********************************************************/

/*
 * The HyperText Transfer Protocol (HTTP) is an application-level protocol for distributed, col-
 * laborative, hypermedia information systems. It is a generic, stateless, protocol which can be
 * used for many tasks beyond its use for hypertext, through extension of its request methods,
 * error codes and headers. A feature of HTTP is the typing and negotiation of data representation,
 * allowing systems to be built independently of the data being transferred.
 */
#define  HTTP_WCSO_MAX_MESSAGE_SIZE                 8192

#define  HTTP_WCSO_TIMEOUT                          30           /* in seconds */

#define  HTTP_WCSO_RECV_BUFFER_ROOM                 256

#define  HTTP_WCSO_FLAG_tlsEnabled                  0x00000001
#define  HTTP_WCSO_FLAG_xsocketEnabled              0x00000002
#define  HTTP_WCSO_FLAG_BspNotifyOnAllConnOnce      0x00000004

typedef  struct
_HTTP_WEBC_SESSION_PROPERTY
{
    char                            PeerName[ANSC_DOMAIN_NAME_SIZE];
    ANSC_IPV4_ADDRESS               PeerAddr;
    USHORT                          PeerPort;
    ANSC_IPV4_ADDRESS               HostAddr;
    USHORT                          HostPort;
    ULONG                           SessionFlags;

}
HTTP_WEBC_SESSION_PROPERTY,  *PHTTP_WEBC_SESSION_PROPERTY;


typedef  struct
_HTTP_SCO_HOST_NAMES
{
    BOOL peerVerify;
    char **hostNames;
    int numHosts;
}
HTTP_SCO_HOST_NAMES, *PHTTP_SCO_HOST_NAMES;

#endif
