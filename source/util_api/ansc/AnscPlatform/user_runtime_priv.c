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


/**********************************************************************

    module:	user_runtime.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2002
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file implements the commonly used C runtime
        library functions.

        *   _ansc_system

    ---------------------------------------------------------------

    environment:

        VxWorks 5.4

    ---------------------------------------------------------------

    author:

        Huaxiang Sun
        Hua Ding

    ---------------------------------------------------------------

    revision:

        04/25/02    initial revision.

**********************************************************************/

#include "ansc_global.h"
#include "user_base.h"
#include "user_runtime.h"
#include "user_debug.h"
#include "user_string.h"

#include <signal.h>
#include <dirent.h>
#include <stdlib.h>

#define  KILL_BUF_SIZE               128

static int kill_by_ppid(int ppid)
{
    DIR *dir;
    struct dirent *next;
    unsigned long tmp_Pid=0;
    unsigned long tmp_PPid=0;
    int i=0;
    FILE *status;
    char filename[KILL_BUF_SIZE];
    char garbage[KILL_BUF_SIZE];

    dir = opendir("/proc");
    if (!dir) {
#if 0
            printf("kill_by_ppid: Cannot open /proc.\n");
#endif
            return -1;
    }

    while ((next = readdir(dir)) != NULL) {
        memset(filename, 0, sizeof(filename));
        memset(garbage, 0, sizeof(garbage));

        if ((strcmp(next->d_name, "..") == 0)||!isdigit(*next->d_name))
                continue;

        sprintf(filename, "/proc/%s/status", next->d_name);
        if (!(status = fopen(filename, "r")) )
                continue;

        /* skip the these lines */
        for (i=0; i<4; i++){
            fgets(garbage, KILL_BUF_SIZE, status);
        }

        if ( 1 != fscanf(status, "Pid: %lu\n", &tmp_Pid) ||
             1 != fscanf(status, "PPid: %lu\n", &tmp_PPid)){
            fclose(status);
#if 0
            printf("kill_by_ppid failed to parse %s.\n", filename);
#endif
            continue;
        }

        fclose(status);

        if ((tmp_PPid == ppid) && (kill(tmp_Pid, SIGTERM) != -1)){
#if 0
            printf("killed process %d.\n", tmp_Pid);
#endif
            closedir(dir);
            return 0;
        }
    }

    closedir(dir);
    return -1;
}


int _ansc_system(char *command)
{
    return system(command);
}
