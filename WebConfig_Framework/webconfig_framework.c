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


#include "webconfig_framework.h"



char mqEventName[64] = {0};
char process_name[64]={0};

int gNumOfSubdocs = 0;

pthread_mutex_t webconfig_exec = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t webconfig_exec_completed = PTHREAD_COND_INITIALIZER;

pthread_mutex_t queue_access = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t reg_subdoc = PTHREAD_MUTEX_INITIALIZER;

pthread_condattr_t webconfig_attr;


PblobRegInfo blobData;

queueInfo  queueData;

setVersion updateVersion;
pErr execReturn = NULL;

/* Function/thread  for debugging purpuse. When FRAMEWORK_DEBUG is created thread logs all the registered sub docs and version 
 and also the requests in queue */
void* display_subDocs()
{

	CcspTraceInfo(("Inside FUNC %s LINE %d\n",__FUNCTION__,__LINE__));
	PblobRegInfo blobDisplayData;
	FILE *fp;
	int i;
    	int interval =DEFAULT_DEBUG_INTERVAL , num_of_dbg_iter=DEFAULT_DEBUG_ITER;

	pthread_detach(pthread_self());
	while (1)
	{

		sleep(DEFAULT_DEBUG_INTERVAL);
		fp = fopen (FRAMEWORK_DEBUG, "r");
	    	if( fp )
	    	{ 
	    		fscanf(fp, "%d %d", &interval,&num_of_dbg_iter);

			if ( interval <= 0 )
				interval = DEFAULT_DEBUG_INTERVAL;
			 
			if ( num_of_dbg_iter < 0 )
				num_of_dbg_iter = DEFAULT_DEBUG_ITER ;

			CcspTraceInfo(("interval is %d, num_of_dbg_iter is %d \n",interval,num_of_dbg_iter));

			do
			{
				pthread_mutex_lock(&reg_subdoc);

				blobDisplayData = blobData;

				for (i=0 ; i < gNumOfSubdocs ; i++)
				{	
					CcspTraceInfo(("SUBDOC_NAME is %s , current version is %u \n",blobDisplayData->subdoc_name, blobDisplayData->version));
    					blobDisplayData ++;

				}

				pthread_mutex_unlock(&reg_subdoc);

				i = 0;
				
				pthread_mutex_lock(&queue_access);


				if (isQueueEmpty())
			  	{		
					CcspTraceInfo(("%s : Queue Empty\n",__FUNCTION__));
			  	}

			  	else
			  	{
			  		for( i = queueData.front; i!=queueData.rear; i=(i+1)%QUEUE_SIZE) 
				    {
				  		  CcspTraceInfo(("TXID %hu version %u timeout %lu state %d \n",queueData.txid_queue[i],queueData.version_queue[i],queueData.timeout_queue[i],queueData.blob_state_queue[i]));

				    }
				    CcspTraceInfo(("TXID %hu version %u timeout %lu state %d \n",queueData.txid_queue[i],queueData.version_queue[i],queueData.timeout_queue[i],queueData.blob_state_queue[i]));


			  	}

			  	pthread_mutex_unlock(&queue_access);


				num_of_dbg_iter--;
				sleep(interval);

			} while((num_of_dbg_iter>0) && (access(FRAMEWORK_DEBUG, F_OK) == 0));

			fclose(fp);
			fp=NULL;

			unlink(FRAMEWORK_DEBUG);
			interval = DEFAULT_DEBUG_INTERVAL;
			num_of_dbg_iter = DEFAULT_DEBUG_ITER;
		}	
	}
	return NULL;
}

