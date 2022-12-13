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
#include <string.h>
#include <pthread.h>
#include <ccsp_message_bus.h>
#include <ccsp_base_api.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <dslh_definitions_database.h>
#include "ccsp_trace.h"
#include <rbus_message_bus.h>

#define MAX_EVENT_NAME_LEN 128

extern void* COSAGetMessageBusHandle();
extern int   CcspBaseIf_timeout_seconds;
extern int   CcspBaseIf_timeout_getval_seconds;

int   CcspBaseIf_timeout_protect_plus_seconds    = 5;
int   CcspBaseIf_deadlock_detection_time_normal_seconds = -1; //will be reassigned by dbus initiate function
int   CcspBaseIf_deadlock_detection_time_getval_seconds = -1; //will be reassigned by dbus initiate function

int deadlock_detection_enable                           = 0;
CCSP_DEADLOCK_DETECTION_INFO    deadlock_detection_info = {{{0}}, NULL, NULL, 0, 0, 0, 0};;
DEADLOCK_ARRAY*  deadlock_detection_log                 = NULL; //this variable is protected by mutex lock in deadlock_detection_info
unsigned int deadlock_detection_log_index               = 0;

void CcspBaseIf_deadlock_detection_log_save();

static inline time_t GetCurrentTime(void)
{
#if defined(CLOCK_MONOTONIC)
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return now.tv_sec;
#else
    return time(NULL); 
#endif
}

/* These two macro are used to transfer information for deadlock detection thread */
#define CCSP_DEADLOCK_DETECTION_ENTER(messageType1, parameterArray, size1)     \
    if ( deadlock_detection_enable )                                     \
    {                                                                    \
        if ( !deadlock_detection_info.messageType )                      \
        {                                                                \
            pthread_mutex_lock(&(deadlock_detection_info.info_mutex));   \
            deadlock_detection_info.messageType   = messageType1;         \
            deadlock_detection_info.parameterInfo = (void *)parameterArray; \
            deadlock_detection_info.size          = size1;                \
            deadlock_detection_info.enterTime     = GetCurrentTime();           \
            if ( strstr("getParameterValues", messageType1 ) )            \
            {    deadlock_detection_info.detectionDuration = CcspBaseIf_deadlock_detection_time_getval_seconds; } \
            else                                                         \
            {    deadlock_detection_info.detectionDuration = CcspBaseIf_deadlock_detection_time_normal_seconds; } \
            pthread_mutex_unlock(&(deadlock_detection_info.info_mutex)); \
            CcspBaseIf_deadlock_detection_log_save();                   \
        }else                                                            \
        {                                                                \
            CcspTraceError(("CCSP_DEADLOCK_DETECTION_EXIT -- error call. last detection doesn't exit(%s)\n", deadlock_detection_info.messageType)); \
            syslog(LOG_ERR, "CCSP_DEADLOCK_DETECTION_EXIT -- error call. last detection doesn't exit(%s)\n", deadlock_detection_info.messageType); \
        }                                                                \
    }                                                                    \

#define CCSP_DEADLOCK_DETECTION_EXIT(messageType1)                        \
    if ( deadlock_detection_enable )                                     \
    {                                                                    \
        if ( strstr(deadlock_detection_info.messageType, messageType1 ))  \
        {                                                                 \
            pthread_mutex_lock(&(deadlock_detection_info.info_mutex));    \
            deadlock_detection_info.messageType   = NULL;               \
            deadlock_detection_info.parameterInfo = NULL;               \
            deadlock_detection_info.size          = 0;                  \
            deadlock_detection_info.enterTime     = 0;                  \
            deadlock_detection_info.detectionDuration = 0;              \
            pthread_mutex_unlock(&(deadlock_detection_info.info_mutex));  \
        }else                                                            \
        {                                                                \
            CcspTraceError(("CCSP_DEADLOCK_DETECTION_EXIT -- error call. it's not current message type(%s)\n", deadlock_detection_info.messageType)); \
            syslog(LOG_ERR, "CCSP_DEADLOCK_DETECTION_EXIT -- error call. it's not current message type(%s)\n", deadlock_detection_info.messageType); \
        }                                                                \
    }                                                                    \
    
/* These two macroes are used to hide complex log system of deadlock detection */
#define  CCSP_DEADLOCK_PRINT_INTERNAL(args...)       \
    do {                                            \
        snprintf(oneline, 1023, args);            \
    }while(0);                                  \

#define CCSP_DEADLOCK_PRINT(aaa)        \
    {                                 \
        char oneline[1024] = {0};     \
        CCSP_DEADLOCK_PRINT_INTERNAL aaa; \
        CcspTraceError(("%s", oneline));  \
        syslog(LOG_ERR, oneline); \
        if ( fd ) fputs( oneline, fd ); \
    }   \


#define CCSP_DEADLOCK_INFO_PRINT(args...)     \
{                                             \
    snprintf( (*deadlock_detection_log)[index], deadlock_detection_log_linelen-1, args);  \
    if ( ++index >= deadlock_detection_log_linenum ) \
        index = 0;                                   \
}                                                    \


