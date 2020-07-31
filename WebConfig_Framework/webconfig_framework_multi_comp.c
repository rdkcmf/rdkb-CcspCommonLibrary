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



#ifdef WBCFG_MULTI_COMP_SUPPORT
#include "webconfig_framework.h"
#include "webconfig_bus_interface.h"

extern int gBroadcastSubscribed ;
extern int gMasterSubscribed ;
extern int gSlaveSubscribed ;

extern char multiCompMqEventName[64];
extern char mCompMqSlaveName[64];

pthread_mutex_t mCompWebconfig_exec = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t mCompWebconfig_exec_completed = PTHREAD_COND_INITIALIZER;
pthread_cond_t mCompWebconfig_slave_exec_completed = PTHREAD_COND_INITIALIZER;

pthread_mutex_t mCompQueue_access = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t multiCompState_access = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t multiRegData_access = PTHREAD_MUTEX_INITIALIZER;

extern pthread_mutex_t webconfig_exec ;
extern pthread_cond_t webconfig_exec_completed ;

extern pthread_condattr_t webconfig_attr;
extern pthread_mutex_t queue_access ;

extern pErr execReturn ;
pErr execReturnMultiComp = NULL;

pErr execReturnMultiCompSlave = NULL;

static pthread_mutex_t MultiCompMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t MultiCompCond = PTHREAD_COND_INITIALIZER;

pthread_condattr_t mCompWebconfig_attr;
queueInfo  mCompQueueData;

extern int checkNewVersionUpdateRequired(execData *exec_data,int *queueIndex);
extern size_t getPendingQueueTimeout(uint16_t txid);
extern blobRegInfo* getAddress(char* subdoc_name);
extern int isQueueEmpty();

extern void* execute_request(void *arg);

int receivedReadyToReceiveSignal = 0, receivedCompletedSignal = 0 ;
extern char process_name[64] ;

char subdocInExec[64] = {0} ;
multiCompExecStatus *gMultiCompExecState = NULL ;


int gNumOfComponents = 0 , gNumOfMultiCompSubDoc = 0 ;
int gReadyToReceive = 0;

multiCompSubDocReg *multiCompSubDocInfo;

int slaveExecutionCount = 0;


void* gMasterExecData = NULL ;
MultiComp_ExecInfo  *gMultiCompDataAddress = NULL;

void* event_register_slave(void* subdoc_name)
{
    pthread_detach(pthread_self());
    if ( slaveExecutionCount == 0 )
    {
        EventRegister(SLAVE_COMP_SIGNAL_NAME);
        subscribeToEvent(SLAVE_COMP_SIGNAL_NAME);

    }

    slaveExecutionCount++;

    int counter = 0 ;

    if ( rbus_enabled == 1 )
    {
        while ( !gMasterSubscribed )
        {
            sleep(2);
            counter += 2 ;
            if ( counter > MAX_RESPONSE_TIME )
            {
                CcspTraceInfo(("master failed to subscribe to %s\n",MASTER_COMP_SIGNAL_NAME));
                break;

            }
        }
        sleep (2);
    }

    char data[256] = {0};
    memset(data,0,sizeof(data));
    snprintf(data,sizeof(data),"%s,%s,%d",process_name,(char*)subdoc_name,READY_TO_RECEIVE_DATA);

    sendDataToEvent(MASTER_COMP_SIGNAL_NAME,data);
    if (subdoc_name != NULL)
    {
       free(subdoc_name);
       subdoc_name = NULL;
    }
  return NULL;
}

// Function to parse the data received from broadcast event
void parseBroadcastData(const char* info)
{
        char str[512] = {0};
        memset(str,0,sizeof(str));
        snprintf(str,sizeof(str),"%s",info);
        pthread_t slaveEvent_tid;
        char *token[64];

        int i= -1, count=0 ;

        char* rest = str;
        while ((token[++i] = strtok_r(rest, ",", &rest))) 
              count++;

        if ( count != 3 )
        {
          CcspTraceError(("Invalid number of parameters passed\n"));
            return;
        }
        char* lsubdocName = NULL;
        i = 0 ;
        if( (strncmp(process_name,token[i],sizeof(process_name) -1) == 0 )  && ( atoi(token[++i]) == IAM_MASTER ) )          
        {

            lsubdocName = malloc(SUBDOC_NAME_SZ);
            memset(lsubdocName,0,SUBDOC_NAME_SZ);

            strncpy(lsubdocName,token[++i],SUBDOC_NAME_SZ);
            pthread_create(&slaveEvent_tid, NULL, event_register_slave,lsubdocName);


        }
        return ;
}

/*************************************************************************************************************************************

    caller:    Callback function

    prototype:

        void
        multiCompBroadCastSignal_callbk
        (
            char* info, void *user_data
        );

    description :

	           Callback function for broadcasting event, master will send the slave comp name and subdoc in broadcast event
        	   After receiving the event slave will register for slave event to establish POP through which master will send data to slave

    argument:   
        	   char* info --> Data sent by the caller/slave
        	   void *user_data  -->usuaully NULL value received, can have comp specific info

*******************************************************************************************************************************************/
void multiCompBroadCastSignal_callbk(char* info, void *user_data)
{

    CcspTraceInfo(("%s : Data Received is %s , user data is %s \n",__FUNCTION__,info,(char*)user_data));
    parseBroadcastData(info);

}

// Function to parse the data received from master event
void parseMasterData(const char* info)
{
      if ( gMultiCompExecState == NULL )
      {
          CcspTraceInfo(("%s : gMultiCompExecState is NULL , returning \n",__FUNCTION__));
          return ;
      }

        char str[512] = {0};
        memset(str,0,sizeof(str));
        snprintf(str,sizeof(str),"%s",info);

        char *token[64];
        int i=-1, count=0;
        char* rest = str;

        while ((token[++i] = strtok_r(rest, ",", &rest))) 
          count++;

        if ( count <= 2  )
        {
           CcspTraceError(("Invalid number of parameters passed\n"));
           return;
        }
      
        int validResponse = 0;
        i = 0 ;


        pthread_mutex_lock(&multiCompState_access);

        multiCompExecStatus *lMultiCompExecData = NULL ;

        lMultiCompExecData =  gMultiCompExecState ;
        int j;
        for (j=0; j < gNumOfComponents ; j++)
        {
            if ( strncmp(token[i],lMultiCompExecData->componentName,strlen(lMultiCompExecData->componentName)) == 0 )
            {
                  if ( strncmp(subdocInExec,token[++i],sizeof(subdocInExec)-1) == 0 )
                  {
                      validResponse = 1 ;
                      CcspTraceInfo(("Valid response\n"));
                      break;
                  }
            }

            lMultiCompExecData++;
        }

        i++;

        if ( validResponse == 1 )
        {
          if ( atoi(token[i]) == READY_TO_RECEIVE_DATA )
                {
                    CcspTraceInfo(("READY_TO_RECEIVE_DATA, sending pthread_cond_signal\n"));
                    gReadyToReceive = 1;
                    pthread_cond_signal(&MultiCompCond);

                }

                else if ( atoi(token[i])  == TIMEOUT_VALUE )
                {
                    lMultiCompExecData->timeout =  atoi(token[++i]) ;
                    CcspTraceInfo(("Received timeout is %d,\n",lMultiCompExecData->timeout));
                    pthread_cond_signal(&MultiCompCond);
                }

                else if ( atoi(token[i])  == EXECUTION_SUCCESSFULLY_COMPLETED )
                {
                  CcspTraceInfo(("EXECUTION_SUCCESSFULLY_COMPLETED, sending pthread_cond_signal\n"));

                      lMultiCompExecData->executionStatus = 1 ;
                      lMultiCompExecData->execResult = atoi((token[++i])) ;

                      if ( token[++i] != NULL )
                      {
                          strncpy(lMultiCompExecData->execRetMsg,token[i],sizeof(lMultiCompExecData->execRetMsg)-1);
                      }
                   
                      pthread_cond_signal(&MultiCompCond);

                }

                else if ( atoi(token[i]) == EXECUTION_FAILED )
                {

                      CcspTraceInfo(("EXECUTION_FAILED, sending pthread_cond_signal\n"));

                      lMultiCompExecData->executionStatus = 0 ;
                      lMultiCompExecData->execResult = atoi((token[++i])) ;
                      if ( token[++i] != NULL )
                      {
                          strncpy(lMultiCompExecData->execRetMsg,token[i],sizeof(lMultiCompExecData->execRetMsg)-1);
                      }
                      pthread_cond_signal(&MultiCompCond);

                }
        }


        pthread_mutex_unlock(&multiCompState_access);
}

/*************************************************************************************************************************************

    caller:    Callback function

    prototype:

        void
        multiCompMasterProcessSignal_callbk
        (
            char* info, void *user_data
        );

    description :

           	Callback function of the master component, slave will send the data in multiCompMasterProcessSignal
           	After receiving the valid data/response from slave component, master will update it multiCompExecStatus struct and 
           	call the pthread cond signal to wake up the message queue thread.

    argument:   
        	char* info --> Data sent by the caller/slave
        	void *user_data  -->usuaully NULL value received, can have comp specific info

*******************************************************************************************************************************************/

void multiCompMasterProcessSignal_callbk(char* info, void *user_data)
{
  	CcspTraceInfo(("%s : Data Received is %s , user data is %s \n",__FUNCTION__,info,(char*)user_data));
    parseMasterData(info);

}

/*************************************************************************************************************************************

    caller:  multiCompSlaveProcessSignal_callbk

    prototype:

        void
        sendTimeoutToMaster
            (
              char *subdoc_name,
              size_t timeout         
            );

    description :
    
            	Function to send timeout details to master component

    argument:   
              	char *subdoc_name    --> subdoc name
              	size_t timeout       --> Timeout value needed to execute subdoc subdoc_name
    
****************************************************************************************************************************************/


void sendTimeoutToMaster(char* subdoc_name,size_t timeout)
{
        CcspTraceInfo(("Entering %s\n",__FUNCTION__));

	char data[256] = {0};
  memset(data,0,sizeof(data));

    	snprintf(data,sizeof(data),"%s,%s,%d,%u",process_name,subdoc_name,TIMEOUT_VALUE,timeout);

      sendDataToEvent(MASTER_COMP_SIGNAL_NAME,data);
}

/*************************************************************************************************************************************

    caller:  multiCompSlaveProcessSignal_callbk

    prototype:

        void
        rollbackLastExec
            (
              char *subdoc_name,
            );

    description :
    
           	Function to rollback of last execution based on the subdoc name received

    argument:   
              char *subdoc_name    --> subdoc name    
****************************************************************************************************************************************/

