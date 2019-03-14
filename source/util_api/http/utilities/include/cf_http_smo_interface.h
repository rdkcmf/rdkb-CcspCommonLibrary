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

    module:	cf_http_smo_interface.h

        For Content Filtering HTTP Session Management Object (SMO),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file includes all the header files required by
        Content Filtering HTTP Session Management Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan   

    ---------------------------------------------------------------

    revision:

        03/20/02    initial revision.

    ---------------------------------------------------------------

    last modified:  
    
**********************************************************************/

#ifndef  _CF_HTTP_SMO_INTERFACE_DEC
#define  _CF_HTTP_SMO_INTERFACE_DEC

#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"

/*
 * Define some const values that will be used in the object mapper object definition.
 */

#define  CF_HTTP_SMO_COMPONENT_NAME                 "CfHttpSmoComponent"
#define  CF_HTTP_SMO_COMPONENT_OID                  ANSC_OBJECT_OID_NULL
#define  CF_HTTP_SMO_COMPONENT_TYPE                 ANSC_OBJECT_TYPE_NULL

#define  CF_HTTP_SMO_SESSION_HASH_TABLE_SIZE        64

/* Session Error */
#define  CF_HTTP_SMO_SESSION_STATUS_BASE            0
#define  CF_HTTP_SMO_SESSION_NORMAL                 CF_HTTP_SMO_SESSION_STATUS_BASE + 0
#define  CF_HTTP_SMO_SESSION_BLOCK                  CF_HTTP_SMO_SESSION_STATUS_BASE + 1
#define  CF_HTTP_SMO_SESSION_NETWORK_ERROR          CF_HTTP_SMO_SESSION_STATUS_BASE + 2           

#define  CF_HTTP_SMO_TIMER_INTERVAL                 2000    /* 2000 milliseconds */
#define  CF_HTTP_SMO_SESSION_TIMEOUT                30      /* 30 seconds */
#define  CF_HTTP_SMO_SESSION_MAX_RETRY_TIMES        1       /* maximum retry times */

#define  CF_HTTP_SMO_SESSION_MIN_MONITOR_TIME       120     /* 120 seconds */

#define  CF_HTTP_SMO_SESSION_MIN_FAILURE_SESSIONS   32      /* minimal session count */

#define  CF_HTTP_SMO_SESSION_LINEAR_BUFFER_SIZE     64

typedef  struct
_CF_HTTP_SESSION_ENTITY
{
    SINGLE_LINK_ENTRY               Linkage;                
    ANSC_HANDLE                     hHttpTranRec;           /* Transcation Record object */
    ANSC_HANDLE                     Url;                    /* Request URL (absolute URI) */
    ULONG                           ID;                     /* identification assigned to this entity */
    ULONG                           ulHashIndex;            /* Hash table index */
    ULONG                           TimeStamp;              /* time stamp */
    ULONG                           ulRetryTimes;           /* retry times */
    PUCHAR                          pCategory;              /* set up when the page is blocked */
}
CF_HTTP_SESSION_ENTITY, *PCF_HTTP_SESSION_ENTITY;

#define  ACCESS_CF_HTTP_SESSION_ENTITY(p)                       \
         ACCESS_CONTAINER(p, CF_HTTP_SESSION_ENTITY, Linkage)

typedef
ANSC_HANDLE
(*PFN_CF_HTTP_SMO_ADD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTransRec,      /* Transacation Record Object */
        ANSC_HANDLE                 hMessage        /* Basic Message Object */
    );

typedef
void
(*PFN_CF_HTTP_SMO_REMOVE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTransRec       /* Transacation Record Object */
    );

typedef
ANSC_STATUS
(*PFN_CF_HTTP_SMO_RESUME)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulID
    );

typedef
ANSC_STATUS
(*PFN_CF_HTTP_SMO_TERMINATE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulID
    );

typedef
ANSC_STATUS
(*PFN_CF_HTTP_SMO_BLOCK)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulID,
        ULONG                       ulSessErrCode   /* Session error code */
    );

