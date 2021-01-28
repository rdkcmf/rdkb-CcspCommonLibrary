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

        For Dbus base library Implementation,
        Common Component Software Platform (CCSP)

    ---------------------------------------------------------------

    environment:

        platform dependent

    ---------------------------------------------------------------

    author:

        Qiang Tu

    ---------------------------------------------------------------

    revision:

        06/17/11    initial revision.

**********************************************************************/

#ifndef CCSP_MESSAGE_BUS_H
#define CCSP_MESSAGE_BUS_H

#include <dbus/dbus.h>
#include <pthread.h>

/*
notes: see readme.txt
*/

//match the error code in ccsp_base_api.h
#define CCSP_Message_Bus_OK             100
#define CCSP_Message_Bus_OOM            101
#define CCSP_Message_Bus_ERROR          102

#define CCSP_MESSAGE_BUS_CANNOT_CONNECT 190
#define CCSP_MESSAGE_BUS_TIMEOUT        191
#define CCSP_MESSAGE_BUS_NOT_EXIST      192
#define CCSP_MESSAGE_BUS_NOT_SUPPORT    193 //remote can't support this api

// resource limits
#define CCSP_MESSAGE_BUS_MAX_CONNECTION         15
#define CCSP_MESSAGE_BUS_MAX_FILTER             50
#define CCSP_MESSAGE_BUS_MAX_PATH               20

#define CCSP_MESSAGE_BUS_TIMEOUT_MAX_SECOND     60

#define WRITEID         "WRITEID"
#define PARAM_SIZE      "PARAM_SIZE"
#define SESSIONID       "SESSIONID"
#ifdef COMMIT
    #undef COMMIT
#endif
#define COMMIT          "COMMIT"
#define INVALID_PARAM   "INVALID_PARAM"
#define RESULT          "RESULT"
#define OBJNAME         "OBJNAME"
#define INST_NUM        "INST_NUM"
#define PRIORITY        "PRIORITY"
#define PARAM_NAME      "PARAM_NAME"
#define NEXT_LEVEL      "NEXT_LEVEL"


typedef void*(*CCSP_MESSAGE_BUS_MALLOC) ( size_t size ); // this signature is different from standard malloc
typedef void (*CCSP_MESSAGE_BUS_FREE)   ( void * ptr );

/*copy from ansc so we don't rely on ansc*/
typedef  struct
_CCSP_MSG_SINGLE_LINK_ENTRY
{
    struct  _CCSP_MSG_SINGLE_LINK_ENTRY*     Next;

} CCSP_MSG_SINGLE_LINK_ENTRY,  *PCCSP_MSG_SINGLE_LINK_ENTRY;

/*
 * special single linked list, the difference is that this one keeps track of the first element; we use this list as the
 * FIFO queue
 */
typedef  struct
_CCSP_MSG_QUEUE
{
    PCCSP_MSG_SINGLE_LINK_ENTRY               First;
    PCCSP_MSG_SINGLE_LINK_ENTRY               Last;

} CCSP_MSG_QUEUE,  *PCCSP_MSG_QUEUE;

typedef  struct
_CCSP_REQ_DESCRIPTOR
{
    CCSP_MSG_SINGLE_LINK_ENTRY               Linkage;
    DBusConnection  *conn;
    DBusMessage     *message;
    void            *user_data;

} CCSP_REQ_DESCRIPTOR,  *PCCSP_REQ_DESCRIPTOR;

#define  CCSP_MSG_ACCESS_CONTAINER(address, type, field)     \
         ((type*)((char*)(address) - (unsigned long)(&((type*)0)->field)))

typedef struct _CCSP_MESSAGE_PATH_INFO
{
    char *path;
//    DBusObjectPathMessageFunction callback;
    DBusObjectPathVTable echo_vtable;
    void *user_data;

} CCSP_MESSAGE_PATH_INFO;

typedef struct _CCSP_MESSAGE_FILTER
{
    char* path;
    char *interface;
    char *event;
    char *sender;
    int used;

} CCSP_MESSAGE_FILTER;

typedef struct _CCSP_MESSAGE_BUS_CONNECTION
{
    DBusConnection *conn;
    char address[256];
    pthread_mutex_t connect_mutex;
    int connected;
    void * bus_info_ptr;
    DBusLoop *loop;
    int self_pipe[2];
    pthread_mutex_t dispatch_mutex;
    int needs_dispatch;
    pthread_t loop_thread;
    pthread_t connect_thread;

} CCSP_MESSAGE_BUS_CONNECTION;