void rollbackLastExec(char* subdoc_name)
{

    CcspTraceInfo(("Entering %s\n",__FUNCTION__));

  	int ret = ROLLBACK_SUCCESS ;

    pthread_mutex_lock(&multiRegData_access);
      	multiCompSubDocReg *lmultiCompRegData;

      	lmultiCompRegData = multiCompSubDocInfo;
      	int i = 0;
      	for (i=0 ;i < gNumOfMultiCompSubDoc ; i ++)
      	{

        	if ( strcmp(subdoc_name,lmultiCompRegData->multi_comp_subdoc )  == 0 )
          	{

                	ret = lmultiCompRegData->rollbackFunc();
                	if ( ret != ROLLBACK_SUCCESS )
                 	{
                     		CcspTraceError(("rollback failed for %s, return value is %d\n",subdoc_name,ret));
                 	}
          	}

          	lmultiCompRegData++;
      	}
        
	   pthread_mutex_unlock(&multiRegData_access);


    	slaveExecutionCount--;
    	// Unreg slave signal if count is zero
   	if ( slaveExecutionCount == 0 )
    	{
          UnSubscribeFromEvent(SLAVE_COMP_SIGNAL_NAME);
          UnregisterFromEvent(SLAVE_COMP_SIGNAL_NAME);

    	}
      	return ;
}

/*************************************************************************************************************************************

    caller:  multiCompSlaveProcessSignal_callbk

    prototype:

        size_t
        getMultiCompTimeOut
            (
              char *subdoc_name,
            );

    description :
    
           	Function to get timeout value to execute the data by slave component

    argument:   
              char *subdoc_name    --> subdoc name    
****************************************************************************************************************************************/


size_t getMultiCompTimeOut(char* subdoc_name)
{
        CcspTraceInfo(("Entering %s\n",__FUNCTION__));
      	pthread_mutex_lock(&multiRegData_access);

      	multiCompSubDocReg *lmultiCompRegData;

      	lmultiCompRegData = multiCompSubDocInfo;
      	int i , ret = 0;
      	for (i=0 ;i < gNumOfMultiCompSubDoc ; i ++)
      	{

          	if ( strcmp(subdoc_name,lmultiCompRegData->multi_comp_subdoc )  == 0 )
          	{
                	if (lmultiCompRegData->calcTimeout)
                  {
                        ret = lmultiCompRegData->calcTimeout();
                        pthread_mutex_unlock(&multiRegData_access);
                        return ret;
                  }
                	else
               			  break;
          	}

          lmultiCompRegData++;
      }
      
      pthread_mutex_unlock(&multiRegData_access);

      return DEFAULT_TIMEOUT;
}

// Function to send blob execution results to master component

/*************************************************************************************************************************************

    caller:  multiCompSlaveProcessSignal_callbk, messageQueueProcessingMultiCompSlave

    prototype:

        void
        sendBlobExecutionResult
            (
                char* subdoc_name, 
                int exec_status, 
                int execRet, 
                char* execRetMsg
            );

    description :
    
          Function to send blob execution results to master component

    argument:   
              char *subdoc_name    --> subdoc name  
              int exec_status,     --> Execution status
              int execRet,         --> Return value of blob execution
              char* execRetMsg     --> Error message of blob execution
****************************************************************************************************************************************/

void sendBlobExecutionResult(char* subdoc_name, int exec_status, int execRet, char* execRetMsg)
{
  	CcspTraceInfo(("Entering %s\n",__FUNCTION__));
	char data[512] = {0} ;
 	 memset(data,0,sizeof(data));
    	snprintf(data,sizeof(data),"%s,%s,%d,%d,%s",process_name,subdoc_name,exec_status,execRet,execRetMsg);
    	slaveExecutionCount--;

      	sendDataToEvent(MASTER_COMP_SIGNAL_NAME,data);
      	// Unreg slave signal if count is zero
      	if ( slaveExecutionCount == 0 )
      	{
        	UnSubscribeFromEvent(SLAVE_COMP_SIGNAL_NAME);
          	UnregisterFromEvent(SLAVE_COMP_SIGNAL_NAME);
      	}
}

// Function to parse the data received from slave event
void parseSlaveData(const char* info)
{
        char *str = NULL;
        int data_sz = (sizeof(char) * MAX_DATA_SIZE_TO_SLAVE ) ;

        str = malloc(data_sz);

        memset(str,0,data_sz);
        snprintf(str,data_sz,"%s",info);

        char* rest = str;
        char *token[64];
        char *dataToQueue = NULL;

        int thread_retVal = 0;
        int i=-1, count=0 ;
        pthread_t tid_exec_slave;
        while ((token[++i] = strtok_r(rest, ",", &rest)) && count < 2 ) 
              count++;
    
        i = 0 ; 
        if ( strcmp(process_name,token[i]) == 0 )
        {
              i++;
              if ( atoi(token[i]) ==  BLOB_EXEC_REQUEST_TIMEOUT )
              {
                  size_t timeout = getMultiCompTimeOut(token[++i]);

                  sendTimeoutToMaster(token[i],timeout);
                  goto EXIT;
              }
              else if ( atoi(token[i]) ==  BLOB_EXEC_DATA )
              {
                  dataToQueue = malloc(data_sz);
                  memset(dataToQueue,0,data_sz);
                  snprintf(dataToQueue,data_sz,"%s,%s",token[++i],rest);

                  thread_retVal =pthread_create(&tid_exec_slave, NULL, ExecuteMultiCompRequest_thread,(void*)dataToQueue);

                  if ( 0 == thread_retVal)
                  {
                      CcspTraceInfo(("%s : ExecuteMultiCompRequest_thread created successfully \n",__FUNCTION__));
                  }
                  #if 0
                  mqd_t mq;
                  mq = mq_open(mCompMqSlaveName, O_WRONLY);
                  if ( (mqd_t)-1 == mq )
                  {
                        CcspTraceError(("%s message queue open failed , ERROR : %s\n",__FUNCTION__,strerror(errno)));
                        sendBlobExecutionResult(subdocInExec,EXECUTION_FAILED,MQUEUE_OPEN_FAILED,"message queue open failed");  
                        return;
                  }
                  if ( 0 != mq_send(mq, (char*) dataToQueue, sizeof(dataToQueue), 0))
                  {
                          sendBlobExecutionResult(subdocInExec,EXECUTION_FAILED,MQUEUE_SEND_FAILED,"message queue send failed");  
                          CcspTraceError(("%s message queue send failed , ERROR is : %s\n",__FUNCTION__,strerror(errno)));
                          if ((0 != mq_close(mq)))
                              CcspTraceError(("%s message queue close failed , ERROR is : %s\n",__FUNCTION__,strerror(errno)));
                        return;
                  }


                  if ((0 != mq_close(mq)))
                          CcspTraceError(("%s message queue close failed , ERROR is : %s\n",__FUNCTION__,strerror(errno)));
                  #endif
                  goto EXIT;
                }

          	else if ( atoi(token[i]) ==  ROLLBACK_LAST_REQUEST )
                {
                  	rollbackLastExec(token[++i]);
                  	goto EXIT;
              	} 
      }

EXIT :
      if (str!=NULL)
      {
        free(str);
        str = NULL;
      }
      return ;
}

/*************************************************************************************************************************************

    caller:    Callback function

    prototype:

        void
        multiCompSlaveProcessSignal_callbk
        (
            char* info, void *user_data
        );

    description :
    
           Callback function of the slave component, master will send the data in multiCompSlaveProcessSignal
           Slave will recieve and parse the data and for timeout request it will call timeout handler of the subdoc
           and in case of data it will snd the data in mCompMqSlaveName queue. Queue processing thread will 
           receive the data nad process it.
           
    argument:   
        char* info --> Data sent by the caller/master
        void *user_data  -->usuaully NULL value received, can have comp specific info

*******************************************************************************************************************************************/

void multiCompSlaveProcessSignal_callbk(char* info, void *user_data)
{

    	CcspTraceInfo(("%s : Data Received is %s , user data is %s \n",__FUNCTION__,info,(char*)user_data));
    	parseSlaveData(info);
		return;

}

/*************************************************************************************************************************************

    caller:    addEntryToMultiCompQueue

    prototype:

        int
        isMultiCompQueueFull
        (
        );

    description :
    
         Function to check if Queue is full
    
    return :
         returns 1 if queue is full , otherwise returns 0

****************************************************************************************************************************************/

int isMultiCompQueueFull() 
{  
	if( (mCompQueueData.front == mCompQueueData.rear + 1) || (mCompQueueData.front == 0 && mCompQueueData.rear == QUEUE_SIZE-1)) 
  	{
    		return 1;
  	}	 
	return 0;
} 


/*************************************************************************************************************************************

    caller:    checkIfVersionExecInMultiCompQueue,removeEntryFromMultiCompQueue

    prototype:

        int
        isMultiCompQueueEmpty
        (
        );

    description :
    
         Function to check if multi comp queue is empty. 
    
    return :
         returns 1 if queue is empty , otherwise returns 0

****************************************************************************************************************************************/

int isMultiCompQueueEmpty() 
{  
	if(mCompQueueData.front == -1) 
  	{	 
    		return 1;
  	}

  	return 0;

}


/*************************************************************************************************************************************

    caller:    PushMultiCompBlobRequest

    prototype:

        void
        removeEntryfromRearEndMultiComp
        (
        );

     description :
    
          Function removes entry from rear , required when mq_send call fails 

****************************************************************************************************************************************/

void removeEntryfromRearEndMultiComp()
{
    	pthread_mutex_lock(&mCompQueue_access);

    	mCompQueueData.txid_queue[mCompQueueData.rear] = 0;
    	mCompQueueData.version_queue[mCompQueueData.rear] = 0;
    	mCompQueueData.timeout_queue[mCompQueueData.rear] = 0;
    	mCompQueueData.blob_state_queue[mCompQueueData.rear] = NOT_STARTED;
    	mCompQueueData.execDataPointerAddr[mCompQueueData.rear] = NULL ;

    	if (mCompQueueData.front == mCompQueueData.rear)
    	{
          	mCompQueueData.front = -1;
          	mCompQueueData.rear = -1;
    	}
    	else
    	{
          	mCompQueueData.rear = (mCompQueueData.rear -1) % QUEUE_SIZE;     
	
         	if (mCompQueueData.rear < 0) 
          	{
                	mCompQueueData.rear += QUEUE_SIZE;   
          	}
    	}
    	pthread_mutex_unlock(&mCompQueue_access);
}

/*************************************************************************************************************************************

    caller:    messageQueueProcessingMultiComp

    prototype:

        void
        removeEntryFromMultiCompQueue
        (
        );

    description :
    
          Function removes entry from queue after blob execution is complete

****************************************************************************************************************************************/

