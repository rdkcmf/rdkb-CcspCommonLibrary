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
#include <dslh_definitions_database.h>

#ifdef PSM_SLAP_VAR
#include <slap_definitions.h>
#endif
#include <ccsp_psm_helper.h>
#include "ccsp_trace.h"

/* For AnscEqualString */
#include "ansc_platform.h"

int   CcspBaseIf_timeout_seconds        = 60; //seconds
int   CcspBaseIf_timeout_getval_seconds = 120; //seconds

int CcspBaseIf_freeResources(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    int priority
)
{
    DBusMessage *message;
    DBusMessage *reply;
    int ret = CCSP_FAILURE;
    dbus_int32_t tmp1 ;
    DBusMessageIter iter;
	dbus_int32_t tmp;

    message = dbus_message_new_method_call (dst_component_id,
                                            dbus_path,
                                            CCSP_DBUS_INTERFACE_BASE,
                                            "freeResources");
    if (!message )
    {
        CcspTraceError(("No memory\n"));
        return CCSP_ERR_MEMORY_ALLOC_FAIL;
    }
    dbus_message_iter_init_append (message, &iter);

    tmp = priority;
    if (!dbus_message_iter_append_basic (&iter, DBUS_TYPE_INT32, &tmp))

    ret = CCSP_Message_Bus_Send_Msg(bus_handle, message, CcspBaseIf_timeout_seconds , &reply);
    if(reply )
    {
        if(dbus_message_get_args (reply,
                                  NULL,
                                  DBUS_TYPE_INT32, &tmp1,
                                  DBUS_TYPE_INVALID))
        {
            ret = tmp1;
        }

        dbus_message_unref (reply);
    }
    dbus_message_unref (message);

    return ret;
}

int CcspBaseIf_queryStatus(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    int *internalState
)
{
    char * parameterNames[2];
    char tmp[512];
    int size;
    int ret ;
    parameterValStruct_t **parameterVal = 0;
//    sprintf(tmp,"%s%s.%s",CcspBaseIf_DataModel,dst_component_id, "State");
    sprintf(tmp,"%s.%s",dst_component_id, "State");
    parameterNames[0] = tmp;

    ret = CcspBaseIf_getParameterValues(
              bus_handle,
              dst_component_id,
              dbus_path,
              parameterNames,
              1,
              &size,
              &parameterVal );
    if(ret == CCSP_SUCCESS && size >= 1)
    {
        *internalState = atoi(parameterVal[0]->parameterValue);
        free_parameterValStruct_t(bus_handle, size, parameterVal);
    }

    return ret;
}

int CcspBaseIf_healthCheck(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    int *health)
{
    char * parameterNames[2];
    char tmp[512];
    parameterValStruct_t **parameterVal = 0;
    int size;
    int ret ;

//    sprintf(tmp,"%s%s.%s",CcspBaseIf_DataModel,dst_component_id, "Health");
    sprintf(tmp,"%s.%s",dst_component_id, "State");
    parameterNames[0] = tmp;
    ret = CcspBaseIf_getParameterValues(
              bus_handle,
              dst_component_id,
              dbus_path,
              parameterNames,
              1,
              &size,
              &parameterVal );
    if(ret == CCSP_SUCCESS  && size >= 1)
    {
        *health = atoi(parameterVal[0]->parameterValue);
        free_parameterValStruct_t(bus_handle, size, parameterVal);
    }

    return ret;
}


int CcspBaseIf_getAllocatedMemory(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    int *directAllocatedMemory)
{
    char * parameterNames[2];
    char tmp[512];
    int size;
    parameterValStruct_t **parameterVal = 0;
    int ret ;
//    sprintf(tmp,"%s%s.%s",CcspBaseIf_DataModel,dst_component_id, "Memory.Consumed");
    sprintf(tmp,"%s.%s",dst_component_id, "State");
    parameterNames[0] = tmp;

    ret = CcspBaseIf_getParameterValues(
              bus_handle,
              dst_component_id,
              dbus_path,
              parameterNames,
              1,
              &size,
              &parameterVal );
    if(ret == CCSP_SUCCESS  && size >= 1)
    {
        *directAllocatedMemory = atoi(parameterVal[0]->parameterValue);
        free_parameterValStruct_t(bus_handle, size, parameterVal);
    }

    return ret;
}

int CcspBaseIf_getMaxMemoryUsage(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    int *memoryUsage)
{
    char * parameterNames[2];
    char tmp[512];
    int size;
    int ret ;
    parameterValStruct_t **parameterVal = 0;
//    sprintf(tmp,"%s%s.%s",CcspBaseIf_DataModel,dst_component_id, "Memory.MinUsage");
    sprintf(tmp,"%s.%s",dst_component_id, "State");
    parameterNames[0] = tmp;

    ret = CcspBaseIf_getParameterValues(
              bus_handle,
              dst_component_id,
              dbus_path,
              parameterNames,
              1,
              &size,
              &parameterVal );
    if(ret == CCSP_SUCCESS  && size >= 1)
    {
        *memoryUsage = atoi(parameterVal[0]->parameterValue);
        free_parameterValStruct_t(bus_handle, size, parameterVal);
    }

    return ret;
}


int CcspBaseIf_setParameterValues(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    int sessionId,
    unsigned int writeID,
    parameterValStruct_t *val,
    int size,
    dbus_bool commit,
    char ** invalidParameterName
)
{
    DBusMessage *message;
    DBusMessage *reply;
    int ret = CCSP_FAILURE;
    dbus_int32_t res ;
    DBusMessageIter iter;
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    DBusMessageIter array_iter;
    DBusMessageIter struct_iter;
    dbus_int32_t tmp ;
    dbus_uint32_t utmp ;
    int i;

    *invalidParameterName = 0;
    message = dbus_message_new_method_call (dst_component_id,
                                            dbus_path,
                                            CCSP_DBUS_INTERFACE_BASE,
                                            "setParameterValues");
    if (!message )
    {
        CcspTraceError(("No memory\n"));
        return CCSP_ERR_MEMORY_ALLOC_FAIL;
    }

    dbus_message_iter_init_append (message, &iter);
    tmp = sessionId;
    if (!dbus_message_iter_append_basic (&iter, DBUS_TYPE_INT32, &tmp))
        CcspTraceError(("error\n"));

    utmp = writeID;

    if (!dbus_message_iter_append_basic (&iter, DBUS_TYPE_UINT32, &utmp))
        CcspTraceError(("error\n"));

    ret = dbus_message_iter_open_container (&iter,
                                            DBUS_TYPE_ARRAY,
                                            "(ssi)",
                                            &array_iter);
    for(i = 0; i < size; i++)
    {
        dbus_message_iter_open_container (&array_iter,
                                          DBUS_TYPE_STRUCT,
                                          "ssi",
                                          &struct_iter);

        DBUS_MESSAGE_APPEND_STRING(&struct_iter, val[i].parameterName);
        DBUS_MESSAGE_APPEND_STRING(&struct_iter, val[i].parameterValue);
        tmp = val[i].type;
        ret = dbus_message_iter_append_basic (&struct_iter, DBUS_TYPE_INT32, &tmp);

        dbus_message_iter_close_container (&array_iter,
                                           &struct_iter);
    }

    ret = dbus_message_iter_close_container (&iter,
            &array_iter);

    tmp = size;
    dbus_message_iter_append_basic (&iter, DBUS_TYPE_INT32, &tmp);
    dbus_message_iter_append_basic (&iter, DBUS_TYPE_BOOLEAN, &commit);
    ret = CCSP_Message_Bus_Send_Msg(bus_handle, message, CcspBaseIf_timeout_seconds , &reply);
    if(reply )
    {
	    char *str = 0;
        DBusMessageIter iter;
        dbus_message_iter_init (reply, &iter);
        if(dbus_message_iter_get_arg_type (&iter) == DBUS_TYPE_STRING)
        {
            dbus_message_iter_get_basic (&iter, &str);
            if(str)
            {
               *invalidParameterName = bus_info->mallocfunc(strlen(str)+1);
                strcpy(*invalidParameterName, str);
            }
        }

        dbus_message_iter_next (&iter);
        ret = CCSP_CR_ERR_INVALID_PARAM;
        if(dbus_message_iter_get_arg_type (&iter) == DBUS_TYPE_INT32)
        {
            dbus_message_iter_get_basic (&iter, &res);
            ret = res;
        }

        dbus_message_unref (reply);
    }
    dbus_message_unref (message);
    return ret;
}

int CcspBaseIf_setCommit(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    int sessionId,
    unsigned int writeID,
    dbus_bool commit
)
{
    DBusMessage *message;
    DBusMessage *reply;
    int ret = CCSP_FAILURE;
    dbus_int32_t tmp1 ;
    dbus_uint32_t utmp ;

    message = dbus_message_new_method_call (dst_component_id,
                                            dbus_path,
                                            CCSP_DBUS_INTERFACE_BASE,
                                            "setCommit");
    if (!message )
    {
        CcspTraceError(("No memory\n"));
        return CCSP_ERR_MEMORY_ALLOC_FAIL;
    }
    tmp1 = sessionId;
    dbus_message_append_args (message, DBUS_TYPE_INT32, &tmp1, DBUS_TYPE_INVALID);
    utmp = writeID;
    dbus_message_append_args (message, DBUS_TYPE_UINT32, &utmp, DBUS_TYPE_INVALID);
    dbus_message_append_args (message, DBUS_TYPE_BOOLEAN, &commit, DBUS_TYPE_INVALID);

    ret = CCSP_Message_Bus_Send_Msg(bus_handle, message, CcspBaseIf_timeout_seconds , &reply);
    if(reply )
    {
        if(dbus_message_get_args (reply,
                                  NULL,
                                  DBUS_TYPE_INT32, &tmp1,
                                  DBUS_TYPE_INVALID))
        {
            ret = tmp1;
        }

        dbus_message_unref (reply);
    }
    dbus_message_unref (message);

    return ret;
}

//caller need free parameterval
int CcspBaseIf_getParameterValues(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    char * parameterNames[],
    int param_size,
    int *val_size,
    parameterValStruct_t ***parameterval
)
{
    DBusMessage *message;
    DBusMessage *reply;
    int ret = CCSP_FAILURE;
    dbus_int32_t res ;
    dbus_int32_t tmp ;
    DBusMessageIter iter;
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    DBusMessageIter array_iter;
    int i;
    dbus_uint32_t utmp ;
    unsigned int writeID;

    parameterValStruct_t **val = 0;
    *val_size = 0;

    message = dbus_message_new_method_call (dst_component_id,
                                            dbus_path,
                                            CCSP_DBUS_INTERFACE_BASE,
                                            "getParameterValues");
    if (!message )
    {
        CcspTraceError(("No memory\n"));
        return CCSP_ERR_MEMORY_ALLOC_FAIL;
    }

    dbus_message_iter_init_append (message, &iter);

    if ( _ansc_strcmp(bus_info->component_id, "ccsp.busclient" ) == 0 )
    {
        writeID = DSLH_MPA_ACCESS_CONTROL_CLIENTTOOL;
    }
    else if ( _ansc_strcmp(bus_info->component_id, "ccsp.cisco.spvtg.ccsp.snmp" ) == 0 )
    {
        writeID = DSLH_MPA_ACCESS_CONTROL_SNMP;
    }
    else if ( _ansc_strcmp(bus_info->component_id, "com.cisco.spvtg.ccsp.lms") == 0 )
    {
        writeID = DSLH_MPA_ACCESS_CONTROL_LM;
    }
    else if ( _ansc_strcmp(bus_info->component_id, "com.cisco.spvtg.ccsp.wifi") == 0 )
    {
        writeID = DSLH_MPA_ACCESS_CONTROL_WIFI;
    }
    else
        writeID = DSLH_MPA_ACCESS_CONTROL_ACS;

    utmp = writeID;
    if (!dbus_message_iter_append_basic (&iter, DBUS_TYPE_UINT32, &utmp))
        CcspTraceError(("error\n"));

    ret = dbus_message_iter_open_container (&iter,
                                            DBUS_TYPE_ARRAY,
                                            "s",
                                            &array_iter);
    for(i = 0; i < param_size; i++)
    {
        DBUS_MESSAGE_APPEND_STRING( &array_iter, parameterNames[i]);
//        ret = dbus_message_iter_append_basic (&array_iter, DBUS_TYPE_STRING, &parameterNames[i]);
    }

    ret = dbus_message_iter_close_container (&iter,
            &array_iter);

    tmp = param_size;
    dbus_message_iter_append_basic (&iter, DBUS_TYPE_INT32, &tmp);

    ret = CCSP_Message_Bus_Send_Msg(bus_handle, message, CcspBaseIf_timeout_getval_seconds , &reply);
    if(reply )
    {
        DBusMessageIter iter;

        DBusMessageIter subiter;
        DBusMessageIter array_iter;

        dbus_message_iter_init (reply, &iter);
        DBUS_MESSAGE_ITER_RECURSE(&iter, &array_iter , DBUS_TYPE_ARRAY, 0, CCSP_FAILURE, bus_info->freefunc);
        do
        {
            int type = dbus_message_iter_get_arg_type (&array_iter);

            if (type == DBUS_TYPE_INVALID)
                break;
            *val_size = *val_size + 1;
        }
        while (dbus_message_iter_next (&array_iter));

        dbus_message_iter_init (reply, &iter);
        DBUS_MESSAGE_ITER_RECURSE(&iter, &array_iter , DBUS_TYPE_ARRAY, 0, CCSP_FAILURE,bus_info->freefunc);
        if(*val_size)
        {
            val = bus_info->mallocfunc(*val_size*sizeof(parameterValStruct_t *));
            memset(val, 0, *val_size*sizeof(parameterValStruct_t *));
            for(i = 0; i < *val_size; i++)
            {
                char * str = 0;
                dbus_int32_t tmp ;

                DBUS_MESSAGE_ITER_RECURSE(&array_iter, &subiter , DBUS_TYPE_STRUCT, val, CCSP_FAILURE, bus_info->freefunc);
                val[i] = bus_info->mallocfunc(sizeof(parameterValStruct_t));
                memset(val[i], 0, sizeof(parameterValStruct_t));
                if(dbus_message_iter_get_arg_type (&subiter) == DBUS_TYPE_STRING)
                {
                    dbus_message_iter_get_basic (&subiter, &str);
                    if(str)
                    {
                        val[i]->parameterName = bus_info->mallocfunc(strlen(str)+1);
                        strcpy( val[i]->parameterName, str);
                    }
                    else
                        val[i]->parameterName = NULL;
                }
                dbus_message_iter_next (&subiter);

                if(dbus_message_iter_get_arg_type (&subiter) == DBUS_TYPE_STRING)
                {
                    dbus_message_iter_get_basic (&subiter, &str);
                    if(str)
                    {
                        val[i]->parameterValue = bus_info->mallocfunc(strlen(str)+1);
                        strcpy(val[i]->parameterValue, str);
                    }
                    else
                        val[i]->parameterValue = NULL;
                }

                dbus_message_iter_next (&subiter);

                if(dbus_message_iter_get_arg_type (&subiter) == DBUS_TYPE_INT32)
                {
                    dbus_message_iter_get_basic (&subiter, &tmp);
                    val[i]->type = tmp;
                }
                dbus_message_iter_next	 (&array_iter);
            }
        }
        dbus_message_iter_next	 (&iter);
        if(dbus_message_iter_get_arg_type (&iter) == DBUS_TYPE_INT32)
        {
            dbus_message_iter_get_basic (&iter, &res);
            ret = res;
        }

        dbus_message_unref (reply);
        
        /*check for shared memory*/
        if (*val_size == 1 &&
            !strcmp(val[0]->parameterName, SHM_PARAM_NAME) &&
            !strcmp(val[0]->parameterValue, SHM_PARAM_NAME) &&
            ret == CCSP_SUCCESS)
        {
            /*for shared memory, type field actually stores shared memory size*/
            int shmSize = val[0]->type;
            
            CcspTraceInfo(("dbus uses shared memory, totalSize %d\n", shmSize));
            free_parameterValStruct_t(bus_handle, *val_size, val);
            ret = CcspBaseIf_getParameterValues_Shm(bus_handle, shmSize, val_size, &val);
            
        }
    }

    //    CcspTraceDebug(("<pid%d>[%s]: ret=%d\n", getpid(), __FUNCTION__, ret));

    dbus_message_unref (message);
    *parameterval = val;
    return ret;

}