static const char* Base_Introspect_msg =
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<node name=\"/com/cisco/spvtg/ccsp/CCSPComponent\">\n"
    "    <interface name=\"com.cisco.spvtg.ccsp.baseInterface\">\n"
    "        \n"
    "        <method name=\"initialize\">\n"
    "            <arg type=\"i\" name=\"status\" direction=\"out\" />\n"
    "        </method>\n"
    "        <method name=\"finalize\">\n"
    "            <arg type=\"i\" name=\"status\" direction=\"out\" />\n"
    "        </method>\n"
    "        \n"
    "        <!--\n"
    "            This API bus_info->freefunc frees up resources such as allocated memory, flush caches etc, if possible. \n"
    "            This is invoked by Test and Diagnostic Manager, as a proactive measure, when it \n"
    "            detects low memory conditions.     \n"
    "        -->\n"
    "        <method name=\"bus_info->freefunc frees Resources\">\n"
    "            <arg type=\"i\" name=\"priority\" direction=\"in\" />\n"
    "            <arg type=\"i\" name=\"status\" direction=\"out\" />\n"
    "        </method>\n"
    "        \n"
    "        <!--\n"
    "            DEPRECATED\n"
    "            This API is used to retrieve the Component Metadata. The Component Metadata \n"
    "            includes the following information: \n"
    "            - Component Name\n"
    "            - Component Author \n"
    "            - Component Version \n"
    "        -->\n"
    "        <method name=\"getComponentMetadata\">\n"
    "            <arg type=\"s\" name=\"component_name\" direction=\"out\" />\n"
    "            <arg type=\"s\" name=\"component_author\" direction=\"out\" />\n"
    "            <arg type=\"s\" name=\"component_version\" direction=\"out\" />\n"
    "            <arg type=\"i\" name=\"status\" direction=\"out\" />\n"
    "        </method>\n"
    "        \n"
    "        <!-- \n"
    "            DEPRECATED \n"
    "            Logging APIs  \n"
    "        -->\n"
    "        <method name=\"enableLogging\">\n"
    "            <arg type=\"b\" name=\"enable\" direction=\"in\" />\n"
    "            <arg type=\"i\" name=\"status\" direction=\"out\" />\n"
    "        </method>\n"
    "        <!-- DEPRECATED   -->\n"
    "        <method name=\"setLoggingLevel\">\n"
    "            <arg type=\"i\" name=\"level\" direction=\"in\" />\n"
    "            <arg type=\"i\" name=\"status\" direction=\"out\" />\n"
    "        </method>\n"
    "\n"
    "        <!-- \n"
    "            DEPRECATED\n"
    "            This API returns the internal state of the component. The state reflects the \n"
    "            Component\'s internal lifecycle state\n"
    "        -->\n"
    "        <method name=\"queryStatus\">\n"
    "            <arg type=\"i\" name=\"internalState\" direction=\"out\" />\n"
    "            <arg type=\"i\" name=\"status\" direction=\"out\" />\n"
    "        </method> \n"
    "        \n"
    "        <!-- \n"
    "            DEPRECATED \n"
    "            This API returns the health of the component as \'Red/Bad\', \'Yellow/warning\', \'Green/good\' \n"
    "        -->\n"
    "        <method name=\"healthCheck\">\n"
    "            <arg type=\"i\" name=\"health\" direction=\"out\" />\n"
    "            <arg type=\"i\" name=\"status\" direction=\"out\" />\n"
    "        </method>  \n"
    "        \n"
    "        <!--\n"
    "            DEPRECATED\n"
    "            This API returns the amount of direct memory allocated by the component. Typically the \n"
    "            Process statistics can be retrieved by querying the /proc/<PID> file system under    \n"
    "            Linux OS. However, in cases where more than one component are grouped into a \n"
    "            single process, this API provides component level memory usage which can be very \n"
    "            useful to isolate low memory conditions. \n"
    "        -->\n"
    "        <method name=\"getAllocatedMemory\">\n"
    "            <arg type=\"i\" name=\"directAllocatedMemory\" direction=\"out\" />\n"
    "            <arg type=\"i\" name=\"status\" direction=\"out\" />\n"
    "        </method>  \n"
    "        \n"
    "        <!-- \n"
    "            DEPRECATED\n"
    "            This API returns the mmaximum memory requirements for the component. It is the \n"
    "            component owner\'s best estimates   \n"
    "        -->\n"
    "        <method name=\"getMaxMemoryUsage\">\n"
    "            <arg type=\"i\" name=\"memoryUsage\" direction=\"out\" />\n"
    "            <arg type=\"i\" name=\"status\" direction=\"out\" />\n"
    "        </method>  \n"
    " \n"
    "        <!-- \n"
    "            DEPRECATED\n"
    "            This API returns the minimum memory requirements for the component. It is the \n"
    "            component owner\'s best estimates   \n"
    "        -->\n"
    "        <method name=\"getMinMemoryUsage\">\n"
    "            <arg type=\"i\" name=\"memoryUsage\" direction=\"out\" />\n"
    "            <arg type=\"i\" name=\"status\" direction=\"out\" />\n"
    "        </method>  \n"
    " \n"
    "        <!-- Data model parameters \"set\" APIs\n"
    "            typedef struct {\n"
    "                const char *parameterName; \n"
    "                unsigned char *parameterValue;\n"
    "                dataType_e type; \n"
    "            } parameterValStruct_t; \n"
    "            \n"
    "            typedef enum {\n"
    "                ccsp_string = 0, \n"
    "                ccsp_int,\n"
    "                ccsp_unsignedInt,\n"
    "                ccsp_boolean,\n"
    "                ccsp_dateTime,\n"
    "                ccsp_base64,\n"
    "                ccsp_long, \n"
    "                ccsp_unsignedLong, \n"
    "                ccsp_float, \n"
    "                ccsp_double,\n"
    "                ccsp_byte,  // char \n"
    "                (any other simple type that I may have missed),   \n"
    "                ccsp_none \n"
    "            } datatype_e\n"
    "        -->\n"
    "        <method name=\"setParameterValues\">\n"
    "            <arg type=\"i\" name=\"sessionId\" direction=\"in\" />\n"
    "            <arg type=\"i\" name=\"writeID\" direction=\"in\" />\n"
    "            <arg type=\"a(ssi)\" name=\"parameterValStruct\" direction=\"in\" />\n"
    "            <arg type=\"i\" name=\"size\" direction=\"in\" />\n"
    "            <arg type=\"b\" name=\"commit\" direction=\"in\" />\n"
    "            <arg type=\"s\" name=\"invalidParameterName\" direction=\"out\" />"
    "            <arg type=\"i\" name=\"status\" direction=\"out\" />\n"
    "        </method>\n"
    "        \n"
    "        <method name=\"setCommit\">\n"
    "            <arg type=\"i\" name=\"sessionId\" direction=\"in\" />\n"
    "            <arg type=\"i\" name=\"writeID\" direction=\"in\" />\n"
    "            <arg type=\"b\" name=\"commit\" direction=\"in\" />\n"
    "            <arg type=\"i\" name=\"status\" direction=\"out\" />\n"
    "        </method>\n"
    "                \n"
    "        <!-- Data model parameters \"get\" APIs   -->\n"
    "        <method name=\"getParameterValues\">\n"
    "            <arg type=\"as\" name=\"parameterNames\" direction=\"in\" />\n"
    "            <arg type=\"i\" name=\"size\" direction=\"in\" />\n"
    "            <arg type=\"a(ssi)\" name=\"parameterValStruct\" direction=\"out\" />\n"
    "            <arg type=\"i\" name=\"status\" direction=\"out\" />\n"
    "        </method>\n"
    "        \n"
    "        <!-- \n"
    "            This API sets the attributes on data model parameters\n"
    "            typedef struct { \n"
    "                const char* parameterName; \n"
    "                boolean notificationChanged; \n"
    "                boolean notification; \n"
    "                enum access_e access; // (CCSP_RO, CCSP_RW, CCSP_WO)\n"
    "                boolean accessControlChanged; \n"
    "                unsigned int accessControlBitmask;\n"
    "                unsigned int RequesterID;\n"
    "                  //  0x00000000 ACS\n"
    "                  //  0x00000001 XMPP\n"
    "                  //  0x00000002 CLI\n"
    "                  //  0x00000004 WebUI\n"
    "                  //  ... \n"
    "                  //  0xFFFFFFFF  ANYBODY (reserved and default value for all parameters)\n"
    "           } parameterAttribStruct_t; \n"
    "        -->\n"
    "        <method name=\"setParameterAttributes\">\n"
    "            <arg type=\"i\" name=\"sessionId\" direction=\"in\" />\n"
    "            <arg type=\"a(sbbibuu)\" name=\"parameterAttributeStruct\" direction=\"in\" />\n"
    "            <arg type=\"i\" name=\"size\" direction=\"in\" />\n"
    "            <arg type=\"i\" name=\"status\" direction=\"out\" />\n"
    "        </method>\n"
    "        \n"
    "        <method name=\"getParameterAttributes\">\n"
    "            <arg type=\"as\" name=\"parameterNames\" direction=\"in\" />\n"
    "            <arg type=\"i\" name=\"size\" direction=\"in\" />\n"
    "            <arg type=\"a(sbbibu)\" name=\"parameterAttributeStruct\" direction=\"out\" />\n"
    "            <arg type=\"i\" name=\"status\" direction=\"out\" />\n"
    "        </method>\n"
    "        \n"
    "        <!-- \n"
    "            This API adds a row to a table object. The object name is a partial path \n"
    "            and must end with a \".\" (dot). The API returns the instance number of the \n"
    "            row.\n"
    "        -->\n"
    "        <method name=\"AddTblRow\">\n"
    "            <arg type=\"i\" name=\"sessionId\" direction=\"in\" />\n"
    "            <arg type=\"s\" name=\"objectName\" direction=\"in\" />\n"
    "            <arg type=\"u\" name=\"instanceNumber\" direction=\"out\" />\n"
    "            <arg type=\"i\" name=\"status\" direction=\"out\" />\n"
    "        </method>\n"
    "        \n"
    "        <!-- \n"
    "            This API deletes a row from the table object. The object name is a partial \n"
    "            path and must end with a \".\" (dot) after the instance number.\n"
    "        -->\n"
    "        <method name=\"DeleteTblRow\">\n"
    "            <arg type=\"i\" name=\"sessionId\" direction=\"in\" />\n"
    "            <arg type=\"s\" name=\"objectName\" direction=\"in\" />\n"
    "            <arg type=\"i\" name=\"status\" direction=\"out\" />\n"
    "        </method>\n"
    "        \n"
    "        <!--\n"
    "            This API is used to return the supported parameter names under a data model object\n"
    "            parameterName is either a complete Parameter name, or a partial path name of an object.            \n"
    "            nextLevel \n"
    "                If false, the response MUST contain the Parameter or object whose name exactly\n"
    "                matches the ParameterPath argument, plus all Parameters and objects that are\n"
    "                descendents of the object given by the ParameterPath argument, if any (all levels\n"
    "                below the specified object in the object hierarchy).\n"
    "            \n"
    "                If true, the response MUST contain all Parameters and objects that are next-level\n"
    "                children of the object given by the ParameterPath argument, if any.\n"
    "            parameterInfoStruct is defined as: \n"
    "                typedef struct {\n"
    "                    comst char *name; \n"
    "                    boolean writable; \n"
    "                }\n"
    "        -->\n"
    "        <method name=\"getParameterNames\">\n"
    "            <arg type=\"s\" name=\"parameterName\" direction=\"in\" />\n"
    "            <arg type=\"b\" name=\"nextLevel\" direction=\"in\" />\n"
    "            <arg type=\"a(sb)\" name=\"parameterInfoStruct\" direction=\"out\" />\n"
    "            <arg type=\"i\" name=\"status\" direction=\"out\" />\n"
    "        </method>\n"
    "        \n"
    "        <!-- \n"
    "            This API is used in diagnostic mode. This must be used asynchronously. \n"
    "            The use case is that the Test and Diagnostic Manager (TDM) CCSP component can leverage this feature \n"
    "            in the Component Registrar to validate parameter types. The TDM sends commands to other components to \n"
    "            run diagnostics. The TDM invokes a buscheck() request to each component one at a time in diagnostic mode. \n"
    "            When each component receives buscheck(), it invokes the namespace type check API in the Component \n"
    "            Registrar for each of the data model parameters accessed by this component and owned by another component. \n"
    "            The Component Registrar verifies that each data model parameter is registered by a component and that the \n"
    "            data model type specified in the API is the same as the data model type registered by the \'owner\' component. \n"
    "            The component sends TDM a response to buscheck() with all checked parameter names and PASS/FAIL for each \n"
    "            parameter. If during buscheck(), it is found that there are missing or unregistered parameters, \n"
    "            appropriate errors are flagged. \n"
    "        -->\n"
    "        <method name=\"busCheck\">\n"
    "            <arg type=\"i\" name=\"status\" direction=\"out\" />\n"
    "        </method>\n"
    "        \n"
    "        <!--\n"
    "            Signal contains the following information: \n"
    "            typedef struct {\n"
    "                const char *parameterName; \n"
    "                const char* oldValue; \n"
    "                const char* int newValue;\n"
    "                unsigned int writeID; \n"
    "            } parameterSigStruct_t; \n"
    "        -->\n"
    "        <signal name=\"parameterValueChangeSignal\">\n"
    "            <arg type=\"a(sssisu)\" name=\"parameterSigStruct\" direction=\"out\" />\n"
    "            <arg type=\"i\" name=\"size\" direction=\"out\" />\n"
    "        </signal>\n"
    "        \n"
    "       \n"
    "    </interface>\n"
    "</node>\n"
    ;



void CcspBaseIf_SetCallback
(
    void* bus_handle,
    CCSP_Base_Func_CB*  func
)
{

    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    if(!bus_info->CcspBaseIf_func)
    	bus_info->CcspBaseIf_func = bus_info->mallocfunc(sizeof(CCSP_Base_Func_CB));
    *((CCSP_Base_Func_CB*)(bus_info->CcspBaseIf_func)) = *func;
}

void CcspBaseIf_SetCallback2
(
    void* bus_handle,
    char *name,
    void*  func,
    void * user_data
)
{
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    if(!bus_info->CcspBaseIf_func)
    {
	    bus_info->CcspBaseIf_func = bus_info->mallocfunc(sizeof(CCSP_Base_Func_CB));
	    memset(bus_info->CcspBaseIf_func, 0, sizeof(CCSP_Base_Func_CB));
    }   
    CCSP_Base_Func_CB* cb = (CCSP_Base_Func_CB*)bus_info->CcspBaseIf_func;
    if(!strcmp("setParameterValues", name))
    {
	    cb->setParameterValues = func;
	    cb->setParameterValues_data = user_data;
    }   
       
    else if(!strcmp("setCommit", name))
    {
	    cb->setCommit = func;
	    cb->setCommit_data = user_data;
    }   
       
    else if(!strcmp("getParameterValues", name))
    {
        cb->getParameterValues = func;
	    cb->getParameterValues_data = user_data;
    }   
       
    else if(!strcmp("setParameterAttributes", name))
    {
       cb->setParameterAttributes = func;
       cb->setParameterAttributes_data = user_data;
    }   
       
    else if(!strcmp("getParameterAttributes", name))
    {
       cb->getParameterAttributes = func;
	   cb->getParameterAttributes_data = user_data;
    }   
       
    else if(!strcmp("AddTblRow", name))
    {
       cb->AddTblRow = func;
	   cb->AddTblRow_data = user_data;
    }   
       
    else if(!strcmp("DeleteTblRow", name))
    {
       cb->DeleteTblRow = func;
	   cb->DeleteTblRow_data = user_data;
    }   
       
    else if(!strcmp("getParameterNames", name))
    {
       cb->getParameterNames = func;
	   cb->getParameterNames_data = user_data;
    }   
       
    else if(!strcmp("freeResources", name))
    {
       cb->freeResources = func;
	   cb->freeResources_data = user_data;
    }   
       
    else if(!strcmp("busCheck", name))
    {
       cb->busCheck = func;
	   cb->busCheck_data = user_data;
    }   
       
    else if(!strcmp("initialize", name))
    {
       cb->initialize = func;
	   cb->initialize_data = user_data;
    }   
       
    else if(!strcmp("finalize", name))
    {
       cb->finalize = func;
	   cb->finalize_data = user_data;
    }   
       
    else if(!strcmp("componentStartDie", name))
    {
        cb->componentDie = func;
	    cb->componentDie_data = user_data;
    }   
       
    else if(!strcmp("parameterValueChangeSignal", name))
    {
       cb->parameterValueChangeSignal = func;
	   cb->parameterValueChangeSignal_data = user_data;
    }   
       
    else if(!strcmp("deviceProfileChangeSignal", name))
    {
        cb->deviceProfileChangeSignal = func;
	    cb->deviceProfileChangeSignal_data = user_data;
    }   
       
    else if(!strcmp("currentSessionIDSignal", name))
    {
        cb->currentSessionIDSignal = func;
	    cb->currentSessionIDSignal_data = user_data;
    }   
      
    else if(!strncmp("telemetryDataSignal", name, MAX_EVENT_NAME_LEN))
    {
        cb->telemetryDataSignal = func;
        cb->telemetryDataSignal_data = user_data;
    }
 
    else if(!strcmp("diagCompleteSignal", name))
    {
        cb->diagCompleteSignal = func;
	    cb->diagCompleteSignal_data = user_data;
    }   
       
    else if(!strcmp("systemReadySignal", name))
    {
        cb->systemReadySignal = func;
	    cb->systemReadySignal_data = user_data;
    }

    else if(!strcmp("systemRebootSignal", name))
    {
        cb->systemRebootSignal = func;
	    cb->systemRebootSignal_data = user_data;
    }
    else if(!strcmp(STBSERVICE_CDL_DLC_SIGNAL, name))
    {
        cb->dlCompleteSignal = func;
        cb->dlCompleteSignal_data = user_data;
    }
    else if(!strcmp("webconfigSignal", name))
    {
        cb->webconfigSignal = func;
        cb->webconfigSignal_data = user_data;
    }
        else if(!strcmp("multiCompBroadCastSignal", name))
        {
            cb->multiCompBroadCastSignal = func;
            cb->multiCompBroadCastSignal_data = user_data;
        }

        else if(!strcmp("multiCompMasterProcessSignal", name))
        {
            cb->multiCompMasterProcessSignal = func;
            cb->multiCompMasterProcessSignal_data = user_data;
        }
        
        else if(!strcmp("multiCompSlaveProcessSignal", name))
        {
            cb->multiCompSlaveProcessSignal = func;
            cb->multiCompSlaveProcessSignal_data = user_data;
        }
        else if(!strcmp("TunnelStatus", name))
        {
            cb->TunnelStatus = func;
            cb->TunnelStatus_data = user_data;
        }
        else if(!strcmp("WifiDbStatus", name))
        {
            cb->WifiDbStatus = func;
            cb->WifiDbStatus_data = user_data;
        }

}