void removeEntryFromMultiCompQueue() 
{ 
	    pthread_mutex_lock(&mCompQueue_access);

    	if (isMultiCompQueueEmpty())
    	{
        	CcspTraceInfo(("%s queue empty\n",__FUNCTION__));
        	pthread_mutex_unlock(&mCompQueue_access);

        	return ; 

    	}
        mCompQueueData.txid_queue[mCompQueueData.front] = 0;
        mCompQueueData.version_queue[mCompQueueData.front] = 0;
        mCompQueueData.timeout_queue[mCompQueueData.front] = 0;
        mCompQueueData.blob_state_queue[mCompQueueData.front] = NOT_STARTED;
        mCompQueueData.execDataPointerAddr[mCompQueueData.front] = NULL ;

      	if (mCompQueueData.front == mCompQueueData.rear)
      	{
        	      mCompQueueData.front = -1;
                mCompQueueData.rear = -1;
      	}
    	else
    	{
      		mCompQueueData.front = (mCompQueueData.front + 1) % QUEUE_SIZE;
    	}

    	pthread_mutex_unlock(&mCompQueue_access);

} 
/*************************************************************************************************************************************

    caller:    PushMultiCompBlobRequest

    prototype:

        int
        addEntryToMultiCompQueue
            (
              uint32_t version,
              uint16_t txid, 
              unsigned long timeout,
              void *exec_data
            );

    description :
    
          To Add entry to queue which maintains the txid, version and timeout of blob requests

    argument:   
          uint32_t version --> Version number of the new request
          uint16_t txid   -->  transaction id of new request
          unsigned long timeout --> timeout value of new request
          void *exec_data    --> exec_data pointer address , which will be used to free the memory after blob exec
  
    return :
          returns 1 if adding entry to queue is success , otherwise returns 0

****************************************************************************************************************************************/

int addEntryToMultiCompQueue(uint32_t version,uint16_t txid, unsigned long timeout,void *exec_data) 
{ 

	pthread_mutex_lock(&mCompQueue_access);

  	if (isMultiCompQueueFull() )
    	{ 
      		CcspTraceInfo(("Queue full\n"));
      		pthread_mutex_unlock(&mCompQueue_access);

          	return 1; 
    	}

    	if(mCompQueueData.front == -1) 
      		mCompQueueData.front = 0;

    
    	mCompQueueData.rear = (mCompQueueData.rear + 1) % QUEUE_SIZE;

    	mCompQueueData.txid_queue[mCompQueueData.rear] = txid; 
    	mCompQueueData.version_queue[mCompQueueData.rear] = version ;
    	mCompQueueData.timeout_queue[mCompQueueData.rear] = timeout;
    	mCompQueueData.blob_state_queue[mCompQueueData.rear] = PENDING;
  	mCompQueueData.execDataPointerAddr[mCompQueueData.rear] = (execData*) exec_data;

 	pthread_mutex_unlock(&mCompQueue_access);

  	return 0;

} 

/*************************************************************************************************************************************

    caller:    PushMultiCompBlobRequest

    prototype:

        size_t
        getMultiCompPendingQueueTimeout
            (
        uint16_t txid
            );

    description :
    	        Function to calculate queue timeout , need to add all queue timeout to subdoc execution time when requests are in queue 

    argument:   
          	uint16_t txid -->transaction id number
    
    return :
          	returns pending timeout value 
****************************************************************************************************************************************/

size_t getMultiCompPendingQueueTimeout(uint16_t txid)
{
	int queueTimeout = 0;
        CcspTraceInfo(("Entering %s\n",__FUNCTION__));

  	pthread_mutex_lock(&mCompQueue_access);
    	if (isMultiCompQueueEmpty())
    	{
        	CcspTraceInfo(("%s : Queue Empty\n",__FUNCTION__));
        	pthread_mutex_unlock(&mCompQueue_access);

        	return queueTimeout ; 

    	}
	int i ;
  	for( i = mCompQueueData.front; i!=mCompQueueData.rear; i=(i+1)%QUEUE_SIZE) 
    	{
        	if ( mCompQueueData.txid_queue[i] == txid )
        	{
      			pthread_mutex_unlock(&mCompQueue_access);
          		return queueTimeout;
        	} 
          	queueTimeout += mCompQueueData.timeout_queue[i];
      	}
      	if ( mCompQueueData.txid_queue[i] == txid )
      	{
    		pthread_mutex_unlock(&mCompQueue_access);
        	return queueTimeout;
      	}
	queueTimeout += mCompQueueData.timeout_queue[i];
  	pthread_mutex_unlock(&mCompQueue_access);

    return queueTimeout; 
}

/*************************************************************************************************************************************

    caller:  messageQueueProcessingMultiComp

    prototype:

        void
        sendBroadCastEvent
            (
              char *compName,
              char *subdocName          
            );

    description :
    
         	Function to send parameter details to slave component for execution


    argument:   
              char *compName    --> slave component name
              char *subdocName  --> subdoc name  
              MultiComp_ExecInfo  *multiCompExecData  --> This struct has the data which needs to be sent to slave component
    
****************************************************************************************************************************************/
//newly modified changes 
void sendDataToSlaveComp(char* compName , char* subdoc_name, char*  blob)
{
      	CcspTraceInfo(("Entering %s, blob is %s\n\n",__FUNCTION__,blob));

	    char *data = NULL;
      int data_sz = (sizeof(char) * MAX_DATA_SIZE_TO_SLAVE ) ;

      data = malloc (data_sz) ;
      if (data)
      {
          memset(data,0,data_sz);
          snprintf(data,data_sz,"%s,%d,%s,",compName,BLOB_EXEC_DATA,subdoc_name);

          memcpy(data+strlen(data),blob,data_sz-strlen(data)-1);
          CcspTraceInfo(("%s, Data to be sent is %s\n",__FUNCTION__,data));
          sendDataToEvent(SLAVE_COMP_SIGNAL_NAME,data);
          free(data);
          data = NULL;
     
      }
      else
      {
        CcspTraceError(("%s : mem alloc failed\n",__FUNCTION__));
      }
}

/*************************************************************************************************************************************

    caller:  messageQueueProcessingMultiComp

    prototype:

        void
        sendBroadCastEvent
            (
              char *compName,
              char *subdocName          
            );

    description :
    
          	Send broadcast event , destination/slave comp will receive the request and respond to a broadcast request

    argument:   
              char *compName    --> slave component name
              char *subdocName  --> subdoc name  
    
****************************************************************************************************************************************/

void sendBroadCastEvent(char* compName,char* subdoc_name )
{
    	CcspTraceInfo(("Entering %s\n",__FUNCTION__));
	    char data[256] = {0};
    	snprintf(data,sizeof(data),"%s,%d,%s",compName,IAM_MASTER,subdoc_name);
    	sendDataToEvent(BROADCASTSIGNAL_NAME,data);
}

/*************************************************************************************************************************************

    caller:  messageQueueProcessingMultiComp

    prototype:

        void
        sendSignaltoRollBack
            (
              char *compName,
              char *subdocName          
            );

    description :
    
              	In case of execution failure , send signal to rollback other components which has initiated / completed the execution
    argument:   
              	char *compName    --> slave component name
              	char *subdocName  --> subdoc name  
    
****************************************************************************************************************************************/

void sendSignaltoRollBack(char* comp_name, char* subdoc_name)
{

        CcspTraceInfo(("Entering %s\n",__FUNCTION__));

        sendBroadCastEvent(comp_name,subdoc_name);

        char data[256] = {0};
        memset(data,0,sizeof(data));

        snprintf(data,sizeof(data),"%s,%d,%s",comp_name,ROLLBACK_LAST_REQUEST,subdoc_name);


        struct timespec abs_time;

        pthread_mutex_lock(&MultiCompMutex);

        memset(&abs_time, 0, sizeof(abs_time));

        clock_gettime(CLOCK_MONOTONIC, &abs_time);

        abs_time.tv_sec += MAX_RESPONSE_TIME ;
        abs_time.tv_nsec += 0;

        int err = pthread_cond_timedwait(&MultiCompCond, &MultiCompMutex, &abs_time);
        if ( err == ETIMEDOUT )
        {
            CcspTraceError(("%s : Component failed to respond to rollback the request\n",__FUNCTION__));   
            return;                  
        }

        pthread_mutex_unlock(&MultiCompMutex);

        sendDataToEvent(SLAVE_COMP_SIGNAL_NAME,data);
}

/*************************************************************************************************************************************

    caller:  messageQueueProcessingMultiComp

    prototype:

        void
        requestTimeOutFromSlave
            (
              char *compName,
              char *subdocName          
            );

    description :
    
          	send signal to reuquest the timeout from slave component. Both slave comp name and subdoc name to passed since sometimes
          	SLAVE_COMP_SIGNAL_NAME signal can be registered by multiple components

    argument:   
              char *compName    --> slave component name
              char *subdocName  --> subdoc name  
    
****************************************************************************************************************************************/

void requestTimeOutFromSlave(char *compName,char *subdocName)
{
    	CcspTraceInfo(("Entering %s\n",__FUNCTION__));
	   char data[256] = {0};
    	snprintf(data,sizeof(data),"%s,%d,%s",compName,BLOB_EXEC_REQUEST_TIMEOUT,subdocName);
    	sendDataToEvent(SLAVE_COMP_SIGNAL_NAME,data);
}


/*************************************************************************************************************************************

    caller:  messageQueueProcessingMultiCompSlave

    prototype:

        void*
        execute_request_slave
            (
              void *data            
            );

    description :
    
                 timed thread to execute the blob request of the slave component

    argument:   
                 void *data  --> Data received from master component. Contains parameters based on feature.


****************************************************************************************************************************************/

void* execute_request_slave(void *data)
{

	CcspTraceInfo(("Inside FUNC %s LINE %d \n",__FUNCTION__,__LINE__));

  	int oldtype;

      	/* allow the thread to be killed at any time */
    	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldtype);
    	pthread_detach(pthread_self());
    	pthread_mutex_lock(&multiRegData_access);


    	multiCompSubDocReg *lmultiCompRegData;

    	lmultiCompRegData = multiCompSubDocInfo;
    	int j ;
    	for (j=0; j < gNumOfMultiCompSubDoc ; j++)
    	{
        	if ( strcmp(subdocInExec,lmultiCompRegData->multi_comp_subdoc )  == 0 )
          	{
                	break;
          	}

          	lmultiCompRegData++;
    	}

    	if ( lmultiCompRegData->executeBlobRequest )
    	{
        	execReturnMultiCompSlave = lmultiCompRegData->executeBlobRequest((char*)data);

    	}


    	pthread_mutex_unlock(&multiRegData_access);

    	/* wake up the caller if execution is completed in time */
    	pthread_cond_signal(&mCompWebconfig_slave_exec_completed);
    
    	return NULL;
}

/*************************************************************************************************************************************

    caller:    initMessageQueue

    prototype:

        void*
        messageQueueProcessingMultiCompSlave
        (
        );

    description :
                  thread to receive and executes the slave component data. 
                  Based on the subdoc name received , messageQueueProcessingMultiCompSlave calls the register execution handler .
                  It sends success/failure to master component to signal

****************************************************************************************************************************************/

