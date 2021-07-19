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

#include <pthread.h>
#include <ccsp_message_bus.h>
#include <ccsp_base_api.h>
#include "ccsp_trace.h"
#include "ansc_platform.h"
#include "safec_lib_common.h"

#ifndef _GNU_SOURCE
    #define _GNU_SOURCE
#endif
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/user.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/queue.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <signal.h>
#include <malloc.h>
#include <sys/mman.h>
#include <sys/time.h>

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <sys/un.h>
#include <netdb.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/poll.h>
#include <sys/time.h>

#define SHM_FILE "/var/ccsp_shm_file"

#ifndef PAGE_SIZE
    #define PAGE_SIZE ((getpagesize()))
#endif

static char * CreateShm(int size)
{
    int		shmid;
    char	 *shmptr;
    key_t       key;
    FILE        * fp = NULL;
    
    fp = fopen(SHM_FILE, "w+");
    if (fp)
        fclose(fp);
    
    key = ftok(SHM_FILE, 1);
    
    if (key  ==-1)
        return NULL;
    
    if ((shmid = shmget(key, size, IPC_CREAT |IPC_EXCL |0600)) < 0)
    {
        if (errno == EEXIST)
        {
            CcspTraceError(("receiver didn't remove this SHM segment? has to remove it!\n"));
            if ((shmid = shmget(key, 1,  0600)) >0)/*to get an existing shmid, must use a smaller size*/
            {
                if (shmctl(shmid, IPC_RMID, 0) < 0)
                {
                    perror("shmctl");
                    return NULL;
                }
                if ((shmid = shmget(key, size, IPC_CREAT |0600)) < 0)
                {
                    perror("shmget1");
                    return NULL;
                }
            }
            else
            {
                perror("shmget");            
                return NULL;
            }
        }
        else
            return NULL;
    }

    if ((shmptr = shmat(shmid, 0, 0)) == (void *)-1)
    {
        perror("shmat");
        return NULL;
    }
 
    CcspTraceInfo(("CreateShm success!\n"));
    
    return shmptr;

}

/*
 *  Processor word boundary access -- some processors only allow writing on 4-byte word boundary, 
 *  this function is used to adjust the pointer address
 */
static inline int _get_align_num(int num)
{
    int tmp = num%4;
        
    if (tmp == 0)
        return num;
    else if (tmp == 1)
        return num+3;
    else if (tmp == 2 )
        return num+2;
    else
        return num+1;
}

#define EXTRA_ROOM PAGE_SIZE*2
static int GetParamValueArrSize(parameterValStruct_t ** val, int size)
{
    int ret = 0;
    int i;

    if (!val)
        return EXTRA_ROOM;

    for (i=0; i<size; i++)
    {
        if (val[i]->parameterName)
            ret += _get_align_num(strlen(val[i]->parameterName) + 1);
        else
            ret += 4;
        
        if (val[i]->parameterValue)
            ret += _get_align_num(strlen(val[i]->parameterValue) + 1);
        else
            ret += 4;

        ret += sizeof(val[i]->type);
    }

    return ret + EXTRA_ROOM;
}


static char * GetShm(int * pShmid, int shmSize)
{
	char	*shmptr;
    key_t key;

    key = ftok(SHM_FILE, 1);

    if (key  ==-1)
    {
        return NULL;
    }

    if ((*pShmid = shmget(key, shmSize, 0600)) < 0)
    {
        return NULL;
    }

    if ((shmptr = shmat(*pShmid, 0, 0)) == (void *)-1)
    {
        return NULL;
    }

    return shmptr;
}

static char * _CloneString(char * src, CCSP_MESSAGE_BUS_MALLOC mallocfunc)
{
    char * str = NULL;
    
    if (!src || !mallocfunc)
        return NULL;

    str = mallocfunc(strlen(src)+1);
    strcpy(str, src);
    return str;
}

