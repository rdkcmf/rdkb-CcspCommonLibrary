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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <dbus/dbus.h>
#include <ccsp_message_bus.h>
#include "ccsp_base_api.h"
#include "ccsp_trace.h"
#include <fcntl.h>
#include <rbus_message_bus.h>
#include "ansc_platform.h"
#include <dslh_definitions_database.h>
#include "ccsp_rbus_value_change.h"
#include "safec_lib_common.h"

#include <sys/time.h>
#include <time.h>

#ifdef _DEBUG
// #define _DEBUG_LOCAL_
#endif

/* Use a 20 second timeout waiting for message replies */
#define CCSP_MESSAGE_BUS_REPLY_TIMEOUT_SECONDS 20

/* Use a 2 second timeout for sending strings */
#define CCSP_MESSAGE_BUS_SEND_STR_TIMEOUT_SECONDS 2

/* Use a timeout waiting for threads to clean up during exit */
/* For initial connection, use a very conservative timeout since previously connect would wait forever */
#define CCSP_MESSAGE_BUS_CONNECT_TIMEOUT_SECONDS 300
#define CCSP_MESSAGE_BUS_DISCONNECT_TIMEOUT_SECONDS 60

/* For each processing loop user a 1.03 second timeout */
#define CCSP_MESSAGE_BUS_PROCESSING_TIMEOUT_SECONDS 1
#define CCSP_MESSAGE_BUS_PROCESSING_TIMEOUT_NANOSECONDS 30000000

/* When we need to wake the mainloop, try it every 50ms */
#define CCSP_MESSAGE_MAINLOOP_WAKE_RETRY_MS 50

// EXTERNAL
extern void dbus_connection_lock (DBusConnection *connection);
extern void dbus_connection_unlock (DBusConnection *connection);

extern CCSP_DEADLOCK_DETECTION_INFO deadlock_detection_info;
extern int   CcspBaseIf_timeout_protect_plus_seconds;
extern int   CcspBaseIf_deadlock_detection_time_normal_seconds;
extern int   CcspBaseIf_deadlock_detection_time_getval_seconds;
extern int   CcspBaseIf_timeout_seconds;
extern int   CcspBaseIf_timeout_getval_seconds;
extern int   deadlock_detection_enable;
extern DEADLOCK_ARRAY*  deadlock_detection_log;
extern void* CcspBaseIf_Deadlock_Detection_Thread(void *);

// GLOBAL VAR
static int ccsp_bus_ref_count = 0;
static pthread_t thread_dbus_process = 0;
static pthread_t thread_dbus_deadlock_monitor = 0;

// TYPE DEF
typedef struct
{
    DBusLoop *loop;
    DBusConnection *connection;

} CData;

// FUNCTION PROTOCOL
// internal functions

inline static int                           CcspMsgQueueInit(CCSP_MSG_QUEUE *Queue);
inline static int                           CcspMsgQueuePushEntry(CCSP_MSG_QUEUE *Queue, CCSP_MSG_SINGLE_LINK_ENTRY *Entry);
inline static PCCSP_MSG_SINGLE_LINK_ENTRY   CcspMsgQueuePopEntry(CCSP_MSG_QUEUE *Queue); 

/*static dbus_bool_t       connection_watch_callback(DBusWatch*, unsigned int, void*);*/
static dbus_bool_t       add_watch(DBusWatch*, void*);
static void              remove_watch(DBusWatch*, void*);
static void              toggle_watch(DBusWatch*, void*);
/*static void              connection_timeout_callback(DBusTimeout*, void*);*/
static dbus_bool_t       add_timeout(DBusTimeout*, void*);
static void              remove_timeout(DBusTimeout*, void*);
static CData*            cdata_new(DBusLoop*, DBusConnection*);
static void              cdata_free(void *data);
static void              path_unregistered_func(DBusConnection*, void*);
static void              dispatch_status_func(DBusConnection*, DBusDispatchStatus, void*);
static DBusHandlerResult thread_path_message_func(DBusConnection*, DBusMessage*, void*);
static void              ccsp_msg_bus_reconnect(CCSP_MESSAGE_BUS_CONNECTION *);
static DBusHandlerResult filter_func(DBusConnection*, DBusMessage*, void*);
static dbus_bool_t       ccsp_connection_setup(DBusLoop*, CCSP_MESSAGE_BUS_CONNECTION*);
static void              ccsp_msg_check_resp_sync(DBusPendingCall*, void*);
static void              CCSP_Message_Bus_Strip(char * str);
static void*             CCSP_Message_Bus_Loop_Thread(void * DBusLoopPtr);
static void*             CCSP_Message_Bus_Connect_Thread(void * ccsp_msg_bus_connection_ptr);
static void*             CCSP_Message_Bus_Process_Thread(void * ccsp_bus_info_ptr);
static void              append_event_info(char*, const char*, const char*, const char*, const char*);
static int               CCSP_Message_Bus_Register_Event_Priv(DBusConnection*, const char*, const char*, const char*, const char*, int);
static int               CCSP_Message_Save_Register_Event(void*, const char*, const char*, const char*, const char*);
static int               CCSP_Message_Bus_Register_Path_Priv(void*, const char*, DBusObjectPathMessageFunction, void*);
static int               CCSP_Message_Bus_Register_Path_Priv_rbus(void*, rbus_callback_t, void*);
static int               thread_path_message_func_rbus(const char * destination, const char * method, rbusMessage in, void * user_data, rbusMessage *out, const rtMessageHeader* hdr);
static int               analyze_reply(DBusMessage*, DBusMessage*, DBusMessage**);
static DBusWakeupMainFunction wake_mainloop(void *);
static int tunnelStatus_signal_rbus(const char * destination, const char * method, rbusMessage request, void * user_data, rbusMessage *response, const rtMessageHeader* hdr);
static int webcfg_signal_rbus (const char * destination, const char * method, rbusMessage request, void * user_data, rbusMessage *response, const rtMessageHeader* hdr);
static int wifiDbStatus_signal_rbus(const char * destination, const char * method, rbusMessage request, void * user_data, rbusMessage *response, const rtMessageHeader* hdr);
static int telemetry_send_signal_rbus(const char * destination, const char * method, rbusMessage request, void * user_data, rbusMessage *response, const rtMessageHeader* hdr);
static int cssp_event_subscribe_override_handler_rbus(char const* object,  char const* eventName, char const* listener, int added, const rbusMessage payload, void* userData);
int rbus_enabled = 0;

// External Interface, defined in ccsp_message_bus.h
/*
void CCSP_Msg_SleepInMilliSeconds(int milliSecond);
int  CCSP_Message_Bus_Init(char*, char*, void**, CCSP_MESSAGE_BUS_MALLOC, CCSP_MESSAGE_BUS_FREE);
void CCSP_Message_Bus_Exit(void *bus_handle);
int  CCSP_Message_Bus_Register_Event(void*, const char*, const char*, const char*, const char*);
int  CCSP_Message_Bus_UnRegister_Event(void*, const char*, const char*, const char*, const char*);
void CCSP_Message_Bus_Set_Event_Callback(void*, DBusObjectPathMessageFunction, void*);
#define CCSP_Message_Bus_Register_Path CCSP_Message_Bus_Register_Path2
int  CCSP_Message_Bus_Register_Path2(void*, const char*, DBusObjectPathMessageFunction, void*);
int  CCSP_Message_Bus_Send_Str(DBusConnection*, char*, const char*, const char*, const char*, char*);
int  CCSP_Message_Bus_Send_Msg(void*, DBusMessage*, int, DBusMessage**);
int  CCSP_Message_Bus_Send_Msg_Block(void*, DBusMessage*, int, DBusMessage**);
*/

// IMPLEMENTATION

/* Helper function to calculate a timeout based on monotonic clock if available */
inline
static void NewTimeout
(
    struct timespec * timeout,
    time_t secs, 
    long nsecs
)
{
    /* Non-WIN32 implementation */
#if !defined(CLOCK_MONOTONIC)
    struct timeval now;
    gettimeofday(&now, NULL);
    timeout->tv_sec = now.tv_sec + secs;
    timeout->tv_nsec = (now.tv_usec * 1000) + nsecs;
    
#else
    clock_gettime(CLOCK_MONOTONIC, timeout);
    timeout->tv_sec += secs;
    timeout->tv_nsec += nsecs;
#endif 


    /* Take care of rollover */
    if (timeout->tv_nsec > 1000000000)
    {
        timeout->tv_nsec -= 1000000000;
        timeout->tv_sec += 1;
    }
}

/* Helper to create condition variables using monotonic clock if available */
static int NewCondVar
(
   pthread_cond_t * cv
)
{
#if defined(CLOCK_MONOTONIC)
    pthread_condattr_t attr;
    pthread_condattr_init (&attr);
    if ( 0 != pthread_condattr_setclock(&attr, CLOCK_MONOTONIC))
    {
        CcspTraceError(("<%s>: Couldn't select monotonic clock for condition variable!\n", __FUNCTION__));
        pthread_condattr_destroy(&attr);
        return -1;
    }
    pthread_cond_init (cv, &attr);
    pthread_condattr_destroy(&attr);
#else
    pthread_cond_init (cv, NULL);
#endif
    return 0;
}

inline 
static int                           
CcspMsgQueueInit
(
    CCSP_MSG_QUEUE *Queue
)
{
    if (!Queue) 
    { 
        CcspTraceError(("%d <%s> Queue is NULL\n", getpid(), __FUNCTION__));
        return -1; 
    }

    (Queue)->First = (Queue)->Last = NULL;      
    return 0;
}

inline 
static int                           
CcspMsgQueuePushEntry
(
    CCSP_MSG_QUEUE *Queue, 
    CCSP_MSG_SINGLE_LINK_ENTRY *Entry
)
{
    if(!Queue || !Entry) 
    { 
        CcspTraceError(("%d <%s> Queue and/or Entry is NULL\n", getpid(), __FUNCTION__));
        return -1;
    }

    (Entry)->Next = NULL;                                              
    if ( (Queue)->Last == NULL )                                        
    {                                                               
        (Queue)->First = (Entry);                                       
    }                                                                   
    else                                                                
    {                                                               
        ((Queue)->Last)->Next = (Entry);                                
    }                                                                                                                                                               
    (Queue)->Last = (Entry);                                            
    return 0;
}

inline 
static PCCSP_MSG_SINGLE_LINK_ENTRY 
CcspMsgQueuePopEntry
(
    CCSP_MSG_QUEUE *Queue
) 
{
    if(!Queue) 
    {  
        CcspTraceError(("%d <%s> Queue is NULL\n", getpid(), __FUNCTION__));
        return NULL;
    }

    PCCSP_MSG_SINGLE_LINK_ENTRY  head = (Queue)->First;
    if ( head != NULL )
    {
        (Queue)->First = head->Next;

        if ((Queue)->First == NULL)
        {
            (Queue)->Last = NULL;
        }
    }
    return head;
}

//unused function
#if 0
static dbus_bool_t 
connection_watch_callback 
(
    DBusWatch     *watch,
    unsigned int   condition,
    void          *data
)
{
    UNREFERENCED_PARAMETER(data);
    return dbus_watch_handle (watch, condition);
}
#endif

static dbus_bool_t 
add_watch
(
    DBusWatch *watch,
    void      *data
)
{
    CData *cd = data;
    
    return dbus_loop_add_watch
               (
                   cd->loop,
                   watch
                );
}

static void 
remove_watch
(
    DBusWatch *watch,
    void      *data
)
{
    CData *cd = data;
    
    dbus_loop_remove_watch
        (
             cd->loop,
             watch
         );
}

static void
toggle_watch
(
    DBusWatch *watch,
    void      *data
)
{
    CData *cd = data;

    dbus_loop_toggle_watch
        (
             cd->loop,
             watch
         );
}
//unused function
#if 0
static void 
connection_timeout_callback 
(
    DBusTimeout   *timeout,
    void          *data
)
{
    UNREFERENCED_PARAMETER(data);
    /* Can return FALSE on OOM but we just let it fire again later */
    dbus_timeout_handle (timeout);
}
#endif

static dbus_bool_t 
add_timeout 
(
    DBusTimeout *timeout,
    void        *data
)
{
    CData *cd = data;

    return dbus_loop_add_timeout
               (
                   cd->loop,
                   timeout
                );
}

static void 
remove_timeout 
(
    DBusTimeout *timeout,
    void        *data
)
{
    CData *cd = data;

    dbus_loop_remove_timeout
        (
            cd->loop,
            timeout
         );
}

static CData* 
cdata_new 
(
    DBusLoop       *loop,
    DBusConnection *connection
)
{
    CData *cd = NULL;

    cd = dbus_new0 (CData, 1);
    if (cd == NULL) return NULL;

    cd->loop = loop;
    cd->connection = connection;
    
    dbus_connection_ref (cd->connection);
    dbus_loop_ref (cd->loop);

    return cd;
}

static void 
cdata_free 
(
    void *data
)
{
    CData *cd = data;

    dbus_loop_remove_wake(cd->loop);
    dbus_connection_unref (cd->connection);
    dbus_loop_unref (cd->loop);

    dbus_free (cd);
}

/*
static unsigned int
UserGetTickInMilliSeconds2()
{
    struct timeval                  tv = {0};

    gettimeofday(&tv, NULL);

    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
};
*/

static void 
path_unregistered_func 
(
    DBusConnection  *connection,
    void            *user_data
)
{
    UNREFERENCED_PARAMETER(connection);
    UNREFERENCED_PARAMETER(user_data);
    /* connection was finalized */
}

static void 
dispatch_status_func
(
    DBusConnection    *conn,
    DBusDispatchStatus new_status,
    void              *data
)
{
    UNREFERENCED_PARAMETER(conn);
    CCSP_MESSAGE_BUS_CONNECTION *connection = (CCSP_MESSAGE_BUS_CONNECTION *)data;

    if (new_status != DBUS_DISPATCH_COMPLETE)
    {
        pthread_mutex_lock(&connection->dispatch_mutex);
        connection->needs_dispatch = 1;
        pthread_mutex_unlock(&connection->dispatch_mutex);
    }
}