void  free_parameterValStruct_t (void *bus_handle, int size,parameterValStruct_t **val)
{
    int i;
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
	
	if(val) 
	{	
    if(size)
    {
        for(i = 0; i < size; i++)
        {
        		if(val[i]) 
				{
            if(val[i]->parameterName)  bus_info->freefunc(val[i]->parameterName);
            if(val[i]->parameterValue) bus_info->freefunc(val[i]->parameterValue);
            bus_info->freefunc(val[i]);
        }
        	}
    	}

        bus_info->freefunc(val);
    }
}

int CcspBaseIf_setParameterAttributes(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    int sessionId,
    parameterAttributeStruct_t *val,
    int size
)
{
    DBusMessage *message;
    DBusMessage *reply;
    int ret = CCSP_FAILURE;
    dbus_int32_t res ;
    dbus_int32_t  tmp ;
    dbus_uint32_t utmp ;
    DBusMessageIter iter;
    DBusMessageIter array_iter;
    DBusMessageIter struct_iter;
    int i;

    message = dbus_message_new_method_call (dst_component_id,
                                            dbus_path,
                                            CCSP_DBUS_INTERFACE_BASE,
                                            "setParameterAttributes");
    if (!message )
    {
        CcspTraceError(("No memory\n"));
        return CCSP_ERR_MEMORY_ALLOC_FAIL;
    }

    dbus_message_iter_init_append (message, &iter);

    tmp = sessionId;
    if (!dbus_message_iter_append_basic (&iter, DBUS_TYPE_INT32, &tmp))
        CcspTraceError(("error\n"));


    ret = dbus_message_iter_open_container (&iter,
                                            DBUS_TYPE_ARRAY,
                                            "(sbbibu)",
                                            &array_iter);
    for(i = 0; i < size ; i++)
    {
        dbus_message_iter_open_container (&array_iter,
                                          DBUS_TYPE_STRUCT,
                                          "sbbibu",
                                          &struct_iter);

        DBUS_MESSAGE_APPEND_STRING( &struct_iter, val[i].parameterName);

        ret = dbus_message_iter_append_basic (&struct_iter, DBUS_TYPE_BOOLEAN, &val[i].notificationChanged);
        ret = dbus_message_iter_append_basic (&struct_iter, DBUS_TYPE_BOOLEAN, &val[i].notification);
        tmp = val[i].access;
        ret = dbus_message_iter_append_basic (&struct_iter, DBUS_TYPE_INT32, &tmp);
        ret = dbus_message_iter_append_basic (&struct_iter, DBUS_TYPE_BOOLEAN, &val[i].accessControlChanged);
        utmp = val[i].accessControlBitmask;
        ret = dbus_message_iter_append_basic (&struct_iter, DBUS_TYPE_UINT32, &utmp);

        dbus_message_iter_close_container (&array_iter,
                                           &struct_iter);
    }

    ret = dbus_message_iter_close_container (&iter,
            &array_iter);

    tmp = size;
    dbus_message_iter_append_basic (&iter, DBUS_TYPE_INT32, &tmp);
    ret = CCSP_Message_Bus_Send_Msg(bus_handle, message, CcspBaseIf_timeout_seconds , &reply);
    if(reply )
    {
        if(dbus_message_get_args (reply,
                                  NULL,
                                  DBUS_TYPE_INT32, &res,
                                  DBUS_TYPE_INVALID))
        {
            ret = res;
        }

        dbus_message_unref (reply);
    }
    dbus_message_unref (message);
    return ret;
}

int CcspBaseIf_getParameterAttributes(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    char * parameterNames[],
    int size,
    int *val_size,
    parameterAttributeStruct_t ***parameterAttributeval

)
{
    DBusMessage *message;
    DBusMessage *reply;
    int ret = CCSP_FAILURE;
    dbus_int32_t res ;
    dbus_int32_t tmp ;
    DBusMessageIter iter;
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    DBusMessageIter array_iter;
    int i;

    parameterAttributeStruct_t **val = 0;
    *val_size = 0;
    message = dbus_message_new_method_call (dst_component_id,
                                            dbus_path,
                                            CCSP_DBUS_INTERFACE_BASE,
                                            "getParameterAttributes");
    if (!message )
    {
        CcspTraceError(("No memory\n"));
        return CCSP_ERR_MEMORY_ALLOC_FAIL;
    }

    dbus_message_iter_init_append (message, &iter);


    ret = dbus_message_iter_open_container (&iter,
                                            DBUS_TYPE_ARRAY,
                                            "s",
                                            &array_iter);
    for(i = 0; i < size; i++)
    {

        DBUS_MESSAGE_APPEND_STRING( &array_iter, parameterNames[i]);
    }

    ret = dbus_message_iter_close_container (&iter,
            &array_iter);

    tmp = size;
    dbus_message_iter_append_basic (&iter, DBUS_TYPE_INT32, &tmp);

    ret = CCSP_Message_Bus_Send_Msg(bus_handle, message, CcspBaseIf_timeout_seconds , &reply);
    if(reply )
    {
        DBusMessageIter iter;

        DBusMessageIter subiter;
        DBusMessageIter array_iter;

        dbus_message_iter_init (reply, &iter);
        DBUS_MESSAGE_ITER_RECURSE(&iter, &array_iter , DBUS_TYPE_ARRAY, 0, CCSP_FAILURE,bus_info->freefunc);
        do
        {
            int type = dbus_message_iter_get_arg_type (&array_iter);

            if (type == DBUS_TYPE_INVALID)
                break;
            *val_size = *val_size + 1;
        }
        while (dbus_message_iter_next (&array_iter));

        dbus_message_iter_init (reply, &iter);
        DBUS_MESSAGE_ITER_RECURSE(&iter, &array_iter , DBUS_TYPE_ARRAY, 0, CCSP_FAILURE,bus_info->freefunc);

        if(*val_size)
        {
            val = bus_info->mallocfunc(*val_size*sizeof(parameterAttributeStruct_t *));
            memset(val, 0, *val_size*sizeof(parameterAttributeStruct_t *));
            for(i = 0; i < *val_size; i++)
            {
                char * str = 0;
                dbus_int32_t tmp ;
                dbus_uint32_t utmp ;
                dbus_bool_t  btmp ;

                DBUS_MESSAGE_ITER_RECURSE(&array_iter, &subiter , DBUS_TYPE_STRUCT, val, CCSP_FAILURE, bus_info->freefunc);
                val[i] = bus_info->mallocfunc(sizeof(parameterAttributeStruct_t));
                memset(val[i], 0, sizeof(parameterAttributeStruct_t));
                if(dbus_message_iter_get_arg_type (&subiter) == DBUS_TYPE_STRING)
                {
                    dbus_message_iter_get_basic (&subiter, &str);
                    if(str)
                    {
                        val[i]->parameterName = bus_info->mallocfunc(strlen(str)+1);
                        strcpy( val[i]->parameterName, str);
                    }
                    else
                        val[i]->parameterName = NULL;
                }
                dbus_message_iter_next (&subiter);

                if(dbus_message_iter_get_arg_type (&subiter) == DBUS_TYPE_BOOLEAN)
                {
                    dbus_message_iter_get_basic (&subiter, &btmp);
                    val[i]->notificationChanged = btmp;
                }
                dbus_message_iter_next (&subiter);
                if(dbus_message_iter_get_arg_type (&subiter) == DBUS_TYPE_BOOLEAN)
                {
                    dbus_message_iter_get_basic (&subiter, &btmp);
                    val[i]->notification = btmp;

                }

                dbus_message_iter_next (&subiter);
                if(dbus_message_iter_get_arg_type (&subiter) == DBUS_TYPE_INT32)
                {
                    dbus_message_iter_get_basic (&subiter, &tmp);
                    val[i]->access = tmp;
                }

                dbus_message_iter_next (&subiter);
                if(dbus_message_iter_get_arg_type (&subiter) == DBUS_TYPE_BOOLEAN)
                {
                    dbus_message_iter_get_basic (&subiter, &btmp);
                    val[i]->accessControlChanged = btmp;
                }

                dbus_message_iter_next (&subiter);
                if(dbus_message_iter_get_arg_type (&subiter) == DBUS_TYPE_UINT32)
                {
                    dbus_message_iter_get_basic (&subiter, &utmp);
                    val[i]->accessControlBitmask = utmp;
                }


                dbus_message_iter_next	 (&array_iter);
            }
        }
        dbus_message_iter_next	 (&iter);
        if(dbus_message_iter_get_arg_type (&iter) == DBUS_TYPE_INT32)
        {
            dbus_message_iter_get_basic (&iter, &res);
            ret = res;
        }

        dbus_message_unref (reply);
    }
    dbus_message_unref (message);
    *parameterAttributeval = val;
    return ret;

}

void free_parameterAttributeStruct_t(void *bus_handle, int size, parameterAttributeStruct_t **val)
{
    int i;
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    if(size)
    {
        for(i = 0; i < size; i++)
        {
            if(val[i]->parameterName)  bus_info->freefunc(val[i]->parameterName);
            bus_info->freefunc(val[i]);
        }

        bus_info->freefunc(val);
    }
}

int CcspBaseIf_AddTblRow(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    int sessionId,
    char *objectName,
    int *instanceNumber
)
{
    DBusMessage *message;
    DBusMessage *reply;
    int ret = CCSP_FAILURE;
    dbus_int32_t tmp1 = sessionId;
    dbus_int32_t tmp2;
    dbus_uint32_t utmp ;

    message = dbus_message_new_method_call (dst_component_id,
                                            dbus_path,
                                            CCSP_DBUS_INTERFACE_BASE,
                                            "AddTblRow");
    if (!message )
    {
        CcspTraceError(("No memory\n"));
        return CCSP_ERR_MEMORY_ALLOC_FAIL;
    }


    dbus_message_append_args (message, DBUS_TYPE_INT32, &tmp1, DBUS_TYPE_STRING, &objectName, DBUS_TYPE_INVALID);

    ret = CCSP_Message_Bus_Send_Msg(bus_handle, message, CcspBaseIf_timeout_seconds , &reply);
    if(reply )
    {
        if(dbus_message_get_args (reply,
                                  NULL,
                                  DBUS_TYPE_UINT32, &utmp,
                                  DBUS_TYPE_INT32, &tmp2,
                                  DBUS_TYPE_INVALID))
        {
            *instanceNumber = utmp;
            ret = tmp2;
        }

        dbus_message_unref (reply);
    }
    dbus_message_unref (message);

    return ret;
}