void* ExecuteMultiCompRequest_thread(void* arg)
{

        CcspTraceInfo(("Inside FUNC %s LINE %d \n",__FUNCTION__,__LINE__));

        char *data = (char*)arg ;
       // char data[MAX_DATA_SIZE_TO_SLAVE] = {0};
       // memset(data,0,sizeof(data));
       // strncpy(data,(char*)arg,sizeof(data)-1);
        pthread_detach(pthread_self());
        CcspTraceInfo(("Data received is %s \n",data));

        struct timespec abs_time;

        pthread_t tid;
        int err = 0, pthreadRetValue = 0,rollbkRet = 0;

        multiCompSubDocReg *lmultiCompRegData = NULL;
        errno = 0;
        memset(&abs_time, 0, sizeof(abs_time));
        memset(subdocInExec,0,sizeof(subdocInExec));
        err = 0 ; pthreadRetValue = 0; rollbkRet = 0 ;
        int j = 0;

        while (1)
        {
            pthread_mutex_lock(&queue_access);
            pthread_mutex_lock(&mCompQueue_access);

            if ( (isQueueEmpty())  && (isMultiCompQueueEmpty() ) )
            {   
                CcspTraceInfo(("%s : Queue is Empty, executing slave request \n",__FUNCTION__));
                              
                pthread_mutex_unlock(&mCompQueue_access);

                pthread_mutex_unlock(&queue_access);
                break;
            }

                      
            pthread_mutex_unlock(&mCompQueue_access);
            pthread_mutex_unlock(&queue_access);          
            sleep(5);
            
        }
                pthread_mutex_lock(&webconfig_exec);
                char* rest  = data ;     
                char *token = NULL;
                token = strtok_r(rest, ",", &rest) ;

                if ( token != NULL )
                {
                  strncpy(subdocInExec, token,sizeof(subdocInExec)-1);
                }

                // TODO : need to discuss this , using the lock in rollback and getTimeout as well. Need to see how efficiently we can use the lock
                pthread_mutex_lock(&multiRegData_access);

                lmultiCompRegData = multiCompSubDocInfo;


                for (j=0; j < gNumOfMultiCompSubDoc ; j++)
                {
                    if ( strcmp(subdocInExec,lmultiCompRegData->multi_comp_subdoc )  == 0 )
                    {
                          break;
                    }

                    lmultiCompRegData++;
                }
                pthread_mutex_unlock(&multiRegData_access);


                int timeout = getMultiCompTimeOut( subdocInExec );
                pthreadRetValue=pthread_create(&tid, NULL, execute_request_slave,(void*)rest);

                if ( 0 == pthreadRetValue )
                {

                    clock_gettime(CLOCK_MONOTONIC, &abs_time);

                    abs_time.tv_sec += MAX_FUNC_EXEC_TIMEOUT * timeout;
                    abs_time.tv_nsec += 0;

                    err = pthread_cond_timedwait(&mCompWebconfig_slave_exec_completed, &webconfig_exec, &abs_time);
                    //err = pthread_cond_timedwait(&mCompWebconfig_slave_exec_completed, &multiRegData_access, &abs_time);
                    if (err == ETIMEDOUT)
                    {
                          pthread_cancel(tid);

                          if (lmultiCompRegData->rollbackFunc)   
                          {
                                  rollbkRet = lmultiCompRegData->rollbackFunc(subdocInExec);

                                  if (rollbkRet != ROLLBACK_SUCCESS )
                                      CcspTraceError(("%s : Rolling back of data failed\n",__FUNCTION__));         
                          }        

                          pthread_mutex_unlock(&multiRegData_access);
                          sendBlobExecutionResult(subdocInExec,EXECUTION_FAILED,BLOB_EXECUTION_TIMEDOUT,"blob execution timedout"); 

                    }
                  
                  else
                  {
  
                          if ( BLOB_EXEC_SUCCESS == execReturnMultiCompSlave->ErrorCode )
                          {
                                CcspTraceInfo(("%s : Execution success , sending completed ACK\n",__FUNCTION__));
                                sendBlobExecutionResult(subdocInExec,EXECUTION_SUCCESSFULLY_COMPLETED,BLOB_EXEC_SUCCESS,"");  
                          }
                          else
                          {
                              CcspTraceError(("%s : Execution failed Error Code :%hu Reason: %s \n",__FUNCTION__,execReturnMultiCompSlave->ErrorCode,execReturnMultiCompSlave->ErrorMsg));

                              pthread_mutex_lock(&multiRegData_access);

                              if ( (lmultiCompRegData->rollbackFunc) && ( VALIDATION_FALIED != execReturnMultiCompSlave->ErrorCode ) )
                              { 
                                    if (lmultiCompRegData->rollbackFunc )
                                    {
                                            rollbkRet = lmultiCompRegData->rollbackFunc(subdocInExec);

                                            if (rollbkRet != ROLLBACK_SUCCESS )
                                                  CcspTraceError(("%s : Rolling back of data failed\n",__FUNCTION__));    
                                    }

                              }
                              pthread_mutex_unlock(&multiRegData_access);
                              sendBlobExecutionResult(subdocInExec,EXECUTION_FAILED,execReturnMultiCompSlave->ErrorCode,execReturnMultiCompSlave->ErrorMsg);  


                          }

                  }
                } 
                else
                {
                  CcspTraceError(("%s: execute_request pthread_create failed , ERROR : %s \n", __FUNCTION__,strerror(errno)));

                      execReturnMultiCompSlave = lmultiCompRegData->executeBlobRequest((char*)rest);

                      if ( BLOB_EXEC_SUCCESS == execReturnMultiCompSlave->ErrorCode )
                      {
                              CcspTraceInfo(("%s : Execution success , sending completed ACK\n",__FUNCTION__));
                              sendBlobExecutionResult(subdocInExec,EXECUTION_SUCCESSFULLY_COMPLETED,BLOB_EXEC_SUCCESS,"");  
                      }
                      else
                    {
                          CcspTraceError(("%s : Execution failed Error Code :%hu Reason: %s \n",__FUNCTION__,execReturnMultiCompSlave->ErrorCode,execReturnMultiCompSlave->ErrorMsg));
                          pthread_mutex_lock(&multiRegData_access);

                          if ( ( lmultiCompRegData->rollbackFunc ) && ( VALIDATION_FALIED != execReturnMultiCompSlave->ErrorCode ) )
                          {
                                  rollbkRet = lmultiCompRegData->rollbackFunc(subdocInExec);

                                  if (rollbkRet != ROLLBACK_SUCCESS )
                                        CcspTraceError(("%s : Rolling back of data failed\n",__FUNCTION__));
                          }

                          pthread_mutex_unlock(&multiRegData_access);
                          sendBlobExecutionResult(subdocInExec,EXECUTION_FAILED,execReturnMultiCompSlave->ErrorCode,execReturnMultiCompSlave->ErrorMsg);  

                      }

                }

          if ( lmultiCompRegData->freeResources ) 
            lmultiCompRegData->freeResources();

          if (execReturnMultiCompSlave)
          {
                free(execReturnMultiCompSlave);
                execReturnMultiCompSlave = NULL;
          }

          if (data != NULL)
          {
              free(data);
              data = NULL;
          }
        pthread_mutex_unlock(&webconfig_exec);

        return NULL;
}

#if 0
/*************************************************************************************************************************************

    caller:    initMessageQueue

    prototype:

        void*
        messageQueueProcessingMultiCompSlave
        (
        );

    description :
                  thread to receive and executes the slave component data. 
                  Based on the subdoc name received , messageQueueProcessingMultiCompSlave calls the register execution handler .
                  It sends success/failure to master component to signal

****************************************************************************************************************************************/

