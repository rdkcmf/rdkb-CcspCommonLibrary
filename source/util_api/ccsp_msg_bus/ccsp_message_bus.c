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
#include <string.h>
#include <pthread.h>
#include <dbus/dbus.h>
#include <dbus/dbus-mainloop.h>
#include <ccsp_message_bus.h>
#include "ccsp_base_api.h"
#include "ccsp_trace.h"

#ifndef WIN32 
#include <sys/time.h>
#include <time.h>
#else
#include <sys/timeb.h>
const int NANOSEC_PER_MILLISEC = 1000000;
#endif

#ifdef _DEBUG
// #define _DEBUG_LOCAL_
#endif

// EXTERNAL
extern void _dbus_connection_lock (DBusConnection *connection);
extern void _dbus_connection_unlock (DBusConnection *connection);

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
static pthread_t thread_dbus_connect = 0;
static pthread_t thread_dbus_loop = 0;
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

static dbus_bool_t       connection_watch_callback(DBusWatch*, unsigned int, void*);
static dbus_bool_t       add_watch(DBusWatch*, void*);
static void              remove_watch(DBusWatch*, void*);
static void              connection_timeout_callback(DBusTimeout*, void*);
static dbus_bool_t       add_timeout(DBusTimeout*, void*);
static void              remove_timeout(DBusTimeout*, void*);
static CData*            cdata_new(DBusLoop*, DBusConnection*);
static void              cdata_free(void *data);
static void              path_unregistered_func(DBusConnection*, void*);
static void              dispatch_status_func(DBusConnection*, DBusDispatchStatus, void*);
static DBusHandlerResult thread_path_message_func(DBusConnection*, DBusMessage*, void*);
static void              ccsp_msg_bus_reconnect(CCSP_MESSAGE_BUS_CONNECTION *);
static DBusHandlerResult filter_func(DBusConnection*, DBusMessage*, void*);
static dbus_bool_t       ccsp_connection_setup(DBusLoop*, DBusConnection*);
static void              ccsp_msg_check_resp_sync(DBusPendingCall*, void*);
static void              CCSP_Message_Bus_Strip(char * str);
static void*             CCSP_Message_Bus_Loop_Thread(void * DBusLoopPtr);
static void*             CCSP_Message_Bus_Connect_Thread(void * ccsp_msg_bus_connection_ptr);
static void*             CCSP_Message_Bus_Process_Thread(void * ccsp_bus_info_ptr);
static void              append_event_info(char*, const char*, const char*, const char*, const char*);
static int               CCSP_Message_Bus_Register_Event_Priv(DBusConnection*, const char*, const char*, const char*, const char*, int);
static int               CCSP_Message_Save_Register_Event(void*, const char*, const char*, const char*, const char*);
static int               CCSP_Message_Bus_Register_Path_Priv(void*, const char*, DBusObjectPathMessageFunction, void*);
static int               analyze_reply(DBusMessage*, DBusMessage*, DBusMessage**);

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


static dbus_bool_t 
connection_watch_callback 
(
    DBusWatch     *watch,
    unsigned int   condition,
    void          *data
)
{
    return dbus_watch_handle (watch, condition);
}

