/*
 * If not stated otherwise in this file or this component's Licenses.txt file
 * the following copyright and licenses apply:
 *
 * Copyright 2019 RDK Management
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

/*
    Value-Change Detection:
    Simple API that allows you to add/remove parameters you wish to check value-change for.
    Uses a single thread to poll parameter values across all rbus handles.
    The thread is started on first param added and stopped on last param removed.
    A poling period (default 30 seconds) helps to limit the cpu usage.
    Runs in the provider process, so the value are got with direct callbacks and not over the network.
    The technique is simple:
    1) when a param is added, get and cache its current value.
    2) on a background thread, periodically get the latest value and compare to cached value.
    3) if the value has change, publish an event.
*/

#define _GNU_SOURCE 1 //needed for pthread_mutexattr_settype

#include "ccsp_rbus_value_change.h"
#include "rbus_message_bus.h"
#include "ccsp_base_api.h"
#include "ccsp_message_bus.h"
#include "ccsp_trace.h"
#include <rtmessage/rtVector.h>
#include <rtmessage/rtLog.h>
#include <rbus/rbus.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>

#define VC_LOCK() {int rc = pthread_mutex_lock(&vcmutex); (void)rc;}
#define VC_UNLOCK() {pthread_mutex_unlock(&vcmutex);}

extern void rbusFilter_InitFromMessage(rbusFilter_t* filter, rbusMessage msg);

static int              vcinit      = 0;
static int              vcrunning   = 0;
static int              vcperiod    = 1;//seconds
static rtVector         vcparams    = NULL;
static pthread_mutex_t  vcmutex;
static pthread_t        vcthread;

typedef struct ValueChangeRecord
{
    void* handle;
    char* listener; 
    char* parameter; 
    int32_t componentId;
    rbusFilter_t filter;
    int32_t interval;
    int32_t duration;
    char* value;
} ValueChangeRecord;

static void rbusValueChange_Init()
{
    CcspTraceDebug(("%s\n", __FUNCTION__));

    if(vcinit)
        return;

    vcinit = 1;

    rtVector_Create(&vcparams);

    pthread_mutexattr_t attrib;
    pthread_mutexattr_init(&attrib);
    pthread_mutexattr_settype(&attrib, PTHREAD_MUTEX_ERRORCHECK);
    if(0 != pthread_mutex_init(&vcmutex, &attrib))
    {
        CcspTraceError(("%s: failed to initialize mutex\n", __FUNCTION__));
    }
}

static void vcParams_Free(void* p)
{
    ValueChangeRecord* rec = (ValueChangeRecord*)p;
    if(rec->listener)
        free(rec->listener);
    if(rec->parameter)
        free(rec->parameter);
    if(rec->value)
        free(rec->value);
    if(rec->filter)
        rbusFilter_Release(rec->filter);
    free(rec);
}

static ValueChangeRecord* vcParams_Find(void* handle, const char* listener, const char* parameter, int32_t componentId, rbusFilter_t filter)
{
    size_t i;

    CcspTraceDebug(("%s %s %s filter=%p\n", __FUNCTION__, listener, parameter, filter));

    for(i=0; i < rtVector_Size(vcparams); ++i)
    {
        ValueChangeRecord* rec = (ValueChangeRecord*)rtVector_At(vcparams, i);

        if( rec &&
            rec->handle == handle &&
            strcmp(rec->listener, listener) == 0 &&
            strcmp(rec->parameter, parameter) == 0 &&
            rec->componentId == componentId &&
            rbusFilter_Compare(rec->filter, filter) == 0)
        {
            return rec;
        }
    }
    return NULL;
}

static parameterValStruct_t** rbusValueChange_GetParameterValue(ValueChangeRecord* rec)
{
    int rc;
    int num_values;
    parameterValStruct_t **values = 0;

    CCSPBASEIF_GETPARAMETERVALUES getParameterValues = 
        ((CCSP_Base_Func_CB* )(((CCSP_MESSAGE_BUS_INFO*)rec->handle)->CcspBaseIf_func))->getParameterValues;
    void* getParameterValues_data = 
        ((CCSP_Base_Func_CB* )(((CCSP_MESSAGE_BUS_INFO*)rec->handle)->CcspBaseIf_func))->getParameterValues_data;

    rc = getParameterValues(0, &rec->parameter, 1, &num_values, &values, getParameterValues_data);

    if(rc == CCSP_SUCCESS)
    {
        if(num_values == 1)
        {
            CcspTraceDebug(("%s %s success %s\n", __FUNCTION__, rec->parameter, values[0]->parameterValue));

            return values;
        }
        else
        {
            CcspTraceWarning(("%s %s unexpected num_vals %d\n", __FUNCTION__, rec->parameter, num_values));
        }

        free_parameterValStruct_t(rec->handle, num_values, values);
    }
    else
    {
        CcspTraceError(("%s %s failed %d\n", __FUNCTION__, rec->parameter, rc));
    }
    return NULL;
}