typedef
ANSC_HANDLE
(*PFN_CF_HTTP_SMO_GET_ENTITY)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulID
    );

typedef
ULONG
(*PFN_CF_HTTP_SMO_GET_ID)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_CF_HTTP_SMO_GET_REQ_URI)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTransRec,      /* Transacation Record Object */
        ANSC_HANDLE                 hMessage        /* Basic Message Object */
    );

typedef
void
(*PFN_CF_HTTP_SMO_CLEAR_TABLE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_CF_HTTP_SMO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef
ANSC_STATUS
(*PFN_CF_HTTP_SMO_CREATE_TIMER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_CF_HTTP_SMO_REMOVE_TIMER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_STATUS
(*PFN_CF_HTTP_SMO_START_TIMER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_STATUS
(*PFN_CF_HTTP_SMO_STOP_TIMER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_CF_HTTP_SMO_SET_LOG_FILE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hLogFile
    );

/*
 * As a standard practice, we encapsulate all functional and feature objects inside an Object
 * Container Object so it will function as a black-box implementation when we have to integrate
 * an Ansc component with other systems. However, that's not the only reason why we need to
 * define the Container Object: we also use container as the building block for constructing
 * larger systems.
 */
#define  CF_HTTP_SMO_CLASS_CONTENT                                                      \
    /* duplication of the base object class content */                                  \
    ANSCCO_CLASS_CONTENT                                                                \
    /* start of object class content */                                                 \
    ANSC_LOCK                       SessionAccessLock;                                  \
    ULONG                           NextSessionID;                                      \
    QUEUE_HEADER                    SessionList[CF_HTTP_SMO_SESSION_HASH_TABLE_SIZE];   \
    ANSC_HANDLE                     hContext;                                           \
    /* Timer Descriptor Object */                                                       \
    ANSC_HANDLE                     hTdo;                                               \
    /* Timer Descriptor Client Object */                                                \
    ANSC_HANDLE                     hTdoClient;                                         \
    ULONG                           LastFailureTime;                                    \
    ULONG                           ulFailureSessCount;                                 \
    BOOL                            bTaskExited;                                        \
    ANSC_HANDLE                     hLogFile;                                           \
    /* start of methods */                                                              \
    PFN_CF_HTTP_SMO_ADD             AddSession;                                         \
    PFN_CF_HTTP_SMO_REMOVE          RemoveSession;                                      \
    PFN_CF_HTTP_SMO_RESUME          ResumeSession;                                      \
    PFN_CF_HTTP_SMO_TERMINATE       TerminateSession;                                   \
    PFN_CF_HTTP_SMO_BLOCK           BlockSession;                                       \
    PFN_CF_HTTP_SMO_GET_ENTITY      GetSession;                                         \
    PFN_CF_HTTP_SMO_GET_ID          GetID;                                              \
    PFN_CF_HTTP_SMO_GET_REQ_URI     GetReqUri;                                          \
    PFN_CF_HTTP_SMO_CLEAR_TABLE     ClearSessionTable;                                  \
    PFN_CF_HTTP_SMO_SET_CONTEXT     SetContext;                                         \
    PFN_CF_HTTP_SMO_CREATE_TIMER    CreateTimer;                                        \
    PFN_CF_HTTP_SMO_REMOVE_TIMER    RemoveTimer;                                        \
    PFN_CF_HTTP_SMO_START_TIMER     StartTimer;                                         \
    PFN_CF_HTTP_SMO_STOP_TIMER      StopTimer;                                          \
    PFN_CF_HTTP_SMO_SET_LOG_FILE    SetLogFile;                                         \

/* content filtering HTTP Session Management Object */
typedef  struct
_CF_HTTP_SMO_OBJECT
{
    CF_HTTP_SMO_CLASS_CONTENT
}
CF_HTTP_SMO_OBJECT,    *PCF_HTTP_SMO_OBJECT;


#define  ACCESS_CF_HTTP_SMO_OBJECT(p)            \
         ACCESS_CONTAINER(p, CF_HTTP_SMO_OBJECT, Linkage)

#endif