int CcspBaseIf_DeleteTblRow(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    int sessionId,
    char * objectName
)
{
    DBusMessage *message;
    DBusMessage *reply;
    int ret = CCSP_FAILURE;
    dbus_int32_t tmp1 = sessionId;

    message = dbus_message_new_method_call (dst_component_id,
                                            dbus_path,
                                            CCSP_DBUS_INTERFACE_BASE,
                                            "DeleteTblRow");
    if (!message )
    {
        CcspTraceError(("No memory\n"));
        return CCSP_ERR_MEMORY_ALLOC_FAIL;
    }


    dbus_message_append_args (message, DBUS_TYPE_INT32, &tmp1, DBUS_TYPE_STRING, &objectName, DBUS_TYPE_INVALID);

    ret = CCSP_Message_Bus_Send_Msg(bus_handle, message, CcspBaseIf_timeout_seconds , &reply);
    if(reply )
    {
        if(dbus_message_get_args (reply,
                                  NULL,
                                  DBUS_TYPE_INT32, &tmp1,
                                  DBUS_TYPE_INVALID))
        {
            ret = tmp1;
        }


        dbus_message_unref (reply);
    }
    dbus_message_unref (message);

    return ret;
}

int CcspBaseIf_getParameterNames(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    char * parameterName,
    dbus_bool nextLevel,
    int *size ,
    parameterInfoStruct_t ***parameter
)
{
    DBusMessage *message;
    DBusMessage *reply;
    int ret = CCSP_FAILURE;
    dbus_int32_t res ;
    dbus_bool_t btmp ;
    DBusMessageIter iter;
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    parameterInfoStruct_t **val=NULL;

    *parameter = 0;
    *size = 0;
    message = dbus_message_new_method_call (dst_component_id,
                                            dbus_path,
                                            CCSP_DBUS_INTERFACE_BASE,
                                            "getParameterNames");
    if (!message )
    {
        CcspTraceError(("No memory\n"));
        return CCSP_ERR_MEMORY_ALLOC_FAIL;
    }

    dbus_message_iter_init_append (message, &iter);

    DBUS_MESSAGE_APPEND_STRING(&iter, parameterName);

    btmp = nextLevel;
    dbus_message_iter_append_basic (&iter, DBUS_TYPE_BOOLEAN, &btmp);

    ret = CCSP_Message_Bus_Send_Msg(bus_handle, message, CcspBaseIf_timeout_seconds , &reply);
    if(reply )
    {
        DBusMessageIter iter;
        DBusMessageIter subiter;
        DBusMessageIter array_iter;

        dbus_message_iter_init (reply, &iter);

        DBUS_MESSAGE_ITER_RECURSE(&iter, &array_iter , DBUS_TYPE_ARRAY, 0, CCSP_FAILURE,bus_info->freefunc);
        do
        {
            int type = dbus_message_iter_get_arg_type (&array_iter);

            if (type == DBUS_TYPE_INVALID)
                break;
            *size = *size + 1;
        }
        while (dbus_message_iter_next (&array_iter));

        dbus_message_iter_init (reply, &iter);
        DBUS_MESSAGE_ITER_RECURSE(&iter, &array_iter , DBUS_TYPE_ARRAY, 0, CCSP_FAILURE,bus_info->freefunc);

        if(*size)
        {
            int i;
            val = bus_info->mallocfunc(*size*sizeof(parameterInfoStruct_t *));
            memset(val, 0, *size*sizeof(parameterInfoStruct_t *));
            for(i = 0; i < *size; i++)
            {
                char * str = 0;

                DBUS_MESSAGE_ITER_RECURSE(&array_iter, &subiter , DBUS_TYPE_STRUCT, val, CCSP_FAILURE, bus_info->freefunc);
                val[i] = bus_info->mallocfunc(sizeof(parameterInfoStruct_t));
                memset(val[i], 0, sizeof(parameterInfoStruct_t));
                if(dbus_message_iter_get_arg_type (&subiter) == DBUS_TYPE_STRING)
                {
                    dbus_message_iter_get_basic (&subiter, &str);
                    if(str)
                    {
                        val[i]->parameterName = bus_info->mallocfunc(strlen(str)+1);
                        strcpy( val[i]->parameterName, str);
                    }
                    else
                        val[i]->parameterName = NULL;

                }


                dbus_message_iter_next (&subiter);

                if(dbus_message_iter_get_arg_type (&subiter) == DBUS_TYPE_BOOLEAN)
                {
                    dbus_message_iter_get_basic (&subiter, &btmp);
                    val[i]->writable = btmp;
                }

                dbus_message_iter_next	 (&array_iter);
            }
        }
        dbus_message_iter_next	 (&iter);
        if(dbus_message_iter_get_arg_type (&iter)  == DBUS_TYPE_INT32)
        {
            dbus_message_iter_get_basic (&iter, &res);
            ret = res;
        }

        dbus_message_unref (reply);
    }
    dbus_message_unref (message);
    *parameter = val;
    return ret;

}

void free_parameterInfoStruct_t (void *bus_handle, int size, parameterInfoStruct_t **val)
{
    int i;
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    if(size)
    {
        for(i = 0; i < size; i++)
        {
            if(val[i]->parameterName)  bus_info->freefunc(val[i]->parameterName);
            bus_info->freefunc(val[i]);
        }

        bus_info->freefunc(val);
    }
}


int CcspBaseIf_EnumRecords
(
    void*   bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    char* pParentPath,
    dbus_bool nextLevel,
    unsigned int * pulNumRec,
    PCCSP_BASE_RECORD*  ppRecArray
)
{
    parameterInfoStruct_t **parameter;
    int size;
    int i;
    PCCSP_BASE_RECORD  pRecArray = 0;
    int inst_num;
    char buf[CCSP_BASE_PARAM_LENGTH];
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;


    int ret = CcspBaseIf_getParameterNames(
                  bus_info,
                  dst_component_id,
                  dbus_path,
                  pParentPath,
                  nextLevel,
                  &size ,
                  &parameter
              );

    if( ret != CCSP_SUCCESS)
        return ret;

    if(size)
    {
        pRecArray = bus_info->mallocfunc(size*sizeof(CCSP_BASE_RECORD));
        memset(pRecArray, 0, size*sizeof(CCSP_BASE_RECORD));
        for(i = 0 ; i < size; i++)
        {
            if(nextLevel)
            {
                int type = CcspBaseIf_getObjType(pParentPath, parameter[i]->parameterName, &inst_num, buf);
	            if(type == CCSP_BASE_INSTANCE)
	            {
	                pRecArray[i].Instance.InstanceNumber = inst_num;
	                pRecArray[i].RecordType = CCSP_BASE_INSTANCE;
	            }
	            else if(type == CCSP_BASE_PARAM || type == CCSP_BASE_OBJECT)
	            {
	                strcpy(pRecArray[i].Instance.Name, buf);
	                pRecArray[i].RecordType = type;
	            }
            }else
            {
			    char *p;
	            if(strlen(parameter[i]->parameterName) > strlen(pParentPath))
	            {
			        p = parameter[i]->parameterName + strlen(pParentPath);
				    strncpy(pRecArray[i].Instance.Name, p, CCSP_BASE_PARAM_LENGTH-1);
	                pRecArray[i].RecordType = CCSP_BASE_PARAM;
                }
            }
               
        }
    }
    *ppRecArray = pRecArray;
    *pulNumRec  = size;

    free_parameterInfoStruct_t(bus_handle, size, parameter);
    return ret;

}

void free_CCSP_BASE_RECORD (void* bus_handle, PCCSP_BASE_RECORD pInstanceArray)
{
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    if(pInstanceArray) bus_info->freefunc(pInstanceArray);
}

int
CcspBaseIf_GetNextLevelInstances
(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    char* pObjectName,
    unsigned int*  pNums,
    unsigned int** pNumArray
)
{
    parameterInfoStruct_t **parameter;
    int size;
    unsigned int*val = 0;
    char buf[CCSP_BASE_PARAM_LENGTH];
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    int i;
    int inst_num;

    int ret = CcspBaseIf_getParameterNames(
                  bus_handle,
                  dst_component_id,
                  dbus_path,
                  pObjectName,
                  1,
                  &size ,
                  &parameter
              );

    if( ret != CCSP_SUCCESS)
        return ret;

    *pNums = 0;
    for(i = 0; i < size; i++)
    {
        if(CcspBaseIf_getObjType(pObjectName, parameter[i]->parameterName, &inst_num, buf) == CCSP_BASE_INSTANCE)
            *pNums= *pNums + 1;
    }
    if(*pNums)
        val = bus_info->mallocfunc(*pNums*sizeof(unsigned int));

    *pNums = 0;
    for(i = 0; i < size; i++)
    {
        if(CcspBaseIf_getObjType(pObjectName, parameter[i]->parameterName, &inst_num, buf) == CCSP_BASE_INSTANCE)
        {
            val[*pNums] = inst_num;
            *pNums = *pNums + 1;
        }
    }
    free_parameterInfoStruct_t(bus_handle, size, parameter);
    * pNumArray = val;
    return ret;
}


int CcspBaseIf_registerCapabilities(
    void* bus_handle,
    const char* dst_component_id,
    const char *component_name,
    int component_version,
    const char *dbus_path,
    const char *subsystem_prefix,
    name_spaceType_t * name_space,
    int size
)
{
    DBusMessage *message;
    DBusMessage *reply;
    int ret = CCSP_FAILURE;
    dbus_int32_t res ;
    dbus_int32_t tmp ;
    DBusMessageIter iter;
    DBusMessageIter array_iter;
    DBusMessageIter struct_iter;
    int i;

    message = dbus_message_new_method_call (dst_component_id,
                                            CCSP_DBUS_PATH_CR,
                                            CCSP_DBUS_INTERFACE_CR,
                                            "registerCapabilities");
    if (!message )
    {
        CcspTraceError(("No memory\n"));
        return CCSP_ERR_MEMORY_ALLOC_FAIL;
    }

    dbus_message_iter_init_append (message, &iter);
    DBUS_MESSAGE_APPEND_STRING (&iter, component_name);

    dbus_message_iter_append_basic (&iter, DBUS_TYPE_INT32,  &component_version);
    DBUS_MESSAGE_APPEND_STRING (&iter, dbus_path);
    DBUS_MESSAGE_APPEND_STRING (&iter, subsystem_prefix);


    ret = dbus_message_iter_open_container (&iter,
                                            DBUS_TYPE_ARRAY,
                                            "(si)",
                                            &array_iter);
    for(i = 0; i < size; i++)
    {
        dbus_message_iter_open_container (&array_iter,
                                          DBUS_TYPE_STRUCT,
                                          "si",
                                          &struct_iter);

        DBUS_MESSAGE_APPEND_STRING (&struct_iter, name_space[i].name_space);
        tmp = name_space[i].dataType;
        ret = dbus_message_iter_append_basic (&struct_iter, DBUS_TYPE_INT32, &tmp);

        dbus_message_iter_close_container (&array_iter,
                                           &struct_iter);
    }

    ret = dbus_message_iter_close_container (&iter,
            &array_iter);

    tmp = size;
    dbus_message_iter_append_basic (&iter, DBUS_TYPE_INT32, &tmp);

    ret = CCSP_Message_Bus_Send_Msg(bus_handle, message, CcspBaseIf_timeout_seconds , &reply);
    if(reply )
    {
        DBusMessageIter iter;
        dbus_message_iter_init (reply, &iter);

        if(dbus_message_iter_get_arg_type (&iter)  == DBUS_TYPE_INT32)
        {
            dbus_message_iter_get_basic (&iter, &res);
            ret = res;
        }

        dbus_message_unref (reply);
    }
    dbus_message_unref (message);
    return ret;

}

int CcspBaseIf_unregisterNamespace (
    void* bus_handle,
    const char* dst_component_id,
    const char *component_name,
    const char *name_space)
{
    DBusMessage *message;
    DBusMessage *reply;
    int ret = CCSP_FAILURE;
    dbus_int32_t res ;
    DBusMessageIter iter;

    message = dbus_message_new_method_call (dst_component_id,
                                            CCSP_DBUS_PATH_CR,
                                            CCSP_DBUS_INTERFACE_CR,
                                            "unregisterNamespace");
    if (!message )
    {
        CcspTraceError(("No memory\n"));
        return CCSP_ERR_MEMORY_ALLOC_FAIL;
    }

    dbus_message_iter_init_append (message, &iter);
    DBUS_MESSAGE_APPEND_STRING (&iter, component_name);
    DBUS_MESSAGE_APPEND_STRING (&iter, name_space);


    ret = CCSP_Message_Bus_Send_Msg(bus_handle, message, CcspBaseIf_timeout_seconds , &reply);
    if(reply )
    {
        DBusMessageIter iter;
        dbus_message_iter_init (reply, &iter);

        if(dbus_message_iter_get_arg_type (&iter)  == DBUS_TYPE_INT32)
        {
            dbus_message_iter_get_basic (&iter, &res);
            ret = res;
        }

        dbus_message_unref (reply);
    }
    dbus_message_unref (message);
    return ret;

}

int CcspBaseIf_unregisterComponent (
    void* bus_handle,
    const char* dst_component_id,
    const char *component_name
)
{
    DBusMessage *message;
    DBusMessage *reply;
    int ret = CCSP_FAILURE;
    dbus_int32_t res ;
    DBusMessageIter iter;

    message = dbus_message_new_method_call (dst_component_id,
                                            CCSP_DBUS_PATH_CR,
                                            CCSP_DBUS_INTERFACE_CR,
                                            "unregisterComponent");
    if (!message )
    {
        CcspTraceError(("No memory\n"));
        return CCSP_ERR_MEMORY_ALLOC_FAIL;
    }

    dbus_message_iter_init_append (message, &iter);
    DBUS_MESSAGE_APPEND_STRING (&iter, component_name);

    ret = CCSP_Message_Bus_Send_Msg(bus_handle, message, CcspBaseIf_timeout_seconds , &reply);
    if(reply )
    {
        DBusMessageIter iter;
        dbus_message_iter_init (reply, &iter);

        if(dbus_message_iter_get_arg_type (&iter) == DBUS_TYPE_INT32)
        {
            dbus_message_iter_get_basic (&iter, &res);
            ret = res;
        }

        dbus_message_unref (reply);
    }
    dbus_message_unref (message);
    return ret;

}

