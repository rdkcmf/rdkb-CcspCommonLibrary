#include "webconfig_bus_interface.h"
#ifdef WBCFG_MULTI_COMP_SUPPORT

static rbusHandle_t bus_handle_rbus = NULL;

int gBroadcastSubscribed = 0;
int gMasterSubscribed = 0;
int gSlaveSubscribed = 0;

rbusDataElement_t dataElements_slave[1] = {
                            {MASTER_COMP_SIGNAL_NAME, RBUS_ELEMENT_TYPE_EVENT, {NULL,NULL,NULL,NULL, eventSubHandler, NULL}}
                        };

rbusDataElement_t dataElements_master[1] = {
                 {SLAVE_COMP_SIGNAL_NAME, RBUS_ELEMENT_TYPE_EVENT, {NULL,NULL,NULL,NULL, eventSubHandler, NULL}}
               };   

rbusDataElement_t dataElements_broadcast[1] = {
              {BROADCASTSIGNAL_NAME, RBUS_ELEMENT_TYPE_EVENT, {NULL,NULL,NULL,NULL, eventSubHandler, NULL}}
          };

extern char process_name[64] ;

/*************************************************************************************************************************************

    caller:    callback function for rbus events

    prototype:
        void multiComp_callbk_rbus
        (rbusHandle_t handle,
         rbusEvent_t const* event, 
         rbusEventSubscription_t* subscription) 
            
        );

    description :
    callback function for rbus events, based on the event name it calls the api to parse the data


****************************************************************************************************************************************/

void multiComp_callbk_rbus(rbusHandle_t handle, rbusEvent_t const* event, rbusEventSubscription_t* subscription) {
    CcspTraceInfo(("Entering %s\n", __FUNCTION__));

    (void)(handle);
    (void)(subscription);

    const char* eventName = event->name;

    rbusValue_t valBuff;
    valBuff = rbusObject_GetValue(event->data, NULL );
    if(!valBuff)
    {
        CcspTraceInfo(("FAIL: value is NULL\n"));
    }
    else
    {
        const char* data = rbusValue_GetString(valBuff, NULL);
        CcspTraceInfo(("rbus event callback Event is %s , data is %s\n",eventName,data));

        if ( strncmp(eventName,BROADCASTSIGNAL_NAME,strlen(BROADCASTSIGNAL_NAME)) == 0 )
        {
              parseBroadcastData(data);

        }
        else if ( strncmp(eventName,MASTER_COMP_SIGNAL_NAME,strlen(MASTER_COMP_SIGNAL_NAME)) == 0 )
        {
              parseMasterData(data);

        }
        else if ( strncmp(eventName,SLAVE_COMP_SIGNAL_NAME,strlen(SLAVE_COMP_SIGNAL_NAME)) == 0 )
        {
              parseSlaveData(data);
        }
    }  


    CcspTraceInfo(("Exiting %s\n", __FUNCTION__));

}

/*************************************************************************************************************************************

    caller:    event subscription notification

    prototype:
    rbusError_t eventSubHandler
    (
      rbusHandle_t handle, 
      rbusEventSubAction_t action, 
      const char* eventName, 
      rbusFilter_t filter, 
      int32_t interval, 
      bool* autoPublish
      );


    description :
    Call back function notifies when rbus event subscription/unsuscription happens

****************************************************************************************************************************************/


rbusError_t eventSubHandler(rbusHandle_t handle, rbusEventSubAction_t action, const char* eventName, rbusFilter_t filter, int32_t interval, bool* autoPublish)
{
    (void)handle;
    (void)filter;
    (void)interval;
    (void)autoPublish;

    CcspTraceInfo((
        "eventSubHandler called:\n" \
        "\taction=%s\n" \
        "\teventName=%s\n",
        action == RBUS_EVENT_ACTION_SUBSCRIBE ? "subscribe" : "unsubscribe",
        eventName));

    if(!strcmp(BROADCASTSIGNAL_NAME, eventName))
    {
        gBroadcastSubscribed = action == RBUS_EVENT_ACTION_SUBSCRIBE ? 1 : 0;
    }
    else if(!strcmp(MASTER_COMP_SIGNAL_NAME, eventName))
    {
        gMasterSubscribed = action == RBUS_EVENT_ACTION_SUBSCRIBE ? 1 : 0;
    }
    else if(!strcmp(SLAVE_COMP_SIGNAL_NAME, eventName))
    {
        gSlaveSubscribed = action == RBUS_EVENT_ACTION_SUBSCRIBE ? 1 : 0;
    }
    else
    {
        CcspTraceError(("provider: eventSubHandler unexpected eventName %s\n", eventName));
    }

    return RBUS_ERROR_SUCCESS;
}

