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

    module:	ansc_status.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        status codes.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        01/15/01    initial revision.
        01/19/01    restructure the container object, add and
                    remove a few things
        04/01/01    add user mode wrapper functions
        12/25/02    separated from ansc_wrapper_base.h

**********************************************************************/


#ifndef  _ANSC_STATUS_
#define  _ANSC_STATUS_


/***********************************************************
           DEFINITION OF CONTAINER/OBJECT STATUS
***********************************************************/

/*
 * We define some global status values that can be used by any objects.
 */
#define  ANSC_STATUS_SUCCESS                        0
#define  ANSC_STATUS_FAILURE                        0xFFFFFFFF
#define  ANSC_STATUS_RESOURCES                      ANSC_STATUS_FAILURE     - 1
#define  ANSC_STATUS_DISCARD                        ANSC_STATUS_FAILURE     - 2
#define  ANSC_STATUS_UNDETERMINED                   ANSC_STATUS_FAILURE     - 3
#define  ANSC_STATUS_PENDING                        ANSC_STATUS_FAILURE     - 4
#define  ANSC_STATUS_PASS_THROUGH                   ANSC_STATUS_FAILURE     - 5
#define  ANSC_STATUS_BAD_SIZE                       ANSC_STATUS_FAILURE     - 6
#define  ANSC_STATUS_BAD_OFFSET                     ANSC_STATUS_FAILURE     - 7
#define  ANSC_STATUS_BAD_MEDIA                      ANSC_STATUS_FAILURE     - 8
#define  ANSC_STATUS_BAD_NETWORK                    ANSC_STATUS_FAILURE     - 9
#define  ANSC_STATUS_UNAPPLICABLE                   ANSC_STATUS_FAILURE     - 10
#define  ANSC_STATUS_BLOCK_SESSION                  ANSC_STATUS_FAILURE     - 11
#define  ANSC_STATUS_RESET_SESSION                  ANSC_STATUS_FAILURE     - 12
#define  ANSC_STATUS_BAD_QUANTA                     ANSC_STATUS_FAILURE     - 13
#define  ANSC_STATUS_DONT_FRAGMENT                  ANSC_STATUS_FAILURE     - 14
#define  ANSC_STATUS_CANT_FIND                      ANSC_STATUS_FAILURE     - 15
#define  ANSC_STATUS_COLLISION                      ANSC_STATUS_FAILURE     - 16
#define  ANSC_STATUS_BAD_SEQ                        ANSC_STATUS_FAILURE     - 17
#define  ANSC_STATUS_BAD_AUTH_DATA                  ANSC_STATUS_FAILURE     - 18
#define  ANSC_STATUS_BAD_PAYLOAD                    ANSC_STATUS_FAILURE     - 19
#define  ANSC_STATUS_BAD_PADDING                    ANSC_STATUS_FAILURE     - 20
#define  ANSC_STATUS_UNMATCHED                      ANSC_STATUS_FAILURE     - 21
#define  ANSC_STATUS_NOT_READY                      ANSC_STATUS_FAILURE     - 22
#define  ANSC_STATUS_NO_MORE_DATA                   ANSC_STATUS_FAILURE     - 23
#define  ANSC_STATUS_MORE_DATA                      ANSC_STATUS_FAILURE     - 24
#define  ANSC_STATUS_NOT_COMPLETE                   ANSC_STATUS_FAILURE     - 25
#define  ANSC_STATUS_BAD_INDEX                      ANSC_STATUS_FAILURE     - 26
#define  ANSC_STATUS_NOT_SUPPORTED                  ANSC_STATUS_FAILURE     - 27
#define  ANSC_STATUS_DONT_WANT                      ANSC_STATUS_FAILURE     - 28
#define  ANSC_STATUS_CONSUMED                       ANSC_STATUS_FAILURE     - 29
#define  ANSC_STATUS_TIMEOUT                        ANSC_STATUS_FAILURE     - 30
#define  ANSC_STATUS_QUEUED                         ANSC_STATUS_FAILURE     - 31
#define  ANSC_STATUS_BAD_NAME                       ANSC_STATUS_FAILURE     - 32
#define  ANSC_STATUS_BAD_ADDR                       ANSC_STATUS_FAILURE     - 33
#define  ANSC_STATUS_BAD_TYPE                       ANSC_STATUS_FAILURE     - 34
#define  ANSC_STATUS_BAD_ID                         ANSC_STATUS_FAILURE     - 35
#define  ANSC_STATUS_BAD_COUNT                      ANSC_STATUS_FAILURE     - 36
#define  ANSC_STATUS_INTERNAL_ERROR                 ANSC_STATUS_FAILURE     - 37
#define  ANSC_STATUS_DO_IT_AGAIN                    ANSC_STATUS_FAILURE     - 38
#define  ANSC_STATUS_NOT_SPECIFIED                  ANSC_STATUS_FAILURE     - 39
#define  ANSC_STATUS_CONTINUE                       ANSC_STATUS_FAILURE     - 40
#define  ANSC_STATUS_ACCESS_DENIED                  ANSC_STATUS_FAILURE     - 41
#define  ANSC_STATUS_INVALID_HANDLE                 ANSC_STATUS_FAILURE     - 42
#define  ANSC_STATUS_NOT_EMPTY                      ANSC_STATUS_FAILURE     - 43
#define  ANSC_STATUS_BAD_REFERENCE                  ANSC_STATUS_FAILURE     - 44
#define  ANSC_STATUS_SHARING_VIOLATION              ANSC_STATUS_FAILURE     - 45
#define  ANSC_STATUS_BAD_PARAMETER                  ANSC_STATUS_FAILURE     - 46

#define  ANSC_STATUS_IPC_ERROR                      ANSC_STATUS_FAILURE     - 0xFFFF
#define  ANSC_STATUS_IPC_TRANSMITTER_UNAVAILABLE    ANSC_STATUS_IPC_ERROR   - 1
#define  ANSC_STATUS_IPC_DST_PORT_UNAVAILABLE       ANSC_STATUS_IPC_ERROR   - 2
#define  ANSC_STATUS_IPC_LINK_FAILURE               ANSC_STATUS_IPC_ERROR   - 3
#define  ANSC_STATUS_IPC_RECEIVER_UNAVAILABLE       ANSC_STATUS_IPC_ERROR   - 4
#define  ANSC_STATUS_IPC_INVALID_CODE               ANSC_STATUS_IPC_ERROR   - 5
#define  ANSC_STATUS_IPC_INVALID_OID                ANSC_STATUS_IPC_ERROR   - 6
#define  ANSC_STATUS_IPC_INVALID_RID                ANSC_STATUS_IPC_ERROR   - 7
#define  ANSC_STATUS_IPC_TRY_AGAIN                  ANSC_STATUS_IPC_ERROR   - 8
#define  ANSC_STATUS_IPC_UNAPPLICABLE               ANSC_STATUS_IPC_ERROR   - 9


#endif