int CcspBaseIf_discComponentSupportingNamespace (
    void* bus_handle,
    const char* dst_component_id,
    const char *name_space,
    const char *subsystem_prefix,
    componentStruct_t ***components,
    int *size)
{
    DBusMessage *message;
    DBusMessage *reply;
    int ret = CCSP_FAILURE;
    dbus_int32_t res ;
    DBusMessageIter iter;
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;

    componentStruct_t **val=NULL;
    *components = 0;
    *size = 0;
    message = dbus_message_new_method_call (dst_component_id,
                                            CCSP_DBUS_PATH_CR,
                                            CCSP_DBUS_INTERFACE_CR,
                                            "discComponentSupportingNamespace");
    if (!message )
    {
        CcspTraceError(("No memory\n"));
        return CCSP_ERR_MEMORY_ALLOC_FAIL;
    }

    dbus_message_iter_init_append (message, &iter);
    DBUS_MESSAGE_APPEND_STRING (&iter, name_space);
    DBUS_MESSAGE_APPEND_STRING (&iter, subsystem_prefix);

    ret = CCSP_Message_Bus_Send_Msg(bus_handle, message, CcspBaseIf_timeout_seconds , &reply);
    if(reply )
    {
        DBusMessageIter iter;
        DBusMessageIter array_iter;
        DBusMessageIter struct_iter;

        dbus_message_iter_init (reply, &iter);


        DBUS_MESSAGE_ITER_RECURSE(&iter, &array_iter , DBUS_TYPE_ARRAY, 0, CCSP_FAILURE,bus_info->freefunc);
        do
        {
            int type = dbus_message_iter_get_arg_type (&array_iter);

            if (type == DBUS_TYPE_INVALID)
                break;
            *size = *size + 1;
        }
        while (dbus_message_iter_next (&array_iter));

        dbus_message_iter_init (reply, &iter);
        DBUS_MESSAGE_ITER_RECURSE(&iter, &array_iter , DBUS_TYPE_ARRAY, 0, CCSP_FAILURE,bus_info->freefunc);

        if(*size)
        {
            int i;
            val = bus_info->mallocfunc(*size*sizeof(componentStruct_t *));
            memset(val, 0, *size*sizeof(componentStruct_t *));
            for(i = 0; i < *size; i++)
            {
                char * str = 0;
                DBUS_MESSAGE_ITER_RECURSE(&array_iter, &struct_iter, DBUS_TYPE_STRUCT, val, CCSP_FAILURE, bus_info->freefunc);

                val[i] = bus_info->mallocfunc(sizeof(componentStruct_t));
                if(dbus_message_iter_get_arg_type (&struct_iter) == DBUS_TYPE_STRING)
                {
                    dbus_message_iter_get_basic (&struct_iter, &str);
                    if(str)
                    {
                        val[i]->componentName = bus_info->mallocfunc(strlen(str)+1);
                        strcpy( val[i]->componentName, str);
                    }
                    else
                        val[i]->componentName = NULL;

                }
                dbus_message_iter_next	 (&struct_iter);

                if(dbus_message_iter_get_arg_type (&struct_iter) == DBUS_TYPE_STRING)
                {
                    dbus_message_iter_get_basic (&struct_iter, &str);
                    if(str)
                    {
                        val[i]->dbusPath = bus_info->mallocfunc(strlen(str)+1);
                        strcpy( val[i]->dbusPath, str);
                    }
                    else
                        val[i]->dbusPath = NULL;

                }

                dbus_message_iter_next	 (&struct_iter);
                if(dbus_message_iter_get_arg_type (&struct_iter) == DBUS_TYPE_INT32)
                {
                    dbus_message_iter_get_basic (&struct_iter, &res);
                    val[i]->type = res;
                }
                
                dbus_message_iter_next	 (&struct_iter);
                if(dbus_message_iter_get_arg_type (&struct_iter) == DBUS_TYPE_STRING)
                {
                    dbus_message_iter_get_basic (&struct_iter, &str);
                    if(str)
                    {
                        val[i]->remoteCR_name = bus_info->mallocfunc(strlen(str)+1);
                        strcpy( val[i]->remoteCR_name, str);
                    }
                    else
                        val[i]->remoteCR_name = NULL;

                }

                dbus_message_iter_next	 (&struct_iter);
                if(dbus_message_iter_get_arg_type (&struct_iter) == DBUS_TYPE_STRING)
                {
                    dbus_message_iter_get_basic (&struct_iter, &str);
                    if(str)
                    {
                        val[i]->remoteCR_dbus_path = bus_info->mallocfunc(strlen(str)+1);
                        strcpy( val[i]->remoteCR_dbus_path, str);
                    }
                    else
                        val[i]->remoteCR_dbus_path = NULL;

                }

                dbus_message_iter_next	 (&array_iter);
            }
        }
        dbus_message_iter_next	 (&iter); // pass size
        dbus_message_iter_next	 (&iter);
        if(dbus_message_iter_get_arg_type (&iter) == DBUS_TYPE_INT32)
        {
            dbus_message_iter_get_basic (&iter, &res);
            ret = res;
        }

        dbus_message_unref (reply);
    }
    dbus_message_unref (message);
    *components = val;
    return ret;

}

void free_char_t (void *bus_handle, int size, char **val)
{
    int i;
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    if(size)
    {
        for(i = 0; i < size; i++)
        {
            if(val[i])  bus_info->freefunc(val[i]);
        }

        bus_info->freefunc(val);
    }
}

int CcspBaseIf_discComponentSupportingDynamicTbl (
    void* bus_handle,
    const char* dst_component_id,
    const char *name_space,
    const char *subsystem_prefix,
    componentStruct_t **component
)
{
    DBusMessage *message;
    DBusMessage *reply;
    int ret = CCSP_FAILURE;
    dbus_int32_t res ;
    DBusMessageIter iter;
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;

    componentStruct_t *val=NULL;
    *component = 0;
    message = dbus_message_new_method_call (dst_component_id,
                                            CCSP_DBUS_PATH_CR,
                                            CCSP_DBUS_INTERFACE_CR,
                                            "discComponentSupportingDynamicTbl");
    if (!message )
    {
        CcspTraceError(("No memory\n"));
        return CCSP_ERR_MEMORY_ALLOC_FAIL;
    }

    dbus_message_iter_init_append (message, &iter);
    DBUS_MESSAGE_APPEND_STRING (&iter, name_space);
    DBUS_MESSAGE_APPEND_STRING (&iter, subsystem_prefix);

    ret = CCSP_Message_Bus_Send_Msg(bus_handle, message, CcspBaseIf_timeout_seconds , &reply);
    if(reply )
    {
        DBusMessageIter iter;
        DBusMessageIter struct_iter;
        char * str = 0;
        dbus_message_iter_init (reply, &iter);

        val = bus_info->mallocfunc(sizeof(componentStruct_t));
        DBUS_MESSAGE_ITER_RECURSE(&iter, &struct_iter, DBUS_TYPE_STRUCT, val, CCSP_FAILURE, bus_info->freefunc);
        if(dbus_message_iter_get_arg_type (&struct_iter) == DBUS_TYPE_STRING)
        {
            dbus_message_iter_get_basic (&struct_iter, &str);
            if(str)
            {
                val->componentName = bus_info->mallocfunc(strlen(str)+1);
                strcpy( val->componentName, str);
            }
            else
                val->componentName = NULL;

        }
        dbus_message_iter_next	 (&struct_iter);

        if(dbus_message_iter_get_arg_type (&struct_iter) == DBUS_TYPE_STRING)
        {
            dbus_message_iter_get_basic (&struct_iter, &str);
            if(str)
            {
                val->dbusPath = bus_info->mallocfunc(strlen(str)+1);
                strcpy( val->dbusPath, str);
            }
            else
                val->dbusPath = NULL;

        }
        
        dbus_message_iter_next	 (&struct_iter);
        if(dbus_message_iter_get_arg_type (&struct_iter) == DBUS_TYPE_INT32)
        {
            dbus_message_iter_get_basic (&struct_iter, &res);
            val->type = res;
        }
        
        dbus_message_iter_next	 (&struct_iter);
        if(dbus_message_iter_get_arg_type (&struct_iter) == DBUS_TYPE_STRING)
        {
            dbus_message_iter_get_basic (&struct_iter, &str);
            if(str)
            {
                val->remoteCR_name = bus_info->mallocfunc(strlen(str)+1);
                strcpy( val->remoteCR_name, str);
            }
            else
                val->remoteCR_name = NULL;

        }

        dbus_message_iter_next	 (&struct_iter);
        if(dbus_message_iter_get_arg_type (&struct_iter) == DBUS_TYPE_STRING)
        {
            dbus_message_iter_get_basic (&struct_iter, &str);
            if(str)
            {
                val->remoteCR_dbus_path = bus_info->mallocfunc(strlen(str)+1);
                strcpy( val->remoteCR_dbus_path, str);
            }
            else
                val->remoteCR_dbus_path = NULL;

        }


        dbus_message_iter_next	 (&iter);
        if(dbus_message_iter_get_arg_type (&iter) == DBUS_TYPE_INT32)
        {
            dbus_message_iter_get_basic (&iter, &res);
            ret = res;
        }

        dbus_message_unref (reply);
    }
    dbus_message_unref (message);
    *component = val;
    return ret;

}


int CcspBaseIf_discNamespaceSupportedByComponent (
    void* bus_handle,
    const char* dst_component_id,
    const char *component_name,
    name_spaceType_t ***name_space,
    int *size
)
{
    DBusMessage *message;
    DBusMessage *reply;
    int ret = CCSP_FAILURE;
    dbus_int32_t res ;
    DBusMessageIter iter;
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;

    name_spaceType_t **val=NULL;
    *name_space = 0;
    *size = 0;
    message = dbus_message_new_method_call (dst_component_id,
                                            CCSP_DBUS_PATH_CR,
                                            CCSP_DBUS_INTERFACE_CR,
                                            "discNamespaceSupportedByComponent");
    if (!message )
    {
        CcspTraceError(("No memory\n"));
        return CCSP_ERR_MEMORY_ALLOC_FAIL;
    }

    dbus_message_iter_init_append (message, &iter);
    DBUS_MESSAGE_APPEND_STRING (&iter, component_name);

    ret = CCSP_Message_Bus_Send_Msg(bus_handle, message, CcspBaseIf_timeout_seconds , &reply);
    if(reply )
    {
        DBusMessageIter iter;
        DBusMessageIter array_iter;
        DBusMessageIter struct_iter;
        dbus_message_iter_init (reply, &iter);


        DBUS_MESSAGE_ITER_RECURSE(&iter, &array_iter , DBUS_TYPE_ARRAY, 0, CCSP_FAILURE,bus_info->freefunc);
        do
        {
            int type = dbus_message_iter_get_arg_type (&array_iter);

            if (type == DBUS_TYPE_INVALID)
                break;
            *size = *size + 1;
        }
        while (dbus_message_iter_next (&array_iter));

        dbus_message_iter_init (reply, &iter);
        DBUS_MESSAGE_ITER_RECURSE(&iter, &array_iter , DBUS_TYPE_ARRAY, 0, CCSP_FAILURE,bus_info->freefunc);

        if(*size)
        {
            int i;
            val = bus_info->mallocfunc(*size*sizeof(name_spaceType_t *));
            memset(val, 0, *size*sizeof(name_spaceType_t *));
            for(i = 0; i < *size; i++)
            {
                char * str = 0;
	            val[i] = bus_info->mallocfunc(sizeof(name_spaceType_t ));

                DBUS_MESSAGE_ITER_RECURSE(&array_iter, &struct_iter, DBUS_TYPE_STRUCT, val, CCSP_FAILURE, bus_info->freefunc);
                if(dbus_message_iter_get_arg_type (&struct_iter) == DBUS_TYPE_STRING)
                {
                    dbus_message_iter_get_basic (&struct_iter, &str);
                    if(str)
                    {
                        val[i]->name_space = bus_info->mallocfunc(strlen(str)+1);
                        strcpy( val[i]->name_space, str);
                    }
                    else
                        val[i]->name_space = NULL;

                }
                
		        dbus_message_iter_next	 (&struct_iter);
		        if(dbus_message_iter_get_arg_type (&struct_iter) == DBUS_TYPE_INT32)
		        {
		            dbus_message_iter_get_basic (&struct_iter, &res);
		            val[i]->dataType = res;
		        }

                dbus_message_iter_next	 (&array_iter);
            }
        }
        dbus_message_iter_next	 (&iter); // pass size
        dbus_message_iter_next	 (&iter);
        if(dbus_message_iter_get_arg_type (&iter) == DBUS_TYPE_INT32)
        {
            dbus_message_iter_get_basic (&iter, &res);
            ret = res;
        }

        dbus_message_unref (reply);
    }
    dbus_message_unref (message);
    *name_space = val;
    return ret;

}

void free_name_spaceType_t (void* bus_handle, int size, name_spaceType_t **val)
{
	int i;
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    if(size)
    {
        for(i = 0; i < size; i++)
        {
            if(val[i]->name_space)  bus_info->freefunc(val[i]->name_space);
            bus_info->freefunc(val[i]);
        }

        bus_info->freefunc(val);
    }
}

