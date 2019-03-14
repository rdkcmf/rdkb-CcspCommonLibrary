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

    module:	ansc_bsto_properties.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines data structures and forms that
        describe Broker Server Tcp Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/19/02    initial revision.

**********************************************************************/


#ifndef  _ANSC_BSTO_PROPERTIES_
#define  _ANSC_BSTO_PROPERTIES_


/***********************************************************
      BROKER SERVER TCP OBJECT PROPERTY DATA STRUCTURES
***********************************************************/

/*
 * Overview form reveals the general information about the Broker Server Object:
 *
 *      $ start time and current time stamp
 *      $ number of active engines
 *      $ minimum socket count
 *      $ maximum socket count
 *      $ current socket count
 *      $ number of rejected requests due to lack of available socket
 *      $ number of rejected requests in the last minute
 *      $ number of requests served so far
 *      $ number of requests served in the last minute
 */
typedef  struct
_ANSC_BSTO_OVERVIEW_FORM
{
    ULONG                           StartTime;                                  /* system-up time in seconds            */
    ULONG                           CurrentTime;                                /* system-up time in seconds            */
    ULONG                           EngineCount;                                /* number of active engines             */
    ULONG                           MinSocketCount;                             /* initial socket count                 */
    ULONG                           MaxSocketCount;                             /* maximum socket count                 */
    ULONG                           CurSocketCount;                             /* current socket count                 */
    ULONG                           TccCount;                                   /* total number of corrupted request    */
    ULONG                           MccCount;                                   /* corrupted request in the last minute */
    ULONG                           TrcCount;                                   /* total number of clients rejected     */
    ULONG                           MrcCount;                                   /* clients rejected in the last minute  */
    ULONG                           TscCount;                                   /* total number of clients served       */
    ULONG                           MscCount;                                   /* clients served in the last minute    */
}
ANSC_BSTO_OVERVIEW_FORM,  *PANSC_BSTO_OVERVIEW_FORM;

/*
 * Snapshot form reveals statistic information about all the active engines:
 *
 *      $ engine start time
 *      $ number of clients is currently being served
 *      $ number of sockets in the recv_set
 *      $ number of sockets in the send_set
 *      $ average process time on each request in milliseconds
 *      $ total number of clients served so far
 *      $ number of clients served in the last minute
 */
typedef  struct
_ANSC_BSTO_SNAPSHOT_FORM
{
    ULONG                           EngineIndex;                                /* index of the active engine           */
    ULONG                           StartTime;                                  /* system-up time in seconds            */
    ULONG                           MaxClientCount;                             /* number of maximum client requests    */
    ULONG                           OffClientCount;                             /* number of official client requests   */
    ULONG                           CurClientCount;                             /* number of active client requests     */
    ULONG                           CurRecvSetSize;                             /* number of sockets in the recv_set    */
    ULONG                           CurSendSetSize;                             /* number of sockets in the send_set    */
    ULONG                           AvgServingTime;                             /* average serving time in milliseconds */
    ULONG                           TtcCount;                                   /* total number of time-out request     */
    ULONG                           MtcCount;                                   /* time-out request in the last minute  */
    ULONG                           TrcCount;                                   /* total number of clients rejected     */
    ULONG                           MrcCount;                                   /* clients rejected in the last minute  */
    ULONG                           TscCount;                                   /* total number of clients served       */
    ULONG                           MscCount;                                   /* clients served in the last minute    */
}
ANSC_BSTO_SNAPSHOT_FORM,  *PANSC_BSTO_SNAPSHOT_FORM;

/*
 * Close-up form reveals detailed information on each specific client socket request:
 *
 *      $ when was the connection established
 *      $ current system time
 *      $ the last recv_time from this client
 *      $ the last send_time to this client
 *      $ how many bytes been received
 *      $ how many bytes been sent
 */
#define  ANSC_MAX_BSTO_SOCKET_COUNT                 128

typedef  struct
_ANSC_BSTO_SOCKET_INFO
{
    ULONG                           StartTime;                                  /* system-up time in seconds     */
    ULONG                           CurrentTime;                                /* system-up time in seconds     */
    ULONG                           LastRecvAt;                                 /* system-up time in seconds     */
    ULONG                           LastSendAt;                                 /* system-up time in seconds     */
    ULONG                           RecvBytesCount;                             /* how many octets been received */
    ULONG                           SendBytesCount;                             /* how many octets been sent     */
}
ANSC_BSTO_SOCKET_INFO,  *PANSC_BSTO_SOCKET_INFO;

typedef  struct
_ANSC_BSTO_CLOSE_UP_FORM
{
    ULONG                           EngineIndex;                                /* index of the active engine */
    ULONG                           SocketCount;                                /* size of the socket array   */
    ANSC_BSTO_SOCKET_INFO           SocketArray[ANSC_MAX_BSTO_SOCKET_COUNT];    /* socket info array          */
}
ANSC_BSTO_CLOSE_UP_FORM,  *PANSC_BSTO_CLOSE_UP_FORM;


#endif
