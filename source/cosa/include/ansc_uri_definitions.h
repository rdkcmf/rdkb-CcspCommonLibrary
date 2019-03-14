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

    module:	ansc_uri_definitions.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file encapsulates some of the most important
        definitions specified by URI Generic Syntax.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/09/03    initial revision.

**********************************************************************/


#ifndef  _ANSC_URI_DEFINITIONS_
#define  _ANSC_URI_DEFINITIONS_


/***********************************************************
       GENERIC AND SOME POPULAR URI SYNTAX DEFINITIONS
***********************************************************/

/*
 * A URI is a sequence of characters, which is not always represented as a sequence of octets. A
 * URI may be transcribed from a non-network source, and thus should consist of characters that are
 * most likely to be able to be typed into a computer, within the constraints imposed by keyboards
 * (and related input devices) across languages and locales. A URI often needs to be remembered by
 * people, and it is easier for people to remember a URI when it consists of meaningful components.
 */
#define  ANSC_URI_HOST_TYPE_HOSTNAME                1
#define  ANSC_URI_HOST_TYPE_IPV4_ADDRESS            2
#define  ANSC_URI_HOST_TYPE_IPV6_ADDRESS            3

typedef  struct
_ANSC_URI_HOST
{
    ULONG                           Type;
    union
    {
        char*                       HostName;
        char*                       Ipv4Addr;
        char*                       Ipv6Addr;
    }Name;
}
ANSC_URI_HOST,  *PANSC_URI_HOST;

#define  AnscInitUriHost(uri_host)                                                          \
         {                                                                                  \
            uri_host->Type          = ANSC_URI_HOST_TYPE_HOSTNAME;                          \
            uri_host->Name.HostName = NULL;                                                 \
         }

#define  AnscCleanUriHost(uri_host)                                                         \
         {                                                                                  \
            if ( uri_host->Name.HostName )                                                  \
            {                                                                               \
                AnscFreeMemory(uri_host->Name.HostName);                                    \
                                                                                            \
                uri_host->Name.HostName = NULL;                                             \
            }                                                                               \
         }

#define  AnscFreeUriHost(uri_host)                                                          \
         {                                                                                  \
            AnscCleanUriHost(uri_host);                                                     \
            AnscFreeMemory  (uri_host);                                                     \
         }

typedef  struct
_ANSC_URI_HOST_PORT
{
    ULONG                           Type;
    union
    {
        char*                       HostName;
        char*                       Ipv4Addr;
        char*                       Ipv6Addr;
    }Name;
    int                             Port;           /* -1 means no port information is available */
}
ANSC_URI_HOST_PORT,  *PANSC_URI_HOST_PORT;

#define  AnscInitUriHostPort(uri_host_port)                                                 \
         {                                                                                  \
            uri_host_port->Type          = ANSC_URI_HOST_TYPE_HOSTNAME;                     \
            uri_host_port->Name.HostName = NULL;                                            \
            uri_host_port->Port          = -1;                                              \
         }

#define  AnscCleanUriHostPort(uri_host_port)                                                \
         {                                                                                  \
            if ( uri_host_port->Name.HostName )                                             \
            {                                                                               \
                AnscFreeMemory(uri_host_port->Name.HostName);                               \
                                                                                            \
                uri_host_port->Name.HostName = NULL;                                        \
            }                                                                               \
         }

#define  AnscFreeUriHostPort(uri_host_port)                                                 \
         {                                                                                  \
            AnscCleanUriHostPort(uri_host_port);                                            \
            AnscFreeMemory      (uri_host_port);                                            \
         }

typedef  struct
_ANSC_URI_SERVER
{
    char*                           User;
    char*                           Pswd;
    ANSC_URI_HOST*                  Host;
    int                             Port;           /* -1 means no port information is available */
}
ANSC_URI_SERVER,  *PANSC_URI_SERVER;

#define  AnscInitUriServer(uri_server)                                                      \
         {                                                                                  \
            uri_server->User = NULL;                                                        \
            uri_server->Pswd = NULL;                                                        \
            uri_server->Host = NULL;                                                        \
            uri_server->Port = -1;                                                          \
         }