static rbusValueType_t rbusValueChange_GetDataType(enum dataType_e dt)
{
    switch(dt)
    {
    case ccsp_string: return RBUS_STRING;
    case ccsp_int: return RBUS_INT32;
    case ccsp_unsignedInt: return RBUS_UINT32;
    case ccsp_boolean: return RBUS_BOOLEAN;
    case ccsp_dateTime: return RBUS_DATETIME;
    //case ccsp_base64: return RBUS_STRING;/*FIXME is this an issue*/
    case ccsp_long: return RBUS_INT64;
    case ccsp_unsignedLong: return RBUS_UINT64;
    case ccsp_float: return RBUS_SINGLE;
    case ccsp_double: return RBUS_DOUBLE;
    case ccsp_byte: return RBUS_BYTES;
    case ccsp_none:
    default: return RBUS_NONE;
    }
}

static int rbusValueChange_getFilterResult(ValueChangeRecord* rec, parameterValStruct_t* val)
{
    rbusValue_t oldVal, newVal;
    rbusValueType_t type;

    if(!rec->filter)
        return -1;

    rbusValue_Init(&oldVal);
    rbusValue_Init(&newVal);

    type  = rbusValueChange_GetDataType(val->type);

    rbusValue_SetFromString(oldVal, type, rec->value);
    rbusValue_SetFromString(newVal, type, val->parameterValue);

    int oldResult = rbusFilter_Apply(rec->filter, oldVal);
    int newResult = rbusFilter_Apply(rec->filter, newVal);

    rbusValue_Release(oldVal);
    rbusValue_Release(newVal);

    if(newResult != oldResult)
    {
        CcspTraceDebug(("%s: filter triggered=%d for %s\n", __FUNCTION__, newResult, rec->parameter));

        if(newResult)
            return 1;
        else
            return 0;
    }
    else
    {
        return -1;
    }
}

void rbusFilter_AppendToMessage(rbusFilter_t filter, rbusMessage msg);/*from librbus.so*/

static void rbusValueChange_handlePublish(ValueChangeRecord* rec, parameterValStruct_t* val, int filterResult)
{
    rbusMessage msg;
    rbus_error_t err;

    /*construct a message just like rbus would construct it*/
    rbusMessage_Init(&msg);

    rbusMessage_SetString(msg, rec->parameter);/*event name*/
    rbusMessage_SetInt32(msg, RBUS_EVENT_VALUE_CHANGED);/*event type*/
    rbusMessage_SetString(msg, NULL);/*object name*/
    rbusMessage_SetInt32(msg, RBUS_OBJECT_SINGLE_INSTANCE);/*object type*/
    rbusMessage_SetInt32(msg, filterResult == -1 ? 2 : 3);/*number properties*/
    //prop 1: value
    rbusMessage_SetString(msg, "value");
    rbusMessage_SetInt32(msg, val->type);/*alternavitely we could use the true rbus type/value currently stored in newVal*/
    rbusMessage_SetString(msg, val->parameterValue);
    //prop 2: oldValue
    rbusMessage_SetString(msg, "oldValue");
    rbusMessage_SetInt32(msg, val->type);/*alternavitely we could use the true rbus type/value currently stored in oldVal*/
    rbusMessage_SetString(msg, rec->value);
    //prop 3: filter
    if(filterResult != -1)
    {
        rbusMessage_SetString(msg, "filter");
        rbusMessage_SetInt32(msg, ccsp_boolean);
        rbusMessage_SetString(msg, filterResult == 0 ? "0" : "1");
    }
    rbusMessage_SetInt32(msg, 0);/*object child object count*/
    if(rec->filter)
    {
        rbusMessage_SetInt32(msg, 1);
        rbusFilter_AppendToMessage(rec->filter, msg);
    }
    else
    {
        rbusMessage_SetInt32(msg, 0);
    }
    rbusMessage_SetInt32(msg, rec->componentId);

    CcspTraceDebug(("%s: publising event %s to listener %s componentId %d\n", __FUNCTION__, rec->parameter, rec->listener, rec->componentId));

    err = rbus_publishSubscriberEvent(
        ((CCSP_MESSAGE_BUS_INFO*)rec->handle)->component_id,  
        rec->parameter, 
        rec->listener, 
        msg);

    rbusMessage_Release(msg);

    if(err != RTMESSAGE_BUS_SUCCESS)
    {
        CcspTraceError(("%s rbus_publishSubscriberEvent failed error %d\n", __FUNCTION__, err));
    }
}