/* Function registers all the subdocs during component initialization */
void register_sub_docs(blobRegInfo *bInfo,int NumOfSubdocs, getVersion getv , setVersion setv )
{
	CcspTraceInfo(("Inside FUNC %s LINE %d\n",__FUNCTION__,__LINE__));

	gNumOfSubdocs=NumOfSubdocs;

	blobData= bInfo ;


	memset(&queueData, 0, sizeof(queueData));
	queueData.front = -1;
	queueData.rear = -1;
	int i ;

	if ( getv != NULL )
	{
	 	for (i=0 ; i < gNumOfSubdocs ; i++)
		{
			 bInfo->version = getv(bInfo->subdoc_name);
			 bInfo++;
		}
	}

	if ( setv != NULL )
			updateVersion = setv;


	// Intialize message queue
	initMessageQueue();

	pthread_t tid;

	int ret = pthread_create(&tid, NULL, &display_subDocs, NULL); 

	if ( ret != 0 )
		CcspTraceError(("%s: display_subDocs pthread_create failed , ERROR : %s \n", __FUNCTION__,strerror(errno)));


}

/* Function to notify subdoc version to webconfig client */
void notifyVersion_to_Webconfig(char* subdoc_name, uint32_t version,int process_crashed)
{

	CcspTraceInfo(("%s : doc name %s , doc version %u\n",__FUNCTION__,subdoc_name,version));

	char data[128]= {0};
    
    	if ( process_crashed == 1 )
    	{
   	   	 snprintf(data,sizeof(data),"%s,0,%u,%s",subdoc_name,version,COMPONENT_CRASH_EVENT);
    	}
    	else
    	{
        	snprintf(data,sizeof(data),"%s,0,%u,%s",subdoc_name,version,COMPONENT_INIT_EVENT);
    	}

	int ret = CcspBaseIf_WebConfigSignal(bus_handle, data);

	if ( ret != CCSP_SUCCESS )
		CcspTraceError(("%s : CcspBaseIf_WebConfigSignal failed,  ret value is %d\n",__FUNCTION__,ret));

}

/* Function to check if component is coming after crash , function takes component initialized file as an argument */

void check_component_crash(char* init_file)
{

	CcspTraceInfo(("Inside FUNC %s LINE %d, init file is %s \n",__FUNCTION__,__LINE__,init_file));

	/**
	 * Components based purely on RBUS will not invoke CCSP_Message_Bus_Init.
	 * Check and update rbus_enabled state .
	 */
    #ifndef _RBUS_NOT_REQ_
    rbus_enabled = (access("/nvram/rbus_support", F_OK) == 0);
    #endif

    	int comp_crashed = 0 ;
	int fd = access(init_file, F_OK); 
    	if(fd == 0)
    	{ 
		CcspTraceInfo(("%s file present, component is coming after crash. Need to notify webconfig \n",init_file )); 
        	comp_crashed = 1 ;
        }
        else
        {
                CcspTraceInfo(("%s file not present, need to send component init event to webconfig \n",init_file )); 

        }
    	pthread_mutex_lock(&reg_subdoc);
    	PblobRegInfo blobNotify;

    	blobNotify = blobData;

	int i ;
	for (i=0 ; i < gNumOfSubdocs ; i++)
	{
		notifyVersion_to_Webconfig (blobNotify->subdoc_name,blobNotify->version,comp_crashed);
	        blobNotify++;
	}

	pthread_mutex_unlock(&reg_subdoc);
}

/* Function available to calculate the timeout if component doesn't provide the timeout function */
size_t defFunc_calculateTimeout(size_t numOfEntries)
{
	// value in seconds
	return  (DEFAULT_TIMEOUT + (numOfEntries * DEFAULT_TIMEOUT_PER_ENTRY)) ;
}

/* Function to send NACK to webconfig client */
void send_NACK (char *subdoc_name, uint16_t txid, uint32_t version, uint16_t ErrCode,char *failureReason)  
{
		
	CcspTraceInfo(("%s : doc name %s , doc version %u, txid is %hu, ErrCode is %hu, Failure reason is %s\n",__FUNCTION__,subdoc_name,version,txid,ErrCode,failureReason));

	char data[256]= {0};
    	snprintf(data,sizeof(data),"%s,%hu,%u,NACK,0,%s,%hu,%s",subdoc_name,txid,version,process_name,ErrCode,failureReason);

	int ret = CcspBaseIf_WebConfigSignal(bus_handle, data);

	if ( ret != CCSP_SUCCESS )
		CcspTraceError(("%s : CcspBaseIf_WebConfigSignal failed,  ret value is %d\n",__FUNCTION__,ret));

}