#define  AnscCleanUriServer(uri_server)                                                     \
         {                                                                                  \
            if ( uri_server->User )                                                         \
            {                                                                               \
                AnscFreeMemory(uri_server->User);                                           \
                                                                                            \
                uri_server->User = NULL;                                                    \
            }                                                                               \
                                                                                            \
            if ( uri_server->Pswd )                                                         \
            {                                                                               \
                AnscFreeMemory(uri_server->Pswd);                                           \
                                                                                            \
                uri_server->Pswd = NULL;                                                    \
            }                                                                               \
                                                                                            \
            if ( uri_server->Host )                                                         \
            {                                                                               \
                AnscFreeUriHost(uri_server->Host);                                          \
                                                                                            \
                uri_server->Host = NULL;                                                    \
            }                                                                               \
         }

#define  AnscFreeUriServer(uri_server)                                                      \
         {                                                                                  \
            AnscCleanUriServer(uri_server);                                                 \
            AnscFreeMemory    (uri_server);                                                 \
         }

#define  ANSC_URI_PART_TYPE_OTHER                   0
#define  ANSC_URI_PART_TYPE_HIER                    1
#define  ANSC_URI_PART_TYPE_OPAQUE                  2

#define  ANSC_URI_PATH_TYPE_OTHER                   0
#define  ANSC_URI_PATH_TYPE_NET                     1
#define  ANSC_URI_PATH_TYPE_ABS                     2
#define  ANSC_URI_PATH_TYPE_REL                     3

#define  ANSC_URI_AUTHORITY_TYPE_OTHER              0
#define  ANSC_URI_AUTHORITY_TYPE_SERVER             1
#define  ANSC_URI_AUTHORITY_TYPE_REGNAME            2

#define  ANSC_URI_MAX_PATH_LEVEL                    32

typedef  struct
_ANSC_URI_ABSOLUTE
{
    char*                           Scheme;
    ULONG                           UriPartType;
    char*                           OpaquePart;
    ULONG                           AuthorityType;
    union
    {
        ANSC_URI_SERVER*            Server;
        char*                       RegName;
    }Authority;
    ULONG                           PathLevel;
    char*                           PathArray[ANSC_URI_MAX_PATH_LEVEL];
    char*                           Query;
}
ANSC_URI_ABSOLUTE,  *PANSC_URI_ABSOLUTE;

#define  AnscInitUriAbsolute(uri_absolute)                                                  \
         {                                                                                  \
            ULONG                   kk = 0;                                                 \
                                                                                            \
            uri_absolute->Scheme        = NULL;                                             \
            uri_absolute->UriPartType   = ANSC_URI_PART_TYPE_HIER;                          \
            uri_absolute->OpaquePart    = NULL;                                             \
            uri_absolute->AuthorityType = ANSC_URI_AUTHORITY_TYPE_SERVER;                   \
                                                                                            \
            switch ( uri_absolute->AuthorityType )                                          \
            {                                                                               \
                case    ANSC_URI_AUTHORITY_TYPE_SERVER :                                    \
                                                                                            \
                        uri_absolute->Authority.Server = NULL;                              \
                                                                                            \
                        break;                                                              \
                                                                                            \
                case    ANSC_URI_AUTHORITY_TYPE_REGNAME :                                   \
                                                                                            \
                        uri_absolute->Authority.RegName = NULL;                             \
                                                                                            \
                        break;                                                              \
                                                                                            \
                default :                                                                   \
                                                                                            \
                        break;                                                              \
            }                                                                               \
                                                                                            \
            uri_absolute->PathLevel     = 0;                                                \
            uri_absolute->Query         = NULL;                                             \
                                                                                            \
            for ( kk = 0; kk < ANSC_URI_MAX_PATH_LEVEL; kk++ )                              \
            {                                                                               \
                uri_absolute->PathArray[kk] = NULL;                                         \
            }                                                                               \
         }