static DBusHandlerResult
thread_path_message_func 
(
    DBusConnection  *conn,
    DBusMessage     *message,
    void            *user_data
)
{
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)user_data;
    
    //push to a queue, signal the processing thread, then return immediately
    CCSP_REQ_DESCRIPTOR *req_rec = (CCSP_REQ_DESCRIPTOR *)bus_info->mallocfunc(sizeof(CCSP_REQ_DESCRIPTOR));  
    if(req_rec)
    {
        req_rec->conn = conn;
        req_rec->message = message;
        req_rec->user_data = user_data;
        dbus_message_ref (message);

        pthread_mutex_lock(&bus_info->msg_mutex);
        CcspMsgQueuePushEntry(bus_info->msg_queue,&(req_rec->Linkage));
        pthread_cond_signal(&bus_info->msg_threshold_cv);
        pthread_mutex_unlock(&bus_info->msg_mutex);
    }  

    return DBUS_HANDLER_RESULT_HANDLED;
}

static void 
ccsp_msg_bus_reconnect
(
    CCSP_MESSAGE_BUS_CONNECTION *connection
) 
{
    CCSP_MESSAGE_BUS_INFO *bus_info =(CCSP_MESSAGE_BUS_INFO *) connection->bus_info_ptr;

    int rc = 0;
    struct timespec timeout = { 0, 0 };

    pthread_mutex_lock(&bus_info->msg_mutex);
    /*
       Wait for any currently running CCSP_Message_Bus_Connect_Thread threads
       to terminate. If there are any threads running, it's because of multiple
       calls to this reconnect function (since CCSP_Message_Bus_Init will wait
       for any connect threads that it starts to terminate before continuing).
    */

    if (bus_info->dbus_connect_thread_count > 0) {
        NewTimeout(&timeout, CCSP_MESSAGE_BUS_CONNECT_TIMEOUT_SECONDS, 0);
        while (bus_info->run && (bus_info->dbus_connect_thread_count > 0) && (rc == 0))
            rc = pthread_cond_timedwait(&bus_info->msg_threshold_cv, &bus_info->msg_mutex, &timeout);
    }
    if (rc != 0) {
        CcspTraceError(("<%s>: error %d waiting for dbus_connect_thread_count to return to 0\n", __FUNCTION__, rc));
    }

    /* Check if the connection is now re-established by another thread */
    if (dbus_connection_get_is_connected(connection->conn))
    {
        pthread_mutex_unlock(&bus_info->msg_mutex);
        return;
    }
    else
    {
        CcspTraceError(("<%s> not dbus_connection_get_is_connected \n", __FUNCTION__));
    }

    /* Now that we're sure the connection is disconnected, flag it as such */
    pthread_mutex_lock(&bus_info->info_mutex);
    connection->connected = 0;
    pthread_mutex_unlock(&bus_info->info_mutex);

    CcspTraceWarning(("<%s>: Re-establishing connection...\n", __FUNCTION__));
    bus_info->dbus_connect_thread_count++;
    pthread_create(&connection->connect_thread, NULL, CCSP_Message_Bus_Connect_Thread, (void *)connection); 
    CcspTraceWarning(("Ok.\n"));

    /* Now wait for the reconnect to finish */
    if (bus_info->dbus_connect_thread_count > 0) {
        NewTimeout(&timeout, CCSP_MESSAGE_BUS_CONNECT_TIMEOUT_SECONDS, 0);
        while (bus_info->run && (bus_info->dbus_connect_thread_count > 0) && (rc == 0))
            rc = pthread_cond_timedwait(&bus_info->msg_threshold_cv, &bus_info->msg_mutex, &timeout);
    }
    if (rc != 0) {
        CcspTraceError(("<%s>: error %d waiting for dbus_connect_thread_count to return to 0\n", __FUNCTION__, rc));
    }
    pthread_mutex_unlock(&bus_info->msg_mutex);

    return;
}

static DBusHandlerResult 
filter_func 
(
    DBusConnection     *conn,
    DBusMessage        *message,
    void               *user_data
)
{
    CCSP_MESSAGE_BUS_CONNECTION *connection = (CCSP_MESSAGE_BUS_CONNECTION *)user_data;
    CCSP_MESSAGE_BUS_INFO *bus_info =(CCSP_MESSAGE_BUS_INFO *) connection->bus_info_ptr;
    
    switch (dbus_message_get_type (message)) 
    {
    case DBUS_MESSAGE_TYPE_SIGNAL:
        
        if (dbus_message_is_signal (message,
                                    DBUS_INTERFACE_LOCAL,
                                    "Disconnected"))
        {
            // This is normal at process exit
        
            // CcspTraceDebug(("<%s>: Signal received: Bus disconnected!\n", __FUNCTION__));
            
            if (bus_info) {
                
                
                if(bus_info->run)
                {
                    // This is not normal
                    CcspTraceError(("<%s>: Signal received: Bus disconnected!\n", __FUNCTION__));
                    ccsp_msg_bus_reconnect(connection); 
                }
            }
        }
        else
        {
            if(bus_info->sig_callback)
                thread_path_message_func(conn, message, bus_info);
        }

        return DBUS_HANDLER_RESULT_HANDLED;
        break;

    default:
        return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
        break;
    }
}

static dbus_bool_t 
ccsp_connection_setup 
(
    DBusLoop       *loop,
    CCSP_MESSAGE_BUS_CONNECTION *connection
)
{
    CData *cd = NULL;

    // dispatch_status, returns, or will wait for memory
    dbus_connection_set_dispatch_status_function 
        (
            connection->conn, 
            dispatch_status_func,
            connection, 
            NULL
         );

    // watch
    cd = cdata_new (loop, connection->conn);
    if (cd == NULL) goto NO_MEM;

    if ( ! dbus_connection_set_watch_functions 
               (
                   connection->conn,
                   add_watch,
                   remove_watch,
                   toggle_watch,
                   cd, 
                   cdata_free
                ))
        goto NO_MEM;

    // timeout
    cd = cdata_new (loop, connection->conn);
    if (cd == NULL) goto NO_MEM;
    
    if ( ! dbus_connection_set_timeout_functions 
               (
                   connection->conn,
                   add_timeout,
                   remove_timeout,
                   NULL,
                   cd, 
                   cdata_free
                ))
        goto NO_MEM;

    // check dispatch status
    if (dbus_connection_get_dispatch_status (connection->conn) != DBUS_DISPATCH_COMPLETE)
    {
        if ( ! dbus_loop_queue_dispatch (loop, connection->conn))
            goto NO_MEM;
    }

    return TRUE;

NO_MEM:
    if (cd) cdata_free (cd);

    // set everything to NULL
    dbus_connection_set_dispatch_status_function (connection->conn, NULL, NULL, NULL);
    dbus_connection_set_watch_functions (connection->conn, NULL, NULL, NULL, NULL, NULL);
    dbus_connection_set_timeout_functions (connection->conn, NULL, NULL, NULL, NULL, NULL);

    return FALSE;
}

static void
ccsp_msg_check_resp_sync 
(
    DBusPendingCall *pcall,
    void *user_data
)
{
    UNREFERENCED_PARAMETER(pcall);
    CCSP_MESSAGE_BUS_CB_DATA *cb_data = (CCSP_MESSAGE_BUS_CB_DATA *)user_data;

    pthread_mutex_lock(&cb_data->count_mutex);
    pthread_cond_signal(&cb_data->count_threshold_cv);
    pthread_mutex_unlock(&cb_data->count_mutex);
}


static void 
CCSP_Message_Bus_Strip
(
    char * str
)
{
    while(*str)
    {
        if(*str == 0xa || * str == 0xd)
        {
            *str = 0;
            break;
        }
        str++;
    }
}

static void *
CCSP_Message_Bus_Loop_Thread
(
    void * connptr
)
{
    CCSP_MESSAGE_BUS_CONNECTION *connection = (CCSP_MESSAGE_BUS_CONNECTION *)connptr;
    CCSP_MESSAGE_BUS_INFO *bus_info =(CCSP_MESSAGE_BUS_INFO *) connection->bus_info_ptr;
    DBusLoop *loop = (DBusLoop *)connection->loop;

    pthread_detach(pthread_self());

    while (bus_info->run)
    {
        /* Flush any queued outgoing messages */
        dbus_connection_flush(connection->conn);

        /* Run an iteration of the mainloop */
        dbus_loop_iterate (loop, TRUE);

        /* Queue dispatch if necessary */
        pthread_mutex_lock(&connection->dispatch_mutex);
        if (connection->needs_dispatch)
        {
            while (!dbus_loop_queue_dispatch (loop, connection->conn))
                dbus_wait_for_memory ();

            /* Unset the flag */
            connection->needs_dispatch = 0;    
        }
        pthread_mutex_unlock(&connection->dispatch_mutex);

    }

    // signal that this thread is done
    pthread_mutex_lock(&bus_info->msg_mutex);
    if (bus_info->dbus_loop_thread_count > 0)
    {
        bus_info->dbus_loop_thread_count--;
    }
    else
    {
        CcspTraceError(("<%s> unexpected bus_info->dbus_loop_thread_count (%d)!\n", __FUNCTION__, bus_info->dbus_loop_thread_count));
    }
    pthread_cond_signal(&bus_info->msg_threshold_cv);
    pthread_mutex_unlock(&bus_info->msg_mutex);

    return NULL;
}

static void *
CCSP_Message_Bus_Connect_Thread
(
    void * connptr
)
{
    CCSP_MESSAGE_BUS_CONNECTION *connection = (CCSP_MESSAGE_BUS_CONNECTION *)connptr;
    CCSP_MESSAGE_BUS_INFO *bus_info =(CCSP_MESSAGE_BUS_INFO *) connection->bus_info_ptr;
    DBusError error;
    DBusConnection *conn_new = NULL;
    DBusConnection *conn_old = NULL;
    int ret = 0;
    int i = 0;
    int ct = 0;	

    pthread_detach(pthread_self());

    //    CcspTraceDebug(("<%s> connect started\n", __FUNCTION__));

    while(bus_info->run) 
    {
        // uses "break" at the end to get out of this while loop 

        dbus_error_init (&error);
        conn_new = dbus_connection_open_private (connection->address, &error);
        if(conn_new == NULL)
        {
	    ct ++;
            if(ct > 20) ct = 0;
            dbus_error_free (&error);
            //            CCSP_Msg_SleepInMilliSeconds(3000);
            CCSP_Msg_SleepInMilliSeconds(200);
            connection->conn = NULL;
            continue;
        }
        
        // save new connection
        conn_old = connection->conn;
        connection->conn = conn_new;

        if ( ! ccsp_connection_setup (connection->loop, connection))
        {
            CcspTraceError(("<%s> Couldn't ccsp_connection_setup loop!\n", __FUNCTION__));
            dbus_error_free (&error);
            dbus_connection_close(conn_new);
            dbus_connection_unref (conn_new);
            connection->conn = conn_old;
            continue;
        }

        if ( ! dbus_bus_register (conn_new, &error))
        {
            CcspTraceError(("<%s> Failed to register connection to bus at %s: %s\n",
                            __FUNCTION__, connection->address, error.message));
            dbus_error_free (&error);
            dbus_connection_close(conn_new);
            dbus_connection_unref (conn_new);
            connection->conn = conn_old;
            continue;
        }

        if(strlen(bus_info->component_id))
        {
            ret = dbus_bus_request_name 
                      (
                          conn_new, 
                          bus_info->component_id,
                          DBUS_NAME_FLAG_ALLOW_REPLACEMENT|DBUS_NAME_FLAG_REPLACE_EXISTING|DBUS_NAME_FLAG_DO_NOT_QUEUE,
                          &error
                       );

            if (dbus_error_is_set (&error))
	    {
                CcspTraceError(
                    (
                        "<%s>"
                        "Failed to request name %s:"
                        " ret=%d,"
                        " error=%s\n",
                        __FUNCTION__, 
                        bus_info->component_id, 
                        ret, 
                        error.message
                     ));
                dbus_error_free (&error);
                dbus_connection_close(conn_new);
                dbus_connection_unref (conn_new);
                connection->conn = conn_old;
                continue;
            }

            if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER && 
                ret != DBUS_REQUEST_NAME_REPLY_ALREADY_OWNER)
	    {
                CcspTraceError(
                    (
                        "<%s>"
                        "Request name returned %d:"
                        "someone already owns the name %s \n", 
                        __FUNCTION__, 
                        ret, 
                        bus_info->component_id
                     ));
                dbus_error_free (&error);
                dbus_connection_close(conn_new);
                dbus_connection_unref (conn_new);
                //                CCSP_Msg_SleepInMilliSeconds(3000);
                CCSP_Msg_SleepInMilliSeconds(200);
                connection->conn = conn_old;
                continue;
            }
        }

        for(i = 0; i < CCSP_MESSAGE_BUS_MAX_PATH; i++)
        {
            if(bus_info->path_array[i].path != NULL)
            {
                dbus_connection_try_register_object_path
                    (
                        conn_new,
                        bus_info->path_array[i].path,
                        &bus_info->path_array[i].echo_vtable,
                        bus_info->path_array[i].user_data,
                        NULL
                     );
            }
        }


        for(i = 0; i < CCSP_MESSAGE_BUS_MAX_FILTER; i++)
        {
            if(bus_info->filters[i].event != NULL)
            {
                CCSP_Message_Bus_Register_Event_Priv
                    (
                        conn_new,
                        bus_info->filters[i].sender, 
                        bus_info->filters[i].path, 
                        bus_info->filters[i].interface, 
                        bus_info->filters[i].event,
                        1
                     );
            }
        }

        if ( ! dbus_connection_add_filter (conn_new, filter_func, connection, NULL))
        {
            CcspTraceError(("<%s> Couldn't add filter!\n", __FUNCTION__));
            dbus_error_free (&error);
            dbus_connection_close(conn_new);
            dbus_connection_unref (conn_new);
            connection->conn = conn_old;
            continue;
        }

        // Everything is ok
        dbus_error_free (&error);
        break;
    }

    pthread_mutex_lock(&bus_info->info_mutex);
    connection->connected = 1;
    pthread_mutex_unlock(&bus_info->info_mutex);

    if(conn_old)
    {
        dbus_connection_close(conn_old);
        // RTian 5/3/2013        CCSP_Msg_SleepInMilliSeconds(2000);
        dbus_connection_unref(conn_old);
    }

    // signal the waiting call in Bus_Init or Reconnect
    pthread_mutex_lock(&bus_info->msg_mutex);

    if (bus_info->dbus_connect_thread_count > 0)
        bus_info->dbus_connect_thread_count--;
    else
        CcspTraceError(("<%s> unexpected bus_info->dbus_connect_thread_count (%d)!\n", __FUNCTION__, bus_info->dbus_connect_thread_count));

    pthread_cond_signal(&bus_info->msg_threshold_cv);
    pthread_mutex_unlock(&bus_info->msg_mutex);

    return NULL;
}