/* Function to send ACK to webconfig client */

void send_ACK (char *subdoc_name, uint16_t txid, uint32_t version, unsigned long timeout,char *msg )
{
	CcspTraceInfo(("%s : doc name %s , doc version %u, txid is %hu  timeout is %lu\n",__FUNCTION__,subdoc_name,version,txid,timeout));

	char data[256]= {0};

	if ( msg[0] == '\0' || msg[0] == '0' )
	{
    		snprintf(data,sizeof(data),"%s,%hu,%u,ACK,%lu",subdoc_name,txid,version,timeout);
	}

    	else
    	{
       		snprintf(data,sizeof(data),"%s,%hu,%u,ACK;%s,%lu",subdoc_name,txid,version,msg,timeout);
	
	}

	int ret = CcspBaseIf_WebConfigSignal(bus_handle, data);

	if ( ret != CCSP_SUCCESS )
		CcspTraceError(("%s : CcspBaseIf_WebConfigSignal failed,  ret value is %d\n",__FUNCTION__,ret));

}

/* Function to update subdoc version and state after completion of blob request */

void updateVersionAndState(uint32_t version, int blob_execution_retValue,PblobRegInfo blobUpdate)
{
	pthread_mutex_lock(&queue_access);

	if ( blob_execution_retValue == BLOB_EXEC_SUCCESS )
	{
		
		pthread_mutex_lock(&reg_subdoc);

		if (updateVersion != NULL )
	   		 updateVersion(blobUpdate->subdoc_name,version);

	   	blobUpdate->version = version;

	   	 pthread_mutex_unlock(&reg_subdoc);

	    	queueData.blob_state_queue[queueData.front] = COMPLETED;
	}
	else
	{
	    	queueData.blob_state_queue[queueData.front]  = FAILED;
	}

	pthread_mutex_unlock(&queue_access);
}

/* Function to get pointer address , required to update the version after successfull completion of blob execution */
blobRegInfo* getAddress(char* subdoc_name)
{
	pthread_mutex_lock(&reg_subdoc);

	PblobRegInfo blobReturnPointer;
    	blobReturnPointer = blobData;
    	int i;
	for(i = 0 ; i <gNumOfSubdocs ; i++)
	{
		if( strcmp(blobReturnPointer->subdoc_name,subdoc_name) == 0 )
		{
			pthread_mutex_unlock(&reg_subdoc);
	    
			return blobReturnPointer;
		}
		blobReturnPointer++;
	}
	pthread_mutex_unlock(&reg_subdoc);

	return NULL;
}

/* Check if queue if full which maintanins the txid, version and timeout of blob requests */
int isQueueFull() 
{  
	if( (queueData.front == queueData.rear + 1) || (queueData.front == 0 && queueData.rear == QUEUE_SIZE-1)) 
	{
		return 1;
	}	

	return 0;
} 

/* To Add entry to queue which maintains the txid, version and timeout of blob requests */

int addEntryToQueue(uint32_t version,uint16_t txid, unsigned long timeout,void *exec_data) 
{ 

	pthread_mutex_lock(&queue_access);

	if (isQueueFull() )
   	{	
   		CcspTraceError(("Queue full\n"));
   		pthread_mutex_unlock(&queue_access);

        	return 1; 
   	}

   	if(queueData.front == -1) 
   		queueData.front = 0;

    
   	queueData.rear = (queueData.rear + 1) % QUEUE_SIZE;

   	queueData.txid_queue[queueData.rear] = txid; 
   	queueData.version_queue[queueData.rear] = version ;
   	queueData.timeout_queue[queueData.rear] = timeout;
   	queueData.blob_state_queue[queueData.rear] = PENDING;

	queueData.execDataPointerAddr[queueData.rear] = (execData*) exec_data;

	pthread_mutex_unlock(&queue_access);

	return 0;

} 
  
  /* Check if queue is empty. */