/* thread created to subscribe to broadcast event published by master in slave component
It waits till master registers the event it is publishing*/
void*  event_subscribe_bcast(void* arg)
{
    CcspTraceInfo(("Entering %s\n", __FUNCTION__));
    (void)arg;
    int ret = 0;
    while(1)
    {
        ret = subscribeToEvent(BROADCASTSIGNAL_NAME);

        if ( RBUS_ERROR_SUCCESS == ret )
        {
               break;

        }                                    
        sleep(5);
    }
    CcspTraceInfo(("Exiting %s\n", __FUNCTION__));
    return NULL;
}

/*************************************************************************************************************************************

    caller:    initMultiCompSlave

    void eventRegisterSlave
    (
    );

    description :
    Event registration from slave component to receive data from master

****************************************************************************************************************************************/

void eventRegisterSlave()
{
  int ret = 0;
  CcspTraceInfo(("Entering %s\n", __FUNCTION__));
  if (rbus_enabled == 1 )
  {
      ret = RBUS_ERROR_SUCCESS;
      ret = rbus_open(&bus_handle_rbus, process_name);

      if(ret != RBUS_ERROR_SUCCESS) {
          CcspTraceError(("%s: init failed with error code %d \n", __FUNCTION__, ret));
          return ;
      }
      // creating thread to register to broadcast event 
      pthread_t bcast_regtid;
      pthread_create(&bcast_regtid, NULL, event_subscribe_bcast,NULL);
  }
  else
  {
        CcspBaseIf_SetCallback2(bus_handle, BROADCASTSIGNAL_NAME,
                    multiCompBroadCastSignal_callbk, NULL);

        ret = CcspBaseIf_Register_Event(bus_handle, NULL, BROADCASTSIGNAL_NAME);
            if (ret != CCSP_Message_Bus_OK) {
                CcspTraceError(("multiCompBroadCastSignal reg unsuccessfull\n"));

            } 
        else 
        {
                CcspTraceInfo(("multiCompBroadCastSignal Registration with CCSP Bus successful\n"));
        }

            CcspBaseIf_SetCallback2(bus_handle, SLAVE_COMP_SIGNAL_NAME,
              multiCompSlaveProcessSignal_callbk, NULL);

  }

}

/*************************************************************************************************************************************

    caller:    initMultiCompMaster

    void eventRegisterMaster
    (
    );

    description :
    Event registration from master component to receive data from slave

****************************************************************************************************************************************/


void eventRegisterMaster()
{

  int ret = 0;

  CcspTraceInfo(("%s for process  %s \n", __FUNCTION__, process_name));
  if (rbus_enabled == 1 )
  {
          ret = RBUS_ERROR_SUCCESS;
          ret = rbus_open(&bus_handle_rbus, process_name);

          if(ret != RBUS_ERROR_SUCCESS) {
          CcspTraceError(("%s: init failed with error code %d \n", __FUNCTION__, ret));
          return ;
        }

          /***
           * Register data elements with rbus for EVENTS.
           */
          ret = rbus_regDataElements(bus_handle_rbus, 1, dataElements_broadcast);
          if(ret != RBUS_ERROR_SUCCESS)
          {
              CcspTraceError(("Failed to register multiCompBroadCastSignal data elements with rbus. Error code : %d\n", ret));
          }   

  }
  else
  {
        CcspBaseIf_SetCallback2(bus_handle, MASTER_COMP_SIGNAL_NAME,
              multiCompMasterProcessSignal_callbk, NULL);
  }

}