static void *
CCSP_Message_Bus_Process_Thread
(
    void * user_data
)
{
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)user_data;
    PCCSP_MSG_SINGLE_LINK_ENTRY entry = NULL;
    CCSP_REQ_DESCRIPTOR *req_rec = NULL;

    struct timespec timeout = { 0, 0 };
    
    struct timespec curTime = { 0, 0 };
    struct timespec preTime = { 0, 0 };
    int ret;

    NewTimeout(&preTime, 0, 0);

    while(bus_info->run)
    {
        // block the loop to save CPU time
        pthread_mutex_lock(&bus_info->msg_mutex);        

        NewTimeout(&timeout, CCSP_MESSAGE_BUS_PROCESSING_TIMEOUT_SECONDS, CCSP_MESSAGE_BUS_PROCESSING_TIMEOUT_NANOSECONDS);

	/*CID: 64258 Unchecked return value*/
        ret = pthread_cond_timedwait(&bus_info->msg_threshold_cv, &bus_info->msg_mutex, &timeout);
	/* timeouts are normal here, only log real errors */
	if ((ret != 0) && (ret != ETIMEDOUT))
	{
	     CcspTraceDebug(("<%s>: pthread_cond_timedwait ERROR %d!!!\n", __FUNCTION__, ret));
	}

        // now the processing, after either received a signal or timed out
        entry = CcspMsgQueuePopEntry(bus_info->msg_queue);
        while(bus_info->run && entry)
        {
            pthread_mutex_unlock(&bus_info->msg_mutex);
	    
            req_rec = CCSP_MSG_ACCESS_CONTAINER(entry,CCSP_REQ_DESCRIPTOR,Linkage);
            if(dbus_message_get_type(req_rec->message) == DBUS_MESSAGE_TYPE_SIGNAL)
                bus_info->sig_callback(req_rec->conn, req_rec->message, req_rec->user_data); 
            else if(bus_info->thread_msg_func)
                bus_info->thread_msg_func(req_rec->conn, req_rec->message, req_rec->user_data);             
            dbus_message_unref(req_rec->message);
            bus_info->freefunc(req_rec);

            pthread_mutex_lock(&bus_info->msg_mutex);
            entry = CcspMsgQueuePopEntry(bus_info->msg_queue);
        }
        pthread_mutex_unlock(&bus_info->msg_mutex);

        /* We leverage this pthread to check dbus connection. */
        NewTimeout(&curTime, 0, 0);
        if ( bus_info->run && ( ( curTime.tv_sec - preTime.tv_sec ) > CCSP_MESSAGE_BUS_TIMEOUT_MAX_SECOND )  && ( ( curTime.tv_sec - preTime.tv_sec ) < ( 10 * CCSP_MESSAGE_BUS_TIMEOUT_MAX_SECOND ) ) ){
            CcspTraceWarning(("<%s> !!!!PSM mode switching happened. Send singal to check dbus connection\n", __FUNCTION__));
            CcspBaseIf_SendsystemKeepaliveSignal(bus_info);
        }
        preTime = curTime; 
    }   

    return NULL;
}

void 
CCSP_Msg_SleepInMilliSeconds
(
    int milliSecond
)
{
    struct timeval tm;
    tm.tv_sec = milliSecond/1000;
    tm.tv_usec = (milliSecond%1000)*1000;
    select(0, NULL, NULL, NULL, &tm);
}

int
CCSP_Msg_IsRbus_enabled
(
void
)
{
    if (0 == rbus_enabled)
    {
        rbus_enabled = (RBUS_ENABLED == rbus_checkStatus()) ? 1 : 0;
    }

    CcspTraceWarning(("%s is enabled\n", rbus_enabled ? "RBus" : "DBus"));
    return rbus_enabled;
}

void
ccsp_rbus_logHandler
(
    rtLogLevel level,
    const char* file,
    int line,
    int threadId,
    char* message
)
{
    UNREFERENCED_PARAMETER(threadId);
    switch (level)
    {
        case RT_LOG_FATAL:
            CcspTraceCritical(("%s:%d %s\n", file, line, message));
            break;
        case RT_LOG_ERROR:
            CcspTraceError(("%s:%d %s\n", file, line, message));
            break;
        case RT_LOG_WARN:
            CcspTraceWarning(("%s:%d %s\n", file, line, message));
            break;
        case RT_LOG_INFO:
            CcspTraceInfo(("%s:%d %s\n", file, line, message));
            break;
        case RT_LOG_DEBUG:
            CcspTraceDebug(("%s:%d %s\n", file, line, message));
            break;
    }
    return;
}

int 
CCSP_Message_Bus_Init
(
    char *component_id,
    char *config_file,
    void **bus_handle,
    CCSP_MESSAGE_BUS_MALLOC mallocfc,
    CCSP_MESSAGE_BUS_FREE   freefc

)
{
    FILE                  *fp              = NULL;
    CCSP_MESSAGE_BUS_INFO *bus_info        = NULL;
    char                  address[256]     = {0};
    int                   count            = 0;
    int                   flags            = 0;
    int                   rc               = 0;
    struct                timespec timeout = { 0, 0 };

    static void *stashedHandleForTelemetry = NULL; /*added for telemetry*/

    /*if its telemetry and we have a stashed handle, give it back
      if no handle yet (which is unexpected) then do full init so t2 gets something back
    */
    /*CID: 144417 Dereference before null check*/
    if(component_id && strcmp(component_id, "com.cisco.spvtg.ccsp.t2commonlib") == 0)
    {
        if(stashedHandleForTelemetry)
        {
            CcspTraceWarning(("<%s>: telemetry2_0 stashed handle returned\n", __FUNCTION__));
            *bus_handle = stashedHandleForTelemetry;
            return 0;
        }
        else
        {
            CcspTraceWarning(("<%s>: telemetry2_0 stashed handle does not exist so creating new\n", __FUNCTION__));
        }
    }

    if(!config_file)
        config_file = "ccsp_msg.cfg";

    if ((fp = fopen(config_file, "r")) == NULL) {
        CcspTraceError(("<%s>: cannot open %s, try again after a while\n", __FUNCTION__, config_file));
        sleep(2);

        if ((fp = fopen(config_file, "r")) == NULL) {
            CcspTraceError(("<%s>: cannot open %s\n", __FUNCTION__, config_file));
            return -1;
        }
    }
        
    // alloc memory, assign return value
    if(mallocfc) bus_info =(CCSP_MESSAGE_BUS_INFO*) mallocfc(sizeof(CCSP_MESSAGE_BUS_INFO));
    else bus_info =(CCSP_MESSAGE_BUS_INFO*) malloc(sizeof(CCSP_MESSAGE_BUS_INFO));
    if( ! bus_info)
    {
        CcspTraceError(("<%s>: No memory\n", __FUNCTION__));
        fclose(fp);/*RDKB-6234, CID-33427, free the resource before return*/
        return -1;
    }
    memset(bus_info, 0, sizeof(CCSP_MESSAGE_BUS_INFO));
    *bus_handle = bus_info; // return

    /*stash this for telemetry*/
    stashedHandleForTelemetry = *bus_handle;

    // assign malloc and free func
    if(mallocfc) bus_info->mallocfunc = mallocfc;
    else bus_info->mallocfunc = malloc;
    if(freefc) bus_info->freefunc = freefc ;
    else bus_info->freefunc = free ;

    // bus name
    if(component_id) {
       strncpy(bus_info->component_id, component_id , sizeof(bus_info->component_id) -1);
       bus_info->component_id[sizeof(bus_info->component_id)-1] = '\0';
    }

    CCSP_Msg_IsRbus_enabled();
    RBUS_LOG("%s is enabled\n", rbus_enabled ? "RBus" : "DBus");  
    if(rbus_enabled)
    {
        rbus_error_t err = RTMESSAGE_BUS_SUCCESS;
        CCSP_Message_Bus_Register_Path_Priv_rbus(bus_info, thread_path_message_func_rbus, bus_info);

        /* Register with rtLog to use CCSPTRACE_LOGS */
        rtLogSetLogHandler(ccsp_rbus_logHandler);

        err = rbus_openBrokerConnection(component_id);
        if( err != RTMESSAGE_BUS_SUCCESS &&
                err != RTMESSAGE_BUS_ERROR_INVALID_STATE/*connection already opened. which is allowed*/)
        {
            CcspTraceError(("<%s>: rbus_openBrokerConnection fails for component_id=%s with %d\n", __FUNCTION__,component_id,err));
            fclose(fp);
            bus_info->freefunc(bus_info);
            *bus_handle = NULL;
            return -1;
        }
        else
        {
            CcspTraceInfo(("connection opened for %s\n",component_id));

            if((err = rbus_registerObj(component_id, (rbus_callback_t) bus_info->rbus_callback, bus_info)) != RTMESSAGE_BUS_SUCCESS)
            {
                CcspTraceError(("<%s>: rbus_registerObj fails for %s\n", __FUNCTION__, component_id));
            }
            else
            {
                if((err = rbus_registerSubscribeHandler(component_id, cssp_event_subscribe_override_handler_rbus, bus_info)) != RTMESSAGE_BUS_SUCCESS)
                {
                    rtLog_Error("<%s>: rbus_registerSubscribeHandler() failed with %d.  Rbus value change will not work.", __FUNCTION__, err);
                }

                if(strcmp(component_id,"eRT.com.cisco.spvtg.ccsp.tr069pa") == 0)
                {
                    if((err = rbus_registerEvent(component_id,CCSP_DIAG_COMPLETE_SIGNAL,NULL,NULL)) != RTMESSAGE_BUS_SUCCESS)
                        RBUS_LOG_ERR("%s : rbus_registerEvent returns Err: %d for diagCompleteSignal\n", __FUNCTION__, err);
                }
                else if(strcmp(component_id,"eRT.com.cisco.spvtg.ccsp.rm") == 0)
                {
                    if((err = rbus_registerEvent(component_id,CCSP_SYSTEM_REBOOT_SIGNAL,NULL,NULL)) != RTMESSAGE_BUS_SUCCESS)
                        RBUS_LOG_ERR("%s : rbus_registerEvent returns Err: %d for systemRebootSignal", __FUNCTION__, err);
                }
                else if(strcmp(component_id,"eRT.com.cisco.spvtg.ccsp.webpaagent") == 0)
                {
                    rbus_method_table_entry_t table[1] = {
                                                            {"webconfigSignal", (void*)bus_info, webcfg_signal_rbus},
                                                         };
                    if(( err = rbus_registerMethodTable(component_id, table, 1) != RTMESSAGE_BUS_SUCCESS ))
                    {
                        RBUS_LOG_ERR("%s : rbus_registerMethodTable returns Err: %d",  __FUNCTION__, err);
                    }
                }
                else if(strcmp(component_id,"eRT.com.cisco.spvtg.ccsp.wifi") == 0)
                {
                    rbus_method_table_entry_t table[2] = {
                                                            {"TunnelStatus", (void*)bus_info, tunnelStatus_signal_rbus},
                                                            {"WifiDbStatus", (void*)bus_info, wifiDbStatus_signal_rbus},
                                                         };
                    if(( err = rbus_registerMethodTable(component_id, table, 2) != RTMESSAGE_BUS_SUCCESS ))
                    {
                        RBUS_LOG_ERR("%s : rbus_registerMethodTable returns Err: %d",  __FUNCTION__, err);
                    }
                }
                else if(strcmp(component_id,"eRT.com.cisco.spvtg.ccsp.telemetry") == 0)
                {
                     rbus_method_table_entry_t table[1] = {
                                {CCSP_TELEMETRY_DATA_SIGNAL, (void*)bus_info, telemetry_send_signal_rbus}, 
                                        };
                    if(( err = rbus_registerMethodTable(component_id, table, 1) != RTMESSAGE_BUS_SUCCESS ))
                    {
                        RBUS_LOG_ERR("%s : rbus_registerMethodTable returns Err: %d",  __FUNCTION__, err);
                    }
                }
            }
        }
	/*CID: 110434 Resource leak*/
	fclose(fp);
        return 0;
    }
    //    CcspTraceDebug(("<%s>: component id = '%s'\n", __FUNCTION__, bus_info->component_id));

    // init var, mutex, msg_queue
    pthread_mutex_init(&bus_info->info_mutex, NULL);
    bus_info->msg_queue = NULL;
    bus_info->msg_queue = (CCSP_MSG_QUEUE*)bus_info->mallocfunc(sizeof(CCSP_MSG_QUEUE));
    if( ! bus_info->msg_queue)
    {
        CcspTraceError(("<%s>: No memory\n", __FUNCTION__));
        fclose(fp);/*RDKB-6234, CID-33427, free the resource before return*/
        bus_info->freefunc(bus_info);
        return -1;
    }
    CcspMsgQueueInit(bus_info->msg_queue);
    pthread_mutex_init(&bus_info->msg_mutex, NULL);

    if (NewCondVar(&bus_info->msg_threshold_cv) == -1)
    {
        CcspTraceError(("<%s>: Couldn't initialize condition variable!\n", __FUNCTION__));
        fclose(fp);
        bus_info->freefunc(bus_info);
        return -1;
    }

    bus_info->dbus_connect_thread_count = 0;
    bus_info->dbus_loop_thread_count = 0;
 
    pthread_mutex_lock(&bus_info->info_mutex);
    bus_info->run = 1;
    pthread_mutex_unlock(&bus_info->info_mutex);

    // init the default Dbus threads
    if(ccsp_bus_ref_count == 0)
        dbus_threads_init_default();

    ccsp_bus_ref_count++;

    /* Set up the new connections */
    pthread_mutex_lock(&bus_info->msg_mutex);
    while (fgets(address, sizeof(address), fp) && 
           count < CCSP_MESSAGE_BUS_MAX_CONNECTION)
    {
        /*assume the first address is our primary connection*/
        CCSP_Message_Bus_Strip(address);  // strip out \cr and \lf
        if(*address == 0) break;
        strncpy
            (   
                bus_info->connection[count].address, 
                address, 
                sizeof(bus_info->connection[count].address)-1
             );
        (bus_info->connection[count].address)[sizeof(bus_info->connection[count].address)-1] = '\0';

        /*
        CcspTraceDebug(
            (   
                "<%s>:"
                " socket address = '%s'\n", 
                __FUNCTION__, 
                bus_info->connection[count].address
            ));
        */

        // start the loop and connect threads, should be just one of each, even if count > 1
        bus_info->dbus_loop_thread_count++;

        /* Lock protecting the dispatch status of this connection */
        pthread_mutex_init(&bus_info->connection[count].dispatch_mutex, NULL);

        /* Lock protecting the connection state of this connection */
        pthread_mutex_init(&bus_info->connection[count].connect_mutex, NULL);
     
        bus_info->connection[count].bus_info_ptr = (void *)bus_info;
        bus_info->connection[count].loop = dbus_loop_new();
        
        /* Create the self pipe and set both ends to non-blocking */
        if ( (rc = pipe(bus_info->connection[count].self_pipe)) != -1 )
        {
            flags = fcntl(bus_info->connection[count].self_pipe[0], F_GETFL);
            flags |= O_NONBLOCK;
            rc = fcntl(bus_info->connection[count].self_pipe[0], F_SETFL, flags);
        }
        if (rc != -1)
        {
            flags = fcntl(bus_info->connection[count].self_pipe[1], F_GETFL);
            flags |= O_NONBLOCK;
            rc = fcntl(bus_info->connection[count].self_pipe[1], F_SETFL, flags);
        }
        if (rc != -1)
        {
            if (!dbus_loop_add_wake(bus_info->connection[count].loop, bus_info->connection[count].self_pipe[0]))
            {
                CcspTraceError(("<%s>: Couldn't add mainloop wakeup watch!\n", __FUNCTION__));
                rc = -1;
            }
        }
        if (rc == -1)
        {
            CcspTraceError(("<%s>: Couldn't create self pipe for waking mainloop!\n", __FUNCTION__));
            fclose(fp);
            close(bus_info->connection[count].self_pipe[0]);
            close(bus_info->connection[count].self_pipe[1]);
            bus_info->freefunc(bus_info);
            ccsp_bus_ref_count--;
            pthread_mutex_unlock(&bus_info->msg_mutex);
            pthread_mutex_destroy(&bus_info->msg_mutex);
            pthread_cond_destroy (&bus_info->msg_threshold_cv);
            pthread_mutex_destroy(&bus_info->connection[count].dispatch_mutex);
            pthread_mutex_destroy(&bus_info->connection[count].connect_mutex);
            return -1;
        }

        bus_info->dbus_connect_thread_count++;

        pthread_create
            (
                &bus_info->connection[count].connect_thread, 
                NULL, 
                CCSP_Message_Bus_Connect_Thread, 
                (void *)(&(bus_info->connection[count]))
             );

        count++;
    }
    fclose(fp);
    rc = 0;
    NewTimeout(&timeout, CCSP_MESSAGE_BUS_CONNECT_TIMEOUT_SECONDS, 0);
    while ((bus_info->dbus_connect_thread_count > 0) && (rc == 0))
        rc = pthread_cond_timedwait(&bus_info->msg_threshold_cv, &bus_info->msg_mutex, &timeout);

    /*
       Something really bad happened.
       A timeout while waiting for the counter to return to 0 means that one or
       more CCSP_Message_Bus_Connect_Thread threads got stuck and failed to
       terminate. We print an error and continue but it's really a fatal error.
    */
    if (rc != 0) {
        CcspTraceError(("<%s>: error %d waiting for dbus_connect_thread_count to return to 0\n", __FUNCTION__, rc));
    }

    /* Now that the threads are connected and configured, start the mainloops */
    for (count = 0; count < CCSP_MESSAGE_BUS_MAX_CONNECTION; count++)
    {
        if (bus_info->connection[count].conn && bus_info->connection[count].loop)
        {
            dbus_connection_set_wakeup_main_function
                (
                    bus_info->connection[count].conn,
                    (void *)&wake_mainloop,
                    (void *)(&bus_info->connection[count]),
                    NULL
                );

            pthread_create
                (
                    &bus_info->connection[count].loop_thread, 
                    NULL, 
                    CCSP_Message_Bus_Loop_Thread, 
                    (void *)(&(bus_info->connection[count]))
                 );
        }
    }    
    pthread_mutex_unlock(&bus_info->msg_mutex);

    //create a thread to handle dbus request
    pthread_create(&thread_dbus_process, NULL, CCSP_Message_Bus_Process_Thread, (void *)bus_info);

    //create a thread to monitor deadlock. Currently Only PandM enabled
    if ( strstr(bus_info->component_id, "com.cisco.spvtg.ccsp.pam" ) != 0 )
    {
        if (mallocfc) deadlock_detection_log =(DEADLOCK_ARRAY*) mallocfc(sizeof(DEADLOCK_ARRAY));
        else deadlock_detection_log =(DEADLOCK_ARRAY*) malloc(sizeof(DEADLOCK_ARRAY));
        if ( ! deadlock_detection_log ) 
        {
            CcspTraceError(("<%s>: No memory for deadlock log\n", __FUNCTION__));
            return -1;
        }
        memset(deadlock_detection_log, 0, sizeof(DEADLOCK_ARRAY));

        deadlock_detection_enable = 1;
        CcspBaseIf_deadlock_detection_time_normal_seconds = CcspBaseIf_timeout_seconds        + 30 + CcspBaseIf_timeout_protect_plus_seconds;
        CcspBaseIf_deadlock_detection_time_getval_seconds = CcspBaseIf_timeout_getval_seconds + 30 + CcspBaseIf_timeout_protect_plus_seconds;
        pthread_mutex_init(&(deadlock_detection_info.info_mutex), NULL);

        pthread_create
            (
                &thread_dbus_deadlock_monitor, 
                NULL, 
                CcspBaseIf_Deadlock_Detection_Thread, 
                (void *)bus_info
             ); 

        CcspTraceDebug(("<%s>: Deadlock monitor for %s started.\n", __FUNCTION__, bus_info->component_id));
    }

    return 0;
}