int isQueueEmpty() 
{  
	if(queueData.front == -1) 
	{	
		return 1;
	}

	return 0;

}
 
 /* To remove entry from rear , required when mq_send call fails */

void removeEntryfromRearEnd()
{
	pthread_mutex_lock(&queue_access);

	queueData.txid_queue[queueData.rear] = 0;
	queueData.version_queue[queueData.rear] = 0;
        queueData.timeout_queue[queueData.rear] = 0;
        queueData.blob_state_queue[queueData.rear] = NOT_STARTED;
	queueData.execDataPointerAddr[queueData.rear] = NULL ;

	if (queueData.front == queueData.rear)
    	{
		queueData.front = -1;
       		queueData.rear = -1;
   	}
  	else
  	{
		queueData.rear = (queueData.rear -1) % QUEUE_SIZE;     

       		if (queueData.rear < 0) 
        	{
            		queueData.rear += QUEUE_SIZE;   
        	}
  	}
   	pthread_mutex_unlock(&queue_access);
}

 /* To remove entry after completion of blob request execution */

void removeEntryFromQueue() 
{ 
	pthread_mutex_lock(&queue_access);

  	if (isQueueEmpty())
  	{
  	  	CcspTraceInfo(("%s queue empty\n",__FUNCTION__));
  	  	pthread_mutex_unlock(&queue_access);

  	  	return ; 

  	}


    	queueData.txid_queue[queueData.front] = 0;
    	queueData.version_queue[queueData.front] = 0;
    	queueData.timeout_queue[queueData.front] = 0;
       	queueData.blob_state_queue[queueData.front] = NOT_STARTED;
       	queueData.execDataPointerAddr[queueData.front] = NULL ;

       	if (queueData.front == queueData.rear)
		{
    	        queueData.front = -1;
                queueData.rear = -1;
        }
  	else
  	{
  		queueData.front = (queueData.front + 1) % QUEUE_SIZE;
  	}

   	pthread_mutex_unlock(&queue_access);

} 

/* timed thread to execute the blob request, */

void* execute_request(void *arg)
{

	CcspTraceInfo(("Inside FUNC %s LINE %d \n",__FUNCTION__,__LINE__));

	int oldtype;

    	/* allow the thread to be killed at any time */
   	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldtype);
   	pthread_detach(pthread_self());


    	execData *exec_data  = (execData*) arg;

	execReturn = exec_data->executeBlobRequest(exec_data->user_data);

   	/* wake up the caller if execution is completed in time */
    	pthread_cond_signal(&webconfig_exec_completed);
   	return NULL;
}

/* Thread to monitor all blob execution requests (message queue) */

