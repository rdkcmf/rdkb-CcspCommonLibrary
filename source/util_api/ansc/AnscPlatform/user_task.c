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

#include "ansc_platform.h"

static ULONG gTaskNum = 0;

void *
UserCreateTask
    (
        void *		                routine,
        ULONG		                stackSize,
        int                         priority,
        void *                      context,
	    void *                      name
    )
{
    UNREFERENCED_PARAMETER(name);
    UNREFERENCED_PARAMETER(priority);
    pthread_t           tid;
    int rc;
    pthread_attr_t      attr;

    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, stackSize);
    rc = pthread_create(&tid, &attr /*NULL*/, (void* (*)(void *))routine, context);
    pthread_attr_destroy(&attr);
    if(rc)                      /* could not create thread */
    {
//        AnscTrace("ERROR: pthread_create for %s failed with error %d \n", (char *)name, rc);
        return NULL;
    }

    gTaskNum++;
//    AnscTrace("Task: #%d, %s-%lu-%p Created!\n", gTaskNum, (char *)name, tid, pTaskRecord);

    return (void *)tid;
}

int
UserDestroyTask
    (
        void *      context
    )
{
    //PANSC_TASK_RECORD   pTaskRecord  = (PANSC_TASK_RECORD)context;
    pthread_t           tid = (pthread_t)context;

    gTaskNum--;
#if 0
    printf("# Task: %s (tid = %lu) Delete! total %d Tasks left\n", pTaskRecord->Name, pTaskRecord->Handle, gTaskNum);
#endif

     pthread_detach(tid);

    /* do not use pthread_cancel within thread context, it causes memory leaks and other issues on Linux */
    /*
    pthread_cancel(tid);
    */

    return 0;
}