void* messageQueueProcessingMultiCompSlave()
{
    CcspTraceInfo(("Inside FUNC %s LINE %d \n",__FUNCTION__,__LINE__));

    //  blobRegInfo *blobDataProcessing;

    //char *queue_name = (char*) arg;
    pthread_detach(pthread_self());

        struct timespec abs_time;

        pthread_t tid;
        int err, pthreadRetValue,rollbkRet;

        mqd_t mq;
        struct mq_attr attr;
        char buffer[MAX_DATA_SIZE_TO_SLAVE + 1];

        multiCompSubDocReg *lmultiCompRegData = NULL;

        /* initialize the queue attributes */
        attr.mq_flags = 0;
        attr.mq_maxmsg = MAX_EVENTS_IN_MQUEUE;
        attr.mq_msgsize = MAX_DATA_SIZE_TO_SLAVE;

        attr.mq_curmsgs = 0;
        errno = 0;
        char data[MAX_DATA_SIZE_TO_SLAVE] = {0};

        /* create the message queue */
          mq_unlink(mCompMqSlaveName);
        mq = mq_open(mCompMqSlaveName, O_CREAT | O_RDONLY, 0644, &attr);
        if ( (mqd_t)-1 == mq )
        {
            CcspTraceError(("message queue open failed , ERROR : %s. Returning from %s\n",strerror(errno),__FUNCTION__));
            return NULL;

        }
       
          int j;
        pthread_condattr_init(&mCompWebconfig_attr);
        pthread_condattr_setclock(&mCompWebconfig_attr, CLOCK_MONOTONIC);
        pthread_cond_init(&mCompWebconfig_slave_exec_completed, &mCompWebconfig_attr);

        while(1)
        {

            memset(&abs_time, 0, sizeof(abs_time));
            memset(buffer,0,sizeof(buffer));
            memset(subdocInExec,0,sizeof(subdocInExec));
            err = 0 ; pthreadRetValue = 0; rollbkRet = 0 ;
            errno = 0, j = 0;

            ssize_t bytes_read;

            /* receive the message */
            bytes_read = mq_receive(mq, buffer, MAX_DATA_SIZE_TO_SLAVE, NULL);

            if ( bytes_read >= 0 ) 
            {
            
                    buffer[bytes_read] = '\0';
                    // Processing pending
                    CcspTraceInfo(("Data received %s\n",__FUNCTION__));


                    while (1)
                    {
                            pthread_mutex_lock(&queue_access);
                            pthread_mutex_lock(&mCompQueue_access);

                            if ( (isQueueEmpty())  && (isMultiCompQueueEmpty() ) )
                            {   
                                    CcspTraceInfo(("%s : Queue is Empty, executing slave request \n",__FUNCTION__));
                                
                                    pthread_mutex_unlock(&mCompQueue_access);

                                    pthread_mutex_unlock(&queue_access);
                                    break;
                            }

                      
                            pthread_mutex_unlock(&mCompQueue_access);

                            pthread_mutex_unlock(&queue_access);
                      
                        sleep(5);
            
                    }
                    pthread_mutex_lock(&webconfig_exec);

                memset(data,0,sizeof(data));

                    snprintf(data,sizeof(data),"%s",buffer);
                    char* rest  = data ;     
                    char *token;
                    token = strtok_r(rest, ",", &rest) ;

                    strncpy(subdocInExec, token,sizeof(subdocInExec)-1);

                    // TODO : need to discuss this , using the lock in rollback and getTimeout as well. Need to see how efficiently we can use the lock
                    pthread_mutex_lock(&multiRegData_access);

                  lmultiCompRegData = multiCompSubDocInfo;


                    for (j=0; j < gNumOfMultiCompSubDoc ; j++)
                    {
                        if ( strcmp(subdocInExec,lmultiCompRegData->multi_comp_subdoc )  == 0 )
                        {
                                break;
                        }

                        lmultiCompRegData++;
                    }
                    pthread_mutex_unlock(&multiRegData_access);


                    int timeout = getMultiCompTimeOut( subdocInExec );
                    pthreadRetValue=pthread_create(&tid, NULL, execute_request_slave,(void*)rest);

                    if ( 0 == pthreadRetValue )
                    {

                        clock_gettime(CLOCK_MONOTONIC, &abs_time);

                        abs_time.tv_sec += MAX_FUNC_EXEC_TIMEOUT * timeout;
                        abs_time.tv_nsec += 0;

                        err = pthread_cond_timedwait(&mCompWebconfig_slave_exec_completed, &webconfig_exec, &abs_time);
                    //err = pthread_cond_timedwait(&mCompWebconfig_slave_exec_completed, &multiRegData_access, &abs_time);
                        if (err == ETIMEDOUT)
                        {
                                pthread_cancel(tid);

                                sendBlobExecutionResult(subdocInExec,EXECUTION_FAILED,BLOB_EXECUTION_TIMEDOUT,"blob execution timedout");  
                                // TODO : Need to discuss lock, it is not unlocked from thread in timeout

                          //pthread_mutex_lock(&multiRegData_access);

                    			if (lmultiCompRegData->rollbackFunc)   
                    			{
                            			rollbkRet = lmultiCompRegData->rollbackFunc(subdocInExec);

                            			if (rollbkRet != ROLLBACK_SUCCESS )
                                   		CcspTraceError(("%s : Rolling back of data failed\n",__FUNCTION__));         
                    			}			   

                    			pthread_mutex_unlock(&multiRegData_access);


                		}
                  
                	else
               	 	{
  
                      		if ( BLOB_EXEC_SUCCESS == execReturnMultiCompSlave->ErrorCode )
                      		{
                            		CcspTraceInfo(("%s : Execution success , sending completed ACK\n",__FUNCTION__));
                            		sendBlobExecutionResult(subdocInExec,EXECUTION_SUCCESSFULLY_COMPLETED,BLOB_EXEC_SUCCESS,"");  
                      		}
                      		else
                      		{
                          		CcspTraceError(("%s : Execution failed Error Code :%hu Reason: %s \n",__FUNCTION__,execReturnMultiCompSlave->ErrorCode,execReturnMultiCompSlave->ErrorMsg));

                          		sendBlobExecutionResult(subdocInExec,EXECUTION_FAILED,execReturnMultiCompSlave->ErrorCode,execReturnMultiCompSlave->ErrorMsg);  
                          		pthread_mutex_lock(&multiRegData_access);

                          		if ( (lmultiCompRegData->rollbackFunc) && ( VALIDATION_FALIED != execReturnMultiCompSlave->ErrorCode ) )
                          		{ 
                                		if (lmultiCompRegData->rollbackFunc )
                                		{
                                        		rollbkRet = lmultiCompRegData->rollbackFunc(subdocInExec);

                                        		if (rollbkRet != ROLLBACK_SUCCESS )
                                             			CcspTraceError(("%s : Rolling back of data failed\n",__FUNCTION__));    
                                		}

                          		}
                          		pthread_mutex_unlock(&multiRegData_access);


                       		}

                 	}
              	} 

              	else
              	{
                	CcspTraceError(("%s: execute_request pthread_create failed , ERROR : %s \n", __FUNCTION__,strerror(errno)));

                     	execReturnMultiCompSlave = lmultiCompRegData->executeBlobRequest((char*)rest);

                    	if ( BLOB_EXEC_SUCCESS == execReturnMultiCompSlave->ErrorCode )
                    	{
                            	CcspTraceInfo(("%s : Execution success , sending completed ACK\n",__FUNCTION__));
                            	sendBlobExecutionResult(subdocInExec,EXECUTION_SUCCESSFULLY_COMPLETED,BLOB_EXEC_SUCCESS,"");  
                    	}
                    	else
                   	{
                        	CcspTraceError(("%s : Execution failed Error Code :%hu Reason: %s \n",__FUNCTION__,execReturnMultiCompSlave->ErrorCode,execReturnMultiCompSlave->ErrorMsg));
                        	sendBlobExecutionResult(subdocInExec,EXECUTION_FAILED,execReturnMultiCompSlave->ErrorCode,execReturnMultiCompSlave->ErrorMsg);  
                        	pthread_mutex_lock(&multiRegData_access);

                        	if ( ( lmultiCompRegData->rollbackFunc ) && ( VALIDATION_FALIED != execReturnMultiCompSlave->ErrorCode ) )
                        	{
                                	rollbkRet = lmultiCompRegData->rollbackFunc(subdocInExec);

                                	if (rollbkRet != ROLLBACK_SUCCESS )
                                      	CcspTraceError(("%s : Rolling back of data failed\n",__FUNCTION__));
                        	}

                        	pthread_mutex_unlock(&multiRegData_access);

                    	}

               	}


          }

          if ( lmultiCompRegData->freeResources ) 
          	lmultiCompRegData->freeResources();

          if (execReturnMultiCompSlave)
          {
              	free(execReturnMultiCompSlave);
              	execReturnMultiCompSlave = NULL;
          }

	  pthread_mutex_unlock(&webconfig_exec);

	}

      	return NULL;

}   

#endif   
/*************************************************************************************************************************************

    caller:  messageQueueProcessingMultiComp

    prototype:

        void*
        execute_request_master
            (
              void *arg            
            );

    description :
                Thread which executes master components request

    argument:   
                void *arg  --> The structure which is parsed from the blob. Contains parameters based on feature.

****************************************************************************************************************************************/

void* execute_request_master(void *arg)
{

  	CcspTraceInfo(("Inside FUNC %s LINE %d \n",__FUNCTION__,__LINE__));

  	int oldtype;
  	int j = 0;
      	/* allow the thread to be killed at any time */
    	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldtype);
    	pthread_detach(pthread_self());

    	execData *exec_data  = (execData*) arg;
    	execReturnMultiComp = exec_data->executeBlobRequest(gMasterExecData);
    	pthread_mutex_lock(&multiCompState_access);
    	if ( gMultiCompExecState == NULL )
    	{
        	CcspTraceInfo(("%s : gMultiCompExecState is NULL , returning \n",__FUNCTION__));

            	if ( execReturnMultiComp != NULL)
            	{
              		free(execReturnMultiComp);
              		execReturnMultiComp = NULL;
            	}

          	pthread_mutex_unlock(&multiCompState_access);

           	return NULL;
    	}
      	multiCompExecStatus *lMultiCompExecData = NULL ;

      	lMultiCompExecData =  gMultiCompExecState ;

      	j = 0;
      	for (j=0; j < gNumOfComponents ; j++)
      	{
       		if ( (strncmp(process_name,lMultiCompExecData->componentName,sizeof(process_name)-1 )) == 0 )
          	{
              		break;
          	}

          	lMultiCompExecData++;
      	}
      
      	if ( execReturnMultiComp->ErrorCode == BLOB_EXEC_SUCCESS )
      	{
        	lMultiCompExecData->executionStatus = 1 ;
      	}
                    
       	lMultiCompExecData->execResult =  execReturnMultiComp->ErrorCode ;
       	strncpy(lMultiCompExecData->execRetMsg,execReturnMultiComp->ErrorMsg,sizeof(lMultiCompExecData->execRetMsg)-1);

       	pthread_mutex_unlock(&multiCompState_access);

      	CcspTraceInfo(("completed %s LINE %d \n",__FUNCTION__,__LINE__));

    	/* wake up the caller if execution is completed in time */
      	pthread_cond_signal(&MultiCompCond);
    	
	return NULL;
}

/*************************************************************************************************************************************

    caller:    initMessageQueue

    prototype:

        void*
        messageQueueProcessing
            (
            );

    description :
    
                Thread to monitor multi component blob execution requests (message queue).
                After receiving the requests it waits for single component request to complete , priority is given to single component exec.
                If not master component, then need to send broadcast event to check if slave component is ready to receives the request. 
                If ready get the timeout and send the paramters to slave . If master create a thread and execute the request
                
                If execution needs to be run in paralles send data to all components involved and then
                wait for all component execution to complete . If execution fails/times out after MAX time , send NACK and rollback


****************************************************************************************************************************************/