static int ParseShm(char * shmPtr, parameterValStruct_t *** pValStructs, int * pCount, int shmid, CCSP_MESSAGE_BUS_INFO * bus_info) 
{
    char * p = shmPtr;
    int size = 0, i = 0, type = 0;

    /*shared memory map: parameterValStruct_t array size(4B) + parameterValStruct_t array (type, name, value) content*/
    size = *(int *)p;
    p += sizeof(int);
    CcspTraceInfo(("%s size %d\n", __FUNCTION__, size));

#if 1
    *pCount = size;
    *pValStructs = bus_info->mallocfunc(size * sizeof(parameterValStruct_t *));
    if (!(*pValStructs)) /*RDKB-6232, CID-33345, validate malloc*/
    {
        CcspTraceError(("can't malloc\n"));
        return -1;
    }

    for (i=0; i<size; i++)
    {
        type = *(int *)p;
        p += sizeof(int);

        (*pValStructs)[i] = bus_info->mallocfunc(sizeof(parameterValStruct_t));
        /*the error handling is tiring for point ***, omit it here.*/
        if (!(*pValStructs)[i])
        {
            CcspTraceError(("%s out of memory\n", __FUNCTION__));
            return -1;
        }
            
        (*pValStructs)[i]->parameterName = _CloneString(p, bus_info->mallocfunc);
        p += _get_align_num(strlen(p) +1);

        (*pValStructs)[i]->parameterValue = _CloneString(p, bus_info->mallocfunc); 
        p += _get_align_num(strlen(p) +1);            

        (*pValStructs)[i]->type = type;
    }

#if 0
    {
        FILE * fp = fopen("./shm_out", "w+");
        if (fp)
        {
            for (i=0; i<*pCount; i++)
                fprintf(fp, "name:value %s:%s\n", (*pValStructs)[i]->parameterName, (*pValStructs)[i]->parameterValue ? (*pValStructs)[i]->parameterValue:"");
            fclose(fp);
        }
    }
#endif

    if (shmdt(shmPtr))
        perror("shmdt");
    
    if (shmctl(shmid, IPC_RMID, 0) < 0)
        perror("shmctl");

#endif

    return 0;
}

int CcspBaseIf_getParameterValues_Shm (
    CCSP_MESSAGE_BUS_INFO * bus_info,
    int shmSize,
    int *val_size,
    parameterValStruct_t ***parameterval
)
{
    int ret = CCSP_SUCCESS;
    char * shmPtr = NULL;
    int shmid = 0;

    shmPtr = GetShm(&shmid, shmSize);
    if (!shmPtr)
        return CCSP_FAILURE;

    ret = ParseShm(shmPtr, parameterval , val_size, shmid, bus_info);

    if (ret)
        return CCSP_FAILURE;
    else
        return CCSP_SUCCESS;
}

int CcspBaseIf_base_path_message_write_shm (
    void* bus_handle,
    int size,
    parameterValStruct_t **val,
    int * shmSize
    )
{
    UNREFERENCED_PARAMETER(bus_handle);
    char * shmPtr = NULL;
    char * p;
    int totalSize = 0;
    int i = 0;
    errno_t rc = -1;

    totalSize = GetParamValueArrSize(val, size);
    if ( (shmPtr = CreateShm(totalSize)) == NULL)
        goto done;

    *shmSize = totalSize;
    
    p = shmPtr;
    *(int *)p = size;
    p += sizeof(int);

    for(i = 0; i < size; i++)
    {
        *(int *)p = val[i]->type;
        p += sizeof(int);

        if (val[i]->parameterName)
        {
            rc = strcpy_s(p, (unsigned int)totalSize, val[i]->parameterName);
            ERR_CHK(rc);
            p += _get_align_num(strlen(val[i]->parameterName) +1);
        }
        else
            p += 4;
            
        if (val[i]->parameterValue)
        {
            rc = strcpy_s(p, (unsigned int)totalSize, val[i]->parameterValue);
            ERR_CHK(rc);
            p += _get_align_num(strlen(val[i]->parameterValue) +1);
        }
        else
            p += 4;

    }

    if (shmdt(shmPtr))
        perror("shmdt");
    
done:
    return 0;
}