/*************************************************************************************************************************************

    caller:    parseBroadcastData,sendBlobExecutionResult , sendDataToSlaveComp 

    void sendDataToEvent
    (
      char* event_name ,
      char* eventData
    );

    description :
    RBUS/DBUS events to send data between master and slave

****************************************************************************************************************************************/


void sendDataToEvent(char* event_name , char* eventData)
{
    CcspTraceInfo(("%s : Data is %s , rbus_enabled is %d\n", __FUNCTION__,eventData,rbus_enabled));
    int ret = 0;
    if ( rbus_enabled == 1 )
    {
            rbusEvent_t event;
            rbusObject_t data;
            rbusValue_t value;
            rbusError_t ret = RBUS_ERROR_SUCCESS;

            rbusValue_Init(&value);
            rbusValue_SetString(value, eventData);
            rbusObject_Init(&data, NULL);
            rbusObject_SetValue(data, event_name, value);

            event.name = event_name;
            event.data = data;
            event.type = RBUS_EVENT_GENERAL;
            ret = rbusEvent_Publish(bus_handle_rbus, &event);
            if(ret != RBUS_ERROR_SUCCESS) {
                CcspTraceInfo(("provider: rbusEvent_Publish Event1 failed: %d\n", ret));
            }

            rbusValue_Release(value);
            CcspTraceInfo(("%s --out\n", __FUNCTION__));

    }
    else
    {
        CcspTraceInfo(("%s : calling CcspBaseIf_SendSignal_WithData,event_name is %s\n", __FUNCTION__,event_name));

        ret = CcspBaseIf_SendSignal_WithData(bus_handle,event_name,eventData);

        if ( ret != CCSP_SUCCESS )
        {
              CcspTraceError(("%s : %s event failed,  ret value is %d\n",__FUNCTION__,event_name,ret));
        }


    }
    CcspTraceInfo(("%s : return value is %d \n", __FUNCTION__,ret));


}

/*************************************************************************************************************************************

    caller:    messageQueueProcessingMultiComp, messageQueueProcessingMultiCompSlave

    void EventRegister
    (
      char* EventName,
    );

    description :
    RBUS/DBUS event registration in runtime for master and slave communication
****************************************************************************************************************************************/

void EventRegister(char* EventName)
{

    CcspTraceInfo(("%s : event name is %s\n", __FUNCTION__,EventName));

    int ret = 0 ;
    if (rbus_enabled == 1 )
    {
            /***
            * Register data elements with rbus for EVENTS.
            */            
            if ( strncmp(EventName,SLAVE_COMP_SIGNAL_NAME,strlen(SLAVE_COMP_SIGNAL_NAME)) == 0 )
            {
                    ret = rbus_regDataElements(bus_handle_rbus, 1, dataElements_slave);
            }
            else if ( strncmp(EventName,MASTER_COMP_SIGNAL_NAME,strlen(MASTER_COMP_SIGNAL_NAME)) == 0 )
            {
                    ret = rbus_regDataElements(bus_handle_rbus, 1, dataElements_master);
            }

    }
    else
    {
            ret = CcspBaseIf_Register_Event(bus_handle, NULL, EventName );
            if (ret != CCSP_Message_Bus_OK) {
                    CcspTraceError(("%s reg unsuccessfull\n",EventName));
                    return ;
            } else {
                    CcspTraceInfo(("%s registration with CCSP Bus successful\n",EventName));
            }
    }
    CcspTraceInfo(("Exiting from %s\n", __FUNCTION__));

    return;

}

/*************************************************************************************************************************************

    caller:    messageQueueProcessingMultiComp

    void UnregisterFromEvent
    (
      char* EventName,
    );

    description :
    API to unsubscribe from rbus events
****************************************************************************************************************************************/