void* messageQueueProcessing()
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

    	/* initialize the queue attributes */
   	 attr.mq_flags = 0;
    	attr.mq_maxmsg = MAX_EVENTS_IN_MQUEUE;
    	attr.mq_msgsize = MAX_SIZE;
    	attr.mq_curmsgs = 0;
    	errno = 0;
    	/* create the message queue */
    	mq_unlink(mqEventName);
    	mq = mq_open(mqEventName, O_CREAT | O_RDONLY, 0644, &attr);
    	if ( (mqd_t)-1 == mq )
    	{
    		CcspTraceError(("message queue open failed , ERROR : %s. Returning from %s\n",strerror(errno),__FUNCTION__));
    		return NULL;

    	}

 	pthread_condattr_init(&webconfig_attr);
	pthread_condattr_setclock(&webconfig_attr, CLOCK_MONOTONIC);
	pthread_cond_init(&webconfig_exec_completed, &webconfig_attr);

	// TODO check if pthread_cond_init needs to be done everytime event is received , also check if pthread_cond_destroy is required
    	while(1)
    	{

    		memset(&abs_time, 0, sizeof(abs_time));
    		memset(buffer,0,sizeof(buffer));

    		blobDataProcessing = NULL;
    		err = 0 ; pthreadRetValue = 0; rollbkRet = 0 ;
			errno = 0;

    		ssize_t bytes_read;

    		/* receive the message */
     		bytes_read = mq_receive(mq, buffer, MAX_SIZE, NULL);

     		if ( bytes_read >= 0 ) 
     		{
	      		
			buffer[bytes_read] = '\0';
	        	// Processing pending
	       		CcspTraceInfo(("Received event\n"));

	        	pthread_mutex_lock(&webconfig_exec);

		       	execData *exec_data  = (execData*) buffer;

		       	blobDataProcessing=getAddress(exec_data->subdoc_name);

			pthread_mutex_lock(&queue_access);

			queueData.blob_state_queue[queueData.front] = IN_PROGRESS;

			pthread_mutex_unlock(&queue_access);

		        if ( exec_data->executeBlobRequest )
			{
			 
				pthreadRetValue=pthread_create(&tid, NULL, execute_request,(void*)buffer);

			        if ( 0 == pthreadRetValue )
			        {

				       	clock_gettime(CLOCK_MONOTONIC, &abs_time);

				       	abs_time.tv_sec += MAX_FUNC_EXEC_TIMEOUT * queueData.timeout_queue[queueData.front] ;
				       	abs_time.tv_nsec += 0;

		        		err = pthread_cond_timedwait(&webconfig_exec_completed, &webconfig_exec, &abs_time);

		        		if (err == ETIMEDOUT)
		        		{
		        			pthread_cancel(tid);

		           			CcspTraceError(("%s: subdoc %s , txid %hu , version %u execution timedout\n", __FUNCTION__,exec_data->subdoc_name,queueData.txid_queue[queueData.front],exec_data->version));

						updateVersionAndState(exec_data->version,0,blobDataProcessing);
			            		send_NACK(exec_data->subdoc_name,queueData.txid_queue[queueData.front],exec_data->version,BLOB_EXECUTION_TIMEDOUT,"Blob Execution Timedout");
						
			            		if ( exec_data->rollbackFunc )
						{
							rollbkRet = exec_data->rollbackFunc();

							if (rollbkRet != ROLLBACK_SUCCESS )
			            				CcspTraceError(("%s : Rolling back of data failed\n",__FUNCTION__));
			            		
						}

				        }
				        	
				        else
				        {
	
			        		if ( BLOB_EXEC_SUCCESS == execReturn->ErrorCode )
			        		{
							CcspTraceInfo(("%s : Execution success , sending completed ACK\n",__FUNCTION__));
							updateVersionAndState(exec_data->version,execReturn->ErrorCode,blobDataProcessing);
							send_ACK(exec_data->subdoc_name,queueData.txid_queue[queueData.front],exec_data->version,0,execReturn->ErrorMsg);
				        	}
				        	else
						{
							CcspTraceError(("%s : Execution failed Error Code :%hu Reason: %s \n",__FUNCTION__,execReturn->ErrorCode,execReturn->ErrorMsg));
							updateVersionAndState(exec_data->version,execReturn->ErrorCode,blobDataProcessing);
							send_NACK(exec_data->subdoc_name,queueData.txid_queue[queueData.front],exec_data->version,execReturn->ErrorCode,execReturn->ErrorMsg);
			                if ( (exec_data->rollbackFunc) && ( VALIDATION_FALIED != execReturn->ErrorCode ) )
                                               		{	
								rollbkRet = exec_data->rollbackFunc();

								if (rollbkRet != ROLLBACK_SUCCESS )
			            				CcspTraceError(("%s : Rolling back of data failed\n",__FUNCTION__));
							}

						}

		        		}
		        	}	

			        else
			        {
			        	CcspTraceError(("%s: execute_request pthread_create failed , ERROR : %s \n", __FUNCTION__,strerror(errno)));

				        execReturn = exec_data->executeBlobRequest(exec_data->user_data);

				        if ( BLOB_EXEC_SUCCESS == execReturn->ErrorCode )
				        {
						CcspTraceInfo(("%s : Execution success , sending completed ACK\n",__FUNCTION__));
						updateVersionAndState(exec_data->version,execReturn->ErrorCode,blobDataProcessing);
						send_ACK(exec_data->subdoc_name,queueData.txid_queue[queueData.front],exec_data->version,0,execReturn->ErrorMsg);
				        }
				        else
					{

						CcspTraceError(("%s : Execution failed Error Code :%hu Reason: %s \n",__FUNCTION__,execReturn->ErrorCode,execReturn->ErrorMsg));
						updateVersionAndState(exec_data->version,execReturn->ErrorCode,blobDataProcessing);
						send_NACK(exec_data->subdoc_name,queueData.txid_queue[queueData.front],exec_data->version,execReturn->ErrorCode,execReturn->ErrorMsg);
                        if ( (exec_data->rollbackFunc) && ( VALIDATION_FALIED != execReturn->ErrorCode ) )
                                       		{
							rollbkRet = exec_data->rollbackFunc();

							if (rollbkRet != ROLLBACK_SUCCESS )
			            				CcspTraceError(("%s : Rolling back of data failed\n",__FUNCTION__));
						}
					}
				}

			}
		    	else
			{	
				CcspTraceError(("%s executeBlobRequest function pointer is NULL , Send NACK\n",__FUNCTION__));
				updateVersionAndState(exec_data->version,execReturn->ErrorCode,blobDataProcessing);
				send_NACK(exec_data->subdoc_name,queueData.txid_queue[queueData.front],exec_data->version,NULL_BLOB_EXEC_POINTER,"Null Execution Pointer passed");

			}


				if ( exec_data->freeResources ) 
					exec_data->freeResources(queueData.execDataPointerAddr[queueData.front]);

				if ( execReturn != NULL)
				{
					free(execReturn);
					execReturn = NULL;
				}

				removeEntryFromQueue();

		    	pthread_mutex_unlock(&webconfig_exec);
	 
	    	}    	
   	}
	return NULL;
}