int CcspBaseIf_getRegisteredComponents (
    void* bus_handle,
    const char* dst_component_id,
    registeredComponent_t ***components,
    int *size
)
{
    DBusMessage *message;
    DBusMessage *reply;
    int ret = CCSP_FAILURE;
    dbus_int32_t res ;
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;

    registeredComponent_t **val=NULL;
    *components = 0;
    *size = 0;
    message = dbus_message_new_method_call (dst_component_id,
                                            CCSP_DBUS_PATH_CR,
                                            CCSP_DBUS_INTERFACE_CR,
                                            "getRegisteredComponents");
    if (!message )
    {
        CcspTraceError(("No memory\n"));
        return CCSP_ERR_MEMORY_ALLOC_FAIL;
    }

    ret = CCSP_Message_Bus_Send_Msg(bus_handle, message, CcspBaseIf_timeout_seconds , &reply);
    if(reply )
    {
        DBusMessageIter iter;

        DBusMessageIter array_iter;
        DBusMessageIter struct_iter;
        dbus_message_iter_init (reply, &iter);

        DBUS_MESSAGE_ITER_RECURSE(&iter, &array_iter , DBUS_TYPE_ARRAY, 0, CCSP_FAILURE,bus_info->freefunc);
        do
        {
            int type = dbus_message_iter_get_arg_type (&array_iter);

            if (type == DBUS_TYPE_INVALID)
                break;
            *size = *size + 1;
        }
        while (dbus_message_iter_next (&array_iter));

        dbus_message_iter_init (reply, &iter);
        DBUS_MESSAGE_ITER_RECURSE(&iter, &array_iter , DBUS_TYPE_ARRAY, 0, CCSP_FAILURE,bus_info->freefunc);

        if(*size)
        {
            int i;
            val = bus_info->mallocfunc(*size*sizeof(registeredComponent_t *));
            memset(val, 0, *size*sizeof(registeredComponent_t *));
            for(i = 0; i < *size; i++)
            {
                char * str = 0;
                DBUS_MESSAGE_ITER_RECURSE(&array_iter, &struct_iter, DBUS_TYPE_STRUCT, val, CCSP_FAILURE, bus_info->freefunc);

                val[i] = bus_info->mallocfunc(sizeof(componentStruct_t));
                if(dbus_message_iter_get_arg_type (&struct_iter) == DBUS_TYPE_STRING)
                {
                    dbus_message_iter_get_basic (&struct_iter, &str);
                    if(str)
                    {
                        val[i]->componentName = bus_info->mallocfunc(strlen(str)+1);
                        strcpy( val[i]->componentName, str);
                    }
                    else
                        val[i]->componentName = NULL;

                }
                dbus_message_iter_next	 (&struct_iter);

                if(dbus_message_iter_get_arg_type (&struct_iter) == DBUS_TYPE_STRING)
                {
                    dbus_message_iter_get_basic (&struct_iter, &str);
                    if(str)
                    {
                        val[i]->dbusPath = bus_info->mallocfunc(strlen(str)+1);
                        strcpy( val[i]->dbusPath, str);
                    }
                    else
                        val[i]->dbusPath = NULL;

                }

                dbus_message_iter_next	 (&struct_iter);

                if(dbus_message_iter_get_arg_type (&struct_iter) == DBUS_TYPE_STRING)
                {
                    dbus_message_iter_get_basic (&struct_iter, &str);
                    if(str)
                    {
                        val[i]->subsystem_prefix = bus_info->mallocfunc(strlen(str)+1);
                        strcpy( val[i]->subsystem_prefix, str);
                    }
                    else
                        val[i]->subsystem_prefix = NULL;

                }
                dbus_message_iter_next	 (&array_iter);
            }
        }
        dbus_message_iter_next	 (&iter); // pass size
        dbus_message_iter_next	 (&iter);
        if(dbus_message_iter_get_arg_type (&iter) == DBUS_TYPE_INT32)
        {
            dbus_message_iter_get_basic (&iter, &res);
            ret = res;
        }

        dbus_message_unref (reply);
    }
    dbus_message_unref (message);

    *components = val;
    return ret;

}

void free_componentStruct_t (void *bus_handle, int size, componentStruct_t **val)
{
    int i;
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    if(size)
    {
        for(i = 0; i < size; i++)
        {
            if(val[i]->componentName)  bus_info->freefunc(val[i]->componentName);
            if(val[i]->dbusPath)  bus_info->freefunc(val[i]->dbusPath);
            if(val[i]->remoteCR_name)  bus_info->freefunc(val[i]->remoteCR_name);
            if(val[i]->remoteCR_dbus_path)  bus_info->freefunc(val[i]->remoteCR_dbus_path);
            bus_info->freefunc(val[i]);
        }

        bus_info->freefunc(val);
    }
}

void free_registeredComponent_t (void *bus_handle, int size, registeredComponent_t **val)
{
    int i;
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    if(size)
    {
        for(i = 0; i < size; i++)
        {
            if(val[i]->componentName)  bus_info->freefunc(val[i]->componentName);
            if(val[i]->dbusPath)  bus_info->freefunc(val[i]->dbusPath);
            if(val[i]->subsystem_prefix)  bus_info->freefunc(val[i]->subsystem_prefix);
            bus_info->freefunc(val[i]);
        }

        bus_info->freefunc(val);
    }
}


void free_componentStruct_t2 (void *bus_handle, componentStruct_t *val)
{
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    if(val)
    {
        if(val->componentName)  bus_info->freefunc(val->componentName);
        if(val->dbusPath)  bus_info->freefunc(val->dbusPath);
        if(val->remoteCR_name)  bus_info->freefunc(val->remoteCR_name);
        if(val->remoteCR_dbus_path)  bus_info->freefunc(val->remoteCR_dbus_path);

        bus_info->freefunc(val);
    }
}

int CcspBaseIf_checkNamespaceDataType (
    void* bus_handle,
    const char* dst_component_id,
    name_spaceType_t * name_space,
    const char *subsystem_prefix,
    dbus_bool *typeMatch)
{
    DBusMessage *message;
    DBusMessage *reply;
    DBusMessageIter struct_iter;
    int ret = CCSP_FAILURE;
    dbus_int32_t res ;
    dbus_int32_t tmp ;
    DBusMessageIter iter;
    dbus_bool_t btmp ;

    message = dbus_message_new_method_call (dst_component_id,
                                            CCSP_DBUS_PATH_CR,
                                            CCSP_DBUS_INTERFACE_CR,
                                            "checkNamespaceDataType");
    if (!message )
    {
        CcspTraceError(("No memory\n"));
        return CCSP_ERR_MEMORY_ALLOC_FAIL;
    }

    dbus_message_iter_init_append (message, &iter);
    dbus_message_iter_open_container (&iter,
                                      DBUS_TYPE_STRUCT,
                                      "si",
                                      &struct_iter);

    DBUS_MESSAGE_APPEND_STRING (&struct_iter, name_space->name_space);
    tmp = name_space->dataType;
    dbus_message_iter_append_basic (&struct_iter, DBUS_TYPE_INT32, &tmp);
    dbus_message_iter_close_container (&iter,
                                       &struct_iter);
    
    DBUS_MESSAGE_APPEND_STRING (&iter, subsystem_prefix);

    ret = CCSP_Message_Bus_Send_Msg(bus_handle, message, CcspBaseIf_timeout_seconds , &reply);
    if(reply )
    {
        DBusMessageIter iter;
        dbus_message_iter_init (reply, &iter);

        if(dbus_message_iter_get_arg_type (&iter) == DBUS_TYPE_BOOLEAN)
        {
            dbus_message_iter_get_basic (&iter, &btmp);
            *typeMatch = btmp;

        }

        dbus_message_iter_next	 (&iter);

        if(dbus_message_iter_get_arg_type (&iter) == DBUS_TYPE_INT32)
        {
            dbus_message_iter_get_basic (&iter, &res);
            ret = res;
        }

        dbus_message_unref (reply);
    }
    dbus_message_unref (message);
    return ret;

}

int CcspBaseIf_dumpComponentRegistry (
    void* bus_handle,
    const char* dst_component_id
)
{
    DBusMessage *message;
    DBusMessage *reply;
    int ret = CCSP_FAILURE;
    dbus_int32_t res ;

    message = dbus_message_new_method_call (dst_component_id,
                                            CCSP_DBUS_PATH_CR,
                                            CCSP_DBUS_INTERFACE_CR,
                                            "dumpComponentRegistry");
    if (!message )
    {
        CcspTraceError(("No memory\n"));
        return CCSP_ERR_MEMORY_ALLOC_FAIL;
    }


    ret = CCSP_Message_Bus_Send_Msg(bus_handle, message, CcspBaseIf_timeout_seconds , &reply);
    if(reply )
    {
        DBusMessageIter iter;
        dbus_message_iter_init (reply, &iter);

        if(dbus_message_iter_get_arg_type (&iter) == DBUS_TYPE_INT32)
        {
            dbus_message_iter_get_basic (&iter, &res);
            ret = res;
        }

        dbus_message_unref (reply);
    }
    dbus_message_unref (message);
    return ret;

}

int CcspBaseIf_isSystemReady (
    void* bus_handle,
    const char* dst_component_id,
    dbus_bool *val
)
{
    DBusMessage *message;
    DBusMessage *reply;
    int ret = CCSP_FAILURE;
    dbus_int32_t res ;
    dbus_bool_t btmp ;

    message = dbus_message_new_method_call (dst_component_id,
                                            CCSP_DBUS_PATH_CR,
                                            CCSP_DBUS_INTERFACE_CR,
                                            "isSystemReady");
    if (!message )
    {
        CcspTraceError(("No memory\n"));
        return CCSP_ERR_MEMORY_ALLOC_FAIL;
    }


    ret = CCSP_Message_Bus_Send_Msg(bus_handle, message, CcspBaseIf_timeout_seconds , &reply);
    if(reply )
    {
        DBusMessageIter iter;
        dbus_message_iter_init (reply, &iter);

        if(dbus_message_iter_get_arg_type (&iter) == DBUS_TYPE_BOOLEAN)
        {
            dbus_message_iter_get_basic (&iter, &btmp);
            *val = btmp;

        }
        dbus_message_iter_next	 (&iter);
        if(dbus_message_iter_get_arg_type (&iter) == DBUS_TYPE_INT32)
        {
            dbus_message_iter_get_basic (&iter, &res);
            ret = res;
        }

        dbus_message_unref (reply);
    }
    dbus_message_unref (message);
    return ret;
}

int CcspBaseIf_requestSessionID (
    void* bus_handle,
    const char* dst_component_id,
    int priority,
    int *sessionID
)
{
    DBusMessage *message;
    DBusMessage *reply;
    int ret = CCSP_FAILURE;
    dbus_int32_t res ;
    dbus_int32_t tmp ;

    message = dbus_message_new_method_call (dst_component_id,
                                            CCSP_DBUS_PATH_CR,
                                            CCSP_DBUS_INTERFACE_CR,
                                            "requestSessionID");
    if (!message )
    {
        CcspTraceError(("No memory\n"));
        return CCSP_ERR_MEMORY_ALLOC_FAIL;
    }

    tmp = priority;
    dbus_message_append_args (message, DBUS_TYPE_INT32, &tmp, DBUS_TYPE_INVALID);

    ret = CCSP_Message_Bus_Send_Msg(bus_handle, message, CcspBaseIf_timeout_seconds , &reply);
    if(reply )
    {
        DBusMessageIter iter;
        dbus_message_iter_init (reply, &iter);

        if(dbus_message_iter_get_arg_type (&iter) == DBUS_TYPE_INT32)
        {
            dbus_message_iter_get_basic (&iter, &tmp);
            *sessionID = tmp;
        }

        dbus_message_iter_next(&iter);
        if(dbus_message_iter_get_arg_type (&iter) == DBUS_TYPE_INT32)
        {
            dbus_message_iter_get_basic (&iter, &res);
            ret = res;
        }

        dbus_message_unref (reply);
    }
    dbus_message_unref (message);
    return ret;
}


int CcspBaseIf_getCurrentSessionID (
    void* bus_handle,
    const char* dst_component_id,
    int *priority,
    int *sessionID
)
{
    DBusMessage *message;
    DBusMessage *reply;
    int ret = CCSP_FAILURE;
    dbus_int32_t res ;
    dbus_int32_t tmp ;

    message = dbus_message_new_method_call (dst_component_id,
                                            CCSP_DBUS_PATH_CR,
                                            CCSP_DBUS_INTERFACE_CR,
                                            "getCurrentSessionID");
    if (!message )
    {
        CcspTraceError(("No memory\n"));
        return CCSP_ERR_MEMORY_ALLOC_FAIL;
    }

    ret = CCSP_Message_Bus_Send_Msg(bus_handle, message, CcspBaseIf_timeout_seconds , &reply);
    if(reply )
    {
        DBusMessageIter iter;
        dbus_message_iter_init (reply, &iter);

        if(dbus_message_iter_get_arg_type (&iter) == DBUS_TYPE_INT32)
        {
            dbus_message_iter_get_basic (&iter, &tmp);
            *sessionID = tmp;
        }

        dbus_message_iter_next(&iter);
        if(dbus_message_iter_get_arg_type (&iter) == DBUS_TYPE_INT32)
        {
            dbus_message_iter_get_basic (&iter, &tmp);
            *priority = tmp;
        }
        dbus_message_iter_next(&iter);
        if(dbus_message_iter_get_arg_type (&iter) == DBUS_TYPE_INT32)
        {
            dbus_message_iter_get_basic (&iter, &res);
            ret = res;
        }

        dbus_message_unref (reply);
    }
    dbus_message_unref (message);
    return ret;
}