/* 
    DBus needs a way to wake up the main loop that is polling waiting for input
    in case there is output queued to send, in case shutdown is needed, etc. 
*/
static DBusWakeupMainFunction
wake_mainloop(
void *data
)
{
    int old_errno;

    CCSP_MESSAGE_BUS_CONNECTION *connection = (CCSP_MESSAGE_BUS_CONNECTION *) data;

    if (connection->loop)
    {
        old_errno = errno;
        while(write(connection->self_pipe[1], "x", 1) == -1 && errno == EAGAIN)
            CCSP_Msg_SleepInMilliSeconds(CCSP_MESSAGE_MAINLOOP_WAKE_RETRY_MS);
        errno = old_errno;
    }
    return NULL;
}

void 
CCSP_Message_Bus_Exit
(
    void *bus_handle
)
{
    int i;
    int ret = 0;

    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    struct timespec timeout = { 0, 0 };

    if(rbus_enabled)
    {
        rbus_error_t err = RTMESSAGE_BUS_SUCCESS;

        err = rbus_closeBrokerConnection();
        if(RTMESSAGE_BUS_SUCCESS != err)
            CcspTraceError(("<%s>: rbus_closeBrokerConnection fails with %d\n", __FUNCTION__,err));
        bus_info->freefunc(bus_info);
        bus_info = NULL;

        return;
    }

    /* First set run to 0 so processing thread will exit */
    pthread_mutex_lock(&bus_info->info_mutex);
    bus_info->run = 0;
    pthread_mutex_unlock(&bus_info->info_mutex);

    /* Tell the main loop threads to stop and close the connections */
    pthread_mutex_lock(&bus_info->msg_mutex);
    for(i = 0; i < CCSP_MESSAGE_BUS_MAX_CONNECTION; i++)
    {
        /* Stop all the loop threads */
        if(bus_info->connection[i].loop && bus_info->connection[i].conn )
        {
            dbus_connection_lock(bus_info->connection[i].conn);
            wake_mainloop(&bus_info->connection[i]);
            dbus_connection_unlock(bus_info->connection[i].conn);
        }
    }

    /* Wake up any threads waiting on the condition variable so that they will shut down */
    pthread_cond_signal(&bus_info->msg_threshold_cv);
    pthread_mutex_unlock(&bus_info->msg_mutex);

    { // join all the threads started in init

        char *msg = NULL;

        /*
           All CCSP_Message_Bus_Connect_Thread threads are detachd and will have
           terminated long before we reach this point, so they don't need to be
           handled here.

           All CCSP_Message_Bus_Process_Thread threads will terminate themselves
           when bus_info->run is set to 0. We still need to wait for this thread 
           to terminate though before freeing the dbus resources it uses.

           The CcspBaseIf_Deadlock_Detection_Thread thread is not detached, so
           we need to call pthread_join for it.
        */

        if(thread_dbus_deadlock_monitor && (ret = pthread_join(thread_dbus_deadlock_monitor, (void **)&msg)) != 0) {
            CcspTraceError(("<%s>: thread deadlock monitor join returned %d with error %s\n", __FUNCTION__, ret, msg));
        }
        if(msg) {
            free(msg);
            msg = NULL;
        }

        if(thread_dbus_process && (ret = pthread_join(thread_dbus_process, (void **)&msg)) != 0) {
            CcspTraceError(("<%s>: thread dbus process join returned %d with error %s\n", __FUNCTION__, ret, msg));
        }
        if(msg) {
            free(msg);
            msg = NULL;
        }
    }

    pthread_mutex_lock(&bus_info->msg_mutex);
    /* Wait for loop threads to stop */
    if (bus_info->dbus_loop_thread_count > 0)
    {
        ret = 0;
        NewTimeout(&timeout, CCSP_MESSAGE_BUS_DISCONNECT_TIMEOUT_SECONDS, 0);
        while ((bus_info->dbus_loop_thread_count > 0) && (ret == 0)) {
            ret = pthread_cond_timedwait(&bus_info->msg_threshold_cv, &bus_info->msg_mutex, &timeout);
        }
    }
    pthread_mutex_unlock(&bus_info->msg_mutex);

    /* Get rid of all the connections and free the resources */
    for(i = 0; i < CCSP_MESSAGE_BUS_MAX_CONNECTION; i++)
    {
        if(bus_info->connection[i].conn )
        {
            dbus_connection_close(bus_info->connection[i].conn);
            dbus_connection_unref(bus_info->connection[i].conn);
            close(bus_info->connection[i].self_pipe[0]);
            close(bus_info->connection[i].self_pipe[1]);
            pthread_mutex_destroy(&bus_info->connection[i].dispatch_mutex);
            pthread_mutex_destroy(&bus_info->connection[i].connect_mutex);
        }
    }

    // RTian 5/3/2013    CCSP_Msg_SleepInMilliSeconds(1000);
    pthread_mutex_lock(&bus_info->info_mutex);
    for(i = 0; i < CCSP_MESSAGE_BUS_MAX_FILTER; i++)
    {
        if(bus_info->filters[i].sender)    bus_info->freefunc(bus_info->filters[i].sender);
        if(bus_info->filters[i].path)      bus_info->freefunc(bus_info->filters[i].path);
        if(bus_info->filters[i].interface) bus_info->freefunc(bus_info->filters[i].interface);
        if(bus_info->filters[i].event)     bus_info->freefunc(bus_info->filters[i].event);
    }

    for(i = 0; i < CCSP_MESSAGE_BUS_MAX_PATH; i++)
    {
        if(bus_info->path_array[i].path) {
            bus_info->freefunc(bus_info->path_array[i].path);
        }
    }

    if(bus_info->CcspBaseIf_func) bus_info->freefunc(bus_info->CcspBaseIf_func);

    if(bus_info->msg_queue)
    {
        PCCSP_MSG_SINGLE_LINK_ENTRY entry = NULL;
        CCSP_REQ_DESCRIPTOR *req_rec = NULL;       
        while((entry = CcspMsgQueuePopEntry(bus_info->msg_queue)) != NULL) {
            req_rec = CCSP_MSG_ACCESS_CONTAINER(entry,CCSP_REQ_DESCRIPTOR,Linkage);
            dbus_message_unref(req_rec->message);            
            bus_info->freefunc(req_rec);
        }

        bus_info->freefunc(bus_info->msg_queue);
        bus_info->msg_queue = NULL;
        pthread_mutex_destroy(&bus_info->msg_mutex);
        pthread_cond_destroy (&bus_info->msg_threshold_cv);
    }
    pthread_mutex_unlock(&bus_info->info_mutex);
    pthread_mutex_destroy(&bus_info->info_mutex);

    bus_info->freefunc(bus_info);
    bus_info = NULL;
    ccsp_bus_ref_count--;
   // if(ccsp_bus_ref_count == 0) dbus_shutdown();

    //    CcspTraceDebug(("<%s>: component_id = '%s'\n", __FUNCTION__, bus_info->component_id));
    
    return;
}

static void
append_event_info
(
     char * destination,
     const char * sender,
     const char * path,
     const char * interface,
     const char * event_name
)
{
    char buf[512] = {0};
    errno_t rc = -1;

    if(sender)
    {
        rc = sprintf_s(buf, sizeof(buf), ",sender='%s'", sender);
        if(rc < EOK)
        {
            ERR_CHK(rc);
        }
        /*destination is a pointer, pointing to 512 bytes data*/
        rc = strcat_s(destination, 512, buf);
        ERR_CHK(rc);
    }

    if(path)
    {
        rc = sprintf_s(buf, sizeof(buf), ",path='%s'", path);
        if(rc < EOK)
        {
            ERR_CHK(rc);
        }
        /*destination is a pointer, pointing to 512 bytes data*/
        rc = strcat_s(destination, 512, buf);
        ERR_CHK(rc);
    }

    if(interface)
    {
        rc = sprintf_s(buf, sizeof(buf), ",interface='%s'", interface);
        if(rc < EOK)
        {
            ERR_CHK(rc);
        }
        /*destination is a pointer, pointing to 512 bytes data*/
        rc = strcat_s(destination, 512, buf);
        ERR_CHK(rc);
    }

    if(event_name)
    {
        rc = sprintf_s(buf, sizeof(buf), ",member='%s'", event_name);
        if(rc < EOK)
        {
            ERR_CHK(rc);
        }
        /*destination is a pointer, pointing to 512 bytes data*/
        rc = strcat_s(destination, 512, buf);
        ERR_CHK(rc);
    }

    return;
}