static dbus_bool_t 
add_watch 
(
    DBusWatch *watch,
    void      *data
)
{
    CData *cd = data;
    
    return _dbus_loop_add_watch 
               (
                   cd->loop,
                   watch,
                   connection_watch_callback,
                   cd, 
                   NULL
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
    
    _dbus_loop_remove_watch 
        (
             cd->loop,
             watch, 
             connection_watch_callback, 
             cd
         );
}

static void 
connection_timeout_callback 
(
    DBusTimeout   *timeout,
    void          *data
)
{
    /* Can return FALSE on OOM but we just let it fire again later */
    dbus_timeout_handle (timeout);
}

static dbus_bool_t 
add_timeout 
(
    DBusTimeout *timeout,
    void        *data
)
{
    CData *cd = data;

    return _dbus_loop_add_timeout 
               (
                   cd->loop,
                   timeout, 
                   connection_timeout_callback, 
                   cd, 
                   NULL
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

    _dbus_loop_remove_timeout 
        (
            cd->loop,
            timeout, 
            connection_timeout_callback, 
            cd
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
    _dbus_loop_ref (cd->loop);

    return cd;
}

static void 
cdata_free 
(
    void *data
)
{
    CData *cd = data;

    dbus_connection_unref (cd->connection);
    _dbus_loop_unref (cd->loop);

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
    /* connection was finalized */
}

static void 
dispatch_status_func
(
    DBusConnection    *connection,
    DBusDispatchStatus new_status,
    void              *data
)
{
    DBusLoop *loop = data;

    if (new_status != DBUS_DISPATCH_COMPLETE)
    {
        while ( ! _dbus_loop_queue_dispatch (loop, connection))
            _dbus_wait_for_memory ();
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
    CCSP_MESSAGE_BUS_CONNECTION *conn
) 
{
    char *msg = NULL; int ret = 0;

    if(thread_dbus_connect && (ret = pthread_join(thread_dbus_connect, (void **)&msg)) != 0) {
        CcspTraceError(("<%s>: thread connect join returned %d with error %s\n", __FUNCTION__, ret, msg));
    }

    CcspTraceWarning(("<%s>: Re-establishing connection...", __FUNCTION__));
    pthread_create(&thread_dbus_connect, NULL, CCSP_Message_Bus_Connect_Thread, (void *)conn); 
    CcspTraceWarning(("Ok.\n"));

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
                
                pthread_mutex_lock(&bus_info->info_mutex);
                connection->connected = 0;
                pthread_mutex_unlock(&bus_info->info_mutex);
                
                if(bus_info->run)
                {
                    // This is not normal
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
    DBusConnection *connection
)
{
    CData *cd = NULL;

    // dispatch_status, returns, or will wait for memory
    dbus_connection_set_dispatch_status_function 
        (
            connection, 
            dispatch_status_func,
            loop, 
            NULL
         );

    // watch
    cd = cdata_new (loop, connection);
    if (cd == NULL) goto NO_MEM;

    /* Because dbus-mainloop.c checks dbus_timeout_get_enabled(),
     * dbus_watch_get_enabled() directly, we don't have to provide
     * "toggled" callbacks.
     */

    if ( ! dbus_connection_set_watch_functions 
               (
                   connection,
                   add_watch,
                   remove_watch,
                   NULL,
                   cd, 
                   cdata_free
                ))
        goto NO_MEM;

    // timeout
    cd = cdata_new (loop, connection);
    if (cd == NULL) goto NO_MEM;
    
    if ( ! dbus_connection_set_timeout_functions 
               (
                   connection,
                   add_timeout,
                   remove_timeout,
                   NULL,
                   cd, 
                   cdata_free
                ))
        goto NO_MEM;

    // check dispatch status
    if (dbus_connection_get_dispatch_status (connection) != DBUS_DISPATCH_COMPLETE)
    {
        if ( ! _dbus_loop_queue_dispatch (loop, connection))
            goto NO_MEM;
    }

    return TRUE;

NO_MEM:
    if (cd) cdata_free (cd);

    // set everything to NULL
    dbus_connection_set_dispatch_status_function (connection, NULL, NULL, NULL);
    dbus_connection_set_watch_functions (connection, NULL, NULL, NULL, NULL, NULL);
    dbus_connection_set_timeout_functions (connection, NULL, NULL, NULL, NULL, NULL);

    return FALSE;
}

static void
ccsp_msg_check_resp_sync 
(
    DBusPendingCall *pcall,
    void *user_data
)
{
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
    void *ptr
)
{
    DBusLoop *loop = (DBusLoop *)ptr;
    _dbus_loop_run (loop); // main loop, does _ref and _unref
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
            continue;
        }
        
        if ( ! ccsp_connection_setup (connection->loop, conn_new))
        {
            CcspTraceError(("<%s> Couldn't ccsp_connection_setup loop!\n", __FUNCTION__));
            dbus_error_free (&error);
            dbus_connection_close(conn_new);
            dbus_connection_unref (conn_new);
            continue;
        }

        if ( ! dbus_bus_register (conn_new, &error))
        {
            CcspTraceError(("<%s> Failed to register connection to bus at %s: %s\n",
                            __FUNCTION__, connection->address, error.message));
            dbus_error_free (&error);
            dbus_connection_close(conn_new);
            dbus_connection_unref (conn_new);
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
            continue;
        }

        // Everything is ok
        dbus_error_free (&error);
        break;
    }

    // save new connection
    conn_old = connection->conn;

    pthread_mutex_lock(&bus_info->info_mutex);
    connection->conn = conn_new;
    connection->connected = 1;
    pthread_mutex_unlock(&bus_info->info_mutex);

    // signal the waiting call in Bus_Init or Reconnect
    pthread_mutex_lock(&bus_info->msg_mutex);
    pthread_cond_signal(&bus_info->msg_threshold_cv);
    pthread_mutex_unlock(&bus_info->msg_mutex);

    if(conn_old)
    {
        dbus_connection_close(conn_old);
        // RTian 5/3/2013        CCSP_Msg_SleepInMilliSeconds(2000);
        dbus_connection_unref(conn_old);
    }

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

#ifndef WIN32 
    struct timeval now;
    struct timespec timeout;
#else
    struct timespec timeout = { 0, 0 };
    struct _timeb currSysTime;
#endif
    
    unsigned long                                 curTime        = 0;
    unsigned long                                 preTime        = 0;

    preTime  = time(NULL);

    while(bus_info->run)
    {
        // block the loop to save CPU time
        pthread_mutex_lock(&bus_info->msg_mutex);        
#ifndef WIN32 
        gettimeofday(&now, NULL);
        timeout.tv_sec = now.tv_sec + 1;
        timeout.tv_nsec = now.tv_usec * 1000 + 20000000; // total of ~1.02s
#else
        _ftime(&currSysTime);
        timeout.tv_sec = currSysTime.time;
        timeout.tv_nsec = NANOSEC_PER_MILLISEC * (currSysTime.millitm + 30) ;
#endif
        pthread_cond_timedwait(&bus_info->msg_threshold_cv, &bus_info->msg_mutex, &timeout);

        // now the processing, after either received a signal or timed out
        entry = CcspMsgQueuePopEntry(bus_info->msg_queue);
        while(entry)
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
        curTime = time(NULL);
        if ( ( curTime-preTime ) > CCSP_MESSAGE_BUS_TIMEOUT_MAX_SECOND  ){
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
#ifndef WIN32 
    struct timeval tm;
    tm.tv_sec = milliSecond/1000;
    tm.tv_usec = (milliSecond%1000)*1000;
    select(0, NULL, NULL, NULL, &tm);
#else
    _dbus_sleep_milliseconds(milliSecond);
#endif
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
    
#ifndef WIN32 
    struct timeval now;
    struct timespec timeout;
#else
    struct timespec timeout = { 0, 0 };
    struct _timeb currSysTime;
#endif

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
        return -1;
    }
    memset(bus_info, 0, sizeof(CCSP_MESSAGE_BUS_INFO));
    *bus_handle = bus_info; // return

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

    //    CcspTraceDebug(("<%s>: component id = '%s'\n", __FUNCTION__, bus_info->component_id));

    // init var, mutex, msg_queue
    pthread_mutex_init(&bus_info->info_mutex, NULL);
    bus_info->msg_queue = NULL;
    bus_info->msg_queue = (CCSP_MSG_QUEUE*)bus_info->mallocfunc(sizeof(CCSP_MSG_QUEUE));
    if( ! bus_info->msg_queue)
    {
        CcspTraceError(("<%s>: No memory\n", __FUNCTION__));
        return -1;
    }
    CcspMsgQueueInit(bus_info->msg_queue);
    pthread_mutex_init(&bus_info->msg_mutex, NULL);
    pthread_cond_init (&bus_info->msg_threshold_cv, NULL);
 
    pthread_mutex_lock(&bus_info->info_mutex);
    bus_info->run = 1;
    pthread_mutex_unlock(&bus_info->info_mutex);

    // init the default Dbus threads
    if(ccsp_bus_ref_count == 0) dbus_threads_init_default();
    ccsp_bus_ref_count++;

    // Start loop and connect threads to the socket address(es)
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
        bus_info->connection[count].loop = _dbus_loop_new();
        pthread_create
            (
                &thread_dbus_loop, 
                NULL, 
                CCSP_Message_Bus_Loop_Thread, 
                (void *)(bus_info->connection[count].loop)
             );

        bus_info->connection[count].bus_info_ptr = (void *)bus_info;
        pthread_create
            (
                &thread_dbus_connect, 
                NULL, 
                CCSP_Message_Bus_Connect_Thread, 
                (void *)(&(bus_info->connection[count]))
             );
        count++;
    }

    //    CcspTraceDebug(("<%s>: count = %d\n", __FUNCTION__, count));

    fclose(fp);

    /*
#ifndef WIN32 
    gettimeofday(&now, NULL);
    timeout.tv_sec = now.tv_sec + 3;
    timeout.tv_nsec = now.tv_usec ; 
#else
    _ftime(&currSysTime);
    timeout.tv_sec = currSysTime.time +2;
    timeout.tv_nsec = NANOSEC_PER_MILLISEC * (currSysTime.millitm + 2000) ;
#endif
    // count  = pthread_cond_timedwait(&bus_info->msg_threshold_cv, &bus_info->msg_mutex, &timeout); 
    */

    // Change to pthread_cond_wait() since connect thread may fail under high cpu usage 
    // i.e., wait until the connect thread succeeds and signal msg_threshold_cv
    pthread_mutex_lock(&bus_info->msg_mutex);
    count  = pthread_cond_wait(&bus_info->msg_threshold_cv, &bus_info->msg_mutex);
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


void 
CCSP_Message_Bus_Exit
(
    void *bus_handle
)
{
    int i;
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;

    /* Set run to 0, and Trigger to CCSP_Message_Bus_Process_Thread to exit */
    pthread_mutex_lock(&bus_info->info_mutex);
    bus_info->run = 0;
    pthread_mutex_unlock(&bus_info->info_mutex);
    pthread_mutex_lock(&bus_info->msg_mutex);
    pthread_cond_signal(&bus_info->msg_threshold_cv);
    pthread_mutex_unlock(&bus_info->msg_mutex);

    { // join all the threads started in init

        char *msg = NULL; int ret = 0;

        if(thread_dbus_connect && (ret = pthread_join(thread_dbus_connect, (void **)&msg)) != 0) {
            CcspTraceError(("<%s>: thread connect join returned %d with error %s\n", __FUNCTION__, ret, msg));
        }
        
        if(thread_dbus_process && (ret = pthread_join(thread_dbus_process, (void **)&msg)) != 0) {
            CcspTraceError(("<%s>: thread process join returned %d with error %s\n", __FUNCTION__, ret, msg));
        }

        if(thread_dbus_deadlock_monitor && (ret = pthread_join(thread_dbus_deadlock_monitor, (void **)&msg)) != 0) {
            CcspTraceError(("<%s>: thread deadlock monitor join returned %d with error %s\n", __FUNCTION__, ret, msg));
        }

        // the loop thread takes a long time to exit
        // so it is skipped and will let OS to clean it up
        /*
        if(thread_dbus_loop && (ret = pthread_join(thread_dbus_loop, (void **)&msg)) != 0) {
            CcspTraceError(("<%s>: thread loop join returned %d with error %s\n", __FUNCTION__, ret, msg));
        }
        */
    }

    for(i = 0; i < CCSP_MESSAGE_BUS_MAX_CONNECTION; i++)
    {
        if(bus_info->connection[i].conn )
        {
            dbus_connection_close(bus_info->connection[i].conn);
            dbus_connection_unref(bus_info->connection[i].conn) ;
        }

        // CCSP_Msg_SleepInMilliSeconds(1000);

        if(bus_info->connection[i].loop )
        {
            _dbus_loop_quit(bus_info->connection[i].loop);
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
    if(ccsp_bus_ref_count == 0) dbus_shutdown();

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

    if(sender)
    {
        sprintf(buf,",sender='%s'", sender);
        strcat(destination, buf);
    }

    if(path)
    {
        sprintf(buf,",path='%s'", path);
        strcat(destination, buf);
    }

    if(interface)
    {
        sprintf(buf,",interface='%s'", interface);
        strcat(destination, buf);
    }

    if(event_name)
    {
        sprintf(buf,",member='%s'", event_name);
        strcat(destination, buf);
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

    strcpy(tmp, "type='signal'");
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
                strcpy(bus_info->filters[i].path, path);
            }

            if(interface)
            {
                bus_info->filters[i].interface = bus_info->mallocfunc(strlen(interface)+1);
                strcpy(bus_info->filters[i].interface, interface);
            }

            if(event_name)
            {
                bus_info->filters[i].event = bus_info->mallocfunc(strlen(event_name)+1);
                strcpy(bus_info->filters[i].event, event_name);
            }
            if(sender)
            {
                bus_info->filters[i].sender = bus_info->mallocfunc(strlen(sender)+1);
                strcpy(bus_info->filters[i].sender, sender);
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

    int i, j;

    dbus_error_init (&error);
    pthread_mutex_lock(&bus_info->info_mutex);
    for(i = 0; i < CCSP_MESSAGE_BUS_MAX_PATH; i++)
    {
        if(bus_info->path_array[i].path == NULL)
        {
            bus_info->path_array[i].path = bus_info->mallocfunc(strlen(path)+1);
            strcpy(bus_info->path_array[i].path, path);
            bus_info->path_array[i].user_data = user_data ;
            bus_info->path_array[i].echo_vtable.unregister_function = path_unregistered_func;
            bus_info->path_array[i].echo_vtable.message_function = funcptr;

            break;
        }
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

int 
CCSP_Message_Bus_Register_Path2
(
    void* bus_handle,
    const char* path,
    DBusObjectPathMessageFunction funcptr,
    void * user_data
)
{
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
    int type = 0;

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
    _dbus_connection_lock(conn);
    reply = dbus_pending_call_steal_reply(pcall);

    if( ! reply) 
    {
        // wait for it once again with timeout

#ifndef WIN32 
        struct timeval now;
        struct timespec timeout;
#else
        struct timespec timeout = { 0, 0 };
        struct _timeb currSysTime;
#endif
        pthread_mutex_init(&cb_data->count_mutex, NULL);
        pthread_cond_init (&cb_data->count_threshold_cv, NULL);

        pthread_mutex_lock(&cb_data->count_mutex);
        dbus_pending_call_set_notify (pcall, ccsp_msg_check_resp_sync, (void *)cb_data, NULL);

        _dbus_connection_unlock(conn);

#ifndef WIN32 
        gettimeofday(&now, NULL);
        //        timeout.tv_sec = now.tv_sec + 1;
        timeout.tv_sec = now.tv_sec + 2;
        timeout.tv_nsec = now.tv_usec * 1000;
#else
        _ftime(&currSysTime);
        timeout.tv_sec = currSysTime.time;
        timeout.tv_nsec = NANOSEC_PER_MILLISEC * currSysTime.millitm;
        //        timeout.tv_sec += 1;
        timeout.tv_sec += 2;
#endif

        if(pthread_cond_timedwait(&cb_data->count_threshold_cv, &cb_data->count_mutex, &timeout) != 0)
        {
            dbus_pending_call_cancel(pcall);
            //            CcspTraceWarning(("<%s>: reply pthread_cond_timedwait timed out\n", __FUNCTION__));

            //in case ccsp_msg_check_resp_sync is called between dbus_pending_call_cancel and pthread_cond_timedwait
            // -> Cannot happen if it is a timed wait, increase the timeout amount instead. RTian 07/08/2013
            // CCSP_Msg_SleepInMilliSeconds(500); 
        }
        else
        {
            reply = dbus_pending_call_steal_reply(pcall);
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
        _dbus_connection_unlock(conn);
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
    int i;

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

        if ( ! dbus_connection_get_is_connected(conn))
        {
            CcspTraceError(("<%s> not dbus_connection_get_is_connected \n", __FUNCTION__));

            pthread_mutex_lock(&bus_info->info_mutex);
            bus_info->connection[i].connected = 0;
            pthread_mutex_unlock(&bus_info->info_mutex);

            if(bus_info->run)
            {
                // this is not normal
                ccsp_msg_bus_reconnect(&((bus_info->connection[i])));
            }
        }

        ret = CCSP_Message_Bus_OOM;
        goto EXIT;
    }

    _dbus_connection_lock(conn);
    reply = dbus_pending_call_steal_reply(pcall);
    if( ! reply)  
    {
        // try again with a timed wait for reply
#ifndef WIN32 
        struct timeval now;
        struct timespec timeout;
#else
        struct timespec timeout = { 0, 0 };
        struct _timeb currSysTime;
#endif
        cb_data = (CCSP_MESSAGE_BUS_CB_DATA *)bus_info->mallocfunc(sizeof(CCSP_MESSAGE_BUS_CB_DATA));
        if( ! cb_data)
        {
            CcspTraceError(("<%s>: cb_data malloc fail \n", __FUNCTION__));
            _dbus_connection_unlock(conn);
            ret = CCSP_Message_Bus_OOM;
            goto EXIT;
        }
        cb_data->message = message;
        cb_data->succeed = 0;

        pthread_mutex_init(&cb_data->count_mutex, NULL);
        pthread_cond_init (&cb_data->count_threshold_cv, NULL);

        pthread_mutex_lock(&cb_data->count_mutex);
        dbus_pending_call_set_notify (pcall, ccsp_msg_check_resp_sync, (void *)cb_data, NULL);

        _dbus_connection_unlock(conn);

#ifndef WIN32 
        gettimeofday(&now, NULL);
        timeout.tv_sec = now.tv_sec + (timeout_seconds + 1);
        timeout.tv_nsec = now.tv_usec * 1000;
#else
        _ftime(&currSysTime);
        timeout.tv_sec = currSysTime.time;
        timeout.tv_nsec = NANOSEC_PER_MILLISEC * currSysTime.millitm;
        timeout.tv_sec += (timeout_seconds + 1);
#endif
        if(pthread_cond_timedwait(&cb_data->count_threshold_cv, &cb_data->count_mutex, &timeout) != 0)
        {
            dbus_pending_call_cancel(pcall);
            //            CcspTraceWarning(("<%s>: pthread_cond_timedwait timeout\n", __FUNCTION__));

            // in case ccsp_msg_check_resp_sync is called between dbus_pending_call_cancel and pthread_cond_timedwait
            // CCSP_Msg_SleepInMilliSeconds(1000); 

            ret = CCSP_MESSAGE_BUS_TIMEOUT;
        }
        else
        {
            reply = dbus_pending_call_steal_reply(pcall);
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
        _dbus_connection_unlock(conn);
        ret = analyze_reply(message, reply, result);
    }

    if(pcall) dbus_pending_call_unref(pcall);

EXIT:
    //    dbus_connection_unref (conn);
    return ret;
}



/*not recommended  for multithread high speed app, it will crash */
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
    }
    else if (reply)
    {
        ret = analyze_reply(message, reply, result);
    }
    dbus_error_free(&dbus_err);

    return ret;
}

