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

    module:	dslh_filemo_interface.h

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Dslh File Manager Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/16/05    initial revision.

**********************************************************************/


#ifndef  _DSLH_FILEMO_INTERFACE_
#define  _DSLH_FILEMO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "dslh_properties.h"

#include "dslh_ifo_ftm.h"


/***********************************************************
        DSLH FILE MANAGER COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_DSLHFILEMO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHFILEMO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_DSLHFILEMO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHFILEMO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ANSC_STATUS
(*PFN_DSLHFILEMO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_DSLHFILEMO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_DSLHFILEMO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHFILEMO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHFILEMO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHFILEMO_SETUP_ENV)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHFILEMO_CLOSE_ENV)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_DSLHFILEMO_GET_SESSION1)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pCommandKey
    );

typedef  ANSC_HANDLE
(*PFN_DSLHFILEMO_GET_SESSION2)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSessionType,
        ULONG                       ulFileType
    );

typedef  ANSC_STATUS
(*PFN_DSLHFILEMO_ADD_SESSION)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSessionType,
        ULONG                       ulFileType,
        char*                       pCommandKey
    );

typedef  ANSC_STATUS
(*PFN_DSLHFILEMO_DEL_SESSION)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pCommandKey
    );

typedef  ANSC_STATUS
(*PFN_DSLHFILEMO_DEL_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHFILEMO_ASYNC_TRANSFER)
    (
        ANSC_HANDLE                 hTransferReq
    );

typedef  ANSC_STATUS
(*PFN_DSLHFILEMO_QUEUE_REQ)
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pFtReq
    );

/*
 * If the CPE is instructed to perform a file transfer via the Download or Upload request from the
 * ACS, and if the file location is specified as an HTTP URL with the same host name as the ACS,
 * then the CPE MAY choose any of the following approaches in performing the transfer:
 *
 *      - The CPE MAY send the HTTP get/post over the already established connection.
 *        Once the file has been transferred, the CPE MAY then proceed in sending addi-
 *        tional messages to the ACS while continuing to maintain the connection.
 *      - The CPE MAY open a second connection over which to transfer the file, while
 *        maintaining the session to the ACS over which it may continue to send messages.
 *      - The CPE MAY terminate the session tot he ACS and then perform the transfer.
 *
 * If the file location is not an HTTP URL or is not in the same domain as the ACS, then only the
 * latter two options are available to it.
 */
#define  DSLH_FILE_MANAGER_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    DSLH_FILE_MANAGER_PROPERTY      Property;                                               \
    ANSC_HANDLE                     hDslhFtmIf;                                             \
    ANSC_HANDLE                     hHttpAcmIf;                                             \
    ANSC_HANDLE                     hDslhCpeController;                                     \
    ULONG                           AsyncTaskCount;                                         \
    BOOL                            bActive;                                                \
                                                                                            \
    QUEUE_HEADER                    TsoQueue;                                               \
    ANSC_LOCK                       TsoQueueLock;                                           \
                                                                                            \
    PFN_DSLHFILEMO_GET_IF           GetDslhFtmIf;                                           \
    PFN_DSLHFILEMO_GET_CONTEXT      GetDslhCpeController;                                   \
    PFN_DSLHFILEMO_SET_CONTEXT      SetDslhCpeController;                                   \
    PFN_DSLHFILEMO_GET_PROPERTY     GetProperty;                                            \
    PFN_DSLHFILEMO_SET_PROPERTY     SetProperty;                                            \
    PFN_DSLHFILEMO_RESET            ResetProperty;                                          \
    PFN_DSLHFILEMO_RESET            Reset;                                                  \
                                                                                            \
    PFN_DSLHFILEMO_ENGAGE           Engage;                                                 \
    PFN_DSLHFILEMO_CANCEL           Cancel;                                                 \
    PFN_DSLHFILEMO_SETUP_ENV        SetupEnv;                                               \
    PFN_DSLHFILEMO_CLOSE_ENV        CloseEnv;                                               \
                                                                                            \
    PFN_DSLHFILEMO_GET_SESSION1     GetTransferSession1;                                    \
    PFN_DSLHFILEMO_GET_SESSION2     GetTransferSession2;                                    \
    PFN_DSLHFILEMO_ADD_SESSION      AddTransferSession;                                     \
    PFN_DSLHFILEMO_DEL_SESSION      DelTransferSession;                                     \
    PFN_DSLHFILEMO_DEL_ALL          DelAllTransferSessions;                                 \
                                                                                            \
    PFN_DSLHFTMIF_DOWNLOAD          FtmDownload;                                            \
    PFN_DSLHFTMIF_UPLOAD            FtmUpload;                                              \
    PFN_DSLHFTMIF_GET_TRANS         FtmGetQueuedTransfers;                                  \
                                                                                            \
    /* end of object class content */                                                       \