static int  
CCSP_Message_Bus_Register_Event_Priv
(
    DBusConnection *conn,
    const char* sender,
    const char* path,
    const char* interface,
    const char* event_name,
    int ifregister

)
{
    char tmp[512] = {0};
    int  ret = 0;
    errno_t rc = -1;

    rc = strcpy_s(tmp, sizeof(tmp), "type='signal'");
    ERR_CHK(rc);
    append_event_info(tmp, sender, path, interface, event_name);

    if(ifregister)
        ret = CCSP_Message_Bus_Send_Str 
                  (
                      conn,
                      DBUS_SERVICE_DBUS,
                      DBUS_PATH_DBUS,
                      DBUS_INTERFACE_DBUS,
                      "AddMatch", 
                      tmp
                   );
    else
        ret = CCSP_Message_Bus_Send_Str 
                  (
                      conn,
                      DBUS_SERVICE_DBUS,
                      DBUS_PATH_DBUS,
                      DBUS_INTERFACE_DBUS,
                      "RemoveMatch", 
                      tmp
                   );
    
    return ret;
}


static int 
CCSP_Message_Save_Register_Event
(
    void* bus_handle,
    const char* sender,
    const char* path,
    const char* interface,
    const char* event_name
)
{
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    int i;
    errno_t rc = -1;

    pthread_mutex_lock(&bus_info->info_mutex);

    for(i = 0; i < CCSP_MESSAGE_BUS_MAX_FILTER; i++)
    {
        // find the first empty slot, save, and return
        if(bus_info->filters[i].used  == 0)
        {
            bus_info->filters[i].used = 1;

            if(path)
            {
                bus_info->filters[i].path = bus_info->mallocfunc(strlen(path)+1);
                rc = strcpy_s(bus_info->filters[i].path, (strlen(path)+1), path);
                ERR_CHK(rc);
            }

            if(interface)
            {
                bus_info->filters[i].interface = bus_info->mallocfunc(strlen(interface)+1);
                rc = strcpy_s(bus_info->filters[i].interface, (strlen(interface)+1), interface);
                ERR_CHK(rc);
            }

            if(event_name)
            {
                bus_info->filters[i].event = bus_info->mallocfunc(strlen(event_name)+1);
                rc = strcpy_s(bus_info->filters[i].event, (strlen(event_name)+1), event_name);
                ERR_CHK(rc);
            }
            if(sender)
            {
                bus_info->filters[i].sender = bus_info->mallocfunc(strlen(sender)+1);
                rc = strcpy_s(bus_info->filters[i].sender, (strlen(sender)+1), sender);
                ERR_CHK(rc);
            }

            pthread_mutex_unlock(&bus_info->info_mutex);
            return CCSP_Message_Bus_OK;
        }
    }

    // all slots are in use
    pthread_mutex_unlock(&bus_info->info_mutex);
    return CCSP_Message_Bus_OOM;
}


int  
CCSP_Message_Bus_Register_Event
(
    void* bus_handle,
    const char* sender,
    const char* path,
    const char* interface,
    const char* event_name
)
{
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    int i = 0;
    DBusConnection *conn = NULL;
    int ret = 0;

    pthread_mutex_lock(&bus_info->info_mutex);
    for(i = 0; i < CCSP_MESSAGE_BUS_MAX_CONNECTION; i++)
    {
        if(bus_info->connection[i].connected && bus_info->connection[i].conn)
        {
            conn = bus_info->connection[i].conn;
            dbus_connection_ref (conn);
            pthread_mutex_unlock(&bus_info->info_mutex);

            ret = CCSP_Message_Bus_Register_Event_Priv(conn, sender, path, interface, event_name, 1);
            dbus_connection_unref (conn);
            if(ret != CCSP_Message_Bus_OK) return ret;

            pthread_mutex_lock(&bus_info->info_mutex);
        }
    }
    pthread_mutex_unlock(&bus_info->info_mutex);

    return CCSP_Message_Save_Register_Event(bus_handle, sender, path, interface, event_name);
}


int  
CCSP_Message_Bus_UnRegister_Event
(
    void* bus_handle,
    const char* sender,
    const char* path,
    const char* interface,
    const char* event_name
)
{
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    int i = 0;
    DBusConnection *conn = NULL;

    // unregister event
    pthread_mutex_lock(&bus_info->info_mutex);
    for(i = 0; i < CCSP_MESSAGE_BUS_MAX_CONNECTION; i++)
    {
        if(bus_info->connection[i].connected && bus_info->connection[i].conn )
        {
            conn = bus_info->connection[i].conn;
            dbus_connection_ref (conn);
            pthread_mutex_unlock(&bus_info->info_mutex);

            CCSP_Message_Bus_Register_Event_Priv(conn, sender, path, interface, event_name, 0);
            dbus_connection_unref (conn);

            pthread_mutex_lock(&bus_info->info_mutex);
        }
    }
    pthread_mutex_unlock(&bus_info->info_mutex);

    // clear local cache
    char target[512] = {0};
    memset(target, 0, sizeof(target));
    append_event_info(target, sender, path, interface, event_name);

    char candidate[512] = {0};
    pthread_mutex_lock(&bus_info->info_mutex);
    for(i = 0; i < CCSP_MESSAGE_BUS_MAX_FILTER; i++)
    {
        if(bus_info->filters[i].used )
        {
            memset(candidate, 0, sizeof(candidate));
            append_event_info
                ( 
                    candidate, 
                    bus_info->filters[i].sender,
                    bus_info->filters[i].path,
                    bus_info->filters[i].interface,
                    bus_info->filters[i].event
                  );

            if( strcmp(target, candidate) == 0) 
            {
                if(bus_info->filters[i].sender)    bus_info->freefunc(bus_info->filters[i].sender);
                if(bus_info->filters[i].path)      bus_info->freefunc(bus_info->filters[i].path);
                if(bus_info->filters[i].interface) bus_info->freefunc(bus_info->filters[i].interface);
                if(bus_info->filters[i].event)     bus_info->freefunc(bus_info->filters[i].event);

                bus_info->filters[i].sender        = NULL;
                bus_info->filters[i].path          = NULL;
                bus_info->filters[i].interface     = NULL;
                bus_info->filters[i].event         = NULL;

                bus_info->filters[i].used       = 0;
                
                break;
            }
        }
    }
    pthread_mutex_unlock(&bus_info->info_mutex);

    if(i == CCSP_MESSAGE_BUS_MAX_FILTER)
        return CCSP_Message_Bus_ERROR;
    else
        return CCSP_Message_Bus_OK;
}

void  
CCSP_Message_Bus_Set_Event_Callback
(
    void* bus_handle,
    DBusObjectPathMessageFunction   callback,
    void * user_data
)
{
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    bus_info->user_data = user_data;
    bus_info->sig_callback = callback;
}


static int  
CCSP_Message_Bus_Register_Path_Priv
(
    void* bus_handle,
    const char* path,
    DBusObjectPathMessageFunction funcptr,
    void * user_data
)
{
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    int ret = CCSP_Message_Bus_ERROR;
    DBusError error;
    errno_t rc = -1;

    int i, j;

    dbus_error_init (&error);
    pthread_mutex_lock(&bus_info->info_mutex);
    for(i = 0; i < CCSP_MESSAGE_BUS_MAX_PATH; i++)
    {
        if(bus_info->path_array[i].path == NULL)
        {
            bus_info->path_array[i].path = bus_info->mallocfunc(strlen(path)+1);
            rc = strcpy_s(bus_info->path_array[i].path, (strlen(path)+1), path);
            ERR_CHK(rc);
            bus_info->path_array[i].user_data = user_data ;
            bus_info->path_array[i].echo_vtable.unregister_function = path_unregistered_func;
            bus_info->path_array[i].echo_vtable.message_function = funcptr;

            break;
        }
    }
    
    if(rbus_enabled)
    {
        pthread_mutex_unlock(&bus_info->info_mutex);
        dbus_error_free(&error);
        return CCSP_Message_Bus_OK;
        /*
         * this is already handed by CCSP_Message_Bus_Register_Path_Priv_rbus during init
         * hence there is no need to progress further in this function in rbus mode.
        */
    }
    
    if(i != CCSP_MESSAGE_BUS_MAX_PATH) 
    {
        for(j = 0; j < CCSP_MESSAGE_BUS_MAX_CONNECTION; j++)
        {
            if(bus_info->connection[j].connected && bus_info->connection[j].conn )
            {
                if(dbus_connection_try_register_object_path
                        (
                         bus_info->connection[j].conn,
                         path,
                         &bus_info->path_array[i].echo_vtable,
                         (void*)user_data,
                         &error
                        ))
                    ret = CCSP_Message_Bus_OK;
            }
        }
    }

    pthread_mutex_unlock(&bus_info->info_mutex);
    dbus_error_free(&error);

    return ret;
}

void ccsp_handle_rbus_component_reply (void* bus_handle, rbusMessage msg, rbusValueType_t typeVal, enum dataType_e *pType, char** pStringValue)
{
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    int32_t ival = 0;
    int64_t i64 = 0;
    double fval = 0;
    const void *pValue = NULL;
    int length = 0;
    char *pTmp = NULL;
    int n = 0;

    switch(typeVal)
    {
        case RBUS_INT16:
        case RBUS_INT32:
            rbusMessage_GetInt32(msg, &ival);
            n = snprintf(pTmp, 0, "%d", ival) + 1;
            *pStringValue = bus_info->mallocfunc(n);
            snprintf(*pStringValue, (unsigned int)n, "%d", ival);
            *pType = ccsp_int;
            break;

        case RBUS_UINT16:
        case RBUS_UINT32:
            rbusMessage_GetInt32(msg, &ival);
            n = snprintf(pTmp, 0, "%u", (uint32_t)ival) + 1;
            *pStringValue = bus_info->mallocfunc(n);
            snprintf(*pStringValue, (unsigned int)n, "%u", (uint32_t)ival);
            *pType = ccsp_unsignedInt;
            break;

        case RBUS_INT64:
        {
            rbusMessage_GetInt64(msg, &i64);
#ifdef _64BIT_ARCH_SUPPORT_
            n = snprintf(pTmp, 0, "%ld", i64) + 1;
#else
            n = snprintf(pTmp, 0, "%lld", i64) + 1;
#endif
            *pStringValue = bus_info->mallocfunc(n);
#ifdef _64BIT_ARCH_SUPPORT_
            snprintf(*pStringValue, (unsigned int)n, "%ld", i64);
#else
            snprintf(*pStringValue, (unsigned int)n, "%lld", i64);
#endif	    
            *pType = ccsp_long;
            break;
        }
        case RBUS_UINT64:
        {
            rbusMessage_GetInt64(msg, &i64);
#ifdef _64BIT_ARCH_SUPPORT_
            n = snprintf(pTmp, 0, "%lu", (uint64_t)i64) + 1;
#else
            n = snprintf(pTmp, 0, "%llu", (uint64_t)i64) + 1;
#endif
            *pStringValue = bus_info->mallocfunc(n);
#ifdef _64BIT_ARCH_SUPPORT_
            snprintf(*pStringValue, (unsigned int)n, "%lu", (uint64_t)i64);
#else	
            snprintf(*pStringValue, (unsigned int)n, "%llu", (uint64_t)i64);
#endif	    
            *pType = ccsp_unsignedLong;
            break;
        }
        case RBUS_SINGLE:
            rbusMessage_GetDouble(msg, &fval);
            n = snprintf(pTmp, 0, "%f", fval) + 1;
            *pStringValue = bus_info->mallocfunc(n);
            snprintf(*pStringValue, (unsigned int)n, "%f", fval);
            *pType = ccsp_float;
            break;
        case RBUS_DOUBLE:
            rbusMessage_GetDouble(msg, &fval);
            n = snprintf(pTmp, 0, "%f", fval) + 1;
            *pStringValue = bus_info->mallocfunc(n);
            snprintf(*pStringValue, (unsigned int)n, "%f", fval);
            *pType = ccsp_double;
            break;
        case RBUS_DATETIME:
        {
            rbusDateTime_t rbus_time = {{0},{0}};
            char tmpBuff[40] = {0};
            rbusMessage_GetBytes(msg, (uint8_t const**)&pValue, (uint32_t*)&length);
            memcpy(&rbus_time,pValue,sizeof(rbusDateTime_t));
            if(0 == rbus_time.m_time.tm_year) {
                snprintf(tmpBuff, sizeof(tmpBuff), "%04d-%02d-%02dT%02d:%02d:%02d", rbus_time.m_time.tm_year,
                                                                    rbus_time.m_time.tm_mon,
                                                                    rbus_time.m_time.tm_mday,
                                                                    rbus_time.m_time.tm_hour,
                                                                    rbus_time.m_time.tm_min,
                                                                    rbus_time.m_time.tm_sec);
            } else {
                /* tm_mon represents month from 0 to 11. So increment tm_mon by 1.
                   tm_year represents years since 1900. So add 1900 to tm_year.
                 */
                snprintf(tmpBuff, sizeof(tmpBuff), "%04d-%02d-%02dT%02d:%02d:%02d", rbus_time.m_time.tm_year+1900,
                                                                    rbus_time.m_time.tm_mon+1,
                                                                    rbus_time.m_time.tm_mday,
                                                                    rbus_time.m_time.tm_hour,
                                                                    rbus_time.m_time.tm_min,
                                                                    rbus_time.m_time.tm_sec);
            }
            n = snprintf(pTmp, 0, "0000-00-00T00:00:00+00:00") + 1;
            *pStringValue = bus_info->mallocfunc(n);
            if( rbus_time.m_tz.m_tzhour || rbus_time.m_tz.m_tzmin )
            {
                snprintf(*pStringValue, (unsigned int)n, "%s%c%02d:%02d", tmpBuff, (0 == rbus_time.m_tz.m_isWest) ? '+':'-', rbus_time.m_tz.m_tzhour, rbus_time.m_tz.m_tzmin);
            }
            else
            {
                snprintf(*pStringValue, (unsigned int)n, "%sZ", tmpBuff);
            }
            *pType = ccsp_dateTime;
        }
            break;
        case RBUS_BOOLEAN:
        {
            rbusMessage_GetBytes(msg, (uint8_t const**)&pValue, (uint32_t *)&length);
            unsigned char boolValue = *(unsigned char*)pValue;
            n = snprintf(pTmp, 0, "false") + 1;
            *pStringValue = bus_info->mallocfunc(n);
            snprintf(*pStringValue, (unsigned int)n, "%s", boolValue ? "true" : "false");
            *pType = ccsp_boolean;
            break;
        }
        case RBUS_CHAR:
        case RBUS_INT8:
        {
            rbusMessage_GetBytes(msg, (uint8_t const**)&pValue, (uint32_t *)&length);
            signed char tmpValue = *(signed char*)pValue;
            n = snprintf(pTmp, 0, "%d", tmpValue) + 1;
            *pStringValue = bus_info->mallocfunc(n);
            snprintf(*pStringValue, (unsigned int)n, "%d", tmpValue);
            *pType = ccsp_int;
            break;
        }
        case RBUS_UINT8:
        case RBUS_BYTE:
        {
            rbusMessage_GetBytes(msg, (uint8_t const**)&pValue, (uint32_t *)&length);
            unsigned char tmpValue = *(unsigned char*)pValue;
            n = snprintf(pTmp, 0, "%u", tmpValue) + 1;
            *pStringValue = bus_info->mallocfunc(n);
            snprintf(*pStringValue, (unsigned int)n, "%u", tmpValue);
            *pType = ccsp_unsignedInt;
            break;
        }
        case RBUS_STRING:
            rbusMessage_GetBytes(msg, (uint8_t const**)&pValue, (uint32_t *)&length);
            n = length + 1;
            *pStringValue = bus_info->mallocfunc(n);
            snprintf(*pStringValue, (unsigned int)n, "%s", (char*)pValue);
            *pType = ccsp_string;
            break;
        case RBUS_BYTES:
        {
            int k = 0;
            unsigned char* pVar = NULL;
            char* pStrVar = NULL;
            unsigned char tmp = 0;
            rbusMessage_GetBytes(msg, (uint8_t const**)&pVar, (uint32_t *)&length);

            n = (2 * length) + 1;
            *pType = ccsp_byte;
            pStrVar = bus_info->mallocfunc(n);
            *pStringValue = pStrVar;
            for (k = 0; k < length; k++)
            {
                tmp = pVar[k];
                snprintf (&pStrVar[k * 2], (unsigned int)n, "%02X", tmp);
            }
            break;
        }
        case RBUS_PROPERTY:
        case RBUS_OBJECT:
        case RBUS_NONE:
        default:
            *pStringValue = bus_info->mallocfunc(10);
            strncpy(*pStringValue, "", 10);
            *pType = ccsp_none;
            break;
    }
    return;
}