int CcspBaseIf_informEndOfSession (
    void* bus_handle,
    const char* dst_component_id,
    int sessionID
)
{
    DBusMessage *message;
    DBusMessage *reply;
    int ret = CCSP_FAILURE;
    dbus_int32_t res ;
    dbus_int32_t tmp ;

    message = dbus_message_new_method_call (dst_component_id,
                                            CCSP_DBUS_PATH_CR,
                                            CCSP_DBUS_INTERFACE_CR,
                                            "informEndOfSession");
    if (!message )
    {
        CcspTraceError(("No memory\n"));
        return CCSP_ERR_MEMORY_ALLOC_FAIL;
    }

    tmp = sessionID;
    dbus_message_append_args (message, DBUS_TYPE_INT32, &tmp, DBUS_TYPE_INVALID);
    ret = CCSP_Message_Bus_Send_Msg(bus_handle, message, CcspBaseIf_timeout_seconds , &reply);
    if(reply )
    {
        DBusMessageIter iter;
        dbus_message_iter_init (reply, &iter);

        if(dbus_message_iter_get_arg_type (&iter) == DBUS_TYPE_INT32)
        {
            dbus_message_iter_get_basic (&iter, &res);
            ret = res;
        }

        dbus_message_unref (reply);
    }
    dbus_message_unref (message);
    return ret;
}

int CcspBaseIf_getHealth(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    int *health)
{
    DBusMessage *message= NULL;
    DBusMessage *reply = NULL;
    int ret = CCSP_FAILURE;
    dbus_int32_t res ;
    dbus_int32_t status;
    DBusMessageIter iter;

    message = dbus_message_new_method_call (dst_component_id,
                                            dbus_path,
                                            CCSP_DBUS_INTERFACE_BASE,
                                            "getHealth");
    if (!message )
    {
        printf ("No memory\n");
        return CCSP_ERR_MEMORY_ALLOC_FAIL;
    }
    ret = CCSP_Message_Bus_Send_Msg(bus_handle, message, CcspBaseIf_timeout_seconds , &reply);
    if(reply && CCSP_Message_Bus_OK == ret)
    {
        dbus_message_iter_init (reply, &iter);
        if(dbus_message_iter_get_arg_type (&iter) == DBUS_TYPE_INT32)
        {
            dbus_message_iter_get_basic (&iter, &status);
            *health = status;
            dbus_message_unref (reply);
            return CCSP_SUCCESS;
        }
    }
    return CCSP_FAILURE;
}

BOOLEAN waitConditionReady(void* hMBusHandle, const char* dst_component_id, char* dbus_path, char *src_component_id){
    #define MAX_WAIT_TIME 10
    #define TIME_INTERVAL 1000 // 1s
    #define CCSP_COMMON_COMPONENT_HEALTH_Green 3
    int times = 0;
    int ret   = 0;
    int health = 0;
    printf("%s start to check %s status\n", src_component_id, dst_component_id);
    while(times++ < MAX_WAIT_TIME)
    {
        //to call TAD dbus message to check TAD is ready
        ret = CcspBaseIf_getHealth(hMBusHandle, dst_component_id, dbus_path, &health);
	    if(health != CCSP_COMMON_COMPONENT_HEALTH_Green || CCSP_SUCCESS != ret)//CCSP_COMMON_COMPONENT_HEALTH_Green
        {
            CCSP_Msg_SleepInMilliSeconds(TIME_INTERVAL);
            printf("%s try to check %s health for %d times, health is %d\n", src_component_id, dst_component_id, times, health);
        }
        else
        {
            printf("%s is ready, %s continue\n",dst_component_id, src_component_id);
            return true;
        }
    }
    printf("Time is out! %s is not ready, health status is %d, but %s continue\n", dst_component_id, health, src_component_id);
    return false;
}

int CcspBaseIf_busCheck(
    void* bus_handle,
    const char* dst_component_id
)
{
    DBusMessage *message;
    DBusMessage *reply;
    int ret = CCSP_FAILURE;
    dbus_int32_t res ;

    message = dbus_message_new_method_call (dst_component_id,
                                            CCSP_DBUS_PATH_CR,
                                            CCSP_DBUS_INTERFACE_CR,
                                            "busCheck");
    if (!message )
    {
        CcspTraceError(("No memory\n"));
        return CCSP_ERR_MEMORY_ALLOC_FAIL;
    }

    ret = CCSP_Message_Bus_Send_Msg(bus_handle, message, CcspBaseIf_timeout_seconds , &reply);
    if(reply )
    {
        DBusMessageIter iter;
        dbus_message_iter_init (reply, &iter);

        if(dbus_message_iter_get_arg_type (&iter) == DBUS_TYPE_INT32)
        {
            dbus_message_iter_get_basic (&iter, &res);
            ret = res;
        }

        dbus_message_unref (reply);
    }
    dbus_message_unref (message);
    return ret;
}

int CcspBaseIf_finalize(
    void* bus_handle,
    const char* dst_component_id
)
{
    DBusMessage *message;
    DBusMessage *reply;
    int ret = CCSP_FAILURE;
    dbus_int32_t res ;

    message = dbus_message_new_method_call (dst_component_id,
                                            CCSP_DBUS_PATH_CR,
                                            CCSP_DBUS_INTERFACE_CR,
                                            "finalize");
    if (!message )
    {
        CcspTraceError(("No memory\n"));
        return CCSP_ERR_MEMORY_ALLOC_FAIL;
    }

    ret = CCSP_Message_Bus_Send_Msg(bus_handle, message, CcspBaseIf_timeout_seconds , &reply);
    if(reply )
    {
        DBusMessageIter iter;
        dbus_message_iter_init (reply, &iter);

        if(dbus_message_iter_get_arg_type (&iter) == DBUS_TYPE_INT32)
        {
            dbus_message_iter_get_basic (&iter, &res);
            ret = res;
        }

        dbus_message_unref (reply);
    }
    dbus_message_unref (message);
    return ret;
}

int CcspBaseIf_initialize(
    void* bus_handle,
    const char* dst_component_id
)
{
    DBusMessage *message;
    DBusMessage *reply;
    int ret = CCSP_FAILURE;
    dbus_int32_t res ;

    message = dbus_message_new_method_call (dst_component_id,
                                            CCSP_DBUS_PATH_CR,
                                            CCSP_DBUS_INTERFACE_CR,
                                            "initialize");
    if (!message )
    {
        CcspTraceError(("No memory\n"));
        return CCSP_ERR_MEMORY_ALLOC_FAIL;
    }

    ret = CCSP_Message_Bus_Send_Msg(bus_handle, message, CcspBaseIf_timeout_seconds , &reply);
    if(reply )
    {
        DBusMessageIter iter;
        dbus_message_iter_init (reply, &iter);

        if(dbus_message_iter_get_arg_type (&iter) == DBUS_TYPE_INT32)
        {
            dbus_message_iter_get_basic (&iter, &res);
            ret = res;
        }

        dbus_message_unref (reply);
    }
    dbus_message_unref (message);
    return ret;
}



int CcspBaseIf_getObjType(char * parent_name, char * name, int *inst_num, char * buf)
{
    int tmp;
    char *p;
    int len;

    if(!name)
        return CCSP_CR_ERR_INVALID_PARAM;
    len = strlen(name);
    if(len == 0 || len > CCSP_BASE_PARAM_LENGTH)
        return CCSP_CR_ERR_INVALID_PARAM;

    if(name[len-1] != '.')
    {
        p = name;

        if ( len > strlen(parent_name) )
        {
            p = name + strlen(parent_name);
        }

        strncpy(buf, p, CCSP_BASE_PARAM_LENGTH-1);
        return CCSP_BASE_PARAM;
    }

    if(len <= 1 )
        return CCSP_CR_ERR_INVALID_PARAM;
    tmp = len - 1;
    while(tmp && (name[tmp-1] != '.') )
    {
        if(name[tmp-1] > '9' || 	name[tmp-1] < '0')
        {
            p = name;

            if ( len > strlen(parent_name) )
            {
                p = name + strlen(parent_name);
            }

            strncpy(buf, p, CCSP_BASE_PARAM_LENGTH-1);
            buf[strlen(p)-1] = 0;
            return CCSP_BASE_OBJECT;
        }
        tmp--;
    }

    p = name;

    if ( len > strlen(parent_name) )
    {
        p = name + strlen(parent_name);
    }

    strncpy(buf, p, CCSP_BASE_PARAM_LENGTH-1);
    buf[strlen(p)-1] = 0;
    *inst_num = atoi(buf);

    return CCSP_BASE_INSTANCE;
}


int CcspBaseIf_registerBase(
    void* bus_handle,
    const char* dst_component_id,
    const char *component_name,
    int component_version,
    const char *dbus_path,
    const char *subsystem_prefix
)
{
#define  CCSP_NAME_PREFIX      ""	

    char buf[11][256];
    name_spaceType_t name_space[11];
    sprintf(buf[0],"%s%s.Name",CCSP_NAME_PREFIX,component_name);
    name_space[0].name_space = buf[0];
    name_space[0].dataType = ccsp_string;
    
    sprintf(buf[1],"%s%s.Version",CCSP_NAME_PREFIX,component_name);
    name_space[1].name_space = buf[1];
    name_space[1].dataType = ccsp_int;
    
    sprintf(buf[2],"%s%s.Author",CCSP_NAME_PREFIX,component_name);
    name_space[2].name_space = buf[2];
    name_space[2].dataType = ccsp_string;
    
    sprintf(buf[3],"%s%s.Health",CCSP_NAME_PREFIX,component_name);
    name_space[3].name_space = buf[3];
    name_space[3].dataType = ccsp_string;
    
    sprintf(buf[4],"%s%s.State",CCSP_NAME_PREFIX,component_name);
    name_space[4].name_space = buf[4];
    name_space[4].dataType = ccsp_int;
    
    
    sprintf(buf[5],"%s%s.Logging.Enable",CCSP_NAME_PREFIX,component_name);
    name_space[5].name_space = buf[5];
    name_space[5].dataType = ccsp_boolean;
    
    
    sprintf(buf[6],"%s%s.Logging.LogLevel",CCSP_NAME_PREFIX,component_name);
    name_space[6].name_space = buf[6];
    name_space[6].dataType = ccsp_int;
    
    
    sprintf(buf[7],"%s%s.Memory.MinUsage",CCSP_NAME_PREFIX,component_name);
    name_space[7].name_space = buf[7];
    name_space[7].dataType = ccsp_int;
    
    
    sprintf(buf[8],"%s%s.Memory.MaxUsage",CCSP_NAME_PREFIX,component_name);
    name_space[8].name_space = buf[8];
    name_space[8].dataType = ccsp_int;
    
    sprintf(buf[9],"%s%s.Memory.Consumed",CCSP_NAME_PREFIX,component_name);
    name_space[9].name_space = buf[9];
    name_space[9].dataType = ccsp_int;
    return CcspBaseIf_registerCapabilities(
               bus_handle,
               dst_component_id,
               component_name,
               component_version,
               dbus_path,
               subsystem_prefix,
               name_space,
               10
           );
}

int CcspBaseIf_SendparameterValueChangeSignal (
    void* bus_handle,
    parameterSigStruct_t *val,
    int size
)
{
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    DBusMessage *message;
    int i;
    DBusConnection *conn;
    DBusMessageIter array_iter;
    DBusMessageIter struct_iter;
    dbus_int32_t tmp ;
    dbus_uint32_t utmp ;
    DBusMessageIter iter;

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
        return CCSP_ERR_NOT_CONNECT;


    message = dbus_message_new_signal (CCSP_DBUS_PATH_EVENT, CCSP_DBUS_INTERFACE_EVENT, "parameterValueChangeSignal" );

    if(!message)
        return CCSP_ERR_MEMORY_ALLOC_FAIL;


    dbus_message_iter_init_append (message, &iter);



    dbus_message_iter_open_container (&iter,
                                      DBUS_TYPE_ARRAY,
                                      "(sssisu)",
                                      &array_iter);
    for(i = 0; i < size; i++)
    {
        dbus_message_iter_open_container (&array_iter,
                                          DBUS_TYPE_STRUCT,
                                          "sssisu",
                                          &struct_iter);

        DBUS_MESSAGE_APPEND_STRING (&struct_iter, val[i].parameterName);

        DBUS_MESSAGE_APPEND_STRING (&struct_iter, val[i].oldValue);
        DBUS_MESSAGE_APPEND_STRING (&struct_iter, val[i].newValue);
        tmp = val[i].type;
        dbus_message_iter_append_basic (&struct_iter, DBUS_TYPE_INT32, &tmp);
        DBUS_MESSAGE_APPEND_STRING (&struct_iter, val[i].subsystem_prefix);
        utmp = val[i].writeID;
        dbus_message_iter_append_basic (&struct_iter, DBUS_TYPE_UINT32, &utmp);

        dbus_message_iter_close_container (&array_iter,
                                           &struct_iter);
    }

    dbus_message_iter_close_container (&iter,
                                       &array_iter);

    tmp = size;
    dbus_message_iter_append_basic (&iter, DBUS_TYPE_INT32, &tmp);

    dbus_connection_send (conn, message, NULL);

    dbus_message_unref (message);
    return CCSP_SUCCESS;
}