#define  AnscCleanUriAbsolute(uri_absolute)                                                 \
         {                                                                                  \
            ULONG                   kk = 0;                                                 \
                                                                                            \
            if ( uri_absolute->Scheme )                                                     \
            {                                                                               \
                AnscFreeMemory(uri_absolute->Scheme);                                       \
                                                                                            \
                uri_absolute->Scheme = NULL;                                                \
            }                                                                               \
                                                                                            \
            if ( uri_absolute->OpaquePart )                                                 \
            {                                                                               \
                AnscFreeMemory(uri_absolute->OpaquePart);                                   \
                                                                                            \
                uri_absolute->OpaquePart = NULL;                                            \
            }                                                                               \
                                                                                            \
            switch ( uri_absolute->AuthorityType )                                          \
            {                                                                               \
                case    ANSC_URI_AUTHORITY_TYPE_SERVER :                                    \
                                                                                            \
                        if ( uri_absolute->Authority.Server )                               \
                        {                                                                   \
                            AnscFreeUriServer(uri_absolute->Authority.Server);              \
                                                                                            \
                            uri_absolute->Authority.Server = NULL;                          \
                        }                                                                   \
                                                                                            \
                        break;                                                              \
                                                                                            \
                case    ANSC_URI_AUTHORITY_TYPE_REGNAME :                                   \
                                                                                            \
                        if ( uri_absolute->Authority.RegName )                              \
                        {                                                                   \
                            AnscFreeMemory(uri_absolute->Authority.RegName );               \
                                                                                            \
                            uri_absolute->Authority.RegName = NULL;                         \
                        }                                                                   \
                                                                                            \
                        break;                                                              \
                                                                                            \
                default :                                                                   \
                                                                                            \
                        break;                                                              \
            }                                                                               \
                                                                                            \
            for ( kk = 0; kk < uri_absolute->PathLevel; kk++ )                              \
            {                                                                               \
                if ( uri_absolute->PathArray[kk] )                                          \
                {                                                                           \
                    AnscFreeMemory(uri_absolute->PathArray[kk]);                            \
                                                                                            \
                    uri_absolute->PathArray[kk] = NULL;                                     \
                }                                                                           \
            }                                                                               \
                                                                                            \
            if ( uri_absolute->Query )                                                      \
            {                                                                               \
                AnscFreeMemory(uri_absolute->Query);                                        \
                                                                                            \
                uri_absolute->Query = NULL;                                                 \
            }                                                                               \
         }

#define  AnscFreeUriAbsolute(uri_absolute)                                                  \
         {                                                                                  \
            AnscCleanUriAbsolute(uri_absolute);                                             \
            AnscFreeMemory      (uri_absolute);                                             \
         }

typedef  struct
_ANSC_URI_RELATIVE
{
    ULONG                           PathType;
    ULONG                           AuthorityType;
    union
    {
        ANSC_URI_SERVER*            Server;
        char*                       RegName;
    }Authority;
    char*                           RelSegment;
    ULONG                           PathLevel;
    char*                           PathArray[ANSC_URI_MAX_PATH_LEVEL];
    char*                           Query;
}
ANSC_URI_RELATIVE,  *PANSC_URI_RELATIVE;

#define  AnscInitUriRelative(uri_relative)                                                  \
         {                                                                                  \
            ULONG                   kk = 0;                                                 \
                                                                                            \
            uri_relative->PathType      = ANSC_URI_PATH_TYPE_REL;                           \
            uri_relative->AuthorityType = ANSC_URI_AUTHORITY_TYPE_SERVER;                   \
                                                                                            \
            switch ( uri_relative->AuthorityType )                                          \
            {                                                                               \
                case    ANSC_URI_AUTHORITY_TYPE_SERVER :                                    \
                                                                                            \
                        uri_relative->Authority.Server = NULL;                              \
                                                                                            \
                        break;                                                              \
                                                                                            \
                case    ANSC_URI_AUTHORITY_TYPE_REGNAME :                                   \
                                                                                            \
                        uri_relative->Authority.RegName = NULL;                             \
                                                                                            \
                        break;                                                              \
                                                                                            \
                default :                                                                   \
                                                                                            \
                        break;                                                              \
            }                                                                               \
                                                                                            \
            uri_relative->RelSegment = NULL;                                                \
            uri_relative->PathLevel  = 0;                                                   \
            uri_relative->Query      = NULL;                                                \
                                                                                            \
            for ( kk = 0; kk < ANSC_URI_MAX_PATH_LEVEL; kk++ )                              \
            {                                                                               \
                uri_relative->PathArray[kk] = NULL;                                         \
            }                                                                               \
         }