unsigned int get_writeid(const char *str)
{
    if ( _ansc_strcmp(str, "ccsp.busclient" ) == 0 )
        return DSLH_MPA_ACCESS_CONTROL_CLIENTTOOL;
    else if ( _ansc_strcmp(str, "ccsp.cisco.spvtg.ccsp.snmp" ) == 0 )
        return DSLH_MPA_ACCESS_CONTROL_SNMP;
    else if ( _ansc_strcmp(str, "com.cisco.spvtg.ccsp.lms") == 0 )
        return DSLH_MPA_ACCESS_CONTROL_LM;
    else if ( _ansc_strcmp(str, "com.cisco.spvtg.ccsp.wifi") == 0 )
        return DSLH_MPA_ACCESS_CONTROL_WIFI;
    else
        return  DSLH_MPA_ACCESS_CONTROL_ACS;
}

char* writeid_to_string(unsigned int writeid)
{
    if(writeid == DSLH_MPA_ACCESS_CONTROL_WEBUI)
        return "writeid_webui";
    else if(writeid == DSLH_MPA_ACCESS_CONTROL_SNMP)
        return "writeid_snmp";
    else if(writeid == DSLH_MPA_ACCESS_CONTROL_ACS)
        return "writeid_acs";
    else if(writeid == DSLH_MPA_ACCESS_CONTROL_CLI)
        return "writeid_cli";
    else if(writeid == DSLH_MPA_ACCESS_CONTROL_WEBPA)
        return "writeid_webpa";
    else if(writeid == DSLH_MPA_ACCESS_CONTROL_XPC)
        return "writeid_xpc";
    else if(writeid == DSLH_MPA_ACCESS_CONTROL_NOTIFY_COMP)
        return "writeid_notify";
    else if(writeid == DSLH_MPA_ACCESS_CONTROL_CLIENTTOOL)
        return "writeid_clienttool";
    else if(writeid == DSLH_MPA_ACCESS_CONTROL_LM)
        return "writeid_lm";
    else if(writeid == DSLH_MPA_ACCESS_CONTROL_WIFI)
        return "writeid_wifi";
    else if(writeid == DSLH_MPA_ACCESS_CONTROL_XMPP)
        return "writeid_xmpp";
    else if(writeid == DSLH_MPA_ACCESS_CONTROL_WEBCONFIG)
        return "webconfig";
    else
        return "writeid_cli";
}

unsigned int string_to_writeid(const char *str)
{
    if ( _ansc_strcmp(str, "writeid_webui") == 0 )
        return DSLH_MPA_ACCESS_CONTROL_WEBUI;
    else if ( _ansc_strcmp(str, "writeid_snmp") == 0 )
        return DSLH_MPA_ACCESS_CONTROL_SNMP;
    else if ( _ansc_strcmp(str, "writeid_acs") == 0 )
        return DSLH_MPA_ACCESS_CONTROL_ACS;
    else if ( _ansc_strcmp(str, "writeid_cli") == 0 )
        return DSLH_MPA_ACCESS_CONTROL_CLI;
    else if ( _ansc_strcmp(str, "writeid_webpa") == 0 )
        return DSLH_MPA_ACCESS_CONTROL_WEBPA;
    else if ( _ansc_strcmp(str, "writeid_xpc") == 0 )
        return DSLH_MPA_ACCESS_CONTROL_XPC;
    else if ( _ansc_strcmp(str, "writeid_notify") == 0 )
        return DSLH_MPA_ACCESS_CONTROL_NOTIFY_COMP;
    else if ( _ansc_strcmp(str, "writeid_clienttool") == 0 )
        return DSLH_MPA_ACCESS_CONTROL_CLIENTTOOL;
    else if ( _ansc_strcmp(str, "writeid_lm") == 0 )
        return DSLH_MPA_ACCESS_CONTROL_LM;
    else if ( _ansc_strcmp(str, "writeid_wifi") == 0 )
        return DSLH_MPA_ACCESS_CONTROL_WIFI;
    else if ( _ansc_strcmp(str, "writeid_xmpp") == 0 )
        return DSLH_MPA_ACCESS_CONTROL_XMPP;
    else if ( _ansc_strcmp(str, "webconfig") == 0 )
        return DSLH_MPA_ACCESS_CONTROL_WEBCONFIG;

    return DSLH_MPA_ACCESS_CONTROL_CLI;
}