void UnSubscribeFromEvent(char* EventName)
{
    if (rbus_enabled == 1 )
    {
        CcspTraceInfo(("%s : event name is %s\n", __FUNCTION__,EventName));
        int ret = 0 ;
          ret = rbusEvent_Unsubscribe(bus_handle_rbus, EventName);
          if ( ret != RBUS_ERROR_SUCCESS )
          {
              CcspTraceError(("%s Unsubscribe failed\n",EventName));
              return ;
          } 
          else {
              CcspTraceInfo(("%s Unsubscribe with rbus successful\n",EventName));
          }
    }
}
/*************************************************************************************************************************************

    caller:    messageQueueProcessingMultiComp, messageQueueProcessingMultiCompSlave

    void UnregisterFromEvent
    (
      char* EventName,
    );

    description :
    API to unregister from events, unpublish events for rbus
****************************************************************************************************************************************/

void UnregisterFromEvent(char* EventName)
{
    CcspTraceInfo(("%s : event name is %s\n", __FUNCTION__,EventName));
    int ret = 0 ;
          if ( rbus_enabled == 1 )
          {
                if ( strncmp(EventName,SLAVE_COMP_SIGNAL_NAME,strlen(SLAVE_COMP_SIGNAL_NAME)) == 0 )
                {
                        ret = rbus_unregDataElements(bus_handle_rbus, 1, dataElements_slave);
                        gMasterSubscribed = 0;

                }
                else if ( strncmp(EventName,MASTER_COMP_SIGNAL_NAME,strlen(MASTER_COMP_SIGNAL_NAME)) == 0 )
                {
                        ret = rbus_unregDataElements(bus_handle_rbus, 1, dataElements_master);
                        gSlaveSubscribed = 0;

                }
          }
          else
          {
                ret = CcspBaseIf_UnRegister_Event(bus_handle, NULL, EventName);
                if (ret != CCSP_Message_Bus_OK) {
                    CcspTraceError(("%s unreg failed\n",EventName));
                    return ;
                } else {
                    CcspTraceInfo(("%s UnRegistration with CCSP Bus successful\n",EventName));
                }
          }

    CcspTraceInfo(("Exiting from %s\n", __FUNCTION__));
    return ;
}

/*************************************************************************************************************************************

    caller:    messageQueueProcessingMultiComp

    void UnregisterFromEvent
    (
      char* EventName,
    );

    description :
    subscribe to event
****************************************************************************************************************************************/

int subscribeToEvent(char* EventName)
{
    int ret = RBUS_ERROR_SUCCESS ;
    CcspTraceInfo(("%s : event name is %s\n", __FUNCTION__,EventName));
    if (rbus_enabled == 1 )
    {

        char user_data[64] = {0};
        if ( strncmp(EventName,SLAVE_COMP_SIGNAL_NAME,strlen(SLAVE_COMP_SIGNAL_NAME)) == 0 )
        {
            strncpy(user_data,"slaveSignal",sizeof(user_data)-1);
        }
        else if ( strncmp(EventName,MASTER_COMP_SIGNAL_NAME,strlen(MASTER_COMP_SIGNAL_NAME)) == 0 )
        {       
            strncpy(user_data,"masterSignal",sizeof(user_data)-1);
        }
        else if ( strncmp(EventName,BROADCASTSIGNAL_NAME,strlen(MASTER_COMP_SIGNAL_NAME)) == 0 )
        {       
            strncpy(user_data,"broadcastSignal",sizeof(user_data)-1);
        }

        ret = rbusEvent_Subscribe(bus_handle_rbus, EventName, multiComp_callbk_rbus, user_data);
        if(ret != RBUS_ERROR_SUCCESS) {
            CcspTraceError(("Unable to subscribe to event %s with rbus error code : %d\n", EventName, ret));
        } 
        
        CcspTraceInfo(("%s : subscribe to %s ret value is %d\n", __FUNCTION__,EventName,ret));

    }
   return ret;   
}

#endif 
/*************************************************************************************************************************************

    caller:    messageQueueProcessing, messageQueueProcessingMultiComp

    void sendWebConfigSignal
    (
      char* data,
    );

    description :
    API sends ACK/ACK to webconfig client
****************************************************************************************************************************************/

void sendWebConfigSignal(char* data)
{
        int ret =  CcspBaseIf_WebConfigSignal(bus_handle, data);

        if ( ret != CCSP_SUCCESS )
            CcspTraceError(("%s : CcspBaseIf_WebConfigSignal failed,  ret value is %d\n",__FUNCTION__,ret));
    
    return ;
}
