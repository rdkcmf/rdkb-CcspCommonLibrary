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
#include <rbus_message_bus.h>
#include <dslh_definitions_database.h>

#ifdef PSM_SLAP_VAR
#include <slap_definitions.h>
#endif
#include <ccsp_psm_helper.h>
#include "ccsp_trace.h"

/* For AnscEqualString */
#include "ansc_platform.h"

#include "ansc_xml_dom_parser_interface.h"
#include "ansc_xml_dom_parser_external_api.h"
#include "ansc_xml_dom_parser_status.h"
#include "safec_lib_common.h"

/* define default CR device profile name */
#ifndef CCSP_ETHWAN_ENABLE
#define CCSP_ETHWAN_ENABLE "/nvram/ETHWAN_ENABLE"
#endif

#ifndef CCSP_CR_DEVICE_PROFILE_XML_FILE
#define CCSP_CR_DEVICE_PROFILE_XML_FILE "/usr/ccsp/cr-deviceprofile.xml"
#endif

#ifndef CCSP_CR_ETHWAN_DEVICE_PROFILE_XML_FILE
#define CCSP_CR_ETHWAN_DEVICE_PROFILE_XML_FILE "/usr/ccsp/cr-ethwan-deviceprofile.xml"
#endif

typedef struct _component_info {
    char **list;
    int size;
} component_info;

int   CcspBaseIf_timeout_seconds        = 60; //seconds
int   CcspBaseIf_timeout_getval_seconds = 120; //seconds
#define  CcspBaseIf_timeout_rbus  (CcspBaseIf_timeout_seconds * 1000) // in milliseconds
#define  CcspBaseIf_timeout_getval_rbus  (CcspBaseIf_timeout_getval_seconds * 1000) // in milliseconds

int CcspBaseIf_freeResources(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    int priority
)
{
    if(rbus_enabled == 1)
    {
        RBUS_LOG("rbus %s supports this function in different way\n", __FUNCTION__);
        return CCSP_SUCCESS;
    }
    DBusMessage *message = NULL;
    DBusMessage *reply = NULL; /*RDKB-6231, CID-33211, init before use */
    int ret = CCSP_FAILURE;
    dbus_int32_t tmp1 = 0;
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
    errno_t rc = -1;
    parameterValStruct_t **parameterVal = 0;
//    sprintf(tmp,"%s%s.%s",CcspBaseIf_DataModel,dst_component_id, "State");
    rc = sprintf_s(tmp, sizeof(tmp), "%s.State",dst_component_id);
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }
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
    errno_t rc = -1;

//    sprintf(tmp,"%s%s.%s",CcspBaseIf_DataModel,dst_component_id, "Health");
    rc = sprintf_s(tmp, sizeof(tmp), "%s.State",dst_component_id);
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }
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
    errno_t rc = -1;
//    sprintf(tmp,"%s%s.%s",CcspBaseIf_DataModel,dst_component_id, "Memory.Consumed");
    rc = sprintf_s(tmp, sizeof(tmp), "%s.State",dst_component_id);
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }
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
    errno_t rc = -1;
//    sprintf(tmp,"%s%s.%s",CcspBaseIf_DataModel,dst_component_id, "Memory.MinUsage");
    rc = sprintf_s(tmp, sizeof(tmp), "%s.State",dst_component_id);
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }
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

int CcspBaseIf_setParameterValues_rbus(
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
    UNREFERENCED_PARAMETER(dbus_path);
    int i = 0;
    int ret = CCSP_FAILURE;
    char *writeID_str = writeid_to_string(writeID);
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    if (*invalidParameterName)
    {
        *invalidParameterName = NULL; // initialize
    }
    rbusMessage request, response;

    /* There is a case which we have seen in RDKB-29328, where set is called with Size as 0.
     * No action to be taken for that..
     */
    if (0 == size)
    {
        CcspTraceWarning(("%s component calls SET without the dml element name. Returning success as there no action taken\n", bus_info->component_id));
        *invalidParameterName = 0;
        ret = CCSP_SUCCESS;
        return ret;
    }

    rbusMessage_Init(&request);
    rbusMessage_SetInt32(request, sessionId);
    rbusMessage_SetString(request, writeID_str);
    rbusMessage_SetInt32(request, size);

    for(i = 0; i < size; i++)
    {
        rbusMessage_SetString(request, val[i].parameterName);
        rbusMessage_SetInt32(request, val[i].type);
        rbusMessage_SetString(request, val[i].parameterValue);
    }
    rbusMessage_SetString(request, commit ? "TRUE" : "FALSE");

    /* If the size is 0, val itself is NULL; val[0].parameterName is NULL pointer dereferencing. We avoided it in the above if condition per RDKB-29328 */
    const char *object_name = val[0].parameterName;
    if(dst_component_id && (strstr(dst_component_id, ".psm")))
    {
        object_name = dst_component_id;
    }

    RBUS_LOG("%s Calling rbus_invokeRemoteMethod for param on %s\n", __FUNCTION__, object_name);
    if((ret = Rbus_to_CCSP_error_mapper(rbus_invokeRemoteMethod(object_name, METHOD_SETPARAMETERVALUES, request, CcspBaseIf_timeout_rbus, &response))) != CCSP_Message_Bus_OK)
    {
        RBUS_LOG_ERR("%s rbus_invokeRemoteMethod: for param[0]=%s failed with Err: %d\n", __FUNCTION__, val[0].parameterName, ret);
        return ret;
    }

    rbusMessage_GetInt32(response, &ret);
    if((ret == CCSP_SUCCESS) || (ret == RBUS_RETURN_CODE_SUCCESS))
    {
        const char *str = NULL;
        rbusMessage_GetString(response, &str); //invalid param
        if(str)
        {
            *invalidParameterName = bus_info->mallocfunc(strlen(str)+1);
            strcpy(*invalidParameterName, str);
        }
        else
            *invalidParameterName = 0;
        ret = CCSP_SUCCESS;
    }
    else if(ret < CCSP_SUCCESS)
    {
        ret = Rbus2_to_CCSP_error_mapper(ret);
    }
    rbusMessage_Release(response);
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
    if(rbus_enabled == 1)
    {
        return CcspBaseIf_setParameterValues_rbus(bus_handle, dst_component_id, dbus_path,sessionId, writeID,val,size,commit,invalidParameterName);
    }

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
    errno_t rc = -1;

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
        //CcspTraceError(("dbus_message_iter_append_basic ret = %d \n", ret));

        //CcspTraceInfo(("%d : Name = %s , value = %s \n", i, val[i].parameterName, val[i].parameterValue));

        dbus_message_iter_close_container (&array_iter,
                &struct_iter);
    }

    ret = dbus_message_iter_close_container (&iter,
            &array_iter);
    //	CcspTraceError(("dbus_message_iter_close_container ret = %d \n", ret));
    tmp = size;
    dbus_message_iter_append_basic (&iter, DBUS_TYPE_INT32, &tmp);
    dbus_message_iter_append_basic (&iter, DBUS_TYPE_BOOLEAN, &commit);
    ret = CCSP_Message_Bus_Send_Msg(bus_handle, message, CcspBaseIf_timeout_seconds , &reply);
    //CcspTraceError(("CCSP_Message_Bus_Send_Msg ret = %d \n", ret));

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
                rc = strcpy_s(*invalidParameterName, (strlen(str)+1), str);
                ERR_CHK(rc);
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