/* Component call this api in boot up with message queue name , this function creats messageQueueProcessing thread to process blob requets */
void initMessageQueue()

{
	CcspTraceInfo(("Inside FUNC %s LINE %d \n",__FUNCTION__,__LINE__));

	pthread_t tid;

    	FILE  *pFile      = NULL;

    	char command[32]= {0};
    	snprintf(command,sizeof(command),"cat /proc/%d/comm",getpid());

    	pFile = popen(command, "r");
    	if(pFile)
    	{
        	fgets(process_name, sizeof(process_name), pFile);
        	pclose(pFile);
        	pFile = NULL ;
        	char* pos;
        	if(process_name[0] != '\0')
        	{
           		if ( ( pos = strchr( process_name, '\n' ) ) != NULL ) {
               	 	*pos = '\0';
              		}
        	}

     	}

     	snprintf(mqEventName,sizeof(mqEventName), "%s-%s",WEBCONFIG_QUEUE_NAME,process_name);

	int ret = pthread_create(&tid, NULL, &messageQueueProcessing, NULL); 

	if ( ret != 0 )
		CcspTraceError(("%s: messageQueueProcessing pthread_create failed , ERROR : %s \n", __FUNCTION__,strerror(errno)));

}

/* Function returns if version execution is in queue , if version in queue already then just need to update TX ID . */