void* messageQueueProcessingMultiComp()
{
      	CcspTraceInfo(("Inside FUNC %s LINE %d \n",__FUNCTION__,__LINE__));

      	blobRegInfo *blobDataProcessing;

      	//char *queue_name = (char*) arg;
      	pthread_detach(pthread_self());

      	struct timespec abs_time;

      	pthread_t tid;
      	int err, pthreadRetValue,rollbkRet;

      	mqd_t mq;
      	struct mq_attr attr;
      	char buffer[MAX_SIZE + 1];
        char blob_fName[128] = {0} ;
      	/* initialize the queue attributes */
      	attr.mq_flags = 0;
       	attr.mq_maxmsg = MAX_EVENTS_IN_MQUEUE;
      	attr.mq_msgsize = MAX_SIZE;
      	attr.mq_curmsgs = 0;
      	errno = 0;
      	/* create the message queue */
        int slaveSubscribeTimeCounter = 0 ;
      	mq_unlink(multiCompMqEventName);
      	mq = mq_open(multiCompMqEventName, O_CREAT | O_RDONLY, 0644, &attr);
      	if ( (mqd_t)-1 == mq )
      	{
        	CcspTraceError(("message queue open failed , ERROR : %s. Returning from %s\n",strerror(errno),__FUNCTION__));
        	return NULL;

      	}


      	pthread_condattr_init(&mCompWebconfig_attr);
      	pthread_condattr_setclock(&mCompWebconfig_attr, CLOCK_MONOTONIC);
     	// pthread_cond_init(&webconfig_exec_completed, &webconfig_attr);
      	pthread_cond_init(&MultiCompCond, &mCompWebconfig_attr); 


      	multiCompExecStatus *mCompExecState = NULL;

      	int totalTimeout = 0 ,  j = 0 , ret = 0;
      	int numOfSuccessFullExec = 0 ;
      	
	      while(1)
      	{
        	memset(&abs_time, 0, sizeof(abs_time));
        	memset(buffer,0,sizeof(buffer));
                memset(blob_fName,0,sizeof(blob_fName));
        	blobDataProcessing = NULL;
        	err = 0 ; pthreadRetValue = 0; rollbkRet = 0 ,totalTimeout = 0 , numOfSuccessFullExec = 0 ;
        	errno = 0;
          ret = 0;
           	j = 0;
        	ssize_t bytes_read;
        	gMasterExecData = NULL , gMultiCompDataAddress = NULL ;
          slaveSubscribeTimeCounter = 0 , gReadyToReceive = 0;
        	/* receive the message */
        	bytes_read = mq_receive(mq, buffer, MAX_SIZE, NULL);

        	if ( bytes_read >= 0 ) 
        	{
            
            		buffer[bytes_read] = '\0';
            		// Processing pending
            		CcspTraceInfo(("Data received %s\n",__FUNCTION__));

			// Waiting for single component request to complete , priority is given to single component

            		while (1)
            		{
                      		pthread_mutex_lock(&queue_access);

                      		if ( isQueueEmpty() )
                      		{   
                             		CcspTraceInfo(("%s : Queue is Empty, executing multicomp request \n",__FUNCTION__));
                             
                             		pthread_mutex_unlock(&queue_access);
                             		break;
                      		}

                
                      		pthread_mutex_unlock(&queue_access);
                      
               			sleep(5);
            
            		}

            		pthread_mutex_lock(&webconfig_exec);

            		execData *exec_data  = (execData*) buffer;

            		policySequence *sequenceData = (policySequence*) exec_data->user_data;

            		blobDataProcessing=getAddress(exec_data->subdoc_name);

            		CcspTraceInfo(("Execution needed in sequence %d\n",sequenceData->isExecInSequenceNeeded));

            		gMultiCompDataAddress = sequenceData->multiCompExecData;
            		pthread_mutex_lock(&multiCompState_access);

            		mCompExecState = (multiCompExecStatus*) malloc (sequenceData->numOfComponents*sizeof(multiCompExecStatus));
            		memset(mCompExecState,0,sequenceData->numOfComponents*sizeof(multiCompExecStatus));

            		gMultiCompExecState = mCompExecState ;


            		gNumOfComponents = sequenceData->numOfComponents;
                pthread_mutex_unlock(&multiCompState_access);

            		memset(subdocInExec,0,sizeof(subdocInExec));
            		strncpy(subdocInExec,exec_data->subdoc_name,sizeof(subdocInExec)-1); 
// TODO : make subscribe blocking

                 EventRegister(MASTER_COMP_SIGNAL_NAME);

            		 for (j=0 ; j <  sequenceData->numOfComponents ; j ++)
            		 {
                		CcspTraceInfo(("numOfComponents in multi comp request %d\n",sequenceData->numOfComponents));

                		// If not master , then need to send broadcast event to check if slave component is ready to receives the request. If ready get the timeout and send the paramters to slave 
                		if ( sequenceData->multiCompExecData->isMaster == 0 )
                		{
                    			pthread_mutex_lock(&multiCompState_access);
                    			strncpy(mCompExecState->componentName,sequenceData->multiCompExecData->CompName,sizeof(mCompExecState->componentName)-1);
                   			  mCompExecState->executionStatus = 0;
                    			mCompExecState->timeout = 0 ;
                    			pthread_mutex_unlock(&multiCompState_access);
                    			sendBroadCastEvent(sequenceData->multiCompExecData->CompName,subdocInExec);

                                	slaveSubscribeTimeCounter =0 ;
                                	while(1)
                                	{
                                    		ret = subscribeToEvent(MASTER_COMP_SIGNAL_NAME);
                                    		if ( RBUS_ERROR_SUCCESS == ret )
                                    		{
                                          		break;
                                    		}
                                      
                                    		else if (slaveSubscribeTimeCounter > MAX_RESPONSE_TIME )
                                      		{
                                          		CcspTraceError(("Unable to subscribe to event %s with rbus \n", MASTER_COMP_SIGNAL_NAME));
                                          		send_NACK(exec_data->subdoc_name,mCompQueueData.txid_queue[mCompQueueData.front],exec_data->version,SLAVE_RESPONSE_TIME_OUT,"Slave response Timedout");
                                          		goto ROLLBACK;
                                      		}

                                      		slaveSubscribeTimeCounter +=5;
                                      		sleep(5);
                                	}
                    			pthread_mutex_lock(&MultiCompMutex);
                    			memset(&abs_time, 0, sizeof(abs_time));
                    			clock_gettime(CLOCK_MONOTONIC, &abs_time);

                    			abs_time.tv_sec += MAX_RESPONSE_TIME ;
                    			abs_time.tv_nsec += 0;
                          		if ( gReadyToReceive == 0 )
                          		{
                              			err = pthread_cond_timedwait(&MultiCompCond, &MultiCompMutex, &abs_time);
                              			if ( err == ETIMEDOUT )
                              			{
                                  			send_NACK(exec_data->subdoc_name,mCompQueueData.txid_queue[mCompQueueData.front],exec_data->version,SLAVE_RESPONSE_TIME_OUT,"Slave response Timedout");
                                  			pthread_mutex_unlock(&MultiCompMutex);
                                  			goto ROLLBACK;
                              			} 
                          		}

                          		gReadyToReceive = 0;
					pthread_mutex_unlock(&MultiCompMutex);
                    			requestTimeOutFromSlave(sequenceData->multiCompExecData->CompName,exec_data->subdoc_name);

                    			pthread_mutex_lock(&MultiCompMutex);

                    			memset(&abs_time, 0, sizeof(abs_time));

                   	 		  clock_gettime(CLOCK_MONOTONIC, &abs_time);

                    			abs_time.tv_sec += MAX_RESPONSE_TIME ;
                    			abs_time.tv_nsec += 0;


                          		pthread_mutex_lock(&multiCompState_access);
                          		if ( mCompExecState->timeout == 0 )
                          		{
                              			pthread_mutex_unlock(&multiCompState_access);
                              			err = pthread_cond_timedwait(&MultiCompCond, &MultiCompMutex, &abs_time);

                            			if ( err == ETIMEDOUT )
                            			{
                                		// If times out , master will fail or rollback the request. Webconfig client will retry the request
                                			send_NACK(exec_data->subdoc_name,mCompQueueData.txid_queue[mCompQueueData.front],exec_data->version,SLAVE_RESPONSE_TIME_OUT,"Slave response Timedout");
                                			pthread_mutex_unlock(&MultiCompMutex);
                                			goto ROLLBACK;
                            			}
                          		}
                          		else
                          		{
                              			pthread_mutex_unlock(&multiCompState_access);
                          		} 
                          		pthread_mutex_unlock(&MultiCompMutex);

                          		pthread_mutex_lock(&multiCompState_access);
                    			totalTimeout += mCompExecState->timeout ; 
                          		pthread_mutex_unlock(&multiCompState_access);
                    			sendDataToSlaveComp(sequenceData->multiCompExecData->CompName,exec_data->subdoc_name,sequenceData->multiCompExecData->comp_exec_data);
					
					// If sequential execution is needed , wait for client reply that execution is completed , or wait till MAX timeout.
                    			if ( sequenceData->isExecInSequenceNeeded == 1 )
                    			{
                         			pthread_mutex_lock(&MultiCompMutex);
                         			memset(&abs_time, 0, sizeof(abs_time));
                         			clock_gettime(CLOCK_MONOTONIC, &abs_time);
                         			abs_time.tv_sec += MAX_FUNC_EXEC_TIMEOUT * mCompExecState->timeout ;
                         			abs_time.tv_nsec += 0;

                         			err = pthread_cond_timedwait(&MultiCompCond, &MultiCompMutex, &abs_time);

                         			if ( err == ETIMEDOUT )
                         			{
                               				send_NACK(exec_data->subdoc_name,mCompQueueData.txid_queue[mCompQueueData.front],exec_data->version,SLAVE_EXEC_TIME_OUT,"Slave comp Execution Timedout");
                               				pthread_mutex_unlock(&MultiCompMutex);
                               				goto ROLLBACK;
                         			}
                         			else
                         			{
                               				pthread_mutex_lock(&multiCompState_access);
                               				if ( mCompExecState->execResult != BLOB_EXEC_SUCCESS )
                               				{
                                    				send_NACK(exec_data->subdoc_name,mCompQueueData.txid_queue[mCompQueueData.front],exec_data->version,mCompExecState->execResult,mCompExecState->execRetMsg);
                                    				pthread_mutex_unlock(&MultiCompMutex);
                                    				pthread_mutex_unlock(&multiCompState_access);
                                    				goto ROLLBACK;
                                			}
                                			pthread_mutex_unlock(&multiCompState_access);
                            			}

                            			pthread_mutex_unlock(&MultiCompMutex);
                          		}        
                                
                                	UnSubscribeFromEvent(MASTER_COMP_SIGNAL_NAME);
                      		}
                      		else
                      		{
                          		strncpy(mCompExecState->componentName,process_name,sizeof(mCompExecState->componentName));
                          
                          		pthread_mutex_lock(&MultiCompMutex);
                          		// TODO : calcTimeout func parameter in case of masterComp timeout
                          		int masterTimeout = exec_data->calcTimeout(1) ;

                          		totalTimeout += masterTimeout ; 

                          		if ( exec_data->executeBlobRequest )
                          		{
                              			gMasterExecData = sequenceData->multiCompExecData->comp_exec_data ;
                                  		pthreadRetValue=pthread_create(&tid, NULL, execute_request_master,(void*)buffer);
                                  		if ( 0 != pthreadRetValue )
                                  		{
                                    			CcspTraceError(("%s: execute_request pthread_create failed , ERROR : %s \n", __FUNCTION__,strerror(errno)));

                                    			execReturnMultiComp = exec_data->executeBlobRequest(sequenceData->multiCompExecData->comp_exec_data);

                                    			if ( BLOB_EXEC_SUCCESS == execReturn->ErrorCode )
                                    			{
                                        			mCompExecState->executionStatus = 1;

                                        			CcspTraceInfo(("%s : Execution success , sending completed ACK\n",__FUNCTION__));


                                    			}
                                    			else
                                    			{
                                      				CcspTraceError(("%s : Execution failed Error Code :%hu Reason: %s \n",__FUNCTION__,execReturnMultiComp->ErrorCode,execReturnMultiComp->ErrorMsg));
                                      				send_NACK(exec_data->subdoc_name,mCompQueueData.txid_queue[mCompQueueData.front],exec_data->version,execReturnMultiComp->ErrorCode,execReturnMultiComp->ErrorMsg);
                                      				if ( (exec_data->rollbackFunc) && ( VALIDATION_FALIED != execReturnMultiComp->ErrorCode ) )
                                      				{
                                        				rollbkRet = exec_data->rollbackFunc();
                                        				if (rollbkRet != ROLLBACK_SUCCESS )
                                                    				CcspTraceError(("%s : Rolling back of master comp data failed\n",__FUNCTION__));

                                              				pthread_mutex_unlock(&MultiCompMutex);
                                        				goto ROLLBACK;
                                      				}
                                    			}			
                                		}
                                		else if ( 0 == pthreadRetValue && sequenceData->isExecInSequenceNeeded )
                                		{
                                    			clock_gettime(CLOCK_MONOTONIC, &abs_time);
                                  			//  abs_time.tv_sec += MAX_FUNC_EXEC_TIMEOUT * mCompQueueData.timeout_queue[mCompQueueData.front] ;
                                    			// provide only master timeout
                                    			abs_time.tv_sec += MAX_FUNC_EXEC_TIMEOUT * exec_data->calcTimeout(1) ;
                                    			abs_time.tv_nsec += 0;
                                    			err = pthread_cond_timedwait(&MultiCompCond, &MultiCompMutex, &abs_time);
                                    			if (err == ETIMEDOUT)
                                    			{
                                         			pthread_cancel(tid);

                                         			CcspTraceError(("%s: subdoc %s , txid %hu , version %u execution timedout\n", __FUNCTION__,exec_data->subdoc_name,mCompQueueData.txid_queue[mCompQueueData.front],exec_data->version));
                                         			send_NACK(exec_data->subdoc_name,mCompQueueData.txid_queue[mCompQueueData.front],exec_data->version,BLOB_EXECUTION_TIMEDOUT,"Blob Execution Timedout");
                                         			pthread_mutex_unlock(&MultiCompMutex);
                                        			if ( exec_data->rollbackFunc )
                                        			{
                                             				rollbkRet = exec_data->rollbackFunc();
                                             				if (rollbkRet != ROLLBACK_SUCCESS )
                                                				CcspTraceError(("%s : Rolling back of master comp data failed\n",__FUNCTION__));
                                             				goto ROLLBACK;
                                         			}

                                    			}
                                    			else
                                    			{
                                      				if ( BLOB_EXEC_SUCCESS == execReturnMultiComp->ErrorCode )
                                      				{
                                          				CcspTraceInfo(("%s : Master component execution success\n",__FUNCTION__));
                                      				}
                                      				else
                                      				{
                                          				CcspTraceError(("%s : Execution failed Error Code :%hu Reason: %s \n",__FUNCTION__,execReturnMultiComp->ErrorCode,execReturnMultiComp->ErrorMsg));
                                          				send_NACK(exec_data->subdoc_name,mCompQueueData.txid_queue[mCompQueueData.front],exec_data->version,execReturnMultiComp->ErrorCode,execReturnMultiComp->ErrorMsg);
                                          				if ( (exec_data->rollbackFunc) && ( VALIDATION_FALIED != execReturnMultiComp->ErrorCode ) )
                                          				{ 
                                                				rollbkRet = exec_data->rollbackFunc();

                                                				if (rollbkRet != ROLLBACK_SUCCESS )
                                                    					CcspTraceError(("%s : Rolling back of master comp data failed\n",__FUNCTION__));
                                                
                                                				pthread_mutex_unlock(&MultiCompMutex);
                                                				goto ROLLBACK;
                                           				}
                                      				}
                                    			}
                                  		} 

                          		}
                         		else
                          		{ 
                            			CcspTraceError(("%s executeBlobRequest function pointer is NULL , Send NACK\n",__FUNCTION__));
                            			send_NACK(exec_data->subdoc_name,mCompQueueData.txid_queue[mCompQueueData.front],exec_data->version,NULL_BLOB_EXEC_POINTER,"Null Execution Pointer passed");
                            			pthread_mutex_unlock(&MultiCompMutex);
 
                            			goto ROLLBACK;
                          		}

                        		pthread_mutex_unlock(&MultiCompMutex);
                      		}
                      		sequenceData->multiCompExecData++;
                      		mCompExecState++ ;
                 	}

                 	if (  sequenceData->isExecInSequenceNeeded == 0 )
                 	{

                    		memset(&abs_time, 0, sizeof(abs_time));
                    		clock_gettime(CLOCK_MONOTONIC, &abs_time);
                    		abs_time.tv_sec += MAX_FUNC_EXEC_TIMEOUT * totalTimeout ;
                    		abs_time.tv_nsec += 0;

                    		// Wait for all component execution to complete , or timeout after MAX time. If execution fails call rollback
                    		while (1)
                    		{ 

                          		mCompExecState = gMultiCompExecState;
                          		pthread_mutex_lock(&multiCompState_access);
                          		numOfSuccessFullExec = 0;
                          		for (j=0 ; j < sequenceData->numOfComponents ; j++ )
                          		{

                            			 // if ( mCompExecState->componentName != '\0' || mCompExecState->componentName != '0' )
                              			{
                                    			if ( mCompExecState->executionStatus == 0 && mCompExecState->execResult != BLOB_EXEC_SUCCESS && mCompExecState->execResult !=0 )
                                    			{
                                        			send_NACK(exec_data->subdoc_name,mCompQueueData.txid_queue[mCompQueueData.front],exec_data->version,mCompExecState->execResult,mCompExecState->execRetMsg);
                                        			pthread_mutex_unlock(&multiCompState_access);
                                        			goto ROLLBACK;
                                    			} 
                              			}

                              			if ( mCompExecState->executionStatus == 1 )
                                  			numOfSuccessFullExec++ ;

                              			mCompExecState++;
                          		}

                         		pthread_mutex_unlock(&multiCompState_access);

                        		if ( numOfSuccessFullExec == sequenceData->numOfComponents )
                        		{
                              			CcspTraceInfo(("Execution completed, call post execution\n"));
                                    		if ( sequenceData->postExecSettings != NULL )
                                    		{
                                            		if ( POST_EXEC_SUCCESS == sequenceData->postExecSettings() )
                                            		{
                                                  		CcspTraceInfo(("post exec success , sending successful completion signal\n"));
                                            		}

                                                  	goto SENDACK;
                                           	 }
                                    		 else
                                    		 {
                                       			CcspTraceInfo(("post exec in NULL , sending successful completion signal\n"));
                                       			goto SENDACK;

                                    		}

                        		}

                        	pthread_mutex_lock(&MultiCompMutex);

                        	err = pthread_cond_timedwait(&MultiCompCond, &MultiCompMutex, &abs_time);

                        	if ( err == ETIMEDOUT )
                        	{
                                  	send_NACK(exec_data->subdoc_name,mCompQueueData.txid_queue[mCompQueueData.front],exec_data->version,SLAVE_EXEC_TIME_OUT,"Blob Execution Timedout");

                                    	pthread_mutex_unlock(&MultiCompMutex);
                                  	goto ROLLBACK;
                        	}

                        	pthread_mutex_unlock(&MultiCompMutex);

                    	    }

                	}
                	else
                	{                 
                      		if ( sequenceData->postExecSettings != NULL )
                      		{
                          		if ( POST_EXEC_SUCCESS == sequenceData->postExecSettings() )
                          		{
                              			CcspTraceInfo(("post exec success , sending successful completion signal\n"));

                          		}
                              		goto SENDACK;
                          	}
                      
                      		else
                      		{
                          		CcspTraceInfo(("post exec in NULL , sending successful completion signal\n"));
                          		goto SENDACK;
                      		}                	
                  	}


	// Update version and send ACK to webconfig client
SENDACK:
        updateVersionAndState(exec_data->version,execReturnMultiComp->ErrorCode,blobDataProcessing);
        send_ACK(exec_data->subdoc_name,mCompQueueData.txid_queue[mCompQueueData.front],exec_data->version,0,execReturnMultiComp->ErrorMsg);
        goto EXIT;


/* If any component exeuction fails, rollback needs to be called. In case of sequential execution rollback will be called based on executionStatus
In non sequential case rollback is called for all the components*/
ROLLBACK:

	CcspTraceInfo(("Rolling back the already executed data\n"));
        snprintf(blob_fName,sizeof(blob_fName),"/tmp/.%s%u",exec_data->subdoc_name,exec_data->version);
          CcspTraceInfo(("unlinking file %s\n",blob_fName));

        unlink(blob_fName);
        mCompExecState = gMultiCompExecState ;
    
        pthread_mutex_lock(&multiCompState_access);
        for (j=0; j < gNumOfComponents ; j++)
       	{
        	CcspTraceInfo(("CompName is %s, executionStatus is %d \n",mCompExecState->componentName,mCompExecState->executionStatus));
              	if (sequenceData->isExecInSequenceNeeded == 1 )
              	{
                	if ( mCompExecState->executionStatus == 1 )
                	{
                      		if ( strncmp(process_name,mCompExecState->componentName,sizeof(process_name) - 1) == 0 )
                      		{
                            		if ( (exec_data->rollbackFunc) && ( VALIDATION_FALIED != execReturnMultiComp->ErrorCode ) )
                            		{ 
                                  		rollbkRet = exec_data->rollbackFunc();

                                  		if (rollbkRet != ROLLBACK_SUCCESS )
                                          		CcspTraceError(("%s : Rolling back of master comp data failed\n",__FUNCTION__));


                            		}
                      		}
                      		else
                      		{     

                        		// TODO : need to discuss this lock
                            		pthread_mutex_unlock(&multiCompState_access);
                            		sendSignaltoRollBack(mCompExecState->componentName,subdocInExec);
                            		pthread_mutex_lock(&multiCompState_access);

                      		}
                  	}
              }

              else
              {
                      if ( strncmp(process_name,mCompExecState->componentName,sizeof(process_name)-1) == 0 )
                      {
                            if ( (exec_data->rollbackFunc) )
                            { 
                                  rollbkRet = exec_data->rollbackFunc();

                                  if (rollbkRet != ROLLBACK_SUCCESS )
                                          CcspTraceError(("%s : Rolling back of master comp data failed\n",__FUNCTION__));

                            }
                      }
                      else
                      {
                      		pthread_mutex_unlock(&multiCompState_access);
                            	sendSignaltoRollBack(mCompExecState->componentName,subdocInExec);
                            	pthread_mutex_lock(&multiCompState_access);
                      }
              }
              mCompExecState++;
        }
        pthread_mutex_unlock(&multiCompState_access);
        CcspTraceInfo((" Rollback complete\n"));

//  Unreg from master communication event and free all the allocated memories. 
EXIT:

        UnSubscribeFromEvent(MASTER_COMP_SIGNAL_NAME);
        UnregisterFromEvent(MASTER_COMP_SIGNAL_NAME);
        pthread_mutex_lock(&multiCompState_access);
        gNumOfComponents = 0;
        mCompExecState = gMultiCompExecState ;
        
	if (mCompExecState != NULL )
        {
        	free(mCompExecState);
                mCompExecState = NULL ;
                gMultiCompExecState = NULL ;
        }
        pthread_mutex_unlock(&multiCompState_access);
        sequenceData->multiCompExecData = gMultiCompDataAddress;

        if ( exec_data->freeResources ) 
                exec_data->freeResources(mCompQueueData.execDataPointerAddr[mCompQueueData.front]);

        if ( execReturnMultiComp != NULL)
        {
              free(execReturnMultiComp);
              execReturnMultiComp = NULL;
        }
     }     
     removeEntryFromMultiCompQueue();
     pthread_mutex_unlock(&webconfig_exec);

  }
  return NULL;
}

