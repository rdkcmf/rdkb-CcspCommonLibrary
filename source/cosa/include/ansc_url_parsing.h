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

    module:	ansc_url_parsing.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros related to URL parsing from HTTP header.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Ding, Hua

    ---------------------------------------------------------------

    revision:

        08/29/05    initial revision.

**********************************************************************/


#ifndef  _ANSC_URL_PARSING_
#define  _ANSC_URL_PARSING_


/***********************************************************
                        CONSTANTS
***********************************************************/

#define  ANSC_URL_PARSING_MIN_HTTP_HEADER_LENGTH    16
#define  ANSC_URL_PARSING_MAX_EXTENSION_LENGTH      8

/***********************************************************
                        STRUCTURES
***********************************************************/

typedef  struct
_ANSC_URL_PARSING_REQUEST
{
    ULONG                           ulRequestMethod;        /* what is the request method type.                 */
    ANSC_IPV4_ADDRESS               DstIp;                  /* caller must pass in the destination IP address   */
    PCHAR                           pURL;                   /* url that requested. It should be absolute url.
                                                               The format is like: "http://www.yahoo.com/abc".  */
}
ANSC_URL_PARSING_REQUEST,  *PANSC_URL_PARSING_REQUEST;


/***********************************************************
                       FUNCTION PROTOTYPES
***********************************************************/

BOOLEAN
AnscUrlParsingIsHttpRequest
    (
        PVOID                       pHttpPayload,
        ULONG                       ulPayloadSize
    );

ANSC_STATUS
AnscUrlParsingGetHttpReqInfo
    (
        PVOID                       pHttpPayload,
        PANSC_URL_PARSING_REQUEST   pRequestInfo,
        PBOOLEAN                    pbIsGraphicsSkipped
    );


#endif