#define  AnscCleanUriRelative(uri_relative)                                                 \
         {                                                                                  \
            ULONG                   kk = 0;                                                 \
                                                                                            \
            switch ( uri_relative->AuthorityType )                                          \
            {                                                                               \
                case    ANSC_URI_AUTHORITY_TYPE_SERVER :                                    \
                                                                                            \
                        if ( uri_relative->Authority.Server )                               \
                        {                                                                   \
                            AnscFreeUriServer(uri_relative->Authority.Server);              \
                                                                                            \
                            uri_relative->Authority.Server = NULL;                          \
                        }                                                                   \
                                                                                            \
                        break;                                                              \
                                                                                            \
                case    ANSC_URI_AUTHORITY_TYPE_REGNAME :                                   \
                                                                                            \
                        if ( uri_relative->Authority.RegName )                              \
                        {                                                                   \
                            AnscFreeMemory(uri_relative->Authority.RegName );               \
                                                                                            \
                            uri_relative->Authority.RegName = NULL;                         \
                        }                                                                   \
                                                                                            \
                        break;                                                              \
                                                                                            \
                default :                                                                   \
                                                                                            \
                        break;                                                              \
            }                                                                               \
                                                                                            \
            if ( uri_relative->RelSegment )                                                 \
            {                                                                               \
                AnscFreeMemory(uri_relative->RelSegment);                                   \
                                                                                            \
                uri_relative->RelSegment = NULL;                                            \
            }                                                                               \
                                                                                            \
            for ( kk = 0; kk < uri_relative->PathLevel; kk++ )                              \
            {                                                                               \
                if ( uri_relative->PathArray[kk] )                                          \
                {                                                                           \
                    AnscFreeMemory(uri_relative->PathArray[kk]);                            \
                                                                                            \
                    uri_relative->PathArray[kk] = NULL;                                     \
                }                                                                           \
            }                                                                               \
                                                                                            \
            if ( uri_relative->Query )                                                      \
            {                                                                               \
                AnscFreeMemory(uri_relative->Query);                                        \
                                                                                            \
                uri_relative->Query = NULL;                                                 \
            }                                                                               \
         }

#define  AnscFreeUriRelative(uri_relative)                                                  \
         {                                                                                  \
            AnscCleanUriRelative(uri_relative);                                             \
            AnscFreeMemory      (uri_relative);                                             \
         }

#define  ANSC_URI_REFERENCE_TYPE_ABSOLUTE           1
#define  ANSC_URI_REFERENCE_TYPE_RELATIVE           2

typedef  struct
_ANSC_GENERIC_URI
{
    ULONG                           ReferenceType;
    union
    {
        ANSC_URI_ABSOLUTE*          UriAbsolute;
        ANSC_URI_RELATIVE*          UriRelative;
    }Reference;
    char*                           Fragment;
}
ANSC_GENERIC_URI,  *PANSC_GENERIC_URI;

#define  AnscInitGenericUri(generic_uri)                                                    \
         {                                                                                  \
            generic_uri->ReferenceType         = ANSC_URI_REFERENCE_TYPE_ABSOLUTE;          \
            generic_uri->Reference.UriAbsolute = NULL;                                      \
            generic_uri->Fragment              = NULL;                                      \
         }

#define  AnscCleanGenericUri(generic_uri)                                                   \
         {                                                                                  \
            switch ( generic_uri->ReferenceType )                                           \
            {                                                                               \
                case    ANSC_URI_REFERENCE_TYPE_ABSOLUTE :                                  \
                                                                                            \
                        if ( generic_uri->Reference.UriAbsolute )                           \
                        {                                                                   \
                            AnscFreeUriAbsolute(generic_uri->Reference.UriAbsolute);        \
                                                                                            \
                            generic_uri->Reference.UriAbsolute = NULL;                      \
                        }                                                                   \
                                                                                            \
                        break;                                                              \
                                                                                            \
                case    ANSC_URI_REFERENCE_TYPE_RELATIVE :                                  \
                                                                                            \
                        if ( generic_uri->Reference.UriRelative )                           \
                        {                                                                   \
                            AnscFreeUriRelative(generic_uri->Reference.UriRelative);        \
                                                                                            \
                            generic_uri->Reference.UriRelative = NULL;                      \
                        }                                                                   \
                                                                                            \
                        break;                                                              \
                                                                                            \
                default :                                                                   \
                                                                                            \
                        break;                                                              \
            }                                                                               \
                                                                                            \
            if ( generic_uri->Fragment )                                                    \
            {                                                                               \
                AnscFreeMemory(generic_uri->Fragment);                                      \
                                                                                            \
                generic_uri->Fragment = NULL;                                               \
            }                                                                               \
         }

#define  AnscFreeGenericUri(generic_uri)                                                    \
         {                                                                                  \
            AnscCleanGenericUri(generic_uri);                                               \
            AnscFreeMemory     (generic_uri);                                               \
         }


#endif