/*************************************************************************************************************************************

    caller:    checkNewVersionUpdateRequired

    prototype:

        size_t
        checkIfVersionExecInMultiCompQueue
            (    
              uint16_t txid
            );


    description :
    
                Function returns if version execution is in queue , if version in queue already then just need to update TX ID 

    argument:   
                uint32_t version --> version number of new request
                int *queueIndex  --> function need to fill this data and return to caller, caller will use this index to update the 
                new transaction id info

  return :
                returns VERSION_UPDATE_REQUIRED is subdoc has newer version , returns EXECUTION_IN_QUEUE is exec is in queue already
****************************************************************************************************************************************/


int checkIfVersionExecInMultiCompQueue(uint32_t version,int *queueIndex)
{
  	CcspTraceInfo(("Entering %s\n",__FUNCTION__));
    	pthread_mutex_lock(&mCompQueue_access);
   	if (isMultiCompQueueEmpty())
    	{
     		CcspTraceInfo(("%s : Queue Empty\n",__FUNCTION__));
    		pthread_mutex_unlock(&mCompQueue_access);

        	return VERSION_UPDATE_REQUIRED ; 

    	}
  	int i ;
        for( i = mCompQueueData.front; i!=mCompQueueData.rear; i=(i+1)%QUEUE_SIZE) 
      	{

    		if ( version == mCompQueueData.version_queue[i] )
    		{
            		*queueIndex = i;
            		pthread_mutex_unlock(&mCompQueue_access);

            		return EXECUTION_IN_QUEUE ;
       		}  
      	}
  	if ( version == mCompQueueData.version_queue[i] )
  	{
        	*queueIndex = i;
        	pthread_mutex_unlock(&mCompQueue_access);

    		return EXECUTION_IN_QUEUE ;
	}

        pthread_mutex_unlock(&mCompQueue_access);
    	return VERSION_UPDATE_REQUIRED; 
}