#define CCSP_DBUS_LARGE_REPLY_SIZE_MIN 75000  // bytes
#define MAX_DBUS_PARAM_SIZE 50000

DBusHandlerResult
CcspBaseIf_base_path_message_func (DBusConnection  *conn,
                                   DBusMessage     *message,
                                   DBusMessage     *reply,
                                   const char *interface,
                                   const char *method,
                                   CCSP_MESSAGE_BUS_INFO *bus_info)
{
    //    CcspTraceDebug(("<%s.%d> input: interface='%s', method='%s'\n", __FUNCTION__, getpid(), interface, method));
    if(rbus_enabled == 1)
    {
        CcspTraceError(("came inside %s in rbus it should be handled in thread_path_message_func_rbus\n",__FUNCTION__));
        return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
    }
    /*CID: 56798 Dereference before null check*/
    if( bus_info == NULL)
    {
        CcspTraceError((" %s bus_info is NULL \n",__FUNCTION__));
        return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
    }
    CCSP_Base_Func_CB* func = (CCSP_Base_Func_CB* )bus_info->CcspBaseIf_func;
    if(!strcmp("org.freedesktop.DBus.Introspectable", interface)  && !strcmp(method, "Introspect"))
    {

        if ( !dbus_message_append_args (reply, DBUS_TYPE_STRING, &Base_Introspect_msg, DBUS_TYPE_INVALID))
            CcspTraceError(("No memory\n"));

        if (!dbus_connection_send (conn, reply, NULL))
            CcspTraceError(("No memory\n"));

        dbus_message_unref (reply);
        return DBUS_HANDLER_RESULT_HANDLED;

    }
    else if (!strcmp(CCSP_DBUS_INTERFACE_BASE, interface) && !strcmp("busCheck", method) && func->busCheck)
    {
        DBusMessageIter iter;
        dbus_int32_t tmp ;
        dbus_int32_t result ;

        result = func->busCheck(func->busCheck_data);

        dbus_message_iter_init_append (reply, &iter);

        tmp = result;
        dbus_message_iter_append_basic (&iter, DBUS_TYPE_INT32, &tmp);
        if (!dbus_connection_send (conn, reply, NULL))
            CcspTraceError(("No memory\n"));

        dbus_message_unref (reply);
        return DBUS_HANDLER_RESULT_HANDLED;
    }

    else if (!strcmp(CCSP_DBUS_INTERFACE_BASE, interface) && !strcmp("initialize", method) && func->initialize)
    {
        DBusMessageIter iter;
        dbus_int32_t tmp ;
        dbus_int32_t result ;

        result = func->initialize(func->initialize_data);

        dbus_message_iter_init_append (reply, &iter);

        tmp = result;
        dbus_message_iter_append_basic (&iter, DBUS_TYPE_INT32, &tmp);
        if (!dbus_connection_send (conn, reply, NULL))
            CcspTraceError(("No memory\n"));

        dbus_message_unref (reply);
        return DBUS_HANDLER_RESULT_HANDLED;
    }
    else if (!strcmp(CCSP_DBUS_INTERFACE_BASE, interface) && !strcmp("finalize", method) && func->finalize)
    {
        DBusMessageIter iter;
        dbus_int32_t tmp ;
        dbus_int32_t result ;

        result = func->finalize(func->finalize_data);

        dbus_message_iter_init_append (reply, &iter);

        tmp = result;
        dbus_message_iter_append_basic (&iter, DBUS_TYPE_INT32, &tmp);
        if (!dbus_connection_send (conn, reply, NULL))
            CcspTraceError(("No memory\n"));

        dbus_message_unref (reply);
        return DBUS_HANDLER_RESULT_HANDLED;
    }
    //    fprintf(stderr, "dbus_message_is_method_call %d\n", UserGetTickInMilliSeconds2());
    else if (!strcmp(CCSP_DBUS_INTERFACE_BASE, interface) && !strcmp("setParameterValues", method) && func->setParameterValues)
    {
        DBusMessageIter iter = { 0 };
        DBusMessageIter array_iter = { 0 };
        DBusMessageIter struct_iter= { 0 };
        dbus_int32_t tmp = 0, count = 0;
        dbus_int32_t result = 0;
        dbus_int32_t sessionId = 0;
        dbus_uint32_t writeID = 0;
        int param_size = 0;
        parameterValStruct_t * parameterVal = 0;
        dbus_bool commit = 0; /*RDKB-6233, CID-32915, init before use*/
        int i = 0;
        char *invalidParameterName = 0;
        char ** names = NULL;
        /* get param array count first */
        dbus_message_get_args (message,
                                  NULL,
                                  DBUS_TYPE_ARRAY, DBUS_TYPE_STRING, &names, &i,
                                  DBUS_TYPE_INT32, &count,
                                  DBUS_TYPE_INVALID);
        dbus_message_iter_init (message, &iter);
        if(dbus_message_iter_get_arg_type (&iter) == DBUS_TYPE_INT32)
        {
            dbus_message_iter_get_basic (&iter, &sessionId);
        }
        dbus_message_iter_next (&iter);

        if(dbus_message_iter_get_arg_type (&iter) == DBUS_TYPE_UINT32)
        {
            dbus_message_iter_get_basic (&iter, &writeID);
        }
        dbus_message_iter_next (&iter);

        DBUS_MESSAGE_ITER_RECURSE_SRV(&iter, &array_iter , DBUS_TYPE_ARRAY, 0, bus_info->freefunc);
        param_size = 0;
        do
        {
            int type = dbus_message_iter_get_arg_type (&array_iter);

            if (type == DBUS_TYPE_INVALID)
                break;
            param_size = param_size + 1;
	    if (count > 0 && param_size >= count)
                break;
        }
        while (dbus_message_iter_next (&array_iter));

        if(param_size)
        {
            parameterVal = bus_info->mallocfunc(param_size*sizeof(parameterValStruct_t ));
            memset(parameterVal, 0, param_size*sizeof(parameterValStruct_t ));
            DBUS_MESSAGE_ITER_RECURSE_SRV(&iter, &array_iter , DBUS_TYPE_ARRAY, parameterVal, bus_info->freefunc);
        }

        for(i = 0; i < param_size; i++)
        {

            DBUS_MESSAGE_ITER_RECURSE_SRV(&array_iter, &struct_iter , DBUS_TYPE_STRUCT, parameterVal, bus_info->freefunc);
            if(dbus_message_iter_get_arg_type (&struct_iter) == DBUS_TYPE_STRING)
            {
                parameterVal[i].parameterName = NULL;
                dbus_message_iter_get_basic (&struct_iter, &parameterVal[i].parameterName);
            }
            dbus_message_iter_next (&struct_iter);
            if(dbus_message_iter_get_arg_type (&struct_iter) == DBUS_TYPE_STRING)
            {
                parameterVal[i].parameterValue = NULL;
                dbus_message_iter_get_basic (&struct_iter, &parameterVal[i].parameterValue);
            }
            dbus_message_iter_next (&struct_iter);
            if(dbus_message_iter_get_arg_type (&struct_iter) == DBUS_TYPE_INT32)
            {
                dbus_message_iter_get_basic (&struct_iter, &parameterVal[i].type);
            }

            dbus_message_iter_next (&array_iter);
        }
        dbus_message_iter_next (&iter); //pass size
        dbus_message_iter_next (&iter);
        if(dbus_message_iter_get_arg_type (&iter) == DBUS_TYPE_BOOLEAN)
        {
            dbus_message_iter_get_basic (&iter, &commit);
        }

        CCSP_DEADLOCK_DETECTION_ENTER("setParameterValues", parameterVal, param_size)
            
        result = func->setParameterValues(sessionId, writeID, parameterVal, param_size, commit,&invalidParameterName, func->setParameterValues_data);
        
        CCSP_DEADLOCK_DETECTION_EXIT("setParameterValues")
        
        dbus_message_iter_init_append (reply, &iter);

        DBUS_MESSAGE_APPEND_STRING (&iter, invalidParameterName);
        tmp = result;
        dbus_message_iter_append_basic (&iter, DBUS_TYPE_INT32, &tmp);
        if (!dbus_connection_send (conn, reply, NULL))
            CcspTraceError(("No memory\n"));

        bus_info->freefunc(parameterVal);
        bus_info->freefunc(invalidParameterName);
        dbus_message_unref (reply);
        return DBUS_HANDLER_RESULT_HANDLED;
    }

    else if (!strcmp(CCSP_DBUS_INTERFACE_BASE, interface) && !strcmp("setCommit", method) && func->setCommit)
    {
        DBusMessageIter iter = {0};
        dbus_int32_t tmp = 0;
        dbus_int32_t result = 0;
        dbus_int32_t sessionId = 0; /*RDKB-6233, CID-32928, init before use*/
        dbus_uint32_t writeID = 0; /*RDKB-6233, CID-32967, init before use*/
        dbus_bool commit = 0;

        dbus_message_iter_init (message, &iter);
        if(dbus_message_iter_get_arg_type (&iter) == DBUS_TYPE_INT32)
        {
            dbus_message_iter_get_basic (&iter, &sessionId);
        }
        dbus_message_iter_next (&iter);

        if(dbus_message_iter_get_arg_type (&iter) == DBUS_TYPE_UINT32)
        {
            dbus_message_iter_get_basic (&iter, &writeID);
        }

        dbus_message_iter_next (&iter);
        if(dbus_message_iter_get_arg_type (&iter) == DBUS_TYPE_BOOLEAN)
        {
            dbus_message_iter_get_basic (&iter, &commit);
        }

        CCSP_DEADLOCK_DETECTION_ENTER( "setCommit", NULL, 0 )

        result = func->setCommit(sessionId, writeID, commit, func->setCommit_data);

        CCSP_DEADLOCK_DETECTION_EXIT( "setCommit" )

        dbus_message_iter_init_append (reply, &iter);

        tmp = result;
        dbus_message_iter_append_basic (&iter, DBUS_TYPE_INT32, &tmp);
        if (!dbus_connection_send (conn, reply, NULL))
            CcspTraceError(("No memory\n"));

        dbus_message_unref (reply);
        return DBUS_HANDLER_RESULT_HANDLED;
    }

    else if (!strcmp(CCSP_DBUS_INTERFACE_BASE, interface) && !strcmp("getParameterValues", method) && func->getParameterValues)
    {
        DBusMessageIter iter;
        DBusMessageIter array_iter;
        DBusMessageIter struct_iter;
        dbus_int32_t tmp,count = 0;
        dbus_int32_t result ;
        int param_size;
        char ** parameterNames = 0;
        int i;
        int size = 0;
        parameterValStruct_t **val = 0;
        dbus_uint32_t writeID = 0;
        char bIsWifi = 0;

        dbus_message_iter_init (message, &iter);
        if(dbus_message_iter_get_arg_type (&iter) == DBUS_TYPE_UINT32)
        {
            dbus_message_iter_get_basic (&iter, &writeID);
        }
        dbus_message_iter_next (&iter);

        if ( ( strcmp(bus_info->component_id, "eRT.com.cisco.spvtg.ccsp.wifi") == 0) || 
             writeID == DSLH_MPA_ACCESS_CONTROL_WIFI )
        {
            bIsWifi = 1;
        }
        
        DBUS_MESSAGE_ITER_RECURSE_SRV(&iter, &array_iter , DBUS_TYPE_ARRAY, 0, bus_info->freefunc);
        param_size = 0;
        do
        {
            int type = dbus_message_iter_get_arg_type (&array_iter);

            if (type == DBUS_TYPE_INVALID)
                break;
            param_size = param_size + 1;
            if (count > 0 && param_size >= count)
                break;
        }
        while (dbus_message_iter_next (&array_iter));

        if(param_size)
        {
            parameterNames = bus_info->mallocfunc(param_size*sizeof(char *));
            memset(parameterNames, 0, param_size*sizeof(char *));
            DBUS_MESSAGE_ITER_RECURSE_SRV(&iter, &array_iter , DBUS_TYPE_ARRAY, 0, bus_info->freefunc);
        }
        for(i = 0; i < param_size; i++)
        {
            if(dbus_message_iter_get_arg_type (&array_iter) == DBUS_TYPE_STRING)
            {
                parameterNames[i] = NULL;
                dbus_message_iter_get_basic (&array_iter, &parameterNames[i]);
            }
            dbus_message_iter_next (&array_iter);
        }

        size = 0;

        CCSP_DEADLOCK_DETECTION_ENTER("getParameterValues", parameterNames, param_size)

        result = func->getParameterValues(writeID, parameterNames,param_size, &size, &val , func->getParameterValues_data);

        CCSP_DEADLOCK_DETECTION_EXIT("getParameterValues")

        bus_info->freefunc(parameterNames);

        dbus_message_iter_init_append (reply, &iter);

        dbus_message_iter_open_container (&iter,
                                          DBUS_TYPE_ARRAY,
                                          "(ssi)",
                                          &array_iter);

        if(result == CCSP_SUCCESS)
        {

            int i;
			
            if ((size < MAX_DBUS_PARAM_SIZE) || ( bIsWifi ))
            {
            for(i = 0; i < size; i++)
            {
                dbus_message_iter_open_container (&array_iter,
                                                  DBUS_TYPE_STRUCT,
                                                  "ssi",
                                                  &struct_iter);

                DBUS_MESSAGE_APPEND_STRING (&struct_iter, val[i]->parameterName);

                DBUS_MESSAGE_APPEND_STRING (&struct_iter, val[i]->parameterValue);

                tmp = val[i]->type;
                dbus_message_iter_append_basic (&struct_iter, DBUS_TYPE_INT32, &tmp);

                    dbus_message_iter_close_container (&array_iter, &struct_iter);
                }
            }
            else
            {
                char * buf = SHM_PARAM_NAME;
                int shmSize = 0;

                CcspBaseIf_base_path_message_write_shm(bus_info, size, val, &shmSize); 
                CcspTraceInfo(("<%s.%d> dz write shm size %d\n", __FUNCTION__, getpid(), shmSize));
                /*go shared memory way
                 the format: SHM_PARAM_NAME:SHM_PARAM_NAME:shared-memory-size*/
                dbus_message_iter_open_container (&array_iter,
                                                  DBUS_TYPE_STRUCT,
                                                  "ssi",
                                                  &struct_iter);

                DBUS_MESSAGE_APPEND_STRING (&struct_iter, buf);

                DBUS_MESSAGE_APPEND_STRING (&struct_iter, buf);
                tmp = shmSize;
                dbus_message_iter_append_basic (&struct_iter, DBUS_TYPE_INT32, &tmp); 

                dbus_message_iter_close_container (&array_iter, &struct_iter);
            }
        }

        dbus_message_iter_close_container (&iter, &array_iter);


        tmp = result;
        dbus_message_iter_append_basic (&iter, DBUS_TYPE_INT32, &tmp);
        if (!dbus_connection_send (conn, reply, (dbus_uint32_t *)&tmp))
            CcspTraceError(("<%s.%d> No memory\n", __FUNCTION__, getpid()));

        if ( dbus_connection_get_outgoing_size(conn) > CCSP_DBUS_LARGE_REPLY_SIZE_MIN && 
             dbus_connection_has_messages_to_send(conn)) 
        { 
            // printf("SERVER Large Msg - %d: Outgoing queue size = %d bytes\n", 
            //        tmp, dbus_connection_get_outgoing_size(conn));
            dbus_connection_flush(conn); 
        }
        
        dbus_message_unref (reply);
        free_parameterValStruct_t(bus_info, size, val);
        return DBUS_HANDLER_RESULT_HANDLED;
    }

    else if (!strcmp(CCSP_DBUS_INTERFACE_BASE, interface) && !strcmp("setParameterAttributes", method) && func->setParameterAttributes)
    {
        DBusMessageIter iter;
        DBusMessageIter array_iter;
        DBusMessageIter struct_iter;
        dbus_int32_t tmp ,count = 0;
        char ** names;
        dbus_uint32_t utmp ;
        dbus_uint32_t utmp1 ;
        dbus_int32_t sessionId ;
        dbus_int32_t result ;
        int param_size;
        parameterAttributeStruct_t * parameterAttribute = 0;
        int i;

        /* get param array count first */
        dbus_message_get_args (message,
                                  NULL,
                                  DBUS_TYPE_ARRAY, DBUS_TYPE_STRING, &names, &i,
                                  DBUS_TYPE_INT32, &count,
                                  DBUS_TYPE_INVALID);
        //printf("setParameterValue - message has %p, count %d.\n", names, count);

        dbus_message_iter_init (message, &iter);

        if(dbus_message_iter_get_arg_type (&iter) == DBUS_TYPE_INT32)
        {
            dbus_message_iter_get_basic (&iter, &sessionId);
        }
        dbus_message_iter_next (&iter);

        DBUS_MESSAGE_ITER_RECURSE_SRV(&iter, &array_iter , DBUS_TYPE_ARRAY, 0, bus_info->freefunc);
        param_size = 0;
        do
        {
            int type = dbus_message_iter_get_arg_type (&array_iter);

            if (type == DBUS_TYPE_INVALID)
                break;
            param_size = param_size + 1;
            if (count > 0 && param_size >= count)
                break;
        }
        while (dbus_message_iter_next (&array_iter));

        if(param_size)
        {
            parameterAttribute = bus_info->mallocfunc(param_size*sizeof(parameterAttributeStruct_t));
            memset(parameterAttribute, 0, param_size*sizeof(parameterAttributeStruct_t));
            DBUS_MESSAGE_ITER_RECURSE_SRV(&iter, &array_iter , DBUS_TYPE_ARRAY, parameterAttribute, bus_info->freefunc);
        }
        for(i = 0; i < param_size; i++)
        {

            DBUS_MESSAGE_ITER_RECURSE_SRV(&array_iter, &struct_iter , DBUS_TYPE_STRUCT, parameterAttribute, bus_info->freefunc);
            if(dbus_message_iter_get_arg_type (&struct_iter) == DBUS_TYPE_STRING)
            {
                parameterAttribute[i].parameterName = NULL;
                dbus_message_iter_get_basic (&struct_iter, &parameterAttribute[i].parameterName);
            }
            dbus_message_iter_next (&struct_iter);
            if(dbus_message_iter_get_arg_type (&struct_iter) == DBUS_TYPE_BOOLEAN)
            {
                dbus_message_iter_get_basic (&struct_iter, &parameterAttribute[i].notificationChanged);
            }
            dbus_message_iter_next (&struct_iter);
            if(dbus_message_iter_get_arg_type (&struct_iter) == DBUS_TYPE_BOOLEAN)
            {
                dbus_message_iter_get_basic (&struct_iter, &parameterAttribute[i].notification);
            }
            dbus_message_iter_next (&struct_iter);
            if(dbus_message_iter_get_arg_type (&struct_iter) == DBUS_TYPE_INT32)
            {
                dbus_message_iter_get_basic (&struct_iter, &tmp);
                parameterAttribute[i].access = tmp;
            }

            dbus_message_iter_next (&struct_iter);
            if(dbus_message_iter_get_arg_type (&struct_iter) == DBUS_TYPE_BOOLEAN)
            {
                dbus_message_iter_get_basic (&struct_iter, &parameterAttribute[i].accessControlChanged);
            }

            dbus_message_iter_next (&struct_iter);
            if(dbus_message_iter_get_arg_type (&struct_iter) == DBUS_TYPE_UINT32)
            {
                dbus_message_iter_get_basic (&struct_iter, &utmp);
                parameterAttribute[i].accessControlBitmask = utmp;
            }
            dbus_message_iter_next (&struct_iter);
            if(dbus_message_iter_get_arg_type (&struct_iter) == DBUS_TYPE_UINT32)
            {
                dbus_message_iter_get_basic (&struct_iter, &utmp1);
                parameterAttribute[i].RequesterID = utmp1;
		if(parameterAttribute[i].RequesterID != 0)
                CcspTraceInfo(("Notification (%s) Param %s RequesterID %u\n", __FUNCTION__,parameterAttribute[i].parameterName,parameterAttribute[i].RequesterID));
            }

            dbus_message_iter_next (&array_iter);
        }

        CCSP_DEADLOCK_DETECTION_ENTER("setParameterAttributes", parameterAttribute, param_size)

        result = func->setParameterAttributes(sessionId, parameterAttribute ,param_size, func->setParameterAttributes_data);

        CCSP_DEADLOCK_DETECTION_EXIT("setParameterAttributes")

        bus_info->freefunc(parameterAttribute);

        dbus_message_iter_init_append (reply, &iter);

        tmp = result;
        dbus_message_iter_append_basic (&iter, DBUS_TYPE_INT32, &tmp);
        if (!dbus_connection_send (conn, reply, NULL))
            CcspTraceError(("No memory\n"));

        dbus_message_unref (reply);
        return DBUS_HANDLER_RESULT_HANDLED;
    }

    else if (!strcmp(CCSP_DBUS_INTERFACE_BASE, interface) && !strcmp("getParameterAttributes", method) && func->getParameterAttributes)
    {
        DBusMessageIter iter;
        DBusMessageIter array_iter;
        DBusMessageIter struct_iter;
        dbus_int32_t tmp ,count = 0;
        dbus_uint32_t utmp = 0 ;
        /*CID: 58299 Uninitialized scalar variable*/
        dbus_bool_t btmp = 0 ;
	char ** names = NULL;
        dbus_int32_t result = 0;
        int param_size = 0;
        char ** parameterNames = 0;
        parameterAttributeStruct_t **val = 0;
        int i = 0;
        int size = 0;

	/* get param array count first */
        dbus_message_get_args (message,
                                  NULL,
                                  DBUS_TYPE_ARRAY, DBUS_TYPE_STRING, &names, &i,
                                  DBUS_TYPE_INT32, &count,
                                  DBUS_TYPE_INVALID);
        dbus_message_iter_init (message, &iter);
        DBUS_MESSAGE_ITER_RECURSE_SRV(&iter, &array_iter , DBUS_TYPE_ARRAY, 0, bus_info->freefunc);
        param_size = 0;
        do
        {
            int type = dbus_message_iter_get_arg_type (&array_iter);

            if (type == DBUS_TYPE_INVALID)
                break;
            param_size = param_size + 1;
	    if (count > 0 && param_size >= count)
                break;
        }
        while (dbus_message_iter_next (&array_iter));

        if(param_size)
        {
            parameterNames = bus_info->mallocfunc(param_size*sizeof(char *));
            memset(parameterNames, 0, param_size*sizeof(char *));
            DBUS_MESSAGE_ITER_RECURSE_SRV(&iter, &array_iter , DBUS_TYPE_ARRAY, parameterNames, bus_info->freefunc);
        }
        for(i = 0; i < param_size; i++)
        {
            if(dbus_message_iter_get_arg_type (&array_iter) == DBUS_TYPE_STRING)
            {
                parameterNames[i] = NULL;
                dbus_message_iter_get_basic (&array_iter, &parameterNames[i]);
            }
            dbus_message_iter_next (&array_iter);
        }

        size = 0;

        CCSP_DEADLOCK_DETECTION_ENTER("getParameterAttributes", parameterNames, param_size)

        result = func->getParameterAttributes(parameterNames,param_size, &size, &val , func->getParameterAttributes_data);

        CCSP_DEADLOCK_DETECTION_EXIT("getParameterAttributes")


        bus_info->freefunc(parameterNames);

        dbus_message_iter_init_append (reply, &iter);

        dbus_message_iter_open_container (&iter,
                                         DBUS_TYPE_ARRAY,
                                         "(sbbibu)",
                                         &array_iter);
        if(result == CCSP_SUCCESS)
        {

            int i;
            for(i = 0; i < size; i++)
            {
                dbus_message_iter_open_container (&array_iter,
                                                  DBUS_TYPE_STRUCT,
                                                  "sbbibu",
                                                  &struct_iter);

                DBUS_MESSAGE_APPEND_STRING (&struct_iter, val[i]->parameterName);

                btmp = val[i]->notificationChanged;
                dbus_message_iter_append_basic (&struct_iter, DBUS_TYPE_BOOLEAN, &btmp);
                btmp = val[i]->notification;
                dbus_message_iter_append_basic (&struct_iter, DBUS_TYPE_BOOLEAN, &btmp);

                tmp = val[i]->access;
                dbus_message_iter_append_basic (&struct_iter, DBUS_TYPE_INT32, &tmp);
                btmp = val[i]->accessControlChanged;
                dbus_message_iter_append_basic (&struct_iter, DBUS_TYPE_BOOLEAN, &btmp);
                utmp = val[i]->accessControlBitmask;
                dbus_message_iter_append_basic (&struct_iter, DBUS_TYPE_UINT32, &utmp);

                dbus_message_iter_close_container (&array_iter,
                                                   &struct_iter);
            }
        }

        dbus_message_iter_close_container (&iter,
                &array_iter);


        tmp = result;
        dbus_message_iter_append_basic (&iter, DBUS_TYPE_INT32, &tmp);
        if (!dbus_connection_send (conn, reply, NULL))
            CcspTraceError(("No memory\n"));

        dbus_message_unref (reply);
        free_parameterAttributeStruct_t(bus_info, size, val);
        return DBUS_HANDLER_RESULT_HANDLED;
    }

    else if (!strcmp(CCSP_DBUS_INTERFACE_BASE, interface) && !strcmp("AddTblRow", method) && func->AddTblRow)
    {
        DBusMessageIter iter;
        int instanceNumber = 0;
        dbus_int32_t result = 0;
        dbus_int32_t tmp = 0;
        dbus_uint32_t utmp = 0 ;
        dbus_int32_t sessionId = 0 ;
        /*CID: 58495 Uninitialized pointer read*/
        char * str = NULL;

        dbus_message_iter_init (message, &iter);
        if(dbus_message_iter_get_arg_type (&iter) == DBUS_TYPE_INT32)
        {
            dbus_message_iter_get_basic (&iter, &sessionId);
        }
        dbus_message_iter_next (&iter);

        if(dbus_message_iter_get_arg_type (&iter) == DBUS_TYPE_STRING)
        {
            dbus_message_iter_get_basic (&iter, &str);
        }

        CCSP_DEADLOCK_DETECTION_ENTER("AddTblRow", str, 0)

        result = func->AddTblRow(sessionId, str, &instanceNumber , func->AddTblRow_data);

        CCSP_DEADLOCK_DETECTION_EXIT("AddTblRow")

        dbus_message_iter_init_append (reply, &iter);

        utmp = instanceNumber;
        dbus_message_iter_append_basic (&iter, DBUS_TYPE_UINT32, &utmp);

        tmp = result;
        dbus_message_iter_append_basic (&iter, DBUS_TYPE_INT32, &tmp);
        if (!dbus_connection_send (conn, reply, NULL))
            CcspTraceError(("No memory\n"));

        dbus_message_unref (reply);
        return DBUS_HANDLER_RESULT_HANDLED;
    }

    else if (!strcmp(CCSP_DBUS_INTERFACE_BASE, interface) && !strcmp("DeleteTblRow", method) && func->DeleteTblRow)
    {
        DBusMessageIter iter;
        dbus_int32_t tmp ;
        dbus_int32_t result ;
        dbus_int32_t sessionId ;
	/* CID 58495 fix */
        char * str = NULL;
        

        dbus_message_iter_init (message, &iter);
        if(dbus_message_iter_get_arg_type (&iter) == DBUS_TYPE_INT32)
        {
            dbus_message_iter_get_basic (&iter, &sessionId);
        }
        dbus_message_iter_next (&iter);

        if(dbus_message_iter_get_arg_type (&iter) == DBUS_TYPE_STRING)
        {
            dbus_message_iter_get_basic (&iter, &str);
        }

        CCSP_DEADLOCK_DETECTION_ENTER("DeleteTblRow", str, 0 )

        result = func->DeleteTblRow(sessionId, str , func->DeleteTblRow_data);

        CCSP_DEADLOCK_DETECTION_EXIT("DeleteTblRow" )

        dbus_message_iter_init_append (reply, &iter);

        tmp = result;
        dbus_message_iter_append_basic (&iter, DBUS_TYPE_INT32, &tmp);
        if (!dbus_connection_send (conn, reply, NULL))
            CcspTraceError(("No memory\n"));

        dbus_message_unref (reply);
        return DBUS_HANDLER_RESULT_HANDLED;
    }

    else if (!strcmp(CCSP_DBUS_INTERFACE_BASE, interface) && !strcmp("getParameterNames", method) && func->getParameterNames)
    {
        DBusMessageIter iter;
        DBusMessageIter array_iter;
        DBusMessageIter struct_iter;
        dbus_int32_t tmp ;
        dbus_bool_t btmp ;
        dbus_int32_t result ;
        int size = 0;
        char * parameterName = 0;
        parameterInfoStruct_t **val = 0;

        dbus_message_iter_init (message, &iter);
        if(dbus_message_iter_get_arg_type (&iter) == DBUS_TYPE_STRING)
        {
            dbus_message_iter_get_basic (&iter, &parameterName);
        }

        dbus_message_iter_next (&iter);
        if(dbus_message_iter_get_arg_type (&iter) == DBUS_TYPE_BOOLEAN)
        {
            dbus_message_iter_get_basic (&iter, &btmp);
        }

        CCSP_DEADLOCK_DETECTION_ENTER("getParameterNames", parameterName, 0 )
        
        result = func->getParameterNames(parameterName,btmp, &size, &val, func->getParameterNames_data );

        CCSP_DEADLOCK_DETECTION_EXIT("getParameterNames" )

        dbus_message_iter_init_append (reply, &iter);

        dbus_message_iter_open_container (&iter,
                                          DBUS_TYPE_ARRAY,
                                          "(sb)",
                                          &array_iter);
        if(result == CCSP_SUCCESS)
        {

            int i;
            for(i = 0; i < size; i++)
            {
                dbus_message_iter_open_container (&array_iter,
                                                  DBUS_TYPE_STRUCT,
                                                  "sb",
                                                  &struct_iter);

                DBUS_MESSAGE_APPEND_STRING (&struct_iter, val[i]->parameterName);
                btmp = val[i]->writable;
                dbus_message_iter_append_basic (&struct_iter, DBUS_TYPE_BOOLEAN, &btmp);
                dbus_message_iter_close_container (&array_iter,
                                                   &struct_iter);
            }
        }

        dbus_message_iter_close_container (&iter,
                &array_iter);


        tmp = result;
        dbus_message_iter_append_basic (&iter, DBUS_TYPE_INT32, &tmp);
        if (!dbus_connection_send (conn, reply, NULL))
            CcspTraceError(("No memory\n"));

        dbus_message_unref (reply);
        free_parameterInfoStruct_t(bus_info, size, val);
        return DBUS_HANDLER_RESULT_HANDLED;
    }
    else if (!strcmp(CCSP_DBUS_INTERFACE_BASE, interface) && !strcmp("freeResources", method) && func->freeResources)
    {
        DBusMessageIter iter;
        /*CID:65337 Uninitialized scalar variable*/
        dbus_int32_t tmp = 0 ;
        dbus_int32_t result = 0 ;
        dbus_int32_t priority = 0 ;
        

        dbus_message_iter_init (message, &iter);
        if(dbus_message_iter_get_arg_type (&iter) == DBUS_TYPE_INT32)
        {
            dbus_message_iter_get_basic (&iter, &priority);
        }


        result = func->freeResources(priority, func->freeResources_data);

        dbus_message_iter_init_append (reply, &iter);

        tmp = result;
        dbus_message_iter_append_basic (&iter, DBUS_TYPE_INT32, &tmp);
        if (!dbus_connection_send (conn, reply, NULL))
            CcspTraceError(("No memory\n"));

        dbus_message_unref (reply);
        return DBUS_HANDLER_RESULT_HANDLED;
    }
    else if(!strcmp(CCSP_DBUS_INTERFACE_BASE, interface) && !strcmp("getHealth", method) && func->getHealth)
    {
        DBusMessageIter iter;
        dbus_int32_t result ;

        dbus_message_iter_init (message, &iter);
        result = func->getHealth();
        dbus_message_iter_init_append (reply, &iter);

        dbus_message_iter_append_basic (&iter, DBUS_TYPE_INT32, &result);
        if (!dbus_connection_send (conn, reply, NULL))
            CcspTraceError(("No memory\n"));
        dbus_message_unref (reply);
        return DBUS_HANDLER_RESULT_HANDLED;
    }
    else if(!strcmp(CCSP_DBUS_INTERFACE_PA, interface) && !strcmp("restartBootstrap", method) && func->restartBootstrap)
    {
	    DBusMessageIter iter;
        dbus_int32_t tmp ;
        dbus_int32_t result ;

        result = func->restartBootstrap(func->restartBootstrap_data);

        dbus_message_iter_init_append (reply, &iter);
	
        tmp = (result==0)?0:1;
        
        dbus_message_iter_append_basic (&iter, DBUS_TYPE_INT32, &tmp);
        if (!dbus_connection_send (conn, reply, NULL))
            printf ("No memory\n");

        dbus_message_unref (reply);
        return DBUS_HANDLER_RESULT_HANDLED;    
    }

    dbus_message_unref (reply);
    reply = dbus_message_new_error (message,
                                    DBUS_ERROR_UNKNOWN_METHOD,
                                    "Unknown method invoked ");

    dbus_connection_send (conn, reply, NULL);
    dbus_message_unref (reply);
    return DBUS_HANDLER_RESULT_HANDLED;
}