int CcspBaseIf_SendtransferCompleteSignal (
    void* bus_handle
)
{
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    DBusMessage *message;
    int i;
    DBusConnection *conn;

    /*to support daemon redundency*/
    pthread_mutex_lock(&bus_info->info_mutex);
    for(i = 0; i < CCSP_MESSAGE_BUS_MAX_CONNECTION; i++)
    {
        if(bus_info->connection[i].connected && bus_info->connection[i].conn )
        {
            conn = bus_info->connection[i].conn;
            dbus_connection_ref (conn);
            break;
        }

    }
    pthread_mutex_unlock(&bus_info->info_mutex);

    if(i ==  CCSP_MESSAGE_BUS_MAX_CONNECTION)
        return CCSP_ERR_NOT_CONNECT;


    message = dbus_message_new_signal (CCSP_DBUS_PATH_EVENT, CCSP_DBUS_INTERFACE_EVENT, "transferCompleteSignal" );

    if(!message)
        return CCSP_ERR_MEMORY_ALLOC_FAIL;

    dbus_connection_send (conn, message, NULL);

    dbus_message_unref (message);
    return CCSP_SUCCESS;
}

int CcspBaseIf_SendtransferFailedSignal (
    void* bus_handle
)
{
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    DBusMessage *message;
    int i;
    DBusConnection *conn;

    /*to support daemon redundency*/
    pthread_mutex_lock(&bus_info->info_mutex);
    for(i = 0; i < CCSP_MESSAGE_BUS_MAX_CONNECTION; i++)
    {
        if(bus_info->connection[i].connected && bus_info->connection[i].conn )
        {
            conn = bus_info->connection[i].conn;
            dbus_connection_ref (conn);
            break;
        }

    }
    pthread_mutex_unlock(&bus_info->info_mutex);

    if(i ==  CCSP_MESSAGE_BUS_MAX_CONNECTION)
        return CCSP_ERR_NOT_CONNECT;


    message = dbus_message_new_signal (CCSP_DBUS_PATH_EVENT, CCSP_DBUS_INTERFACE_EVENT, "transferFailedSignal" );

    if(!message)
        return CCSP_ERR_MEMORY_ALLOC_FAIL;

    dbus_connection_send (conn, message, NULL);

    dbus_message_unref (message);
    return CCSP_SUCCESS;
}


int CcspBaseIf_SenddeviceProfileChangeSignal (
    void* bus_handle,
    char *component_name,
    char *component_dbus_path,
    dbus_bool isAvailable
)
{
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    DBusMessage *message;
    int i;
    DBusConnection *conn;
    dbus_bool_t btmp = isAvailable;

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
        return CCSP_ERR_NOT_CONNECT;


    message = dbus_message_new_signal (CCSP_DBUS_PATH_EVENT, CCSP_DBUS_INTERFACE_EVENT, "deviceProfileChangeSignal" );

    if(!message)
        return CCSP_ERR_MEMORY_ALLOC_FAIL;

    btmp = isAvailable;
    dbus_message_append_args (message, DBUS_TYPE_STRING, &component_name, DBUS_TYPE_INVALID);
    dbus_message_append_args (message, DBUS_TYPE_STRING, &component_dbus_path, DBUS_TYPE_INVALID);
    dbus_message_append_args (message, DBUS_TYPE_BOOLEAN, &btmp, DBUS_TYPE_INVALID);

    dbus_connection_send (conn, message, NULL);

    dbus_message_unref (message);
    return CCSP_SUCCESS;
}


int CcspBaseIf_SendcurrentSessionIDSignal (
    void* bus_handle,
    int priority,
    int sessionID
)
{
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    DBusMessage *message;
    int i;
    DBusConnection *conn;
    dbus_int32_t tmp ;

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
        return CCSP_ERR_NOT_CONNECT;


    message = dbus_message_new_signal (CCSP_DBUS_PATH_EVENT, CCSP_DBUS_INTERFACE_EVENT, "currentSessionIDSignal" );

    if(!message)
        return CCSP_ERR_MEMORY_ALLOC_FAIL;

    tmp = priority;
    dbus_message_append_args (message, DBUS_TYPE_INT32, &tmp, DBUS_TYPE_INVALID);
    tmp = sessionID;
    dbus_message_append_args (message, DBUS_TYPE_INT32, &tmp, DBUS_TYPE_INVALID);

    dbus_connection_send (conn, message, NULL);

    dbus_message_unref (message);
    return CCSP_SUCCESS;
}

int CcspBaseIf_SendSignal(
    void * bus_handle,
    char *event
)
{
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    DBusMessage *message;
    int i;
    DBusConnection *conn;

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
        return CCSP_ERR_NOT_CONNECT;


    message = dbus_message_new_signal (CCSP_DBUS_PATH_EVENT, CCSP_DBUS_INTERFACE_EVENT, event);

    if(!message)
        return CCSP_ERR_MEMORY_ALLOC_FAIL;


    dbus_connection_send (conn, message, NULL);

    dbus_message_unref (message);
    return CCSP_SUCCESS;
}

int CcspBaseIf_SenddiagCompleteSignal(void * bus_handle)
{
    return CcspBaseIf_SendSignal(bus_handle,  "diagCompleteSignal");
}

int CcspBaseIf_SendsystemReadySignal(void * bus_handle)
{
    return CcspBaseIf_SendSignal(bus_handle,  "systemReadySignal");
}

int CcspBaseIf_SendsystemRebootSignal(void * bus_handle)
{
    return CcspBaseIf_SendSignal(bus_handle,  "systemRebootSignal");
}

/* This keep alive message just wants to say "hello" to dbus daemon. No one care it.*/
int CcspBaseIf_SendsystemKeepaliveSignal(void * bus_handle)
{
    return CcspBaseIf_SendSignal(bus_handle,  "systemKeepaliveSignal");
}


//PSM function

#ifdef PSM_SLAP_VAR
int PSM_Set_Record_Value
(
    void*                       bus_handle,
    char const * const          pSubSystemPrefix,
    char const * const          pRecordName,
    unsigned int const          ulRecordType,
    PSLAP_VARIABLE              pValue
)
{
    parameterValStruct_t val[1];
    char buf[128];
    char psmName[256];
    char*  var_string = 0;
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    char *str;
    int ret;

    val[0].parameterName  = pRecordName;
    val[0].parameterValue = buf;
    switch(pValue->Syntax)
    {
    case SLAP_VAR_SYNTAX_int:
        sprintf(
            buf,
            "%d",
            pValue->Variant.varInt
        );
        val[0].type = ccsp_int;
        break;
    case SLAP_VAR_SYNTAX_uint32:
        sprintf(
            buf,
            "%u",
            (unsigned int) pValue->Variant.varUint32
        );
        val[0].type = ccsp_unsignedInt;
        break;
    case SLAP_VAR_SYNTAX_bool:
        if(pValue->Variant.varBool)
            strcpy(buf,PSM_TRUE);
        else
            strcpy(buf,PSM_FALSE);
        val[0].type = ccsp_boolean;
        break;
    case SLAP_VAR_SYNTAX_string:
        val[0].parameterValue = pValue->Variant.varString;
        val[0].type = ccsp_string;
        break;

    case SLAP_VAR_SYNTAX_TYPE_ucharArray:
    {
        SLAP_UCHAR_ARRAY* var_uchar_array = pValue->Variant.varUcharArray;
        var_string   = bus_info->mallocfunc(var_uchar_array->VarCount * 2 + 1);
        int i;
        val[0].type = ccsp_byte;
        for ( i = 0; i < var_uchar_array->VarCount; i++ )
        {
            sprintf
            (
                &var_string[i * 2],
                "%02X",
                var_uchar_array->Array.arrayUchar[i]
            );
        }

        val[0].parameterValue = var_string;
        break;
    }
    default:
        return CCSP_CR_ERR_INVALID_PARAM;
    }

    if ( pSubSystemPrefix && pSubSystemPrefix[0] != 0 )
    {
        sprintf(psmName, "%s%s", pSubSystemPrefix, CCSP_DBUS_PSM);
    }
    else
    {
        strcpy(psmName, CCSP_DBUS_PSM);
    }

    ret =  CcspBaseIf_setParameterValues(
                   bus_handle,
                   psmName,
                   CCSP_DBUS_PATH_PSM,
                   0,
                   0,
                   val,
                   1,
                   1,
                   &str
               );
    if(var_string)
        bus_info->freefunc(var_string);
    if(str)  
       bus_info->freefunc(str);
    return ret;
}

int PSM_Get_Record_Value
(
    void*                       bus_handle,
    char const * const          pSubSystemPrefix,
    char const * const          pRecordName,
    unsigned int *              ulRecordType,
    PSLAP_VARIABLE              pValue
)
{
    char * parameterNames[1];
    char psmName[256];
    int size;
    parameterNames[0] = pRecordName;
    parameterValStruct_t **val = 0;
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;

    if ( pSubSystemPrefix && pSubSystemPrefix[0] != 0 )
    {
        sprintf(psmName, "%s%s", pSubSystemPrefix, CCSP_DBUS_PSM);
    }
    else
    {
        strcpy(psmName, CCSP_DBUS_PSM);
    }

    int ret = CcspBaseIf_getParameterValues(
                  bus_handle,
                  psmName,
                  CCSP_DBUS_PATH_PSM,
                  parameterNames,
                  1,
                  &size,
                  &val
              );

    if(ret != CCSP_SUCCESS )
        return ret;

    if(size < 1)
        return CCSP_CR_ERR_INVALID_PARAM;

    if(ulRecordType)    
       *ulRecordType = val[0]->type;

    /* ccsp data type is always defined the same as SYS_REP_RECORD_TYPE__? */
    switch(val[0]->type)
    {
        long long tmp ;
    case ccsp_int:
        pValue->Syntax = SLAP_VAR_SYNTAX_int ;
        pValue->Variant.varInt = atoi(val[0]->parameterValue);
        break;
    case ccsp_unsignedInt:
        pValue->Syntax = SLAP_VAR_SYNTAX_uint32 ;
        tmp = atoll(val[0]->parameterValue);
        pValue->Variant.varUint32 = tmp;
        break;
    case ccsp_boolean:
        pValue->Syntax = SLAP_VAR_SYNTAX_bool ;

        if(!strcmp(val[0]->parameterValue,PSM_FALSE) )
        {
            pValue->Variant.varBool = SLAP_FALSE;
        }
        else if( AnscEqualString(val[0]->parameterValue, "false", FALSE))
        {
            pValue->Variant.varBool = SLAP_FALSE;
        }
        else if ( AnscEqualString(val[0]->parameterValue, "true", FALSE))
        {
            pValue->Variant.varBool = TRUE; /*Do not use SLAP_TRUE, its value of -1 causes conversion issues*/
        }
        else
        {
            pValue->Variant.varBool = TRUE; /*Do not use SLAP_TRUE, its value of -1 causes conversion issues*/
        }
        break;
    case ccsp_string:
        pValue->Syntax = SLAP_VAR_SYNTAX_string ;
        if ( pValue->Variant.varString )
        {
            bus_info->freefunc(pValue->Variant.varString);
        }
        pValue->Variant.varString = bus_info->mallocfunc(strlen(val[0]->parameterValue)+1);
        strcpy(pValue->Variant.varString, val[0]->parameterValue);
        break;
    case ccsp_byte:
//	    case PSM_RECORD_TYPE_REFER:
    {
        SLAP_UCHAR_ARRAY*              var_ucharArray = (SLAP_UCHAR_ARRAY*         )NULL;
        int                            ulUcharCount   = strlen(val[0]->parameterValue) / 2;
        int                            ulTmpValue     = 0;
        int                            i              = 0;
        char                           temp_char[3];
        char *var_uchar_array = val[0]->parameterValue;

        pValue->Syntax = SLAP_VAR_SYNTAX_TYPE_ucharArray ;
        var_ucharArray = (SLAP_UCHAR_ARRAY*)bus_info->mallocfunc(sizeof(SLAP_UCHAR_ARRAY) + ulUcharCount);

        var_ucharArray->Size     = sizeof(SLAP_UCHAR_ARRAY) + ulUcharCount;
        var_ucharArray->VarCount = ulUcharCount;
        var_ucharArray->Syntax   = SLAP_VAR_SYNTAX_ucharArray;

        for ( i = 0; i < ulUcharCount; i++ )
        {
            temp_char[0] = *var_uchar_array;
            var_uchar_array++;
            temp_char[1] = *var_uchar_array;
            var_uchar_array++;
            temp_char[2] = 0;

            sscanf
            (
                temp_char,
                "%02X",
                &ulTmpValue
            );

            var_ucharArray->Array.arrayUchar[i] = (UCHAR)ulTmpValue;
        }
        if ( pValue->Variant.varUcharArray )
        {
            bus_info->freefunc(pValue->Variant.varUcharArray);
        }
        pValue->Variant.varUcharArray = var_ucharArray;
    }
    break;
    default:
        ret = CCSP_CR_ERR_INVALID_PARAM;
    }
    free_parameterValStruct_t(bus_handle , size, val);
    return ret;
}

#endif

int PSM_Set_Record_Value2
(
    void*                       bus_handle,
    char const * const          pSubSystemPrefix,
    char const * const          pRecordName,
    unsigned int const          ulRecordType,
    char const * const          pVal
)
{
    parameterValStruct_t val[1];
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    char *str;
    char psmName[256];
    int ret;

    val[0].parameterName  = pRecordName;
    val[0].type = ulRecordType;
    if(ulRecordType == ccsp_boolean)
    {
        if(strcmp(pVal,PSM_FALSE) && strcmp(pVal, PSM_TRUE))
        {
	        return CCSP_CR_ERR_INVALID_PARAM;
    } 
    } 
    
    if ( pSubSystemPrefix && pSubSystemPrefix[0] != 0 )
    {
        sprintf(psmName, "%s%s", pSubSystemPrefix, CCSP_DBUS_PSM);
    }
    else
    {
        strcpy(psmName, CCSP_DBUS_PSM);
    }

    val[0].parameterValue = pVal;
    ret = CcspBaseIf_setParameterValues(
               bus_handle,
               psmName,
               CCSP_DBUS_PATH_PSM,
               0,
               0,
               val,
               1,
               1,
               &str
           );
    
                  
    if(str)  
       bus_info->freefunc(str);
    return ret;
}