void initMessageQueueMaster()
{
    CcspTraceInfo(("Inside FUNC %s LINE %d \n",__FUNCTION__,__LINE__));

    pthread_t tid;

      int ret;

      snprintf(multiCompMqEventName,sizeof(multiCompMqEventName), "%s-%s",WEBCONFIG_QUEUE_NAME_MULTI_COMP,process_name);

      ret = pthread_create(&tid, NULL, &messageQueueProcessingMultiComp, NULL); 

      if ( ret != 0 )
        CcspTraceError(("%s: messageQueueProcessingMultiComp pthread_create failed , ERROR : %s \n", __FUNCTION__,strerror(errno)));

}

#if 0
void initMessageQueueSlave()
{
  CcspTraceInfo(("Inside FUNC %s LINE %d \n",__FUNCTION__,__LINE__));

    pthread_t tid;

    int ret;
    snprintf(mCompMqSlaveName,sizeof(mCompMqSlaveName), "%s-%s",WEBCONFIG_QUEUE_NAME_MULTI_COMP_SLAVE,process_name);
    
    ret = pthread_create(&tid, NULL, &messageQueueProcessingMultiCompSlave, NULL); 

    if ( ret != 0 )
      CcspTraceError(("%s: messageQueueProcessingMultiCompSlave pthread_create failed , ERROR : %s \n", __FUNCTION__,strerror(errno)));

}
#endif


void initMultiCompMaster()
{
    initMessageQueueMaster();
    eventRegisterMaster();
}


void initMultiCompSlave()
{
    #if 0
    initMessageQueueSlave();
    #endif
    pthread_condattr_init(&mCompWebconfig_attr);
    pthread_condattr_setclock(&mCompWebconfig_attr, CLOCK_MONOTONIC);
    pthread_cond_init(&mCompWebconfig_slave_exec_completed, &mCompWebconfig_attr);

    eventRegisterSlave();
}
/*************************************************************************************************************************************

    caller:    componenent calls this function after receivng the blob request(multi component request)

    prototype:

        void
        register_MultiComp_subdoc_handler
            (
                multiCompSubDocReg *mCompReg,
                int NumOfMCompSubdocs
            );

    description :
    
                This function is called by slave components during it's initialization , slave component maintains 
                table of timeout func,execution handler for blob, rollback and free resources for all the subdocs which component is slave

    argument:   
                multiCompSubDocReg *mCompReg,
                int NumOfMCompSubdocs    -->Todtal number of subdocs supported as a slave by component
****************************************************************************************************************************************/


void register_MultiComp_subdoc_handler(multiCompSubDocReg *mCompReg,int NumOfMCompSubdocs)
{
    	CcspTraceInfo(("Entering %s\n",__FUNCTION__));
    	gNumOfMultiCompSubDoc = NumOfMCompSubdocs;
        multiCompSubDocInfo = mCompReg ;
  
    	initMultiCompSlave();
}


/*************************************************************************************************************************************

    caller:    componenent calls this function after receivng the blob request(multi component request)

    prototype:

        void
        PushMultiCompBlobRequest
            (
              execData *exec_data
            );

    description :
    
              Function to decide and push the blob execution request to message queue 

    argument:   
              execData *exec_data --> Component fills policySequence structure , which has all the info needed to execute the blob request
    return :
              returns pointer address of the subdoc name , used to update newer version of subdoc

****************************************************************************************************************************************/


void PushMultiCompBlobRequest (execData *exec_data )
{

	CcspTraceError(("%s : subdoc_name %s , txid %hu, version %u , entries %lu\n",__FUNCTION__,exec_data->subdoc_name,exec_data->txid,exec_data->version,exec_data->numOfEntries));
  	unsigned long timeout = 0;
  	unsigned long timeout_to_webconfig = 0 ;
    int queueIndex=0;
    int retVal = 0 ;
      	mqd_t mq;
      	//struct mq_attr attr;
      	/* open the message queue */
    	if ( rbus_enabled == 1 && !gBroadcastSubscribed )
    	{
        	CcspTraceError(("Slave component is not yet ready to receive the requests, sending NACK \n"));
        	send_NACK(exec_data->subdoc_name,exec_data->txid,exec_data->version,SLAVE_NOT_READY,"Slave Comp Not Ready");  
        	goto EXIT;
    	}
  	retVal =  checkNewVersionUpdateRequired(exec_data,&queueIndex);
  	if ( VERSION_UPDATE_REQUIRED == retVal ) 
  	{
        	CcspTraceError(("New version available , prcessing new Blob request\n"));
        	mq = mq_open(multiCompMqEventName, O_WRONLY);
        	//CHECK((mqd_t)-1 != mq);
        	if ( (mqd_t)-1 == mq )
        	{
          		CcspTraceError(("%s message queue open failed , ERROR : %s\n",__FUNCTION__,strerror(errno)));
          		send_NACK(exec_data->subdoc_name,exec_data->txid,exec_data->version,MQUEUE_OPEN_FAILED,"MQ OPEN FAILED");  
          		goto EXIT;

        	}
    		if (!exec_data->calcTimeout)
    		{
      			CcspTraceError(("calculateTimeout is NULL , using default timeout routine\n"));
      			exec_data->calcTimeout = defFunc_calculateTimeout;
    		}
      		timeout = exec_data->calcTimeout(exec_data->numOfEntries);
          	timeout = timeout * MAX_FUNC_EXEC_TIMEOUT ;

      		CcspTraceError(("%s timeout received from calcTimeout is %lu\n",__FUNCTION__,timeout));
      		timeout_to_webconfig = timeout + (getPendingQueueTimeout(exec_data->txid)) + (getMultiCompPendingQueueTimeout(exec_data->txid)) ;
    		if (! (addEntryToMultiCompQueue(exec_data->version,exec_data->txid,timeout,exec_data)) )
    		{
   
               		CcspTraceError(("%s : Send received request ACK , timeout is %lu\n",__FUNCTION__,timeout_to_webconfig));
               		send_ACK(exec_data->subdoc_name,exec_data->txid,exec_data->version,timeout_to_webconfig,"");
                	if ( 0 != mq_send(mq, (char*) exec_data, sizeof(*exec_data), 0))
                	{
                    		CcspTraceError(("%s message queue send failed , ERROR is : %s\n",__FUNCTION__,strerror(errno)));
                    		send_NACK(exec_data->subdoc_name,exec_data->txid,exec_data->version,MQUEUE_SEND_FAILED,"MQ SEND FAILED");
                    		removeEntryfromRearEndMultiComp();

                    		if ((0 != mq_close(mq)))
                        		CcspTraceError(("%s message queue close failed , ERROR is : %s\n",__FUNCTION__,strerror(errno)));
                    		goto EXIT;

      			}
                	if ((0 != mq_close(mq)))
                    		CcspTraceError(("%s message queue close failed , ERROR is : %s\n",__FUNCTION__,strerror(errno)));

                	return;
    		}
    		else
    		{
      			CcspTraceInfo(("%s QUEUE FULL\n",__FUNCTION__));
      			send_NACK(exec_data->subdoc_name,exec_data->txid,exec_data->version,QUEUE_PUSH_FAILED,"Queue is Full");     
    		}
  	}
  	else if ( EXECUTION_IN_QUEUE == retVal )
  	{
    		CcspTraceInfo((" %s : Execution is in progress, updating transaction id\n",__FUNCTION__));
    		pthread_mutex_lock(&mCompQueue_access);

      		mCompQueueData.txid_queue[queueIndex] = exec_data->txid;
    		pthread_mutex_unlock(&mCompQueue_access);
    		if (!exec_data->calcTimeout)
    		{
      			CcspTraceInfo(("calculateTimeout is NULL , using default timeout routine\n"));
      			exec_data->calcTimeout = defFunc_calculateTimeout;
    		}
    		timeout = exec_data->calcTimeout(exec_data->numOfEntries);
        // for multi-comp request sending timeout*3 for webconfig 
        timeout = timeout * MAX_FUNC_EXEC_TIMEOUT ;
    		CcspTraceInfo(("%s timeout received from calcTimeout is %lu\n",__FUNCTION__,timeout));
    		timeout_to_webconfig = timeout + (getPendingQueueTimeout(exec_data->txid)) + (getMultiCompPendingQueueTimeout(exec_data->txid)) ;
    		CcspTraceInfo(("%s : Send received request ACK , timeout is %lu\n",__FUNCTION__,timeout_to_webconfig));
    		send_ACK(exec_data->subdoc_name,exec_data->txid,exec_data->version,timeout_to_webconfig,"");

  	}
  	else if ( VERSION_ALREADY_EXIST == retVal )
  	{
    		CcspTraceInfo(("Already having updated version, no need to prcess Blob request\n"));
    		send_ACK(exec_data->subdoc_name,exec_data->txid,exec_data->version,0,"");

  	}
  	else if ( SUBDOC_NOT_SUPPORTED == retVal )
  	{
    		CcspTraceError(("Subdoc not registered , support not available . sending NACK\n"));
    		send_NACK(exec_data->subdoc_name,exec_data->txid,exec_data->version,SUBDOC_NOT_SUPPORTED,"INVALID SUBDOC");
  	}
EXIT :
  	if ( exec_data->freeResources ) 
    		exec_data->freeResources(exec_data);    

}
#endif