DBusHandlerResult
CcspBaseIf_evt_callback (DBusConnection  *conn,
              DBusMessage     *message,
              void            *user_data)
{
    if(rbus_enabled == 1)
    {
        CcspTraceInfo(("came inside %s in rbus it will be handled in CcspBaseIf_evt_callback_rbus\n",__FUNCTION__));
        return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
    }
    CCSP_MESSAGE_BUS_INFO *bus_info =(CCSP_MESSAGE_BUS_INFO *) user_data;
    CCSP_Base_Func_CB* func = (CCSP_Base_Func_CB* )bus_info->CcspBaseIf_func;
    const char *interface = dbus_message_get_interface(message);
    const char *method   = dbus_message_get_member(message);
    //    fprintf(stderr, "evt_callback %s %s \n", interface , method);

    if(!strcmp(method,"NameOwnerChanged") && !strcmp(interface, "org.freedesktop.DBus") && func->componentDie)
    {
	    char *comp_name = 0;
	    char *old_owner = 0;
	    char *new_owner = 0;
        if(dbus_message_get_args (message,
                                  NULL,
                                  DBUS_TYPE_STRING, &comp_name,
                                  DBUS_TYPE_STRING, &old_owner,
                                  DBUS_TYPE_STRING, &new_owner,
                                  DBUS_TYPE_INVALID))
        {
            //		    fprintf(stderr, "evt_callback %s %s %s \n", comp_name, old_owner , new_owner);
		    if(new_owner && *new_owner != 0 && strcmp(comp_name,new_owner) )
		    {
			    func->componentDie(comp_name,1, func->componentDie_data);
			    
		    }    
		    
		    if(new_owner && *new_owner == 0 && strcmp(comp_name,old_owner))
		    {
			    func->componentDie(comp_name,0, func->componentDie_data);
			}
        }
	}

    if(!strcmp(method,"parameterValueChangeSignal") && !strcmp(interface, CCSP_DBUS_INTERFACE_EVENT) && func->parameterValueChangeSignal)
    {
        parameterSigStruct_t *val = 0;
        DBusMessageIter iter;
        DBusMessageIter array_iter;
        DBusMessageIter struct_iter;
        int param_size;
        int i;

        dbus_message_iter_init (message, &iter);

        DBUS_MESSAGE_ITER_RECURSE_SIG(&iter, &array_iter , DBUS_TYPE_ARRAY, 0, bus_info->freefunc);
        param_size = 0;
        do
        {
            int type = dbus_message_iter_get_arg_type (&array_iter);

            if (type == DBUS_TYPE_INVALID)
                break;
            param_size = param_size + 1;
        }
        while (dbus_message_iter_next (&array_iter));

        if(param_size)
        {
            val = bus_info->mallocfunc(param_size*sizeof(parameterSigStruct_t ));
            memset(val, 0, param_size*sizeof(parameterSigStruct_t ));
            DBUS_MESSAGE_ITER_RECURSE_SIG(&iter, &array_iter , DBUS_TYPE_ARRAY, val, bus_info->freefunc);
        }

        for(i = 0; i < param_size; i++)
        {

            DBUS_MESSAGE_ITER_RECURSE_SIG(&array_iter, &struct_iter , DBUS_TYPE_STRUCT, val, bus_info->freefunc);
            if(dbus_message_iter_get_arg_type (&struct_iter) == DBUS_TYPE_STRING)
            {
                dbus_message_iter_get_basic (&struct_iter, &val[i].parameterName);
            }
            dbus_message_iter_next (&struct_iter);
            if(dbus_message_iter_get_arg_type (&struct_iter) == DBUS_TYPE_STRING)
            {
                dbus_message_iter_get_basic (&struct_iter, &val[i].oldValue);
            }
            dbus_message_iter_next (&struct_iter);
            if(dbus_message_iter_get_arg_type (&struct_iter) == DBUS_TYPE_STRING)
            {
                dbus_message_iter_get_basic (&struct_iter, &val[i].newValue);
            }
            
            dbus_message_iter_next (&struct_iter);
            if(dbus_message_iter_get_arg_type (&struct_iter) == DBUS_TYPE_INT32)
            {
                dbus_message_iter_get_basic (&struct_iter, &val[i].type);
            }
            dbus_message_iter_next (&struct_iter);
            if(dbus_message_iter_get_arg_type (&struct_iter) == DBUS_TYPE_STRING)
            {
                dbus_message_iter_get_basic (&struct_iter, &val[i].subsystem_prefix);
            }

            dbus_message_iter_next (&struct_iter);
            if(dbus_message_iter_get_arg_type (&struct_iter) == DBUS_TYPE_UINT32)
            {
                dbus_message_iter_get_basic (&struct_iter, &val[i].writeID);
            }
            dbus_message_iter_next (&array_iter);
        }
        
        func->parameterValueChangeSignal(val, param_size,func->parameterValueChangeSignal_data);
                    
        bus_info->freefunc(val);

	}
    if(!strcmp(method,"deviceProfileChangeSignal") && !strcmp(interface, CCSP_DBUS_INTERFACE_EVENT) && func->deviceProfileChangeSignal)
    {
        dbus_bool isAvailable;
	    char *component_name = 0;
	    char *component_dbus_path = 0;
        if(dbus_message_get_args (message,
                                  NULL,
                                  DBUS_TYPE_STRING, &component_name,
                                  DBUS_TYPE_STRING, &component_dbus_path,
                                  DBUS_TYPE_BOOLEAN, &isAvailable,
                                  DBUS_TYPE_INVALID))
        {
    	    func->deviceProfileChangeSignal(component_name,component_dbus_path,isAvailable,func->deviceProfileChangeSignal_data);
        }
	    
	}
    if(!strcmp(method,"currentSessionIDSignal") && !strcmp(interface, CCSP_DBUS_INTERFACE_EVENT) && func->currentSessionIDSignal)
    {
        dbus_int32_t  sessionID;
        dbus_int32_t  priority;
        if(dbus_message_get_args (message,
                                  NULL,
                                  DBUS_TYPE_INT32, &priority,
                                  DBUS_TYPE_INT32, &sessionID,
                                  DBUS_TYPE_INVALID))
    	    func->currentSessionIDSignal(priority, sessionID, func->currentSessionIDSignal_data);
	}
    if(!strncmp(method,"telemetryDataSignal",MAX_EVENT_NAME_LEN) && !strncmp(interface, CCSP_DBUS_INTERFACE_EVENT,MAX_EVENT_NAME_LEN) && func->telemetryDataSignal)
    {
        char* telemetry_data = 0;
        if(dbus_message_get_args (message,
                                  NULL,
                                  DBUS_TYPE_STRING, &telemetry_data,
                                  DBUS_TYPE_INVALID))
           func->telemetryDataSignal(telemetry_data, func->telemetryDataSignal_data);
    }
    if(!strcmp(method,"diagCompleteSignal") && !strcmp(interface, CCSP_DBUS_INTERFACE_EVENT) && func->diagCompleteSignal)
    {
    	    func->diagCompleteSignal(func->diagCompleteSignal_data);
	}
    if(!strcmp(method,"systemReadySignal") && !strcmp(interface, CCSP_DBUS_INTERFACE_EVENT) && func->systemReadySignal)
    {
    	    func->systemReadySignal(func->systemReadySignal_data);
    }

    if(!strcmp(method,"webconfigSignal") && !strcmp(interface, CCSP_DBUS_INTERFACE_EVENT) && func->webconfigSignal)
    {
        char* webconfig_data = 0;
        if(dbus_message_get_args (message,
                                  NULL,
                                  DBUS_TYPE_STRING, &webconfig_data,
                                  DBUS_TYPE_INVALID))
           func->webconfigSignal(webconfig_data, func->webconfigSignal_data);
    }
            if(!strcmp(method,"multiCompBroadCastSignal") && !strcmp(interface, CCSP_DBUS_INTERFACE_EVENT) && func->multiCompBroadCastSignal)
            {
                char* multiCompBroadCastSignal_data = 0;
                if(dbus_message_get_args (message,
                                          NULL,
                                          DBUS_TYPE_STRING, &multiCompBroadCastSignal_data,
                                          DBUS_TYPE_INVALID))
                   func->multiCompBroadCastSignal(multiCompBroadCastSignal_data, func->multiCompBroadCastSignal_data);
            }
                if(!strcmp(method,"multiCompMasterProcessSignal") && !strcmp(interface, CCSP_DBUS_INTERFACE_EVENT) && func->multiCompMasterProcessSignal)
            {
                char* multiCompMasterProcessSignal_data = 0;
                if(dbus_message_get_args (message,
                                          NULL,
                                          DBUS_TYPE_STRING, &multiCompMasterProcessSignal_data,
                                          DBUS_TYPE_INVALID))
                   func->multiCompMasterProcessSignal(multiCompMasterProcessSignal_data, func->multiCompMasterProcessSignal_data);
            }
            if(!strcmp(method,"multiCompSlaveProcessSignal") && !strcmp(interface, CCSP_DBUS_INTERFACE_EVENT) && func->multiCompSlaveProcessSignal)
            {
                char* multiCompSlaveProcessSignal_data = 0;
                if(dbus_message_get_args (message,
                                          NULL,
                                          DBUS_TYPE_STRING, &multiCompSlaveProcessSignal_data,
                                          DBUS_TYPE_INVALID))
                   func->multiCompSlaveProcessSignal(multiCompSlaveProcessSignal_data, func->multiCompSlaveProcessSignal_data);
            }

            if(!strcmp(method,"TunnelStatus") && !strcmp(interface, CCSP_DBUS_INTERFACE_EVENT) && func->TunnelStatus)
            {
                char* TunnelStatus_data = 0;
                if(dbus_message_get_args (message,
                                          NULL,
                                          DBUS_TYPE_STRING, &TunnelStatus_data,
                                          DBUS_TYPE_INVALID))
                   func->TunnelStatus(TunnelStatus_data, func->TunnelStatus_data);
            }

            if(!strcmp(method,"WifiDbStatus") && !strcmp(interface, CCSP_DBUS_INTERFACE_EVENT) && func->WifiDbStatus)
            {
                char* WifiDbStatus_data = 0;
                if(dbus_message_get_args (message,
                                          NULL,
                                          DBUS_TYPE_STRING, &WifiDbStatus_data,
                                          DBUS_TYPE_INVALID))
                   func->WifiDbStatus(WifiDbStatus_data, func->WifiDbStatus_data);
            }
    
	if(!strcmp(method,STBSERVICE_CDL_DLC_SIGNAL) && !strcmp(interface, STBSERVICE_CDL_INTERFACE) && func->dlCompleteSignal)
    {
        DBusMessageIter iter = {0};
        dbus_message_iter_init (message, &iter);
        dbus_uint32_t startTime = 0;    /*RDKB-6233, CID-32992, init before use*/
        dbus_uint32_t completeTime = 0; /*RDKB-6233, CID-33215, init before use*/
        
        dbus_message_iter_next (&iter);
        if(dbus_message_iter_get_arg_type (&iter) == DBUS_TYPE_UINT32)
        {
            dbus_message_iter_get_basic (&iter, &startTime);
        }
        dbus_message_iter_next (&iter);
        if(dbus_message_iter_get_arg_type (&iter) == DBUS_TYPE_UINT32)
        {
            dbus_message_iter_get_basic (&iter, &completeTime);
        }
        func->dlCompleteSignal(startTime, completeTime ,func->dlCompleteSignal_data);
	}
    if(!strcmp(method,"systemRebootSignal") && !strcmp(interface, CCSP_DBUS_INTERFACE_EVENT) && func->systemRebootSignal)
    {
    	    func->systemRebootSignal(func->systemRebootSignal_data);
	}
	else if(bus_info->default_sig_callback)
	   return bus_info->default_sig_callback(conn,message,user_data);
       
    return DBUS_HANDLER_RESULT_HANDLED;
}