typedef struct _CCSP_MESSAGE_BUS_INFO
{
    char      component_id[256];
    DBusObjectPathMessageFunction   sig_callback;
    DBusObjectPathMessageFunction   default_sig_callback;
    void * user_data;
    int run;
    CCSP_MESSAGE_BUS_CONNECTION connection[CCSP_MESSAGE_BUS_MAX_CONNECTION];
    CCSP_MESSAGE_FILTER filters[CCSP_MESSAGE_BUS_MAX_FILTER];
    CCSP_MESSAGE_PATH_INFO path_array[CCSP_MESSAGE_BUS_MAX_PATH];
    pthread_mutex_t info_mutex;
    void * CcspBaseIf_func;
    CCSP_MESSAGE_BUS_MALLOC  mallocfunc ;
    CCSP_MESSAGE_BUS_FREE      freefunc ;
    
    pthread_mutex_t msg_mutex;
    pthread_cond_t  msg_threshold_cv;
    CCSP_MSG_QUEUE *msg_queue;
    DBusObjectPathMessageFunction thread_msg_func;
    int dbus_connect_thread_count;
    int dbus_loop_thread_count;
    void* rbus_callback;

} CCSP_MESSAGE_BUS_INFO;

typedef struct _CCSP_DEADLOCK_DETECTION_INFO
{
    pthread_mutex_t info_mutex;
    char *          messageType;
    void *          parameterInfo;
    unsigned long   size;
    unsigned long   enterTime;
    unsigned long   detectionDuration;
    unsigned long   timepassed;

} CCSP_DEADLOCK_DETECTION_INFO;

#define deadlock_detection_log_linenum 200
#define deadlock_detection_log_linelen 192 
#define deadlock_detection_log_file    "/var/log/ccsp_emergency.log"

typedef char DEADLOCK_ARRAY[deadlock_detection_log_linenum][deadlock_detection_log_linelen];

typedef struct _CCSP_MESSAGE_BUS_CB_DATA
{
    pthread_mutex_t count_mutex;
    pthread_cond_t count_threshold_cv;
    char*         response;
    DBusMessage *message;
    int succeed;

} CCSP_MESSAGE_BUS_CB_DATA;

// void ccsp_msg_check_resp_sync (DBusPendingCall *pcall, void *user_data); // local, RTian 07/03/2013

// EXTERNAL INTERFACES

void CCSP_Msg_SleepInMilliSeconds(int milliSecond);

int  CCSP_Msg_IsRbus_enabled(void);

/*if mallocfunc, freefunc,config_file is NULL, default value will be used */
int CCSP_Message_Bus_Init
(
    char*             component_id,
    char * config_file,
    void **bus_handle,
    CCSP_MESSAGE_BUS_MALLOC mallocfunc,
    CCSP_MESSAGE_BUS_FREE   freefunc
);

void CCSP_Message_Bus_Exit
(
    void *bus_handle
);

/*can be called multi-time, sender, path,interface,event_name at least 1 is not null*/
int  CCSP_Message_Bus_Register_Event
(
    void* bus_handle,
    const char* sender,
    const char* path,
    const char* interface,
    const char* event_name
);

int  CCSP_Message_Bus_UnRegister_Event
(
    void* bus_handle,
    const char* sender,
    const char* path,
    const char* interface,
    const char* event_name
);

void  CCSP_Message_Bus_Set_Event_Callback
(
    void* bus_handle,
    DBusObjectPathMessageFunction   callback,
    void * user_data
);

/*can register more than one time*/
#define CCSP_Message_Bus_Register_Path CCSP_Message_Bus_Register_Path2

int CCSP_Message_Bus_Register_Path2
(
    void* bus_handle,
    const char* path,
    DBusObjectPathMessageFunction funcptr,
    void * user_data
);

/*send on a connection, privite function*/
int  CCSP_Message_Bus_Send_Str
(
    DBusConnection *conn,
    char* component_id,
    const char* path,
    const char* interface,
    const char* method,
    char* request
);

int CCSP_Message_Bus_Send_Msg
(
    void* bus_handle,
    DBusMessage *message,
    int timeout_seconds,
    DBusMessage **result
);

/*not recommended  for multithread high speed app, it will crash */
int CCSP_Message_Bus_Send_Msg_Block
(
    void* bus_handle,
    DBusMessage *message,
    int timeout_seconds,
    DBusMessage **result
);

#define DBUS_REGISTER_PATH_TIMES 10

#endif