int CcspBaseIf_setCommit_rbus(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    int sessionId,
    unsigned int writeID,
    dbus_bool commit
    )
{
    UNREFERENCED_PARAMETER(bus_handle);
    UNREFERENCED_PARAMETER(dbus_path);
    int ret = CCSP_FAILURE;
    char *writeID_str = writeid_to_string(writeID);
    rbusMessage request, response;

    rbusMessage_Init(&request);
    rbusMessage_SetInt32(request, sessionId);
    rbusMessage_SetString(request, writeID_str);
    rbusMessage_SetInt32(request, (int32_t)commit);

     if((ret = Rbus_to_CCSP_error_mapper(rbus_invokeRemoteMethod(dst_component_id , METHOD_COMMIT, request, CcspBaseIf_timeout_rbus, &response))) != CCSP_Message_Bus_OK)
     {
        RBUS_LOG_ERR("%s rbus_invokeRemoteMethod on %s: Err: %d\n", __FUNCTION__, dst_component_id, ret);
        return ret;
    }

    rbusMessage_GetInt32(response, &ret);
    rbusMessage_Release(response);
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
    if(rbus_enabled == 1)
    {
        return CcspBaseIf_setCommit_rbus(bus_handle, dst_component_id, dbus_path, sessionId, writeID, commit);
    }

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

int CcspBaseIf_getParameterValues_rbus(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    char * parameterNames[],
    int param_size,
    int *val_size,
    parameterValStruct_t ***parameterval
    )
{
    UNREFERENCED_PARAMETER(dbus_path);
    parameterValStruct_t **val = 0;
    *val_size = 0;
    int err = CCSP_FAILURE, ret = CCSP_FAILURE;
    int i = 0;
    int param_len = 0;
    int32_t type = 0;
    errno_t rc = -1;
    rbusMessage request, response;
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;

    /* There is a case which we have seen in RDKB-29328, where set is called with Size as 0.
     * No action to be taken for that..
     */
    if (0 == param_size)
    {
        CcspTraceWarning(("%s component calls GET without the dml element name. Returning success as there no action taken\n", bus_info->component_id));
        ret = CCSP_SUCCESS;
        *val_size = 0;
        return ret;
    }

    rbusMessage_Init(&request);
    rbusMessage_SetString(request, bus_info->component_id);
    rbusMessage_SetInt32(request, (int32_t)param_size);

    for(i = 0; i < param_size; i++)
    {
        rbusMessage_SetString(request, parameterNames[i]);
    }

    /* If the param_size is 0, parameterNames is NULL; We avoided it in the above if condition per RDKB-29328 */
    param_len = strlen(parameterNames[0]);
    const char *object_name = parameterNames[0];
    if(dst_component_id) {
        if((parameterNames[0][param_len - 1] == '.') || strstr(dst_component_id, ".psm"))
            object_name = dst_component_id;
    }

    RBUS_LOG("Calling rbus_invokeRemoteMethod for %s\n", object_name);
    if((err = Rbus_to_CCSP_error_mapper(rbus_invokeRemoteMethod(object_name, METHOD_GETPARAMETERVALUES, request, CcspBaseIf_timeout_getval_rbus, &response))) != CCSP_Message_Bus_OK)
    {
        RBUS_LOG_ERR("%s rbus_invokeRemoteMethod: for param[0]=%s failed with Err: %d\n", __FUNCTION__, parameterNames[0], err);
        return err;
    }

    rbusMessage_GetInt32(response, &ret);
    if((ret == CCSP_SUCCESS) || (ret == RBUS_RETURN_CODE_SUCCESS))
    {
        ret = CCSP_SUCCESS;
        rbusMessage_GetInt32(response, val_size);
        RBUS_LOG("No. of o/p params: %d\n", *val_size);
        if(*val_size)
        {
            val = bus_info->mallocfunc(*val_size*sizeof(parameterValStruct_t *));
            memset(val, 0, *val_size*sizeof(parameterValStruct_t *));
            const char *tmpbuf = NULL;

            for(i = 0; i < *val_size; i++)
            {
                val[i] = bus_info->mallocfunc(sizeof(parameterValStruct_t));
                memset(val[i], 0, sizeof(parameterValStruct_t));

                /* Get Name */
                tmpbuf = NULL;
                rbusMessage_GetString(response, &tmpbuf);
                val[i]->parameterName = bus_info->mallocfunc(strlen(tmpbuf)+1);
                rc = strcpy_s(val[i]->parameterName, (strlen(tmpbuf)+1), tmpbuf);
                ERR_CHK(rc);

                /* Get Type */
                rbusMessage_GetInt32(response, &type);
                if (type < RBUS_BOOLEAN)
                {
                    /* Update the Type */
                    val[i]->type = type;

                    /* Get Value */
                    tmpbuf = NULL;
                    rbusMessage_GetString(response, &tmpbuf);
                    val[i]->parameterValue = bus_info->mallocfunc(strlen(tmpbuf)+1);
                    /*
                     * LIMITATION
                     * Below strcpy_s() api reverting to strcpy() api,
                     * Because, safec has the limitation of copying only 4k ( RSIZE_MAX ) to destination pointer
                     * And here, we have source pointer size more than 4k, i.e simetimes 190k also . So it won't copy to destination.
                     */
                     strcpy(val[i]->parameterValue, tmpbuf);
                }
                else
                {
                    ccsp_handle_rbus_component_reply (bus_info, response, (rbusValueType_t) type, &val[i]->type, &val[i]->parameterValue);
                }

                RBUS_LOG("Param [%d] Name = %s, Type = %d, Value = %s\n", i,val[i]->parameterName, val[i]->type, val[i]->parameterValue);
            }
        }
    }
    else if(ret < CCSP_SUCCESS)
    {
        ret = Rbus2_to_CCSP_error_mapper(ret);
    }

    rbusMessage_Release(response);
    *parameterval = val;
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
    if(rbus_enabled == 1)
    {
        return CcspBaseIf_getParameterValues_rbus(bus_handle, dst_component_id, dbus_path, parameterNames, param_size, val_size, parameterval);
    }

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
    errno_t rc = -1;
    unsigned int writeID = DSLH_MPA_ACCESS_CONTROL_ACS;

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

    writeID = get_writeid(bus_info->component_id);

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
                        rc = strcpy_s( val[i]->parameterName, (strlen(str)+1), str);
                        ERR_CHK(rc);
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
                       /*
                        * LIMITATION
                        * Below strcpy_s() api reverting to strcpy() api,
                        * Because, safec has the limitation of copying only 4k ( RSIZE_MAX ) to destination pointer
                        * And here, we have source pointer size more than 4k, i.e simetimes 190k also . So it won't copy to destination.
                        */
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

        /* check whether parameterName and parameterValue is not NULL to avoid segFault */
        if (*val_size == 1 && val[0]->parameterName && val[0]->parameterValue)
        { 
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

int CcspBaseIf_setParameterAttributes_rbus(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    int sessionId,
    parameterAttributeStruct_t *val,
    int size
    )
{
    UNREFERENCED_PARAMETER(dbus_path);
    int i = 0, ret = 0, ret1 = 0;
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    rbusMessage request, response;

    if (0 == size)
    {
        CcspTraceWarning(("%s component calls SET attributes without the dml element name. Returning success as there no action taken\n", bus_info->component_id));
        ret = CCSP_SUCCESS;
        return ret;
    }

    if (NULL == val)
    {
        CcspTraceWarning(("%s component calls SET with invalid attributes. Returning success as there no action taken\n", bus_info->component_id));
        ret = CCSP_SUCCESS;
        return ret;
    }

#ifdef USE_NOTIFY_COMPONENT
    parameterValStruct_t notif_val[1];
    char compo[256] = "eRT.com.cisco.spvtg.ccsp.notifycomponent";
    char bus[256] = "/com/cisco/spvtg/ccsp/notifycomponent";
    char param_name[256] = "Device.NotifyComponent.Notifi_ParamName";
    char* faultParam = NULL;
    UINT notification_count = 0;
    //char PA_name[256];
    char notification_parameter[256];
    char** p_notification_parameter = NULL;
    //_ansc_strcpy(PA_name, bus_info->component_id);
    p_notification_parameter = (char**) (bus_info->mallocfunc(sizeof(char*) * size));
    if (!p_notification_parameter )
    {
        CcspTraceError(("No memory\n"));
        return CCSP_ERR_MEMORY_ALLOC_FAIL;
    }
    memset(p_notification_parameter, 0, sizeof(char*) * size);
#endif
    char PA_name[256];
    int isTR069Req = 0;
    errno_t rc = -1;
    memset(PA_name,0,sizeof(PA_name));
    rc = strcpy_s(PA_name, sizeof(PA_name), bus_info->component_id);
    ERR_CHK(rc);

    if(strstr(PA_name, ".tr069pa"))
    {
            isTR069Req = 1;
    }

    rbusMessage_Init(&request);
    rbusMessage_SetInt32(request, sessionId);
    rbusMessage_SetInt32(request, size);

    for(i = 0; i < size; i++)
    {
        rbusMessage_SetString(request, val[i].parameterName);
#ifdef USE_NOTIFY_COMPONENT
        if(val[i].notificationChanged)
        {
            rc = sprintf_s(notification_parameter, sizeof(notification_parameter), "%s,%s,%s",val[i].parameterName,PA_name, ((val[i].notification) ? "true" : "false"));
            if(rc < EOK)
            {
                ERR_CHK(rc);
            }
            p_notification_parameter[notification_count] = (char *) bus_info->mallocfunc(strlen(notification_parameter)+1);
            rc = strcpy_s(p_notification_parameter[notification_count] , (strlen(notification_parameter)+1), notification_parameter);
            ERR_CHK(rc);
            notification_count++;
        }
#endif
        rbusMessage_SetInt32(request, val[i].notificationChanged);
        rbusMessage_SetInt32(request, val[i].notification);
        rbusMessage_SetInt32(request, val[i].access);
        rbusMessage_SetInt32(request, val[i].accessControlChanged);
        rbusMessage_SetInt32(request, val[i].accessControlBitmask);
        if(isTR069Req)
        {
            val[i].RequesterID = DSLH_MPA_ACCESS_CONTROL_ACS;
        }
        else
            val[i].RequesterID = 0;
        
        rbusMessage_SetInt32(request, val[i].RequesterID);
    }

    const char *object_name = val[0].parameterName;
    if(dst_component_id)
    {
        if(strstr(dst_component_id, ".psm"))
        {
            object_name = dst_component_id;
        }
    }

     if((ret = Rbus_to_CCSP_error_mapper(rbus_invokeRemoteMethod(object_name, METHOD_SETPARAMETERATTRIBUTES, request, CcspBaseIf_timeout_rbus, &response))) != CCSP_Message_Bus_OK)
     {
        RBUS_LOG_ERR("%s rbus_invokeRemoteMethod on %s: Err: %d\n", __FUNCTION__, dst_component_id, ret);
        return ret;
    }

#ifdef USE_NOTIFY_COMPONENT
    notif_val[0].parameterName = param_name;
    notif_val[0].type = ccsp_string;
    for(i = 0; (unsigned int)i < notification_count; i++)
    {
        notif_val[0].parameterValue = p_notification_parameter[i];
        ret1 = CcspBaseIf_setParameterValues(
                bus_handle,
                compo,
                bus,
                sessionId,
                0,
                notif_val,
                1,
                TRUE,
                &faultParam
                );
        if(ret1 != CCSP_SUCCESS)
        {
            CcspTraceError(("NOTIFICATION: %s : CcspBaseIf_setParameterValues failed. ret value = %d \n", __FUNCTION__, ret1));
            CcspTraceError(("NOTIFICATION: %s : Parameter = %s \n", __FUNCTION__, notif_val[0].parameterValue));
        }
    }
    for(i = 0; (unsigned int)i < notification_count ; i++)
    {
        if(p_notification_parameter[i])
            bus_info->freefunc(p_notification_parameter[i]);
    }
    if(p_notification_parameter)
        bus_info->freefunc(p_notification_parameter);
#endif

    rbusMessage_GetInt32(response, &ret);
    rbusMessage_Release(response);
    return ret;
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
    if(rbus_enabled == 1)
        return CcspBaseIf_setParameterAttributes_rbus(bus_handle, dst_component_id, dbus_path, sessionId, val, size);

    DBusMessage *message;
    DBusMessage *reply;
    int ret = CCSP_FAILURE;
    int ret1 = CCSP_FAILURE;
    dbus_int32_t res ;
    dbus_int32_t  tmp ;
    dbus_uint32_t utmp ;
    dbus_uint32_t utmp1 ;
    DBusMessageIter iter;
    DBusMessageIter array_iter;
    DBusMessageIter struct_iter;
    int i;
#ifdef USE_NOTIFY_COMPONENT
    parameterValStruct_t notif_val[1];
    char compo[256] = "eRT.com.cisco.spvtg.ccsp.notifycomponent";
    char bus[256] = "/com/cisco/spvtg/ccsp/notifycomponent";
    char param_name[256] = "Device.NotifyComponent.Notifi_ParamName";
    char* faultParam = NULL;
    UINT notification_count = 0;
 
    char true_false[10];
    char notification_parameter[256];
    char** p_notification_parameter;
#endif
    char PA_name[256];
    int isTR069Req = 0;
    errno_t rc = -1;
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    memset(PA_name,0,sizeof(PA_name));
    rc = strcpy_s(PA_name, sizeof(PA_name), bus_info->component_id);
    ERR_CHK(rc);

    if(strstr(PA_name, ".tr069pa"))
    {
            isTR069Req = 1;
    }

    message = dbus_message_new_method_call (dst_component_id,
            dbus_path,
            CCSP_DBUS_INTERFACE_BASE,
            "setParameterAttributes");
    if (!message )
    {
        CcspTraceError(("No memory\n"));
        return CCSP_ERR_MEMORY_ALLOC_FAIL;
    }
#ifdef USE_NOTIFY_COMPONENT
    p_notification_parameter = (char**) bus_info->mallocfunc(sizeof(char*) * size);

    if (!p_notification_parameter )
    {
        CcspTraceError(("No memory\n"));
        return CCSP_ERR_MEMORY_ALLOC_FAIL;
    }

    memset(p_notification_parameter, 0, sizeof(char*) * size);
#endif
    dbus_message_iter_init_append (message, &iter);

    tmp = sessionId;
    if (!dbus_message_iter_append_basic (&iter, DBUS_TYPE_INT32, &tmp))
        CcspTraceError(("error\n"));


    ret = dbus_message_iter_open_container (&iter,
            DBUS_TYPE_ARRAY,
            "(sbbibuu)",
            &array_iter);
    for(i = 0; i < size ; i++)
    {
        dbus_message_iter_open_container (&array_iter,
                DBUS_TYPE_STRUCT,
                "sbbibuu",
                &struct_iter);

        DBUS_MESSAGE_APPEND_STRING( &struct_iter, val[i].parameterName);
#ifdef USE_NOTIFY_COMPONENT
        if(val[i].notificationChanged)
        {
            rc = strcpy_s(true_false, sizeof(true_false), ((val[i].notification) ? "true" : "false"));
            ERR_CHK(rc);

            rc = sprintf_s(notification_parameter,sizeof(notification_parameter),"%s,%s,%s",val[i].parameterName,PA_name, true_false);
            if(rc < EOK) {
                ERR_CHK(rc);
            }
            p_notification_parameter[notification_count] = (char *) bus_info->mallocfunc(strlen(notification_parameter)+1);
            rc = strcpy_s(p_notification_parameter[notification_count] , (strlen(notification_parameter)+1), notification_parameter);
            ERR_CHK(rc);


            notification_count++;

        }
#endif
        ret = dbus_message_iter_append_basic (&struct_iter, DBUS_TYPE_BOOLEAN, &val[i].notificationChanged);
        ret = dbus_message_iter_append_basic (&struct_iter, DBUS_TYPE_BOOLEAN, &val[i].notification);
        tmp = val[i].access;
        ret = dbus_message_iter_append_basic (&struct_iter, DBUS_TYPE_INT32, &tmp);
        ret = dbus_message_iter_append_basic (&struct_iter, DBUS_TYPE_BOOLEAN, &val[i].accessControlChanged);
        utmp = val[i].accessControlBitmask;
        ret = dbus_message_iter_append_basic (&struct_iter, DBUS_TYPE_UINT32, &utmp);
        if(isTR069Req)
        {
            val[i].RequesterID = DSLH_MPA_ACCESS_CONTROL_ACS;
        }
        else
            val[i].RequesterID = 0;

        utmp1 = val[i].RequesterID;
        ret = dbus_message_iter_append_basic (&struct_iter, DBUS_TYPE_UINT32, &utmp1);
        //CcspTraceInfo(("NOTIFICATION: %s : Param %s RequesterID = %lu \n", __FUNCTION__,val[i].parameterName, val[i].RequesterID));
        
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
#ifdef USE_NOTIFY_COMPONENT

    notif_val[0].parameterName = param_name;
    notif_val[0].type = ccsp_string;

    for(i = 0; (unsigned int)i < notification_count ; i++)
    {
        notif_val[0].parameterValue = p_notification_parameter[i];

        ret1 = CcspBaseIf_setParameterValues(
                bus_handle,
                compo,
                bus,
                sessionId,
                0,
                notif_val,
                1,
                TRUE,
                &faultParam
                );

        if(ret1 != CCSP_SUCCESS)
        {
            CcspTraceError(("NOTIFICATION: %s : CcspBaseIf_setParameterValues failed. ret value = %d \n", __FUNCTION__, ret1));
            CcspTraceError(("NOTIFICATION: %s : Parameter = %s \n", __FUNCTION__, notif_val[0].parameterValue));
        }

    }


    for(i = 0; (unsigned int)i < notification_count ; i++)
    {
        if(p_notification_parameter[i])
            bus_info->freefunc(p_notification_parameter[i]);
    }

    if(p_notification_parameter)
        bus_info->freefunc(p_notification_parameter);
#endif
    return ret;
}

int CcspBaseIf_getParameterAttributes_rbus(
        void* bus_handle,
        const char* dst_component_id,
        char* dbus_path,
        char * parameterNames[],
        int size,
        int *val_size,
        parameterAttributeStruct_t ***parameterAttributeval
        )
{
    UNREFERENCED_PARAMETER(dbus_path);
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    int i=0, err = CCSP_FAILURE, ret = CCSP_FAILURE;
    parameterAttributeStruct_t **val = 0;
    *val_size = 0;
    rbusMessage request, response;
    errno_t rc = -1;

    if (0 == size)
    {
        CcspTraceWarning(("%s component calls GET attributes without the dml element name. Returning success as there no action taken\n", bus_info->component_id));
        ret = CCSP_SUCCESS;
        return ret;
    }

    for(i = 0; i < size; i++)
    {
        if(NULL == parameterNames[i])
        {
            CcspTraceWarning(("%s component calls GET attributes with NULL parameter names. Returning success as there no action taken\n", bus_info->component_id));
            ret = CCSP_SUCCESS;
            return ret;
        }
    }

    rbusMessage_Init(&request);
    rbusMessage_SetInt32(request,size);
    for(i = 0; i < size; i++)
    {
        rbusMessage_SetString(request, parameterNames[i]);
    }

    const char *object_name = parameterNames[0];
    int param_len = strlen(parameterNames[0]);
    if(dst_component_id)
    {
        if((parameterNames[0][param_len - 1] == '.') && (strstr(dst_component_id, ".psm")))
        {
            object_name = dst_component_id;
        }
    }

     if((err = Rbus_to_CCSP_error_mapper(rbus_invokeRemoteMethod(object_name, METHOD_GETPARAMETERATTRIBUTES, request, CcspBaseIf_timeout_rbus, &response))) != CCSP_Message_Bus_OK)
    {
        RBUS_LOG_ERR("%s rbus_invokeRemoteMethod on %s: Err: %d\n", __FUNCTION__, dst_component_id, err);
        return err;
    }

    rbusMessage_GetInt32(response, &ret);
    if( ret == CCSP_SUCCESS )
    {
        rbusMessage_GetInt32(response, val_size);
        if(*val_size)
        {
            val = bus_info->mallocfunc(*val_size*sizeof(parameterAttributeStruct_t *));
            memset(val, 0, *val_size*sizeof(parameterAttributeStruct_t *));
            const char *tmpbuf = NULL;

            for(i = 0; i < *val_size; i++)
            {
                val[i] = bus_info->mallocfunc(sizeof(parameterAttributeStruct_t));
                memset(val[i], 0, sizeof(parameterAttributeStruct_t));
                tmpbuf = NULL;
                rbusMessage_GetString(response, &tmpbuf);
                val[i]->parameterName = bus_info->mallocfunc(strlen(tmpbuf)+1);
                rc = strcpy_s(val[i]->parameterName, (strlen(tmpbuf)+1), tmpbuf);
                ERR_CHK(rc);
                rbusMessage_GetInt32(response, (int32_t*)&val[i]->notificationChanged);
                rbusMessage_GetInt32(response, (int32_t*)&val[i]->notification);
                rbusMessage_GetInt32(response, (int32_t*)&val[i]->accessControlChanged);
                rbusMessage_GetInt32(response, (int32_t*)&val[i]->access);
                rbusMessage_GetInt32(response, (int32_t*)&val[i]->accessControlBitmask);
            }
        }
    }
    else if(ret < CCSP_SUCCESS)
    {
        ret = Rbus2_to_CCSP_error_mapper(ret);
    }

    rbusMessage_Release(response);
    *parameterAttributeval = val;
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
    if(rbus_enabled == 1)
        return CcspBaseIf_getParameterAttributes_rbus(bus_handle, dst_component_id, dbus_path, parameterNames, size, val_size, parameterAttributeval);

    DBusMessage *message;
    DBusMessage *reply;
    int ret = CCSP_FAILURE;
    dbus_int32_t res ;
    dbus_int32_t tmp ;
    DBusMessageIter iter;
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    DBusMessageIter array_iter;
    int i;
    errno_t rc = -1;

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
                        rc = strcpy_s( val[i]->parameterName, (strlen(str)+1), str);
                        ERR_CHK(rc);
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

int CcspBaseIf_AddTblRow_rbus(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    int sessionId,
    char *objectName,
    int *instanceNumber
    )
{
    UNREFERENCED_PARAMETER(bus_handle);
    UNREFERENCED_PARAMETER(dbus_path);
    int ret = CCSP_FAILURE;
    int32_t tmp = 0;
    rbusMessage request, response;

    if(objectName == NULL || instanceNumber == NULL ||  objectName[0] == '\0' || objectName[strlen(objectName)-1] != '.')
    {
        return CCSP_ERR_INVALID_PARAMETER_VALUE;
    }
    rbusMessage_Init(&request);
    rbusMessage_SetInt32(request, sessionId);
    rbusMessage_SetString(request, objectName);

    if((ret = Rbus_to_CCSP_error_mapper(rbus_invokeRemoteMethod(dst_component_id, METHOD_ADDTBLROW, request, CcspBaseIf_timeout_rbus, &response))) != CCSP_Message_Bus_OK)
    {
        RBUS_LOG_ERR("%s rbus_invokeRemoteMethod for %s: Err: %d\n", __FUNCTION__, dst_component_id, ret);
        return ret;
    }

    rbusMessage_GetInt32(response, &ret); //result
    rbusMessage_GetInt32(response, &tmp); //inst num

    rbusMessage_Release(response);
    *instanceNumber = tmp;

    /* If the component that owns the table is rbus-2.0 component, the return code will be different. */
    if (RBUS_RETURN_CODE_SUCCESS == ret)
        ret = CCSP_SUCCESS;
    else if(ret < CCSP_SUCCESS)
    {
        ret = Rbus2_to_CCSP_error_mapper(ret);
    }

    return ret;
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
    if(rbus_enabled == 1)
        return CcspBaseIf_AddTblRow_rbus(bus_handle, dst_component_id, dbus_path, sessionId, objectName, instanceNumber);

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

int CcspBaseIf_DeleteTblRow_rbus(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    int sessionId,
    char * objectName
    )
{
    UNREFERENCED_PARAMETER(bus_handle);
    UNREFERENCED_PARAMETER(dbus_path);
    int ret = CCSP_FAILURE;
    rbusMessage request, response;

    rbusMessage_Init(&request);
    rbusMessage_SetInt32(request, sessionId);
    rbusMessage_SetString(request, objectName);

     if((ret = Rbus_to_CCSP_error_mapper(rbus_invokeRemoteMethod(dst_component_id, METHOD_DELETETBLROW, request, CcspBaseIf_timeout_rbus, &response))) != CCSP_Message_Bus_OK)
    {
        RBUS_LOG_ERR("%s rbus_invokeRemoteMethod on %s: Err: %d\n", __FUNCTION__, dst_component_id, ret);
        return CCSP_FAILURE;
    }

    rbusMessage_GetInt32(response, &ret);
    rbusMessage_Release(response);

    /* If the component that owns the table is rbus-2.0 component, the return code will be different. */
    if (RBUS_RETURN_CODE_SUCCESS == ret)
        ret = CCSP_SUCCESS;
    else if(ret < CCSP_SUCCESS)
    {
        ret = Rbus2_to_CCSP_error_mapper(ret);
    }

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
    if(rbus_enabled == 1)
        return CcspBaseIf_DeleteTblRow_rbus(bus_handle, dst_component_id, dbus_path, sessionId, objectName);

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

int CcspBaseIf_getParameterNames_rbus(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    char * parameterName,
    dbus_bool nextLevel,
    int *size ,
    parameterInfoStruct_t ***parameter
    )
{
    UNREFERENCED_PARAMETER(dbus_path);
    int32_t elemType = 0, accessFlags = 0;
    int i = 0, param_len = 0, ret = CCSP_FAILURE;
    rbusMessage request, response;
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    parameterInfoStruct_t **val=NULL;
    *parameter = 0;
    *size = 0;
    errno_t rc = -1;

    if(NULL == parameterName)
    {
        CcspTraceWarning(("%s component calls GET with invalid parameter name. Returning success as there no action taken\n", bus_info->component_id));
        ret = CCSP_SUCCESS;
        return ret;
    }
    rbusMessage_Init(&request);
    param_len = strlen(parameterName);
    rbusMessage_SetString(request, parameterName);
    rbusMessage_SetInt32(request, (int32_t)(nextLevel ? -1 : RBUS_MAX_NAME_DEPTH));
    rbusMessage_SetInt32(request, 0);/*not row names*/

    const char *object_name = parameterName;
    if(dst_component_id)
    {
        if((parameterName[param_len - 1] == '.') || (strstr(dst_component_id, ".psm")))
        {
            object_name = dst_component_id;
        }
    }

    RBUS_LOG("Calling rbus_invokeRemoteMethod for %s\n",object_name);
    if((ret = Rbus_to_CCSP_error_mapper(rbus_invokeRemoteMethod(object_name, METHOD_GETPARAMETERNAMES, request, CcspBaseIf_timeout_rbus, &response))) != CCSP_Message_Bus_OK)
    {
        RBUS_LOG_ERR("%s rbus_invokeRemoteMethod on %s: Err: %d\n", __FUNCTION__, dst_component_id, ret);
        return ret;
    }

    rbusMessage_GetInt32(response, &ret);
    if(ret == CCSP_SUCCESS || ret == RBUS_RETURN_CODE_SUCCESS)
    {
        ret = CCSP_SUCCESS;
        rbusMessage_GetInt32(response, size);
        if(*size)
        {
            val = bus_info->mallocfunc(*size*sizeof(parameterInfoStruct_t *));
            memset(val, 0, *size*sizeof(parameterInfoStruct_t *));
            const char *tmpbuf = NULL;

            for(i = 0; i < *size; i++)
            {
                val[i] = bus_info->mallocfunc(sizeof(parameterInfoStruct_t));
                /*CID: 110482 Wrong sizeof argument*/
                memset(val[i], 0, sizeof(parameterInfoStruct_t));
                tmpbuf = NULL;
                rbusMessage_GetString(response, &tmpbuf);
                val[i]->parameterName = bus_info->mallocfunc(strlen(tmpbuf)+1);
                rc = strcpy_s(val[i]->parameterName, (strlen(tmpbuf)+1), tmpbuf);
                ERR_CHK(rc);
                rbusMessage_GetInt32(response, &elemType);
                rbusMessage_GetInt32(response, &accessFlags);
                val[i]->writable = elemType == RBUS_ELEMENT_TYPE_TABLE ? accessFlags & RBUS_ACCESS_ADDROW : accessFlags & RBUS_ACCESS_SET;
                RBUS_LOG("Param [%d] Name = %s, Writable = %d\n", i, val[i]->parameterName, val[i]->writable);                
            }
        }
    }
    else if(ret < CCSP_SUCCESS)
    {
        ret = Rbus2_to_CCSP_error_mapper(ret);
    }
    *parameter = val;
    rbusMessage_Release(response);
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
    if(rbus_enabled == 1)
        return CcspBaseIf_getParameterNames_rbus(bus_handle, dst_component_id, dbus_path, parameterName, nextLevel, size, parameter);

    DBusMessage *message;
    DBusMessage *reply;
    int ret = CCSP_FAILURE;
    dbus_int32_t res ;
    dbus_bool_t btmp ;
    DBusMessageIter iter;
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    parameterInfoStruct_t **val=NULL;
    errno_t rc = -1;

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
                        rc = strcpy_s( val[i]->parameterName, (strlen(str)+1), str);
                        ERR_CHK(rc);
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
    errno_t rc = -1;


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
	                rc = strcpy_s(pRecArray[i].Instance.Name, sizeof(pRecArray[i].Instance.Name), buf);
                    ERR_CHK(rc);
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

/* The Cr component uses the rbus 2.0 api and implements the method Device.CR.RegisterComponent()
 * which other ccsp component must call to register themselves.  Once all required components
 * are registered, Cr will set its property Device.CR.SystemReady to true, which will
 * send a value-change event to all listeners.
 */
static rbusCoreError_t registerComponentWithCr_rbus(const char *component_name)
{
    rbusCoreError_t err = RBUSCORE_SUCCESS;
    rbusMessage request, response;

    rbusMessage_Init(&request);
    /*pack the message the specific way rbus 2.0 requires*/
    rbusMessage_SetInt32(request, 0);
    rbusMessage_SetString(request, "Device.CR.RegisterComponent()"); 
    rbusMessage_SetString(request, NULL);/*object name*/
    rbusMessage_SetInt32(request, 0);/*object type*/
    rbusMessage_SetInt32(request, 1);/*num properties*/
    rbusMessage_SetString(request, "name");/*the name property*/
    rbusMessage_SetInt32(request, RBUS_STRING);/*the value type*/
    rbusMessage_SetBytes(request, (uint8_t const*)component_name, strlen(component_name)+1);/*the actual value*/
    rbusMessage_SetInt32(request, 0);/*object child object count*/

    if((err = rbus_invokeRemoteMethod("Device.CR.RegisterComponent()", METHOD_RPC, request, 1000, &response)) == RBUSCORE_SUCCESS)
    {
        int returnCode = 0;
        rbusMessage_GetInt32(response, &returnCode);
        rbusMessage_Release(response);
        if(returnCode != 0)
        {
            RBUS_LOG_ERR("%s rbus_invokeRemoteMethod for Device.CR.RegisterComponent() for %s got returnCode Err: %d\n", __FUNCTION__, component_name, returnCode);
            err = RBUSCORE_ERROR_GENERAL;
        }
    }
    else
    {
        RBUS_LOG_ERR("%s rbus_invokeRemoteMethod for Device.CR.RegisterComponent() for %s returned with Err: %d\n", __FUNCTION__, component_name, err);
    }
    return err;
}

int CcspBaseIf_registerCapabilities_rbus(
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
    UNREFERENCED_PARAMETER(bus_handle);
    UNREFERENCED_PARAMETER(component_version);
    UNREFERENCED_PARAMETER(dst_component_id);
    UNREFERENCED_PARAMETER(dbus_path);
    UNREFERENCED_PARAMETER(subsystem_prefix);
    int i = 0;
    int failedIndex = 0;
    int ret = CCSP_SUCCESS;
    rbusCoreError_t err = RBUSCORE_SUCCESS;

    for(i = 0; i < size; i++)
    {
        if((err = rbus_addElement(component_name, name_space[i].name_space)) != RBUSCORE_SUCCESS)
        {
            RBUS_LOG_ERR("rbus_addElement: %s Err: %d\n", name_space[i].name_space, err);
            failedIndex = i;
            break;
        }
    }

    if (RBUSCORE_SUCCESS == err)
    {
        if((err = registerComponentWithCr_rbus(component_name)) != RBUSCORE_SUCCESS)
        {
            /* Remove all elements when registration with CR has failed */
            failedIndex = size;
        }
    }

    if (RBUSCORE_SUCCESS != err)
    {
        RBUS_LOG_ERR("unregister all the params are we failed to register a param\n");
        for(i = 0; i < failedIndex; i++)
        {
            if((err = rbus_removeElement(component_name, name_space[i].name_space)) != RBUSCORE_SUCCESS)
            {
                RBUS_LOG_ERR("rbus_removeElement: %s Err: %d\n", name_space[i].name_space, err);
            }
        }
        ret = CCSP_FAILURE;
    }
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
    if(rbus_enabled == 1)
        return CcspBaseIf_registerCapabilities_rbus(bus_handle, dst_component_id, component_name, component_version, dbus_path, subsystem_prefix, name_space, size);

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

int CcspBaseIf_unregisterNamespace_rbus (
    void* bus_handle,
    const char* dst_component_id,
    const char *component_name,
    const char *name_space)
{
    UNREFERENCED_PARAMETER(bus_handle);
    UNREFERENCED_PARAMETER(dst_component_id);
    RBUS_LOG("%s calling rbus_removeElement for %s with component %s\n", __FUNCTION__, name_space, component_name);

    if(RBUSCORE_SUCCESS != rbus_removeElement(component_name, name_space))
    {
        RBUS_LOG("%s rbus_removeElement fails\n", __FUNCTION__);
        return CCSP_FAILURE;
    }

    RBUS_LOG("%s rbus_removeElement succeeds\n", __FUNCTION__);
    return CCSP_SUCCESS;
}

int CcspBaseIf_unregisterNamespace (
    void* bus_handle,
    const char* dst_component_id,
    const char *component_name,
    const char *name_space)
{
    if(rbus_enabled == 1)
        return CcspBaseIf_unregisterNamespace_rbus(bus_handle, dst_component_id, component_name, name_space);

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

int CcspBaseIf_unregisterComponent_rbus (
    void* bus_handle,
    const char* dst_component_id,
    const char *component_name
    )
{
    UNREFERENCED_PARAMETER(bus_handle);
    UNREFERENCED_PARAMETER(dst_component_id);
    UNREFERENCED_PARAMETER(component_name);

    /*
     * When Process/Component calls, CcspBaseIf_registerCapabilities,
     *      In dBus mode: The request goes to CcspCR and the CR creates ComponentDB & NameSpaceDB
     *                    and also tracks the maintains this registration to report SystemReady event.
     *      In rBus mode: No DBs are created. The namespace is directly registered at rbus.
     *                    We were not using CcspCR at all in rBus mode but but in recent times,
     *                    we repurposed it to monitor for registration & send SystemReady event.
     *
     *
     * So when rBus enabled and if a component calls CcspBaseIf_unregisterComponent_rbus(), we must
     * notify CcspCR to mark this component as NOT_REGISTERED for SystemReady event.
     *
     * But In RDK-B system, all the components running as daemon & in while(1),
     * So CcspBaseIf_unregisterComponent() is never get invoked. Only the unit testing needs this.
     *
     * Hence, returning SUCCESS.
     */
    return CCSP_SUCCESS;
}

int CcspBaseIf_unregisterComponent (
    void* bus_handle,
    const char* dst_component_id,
    const char *component_name
)
{
    if(rbus_enabled == 1)
        return CcspBaseIf_unregisterComponent_rbus(bus_handle, dst_component_id, component_name);

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

int CcspBaseIf_discComponentSupportingNamespace_rbus (
    void* bus_handle,
    const char* dst_component_id,
    const char *name_space,
    const char *subsystem_prefix,
    componentStruct_t ***components,
    int *size)
{
    UNREFERENCED_PARAMETER(subsystem_prefix);
    UNREFERENCED_PARAMETER(dst_component_id);
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    componentStruct_t **val=NULL;
    *components = 0;

    if (bus_info == NULL)
    {
        return CCSP_FAILURE;
    }

    RBUS_LOG("%s from %s for the namespace: %s\n", __FUNCTION__, bus_info->component_id, name_space);

#if 0
    /*Handle WebPA and other components that need the component name while doing get/set*/
    if(( _ansc_strcmp(bus_info->component_id, "eRT.com.cisco.spvtg.ccsp.webpaagent" ) == 0) ||
       ( _ansc_strcmp(bus_info->component_id, "eRT.com.cisco.spvtg.ccsp.pam" ) == 0 )       ||
       ( _ansc_strcmp(bus_info->component_id, "ccsp.cisco.spvtg.ccsp.snmp" ) == 0 ))
#endif
    {
        int i = 0;
        char **compName = 0, *pcomp =NULL;
        int num = 0; //only 1 element is passed to get it's component name

        /* Set to 0 before we discover; the discovery will result a proper number */
        *size = 0;
        errno_t rc = -1;

        if(RBUSCORE_SUCCESS == rbus_discoverElementObjects(name_space, &num, &compName))
        {
            /*CID: 126470 Dereference before null check*/
	    if(*compName == NULL)
            {
              RBUS_LOG_ERR("Couldnt find the matching component returning Failure");
              return CCSP_FAILURE;
	    }
            pcomp = compName[0];
            if ((num == 1) && (0 == strcmp(pcomp, "")))
            {
                RBUS_LOG("%s Namespace: %s is not supported\n", __FUNCTION__, name_space);
                return CCSP_CR_ERR_UNSUPPORTED_NAMESPACE;
            }
            else
            {
                *size = num;
                val = bus_info->mallocfunc(num * sizeof(componentStruct_t *));

                for(i = 0; i < num; i++)
                {
                    val[i] = bus_info->mallocfunc(sizeof(componentStruct_t));
                    val[i]->componentName = bus_info->mallocfunc(strlen(compName[i])+1);
                    val[i]->dbusPath = bus_info->mallocfunc(strlen(compName[i])+1);
                    rc = strcpy_s( val[i]->componentName, (strlen(compName[i])+1), compName[i]);
                    ERR_CHK(rc);
                    rc = strcpy_s( val[i]->dbusPath, (strlen(compName[i])+1), compName[i]);
                    ERR_CHK(rc);
                    val[i]->type = ccsp_string;
                    val[i]->remoteCR_name = NULL;
                    val[i]->remoteCR_dbus_path = NULL;
                }
            }

            /* Free the memory */
            for(i = 0; i < num; i++)
            {
                if(compName[i] !=NULL)
                    free(compName[i]);
            }

            if(compName != NULL)
                free(compName);

            *components = val;
            return CCSP_SUCCESS;
        }
        else
        {
            RBUS_LOG_ERR("%s Couldnt find the matching component returning Failure for %s\n", __FUNCTION__, name_space);
            return CCSP_FAILURE;
        }
    }

#if 0
    RBUS_LOG("%s Namespace: %s\n", __FUNCTION__, name_space);//check if its a wildcard expression
    if(name_space[strlen(name_space)-1] == '.')
    {
        char** destinations = NULL;
        int i = 0, ret = 0;
        RBUS_LOG("%s Wildcard expression: %s\n", __FUNCTION__, name_space);
        ret = rbus_discoverWildcardDestinations(name_space, size, &destinations);

        if(ret == RBUSCORE_SUCCESS)
        {
            int sizeSave = *size;
            if(*size == 0)
            {
                /* Possibly the Table entry.. lets avoide one another socket call and return the namespace itself as component name as RBUS supports it.
                 * There might be a error printed in log in rbus mode for a GET in a worst case scenario but thats OKEY.
                 * Because in dbus mode, it makes socket call and determine whether the table entry exists or not and returns failure and the client
                 * will not even call GET. But there is a costly socket call involved here which is avoided in rbus mode.
                 */
                RBUS_LOG("%s Return the given namespace (%s) itself as component name\n", __FUNCTION__, name_space);
                int length = strlen(name_space);
                *size = 1;
                val = bus_info->mallocfunc(*size*sizeof(componentStruct_t *));
                val[0] = bus_info->mallocfunc(sizeof(componentStruct_t));
                val[0]->componentName = bus_info->mallocfunc(length+1);
                val[0]->dbusPath = bus_info->mallocfunc(length+1);
                strcpy( val[0]->componentName, name_space);
                strcpy( val[0]->dbusPath, name_space);
                val[0]->type = ccsp_string;
                val[0]->remoteCR_name = NULL;
                val[0]->remoteCR_dbus_path = NULL;
            }
            else
            {
                val = bus_info->mallocfunc(*size*sizeof(componentStruct_t *));
                for(i = 0; i < *size; i++)
                {
                    RBUS_LOG("Destination %d is %s\n", i, destinations[i]);
                    val[i] = bus_info->mallocfunc(sizeof(componentStruct_t));
                    val[i]->componentName = bus_info->mallocfunc(strlen(destinations[i])+1);
                    val[i]->dbusPath = bus_info->mallocfunc(strlen(destinations[i])+1);
                    strcpy( val[i]->componentName, destinations[i]);
                    strcpy( val[i]->dbusPath, destinations[i]);
                    val[i]->type = ccsp_string;
                    val[i]->remoteCR_name = NULL;
                    val[i]->remoteCR_dbus_path = NULL;
                }
            }
            for(i = 0; i < sizeSave; i++)
                free(destinations[i]);
            if(destinations)
                free(destinations);
        }
        else
        {
            RBUS_LOG_ERR("%s rbus_discoverWildcardDestinations failed for %s Error: %d\n", __FUNCTION__, name_space, ret);
        }
    }
    else
    {
        int length = strlen(name_space);
        RBUS_LOG("%s: Non Wildcard expression: %s\n", __FUNCTION__, name_space);
        *size = 1;
        val = bus_info->mallocfunc(*size*sizeof(componentStruct_t *));
        val[0] = bus_info->mallocfunc(sizeof(componentStruct_t));
        val[0]->componentName = bus_info->mallocfunc(length+1);
        val[0]->dbusPath = bus_info->mallocfunc(length+1);
        strcpy( val[0]->componentName, name_space);
        strcpy( val[0]->dbusPath, name_space);
        val[0]->type = ccsp_string;
        val[0]->remoteCR_name = NULL;
        val[0]->remoteCR_dbus_path = NULL;
    }

    *components = val;
    return CCSP_SUCCESS;
#endif
}

int CcspBaseIf_discComponentSupportingNamespace (
    void* bus_handle,
    const char* dst_component_id,
    const char *name_space,
    const char *subsystem_prefix,
    componentStruct_t ***components,
    int *size)
{
    if(rbus_enabled == 1)
        return CcspBaseIf_discComponentSupportingNamespace_rbus(bus_handle, dst_component_id, name_space, subsystem_prefix, components, size);

    DBusMessage *message;
    DBusMessage *reply;
    int ret = CCSP_FAILURE;
    dbus_int32_t res ;
    DBusMessageIter iter;
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;

    if (bus_handle == NULL)
    {
        return CCSP_FAILURE;
    }

    componentStruct_t **val=NULL;
    *components = 0;
    *size = 0;
    errno_t rc = -1;
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
    
    RBUS_LOG("%s calling CR for : %s\n", __FUNCTION__, name_space);
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
                        rc = strcpy_s( val[i]->componentName, (strlen(str)+1), str);
                        ERR_CHK(rc);
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
                        rc = strcpy_s( val[i]->dbusPath, (strlen(str)+1), str);
                        ERR_CHK(rc);
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
                        rc = strcpy_s( val[i]->remoteCR_name, (strlen(str)+1), str);
                        ERR_CHK(rc);
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
                        rc = strcpy_s( val[i]->remoteCR_dbus_path, (strlen(str)+1), str);
                        ERR_CHK(rc);
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

    /* Print only when extra log enabled */
    if (ret != CCSP_SUCCESS)
        RBUS_LOG("%s failed for Namespace: %s with error code %d\n", __FUNCTION__, name_space, ret);

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
    if(rbus_enabled == 1)
    {
        RBUS_LOG_ERR("%s unsupported function in rbus is called, please check\n", __FUNCTION__);
        return CCSP_FAILURE;
    }
    DBusMessage *message;
    DBusMessage *reply;
    int ret = CCSP_FAILURE;
    dbus_int32_t res ;
    DBusMessageIter iter;
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    errno_t rc = -1;

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
                rc = strcpy_s( val->componentName, (strlen(str)+1), str);
                ERR_CHK(rc);
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
                rc = strcpy_s( val->dbusPath, (strlen(str)+1), str);
                ERR_CHK(rc);
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
                rc = strcpy_s( val->remoteCR_name, (strlen(str)+1), str);
                ERR_CHK(rc);
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
                rc = strcpy_s( val->remoteCR_dbus_path, (strlen(str)+1), str);
                ERR_CHK(rc);
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

int CcspBaseIf_discNamespaceSupportedByComponent_rbus (
    void* bus_handle,
    const char* dst_component_id,
    const char *component_name,
    name_spaceType_t ***name_space,
    int *size
    )
{
    UNREFERENCED_PARAMETER(dst_component_id);
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    char** elements;
    int num_elements;
    int ret = CCSP_FAILURE;
    name_spaceType_t **val = NULL;
    *name_space = 0;
    *size = 0;
    errno_t rc = -1;

    RBUS_LOG("calling %s for %s \n", __FUNCTION__, component_name);
    ret = rbus_discoverObjectElements(component_name, &num_elements, &elements);

    if(ret == RBUSCORE_SUCCESS)
    {
        int i;
        RBUS_LOG("%s returns num_elements as %d\n", __FUNCTION__, num_elements);

        if(num_elements)
        {
            val = bus_info->mallocfunc(num_elements * sizeof(name_spaceType_t *));
            memset(val, 0, num_elements * sizeof(name_spaceType_t *));

            for(i = 0; i < num_elements; i++)
            {
                val[i] = bus_info->mallocfunc(sizeof(name_spaceType_t));
                val[i]->name_space = bus_info->mallocfunc(strlen(elements[i])+1);
                rc = strcpy_s(val[i]->name_space, (strlen(elements[i])+1), elements[i]);
                ERR_CHK(rc);
                RBUS_LOG("%s returns name_space %d as %s\n", __FUNCTION__, i, val[i]->name_space);
            }
        }

        RBUS_LOG("exiting %s\n", __FUNCTION__);
        for(i = 0; i < num_elements; i++)
            free(elements[i]);
        free(elements);
        *name_space = val;
        *size = num_elements;
        return CCSP_SUCCESS;
    }

    return CCSP_FAILURE;
}

int CcspBaseIf_discNamespaceSupportedByComponent (
    void* bus_handle,
    const char* dst_component_id,
    const char *component_name,
    name_spaceType_t ***name_space,
    int *size
)
{
    if(rbus_enabled == 1)
    {
        return CcspBaseIf_discNamespaceSupportedByComponent_rbus(bus_handle, dst_component_id, component_name, name_space, size);
    }

    DBusMessage *message;
    DBusMessage *reply;
    int ret = CCSP_FAILURE;
    dbus_int32_t res ;
    DBusMessageIter iter;
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;

    name_spaceType_t **val=NULL;
    *name_space = 0;
    *size = 0;
    errno_t rc = -1;
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
                        rc = strcpy_s( val[i]->name_space, (strlen(str)+1), str);
                        ERR_CHK(rc);
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

int CcspBaseIf_getRegisteredComponents_rbus(
    void* bus_handle,
    const char* dst_component_id,
    registeredComponent_t ***components,
    int *size
    )
{
    UNREFERENCED_PARAMETER(dst_component_id);
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    registeredComponent_t **val = NULL;
    *components = 0;
    *size = 0;
    int num_components = 0;
    char** component_names;
    errno_t rc = -1;

    int ret = rbus_discoverRegisteredComponents(&num_components, &component_names);

    if(ret == RBUSCORE_SUCCESS)
    {
        int i;
        RBUS_LOG("%s returns size as %d\n", __FUNCTION__, num_components);

        if(num_components)
        {
            val = bus_info->mallocfunc(num_components * sizeof(registeredComponent_t *));
            memset(val, 0, num_components * sizeof(registeredComponent_t *));

            for(i = 0; i < num_components; i++)
            {
                val[i] = bus_info->mallocfunc(sizeof(registeredComponent_t));
                val[i]->componentName = NULL;
                val[i]->dbusPath = NULL;
                val[i]->subsystem_prefix = NULL;
                val[i]->componentName = bus_info->mallocfunc(strlen(component_names[i])+1);
                rc = strcpy_s(val[i]->componentName, (strlen(component_names[i])+1), component_names[i]);
                ERR_CHK(rc);
                RBUS_LOG("%s returns component %d as %s\n", __FUNCTION__, i, val[i]->componentName);
            }
        }
        for(i = 0; i < num_components; i++)
            free(component_names[i]);
        free(component_names);
    }
    *components = val;
    *size = num_components;
    return CCSP_SUCCESS;
}

int CcspBaseIf_getRegisteredComponents (
    void* bus_handle,
    const char* dst_component_id,
    registeredComponent_t ***components,
    int *size
)
{
    if(rbus_enabled == 1)
        return CcspBaseIf_getRegisteredComponents_rbus(bus_handle, dst_component_id, components, size);

    DBusMessage *message;
    DBusMessage *reply;
    int ret = CCSP_FAILURE;
    dbus_int32_t res ;
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;

    registeredComponent_t **val=NULL;
    *components = 0;
    *size = 0;
    errno_t rc = -1;
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
                        rc = strcpy_s( val[i]->componentName, (strlen(str)+1), str);
                        ERR_CHK(rc);
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
                        rc = strcpy_s( val[i]->dbusPath, (strlen(str)+1), str);
                        ERR_CHK(rc);
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
                        rc = strcpy_s( val[i]->subsystem_prefix, (strlen(str)+1), str);
                        ERR_CHK(rc);
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
    if(rbus_enabled == 1)
    {
        RBUS_LOG_ERR("%s unsupported function in rbus is called, please check\n", __FUNCTION__);
        return CCSP_FAILURE;
    }
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
    if(rbus_enabled == 1)
    {
        RBUS_LOG_ERR("%s unsupported function in rbus is called, please check\n", __FUNCTION__);
        return CCSP_FAILURE;
    }
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

#if 0
void update_component_info(component_info *compInfo)
{
    ANSC_HANDLE                     pFileHandle        = NULL;
    char*                           pXMLContent        = NULL;
    char*                           pBackContent       = NULL;
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode           = (PANSC_XML_DOM_NODE_OBJECT)NULL;
    PANSC_XML_DOM_NODE_OBJECT       pListNode          = (PANSC_XML_DOM_NODE_OBJECT)NULL;
    PANSC_XML_DOM_NODE_OBJECT       pChildNode         = (PANSC_XML_DOM_NODE_OBJECT)NULL;
    PANSC_XML_DOM_NODE_OBJECT       pComponentNode     = (PANSC_XML_DOM_NODE_OBJECT)NULL;
    char                            buffer[512]        = { 0 };
    ULONG                           uLength            = 512;
    ULONG                           uFileLength        = 0;
    ULONG                           uBufferSize        = 0;
    USHORT                          uComponentCount    = 0;
    errno_t                         rc                 = -1;

    /* load from the file */
    if (access(CCSP_ETHWAN_ENABLE, F_OK) == 0)
    {
        pFileHandle =
            AnscOpenFile
            (
             CCSP_CR_ETHWAN_DEVICE_PROFILE_XML_FILE,
             ANSC_FILE_O_BINARY | ANSC_FILE_O_RDONLY,
             ANSC_FILE_S_IREAD
            );
    }
    else
    {
        pFileHandle =
            AnscOpenFile
            (
             CCSP_CR_DEVICE_PROFILE_XML_FILE,
             ANSC_FILE_O_BINARY | ANSC_FILE_O_RDONLY,
             ANSC_FILE_S_IREAD
            );
    }

    if( pFileHandle == NULL)
    {
        AnscTrace("Failed to load the file : '%s'\n", CCSP_CR_DEVICE_PROFILE_XML_FILE);
        return;
    }

    uFileLength = AnscGetFileSize( pFileHandle);
    pXMLContent = (char*)AnscAllocateMemory( uFileLength + 8);

    if( pXMLContent == NULL)
    {
        AnscCloseFile(pFileHandle); /*RDKB-6901, CID-33521, free unused resources before exit */
        return;
    }

    uBufferSize = uFileLength + 8;
    if( AnscReadFile( pFileHandle, pXMLContent, &uBufferSize) != ANSC_STATUS_SUCCESS)
    {
        AnscFreeMemory(pXMLContent);
        AnscCloseFile(pFileHandle); /*RDKB-6901, CID-33521, free unused resources before exit */
        return;
    }

    if( pFileHandle != NULL)
    {
        AnscCloseFile(pFileHandle);
    }

    /* parse the XML content */
    pBackContent = pXMLContent;
    pXmlNode = (PANSC_XML_DOM_NODE_OBJECT)
        AnscXmlDomParseString((ANSC_HANDLE)NULL, (PCHAR*)&pXMLContent, uBufferSize);
    AnscFreeMemory(pBackContent);

    if( pXmlNode == NULL)
    {
        AnscTraceWarning(("Failed to parse the CR profile file.\n"));
        return;
    }

    /* get the component array node */
    pListNode = (PANSC_XML_DOM_NODE_OBJECT) AnscXmlDomNodeGetChildByName(pXmlNode, "components");
    /* get the name */
    if(pListNode != NULL)
    {
        pChildNode = (PANSC_XML_DOM_NODE_OBJECT) AnscXmlDomNodeGetHeadChild(pListNode);
        compInfo->list = (char **)AnscAllocateMemory(sizeof(char*) * AnscQueueQueryDepth(&pListNode->ChildNodeQueue));

        while(pChildNode != NULL && compInfo->list != NULL)
        {
            /* load component information */
            pComponentNode = (PANSC_XML_DOM_NODE_OBJECT) pChildNode->GetChildByName(pChildNode, "name");
            uLength = sizeof(buffer)-1;

            if( pComponentNode != NULL && pComponentNode->GetDataString(pComponentNode, NULL, buffer, &uLength) == ANSC_STATUS_SUCCESS && uLength > 0)
            {
                compInfo->list[compInfo->size] = (char*)AnscAllocateMemory(AnscSizeOfString(buffer) + 1);
                buffer[uLength] = '\0';
                rc = strcpy_s(compInfo->list[compInfo->size], (AnscSizeOfString(buffer) + 1), (char*)buffer);
                ERR_CHK(rc);
                RBUS_LOG("%s component name read is %s \n",__FUNCTION__, compInfo->list[compInfo->size]);
                compInfo->size++;
            }
            pChildNode = (PANSC_XML_DOM_NODE_OBJECT) AnscXmlDomNodeGetNextChild(pListNode, pChildNode);
        }
    }
    else
    {
        RBUS_LOG("\"components\" list is empty\n");
    }
    if( pXmlNode != NULL)
    {
        pXmlNode->Remove(pXmlNode);
    }
}
#endif

/* The Cr component uses rbus 2.0 api and registers the property "Device.CR.SystemReady" 
 * which has a boolean value indicating if all required components have registered with Cr or not.
 */
int CcspBaseIf_isSystemReady_rbus(
    void* bus_handle,
    const char* dst_component_id,
    dbus_bool *val
    )
{
    int ret = CCSP_SUCCESS;
    char* parameterNames[1] = {"Device.CR.SystemReady"};
    int size;
    parameterValStruct_t** value = 0;

    *val = 0;

    ret = CcspBaseIf_getParameterValues_rbus(
              bus_handle,
              dst_component_id,
              NULL,
              parameterNames,
              1,
              &size,
              &value );

    if(ret == CCSP_SUCCESS  && size >= 1)
    {
        CcspTraceDebug(("%s %s is %s\n", __FUNCTION__, parameterNames[0], value[0]->parameterValue));
        if(strcasecmp(value[0]->parameterValue, "true") == 0 )
            *val = 1;
        else
            *val = 0;
        free_parameterValStruct_t(bus_handle, size, value);
    }
    else
    {
        CcspTraceError(("%s CcspBaseIf_getParameterValues_rbus %s failed ret=%d\n", __FUNCTION__, parameterNames[0], ret));
    }

    return ret;
}

int CcspBaseIf_isSystemReady (
    void* bus_handle,
    const char* dst_component_id,
    dbus_bool *val
)
{
    if(rbus_enabled == 1)
        return CcspBaseIf_isSystemReady_rbus(bus_handle, dst_component_id, val);

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

int CcspBaseIf_requestSessionID_rbus (
    void* bus_handle,
    const char* dst_component_id,
    int priority,
    int *sessionID)
{
    UNREFERENCED_PARAMETER(dst_component_id);
    UNREFERENCED_PARAMETER(priority);
    rbusError_t rc = RBUS_ERROR_SUCCESS;
    rc = rbus_createSession(bus_handle, (uint32_t *)sessionID);
    if(rc == RBUS_ERROR_SUCCESS)
    {
       RBUS_LOG("Got new session id %d\n", *sessionID);
       return CCSP_SUCCESS;
    }
    else
    {
        RBUS_LOG_ERR("RPC with session manager failed.\n");
    }
    return CCSP_FAILURE;
}

int CcspBaseIf_requestSessionID (
    void* bus_handle,
    const char* dst_component_id,
    int priority,
    int *sessionID
)
{
    if(rbus_enabled == 1)
        return CcspBaseIf_requestSessionID_rbus(bus_handle, dst_component_id, priority, sessionID);

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

int CcspBaseIf_getCurrentSessionID_rbus (
    void* bus_handle,
    const char* dst_component_id,
    int *priority,
    int *sessionID
    )
{
    UNREFERENCED_PARAMETER(dst_component_id);
    UNREFERENCED_PARAMETER(priority);
    rbusError_t result = RBUS_ERROR_SUCCESS;
    result = rbus_getCurrentSession(bus_handle, (uint32_t *)sessionID);
    if(result == RBUS_ERROR_SUCCESS)
    {
       RBUS_LOG("Got new session id %d\n", *sessionID);
       return CCSP_SUCCESS;
    }
    else
    {
        RBUS_LOG_ERR("RPC with session manager failed.\n");
    }
    return CCSP_FAILURE;
}

int CcspBaseIf_getCurrentSessionID (
    void* bus_handle,
    const char* dst_component_id,
    int *priority,
    int *sessionID
)
{
    if(rbus_enabled == 1)
        return CcspBaseIf_getCurrentSessionID_rbus(bus_handle, dst_component_id, priority, sessionID);

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

int CcspBaseIf_informEndOfSession_rbus (
    void* bus_handle,
    const char* dst_component_id,
    int sessionID
    )
{
    UNREFERENCED_PARAMETER(dst_component_id);
    rbusError_t result = RBUS_ERROR_SUCCESS;
    result = rbus_closeSession(bus_handle, sessionID);
    if(result == RBUS_ERROR_SUCCESS)
    {
       RBUS_LOG("Successfully ended session %d.\n", sessionID);
       return CCSP_SUCCESS;
    }
    else
    {
        RBUS_LOG_ERR("RPC with session manager failed.\n");
    }
    return CCSP_FAILURE;
}

int CcspBaseIf_informEndOfSession (
    void* bus_handle,
    const char* dst_component_id,
    int sessionID
)
{
    if(rbus_enabled == 1)
        return CcspBaseIf_informEndOfSession_rbus (bus_handle, dst_component_id, sessionID);

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

/* For rbus2.0 based component, Health is a parameter you get. */
int CcspBaseIf_getHealth_rbus2(
    void* bus_handle,
    const char* dst_component_id,
    int *health
    )
{
    int ret = CCSP_SUCCESS;
    char* parameterNames[1];
    char param[256];
    int size;
    parameterValStruct_t** value = 0;

    *health = 0;

    /*strip any eRT. prefix*/
    snprintf(param, 256, "%s.Health", dst_component_id + (strncmp(dst_component_id, "eRT.", 4) ? 0 : 4));
    parameterNames[0] = param;

    ret = CcspBaseIf_getParameterValues_rbus(
              bus_handle,
              dst_component_id,
              NULL,
              parameterNames,
              1,
              &size,
              &value );

    if(ret == CCSP_SUCCESS  && size >= 1)
    {
        CcspTraceDebug(("%s %s is %s\n", __FUNCTION__, param, value[0]->parameterValue));
        *health = atoi(value[0]->parameterValue);
        free_parameterValStruct_t(bus_handle, size, value);
    }
    else
    {
        CcspTraceError(("%s CcspBaseIf_getParameterValues_rbus %s failed ret=%d\n", __FUNCTION__, param, ret));
    }

    return ret;
}


int CcspBaseIf_getHealth_rbus(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    int *health)
{
    UNREFERENCED_PARAMETER(dbus_path);

    /*call CcspBaseIf_getHealth_rbus2 for any rbus2.0 based components*/
    if(strstr(dst_component_id, CCSP_DBUS_INTERFACE_CR))
    {
        return CcspBaseIf_getHealth_rbus2(bus_handle, dst_component_id, health);
    }
    else
    {
        int ret = CCSP_FAILURE;
        int32_t status = 0;
        rbusMessage request, response;

        rbusMessage_Init(&request);
        if((ret = Rbus_to_CCSP_error_mapper(rbus_invokeRemoteMethod(dst_component_id, METHOD_GETHEALTH, request, CcspBaseIf_timeout_rbus, &response))) != CCSP_Message_Bus_OK)
        {
            RBUS_LOG_ERR("%s rbus_invokeRemoteMethod: Err: %d\n", __FUNCTION__, ret);
            return CCSP_FAILURE;
        }

        rbusMessage_GetInt32(response, &status);
        RBUS_LOG("exiting CcspBaseIf_getHealth_rbus with status %d\n", status);
        *health = (int)status;
        rbusMessage_Release(response);
    }
    return CCSP_SUCCESS;
}

int CcspBaseIf_getHealth(
    void* bus_handle,
    const char* dst_component_id,
    char* dbus_path,
    int *health)
{
    if(rbus_enabled == 1)
        return CcspBaseIf_getHealth_rbus(bus_handle, dst_component_id, dbus_path, health);

    DBusMessage *message= NULL;
    DBusMessage *reply = NULL;
    int ret = CCSP_FAILURE;
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
            dbus_message_unref (message);
            return CCSP_SUCCESS;
        }
    }
    dbus_message_unref (message);
    return CCSP_FAILURE;
}

BOOLEAN waitConditionReady(void* hMBusHandle, const char* dst_component_id, char* dbus_path, char *src_component_id){
    #define MAX_WAIT_TIME 10
    #define TIME_INTERVAL 2000 // 2s
//    #define MAX_WAIT_TIME 5
  //  #define TIME_INTERVAL 200 //
    #define CCSP_COMMON_COMPONENT_HEALTH_Green 3
    int times = 0;
    int ret   = 0;
    int health = 0;
    printf("%s start to check %s status\n", src_component_id, dst_component_id);
    CcspTraceInfo(("%s start to check %s status\n", src_component_id, dst_component_id));
    while(times++ < MAX_WAIT_TIME)
    {
        //to call TAD dbus message to check TAD is ready
        ret = CcspBaseIf_getHealth(hMBusHandle, dst_component_id, dbus_path, &health);
	    if(health != CCSP_COMMON_COMPONENT_HEALTH_Green || CCSP_SUCCESS != ret)//CCSP_COMMON_COMPONENT_HEALTH_Green
        {
            CCSP_Msg_SleepInMilliSeconds(TIME_INTERVAL);
            printf("%s try to check %s health for %d times, health is %d\n", src_component_id, dst_component_id, times, health);
            CcspTraceInfo(("%s try to check %s health for %d times, health is %d\n", src_component_id, dst_component_id, times, health));
        }
        else
        {
            printf("%s is ready, %s continue\n",dst_component_id, src_component_id);
            CcspTraceInfo(("%s is ready, %s continue\n",dst_component_id, src_component_id));
            return true;
        }
    }
    printf("Time is out! %s is not ready, health status is %d, but %s continue\n", dst_component_id, health, src_component_id);
    CcspTraceError(("Time is out! %s is not ready, health status is %d, but %s continue\n", dst_component_id, health, src_component_id));
    return false;
}

int CcspBaseIf_busCheck(
    void* bus_handle,
    const char* dst_component_id
)
{
    if(rbus_enabled == 1)
    {
        RBUS_LOG("rbus %s supports this function in a differnt way\n", __FUNCTION__);
        return CCSP_SUCCESS;
    }
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
    if(rbus_enabled == 1)
    {
        RBUS_LOG("rbus %s supports this function in a different way\n", __FUNCTION__);
        return CCSP_SUCCESS;
    }
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
    if(rbus_enabled == 1)
    {
        RBUS_LOG("rbus %s supports this function in a different way\n", __FUNCTION__);
        return CCSP_SUCCESS;
    }
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

        if ( (unsigned int)len > strlen(parent_name) )
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

            if ( (unsigned int)len > strlen(parent_name) )
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

    if ( (unsigned int)len > strlen(parent_name) )
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
    errno_t rc = -1;
    rc = sprintf_s(buf[0],sizeof(buf[0]), "%s%s.Name",CCSP_NAME_PREFIX,component_name);
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }
    name_space[0].name_space = buf[0];
    name_space[0].dataType = ccsp_string;

    rc = sprintf_s(buf[1],sizeof(buf[1]),"%s%s.Version",CCSP_NAME_PREFIX,component_name);
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }
    name_space[1].name_space = buf[1];
    name_space[1].dataType = ccsp_int;

    rc = sprintf_s(buf[2],sizeof(buf[2]),"%s%s.Author",CCSP_NAME_PREFIX,component_name);
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }
    name_space[2].name_space = buf[2];
    name_space[2].dataType = ccsp_string;

    rc = sprintf_s(buf[3],sizeof(buf[3]),"%s%s.Health",CCSP_NAME_PREFIX,component_name);
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }
    name_space[3].name_space = buf[3];
    name_space[3].dataType = ccsp_string;

    rc = sprintf_s(buf[4],sizeof(buf[4]),"%s%s.State",CCSP_NAME_PREFIX,component_name);
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }
    name_space[4].name_space = buf[4];
    name_space[4].dataType = ccsp_int;


    rc = sprintf_s(buf[5],sizeof(buf[5]),"%s%s.Logging.Enable",CCSP_NAME_PREFIX,component_name);
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }
    name_space[5].name_space = buf[5];
    name_space[5].dataType = ccsp_boolean;


    rc = sprintf_s(buf[6],sizeof(buf[6]),"%s%s.Logging.LogLevel",CCSP_NAME_PREFIX,component_name);
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }
    name_space[6].name_space = buf[6];
    name_space[6].dataType = ccsp_int;


    rc = sprintf_s(buf[7],sizeof(buf[7]),"%s%s.Memory.MinUsage",CCSP_NAME_PREFIX,component_name);
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }
    name_space[7].name_space = buf[7];
    name_space[7].dataType = ccsp_int;


    rc = sprintf_s(buf[8],sizeof(buf[8]),"%s%s.Memory.MaxUsage",CCSP_NAME_PREFIX,component_name);
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }
    name_space[8].name_space = buf[8];
    name_space[8].dataType = ccsp_int;

    rc = sprintf_s(buf[9],sizeof(buf[9]),"%s%s.Memory.Consumed",CCSP_NAME_PREFIX,component_name);
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }
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

int CcspBaseIf_SendparameterValueChangeSignal_rbus (
    void* bus_handle,
    parameterSigStruct_t *val,
    int size
    )
{
    int32_t utmp = 0, tmp = 0;
    int i = 0;
    rbusCoreError_t err = RBUSCORE_SUCCESS;
    rbusMessage request;
    rbusMessage response;

    rbusMessage_Init(&request);
    rbusMessage_SetInt32(request, size);

    for(i = 0; i < size; i++)
    {
        rbusMessage_SetString(request, val[i].parameterName);
        rbusMessage_SetString(request, val[i].oldValue);
        rbusMessage_SetString(request, val[i].newValue);
        tmp = val[i].type;
        rbusMessage_SetInt32(request, tmp);
        rbusMessage_SetString(request, val[i].subsystem_prefix);
        utmp = val[i].writeID;
        rbusMessage_SetInt32(request, (int32_t)utmp);
    }

    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    err = rbus_invokeRemoteMethod("eRT.com.cisco.spvtg.ccsp.tr069pa", "parameterValueChangeSignal", request, CcspBaseIf_timeout_rbus, &response);
    if(err != RBUSCORE_SUCCESS)
    {
        RBUS_LOG_ERR("%s : rbus_publishEvent returns Err: %d for bus_info->component_id =%s\n", __FUNCTION__, err,bus_info->component_id);
        return CCSP_FAILURE;
    }

    rbusMessage_Release(response);
    return CCSP_SUCCESS;
}

int CcspBaseIf_SendparameterValueChangeSignal (
    void* bus_handle,
    parameterSigStruct_t *val,
    int size
)
{
    if(rbus_enabled == 1)
        return CcspBaseIf_SendparameterValueChangeSignal_rbus(bus_handle, val, size);

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
    if(rbus_enabled == 1)
        return CcspBaseIf_SendSignal_rbus(bus_handle, "transferCompleteSignal");
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
    if(rbus_enabled == 1)
        return CcspBaseIf_SendSignal_rbus(bus_handle, "transferFailedSignal");
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
int CcspBaseIf_SenddeviceProfileChangeSignal_rbus (
    void* bus_handle,
    char *component_name,
    char *component_dbus_path,
    dbus_bool isAvailable
)
{
    int ret = CCSP_SUCCESS;
    rbusCoreError_t err = RBUSCORE_SUCCESS;

    rbusMessage request;
    rbusMessage_Init(&request);
    rbusMessage_SetString(request,component_name);
    rbusMessage_SetString(request,component_dbus_path);
    rbusMessage_SetInt32(request, (int32_t)isAvailable);

    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    RBUS_LOG("%s : rbus_publishEvent object_name: %s  :: event_name : %s :: \n", __FUNCTION__, bus_info->component_id, "deviceProfileChangeSignal");
    err = rbus_publishEvent(bus_info->component_id,"deviceProfileChangeSignal",request);
    if (err != RBUSCORE_SUCCESS)
    {
        RBUS_LOG_ERR("%s : rbus_publishEvent returns Err: %d\n", __FUNCTION__, err);
        ret = CCSP_FAILURE;
    }
    rbusMessage_Release(request);

    return ret;
}


int CcspBaseIf_SenddeviceProfileChangeSignal (
    void* bus_handle,
    char *component_name,
    char *component_dbus_path,
    unsigned char isAvailable
)
{
    if(rbus_enabled == 1)
    {
        return CcspBaseIf_SenddeviceProfileChangeSignal_rbus(bus_handle,component_name,component_dbus_path,isAvailable); 
    }
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

int CcspBaseIf_SendcurrentSessionIDSignal_rbus (
    void* bus_handle,
    int priority,
    int sessionID
    )
{
    int ret = CCSP_SUCCESS;
    rbusCoreError_t err = RBUSCORE_SUCCESS;

    rbusMessage request;
    rbusMessage_Init(&request);

    rbusMessage_SetInt32(request, (int32_t)priority);
    rbusMessage_SetInt32(request, (int32_t)sessionID);

    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    err = rbus_publishEvent( bus_info->component_id, "currentSessionIDSignal", request);

    if (err != RBUSCORE_SUCCESS)
    {
        RBUS_LOG_ERR("%s : rbus_publishEvent returns Err: %d\n", __FUNCTION__, err);
        ret = CCSP_FAILURE;
    }
    rbusMessage_Release(request);

    return ret;
}

int CcspBaseIf_SendcurrentSessionIDSignal (
    void* bus_handle,
    int priority,
    int sessionID
)
{
    if(rbus_enabled == 1)
        return CcspBaseIf_SendcurrentSessionIDSignal_rbus(bus_handle, priority, sessionID);

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

void* CcspBaseIf_SendTelemetryDataSignal_rbus(void* telemetry_data)
{
  int ret;
  rbusMessage out;
  rbusMessage response;
  rbusMessage_Init(&out);
  char* Telemetry_data = (char*)telemetry_data;
  rbusMessage_SetString(out, Telemetry_data);
  if((ret = Rbus_to_CCSP_error_mapper(rbus_invokeRemoteMethod("eRT.com.cisco.spvtg.ccsp.telemetry", CCSP_TELEMETRY_DATA_SIGNAL,out, CcspBaseIf_timeout_rbus, &response))) != CCSP_Message_Bus_OK)
  {
            RBUS_LOG_ERR("%s rbus_invokeRemoteMethod for telemetry data:%s returns with Err: %d\n", __FUNCTION__,Telemetry_data, ret);
  }
  rbusMessage_Release(response);
  return NULL;
}

int CcspBaseIf_SendTelemetryDataSignal (
    void* bus_handle,
    char* telemetry_data
)
{
    if(rbus_enabled == 1) 
    {
        int ret = CCSP_SUCCESS;
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, CcspBaseIf_SendTelemetryDataSignal_rbus, (void*)telemetry_data); 
        pthread_join(thread_id, NULL); 
        return ret;
    }
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


    message = dbus_message_new_signal (CCSP_DBUS_PATH_EVENT, CCSP_DBUS_INTERFACE_EVENT, "telemetryDataSignal" );

    if(!message)
        return CCSP_ERR_MEMORY_ALLOC_FAIL;

    dbus_message_append_args (message, DBUS_TYPE_STRING, &telemetry_data, DBUS_TYPE_INVALID);

    dbus_connection_send (conn, message, NULL);

    dbus_message_unref (message);
    return CCSP_SUCCESS;
}

#if 0
int CcspBaseIf_WebConfigSignal_rbus (
    void* bus_handle,
    char* webconfig
)
{
    UNREFERENCED_PARAMETER(bus_handle);
    int ret = CCSP_FAILURE;
    rbusMessage request, response;

    rbusMessage_Init(&request);
    rbusMessage_SetString(request,webconfig);
    RBUS_LOG("%s : rbus_publishEvent :: event_name : %s :: \n", __FUNCTION__, "webconfigSignal");
    if((ret = Rbus_to_CCSP_error_mapper(rbus_invokeRemoteMethod("eRT.com.cisco.spvtg.ccsp.webpaagent", "webconfigSignal", request, CcspBaseIf_timeout_rbus, &response))) != CCSP_Message_Bus_OK)
    {
        RBUS_LOG_ERR("%s rbus_invokeRemoteMethod for webconfigSignal failed & returns with Err: %d\n", __FUNCTION__, ret);
        ret = CCSP_FAILURE;
    }
    else
    {
        rbusMessage_Release(response);
    }
    return ret;
}

#endif 
int CcspBaseIf_WebConfigSignal (
    void* bus_handle,
    char* webconfig
)
{

    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    DBusMessage *message;
    int i;
    DBusConnection *conn = NULL ;

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


    message = dbus_message_new_signal (CCSP_DBUS_PATH_EVENT, CCSP_DBUS_INTERFACE_EVENT, "webconfigSignal" );

    if(!message)
        return CCSP_ERR_MEMORY_ALLOC_FAIL;

    dbus_message_append_args (message, DBUS_TYPE_STRING, &webconfig, DBUS_TYPE_INVALID);

    dbus_connection_send (conn, message, NULL);

    dbus_message_unref (message);
    return CCSP_SUCCESS;
}

int CcspBaseIf_SendSignal(
    void * bus_handle,
    char *event
)
{
    if(rbus_enabled == 1)
    {
        return CcspBaseIf_SendSignal_rbus(bus_handle, event);
    }
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

#if 0
int CcspBaseIf_SendSignal_WithData_rbus(
    void * bus_handle,
    char *eventName,
    char* eventData
)
{
    CcspTraceInfo(("%s --in %s %s\n", __FUNCTION__, eventName, eventData));

    rbusEvent_t event;
    rbusObject_t data;
    rbusValue_t value;
    rbusError_t ret = RBUS_ERROR_SUCCESS;

    rbusValue_Init(&value);
    rbusValue_SetString(value, eventData);
    rbusObject_Init(&data, NULL);
    rbusObject_SetValue(data, eventName, value);

    event.name = eventName;
    event.data = data;
    event.type = RBUS_EVENT_GENERAL;
    ret = rbusEvent_Publish(bus_handle, &event);
    if(ret != RBUS_ERROR_SUCCESS) {
        CcspTraceInfo(("provider: rbusEvent_Publish Event1 failed: %d\n", ret));
    }

    rbusValue_Release(value);
    CcspTraceInfo(("%s --out\n", __FUNCTION__));
    return Rbus2_to_CCSP_error_mapper(ret);
}

#endif

int CcspBaseIf_TunnelStatus_Tr181_Signal_rbus (
    void* bus_handle,
    char* TunnelStatus
)
{
    UNREFERENCED_PARAMETER(bus_handle);
    int ret = CCSP_FAILURE;
    rbusMessage request, response;

    rbusMessage_Init(&request);
    rbusMessage_SetString(request,TunnelStatus);
    RBUS_LOG("%s : rbus_publishEvent :: event_name : %s :: \n", __FUNCTION__,
                                                "Device.X_COMCAST-COM_GRE.Tunnel.1.TunnelStatus");
    if((ret = Rbus_to_CCSP_error_mapper(rbus_invokeRemoteMethod("eRT.com.cisco.spvtg.ccsp.wifi",
                                     "Device.X_COMCAST-COM_GRE.Tunnel.1.TunnelStatus", request,
                                      CcspBaseIf_timeout_rbus, &response))) != CCSP_Message_Bus_OK)
    {
        RBUS_LOG_ERR("%s rbus_invokeRemoteMethod for TunnelStatus failed & returns with Err: %d\n",
                                                                                __FUNCTION__, ret);
        ret = CCSP_FAILURE;
    }
    rbusMessage_Release(response);
    return ret;
}

int CcspBaseIf_TunnelStatusSignal_rbus (
    void* bus_handle,
    char* TunnelStatus
)
{
    UNREFERENCED_PARAMETER(bus_handle);
    int ret = CCSP_FAILURE;
    rbusMessage request, response;

    rbusMessage_Init(&request);
    rbusMessage_SetString(request,TunnelStatus);
    RBUS_LOG("%s : rbus_publishEvent :: event_name : %s :: \n", __FUNCTION__, "TunnelStatus");
    if((ret = Rbus_to_CCSP_error_mapper(rbus_invokeRemoteMethod("eRT.com.cisco.spvtg.ccsp.wifi", "TunnelStatus", request, CcspBaseIf_timeout_rbus, &response))) != CCSP_Message_Bus_OK)
    {
        RBUS_LOG_ERR("%s rbus_invokeRemoteMethod for TunnelStatus failed & returns with Err: %d\n", __FUNCTION__, ret);
        ret = CCSP_FAILURE;
    }
    rbusMessage_Release(response);
    return ret;
}

int CcspBaseIf_WifiDbStatusSignal_rbus (
    void* bus_handle,
    char* WifiDbStatus
)
{
    UNREFERENCED_PARAMETER(bus_handle);
    int ret = CCSP_FAILURE;
    rbusMessage request, response;

    rbusMessage_Init(&request);
    rbusMessage_SetString(request,WifiDbStatus);
    RBUS_LOG("%s : rbus_publishEvent :: event_name : %s :: \n", __FUNCTION__, "WifiDbStatus");
    if((ret = Rbus_to_CCSP_error_mapper(rbus_invokeRemoteMethod("eRT.com.cisco.spvtg.ccsp.wifi", "WifiDbStatus", request, CcspBaseIf_timeout_rbus, &response))) != CCSP_Message_Bus_OK)
    {
        RBUS_LOG_ERR("%s rbus_invokeRemoteMethod for WifiDbStatus failed & returns with Err: %d\n", __FUNCTION__, ret);
        ret = CCSP_FAILURE;
    }
    rbusMessage_Release(response);
    return ret;
}

int CcspBaseIf_SendSignal_WithData(
    void * bus_handle,
    char *event,
    char* data
)
{
    CcspTraceInfo(("Entering %s, event is %s\n", __FUNCTION__, event));

    if(rbus_enabled == 1)
    {
        if (strcmp(event, "TunnelStatus") == 0 )
        {
            return CcspBaseIf_TunnelStatusSignal_rbus(bus_handle,data);

        }
        else if(strcmp(event, "WifiDbStatus") == 0)
        {
            return CcspBaseIf_WifiDbStatusSignal_rbus(bus_handle,data);
        }
        else if(strcmp(event, "Device.X_COMCAST-COM_GRE.Tunnel.1.TunnelStatus") == 0)
        {
            return CcspBaseIf_TunnelStatus_Tr181_Signal_rbus(bus_handle,data);
        }
        else
        {
            RBUS_LOG_ERR("%s unsupported function in rbus is called, please check\n", __FUNCTION__);
            return CCSP_FAILURE;
        }
    }

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

    dbus_message_append_args (message, DBUS_TYPE_STRING, &data, DBUS_TYPE_INVALID);


    dbus_connection_send (conn, message, NULL);

    dbus_message_unref (message);
    CcspTraceInfo(("Exiting %s\n", __FUNCTION__));

    return CCSP_SUCCESS;
}

int CcspBaseIf_SenddiagCompleteSignal(void * bus_handle)
{
    if(rbus_enabled == 1)
        return CcspBaseIf_SendSignal_rbus(bus_handle,  "diagCompleteSignal");
    return CcspBaseIf_SendSignal(bus_handle,  "diagCompleteSignal");
}

int CcspBaseIf_SendsystemReadySignal(void * bus_handle)
{
    if(rbus_enabled == 1)
        return CcspBaseIf_SendSignal_rbus(bus_handle,  "systemReadySignal");
    return CcspBaseIf_SendSignal(bus_handle,  "systemReadySignal");
}

int CcspBaseIf_SendsystemRebootSignal(void * bus_handle)
{
    if(rbus_enabled == 1)
        return CcspBaseIf_SendSignal_rbus(bus_handle,  "systemRebootSignal");
    return CcspBaseIf_SendSignal(bus_handle,  "systemRebootSignal");
}

/* This keep alive message just wants to say "hello" to dbus daemon. No one care it.*/
int CcspBaseIf_SendsystemKeepaliveSignal(void * bus_handle)
{
    if(rbus_enabled == 1)
        return CcspBaseIf_SendSignal_rbus(bus_handle,  "systemKeepaliveSignal");
    return CcspBaseIf_SendSignal(bus_handle,  "systemKeepaliveSignal");
}

int CcspBaseIf_SendSignal_rbus(void * bus_handle, char *event)
{
    rbusCoreError_t err = RBUSCORE_SUCCESS;
    int ret = CCSP_SUCCESS;
    rbusMessage out;
    rbusMessage_Init(&out);

    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    RBUS_LOG("%s : rbus_publishEvent object_name: %s  :: event_name : %s :: \n", __FUNCTION__, bus_info->component_id, event);
    err = rbus_publishEvent(bus_info->component_id, event, out);
    if (err != RBUSCORE_SUCCESS)
    {
        RBUS_LOG_ERR("%s : rbus_publishEvent returns Err: %d\n", __FUNCTION__, err);
        ret = CCSP_FAILURE;
    }

    return ret;
}

/* Subscribe to an event from a rbus 2.0 api component
 * As we migrate components from Ccsp to rbus2, more and will be based on rbus2
 */
int subscribeToRbus2Event_rbus
(
 void* bus_handle,
 const char* event_name
)
{
    rbusCoreError_t err;
    int provider_err = 0;
    rbusMessage payload = NULL;

    rbusMessage_Init(&payload);
    rbusMessage_SetInt32(payload, -1);
    rbusMessage_SetInt32(payload, 0);
    rbusMessage_SetInt32(payload, 0);
    rbusMessage_SetInt32(payload, 0);

    err = rbus_subscribeToEvent(NULL, event_name, CcspBaseIf_evt_callback_rbus, payload, bus_handle, &provider_err);

    if(payload)
    {
        rbusMessage_Release(payload);
    }

    if(err == RBUSCORE_SUCCESS)
    {
        RBUS_LOG("%s : rbus_subscribeToEvent success for %s\n", __FUNCTION__, event_name);
        return CCSP_SUCCESS;
    }
    else
    {
        RBUS_LOG_ERR("%s : rbus_subscribeToEvent returned err:%d provider_err:%d for %s\n", __FUNCTION__, err, provider_err, event_name);
        return CCSP_FAILURE;
    }
}

int CcspBaseIf_Register_Event_rbus
(
 void* bus_handle,
 const char* sender,
 const char* event_name
 )
{
    UNREFERENCED_PARAMETER(sender);
    CcspTraceInfo(("%s : rbus_registerEvent called for event: %s\n", __FUNCTION__, event_name));
    rbusCoreError_t err = RBUSCORE_SUCCESS;
    char * comp = NULL;

    if(strcmp(event_name, "diagCompleteSignal") == 0)
        comp = "eRT.com.cisco.spvtg.ccsp.tr069pa";
    else if(strcmp(event_name, "systemReadySignal") == 0 || strcmp(event_name, "Device.CR.SystemReady") == 0)
        /*systemReadySignal now implemented by Cr using rbus 2.0 api as property Device.CR.SystemReady whose value-change event we subscribe to*/
        return subscribeToRbus2Event_rbus(bus_handle, "Device.CR.SystemReady");
    else if(strcmp(event_name, "systemRebootSignal") == 0)
        comp = "eRT.com.cisco.spvtg.ccsp.rm";
    else if((strcmp(event_name, "currentSessionIDSignal") == 0) || (strcmp(event_name, "deviceProfileChangeSignal") == 0))
    {
    	RBUS_LOG("%s currentSessionIDSignal/deviceProfileChangeSignal not supported in rbus mode\n", __FUNCTION__);
        return CCSP_SUCCESS;
    }
    else if ((strcmp(event_name, "webconfigSignal") == 0) || (strcmp(event_name, "parameterValueChangeSignal") == 0) || (strcmp(event_name, "reboot") == 0) || (strcmp(event_name, "TunnelStatus")  == 0) || (strcmp(event_name, "WifiDbStatus") == 0))
    {
        RBUS_LOG ("RBUS_EVENT_SUBSCRIBED:: just like method for %s \n", event_name);
        return CCSP_SUCCESS;
    }
    else
    {
        RBUS_LOG_ERR("%s RBUS_EVENT_NOT_SUBSCRIBED:: Failed to subscribe %s \n", __FUNCTION__, event_name);
        return CCSP_FAILURE;
    }

    err = rbus_subscribeToEvent(comp , event_name, CcspBaseIf_evt_callback_rbus, NULL, bus_handle, NULL);

    if (err != RBUSCORE_SUCCESS)
    {
        RBUS_LOG_ERR("%s : rbus_subscribeToEvent returns Err: %d : Event %s\n", __FUNCTION__, err, event_name);
        return CCSP_FAILURE;
    }

    RBUS_LOG("%s : rbus_registerEvent success for : %s\n", __FUNCTION__, event_name);
    return CCSP_SUCCESS;
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
    UNREFERENCED_PARAMETER(ulRecordType);
    parameterValStruct_t val[1];
    char buf[128];
    char psmName[256];
    char*  var_string = 0;
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    char *str = NULL;
    int ret;
    errno_t rc = -1;

    val[0].parameterName  = (char *)pRecordName;
    val[0].parameterValue = buf;
    switch(pValue->Syntax)
    {
    case SLAP_VAR_SYNTAX_int:
        rc = sprintf_s(
            buf,
            sizeof(buf),
            "%d",
            pValue->Variant.varInt
        );
        if(rc < EOK)
        {
            ERR_CHK(rc);
        }
        val[0].type = ccsp_int;
        break;
    case SLAP_VAR_SYNTAX_uint32:
        rc = sprintf_s(
            buf,
            sizeof(buf),
            "%u",
            (unsigned int) pValue->Variant.varUint32
        );
        if(rc < EOK)
        {
            ERR_CHK(rc);
        }
        val[0].type = ccsp_unsignedInt;
        break;
    case SLAP_VAR_SYNTAX_bool:
        rc = strcpy_s(buf, sizeof(buf), ((pValue->Variant.varBool) ? PSM_TRUE : PSM_FALSE));
        ERR_CHK(rc);
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
        unsigned int i;
        val[0].type = ccsp_byte;
        for ( i = 0; i < var_uchar_array->VarCount; i++ )
        {
            rc = sprintf_s
            (
                &var_string[i * 2],
                (var_uchar_array->VarCount * 2 + 1),
                "%02X",
                var_uchar_array->Array.arrayUchar[i]
            );
            if(rc < EOK)
            {
                ERR_CHK(rc);
            }
        }

        val[0].parameterValue = var_string;
        break;
    }
    default:
        return CCSP_CR_ERR_INVALID_PARAM;
    }

    if ( pSubSystemPrefix && pSubSystemPrefix[0] != 0 )
    {
        rc = sprintf_s(psmName, sizeof(psmName), "%s%s", pSubSystemPrefix, CCSP_DBUS_PSM);
        if(rc < EOK)
        {
            ERR_CHK(rc);
        }
    }
    else
    {
        rc = strcpy_s(psmName, sizeof(psmName), CCSP_DBUS_PSM);
        ERR_CHK(rc);
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
    parameterNames[0] =(char *)pRecordName;
    parameterValStruct_t **val = 0;
    errno_t rc = -1;
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;

    if ( pSubSystemPrefix && pSubSystemPrefix[0] != 0 )
    {
        rc = sprintf_s(psmName, sizeof(psmName), "%s%s", pSubSystemPrefix, CCSP_DBUS_PSM);
        if(rc < EOK)
        {
            ERR_CHK(rc);
        }
    }
    else
    {
        rc = strcpy_s(psmName, sizeof(psmName), CCSP_DBUS_PSM);
        ERR_CHK(rc);
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
        else if( strcasecmp(val[0]->parameterValue, "false") == 0 )
        {
            pValue->Variant.varBool = SLAP_FALSE;
        }
        else if ( strcasecmp(val[0]->parameterValue, "true") == 0 )
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
    char *str = NULL;
    char psmName[256];
    int ret;
    errno_t rc = -1;

    val[0].parameterName  = (char *)pRecordName;
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
        rc = sprintf_s(psmName, sizeof(psmName), "%s%s", pSubSystemPrefix, CCSP_DBUS_PSM);
        if(rc < EOK)
        {
            ERR_CHK(rc);
        }
    }
    else
    {
        rc = strcpy_s(psmName, sizeof(psmName), CCSP_DBUS_PSM);
        ERR_CHK(rc);
    }

    val[0].parameterValue = (char *)pVal;
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
    errno_t rc = -1;

	*pValue = NULL;
    if ( pSubSystemPrefix && pSubSystemPrefix[0] != 0 )
    {
        rc = sprintf_s(psmName, sizeof(psmName), "%s%s", pSubSystemPrefix, CCSP_DBUS_PSM);
        if(rc < EOK)
        {
            ERR_CHK(rc);
        }
    }
    else
    {
        rc = strcpy_s(psmName, sizeof(psmName), CCSP_DBUS_PSM);
        ERR_CHK(rc);
    }

    parameterNames[0] = (char *)pRecordName;

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
    errno_t rc = -1;

    if ( pSubSystemPrefix && pSubSystemPrefix[0] != 0 )
    {
        rc = sprintf_s(psmName, sizeof(psmName), "%s%s", pSubSystemPrefix, CCSP_DBUS_PSM);
        if(rc < EOK)
        {
            ERR_CHK(rc);
        }
    }
    else
    {
        rc = strcpy_s(psmName, sizeof(psmName), CCSP_DBUS_PSM);
        ERR_CHK(rc);
    }

    if ( pRecordName[strlen(pRecordName)-1] == '.' )
    {
        ret = CcspBaseIf_getParameterNames(
                      bus_handle,
                      psmName,
                      CCSP_DBUS_PATH_PSM,
                      (char *)pRecordName,
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
        attr_val[0].parameterName = (char *)pRecordName; 
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
   errno_t rc = -1;

   if ( pSubSystemPrefix && pSubSystemPrefix[0] != 0 )
   {
        rc = sprintf_s(psmName, sizeof(psmName), "%s%s", pSubSystemPrefix, CCSP_DBUS_PSM);
        if(rc < EOK)
        {
            ERR_CHK(rc);
        }
   }
   else
   {
        rc = strcpy_s(psmName, sizeof(psmName), CCSP_DBUS_PSM);
        ERR_CHK(rc);
   }

   return CcspBaseIf_GetNextLevelInstances
		(
		    bus_handle,
		    psmName,
		    CCSP_DBUS_PATH_PSM,
		    (char *)pParentPath,
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
   errno_t rc = -1;

   if ( pSubSystemPrefix && pSubSystemPrefix[0] != 0 )
   {
        rc = sprintf_s(psmName, sizeof(psmName), "%s%s", pSubSystemPrefix, CCSP_DBUS_PSM);
        if(rc < EOK)
        {
            ERR_CHK(rc);
        }
   }
   else
   {
        rc = strcpy_s(psmName, sizeof(psmName), CCSP_DBUS_PSM);
        ERR_CHK(rc);
   }

   return CcspBaseIf_EnumRecords
		(
		    bus_handle,
		    psmName,
		    CCSP_DBUS_PATH_PSM,
		    (char *)pParentPath,
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
    if(rbus_enabled == 1)
    {
        return CcspBaseIf_Register_Event(bus_handle, sender, event_name);
    }
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
    if(rbus_enabled == 1)
    {
        return CcspBaseIf_UnRegister_Event(bus_handle, sender, event_name);
    }
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
    if(rbus_enabled == 1)
    {
        RBUS_LOG_ERR("%s unsupported function in rbus is called, please check\n", __FUNCTION__);
        return CCSP_FAILURE;
    }
    DBusMessage *message;
    DBusMessage *reply;
    int ret = CCSP_FAILURE;
    dbus_int32_t res ;
    dbus_int32_t tmp ;
    DBusMessageIter iter;
    DBusMessageIter struct_iter;

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

int CcspBaseIf_Register_Event
(
    void* bus_handle,
    const char* sender,
    const char* event_name
)
{
    if(rbus_enabled == 1)
        return CcspBaseIf_Register_Event_rbus(bus_handle, sender, event_name);
    CCSP_Message_Bus_Set_Event_Callback(bus_handle,CcspBaseIf_evt_callback, bus_handle);
    return CCSP_Message_Bus_Register_Event(bus_handle, sender, CCSP_DBUS_PATH_EVENT, CCSP_DBUS_INTERFACE_EVENT,event_name );
}

int CcspBaseIf_UnRegister_Event_rbus(
    void* bus_handle,
    const char* sender,
    const char* event_name)
{
    UNREFERENCED_PARAMETER(bus_handle);
    /* Ref the CcspBaseIf_Register_Event_rbus() for webconfigSignal event */
    if ((strcmp(event_name, "webconfigSignal") == 0) || (strcmp(event_name, "reboot") == 0) || (strcmp(event_name, "TunnelStatus") == 0)  || (strcmp(event_name, "WifiDbStatus") == 0))
    {
        RBUS_LOG ("RBUS_EVENT_UNSUBSCRIBED:: %s signal is handled as method and nothing to unsubscribe.\n", event_name);
        return CCSP_SUCCESS;
    }

    if(RBUSCORE_SUCCESS != rbus_unsubscribeFromEvent(sender, event_name, NULL))
    {
        RBUS_LOG_ERR("rbus_unsubscribeFromEvent::CcspBaseIf_UnRegister_Event_rbus returns error for sender %s for event_name %s \n", sender, event_name);
        return CCSP_FAILURE;
    }
    return CCSP_SUCCESS;
}

int CcspBaseIf_UnRegister_Event
(
    void* bus_handle,
    const char* sender,
    const char* event_name
)
{
    if(rbus_enabled == 1)
        return CcspBaseIf_UnRegister_Event_rbus(bus_handle, sender, event_name);
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

int getPartnerId (char *partnerID)
{
	char buffer[64];
	FILE *file;
	char *pos = NULL;

	if ((file = popen ("syscfg get PartnerID", "r")) != NULL)
	{
		pos = fgets (buffer, sizeof(buffer), file);
		pclose (file);
	}

	if ((pos == NULL) && ((file = popen ("/lib/rdk/getpartnerid.sh GetPartnerID", "r")) != NULL))
	{
		pos = fgets (buffer, sizeof(buffer), file);
		pclose (file);
	}

	if (pos)
	{
		size_t len = strlen (pos);

		if ((len > 0) && (pos[len - 1] == '\n'))
		{
			len--;
		}

		memcpy (partnerID, pos, len);
		partnerID[len] = 0;

		return CCSP_SUCCESS;
	}

	CcspTraceInfo(("%s : Error in opening File\n", __FUNCTION__));

	*partnerID = 0;

	return CCSP_FAILURE;
}

int Rbus_to_CCSP_error_mapper (int Rbus_error_code)
{
    int CCSP_error_code = CCSP_Message_Bus_ERROR;
    switch (Rbus_error_code)
    {
        case  RBUSCORE_SUCCESS                                 : CCSP_error_code = CCSP_Message_Bus_OK; break;
        case  RBUSCORE_ERROR_GENERAL                           : CCSP_error_code = CCSP_Message_Bus_ERROR; break;
        case  RBUSCORE_ERROR_INVALID_PARAM                     : CCSP_error_code = CCSP_ERR_INVALID_PARAMETER_VALUE; break;
        case  RBUSCORE_ERROR_INSUFFICIENT_MEMORY               : CCSP_error_code = CCSP_Message_Bus_OOM; break;
        case  RBUSCORE_ERROR_INVALID_STATE                     : CCSP_error_code = CCSP_Message_Bus_ERROR; break;
        case  RBUSCORE_ERROR_REMOTE_END_DECLINED_TO_RESPOND    : CCSP_error_code = CCSP_Message_Bus_ERROR; break;
        case  RBUSCORE_ERROR_REMOTE_END_FAILED_TO_RESPOND      : CCSP_error_code = CCSP_MESSAGE_BUS_TIMEOUT; break;
        case  RBUSCORE_ERROR_REMOTE_TIMED_OUT                  : CCSP_error_code = CCSP_MESSAGE_BUS_TIMEOUT; break;
        case  RBUSCORE_ERROR_MALFORMED_RESPONSE                : CCSP_error_code = CCSP_ERR_UNSUPPORTED_PROTOCOL; break;
        case  RBUSCORE_ERROR_UNSUPPORTED_METHOD                : CCSP_error_code = CCSP_MESSAGE_BUS_NOT_SUPPORT; break;
        case  RBUSCORE_ERROR_UNSUPPORTED_EVENT                 : CCSP_error_code = CCSP_MESSAGE_BUS_NOT_SUPPORT; break;
        case  RBUSCORE_ERROR_OUT_OF_RESOURCES                  : CCSP_error_code = CCSP_Message_Bus_OOM; break;
        case  RBUSCORE_ERROR_DESTINATION_UNREACHABLE           : CCSP_error_code = CCSP_MESSAGE_BUS_CANNOT_CONNECT; break;
    }
    return CCSP_error_code;
}

int Rbus2_to_CCSP_error_mapper (int Rbus_error_code)
{
    int CCSP_error_code = CCSP_Message_Bus_ERROR;
    switch (Rbus_error_code)
    {
        case  RBUS_ERROR_SUCCESS                            : CCSP_error_code = CCSP_Message_Bus_OK; break;
        case  RBUS_ERROR_BUS_ERROR                          : CCSP_error_code = CCSP_Message_Bus_ERROR; break;
        case  RBUS_ERROR_INVALID_INPUT                      : CCSP_error_code = CCSP_ERR_INVALID_PARAMETER_VALUE; break;
        case  RBUS_ERROR_NOT_INITIALIZED                    : CCSP_error_code = CCSP_Message_Bus_ERROR; break;
        case  RBUS_ERROR_OUT_OF_RESOURCES                   : CCSP_error_code = CCSP_Message_Bus_OOM; break;
        case  RBUS_ERROR_DESTINATION_NOT_FOUND              : CCSP_error_code = CCSP_MESSAGE_BUS_CANNOT_CONNECT; break;
        case  RBUS_ERROR_DESTINATION_NOT_REACHABLE          : CCSP_error_code = CCSP_MESSAGE_BUS_CANNOT_CONNECT; break;
        case  RBUS_ERROR_DESTINATION_RESPONSE_FAILURE       : CCSP_error_code = CCSP_MESSAGE_BUS_TIMEOUT; break;
        case  RBUS_ERROR_INVALID_RESPONSE_FROM_DESTINATION  : CCSP_error_code = CCSP_ERR_UNSUPPORTED_PROTOCOL; break;
        case  RBUS_ERROR_INVALID_OPERATION                  : CCSP_error_code = CCSP_MESSAGE_BUS_NOT_SUPPORT; break;
        case  RBUS_ERROR_INVALID_EVENT                      : CCSP_error_code = CCSP_MESSAGE_BUS_NOT_SUPPORT; break;
        case  RBUS_ERROR_INVALID_HANDLE                     : CCSP_error_code = CCSP_MESSAGE_BUS_NOT_SUPPORT; break;
        case  RBUS_ERROR_SESSION_ALREADY_EXIST              : CCSP_error_code = CCSP_CR_ERR_SESSION_IN_PROGRESS; break;
        case  RBUS_ERROR_COMPONENT_NAME_DUPLICATE           : CCSP_error_code = CCSP_Message_Bus_ERROR; break;
        case  RBUS_ERROR_ELEMENT_NAME_DUPLICATE             : CCSP_error_code = CCSP_Message_Bus_ERROR; break;
        case  RBUS_ERROR_ELEMENT_NAME_MISSING               : CCSP_error_code = CCSP_Message_Bus_ERROR; break;
        case  RBUS_ERROR_COMPONENT_DOES_NOT_EXIST           : CCSP_error_code = CCSP_Message_Bus_ERROR; break;
        case  RBUS_ERROR_ELEMENT_DOES_NOT_EXIST             : CCSP_error_code = CCSP_Message_Bus_ERROR; break;
        case  RBUS_ERROR_ACCESS_NOT_ALLOWED                 : CCSP_error_code = CCSP_ERR_REQUEST_REJECTED; break;
        case  RBUS_ERROR_INVALID_CONTEXT                    : CCSP_error_code = CCSP_Message_Bus_ERROR; break;
        case  RBUS_ERROR_TIMEOUT                            : CCSP_error_code = CCSP_MESSAGE_BUS_TIMEOUT; break;
        case  RBUS_ERROR_ASYNC_RESPONSE                     : CCSP_error_code = CCSP_Message_Bus_ERROR; break;
        case  RBUS_ERROR_INVALID_METHOD                     : CCSP_error_code = CCSP_ERR_METHOD_NOT_SUPPORTED; break;
        case  RBUS_ERROR_NOSUBSCRIBERS                      : CCSP_error_code = CCSP_Message_Bus_ERROR; break;
    }
    return CCSP_error_code;
}