typedef  struct
_DSLH_FILE_MANAGER_OBJECT
{
    DSLH_FILE_MANAGER_CLASS_CONTENT
}
DSLH_FILE_MANAGER_OBJECT,  *PDSLH_FILE_MANAGER_OBJECT;

#define  ACCESS_DSLH_FILE_MANAGER_OBJECT(p)         \
         ACCESS_CONTAINER(p, DSLH_FILE_MANAGER_OBJECT, Linkage)


/***********************************************************
       DSLH FILE MANAGER TRANSFER REQUEST DEFINITION
***********************************************************/

/*
 * Despite what's specified in TR-069 about file transfer, we always establish a dedicated session
 * to perform file transfer (download or upload) once requested. So both Download() and Upload()
 * calls will always be completed asynchronously.
 */
#define  DSLH_FILEMO_SESSION_TYPE_download             1
#define  DSLH_FILEMO_SESSION_TYPE_upload               2

#define  DSLH_FILEMO_SESSION_STATE_notStarted          1
#define  DSLH_FILEMO_SESSION_STATE_inProcess           2
#define  DSLH_FILEMO_SESSION_STATE_completed           3

typedef  void
(*PFN_DSLHFILEMO_TS_RMCTX)
    (
        void*                       pFtCtx
    );

typedef  struct
_DSLH_FILEMO_TRANSFER_SESSION
{
    SINGLE_LINK_ENTRY               Linkage;
    ULONG                           SessionType;
    ULONG                           FileType;
    char*                           CommandKey;
    ULONG                           State;

    void*                           pFtReq;
    void*                           pFtCtx;
    PFN_DSLHFILEMO_TS_RMCTX         pfnFreeFtCtx;
}
DSLH_FILEMO_TRANSFER_SESSION,  *PDSLH_FILEMO_TRANSFER_SESSION;

#define  ACCESS_DSLH_FILEMO_TRANSFER_SESSION(p)     \
         ACCESS_CONTAINER(p, DSLH_FILEMO_TRANSFER_SESSION, Linkage)

#define  DslhFilemoCleanTransferSession(trans_session)                                      \
         {                                                                                  \
            if ( trans_session->CommandKey )                                                \
            {                                                                               \
                AnscFreeMemory(trans_session->CommandKey);                                  \
                                                                                            \
                trans_session->CommandKey = NULL;                                           \
            }                                                                               \
                                                                                            \
            if ( trans_session->pFtCtx )                                                    \
            {                                                                               \
                if ( trans_session->pfnFreeFtCtx )                                          \
                    (*trans_session->pfnFreeFtCtx)(trans_session->pFtCtx);                  \
                trans_session->pFtCtx = NULL;                                               \
            }                                                                               \
         }

#define  DslhFilemoFreeTransferSession(trans_session)                                       \
         {                                                                                  \
            DslhFilemoCleanTransferSession(trans_session)                                   \
            AnscFreeMemory                (trans_session);                                  \
         }


#endif