int checkIfVersionExecInQueue(uint32_t version,int *queueIndex)
{
	pthread_mutex_lock(&queue_access);

  	if (isQueueEmpty())
  	{
  		CcspTraceError(("%s : Queue Empty\n",__FUNCTION__));
		pthread_mutex_unlock(&queue_access);

  	  	return VERSION_UPDATE_REQUIRED ; 

  	}

	int i ;

    	for( i = queueData.front; i!=queueData.rear; i=(i+1)%QUEUE_SIZE) 
    	{

		if ( version == queueData.version_queue[i] )
		{
	        	*queueIndex = i;
	        	pthread_mutex_unlock(&queue_access);

	        	return EXECUTION_IN_QUEUE ;
	   	 }	
    	}


	if ( version == queueData.version_queue[i] )
	{
	    	*queueIndex = i;
	     	pthread_mutex_unlock(&queue_access);

		return EXECUTION_IN_QUEUE ;
	}

	pthread_mutex_unlock(&queue_access);

    return VERSION_UPDATE_REQUIRED; 
}

/* Function to check if version update is required, if version is already exist or request in queue then no need to push the request to message queue */

int checkNewVersionUpdateRequired(char *subdoc_name,uint32_t version,int *queueIndex)
{
	CcspTraceInfo(("Inside FUNC %s LINE %d \n",__FUNCTION__,__LINE__));
	int i =0 ;

    	pthread_mutex_lock(&reg_subdoc);

	PblobRegInfo blobCheckVersion;;

	blobCheckVersion = blobData;

	for (i=0 ; i < gNumOfSubdocs ; i++)
	{
	        if ( strcmp(blobCheckVersion->subdoc_name,subdoc_name) == 0)
	        {
			if ( (uint32_t)blobCheckVersion->version == version )
	        	{
	        		pthread_mutex_unlock(&reg_subdoc);

	        		return VERSION_ALREADY_EXIST;
	        	}

	        	else 
	        	{	 
	        		pthread_mutex_unlock(&reg_subdoc);

	        		return (checkIfVersionExecInQueue(version,queueIndex) );

	        	}

	        }

	     blobCheckVersion++;   
	}

	pthread_mutex_unlock(&reg_subdoc);

	return SUBDOC_NOT_SUPPORTED;
}

/* Function to calculate queue timeout , need to add queuetimeout to subdoc execution time when requests are in queue */
size_t getPendingQueueTimeout(uint16_t txid)
{
	int queueTimeout = 0;

	pthread_mutex_lock(&queue_access);

  	if (isQueueEmpty())
  	{
  		CcspTraceInfo(("%s : Queue Empty\n",__FUNCTION__));
		pthread_mutex_unlock(&queue_access);

  	  	return queueTimeout ; 

  	}
	int i ;
	for( i = queueData.front; i!=queueData.rear; i=(i+1)%QUEUE_SIZE) 
   	{

    		if ( queueData.txid_queue[i] == txid )
    		{
			pthread_mutex_unlock(&queue_access);
    			return queueTimeout;
    		} 

        	queueTimeout += queueData.timeout_queue[i];
    	}

    	if ( queueData.txid_queue[i] == txid )
    	{
		pthread_mutex_unlock(&queue_access);
    		return queueTimeout;
    	}
	queueTimeout += queueData.timeout_queue[i];

	pthread_mutex_unlock(&queue_access);

    return queueTimeout; 
}