static int thread_path_message_func_rbus(const char * destination, const char * method, rbusMessage request, void * user_data, rbusMessage *response, const rtMessageHeader* hdr)
{
    UNREFERENCED_PARAMETER(destination);
    (void)hdr;
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)user_data;

    CCSP_Base_Func_CB* func = (CCSP_Base_Func_CB* )bus_info->CcspBaseIf_func;
    RBUS_LOG("%s Component ID: %s\n", __FUNCTION__, bus_info->component_id);
    if(NULL != method && func != NULL)
    {
        if(!strncmp(method, METHOD_GETPARAMETERVALUES, MAX_METHOD_NAME_LENGTH) && func->getParameterValues)
        {
            int result = 0, i =0, size =0;
            unsigned int writeID = DSLH_MPA_ACCESS_CONTROL_ACS;
            const char *writeID_str = NULL;
            int32_t param_size = 1, tmp = 0;
            char **parameterNames = 0;
            parameterValStruct_t **val = 0;

            if(rbusMessage_GetString(request, &writeID_str) == RT_OK)
                writeID = get_writeid(writeID_str);
            rbusMessage_GetInt32(request, &param_size);

            if(param_size > 0)
            {
                parameterNames = bus_info->mallocfunc(param_size*sizeof(char *));
                memset(parameterNames, 0, param_size*sizeof(char *));
            }

            for(i = 0; i < param_size; i++)
            {
                parameterNames[i] = NULL;
                rbusMessage_GetString(request, (const char**)&parameterNames[i]);
                RBUS_LOG("%s parameterNames[%d]: %s\n", __FUNCTION__, i, parameterNames[i]);
            }

            result = func->getParameterValues(writeID, parameterNames, param_size, &size, &val , func->getParameterValues_data);
            RBUS_LOG("%s size %d result %d\n", __FUNCTION__, size, result);
            bus_info->freefunc(parameterNames);

            rbusMessage_Init(response);
            tmp = result;
            rbusMessage_SetInt32(*response, tmp); //result
            if(tmp == CCSP_SUCCESS )
            {
                rbusMessage_SetInt32(*response, size);
                for(i = 0; i < size; i++)
                {
                    RBUS_LOG("%s val[%d]->parameterName %s val[%d]->parameterValue %s\n", __FUNCTION__, i, val[i]->parameterName, i, val[i]->parameterValue);
                    rbusMessage_SetString(*response, val[i]->parameterName);
                    rbusMessage_SetInt32(*response, val[i]->type);
                    rbusMessage_SetString(*response, val[i]->parameterValue);
                }
            }
            free_parameterValStruct_t(bus_info, size, val);
        }
        else if(!strncmp(method, METHOD_GETHEALTH, MAX_METHOD_NAME_LENGTH) && func->getHealth)
        {
            int32_t result = 0;
            result = func->getHealth();
            rbusMessage_Init(response);
            rbusMessage_SetInt32(*response, result);
            RBUS_LOG("exiting METHOD_GETHEALTH with result %d\n", result);
            return DBUS_HANDLER_RESULT_HANDLED;
        }
        else if(!strncmp(method, METHOD_SETPARAMETERVALUES, MAX_METHOD_NAME_LENGTH) && func->setParameterValues)
        {
            int param_size = 0, i = 0, result = 0;
            parameterValStruct_t * parameterVal = 0;
            unsigned int writeID = DSLH_MPA_ACCESS_CONTROL_CLI;
            const char * writeID_str = NULL;
            int32_t sessionId = 0, tmp = 0;
            dbus_bool commit = 0;
            char *invalidParameterName = 0;
            int32_t dataType = 0;

            rbusMessage_GetInt32(request, &sessionId);
            if(rbusMessage_GetString(request, &writeID_str) == RT_OK)
                writeID = string_to_writeid(writeID_str);
            rbusMessage_GetInt32(request, (int32_t*)&param_size);
            if(param_size > 0)
            {
                parameterVal = bus_info->mallocfunc(param_size*sizeof(parameterValStruct_t ));
                memset(parameterVal, 0, param_size*sizeof(parameterValStruct_t ));
            }

            for(i = 0; i < param_size; i++)
            {
                parameterVal[i].parameterName = NULL;
                rbusMessage_GetString(request, (const char**)&parameterVal[i].parameterName);
                rbusMessage_GetInt32(request, &dataType);
                if (dataType < RBUS_BOOLEAN)
                {
                    parameterVal[i].type = dataType;
                    parameterVal[i].parameterValue = NULL;
                    rbusMessage_GetString(request, (const char**)&parameterVal[i].parameterValue);
                }
                else
                {
                    ccsp_handle_rbus_component_reply (bus_info, request, (rbusValueType_t) dataType, &parameterVal[i].type, &parameterVal[i].parameterValue);
                }
            }
            const char *str = NULL;
            rbusMessage_GetString(request, &str); //commit
            commit = (str && strcasecmp(str, "TRUE") == 0)?1:0;
            result = func->setParameterValues(sessionId, writeID, parameterVal, param_size, commit,&invalidParameterName, func->setParameterValues_data);
            RBUS_LOG("%s :setParameterValues result %d\n", __FUNCTION__, result);
            rbusMessage_Init(response);
            tmp = result;
            rbusMessage_SetInt32(*response, tmp); //result
            if(invalidParameterName != NULL)
                rbusMessage_SetString(*response, invalidParameterName); //invalid param
            else
                rbusMessage_SetString(*response, ""); //invalid param
            bus_info->freefunc(parameterVal);
            bus_info->freefunc(invalidParameterName);
            return DBUS_HANDLER_RESULT_HANDLED;
        }
        else if(!strncmp(method, METHOD_GETPARAMETERATTRIBUTES, MAX_METHOD_NAME_LENGTH) && func->getParameterAttributes)
        {
            char **parameterNames = 0;
            parameterAttributeStruct_t **val = 0;
            int size = 0, result = 0, i = 0, param_size = 0;
            int32_t tmp = 0;
            rbusMessage_GetInt32(request, &param_size);

            if(param_size)
            {
                parameterNames = bus_info->mallocfunc(param_size*sizeof(char *));
                memset(parameterNames, 0, param_size*sizeof(char *));
            }

            for(i = 0; i < param_size; i++)
            {
                parameterNames[i] = NULL;
                rbusMessage_GetString(request, (const char**)&parameterNames[i]);
                RBUS_LOG("%s parameterNames[%d]: %s\n", __FUNCTION__, i, parameterNames[i]);
            }

            size = 0;
            result = func->getParameterAttributes(parameterNames, param_size, &size, &val , func->getParameterAttributes_data);
            bus_info->freefunc(parameterNames);
            rbusMessage_Init(response);
            tmp = result;
            rbusMessage_SetInt32(*response, tmp);
            if( tmp == CCSP_SUCCESS )
            {
                rbusMessage_SetInt32(*response, (int32_t)size);

                for(i = 0; i < size; i++)
                {
                    rbusMessage_SetString(*response, val[i]->parameterName);
                    rbusMessage_SetInt32(*response, val[i]->notificationChanged);
                    rbusMessage_SetInt32(*response, val[i]->notification);
                    rbusMessage_SetInt32(*response, val[i]->accessControlChanged);
                    rbusMessage_SetInt32(*response, val[i]->access);
                    rbusMessage_SetInt32(*response, val[i]->accessControlBitmask);
                }
            }
            free_parameterAttributeStruct_t(bus_info, size, val);
            return DBUS_HANDLER_RESULT_HANDLED;
        }
        else if(!strncmp(method, METHOD_COMMIT, MAX_METHOD_NAME_LENGTH) && func->setCommit)
        {
            int32_t tmp = 0, sessionId = 0, commit = 0;
            unsigned int writeID = DSLH_MPA_ACCESS_CONTROL_CLI;
            const char * writeID_str = NULL;
            int result = 0;
            rbusMessage_GetInt32(request, &sessionId);
            if(rbusMessage_GetString(request, &writeID_str) == RT_OK)
                writeID = string_to_writeid(writeID_str);
            rbusMessage_GetInt32(request, &commit);
            result = func->setCommit(sessionId, writeID, commit, func->setCommit_data);
            rbusMessage_Init(response);
            tmp = result;
            rbusMessage_SetInt32(*response, tmp); //result
            return DBUS_HANDLER_RESULT_HANDLED;
        }
        else if(!strncmp(method, METHOD_SETPARAMETERATTRIBUTES, MAX_METHOD_NAME_LENGTH) && func->setParameterAttributes)
        {
            parameterAttributeStruct_t * parameterAttribute = 0;
            int i = 0, sessionId = 0, param_size = 0, result = 0, tmp = 0;
            rbusMessage_GetInt32(request, &sessionId);
            rbusMessage_GetInt32(request, &param_size);
            if(param_size > 0)
            {
                parameterAttribute = bus_info->mallocfunc(param_size*sizeof(parameterAttributeStruct_t));
                memset(parameterAttribute, 0, param_size*sizeof(parameterAttributeStruct_t));
            }
            for(i = 0; i < param_size; i++)
            {
                rbusMessage_GetString(request, (const char**)&parameterAttribute[i].parameterName);
                rbusMessage_GetInt32(request, (int32_t*)&parameterAttribute[i].notificationChanged);
                rbusMessage_GetInt32(request, (int32_t*)&parameterAttribute[i].notification);
                rbusMessage_GetInt32(request, (int32_t*)&parameterAttribute[i].access);
                rbusMessage_GetInt32(request, (int32_t*)&parameterAttribute[i].accessControlChanged);
                rbusMessage_GetInt32(request, (int32_t*)&parameterAttribute[i].accessControlBitmask);
                rbusMessage_GetInt32(request, (int32_t*)&parameterAttribute[i].RequesterID);
            }
            result = func->setParameterAttributes(sessionId, parameterAttribute, param_size, func->setParameterAttributes_data);
            rbusMessage_Init(response);
            tmp = result;
            rbusMessage_SetInt32(*response, tmp); //result
            bus_info->freefunc(parameterAttribute);
            return DBUS_HANDLER_RESULT_HANDLED;
        }
        else if(!strncmp(method, METHOD_GETPARAMETERNAMES, MAX_METHOD_NAME_LENGTH) && func->getParameterNames)
        {
            int i = 0,size = 0;
            int32_t requestedDepth, rowNamesOnly = 0, result = 0, tmp = 0;
            char * parameterName = 0;
            parameterInfoStruct_t **val = 0;
            rbusMessage_GetString(request, (const char**)&parameterName);
            rbusMessage_GetInt32(request, &requestedDepth); 
            rbusMessage_GetInt32(request, &rowNamesOnly);
            rbusMessage_Init(response);
            result = func->getParameterNames(parameterName, requestedDepth == -1, &size, &val, func->getParameterNames_data );
            tmp = result;
            rbusMessage_SetInt32(*response, tmp); //result
            if( tmp == CCSP_SUCCESS)
            {
                int actualCount = 0;
                char buf[CCSP_BASE_PARAM_LENGTH];
                int inst_num;
                int type;
                                
                if(rowNamesOnly)
                {
                    for(i = 0; i < size; i++)
                    {
                        type = CcspBaseIf_getObjType(parameterName, val[i]->parameterName, &inst_num, buf);
                        if(type == CCSP_BASE_INSTANCE)
                            actualCount++;
                    }
                }
                else
                {
                    actualCount = size;
                }

                rbusMessage_SetInt32(*response, actualCount);

                for(i = 0; i < size; i++)
                {
                    
                    type = CcspBaseIf_getObjType(parameterName, val[i]->parameterName, &inst_num, buf);

                    RBUS_LOG("%s Param [%d] Name=%s, Writable=%d, Type=%d\n", __FUNCTION__, i, val[i]->parameterName, val[i]->writable, type);

                    if(rowNamesOnly)
                    {
                        if(type != CCSP_BASE_INSTANCE)
                            continue;
                        rbusMessage_SetInt32(*response, (int32_t)inst_num); /*instancen number*/    
                        rbusMessage_SetString(*response, ""); /*alias -- which is unsupported in ccsp*/    
                    }
                    else
                    {
                        rbusElementType_t elemType = 0;
                        rbusAccess_t accessFlags = 0;

                        /* determine element type */
                        if(type == CCSP_BASE_PARAM)
                        {
                            elemType = RBUS_ELEMENT_TYPE_PROPERTY;
                        }
                        else if(type == CCSP_BASE_INSTANCE)
                        {
                            elemType = 0; /*object*/
                        }
                        else if(type == CCSP_BASE_OBJECT)
                        {
                            /*there's no way to know completely if its a table or a plain object
                              the writable flag can be true for some table types but not all 
                              so the following might set static and dynamic tables (which are both read-only tables) to type object*/
                            if(val[i]->writable)
                                elemType = RBUS_ELEMENT_TYPE_TABLE;
                            else
                                elemType = 0;/*object*/
                        }

                        /* determine access flags */
                        accessFlags = RBUS_ACCESS_GET; /*can read everything */

                        if(elemType == RBUS_ELEMENT_TYPE_PROPERTY)
                        {
                            accessFlags |=  RBUS_ACCESS_SUBSCRIBE;  /*can subscribe to value-change events*/
                            if(val[i]->writable)
                                accessFlags |= RBUS_ACCESS_SET;
                        }
                        else if(elemType == RBUS_ELEMENT_TYPE_TABLE)
                        {
                            if(val[i]->writable)
                                accessFlags |= RBUS_ACCESS_ADDROW | RBUS_ACCESS_REMOVEROW;
                        }
                        else /*objects or rows*/
                        {   
                            if(val[i]->writable)
                                accessFlags |= RBUS_ACCESS_SET;
                        }

                        rbusMessage_SetString(*response, val[i]->parameterName);
                        rbusMessage_SetInt32(*response, (int32_t)elemType);
                        rbusMessage_SetInt32(*response, (int32_t)accessFlags);
                    }
                }
            }
            free_parameterInfoStruct_t(bus_info, size, val);
        }
        else if (!strncmp(method, METHOD_ADDTBLROW, MAX_METHOD_NAME_LENGTH) && func->AddTblRow)
        {
            int instanceNumber = 0, result = 0;
            int32_t tmp = 0, sessionId = 0;
            char *str = 0, *inst_str = 0;
            rbus_error_t err = RTMESSAGE_BUS_SUCCESS;
            rbusMessage_GetInt32(request, &sessionId);
            rbusMessage_GetString(request, (const char**)&str); //object name
            result = func->AddTblRow(sessionId, str, &instanceNumber , func->AddTblRow_data);
            if (result == CCSP_SUCCESS)
            {
                inst_str = (char*)bus_info->mallocfunc(strlen(str)+12);
                sprintf(inst_str, "%s.%d.", str, instanceNumber);
                if((err = rbus_addElement(bus_info->component_id, inst_str)) != RTMESSAGE_BUS_SUCCESS)
                {
                    RBUS_LOG_ERR("rbus_addElement: Component: %s Obj: %s Err: %d\n", bus_info->component_id, str, err);
                }
            }
            rbusMessage_Init(response);
            tmp = result;
            rbusMessage_SetInt32(*response, tmp); //result
            tmp = instanceNumber;
            rbusMessage_SetInt32(*response, tmp); //inst num
            return DBUS_HANDLER_RESULT_HANDLED;
        }
        else if (!strncmp(method, METHOD_DELETETBLROW, MAX_METHOD_NAME_LENGTH) && func->DeleteTblRow)
        {
            int result = 0;
            int32_t tmp = 0, sessionId = 0;
            char * str = 0;
            rbus_error_t err = RTMESSAGE_BUS_SUCCESS;
            rbusMessage_GetInt32(request, &sessionId);
            rbusMessage_GetString(request, (const char**)&str); //obj name
            result = func->DeleteTblRow(sessionId, str , func->DeleteTblRow_data);
            if (result == CCSP_SUCCESS)
            {
                if((err = rbus_removeElement(bus_info->component_id, str)) != RTMESSAGE_BUS_SUCCESS)
                {
                    RBUS_LOG_ERR("rbus_removeElement: Component: %s Obj: %s Err: %d\n", bus_info->component_id, str, err);
                }
            }
            rbusMessage_Init(response);
            tmp = result;
            rbusMessage_SetInt32(*response, tmp); //result
            return DBUS_HANDLER_RESULT_HANDLED;
        }
        else if (!strcmp(method, CCSP_PARAMETER_VALUE_CHANGE_SIGNAL) && func->parameterValueChangeSignal)
        {
            parameterSigStruct_t *val = 0;
            int param_size = 0, i = 0;

            rbusMessage_GetInt32(request, (int32_t*)&param_size);

            if(param_size)
            {
                val = bus_info->mallocfunc(param_size*sizeof(parameterSigStruct_t ));
                memset(val, 0, param_size*sizeof(parameterSigStruct_t ));

                for(i = 0; i < param_size; i++)
                {
                    rbusMessage_GetString(request, &val[i].parameterName);
                    rbusMessage_GetString(request, &val[i].oldValue);
                    rbusMessage_GetString(request, &val[i].newValue);
                    rbusMessage_GetInt32(request, (int32_t*)&val[i].type);
                    rbusMessage_GetString(request, &val[i].subsystem_prefix);
                    rbusMessage_GetInt32(request, (int32_t*)&val[i].writeID);
                }

                func->parameterValueChangeSignal(val, param_size,func->parameterValueChangeSignal_data);

                bus_info->freefunc(val);
            }
        }
    }
    return 0;
}

static int webcfg_signal_rbus(const char * destination, const char * method, rbusMessage request, void * user_data, rbusMessage *response, const rtMessageHeader* hdr)
{
    UNREFERENCED_PARAMETER(response);
    (void) destination;
    (void) method;
    (void) hdr;
    int err = CCSP_Message_Bus_OK;
    CCSP_MESSAGE_BUS_INFO *bus_info =(CCSP_MESSAGE_BUS_INFO *) user_data;
    CCSP_Base_Func_CB* func = (CCSP_Base_Func_CB* )bus_info->CcspBaseIf_func;

    RBUS_LOG ("%s Received %s", __FUNCTION__, method);
    if ((func) && (func->webconfigSignal))
    {
        char* webconfig_data = 0;
        RBUS_LOG_ERR("Handling : %s from Component: %s", method, bus_info->component_id);
        rbusMessage_GetString(request, (char const**)&webconfig_data);
        func->webconfigSignal(webconfig_data, func->webconfigSignal_data);
    }
    return err;
}

static int tunnelStatus_signal_rbus(const char * destination, const char * method, rbusMessage request, void * user_data, rbusMessage *response, const rtMessageHeader* hdr)
{
    UNREFERENCED_PARAMETER(response);
    (void) destination;
    (void) method;
    (void) hdr;
    int err = CCSP_Message_Bus_OK;
    CCSP_MESSAGE_BUS_INFO *bus_info =(CCSP_MESSAGE_BUS_INFO *) user_data;
    CCSP_Base_Func_CB* func = (CCSP_Base_Func_CB* )bus_info->CcspBaseIf_func;

    RBUS_LOG ("%s Received %s", __FUNCTION__, method);
    if (func->TunnelStatus)
    {
        char* tunnel_status = 0;
        RBUS_LOG_ERR("Handling : %s from Component: %s", method, bus_info->component_id);
        rbusMessage_GetString(request, (char const**)&tunnel_status);
        func->TunnelStatus(tunnel_status, func->TunnelStatus_data);
    }
    return err;
}

static int wifiDbStatus_signal_rbus(const char * destination, const char * method, rbusMessage request, void * user_data, rbusMessage *response, const rtMessageHeader* hdr)
{
    UNREFERENCED_PARAMETER(response);
    (void) destination;
    (void) method;
    (void) hdr;
    int err = CCSP_Message_Bus_OK;
    CCSP_MESSAGE_BUS_INFO *bus_info =(CCSP_MESSAGE_BUS_INFO *) user_data;
    CCSP_Base_Func_CB* func = (CCSP_Base_Func_CB* )bus_info->CcspBaseIf_func;

    RBUS_LOG ("%s Received %s", __FUNCTION__, method);
    if ((func) && (func->WifiDbStatus))
    {
        char* wifidb_status = 0;
        RBUS_LOG_ERR("Handling : %s from Component: %s", method, bus_info->component_id);
        rbusMessage_GetString(request, (char const**)&wifidb_status);
        func->WifiDbStatus(wifidb_status, func->WifiDbStatus_data);
    }
    return err;
}

static int telemetry_send_signal_rbus(const char * destination, const char * method, rbusMessage request, void * user_data, rbusMessage *response, const rtMessageHeader* hdr)
{
    UNREFERENCED_PARAMETER(response);
    (void) destination;
    (void) method;
    (void) hdr;
    int err = CCSP_Message_Bus_OK;
    char* telemetry_data = NULL;
    CCSP_MESSAGE_BUS_INFO *bus_info =(CCSP_MESSAGE_BUS_INFO *) user_data;
    CCSP_Base_Func_CB* func = (CCSP_Base_Func_CB* )bus_info->CcspBaseIf_func;

    if ((func) && (func->telemetryDataSignal))
    {
        rbusMessage_GetString(request, (const char**)&telemetry_data);
        func->telemetryDataSignal(telemetry_data,func->telemetryDataSignal_data);
    }
    return err;
}
static int
CCSP_Message_Bus_Register_Path_Priv_rbus
(
 void* bus_handle,
 rbus_callback_t funcptr,
 void * user_data
)
{
    UNREFERENCED_PARAMETER(user_data);
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    bus_info->rbus_callback = (void *)funcptr;
    return CCSP_Message_Bus_OK;
}