static void* rbusValueChange_pollingThreadFunc(void *userData)
{

    (void)(userData);
    CcspTraceWarning(("%s: start\n", __FUNCTION__));

    while(vcrunning)
    {
        size_t i;
        
        sleep(vcperiod);
        
        if(!vcrunning)
            break;

        VC_LOCK();//############ LOCK ############
        //TODO: VC_LOCK around the whole for loop might not be efficient
        //      What if rbusEvent_Publish takes too long.  
        //      This could block the _callback_handler, which calls rbusValueChange_AddParameter, for too long

        for(i=0; i < rtVector_Size(vcparams); ++i)
        {
            parameterValStruct_t **val = NULL;

            ValueChangeRecord* rec = (ValueChangeRecord*)rtVector_At(vcparams, i);
            if(!rec)
                continue;

            val = rbusValueChange_GetParameterValue(rec);

            if(val)
            {
                if(strcmp(rec->value, val[0]->parameterValue) != 0)
                {
                    int filterResult = -1;
                    bool publish = true;

                    CcspTraceDebug (("%s: value change detected for %s\n", __FUNCTION__, rec->parameter));

                    /*check filter -- if there's a filter, we only publish if filter is triggered*/
                    if(rec->filter)
                    {
                        filterResult = rbusValueChange_getFilterResult(rec, val[0]);

                        if(filterResult == -1)/*-1 means the filter was not triggered, so don't publish*/
                        {
                            CcspTraceDebug(("%s: no publish\n", __FUNCTION__));
                            publish = false;
                        }
                    }

                    /*send event to listener*/
                    if(publish)
                    {
                        rbusValueChange_handlePublish(rec, val[0], filterResult);
                    }

                    /*update the record's property with new value*/
                    free(rec->value);
                    rec->value = strdup(val[0]->parameterValue);
                }
                else
                {
                    CcspTraceDebug (("%s: value change not detected for %s\n", __FUNCTION__, rec->parameter));
                }
     
                free_parameterValStruct_t(rec->handle, 1, val);
            }
            else
            {
                CcspTraceWarning(("%s getParameterValues failed\n", __FUNCTION__));
            }
        }

        VC_UNLOCK();//############ UNLOCK ############
    }
    CcspTraceWarning(("%s: stop\n", __FUNCTION__));
    return NULL;
}

static void Ccsp_RbusValueChange_ReadPayload(
    rbusMessage payload,
    int32_t* componentId,
    int32_t* interval,
    int32_t* duration,
    rbusFilter_t* filter)
{
    *componentId = 0;
    *interval = 0;
    *duration = 0;
    *filter = NULL;

    if(payload)
    {
        int hasFilter;
        rbusMessage_GetInt32(payload, componentId);
        rbusMessage_GetInt32(payload, interval);
        rbusMessage_GetInt32(payload, duration);
        rbusMessage_GetInt32(payload, &hasFilter);
        if(hasFilter)
            rbusFilter_InitFromMessage(filter, payload);
    }
}

