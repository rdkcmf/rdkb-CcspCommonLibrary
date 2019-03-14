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

    module:	ansc_http_pub_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the object interface for the http 
        request utility  object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        05/19/04    initial revision.

**********************************************************************/


#ifndef  _ANSC_HTTP_PUB_INTERFACE_
#define  _ANSC_HTTP_PUB_INTERFACE_

/***********************************************************
              GENERAL CRYPTO OBJECT DEFINITION
***********************************************************/

typedef  struct
_ANSC_HTTP_CONTENT
{
    ULONG                           ulContentSize;
    PVOID                           pContent;
}
ANSC_HTTP_CONTENT,  *PANSC_HTTP_CONTENT;


#define  AnscInitHttpContent(tbi)                                                           \
         {                                                                                  \
            AnscZeroMemory(tbi, sizeof(ANSC_HTTP_CONTENT));                                 \
         }

#define AnscClearHttpContent(tbi)                                                           \
        {                                                                                   \
            if( tbi->pContent != NULL)                                                      \
            {                                                                               \
                AnscFreeMemory(tbi->pContent);                                              \
            }                                                                               \
         }

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_HTTP_PUB_GET)
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       hHttpUrl,
        ANSC_HANDLE                 hHttpContent
    );

typedef  ANSC_STATUS
(*PFN_HTTP_PUB_REMOVE)
    (
        ANSC_HANDLE                 hOwnerContext
    );

typedef  ANSC_STATUS
(*PFN_HTTP_PUB_INIT)
    (
        ANSC_HANDLE                 hOwnerContext
    );


#define  _ANSC_HTTP_PUB_BASE                                                    \
    ANSC_HANDLE                     hContainerContext;                          \
    ANSC_HANDLE                     hHttpSimpleClient;                          \
    ANSC_HANDLE                     hHttpMsgParser;                             \
    ANSC_HANDLE                     hBspIf;                                     \
    ANSC_LOCK                       httpLock;                                   \
    ANSC_EVENT                      httpEvent;                                  \
    ANSC_HTTP_CONTENT               httpContent;                                \
                                                                                \
    PFN_HTTP_PUB_REMOVE             Remove;                                     \
    PFN_HTTP_PUB_INIT               Initialize;                                 \
                                                                                \
    PFN_HTTP_PUB_GET                GetUrl;                                     \



/*
 * The Crypto Object provides a set of crypto wrapper functions for other container objects to
 * perform symmetric encryption/decryption, hmac-based authentication, one-way hash function and
 * asymmetric encryption/decryption, and signature verification.
 */
typedef  struct
_ANSC_HTTP_PUB_OBJECT
{
    _ANSC_HTTP_PUB_BASE
}
ANSC_HTTP_PUB_OBJECT,  *PANSC_HTTP_PUB_OBJECT;


#endif