/* Handle a value change event from an rbus 2.0 api component
 * As we migrate components from Ccsp to rbus2, more and will be based on rbus2
 */
int handleValueChangeEvent_rbus(const char * object_name, const char * event_name, rbusMessage message, void * user_data)
{
    UNREFERENCED_PARAMETER(object_name);
    CCSP_MESSAGE_BUS_INFO *bus_info =(CCSP_MESSAGE_BUS_INFO *) user_data;
    CCSP_Base_Func_CB* func = (CCSP_Base_Func_CB* )bus_info->CcspBaseIf_func;
    char const* msg_event_name;
    int msg_event_type;
    char const* msg_obj_name;
    int msg_obj_type;
    int msg_num_props;
    char const* msg_prop1_name;
    int msg_prop1_type;
    uint32_t msg_prop1_len;
    uint8_t const* msg_prop1_data;

    /*get data from the message the specific way rbus 2.0 packs it*/
    rbusMessage_GetString(message, (char const**) &msg_event_name);
    rbusMessage_GetInt32(message, (int*) &msg_event_type);
    rbusMessage_GetString(message, &msg_obj_name);
    rbusMessage_GetInt32(message, &msg_obj_type);
    rbusMessage_GetInt32(message, (int*) &msg_num_props);
    rbusMessage_GetString(message, (char const**) &msg_prop1_name);
    rbusMessage_GetInt32(message, (int*) &msg_prop1_type);
    rbusMessage_GetBytes(message, &msg_prop1_data, &msg_prop1_len);

    if(!strcmp(event_name, "Device.CR.SystemReady") && func->systemReadySignal)
    {
        /*expect to get a 1 bytes boolean*/
        if(msg_prop1_type == RBUS_BOOLEAN && msg_prop1_len == 1)
        {
            if(msg_prop1_data[0])
            {
                CcspTraceInfo(("System Ready Event Received\n"));

                func->systemReadySignal(func->systemReadySignal_data);
            }
            else
            {
                CcspTraceInfo(("%s %s unexpected value %d\n", __FUNCTION__, event_name, (int)msg_prop1_data[0]));
            }
        }
        else
        {
            CcspTraceError(("%s %s unexpected type %d\n", __FUNCTION__, event_name, msg_prop1_type));
        }
    }
    return RBUSCORE_SUCCESS;
}