int Ccsp_RbusValueChange_Subscribe(
    void* handle, 
    const char* listener, 
    const char* parameter,
    rbusMessage payload)
{
    ValueChangeRecord* rec;
    int32_t componentId = 0;
    int32_t interval = 0;
    int32_t duration = 0;
    rbusFilter_t filter = NULL;

    CcspTraceDebug(("%s: %s\n", __FUNCTION__, parameter));

    if(!vcinit)
    {
        rbusValueChange_Init();
    }

    /*verify bus handle is valid*/
    if( handle == NULL || 
       ((CCSP_MESSAGE_BUS_INFO*)handle)->CcspBaseIf_func == NULL || 
       ((CCSP_Base_Func_CB* )(((CCSP_MESSAGE_BUS_INFO*)handle)->CcspBaseIf_func))->getParameterValues == NULL)
    {
        CcspTraceError(("%s NULL bus info\n", __FUNCTION__));
        return CCSP_FAILURE;
    }

    Ccsp_RbusValueChange_ReadPayload(payload, &componentId, &interval, &duration, &filter);

    /* only add the property if its not already in the list */

    VC_LOCK();

    rec = vcParams_Find(handle, listener, parameter, componentId, filter);

    VC_UNLOCK();

    if(!rec)
    {
        parameterValStruct_t **val = NULL;

        rec = (ValueChangeRecord*)malloc(sizeof(ValueChangeRecord));
        rec->handle = handle;
        rec->listener = strdup(listener);
        rec->parameter = strdup(parameter);
        rec->componentId = componentId;
        rec->filter = filter;
        rec->interval = interval;
        rec->duration = duration;
        rec->value = NULL;
        if(rec->filter)
            rbusFilter_Retain(rec->filter);

        val = rbusValueChange_GetParameterValue(rec);
        if(val)
        {
            rec->value = strdup(val[0]->parameterValue);
            free_parameterValStruct_t(rec->handle, 1, val);
        }

        CcspTraceDebug(("%s: %s new subscriber from listener %s componentId %d\n", __FUNCTION__, rec->parameter, rec->listener, rec->componentId));

        VC_LOCK();
        rtVector_PushBack(vcparams, rec);
        /* start polling thread if needed */
        if(!vcrunning)
        {
            vcrunning = 1;
            pthread_create(&vcthread, NULL, rbusValueChange_pollingThreadFunc, NULL);
        }
        VC_UNLOCK();
    }
    else
    {
        CcspTraceDebug(("%s: ignoring duplicate subscribe for %s from listener %s componentId %d\n", __FUNCTION__, rec->parameter, rec->listener, rec->componentId));
    }

    if(filter)
    {
        rbusFilter_Release(filter);
    }

    return CCSP_SUCCESS;
}

int Ccsp_RbusValueChange_Unsubscribe(
    void* handle,
    const char* listener, 
    const char* parameter, 
    rbusMessage payload)
{
    ValueChangeRecord* rec;
    int32_t componentId = 0;
    int32_t interval = 0;
    int32_t duration = 0;
    rbusFilter_t filter = NULL;

    (void)(handle);

    CcspTraceDebug(("%s: %s\n", __FUNCTION__, parameter));

    if(!vcinit)
    {
        return CCSP_FAILURE;
    }

    Ccsp_RbusValueChange_ReadPayload(payload, &componentId, &interval, &duration, &filter);

    VC_LOCK();

    rec = vcParams_Find(handle, listener, parameter, componentId, filter);

    VC_UNLOCK();

    if(filter)
    {
        rbusFilter_Release(filter);
    }

    if(rec)
    {
        int stopThread = 0;

        VC_LOCK();

        rtVector_RemoveItem(vcparams, rec, vcParams_Free);

        /* if there's nothing left to poll then shutdown the polling thread */

        if(vcrunning && rtVector_Size(vcparams) == 0)
        {
            stopThread = 1;
            vcrunning = 0;
        }

        VC_UNLOCK();

        if(stopThread)
        {
            pthread_join(vcthread, NULL);
        }
        return CCSP_SUCCESS;
    }
    else
    {
        CcspTraceWarning(("%s: value change param not found: %s\n", __FUNCTION__, parameter));
        return CCSP_FAILURE;
    }
}

int rbusValueChange_Close(void* handle)
{
    CcspTraceDebug(("%s\n", __FUNCTION__));

    if(!vcinit)
    {
        return CCSP_FAILURE;
    }

    size_t i = 0;
    while(i < rtVector_Size(vcparams))
    {
        ValueChangeRecord* rec = (ValueChangeRecord*)rtVector_At(vcparams, i);
        if(rec && rec->handle == handle)
        {
            rtVector_RemoveItem(vcparams, rec, vcParams_Free);
        }
        else
        {
            i++; 
        }
    }

    //clean up everything once all params are removed
    //but check the size to ensure we do not clean up if params for other rbus handles exist
    if(rtVector_Size(vcparams) == 0)
    {
        if(vcrunning)
        {
            vcrunning = 0;
            pthread_join(vcthread, NULL);
        }

        rtVector_Destroy(vcparams, vcParams_Free);
        pthread_mutex_destroy(&vcmutex);
        vcparams = NULL;
        vcinit = 0;
    }

    return CCSP_SUCCESS;
}