int PSM_Get_Record_Value2
(
    void*                       bus_handle,
    char const * const          pSubSystemPrefix,
    char const * const          pRecordName,
    unsigned int *               ulRecordType,
    char**                      pValue
)
{
    char * parameterNames[1] = {NULL};
    char psmName[256] = {0};
    int size = 0;
    parameterValStruct_t **val = 0;
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
	int ret = 0;

	*pValue = NULL;
    if ( pSubSystemPrefix && pSubSystemPrefix[0] != 0 )
    {
        sprintf(psmName, "%s%s", pSubSystemPrefix, CCSP_DBUS_PSM);
    }
    else
    {
        strcpy(psmName, CCSP_DBUS_PSM);
    }

    parameterNames[0] = pRecordName;

    /*
    CcspTraceDebug(("<pid%d>[%s]: psmName='%s', path='%s', para='%s'\n",
                    getpid(), __FUNCTION__,
                    psmName, CCSP_DBUS_PATH_PSM, parameterNames[0]));
    */

    ret = CcspBaseIf_getParameterValues(
                  bus_handle,
                  psmName,
                  CCSP_DBUS_PATH_PSM,
                  parameterNames,
                  1,
                  &size,
                  &val
              );

    /*
    CcspTraceDebug(("<pid%d>[%s]: ret='%d', size='%d'\n", 
                    getpid(), __FUNCTION__, ret, size));

    if(val && val[0] && size) {
        CcspTraceDebug(("<pid%d>[%s]: name='%s', value='%s'\n", 
                        getpid(), __FUNCTION__, 
                        val[0]->parameterName ? val[0]->parameterName : "NULL",
                        val[0]->parameterValue ? val[0]->parameterValue : "NULL"));
    }
    */

    if(ret == CCSP_SUCCESS) {
        if(val && val[0] && size) {
            if(ulRecordType) *ulRecordType = val[0]->type;
    *pValue = bus_info->mallocfunc(strlen(val[0]->parameterValue)+1);
    strcpy(*pValue,  val[0]->parameterValue);
        }
        else ret = CCSP_CR_ERR_INVALID_PARAM;
    }

    free_parameterValStruct_t(bus_handle , size, val);

    return ret;
}


int PSM_Del_Record
(
    void*                       bus_handle,
    char const * const          pSubSystemPrefix,
    char const * const          pRecordName
)
{
    parameterAttributeStruct_t attr_val[1];
    parameterInfoStruct_t **parameter;
    char psmName[256];
    int size;
    int orgSize;
    int ret;

    if ( pSubSystemPrefix && pSubSystemPrefix[0] != 0 )
    {
        sprintf(psmName, "%s%s", pSubSystemPrefix, CCSP_DBUS_PSM);
    }
    else
    {
        strcpy(psmName, CCSP_DBUS_PSM);
    }

    if ( pRecordName[strlen(pRecordName)-1] == '.' )
    {
        ret = CcspBaseIf_getParameterNames(
                      bus_handle,
                      psmName,
                      CCSP_DBUS_PATH_PSM,
                      pRecordName,
                      0,
                      &size ,
                      &parameter
                  );

        if( ret != CCSP_SUCCESS)
            return ret;

        orgSize = size;
        for ( ; size > 0; size-- )
        {
            attr_val[0].parameterName = parameter[size-1]->parameterName;
            attr_val[0].notificationChanged = 0;
            attr_val[0].notification = 0;
            attr_val[0].access = 0;
            attr_val[0].accessControlChanged = 1;
            attr_val[0].accessControlBitmask = 0;


            ret = CcspBaseIf_setParameterAttributes(
                        bus_handle,
                        psmName,
                        CCSP_DBUS_PATH_PSM,
                        0,
                        attr_val,
                        1
                    );

            if ( ret != CCSP_SUCCESS ) 
                break;
        }

        free_parameterInfoStruct_t(bus_handle, orgSize, parameter);

        return ret;
    }
    else
    {
        attr_val[0].parameterName = pRecordName;
        attr_val[0].notificationChanged = 0;
        attr_val[0].notification = 0;
        attr_val[0].access = 0;
        attr_val[0].accessControlChanged = 1;
        attr_val[0].accessControlBitmask = 0;


        return  CcspBaseIf_setParameterAttributes(
                    bus_handle,
                    psmName,
                    CCSP_DBUS_PATH_PSM,
                    0,
                    attr_val,
                    1
                );
    }
}

int PsmGroupGet(void *bus_handle, const char *subsys, 
        const char *names[], int nname, parameterValStruct_t ***records, int *nrec)
{
    char psmName[256];

    if (!bus_handle || !names || !records || !nrec)
        return CCSP_FAILURE;

    snprintf(psmName, sizeof(psmName), "%s%s", (subsys ? subsys : ""), CCSP_DBUS_PSM);

    return CcspBaseIf_getParameterValues(bus_handle, psmName, CCSP_DBUS_PATH_PSM, 
            (char **)names, nname, nrec, records);
}

void PsmFreeRecords(void *bus_handle, parameterValStruct_t **records, int nrec)
{
    free_parameterValStruct_t(bus_handle, nrec, records);
}

int PsmGetNextLevelInstances
(
   void* bus_handle,
   char const * const pSubSystemPrefix,
   char const * const pParentPath, 
   unsigned int* pulNumInstance, 
   unsigned int**  ppInstanceArray
)
{  
   char psmName[256];

   if ( pSubSystemPrefix && pSubSystemPrefix[0] != 0 )
   {
        sprintf(psmName, "%s%s", pSubSystemPrefix, CCSP_DBUS_PSM);
   }
   else
   {
        strcpy(psmName, CCSP_DBUS_PSM);
   }

   return CcspBaseIf_GetNextLevelInstances
		(
		    bus_handle,
		    psmName,
		    CCSP_DBUS_PATH_PSM,
		    pParentPath,
		    pulNumInstance,
		    ppInstanceArray
		);
}


int PsmEnumRecords
(
    void* bus_handle,
    char const * const pSubSystemPrefix,
    char const * const pParentPath,
    dbus_bool nextLevel,
    unsigned int * pulNumRec,
    PCCSP_BASE_RECORD*  ppRecArray
)
{
   char psmName[256];

   if ( pSubSystemPrefix && pSubSystemPrefix[0] != 0 )
   {
        sprintf(psmName, "%s%s", pSubSystemPrefix, CCSP_DBUS_PSM);
   }
   else
   {
        strcpy(psmName, CCSP_DBUS_PSM);
   }

   return CcspBaseIf_EnumRecords
		(
		    bus_handle,
		    psmName,
		    CCSP_DBUS_PATH_PSM,
		    pParentPath,
		    nextLevel,
		    pulNumRec,
		    ppRecArray
		);
}

int PSM_Reset_UserChangeFlag
(
    void*                       bus_handle,
    char const * const          pSubSystemPrefix,
    char const * const          pathName 
)
{
    char record_name[256];

    snprintf(record_name, sizeof(record_name), "UserChanged.%s", pathName);

    return PSM_Del_Record(bus_handle, pSubSystemPrefix, record_name);
}

/* The function is called to register event, if the interface name and data path is NULL. Default is register the base interface*/
int  CcspIf_Register_Event
(
    void* bus_handle,
    const char* sender,
    const char* event_name,
    const char* dbus_path_event,
    const char* dbus_interface_event
)
{
    if(NULL == dbus_path_event && NULL == dbus_interface_event)
        return CcspBaseIf_Register_Event(bus_handle, sender, event_name);
   
    CCSP_Message_Bus_Set_Event_Callback(bus_handle,CcspBaseIf_evt_callback, bus_handle); 	
    return CCSP_Message_Bus_Register_Event(bus_handle, sender, dbus_path_event, dbus_interface_event,event_name);
}

int  CcspIf_UnRegister_Event
(
    void* bus_handle,
    const char* sender,
    const char* event_name,
    const char* dbus_path_event,
    const char* dbus_interface_event
)
{
    if(NULL == dbus_path_event && NULL == dbus_interface_event)
    {
        return CcspBaseIf_UnRegister_Event(bus_handle, sender, event_name);
    }
    return CCSP_Message_Bus_UnRegister_Event(bus_handle, sender, dbus_path_event, dbus_interface_event,event_name);	
}

/*
 * <method name="notifyCwmpEvent">
 *          <arg type="(iss)" name="event" direction="in" />
 *          <arg type="i" name="status" direction="out" />
 * </method>
 */

int
CcspIf_notifyCwmpEventToAP
    (
        ANSC_HANDLE                         bus_handle,
        ULONG                               ulEventCode,
        char*                               pEventDesp,
        char*                               pTimeStamp
    )
{
    DBusMessage *message;
    DBusMessage *reply;
    int ret = CCSP_FAILURE;
    dbus_int32_t res ;
    dbus_int32_t tmp ;
    DBusMessageIter iter;
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    DBusMessageIter struct_iter;
    int i;

    message = dbus_message_new_method_call (STBSERVICE_AP_DESTINATION_NAME,
                                            STBSERVICE_AP_PATH_NAME,
                                            STBSERVICE_AP_INTERFACE_NAME,
                                            "notifyCwmpEvent");
    if (!message )
    {
        printf ("No memory\n");
        return CCSP_ERR_MEMORY_ALLOC_FAIL;
    }

    dbus_message_iter_init_append (message, &iter);


    ret = dbus_message_iter_open_container (&iter,
                                            DBUS_TYPE_STRUCT,
                                            "iss",
                                            &struct_iter);


    tmp = ulEventCode;
    ret = dbus_message_iter_append_basic (&struct_iter, DBUS_TYPE_INT32, &tmp);
    DBUS_MESSAGE_APPEND_STRING(&struct_iter, pEventDesp);
    DBUS_MESSAGE_APPEND_STRING(&struct_iter, pTimeStamp);

    ret = dbus_message_iter_close_container (&iter, &struct_iter);

    ret = CCSP_Message_Bus_Send_Msg(bus_handle, message, NOTIFYCWMPEVENT_TIMEOUT , &reply);
    if(reply )
    {
	    char *str = 0;
        DBusMessageIter iter;
        dbus_message_iter_init (reply, &iter);
        if(dbus_message_iter_get_arg_type (&iter) == DBUS_TYPE_INT32)
        {
            dbus_message_iter_get_basic (&iter, &res);
            ret = res;
        }
        else
        {
            ret = CCSP_CR_ERR_INVALID_PARAM;
        }

        dbus_message_unref (reply);
    }

    dbus_message_unref (message);
    return ret;
}

int  CcspBaseIf_Register_Event
(
    void* bus_handle,
    const char* sender,
    const char* event_name
)
{
   CCSP_Message_Bus_Set_Event_Callback(bus_handle,CcspBaseIf_evt_callback, bus_handle); 	
   return CCSP_Message_Bus_Register_Event(bus_handle, sender, CCSP_DBUS_PATH_EVENT, CCSP_DBUS_INTERFACE_EVENT,event_name );
}

int  CcspBaseIf_UnRegister_Event
(
    void* bus_handle,
    const char* sender,
    const char* event_name
)
{
   return CCSP_Message_Bus_UnRegister_Event(bus_handle, sender, CCSP_DBUS_PATH_EVENT, CCSP_DBUS_INTERFACE_EVENT,event_name );
	
}


int CcspBaseIf_GetRemoteParameterValue(
    void* bus_handle,
    const char* cr_component_id,
    const char *name_space,
    const char *subsystem_prefix,
    char * parameterNames[],
    int size,
    int *val_size,
    parameterValStruct_t ***val
)
{
  int ret;
  componentStruct_t **components;
  int msize;

  ret = CcspBaseIf_discComponentSupportingNamespace (
          bus_handle,
          cr_component_id,
          name_space,
          subsystem_prefix,
          &components,
          &msize);

  if(ret != CCSP_SUCCESS )
     return ret;
     
  if(msize < 1)
     return CCSP_CR_ERR_UNSUPPORTED_NAMESPACE;
   
  ret = CcspBaseIf_getParameterValues(
          bus_handle,
          components[0]->componentName,
          components[0]->dbusPath,
          parameterNames,
          size,
          val_size,
          val
          );
  
  return ret;
}


int CcspBaseIf_SetRemoteParameterValue
(
    void* bus_handle,
    const char* cr_component_id,
    const char *name_space,
    const char *subsystem_prefix,
    int sessionId,
    unsigned int writeID,
    parameterValStruct_t *val,
    int size,
    dbus_bool commit,
    char ** invalidParameterName
)
{
  int ret;
  componentStruct_t **components;
  int msize;
  ret = CcspBaseIf_discComponentSupportingNamespace (
          bus_handle,
          cr_component_id,
          name_space,
          subsystem_prefix,
          &components,
          &msize);

  if(ret != CCSP_SUCCESS )
     return ret;
     
  if(msize < 1)
     return CCSP_CR_ERR_UNSUPPORTED_NAMESPACE;
   
  ret = CcspBaseIf_setParameterValues(
          bus_handle,
          components[0]->componentName,
          components[0]->dbusPath,
          sessionId,
          writeID,
          val,
          size,
          commit,
          invalidParameterName
          );
  
  return ret;

}