int CcspBaseIf_evt_callback_rbus(const char * object_name, const char * event_name, rbusMessage message, void * user_data)
{
    UNREFERENCED_PARAMETER(object_name);
    CCSP_MESSAGE_BUS_INFO *bus_info =(CCSP_MESSAGE_BUS_INFO *) user_data;
    CCSP_Base_Func_CB* func = (CCSP_Base_Func_CB* )bus_info->CcspBaseIf_func;

    if(!strcmp(event_name,"parameterValueChangeSignal") && func->parameterValueChangeSignal)
    {
        parameterSigStruct_t *val = 0;
        int param_size = 0, i = 0;

        rbusMessage_GetInt32(message, (int32_t*)&param_size);

        if(param_size)
        {
            val = bus_info->mallocfunc(param_size*sizeof(parameterSigStruct_t ));
            memset(val, 0, param_size*sizeof(parameterSigStruct_t ));
        }

        for(i = 0; i < param_size; i++)
        {
            rbusMessage_GetString(message, &val[i].parameterName);
            rbusMessage_GetString(message, (const char**)&val[i].oldValue);
            rbusMessage_GetString(message, (const char**)&val[i].newValue);
            rbusMessage_GetInt32(message, (int32_t*)&val[i].type);
            rbusMessage_GetString(message, &val[i].subsystem_prefix);
            rbusMessage_GetInt32(message, (int32_t*)&val[i].writeID);
        }

        func->parameterValueChangeSignal(val, param_size,func->parameterValueChangeSignal_data);

        bus_info->freefunc(val);
    }
    else if(!strcmp(event_name,"deviceProfileChangeSignal") && func->deviceProfileChangeSignal)
    {
        int32_t isAvailable;
        char *component_name = 0;
        char *component_dbus_path = 0;
        rbusMessage_GetString(message, (const char**)&component_name);
        rbusMessage_GetString(message, (const char**)&component_dbus_path);
        rbusMessage_GetInt32(message, &isAvailable);
        func->deviceProfileChangeSignal(component_name,component_dbus_path,isAvailable,func->deviceProfileChangeSignal_data);
    }

    else if(!strcmp(event_name,"currentSessionIDSignal") && func->currentSessionIDSignal)
    {
        int32_t sessionID;
        int32_t priority;
        rbusMessage_GetInt32(message, &priority);
        rbusMessage_GetInt32(message, &sessionID);
        func->currentSessionIDSignal(priority, sessionID, func->currentSessionIDSignal_data);
    }
    else if(!strcmp(event_name,"diagCompleteSignal") && func->diagCompleteSignal)
    {
        func->diagCompleteSignal(func->diagCompleteSignal_data);
    }
    else if(!strcmp(event_name, "Device.CR.SystemReady") && func->systemReadySignal)
    {
        handleValueChangeEvent_rbus(object_name, event_name, message, user_data);
    }
    else if(!strcmp(event_name,"systemRebootSignal") && func->systemRebootSignal)
    {
        func->systemRebootSignal(func->systemRebootSignal_data);
    }
    else
    {
        CcspTraceError(("%s %s event not handled\n",__FUNCTION__, event_name));
        return RBUSCORE_ERROR_UNSUPPORTED_EVENT;
    }

    return RBUSCORE_SUCCESS;
}