/* Function to decide and push the blob execution request to message queue */
void PushBlobRequest (execData *exec_data )  
{

	CcspTraceInfo(("%s : subdoc_name %s , txid %hu, version %u , entries %lu\n",__FUNCTION__,exec_data->subdoc_name,exec_data->txid,exec_data->version,exec_data->numOfEntries));

	unsigned long timeout = 0;
	unsigned long timeout_to_webconfig = 0 ;
    	mqd_t mq;


    	/* open the message queue */


    	int queueIndex=0;

	int retVal =  checkNewVersionUpdateRequired(exec_data->subdoc_name,exec_data->version,&queueIndex);

	if ( VERSION_UPDATE_REQUIRED == retVal ) 
	{
		CcspTraceInfo(("New version available , prcessing new Blob request\n"));
    		mq = mq_open(mqEventName, O_WRONLY);
    		//CHECK((mqd_t)-1 != mq);
        	if ( (mqd_t)-1 == mq )
    		{
    			CcspTraceError(("%s message queue open failed , ERROR : %s\n",__FUNCTION__,strerror(errno)));
			send_NACK(exec_data->subdoc_name,exec_data->txid,exec_data->version,MQUEUE_OPEN_FAILED,"MQ OPEN FAILED");  
	  		
	  		if ( exec_data->freeResources ) 
	  			exec_data->freeResources(exec_data);

	  		return ;

    		}

		if (!exec_data->calcTimeout)
		{
			CcspTraceInfo(("calculateTimeout is NULL , using default timeout routine\n"));
			exec_data->calcTimeout = defFunc_calculateTimeout;
		}
		
			timeout = exec_data->calcTimeout(exec_data->numOfEntries);
			CcspTraceInfo(("%s timeout received from calcTimeout is %lu\n",__FUNCTION__,timeout));

			timeout_to_webconfig = timeout + (getPendingQueueTimeout(exec_data->txid));

		if (! (addEntryToQueue(exec_data->version,exec_data->txid,timeout,exec_data)) )
		{

			#if 0
			if (mq_getattr(mq, &attr) == 0)
		        {

				if ( attr.mq_curmsgs > 0 )
				timeout = (unsigned int) (timeout * attr.mq_curmsgs) ;

				CcspTraceInfo(("Num of events in queue are %lu\n",attr.mq_curmsgs));
	     		}

			#endif		
	
			CcspTraceInfo(("%s : Send received request ACK , timeout is %lu\n",__FUNCTION__,timeout_to_webconfig));

			send_ACK(exec_data->subdoc_name,exec_data->txid,exec_data->version,timeout_to_webconfig,"");
			
            		if ( 0 != mq_send(mq, (char*) exec_data, sizeof(*exec_data), 0))
                        {
                 		CcspTraceError(("%s message queue send failed , ERROR is : %s\n",__FUNCTION__,strerror(errno)));
                		send_NACK(exec_data->subdoc_name,exec_data->txid,exec_data->version,MQUEUE_SEND_FAILED,"MQ SEND FAILED");
                 		removeEntryfromRearEnd();

			}


			if ((0 != mq_close(mq)))
            			CcspTraceError(("%s message queue close failed , ERROR is : %s\n",__FUNCTION__,strerror(errno)));

            		return ;

		}

		else
		{
			CcspTraceError(("%s QUEUE FULL\n",__FUNCTION__));

			send_NACK(exec_data->subdoc_name,exec_data->txid,exec_data->version,QUEUE_PUSH_FAILED,"Queue is Full");			
		}

		if ((0 != mq_close(mq)))
            		CcspTraceError(("%s message queue close failed , ERROR is : %s\n",__FUNCTION__,strerror(errno)));


	}
	else if ( EXECUTION_IN_QUEUE == retVal )
	{
		CcspTraceInfo((" %s : Execution is in progress, updating transaction id\n",__FUNCTION__));
		pthread_mutex_lock(&queue_access);

		queueData.txid_queue[queueIndex] = exec_data->txid;
		pthread_mutex_unlock(&queue_access);

		
		if (!exec_data->calcTimeout)
		{
			CcspTraceInfo(("calculateTimeout is NULL , using default timeout routine\n"));
			exec_data->calcTimeout = defFunc_calculateTimeout;
		}
		
		timeout = exec_data->calcTimeout(exec_data->numOfEntries);

		CcspTraceInfo(("%s timeout received from calcTimeout is %lu\n",__FUNCTION__,timeout));

	   	timeout_to_webconfig = timeout + (getPendingQueueTimeout(exec_data->txid));

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

	if ( exec_data->freeResources ) 
		exec_data->freeResources(exec_data);
}