/*
 *  Added to support rbus value-change detection
 *  This will check if eventName refers to a parameter (and not an event like CCSP_SYSTEM_READY_SIGNAL)
 *  If its a parameter it will assume value-change is desired and pass the data to Ccsp_RbusValueChange api to handle it
 */
static int cssp_event_subscribe_override_handler_rbus(
    char const* object,
    char const* eventName,
    char const* listener,
    int added,
    const rbusMessage payload,
    void* userData)
{
    size_t slen;
    (void)object;

    //determine if eventName is a parameter
    slen = strlen(eventName);
    if(slen < 7 || strncmp(eventName, "Device.", 7) != 0 || eventName[slen-1] == '!')
    {
        //not a parameter so return special error so rbus_core will search its registered event list for actual events like CCSP_SYSTEM_READY_SIGNAL
        CcspTraceWarning(("%s: ignored %s\n", __FUNCTION__, eventName));
        return RTMESSAGE_BUS_SUBSCRIBE_NOT_HANDLED;
    }

    CcspTraceDebug(("%s: %s\n", __FUNCTION__, eventName));

    if(added)
    {
        Ccsp_RbusValueChange_Subscribe(userData, listener, eventName, payload);
    }
    else
    {
        Ccsp_RbusValueChange_Unsubscribe(userData, listener, eventName, payload);
    }

    return RTMESSAGE_BUS_SUCCESS;
}

int 
CCSP_Message_Bus_Register_Path2
(
    void* bus_handle,
    const char* path,
    DBusObjectPathMessageFunction funcptr,
    void * user_data
)
{
    UNREFERENCED_PARAMETER(user_data);
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;

    pthread_mutex_lock(&bus_info->info_mutex);
    bus_info->thread_msg_func = funcptr;
    pthread_mutex_unlock(&bus_info->info_mutex);

    // !!! regardless of what funcptr is, 
    // !!! it is always registered with thread_path_message_func for message handling
    return CCSP_Message_Bus_Register_Path_Priv
               (
                   bus_handle, 
                   path, 
                   thread_path_message_func, 
                   bus_handle
                );
}

static int 
analyze_reply
(
    DBusMessage *message,
    DBusMessage *reply,
    DBusMessage **result
)
{
    int ret  = CCSP_Message_Bus_ERROR;
    int type = dbus_message_get_type (reply);

    if (type == DBUS_MESSAGE_TYPE_METHOD_RETURN)
    {
        if(result) *result =  reply;
        else dbus_message_unref(reply);

        ret = CCSP_Message_Bus_OK;
    }
    else
    {
        const char *err = dbus_message_get_error_name (reply);

        CcspTraceWarning(("<%s>: DbusSend error='%s', msg='%s'\n", 
                          __FUNCTION__, err, dbus_message_get_destination(message)));

        dbus_message_unref (reply);

        if(strcmp(err, "org.freedesktop.DBus.Error.ServiceUnknown") == 0)
            ret = CCSP_MESSAGE_BUS_NOT_EXIST;
        else 
            ret = CCSP_MESSAGE_BUS_NOT_SUPPORT;
    }
    
    return ret;
}

/*send a string _WITHOUT_ return param on specified connection*/
int
CCSP_Message_Bus_Send_Str
(
    DBusConnection *conn,
    char* component_id,
    const char* path,
    const char* interface,
    const char* method,
    char* request
)
{
    DBusMessage *message = NULL;
    DBusMessage *reply   = NULL;
    DBusPendingCall *pcall = NULL;
    CCSP_MESSAGE_BUS_CB_DATA *cb_data = NULL;

    int ret = CCSP_Message_Bus_ERROR;
    //    static int ct = 0;

    // construct base message
    message = dbus_message_new_method_call
                  (
                      component_id,
                      path,
                      interface,
                      method
                   );
    if ( ! message )
    {
        CcspTraceError(("<%s>: No memory\n", __FUNCTION__));
        ret = CCSP_Message_Bus_OOM;
        goto EXIT;
    }

    cb_data = (CCSP_MESSAGE_BUS_CB_DATA *)malloc(sizeof(CCSP_MESSAGE_BUS_CB_DATA));
    if(cb_data == NULL)
    {
        CcspTraceError(("<%s>: No memory\n", __FUNCTION__));
        ret = CCSP_Message_Bus_OOM;
        goto EXIT;
    }
    cb_data->message = message;
    cb_data->succeed = 0;

    // append and send request
    dbus_message_append_args (message, DBUS_TYPE_STRING, &request,
                              DBUS_TYPE_INVALID);
    if (dbus_connection_send_with_reply(conn, message, &pcall, 0x7fffffff) == 0 || pcall == NULL)
    {
        CcspTraceError(("<%s>: dbus_connection_send_with_reply fail\n", __FUNCTION__));
        ret = CCSP_Message_Bus_OOM;
        goto EXIT;
    }

    // get reply
    dbus_connection_lock(conn);
    reply = dbus_pending_call_steal_reply(pcall);

    if( ! reply)
    {
        // wait for it once again with timeout
        struct timespec timeout = { 0, 0 };

        pthread_mutex_init(&cb_data->count_mutex, NULL);
        pthread_cond_init (&cb_data->count_threshold_cv, NULL);

        if (NewCondVar(&cb_data->count_threshold_cv) == -1)
        {
            dbus_connection_unlock(conn);
            CcspTraceError(("<%s>: Couldn't initialize condition variable!\n", __FUNCTION__));
            ret = CCSP_Message_Bus_OOM;
            goto EXIT;
        }

        pthread_mutex_lock(&cb_data->count_mutex);
        dbus_pending_call_set_notify (pcall, ccsp_msg_check_resp_sync, (void *)cb_data, NULL);

        reply = dbus_pending_call_steal_reply(pcall);
        dbus_connection_unlock(conn);

        NewTimeout(&timeout, CCSP_MESSAGE_BUS_SEND_STR_TIMEOUT_SECONDS, 0);
        if (reply)
        {
            ret = analyze_reply(message, reply, NULL);
        }        
        else if(pthread_cond_timedwait(&cb_data->count_threshold_cv, &cb_data->count_mutex, &timeout) != 0)
        {
            dbus_pending_call_cancel(pcall);
            //            CcspTraceWarning(("<%s>: reply pthread_cond_timedwait timed out\n", __FUNCTION__));

            //in case ccsp_msg_check_resp_sync is called between dbus_pending_call_cancel and pthread_cond_timedwait
            // -> Cannot happen if it is a timed wait, increase the timeout amount instead. RTian 07/08/2013
            // CCSP_Msg_SleepInMilliSeconds(500);
        }
        else
        {
            dbus_connection_lock(conn);
            reply = dbus_pending_call_steal_reply(pcall);
            dbus_connection_unlock(conn);
            if(reply)
            {
                ret = analyze_reply(message, reply, NULL);
            }
        }
        pthread_mutex_unlock(&cb_data->count_mutex);

        pthread_mutex_destroy(&cb_data->count_mutex);
        pthread_cond_destroy(&cb_data->count_threshold_cv);
    }
    else
    {
        dbus_connection_unlock(conn);
        ret = analyze_reply(message, reply, NULL);
    }

    if(pcall) dbus_pending_call_unref (pcall);

EXIT:
    //    if(reply) dbus_message_unref(reply);
    //    if(pcall) dbus_pending_call_unref(pcall);
    if(message) dbus_message_unref(message);
    if(cb_data) free(cb_data);
    return ret;
}

/*This is complicated.
Because we have to handle multi-thread send/receive and connection disconnect issue, and dbus provide little help*/
int
CCSP_Message_Bus_Send_Msg
(
    void* bus_handle,
    DBusMessage *message,
    int timeout_seconds,
    DBusMessage **result
)
{
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    DBusConnection *conn = NULL;
    DBusMessage *reply = NULL;
    DBusPendingCall *pcall = NULL;
    CCSP_MESSAGE_BUS_CB_DATA *cb_data = NULL;
    int ret  = CCSP_Message_Bus_ERROR;
    int i = 0;
    int rc = 0;

    *result = NULL;  // return value

    /*to support daemon redundency*/
    // connect to first connection on buf_info->connection[i]
    pthread_mutex_lock(&bus_info->info_mutex);
    for(i = 0; i < CCSP_MESSAGE_BUS_MAX_CONNECTION; i++)
    {
        if(bus_info->connection[i].connected && bus_info->connection[i].conn )
        {
            conn = bus_info->connection[i].conn;
            //  dbus_connection_ref (conn);
            break;
        }

    }
    pthread_mutex_unlock(&bus_info->info_mutex);

    if(i ==  CCSP_MESSAGE_BUS_MAX_CONNECTION)
        return CCSP_MESSAGE_BUS_CANNOT_CONNECT;

    if ( ! dbus_connection_send_with_reply(conn, message, &pcall, 0x7fffffff) || ! pcall)
    {
        CcspTraceError(("<%s>: dbus_connection_send_with_reply fail\n", __FUNCTION__));

        /* Reconnect if disconnected */
        ccsp_msg_bus_reconnect(&((bus_info->connection[i])));

        ret = CCSP_Message_Bus_OOM;
        goto EXIT;
    }

    dbus_connection_lock(conn);
    reply = dbus_pending_call_steal_reply(pcall);
    if( ! reply)
    {
        // try again with a timed wait for reply
        struct timespec timeout = { 0, 0 };

        cb_data = (CCSP_MESSAGE_BUS_CB_DATA *)bus_info->mallocfunc(sizeof(CCSP_MESSAGE_BUS_CB_DATA));
        if( ! cb_data)
        {
            CcspTraceError(("<%s>: cb_data malloc fail \n", __FUNCTION__));
            dbus_connection_unlock(conn);
            ret = CCSP_Message_Bus_OOM;
            goto EXIT;
        }
        cb_data->message = message;
        cb_data->succeed = 0;

        if (pthread_mutex_init(&cb_data->count_mutex, NULL) != 0)
        {
            dbus_connection_unlock(conn);
            CcspTraceError(("<%s>: Couldn't initialize count_mutex!\n", __FUNCTION__));
            ret = CCSP_Message_Bus_OOM;
            goto EXIT;
        }
        if (NewCondVar(&cb_data->count_threshold_cv) == -1)
        {
            dbus_connection_unlock(conn);
            CcspTraceError(("<%s>: Couldn't initialize condition variable!\n", __FUNCTION__));
            ret = CCSP_Message_Bus_OOM;
            goto EXIT;
        }

        pthread_mutex_lock(&cb_data->count_mutex);
        dbus_pending_call_set_notify (pcall, ccsp_msg_check_resp_sync, (void *)cb_data, NULL);

        reply = dbus_pending_call_steal_reply(pcall);
        dbus_connection_unlock(conn);
        
        NewTimeout(&timeout, timeout_seconds + 1, 0);
        if (reply)
        {
            ret = analyze_reply(message, reply, result);
        }
        else if((rc = pthread_cond_timedwait(&cb_data->count_threshold_cv, &cb_data->count_mutex, &timeout)) != 0)
        {
            dbus_pending_call_cancel(pcall);
            //            CcspTraceWarning(("<%s>: pthread_cond_timedwait timeout\n", __FUNCTION__));

            // in case ccsp_msg_check_resp_sync is called between dbus_pending_call_cancel and pthread_cond_timedwait
            // CCSP_Msg_SleepInMilliSeconds(1000);

            ret = CCSP_MESSAGE_BUS_TIMEOUT;
        }
        else
        {
            dbus_connection_lock(conn);
            reply = dbus_pending_call_steal_reply(pcall);
            dbus_connection_unlock(conn);
            if (reply)
            {
                ret = analyze_reply(message, reply, result);
            }
            else {} // do nothing
        }
        pthread_mutex_unlock(&cb_data->count_mutex);

        pthread_mutex_destroy(&cb_data->count_mutex);
        pthread_cond_destroy(&cb_data->count_threshold_cv);
        bus_info->freefunc(cb_data);
    }
    else
    {
        dbus_connection_unlock(conn);
        ret = analyze_reply(message, reply, result);
    }

    if(pcall) dbus_pending_call_unref(pcall);

EXIT:
    //    dbus_connection_unref (conn);
    return ret;
}

/* Doesn't work for messages that need to be received and replied to by the same connection that sent them */
int 
CCSP_Message_Bus_Send_Msg_Block
(
    void* bus_handle,
    DBusMessage *message,
    int timeout_mseconds,
    DBusMessage **result
)
{
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    DBusConnection *conn = NULL;
    DBusMessage *reply = NULL;
    DBusError dbus_err;
    int ret  = CCSP_Message_Bus_ERROR;
    int i;

    *result = NULL;

    /*to support daemon redundency*/
    pthread_mutex_lock(&bus_info->info_mutex);
    for(i = 0; i < CCSP_MESSAGE_BUS_MAX_CONNECTION; i++)
    {
        if(bus_info->connection[i].connected && bus_info->connection[i].conn )
        {
            conn = bus_info->connection[i].conn;
            break;
        }
    }
    pthread_mutex_unlock(&bus_info->info_mutex);

    if(i ==  CCSP_MESSAGE_BUS_MAX_CONNECTION)
        return CCSP_MESSAGE_BUS_CANNOT_CONNECT;

    dbus_error_init(&dbus_err);
    reply = dbus_connection_send_with_reply_and_block(conn, message, timeout_mseconds, &dbus_err);
    if (dbus_error_is_set(&dbus_err))
    {
        CcspTraceError(("<%s> error: %s\n", __FUNCTION__, dbus_err.message));

        /* Reconnect if disconnected */
        ccsp_msg_bus_reconnect(&((bus_info->connection[i])));

        if(reply) dbus_message_unref(reply);
    }
    else if (reply)
    {
        ret = analyze_reply(message, reply, result);
    }
    dbus_error_free(&dbus_err);

    return ret;
}