void  CcspBaseIf_Set_Default_Event_Callback
(
    void* bus_handle,
    DBusObjectPathMessageFunction   callback
)
{
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    bus_info->default_sig_callback = callback;
    
}

/*
    This function is used to save dbus call in a round queue.
*/
void CcspBaseIf_deadlock_detection_log_save 
( 
    void
)
{
    CCSP_DEADLOCK_DETECTION_INFO* info           = (CCSP_DEADLOCK_DETECTION_INFO*)(&deadlock_detection_info);
    parameterValStruct_t *        parameterVal   = 0;     //setParameterValues
    char *                        parameterName  = 0;     //getParameterNames
    char **                       parameterNames = 0;     //getParameterValues / getParameterAttributes
    parameterAttributeStruct_t  * parameterAttribute = 0; //setParameterAttributes
    char *                        str            = NULL;  //AddTblRow / DeleteTblRow
    unsigned long                 size           = 0;
    unsigned long                 index          = deadlock_detection_log_index;
    unsigned int                  i              = 0;
    char                          timestr[128]   = {0};
    time_t                        t1             = GetCurrentTime();
    
    if ( !deadlock_detection_enable )
        return;

    strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", localtime(&t1));   
    
    pthread_mutex_lock(&(info->info_mutex));

    if ( info->messageType )
    {
        size = info->size;
        if ( strstr("setParameterValues", info->messageType ) )
        {
            parameterVal = (parameterValStruct_t *)(info->parameterInfo);
            
            CCSP_DEADLOCK_INFO_PRINT("%s **Ccsp Dbus call:setParameterValues.\n", timestr);
            for ( i = 0; i<size; i++ )
            {
                CCSP_DEADLOCK_INFO_PRINT("parameters[%d] : %s/%s\n", i, parameterVal[i].parameterName, parameterVal[i].parameterValue);
            }
        }
        else if ( strstr("getParameterValues", info->messageType ))
        {
            parameterNames = (char **)(info->parameterInfo);

            CCSP_DEADLOCK_INFO_PRINT("%s **Ccsp Dbus call:getParameterValues.\n", timestr);
            for ( i = 0; i<size; i++ )
            {
                CCSP_DEADLOCK_INFO_PRINT("parameters[%d] : %s\n", i, parameterNames[i]);            
            }
        }
        else if ( strstr("getParameterNames", info->messageType ))
        {
            parameterName = (char *)(info->parameterInfo);

            CCSP_DEADLOCK_INFO_PRINT("%s **Ccsp Dbus call:getParameterNames.\n", timestr);
            CCSP_DEADLOCK_INFO_PRINT("parameterName : %s\n", parameterName);            
        }
        else if ( strstr("getParameterAttributes", info->messageType ))
        {
            parameterNames = (char **)(info->parameterInfo);

            CCSP_DEADLOCK_INFO_PRINT("%s **Ccsp Dbus call:getParameterAttributes.\n", timestr);
            for ( i = 0; i<size; i++ )
            {
                CCSP_DEADLOCK_INFO_PRINT("parameters[%d] : %s\n", i, parameterNames[i]);            
            }
        }
        else if ( strstr("setParameterAttributes", info->messageType ))
        {
            parameterAttribute = (parameterAttributeStruct_t *)(info->parameterInfo);

            CCSP_DEADLOCK_INFO_PRINT("%s **Ccsp Dbus call:setParameterAttributes.\n", timestr);
            for ( i = 0; i<size; i++ )
            {
                CCSP_DEADLOCK_INFO_PRINT("parameters[%d] : %s, NtfChg:%d,Ntf:%d,Acc:%u,AcCtlChg:%d,AcCtlBitm:%u\n", \
                                     i, \
                                     parameterAttribute[i].parameterName,\
                                     parameterAttribute[i].notificationChanged,\
                                     parameterAttribute[i].notification,\
                                     parameterAttribute[i].access,\
                                     parameterAttribute[i].accessControlChanged,\
                                     parameterAttribute[i].accessControlBitmask);
            }
        }
        else if ( strstr("AddTblRow", info->messageType ))
        {
            str = (char * )(info->parameterInfo);
            CCSP_DEADLOCK_INFO_PRINT("%s **Ccsp Dbus call:AddTblRow.\n", timestr);
            CCSP_DEADLOCK_INFO_PRINT("row: %s\n", str);         
        }
        else if ( strstr("DeleteTblRow", info->messageType ))
        {
            str = (char * )(info->parameterInfo);
            CCSP_DEADLOCK_INFO_PRINT("%s **Ccsp Dbus call:DeleteTblRow.\n", timestr);
            CCSP_DEADLOCK_INFO_PRINT("row: %s\n", str);
        }
        else if ( strstr("setCommit", info->messageType ))
        {
            CCSP_DEADLOCK_INFO_PRINT("%s **Ccsp Dbus call:setCommit.\n", timestr);
        }

    }

    deadlock_detection_log_index = index;
    
    pthread_mutex_unlock(&(info->info_mutex));

    return;
}

/*
    This function is used to trigger log cache print.
    When some signals are received, call this funtion.
    When deadlock happened, call this function.
*/
void
CcspBaseIf_deadlock_detection_log_print
(
    int sig
)
{
    CCSP_DEADLOCK_DETECTION_INFO* info           = (CCSP_DEADLOCK_DETECTION_INFO*)(&deadlock_detection_info);
    FILE *                        fd             = NULL;
    int                           i              = 0;
    char                          timestr[128]   = {0};
    time_t                        t1             = GetCurrentTime();
    
    if ( !deadlock_detection_enable )
        return;

    strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", localtime(&t1));   


    pthread_mutex_lock(&(info->info_mutex));

    fd = fopen( deadlock_detection_log_file, "a+" );
    if(fd) /*RDKB-6233, CID-32959 , null check before use*/
    {
        fseek(fd,0L,SEEK_END);

        if ( ftell(fd) > 10000)
        {
            fclose(fd);
            fd = fopen( deadlock_detection_log_file, "w+" );
        }
    }

    CCSP_DEADLOCK_PRINT(("\n%s CCSP Dbus Call stack trace printing  --  ************************************\n", timestr))

    /*write previous logs */
    i = deadlock_detection_log_index;
    do
    {
        if ( (*deadlock_detection_log)[i][0] )
            CCSP_DEADLOCK_PRINT(((*deadlock_detection_log)[i]));

        i++;

        if ( i >= deadlock_detection_log_linenum )
            i = 0;

        if ( (unsigned int)i == deadlock_detection_log_index )
            break;
    }while(1);

    /* When signal trigger this function. Print the following */
    if ( sig == SIGINT ) {
    	CCSP_DEADLOCK_PRINT(("%s **SIGINT received! exiting now !!!!!!!!  The last accessing call is above.\n", timestr));
    }
    else if ( sig == SIGUSR1 ) {
    	CCSP_DEADLOCK_PRINT(("%s **SIGUSR1 received! The last accessing call is last one.\n", timestr));
    }
    else if ( sig == SIGUSR2 ) {
    	CCSP_DEADLOCK_PRINT(("%s **SIGUSR2 received! The last accessing call is last one.\n", timestr));
    }
    else if ( sig == SIGCHLD ) {
    	CCSP_DEADLOCK_PRINT(("%s **SIGCHLD received! The last accessing call is last one.\n", timestr));
    }
    else if ( sig == SIGPIPE ) {
    	CCSP_DEADLOCK_PRINT(("%s **SIGPIPE received! The last accessing call is last one.\n", timestr));
    }
    else if ( sig == SIGTERM )
    {
        CCSP_DEADLOCK_PRINT(("%s **SIGTERM received! exiting now !!!!!!!!  The last accessing call is last one.\n", timestr));
    }
    else if ( sig == SIGKILL )
    {
        CCSP_DEADLOCK_PRINT(("%s **SIGKILL received! exiting now !!!!!!!!  The last accessing call is last one.\n", timestr));
    }
    else if ( sig != 0 ) {
    	CCSP_DEADLOCK_PRINT(("%s **Signal %d received! exiting now !!!!!!!!  The last accessing call is last one.\n", timestr, sig));
    }
    
    if ( fd ) 
        fclose(fd);

    pthread_mutex_unlock(&(info->info_mutex));

    return;
    
}


void sig_empty_handler(int sig)
{
    UNREFERENCED_PARAMETER(sig);
    return;
}

/*
    Create a thread to monitor dbus call. 
    When deadlock happened, trigger to print trace stack
*/
void *
CcspBaseIf_Deadlock_Detection_Thread
(
    void * user_data
)
{
    UNREFERENCED_PARAMETER(user_data);
    CCSP_DEADLOCK_DETECTION_INFO* info           = (CCSP_DEADLOCK_DETECTION_INFO*)(&deadlock_detection_info);
    FILE *                        fd             = NULL;

    struct timespec               time1          = {0};
    unsigned long                 currentTime    = 0;
    unsigned long                 deadLockHappen = 0;
    
    time1.tv_sec = 3;
    time1.tv_nsec = 0;

    while( deadlock_detection_enable )
    {
        if ( info->messageType )
        {
            pthread_mutex_lock(&(info->info_mutex));

            currentTime = GetCurrentTime();
            if ( (currentTime - info->enterTime) >= info->detectionDuration )
            {
               // (2*(info->detectionDuration)) This check is just for checking invalid time difference  
                if (( currentTime - info->enterTime ) > (2*(info->detectionDuration))) 
                {
                    //This is invalid time difference. Just neglect
                    info->enterTime = currentTime - info->timepassed ;
                    CcspTraceWarning((" **** info->timepassed %lu ******\n",info->timepassed));
                    CcspTraceWarning((" **** info->enterTime %lu ******\n",info->enterTime));
                    CcspTraceWarning((" **** currentTime %lu ******\n",currentTime));
                }
                else
                {
                    deadLockHappen = 1;
                }  
            }
            
            pthread_mutex_unlock(&(info->info_mutex));
        }

        if (deadLockHappen)
        {
            break;
        }
    
        nanosleep(&time1, NULL);
    }

    if (deadLockHappen)
    {  
        char                          timestr[128]   = {0};
        time_t                        t1             = GetCurrentTime();
        
        strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", localtime(&t1));   
    
        CcspBaseIf_deadlock_detection_log_print(0);

        pthread_mutex_lock(&(info->info_mutex));
        
        fd = fopen( deadlock_detection_log_file, "a+" );

        if(fd) /*RDKB-6233, CID-33441, null check before use*/
        {
            if ( ftell(fd) > 10000) rewind(fd);
        }

        CCSP_DEADLOCK_PRINT(("%s **CCSP Deadlock happened. Exiting after(5 sec)!!!!!!!!     The last fail accessing call is last one.\n", timestr));

        time1.tv_sec = 5;
        nanosleep(&time1, NULL);

        if ( fd ) 
            fclose(fd);
        
        pthread_mutex_unlock(&(info->info_mutex));
        
        // TODO: When a memory access violation occurs, the empty signal handler simply returns and execution continues.
        // This immediately triggers another SIGSEGV and the cycle repeats endlessly.
        // The obvious workaround it to comment out these 4 lines. We are trying to terminate the process anyway,
        // so we should let the SIGSEGV crash the process instead of trying to recover.
        //signal(SIGSEGV, sig_empty_handler);
        //signal(SIGBUS, sig_empty_handler);
        //signal(SIGFPE, sig_empty_handler);
        //signal(SIGILL, sig_empty_handler);

        
        exit(-1);
    }

    
    return NULL;
}

